// Exchange2003.h : main header file for the EXCHANGE2003 DLL
//

#if !defined(AFX_EXCHANGE2003_H__242833A5_BFEE_4D42_AE47_88B0286C00BC__INCLUDED_)
#define AFX_EXCHANGE2003_H__242833A5_BFEE_4D42_AE47_88B0286C00BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CExchange2003App
// See Exchange2003.cpp for the implementation of this class
//

class CExchange2003App : public CWinApp
{
public:
	CExchange2003App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExchange2003App)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CExchange2003App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXCHANGE2003_H__242833A5_BFEE_4D42_AE47_88B0286C00BC__INCLUDED_)
