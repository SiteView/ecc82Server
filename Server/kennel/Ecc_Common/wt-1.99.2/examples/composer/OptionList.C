// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "OptionList.h"
#include "Option.h"

OptionList::OptionList(WContainerWidget *parent)
  : WContainerWidget(parent),
    optionNeedReset_(0)
{
  resize(WLength(), WLength(2.5, WLength::FontEx));
}

void OptionList::add(Option *option)
{
  addWidget(option);
  option->setOptionList(this);

  if (!options_.empty()) {
    options_.back()->addSeparator();
  }

  options_.push_back(option);
}

void OptionList::update()
{
  if (optionNeedReset_ != 0)
    optionNeedReset_->resetLearnedSlots();

  optionNeedReset_ = 0;
}

void OptionList::optionVisibilityChanged(Option *opt, bool hidden)
{
  /*
   * Check if it was the last visible option, in that case the second last
   * visible option loses its separator.
   */
  for (unsigned i = options_.size() - 1; i > 0; --i) {
    if (options_[i] == opt) {
      for (int j = i - 1; j >= 0; --j) {
	if (!options_[j]->isHidden()) {
	  if (hidden)
	    options_[j]->hideSeparator();
	  else
	    options_[j]->showSeparator();
	  break;
	}
      }
      break;
    } else
      if (!options_[i]->isHidden())
	break;
  }

  /*
   * The Option to the right needs to relearn its stateless
   * slot code for hide() and show().
   */
  for (unsigned i = 0; i < options_.size(); ++i) {
    if (options_[i] == opt) {
      for (unsigned j = i + 1; j < options_.size(); ++j) {
	if (!options_[j]->isHidden()) {
	  optionNeedReset_ = options_[j];
	  break;
	}
      }

      break;
    }
  }
}

