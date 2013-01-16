/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WConnection_"
#include "WSignalInstance_"
#include "WSlotInstance_"

int WConnection_::m_nCount = 0;

WConnection_::WConnection_(WSignalInstance_ *signal,
			   WSlotInstance_ *slot,
			   WObject::ConnectionType type)
  : signal_(signal),
    slot_(slot),
    type_(type)
{
  signal_->addConnection(this);
  slot_->addConnection(this);
  m_nCount ++;
}

WConnection_::~WConnection_()
{
  signal_->removeConnection(this);
  slot_->removeConnection(this);
  m_nCount --;
}

bool WConnection_::trigger(void **args)
{
  return slot_->trigger(signal_->object(), args);
}

std::string WConnection_::getStaticCode() const
{
  return slot_->getStaticCode();
}
