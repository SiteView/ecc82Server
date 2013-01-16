// Curl.h : main header file for the CURL DLL
//

#if !defined(AFX_CURL_H__E61B025E_1E48_4724_8491_DA9A2ED3035B__INCLUDED_)
#define AFX_CURL_H__E61B025E_1E48_4724_8491_DA9A2ED3035B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCurlApp
// See Curl.cpp for the implementation of this class
//

class CCurlApp : public CWinApp
{
public:
	CCurlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurlApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCurlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURL_H__E61B025E_1E48_4724_8491_DA9A2ED3035B__INCLUDED_)
