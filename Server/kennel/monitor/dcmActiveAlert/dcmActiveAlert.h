// dcmActiveAlert.h : dcmActiveAlert DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CdcmActiveAlertApp
// �йش���ʵ�ֵ���Ϣ������� dcmActiveAlert.cpp
//

class CdcmActiveAlertApp : public CWinApp
{
public:
	CdcmActiveAlertApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
