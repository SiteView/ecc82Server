/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <iostream>
#include <boost/lexical_cast.hpp>

#include "WWebWidget"
#include "WContainerWidget"
#include "WApplication"
#include "WSignalInstance_"
#include "WSlotInstance_"
#include "DomElement.h"
#include "JavaScript.h"
#include <Algorithm>



#include <iostream>
//#include "WWebWidget"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

using namespace Wt;

WSlot_ WWebWidget::slots_[] = { WSlot_() };
WSignal_ WWebWidget::signals_[] = { WSignal_() };

void WWebWidget::buildSlotMap(WSlotMap& result)
{
  WWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WWebWidget::buildSignalMap(WSignalMap& result)
{
  WWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WWebWidget::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WWidget::triggerSlot(sender, slot, args);
}
bool WWebWidget::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WWidget::undoTriggerSlot(slot, args);
}


WWebWidget::WWebWidget(WContainerWidget *parent)
  : WWidget(),
    positionScheme_(Static),
    floatSide_(None),
    clearSides_(None),
    hidden_(false),
    popup_(false),
    inline_(true),
    decorationStyle_(this),
    verticalAlignment_(AlignBaseline),
    parent_(0),
    geometryChanged_(false),
    marginsChanged_(false),
    hiddenChanged_(false),
    styleClassChanged_(false),
    toolTipChanged_(false),
    renderState_(RenderUpdate),
    stubbed_(false),
    formObject_(false)
{
  for (unsigned i = 0; i < 4; ++i)
    margin_[i] = WLength(0);

  if (parent)
    parent->addWidget(this);
}

void WWebWidget::setFormObject(bool how)
{
  formObject_ = how;
}

void WWebWidget::repaint()
{
  renderState_ = RenderUpdate;
}

void WWebWidget::renderOk()
{
  renderState_ = RenderOk;
}

void WWebWidget::signalConnectionsChanged()
{
  repaint();
}

WWebWidget::~WWebWidget()
{
  while (children_.size())
    delete children_[0];

  for (unsigned i = 0; i < childRemoveChanges_.size(); ++i)
    delete childRemoveChanges_[i];

  setParent(0);
}

void WWebWidget::removeChild(WWidget *w)
{
  std::vector<WWidget *>::iterator i
	  = std::find(children_.begin(), children_.end(), w);
  if (i != children_.end()) {
    DomElement *e = DomElement::getForUpdate(w, DomElement::DIV);
    e->removeFromParent();
    childRemoveChanges_.push_back(e);

    w->webWidget()->parent_ = 0;
    children_.erase(i);
  } else {
    std::cerr << "child disappeared in widget ?" << std::endl;
    exit(1);
  }
}

void WWebWidget::setParent(WWidget *p)
{
  if (parent_)
    parent_->removeChild(this);
  if (p)
    p->addChild(this);
}


WWidget *WWebWidget::parent() const
{
  return parent_;
}

void WWebWidget::setPositionScheme(PositionScheme scheme)
{
  positionScheme_ = scheme;

  if ((scheme == Absolute) || (scheme == Fixed))
    inline_ = false;

  geometryChanged_ = true;
  repaint();
}

void WWebWidget::resize(WLength width, WLength height)
{
  width_ = width;
  height_ = height;

  geometryChanged_ = true;
  repaint();
}

void WWebWidget::setMinimumSize(WLength width, WLength height)
{
  minimumWidth_ = width;
  minimumHeight_ = height;

  geometryChanged_ = true;
  repaint();
}

void WWebWidget::setMaximumSize(WLength width, WLength height)
{
  maximumWidth_ = width;
  maximumHeight_ = height;

  geometryChanged_ = true;
  repaint();
}

void WWebWidget::setFloatSide(Side s)
{
  floatSide_ = s;

  geometryChanged_ = true;
  repaint();
}

WWidget::Side WWebWidget::floatSide() const
{
  return floatSide_;
}

void WWebWidget::setClearSides(int sides)
{
  clearSides_ = sides;

  geometryChanged_ = true;
  repaint();
}

int WWebWidget::clearSides() const
{
  return clearSides_;
}

