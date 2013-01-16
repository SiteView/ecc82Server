////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//һ���������Ͷ����ࣺ
//    1�����ݽṹ
//	     a��Id���������ͱ�������Ĳ���������ʱ��ȡ�
//    2�������ⲿDll�Է��ͱ������麯����
//    3����¼������־�ĺ�����
//�����������������ࣺ�ʼ������š��ű�������
//    1����д�����ⲿDll�Է��ͱ������麯����
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

// 2007/7/18 ���� Ϊ��ͨ��������Ը��ٴ�������ʱ���������StringHelper.h�еĵ�����Ϣ�������
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

//////////////AlertLog��־���ݿ�Ĳ��뺯��///////

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

//���¼��AlertLog��
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
	strDebugInfo += "\r\n------------------�����¼���Ϣ��ʼ-----------------------------\r\n";
	strDebugInfo += ("���������Id��" + strAlertMonitorId + "\r\n");
	strDebugInfo += ("�����¼����ͣ���SendObj��\r\n");
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nSendId);
	string strCount = chItem;
	sprintf(chItem, "%d", nEventCount);
	string strEventCount = chItem;
	sprintf(chItem, "%d", nEventType);
	string strEventType = chItem;
	strDebugInfo += ("�����¼���ţ�" + strCount + "\r\n");
	strDebugInfo += ("ԭʼ�¼���ţ�" + strEventCount + "\r\n");
	strDebugInfo += ("ԭʼ�¼����ͣ�" + strEventType + "\r\n");
	strDebugInfo += ("���������¼��ı�����" + strAlertName + "\r\n");
	strDebugInfo += "------------------�����¼���Ϣ����------------------------------\r\n";

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

//���ͱ���
bool CAlertEmailSendObj::SendAlert()
{	
	string szEmailServer = "", szEmailfrom = "", szUserID = "", szUserPwd = "",
		szBackServer = "" ,szTmp = "";

	//��email.ini��ȡsmtp�����͵�ַ�ȷ��Ͳ���

	// SMTP ������
	szEmailServer=GetIniFileString("email_config", "server", "",  "email.ini");
	// Email from
	szEmailfrom=GetIniFileString("email_config", "from", "",  "email.ini");
	// У���û�
	szUserID=GetIniFileString("email_config", "user", "",  "email.ini");
	// У������
	szUserPwd=GetIniFileString("email_config", "password", "",  "email.ini");
	Des mydes;
	char dechar[1024]={0};
	if(szUserPwd.size()>0)
	{
		mydes.Decrypt(szUserPwd.c_str(),dechar);
		szUserPwd = dechar;
	}
	// ����SMTP������
	szBackServer=GetIniFileString("email_config", "backupserver", "",  "email.ini");

	//��ȡemail���յ�ַ��

	//�Ƿ��������ʼ�
	if(bUpgrade)
	{
		szMailTo = strAlertUpgradeToValue;
	}
	else
	{
		//if(strEmailAdressValue != "����")
		if(strcmp(strEmailAdressValue.c_str(), CAlertMain::strOther.c_str()) != 0)
		{
			//�н�����

			DebugePrint(strEmailAdressValue.c_str());

			//�ʼ����յ�ַ��ֹ�򷵻�
			if(!GetInfoFromEmailAddress(strEmailAdressValue))
			{
				DebugePrint("EmailAdress Disable");
				return true;
			}

			//��ʱ��ģ��ֵΪ�ʼ����յ�ַ�趨��ֵ��strEmailTemplateValue

			//�ʼ����յ�ַ�е�������������ݵ���ƥ���ж��Ƿ����ʼ�
			if(!CAlertMain::IsScheduleMatch(szSchedule))
			{
				//��¼��־������¼�����
				DebugePrint("Schedule Disable");
				return true;
			}
		}
		else
		{
			//�����ʼ���ַ
			szMailTo = strOtherAdressValue;
			//strEmailTemplateValue = strAlertTemplateValue;
			//��ʱ��ģ��ֵΪ��strEmailTemplateValue
		}
	}

	if(szMailTo == "")
	{
		CAlertSendObj::InsertRecord(szMailTo, 1, 0);
		return false;
	}

	//����AlertTitle��AlertContent
	MakeAlertTitle();
	MakeAlertContent();
	//int iEmailRet = 1;

	//����...	
	//CAlertMain::pSendEmail(szEmailServer.c_str(), szEmailfrom.c_str(), strAlertTitle.c_str(), strAlertContent.c_str(),
	//	szEmailContent.c_str(), szUserPwd.c_str(),	szUserID.c_str(), iEmailRet);
	string strDebugInfo = "";
	strDebugInfo += "\r\n------------------�����ʼ���ʼ-----------------------------\r\n";
	strDebugInfo += ("���������Id��" + strAlertMonitorId + "\r\n");
	strDebugInfo += ("�����¼����ͣ���Email��\r\n");
	strDebugInfo += ("strEmailAdressValue��" + strEmailAdressValue + "\r\n");
	strDebugInfo += ("szEmailServer��" + szEmailServer + "\r\n");
	strDebugInfo += ("szEmailfrom��" + szEmailfrom + "\r\n");
	strDebugInfo += ("szMailTo��" + szMailTo + "\r\n");
	strDebugInfo += ("strAlertTitle��" + strAlertTitle + "\r\n");
	strDebugInfo += ("strAlertContent��" + strAlertContent + "\r\n");
	strDebugInfo += ("szUserID��" + szUserID + "\r\n");
	strDebugInfo += ("szUserPwd��" + szUserPwd + "\r\n");
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nSendId);
	string strCount = chItem;
	sprintf(chItem, "%d", nEventCount);
	string strEventCount = chItem;
	sprintf(chItem, "%d", nEventType);
	string strEventType = chItem;
	strDebugInfo += ("�����¼���ţ�" + strCount + "\r\n");
	strDebugInfo += ("ԭʼ�¼���ţ�" + strEventCount + "\r\n");
	strDebugInfo += ("ԭʼ�¼����ͣ�" + strEventType + "\r\n");

	//����Ƕ���ʼ���ַ�ö��ŷָ��� 
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
			//��д���ʼ����ʹ��룬�Դ˹��̽�����Ӧ�޸�
			//�պ� 2007-07-24
			//+++++++++++++�޸Ŀ�ʼ �պ� 2007-07-24+++++++++++++
			/*
			bSucess = CAlertMain::pSendEmail(szEmailServer.c_str(), szEmailfrom.c_str(), (*listEmailItem).c_str(),
			strAlertTitle.c_str(), strAlertContent.c_str(), szUserID.c_str(), szUserPwd.c_str());
			*/
			bSucess = CAlertMain::pSendEmail(szEmailServer.c_str(), szEmailfrom.c_str(), (*listEmailItem).c_str(),
				strAlertTitle.c_str(), strAlertContent.c_str(), szUserID.c_str(), szUserPwd.c_str(), NULL);
			//+++++++++++++�޸Ŀ����� �պ� 2007-07-24+++++++++++++

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

	strDebugInfo += "------------------�����ʼ�����------------------------------\r\n";
	DebugePrint(strDebugInfo);


	return bSucess;
}

