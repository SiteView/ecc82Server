// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2007 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "FileTreeTableNode.h"

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/exception.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <time.h>

#include <WIconPair>
#include <WStringUtil>
#include <WText>

using namespace Wt;

FileTreeTableNode::FileTreeTableNode(const boost::filesystem::path& path)
  : WTreeTableNode(Wt::widen(path.leaf()), createIcon(path)),
    path_(path)
{
  label()->setFormatting(WText::PlainFormatting);

  if (boost::filesystem::exists(path)) {
    if (!boost::filesystem::is_directory(path)) {
      int fsize = boost::filesystem::file_size(path);
      setColumnWidget(1, new WText(boost::lexical_cast<std::wstring>(fsize)));
      columnWidget(1)->setStyleClass(L"fsize");
    }

    std::time_t t = boost::filesystem::last_write_time(path);
    struct tm ttm;
    localtime_r(&t, &ttm);

    char c[100];
    strftime(c, 100, "%b %d %Y", &ttm);

    setColumnWidget(2, new WText(Wt::widen(c)));
    columnWidget(2)->setStyleClass(L"date");
  }
}

WIconPair *FileTreeTableNode::createIcon(const boost::filesystem::path& path)
{
  if (boost::filesystem::exists(path)
      && boost::filesystem::is_directory(path))
    return new WIconPair("../treelist/icons/yellow-folder-closed.png",
			 "../treelist/icons/yellow-folder-open.png", false);
  else
    return new WIconPair("../treelist/icons/document.png",
			 "../treelist/icons/yellow-folder-open.png", false);
}

void FileTreeTableNode::populate()
{
  try {
    if (boost::filesystem::is_directory(path_)) {
      std::set<boost::filesystem::path> paths;
      boost::filesystem::directory_iterator end_itr;

      for (boost::filesystem::directory_iterator i(path_); i != end_itr; ++i)
	paths.insert(*i);

      for (std::set<boost::filesystem::path>::iterator i = paths.begin();
	   i != paths.end(); ++i)
	addChildNode(new FileTreeTableNode(*i));
    }
  } catch (boost::filesystem::filesystem_error& e) {
    std::cerr << e.what() << std::endl;
  }
}
