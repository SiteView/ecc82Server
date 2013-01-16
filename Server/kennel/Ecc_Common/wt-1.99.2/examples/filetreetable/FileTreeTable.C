// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2007 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "FileTreeTable.h"
#include "FileTreeTableNode.h"

#include <WMessage>
#include <WText>

using namespace Wt;

FileTreeTable::FileTreeTable(const boost::filesystem::path& path,
			     WContainerWidget *parent)
  : WTreeTable(parent)
{
  addColumn(L"Size", WLength(80));
  addColumn(L"Modified", WLength(90));

  header(1)->setStyleClass("fsize");
  header(2)->setStyleClass("date");

  setTreeRoot(new FileTreeTableNode(path), L"File");

  treeRoot()->setImagePack("../treelist/icons/");
  treeRoot()->expand();
}
