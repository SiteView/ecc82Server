// NTPerfTest.h : main header file for the NTPERFTEST DLL
//

#if !defined(AFX_NTPERFTEST_H__F4219F01_1597_4882_9BC6_F5993CA99B74__INCLUDED_)
#define AFX_NTPERFTEST_H__F4219F01_1597_4882_9BC6_F5993CA99B74__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CNTPerfTestApp
// See NTPerfTest.cpp for the implementation of this class
//

class CNTPerfTestApp : public CWinApp
{
public:
	CNTPerfTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNTPerfTestApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CNTPerfTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NTPERFTEST_H__F4219F01_1597_4882_9BC6_F5993CA99B74__INCLUDED_)
