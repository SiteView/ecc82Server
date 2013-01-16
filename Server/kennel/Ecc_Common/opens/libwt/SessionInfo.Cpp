/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "SessionInfo.h"

SessionInfo::SessionInfo(const std::string sessionId,
			 pid_t childPId, const std::string socketPath)
  : sessionId_(sessionId),
    childPId_(childPId),
    socketPath_(socketPath)
{ }
