// db2mfcmonitor.h : db2mfcmonitor DLL ����ͷ�ļ�
//

#if !defined(AFX_DB2MFCMONITOR_H__85A6E73E_147D_4A9A_AFDB_C79267647FD8__INCLUDED_)
#define AFX_DB2MFCMONITOR_H__85A6E73E_147D_4A9A_AFDB_C79267647FD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// Cdb2mfcmonitorApp
// �йش���ʵ�ֵ���Ϣ������� db2mfcmonitor.cpp
//

class Cdb2mfcmonitorApp : public CWinApp
{
public:
	Cdb2mfcmonitorApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DB2MFCMONITOR_H__85A6E73E_147D_4A9A_AFDB_C79267647FD8__INCLUDED_)