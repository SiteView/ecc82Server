// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef HOME_H_
#define HOME_H_

#include <WContainerWidget>

namespace Wt {
  class WStackedWidget;
  class WTreeNode;
  class WTable;
}

using namespace Wt;

class Home : public WContainerWidget
{
public:
  Home(WContainerWidget *parent);

  virtual void refresh();

private:
  WStackedWidget *contents_;

  WWidget *introduction();
  WWidget *news();
  WWidget *status();
  WWidget *features();
  WWidget *documentation();
  WWidget *examples();
  WWidget *download();
  WWidget *community();

  WTable *recentNews_;
  WTable *historicalNews_;
  WTable *releases_;

  void readNews(WTable *newsTable, const std::string newsfile);
  void readReleases(WTable *releaseTable, const std::string releasefile);
  static std::string href(const std::string url,
			  const std::string description);

  WTreeNode *makeTreeMap(const std::string name, WTreeNode *parent);
  WTreeNode *makeTreeFile(const std::string name, WTreeNode *parent);

  WWidget *helloWorldExample();
  WWidget *homepageExample();
  WWidget *formExample();
  WWidget *treelistExample();
  WWidget *hangmanExample();
  WWidget *styleExample();
  WWidget *composerExample();
  WWidget *dragdropExample();
  WWidget *fileExplorerExample();

  void changeTitle(WMenuItem *item);
};

#endif // HOME_H_
