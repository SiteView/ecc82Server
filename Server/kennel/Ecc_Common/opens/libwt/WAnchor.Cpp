/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WAnchor"
#include "WLabel"
#include "DomElement.h"

#include <iostream>
//#include "WAnchor"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WAnchor::slots_[] = { WSlot_() };
WSignal_ WAnchor::signals_[] = { WSignal_() };

void WAnchor::buildSlotMap(WSlotMap& result)
{
  WInteractWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WAnchor::buildSignalMap(WSignalMap& result)
{
  WInteractWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WAnchor::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WInteractWidget::triggerSlot(sender, slot, args);
}
bool WAnchor::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WInteractWidget::undoTriggerSlot(slot, args);
}

WAnchor::WAnchor(WContainerWidget *parent)
  : WInteractWidget(parent),
    refChanged_(false)
{
  label_ = new WLabel();
  label_->setParent(this);
}

WAnchor::WAnchor(const std::string ref, WContainerWidget *parent)
  : WInteractWidget(parent),
    ref_(ref),
    refChanged_(false)
{
  label_ = new WLabel();
  label_->setParent(this);
}

WAnchor::WAnchor(const std::string ref, const std::string text,
		 WContainerWidget *parent)
  : WInteractWidget(parent),
    ref_(ref),
    refChanged_(false)
{ 
  label_ = new WLabel(text, 0);
  label_->setParent(this);
}

WAnchor::WAnchor(const std::string ref, const WMessage& message,
		 WContainerWidget *parent)
  : WInteractWidget(parent),
    ref_(ref),
    refChanged_(false)
{ 
  label_ = new WLabel(message, 0);
  label_->setParent(this);
}

WAnchor::WAnchor(const std::string ref, WImage *image,
		 WContainerWidget *parent)
  : WInteractWidget(parent),
    ref_(ref),
    refChanged_(false)
{ 
  label_ = new WLabel(image, 0);
  label_->setParent(this);
}


void WAnchor::setRef(const std::string ref)
{
  ref_ = ref;
  refChanged_ = true;
  repaint();
}

void WAnchor::updateDom(DomElement& element, bool all)
{
  if (refChanged_ || all) {
    element.setAttribute("href", ref_);
    refChanged_ = false;
  }

  WInteractWidget::updateDom(element, all);
}

DomElement *WAnchor::createDomElement()
{
  DomElement *result = DomElement::createNew(DomElement::A);
  result->setId(this);

  updateDom(*result, true);

  result->addChild(((WWebWidget *)label_)->createDomElement());

  return result;
}

void WAnchor::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::A);
    updateDom(*e, false);
    result.push_back(e);
  }

  if (label_)
    ((WWebWidget *)label_)->getDomChanges(result);
}
