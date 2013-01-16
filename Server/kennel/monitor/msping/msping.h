// msping.h : main header file for the MSPING DLL
//

#if !defined(AFX_MSPING_H__B500B85E_7673_48F0_8273_F0F0116B3060__INCLUDED_)
#define AFX_MSPING_H__B500B85E_7673_48F0_8273_F0F0116B3060__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMspingApp
// See msping.cpp for the implementation of this class
//

class CMspingApp : public CWinApp
{
public:
	CMspingApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMspingApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CMspingApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSPING_H__B500B85E_7673_48F0_8273_F0F0116B3060__INCLUDED_)
