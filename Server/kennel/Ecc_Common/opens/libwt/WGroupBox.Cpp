/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WGroupBox"
#include "DomElement.h"

#include <iostream>
//#include "WGroupBox"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WGroupBox::slots_[] = { WSlot_() };
WSignal_ WGroupBox::signals_[] = { WSignal_() };

void WGroupBox::buildSlotMap(WSlotMap& result)
{
  WContainerWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WGroupBox::buildSignalMap(WSignalMap& result)
{
  WContainerWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WGroupBox::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WContainerWidget::triggerSlot(sender, slot, args);
}
bool WGroupBox::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WContainerWidget::undoTriggerSlot(slot, args);
}


WGroupBox::WGroupBox(WContainerWidget *parent)
  : WContainerWidget(parent),
    titleChanged_(false)
{ }

WGroupBox::WGroupBox(const std::string title, WContainerWidget *parent)
  : WContainerWidget(parent),
    title_(title),
    titleChanged_(false)
{ }

WGroupBox::WGroupBox(const WMessage& titleMessage, WContainerWidget *parent)
  : WContainerWidget(parent),
    titleMessage_(titleMessage),
    titleChanged_(false)
{ 
  title_ = titleMessage_.value();
}

void WGroupBox::setTitle(const std::string title)
{
  titleMessage_ = WMessage();
  title_ = title;
  titleChanged_ = true;
  repaint();
}

void WGroupBox::setTitleMessage(const WMessage& message)
{
  setTitle(message.value());
  titleMessage_ = message;
}

void WGroupBox::updateDom(DomElement& element,  bool all)
{
  if (titleChanged_) {
    DomElement *legend
      = DomElement::getForUpdate(formName() + "l", DomElement::LEGEND);
    legend->setProperty(Wt::PropertyInnerHTML, title_);
    titleChanged_ = false;
  }

  WContainerWidget::updateDom(element, all);

  renderOk();
}

DomElement *WGroupBox::createDomElement()
{
  DomElement *result = DomElement::createNew(DomElement::FIELDSET);
  result->setId(this);

  DomElement *legend = DomElement::createNew(DomElement::LEGEND);
  legend->setId(formName() + "l");
  legend->setProperty(Wt::PropertyInnerHTML, title_);
  result->addChild(legend);

  createDomChildren(*result);

  updateDom(*result, true);

  return result;
}

void WGroupBox::refresh()
{
  if (!titleMessage_.empty()
      && titleMessage_.value() != title_)
    setTitleMessage(WMessage(titleMessage_));

  WContainerWidget::refresh();
}
