/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WLabel"
#include "DomElement.h"
#include "WText"
#include "WImage"
#include "WFormWidget"

#include <iostream>
//#include "WLabel"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WLabel::slots_[] = { WSlot_() };
WSignal_ WLabel::signals_[] = { WSignal_() };

void WLabel::buildSlotMap(WSlotMap& result)
{
  WInteractWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WLabel::buildSignalMap(WSignalMap& result)
{
  WInteractWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WLabel::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WInteractWidget::triggerSlot(sender, slot, args);
}
bool WLabel::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WInteractWidget::undoTriggerSlot(slot, args);
}


WLabel::WLabel(WContainerWidget *parent)
  : WInteractWidget(parent),
    buddy_(0),
    text_(0),
    image_(0),
    buddyChanged_(false)
{ }

WLabel::WLabel(const std::string text, WContainerWidget *parent)
  : WInteractWidget(parent),
    buddy_(0),
    image_(0),
    buddyChanged_(false)
{ 
  text_ = new WText(text);
  text_->setParent(this);
}

WLabel::WLabel(const WMessage& message, WContainerWidget *parent)
  : WInteractWidget(parent),
    buddy_(0),
    image_(0),
    buddyChanged_(false)
{ 
  text_ = new WText(message);
  text_->setParent(this);
}

WLabel::WLabel(WImage *image, WContainerWidget *parent)
  : WInteractWidget(parent),
    buddy_(0),
    text_(0),
    buddyChanged_(false)
{ 
  image_ = image;
  image_->setParent(this);
}

WLabel::~WLabel()
{
  setBuddy(0);
}

void WLabel::setBuddy(WFormWidget *buddy)
{
  if (buddy_)
    buddy_->setLabel(0);

  buddy_ = buddy;
  if (buddy_)
  {
    buddy_->setLabel(this);
    //this->setParent(buddy_);
  }

  buddyChanged_ = true;
  repaint();
}

void WLabel::setText(const std::string text)
{
  if (!text_) {
    text_ = new WText(text, 0);
    text_->setParent(this);
  } else
    text_->setText(text);
}

const std::string WLabel::text() const
{
  if (text_)
    return text_->text();
  else
    return std::string();
}

void WLabel::setImage(WImage *image)
{
  if (image_)
    delete image_;
  image_ = image;
  image_->setParent(this);
}

void WLabel::updateDom(DomElement& element, bool all)
{
  if (buddyChanged_ || all) {
    if (buddy_)
      element.setAttribute("for", buddy_->formName());

    buddyChanged_ = false;
  }

  WInteractWidget::updateDom(element, all);

  renderOk();
}

DomElement *WLabel::createDomElement()
{
  DomElement *result = DomElement::createNew(DomElement::LABEL);
  result->setId(this);

  updateDom(*result, true);

  if (text_)
    result->addChild(((WWebWidget *)text_)->createDomElement());

  if (image_)
    result->addChild(((WWebWidget *)image_)->createDomElement());

  return result;
}

void WLabel::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::LABEL);
    updateDom(*e, false);
    result.push_back(e);
  }

  if (text_)
    ((WWebWidget *)text_)->getDomChanges(result);
  if (image_)
    ((WWebWidget *)image_)->getDomChanges(result);
}
