// AlertServer.cpp : 定义控制台应用程序的入口点。
//报警程序主框架
//一、报警发送对象类：
//    1、数据结构
//	     a、Id、其他发送报警所需的参数、发送时间等。
//    2、调用外部Dll以发送报警的虚函数。
//    3、记录报警日志的函数。
//二、派生报警对象类：邮件、短信、脚本、声音
//    1、重写调用外部Dll以发送报警的虚函数。
//三、输入事件类
//    1、数据结构
//	     a、MonitorId、EventType、EventCount等。
//四、基础报警配置类：
//    1、数据结构
//	      a、AlertTargetList。
//	      b、报警名称、报警条件等。
//		  c、报警计数。
//    2、读监测配置以分解出正确的AlertTargetList的函数。
//    3、根据MonitorId和输入事件判断是否匹配发送条件的虚函数。
//    4、生成报警发送对象的虚函数。
//    5、根据报警计数做升级或停止计数等。
//五、派生基础报警配置类：邮件、短信、脚本、声音
//    1、重写根据MonitorId和输入事件判断是否匹配发送条件的虚函数。
//    2、重写生成报警发送对象的虚函数。
//六、主程序类：
//    1、数据结构
//		a、一个AlertBaseObjList, 类型为基础报警配置类。
//		b、四个WaitSendAlertObjList, 类型为报警发送对象类。
//		c、一个输入EventList, 类型为输入事件类。
//		d、线程：输入、输出。
//    2、控制流程
//		a、读alert.ini以初始化AlertBaseObjList， 并分解出正确的AlertTargetList。
//		b、循环读取输入Event到EventList。
//		c、循环根据AlertBaseObjList和EventList里的Event匹配，并生成AlertSendObj且推送到WaitSendAlertObjList。
//		d、循环将WaitSendAlertObjList发送出去， 维护WaitSendAlertObjList并记录日志。
//七、其他：
//    1、包装成服务的类。
//    2、与报警日志查询相关的类。
//八、升级次数和停止次数带来的问题。
//九、执行脚本时固定脚本和用户自定义脚本的执行方式不一样。
//十、短信报警的端口配置改变时通知本程序， 本程序重新绑定端口（读短信配置ini）。
//十一、任务计划的加入带来的问题-------很复杂的-------有一个统一逻辑做判断， 分离开了复杂度。
//十二、事件升级以解决级联报警问题。
//十三、1、正常。2、告警。3、错误。4、错误。5、禁止。
//十四、Alert.ini文件发生改变则更新AlertBaseObjList。

#include "stdafx.h"
#include "AlertMain.h"
#include "./base/funcGeneral.h"
#include <WinSvc.h>
#include <direct.h>
#include "svdb/svapi/svapi.h"

VOID  WINAPI StartAlertService(DWORD argc, LPTSTR *argv);//启动服务
VOID  WINAPI sch_ServiceCtrlHandler(DWORD opcode);//服务控制
BOOL  InstallService(CString strServiceName);//安装服务
BOOL  StopService(CString strServiceName);//停止服务
BOOL  InitService();//初始化服务线程

void WriteLogFile(CString strMsg);//写日志文件
void Run();

SERVICE_STATUS          sch_ServiceStatus; 
SERVICE_STATUS_HANDLE   sch_ServiceStatusHandle; 

CString g_strRootPath;
CString g_strMessage;

bool hasrun()
{

#ifdef WIN32

    HANDLE hInstance=::CreateEvent(NULL,TRUE,FALSE,"Global\\SiteView-AlertServer");

    if(hInstance==NULL)
    {
        puts("Create event of instance  failed");
        return false;
    }

    if(::GetLastError()==ERROR_ALREADY_EXISTS)
    {
        puts("Instance has exist");
        return true;
    }

    ::SetLastError(0);
    return false;

#endif
}


/*
 *	函数介绍：程序入口函数  
 *  输入参数：argv[1]为-install是安装服务，-remove时删除服务，argv[2]安装时为服务控制台显示服务名称
    删除时为注册服务名称，此程序为AlertService
 *  输出参数：无
 *  返回值：
 */
