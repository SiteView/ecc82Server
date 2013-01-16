// MySQLMonitor.h : main header file for the MYSQLMONITOR DLL
//

#if !defined(AFX_MYSQLMONITOR_H__71B30B32_A213_4022_9B3E_66433D34A00C__INCLUDED_)
#define AFX_MYSQLMONITOR_H__71B30B32_A213_4022_9B3E_66433D34A00C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMySQLMonitorApp
// See MySQLMonitor.cpp for the implementation of this class
//

class CMySQLMonitorApp : public CWinApp
{
public:
	CMySQLMonitorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMySQLMonitorApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMySQLMonitorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern "C" __declspec(dllexport)
BOOL MySQL(const char * strParas, char * szReturn, int& nSize);

/*****************************************************************/

void Parser(const char* szResult, const char *chHost, char *szResturn);

/*****************************************************************/

BOOL IsLocalHost(CString sHostName);

/*****************************************************************/

extern "C" __declspec(dllexport)
BOOL MySqlMem(const char * strParas, char * szReturn, int& nSize);

/*****************************************************************
       MySQL½ø³Ì¼à²âÆ÷
*****************************************************************/
extern "C" __declspec(dllexport)
BOOL ProcessList(const char * strParas, char * szReturn, int &nSize);

/*****************************************************************
       MySQLÍ¬²½×´Ì¬¼à²âÆ÷
*****************************************************************/
extern "C" __declspec(dllexport)
BOOL SlaveStatus(const char * strParas, char * szReturn, int &nSize);

/*****************************************************************
       MySQL·þÎñÆ÷×´Ì¬¼à²âÆ÷
*****************************************************************/
extern "C" __declspec(dllexport)
BOOL Status(const char * strParas, char * szReturn, int &nSize);

#define		__MACHINENAME__					"_MachineName="
#define		__PORT__						"_MySQLPort="
#define		__USERACCOUNT__					"_UserAccount="
#define		__PASSWORD__					"_PassWord="
#define		__WINUSERACCOUNT__				"_WinUserAccount="
#define		__WINPASSWORD__					"_WinPassWord="
#define     __DBNAME__                      "_DBName="
#define     __MONITORID__                   "_MonitorID="
#define		__PROID__						"_ProID="

#define     MAX_BUFF_LEN    256
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSQLMONITOR_H__71B30B32_A213_4022_9B3E_66433D34A00C__INCLUDED_)
