// SybaseMonitor.h : main header file for the SYBASEMONITOR DLL
//

#if !defined(AFX_SYBASEMONITOR_H__2BAEC7CD_8041_4D64_B2F7_C76E4D78D099__INCLUDED_)
#define AFX_SYBASEMONITOR_H__2BAEC7CD_8041_4D64_B2F7_C76E4D78D099__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSybaseMonitorApp
// See SybaseMonitor.cpp for the implementation of this class
//

class CSybaseMonitorApp : public CWinApp
{
public:
	CSybaseMonitorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSybaseMonitorApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSybaseMonitorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYBASEMONITOR_H__2BAEC7CD_8041_4D64_B2F7_C76E4D78D099__INCLUDED_)
