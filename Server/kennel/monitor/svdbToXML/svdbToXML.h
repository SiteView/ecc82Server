// svdbToXML.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "resource.h"		// ������


// CsvdbToXMLApp:
// �йش����ʵ�֣������ svdbToXML.cpp
//

class CsvdbToXMLApp : public CWinApp
{
public:
	CsvdbToXMLApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CsvdbToXMLApp theApp;
