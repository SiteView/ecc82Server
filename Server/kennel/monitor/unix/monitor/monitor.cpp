// Monitor.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Monitor.h"

#include "base\funcGeneral.h"
#include <list>

#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	MAX_REC_LEN     100*1024


#ifdef WIN32
#pragma warning (disable : 4267)
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
// CMonitorApp

BEGIN_MESSAGE_MAP(CMonitorApp, CWinApp)
	//{{AFX_MSG_MAP(CMonitorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonitorApp construction

CMonitorApp::CMonitorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}
/*
BOOL CMonitorApp::InitInstance()
{
	return TRUE;
}

int CMonitorApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWinApp::ExitInstance();
}
*/
/////////////////////////////////////////////////////////////////////////////
// The one and only CMonitorApp object

#include "Utils.h"
#include <ras.h>
#include <raserror.h>

#include "Ping.h"			// Ping Monitor
#include "Port.h"			// Port Monitor
#include "Apache.h"			// Apache Monitor
#include "iPlanet.h"		// Netscape iPlanet Monitor
#include "SilverStream.h"	// Silver Stream Monitor
#include "SNMP.h"			// SNMP Monitor
#include "DNS.h"			// DNS Monitor
#include "TelMonitor.h"		// Server Monitor Based On Telnet Protocol
#include "SSHMonitor.h"		// Server Monitor Based On SSH Protocol
#include "HttpMonitor.h"	// Server Monitor Based On HTTP Protocol
#include "RloginMonitor.h"	// Server Monitor Based On Rlogin Protocol
#include "News.h"			// News Monitor
#include "Radius.h"			// RADIUS Monitor
#include "FTP.h"			// FTP Monitor
#include "DB.h"				// DB Monitor
#include "LDAP.h"			// LDAP Monitor
#include "CheckPoint.h"		// CheckPoint FireWall-1 Monitor
#include "Mail.h"			// Mail Monitor
#include "Traffic.h"		// Traffic Monitor
#include "Interfaces.h"		// Get Interfaces
#include "Dynamo.h"			// Dynamo Application Server Monitor
#include "URL.h"			// URL Monitor
#include "GTJA.h"			// Special For GTJA (foolish)
#include "MAPIProto.h"		// MAPI Monitor
#include "CiscoLog.h"		// Cisco Router Logging Monitor
#include "CiscoCPU.h"		// Cisco Router CPU Percentage Monitor
#include "F5.h"				// F5 Big-IP Monitor
#include "DHCP.h"			// DHCP Server Monitor
#include "MQ.h"				// MQ Monitor
#include "WebLogic5x.h"		// WebLogic 5.x Application Server Monitor
#include "WebLogic7x.h"		// WebLogic 7.x Application Server Monitor
#include "TRAP.h"			// SNMP Trap Monitor
#include "Websphere.h"		// Websphere Monitor
#include "NetScreen.h"		// NetScreen Firewall Monitor
#include "LogFile.h"		// Log File Monitor
#include "AIXErrpt.h"		// AIX Errpt Monitor
#include "Sybase.h"			// Sybase Monitor
#include "Bandwidth.h"		// Bandwidth Monitor
#include "Script.h"			// Script Monitor
#include "RemotePing.h"		// Remote Ping Monitor
#include "VPN.h"			// VPN Connection Monitor
#include "Cluster.h"		// Cluster Status Monitor
#include "Ctron.h"			// CableTron Related Monitor
#include "Foundry.h"		// Foundry Recource Monitor

CMonitorApp theApp;
void DebugInfo(const char * szMsg);



#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
int WriteLog( const char* str )
{
	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	char szLogFile[] = "monitor.log";

	// �ж��ļ���С���ڲ����ļ��������ʵ��
	struct _stat buf;
	if( _stat( szLogFile, &buf ) == 0 )
	{
		if( buf.st_size > 100*1024 )
		{
			FILE *log = fopen( szLogFile, "w" );
			if( log != NULL )
				fclose( log );
		}
	}


	FILE *log = fopen( szLogFile,"a+");
	if( log != NULL )
	{
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fclose( log );
	}

	return 1;
}



void WriteLogFile(const char* szMsg)
{
    FILE *stream;
    char chTime[50] = {0};
    //���ļ�
#if _DEBUG
    stream=fopen(FuncGetInstallRootPath() + "\\Temp\\Monitor.log","a");
#else
    stream=fopen(FuncGetInstallRootPath() + "\\Monitormanager\\Temp\\Monitor.log","a");
#endif
    if(stream == NULL)
        return;//���ļ�ʧ�ܣ�ֱ�ӷ���
   	SYSTEMTIME time;
	//ȡϵͳ��ǰʱ��
	GetLocalTime(&time);
    //sprintf(chTime ,"%02d��-%02d��-%02d�� %02dʱ-%02d��-%02d�� ",
    sprintf(chTime ,"%dY-%dM-%dD %dH-%dM-%dS ",
				//time.wYear - 2000,
                time.wYear,
				time.wMonth,
				time.wDay,
				time.wHour,
				time.wMinute,
				time.wSecond
			);
    fputs(chTime , stream);
	fputs(szMsg,stream);
    fputs("\n",stream);
	fclose(stream);
}
/****************************************************************************
	AIX Errpt Monitor Export Function
****************************************************************************/
bool TELAIXERRPT(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strMid = _T(""),
				strIniFile = _T("");
	int			nPort = 23;

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	int			nGID = 0;
				//nMID = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__GROUPID__, 0) == 0)
		{
			nGID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__GROUPID__)));
		}
		else if(strTemp.Find(__MONITORID__, 0) == 0)
		{
			//nMID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__)));
			strMid=strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_73%>"));//<%IDS_Monitor_73%>"ȱ�ٷ�����"
		return false;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_74%>"));//<%IDS_Monitor_74%>"����˿ڴ���"
		return false;
	}

	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_75%>"));//<%IDS_Monitor_75%>"ȱ���û���"
		return false;
	}

	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_77%>"));//<%IDS_Monitor_77%>"�����¼��ʾ����������ʾ������ʾ��"
		return false;
	}
	
	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_78%>"));//<%IDS_Monitor_78%>"ͨ�ų�ʼ������"
		return false;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	bool ret = AIXERRPT_TEL_VALUE(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								strProxyServer.GetBuffer(strProxyServer.GetLength()), 
								nProxyPort, 
								strProxyUid.GetBuffer(strProxyUid.GetLength()), 
								strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
								strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
								strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
								strPrompt.GetBuffer(strPrompt.GetLength()), 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								nGID, 
								strMid.GetBuffer(strMid.GetLength()),
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);

	WSA_Free();
    WriteLogFile(szReturn);
	return ret;
}

BOOL SSHAIXERRPT(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""), 
				strMid = _T(""),
				strIniFile = _T(""),
				strPrivateKeyFile = _T("");
	int			nPort = 22;

	int			nGID = 0;
				//nMID = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__GROUPID__, 0) == 0)
		{
			nGID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__GROUPID__)));
		}
		else if(strTemp.Find(__MONITORID__, 0) == 0)
		{
//			nMID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__)));
			strMid=strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else if(strTemp.Find(__PRIKEYPATH__, 0) == 0)
		{
			strPrivateKeyFile = strTemp.Right(strTemp.GetLength() - strlen(__PRIKEYPATH__));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_79%>"));//<%IDS_Monitor_79%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_80%>"));//<%IDS_Monitor_80%>"����˿ڴ���"
		return FALSE;
	}

	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_81%>"));//<%IDS_Monitor_81%>"ȱ���û���"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_83%>"));//<%IDS_Monitor_83%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL ret = AIXERRPT_SSH_VALUE(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								nGID, 
								//nMID, 
								strMid.GetBuffer(strMid.GetLength()), 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								strPrivateKeyFile.GetBuffer(strPrivateKeyFile.GetLength()),
								szReturn);

	WSA_Free();

	return ret;
}


extern "C" __declspec(dllexport) 
bool AIXERRPT(const char *strParas, char * szReturn, int & nSize)
//(CStringList &paramList, char *szReturn)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int nProtoType = 0;
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROTOCOLTYPE__, 0) == 0)
		{
			nProtoType =atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOLTYPE__)));
			break;
		}
	}

	if(nProtoType == 0) nProtoType = 2;

    bool bRet = false;

	switch(nProtoType)
	{
	case 1:
		bRet = SSHAIXERRPT(paramList, szReturn);
        break;
	case 2:
		bRet = TELAIXERRPT(paramList, szReturn);
        break;
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_84%>"));//<%IDS_Monitor_84%>"Э�����ʹ���"
        break;
	}

	return bRet;
}

/****************************************************************************
	Apache Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL APACHE(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strURL = _T(""), 
				strUser = _T(""), 
				strPass = _T("");
	int			nTimeout = 60;

	CString		strProxyServer = _T(""), 
				strProxyUser = _T(""), 
				strProxyPass = _T("");
	int			nProxyPort = 8080;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	// NT Chalenge

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERMANURL__, 0) == 0)
		{
			strURL = strTemp.Right(strTemp.GetLength() - strlen(__SERVERMANURL__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUser = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPass = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
	}

	if(strURL.IsEmpty())
	{
		// Input URL Empty
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_1%>"));//<%IDS_Monitor_1%>"ȱ��URL"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		// Input Timeout Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_2%>"));//<%IDS_Monitor_2%>"���볬ʱ����"
		return FALSE;
	}

	if(!strProxyServer.IsEmpty() && nProxyPort <= 0)
	{
		// Input Proxy Port Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_3%>"));//<%IDS_Monitor_3%>"����������˿ڴ���"
		return FALSE;
	}

	return APACHE_MONITOR(strURL.GetBuffer(strURL.GetLength()), 
						nTimeout, 
						strUser.GetBuffer(strUser.GetLength()), 
						strPass.GetBuffer(strPass.GetLength()), 
						strProxyServer.GetBuffer(strProxyServer.GetLength()), 
						nProxyPort, 
						strProxyUser.GetBuffer(strProxyUser.GetLength()), 
						strProxyPass.GetBuffer(strProxyPass.GetLength()), 
						strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
						szReturn);
}

/****************************************************************************
	Apache Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL APACHE_2_0_34(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strURL = _T(""), 
				strUser = _T(""), 
				strPass = _T("");
	int			nTimeout = 60;

	CString		strProxyServer = _T(""), 
				strProxyUser = _T(""), 
				strProxyPass = _T("");
	int			nProxyPort = 8080;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	// NT Chalenge

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERMANURL__, 0) == 0)
		{
			strURL = strTemp.Right(strTemp.GetLength() - strlen(__SERVERMANURL__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUser = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPass = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
	}

	if(strURL.IsEmpty())
	{
		// Input URL Empty
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_URL",
            "URL_LOST_ADDRESS"));//<%IDS_Monitor_1%>"ȱ��URL"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		// Input Timeout Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_URL",
            "URL_TIMEOUT_VALUE_ERROR"));//<%IDS_Monitor_2%>"���볬ʱ����"
		return FALSE;
	}

	if(!strProxyServer.IsEmpty() && nProxyPort <= 0)
	{
		// Input Proxy Port Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_URL",
            "URL_PROXY_PORT_ERROR"));//<%IDS_Monitor_3%>"����������˿ڴ���"
		return FALSE;
	}

	return APACHE_2_0_34_MONITOR(strURL.GetBuffer(strURL.GetLength()), 
								nTimeout, 
								strUser.GetBuffer(strUser.GetLength()), 
								strPass.GetBuffer(strPass.GetLength()), 
								strProxyServer.GetBuffer(strProxyServer.GetLength()), 
								nProxyPort, 
								strProxyUser.GetBuffer(strProxyUser.GetLength()), 
								strProxyPass.GetBuffer(strProxyPass.GetLength()), 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);
}

/****************************************************************************
	Bandwidth Monitor Of Some Interface Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL BANDWIDTH(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strCommunity = _T("public");
	int		nPort = 161, 
			nIndex = 0, 
			nTimeout = 5, 
			nRetryDelay = 1;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			nIndex = atoi(strTemp.Right(strTemp.GetLength() - strlen(__INDEX__)));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_28%>"));//<%IDS_Monitor_28%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_29%>"));//<%IDS_Monitor_29%>"����˿ڴ���"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_31%>"));//<%IDS_Monitor_31%>"���볬ʱ����"
		return FALSE;
	}

	if(nRetryDelay < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_32%>"));//<%IDS_Monitor_32%>"����ʱ��������"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_Monitor_33%>"));//<%IDS_Monitor_33%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = BANDWIDTH_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strCommunity.GetBuffer(strCommunity.GetLength()), 
									nIndex, 
									nRetryDelay, 
									nTimeout, 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									szReturn);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	CableTron SSR CPU Utilization Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport)
BOOL CTRONSSRCPU(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strServer = _T(""), 
				strCommunity = _T("public");
	int			nPort = 161, 
				nRetryDelay = 1, 
				nTimeout = 5;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		// Input Server Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_4%>"));//<%IDS_Monitor_4%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		// Input Timeout Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_6%>"));//<%IDS_Monitor_6%>"���볬ʱ����"
		return FALSE;
	}

	if(nRetryDelay < 0)
	{
		// Input Retry Delay Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_7%>"));//<%IDS_Monitor_7%>"��������ʱ��������"
		return FALSE;
	}

	if(WSA_Init())
	{
		// WSA Initialize Fail
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_8%>"));//<%IDS_Monitor_8%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = CTRONSSRCPU_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strCommunity.GetBuffer(strCommunity.GetLength()), 
									nRetryDelay, 
									nTimeout, 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									szReturn);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	CableTron Ethernet Status Monitor [RMON] Export Function
****************************************************************************/
extern "C" __declspec(dllexport)
BOOL CTRONETHERSTATUS(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strServer = _T(""), 
				strCommunity = _T("public"), 
				strIndex = _T("0");
	int			nPort = 161, 
				nRetryDelay = 1, 
				nTimeout = 5;
	int			nGID = 0, nMID = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			strIndex = strTemp.Right(strTemp.GetLength() - strlen(__INDEX__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else if(strTemp.Find(__GROUPID__, 0) == 0)
		{
			nGID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__GROUPID__)));
		}
		else if(strTemp.Find(__MONITORID__, 0) == 0)
		{
			nMID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		// Input Server Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_4%>"));//<%IDS_Monitor_4%>"ȱ�ٷ�����"
		return FALSE;
	}

	for(int i = 0;i < strIndex.GetLength();i ++)
	{
		TCHAR ch = strIndex.GetAt(i);
		if(ch != '.' && !isdigit(ch))
		{
			// Input Request OID Index Error
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_5%>"));//<%IDS_Monitor_5%>"�������������������"
			return FALSE;
		}
	}

	if(nTimeout <= 0)
	{
		// Input Timeout Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_6%>"));//<%IDS_Monitor_6%>"���볬ʱ����"
		return FALSE;
	}

	if(nRetryDelay < 0)
	{
		// Input Retry Delay Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_7%>"));//<%IDS_Monitor_7%>"��������ʱ��������"
		return FALSE;
	}

	if(WSA_Init())
	{
		// WSA Initialize Fail
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_8%>"));//<%IDS_Monitor_8%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = CTRONETHERSTATUS_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
											nPort, 
											strCommunity.GetBuffer(strCommunity.GetLength()), 
											strIndex, 
											nRetryDelay, 
											nTimeout, 
											nGID, 
											nMID, 
											strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
											szReturn);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	CableTron Ethernet Status Monitor [RMON] Export Function
****************************************************************************/
extern "C" __declspec(dllexport)
BOOL CTRONETHERINDEX(CStringList &paramList, CStringList &lstTexts, CStringList &lstValues)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strServer = _T(""), 
				strCommunity = _T("public");
	int			nPort = 161, 
				nRetryDelay = 1, 
				nTimeout = 5;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		return FALSE;
	}

	if(nRetryDelay < 0)
	{
		return FALSE;
	}

	if(WSA_Init())
	{
		return FALSE;
	}

	CString strBuffer = _T("");
	BOOL bResult = CtronEtherIndexs(strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strCommunity.GetBuffer(strCommunity.GetLength()), 
									nRetryDelay, 
									nTimeout, 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									strBuffer);

	WSA_Free();

	if(bResult == TRUE)
	{
		char buff[256] = {0};
		char *ca = strBuffer.GetBuffer(strBuffer.GetLength()), *cb = NULL, *cc = NULL;
		while(cb = strstr(ca, "$$$"))
		{
			ca[cb - ca] = 0;
			
			cc = strstr(ca, "###");
			memset(buff, 0, sizeof(buff));
			strncpy(buff, ca, cc - ca);
			lstValues.AddTail(buff);
			ca = cc + 3;

			cc = strstr(ca, "###");
			memset(buff, 0, sizeof(buff));
			strncpy(buff, ca, cc - ca);
			lstTexts.AddTail(buff);
			ca = cc + 3;

			ca = cb + 3;
		}
	}

	return bResult;
}

/****************************************************************************
	CheckPoint Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL CHECKPOINT(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strServer = _T(""), 
				strCommunity = _T("public"), 
				strIndex = _T("0");
	int			nPort = 161, 
				nRetryDelay = 1, 
				nTimeout = 5;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			strIndex = strTemp.Right(strTemp.GetLength() - strlen(__INDEX__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		// Input Server Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_4%>"));//<%IDS_Monitor_4%>"ȱ�ٷ�����"
		return FALSE;
	}

	for(int i = 0;i < strIndex.GetLength();i ++)
	{
		TCHAR ch = strIndex.GetAt(i);
		if(ch != '.' && !isdigit(ch))
		{
			// Input Request OID Index Error
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_5%>"));//<%IDS_Monitor_5%>"�������������������"
			return FALSE;
		}
	}

	if(nTimeout <= 0)
	{
		// Input Timeout Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_6%>"));//<%IDS_Monitor_6%>"���볬ʱ����"
		return FALSE;
	}

	if(nRetryDelay < 0)
	{
		// Input Retry Delay Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_7%>"));//<%IDS_Monitor_7%>"��������ʱ��������"
		return FALSE;
	}

	if(WSA_Init())
	{
		// WSA Initialize Fail
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_8%>"));//<%IDS_Monitor_8%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = CHECKPOINT_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strCommunity.GetBuffer(strCommunity.GetLength()), 
									strIndex, 
									nRetryDelay, 
									nTimeout, 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									szReturn);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	Cluster Monitor Export Function 
*****************************************************************************/
BOOL TELCLUSTER(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T("");
	int			nPort = 23; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strClusterLog = _T("");

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__CLUSTERLOG__, 0) == 0)
		{
			strClusterLog = strTemp.Right(strTemp.GetLength() - strlen(__CLUSTERLOG__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_129%>"));//<%IDS_Monitor_129%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_130%>"));//<%IDS_Monitor_130%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_131%>"));//<%IDS_Monitor_131%>"ȱ���û���"
		return FALSE;
	}
	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_132%>"));//<%IDS_Monitor_132%>"�����¼��ʾ����������ʾ������ʾ��"
		return FALSE;
	}
	if(strClusterLog.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("%IDS_Monitor_223%>"));
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_133%>"));//<%IDS_Monitor_133%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	BOOL ret = TELCLUSTER_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								strProxyServer.GetBuffer(strProxyServer.GetLength()), 
								nProxyPort, 
								strProxyUid.GetBuffer(strProxyUid.GetLength()), 
								strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
								strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
								strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
								strPrompt.GetBuffer(strPrompt.GetLength()), 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								strClusterLog.GetBuffer(strClusterLog.GetLength()), 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);

	WSA_Free();

	return ret;
}

BOOL SSHCLUSTER(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T("");
	int			nPort = 22;

	CString		strClusterLog = _T("");

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__CLUSTERLOG__, 0) == 0)
		{
			strClusterLog = strTemp.Right(strTemp.GetLength() - strlen(__CLUSTERLOG__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_IP_ADDRESS_NULL"));//<%IDS_Monitor_134%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_135%>"));//<%IDS_Monitor_135%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_136%>"));//<%IDS_Monitor_136%>"ȱ���û���"
		return FALSE;
	}
	if(strClusterLog.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("%IDS_Monitor_223%>"));
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_137%>"));//<%IDS_Monitor_137%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL ret = SSHCLUSTER_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								strClusterLog.GetBuffer(strClusterLog.GetLength()), 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);

	WSA_Free();

	return ret;
}

BOOL HTTPCLUSTER(CStringList &paramList, char *szReturn)
{
	CString		strRemoteUrl = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T("");

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strClusterLog = _T("");

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strRemoteUrl = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CLUSTERLOG__, 0) == 0)
		{
			strClusterLog = strTemp.Right(strTemp.GetLength() - strlen(__CLUSTERLOG__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strRemoteUrl.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_129%>"));//<%IDS_Monitor_129%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(strClusterLog.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("%IDS_Monitor_223%>"));
		return FALSE;
	}

	return HTTPCLUSTER_MONITOR(strRemoteUrl.GetBuffer(strRemoteUrl.GetLength()), 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strIniFile.GetBuffer(strIniFile.GetLength()), 
							strClusterLog.GetBuffer(strClusterLog.GetLength()), 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);
}

BOOL RLOGINCLUSTER(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T("");
	int			nPort = 513; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strClusterLog = _T("");

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__CLUSTERLOG__, 0) == 0)
		{
			strClusterLog = strTemp.Right(strTemp.GetLength() - strlen(__CLUSTERLOG__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_129%>"));//<%IDS_Monitor_129%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_130%>"));//<%IDS_Monitor_130%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_131%>"));//<%IDS_Monitor_131%>"ȱ���û���"
		return FALSE;
	}
	if(strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_132%>"));//<%IDS_Monitor_132%>"�����¼��ʾ����������ʾ������ʾ��"
		return FALSE;
	}
	if(strClusterLog.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("%IDS_Monitor_223%>"));
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_133%>"));//<%IDS_Monitor_133%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	BOOL ret = RLOGINCLUSTER_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strUID.GetBuffer(strUID.GetLength()), 
									strPWD.GetBuffer(strPWD.GetLength()), 
									strProxyServer.GetBuffer(strProxyServer.GetLength()), 
									nProxyPort, 
									strProxyUid.GetBuffer(strProxyUid.GetLength()), 
									strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
									strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
									strPrompt.GetBuffer(strPrompt.GetLength()), 
									strIniFile.GetBuffer(strIniFile.GetLength()), 
									strClusterLog.GetBuffer(strClusterLog.GetLength()), 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									szReturn);

	WSA_Free();

	return ret;
}

extern "C" __declspec(dllexport)
BOOL CLUSTER(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int nProtoType = 0;
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROTOCOLTYPE__, 0) == 0)
		{
			nProtoType =atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOLTYPE__)));
			break;
		}
	}

	if(nProtoType == 0) nProtoType = 2;

	switch(nProtoType)
	{
	case 1:
		return SSHCLUSTER(paramList, szReturn);
	case 2:
		return TELCLUSTER(paramList, szReturn);
	case 3:
		return HTTPCLUSTER(paramList, szReturn);
	case 4:
		return RLOGINCLUSTER(paramList, szReturn);
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_PROTOCOL_TYPE_ERROR"));//<%IDS_Monitor_138%>"Э�����ʹ���"
		return FALSE;
	}

	return TRUE;
}

