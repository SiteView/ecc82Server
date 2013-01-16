/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <algorithm>

#include "WApplication"
#include "WContainerWidget"
#include "WTimerWidget"

#include "WebSession.h"
#include "WebRequest.h"
#include "WebController.h"
#include "Configuration.h"

namespace Wt {

/*
 * About the mutexes.
 *
 * A living session is identified and a handler is created for it while
 * holding the global sessions lock.
 *
 * Sessions are expired when the global sessions lock is in possession, and
 * these two conditions make that a session dies atomically.
 *
 */

#ifdef THREADED
boost::thread_specific_ptr<WebSession *> WebSession::threadSession_;
#else
WebSession *WebSession::threadSession_;
#endif // THREADED

WebSession::WebSession(const std::string sessionId,
		       const std::string sessionPath)
  : state_(JustCreated),
    sessionId_(sessionId),
    sessionPath_(sessionPath),
    renderer_(*this),
    env_(this),
    app_(0),
    debug_(false)
{
  std::cerr << "Session " + sessionId_ + " created." << std::endl;
  expire_ = Time() + 60*1000;
}

WebSession::~WebSession()
{
  delete app_;
  unlink(sessionPath_.c_str());

  std::cerr << "Session " + sessionId_ + " destroyed." << std::endl;
}

void WebSession::setState(State state, int timeout)
{
#ifdef THREADED
  boost::mutex::scoped_lock stateLock(stateMutex_);
#endif // THREADED

  if (state_ != Dead) {
    state_ = state;
    expire_ = Time() + timeout*1000;
  }
}

void WebSession::init(const CgiParser& cgi, const WebRequest& request)
{
  env_.init(cgi, request);

  url_ = request.scriptName();
  appBinaryname_ = url_;

  std::string::size_type slashpos = url_.find_last_of('/');
  if (slashpos != std::string::npos) {
    appBinaryname_.erase(0, slashpos+1);
  }
  std::cerr << appBinaryname_ << std::endl;

  if (WebController::conf().sessionTracking() == Configuration::CookiesURL
      && env_.supportsCookies()
      && !WebController::conf().reloadIsNewSession())
    relativeUrl_ = "?a=b";
  else
    relativeUrl_ = "?wtd=" + sessionId_;

  url_ += relativeUrl_;
}

void WebSession::start(WApplication::ApplicationCreator createApplication,
		       std::string hash)
{
  hash_ = hash;

  app_ = createApplication(env_);

  if (!app_)
    kill();
}

void WebSession::kill()
{
#ifdef THREADED
  boost::mutex::scoped_lock stateLock(stateMutex_);
#endif // THREADED

  state_ = Dead;

  if (handlers_.empty()) {
#ifdef THREADED
    // we may unlock because the session has already been removed
    // from the sessions list, and thus, once the list is empty it is
    // guaranteed to stay empty.
    stateLock.unlock();
#endif // THREADED

    delete this;
  }
}

void WebSession::setDebug(bool debug)
{
  debug_ = debug;
}

void WebSession::refresh()
{
  app_->refresh();
}

void WebSession::checkTimers()
{
  WContainerWidget *timers = app_->timerRoot();

  const std::vector<WWidget *>& timerWidgets = timers->children();

  std::vector<WTimerWidget *> expired;

  for (unsigned i = 0; i < timerWidgets.size(); ++i) {
    WTimerWidget *wti = dynamic_cast<WTimerWidget *>(timerWidgets[i]);

    if (wti->timerExpired())
      expired.push_back(wti);
  }

  WMouseEvent dummy;

  for (unsigned i = 0; i < expired.size(); ++i)
    expired[i]->clicked.emit(dummy);
}

void WebSession::redirect(const std::string url)
{
  redirect_ = url;
}

std::string WebSession::getRedirect()
{
  std::string result = redirect_;
  redirect_.clear();
  return result;
}

WebSession::Handler::Handler(WebSession& session)
  : session_(session),
    killed_(false)
{
#ifdef THREADED
  boost::mutex::scoped_lock stateLock(session_.stateMutex_);
#endif // THREADED
  session_.handlers_.push_back(this);
  //std::cerr << "handlers: " << session_.handlers_.size() << std::endl;

#ifdef THREADED
  sessionPtr_ = &session_;
  threadSession_.reset(&sessionPtr_);
#else
  threadSession_ = &session;
#endif // THREADED
}

WebSession::Handler::~Handler()
{
  {
#ifdef THREADED
    boost::mutex::scoped_lock stateLock(session_.stateMutex_);
#endif // THREADED

    std::vector<Handler *>::iterator i
      = std::find(session_.handlers_.begin(), session_.handlers_.end(), this);

    session_.handlers_.erase(i);
  }

  if (session_.handlers_.size() == 0)
    session_.hibernate();

  if (killed_)
    session_.kill();

#ifdef THREADED
  threadSession_.release();
#endif // THREADED
}

void WebSession::Handler::killSession()
{
  killed_ = true;
}

bool WebSession::Handler::sessionDead()
{
  return (killed_ || session_.done());
}

void WebSession::hibernate()
{
  if (app_)
    app_->messageResourceBundle().hibernate();
}

WResource *WebSession::decodeResource(const std::string resourceId)
{
  WResource *resource = app_->decodeExposedResource(resourceId);

  if (resource)
    return resource;
  else {
    std::cerr << "Error: decodeResource did not yield an exposed resource."
	      << std::endl;
    return 0;
  }
}

EventSignalBase *WebSession::decodeSignal(const std::string signalId)
{
  EventSignalBase *result = app_->decodeExposedSignal(signalId);

  if (result)
    return result;
  else {
    std::cerr << "Warning: decodeSignal did not yield an exposed signal."
	      << std::endl;
    return 0;    
  }
}

WebSession *WebSession::instance()
{
#ifdef THREADED
  return *threadSession_;
#else
  return threadSession_;
#endif // THREADED
}

void WebSession::pushEmitStack(WObject *o)
{
  emitStack_.push_back(o);
}

void WebSession::popEmitStack()
{
  emitStack_.pop_back();
}

WObject *WebSession::emitStackTop()
{
  if (!emitStack_.empty())
    return emitStack_.back();
  else
    return 0;
}

}
