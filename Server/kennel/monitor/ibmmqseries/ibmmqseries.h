// ibmmqseries.h : ibmmqseries DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CibmmqseriesApp
// �йش���ʵ�ֵ���Ϣ������� ibmmqseries.cpp
//

class CibmmqseriesApp : public CWinApp
{
public:
	CibmmqseriesApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
