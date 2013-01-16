/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include "WAutoCompleteSuggestion"
#include "WContainerWidget"
#include "WTable"
#include "WText"
#include "WAutoCompleteLineEdit"
#include "WLineEdit"

#include <iostream>
//#include "WAutoCompleteSuggestion"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WAutoCompleteSuggestion::slots_[] = { WSlot_("gotClick()", true), WSlot_() };
WSignal_ WAutoCompleteSuggestion::signals_[] = { WSignal_() };

void WAutoCompleteSuggestion::buildSlotMap(WSlotMap& result)
{
  WCompositeWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WAutoCompleteSuggestion::buildSignalMap(WSignalMap& result)
{
  WCompositeWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WAutoCompleteSuggestion::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 0) {
    sender_ = sender;    gotClick();
    return true;
  }
  return WCompositeWidget::triggerSlot(sender, slot, args);
}
bool WAutoCompleteSuggestion::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  if (slot == slots_ + 0) {
    undoGotClick();
    return true;
  }
  return WCompositeWidget::undoTriggerSlot(slot, args);
}


WAutoCompleteSuggestion
::WAutoCompleteSuggestion(WAutoCompleteLineEdit *lineEdit,
			  int suggestionIndex,
			  WContainerWidget *parent)
  : lineEdit_(lineEdit),
    suggestionIndex_(suggestionIndex)
{
  setImplementation(impl_ = new WContainerWidget(parent));

  setStyleClass("suggestion");
  new WText(" " + lineEdit->suggestions_[suggestionIndex]+ " ", impl_);

  connect(parent, SIGNAL(clicked()), this, SLOT(gotClick()), STATIC);
}

void WAutoCompleteSuggestion::gotClick()
{
  previousText_ = lineEdit_->lineEdit_->text();
  lineEdit_->suggestionsList_->hide();
  lineEdit_->selectSuggestion(suggestionIndex_);
}

void WAutoCompleteSuggestion::undoGotClick()
{
  lineEdit_->lineEdit_->setText(previousText_);
}
