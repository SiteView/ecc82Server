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
#include <stack>
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
#include "base64.h"

#define OTL_ODBC  //以ODBC方式使用otl
#include "otlv4.h"
#pragma comment(lib,"odbc32.lib")

#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")
using namespace std;

//#include "StringEx.h"
#include "json_spirit.h"

using namespace ost;
static const basic_string <char>::size_type npos = -1;
extern void DebugePrint(string strDebugInfo);
#define XieCheng
#include "afxinet.h"
#include "windows.h"
#define  MAX_SMS_LENGTH 50
#include "./Base/StringHelper.h"
#include ".\alertsendobj.h"
//#include <Python.h>
using namespace SH;

const char svPassword[] = "password";
const int  svBufferSize = 1024 * 4;
//

map<int, string, less<int> > lsDeviceId;

//////////////AlertLog日志数据库的插入函数///////

using namespace std;
using namespace svutil;

using namespace boost;
using namespace json_spirit;



typedef unsigned char BYTE;

void add_pair( Object& obj, const string& name, const Value& value )
{
	obj.push_back( Pair( name, value ) );
}

// adds a new child object to an existing object,
// returns the new child object
//
Object& add_child_obj( Object& parent, const string& name )
{
	parent.push_back( Pair( name, Object() ) );

	return parent.back().value_.get_obj();
}

// adds a new child object to an existing array,
// returns the new child object
//
Object& add_child_obj( Array& parent )
{
	parent.push_back( Object() );

	return parent.back().get_obj();
}

Array& add_array( Object& obj, const string& name )
{
	obj.push_back( Pair( name, Array() ) );

	return obj.back().value_.get_array();
}

bool same_name( const Pair& pair, const string& name )
{
	return pair.name_ == name;
}

const Value& find_value( const Object& obj, const string& name )
{
	Object::const_iterator i=obj.begin();
	for(i=obj.begin();i!=obj.end();i++)
	{
//		if (i->
	}
	if( i == obj.end() ) 
		return Value::null;

	return i->value_;
}

const Array& find_array( const Object& obj, const string& name )
{
	return find_value( obj, name ).get_array(); 
}

int find_int( const Object& obj, const string& name )
{
	return find_value( obj, name ).get_int();
}

string find_str( const Object& obj, const string& name )
{
	return find_value( obj, name ).get_str();
}


//===============================================================
extern char strTemp[1024];

int WriteLog(const char* str);
//===============================================================


//utf8 转 Unicode
std::wstring Utf82Unicode(const std::string& utf8string)
{
	int widesize = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0)
	{
		throw std::exception("Error in conversion.");
	}

	std::vector<wchar_t> resultstring(widesize);

	int convresult = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, &resultstring[0], widesize);

	if (convresult != widesize)
	{
		throw std::exception("La falla!");
	}

	return std::wstring(&resultstring[0]);
}
//unicode 转为 ascii
string WideByte2Acsi(wstring& wstrcode)
{
	int asciisize = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, NULL, 0, NULL, NULL);
	if (asciisize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (asciisize == 0)
	{
		throw std::exception("Error in conversion.");
	}
	std::vector<char> resultstring(asciisize);
	int convresult =::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, &resultstring[0], asciisize, NULL, NULL);

	if (convresult != asciisize)
	{
		throw std::exception("La falla!");
	}

	return std::string(&resultstring[0]);
}

//utf-8 转 ascii
string UTF_82ASCII(string& strUtf8Code)
{
	string strRet("");

	//先把 utf8 转为 unicode 
	wstring wstr = Utf82Unicode(strUtf8Code);
	//最后把 unicode 转为 ascii
	strRet = WideByte2Acsi(wstr);

	return strRet;
}
///////////////////////////////////////////////////////////////////////
//ascii 转 Unicode
wstring Acsi2WideByte(string& strascii)
{
	int widesize = MultiByteToWideChar (CP_ACP, 0, (char*)strascii.c_str(), -1, NULL, 0);
	if (widesize == ERROR_NO_UNICODE_TRANSLATION)
	{
		throw std::exception("Invalid UTF-8 sequence.");
	}
	if (widesize == 0)
	{
		throw std::exception("Error in conversion.");
	}
	std::vector<wchar_t> resultstring(widesize);
	int convresult = MultiByteToWideChar (CP_ACP, 0, (char*)strascii.c_str(), -1, &resultstring[0], widesize);
	if (convresult != widesize)
	{
		throw std::exception("La falla!");
	}

	return std::wstring(&resultstring[0]);
}

//Unicode 转 Utf8
std::string Unicode2Utf8(const std::wstring& widestring)
{
	int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);
	if (utf8size == 0)
	{
		throw std::exception("Error in conversion.");
	}

	std::vector<char> resultstring(utf8size);

	int convresult = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, &resultstring[0], utf8size, NULL, NULL);

	if (convresult != utf8size)
	{
		throw std::exception("La falla!");
	}

	return std::string(&resultstring[0]);
}

