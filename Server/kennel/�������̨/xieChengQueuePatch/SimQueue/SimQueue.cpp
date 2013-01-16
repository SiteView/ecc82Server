// ServiceTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "stdio.h"
#include "tchar.h"
#include <windows.h>
#include <iostream>


#include <atlstr.h> //CString
#include <atltime.h>// CTime
#include <sys/stat.h>   //struct _stat();

using namespace std;

#include "svapi.h"
#pragma comment(lib, "svapi.lib")

#include "scasvapi.h"
#pragma comment(lib, "scasvapi.lib")



//typedef int(*lpSokectServer)();
std::string TrimSpace(const std::string & input)
{
	string ret=input;
	try{
		while( ret.find(" ")==0 )
		{
			ret.erase(0,1);
		}
		if(ret.empty())
		{
			return ret;
		}
		std::string::size_type lastpos;
		while( ret.rfind(" ") == (lastpos=(ret.size()-1)) )
		{
			ret.erase(lastpos,1);
		}
	}
	catch(...)
	{
		ret= input;
	}
	return ret;
}

string GetValueInNodeData(const NodeData & inwhat,string key,string & estr)
{
	try{
		NodeData::const_iterator nit= inwhat.find(key);	
		string value;
		if(nit!=inwhat.end() && !(value=nit->second).empty() )
			return TrimSpace(value);

		estr+="   传入请求中没有 \"";
		estr+=key;
		estr+="\", 或其 value 为空.";
	}
	catch(...)
	{
		estr="Exception in GetValueInNodeData.";
	}
	return "";
}

//定义全局函数变量
void Init();
BOOL IsInstalled();
BOOL Install();
BOOL Uninstall();
void LogEvent(LPCTSTR pszFormat, ...);
void WINAPI ServiceMain();
void WINAPI ServiceStrl(DWORD dwOpcode);

TCHAR szServiceName[] = _T("SimQueue");
BOOL bInstall;
SERVICE_STATUS_HANDLE hServiceStatus;
SERVICE_STATUS status;
DWORD dwThreadID;

int mainRun(void);

void WriteLog( const char* str )
{
	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	string path = GetSiteViewRootPath();
	path += "\\temp\\SimQueue.log";

	// 判断文件大小：在不打开文件的情况下实现
	struct _stat buf;
	if( _stat( path.c_str(), &buf ) == 0 )
	{
		if( buf.st_size > 500*1024 )
		{
			FILE *log = fopen( path.c_str(), "w" );
			if( log != NULL )
				fclose( log );
		}
	}


	FILE *log = fopen( path.c_str(),"a+");
	if( log != NULL )
	{
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fclose( log );
	}

}
int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	Init();

	WriteLog("Service start....");

	dwThreadID = ::GetCurrentThreadId();

	SERVICE_TABLE_ENTRY st[] =
	{
		{ szServiceName, (LPSERVICE_MAIN_FUNCTION)ServiceMain },
		{ NULL, NULL }
	};

	if (stricmp(lpCmdLine, "/install") == 0)
	{
		Install();
	}
	else if (stricmp(lpCmdLine, "/uninstall") == 0)
	{
		Uninstall();
	}
	else
	{
		if (!::StartServiceCtrlDispatcher(st))
		{
			LogEvent(_T("Register Service Main Function Error!"));
		}
	}

	return 0;
}

void Init()
{
	hServiceStatus = NULL;
	status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	status.dwCurrentState = SERVICE_STOPPED;
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
	status.dwWin32ExitCode = 0;
	status.dwServiceSpecificExitCode = 0;
	status.dwCheckPoint = 0;
	status.dwWaitHint = 0;
}


//Description:	服务主函数，这在里进行控制对服务控制的注册
void WINAPI ServiceMain()
{
	// Register the control request handler
	status.dwCurrentState = SERVICE_START_PENDING;
	status.dwControlsAccepted = SERVICE_ACCEPT_STOP;


	//注册服务控制
	hServiceStatus = RegisterServiceCtrlHandler(szServiceName, ServiceStrl);
	if (hServiceStatus == NULL)
	{
		LogEvent(_T("Handler not installed"));
		return;
	}
	SetServiceStatus(hServiceStatus, &status);

	status.dwWin32ExitCode = S_OK;
	status.dwCheckPoint = 0;
	status.dwWaitHint = 0;
	status.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hServiceStatus, &status);

	std::string szRootPath =GetSiteViewRootPath();

	string iniPath= szRootPath + "\\data\\tmpinifile\\xiechengQueue.ini";

	CString str;
	CTimeSpan  refreshTime;
	CTime firstTime;
	CTime secendTime;
	UINT  refreshIntervalTime = 0;  //分钟

	firstTime = CTime::GetCurrentTime();
	//str = firstTime.Format("现在时间 %Y-%m-%d %H:%M:%S");

	while (1)
	{
		refreshIntervalTime = GetPrivateProfileInt("xiechengQueue", "RefreshTime", 25, iniPath.c_str());
		secendTime = CTime::GetCurrentTime();

		refreshTime = secendTime-firstTime;

		if (refreshTime.GetTotalSeconds() < refreshIntervalTime*60 )
		{
			Sleep(1000);
			continue;
		}
		firstTime = secendTime;



		str = secendTime.Format("刷新时间 %Y-%m-%d %H:%M:%S");


		WriteLog(str.GetBuffer(str.GetLength()));

		WinExec("xieChengQueuePatch.exe", SW_SHOWNORMAL);
	}

	//----------------------------------------------------------	

	WriteLog("Service stopped1");
	status.dwCurrentState = SERVICE_STOPPED;
	SetServiceStatus(hServiceStatus, &status);
	WriteLog("Service stopped2");
	LogEvent(_T("Service stopped"));

}


