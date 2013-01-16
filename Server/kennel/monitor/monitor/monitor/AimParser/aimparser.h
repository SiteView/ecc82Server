// AimParser.h : main header file for the AIMPARSER DLL
//

#if !defined(AFX_AIMPARSER_H__6F488712_2190_45D2_A570_50B435DC05A1__INCLUDED_)
#define AFX_AIMPARSER_H__6F488712_2190_45D2_A570_50B435DC05A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAimParserApp
// See AimParser.cpp for the implementation of this class
//

class CAimParserApp : public CWinApp
{
public:
	CAimParserApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAimParserApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAimParserApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AIMPARSER_H__6F488712_2190_45D2_A570_50B435DC05A1__INCLUDED_)