//ascii 转 Utf8
string ASCII2UTF_8(string& strAsciiCode)
{
	string strRet("");

	//先把 ascii 转为 unicode 
	wstring wstr = Acsi2WideByte(strAsciiCode);
	//最后把 unicode 转为 utf8
	strRet = Unicode2Utf8(wstr);

	return strRet;
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
    char data[2048]={0};

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
    char data[2048]={0};

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
	//modyfy by jiewen.zhang，增加判断，当监视器不存在时，不发送告警
	string stmp=CAlertMain::GetMonitorTitle(strAlertMonitorId);
	if(stmp.empty())
		return false;

	WriteLog("\n\n====================CAlertEmailSendObj::SendAlert=======================");

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
		//-------------------------------------------------------------------------------------
		//修改邮件报警方式：绑定方式、独立方式和值班表方式同时发送；（zou_xiao 08－12－02）
		//-------------------------------------------------------------------------------------

		send_list.clear();

		string str_email_template = strEmailTemplateValue;//保存邮件模板，用于独立方式和值班表方式

		//绑定方式
		if(strcmp(strEmailAdressValue.c_str(), CAlertMain::strOther.c_str()) != 0)
		{
			WriteLog("============ 绑定方式 =============");

			string strTemp=strEmailAdressValue;

			basic_string <char>::size_type index1=0,index2=0;

			static const basic_string <char>::size_type npos = -1;

			

			while((index2=strTemp.find(",",index1+1))!=npos)
			{
				WriteLog("接收邮件地址:");
				WriteLog(strTemp.substr(index1,index2-index1).c_str());
				WriteLog("\n");
				if(GetInfoFromEmailAddress(strTemp.substr(index1,index2-index1)))
				{
					SendInfo send_info;
					send_info.str_mail_to = szMailTo;

					MakeAlertTitle();
					MakeAlertContent();

					send_info.str_alert_content = strAlertContent;
					send_info.str_alert_title = strAlertTitle;

					WriteLog(send_info.str_mail_to.c_str());
					WriteLog(send_info.str_alert_content.c_str());
					WriteLog(send_info.str_alert_title.c_str());

					send_list.push_back(send_info);
				}
				index1=index2+1;
			}

			index2 = strTemp.size();

			WriteLog("最后一个的接收邮件地址:");
			WriteLog(strTemp.substr(index1,index2-index1).c_str());
			WriteLog("\n");
			if(GetInfoFromEmailAddress(strTemp.substr(index1,index2-index1)))
			{

				SendInfo send_info;
				send_info.str_mail_to = szMailTo;

				MakeAlertTitle();
				MakeAlertContent();

				send_info.str_alert_content = strAlertContent;
				send_info.str_alert_title = strAlertTitle;

				WriteLog(send_info.str_mail_to.c_str());
				WriteLog(send_info.str_alert_content.c_str());
				WriteLog(send_info.str_alert_title.c_str());

				send_list.push_back(send_info);
			}
		}
		
		//独立方式
		if(!strOtherAdressValue.empty())
		{
			WriteLog("============ 独立方式 =============");

			strEmailTemplateValue = str_email_template;

			MakeAlertTitle();
			MakeAlertContent();

			WriteLog("strOtherAdressValue=");
			WriteLog(strOtherAdressValue.c_str());

			string strTemp=strOtherAdressValue;

			basic_string <char>::size_type index1=0,index2=0;

			static const basic_string <char>::size_type npos = -1;

			while((index2=strTemp.find(",",index1+1))!=npos)
			{
                SendInfo send_info;

				send_info.str_mail_to = strTemp.substr(index1,index2-index1);				

				send_info.str_alert_content = strAlertContent;
				send_info.str_alert_title = strAlertTitle;

				WriteLog(send_info.str_mail_to.c_str());
				WriteLog(send_info.str_alert_content.c_str());
				WriteLog(send_info.str_alert_title.c_str());

				send_list.push_back(send_info);

				index1=index2+1;
			}

			index2 = strTemp.size();

			SendInfo send_info;

			send_info.str_mail_to = strTemp.substr(index1,index2-index1);				

			send_info.str_alert_content = strAlertContent;
			send_info.str_alert_title = strAlertTitle;

			WriteLog(send_info.str_mail_to.c_str());
			WriteLog(send_info.str_alert_content.c_str());
			WriteLog(send_info.str_alert_title.c_str());

			send_list.push_back(send_info);

		}


		

		//值班表方式
		szMailTo = CAlertMain::GetCfgFromWatchList(strAlertIndex, true);
		WriteLog("szMailTo=");
		WriteLog(szMailTo.c_str());

		if(!szMailTo.empty())
		{
			WriteLog("============ 值班表方式  =============");

			strEmailTemplateValue = str_email_template;

			MakeAlertTitle();
			MakeAlertContent();

			string strTemp = szMailTo;

			basic_string <char>::size_type index1=0,index2=0;

			static const basic_string <char>::size_type npos = -1;

			while((index2=strTemp.find(",",index1+1))!=npos)
			{
                SendInfo send_info;

				send_info.str_mail_to = strTemp.substr(index1,index2-index1);				

				send_info.str_alert_content = strAlertContent;
				send_info.str_alert_title = strAlertTitle;

				WriteLog(send_info.str_mail_to.c_str());
				WriteLog(send_info.str_alert_content.c_str());
				WriteLog(send_info.str_alert_title.c_str());

				send_list.push_back(send_info);

				index1=index2+1;
			}

			index2 = strTemp.size();

			SendInfo send_info;

			send_info.str_mail_to = strTemp.substr(index1,index2-index1);				

			send_info.str_alert_content = strAlertContent;
			send_info.str_alert_title = strAlertTitle;

			WriteLog(send_info.str_mail_to.c_str());
			WriteLog(send_info.str_alert_content.c_str());
			WriteLog(send_info.str_alert_title.c_str());

			send_list.push_back(send_info);

		}

		//-------------------------------------------------------------------------------------

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

	/*
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
	*/

	//////////////////////modify end at 07/07/31 //////////////////////////////////

	//构造AlertTitle和AlertContent
	//MakeAlertTitle();
	//MakeAlertContent();
	//int iEmailRet = 1;

	//发送...	
	//CAlertMain::pSendEmail(szEmailServer.c_str(), szEmailfrom.c_str(), strAlertTitle.c_str(), strAlertContent.c_str(),
	//	szEmailContent.c_str(), szUserPwd.c_str(),	szUserID.c_str(), iEmailRet);
	
	/*
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
	*/
	
	//如果是多个邮件地址用逗号分隔则 
	//std::list<string> listEmail;
	//std::list<string>::iterator listEmailItem;
	//OutputDebugString (strAlertContent.c_str());
	std::list<SendInfo>::iterator listEmailItem;
	string str_send_address;
	string str_unsend_address;

	bool bSucess = false;
	bool bAllSucess = true;

	try
	{
		/*
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
		*/

		szMailTo.clear();

		for(listEmailItem = send_list.begin(); listEmailItem!=send_list.end(); listEmailItem++)
		{
			WriteLog("==================================================");
			WriteLog((*listEmailItem).str_mail_to.c_str());
			WriteLog((*listEmailItem).str_alert_title.c_str());
			WriteLog((*listEmailItem).str_alert_content.c_str());
			WriteLog("==================================================");

			if(szMailTo.empty())
				szMailTo = (*listEmailItem).str_mail_to;
			else
				szMailTo += "," + (*listEmailItem).str_mail_to;


			bSucess = CAlertMain::pSendEmail(szEmailServer.c_str(), szEmailfrom.c_str(), (*listEmailItem).str_mail_to.c_str(),
				(*listEmailItem).str_alert_title.c_str(), (*listEmailItem).str_alert_content.c_str(), szUserID.c_str(), szUserPwd.c_str(), NULL);
			//bSucess = CAlertMain::SendMail(szEmailServer, szEmailfrom, (*listEmailItem),
			//	strAlertTitle, strAlertContent, szUserID, szUserPwd);

			
			if(bSucess)
			{
				if(str_send_address.empty())
					str_send_address = (*listEmailItem).str_mail_to;
				else
					str_send_address += "," + (*listEmailItem).str_mail_to;

				
			}
			else
			{
				if(str_unsend_address.empty())
					str_unsend_address = (*listEmailItem).str_mail_to;
				else
					str_unsend_address += "," + (*listEmailItem).str_mail_to;
				bAllSucess = false;

			}

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
			if(!str_send_address.empty())
			{		
				CAlertSendObj::InsertRecord(str_send_address, strIdcId, 1, 1);
			}
			if(!str_unsend_address.empty())
			{
				CAlertSendObj::InsertRecord(str_unsend_address, strIdcId, 1, 0);
			}

		}
		else
		{
			if(!str_send_address.empty())
			{		
				CAlertSendObj::InsertRecord(str_send_address, 1, 1);
			}
			if(!str_unsend_address.empty())
			{
				CAlertSendObj::InsertRecord(str_unsend_address, 1, 0);
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

		DebugePrint("\r\n**********发送邮件异常********\r\n");
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

//	strDebugInfo += "------------------发送邮件结束------------------------------\r\n";
//	DebugePrint(strDebugInfo);
	DebugePrint("------------------发送邮件结束------------------------------\r\n");
	
/*
	string strURL="http://192.168.0.181:18080/itsm/control/JSonService";
	//发送URL
	CInternetSession session;
	CHttpFile*   fileGet= NULL;
	try
	{
		// 试着连接到指定URL
		fileGet = (CHttpFile*) session.OpenURL(strURL.c_str()); 
	}
	catch (...)
	{
		// 如果有错误的话，返回
		return FALSE;
	}

	//读出服务器的返回值
	if (fileGet)
	{
		json_spirit::Value value;
		//发送内容，获得令牌
		DWORD dwStatusCode;   
		string strContent;
		strContent="jsondata={\"dowhat\":\"login\",\"password\":\"admin\",\"username\":\"admin\"}";
//		fileGet->AddRequestHeaders("charset=UTF-8");
//		fileGet->AddRequestHeaders("pageEncoding=\"UTF-8\"");
		fileGet->SendRequest("",0,(LPVOID)(LPCTSTR)strContent.c_str(),strContent.length());
		fileGet -> QueryInfoStatusCode(dwStatusCode);   
		string content;   
		CString data;   
		string strResult,strKey,strToken;
		if(dwStatusCode == HTTP_STATUS_OK)   
		{   
			while (fileGet -> ReadString(data))   
			{   
				strResult  += data + "\r\n";
			}   
			//解析令牌
			json_spirit::read(strResult,value);
			json_spirit::Object root_obj( value.get_obj() );
			Object::const_iterator it=root_obj.begin();
			if( it == root_obj.end() ) 
				return false;

			strKey=it->name_;
			//返回的result后面即为令牌
			if (strKey=="result")
				strToken=(it->value_).get_str();

			//关闭连接
			fileGet->Close();
			//打开连接
			fileGet = (CHttpFile*) session.OpenURL(strURL.c_str()); 
			string strContent1;
			//第二步，利用令牌发送内容

			SYSTEMTIME sysTime;
			GetLocalTime(&sysTime);
			char sendbuff[4096];
			//把内容转换成json格式
			string jsonTitle,jsonContent;
			json_spirit::Object obj1;
			string stTime,strIssue,strHead;
			//加工时间
			obj1.push_back( Pair( "minutes", sysTime.wMinute ) );
			obj1.push_back( Pair( "hours", sysTime.wHour ) );
			obj1.push_back( Pair( "seconds", sysTime.wSecond ) );
			obj1.push_back( Pair( "year", sysTime.wYear ) );
			obj1.push_back( Pair( "month", sysTime.wMonth ) );
			obj1.push_back( Pair( "date", sysTime.wDay ) );
			stTime =json_spirit::write_formatted( obj1);
			//加入内容
			obj1.clear();
			obj1.push_back( Pair( "summary", strAlertTitle ) );
			obj1.push_back( Pair( "description", strAlertContent ) );
			obj1.push_back( Pair( "reporter", "admin") );
			obj1.push_back( Pair( "assignee", "admin") );
			obj1.push_back( Pair( "project", "TEST") );
			obj1.push_back( Pair( "type", "1") );
			obj1.push_back( Pair( "priority", "1") );
//			obj1.push_back( Pair( "duedate", stTime) );
			strIssue =json_spirit::write_formatted( obj1);
			//去掉后面的一个“｝”
			strIssue=strIssue.substr(0,strIssue.length()-1);
			obj1.clear();
			//加入头
			obj1.push_back( Pair( "dowhat", "createIssue" ) );
			obj1.push_back( Pair( "token", strToken ) );
//			obj1.push_back( Pair( "rIssue", strIssue) );
			strHead ="jsondata={\"dowhat\":\"createIssue\",\"token\":\"";
			strHead=strHead+strToken+"\",\"rIssue\":";

			string strSend;
			strSend=strHead+strIssue+",\"duedate\":"+stTime+"}}";

//			sprintf(sendbuff,"jsondata={\"dowhat\":\"createIssue\",\"token\":\"%s\",\"rIssue\":{\"id\":null,\"key\":null,%s,\"reporter\":\"admin\",\"assignee\":\"admin\",\"environment\":null,\"project\":\"TEST\",\"type\":\"1\",\"status\":null,\"priority\":\"1\",\"resolution\":null,\"created\":null,\"updated\":null,\"votes\":0,\"components\":null,\"affectsVersions\":null,\"fixVersions\":null,\"customFieldValues\":null,\"attachmentNames\":null,\"duedate\":{\"minutes\":%d,\"seconds\":%d,\"hours\":%d,\"month\":%d,\"year\":%d,\"date\":%d}}}",
//				strToken.c_str(),stemp.c_str(),sysTime.wHour,sysTime.wMinute,sysTime.wSecond,sysTime.wMonth,sysTime.wYear,sysTime.wDay);
			string strSend1=strSend;
			//将字符串转换为utf8
			strSend=GB2312ToUTF8(strSend1);
			cout<<endl<<"发送:。。。"<<strSend1<<endl;
			fileGet->SendRequest("",0,(LPVOID)(LPCTSTR)strSend.c_str(),strSend.length());
			fileGet -> QueryInfoStatusCode(dwStatusCode);   
			if(dwStatusCode == HTTP_STATUS_OK)   
			{   
				// 			CString content;   
				// 			CString data;   
				while (fileGet -> ReadString(data))   
				{   
					content  += data + "\r\n";   
				}   
			}
			//
			strSend1=UTF_82ASCII(content);

			//退出JSON
			//关闭连接
			fileGet->Close();
			//打开连接
			fileGet = (CHttpFile*) session.OpenURL(strURL.c_str()); 
			sprintf(sendbuff,"jsondata={\"dowhat\":\"logout\",\"token\":\"%s\"}",strToken.c_str());
			fileGet->SendRequest("",0,(LPVOID)(LPCTSTR)sendbuff,sizeof(sendbuff));
			fileGet -> QueryInfoStatusCode(dwStatusCode);   
			if(dwStatusCode == HTTP_STATUS_OK)   
			{   
				while (fileGet -> ReadString(data))   
				{   
					content  += data + "\r\n";   
				}   
			}
		}
	}
	//关闭连接
	fileGet->Close();
	session.Close();
*/
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


//按模板生成报警标题
//modify by jiewen.zhang on 08-11-06
void CAlertEmailSendObj::MakeAlertTitle()
{
	string strTmp;

	string strDeviceId = FindParentID(strAlertMonitorId);

	//从短信模板中获得模板参数
	strAlertTitle = GetIniFileString("Email", strEmailTemplateValue, "", "TxtTemplate.Ini");
	int nLength = strAlertTitle.length();
	int nPos=strAlertTitle.find("&");
	//cout<<endl<<"邮件标题模板全部＝："<<strEmailTemplateValue<<":"<<strAlertTitle<<endl;
	//如果找到了"\\;"
	if (nPos!=string::npos)
	{
		strAlertTitle=strAlertTitle.substr(0,nPos);
	}
	else//如果没找到，则人为设置一个标题，因为在邮件中需要一个标题
		strAlertTitle = "警报来自SiteView。@Group@ : @monitor@";
//	cout<<endl<<"邮件标题模板＝："<<strAlertTitle<<endl;
	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@Group@", CAlertMain::GetDeviceTitle(strAlertMonitorId));
	strAlertTitle = strTmp;
	
	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@monitor@", CAlertMain::GetMonitorTitle(strAlertMonitorId));
	strAlertTitle = strTmp;


	//替换模板中的一些参数 sxf 2008-12-4
	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@Monitor@", CAlertMain::GetMonitorTitle(strAlertMonitorId));
	strAlertTitle = strTmp;

	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@Device@", CAlertMain::GetDeviceTitle(strAlertMonitorId));
	strAlertTitle = strTmp;

	string strFullPathGroup = CAlertMain::GetAllGroupTitle(strDeviceId) + CAlertMain::GetDeviceTitle(strAlertMonitorId);
	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@FullPathGroup@", strFullPathGroup);
	strAlertTitle = strTmp;

	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@MonitorAlertDes@", CAlertMain::GetMonitorParameterValue(strAlertMonitorId,"sv_description"));
	strAlertTitle = strTmp;
	//
	
	//string strGrouptId = FindParentID(strDeviceId);
	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@AllGroup@", CAlertMain::GetAllGroupTitle(strDeviceId));
	strAlertTitle = strTmp;

	//添加状态
	//strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Status@", CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_stateString"));
	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@Status@", strEventDes);	
	strAlertTitle = strTmp;

	//添加时间	
	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@Time@", strTime);	
	strAlertTitle = strTmp;

	//加进程名
	string strMonitorType = CAlertMain::GetMonitorPropValue(strAlertMonitorId, "sv_monitortype");	
	if(strMonitorType == "14" || strMonitorType == "33" || strMonitorType == "41"
		|| strMonitorType == "111" || strMonitorType == "174"  || strMonitorType == "175")
	{
		// 14 Service  33 Nt4.0Process  41 Process  111 UnixProcess  174 SNMP_Process  175 SNMP_Service
		//strAlertTitle += " ";		

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
		//CloseResource(paramNode);
		CloseMonitor(hMon);
		strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@Process@", strProcName);	
		strAlertTitle = strTmp;
		
	}

	//加报警等级
	string strWarning;
	//加状态
	//1、正常。2、危险。3、错误。4、禁止。5、错误。 warnning( error ..) 
	if(nEventType == 1)
	{
		strWarning = "Ok";
	}
	else if(nEventType == 2)
	{
		strWarning = "Warnning";
	}
	else if(nEventType == 3)
	{
		strWarning = "Error";
	}
	else if(nEventType == 4)
	{
		strWarning = "Disable";
	}
	else
	{
		strWarning = "Error";
	}	
	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@Warning@", strWarning);	
	strAlertTitle = strTmp;

	OutputDebugString("============= CAlertEmailSendObj::MakeAlertTitle ==============\n");
	OutputDebugString(strAlertTitle.c_str());
	OutputDebugString("\n");
//	cout<<endl<<"邮件头"<<strAlertTitle<<endl;
}

//将阈值变量的名字换成中文
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

	string strDeviceId = FindParentID(strAlertMonitorId);


	//从模板中获得模板参数
	strAlertContent = GetIniFileString("Email", strEmailTemplateValue, "", "TxtTemplate.Ini");
	//strAlertContent = GetTemplateContent("Email", strEmailTemplateValue);
	//printf("----邮件模板----------------\n");
	//printf("%s\n", strAlertContent.c_str());
	//printf("-----------------------------\n");

	int nPos=strAlertContent.find("&");
	//如果找到了‘&’
//	cout<<endl<<"邮件内容模板全部＝："<<strAlertContent<<endl;
	int nLength = strAlertContent.length();
	if (nPos!=string::npos)
	{
		strAlertContent=strAlertContent.substr(nPos+1,nLength);
	}

//	cout<<endl<<"邮件内容模板＝："<<strAlertContent<<endl;
	
	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Group@", CAlertMain::GetDeviceTitle(strAlertMonitorId));
	strAlertContent = strTmp;
	
	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@monitor@", CAlertMain::GetMonitorTitle(strAlertMonitorId));
	strAlertContent = strTmp;

	//替换模板中的一些参数 sxf 2008-12-4
	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Monitor@", CAlertMain::GetMonitorTitle(strAlertMonitorId));
	strAlertContent = strTmp;

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Device@", CAlertMain::GetDeviceTitle(strAlertMonitorId));
	strAlertContent = strTmp;

	string strFullPathGroup = CAlertMain::GetAllGroupTitle(strDeviceId) + CAlertMain::GetDeviceTitle(strAlertMonitorId);
	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@FullPathGroup@", strFullPathGroup);
	strAlertContent = strTmp;

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@MonitorAlertDes@", CAlertMain::GetMonitorParameterValue(strAlertMonitorId,"sv_description"));
	strAlertContent = strTmp;
	//

	
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
		strFreq = CAlertMain::strMontorFreq.c_str();
		strFreq += ":";
		strFreq += CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_frequency");
		strFreq += CAlertMain::strMinute.c_str();

	}
	else
	{
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

	//CloseResource(map);
	CloseMonitor(hMon);
	//--------------------------------------------------------------------

	/*
	WriteLog("\n\n==================CAlertEmailSendObj::MakeAlertContent==================");
	WriteLog("monitortype=");WriteLog(getvalue.c_str());
	WriteLog("errorvalue=");WriteLog(szErrorValue.c_str());
	WriteLog("Fazhi=");WriteLog(CAlertMain::strMonitorFazhi.c_str());
	*/
	
	//strAlertContent += "\n阀值:     ";
	strAlertContent += "\n";
	strAlertContent += CAlertMain::strMonitorFazhi.c_str();
	strAlertContent += "       ";
	strAlertContent +=  strErrorValue;
	strAlertContent += "\n";
	//add end

	//替换@Log@ sxf 2008-12-4

	string strLogPath = GetSiteViewRootPath()+ "data\\Temp" +strAlertMonitorId;

	string strLog = CAlertMain::GetFileLastLine(strLogPath,"");

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Log@",  strLog);
	strAlertContent = strTmp;
	//

	////Begin
	////替换所有格式如下的 @@Key@@ ，得到Key，并得到相应的键值
	////printf("Log Start");
	//CString strMfcTemp, strTempKey, strPath,strLocalContent;	
	//strLocalContent= CString(strAlertContent.c_str());
	//CString strTempKeyValue = _T(""); 
	//while(strLocalContent.Find("@",0)>0)
	//{
	//	//printf("1");
	//	int nTemp = strLocalContent.Find("@",0);
	//	strMfcTemp = strLocalContent.Right(strLocalContent.GetLength() - nTemp - 1);
	//	//printf(strMfcTemp);
	//	strLocalContent = strLocalContent.Left(nTemp);
	//	//printf(strLocalContent);
	//	nTemp = strMfcTemp.Find("@",0);
	//	strTempKey = strMfcTemp.Left(nTemp);
	//	strMfcTemp = strTempKey;
	//	//printf(strMfcTemp);
	//	//strMfcTemp = strMfcTemp.Right(strMfcTemp.GetLength()-nTemp-1);
	//	//strTempKeyValue=FuncGetProfileStringBy(strSection,strTempKey,strGroupFile);
	//	//printf(strMfcTemp);
	//	if(strMfcTemp == "Log")
	//	{
	//		//printf("Entenr Log");
	//		//文件名称
	//		strPath.Format("%s\\%s\\%s.txt", GetSiteViewRootPath().c_str(), "data\\Temp", strAlertMonitorId.c_str());

	//		//printf(strPath);

	//		//取出该次报警对应的数据并替换@@Log@@
	//		//根据strTime判断， 如果最近一次时间大于。。。 如果最近一次时间小于。。。
	//		int nFileLength = 0;
	//		CFile* pFile = NULL;
	//		TRY	
	//		{
	//			pFile = new CFile(strPath, CFile::modeRead | CFile::shareDenyNone);
	//			nFileLength = pFile->GetLength();
	//			
	//		}
	//		CATCH(CFileException, pEx)	
	//		{
	//			if (pFile != NULL) 
	//			{
	//				pFile->Close();
	//				delete pFile;
	//			}

	//			return;
	//		}
	//		END_CATCH

	//		if (pFile != NULL) 
	//		{
	//			pFile->Close();
	//			delete pFile;
	//		}

	//		if (0 == nFileLength)
	//			return;

	//		CStringEx strTotleContent = _T("");
	//		CStringEx strLogContent = _T("");
	//		FILE * fp = fopen((LPCTSTR)strPath, "r");
	//		if (fp)
	//		{
	//			char * buffer = NULL;
	//			buffer = (char*)malloc(nFileLength + 1);
	//			if (buffer) 
	//			{
	//				memset(buffer, 0, nFileLength + 1);
	//				fread(buffer, sizeof(char), nFileLength + 1, fp);
	//				strTotleContent.Format("%s", buffer);	
	//				free(buffer);
	//			}
	//			fclose(fp);
	//		}
	//		
	//		if(strTotleContent != "")
	//		{
	//			int nStart = strTotleContent.ReverseFind("[Time is", -1);
	//			//strLogContent = strTotleContent.Right(strTotleContent.GetLength() - nStart - 2);
	//			strLogContent = strTotleContent.Right(strTotleContent.GetLength() - nStart);
	//			//DebugePrint("ReverseFind:\n");
	//			//DebugePrint(strLogContent);
	//			//printf(strLogContent);
	//			string strStdLogContent = strLogContent;
	//			//DebugePrint(strStdLogContent.c_str());
	//			strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Log@",  strStdLogContent);	
	//			strAlertContent = strTmp;
	//		}
	//	}

	//	//printf("Key %s Value is %s\n",strTempKey,strTempKeyValue);
	//	
	//	//if(!strTempKeyValue.IsEmpty())
	//	//{			
	//	//	strLocalContent=strLocalContent+strTempKeyValue+strTemp;
	//	//}
	//	//else
	//	//{
	//	//	strLocalContent=strLocalContent+"无设定"+strTemp;
	//	//}
	//}


	//加报警等级
	string strWarning;
	//加状态
	//1、正常。2、危险。3、错误。4、禁止。5、错误。 warnning( error ..) 
	if(nEventType == 1)
	{
		strWarning = "Ok";
	}
	else if(nEventType == 2)
	{
		strWarning = "Warnning";
	}
	else if(nEventType == 3)
	{
		strWarning = "Error";
	}
	else if(nEventType == 4)
	{
		strWarning = "Disable";
	}
	else
	{
		strWarning = "Error";
	}	
	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@Warning@", strWarning);	
	strAlertTitle = strTmp;
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
					{
						return false;
					}
					//szMailTo += GetIniFileString((*keyitem), "MailList", "", "emailAdress.ini");
					
					OutputDebugString("MailTo=");
					OutputDebugString(szMailTo.c_str());

					//GetIniFileString((*keyitem), "Template", "", "emailAdress.ini");
					//szSchedule = GetIniFileString((*keyitem), "Schedule", "", "emailAdress.ini");
					//GetIniFileString((*keyitem), "Des", "" , "emailAdress.ini");

					//szSchedule = GetIniFileString((*keyitem), "Schedule", "", "emailAdress.ini");
					//strEmailTemplateValue = GetIniFileString((*keyitem), "Template", "", "emailAdress.ini");
					
					szSchedule = GetIniFileString((*keyitem), "Schedule", "", "emailAdress.ini");

					//邮件接收地址有调度条件　则根据调度匹配判断是否将邮件地址与模板放入队列 （zou_xiao 08-12-2） 
					if(CAlertMain::IsScheduleMatch(szSchedule))
					{
						szMailTo = GetIniFileString((*keyitem), "MailList", "", "emailAdress.ini");
						strEmailTemplateValue = GetIniFileString((*keyitem), "Template", "", "emailAdress.ini");

						return true;
					}else
					{
						return false;
					}
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
						return false;
					//szMailTo += GetIniFileString((*keyitem), "MailList", "", "emailAdress.ini", "localhost", strIdcId);
					OutputDebugString("MailTo=");
					OutputDebugString(szMailTo.c_str());
					//GetIniFileString((*keyitem), "Template", "", "emailAdress.ini");
					//szSchedule = GetIniFileString((*keyitem), "Schedule", "", "emailAdress.ini", "localhost", strIdcId);
					//GetIniFileString((*keyitem), "Des", "" , "emailAdress.ini");

					//szSchedule = GetIniFileString((*keyitem), "Schedule", "", "emailAdress.ini");
					//strEmailTemplateValue = GetIniFileString((*keyitem), "Template", "", "emailAdress.ini", "localhost", strIdcId);
					
					szSchedule = GetIniFileString((*keyitem), "Schedule", "", "emailAdress.ini", "localhost", strIdcId);
									
					//邮件接收地址有调度条件　则根据调度匹配判断是否将邮件地址与模板放入队列 （zou_xiao 08-12-2） 
					if(CAlertMain::IsScheduleMatch(szSchedule))
					{
						szMailTo = GetIniFileString((*keyitem), "MailList", "", "emailAdress.ini", "localhost", strIdcId);
						strEmailTemplateValue = GetIniFileString((*keyitem), "Template", "", "emailAdress.ini", "localhost", strIdcId);
						return true;
					}else
					{
						return false;
					}


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
	//modyfy by jiewen.zhang，增加判断，当监视器不存在时，不发送告警
	string stmp=CAlertMain::GetMonitorTitle(strAlertMonitorId);
	if(stmp.empty())
		return false;
	string szWebUser = "", szWebPwd = "", szComPort = "", szTmp = "";

	//读email.ini获取smtp、发送地址等发送参数

	//////////////////////begin to modify at 07/07/31 /////////////////////////////
	//#ifdef IDC_Version
	if(GetCgiVersion().compare("IDC") == 0)
	{
	//////////////////////modify end at 07/07/31 //////////////////////////////////

		//用户名
		szWebUser=GetIniFileString("SMSWebConfig", "User", "",  "smsconfig.ini");
		// 密码
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


	//是否是升级邮件
	if(bUpgrade)
	{
		szSmsTo = strAlertUpgradeToValue;
	}
	else
	{
		//-------------------------------------------------------------------------------------
		//修改短信报警方式：绑定方式、独立方式和值班表方式同时发送；（zou_xiao 08－12－03）
		//-------------------------------------------------------------------------------------

		send_list.clear();

		string str_sms_template = strSmsTemplateValue;//保存短信模板，用于独立方式和值班表方式

		WriteLog("============ 绑定方式 =============");

		//绑定方式
		if(strcmp(strSmsNumberValue.c_str(), CAlertMain::strOther.c_str()) != 0)
		{
			string strTemp=strSmsNumberValue;

			basic_string <char>::size_type index1=0,index2=0;

			static const basic_string <char>::size_type npos = -1;
			

			while((index2=strTemp.find(",",index1+1))!=npos)
			{
				WriteLog("接收短信号码:");
				WriteLog(strTemp.substr(index1,index2-index1).c_str());
				WriteLog("\n");
				if(GetInfoFromSmsAddress(strTemp.substr(index1,index2-index1)))
				{
					SMSInfo sms_info;
					sms_info.str_sms_to = szSmsTo;

					if(strSmsSendMode == "Web")
					{
						MakeWebAlertContent();
					}
					else if(strSmsSendMode == "Com")
					{
						MakeComAlertContent();
					}
					else if (strSmsSendMode == "DataBase")
					{
						MakeComAlertContent();
					}
					else if(strSmsSendMode != "")
					{
						MakeComAlertContent();
					}

					sms_info.str_alert_content = strAlertContent;

					WriteLog(sms_info.str_sms_to.c_str());
					WriteLog(sms_info.str_alert_content.c_str());

					send_list.push_back(sms_info);
				}
				index1=index2+1;
			}

			index2 = strTemp.size();

			WriteLog("最后一个接收短信号码:");
			WriteLog(strTemp.substr(index1,index2-index1).c_str());
			WriteLog("\n");
			if(GetInfoFromSmsAddress(strTemp.substr(index1,index2-index1)))
			{

				SMSInfo sms_info;
				sms_info.str_sms_to = szSmsTo;

				if(strSmsSendMode == "Web")
				{
					MakeWebAlertContent();
				}
				else if(strSmsSendMode == "Com")
				{
					MakeComAlertContent();
				}else if (strSmsSendMode == "DataBase")
				{
					MakeComAlertContent();
				}
				else if(strSmsSendMode != "")
				{
					MakeComAlertContent();
				}

				sms_info.str_alert_content = strAlertContent;

				WriteLog(sms_info.str_sms_to.c_str());
				WriteLog(sms_info.str_alert_content.c_str());

				send_list.push_back(sms_info);
			}
		}else 

		//独立方式
		if(!strOtherNumberValue.empty())
		{
			strSmsTemplateValue = str_sms_template;

			if(strSmsSendMode == "Web")
			{
				MakeWebAlertContent();
			}
			else if(strSmsSendMode == "Com")
			{
				MakeComAlertContent();
			}else if (strSmsSendMode == "Database")
			{
				MakeComAlertContent();
			}
			else if(strSmsSendMode != "")
			{
				MakeComAlertContent();
			}

			WriteLog("strOtherNumberValue=");
			WriteLog(strOtherNumberValue.c_str());

			string strTemp=strOtherNumberValue;

			basic_string <char>::size_type index1=0,index2=0;

			static const basic_string <char>::size_type npos = -1;

			while((index2=strTemp.find(",",index1+1))!=npos)
			{
				SMSInfo sms_info;
				sms_info.str_sms_to = strTemp.substr(index1,index2-index1);				

				sms_info.str_alert_content = strAlertContent;

				WriteLog(sms_info.str_sms_to.c_str());
				WriteLog(sms_info.str_alert_content.c_str());

				send_list.push_back(sms_info);

				index1=index2+1;
			}

			index2 = strTemp.size();

			SMSInfo sms_info;
			sms_info.str_sms_to = strTemp.substr(index1,index2-index1);				

			sms_info.str_alert_content = strAlertContent;

			WriteLog(sms_info.str_sms_to.c_str());
			WriteLog(sms_info.str_alert_content.c_str());

			send_list.push_back(sms_info);

		}


		WriteLog("============ 值班表方式  =============");

		//值班表方式
		szSmsTo = CAlertMain::GetCfgFromWatchList(strAlertIndex, false);
		WriteLog("szSmsTo=");
		WriteLog(szSmsTo.c_str());

		if(!szSmsTo.empty())
		{
			strSmsTemplateValue = str_sms_template;

			if(strSmsSendMode == "Web")
			{
				MakeWebAlertContent();
			}
			else if(strSmsSendMode == "Com")
			{
				MakeComAlertContent();
			}else if (strSmsSendMode == "Database")
			{
				MakeComAlertContent();
			}
			else if(strSmsSendMode != "")
			{
				MakeComAlertContent();
			}

			string strTemp = szSmsTo;

			basic_string <char>::size_type index1=0,index2=0;

			static const basic_string <char>::size_type npos = -1;

			while((index2=strTemp.find(",",index1+1))!=npos)
			{
				SMSInfo sms_info;
				sms_info.str_sms_to = strTemp.substr(index1,index2-index1);				

				sms_info.str_alert_content = strAlertContent;

				WriteLog(sms_info.str_sms_to.c_str());
				WriteLog(sms_info.str_alert_content.c_str());

				send_list.push_back(sms_info);

				index1=index2+1;
			}

			index2 = strTemp.size();

			SMSInfo sms_info;
			sms_info.str_sms_to = strTemp.substr(index1,index2-index1);				

			sms_info.str_alert_content = strAlertContent;

			WriteLog(sms_info.str_sms_to.c_str());
			WriteLog(sms_info.str_alert_content.c_str());

			send_list.push_back(sms_info);

		}

		//-------------------------------------------------------------------------------------

		
	}

	//如果是多个手机号码用逗号分隔则 
	std::list<SMSInfo>::iterator listSMSItem;
	string str_unsend_phone;
	string str_send_phone;
	string str_all_phone;

	bool bSucess = false;
	try
	{
		for(listSMSItem = send_list.begin(); listSMSItem!=send_list.end(); listSMSItem++)
		{
			if(str_all_phone.empty())
				str_all_phone = (*listSMSItem).str_sms_to;
			else
				str_all_phone += "," + (*listSMSItem).str_sms_to;

			if(strSmsSendMode == "Web")
			{
				szSmsTo = (*(listSMSItem)).str_sms_to;
				strAlertContent = (*(listSMSItem)).str_alert_content;

				if(SendSmsFromWeb())
				{
					bSucess = true;

					if(str_send_phone.empty())
						str_send_phone = szSmsTo;
					else
						str_send_phone += "," + szSmsTo;
				}
				else
				{
					if(str_unsend_phone.empty())
						str_unsend_phone = szSmsTo;
					else
						str_unsend_phone += "," + szSmsTo;
				}


			}
			else if(strSmsSendMode == "Com")
			{
				szSmsTo = (*(listSMSItem)).str_sms_to;
				strAlertContent = (*(listSMSItem)).str_alert_content;

				if(SendSmsFromCom())
				{
					bSucess = true;

					if(str_send_phone.empty())
						str_send_phone = szSmsTo;
					else
						str_send_phone += "," + szSmsTo;
				}
				else
				{
					if(str_unsend_phone.empty())
						str_unsend_phone = szSmsTo;
					else
						str_unsend_phone += "," + szSmsTo;
				}

			}else if (strSmsSendMode == "DataBase")
			{

				szSmsTo = (*(listSMSItem)).str_sms_to;
				strAlertContent = (*(listSMSItem)).str_alert_content;

				if(SendSmsFromDatabase())
				{
					bSucess = true;

					if(str_send_phone.empty())
						str_send_phone = szSmsTo;
					else
						str_send_phone += "," + szSmsTo;
				}
				else
				{
					if(str_unsend_phone.empty())
						str_unsend_phone = szSmsTo;
					else
						str_unsend_phone += "," + szSmsTo;
				}

			}
			else if(strSmsSendMode != "")
			{
				szSmsTo = (*(listSMSItem)).str_sms_to;
				strAlertContent = (*(listSMSItem)).str_alert_content;

				printf("===========SendSmsFromSelfDefine:%s\n",strSmsSendMode.c_str());

				if(SendSmsFromSelfDefine())
				{
					bSucess = true;

					if(str_send_phone.empty())
						str_send_phone = szSmsTo;
					else
						str_send_phone += "," + szSmsTo;
				}
				else
				{
					if(str_unsend_phone.empty())
						str_unsend_phone = szSmsTo;
					else
						str_unsend_phone += "," + szSmsTo;
				}

			}
			else 
			{
				szSmsTo = (*(listSMSItem)).str_sms_to;
				
				if(str_unsend_phone.empty())
					str_unsend_phone = szSmsTo;
				else
					str_unsend_phone += "," + szSmsTo;
				
			}

		}

		if(GetCgiVersion().compare("IDC") == 0)
		{
			if(!str_send_phone.empty())
			{
				CAlertSendObj::InsertRecord(str_send_phone, strIdcId, 2, 1);
			}
			if(!str_unsend_phone.empty())
			{
				CAlertSendObj::InsertRecord(str_unsend_phone, strIdcId, 2, 0);
			}
		}
		else
		{
			if(!str_send_phone.empty())
			{
				CAlertSendObj::InsertRecord(str_send_phone, 2, 1);
			}
			if(!str_unsend_phone.empty())
			{
				CAlertSendObj::InsertRecord(str_unsend_phone, 2, 0);
			}
		}

		/*
		OutputDebugString("-----------------call senalert() for test selfdefine------------\n");
		if(strSmsSendMode == "Web")
		{
			MakeWebAlertContent();
			bSucess = SendSmsFromWeb();
		}
		else if(strSmsSendMode == "Com")
		{
			MakeComAlertContent();
			bSucess = SendSmsFromCom();
		}
		else if(strSmsSendMode != "")
		{
			sprintf(strTemp,"=============== SendAlert,monitorid=%s ===============",strAlertMonitorId.c_str());
			WriteLog(strTemp);

			MakeComAlertContent();

			sprintf(strTemp,"=============== SendAlert,monitorid=%s,AlertContent=%s ===============",
				strAlertMonitorId.c_str(),strAlertContent.c_str());
			WriteLog(strTemp);

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
		*/
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
			CAlertSendObj::InsertRecord(str_all_phone, strIdcId, 2, 0);
		}
		else
		{
			CAlertSendObj::InsertRecord(str_all_phone, 2, 0);
		}
		//////////////////////modify end at 07/07/31 //////////////////////////////////
		throw -9;

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
	string strTmp;
	//从短信模板中获得模板参数
	strAlertTitle = GetIniFileString("SMS", strSmsTemplateValue, "", "TxtTemplate.Ini");
	int nLength = strAlertTitle.length();
	int nPos=strAlertTitle.find("&");
	//如果找到了‘&’
	if (nPos!=string::npos)
	{
		strAlertTitle=strAlertTitle.substr(0,nPos);
	}
	else//如果没找到
		strAlertTitle = "";//将标题模板设置为空

//	cout<<endl<<"标题模板＝："<<strAlertTitle<<endl;
	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@Group@", CAlertMain::GetDeviceTitle(strAlertMonitorId));
	strAlertTitle = strTmp;

	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@monitor@", CAlertMain::GetMonitorTitle(strAlertMonitorId));
	strAlertTitle = strTmp;

	string strDeviceId = FindParentID(strAlertMonitorId);
	//string strGrouptId = FindParentID(strDeviceId);
	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@AllGroup@", CAlertMain::GetAllGroupTitle(strDeviceId));
	strAlertTitle = strTmp;

	//添加状态
	//strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Status@", CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_stateString"));
	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@Status@", strEventDes);	
	strAlertTitle = strTmp;

	//添加时间	
	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@Time@", strTime);	
	strAlertTitle = strTmp;

	//加进程名
	string strMonitorType = CAlertMain::GetMonitorPropValue(strAlertMonitorId, "sv_monitortype");	
	if(strMonitorType == "14" || strMonitorType == "33" || strMonitorType == "41"
		|| strMonitorType == "111" || strMonitorType == "174"  || strMonitorType == "175")
	{
		// 14 Service  33 Nt4.0Process  41 Process  111 UnixProcess  174 SNMP_Process  175 SNMP_Service
		//strAlertTitle += " ";		

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
		strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@Process@", strProcName);	
		strAlertTitle = strTmp;

		//CloseResource(paramNode);
		CloseMonitor(hMon);
	}
}

//生成Com方式发送的报警信息
//将Com方式与Web方式分开的目的是因为读配置读文件的section不一样
void CAlertSmsSendObj::MakeComAlertContent()
{
	sprintf(strTemp,"=============== CAlertSmsSendObj::MakeComAlertContent,monitorid=%s ===============",strAlertMonitorId.c_str());
	//WriteLog(strTemp);

	string strTmp;
	//从短信模板中获得模板参数

	sprintf(strTemp,"=============== CAlertSmsSendObj::MakeComAlertContent,monitorid=%s,SmsTemplateValue=%s ===============",
		strAlertMonitorId.c_str(), strSmsTemplateValue.c_str());
	WriteLog(strTemp);

	strAlertContent = GetIniFileString("SMS", strSmsTemplateValue, "", "TxtTemplate.Ini");
	//	strAlertContent = GetTemplateContent("SMS", strSmsTemplateValue);

	sprintf(strTemp,"=============== CAlertSmsSendObj::MakeComAlertContent,monitorid=%s,default_content=%s ===============",
		strAlertMonitorId.c_str(), strAlertContent.c_str());
	WriteLog(strTemp);

	int nLength = strAlertContent.length();
	int nPos=strAlertContent.find("&");
	//如果找到了"\\;"
	if (nPos!=string::npos)
	{
		strAlertContent=strAlertContent.substr(nPos+2,strAlertContent.length());
	}
	// 	cout<<endl<<"内容模板＝："<<strAlertContent<<endl;

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Group@", CAlertMain::GetDeviceTitle(strAlertMonitorId));
	strAlertContent = strTmp;

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@monitor@", CAlertMain::GetMonitorTitle(strAlertMonitorId));
	strAlertContent = strTmp;


#ifndef SHORT_SMS//zou_xiao 08-12-01
	string strDeviceId = FindParentID(strAlertMonitorId);
	//string strGrouptId = FindParentID(strDeviceId);
	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@AllGroup@", CAlertMain::GetAllGroupTitle(strDeviceId));
	strAlertContent = strTmp;
	
	sprintf(strTemp,"=============== CAlertSmsSendObj::MakeComAlertContent,monitorid=%s,AllGroup=%s,AlertContent=%s ===============",
		strAlertMonitorId.c_str(), CAlertMain::GetAllGroupTitle(strDeviceId).c_str(),strAlertContent.c_str());
	WriteLog(strTemp);
#endif

	
	//strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Status@", CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_stateString"));
	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Status@", strEventDes);
	strAlertContent = strTmp;

	/*
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
	*/


	//strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Time@",  CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_lastMeasurementTime"));

	string str_time = strTime;

#ifdef SHORT_SMS//zou_xiao 08-12-01
	basic_string <char>::size_type index = str_time.find("-");
	str_time.erase(str_time.begin(),str_time.begin()+index+1);
#endif

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Time@", str_time);
	strAlertContent = strTmp;

	//替换@Log@ sxf 2008-12-4

	string strLogPath = GetSiteViewRootPath()+ "data\\Temp" +strAlertMonitorId;

	string strLog = CAlertMain::GetFileLastLine(strLogPath,"");

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Log@",  strLog);
	strAlertContent = strTmp;
	//

	////Begin
	////替换所有格式如下的 @@Key@@ ，得到Key，并得到相应的键值
	////printf("Log Start");
	//CString strMfcTemp, strTempKey, strPath, strLocalContent;
	//strLocalContent= CString(strAlertContent.c_str());
	//CString strTempKeyValue = _T(""); 
	//while(strLocalContent.Find("@",0)>0)
	//{
	//	//printf("1");
	//	int nTemp = strLocalContent.Find("@",0);
	//	strMfcTemp = strLocalContent.Right(strLocalContent.GetLength() - nTemp - 1);
	//	//printf(strMfcTemp);
	//	strLocalContent = strLocalContent.Left(nTemp);
	//	//printf(strLocalContent);
	//	nTemp = strMfcTemp.Find("@",0);
	//	strTempKey = strMfcTemp.Left(nTemp);
	//	strMfcTemp = strTempKey;
	//	//printf(strMfcTemp);
	//	//strMfcTemp = strMfcTemp.Right(strMfcTemp.GetLength()-nTemp-1);
	//	//strTempKeyValue=FuncGetProfileStringBy(strSection,strTempKey,strGroupFile);
	//	//printf(strMfcTemp);
	//	if(strMfcTemp == "Log")
	//	{
	//		//printf("Entenr Log");
	//		//文件名称
	//		strPath.Format("%s\\%s\\%s.txt", GetSiteViewRootPath().c_str(), "data\\Temp", strAlertMonitorId.c_str());

	//		//printf(strPath);

	//		//取出该次报警对应的数据并替换@@Log@@
	//		//根据strTime判断， 如果最近一次时间大于。。。 如果最近一次时间小于。。。
	//		int nFileLength = 0;
	//		CFile* pFile = NULL;
	//		TRY	
	//		{
	//			pFile = new CFile(strPath, CFile::modeRead | CFile::shareDenyNone);
	//			nFileLength = pFile->GetLength();
	//		}
	//		CATCH(CFileException, pEx)	
	//		{
	//			if (pFile != NULL) 
	//			{
	//				pFile->Close();
	//				delete pFile;
	//			}

	//			return;
	//		}
	//		END_CATCH

	//			if (pFile != NULL) 
	//			{
	//				pFile->Close();
	//				delete pFile;
	//			}

	//			if (0 == nFileLength)
	//				return;

	//			CString strTotleContent = _T("");
	//			CString strLogContent = _T("");
	//			FILE * fp = fopen((LPCTSTR)strPath, "r");
	//			if (fp)
	//			{
	//				char * buffer = NULL;
	//				buffer = (char*)malloc(nFileLength + 1);
	//				if (buffer) 
	//				{
	//					memset(buffer, 0, nFileLength + 1);
	//					fread(buffer, sizeof(char), nFileLength + 1, fp);
	//					strTotleContent.Format("%s", buffer);	
	//					free(buffer);
	//				}
	//				fclose(fp);
	//			}

	//			if(strTotleContent != "")
	//			{
	//				int nStart = strTotleContent.ReverseFind(']');

	//				strLogContent = strTotleContent.Right(strTotleContent.GetLength() - nStart - 2);
	//				//printf(strLogContent);
	//				string strStdLogContent = strLogContent;

	//				strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Log@",  strStdLogContent);	
	//				strAlertContent = strTmp;
	//			}
	//	}
	//}

}

//生成WEB方式发送的报警信息
void CAlertSmsSendObj::MakeWebAlertContent()
{
	string strTmp;
	//从短信模板中获得模板参数
	strAlertContent = GetIniFileString("WebSmsConfige", strSmsTemplateValue, "", "TxtTemplate.Ini");
	
	int nLength = strAlertContent.length();
 	int nPos=strAlertContent.find("\\;");
	//如果找到了"\\;"
	if (nPos!=string::npos)
	{
		strAlertContent=strAlertContent.substr(nPos+2,strAlertContent.length());
	}
	// 	cout<<endl<<"内容模板＝："<<strAlertContent<<endl;

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Group@", CAlertMain::GetDeviceTitle(strAlertMonitorId));
	strAlertContent = strTmp;

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@monitor@", CAlertMain::GetMonitorTitle(strAlertMonitorId));
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

	//替换@Log@ sxf 2008-12-18

	string strLogPath = GetSiteViewRootPath()+ "data\\Temp" +strAlertMonitorId;

	string strLog = CAlertMain::GetFileLastLine(strLogPath,"");

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Log@",  strLog);
	strAlertContent = strTmp;
	//
	
	////Begin
	////替换所有格式如下的 @@Key@@ ，得到Key，并得到相应的键值
	////printf("Log Start");
	//CString strMfcTemp, strTempKey, strPath, strLocalContent;
	//strLocalContent= CString(strAlertContent.c_str());
	//CString strTempKeyValue = _T(""); 
	//while(strLocalContent.Find("@",0)>0)
	//{
	//	//printf("1");
	//	int nTemp = strLocalContent.Find("@",0);
	//	strMfcTemp = strLocalContent.Right(strLocalContent.GetLength() - nTemp - 1);
	//	//printf(strMfcTemp);
	//	strLocalContent = strLocalContent.Left(nTemp);
	//	//printf(strLocalContent);
	//	nTemp = strMfcTemp.Find("@",0);
	//	strTempKey = strMfcTemp.Left(nTemp);
	//	strMfcTemp = strTempKey;
	//	//printf(strMfcTemp);
	//	//strMfcTemp = strMfcTemp.Right(strMfcTemp.GetLength()-nTemp-1);
	//	//strTempKeyValue=FuncGetProfileStringBy(strSection,strTempKey,strGroupFile);
	//	//printf(strMfcTemp);
	//	if(strMfcTemp == "Log")
	//	{
	//		//printf("Entenr Log");
	//		//文件名称
	//		strPath.Format("%s\\%s\\%s.txt", GetSiteViewRootPath().c_str(), "data\\Temp", strAlertMonitorId.c_str());

	//		//printf(strPath);

	//		//取出该次报警对应的数据并替换@@Log@@
	//		//根据strTime判断， 如果最近一次时间大于。。。 如果最近一次时间小于。。。
	//		int nFileLength = 0;
	//		CFile* pFile = NULL;
	//		TRY	
	//		{
	//			pFile = new CFile(strPath, CFile::modeRead | CFile::shareDenyNone);
	//			nFileLength = pFile->GetLength();
	//		}
	//		CATCH(CFileException, pEx)	
	//		{
	//			if (pFile != NULL) 
	//			{
	//				pFile->Close();
	//				delete pFile;
	//			}

	//			return;
	//		}
	//		END_CATCH

	//			if (pFile != NULL) 
	//			{
	//				pFile->Close();
	//				delete pFile;
	//			}

	//			if (0 == nFileLength)
	//				return;

	//			CString strTotleContent = _T("");
	//			CString strLogContent = _T("");
	//			FILE * fp = fopen((LPCTSTR)strPath, "r");
	//			if (fp)
	//			{
	//				char * buffer = NULL;
	//				buffer = (char*)malloc(nFileLength + 1);
	//				if (buffer) 
	//				{
	//					memset(buffer, 0, nFileLength + 1);
	//					fread(buffer, sizeof(char), nFileLength + 1, fp);
	//					strTotleContent.Format("%s", buffer);	
	//					free(buffer);
	//				}
	//				fclose(fp);
	//			}

	//			if(strTotleContent != "")
	//			{
	//				int nStart = strTotleContent.ReverseFind(']');

	//				strLogContent = strTotleContent.Right(strTotleContent.GetLength() - nStart - 2);
	//				//printf(strLogContent);
	//				string strStdLogContent = strLogContent;

	//				strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Log@",  strStdLogContent);	
	//				strAlertContent = strTmp;
	//			}
	//	}

	//}
	////End
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
					{
						return false;
					}
					//szSmsTo += GetIniFileString((*keyitem), "Phone", "", "smsphoneset.ini");
					szSchedule = GetIniFileString((*keyitem), "Plan", "", "smsphoneset.ini");

					if(CAlertMain::IsScheduleMatch(szSchedule))
					{
						szSmsTo = GetIniFileString((*keyitem), "Phone", "", "smsphoneset.ini");
						strSmsTemplateValue = GetIniFileString((*keyitem), "Template", "", "smsphoneset.ini");
						return true;
					}else
					{
						return false;
					}

					//strSmsTemplateValue = GetIniFileString((*keyitem), "Template", "", "smsphoneset.ini");
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
					{
						return false;
					}
					//szSmsTo += GetIniFileString((*keyitem), "Phone", "", "smsphoneset.ini", "localhost", strIdcId);
					szSchedule = GetIniFileString((*keyitem), "Plan", "", "smsphoneset.ini", "localhost", strIdcId);

					if(CAlertMain::IsScheduleMatch(szSchedule))
					{
						szSmsTo = GetIniFileString((*keyitem), "Phone", "", "smsphoneset.ini", "localhost", strIdcId);
						strSmsTemplateValue = GetIniFileString((*keyitem), "Template", "", "smsphoneset.ini", "localhost", strIdcId);
						return true;
					}else
					{
						return false;
					}
					//strSmsTemplateValue = GetIniFileString((*keyitem), "Template", "", "smsphoneset.ini", "localhost", strIdcId);
				}
			}
		}

	//////////////////////begin to modify at 07/07/31 /////////////////////////////
	//#endif
	}
	//////////////////////modify end at 07/07/31 //////////////////////////////////

	return false;
}