//Description:			服务控制主函数，这里实现对服务的控制，
//						当在服务管理器上停止或其它操作时，将会运行此处代码
//Input:				dwOpcode：控制服务的状态
//#define SERVICE_STOPPED                        0x00000001
//#define SERVICE_START_PENDING                  0x00000002
//#define SERVICE_STOP_PENDING                   0x00000003
//#define SERVICE_RUNNING                        0x00000004
//#define SERVICE_CONTINUE_PENDING               0x00000005
//#define SERVICE_PAUSE_PENDING                  0x00000006
//#define SERVICE_PAUSED                         0x00000007
void WINAPI ServiceStrl(DWORD dwOpcode)
{
	switch (dwOpcode)
	{
	case SERVICE_CONTROL_STOP:
		//status.dwCurrentState = SERVICE_STOP_PENDING;
		status.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(hServiceStatus, &status);
		PostThreadMessage(dwThreadID, WM_CLOSE, 0, 0);
		WriteLog("service stop");
		break;
	case SERVICE_CONTROL_PAUSE:
		break;
	case SERVICE_CONTROL_CONTINUE:
		break;
	case SERVICE_CONTROL_INTERROGATE:
		break;
	case SERVICE_CONTROL_SHUTDOWN:
		status.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(hServiceStatus, &status);
		PostThreadMessage(dwThreadID, WM_CLOSE, 0, 0);
		WriteLog("service shutdown");
		break;
	default:
		LogEvent(_T("Bad service request"));
	}
}

//Description:			判断服务是否已经被安装
BOOL IsInstalled()
{
	BOOL bResult = FALSE;

	//打开服务控制管理器
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM != NULL)
	{
		//打开服务
		SC_HANDLE hService = ::OpenService(hSCM, szServiceName, SERVICE_QUERY_CONFIG);
		if (hService != NULL)
		{
			bResult = TRUE;
			::CloseServiceHandle(hService);
		}
		::CloseServiceHandle(hSCM);
	}
	return bResult;
}


//Description:			安装服务函数
BOOL Install()
{
	if (IsInstalled())
		return TRUE;

	//打开服务控制管理器
	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		MessageBox(NULL, _T("Couldn't open service manager"), szServiceName, MB_OK);
		return FALSE;
	}

	// Get the executable file path
	TCHAR szFilePath[MAX_PATH];
	::GetModuleFileName(NULL, szFilePath, MAX_PATH);

	//创建服务
	SC_HANDLE hService = ::CreateService(
		hSCM, szServiceName, szServiceName,
		SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS,
		SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL,
		szFilePath, NULL, NULL, _T(""), NULL, NULL);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't create service"), szServiceName, MB_OK);
		return FALSE;
	}

	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);
	return TRUE;
}


//Description:			删除服务函数
BOOL Uninstall()
{
	if (!IsInstalled())
		return TRUE;

	SC_HANDLE hSCM = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (hSCM == NULL)
	{
		MessageBox(NULL, _T("Couldn't open service manager"), szServiceName, MB_OK);
		return FALSE;
	}

	SC_HANDLE hService = ::OpenService(hSCM, szServiceName, SERVICE_STOP | DELETE);

	if (hService == NULL)
	{
		::CloseServiceHandle(hSCM);
		MessageBox(NULL, _T("Couldn't open service"), szServiceName, MB_OK);
		return FALSE;
	}
	SERVICE_STATUS status;
	::ControlService(hService, SERVICE_CONTROL_STOP, &status);

	//删除服务
	BOOL bDelete = ::DeleteService(hService);
	::CloseServiceHandle(hService);
	::CloseServiceHandle(hSCM);

	if (bDelete)
		return TRUE;

	LogEvent(_T("Service could not be deleted"));
	return FALSE;
}


