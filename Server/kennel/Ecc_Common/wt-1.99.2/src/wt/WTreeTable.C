/*
 * Copyright (C) 2007 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "WContainerWidget"
#include "WMessage"
#include "WTreeTable"
#include "WText"
#include "WTreeTableNode"

namespace Wt {

WTreeTable::WTreeTable(WContainerWidget *parent)
  : WCompositeWidget(parent),
    treeRoot_(0)
{
  setImplementation(impl_ = new WContainerWidget());

  WContainerWidget *headers = new WContainerWidget(impl_); 
  headers->setStyleClass(L"header");
  headers->resize(WLength(100, WLength::Percentage), WLength());

  headerContainer_ = new WContainerWidget(headers);
  headerContainer_->setFloatSide(Right);

  headers->addWidget(new WText(L""));
  columnWidths_.push_back(WLength());
}

void WTreeTable::setTreeRoot(WTreeTableNode *root, const WMessage& h)
{
  if (treeRoot_)
    delete treeRoot_;

  header(0)->setText(h);

  treeRoot_ = root;
  treeRoot_->setTable(this);
  impl_->addWidget(treeRoot_);
}

void WTreeTable::addColumn(const WMessage& header, WLength width)
{
  WText *t = new WText(header);
  t->resize(width, WLength());
  t->setInline(false);
  t->setFloatSide(Left);
  headerContainer_->addWidget(t);

  columnWidths_.push_back(width);
}

WText *WTreeTable::header(int column) const
{
  if (column == 0)
    return dynamic_cast<WText *>
      (dynamic_cast<WContainerWidget *>(impl_->children()[0])
       ->children()[1]);
  else
    return dynamic_cast<WText *>(headerContainer_->children()[column - 1]);
}

}
