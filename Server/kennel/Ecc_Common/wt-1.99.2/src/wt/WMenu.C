/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <iostream>
#include <boost/lexical_cast.hpp>

#include "WApplication"
#include "WStackedWidget"
#include "WTable"
#include "WTableCell"
#include "WText"
#include "WMenu"
#include "WMenuItem"

using std::find;

namespace Wt {

WMenu::WMenu(WStackedWidget *contentsStack, Orientation orientation,
	     WContainerWidget *parent)
  : WCompositeWidget(parent),
    contentsStack_(contentsStack),
    orientation_(orientation),
    current_(-1)
{
  setImplementation(layout_ = new WTable());
}

WMenu::~WMenu()
{
  for (unsigned i = 0; i < items_.size(); ++i)
    delete items_[i];
}

void WMenu::enableBrowserHistory(const std::string id)
{
  historyScope_ = id;
  setFromState(wApp->state(historyScope_));

  wApp->stateChanged.connect(SLOT(this, WMenu::appStateChanged));
}

WMenuItem *WMenu::addItem(const WMessage& name, WWidget *contents,
			  WMenuItem::LoadPolicy policy)
{
  return addItem(new WMenuItem(name, contents, policy));
}

WMenuItem *WMenu::addItem(WMenuItem *item)
{
  item->setMenu(this);

  WTableCell *parent
    = layout_->elementAt((orientation_ == Vertical) ? items_.size() : 0, 0);

  parent->addWidget(item->itemWidget());

  // separate horizontal items so wrapping will occur inbetween items.
  if (orientation_ == Horizontal)
    new WText(L" ", parent);

  items_.push_back(item);
  if (orientation_ == Horizontal)
    item->itemWidget()->setMargin(WLength(5), Right);

  for (unsigned i = 0; i < items_.size(); ++i)
    item->resetLearnedSlots();

  contentsStack_->addWidget(item->contents());

  if (items_.size() == 1)
    select(0);
  else
    item->renderSelected(false);

  return item;
}

void WMenu::select(int index)
{
  previousCurrent_ = current_;
  current_ = index;
  contentsStack_->setCurrentIndex(current_);

  items_[current_]->loadContents();

  for (unsigned i = 0; i < items_.size(); ++i)
    items_[i]->renderSelected((int)i == current_);

  if (!historyScope_.empty())
    wApp->setState(historyScope_,
		   boost::lexical_cast<std::string>(current_));

  itemSelected.emit(items_[current_]);
}

void WMenu::appStateChanged(const std::string scope, const std::string value)
{
  if (scope == historyScope_)
    setFromState(value);
}

void WMenu::setFromState(const std::string value)
{
  try {
    int i = value.empty() ? 0 : boost::lexical_cast<int>(value);

    if (i >= 0 && i < static_cast<int>(items_.size()))
      if (i != current_)
	select(i);
  } catch (std::exception& e) {
    std::cerr << "WMenu::setFromState could not handle: '"
	      << value << "'" << std::endl;
  } catch (...) {
    std::cerr << "WMenu::setFromState failed." << std::endl;
  } 
}

void WMenu::select(WMenuItem *item)
{
  select(indexOf(item));
}

int WMenu::indexOf(WMenuItem *item)
{
  return find(items_.begin(), items_.end(), item) - items_.begin();
}

void WMenu::undoSelect()
{
  select(previousCurrent_);
}

}