void CAlertSmsSendObj::String2Array(std::vector<std::string> &vOut,string strIn,string strDiv)
{
	int pos1= 0,pos2 = 0;
	
	vOut.empty();

	if (strIn.size() <=0)
		return;

	while(1)
	{
		pos2 = strIn.find(strDiv, pos1);

		if (pos2 < 0)
		{
			vOut.push_back(strIn.substr(pos1, strIn.length() -pos1));
			break;
		}
		
		vOut.push_back(strIn.substr(pos1, pos2 -pos1));
		
		pos1 = pos2 +1;
		pos2 = pos1;

	}
	
}

int CAlertSmsSendObj::process_msg(std::vector<std::string> &vOut,string strIn,string strFmt,int msglen)
{
	string strMsg = strIn;

	std::vector<std::string> vTmp;

	if (strMsg.size() > msglen+4)
	{
		
		int i = 1;
		while(strMsg.size() > 0)
		{

			int count = 0;
			bool split = false;
			string strtmp;

			while(count < strMsg.size())
			{
				unsigned char c = strMsg[count];

				strtmp.push_back(c);

				if (c > 128)
					split = !split;

				++count;

				if ((count==msglen-1))
				{
					if (!split)
						break;
					else
					{
						strtmp.push_back(strMsg[count]);
						++count;
						break;
					}
				}

			}

			strMsg.erase(0,strtmp.size());
			vTmp.push_back(strtmp);
		}
		
		for(int i=0; i < vTmp.size(); i++)
		{
			char szTmp[512];

			if (strFmt.size() > 0)
			{
				sprintf(szTmp, strFmt.c_str(), vTmp.size(),i+1,vTmp[i].c_str());
				vOut.push_back(szTmp);
			}else
			{
				vOut.push_back(vTmp[i]);
			}
		}
		vTmp.clear();

	}else
	{
		vOut.push_back(strIn);
	}
	return 1;
}

