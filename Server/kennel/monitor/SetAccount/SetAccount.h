// SetAccount.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CSetAccountApp:
// �йش����ʵ�֣������ SetAccount.cpp
//

class CSetAccountApp : public CWinApp
{
public:
	CSetAccountApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSetAccountApp theApp;
