// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * All rights reserved.
 */

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <exception>
#include <string>
#include "WDllDefs.h"
#include "WApplication"

namespace boost {
  namespace program_options {
    class variables_map;
  }
}

namespace Wt {

class WT_API Configuration
{
public:
  class Exception : public std::exception
  {
  public:
    Exception(const std::string what);
    ~Exception() throw();

    const char *what() const throw() { return what_.c_str(); }

  private:
    std::string what_;
  };

  Configuration(int argc, char **argv, WApplication::ApplicationCreator createApplication);

  enum SessionPolicy { DedicatedProcess, SharedProcess };
  enum SessionTracking { CookiesURL, URL };

  WApplication::ApplicationCreator getApplicationCreator() const { return createApplication_; }
  SessionPolicy    sessionPolicy() const { return sessionPolicy_; }
  int              numProcesses() const { return numProcesses_; }
  int              maxNumSessions() const { return maxNumSessions_; }
  int              maxRequestSize() const { return maxRequestSize_; }
  SessionTracking  sessionTracking() const { return sessionTracking_; }
  bool             reloadIsNewSession() const { return reloadIsNewSession_; }
  int              sessionTimeout() const { return sessionTimeout_; }
  std::string      valgrindPath() const { return valgrindPath_; }
  bool             allowDebug() const { return allowDebug_; }
  std::string      runDirectory() const { return runDirectory_; }
  int              sessionIdLength() const { return sessionIdLength_; }

  /*
   * Generate a unique session Id, and create its session file.
   */
  std::string      generateSessionId() const;

private:
  WApplication::ApplicationCreator createApplication_;
  SessionPolicy   sessionPolicy_;
  int             numProcesses_;
  int             maxNumSessions_;
  int             maxRequestSize_;
  SessionTracking sessionTracking_;
  bool            reloadIsNewSession_;
  int             sessionTimeout_;
  std::string     valgrindPath_;
  bool            allowDebug_;
  std::string     runDirectory_;
  int             sessionIdLength_;
  int		  pid_;

  void readConfiguration(const char *configurationFile, const char *location);
};

}

#endif // HTTP_CONFIGURATION_HPP
