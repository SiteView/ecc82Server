/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WImage"
#include "WContainerWidget"
#include "WCssDecorationStyle"
#include "WIconPair"

namespace Wt {

WIconPair::WIconPair(const std::string icon1URI, const std::string icon2URI,
		     bool clickIsSwitch, WContainerWidget *parent)
  : WCompositeWidget(parent),
    impl_(new WContainerWidget()),
    icon1_(new WImage(icon1URI, impl_)),
    icon2_(new WImage(icon2URI, impl_)),
    icon1Clicked(icon1_->clicked),
    icon2Clicked(icon2_->clicked)
{
  setImplementation(impl_);

  implementStateless(&WIconPair::showIcon1, &WIconPair::undoShowIcon1);
  implementStateless(&WIconPair::showIcon2, &WIconPair::undoShowIcon2);

  setInline(true);

  icon2_->hide();

  if (clickIsSwitch) {
    icon1_->clicked.connect(SLOT(icon1_, WImage::hide));
    icon1_->clicked.connect(SLOT(icon2_, WImage::show));

    icon2_->clicked.connect(SLOT(icon2_, WImage::hide));
    icon2_->clicked.connect(SLOT(icon1_, WImage::show));

    decorationStyle().setCursor(WCssDecorationStyle::Pointer);
  }
}

void WIconPair::setState(int num)
{
  if (num == 0) {
    icon1_->show();
    icon2_->hide();
  } else {
    icon1_->hide();
    icon2_->show();
  }
}

int WIconPair::state() const
{
  return (icon1_->isHidden() ? 1 : 0);
}

void WIconPair::showIcon1()
{
  previousState_ = (icon1_->isHidden() ? 1 : 0);
  setState(0);
}

void WIconPair::showIcon2()
{
  previousState_ = (icon1_->isHidden() ? 1 : 0);
  setState(1);
}

void WIconPair::undoShowIcon1()
{
  setState(previousState_);
}

void WIconPair::undoShowIcon2()
{
  setState(previousState_);
}

}
