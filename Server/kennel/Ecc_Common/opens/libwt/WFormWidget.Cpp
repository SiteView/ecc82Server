/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WFormWidget"
#include "DomElement.h"
#include "WLabel"

#include <iostream>
//#include "WFormWidget"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WFormWidget::slots_[] = { WSlot_("enable()", true), WSlot_("disable()", true), WSlot_("setEnabled(bool)", false), WSlot_() };
WSignal_ WFormWidget::signals_[] = { WSignal_("selected()", true), WSignal_("blurred()", true), WSignal_("focussed()", true), WSignal_() };

void WFormWidget::buildSlotMap(WSlotMap& result)
{
  WInteractWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WFormWidget::buildSignalMap(WSignalMap& result)
{
  WInteractWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

void WFormWidget::changed()
{
  void **args = 0;
  triggerSignal(signals_ + 0, args);
}

void WFormWidget::selected()
{
  void **args = 0;
  triggerSignal(signals_ + 1, args);
}

void WFormWidget::blurred()
{
  void **args = 0;
  triggerSignal(signals_ + 2, args);
}

void WFormWidget::focussed()
{
  void **args = 0;
  triggerSignal(signals_ + 3, args);
}

bool WFormWidget::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 0) {
    sender_ = sender;    enable();
    return true;
  }
  if (slot == slots_ + 1) {
    sender_ = sender;    disable();
    return true;
  }
  if (slot == slots_ + 2) {
    sender_ = sender;    setEnabled(*((bool *)args[0]));
    return true;
  }
  return WInteractWidget::triggerSlot(sender, slot, args);
}
bool WFormWidget::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 0) {
    undoEnable();
    return true;
  }
  if (slot == slots_ + 1) {
    undoDisable();
    return true;
  }
  return WInteractWidget::undoTriggerSlot(slot, args);
}


WFormWidget::WFormWidget(WContainerWidget *parent)
  : WInteractWidget(parent),
    label_(0),
    enabled_(true),
    enabledChanged_(false)
{ }

WFormWidget::~WFormWidget()
{
  if (label_)
    label_->setBuddy(0);
}

void WFormWidget::enable()
{
  wasEnabled_ = enabled_;
  setEnabled(true);
}

void WFormWidget::disable()
{
  wasEnabled_ = enabled_;
  setEnabled(false);
}

void WFormWidget::undoEnable()
{
  setEnabled(wasEnabled_);
}

void WFormWidget::undoDisable()
{
  undoEnable();
}

void WFormWidget::setEnabled(bool enabled)
{
  enabled_ = enabled;
  enabledChanged_ = true;
  repaint();
}

void WFormWidget::updateDom(DomElement& element, bool all)
{
  updateSignalConnection(element, SIGNAL(changed()), "change", all);
  updateSignalConnection(element, SIGNAL(selected()), "select", all);
  updateSignalConnection(element, SIGNAL(blurred()), "blur", all);
  updateSignalConnection(element, SIGNAL(focussed()), "focus", all);

  if (enabledChanged_ || all) {
    element.setProperty(Wt::PropertyDisabled, enabled_ ? "false" : "true");
    enabledChanged_ = false;
  }

  WInteractWidget::updateDom(element, all);
}

void WFormWidget::setLabel(WLabel *label)
{
  label_ = label;
}

void WFormWidget::setHidden(bool hidden)
{
  if (label_)
    label_->setHidden(hidden);

  WInteractWidget::setHidden(hidden);
}
