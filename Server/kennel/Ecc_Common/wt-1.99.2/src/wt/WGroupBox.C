/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WGroupBox"
#include "WStringUtil"

#include "DomElement.h"

namespace Wt {

WGroupBox::WGroupBox(WContainerWidget *parent)
  : WContainerWidget(parent),
    titleChanged_(false)
{ }

WGroupBox::WGroupBox(const WMessage& title, WContainerWidget *parent)
  : WContainerWidget(parent),
    title_(title),
    titleChanged_(false)
{ }

void WGroupBox::setTitle(const WMessage& title)
{
  title_ = title;
  titleChanged_ = true;
  repaint();
}

void WGroupBox::updateDom(DomElement& element, bool all)
{
  if (titleChanged_) {
    DomElement *legend
      = DomElement::getForUpdate(formName() + "l", DomElement::LEGEND);
    legend->setProperty(Wt::PropertyInnerHTML,
			toUTF8(escapeText(title_.value())));
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
  legend->setProperty(Wt::PropertyInnerHTML,
		      toUTF8(escapeText(title_.value())));
  result->addChild(legend);

  createDomChildren(*result);

  updateDom(*result, true);

  return result;
}

void WGroupBox::refresh()
{
  if (title_.refresh()) {
    titleChanged_ = true;
    repaint();
  }

  WContainerWidget::refresh();
}

}