int _tmain(int argc, _TCHAR* argv[])
{
	int nRetCode = 0;
	if(WriteIniFileString("testsection","testkey","testvalue","alert.ini"))
	{
		DeleteIniFileSection("testsection","alert.ini");
	}
	if(WriteIniFileString("testsection","testkey","testvalue","TxtTemplate.ini"))
	{
		DeleteIniFileSection("testsection","TxtTemplate.ini");
	}
	if(WriteIniFileString("testsection","testkey","testvalue","smsconfig.ini"))
	{
		DeleteIniFileSection("testsection","smsconfig.ini");
	}
	if(WriteIniFileString("testsection","testkey","testvalue","general.ini"))
	{
		DeleteIniFileSection("testsection","general.ini");
	}
	if(WriteIniFileString("testsection","testkey","testvalue","itsmConfig.ini"))
	{
		DeleteIniFileSection("testsection","itsmConfig.ini");
	}
	if(WriteIniFileString("testsection","testkey","testvalue","watchsheetcfg.ini"))
	{
		DeleteIniFileSection("testsection","watchsheetcfg.ini");
	}
	if(WriteIniFileString("testsection","testkey","testvalue","email.ini"))
	{
		DeleteIniFileSection("testsection","email.ini");
	}
	if(WriteIniFileString("testsection","testkey","testvalue","emailAdress.ini"))
	{
		DeleteIniFileSection("testsection","emailAdress.ini");
	}
	if(WriteIniFileString("testsection","testkey","testvalue","smsphoneset.ini"))
	{
		DeleteIniFileSection("testsection","smsphoneset.ini");
	}
	if(WriteIniFileString("testsection","testkey","testvalue","interfacedll.ini"))
	{
		DeleteIniFileSection("testsection","interfacedll.ini");
	}
	if(WriteIniFileString("testsection","testkey","testvalue","OfbizConfig.ini"))
	{
		DeleteIniFileSection("testsection","OfbizConfig.ini");
	}

#if 1
    try
	{
		if(hasrun())
			return 0;

		CAlertMain alertmain;
		
		alertmain.Init();
		
		//MSG msg;
		//BOOL bRet=TRUE;
		//while((bRet=::GetMessage(&msg,NULL,0,0))!=0)
		//{
		//	if(bRet==-1)
		//	{
		//		continue;
		//	}

		//	TranslateMessage(&msg);
		//	::DispatchMessage(&msg);
		//}
		MSG msg;
		while(::GetMessage(&msg,NULL,0,0))
		{
			TranslateMessage(&msg);
			::DispatchMessage(&msg);
			switch(msg.message)
			{
				case WM_QUIT:
					exit(1);
					break;
				default:
					break;
			}
		}
	}
	catch(...)
	{
	}
#else
//   argc = 2;
//	argv[1] = "-remove";
//	argv[2] = "AlertService";
//#if 1
//    RunThread(NULL);
//#else

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}

	g_strRootPath= FuncGetInstallRootPath();
	SERVICE_TABLE_ENTRY Table[] = 
	{
		{"AlertService7",StartAlertService},
		{NULL,        NULL}
	};
	CString outStr = "parameter required in the command line";
	if(argc <= 1)
	{
		
		cout << (LPCTSTR)outStr << endl;
	}
	else
	{
		if(_stricmp(argv[1],"-install") == 0)
		{
			InstallService(argv[2]);
		}
		else if(_stricmp(argv[1],"-remove") == 0)
		{
			StopService(argv[2]);
		}
		else
		{
			cout << outStr << endl;
		}
	}



	if(!StartServiceCtrlDispatcher(Table))
    { 
		g_strMessage.Format("StartServiceCtrlDispatcher error = %d\r\n", GetLastError());
		WriteLogFile(g_strMessage);
    } 	
#endif
	//CAlertMain::pSender.Release();
	return nRetCode;
}

void Run()
{
	//while(true)
	//{
	//	Sleep(100);
	//}
	
	//CAlertMain::pSender.Release();
	//alertmain.Run();	
}

/*
 *	函数介绍：服务启动函数
 */
