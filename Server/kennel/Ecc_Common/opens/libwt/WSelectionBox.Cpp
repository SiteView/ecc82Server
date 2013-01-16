/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <boost/lexical_cast.hpp>

#include "WSelectionBox"
#include "DomElement.h"

#include <iostream>
//#include "WSelectionBox"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WSelectionBox::slots_[] = { WSlot_() };
WSignal_ WSelectionBox::signals_[] = { WSignal_() };

void WSelectionBox::buildSlotMap(WSlotMap& result)
{
  WComboBox::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WSelectionBox::buildSignalMap(WSignalMap& result)
{
  WComboBox::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WSelectionBox::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WComboBox::triggerSlot(sender, slot, args);
}
bool WSelectionBox::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WComboBox::undoTriggerSlot(slot, args);
}


WSelectionBox::WSelectionBox(WContainerWidget *parent)
  : WComboBox(parent),
    verticalSize_(5),
    verticalSizeChanged_(false)
{ }

void WSelectionBox::setVerticalSize(int items)
{
  verticalSize_ = items;
  verticalSizeChanged_ = true;
  repaint();
}

void WSelectionBox::updateDom(DomElement& element, bool all)
{
  if (verticalSizeChanged_ || all) {
    element.setAttribute("size",
			 boost::lexical_cast<std::string>(verticalSize_));
    verticalSizeChanged_ = false;
  }

  WComboBox::updateDom(element, all);

  renderOk();
}
