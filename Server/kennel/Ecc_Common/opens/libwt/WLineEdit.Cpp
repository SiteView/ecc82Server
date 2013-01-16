/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <boost/lexical_cast.hpp>

#include "WLineEdit"
#include "DomElement.h"

#include <iostream>
//#include "WLineEdit"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WLineEdit::slots_[] = { WSlot_("setText(const std::string)", false), WSlot_() };
WSignal_ WLineEdit::signals_[] = { WSignal_() };

void WLineEdit::buildSlotMap(WSlotMap& result)
{
  WFormWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WLineEdit::buildSignalMap(WSignalMap& result)
{
  WFormWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WLineEdit::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 0) {
    sender_ = sender;    setText(*((const std::string *)args[0]));
    return true;
  }
  return WFormWidget::triggerSlot(sender, slot, args);
}
bool WLineEdit::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WFormWidget::undoTriggerSlot(slot, args);
}


WLineEdit::WLineEdit(WContainerWidget *parent)
  : WFormWidget(parent),
    textSize_(10),
    echoMode_(Normal),
    contentChanged_(false),
    textSizeChanged_(false),
    echoModeChanged_(false)
{ 
  setInline(true);
  setFormObject(true);
}

WLineEdit::WLineEdit(const std::string text, WContainerWidget *parent)
  : WFormWidget(parent),
    content_(text),
    textSize_(10),
    echoMode_(Normal),
    contentChanged_(false),
    textSizeChanged_(false),
    echoModeChanged_(false)
{ 
  setInline(true);
  setFormObject(true);
}

void WLineEdit::setText(const std::string text)
{
  content_ = text;
  contentChanged_ = true;
  repaint();
}

void WLineEdit::setTextSize(int chars)
{
  textSize_ = chars;
  textSizeChanged_ = true;
  repaint();
}

void WLineEdit::setEchoMode(EchoMode echoMode)
{
  echoMode_ = echoMode;
  echoModeChanged_ = true;
  repaint();
}

void WLineEdit::updateDom(DomElement& element, bool all)
{
  if (contentChanged_ || all) {
    element.setAttribute("value", content_);
    element.setProperty(Wt::PropertyValue, content_);
    contentChanged_ = false;
  }

  if (echoModeChanged_) {
    element.setAttribute("type", echoMode_ == Normal ? "text" : "password");
    echoModeChanged_ = false;
  }

  if (textSizeChanged_ || all) {
    element.setAttribute("size",
			 boost::lexical_cast<std::string>(textSize_));
    textSizeChanged_ = false;
  }

  WFormWidget::updateDom(element, all);
}

DomElement *WLineEdit::createDomElement()
{
  DomElement *result = DomElement::createNew(DomElement::INPUT);
  result->setAttribute("type", echoMode_ == Normal ? "text" : "password");

  result->setId(this, true);

  updateDom(*result, true);

  return result;
}

void WLineEdit::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::INPUT);
    updateDom(*e, false);
    result.push_back(e);
  }
}

void WLineEdit::setFormData(const char *value, int length,
			    const char *filename)
{
  content_ = value;
}
