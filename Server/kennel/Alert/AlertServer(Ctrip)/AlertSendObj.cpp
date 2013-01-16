////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//一、报警发送对象类：
//    1、数据结构
//	     a、Id、其他发送报警所需的参数、发送时间等。
//    2、调用外部Dll以发送报警的虚函数。
//    3、记录报警日志的函数。
//二、派生报警对象类：邮件、短信、脚本、声音
//    1、重写调用外部Dll以发送报警的虚函数。
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include <map>
#include <list>
#include <string>
#include <iostream>
#include <cc++/thread.h>
#include "AlertMain.h"
#include "Dragonflow.h"
#include "AlertSendObj.h"
#include "svdb/svapi/svapi.h"
#include "svdb/svapi/svdbapi.h"
#include "base/funcGeneral.h"
#include "base/des.h"
#include <afxinet.h>
#include "Base64.h"

using namespace ost;
static const basic_string <char>::size_type npos = -1;
extern void DebugePrint(string strDebugInfo);
#define XieCheng
#include "afxinet.h"
#include "windows.h"
#define  MAX_SMS_LENGTH 60
#include "base/StringHelper.h"
//#include <Python.h>
using namespace SH;

const char svPassword[] = "password";
const int  svBufferSize = 1024 * 4;
//

map<int, string, less<int> > lsDeviceId;

//////////////AlertLog日志数据库的插入函数///////

using namespace std;
using namespace svutil;
typedef unsigned char BYTE;

void WriteLog( string strDebugInfo );

//inline BYTE toHex(const BYTE &x)
//{
//	return x > 9 ? x + 55: x + 48;
//}
//
//
//string urlEncoding( string &sIn )
//{
//	string sOut;
//	for( int ix = 0; ix < sIn.size(); ix++ )
//	{
//		BYTE buf[4];
//		memset( buf, 0, 4 );
//		if( isalnum( (BYTE)sIn[ix] ) )
//		{
//			buf[0] = sIn[ix];
//		}
//		else if ( isspace( (BYTE)sIn[ix] ) )
//		{
//			buf[0] = '+';
//			cout << "sp" << endl;
//		}
//		else
//		{
//			buf[0] = '%';
//			buf[1] = toHex( (BYTE)sIn[ix] >> 4 );
//			buf[2] = toHex( (BYTE)sIn[ix] % 16);
//		}
//		sOut += (char *)buf;
//	}
//	return sOut;
//}

struct RecordHead
{

    int prercord;

    int state;

    TTime createtime;

    int datalen;
}; 

//
char *buildbuf(int data,char *pt,int buflen)
{

    if(pt==NULL)
        return NULL;

    if(buflen<sizeof(int))
        return NULL;

    memmove(pt,&data,sizeof(int));

    pt+=sizeof(int);

    return pt;
}

//
char *buildbuf(float data,char *pt,int buflen)
{

    if(pt==NULL)

        return NULL;

    if(buflen<sizeof(float))

        return NULL;

    memmove(pt,&data,sizeof(float));

    pt+=sizeof(float);

    return pt;
}

//
char *buildbuf(string data,char *pt,int buflen)
{

    if(pt==NULL)

        return NULL;

    if(buflen<data.size()+1)

        return NULL;

    strcpy(pt,data.c_str());

    pt+=data.size();

    pt[0]='\0';

    pt++;

    return pt;
}

//插记录到AlertLog表
bool InsertRecord(string strTableName, string strAlertIndex, string strAlertTime, string strRuleName, 
				  string strEntityName, string strMonitorName, string strAlertReceive, int nType, int nStatu)
{
    char data[1024]={0};

    RecordHead *prd=(RecordHead*)data;

    char *pt=data+sizeof(RecordHead);

    char *pm=NULL;
	
	if((pm=::buildbuf(strAlertIndex,pt,1024))==NULL)
    {
        //puts("build alertindex failed");

        return false;
	}
	
	OutputDebugString(strAlertIndex.c_str());
	if((pm=::buildbuf(strAlertTime,pm,1024))==NULL)
    {
        //puts("build alerttime failed");

        return false;
	}
    
	if((pm=::buildbuf(strRuleName,pm,1024))==NULL)
    {
        //puts("build rulename failed");

        return false;
	}

	if((pm=::buildbuf(strEntityName,pm,1024))==NULL)
    {
        //puts("build entityname failed");

        return false;
	}
	
	if((pm=::buildbuf(strMonitorName,pm,1024))==NULL)
    {
        //puts("build monitorname failed");

        return false;
	}
	
	if((pm=::buildbuf(strAlertReceive,pm,1024))==NULL)
    {
        //puts("build alertreceivename failed");

        return false;
	}

    if((pm=::buildbuf(nType,pm,1024))==NULL)
    {
        //puts("build nMoitorType failed");

        return false;
	}

    if((pm=::buildbuf(nStatu,pm,1024))==NULL)
    {
        //puts("build nAlertStatu failed");

        return false;
    }

    prd->datalen=pm-pt;

    prd->state=1;

    prd->createtime=svutil::TTime::GetCurrentTimeEx();

    strcpy(pm,"DynString");

    int len=pm-data;

    len+=strlen(pm)+1;

    if(!::AppendRecord(strTableName,data,len))
    {
        //puts("Append record failed");
        return false;
    }
	//else
 //       puts("Append OK");

	return true;
}

//idc 的插记录到AlertLog表
bool InsertRecord(string strTableName, string strAlertIndex, string strAlertTime, string strRuleName, string strIdcId, 
				  string strEntityName, string strMonitorName, string strAlertReceive, int nType, int nStatu)
{
    char data[1024]={0};

    RecordHead *prd=(RecordHead*)data;

    char *pt=data+sizeof(RecordHead);

    char *pm=NULL;
	
	if((pm=::buildbuf(strAlertIndex,pt,1024))==NULL)
    {
        //puts("build alertindex failed");

        return false;
	}
	
	OutputDebugString(strAlertIndex.c_str());
	if((pm=::buildbuf(strAlertTime,pm,1024))==NULL)
    {
        //puts("build alerttime failed");

        return false;
	}
    
	if((pm=::buildbuf(strRuleName,pm,1024))==NULL)
    {
        //puts("build rulename failed");

        return false;
	}

	if((pm=::buildbuf(strIdcId,pm,1024))==NULL)
    {
        //puts("build rulename failed");

        return false;
	}
	

	if((pm=::buildbuf(strEntityName,pm,1024))==NULL)
    {
        //puts("build entityname failed");

        return false;
	}
	
	if((pm=::buildbuf(strMonitorName,pm,1024))==NULL)
    {
        //puts("build monitorname failed");

        return false;
	}
	
	if((pm=::buildbuf(strAlertReceive,pm,1024))==NULL)
    {
        //puts("build alertreceivename failed");

        return false;
	}

    if((pm=::buildbuf(nType,pm,1024))==NULL)
    {
        //puts("build nMoitorType failed");

        return false;
	}

    if((pm=::buildbuf(nStatu,pm,1024))==NULL)
    {
        //puts("build nAlertStatu failed");

        return false;
    }

    prd->datalen=pm-pt;

    prd->state=1;

    prd->createtime=svutil::TTime::GetCurrentTimeEx();

    strcpy(pm,"DynString");

    int len=pm-data;

    len+=strlen(pm)+1;

    if(!::AppendRecord(strTableName,data,len))
    {
        //puts("Append record failed");
        return false;
    }
	//else
 //       puts("Append OK");

	return true;
}


string GetTemplateContent(string strType, string strSection)
{
	string strTmpValue = "", strTmpIds = "";
	strTmpIds += "IDS_";
	strTmpIds += strType;
	strTmpIds += "_";
	strTmpIds += strSection;
	strTmpIds += "_Content";

	OBJECT objRes=LoadResource("default", "localhost");  
	if( objRes !=INVALID_VALUE )
	{	
		MAPNODE ResNode=GetResourceNode(objRes);
		if( ResNode != INVALID_VALUE )
		{
			FindNodeValue(ResNode,strTmpIds.c_str(), strTmpValue);
		}
		CloseResource(objRes);
	}
	
	return strTmpValue;
}

//////////////////////////////////////////////////////

CAlertSendObj::CAlertSendObj()
{
	nType = 0;
	bUpgrade = false;
}

//
CAlertSendObj::~CAlertSendObj()
{

}

//
bool CAlertSendObj:: SendAlert()
{
	return true;
}

//
void  CAlertSendObj::InsertRecord(string strAlertReceive, int nType, int nStatu)
{
	TTime alertTime = TTime::GetCurrentTimeEx();
	string strEntityName = CAlertMain::GetDeviceTitle(strAlertMonitorId);
	string strMonitorName = CAlertMain::GetMonitorTitle(strAlertMonitorId);

	::InsertRecord("alertlogs", strAlertIndex, alertTime.Format(), strAlertName, strEntityName, strMonitorName, strAlertReceive, nType, nStatu);
}

//idc
void  CAlertSendObj::InsertRecord(string strAlertReceive, string strIdcId, int nType, int nStatu)
{
	TTime alertTime = TTime::GetCurrentTimeEx();
	string strEntityName = CAlertMain::GetDeviceTitle(strAlertMonitorId);
	string strMonitorName = CAlertMain::GetMonitorTitle(strAlertMonitorId);

	::InsertRecord("alertlogs", strAlertIndex, alertTime.Format(), strAlertName, strIdcId, strEntityName, strMonitorName, strAlertReceive, nType, nStatu);
}

//
string CAlertSendObj::GetDebugInfo()
{
	string strDebugInfo = "";
	strDebugInfo += "\r\n------------------发送事件信息开始-----------------------------\r\n";
	strDebugInfo += ("报警监测器Id：" + strAlertMonitorId + "\r\n");
	strDebugInfo += ("发送事件类型：　SendObj　\r\n");
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nSendId);
	string strCount = chItem;
	sprintf(chItem, "%d", nEventCount);
	string strEventCount = chItem;
	sprintf(chItem, "%d", nEventType);
	string strEventType = chItem;
	strDebugInfo += ("发送事件序号：" + strCount + "\r\n");
	strDebugInfo += ("原始事件序号：" + strEventCount + "\r\n");
	strDebugInfo += ("原始事件类型：" + strEventType + "\r\n");
	strDebugInfo += ("产生发送事件的报警：" + strAlertName + "\r\n");
	strDebugInfo += ("IDCUSERID：" + strIdcId + "\r\n");
	strDebugInfo += "------------------发送事件信息结束------------------------------\r\n";

	return strDebugInfo;
}

//
void CAlertSendObj::SetUpgradeTrue()
{
	bUpgrade = true;
}

//
CAlertEmailSendObj::CAlertEmailSendObj()
{	
	szSchedule = "";
	szMailTo = "";
	nType = 1;
}

//
CAlertEmailSendObj::~CAlertEmailSendObj()
{

}

