/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 *
 * Integrated memory management to XLObject
 * abj <xynopsis@yahoo.com> 2006.4.20
 *
 */

#include <iostream>

#include "WCompositeWidget"
#include "WContainerWidget"
#include "WStringUtil"

namespace Wt {

WCompositeWidget::WCompositeWidget(WContainerWidget *parent)
  : WWidget(parent),
    impl_(0)
{
  if (parent)
    parent->addWidget(this);
}

WCompositeWidget::~WCompositeWidget()
{
  setParent(0);

  delete impl_;
}

const std::string WCompositeWidget::formName() const
{
  return impl_->formName();
}

void WCompositeWidget::setPositionScheme(PositionScheme scheme)
{
  impl_->setPositionScheme(scheme);
}

WWidget::PositionScheme WCompositeWidget::positionScheme() const
{
  return impl_->positionScheme();
}

void WCompositeWidget::setOffset(int s, WLength x)
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
  return impl_->setInline(how);
}

bool WCompositeWidget::isInline() const
{
  return impl_->isInline();
}

WCssDecorationStyle& WCompositeWidget::decorationStyle()
{
  return impl_->decorationStyle();
}

void WCompositeWidget::setStyleClass(const std::wstring styleClass)
{
  impl_->setStyleClass(styleClass);
}

void WCompositeWidget::setStyleClass(const char *styleClass)
{
  setStyleClass(widen(styleClass));
}

const std::wstring WCompositeWidget::styleClass() const
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

void WCompositeWidget::setToolTip(const WMessage& text)
{
  impl_->setToolTip(text);
}

WMessage WCompositeWidget::toolTip() const
{
  return impl_->toolTip();
}

void WCompositeWidget::refresh()
{
  impl_->refresh();
}

void WCompositeWidget::addChild(WWidget *child)
{
  if (child != impl_)
    impl_->addChild(child);
  else
    impl_->parent_ = this;
}

void WCompositeWidget::removeChild(WWidget *child)
{
  if (child != impl_)
    impl_->removeChild(child);
  else
    impl_->parent_ = 0;
}

void WCompositeWidget::addNewSibling(WWidget *sibling)
{
  impl_->addNewSibling(sibling);
}

bool WCompositeWidget::isVisible() const
{
  if (parent())
    return parent()->isVisible();
  else
    return true;
}

bool WCompositeWidget::isStubbed() const
{
  if (parent())
    return parent()->isStubbed();
  else
    return false;
}

void WCompositeWidget::setAttributeValue(const std::string attribute,
					 const std::wstring value)
{
  impl_->setAttributeValue(attribute, value);
}


void WCompositeWidget::load()
{
  if (impl_)
    impl_->load();
}

bool WCompositeWidget::loaded() const
{
  return impl_->loaded();
}

void WCompositeWidget::setImplementation(WWidget *widget)
{
  if (widget->parent() != 0) {
    std::cerr << "Wt 1.1.3 changed the WCompositeWidget: the implementaition "
      "widget in setImplementation(widget) MAY not have a parent." << std::endl
	      << "Fix your program!" << std::endl;
  }

  impl_ = widget;
  if (loaded())
    impl_->load();

  widget->setParent(this);
}

}
