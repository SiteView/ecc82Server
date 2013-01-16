#include "MainTable.h"

#include <Algorithm>
#include <WText>
#include <WTable>
#include <WImage>
#include <WTableCell>

#include "../../kennel/svdb/svapi/svapi.h"

//////////////////////////////////////////////////////////////////////////////////
// start
CMainTable::CMainTable(WContainerWidget * parent, const std::string strTitle)
:WTable(parent)
{
	bShowHelp = false;

	this->setStyleClass("t1");
	pTitle = new WText(strTitle, (WContainerWidget*)this->elementAt(0,0));

	this->elementAt(0,0)->setStyleClass("t1title");

	//Resource
	OBJECT objRes=LoadResource("default", "localhost");  
	if( objRes !=INVALID_VALUE )
	{	
		MAPNODE ResNode=GetResourceNode(objRes);
		if( ResNode != INVALID_VALUE )
		{
			FindNodeValue(ResNode,"IDS_Translate",strTranslate);
			FindNodeValue(ResNode,"IDS_Translate_Tip",strTranslateTip);
			FindNodeValue(ResNode,"IDS_Refresh",strRefresh);
			FindNodeValue(ResNode,"IDS_Refresh_Tip",strRefreshTip);
			FindNodeValue(ResNode,"IDS_Help",strHelp);
			FindNodeValue(ResNode,"IDS_Return",strReturn);
		}
		CloseResource(objRes);
	}

	pTranslateBtn = new WPushButton(strTranslate, (WContainerWidget *)this->elementAt(1, 0));
    pTranslateBtn->setToolTip(strTranslateTip);
	pTranslateBtn->hide();

	new WText("&nbsp;&nbsp;&nbsp;&nbsp;", (WContainerWidget *)this->elementAt(1, 0));

	pExChangeBtn = new WPushButton(strRefresh, (WContainerWidget *)this->elementAt(1, 0));
	pExChangeBtn->setToolTip(strRefreshTip);
	pExChangeBtn->hide();

	new WText("&nbsp;&nbsp;&nbsp;&nbsp;", (WContainerWidget *)this->elementAt(1, 0));
	
	m_pHelpimg = new WImage("../Images/help.gif", (WContainerWidget *)this->elementAt(1, 0));
	this->elementAt(1, 0)->setContentAlignment(AlignRight);
	m_pHelpimg ->setStyleClass("helpimg");
    m_pHelpimg ->setToolTip(strHelp);

	strBack = strReturn;
	pBackBtn = new WPushButton(strBack, (WContainerWidget *)this->elementAt(1, 0));
	this->elementAt(1, 0)->setContentAlignment(AlignRight);
	pBackBtn->setStyleClass("bg_button");
    pBackBtn->setToolTip(strBack);
	pBackBtn->hide();

	WObject::connect(m_pHelpimg,SIGNAL(clicked()),this,SLOT(ShowOrHideHelp()));

}


void CMainTable::ShowOrHideHelp()
{
	if(bShowHelp)
	{
		bShowHelp = false;
		for (unsigned i = 0; i < m_pListHelpText.size(); ++i)
		{
			m_pListHelpText[i]->hide();
		}

	}
	else
	{
		bShowHelp = true;
		for (unsigned i = 0; i < m_pListHelpText.size(); ++i)
		{
			m_pListHelpText[i]->show();
		}
	}
}

void CMainTable::ShowErrorMsg(std::list<string> msgList)
{
	for (unsigned j = 0; j < m_pListErrorText.size(); j++)
	{
		m_pListErrorText[j]->hide();
	}			

	std::list<string>::iterator msgitem;
	for(msgitem = msgList.begin(); msgitem != msgList.end(); msgitem ++)	
	{
		for (unsigned j = 0; j < m_pListErrorText.size(); j++)
		{
			if(m_pListErrorText[j]->text() == *msgitem)
			{
				m_pListErrorText[j]->show();
				continue;
			}
		}			
	}
}

void CMainTable::HideAllErrorMsg()
{
	for (unsigned j = 0; j < m_pListErrorText.size(); j++)
	{
		m_pListErrorText[j]->hide();
	}			
}

void CMainTable::AddHelpText(WTable* childTable,const std::string strHelp, int nRow, int nCol)
{
	WText* pText = new WText(strHelp, childTable->elementAt(nRow, nCol));
	childTable->elementAt(nRow, nCol)->setStyleClass("helps");
	m_pListHelpText.push_back(pText);
	pText->hide();
}

void CMainTable::AddErrorText(WTable* childTable,const std::string strHelp, int nRow, int nCol)
{
	WText* pText = new WText(strHelp, childTable->elementAt(nRow, nCol));
	childTable->elementAt(nRow, nCol)->setStyleClass("errors");
	m_pListErrorText.push_back(pText);
	pText->hide();
}

void CMainTable:: ShowBackBtn()
{
	m_pHelpimg->hide();
	pBackBtn->show();
}

void  CMainTable::ShowHelpBtn()
{
	m_pHelpimg->show();
	pBackBtn->hide();
}
