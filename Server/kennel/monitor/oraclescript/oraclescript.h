// oraclescript.h : oraclescript DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CoraclescriptApp
// �йش���ʵ�ֵ���Ϣ������� oraclescript.cpp
//

class CoraclescriptApp : public CWinApp
{
public:
	CoraclescriptApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