//Description:			记录服务事件
void LogEvent(LPCTSTR pFormat, ...)
{
	TCHAR    chMsg[256];
	HANDLE  hEventSource;
	LPTSTR  lpszStrings[1];
	va_list pArg;

	va_start(pArg, pFormat);
	_vstprintf(chMsg, pFormat, pArg);
	va_end(pArg);

	lpszStrings[0] = chMsg;

	hEventSource = RegisterEventSource(NULL, szServiceName);
	if (hEventSource != NULL)
	{
		ReportEvent(hEventSource, EVENTLOG_INFORMATION_TYPE, 0, 0, NULL, 1, 0, (LPCTSTR*) &lpszStrings[0], NULL);
		DeregisterEventSource(hEventSource);
	}
}

int mainRun(void)
{
	std::string szRootPath =GetSiteViewRootPath();
	string dpath= szRootPath + "\\itsm\\hot-deploy\\svdb\\config\\" + "scasvapi.dll";
	string iniPath= szRootPath + "\\data\\tmpinifile\\xiechengQueue.ini";

	//printf("dpath = %s\n", dpath.c_str());

	printf("iniPath = %s\n", iniPath.c_str());
	printf("开始刷新...\n");

	//FindWindow("CC", "scsvapiTest.exe");
	//string dpath = "D:\\siteview\\SiteView ECC\\itsm\\hot-deploy\\svdb\\config\\scasvapi.dll";
	//printf("dpath = %s\n", dpath.c_str());


	UINT      refreshTime = 0;  //分钟
	DWORD     firstTime  = 0;
	DWORD     secendTime = 0;
	int       enable = 0;

	firstTime = GetTickCount();

	ForestMap fmap;
	NodeData  inwhat;
	string    estr = "";
	string    parentID = "";
	char      monitorID[50];

	ForestMap fmap2;
	NodeData  inwhat2;
	string    estr2 = "";

	memset(monitorID, 0, sizeof(monitorID));

	while(1)
	{	
		refreshTime = GetPrivateProfileInt("xiechengQueue", "RefreshTime", 25, iniPath.c_str());
		enable = GetPrivateProfileInt("xiechengQueue", "Enable", 0, iniPath.c_str());

		if (enable==0)
		{
			printf("未启用...\n");
			WriteLog("enable==0");
			return 0;
		}
		secendTime = GetTickCount();

		if ((secendTime-firstTime) < refreshTime*60*1000 )
		{
			Sleep(1000);
			continue;
		}
		firstTime = secendTime;

		//ForestMap fmap;
		//NodeData  inwhat;
		//string    estr = "";
		//string    parentID = "";
		//char      monitorID[50];

		//memset(monitorID, 0, sizeof(monitorID));


		fmap.clear();
		inwhat.clear();
		estr = "";
		parentID = "";
		memset(monitorID, '\0', sizeof(monitorID));



		GetPrivateProfileString("xiechengQueue", "MonitorID", "" ,monitorID, sizeof(monitorID)-1, iniPath.c_str());
		parentID = monitorID;
		//printf("parentid = %s, lenth = %d\n", parentID.c_str(), parentID.length());
		parentID = parentID.substr(0, parentID.rfind("."));

		//printf("monitorID = %s\n", monitorID);
		//printf("parentid = %s\n", parentID.c_str());
		//printf("refreshTime = %d\n", refreshTime);

		inwhat.insert(make_pair("dowhat",        "RefreshMonitors"));
		inwhat.insert(make_pair("id",            monitorID));
		inwhat.insert(make_pair("instantReturn", "true"));
		inwhat.insert(make_pair("parentid",      parentID));

		int res = GetUnivData(fmap, inwhat, estr);
		if (res==1)
		{
			cout <<  "RefreshMonitors OK" << endl; 	
		}
		else
		{
			cout <<  "RefreshMonitors bad" << endl; 
			cout << "estr "<< estr << endl;
			WriteLog("res==0, estr:");
			WriteLog(estr.c_str());
			Sleep(10000);
			continue;
			//return 0;
		}

		//---------------------找到队列名-----------------------------------
		string queueName = "";
		//string queueName = 0; //编译不报错,运行出错。-_-！
		ForestMap::const_iterator mit = fmap.begin();
		NodeData::const_iterator nit = mit->second.find("queueName");
		queueName = TrimSpace(nit->second);

		//printf("queueName = %s\n", queueName.c_str());
		//---------------------找到队列名-----------------------------------

		Sleep(3000);

		fmap2.clear();
		inwhat2.clear();
		estr2 = "";

		inwhat.insert(make_pair("dowhat",     "GetRefreshed"));
		inwhat.insert(make_pair("queueName",  queueName));
		inwhat.insert(make_pair("parentid",   parentID));


		GetUnivData(fmap2, inwhat2, estr2);
		DeleteQueue(queueName);
		if (res==1)
		{
			cout <<  "GetRefreshed OK" << endl; 
		}
		else
		{
			cout <<  "GetRefreshed bad" << endl; 
			cout << "estr2 "<< estr2 << endl;
			WriteLog("res==0, estr2=");
			WriteLog(estr2.c_str());
			//Sleep(10000);
			//continue;
			return 0;
		}
	}//end While

	return 1;
}