//
string CAlertEmailSendObj::GetDebugInfo()
{
	string strDebugInfo = "";
	strDebugInfo += "\r\n------------------�����¼���Ϣ��ʼ-----------------------------\r\n";
	strDebugInfo += ("���������Id��" + strAlertMonitorId + "\r\n");
	strDebugInfo += ("�����¼����ͣ���Email��\r\n");
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nSendId);
	string strCount = chItem;
	sprintf(chItem, "%d", nEventCount);
	string strEventCount = chItem;
	sprintf(chItem, "%d", nEventType);
	string strEventType = chItem;
	strDebugInfo += ("�����¼���ţ�" + strCount + "\r\n");
	strDebugInfo += ("ԭʼ�¼���ţ�" + strEventCount + "\r\n");
	strDebugInfo += ("ԭʼ�¼����ͣ�" + strEventType + "\r\n");
	strDebugInfo += ("���������¼��ı�����" + strAlertName + "\r\n");
	strDebugInfo += "------------------�����¼���Ϣ����------------------------------\r\n";
	return strDebugInfo;
}

//���ɱ�������
void CAlertEmailSendObj::MakeAlertTitle()
{
	//Ϊ���Ticket #112 ��������ECC�ʼ��������ⶨ��������޸�������롣
	//�պ� 2007-08-02

	//+++++++++++++++++++++++++++�����޸Ŀ�ʼ  �պ� 2007-08-02+++++++++++++++++++++++++++
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
	//+++++++++++++++++++++++++++�����޸Ľ���  �պ� 2007-08-02+++++++++++++++++++++++++++
}

