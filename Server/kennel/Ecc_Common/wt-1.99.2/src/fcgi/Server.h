// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 * $Id: Server.h,v 1.4 2006/11/26 17:07:57 jozef Exp $
 */
#ifndef SERVER_H_
#define SERVER_H_

#include <string>
#include <map>

#include "Configuration.h"
#include "WApplication"

namespace Wt {

class SessionInfo;

class Server
{
public:
  Server(int argc, char *argv[], WApplication::ApplicationCreator createApp);
  int main();

  static Server *instance;

  void handleSigChld();

private:
  int argc_;
  char **argv_;
  Configuration conf_;

  void spawnSharedProcess();
  int  connectToSession(std::string sessionId, std::string socketPath,
			int maxTries);
  bool getSessionFromQueryString(const std::string uri,
				 std::string& sessionId);
  void checkConfig();

  /*
   * For DedicatedProcess session policy
   */
  typedef std::map<std::string, SessionInfo *> SessionMap;
  SessionMap sessions_;

  /*
   * For SharedProcess session policy
   */
  std::vector<int> sessionProcessPids_;

  const std::string socketPath(const std::string sessionId);
};

}

#endif // WT_SERVER_H_
