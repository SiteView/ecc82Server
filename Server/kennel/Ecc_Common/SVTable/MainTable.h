#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _MAINTABLE_H_
#define _MAINTABLE_H_

#include <list>

using namespace std;

#include <WText>
#include <WTable>
#include <WImage>
#include <WTableCell>
#include <WContainerWidget>
#include <WPushButton>

//////////////////////////////////////////////////////////////////////////////////
// class CMainTable
class CMainTable : public WTable
{
    //MOC: W_OBJECT CMainTable:WTable
    W_OBJECT;
public:
    CMainTable(WContainerWidget * parent, const std::string strTitle);
public :
	std::vector<WText *> m_pListHelpText ;
	std::vector<WText *> m_pListErrorText ;
	void AddHelpText(WTable* childTable,const std::string strHelp, int nRow, int nCol);
	void AddErrorText(WTable* childTable,const std::string strHelp, int nRow, int nCol);
    void ShowErrorMsg(std::list<string> msgList);
	void HideAllErrorMsg();

	string strBack;
	WText * pTitle;
	WPushButton * pBackBtn;
	WPushButton * pTranslateBtn;
	WPushButton * pExChangeBtn;
	WImage * m_pHelpimg;
	void ShowBackBtn();
	void ShowHelpBtn();
private slots:
    //MOC: SLOT CMainTable::ShowOrHideHelp()
    void ShowOrHideHelp();
private:
	bool bShowHelp;	
	string strTranslate;
	string strTranslateTip;
	string strRefresh;
	string strRefreshTip;
	string strHelp;
	string strReturn;
private :
	;
};

#endif