/****************************************************************************
	Database Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL DB(const char *strParas,  char *szReturn,int & nSize)
//(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strDBConn = _T(""), 
				strQuery = _T("");

	CString		strDBMatch = _T(""), 
				strDBUser = _T(""), 
				strDBPass = _T(""), 
				strDBDriver = _T(""), 
				strQFile = _T("");
	int			nConnTimeout = 60, 
				nQTimeout = 60;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__DSNNAME1__, 0) == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__DSNNAME1__));
		}
		else if(strTemp.Find(__SQLSTR__, 0) == 0)
		{
			strQuery = strTemp.Right(strTemp.GetLength() - strlen(__SQLSTR__));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strDBMatch = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strDBPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__FILEPATH__, 0) == 0)
		{
			strQFile = strTemp.Right(strTemp.GetLength() - strlen(__FILEPATH__));
		}
		else if(strTemp.Find(__CONNECTTIMEOUT__, 0) == 0)
		{
			nConnTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__CONNECTTIMEOUT__)));
		}
		else if(strTemp.Find(__QUERYTIMEOUT__, 0) == 0)
		{
			nQTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__QUERYTIMEOUT__)));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strDBConn.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_14%>"));//<%IDS_Monitor_14%>"ȱ�����ݿ�����"
		return FALSE;
	}

	if(strQuery.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_15%>"));//<%IDS_Monitor_15%>"ȱ�ٲ�ѯ���"
		return FALSE;
	}

	if(nConnTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_16%>"));//<%IDS_Monitor_16%>"�������ӳ�ʱ����"
		return FALSE;
	}

	if(nQTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_17%>"));//<%IDS_Monitor_17%>"�����ѯ��ʱ����"
		return FALSE;
	}

	BOOL bReturn= DB_MONITOR(strDBConn.GetBuffer(strDBConn.GetLength()), 
					strQuery.GetBuffer(strQuery.GetLength()), 
					nConnTimeout, 
					nQTimeout, 
					strDBUser.GetBuffer(strDBUser.GetLength()), 
					strDBPass.GetBuffer(strDBPass.GetLength()), 
					strDBDriver.GetBuffer(strDBDriver.GetLength()), 
					strQFile.GetBuffer(strQFile.GetLength()), 
					strDBMatch.GetBuffer(strDBMatch.GetLength()), 
					strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
					szReturn);
	CString strTmp=szReturn;
	MakeCharByString(szReturn,nSize,strTmp);
	return bReturn;
}

/****************************************************************************
	DNS Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL DNS(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strDNSServ = _T(""), 
				strDomain = _T("");
	int			nDNSPort = 53;

	CStringList	lstAddress;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strDNSServ = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nDNSPort = atoi(strTemp);
			}
			else
			{
				strDNSServ = strTemp;
			}
		}
		else if(strTemp.Find(__MACHINENAME__,0)==0)
		{
			strDNSServ=strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
        else if(strTemp.Find(__PORT__,0)==0)
        {
            nDNSPort = atoi(strTemp.Right(strTemp.GetLength()-strlen(__PORT__)));
        }
		else if(strTemp.Find(__DOMAINNAME__, 0) == 0)
		{
			strDomain = strTemp.Right(strTemp.GetLength() - strlen(__DOMAINNAME__));
		}
		else if(strTemp.Find(__MATCHIP__, 0) == 0)
		{
			int index = 0;
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__MATCHIP__));
			while(1)
			{
				index = strTemp.Find(',', 0);
				if(index > 0)
				{
					lstAddress.AddTail(strTemp.Left(index));
					strTemp.Delete(0, index + 1);
				}
				else if(index == 0)
				{
					strTemp.Delete(0, 1);
				}
				else
				{
					if(strTemp.IsEmpty())
						break;
					else
					{
						lstAddress.AddTail(strTemp);
						break;
					}
				}
			}
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strDNSServ.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_22%>"));//<%IDS_Monitor_22%>"ȱ��DNS��������ַ"
		return FALSE;
	}

	if(strDomain.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_23%>"));//<%IDS_Monitor_23%>"ȱ��Ҫ��ѯIP��ַ������"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_24%>"));//<%IDS_Monitor_24%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = DNS_MONITOR(strDNSServ.GetBuffer(strDNSServ.GetLength()), 
							nDNSPort, 
							strDomain.GetBuffer(strDomain.GetLength()), 
							lstAddress, 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	Dynamo Application Server Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL DYNAMO(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strCommunity = _T("public"), 
			strIndex = _T("0");
	int		nPort = 8870, 
			nTimeout = 5, 
			nRetryDelay = 1;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			strIndex = strTemp.Right(strTemp.GetLength() - strlen(__INDEX__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_28%>"));//<%IDS_Monitor_28%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_29%>"));//<%IDS_Monitor_29%>"����˿ڴ���"
		return FALSE;
	}

	for(int i = 0;i < strIndex.GetLength();i ++)
	{
		TCHAR ch = strIndex.GetAt(i);
		if(ch != '.' && !isdigit(ch))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_30%>"));//<%IDS_Monitor_30%>"�������������������"
			return FALSE;
		}
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_31%>"));//<%IDS_Monitor_31%>"���볬ʱ����"
		return FALSE;
	}

	if(nRetryDelay < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_32%>"));//<%IDS_Monitor_32%>"����ʱ��������"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_Monitor_33%>"));//<%IDS_Monitor_33%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = DYNAMO_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strCommunity.GetBuffer(strCommunity.GetLength()), 
								strIndex, 
								nRetryDelay, 
								nTimeout, 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	F5 Big-IP Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL F5(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strCommunity = _T("public"), 
			strIndex = _T("0");
	int		nPort = 161, 
			nTimeout = 5, 
			nRetryDelay = 1;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			strIndex = strTemp.Right(strTemp.GetLength() - strlen(__INDEX__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_34%>"));//<%IDS_Monitor_34%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_35%>"));//<%IDS_Monitor_35%>"�˿ڴ���"
		return FALSE;
	}

	for(int i = 0;i < strIndex.GetLength();i ++)
	{
		TCHAR ch = strIndex.GetAt(i);
		if(ch != '.' && !isdigit(ch))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_36%>"));//<%IDS_Monitor_36%>"�������������������"
			return FALSE;
		}
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_37%>"));//<%IDS_Monitor_37%>"��ʱ����"
		return FALSE;
	}

	if(nRetryDelay < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_38%>"));//<%IDS_Monitor_38%>"����ʱ��������"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_39%>"));//<%IDS_Monitor_39%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = F5_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
							nPort, 
							strCommunity.GetBuffer(strCommunity.GetLength()), 
							strIndex, 
							nRetryDelay, 
							nTimeout, 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	Foundry CPU Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL FOUNDRYCPUS(CStringList &paramList, CStringList &lstTexts, CStringList &lstValues)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strCommunity = _T("public");
	int		nPort = 161, 
			nTimeout = 5, 
			nRetryDelay = 1;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		return FALSE;
	}

	if(nPort <= 0)
	{
		return FALSE;
	}

	if(WSA_Init())
	{
		return FALSE;
	}

	char szReturn[8192] = {0};
	BOOL bResult = FOUNDRYCPUS_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strCommunity.GetBuffer(strCommunity.GetLength()), 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									szReturn);

	WSA_Free();

	if(bResult)
	{
		char	*ca = szReturn, 
				*cb = NULL;
		CString strTemp = _T("");

		while(cb = strstr(ca, "$$$"))
		{
			if(cb > ca)
			{
				strTemp.Format("%d", atoi(ca));
				lstValues.AddTail(strTemp);
				strTemp.Format("Module Slot Number %d's CPU", atoi(ca));
				lstTexts.AddTail(strTemp);
			}

			ca = cb + 3;
		}
	}

	return bResult;
}


extern "C" __declspec(dllexport) 
BOOL FOUNDRYCPU(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strCommunity = _T("public"), 
			strIndex = _T("0");
	int		nPort = 161, 
			nTimeout = 5, 
			nRetryDelay = 1;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			strIndex = strTemp.Right(strTemp.GetLength() - strlen(__INDEX__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_34%>"));//<%IDS_Monitor_34%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_35%>"));//<%IDS_Monitor_35%>"�˿ڴ���"
		return FALSE;
	}

	for(int i = 0;i < strIndex.GetLength();i ++)
	{
		TCHAR ch = strIndex.GetAt(i);
		if(ch != '.' && !isdigit(ch))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_36%>"));//<%IDS_Monitor_36%>"�������������������"
			return FALSE;
		}
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_37%>"));//<%IDS_Monitor_37%>"��ʱ����"
		return FALSE;
	}

	if(nRetryDelay < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_38%>"));//<%IDS_Monitor_38%>"����ʱ��������"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_39%>"));//<%IDS_Monitor_39%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = FOUNDRYCPU_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strCommunity.GetBuffer(strCommunity.GetLength()), 
									strIndex, 
									nRetryDelay, 
									nTimeout, 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									szReturn);

	WSA_Free();

	return bResult;
}

extern "C" __declspec(dllexport) 
BOOL FOUNDRYMEM(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strCommunity = _T("public"), 
			strIndex = _T("0");
	int		nPort = 161, 
			nTimeout = 5, 
			nRetryDelay = 1;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			strIndex = strTemp.Right(strTemp.GetLength() - strlen(__INDEX__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_34%>"));//<%IDS_Monitor_34%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_35%>"));//<%IDS_Monitor_35%>"�˿ڴ���"
		return FALSE;
	}

	for(int i = 0;i < strIndex.GetLength();i ++)
	{
		TCHAR ch = strIndex.GetAt(i);
		if(ch != '.' && !isdigit(ch))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_36%>"));//<%IDS_Monitor_36%>"�������������������"
			return FALSE;
		}
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_37%>"));//<%IDS_Monitor_37%>"��ʱ����"
		return FALSE;
	}

	if(nRetryDelay < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_38%>"));//<%IDS_Monitor_38%>"����ʱ��������"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_39%>"));//<%IDS_Monitor_39%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = FOUNDRYCPU_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strCommunity.GetBuffer(strCommunity.GetLength()), 
									strIndex, 
									nRetryDelay, 
									nTimeout, 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									szReturn);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	FTP Monitor Export Function
****************************************************************************/
extern int passive_mode;
extern "C" __declspec(dllexport) 
BOOL FTP(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strFTPServ = _T(""), 
				strDownFile = _T(""), 
				strFTPUser = _T(""), 
				strFTPPass = _T("");
	int			nFTPPort = 21;

	CString		strProxyServ = _T(""), 
				strProxyUser = _T(""), 
				strProxyPass = _T(""), 
				strFTPMatch = _T("");
	int			nProxyType = 0, 
				nProxyPort = 21, 
				nTimeout = 60;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());
	
	// Check Content Change

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strFTPServ = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nFTPPort = atoi(strTemp);
			}
			else
			{
				strFTPServ = strTemp;
			}
		}
		else if(strTemp.Find(__DOWNLOADFILE__, 0) == 0)
		{
			strDownFile = strTemp.Right(strTemp.GetLength() - strlen(__DOWNLOADFILE__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strFTPUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strFTPPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROXYTYPE__, 0) == 0)
		{
			nProxyType = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROXYTYPE__)));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServ = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				index = strTemp.Find(':', 0);
				if(index >= 0)
				{
					nProxyPort = atoi(strTemp.Left(index));
					strTemp.Delete(0, index + 1);
					nProxyType = atoi(strTemp);
				}
				else
				{
					nProxyPort = atoi(strTemp);
				}
			}
			else
			{
				strProxyServ = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUser = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPass = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__PASSIVEMODE__, 0) == 0)
		{
			passive_mode = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PASSIVEMODE__)));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strFTPMatch = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strFTPServ.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_40%>"));//<%IDS_Monitor_40%>"ȱ��FTP��������ַ"
		return FALSE;
	}

	if(strDownFile.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_41%>"));//<%IDS_Monitor_41%>"ȱ�������ļ���"
		return FALSE;
	}

	if(strFTPUser.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_42%>"));//<%IDS_Monitor_42%>"ȱ���û���"
		return FALSE;
	}

	if(nFTPPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_43%>"));//<%IDS_Monitor_43%>"�˿ڴ���"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_44%>"));//<%IDS_Monitor_44%>"��ʱ����"
		return FALSE;
	}

	if(!strProxyServ.IsEmpty() && nProxyPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_45%>"));//<%IDS_Monitor_45%>"����������˿ڴ���"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_46%>"));//<%IDS_Monitor_46%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = FTP_MONITOR(strFTPServ.GetBuffer(strFTPServ.GetLength()), 
							nFTPPort, 
							strDownFile.GetBuffer(strDownFile.GetLength()), 
							strFTPUser.GetBuffer(strFTPUser.GetLength()), 
							strFTPPass.GetBuffer(strFTPPass.GetLength()), 
							nTimeout, 
							nProxyType, 
							strProxyServ.GetBuffer(strProxyServ.GetLength()), 
							nProxyPort, 
							strProxyUser.GetBuffer(strProxyPass.GetLength()), 
							strProxyPass.GetBuffer(strProxyPass.GetLength()), 
							strFTPMatch.GetBuffer(strFTPMatch.GetLength()), 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	TFTP Server Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL TFTP(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strTFTPServer = _T(""), 
				strDownloadFile = _T("");
	int			nTFTPPort = 69, 
				nTimeout = 60;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());
	
	// Check Content Change

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strTFTPServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nTFTPPort = atoi(strTemp);
			}
			else
			{
				strTFTPServer = strTemp;
			}
		}
		else if(strTemp.Find(__DOWNLOADFILE__, 0) == 0)
		{
			strDownloadFile = strTemp.Right(strTemp.GetLength() - strlen(__DOWNLOADFILE__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strTFTPServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_40%>"));//<%IDS_Monitor_40%>"ȱ��FTP��������ַ"
		return FALSE;
	}

	if(strDownloadFile.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_41%>"));//<%IDS_Monitor_41%>"ȱ�������ļ���"
		return FALSE;
	}

	if(nTFTPPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_43%>"));//<%IDS_Monitor_43%>"�˿ڴ���"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_44%>"));//<%IDS_Monitor_44%>"��ʱ����"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_46%>"));//<%IDS_Monitor_46%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = TFTP_MONITOR(strTFTPServer.GetBuffer(strTFTPServer.GetLength()), 
								nTFTPPort, 
								strDownloadFile.GetBuffer(strDownloadFile.GetLength()), 
								nTimeout, 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	iPlanet Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL IPLANET(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strURL = _T(""), 
				strUser = _T(""), 
				strPass = _T("");
	int			nTimeout = 60;

	CString		strProxyServer = _T(""), 
				strProxyUser = _T(""), 
				strProxyPass = _T("");
	int			nProxyPort = 8080;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	// NT Chalenge

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERMANURL__, 0) == 0)
		{
			strURL = strTemp.Right(strTemp.GetLength() - strlen(__SERVERMANURL__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUser = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPass = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strURL.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_47%>"));//<%IDS_Monitor_47%>"ȱ��URL"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_48%>"));//<%IDS_Monitor_48%>"��ʱ����"
		return FALSE;
	}

	if(!strProxyServer.IsEmpty() && nProxyPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_49%>"));//<%IDS_Monitor_49%>"����������˿ڴ���"
		return FALSE;
	}

	return IPLANET_MONITOR(strURL.GetBuffer(strURL.GetLength()), 
						nTimeout, 
						strUser.GetBuffer(strUser.GetLength()), 
						strPass.GetBuffer(strPass.GetLength()), 
						strProxyServer.GetBuffer(strProxyServer.GetLength()), 
						nProxyPort, 
						strProxyUser.GetBuffer(strProxyUser.GetLength()), 
						strProxyPass.GetBuffer(strProxyPass.GetLength()), 
						strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
						szReturn);
}

/****************************************************************************
	LDAP Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL LDAP(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strProvider = _T(""), 
				strPrin = _T(""), 
				strCred = _T("");
	int			nPort = 389;

	CString		strMatch = _T(""), 
				strQuery = _T(""), 
				strFilter = _T("");

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROVIDER__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROVIDER__));
			strTemp.MakeLower();
			if(strTemp.Find("ldap://", 0) != 0)
			{
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_50%>"));//<%IDS_Monitor_50%>"LDAP URL ��ʽ����"
				return FALSE;
			}
			strTemp.Delete(0, 7);
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProvider = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strProvider = strTemp;
			}
		}
		else if(strTemp.Find(__PRINCIPAL__, 0) == 0)
		{
			strPrin = strTemp.Right(strTemp.GetLength() - strlen(__PRINCIPAL__));
		}
		else if(strTemp.Find(__CREDENTIALS__, 0) == 0)
		{
			strCred = strTemp.Right(strTemp.GetLength() - strlen(__CREDENTIALS__));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strMatch = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__QUERY__, 0) == 0)
		{
			strQuery = strTemp.Right(strTemp.GetLength() - strlen(__QUERY__));
		}
		else if(strTemp.Find(__FILTER__, 0) == 0)
		{
			strFilter = strTemp.Right(strTemp.GetLength() - strlen(__FILTER__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strProvider.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_51%>"));//<%IDS_Monitor_51%>"ȱ�� Provider"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_52%>"));//<%IDS_Monitor_52%>"�˿ڴ���"
		return FALSE;
	}

	if(strPrin.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_53%>"));//<%IDS_Monitor_53%>"ȱ�� Principal"
		return FALSE;
	}

	return LDAP_MONITOR(strProvider.GetBuffer(strProvider.GetLength()), 
						nPort, 
						strPrin.GetBuffer(strPrin.GetLength()), 
						strCred.GetBuffer(strCred.GetLength()), 
						strMatch.GetBuffer(strMatch.GetLength()), 
						strQuery.GetBuffer(strQuery.GetLength()), 
						strFilter.GetBuffer(strFilter.GetLength()), 
						strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
						szReturn);
}


/****************************************************************************
	Log File Monitor Export Function
****************************************************************************/

BOOL TELLOGFILE(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T("");
	int			nPort = 23, 
				nOnce= 0,
				nAppendMethod = 1;

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;
	

	CString		strLogFile = _T(""), 
				strcMatch = _T(""), 
				strRuleFile = _T("");
	CString		strMid = _T("");

	int			nGID = 0;
				//nMID = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\MonitorManager\\temp", FuncGetInstallRootPath());

//puts(strCustomerPath);

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__LOGFILE__, 0) == 0)
		{
			strLogFile = strTemp.Right(strTemp.GetLength() - strlen(__LOGFILE__));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strcMatch = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__RULEFILE__, 0) == 0)
		{
			strRuleFile = strTemp.Right(strTemp.GetLength() - strlen(__RULEFILE__));
		}
		else if(strTemp.Find(__GROUPID__, 0) == 0)
		{
			nGID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__GROUPID__)));
		}
		else if(strTemp.Find(__MONITORID__, 0) == 0)
		{
			//nMID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__)));
			strMid=strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__));
		}
		else if(strTemp.Find(__APPENDMETHOD__, 0) == 0)
		{
			nAppendMethod = atoi(strTemp.Right(strTemp.GetLength() - strlen(__APPENDMETHOD__)));
		}
		else if(strTemp.Find(__ALWAYSMONITOR__, 0) == 0)
		{
			nOnce = atoi(strTemp.Right(strTemp.GetLength() - strlen(__ALWAYSMONITOR__)));
		}

		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_73%>"));//<%IDS_Monitor_73%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_74%>"));//<%IDS_Monitor_74%>"����˿ڴ���"
		return FALSE;
	}

	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_75%>"));//<%IDS_Monitor_75%>"ȱ���û���"
		return FALSE;
	}

	if(strLogFile.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_214%>"));//<%IDS_Monitor_76%>"�������������"
		return FALSE;
	}

	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_77%>"));//<%IDS_Monitor_77%>"�����¼��ʾ����������ʾ������ʾ��"
		return FALSE;
	}
	
	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_78%>"));//<%IDS_Monitor_78%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	BOOL ret = LOGFILE_TEL_VALUE(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								strProxyServer.GetBuffer(strProxyServer.GetLength()), 
								nProxyPort, 
								strProxyUid.GetBuffer(strProxyUid.GetLength()), 
								strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
								strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
								strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
								strPrompt.GetBuffer(strPrompt.GetLength()), 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								strLogFile.GetBuffer(strLogFile.GetLength()), 
								strcMatch.GetBuffer(strcMatch.GetLength()), 
								strRuleFile.GetBuffer(strRuleFile.GetLength()), 
								nAppendMethod, 
								nGID, 
								strMid.GetBuffer(strMid.GetLength()), 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								nOnce,
								szReturn);

	WSA_Free();

	return ret;
}


BOOL SSHLOGFILE(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""), 
				strMid= _T(""), 
				strIniFile = _T(""),
				strPrivateKeyFile = _T("");
	int			nPort = 22, 
				nOnce= 0,
				nAppendMethod = 1;

	CString		strLogFile = _T(""), 
				strcMatch = _T(""), 
				strRuleFile = _T("");

	int			nGID = 0, 
				nMID = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		puts(strTemp);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));

		}
		else if(strTemp.Find(__LOGFILE__, 0) == 0)
		{
			strLogFile = strTemp.Right(strTemp.GetLength() - strlen(__LOGFILE__));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strcMatch = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__RULEFILE__, 0) == 0)
		{
			strRuleFile = strTemp.Right(strTemp.GetLength() - strlen(__RULEFILE__));
		}
		else if(strTemp.Find(__GROUPID__, 0) == 0)
		{
			nGID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__GROUPID__)));
		}
		else if(strTemp.Find(__MONITORID__, 0) == 0)
		{
		//	nMID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__)));
			strMid=strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__));
		}
		else if(strTemp.Find(__APPENDMETHOD__, 0) == 0)
		{
			nAppendMethod = atoi(strTemp.Right(strTemp.GetLength() - strlen(__APPENDMETHOD__)));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else if(strTemp.Find(__ALWAYSMONITOR__, 0) == 0)
		{
			nOnce = atoi(strTemp.Right(strTemp.GetLength() - strlen(__ALWAYSMONITOR__)));
		}
		else if(strTemp.Find(__PRIKEYPATH__, 0) == 0)
		{
			strPrivateKeyFile = strTemp.Right(strTemp.GetLength() - strlen(__PRIKEYPATH__));
		}

	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_79%>"));//<%IDS_Monitor_79%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_80%>"));//<%IDS_Monitor_80%>"����˿ڴ���"
		return FALSE;
	}

	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_81%>"));//<%IDS_Monitor_81%>"ȱ���û���"
		return FALSE;
	}

	if(strLogFile.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_214%>"));//<%IDS_Monitor_82%>"�������������"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_83%>"));//<%IDS_Monitor_83%>"ͨ�ų�ʼ������"
		return FALSE;
	}


	BOOL ret = LOGFILE_SSH_VALUE(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								strLogFile.GetBuffer(strLogFile.GetLength()), 
								strcMatch.GetBuffer(strcMatch.GetLength()), 
								strRuleFile.GetBuffer(strRuleFile.GetLength()), 
								nAppendMethod, 
								nGID, 
								strMid.GetBuffer(strMid.GetLength()),  
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								nOnce,
								strPrivateKeyFile.GetBuffer(strPrivateKeyFile.GetLength()),
								szReturn);

	WSA_Free();

	return ret;
}


extern "C" __declspec(dllexport) 
BOOL LOGFILE(const char *strParas, char * szReturn, int & nSize)
//(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	int nProtoType = 0;
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROTOCOLTYPE__, 0) == 0)
		{
			nProtoType =atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOLTYPE__)));
			break;
		}
	}

	if(nProtoType == 0) nProtoType = 2;
	BOOL bRet;
	switch(nProtoType)
	{
	case 1:
		 bRet =SSHLOGFILE(paramList, szReturn);
		 break;
	case 2:
		bRet=TELLOGFILE(paramList, szReturn);
		break;
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_84%>"));//<%IDS_Monitor_84%>"Э�����ʹ���"
		return FALSE;
	}

	CString strOutRet;
	strOutRet =szReturn;
	MakeCharByString(szReturn,nSize,strOutRet);
	return bRet;
}

