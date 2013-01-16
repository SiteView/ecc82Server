/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WStackedWidget"

using std::find;

namespace Wt {

WStackedWidget::WStackedWidget(WContainerWidget *parent)
  : WContainerWidget(parent),
    currentIndex_(-1)
{ }

void WStackedWidget::addWidget(WWidget *widget)
{
  insertWidget(widgets_.size(), widget);
}

int WStackedWidget::count() const
{
  return widgets_.size();
}

int WStackedWidget::currentIndex() const
{
  return currentIndex_;
}

WWidget *WStackedWidget::currentWidget() const
{
  return widgets_[currentIndex_];
}

int WStackedWidget::indexOf(WWidget *widget) const
{
  return find(widgets_.begin(), widgets_.end(), widget) - widgets_.begin();
}

int WStackedWidget::insertWidget(int index, WWidget *widget)
{
  // do not bother inserting in correct place since this does not matter
  // as only one will be visible at each time
  WContainerWidget::addWidget(widget);

  widget->hide();
  widgets_.insert(widgets_.begin() + index, widget);

  if (currentIndex_ == -1)
    setCurrentIndex(0);

  return index;
}

void WStackedWidget::removeWidget(WWidget *widget)
{
  widgets_.erase(widgets_.begin() + indexOf(widget));
  if (currentIndex_ >= (int)widgets_.size())
    setCurrentIndex(widgets_.size() - 1);
}

WWidget *WStackedWidget::widget(int index) const
{
  return widgets_[index];
}

void WStackedWidget::setCurrentIndex(int index)
{
  currentIndex_ = index;
  for (unsigned i = 0; i < (unsigned)widgets_.size(); ++i)
    widgets_[i]->setHidden(currentIndex_ != (int)i);
}

void WStackedWidget::setCurrentWidget(WWidget *widget)
{
  setCurrentIndex(indexOf(widget));
}

}