//发送报警
bool CAlertEmailSendObj::SendAlert()
{	
    string szEmailServer = "", szEmailfrom = "", szUserID = "", szUserPwd = "",
			szBackServer = "" ,szTmp = "";

	//读email.ini获取smtp、发送地址等发送参数
	//////////////////////begin to modify at 07/07/31 /////////////////////////////
	//#ifdef IDC_Version
	if(GetCgiVersion().compare("IDC") == 0)
	{
	//////////////////////modify end at 07/07/31 //////////////////////////////////

		// SMTP 服务器
		szEmailServer=GetIniFileString("email_config", "server", "",  "email.ini");
		// Email from
		szEmailfrom=GetIniFileString("email_config", "from", "",  "email.ini");
		// 校验用户
		szUserID=GetIniFileString("email_config", "user", "",  "email.ini");
		// 校验密码
		szUserPwd=GetIniFileString("email_config", "password", "",  "email.ini");

		Des mydes;
		char dechar[1024]={0};
		if(szUserPwd.size()>0)
		{
			mydes.Decrypt(szUserPwd.c_str(),dechar);
			szUserPwd = dechar;
		}
		// 备份SMTP服务器
		szBackServer=GetIniFileString("email_config", "backupserver", "",  "email.ini");

	//////////////////////begin to modify at 07/07/31 /////////////////////////////
	//#else
	}
	else
	{
	//////////////////////modify end at 07/07/31 //////////////////////////////////

		// SMTP 服务器
		szEmailServer=GetIniFileString("email_config", "server", "",  "email.ini", "localhost", strIdcId);
		// Email from
		szEmailfrom=GetIniFileString("email_config", "from", "",  "email.ini", "localhost", strIdcId);
		// 校验用户
		szUserID=GetIniFileString("email_config", "user", "",  "email.ini", "localhost", strIdcId);
		// 校验密码
		szUserPwd=GetIniFileString("email_config", "password", "",  "email.ini", "localhost", strIdcId);

		Des mydes;
		char dechar[1024]={0};
		if(szUserPwd.size()>0)
		{
			mydes.Decrypt(szUserPwd.c_str(),dechar);
			szUserPwd = dechar;
		}
		// 备份SMTP服务器
		szBackServer=GetIniFileString("email_config", "backupserver", "",  "email.ini", "localhost", strIdcId);
	
	//////////////////////begin to modify at 07/07/31 /////////////////////////////
	//#endif
	}
	//////////////////////modify end at 07/07/31 //////////////////////////////////

	//读取email接收地址等

	//是否是升级邮件
	if(bUpgrade)
	{
		szMailTo = strAlertUpgradeToValue;
	}
	else
	{
		//if(strEmailAdressValue != "其他")
		//值班报警优先级高 返回空则没有值班报警(默认)
		//if(strSmsNumberValue != "其他")
		szMailTo = CAlertMain::GetCfgFromWatchList(strAlertIndex, true); //变成根据值班表项读数据呢？--不用分解alertindex
		if(szMailTo.empty())
		{
			DebugePrint("邮件 没有配置值班表");

			if(strcmp(strEmailAdressValue.c_str(), CAlertMain::strOther.c_str()) != 0)
			{
				//有接收人
				DebugePrint(strEmailAdressValue.c_str());

				//邮件接收地址禁止则返回
				if(!GetInfoFromEmailAddress(strEmailAdressValue))
				{
					DebugePrint("EmailAdress Disable");
					return true;
				}
				
				//此时的模板值为邮件接收地址设定的值：strEmailTemplateValue

				//邮件接收地址有调度条件　则根据调度匹配判断是否发送邮件
				if(!CAlertMain::IsScheduleMatch(szSchedule))
				{
					//记录日志　并记录此情况
					DebugePrint("Schedule Disable");
					return true;
				}
			}
			else
			{
				//其他邮件地址
				szMailTo = strOtherAdressValue;
				//strEmailTemplateValue = strAlertTemplateValue;
				//此时的模板值为：strEmailTemplateValue
			}
		}
		else
		{
			strEmailAdressValue = szMailTo;
			
			DebugePrint("配置了值班表");

			//有接收人
			DebugePrint(strEmailAdressValue.c_str());

			//邮件接收地址禁止则返回
			if(!GetInfoFromEmailAddress(strEmailAdressValue))
			{
				DebugePrint("EmailAdress Disable");
				return true;
			}
			
			//此时的模板值为邮件接收地址设定的值：strEmailTemplateValue

			//邮件接收地址有调度条件　则根据调度匹配判断是否发送邮件
			if(!CAlertMain::IsScheduleMatch(szSchedule))
			{
				//记录日志　并记录此情况
				DebugePrint("Schedule Disable");
				return true;
			}
		}
	}

	//////////////////////begin to modify at 07/07/31 /////////////////////////////
	//#ifdef IDC_Version
	//	if(szMailTo == "")
	//	{
	//		CAlertSendObj::InsertRecord(szMailTo, strIdcId, 1, 0);
	//		return false;
	//	}
	//#else
	//	if(szMailTo == "")
	//	{
	//		CAlertSendObj::InsertRecord(szMailTo, 1, 0);
	//		return false;
	//	}
	//#endif

	if(szMailTo == "")
	{
		if(GetCgiVersion().compare("IDC") == 0)
		{
			CAlertSendObj::InsertRecord(szMailTo, strIdcId, 1, 0);
		}
		else
		{
			CAlertSendObj::InsertRecord(szMailTo, 1, 0);
		}

		return false;
	}

	//////////////////////modify end at 07/07/31 //////////////////////////////////

	//构造AlertTitle和AlertContent
	MakeAlertTitle();
	MakeAlertContent();
	//int iEmailRet = 1;

	//发送...	
	//CAlertMain::pSendEmail(szEmailServer.c_str(), szEmailfrom.c_str(), strAlertTitle.c_str(), strAlertContent.c_str(),
	//	szEmailContent.c_str(), szUserPwd.c_str(),	szUserID.c_str(), iEmailRet);
	string strDebugInfo = "";
	strDebugInfo += "\r\n------------------发送邮件开始-----------------------------\r\n";
	strDebugInfo += ("报警监测器Id：" + strAlertMonitorId + "\r\n");
	strDebugInfo += ("发送事件类型：　Email　\r\n");
	strDebugInfo += ("strEmailAdressValue：" + strEmailAdressValue + "\r\n");
	strDebugInfo += ("szEmailServer：" + szEmailServer + "\r\n");
	strDebugInfo += ("szEmailfrom：" + szEmailfrom + "\r\n");
	strDebugInfo += ("szMailTo：" + szMailTo + "\r\n");
	strDebugInfo += ("strAlertTitle：" + strAlertTitle + "\r\n");
	strDebugInfo += ("strAlertContent：" + strAlertContent + "\r\n");
	strDebugInfo += ("szUserID：" + szUserID + "\r\n");
	strDebugInfo += ("szUserPwd：" + szUserPwd + "\r\n");
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nSendId);
	string strCount = chItem;
	sprintf(chItem, "%d", nEventCount);
	string strEventCount = chItem;
	sprintf(chItem, "%d", nEventType);
	string strEventType = chItem;
	strDebugInfo += ("发送事件序号：" + strCount + "\r\n");
	strDebugInfo += ("原始事件序号：" + strEventCount + "\r\n");
	strDebugInfo += ("原始事件类型：" + strEventType + "\r\n");
	strDebugInfo += ("IDCUSERID：" + strIdcId + "\r\n");
	
	DebugePrint(strDebugInfo);
	
	//如果是多个邮件地址用逗号分隔则 
	std::list<string> listEmail;
	std::list<string>::iterator listEmailItem;
	OutputDebugString (strAlertContent.c_str());
	bool bSucess = false;
	bool bAllSucess = true;

	try
	{
		/*
		CAlertMain::ParserToken(listEmail, szMailTo.c_str(), ",");		
		
		for(listEmailItem = listEmail.begin(); listEmailItem!=listEmail.end(); listEmailItem++)
		{
//			strAlertTitle+="张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件张杰文开始发送邮件";
//		DebugePrint("张杰文开始发送邮件");
//			DebugePrint("张杰文开始发送邮件");
			DebugePrint(strAlertTitle);
		//modify by zjw 张杰文，暂时这么做
		//调用其它邮件发送程序的类
//			bSucess = CAlertMain::pSendEmail(szEmailServer.c_str(), szEmailfrom.c_str(), (*listEmailItem).c_str(),
//				strAlertTitle.c_str(), strAlertContent.c_str(), szUserID.c_str(), szUserPwd.c_str(), NULL);
			CMIMEMessage msg;
			CSMTP smtp( szEmailServer.c_str());
		   
			msg.m_sFrom    = szEmailfrom.c_str();
				msg.m_sSubject = strAlertTitle.c_str();
				msg.m_sBody    = strAlertContent.c_str();
			msg.AddMultipleRecipients( (*listEmailItem).c_str() );

			if ( !szUserID.empty() )
			{
				msg.AddMultipleRecipients( szUserID.c_str(), CMailMessage::CC);
			}

			if ( !szUserPwd.empty() )
			{
   				msg.AddMultipleRecipients( szUserPwd.c_str(), CMailMessage::BCC);
			}
			
			smtp.Connect();
			
			bSucess=smtp.SendMessage( &msg );
			if(!bSucess)
			{
				bAllSucess = false;
				DebugePrint("发送邮件不成功");
			}
			else
				DebugePrint("发送邮件成功");

		}
		*/
		CAlertMain::ParserToken(listEmail, szMailTo.c_str(), ",");		
		
		for(listEmailItem = listEmail.begin(); listEmailItem!=listEmail.end(); listEmailItem++)
		{
			bSucess = CAlertMain::pSendEmail(szEmailServer.c_str(), szEmailfrom.c_str(), (*listEmailItem).c_str(),
				strAlertTitle.c_str(), strAlertContent.c_str(), szUserID.c_str(), szUserPwd.c_str(), NULL);
			//bSucess = CAlertMain::SendMail(szEmailServer, szEmailfrom, (*listEmailItem),
			//	strAlertTitle, strAlertContent, szUserID, szUserPwd);

			if(!bSucess)
			{
				bAllSucess = false;
				DebugePrint("\r\n*****发送邮件不成功*****\r\n");
			}
			else
				DebugePrint("\r\n*****发送邮件成功*****\r\n");

		}

		//////////////////////begin to modify at 07/07/31 /////////////////////////////
		//#ifdef IDC_Version
		//	if(bAllSucess)
		//	{		
		//		CAlertSendObj::InsertRecord(szMailTo, strIdcId, 1, 1);
		//	}
		//	else
		//	{
		//		CAlertSendObj::InsertRecord(szMailTo, strIdcId, 1, 0);
		//	}
		//#else
		//	if(bAllSucess)
		//	{		
		//		CAlertSendObj::InsertRecord(szMailTo, 1, 1);
		//	}
		//	else
		//	{
		//		CAlertSendObj::InsertRecord(szMailTo, 1, 0);
		//	}
		//#endif

		if(GetCgiVersion().compare("IDC") == 0)
		{
			if(bAllSucess)
			{		
				CAlertSendObj::InsertRecord(szMailTo, strIdcId, 1, 1);
			}
			else
			{
				CAlertSendObj::InsertRecord(szMailTo, strIdcId, 1, 0);
			}

		}
		else
		{
			if(bAllSucess)
			{		
				CAlertSendObj::InsertRecord(szMailTo, 1, 1);
			}
			else
			{
				CAlertSendObj::InsertRecord(szMailTo, 1, 0);
			}

		}
		//////////////////////modify end at 07/07/31 //////////////////////////////////

	}
	catch(...)
	{
		//////////////////////begin to modify at 07/07/31 /////////////////////////////
		//#ifdef IDC_Version
		//		CAlertSendObj::InsertRecord(szMailTo, strIdcId, 1, 0);
		//#else
		//		CAlertSendObj::InsertRecord(szMailTo, 1, 0);
		//#endif

		if(GetCgiVersion().compare("IDC") == 0)
		{
			CAlertSendObj::InsertRecord(szMailTo, strIdcId, 1, 0);
		}
		else
		{
			CAlertSendObj::InsertRecord(szMailTo, 1, 0);
		}
		//////////////////////modify end at 07/07/31 //////////////////////////////////

	}

	strDebugInfo += "------------------发送邮件结束------------------------------\r\n";
	DebugePrint(strDebugInfo);
	

	return bSucess;
}

//
string CAlertEmailSendObj::GetDebugInfo()
{
	string strDebugInfo = "";
	strDebugInfo += "\r\n------------------发送事件信息开始-----------------------------\r\n";
	strDebugInfo += ("报警监测器Id：" + strAlertMonitorId + "\r\n");
	strDebugInfo += ("发送事件类型：　Email　\r\n");
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nSendId);
	string strCount = chItem;
	sprintf(chItem, "%d", nEventCount);
	string strEventCount = chItem;
	sprintf(chItem, "%d", nEventType);
	string strEventType = chItem;
	strDebugInfo += ("发送事件序号：" + strCount + "\r\n");
	strDebugInfo += ("原始事件序号：" + strEventCount + "\r\n");
	strDebugInfo += ("原始事件类型：" + strEventType + "\r\n");
	strDebugInfo += ("产生发送事件的报警：" + strAlertName + "\r\n");
	strDebugInfo += ("IDCUSERID：" + strIdcId + "\r\n");
	strDebugInfo += "------------------发送事件信息结束------------------------------\r\n";
	return strDebugInfo;
}



void UnicodeToUTF8(char* pOut,wchar_t* pText)
{
    char* pchar = (char *)pText;
    pOut[0] = (0xE0 | ((pchar[1] & 0xF0) >> 4));
    pOut[1] = (0x80 | ((pchar[1] & 0x0F) << 2)) + ((pchar[0] & 0xC0) >> 6);
    pOut[2] = (0x80 | (pchar[0] & 0x3F));
}
   

string GB2312ToUTF8(string intext)
{
	if(intext.empty())
		return "";

	int pLen= intext.size();
	int nLength = pLen* 3;  // exactly should be:  *3/2 +1

	char *pText=new char[nLength];
	if(pText==NULL)
		return "";
	memset(pText,0,nLength);
	strcpy(pText,intext.c_str());

	char* rst = new char[nLength];
	if(rst==NULL)
	{
		delete []pText; 
		return "";
	}
	memset(rst,0,nLength);
 
	int i,j;
	char buf[4]={0};
	for(i=0,j=0; i<pLen; )
	{
		if( *(pText + i) >= 0)
			rst[j++] = pText[i++];
		else
		{
			wchar_t pbuffer;
			MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,pText+i,2,&pbuffer,1); //Gb2312ToUnicode
			UnicodeToUTF8(buf,&pbuffer);
			memmove(rst+j,buf,3);

			j += 3;
			i += 2;
		}
	}
	rst[j] = '\0';

	string str = rst;            
	delete []rst;  
	delete []pText; 

	return str;
}


void UTF8ToUnicode(wchar_t* pOut, char* pText)
{
	char* uchar = (char *)pOut;
	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
}
   

string UTF8ToGB2312(string intext)
{
	if(intext.empty())
		return "";

	int pLen= intext.size();
	int nLength = pLen* 3; // exactly should be:  +1

	char *pText=new char[nLength];
	if(pText==NULL)
		return "";
	memset(pText,0,nLength);
	strcpy(pText,intext.c_str());

	char* rst = new char[nLength];
	if(rst==NULL)
	{
		delete []pText; 
		return "";
	}
	memset(rst,0,nLength);
 
	char buf[4]={0};
	int i,j;
	for(i=0,j=0; i<pLen; )
	{
		if( *(pText + i) >= 0)
			rst[j++] = pText[i++];
		else
		{
			wchar_t pbuffer;
			UTF8ToUnicode(&pbuffer,pText+i);
			WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,&pbuffer,1,buf,sizeof(WCHAR),NULL,NULL);//UnicodeToGb2312
			memmove(rst+j,buf,2);

			i += 3;
			j += 2;
		}
	}
	rst[j] = '\0';

	string str = rst;            
	delete []rst;  
	delete []pText; 

	return str;
}

//生成报警标题
void CAlertEmailSendObj::MakeAlertTitle()
{
#ifndef XieCheng
	strAlertTitle += CAlertMain::GetMonitorTitle(strAlertMonitorId);
	strAlertTitle += "-";
	strAlertTitle += CAlertMain::GetDeviceTitle(strAlertMonitorId);
#else	
	//携程报警改造 为适应 ITSM ...
	//!SV-200804021235!设备名.监测器名称 + ＂＂ +  warnning( error ..)  + ＂＂ +  procName( ServerName )　 + ＂＂ +  Down( OK ..)
	char chTime[50] = {0};
   	SYSTEMTIME time;
	
	//取系统当前时间
	GetLocalTime(&time);
    sprintf(chTime ,"SV-%04d%02d%02d%02d%02d",
				time.wYear,
				time.wMonth,
				time.wDay,
				time.wHour,
				time.wMinute);
				//time.wSecond);
	strAlertTitle = chTime;
	strAlertTitle += "!";
	strAlertTitle += CAlertMain::GetDeviceTitle(strAlertMonitorId);
	//strAlertTitle += "_";
	//strAlertTitle += CAlertMain::GetDeviceTitle(strAlertMonitorId);
	strAlertTitle += ".";
	//strAlertTitle += CAlertMain::GetMonitorTitle(strAlertMonitorId);
	string strMonitorName = CAlertMain::GetMonitorTitle(strAlertMonitorId);
	basic_string <char>::size_type index = strMonitorName.find( "：" );

	if( index != basic_string<char>::npos )
	{
        strMonitorName.replace( index, 1, ":" );
		basic_string <char>::iterator it = strMonitorName.begin() + index + 1;
		strMonitorName.erase( it );
	}

	strAlertTitle += strMonitorName;
	
	
	/*
	//procName( ServerName )
	string strMonitorType = CAlertMain::GetMonitorPropValue(strAlertMonitorId, "sv_monitortype");	
	if(strMonitorType == "14" || strMonitorType == "33" || strMonitorType == "41"
		|| strMonitorType == "111" || strMonitorType == "174"  || strMonitorType == "175")
	{
		// 14 Service  33 Nt4.0Process  41 Process  111 UnixProcess  174 SNMP_Process  175 SNMP_Service
		strAlertTitle += ":";		

		string strProcName = "";
		OBJECT hMon = GetMonitor(strAlertMonitorId);
		MAPNODE paramNode = GetMonitorParameter(hMon);		
		if(strMonitorType == "14")
		{
			FindNodeValue(paramNode, "_Service", strProcName);
		}
		else if(strMonitorType == "33")
		{
			FindNodeValue(paramNode, "_monitorProcessList", strProcName);
		}
		else if(strMonitorType == "41")
		{
			FindNodeValue(paramNode, "_monitorProcessList", strProcName);
		}
		else if(strMonitorType == "111")
		{
			FindNodeValue(paramNode, "_Service", strProcName);
		}
		else if(strMonitorType == "174")
		{
			FindNodeValue(paramNode, "_SelValue", strProcName);
		}
		else if(strMonitorType == "175")
		{
			FindNodeValue(paramNode, "_InterfaceIndex", strProcName);
		}
		else
		{
			
		}
		strAlertTitle += strProcName;

		
		//procName( ServerName ) Down( OK ..) 取进程个数呢?。。。
		strAlertTitle += " ";		
		if(nEventType == 1)
		{
			//1、正常。
			strAlertTitle += "Ok";
		}
		else if(nEventType == 3 || nEventType == 4 || nEventType == 5)
		{
			//3、错误。4、禁止。5、错误。
			strAlertTitle += "Down";
		}
		else
		{
			//2、危险。
			strAlertTitle += "Down";
		}
		//string strAlertTitleTmp = GB2312ToUTF8(strAlertTitle);
		//strAlertTitle = strAlertTitleTmp;

	}
	*/


	//资源状态
	strAlertTitle += " ";	
	//1、正常。2、危险。3、错误。4、禁止。5、错误。 warnning( error ..) 
	if(nEventType == 1)
	{
		strAlertTitle += "Ok";
	}
	else if(nEventType == 2)
	{
		strAlertTitle += "Warning";
	}
	else if(nEventType == 3)
	{
		strAlertTitle += "Error";
	}
	else if(nEventType == 4)
	{
		strAlertTitle += "Disable";
	}
	/*
	else if(nEventType == 5)
	{
		strAlertTitle += "Disable";
	}
	*/
	else
	{
		strAlertTitle += "Error";
	}

#endif
}


