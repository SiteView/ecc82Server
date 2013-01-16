/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <boost/lexical_cast.hpp>

#include "WApplication"
#include "WContainerWidget"
#include "WWebWidget"
#include "WStringUtil"

#include "DomElement.h"
#include "WebController.h"
#include "Configuration.h"
#include "WebRenderer.h"
#include "WebRequest.h"
#include "WebSession.h"
#include "FileServe.h"
#include "WtRandom.h"
#include "EscapeOStream.h"
#ifdef WIN32
#include <process.h> // for getpid()
#endif

namespace skeletons {
  extern const char *Boot_html;
  extern const char *Ajax_html;
  extern const char *Plain_html;
  extern const char *JsNoAjax_html;
}

namespace Wt {

const int MESSAGE_COUNTER_SIZE = 5;

/*
 * To add multiple windows: we need to manage an update map per window.
 * On every request we may need to do something for every window. Even
 * if for example the request is a reload of one particular window. Also,
 * how do we get the changes to the right window ? Should client-side the
 * window also let the other windows update ? No, we must be able to tunnel
 * pieces of JavaScript code on the client to the right window. This must
 * be possible, right ?
 *
 * Thus, perhaps it is still the easiest thing to do to have one updatemap
 * but when we collect JS, we collect it into separate buffers. And we change
 * the update protocol -- or rather we make sure the JavaScript we evaluate
 * does in fact everything we need (tunneling JavaScript code to the correct
 * window):
 *
 * evalWindow("wId", "hide(...); bla(....);"); 
 */
WebRenderer::WebRenderer(WebSession& session)
  : session_(session),
    visibleOnly_(true),
    twoPhaseThreshold_(5000),
    learning_(false)
{
  charEncoding_ = "UTF-8";
}

void WebRenderer::setTwoPhaseThreshold(int bytes)
{
  twoPhaseThreshold_ = bytes;
}

void WebRenderer::needUpdate(WWebWidget *w)
{
  if (session_.env().ajax()) {
    UpdateMap& usedMap = updateMap_;
    //std::cerr << "needUpdate: " << w->formName() << std::endl;

    usedMap[w->formName()] = w;
  }
}

void WebRenderer::doneUpdate(WWebWidget *w)
{
  //std::cerr << "doneUpdate: " << w->formName() << std::endl;

  UpdateMap& usedMap = updateMap_;

  UpdateMap::iterator i = usedMap.find(w->formName());
  if (i != usedMap.end())
    usedMap.erase(i);
}

const std::vector<WObject *>& WebRenderer::formObjects() const
{
  return currentFormObjects_;
}

void WebRenderer::saveChanges()
{
  collectJS(session_.app()->domRoot()->webWidget(), &collectedChanges_);
}

void WebRenderer::discardChanges()
{
  collectJS(session_.app()->domRoot()->webWidget(), 0);
}

void WebRenderer::letReload(WebRequest& request, bool parentFrame)
{
  serveJavascriptUpdate(request,
			parentFrame
			? "window.parent.document.location.reload(true);"
			: "document.location.reload(true);");
}

void WebRenderer::serveMainWidget(WebRequest& request,
				  ResponseType responseType)
{
  switch (responseType) {
  case ResponseEval:
    serveJavascriptUpdate(request, session_.app()->domRoot_);
    break;
  case ResponseHTML:
    serveMainpage(request, session_.app()->domRoot_);
    break;
  }
}

void WebRenderer::serveBootstrap(WebRequest& request)
{
  FileServe boot(skeletons::Boot_html);
  boot.setVar("RELATIVE_URL", session_.relativeUrl());
  boot.setVar("RANDOMSEED",
	      boost::lexical_cast<std::string>(lrand48() + getpid()));
  boot.setVar("RELOAD_IS_NEWSESSION",
	      WebController::conf().reloadIsNewSession()
	      ? "true" : "false");
  boot.setVar("USE_COOKIES",
	      WebController::conf().sessionTracking()
	      == Configuration::CookiesURL
	      ? "true" : "false");
  setHeaders(request, "text/html");
  boot.stream(request.out());
}

void WebRenderer::serveError(WebRequest& request, const std::exception& e,
			     ResponseType responseType)
{
  serveError(request, std::string(e.what()), responseType);
}

void WebRenderer::serveError(WebRequest& request, const std::string message,
			     ResponseType responseType)
{
  if (responseType == ResponseHTML) {
    request.setContentType("text/html");
    request.out()
      << "<title>Error occurred.</title>"
      << (session_.env().inFrame()
	  ? "<script>window.parent.document.title='Error occured.';</script>"
	  : "")
      << "<h2>Error occurred.</h2>"
      << toUTF8(WWebWidget::escapeText(widen(message), true))
      << std::endl;    
  } else {
    collectedChanges_ << "alert(";
    DomElement::jsStringLiteral(collectedChanges_,
				"Error occurred:\n" + message, '\'');
    collectedChanges_ << ");";
  }
}

void WebRenderer::setCookie(const std::string name, const std::string value,
			    int maxAge, const std::string domain,
			    const std::string path)
{
  cookiesToSet_.push_back(Cookie(name, value, path, domain, maxAge));
}

static void replace(std::string& s, char c, std::string r)
{
  std::string::size_type p = 0;

  while ((p = s.find(c, p)) != std::string::npos) {
    s.replace(p, 1, r);
    p += r.length();
  }
}

void WebRenderer::setHeaders(WebRequest& request, const std::string mimeType)
{
  std::string cookies;

  for (unsigned i = 0; i < cookiesToSet_.size(); ++i) {
    std::string value = cookiesToSet_[i].value;
    replace(value, '"', "\"");

    cookies += cookiesToSet_[i].name + "=\"" + value + "\";Version=\"1\";";
    if (cookiesToSet_[i].maxAge != -1)
      cookies += "Max-Age=\""
	+ boost::lexical_cast<std::string>(cookiesToSet_[i].maxAge) + "\";";
    if (!cookiesToSet_[i].domain.empty())
      cookies += "Domain=\"" + cookiesToSet_[i].domain + "\";";
    if (!cookiesToSet_[i].path.empty())
      cookies += "Path=\"" + cookiesToSet_[i].path + "\";";
  }
  cookiesToSet_.clear();

  if (!cookies.empty())
    request.addHeader("Set-Cookie", cookies);

  request.setContentType(mimeType);
}

void WebRenderer::serveJavascriptUpdate(WebRequest& request,
					const std::string js)
{
  if (session_.debug())
      std::cerr << "Responding: " << std::endl
		<< ">>>>>>>" << std::endl
		<< js << std::endl
		<< "<<<<<<<" << std::endl;

  std::string contentType = "text/plain";
  if (!charEncoding_.empty())
    contentType += "; charset=" + charEncoding_;

  setHeaders(request, contentType);
  request.out() << js << "\r\n";
}

void WebRenderer::serveJavascriptUpdate(WebRequest& request,
					WWidget *mainWidget)
{
  std::string redirect = session_.getRedirect();

  if (!redirect.empty()) {
    std::string js = "if(document.images) window.";
    if (session_.env().inFrame())
      js += "parent.";
    js += "location.replace('" + redirect + "'); else window.";
    if (session_.env().inFrame())
      js += "parent.";
    js += "location.href='" + redirect + "'";
    serveJavascriptUpdate(request, js);
    return;
  }

  std::string contentType = "text/plain";
  if (!charEncoding_.empty())
    contentType += "; charset=" + charEncoding_;

  setHeaders(request, contentType);
  
  if (!session_.app()->isQuited())
    visibleOnly_ = true;

  request.out() << collectedChanges_.str();
  collectedChanges_.str("");

  collectJavascriptUpdate(mainWidget, request.out());

  if (!session_.app()->isQuited()) {
    visibleOnly_ = false;

    collectJavascriptUpdate(mainWidget, collectedChanges_);

    if (collectedChanges_.rdbuf()->in_avail()
	< (int)twoPhaseThreshold_) {
      request.out() << collectedChanges_.str();
      collectedChanges_.str("");
    } else {
      request.out() << "update(null, 'none', null, false);";
    }
  }
}

void WebRenderer::serveMainpage(WebRequest& request, WWidget *topWidget)
{
  std::string redirect = session_.getRedirect();

  if (!redirect.empty()) {
    request.setRedirect(redirect);
    return;
  }

  WWebWidget *mainWebWidget = topWidget->webWidget();
  WApplication *app = session_.app();

  mainWebWidget->propagateRenderOk();

  visibleOnly_ = true;

  if (session_.env().ajax()) {
    preLearnStateless();
    collectedChanges_.str("");
  }
  /*
   * The element to render. This automatically creates loading stubs for
   * invisible widgets, which is excellent for both javascript and
   * non-javascript versions.
   */
  DomElement *mainElement = mainWebWidget->createSDomElement();

  /*
   * The form objects
   */
  std::vector<WObject *> formObjects;
  mainWebWidget->getFormObjects(formObjects);
  std::string formObjectsVar = createFormsVar(formObjects);

  currentFormObjectsVar_ = formObjectsVar;
  currentFormObjects_ = formObjects;

  std::string styleSheets;
  for (unsigned i = 0; i < app->styleSheets_.size(); ++i)
    styleSheets += "<link href='" + app->styleSheets_[i]
      + "' rel='stylesheet' type='text/css' />";

  FileServe page(session_.env().ajax() ? skeletons::Ajax_html :
		 session_.env().javaScript() ? skeletons::JsNoAjax_html :
		 skeletons::Plain_html);
  page.setVar("RELATIVE_URL", session_.relativeUrl());
  page.setVar("TITLE", toUTF8(app->title().value()));
  page.setVar("FORMOBJECTS", formObjectsVar);
  page.setVar("KEEPALIVE",
	      boost::lexical_cast<std::string>
	      (WebController::conf().sessionTimeout() / 2));
  page.setVar("STYLESHEET", app->styleSheet().cssText());
  page.setVar("STYLESHEETS", styleSheets);
  page.setVar("RANDOMSEED",
	      boost::lexical_cast<std::string>(lrand48() + getpid()));
  page.setVar("MESSAGE_COUNTER_SIZE", 
	      boost::lexical_cast<std::string>(MESSAGE_COUNTER_SIZE));
  page.setVar("INFRAME", session_.env().inFrame() ? "true" : "false");

  std::string states;
  for (WApplication::StateMap::const_iterator i = app->state_.begin();
       i != app->state_.end(); ++i) {
    states += "historyStates['" + i->first + "']='" + i->second + "';";
  }
  page.setVar("STATES_INIT", states);

  session_.app()->titleChanged_ = false;
  session_.app()->statesChanged_.clear();

  std::string contentType = "text/html";
  if (!charEncoding_.empty())
    contentType += "; charset=" + charEncoding_;

  setHeaders(request, contentType);
  page.stream(request.out(), "HTML");

  DomElement::TimeoutList timeouts;
  EscapeOStream out(request.out());
  mainElement->asHTML(out, timeouts);
  delete mainElement;

  std::stringstream onload;
  {
    EscapeOStream sonload(onload);
    DomElement::createTimeoutJs(sonload, timeouts);
  }

  page.setVar("ONLOAD", onload.str());

  int refresh = WebController::conf().sessionTimeout() / 3;
  for (unsigned i = 0; i < timeouts.size(); ++i)
    refresh = std::min(refresh, 1 + timeouts[i].first/1000);
  if (session_.app()->isQuited())
    refresh = 100000; // ridiculously large
  page.setVar("REFRESH", boost::lexical_cast<std::string>(refresh));

  page.stream(request.out());

  visibleOnly_ = false;
}

void WebRenderer::collectChanges(WWebWidget *root,
				 std::vector<DomElement *>& changes)
{
  std::multimap<int, std::string> depthOrder;

  UpdateMap& usedMap = updateMap_;

  for (UpdateMap::const_iterator i = usedMap.begin();
       i != usedMap.end(); ++i) {
    int depth = 1;

    WWidget *father = i->second;
    for (; father->parent(); father = father->parent(), ++depth) {
      if (father->webWidget() == root)
	break;
    }

    if (root != father->webWidget()) {
      std::cerr << "ignoring: " << i->second->formName()
      		<< " " << father->formName() << std::endl;
      continue; /* not in this root */
    }

    depthOrder.insert(std::make_pair(depth, i->first));
  }

  for (std::multimap<int, std::string>::const_iterator i = depthOrder.begin();
       i != depthOrder.end(); ++i) {
    UpdateMap::iterator j = usedMap.find(i->second);
    if (j != usedMap.end()) {
      WWebWidget *w = j->second;


      //std::cerr << learning_ << " " << loading_ 
      //          << " updating: " << w->formName() << std::endl;

      if (!learning_ && visibleOnly_) {
	if (!w->isStubbed()) {
	  if (!w->isVisible()) {
	    // We should postpone rendering the changes -- but
	    // at the same time need to propageRenderOk() now for stateless
	    // slot learning to work properly.
	    w->getSDomChanges(changes);
	  } else
	    w->getSDomChanges(changes);
	} else
	  ; //std::cerr << "Ignoring: " << w->formName() << std::endl;
      } else
	w->getSDomChanges(changes);
    }
  }
}

void WebRenderer::collectJavascriptUpdate(WWidget *topWidget,
					  std::ostream& out)
{
  WWebWidget *mainWebWidget = topWidget->webWidget();

  if (&out != &collectedChanges_) {
    out << collectedChanges_.str();
    collectedChanges_.str("");
  }

  collectJS(mainWebWidget, &out);

  /*
   * Now, as we have cleared and recorded all javascript changes that were
   * caused by the actual code, we can learn stateless code and collect
   * changes that result.
   */
  preLearnStateless();

  if (&out != &collectedChanges_) {
    out << collectedChanges_.str();
    collectedChanges_.str("");
  }

  collectJS(mainWebWidget, &out);

  std::vector<WObject *> formObjects;
  mainWebWidget->getFormObjects(formObjects);

  std::string formObjectsVar = createFormsVar(formObjects);

  if (formObjectsVar != currentFormObjectsVar_) {
    out << "formObjects = new Array(" << formObjectsVar << ");";
    currentFormObjectsVar_ = formObjectsVar;
    currentFormObjects_ = formObjects;
  }

  WApplication *app = session_.app();

  if (app->isQuited()) {
    out << "quited = true;";
    out << "clearTimeout(keepAliveTimer);";

    WContainerWidget *timers = app->timerRoot();
    DomElement *d = DomElement::getForUpdate(timers, DomElement::DIV);
    d->removeFromParent();
    EscapeOStream sout(out);
    d->asJavaScript(sout, DomElement::Delete);

    delete d;
  }
}

std::string
WebRenderer::createFormsVar(const std::vector<WObject *>& formObjects)
{
  std::string formObjectsVar;
  for (unsigned i = 0; i < formObjects.size(); ++i) {
    if (i != 0)
      formObjectsVar += ',';
    formObjectsVar += "'" + formObjects[i]->formName() + "'";
  }

  return formObjectsVar;
}

void WebRenderer::collectJS(WWebWidget *mainWebWidget, std::ostream* js)
{
  std::vector<DomElement *> changes;

  collectChanges(mainWebWidget, changes);
  if (js) {
    EscapeOStream sout(*js);

    for (unsigned i = 0; i < changes.size(); ++i)
      changes[i]->asJavaScript(sout, DomElement::Delete);

    for (unsigned i = 0; i < changes.size(); ++i)
      changes[i]->asJavaScript(sout, DomElement::Create);

    for (unsigned i = 0; i < changes.size(); ++i) {
      changes[i]->asJavaScript(sout, DomElement::Update);
      delete changes[i];
    }
  } else {
    for (unsigned i = 0; i < changes.size(); ++i)
      delete changes[i];
  }

  WApplication *app = session_.app();

  if (js) { 
    if (app->titleChanged_) {
      if (session_.env().inFrame())
	*js << "window.parent.";
      *js << "document.title=";
      DomElement::jsStringLiteral(*js, 
				  toUTF8(app->title().value()),
				  '\'');
      *js << ";";
    }

    for (std::set<std::string>::const_iterator i = app->statesChanged_.begin();
	 i != app->statesChanged_.end(); ++i) {
      std::string k = *i;
      std::string v = app->state_[k];

      *js << "historyChangeState(";
      DomElement::jsStringLiteral(*js, k, '\'');
      *js << ",";
      DomElement::jsStringLiteral(*js, v, '\'');
      *js << ");";
    }
  }

  app->titleChanged_ = false;
  app->statesChanged_.clear();
}

void WebRenderer::preLearnStateless()
{
  const WApplication::SignalMap& ss = session_.app()->exposedSignals();

  for (WApplication::SignalMap::const_iterator i = ss.begin();
       i != ss.end(); ++i) {
    WWidget *ww = dynamic_cast<WWidget *>(i->second->sender());

    if (!ww->isStubbed() && ww->adam() == session_.app()->domRoot())
      i->second->processPreLearnStateless(this);
  }
}

std::string WebRenderer::learn(WStatelessSlot* slot)
{
  WWebWidget *mainWebWidget = session_.app()->domRoot()->webWidget();
  collectJS(mainWebWidget, &collectedChanges_);

  if (slot->type() == WStatelessSlot::PreLearnStateless)
    learning_ = true;

  slot->trigger();

  std::stringstream js;
  collectJS(mainWebWidget, &js);

  std::string result = js.str();

  if (slot->type() == WStatelessSlot::PreLearnStateless) {
    slot->undoTrigger();
    collectJS(mainWebWidget, 0);

    learning_ = false;
  } else { // AutoLearnStateless
    collectedChanges_ << result;
  }

  slot->setJavaScript(result);

  return result;
}

}