void WWebWidget::setVerticalAlignment(VerticalAlignment va,
				   WLength length)
{
  verticalAlignment_ = va;
  verticalAlignmentLength_ = length;

  geometryChanged_ = true;
  repaint();
}

void WWebWidget::setOffset(Side s, WLength l)
{
  if (s & Top)
    offsets_[0] = l;
  if (s & Right)
    offsets_[1] = l;
  if (s & Bottom)
    offsets_[2] = l;
  if (s & Left)
    offsets_[3] = l;

  geometryChanged_ = true;
  repaint();
}

WLength WWebWidget::offset(Side s) const
{
  switch (s) {
  case Top:
    return offsets_[0];
  case Right:
    return offsets_[1];
  case Bottom:
    return offsets_[2];
  case Left:
    return offsets_[3];
  default:
    std::cerr << "WWebWidget::offset() with weird offset called?"
	      << std::endl;
    return offsets_[0];
  }
}

int WWebWidget::zIndex() const
{
  if (parent_)
    return parent_->webWidget()->zIndex()
      + (popup_ ? 1 : 0);
  else
    return popup_ ? 1 : 0;
}

void WWebWidget::setInline(bool inl)
{
  inline_ = inl;

  geometryChanged_ = true;
  repaint();
}

void WWebWidget::setPopup(bool popup)
{
  popup_ = popup;

  geometryChanged_ = true;
  repaint();
}

void WWebWidget::setMargin(WLength length, int sides)
{
  if (sides & Top)
    margin_[0] = length;
  if (sides & Right)
    margin_[1] = length;
  if (sides & Bottom)
    margin_[2] = length;
  if (sides & Left)
    margin_[3] = length;

  marginsChanged_ = true;
  repaint();
}

WLength WWebWidget::margin(Side side) const
{
  switch (side) {
  case Top:
    return margin_[0];
  case Right:
    return margin_[1];
  case Bottom:
    return margin_[2];
  case Left:
    return margin_[3];
  default:
    std::cerr << "Error: WWebWidget::margin(Side) with illegal side."
	      << std::endl;
    return WLength();
  }
}

void WWebWidget::setStyleClass(const std::string styleClass)
{
  styleClass_ = styleClass;

  styleClassChanged_ = true;
  repaint();
}

void WWebWidget::setToolTip(const std::string text)
{
  toolTipMessage_ = WMessage();
  toolTip_ = text;
  toolTipChanged_ = true;

  repaint();
}

void WWebWidget::setToolTipMessage(const WMessage& message)
{
  setToolTip(message.value());
  toolTipMessage_ = message;
}

void WWebWidget::setHidden(bool hidden)
{
  hidden_ = hidden;

  hiddenChanged_ = true;
  repaint();
}

void WWebWidget::hide()
{
  wasHidden_ = hidden_;
  setHidden(true);
}

void WWebWidget::show()
{ 
  wasHidden_ = hidden_;
  setHidden(false);
}

void WWebWidget::undoHide()
{
  undoShow();
}

void WWebWidget::undoShow()
{
  setHidden(wasHidden_);
}

void WWebWidget::addChild(WWidget *child)
{
  if (child->parent() == 0) {
    children_.push_back(child);
    child->webWidget()->parent_ = this;
  } else {
    std::cerr << "WWebWidget::addChild of child with parent!" << std::endl;
  }
}

