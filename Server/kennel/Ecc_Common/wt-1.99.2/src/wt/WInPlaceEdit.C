/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "WInPlaceEdit"
#include "WCssDecorationStyle"
#include "WContainerWidget"
#include "WPushButton"
#include "WText"
#include "WLineEdit"

namespace Wt {

WInPlaceEdit::WInPlaceEdit(const std::wstring text, WContainerWidget *parent)
  : WCompositeWidget(parent),
    valueChanged(this)
{
  setImplementation(impl_ = new WContainerWidget());
  setInline(true);

  text_ = new WText(text, impl_);
  text_->setFormatting(WText::PlainFormatting);
  text_->decorationStyle().setCursor(WCssDecorationStyle::Default);

  edit_ = new WLineEdit(text, impl_);
  edit_->setTextSize(20);
  save_ = new WPushButton(L"Save", impl_);
  cancel_ = new WPushButton(L"Cancel", impl_);
  edit_->hide();
  save_->hide();
  cancel_->hide();

  text_->clicked.connect(SLOT(text_,   WWidget::hide));
  text_->clicked.connect(SLOT(edit_,   WWidget::show));
  text_->clicked.connect(SLOT(save_,   WWidget::show));
  text_->clicked.connect(SLOT(cancel_, WWidget::show));

  save_->clicked.connect(SLOT(save_,   WWidget::hide));
  save_->clicked.connect(SLOT(cancel_, WWidget::hide));
  save_->clicked.connect(SLOT(edit_,   WFormWidget::disable));
  save_->clicked.connect(SLOT(this,    WInPlaceEdit::save));

  cancel_->clicked.connect(SLOT(save_,   WWidget::hide));
  cancel_->clicked.connect(SLOT(cancel_, WWidget::hide));
  cancel_->clicked.connect(SLOT(edit_,   WWidget::hide));
  cancel_->clicked.connect(SLOT(text_,   WWidget::show));
}

const std::wstring WInPlaceEdit::text() const
{
  return text_->text().value();
}

void WInPlaceEdit::save()
{
  edit_->hide();
  text_->show();
  text_->setText(edit_->text());
  edit_->enable();

  valueChanged.emit(edit_->text());
}

}