// 将阈值变量的名字换成中文
bool CAlertEmailSendObj::ToChinese(string& strDes,const string strSour)
{
	bool ret = false;

	OBJECT hMon = GetMonitor(strAlertMonitorId);
	std::string getvalue;
	MAPNODE ma=GetMonitorMainAttribNode(hMon);	
	FindNodeValue( ma,"sv_monitortype",getvalue);
	
	OBJECT hTemplet = GetMonitorTemplet(atoi(getvalue.c_str()));
	LISTITEM item;
	FindMTReturnFirst(hTemplet,item);
	MAPNODE returnobjNode;
	while( (returnobjNode = FindNext(item)) != INVALID_VALUE )
	{
		string strReturn;
		FindNodeValue(returnobjNode, "sv_name", strReturn);
		if(strReturn==strSour)
		{
			FindNodeValue(returnobjNode,"sv_label",strDes);
			ret = true;
		}
	}

	//CloseResource(ma);

	CloseMonitorTemplet(hTemplet);
	CloseMonitor(hMon);

	return ret;
}





//生成报警内容
void CAlertEmailSendObj::MakeAlertContent()
{
	string strTmp;
	strAlertContent = GetIniFileString("Email", strEmailTemplateValue, "", "TxtTemplate.Ini");
	//strAlertContent = GetTemplateContent("Email", strEmailTemplateValue);

	int nLength = strAlertContent.length();
	
	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Group@", CAlertMain::GetDeviceTitle(strAlertMonitorId));
	strAlertContent = strTmp;

	string strMonitorName = CAlertMain::GetMonitorTitle( strAlertMonitorId );

	string strMonitorType = CAlertMain::GetMonitorPropValue(strAlertMonitorId, "sv_monitortype");	
	if(strMonitorType == "14" || strMonitorType == "33" || strMonitorType == "41"
		|| strMonitorType == "111" || strMonitorType == "174"  || strMonitorType == "175")
	{
		// 14 Service  33 Nt4.0Process  41 Process  111 UnixProcess  174 SNMP_Process  175 SNMP_Service
		strMonitorName += ":";		

		string strProcName = "";
		OBJECT hMon = GetMonitor(strAlertMonitorId);
		MAPNODE paramNode = GetMonitorParameter(hMon);		
		if(strMonitorType == "14")
		{
			FindNodeValue(paramNode, "_Service", strProcName);
		}
		else if(strMonitorType == "33")
		{
			FindNodeValue(paramNode, "_monitorProcessList", strProcName);
		}
		else if(strMonitorType == "41")
		{
			FindNodeValue(paramNode, "_monitorProcessList", strProcName);
		}
		else if(strMonitorType == "111")
		{
			FindNodeValue(paramNode, "_Service", strProcName);
		}
		else if(strMonitorType == "174")
		{
			FindNodeValue(paramNode, "_SelValue", strProcName);
		}
		else if(strMonitorType == "175")
		{
			FindNodeValue(paramNode, "_InterfaceIndex", strProcName);
		}
		else
		{
			
		}
		strMonitorName += strProcName;
	}
	
	//strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@monitor@", CAlertMain::GetMonitorTitle(strAlertMonitorId));
	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@monitor@", strMonitorName );
	strAlertContent = strTmp;

	string strDeviceId = FindParentID(strAlertMonitorId);
	//string strGrouptId = FindParentID(strDeviceId);
	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@AllGroup@", CAlertMain::GetAllGroupTitle(strDeviceId));
	strAlertContent = strTmp;

	//strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Status@", CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_stateString"));
	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Status@", strEventDes);	
	strAlertContent = strTmp;

	int nUnit = 0;
	sscanf(CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_frequencyUnit").c_str(), "%d", &nUnit);
	string strFreq;
	if(nUnit==60)
	{
		//strFreq = "监测频率:";
		//strFreq += CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_frequency");
		//strFreq += "分钟";
		strFreq = CAlertMain::strMontorFreq.c_str();
		strFreq += ":";
		strFreq += CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_frequency");
		strFreq += CAlertMain::strMinute.c_str();

	}
	else
	{
		//strFreq = "监测频率:";
		//strFreq += CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_frequency");
		//strFreq += "小时";

		strFreq = CAlertMain::strMontorFreq.c_str();
		strFreq += ":";
		strFreq += CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_frequency");
		strFreq += CAlertMain::strHour.c_str();
	}

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@frequency@", strFreq);
	strAlertContent = strTmp;


	//strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Time@",  CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_lastMeasurementTime"));
	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Time@",  strTime);	
	strAlertContent = strTmp;

	//2006-10-23 jiang 
	OBJECT hTemplet;
	OBJECT hMon = GetMonitor(strAlertMonitorId);
	/*
	std::string getvalue;
	MAPNODE ma=GetMonitorMainAttribNode(hMon);
	std::string szErrorValue;
	//monitortemplet ID
	if ( FindNodeValue( ma,"sv_monitortype",getvalue) )
	{			
		//monitortemplet 句柄
		hTemplet = GetMonitorTemplet(atoi(getvalue.c_str()));
		MAPNODE node = GetMTMainAttribNode(hTemplet);
		//monitortemplet 标签
		
		//报告设置是否显示阀值
		MAPNODE errorNode = GetMTErrorAlertCondition(hTemplet);
		FindNodeValue(errorNode, "sv_value", szErrorValue);			

	}
	*/

	//--------------------------------------------------------------------
	//_zouxiao_2008.7.18
	//读实际存在的监测器信息，而不是监测器模板的信息
	string strErrorValue;
	
	MAPNODE map;
	
	if(nEventType == 1)
	{
		map=GetMonitorGoodAlertCondition(hMon);
	}
	else if(nEventType == 2)
	{
		map=GetMonitorWarningAlertCondition(hMon);
	}
	else if(nEventType == 3)
	{
		map=GetMonitorErrorAlertCondition(hMon);
	}
	else if(nEventType == 4)
	{
		map=GetMonitorErrorAlertCondition(hMon);
	}
	else if(nEventType == 5)
	{
		map=GetMonitorErrorAlertCondition(hMon);
	}
	else
	{
		map=GetMonitorErrorAlertCondition(hMon);
	}

	string strCondCount;
	FindNodeValue(map,"sv_conditioncount",strCondCount);
	int nCondCount=atoi(strCondCount.c_str());

	for(int i=0;i!=nCondCount;i++)
	{
		char chtemp[256];
		string strtemp;

		if(i!=0)
		{
			strErrorValue+=" ";
			
			FindNodeValue(map,"sv_expression",strtemp);

			sprintf(chtemp,"%d#",i);

			int pos1=strtemp.find(chtemp);
			
			int j=i+1;
			memset(chtemp,0,256);

			sprintf(chtemp,"#%d",j);

			int pos2=strtemp.find(chtemp);

			

			int count=pos2-pos1-2;
			strErrorValue+=strtemp.substr(pos1+2,count);
			
			strErrorValue+=" ";
		}
		
		sprintf(chtemp,"sv_paramname%d",i+1);
        FindNodeValue(map,chtemp,strtemp);
		string strchinesetemp;
		ToChinese(strchinesetemp,strtemp);
		strErrorValue+=strchinesetemp;
		//strErrorValue+=strtemp;

		sprintf(chtemp,"sv_operate%d",i+1);
        FindNodeValue(map,chtemp,strtemp);
		strErrorValue+=strtemp;

		sprintf(chtemp,"sv_paramvalue%d",i+1);
        FindNodeValue(map,chtemp,strtemp);
		strErrorValue+=strtemp;
	}

	if( hMon != INVALID_VALUE )
	{
		CloseMonitor(hMon);
	}
	//--------------------------------------------------------------------
	
	//strAlertContent += "\n阀值:     ";
	strAlertContent += "\n";
	strAlertContent += CAlertMain::strMonitorFazhi.c_str();
	strAlertContent += "       ";
	strAlertContent += strErrorValue;
	strAlertContent += "\n";
	//add end

	/*
	//Begin
	//替换所有格式如下的 @@Key@@ ，得到Key，并得到相应的键值
	//printf("Log Start");
	CString strMfcTemp, strTempKey, strPath,strLocalContent;	
	strLocalContent= CString(strAlertContent.c_str());
	CString strTempKeyValue = _T(""); 
	while(strLocalContent.Find("@",0)>0)
	{
		//printf("1");
		int nTemp = strLocalContent.Find("@",0);
		strMfcTemp = strLocalContent.Right(strLocalContent.GetLength() - nTemp - 1);
		//printf(strMfcTemp);
		strLocalContent = strLocalContent.Left(nTemp);
		//printf(strLocalContent);
		nTemp = strMfcTemp.Find("@",0);
		strTempKey = strMfcTemp.Left(nTemp);
		strMfcTemp = strTempKey;
		//printf(strMfcTemp);
		//strMfcTemp = strMfcTemp.Right(strMfcTemp.GetLength()-nTemp-1);
		//strTempKeyValue=FuncGetProfileStringBy(strSection,strTempKey,strGroupFile);
		//printf(strMfcTemp);
		if(strMfcTemp == "Log")
		{
			//printf("Entenr Log");
			//文件名称
			strPath.Format("%s\\%s\\%s.txt", GetSiteViewRootPath().c_str(), "data\\Temp", strAlertMonitorId.c_str());

			//printf(strPath);

			//取出该次报警对应的数据并替换@@Log@@
			//根据strTime判断， 如果最近一次时间大于。。。 如果最近一次时间小于。。。
			int nFileLength = 0;
			CFile* pFile = NULL;
			TRY	
			{
				pFile = new CFile(strPath, CFile::modeRead | CFile::shareDenyNone);
				nFileLength = pFile->GetLength();
			}
			CATCH(CFileException, pEx)	
			{
				if (pFile != NULL) 
				{
					pFile->Close();
					delete pFile;
				}

				return;
			}
			END_CATCH

			if (pFile != NULL) 
			{
				pFile->Close();
				delete pFile;
			}

			if (0 == nFileLength)
				return;

			CStringEx strTotleContent = _T("");
			CStringEx strLogContent = _T("");
			FILE * fp = fopen((LPCTSTR)strPath, "r");
			if (fp)
			{
				char * buffer = NULL;
				buffer = (char*)malloc(nFileLength + 1);
				if (buffer) 
				{
					memset(buffer, 0, nFileLength + 1);
					fread(buffer, sizeof(char), nFileLength + 1, fp);
					strTotleContent.Format("%s", buffer);	
					free(buffer);
				}
				fclose(fp);
			}
			
			if(strTotleContent != "")
			{
				int nStart = strTotleContent.ReverseFind("[Time is", -1);
				//strLogContent = strTotleContent.Right(strTotleContent.GetLength() - nStart - 2);
				strLogContent = strTotleContent.Right(strTotleContent.GetLength() - nStart);
				//DebugePrint("ReverseFind:\n");
				//DebugePrint(strLogContent);
				//printf(strLogContent);
				string strStdLogContent = strLogContent;
				//DebugePrint(strStdLogContent.c_str());
				strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Log@",  strStdLogContent);	
				strAlertContent = strTmp;
			}
		}
		

		//printf("Key %s Value is %s\n",strTempKey,strTempKeyValue);
		
		//if(!strTempKeyValue.IsEmpty())
		//{			
		//	strLocalContent=strLocalContent+strTempKeyValue+strTemp;
		//}
		//else
		//{
		//	strLocalContent=strLocalContent+"无设定"+strTemp;
		//}
	}
	*/
	//End
	//printf("Log End");
}

//
void CAlertEmailSendObj::SetUpgradeTrue()
{
	bUpgrade = true;
}

//获取Schedule和MailList信息
bool CAlertEmailSendObj::GetInfoFromEmailAddress(string strAddressName)
{
	bool bReturn = true;
	//emailAdress.ini
	std::list<string> keylist;
	std::list<string>::iterator keyitem;

	//////////////////////begin to modify at 07/07/31 /////////////////////////////
	//#ifdef IDC_Version
	if(GetCgiVersion().compare("IDC") == 0)
	{
	//////////////////////modify end at 07/07/31 //////////////////////////////////

		//获取地址列表。。。
		if(GetIniFileSections(keylist, "emailAdress.ini"))
		{
			//初始化地址列表。。。
			for(keyitem = keylist.begin(); keyitem != keylist.end(); keyitem ++)	
			{
				//printf((*keyitem).c_str());
				//printf(GetIniFileString((*keyitem), "Name", "" , "emailAdress.ini").c_str());
				if(strAddressName == GetIniFileString((*keyitem), "Name", "" , "emailAdress.ini"))
				{
					//printf(" GetInfoFromEmailAddress szMailTo");
					if(GetIniFileInt((*keyitem), "bCheck", 0, "emailAdress.ini") != 0)
						bReturn = false;
					szMailTo = GetIniFileString((*keyitem), "MailList", "", "emailAdress.ini");
					//GetIniFileString((*keyitem), "Template", "", "emailAdress.ini");
					szSchedule = GetIniFileString((*keyitem), "Schedule", "", "emailAdress.ini");
					//GetIniFileString((*keyitem), "Des", "" , "emailAdress.ini");

					//szSchedule = GetIniFileString((*keyitem), "Schedule", "", "emailAdress.ini");
					strEmailTemplateValue = GetIniFileString((*keyitem), "Template", "", "emailAdress.ini");
				}
			}
		}

	//////////////////////begin to modify at 07/07/31 /////////////////////////////
	//#else
	}
	else
	{
	//////////////////////modify end at 07/07/31 //////////////////////////////////

		//获取地址列表。。。
		if(GetIniFileSections(keylist, "emailAdress.ini", "localhost", strIdcId))
		{
			//初始化地址列表。。。
			for(keyitem = keylist.begin(); keyitem != keylist.end(); keyitem ++)	
			{
				//printf((*keyitem).c_str());
				//printf(GetIniFileString((*keyitem), "Name", "" , "emailAdress.ini").c_str());
				if(strAddressName == GetIniFileString((*keyitem), "Name", "" , "emailAdress.ini", "localhost", strIdcId))
				{
					//printf(" GetInfoFromEmailAddress szMailTo");
					if(GetIniFileInt((*keyitem), "bCheck", 0, "emailAdress.ini", "localhost", strIdcId) != 0)
						bReturn = false;
					szMailTo = GetIniFileString((*keyitem), "MailList", "", "emailAdress.ini", "localhost", strIdcId);
					//GetIniFileString((*keyitem), "Template", "", "emailAdress.ini");
					szSchedule = GetIniFileString((*keyitem), "Schedule", "", "emailAdress.ini", "localhost", strIdcId);
					//GetIniFileString((*keyitem), "Des", "" , "emailAdress.ini");

					//szSchedule = GetIniFileString((*keyitem), "Schedule", "", "emailAdress.ini");
					strEmailTemplateValue = GetIniFileString((*keyitem), "Template", "", "emailAdress.ini", "localhost", strIdcId);
				}
			}
		}

	//////////////////////begin to modify at 07/07/31 /////////////////////////////
	//#endif
	}
	//////////////////////modify end at 07/07/31 //////////////////////////////////

	return bReturn;
}

//
CAlertSmsSendObj::CAlertSmsSendObj()
{
	szSchedule = "";
	szSmsTo = "";
	nType = 2;
}

//
CAlertSmsSendObj::~CAlertSmsSendObj()
{

}

//
bool CAlertSmsSendObj:: SendAlert()
{
    string szWebUser = "", szWebPwd = "", szComPort = "", szTmp = "";

	//读email.ini获取smtp、发送地址等发送参数

	//////////////////////begin to modify at 07/07/31 /////////////////////////////
	//#ifdef IDC_Version
	if(GetCgiVersion().compare("IDC") == 0)
	{
	//////////////////////modify end at 07/07/31 //////////////////////////////////

		// SMTP 服务器
		szWebUser=GetIniFileString("SMSWebConfig", "User", "",  "smsconfig.ini");
		// Email from
		szWebPwd=GetIniFileString("SMSWebConfig", "Pwd", "",  "smsconfig.ini");
		Des mydes;
		char dechar[1024]={0};
		if(szWebPwd.size()>0)
		{
			mydes.Decrypt(szWebPwd.c_str(),dechar);
			szWebPwd = dechar;
		}

		// 校验用户
		szComPort=GetIniFileString("SMSCommConfig", "Port", "",  "smsconfig.ini");

	//////////////////////begin to modify at 07/07/31 /////////////////////////////
	//#else
	}
	else
	{
	//////////////////////modify end at 07/07/31 //////////////////////////////////

		// SMTP 服务器
		szWebUser=GetIniFileString("SMSWebConfig", "User", "",  "smsconfig.ini", "localhost", strIdcId);
		// Email from
		szWebPwd=GetIniFileString("SMSWebConfig", "Pwd", "",  "smsconfig.ini", "localhost", strIdcId);
		Des mydes;
		char dechar[1024]={0};
		if(szWebPwd.size()>0)
		{
			mydes.Decrypt(szWebPwd.c_str(),dechar);
			szWebPwd = dechar;
		}

		// 校验用户
		szComPort=GetIniFileString("SMSCommConfig", "Port", "",  "smsconfig.ini", "localhost", strIdcId);

	//////////////////////begin to modify at 07/07/31 /////////////////////////////
	//#endif
	}
	//////////////////////modify end at 07/07/31 //////////////////////////////////

	//读取email接收地址等

	//是否是升级邮件
	if(bUpgrade)
	{
		szSmsTo = strAlertUpgradeToValue;
	}
	else
	{
		//值班报警优先级高 返回空则没有值班报警(默认)
		//if(strSmsNumberValue != "其他")
		szSmsTo = CAlertMain::GetCfgFromWatchList(strAlertIndex, false); //变成根据值班表项读数据呢？--不用分解alertindex
		if(szSmsTo.empty())
		{
			DebugePrint("没有配置值班表");

			//有接收人
			if(strcmp(strSmsNumberValue.c_str(), CAlertMain::strOther.c_str()) != 0)	
			{
				//短信接收地址禁止则返回
				if(!GetInfoFromSmsAddress(strSmsNumberValue))
				{
					DebugePrint("SmsAdress Disable");
					return true;
				}
				
				//此时的模板值为邮件接收地址设定的值：strEmailTemplateValue

				//短信接收地址有调度条件　则根据调度匹配判断是否发送邮件
				if(!CAlertMain::IsScheduleMatch(szSchedule))
				{
					//记录日志　并记录此情况
					DebugePrint("Schedule Disable");
					return true;
				}
			}
			else
			{
				//其他邮件地址
				szSmsTo = strOtherNumberValue;
				
				//此时的模板值为：strEmailTemplateValue
			}
		}
		else
		{
			//值班报警
			DebugePrint("配置了值班表");
			//只szSmsTo参数　 够吗?
			strSmsNumberValue = szSmsTo;
			
			//短信接收地址禁止则返回
			if(!GetInfoFromSmsAddress(strSmsNumberValue))
			{
				DebugePrint("SmsAdress Disable");
				return true;
			}
			
			//此时的模板值为邮件接收地址设定的值：strEmailTemplateValue

			//短信接收地址有调度条件　则根据调度匹配判断是否发送邮件
			if(!CAlertMain::IsScheduleMatch(szSchedule))
			{
				//记录日志　并记录此情况
				DebugePrint("Schedule Disable");
				return true;
			}
		}
	}

	//////////////////////begin to modify at 07/07/31 /////////////////////////////
	//#ifdef IDC_Version
	//	if(szSmsTo == "")
	//	{
	//		CAlertSendObj::InsertRecord(szSmsTo, strIdcId, 2, 0);
	//		return false;
	//	}
	//#else
	//	if(szSmsTo == "")
	//	{
	//		return false;
	//	}
	//#endif

	if(szSmsTo == "")
	{
		if(GetCgiVersion().compare("IDC") == 0)
		{
			CAlertSendObj::InsertRecord(szSmsTo, strIdcId, 2, 0);
		}
		else
		{
			CAlertSendObj::InsertRecord(szSmsTo, 2, 0);

		}

		return false;
	}

	//////////////////////modify end at 07/07/31 //////////////////////////////////

	//构造AlertTitle和AlertContent
	MakeAlertTitle();
	MakeAlertContent();
	//int iEmailRet = 1;

	printf( "报警内容：%s\n", strAlertContent.c_str() );



	//发送...	
	string strDebugInfo = "";
	strDebugInfo += "\r\n------------------发送短信开始-----------------------------\r\n";
	strDebugInfo += ("报警监测器Id：" + strAlertMonitorId + "\r\n");
	strDebugInfo += ("发送事件类型：　Sms　\r\n");
	strDebugInfo += ("strSmsNumberValue：" + strSmsNumberValue + "\r\n");
	strDebugInfo += ("szWebUser：" + szWebUser + "\r\n");
	strDebugInfo += ("szWebPwd：" + szWebPwd + "\r\n");
	strDebugInfo += ("szComPort：" + szComPort + "\r\n");
	strDebugInfo += ("szSmsTo：" + szSmsTo + "\r\n");
	strDebugInfo += ("strAlertTitle：" + strAlertTitle + "\r\n");
	strDebugInfo += ("strAlertContent：" + strAlertContent + "\r\n");
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nSendId);
	string strCount = chItem;
	sprintf(chItem, "%d", nEventCount);
	string strEventCount = chItem;
	sprintf(chItem, "%d", nEventType);
	string strEventType = chItem;
	strDebugInfo += ("发送事件序号：" + strCount + "\r\n");
	strDebugInfo += ("原始事件序号：" + strEventCount + "\r\n");
	strDebugInfo += ("原始事件类型：" + strEventType + "\r\n");
	strDebugInfo += ("IDCUSERID：" + strIdcId + "\r\n");

	//如果是多个手机号码用逗号分隔则 
	bool bSucess = false;
	try
	{
		OutputDebugString("-----------------call senalert() for test selfdefine------------\n");
		if(strSmsSendMode == "Web")
		{
			bSucess = SendSmsFromWeb();
		}
		else if(strSmsSendMode == "Com")
		{
			bSucess = SendSmsFromCom();
		}
		else if(strSmsSendMode != "")
		{
			bSucess = SendSmsFromSelfDefine();
		}
		else 
		{
			OutputDebugString("---------call sendsmsfromselfdefine()-------------------\n");
			DebugePrint("---------call sendsmsfromselfdefine()-------------------\n");
			//bSucess = SendSmsFromSelfDefine();
			bSucess = false;
		}

		strDebugInfo += "------------------发送短信结束------------------------------\r\n";
		DebugePrint(strDebugInfo);

		//////////////////////begin to modify at 07/07/31 /////////////////////////////
		//#ifdef IDC_Version		
		//	if(bSucess)
		//	{
		//		CAlertSendObj::InsertRecord(szSmsTo, strIdcId, 2, 1);
		//	}
		//	else
		//	{
		//		CAlertSendObj::InsertRecord(szSmsTo, strIdcId, 2, 0);
		//	}
		//#else
		//	if(bSucess)
		//	{
		//		CAlertSendObj::InsertRecord(szSmsTo, 2, 1);
		//	}
		//	else
		//	{
		//		CAlertSendObj::InsertRecord(szSmsTo, 2, 0);
		//	}
		//#endif

		if(GetCgiVersion().compare("IDC") == 0)
		{
			if(bSucess)
			{
				CAlertSendObj::InsertRecord(szSmsTo, strIdcId, 2, 1);
			}
			else
			{
				CAlertSendObj::InsertRecord(szSmsTo, strIdcId, 2, 0);
			}
		}
		else
		{
			if(bSucess)
			{
				CAlertSendObj::InsertRecord(szSmsTo, 2, 1);
			}
			else
			{
				CAlertSendObj::InsertRecord(szSmsTo, 2, 0);
			}
		}
		//////////////////////modify end at 07/07/31 //////////////////////////////////
	}
	catch(...)
	{
		//////////////////////begin to modify at 07/07/31 /////////////////////////////
		//#ifdef IDC_Version		
		//	CAlertSendObj::InsertRecord(szSmsTo, strIdcId, 2, 0);
		//#else
		//	CAlertSendObj::InsertRecord(szSmsTo, 2, 0);
		//#endif

		if(GetCgiVersion().compare("IDC") == 0)
		{
			CAlertSendObj::InsertRecord(szSmsTo, strIdcId, 2, 0);
		}
		else
		{
			CAlertSendObj::InsertRecord(szSmsTo, 2, 0);
		}
		//////////////////////modify end at 07/07/31 //////////////////////////////////

	}

	return bSucess;
}

