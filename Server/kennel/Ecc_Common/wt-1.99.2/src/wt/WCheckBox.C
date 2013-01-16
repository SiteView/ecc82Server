/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WCheckBox"
#include "DomElement.h"

namespace Wt {

WCheckBox::WCheckBox(WContainerWidget *parent)
  : WAbstractToggleButton(parent)
{
  setFormObject(true);
}

WCheckBox::WCheckBox(const WMessage& text, WContainerWidget *parent)
  : WAbstractToggleButton(text, parent)
{
  setFormObject(true);
}

DomElement *WCheckBox::createDomElement()
{
  DomElement *checkBox = DomElement::createNew(DomElement::INPUT);
  checkBox->setAttribute("type", "checkbox");
  checkBox->setId(this, true);

  updateDom(*checkBox, true);

  return checkBox;
}

void WCheckBox::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::INPUT);
    updateDom(*e, false);
    result.push_back(e);
  }
}

}
