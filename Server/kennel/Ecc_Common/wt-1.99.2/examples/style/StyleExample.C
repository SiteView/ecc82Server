/*
 * Copyright (C) 2006 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <boost/lexical_cast.hpp>

#include <WLineEdit>
#include <WIntValidator>
#include <WText>
#include <WPushButton>
#include <WApplication>
#include <WBreak>

#include "RoundedWidget.h"
#include "StyleExample.h"

wchar_t loremipsum[] = L"Lorem ipsum dolor sit amet, consectetur adipisicing "
   "elit, sed do eiusmod tempor incididunt ut labore et "
   "dolore magna aliqua. Ut enim ad minim veniam, quis "
   "nostrud exercitation ullamco laboris nisi ut aliquip " 
   "ex ea commodo consequat. Duis aute irure dolor in "
   "reprehenderit in voluptate velit esse cillum dolore eu "
   "fugiat nulla pariatur. Excepteur sint occaecat cupidatat "
   "non proident, sunt in culpa qui officia deserunt mollit "
   "anim id est laborum.";

StyleExample::StyleExample(WContainerWidget *parent)
  : WContainerWidget(parent)
{
  w_ = new RoundedWidget(RoundedWidget::All, this);

  new WText(loremipsum, w_->contents());
  new WBreak(this);

  new WText(L"Color (rgb): ", this);
  r_ = createColorLineEdit(w_->backgroundColor().red());
  g_ = createColorLineEdit(w_->backgroundColor().green());
  b_ = createColorLineEdit(w_->backgroundColor().blue());

  new WBreak(this);

  new WText(L"Radius (px): ", this);
  radius_ = createColorLineEdit(w_->cornerRadius());

  new WBreak(this);

  WPushButton *p = new WPushButton(L"Update!", this);
  p->clicked.connect(SLOT(this, StyleExample::updateStyle));

  new WBreak(this);

  error_ = new WText(L"", this);
}

WLineEdit *StyleExample::createColorLineEdit(int value)
{
  WLineEdit *le = new WLineEdit(boost::lexical_cast<std::wstring>(value),
				this);
  le->setTextSize(3);
  le->setValidator(new WIntValidator(0, 255));

  return le;
}

void StyleExample::updateStyle()
{
  if ((r_->validate() == WValidator::Valid)
      && (g_->validate() == WValidator::Valid)
      && (b_->validate() == WValidator::Valid)
      && (radius_->validate() == WValidator::Valid)) {
    w_->setBackgroundColor(WColor(boost::lexical_cast<int>(r_->text()),
				  boost::lexical_cast<int>(g_->text()),
				  boost::lexical_cast<int>(b_->text())));
    w_->setCornerRadius(boost::lexical_cast<int>(radius_->text()));
    error_->setText(L"");
  } else {
    error_->setText(L"All values must be numbers between 0 and 255...");
  }
}

WApplication *createApplication(const WEnvironment& env)
{
  WApplication *app = new WApplication(env);
  app->setTitle(L"Style example");

  app->root()->addWidget(new StyleExample());
  return app;
}

int main(int argc, char **argv)
{
   return WRun(argc, argv, &createApplication);
}