//
string CAlertSmsSendObj::GetDebugInfo()
{
	string strDebugInfo = "";
	strDebugInfo += "\r\n------------------发送事件信息开始-----------------------------\r\n";
	strDebugInfo += ("报警监测器Id：" + strAlertMonitorId + "\r\n");
	strDebugInfo += ("发送事件类型：　Sms　\r\n");
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nSendId);
	string strCount = chItem;
	sprintf(chItem, "%d", nEventCount);
	string strEventCount = chItem;
	sprintf(chItem, "%d", nEventType);
	string strEventType = chItem;
	strDebugInfo += ("发送事件序号：" + strCount + "\r\n");
	strDebugInfo += ("原始事件序号：" + strEventCount + "\r\n");
	strDebugInfo += ("原始事件类型：" + strEventType + "\r\n");
	strDebugInfo += ("产生发送事件的报警：" + strAlertName + "\r\n");
	strDebugInfo += ("IDCUSERID：" + strIdcId + "\r\n");
	strDebugInfo += "------------------发送事件信息结束------------------------------\r\n";
	return strDebugInfo;
}

//生成报警标题
void CAlertSmsSendObj::MakeAlertTitle()
{
	strAlertTitle += CAlertMain::GetMonitorTitle(strAlertMonitorId);
	strAlertTitle += "-";
	strAlertTitle += CAlertMain::GetDeviceTitle(strAlertMonitorId);
}



//=========================================================================
// 增加代码：获取报警信息
// 增加人：邹晓
// 增加时间：2009.03.05
//=========================================================================
void GetAlertCondition( string strMonitorID, int nAlertStat, bool bTrue, list<string>& listDescription )
{
	OBJECT objMonitor = GetMonitor( strMonitorID );
	if(objMonitor != INVALID_VALUE )
    {
		MAPNODE node;

		if( nAlertStat == 2 )
		{
			node = GetMonitorWarningAlertCondition( objMonitor );
		}
		else
		{
			node = GetMonitorErrorAlertCondition( objMonitor );
		}

        if( node != INVALID_VALUE )
        {
			string strConditionCount = "";
			FindNodeValue( node, "sv_conditioncount", strConditionCount );
			int nCount(0);
			nCount = atoi( strConditionCount.c_str() );

			if( nCount == 0 )
			{
				CloseMonitor(objMonitor);
				return;
			}
			else
			{
				list<string> listCondition;
				string strCondition;
				char strParamName[32] = {0};

				for( int i=0; i!=nCount; i++ )
				{
					sprintf( strParamName, "sv_paramname%d", i+1 ); 
					FindNodeValue( node, strParamName, strCondition );
					listCondition.push_back( strCondition );
				}

				if( !listCondition.empty() )
				{
					map< string, string > mapReturn;

					MAPNODE mainNode = GetMonitorMainAttribNode( objMonitor );
					if( mainNode != INVALID_VALUE )
					{
						string strMTID;
						FindNodeValue( mainNode, "sv_monitortype", strMTID );

						OBJECT objMT = GetMonitorTemplet( atoi(strMTID.c_str()) );
						if( objMT != INVALID_VALUE )
						{
							LISTITEM item;
							FindMTReturnFirst( objMT, item );
							
							MAPNODE returnNode;
							string strReturnName;
							string strReturnLabel;
							while( ( returnNode = FindNext(item) ) != INVALID_VALUE )
							{
								FindNodeValue( returnNode, "sv_name", strReturnName );
								FindNodeValue( returnNode, "sv_label", strReturnLabel );						
								mapReturn[strReturnName] = strReturnLabel;
							}

							CloseMonitorTemplet(objMT);

						}

					}

					if( !mapReturn.empty() )
					{
						map< string, string >::iterator itMap;
						list<string> listAlert;
						list<string> listUnAlert;

						for( list<string>::iterator it=listCondition.begin(); it!=listCondition.end(); it++ )
						{
							if( ( itMap = mapReturn.find(*it) ) != mapReturn.end() )
							{
								listAlert.push_back( itMap->second );
								mapReturn.erase( itMap );
							}
								
						}

						for( itMap=mapReturn.begin(); itMap!=mapReturn.end(); itMap++ )
						{
							listUnAlert.push_back( itMap->second );
						}

						if( bTrue )
						{
							listDescription = listAlert;
						}
						else
						{
							listDescription = listUnAlert;
						}
					}					
				}
			}
		}

		CloseMonitor(objMonitor);
	}

}


