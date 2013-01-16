/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WButtonGroup"
#include "WRadioButton"

using std::find;

namespace Wt {

WButtonGroup::WButtonGroup(WObject* parent)
    :WObject(parent)
{ }

WButtonGroup::~WButtonGroup()
{
  for (unsigned i = 0; i < buttons_.size(); ++i) {
    buttons_[i]->setGroup(0);
  }
}

void WButtonGroup::addButton(WRadioButton *button)
{
  buttons_.push_back(button);
  button->setGroup(this);
}

void WButtonGroup::removeButton(WRadioButton *button)
{
  buttons_.erase(find(buttons_.begin(), buttons_.end(), button));
  button->setGroup(0);
}

}