/****************************************************************************
	MAPI Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL MAPI(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strExServer = _T(""), 
			strsMailBox = _T(""), 
			strsPassword = _T(""), 
			strrMailBox = _T(""), 
			strrPassword = _T("");
	int		nMessBody = 0, 
			nMessAttachment = 0;

	CString	strRcptTo = _T(""), 
			strAttach = _T(""), 
			strMatch = _T("");

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__RCPTADDRESS__, 0) == 0)
		{
			strRcptTo = strTemp.Right(strTemp.GetLength() - strlen(__RCPTADDRESS__));
		}
		else if(strTemp.Find(__ATTACHMENT__, 0) == 0)
		{
			strAttach = strTemp.Right(strTemp.GetLength() - strlen(__ATTACHMENT__));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strMatch = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strRcptTo.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_62%>"));//<%IDS_Monitor_62%>"�ռ��˵�ַ����"
		return FALSE;
	}

	if(!strAttach.IsEmpty()) 
	{
		if((_access(strAttach, 0x04)) == -1)
		{
			if(errno == EACCES)
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_63%>"));//<%IDS_Monitor_63%>"�����ļ��޶�Ȩ��"
			else if(errno == ENOENT)
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_64%>"));//<%IDS_Monitor_64%>"�����ļ�������"
			else
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_65%>"));//<%IDS_Monitor_65%>"�򿪸����ļ�ʧ��"

			return FALSE;
		}
	}

	DWORD dwBeginTime = ::GetTickCount();
	DWORD dwEndTime = 0;
	CMAPIProto	*pMAPI = new CMAPIProto();
	LHANDLE lhSession;

	//printf("InitMAPI Start\n");

	if(!pMAPI->InitMAPI())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_66%>"));//<%IDS_Monitor_66%>"MAPI���ʼ������"
		delete pMAPI;
		return FALSE;
	}

	//printf("InitMAPI End\n");
	//Sleep(1000);
	//printf("MAPILogon Start\n");

	//MAPILogon(ULONG ulUIParam, LPSTR lpszProfileName, LPSTR lpszPassword, FLAGS flFlags, ULONG ulReserved, LPLHANDLE lplhSession, char *szReturn)

	if(!pMAPI->MAPILogon(0, NULL, NULL, 0, 0, &lhSession, szReturn))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_67%>"));//<%IDS_Monitor_67%>"MAPI��¼����"
		delete pMAPI;
		return FALSE;
	}

	//printf("MAPILogon End\n");
	//Sleep(1000);
	//printf("MAPIResolveName start\n");

	// send mail
	MapiMessage		message;
	memset(&message, 0, sizeof(message));
	message.ulReserved = 0;
	message.lpszMessageType = NULL;

	char subject[512];
	strcpy(subject, "1");
	message.lpszSubject = subject;

	char text[5000];
	CTime curTime = CTime::GetCurrentTime();
	sprintf(text, "DragonFlow MAPI Monitor test message sent at %d-%d-%d %d:%d:%d", 
						curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), 
						curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond());
	message.lpszNoteText = text;

	message.flFlags = MAPI_SENT;

	MapiRecipDesc recip;
	memset(&recip, 0, sizeof(recip));
	recip.ulReserved = 0;
	recip.ulRecipClass = 0;
	recip.lpszAddress = strRcptTo.GetBuffer(strRcptTo.GetLength());
	recip.lpszName = strRcptTo.GetBuffer(strRcptTo.GetLength());
	message.lpOriginator = &recip;

	message.nRecipCount = 1;

	char recipient[512];
	strcpy(recipient, strRcptTo);
	if(!pMAPI->MAPIResolveName(lhSession, 0, recipient, 0, 0, &message.lpRecips, szReturn))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_68%>"));//<%IDS_Monitor_68%>"�ռ��˴���"
		delete pMAPI;
		return FALSE;
	}

	message.lpRecips->ulRecipClass = MAPI_TO;

	if(strAttach.IsEmpty())
		message.nFileCount = 0;
	else
	{
		CString sPath, sFileName;
		MapiFileDesc FileInfo;
		char path[512];
		char filename[512];

		int nPos = strAttach.ReverseFind('\\');
		if (nPos == -1)
		{
			sPath = strAttach;
		}
		else
		{
			sPath = strAttach;
			sFileName = strAttach.Mid(nPos +1);
		}
		strcpy(path, sPath);
		strcpy(filename, sFileName);

		message.nFileCount = 1;
		FileInfo.ulReserved = 0;

		FileInfo.flFlags = 0;

		FileInfo.nPosition = -1;
		FileInfo.lpszPathName = path;
		FileInfo.lpszFileName = filename;
		FileInfo.lpFileType = NULL;
		message.lpFiles = &FileInfo;
	}

	//Sleep(1000);
	//printf("MAPISendMail start\n");
	
	if(!pMAPI->MAPISendMail(lhSession, 0, &message, MAPI_NEW_SESSION, 0, szReturn))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_68%>"));//<%IDS_Monitor_68%>"�����ʼ�����"
		delete pMAPI;
		return FALSE;
	}
	

	//printf("MAPISendMail end\n");
	Sleep(5000);
	//printf("MAPIFreeBuffer start\n");

	if(!pMAPI->MAPIFreeBuffer(message.lpRecips, szReturn))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_69%>"));//<%IDS_Monitor_69%>"�ڴ��ͷŴ���"
		delete pMAPI;
		return FALSE;
	}

	//printf("MAPIFreeBuffer end\n");
	Sleep(2000);
	//printf("MAPIFindNext start\n");

	// receive mail
	char pMessageID [513];
	if(!pMAPI->MAPIFindNext(lhSession, NULL, NULL, 0, 
		MAPI_LONG_MSGID | MAPI_GUARANTEE_FIFO, 0, pMessageID, szReturn))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_70%>"));//<%IDS_Monitor_70%>"�ʼ���λ����"
		delete pMAPI;
		return FALSE;
	}

	//printf("MAPIFindNext end\n");
	//Sleep(1);
	//printf("MAPIReadMail start\n");

	lpMapiMessage	pMessage = NULL;
	long nFlags = MAPI_SUPPRESS_ATTACH;
	if(!pMAPI->MAPIReadMail(lhSession, NULL, pMessageID, nFlags, 0, &pMessage, szReturn))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_71%>"));//<%IDS_Monitor_71%>"���ʼ�����"
		delete pMAPI;
		return FALSE;
	}

	//printf("MAPIReadMail end\n");
	//Sleep(2000); 

	if(!strMatch.IsEmpty())
	{
		if(strstr(pMessage->lpszNoteText, strMatch.GetBuffer(strMatch.GetLength())))
		{
			dwEndTime = ::GetTickCount();
		}
		else
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_72%>"));//<%IDS_Monitor_72%>"ƥ�����"
			delete pMAPI;
			return FALSE;
		}
	}
	else
	{
		dwEndTime = ::GetTickCount();
	}

	sprintf(szReturn, "roundTripTime=%ld$", dwEndTime - dwBeginTime);

	delete pMAPI;

	return TRUE;
}

/****************************************************************************
	NetScreen Firewall Resource Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL NETSCREENRES(const char *strParas, char * szReturn, int & nSize)
//(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strCommunity = _T("public"), 
			strIndex = _T("0");
	int		nPort = 161, 
			nTimeout = 5, 
			nRetryDelay = 1;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			strIndex = strTemp.Right(strTemp.GetLength() - strlen(__INDEX__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_28%>"));//<%IDS_Monitor_28%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_29%>"));//<%IDS_Monitor_29%>"����˿ڴ���"
		return FALSE;
	}

	for(int i = 0;i < strIndex.GetLength();i ++)
	{
		TCHAR ch = strIndex.GetAt(i);
		if(ch != '.' && !isdigit(ch))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_30%>"));//<%IDS_Monitor_30%>"�������������������"
			return FALSE;
		}
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_31%>"));//<%IDS_Monitor_31%>"���볬ʱ����"
		return FALSE;
	}

	if(nRetryDelay < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_32%>"));//<%IDS_Monitor_32%>"����ʱ��������"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_Monitor_33%>"));//<%IDS_Monitor_33%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = NETSCREENRES_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
										nPort, 
										strCommunity.GetBuffer(strCommunity.GetLength()), 
										strIndex, 
										nRetryDelay, 
										nTimeout, 
										strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
										szReturn);


	CString strOutRet;
	strOutRet =szReturn;
	MakeCharByString(szReturn,nSize,strOutRet);

	WSA_Free();

	return TRUE;
}

/****************************************************************************
	NetScreen Firewall Service Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL NETSCREENSERVICE(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strCommunity = _T("public"), 
			strIndex = _T("0");
	int		nPort = 161, 
			nTimeout = 5, 
			nRetryDelay = 1;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			strIndex = strTemp.Right(strTemp.GetLength() - strlen(__INDEX__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_28%>"));//<%IDS_Monitor_28%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_29%>"));//<%IDS_Monitor_29%>"����˿ڴ���"
		return FALSE;
	}

	for(int i = 0;i < strIndex.GetLength();i ++)
	{
		TCHAR ch = strIndex.GetAt(i);
		if(ch != '.' && !isdigit(ch))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_30%>"));//<%IDS_Monitor_30%>"�������������������"
			return FALSE;
		}
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_31%>"));//<%IDS_Monitor_31%>"���볬ʱ����"
		return FALSE;
	}

	if(nRetryDelay < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_32%>"));//<%IDS_Monitor_32%>"����ʱ��������"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_Monitor_33%>"));//<%IDS_Monitor_33%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = NETSCREENSERVICE_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
											nPort, 
											strCommunity.GetBuffer(strCommunity.GetLength()), 
											strIndex, 
											nRetryDelay, 
											nTimeout, 
											strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
											szReturn);

	WSA_Free();

	return TRUE;
}

/****************************************************************************
	NetScreen Firewall ServiceGroup Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL NETSCREENSERVICEGROUP(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strCommunity = _T("public"), 
			strIndex = _T("0");
	int		nPort = 161, 
			nTimeout = 5, 
			nRetryDelay = 1;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			strIndex = strTemp.Right(strTemp.GetLength() - strlen(__INDEX__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_28%>"));//<%IDS_Monitor_28%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_29%>"));//<%IDS_Monitor_29%>"����˿ڴ���"
		return FALSE;
	}

	for(int i = 0;i < strIndex.GetLength();i ++)
	{
		TCHAR ch = strIndex.GetAt(i);
		if(ch != '.' && !isdigit(ch))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_30%>"));//<%IDS_Monitor_30%>"�������������������"
			return FALSE;
		}
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_31%>"));//<%IDS_Monitor_31%>"���볬ʱ����"
		return FALSE;
	}

	if(nRetryDelay < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_32%>"));//<%IDS_Monitor_32%>"����ʱ��������"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_Monitor_33%>"));//<%IDS_Monitor_33%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = NETSCREENSERVICEGROUP_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
												nPort, 
												strCommunity.GetBuffer(strCommunity.GetLength()), 
												strIndex, 
												nRetryDelay, 
												nTimeout, 
												strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
												szReturn);

	WSA_Free();

	return TRUE;
}

/****************************************************************************
	NetScreen Firewall ServiceGroupMember Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL NETSCREENSERVICEGROUPMEMBER(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strCommunity = _T("public"), 
			strIndex = _T("0");
	int		nPort = 161, 
			nTimeout = 5, 
			nRetryDelay = 1;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			strIndex = strTemp.Right(strTemp.GetLength() - strlen(__INDEX__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_28%>"));//<%IDS_Monitor_28%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_29%>"));//<%IDS_Monitor_29%>"����˿ڴ���"
		return FALSE;
	}

	for(int i = 0;i < strIndex.GetLength();i ++)
	{
		TCHAR ch = strIndex.GetAt(i);
		if(ch != '.' && !isdigit(ch))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_30%>"));//<%IDS_Monitor_30%>"�������������������"
			return FALSE;
		}
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_31%>"));//<%IDS_Monitor_31%>"���볬ʱ����"
		return FALSE;
	}

	if(nRetryDelay < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_32%>"));//<%IDS_Monitor_32%>"����ʱ��������"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_Monitor_33%>"));//<%IDS_Monitor_33%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = NETSCREENSERVICEGROUPMEMBER_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
													nPort, 
													strCommunity.GetBuffer(strCommunity.GetLength()), 
													strIndex, 
													nRetryDelay, 
													nTimeout, 
													strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
													szReturn);

	WSA_Free();

	return TRUE;
}

/****************************************************************************
	NetScreen Firewall VPN Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL NETSCREENVPN(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strCommunity = _T("public"), 
			strIndex = _T("0");
	int		nPort = 161, 
			nTimeout = 5, 
			nRetryDelay = 1;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			strIndex = strTemp.Right(strTemp.GetLength() - strlen(__INDEX__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_28%>"));//<%IDS_Monitor_28%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_29%>"));//<%IDS_Monitor_29%>"����˿ڴ���"
		return FALSE;
	}

	for(int i = 0;i < strIndex.GetLength();i ++)
	{
		TCHAR ch = strIndex.GetAt(i);
		if(ch != '.' && !isdigit(ch))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_30%>"));//<%IDS_Monitor_30%>"�������������������"
			return FALSE;
		}
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_31%>"));//<%IDS_Monitor_31%>"���볬ʱ����"
		return FALSE;
	}

	if(nRetryDelay < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_32%>"));//<%IDS_Monitor_32%>"����ʱ��������"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_Monitor_33%>"));//<%IDS_Monitor_33%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = NETSCREENVPN_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
										nPort, 
										strCommunity.GetBuffer(strCommunity.GetLength()), 
										strIndex, 
										nRetryDelay, 
										nTimeout, 
										strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
										szReturn);

	WSA_Free();

	return TRUE;
}

/****************************************************************************
	NetScreen Firewall Attack Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL NETSCREENATTACK(const char *strParas, char * szReturn, int & nSize)
//(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strCommunity = _T("public"), 
			strIndex = _T("0");
	int		nPort = 161, 
			nTimeout = 5, 
			nRetryDelay = 1;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			strIndex = strTemp.Right(strTemp.GetLength() - strlen(__INDEX__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_28%>"));//<%IDS_Monitor_28%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_29%>"));//<%IDS_Monitor_29%>"����˿ڴ���"
		return FALSE;
	}

	for(int i = 0;i < strIndex.GetLength();i ++)
	{
		TCHAR ch = strIndex.GetAt(i);
		if(ch != '.' && !isdigit(ch))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_30%>"));//<%IDS_Monitor_30%>"�������������������"
			return FALSE;
		}
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_31%>"));//<%IDS_Monitor_31%>"���볬ʱ����"
		return FALSE;
	}

	if(nRetryDelay < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_32%>"));//<%IDS_Monitor_32%>"����ʱ��������"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_Monitor_33%>"));//<%IDS_Monitor_33%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = NETSCREENATTACK_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
										nPort, 
										strCommunity.GetBuffer(strCommunity.GetLength()), 
										strIndex, 
										nRetryDelay, 
										nTimeout, 
										strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
										szReturn);

	CString strOutRet;
	strOutRet =szReturn;
	MakeCharByString(szReturn,nSize,strOutRet);

	WSA_Free();

	return TRUE;
}

/****************************************************************************
	NetScreen Firewall Packet Drop Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL NETSCREENPACKDROP(const char *strParas, char * szReturn, int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strCommunity = _T("public"), 
			strIndex = _T("0");
	int		nPort = 161, 
			nTimeout = 5, 
			nRetryDelay = 1;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			strIndex = strTemp.Right(strTemp.GetLength() - strlen(__INDEX__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_28%>"));//<%IDS_Monitor_28%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_29%>"));//<%IDS_Monitor_29%>"����˿ڴ���"
		return FALSE;
	}

	for(int i = 0;i < strIndex.GetLength();i ++)
	{
		TCHAR ch = strIndex.GetAt(i);
		if(ch != '.' && !isdigit(ch))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_30%>"));//<%IDS_Monitor_30%>"�������������������"
			return FALSE;
		}
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_31%>"));//<%IDS_Monitor_31%>"���볬ʱ����"
		return FALSE;
	}

	if(nRetryDelay < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_32%>"));//<%IDS_Monitor_32%>"����ʱ��������"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_Monitor_33%>"));//<%IDS_Monitor_33%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = NETSCREENPACKDROP_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
											nPort, 
											strCommunity.GetBuffer(strCommunity.GetLength()), 
											strIndex, 
											nRetryDelay, 
											nTimeout, 
											strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
											szReturn);

	WSA_Free();
	CString strOutRet;
	strOutRet =szReturn;
	MakeCharByString(szReturn,nSize,strOutRet);

	return TRUE;
}

/****************************************************************************
	MQ Status Monitor Export Function
****************************************************************************/
bool TELMQSTATUS(CStringList &paramList, char *szReturn)
{
	OutputDebugString("-----------TELMQSTATUS() call start----------------\n");
	
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:");
	int			nPort = 23; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strQueueManager = _T(""), 
				strChannelName = _T(""),
				strQueueName = _T("");

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__MQQUEUEMANAGER__, 0) == 0)
		{
			strQueueManager = strTemp.Right(strTemp.GetLength() - strlen(__MQQUEUEMANAGER__));
		}
		else if(strTemp.Find(__MQQUEUENAME__, 0) == 0)
		{
			strQueueName = strTemp.Right(strTemp.GetLength() - strlen(__MQQUEUENAME__));
		}
		else if(strTemp.Find(__MQCHANNELNAME__, 0) == 0)
		{
			strChannelName = strTemp.Right(strTemp.GetLength() - strlen(__MQCHANNELNAME__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_IP_ADDRESS_NULL"));//<%IDS_Monitor_73%>"ȱ�ٷ�����"
		return false;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_PORT_NULL"));//<%IDS_Monitor_74%>"����˿ڴ���"
		return false;
	}

	if(strUID.IsEmpty())
	{
		OutputDebugString("-----------telmqstatus() �û���Ϊ��---------------\n");
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_USER_NULL"));//<%IDS_Monitor_75%>"ȱ���û���"
		return false;
	}

/*	if(strQueueName.IsEmpty())
	{
		OutputDebugString("--------������Ϊ��--------\n");
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_76%>"));//<%IDS_Monitor_76%>"�������������"
		return false;
	}
*/
	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SREVER",
            "UNIX_SERVER_PROMPT_NULL"));//<%IDS_Monitor_77%>"�����¼��ʾ����������ʾ������ʾ��"
		return false;
	}
	
	if(WSA_Init())
	{
		
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_78%>"ͨ�ų�ʼ������"
		return false;
	}
	
	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	bool ret = MQ_TEL_STATUS_VALUE(strServer.GetBuffer(strServer.GetLength()), 
							nPort, 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
							strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
							strPrompt.GetBuffer(strPrompt.GetLength()), 
							strQueueManager.GetBuffer(strQueueManager.GetLength()), 
							strQueueName.GetBuffer(strQueueName.GetLength()), 
							strChannelName.GetBuffer(strChannelName.GetLength()),
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);

	WSA_Free();

	return ret;
}

/****************************************************************************
	MQ Monitor Export Function
****************************************************************************/
bool TELMQ(CStringList &paramList, char *szReturn)
{
	OutputDebugString("--------TELMQ() call start----------\n");
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:");
	int			nPort = 23; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strQueueManager = _T(""), 
				strQueueName = _T("");

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__MQQUEUEMANAGER__, 0) == 0)
		{
			strQueueManager = strTemp.Right(strTemp.GetLength() - strlen(__MQQUEUEMANAGER__));
		}
		else if(strTemp.Find(__MQQUEUENAME__, 0) == 0)
		{
			strQueueName = strTemp.Right(strTemp.GetLength() - strlen(__MQQUEUENAME__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_IP_ADDRESS_NULL"));//<%IDS_Monitor_73%>"ȱ�ٷ�����"
		return false;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_PORT_NULL"));//<%IDS_Monitor_74%>"����˿ڴ���"
		return false;
	}

	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_USER_NULL"));//<%IDS_Monitor_75%>"ȱ���û���"
		return false;
	}

	if(strQueueName.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_76%>"));//<%IDS_Monitor_76%>"�������������"
		return false;
	}

	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SREVER",
            "UNIX_SERVER_PROMPT_NULL"));//<%IDS_Monitor_77%>"�����¼��ʾ����������ʾ������ʾ��"
		return false;
	}
	
	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_78%>"ͨ�ų�ʼ������"
		return false;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	bool ret = MQ_TEL_VALUE(strServer.GetBuffer(strServer.GetLength()), 
							nPort, 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
							strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
							strPrompt.GetBuffer(strPrompt.GetLength()), 
							strQueueManager.GetBuffer(strQueueManager.GetLength()), 
							strQueueName.GetBuffer(strQueueName.GetLength()), 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);

	WSA_Free();

	return ret;
}

bool SSHMQ(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T("");
	int			nPort = 22;

	CString		strQueueManager = _T(""), 
				strQueueName = _T("");

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__MQQUEUEMANAGER__, 0) == 0)
		{
			strQueueManager = strTemp.Right(strTemp.GetLength() - strlen(__MQQUEUEMANAGER__));
		}
		else if(strTemp.Find(__MQQUEUENAME__, 0) == 0)
		{
			strQueueName = strTemp.Right(strTemp.GetLength() - strlen(__MQQUEUENAME__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

    if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_IP_ADDRESS_NULL"));//<%IDS_Monitor_73%>"ȱ�ٷ�����"
		return false;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_PORT_NULL"));//<%IDS_Monitor_74%>"����˿ڴ���"
		return false;
	}

	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_USER_NULL"));//<%IDS_Monitor_75%>"ȱ���û���"
		return false;
	}

	if(strQueueName.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_76%>"));//<%IDS_Monitor_76%>"�������������"
		return false;
	}
	
	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_78%>"ͨ�ų�ʼ������"
		return false;
	}

	bool ret = MQ_SSH_VALUE(strServer.GetBuffer(strServer.GetLength()), 
							nPort, 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strQueueManager.GetBuffer(strQueueManager.GetLength()), 
							strQueueName.GetBuffer(strQueueName.GetLength()), 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);

	WSA_Free();

	return ret;
}

bool HTTPMQ(CStringList &paramList, char *szReturn)
{
	CString		strRemoteUrl = _T(""),
				strUID = _T(""),
				strPWD = _T("");

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strQueueManager = _T(""), 
				strQueueName = _T("");

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strRemoteUrl = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__MQQUEUEMANAGER__, 0) == 0)
		{
			strQueueManager = strTemp.Right(strTemp.GetLength() - strlen(__MQQUEUEMANAGER__));
		}
		else if(strTemp.Find(__MQQUEUENAME__, 0) == 0)
		{
			strQueueName = strTemp.Right(strTemp.GetLength() - strlen(__MQQUEUENAME__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strRemoteUrl.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_73%>"));//<%IDS_Monitor_73%>"ȱ�ٷ�����"
		return false;
	}

	if(strQueueName.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_76%>"));//<%IDS_Monitor_76%>"�������������"
		return false;
	}

	return HTTP_MQ_VALUE(strRemoteUrl.GetBuffer(strRemoteUrl.GetLength()), 
						strUID.GetBuffer(strUID.GetLength()), 
						strPWD.GetBuffer(strPWD.GetLength()), 
						strProxyServer.GetBuffer(strProxyServer.GetLength()), 
						nProxyPort, 
						strProxyUid.GetBuffer(strProxyUid.GetLength()), 
						strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
						strQueueManager.GetBuffer(strQueueManager.GetLength()), 
						strQueueName.GetBuffer(strQueueName.GetLength()), 
						strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
						szReturn);
}

bool RLOGINMQ(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strPwdPrompt = _T("assword:");
	int			nPort = 513; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strQueueManager = _T(""), 
				strQueueName = _T("");

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__MQQUEUEMANAGER__, 0) == 0)
		{
			strQueueManager = strTemp.Right(strTemp.GetLength() - strlen(__MQQUEUEMANAGER__));
		}
		else if(strTemp.Find(__MQQUEUENAME__, 0) == 0)
		{
			strQueueName = strTemp.Right(strTemp.GetLength() - strlen(__MQQUEUENAME__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_73%>"));//<%IDS_Monitor_73%>"ȱ�ٷ�����"
		return false;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_74%>"));//<%IDS_Monitor_74%>"����˿ڴ���"
		return false;
	}

	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_75%>"));//<%IDS_Monitor_75%>"ȱ���û���"
		return false;
	}

	if(strQueueName.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_76%>"));//<%IDS_Monitor_76%>"�������������"
		return false;
	}

	if(strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_77%>"));//<%IDS_Monitor_77%>"�����¼��ʾ����������ʾ������ʾ��"
		return false;
	}
	
	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_78%>"));//<%IDS_Monitor_78%>"ͨ�ų�ʼ������"
		return false;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	bool ret = RLOGIN_MQ_VALUE(strServer.GetBuffer(strServer.GetLength()), 
							nPort, 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
							strPrompt.GetBuffer(strPrompt.GetLength()), 
							strQueueManager.GetBuffer(strQueueManager.GetLength()), 
							strQueueName.GetBuffer(strQueueName.GetLength()), 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);

	WSA_Free();

	return ret;
}

extern "C" __declspec(dllexport) 
bool MQSTATUS(const char *strParas, char * szReturn, int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CStringList paramList;

	MakeStringListByChar(paramList,strParas);
	int nProtoType = 0;
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROTOCOLTYPE__, 0) == 0)
		{
			nProtoType =atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOLTYPE__)));
			break;
		}
	}

	if(nProtoType == 0) nProtoType = 2;

    bool bResult = false;
	switch(nProtoType)
	{
	case 1:
		bResult = SSHMQ(paramList, szReturn);
        break;
	case 2:
		bResult = TELMQSTATUS(paramList, szReturn);
        break;
	case 3:
		bResult = HTTPMQ(paramList, szReturn);
        break;
	case 4:
		bResult = RLOGINMQ(paramList, szReturn);
        break;
	
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC", "BASIC_PROTOCOL_TYPE_ERROR"));//<%IDS_Monitor_84%>"Э�����ʹ���"
        break;
	}

    CString strBuffer = szReturn;
	MakeCharByString(szReturn, nSize, strBuffer);

	return bResult;
}

extern "C" __declspec(dllexport) 
bool MQ(const char *strParas, char * szReturn, int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CStringList paramList;

	MakeStringListByChar(paramList,strParas);
	int nProtoType = 0;
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROTOCOLTYPE__, 0) == 0)
		{
			nProtoType =atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOLTYPE__)));
			break;
		}
	}

	if(nProtoType == 0) nProtoType = 2;

    bool bResult = false;
	switch(nProtoType)
	{
	case 1:
		bResult = SSHMQ(paramList, szReturn);
        break;
	case 2:
		bResult = TELMQ(paramList, szReturn);
        break;
	case 3:
		bResult = HTTPMQ(paramList, szReturn);
        break;
	case 4:
		bResult = RLOGINMQ(paramList, szReturn);
        break;
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC", "BASIC_PROTOCOL_TYPE_ERROR"));//<%IDS_Monitor_84%>"Э�����ʹ���"
        break;
	}

    CString strBuffer = szReturn;
	MakeCharByString(szReturn, nSize, strBuffer);

	return bResult;
}

