// SocketURLMonitor.h : SocketURLMonitor DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CSocketURLMonitorApp
// �йش���ʵ�ֵ���Ϣ������� SocketURLMonitor.cpp
//

class CSocketURLMonitorApp : public CWinApp
{
public:
	CSocketURLMonitorApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
