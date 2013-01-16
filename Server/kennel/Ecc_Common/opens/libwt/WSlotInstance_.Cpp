/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <iostream>

#include "WSlotInstance_"
#include "WSignalInstance_"
#include "WConnection_"
#include "WObject"
#include "WSlot_"
#include <Algorithm>
#include <windows.h>

int WSlotInstance_::m_nCount = 0;

WSlotInstance_::WSlotInstance_(WObject *object, WSlot_ *slot)
  :  slot_(slot),
     object_(object),
     staticCodeCollected_(false)
{
    m_nCount ++;
}

WSlotInstance_::~WSlotInstance_()
{
    m_nCount --;
  while (rConnections_.size())
    delete rConnections_[0];
}

void WSlotInstance_::addConnection(WConnection_ *connection)
{
  rConnections_.push_back(connection);
}

void WSlotInstance_::removeConnection(WConnection_ *connection)
{
  std::vector<WConnection_ *>::iterator i
	  = std::find(rConnections_.begin(), rConnections_.end(), connection);
  if (i == rConnections_.end()) {
    std::cerr << "Inconsitency in slot rConnections: could not find"
      " connection" << std::endl;
    exit(1);
  } else {
    /*
     * inform the signal that the static code changes because a 
     * connection is deleted
     */
    if (connection->type() == WObject::STATIC)
      connection->signal()->setStaticCodeChanged();

    rConnections_.erase(i);
  }
}

void WSlotInstance_::invalidateStaticCode()
{
  staticCodeCollected_ = false;
}

void WSlotInstance_::setStaticCode(const std::string s)
{
//	OutputDebugString(s.c_str());
	//OutputDebugString(" -- setStaticCode \n");
  staticCode_ = s;
  staticCodeCollected_ = true;

  /*
   * Inform all signals that their static code must be updated
   */
  for (unsigned i = 0; i < rConnections_.size(); ++i)
    rConnections_[i]->signal()->setStaticCodeChanged();
}

bool WSlotInstance_::trigger(WObject *sender, void **args)
{
  return object_->triggerSlot(sender, slot_, args);
}

void WSlotInstance_::undoTrigger(void **args)
{
  object_->undoTriggerSlot(slot_, args);
}

bool WSlotInstance_::needTriggerStatic() const
{
  return !staticCodeCollected_ && slot_->canStatic();
}
