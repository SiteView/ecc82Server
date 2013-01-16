/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WPushButton"
#include "DomElement.h"

#include <iostream>
//#include "WPushButton"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WPushButton::slots_[] = { WSlot_() };
WSignal_ WPushButton::signals_[] = { WSignal_() };

void WPushButton::buildSlotMap(WSlotMap& result)
{
  WFormWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WPushButton::buildSignalMap(WSignalMap& result)
{
  WFormWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WPushButton::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WFormWidget::triggerSlot(sender, slot, args);
}
bool WPushButton::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WFormWidget::undoTriggerSlot(slot, args);
}


WPushButton::WPushButton(WContainerWidget *parent)
  : WFormWidget(parent),
    textChanged_(false)
{
	redict_ = NULL;
}

WPushButton::WPushButton(const std::string text, WContainerWidget *parent)
  : WFormWidget(parent),
    text_(text),
    textChanged_(false)
{ 
	redict_ = NULL;
}



WPushButton::WPushButton(const WMessage& message, WContainerWidget *parent)
  : WFormWidget(parent),
    message_(message),
    textChanged_(false)
{ 
	redict_ = NULL;
  text_ = message_.value();
}

void WPushButton::setText(const std::string text)
{
  message_ = WMessage();
  text_ = text;
  textChanged_ = true;

  repaint();
}

void WPushButton::setMessage(const WMessage& message)
{
  setText(message.value());
  message_ = message;
}

void WPushButton::updateDom(DomElement& element, bool all)
{
  if (textChanged_ || all) {
    element.setProperty(Wt::PropertyInnerHTML, text_);
    textChanged_ = false;
  }
  element.redict_ = redict_;

  WFormWidget::updateDom(element, all);
}

DomElement *WPushButton::createDomElement()
{
  DomElement *result = DomElement::createNew(DomElement::BUTTON);
  result->setAttribute("type", "button"); // default button
  result->setId(this);

  updateDom(*result, true);

  result -> redict_ = redict_;

  return result;
}

void WPushButton::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::INPUT);
    updateDom(*e, false);
    result.push_back(e);
  }
}

void WPushButton::refresh()
{
  if (!message_.empty()
      && (message_.value() != text_))
    setMessage(WMessage(message_));

  WFormWidget::refresh();
}
