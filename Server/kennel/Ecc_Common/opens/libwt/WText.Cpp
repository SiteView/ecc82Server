/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <iostream>

#include "WText"
#include "DomElement.h"

#include <iostream>
//#include "WText"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WText::slots_[] = { WSlot_() };
WSignal_ WText::signals_[] = { WSignal_() };

void WText::buildSlotMap(WSlotMap& result)
{
  WInteractWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WText::buildSignalMap(WSignalMap& result)
{
  WInteractWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WText::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WInteractWidget::triggerSlot(sender, slot, args);
}
bool WText::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WInteractWidget::undoTriggerSlot(slot, args);
}


WText::WText(WContainerWidget *parent)
  : WInteractWidget(parent),
    formatting_(XHMTLFormatting),
    textChanged_(false)

{
	memset(contextmenu_,0,JAVATEXT);
  setInline(true);
}

WText::WText(const std::string text, WContainerWidget *parent)
  : WInteractWidget(parent),
    text_(text),
    formatting_(XHMTLFormatting),
    textChanged_(false)

{
	text_=text;
	memset(contextmenu_,0,JAVATEXT);
}

WText::WText(const WMessage& message, WContainerWidget *parent)
  : WInteractWidget(parent),
    message_(message),
    formatting_(XHMTLFormatting),
    textChanged_(false)

{

	memset(contextmenu_,0,JAVATEXT);
  text_ = message.value();
}

WLength WText::hintedWidth() const
{
  return WLength(text_.length(), WLength::FontEx);
}

void WText::setText(const std::string text)
{
  message_ = WMessage();
  text_ = text;
  textChanged_ = true;
  repaint();
}

void WText::setMessage(const WMessage& message)
{
  setText(message.value());
  message_ = message;
}

void WText::updateDom(DomElement& element, bool all)
{
  if (textChanged_ || all) {
    element.setProperty(Wt::PropertyInnerHTML, formattedText());
    textChanged_ = false;
  }

  element.contextmenu_ = contextmenu_;
//  element.handtext = handtext;
//  element.bkcolor1_ = bkcolor1_;
//  element.bkcolor2_ = bkcolor2_;

  WInteractWidget::updateDom(element, all);
}

static void replace(std::string& s, char c, std::string r)
{
  unsigned p = 0;

  while ((p = s.find(c, p)) != std::string::npos) {
    s.replace(p, 1, r);
    p += r.length();
  }
}

void WText::setFormatting(Formatting formatting)
{
  formatting_ = formatting;
}

std::string WText::formattedText() const
{
  if (formatting_ == XHMTLFormatting)
  {
      std::string result = text_;
    //replace(result, '\\', "\\\\");
    return result;
  }
  else {
    std::string result = text_;
    replace(result, '\\', "&#92;");
    replace(result, '\'', "&#39;");
    replace(result, '&', "&amp;");
    replace(result, '<', "&lt;");
    replace(result, '>', "&gt;");
    replace(result, '"', "&quot;");

    return result;
  }
}

DomElement *WText::createDomElement()
{
  DomElement *result = DomElement::createNew(DomElement::SPAN);
  result->setId(this);

  updateDom(*result, true);

  //result -> handtext = handtext;
  result -> contextmenu_ = contextmenu_;

//  result -> bkcolor1_ = bkcolor1_;
 // result -> bkcolor2_ = bkcolor2_;

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
  if (!message_.empty()
      && (message_.value() != text_))
    setMessage(WMessage(message_));

  WInteractWidget::refresh();
}
