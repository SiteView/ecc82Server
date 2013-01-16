// db2_7.h : db2_7 DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// 主符号


// Cdb2_7App
// 有关此类实现的信息，请参阅 db2_7.cpp
//

class Cdb2_7App : public CWinApp
{
public:
	Cdb2_7App();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
