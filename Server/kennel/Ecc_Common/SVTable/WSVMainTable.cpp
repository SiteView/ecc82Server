#include ".\wsvmaintable.h"
#include "WText"
//#include "WSVFlexTable.h"
#include "WImage"
#include "WTableCell"

WSVMainTable::WSVMainTable(WContainerWidget * parent, const std::string strTitle, const bool bHelp_)
:WTable(parent)
{
	bHelp = bHelp_;
//	pThreeTable =NULL;
	//第一层table
	this->setStyleClass("padding_top");

	//第二层table
	pMainTable  = new WTable((WContainerWidget *)this->elementAt(0,0));
	//this->GetRow(0)->setStyleClass("padding_top");
	//pMainTable->setStyleClass("padding_top");

	//第三层table
	WTable* pTopViewTable = new WTable((WContainerWidget *)pMainTable->elementAt(0,0));
	pTopViewTable->setStyleClass("padding_2");
	
	pTitleTxt = new WText(strTitle,(WContainerWidget *)pTopViewTable->elementAt(0,0));
	pTopViewTable->elementAt(0,0)->setStyleClass("textbold");
	strcpy(pTopViewTable->elementAt(0,0)->contextmenu_,"nowrap");

	if(bHelp)
	{
		pTopViewTable->elementAt(0, 1)->setContentAlignment(AlignRight);
		pHelpImg = new WImage("/Images/help.png","", pTopViewTable->elementAt(0, 1));
		pHelpImg->setStyleClass("hand");
	}
}
//WSVFlexTable * WSVMainTable::GetContentTable(std::string strFlexTitle)
WTable * WSVMainTable::GetContentTable()
{
	/*
	//第三层table
	if(pThreeTable==NULL)
	{
		 pThreeTable = new WSVFlexTable((WContainerWidget *)pMainTable->elementAt(1,0),strFlexTitle);  
		pThreeTable->setStyleClass("width100");
	}
	return pThreeTable;
	*/
	return pMainTable;
}

bool WSVMainTable::HaveHelp()
{
	return bHelp;
}

WSVMainTable::~WSVMainTable(void)
{
	
}