VOID  WINAPI StartAlertService(DWORD argc, LPTSTR *argv)
{

 
    sch_ServiceStatus.dwServiceType        = SERVICE_WIN32; 
    sch_ServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
    sch_ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE; 
    sch_ServiceStatus.dwWin32ExitCode      = 0; 
    sch_ServiceStatus.dwServiceSpecificExitCode = 0; 
    sch_ServiceStatus.dwCheckPoint         = 0; 
    sch_ServiceStatus.dwWaitHint           = 0; 
 
    sch_ServiceStatusHandle = RegisterServiceCtrlHandler("AlertService7", sch_ServiceCtrlHandler);

	if(sch_ServiceStatusHandle == 0)
	{
		g_strMessage.Format("RegisterServiceCtrlHandler failed %d\r\n", GetLastError());
		WriteLogFile(g_strMessage);
		return;
	}

    // Initialization complete - report running status. 
    sch_ServiceStatus.dwCurrentState       = SERVICE_RUNNING; 
    sch_ServiceStatus.dwCheckPoint         = 0; 
    sch_ServiceStatus.dwWaitHint           = 0; 
 
    if (!SetServiceStatus (sch_ServiceStatusHandle, &sch_ServiceStatus)) 
    { 
		g_strMessage.Format("SetServiceStatus error %ld\r\n", GetLastError());
		WriteLogFile(g_strMessage);
		return;
    } 
	
	//InitService();//启动实际工作线程
	Run();
}

/*
 *	函数介绍：服务控制函数
 *  输入参数：控制码Opcode
 *  输出参数: 无
 *  返回值：无
 */
VOID WINAPI sch_ServiceCtrlHandler(DWORD Opcode) 
{ 
    switch(Opcode) 
    { 
        case SERVICE_CONTROL_PAUSE: //暂停服务
        // Do whatever it takes to pause here. 
            sch_ServiceStatus.dwCurrentState = SERVICE_PAUSED; 
			//AddToLog("Pause Service\r\n");
            break; 
 
        case SERVICE_CONTROL_CONTINUE: //继续服务
        // Do whatever it takes to continue here. 
            sch_ServiceStatus.dwCurrentState = SERVICE_RUNNING; 

			//AddToLog("Continue Service\r\n");
            break; 
 
        case SERVICE_CONTROL_STOP: //停止服务
        // Do whatever it takes to stop here. 
            
            sch_ServiceStatus.dwWin32ExitCode = 0; 
            sch_ServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
            sch_ServiceStatus.dwCheckPoint    = 0; 
            sch_ServiceStatus.dwWaitHint      = 0; 
            
			//smsPort.CloseCom();
			CAlertMain::CloseSerialPort();

            if(!SetServiceStatus(sch_ServiceStatusHandle, &sch_ServiceStatus))
            { 
				g_strMessage.Format("SetServiceStatus error %ld\r\n", GetLastError());
				WriteLogFile(g_strMessage);
            } 

            return; 
 
        case SERVICE_CONTROL_INTERROGATE: 
        // Fall through to send current status. 
            break; 
 
        default:
			g_strMessage.Format("Unrecognized opcode %ld\r\n", Opcode);
			WriteLogFile(g_strMessage);
    } 
 
    // Send current status. 
    if (!SetServiceStatus (sch_ServiceStatusHandle,  &sch_ServiceStatus)) 
    { 
		g_strMessage.Format("SetServiceStatus error %ld\r\n", GetLastError());
		WriteLogFile(g_strMessage);
		return;
    } 
} 

/*
 *	函数介绍：安装服务函数
 *  输入参数：strServiceName为在服务控制台中显示的名称
 *  输出参数：无
 *  返回值：服务启动正确返回TRUE，否则返回FALSE
 */
