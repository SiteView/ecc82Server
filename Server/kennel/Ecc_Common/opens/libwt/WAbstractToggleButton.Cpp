/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WAbstractToggleButton"
#include "WLabel"
#include "DomElement.h"
#include "WSignalInstance_"

#include <iostream>
//#include "WAbstractToggleButton"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WAbstractToggleButton::slots_[] = { WSlot_("setChecked(bool)", false), WSlot_("setChecked()", true), WSlot_("setUnChecked()", true), WSlot_() };
WSignal_ WAbstractToggleButton::signals_[] = { WSignal_("checked()", false), WSignal_("unChecked()", false), WSignal_() };

void WAbstractToggleButton::buildSlotMap(WSlotMap& result)
{
  WFormWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WAbstractToggleButton::buildSignalMap(WSignalMap& result)
{
  WFormWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

void WAbstractToggleButton::checked()
{
  void **args = 0;
  triggerSignal(signals_ + 0, args);
}

void WAbstractToggleButton::unChecked()
{
  void **args = 0;
  triggerSignal(signals_ + 1, args);
}

bool WAbstractToggleButton::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 0) {
    sender_ = sender;    setChecked(*((bool *)args[0]));
    return true;
  }
  if (slot == slots_ + 1) {
    sender_ = sender;    setChecked();
    return true;
  }
  if (slot == slots_ + 2) {
    sender_ = sender;    setUnChecked();
    return true;
  }
  return WFormWidget::triggerSlot(sender, slot, args);
}
bool WAbstractToggleButton::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 1) {
    undoSetChecked();
    return true;
  }
  if (slot == slots_ + 2) {
    undoSetUnChecked();
    return true;
  }
  return WFormWidget::undoTriggerSlot(slot, args);
}


WAbstractToggleButton::WAbstractToggleButton(WContainerWidget *parent)
  : WFormWidget(parent),
    checked_(false),
    checkedChanged_(false)
{ 
  WLabel *label = new WLabel(parent);
  label->setBuddy(this);
}

WAbstractToggleButton::WAbstractToggleButton(const std::string text,
					     WContainerWidget *parent)
  : WFormWidget(parent),
    checked_(false),
    checkedChanged_(false)
{ 
  WLabel *label = new WLabel(text, parent);
  label->setBuddy(this);
}

WAbstractToggleButton::WAbstractToggleButton(const WMessage& message,
					     WContainerWidget *parent)
  : WFormWidget(parent),
    checked_(false),
    checkedChanged_(false)
{ 
  WLabel *label = new WLabel(message, parent);
  label->setBuddy(this);
}

void WAbstractToggleButton::setText(const std::string text)
{
  label()->setText(text);
}

void WAbstractToggleButton::setChecked(bool how)
{
  checked_ = how;
  checkedChanged_ = true;

  if (how)
    emit checked();
  else
    emit unChecked();

  repaint();
}

void WAbstractToggleButton::setChecked()
{
  wasChecked_ = checked_;
  setChecked(true);
}

void WAbstractToggleButton::setUnChecked()
{
  wasChecked_ = checked_;
  setChecked(false);
}

void WAbstractToggleButton::undoSetChecked()
{
  setChecked(checked_);
}

void WAbstractToggleButton::undoSetUnChecked()
{
  undoSetChecked();
}

void WAbstractToggleButton::updateDom(DomElement& element, bool all)
{
  if (checkedChanged_ || all) {
    element.setProperty(Wt::PropertyChecked, checked_ ? "true" : "false");
    checkedChanged_ = false;
  }

  const WSignalInstance_ *changeS = getSignal(SIGNAL(changed()));
  const WSignalInstance_ *checkedS = getSignal(SIGNAL(checked()));
  const WSignalInstance_ *unCheckedS = getSignal(SIGNAL(unChecked()));

  bool needUpdateChangeSignal =
    (changeS->connectivityChanged()
     || checkedS->connectivityChanged()
     || unCheckedS->connectivityChanged()
     || changeS->staticCodeChanged()
     || checkedS->staticCodeChanged()
     || unCheckedS->staticCodeChanged());

  WFormWidget::updateDom(element, all);

  if (needUpdateChangeSignal || all) {
    element.removeEventSignal("change");

    DomElement *e = DomElement::getForUpdate(this, DomElement::INPUT);

    if (isConnected(changeS))
      element.addEventSignal("change", *changeS, 0, 0);
    if (isConnected(checkedS))
      element.addEventSignal("change", *checkedS, 0,
			     (e->createReference()
			      + ".checked == true").c_str());
    if (isConnected(unCheckedS))
      element.addEventSignal("change", *unCheckedS, 0,
			     (e->createReference()
			      + ".checked == false").c_str());
    delete e;
  }
}

void WAbstractToggleButton::setFormData(const char *value, int length,
					const char *filename)
{
  std::string valueS = value;
  checked_ = (valueS != "0");
}

void WAbstractToggleButton::setNoFormData()
{
  if (isLoaded() && isEnabled())
    checked_ = false;
}
