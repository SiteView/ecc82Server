/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <iostream>

#include "WContainerWidget"
#include "DomElement.h"
#include <Algorithm>



#include <iostream>
//#include "WContainerWidget"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

using namespace Wt;

WSlot_ WContainerWidget::slots_[] = { WSlot_() };
WSignal_ WContainerWidget::signals_[] = { WSignal_() };

void  WContainerWidget::buildSlotMap(WSlotMap& result)
{
  WInteractWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WContainerWidget::buildSignalMap(WSignalMap& result)
{
  WInteractWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WContainerWidget::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WInteractWidget::triggerSlot(sender, slot, args);
}
bool WContainerWidget::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WInteractWidget::undoTriggerSlot(slot, args);
}

WContainerWidget::WContainerWidget(WContainerWidget *parent)
  : WInteractWidget(parent),
    contentAlignment_(AlignLeft),
    contentAlignmentChanged_(false),
    paddingsChanged_(false)
{ 
  setInline(false);
}

void WContainerWidget::addWidget(WWidget *widget)
{
  if (widget->parent() == 0) {
    addedChildren_.push_back(widget);
    contentAlignmentChanged_ = true; // children margins hacks
    repaint();

    widget->setParent(this);
  } else {
    std::cerr << "Warning: WContainerWidget: ignoring twice addWidget!"
	      << std::endl;
  }
}

void WContainerWidget::removeWidget(WWidget *widget)
{
  widget->setParent((WWidget *)0);
  repaint();
}


void WContainerWidget::clear()
{
  while (!children().empty())
    delete children().front();
}

void WContainerWidget::removeChild(WWidget *child)
{
  std::vector<WWidget *>::iterator i
	  = std::find(addedChildren_.begin(), addedChildren_.end(), child);

  if (i != addedChildren_.end())
    addedChildren_.erase(i);

  WWebWidget::removeChild(child);
}

void WContainerWidget::setContentAlignment(HorizontalAlignment ha)
{
  contentAlignment_ = ha;
  contentAlignmentChanged_ = true;
  repaint();
}

void WContainerWidget::setPadding(WLength length, int sides)
{
  if (sides & Top)
    padding_[0] = length;
  if (sides & Right)
    padding_[1] = length;
  if (sides & Bottom)
    padding_[2] = length;
  if (sides & Left)
    padding_[3] = length;

  paddingsChanged_ = true;
  repaint();
}

WLength WContainerWidget::padding(Side side) const
{
  switch (side) {
  case Top:
    return padding_[0];
  case Right:
    return padding_[1];
  case Bottom:
    return padding_[2];
  case Left:
    return padding_[3];
  default:
    std::cerr << "Error: WContainerWidget::padding(Side) with illegal side."
	      << std::endl;
    return WLength();
  }
}

void WContainerWidget::updateDom(DomElement& element, bool all)
{
  if (contentAlignmentChanged_ || all) {
    switch(contentAlignment_) {
    case AlignLeft:
      //if (contentAlignmentChanged_)
	element.setProperty(PropertyStyleTextAlign, "left");
      break;
    case AlignRight:
      element.setProperty(PropertyStyleTextAlign, "right");
      break;
    case AlignCenter:
      element.setProperty(PropertyStyleTextAlign, "center");
      break;
    case AlignJustify:
      element.setProperty(PropertyStyleTextAlign, "justify");
    }

    /*
     * Welcome to CSS hell.
     *
     * Apparently, the text-align property only applies to inline elements.
     * To center non-inline children, the standard says to set its left and
     * right margin to 'auto'.
     *
     * I assume the same applies for aligning to the right ?
     */
    for (unsigned i = 0; i < children().size(); ++i) {
      WWidget *child = children()[i];

      if (!child->isInline()) {
	if (contentAlignment_ == AlignCenter) {
	  if (!child->margin(Left).isAuto())
	    child->setMargin(WLength(), Left);
	  if (!child->margin(Right).isAuto())
	    child->setMargin(WLength(), Right);
	}
	if (contentAlignment_ == AlignRight) {
	  if (!child->margin(Left).isAuto())
	    child->setMargin(WLength(), Left);
	}
      }
    }

    contentAlignmentChanged_ = false;
  }

  if (paddingsChanged_ || all) {
    if (paddingsChanged_
	|| !padding_[0].isAuto()
	|| !padding_[1].isAuto()
	|| !padding_[2].isAuto()
	|| !padding_[3].isAuto()) {
      if ((padding_[0] == padding_[1])
	  && (padding_[0] == padding_[2])
	  && (padding_[0] == padding_[3]))
	element.setProperty(PropertyStylePadding,
			    padding_[0].cssText());
      else
	element.setProperty(PropertyStylePadding,
			    padding_[0].cssText()
			    + " " + padding_[1].cssText()
			    + " " + padding_[2].cssText()
			    + " " + padding_[3].cssText());
    }

    paddingsChanged_ = false;
  }

  WInteractWidget::updateDom(element, all);

  for (unsigned i = 0; i < addedChildren_.size(); ++i) {
    DomElement *c = addedChildren_[i]->webWidget()->createSDomElement();
    element.addChild(c);
  }
  addedChildren_.clear();
}

DomElement *
WContainerWidget::createDomElement()
{
  DomElement *result;
  
  result = DomElement::createNew(isInline() ?
				 DomElement::SPAN : DomElement::DIV);
  result->setId(this);

  addedChildren_.clear();
  updateDom(*result, true);

  createDomChildren(*result);

  renderOk();

  return result;
}

void WContainerWidget::createDomChildren(DomElement& parent)
{
  for (unsigned i = 0; i < children().size(); ++i) {
    DomElement *c = children()[i]->webWidget()->createSDomElement();
    parent.addChild(c);
  }

  addedChildren_.clear();
}

void WContainerWidget::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e
      = DomElement::getForUpdate(this, isInline() ? DomElement::SPAN
				 : DomElement::DIV);
    updateDom(*e, false);
    result.push_back(e);
  }

  for (unsigned i = 0; i < children().size(); ++i) {
    children()[i]->webWidget()->getSDomChanges(result);
  }

  renderOk();
}
