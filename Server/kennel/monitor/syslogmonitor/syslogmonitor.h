// syslogmonitor.h : syslogmonitor DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// 主符号
#include <string>
#include <list>
#include <iostream>
using namespace std;

#include <scsvapi/svapi.h>
#include <scsvapi/svdbapi.h>
// CsyslogmonitorApp
// 有关此类实现的信息，请参阅 syslogmonitor.cpp
//

class CsyslogmonitorApp : public CWinApp
{
public:
	CsyslogmonitorApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

#define		__SOURCEIP__					"_IPName="
#define		__FACILITY__					"_Facelity="
#define		__LEVEL__					    "_Level="
#define		__MATCHSTRING__					"_Matchstring="
#define		__MONITORCONDITION__			"_monitorcondition="
#define		__MONITORID__			        "_MonitorID="

extern "C" __declspec(dllexport) 
BOOL GetSysLog(const char * strParas, char * szReturn,int& nSize);

//int nTotaleLines;
//int nMatchLines;
//std::string strSysLogMsgCond;
//std::string strSysLogIpCond;
//
//int nFacilityCond;
//int nSeveritiesCond;
//std::string strStartTimeCond;
//std::string strEndTimeCond;	
//std::string strDyn;
//
//TTime startTime;
//TTime endTime;

bool IsCondMatch(int nCond, string strCondValue, int nCondValue, string strStartTimeCond, string strEndTimeCond, string strSysLogMsgCond, string strSysLogIpCond, int nFacilityCond, int nSeveritiesCond, TTime startTime, TTime endTime);
BOOL ParserContent( char *  content,long &lTotalLine, long & lMatches ,char *matchstr,char *strmid,char * szResult);
void QueryRecordSet(string strTableName, string strStdMid, string strStartTimeCond, string strEndTimeCond, string strSysLoginMsgCond, string strSysLoginIpCond, int nFacilityCond, int nSeveritiesCond, int &nMatchLines);
bool WriteLogTime(string strMid);
bool WriteLogData(string strMid, string strQSysLogTime, string strQSysLogIp, string strQSysLogMsg, string strFacility, string strLevel);
string GetSeveritiesStrFormInt(int nSeverities);
string GetFicilityStrFormInt(int nFacility);