// TuxedoMonitor.h : TuxedoMonitor DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// 主符号


// CTuxedoMonitorApp
// 有关此类实现的信息，请参阅 TuxedoMonitor.cpp
//

class CTuxedoMonitorApp : public CWinApp
{
public:
	CTuxedoMonitorApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