//生成报警内容
void CAlertSmsSendObj::MakeAlertContent()
{
	string strTmp;
	strAlertContent = GetIniFileString("SMS", strSmsTemplateValue, "", "TxtTemplate.Ini");
	//strAlertContent = GetTemplateContent("SMS", strSmsTemplateValue);

	int nLength = strAlertContent.length();
	
	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Group@", CAlertMain::GetDeviceTitle(strAlertMonitorId));
	strAlertContent = strTmp;
	
	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@monitor@", CAlertMain::GetMonitorTitle(strAlertMonitorId));
	strAlertContent = strTmp;

	string strDeviceId = FindParentID(strAlertMonitorId);
	//string strGrouptId = FindParentID(strDeviceId);
	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@AllGroup@", CAlertMain::GetAllGroupTitle(strDeviceId));
	strAlertContent = strTmp;

	//strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Status@", CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_stateString"));

	//--------------------------------------------------------------------------------
	// 修改内容：精简监测器状态信息，只输出报警条件信息
	// 修改人：邹晓
	// 修改时间：2009.03.12
	//--------------------------------------------------------------------------------
	string strStatus = strEventDes;

	basic_string<char>::size_type nBegin, nEnd;

	list<string> listDes;
	
	GetAlertCondition( strAlertMonitorId, nEventType, false, listDes );

	for( list<string>::iterator it=listDes.begin(); it!=listDes.end(); it++ )
	{
		if( (nBegin = strStatus.find(*it)) != basic_string<char>::npos &&
			(nEnd = strStatus.find( ",", nBegin )) != basic_string<char>::npos )
		{
			strStatus.erase( nBegin, nEnd+2-nBegin );
		}

	}

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Status@", strStatus);
	strAlertContent = strTmp;

	int nUnit = 0;
	sscanf(CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_frequencyUnit").c_str(), "%d", &nUnit);
	string strFreq;
	if(nUnit==60)
	{
		//strFreq = "监测频率:";
		//strFreq += CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_frequency");
		//strFreq += "分钟";
		strFreq = CAlertMain::strMontorFreq.c_str();
		strFreq += ":";
		strFreq += CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_frequency");
		strFreq += CAlertMain::strMinute.c_str();

	}
	else
	{
		//strFreq = "监测频率:";
		//strFreq += CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_frequency");
		//strFreq += "小时";

		strFreq = CAlertMain::strMontorFreq.c_str();
		strFreq += ":";
		strFreq += CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_frequency");
		strFreq += CAlertMain::strHour.c_str();
	}

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@frequency@", strFreq);
	strAlertContent = strTmp;


	//strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Time@",  CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_lastMeasurementTime"));
	//--------------------------------------------------------------------------------
	// 修改内容：精简监测器时间信息，去掉年
	// 修改人：邹晓
	// 修改时间：2009.03.12
	//--------------------------------------------------------------------------------
	string strShortTime = strTime;
	basic_string<char>::size_type nCount = strShortTime.find("-");
	strShortTime.erase( 0, nCount+1 );

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Time@",  strShortTime );
	strAlertContent = strTmp;

	//Begin
	//替换所有格式如下的 @@Key@@ ，得到Key，并得到相应的键值
	//printf("Log Start");
	CString strMfcTemp, strTempKey, strPath, strLocalContent;
	strLocalContent= CString(strAlertContent.c_str());
	CString strTempKeyValue = _T(""); 
	while(strLocalContent.Find("@",0)>0)
	{
		//printf("1");
		int nTemp = strLocalContent.Find("@",0);
		strMfcTemp = strLocalContent.Right(strLocalContent.GetLength() - nTemp - 1);
		//printf(strMfcTemp);
		strLocalContent = strLocalContent.Left(nTemp);
		//printf(strLocalContent);
		nTemp = strMfcTemp.Find("@",0);
		strTempKey = strMfcTemp.Left(nTemp);
		strMfcTemp = strTempKey;
		//printf(strMfcTemp);
		//strMfcTemp = strMfcTemp.Right(strMfcTemp.GetLength()-nTemp-1);
		//strTempKeyValue=FuncGetProfileStringBy(strSection,strTempKey,strGroupFile);
		//printf(strMfcTemp);
		if(strMfcTemp == "Log")
		{
			//printf("Entenr Log");
			//文件名称
			strPath.Format("%s\\%s\\%s.txt", GetSiteViewRootPath().c_str(), "data\\Temp", strAlertMonitorId.c_str());

			//printf(strPath);

			//取出该次报警对应的数据并替换@@Log@@
			//根据strTime判断， 如果最近一次时间大于。。。 如果最近一次时间小于。。。
			int nFileLength = 0;
			CFile* pFile = NULL;
			TRY	
			{
				pFile = new CFile(strPath, CFile::modeRead | CFile::shareDenyNone);
				nFileLength = pFile->GetLength();
			}
			CATCH(CFileException, pEx)	
			{
				if (pFile != NULL) 
				{
					pFile->Close();
					delete pFile;
				}

				return;
			}
			END_CATCH

			if (pFile != NULL) 
			{
				pFile->Close();
				delete pFile;
			}

			if (0 == nFileLength)
				return;

			CString strTotleContent = _T("");
			CString strLogContent = _T("");
			FILE * fp = fopen((LPCTSTR)strPath, "r");
			if (fp)
			{
				char * buffer = NULL;
				buffer = (char*)malloc(nFileLength + 1);
				if (buffer) 
				{
					memset(buffer, 0, nFileLength + 1);
					fread(buffer, sizeof(char), nFileLength + 1, fp);
					strTotleContent.Format("%s", buffer);	
					free(buffer);
				}
				fclose(fp);
			}
			
			if(strTotleContent != "")
			{
				int nStart = strTotleContent.ReverseFind(']');
			
				strLogContent = strTotleContent.Right(strTotleContent.GetLength() - nStart - 2);
				//printf(strLogContent);
				string strStdLogContent = strLogContent;

				strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Log@",  strStdLogContent);	
				strAlertContent = strTmp;
			}
		}

		//printf("Key %s Value is %s\n",strTempKey,strTempKeyValue);
		
		//if(!strTempKeyValue.IsEmpty())
		//{			
		//	strLocalContent=strLocalContent+strTempKeyValue+strTemp;
		//}
		//else
		//{
		//	strLocalContent=strLocalContent+"无设定"+strTemp;
		//}
	}
	//End
}

//
void CAlertSmsSendObj::SetUpgradeTrue()
{
	bUpgrade = true;
}

//获取Schedule和Phone信息
bool CAlertSmsSendObj::GetInfoFromSmsAddress(string strAddressName)
{
	bool bReturn = true;
	//emailAdress.ini
	std::list<string> keylist;
	std::list<string>::iterator keyitem;

	//////////////////////begin to modify at 07/07/31 /////////////////////////////
	//#ifdef IDC_Version
	if(GetCgiVersion().compare("IDC") == 0)
	{
	//////////////////////modify end at 07/07/31 //////////////////////////////////

		//获取地址列表
		if(GetIniFileSections(keylist, "smsphoneset.ini"))
		{
			//初始化地址列表。。。
			for(keyitem = keylist.begin(); keyitem != keylist.end(); keyitem ++)	
			{
				//printf((*keyitem).c_str());
				//printf(GetIniFileString((*keyitem), "Name", "" , "emailAdress.ini").c_str());
				if(strAddressName == GetIniFileString((*keyitem), "Name", "" , "smsphoneset.ini"))
				{
					//printf(" GetInfoFromEmailAddress szMailTo");
					if(GetIniFileString((*keyitem), "Status", "", "smsphoneset.ini") != "Yes")
						bReturn = false;
					szSmsTo = GetIniFileString((*keyitem), "Phone", "", "smsphoneset.ini");
					szSchedule = GetIniFileString((*keyitem), "Plan", "", "smsphoneset.ini");

					//
					strSmsTemplateValue = GetIniFileString((*keyitem), "Template", "", "smsphoneset.ini");
				}
			}
		}

	//////////////////////begin to modify at 07/07/31 /////////////////////////////
	//#else
	}
	else
	{
	//////////////////////modify end at 07/07/31 //////////////////////////////////

		//获取地址列表
		if(GetIniFileSections(keylist, "smsphoneset.ini", "localhost", strIdcId))
		{
			//初始化地址列表。。。
			for(keyitem = keylist.begin(); keyitem != keylist.end(); keyitem ++)	
			{
				//printf((*keyitem).c_str());
				//printf(GetIniFileString((*keyitem), "Name", "" , "emailAdress.ini").c_str());
				if(strAddressName == GetIniFileString((*keyitem), "Name", "" , "smsphoneset.ini", "localhost", strIdcId))
				{
					//printf(" GetInfoFromEmailAddress szMailTo");
					if(GetIniFileString((*keyitem), "Status", "", "smsphoneset.ini", "localhost", strIdcId) != "Yes")
						bReturn = false;
					szSmsTo = GetIniFileString((*keyitem), "Phone", "", "smsphoneset.ini", "localhost", strIdcId);
					szSchedule = GetIniFileString((*keyitem), "Plan", "", "smsphoneset.ini", "localhost", strIdcId);

					//
					strSmsTemplateValue = GetIniFileString((*keyitem), "Template", "", "smsphoneset.ini", "localhost", strIdcId);
				}
			}
		}

	//////////////////////begin to modify at 07/07/31 /////////////////////////////
	//#endif
	}
	//////////////////////modify end at 07/07/31 //////////////////////////////////

	return bReturn;
}

//通过串口方式发送短信
bool CAlertSmsSendObj::SendSmsFromCom()
{
	int nSMSMaxLength = GetIniFileInt( "SMSCommConfig", "SMSLength", 70, "smsconfig.ini" );

	CString strContent = "";
	//strContent += strAlertTitle.c_str();
	//strContent += "\n";
	//strContent += strAlertContent.c_str();
	
	//如果是多个手机号码用逗号分隔则 
	std::list<string> listSms;
	std::list<string>::iterator listSmsItem;
	
	CAlertMain::ParserToken(listSms, szSmsTo.c_str(), ",");
	bool bSucess = true;
	bool bAllSucess = true;
	for(listSmsItem = listSms.begin(); listSmsItem!=listSms.end(); listSmsItem++)
	{
		CString strSmsTo = (*listSmsItem).c_str();
	
		//如果是多个手机号码用逗号分隔则 
		std::list<string> listSmsContent;
		std::list<string>::iterator listSmsContentItem;		

		listSmsContent.push_back( strAlertContent );
		
		//CAlertMain::ParserToLength(listSmsContent, strAlertContent, 50);
		for(listSmsContentItem = listSmsContent.begin(); listSmsContentItem!=listSmsContent.end(); listSmsContentItem++)
		{
			//char szTemp[1024] = {0};
			//sprintf( szTemp, "开始发生信息：%s, 接受号码：%s", (*listSmsContentItem).c_str(), strSmsTo.GetBuffer() );
			//string strTemp = szTemp;
			//WriteLog( strTemp );

			strContent = (*listSmsContentItem).c_str();		
			//DebugePrint((*listSmsContentItem).c_str());
			//DebugePrint("SendSmsFromCom\r\n");
			if(CAlertMain::SendSmsFromComm(strSmsTo, strContent, nSMSMaxLength) == -1)
				bSucess = false;
			strContent = "";
		}
		
		//CAlertMain::SendSmsFromComm(strSmsTo, "This is com test1");
	}

	return bSucess;
}

//
//BOOL GetSourceHtml(char const * theUrl, char * retState) 
//{
//	CInternetSession session;
//	CInternetFile* file = NULL;
//	try
//	{
//		// 试着连接到指定URL
//		file = (CInternetFile*) session.OpenURL(theUrl); 
//	}
//	catch (...)
//	{
//		// 如果有错误的话，置内容为空
//		strcpy(retState, "error");
//		return FALSE;
//	}
//
//	if (file)
//	{
//		CString  somecode;
//
//		bool flagReplace = false;
//		int replaceNum = 0;
//		while (file->ReadString(somecode) != NULL) //如果采用LPTSTR类型，读取最大个数nMax置0，使它遇空字符时结束
//		{
//			strcpy(retState, somecode);
//		}
//
//	}
//	else
//	{
//		strcpy(retState, "error");
//		return FALSE;
//	}
//	return TRUE;
//}

//通过Web方式发送短信
bool CAlertSmsSendObj::SendSmsFromWeb()
{
	bool bRet = true;

	string User("test");
	string Pwd("testpwd123");


	User = GetIniFileString("SMSWebConfig", "User", "", "smsconfig.ini");
	Pwd = GetIniFileString("SMSWebConfig", "Pwd", "", "smsconfig.ini");


	Des mydes;
	char dechar[1024]={0};
	if(Pwd.size()>0)
	{
		mydes.Decrypt(Pwd.c_str(),dechar);
		Pwd = dechar;
	}

	//截去多于的空格等
	string strTrim = " \n\r";
	if (strAlertContent.empty())
	{
		return false;
	}
	strAlertContent.erase(0, strAlertContent.find_first_not_of(strTrim));
	strAlertContent.erase(strAlertContent.find_last_not_of(strTrim) + 1);

	std::list<string> listSms;
	std::list<string>::iterator listSmsItem;

	// web方式不对短信作分解，由web平台自己执行
	listSms.push_back( strAlertContent );

	/*
	CAnalyzer4_bstr_t Analyzer;
	_bstr_t strMsgs = strAlertContent.c_str();
	OutputDebugString(strAlertContent.c_str());

	int nMaxSMSLen = MAX_SMS_LENGTH;
	//char * buf = new char[300];

	// 对原始短信正文进行宽字符分段
	int nPage = Analyzer.Analyzer(strMsgs, nMaxSMSLen);
	sprintf(dechar, "npage = %d; len=%d", nPage, strAlertContent.length());
	OutputDebugString(dechar);
	for (int i = 0; i < nPage; i++)
	{
		char buf[300] = {0};
		_bstr_t strMsg = Analyzer.GetResult(i);
		strcpy(buf, strMsg);
		listSms.push_back(buf);
	}
	*/

	for(listSmsItem = listSms.begin(); listSmsItem!=listSms.end(); listSmsItem++)
	{
		char buf[1024] = {0};
		string url = urlEncoding(*listSmsItem);
		OutputDebugString((*listSmsItem).c_str());
		string sendUrl = "http://www.smshelper.com:8090/sendsms?user=" + User 
			+ "&pwd=" + Pwd 
			+ "&phone=" + szSmsTo
			+ "&extnum=YL"
			+ "&msg=" + url;
		OutputDebugString(sendUrl.c_str());
		GetSourceHtml(sendUrl.c_str(), buf);
		OutputDebugString(buf);
		string sRet = buf;
		string::size_type indexBeg,indexEnd;
		static const string::size_type npos = -1;
		indexBeg = sRet.find("smstotal=");
		if (indexBeg != npos)
		{
			indexEnd = sRet.find("&");
			string strNum = sRet.substr(indexBeg+strlen("smstotal="), indexEnd-(indexBeg+strlen("smstotal=")));
			OutputDebugString("sxc");
			OutputDebugString(strNum.c_str());
			OutputDebugString("sxc");
			int num = 0;
			num = atoi(strNum.c_str());
			if (!num)
			{
				bRet = false;
			}
		}
		else
		{
			bRet = false;
		}
	}

	return bRet;
}

////通过Web方式发送短信
//bool CAlertSmsSendObj::SendSmsFromWeb()
//{
//	if(CAlertMain::pSender == NULL)
//		return false;
//    bool bRet = true;
//	
//	string sret;
//
//	//////////////////////begin to modify at 07/07/31 /////////////////////////////
//
//	//#ifdef IDC_Version
//	//	string User = GetIniFileString("SMSWebConfig", "User", sret, "smsconfig.ini", "localhost", strIdcId);
//	//	string Pwd = GetIniFileString("SMSWebConfig", "Pwd", sret, "smsconfig.ini", "localhost", strIdcId);
//	//
//	//	Des mydes;
//	//	char dechar[1024]={0};
//	//	if(Pwd.size()>0)
//	//	{
//	//		mydes.Decrypt(Pwd.c_str(),dechar);
//	//		Pwd = dechar;
//	//	}
//	//#else
//	//	string User = GetIniFileString("SMSWebConfig", "User", sret, "smsconfig.ini");
//	//	string Pwd = GetIniFileString("SMSWebConfig", "Pwd", sret, "smsconfig.ini");
//	//
//	//	Des mydes;
//	//	char dechar[1024]={0};
//	//	if(Pwd.size()>0)
//	//	{
//	//		mydes.Decrypt(Pwd.c_str(),dechar);
//	//		Pwd = dechar;
//	//	}
//	//#endif
//
//	string User("");
//	string Pwd("");
//
//	if(GetCgiVersion().compare("IDC") == 0)
//	{
//		User = GetIniFileString("SMSWebConfig", "User", sret, "smsconfig.ini", "localhost", strIdcId);
//		Pwd = GetIniFileString("SMSWebConfig", "Pwd", sret, "smsconfig.ini", "localhost", strIdcId);
//	}
//	else
//	{
//		User = GetIniFileString("SMSWebConfig", "User", sret, "smsconfig.ini");
//		Pwd = GetIniFileString("SMSWebConfig", "Pwd", sret, "smsconfig.ini");
//	}
//
//	Des mydes;
//	char dechar[1024]={0};
//	if(Pwd.size()>0)
//	{
//		mydes.Decrypt(Pwd.c_str(),dechar);
//		Pwd = dechar;
//	}
//
//	//////////////////////modify end at 07/07/31 //////////////////////////////////
//
//
//	//string strSMS = "This is test!";
//	string strSMS = "";
//	//strSMS += strAlertTitle;
//	//strSMS += "\n";
//	strSMS += strAlertContent;
//
//	//如果是多个手机号码用逗号分隔则 
//	std::list<string> listSms;
//	std::list<string>::iterator listSmsItem;
//	
//	//CAlertMain::ParserToken(listSms, szSmsTo.c_str(), ",");
//	CAlertMain::ParserToLength(listSms, strSMS, 140);
//	
//
//	//::CoInitialize(NULL);
//	{
//		//HRESULT hr = S_OK;
//
//		//IUMSmSendPtr pSender = NULL;
//		//hr = pSender.CreateInstance("SMSend.UMSmSend");
//		//if( SUCCEEDED(hr) && (NULL != pSender) )
//		{
//
//			string strPhone = szSmsTo;
//
//			for(listSmsItem = listSms.begin(); listSmsItem!=listSms.end(); listSmsItem++)
//			{
//
//				// 构造短消息XML:
//				/*_bstr_t bstrSendSMSXML("<?xml version=\"1.0\" encoding=\"GB2312\"?> \
//				<message><CompAccount>companyName</CompAccount><password>companyPass</password><InternalUserName>demo</InternalUserName><MobileNumber>13910000000</MobileNumber></message>");
//				*/
//
//				char chSMSXML[1024] = {0};
//				sprintf(chSMSXML, "<?xml version=\"1.0\"?><message><EntCode>62016161</EntCode>" \
//					"<EntUserID>%s</EntUserID><password>%s</password><Content>%s</Content>" \
//					"<DestMobileNumber>%s</DestMobileNumber><URGENT_Flag>1</URGENT_Flag>" \
//					"<ScheduledTime></ScheduledTime><Batch_SendID></Batch_SendID><DataType>15" \
//					"</DataType><SrcNumber></SrcNumber></message>", User.c_str(), Pwd.c_str(), /*strSMS.c_str()*/(*listSmsItem).c_str(), 
//					strPhone.c_str());
//
//				//
//				//  公钥文件的物理位置:
//				//_bstr_t bstrPublicKeyPath("D:\\Program Files\\SMSIIGatewayAPI\\PublicKey\\pub.txt");
//				string strPath = "";
//				strPath = GetSiteViewRootPath() + "\\fcgi-bin\\pub.txt";		
//
//				_bstr_t bstrPublicKeyPath(strPath.c_str());
//				//
//				//  服务器端接收的ASP页面的URL
//				//_bstr_t bstrServerSiteURL("http://gateway.bjums.com/smssite/smsstart.asp");
//				_bstr_t bstrServerSiteURL("http://sms.bmcc.com.cn/GatewayAPI/SMSIIGateWay.asp");
//				//
//				///////////////////////////////////////////////////////////
//
//
//				///////////////////////////////////////////////////////////
//				//
//				// 设置组件接口所需要的参数
//				//
//				//  第一个:公钥文件的物理位置
//				//printf("公钥文件的物理位置: %s\n", (char*)bstrPublicKeyPath);
//				CAlertMain::pSender->SetPkpath(bstrPublicKeyPath);
//				//pSender->SetPkpath(bstrPublicKeyPath);
//				//
//				//  第二个:服务器端接收的ASP页面的URL
//				//printf("服务器端接收的ASP页面的URL: %s\n\n", (char*)bstrServerSiteURL);
//				CAlertMain::pSender->SetServerSite(bstrServerSiteURL);
//				//pSender->SetServerSite(bstrServerSiteURL);
//				//
//				///////////////////////////////////////////////////////////
//
//
//				///////////////////////////////////////////////////////////
//				//
//				// 实际发送短消息XML给服务器
//				//
//				// 第一个参数257代表采用RSA加密算法
//				// 第二个参数就是短消息XML
//
//				//printf("实际发送短消息XML给服务器: %s\n\n", (char*)bstrSendSMSXML);
//				//pSender->LoadSendXML("257",
//				//	                 bstrSendSMSXML);
//				//printf("实际发送短消息XML给服务器: %s\n\n", (char*)chSMSXML);
//				CAlertMain::pSender->LoadSendXML("257", 
//					chSMSXML);
//				printf("实际发送短消息XML给服务器: %s\n\n", (char*)chSMSXML);
//				//pSender->LoadSendXML("257", 
//				//	chSMSXML);
//
//				//
//				///////////////////////////////////////////////////////////
//
//
//				///////////////////////////////////////////////////////////
//				//
//				// 发送短消息XML之后，可以通过以下两个方法得到服务器方的反馈
//				//  1:GetResponseText(通过这个方法可以取到服务器端返回的responseText)
//				//  2:GetHTTPPostStatus(通过这个方法可以取到服务器端返回的状态值,即200、404、500之类的值)
//				//
//				// 第一个参数257代表采用RSA加密算法
//				// 第二个参数就是短消息XML
//				_bstr_t bstrServerResponseText = CAlertMain::pSender->GetResponseText();
//				//_bstr_t bstrServerResponseText = pSender->GetResponseText();
//				printf("服务器端返回的responseText: %s\n", (char*)bstrServerResponseText);
//				//
//				long    lServerResponseStatus  = CAlertMain::pSender->GetHTTPPostStatus();
//				//long    lServerResponseStatus  = pSender->GetHTTPPostStatus();
//				printf("服务器端返回的状态值: %d\n", lServerResponseStatus);
//				char buf[256];
//				char buf1[256];
//				memset(buf, 0, 256);
//				memset(buf1, 0, 256);
//				sprintf(buf, "服务器端返回的responseText: %s\n", (char*)bstrServerResponseText);
//				sprintf(buf1, "服务器端返回的状态值:%d\n", lServerResponseStatus);
//				strcat(buf, buf1);
//
//				std::string sRet = buf;
//				int bFind = sRet.find("提交成功", 0);
//
//				if(bFind >= 0)
//				{
//					//成功
//					//bRet = true;
//				}
//				else
//				{
//					//失败
//					bRet = false;
//				}
//			}
//		}
//	}
//	//::CoUninitialize();
//	
//	return bRet;
//	//return bRet;
//}
//
//int CAlertSmsSendObj::GetResponseCode(_bstr_t bstrResponse)
//{
//	char chResponse[1024], chTmp[1024] ;
//	sprintf(chResponse, "%s", (char*)bstrResponse);
//
//	strlwr(chResponse);
//	char *chTemp = strstr(chResponse, "<errornumber>");
//	if(chTemp)
//	{
//		sprintf(chTmp, "%s", chTemp + strlen("<errornumber>"));
//		chTemp = strstr(chTmp, "</errornumber>");
//		if(chTemp)
//		{
//			int nCode = strlen(chTmp) - strlen(chTemp);
//			memset(chResponse, 0, strlen(chResponse));
//			strncpy(chResponse, chTmp, nCode);
//			chResponse[nCode] = '\0';
//			return atoi(chResponse);
//		}
//		else
//		{
//			return -2;
//		}
//	}
//	else
//	{
//		return -1;
//	}
//}
//
////
//BSTR CAlertSmsSendObj::UTF2GB(LPCSTR lp, int nLen)
//{
//	BSTR str = NULL;
//	int nConvertedLen = MultiByteToWideChar(CP_UTF8, 0, lp,
//		nLen, NULL, NULL);
//
//	// BUG FIX #1 (from Q241857): only subtract 1 from 
//	// the length if the source data is nul-terminated
//	if (nLen == -1)
//		nConvertedLen--;
//
//	str = ::SysAllocStringLen(NULL, nConvertedLen);
//	if (str != NULL)
//	{
//		MultiByteToWideChar(CP_UTF8, 0, lp, nLen, str, nConvertedLen);
//	}
//	return str;
//}

//
bool CAlertSmsSendObj::SendSmsFromSelfDefine()
{
	OutputDebugString("----------------SendSmsFromSelfDefine() call success--------------\n");	
	string strDllName, strDllFuncParam;

	strDllName = strSmsSendMode;
	strDllFuncParam = GetIniFileString(strDllName, "DllFunParam", "",  "interfacedll.ini");
	OutputDebugString("----------------SendSmsFromSelfDefine() call success DllName--------------\n");	
	OutputDebugString(strDllName.c_str());
	OutputDebugString("----------------SendSmsFromSelfDefine() call success DllFuncParam--------------\n");	
	OutputDebugString(strDllFuncParam.c_str());
	
	//if((strDllName == "") || (strDllFuncParam == ""))
	if((strDllName == ""))
	{
		return false;
	}

	//if(strSmsSendMode == "Dial")
	//{
	//	strDllName = GetIniFileString("SMSDllConfig", "DllName", "",  "dialconfig.ini");
	//	strDllFuncParam = GetIniFileString("SMSDllConfig", "DllFunParam", "", "dialconfig.ini");
	//}
	//else
	//{
	//	strDllName = GetIniFileString("SMSDllConfig", "DllName", "",  "smsconfig.ini");
	//	strDllFuncParam = GetIniFileString("SMSDllConfig", "DllFunParam", "", "smsconfig.ini");	
	//}

	int pos = 0;

	pos = strDllName.find("(", 0);
	strDllName = strDllName.substr(0, pos);

	//如果是电话报警需要重新组织参数：（只支持用其他来播放多语音源文件）
	//	1、szSmsTo如果没有输入用逗号分隔的形式如：端口 语音文件路径;电话号码 则用原有的strDllFuncParam和szSmsTo做动态库的输入参数。
	//	2、szSmsTo如果形式如端口 语音文件路径;电话号码 则从szSmsTo分解出strDllFuncParam和szSmsTo做动态库的输入参数。
	//	3、电话报警暂时可能不支持用逗号分隔多个号码的情形。
	
	if(strSmsSendMode == "TelephoneDll.dll")
	{
		if(szSmsTo.find(";") != -1)
		{
			std::list<string> listTelparam;
			CAlertMain::ParserToken(listTelparam, szSmsTo.c_str(), ";");
			strDllFuncParam = listTelparam.front();
			szSmsTo = listTelparam.back();
			DebugePrint("------------------- TelephoneDll strDllFuncParam ----------------\n");
			DebugePrint(strDllFuncParam.c_str());
			DebugePrint("------------------- TelephoneDll szSmsTo ----------------\n");
			DebugePrint(szSmsTo.c_str());
		}
	}
	
	//string param[3];
	//for(int i = 0; i < 3; i++)
	//{
	//	param[i] = "";
	//}

	//list<string> paramlist;
	//list<string>::iterator paramitem;
	//pos = 0;
	//int pos1 = strDllFuncParam.find(" ", pos);
	//while(pos1 >= 0)
	//{		
	//	string temp = strDllFuncParam.substr(pos, pos1 - pos);
	//	paramlist.push_back(temp);
	//	pos = pos1 + 1;
	//	pos1 = strDllFuncParam.find(" ", pos);		
	//}
	//string temp1 = strDllFuncParam.substr(pos , strDllFuncParam.size() - pos);
	//paramlist.push_back(temp1);

	//int iparam = 0;
	//for(paramitem = paramlist.begin(); paramitem != paramlist.end(); paramitem++)
	//{
	//	param[iparam] = *paramitem;
	//	iparam ++;
	//}

	typedef int (* smssend)(char *, char *, char *);
	smssend smssend1;

	std::string szRootPath =GetSiteViewRootPath();
	char path[256];
	
	strcpy(path, szRootPath.c_str());
	strcat(path, "\\smsplugin\\");
	strcat(path , strDllName.c_str());

	OutputDebugString("-------------start loadlibrary self smsdll------------------\n");
	OutputDebugString(path);
	OutputDebugString("\n");

	HMODULE hMod = LoadLibrary(path);
	if(hMod != 0)
	{
		smssend1 = (smssend)GetProcAddress(hMod, "run");
		if(smssend1 != 0)
		{
			CString strContent = "";			
			
			//如果是多个手机号码用逗号分隔则 
			std::list<string> listSms;
			std::list<string>::iterator listSmsItem;
			
			CAlertMain::ParserToken(listSms, szSmsTo.c_str(), ",");
			bool bSucess = true;
			bool bAllSucess = true;
			for(listSmsItem = listSms.begin(); listSmsItem!=listSms.end(); listSmsItem++)
			{
				CString strSmsTo = (*listSmsItem).c_str();
			
				//如果是多个手机号码用逗号分隔则 
				std::list<string> listSmsContent;
				std::list<string>::iterator listSmsContentItem;		
				
				//DebugePrint("------------------- smssend1 input param strDllFuncParam ----------------\n");
				//DebugePrint(strDllFuncParam.c_str());
				//
				//OutputDebugString(strDllFuncParam.c_str());
				//
				//DebugePrint("------------------- smssend1  input param strSmsTo ----------------\n");
				//DebugePrint(strSmsTo.GetBuffer());
				//
				//OutputDebugString(strSmsTo.GetBuffer());
				//
				//DebugePrint("------------------- smssend1  input param strAlertContent ----------------\n");
				//DebugePrint(strAlertContent.c_str());
				//
				//OutputDebugString(strAlertContent.c_str());

				smssend1((char*)strDllFuncParam.c_str(), strSmsTo.GetBuffer(), (char*)strAlertContent.c_str());
				
				//CAlertMain::SendSmsFromComm(strSmsTo, "This is com test1");
			}			
		}
	}

	if(hMod != 0)
	{
		FreeLibrary(hMod);
	}

	return true;
}
//
int CAlertSmsSendObj::GetResponseCode(_bstr_t bstrResponse)
{
    char chResponse[1024], chTmp[1024] ;
    sprintf(chResponse, "%s", (char*)bstrResponse);

    strlwr(chResponse);
    char *chTemp = strstr(chResponse, "<errornumber>");
    if(chTemp)
    {
        sprintf(chTmp, "%s", chTemp + strlen("<errornumber>"));
        chTemp = strstr(chTmp, "</errornumber>");
        if(chTemp)
        {
            int nCode = strlen(chTmp) - strlen(chTemp);
            memset(chResponse, 0, strlen(chResponse));
            strncpy(chResponse, chTmp, nCode);
            chResponse[nCode] = '\0';
            return atoi(chResponse);
        }
        else
        {
            return -2;
        }
    }
    else
    {
        return -1;
    }
}

//
BSTR CAlertSmsSendObj::UTF2GB(LPCSTR lp, int nLen)
{
   BSTR str = NULL;
   int nConvertedLen = MultiByteToWideChar(CP_UTF8, 0, lp,
     nLen, NULL, NULL);
 
   // BUG FIX #1 (from Q241857): only subtract 1 from 
   // the length if the source data is nul-terminated
   if (nLen == -1)
      nConvertedLen--;
 
   str = ::SysAllocStringLen(NULL, nConvertedLen);
   if (str != NULL)
   {
     MultiByteToWideChar(CP_UTF8, 0, lp, nLen, str, nConvertedLen);
   }
   return str;
}


//
CAlertScriptSendObj::CAlertScriptSendObj()
{
	nType = 3;	
}

//
string CAlertScriptSendObj::GetDebugInfo()
{
	string strDebugInfo = "";
	strDebugInfo += "\r\n------------------发送事件信息开始-----------------------------\r\n";
	strDebugInfo += ("报警监测器Id：" + strAlertMonitorId + "\r\n");
	strDebugInfo += ("发送事件类型：　Script　\r\n");
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nSendId);
	string strCount = chItem;
	sprintf(chItem, "%d", nEventCount);
	string strEventCount = chItem;
	sprintf(chItem, "%d", nEventType);
	string strEventType = chItem;
	strDebugInfo += ("发送事件序号：" + strCount + "\r\n");
	strDebugInfo += ("原始事件序号：" + strEventCount + "\r\n");
	strDebugInfo += ("原始事件类型：" + strEventType + "\r\n");
	strDebugInfo += ("产生发送事件的报警：" + strAlertName + "\r\n");
	strDebugInfo += ("IDCUSERID：" + strIdcId + "\r\n");
	strDebugInfo += "------------------发送事件信息结束------------------------------\r\n";
	return strDebugInfo;
}

