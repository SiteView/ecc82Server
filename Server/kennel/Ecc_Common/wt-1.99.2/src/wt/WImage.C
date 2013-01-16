/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WImage"
#include "DomElement.h"

namespace Wt {

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

WImage::WImage(const std::string imageRef, const WMessage& altText,
	       WContainerWidget *parent)
  : WInteractWidget(parent),
    altText_(altText),
    imageRef_(imageRef),
    resource_(0),
    altTextChanged_(false),
    imageRefChanged_(false)
{ }

WImage::WImage(WResource *resource, const WMessage& altText,
	       WContainerWidget *parent)
  : WInteractWidget(parent),
    altText_(altText),
    resource_(resource),
    altTextChanged_(false),
    imageRefChanged_(false)
{
  resource_->dataChanged.connect(SLOT(this, WImage::resourceChanged));
  imageRef_ = resource_->generateUrl();
}

void WImage::setResource(WResource *resource)
{
  resource_ = resource;
  resource_->dataChanged.connect(SLOT(this, WImage::resourceChanged));
  imageRef_ = resource_->generateUrl();

  repaint();
}

void WImage::setAlternateText(const WMessage& text)
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
    //element.setAttribute("alt", altText_.value());
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

}
