// TuxedoMonitor.h : TuxedoMonitor DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CTuxedoMonitorApp
// �йش���ʵ�ֵ���Ϣ������� TuxedoMonitor.cpp
//

class CTuxedoMonitorApp : public CWinApp
{
public:
	CTuxedoMonitorApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};