/****************************************************************************
	News Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL NEWS(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CStringList paramList;

	MakeStringListByChar(paramList,strParas);
//(CStringList &paramList, char *szReturn)

	

	CString		strServer = _T("");
	CStringList	lstGroups;
	int			nPort = 119; 

	CString		strUser = _T(""), 
				strPass = _T(""), 
				strConnAddr = _T("");
	int			nTimeout = 60;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__GROUP__, 0) == 0)
		{
			int index = 0;
			CString strGroups = strTemp.Right(strTemp.GetLength() - strlen(__GROUP__));
			while((index = strGroups.Find(',', 0)) >= 0)
			{
				if(index == 0)
					strGroups.Delete(0, 1);
				else
				{
					lstGroups.AddTail(strGroups.Left(index));
					strGroups.Delete(0, index + 1);
				}
			}

			if(!strGroups.IsEmpty())
				lstGroups.AddTail(strGroups);
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__CONNECTADDRESS__, 0) == 0)
		{
			strConnAddr = strTemp.Right(strTemp.GetLength() - strlen(__CONNECTADDRESS__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_85%>"));//<%IDS_Monitor_85%>"ȱ����������IP��ַ"
		return FALSE;
	}

	if(lstGroups.GetCount() == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_86%>"));//<%IDS_Monitor_86%>"ȱ������������"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_87%>"));//<%IDS_Monitor_87%>"����˿ڴ���"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_88%>"));//<%IDS_Monitor_88%>"���볬ʱ����"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_89%>"));//<%IDS_Monitor_89%>"ͨ�ų�ʼ��ʧ��"
		return FALSE;
	}

	BOOL bResult = NEWS_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								lstGroups, 
								strUser.GetBuffer(strUser.GetLength()), 
								strPass.GetBuffer(strPass.GetLength()), 
								nTimeout, 
								strConnAddr.GetBuffer(strConnAddr.GetLength()), 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);
	CString strInput;
	strInput =szReturn;
	MakeCharByString(szReturn,nSize,strInput);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	Ping Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL PING(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strHost = _T("");

	int			nTimeout = 5000, 
				nBytes = 64;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strHost = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__SENDBYTES__, 0) == 0)
		{
			nBytes = atoi(strTemp.Right(strTemp.GetLength() - strlen(__SENDBYTES__)));
			if(nBytes > 256) nBytes = 256;
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strHost.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_93%>"));//<%IDS_Monitor_93%>"ȱ����������IP��ַ"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_94%>"));//<%IDS_Monitor_94%>"���볬ʱ����"
		return FALSE;
	}

	if(nBytes <= 0 || nBytes > 256)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_95%>"));//<%IDS_Monitor_95%>"�����ֽڴ�С����"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_96%>"));//<%IDS_Monitor_96%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = PING_MONITOR(strHost.GetBuffer(strHost.GetLength()), 
								nTimeout, 
								nBytes, 
								FALSE, 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);

	WSA_Free();

	return bResult;
}

extern "C" __declspec(dllexport) 
BOOL VIPPING(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strHost = _T("");

	int			nTimeout = 5000, 
				nBytes = 64;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strHost = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__SENDBYTES__, 0) == 0)
		{
			nBytes = atoi(strTemp.Right(strTemp.GetLength() - strlen(__SENDBYTES__)));
			if(nBytes > 256) nBytes = 256;
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strHost.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_97%>"));//<%IDS_Monitor_97%>"ȱ����������IP��ַ"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_98%>"));//<%IDS_Monitor_98%>"���볬ʱ����"
		return FALSE;
	}

	if(nBytes <= 0 || nBytes > 256)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_99%>"));//<%IDS_Monitor_99%>"�����ֽڴ�С����"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_100%>"));//<%IDS_Monitor_100%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = PING_MONITOR(strHost.GetBuffer(strHost.GetLength()), 
								nTimeout, 
								nBytes, 
								TRUE, 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	Port Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL PORT(const char *strParas,  char *szReturn,int & nSize)
//(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strHost = _T("");
	int			nPort = 0;

	CString		strSend = _T(""), 
				strMatch = _T("");
	int			nTimeout = 60;

	CString		strProxyServ = _T(""), 
				strProxyUser = _T(""), 
				strProxyPass = _T("");
	int			nProxyPort = 1080;

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
			strHost = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__CONNPORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__CONNPORT__)));
		}
		else if(strTemp.Find(__SENDSTR__, 0) == 0)
		{
			strSend = strTemp.Right(strTemp.GetLength() - strlen(__SENDSTR__));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strMatch = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServ = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServ = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUser = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPass = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strHost.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_101%>"));//<%IDS_Monitor_101%>"ȱ����������IP��ַ"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_102%>"));//<%IDS_Monitor_102%>"����˿ڴ���"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_103%>"));//<%IDS_Monitor_103%>"���볬ʱ����"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_104%>"));//<%IDS_Monitor_104%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = PORT_MONITOR(strHost.GetBuffer(strHost.GetLength()), 
								nPort, 
								strSend.GetBuffer(strSend.GetLength()), 
								strMatch.GetBuffer(strMatch.GetLength()), 
								nTimeout, 
								strProxyServ.GetBuffer(strProxyServ.GetLength()), 
								nProxyPort, 
								strProxyUser.GetBuffer(strProxyUser.GetLength()), 
								strProxyPass.GetBuffer(strProxyPass.GetLength()), 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	Radius Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL RADIUS(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strServer = _T(""), 
				strSecret = _T(""), 
				strUser =_T(""), 
				strPass = _T("");
	
	int			nTimeout = 30, 
				nPort = 1645;
	CString		strMatch = _T("");

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__SECRET__, 0) == 0)
		{
			strSecret = strTemp.Right(strTemp.GetLength() - strlen(__SECRET__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strMatch = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_105%>"));//<%IDS_Monitor_105%>"ȱ�ٷ�������ַ"
		return FALSE;
	}

	if(strSecret.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_106%>"));//<%IDS_Monitor_106%>"ȱ�ٹ�������"
		return FALSE;
	}

	if(strUser.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_107%>"));//<%IDS_Monitor_107%>"ȱ���û���"
		return FALSE;
	}

	if(strPass.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_108%>"));//<%IDS_Monitor_108%>"ȱ������"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_109%>"));//<%IDS_Monitor_109%>"����˿ڴ���"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_110%>"));//<%IDS_Monitor_110%>"���볬ʱ����"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_111%>"));//<%IDS_Monitor_111%>"ͨ�ų�ʼ��ʧ��"
		return FALSE;
	}

	BOOL bResult = RADIUS_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strSecret.GetBuffer(strSecret.GetLength()), 
								strUser.GetBuffer(strUser.GetLength()), 
								strPass.GetBuffer(strPass.GetLength()), 
								nTimeout, 
								strMatch.GetBuffer(strMatch.GetLength()), 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	SilverStream Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL SILVERSTREAM(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strURL = _T(""), 
				strUser = _T(""), 
				strPass = _T("");
	int			nTimeout = 60;

	CString		strProxyServer = _T(""), 
				strProxyUser = _T(""), 
				strProxyPass = _T("");
	int			nProxyPort = 8080;

	// NT Chalenge

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERMANURL__, 0) == 0)
		{
			strURL = strTemp.Right(strTemp.GetLength() - strlen(__SERVERMANURL__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUser = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPass = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strURL.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_115%>"));//<%IDS_Monitor_115%>"ȱ��URL"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_116%>"));//<%IDS_Monitor_116%>"���볬ʱ����"
		return FALSE;
	}

	if(!strProxyServer.IsEmpty() && nProxyPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_117%>"));//<%IDS_Monitor_117%>"�������������˿ڴ���"
		return FALSE;
	}

	return SILVERSTREAM_MONITOR(strURL.GetBuffer(strURL.GetLength()), 
								nTimeout, 
								strUser.GetBuffer(strUser.GetLength()), 
								strPass.GetBuffer(strPass.GetLength()), 
								strProxyServer.GetBuffer(strProxyServer.GetLength()), 
								nProxyPort, 
								strProxyUser.GetBuffer(strProxyUser.GetLength()), 
								strProxyPass.GetBuffer(strProxyPass.GetLength()), 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);
}

/****************************************************************************
	SNMP Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL SNMP(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strHost = _T(""), 
				strObject = _T(""), 
				strCommunity = _T("public"), 
				strIndex = _T("0");
	int			nPort = 161;

	int			nTimeout = 5, 
				nRetryDelay = 1;
	CString		strMatch = _T("");

	// More Parameters

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__)); 
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strHost = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strHost = strTemp;
			}
		}
		else if(strTemp.Find(__REQUIREID__, 0) == 0)
		{
			strObject = strTemp.Right(strTemp.GetLength() - strlen(__REQUIREID__));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			strIndex = strTemp.Right(strTemp.GetLength() - strlen(__INDEX__));
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}

		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strMatch = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strHost.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_118%>"));//<%IDS_Monitor_118%>"ȱ��������ַ"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_119%>"));//<%IDS_Monitor_119%>"����˿ڴ���"
		return FALSE;
	}

	if(strObject.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_120%>"));//<%IDS_Monitor_120%>"ȱ���������"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_121%>"));//<%IDS_Monitor_121%>"���볬ʱ����"
		return FALSE;
	}

	if(nRetryDelay <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_122%>"));//<%IDS_Monitor_122%>"��������ʱ��������"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_123%>"));//<%IDS_Monitor_123%>"ͨ�ų�ʼ��ʧ��"
		return FALSE;
	}

	CString strReqID = strObject + "." + strIndex;
	BOOL bResult = SNMP_MONITOR(strHost.GetBuffer(strHost.GetLength()), 
								nPort, 
								strReqID.GetBuffer(strReqID.GetLength()), 
								strCommunity.GetBuffer(strCommunity.GetLength()), 
								nTimeout, 
								nRetryDelay, 
								strMatch.GetBuffer(strMatch.GetLength()), 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	Database Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL SYBASE(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strDBConn = _T("");

	CString		strDBUser = _T(""), 
				strDBPass = _T("");

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__DSNNAME__, 0) == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__DSNNAME__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strDBPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strDBConn.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_14%>"));//<%IDS_Monitor_14%>"ȱ�����ݿ�����"
		return FALSE;
	}

	return SYBASE_MONITOR(strDBConn.GetBuffer(strDBConn.GetLength()), 
						strDBUser.GetBuffer(strDBUser.GetLength()), 
						strDBPass.GetBuffer(strDBPass.GetLength()), 
						strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
						szReturn);
}

/****************************************************************************
	Another Interface Traffic Monitor Export Function
****************************************************************************/
typedef BOOL(*lpNetTraffic)(CStringList &paramList, char *szReturn);
extern "C" __declspec(dllexport) 
BOOL NETTRAFFIC(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HINSTANCE hInstance;
	lpNetTraffic pNetTraffic;
	BOOL bResult = TRUE;

	hInstance = ::LoadLibrary("NetTraffic.dll");
	if (!hInstance) 
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_212%>"));
		return FALSE;
	}

	pNetTraffic = (lpNetTraffic)::GetProcAddress(hInstance, "NetTRAFFIC");
	if(!pNetTraffic)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_212%>"));
		bResult = FALSE;
		goto err;
	}

	bResult = (*pNetTraffic)(paramList, szReturn);

err:
	if(hInstance) ::FreeLibrary(hInstance);
	return bResult;
}

/****************************************************************************
	Interface Traffic Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL TRAFFIC(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strHost = _T(""), 
				strCommunity = _T("public");
	int			nPort = 161, 
				nIfIndex = 0;

	int			nTimeout = 5, 
				nRetryDelay = 1;
	int			nGID = 0, 
				nMID = 0;
	CString strMonitorID = _T("");
	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__)); 
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strHost = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strHost = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__INTERFACEINDEX__, 0) == 0)
		{
			nIfIndex = atoi(strTemp.Right(strTemp.GetLength() - strlen(__INTERFACEINDEX__)));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		//else if(strTemp.Find(__GROUPID__, 0) == 0)
		//{
		//	nGID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__GROUPID__)));
		//}
		else if(strTemp.Find(__MONITORID__, 0) == 0)
		{
			strMonitorID = strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
	}

	if(strHost.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_124%>"));//<%IDS_Monitor_124%>"ȱ��������ַ"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_125%>"));//<%IDS_Monitor_125%>"����˿ڴ���"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_126%>"));//<%IDS_Monitor_126%>"���볬ʱ����"
		return FALSE;
	}

	if(nRetryDelay <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_127%>"));//<%IDS_Monitor_127%>"��������ʱ��������"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_128%>"));//<%IDS_Monitor_128%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = TRAFFIC_MONITOR(strHost.GetBuffer(strHost.GetLength()), 
								nPort, 
								strCommunity.GetBuffer(strCommunity.GetLength()), 
								nIfIndex, 
								nTimeout, 
								nRetryDelay, 
								strMonitorID, 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);

	WSA_Free();

	return bResult;
}


extern "C" __declspec(dllexport) 
BOOL HUAWEICPU(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strCommunity = _T("public"), 
			strIndex = _T("");
	int		nPort = 161, 
			nTimeout = 5, 
			nRetryDelay = 1;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			int index = 0;
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			if((index = strTemp.Find(':', 0)) >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			strIndex = strTemp.Right(strTemp.GetLength() - strlen(__INDEX__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_197%>"));//<%IDS_Monitor_197%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_198%>"));//<%IDS_Monitor_198%>"����˿ڴ���"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_199%>"));//<%IDS_Monitor_199%>"ͨ�ų�ʼ��ʧ��"
		return FALSE;
	}

	BOOL bResult = HUAWEICPU_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strIndex, 
									strCommunity.GetBuffer(strCommunity.GetLength()), 
									nTimeout, 
									nRetryDelay, 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									szReturn);

	WSA_Free();

	return bResult;
}

//------------------------------------------------------------------------------
extern "C" __declspec(dllexport) 
BOOL HUAWEISWITCHCPU(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strCommunity = _T("public"), 
			strIndex = _T(""),	
			strType ="8016";
	int		nPort = 161, 
			nTimeout = 5, 
			nRetryDelay = 1;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			int index = 0;
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			if((index = strTemp.Find(':', 0)) >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			strIndex = strTemp.Right(strTemp.GetLength() - strlen(__INDEX__));
		}
		else if(strTemp.Find(__NETWORK_TYPE__, 0) == 0)
		{
			strType = strTemp.Right(strTemp.GetLength() - strlen(__NETWORK_TYPE__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_197%>"));//<%IDS_Monitor_197%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_198%>"));//<%IDS_Monitor_198%>"����˿ڴ���"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_199%>"));//<%IDS_Monitor_199%>"ͨ�ų�ʼ��ʧ��"
		return FALSE;
	}

	BOOL bResult = HUAWEISWITCH_CPU(strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strIndex, 
									strCommunity.GetBuffer(strCommunity.GetLength()), 
									nTimeout, 
									nRetryDelay, 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									strType,
									szReturn);

	WSA_Free();

	return bResult;
}


/****************************************************************************
	SNMP Trap Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL TRAP(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strMatchContent = _T("");
	int		nAlertType = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strMatchContent = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__ALERTTYPE__, 0) == 0)
		{
			nAlertType = atoi(strTemp.Right(strTemp.GetLength() - strlen(__ALERTTYPE__)));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	return TRAP_MONITOR(strMatchContent.GetBuffer(strMatchContent.GetLength()), 
						nAlertType, 
						strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
						szReturn);
}

/****************************************************************************
	Export Functions Of CPU Monitor
****************************************************************************/
BOOL TELCPU(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T("");
	int			nPort = 23; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
	}
	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_IP_ADDRESS_NULL"));//<%IDS_Monitor_134%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_PORT_NULL"));//<%IDS_Monitor_135%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_USER_NULL"));//<%IDS_Monitor_136%>"ȱ���û���"
		return FALSE;
	}

	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SREVER",
            "UNIX_SERVER_PROMPT_NULL"));//<%IDS_Monitor_132%>"�����¼��ʾ����������ʾ������ʾ��"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_137%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	
	strPrompt.TrimLeft();
	strPrompt.TrimRight();
    BOOL ret = TRUE;
    try
    {
        ret = MonitorValue(strServer.GetBuffer(strServer.GetLength()), 
							nPort, 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
							strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
							strPrompt.GetBuffer(strPrompt.GetLength()), 
							strIniFile.GetBuffer(strIniFile.GetLength()), 
							CPU_TYPE_MONITOR, 
							NULL, 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);
    }
    catch(...)
    {
        WriteLogFile("Monitor Cpu faile(Telnet type)");
        char chMsg[256] = {0};
        sprintf(chMsg, "Telnet Monitor Cpu Failed:%s", strServer);
        WriteLogFile(chMsg);
    }
    
	WSA_Free();

	return ret;
}

BOOL SSHCPU(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T(""),
				strPrivateKeyFile = _T("");

	int			nPort = 22;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else if(strTemp.Find("_PriKeyPath=", 0) == 0)
		{
			strPrivateKeyFile = strTemp.Right(strTemp.GetLength() - strlen("_PriKeyPath="));
		}

	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_IP_ADDRESS_NULL"));//<%IDS_Monitor_134%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_PORT_NULL"));//<%IDS_Monitor_135%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_USER_NULL"));//<%IDS_Monitor_136%>"ȱ���û���"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_137%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL ret = SSHMonitorValue(	strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								CPU_TYPE_MONITOR, 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								NULL, 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()),
								strPrivateKeyFile.GetBuffer(strPrivateKeyFile.GetLength()),
								szReturn);

	WSA_Free();

	return ret;
}

BOOL HTTPCPU(CStringList &paramList, char *szReturn)
{
	CString		strRemoteUrl = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T("")
				;

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strRemoteUrl = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}		
		else
		{
		}
	}

	if(strRemoteUrl.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_129%>"));//<%IDS_Monitor_129%>"ȱ�ٷ�����"
		return FALSE;
	}

	return HttpMonitorValue(strRemoteUrl.GetBuffer(strRemoteUrl.GetLength()), 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strIniFile.GetBuffer(strIniFile.GetLength()), 
							CPU_TYPE_MONITOR, 
							NULL, 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 						
							szReturn);
}

BOOL RLOGINCPU(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T("");
	int			nPort = 513; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_129%>"));//<%IDS_Monitor_129%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_130%>"));//<%IDS_Monitor_130%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_131%>"));//<%IDS_Monitor_131%>"ȱ���û���"
		return FALSE;
	}
	if(strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_132%>"));//<%IDS_Monitor_132%>"�����¼��ʾ����������ʾ������ʾ��"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_133%>"));//<%IDS_Monitor_133%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	BOOL ret = RloginMonitorValue(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								strProxyServer.GetBuffer(strProxyServer.GetLength()), 
								nProxyPort, 
								strProxyUid.GetBuffer(strProxyUid.GetLength()), 
								strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
								strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
								strPrompt.GetBuffer(strPrompt.GetLength()), 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								CPU_TYPE_MONITOR, 
								NULL, 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);

	WSA_Free();

	return ret;
}

//extern "C" __declspec(dllexport) 
//BOOL CPU(CStringList &paramList, char *szReturn)
extern "C" __declspec(dllexport) 
bool CPU( const char *strSrc,  char *szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList paramList;

	MakeStringListByChar(paramList,strSrc);
	int nProtoType = 0;
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROTOCOLTYPE__, 0) == 0)
		{
			nProtoType =atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOLTYPE__)));
			break;
		}
	}

	if(nProtoType == 0) nProtoType = 2;

	bool bRet = false;
	switch(nProtoType)
	{
	case 1:
		bRet= SSHCPU(paramList, szReturn);
		break;
	case 2:
		bRet= TELCPU(paramList, szReturn);
		break;
	case 3:
		bRet = HTTPCPU(paramList, szReturn);
        break;
	case 4:
		bRet = RLOGINCPU(paramList, szReturn);
        break;
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_PROTOCOL_TYPE_ERROR"));//<%IDS_Monitor_138%>"Э�����ʹ���"
		return FALSE;
	}

	CString strTmp=szReturn;
	MakeCharByString(szReturn,nSize,strTmp);


	return bRet;
}


/****************************************************************************
	Export Functions Of HP_CPU Monitor
****************************************************************************/
BOOL HP_TELCPU(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T("");
	int			nPort = 23; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
	}
	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_IP_ADDRESS_NULL"));//<%IDS_Monitor_134%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_PORT_NULL"));//<%IDS_Monitor_135%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_USER_NULL"));//<%IDS_Monitor_136%>"ȱ���û���"
		return FALSE;
	}

	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SREVER",
            "UNIX_SERVER_PROMPT_NULL"));//<%IDS_Monitor_132%>"�����¼��ʾ����������ʾ������ʾ��"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_137%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
    BOOL ret = TRUE;
    try
    {
        ret = MonitorValue(strServer.GetBuffer(strServer.GetLength()), 
							nPort, 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
							strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
							strPrompt.GetBuffer(strPrompt.GetLength()), 
							strIniFile.GetBuffer(strIniFile.GetLength()), 
							HP_CPU_TYPE_MONITOR, 
							NULL, 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);
    }
    catch(...)
    {
        WriteLogFile("Monitor Cpu faile(Telnet type)");
        char chMsg[256] = {0};
        sprintf(chMsg, "Telnet Monitor Cpu Failed:%s", strServer);
        WriteLogFile(chMsg);
    }
    
	WSA_Free();

	return ret;
}

BOOL HP_SSHCPU(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T(""),
				strPrivateKeyFile = _T("");

	int			nPort = 22;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else if(strTemp.Find("_PriKeyPath=", 0) == 0)
		{
			strPrivateKeyFile = strTemp.Right(strTemp.GetLength() - strlen("_PriKeyPath="));
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_IP_ADDRESS_NULL"));//<%IDS_Monitor_134%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_PORT_NULL"));//<%IDS_Monitor_135%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_USER_NULL"));//<%IDS_Monitor_136%>"ȱ���û���"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_137%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL ret = SSHMonitorValue(	strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								HP_CPU_TYPE_MONITOR, 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								NULL, 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								strPrivateKeyFile.GetBuffer(strPrivateKeyFile.GetLength()),
								szReturn);

	WSA_Free();

	return ret;
}

BOOL HP_HTTPCPU(CStringList &paramList, char *szReturn)
{
	CString		strRemoteUrl = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T("");

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strRemoteUrl = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strRemoteUrl.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_129%>"));//<%IDS_Monitor_129%>"ȱ�ٷ�����"
		return FALSE;
	}

	return HttpMonitorValue(strRemoteUrl.GetBuffer(strRemoteUrl.GetLength()), 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strIniFile.GetBuffer(strIniFile.GetLength()), 
							HP_CPU_TYPE_MONITOR, 
							NULL, 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);
}

BOOL HP_RLOGINCPU(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T("");
	int			nPort = 513; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_129%>"));//<%IDS_Monitor_129%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_130%>"));//<%IDS_Monitor_130%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_131%>"));//<%IDS_Monitor_131%>"ȱ���û���"
		return FALSE;
	}
	if(strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_132%>"));//<%IDS_Monitor_132%>"�����¼��ʾ����������ʾ������ʾ��"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_133%>"));//<%IDS_Monitor_133%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	BOOL ret = RloginMonitorValue(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								strProxyServer.GetBuffer(strProxyServer.GetLength()), 
								nProxyPort, 
								strProxyUid.GetBuffer(strProxyUid.GetLength()), 
								strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
								strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
								strPrompt.GetBuffer(strPrompt.GetLength()), 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								HP_CPU_TYPE_MONITOR, 
								NULL, 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);

	WSA_Free();

	return ret;
}

extern "C" __declspec(dllexport) 
BOOL HP_CPU(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int nProtoType = 0;
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROTOCOLTYPE__, 0) == 0)
		{
			nProtoType =atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOLTYPE__)));
			break;
		}
	}

	if(nProtoType == 0) nProtoType = 2;

	switch(nProtoType)
	{
	case 1:
		return HP_SSHCPU(paramList, szReturn);
	case 2:
		return HP_TELCPU(paramList, szReturn);
	case 3:
		return HP_HTTPCPU(paramList, szReturn);
	case 4:
		return HP_RLOGINCPU(paramList, szReturn);
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_PROTOCOL_TYPE_ERROR"));//<%IDS_Monitor_138%>"Э�����ʹ���"  
		return FALSE;
	}

	return TRUE;
}


BOOL TELUNIXPERF(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T("");
	int			nPort = 23; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_129%>"));//<%IDS_Monitor_129%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_130%>"));//<%IDS_Monitor_130%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_131%>"));//<%IDS_Monitor_131%>"ȱ���û���"
		return FALSE;
	}
	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_132%>"));//<%IDS_Monitor_132%>"�����¼��ʾ����������ʾ������ʾ��"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_133%>"));//<%IDS_Monitor_133%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	BOOL ret = UnixPerfMonitorValue(strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strUID.GetBuffer(strUID.GetLength()), 
									strPWD.GetBuffer(strPWD.GetLength()), 
									strProxyServer.GetBuffer(strProxyServer.GetLength()), 
									nProxyPort, 
									strProxyUid.GetBuffer(strProxyUid.GetLength()), 
									strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
									strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
									strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
									strPrompt.GetBuffer(strPrompt.GetLength()), 
									strIniFile.GetBuffer(strIniFile.GetLength()), 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									szReturn);

	WSA_Free();

	return ret;
}

BOOL SSHUNIXPERF(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T(""),
				strPrivateKeyFile = _T("");

	int			nPort = 22;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else if(strTemp.Find("_PriKeyPath", 0) == 0)
		{
			strPrivateKeyFile = strTemp.Right(strTemp.GetLength() - strlen("_PriKeyPath"));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_IP_ADDRESS_NULL"));//<%IDS_Monitor_134%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_135%>"));//<%IDS_Monitor_135%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_136%>"));//<%IDS_Monitor_136%>"ȱ���û���"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_137%>"));//<%IDS_Monitor_137%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL ret = SSHUnixPerfMonitorValue(	strServer.GetBuffer(strServer.GetLength()), 
										nPort, 
										strUID.GetBuffer(strUID.GetLength()), 
										strPWD.GetBuffer(strPWD.GetLength()), 
										strIniFile.GetBuffer(strIniFile.GetLength()), 
										strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
										strPrivateKeyFile.GetBuffer(strPrivateKeyFile.GetLength()),
										szReturn);

	WSA_Free();

	return ret;
}

BOOL HTTPUNIXPERF(CStringList &paramList, char *szReturn)
{
	CString		strRemoteUrl = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T("");

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strRemoteUrl = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strRemoteUrl.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_129%>"));//<%IDS_Monitor_129%>"ȱ�ٷ�����"
		return FALSE;
	}

	return HttpUnixPerfMonitorValue(strRemoteUrl.GetBuffer(strRemoteUrl.GetLength()), 
									strUID.GetBuffer(strUID.GetLength()), 
									strPWD.GetBuffer(strPWD.GetLength()), 
									strProxyServer.GetBuffer(strProxyServer.GetLength()), 
									nProxyPort, 
									strProxyUid.GetBuffer(strProxyUid.GetLength()), 
									strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
									strIniFile.GetBuffer(strIniFile.GetLength()), 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									szReturn);
}

BOOL RLOGINUNIXPERF(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T("");
	int			nPort = 513; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_129%>"));//<%IDS_Monitor_129%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_130%>"));//<%IDS_Monitor_130%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_131%>"));//<%IDS_Monitor_131%>"ȱ���û���"
		return FALSE;
	}
	if(strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_132%>"));//<%IDS_Monitor_132%>"�����¼��ʾ����������ʾ������ʾ��"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_133%>"));//<%IDS_Monitor_133%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	BOOL ret = RloginUnixPerfMonitorValue(strServer.GetBuffer(strServer.GetLength()), 
										nPort, 
										strUID.GetBuffer(strUID.GetLength()), 
										strPWD.GetBuffer(strPWD.GetLength()), 
										strProxyServer.GetBuffer(strProxyServer.GetLength()), 
										nProxyPort, 
										strProxyUid.GetBuffer(strProxyUid.GetLength()), 
										strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
										strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
										strPrompt.GetBuffer(strPrompt.GetLength()), 
										strIniFile.GetBuffer(strIniFile.GetLength()), 
										strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
										szReturn);

	WSA_Free();

	return ret;
}

extern "C" __declspec(dllexport) 
BOOL UNIXPERF(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int nProtoType = 0;
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROTOCOLTYPE__, 0) == 0)
		{
			nProtoType =atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOLTYPE__)));
			break;
		}
	}

	if(nProtoType == 0) nProtoType = 2;

	switch(nProtoType)
	{
	case 1:
		return SSHUNIXPERF(paramList, szReturn);
	case 2:
		return TELUNIXPERF(paramList, szReturn);
	case 3:
		return HTTPUNIXPERF(paramList, szReturn);
	case 4:
		return RLOGINUNIXPERF(paramList, szReturn);
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_PROTOCOL_TYPE_ERROR"));//<%IDS_Monitor_138%>"Э�����ʹ���"
		return FALSE;
	}

	return TRUE;
}

/****************************************************************************
	Export Functions Of Memory Monitor
****************************************************************************/
BOOL TELMEMORY(CStringList &paramList, char *szReturn,int nstyle)
{
	WriteLog("\n\n============== TELMEMORY =================");

	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T("");
	int			nPort = 23; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
	}

	WriteLog("���������");
	WriteLog("strServer=");WriteLog(strServer.GetBuffer());
