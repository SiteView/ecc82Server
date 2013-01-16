// sybaseMonitor2.h : sybaseMonitor2 DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// 主符号


// CsybaseMonitor2App
// 有关此类实现的信息，请参阅 sybaseMonitor2.cpp
//

class CsybaseMonitor2App : public CWinApp
{
public:
	CsybaseMonitor2App();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
