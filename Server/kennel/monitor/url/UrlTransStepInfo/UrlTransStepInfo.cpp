//#include <time.h>
#include ".\UrlTransStepInfo.h"
//#include "../../kennel/svdb/svapi/libutil/Time.h"
#include "..\svtable\FlexTable.h"
#include "..\svtable\MainTable.h"

#include "websession.h"
#include <WApplication>
#include <WCheckBox>
#include <WComboBox>
#include <WText>
#include <WLineEdit>
#include <WSelectionBox>
#include <WButtonGroup>
#include <WRadioButton>
#include "WSignalMapper"
#include <WScrollArea>
#include <SVTable.h>
#include <WButtonTab>
#include <WPushButton>
#include <WTextArea>
#include "..\svtable\FlexTable.h"
#include "..\svtable\MainTable.h"
#include "..\svtable\WSVButton.h"
#include "..\svtable\WSVMainTable.h"
#include "..\svtable\WSVFlexTable.h"
#include "../base/OperateLog.h"
//#include ".\useroperatelog.h"
#include "../../kennel/svdb/svapi/svapi.h"
#include "../../kennel/svdb/svapi/idcsvapi.h"
#include "../../kennel/svdb/svapi/svdbapi.h"
using namespace std;
using namespace svutil;

//#define WTGET

//
bool ParserToken(list<string >&pTokenList, const char * pQueryString, char *pSVSeps)
{
    char * token = NULL;
    // duplicate string
	char * cp = ::strdup(pQueryString);
    if (cp)
    {
        char * pTmp = cp;
        if (pSVSeps) // using separators
            token = strtok( pTmp , pSVSeps);
        else // using separators
			return false;
            //token = strtok( pTmp, chDefSeps);
        // every field
        while( token != NULL )
        {
            //triml(token);
            //AddListItem(token);
			pTokenList.push_back(token);
            // next field
            if (pSVSeps)
                token = strtok( NULL , pSVSeps);
            else
               return false;
				//token = strtok( NULL, chDefSeps);
        }
        // free memory
        free(cp);
    }
    return true;
}

//
CUrlTransStepInfo::CUrlTransStepInfo(WContainerWidget *parent ):
WContainerWidget(parent)
{
	SetSvdbAddrByFile("svapi.ini");

	//////////////////modify at 07/07/31
	SetCgiVersionByFile("svapi.ini");
	///////////////////////////////////

	strIdcUserId = GetWebIdcUserID();

	strIniFileName = "1_1_UrlStepInfo.ini";

	string keys="IDS_ConfirmCancel,\
		,IDS_Affirm,\
		,IDS_All_Select,\
		,IDS_None_Select,\
		,IDS_Invert_Select,\
		,IDS_SAVE,\
		,IDS_Cancel1,\
		,IDS_DIList_Edit,\
		,IDS_UrlTransStep_Title,\
		,IDS_UrlTransStep_StepName,\
		,IDS_UrlTransStep_StepType,\
		,IDS_UrlTransStep_StepList,\
		,IDS_UrlTransStep_DelStep,\
		,IDS_UrlTransStep_AddStep,\
		,IDS_UrlTransStep_StepInfo,\
		,IDS_UrlTransStep_BaseInfo,\
		,IDS_UrlTransStep_Url,\
		,IDS_UrlTransStep_Links,\
		,IDS_UrlTransStep_Forms,\
		,IDS_UrlTransStep_Frames,\
		,IDS_UrlTransStep_Refreshs,\
		,IDS_UrlTransStep_pPostData,\
		,IDS_UrlTransStep_AdvanceInfo,\
		,IDS_UrlTransStep_Content,\
		,IDS_UrlTransStep_ErrorContent,\
		,IDS_UrlTransStep_UserName,\
		,IDS_UrlTransStep_UserPwd,\
		,IDS_UrlTransStep_ShowHtml,\
		,IDS_UrlTransStep_DelStepAffirm,\
		,IDS_UrlTransStep_DelStepAffirm,\
		,IDS_UrlTransStep_AllowStepLimit,\
		,IDS_UrlTransStep_AllowStepDel,\
		,IDS_UrlTransStep_AllowStepEdit,";
	//
	OBJECT objRes=LoadResourceByKeys(keys,"default", "localhost");	
	//Resource
	//OBJECT objRes=LoadResource("default", "localhost");  
	if( objRes !=INVALID_VALUE )
	{	
		MAPNODE ResNode=GetResourceNode(objRes);
		if( ResNode != INVALID_VALUE )
		{
			FindNodeValue(ResNode,"IDS_ConfirmCancel",szButNum);
			FindNodeValue(ResNode,"IDS_Affirm",szButMatch);
			FindNodeValue(ResNode,"IDS_All_Select", szTipSelAll);
			FindNodeValue(ResNode,"IDS_None_Select", szTipNotSelAll);
			FindNodeValue(ResNode,"IDS_Invert_Select", szTipInvSel);			
			FindNodeValue(ResNode,"IDS_SAVE", strSave);
			FindNodeValue(ResNode,"IDS_Cancel1", strCancel);
			FindNodeValue(ResNode,"IDS_DIList_Edit", strEdit);
			FindNodeValue(ResNode,"IDS_UrlTransStep_Title", strTitle);
			FindNodeValue(ResNode,"IDS_UrlTransStep_StepName", strStepNameLabel);
			FindNodeValue(ResNode,"IDS_UrlTransStep_StepType", strStepTypeLabel);
			FindNodeValue(ResNode,"IDS_UrlTransStep_StepList", strStepListLabel);
			FindNodeValue(ResNode,"IDS_UrlTransStep_DelStep", strDel);
			FindNodeValue(ResNode,"IDS_UrlTransStep_AddStep", strAddStep);
			FindNodeValue(ResNode,"IDS_UrlTransStep_StepInfo", strStepInfo);
			FindNodeValue(ResNode,"IDS_UrlTransStep_BaseInfo", strBaseInfo);
			FindNodeValue(ResNode,"IDS_UrlTransStep_Url", strUrlsLabel);
			FindNodeValue(ResNode,"IDS_UrlTransStep_Links", strLinksLabel);
			FindNodeValue(ResNode,"IDS_UrlTransStep_Forms", strFormsLabel);
			FindNodeValue(ResNode,"IDS_UrlTransStep_Frames", strFramesLabel);
			FindNodeValue(ResNode,"IDS_UrlTransStep_Refreshs", strRefreshsLabel);
			FindNodeValue(ResNode,"IDS_UrlTransStep_pPostData", strPostDatasLabel);
			FindNodeValue(ResNode,"IDS_UrlTransStep_AdvanceInfo", strAdvanceInfo);
			FindNodeValue(ResNode,"IDS_UrlTransStep_Content", strConentLabel);
			FindNodeValue(ResNode,"IDS_UrlTransStep_ErrorContent", strErrorConentLabel);
			FindNodeValue(ResNode,"IDS_UrlTransStep_UserName", strUserNameLabel);
			FindNodeValue(ResNode,"IDS_UrlTransStep_UserPwd", strUserPwdLabel);
			FindNodeValue(ResNode,"IDS_UrlTransStep_ShowHtml", strShowHtml);
			FindNodeValue(ResNode,"IDS_UrlTransStep_DelStepAffirm", strDelStepAffirm);
			FindNodeValue(ResNode,"IDS_UrlTransStep_AllowStepLimit", strStepLimit);
			FindNodeValue(ResNode,"IDS_UrlTransStep_AllowStepDel", strStepDelTip);
			FindNodeValue(ResNode,"IDS_UrlTransStep_AllowStepEdit", strStepEditTip); 
		}
		CloseResource(objRes);
	}
	
	char buf_tmp[4096]={0};
	int nSize =4095;

#ifdef WTGET
	GetEnvironmentVariable( "QUERY_STRING", buf_tmp,nSize);
#else
	char *tmpquery = getenv("QUERY_STRING");
	if(tmpquery)
		strcpy(buf_tmp,tmpquery);
#endif
	if(buf_tmp != NULL)
	{
		string buf1 = buf_tmp;
		int pos = buf1.find("=", 0);
		string querystr = buf1.substr(pos+1, buf1.size() - pos - 1);
		strMonitorId = querystr;

		strIniFileName = strMonitorId;
		strIniFileName += "_UrlStepInfo.ini";

		OutputDebugString("Start:");
		OutputDebugString(strIniFileName.c_str());
	}

	ShowMainTable();
}

