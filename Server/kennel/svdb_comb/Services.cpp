#include "Services.h"
#include "svdbtype.h"
#include <iostream>
#include "Util.h"

#ifdef WIN32
#include "windows.h"
#include "winsvc.h"
#endif

using namespace std;

extern string g_dbPName;


bool RunDB(void);

#ifdef WIN32

PROCESS_INFORMATION g_dbpi;
bool g_Disabled=false;
bool g_SubProcessState=true;

extern char *g_account;
extern char *g_password;

const char *g_ServiceName="SiteView_SVDB";
const char *g_ServiceDName="SiteView_SVDB";

SERVICE_STATUS          SVS_ServiceStatus; 
SERVICE_STATUS_HANDLE   SVS_ServiceStatusHandle;

DWORD SVS_ServiceInitialization(DWORD argc, LPTSTR *argv, DWORD *specificError);


void  WINAPI SVS_ServiceCtrlHandler(DWORD opcode)
{
	switch(opcode)
	{
        case SERVICE_CONTROL_PAUSE: 
           SVS_ServiceStatus.dwCurrentState=SERVICE_PAUSED;
             break; 
 
        case SERVICE_CONTROL_CONTINUE: 
           SVS_ServiceStatus.dwCurrentState=SERVICE_RUNNING;
            break; 
 
        case SERVICE_CONTROL_STOP: 
		case SERVICE_CONTROL_SHUTDOWN:
            SVS_ServiceStatus.dwWin32ExitCode = 0; 
            SVS_ServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
            SVS_ServiceStatus.dwCheckPoint    = 0; 
            SVS_ServiceStatus.dwWaitHint      = 0; 

			g_SubProcessState=false;

		   ::PostThreadMessage(g_dbpi.dwThreadId,WM_QUIT,0,0);
			::Sleep(2000);
//			::TerminateProcess(g_pi.hProcess,3);

//			AddToEventLog("服务退出正终止子进程");

 
            if(!SetServiceStatus(SVS_ServiceStatusHandle, &SVS_ServiceStatus))
            { 
//				g_strMessage.Format("设置服务状态出错: %ld\r\n", GetLastError());
//				AddToErrorLog(g_strMessage);
            } 
			
            return; 
 
        case SERVICE_CONTROL_INTERROGATE: 
        // Fall through to send current status. 
//			AddToErrorLog("Service Control Interrogate\r\n");
            break; 
 
        default:
//			g_strMessage.Format("Unrecognized opcode %ld\r\n", opcode);
//			AddToErrorLog(g_strMessage);
			break;
    } 

    if (!SetServiceStatus (SVS_ServiceStatusHandle,  &SVS_ServiceStatus)) 
    { 
//		g_strMessage.Format("设置服务状态出错: %ld\r\n", GetLastError());
	
//		AddToErrorLog(g_strMessage);
    } 


}
void WINAPI SVS_ServiceStart(DWORD argc, LPTSTR *argv) 
{
	
	DWORD status=0;
	DWORD specificError=0;

    SVS_ServiceStatus.dwServiceType        = SERVICE_WIN32; 
    SVS_ServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
    SVS_ServiceStatus.dwControlsAccepted   =	SERVICE_ACCEPT_SHUTDOWN | 
												SERVICE_ACCEPT_STOP | 
												SERVICE_ACCEPT_PAUSE_CONTINUE; 
    SVS_ServiceStatus.dwWin32ExitCode      = 0; 
    SVS_ServiceStatus.dwServiceSpecificExitCode = 0; 
    SVS_ServiceStatus.dwCheckPoint         = 0; 
    SVS_ServiceStatus.dwWaitHint           = 0; 

	SVS_ServiceStatusHandle=::RegisterServiceCtrlHandler("SVDB_SERVER",SVS_ServiceCtrlHandler);
    if (SVS_ServiceStatusHandle == (SERVICE_STATUS_HANDLE)0) 
    { 
//		g_strMessage.Format("RegisterServiceCtrlHandler failed %d\r\n", GetLastError());
//     	AddToErrorLog(g_strMessage);
		return; 
    } 

    status = SVS_ServiceInitialization(argc,argv, &specificError); 
 
    // Handle error condition 
    if (status != NO_ERROR) 
    { 
        SVS_ServiceStatus.dwCurrentState       = SERVICE_STOPPED; 
        SVS_ServiceStatus.dwCheckPoint         = 0; 
        SVS_ServiceStatus.dwWaitHint           = 0; 
        SVS_ServiceStatus.dwWin32ExitCode      = status; 
        SVS_ServiceStatus.dwServiceSpecificExitCode = specificError; 
 
        SetServiceStatus (SVS_ServiceStatusHandle, &SVS_ServiceStatus); 

//		g_strMessage = _T("服务初始化失败!\r\n");
//		AddToErrorLog(g_strMessage);
        return; 
    } 


    SVS_ServiceStatus.dwCurrentState       = SERVICE_RUNNING; 
    SVS_ServiceStatus.dwCheckPoint         = 0; 
    SVS_ServiceStatus.dwWaitHint           = 0; 
 
    if (!SetServiceStatus (SVS_ServiceStatusHandle, &SVS_ServiceStatus)) 
    { 
//		g_strMessage.Format("设置服务状态出错 %ld\r\n", GetLastError());
//		AddToErrorLog(g_strMessage);
		return;
    }

	HANDLE m_handle=GetCurrentProcess();
	DWORD iPriority=GetPriorityClass(m_handle);
    SetPriorityClass(m_handle,HIGH_PRIORITY_CLASS);
    HANDLE m_thread=GetCurrentThread();
	SetThreadPriority(m_thread,THREAD_PRIORITY_ABOVE_NORMAL);
//	AddToEventLog("服务启动成功!");

/*	while(!Check())
	{
		::Sleep(g_lCheckTime*1000*60);
	}*/


   if(!RunDB())
   {
        SVS_ServiceStatus.dwCurrentState       = SERVICE_STOPPED; 
        SVS_ServiceStatus.dwCheckPoint         = 0; 
        SVS_ServiceStatus.dwWaitHint           = 0; 
        SVS_ServiceStatus.dwWin32ExitCode      = status; 
        SVS_ServiceStatus.dwServiceSpecificExitCode = specificError; 
 
        SetServiceStatus (SVS_ServiceStatusHandle, &SVS_ServiceStatus); 

		::Sleep(1000);
		return ;

   }

	MSG msg;
	while(::GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}



}
DWORD SVS_ServiceInitialization(DWORD argc, LPTSTR *argv, DWORD *specificError) 
{ 
    argv; 
    argc; 
    specificError; 
    return(0); 
}

