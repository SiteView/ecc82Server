/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WRadioButton"
#include "DomElement.h"
#include "WButtonGroup"

namespace Wt {

WRadioButton::WRadioButton(WContainerWidget *parent)
  : WAbstractToggleButton(parent),
    buttonGroup_(0)
{
  setFormObject(true);
}

WRadioButton::WRadioButton(const WMessage& text, WContainerWidget *parent)
  : WAbstractToggleButton(text, parent),
    buttonGroup_(0)
{
  setFormObject(true);
}

WRadioButton::~WRadioButton()
{
  if(buttonGroup_)
    buttonGroup_->removeButton(this);
}

void WRadioButton::updateDom(DomElement& element, bool all)
{
  WAbstractToggleButton::updateDom(element, all);
}

DomElement *WRadioButton::createDomElement()
{
  DomElement *checkBox = DomElement::createNew(DomElement::INPUT);
  checkBox->setAttribute("type", "radio");
  checkBox->setId(this);
  if (buttonGroup_)
    checkBox->setAttribute("name", buttonGroup_->formName());
  else
    checkBox->setAttribute("name", formName());

  updateDom(*checkBox, true);

  return checkBox;
}

void WRadioButton::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::INPUT);
    updateDom(*e, false);
    result.push_back(e);
  }
}

void WRadioButton::setGroup(WButtonGroup *group)
{
  buttonGroup_ = group;
}

}
