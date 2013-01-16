// Lotus.h : main header file for the LOTUS DLL
//

#if !defined(AFX_LOTUS_H__CF364F2A_1A2E_42D8_B20C_1E9CCE68011B__INCLUDED_)
#define AFX_LOTUS_H__CF364F2A_1A2E_42D8_B20C_1E9CCE68011B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLotusApp
// See Lotus.cpp for the implementation of this class
//

class CLotusApp : public CWinApp
{
public:
	CLotusApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLotusApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CLotusApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOTUS_H__CF364F2A_1A2E_42D8_B20C_1E9CCE68011B__INCLUDED_)
