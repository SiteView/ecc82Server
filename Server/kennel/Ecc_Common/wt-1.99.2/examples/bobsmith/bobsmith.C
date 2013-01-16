/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <WApplication>
#include <WText>
#include <WInPlaceEdit>

using namespace Wt;

WApplication *createApplication(const WEnvironment& env)
{
  WApplication *app = new WApplication(env);

  new WText("Name: ", app->root());
  WInPlaceEdit *edit = new WInPlaceEdit(L"Bob Smith", app->root());
  edit->setStyleClass(L"inplace");

  app->styleSheet().addRule("*.inplace span:hover", L"background-color: gray");

  return app;
}

int main(int argc, char **argv)
{
   return WRun(argc, argv, &createApplication);
}
