// testws.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "testws.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	//test
	int nRetCode = 0;

	// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: ���Ĵ�������Է���������Ҫ
		_tprintf(_T("��������: MFC ��ʼ��ʧ��\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: �ڴ˴�ΪӦ�ó������Ϊ��д���롣
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
		
		estr+= "  ��ȡdll����ʧ��, Failed to GetProcAddress: SendMsg;  ";
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
