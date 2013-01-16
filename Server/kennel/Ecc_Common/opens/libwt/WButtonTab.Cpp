/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WButtonTab"
#include "WText"
#include "WContainerWidget"

#include <iostream>
//#include "WButtonTab"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WButtonTab::slots_[] = { WSlot_() };
WSignal_ WButtonTab::signals_[] = { WSignal_() };

void WButtonTab::buildSlotMap(WSlotMap& result)
{
  WAbstractTab::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WButtonTab::buildSignalMap(WSignalMap& result)
{
  WAbstractTab::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WButtonTab::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WAbstractTab::triggerSlot(sender, slot, args);
}
bool WButtonTab::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WAbstractTab::undoTriggerSlot(slot, args);
}


WButtonTab::WButtonTab(const std::string text)
  : WAbstractTab(text, "tabbutton", "tabbuttoncurrent")
{ }

WWidget *WButtonTab::createButton()
{
  WContainerWidget *result = new WContainerWidget();

  tabButton_ = new WText(label(), result);
  connect(tabButton_, SIGNAL(clicked()), this, SLOT(tabSelected()), STATIC);
  tabButton_->setMargin(WLength(10));

  return result;
}

void WButtonTab::setLabel(const std::string text)
{
  tabButton_->setText(text);
  WAbstractTab::setLabel(text);
}
