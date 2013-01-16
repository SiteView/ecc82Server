// SQLServer.h : main header file for the SQLSERVER DLL
//

#if !defined(AFX_SQLSERVER_H__DB58A0EE_764E_4FA5_898D_EBA5A8A1ECBC__INCLUDED_)
#define AFX_SQLSERVER_H__DB58A0EE_764E_4FA5_898D_EBA5A8A1ECBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSQLServerApp
// See SQLServer.cpp for the implementation of this class
//

class CSQLServerApp : public CWinApp
{
public:
	CSQLServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSQLServerApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSQLServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport) 
BOOL SYSTEMDSN(const char * strParas, char * strReturn, int& nSize, char *strFlag = NULL);

extern "C" __declspec(dllexport)
BOOL SQLBufferMonitr(const char * strParas, char * szReturn, int& nSize);
extern "C" __declspec(dllexport)
BOOL SQLMemoryMonitr(const char * strParas, char * szReturn, int& nSize);
extern "C" __declspec(dllexport)
BOOL SQLGeneralStatisticsMonitr(const char * strParas, char * szReturn, int& nSize);
extern "C" __declspec(dllexport)
BOOL SQLCacheMonitr(const char * strParas, char * szReturn, int& nSize);
extern "C" __declspec(dllexport)
BOOL SQLStatisticsMonitr(const char * strParas, char * szReturn, int& nSize);
BOOL SQLMonitr(CStringList& paramList, char *szReturn,int tplid );

void WriteLogFile(char* chMsg);
/////////////////////////////////////////////////////////////////////////////
#define     __MACHINENAME__                 "_DsnName="
#define		__USERACCOUNT__					"_UserAccount="
#define		__PASSWORD__					"_PassWord="
#define		__SQL__							"_SQL="
#define     __GROUPID__						"_GroupID"
#define     __MID__							"_MonitorID"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SQLSERVER_H__DB58A0EE_764E_4FA5_898D_EBA5A8A1ECBC__INCLUDED_)
