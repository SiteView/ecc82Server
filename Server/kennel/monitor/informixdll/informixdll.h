// InformixDll.h : main header file for the INFORMIXDLL DLL
//

#if !defined(AFX_INFORMIXDLL_H__745E3185_6F96_4729_84F4_2EA20338C822__INCLUDED_)
#define AFX_INFORMIXDLL_H__745E3185_6F96_4729_84F4_2EA20338C822__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CInformixDllApp
// See InformixDll.cpp for the implementation of this class
//

class CInformixDllApp : public CWinApp
{
public:
	CInformixDllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInformixDllApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CInformixDllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFORMIXDLL_H__745E3185_6F96_4729_84F4_2EA20338C822__INCLUDED_)
