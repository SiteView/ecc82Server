/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WBreak"
#include "DomElement.h"

namespace Wt {

WBreak::WBreak(WContainerWidget *parent)
  : WWebWidget(parent)
{ }

void WBreak::updateDom(DomElement& element, bool all)
{
  WWebWidget::updateDom(element, all);

  renderOk();
}

DomElement *WBreak::createDomElement()
{
  DomElement *result = DomElement::createNew(DomElement::BR);
  result->setId(this);

  updateDom(*result, true);

  return result;
}

void WBreak::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::BR);
    updateDom(*e, false);
    result.push_back(e);
  }
}

}
