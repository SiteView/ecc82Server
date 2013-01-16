/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WBreak"
#include "DomElement.h"

#include <iostream>
//#include "WBreak"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WBreak::slots_[] = { WSlot_() };
WSignal_ WBreak::signals_[] = { WSignal_() };

void WBreak::buildSlotMap(WSlotMap& result)
{
  WWebWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WBreak::buildSignalMap(WSignalMap& result)
{
  WWebWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WBreak::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WWebWidget::triggerSlot(sender, slot, args);
}
bool WBreak::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WWebWidget::undoTriggerSlot(slot, args);
}

WBreak::WBreak(WContainerWidget *parent)
  : WWebWidget(parent)
{ }

void WBreak::updateDom(DomElement& element, bool all)
{
  WWebWidget::updateDom(element, all);

  renderOk();
}

DomElement *WBreak::createDomElement()
{
  DomElement *result = DomElement::createNew(DomElement::BR);
  result->setId(this);

  updateDom(*result, true);

  return result;
}

void WBreak::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::BR);
    updateDom(*e, false);
    result.push_back(e);
  }
}
