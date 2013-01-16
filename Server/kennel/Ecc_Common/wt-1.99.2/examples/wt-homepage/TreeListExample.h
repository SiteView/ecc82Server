// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef TREE_LIST_EXAMPLE
#define TREE_LIST_EXAMPLE

#include <WContainerWidget>

namespace Wt {
  class WPushButton;
  class WLineEdit;
}

using namespace Wt;

class TreeNode;

class TreeListExample : public WContainerWidget
{
public:
  TreeListExample(WContainerWidget *parent = 0);

private slots:
  void addMap();
  void removeMap();

private:
  TreeNode *tree_;
  TreeNode *testMap_;
  int testCount_;

  WLineEdit   *mapNameEdit_;
  WPushButton *addMapButton_;
  WPushButton *removeMapButton_;

  TreeNode *makeTreeMap(const std::wstring name, TreeNode *parent);
  TreeNode *makeTreeFile(const std::wstring name, TreeNode *parent);
};

#endif // TREE_LIST_EXAMPLE