//	WriteLog("nPort=");WriteLog(atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__))));
	WriteLog("strUID=");WriteLog(strUID.GetBuffer());
	WriteLog("strPWD=");WriteLog(strPWD.GetBuffer());
	WriteLog("strIniFile=");WriteLog(strIniFile.GetBuffer());
	WriteLog("strPrompt=");WriteLog(strPrompt.GetBuffer());

	WriteLog("strLogPrompt=");WriteLog(strLogPrompt.GetBuffer());
	WriteLog("strPwdPrompt=");WriteLog(strPwdPrompt.GetBuffer());
	WriteLog("strProxyServer=");WriteLog(strProxyServer.GetBuffer());
	WriteLog("strProxyUid=");WriteLog(strProxyUid.GetBuffer());
	WriteLog("strProxyPwd=");WriteLog(strProxyPwd.GetBuffer());
	WriteLog("strCustomerPath=");WriteLog(strCustomerPath.GetBuffer());






	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_IP_ADDRESS_NULL"));//<%IDS_Monitor_134%>"ȱ�ٷ�����"
		WriteLog("ȱ�ٷ�����");

		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_PORT_NULL"));//<%IDS_Monitor_135%>"����˿ڴ���"
		WriteLog("����˿ڴ���");
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_USER_NULL"));//<%IDS_Monitor_136%>"ȱ���û���"
		WriteLog("ȱ���û���");

		return FALSE;
	}

	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SREVER",
            "UNIX_SERVER_PROMPT_NULL"));//<%IDS_Monitor_132%>"�����¼��ʾ����������ʾ������ʾ��"
		WriteLog("�����¼��ʾ����������ʾ������ʾ��");

		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_137%>"ͨ�ų�ʼ������"
		WriteLog("ͨ�ų�ʼ������");

		return FALSE;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
    BOOL ret = TRUE;
    try
    {
	    ret = MonitorValue(strServer.GetBuffer(strServer.GetLength()), 
							nPort, 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
							strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
							strPrompt.GetBuffer(strPrompt.GetLength()), 
							strIniFile.GetBuffer(strIniFile.GetLength()), 
							nstyle, 
							NULL, 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);
    }
    catch(...)
    {
        WriteLogFile("Monitor Memory faile(Telnet type)");
        char chMsg[256] = {0};
        sprintf(chMsg, "Telnet Monitor Memory Failed:%s", strServer);
        WriteLogFile(chMsg);
    }

	
     

	WSA_Free();

	return ret;
}

/****************************************************************************
	Ϊ׿����Ŀд�ģ�Ŀ����ʹ�ô�����ڴ������������ڴ�ʹ����
    ����Ϊͨ��top�����޷�����ȡ��hp unix���ڴ����������Ա������û��ֶ����룩
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL TELMEMORY_zw(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T("");
	int			nPort = 23; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

    CString     strTotalMem = _T("");

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
        else if (strTemp.Find(__TOTAL_MEM__, 0) == 0)
        {
            strTotalMem = strTemp.Right(strTemp.GetLength() - strlen(__TOTAL_MEM__));
        }
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_IP_ADDRESS_NULL"));//<%IDS_Monitor_134%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_PORT_NULL"));//<%IDS_Monitor_135%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_USER_NULL"));//<%IDS_Monitor_136%>"ȱ���û���"
		return FALSE;
	}

	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SREVER",
            "UNIX_SERVER_PROMPT_NULL"));//<%IDS_Monitor_132%>"�����¼��ʾ����������ʾ������ʾ��"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_137%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	BOOL ret = MonitorValue_zw(strServer.GetBuffer(strServer.GetLength()), 
							nPort, 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
							strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
							strPrompt.GetBuffer(strPrompt.GetLength()), 
							strIniFile.GetBuffer(strIniFile.GetLength()), 
                            strTotalMem.GetBuffer(strTotalMem.GetLength()),
							MEMORY_TYPE_MONITOR, 
							NULL, 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);

	WSA_Free();

	return ret;
}

BOOL SSHMEMORY(CStringList &paramList, char *szReturn,int nstyle)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T(""),
				strPrivateKeyFile = _T("");
	int			nPort = 22;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else if(strTemp.Find("_PriKeyPath=", 0) == 0)
		{
			strPrivateKeyFile = strTemp.Right(strTemp.GetLength() - strlen("_PriKeyPath="));
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_IP_ADDRESS_NULL"));//<%IDS_Monitor_134%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_PORT_NULL"));//<%IDS_Monitor_135%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_USER_NULL"));//<%IDS_Monitor_136%>"ȱ���û���"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_137%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	//-----------------------------------------------------------------------------------------------------------
	// �������ݣ������aix��solaris�������ڴ���Ϣ��ȡ������ȡ��prtconf���������ڴ棬vmstat��ʣ�������ڴ棩
	// �����ˣ�����
	// ����ʱ�䣺2009.02.25
	//-----------------------------------------------------------------------------------------------------------
	BOOL ret(false);
	if( ( nstyle == PMEMORY_TYPE_MONITOR ) && 
		( ( strcmp( strIniFile.GetBuffer(strIniFile.GetLength()), "AIX" ) == 0 ) || 
		  ( strcmp( strIniFile.GetBuffer(strIniFile.GetLength()), "Sun" ) == 0 ) || 
		  ( strcmp( strIniFile.GetBuffer(strIniFile.GetLength()), "sun_china" ) == 0 ) ) )
	{
		WriteLog( "��ȡaix�������ڴ棡" );
		float fTotalMemory(0.0);
		float fFreeMemory(0.0);
		float fPercentUsed(0.0);

		char buffer[1024] = {0};

		// ���Ȼ�ȡ�������ڴ�
		ret = SSHMonitorValue(	strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strUID.GetBuffer(strUID.GetLength()), 
									strPWD.GetBuffer(strPWD.GetLength()), 
									TOTAL_PMEMORY, 
									strIniFile.GetBuffer(strIniFile.GetLength()), 
									NULL, 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									strPrivateKeyFile.GetBuffer(strPrivateKeyFile.GetLength()),
									buffer );

		char* pszTemp = strchr( buffer, '=' ) + 1;

		fTotalMemory =(float)atof( pszTemp );


		// Ȼ���ȡ�����ڴ�ʣ����
		ret = SSHMonitorValue(	strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strUID.GetBuffer(strUID.GetLength()), 
									strPWD.GetBuffer(strPWD.GetLength()), 
									FREE_PMEMORY, 
									strIniFile.GetBuffer(strIniFile.GetLength()), 
									NULL, 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									strPrivateKeyFile.GetBuffer(strPrivateKeyFile.GetLength()),
									buffer );

		pszTemp = strchr( buffer, '=' ) + 1;

		fFreeMemory =(float)atof( pszTemp );

		if( fTotalMemory > 0 )
		{
            fPercentUsed = ( fTotalMemory - fFreeMemory ) / fTotalMemory;
			fTotalMemory /= ( 1024*1024 );
			fFreeMemory /= ( 1024*1024 );
			fPercentUsed *= 100;
		}

		sprintf( szReturn, "totalMemory=%.2f$freeMemory=%.2f$percentUsed=%.2f$", fTotalMemory, fFreeMemory, fPercentUsed );

	}// ��������
	else
	{
		ret = SSHMonitorValue(	strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strUID.GetBuffer(strUID.GetLength()), 
									strPWD.GetBuffer(strPWD.GetLength()), 
									nstyle, 
									strIniFile.GetBuffer(strIniFile.GetLength()), 
									NULL, 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									strPrivateKeyFile.GetBuffer(strPrivateKeyFile.GetLength()),
									szReturn);
	}

	WSA_Free();

	return ret;
}


BOOL HTTPMEMORY(CStringList &paramList, char *szReturn,int nstyle)
{
	CString		strRemoteUrl = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T("");

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strRemoteUrl = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strRemoteUrl.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_139%>"));//<%IDS_Monitor_139%>"ȱ�ٷ�����"
		return FALSE;
	}

	return HttpMonitorValue(strRemoteUrl.GetBuffer(strRemoteUrl.GetLength()), 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strIniFile.GetBuffer(strIniFile.GetLength()), 
							nstyle, 
							NULL, 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);
}

BOOL RLOGINMEMORY(CStringList &paramList, char *szReturn,int nstyle)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T("");
	int			nPort = 513; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_139%>"));//<%IDS_Monitor_139%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_140%>"));//<%IDS_Monitor_140%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_141%>"));//<%IDS_Monitor_141%>"ȱ���û���"
		return FALSE;
	}
	if(strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_142%>"));//<%IDS_Monitor_142%>"�����¼��ʾ����������ʾ������ʾ��"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_143%>"));//<%IDS_Monitor_143%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	BOOL ret = RloginMonitorValue(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								strProxyServer.GetBuffer(strProxyServer.GetLength()), 
								nProxyPort, 
								strProxyUid.GetBuffer(strProxyUid.GetLength()), 
								strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
								strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
								strPrompt.GetBuffer(strPrompt.GetLength()), 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								nstyle, 
								NULL, 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);

	WSA_Free();

	return ret;
}

//ȡUNIX�����ڴ�򽻻��ռ�
extern "C" __declspec(dllexport) 
BOOL MEMORY(const char *strParas,  char *szReturn,int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	WriteLog("\n\n============== MEMORY =================");

    
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	int nProtoType = 0;
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROTOCOLTYPE__, 0) == 0)
		{
			nProtoType =atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOLTYPE__)));
			break;
		}
	}

	if(nProtoType == 0) nProtoType = 2;

	bool bRet;
	switch(nProtoType)
	{
	case 1:
		bRet= SSHMEMORY(paramList, szReturn,MEMORY_TYPE_MONITOR);
		break;
	case 2:
		bRet= TELMEMORY(paramList, szReturn,MEMORY_TYPE_MONITOR);
		break;
	case 3:
		return HTTPMEMORY(paramList, szReturn,MEMORY_TYPE_MONITOR);
	case 4:
		return RLOGINMEMORY(paramList, szReturn,MEMORY_TYPE_MONITOR);
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_PROTOCOL_TYPE_ERROR"));//<%IDS_Monitor_148%>"Э�����ʹ���"
		return FALSE;
	}

		//MakeDisksReturn(szReturn,nSize);

	CString strTmp=szReturn;

	MakeCharByString(szReturn,nSize,strTmp);

	return bRet;

	return TRUE;
}

//ȡUNIX�����ڴ�
extern "C" __declspec(dllexport) 
BOOL PMEMORY(const char *strParas, char *szReturn, int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	int nProtoType = 0;
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROTOCOLTYPE__, 0) == 0)
		{
			nProtoType =atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOLTYPE__)));
			break;
		}
	}

	if(nProtoType == 0) nProtoType = 2;

	bool bRet = FALSE;
	switch(nProtoType)
	{
	case 1:
		bRet = SSHMEMORY(paramList, szReturn,PMEMORY_TYPE_MONITOR);
		break;
	case 2:
		//OutputDebugString("OK!\n");
		bRet = TELMEMORY(paramList, szReturn,PMEMORY_TYPE_MONITOR);
		break;
	case 3:
		return HTTPMEMORY(paramList, szReturn,PMEMORY_TYPE_MONITOR);
	case 4:
		return RLOGINMEMORY(paramList, szReturn,PMEMORY_TYPE_MONITOR);
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_PROTOCOL_TYPE_ERROR"));//<%IDS_Monitor_148%>"Э�����ʹ���"
		return FALSE;
	}

	CString strTmp=szReturn;

	WriteLog( szReturn );

	MakeCharByString(szReturn,nSize,strTmp);
	
	return bRet;
}
/****************************************************************************
	Export Functions Of Disk Monitor
****************************************************************************/
bool TELDISK(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T(""), 
				strDisk = _T("");
	int			nPort = 23; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__DISK__, 0) == 0)
		{
			strDisk = strTemp.Right(strTemp.GetLength() - strlen(__DISK__));
			OutputDebugString(strDisk);
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
	}

    if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_IP_ADDRESS_NULL"));//<%IDS_Monitor_134%>"ȱ�ٷ�����"
		return false;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_PORT_NULL"));//<%IDS_Monitor_135%>"����˿ڴ���"
		return false;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_USER_NULL"));//<%IDS_Monitor_136%>"ȱ���û���"
		return false;
	}

	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SREVER",
            "UNIX_SERVER_PROMPT_NULL"));//<%IDS_Monitor_132%>"�����¼��ʾ����������ʾ������ʾ��"
		return false;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_137%>"ͨ�ų�ʼ������"
		return false;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
    bool ret = true;
    try
    {
		ret = MonitorValue(strServer.GetBuffer(strServer.GetLength()), 
							nPort, 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
							strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
							strPrompt.GetBuffer(strPrompt.GetLength()), 
							strIniFile.GetBuffer(strIniFile.GetLength()), 
							DISK_TYPE_MONITOR, 
							strDisk.GetBuffer(strDisk.GetLength()), 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);
    }
    catch(...)
    {
        WriteLogFile("Monitor Disk faile(Telnet type)");
        char chMsg[256] = {0};
        sprintf(chMsg, "Telnet monitor disk Failed:%s", strServer);
        WriteLogFile(chMsg);
    }

	WSA_Free();

	return ret;
}

bool SSHDISK(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T(""), 
				strDisk = _T(""),
				strPrivateKeyFile = _T("");
	int			nPort = 22; 

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__DISK__, 0) == 0)
		{
			strDisk = strTemp.Right(strTemp.GetLength() - strlen(__DISK__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else if(strTemp.Find("_PriKeyPath=", 0) == 0)
		{
			strPrivateKeyFile = strTemp.Right(strTemp.GetLength() - strlen("_PriKeyPath="));
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_IP_ADDRESS_NULL"));//<%IDS_Monitor_134%>"ȱ�ٷ�����"
		return false;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_PORT_NULL"));//<%IDS_Monitor_135%>"����˿ڴ���"
		return false;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_USER_NULL"));//<%IDS_Monitor_136%>"ȱ���û���"
		return false;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_137%>"ͨ�ų�ʼ������"
		return false;
	}

	bool ret = SSHMonitorValue(	strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								DISK_TYPE_MONITOR, 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								strDisk.GetBuffer(strDisk.GetLength()), 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								strPrivateKeyFile.GetBuffer(strPrivateKeyFile.GetLength()),
								szReturn);

	WSA_Free();

	return ret;
}

BOOL HTTPDISK(CStringList &paramList, char *szReturn)
{
	CString		strRemoteUrl = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T(""), 
				strDisk = _T("");

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strRemoteUrl = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__DISK__, 0) == 0)
		{
			strDisk = strTemp.Right(strTemp.GetLength() - strlen(__DISK__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strRemoteUrl.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_149%>"));//<%IDS_Monitor_149%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(strDisk.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_152%>"));//<%IDS_Monitor_152%>"ȱ�ٴ�������"
		return FALSE;
	}

	return HttpMonitorValue(strRemoteUrl.GetBuffer(strRemoteUrl.GetLength()), 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strIniFile.GetBuffer(strIniFile.GetLength()), 
							DISK_TYPE_MONITOR, 
							strDisk.GetBuffer(strDisk.GetLength()), 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);
}

BOOL RLOGINDISK(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T(""), 
				strDisk = _T("");
	int			nPort = 513; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__DISK__, 0) == 0)
		{
			strDisk = strTemp.Right(strTemp.GetLength() - strlen(__DISK__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_149%>"));//<%IDS_Monitor_149%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_150%>"));//<%IDS_Monitor_150%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_151%>"));//<%IDS_Monitor_151%>"ȱ���û���"
		return FALSE;
	}
	if(strDisk.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_152%>"));//<%IDS_Monitor_152%>"ȱ�ٴ�������"
		return FALSE;
	}
	if(strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_153%>"));//<%IDS_Monitor_153%>"�����¼��ʾ����������ʾ������ʾ��"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_154%>"));//<%IDS_Monitor_153%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	BOOL ret = RloginMonitorValue(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								strProxyServer.GetBuffer(strProxyServer.GetLength()), 
								nProxyPort, 
								strProxyUid.GetBuffer(strProxyUid.GetLength()), 
								strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
								strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
								strPrompt.GetBuffer(strPrompt.GetLength()), 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								DISK_TYPE_MONITOR, 
								strDisk.GetBuffer(strDisk.GetLength()), 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);

	WSA_Free();

	return ret;
}

extern "C" __declspec(dllexport) 
bool DISK(const char *strParas,  char *szReturn,int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

		CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	int nProtoType = 0;
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROTOCOLTYPE__, 0) == 0)
		{
			nProtoType =atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOLTYPE__)));
			break;
		}
	}

	if(nProtoType == 0) nProtoType = 2;

	bool bRet;

	switch(nProtoType)
	{
	case 1:
		bRet= SSHDISK(paramList, szReturn);
		break;
	case 2:
		bRet= TELDISK(paramList, szReturn);
		break;
	//case 3:
	//	return HTTPDISK(paramList, szReturn);
	//case 4:
	//	return RLOGINDISK(paramList, szReturn);
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_PROTOCOL_TYPE_ERROR"));//<%IDS_Monitor_160%>"Э�����ʹ���"
		return false;
	}
	CString strTmp=szReturn;
	MakeCharByString(szReturn,nSize,strTmp);
	

	return bRet;
}

/****************************************************************************
	Export Functions Of Service Monitor
****************************************************************************/
bool TELSERVICE(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T(""), 
				strService = _T("");
	int			nPort = 23; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__SERVICE__, 0) == 0)
		{
			strService = strTemp.Right(strTemp.GetLength() - strlen(__SERVICE__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_IP_ADDRESS_NULL"));//<%IDS_Monitor_134%>"ȱ�ٷ�����"
		return false;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_PORT_NULL"));//<%IDS_Monitor_135%>"����˿ڴ���"
		return false;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_USER_NULL"));//<%IDS_Monitor_136%>"ȱ���û���"
		return false;
	}
	if(strService.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVICE",
            "UNIX_SERVICE_NAME_NULL"));//<%IDS_Monitor_164%>"ȱ�ٷ�������"
		return false;
	}
	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SREVER",
            "UNIX_SERVER_PROMPT_NULL"));//<%IDS_Monitor_132%>"�����¼��ʾ����������ʾ������ʾ��"
		return false;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_137%>"ͨ�ų�ʼ������"
		return false;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
    bool ret = true;
	printf("%s -%s ", strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()));
    try
    {
	    ret = MonitorValue(strServer.GetBuffer(strServer.GetLength()), 
							nPort, 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
							strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
							strPrompt.GetBuffer(strPrompt.GetLength()), 
							strIniFile.GetBuffer(strIniFile.GetLength()), 
							SERVICE_TYPE_MONITOR, 
							strService.GetBuffer(strService.GetLength()), 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);
    }
    catch(...)
    {
        WriteLogFile("Monitor Service faile(Telnet type)");
        char chMsg[256] = {0};
        sprintf(chMsg, "Telnet monitor process Failed:%s", strServer);
        WriteLogFile(chMsg);
    }

	WSA_Free();

	return ret;
}

bool SSHSERVICE(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T(""), 
				strService = _T(""),
				strPrivateKeyFile = _T("");
	int			nPort = 22; 

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__SERVICE__, 0) == 0)
		{
			strService = strTemp.Right(strTemp.GetLength() - strlen(__SERVICE__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else if(strTemp.Find("_PriKeyPath=", 0) == 0)
		{
			strPrivateKeyFile = strTemp.Right(strTemp.GetLength() - strlen("_PriKeyPath="));
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_IP_ADDRESS_NULL"));//<%IDS_Monitor_134%>"ȱ�ٷ�����"
		return false;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_PORT_NULL"));//<%IDS_Monitor_135%>"����˿ڴ���"
		return false;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_USER_NULL"));//<%IDS_Monitor_136%>"ȱ���û���"
		return false;
	}
	if(strService.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVICE",
            "UNIX_SERVICE_NAME_NULL"));//<%IDS_Monitor_170%>"ȱ�ٷ�������"
		return false;
	}


	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_137%>"ͨ�ų�ʼ������"
		return false;
	}

	bool ret = SSHMonitorValue(	strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								SERVICE_TYPE_MONITOR, 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								strService.GetBuffer(strService.GetLength()), 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()),
								strPrivateKeyFile.GetBuffer(strPrivateKeyFile.GetLength()),
								szReturn);

	WSA_Free();

	return ret;
}

BOOL HTTPSERVICE(CStringList &paramList, char *szReturn)
{
	CString		strRemoteUrl = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T(""), 
				strService = _T("");

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strRemoteUrl = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__SERVICE__, 0) == 0)
		{
			strService = strTemp.Right(strTemp.GetLength() - strlen(__SERVICE__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strRemoteUrl.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_161%>"));//<%IDS_Monitor_161%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(strService.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_164%>"));//<%IDS_Monitor_164%>"ȱ�ٷ�������"
		return FALSE;
	}

	return HttpMonitorValue(strRemoteUrl.GetBuffer(strRemoteUrl.GetLength()), 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strIniFile.GetBuffer(strIniFile.GetLength()), 
							SERVICE_TYPE_MONITOR, 
							strService.GetBuffer(strService.GetLength()), 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);
}

BOOL RLOGINSERVICE(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T(""), 
				strService = _T("");
	int			nPort = 513; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__SERVICE__, 0) == 0)
		{
			strService = strTemp.Right(strTemp.GetLength() - strlen(__SERVICE__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_161%>"));//<%IDS_Monitor_161%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_162%>"));//<%IDS_Monitor_162%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_163%>"));//<%IDS_Monitor_163%>"ȱ���û���"
		return FALSE;
	}
	if(strService.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_164%>"));//<%IDS_Monitor_164%>"ȱ�ٷ�������"
		return FALSE;
	}
	if(strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_165%>"));//<%IDS_Monitor_165%>"�����¼��ʾ����������ʾ������ʾ��"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_166%>"));//<%IDS_Monitor_166%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	BOOL ret = RloginMonitorValue(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								strProxyServer.GetBuffer(strProxyServer.GetLength()), 
								nProxyPort, 
								strProxyUid.GetBuffer(strProxyUid.GetLength()), 
								strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
								strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
								strPrompt.GetBuffer(strPrompt.GetLength()), 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								SERVICE_TYPE_MONITOR, 
								strService.GetBuffer(strService.GetLength()), 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);

	WSA_Free();

	return ret;
}

extern "C" __declspec(dllexport) 
bool SERVICE(const char *strParas,  char *szReturn,int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	int nProtoType = 0;
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROTOCOLTYPE__, 0) == 0)
		{
			nProtoType =atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOLTYPE__)));
			break;
		}
	}

	if(nProtoType == 0) nProtoType = 2;

	bool bRet;
	switch(nProtoType)
	{
	case 1:
		bRet= SSHSERVICE(paramList, szReturn);
		break;
	case 2:
		bRet= TELSERVICE(paramList, szReturn);
		break;
	//case 3:
	//	return HTTPSERVICE(paramList, szReturn);
	//case 4:
	//	return RLOGINSERVICE(paramList, szReturn);
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_PROTOCOL_TYPE_ERROR"));//<%IDS_Monitor_172%>"Э�����ʹ���"
		return false;
	}
	
	CString strTmp=szReturn;
	MakeCharByString(szReturn,nSize,strTmp);

	return bRet;
}





bool TELUSERCPU(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T(""), 
				strUserName = _T("");

	int			nPort = 23; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find("userName=", 0) == 0)
		{
			strUserName = strTemp.Right(strTemp.GetLength() - strlen("userName="));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_IP_ADDRESS_NULL"));//<%IDS_Monitor_134%>"ȱ�ٷ�����"
		return false;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_PORT_NULL"));//<%IDS_Monitor_135%>"����˿ڴ���"
		return false;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_USER_NULL"));//<%IDS_Monitor_136%>"ȱ���û���"
		return false;
	}
	if(strUserName.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVICE",
            "UNIX_SERVICE_NAME_NULL"));//<%IDS_Monitor_164%>"ȱ�ٷ�������"
		return false;
	}
	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SREVER",
            "UNIX_SERVER_PROMPT_NULL"));//<%IDS_Monitor_132%>"�����¼��ʾ����������ʾ������ʾ��"
		return false;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_137%>"ͨ�ų�ʼ������"
		return false;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
    bool ret = true;
	printf("%s -%s ", strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()));
    try
    {
	    ret = MonitorValue(strServer.GetBuffer(strServer.GetLength()), 
							nPort, 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
							strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
							strPrompt.GetBuffer(strPrompt.GetLength()), 
							strIniFile.GetBuffer(strIniFile.GetLength()), 
							USER_CPU_MONITOR, 
							strUserName.GetBuffer(strUserName.GetLength()), 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);
    }
    catch(...)
    {
        WriteLogFile("Monitor Service faile(Telnet type)");
        char chMsg[256] = {0};
        sprintf(chMsg, "Telnet monitor process Failed:%s", strServer);
        WriteLogFile(chMsg);
    }

	WSA_Free();

	return ret;
}

bool SSHUSERCPU(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T(""), 
				strUserName = _T(""),
				strPrivateKeyFile = _T("");
	int			nPort = 22; 

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find("userName=", 0) == 0)
		{
			strUserName = strTemp.Right(strTemp.GetLength() - strlen("userName="));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else if(strTemp.Find("_PriKeyPath=", 0) == 0)
		{
			strPrivateKeyFile = strTemp.Right(strTemp.GetLength() - strlen("_PriKeyPath="));
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_IP_ADDRESS_NULL"));//<%IDS_Monitor_134%>"ȱ�ٷ�����"
		return false;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_PORT_NULL"));//<%IDS_Monitor_135%>"����˿ڴ���"
		return false;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_USER_NULL"));//<%IDS_Monitor_136%>"ȱ���û���"
		return false;
	}
	if(strUserName.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVICE",
            "UNIX_SERVICE_NAME_NULL"));//<%IDS_Monitor_170%>"ȱ�ٷ�������"
		return false;
	}


	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_137%>"ͨ�ų�ʼ������"
		return false;
	}

	bool ret = SSHMonitorValue(	strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								USER_CPU_MONITOR,
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								strUserName.GetBuffer(strUserName.GetLength()), 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()),
								strPrivateKeyFile.GetBuffer(strPrivateKeyFile.GetLength()),
								szReturn);

	WSA_Free();

	return ret;
}