//���ɱ�������
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
		//strFreq = "���Ƶ��:";
		//strFreq += CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_frequency");
		//strFreq += "����";
		strFreq = CAlertMain::strMontorFreq.c_str();
		strFreq += ":";
		strFreq += CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_frequency");
		strFreq += CAlertMain::strMinute.c_str();

	}
	else
	{
		//strFreq = "���Ƶ��:";
		//strFreq += CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_frequency");
		//strFreq += "Сʱ";

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

	/* // ��һ�δ���ȡ�����ʼ�β��������ֵ�����Ǵ���ĴӼ����ģ������ȡ�÷�ֵ������յ��ı����ʼ��з�ֵ���ǲ����ġ�
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
	//monitortemplet ���
	hTemplet = GetMonitorTemplet(atoi(getvalue.c_str()));
	MAPNODE node = GetMTMainAttribNode(hTemplet);
	//monitortemplet ��ǩ

	//���������Ƿ���ʾ��ֵ
	MAPNODE errorNode = GetMTErrorAlertCondition(hTemplet);
	FindNodeValue(errorNode, "sv_value", szErrorValue);
	OUT_DEBUG(szErrorValue.c_str());
	}
	*/

	// Ϊ���Ticket #26�����ʼ���ֵ������޸�������롣
	// �պ� 2007-07-18
	//++++++++++++++++++�����޸Ŀ�ʼ �պ� 2007-07-18++++++++++++++++++

	// ++++++ ȡ��ǰ�������������ı�����ֵ ++++++
	// 2007/6/26 ����
	// Ϊ���Ticket #26�����ʼ���ֵ������������롣

	std::string szErrorValue;
	// ȡ��ǰ�����ļ��������
	OBJECT hMon = GetMonitor(strAlertMonitorId);

	MAPNODE ma = INVALID_VALUE;  //��ma�Ƶ����涨�壬�����ں���û���� �պ� 2007-07-18

	if (NULL != hMon)
	{
		//��ma�Ƶ����涨�壬�˴�ע�͵��������ں���û���� �պ� 2007-07-18
		//++++++++++++++++++ע�Ϳ�ʼ �պ� 2007-07-18++++++++++++++++++
		/*
		MAPNODE ma = NULL;
		*/
		//++++++++++++++++++ע�ͽ��� �պ� 2007-07-18++++++++++++++++++

		// ȡ�����������󱨾����ò�������
		switch(nEventType)
		{
		case 1: // ��������
			ma = GetMonitorGoodAlertCondition(hMon);
			break;
		case 2: // Σ�ձ���
			ma = GetMonitorWarningAlertCondition(hMon);
			break;
		case 3: // ���󱨾�
			ma = GetMonitorErrorAlertCondition(hMon);
			break;
		}


		//�˶δ�����������˵�һ�����������ķ�ֵ���֣�����ע�ͣ����ں������Ӵ�������ɶ��������������ϸ��� �պ� 2007-07-18
		//++++++++++++++++++ע�Ϳ�ʼ �պ� 2007-07-18++++++++++++++++++
		/*
		if (NULL != ma)
		{
		// ȡ���󱨾���ֵ
		FindNodeValue( ma, "sv_paramvalue1", szErrorValue);
		}
		*/
		//++++++++++++++++++ע�ͽ��� �պ� 2007-07-18++++++++++++++++++
	}
	//++++++++++++++++++�����޸Ľ��� �պ� 2007-07-18++++++++++++++++++

	// Ϊ���Ticket #26�����ʼ���ֵ���������������롣
	// �պ� 2007-07-18

	//++++++++++++++++++�������ӿ�ʼ �պ� 2007-07-18++++++++++++++++++
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

				//�޸Ķ�������������ֻ��ʾ���һ��������
				//�պ� 2007-07-19

				if(ma != INVALID_VALUE)
				{
					string strCount,strParamName,strParamValue,strParamOperate,strParamRelation,strReturn;
					bool bFind = FindNodeValue(ma, "sv_conditioncount", strCount);  //��ȡ��ֵ��������Ŀ �պ� 2007-07-18
					int iCount = atoi(strCount.c_str());
					for(int i=1; i<= iCount; i++)
					{
						//�޸Ķ�������������ֻ��ʾ���һ�������� �պ� 2007-07-19
						//szErrorValue = "[";
						strTempCon = "[";

						string paraIndex;
						char buf[65];
						itoa(i,buf,10);
						paraIndex = buf;
						strTemp = "sv_paramname" + paraIndex;
						bFind = FindNodeValue(ma, strTemp, strReturn);

						monitorNode = GetMonitorMainAttribNode(hMon);
						std::string getvalue;   //��getvalue���Ǽ�������ģ���е�ID �պ� 2007-07-18
						if ( FindNodeValue( monitorNode,"sv_monitortype",getvalue) )
						{			
							OUT_DEBUG(getvalue.c_str());					
							objTempMonitor = GetMonitorTemplet(atoi(getvalue.c_str())); //������ģ����� �պ� 2007-07-18
						}
						if(objTempMonitor != INVALID_VALUE)
						{
							//�ڼ�����ģ��ķ���ֵ�в��ҷ�ֵ���������� �պ� 2007-07-18
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

											//�޸Ķ�������������ֻ��ʾ���һ�������� �պ� 2007-07-19
											//szErrorValue += strParamName;
											strTempCon += strParamName;

										}
									}
								}
							}
						}	
						strTemp = "sv_operate" + paraIndex;
						bFind = FindNodeValue(ma, strTemp, strParamOperate); //��ȡ��ֵ�������ţ����硰>����<=���� �պ� 2007-07-18

						//�޸Ķ�������������ֻ��ʾ���һ�������� �պ� 2007-07-19
						//szErrorValue += strParamOperate;
						strTempCon += strParamOperate;

						strTemp = "sv_paramvalue" + paraIndex;
						bFind = FindNodeValue(ma, strTemp, strParamValue);	//��ȡ��ֵ �պ� 2007-07-18

						//�޸Ķ�������������ֻ��ʾ���һ�������� �պ� 2007-07-19
						//szErrorValue += strParamValue;
						//szErrorValue += "]";
						strTempCon += strParamValue;
						strTempCon += "]";

						szErrorValue += strTempCon; //�޸Ķ�������������ֻ��ʾ���һ�������� �պ� 2007-07-19 ����һ��

						if(i<iCount)
						{
							strTemp = "sv_relation" + paraIndex;
							bFind = FindNodeValue(ma, strTemp, strParamRelation); //��ȡ��ϵ�� �պ� 2007-07-18

							//�޸Ķ�������������ֻ��ʾ���һ�������� �պ� 2007-07-19
							//szErrorValue += strParamRelation;
							szErrorValue = szErrorValue + " " + strParamRelation + " "; //����ո�ʹ��ʾ������ �պ� 2007-07-18																	
						}
					}
				}								
				ParamList.clear();
				CloseMonitorTemplet(objTempMonitor);				
			}
		}
		CloseResource(objRes);
	}

	CloseMonitor(hMon); //�رռ��������� �պ� 2007-07-18
	OUT_DEBUG(szErrorValue.c_str());
	//++++++++++++++++++�������ӽ��� �պ� 2007-07-18++++++++++++++++++

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

	// ------ ȡ��ǰ�������������ı�����ֵ ------

	//strAlertContent += "\n��ֵ:     ";
	strAlertContent += "\n";
	strAlertContent += CAlertMain::strMonitorFazhi.c_str();
	strAlertContent += "       ";
	strAlertContent +=  szErrorValue;
	strAlertContent += "\n";
	//add end

	//Begin
	//�滻���и�ʽ���µ� @@Key@@ ���õ�Key�����õ���Ӧ�ļ�ֵ
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
			//�ļ�����
			strPath.Format("%s\\%s\\%s.txt", GetSiteViewRootPath().c_str(), "data\\Temp", strAlertMonitorId.c_str());

			//printf(strPath);

			//ȡ���ôα�����Ӧ�����ݲ��滻@@Log@@
			//����strTime�жϣ� ������һ��ʱ����ڡ����� ������һ��ʱ��С�ڡ�����
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
		//	strLocalContent=strLocalContent+"���趨"+strTemp;
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

