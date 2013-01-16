/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WPushButton"
#include "WStringUtil"

#include "DomElement.h"

namespace Wt {

WPushButton::WPushButton(WContainerWidget *parent)
  : WFormWidget(parent),
    textChanged_(false)
{ }

WPushButton::WPushButton(const WMessage& text, WContainerWidget *parent)
  : WFormWidget(parent),
    text_(text),
    textChanged_(false)
{ }

void WPushButton::setText(const WMessage& text)
{
  text_ = text;
  textChanged_ = true;

  repaint();
}

void WPushButton::updateDom(DomElement& element, bool all)
{
  if (textChanged_ || all) {
    element
      .setProperty(Wt::PropertyInnerHTML,
		   text_.literal()
		   ? toUTF8(escapeText(text_.value()))
		   : toUTF8(text_.value()));
    textChanged_ = false;
  }

  WFormWidget::updateDom(element, all);
}

DomElement *WPushButton::createDomElement()
{
  DomElement *result = DomElement::createNew(DomElement::BUTTON);
  result->setAttribute("type", "button"); // default button
  result->setId(this);

  updateDom(*result, true);

  return result;
}

void WPushButton::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::INPUT);
    updateDom(*e, false);
    result.push_back(e);
  }
}

void WPushButton::refresh()
{
  if (text_.refresh()) {
    textChanged_ = true;
    repaint();
  }

  WFormWidget::refresh();
}

}
