// GenLicDlg.h : ͷ�ļ�
//

#pragma once

#include <afxwin.h>
#include "afxcmn.h"

#include "afxdtctl.h"
#include "afxwin.h"

#include <ctype.h>

#include <string>
#include <vector>
#include <list>


using namespace std;

typedef basic_string<char>::size_type S_T;
static const S_T npos = -1;

vector<string> tokenize(const string& src, string tok, bool trim, string null_subst="");



// CGenLicDlg �Ի���
class CGenLicDlg : public CDialog
{
// ����
public:
	CGenLicDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_GENLIC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CCheckListBox m_CheckList;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRdEcc();
	afx_msg void OnBnClickedRdNnm();
	afx_msg void OnBnClickedRdDm();
	afx_msg void OnBnClickedChkSelectAll();
	afx_msg void OnBnClickedBtFiledialog();
	afx_msg void OnBnClickedOk();

public:
	CEdit m_EditPoints;
	CSpinButtonCtrl m_SpinCtrl_Points;
	CSpinButtonCtrl m_SpinCtrl_Devices;
	CSpinButtonCtrl m_SpinCtrl_DelayDays;
	CEdit m_EditDevices;
	CEdit m_EditDelayDays;

private:
	CString GetData(void);

private:
	int m_RdProduct;
	// ��ȡ���õ�����
	LICENSEDATA srcData;
	afx_msg void OnBnClickedBtGetoldlic();
	CDateTimeCtrl m_StartDate;
	CEdit m_EditUser;
	CEdit m_EditSupport;
	CButton m_CheckDefault;
	afx_msg void OnBnClickedBtTest();
	afx_msg void OnBnClickedBtInctime();
	afx_msg void OnBnClickedBtCpuid();
	afx_msg void OnBnClickedItsm();
public:
	CEdit m_EditPsw;
	CEdit m_MachineCode;
	CButton m_CheckBox;
	CButton m_FormalVer;
	afx_msg void OnBnClickedLog();
	afx_msg void OnBnClickedCancel();
};
