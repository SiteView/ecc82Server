// WinDelayTime.h : WinDelayTime DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CWinDelayTimeApp
// �йش���ʵ�ֵ���Ϣ������� WinDelayTime.cpp
//

class CWinDelayTimeApp : public CWinApp
{
public:
	CWinDelayTimeApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
