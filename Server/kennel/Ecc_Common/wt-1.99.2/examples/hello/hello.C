/*
 * Copyright (C) 2006 Wim Dumon
 *
 * See the LICENSE file for terms of use.
 */

#include <WApplication>
#include <WContainerWidget>
#include <WText>
#include <WPushButton>

using namespace Wt;

// This is the entry-point for a new session.
//
// This function is executed when a new user surfs to the Wt application,
// and after the library has negotiated browser support. This function must
// return a new WApplication object.

WApplication *createApplication(const WEnvironment& env)
{
  // Instantiate the Wt application.
  WApplication *appl = new WApplication(env);

  // Set application title
  appl->setTitle("Hello world!");

  // Widgets can be added to a parent by calling addWidget()...
  // WApplication::root() is a WContainerWidget that is associated with
  // the entire browser window.
  appl->root()->addWidget(new WText(L"<h1>Hello, World!</h1>"));

  // .. or by specifying a parent at construction
  WPushButton *Button = new WPushButton(L"Quit", appl->root());

  // React to user events using a signal/slot mechanism.
  Button->clicked.connect(SLOT(appl, WApplication::quit));

  return appl;
}

int main(int argc, char **argv)
{
   return WRun(argc, argv, &createApplication);
}

