/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WTabWidget"
#include "WAbstractTab"
#include "WTableCell"

#include <iostream>
//#include "WTabWidget"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WTabWidget::slots_[] = { WSlot_("setCurrentIndex(int)", false), WSlot_("setCurrentTab(WAbstractTab *)", false), WSlot_() };
WSignal_ WTabWidget::signals_[] = { WSignal_() };

void WTabWidget::buildSlotMap(WSlotMap& result)
{
  WTable::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WTabWidget::buildSignalMap(WSignalMap& result)
{
  WTable::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WTabWidget::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 0) {
    sender_ = sender;    setCurrentIndex(*((int *)args[0]));
    return true;
  }
  if (slot == slots_ + 1) {
    sender_ = sender;    setCurrentTab(*((WAbstractTab * *)args[0]));
    return true;
  }
  return WTable::triggerSlot(sender, slot, args);
}
bool WTabWidget::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WTable::undoTriggerSlot(slot, args);
}


WTabWidget::WTabWidget(WContainerWidget *parent)
  : WTable(parent),
    currentIndex_(-1)
{
  tabBar_ = new WTable(elementAt(0, 0));
  tabWindowParent_ = elementAt(1, 0);
}

void WTabWidget::insertTab(int index, WAbstractTab *tab)
{
  tabs_.insert(tabs_.begin() + index, tab);

  tab->tabWidget_ = this;
  tab->window_ = new WContainerWidget(tabWindowParent());
  tab->window_->hide();

  insertRow(index);

  tab->doCreateButton(tabBarCell(index));

  if (currentIndex_ == -1)
    setCurrentIndex(0);
}

void WTabWidget::addTab(WAbstractTab *tab)
{
  insertTab(tabs_.size(), tab);
}

WAbstractTab *WTabWidget::getTab(int index) const
{
  return tabs_[index];
}

void WTabWidget::setCurrentIndex(int index)
{
  currentIndex_ = index;
  setCurrentTab(tabs_[currentIndex_]);
}

void WTabWidget::setCurrentTab(WAbstractTab *tab)
{
  for (unsigned i = 0; i < tabs_.size(); ++i) {
    if (tabs_[i] == tab)
      tabs_[i]->show();
    else
      tabs_[i]->hide();
  }
}

WContainerWidget *WTabWidget::tabBarCell(int index)
{
  return tabBar_->elementAt(0, index);
}
