/*
 * Copyright (C) 2005 Koen Deforche, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#include <boost/lexical_cast.hpp>

#include "WTableCell"
#include "WTable"
#include "DomElement.h"

#include <iostream>
//#include "WTableCell"
#include <WSignal_>
#include <WSignalInstance_>
#include <WSlot_>

#include <WSlotInstance_>

WSlot_ WTableCell::slots_[] = { WSlot_() };
WSignal_ WTableCell::signals_[] = { WSignal_() };

void WTableCell::buildSlotMap(WSlotMap& result)
{
  WContainerWidget::buildSlotMap(result);
  for (int i = 0; slots_[i].good(); ++i)
    result[slots_[i].name()] = new WSlotInstance_(this, slots_ + i);
}

void WTableCell::buildSignalMap(WSignalMap& result)
{
  WContainerWidget::buildSignalMap(result);
  for (int i = 0; signals_[i].good(); ++i)
    result[signals_[i].name()] = new WSignalInstance_(this, signals_ + i);
}

bool WTableCell::triggerSlot(WObject *sender, const WSlot_ *slot, void **args)
{
  return WContainerWidget::triggerSlot(sender, slot, args);
}
bool WTableCell::undoTriggerSlot(const WSlot_ *slot, void **args)
{
  return WContainerWidget::undoTriggerSlot(slot, args);
}


WTableCell::WTableCell(WTableRow *row, int column)
  : WContainerWidget(0),
    row_(row),
    column_(column),
    rowSpan_(1),
    columnSpan_(1),
	tablecellprop_(1)
{
	tablecellprop = NULL;
  setParent(row->table());
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
/*	if (refChanged_ || all) {
		element.setAttribute("href", ref_);
		refChanged_ = false;
	}
*/	
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

  
  if(tablecellprop_ != 1)
  {
	char * token = NULL;
		// duplicate string
		char * cp = strdup(tablecellprop);
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
				td->setAttribute(szAttrName.c_str(), szAttrValue.c_str());
			}

				token = strtok( NULL , " ");
			}
			free(cp);
		}
  }
 createDomChildren(*td);

  updateDom(*td, true);

  renderOk();

  return td;
}


void WTableCell::SetMouseMove(const std::string strMsOver,const std::string strMsWent)
{
	
	
}