/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "WCompositeWidget"

#include <iostream>
//#include "WCompositeWidget"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WCompositeWidget::slots_[] = { WSlot_() };
WSignal_ WCompositeWidget::signals_[] = { WSignal_() };

void WCompositeWidget::buildSlotMap(WSlotMap& result)
{
  WWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WCompositeWidget::buildSignalMap(WSignalMap& result)
{
  WWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WCompositeWidget::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WWidget::triggerSlot(sender, slot, args);
}
bool WCompositeWidget::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WWidget::undoTriggerSlot(slot, args);
}


WCompositeWidget::WCompositeWidget()
{ }

WCompositeWidget::~WCompositeWidget()
{
  delete impl_;
}

WWidget *WCompositeWidget::parent() const
{
  return impl_->parent();
}

void WCompositeWidget::setPositionScheme(PositionScheme scheme)
{
  impl_->setPositionScheme(scheme);
}

WWidget::PositionScheme WCompositeWidget::positionScheme() const
{
  return impl_->positionScheme();
}

void WCompositeWidget::setOffset(Side s, WLength x)
{
  impl_->setOffset(s, x);
}

WLength WCompositeWidget::offset(Side s) const
{
  return impl_->offset(s);
}

void WCompositeWidget::resize(WLength width, WLength height)
{
  impl_->resize(width, height);
}

WLength WCompositeWidget::width() const
{
  return impl_->width();
}

WLength WCompositeWidget::height() const
{
  return impl_->height();
}

void WCompositeWidget::setMinimumSize(WLength width, WLength height)
{
  impl_->setMinimumSize(width, height);
}

void WCompositeWidget::setMaximumSize(WLength width, WLength height)
{
  impl_->setMaximumSize(width, height);
}

void WCompositeWidget::setFloatSide(Side s)
{
  impl_->setFloatSide(s);
}

WWidget::Side WCompositeWidget::floatSide() const
{
  return impl_->floatSide();
}

void WCompositeWidget::setClearSides(int sides)
{
  impl_->setClearSides(sides);
}

int WCompositeWidget::clearSides() const
{
  return impl_->clearSides();
}

void WCompositeWidget::setMargin(WLength margin, int sides)
{
  impl_->setMargin(margin, sides);
}

WLength WCompositeWidget::margin(Side side) const
{
  return impl_->margin(side);
}

void WCompositeWidget::setHidden(bool how)
{
  impl_->setHidden(how);
}

bool WCompositeWidget::isHidden() const
{
  return impl_->isHidden();
}

void WCompositeWidget::setPopup(bool how)
{
  impl_->setPopup(how);
}

bool WCompositeWidget::isPopup() const
{
  return impl_->isPopup();
}

void WCompositeWidget::setInline(bool how)
{
 // return impl_->setInline(how);
	impl_->setInline(how);
}

bool WCompositeWidget::isInline() const
{
  return impl_->isInline();
}

WCssDecorationStyle& WCompositeWidget::decorationStyle()
{
  return impl_->decorationStyle();
}

void WCompositeWidget::setStyleClass(const std::string styleClass)
{
  impl_->setStyleClass(styleClass);
}

const std::string WCompositeWidget::styleClass() const
{
  return impl_->styleClass();
}

void WCompositeWidget::setVerticalAlignment(VerticalAlignment alignment,
					    WLength length)
{
  impl_->setVerticalAlignment(alignment, length);
}

WWidget::VerticalAlignment WCompositeWidget::verticalAlignment() const
{
  return impl_->verticalAlignment();
}

WLength WCompositeWidget::verticalAlignmentLength() const
{
  return impl_->verticalAlignmentLength();
}

WWebWidget *WCompositeWidget::webWidget()
{
  return impl_->webWidget();
}

void WCompositeWidget::setToolTip(const std::string text)
{
  impl_->setToolTip(text);
}

void WCompositeWidget::setToolTipMessage(const WMessage& message)
{
  impl_->setToolTipMessage(message);
}

std::string WCompositeWidget::toolTip() const
{
  return impl_->toolTip();
}

const WMessage& WCompositeWidget::toolTipMessage() const
{
  return impl_->toolTipMessage();
}

void WCompositeWidget::refresh()
{
  impl_->refresh();
}

void WCompositeWidget::hide()
{
  impl_->hide();
}

void WCompositeWidget::show()
{
  impl_->show();
}

void WCompositeWidget::undoHide()
{
  impl_->undoHide();
}

void WCompositeWidget::undoShow()
{
  impl_->undoShow();
}

void WCompositeWidget::addChild(WWidget *child)
{
  impl_->addChild(child);
}

void WCompositeWidget::removeChild(WWidget *child)
{
  impl_->removeChild(child);
}

void WCompositeWidget::setParent(WWidget *parent)
{
  impl_->setParent(parent);
}

const std::vector<WWidget *>& WCompositeWidget::children() const
{
  return impl_->children();
}

void WCompositeWidget::setImplementation(WWidget *widget)
{
  impl_ = widget;
}