//通过数据库发送短信
bool CAlertSmsSendObj::SendSmsFromDatabase(string& err)
{
	otl_connect::otl_initialize();

	otl_connect db;
	std::vector<std::string> strPhones; //
	std::vector<std::string> strMsgs;	//

	//
	string strInsert;
	string strValues;
	string strSql;

	//方式
	string strParmCount = GetIniFileString("DataBaseConfig", "PramsCount", "0", "smsconfig.ini");
	string InsertWay = GetIniFileString("DataBaseConfig", "InsertWay", "", "smsconfig.ini");
	string dbType = GetIniFileString("DataBaseConfig", "DataBaseType", "", "smsconfig.ini");
	string cnnString = GetIniFileString("DataBaseConfig", "ConnectionString", "", "smsconfig.ini");

	int paramCount = 0;
	

	std::vector<std::string> strParams;
	std::vector<std::string> strParaTypes;
	std::vector<std::string> strParaDescs;
	std::vector<int> strParaLengths;
	std::vector<std::string> strTags;		//占位符
	string procType;


	char *cEnd = NULL;
	paramCount = strtol(strParmCount.c_str(),&cEnd,10);
	printf("==InsertWay:%s,DataBaseType:%s,cnnString:%s,paramCount:%d\n", InsertWay.c_str(), dbType.c_str(), cnnString.c_str(),paramCount);
	//没有参数当然插不入信息了
	if (paramCount < 2)
	{
		err= "错误：设置的参数太少";
		return false;
	}

	//取出参数信息
	for(int i = 0; i < paramCount; i++)
	{
		char szTmp[32];
		string strType;
		sprintf(szTmp, "PramName%d", i);
		strParams.push_back(GetIniFileString("DataBaseConfig", szTmp, "", "smsconfig.ini"));
		sprintf(szTmp, "PramType%d", i);
		strType = GetIniFileString("DataBaseConfig", szTmp, "", "smsconfig.ini");
		strParaTypes.push_back(strType);
		sprintf(szTmp, "PramLength%d", i);

		string strLen = GetIniFileString("DataBaseConfig", szTmp, "0", "smsconfig.ini");

		strParaLengths.push_back(strtol(strLen.c_str(), &cEnd,10));

		if (strType.compare("Int") == 0)
			sprintf(szTmp, ":P%d<int,in>", i);
		else if(strType.compare("int") == 0)
			sprintf(szTmp, ":P%d<int,in>", i);
		else if(strType.compare("String") == 0)
			sprintf(szTmp, ":P%d<varchar,in>", i);
		else if (strType.compare("string") == 0)
			sprintf(szTmp, ":P%d<varchar,in>", i);
		else if (strType.compare("DateTime") == 0)
			sprintf(szTmp, ":P%d<timestamp,in>", i);
		else
			sprintf(szTmp, ":P%d<varchar,in>", i);

		strTags.push_back(szTmp);

		sprintf(szTmp, "PramDesc%d", i);
		strParaDescs.push_back(GetIniFileString("DataBaseConfig", szTmp, "", "smsconfig.ini"));

	}

	printf("==== 取参数成功了===\n");
	

	
	if (InsertWay.compare("1") == 0)
	{
		//通过表插入
		//
		string tableName = GetIniFileString("DataBaseConfig", "TableName", "", "smsconfig.ini");
		
		//拼装SQL语句
		strInsert = "insert into " + tableName + "(" + strParams[0];
		strValues = " values(" + strTags[0];

		for(std::vector<std::string>::size_type i = 1; i < strParams.size(); i++)
		{
			strInsert += "," + strParams[i];
			strValues += "," + strTags[i];
		}

		strInsert +=")";
		strValues +=")";


		strInsert += strValues;
		strSql = strInsert;
		//
	

	}else if (InsertWay.compare("0") == 0)
	{
		//通过存储过程插入
		//取得存储过程配置参数
		string procName = GetIniFileString("DataBaseConfig", "PorcName", "", "smsconfig.ini");
		string procReturn = GetIniFileString("DataBaseConfig", "PorcReturn", "", "smsconfig.ini");
		

		if (procReturn.size() > 0)
		{
			procType = GetIniFileString("DataBaseConfig", "PorcType", "", "smsconfig.ini");
		}

		//拼装SQL
		if(procReturn.size()>0)
		{
			if (procType.compare("Int") == 0)
				strInsert ="{ :r(int,out)=";
			else
				strInsert ="{ :r(varchar,out)=";
		}
		else
			strInsert = "{ ";

		strInsert += "call " + procName;

		strInsert += "(" + strTags[0];

		for(std::vector<std::string>::size_type i = 1; i < strParams.size(); i++)
		{
			strInsert += "," + strTags[i];
		}
		strInsert +=")}";

		strSql = strInsert;

	}


	try
	{
		db.rlogon(cnnString.c_str());
		String2Array(strPhones,szSmsTo,",");
		process_msg(strMsgs,strAlertContent,"",strParaLengths[1] );
		
		//执行SQL
		for (std::vector<std::string>::iterator i = strPhones.begin(); i != strPhones.end(); i++)
		{
			for(std::vector<std::string>::iterator j = strMsgs.begin(); j != strMsgs.end(); j++)
			{

				printf("======*****SQL:%s\n", strSql.c_str());

				otl_long_string mobile(i->c_str(),0,i->size());
				otl_long_string msg(j->c_str(), 0,j->size());
				SYSTEMTIME st;

				::GetLocalTime(&st);
				otl_datetime tm(st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);

				if (dbType.compare("Sybase") == 0) //Sybase另外处理
				{
					string strTmpSql = strSql;

					if (InsertWay.compare("0") == 0) 
					{ //存储过程
						strTmpSql = CAlertMain::ReplaceStdString(strTmpSql,"r(int,out)=","");
					}

					for(std::vector<std::string>::size_type k = 0; k < strParaDescs.size(); k++)
					{
						if (strParaDescs[k].compare("手机号码")==0)
						{
							if (strParaTypes[k].compare("Int") == 0)
								strTmpSql = CAlertMain::ReplaceStdString(strTmpSql,strTags[k],*i);
							else
								strTmpSql = CAlertMain::ReplaceStdString(strTmpSql,strTags[k],string("'")+ *i + string("'"));
						}
						else if (strParaDescs[k].compare("短信内容")==0)
						{
							if (strParaTypes[k].compare("Int") == 0)
								strTmpSql = CAlertMain::ReplaceStdString(strTmpSql,strTags[k],*j);
							else
								strTmpSql = CAlertMain::ReplaceStdString(strTmpSql,strTags[k],string("'")+ *j+ string("'"));
						}
						else if (strParaDescs[k].compare("发送时间")==0)
							strTmpSql = CAlertMain::ReplaceStdString(strTmpSql,strTags[k],"getdate()");
					}

					db.direct_exec(strTmpSql.c_str());
					
				}else
				{
					otl_stream s(1, strSql.c_str(),db);

					for(std::vector<std::string>::iterator k = strParaDescs.begin(); k !=strParaDescs.end(); k++)
					{
						if (k->compare("手机号码")==0)
							s<<mobile;
						else if (k->compare("短信内容")==0)
							s<<msg;
						else if (k->compare("发送时间")==0)
							s<<tm;
					}

					if (procType.size() > 0)
					{
						if (procType.compare("Int") == 0)
						{
							int iRet;
							s>>iRet;
						}/*else if (procType.compare("bool") == 0)
						{
							bool bRet;
							s>>bRet;
						}*/else
						{
							otl_long_string ret;
							s>>ret;
							
						}
					}
				}

			}
		}

		err = "成功：插入数据库成功。";

	}catch(otl_exception& p) 
	{  
		// intercept OTL exceptions  
		WriteLog((const char*)p.msg); // print out error message  
		err= "错误："+ string((const char*)p.msg);
		WriteLog(p.stm_text); // print out SQL that caused the error  
		db.rollback();
		printf("==%s\n",err.c_str());
		return false;
	} 
	
	return true;
}

