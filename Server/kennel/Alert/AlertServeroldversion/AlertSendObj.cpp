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
#include <svapi.h>
#include <svdbapi.h>
#include <funcGeneral.h>
#include <des.h>
//#include "StringEx.h"
#define  MAX_SMS_LENGTH 60

// 2007/7/18 龚超 为了通过输出调试跟踪代码运行时错误而引用StringHelper.h中的调试信息输出工具
#include "../base/StringHelper.h"
using namespace SH;

using namespace std;
using namespace ost;
static const basic_string <char>::size_type npos = -1;
extern void DebugePrint(string strDebugInfo);

const char svPassword[] = "password";
const int  svBufferSize = 1024 * 4;
//

map<int, string, less<int> > lsDeviceId;

//////////////AlertLog日志数据库的插入函数///////

using namespace std;

typedef unsigned char BYTE;

inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
}


string urlEncoding( string &sIn )
{
	string sOut;
	for( int ix = 0; ix < sIn.size(); ix++ )
	{
		BYTE buf[4];
		memset( buf, 0, 4 );
		if( isalnum( (BYTE)sIn[ix] ) )
		{
			buf[0] = sIn[ix];
		}
		else if ( isspace( (BYTE)sIn[ix] ) )
		{
			buf[0] = '+';
			cout << "sp" << endl;
		}
		else
		{
			buf[0] = '%';
			buf[1] = toHex( (BYTE)sIn[ix] >> 4 );
			buf[2] = toHex( (BYTE)sIn[ix] % 16);
		}
		sOut += (char *)buf;
	}
	return sOut;
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

	prd->createtime=TTime::GetCurrentTimeEx();

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

	//读取email接收地址等

	//是否是升级邮件
	if(bUpgrade)
	{
		szMailTo = strAlertUpgradeToValue;
	}
	else
	{
		//if(strEmailAdressValue != "其他")
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

	if(szMailTo == "")
	{
		CAlertSendObj::InsertRecord(szMailTo, 1, 0);
		return false;
	}

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

	//如果是多个邮件地址用逗号分隔则 
	std::list<string> listEmail;
	std::list<string>::iterator listEmailItem;
	OutputDebugString (strAlertContent.c_str());
	bool bSucess = false;
	bool bAllSucess = true;

	try
	{
		CAlertMain::ParserToken(listEmail, szMailTo.c_str(), ",");		

		for(listEmailItem = listEmail.begin(); listEmailItem!=listEmail.end(); listEmailItem++)
		{
			//重写了邮件发送代码，对此工程进行相应修改
			//苏合 2007-07-24
			//+++++++++++++修改开始 苏合 2007-07-24+++++++++++++
			/*
			bSucess = CAlertMain::pSendEmail(szEmailServer.c_str(), szEmailfrom.c_str(), (*listEmailItem).c_str(),
			strAlertTitle.c_str(), strAlertContent.c_str(), szUserID.c_str(), szUserPwd.c_str());
			*/
			bSucess = CAlertMain::pSendEmail(szEmailServer.c_str(), szEmailfrom.c_str(), (*listEmailItem).c_str(),
				strAlertTitle.c_str(), strAlertContent.c_str(), szUserID.c_str(), szUserPwd.c_str(), NULL);
			//+++++++++++++修改开结束 苏合 2007-07-24+++++++++++++

			//bSucess = CAlertMain::SendMail(szEmailServer, szEmailfrom, (*listEmailItem),
			//	strAlertTitle, strAlertContent, szUserID, szUserPwd);

			if(!bSucess)
				bAllSucess = false;
		}

		if(bAllSucess)
		{		
			CAlertSendObj::InsertRecord(szMailTo, 1, 1);
		}
		else
		{
			CAlertSendObj::InsertRecord(szMailTo, 1, 0);
		}

	}
	catch(...)
	{
		CAlertSendObj::InsertRecord(szMailTo, 1, 0);
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
	strDebugInfo += "------------------发送事件信息结束------------------------------\r\n";
	return strDebugInfo;
}

//生成报警标题
void CAlertEmailSendObj::MakeAlertTitle()
{
	//为解决Ticket #112 东方有线ECC邮件报警主题定制问题而修改下面代码。
	//苏合 2007-08-02

	//+++++++++++++++++++++++++++代码修改开始  苏合 2007-08-02+++++++++++++++++++++++++++
	/*
	strAlertTitle += CAlertMain::GetMonitorTitle(strAlertMonitorId);
	strAlertTitle += "-";
	strAlertTitle += CAlertMain::GetDeviceTitle(strAlertMonitorId);
	*/
	string strTmp;
	int isTitleCheck = GetIniFileInt(strAlertIndex, "isCheck", 0,  "alert.ini");
	if(isTitleCheck == 1)
	{
		strAlertTitle = GetIniFileString(strAlertIndex, "EmailTitle", "", "alert.ini");
		strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@device@", CAlertMain::GetDeviceTitle(strAlertMonitorId));
		strAlertTitle = strTmp;
		strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@time@",  strTime);	
		strAlertTitle = strTmp;
		strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@monitor@", CAlertMain::GetMonitorTitle(strAlertMonitorId));
		strAlertTitle = strTmp;

		string strDeviceId = FindParentID(strAlertMonitorId);
		strTmp = CAlertMain::ReplaceStdString(strAlertTitle, "@group@", CAlertMain::GetAllGroupTitle(strDeviceId));
		strAlertTitle = strTmp;
	}
	else
	{
		strAlertTitle += CAlertMain::GetMonitorTitle(strAlertMonitorId);
		strAlertTitle += "-";
		strAlertTitle += CAlertMain::GetDeviceTitle(strAlertMonitorId);
	}	
	//+++++++++++++++++++++++++++代码修改结束  苏合 2007-08-02+++++++++++++++++++++++++++
}

//生成报警内容
void CAlertEmailSendObj::MakeAlertContent()
{
	string strTmp;
	//strAlertContent = GetIniFileString("Email", strEmailTemplateValue, "", "TxtTemplate.Ini");
	strAlertContent = GetTemplateContent("Email", strEmailTemplateValue);

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

	/* // 这一段代码取报警邮件尾部报警阀值，但是错误的从监测器模板中提取该阀值，因此收到的报警邮件中阀值总是不会变的。
	//2006-10-23 jiang 
	OBJECT hTemplet;
	OBJECT hMon = GetMonitor(strAlertMonitorId);
	OUT_DEBUG(strAlertMonitorId.c_str());
	std::string getvalue;
	MAPNODE ma=GetMonitorMainAttribNode(hMon);
	std::string szErrorValue;
	//monitortemplet ID
	if ( FindNodeValue( ma,"sv_monitortype",getvalue) )
	{			
	OUT_DEBUG(getvalue.c_str());
	//monitortemplet 句柄
	hTemplet = GetMonitorTemplet(atoi(getvalue.c_str()));
	MAPNODE node = GetMTMainAttribNode(hTemplet);
	//monitortemplet 标签

	//报告设置是否显示阀值
	MAPNODE errorNode = GetMTErrorAlertCondition(hTemplet);
	FindNodeValue(errorNode, "sv_value", szErrorValue);
	OUT_DEBUG(szErrorValue.c_str());
	}
	*/

	// 为解决Ticket #26报警邮件阀值错误而修改下面代码。
	// 苏合 2007-07-18
	//++++++++++++++++++代码修改开始 苏合 2007-07-18++++++++++++++++++

	// ++++++ 取当前报警监测器对象的报警阀值 ++++++
	// 2007/6/26 龚超
	// 为解决Ticket #26报警邮件阀值错误而修正代码。

	std::string szErrorValue;
	// 取当前报警的监测器对象
	OBJECT hMon = GetMonitor(strAlertMonitorId);

	MAPNODE ma = INVALID_VALUE;  //将ma移到外面定义，以免在后面没定义 苏合 2007-07-18

	if (NULL != hMon)
	{
		//将ma移到外面定义，此处注释掉，以免在后面没定义 苏合 2007-07-18
		//++++++++++++++++++注释开始 苏合 2007-07-18++++++++++++++++++
		/*
		MAPNODE ma = NULL;
		*/
		//++++++++++++++++++注释结束 苏合 2007-07-18++++++++++++++++++

		// 取监测器对象错误报警设置参数对象
		switch(nEventType)
		{
		case 1: // 正常报警
			ma = GetMonitorGoodAlertCondition(hMon);
			break;
		case 2: // 危险报警
			ma = GetMonitorWarningAlertCondition(hMon);
			break;
		case 3: // 错误报警
			ma = GetMonitorErrorAlertCondition(hMon);
			break;
		}


		//此段代码仅仅给出了第一个报警条件的阀值数字，将其注释，并在后面增加代码以完成多个报警条件的详细情况 苏合 2007-07-18
		//++++++++++++++++++注释开始 苏合 2007-07-18++++++++++++++++++
		/*
		if (NULL != ma)
		{
		// 取错误报警阀值
		FindNodeValue( ma, "sv_paramvalue1", szErrorValue);
		}
		*/
		//++++++++++++++++++注释结束 苏合 2007-07-18++++++++++++++++++
	}
	//++++++++++++++++++代码修改结束 苏合 2007-07-18++++++++++++++++++

	// 为解决Ticket #26报警邮件阀值错误而增加下面代码。
	// 苏合 2007-07-18

	//++++++++++++++++++代码增加开始 苏合 2007-07-18++++++++++++++++++
	OBJECT objRes=LoadResource("default", "localhost");  
	if( objRes !=INVALID_VALUE )
	{	
		MAPNODE ResNode=GetResourceNode(objRes);
		if( ResNode != INVALID_VALUE )
		{

			OBJECT objTempMonitor = INVALID_VALUE;
			MAPNODE monitorTempNode;
			MAPNODE monitorNode;

			if(hMon!=INVALID_VALUE)
			{
				PAIRLIST ParamList;
				LISTITEM ParamItem;
				string strTemp, strTempCon;

				//修改多个报警条件组合只显示最后一条的问题
				//苏合 2007-07-19

				if(ma != INVALID_VALUE)
				{
					string strCount,strParamName,strParamValue,strParamOperate,strParamRelation,strReturn;
					bool bFind = FindNodeValue(ma, "sv_conditioncount", strCount);  //读取阀值条件的数目 苏合 2007-07-18
					int iCount = atoi(strCount.c_str());
					for(int i=1; i<= iCount; i++)
					{
						//修改多个报警条件组合只显示最后一条的问题 苏合 2007-07-19
						//szErrorValue = "[";
						strTempCon = "[";

						string paraIndex;
						char buf[65];
						itoa(i,buf,10);
						paraIndex = buf;
						strTemp = "sv_paramname" + paraIndex;
						bFind = FindNodeValue(ma, strTemp, strReturn);

						monitorNode = GetMonitorMainAttribNode(hMon);
						std::string getvalue;   //“getvalue”是监视器在模板中的ID 苏合 2007-07-18
						if ( FindNodeValue( monitorNode,"sv_monitortype",getvalue) )
						{			
							OUT_DEBUG(getvalue.c_str());					
							objTempMonitor = GetMonitorTemplet(atoi(getvalue.c_str())); //监视器模板对象 苏合 2007-07-18
						}
						if(objTempMonitor != INVALID_VALUE)
						{
							//在监视器模板的返回值中查找阀值条件的名字 苏合 2007-07-18
							if(FindMTReturnFirst(objTempMonitor,ParamItem))
							{
								while((monitorTempNode=::FindNext(ParamItem))!=INVALID_VALUE )
								{ 
									ParamList.clear();
									if(::EnumNodeAttrib(monitorTempNode,ParamList))
									{	
										string sReturnName = "", MonitorIDSName ="";
										FindNodeValue(monitorTempNode, "sv_name",sReturnName);
										if(sReturnName == strReturn)
										{
											FindNodeValue(monitorTempNode, "sv_label",MonitorIDSName);
											FindNodeValue(ResNode,MonitorIDSName,strParamName);

											//修改多个报警条件组合只显示最后一条的问题 苏合 2007-07-19
											//szErrorValue += strParamName;
											strTempCon += strParamName;

										}
									}
								}
							}
						}	
						strTemp = "sv_operate" + paraIndex;
						bFind = FindNodeValue(ma, strTemp, strParamOperate); //读取阀值操作符号（比如“>”“<=”） 苏合 2007-07-18

						//修改多个报警条件组合只显示最后一条的问题 苏合 2007-07-19
						//szErrorValue += strParamOperate;
						strTempCon += strParamOperate;

						strTemp = "sv_paramvalue" + paraIndex;
						bFind = FindNodeValue(ma, strTemp, strParamValue);	//读取阀值 苏合 2007-07-18

						//修改多个报警条件组合只显示最后一条的问题 苏合 2007-07-19
						//szErrorValue += strParamValue;
						//szErrorValue += "]";
						strTempCon += strParamValue;
						strTempCon += "]";

						szErrorValue += strTempCon; //修改多个报警条件组合只显示最后一条的问题 苏合 2007-07-19 增加一行

						if(i<iCount)
						{
							strTemp = "sv_relation" + paraIndex;
							bFind = FindNodeValue(ma, strTemp, strParamRelation); //读取关系符 苏合 2007-07-18

							//修改多个报警条件组合只显示最后一条的问题 苏合 2007-07-19
							//szErrorValue += strParamRelation;
							szErrorValue = szErrorValue + " " + strParamRelation + " "; //加入空格，使显示更美观 苏合 2007-07-18																	
						}
					}
				}								
				ParamList.clear();
				CloseMonitorTemplet(objTempMonitor);				
			}
		}
		CloseResource(objRes);
	}

	CloseMonitor(hMon); //关闭监视器对象 苏合 2007-07-18
	OUT_DEBUG(szErrorValue.c_str());
	//++++++++++++++++++代码增加结束 苏合 2007-07-18++++++++++++++++++

	// for debug
	/*
	CString strDbgText;
	strDbgText.Format("AlertTitle[%s], AlertUpgradeToValue[%s], Schedule[%s], AlertIndex[%s], AlertName[%s], EventType[%d], Type[%d]",
	strAlertTitle.c_str(), 
	strAlertUpgradeToValue.c_str(), 
	szSchedule.c_str(), 
	strAlertIndex.c_str(), 
	strAlertName.c_str(), 
	nEventType, 
	nType);
	OUT_DEBUG(LPCSTR(strDbgText));
	*/

	// ------ 取当前报警监测器对象的报警阀值 ------

	//strAlertContent += "\n阀值:     ";
	strAlertContent += "\n";
	strAlertContent += CAlertMain::strMonitorFazhi.c_str();
	strAlertContent += "       ";
	strAlertContent +=  szErrorValue;
	strAlertContent += "\n";
	//add end

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

				/*if(strTotleContent != "")
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
				}*/
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
	ODS_DEFINE(_T(" for debug"));
	ODS_OUTDEBUG(_T("CAlertSmsSendObj:: SendAlert ... "));

	string szWebUser = "", szWebPwd = "", szComPort = "", szTmp = "";

	//读email.ini获取smtp、发送地址等发送参数

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

	//读取email接收地址等

	//是否是升级邮件
	if(bUpgrade)
	{
		szSmsTo = strAlertUpgradeToValue;
	}
	else
	{
		//if(strSmsNumberValue != "其他")
		if(strcmp(strSmsNumberValue.c_str(), CAlertMain::strOther.c_str()) != 0)
		{
			//有接收人

			//邮件接收地址禁止则返回
			if(!GetInfoFromSmsAddress(strSmsNumberValue))
			{
				DebugePrint("SmsAdress Disable");
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
			szSmsTo = strOtherNumberValue;

			//此时的模板值为：strEmailTemplateValue
		}
	}

	if(szSmsTo == "")
	{
		CAlertSendObj::InsertRecord(szSmsTo, 2, 0);
		return false;
	}

	//构造AlertTitle和AlertContent
	MakeAlertTitle();
	MakeAlertContent();
	//int iEmailRet = 1;



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

		if(bSucess)
		{
			CAlertSendObj::InsertRecord(szSmsTo, 2, 1);
		}
		else
		{
			CAlertSendObj::InsertRecord(szSmsTo, 2, 0);
		}
	}
	catch(...)
	{
		CAlertSendObj::InsertRecord(szSmsTo, 2, 0);
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

//生成报警内容
void CAlertSmsSendObj::MakeAlertContent()
{
	string strTmp;
	//strAlertContent = GetIniFileString("SMS", strSmsTemplateValue, "", "TxtTemplate.Ini");
	strAlertContent = GetTemplateContent("SMS", strSmsTemplateValue);

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

	return bReturn;
}

//通过串口方式发送短信
bool CAlertSmsSendObj::SendSmsFromCom()
{
	ODS_DEFINE(_T(" for debug"));
	ODS_OUTDEBUG(_T("CAlertSmsSendObj::SendSmsFromCom ... "));

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

		// 2007/7/4 龚超 修正短信正文无端被切断的问题
		strContent = strAlertContent.c_str();
		if(CAlertMain::SendSmsFromComm(strSmsTo, strContent) == -1)
			bSucess = false;

		/* 该段代码目的不明，但造成短信正文被强行切割成若干段，每段正文都<=50字节 2007/7/4 龚超
		//如果是多个手机号码用逗号分隔则 
		std::list<string> listSmsContent;
		std::list<string>::iterator listSmsContentItem;		

		CAlertMain::ParserToLength(listSmsContent, strAlertContent, 50);
		for(listSmsContentItem = listSmsContent.begin(); listSmsContentItem!=listSmsContent.end(); listSmsContentItem++)
		{
		strContent = (*listSmsContentItem).c_str();		
		//DebugePrint((*listSmsContentItem).c_str());
		//DebugePrint("SendSmsFromCom\r\n");
		if(CAlertMain::SendSmsFromComm(strSmsTo, strContent) == -1)
		bSucess = false;
		strContent = "";
		}
		*/

		//CAlertMain::SendSmsFromComm(strSmsTo, "This is com test1");
	}

	return bSucess;
}

//

// bool CAlertSmsSendObj::SendSmsFromWeb()
// {
// 	if(CAlertMain::pSender == NULL)
// 		return false;
//     bool bRet = true;
// 	
// 	string sret;
// 	
// 	string User = GetIniFileString("SMSWebConfig", "User", sret, "smsconfig.ini");
// 	string Pwd = GetIniFileString("SMSWebConfig", "Pwd", sret, "smsconfig.ini");
// 
// 	Des mydes;
// 	char dechar[1024]={0};
 	//if(Pwd.size()>0)
 	//{
 	//	mydes.Decrypt(Pwd.c_str(),dechar);
 	//	Pwd = dechar;
 
 
 	//}
// 
// 	//string strSMS = "This is test!";
// 	string strSMS = "";
// 	//strSMS += strAlertTitle;
// 	//strSMS += "\n";
// 	strSMS += strAlertContent;
// 
// 	//如果是多个手机号码用逗号分隔则 
// 	std::list<string> listSms;
// 	std::list<string>::iterator listSmsItem;
// 	
// 	//CAlertMain::ParserToken(listSms, szSmsTo.c_str(), ",");
// 	CAlertMain::ParserToLength(listSms, strSMS, 140);
// 	
// 
// 	//::CoInitialize(NULL);
// 	{
// 		//HRESULT hr = S_OK;
// 
// 		//IUMSmSendPtr pSender = NULL;
// 		//hr = pSender.CreateInstance("SMSend.UMSmSend");
// 		//if( SUCCEEDED(hr) && (NULL != pSender) )
// 		{
// 
// 			string strPhone = szSmsTo;
// 
// 			for(listSmsItem = listSms.begin(); listSmsItem!=listSms.end(); listSmsItem++)
// 			{
// 
// 				// 构造短消息XML:
// 				/*_bstr_t bstrSendSMSXML("<?xml version=\"1.0\" encoding=\"GB2312\"?> \
// 				<message><CompAccount>companyName</CompAccount><password>companyPass</password><InternalUserName>demo</InternalUserName><MobileNumber>13910000000</MobileNumber></message>");
// 				*/
// 
// 				char chSMSXML[1024] = {0};
// 				sprintf(chSMSXML, "<?xml version=\"1.0\"?><message><EntCode>62016161</EntCode>" \
// 					"<EntUserID>%s</EntUserID><password>%s</password><Content>%s</Content>" \
// 					"<DestMobileNumber>%s</DestMobileNumber><URGENT_Flag>1</URGENT_Flag>" \
// 					"<ScheduledTime></ScheduledTime><Batch_SendID></Batch_SendID><DataType>15" \
// 					"</DataType><SrcNumber></SrcNumber></message>", User.c_str(), Pwd.c_str(), /*strSMS.c_str()*/(*listSmsItem).c_str(), 
// 					strPhone.c_str());
// 
// 				//
// 				//  公钥文件的物理位置:
// 				//_bstr_t bstrPublicKeyPath("D:\\Program Files\\SMSIIGatewayAPI\\PublicKey\\pub.txt");
// 				string strPath = "";
// 				strPath = GetSiteViewRootPath() + "\\fcgi-bin\\pub.txt";		
// 
// 				_bstr_t bstrPublicKeyPath(strPath.c_str());
// 				//
// 				//  服务器端接收的ASP页面的URL
// 				//_bstr_t bstrServerSiteURL("http://gateway.bjums.com/smssite/smsstart.asp");
// 				_bstr_t bstrServerSiteURL("http://sms.bmcc.com.cn/GatewayAPI/SMSIIGateWay.asp");
// 				//
// 				///////////////////////////////////////////////////////////
// 
// 
// 				///////////////////////////////////////////////////////////
// 				//
// 				// 设置组件接口所需要的参数
// 				//
// 				//  第一个:公钥文件的物理位置
// 				//printf("公钥文件的物理位置: %s\n", (char*)bstrPublicKeyPath);
// 				CAlertMain::pSender->SetPkpath(bstrPublicKeyPath);
// 				//pSender->SetPkpath(bstrPublicKeyPath);
// 				//
// 				//  第二个:服务器端接收的ASP页面的URL
// 				//printf("服务器端接收的ASP页面的URL: %s\n\n", (char*)bstrServerSiteURL);
// 				CAlertMain::pSender->SetServerSite(bstrServerSiteURL);
// 				//pSender->SetServerSite(bstrServerSiteURL);
// 				//
// 				///////////////////////////////////////////////////////////
// 
// 
// 				///////////////////////////////////////////////////////////
// 				//
// 				// 实际发送短消息XML给服务器
// 				//
// 				// 第一个参数257代表采用RSA加密算法
// 				// 第二个参数就是短消息XML
// 
// 				//printf("实际发送短消息XML给服务器: %s\n\n", (char*)bstrSendSMSXML);
// 				//pSender->LoadSendXML("257",
// 				//	                 bstrSendSMSXML);
// 				//printf("实际发送短消息XML给服务器: %s\n\n", (char*)chSMSXML);
// 				CAlertMain::pSender->LoadSendXML("257", 
// 					chSMSXML);
// 				printf("实际发送短消息XML给服务器: %s\n\n", (char*)chSMSXML);
// 				//pSender->LoadSendXML("257", 
// 				//	chSMSXML);
// 
// 				//
// 				///////////////////////////////////////////////////////////
// 
// 
// 				///////////////////////////////////////////////////////////
// 				//
// 				// 发送短消息XML之后，可以通过以下两个方法得到服务器方的反馈
// 				//  1:GetResponseText(通过这个方法可以取到服务器端返回的responseText)
// 				//  2:GetHTTPPostStatus(通过这个方法可以取到服务器端返回的状态值,即200、404、500之类的值)
// 				//
// 				// 第一个参数257代表采用RSA加密算法
// 				// 第二个参数就是短消息XML
// 				_bstr_t bstrServerResponseText = CAlertMain::pSender->GetResponseText();
// 				//_bstr_t bstrServerResponseText = pSender->GetResponseText();
// 				printf("服务器端返回的responseText: %s\n", (char*)bstrServerResponseText);
// 				//
// 				long    lServerResponseStatus  = CAlertMain::pSender->GetHTTPPostStatus();
// 				//long    lServerResponseStatus  = pSender->GetHTTPPostStatus();
// 				printf("服务器端返回的状态值: %d\n", lServerResponseStatus);
// 				char buf[256];
// 				char buf1[256];
// 				memset(buf, 0, 256);
// 				memset(buf1, 0, 256);
// 				sprintf(buf, "服务器端返回的responseText: %s\n", (char*)bstrServerResponseText);
// 				sprintf(buf1, "服务器端返回的状态值:%d\n", lServerResponseStatus);
// 				strcat(buf, buf1);
// 
// 				std::string sRet = buf;
// 				int bFind = sRet.find("提交成功", 0);
// 
// 				if(bFind >= 0)
// 				{
// 					//成功
// 					//bRet = true;
// 				}
// 				else
// 				{
// 					//失败
// 					bRet = false;
// 				}
// 			}
// 		}
// 	}
// 	//::CoUninitialize();
// 	
// 	return bRet;
// 	//return bRet;
// }

BOOL GetSourceHtml(char const * theUrl, char * retState) 
{
	CInternetSession session;
	CInternetFile* file = NULL;
	try
	{
		// 试着连接到指定URL
		file = (CInternetFile*) session.OpenURL(theUrl); 
	}
	catch (...)
	{
		// 如果有错误的话，置内容为空
		strcpy(retState, "error");
		return FALSE;
	}

	if (file)
	{
		CString  somecode;

		bool flagReplace = false;
		int replaceNum = 0;
		while (file->ReadString(somecode) != NULL) //如果采用LPTSTR类型，读取最大个数nMax置0，使它遇空字符时结束
		{
			strcpy(retState, somecode);
		}

	}
	else
	{
		strcpy(retState, "error");
		return FALSE;
	}
	return TRUE;
}

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

	CAnalyzer4_bstr_t Analyzer;
	_bstr_t strMsgs = strAlertContent.c_str();
	OutputDebugString(strAlertContent.c_str());
	int nMaxSMSLen = MAX_SMS_LENGTH;
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

	for(listSmsItem = listSms.begin(); listSmsItem!=listSms.end(); listSmsItem++)
	{
		string url = urlEncoding(*listSmsItem);
		OutputDebugString((*listSmsItem).c_str());
		memset(buf, 0, 300);
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
	delete [] buf;

	return bRet;
}

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

	if((strDllName == "") || (strDllFuncParam == ""))
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
	catch(...)
	{
		CAlertSendObj::InsertRecord(strServerTextValue, 3, 0);
	}
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


	string strTmp = GetIniFileString("Scripts", strScriptFileValue, "",  "TxtTemplate.Ini");

	DebugePrint(strTmp);
	DebugePrint("DoWinScript\r\n");

	CString strScriptFile(strTmp.c_str());

	// 2007/7/25 龚超 增加代码以解决定制脚本文件名的正确获取
	// 如果是一个定制脚本，则通过GetIniFileString是无法正确获得脚本文件名的，strScriptFileValue本身就是
	// 定制脚本的文件名
	if (strScriptFile.GetLength() < 1)
		strScriptFile = strScriptFileValue.c_str();

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

			// 2007/7/25 龚超 以下代码行被注释是因为其格式化方式存在问题无法正确执行脚本脚本文件
			// strSpt.Format("cscript %s\\scripts\\%s %s",strInstallPath,strScriptFile,strScriptParam);

			// 2007/7/25 龚超 更新脚本执行串格式化方法
			strSpt.Format("cscript \"%s\\cscripts\\%s\" \"%s\"", strInstallPath, strScriptFile, strScriptParam);

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

	try
	{
		CString strReturn = DoMsgBeep();

		OutputDebugString(strReturn);

		if(strReturn == "ok")
		{
			CAlertSendObj::InsertRecord(strServerValue, 4, 1);
		}
		else
		{
			CAlertSendObj::InsertRecord(strServerValue, 4, 0);
		}
	}
	catch(...)
	{
		CAlertSendObj::InsertRecord(strServerValue, 4, 0);
	}

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
