// GenLicDlg.h : 头文件
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



// CGenLicDlg 对话框
class CGenLicDlg : public CDialog
{
// 构造
public:
	CGenLicDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GENLIC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CCheckListBox m_CheckList;

	// 生成的消息映射函数
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
	// 获取设置的数据
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
