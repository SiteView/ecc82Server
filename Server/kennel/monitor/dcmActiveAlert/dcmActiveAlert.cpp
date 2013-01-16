// dcmActiveAlert.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "dcmActiveAlert.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "svapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//
//	注意！
//
//		如果此 DLL 动态链接到 MFC
//		DLL，从此 DLL 导出并
//		调入 MFC 的任何函数在函数的最前面
//		都必须添加 AFX_MANAGE_STATE 宏。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CdcmActiveAlertApp

BEGIN_MESSAGE_MAP(CdcmActiveAlertApp, CWinApp)
END_MESSAGE_MAP()


// CdcmActiveAlertApp 构造

CdcmActiveAlertApp::CdcmActiveAlertApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CdcmActiveAlertApp 对象

CdcmActiveAlertApp theApp;


// CdcmActiveAlertApp 初始化

BOOL CdcmActiveAlertApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
bool MakeStringListByChar(const char * strParas , CStringList &strList)
{
	const char * p = strParas;
	while(*p)
	{
		strList.AddTail(p);
		p += strlen(p) +1;
	}

	return true;
}

bool MakeCharByString(CString strInput , char * szReturn , int &nSize)
{
	int nLen = strInput.GetLength();
	if(nLen+1 > nSize)
	{
		sprintf(szReturn,"error=%s","The character string size is too larage!");
		return false;
	}
	char * p = szReturn;
	strcpy(p , strInput.GetBuffer(nLen));
	while(*p)
	{
		if(*p == '$')
			*p = '\0';
		p++;
	}

	return true;
}

void PrintLog(const char * strReceive)
{
#ifndef _DEBUG
	return;
#endif
	char timebuf[128] = {0} , datebuf[128] = {0} , tempbuf[1024] = {0};
	_strdate(datebuf);
	_strtime(timebuf);
	sprintf(tempbuf , "%s-%s" , datebuf , timebuf);

	ofstream fs;
	fs.open("DcmActiveAlert.log" , ios::app);
	fs<<tempbuf<<"\t"<<strReceive<<endl;
	fs.close();

	return;
}

bool CovertToLocalTime(CString & strGMTTime)
{
	char timebuf[128] = {0};
	sprintf(timebuf,"%s",strGMTTime.GetBuffer(strGMTTime.GetLength()));
	tm t1,*t2;

	if(strlen(timebuf)>0)
	{
		char *p = timebuf;
		while(*p)
		{
			if((*p == '-')||(*p == ':')||(*p == 'T')||(*p == 'Z'))
				*p = '\0';
			p++;
		}
		p=timebuf;
		t1.tm_year = atoi(p) - 1900;
		p += strlen(p) +1;
		t1.tm_mon = atoi(p) -1;
		p += strlen(p) +1;
		t1.tm_mday = atoi(p);
		p += strlen(p) + 1;
		t1.tm_hour = atoi(p);
		p += strlen(p) + 1;
		t1.tm_min = atoi(p);
		p += strlen(p) + 1;
		t1.tm_sec = atoi(p);


		time_t time1 = mktime(&t1);

		TIME_ZONE_INFORMATION tz;
		GetTimeZoneInformation(&tz);
		int UTC = (tz.Bias + tz.StandardBias) * (-60);
		time1 += UTC;
		t2 = localtime(&time1);
	}
	else
	{
		time_t now;
		time(&now);
		t2 = localtime(&now);
	}

	char strTime[128] = {0};
	sprintf(strTime,"%d-%d-%d %d:%d:%d" , t2->tm_year+1900 , t2->tm_mon+1 , t2->tm_mday , t2->tm_hour , t2->tm_min , t2->tm_sec);
	strGMTTime.Format("%s",strTime);

	return true;
}