//
bool CAlertScriptSendObj::SendAlert()
{
	string strDebugInfo = "";
	strDebugInfo += "\r\n------------------发送脚本报警开始-----------------------------\r\n";

	bool bSucess = false;
	try
	{
		if(strServerTextValue.find("_win") != -1 || strServerTextValue == "127.0.0.1")
		{
			GetUserInfoFromServer();
			string strTmp = strServerTextValue.substr(0, strServerTextValue.length() - 6);
			strServerTextValue = strTmp;

			CString strReturn = DoWinScript();
			if(strReturn == "ok")
				bSucess = true;
		}
		else
		{		
			bSucess = DoUnixScript();
		}

		//////////////////////begin to modify at 07/07/31 /////////////////////////////
		//#ifdef IDC_Version			
		//		if(bSucess)
		//		{
		//			if(strScriptServerId == "127.0.0.1")
		//			{
		//				CAlertSendObj::InsertRecord(strScriptServerId, strIdcId, 3, 1);
		//			}
		//			else
		//			{
		//				CAlertSendObj::InsertRecord(strServerTextValue, strIdcId, 3, 1);
		//			}
		//		}
		//		else
		//		{
		//			if(strScriptServerId == "127.0.0.1")
		//			{
		//				CAlertSendObj::InsertRecord(strScriptServerId, strIdcId, 3, 0);
		//			}
		//			else
		//			{
		//				CAlertSendObj::InsertRecord(strServerTextValue, strIdcId, 3, 0);	
		//			}
		//		}
		//	}
		//	catch(...)
		//	{
		//		CAlertSendObj::InsertRecord(strServerTextValue, strIdcId, 3, 0);
		//	}
		//
		//#else
		//		if(bSucess)
		//		{
		//			if(strScriptServerId == "127.0.0.1")
		//			{
		//				CAlertSendObj::InsertRecord(strScriptServerId, 3, 1);
		//			}
		//			else
		//			{
		//				CAlertSendObj::InsertRecord(strServerTextValue, 3, 1);
		//			}
		//		}
		//		else
		//		{
		//			if(strScriptServerId == "127.0.0.1")
		//			{
		//				CAlertSendObj::InsertRecord(strScriptServerId, 3, 0);
		//			}
		//			else
		//			{
		//				CAlertSendObj::InsertRecord(strServerTextValue, 3, 0);	
		//			}
		//		}
		//	}
		//	catch(...)
		//	{
		//		CAlertSendObj::InsertRecord(strServerTextValue, 3, 0);
		//	}
		//#endif

		if(GetCgiVersion().compare("IDC") == 0)
		{
			if(bSucess)
			{
				if(strScriptServerId == "127.0.0.1")
				{
					CAlertSendObj::InsertRecord(strScriptServerId, strIdcId, 3, 1);
				}
				else
				{
					CAlertSendObj::InsertRecord(strServerTextValue, strIdcId, 3, 1);
				}
			}
			else
			{
				if(strScriptServerId == "127.0.0.1")
				{
					CAlertSendObj::InsertRecord(strScriptServerId, strIdcId, 3, 0);
				}
				else
				{
					CAlertSendObj::InsertRecord(strServerTextValue, strIdcId, 3, 0);	
				}
			}
		}
		else
		{
			if(bSucess)
			{
				if(strScriptServerId == "127.0.0.1")
				{
					CAlertSendObj::InsertRecord(strScriptServerId, 3, 1);
				}
				else
				{
					CAlertSendObj::InsertRecord(strServerTextValue, 3, 1);
				}
			}
			else
			{
				if(strScriptServerId == "127.0.0.1")
				{
					CAlertSendObj::InsertRecord(strScriptServerId, 3, 0);
				}
				else
				{
					CAlertSendObj::InsertRecord(strServerTextValue, 3, 0);	
				}
			}
		}

	}
	catch(...)
	{
		if(GetCgiVersion().compare("IDC") == 0)
		{
			CAlertSendObj::InsertRecord(strServerTextValue, strIdcId, 3, 0);
		}
		else
		{
			CAlertSendObj::InsertRecord(strServerTextValue, 3, 0);
		}
	}
	//////////////////////modify end at 07/07/31 //////////////////////////////////

	strDebugInfo += ("报警监测器Id：" + strAlertMonitorId + "\r\n");
	strDebugInfo += ("发送事件类型：　Script　\r\n");
	strDebugInfo += ("strServerTextValue：" + strServerTextValue + "\r\n");
	strDebugInfo += ("strScriptServerId：" + strScriptServerId + "\r\n");
	strDebugInfo += ("strScriptFileValue：" + strScriptFileValue + "\r\n");
	strDebugInfo += ("strScriptParamValue：" + strScriptParamValue + "\r\n");
	strDebugInfo += ("strUserName：" + strUserName + "\r\n");
	strDebugInfo += ("strUserPwd：" + strUserPwd + "\r\n");
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nSendId);
	string strCount = chItem;
	sprintf(chItem, "%d", nEventCount);
	string strEventCount = chItem;
	sprintf(chItem, "%d", nEventType);
	string strEventType = chItem;
	strDebugInfo += ("发送事件序号：" + strCount + "\r\n");
	strDebugInfo += ("原始事件序号：" + strEventCount + "\r\n");
	strDebugInfo += ("原始事件类型：" + strEventType + "\r\n");
	strDebugInfo += ("IDCUSERID：" + strIdcId + "\r\n");
	strDebugInfo += "------------------发送脚本报警结束------------------------------\r\n";
	DebugePrint(strDebugInfo);

	return true;
}

