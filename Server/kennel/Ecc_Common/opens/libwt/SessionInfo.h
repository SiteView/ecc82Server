/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
// This may look like C code, but it's really -*- C++ -*-
#ifndef QWEB_SESSIONINFO_H_
#define QWEB_SESSIONINFO_H_

#define pid_t int

#include <stdio.h>
#include <string>

class __declspec(dllexport) SessionInfo
{
public:
  SessionInfo(const std::string sessionId,
	      pid_t childPId, const std::string socketPath);

  const std::string sessionId() const { return sessionId_; }
  pid_t childPId() const { return childPId_; }
  const std::string socketPath() const { return socketPath_; }

private:
  std::string sessionId_;
  pid_t       childPId_;
  std::string socketPath_;
};

#endif // QWEB_SESSIONINFO_H_