//��ȡSchedule��MailList��Ϣ
bool CAlertEmailSendObj::GetInfoFromEmailAddress(string strAddressName)
{
	bool bReturn = true;
	//emailAdress.ini
	std::list<string> keylist;
	std::list<string>::iterator keyitem;

	//��ȡ��ַ�б�����
	if(GetIniFileSections(keylist, "emailAdress.ini"))
	{
		//��ʼ����ַ�б�����
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

	//��email.ini��ȡsmtp�����͵�ַ�ȷ��Ͳ���

	// SMTP ������
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

	// У���û�
	szComPort=GetIniFileString("SMSCommConfig", "Port", "",  "smsconfig.ini");

	//��ȡemail���յ�ַ��

	//�Ƿ��������ʼ�
	if(bUpgrade)
	{
		szSmsTo = strAlertUpgradeToValue;
	}
	else
	{
		//if(strSmsNumberValue != "����")
		if(strcmp(strSmsNumberValue.c_str(), CAlertMain::strOther.c_str()) != 0)
		{
			//�н�����

			//�ʼ����յ�ַ��ֹ�򷵻�
			if(!GetInfoFromSmsAddress(strSmsNumberValue))
			{
				DebugePrint("SmsAdress Disable");
				return true;
			}

			//��ʱ��ģ��ֵΪ�ʼ����յ�ַ�趨��ֵ��strEmailTemplateValue

			//�ʼ����յ�ַ�е�������������ݵ���ƥ���ж��Ƿ����ʼ�
			if(!CAlertMain::IsScheduleMatch(szSchedule))
			{
				//��¼��־������¼�����
				DebugePrint("Schedule Disable");
				return true;
			}
		}
		else
		{
			//�����ʼ���ַ
			szSmsTo = strOtherNumberValue;

			//��ʱ��ģ��ֵΪ��strEmailTemplateValue
		}
	}

	if(szSmsTo == "")
	{
		CAlertSendObj::InsertRecord(szSmsTo, 2, 0);
		return false;
	}

	//����AlertTitle��AlertContent
	MakeAlertTitle();
	MakeAlertContent();
	//int iEmailRet = 1;



	//����...	
	string strDebugInfo = "";
	strDebugInfo += "\r\n------------------���Ͷ��ſ�ʼ-----------------------------\r\n";
	strDebugInfo += ("���������Id��" + strAlertMonitorId + "\r\n");
	strDebugInfo += ("�����¼����ͣ���Sms��\r\n");
	strDebugInfo += ("strSmsNumberValue��" + strSmsNumberValue + "\r\n");
	strDebugInfo += ("szWebUser��" + szWebUser + "\r\n");
	strDebugInfo += ("szWebPwd��" + szWebPwd + "\r\n");
	strDebugInfo += ("szComPort��" + szComPort + "\r\n");
	strDebugInfo += ("szSmsTo��" + szSmsTo + "\r\n");
	strDebugInfo += ("strAlertTitle��" + strAlertTitle + "\r\n");
	strDebugInfo += ("strAlertContent��" + strAlertContent + "\r\n");
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nSendId);
	string strCount = chItem;
	sprintf(chItem, "%d", nEventCount);
	string strEventCount = chItem;
	sprintf(chItem, "%d", nEventType);
	string strEventType = chItem;
	strDebugInfo += ("�����¼���ţ�" + strCount + "\r\n");
	strDebugInfo += ("ԭʼ�¼���ţ�" + strEventCount + "\r\n");
	strDebugInfo += ("ԭʼ�¼����ͣ�" + strEventType + "\r\n");

	//����Ƕ���ֻ������ö��ŷָ��� 
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

		strDebugInfo += "------------------���Ͷ��Ž���------------------------------\r\n";
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
	strDebugInfo += "\r\n------------------�����¼���Ϣ��ʼ-----------------------------\r\n";
	strDebugInfo += ("���������Id��" + strAlertMonitorId + "\r\n");
	strDebugInfo += ("�����¼����ͣ���Sms��\r\n");
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nSendId);
	string strCount = chItem;
	sprintf(chItem, "%d", nEventCount);
	string strEventCount = chItem;
	sprintf(chItem, "%d", nEventType);
	string strEventType = chItem;
	strDebugInfo += ("�����¼���ţ�" + strCount + "\r\n");
	strDebugInfo += ("ԭʼ�¼���ţ�" + strEventCount + "\r\n");
	strDebugInfo += ("ԭʼ�¼����ͣ�" + strEventType + "\r\n");
	strDebugInfo += ("���������¼��ı�����" + strAlertName + "\r\n");
	strDebugInfo += "------------------�����¼���Ϣ����------------------------------\r\n";
	return strDebugInfo;
}

