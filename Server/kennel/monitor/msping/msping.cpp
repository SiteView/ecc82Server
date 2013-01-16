// msping.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "msping.h"
#include "Ping.h"
#include <base\funcGeneral.h>
#include <fstream>
#include <iostream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CMspingApp

BEGIN_MESSAGE_MAP(CMspingApp, CWinApp)
	//{{AFX_MSG_MAP(CMspingApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMspingApp construction

CMspingApp::CMspingApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMspingApp object
#define		__MACHINENAME__					"_MachineName="
#define		__TIMEOUT__						"_TimeOut="
#define		__SENDBYTES__					"_SendBytes="
#define		__CUSTOMERPATH__				"_CustomerPath="
#define		__SENDNUMS__					"_SendNums="
#define		__PINGHOST__					"_PingHost="
int WSA_Init();

void WSA_Free();


CMspingApp theApp;
int PrintLog(const char * strInfo)
{
#ifndef __DEBUG
	return 0;
#endif
	char datebuf[128]={0},timebuf[128]={0},tempbuf[10240]={0};
	_strdate(datebuf);
	_strtime(timebuf);
	sprintf(tempbuf,"%s-%s",datebuf,timebuf);
	ofstream FileStream;
	FileStream.open("PingMonitor.log",ios::app);
	FileStream<<tempbuf<<"\t"<<strInfo<<endl;
	FileStream.close();
	return 0;

}

/****************************************************************************
	Ping Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL PING(const char * strParas, char * szReturn, int& nSize)
//(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	PrintLog("========PING FUN========");
	PrintLog("监测器输入参数如下：");
	const char * strReceive=strParas;
	while(*strReceive)
	{
		PrintLog(strReceive);
		strReceive+=strlen(strReceive)+1;
	}

	PrintLog("监测器输入参数结束\n");

	CString		strHost = _T("");

	int			nTimeout = 5000, 
				nBytes = 64,
				nSendNums = 6;

	CString		strCustomerPath = _T("");
	CStringList paramList;

	MakeStringListByChar(paramList,strParas);


	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strHost = strTemp.Right(strTemp.GetLength() - (int)strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - (int)strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__SENDNUMS__, 0) == 0)
		{
			nSendNums = atoi(strTemp.Right(strTemp.GetLength() - (int)strlen(__SENDNUMS__)));
		}
		else if(strTemp.Find(__SENDBYTES__, 0) == 0)
		{
			nBytes = atoi(strTemp.Right(strTemp.GetLength() - (int)strlen(__SENDBYTES__)));
			if(nBytes > 256) nBytes = 256;
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - (int)strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strHost.IsEmpty())
	{
		PrintLog("输入主机地址错误！");
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_PING",
            "PING_IP_ADDRESS_NULL"));//<%IDS_Monitor_93%>"缺少主机名或IP地址"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		PrintLog("输入超时错误！");
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_PING",
            "PING_TIMEOUT_VALUE_ERROR"));//<%IDS_Monitor_94%>"输入超时错误"
		return FALSE;
	}

	if(nBytes <= 0 || nBytes > 256)
	{
		PrintLog("输入字节大小错误！");
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_PING",
            "PING_INPUT_BYTE_ERROR"));//<%IDS_Monitor_95%>"输入字节大小错误"
		return FALSE;
	}

	if(WSA_Init())
	{
		PrintLog("WSA_Init错误！");
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_96%>"通信初始化错误"
		return FALSE;
	}

//	nTimeout =2000;
//	nBytes =32;
	BOOL bResult = PING_MONITOR(strHost.GetBuffer(strHost.GetLength()), 
								nTimeout, 
								nBytes, 
								nSendNums,
								FALSE, 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn,nSize);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	Ping Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL PINGHOST(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strHost = _T("");

	int			nTimeout = 5000, 
				nBytes = 64,
				nSendNums = 6;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());
	CStringList paramList;

	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strHost = strTemp.Right(strTemp.GetLength() - (int)strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - (int)strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__SENDBYTES__, 0) == 0)
		{
			nBytes = atoi(strTemp.Right(strTemp.GetLength() - (int)strlen(__SENDBYTES__)));
			if(nBytes > 256) nBytes = 256;
		}
		else if(strTemp.Find(__SENDNUMS__, 0) == 0)
		{
			nSendNums = atoi(strTemp.Right(strTemp.GetLength() - (int)strlen(__SENDNUMS__)));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - (int)strlen(__CUSTOMERPATH__)));
		}
		else
		{
			
		}
	}

	if(strHost.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_PING",
            "PING_IP_ADDRESS_NULL"));//<%IDS_Monitor_93%>"缺少主机名或IP地址"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_PING",
            "PING_TIMEOUT_VALUE_ERROR"));//<%IDS_Monitor_94%>"输入超时错误"
		return FALSE;
	}

	if(nBytes <= 0 || nBytes > 256)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_PING",
            "PING_INPUT_BYTE_ERROR"));//<%IDS_Monitor_95%>"输入字节大小错误"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_96%>"通信初始化错误"
		return FALSE;
	}

//	nTimeout =2000;
//	nBytes =32;
	cout<<"PING_MONITOR nBytes="<<nBytes<<endl;
	BOOL bResult = PING_MONITOR(strHost.GetBuffer(strHost.GetLength()), 
								nTimeout, 
								nBytes, 
								nSendNums,
								FALSE, 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn,nSize);

	WSA_Free();

	return bResult;
}
