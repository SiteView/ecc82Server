// db2_7.h : db2_7 DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// Cdb2_7App
// �йش���ʵ�ֵ���Ϣ������� db2_7.cpp
//

class Cdb2_7App : public CWinApp
{
public:
	Cdb2_7App();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