//获取Windows服务器的用户名称和密码
void CAlertScriptSendObj::GetUserInfoFromServer()
{
	if(strScriptServerId == "127.0.0.1")
	{
		strUserName = "";
		strUserPwd = "";
		strServerTextValue = "";
		return;
	}

	//..	
    string m_szQuery = "";
    //OBJECT objDevice = GetEntity(m_szDeviceIndex, m_szIDCUser, m_szIDCPwd);
	OBJECT objDevice = GetEntity(strScriptServerId);
    if(objDevice != INVALID_VALUE)
    {
        MAPNODE mainnode = GetEntityMainAttribNode(objDevice);
        if(mainnode != INVALID_VALUE)
        {
            string szValue = "";
            FindNodeValue(mainnode, "_UserAccount", szValue);
			strUserName = szValue;

			szValue = "";
            FindNodeValue(mainnode, "_PassWord", szValue);
			char szOutput[512] = {0};
            Des des;
            if(des.Decrypt(szValue.c_str(), szOutput))
			{
                szValue = szOutput;
				strUserPwd = szValue;
			}
        }
        CloseEntity(objDevice);
    }
}

//执行Windows脚本
CString CAlertScriptSendObj::DoWinScript()
{
	CString strToServer = _T(""),
			strLast = _T("");

	//CoInitialize(NULL);
	//_Alert *myRef=NULL;
	//HRESULT hr=CoCreateInstance(CLSID_Alert,NULL,
	//						CLSCTX_ALL,
	//						IID__Alert,(void **)&myRef);
	//if(SUCCEEDED(hr))
	//{
	//	OutputDebugString("creat com success");
	//}
	//else 
	//{
	//	OutputDebugString("creat com failed");
	//}

	char  cToServer[56] = {0},
		  cLast[56] = {0};

	VARIANT  a,b,c,d,e, vResult;
	BSTR bstrTmp;
	VariantInit(&a);
	VariantInit(&b);
	VariantInit(&c);
	VariantInit(&d);
	VariantInit(&e);
	VariantInit(&vResult);

	char cParam [1024] = {0};
	strcpy(cParam, strScriptParamValue.c_str());
	
	CString strServer(strServerTextValue.c_str());	
	CString strUser(strUserName.c_str());
	CString strPwd(strUserPwd.c_str());
	
	OutputDebugString(strServerTextValue.c_str());
	OutputDebugString(strUserName.c_str());
	OutputDebugString(strUserPwd.c_str());

	//////////////////////begin to modify at 07/07/31 /////////////////////////////
	//#ifdef IDC_Version
	//	string strTmp = GetIniFileString("Scripts", strScriptFileValue, "",  "TxtTemplate.Ini", "localhost", strIdcId);
	//#else
	//	string strTmp = GetIniFileString("Scripts", strScriptFileValue, "",  "TxtTemplate.Ini");
	//#endif

	string strTmp("");
	if(GetCgiVersion().compare("IDC") == 0)
	{
		strTmp = GetIniFileString("Scripts", strScriptFileValue, "",  "TxtTemplate.Ini", "localhost", strIdcId);
	}
	else
	{
		strTmp = GetIniFileString("Scripts", strScriptFileValue, "",  "TxtTemplate.Ini");
	}
	//////////////////////modify end at 07/07/31 //////////////////////////////////

	DebugePrint(strTmp);
	DebugePrint("DoWinScript\r\n");

	CString strScriptFile(strTmp.c_str());
	CString strScriptParam(strScriptParamValue.c_str());
	
	
	bstrTmp=strServer.AllocSysString();
	a.vt=VT_BSTR;
	a.bstrVal=bstrTmp;
	
	bstrTmp=strUser.AllocSysString();
	b.bstrVal=bstrTmp;
	b.vt=VT_BSTR;
	
	bstrTmp=strPwd.AllocSysString();
	c.vt=VT_BSTR;
	c.bstrVal=bstrTmp;

	CString strReturn = _T("");
	int nParamCount = 0;

	//if(SUCCEEDED(hr))
	{
		if(strScriptFile == "SendMessage.vbs")
		{
			DebugePrint("SendMessage.vbs\r\n");

			nParamCount = sscanf(cParam, "%s %s", cToServer, cLast);
			if(nParamCount == 2)
			{
				CString strToServer, strLast;

				strToServer.Format("%s", cToServer);
				OutputDebugString(cToServer);
				OutputDebugString(cLast);
				
				strLast.Format("%s", cLast);
						
				bstrTmp=strToServer.AllocSysString();
				d.bstrVal=bstrTmp;
				d.vt=VT_BSTR;
				
				//strLast = strScriptParam.Right(strScriptParam.GetLength() - strToServer.GetLength() - 1);
				bstrTmp=strLast.AllocSysString();
				e.vt=VT_BSTR;
				e.bstrVal=bstrTmp;
				
				CAlertMain::myScriptRef->alertSendMessage(&a,&b,&c,&d,&e,&vResult);
				//myRef->alertSendMessage(&a,&b,&c,&d,&e,&vResult);
				CString strResult(vResult.bstrVal);
	
				strReturn = strResult;
				OutputDebugString(strReturn);
				//myRef->Release();
				//myRef = NULL;
			}
		}
		else if(strScriptFile == "Reboot.vbs")
		{
			DebugePrint("Reboot.vbs\r\n");
			BOOL nReturn = FALSE;

			CAlertMain::myScriptRef->alertReboot(&a,&b,&c,&vResult);
			//myRef->alertReboot(&a,&b,&c,&vResult);
			CString strResult(vResult.bstrVal);
			strReturn = strResult;
			//myRef->Release();
			//myRef = NULL;	
		}
		else if(strScriptFile == "RestartIIS.vbs")
		{
			DebugePrint("RestartIIS.vbs\r\n");
			CAlertMain::myScriptRef->alertRestartIIS(&a,&b,&c,&vResult);
			//myRef->alertRestartIIS(&a,&b,&c,&vResult);
			CString strResult(vResult.bstrVal);

			strReturn = strResult;
			//myRef->Release();
			//myRef = NULL;
		}
		else if(strScriptFile == "RestartService.vbs")
		{
			DebugePrint("RestartService.vbs\r\n");
			bstrTmp=strScriptParam.AllocSysString();
			d.bstrVal=bstrTmp;
			d.vt=VT_BSTR;
				
			CAlertMain::myScriptRef->alertRestartService(&a,&b,&c,&d,&vResult);
			//myRef->alertRestartService(&a,&b,&c,&d,&vResult);
			CString strResult(vResult.bstrVal);
				
			strReturn = strResult;
			//myRef->Release();
			//myRef = NULL;
		}
		else if(strScriptFile == "Shutdown.vbs")
		{
			DebugePrint("Shutdown.vbs\r\n");
			CAlertMain::myScriptRef->alertShutdown(&a,&b,&c,&vResult);
			//myRef->alertShutdown(&a,&b,&c,&vResult);
			CString strResult(vResult.bstrVal);
			
			strReturn = strResult;
			//myRef->Release();
			//myRef = NULL;
		}
		else if (strScriptFile == "PlayRemoteSound.vbs")
		{
			DebugePrint("PlayRemoteSound.vbs\r\n");
			strToServer = ::FuncGetLeftStringByMark(strScriptParam, "\" \"", 1);
			strToServer = strToServer + "\"";
			strToServer.Replace("\"", "");
			strLast = ::FuncGetRightString(strScriptParam, "\" \"");
			strLast = "\"" + strLast;
			strLast.Replace("\"", "");

			OutputDebugString(strToServer);
			OutputDebugString(strLast);
			
			bstrTmp=strToServer.AllocSysString();
			d.bstrVal=bstrTmp;
			d.vt=VT_BSTR;
			
			/*strLast = strScriptParam.Right(strScriptParam.GetLength() - strToServer.GetLength() - 1);*/
			bstrTmp=strLast.AllocSysString();
			e.vt=VT_BSTR;
			e.bstrVal=bstrTmp;
			
			CAlertMain::myScriptRef->alertPlaysound(&a,&b,&c,&d,&e,&vResult);
			CString strResult(vResult.bstrVal);

			OutputDebugString(strResult);

			strReturn = strResult;
			//myRef->Release();
			//myRef = NULL;
		}
		else
		{
			//其他脚本．．．
			CString strSpt=_T("");
			CString strInstallPath = _T("");
			strInstallPath = FuncGetInstallPath();
			
			strSpt.Format("cscript %s\\scripts\\%s %s",strInstallPath,strScriptFile,strScriptParam);
			OutputDebugString(strSpt);
			::WinExec(strSpt, SW_HIDE);
			strReturn="ok";
		}

		VariantClear(&a);
		VariantClear(&b);
		VariantClear(&c);
		VariantClear(&d);
		VariantClear(&e);
		VariantClear(&vResult);

	}

	//CoUninitialize();
	return strReturn;
}

