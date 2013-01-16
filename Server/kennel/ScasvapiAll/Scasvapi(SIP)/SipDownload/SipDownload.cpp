// SipDownload.cpp : 定义控制台应用程序的入口点。
// Sip版ECC中用于管理Sip下载功能的服务程序 created by lifeng.wang  2009-10-22

#include "stdafx.h"
#include "SipDownload.h"

void WriteLog(const char * strMsg)
{
    char datebuf[128] , timebuf[128] , tempbuf[1024];
	_tzset();
	_strdate(datebuf);
	_strtime(timebuf);

	sprintf(tempbuf , "%s-%s\t" , datebuf , timebuf);

	char filename[128] = {0};

	string strPath = "";
	strPath = GetSiteViewRootPath();

	sprintf(filename , "%s\\fcgi-bin\\SipDownload.log" , strPath.c_str());

	FILE * pFile;
	struct _stat buf;
	if(_stat(filename , &buf) == 0)
	{
		if(buf.st_size >= 5000*1024)
		{
			pFile = fopen(filename , "w");
			if(pFile != NULL)
				fclose(pFile);
		}
	}
	pFile = fopen(filename ,"a+");	
	if(pFile != NULL)
	{
		fprintf(pFile ,"%s%s\n" , tempbuf , strMsg);
		fclose(pFile);
	}    
}

void GetErrorMsg(char * strMsg)
{
	DWORD dwError = ::GetLastError();
	LPSTR lpBuffer = NULL;
	::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwError,
		NULL,
		(LPTSTR)& lpBuffer,
		0,
		NULL
		);
	sprintf(strMsg , "%s" , lpBuffer);
	cout<< dwError<<endl;
	::LocalFree(lpBuffer);
}

bool Install()
{
	bool bRet = false;
	char buf[1024] = {0};
	sprintf(buf , "\"%s - service\"" , g_mcPName.c_str());

	SC_HANDLE hs = ::OpenSCManager(NULL , SERVICES_ACTIVE_DATABASE , SC_MANAGER_ALL_ACCESS);

	if(hs == NULL)
	{
		WriteLog("Open SCManger failed!");
		return false;
	}

	if(::CreateService(hs , g_ServiceName , g_ServiceName ,
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START,
		SERVICE_ERROR_NORMAL,
		buf,
		"",
		NULL , NULL , g_account , g_password) == NULL)
	{
//		WriteLog("Install service failed!");
		char lpBuffer[1024] = {0};
		GetErrorMsg(lpBuffer);
		if(lpBuffer != NULL)
		{
			WriteLog(lpBuffer);
			cout<<lpBuffer<<endl;
		}
		if(g_account != NULL)
			free(g_account);
		if(g_password != NULL)
			free(g_password);
		return false;
	}
	else
	{
		WriteLog("Install service successful");
		bRet = true;
	}
	if(g_account != NULL)
		free(g_account);
	if(g_password != NULL)
		free(g_password);
	return bRet;
}

bool UnInstall()
{
	bool bRet = false;
	LPSTR lpBuffer = NULL;
	SC_HANDLE hsm = ::OpenSCManager(NULL , SERVICES_ACTIVE_DATABASE , SC_MANAGER_ALL_ACCESS);
	if(hsm == NULL)
	{
		WriteLog("Open SCManager failed");
		WriteLog("Remove service failed");
		return bRet;
	}
	SC_HANDLE hsc = ::OpenService(hsm , g_ServiceName , DELETE);
	if(hsc == NULL)
	{
		WriteLog("Open service failed");
		char lpBuffer[1024];
		GetErrorMsg(lpBuffer);
		WriteLog(lpBuffer);
		return false;
	}
	if(::DeleteService(hsc) == 0)
	{
        WriteLog("Remove service failed!");
		char lpBuffer[1024];
		GetErrorMsg(lpBuffer);
		WriteLog(lpBuffer);
		return false;
	}
	bRet = true;
	return bRet;
}

bool Init()
{
	bool bRet = false;
	try
	{
		nMax_Updating_Num = atoi(GetIniFileString("OtherSet" , "maxUpCount" , "" , "DeviceUpgrade.ini").c_str());
		nUpdating_Num = 0;

		SvIniFile inifile;
		SvIniFile::iterator pMap;
		GetSvIniFileBySections(inifile , "SipDownload.ini" , "default");

		pMap = inifile.begin();
		while(pMap != inifile.end())
		{
			string strTemp = pMap->second.find("UpdateState")->second;
//			if(pMap->second.find("UpdateState")->second.compare("正在升级") == 0)
			if(strTemp.compare("正在升级") == 0)
			{
				string delayTime = pMap->second.find("DelayTime")->second;
				if(delayTime.empty())
				{
					nUpdating_Num++;
					pMap++;
					continue;
				}
				string beginTime = pMap->second.find("BeginTime")->second;
				time_t CT;	//当前时间
				time_t ST;	//开始时间
				time_t DT;	//超时时间
				CTime t1 = CTime::GetCurrentTime();
				CT = t1.GetTime();
				Update::ConvertTime(beginTime , ST);
				DT = (atoi(delayTime.c_str()))*60*1000;
				if((ST+DT) > CT)
				{
					WriteIniFileString(pMap->first , "UpdateState" , "升级失败" , "SipDownload.ini");
				}
				else
				{
					nUpdating_Num++;
				}
			}
			pMap++;
		}

		bRet = true;
	}
	catch(...)
	{
		char strErrorMsg[1024] = {0};
		GetErrorMsg(strErrorMsg);
		WriteLog(strErrorMsg);
		bRet = false;
	}
	return bRet;
}