//通过串口方式发送短信
bool CAlertSmsSendObj::SendSmsFromCom()
{
	bool bSucess = true;
	bool bAllSucess = true;
	
	
	//读取发送短信的长度设置
	/*
	string sendlength="50";
	int nSMSSendLength=CAlertMain::nSMSSendLength;
	sendlength = GetIniFileString("SMSCommConfig", "length", "", "smsconfig.ini");
	if (!sendlength.empty())
		sscanf(sendlength.c_str(),"%d",&nSMSSendLength);
	*/
	string strSMSMaxLength = GetIniFileString( "SMSCommConfig", "length", "70", "smsconfig.ini" );
	int nSMSMaxLength(70);
	if( !strSMSMaxLength.empty() )
	{
		nSMSMaxLength = atoi( strSMSMaxLength.c_str() );
	}
	


	//如果是多个手机号码用逗号分隔则 
	std::list<string> listSms;
	std::list<string>::iterator listSmsItem;
	CAlertMain::ParserToken( listSms, szSmsTo.c_str(), ",");

	

	std::list<string> listSmsContent;
	std::list<string>::iterator listSmsContentItem;

	/*	
	char * buf = new char[300];
	string strSend;
	//将短信截断
	CAnalyzer4_bstr_t Analyzer;
	_bstr_t strMsgs = strAlertContent.c_str();
	OutputDebugString(strAlertContent.c_str());
	//	int nMaxSMSLen = MAX_SMS_LENGTH;
	//修改为用户设定的值

	// 对原始短信正文进行宽字符分段
	int nPage = Analyzer.Analyzer(strMsgs, nSMSSendLength);
	//	sprintf(dechar, "npage = %d; len=%d", nPage, strAlertContent.length());
	//	OutputDebugString(dechar);
	for (int i = 0; i < nPage; i++)
	{
		memset(buf, 0, 300);
		_bstr_t strMsg = Analyzer.GetResult(i);
		strcpy(buf, strMsg);
		listSmsContent.push_back(buf);
	}
	*/


	//如果是多个手机号码用逗号分隔则
	for(listSmsItem = listSms.begin(); listSmsItem!=listSms.end(); listSmsItem++)
	{
		CString strSmsTo = (*listSmsItem).c_str();		

		CString strContent = "";
		listSmsContent.push_back( strAlertContent );
		
		for(listSmsContentItem = listSmsContent.begin(); listSmsContentItem!=listSmsContent.end(); listSmsContentItem++)
		{
			strContent = (*listSmsContentItem).c_str();		
			//DebugePrint((*listSmsContentItem).c_str());
			//DebugePrint("SendSmsFromCom\r\n");
			//修改，当短信发不出去时，尝试三次，如果三次都发不出，则认为失败
			//改为只发送一次，因为有的设备返回了发送失败，而实际上已经发送成功。就有可能造成连续发送了三条短信
//			if(CAlertMain::SendSmsFromComm(strSmsTo, strContent) == -1)
//				bSucess=false;
 			int iCount;
 			for ( iCount=0; iCount<3; iCount++ )
 			{
 				if( CAlertMain::SendSmsFromComm( strSmsTo, strContent, nSMSMaxLength ) == -1 )
 				{
 					Sleep(1000);//停顿一秒后再发送
 					continue;
 				}
 				else
 					break;
 			}
 			if(iCount==3)//如果尝试了三次，则认为发送不成功
 				bSucess=false;
			Sleep(2000);//停顿两秒再发下一短信
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
	string RetValue("OK");

	string strWebHead;
	//从短信模板中读出短信头的模板
	strWebHead = GetIniFileString("WebSmsConfige", strSmsTemplateValue, "", "TxtTemplate.Ini");
	int nLength = strWebHead.length();
	int nPos=strWebHead.find("\\;");
	//如果找到了‘&’
	if (nPos!=string::npos)
	{
		strWebHead=strWebHead.substr(0,nPos);
	}
	else//如果没找到
		strWebHead = "";//将标题模板设置为空

	User = GetIniFileString("SMSWebConfig", "User", "", "smsconfig.ini");

	Pwd = GetIniFileString("SMSWebConfig", "Pwd", "", "smsconfig.ini");
	Des mydes;
	char dechar[1024]={0};
	if(Pwd.size()>0)
	{
		mydes.Decrypt(Pwd.c_str(),dechar);
		Pwd = dechar;
	}

    int nSMSMaxLength(70);
	string sendlength = GetIniFileString("SMSWebConfig", "Length", "", "smsconfig.ini");
	if (!sendlength.empty())
		sscanf(sendlength.c_str(),"%d",&nSMSMaxLength);
	
	/*
	//返回值，如果返回了该值，则表示发送正确	
	string strTmp;
	strTmp = GetIniFileString("SMSWebConfig", "ReturnValue", "", "smsconfig.ini");
	if (!strTmp.empty())
		RetValue=strTmp;
	*/

	string strTmp;
	//构造从短信服务器发送的头
	//替换用户名和密码	
	strTmp = CAlertMain::ReplaceStdString(strWebHead, "@UserName@", User);
	strWebHead=strTmp;
	strTmp = CAlertMain::ReplaceStdString(strWebHead, "@Pwd@", Pwd);
	strWebHead=strTmp;
	//替换手机号码
	strTmp = CAlertMain::ReplaceStdString(strWebHead, "@Phone@", szSmsTo);
	strWebHead=strTmp;
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
	string strSend;
	//将短信截断
	CAnalyzer4_bstr_t Analyzer;
	_bstr_t strMsgs = strAlertContent.c_str();
	OutputDebugString(strAlertContent.c_str());
	//	int nMaxSMSLen = MAX_SMS_LENGTH;
	//修改为用户设定的值

	// 对原始短信正文进行宽字符分段
	int nPage = Analyzer.Analyzer( strMsgs, nSMSMaxLength );
//	sprintf(dechar, "npage = %d; len=%d", nPage, strAlertContent.length());
//	OutputDebugString(dechar);

	for (int i = 0; i < nPage; i++)
	{
		char buf[300] = {0};
		_bstr_t strMsg = Analyzer.GetResult(i);
		strcpy(buf, strMsg);
		listSms.push_back(buf);
	}

	for(listSmsItem = listSms.begin(); listSmsItem!=listSms.end(); listSmsItem++)
	{
		char buf[1024] = {0};		
		//替换内容
		string url = urlEncoding(*listSmsItem);
		strTmp = CAlertMain::ReplaceStdString(strWebHead, "@Content@", url );
		strSend=strTmp;
		GetSourceHtml(strSend.c_str(), buf);
		//判断是否发送正常
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


/*
	CAnalyzer4_bstr_t Analyzer;
	_bstr_t strMsgs = strAlertContent.c_str();
	OutputDebugString(strAlertContent.c_str());
//	int nMaxSMSLen = MAX_SMS_LENGTH;
	//修改为用户设定的值
	int nMaxSMSLen = CAlertMain::nSMSSendLength;
	char * buf = new char[300];

	// 对原始短信正文进行宽字符分段
	int nPage = Analyzer.Analyzer(strMsgs, nMaxSMSLen);
	sprintf(dechar, "npage = %d; len=%d", nPage, strAlertContent.length());
	OutputDebugString(dechar);
	for (int i = 0; i < nPage; i++)
	{
		memset(buf, 0, 300);
		_bstr_t strMsg = Analyzer.GetResult(i);
		strcpy(buf, strMsg);
		listSms.push_back(buf);
	}

/***    张杰文修改
		为适应无锡农行的格式进行的修改
		发送固定格式报文到指定服务地址、端口，报文格式为：
			4位报文长度(20位手机号码(4位短信长度	短信内容))
			注：1、短信内容最长160位。
			2、服务地址、端口可配置。
*******/
/*
	int nMessageLens=0;//提出报文长度
	int nSMSLens=0;//短信长度
	char SMSHead[28];//28位的短信头
	char chtemp[20];//28位的短信头
	memset(SMSHead, 32, 28);//设置为空格
	FillMemory(SMSHead,28,' ');
	int nsize1=szSmsTo.size();
	//在对应的位置填上手机号码
	strcpy(chtemp,szSmsTo.c_str());
	strncpy(SMSHead+4+20-nsize1,chtemp,20);
	//填入短信内容
	printf("\n SMSHead=%s\n",SMSHead);

	for(listSmsItem = listSms.begin(); listSmsItem!=listSms.end(); listSmsItem++)
	{
//		string strSMS = urlEncoding(*listSmsItem);
		string strSMS = (*listSmsItem);
		char chtemp[4];
		OutputDebugString((*listSmsItem).c_str());
		memset(buf, 0, 300);
		//填入短信长度
		sprintf(chtemp,"%4d",strSMS.size());
		strncpy(SMSHead+24,chtemp,4);
		//填入报文长度
		sprintf(chtemp,"%4d",strSMS.size()+24);
		strncpy(SMSHead,chtemp,4);
		string sendUrl = CAlertMain::strSMSSendURL+":"+CAlertMain::strSMSSendPort+SMSHead+strSMS;
		printf("\n SMSHead=%s\nURL=%s\n",SMSHead,sendUrl.c_str());
		DebugePrint(sendUrl);
		OutputDebugString(sendUrl.c_str());
		GetSourceHtml(sendUrl.c_str(), buf);
		OutputDebugString(buf);
		DebugePrint(buf);
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
*/
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

	printf("=====call SendSmsFromSelfDefine。 strDllName:%s,strDllFuncParam:%s\n",strDllName.c_str(),strDllFuncParam.c_str());
	
	if((strDllName == "") || (strDllFuncParam == ""))
	{
		return false;
	}

	printf("=====call SendSmsFromSelfDefine。 \n");

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
	if (pos >= 0)
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
	DebugePrint(path);
	OutputDebugString("\n");

	bool bSucess = true;
	bool bAllSucess = true;
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

				printf("=====Send Mesage:Phone:%s,Msg:%s\n",strSmsTo.GetBuffer(),strAlertContent.c_str());

				int ret = 0;

				try
				{
					//ret=smssend1((char*)strDllFuncParam.c_str(), strSmsTo.GetBuffer(), (char*)strAlertContent.c_str());
					ret = Run_DLL(smssend1,(char*)strDllFuncParam.c_str(),strSmsTo.GetBuffer(),(char*)strAlertContent.c_str());
				}catch(...)
				{
					printf("call %s run error!\n", path);
					throw -1001;
				}
				if (ret==1)
					bSucess = true;
				else
					bSucess = false;
				//CAlertMain::SendSmsFromComm(strSmsTo, "This is com test1");
			}			
		}
	}

	if(hMod != 0)
	{
		FreeLibrary(hMod);
	}

	return bSucess;
}

