// VCDemo.h : main header file for the VCDEMO application
//

#if !defined(AFX_VCDEMO_H__296B952A_9D89_46EB_8288_0A00CC2D085B__INCLUDED_)
#define AFX_VCDEMO_H__296B952A_9D89_46EB_8288_0A00CC2D085B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVCDemoApp:
// See VCDemo.cpp for the implementation of this class
//

class CVCDemoApp : public CWinApp
{
public:
	CVCDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVCDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVCDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCDEMO_H__296B952A_9D89_46EB_8288_0A00CC2D085B__INCLUDED_)
