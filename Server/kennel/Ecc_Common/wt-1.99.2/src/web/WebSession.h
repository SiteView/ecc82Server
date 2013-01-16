// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef WEBSESSION_H_
#define WEBSESSION_H_

#include <string>

#ifdef THREADED
#include <boost/thread.hpp>
#endif // THREADED

#include "TimeUtil.h"
#include "WebRenderer.h"
#include "WEnvironment"

namespace Wt {

class CgiParser;
class WebRequest;
class WApplication;

/*
 * The WebSession stores the state for one session.
 */
class WT_API WebSession
{
public:
  WebSession(const std::string sessionId,
	     const std::string sessionPath);
  ~WebSession();

  std::string appBinaryname() const { return appBinaryname_; }
  std::string url() const { return url_; }
  std::string relativeUrl() const { return relativeUrl_; }

  void setDebug(bool debug);
  bool debug() const { return debug_; }

  enum State { JustCreated, Bootstrap, ExpectLoad, Loaded, Dead };
  State state() const { return state_; }
  void setState(State state, int timeout);
  const Time& expireTime() const { return expire_; }

  bool done() { return state_ == Dead; }

  void init(const CgiParser& cgi, const WebRequest& request);

  void start(WApplication::ApplicationCreator createApp,
	     std::string hash);
  void kill();

  void refresh();
  void checkTimers();
  void hibernate();

  std::string   sessionId() const { return sessionId_; }
  WebRenderer&  renderer() { return renderer_; }
  WEnvironment& env()      { return env_; }
  WApplication  *app()     { return app_; }

  void redirect(const std::string url);
  std::string getRedirect();

  WResource       *decodeResource(const std::string resourceId);
  EventSignalBase *decodeSignal(const std::string signalId);

#ifdef THREADED
  boost::mutex mutex;
#endif // THREADED

  class Handler {
  public:
    Handler(WebSession& session);
    ~Handler();

    void killSession();
    bool sessionDead(); // killed or quited()

  private:
    Handler(const Handler&);

#ifdef THREADED
    WebSession *sessionPtr_;
#endif // THREADED
    WebSession& session_;
    bool        killed_;
  };

  static WebSession *instance();

private:
#ifdef THREADED
  boost::mutex    stateMutex_;
#endif // THREADED

  State                  state_;
  std::vector<Handler *> handlers_;
  std::string            sessionId_;
  std::string            sessionPath_;

  WebRenderer            renderer_; // will become a vector: one per window
  std::string            appBinaryname_;
  std::string            url_;
  std::string            relativeUrl_;
  std::string            redirect_;
  Time                   expire_;

  std::string            hash_;
  WEnvironment           env_;
  WApplication          *app_;
  bool                   debug_;

  std::vector<WObject *> emitStack_;

  void pushEmitStack(WObject *obj);
  void popEmitStack();
  WObject *emitStackTop();

#ifdef THREADED
  static boost::thread_specific_ptr<WebSession *> threadSession_;
#else
  static WebSession *threadSession_;
#endif // THREADED

  friend class WApplication;
  friend class SignalBase;
};

}

#endif // WEBSESSION_H_
