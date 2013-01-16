// svdbToXMLDlg.h : 头文件
//

#pragma once
#include "config.h"
#include "svdbapi.h"

using namespace svutil;

// CsvdbToXMLDlg 对话框
class CsvdbToXMLDlg : public CDialog
{
// 构造
public:
	CsvdbToXMLDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SVDBTOXML_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	// 保存xml文件的路径
	CString m_path;
	// 间隔时间，单位为分钟
	unsigned int m_interval;
	// svdb所在服务器
	CString m_ip;
	CConfig m_config;
public:
	afx_msg void OnBnClickedButton1();
private:
	inline TTime readTime(void);
	inline bool writeTime(TTime const & t);
public:
	afx_msg void OnTimer(UINT nIDEvent);
private:
	void generateXml(TTime const & begTime, TTime const & endTime, string const & address);
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