extern "C" __declspec(dllexport) 
bool GetUserCPU(const char *strParas,  char *szReturn,int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	int nProtoType = 0;
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROTOCOLTYPE__, 0) == 0)
		{
			nProtoType =atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOLTYPE__)));
			break;
		}
	}

	if(nProtoType == 0) nProtoType = 2;

	bool bRet;
	switch(nProtoType)
	{
	case 1:
		bRet= SSHUSERCPU(paramList, szReturn);
		break;
	case 2:
		bRet= TELUSERCPU(paramList, szReturn);
		break;
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_PROTOCOL_TYPE_ERROR"));//<%IDS_Monitor_172%>"Э�����ʹ���"
		return false;
	}
	
	CString strTmp=szReturn;
	MakeCharByString(szReturn,nSize,strTmp);

	return bRet;
}

/****************************************************************************
	Export Functions Of Script Monitor
****************************************************************************/
BOOL TELSCRIPT(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T(""), 
				strScript = _T(""), 
				strParam = _T(""), 
				strcMatch = _T("");
	int			nPort = 23; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__SCRIPT__, 0) == 0)
		{
			strScript = strTemp.Right(strTemp.GetLength() - strlen(__SCRIPT__));
		}
		else if(strTemp.Find(__PARAMETERS__, 0) == 0)
		{
			strParam = strTemp.Right(strTemp.GetLength() - strlen(__PARAMETERS__));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strcMatch = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_173%>"));//<%IDS_Monitor_173%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_174%>"));//<%IDS_Monitor_174%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_175%>"));//<%IDS_Monitor_175%>"ȱ���û���"
		return FALSE;
	}
	if(strScript.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_176%>"));//<%IDS_Monitor_176%>"ȱ�ٽű��ļ�����"
		return FALSE;
	}
	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_177%>"));//<%IDS_Monitor_177%>"�����¼��ʾ����������ʾ������ʾ��"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_178%>"));//<%IDS_Monitor_178%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	BOOL ret = TELEXESCRIPT(strServer.GetBuffer(strServer.GetLength()), 
							nPort, 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
							strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
							strPrompt.GetBuffer(strPrompt.GetLength()), 
							strIniFile.GetBuffer(strIniFile.GetLength()), 
							strScript.GetBuffer(strScript.GetLength()), 
							strParam.GetBuffer(strParam.GetLength()), 
							strcMatch.GetBuffer(strcMatch.GetLength()), 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);

	WSA_Free();

	return ret;
}

BOOL SSHSCRIPT(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T(""), 
				strScript = _T(""), 
				strParam = _T(""), 
				strcMatch = _T("");
	int			nPort = 22; 

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__SCRIPT__, 0) == 0)
		{
			strScript = strTemp.Right(strTemp.GetLength() - strlen(__SCRIPT__));
		}
		else if(strTemp.Find(__PARAMETERS__, 0) == 0)
		{
			strParam = strTemp.Right(strTemp.GetLength() - strlen(__PARAMETERS__));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strcMatch = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_179%>"));//<%IDS_Monitor_179%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_180%>"));//<%IDS_Monitor_180%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_181%>"));//<%IDS_Monitor_181%>"ȱ���û���"
		return FALSE;
	}
	if(strScript.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_182%>"));//<%IDS_Monitor_182%>"ȱ�ٽű��ļ�����"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_183%>"));//<%IDS_Monitor_183%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL ret = SSHEXESCRIPT(strServer.GetBuffer(strServer.GetLength()), 
							nPort, 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strIniFile.GetBuffer(strIniFile.GetLength()), 
							strScript.GetBuffer(strScript.GetLength()), 
							strParam.GetBuffer(strParam.GetLength()), 
							strcMatch.GetBuffer(strcMatch.GetLength()), 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);

	WSA_Free();

	return ret;
}

BOOL HTTPSCRIPT(CStringList &paramList, char *szReturn)
{
	CString		strRemoteUrl = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T(""), 
				strScript = _T(""), 
				strParam = _T(""), 
				strcMatch = _T("");

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strRemoteUrl = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__SCRIPT__, 0) == 0)
		{
			strScript = strTemp.Right(strTemp.GetLength() - strlen(__SCRIPT__));
		}
		else if(strTemp.Find(__PARAMETERS__, 0) == 0)
		{
			strParam = strTemp.Right(strTemp.GetLength() - strlen(__PARAMETERS__));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strcMatch = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strRemoteUrl.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_173%>"));//<%IDS_Monitor_173%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(strScript.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_176%>"));//<%IDS_Monitor_176%>"ȱ�ٽű��ļ�����"
		return FALSE;
	}

	return HTTPEXESCRIPT(strRemoteUrl.GetBuffer(strRemoteUrl.GetLength()), 
						strUID.GetBuffer(strUID.GetLength()), 
						strPWD.GetBuffer(strPWD.GetLength()), 
						strProxyServer.GetBuffer(strProxyServer.GetLength()), 
						nProxyPort, 
						strProxyUid.GetBuffer(strProxyUid.GetLength()), 
						strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
						strIniFile.GetBuffer(strIniFile.GetLength()), 
						strScript.GetBuffer(strScript.GetLength()), 
						strParam.GetBuffer(strParam.GetLength()), 
						strcMatch.GetBuffer(strcMatch.GetLength()), 
						strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
						szReturn);
}

BOOL RLOGINSCRIPT(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T(""), 
				strScript = _T(""), 
				strParam = _T(""), 
				strcMatch = _T("");
	int			nPort = 513; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__SCRIPT__, 0) == 0)
		{
			strScript = strTemp.Right(strTemp.GetLength() - strlen(__SCRIPT__));
		}
		else if(strTemp.Find(__PARAMETERS__, 0) == 0)
		{
			strParam = strTemp.Right(strTemp.GetLength() - strlen(__PARAMETERS__));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strcMatch = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_173%>"));//<%IDS_Monitor_173%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_174%>"));//<%IDS_Monitor_174%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_175%>"));//<%IDS_Monitor_175%>"ȱ���û���"
		return FALSE;
	}
	if(strScript.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_176%>"));//<%IDS_Monitor_176%>"ȱ�ٽű��ļ�����"
		return FALSE;
	}
	if(strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_177%>"));//<%IDS_Monitor_177%>"�����¼��ʾ����������ʾ������ʾ��"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_178%>"));//<%IDS_Monitor_178%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	BOOL ret = RLOGINEXESCRIPT(strServer.GetBuffer(strServer.GetLength()), 
							nPort, 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
							strPrompt.GetBuffer(strPrompt.GetLength()), 
							strIniFile.GetBuffer(strIniFile.GetLength()), 
							strScript.GetBuffer(strScript.GetLength()), 
							strParam.GetBuffer(strParam.GetLength()), 
							strcMatch.GetBuffer(strcMatch.GetLength()), 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);

	WSA_Free();

	return ret;
}

extern "C" __declspec(dllexport) 
BOOL SCRIPT(const char *strParas, char * szReturn, int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList paramList;

	MakeStringListByChar(paramList,strParas);
	int nProtoType = 0;
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROTOCOLTYPE__, 0) == 0)
		{
			nProtoType =atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOLTYPE__)));
			break;
		}
	}

	if(nProtoType == 0) nProtoType = 2;

	BOOL bRet;
	switch(nProtoType)
	{
	case 1:
		bRet= SSHSCRIPT(paramList, szReturn);
		break;
	case 2:
		bRet= TELSCRIPT(paramList, szReturn);
		break;
	case 3:
		return HTTPSCRIPT(paramList, szReturn);
	case 4:
		return RLOGINSCRIPT(paramList, szReturn);
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_184%>"));//<%IDS_Monitor_184%>"Э�����ʹ���"
		return FALSE;
	}
	CString strTmp=szReturn;
	MakeCharByString(szReturn,nSize,strTmp);

	return bRet;
}

/****************************************************************************
	Export Function Of Scripts (Fetch Script Name)
**************************************************************************IDS_Monitor_184*/
BOOL TELSCRIPTS(CStringList &paramList, char * szOut)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T("");
	int			nPort = 23; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

if(strServer.IsEmpty())
	{
		sprintf(szOut, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_73%>"));//<%IDS_Monitor_73%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szOut, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_74%>"));//<%IDS_Monitor_74%>"����˿ڴ���"
		return FALSE;
	}

	if(strUID.IsEmpty())
	{
		sprintf(szOut, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_75%>"));//<%IDS_Monitor_75%>"ȱ���û���"
		return FALSE;
	}

	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szOut, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_77%>"));//<%IDS_Monitor_77%>"�����¼��ʾ����������ʾ������ʾ��"
		return FALSE;
	}
	
	if(WSA_Init())
		return FALSE;

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	CString strResult = TELGETSCRIPT(strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strUID.GetBuffer(strUID.GetLength()), 
									strPWD.GetBuffer(strPWD.GetLength()), 
									strProxyServer.GetBuffer(strProxyServer.GetLength()), 
									nProxyPort, 
									strProxyUid.GetBuffer(strProxyUid.GetLength()), 
									strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
									strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
									strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
									strPrompt.GetBuffer(strPrompt.GetLength()), 
									strIniFile.GetBuffer(strIniFile.GetLength()), 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()));

	WSA_Free();


	if(strResult.IsEmpty())
		return FALSE;

	CString strFilePath=_T("");
	
	strFilePath.Format("%s\\fcgi-bin\\AimParser.dll", FuncGetInstallPath());
//	puts(strFilePath);

	PARSER *pFileParser;
	HINSTANCE hInstance = ::LoadLibrary(strFilePath);
	if (!hInstance) 
	{
//		sprintf(szReturn, "%s", FuncGetStringFromIDS("<%IDS_Monitor_185%>"));//<%IDS_Monitor_185%>"����װ�ش���"
		return FALSE;
	}

	pFileParser = (PARSER *)::GetProcAddress(hInstance, "FileParser");
	if(!pFileParser)
	{
		if(hInstance) ::FreeLibrary(hInstance);
//		sprintf(szReturn, "%s", FuncGetStringFromIDS("<%IDS_Monitor_186%>"));//<%IDS_Monitor_186%>"����װ�ش���"
		return FALSE;
	}

	
	(*pFileParser)(strResult, 21, szOut, strIniFile.GetBuffer(strIniFile.GetLength()));
	
	if(hInstance) ::FreeLibrary(hInstance);

	return TRUE;
}

BOOL SSHSCRIPTS(CStringList &paramList, char * szOut)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T(""); 
	int			nPort = 22; 

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szOut, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_179%>"));//<%IDS_Monitor_179%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szOut, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_180%>"));//<%IDS_Monitor_180%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szOut, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_181%>"));//<%IDS_Monitor_181%>"ȱ���û���"
		return FALSE;
	}



	if(WSA_Init())
		return FALSE;

	CString strResult = SSHGETSCRIPT(strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strUID.GetBuffer(strUID.GetLength()), 
									strPWD.GetBuffer(strPWD.GetLength()), 
									strIniFile.GetBuffer(strIniFile.GetLength()), 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()));

	WSA_Free();

	if(strResult.IsEmpty())
		return FALSE;

	PARSER *pFileParser;
	HINSTANCE hInstance = ::LoadLibrary("AimParser.dll");
	if (!hInstance) 
	{
//		sprintf(szReturn, "%s", FuncGetStringFromIDS("<%IDS_Monitor_188%>"));//<%IDS_Monitor_188%>"����װ�ش���"
		return FALSE;
	}

	pFileParser = (PARSER *)::GetProcAddress(hInstance, "FileParser");
	if(!pFileParser)
	{
		if(hInstance) ::FreeLibrary(hInstance);
//		sprintf(szReturn, "%s", FuncGetStringFromIDS("<%IDS_Monitor_189%>"));//<%IDS_Monitor_189%>"����װ�ش���"
		return FALSE;
	}


	(*pFileParser)(strResult, 21, szOut, strIniFile.GetBuffer(strIniFile.GetLength()));
	if(hInstance) ::FreeLibrary(hInstance);

	return TRUE;
}

BOOL HTTPSCRIPTS(CStringList &paramList, CStringList &lstTexts, CStringList &lstValues)
{
	CString		strRemoteUrl = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T("");

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strRemoteUrl = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strRemoteUrl.IsEmpty())
		return FALSE;

	CString strResult = HTTPGETSCRIPT(strRemoteUrl.GetBuffer(strRemoteUrl.GetLength()), 
									strUID.GetBuffer(strUID.GetLength()), 
									strPWD.GetBuffer(strPWD.GetLength()), 
									strProxyServer.GetBuffer(strProxyServer.GetLength()), 
									nProxyPort, 
									strProxyUid.GetBuffer(strProxyUid.GetLength()), 
									strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
									strIniFile.GetBuffer(strIniFile.GetLength()), 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()));

	if(strResult.IsEmpty())
		return FALSE;

	PARSER *pFileParser;
	HINSTANCE hInstance = ::LoadLibrary("AimParser.dll");
	if (!hInstance) 
	{
//		sprintf(szReturn, "%s", FuncGetStringFromIDS("<%IDS_Monitor_185%>"));//<%IDS_Monitor_185%>"����װ�ش���"
		return FALSE;
	}

	pFileParser = (PARSER *)::GetProcAddress(hInstance, "FileParser");
	if(!pFileParser)
	{
		if(hInstance) ::FreeLibrary(hInstance);
//		sprintf(szReturn, "%s", FuncGetStringFromIDS("<%IDS_Monitor_186%>"));//<%IDS_Monitor_186%>"����װ�ش���"
		return FALSE;
	}

	char szOut[8192] = {0};
	(*pFileParser)(strResult, 21, szOut, strIniFile.GetBuffer(strIniFile.GetLength()));

	int index = 0;
	CString strScripts = szOut;
	while((index = strScripts.Find("$$$", 0)) > 0)
	{
		lstTexts.AddTail(strScripts.Left(index));
		lstValues.AddTail(strScripts.Left(index));
		strScripts.Delete(0, index + 3);
	}

	if(lstTexts.GetCount() == 0)
	{
		if(hInstance) ::FreeLibrary(hInstance);
//		sprintf(szReturn, "%s", FuncGetStringFromIDS("<%IDS_Monitor_187%>"));//<%IDS_Monitor_187%>"�����ڽű�"
		return FALSE;
	}

	if(hInstance) ::FreeLibrary(hInstance);

	return TRUE;
}
BOOL RLOGINSCRIPTS(CStringList &paramList, CStringList &lstTexts, CStringList &lstValues)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T("");
	int			nPort = 513; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
		return FALSE;
	if(nPort == 0)
		return FALSE;
	if(strUID.IsEmpty())
		return FALSE;
	if(strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
		return FALSE;

	if(WSA_Init())
		return FALSE;

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	CString strResult = RLOGINGETSCRIPT(strServer.GetBuffer(strServer.GetLength()), 
										nPort, 
										strUID.GetBuffer(strUID.GetLength()), 
										strPWD.GetBuffer(strPWD.GetLength()), 
										strProxyServer.GetBuffer(strProxyServer.GetLength()), 
										nProxyPort, 
										strProxyUid.GetBuffer(strProxyUid.GetLength()), 
										strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
										strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
										strPrompt.GetBuffer(strPrompt.GetLength()), 
										strIniFile.GetBuffer(strIniFile.GetLength()), 
										strCustomerPath.GetBuffer(strCustomerPath.GetLength()));

	WSA_Free();

	if(strResult.IsEmpty())
		return FALSE;

	PARSER *pFileParser;
	HINSTANCE hInstance = ::LoadLibrary("AimParser.dll");
	if (!hInstance) 
	{
//		sprintf(szReturn, "%s", FuncGetStringFromIDS("<%IDS_Monitor_185%>"));//<%IDS_Monitor_185%>"����װ�ش���"
		return FALSE;
	}

	pFileParser = (PARSER *)::GetProcAddress(hInstance, "FileParser");
	if(!pFileParser)
	{
		if(hInstance) ::FreeLibrary(hInstance);
//		sprintf(szReturn, "%s", FuncGetStringFromIDS("<%IDS_Monitor_186%>"));//<%IDS_Monitor_186%>"����װ�ش���"
		return FALSE;
	}

	char szOut[8192] = {0};
	(*pFileParser)(strResult, 21, szOut, strIniFile.GetBuffer(strIniFile.GetLength()));

	int index = 0;
	CString strScripts = szOut;
	while((index = strScripts.Find("$$$", 0)) > 0)
	{
		lstTexts.AddTail(strScripts.Left(index));
		lstValues.AddTail(strScripts.Left(index));
		strScripts.Delete(0, index + 3);
	}

	if(lstTexts.GetCount() == 0)
	{
		if(hInstance) ::FreeLibrary(hInstance);
//		sprintf(szReturn, "%s", FuncGetStringFromIDS("<%IDS_Monitor_187%>"));//<%IDS_Monitor_187%>"�����ڽű�"
		return FALSE;
	}

	if(hInstance) ::FreeLibrary(hInstance);

	return TRUE;
}

extern "C" __declspec(dllexport) 
BOOL SCRIPTS(const char *strParas, char * strReturn, int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList paramList;
	CStringList lstTexts;
	CStringList lstValues;

	MakeStringListByChar(paramList,strParas);

	int nProtoType = 0;
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROTOCOLTYPE__, 0) == 0)
		{
			nProtoType =atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOLTYPE__)));
			break;
		}
	}

	if(nProtoType == 0) nProtoType = 2;

	char szReturn[8192]={0};
	int bRet;
	switch(nProtoType)
	{
	case 1:
		bRet=SSHSCRIPTS(paramList, szReturn);
		break;
	case 2:
		bRet= TELSCRIPTS(paramList, szReturn);
		break;
	case 3:
		return HTTPSCRIPTS(paramList, lstTexts, lstValues);
	case 4:
		return RLOGINSCRIPTS(paramList, lstTexts, lstValues);
	default:
		return FALSE;
	}

	if(strstr(szReturn,"error=")!=NULL)
	{
		strcpy(strReturn,szReturn);
		return false;
	}

	char  *sa =strReturn;
	int lsize;
	
	lsize =strlen(szReturn);
	
	if(bRet&&nSize>lsize*2)
	{
		nSize =0;
		char *ca = szReturn, *cb = NULL;
		while(1)
		{
			if(cb = strstr(ca, "$$$"))
			{
				char proc[1024] = {0};
				strncpy(proc, ca, cb - ca);
				ca = cb + 3;
				if(strlen(proc)>0)
					sprintf(sa,"%s=%s",proc,proc);
				nSize=nSize+2* strlen(proc)+2;
				sa=sa+2* strlen(proc)+2;


			}
			else
			{
				break;
			}
		}
		OutputDebugString(strReturn);
		
	}
	return TRUE;
}
void MakeDisksReturn(char * szReturn,int& nSize )
{
	char *strParam;

	strParam =_strdup(szReturn);
	std::list<string> 	lstPairs;
	char *ca = strParam, *cb = NULL;
		while(1)
		{
			if(cb = strstr(ca, "$$$"))
			{
				char pair[256] = {0};
				strncpy(pair, ca, cb - ca);
				lstPairs.push_back(pair);
				ca = cb + 3;
			}
			else
			{
				break;
			}
		}

	
		char tmpdisk[256];
		std::list<string> lsResult;
		for(std::list<string>::iterator It= lstPairs.begin();It!=lstPairs.end();It++)
		{
			
			sprintf(tmpdisk,"%s",It->c_str());
			WriteLog("tmpdisk=");
			WriteLog(tmpdisk);

			ca =tmpdisk;
			cb= strstr(ca,"###");
			if(cb)
			{
				char val[256]={0};
				char txt[256]={0};
				std::string strRet;
				strncpy(val, ca, cb - ca);

				strcpy(txt,cb+3);

				WriteLog("val=");
				WriteLog(val);
				WriteLog("txt=");
				WriteLog(txt);

				//if(strcmp(val,"none")!=0)
				//{
				//	strRet= val;
				//	
				//}else  strRet =txt;
				//strRet+="=";
				//strRet+=val;
				//strRet+="(";
				//strRet+=txt;
				//strRet+=")";
				
				//modified by zjw 2007.12.6
				//if(strcmp(val,"none")!=0)
				//{
				//	strRet= val;
				//	strRet+="=";
				//	strRet+= val;
				//}
				//else
				{
					strRet = txt;
					strRet += " ";
					strRet += val;
					strRet += "=";
					strRet += txt; 
					strRet += " ";
					strRet += val;
				}
				WriteLog("strRet=");
				WriteLog(strRet.c_str());


				lsResult.push_back(strRet);

			}
			
		}
		ca =szReturn;
		memset(szReturn,0,nSize);
		int lsize=0;
		while(lsResult.size()>0 && lsize<nSize)
		{
			std::string strTmp =lsResult.front();
			lsResult.pop_front();
			strcpy(ca,strTmp.c_str());
			ca=ca+strTmp.size()+1;
			lsize+=strTmp.size();
			lsize+=1;
			
		}
}


/****************************************************************************
	Export Function Of Disks (Fetch Disk Name)
****************************************************************************/
bool TELDISKS(CStringList &paramList, char *strReturn,int nSize)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T("");
	int			nPort = 23; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
	}

	if(strServer.IsEmpty())
		return false;

	if(nPort == 0)
		return false;

	if(strUID.IsEmpty())
		return false;

	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
		return false;

	if(WSA_Init())
		return false;

	char szReturn[8192] = {0};
	strPrompt.TrimLeft();
	strPrompt.TrimRight();
    bool bResult = TRUE;
    try
    {
	    bResult = MonitorValue(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								strProxyServer.GetBuffer(strProxyServer.GetLength()), 
								nProxyPort, 
								strProxyUid.GetBuffer(strProxyUid.GetLength()), 
								strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
								strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
								strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
								strPrompt.GetBuffer(strPrompt.GetLength()), 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								DISKS_TYPE_MONITOR, 
								NULL, 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);
    }
    catch(...)
    {
        WriteLogFile("List Disk faile(Telnet type)");
        char chMsg[256] = {0};
        sprintf(chMsg, "Telnet list disks Failed:%s", strServer);
        WriteLogFile(chMsg);
    }

	if(static_cast<int>(strlen(szReturn)) > nSize)
    {
		return false;
    }
	else 
    {
		sprintf(strReturn,szReturn);
		MakeDisksReturn(strReturn,nSize);
	}


	WSA_Free();

	return bResult;
}

bool SSHDISKS(CStringList &paramList, char *strReturn,int nSize)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T(""),
				strPrivateKeyFile = _T(""); 

	int			nPort = 22; 

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else if(strTemp.Find("_PriKeyPath=", 0) == 0)
		{
			strPrivateKeyFile = strTemp.Right(strTemp.GetLength() - strlen("_PriKeyPath="));
		}
	}

	if(strServer.IsEmpty())
		return false;

	if(nPort == 0)
		return false;

	if(strUID.IsEmpty())
		return false;

	if(WSA_Init())
		return false;

	char szReturn[8192] = {0};
	bool bResult = SSHMonitorValue(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								DISKS_TYPE_MONITOR, 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								NULL, 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								strPrivateKeyFile.GetBuffer(strPrivateKeyFile.GetLength()),
								szReturn);

	if(static_cast<int>(strlen(szReturn)) > nSize)
    {
		return false;
    }
	else 
    {
		sprintf(strReturn,szReturn);
		MakeDisksReturn(strReturn,nSize);
	}

	WSA_Free();

	return bResult;
}

BOOL HTTPDISKS(CStringList &paramList, CStringList &lstTexts, CStringList &lstValues)
{
	CString		strRemoteUrl = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T("");

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strRemoteUrl = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strRemoteUrl.IsEmpty())
		return FALSE;

	char szReturn[8192] = {0};
	BOOL bResult = HttpMonitorValue(strRemoteUrl.GetBuffer(strRemoteUrl.GetLength()), 
									strUID.GetBuffer(strUID.GetLength()), 
									strPWD.GetBuffer(strPWD.GetLength()), 
									strProxyServer.GetBuffer(strProxyServer.GetLength()), 
									nProxyPort, 
									strProxyUid.GetBuffer(strProxyUid.GetLength()), 
									strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
									strIniFile.GetBuffer(strIniFile.GetLength()), 
									DISKS_TYPE_MONITOR, 
									NULL, 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									szReturn);

	if(bResult)
	{
		CStringList	lstPairs;
		char *ca = szReturn, *cb = NULL;
		while(1)
		{
			if(cb = strstr(ca, "$$$"))
			{
				char pair[256] = {0};
				strncpy(pair, ca, cb - ca);
				lstPairs.AddTail(pair);
				ca = cb + 3;
			}
			else
			{
				break;
			}
		}

		if(lstPairs.GetCount() == 0)
			bResult = FALSE;
		else
		{
			pos = lstPairs.FindIndex(0);
			while(pos)
			{
				CString strTemp = lstPairs.GetNext(pos);
				int index = strTemp.Find("###", 0);
				lstValues.AddTail(strTemp.Left(index));
				strTemp += ")";
				strTemp.Replace("###", " (");
				lstTexts.AddTail(strTemp);
			}
		}
	}

	return bResult;
}

BOOL RLOGINDISKS(CStringList &paramList, CStringList &lstTexts, CStringList &lstValues)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T("");
	int			nPort = 513; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
		return FALSE;
	if(nPort == 0)
		return FALSE;
	if(strUID.IsEmpty())
		return FALSE;
	if(strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
		return FALSE;

	if(WSA_Init())
		return FALSE;

	char szReturn[8192] = {0};
	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	BOOL bResult = RloginMonitorValue(strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strUID.GetBuffer(strUID.GetLength()), 
									strPWD.GetBuffer(strPWD.GetLength()), 
									strProxyServer.GetBuffer(strProxyServer.GetLength()), 
									nProxyPort, 
									strProxyUid.GetBuffer(strProxyUid.GetLength()), 
									strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
									strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
									strPrompt.GetBuffer(strPrompt.GetLength()), 
									strIniFile.GetBuffer(strIniFile.GetLength()), 
									DISKS_TYPE_MONITOR, 
									NULL, 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									szReturn);

	if(bResult)
	{
		CStringList	lstPairs;
		char *ca = szReturn, *cb = NULL;
		while(1)
		{
			if(cb = strstr(ca, "$$$"))
			{
				char pair[256] = {0};
				strncpy(pair, ca, cb - ca);
				lstPairs.AddTail(pair);
				ca = cb + 3;
			}
			else
			{
				break;
			}
		}

		if(lstPairs.GetCount() == 0)
			bResult = FALSE;
		else
		{
			pos = lstPairs.FindIndex(0);
			while(pos)
			{
				CString strTemp = lstPairs.GetNext(pos);
				int index = strTemp.Find("###", 0);
				lstValues.AddTail(strTemp.Left(index));
				strTemp += ")";
				strTemp.Replace("###", " (");
				lstTexts.AddTail(strTemp);
			}
		}
	}

	WSA_Free();

	return bResult;
}

