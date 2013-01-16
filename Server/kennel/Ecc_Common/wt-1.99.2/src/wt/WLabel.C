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

namespace Wt {

WLabel::WLabel(WContainerWidget *parent)
  : WInteractWidget(parent),
    buddy_(0),
    text_(0),
    image_(0),
    buddyChanged_(false)
{ }

WLabel::WLabel(const WMessage& text, WContainerWidget *parent)
  : WInteractWidget(parent),
    buddy_(0),
    image_(0),
    buddyChanged_(false)
{ 
  text_ = new WText(text);
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
    buddy_->setLabel(this);

  buddyChanged_ = true;
  repaint();
}

void WLabel::setText(const WMessage& text)
{
  if (!text_) {
    text_ = new WText(text, 0);
    text_->setParent(this);
  } else
    text_->setText(text);
}

const WMessage& WLabel::text() const
{
  if (text_)
    return text_->text();
  else
    assert(false);
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

}
