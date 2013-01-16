/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/tokenizer.hpp>

#include "WApplication"
#include "WContainerWidget"

#include "WebSession.h"
#include "DomElement.h"

#include "Configuration.h"
#include "WebController.h"

namespace Wt {

//#define WTDEBUG

class Iframe : public WWebWidget
{
public:
  Iframe(std::string uri, WContainerWidget *parent = 0)
    : WWebWidget(parent),
      used(false)
  { 
    if (uri.empty())
      uri_ = generateUrl();
    else
      uri_ = uri;
  }

  bool used;

  std::string uri() const { return uri_; }

private:
  std::string uri_;

  void updateDom(DomElement& element, bool all)
  {
    WWebWidget::updateDom(element, all);
  }

  DomElement *createDomElement()
  {
    DomElement *iframe = DomElement::createNew(DomElement::IFRAME);
    iframe->setProperty(Wt::PropertyStyleBorderLeft, "0px");
    iframe->setProperty(Wt::PropertyStyleBorderRight, "0px");
    iframe->setProperty(Wt::PropertyStyleBorderTop, "0px");
    iframe->setProperty(Wt::PropertyStyleBorderBottom, "0px");
    iframe->setProperty(Wt::PropertyStyleWidth, "0px");
    iframe->setProperty(Wt::PropertyStyleHeight, "0px");
    iframe->setAttribute("src", uri_);
    iframe->setId(this, true);

    updateDom(*iframe, true);

    return iframe;
  }

  void getDomChanges(std::vector<DomElement *>& result)
  {
    if (renderState() == RenderUpdate) {
      DomElement *e = DomElement::getForUpdate(this, DomElement::IFRAME);
      updateDom(*e, false);
      result.push_back(e);
    }
  }