//���ɱ�������
void CAlertSmsSendObj::MakeAlertTitle()
{
	strAlertTitle += CAlertMain::GetMonitorTitle(strAlertMonitorId);
	strAlertTitle += "-";
	strAlertTitle += CAlertMain::GetDeviceTitle(strAlertMonitorId);
}

//���ɱ�������
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
		//strFreq = "���Ƶ��:";
		//strFreq += CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_frequency");
		//strFreq += "����";
		strFreq = CAlertMain::strMontorFreq.c_str();
		strFreq += ":";
		strFreq += CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_frequency");
		strFreq += CAlertMain::strMinute.c_str();

	}
	else
	{
		//strFreq = "���Ƶ��:";
		//strFreq += CAlertMain::GetMonitorPropValue(strAlertMonitorId, "_frequency");
		//strFreq += "Сʱ";

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
	//�滻���и�ʽ���µ� @@Key@@ ���õ�Key�����õ���Ӧ�ļ�ֵ
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
			//�ļ�����
			strPath.Format("%s\\%s\\%s.txt", GetSiteViewRootPath().c_str(), "data\\Temp", strAlertMonitorId.c_str());

			//printf(strPath);

			//ȡ���ôα�����Ӧ�����ݲ��滻@@Log@@
			//����strTime�жϣ� ������һ��ʱ����ڡ����� ������һ��ʱ��С�ڡ�����
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
		//	strLocalContent=strLocalContent+"���趨"+strTemp;
		//}
	}
	//End
}

//
void CAlertSmsSendObj::SetUpgradeTrue()
{
	bUpgrade = true;
}