void WWebWidget::updateDom(DomElement& element, bool all)
{
  /*
   * determine display
   */
  if (hiddenChanged_ || geometryChanged_ || all) {
    if (!hidden_) {
      if (element.isDefaultInline() != inline_) {
	if (inline_) {
	  if (element.type() == DomElement::TABLE)
	    element.setProperty(PropertyStyleDisplay, "inline-table");
	  else if (element.type() != DomElement::TD)
	    element.setProperty(PropertyStyleDisplay, "inline-block");
	} else {
	  element.setProperty(PropertyStyleDisplay, "block");
	}
      } else if (hiddenChanged_) {
	element.setProperty(PropertyStyleDisplay,
			    inline_ ? "inline" : "block");
      }
    } else
      element.setProperty(PropertyStyleDisplay, "none");

    hiddenChanged_ = false;
  }

  if (geometryChanged_ || all) {
    /*
     * set position
     */
    switch (positionScheme_) {
    case Static:
      break;
    case Relative:
      element.setProperty(PropertyStylePosition, "relative"); break;
    case Absolute:
      element.setProperty(PropertyStylePosition, "absolute"); break;
    case Fixed:
      element.setProperty(PropertyStylePosition, "fixed"); break;
    }

    /*
     * set z-index
     */
    if (popup_)
      element.setProperty(PropertyStyleZIndex,
			  boost::lexical_cast<std::string>(zIndex()));

    /*
     * set float
     */
    switch (floatSide_) {
    case None:
      break;
    case Left:
      element.setProperty(PropertyStyleFloat, "left"); break;
      break;
    case Right:
      element.setProperty(PropertyStyleFloat, "right"); break;
      break;
    default:
      /* illegal values */
      ;
    }

    /*
     * set clear: FIXME: multiple values
     */
    switch (clearSides_) {
    case None:
      break;
    case Left:
      element.setProperty(PropertyStyleClear, "left"); break;
      break;
    case Right:
      element.setProperty(PropertyStyleClear, "right"); break;
      break;
    case Verticals:
      element.setProperty(PropertyStyleClear, "both"); break;
      break;
    default:
      /* illegal values */
      ;
    }

    /*
     * set width & height
     */
    if (!width_.isAuto())
      element.setProperty(PropertyStyleWidth, width_.cssText());
    if (!height_.isAuto())
      element.setProperty(PropertyStyleHeight, height_.cssText());
    if (!minimumWidth_.isAuto() && (minimumWidth_.value() != 0))
      element.setProperty(PropertyStyleMinWidth, minimumWidth_.cssText());
    if (!minimumHeight_.isAuto() && (minimumHeight_.value() != 0))
      element.setProperty(PropertyStyleMinHeight, minimumHeight_.cssText());
    if (!maximumWidth_.isAuto()) // == none
      element.setProperty(PropertyStyleMaxWidth, maximumWidth_.cssText());
    if (!maximumHeight_.isAuto()) // == none
      element.setProperty(PropertyStyleMaxHeight, maximumHeight_.cssText());

    /*
     * set offsets
     */
    if (positionScheme_ != Static) {
      static const Property properties[] = { PropertyStyleTop,
					     PropertyStyleRight,
					     PropertyStyleBottom,
					     PropertyStyleLeft };

      for (unsigned i = 0; i < 4; ++i) {
	if (!offsets_[i].isAuto())
	  element.setProperty(properties[i], offsets_[i].cssText());
      }
    }

    /*
     * set vertical alignment
     */
    switch (verticalAlignment_) {
    case AlignBaseline:
      break;
    case AlignSub:
      element.setProperty(PropertyStyleVerticalAlign, "sub"); break;
    case AlignSuper:
      element.setProperty(PropertyStyleVerticalAlign, "super"); break;
    case AlignTop:
      element.setProperty(PropertyStyleVerticalAlign, "top"); break;
    case AlignTextTop:
      element.setProperty(PropertyStyleVerticalAlign, "text-top"); break;
    case AlignMiddle:
      element.setProperty(PropertyStyleVerticalAlign, "middle"); break;
    case AlignBottom:
      element.setProperty(PropertyStyleVerticalAlign, "bottom"); break;
    case AlignTextBottom:
      element.setProperty(PropertyStyleVerticalAlign, "text-bottom"); break;
    case AlignLength:
      element.setProperty(PropertyStyleVerticalAlign,
			  verticalAlignmentLength_.cssText()); break;
    }

    geometryChanged_ = false;
  }

  if (marginsChanged_ || all) {
    if (marginsChanged_ || (margin_[0].value() != 0))
      element.setProperty(PropertyStyleMarginTop, margin_[0].cssText());
    if (marginsChanged_ || (margin_[1].value() != 0))
      element.setProperty(PropertyStyleMarginRight, margin_[1].cssText());
    if (marginsChanged_ || (margin_[2].value() != 0))
      element.setProperty(PropertyStyleMarginBottom, margin_[2].cssText());
    if (marginsChanged_ || (margin_[3].value() != 0))
      element.setProperty(PropertyStyleMarginLeft, margin_[3].cssText());

    marginsChanged_ = false;
  }

  if (toolTipChanged_ || all) {
    if ((toolTip_.length() > 0) || toolTipChanged_)
      element.setAttribute("title", toolTip_);

    toolTipChanged_ = false;
  }

  if (styleClass_.length() == 0) {
    decorationStyle_.updateDomElement(element, all);
  } else {
    if (styleClassChanged_ || all) {
      element.setAttribute("class", styleClass_);
      styleClassChanged_ = false;
    }
  }

  renderOk();

  for (unsigned i = 0; i < childRemoveChanges_.size(); ++i)
    delete childRemoveChanges_[i];

  childRemoveChanges_.clear();
}