//
CUrlTransStepInfo::~CUrlTransStepInfo(void)
{
}

//
void CUrlTransStepInfo::ShowHelp()
{
	m_pStepInfoTable->ShowOrHideHelp();
}

//初始化主界面
void CUrlTransStepInfo::ShowMainTable()
{
	m_pMainTable = new WSVMainTable(this, strTitle, true);

	if(m_pMainTable->pHelpImg)
	{
		connect(m_pMainTable->pHelpImg,SIGNAL(clicked()),this,SLOT(ShowHelp()));
	}
	
	m_pStepListTable = new WSVFlexTable(m_pMainTable->GetContentTable()->elementAt(1,0),List,strStepListLabel);

	m_pStepListTable->SetDivId("listpan2");
	if(m_pStepListTable->GetContentTable()!=NULL)
	{		
		m_pStepListTable->AppendColumn("",WLength(40,WLength::Pixel));
		m_pStepListTable->SetDataRowStyle("table_data_grid_item_img");

		m_pStepListTable->AppendColumn(strStepNameLabel, WLength(50,WLength::Percentage));
		m_pStepListTable->SetDataRowStyle("table_data_grid_item_text");
		
		m_pStepListTable->AppendColumn(strStepTypeLabel, WLength(50,WLength::Percentage));
		m_pStepListTable->SetDataRowStyle("table_data_grid_item_text");
		
		//m_pStepListTable->AppendColumn("Post数据",WLength(120,WLength::Pixel));
		//m_pStepListTable->SetDataRowStyle("table_data_grid_item_text");
		//
		//m_pStepListTable->AppendColumn("匹配内容",WLength(120,WLength::Pixel));
		//m_pStepListTable->SetDataRowStyle("table_data_grid_item_text");
		//
		//m_pStepListTable->AppendColumn("错误匹配内容",WLength(10,WLength::Percentage));
		//m_pStepListTable->SetDataRowStyle("table_data_grid_item_text");

		//m_pStepListTable->AppendColumn("用户名称",WLength(10,WLength::Percentage));
		//m_pStepListTable->SetDataRowStyle("table_data_grid_item_text");

		//m_pStepListTable->AppendColumn("用户密码",WLength(10,WLength::Percentage));
		//m_pStepListTable->SetDataRowStyle("table_data_grid_item_text");

		//m_pStepListTable->AppendColumn("编　辑",WLength(60,WLength::Pixel));
		//m_pStepListTable->SetDataRowStyle("table_data_grid_item_img");

		m_pStepListTable->AppendColumn(strEdit, WLength(60,WLength::Pixel));
		m_pStepListTable->SetDataRowStyle("table_data_grid_item_img");
	}
	
	//全选,反选,全不选
	
	m_pStepListTable->AddStandardSelLink(szTipSelAll,szTipNotSelAll,szTipInvSel);

	WTable * pTbl = new WTable(m_pStepListTable->GetActionTable()->elementAt(0,1));

	m_pStepListTable->GetActionTable()->elementAt(0,1)->setContentAlignment(AlignCenter);
	m_pStepListTable->GetActionTable()->elementAt(0,1)->setStyleClass("textbold");
	pTbl->setStyleClass("widthauto");

	//删除报警
	WSVButton * p_Del = new WSVButton(pTbl->elementAt(0,0), strDel, "button_bg_del.png", "", false);
	connect(p_Del , SIGNAL(clicked()),this, SLOT(BeforeDelStep()));

	//WSVButton * p_Test = new WSVButton(pTbl->elementAt(0,1), "监测器测试", "button_bg_del.png", "", false);
	//connect(p_Test , SIGNAL(clicked()),this, SLOT(TestMonitor()));

	//WSVButton * p_Test = new WSVButton(pTbl->elementAt(0,1), "显示网页测试", "button_bg_del.png", "", false);
	//connect(p_Test , SIGNAL(clicked()),this, SLOT(TestMonitor()));

	m_pStepListTable->GetActionTable()->elementAt(0,2)->setContentAlignment(AlignRight);
	WSVButton * p_Add = new WSVButton(m_pStepListTable->GetActionTable()->elementAt(0,2), strAddStep, "button_bg_add_black.png", strAddStep, true);
	connect( p_Add, SIGNAL(clicked()),"showbar();" ,this, SLOT(AddStep()) , WObject::ConnectionType::JAVASCRIPTDYNAMIC);
	//connect( p_Add, SIGNAL(clicked()), this, SLOT(AddStep()));

	m_pStepListTable->SetNullTipInfo("Not Record!");

	connect(m_pStepListTable->pSelAll, SIGNAL(clicked()), this, SLOT(SelAll()));
	connect(m_pStepListTable->pSelReverse, SIGNAL(clicked()), this, SLOT(SelInvert()));
	connect(m_pStepListTable->pSelNone, SIGNAL(clicked()), this, SLOT(SelNone()));

	connect(&m_stepMapper, SIGNAL(mapped(const std::string)), this, SLOT(EditStep(const std::string)));

	//emit ShowHelp();

	m_pStepInfoTable = new WSVFlexTable(m_pMainTable->GetContentTable()->elementAt(1,0), Group ,strStepInfo);
	if (m_pStepInfoTable->GetContentTable() != NULL)
	{
		m_pStepInfoTable->AppendRows(strBaseInfo);

		WTable * tmpTable = new WTable(m_pStepInfoTable->AppendRowsContent(0, strUrlsLabel, strUrlsLabel, ""));
		p_UrlBtn= new WRadioButton("", tmpTable->elementAt(0, 0));
		pUrl = new WLineEdit("", tmpTable->elementAt(0, 1));
		pUrl->setStyleClass("input_text");

		tmpTable = new WTable(m_pStepInfoTable->AppendRowsContent(0, strLinksLabel, strLinksLabel, ""));
		p_LinksBtn = new WRadioButton("", tmpTable->elementAt(0, 0));
		pLinks = new WSelectionBox(tmpTable->elementAt(0, 1));
		pLinks->setStyleClass("input_text");
		pLinks->setVerticalSize(10);

		tmpTable = new WTable(m_pStepInfoTable->AppendRowsContent(0, strFormsLabel, strFormsLabel, ""));
		p_FormsBtn = new WRadioButton("", tmpTable->elementAt(0, 0));
		pForms = new WComboBox(tmpTable->elementAt(0, 1));
		pForms->setStyleClass("input_text");
		connect(pForms, SIGNAL(changed()), this, SLOT(SelFormsChanged()));
		connect(pForms, SIGNAL(clicked()), this, SLOT(SelFormsChanged()));
		//pForms->setVerticalSize(5);

		tmpTable = new WTable(m_pStepInfoTable->AppendRowsContent(0, strFramesLabel, strFramesLabel, ""));
		p_FramesBtn = new WRadioButton("", tmpTable->elementAt(0, 0));
		pFrames = new WComboBox(tmpTable->elementAt(0, 1));
		pFrames->setStyleClass("input_text");
		//pFrames->setVerticalSize(5);

		tmpTable = new WTable(m_pStepInfoTable->AppendRowsContent(0, strRefreshsLabel, strRefreshsLabel, ""));
		p_RefreshsBtn = new WRadioButton("", tmpTable->elementAt(0, 0));
		pRefreshs = new WComboBox(tmpTable->elementAt(0, 1));
		pRefreshs->setStyleClass("input_text");
		//pRefreshs->setVerticalSize(5);

		WButtonGroup * 	group = new WButtonGroup();
		
		group->addButton(p_UrlBtn);
		group->addButton(p_LinksBtn);
		group->addButton(p_FormsBtn);
		group->addButton(p_FramesBtn);
		group->addButton(p_RefreshsBtn);

		tmpTable = new WTable(m_pStepInfoTable->AppendRowsContent(0, strPostDatasLabel, strPostDatasLabel, ""));
		//WRadioButton * tmpRadioBtn = new WRadioButton(tmpTable->elementAt(0, 0));
		new WText("&nbsp;&nbsp;&nbsp;&nbsp;", tmpTable->elementAt(0, 0));
		pPostData = new WTextArea("", tmpTable->elementAt(0, 1));
		//pPostData = new WLineEdit("", tmpTable->elementAt(0, 1));
		pPostData->setStyleClass("input_text");
		//tmpRadioBtn->hide();

		m_pStepInfoTable->AppendRows(strAdvanceInfo);

		pContent = new WLineEdit("", m_pStepInfoTable->AppendRowsContent(1, strConentLabel, strConentLabel, ""));
		pContent->setStyleClass("input_text");
		pErrorContent = new WLineEdit("", m_pStepInfoTable->AppendRowsContent(1, strErrorConentLabel, strErrorConentLabel, ""));
		pErrorContent->setStyleClass("input_text");
		pUserName = new WLineEdit("", m_pStepInfoTable->AppendRowsContent(1, strUserNameLabel, strUserNameLabel, ""));
		pUserName->setStyleClass("input_text");
		pUserPwd = new WLineEdit("", m_pStepInfoTable->AppendRowsContent(1, strUserPwdLabel, strUserPwdLabel, ""));
		pUserPwd->setStyleClass("input_text");

		m_pStepInfoTable->ShowOrHideHelp();
		m_pStepInfoTable->HideAllErrorMsg();		
	}

	if(m_pStepInfoTable->GetActionTable()!=NULL)
	{
		m_pStepInfoTable->GetActionTable()->elementAt(0,1)->setContentAlignment(AlignCenter);
		
		WSVButton *pOk = new WSVButton(m_pStepInfoTable->GetActionTable()->elementAt(0,1), strSave, "button_bg_m_black.png","",true);
	
		connect(pOk, SIGNAL(clicked()), "showbar();" ,this, SLOT(SaveStep()) , WObject::ConnectionType::JAVASCRIPTDYNAMIC);
		
		WSVButton *pShowHtml = new WSVButton(m_pStepInfoTable->GetActionTable()->elementAt(0,2), strShowHtml, "button_bg_m_black.png","",true);
	
		connect(pShowHtml, SIGNAL(clicked()), "showbar();" ,this, SLOT(ShowHtml()) , WObject::ConnectionType::JAVASCRIPTDYNAMIC);
		
		WSVButton *pCancel = new WSVButton(m_pStepInfoTable->GetActionTable()->elementAt(0,3), strCancel, "button_bg_m_black.png","",true);
	
		connect(pCancel, SIGNAL(clicked()), "showbar();" ,this, SLOT(CancelStep()) , WObject::ConnectionType::JAVASCRIPTDYNAMIC);
	}
	
	//隐藏按钮
	pHideButton = new WPushButton("hide button",this);
	if(pHideButton)
	{
		pHideButton->setToolTip("Hide Button");
		connect(pHideButton,SIGNAL(clicked()),this,SLOT(DelStep()));
		pHideButton->hide();
	}

	std::list<string> keylist;
	std::list<string>::iterator keyitem;
	string strIndex, strStepName, strStepType, 	strPostData, strContent, strErrorContent, strUserName, strUserPwd, strLimitStep;

	//从ini获取步列表
	if(GetIniFileSections(keylist, strIniFileName))
	{		
		//从ini初始化步列表
		for(keyitem = keylist.begin(); keyitem != keylist.end(); keyitem ++)	
		{	
			strIndex = GetIniFileString((*keyitem), "nIndex", "", strIniFileName);
			if(strIndex == "1")
			{
				strProxyUser = GetIniFileString((*keyitem), "ProxyUser", "", strIniFileName);
				strProxyPass = GetIniFileString((*keyitem), "ProxyPass", "", strIniFileName);				
				strProxyServerPort = GetIniFileString((*keyitem), "ProxyServerPort", "", strIniFileName);
				strTimeOut = GetIniFileString((*keyitem), "TimeOut", "", strIniFileName);
				strLimitStep = GetIniFileString((*keyitem), "limitStep", "", strIniFileName);
				nLimitStep = atoi(strLimitStep.c_str());
			}
			//else
			{
				strStepName = GetIniFileString((*keyitem), "StepName", "", strIniFileName);
				strStepType = GetIniFileString((*keyitem), "StepType", "", strIniFileName);
				strPostData = GetIniFileString((*keyitem), "StepPostData", "", strIniFileName);
				strContent = GetIniFileString((*keyitem), "StepContent", "", strIniFileName);
				strErrorContent = GetIniFileString((*keyitem), "StepErrorContent", "", strIniFileName);
				strUserName = GetIniFileString((*keyitem), "StepUserName", "", strIniFileName);
				strUserPwd = GetIniFileString((*keyitem), "StepUserPwd", "", strIniFileName);

				AddListItem(strIndex, strStepName, strStepType, strPostData, strContent, strErrorContent, strUserName, strUserPwd, 0);
			}
		}
	}

	m_pStepInfoTable->hide();	
}