//获取设备运行时参数
string GetDeviceRunParam(string m_szDeviceIndex)
{
    string m_szQuery = "";
    //OBJECT objDevice = GetEntity(m_szDeviceIndex, m_szIDCUser, m_szIDCPwd);
	OBJECT objDevice = GetEntity(m_szDeviceIndex);
    if(objDevice != INVALID_VALUE)
    {
        MAPNODE mainnode = GetEntityMainAttribNode(objDevice);
        if(mainnode != INVALID_VALUE)
        {
            string szDeviceType = "";
            //list<string> lsDeviceParam;
            map<string, string, less<string> > lsDeviceParam;
            if(FindNodeValue(mainnode, "sv_devicetype", szDeviceType))
            {
                //OBJECT objDevice = GetEntityTemplet(szDeviceType, m_szIDCUser, m_szIDCPwd);
				OBJECT objDevice = GetEntityTemplet(szDeviceType);
                if(objDevice != INVALID_VALUE)
                {
                    LISTITEM lsItem;
                    if( FindETContrlFirst(objDevice, lsItem))
                    {
                        MAPNODE objNode;
                        while( (objNode = FindNext(lsItem)) != INVALID_VALUE )
                        {
                            string szName = "", szRun = "";
                            string szType = "";
                            FindNodeValue(objNode, "sv_name", szName);
                            FindNodeValue(objNode, "sv_run", szRun);
                            FindNodeValue(objNode, "sv_type", szType);
                            if(szRun == "true")
                                lsDeviceParam[szName] = szType;
                        }
                    }
                    CloseEntityTemplet(objDevice);
                }
            }
            //list<string>::iterator lstItem;
            map<string, string, less<string> >::iterator lstItem;
            for(lstItem = lsDeviceParam.begin(); lstItem != lsDeviceParam.end(); lstItem ++)
            {
                string szValue = "";
                FindNodeValue(mainnode, (lstItem->first), szValue);
                if((lstItem->second).compare(svPassword) == 0)
                {    
					char szOutput[512] = {0};
                    Des des;
                    if(des.Decrypt(szValue.c_str(), szOutput))
                        szValue = szOutput;
                }
                m_szQuery = m_szQuery + (lstItem->first) + "=" + szValue + "\v";
            }
        }
        CloseEntity(objDevice);
    }

	return m_szQuery;
}

//
void EidtQueryString(string &szQuery, char* pszQueryString)
{    
    if(pszQueryString)
    {
        strcpy(pszQueryString , szQuery.c_str());
        char *pPos = pszQueryString;
        while((*pPos) != '\0' )
        {
            if((*pPos) == '\v')
                (*pPos) = '\0';
            pPos ++;
        }
    }
}

//
void ParserReturnInList(const char * szReturn, list<string> &lsReturn)
{
    const char * pPos = szReturn;
    while(*pPos != '\0')
    {
        int nSize =strlen(pPos);
        lsReturn.push_back(pPos);
        pPos = pPos + nSize + 1;
    }
}

//执行Unix脚本
bool CAlertScriptSendObj::DoUnixScript()
{
	bool bReturn = false;

	string szQuery = GetDeviceRunParam(strScriptServerId);
	char szReturn [svBufferSize] = {0};
	int nSize = sizeof(szReturn);
	//int nSize = szQuery.length();
	char *pszQueryString = new char[szQuery.length()];

	//再加 "_Script=" + strScriptFile + " " + strScriptPara...

	if(pszQueryString)
	{					
		OutputDebugString(szQuery.c_str());
		EidtQueryString(szQuery, pszQueryString);
		OutputDebugString("SelSeverChanged");
		OutputDebugString(pszQueryString);
		if(CAlertMain::pExcuteScript(pszQueryString, szReturn, nSize))
		{						
			list<string> lsReturn;
			std::list<string>::iterator lsReturnItem;
			ParserReturnInList(szReturn, lsReturn);
			
			OutputDebugString(szReturn);

		}
		
		delete []pszQueryString;
	}

	return bReturn;
}

//
CAlertSoundSendObj::CAlertSoundSendObj()
{
	nType = 4;
}

//
CAlertSoundSendObj::~CAlertSoundSendObj()
{

}

//
string CAlertSoundSendObj::GetDebugInfo()
{
	string strDebugInfo = "";
	strDebugInfo += "\r\n------------------发送事件信息开始-----------------------------\r\n";
	strDebugInfo += ("报警监测器Id：" + strAlertMonitorId + "\r\n");
	strDebugInfo += ("发送事件类型：　Sound　\r\n");
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nSendId);
	string strCount = chItem;
	sprintf(chItem, "%d", nEventCount);
	string strEventCount = chItem;
	sprintf(chItem, "%d", nEventType);
	string strEventType = chItem;
	strDebugInfo += ("发送事件序号：" + strCount + "\r\n");
	strDebugInfo += ("原始事件序号：" + strEventCount + "\r\n");
	strDebugInfo += ("原始事件类型：" + strEventType + "\r\n");
	strDebugInfo += ("产生发送事件的报警：" + strAlertName + "\r\n");
	strDebugInfo += "------------------发送事件信息结束------------------------------\r\n";
	return strDebugInfo;
}

//
bool CAlertSoundSendObj::SendAlert()
{
	string strDebugInfo = "";
	strDebugInfo += "\r\n------------------发送声音报警开始-----------------------------\r\n";
	strDebugInfo += ("报警监测器Id：" + strAlertMonitorId + "\r\n");
	strDebugInfo += ("发送事件类型：　Sound　\r\n");
	strDebugInfo += ("strServerValue：" + strServerValue + "\r\n");
	strDebugInfo += ("strLoginNameValue：" + strLoginNameValue + "\r\n");
	strDebugInfo += ("strLoginPwdValue：" + strLoginPwdValue + "\r\n");
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nSendId);
	string strCount = chItem;
	sprintf(chItem, "%d", nEventCount);
	string strEventCount = chItem;
	sprintf(chItem, "%d", nEventType);
	string strEventType = chItem;
	strDebugInfo += ("发送事件序号：" + strCount + "\r\n");
	strDebugInfo += ("原始事件序号：" + strEventCount + "\r\n");
	strDebugInfo += ("原始事件类型：" + strEventType + "\r\n");
	strDebugInfo += ("IDCUSERID：" + strIdcId + "\r\n");


	//////////////////////begin to modify at 07/07/31 /////////////////////////////
	//	try
	//	{
	//		CString strReturn = DoMsgBeep();
	//		
	//		OutputDebugString(strReturn);
	//
	//#ifdef IDC_Version		
	//		if(strReturn == "ok")
	//		{
	//			CAlertSendObj::InsertRecord(strServerValue, strIdcId, 4, 1);
	//		}
	//		else
	//		{
	//			CAlertSendObj::InsertRecord(strServerValue, strIdcId, 4, 0);
	//		}
	//	}
	//	catch(...)
	//	{
	//		CAlertSendObj::InsertRecord(strServerValue, strIdcId, 4, 0);
	//	}
	//#else
	//		if(strReturn == "ok")
	//		{
	//			CAlertSendObj::InsertRecord(strServerValue, 4, 1);
	//		}
	//		else
	//		{
	//			CAlertSendObj::InsertRecord(strServerValue, 4, 0);
	//		}
	//	}
	//	catch(...)
	//	{
	//		CAlertSendObj::InsertRecord(strServerValue, 4, 0);
	//	}
	//#endif
	try
	{
		CString strReturn = DoMsgBeep();
		OutputDebugString(strReturn);

		//DoPythonTaskTest();

		if(GetCgiVersion().compare("IDC") == 0)
		{
			if(strReturn == "ok")
			{
				CAlertSendObj::InsertRecord(strServerValue, strIdcId, 4, 1);
			}
			else
			{
				CAlertSendObj::InsertRecord(strServerValue, strIdcId, 4, 0);
			}
		}
		else
		{
			if(strReturn == "ok")
			{
				CAlertSendObj::InsertRecord(strServerValue, 4, 1);
			}
			else
			{
				CAlertSendObj::InsertRecord(strServerValue, 4, 0);
			}
		}
	}
	catch(...)
	{
		if(GetCgiVersion().compare("IDC") == 0)
		{
			CAlertSendObj::InsertRecord(strServerValue, strIdcId, 4, 0);
		}
		else
		{
			CAlertSendObj::InsertRecord(strServerValue, 4, 0);
		}
	}

	//////////////////////modify end at 07/07/31 //////////////////////////////////

	strDebugInfo += "------------------发送声音报警结束------------------------------\r\n";
	DebugePrint(strDebugInfo);
	
	return true;
}

//
CString CAlertSoundSendObj::DoMsgBeep()
{
	//CoInitialize(NULL);
	//_Alert *myRef=NULL;
	//HRESULT hr=CoCreateInstance(CLSID_Alert,NULL,
	//						CLSCTX_ALL,
	//						IID__Alert,(void **)&myRef);
	//if(SUCCEEDED(hr))
	//{
	//	DebugePrint("CAlertSoundSendObj creat com success");
	//}
	//else 
	//{
	//	DebugePrint("CAlertSoundSendObj creat com failed");
	//}

	VARIANT  a,b,c,d,e, vResult;
	BSTR bstrTmp;
	VariantInit(&a);
	VariantInit(&b);
	VariantInit(&c);
	VariantInit(&d);
	VariantInit(&e);
	VariantInit(&vResult);

	
	CString strServer(strServerValue.c_str());	
	CString strUser(strLoginNameValue.c_str());
	CString strPwd(strLoginPwdValue.c_str());
	if(strServerValue == "127.0.0.1")
	{
		strUser = "";
		strPwd = "";
	}
	
	bstrTmp=strServer.AllocSysString();
	a.vt=VT_BSTR;
	a.bstrVal=bstrTmp;
	
	bstrTmp=strUser.AllocSysString();
	b.bstrVal=bstrTmp;
	b.vt=VT_BSTR;
	
	bstrTmp=strPwd.AllocSysString();
	c.vt=VT_BSTR;
	c.bstrVal=bstrTmp;

	CString strReturn = _T("");
	int nParamCount = 0;

	//if(SUCCEEDED(hr))
	//{
	//	myRef->alertPlayMsgBeep(&a,&b,&c,&vResult);
	//	CString strResult(vResult.bstrVal);
	//		
	//	strReturn = strResult;
	//	myRef->Release();
	//	myRef = NULL;
	//}

	CAlertMain::mySoundRef->alertPlayMsgBeep(&a,&b,&c,&vResult);
	CString strResult(vResult.bstrVal);

	puts("声音报警：\n");
	puts(strResult.GetBuffer());
		
	strReturn = strResult;
	VariantClear(&a);
	VariantClear(&b);
	VariantClear(&c);
	VariantClear(&d);
	VariantClear(&e);
	VariantClear(&vResult);
	//CoUninitialize();
	return strReturn;
}

//
void CAlertSoundSendObj::DoPythonTaskTest()
{
	//OutputDebugString("Py_Initialize");
	//
	//Py_Initialize();	//python 解释器的初始化
	//
	//PyRun_SimpleString("execfile('testrpc.py')");
	//
	//Py_Finalize();		// 清除

	//OutputDebugString("Py_Finalize");
}



//
CAlertPythonSendObj::CAlertPythonSendObj()
{
	nType = 5;
}

//
CAlertPythonSendObj::~CAlertPythonSendObj()
{

}

//
string CAlertPythonSendObj::GetDebugInfo()
{
	string strDebugInfo = "";
	strDebugInfo += "\r\n------------------发送事件信息开始-----------------------------\r\n";
	strDebugInfo += ("报警监测器Id：" + strAlertMonitorId + "\r\n");
	strDebugInfo += ("发送事件类型：　Python　\r\n");
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nSendId);
	string strCount = chItem;
	sprintf(chItem, "%d", nEventCount);
	string strEventCount = chItem;
	sprintf(chItem, "%d", nEventType);
	string strEventType = chItem;
	strDebugInfo += ("发送事件序号：" + strCount + "\r\n");
	strDebugInfo += ("原始事件序号：" + strEventCount + "\r\n");
	strDebugInfo += ("原始事件类型：" + strEventType + "\r\n");
	strDebugInfo += ("产生发送事件的报警：" + strAlertName + "\r\n");
	strDebugInfo += "------------------发送事件信息结束------------------------------\r\n";
	return strDebugInfo;
}

//
bool CAlertPythonSendObj::SendAlert()
{
	DoPythonTask();
	return true;
}
//调用一参数函数
void CAlertPythonSendObj::InvokeWithParm()
{
	////PyObject*	pMod	= NULL;
	////PyObject*	pFunc	= NULL;
	////PyObject*	pParm	= NULL;
	////PyObject*	pRetVal	= NULL;
	////int			iRetVal	= 0;

	//////导入模块
	////pMod = PyImport_ImportModule("testrpc");
	////if(pMod)
	////{
	////	pFunc = PyObject_GetAttrString(pMod, "AddIssue");
	////	if(pFunc)
	////	{
	////		pParm = Py_BuildValue("(s)", "CXYALERTTEST");
	////		pRetVal = PyEval_CallObject(pFunc, pParm);
	////		//PyEval_CallObject(pFunc, NULL);
	////		//PyArg_Parse(pRetVal, "i", &iRetVal);
	////		//cout << "square 5 is: " << iRetVal << endl;
	////	}
	////	else
	////	{
	////		cout << "cannot find function square" << endl;
	////	}
	////}
	////else
	////{
	////	cout << "cannot find FuncDef.py" << endl;
	////}

	//PyObject*	pMod	= NULL;
	//PyObject*	pFunc	= NULL;
	//PyObject*	pParm	= NULL;
	//PyObject*	pRetVal	= NULL;
	//int			iRetVal	= 0;
	////导入模块
	//pMod = PyImport_ImportModule("testrpc");
	//if(pMod)
	//{
	//	pFunc = PyObject_GetAttrString(pMod, "AddIssue");
	//	if(pFunc)
	//	{
	//		pParm = PyTuple_New(2);
	//		PyTuple_SetItem(pParm, 0, Py_BuildValue("s", strReceive.c_str()));
	//		PyTuple_SetItem(pParm, 1, Py_BuildValue("s", strLevel.c_str()));
	//		pRetVal = PyEval_CallObject(pFunc, pParm);
	//		PyArg_Parse(pRetVal, "i", &iRetVal);
	//		cout << "return = " << iRetVal << endl;
	//	}
	//	else
	//	{
	//		//cout << "cannot find function square" << endl;
	//	}
	//}
	//else
	//{
	//	//cout << "cannot find FuncDef.py" << endl;
	//}
}

//
void CAlertPythonSendObj::DoPythonTask()
{
	////OutputDebugString("Py_Initialize");
	//
	//Py_Initialize();	//python 解释器的初始化
	//
	////执行 .py文件的方式
	////string strPython = "execfile(testrpc.py ";
	//////strPython += "Receive:";
	////strPython += strReceive;
	////strPython += " ";
	//////strPython += "Level:";
	////strPython += strLevel;
	////strPython += ")";

	//////string strPython = "globals = {'x': 7} locals = { } execfile(\"testrpc.py\", globals, locals)";
	////OutputDebugString(strPython.c_str());

	////PyRun_SimpleString(strPython.c_str());
	//////PyRun_SimpleString("execfile('testrpc.py')");
	//
	//InvokeWithParm();
	//Py_Finalize();		// 清除

	////OutputDebugString("Py_Finalize");
}