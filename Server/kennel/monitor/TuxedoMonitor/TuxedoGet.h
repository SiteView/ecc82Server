#if !defined(AFX_TUXEDOGET_H__3B5F5012_87F2_4317_B692_E17AC7A033F8__INCLUDED_)
#define AFX_TUXEDOGET_H__3B5F5012_87F2_4317_B692_E17AC7A033F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "fml32.h"
#include "atmi.h"
#include "tpadm.h"

#define		__TIMEOUT__						"_TimeOut="
#define		__SERVERIP__					"_ServerIp="
#define		__SERVERPORT__					"_ServerPort="

#define		__TASKTYPE__					"_TaskType="
#define		__TASKPARAM__					"_TaskParam="

BOOL GetMachineList(const char *strParas, char * szReturn, int & nSize);
BOOL GetMachine(const char *strParas, char * szReturn, int & nSize);
BOOL GetServerList(const char *strParas, char * szReturn, int & nSize);
BOOL GetServer(const char *strParas, char * szReturn, int & nSize);
BOOL GetMsgList(const char *strParas, char * szReturn, int & nSize);
BOOL GetMsg(const char *strParas, char * szReturn, int & nSize);
BOOL GetClientList(const char *strParas, char * szReturn, int & nSize);
BOOL GetClient(const char *strParas, char * szReturn, int & nSize);
BOOL GetServiceList(const char *strParas, char * szReturn, int & nSize);
BOOL GetService(const char *strParas, char * szReturn, int & nSize);
BOOL GetWSHList(const char *strParas, char * szReturn, int & nSize);
BOOL GetWSH(const char *strParas, char * szReturn, int & nSize);

#endif // !defined(AFX_TUXEDOGET_H__3B5F5012_87F2_4317_B692_E17AC7A033F8__INCLUDED_)