//选择不同的form选项
void CUrlTransStepInfo::SelFormsChanged()
{
	pPostData->setText("");
	int nSel = pForms->currentIndex();
	int nIndex = 0;
	for(listitem = lstPostDatas.begin(); listitem != lstPostDatas.end(); listitem++)
	{	
		if(nIndex == nSel)
		{
			pPostData->setText((*listitem).c_str());
			break;
		}
		nIndex++;
	}
}

//
void  CUrlTransStepInfo::AddListItem(string strIndex, string strStepName, string strStepType, 
	string strPostData, string strContent, string strErrorContent, string strUserName, string strUserPwd, int numRow)
{
	numRow = m_pStepListTable->GeDataTable()->numRows();

	Step_LIST list;

	m_pStepListTable->InitRow(numRow);
	
	m_pStepListTable->GeDataTable()->elementAt(numRow , 0)->setContentAlignment(AlignCenter);
	WCheckBox * pSelect = new WCheckBox("", (WContainerWidget*)m_pStepListTable->GeDataTable()->elementAt(numRow, 0));
	
	m_pStepListTable->GeDataTable()->elementAt(numRow , 2)->setContentAlignment(AlignCenter);	
	WText *  pStepNameText = new WText(strStepName, (WContainerWidget*)m_pStepListTable->GeDataTable()->elementAt(numRow , 2));
	
	m_pStepListTable->GeDataTable()->elementAt(numRow , 4)->setContentAlignment(AlignCenter);	
	WText * pStepTypeText = new WText(strStepType, (WContainerWidget*)m_pStepListTable->GeDataTable()->elementAt(numRow , 4));
	
	//WText * pTmpText;
	//m_pStepListTable->GeDataTable()->elementAt(numRow , 6)->setContentAlignment(AlignCenter);	
	//pTmpText = new WText(strPostData, (WContainerWidget*)m_pStepListTable->GeDataTable()->elementAt(numRow , 6));

	//m_pStepListTable->GeDataTable()->elementAt(numRow , 8)->setContentAlignment(AlignCenter);	
	//pTmpText = new WText(strContent, (WContainerWidget*)m_pStepListTable->GeDataTable()->elementAt(numRow , 8));

	//m_pStepListTable->GeDataTable()->elementAt(numRow , 10)->setContentAlignment(AlignCenter);	
	//pTmpText = new WText(strErrorContent, (WContainerWidget*)m_pStepListTable->GeDataTable()->elementAt(numRow , 10));

	//m_pStepListTable->GeDataTable()->elementAt(numRow , 12)->setContentAlignment(AlignCenter);	
	//pTmpText = new WText(strUserName, (WContainerWidget*)m_pStepListTable->GeDataTable()->elementAt(numRow , 12));

	//m_pStepListTable->GeDataTable()->elementAt(numRow , 14)->setContentAlignment(AlignCenter);	
	//pTmpText = new WText(strUserPwd, (WContainerWidget*)m_pStepListTable->GeDataTable()->elementAt(numRow , 14));
	//
	//WImage *pEdit = new WImage("/images/edit.gif", (WContainerWidget*)m_pStepListTable->GeDataTable()->elementAt(numRow , 16));
	//m_pStepListTable->GeDataTable()->elementAt(numRow , 16)->setContentAlignment(AlignCenter);

	WImage *pEdit = new WImage("/images/edit.gif", (WContainerWidget*)m_pStepListTable->GeDataTable()->elementAt(numRow , 6));
	m_pStepListTable->GeDataTable()->elementAt(numRow , 6)->setContentAlignment(AlignCenter);

	pEdit->decorationStyle().setCursor(WCssDecorationStyle::Pointer); 
	m_stepMapper.setMapping(pEdit, strIndex); 
	connect(pEdit, SIGNAL(clicked()),"showbar();",  &m_stepMapper, SLOT(map()) , WObject::ConnectionType::JAVASCRIPTDYNAMIC);

	list.pSelect = pSelect;
	list.pStepNameText = pStepNameText;
	list.pStepTypeText = pStepTypeText;

	list.id = strIndex;
	list.nIndex = numRow + 1;
	
	list.strPostData = strPostData;
	list.strContent = strContent;
	list.strErrorContent = strErrorContent;
	list.strUserName = strUserName;
	list.strUserPwd = strUserPwd;

	m_pListStep.push_back(list);
}

