// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "VCDemo.h"
#include "DlgProxy.h"
#include "VCDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVCDemoDlgAutoProxy

IMPLEMENT_DYNCREATE(CVCDemoDlgAutoProxy, CCmdTarget)

CVCDemoDlgAutoProxy::CVCDemoDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT (AfxGetApp()->m_pMainWnd != NULL);
	ASSERT_VALID (AfxGetApp()->m_pMainWnd);
	ASSERT_KINDOF(CVCDemoDlg, AfxGetApp()->m_pMainWnd);
	m_pDialog = (CVCDemoDlg*) AfxGetApp()->m_pMainWnd;
	m_pDialog->m_pAutoProxy = this;
}

CVCDemoDlgAutoProxy::~CVCDemoDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CVCDemoDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CVCDemoDlgAutoProxy, CCmdTarget)
	//{{AFX_MSG_MAP(CVCDemoDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CVCDemoDlgAutoProxy, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CVCDemoDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IVCDemo to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {1CAF3AB5-0CB4-4396-AD3A-CAEC6E2D40A1}
static const IID IID_IVCDemo =
{ 0x1caf3ab5, 0xcb4, 0x4396, { 0xad, 0x3a, 0xca, 0xec, 0x6e, 0x2d, 0x40, 0xa1 } };

BEGIN_INTERFACE_MAP(CVCDemoDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CVCDemoDlgAutoProxy, IID_IVCDemo, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {D387088D-30DC-4107-9142-F5FAA3DCEEC0}
IMPLEMENT_OLECREATE2(CVCDemoDlgAutoProxy, "VCDemo.Application", 0xd387088d, 0x30dc, 0x4107, 0x91, 0x42, 0xf5, 0xfa, 0xa3, 0xdc, 0xee, 0xc0)

/////////////////////////////////////////////////////////////////////////////
// CVCDemoDlgAutoProxy message handlers
