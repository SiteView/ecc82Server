// TuxedoMonitor.h : main header file for the TUXEDOMONITOR DLL
//

#if !defined(AFX_TUXEDOMONITOR_H__539FE098_588C_4A33_B999_49F3D5993601__INCLUDED_)
#define AFX_TUXEDOMONITOR_H__539FE098_588C_4A33_B999_49F3D5993601__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTuxedoMonitorApp
// See TuxedoMonitor.cpp for the implementation of this class
//

#define		__MACHINENAME__					"_MachineName="
#define		__USER__					"_UserAccount="
#define		__PASSWORD__					"_PassWord="

#define		__PORT__						"_Port="
#define		__NAMESPACE__					"_namespace="
#define		__PROTOCOL__					"_protocol="

#define		__TASKTYPE__					"_TaskType="


class CTuxedoMonitorApp : public CWinApp
{
public:
	CTuxedoMonitorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTuxedoMonitorApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTuxedoMonitorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TUXEDOMONITOR_H__539FE098_588C_4A33_B999_49F3D5993601__INCLUDED_)