//
void  CUrlTransStepInfo::AddStep()
{
	//步数限制， 最多只能添多少步 根据监测器类型来判断......

	strCurAction = "add";

	//获取当前步
	int numRows = m_pStepListTable->GeDataTable()->numRows();
	//numRows++;
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", numRows);
	string strCurStep = chItem;
	
	if(numRows >= nLimitStep)
	{
		WebSession::js_af_up = "hiddenbar();alert('";
		WebSession::js_af_up += strStepLimit;
		WebSession::js_af_up += "')";
		return;
	}

	//添加第二步时需要做一次预分析, 因为第一步在树视图里保存时没有做过解析
	if(strCurStep == "1")
		PaserStep1();

	//从ini获取_TimeOut  _ProxyServerPort _ProxyUser _ProxyPass 
	string strIn = "_ProxyServerPort=";
	strIn += strProxyServerPort;
	strIn += "`_ProxyUser=";
	strIn += strProxyUser;
	strIn += "`_ProxyPass=";
	strIn += strProxyPass;
	strIn += "`_TimeOut=";
	strIn += strTimeOut;
	strIn += "`_MonitorID=";
	strIn += strMonitorId;
	strIn += "`_tranStep=";
	strIn += strCurStep;
	strIn += "`";
	//sample:_ProxyServerPort=`_ProxyUser=`_ProxyPass=`_TimeOut=180`_GroupID=1`_MonitorID=1`_tranStep=1
	
	//转格式 ．．．
	char inBuf[2000];
	sprintf(inBuf, "%s", strIn.c_str());
	char *p;

	p=inBuf;
	int length(0);
	while(*p!='\0')
	{
		if(*p=='\n') break;
		if(*p=='`') *p='\0';
		p++;
		length++;	
	}

	string strBaseUrl = "";

	//清空界面
	pLinks->clear();
	pForms->clear();
	pPostData->setText("");
	
	//if(numRows == 0)
	//{
	//	pUrl->setText(GetIniFileString("0", "StepName", "", strIniFileName));
	//	strBaseUrl = GetIniFileString("0", "StepName", "", strIniFileName);
	//	//GetIniFileString("0", "StepType", "", strIniFileName);
	//	//GetIniFileString("0", "StepPostData", "", strIniFileName);
	//	pContent->setText(GetIniFileString("0", "StepContent", "", strIniFileName));
	//	pErrorContent->setText(GetIniFileString("0", "StepErrorContent", "", strIniFileName));
	//	pUserName->setText(GetIniFileString("0", "StepUserName", "", strIniFileName));
	//	pUserPwd->setText(GetIniFileString("0", "StepUserPwd", "", strIniFileName));
	//}
	//else
	{
		pUrl->setText("");
		pContent->setText("");
		pErrorContent->setText("");
		pUserName->setText("");
		pUserPwd->setText("");	
	}
	
	//for(int i = 0; i < pLinks->count(); i++)
	//	pLinks->removeItem(i);

	//调用UrlTrans.dll的UrlPaserStepList 输入nTranStep等 获取　lstLinks等..
	lstLinks.clear();
	lstForms.clear();
	lstPostDatas.clear();
	
	typedef BOOL (*PURLPASERSTEPLIST)(char* , list<string>&, list<string>&, list<string>&, string&);
	HINSTANCE hInstance =::LoadLibrary("UrlTrans.dll");
	if(hInstance)
	{
		PURLPASERSTEPLIST pfunc=(PURLPASERSTEPLIST)::GetProcAddress(hInstance, "UrlPaserStepList");
		if(pfunc)
		{
			pfunc(inBuf ,lstLinks, lstForms, lstPostDatas, strBaseUrl);
			//pfunc(inBuf ,lstLinks);
		}
	}
	::FreeLibrary(hInstance);

	pUrl->setText(strBaseUrl);

	int nCount = 0;

	//pLinks->addItem("NULL");	
	for(listitem = lstLinks.begin(); listitem != lstLinks.end(); listitem++)
	{	
		if(nCount < 400)
			pLinks->addItem((*listitem).c_str());
		else
			break;
		nCount ++;
	}
	pLinks->setCurrentIndex(0);

	//pForms->addItem("NULL");
	for(listitem = lstForms.begin(); listitem != lstForms.end(); listitem++)
	{	
		pForms->addItem((*listitem).c_str());
	}
	pForms->setCurrentIndex(0);
	
	//pFrames;

	//test...
	//for(int i = 0; i < 2000; i++)
	//	pLinks->addItem("dddddddddd");

	//加一个显示html按钮, 用js的showModalDialog方法显示指定路径的html, 这里构造此步的html
	//string strCurStepHtml = "";
	strCurStepHtml = "";
	strCurStepHtml += "../";
    //strCurStepHtml += "/fcgi-bin/";
	strCurStepHtml += strMonitorId;
	strCurStepHtml += "_CUrlStep";
	strCurStepHtml += strCurStep;
	strCurStepHtml += ".html";

	//显示StepInfo界面　hiddenbar
	WebSession::js_af_up = "hiddenbar();";	
	//m_pStepInfoTable->refresh();
	
	m_pStepInfoTable->show();
	m_pStepListTable->hide();
}

