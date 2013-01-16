// FtpMonitor.h : main header file for the FTPMONITOR DLL
//

#if !defined(AFX_FTPMONITOR_H__2CD9C65F_AC3C_44A0_A410_4607AB3F99F1__INCLUDED_)
#define AFX_FTPMONITOR_H__2CD9C65F_AC3C_44A0_A410_4607AB3F99F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CFtpMonitorApp
// See FtpMonitor.cpp for the implementation of this class
//

#define		__USERACCOUNT__					"_UserAccount="
#define		__PASSWORD__					"_PassWord="

#define		__FTPUSERACCOUNT__				"_FTPUserAccount="
#define		__FTPPASSWORD__					"_FTPPassWord="
#define		__PROTOCOLTYPE__				"_ProtocolType="
#define		__SERVERPORT__					"_ServerPort="
#define		__DOWNLOADFILE__				"_DownLoadFile="
#define		__TIMEOUT__						"_TimeOut="
#define     __IPADDRESS__                   "IPAddress="
#define     __MACHINENAME__                 "_MachineName="
#define		__PORT__						"_Port="
#define		__PROXYSERVERPORT__				"_ProxyServerPort="
#define		__PROXYUSERNAME__				"_ProxyUser="
#define		__PROXYPASSWORD__				"_ProxyPass="
#define		__PASSIVEMODE__					"_PassiveMode="
#define		__CUSTOMERPATH__				"_CustomerPath="
#define		__MATCHSTR__					"_MatchStr="
#define		__PROXYTYPE__					"_ProxyType="

class CFtpMonitorApp : public CWinApp
{
public:
	CFtpMonitorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFtpMonitorApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CFtpMonitorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FTPMONITOR_H__2CD9C65F_AC3C_44A0_A410_4607AB3F99F1__INCLUDED_)
