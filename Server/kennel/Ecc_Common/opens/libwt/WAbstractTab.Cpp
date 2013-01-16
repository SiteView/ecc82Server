/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WAbstractTab"
#include "WText"
#include "WContainerWidget"
#include "WTabWidget"

#include <iostream>
//#include "WAbstractTab"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WAbstractTab::slots_[] = { WSlot_("selectTab()", true), WSlot_() };
WSignal_ WAbstractTab::signals_[] = { WSignal_() };

void WAbstractTab::buildSlotMap(WSlotMap& result)
{
  WObject::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WAbstractTab::buildSignalMap(WSignalMap& result)
{
  WObject::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WAbstractTab::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 0) {
    sender_ = sender;    selectTab();
    return true;
  }
  return WObject::triggerSlot(sender, slot, args);
}
bool WAbstractTab::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 0) {
    undoSelectTab();
    return true;
  }
  return WObject::undoTriggerSlot(slot, args);
}


WAbstractTab::WAbstractTab(const std::string text,
			   const std::string classUnselected,
			   const std::string classSelected)
  : text_(text),
    classUnselected_(classUnselected),
    classSelected_(classSelected)
{ }

const std::string WAbstractTab::label() const
{
  return text_;
}

void WAbstractTab::setLabel(const std::string text)
{
  text_ = text;
}

void WAbstractTab::show()
{
  window()->show();

  buttonWidget_->setStyleClass(classSelected_);
}

void WAbstractTab::hide()
{
  window()->hide();

  buttonWidget_->setStyleClass(classUnselected_);
}

void WAbstractTab::doCreateButton(WContainerWidget *parent)
{
  buttonWidget_ = createButton();
  parent->addWidget(buttonWidget_);
}

void WAbstractTab::selectTab()
{
  lastIndex_ = tabWidget_->currentIndex();
  tabWidget_->setCurrentTab(this);
}

void WAbstractTab::undoSelectTab()
{
  tabWidget_->setCurrentIndex(lastIndex_);
}
