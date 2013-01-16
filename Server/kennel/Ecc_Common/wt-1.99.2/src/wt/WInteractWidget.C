/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WInteractWidget"
#include "WApplication"
#include "WStringUtil"

#include "DomElement.h"

namespace Wt {

WInteractWidget::WInteractWidget(WContainerWidget *parent)
  : WWebWidget(parent),
    keyWentDown(this),
    keyPressed(this),
    keyWentUp(this),
    clicked(this),
    doubleClicked(this),
    mouseWentDown(this),
    mouseWentUp(this),
    mouseWentOut(this),
    mouseWentOver(this),
    mouseMoved(this),
    enterPressed(this)
{ 
}

void WInteractWidget::updateDom(DomElement& element, bool all)
{

  if (all
      || keyWentDown.needUpdate()
      || enterPressed.needUpdate()) {
    std::vector<DomElement::EventAction> actions;
    
    if (enterPressed.isConnected()) {
      actions.push_back
	(DomElement::EventAction("(event.which && event.which == 13)"
				 "|| (event.keyCode && event.keyCode == 13)",
				 enterPressed.javaScript(),
				 enterPressed.encodeCmd(),
				 enterPressed.isExposedSignal()));
    }

    if (keyWentDown.isConnected()) {
      actions.push_back
	(DomElement::EventAction(std::string(),
				 keyWentDown.javaScript(),
				 keyWentDown.encodeCmd(),
				 keyWentDown.isExposedSignal()));
    }

    if (!actions.empty())
      element.setEvent("keydown", actions);
    else if (all)
      element.setEvent("keydown", "", "");
  }

  updateSignalConnection(element, keyPressed, "keypress", all);
  updateSignalConnection(element, keyWentUp, "keyup", all);

  updateSignalConnection(element, clicked, "click", all);
  updateSignalConnection(element, doubleClicked, "dblclick", all);
  updateSignalConnection(element, mouseWentDown, "mousedown", all);
  updateSignalConnection(element, mouseMoved, "mousemove", all);
  updateSignalConnection(element, mouseWentOut, "mouseout", all);
  updateSignalConnection(element, mouseWentOver, "mouseover", all);
  updateSignalConnection(element, mouseWentUp, "mouseup", all);

  WWebWidget::updateDom(element, all);
}

void WInteractWidget::setDraggable(const std::string mimeType,
				   WWidget *dragWidget, bool isDragWidgetOnly,
				   WObject *sourceObject)
{
  if (dragWidget == 0)
    dragWidget = this;

  if (isDragWidgetOnly) {
    dragWidget->hide();
    //addChild(dragWidget);
  }

  setAttributeValue("dmt", widen(mimeType));
  setAttributeValue("dwid", widen(dragWidget->formName()));
  setAttributeValue("dsid", widen(wApp->encodeObject(sourceObject
						     ? sourceObject
						     : this)));

  dragSlot_.setJavascript("dragStart(this, event);");
  mouseWentDown.connect(dragSlot_);
}

}
