/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <boost/lexical_cast.hpp>

#include "WLineEdit"
#include "WStringUtil"

#include "DomElement.h"
#include "CgiParser.h"

namespace Wt {

WLineEdit::WLineEdit(WContainerWidget *parent)
  : WFormWidget(parent),
    textSize_(10),
    echoMode_(Normal),
    contentChanged_(false),
    textSizeChanged_(false),
    echoModeChanged_(false)
{ 
  setInline(true);
  setFormObject(true);
}

WLineEdit::WLineEdit(const std::wstring text, WContainerWidget *parent)
  : WFormWidget(parent),
    content_(text),
    textSize_(10),
    echoMode_(Normal),
    contentChanged_(false),
    textSizeChanged_(false),
    echoModeChanged_(false)
{ 
  setInline(true);
  setFormObject(true);
}

void WLineEdit::setText(const std::wstring text)
{
  content_ = text;
  contentChanged_ = true;
  repaint();
}

void WLineEdit::setTextSize(int chars)
{
  textSize_ = chars;
  textSizeChanged_ = true;
  repaint();
}

void WLineEdit::setEchoMode(EchoMode echoMode)
{
  echoMode_ = echoMode;
  echoModeChanged_ = true;
  repaint();
}

void WLineEdit::updateDom(DomElement& element, bool all)
{
  if (contentChanged_ || all) {
    element.setProperty(Wt::PropertyValue, toUTF8(content_));
    contentChanged_ = false;
  }

  if (echoModeChanged_ || all) {
    element.setAttribute("type", echoMode_ == Normal ? "text" : "password");
    echoModeChanged_ = false;
  }

  if (textSizeChanged_ || all) {
    element.setAttribute("size",
			 boost::lexical_cast<std::string>(textSize_));
    textSizeChanged_ = false;
  }

  WFormWidget::updateDom(element, all);
}

DomElement *WLineEdit::createDomElement()
{
  DomElement *result = DomElement::createNew(DomElement::INPUT);

  result->setId(this, true);

  updateDom(*result, true);

  return result;
}

void WLineEdit::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::INPUT);
    updateDom(*e, false);
    result.push_back(e);
  }
}

void WLineEdit::setFormData(CgiEntry *entry)
{
  content_ = fromUTF8(entry->value());
}

WValidator::State WLineEdit::validate()
{
  if (validator()) {
    int pos;

    return validator()->validate(content_, pos);
  } else
    return WValidator::Valid;
}

}
