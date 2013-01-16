/*
 * Copyright (C) 2007 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "WContainerWidget"
#include "WIconPair"
#include "WTableCell"
#include "WText"
#include "WTreeTable"
#include "WTreeTableNode"

namespace Wt {

WTreeTableNode::WTreeTableNode(const WMessage& labelText,
			       WIconPair *labelIcon,
			       WTreeTableNode *parentNode)
  : WTreeNode(labelText, labelIcon, parentNode),
    table_(0),
    row_(0)
{ }

void WTreeTableNode::addChildNode(WTreeNode *node)
{
  WTreeNode::addChildNode(node);

  if (table_)
    (dynamic_cast<WTreeTableNode *>(node))->setTable(table_);
}

void WTreeTableNode::setColumnWidget(int column, WWidget *widget)
{
  --column;

  createExtraColumns(column + 1);

  delete columnWidgets_[column];

  columnWidgets_[column] = widget;
  columnHaveWidget_[column] = true;

  widget->setInline(false);
  widget->setFloatSide(Left);
  widget->resize(columnWidth(column + 1), WLength());
  if (column == static_cast<int>(columnWidgets_.size()) - 1)
    row_->addWidget(widget);
  else
    row_->insertWidget(widget, columnWidgets_[column + 1]);
}

void WTreeTableNode::createExtraColumns(int numColumns)
{
  if (!row_) {
    row_ = new WContainerWidget();
    labelArea()->insertWidget(row_, labelArea()->children()[0]);
    row_->setFloatSide(Right);
    labelArea()->resize(WLength(100, WLength::Percentage), WLength());
  }

  while (static_cast<int>(columnWidgets_.size()) < numColumns) {
    columnWidgets_.push_back(new WText(L"", row_));
    columnWidgets_.back()
      ->resize(columnWidth(columnWidgets_.size()), WLength());
    columnHaveWidget_.push_back(false);
  }
}

WWidget *WTreeTableNode::columnWidget(int column)
{
  --column;

  return
    (column < static_cast<int>(columnHaveWidget_.size())
     && columnHaveWidget_[column])
    ? columnWidgets_[column]
    : 0;
}

WLength WTreeTableNode::columnWidth(int column)
{
  if (table_)
    return table_->columnWidth(column);
  else
    return WLength();
}

void WTreeTableNode::setTable(WTreeTable *table)
{
  if (table_ != table) {
    table_ = table;

    for (unsigned i = 0; i < childNodes().size(); ++i)
      dynamic_cast<WTreeTableNode *>(childNodes()[i])->setTable(table);

    createExtraColumns(table->numColumns() - 1);

    for (unsigned i = 0; i < columnWidgets_.size(); ++i)
      columnWidgets_[i]->resize(columnWidth(i + 1), WLength());
  }
}

}
