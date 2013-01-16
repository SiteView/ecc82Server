// Tomcat.h : main header file for the TOMCAT DLL
//

#if !defined(AFX_TOMCAT_H__F95CC5A9_2564_4831_8474_C3466730E8CC__INCLUDED_)
#define AFX_TOMCAT_H__F95CC5A9_2564_4831_8474_C3466730E8CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTomcatApp
// See Tomcat.cpp for the implementation of this class
//

class CTomcatApp : public CWinApp
{
public:
	CTomcatApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTomcatApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CTomcatApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOMCAT_H__F95CC5A9_2564_4831_8474_C3466730E8CC__INCLUDED_)