bool InstallService()
{
	char buf[2048]={0};

	sprintf(buf,"\"%s\" -service",g_dbPName.c_str());
	SC_HANDLE hs=OpenSCManager(NULL,SERVICES_ACTIVE_DATABASE,SC_MANAGER_ALL_ACCESS);
	if(hs==NULL)
	{
		puts("Open 'SCManager' failed");
		return false;
	}

	if(::CreateService(hs,g_ServiceName,g_ServiceDName,
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START,
		SERVICE_ERROR_NORMAL,
		buf,
		"",
		NULL,NULL,g_account,g_password)==NULL)
	{
		puts("Install service failed");
		DWORD err=::GetLastError();
		switch(err){
			case	ERROR_ACCESS_DENIED:
				puts("\t The handle to the specified service control manager database does not have SC_MANAGER_CREATE_SERVICE access.");
				break;
			case	ERROR_CIRCULAR_DEPENDENCY:
				puts("\t A circular service dependency was specified.");
				break;
			case	ERROR_DUP_NAME:
				puts("\t The display name already exists in the service control manager database either as a service name or as another display name.");
				break;
			case	ERROR_INVALID_HANDLE:
				puts("\t The handle to the specified service control manager database is invalid.");
				break;
			case	ERROR_INVALID_NAME:
				puts("\t The specified service name is invalid");
				break;
			case	ERROR_INVALID_PARAMETER:
				puts("\t A parameter that was specified is invalid");
				break;
			case	ERROR_INVALID_SERVICE_ACCOUNT:
				puts("\t The user account name specified in the lpServiceStartName parameter does not exist.");
				break;
			case	ERROR_SERVICE_EXISTS:
				puts("\tThe specified service already exists in this database.");
				break;
			default: puts("\t Unknown");
		}
		if(g_account!=NULL)
			free(g_account);
		if(g_password)
			free(g_password);
		return false;
	}
	else
		puts("Install service sucessed");

	if(g_account!=NULL)
		free(g_account);
	if(g_password)
		free(g_password);


	return true;
}

