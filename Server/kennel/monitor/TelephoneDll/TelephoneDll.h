// TelephoneDll.h : TelephoneDll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CTelephoneDllApp
// �йش���ʵ�ֵ���Ϣ������� TelephoneDll.cpp
//

class CTelephoneDllApp : public CWinApp
{
public:
	CTelephoneDllApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
