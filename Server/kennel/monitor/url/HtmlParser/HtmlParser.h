// HtmlParser.h : main header file for the HTMLPARSER DLL
//

#if !defined(AFX_HTMLPARSER_H__4D962236_A9D4_4940_8BA3_1F3442F9B072__INCLUDED_)
#define AFX_HTMLPARSER_H__4D962236_A9D4_4940_8BA3_1F3442F9B072__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHtmlParserApp
// See HtmlParser.cpp for the implementation of this class
//

class CHtmlParserApp : public CWinApp
{
public:
	CHtmlParserApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHtmlParserApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CHtmlParserApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HTMLPARSER_H__4D962236_A9D4_4940_8BA3_1F3442F9B072__INCLUDED_)
