// svdbToXMLDlg.h : ͷ�ļ�
//

#pragma once
#include "config.h"
#include "svdbapi.h"

using namespace svutil;

// CsvdbToXMLDlg �Ի���
class CsvdbToXMLDlg : public CDialog
{
// ����
public:
	CsvdbToXMLDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SVDBTOXML_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	// ����xml�ļ���·��
	CString m_path;
	// ���ʱ�䣬��λΪ����
	unsigned int m_interval;
	// svdb���ڷ�����
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
