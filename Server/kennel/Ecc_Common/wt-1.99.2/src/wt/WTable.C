/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <iostream>
#include <boost/lexical_cast.hpp>

#include "WTable"
#include "WTableCell"
#include "DomElement.h"

using std::find;

namespace Wt {

WTableRow::WTableRow(WTable *table, int numCells)
  : table_(table)
{ 
  expand(numCells);
}

WTableRow::TableData::TableData()
  : cell(0),
    overSpanned(0)
{ }

void WTableRow::expand(int numCells)
{
  int cursize = cells_.size();

  for (int col = cursize; col < numCells; ++col) {
    cells_.push_back(TableData());
    cells_.back().cell = new WTableCell(this, col);
  }
}

void WTableRow::insertColumn(int column)
{
  cells_.insert(cells_.begin() + column, TableData());
  cells_[column].cell = new WTableCell(this, column);

  for (unsigned i = column; i < cells_.size(); ++i)
    cells_[i].cell->column_ = i;
}

int WTableRow::rowNum() const
{
  return find(table_->rows_.begin(), table_->rows_.end(), this)
    - table_->rows_.begin();
}

WTable::WTable(WContainerWidget *parent)
  : WInteractWidget(parent),
    cellPadding_(0),
    gridChanged_(false),
    paddingChanged_(false)
{ 
  setInline(false);
  setIgnoreChildRemoves(true);
}

WTable::~WTable()
{
  for (unsigned i = 0; i < rows_.size(); ++i)
    delete rows_[i];
}

void WTable::printDebug()
{
  std::cerr << "Table: "
	    << formName() << " " << numRows() << "x" << numColumns()
	    << std::endl;
  
  for (int i = 0; i < numRows(); ++i) {
    for (int j = 0; j < numColumns(); ++j) {
      std::cerr << "(" << i << "," << j << "): "
		<< itemAt(i, j).cell << std::endl;
    }
  }
}

WTableCell *WTable::elementAt(int row, int column)
{
  expand(row, column, 1, 1);

  WTableRow::TableData& d = itemAt(row, column);

  return d.cell;
}

void WTable::removeCell(WTableCell *item)
{
  removeCell(item->row(), item->column());
}

void WTable::removeCell(int row, int column)
{
  WTableRow::TableData& d = itemAt(row, column);

  delete d.cell;
  d.cell = new WTableCell(rows_[row], column);
}

void WTable::expand(int row, int column, int rowSpan, int columnSpan)
{
  int newNumRows = row + rowSpan;
  int curNumColumns = numColumns();
  int newNumColumns = std::max(curNumColumns, column + columnSpan);

  if ((newNumRows > numRows())
      || (newNumColumns > curNumColumns)) {
    gridChanged_ = true;
    repaint();

    for (int row = numRows(); row < newNumRows; ++row) {
      rows_.push_back(new WTableRow(this, newNumColumns));
    }

    if (newNumColumns > curNumColumns) {
      for (int row = 0; row < numRows(); ++row) {
	WTableRow *tr = rows_[row];
	tr->expand(newNumColumns);
      }
    }
  }

  //printDebug();
}

int WTable::numRows() const
{
  return rows_.size();
}

int WTable::numColumns() const
{
  return rows_.size() > 0 ? rows_[0]->cells_.size() : 0;
}

void WTable::insertRow(int row)
{
  rows_.insert(rows_.begin() + row, new WTableRow(this, numColumns()));
  gridChanged_ = true;
  repaint();
}

void WTable::insertColumn(int column)
{
  for (unsigned i = 0; i < rows_.size(); ++i) {
    rows_[i]->insertColumn(column);
  }

  gridChanged_ = true;
  repaint();
}

void WTable::deleteRow(int row)
{
  for (int i = 0; i < numColumns(); ++i)
    delete rows_[row]->cells_[i].cell;

  delete rows_[row];
  rows_.erase(rows_.begin() + row);

  gridChanged_ = true;
  repaint();
}

void WTable::clear()
{
  while (numRows() > 0)
    deleteRow(numRows() - 1);
}

void WTable::setCellPadding(int pixels)
{
  cellPadding_ = pixels;
}

void WTable::updateDom(DomElement& element, bool all)
{
  WInteractWidget::updateDom(element, all);

  renderOk();
}

DomElement *WTable::createDomElement()
{
  //printDebug();

  DomElement *table = DomElement::createNew(DomElement::TABLE);
  table->setId(this);
  DomElement *tbody = DomElement::createNew(DomElement::TBODY);

  for (unsigned row = 0; row < (unsigned)numRows(); ++row)
    for (unsigned col = 0; col < (unsigned)numColumns(); ++col)
      itemAt(row, col).overSpanned = false;
  
  for (unsigned row = 0; row < (unsigned)numRows(); ++row) {
    DomElement *tr = DomElement::createNew(DomElement::TR);
    tr->setId(rows_[row]);
    
    for (unsigned col = 0; col < (unsigned)numColumns(); ++col) {
      WTableRow::TableData& d = itemAt(row, col);

      if (!d.overSpanned) {
	DomElement *td = d.cell->createSDomElement();
	tr->addChild(td);

	for (int i = 0; i < d.cell->rowSpan(); ++i)
	  for (int j = 0; j < d.cell->columnSpan(); ++j)
	    if (i + j > 0)
	      itemAt(row + i, col + j).overSpanned = true;
      }
    }

    tbody->addChild(tr);
  }

  table->addChild(tbody);

  updateDom(*table, true);

  gridChanged_ = false;

  return table;
}

void WTable::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate) {
    DomElement *e = DomElement::getForUpdate(this, DomElement::TABLE);

    if (gridChanged_) {
      DomElement *newE = createDomElement();
      e->replaceWith(newE);
    } else {
      updateDom(*e, false);
    }

    result.push_back(e);
  }
}

WTableRow::TableData& WTable::itemAt(int row, int column)
{
  return rows_[row]->cells_[column];
}

}