void  CUrlTransStepInfo::ShowHtml()
{
	////显示网页测试	
	//string strCurStepHtml = "../";
 //   //strCurStepHtml += "/cgi-bin/";
	//strCurStepHtml += strMonitorId;
	//strCurStepHtml += "_CUrlStep";
	//strCurStepHtml += "2";
	//strCurStepHtml += ".html";

	//显示StepInfo界面　hiddenbar
	WebSession::js_af_up += "hiddenbar();showurlstephtm('" ;
	WebSession::js_af_up += strCurStepHtml;
	WebSession::js_af_up +=  "');";	
}

//
void  CUrlTransStepInfo::CancelStep()
{
	WebSession::js_af_up = "hiddenbar();";
	//隐藏StepInfo界面　hiddenbar
	m_pStepInfoTable->hide();
	m_pStepListTable->show();
}

//第一步分析一下（监测器保存时没有分析， 故这里主动分析一下）
void CUrlTransStepInfo::PaserStep1()
{
	//从UrlTransStepInfo界面获取_referenceType等
	string strReferenceType = "", strReference = "", strPostData = "",
		strContent = "", strErrorContent = "", strUserName = "", strUserPwd = "", strCurStep = "1";//postData．．．

	strReference = GetIniFileString("1", "StepName", "", strIniFileName);
	strReferenceType = GetIniFileString("1", "StepType", "", strIniFileName);
	strPostData = GetIniFileString("1", "StepPostData", "", strIniFileName);
	strContent = GetIniFileString("1", "StepContent", "", strIniFileName);
	strErrorContent = GetIniFileString("1", "StepErrorContent", "", strIniFileName);
	strUserName = GetIniFileString("1", "StepUserName", "", strIniFileName);
	strUserPwd = GetIniFileString("1", "StepUserPwd", "", strIniFileName);
	
	//从ini获取_TimeOut  _ProxyServerPort _ProxyUser _ProxyPass 
	string strIn = "_ProxyServerPort=";
	strIn += strProxyServerPort;
	strIn += "`_ProxyUser=";
	strIn += strProxyUser;
	strIn += "`_ProxyPass=";
	strIn += strProxyPass;
	strIn += "`_TimeOut=";
	strIn += strTimeOut;
	strIn += "`_MonitorID=";
	strIn += strMonitorId;
	strIn += "`_tranStep=";
	strIn += strCurStep;
	strIn += "`";
	//sample:_ProxyServerPort=`_ProxyUser=`_ProxyPass=`_TimeOut=180`_GroupID=1`_MonitorID=1`_tranStep=1

	//构成_referenceType+nStep(url Links Forms...) _reference+nStep _postData+nStep _content+nStep _errorContent+nStep _username+nStep _password+nStep链表等
	strIn += MakeInputStrByStep(strCurStep, strReference, strReferenceType, strPostData, strContent, strErrorContent, strUserName, strUserPwd);

	//sample:`_referenceType1=url`_reference1=http://www.sina.com.cn`_postData1=`_content1=`_errorContent1=`_username1=`_password1=`
	
	//转格式 ．．．
	char inBuf[2000];
	sprintf(inBuf, "%s", strIn.c_str());
	char *p;

	p=inBuf;
	int length(0);
	while(*p!='\0')
	{
		if(*p=='\n') break;
		if(*p=='`') *p='\0';
		p++;
		length++;	
	}

	typedef BOOL (*PURLTRANSACTIONBYSTEP)(char* ,char * ,int&);	
	HINSTANCE hInstance =::LoadLibrary("UrlTrans.dll");
	if(hInstance)
	{
		PURLTRANSACTIONBYSTEP pfunc=(PURLTRANSACTIONBYSTEP)::GetProcAddress(hInstance, "UrlTransactionByStep");
		if(pfunc)
		{
			char outBuf[1000];
			int nOutBuf = 1000;
			
			pfunc(inBuf, outBuf, nOutBuf);
		}
	}
	::FreeLibrary(hInstance);
}