//��ȡSchedule��Phone��Ϣ
bool CAlertSmsSendObj::GetInfoFromSmsAddress(string strAddressName)
{
	bool bReturn = true;
	//emailAdress.ini
	std::list<string> keylist;
	std::list<string>::iterator keyitem;

	//��ȡ��ַ�б�
	if(GetIniFileSections(keylist, "smsphoneset.ini"))
	{
		//��ʼ����ַ�б�����
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

//ͨ�����ڷ�ʽ���Ͷ���
bool CAlertSmsSendObj::SendSmsFromCom()
{
	ODS_DEFINE(_T(" for debug"));
	ODS_OUTDEBUG(_T("CAlertSmsSendObj::SendSmsFromCom ... "));

	CString strContent = "";
	//strContent += strAlertTitle.c_str();
	//strContent += "\n";
	//strContent += strAlertContent.c_str();

	//����Ƕ���ֻ������ö��ŷָ��� 
	std::list<string> listSms;
	std::list<string>::iterator listSmsItem;

	CAlertMain::ParserToken(listSms, szSmsTo.c_str(), ",");
	bool bSucess = true;
	bool bAllSucess = true;
	for(listSmsItem = listSms.begin(); listSmsItem!=listSms.end(); listSmsItem++)
	{
		CString strSmsTo = (*listSmsItem).c_str();

		// 2007/7/4 ���� �������������޶˱��жϵ�����
		strContent = strAlertContent.c_str();
		if(CAlertMain::SendSmsFromComm(strSmsTo, strContent) == -1)
			bSucess = false;

		/* �öδ���Ŀ�Ĳ���������ɶ������ı�ǿ���и�����ɶΣ�ÿ�����Ķ�<=50�ֽ� 2007/7/4 ����
		//����Ƕ���ֻ������ö��ŷָ��� 
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
// 	//����Ƕ���ֻ������ö��ŷָ��� 
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
// 				// �������ϢXML:
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
// 				//  ��Կ�ļ�������λ��:
// 				//_bstr_t bstrPublicKeyPath("D:\\Program Files\\SMSIIGatewayAPI\\PublicKey\\pub.txt");
// 				string strPath = "";
// 				strPath = GetSiteViewRootPath() + "\\fcgi-bin\\pub.txt";		
// 
// 				_bstr_t bstrPublicKeyPath(strPath.c_str());
// 				//
// 				//  �������˽��յ�ASPҳ���URL
// 				//_bstr_t bstrServerSiteURL("http://gateway.bjums.com/smssite/smsstart.asp");
// 				_bstr_t bstrServerSiteURL("http://sms.bmcc.com.cn/GatewayAPI/SMSIIGateWay.asp");
// 				//
// 				///////////////////////////////////////////////////////////
// 
// 
// 				///////////////////////////////////////////////////////////
// 				//
// 				// ��������ӿ�����Ҫ�Ĳ���
// 				//
// 				//  ��һ��:��Կ�ļ�������λ��
// 				//printf("��Կ�ļ�������λ��: %s\n", (char*)bstrPublicKeyPath);
// 				CAlertMain::pSender->SetPkpath(bstrPublicKeyPath);
// 				//pSender->SetPkpath(bstrPublicKeyPath);
// 				//
// 				//  �ڶ���:�������˽��յ�ASPҳ���URL
// 				//printf("�������˽��յ�ASPҳ���URL: %s\n\n", (char*)bstrServerSiteURL);
// 				CAlertMain::pSender->SetServerSite(bstrServerSiteURL);
// 				//pSender->SetServerSite(bstrServerSiteURL);
// 				//
// 				///////////////////////////////////////////////////////////
// 
// 
// 				///////////////////////////////////////////////////////////
// 				//
// 				// ʵ�ʷ��Ͷ���ϢXML��������
// 				//
// 				// ��һ������257�������RSA�����㷨
// 				// �ڶ����������Ƕ���ϢXML
// 
// 				//printf("ʵ�ʷ��Ͷ���ϢXML��������: %s\n\n", (char*)bstrSendSMSXML);
// 				//pSender->LoadSendXML("257",
// 				//	                 bstrSendSMSXML);
// 				//printf("ʵ�ʷ��Ͷ���ϢXML��������: %s\n\n", (char*)chSMSXML);
// 				CAlertMain::pSender->LoadSendXML("257", 
// 					chSMSXML);
// 				printf("ʵ�ʷ��Ͷ���ϢXML��������: %s\n\n", (char*)chSMSXML);
// 				//pSender->LoadSendXML("257", 
// 				//	chSMSXML);
// 
// 				//
// 				///////////////////////////////////////////////////////////
// 
// 
// 				///////////////////////////////////////////////////////////
// 				//
// 				// ���Ͷ���ϢXML֮�󣬿���ͨ���������������õ����������ķ���
// 				//  1:GetResponseText(ͨ�������������ȡ���������˷��ص�responseText)
// 				//  2:GetHTTPPostStatus(ͨ�������������ȡ���������˷��ص�״ֵ̬,��200��404��500֮���ֵ)
// 				//
// 				// ��һ������257�������RSA�����㷨
// 				// �ڶ����������Ƕ���ϢXML
// 				_bstr_t bstrServerResponseText = CAlertMain::pSender->GetResponseText();
// 				//_bstr_t bstrServerResponseText = pSender->GetResponseText();
// 				printf("�������˷��ص�responseText: %s\n", (char*)bstrServerResponseText);
// 				//
// 				long    lServerResponseStatus  = CAlertMain::pSender->GetHTTPPostStatus();
// 				//long    lServerResponseStatus  = pSender->GetHTTPPostStatus();
// 				printf("�������˷��ص�״ֵ̬: %d\n", lServerResponseStatus);
// 				char buf[256];
// 				char buf1[256];
// 				memset(buf, 0, 256);
// 				memset(buf1, 0, 256);
// 				sprintf(buf, "�������˷��ص�responseText: %s\n", (char*)bstrServerResponseText);
// 				sprintf(buf1, "�������˷��ص�״ֵ̬:%d\n", lServerResponseStatus);
// 				strcat(buf, buf1);
// 
// 				std::string sRet = buf;
// 				int bFind = sRet.find("�ύ�ɹ�", 0);
// 
// 				if(bFind >= 0)
// 				{
// 					//�ɹ�
// 					//bRet = true;
// 				}
// 				else
// 				{
// 					//ʧ��
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
		// �������ӵ�ָ��URL
		file = (CInternetFile*) session.OpenURL(theUrl); 
	}
	catch (...)
	{
		// ����д���Ļ���������Ϊ��
		strcpy(retState, "error");
		return FALSE;
	}

	if (file)
	{
		CString  somecode;

		bool flagReplace = false;
		int replaceNum = 0;
		while (file->ReadString(somecode) != NULL) //�������LPTSTR���ͣ���ȡ������nMax��0��ʹ�������ַ�ʱ����
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

//ͨ��Web��ʽ���Ͷ���
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

	//��ȥ���ڵĿո��
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

	// ��ԭʼ�������Ľ��п��ַ��ֶ�
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

	//����ǵ绰������Ҫ������֯��������ֻ֧�������������Ŷ�����Դ�ļ���
	//	1��szSmsTo���û�������ö��ŷָ�����ʽ�磺�˿� �����ļ�·��;�绰���� ����ԭ�е�strDllFuncParam��szSmsTo����̬������������
	//	2��szSmsTo�����ʽ��˿� �����ļ�·��;�绰���� ���szSmsTo�ֽ��strDllFuncParam��szSmsTo����̬������������
	//	3���绰������ʱ���ܲ�֧���ö��ŷָ������������Ρ�

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

			//����Ƕ���ֻ������ö��ŷָ��� 
			std::list<string> listSms;
			std::list<string>::iterator listSmsItem;

			CAlertMain::ParserToken(listSms, szSmsTo.c_str(), ",");
			bool bSucess = true;
			bool bAllSucess = true;
			for(listSmsItem = listSms.begin(); listSmsItem!=listSms.end(); listSmsItem++)
			{
				CString strSmsTo = (*listSmsItem).c_str();

				//����Ƕ���ֻ������ö��ŷָ��� 
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
	strDebugInfo += "\r\n------------------�����¼���Ϣ��ʼ-----------------------------\r\n";
	strDebugInfo += ("���������Id��" + strAlertMonitorId + "\r\n");
	strDebugInfo += ("�����¼����ͣ���Script��\r\n");
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nSendId);
	string strCount = chItem;
	sprintf(chItem, "%d", nEventCount);
	string strEventCount = chItem;
	sprintf(chItem, "%d", nEventType);
	string strEventType = chItem;
	strDebugInfo += ("�����¼���ţ�" + strCount + "\r\n");
	strDebugInfo += ("ԭʼ�¼���ţ�" + strEventCount + "\r\n");
	strDebugInfo += ("ԭʼ�¼����ͣ�" + strEventType + "\r\n");
	strDebugInfo += ("���������¼��ı�����" + strAlertName + "\r\n");
	strDebugInfo += "------------------�����¼���Ϣ����------------------------------\r\n";
	return strDebugInfo;
}

//
bool CAlertScriptSendObj::SendAlert()
{
	string strDebugInfo = "";
	strDebugInfo += "\r\n------------------���ͽű�������ʼ-----------------------------\r\n";

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
	strDebugInfo += ("���������Id��" + strAlertMonitorId + "\r\n");
	strDebugInfo += ("�����¼����ͣ���Script��\r\n");
	strDebugInfo += ("strServerTextValue��" + strServerTextValue + "\r\n");
	strDebugInfo += ("strScriptServerId��" + strScriptServerId + "\r\n");
	strDebugInfo += ("strScriptFileValue��" + strScriptFileValue + "\r\n");
	strDebugInfo += ("strScriptParamValue��" + strScriptParamValue + "\r\n");
	strDebugInfo += ("strUserName��" + strUserName + "\r\n");
	strDebugInfo += ("strUserPwd��" + strUserPwd + "\r\n");
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nSendId);
	string strCount = chItem;
	sprintf(chItem, "%d", nEventCount);
	string strEventCount = chItem;
	sprintf(chItem, "%d", nEventType);
	string strEventType = chItem;
	strDebugInfo += ("�����¼���ţ�" + strCount + "\r\n");
	strDebugInfo += ("ԭʼ�¼���ţ�" + strEventCount + "\r\n");
	strDebugInfo += ("ԭʼ�¼����ͣ�" + strEventType + "\r\n");
	strDebugInfo += "------------------���ͽű���������------------------------------\r\n";
	DebugePrint(strDebugInfo);

	return true;
}

//��ȡWindows���������û����ƺ�����
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

//ִ��Windows�ű�
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

	// 2007/7/25 ���� ���Ӵ����Խ�����ƽű��ļ�������ȷ��ȡ
	// �����һ�����ƽű�����ͨ��GetIniFileString���޷���ȷ��ýű��ļ����ģ�strScriptFileValue�������
	// ���ƽű����ļ���
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
			//�����ű�������
			CString strSpt=_T("");
			CString strInstallPath = _T("");
			strInstallPath = FuncGetInstallPath();

			// 2007/7/25 ���� ���´����б�ע������Ϊ���ʽ����ʽ���������޷���ȷִ�нű��ű��ļ�
			// strSpt.Format("cscript %s\\scripts\\%s %s",strInstallPath,strScriptFile,strScriptParam);

			// 2007/7/25 ���� ���½ű�ִ�д���ʽ������
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

//��ȡ�豸����ʱ����
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

//ִ��Unix�ű�
bool CAlertScriptSendObj::DoUnixScript()
{
	bool bReturn = false;

	string szQuery = GetDeviceRunParam(strScriptServerId);
	char szReturn [svBufferSize] = {0};
	int nSize = sizeof(szReturn);
	//int nSize = szQuery.length();
	char *pszQueryString = new char[szQuery.length()];

	//�ټ� "_Script=" + strScriptFile + " " + strScriptPara...

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
	strDebugInfo += "\r\n------------------�����¼���Ϣ��ʼ-----------------------------\r\n";
	strDebugInfo += ("���������Id��" + strAlertMonitorId + "\r\n");
	strDebugInfo += ("�����¼����ͣ���Sound��\r\n");
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nSendId);
	string strCount = chItem;
	sprintf(chItem, "%d", nEventCount);
	string strEventCount = chItem;
	sprintf(chItem, "%d", nEventType);
	string strEventType = chItem;
	strDebugInfo += ("�����¼���ţ�" + strCount + "\r\n");
	strDebugInfo += ("ԭʼ�¼���ţ�" + strEventCount + "\r\n");
	strDebugInfo += ("ԭʼ�¼����ͣ�" + strEventType + "\r\n");
	strDebugInfo += ("���������¼��ı�����" + strAlertName + "\r\n");
	strDebugInfo += "------------------�����¼���Ϣ����------------------------------\r\n";
	return strDebugInfo;
}

//
bool CAlertSoundSendObj::SendAlert()
{
	string strDebugInfo = "";
	strDebugInfo += "\r\n------------------��������������ʼ-----------------------------\r\n";
	strDebugInfo += ("���������Id��" + strAlertMonitorId + "\r\n");
	strDebugInfo += ("�����¼����ͣ���Sound��\r\n");
	strDebugInfo += ("strServerValue��" + strServerValue + "\r\n");
	strDebugInfo += ("strLoginNameValue��" + strLoginNameValue + "\r\n");
	strDebugInfo += ("strLoginPwdValue��" + strLoginPwdValue + "\r\n");
	char chItem[32]  = {0};	
	sprintf(chItem, "%d", nSendId);
	string strCount = chItem;
	sprintf(chItem, "%d", nEventCount);
	string strEventCount = chItem;
	sprintf(chItem, "%d", nEventType);
	string strEventType = chItem;
	strDebugInfo += ("�����¼���ţ�" + strCount + "\r\n");
	strDebugInfo += ("ԭʼ�¼���ţ�" + strEventCount + "\r\n");
	strDebugInfo += ("ԭʼ�¼����ͣ�" + strEventType + "\r\n");

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

	strDebugInfo += "------------------����������������------------------------------\r\n";
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
