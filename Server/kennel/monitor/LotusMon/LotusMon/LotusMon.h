// LotusMon.h : LotusMon DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CLotusMonApp
// �йش���ʵ�ֵ���Ϣ������� LotusMon.cpp
//

class CLotusMonApp : public CWinApp
{
public:
	CLotusMonApp();

	
// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


