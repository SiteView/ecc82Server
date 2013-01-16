#include ".\WSPopTable.h"

#include "WText"
#include "WImage"
#include "WTableCell"
#include "WScrollArea"
#include "windows.h"
//#include "webase.h"

WSPopTable::WSPopTable(WContainerWidget * parent, const bool bHelp_)
:WTable(parent)
{
	//
	this->setStyleClass("pop_bg");
	this->resize(WLength(100, WLength::Percentage),WLength());
	this->elementAt(0, 0)->setStyleClass("tr_align");
	
	//
	pFirstTable = new WTable((WContainerWidget *)this->elementAt(0, 0));
	pFirstTable->setStyleClass("padding_top");
	pFirstTable->resize(WLength(100, WLength::Percentage), WLength());

	bHelp = bHelp_;
}

//返回指定栏目的内容Table
WTable * WSPopTable::GeRowContentTable(int i)
{	
	if(i < m_pListContentTable.size())
		return m_pListContentTable[i];
	else
		return NULL;
}

//返回指定栏目的动作按钮Table
WTable * WSPopTable::GeRowActionTable(int i)
{
	if(i < m_pListActionTable.size())
		return m_pListActionTable[i];
	else
		return NULL;
}

//添加栏目
void WSPopTable::AppendRows(std::string strRowName)
{
	int iLast = pFirstTable->numRows();
	iLast++;

	//RowTitle
	WTable * pTitleTable = new WTable((WContainerWidget *)pFirstTable->elementAt(iLast,0));
	pTitleTable->setStyleClass("padding_5");
	pTitleTable->resize(WLength(100,WLength::Percentage),WLength());
	pTitleTable->elementAt(0,0)->setStyleClass("textbold");
	pTitleText = new WText(strRowName, pTitleTable->elementAt(0,0));

	//Help
	if(bHelp)
	{
		pTitleTable->elementAt(0, 1)->setContentAlignment(AlignRight|AlignTop);
		pHelpImg = new WImage("/Images/help.png","", pTitleTable->elementAt(0, 1));
		pHelpImg->setStyleClass("hand");
	}

	//RowContent
	iLast++;
	pFirstTable->elementAt(iLast,0)->setStyleClass("padding_2");
	
	WTable * pRowContentTable = new WTable((WContainerWidget *)pFirstTable->elementAt(iLast, 0));
	
	pRowContentTable->resize(WLength(100,WLength::Percentage),WLength());
	
	//top
	ShowTop(pRowContentTable);

	//content
	pRowContentTable->elementAt(1, 0)->setStyleClass("pop_table_mid_l");	
	
	WTable * pReturnTable = new WTable((WContainerWidget *)pRowContentTable->elementAt(1, 1));
	
	pReturnTable->resize(WLength(100,WLength::Percentage),WLength());
	pReturnTable->setStyleClass("bgcolor");

	//
	pReturnTable->elementAt(0, 0)->setStyleClass("padding_5");
	m_pListContentTable.push_back(new WTable((WContainerWidget *)pReturnTable->elementAt(0, 0)));

	//分隔线
	pReturnTable->elementAt(1,0)->setStyleClass("padding_5");
	WTable * pLineTable = new WTable((WContainerWidget *)pReturnTable->elementAt(1, 0));
	pLineTable->resize(WLength(100,WLength::Percentage), WLength());
	pLineTable->elementAt(0, 0)->setStyleClass("table_data_input_space");
	new WImage("/Images/space.gif","",pLineTable->elementAt(0, 0));
	
	//ActionTable
	pReturnTable->elementAt(2,0)->setStyleClass("padding_2");
	m_pListActionTable.push_back(new WTable((WContainerWidget *)pReturnTable->elementAt(2,0)));

	pRowContentTable->elementAt(1, 2)->setStyleClass("pop_table_mid_r");
	
	//bottom
	ShowBottom(pRowContentTable);
}

//顶部边框
void WSPopTable::ShowTop(WTable * pTable)
{
	//this->elementAt(1,0)->setStyleClass("table_shadow_l");
	new WImage("/Images/pop_table_top_l.png","",pTable->elementAt(0,0));
	pTable->elementAt(0,1)->setStyleClass("pop_table_top");
	new WImage("/Images/pop_table_top_r.png","",pTable->elementAt(0,2));
	//this->elementAt(1,2)->setStyleClass("table_shadow_r");
}

//底部边框
void WSPopTable::ShowBottom(WTable * pTable)
{
	//this->elementAt(1,0)->setStyleClass("table_shadow_l");
	new WImage("/Images/pop_table_bom_l.png","",pTable->elementAt(2,0));
	pTable->elementAt(2,1)->setStyleClass("pop_table_bom");
	new WImage("/Images/pop_table_bom_r.png","",pTable->elementAt(2,2));
	//this->elementAt(1,2)->setStyleClass("table_shadow_r");
}


void WSPopTable::BtnProc()
{
	
}

void WSPopTable::refresh()
{
	
}

