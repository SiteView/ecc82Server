// SmsSend(USMP).h : SmsSend(USMP) DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// ������


// CSmsSendUSMPApp
// �йش���ʵ�ֵ���Ϣ������� SmsSend(USMP).cpp
//

class CSmsSendUSMPApp : public CWinApp
{
public:
	CSmsSendUSMPApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