//extern "C" __declspec(dllexport) 
//BOOL DISKS(CStringList &paramList, CStringList &lstTexts, CStringList &lstValues)
extern "C" __declspec(dllexport) 
bool  DISKS(const char *strParas, char * strReturn, int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList paramList;
	CStringList lstTexts;
	CStringList lstValues;

	MakeStringListByChar(paramList,strParas);


	int nProtoType = 0;
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROTOCOLTYPE__, 0) == 0)
		{
			nProtoType =atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOLTYPE__)));
			break;
		}
	}

	if(nProtoType == 0) nProtoType = 2;

	switch(nProtoType)
	{
	case 1:
		return SSHDISKS(paramList, strReturn,nSize);
	case 2:
		return TELDISKS(paramList, strReturn,nSize);
	//case 3:
	//	return HTTPDISKS(paramList, lstTexts, lstValues);
	//case 4:
	//	return RLOGINDISKS(paramList, lstTexts, lstValues);
	default:
		return false;
	}
	//MakeDisksReturn(strReturn,nSize);

	return true;
}

/****************************************************************************
	Export Function Of Services (Fetch Service Name)
****************************************************************************/
BOOL TELSERVICES(CStringList &paramList, char *szReturn,int nSize)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T("");
	int			nPort = 23; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
		return FALSE;
	if(nPort == 0)
		return FALSE;
	if(strUID.IsEmpty())
		return FALSE;
	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
		return FALSE;

	if(WSA_Init())
		return FALSE;

	
	strPrompt.TrimLeft();
	strPrompt.TrimRight();
    BOOL bResult = TRUE;
	printf("%s -%s ", strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()));
    try
    {
		
	    bResult = MonitorValue(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								strProxyServer.GetBuffer(strProxyServer.GetLength()), 
								nProxyPort, 
								strProxyUid.GetBuffer(strProxyUid.GetLength()), 
								strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
								strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
								strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
								strPrompt.GetBuffer(strPrompt.GetLength()), 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								SERVICE_TYPE_MONITOR, 
								NULL, 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);
								
		bResult=TRUE;
    }
    catch(...)
    {
        WriteLogFile("List service faile(Telnet type)");
        char chMsg[256] = {0};
        sprintf(chMsg, "Telnet list processes Failed:%s", strServer);
        WriteLogFile(chMsg);
    }

	WSA_Free();

	return bResult;
}

BOOL SSHSERVICES(CStringList &paramList,  char *szReturn,int nSize)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T(""),
				strPrivateKeyFile = _T("");
	int			nPort = 22; 

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else if(strTemp.Find("_PriKeyPath=", 0) == 0)
		{
			strPrivateKeyFile = strTemp.Right(strTemp.GetLength() - strlen("_PriKeyPath="));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
		return FALSE;
	if(nPort == 0)
		return FALSE;
	if(strUID.IsEmpty())
		return FALSE;

	if(WSA_Init())
		return FALSE;

	
	BOOL bResult = SSHMonitorValue(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								SERVICE_TYPE_MONITOR, 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								NULL, 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								strPrivateKeyFile.GetBuffer(strPrivateKeyFile.GetLength()),
								szReturn);


	WSA_Free();

	return bResult;
}

BOOL HTTPSERVICES(CStringList &paramList, CStringList &lstTexts, CStringList &lstValues)
{
	CString		strRemoteUrl = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strIniFile = _T("");

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strRemoteUrl = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strRemoteUrl.IsEmpty())
		return FALSE;

	char szReturn[8192] = {0};
	BOOL bResult = HttpMonitorValue(strRemoteUrl.GetBuffer(strRemoteUrl.GetLength()), 
									strUID.GetBuffer(strUID.GetLength()), 
									strPWD.GetBuffer(strPWD.GetLength()), 
									strProxyServer.GetBuffer(strProxyServer.GetLength()), 
									nProxyPort, 
									strProxyUid.GetBuffer(strProxyUid.GetLength()), 
									strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
									strIniFile.GetBuffer(strIniFile.GetLength()), 
									SERVICE_TYPE_MONITOR, 
									NULL, 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									szReturn);

	if(bResult)
	{
		char *ca = szReturn, *cb = NULL;
		while(1)
		{
			if(cb = strstr(ca, "$$$"))
			{
				char proc[1024] = {0};
				strncpy(proc, ca, cb - ca);
				lstTexts.AddTail(proc);
				lstValues.AddTail(proc);

				ca = cb + 3;
			}
			else
			{
				break;
			}
		}

		if(lstTexts.GetCount() == 0 || lstValues.GetCount() == 0) 
			bResult = FALSE;
	}

	return bResult;
}

BOOL RLOGINSERVICES(CStringList &paramList, CStringList &lstTexts, CStringList &lstValues)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T("");
	int			nPort = 513; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
		return FALSE;
	if(nPort == 0)
		return FALSE;
	if(strUID.IsEmpty())
		return FALSE;
	if(strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
		return FALSE;

	if(WSA_Init())
		return FALSE;

	char szReturn[8192] = {0};
	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	BOOL bResult = RloginMonitorValue(strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strUID.GetBuffer(strUID.GetLength()), 
									strPWD.GetBuffer(strPWD.GetLength()), 
									strProxyServer.GetBuffer(strProxyServer.GetLength()), 
									nProxyPort, 
									strProxyUid.GetBuffer(strProxyUid.GetLength()), 
									strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
									strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
									strPrompt.GetBuffer(strPrompt.GetLength()), 
									strIniFile.GetBuffer(strIniFile.GetLength()), 
									SERVICE_TYPE_MONITOR, 
									NULL, 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									szReturn);

	if(bResult)
	{
		char *ca = szReturn, *cb = NULL;
		while(1)
		{
			if(cb = strstr(ca, "$$$"))
			{
				char proc[1024] = {0};
				strncpy(proc, ca, cb - ca);
				lstTexts.AddTail(proc);
				lstValues.AddTail(proc);

				ca = cb + 3;
			}
			else
			{
				break;
			}
		}

		if(lstTexts.GetCount() == 0 || lstValues.GetCount() == 0) 
			bResult = FALSE;
	}

	WSA_Free();

	return bResult;
}

extern "C" __declspec(dllexport) 
BOOL SERVICES(const char *strParas, char * strReturn, int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList paramList;
	CStringList lstTexts;
	CStringList lstValues;

	int n=nSize;


	MakeStringListByChar(paramList,strParas);

	int nProtoType = 0;
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROTOCOLTYPE__, 0) == 0)
		{
			nProtoType =atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOLTYPE__)));
			break;
		}

//		printf("%s\n",strTemp);

	}

	if(nProtoType == 0) nProtoType = 2;

	BOOL bRBool;
	char szReturn[1024*100] = {0};
	switch(nProtoType)
	{
	case 1:
		bRBool= SSHSERVICES(paramList, szReturn,nSize);
		break;
	case 2:
		bRBool= TELSERVICES(paramList,  szReturn,nSize);
		break;
	case 3:
		bRBool= HTTPSERVICES(paramList, lstTexts, lstValues);
		break;
	case 4:
		bRBool= RLOGINSERVICES(paramList, lstTexts, lstValues);
		break;
	default:
		return FALSE;
	}

	

	char  *sa =strReturn;
	int lsize;
	
	lsize =strlen(szReturn);
	
	
	//if(bRBool&&nSize>lsize*2)
	{
		nSize =0;
		char *ca = szReturn, *cb = NULL;
		//OutputDebugString(ca);
		while(1)
		{
			
			if(cb = strstr(ca, "$$$"))
			{
				char proc[1024] = {0};
				strncpy(proc, ca, cb - ca);
				//ca = cb + 3;

				while(*cb=='$')
					cb++;

				ca=cb;

				if((nSize+2 * strlen(proc) + 2)> n)
					break;


				// 2007/7/24 ����
				// ���⣺����ԭ��Ʋ������ݸ�ʽû�п��ǲ����ַ��а���'='�ַ�����������µ������ַ����г���'='�ַ�
				// ʱ�������⡣����unix����ʱ�ͳ������޷��������ӽ����������а���'='�ַ����̵�����
				// ���������
				// 1. �޸���monitor.dll��SERVICES�����ķ���ֵ�㷨����ԭ����[������]=[������]�ķ���ֵ�θ�ʽ��Ϊ
				//    "[������]"=[������]��
				// 2. �����µĲ��������㷨���ò���������SH::CParamsParser�Է���ֵ���ַ������н�����
				//    SH::CParamsParser�ڽ����ַ���ʱ���Ὣ""��ס���ַ������������ַ�������������ʹ����ֵ��
				//    ����ַ����г�����'='����Ҳ����Ӱ������������"[������]"=[������]����ȷ��'='�ַ��ֽ类��
				//    ��Ϊ���������ַ���������ַ���������ʾ���ұ��ַ�����Ϊ�����ò�����
				// ++++++ Ϊ[������]=[����ֵ]��ʽ����ֵ�ַ���������ַ�������"�� ++++++
				//if(strlen(proc)>0)
				//	sprintf(sa,"\"%s\"=%s",proc,proc);
				//nSize += 2 * strlen(proc) + 4;
				//sa += 2 * strlen(proc) + 4;
				// ------ Ϊ[������]=[����ֵ]��ʽ����ֵ�ַ���������ַ�������"�� ------

				/* 2007/7/24 ���� Ϊ[������]=[����ֵ]��ʽ����ֵ�ַ���������ַ�������"�š�*/
				if(strlen(proc)>0)
					sprintf(sa,"%s=%s",proc,proc);
				nSize += 2 * strlen(proc) + 2;
				sa += 2 * strlen(proc) + 2;
				

			}
			else
			{
				break;
			}
		}

		
	}


	return bRBool;
}

/****************************************************************************
	Export Function Of VPN Connection Test
*****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL VPN(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strVpnConn = _T(""), 
				strUsername = _T(""), 
				strPassword = _T(""), 
				strDomainName = _T("*");

	int			nDialBeforeVpn = 0;
	CString		strDialConn = _T(""), 
				strUsernameD = _T(""), 
				strPasswordD = _T(""), 
				strDomainNameD = _T("*");

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__VPNCONNECTION__, 0) == 0)
		{
			strVpnConn = strTemp.Right(strTemp.GetLength() - strlen(__VPNCONNECTION__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUsername = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPassword = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__DOMAINNAME__, 0) == 0)
		{
			strDomainName = strTemp.Right(strTemp.GetLength() - strlen(__DOMAINNAME__));
		}
		else if(strTemp.Find(__DIALBEFOREVPN__, 0) == 0)
		{
			nDialBeforeVpn = atoi(strTemp.Right(strTemp.GetLength() - strlen(__DIALBEFOREVPN__)));
		}
		else if(strTemp.Find(__DIALCONNECTION__, 0) == 0)
		{
			strDialConn = strTemp.Right(strTemp.GetLength() - strlen(__DIALCONNECTION__));
		}
		else if(strTemp.Find(__USERACCOUNTD__, 0) == 0)
		{
			strUsernameD = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNTD__));
		}
		else if(strTemp.Find(__PASSWORDD__, 0) == 0)
		{
			strPasswordD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDD__));
		}
		else if(strTemp.Find(__DOMAINNAMED__, 0) == 0)
		{
			strDomainNameD = strTemp.Right(strTemp.GetLength() - strlen(__DOMAINNAMED__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strVpnConn.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_217%>"));	// ��������Ͳ����������Ƚ���һ������ר������
		return FALSE;
	}

	if(strUsername.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_218%>"));	// �����������û���
		return FALSE;
	}

	if(strDomainName.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_219%>"));	// ���������ӵ�����
		return FALSE;
	}

	if(nDialBeforeVpn)
	{
		if(strDialConn.IsEmpty())
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_220%>"));	// ��������Ͳ����������Ƚ���һ����������
			return FALSE;
		}

		if(strUsernameD.IsEmpty())
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_221%>"));	// �����벦�������û���
			return FALSE;
		}

		if(strDomainNameD.IsEmpty())
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_222%>"));	// �����벦������Ҫ���ӵ�����
			return FALSE;
		}
	}

	return VPN_MONITOR(	strVpnConn.GetBuffer(strVpnConn.GetLength()), 
						strUsername.GetBuffer(strUsername.GetLength()), 
						strPassword.GetBuffer(strPassword.GetLength()), 
						strDomainName.GetBuffer(strDomainName.GetLength()), 
						nDialBeforeVpn, 
						strDialConn.GetBuffer(strDialConn.GetLength()), 
						strUsernameD.GetBuffer(strUsernameD.GetLength()), 
						strPasswordD.GetBuffer(strPasswordD.GetLength()), 
						strDomainNameD.GetBuffer(strDomainNameD.GetLength()), 
						strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
						szReturn);

}

/****************************************************************************
	Export Function Of Interfaces (Fetch Interface Index)
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL INTERFACES(const char *strParas, char * strReturn, int & nSize)
//(CStringList &paramList, CStringList &lstTexts, CStringList &lstValues)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strServer = _T(""),
				strCommunity = _T("public");
	int			nPort = 161; 

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());
	CStringList paramList;
	CStringList lstTexts;
	CStringList lstValues;

	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

//	FILE *pf=fopen("C:\\listlog.txt","a+");
//	fputs("sssss",pf);

	if(strServer.IsEmpty())
		return FALSE;
//	fputs("kkkkkk",pf);

	if(nPort == 0)
		return FALSE;
//	fputs("zzzzz",pf);

	if(WSA_Init())
		return FALSE;

//	fputs("dddddd",pf);

	CString strBuffer = _T("");
	BOOL bResult = GetInterfaces(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strCommunity.GetBuffer(strCommunity.GetLength()), 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								strBuffer);

//	fputs(strBuffer.GetBuffer(0),pf);
//	fclose(pf);

	if(bResult)
	{
		strBuffer.Replace("###","=");
		strBuffer.Replace("$$$","$");
		MakeCharByString(strReturn,nSize,strBuffer);

	
	}

	WSA_Free();

	return bResult;
}

extern "C" __declspec(dllexport) 
BOOL INTER_DESCR(CStringList &paramList, CStringList &lstTexts, CStringList &lstValues, CStringList &lstDescr)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strServer = _T(""),
				strCommunity = _T("public");
	int			nPort = 161; 

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
		return FALSE;
	if(nPort == 0)
		return FALSE;

	if(WSA_Init())
		return FALSE;

	CString strBuffer = _T("");
	BOOL bResult = GetInterfaces(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strCommunity.GetBuffer(strCommunity.GetLength()), 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								strBuffer);

	if(bResult)
	{
		CStringList	lstPairs;
		char *ca = strBuffer.GetBuffer(strBuffer.GetLength()), *cb = NULL;
		while(1)
		{
			if(cb = strstr(ca, "$$$"))
			{
				char pair[256] = {0};
				strncpy(pair, ca, cb - ca);
				lstPairs.AddTail(pair);
				ca = cb + 3;
			}
			else
			{
				break;
			}
		}

		if(lstPairs.GetCount() == 0)
			bResult = FALSE;
		else
		{
			pos = lstPairs.FindIndex(0);
			while(pos)
			{
				int index = 0;
				CString strTemp = lstPairs.GetNext(pos);
				index = strTemp.Find("###", 0);
				lstValues.AddTail(strTemp.Left(index));

				if(strTemp.Right(3) == "###")
				{
					strTemp = strTemp.Left(strTemp.GetLength() - 3);
					lstDescr.AddTail("");
				}
				else
				{
					strTemp.MakeReverse();
					index = strTemp.Find("###", 0);
					CString strDescr = strTemp.Left(index);
					strDescr.MakeReverse();
					lstDescr.AddTail(strDescr);
					strTemp.MakeReverse();
				}
				
				strTemp.Replace("###", "_");
				lstTexts.AddTail(strTemp);
			}
		}
	}

	WSA_Free();

	return bResult;
}

/****************************************************************************
	Export Function Of Remote Ping
*****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL REMOTEPING(const char *strParas,  char *szReturn,int & nSize)
//(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strServer = _T(""), 
				strRWComm = _T("public");
	int			nPort = 161;

	CString		strPingServer = _T("");
	int			nBytes = 64, 
				nTimeout = 2000;

	int			nGID = 0, 
				nMID = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__)); 
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
/*		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strRWComm = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}*/
		else if(strTemp.Find(__PRIVATE__, 0) == 0)
		{
			strRWComm = strTemp.Right(strTemp.GetLength() - strlen(__PRIVATE__));
		}
        else if(strTemp.Find(__MACHINENAME__,0)==0)
		{
			strServer=strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
        else if(strTemp.Find(__PORT__,0)==0)
        {
            nPort = atoi(strTemp.Right(strTemp.GetLength()-strlen(__PORT__)));
        }
		else if(strTemp.Find(__REMOTEMACHINENAME__, 0) == 0)
		{
			strPingServer = strTemp.Right(strTemp.GetLength() - strlen(__REMOTEMACHINENAME__));
		}
		else if(strTemp.Find(__SENDBYTES__, 0) == 0)
		{
			nBytes = atoi(strTemp.Right(strTemp.GetLength() - strlen(__SENDBYTES__)));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__GROUPID__, 0) == 0)
		{
			nGID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__GROUPID__)));
		}
		else if(strTemp.Find(__MONITORID__, 0) == 0)
		{
			nMID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__)));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_124%>"));//<%IDS_Monitor_124%>"ȱ��������ַ"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_125%>"));//<%IDS_Monitor_125%>"����˿ڴ���"
		return FALSE;
	}

	if(strPingServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_216%>")); // ������Ҫ���Ե�Զ�������ĵ�ַ
		return FALSE;
	}

	if(nBytes <= 0 || nBytes > 256)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_95%>"));//<%IDS_Monitor_95%>"�����ֽڴ�С����"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_126%>"));//<%IDS_Monitor_126%>"���볬ʱ����"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_128%>"));//<%IDS_Monitor_128%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = REMOTEPING_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strRWComm.GetBuffer(strRWComm.GetLength()), 
									strPingServer.GetBuffer(strPingServer.GetLength()), 
									nBytes, 
									nTimeout, 
									nGID, 
									nMID, 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									szReturn);

	//puts(szReturn);
	CString strTmp=szReturn;
	MakeCharByString(szReturn,nSize,strTmp);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	Export Function Of Cisco Equipments Interfaces (Fetch Interface Index)
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL CISCOINTERFACES(CStringList &paramList, CStringList &lstTexts, CStringList &lstValues)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strServer = _T(""),
				strCommunity = _T("public");
	int			nPort = 161; 

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
		return FALSE;
	if(nPort == 0)
		return FALSE;

	if(WSA_Init())
		return FALSE;

	CString strBuffer = _T("");
	BOOL bResult = cisco_GetInterfaces(	strServer.GetBuffer(strServer.GetLength()), 
										nPort, 
										strCommunity.GetBuffer(strCommunity.GetLength()), 
										strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
										strBuffer);

	if(bResult)
	{
		char *ca = strBuffer.GetBuffer(strBuffer.GetLength()), *cb = NULL;
		while(1)
		{
			if(cb = strstr(ca, "$$$"))
			{
				char pair[256] = {0};
				strncpy(pair, ca, cb - ca);
				lstTexts.AddTail(pair);
				lstValues.AddTail(pair);
				ca = cb + 3;
			}
			else
			{
				break;
			}
		}
	}

	WSA_Free();

	return bResult;
}

/****************************************************************************
	Export Function Of URL (Get URL Content)
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL CURL(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strURL = _T(""),				// URL
			strPostData = _T(""),			// Post Data
			strUsername = _T(""),			// Authorization User Name
			strPassword = _T(""),			// Authorization Password
			strProxyServer = _T(""),		// HTTP Proxy
			strProxyUsername = _T(""),		// Proxy Server User Name
			strProxyPassword = _T(""),		// Proxy Server Password
			strMatchContent = _T("");		// Match Content
	int		nProxyPort = 8080, 
			nTimeout = 60, 
			nGID = 0, 
			nMID = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__URL__, 0) == 0)
		{
			strURL = strTemp.Right(strTemp.GetLength() - strlen(__URL__));
		}
		else if(strTemp.Find(__POSTDATA__, 0) == 0)
		{
			strPostData = strTemp.Right(strTemp.GetLength() - strlen(__POSTDATA__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUsername = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPassword = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp.Delete(0, strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUsername = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPassword = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strMatchContent = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__GROUPID__, 0) == 0)
		{
			nGID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__GROUPID__)));
		}
		else if(strTemp.Find(__MONITORID__, 0) == 0)
		{
			nMID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__)));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strURL.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_191%>"));//<%IDS_Monitor_191%>"ȱ��URL"
		return FALSE;
	}

	return URL_MONITOR(	strURL.GetBuffer(strURL.GetLength()), 
						strMatchContent.GetBuffer(strMatchContent.GetLength()), 
						strPostData.GetBuffer(strPostData.GetLength()), 
						nTimeout, 
						strUsername.GetBuffer(strUsername.GetLength()), 
						strPassword.GetBuffer(strPassword.GetLength()), 
						strProxyServer.GetBuffer(strProxyServer.GetLength()), 
						nProxyPort, 
						strProxyUsername.GetBuffer(strProxyUsername.GetLength()), 
						strProxyPassword.GetBuffer(strProxyPassword.GetLength()), 
						nGID, 
						nMID, 
						strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
						szReturn);
}

/****************************************************************************
	Export Function Of GTJA (Check URL)
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL GTJA(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strURL = _T(""),				// URL
			strPostData = _T(""),			// Post Data
			strMatchContent = _T("");		// Match Content
	int		nID = 0, 		    
			nTimeout = 60000;

	int		nGroupID = 0, nMonitorID = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__URL__, 0) == 0)
		{
			strURL = strTemp.Right(strTemp.GetLength() - strlen(__URL__));
		}
		else if(strTemp.Find(__POSTDATA__, 0) == 0)
		{
			strPostData = strTemp.Right(strTemp.GetLength() - strlen(__POSTDATA__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strMatchContent = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__GTJAID__, 0) == 0)
		{
			nID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__GTJAID__)));
		}
		else if(strTemp.Find(__GROUPID__, 0) == 0)
		{			
			nGroupID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__GROUPID__)));
		}
		else if(strTemp.Find(__MONITORID__, 0) == 0)
		{			
			nMonitorID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__)));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strURL.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_192%>"));//<%IDS_Monitor_192%>"ȱ��URL"
		return FALSE;
	}

	CString strInstallPath = _T("");
	strInstallPath = ::FuncGetInstallPath();

	CString strFilePath = _T("");
	strFilePath.Format("%s\\cgi-bin\\%d-%d.html",strInstallPath,nGroupID,nMonitorID);

	CString strCurlParam = _T("");
	strCurlParam.Format("%s\\cgi-bin\\GTJA.exe", strInstallPath);

	CString strCurlParamTmp = _T("");
	strCurlParamTmp.Format(" \"%s\" \"%s\" \"%d\" \"%d\"", strURL, strPostData, nGroupID,nMonitorID);

	strCurlParam = strCurlParam + strCurlParamTmp;

	//DebugInfo("GTJA BEGIN");
	//DWORD dwStart = GetTickCount();
	//int nRet = system(strCurlParam);
	//DWORD dwEnd = GetTickCount();

	int nTotalTime = -999;
	nTotalTime = system(strCurlParam);

	if (nTotalTime < 0) {
		CString strTemp = _T("");
		strTemp.Format("error=ExitCode:%d$", nTotalTime);
		strcpy(szReturn, strTemp);
		return TRUE;
	}
	//DebugInfo("GTJA END");

	CString strRet = _T("");

	//Get DownLoadTime
	CString strContent = _T("");
	//float fDownLoadTime = (float)(1.0 * (dwEnd - dwStart) / 1000.0);
	float fDownLoadTime = (float)(1.0 * nTotalTime / 1000.0);
	strContent.Format("totaltime=%f$", fDownLoadTime);
	strRet += strContent;

	// Get File Size
	CFileException ex;
	CFile myFile;

	if (!myFile.Open((LPCTSTR)strFilePath, CFile::modeRead, &ex)) {
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);
		return TRUE;
	}

	ULONGLONG nLength = myFile.GetLength();
	myFile.Close();
	
	strContent.Format("downloadsize=%u$", nLength);
	strRet += strContent;

	CString strMatchResult = _T("");
	if(strMatchContent.IsEmpty()) //needn't match
		strMatchResult.Format("match=$");			
	else //match
	{
		FILE *fp = NULL;

		fp = fopen(strFilePath,"r");
		if(!fp)
		{
			strRet.Format("%s", FuncGetStringFromIDS("<%IDS_Monitor_193%>"));//<%IDS_Monitor_193%>"error=��ȡ�������ݳ���$"
			strcpy(szReturn,(LPCTSTR)strRet);
			return TRUE;
		}
		
		char szBuf[MAX_REC_LEN] = {0};
		fread(szBuf,1,MAX_REC_LEN,fp);
		fclose(fp);
		fp = NULL;
		
		CString strCurlReturn = _T("");
		strCurlReturn.Format("%s",szBuf);
		int iFindMatch = strCurlReturn.Find(strMatchContent);
		if(iFindMatch == -1)
			strMatchResult = FuncGetStringFromIDS("<%IDS_Monitor_212%>");//<%IDS_Monitor_212%>"match=ʧ��$pre-status=3$"
		else
			strMatchResult = FuncGetStringFromIDS("<%IDS_Monitor_213%>");//<%IDS_Monitor_213%>"match=�ɹ�$pre-status=1$"
	}

	strRet += strMatchResult;

	strcpy(szReturn,(LPCTSTR)strRet);

	return TRUE;
	//BOOL bResult = GTJA_MONITOR(strURL, strPostData, nTimeout, strMatchContent, nID, szReturn);

	//return bResult;
}

/****************************************************************************
	CiscoLog Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL CISCOCONFIG(const char *strParas,  char *szReturn,int & nSize)
//(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strPassword = _T(""), 
			strenPassword = _T("cisco"), 
			strpwdPrompt = _T("assword:"), 
			strPrompt = _T(">"), 
			strenpwdPrompt = _T("assword:"), 
			strenPrompt = _T("#"), 
			strMonitorID=_T(""),
			strRunningConfig = _T("show running-config");
	int		nPort = 23, 
			nGID = 0, 
			nMID = 0;

	CString strProxyServer = _T(""), 
			strProxyUsername = _T(""), 
			strProxyPassword = _T("");
	int		nProxyPort = 0;

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
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__CONNPORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__CONNPORT__)));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPassword = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__ENABLEPASSWORD__, 0) == 0)
		{
			strenPassword = strTemp.Right(strTemp.GetLength() - strlen(__ENABLEPASSWORD__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strpwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__ENABLEPASSWORDPROMPT__, 0) == 0)
		{
			strenpwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__ENABLEPASSWORDPROMPT__));
		}
		else if(strTemp.Find(__ENABLEPROMPT__, 0) == 0)
		{
			strenPrompt = strTemp.Right(strTemp.GetLength() - strlen(__ENABLEPROMPT__));
		}
		else if(strTemp.Find(__RUNNING_CONFIG__, 0) == 0)
		{
			strRunningConfig = strTemp.Right(strTemp.GetLength() - strlen(__RUNNING_CONFIG__));
		}
		else if(strTemp.Find(__GROUPID__, 0) == 0)
		{
			nGID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__GROUPID__)));
		}
/*		else if(strTemp.Find(__MONITORID__, 0) == 0)
		{
			nMID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__)));
		}*/
		else if(strTemp.Find(__MONITORID__, 0) == 0)
		{
			strMonitorID = strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__));
		}

		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUsername = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPassword = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
	}
    printf("Parser parameter ok\n");
	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_194%>"));//<%IDS_Monitor_194%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_195%>"));//<%IDS_Monitor_195%>"�˿ڴ���"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_196%>"));//<%IDS_Monitor_196%>"ͨ�ų�ʼ��ʧ��"
		return FALSE;
	}

	if(strRunningConfig.IsEmpty())
	{
		strRunningConfig = _T("show running-config");
	}

    printf("Monitor config\n");
