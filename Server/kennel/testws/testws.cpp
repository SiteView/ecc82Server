// testws.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "testws.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	//test
	int nRetCode = 0;

	// 初始化 MFC 并在失败时显示错误
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("致命错误: MFC 初始化失败\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: 在此处为应用程序的行为编写代码。
	}

	CString estr = "";
	HMODULE hDll = LoadLibrary(".\\MonitorScheduleWeb.dll");
	if( !hDll )
	{
		estr+= "  Failed to LoadLibrary: ";
		return false;
	}

	//typedef bool (*SendMsg)(const char *pszId, const char *pszText, const char *pszServer, const char *pszPort);
	typedef bool (*SendMsg)(string szId, string pszText, string pszServer, string pszPort, string &errorMsg);
	//typedef bool (*SendMsg)(string szId, string pszText, string pszServer, string pszPort);
	//typedef bool (*SendMsg)(char *, char *, char *, char *, char &);
	SendMsg sendmsg = (SendMsg)::GetProcAddress(hDll, "SendMsg");
	if( !sendmsg )
	{
		
		estr+= "  获取dll函数失败, Failed to GetProcAddress: SendMsg;  ";
		return false;
	}
	else
	{
		bool bret = true;
		char strInputData[256] = {0};
		sprintf(strInputData , "%s" , "eeeeee");
		string errorMsg = "";
		//bret = sendmsg("1.5.6", "ddddd#ddd#dfhfdsf#ggggggggggggg", "127.0.0.1", "8080", *strInputData);
		bret = sendmsg("1.5.6", "ddddd#ddd#dfhfdsf#ggggggggggggg", "127.0.0.1", "8080", errorMsg);
		//bret = sendmsg("1", "ddddd#ddd#dfhfdsf#ggggggggggggg", "127.0.0.1", "8080", *strInputData);
		//bret = sendmsg("xxx", "ddddd#ddd#dfhfdsf#ggggggggggggg", "dddddddddddddd", "dddd");

		if(bret)
		{
			_tprintf(_T("sendmsg sucess\n"));
		}
		else
		{
			_tprintf(_T("sendmsg failed\n"));
			_tprintf(errorMsg.c_str());
		}


	}
	return nRetCode;
}
