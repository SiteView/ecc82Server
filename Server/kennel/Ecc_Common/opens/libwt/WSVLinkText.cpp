/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <iostream>

#include "WSVLinkText"
#include "DomElement.h"

#include <iostream>
//#include "WSVLinkText"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WSVLinkText::slots_[] = { WSlot_() };
WSignal_ WSVLinkText::signals_[] = { WSignal_() };

void WSVLinkText::buildSlotMap(WSlotMap& result)
{
  WInteractWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WSVLinkText::buildSignalMap(WSignalMap& result)
{
  WInteractWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WSVLinkText::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WInteractWidget::triggerSlot(sender, slot, args);
}
bool WSVLinkText::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WInteractWidget::undoTriggerSlot(slot, args);
}


WSVLinkText::WSVLinkText(WContainerWidget *parent)
:WText(parent)
{
	
	initStyle("fontlink","fontlinover");
}


WSVLinkText::WSVLinkText(const std::string text, WContainerWidget *parent)
:WText(text,parent)
{ 
	
	text_=text;
	initStyle("fontlink","fontlinover");

}

WSVLinkText::WSVLinkText(const WMessage& message, WContainerWidget *parent)
:WText(message,parent) 
{
	initStyle("fontlink","fontlinover");

}

void WSVLinkText::initStyle(const std::string normalclass,const std::string overclass)
{
	
	//鼠标设置成手
	// tip
	//变色
	//u
	//deco
	//decorationStyle().setCursor(WCssDecorationStyle::Pointer);
	setStyleClass(normalclass);
	sprintf( contextmenu_,
		" onmouseover=\"this.className='%s'\"  onmouseout=\"this.className='%s'\""
		,overclass.c_str(),normalclass.c_str());

}





void WSVLinkText::updateDom(DomElement& element, bool all)
{

	WText::updateDom(element,all);
}

static void replace(std::string& s, char c, std::string r)
{
  unsigned p = 0;

  while ((p = s.find(c, p)) != std::string::npos) {
    s.replace(p, 1, r);
    p += r.length();
  }
}



DomElement *WSVLinkText::createDomElement()
{
  DomElement *result = DomElement::createNew(DomElement::SPAN);
  result->setId(this);
  updateDom(*result, true);
  result -> contextmenu_ = contextmenu_;
//  OutputDebugString("----------dfdf-------------");
  return result;
}

void WSVLinkText::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::SPAN);
    updateDom(*e, false);
    result.push_back(e);
  }
}

void WSVLinkText::refresh()
{
  if (!message_.empty()
      && (message_.value() != text_))
    setMessage(WMessage(message_));

  WInteractWidget::refresh();
}


void WSVLinkText::setSelfJs(const std::string selfstring)
{
	strcat(contextmenu_,selfstring.c_str());
}