/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <WText>

#include "Option.h"
#include "OptionList.h"

Option::Option(const WMessage& text, WContainerWidget *parent)
  : WContainerWidget(parent),
    sep_(0),
    list_(0)
{
  setInline(true);
  option_ = new WText(text, this);
  option_->setStyleClass(L"option");
}

void Option::setText(const WMessage& text)
{
  option_->setText(text);
}

void Option::setOptionList(OptionList *l)
{
  list_ = l;
}

void Option::addSeparator()
{
  sep_ = new WText(L"|", this);
  sep_->setStyleClass(L"sep");
}

void Option::hideSeparator()
{
  sep_->hide();
}

void Option::showSeparator()
{
  sep_->show();
}

void Option::setHidden(bool hidden)
{
  WContainerWidget::setHidden(hidden);

  if (list_)
    list_->optionVisibilityChanged(this, hidden);
}
