#pragma once

#include "WTable"
#include <list>

using namespace std;
class WImage;
class WText;
class WTable;

enum TableType {Blank, ReportList, List, Group, EntityList, EntityDes, Query, AlertSel};

class WSVFlexTable :public WTable
{
public:
	//MOC: W_OBJECT WSVFlexTable:WTable
    W_OBJECT;

public:
	WSVFlexTable(WContainerWidget * parent, int TableType, const std::string strTitle, const bool showImg = true);
	//virtual ~WSVFlexTable(void);
public :

	WTable * GetContentTable();
	WTable * GetActionTable();
	virtual void refresh();  

	/////////////////////////////////////
	//TreeView.html�ĺ���(List)
	/////////////////////////////////////	
	string strDivId;
	std::vector<WLength> m_pColumnWidth;
	std::vector<string> m_pRowDataCss;
	//map<int, WLength, less<int> > m_pColumnWidth;
	
	WTable * GeDataTable();
	WText * AppendColumn(std::string strColName, WLength width_);
	void AppendColumn(std::string strBigColName, std::string strSmallColName, WLength width_);
	
	void InitRow(int nRow);
	void  SetDataRowStyle(string strCss);
	void SetDivId(string strIn);
	
	string  GetDivId();

	void ShowNullTip();
	void HideNullTip();
	void SetNullTipInfo(string strInfo);

	void AddStandardSelLink(std::string strSelall,std::string strSelNone,std::string strSelReverse );
	
	WTable * dataTitleTable;
	WTable * dataGridTable;
	WTable * pNullTipTable;

	//add by andiliu  ʹ������Կ��Ʊ���
	WText *pTitleTxt;

	/////////////////////////////////////
	//AddGroup.html�ĺ���
	/////////////////////////////////////	
	
	bool bShowHelp;	

	bool bShowImg;
	std::vector<WTable *> m_pListRowTable;
	bool AppendRows(std::string strColName);
	WContainerWidget * AppendRowsContent(int nRow, string strLabel, string strDes, string strError, bool noBlank = false);

	std::vector<WText *> m_pListHelpText;
	std::vector<WText *> m_pListErrorText ;
    void ShowErrorMsg(std::list<string> msgList);
	void HideAllErrorMsg();
	void ShowOrHideHelp();

	//////////////////////////
	//AlertLog.htm�ĺ���
	//////////////////////////
	//WTable * dataGridTable; Ҳ�ô�Talbe
	void InitTable();
	void AppendRows();
	WContainerWidget * AppendRowsContent(int nRows, int nColumn, int nColumnCount, string strLabel, string strDes, string strError, bool noBlank= false);

	//////////////////////////
	//EntityList.htm�ĺ���
	//////////////////////////
	void AppendEntityRow(string strName, string strHref, string strDes);

	//////////////////////////
	//EntityList.htm�ĺ���
	//////////////////////////
	void AppendEntityDesRow(string strName, string strDes);

private slots:
private:
	int nType;
	WImage * pShowImg,*pHideImg;

	WTable* pTopViewTable ;
	WTable * FirstTable; 	
	WTable * pAactionTable;

	void ShowBottom();

public: //standard  select link   
	WText *pSelAll;
	WText *pSelNone;
	WText *pSelReverse;
public : //standard button  // ��ֹ��������ɾ�������

private slots:
	//MOC: SLOT WSVFlexTable::ShowHide()
	void ShowHide();
	

};
