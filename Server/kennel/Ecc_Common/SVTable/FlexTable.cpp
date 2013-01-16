#include "FlexTable.h"

#include <Algorithm>
#include <WText>
#include <WTable>
#include <WImage>
#include <WTableCell>
#include <WPushButton>

#include "../../kennel/svdb/svapi/svapi.h"

//////////////////////////////////////////////////////////////////////////////////
// start
CFlexTable::CFlexTable()
{	
	//Resource
	OBJECT objRes=LoadResource("default", "localhost");  
	if( objRes !=INVALID_VALUE )
	{	
		MAPNODE ResNode=GetResourceNode(objRes);
		if( ResNode != INVALID_VALUE )
		{
			FindNodeValue(ResNode,"IDS_All_Select",m_FormShowText.szTipSelAll);
			FindNodeValue(ResNode,"IDS_None_Select",m_FormShowText.szTipNotSelAll);
			FindNodeValue(ResNode,"IDS_Invert_Select",m_FormShowText.szTipInvSel);
			FindNodeValue(ResNode,"IDS_Delete",m_FormShowText.szTipDel);
		}
		CloseResource(objRes);
	}

	m_pGroupOperate = NULL;
	pSelAll = NULL;
	pSelNone = NULL;
	pSelinvert = NULL;
	pDel = NULL;
	pAdd = NULL;
}
CFlexTable::CFlexTable(WContainerWidget * parent, const std::string strTitle)
:WTable(parent)
{
	//Resource
	OBJECT objRes=LoadResource("default", "localhost");  
	if( objRes !=INVALID_VALUE )
	{	
		MAPNODE ResNode=GetResourceNode(objRes);
		if( ResNode != INVALID_VALUE )
		{
			FindNodeValue(ResNode,"IDS_All_Select",m_FormShowText.szTipSelAll);
			FindNodeValue(ResNode,"IDS_None_Select",m_FormShowText.szTipNotSelAll);
			FindNodeValue(ResNode,"IDS_Invert_Select",m_FormShowText.szTipInvSel);
			FindNodeValue(ResNode,"IDS_Delete",m_FormShowText.szTipDel);
		}
		CloseResource(objRes);
	}

	m_pOpenimg = new WImage("../Images/open.gif", (WContainerWidget *)this->elementAt( 0, 0)); 
	m_pCloseimg = new WImage("../Images/close.gif", (WContainerWidget *)this->elementAt( 0, 0));
	m_pOpenimg->decorationStyle().setCursor(WCssDecorationStyle::Pointer);
	m_pCloseimg->decorationStyle().setCursor(WCssDecorationStyle::Pointer);

	m_pOpenimg->hide();
	WObject::connect(m_pOpenimg,SIGNAL(clicked()),this,SLOT(ShowTable()));
	WObject::connect(m_pCloseimg,SIGNAL(clicked()),this,SLOT(HideTable()));	
	new WText(strTitle, (WContainerWidget*)this->elementAt(0,0));

	this->setStyleClass("t2");
	this->elementAt(0,0)->setStyleClass("t2title");

	m_pContentTable = new WTable((WContainerWidget*)this->elementAt(1,0)) ;	
	m_pContentTable->setStyleClass("t3");
}


void CFlexTable::ShowTable()
{
	m_pContentTable->show();
	m_pOpenimg->hide();
	m_pCloseimg->show();
}

void CFlexTable::HideTable()
{
	m_pContentTable->hide();
	m_pOpenimg->show();
	m_pCloseimg->hide();	
}

void CFlexTable::InitGroupOperate(WTable * pTable)
{
    m_pGroupOperate = new WTable((WContainerWidget *)pTable->elementAt( 2, 0));
}

void CFlexTable::AddGroupOperate(WTable * pTable, std::string szValDel)
{
   m_pGroupOperate = new WTable((WContainerWidget *)pTable->elementAt( 2, 0));

    if ( m_pGroupOperate )
    {
        pSelAll = new WImage("../Images/selall.gif", (WContainerWidget *)m_pGroupOperate->elementAt(0, 1));
        if (pSelAll)
        {
            pSelAll->setStyleClass("imgbutton");
			pSelAll->setToolTip(m_FormShowText.szTipSelAll);
        }

        pSelNone = new WImage("../Images/selnone.gif", (WContainerWidget *)m_pGroupOperate->elementAt(0, 2));
        if (pSelNone)
        {
            pSelNone->setStyleClass("imgbutton");
			pSelNone->setToolTip(m_FormShowText.szTipNotSelAll);
        }

        pSelinvert = new WImage("../Images/selinvert.gif", (WContainerWidget *)m_pGroupOperate->elementAt(0, 3));
        if (pSelinvert)
        {
            pSelinvert->setStyleClass("imgbutton");
			pSelinvert->setToolTip(m_FormShowText.szTipInvSel);
        }

		pvDel = new WImage("../Images/del.gif", (WContainerWidget *)m_pGroupOperate->elementAt(0, 4));
        if (pvDel)
        {
           
			pvDel->setStyleClass("imgbutton");
			pvDel->setToolTip(m_FormShowText.szTipDel);
			pvDel->hide();
        }

		pDel = new WImage("../Images/del.gif", (WContainerWidget *)m_pGroupOperate->elementAt(0, 4));
        if (pDel)
        {           
			pDel->setStyleClass("imgbutton");
			pDel->setToolTip(m_FormShowText.szTipDel);
        }
//		pDel ->hide();	
	}
}

void CFlexTable::AddGroupAddBtn(WTable * pTable,std::string szAddRelPlanBut)
{
	int nColumns = m_pGroupOperate->numColumns();
	pAdd = new WPushButton(szAddRelPlanBut, (WContainerWidget *)m_pGroupOperate->elementAt(0, nColumns + 1));
    if (pAdd)
    {
        pAdd->setToolTip(m_FormShowText.szTipAddNew);
		pAdd->setStyleClass("wizardbutton");
		m_pGroupOperate->elementAt(0, nColumns + 1)->resize(WLength(100,WLength::Percentage),WLength(100,WLength::Percentage));
		m_pGroupOperate->elementAt(0, nColumns + 1)->setContentAlignment(AlignRight);		
    }
}