// WMIWatcherWH.h : WMIWatcherWH DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CWMIWatcherWHApp
// �йش���ʵ�ֵ���Ϣ������� WMIWatcherWH.cpp
//

class CWMIWatcherWHApp : public CWinApp
{
public:
	CWMIWatcherWHApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