BOOL InstallService(CString strServiceName) 
{ 
    DWORD			dwOldCheckPoint; 
    DWORD			dwStartTickCount;
    DWORD			dwWaitTime;
    SERVICE_STATUS	ssStatus; 

	SC_HANDLE		schService;
	SC_HANDLE		schSCManager;

	if((schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
	{
		WriteLogFile("OpenSCManager Failed");
		return FALSE;
	}
    CString path = g_strRootPath + "\\cgi-bin\\AlertService.exe";
	if((schService = CreateService(schSCManager,"AlertService7", strServiceName, SERVICE_ALL_ACCESS,
                    SERVICE_WIN32_OWN_PROCESS,SERVICE_AUTO_START,
                    SERVICE_ERROR_IGNORE,path,NULL,NULL,NULL,NULL,NULL)) == NULL)
	{
		WriteLogFile("OpenService Failed");
		CloseServiceHandle(schSCManager);
		return FALSE;
	}

	if(!StartService(schService, 0,	NULL))
	{
		
		WriteLogFile("StartService Failed");

		CloseServiceHandle(schService);
		CloseServiceHandle(schSCManager);
		return FALSE;
	}
	else 
	{
	}

	// Check the status until the service is no longer start pending. 
 	if (!QueryServiceStatus(schService, &ssStatus))
	{
		CloseServiceHandle(schService);
		CloseServiceHandle(schSCManager);
		return FALSE;
	}

	// Save the tick count and initial checkpoint.
	dwStartTickCount = GetTickCount();
	dwOldCheckPoint = ssStatus.dwCheckPoint;

	while (ssStatus.dwCurrentState == SERVICE_START_PENDING) 
	{ 
		// Do not wait longer than the wait hint. A good interval is 
		// one tenth the wait hint, but no less than 1 second and no 
		// more than 10 seconds. 

		dwWaitTime = ssStatus.dwWaitHint / 10;

		if( dwWaitTime < 1000 )
			dwWaitTime = 1000;
		else if ( dwWaitTime > 10000 )
			dwWaitTime = 10000;

		Sleep( dwWaitTime );

		// Check the status again. 
		if (!QueryServiceStatus(schService,   // handle to service 
								&ssStatus) )  // address of structure
			break; 

		if ( ssStatus.dwCheckPoint > dwOldCheckPoint )
		{
			// The service is making progress.

			dwStartTickCount = GetTickCount();
			dwOldCheckPoint = ssStatus.dwCheckPoint;
		}
		else
		{
			if(GetTickCount()-dwStartTickCount > ssStatus.dwWaitHint)
			{
				// No progress made within the wait hint
				break;
			}
		}
	} 

	if (ssStatus.dwCurrentState == SERVICE_RUNNING) 
	{
		CloseServiceHandle(schService);
		CloseServiceHandle(schSCManager);
		return TRUE;
	}
	else 
	{ 
		CloseServiceHandle(schService); 
		CloseServiceHandle(schSCManager);
		return FALSE;
	} 

	return TRUE;
} 

/*
 *	函数介绍：删除服务函数
 *  输入参数：服务控制台中显示的服务名称
 *  输出参数：无
 *  返回值：  成功返回TRUE，否则FALSE
 */
BOOL StopService(CString strServiceName)
{
	SERVICE_STATUS	ssStatus;
	SC_HANDLE		schService;
	SC_HANDLE		schSCManager;

	if((schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
	{
		return FALSE;
	}

	if((schService = OpenService(schSCManager, TEXT(strServiceName), SERVICE_ALL_ACCESS|DELETE)) == NULL)
	{
		return FALSE;
	}

	// try to stop the service
	if ( ControlService( schService, SERVICE_CONTROL_STOP, &ssStatus ) )
	{
		Sleep( 1000 );

		while( QueryServiceStatus( schService, &ssStatus ) )
		{
			if ( ssStatus.dwCurrentState == SERVICE_STOP_PENDING )
			{
				Sleep( 1000 );
			}
			else
				break;
		}

		if ( ssStatus.dwCurrentState == SERVICE_STOPPED )
		{
			CloseServiceHandle(schService);
			CloseServiceHandle(schSCManager);
			return TRUE;
		}
		else
		{
			CloseServiceHandle(schService);
			CloseServiceHandle(schSCManager);
			return FALSE;
		}
	}
	BOOL suc = DeleteService(schService);
	if(suc)
	{
		CString str = "delete is suceed!";
		cout << str << endl;
	}

	CloseServiceHandle(schService);
	CloseServiceHandle(schSCManager);

	return FALSE;
}

void WriteLogFile(CString strMsg)
{
    FILE *stream;
    char chTime[50] = {0};
    //打开文件
    stream=fopen(FuncGetInstallRootPath() + "\\AlertServer.log","a");
    if(stream == NULL)
        return ;
   	SYSTEMTIME time;
	//取系统当前时间
	GetLocalTime(&time);
    sprintf(chTime ,"%02dY-%02dM-%02dD %02dH-%02dM-%02dS ",
				time.wYear - 2000,
				time.wMonth,
				time.wDay,
				time.wHour,
				time.wMinute,
				time.wSecond
			);
    fputs(chTime , stream);
	fputs(strMsg.GetBuffer(strMsg.GetLength()),stream);
    fputs("\n",stream);
	fclose(stream);
}

//
///*
// *	函数介绍：启动工作线程
// */
//BOOL InitService()
//{
//	//hThread = AfxBeginThread(RunThread,NULL);
// //   hThTask = AfxBeginThread(TaskThread, NULL);
//	//if(hThread == NULL)
//	//{
//	//	return FALSE;
//	//}
//	//else
//	//{
//	//	return TRUE;
//	//}	
//}
//
