// VCDemoDlg.h : header file
//

#if !defined(AFX_VCDEMODLG_H__FF2BFAE1_5A8B_425D_9F3B_67723FA85ABE__INCLUDED_)
#define AFX_VCDEMODLG_H__FF2BFAE1_5A8B_425D_9F3B_67723FA85ABE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CVCDemoDlgAutoProxy;

/////////////////////////////////////////////////////////////////////////////
// CVCDemoDlg dialog

class CVCDemoDlg : public CDialog
{
	DECLARE_DYNAMIC(CVCDemoDlg);
	friend class CVCDemoDlgAutoProxy;

// Construction
public:
	CVCDemoDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CVCDemoDlg();

// Dialog Data
	//{{AFX_DATA(CVCDemoDlg)
	enum { IDD = IDD_VCDEMO_DIALOG };
	CEdit	m_edit_url;
	CString	m_ip;
	CString	m_apiCode;
	CString	m_username;
	CString	m_pwd;
	CString	m_mobile;
	CString	m_strContent;
	CString m_id;
	CString	m_url;
	int		m_wap_push;
	CString	m_strDBName;
	CString	m_sendTime;
	CString	m_srcid;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVCDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CVCDemoDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL m_bInit;
	BOOL CanExit();

	// Generated message map functions
	//{{AFX_MSG(CVCDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSend();
	afx_msg void OnReceiveSM();
	afx_msg void OnReceiveRPT();
	afx_msg void OnExit();
	afx_msg void OnRelease();
	afx_msg void OnInit();
	afx_msg void OnRadioIsWappush();
	afx_msg void OnRadioNoWappush();
	afx_msg void OnRadioPDU();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VCDEMODLG_H__FF2BFAE1_5A8B_425D_9F3B_67723FA85ABE__INCLUDED_)
