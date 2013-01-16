#if !defined(AFX_IEEVENTS_H__A7EC456B_EE52_4EC2_B1F2_BD73532BCCB1__INCLUDED_)
#define AFX_IEEVENTS_H__A7EC456B_EE52_4EC2_B1F2_BD73532BCCB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IEEvents.h : header file
//

#include <afxconv.h>
#include <ExDispID.h>   // For WebBrowser and Internet Explorer events.
#include <mshtml.h>
#include <afxctl.h>
#include <Shlwapi.h>

/////////////////////////////////////////////////////////////////////////////
// CIEEvents command target

class CIEEvents : public CCmdTarget
{
	DECLARE_DYNCREATE(CIEEvents)

	CIEEvents();           // protected constructor used by dynamic creation
	CIEEvents(int nID, char *szReturn, CString strMatchContent);
	virtual ~CIEEvents();

// Attributes
public:
	IWebBrowser2*	m_pIE;
	DWORD			m_dwCookie;
	int				m_nID;
	char			*m_szReturn;
	CString			m_strMatchContent;

// Operations
public:
	void OnBeforeNavigate2(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags,
						VARIANT* TargetFrameName, VARIANT* PostData,
                        VARIANT* Headers, BOOL* Cancel);
	void OnCommandStateChange(long lCommand, BOOL bEnable);
	void OnDocumentComplete(LPDISPATCH pDisp, VARIANT* URL);
	void OnDownloadComplete();
	void OnNavigateComplete2(LPDISPATCH pDisp, VARIANT* URL);
	void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel);
	void OnProgressChange(long lProgress, long lProgressMax);
	void OnStatusTextChange(LPCTSTR lpszText);
	void OnTitleChange(LPCTSTR lpszText);
	void OnQuit();

	CString FuncGetInstallPath();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIEEvents)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIEEvents)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CIEEvents)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IEEVENTS_H__A7EC456B_EE52_4EC2_B1F2_BD73532BCCB1__INCLUDED_)
