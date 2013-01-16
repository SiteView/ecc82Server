/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <iostream>
#include <algorithm>

#include "WContainerWidget"
#include "DomElement.h"

using std::find;

namespace Wt {

WContainerWidget::WContainerWidget(WContainerWidget *parent)
  : WInteractWidget(parent),
    contentAlignment_(AlignLeft),
    addedChildren_(0),
    contentAlignmentChanged_(false),
    paddingsChanged_(false)
{ 
  setInline(false);

  children_ = new std::vector<WWidget *>;
}

void WContainerWidget::addWidget(WWidget *widget)
{
  if (widget->parent() == 0) {
    if (!addedChildren_)
      addedChildren_ = new std::vector<WWidget *>;

    addedChildren_->push_back(widget);
    contentAlignmentChanged_ = true; // children margins hacks
    repaint();

    widget->setParent(this);
  } else {
    std::cerr << "Warning: WContainerWidget: ignoring twice addWidget!"
	      << std::endl;
  }
}

void WContainerWidget::insertWidget(WWidget *widget, WWidget *before)
{
  if (before->parent() != this) {
    std::cerr << "WContainerWidget::insertWidget: before must be already "
      "in this container!" << std::endl;
    return;
  }

  if (widget->parent() == 0) {
    std::vector<WWidget *>::iterator i
      = std::find(children_->begin(), children_->end(), before);
    children_->insert(i, widget);
    contentAlignmentChanged_ = true; // children margins hacks
    repaint();

    before->addNewSibling(widget);

    widget->setParent(this);
  } else {
    std::cerr << "Warning: WContainerWidget: ignoring twice add widget!"
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
  if (addedChildren_) {
    std::vector<WWidget *>::iterator i
      = find(addedChildren_->begin(), addedChildren_->end(), child);

    if (i != addedChildren_->end())
      addedChildren_->erase(i);
  }

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
      if (contentAlignmentChanged_)
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

  bool wasEmpty 
    = (((addedChildren_ ? addedChildren_->size() : 0) == children_->size())
       && (otherImpl_ ? (otherImpl_->scriptFunctions_
			? otherImpl_->scriptFunctions_->empty() : true)
	   : true));
  element.setWasEmpty(wasEmpty);

  if (addedChildren_) {
    for (unsigned i = 0; i < addedChildren_->size(); ++i) {
      DomElement *c = (*addedChildren_)[i]->webWidget()->createSDomElement();
      element.addChild(c);
    }

    delete addedChildren_;
    addedChildren_ = 0;
  }
}

DomElement *
WContainerWidget::createDomElement()
{
  DomElement *result;
  
  result = DomElement::createNew(isInline() ?
				 DomElement::SPAN : DomElement::DIV);
  result->setId(this);

  if (addedChildren_) {
    delete addedChildren_;
    addedChildren_ = 0;
  }
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

  if (addedChildren_) {
    delete addedChildren_;
    addedChildren_ = 0;
  }
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

  renderOk();
}

}