int CAlertSmsSendObj::Run_DLL(RUNFUNC p,char *param, char *to, char *sms)
{
	int ret = 0;
	__try
	{
		ret = (*p)(param,to,sms);
	}__except(EXCEPTION_EXECUTE_HANDLER)
	{
		throw -1;
	}
	return ret;
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

CAlertScriptSendObj::~CAlertScriptSendObj()
{
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
	//modyfy by jiewen.zhang，增加判断，当监视器不存在时，不发送告警
	string stmp=CAlertMain::GetMonitorTitle(strAlertMonitorId);
	if(stmp.empty())
		return false;

	string strDebugInfo = "";
	strDebugInfo += "\r\n------------------发送脚本报警开始-----------------------------\r\n";

	bool bSucess = false;
	try
	{
		if(strServerTextValue.find("_win") != -1 || strServerTextValue == "127.0.0.1")
		{
			GetUserInfoFromServer();
			//string strTmp = strServerTextValue.substr(0, strServerTextValue.length() - 6);
			//strServerTextValue = strTmp;

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
            FindNodeValue(mainnode, "_MachineName", szValue);
			strServerTextValue = szValue;

            szValue = "";
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
	
	CoInitialize( NULL );
	CAlertMain::InitScriptAlertCom();

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


	OutputDebugString("\n服务器信息：");
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

			OutputDebugString("\n返回结果：");
			OutputDebugString(strResult);
			DebugePrint( "返回结果：" );
			DebugePrint( strResult.GetBuffer() );

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
			
			strSpt.Format("cscript %s\\cscripts\\%s %s",strInstallPath,strScriptFile,strScriptParam);
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

	CAlertMain::UnloadScriptAlertCom();
	CoUninitialize();

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

	szQuery += "_Script=" + strScriptFileValue + "\v";
	szQuery += "_Parameters=" + strScriptParamValue + "\v";

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
	string stmp=CAlertMain::GetMonitorTitle(strAlertMonitorId);
	if(stmp.empty())
		return false;

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
	CoInitialize(NULL);
	CAlertMain::InitSoundAlertCom();

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
		strServer = "";
		strUser = "";
		strPwd = "";
	}

	OutputDebugString("\n服务器信息：");
	OutputDebugString(strServer.GetBuffer());
	OutputDebugString(strUser.GetBuffer());
	OutputDebugString(strPwd.GetBuffer());
	
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
		
	strReturn = strResult;
	VariantClear(&a);
	VariantClear(&b);
	VariantClear(&c);
	VariantClear(&d);
	VariantClear(&e);
	VariantClear(&vResult);

	CAlertMain::UnloadSoundAlertCom();
	CoUninitialize();
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


//
std::map<std::string,DWORD> CAlertItsmSendObj::m_mapMonitorCache;

CAlertItsmSendObj::CAlertItsmSendObj()
{	
	szSchedule = "";
	szMailTo = "";
	nType = 1;
	
}

//
CAlertItsmSendObj::~CAlertItsmSendObj()
{

}


void CAlertItsmSendObj::String2Array(std::vector<std::string> &vOut,string strIn,string strDiv)
{
	int pos1= 0,pos2 = 0;

	vOut.clear();

	if (strIn.size() <=0)
		return;

	while(1)
	{
		pos2 = strIn.find(strDiv, pos1);

		if (pos2 < 0)
		{
			vOut.push_back(strIn.substr(pos1, strIn.length() -pos1));
			break;
		}
		
		vOut.push_back(strIn.substr(pos1, pos2 -pos1));
		
		pos1 = pos2 +1;
		pos2 = pos1;

	}
	
}

//获取服务器地址、用户名、密码等参数
bool CAlertItsmSendObj::GetItsmConfig()
{
	//#ifdef IDC_Version
/*	if(GetCgiVersion().compare("IDC") == 0)
	{
		//获得URL地址		
		strURL == GetIniFileString("Ofibiz", "URL", "" , "OfbizConfig.ini");
		//获得用户名
		strUserName == GetIniFileString("Ofibiz", "UserName", "" , "OfbizConfig.ini");
		//获得密码
		strPWD == GetIniFileString("Ofibiz", "PWD", "" , "OfbizConfig.ini");
	}
	else
	{
		//获得URL地址		
		strURL == GetIniFileString("Ofibiz", "URL", "" , "OfbizConfig.ini");
		//获得用户名
		strUserName == GetIniFileString("Ofibiz", "UserName", "" , "OfbizConfig.ini");
		//获得密码
		strPWD == GetIniFileString("Ofibiz", "PWD", "" , "OfbizConfig.ini");
	}
	*/

	//char szTmp[513];
	//char szPath[MAX_PATH+1];
	//char szConfigFile[MAX_PATH+1];
	char szURL[1024];
	//int  iPort;

	//::GetModuleFileName(NULL,szPath, MAX_PATH);
	//::PathRemoveFileSpec(szPath);
	//sprintf(szConfigFile,"%s\\mc.config", szPath);

	//GetPrivateProfileString("itsm_server","itsmServer","",szTmp,512,szConfigFile);
	//iPort = GetPrivateProfileInt("itsm_server", "itsmPort",0,szConfigFile);

	//if (strlen(szTmp) <=0)
	//	return false;

	//sprintf(szURL,"http://%s:%d/itsm/control/JSonService", szTmp, iPort);

	//strURL = szURL;


	string ip = GetIniFileString("itsm_server","itsmServer", "" , "itsmConfig.ini");
	int port = GetIniFileInt("itsm_server","itsmPort",0 , "itsmConfig.ini");

	sprintf(szURL,"http://%s:%d/itsm/control/JSonService", ip.c_str(), port);

	strUserName = "itsm_user";
	strPWD = "!@72Aa";

	strURL = szURL;

	if(strURL.empty() || strUserName.empty())
		return false;
	else 
		return true;
}


string&   replace_all(string&   str,const   string&   old_value,const   string&   new_value)   
{
	while(true)   {
		string::size_type   pos(0);   
		if((pos=str.find(old_value))!=string::npos)   
			str.replace(pos,old_value.length(),new_value);
		else   
			break;
	}   
	return   str;   
}

//发送报警
bool CAlertItsmSendObj::SendAlert()
{	
	char sendbuff[4096];
	std::vector<string> vAssignee;
	CBase64 base64;
	int iTryCount = 0;
	bool noassign = false;
	
	m_mapMonitorCache[strAlertMonitorId] = GetTickCount();

	//modyfy by jiewen.zhang，增加判断，当监视器不存在时，不发送告警
	string stmp=CAlertMain::GetMonitorTitle(strAlertMonitorId);
	string strDev =  FindParentID(strAlertMonitorId);
	
	//查找第二个"."以前的部分,就是组ID(et:1.23)
	string::size_type pos = strAlertMonitorId.find(".");
	if (pos > 0)
	{
		pos = strAlertMonitorId.find(".",pos+1);
	}

	string strGroupID = strAlertMonitorId.substr(0,pos);
	string strGroupTitle = CAlertMain::GetGroupTitle(strGroupID);

	if (strGroupTitle.size() <= 0)
		strGroupTitle = "其他设备";

	string strIssueType = "18";


	if(stmp.empty() || strDev.empty())
	{
		printf("***检测器不存在？\n");
		return false;
	}
	WriteLog("\n\n====================CAlertItsmSendObj::SendAlert=======================");

	//读web.ini获取发送地址等发送参数
	// web 服务器
	//获得服务器的参数
	GetItsmConfig();
	MakeAlertTitle();
	MakeAlertContent();
	//strURL="http://192.168.0.181:18080/itsm/control/JSonService";
	//发送URL
	CInternetSession session;
	CHttpFile*   fileGet= NULL;
	string strData;
	string strKey;
	string strToken;
	string content;
	std::vector<std::string> vStatus;

	try
	{
		//登录
		strData = "{\"dowhat\":\"login\",\"password\":\"" + strPWD + "\",\"username\":\"" + strUserName +"\"}";

		if (!requestJSONAction(&session,strURL,strData,content))
		{
			printf("****登录 itsm 系统失败!\n"); 
			goto eh;
		}

		if (content.size() <=0)
		{
			printf("****登录 itsm 系统失败:%s\n",content.c_str()); 
			goto eh;
		}

		json_spirit::Value value;

		json_spirit::read(content,value);
		json_spirit::Object root_obj( value.get_obj() );
		Object::const_iterator it=root_obj.begin();
		if( it == root_obj.end() ) 
		{
			printf("itsm json 认证失败:%s\n",content.c_str());
			goto eh;
		}
		strKey=it->name_;
		//返回的result后面即为令牌
		if (strKey=="result")
			strToken=(it->value_).get_str();
		else
		{
			printf("itsm json 认证失败:%s\n",content.c_str());
			goto eh;
		}

		//查询组合条件
		sprintf(sendbuff,"{\"dowhat\":\"findAllAlertCondition\",\"token\":\"%s\"}",strToken.c_str());
		if (!requestJSONAction(&session,strURL,string(sendbuff),content))
		{
			printf("itsm json 查询组合条件失败.");
			goto eh;
		}
		//printf("查询组合条件结果:%s\n", content.c_str());

		//content ="{\"result\":[{\"id\":\"1\", \"description\":\"{1.9.5.1} OR {1.9.5.2} AND ({1.9.5.2} OR {1.9.5.1})\",\"displayText\":\"Memory AND Process：AlertServer AND Process：java\",\"conditionName\":\"1\",\"isUse\":true}]}";

		if (!MatchCondition(strAlertMonitorId,content))
		{
			printf("***组合条件匹配不成功\n");
			goto exit;
		}

		//查处所有getStatuses
		/*sprintf(sendbuff,"{\"dowhat\":\"getStatuses\",\"token\":\"%s\"}",strToken.c_str());
		if (!requestJSONAction(&session,strURL,string(sendbuff),content))
		{
			printf("itsm json 查询状态列表.");
			goto eh;
		}
		printf("查询状态列表结果:%s\n", content.c_str());
		*/
		

		//查询改设备是不是有工单了
		
		vStatus.push_back("3");
		vStatus.push_back("1");
		vStatus.push_back("4");
		vStatus.push_back("5");
		vStatus.push_back("6");
		vStatus.push_back("10002");
		for (std::vector<std::string>::iterator is = vStatus.begin(); is != vStatus.end(); is++)
		{
			//sprintf(sendbuff,"{\"dowhat\":\"getIssuesByProjectKeyAndDeviceId\",\"token\":\"%s\",\"projectKey\":\"ECC\",\"id\":\"%s\"}",
			//	strToken.c_str(),strAlertMonitorId.c_str());
			sprintf(sendbuff,"{\"dowhat\":\"getIssuesByProjectKeyAndDeviceIdAndStatus\",\"token\":\"%s\",\"projectKey\":\"ECC\",\"id\":\"%s\",\"status\":\"%s\"}",
				strToken.c_str(),strAlertMonitorId.c_str(),is->c_str());

			printf("***查询设备的工单：%s\n",sendbuff);

			if (!requestJSONAction(&session,strURL,string(sendbuff),content))
			{
				printf("itsm json 查询工单失败.");
				goto eh;
			}

			//printf("查询工单结果:%s\n", content.c_str());

			json_spirit::read(content,value);
			json_spirit::Object root_obj( value.get_obj() );
			Object::const_iterator it=root_obj.begin();
			if( it == root_obj.end() ) 
			{
				printf("itsm json 查询工单失败:%s\n",content.c_str());
				goto eh;
			}
			strKey=it->name_;
			
			if (strKey=="result")
			{
				json_spirit::Array ar=(it->value_).get_array();
				if (ar.size() > 0) //已经有工单就添加Comment
				{
					int CommentCount =0;

					for(json_spirit::Array::iterator i = ar.begin(); i!=ar.end();i++)
					{
						bool bAddComment = false;
						string strIssueKey;
						string strStatus;

						svutil::TTime tmNow = svutil::TTime::GetCurrentTimeEx();

						json_spirit::Object isobj = i->get_obj();

						for(json_spirit::Object::const_iterator j = isobj.begin(); j!=isobj.end(); j++)
						{
							strKey = j->name_;

							//printf("***strKey:%s\n", strKey.c_str());
							
							if (strKey.compare("key")==0)
							{
								strIssueKey = j->value_.get_str();
							}else if (strKey.compare("status")==0)
							{
								strStatus = j->value_.get_str();

							}else if (strKey.compare("updated")==0)
							{
								json_spirit::Object tmobj = j->value_.get_obj();
								
								svutil::TTimeSpan tsTm(1,0,0,0);
								char *cEnd = NULL;
								int iYear =0,iMonth=0,iDay=0,iHour=0,iMinute=0;

								for(json_spirit::Object::const_iterator k = tmobj.begin(); k!=tmobj.end(); k++)
								{
									string strName;
									string strVal;
									strName = k->name_;
									//strVal = k->value_.get_str();

									//printf("***strName:%s,strVal:%d\n",strName.c_str(),k->value_.get_int());
									//continue;

									if (strName.compare("year") == 0)
									{
										iYear = k->value_.get_int() +1900;
									}else if(strName.compare("month") == 0)
									{
										iMonth = k->value_.get_int()+1;
									}else if(strName.compare("date") == 0)
									{
										iDay = k->value_.get_int();
									}else if(strName.compare("hours") == 0)
									{
										iHour = k->value_.get_int();
									}else if(strName.compare("minutes") == 0)
									{
										iMinute = k->value_.get_int();
									}
								}

								//printf("***year:%d,month:%d,date:%d,hours:%d,minutes:%d\n",iYear,iMonth,iDay,iHour,iMinute);

								svutil::TTime tmIssue(iYear,iMonth,iDay,iHour,iMinute,0);
								
								tmIssue = tmIssue + tsTm;

								if (tmNow > tmIssue) //
									bAddComment = true;
							}
						}

						if (strStatus.compare("10001")==0) //是已关闭工单
						{
							continue;
						}

						++CommentCount;

						printf("****bAddComment:%s,strIssueKey:%s\n",bAddComment ? "true":"false",strIssueKey.c_str());
						if (bAddComment && strIssueKey.size()>0) //添加Comment
						{
							string strSend;

							//char szBuf[4*1024];
							json_spirit::Object obj1;

							sprintf(sendbuff,"{\"year\":\"%d\",\"month\":\"%d\",\"date\":\"%d\",\"hours\":\"%d\",\"minutes\":\"%d\",\"seconds\":\"%d\"}",
								tmNow.GetYear(),tmNow.GetMonth(),tmNow.GetDay(),tmNow.GetHour(),tmNow.GetMinute(),tmNow.GetSecond());

							obj1.push_back( Pair("body", strAlertContent) );
							obj1.push_back( Pair( "author", strUserName ) );
							obj1.push_back( Pair( "created", string(sendbuff)) );
		
							string strComment =json_spirit::write_formatted( obj1);

							strSend = "{\"dowhat\":\"addComment\",\"token\":\"" +strToken + "\",\"issueKey\":\"" + strIssueKey;
							strSend += "\",\"remoteComment\":"+strComment + "}";								

							
							//sprintf(szBuf,"{\"dowhat\":\"addComment\",\"token\":\"%s\",\"issueKey\":\"%s\",\"remoteComment\":{\"body\":\"%s\",\"author\":\"%s\",\"created\":{\"year\":\"%d\",\"month\":\"%d\",\"date\":\"%d\",\"hours\":\"%d\",\"minutes\":\"%d\",\"seconds\":\"%d\"}}}",
							//	strToken.c_str(),strIssueKey.c_str(),strAlertContent.c_str(),strUserName.c_str(),
							//	tmNow.GetYear(),tmNow.GetMonth(),tmNow.GetDay(),tmNow.GetHour(),tmNow.GetMinute(),tmNow.GetSecond());
							//printf("***addComment:%s\n", strSend.c_str());
							if (!requestJSONAction(&session,strURL,strSend,content))
							{
								printf("itsm json 添加工单Comment失败.");
								goto eh;
							}

							printf("***添加工单Comment结果:%s\n", content.c_str());
						}
						
					}

					if (CommentCount > 0)
						goto exit;
				}
			}
			else
			{
				printf("itsm json 查询工单失败:%s\n",content.c_str());
				goto eh;
			}
			
		}
		

		//取出设备的责任人
		sprintf(sendbuff,"{\"dowhat\":\"getUserNamesByDeviceId\",\"token\":\"%s\",\"deviceId\":\"%s\"}",
			strToken.c_str(),strDev.c_str());
		
		if (!requestJSONAction(&session,strURL,string(sendbuff),content))
		{
			printf("取出设备的责任人失败！\n");
			goto eh;		
		}

		//content=UTF_82ASCII(content);

		content = replace_all(content,"\"","");

		if (content.find("result") == std::string.npos)
		{
			printf("取出设备的责任人失败:%s\n",content.c_str());
			goto eh;
		}

		content = replace_all(content,"{result:[","");

		content = replace_all(content,"]}","");

		printf("***责任人:%s\n",content.c_str());

		String2Array(vAssignee,content,",");

		if (vAssignee.size() <=0) //没有设备责任人，默认为admin
		{
			vAssignee.push_back("admin");
			noassign = true;
		}
		//

		//取设备类型
		sprintf(sendbuff,"{\"dowhat\":\"getIssueTypeByName\",\"token\":\"%s\",\"name\":\"%s\"}",strToken.c_str(),strGroupTitle.c_str());
		if (!requestJSONAction(&session,strURL,string(sendbuff),content))
		{
			printf("***取工单设备类型失败！\n");
			goto eh;		
		}
		if (content.size() > 0)
		{
			json_spirit::read(content,value);
			json_spirit::Object root_obj( value.get_obj() );
			Object::const_iterator it=root_obj.begin();
			if( it != root_obj.end() ) 
			{
				strKey=it->name_;
				//返回的result后面即为令牌
				if (strKey=="result")
				{
					json_spirit::Object val=(it->value_).get_obj();

					for (Object::const_iterator i= val.begin(); i !=val.end(); i++)
					{
						strKey = i->name_;
						if(strKey == "id")
							strIssueType = (i->value_).get_str();
					}
				
				}
			}
		}
		//

		//创建工单
		for (std::vector<std::string>::iterator j = vAssignee.begin(); j!= vAssignee.end(); j++)
		{

			printf("***为User:%s创建工单\n", j->c_str());
			string strIssueKey;

			string strContent1;
			//第二步，利用令牌发送内容

			SYSTEMTIME sysTime;
			GetLocalTime(&sysTime);
			
			//把内容转换成json格式
			string jsonTitle,jsonContent;
			json_spirit::Object obj1;
			string stTime,strIssue,strHead;
			//加工时间
			obj1.push_back( Pair( "minutes", sysTime.wMinute ) );
			obj1.push_back( Pair( "hours", sysTime.wHour ) );
			obj1.push_back( Pair( "seconds", sysTime.wSecond ) );
			obj1.push_back( Pair( "year", sysTime.wYear ) );
			obj1.push_back( Pair( "month", sysTime.wMonth ) );
			obj1.push_back( Pair( "date", sysTime.wDay ) );
			stTime =json_spirit::write_formatted( obj1);
			//加入内容
			obj1.clear();

			//printf("============\n%s\n==============\n", strAlertContent.c_str());
			
			obj1.push_back( Pair("summary", strAlertTitle) );
			obj1.push_back( Pair( "description", strAlertContent ) );
			obj1.push_back( Pair( "reporter", strUserName) );
			obj1.push_back( Pair( "deviceid", strAlertMonitorId) );
			obj1.push_back( Pair( "assignee", *j) );
			obj1.push_back( Pair( "project", "ECC") );
			obj1.push_back( Pair( "type", strIssueType.c_str()/*"1"*/) );
			obj1.push_back( Pair( "priority", "1") );
			//			obj1.push_back( Pair( "duedate", stTime) );

			strIssue =json_spirit::write_formatted( obj1);
			//去掉后面的一个“｝”
			strIssue=strIssue.substr(0,strIssue.length()-1);
			obj1.clear();
			//加入头
			obj1.push_back( Pair( "dowhat", "createIssue" ) );
			obj1.push_back( Pair( "token", strToken ) );
			strHead ="{\"dowhat\":\"createIssue\",\"token\":\"";
			strHead=strHead+strToken+"\",\"rIssue\":";


			string strSend;
			strSend=strHead+strIssue+",\"duedate\":"+stTime+"}}";

			//printf("***创建工单:%s\n", strSend.c_str());

			if (!requestJSONAction(&session,strURL,strSend,content))
			{
				printf("***创建工单失败!\n");
				goto eh;		
			}

			//printf("***创建结果:%s\n",content.c_str());

			json_spirit::read(content,value);
			json_spirit::Object root_obj( value.get_obj() );
			Object::const_iterator it=root_obj.begin();
			if( it != root_obj.end() ) 
			{
				strKey=it->name_;
				
				if (strKey=="result")
				{
					json_spirit::Object val=(it->value_).get_obj();

					for (Object::const_iterator i= val.begin(); i !=val.end(); i++)
					{
						strKey = i->name_;
						if(strKey == "key")
						{
							strIssueKey = (i->value_).get_str();
							break;
						}

					}
				
				}
			}

			if (strIssueKey.size() <= 0)
			{
				printf("***创建工单失败:%s\n",content.c_str());
			}

			if (strIssueKey.size() > 0 && !noassign)
			{
				//设置工单为已分配
				sprintf(sendbuff,"{\"dowhat\":\"progressWorkflowAction\",\"token\":\"%s\",\"issueKey\":\"%s\",\"actionIdString\":\"11\",\"actionParams\":[{\"id\":\"assignee\",\"values\":[\"%s\"]}]}",
					strToken.c_str(),strIssueKey.c_str(),j->c_str());
				requestJSONAction(&session,strURL,string(sendbuff),content);
				
			}

			
			//

			

		}
exit:
		//退出
		sprintf(sendbuff,"{\"dowhat\":\"logout\",\"token\":\"%s\"}",strToken.c_str());

		requestJSONAction(&session,strURL,string(sendbuff),content);

		printf("***自动创建工单结束。\n");

	}catch(...)
	{
		printf("***自动创建工单过程产生异常。\n");
		return false;
	}
	return true;

eh:
	try
	{
		
		Sleep(50);

		session.Close();

	}catch(...)
	{
		printf("***关闭Internet Session失败!\n");
	}
	return true;
}

//
string CAlertItsmSendObj::GetDebugInfo()
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


//按模板生成报警标题
//modify by jiewen.zhang on 08-11-06
void CAlertItsmSendObj::MakeAlertTitle()
{
	string strTmp;
	//从短信模板中获得模板参数
	//strAlertTitle = GetIniFileString("Email", "Default", "", "TxtTemplate.Ini");
	//int nLength = strAlertTitle.length();
	//int nPos=strAlertTitle.find("&");
	//如果找到了‘&’
	//if (nPos!=string::npos)
	//{
	//	strAlertTitle=strAlertTitle.substr(0,nPos);
	//}
	//else//如果没找到
	//	strAlertTitle = "警报来自SiteView。@Device@:@monitor@";//将标题设置为空


	//if (strAlertTitle.size() <= 0) //添加标题
	{
		strAlertTitle = "警报来自SiteView。" ;
		strAlertTitle += CAlertMain::GetDeviceTitle(strAlertMonitorId);
		strAlertTitle += "：";
		strAlertTitle += CAlertMain::GetMonitorTitle(strAlertMonitorId);
		return;
	}

	string strDeviceId = FindParentID(strAlertMonitorId);

	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@Group@", CAlertMain::GetDeviceTitle(strAlertMonitorId));
	strAlertTitle = strTmp;

	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@monitor@", CAlertMain::GetMonitorTitle(strAlertMonitorId));
	strAlertTitle = strTmp;

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Monitor@", CAlertMain::GetMonitorTitle(strAlertMonitorId));
	strAlertTitle = strTmp;

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Device@", CAlertMain::GetDeviceTitle(strAlertMonitorId));
	strAlertTitle = strTmp;

	string strFullPathGroup = CAlertMain::GetAllGroupTitle(strDeviceId) + CAlertMain::GetDeviceTitle(strAlertMonitorId);
	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@FullPathGroup@", strFullPathGroup);
	strAlertTitle = strTmp;

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@MonitorAlertDes@", CAlertMain::GetMonitorParameterValue(strAlertMonitorId,"sv_description"));
	strAlertTitle = strTmp;

	
	//string strGrouptId = FindParentID(strDeviceId);
	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@AllGroup@", CAlertMain::GetAllGroupTitle(strDeviceId));
	strAlertTitle = strTmp;

	//添加状态
	//strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Status@", CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_stateString"));
	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@Status@", strEventDes);	
	strAlertTitle = strTmp;

	//添加时间	
	strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@Time@", strTime);	
	strAlertTitle = strTmp;

	//加进程名
	string strMonitorType = CAlertMain::GetMonitorPropValue(strAlertMonitorId, "sv_monitortype");	
	if(strMonitorType == "14" || strMonitorType == "33" || strMonitorType == "41"
		|| strMonitorType == "111" || strMonitorType == "174"  || strMonitorType == "175")
	{
		// 14 Service  33 Nt4.0Process  41 Process  111 UnixProcess  174 SNMP_Process  175 SNMP_Service
		//strAlertTitle += " ";		

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
		strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@Process@", strProcName);	
		strAlertTitle = strTmp;

		//CloseResource(paramNode);
		CloseMonitor(hMon);

	}

	/*
	//加空格
	strAlertTitle +=" ";
	//加状态
	//1、正常。2、危险。3、错误。4、禁止。5、错误。 warnning( error ..) 
	if(nEventType == 1)
	{
	strAlertTitle += "Ok";
	}
	else if(nEventType == 2)
	{
	strAlertTitle += "Warnning";
	}
	else if(nEventType == 3)
	{
	strAlertTitle += "Error";
	}
	else if(nEventType == 4)
	{
	strAlertTitle += "Disable";
	}
	else if(nEventType == 5)
	{
	strAlertTitle += "Disable";
	}
	else
	{
	strAlertTitle += "Error";
	}	
	*/


}

//将阈值变量的名字换成中文
bool CAlertItsmSendObj::ToChinese(string& strDes,const string strSour)
{
	int ret = false;

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
	//CloseResource(hTemplet);
	CloseMonitorTemplet(hTemplet);
	CloseMonitor(hMon);
	

	return ret;
}


//生成报警内容
void CAlertItsmSendObj::MakeAlertContent()
{
	string strTmp;
	//从模板中获得模板参数
	strAlertContent = GetIniFileString("Email", "Default", "", "TxtTemplate.Ini");
	//strAlertContent = GetTemplateContent("Email", strEmailTemplateValue);
	int nPos=strAlertContent.find("&");
	//如果找到了‘&’
	int nLength = strAlertContent.length();
	if (nPos!=string::npos)
	{
		strAlertContent=strAlertContent.substr(nPos+2,nLength);
	}

	string strDeviceId = FindParentID(strAlertMonitorId);


	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Group@", CAlertMain::GetDeviceTitle(strAlertMonitorId));
	strAlertContent = strTmp;

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@monitor@", CAlertMain::GetMonitorTitle(strAlertMonitorId));
	strAlertContent = strTmp;


	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Monitor@", CAlertMain::GetMonitorTitle(strAlertMonitorId));
	strAlertContent = strTmp;

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Device@", CAlertMain::GetDeviceTitle(strAlertMonitorId));
	strAlertContent = strTmp;

	string strFullPathGroup = CAlertMain::GetAllGroupTitle(strDeviceId) + CAlertMain::GetDeviceTitle(strAlertMonitorId);
	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@FullPathGroup@", strFullPathGroup);
	strAlertContent = strTmp;

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@MonitorAlertDes@", CAlertMain::GetMonitorParameterValue(strAlertMonitorId,"sv_description"));
	strAlertContent = strTmp;


	
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
		strFreq = CAlertMain::strMontorFreq.c_str();
		strFreq += ":";
		strFreq += CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_frequency");
		strFreq += CAlertMain::strMinute.c_str();

	}
	else
	{
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
		CloseMonitorTemplet(hTemplet);


	}
	
	//CloseResource(ma);

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
	//--------------------------------------------------------------------

	/*
	WriteLog("\n\n==================CAlertEmailSendObj::MakeAlertContent==================");
	WriteLog("monitortype=");WriteLog(getvalue.c_str());
	WriteLog("errorvalue=");WriteLog(szErrorValue.c_str());
	WriteLog("Fazhi=");WriteLog(CAlertMain::strMonitorFazhi.c_str());
	*/

	//CloseResource(map);
	CloseMonitor(hMon);


	//strAlertContent += "\n阀值:     ";
	strAlertContent += "\n";
	strAlertContent += CAlertMain::strMonitorFazhi.c_str();
	strAlertContent += "       ";
	strAlertContent +=  strErrorValue;
	strAlertContent += "\n";
	//add end

	//替换@Log@ sxf 2008-12-4
	string strLogPath = GetSiteViewRootPath()+ "data\\Temp" +strAlertMonitorId;

	string strLog = CAlertMain::GetFileLastLine(strLogPath,"");

	strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Log@",  strLog);
	strAlertContent = strTmp;
	//

	////Begin
	////替换所有格式如下的 @@Key@@ ，得到Key，并得到相应的键值
	////printf("Log Start");
	//CString strMfcTemp, strTempKey, strPath,strLocalContent;	
	//strLocalContent= CString(strAlertContent.c_str());
	//CString strTempKeyValue = _T(""); 
	//while(strLocalContent.Find("@",0)>0)
	//{
	//	//printf("1");
	//	int nTemp = strLocalContent.Find("@",0);
	//	strMfcTemp = strLocalContent.Right(strLocalContent.GetLength() - nTemp - 1);
	//	//printf(strMfcTemp);
	//	strLocalContent = strLocalContent.Left(nTemp);
	//	//printf(strLocalContent);
	//	nTemp = strMfcTemp.Find("@",0);
	//	strTempKey = strMfcTemp.Left(nTemp);
	//	strMfcTemp = strTempKey;
	//	//printf(strMfcTemp);
	//	//strMfcTemp = strMfcTemp.Right(strMfcTemp.GetLength()-nTemp-1);
	//	//strTempKeyValue=FuncGetProfileStringBy(strSection,strTempKey,strGroupFile);
	//	//printf(strMfcTemp);
	//	if(strMfcTemp == "Log")
	//	{
	//		//printf("Entenr Log");
	//		//文件名称
	//		strPath.Format("%s\\%s\\%s.txt", GetSiteViewRootPath().c_str(), "data\\Temp", strAlertMonitorId.c_str());

	//		//printf(strPath);

	//		//取出该次报警对应的数据并替换@@Log@@
	//		//根据strTime判断， 如果最近一次时间大于。。。 如果最近一次时间小于。。。
	//		int nFileLength = 0;
	//		CFile* pFile = NULL;
	//		TRY	
	//		{
	//			pFile = new CFile(strPath, CFile::modeRead | CFile::shareDenyNone);
	//			nFileLength = pFile->GetLength();
	//		}
	//		CATCH(CFileException, pEx)	
	//		{
	//			if (pFile != NULL) 
	//			{
	//				pFile->Close();
	//				delete pFile;
	//			}

	//			return;
	//		}
	//		END_CATCH

	//			if (pFile != NULL) 
	//			{
	//				pFile->Close();
	//				delete pFile;
	//			}

	//			if (0 == nFileLength)
	//				return;

	//			CStringEx strTotleContent = _T("");
	//			CStringEx strLogContent = _T("");
	//			FILE * fp = fopen((LPCTSTR)strPath, "r");
	//			if (fp)
	//			{
	//				char * buffer = NULL;
	//				buffer = (char*)malloc(nFileLength + 1);
	//				if (buffer) 
	//				{
	//					memset(buffer, 0, nFileLength + 1);
	//					fread(buffer, sizeof(char), nFileLength + 1, fp);
	//					strTotleContent.Format("%s", buffer);	
	//					free(buffer);
	//				}
	//				fclose(fp);
	//			}

	//			if(strTotleContent != "")
	//			{
	//				int nStart = strTotleContent.ReverseFind("[Time is", -1);
	//				//strLogContent = strTotleContent.Right(strTotleContent.GetLength() - nStart - 2);
	//				strLogContent = strTotleContent.Right(strTotleContent.GetLength() - nStart);
	//				//DebugePrint("ReverseFind:\n");
	//				//DebugePrint(strLogContent);
	//				//printf(strLogContent);
	//				string strStdLogContent = strLogContent;
	//				//DebugePrint(strStdLogContent.c_str());
	//				strTmp = CAlertMain::ReplaceStdString(strAlertContent, "@Log@",  strStdLogContent);	
	//				strAlertContent = strTmp;
	//			}
	//	}

	//	//printf("Key %s Value is %s\n",strTempKey,strTempKeyValue);

	//	//if(!strTempKeyValue.IsEmpty())
	//	//{			
	//	//	strLocalContent=strLocalContent+strTempKeyValue+strTemp;
	//	//}
	//	//else
	//	//{
	//	//	strLocalContent=strLocalContent+"无设定"+strTemp;
	//	//}
	//}
	////End
	////printf("Log End");
}

//
void CAlertItsmSendObj::SetUpgradeTrue()
{
	bUpgrade = true;
}



bool CAlertItsmSendObj::requestJSONAction(CInternetSession *sess,string& url,string& req,string& reply)
{
	string strContent;
	string content;
	CString data;
	CBase64 base64;
	int iTryCount = 0;
	DWORD dwStatusCode;
	CHttpFile* fileGet =NULL;

	reply = "";

	try
	{

		fileGet= (CHttpFile*) sess->OpenURL(url.c_str()); 
	}catch(...)
	{
		printf("连接到指定的URL错误：%s\n",url.c_str());
		if (fileGet)
			fileGet->Close();

		return false;
	}

	if (!fileGet)
		return false;

	try
	{
		//req = ::GB2312ToUTF8(req);
		strContent = "jsondata=" + base64.Encode(req.c_str(),req.size());
		
		fileGet->SendRequest("",0,(LPVOID)(LPCTSTR)strContent.c_str(),strContent.size());
		fileGet -> QueryInfoStatusCode(dwStatusCode);
		if(dwStatusCode == HTTP_STATUS_OK)   
		{
			iTryCount = 0;
			while (fileGet -> ReadString(data)&& iTryCount++ < 10)   
			{
				content  += data;
			}
		}
	}catch(...)
	{
		printf("Wininet SendRequest error:%u\n",GetLastError());

		if (fileGet)
			fileGet->Close();

		return false;
	}

	if( content.size() <= 0)
	{
		fileGet->Close();
		return false;
	}

	fileGet->Close();

	//base64 解码
	char *szBuf = new char[content.size()*3/2];
	ASSERT(szBuf != NULL);
	memset(szBuf, 0, content.size()*3/2);
	base64.Decode(content.c_str(), szBuf);
	reply = szBuf;
	delete[] szBuf;

	return true;
}
class boolexpression 
{
public:
	//计算操作结果
	char cal(char opz,char op, char opn)
	{
		//printf("****cal:%c,%c,%c\n", opz,op,opn);
		switch(op)
		{
		case '&':
			if (opz == '1' && opn == '1')
				return '1';
			else
				return '0';	
		case '|':
			if (opz == '1' || opn =='1')
				return '1';
			else
				return '0';
		default:
			return 0;
		}
		return 0;


	}

	//取操作符的优先级
	int op_level(char op)
	{
		switch(op)
		{
		case '&':
			return 2;
		case '|':
			return 1;
		default:
			return 0;
		}
	}
	//通过表达是计算出结果
	char result(const char *expression)
	{
		int i = 0;

		std::string  strdesc = expression;
		
		char op = 0;
		char opz = 0;
		char opz2 = 0;
		std::stack<char> opstack;
		std::stack<char> opzstack;

		char result = 0;
		
		while(i < strdesc.size())
		{
			char c= strdesc[i];
			++i;

			if (c == ' ')
				continue;

			if (c =='(' || c == ')')
			{
				if (c == '(')
				{
					if (opz == 0)
					{
					}else if (op == 0)
					{
						if (opz  != 0)
						{
							printf("***condition statement error at:%d\n",i);
							break;
						}
					}else
					{
						if (opz2 == 0)
						{
							opzstack.push(opz);
							opstack.push(op);
							opz = 0;
							op = 0;
						}else
						{
							printf("***condition statement error at:%d\n",i);
							break;
						}
					}

					opstack.push(c); //括号入栈
				}else
				{
					//处理")"
					if (opstack.size() <= 0)
					{
						printf("***condition statement error at:%d\n",i);
						break;
					}

					if (op == 0)
					{
						op = opstack.top();
						opstack.pop();

						if (op == '(')
						{
							op = 0;
							continue;
						}else
						{
							opz2 = opz;
							
							if (opzstack.size() <= 0)
							{
								printf("***condition statement error at:%d\n",i);
								break;
							}
							opz = opzstack.top();
							opzstack.pop();
						}
					}
					{

						if (opz2 == 0)
						{
							printf("***condition statement error at:%d\n",i);
							break;
						}
						
						while(1)
						{
							if (opstack.size() <= 0)
							{
								opz = cal(opz,op,opz2);
								op = 0;
								opz2 = 0;
								break;
							}

							char ops = opstack.top();

							if (ops == '(')
							{
								opz = cal(opz,op,opz2);
								op = 0;
								opz2 = 0;
								opstack.pop();
								break;
							}

							if (opzstack.size() <= 0)
							{
								printf("***condition statement error at:%d\n",i);
								break;
							}

							char opzs = opzstack.top();
							opstack.pop();
							opzstack.pop();

							if (op_level(ops) >= op_level(op))
							{
								opz = cal(opzs,ops,opz);
								
							}else 
							{
								opz = cal(opz,op,opz2);
								opz2 = opz;
								op = ops;
								opz = opzs;
							}
						}
					}
				}

			}else if (c == '&' || c == '|')
			{	
				if (opz == 0)
				{
					printf("***condition statement error at:%d\n",i);
					break;
				}else if (op == 0)
				{
					op = c;
				}else if (opz2 == 0)
				{
					printf("***condition statement error at:%d\n",i);
					break;
				}else
				{
					while(1)
					{
						if (opzstack.size() <= 0)
						{
							if (op_level(op) >= op_level(c))
							{
								opz = cal(opz,op,opz2);
								op = c;
								opz2 = 0;
							}else
							{
								opstack.push(op);
								opzstack.push(opz);
								op = c;
								opz = opz2;
								opz2 = 0;
							}
							break;
						}else 
						{
							char ops = opstack.top();
							char opzs = opzstack.top();
							if (ops == '(')
							{
								opstack.push(op);
								opzstack.push(opz);
								opz = opz2;
								op = c;
								opz2 = 0;
								break;

							}else
							if (op_level(ops) >= op_level(op))
							{
								opz = cal(opzs,ops,opz);
								opstack.pop();
								opzstack.pop();
								continue;
							}else if (op_level(op) >= op_level(c))
							{
								opz = cal(opz,op,opz2);
								op = c;
								opz2 = 0;
								break;
							}else
							{
								opstack.push(op);
								opzstack.push(opz);
								opz = opz2;
								op = c;
								opz2 = 0;
								break;
							}
						}
					}

				}
			}else if (c == '1' || c == '0')
			{
				if (opz == 0)
				{
					opz = c;
				}else if (op == 0)
				{
					printf("***condition statement error at:%d\n",i);
					break;
				}else
				{
					opz2 = c;
				}
			}else
			{
				printf("***condition statement error at:%d\n",i);
				break;
			}
		}

		while(1) //处理完堆栈里的数据同时计算最后的值
		{
			if (opzstack.size() <= 0) 
			{
				if (op > 0)
				{
					opz = cal(opz,op,opz2);
					result = opz;
				}else
				{
					if (opz2 != 0)
					{
						printf("***condition statement error\n");
						break;
					}else
					{
						result = opz;
					}
				}
				break;
			}else 
			{
				char ops = opstack.top();
				char opzs = opzstack.top();

				if (op == 0)
				{
					opz2 = opz;
					op = ops;
					opz = opzs;
					opstack.pop();
					opzstack.pop();
					continue;
				}else if (op_level(ops) >= op_level(op))
				{
					opz = cal(opzs,ops,opz);
					opstack.pop();
					opzstack.pop();
					continue;
				}else 
				{
					opz = cal(opz,op,opz2);
					op = 0;
					opz2 = 0;
				}
			}
		}

		return result;
	}
};
bool CAlertItsmSendObj::MatchCondition(string strMonitorId, string strCondition)
{

	typedef std::map<string,Conditions> ConditionMap;
	ConditionMap conditions;
	
	json_spirit::Value value;
	json_spirit::read(strCondition,value);
	json_spirit::Object root_obj( value.get_obj() );
	Object::const_iterator it=root_obj.begin();
	string strKey;
	
	if(it == root_obj.end()) 
	{
		printf("***没有设置条件\n");
		//printf("itsm json 查询工单失败:%s\n",content.c_str());
		return false;
	}
	strKey=it->name_;
	
	if (strKey=="result")
	{
		json_spirit::Array ar=(it->value_).get_array();
		
		for(json_spirit::Array::iterator i = ar.begin(); i!=ar.end();i++)
		{
			Conditions cd;
			json_spirit::Object isobj = i->get_obj();

			for(json_spirit::Object::const_iterator j = isobj.begin(); j!=isobj.end(); j++)
			{
				strKey = j->name_;
				string strid;

				//printf("***strKey:%s\n", strKey.c_str());
				
				if (strKey.compare("id")==0)
				{
					cd.id = j->value_.get_str();
				}else if(strKey.compare("isUse")==0)
				{
					cd.isuse = j->value_.get_bool();
				}else if (strKey.compare("description")==0)
				{
					cd.desc = j->value_.get_str();
					/*
					json_spirit::Array at=(j->value_).get_array();

					for (json_spirit::Array::iterator ti = at.begin(); ti!= at.end(); ti++)
					{
						json_spirit::Object tiobj = ti->get_obj();

						for(json_spirit::Object::const_iterator k = tiobj.begin(); k!=tiobj.end(); k++)
						{
							string key = k->name_;

							if (key.compare("monitorId") == 0)
							{
								string strMon = k->value_.get_str();
								string::size_type pos2 = strMon.find_last_of(")");
								string::size_type pos1 = strMon.find_last_of("(");
								if (pos2 > pos1+1)
								{
									cd.monitors.push_back(strMon.substr(pos1+1,pos2-pos1-1));
								}
 							}
						}

					}
					*/
				}
				
			}

			if (cd.isuse)
				conditions[cd.id] = cd;
		}
	}

	//没有设置条件将是全部匹配
	if (conditions.size() <= 0)
	{
		printf("***没有设置有效条件\n");
		return false;
	}

	
	for(ConditionMap::iterator ci = conditions.begin(); ci != conditions.end(); ci++)
	{
		std::string strdesc = ci->second.desc;
		std::string strTmpID = "{" + strMonitorId + "}";

		printf("***表达式:%s\n", strdesc.c_str());

		//预处理
		if (strdesc.find(strTmpID) == string.npos) //此表达式不包含此检测器
			continue;

		string strMonID;
		string strTmpDesc;

		for(int j = 0; j < strdesc.size(); j++)
		{
			if (strdesc[j] == '{')
			{
				strMonID = "{";
				continue;
			}
			if(strdesc[j] == '}')
			{
				string tmp = strMonID + "}";

				strMonID = CAlertMain::ReplaceStdString(strMonID,"{","");

				if (strMonID.compare(strMonitorId) == 0)
				{
					strTmpDesc.push_back('1');
				}else
				{
					MonitorCacheType::iterator mc = m_mapMonitorCache.find(strMonID);
					if (mc == m_mapMonitorCache.end())
					{
						strTmpDesc.push_back('0');
					}else
					{
						string frequency = CAlertMain::GetMonitorParameterValue(strMonID,"_frequency");
						DWORD dwNow = GetTickCount();

						DWORD dwTime = mc->second;
						char *cEnd = NULL;
						long nFreq = strtol(frequency.c_str(), &cEnd,10);
						//看该检测器的最后状态是不是有效的（监测频率加了2分钟的缓冲时间）
						if(dwNow - dwTime > (nFreq+2) * 60 * 1000) //
						{	
							strTmpDesc.push_back('0');
						}else
						{
							strTmpDesc.push_back('1');
						}
					}
				}

				strMonID = ""; //清空
				continue;
			}
			
			if (strMonID.size() > 0)
			{
				strMonID.push_back(strdesc[j]);
				continue;
			}
			
			strTmpDesc.push_back(strdesc[j]);
			
		}
		strTmpDesc = CAlertMain::ReplaceStdString(strTmpDesc,"AND","&");
		strTmpDesc =CAlertMain::ReplaceStdString(strTmpDesc,"OR","|");

		printf("***预处理后:%s\n", strTmpDesc.c_str());

		//预处理结束

		//
		boolexpression exp;

		char result = exp.result(strTmpDesc.c_str());
		
		if (result == '1')
		{
			printf("***组合条件匹配成功:%s,ID:%s\n", strdesc.c_str(), strMonitorId.c_str());
			return true;
		}
	}
	/*
	for(ConditionMap::iterator ci = conditions.begin(); ci != conditions.end(); ci++)
	{
		if (ci->second.type.compare("或") == 0)
		{
			for (std::vector<std::string>::iterator mi = ci->second.monitors.begin(); mi != ci->second.monitors.end(); mi++)
			{
				if (mi->compare(strMonitorId) == 0) //或条件，只要列表里有该monitor就匹配成功
				{
					conditions.clear();
					return true;
				}
			}
		}else if (ci->second.type.compare("与") == 0)
		{
			bool allok = true;
			bool bfind = false; 
			for (std::vector<std::string>::iterator mi = ci->second.monitors.begin(); mi != ci->second.monitors.end(); mi++)
			{
				if (mi->compare(strMonitorId) == 0) 
				{
					bfind = true;
				}
				else
				{
					//检查其他的是不是状态符合
					string frequency = CAlertMain::GetMonitorParameterValue(*mi,"_frequency");
					DWORD dwNow = GetTickCount();

					MonitorCacheType::iterator mc = m_mapMonitorCache.find(*mi);
					if (mc == m_mapMonitorCache.end())
					{
						allok = false;
						printf("缓存中没有%s的记录\n",mi->c_str());
						break;
					}

					DWORD dwTime = mc->second;
					char *cEnd = NULL;
					long nFreq = strtol(frequency.c_str(), &cEnd,10);
					//看该检测器的最后状态是不是有效的（监测频率加了2分钟的缓冲时间）
					if(dwNow - dwTime > (nFreq+2) * 60 * 1000) //
					{	
						allok = false;
						printf("缓存中%s的记录过期了\n",mi->c_str());
						break;
					}

					printf("frequency:%d,cache time:%u,now:%u\n", nFreq, dwTime,dwNow);
					
				}
			}

			printf("bfind:%s,allok:%s\n", bfind?"true":"false",allok?"true":"false");

			if (bfind&&allok)
			{
				conditions.clear();
				return true;
			}
		}
	}
	*/
	

	return false;
}
