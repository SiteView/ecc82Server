// CtripURLMonitor.h : CtripURLMonitor DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CCtripURLMonitorApp
// �йش���ʵ�ֵ���Ϣ������� CtripURLMonitor.cpp
//

class CCtripURLMonitorApp : public CWinApp
{
public:
	CCtripURLMonitorApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
