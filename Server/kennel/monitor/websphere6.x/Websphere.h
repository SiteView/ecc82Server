// Websphere.h : Websphere DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CWebsphereApp
// �йش���ʵ�ֵ���Ϣ������� Websphere.cpp
//

class CWebsphereApp : public CWinApp
{
public:
	CWebsphereApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
