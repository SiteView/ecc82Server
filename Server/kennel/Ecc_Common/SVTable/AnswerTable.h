#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _ANSWETABLE_H_
#define _ANSWETABLE_H_

#include <list>

using namespace std;

#include <WText>
#include <WTable>
#include <WImage>
#include <WTableCell>
#include <WContainerWidget>
#include <WPushButton>

//////////////////////////////////////////////////////////////////////////////////
// class CAnswerTable
class CAnswerTable : public WTable
{
    //MOC: W_OBJECT CAnswerTable:WTable
    W_OBJECT;
public:
    CAnswerTable(WContainerWidget * parent, const std::string strTitle);
public :
	string strSaveAndAdd;
	string strSave;
	string strCancel;
	WPushButton * pTranslateBtn;
	WPushButton * pExChangeBtn;
	WPushButton * pSaveAndAdd;
	WPushButton * pSave;
	WPushButton * pCancel;
	std::vector<WText *> m_pListHelpText ;
	std::vector<WText *> m_pListErrorText ;
	void AddHelpText(WTable* childTable,const std::string strHelp, int nRow, int nCol);
	void AddErrorText(WTable* childTable,const std::string strHelp, int nRow, int nCol);
    void ShowErrorMsg(std::list<string> msgList);
	void HideAllErrorMsg();
    inline WTable * GetContentTable()
	{
		return m_pContentTable;
	}
	WImage * m_pHelpimg;
private slots:
    //MOC: SLOT CAnswerTable::ShowOrHideHelp()
    void ShowOrHideHelp();
private:
	WTable * m_pContentTable;
	
	bool bShowHelp;	
private :
	string strTranslate;
	string strTranslateTip;
	string strRefresh;
	string strRefreshTip;
	string strHelp;
};

#endif