// SvExcelPlugin.h : SvExcelPlugin DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CSvExcelPluginApp
// �йش���ʵ�ֵ���Ϣ������� SvExcelPlugin.cpp
//

class CSvExcelPluginApp : public CWinApp
{
public:
	CSvExcelPluginApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