bool UnInstall()
{
	DWORD err=0;
	SC_HANDLE hsm=::OpenSCManager(NULL,SERVICES_ACTIVE_DATABASE,SC_MANAGER_ALL_ACCESS);
	if(hsm==NULL)
	{
		puts("Remove service failed");
		//			puts("原因:");

		puts("\t Open 'SCManger' failed");
		return false;
	}

	SC_HANDLE hsc=::OpenService(hsm,g_ServiceName,DELETE);
	if(hsc==NULL)
	{
		puts("Remove service failed");
		//			puts("原因:");

		err=::GetLastError();
		switch(err){
			case	ERROR_ACCESS_DENIED:
				puts("\t The specified service control manager ");
				break;
			case	ERROR_INVALID_HANDLE:
				puts("\t The specified handle is invalid");
				break;
			case	ERROR_INVALID_NAME:
				puts("\t The specified service name is invalid");
				break;
			case	ERROR_SERVICE_DOES_NOT_EXIST:
				puts("\t The specified service does not exist");
				break;
			default:	puts("\t Unknown error with 'OpenService'");
		}
		return false;
	}

	if(::DeleteService(hsc)==0)
	{
		puts("Remove service failed");
		err=::GetLastError();
		switch(err){
			case	ERROR_ACCESS_DENIED:
				puts("\t The specified handle was not opened with DELETE access");
				break;
			case	ERROR_INVALID_HANDLE:
				puts("\t The specified handle is invalid");
				break;
			case	ERROR_SERVICE_MARKED_FOR_DELETE:
				puts("\t The specified service has already been marked for deletion");
				break;
			default :puts("\t Unknown error");
		}

		return false;
	}
	puts("Remove serivce successed");
	return true;
}

#endif

void RunSvdbServices()
{
#ifdef	WIN32
    SERVICE_TABLE_ENTRY ste[] = 
	{
		{"SVDB_Server",	SVS_ServiceStart}, 
		{ NULL,			NULL			} 
	}; 

	::StartServiceCtrlDispatcher(ste);

#else
	;
#endif
}

#ifdef WIN32

bool RunProcess(PROCESS_INFORMATION*pi,string ProcessName)
{
//	CString strCommandLine=_T("");

    STARTUPINFO si;
  //  PROCESS_INFORMATION pi;
	string strDir=GetRootPathFromKeyCore();
	strDir+="\\fcgi-bin\\";

	DWORD dcid=::GetCurrentProcessId();

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( pi, sizeof(PROCESS_INFORMATION) );


//	strCommandLine.Format("%s\\fcgi-bin\\%s",GetRootPathFromKeyCore(),(LPCSTR)ProcessName);


	int n=0;
	bool bRet=true;

	while(!::CreateProcess(NULL,
		(LPTSTR)ProcessName.c_str(),
		NULL,
		NULL,
		FALSE,
		/*CREATE_NO_WINDOW*/CREATE_NEW_CONSOLE,
		NULL,
		(LPCTSTR)strDir.c_str(),
		&si,pi))
	{
		::Sleep(3000);
		n++;
		if(n==4)
		{
			char buf[256]={0};
			bRet=false;
			sprintf(buf,"Run process:%s failed,error id:%d\n",ProcessName.c_str(),GetLastError());
			AddToErrorLog(buf);
			break;
		}

		DWORD dw=GetLastError();
		printf("error:%d\n",dw);
	}

	return bRet;

}

DWORD WINAPI WatchDBProcess(LPVOID lpParam )
{
	if(g_dbPName.empty())
		return 1;

	 ZeroMemory(&g_dbpi,sizeof(PROCESS_INFORMATION));

	 AddToErrorLog(g_dbPName);

	 if(!::RunProcess(&g_dbpi,g_dbPName))
	 {
//		 AddToErrorLog("Run Alert process failed!");
		 return 1;
	 }

	 ::WaitForSingleObject(g_dbpi.hProcess,INFINITE);

	 ::Sleep(2000);

	 ::CloseHandle(g_dbpi.hProcess);
	 ::CloseHandle(g_dbpi.hThread);

LOOP:

	 if(g_SubProcessState&&(!g_Disabled))
	 {
		 ::Sleep(2000);
		 DWORD dw=0;
		 for(int i=0;i<6;i++)
		 {
			 HANDLE hd=::CreateThread(NULL,0,&WatchDBProcess,NULL,0,&dw);
			 if(hd)
			 {
//				 AddToEventLog("Start watch report thread ok");
				 ::CloseHandle(hd);
				 break;
			 }
			 else if(hd==NULL&&i==5)
			 {
//				 AddToErrorLog("Start watch report thread failed");
				 break;
			 }
		 }
	 }else
	 {
		 Sleep(60*1000);
		 goto LOOP;
	 }

	return 1;

}

#endif


bool Init(void)
{
	if(hasrun())
		return false;

#ifdef WIN32
	DWORD dw;
	for(int i=0;i<3;i++)
	{
		HANDLE hd=::CreateThread(NULL,0,&WatchDBProcess,NULL,0,&dw);
		if(hd)
		{
			::CloseHandle(hd);
			break;
		}
		else if(hd==NULL&&i==2)
		{
			return FALSE;
		}
	}

#endif
	return true;
}

bool RunDB(void)
{
	return Init();
}
