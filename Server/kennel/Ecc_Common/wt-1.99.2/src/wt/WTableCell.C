/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <boost/lexical_cast.hpp>

#include "WTableCell"
#include "WTable"
#include "DomElement.h"

namespace Wt {

WTableCell::WTableCell(WTableRow *row, int column)
  : WContainerWidget(0),
    row_(row),
    column_(column),
    rowSpan_(1),
    columnSpan_(1)
{
  setParent(row->table());
  setContentAlignment(AlignTop | AlignLeft);
}

int WTableCell::row() const
{
  return row_->rowNum();
}

void WTableCell::setRowSpan(int rowSpan)
{
  rowSpan_ = rowSpan;
  row_->table()->expand(row(), column_, rowSpan_, columnSpan_);
}

void WTableCell::setColumnSpan(int colSpan)
{
  columnSpan_ = colSpan;
  row_->table()->expand(row(), column_, rowSpan_, columnSpan_);
}

void WTableCell::setContentAlignment(int contentAlignment)
{
  HorizontalAlignment h = (HorizontalAlignment)(contentAlignment & 0x0F);
  VerticalAlignment v = (VerticalAlignment)(contentAlignment & 0xF0);
  WContainerWidget::setContentAlignment(h);
  WContainerWidget::setVerticalAlignment(v);
}

int WTableCell::contentAlignment() const
{
  return WContainerWidget::contentAlignment() |
    verticalAlignment();
}

void WTableCell::updateDom(DomElement& element, bool all)
{
  WContainerWidget::updateDom(element, all);
}

DomElement *WTableCell::createDomElement()
{
  DomElement *td = DomElement::createNew(DomElement::TD);
  td->setId(this);
  if (rowSpan_ != 1)
    td->setAttribute("rowspan",
		     boost::lexical_cast<std::string>(rowSpan_));
  if (columnSpan_ != 1)
    td->setAttribute("colspan",
		     boost::lexical_cast<std::string>(columnSpan_));

  createDomChildren(*td);

  updateDom(*td, true);

  renderOk();

  return td;
}

}
