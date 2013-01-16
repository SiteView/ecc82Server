/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <iostream>

#include "WText"
#include "WStringUtil"

#include "DomElement.h"

namespace Wt {

WText::WText(WContainerWidget *parent)
  : WInteractWidget(parent),
    formatting_(XHMTLFormatting),
    textChanged_(false)
{
  setInline(true);
}

WText::WText(const WMessage& text, WContainerWidget *parent)
  : WInteractWidget(parent),
    text_(text),
    formatting_(XHMTLFormatting),
    textChanged_(false)
{ }

WLength WText::hintedWidth() const
{
  return WLength(text_.value().length(), WLength::FontEx);
}

void WText::setText(const WMessage& text)
{
  text_ = text;
  textChanged_ = true;
  repaint();
}

void WText::updateDom(DomElement& element, bool all)
{
  if (textChanged_ || all) {
    element.setProperty(Wt::PropertyInnerHTML, formattedText());
    textChanged_ = false;
  }

  WInteractWidget::updateDom(element, all);
}

void WText::setFormatting(Formatting formatting)
{
  formatting_ = formatting;
}

std::string WText::formattedText() const
{
  if (formatting_ == XHMTLFormatting) {
    if (text_.literal())
      return removeScript("<span>" + toUTF8(text_.value()) + "</span>");
    else
      return toUTF8(text_.value()); // trust XML in resource files.
  } else {
    return toUTF8(escapeText(text_.value(), true));
  }
}

DomElement *WText::createDomElement()
{
  DomElement *result = DomElement::createNew(DomElement::SPAN);
  result->setId(this);

  updateDom(*result, true);

  return result;
}

void WText::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::SPAN);
    updateDom(*e, false);
    result.push_back(e);
  }
}

void WText::refresh()
{
  if (text_.refresh()) {
    textChanged_ = true;
    repaint();
  }

  WInteractWidget::refresh();
}

}
