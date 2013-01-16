/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WRadioButton"
#include "DomElement.h"
#include "WButtonGroup"

#include <iostream>
//#include "WRadioButton"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WRadioButton::slots_[] = { WSlot_() };
WSignal_ WRadioButton::signals_[] = { WSignal_() };

void WRadioButton::buildSlotMap(WSlotMap& result)
{
  WAbstractToggleButton::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WRadioButton::buildSignalMap(WSignalMap& result)
{
  WAbstractToggleButton::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WRadioButton::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WAbstractToggleButton::triggerSlot(sender, slot, args);
}
bool WRadioButton::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WAbstractToggleButton::undoTriggerSlot(slot, args);
}


WRadioButton::WRadioButton(WContainerWidget *parent)
  : WAbstractToggleButton(parent),
    buttonGroup_(0)
{
  setFormObject(true);
}

WRadioButton::WRadioButton(const std::string text, WContainerWidget *parent)
  : WAbstractToggleButton(text, parent),
    buttonGroup_(0)
{
  setFormObject(true);
}

void WRadioButton::updateDom(DomElement& element, bool all)
{
  WAbstractToggleButton::updateDom(element, all);
}

DomElement *WRadioButton::createDomElement()
{
  DomElement *checkBox = DomElement::createNew(DomElement::INPUT);
  checkBox->setAttribute("type", "radio");
  checkBox->setId(this);
  if (buttonGroup_)
    checkBox->setAttribute("name", buttonGroup_->formName());
  else
    checkBox->setAttribute("name", formName());

  updateDom(*checkBox, true);

  return checkBox;
}

void WRadioButton::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::INPUT);
    updateDom(*e, false);
    result.push_back(e);
  }
}

void WRadioButton::setGroup(WButtonGroup *group)
{
  buttonGroup_ = group;
}
