// DlgProxy.h : header file
//

#if !defined(AFX_DLGPROXY_H__9D9BB7A4_21CB_401E_87B8_0F777AE726B0__INCLUDED_)
#define AFX_DLGPROXY_H__9D9BB7A4_21CB_401E_87B8_0F777AE726B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVCDemoDlg;

/////////////////////////////////////////////////////////////////////////////
// CVCDemoDlgAutoProxy command target

class CVCDemoDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CVCDemoDlgAutoProxy)

	CVCDemoDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CVCDemoDlg* m_pDialog;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVCDemoDlgAutoProxy)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CVCDemoDlgAutoProxy();

	// Generated message map functions
	//{{AFX_MSG(CVCDemoDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CVCDemoDlgAutoProxy)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CVCDemoDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROXY_H__9D9BB7A4_21CB_401E_87B8_0F777AE726B0__INCLUDED_)
