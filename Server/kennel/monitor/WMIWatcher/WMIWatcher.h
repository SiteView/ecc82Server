// WMIWatcher.h : WMIWatcher DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CWMIWatcherApp
// �йش���ʵ�ֵ���Ϣ������� WMIWatcher.cpp
//

class CWMIWatcherApp : public CWinApp
{
public:
	CWMIWatcherApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
