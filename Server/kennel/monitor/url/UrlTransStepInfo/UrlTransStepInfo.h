#pragma once

#include "WContainerWidget"
#include <string>
#include <list>
#include <iostream>
using namespace std;

#include "../../kennel/svdb/svapi/svapi.h"
#include "../../kennel/svdb/svapi/svdbapi.h"

class WTable;
class WText;
class WImage;
class WLineEdit;
class CFlexTable;
class WComboBox;
class CMainTable;
class WApplication;
class WPushButton;
class WSelectionBox;
class WSVMainTable;
class WSVFlexTable;
class WSVButton;
class WRadioButton;
class WTextArea;
#include "WSignalMapper"

class CUrlTransStepInfo :WContainerWidget
{
    //MOC: W_OBJECT CUrlTransStepInfo:WContainerWidget
    W_OBJECT;
public:
    CUrlTransStepInfo(WContainerWidget *parent = 0);

public : //add user var	

	//列表
	CMainTable * pMainTable;
	
	WSVMainTable *m_pMainTable;
	WSVFlexTable *m_pStepInfoTable;
	WSVFlexTable *m_pStepListTable;

	WLineEdit * pUrl;
	//WComboBox * pLinks;	
	WSelectionBox * pLinks;	
	WComboBox * pForms;
	WTextArea * pPostData;
	//WLineEdit * pPostData;
	WComboBox * pFrames;
	WComboBox * pRefreshs;

	WRadioButton * p_UrlBtn;
	WRadioButton * p_LinksBtn;
	WRadioButton * p_FormsBtn;
	WRadioButton * p_FramesBtn;
	WRadioButton * p_RefreshsBtn;

	WLineEdit * pContent;
	WLineEdit * pErrorContent;
	WLineEdit * pUserName;
	WLineEdit * pUserPwd;

	//
	WPushButton * pHideButton;
	string szButNum,szButMatch, strIsAlertDel;
	string szTipSelAll,szTipNotSelAll,szTipInvSel;
	//
	WApplication *  appSelf;

	//
	typedef struct _Step_LIST 
	{
		int nIndex ;            // 索引
		string id;
		WCheckBox * pSelect;    // 选择框
		
		WText * pStepNameText;          // 步名称
		WText * pStepTypeText;          // 步类型
		
		string strPostData;
		string strContent;
		string strErrorContent;
		string strUserName;
		string strUserPwd;

	}Step_LIST, *LPSTEP_LIST;

	// 列表
	std::list<Step_LIST> m_pListStep;
	//列表中的表项
	std::list<Step_LIST>::iterator m_pListItem;
	
	list<string> lstLinks, lstForms, lstPostDatas;
	list<string>::iterator listitem;

	string strProxyServerPort,	strProxyUser, strProxyPass, strTimeOut, strCurStepHtml;
	int nLimitStep;
	WSignalMapper m_stepMapper;     // 

	string strSave,strCancel,strEdit,strTitle,strStepNameLabel,strStepTypeLabel,strStepListLabel,strDel,strAddStep,strStepInfo,strBaseInfo,
		   strUrlsLabel, strLinksLabel, strFormsLabel,strFramesLabel,strRefreshsLabel,strPostDatasLabel,strAdvanceInfo,strConentLabel,strErrorConentLabel,
			strUserNameLabel, strUserPwdLabel, strShowHtml, strDelStepAffirm, strStepLimit, strStepDelTip, strStepEditTip;
public:
	void ShowMainTable();
	
	void AddListItem(string strIndex, string strStepName, string strStepType, 
	string strPostData, string strContent, string strErrorContent, string strUserName, string strUserPwd, int numRow);
	
	string MakeInputStrByStep(string strIndex, string strStepName, string strStepType, 
		string strPostData, string strContent, string strErrorContent, string strUserName, string strUserPwd);
	
	void PaserStep1();
	//void ShowHtml();
	string strIniFileName;
	string strMonitorId;

	string strInitEditIndex, strCurEditIndex, strCurAction;
	virtual void refresh();

	~CUrlTransStepInfo(void);
public : //language;

	string strIdcUserId;

private slots:
	//MOC: SLOT CUrlTransStepInfo::ShowHelp()
	void ShowHelp();
	//MOC: SLOT CUrlTransStepInfo::AddStep()
	void AddStep();
	//MOC: SLOT CUrlTransStepInfo::SaveStep()
	void SaveStep();
	//MOC: SLOT CUrlTransStepInfo::CancelStep()
	void CancelStep();
    //MOC: SLOT CUrlTransStepInfo::EditStep(const std::string)
    void EditStep(const std::string strIndex);
    //MOC: SLOT CUrlTransStepInfo::SelAll()
    void SelAll();
	//MOC: SLOT CUrlTransStepInfo::SelNone()
	void SelNone();
	//MOC: SLOT CUrlTransStepInfo::SelInvert()
	void SelInvert();
	//MOC: SLOT CUrlTransStepInfo::BeforeDelStep()
	void BeforeDelStep();
	//MOC: SLOT CUrlTransStepInfo::DelStep()
	void DelStep();
	//MOC: SLOT CUrlTransStepInfo::TestMonitor()
	void TestMonitor();
	//MOC: SLOT CUrlTransStepInfo::SelFormsChanged()
	void SelFormsChanged();	
	//MOC: SLOT CUrlTransStepInfo::ShowHtml()
	void ShowHtml();

};