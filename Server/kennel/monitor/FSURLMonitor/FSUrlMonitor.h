// FSUrlMonitor.h : FSUrlMonitor DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CFSUrlMonitorApp
// �йش���ʵ�ֵ���Ϣ������� FSUrlMonitor.cpp
//

class CFSUrlMonitorApp : public CWinApp
{
public:
	CFSUrlMonitorApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