void CALLBACK TimerProc(HWND   hwnd,           //   handle   of   window   for   timer   messages
						UINT   uMsg,           //   WM_TIMER   message
						UINT   idEvent,     //   timer   identifier
						DWORD   dwTime
						)
{
	if(Init())
	{
		if(nUpdating_Num >= nMax_Updating_Num)
		{
			return;
		}
		bool bRet = false;
		Update sip;		
		bRet = sip.Run();
	}
}

BOOL Run(void)
{		

	char strMsg[1024] = {0};
	CTime runTime = CTime::GetCurrentTime();
	int hour = runTime.GetHour();
	sprintf(strMsg , "LocalHour= %d", hour);
	cout<<strMsg<<endl;
	TimerProc(NULL , 0 ,0 ,0);
	int nSend = (int)SetTimer(NULL , 1 , RUNNING_TIMES , (TIMERPROC)TimerProc);		//每分钟运行一次回调函数
	MSG msg;

	if(nSend)
	{
		cout<<nSend<<endl;
	}
	BOOL bRet = FALSE;
	while((bRet=GetMessage(&msg, NULL, 0, 0)) != 0)
//	while(::GetMessage(&msg , NULL , 0 , 0))
	{
		if(bRet != -1)
		{
			if(msg.message == WM_TIMER)
			{
				cout<<"WM_TIMER"<<endl;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			break;
		}
	}

	KillTimer(NULL , nSend);


	return TRUE;
}

void WINAPI SVS_ServiceCtrlHandler(DWORD opcode)
{
	switch(opcode)
	{
	case SERVICE_CONTROL_PAUSE:
		{
			SVS_ServiceStatus.dwCurrentState=SERVICE_PAUSED;
			break;  
		}
	case SERVICE_CONTROL_CONTINUE:
		{
			SVS_ServiceStatus.dwCurrentState=SERVICE_RUNNING;
			break; 
		}
	case SERVICE_CONTROL_STOP: 
	case SERVICE_CONTROL_SHUTDOWN:
		{
			SVS_ServiceStatus.dwWin32ExitCode = 0; 
            SVS_ServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
            SVS_ServiceStatus.dwCheckPoint    = 0; 
            SVS_ServiceStatus.dwWaitHint      = 0; 

			g_SubProcessState=FALSE;

		//	::SetEvent(g_hExit);
		//	::SetEvent(g_hCom);
			::Sleep(1000);
		   ::PostThreadMessage(g_alertpi.dwThreadId,WM_QUIT,0,0);
		    ::PostThreadMessage(g_reportpi.dwThreadId,WM_QUIT,0,0);

			::PostThreadMessage(g_pi.dwThreadId,WM_QUIT,0,0);
			::Sleep(1000);
			::TerminateProcess(g_pi.hProcess,3);
			::TerminateProcess(g_alertpi.hProcess,3);

			WriteLog("服务退出，正终止子进程...");

 
            if(!SetServiceStatus(SVS_ServiceStatusHandle, &SVS_ServiceStatus))
            { 
				WriteLog("设置服务状态出错:");
				char strErrorMsg[1024] = {0};
				GetErrorMsg(strErrorMsg);
				WriteLog(strErrorMsg);
            }
            return; 
		}
	case SERVICE_CONTROL_INTERROGATE:
		{
			//Fall through to send current status. 
			//AddToErrorLog("Service Control Interrogate\r\n");
            break;
		}
	default:
		{
			WriteLog("Unrecognized opcode");
			char strErrorMsg[1024] = {0};
			GetErrorMsg(strErrorMsg);
			WriteLog(strErrorMsg);
			break;
		}
	}
	if (!SetServiceStatus (SVS_ServiceStatusHandle,  &SVS_ServiceStatus)) 
	{
		WriteLog("设置服务状态出错:");
		char strErrorMsg[1024] = {0};
		GetErrorMsg(strErrorMsg);
		WriteLog(strErrorMsg);
    } 
}

DWORD SVS_ServiceInitialization(DWORD argc, LPTSTR *argv, DWORD *specificError) 
{ 
    argv; 
    argc; 
    specificError; 
    return(0); 
}

void WINAPI SVS_ServiceStart(DWORD argc , LPTSTR * argv)
{
	cout<<"Start SVS_ServiceStart"<<endl;
	WriteLog("Start SVS_ServiceStart");

	DWORD	status = 0;
	DWORD	specificError = 0;

	SVS_ServiceStatus.dwServiceType				= SERVICE_WIN32;
	SVS_ServiceStatus.dwCurrentState			= SERVICE_START_PENDING;
	SVS_ServiceStatus.dwControlsAccepted		= SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;
	SVS_ServiceStatus.dwWin32ExitCode			= 0;
	SVS_ServiceStatus.dwServiceSpecificExitCode	= 0;
	SVS_ServiceStatus.dwCheckPoint				= 0;
	SVS_ServiceStatus.dwWaitHint				= 0;

	SVS_ServiceStatusHandle = ::RegisterServiceCtrlHandler("VALIDATESERVER" , SVS_ServiceCtrlHandler);
	if(SVS_ServiceStatusHandle == (SERVICE_STATUS_HANDLE)0)
	{
		WriteLog("RegisterServiceCtrlHandler failed!");
		char strErrMsg[1024] = {0};
		GetErrorMsg(strErrMsg);
		WriteLog(strErrMsg);
		return;
	}
	
	status = SVS_ServiceInitialization(argc,argv, &specificError); 

	if(status != NO_ERROR)
	{
		SVS_ServiceStatus.dwCurrentState       = SERVICE_STOPPED; 
        SVS_ServiceStatus.dwCheckPoint         = 0; 
        SVS_ServiceStatus.dwWaitHint           = 0; 
        SVS_ServiceStatus.dwWin32ExitCode      = status; 
        SVS_ServiceStatus.dwServiceSpecificExitCode = specificError; 
 
        SetServiceStatus (SVS_ServiceStatusHandle, &SVS_ServiceStatus); 

		WriteLog("服务初始化失败!\r\n");		
        return; 
	}

	SVS_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	SVS_ServiceStatus.dwCheckPoint = 0;
	SVS_ServiceStatus.dwWaitHint = 0;

	if (!SetServiceStatus (SVS_ServiceStatusHandle, &SVS_ServiceStatus)) 
    { 
		WriteLog("设置服务状态出错");
		char strErrMsg[1024] = {0};
		GetErrorMsg(strErrMsg);
		WriteLog(strErrMsg);
		return;
    }

	HANDLE	m_Handle = GetCurrentProcess();
	DWORD	iPriority = ::GetPriorityClass(m_Handle);
	::SetPriorityClass(m_Handle , HIGH_PRIORITY_CLASS);

	HANDLE	m_Thread = ::GetCurrentThread();
	::SetThreadPriority(m_Thread ,THREAD_PRIORITY_ABOVE_NORMAL);

	WriteLog("服务启动成功！");
    
	Run();
	MSG msg;
	while(::GetMessage(&msg,NULL,0,0))
	{
		DWORD time = msg.time;
		char strTemp[1024] = {0};
		sprintf(strTemp,"Send message time is :%ld" , time);
		WriteLog(strTemp);
		TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	WriteLog("========SipDownload Start========");

	g_mcPName = argv[0];
//	g_account = _strdup("Test");
//	g_password = _strdup("test");

    
	if(argc > 1)
	{
		if(stricmp(argv[1] , "-install") == 0)
		{
			g_account = _strdup("Test");
			g_password = _strdup("test");
			if(argc == 4)
			{
				g_account = _strdup(argv[2]);
				g_password = _strdup(argv[3]);
			}
			if(Install())
			{
				cout<<"install service successful!"<<endl;
			}
			else
			{
				cout<<"install service failed!"<<endl;
			}
		}
		else if(stricmp(argv[1] , "-uninstall") == 0)
		{
			UnInstall();
		}
		else if(stricmp(argv[1] , "-service") == 0)
		{
			SERVICE_TABLE_ENTRY ste[] =
			{
				{"SipDownload" , SVS_ServiceStart},
				{NULL , NULL}
			};
			if(::StartServiceCtrlDispatcher(ste))
			{
				cout<<"start service ctrl dispatcher "<<endl;
				WriteLog("start service ctrl dispatcher successful!");
			}
			else
			{
				cout<<"start service ctrl dispatcher failed"<<endl;
				char strMSG[1024] = {0};
				GetErrorMsg(strMSG);
				cout<<strMSG<<endl;
				WriteLog(strMSG);
			}

        
		}
		else
		{
			WriteLog("Error parameters!");
			return -1;
		}
	}

	MSG msg;
	while(::GetMessage(&msg,NULL,0,0))
	{
		DWORD time = msg.time;
		char strTemp[1024] = {0};
		sprintf(strTemp,"(_tmain)Send message time is :%ld" , time);
		WriteLog(strTemp);

		puts("Got Message");
		TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	::Sleep(100000);

	return 0;
}