/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WButtonGroup"
#include "WRadioButton"
#include <Algorithm>


#include <iostream>
//#include "WButtonGroup"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WButtonGroup::slots_[] = { WSlot_() };
WSignal_ WButtonGroup::signals_[] = { WSignal_() };

void WButtonGroup::buildSlotMap(WSlotMap& result)
{
  WObject::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WButtonGroup::buildSignalMap(WSignalMap& result)
{
  WObject::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WButtonGroup::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WObject::triggerSlot(sender, slot, args);
}
bool WButtonGroup::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WObject::undoTriggerSlot(slot, args);
}


WButtonGroup::WButtonGroup()
{ }

WButtonGroup::~WButtonGroup()
{
  for (unsigned i = 0; i < buttons_.size(); ++i) {
    buttons_[i]->setGroup(0);
  }
}

void WButtonGroup::addButton(WRadioButton *button)
{
  buttons_.push_back(button);
  button->setGroup(this);
}

void WButtonGroup::removeButton(WRadioButton *button)
{
	buttons_.erase(std::find(buttons_.begin(), buttons_.end(), button));
  button->setGroup(this);
}
