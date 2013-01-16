/*
 * Copyright (C) 2007 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <WApplication>
#include <WContainerWidget>

#include "FileTreeTable.h"

using namespace Wt;

WApplication *createApplication(const WEnvironment& env)
{
  WApplication *app = new WApplication(env);
  app->setTitle(L"File explorer example");
  app->useStyleSheet("filetree.css");

  FileTreeTable *treeTable = new FileTreeTable("/home/kdforc0/project/wt");
  treeTable->resize(WLength(500), WLength());

  app->root()->addWidget(treeTable);

  return app;
}

int main(int argc, char **argv)
{
   return WRun(argc, argv, &createApplication);
}

