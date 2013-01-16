/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WImage"
#include "DomElement.h"

#include <iostream>
//#include "WImage"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WImage::slots_[] = { WSlot_("resourceChanged()", false), WSlot_() };
WSignal_ WImage::signals_[] = { WSignal_() };

void WImage::buildSlotMap(WSlotMap& result)
{
  WInteractWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WImage::buildSignalMap(WSignalMap& result)
{
  WInteractWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WImage::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 0) {
    sender_ = sender;    resourceChanged();
    return true;
  }
  return WInteractWidget::triggerSlot(sender, slot, args);
}
bool WImage::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WInteractWidget::undoTriggerSlot(slot, args);
}


WImage::WImage(WContainerWidget *parent)
  : WInteractWidget(parent),
    resource_(0),
    altTextChanged_(false),
    imageRefChanged_(false)
{ }

WImage::WImage(const std::string imageRef, WContainerWidget *parent)
  : WInteractWidget(parent),
    imageRef_(imageRef),
    resource_(0),
    altTextChanged_(false),
    imageRefChanged_(false)
{ }

WImage::WImage(const std::string imageRef, const std::string altText,
	       WContainerWidget *parent)
  : WInteractWidget(parent),
    altText_(altText),
    imageRef_(imageRef),
    resource_(0),
    altTextChanged_(false),
    imageRefChanged_(false)
{ }

WImage::WImage(WResource *resource, const std::string altText,
	       WContainerWidget *parent)
  : WInteractWidget(parent),
    altText_(altText),
    resource_(resource),
    altTextChanged_(false),
    imageRefChanged_(false)
{
  connect(resource_, SIGNAL(dataChanged()), this, SLOT(resourceChanged()));
  imageRef_ = resource_->generateUrl();
}

void WImage::setAlternateText(const std::string text)
{
  altText_ = text;
  altTextChanged_ = true;

  repaint();
}

void WImage::setImageRef(const std::string ref)
{
  imageRef_ = ref;
  imageRefChanged_ = true;

  repaint();
}

const std::string WImage::imageRef() const
{
  return imageRef_;
}

void WImage::resourceChanged()
{
  setImageRef(resource_->generateUrl());
}

void WImage::updateDom(DomElement& element, bool all)
{
  if (imageRefChanged_ || all) {
    element.setProperty(Wt::PropertySrc, imageRef_);
    imageRefChanged_ = false;
  }

  if (altTextChanged_ || all) {
    //element.setAttribute("alt", altText_);
    altTextChanged_ = false;
  }

  WInteractWidget::updateDom(element, all);

  renderOk();
}

DomElement *WImage::createDomElement()
{
  DomElement *result = DomElement::createNew(DomElement::IMG);
  result->setId(this);

  updateDom(*result, true);

  return result;
}

void WImage::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::IMG);
    updateDom(*e, false);
    result.push_back(e);
  }
}