//
void  CUrlTransStepInfo::SaveStep()
{
	//从UrlTransStepInfo界面获取_referenceType等
	string strReferenceType = "", strReference = "", strPostData = "",
		strContent = "", strErrorContent = "", strUserName = "", strUserPwd = "";//postData．．．
	
	if(p_UrlBtn->isChecked())
	{
		strReferenceType = "url";
		strReference = pUrl->text();
	}
	else if(p_LinksBtn->isChecked())
	{
		strReferenceType = "link";
		strReference = pLinks->currentText();
	}	
	else if(p_FormsBtn->isChecked())
	{
		strReferenceType = "form";
		strReference = pForms->currentText();
		if(!pPostData->text().empty())
		{
			strPostData = pPostData->text();
		}	
	}
	else if(p_FramesBtn->isChecked())
	{
		strReferenceType = "frame";
		strReference = pFrames->currentText();	
	}	
	else if(p_RefreshsBtn->isChecked())
	{
		strReferenceType = "refresh";
		strReference = pRefreshs->currentText();
	}
	else
	{
		return;
	}

	strContent = pContent->text();
	strErrorContent = pErrorContent->text();
	strUserName = pUserName->text();
	strUserPwd = pUserPwd->text();

	string strCurStep = "";
	//获取当前步(增加, 编辑)
	if(strCurAction == "add")
	{
		int numRows = m_pStepListTable->GeDataTable()->numRows();
		numRows++;
		char chItem[32]  = {0};	
		sprintf(chItem, "%d", numRows);
		strCurStep = chItem;
	}
	else
	{
		//nCurEditIndex++;
		//char chItem[32]  = {0};	
		//sprintf(chItem, "%d", nCurEditIndex);
		//strCurStep = chItem;

		strCurStep = strCurEditIndex;
	}

	//从ini获取_TimeOut  _ProxyServerPort _ProxyUser _ProxyPass 
	string strIn = "_ProxyServerPort=";
	strIn += strProxyServerPort;
	strIn += "`_ProxyUser=";
	strIn += strProxyUser;
	strIn += "`_ProxyPass=";
	strIn += strProxyPass;
	strIn += "`_TimeOut=";
	strIn += strTimeOut;
	strIn += "`_MonitorID=";
	strIn += strMonitorId;
	strIn += "`_tranStep=";
	strIn += strCurStep;
	strIn += "`";
	//sample:_ProxyServerPort=`_ProxyUser=`_ProxyPass=`_TimeOut=180`_GroupID=1`_MonitorID=1`_tranStep=1

	//构成_referenceType+nStep(url Links Forms...) _reference+nStep _postData+nStep _content+nStep _errorContent+nStep _username+nStep _password+nStep链表等
	strIn += MakeInputStrByStep(strCurStep, strReference, strReferenceType, strPostData, strContent, strErrorContent, strUserName, strUserPwd);

	//sample:`_referenceType1=url`_reference1=http://www.sina.com.cn`_postData1=`_content1=`_errorContent1=`_username1=`_password1=`
	
	//转格式 ．．．
	char inBuf[2000];
	sprintf(inBuf, "%s", strIn.c_str());
	char *p;

	p=inBuf;
	int length(0);
	while(*p!='\0')
	{
		if(*p=='\n') break;
		if(*p=='`') *p='\0';
		p++;
		length++;	
	}

	typedef BOOL (*PURLTRANSACTIONBYSTEP)(char* ,char * ,int&);	
	HINSTANCE hInstance =::LoadLibrary("UrlTrans.dll");
	if(hInstance)
	{
		PURLTRANSACTIONBYSTEP pfunc=(PURLTRANSACTIONBYSTEP)::GetProcAddress(hInstance, "UrlTransactionByStep");
		if(pfunc)
		{
			char outBuf[1000];
			int nOutBuf = 1000;
			
			pfunc(inBuf, outBuf, nOutBuf);
		}
	}
	::FreeLibrary(hInstance);

	if(strCurAction == "add")
	{
		AddListItem(strCurStep, strReference, strReferenceType, strPostData, strContent, strErrorContent, strUserName, strUserPwd, 0);
	}
	else
	{
		//clear + init吗? 其他呢?...

		//list项目是否可以变简洁些?
		for(m_pListItem = m_pListStep.begin(); m_pListItem != m_pListStep.end(); m_pListItem ++)
		{
			if (m_pListItem->id == strCurStep)
			{
				m_pListItem->pStepNameText->setText(strReference);
				m_pListItem->pStepTypeText->setText(strReferenceType);
				break;
			}
		}
	}

	//保存当前步信息到Ini。。。
	WriteIniFileString(strCurStep, "nIndex", strCurStep, strIniFileName);
	WriteIniFileString(strCurStep, "StepName", strReference, strIniFileName);
	WriteIniFileString(strCurStep, "StepType", strReferenceType, strIniFileName);
	WriteIniFileString(strCurStep, "StepPostData", strPostData, strIniFileName);
	WriteIniFileString(strCurStep, "StepContent", strContent, strIniFileName);
	WriteIniFileString(strCurStep, "StepErrorContent", strErrorContent, strIniFileName);
	WriteIniFileString(strCurStep, "StepUserName", strUserName, strIniFileName);
	WriteIniFileString(strCurStep, "StepUserPwd", strUserPwd, strIniFileName);
	
	WebSession::js_af_up = "hiddenbar();";
	
	//隐藏StepInfo界面　hiddenbar
	m_pStepInfoTable->hide();
	m_pStepListTable->show();
}

//
void  CUrlTransStepInfo::TestMonitor()
{	
	//获取当前步
	int numRows = m_pStepListTable->GeDataTable()->numRows();
	numRows++;
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", numRows);
	string strTotalStep = chItem;

	//从ini获取_TimeOut  _ProxyServerPort _ProxyUser _ProxyPass 
	string strIn = "_ProxyServerPort=";
	strIn += strProxyServerPort;
	strIn += "`_ProxyUser=";
	strIn += strProxyUser;
	strIn += "`_ProxyPass=";
	strIn += strProxyPass;
	strIn += "`_TimeOut=";
	strIn += strTimeOut;
	strIn += "`_MonitorID=";
	strIn += strMonitorId;
	//strIn += "`_tranStep=";
	//strIn += strTotalStep;
	strIn += "`";

	////
	//std::list<string> keylist;
	//std::list<string>::iterator keyitem;
	//string strIndex, strStepName, strStepType, 	strPostData, strContent, strErrorContent, strUserName, strUserPwd;

	////从ini获取步列表
	//if(GetIniFileSections(keylist, strIniFileName))
	//{		
	//	//从ini初始化步列表
	//	for(keyitem = keylist.begin(); keyitem != keylist.end(); keyitem ++)	
	//	{	
	//		strIndex = GetIniFileString((*keyitem), "nIndex", "", strIniFileName);
	//		if(strIndex == "0")
	//			continue;
	//		else
	//		{
	//			strStepName = GetIniFileString((*keyitem), "StepName", "", strIniFileName);
	//			strStepType = GetIniFileString((*keyitem), "StepType", "", strIniFileName);

	//			strIn += MakeInputStrByStep(strIndex, strStepName, strStepType, "", "", "", "", "");
	//		}
	//	}
	//}

	//转格式 ．．．
	char inBuf[2000];
	char outBuf[1000];
	int nOutBuf = 1000;

	sprintf(inBuf, "%s", strIn.c_str());
	char *p;

	p=inBuf;
	int length(0);
	while(*p!='\0')
	{
		if(*p=='\n') break;
		if(*p=='`') *p='\0';
		p++;
		length++;	
	}

	typedef BOOL (*PURLTRANSACTIONBYSTEP)(char* ,char * ,int&);	
	HINSTANCE hInstance =::LoadLibrary("UrlTrans.dll");
	if(hInstance)
	{
		PURLTRANSACTIONBYSTEP pfunc=(PURLTRANSACTIONBYSTEP)::GetProcAddress(hInstance, "UrlTransaction");
		if(pfunc)
		{
			pfunc(inBuf , outBuf,  nOutBuf);
		}
	}
	::FreeLibrary(hInstance);
	
	//结果输出
	string strRetrun(outBuf);
	string strRetrurnJs  = "alert('" + strRetrun + "');"; 
	WebSession::js_af_up = strRetrurnJs;
}

//
string CUrlTransStepInfo::MakeInputStrByStep(string strIndex, string strStepName, string strStepType, 
	string strPostData, string strContent, string strErrorContent, string strUserName, string strUserPwd)
{
	string strIn = "";

	//构成_referenceType+nStep(url Links Forms...) _reference+nStep _postData+nStep _content+nStep _errorContent+nStep _username+nStep _password+nStep链表等
	strIn += "_referenceType";
	strIn += strIndex;
	strIn += "=";
	strIn += strStepType;
	strIn += "`";

	strIn += "_reference";
	strIn += strIndex;
	strIn += "=";
	strIn += strStepName;
	strIn += "`";

	strIn += "_postData";
	strIn += strIndex;
	strIn += "=";
	strIn += strPostData;
	strIn += "`";

	strIn += "_content";
	strIn += strIndex;
	strIn += "=";
	strIn += strContent;
	strIn += "`";

	strIn += "_errorContent";
	strIn += strIndex;
	strIn += "=";
	strIn += strErrorContent;
	strIn += "`";

	strIn += "_username";
	strIn += strIndex;
	strIn += "=";
	strIn += strUserName;
	strIn += "`";

	strIn += "_password";
	strIn += strIndex;
	strIn += "=";
	strIn += strUserPwd;
	strIn += "`";	

	return strIn;
}

