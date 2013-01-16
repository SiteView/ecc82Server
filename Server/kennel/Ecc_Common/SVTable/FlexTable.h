#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _FLEXTABLE_H_
#define _FLEXTABLE_H_

#include <list>

using namespace std;

#include <WText>
#include <WTable>
#include <WImage>
#include <WTableCell>
#include <WContainerWidget>
#include <WPushButton>


//////////////////////////////////////////////////////////////////////////////////
// class CFlexTable
class CFlexTable : public WTable
{
    //MOC: W_OBJECT CFlexTable:WTable
    W_OBJECT;
public:
    CFlexTable(WContainerWidget * parent, const std::string strTitle);
	CFlexTable();
	
	void InitGroupOperate(WTable * pTable);
	void AddGroupOperate(WTable * pTable, std::string szValDel);
	void AddGroupAddBtn(WTable * pTable,std::string szAddRelPlanBut);
public :
    inline WTable * GetContentTable()
	{
		return m_pContentTable;
	}
public:
	typedef struct _FORM_SHOW_TEXT
    {
        string szTipSelAll;
		string szTipNotSelAll;
		string szTipInvSel;
		string szTipAddNew;
		string szAddRelPlanBut;
		string szTipDel;
    public :
        _FORM_SHOW_TEXT()
        {
/*			szTipSelAll = "全选";
			szTipNotSelAll = "全不选";
			szTipInvSel = "反选";
			szTipAddNew = "添加";
			szTipDel = "删除";
*/			//szValDel = "你确认删除任务计划吗？";
        }
    }FORM_SHOW_TEXT;

	FORM_SHOW_TEXT m_FormShowText;
	WTable * m_pGroupOperate;
/*	WText * pSelAll;
	WText * pSelNone;
	WText * pSelinvert;
	WText * pvDel;
	WText * pDel;
*/
	WImage * pSelAll;
	WImage * pSelNone;
	WImage * pSelinvert;
	WImage * pvDel;
	WImage * pDel;

	WText * pTitle;
	WPushButton *pAdd;
	std::vector<WImage *> m_pOtherBtn;	
	//void AddHelpText(const std::string strHelp, int nRow, int nCol);
public slots:
    //MOC: SLOT CFlexTable::ShowTable()
    void ShowTable();
    //MOC: SLOT CFlexTable::HideTable()
    void HideTable();
private:
	WTable * m_pContentTable;
	WImage * m_pOpenimg;
	WImage * m_pCloseimg;

	//Group Operate Image;

	//WImage * m_pHelpOpenimg;
	//WImage * m_pHelpCloseimg;

	//std::vector<WText *> m_pListHelpText ;
private :
	;
};

#endif