// HttpsMonitor.h : main header file for the HTTPSMONITOR DLL
//

#if !defined(AFX_HTTPSMONITOR_H__A5EB115F_C2CB_4D69_9E23_579C057AC553__INCLUDED_)
#define AFX_HTTPSMONITOR_H__A5EB115F_C2CB_4D69_9E23_579C057AC553__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHttpsMonitorApp
// See HttpsMonitor.cpp for the implementation of this class
//

class CHttpsMonitorApp : public CWinApp
{
public:
	CHttpsMonitorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHttpsMonitorApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CHttpsMonitorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTTPSMONITOR_H__A5EB115F_C2CB_4D69_9E23_579C057AC553__INCLUDED_)
