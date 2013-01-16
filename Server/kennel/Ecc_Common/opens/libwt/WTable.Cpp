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
#include <Algorithm>

#include <iostream>
//#include "WTable"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WTableRow::slots_[] = { WSlot_() };
WSignal_ WTableRow::signals_[] = { WSignal_() };

void WTableRow::buildSlotMap(WSlotMap& result)
{
  WContainerWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WTableRow::buildSignalMap(WSignalMap& result)
{
  WContainerWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WTableRow::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WContainerWidget::triggerSlot(sender, slot, args);
}
bool WTableRow::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WContainerWidget::undoTriggerSlot(slot, args);
}
//---------------------wangpeng---------------------------------------
DomElement *WTableRow::createDomElement()
{
  DomElement *tr = DomElement::createNew(DomElement::TR);
  tr->setId(this);
  
  updateDom(*tr, true);

  renderOk();

  return tr;
}
void WTableRow::updateDom(DomElement& element, bool all)
{

	WContainerWidget::updateDom(element, all);
}


//--------------------------------wangpeng------------------------------------------------
WSlot_ WTable::slots_[] = { WSlot_() };
WSignal_ WTable::signals_[] = { WSignal_() };

void WTable::buildSlotMap(WSlotMap& result)
{
  WInteractWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WTable::buildSignalMap(WSignalMap& result)
{
  WInteractWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WTable::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WInteractWidget::triggerSlot(sender, slot, args);
}
bool WTable::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WInteractWidget::undoTriggerSlot(slot, args);
}


WTableRow::WTableRow(WTable *table, int numCells)
  : WContainerWidget(0),
  table_(table)
  
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
	return std::find(table_->rows_.begin(), table_->rows_.end(), this)
    - table_->rows_.begin();
}

WTable::WTable(WContainerWidget *parent)
  : WInteractWidget(parent),
    cellPadding_(0),
    gridChanged_(false),
    paddingChanged_(false),
	tableprop_(1)
{ 
	tableprop = NULL;
	cellSpacing_ =0;
  setInline(false);
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

void WTable::insertRow(int row, std::string property)
{
  WTableRow* temp =	new WTableRow(this, numColumns());
  temp -> property = property;
  rows_.insert(rows_.begin() + row, temp);
  gridChanged_ = true;
  repaint();
}

void WTable::insertColumn(int column)
{
  for (unsigned i = 0; i < rows_.size(); ++i) 
  {
    rows_[i]->insertColumn(column);
  }

  gridChanged_ = true;
  repaint();
}

void WTable::deleteRow(int row)
{
  for (int i = 0; i < numColumns(); ++i)
  {
//	  FILE *pf = fopen("c:\\deleteRow1.txt", "a+");
//	  fputs("for numColumns\n", pf);
//	  fclose(pf);
	  
    delete rows_[row]->cells_[i].cell;
//	pf = fopen("c:\\deleteRow2.txt", "a+");
//	  fputs("delete rows_[row]->cells_[i].cell\n", pf);
//	  fclose(pf);
	  
  }


  delete rows_[row];
//  FILE *  pf = fopen("c:\\deleteRow3.txt", "a+");
//  fputs("delete rows_[row]\n", pf);
//  fclose(pf);

  rows_.erase(rows_.begin() + row);

//  pf = fopen("c:\\deleteRow4.txt", "a+");
//  fputs("rows_.erase(rows_.begin() + row)\n", pf);
//  fclose(pf);

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

void WTable::SetLine(bool bsetLine)
{
	 setInline(bsetLine);
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
  table->addChild(tbody);

  for (unsigned row = 0; row < (unsigned)numRows(); ++row)
    for (unsigned col = 0; col < (unsigned)numColumns(); ++col)
      itemAt(row, col).overSpanned = false;
  
  for (unsigned row1 = 0; row1 < (unsigned)numRows(); ++row1) {
       WTableRow *wtr;
	   wtr =   GetRow(row1);   //new WTableRow(this,numColumns());
	   //std::string atmp= wtr->styleClass();
	   DomElement *tr = wtr->createSDomElement();     //createDomElement();
	   
		// DomElement *tr = DomElement::createNew(DomElement::TR);

		//tbody->addChild(tr);
	tbody->addChild(tr);
//    tr->setId(rows_[row1]);
    
    for (unsigned col = 0; col < (unsigned)numColumns(); ++col) {
      WTableRow::TableData& d = itemAt(row1, col);

      if (!d.overSpanned) {
	//	std::string atmp= d.cell->styleClass();

	DomElement *td = d.cell->createSDomElement();
	//std::string atmp= d.cell->styleClass();
	tr->addChild(td);

	for (int i = 0; i < d.cell->rowSpan(); ++i)
	  for (int j = 0; j < d.cell->columnSpan(); ++j)
	    if (i + j > 0)
	      itemAt(row1 + i, col + j).overSpanned = true;
      }
    }
  }
  if(cellPadding_!=2)
  {
	  
	  sprintf(strCellPadding,"%d",cellPadding_);
	  table->setAttribute("cellPadding",strCellPadding);
	  //table->setProperty( Wt::PropertyCellPadding,strCellPadding);
  }


  if(cellSpacing_!=2)
  {
	  
	  sprintf(strCellSpacing,"%d",cellSpacing_);
	  //table->setProperty( Wt::PropertyCellSpacing,strCellPadding);
	  table->setAttribute("cellSpacing",strCellSpacing);
  }

   if(tableprop_ != 1)
  {
	char * token = NULL;
		// duplicate string
		char * cp = strdup(tableprop);
		if (cp)
		{
			token = strtok(cp , " ");
			// every field
			while( token != NULL )
			{
				int pos2 = 0;
				std::string temp = token;
				if(!temp.empty())
			{
				int pos2 = temp.find("=", 0);
				std::string szAttrName = temp.substr(0, pos2 );
				std::string szAttrValue = temp.substr(pos2 + 1, temp.size() - pos2 - 1);
				table->setAttribute(szAttrName.c_str(), szAttrValue.c_str());
			}

				token = strtok( NULL , " ");
			}
			free(cp);
		}
  }

  updateDom(*table, true);

  gridChanged_ = false;

  return table;
}

void WTable::getDomChanges(std::vector<DomElement *>& result)
{
  if (renderState() == RenderUpdate)
  {
    DomElement *e = DomElement::getForUpdate(this, DomElement::TABLE);

    if (gridChanged_) 
	{
      DomElement *newE = createDomElement();
      e->replaceWith(newE);
    } 
	else 
	{
      updateDom(*e, false);
    }

    result.push_back(e);
  }

  for (unsigned row = 0; row < (unsigned)numRows(); ++row)
    for (unsigned col = 0; col < (unsigned)numColumns(); ++col)
      if (!itemAt(row, col).overSpanned)
	itemAt(row, col).cell->getSDomChanges(result);
	
    for (unsigned row = 0; row < (unsigned)numRows(); ++row)
	{
		GetRow(row)->getSDomChanges(result);
	}

}

WTableRow::TableData& WTable::itemAt(int row, int column)
{
  return rows_[row]->cells_[column];
}

WTableRow * WTable::GetRow(int row)
{
	return rows_[row];
}

void WTable::setCellSpaceing(int nSpace)
{
	cellSpacing_ = nSpace;
}

void WTable::adjustRowStyle(std::string strFirstCss,std::string strNextCss)
{
	for(int i=1;i<numRows();i++ )
	{
		if(i%2 ==1)
			GetRow(i)->setStyleClass(strFirstCss);
		else 
			GetRow(i)->setStyleClass(strNextCss);
	}
}
