// sybaseMonitor2.h : sybaseMonitor2 DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CsybaseMonitor2App
// �йش���ʵ�ֵ���Ϣ������� sybaseMonitor2.cpp
//

class CsybaseMonitor2App : public CWinApp
{
public:
	CsybaseMonitor2App();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