//
void  CUrlTransStepInfo::BeforeDelStep()
{
	string strUserID = GetWebUserID();
	for(m_pListItem = m_pListStep.begin(); m_pListItem != m_pListStep.end(); m_pListItem ++)
	{
		if (m_pListItem->pSelect->isChecked())
		{
			if(pHideButton)
			{
				string strDelDes = pHideButton->getEncodeCmd("xclicked()") ;
				if(!strDelDes.empty())
				{
					//只允许删除最后一步...
					if(m_pListItem->nIndex == m_pStepListTable->GeDataTable()->numRows())
					{
						strDelDes  = "_Delclick('" + strDelStepAffirm + "','" + szButNum + "','" + szButMatch + "','" + strDelDes + "');"; 
						WebSession::js_af_up = strDelDes;
						return;
					}
					else
					{
						WebSession::js_af_up = "alert('";
						WebSession::js_af_up += strStepDelTip;
						WebSession::js_af_up += "')";
						return;
					}
				}					
			}
			break;
		}
	}
}

//
void CUrlTransStepInfo::DelStep()
{
	string strUserID = GetWebUserID();

	string strDeleteAlert;
	for(m_pListItem = m_pListStep.begin(); m_pListItem != m_pListStep.end(); m_pListItem ++)
	{
		if (m_pListItem->pSelect->isChecked())
		{
			std::string temp = m_pListItem->id;

			DeleteIniFileSection(temp, strIniFileName);

			int nRow = ((WTableCell*)(m_pListItem->pSelect->parent()))->row();

			list<Step_LIST>::iterator pItem = m_pListItem;                     

			m_pListItem --;

			string strTemp = pItem->pStepNameText->text();
			strDeleteAlert += strTemp;
			strDeleteAlert += "  ";

			m_pListStep.erase(pItem);          

			m_pStepListTable->GeDataTable()->deleteRow(nRow); 
		}
	}

	if (m_pListStep.size() == 0)
	{
		m_pStepListTable->ShowNullTip();
	}
	else
	{
		m_pStepListTable->HideNullTip();
	}
}

//
void CUrlTransStepInfo::EditStep(const std::string strIndex)
{
	//
	strCurAction = "edit";
	int nCurEditIndex = atoi(strIndex.c_str());

	//只允许编辑最后一步的限制需要吗？
	if(nCurEditIndex != m_pStepListTable->GeDataTable()->numRows())
	{
		WebSession::js_af_up = "hiddenbar();alert('";
		WebSession::js_af_up += strStepEditTip;
		WebSession::js_af_up += "')";
		return;
	}

	//第一步编辑 默认选中url, 不需要获取links等
	
	//根据strIndex 选中的radio及links值、content值的重新赋予等。
	string strCurStep, strStepName, strStepType, 	strPostData, strContent, strErrorContent, strUserName, strUserPwd;

	strStepName = GetIniFileString(strIndex, "StepName", "", strIniFileName);
	strStepType = GetIniFileString(strIndex, "StepType", "", strIniFileName);
	strPostData = GetIniFileString(strIndex, "StepPostData", "", strIniFileName);
	strContent = GetIniFileString(strIndex, "StepContent", "", strIniFileName);
	strErrorContent = GetIniFileString(strIndex, "StepErrorContent", "", strIniFileName);
	strUserName = GetIniFileString(strIndex, "StepUserName", "", strIniFileName);
	strUserPwd = GetIniFileString(strIndex, "StepUserPwd", "", strIniFileName);	


	//取的应该是上一步给本步的list等....
	nCurEditIndex--;
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nCurEditIndex);
	strCurStep = chItem;
	strInitEditIndex = strCurStep;
	strCurEditIndex = strIndex;
	if(nCurEditIndex > 0)
	{
		//从ini获取_TimeOut  _ProxyServerPort _ProxyUser _ProxyPass 
		string strIn = "_ProxyServerPort=";
		strIn += strProxyServerPort;
		strIn += "`_ProxyUser=";
		strIn += strProxyUser;
		strIn += "`_ProxyPass=";
		strIn += strProxyPass;
		strIn += "`_TimeOut=";
		strIn += strTimeOut;
		strIn += "`_MonitorID=";
		strIn += strMonitorId;
		strIn += "`_tranStep=";
		strIn += strCurStep;
		strIn += "`";
		//sample:_ProxyServerPort=`_ProxyUser=`_ProxyPass=`_TimeOut=180`_GroupID=1`_MonitorID=1`_tranStep=1
		
		//转格式 ．．．
		char inBuf[2000];
		sprintf(inBuf, "%s", strIn.c_str());
		char *p;

		p=inBuf;
		int length(0);
		while(*p!='\0')
		{
			if(*p=='\n') break;
			if(*p=='`') *p='\0';
			p++;
			length++;	
		}

		string strBaseUrl = "";

		pUrl->setText(strStepName);
		pContent->setText(strContent);
		pErrorContent->setText(strErrorContent);
		pUserName->setText(strUserName);
		pUserPwd->setText(strUserPwd);	

		//清空界面
		pLinks->clear();
		pForms->clear();
		pPostData->setText(strPostData);
		
		//调用UrlTrans.dll的UrlPaserStepList 输入nTranStep等 获取　lstLinks等..
		lstLinks.clear();
		lstForms.clear();
		lstPostDatas.clear();
		
		typedef BOOL (*PURLPASERSTEPLIST)(char* , list<string>&, list<string>&, list<string>&, string&);
		HINSTANCE hInstance =::LoadLibrary("UrlTrans.dll");
		if(hInstance)
		{
			PURLPASERSTEPLIST pfunc=(PURLPASERSTEPLIST)::GetProcAddress(hInstance, "UrlPaserStepList");
			if(pfunc)
			{
				pfunc(inBuf ,lstLinks, lstForms, lstPostDatas, strBaseUrl);
			}
		}
		::FreeLibrary(hInstance);

		pUrl->setText(strBaseUrl);
		
		if(strStepType == "url")
		{
			p_UrlBtn->setChecked();
		}
		else if(strStepType == "link")
		{
			p_LinksBtn->setChecked();
		}
		else if(strStepType == "form")
		{
			p_FormsBtn->setChecked();
		}
		else if(strStepType == "frame")
		{
			p_FramesBtn->setChecked();
		}
		else if(strStepType == "refresh")
		{
			p_RefreshsBtn->setChecked();
		}
		else
		{
			
		}

		int nCount = 0;

		for(listitem = lstLinks.begin(); listitem != lstLinks.end(); listitem++)
		{	
			if(nCount < 400)
				pLinks->addItem((*listitem).c_str());
			else
				break;
			nCount ++;
		}
		pLinks->setCurrentIndexByStr(strStepName);//....

		for(listitem = lstForms.begin(); listitem != lstForms.end(); listitem++)
		{	
			pForms->addItem((*listitem).c_str());
		}

		//pForms->setCurrentIndexByStr(0);
	}
	else
	{
		//
		p_UrlBtn->setChecked();
		pUrl->setText(strStepName);
	}
	
	WebSession::js_af_up = "hiddenbar();";
	
	strCurStepHtml = "";
	strCurStepHtml += "../";
    //strCurStepHtml += "/fcgi-bin/";
	strCurStepHtml += strMonitorId;
	strCurStepHtml += "_CUrlStep";
	strCurStepHtml += strCurStep;
	strCurStepHtml += ".html";
	
	//界面隐现
	m_pStepListTable->hide();
	m_pStepInfoTable->show();
}

