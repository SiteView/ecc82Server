// LinuxStation.h : LinuxStation DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CLinuxStationApp
// �йش���ʵ�ֵ���Ϣ������� LinuxStation.cpp
//

class CLinuxStationApp : public CWinApp
{
public:
	CLinuxStationApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
