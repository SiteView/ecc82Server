/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <sstream>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#ifdef THREADED
#include <boost/bind.hpp>
#endif // THREADED

#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/util/XMLString.hpp>

#include "WApplication"
#include "WResource"
#include "WEvent"
#include "WStringUtil"

#include "Configuration.h"
#include "CgiParser.h"
#include "WebController.h"
#include "WebRequest.h"
#include "WebSession.h"
#include "WebStream.h"
#include "WtException.h"
#include "TimeUtil.h"

namespace Wt {

WebController *WebController::instance_ = 0;

WebController::WebController(Configuration& configuration,
			     WebStream& stream, std::string singleSessionId)
  : conf_(configuration),
    stream_(&stream),
    singleSessionId_(singleSessionId),
    shutdown_(false)
#ifdef THREADED
  , threadPool_(2)
#endif // THREADED
{
  instance_ = this;

  XERCES_CPP_NAMESPACE_USE;

  try {
    XMLPlatformUtils::Initialize();
  } catch (const XMLException& toCatch) {
    std::cerr << "Error during Xerces initialization!" << std::endl
	      << XMLString::transcode(toCatch.getMessage()) << std::endl;
  }
}

WebController::WebController(Configuration& configuration)
  : conf_(configuration),
    stream_(0),
    shutdown_(false)
{
  instance_ = this;

  XERCES_CPP_NAMESPACE_USE;

  try {
    XMLPlatformUtils::Initialize();
  } catch (const XMLException& toCatch) {
    std::cerr << "Error during Xerces initialization!" << std::endl
	      << XMLString::transcode(toCatch.getMessage()) << std::endl;
  }
}

WebController::~WebController()
{
  XERCES_CPP_NAMESPACE_USE;

  XMLPlatformUtils::Terminate();
}

void WebController::forceShutdown()
{
  shutdown_ = true;
  for (SessionMap::iterator i = sessions_.begin(); i != sessions_.end(); ++i) {
    WebSession::Handler handler(*i->second);
    handler.killSession();
  }
}

Configuration& WebController::conf()
{
  return instance_->conf_;
}

void WebController::run()
{
  WebRequest *request = stream_->getNextRequest(10);

  if (request)
    handleRequest(request);
  else
    if (!singleSessionId_.empty())
      return;

  for (;;) {
    bool haveMoreSessions = expireSessions();

    if (!haveMoreSessions && !singleSessionId_.empty())
      break;

    WebRequest *request = stream_->getNextRequest(120);

    if (shutdown_) {
      sleep(1000);
      return;
    }

    if (request)
      handleRequestThreaded(request);
  }
}

bool WebController::expireSessions()
{
  Time now;

#ifdef THREADED
  /*
   * -- Begin critical section to expire old sessions and compute next
   *    timeout.
   */
  boost::mutex::scoped_lock sessionsLock(mutex_);
#endif // THREADED

  std::vector<SessionMap::iterator> toErase;
  for (SessionMap::iterator i = sessions_.begin(); i != sessions_.end(); ++i) {
    int diff = i->second->expireTime() - now;

    if (diff < 1000) {
      std::cerr << "Expiring session: " << i->second->sessionId() << std::endl;
      WebSession::Handler handler(*i->second);
      handler.killSession();
      toErase.push_back(i);
    }
  }

  for (unsigned i = 0; i < toErase.size(); ++i)
    sessions_.erase(toErase[i]);

  return !sessions_.empty();
}

void WebController::removeSession(WebSession *session)
{
#ifdef THREADED
  boost::mutex::scoped_lock sessionsLock(mutex_);
#endif // THREADED

  SessionMap::iterator i = sessions_.find(session->sessionId());
  if (i != sessions_.end())
    sessions_.erase(i);
}

static void replace(std::string& s, char c, std::string r)
{
  std::string::size_type p = 0;

  while ((p = s.find(c, p)) != std::string::npos) {
    s.replace(p, 1, r);
    p += r.length();
  }
}

std::string WebController::appSessionCookie(std::string url)
{
  replace(url, '(', "#40");
  replace(url, ')', "#41");
  replace(url, '<', "#60");
  replace(url, '>', "#62");
  replace(url, '@', "#64");
  replace(url, ',', "#44");
  replace(url, ';', "#59");
  replace(url, '\\', "#92");
  replace(url, '"', "#34");
  replace(url, '/', "#47");
  replace(url, '[', "#91");
  replace(url, ']', "#93");
  replace(url, '?', "#63");
  replace(url, '=', "#61");
  replace(url, '{', "#123");
  replace(url, '}', "#125");
  replace(url, ' ', "#32");

  return url;
}

void WebController::handleRequestThreaded(WebRequest *request)
{
#ifdef THREADED
  if (stream_->multiThreaded()) {
    threadPool_.schedule(boost::bind(&WebController::handleRequest,
				     this, request));
  } else
    handleRequest(request);
#else
  handleRequest(request);
#endif // THREADED  
}

std::string WebController::sessionFromCookie(std::string cookies,
					     std::string scriptName,
					     int sessionIdLength)
{
  std::string cookieName = appSessionCookie(scriptName);

  boost::regex
    cookieSession_e(".*\\Q" + cookieName
		    + "\\E=\"?([a-zA-Z0-9]{"
		    + boost::lexical_cast<std::string>(sessionIdLength)
		    + "})\"?.*");

  boost::smatch what;

  if (boost::regex_match(cookies, what, cookieSession_e))
    return what[1];
  else
    return std::string();
}

void WebController::handleRequest(WebRequest *request)
{
  if (!stream_)
    expireSessions();

  CgiParser cgi(conf_.maxRequestSize() * 1024);

  try {
    cgi.parse(*request);
  } catch (std::exception& e) {
    std::cerr << "Could not parse request: " << e.what() << std::endl;

    request->setContentType("text/html");
    request->out()
      << "<title>Error occurred.</title>"
      << "<h2>Error occurred.</h2>"
         "Error parsing CGI request: " << e.what() << std::endl;

    delete request;
    return;
  }

  std::string sessionId = singleSessionId_;
  if (sessionId.empty()) {
    /*
     * Get session from request.
     */
    if (conf_.sessionTracking() == Configuration::CookiesURL
	&& !conf_.reloadIsNewSession())
      sessionId = sessionFromCookie(request->headerValue("Cookie"),
				    request->scriptName(),
				    conf_.sessionIdLength());

    if (sessionId.empty()) {
      CgiEntry *entry = cgi.getEntry("wtd");
      if (entry)
	sessionId = entry->value();
    }
  }

#ifdef THREADED
  /*
   * -- Begin critical section to handle the session.
   */
  boost::mutex::scoped_lock sessionsLock(mutex_);
#endif // THREADED

  SessionMap::iterator i = sessions_.find(sessionId);
  WebSession *session = 0;

  if (i == sessions_.end()) {
    try {
      if (singleSessionId_.empty())
	sessionId = conf_.generateSessionId();

      session = new WebSession(sessionId,
			       conf_.runDirectory() + "/" + sessionId);

      if (cgi.getEntry("debug") && conf_.allowDebug()) {
	std::cerr << "setting Debug" << std::endl;
	session->setDebug(true);
      }

      sessions_[sessionId] = session;
    } catch (std::exception& e) {
      std::cerr << "Error creating session: " << e.what()
		<< std::endl;
      delete request;
      return;
    }
  } else {
    session = i->second;
  }

  {

  WebSession::Handler handler(*session);

#ifdef THREADED
  /*
   * -- End critical section to handle the session.
   */
  sessionsLock.unlock();

  /*
   * -- Start critical section exclusive access to session
   */
  boost::mutex::scoped_lock sessionLock(session->mutex);
#endif // THREADED

  WebRenderer::ResponseType type = WebRenderer::ResponseHTML;

  try {
    switch (session->state()) {
    case WebSession::JustCreated: {
      if (conf_.sessionTracking() == Configuration::CookiesURL) {
	request->addHeader("Set-Cookie",
			   appSessionCookie(request->scriptName())
			   + "=\"" + sessionId + "\"; "
			   "Version=\"1\";");
      }

      if (cgi.getEntry("signal") && !cgi.getEntry("request")) {
	std::cerr << "Send reload event -- new session has signal, "
		  << "is thus an old session that has timed out." 
		  << std::endl;

	session->renderer().letReload(*request, true);
	handler.killSession();
      } else if (cgi.getEntry("resource")) {
	std::cerr << "not serving bootstrap for resource request."
		  << std::endl;
	
	handler.killSession();
	session->renderer().serveError
	  (*request, std::string("Not serving bootstrap for resource request"),
	   WebRenderer::ResponseHTML);
      } else {
	session->init(cgi, *request); // env, url

	if (session->env().agentIsSpiderBot()) {
	  session->env().doesJavaScript_ = false;
	  session->env().doesAjax_ = false;
	  session->env().doesCookies_ = false;

          session->start(conf_.getApplicationCreator(), "");
	  session->setState(WebSession::Loaded, conf_.sessionTimeout());
	  session->renderer().serveMainWidget(*request,
					      WebRenderer::ResponseHTML);
	} else {
	  session->setState(WebSession::Bootstrap, 10);
	  session->renderer().serveBootstrap(*request);
	}
      }

      break;
    }
    case WebSession::Bootstrap: {
      CgiEntry *jsE = cgi.getEntry("js");
      CgiEntry *ajaxE = cgi.getEntry("ajax");
      CgiEntry *inframeE = cgi.getEntry("inframe");
      CgiEntry *historyE = cgi.getEntry("historyKey");

      session->env().doesJavaScript_= jsE && jsE->value() == "yes";
      session->env().doesAjax_ = session->env().doesJavaScript_
	&& ajaxE && ajaxE->value() == "yes";
      session->env().doesCookies_
	= !request->headerValue("Cookie").empty();
      session->env().inFrame_ = inframeE != 0;

      /*
       * the ajax=no&js=yes option does not work properly (and is kind
       * of useless anyway ?)
       */
      if (!session->env().doesAjax_)
	session->env().doesJavaScript_ = false;

      session->start(conf_.getApplicationCreator(),
		     historyE ? historyE->value() : "");

      session->setState(session->env().ajax()
			? WebSession::ExpectLoad : WebSession::Loaded,
			conf_.sessionTimeout());

      session->renderer().serveMainWidget(*request, WebRenderer::ResponseHTML);

      break;
    }
    case WebSession::Loaded:
    case WebSession::ExpectLoad: {
      CgiEntry *resourceE = cgi.getEntry("resource");
      CgiEntry *signalE = cgi.getEntry("signal");
      type = (signalE && !cgi.getEntry("request") && session->env().doesAjax_
	      ? WebRenderer::ResponseEval : WebRenderer::ResponseHTML);

      if (resourceE) {
	WResource *resource = session->decodeResource(resourceE->value());

	if (resource) {
	  CgiEntry *dataE = cgi.getEntry("data");

	  if (dataE) {
	    resource->setFormData(dataE);
	  }
#ifdef THREADED
	  if (resource->reentrant()) {
	    sessionLock.unlock();
	  }
#endif // THREADED

	  try {
	    request->setContentType(resource->resourceMimeType());
	    resource->streamResourceData(request->out());
	  } catch (std::exception& e) {
	    throw WtException("Exception while streaming resource", e);
	  } catch (...) {
	    throw WtException("Exception while streaming resource");
	  }

	  if (dataE) {
	    try {
	      if (cgi.postDataExceeded()) {
		std::cerr << "post data exceeded." << std::endl;
		session->app()->requestTooLarge(cgi.postDataExceeded());
		resource->requestTooLarge(cgi.postDataExceeded());
	      } else
		resource->formDataSet();
	    } catch (std::exception& e) {
	      throw WtException("Exception while setting resource data", e);
	    } catch (...) {
	      throw WtException("Exception while setting resource data");
	    }
	  }

	} else {
	  request->setContentType("text/html");
	}
      } else { 
	if (signalE) {
	  //std::cerr << "signal: " << signalE->value() << std::endl;
	  /*
	   * Special signal values:
	   * 'none' : no event, but perhaps a synchronization
	   * 'load' : load invisible content
	   * 'res'  : after a resource received data
	   */

	  if (signalE->value() == "load"
	      && session->state() != WebSession::ExpectLoad) {
	    /*
	     * Apparently the browser used its local copy of the html,
	     * thus it is out of sync with our widget tree: We force
	     * a reload. This fixes the forward/backward button behaviour
	     * when returning to the Wt application.
	     */
	    std::cerr << "Forcing reload -- unexpected load event."
		      << std::endl;
	    session->renderer().letReload(*request, false);

	    break;
	  }

	  if (signalE->value() != "res") {
	    try {
	      std::vector<WObject *> formObjects
		= session->renderer().formObjects();

	      for (unsigned i = 0; i < formObjects.size(); ++i) {
		WObject *obj = formObjects[i];
		std::string objname = obj->formName();

		CgiEntry *entry = cgi.getEntry(objname);
		if (entry) {
		  if (!cgi.postDataExceeded()) {
		    obj->setFormData(entry);
		    obj->formDataSet();
		  } else
		    obj->requestTooLarge(cgi.postDataExceeded());
		} else {
		  if (!cgi.postDataExceeded())
		    obj->setNoFormData();
		}
	      }
	    } catch (std::exception& e) {
	      throw WtException("Exception while setting form data", e);
	    } catch (...) {
	      throw WtException("Exception while setting form data");
	    }
	  }

	  if (signalE->value() == "hist") {
	    try {
	      CgiEntry *historyE = cgi.getEntry("historyKey");
	      if (historyE)
		session->app()->setCurrentHistoryKey(historyE->value());
	    } catch (std::exception& e) {
	      throw WtException("Exception while emitting wApp->stateChanged",
				e);
	    } catch (...) {
	      throw WtException("Exception while emitting wApp->stateChanged");
	    }
	  } else
	    if (signalE->value() != "none" && signalE->value() != "load"
	      && signalE->value() != "res") {
	    EventSignalBase *s = session->decodeSignal(signalE->value());

	    if (s) {
	      JavascriptEvent jsEvent;
	      jsEvent.get(cgi);

	      try {
		// Save pending changes (e.g. from resource completion)
		session->renderer().saveChanges();

		// First we emit all slots that have been learned, and thus
		// whose javascript code may be discarded
		s->processLearnedStateless();

		session->renderer().discardChanges();

		if (!cgi.postDataExceeded()) {
		  // Next we opportunistically autolearn static slots.
		  s->processAutoLearnStateless(&session->renderer());

		  // Finally we trigger all dynamic slots
		  s->processDynamic(jsEvent);
		}
	      } catch (std::exception& e) {
		throw WtException("Exception while handling event", e);
	      } catch (...) {
		throw WtException("Exception while handling event");
	      }
	    }
	  }

	} else {
	  /*
	   * Is a reload
	   */
	  try {
	    session->refresh();
	  } catch (std::exception& e) {
	    throw WtException("Exception while refreshing session", e);
	  } catch (...) {
	    throw WtException("Exception while refreshing session");
	  }
	}

	try {
	  if (!session->env().doesJavaScript_
	      && (type == WebRenderer::ResponseHTML))
	    session->checkTimers();
	} catch (std::exception& e) {
	  throw WtException("Exception while triggering timers", e);
	} catch (...) {
	  throw WtException("Exception while triggering timers");
	}

	try {
	  if (cgi.postDataExceeded())
	    session->app()->requestTooLarge(cgi.postDataExceeded());
	} catch (std::exception& e) {
	  throw WtException("Exception in WApplication::requestTooLarge", e);
	} catch (...) {
	  throw WtException("Exception in WApplication::requestTooLarge");
	}

	if (session->app()->isQuited()) {
	  session->app()->setTitle(session->app()->title().value().empty()
				   ? L"Wt application quited."
				   : (session->app()->title().value()
				      + L" -- Quited."));
	  handler.killSession();
	}

	session->renderer().serveMainWidget(*request, type);
	session->setState((type == WebRenderer::ResponseEval)
			  ? WebSession::Loaded : WebSession::ExpectLoad,
			  conf_.sessionTimeout());
      }
    }
      break;
    case WebSession::Dead:
      throw WtException("Internal error: WebSession is dead?");
    }
  } catch (WtException& e) {
    std::cerr << e.what() << std::endl;

    handler.killSession();
    session->renderer().serveError(*request, e, type);

  } catch (std::exception& e) {
    std::cerr << "Internal error handling request: " << e.what() << std::endl;

    handler.killSession();
    session->renderer().serveError(*request, e, type);
  } catch (...) {
    std::cerr << "Unknown internal error handling request: very bad."
	      << std::endl;

    handler.killSession();
    session->renderer().serveError(*request, std::string("Unknown internal error"), type);
  }

  if (handler.sessionDead()) {
    std::cerr << "Session exiting: " << sessionId << std::endl;
    removeSession(session);
  }

  delete request; // flushes transmission, and performs clean up

  }

#ifdef THREADED
#ifdef NOTHREADPOOL
  if (stream_) {
    boost::thread self;
    boost::mutex::scoped_lock l(threadsMutex_);

    for (unsigned i = 0; i < threads_.size(); ++i) {
      if (*threads_[i] == self) {
	delete threads_[i];
	threads_.erase(threads_.begin() + i);
	break;
      }
    }
  }
#endif
#endif // THREADED
}

}
