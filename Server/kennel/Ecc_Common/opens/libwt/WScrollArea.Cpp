/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "WScrollArea"
#include "WScrollBar"
#include "DomElement.h"

#include <iostream>
//#include "WScrollArea"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WScrollArea::slots_[] = { WSlot_() };
WSignal_ WScrollArea::signals_[] = { WSignal_() };

void WScrollArea::buildSlotMap(WSlotMap& result)
{
  WWebWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WScrollArea::buildSignalMap(WSignalMap& result)
{
  WWebWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WScrollArea::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WWebWidget::triggerSlot(sender, slot, args);
}
bool WScrollArea::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WWebWidget::undoTriggerSlot(slot, args);
}


WScrollArea::WScrollArea(WContainerWidget *parent)
  : WWebWidget(parent),
    widget_(0),
    widgetChanged_(false),
    scrollBarChanged_(false),
    scrollBarPolicy_(ScrollBarAsNeeded),
    scrollBarPolicyChanged_(false)
{
  setInline(false);

  horizontalScrollBar_ = new WScrollBar(this, WScrollBar::Horizontal);
  verticalScrollBar_ = new WScrollBar(this, WScrollBar::Vertical);
}

WScrollArea::~WScrollArea()
{
  delete horizontalScrollBar_;
  delete verticalScrollBar_;
}

void WScrollArea::scrollBarChanged()
{
  scrollBarChanged_ = true;
  repaint();
}

void WScrollArea::setWidget(WWidget *widget)
{
  delete widget_;

  widget_ = widget;
  widgetChanged_ = true;
  repaint();

  if (widget)
    widget->setParent(this);
}

WWidget *WScrollArea::takeWidget()
{
  WWidget *result = widget_;
  widget_ = 0;

  setWidget(0);

  if (result)
    result->setParent(0);

  return result;
}

void WScrollArea::setScrollBarPolicy(ScrollBarPolicy policy)
{
  scrollBarPolicy_ = policy;
  scrollBarPolicyChanged_ = true;
}

void WScrollArea::updateDom(DomElement& element, bool all)
{
  if (widgetChanged_ || all) {
    if (widget_)
      element.addChild(widget_->webWidget()->createDomElement());

    widgetChanged_ = false;
  }

  if (scrollBarChanged_ || all) {
    if ((horizontalScrollBar_->tiesChanged_)
	|| (verticalScrollBar_->tiesChanged_)) {
      horizontalScrollBar_->tiesChanged_ = true;
      verticalScrollBar_->tiesChanged_ = true;
    }
    horizontalScrollBar_->updateDom(element, all);
    verticalScrollBar_->updateDom(element, all);

    scrollBarChanged_ = false;
  }

  if (scrollBarPolicyChanged_ || all) {
    switch (scrollBarPolicy_) {
    case ScrollBarAsNeeded:
      element.setProperty(Wt::PropertyStyleOverflowX, "auto");
      break;
    case ScrollBarAlwaysOff:
      element.setProperty(Wt::PropertyStyleOverflowX, "hidden");
      break;
    case ScrollBarAlwaysOn:
      element.setProperty(Wt::PropertyStyleOverflowX, "scroll");
      break;
    }
    //result->setProperty(Wt::PropertyStyleOverflowY, "auto");

    scrollBarPolicyChanged_ = false;
  }    

  WWebWidget::updateDom(element, all);

  renderOk();
}

DomElement *WScrollArea::createDomElement()
{
  DomElement *result = DomElement::createNew(DomElement::DIV);
  result->setId(this, true);

  updateDom(*result, true);

  return result;
}

void WScrollArea::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::DIV);
    updateDom(*e, false);
    result.push_back(e);
  }

  if (widget_)
    widget_->webWidget()->getSDomChanges(result);
}