  void resourceHTML(std::ostream&) {
  }
};

WApplication::WApplication(const WEnvironment& env)
  : session_(env.session_),
    titleChanged_(false),
    quited_(false)
{
  locale_ = environment().locale();
  session_->app_ = this;

  setCurrentHistoryKey(session_->hash_);

  domRoot_ = new WContainerWidget();
  domRoot_->load();

  timerRoot_ = new WContainerWidget(domRoot_);

  if (environment().javaScript())
    for (unsigned i = 0; i < 10; ++i)
      iframes_.push_back(new Iframe(i == 0
				    ? std::string()
				    : iframes_[0]->uri(), domRoot_));

  widgetRoot_ = new WContainerWidget(domRoot_);

  styleSheet_.addRule("TABLE", L"border-collapse: collapse; border: 0px");
  styleSheet_.addRule("DIV, TD, IMG", L"margin: 0px; padding: 0px;");
  styleSheet_.addRule("BUTTON.wrap", L"border: 0px; "
		      "background-color: transparent; "
		      "margin: 0px; padding: 0px; font-size: inherit; "
		      "pointer: hand; cursor: pointer; cursor: hand;");
}

WWidget *WApplication::useIframe()
{
  for (unsigned i = 0; i < iframes_.size(); ++i) {
    if (!iframes_[i]->used) {
      iframes_[i]->used = true;
      return iframes_[i];
    }
  }

  std::cerr << "running out of iframes..." << std::endl;
  return 0;
}

void WApplication::releaseIframe(WWidget *iframe)
{
  ((Iframe *)iframe)->used = false;
}

void WApplication::useStyleSheet(const std::string uri)
{
  styleSheets_.push_back(uri);
}

WApplication::~WApplication()
{   
  delete domRoot_;
}

const WEnvironment& WApplication::environment() const
{
  return session_->env();
}

void WApplication::setTitle(const WMessage& title)
{
  title_ = title;
  titleChanged_ = true;
}

std::string WApplication::url() const
{
  return session_->url();
}

void WApplication::quit()
{
  quited_ = true;
}

void WApplication::addExposedSignal(Wt::EventSignalBase *signal)
{
#ifdef WTDEBUG
  std::cerr << "WApplication::addExposedSignal: " << signal << std::endl;
#endif
  exposedSignals_[signal->encodeCmd()] = signal;
}

void WApplication::removeExposedSignal(Wt::EventSignalBase *signal)
{
  SignalMap::iterator i = exposedSignals_.find(signal->encodeCmd());

  if (i == exposedSignals_.end()) {
    // ignore: currently signals do not really keep track of being
    // exposed or not.
  } else {
#ifdef WTDEBUG
    std::cerr << " WApplication::removeExposedSignal." << std::endl;    
#endif
    exposedSignals_.erase(i);
  }
}

Wt::EventSignalBase *
WApplication::decodeExposedSignal(const std::string signalName) const
{
  SignalMap::const_iterator i = exposedSignals_.find(signalName);
  
  if (i != exposedSignals_.end())
    return i->second;
  else
    return 0;
}

bool WApplication::isExposedSignal(const EventSignalBase *signal) const
{
  return
    exposedSignals_.find(signal->encodeCmd()) != exposedSignals_.end();
}

const WApplication::SignalMap& WApplication::exposedSignals() const
{
  return exposedSignals_;
}

void WApplication::addExposedResource(WResource *resource)
{
  exposedResources_[resource->formName()] = resource;
}

void WApplication::removeExposedResource(WResource *resource)
{
  ResourceMap::iterator i = exposedResources_.find(resource->formName());

  if (i != exposedResources_.end())
    exposedResources_.erase(i);
}

WResource *WApplication::decodeExposedResource(const std::string resourceName) 
  const
{
  ResourceMap::const_iterator i = exposedResources_.find(resourceName);
  
  if (i != exposedResources_.end())
    return i->second;
  else
    return 0;
}

std::string WApplication::encodeObject(WObject *object)
{
  std::string result
    = "w" + boost::lexical_cast<std::string>(object->internalId());

  encodedObjects_[result] = object;

  return result;
}

WObject *WApplication::decodeObject(const std::string objectId) const
{
  ObjectMap::const_iterator i = encodedObjects_.find(objectId);

  if (i != encodedObjects_.end()) {
    return i->second;
  } else
    return 0;
}

void WApplication::setLocale(const std::string locale)
{
  locale_ = locale;
  refresh();
}

void WApplication::refresh()
{
  messageResourceBundle_.refresh();
  widgetRoot_->refresh();

  if (title_.refresh())
    titleChanged_ = true;
}

void WApplication::redirect(const std::string url)
{
  session_->redirect(url);
}

void WApplication::setTwoPhaseRenderingThreshold(int bytes)
{
  session_->renderer().setTwoPhaseThreshold(bytes);
}

void WApplication::setCookie(const std::string name, const std::string value,
			     int maxAge, const std::string domain,
			     const std::string path)
{
  session_->renderer().setCookie(name, value, maxAge, domain, path);
}

WApplication *WApplication::instance()
{
  return WebSession::instance()->app();
}

int WApplication::maximumRequestSize() const
{
  return WebController::conf().maxRequestSize() * 1024;
}

void WApplication::setState(const std::string scope, const std::string state)
{
  state_[scope] = state;
  statesChanged_.insert(scope);
}

std::string WApplication::state(const std::string scope) const
{
  StateMap::const_iterator i = state_.find(scope);

  if (i != state_.end())
    return i->second;
  else
    return "";
}

void WApplication::setCurrentHistoryKey(std::string stateStr)
{
  typedef boost::tokenizer<boost::char_separator<char> > semicolon_tok;
  semicolon_tok tok(stateStr, boost::char_separator<char>(";"));

  StateMap oldState = state_;
  state_.clear();

  for (semicolon_tok::iterator i = tok.begin(); i != tok.end(); ++i) {
    std::string pair = *i;

    std::string::size_type colonPos = pair.find(':');

    if (colonPos != std::string::npos) {
      std::string key = pair.substr(0, colonPos);
      std::string value  = pair.substr(colonPos + 1);
      state_[key] = value;

      if (oldState.find(key) == oldState.end())
	stateChanged.emit(key, value);
    }
  }

  for (StateMap::const_iterator i = oldState.begin();
       i != oldState.end(); ++i) {
    StateMap::const_iterator j = state_.find(i->first);

    if (j == state_.end())
      stateChanged.emit(i->first, "");
    else
      if (i->second != j->second)
	stateChanged.emit(i->first, j->second);
  }

  statesChanged_.clear();
}

}
