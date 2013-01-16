/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WInteractWidget"

#include <iostream>
//#include "WInteractWidget"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include "DomElement.h"


#include <WSlotInstance_>

WSlot_ WInteractWidget::slots_[] = { WSlot_("gotClick()", false), WSlot_() };
WSignal_ WInteractWidget::signals_[] = { WSignal_("keyWentDown(int)", true), WSignal_("keyPressed(int)", true), WSignal_("keyWentUp(int)", true), WSignal_("clicked()", true), WSignal_("doubleClicked()", true), WSignal_("mouseWentDown()", true), 
		WSignal_("mouseWentUp()", true), WSignal_("mouseWentOut()", true), WSignal_("mouseWentOver()", true), WSignal_("mouseMoved()", true), WSignal_("clicked(void *)", true), WSignal_("redicted()", true),  WSignal_("changed()", true), WSignal_("contextmenu()",true), WSignal_() };

void WInteractWidget::buildSlotMap(WSlotMap& result)
{
  WWebWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WInteractWidget::buildSignalMap(WSignalMap& result)
{
  WWebWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

void WInteractWidget::keyWentDown(int arg0)
{
  void *args[1] = {(void *)(&arg0)};  triggerSignal(signals_ + 0, args);
}

void WInteractWidget::keyPressed(int arg0)
{
  void *args[1] = {(void *)(&arg0)};  triggerSignal(signals_ + 1, args);
}

void WInteractWidget::keyWentUp(int arg0)
{
  void *args[1] = {(void *)(&arg0)};  triggerSignal(signals_ + 2, args);
}

void WInteractWidget::clicked()
{
  void **args = 0;
  triggerSignal(signals_ + 3, args);
}

void WInteractWidget::doubleClicked()
{
  void **args = 0;
  triggerSignal(signals_ + 4, args);
}

void WInteractWidget::mouseWentDown()
{
  void **args = 0;
  triggerSignal(signals_ + 5, args);
}

void WInteractWidget::mouseWentUp()
{
  void **args = 0;
  triggerSignal(signals_ + 6, args);
}

void WInteractWidget::mouseWentOut()
{
  void **args = 0;
  triggerSignal(signals_ + 7, args);
}

void WInteractWidget::mouseWentOver()
{
  void **args = 0;
  triggerSignal(signals_ + 8, args);
}

void WInteractWidget::mouseMoved()
{
  void **args = 0;
  triggerSignal(signals_ + 9, args);
}

void WInteractWidget::clicked(void * arg0)
{
  void *args[1] = {(void *)(&arg0)};  triggerSignal(signals_ + 10, args);
}

void WInteractWidget::contextmenu()
{
	void **args = 0;
	triggerSignal(signals_ + 11, args);
}


bool WInteractWidget::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 0) {
    sender_ = sender;    gotClick();
    return true;
  }
  return WWebWidget::triggerSlot(sender, slot, args);
}
bool WInteractWidget::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WWebWidget::undoTriggerSlot(slot, args);
}


WInteractWidget::WInteractWidget(WContainerWidget *parent)
  : WWebWidget(parent),
    data_(0)
{
	memset(contextmenu_,0,JAVATEXT);
}

void WInteractWidget::setData(void *data)
{
  data_ = data;
}

void WInteractWidget::updateDom(DomElement& element, bool all)
{

	if(strlen(contextmenu_)>0)
	{
		element.contextmenu_=contextmenu_;
	}

	//element.asHTML();
  updateSignalConnection(element, SIGNAL(keyWentDown(int)), "keydown",
			 "event.keyCode != 0",
			 all, "keyCode");
  updateSignalConnection(element, SIGNAL(keyPressed(int)), "keypress",
			 "event.keyCode != 0",
			 all, "keyCode");
  updateSignalConnection(element, SIGNAL(keyWentUp(int)), "keyup",
			 "event.keyCode != 0",
			 all, "keyCode");

  const WSignalInstance_ *clickData = getSignal(SIGNAL(clicked(void *)));
  if (isConnected(clickData))
    connect(this, SIGNAL(clicked()), this, SLOT(gotClick()));

  updateSignalConnection(element, SIGNAL(clicked()), "click", all);
  updateSignalConnection(element, SIGNAL(redicted()), "redict", all);
  updateSignalConnection(element, SIGNAL(changed()), "change", all);

  updateSignalConnection(element, SIGNAL(doubleClicked()), "dblclick", all);
  updateSignalConnection(element, SIGNAL(mouseWentDown()), "mousedown", all);
  updateSignalConnection(element, SIGNAL(mouseMoved()), "mousemove", all);
  updateSignalConnection(element, SIGNAL(mouseWentOut()), "mouseout", all);
  updateSignalConnection(element, SIGNAL(mouseWentOver()), "mouseover", all);
  updateSignalConnection(element, SIGNAL(mouseWentUp()), "mouseup", all);
  updateSignalConnection(element, SIGNAL(contextmenu()), "contextmenu", all);

  WWebWidget::updateDom(element, all);
}

void WInteractWidget::gotClick()
{
  emit clicked(data_);
}