bool WWebWidget::isLoaded() const
{
  if (stubbed_)
    return false;
  else
    if (parent_)
      return parent_->webWidget()->isLoaded();
    else
      return true;
}

void WWebWidget::getSFormObjects(std::vector<WObject *>& result)
{
  if (!stubbed_)
    getFormObjects(result);
}

void WWebWidget::getFormObjects(std::vector<WObject *>& formObjects)
{ 
  if (formObject_)
    formObjects.push_back(this);

  for (unsigned i = 0; i < children_.size(); ++i)
    children_[i]->webWidget()->getSFormObjects(formObjects);
}

void WWebWidget::getSDomChanges(std::vector<DomElement *>& result)
{
  if (stubbed_) {
    if (wApp && wApp->loading()) {
      /* this is during collection of static slots */
      getDomChanges(result);
    } else {
      DomElement *stub = DomElement::getForUpdate(this, DomElement::SPAN);
      DomElement *realElement = createDomElement();
      stub->replaceWith(realElement);
      result.push_back(stub);
      stubbed_ = false;
    }
  } else {
    for (unsigned i = 0; i < childRemoveChanges_.size(); ++i)
      result.push_back(childRemoveChanges_[i]);

    childRemoveChanges_.clear();

    getDomChanges(result);
  }
}

DomElement *WWebWidget::createSDomElement()
{
  if (hidden_ && wApp && wApp->loading()) {
    /*
     * just create a stub, but let the element believe that it is
     * up-to-date so that static slots are loaded correctly.
     */
    DomElement *real = createDomElement();
    delete real;

    stubbed_ = true;

    DomElement *stub = DomElement::createNew(DomElement::SPAN);
    stub->setId(this);
    return stub;
  } else {
    stubbed_ = false;
    return createDomElement();
  }
}
void WWebWidget::updateSignalConnection(DomElement& element,
				     const char *signal,
				     const std::string eventName,
				     bool all, const char *argName)
{
  updateSignalConnection(element, signal, eventName, 0, all, argName);
}
std::string   WWebWidget::getEncodeCmd(const char *signal)
{
	WSignalInstance_ *s = getSignal(signal);
	
		if (isConnected(s))
			return s->encodeCmd();
		else
			return "";
			
	
}

void WWebWidget::updateSignalConnection(DomElement& element,
				     const char *signal,
				     const std::string eventName,
				     const char *condition,
				     bool all, const char *argName)
{
  WSignalInstance_ *s = getSignal(signal);
  if (all
      || s->connectivityChanged()
      || s->staticCodeChanged()) {
    if (isConnected(s))
      element.setEventSignal(eventName, *s, argName, condition);
    else
      element.removeEventSignal(eventName);

    s->connectivityOk();
  }
}

//wangpeng


void WWebWidget::setNoFormData()
{
  if (isLoaded())
    WObject::setNoFormData();
}

void WWebWidget::refresh()
{
  if (!toolTipMessage_.empty()) {
    if (toolTipMessage_.value() != toolTip_)
      setToolTipMessage(WMessage(toolTipMessage_));
  }

  for (unsigned i = 0; i < children().size(); ++i) {
    children()[i]->refresh();
  }
}
