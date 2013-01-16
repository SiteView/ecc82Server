/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "WContainerWidget"
#include "WMenuItem"
#include "WMenu"
#include "WTableCell"
#include "WText"

namespace Wt {

WMenuItem::WMenuItem(const WMessage& text, WWidget *contents,
		     LoadPolicy policy)
  : text_(text),
    itemWidget_(0),
    contentsContainer_(0),
    contents_(contents)
{
  if (policy == PreLoading)
    // prelearn the stateless slot.
    implementStateless(&WMenuItem::select, &WMenuItem::undoSelect);
  else {
    contentsContainer_ = new WContainerWidget();
  }
}

WMenuItem::~WMenuItem()
{
  delete contents_;
}

WInteractWidget *WMenuItem::createItemWidget()
{
  return new WText(text_);
}

WInteractWidget *WMenuItem::itemWidget()
{
  if (!itemWidget_) {
    itemWidget_ = createItemWidget();

    if (contentsContainer_)
      // load contents (will only do something on the first click).
      itemWidget_->clicked.connect(SLOT(this, WMenuItem::loadContents));
    else
      itemWidget_->clicked.connect(SLOT(this, WMenuItem::select));
  }
  return itemWidget_;
}

void WMenuItem::renderSelected(bool selected)
{
  itemWidget()->setStyleClass(selected ? L"itemselected" : L"item");
}

void WMenuItem::loadContents()
{
  if (contentsContainer_) {
    contentsContainer_->addWidget(contents_);
    contentsContainer_ = 0;

    select();

    implementStateless(&WMenuItem::select, &WMenuItem::undoSelect);
    itemWidget_->clicked.connect(SLOT(this, WMenuItem::select));
  }
}

void WMenuItem::setMenu(WMenu *menu)
{
  menu_ = menu;
}

WWidget *WMenuItem::contents()
{
  if (contentsContainer_)
    return contentsContainer_;
  else
    return contents_;
}

void WMenuItem::select()
{
  menu_->select(this);
}

void WMenuItem::undoSelect()
{
  menu_->undoSelect();
}

}
