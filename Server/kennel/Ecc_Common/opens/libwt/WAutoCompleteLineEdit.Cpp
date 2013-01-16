/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WAutoCompleteLineEdit"
#include "WAutoCompleteSuggestion"
#include "WLineEdit"
#include "WTable"
#include "WTableCell"
#include "WText"

#include <iostream>
//#include "WAutoCompleteLineEdit"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WAutoCompleteLineEdit::slots_[] = { WSlot_("lineEditKeyUp(int)", false), WSlot_() };
WSignal_ WAutoCompleteLineEdit::signals_[] = { WSignal_() };

void WAutoCompleteLineEdit::buildSlotMap(WSlotMap& result)
{
  WCompositeWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WAutoCompleteLineEdit::buildSignalMap(WSignalMap& result)
{
  WCompositeWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WAutoCompleteLineEdit::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 0) {
    sender_ = sender;    lineEditKeyUp(*((int *)args[0]));
    return true;
  }
  return WCompositeWidget::triggerSlot(sender, slot, args);
}
bool WAutoCompleteLineEdit::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WCompositeWidget::undoTriggerSlot(slot, args);
}


WAutoCompleteLineEdit::WAutoCompleteLineEdit(WContainerWidget *parent)
  : WCompositeWidget()
{
  create(parent);
}

WAutoCompleteLineEdit::WAutoCompleteLineEdit(const std::string content,
					     WContainerWidget *parent)
  : WCompositeWidget()
{
  create(parent);
  lineEdit_->setText(content);
}

void WAutoCompleteLineEdit::create(WContainerWidget *parent)
{
  setImplementation(layout_ = new WTable(parent));

  setInline(true);
  setVerticalAlignment(AlignMiddle);

  lineEdit_ = new WLineEdit(layout_->elementAt(0, 0));

  suggestionsList_ = new WTable(layout_->elementAt(1, 0));
  suggestionsList_->setPopup(true);
  suggestionsList_->setPositionScheme(Absolute);

  WCssDecorationStyle& style = suggestionsList_->decorationStyle();
  style.setCursor(WCssDecorationStyle::Default);
  style.font().setSize(WFont::Smaller);
  WBorder border(WBorder::Solid);
  border.setWidth(WBorder::Explicit, WLength(2));
  style.setBorder(border);
  style.setBackgroundColor(Wt::white);

  connect(lineEdit_, SIGNAL(keyWentUp(int)),
	  this, SLOT(lineEditKeyUp(int)));
  //connect(lineEdit_, SIGNAL(blurred()),
  //  suggestionsList_, SLOT(hide()), STATIC);

  suggestionsList_->hide();
}

void WAutoCompleteLineEdit::setSuggestions(const std::vector<std::string>&
					   suggestions)
{
  suggestions_ = suggestions;

  suggestionsList_->clear();
  for (unsigned i = 0; i < suggestions_.size(); ++i) {
    WTableCell *cell = suggestionsList_->elementAt(i, 0);
    cell->setContentAlignment(WContainerWidget::AlignJustify);
    new WAutoCompleteSuggestion(this, i, cell);
  }
}

std::string WAutoCompleteLineEdit::suggestion(int suggestionIndex) const
{
  return suggestions_[suggestionIndex];
}

void WAutoCompleteLineEdit::lineEditKeyUp(int keycode)
{
  if (keycode == 27) // ESC
    suggestionsList_->hide();
  else {
    if (lineEdit_->text().length() > 0)
      filterSuggestions(lineEdit_->text());
    else
      suggestionsList_->hide();
  }
}

void WAutoCompleteLineEdit::filterSuggestions(const std::string text)
{
  bool atLeastOne = false;
  for (unsigned i = 0; i < suggestions_.size(); ++i) {
    WTableCell *cell = suggestionsList_->elementAt(i, 0);
    if (suggestionMatches(i)) {
      atLeastOne = true;
      if (cell->isHidden())
	cell->show();
    } else {
      if (!cell->isHidden())
	cell->hide();
    }
  }

  if (atLeastOne && suggestionsList_->isHidden())
    suggestionsList_->show();
  if (!atLeastOne && !suggestionsList_->isHidden())
    suggestionsList_->hide();
}

void WAutoCompleteLineEdit::selectSuggestion(int suggestionIndex)
{
  lineEdit_->setText(suggestion(suggestionIndex));
}

bool WAutoCompleteLineEdit::suggestionMatches(int suggestionIndex) const
{
  if (suggestions_[suggestionIndex].find(lineEdit_->text()) == 0)
    return true;
  else
    return false;
}