/*	BOOL bResult = CISCOCONFIG_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strPassword.GetBuffer(strPassword.GetLength()), 
									strenPassword.GetBuffer(strenPassword.GetLength()), 
									strpwdPrompt.GetBuffer(strpwdPrompt.GetLength()), 
									strPrompt.GetBuffer(strPrompt.GetLength()), 
									strenpwdPrompt.GetBuffer(strenpwdPrompt.GetLength()), 
									strenPrompt.GetBuffer(strenPrompt.GetLength()), 
									strRunningConfig.GetBuffer(strRunningConfig.GetLength()), 
									nGID, 
									nMID, 
									strProxyServer.GetBuffer(strServer.GetLength()), 
									nProxyPort, 
									strProxyUsername.GetBuffer(strProxyUsername.GetLength()), 
									strProxyPassword.GetBuffer(strProxyPassword.GetLength()), 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									szReturn);*/

		BOOL bResult = CISCOCONFIG_MONITOR62(strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strPassword.GetBuffer(strPassword.GetLength()), 
									strenPassword.GetBuffer(strenPassword.GetLength()), 
									strpwdPrompt.GetBuffer(strpwdPrompt.GetLength()), 
									strPrompt.GetBuffer(strPrompt.GetLength()), 
									strenpwdPrompt.GetBuffer(strenpwdPrompt.GetLength()), 
									strenPrompt.GetBuffer(strenPrompt.GetLength()), 
									strRunningConfig.GetBuffer(strRunningConfig.GetLength()), 
									strMonitorID.GetBuffer(strMonitorID.GetLength()),
									strProxyServer.GetBuffer(strServer.GetLength()), 
									nProxyPort, 
									strProxyUsername.GetBuffer(strProxyUsername.GetLength()), 
									strProxyPassword.GetBuffer(strProxyPassword.GetLength()), 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									szReturn);


	WSA_Free();

	return bResult;
}

/****************************************************************************
	CiscoLog Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL CISCOCPU(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strCommunity = _T("public"), 
			strIndex = _T("");
	int		nPort = 161, 
			nTimeout = 5, 
			nRetryDelay = 1;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			int index = 0;
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			if((index = strTemp.Find(':', 0)) >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			strIndex = strTemp.Right(strTemp.GetLength() - strlen(__INDEX__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_197%>"));//<%IDS_Monitor_197%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_198%>"));//<%IDS_Monitor_198%>"����˿ڴ���"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_199%>"));//<%IDS_Monitor_199%>"ͨ�ų�ʼ��ʧ��"
		return FALSE;
	}

	BOOL bResult = CISCOCPU_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
									nPort, 
									strIndex, 
									strCommunity.GetBuffer(strCommunity.GetLength()), 
									nTimeout, 
									nRetryDelay, 
									strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
									szReturn);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	DHCP Server Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL DHCP(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int		nTimeout = 10;
	CString strReqAddress = _T("");

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__REQUESTADDRESS__, 0) == 0)
		{
			strReqAddress = strTemp.Right(strTemp.GetLength() - strlen(__REQUESTADDRESS__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_200%>"));//<%IDS_Monitor_200%>"���볬ʱ����"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_201%>"));//<%IDS_Monitor_201%>"ͨ�ų�ʼ��ʧ��"
		return FALSE;
	}

	BOOL bResult = DHCP_MONITOR(nTimeout, 
								strReqAddress.GetBuffer(strReqAddress.GetLength()), 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	WebLogic 5.x Application Server Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL WEBLOGIC5X(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strCommunity = _T("public"), 
			strIndex = _T("0");
	int		nPort = 161, 
			nTimeout = 5, 
			nRetryDelay = 1;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			strIndex = strTemp.Right(strTemp.GetLength() - strlen(__INDEX__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SingleReport_3%>"));//<%IDS_Monitor_202%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_203%>"));//<%IDS_Monitor_203%>"����˿ڴ���"
		return FALSE;
	}

	for(int i = 0;i < strIndex.GetLength();i ++)
	{
		TCHAR ch = strIndex.GetAt(i);
		if(ch != '.' && !isdigit(ch))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_204%>"));//<%IDS_Monitor_204%>"�������������������"
			return FALSE;
		}
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_205%>"));//<%IDS_Monitor_205%>"���볬ʱ����"
		return FALSE;
	}

	if(nRetryDelay < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_206%>"));//<%IDS_Monitor_206%>"����ʱ��������"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_207%>"));//<%IDS_Monitor_207%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = WEBLOGIC5X_MONITOR(	strServer.GetBuffer(strServer.GetLength()), 
										nPort, 
										strCommunity.GetBuffer(strCommunity.GetLength()), 
										strIndex, 
										nRetryDelay, 
										nTimeout, 
										strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
										szReturn);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	WebLogic 5.x Application Server Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL WEBLOGIC7X(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strCommunity = _T("public"), 
			strIndex = _T("0");
	int		nPort = 161, 
			nTimeout = 5, 
			nRetryDelay = 1;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nPort = atoi(strTemp);
			}
			else
			{
				strServer = strTemp;
			}
		}
		else if(strTemp.Find(__COMMUNITY__, 0) == 0)
		{
			strCommunity = strTemp.Right(strTemp.GetLength() - strlen(__COMMUNITY__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__RETRYDELAY__, 0) == 0)
		{
			nRetryDelay = atoi(strTemp.Right(strTemp.GetLength() - strlen(__RETRYDELAY__)));
		}
		else if(strTemp.Find(__INDEX__, 0) == 0)
		{
			strIndex = strTemp.Right(strTemp.GetLength() - strlen(__INDEX__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SingleReport_3%>"));//<%IDS_Monitor_202%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_203%>"));//<%IDS_Monitor_203%>"����˿ڴ���"
		return FALSE;
	}

	for(int i = 0;i < strIndex.GetLength();i ++)
	{
		TCHAR ch = strIndex.GetAt(i);
		if(ch != '.' && !isdigit(ch))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_204%>"));//<%IDS_Monitor_204%>"�������������������"
			return FALSE;
		}
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_205%>"));//<%IDS_Monitor_205%>"���볬ʱ����"
		return FALSE;
	}

	if(nRetryDelay < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_206%>"));//<%IDS_Monitor_206%>"����ʱ��������"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_207%>"));//<%IDS_Monitor_207%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	BOOL bResult = WEBLOGIC7X_MONITOR(	strServer.GetBuffer(strServer.GetLength()), 
										nPort, 
										strCommunity.GetBuffer(strCommunity.GetLength()), 
										strIndex, 
										nRetryDelay, 
										nTimeout, 
										strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
										szReturn);

	WSA_Free();

	return bResult;
}

/****************************************************************************
	WebSphere Performance Servlet Monitor Export Function
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL WEBSPHERES(CStringList &paramList, CStringList &lstTexts, CStringList &lstValues)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strServletUrl = _T("");
	int		nPort = 80, 
			nGID = 0, 
			nMID = 0;

	CString	strProxyServer = _T(""), 
			strProxyUser = _T(""), 
			strProxyPass = _T("");
	int		nProxyPort = 8080;

	CString	strUsername = _T(""), 
			strPassword = _T("");

	CString	strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__URL__, 0) == 0)
		{
			strServletUrl = strTemp.Right(strTemp.GetLength() - strlen(__URL__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUsername = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPassword = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUser = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPass = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else if(strTemp.Find(__GROUPID__, 0) == 0)
		{
			nGID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__GROUPID__)));
		}
		else if(strTemp.Find(__MONITORID__, 0) == 0)
		{
			nMID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		return FALSE;
	}

	if(strServletUrl.IsEmpty())
	{
		return FALSE;
	}

	if(nPort <= 0)
	{
		return FALSE;
	}

	if(!strProxyServer.IsEmpty() && nProxyPort <= 0)
	{
		return FALSE;
	}

	return WEBSPHERES_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
							strServletUrl.GetBuffer(strServletUrl.GetLength()), 
							nPort, 
							strUsername.GetBuffer(strUsername.GetLength()), 
							strPassword.GetBuffer(strPassword.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUser.GetBuffer(strProxyUser.GetLength()), 
							strProxyPass.GetBuffer(strProxyPass.GetLength()), 
							nGID, 
							nMID, 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							lstTexts, 
							lstValues);
}

extern "C" __declspec(dllexport) 
BOOL WEBSPHERE(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString	strServer = _T(""), 
			strServletUrl = _T(""), 
			strServerName = _T("");
	int		nPort = 80, 
			nGID = 0, 
			nMID = 0;

	CString	strProxyServer = _T(""), 
			strProxyUser = _T(""), 
			strProxyPass = _T("");
	int		nProxyPort = 8080;

	CString	strUsername = _T(""), 
			strPassword = _T("");

	CString	strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__URL__, 0) == 0)
		{
			strServletUrl = strTemp.Right(strTemp.GetLength() - strlen(__URL__));
		}
		else if(strTemp.Find(__SERVERNAME__, 0) == 0)
		{
			strServerName = strTemp.Right(strTemp.GetLength() - strlen(__SERVERNAME__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUsername = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPassword = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUser = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPass = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else if(strTemp.Find(__GROUPID__, 0) == 0)
		{
			nGID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__GROUPID__)));
		}
		else if(strTemp.Find(__MONITORID__, 0) == 0)
		{
			nMID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		// Input Server Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_208%>"));//<%IDS_Monitor_208%>"ȱ��Websphere������ַ"
		return FALSE;
	}

	if(strServletUrl.IsEmpty())
	{
		// Input Servlet URL Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_209%>"));//<%IDS_Monitor_209%>"ȱ��Servlet URL"
		return FALSE;
	}

	if(nPort <= 0)
	{
		// Input Port Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_210%>"));//<%IDS_Monitor_210%>"����˿ڴ���"
		return FALSE;
	}

	if(strServerName.IsEmpty())
	{
		// Input ServerName Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_212%>"));//<%IDS_Monitor_210%>"����˿ڴ���"
		return FALSE;
	}

	if(!strProxyServer.IsEmpty() && nProxyPort <= 0)
	{
		// Input Proxy Server Port Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_211%>"));//<%IDS_Monitor_211%>"����������˿ڴ���"
		return FALSE;
	}

	return WEBSPHERE_MONITOR(strServer.GetBuffer(strServer.GetLength()), 
							strServletUrl.GetBuffer(strServletUrl.GetLength()), 
							nPort, 
							strServerName.GetBuffer(strServerName.GetLength()), 
							strUsername.GetBuffer(strUsername.GetLength()), 
							strPassword.GetBuffer(strPassword.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUser.GetBuffer(strProxyUser.GetLength()), 
							strProxyPass.GetBuffer(strProxyPass.GetLength()), 
							nGID, 
							nMID, 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);
}

/****************************************************************************
	Export Function Of VPN (Fetch VPNs)
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL VPNS(CStringList &paramList, CStringList &lstTexts, CStringList &lstValues)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    LPRASENTRYNAME lpRasEntry;
    LPRASENTRYNAME lpTemp;
    DWORD cbBuf;
    DWORD cEntry;
    DWORD dwRet;
    UINT  ndx;
    char  szMessage[256];

    cbBuf = sizeof(RASENTRYNAME);
    if ((lpRasEntry = ( LPRASENTRYNAME ) _fmalloc((UINT)cbBuf)) != NULL ) 
    {
        lpRasEntry->dwSize = sizeof(RASENTRYNAME);
        dwRet = RasEnumEntries( NULL, NULL, lpRasEntry, &cbBuf, &cEntry );
        if ( dwRet == ERROR_BUFFER_TOO_SMALL )
        {
            if ((lpTemp = ( LPRASENTRYNAME ) _frealloc ( lpRasEntry, (UINT)cbBuf )) != NULL )
            {
                lpRasEntry = lpTemp;
                dwRet = RasEnumEntries( NULL, NULL, lpRasEntry, &cbBuf, &cEntry );
            }
            else
            {
                dwRet = ERROR_NOT_ENOUGH_MEMORY;
            }
        }
        else if ( dwRet != 0 ) // other error
        {
            if ( RasGetErrorString( (UINT)dwRet, szMessage, 256 ) != 0 )
                wsprintf( (LPSTR)szMessage, "Undefined RasEnumEntries Error." );
        }

        if ( dwRet == 0 )  // No errors
        {
            for ( ndx = 0; ndx < cEntry; ndx++ ) 
			{
				lstTexts.AddTail((LPCSTR)lpRasEntry[ndx].szEntryName);
				lstValues.AddTail((LPCSTR)lpRasEntry[ndx].szEntryName);
			}
        }

        _ffree( lpRasEntry );
    } 

	return TRUE;
}

/****************************************************************************
	Export Function Of DatabaseSourceNames (Fetch DSN)
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL SYSTEMDSN(CStringList &paramList, CStringList &lstTexts, CStringList &lstValues)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HKEY hKey = NULL;
	LONG lRet = NULL;	
	lRet = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\ODBC\\ODBC.INI\\ODBC Data Sources", 0, KEY_QUERY_VALUE , &hKey);
	if (lRet == ERROR_SUCCESS) {
		// TODO: 
		DWORD dwIndex = 0;
		while (1) {
			char szValueName[512] = {0};
			DWORD dwSizeValue = sizeof(szValueName) - 1;
			
			lRet = RegEnumValue(hKey, dwIndex, szValueName, &dwSizeValue, NULL, NULL, NULL, NULL);				
			if (lRet != ERROR_SUCCESS)
				break;

			lstTexts.AddTail(szValueName);
			lstValues.AddTail(szValueName);
			dwIndex++;
		}
		RegCloseKey(hKey);
	}

	return TRUE;
	
}


extern "C" __declspec(dllexport) 
BOOL DisplayAgentList(CStringList &paramList,CStringList &listTexts,CStringList &listValues)
{
	/*
	CString strAgentFile = _T(""),
			strValidIds = _T("");
	char	szValidIdsBuf[1024] = {0},
			szIpBuf[512] = {0};
	listTexts.AddTail("����");
	listValues.AddTail("localhost");
	strAgentFile.Format("%s\\groups\\agents.config",FuncGetInstallPath());
	GetPrivateProfileString("information","_validids","",szValidIdsBuf,1024,strAgentFile);
	strValidIds.Format("%s",szValidIdsBuf);
	int iMarkCount = 0;
	iMarkCount = FuncGetCountByMark(strValidIds,"*");
	if(iMarkCount <= 0)
		return FALSE;
	for(int i = 1;i <= iMarkCount;i++)
	{
		CString strId = _T(""),
				strSection = _T(""),
				strTitle = _T("");
		strId = FuncGetLeftStringByMark(strValidIds,"*",i);
		strSection.Format("agent%s",strId);
		GetPrivateProfileString(strSection,"_title","",szIpBuf,512,strAgentFile);
		strTitle.Format("%s",szIpBuf);
		listTexts.AddTail(strTitle);
		listValues.AddTail(strId);
		memset(szIpBuf,0,512);
	}
*/
	return TRUE;
}

void DebugInfo(const char * szMsg)
{
	FILE * fp = NULL;
	fp = fopen("dll.debug", "a+");
	if (fp) {
		fputs(szMsg, fp);
		fputs("\r\n", fp);
		fclose(fp);
	}
}

BOOL isRed(int Groupid,int monitorid)
{
	CString strSection=_T(""),
		strKey=_T(""),
		strDynFileName=_T("");
	strSection.Format("monitor%d",monitorid);
	strKey="_category";
	strDynFileName.Format("%s\\groups\\%d.dyn",::FuncGetInstallPath(),Groupid);

	int st=::FuncGetProfileIntBy(strSection,strKey,strDynFileName);

	return (st==3) ? TRUE:FALSE;
}
/*
BOOL TELLOGFILE2(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T("");
	int			nPort = 23, 
				nAppendMethod = 1;

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strLogFile = _T(""), 
				strcMatch = _T(""), 
				strRuleFile = _T("");

	int			nGID = 0, 
				nMID = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__LOGFILE__, 0) == 0)
		{
			strLogFile = strTemp.Right(strTemp.GetLength() - strlen(__LOGFILE__));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strcMatch = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__RULEFILE__, 0) == 0)
		{
			strRuleFile = strTemp.Right(strTemp.GetLength() - strlen(__RULEFILE__));
		}
		else if(strTemp.Find(__GROUPID__, 0) == 0)
		{
			nGID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__GROUPID__)));
		}
		else if(strTemp.Find(__MONITORID__, 0) == 0)
		{
			nMID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__)));
		}
		else if(strTemp.Find(__APPENDMETHOD__, 0) == 0)
		{
			nAppendMethod = atoi(strTemp.Right(strTemp.GetLength() - strlen(__APPENDMETHOD__)));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_73%>"));//<%IDS_Monitor_73%>"ȱ�ٷ�����"
		return FALSE;
	}

	if(nPort <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_74%>"));//<%IDS_Monitor_74%>"����˿ڴ���"
		return FALSE;
	}

	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_75%>"));//<%IDS_Monitor_75%>"ȱ���û���"
		return FALSE;
	}

	if(strLogFile.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_214%>"));//<%IDS_Monitor_76%>"�������������"
		return FALSE;
	}

	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_77%>"));//<%IDS_Monitor_77%>"�����¼��ʾ����������ʾ������ʾ��"
		return FALSE;
	}

	if(isRed(nGID,nMID))
	{
		//sprintf(szReturn,"error=���д�����!$"/*"ErrorCount=0$TotalCount=0$");
		return FALSE;
	}
	
	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_78%>"));//<%IDS_Monitor_78%>"ͨ�ų�ʼ������"
		return FALSE;
	}


	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	BOOL ret = LOGFILE_TEL_VALUE2(strServer.GetBuffer(strServer.GetLength()), 
								nPort, 
								strUID.GetBuffer(strUID.GetLength()), 
								strPWD.GetBuffer(strPWD.GetLength()), 
								strProxyServer.GetBuffer(strProxyServer.GetLength()), 
								nProxyPort, 
								strProxyUid.GetBuffer(strProxyUid.GetLength()), 
								strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
								strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
								strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
								strPrompt.GetBuffer(strPrompt.GetLength()), 
								strIniFile.GetBuffer(strIniFile.GetLength()), 
								strLogFile.GetBuffer(strLogFile.GetLength()), 
								strcMatch.GetBuffer(strcMatch.GetLength()), 
								strRuleFile.GetBuffer(strRuleFile.GetLength()), 
								nAppendMethod, 
								nGID, 
								nMID, 
								strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
								szReturn);

	WSA_Free();

	return ret;
}*/

/*
extern "C" __declspec(dllexport) 
BOOL UNIXLOGCHECK(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int nProtoType = 0;
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROTOCOLTYPE__, 0) == 0)
		{
			nProtoType =atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOLTYPE__)));
			break;
		}
	}

	if(nProtoType == 0) nProtoType = 2;

	switch(nProtoType)
	{
	case 1:
		return SSHLOGFILE(paramList, szReturn);
	case 2:
		return TELLOGFILE2(paramList, szReturn);
	case 3:
		return HTTPLOGFILE(paramList, szReturn);
	case 4:
		return RLOGINLOGFILE(paramList, szReturn);
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_84%>"));//<%IDS_Monitor_84%>"Э�����ʹ���"
		return FALSE;
	}
	return TRUE;
}
*/
BOOL TELSCRIPT2(CStringList &paramList, char *szReturn)
{
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T(""), 
				strScript = _T(""), 
				strParam = _T(""), 
				strcMatch = _T("");
	int			nPort = 23; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__SCRIPT__, 0) == 0)
		{
			strScript = strTemp.Right(strTemp.GetLength() - strlen(__SCRIPT__));
		}
		else if(strTemp.Find(__PARAMETERS__, 0) == 0)
		{
			strParam = strTemp.Right(strTemp.GetLength() - strlen(__PARAMETERS__));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strcMatch = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_173%>"));//<%IDS_Monitor_173%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_174%>"));//<%IDS_Monitor_174%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_175%>"));//<%IDS_Monitor_175%>"ȱ���û���"
		return FALSE;
	}
	if(strScript.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_176%>"));//<%IDS_Monitor_176%>"ȱ�ٽű��ļ�����"
		return FALSE;
	}
	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_177%>"));//<%IDS_Monitor_177%>"�����¼��ʾ����������ʾ������ʾ��"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_178%>"));//<%IDS_Monitor_178%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	BOOL ret = TELEXESCRIPT2(strServer.GetBuffer(strServer.GetLength()), 
							nPort, 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
							strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
							strPrompt.GetBuffer(strPrompt.GetLength()), 
							strIniFile.GetBuffer(strIniFile.GetLength()), 
							strScript.GetBuffer(strScript.GetLength()), 
							strParam.GetBuffer(strParam.GetLength()), 
							strcMatch.GetBuffer(strcMatch.GetLength()), 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);

	WSA_Free();

	return ret;
}
extern "C" __declspec(dllexport) 
BOOL SCRIPT2(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int nProtoType = 0;
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__PROTOCOLTYPE__, 0) == 0)
		{
			nProtoType =atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOLTYPE__)));
			break;
		}
	}

	if(nProtoType == 0) nProtoType = 2;

	switch(nProtoType)
	{
	case 1:
		return SSHSCRIPT(paramList, szReturn);
	case 2:
		return TELSCRIPT2(paramList, szReturn);
	case 3:
		return HTTPSCRIPT(paramList, szReturn);
	case 4:
		return RLOGINSCRIPT(paramList, szReturn);
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_184%>"));//<%IDS_Monitor_184%>"Э�����ʹ���"
		return FALSE;
	}

	return TRUE;


}



extern "C" __declspec(dllexport) 
BOOL AixDiskAct(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"), 
				strIniFile = _T(""), 
				strDisk = _T("");
	int			nPort = 23; 

	CString		strProxyServer = _T(""), 
				strProxyUid = _T(""), 
				strProxyPwd = _T("");
	int			nProxyPort = 0;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__OSTYPE__, 0) == 0)
		{
			strIniFile = strTemp.Right(strTemp.GetLength() - strlen(__OSTYPE__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUid = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPwd = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__DISK__, 0) == 0)
		{
			strDisk = strTemp.Right(strTemp.GetLength() - strlen(__DISK__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_149%>"));//<%IDS_Monitor_149%>"ȱ�ٷ�����"
		return FALSE;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_150%>"));//<%IDS_Monitor_150%>"����˿ڴ���"
		return FALSE;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_151%>"));//<%IDS_Monitor_151%>"ȱ���û���"
		return FALSE;
	}
	if(strDisk.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_152%>"));//<%IDS_Monitor_152%>"ȱ�ٴ�������"
		return FALSE;
	}
	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_153%>"));//<%IDS_Monitor_153%>"�����¼��ʾ����������ʾ������ʾ��"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_154%>"));//<%IDS_Monitor_153%>"ͨ�ų�ʼ������"
		return FALSE;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	BOOL ret = MonitorValue(strServer.GetBuffer(strServer.GetLength()), 
							nPort, 
							strUID.GetBuffer(strUID.GetLength()), 
							strPWD.GetBuffer(strPWD.GetLength()), 
							strProxyServer.GetBuffer(strProxyServer.GetLength()), 
							nProxyPort, 
							strProxyUid.GetBuffer(strProxyUid.GetLength()), 
							strProxyPwd.GetBuffer(strProxyPwd.GetLength()), 
							strLogPrompt.GetBuffer(strLogPrompt.GetLength()), 
							strPwdPrompt.GetBuffer(strPwdPrompt.GetLength()), 
							strPrompt.GetBuffer(strPrompt.GetLength()), 
							strIniFile.GetBuffer(strIniFile.GetLength()), 
							DISK_ACTIVITY, 
							strDisk.GetBuffer(strDisk.GetLength()), 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);

	WSA_Free();

	return ret;
}