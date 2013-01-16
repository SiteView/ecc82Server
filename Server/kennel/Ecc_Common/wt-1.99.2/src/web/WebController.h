// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 * $Id: WebController.h,v 1.11 2007/01/02 19:53:00 jozef Exp $
 */
#ifndef WEBCONTROLLER_H_
#define WEBCONTROLLER_H_

#include <string>
#include <vector>

#ifdef THREADED
#include <boost/thread.hpp>
#include "threadpool/threadpool.hpp"
#endif

#include "WEnvironment"

namespace Wt {

class CgiParser;
class WebRequest;
class WebStream;
class Configuration;

class WApplication;
class WWidget;
class WObject;
class WResource;
class WStatelessSlot;
class WWebWidget;

/*
 * The controller is a singleton class
 *
 * It either listens for incoming requests from a webstream, using run().
 *
 * Or it may be used to handle an incoming request, using handleRequest().
 * In the latter case, sessions will only expire with a delay -- at the
 * next request. Seems harmless to me.
 *
 * It has the following tasks:
 *  - decode the request
 *  - propagate events
 *  - determine what needs to be served
 *    (a web page, a resource or a javascript update).
 *  - handle session life-cycle: create new sessions, delete quit()ed
 *    sessions, expire sessions on timeout
 *  - manage concurrency
 */
class WT_API WebController
{
public:
  WebController(Configuration& configuration, WebStream& stream,
		std::string singleSessionId = std::string());
  WebController(Configuration& configuration);

  ~WebController();

  void run();
  void handleRequest(WebRequest *request);
  void forceShutdown();

  static Configuration& conf();

  static std::string appSessionCookie(std::string url);
  static std::string sessionFromCookie(std::string cookies,
				       std::string scriptName,
				       int sessionIdLength);

private:
  Configuration& conf_;
  WebStream*     stream_;
  std::string    singleSessionId_;

  typedef std::map<std::string, WebSession *> SessionMap;
  SessionMap sessions_;

  bool shutdown_;

#ifdef THREADED
  // mutex to protect access to the sessions map.
  boost::mutex            mutex_;

  // mutex to protect access to the sessions map.
  boost::threadpool::pool threadPool_;
#endif

  bool expireSessions();
  void removeSession(WebSession *session);
  void handleRequestThreaded(WebRequest *request);

  void checkTimers();

  static WebController *instance_;
};

}

#endif // WEBCONTROLLER_H_