//
void CUrlTransStepInfo::refresh()
{
	char buf_tmp[4096]={0};
	int nSize =4095;

#ifdef WTGET
	GetEnvironmentVariable( "QUERY_STRING", buf_tmp,nSize);
#else
	char *tmpquery = getenv("QUERY_STRING");
	if(tmpquery)
		strcpy(buf_tmp,tmpquery);
#endif
	if(buf_tmp != NULL)
	{
		string buf1 = buf_tmp;
		int pos = buf1.find("=", 0);
		string querystr = buf1.substr(pos+1, buf1.size() - pos - 1);		

		strMonitorId = querystr;

		strIniFileName = strMonitorId;
		strIniFileName += "_UrlStepInfo.ini";

		OutputDebugString("Refresh:");
		OutputDebugString(strIniFileName.c_str());

	}

	//内存会泄露吗？...
	m_pStepListTable->GeDataTable()->clear();
	m_pListStep.clear();

	std::list<string> keylist;
	std::list<string>::iterator keyitem;
	string strIndex, strStepName, strStepType, 	strPostData, strContent, strErrorContent, strUserName, strUserPwd, strLimitStep;

	//从ini获取步列表
	if(GetIniFileSections(keylist, strIniFileName))
	{		
		//从ini初始化步列表
		for(keyitem = keylist.begin(); keyitem != keylist.end(); keyitem ++)	
		{	
			strIndex = GetIniFileString((*keyitem), "nIndex", "", strIniFileName);
			if(strIndex == "1")
			{
				strProxyUser = GetIniFileString((*keyitem), "ProxyUser", "", strIniFileName);
				strProxyPass = GetIniFileString((*keyitem), "ProxyPass", "", strIniFileName);				
				strProxyServerPort = GetIniFileString((*keyitem), "ProxyServerPort", "", strIniFileName);
				strTimeOut = GetIniFileString((*keyitem), "TimeOut", "", strIniFileName);
				strLimitStep = GetIniFileString((*keyitem), "limitStep", "", strIniFileName);
				nLimitStep = atoi(strLimitStep.c_str());
			}
			//else
			{
				strStepName = GetIniFileString((*keyitem), "StepName", "", strIniFileName);
				strStepType = GetIniFileString((*keyitem), "StepType", "", strIniFileName);
				strPostData = GetIniFileString((*keyitem), "StepPostData", "", strIniFileName);
				strContent = GetIniFileString((*keyitem), "StepContent", "", strIniFileName);
				strErrorContent = GetIniFileString((*keyitem), "StepErrorContent", "", strIniFileName);
				strUserName = GetIniFileString((*keyitem), "StepUserName", "", strIniFileName);
				strUserPwd = GetIniFileString((*keyitem), "StepUserPwd", "", strIniFileName);

				AddListItem(strIndex, strStepName, strStepType, strPostData, strContent, strErrorContent, strUserName, strUserPwd, 0);
			}
		}
	}
    
	m_pStepListTable->show();
	m_pStepInfoTable->hide();
}

//选择全部按钮响应
void CUrlTransStepInfo::SelAll()
{
	for(m_pListItem = m_pListStep.begin(); m_pListItem != m_pListStep.end(); m_pListItem ++)
	{
		m_pListItem->pSelect->setChecked(true);
	}
}


//全部不选择按钮响应
void CUrlTransStepInfo::SelNone()
{
	for(m_pListItem = m_pListStep.begin(); m_pListItem != m_pListStep.end(); m_pListItem ++)
	{
		m_pListItem->pSelect->setChecked(false);
	}
}

//反选按钮响应
void CUrlTransStepInfo::SelInvert()
{
	for(m_pListItem = m_pListStep.begin(); m_pListItem != m_pListStep.end(); m_pListItem ++)
	{
		if(m_pListItem->pSelect->isChecked())
		{
			m_pListItem->pSelect->setChecked(false);
		}
		else
		{
			m_pListItem->pSelect->setChecked(true);
		}
	}
}

//添加客户端脚本变量
void AddJsParam(const std::string name, const std::string value, WContainerWidget *parent)
{  
    std::string strTmp("");
    strTmp += "<SCRIPT language='JavaScript' > var ";
    strTmp += name;
    strTmp += "='";
    strTmp += value;
    strTmp += "';</SCRIPT>";
	new WText(strTmp, parent);
}

typedef void(*func)(int , char **);
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void urltransstepinfomain(int argc, char * argv[])
{
    WApplication app(argc, argv);
    app.setTitle("UrlTransStepInfo");

	new WText("<SCRIPT language='JavaScript' src='/Calendar.js'></SCRIPT>", app.root());
	new WText("<SCRIPT language='JavaScript' src='/basic.js'></SCRIPT>", app.root());	
	
	new WText("<div id='frame_loadBra' class=\"panel\" style='z-index:10000; overflow:hidden; \
	      position:absolute; top:0px; left:0px; display:none;'> <table class=\"panel\" ID=\"Table1\"> <tr><td valign=\"middle\" align=\"center\"><img src='/icons/loading.gif'> \
		  </td></tr> </table> </div>", app.root());

	new WText("<div id='view_panel' class='panel_view'>", app.root());

    CUrlTransStepInfo urltransstepinfo(app.root());
	urltransstepinfo.appSelf = &app;
	app.setBodyAttribute("class='workbody' ");

	new WText("</div>");
	AddJsParam("bGeneral","true",app.root());
	AddJsParam("uistyle", "viewpan",app.root());
	AddJsParam("fullstyle", "true",app.root());
	new WText("<SCRIPT language='JavaScript' src='/Script.js'></SCRIPT>", app.root());
    app.exec();
}

//
int main(int argc, char *argv[])
{
    func p = urltransstepinfomain;

	if (argc == 1) 
    {
        char buf[256];

		WebSession s(buf, false);
        s.start(p);
        return 1;
    }
    else
    {
        FCGI_Accept();
        WebSession s("DEBUG", true);
        s.start(p);
        return 1;
    }

    return 0;
}

////优先级　Url Links Forms ．．．
//if(!pUrl->text().empty())
//{
//	strReferenceType = "url";
//	strReference = pUrl->text();
//}
//else if(!pLinks->currentText().empty() && pLinks->currentText() != "NULL")
//{
//	strReferenceType = "link";
//	strReference = pLinks->currentText();	
//}
//else if(!pForms->currentText().empty() && pForms->currentText() != "NULL")
//{
//	strReferenceType = "form";
//	strReference = pForms->currentText();	
//	if(!pPostData->text().empty())
//	{
//		strPostData = pPostData->text();	
//	}
//}
//else
//{
//	
//}


