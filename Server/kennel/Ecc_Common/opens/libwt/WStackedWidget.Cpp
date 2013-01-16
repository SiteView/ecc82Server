/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WStackedWidget"
#include <Algorithm>

#include <iostream>
//#include "WStackedWidget"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WStackedWidget::slots_[] = { WSlot_("setCurrentIndex(int)", false), WSlot_("setCurrentWidget(WWidget *)", false), WSlot_() };
WSignal_ WStackedWidget::signals_[] = { WSignal_() };

void WStackedWidget::buildSlotMap(WSlotMap& result)
{
  WContainerWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WStackedWidget::buildSignalMap(WSignalMap& result)
{
  WContainerWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WStackedWidget::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 0) {
    sender_ = sender;    setCurrentIndex(*((int *)args[0]));
    return true;
  }
  if (slot == slots_ + 1) {
    sender_ = sender;    setCurrentWidget(*((WWidget * *)args[0]));
    return true;
  }
  return WContainerWidget::triggerSlot(sender, slot, args);
}
bool WStackedWidget::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WContainerWidget::undoTriggerSlot(slot, args);
}


WStackedWidget::WStackedWidget(WContainerWidget *parent)
  : WContainerWidget(parent),
    currentIndex_(-1)
{ }

void WStackedWidget::addWidget(WWidget *widget)
{
  insertWidget(widgets_.size(), widget);
}

int WStackedWidget::count() const
{
  return widgets_.size();
}

int WStackedWidget::currentIndex() const
{
  return currentIndex_;
}

WWidget *WStackedWidget::currentWidget() const
{
  return widgets_[currentIndex_];
}

int WStackedWidget::indexOf(WWidget *widget) const
{
	return std::find(widgets_.begin(), widgets_.end(), widget) - widgets_.begin();
}

int WStackedWidget::insertWidget(int index, WWidget *widget)
{
  // do not bother inserting in correct place since this does not matter
  // as only one will be visible at each time
  WContainerWidget::addWidget(widget);

  widget->hide();
  widgets_.insert(widgets_.begin() + index, widget);

  if (currentIndex_ == -1)
    setCurrentIndex(0);

  return index;
}

void WStackedWidget::removeWidget(WWidget *widget)
{
  widgets_.erase(widgets_.begin() + indexOf(widget));
  if (currentIndex_ >= (int)widgets_.size())
    setCurrentIndex(widgets_.size() - 1);
}

WWidget *WStackedWidget::widget(int index) const
{
  return widgets_[index];
}

void WStackedWidget::setCurrentIndex(int index)
{
  currentIndex_ = index;
  for (unsigned i = 0; i < (unsigned)widgets_.size(); ++i)
    widgets_[i]->setHidden(currentIndex_ != (int)i);
}

void WStackedWidget::setCurrentWidget(WWidget *widget)
{
  setCurrentIndex(indexOf(widget));
}