extern "C" __declspec(dllexport)
BOOL dcmActiveAlert(const char *strParas , char * szReturn , int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	const char * strReceive = strParas;
	while(*strReceive)
	{
		PrintLog(strReceive);
		strReceive += strlen(strReceive) +1;
	}

	CString strEvent = _T("");
	CStringList strList;
	CString strMonitorId = _T("");

	MakeStringListByChar(strParas, strList);

	POSITION pos = strList.GetHeadPosition();

	while(pos)
	{
		CString strTemp = strList.GetNext(pos);
		if(strTemp.Find("alertEvent=") == 0)
		{
			strEvent = strTemp.Right(strTemp.GetLength() - static_cast<int>(strlen("alertEvent=")));
		}
		else if(strTemp.Find("_MonitorID=") == 0)
		{
			strMonitorId = strTemp.Right(strTemp.GetLength() - static_cast<int>(strlen("_MonitorID=")));
		}
	}
	

	char strAlertInfo[1024*10] = {0};
	char * p = strAlertInfo;
	sprintf(strAlertInfo ,"%s", strEvent.GetBuffer(strEvent.GetLength()));
	PrintLog("strAlertInfo的内容如下：");
	PrintLog(strAlertInfo);
	while(*p)
	{
		if(*p == '%')
			*p = '\0';
		p++;
	}
//	MakeCharByString(strActiveAlert , szReturn , nSize);
	char * strbuf = strAlertInfo;
	PrintLog("Begin********");
	strbuf++;
	while(*strbuf)
	{
		PrintLog(strbuf);
		strbuf += strlen(strbuf) +1;
	}
	PrintLog("********End");
	CStringList strEventList;
	strbuf = strAlertInfo +1;
	while(*strbuf)
	{
		strEventList.AddTail(strbuf);
		strbuf += strlen(strbuf) + 1;;
	}
//	MakeStringListByChar(strAlertInfo , strEventList);
	pos = strEventList.GetHeadPosition();

	CString strCustomId = _T("");
	CString strAlertType = _T("");
	CString strEntityID = _T("");
	CString strEventTime = _T("");
	CString strEntityLev = _T("");
	CString strEventInfo = _T("");
	while(pos)
	{
		CString strTmp = strEventList.GetNext(pos);
		if(strTmp.Find("CustomEventId=") == 0)
		{
			strCustomId = strTmp.Right(strTmp.GetLength() - static_cast<int>(strlen("CustomEventId=")));
			PrintLog(strCustomId);
		}
		else if(strTmp.Find("PredefinedEventType=") == 0)
		{
			strAlertType = strTmp.Right(strTmp.GetLength() - static_cast<int>(strlen("PredefinedEventType=")));
			PrintLog(strAlertType);
		}
		else if(strTmp.Find("EntityID=") == 0)
		{
			strEntityID = strTmp.Right(strTmp.GetLength() - static_cast<int>(strlen("EntityID=")));
			PrintLog(strEntityID);
		}
		else if(strTmp.Find("EventTime=") == 0)
		{
			strEventTime = strTmp.Right(strTmp.GetLength() - static_cast<int>(strlen("EventTime=")));
			PrintLog(strEventTime);
			CovertToLocalTime(strEventTime);
			PrintLog(strEventTime);
		}
		else if(strTmp.Find("SeverityLevel=") == 0)
		{
			strEntityLev = strTmp.Right(strTmp.GetLength() - static_cast<int>(strlen("SeverityLevel=")));
			PrintLog(strEntityLev);
		}
		else if(strTmp.Find("Info=") == 0)
		{
			strEventInfo = strTmp.Right(strTmp.GetLength() - static_cast<int>(strlen("Info=")));
			PrintLog(strEventInfo);
		}

	}

	if(strEventTime.GetLength()==0)
	{
		time_t now;
		time(&now);
		tm *t2 = localtime(&now);

		char strTime[128] = {0};
		sprintf(strTime,"%d-%d-%d %d:%d:%d" , t2->tm_year+1900 , t2->tm_mon+1 , t2->tm_mday , t2->tm_hour , t2->tm_min , t2->tm_sec);
		strEventTime.Format("%s",strTime);
	}
	if(strCustomId.GetLength() == 0)
		strCustomId=_T("2");
	if(strAlertType.GetLength() == 0)
		strAlertType = _T("2");
	if(strEntityID.GetLength() == 0)
		strEntityID = _T("2");
	if(strEntityLev.GetLength() == 0)
		strEntityLev = _T("CUSTOM");
	if(strEventInfo.GetLength() == 0)
		strEventInfo = _T("NULL");

	char alertType[1024] = {0};
	char alertDescription[1024*5] = {0};
	if(!strCustomId.IsEmpty())
	{
/*
		sprintf(alertType,"CustomEventId=%s",strCustomId.GetBuffer(strCustomId.GetLength()));
		sprintf( alertDescription, "EntityId=%s,EventTime=%s,SeverityLevel=%s,Info=%s",strEntityID,strEventTime,strEntityLev,strEventInfo);
		sprintf(szReturn,"alertType=%s$alertDescription=%s$",alertType , alertDescription);
*/
		sprintf(szReturn , "Severity=%s$alertType=%s$EventTime=%s$EntityID=%s$alertDescription=%s$",
			strEntityLev.GetBuffer(strEntityLev.GetLength()),
			strCustomId.GetBuffer(strCustomId.GetLength()),
			strEventTime.GetBuffer(strEventTime.GetLength()),
			strEntityID.GetBuffer(strEntityID.GetLength()),
			strEventInfo.GetBuffer(strEventInfo.GetLength())
			);

	}
	else
	{
/*
		sprintf(alertType,"PredefinedEventType=%s",strAlertType.GetBuffer(strAlertType.GetLength()));
		sprintf( alertDescription, "EntityId=%s,EventTime=%s,SeverityLevel=%s,Info=%s",strEntityID,strEventTime,strEntityLev,strEventInfo);
		sprintf(szReturn,"alertType=%s$alertDescription=%s$",alertType , alertDescription);
*/
		sprintf(szReturn , "Severity=%s$alertType=%s$EventTime=%s$EntityID=%s$alertDescription=%s$",
			strEntityLev.GetBuffer(strEntityLev.GetLength()),
			strAlertType.GetBuffer(strAlertType.GetLength()),
			strEventTime.GetBuffer(strEventTime.GetLength()),
			strEntityID.GetBuffer(strEntityID.GetLength()),
			strEventInfo.GetBuffer(strEventInfo.GetLength())
			);
	}


	CString strInput = szReturn;
	MakeCharByString(strInput , szReturn , nSize);
	PrintLog("========szReturn:========");
	char *strEnd = szReturn;
	while(*strEnd)
	{
		PrintLog(strEnd);
		strEnd += strlen(strEnd) + 1;
	}
	PrintLog("========End of monitor ========");
	OBJECT obj = Cache_GetMonitor(strMonitorId.GetBuffer(strMonitorId.GetLength()));
	MAPNODE node = GetMonitorParameter(obj);
	string strAlertMsg;
	if(FindNodeValue(node , "alertEvent" , strAlertMsg))
	{
		if(!strAlertMsg.empty())
		{
			strAlertMsg.clear();
			AddNodeAttrib(node , "alertEvent" , strAlertMsg);
			SubmitMonitor(obj);
		}
	}
	return TRUE;
}