// db2_7.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"

#include "db2_7.h"
#include "tablespace.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//	注意！
//
//		如果此 DLL 动态链接到 MFC
//		DLL，从此 DLL 导出并
//		调入 MFC 的任何函数在函数的最前面
//		都必须添加 AFX_MANAGE_STATE 宏。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// Cdb2_7App

BEGIN_MESSAGE_MAP(Cdb2_7App, CWinApp)
END_MESSAGE_MAP()


// Cdb2_7App 构造

Cdb2_7App::Cdb2_7App()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 Cdb2_7App 对象

Cdb2_7App theApp;


// Cdb2_7App 初始化

BOOL Cdb2_7App::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
int main(int argc , char * argv[])
{
	try
	{
		char strParam[1024] = {0};
		char * szReturn;
		int nSize = 10240;
		sprintf(strParam , "_Db2DatabaseName=%s$_UserAccount=%s$_PassWord=%s$" , "db2" , "wlf" , "test");
		char *p = strParam;
		while(*p)
		{
			if(*p == '$')
			{
				*p='\0';
			}
			p++;
		}
		Call_GetTableSpace(strParam , szReturn , nSize);
	}
	catch(...)
	{
		DWORD dwError = GetLastError();
		LPSTR lpBuffer = NULL;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			0,
			(LPSTR)&lpBuffer,
			0,
			NULL
			);
		AfxMessageBox(lpBuffer);
		LocalFree(lpBuffer);
		return false;
	}
	return 0;
}