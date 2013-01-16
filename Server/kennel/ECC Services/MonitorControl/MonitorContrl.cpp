// MonitorContrl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MonitorContrl.h"
#include "winsvc.h"
#include "atltime.h"
//#include "..\base\funcGeneral.h"
#include <svdb/svapi/svdbapi.h>
#include <svdb/libutil/Time.h>
#include <svdb/libutil/buffer.h>
#include <string>
#include <set>
#include <map>

#ifdef _DEBUG
#pragma comment(lib,"../debug/IODll.lib")
#pragma comment(lib,"../IODll/ssleay32.lib")
#pragma comment(lib,"../IODll/libeay32.lib")
#else
#pragma comment(lib,"./release/IODll.lib")
#pragma comment(lib,"./ssleay32.lib")
#pragma comment(lib,"./libeay32.lib")
#endif


#include "PSAPI.h"
#pragma comment( lib, "PSAPI.LIB" )


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

//#define DEBUG_T

#define		WM_COMMUNICATION	WM_USER+135


CWinApp theApp;

using namespace std;

BOOL Run(void);
BOOL Init(void);
BOOL AddToErrorLog(CString strError);
BOOL AddToEventLog(CString strMessage);
BOOL RunMonitorExecutant(PROCESS_INFORMATION*pi);
CString GetRootPath(void);
BOOL Check(void);

BOOL RunRefresh(string queuename,string label,string address);
BOOL RunProcess(PROCESS_INFORMATION*pi,CString ProcessName,bool islocal=false);

DWORD GetProcessMemSize(DWORD ProcessID);

HANDLE g_hCom=NULL;
HANDLE g_hReLoadConfig=NULL;
HANDLE g_hExit=NULL;
HANDLE g_hReloadEvent=NULL;

PROCESS_INFORMATION g_pi;
PROCESS_INFORMATION g_alertpi;
PROCESS_INFORMATION g_reportpi;

#include "base/des.h"
#include "SvDog.h"
#include "DllCall.h"
#include "license.h"
#pragma comment(lib, "doglib.lib")

long	g_lSubRestartTime=0;   //定时重启监测进程的时间间隔(分钟);
UINT	g_uCheckTime=0;        //和监测进程通讯的时间间隔(分钟); 
UINT	g_uExitTimeOut=1;      //终止子进程时要等待的时间(分钟);
CString  g_strMessage=_T("");
long	g_lCheckTime=5;       //检查是否过期的时间间隔(分钟)
unsigned long g_nLastCheckTime; // 保存上次检查的时间
int		g_iStartType=1;       //1为启动MonitorExecutent.exe,2为启动MonitorSchedule.exe
DWORD   g_uMaxMemory=500;     //允许监测进程占用的最大内存数(物理内存单位M);
CString g_strDBAddress="localhost"; //队列服务器地址;
int		g_seid=1;					//seid;

bool    g_bool_watch_AlertServer=true;		//是否守护监测 AlertServer
bool    g_bool_watch_ReportGenerate=true;	//是否守护监测 ReportGenerate
bool    g_bool_watch_Monitorshedule=true;	//是否守护监测 Monitorshedule
std::set <std::string> exestring;			//被守护监测的 exe 列表

CString g_AlertProcessName="AlertServer.exe";
CString g_ReportProcessName="reportgenerate.exe";
CString g_strRefreshQueueName="SiteView70_RefreshInfo";

char *g_account=NULL;
char *g_password=NULL;
string g_mcPName="";

const char *g_ServiceDName="SiteView_Watch";
const char *g_ServiceName="SiteView_Watch";


BOOL	g_Disabled=FALSE;
BOOL g_SubProcessState=TRUE;

SERVICE_STATUS          SVS_ServiceStatus; 
SERVICE_STATUS_HANDLE   SVS_ServiceStatusHandle;

DWORD SVS_ServiceInitialization(DWORD argc, LPTSTR *argv, DWORD *specificError);


std::string TrimSpace(const std::string & input)
{
	string ret=input;
	try{
					
		while( ret.find(" ")==0 )
			ret.erase(0,1);
		if(ret.empty())
			return ret;
		std::string::size_type lastpos;
		while( ret.rfind(" ")==(lastpos=(ret.size()-1)) )
			ret.erase(lastpos,1);
	}
	catch(...)
	{
		ret= input;
	}
	return ret;
}

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

			AddToEventLog("服务退出正终止子进程");

 
            if(!SetServiceStatus(SVS_ServiceStatusHandle, &SVS_ServiceStatus))
            { 
				g_strMessage.Format("设置服务状态出错: %ld\r\n", GetLastError());
				AddToErrorLog(g_strMessage);
            } 
			
            return; 
 
        case SERVICE_CONTROL_INTERROGATE: 
        // Fall through to send current status. 
//			AddToErrorLog("Service Control Interrogate\r\n");
            break; 
 
        default:
			g_strMessage.Format("Unrecognized opcode %ld\r\n", opcode);
			AddToErrorLog(g_strMessage);
			break;
    } 

    if (!SetServiceStatus (SVS_ServiceStatusHandle,  &SVS_ServiceStatus)) 
    { 
		g_strMessage.Format("设置服务状态出错: %ld\r\n", GetLastError());
	
		AddToErrorLog(g_strMessage);
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

	SVS_ServiceStatusHandle=::RegisterServiceCtrlHandler("VALIDATESERVER",SVS_ServiceCtrlHandler);
    if (SVS_ServiceStatusHandle == (SERVICE_STATUS_HANDLE)0) 
    { 
		g_strMessage.Format("RegisterServiceCtrlHandler failed %d\r\n", GetLastError());
       		AddToErrorLog(g_strMessage);
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

		g_strMessage = _T("服务初始化失败!\r\n");
		AddToErrorLog(g_strMessage);
        return; 
    } 


    SVS_ServiceStatus.dwCurrentState       = SERVICE_RUNNING; 
    SVS_ServiceStatus.dwCheckPoint         = 0; 
    SVS_ServiceStatus.dwWaitHint           = 0; 
 
    if (!SetServiceStatus (SVS_ServiceStatusHandle, &SVS_ServiceStatus)) 
    { 
		g_strMessage.Format("设置服务状态出错 %ld\r\n", GetLastError());
		AddToErrorLog(g_strMessage);
		return;
    }

	HANDLE m_handle=GetCurrentProcess();
	DWORD iPriority=GetPriorityClass(m_handle);
    SetPriorityClass(m_handle,HIGH_PRIORITY_CLASS);
    HANDLE m_thread=GetCurrentThread();
	SetThreadPriority(m_thread,THREAD_PRIORITY_ABOVE_NORMAL);
	AddToEventLog("服务启动成功!");

	// 启动线程前服务过期检查
	g_nLastCheckTime = ::GetTickCount();// 保存第一次检查的时间
	while( !Check() )
	{
		::Sleep( g_lCheckTime*1000*60 );
	}


    Run();

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

	sprintf(buf,"\"%s\" -service",g_mcPName.c_str());
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


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
     	return nRetCode;
	}
//#ifndef DEBUG_T

	g_mcPName=argv[0];

	if(argc>1)
	{
		if(::_stricmp(argv[1],"-service")==0)
		{
			SERVICE_TABLE_ENTRY ste[] = 
			{
				{"MonitorContrl",	SVS_ServiceStart}, 
				{ NULL,			NULL			} 
			}; 

			::StartServiceCtrlDispatcher(ste);
		}
		else if(::_stricmp(argv[1],"-install")==0)
		{
			if(argc==4)
			{
				g_account=_strdup(argv[2]);
				g_password=_strdup(argv[3]);
			}

		    InstallService();
		}
		else if(::_stricmp(argv[1],"-uninstall")==0)
			UnInstall();
		else
			puts("Invalid parameter");

		return 1;
	}

//#else


	Run();

	MSG msg;
	while(::GetMessage(&msg,NULL,0,0))
	{
		puts("Got Message");
		TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}
	::Sleep(100000);

//#endif


	return nRetCode;
}
DWORD WINAPI ThreadFunc( LPVOID lpParam )
{
	 ZeroMemory(&g_pi,sizeof(PROCESS_INFORMATION));

	 if(!RunMonitorExecutant(&g_pi))
	 {
		 AddToErrorLog("Run MonitorSchedule process failed");
		 return 1;
	 }

	 ::WaitForSingleObject(g_pi.hProcess,INFINITE);
	 ::ResetEvent(g_hExit);

	 ::Sleep(2000);

	 ::CloseHandle(g_pi.hProcess);
	 ::CloseHandle(g_pi.hThread);

LOOP:

	 if(g_SubProcessState&&(!g_Disabled))
	 {
		 ::Sleep(2000);
		 DWORD dw=0;
		 for(int i=0;i<6;i++)
		 {
			 HANDLE hd=::CreateThread(NULL,0,&ThreadFunc,NULL,0,&dw);
			 if(hd)
			 {
				 AddToEventLog("Succeeded to start thread of watch MonitorSchedule _2 .");
				 ::CloseHandle(hd);
				 break;
			 }
			 else if(hd==NULL&&i==5)
			 {
				 AddToErrorLog("Failed to start thread of watch MonitorSchedule _2 .");
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
DWORD WINAPI WatchAlertProcess(LPVOID lpParam )
{
	 ZeroMemory(&g_alertpi,sizeof(PROCESS_INFORMATION));

	 if(!::RunProcess(&g_alertpi,g_AlertProcessName))
	 {
		 AddToErrorLog("Run Alert process failed!");
		 return 1;
	 }

	 ::WaitForSingleObject(g_alertpi.hProcess,INFINITE);

	 ::Sleep(2000);

	 ::CloseHandle(g_alertpi.hProcess);
	 ::CloseHandle(g_alertpi.hThread);

LOOP:

	 if(g_SubProcessState&&(!g_Disabled))
	 {
		 ::Sleep(2000);
		 DWORD dw=0;
		 for(int i=0;i<6;i++)
		 {
			 HANDLE hd=::CreateThread(NULL,0,&WatchAlertProcess,NULL,0,&dw);
			 if(hd)
			 {
				 AddToEventLog("Start watch alert thread ok");
				 ::CloseHandle(hd);
				 break;
			 }
			 else if(hd==NULL&&i==5)
			 {
				 AddToErrorLog("Start watch alert thread failed");
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
DWORD WINAPI WatchReportProcess(LPVOID lpParam )
{
	 ZeroMemory(&g_reportpi,sizeof(PROCESS_INFORMATION));

	 if(!::RunProcess(&g_reportpi,g_ReportProcessName))
	 {
		 AddToErrorLog("Run Report process failed!");
		 return 1;
	 }

	 ::WaitForSingleObject(g_reportpi.hProcess,INFINITE);

	 ::Sleep(2000);

	 ::CloseHandle(g_reportpi.hProcess);
	 ::CloseHandle(g_reportpi.hThread);

LOOP:

	 if(g_SubProcessState&&(!g_Disabled))
	 {
		 ::Sleep(2000);
		 DWORD dw=0;
		 for(int i=0;i<6;i++)
		 {
			 HANDLE hd=::CreateThread(NULL,0,&WatchReportProcess,NULL,0,&dw);
			 if(hd)
			 {
				 AddToEventLog("Start watch report thread ok");
				 ::CloseHandle(hd);
				 break;
			 }
			 else if(hd==NULL&&i==5)
			 {
				 AddToErrorLog("Start watch report thread failed");
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


DWORD WINAPI WatchUnivProcess(LPVOID lpParam )
{
	int index;
	try{
		if(lpParam==NULL)
			return 1;
		index= (int)lpParam;
	}
	catch(...)
	{
		return 1;
	}

	std::string currentExeName;
	int sindex=0;
	std::set<std::string>::iterator sit;
	for(sit=exestring.begin(); sit!=exestring.end(); ++sit)
	{
		++sindex;
		if(sindex==index)
		{
			currentExeName= *sit;
			break;
		}
	}
	if(sindex!=index)
		return 1;

	PROCESS_INFORMATION nowpi;
	ZeroMemory(&nowpi,sizeof(PROCESS_INFORMATION));

	CString text=currentExeName.c_str();
	if(!::RunProcess(&nowpi,currentExeName.c_str(),true))
	{
		AddToErrorLog("Run process failed: "+text);
		return 1;
	}

	::WaitForSingleObject(nowpi.hProcess,INFINITE);

	::Sleep(2000);

	::CloseHandle(nowpi.hProcess);
	::CloseHandle(nowpi.hThread);

LOOP:

	 if(g_SubProcessState&&(!g_Disabled))
	 {
		 ::Sleep(2000);
		 DWORD dw=0;
		 for(int i=0;i<6;i++)
		 {
			 HANDLE hd=::CreateThread(NULL,0,&WatchUnivProcess,(LPVOID)index,0,&dw);
			 if(hd)
			 {
				 puts(" ");
				 AddToEventLog("Start watch thread ok: "+text);
				 ::CloseHandle(hd);
				 break;
			 }
			 else if(hd==NULL&&i==5)
			 {
				 puts(" ");
				 AddToErrorLog("Start watch thread failed: "+text);
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

DWORD WINAPI CommThreadFunc( LPVOID lpParam )
{
	CTime oldtime=CTime::GetCurrentTime();
	CTime curtime=oldtime;
	CTime lastchecktime=oldtime;
	CTimeSpan st,sct;

	while(1)
	{
		curtime=CTime::GetCurrentTime();
		st=curtime-oldtime;
		sct=curtime-lastchecktime;
		if(sct.GetTotalMinutes()>=g_lCheckTime)
		{
			puts("Inspect MonitorSchedule... ");
			OutputDebugString("Inspect MonitorSchedule... \n");

			lastchecktime=curtime;

			if( !Check() )
			{
				//---------------------------------------------------------------
				// 修改内容：认证失败时，结束并禁止子进程
				// 修改人：邹晓
				// 修改日期：2009.03.12
				//---------------------------------------------------------------
				g_SubProcessState=FALSE;

				if( g_alertpi.hProcess != NULL )
				{
					::PostThreadMessage( g_alertpi.dwThreadId, WM_QUIT, 0, 0 );
					::Sleep(1000);
					::TerminateProcess( g_alertpi.hProcess, 3 );
				}

				if( g_reportpi.hProcess != NULL )
				{
					::PostThreadMessage( g_reportpi.dwThreadId, WM_QUIT, 0, 0 );
					::Sleep(1000);
					::TerminateProcess( g_reportpi.hProcess, 3 );
				}

				if( g_pi.hProcess != NULL )
				{
					::PostThreadMessage( g_pi.dwThreadId, WM_QUIT, 0, 0 );
					::Sleep(1000);
					::TerminateProcess( g_pi.hProcess, 3 );
				}
				
				
				AddToEventLog( "ecc服务已过期，结束进程monitorSchedule、alertServer和report！" );

				::Sleep( g_lCheckTime*60*1000 );
				
				continue;
				
			}
			else
			{
				g_SubProcessState = true;
			}


			g_Disabled=FALSE;

			if(GetProcessMemSize(g_pi.dwProcessId)>g_uMaxMemory)
			{
				AddToEventLog(" MonitorSchedule 内存过高，将退出");
				goto EXIT;
			}

			if( GetProcessMemSize(g_alertpi.dwProcessId) > 100 )
			{
				AddToEventLog( "alertServer 内存过高，将退出" );

				if( g_alertpi.hProcess != NULL )
				{
					::PostThreadMessage( g_alertpi.dwThreadId, WM_QUIT, 0, 0 );
					::Sleep(1000);
					::TerminateProcess( g_alertpi.hProcess, 3 );
				}
				
			}

		}

		if(st.GetTotalMinutes()>=g_lSubRestartTime)
		{
			AddToEventLog(" MonitorSchedule 到达设定重启时间");
//			::SetEvent(g_hExit);
//			::SetEvent(g_hCom);
EXIT:
			::PostThreadMessage(g_pi.dwThreadId,WM_QUIT,(WPARAM)0,(LPARAM)0);
//			::Sleep(2000);

			oldtime=curtime;
			if(g_pi.hProcess==NULL)
			    continue;

			if(::WaitForSingleObject(g_pi.hProcess,g_uExitTimeOut*1000*60)==WAIT_TIMEOUT)
			{
				AddToEventLog("程序退出超时将强行剔除 MonitorSchedule 进程!");
				::TerminateProcess(g_pi.hProcess,2);
			}

		}

		if(!g_Disabled)
		{
			::Sleep(g_uCheckTime*60*1000);
			puts("Try communicate with MonitorSchedule ...");
			OutputDebugString("Try communicate with MonitorSchedule ...\n");

			bool willtermi= true;
			for(int t=1; t<=3; ++t)
			{
				::SetEvent(g_hCom);
				for(int i=1; i<=5; ++i)
					if( ::PostThreadMessage(g_pi.dwThreadId,WM_COMMUNICATION,(WPARAM)0,(LPARAM)0) )
						break;
				::Sleep(2000);

				if(::WaitForSingleObject(g_hCom,1)==WAIT_TIMEOUT)
				{
					willtermi= false;
					break;
				}
				::Sleep(3000);
			}
			if(willtermi)
			{
				AddToEventLog("通信失败将强行剔除 MonitorSchedule 进程");
				::TerminateProcess(g_pi.hProcess,1);
				::Sleep(20000);
				::SetEvent(g_pi.hProcess);
			}
		}
		else
			Sleep(60*1000);
			
			
	}



	return 1;
}
/***************************************************************
只所以多此一个线程而不直接让ReloadConfig.exe
用g_hReLoadConfig直接跟MonitorExecult.exe
通信是为了将来更好的扩展控制如累积加载等。
***************************************************************/
DWORD WINAPI ReloadConfigThreadFunc( LPVOID lpParam )
{
      while(1)
	  {
		  ::WaitForSingleObject(g_hReloadEvent,INFINITE);
		  while(::WaitForSingleObject(g_hReLoadConfig,1)==WAIT_OBJECT_0)
			  ::Sleep(1000);
		  ::SetEvent(g_hReLoadConfig);
		  ::ResetEvent(g_hReloadEvent);
		  AddToEventLog("正在重新加载配置文件!");

	  }

	  return 1;

}

void WriteLog( const char* str )
{
	//	return;

	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	char szProgramName[] = "dyMonitorControl.log";
	//	string	strEccPath = GetSiteViewRootPath();
	string	strEccPath;
	char	strPath[128] = {0};
	::GetModuleFileName(NULL , strPath , 128);
	strEccPath += strPath;
	strEccPath = strEccPath.substr(0 , strEccPath.find_last_of("\\" , strEccPath.length()));
	char szLogFile[128];
	//	sprintf( szLogFile, "%s\\itsm\\runtime\\logs\\%s", strEccPath.c_str(), szProgramName );
	sprintf( szLogFile , "%s\\%s" , strEccPath.c_str() , szProgramName);

	// 判断文件大小：在不打开文件的情况下实现
	
	FILE *log = fopen( szLogFile,"a+");
	if( log != NULL )
	{
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fclose( log );
	}

	return;
}

DWORD WINAPI CheckRefreshThread( LPVOID lpParam )
{
	string label;
	svutil::TTime ct;
	unsigned int len=0;
	string oldlabel;

	int n=0;
	char logBuf[1024] = {0}; // dy

	if(!::CreateQueue(g_strRefreshQueueName.GetBuffer(0),1,"default",g_strDBAddress.GetBuffer(0)))
		printf("Create queue failed: %s\n",g_strRefreshQueueName.GetBuffer(0));

	::ClearQueueMessage(g_strRefreshQueueName.GetBuffer(0),"default",g_strDBAddress.GetBuffer(0));
	printf("Refresh queue:%s,addr:%s\n",g_strRefreshQueueName.GetBuffer(0),g_strDBAddress.GetBuffer(0));

	sprintf(logBuf, "Refresh queue:%s,addr:%s\n",g_strRefreshQueueName.GetBuffer(0),g_strDBAddress.GetBuffer(0));
	WriteLog(logBuf);
	
	while(true)
	{
		MQRECORD mrd=::BlockPeekMQMessage(g_strRefreshQueueName.GetBuffer(0),"default",g_strDBAddress.GetBuffer(0));
		//MQRECORD mrd=::PeekMQMessage(g_strRefreshQueueName.GetBuffer(0),0,"default",g_strDBAddress.GetBuffer(0)); //携程需要非阻塞模式
		if(mrd==INVALID_VALUE)
		{
			printf("queue name:%s,address:%s\n",g_strRefreshQueueName.GetBuffer(0),g_strDBAddress.GetBuffer(0));
			
			memset(logBuf, '\0' ,sizeof(logBuf));
			sprintf(logBuf, "queue name:%s,address:%s\n",g_strRefreshQueueName.GetBuffer(0),g_strDBAddress.GetBuffer(0));
			WriteLog(logBuf);
			WriteLog("Peek failed");


			puts("Peek failed");
			::Sleep(10*1000);
			continue;
		}
		if(!::GetMessageData(mrd,label,ct,NULL,len))
		{
			WriteLog("Get message data failed");

			puts("Get message data failed");
			::CloseMQRecord(mrd);
			continue;
		}
		if(label.compare("DYNPARAM")==0)
		{
			WriteLog("In DYNPARAM");

			puts("In DYNPARAM");
			svutil::buffer buf;
			if(!buf.checksize(len))
			{
				::CloseMQRecord(mrd);
				continue;
			}

			if(!::GetMessageData(mrd,label,ct,buf,len))
			{

				::CloseMQRecord(mrd);
				continue;
			}

			::CloseMQRecord(mrd);


			PROCESS_INFORMATION pi;
			ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));
			CString strparap;
			strparap.Format("slavedynparam.exe %s",(char *)buf);
			printf("Process name:%s\n",strparap.GetBuffer(0));


			RunProcess(&pi,strparap);

			MQRECORD mddd=::PopMessage((LPCSTR)g_strRefreshQueueName,0,"default",(LPCSTR)g_strDBAddress);
			if(mddd!=INVALID_VALUE)
				::CloseMQRecord(mddd);

			continue;
		}
		::CloseMQRecord(mrd);

		printf("oldlabel:%s,label:%s\n",oldlabel.c_str(),label.c_str());

		memset(logBuf, '\0' ,sizeof(logBuf));
		sprintf(logBuf,"oldlabel:%s,label:%s\n",oldlabel.c_str(),label.c_str());
		WriteLog(logBuf);

		if(label.compare(oldlabel)==0)
		{
			n++;
			::Sleep(3*1000);
			if(n>3)
			{
				MQRECORD mddd=::PopMessage((LPCSTR)g_strRefreshQueueName,0,"default",(LPCSTR)g_strDBAddress);
				if(mddd!=INVALID_VALUE)
					::CloseMQRecord(mddd);
			}
			continue;
		}

		n=0;
		oldlabel="";



		if(!RunRefresh((LPCSTR)g_strRefreshQueueName,label,(LPCSTR)g_strDBAddress))
		{
			WriteLog("run failed");

			puts("run failed");
			char buf[256]={0};
			sprintf(buf,"Start monitorschedule.exe failed");
			::PushMessage(label,"Refresh_ERROR",buf,strlen(buf)+1,"default",(LPCSTR)g_strDBAddress);
			::ClearQueueMessage(g_strRefreshQueueName.GetBuffer(0),"default",g_strDBAddress.GetBuffer(0));
		}
		else
		{
			WriteLog("run ok");
			puts("run ok");
		}

		oldlabel=label;

		::Sleep(3*1000);   //delay

	}

}

BOOL Init(void)
{

	g_hCom=::CreateEvent(NULL,TRUE,FALSE,"Global\\Siteview-Communictions");
	if(g_hCom==NULL)
	{
		AddToErrorLog("创建跟子进程通信事件失败\n");
		return FALSE;
	}
	if(::GetLastError()==ERROR_ALREADY_EXISTS)
	{
		AddToErrorLog("该进程已经存在!");
		return FALSE;
	}
/*	g_hReLoadConfig=::CreateEvent(NULL,TRUE,FALSE,"Global\\Siteview-ReloadConfigEvent");

	if(g_hReLoadConfig==NULL)
	{
		AddToErrorLog("创建重新加载配置文件事件失败!");
		return FALSE;
	}
	g_hExit=::CreateEvent(NULL,TRUE,FALSE,"Global\\Siteview-SubExit");

	if(g_hExit==NULL)
	{
		AddToErrorLog("创建子进程退出事件失败!");
		return FALSE;
	}

	g_hReloadEvent=::CreateEvent(NULL,TRUE,FALSE,"Global\\Siteview-ReloadEvent");

	if(g_hReloadEvent==NULL)
	{
		AddToErrorLog("创建重置文件事件失败!");
		return FALSE;
	}*/

	g_SubProcessState=TRUE;

	CString strPath=_T("");
	strPath.Format("%s\\fcgi-bin\\mc.config",GetRootPath());

	printf(strPath+"\n");

	g_bool_watch_AlertServer=	(::GetPrivateProfileInt("EnableWatch","AlertServer",1,strPath)		!=0);
	g_bool_watch_ReportGenerate=(::GetPrivateProfileInt("EnableWatch","reportgenerate",1,strPath)	!=0);
	g_bool_watch_Monitorshedule=(::GetPrivateProfileInt("EnableWatch","MonitorSchedule",1,strPath)	!=0);
	
	printf("\nEnable watching AlertServer: %d\n", g_bool_watch_AlertServer);
	printf("Enable watching reportgenerate: %d\n", g_bool_watch_ReportGenerate);
	printf("Enable watching MonitorSchedule: %d\n", g_bool_watch_Monitorshedule);

	char buf[256]={0};
	::GetPrivateProfileString("ExeToBeWatched","exes","",buf,255,strPath);
	if(strlen(buf)>1)
	{
		try{
			std::string exename;
			char * token = strtok(  buf, "," );
			while( token != NULL )
			{
				exename=TrimSpace(token);
				if( !exename.empty() )
					exestring.insert( exename );
				token = strtok( NULL, "," );
			}
		}
		catch(...)
		{
			printf("Exception to strtok  ExeToBeWatched \n");
		}
	}
	std::set<std::string>::iterator sit;
	for(sit=exestring.begin();  sit!=exestring.end();  ++sit)
		printf("Exe to be Watched: \"%s\"\n",sit->c_str());
	
	printf("\n");

	g_lSubRestartTime=::GetPrivateProfileInt("information","RestartTime",240,strPath);

	if(g_lSubRestartTime==0)
	{
		AddToErrorLog("配置文件有错!");
		return FALSE;
	}
	printf("Restart time: %d\n",g_lSubRestartTime);

	g_uCheckTime=::GetPrivateProfileInt("information","CheckTime",2,strPath);

	if(g_uCheckTime==0)
	{
		AddToErrorLog("配置文件有错!");
		return FALSE;
	}
	printf("Check time: %d\n",g_uCheckTime);

	g_uExitTimeOut=::GetPrivateProfileInt("information","ExitTimeOut",1,strPath);

//	if(g_uExitTimeOut==0)
 //        g_uExitTimeOut=1;

	g_iStartType=::GetPrivateProfileInt("information","StartType",2,strPath);
//	if(g_iStartType==0)
//		g_iStartType=1;

	g_uMaxMemory=::GetPrivateProfileInt("information","MaxMemory",500,strPath);

	if(g_uMaxMemory<20)
		g_uMaxMemory=20;

	::GetPrivateProfileString("information","AlertProcess","",buf,255,strPath);
	if(strlen(buf)>1)
		g_AlertProcessName=buf;

	memset(buf,0,256);
	::GetPrivateProfileString("information","ReportProcess","",buf,255,strPath);
	if(strlen(buf)>1)
		g_ReportProcessName=buf;

	memset(buf,0,256);
	::GetPrivateProfileString("information","RefreshQueueName","",buf,255,strPath);
	if(strlen(buf)>1)
		g_strRefreshQueueName=buf;

	memset(buf,0,256);
	::GetPrivateProfileString("MonitorSchedule","ServerHost","",buf,255,strPath);
	if(strlen(buf)>1)
		g_strDBAddress=buf;

	memset(buf,0,256);
	::GetPrivateProfileString("MonitorSchedule","SEID","",buf,255,strPath);
	if(strlen(buf)>0)
		g_seid=atoi(buf);

	char strr[1024]={0};
	sprintf(strr,"%s_%d",g_strRefreshQueueName.GetBuffer(1),g_seid);

	g_strRefreshQueueName=strr;


	return TRUE;
}
BOOL Run(void)
{
	if(!Init())
	{
		AddToErrorLog("初始化失败!");
		return FALSE;
	}

	int i=0;

	DWORD dw;

	if(g_bool_watch_Monitorshedule)
	{
		for(i=0;i<6;i++)
		{
			HANDLE hd=::CreateThread(NULL,0,&CommThreadFunc,NULL,0,&dw);
			if(hd)
			{
				AddToEventLog("Succeeded to start thread to communicate with MonitorSchedule .");
				::CloseHandle(hd);
				break;
			}
			else if(hd==NULL&&i==5)
			{
				AddToErrorLog("Failed to start thread to communicate with MonitorSchedule .");
				return FALSE;
			}
		}

		for(i=0;i<6;i++)
		{
			HANDLE ht=::CreateThread(NULL,0,&ThreadFunc,NULL,0,&dw);
			if(ht)
			{
				AddToEventLog("Succeeded to start thread of watch MonitorSchedule .");
				::CloseHandle(ht);
				break;
			}
			else if(ht==NULL&&i==5)
			{
				AddToErrorLog("Failed to start thread of watch MonitorSchedule .");
				return FALSE;
			}
		}

		for(i=0;i<6;i++)
		{
			HANDLE hr=::CreateThread(NULL,0,&CheckRefreshThread,NULL,0,&dw);
			if(hr)
			{
				AddToEventLog("Start refresh check thread succeed");
				::CloseHandle(hr);
				break;
			}
			else if(hr==NULL&&i==5)
			{
				AddToErrorLog("Start refresh check thread failed");
				return FALSE;
			}
		}
	}

	//if(g_seid==1)
	{
		if(g_bool_watch_AlertServer)
		{
			for(i=0;i<6;i++)
			{
				HANDLE ha=::CreateThread(NULL,0,&WatchAlertProcess,NULL,0,&dw);
				if(ha)
				{
					AddToEventLog("Start Watch alert thread succeed");
					::CloseHandle(ha);
					break;
				}
				else if(ha==NULL&&i==5)
				{
					AddToErrorLog("Start Watch alert thread failed");
					return FALSE;
				}
			}
		}

		if(g_bool_watch_ReportGenerate)
		{
			for(i=0;i<6;i++)
			{
				HANDLE hrt=::CreateThread(NULL,0,&WatchReportProcess,NULL,0,&dw);
				if(hrt)
				{
					AddToEventLog("Start watch report thread succeed");
					::CloseHandle(hrt);
					break;
				}
				else if(hrt==NULL&&i==5)
				{
					AddToErrorLog("Start watch report thread failed");
					return FALSE;
				}
			}
		}
	}

	int sindex=0;
	std::set<std::string>::iterator sit;
	for(sit=exestring.begin();  sit!=exestring.end();  ++sit)
	{
		++sindex;
		for(i=0;i<6;i++)
		{
			CString text= "to start watch thread: ";
			text+= sit->c_str();
			HANDLE hrt=::CreateThread(NULL,0,&WatchUnivProcess,(LPVOID)sindex,0,&dw);
			if(hrt)
			{
				AddToEventLog("Succeeded " + text );
				::CloseHandle(hrt);
				break;
			}
			else if(hrt==NULL&&i==5)
			{
				AddToErrorLog("Failed " + text );
				return FALSE;
			}
		}
	}
	
	/*	for(i=0;i<6;i++)
	{
		HANDLE hr=::CreateThread(NULL,0,&ReloadConfigThreadFunc,NULL,0,&dw);
		if(hr)
		{
			AddToEventLog("启动重载配置文件线程成功");
			::CloseHandle(hr);
			break;
		}
		else if(hr==NULL&&i==5)
		{
			AddToErrorLog("启动重载配置文件线程失败!");
			return FALSE;
		}
	}
*/
/*
#ifndef DEBUG_T

	MSG msg;
	while(::GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

#endif
	*/



	return TRUE;
}
BOOL AddToErrorLog(CString strError)
{
	CString strTemp=_T(""),
		strFile=_T("");
	strFile.Format("%s\\temp\\mcerror.log",GetRootPath());

	CTime time=CTime::GetCurrentTime();
	strTemp=time.Format("%Y-%m-%d %H:%M:%S");

	strTemp+="\t"+strError+"\r\n";

//#ifndef DEBUG_T
	FILE *fp=NULL;
	fp=::fopen(strFile,"a+");
	if(fp==NULL)
	{
		printf("打开日志文件失败!\n");
		return FALSE;
	}
	::fputs(strTemp,fp);
	::fclose(fp);
//#else
	printf("%s",strTemp);
//#endif
	
	return TRUE;
}
BOOL AddToEventLog(CString strMessage)
{
	CString strTemp=_T(""),
		strFile=_T("");
	strFile.Format("%s\\temp\\mcevent.log",GetRootPath());


	CTime time=CTime::GetCurrentTime();
	strTemp=time.Format("%Y-%m-%d %H:%M:%S");

	strTemp+="\t"+strMessage+"\r\n";
#ifndef DEBUG_T	
	FILE *fp=::fopen(strFile,"a+");
	if(fp==NULL)
	{
		printf("打开日志文件失败!\n");
		return FALSE;
	}
	::fputs(strTemp,fp);
	::fclose(fp);
#else
	printf("%s",strTemp);
#endif
	
	return TRUE;
}
BOOL RunMonitorExecutant(PROCESS_INFORMATION*pi)
{
	CString strCommandLine=_T("");

    STARTUPINFO si;
  //  PROCESS_INFORMATION pi;
	CString strDir=GetRootPath();
	strDir+="\\fcgi-bin\\";

	DWORD dcid=::GetCurrentProcessId();

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( pi, sizeof(PROCESS_INFORMATION) );


	if(g_iStartType==1)
		strCommandLine.Format("%s\\fcgi-bin\\MonitorExecutant.exe %d",GetRootPath(),dcid);
    else
		strCommandLine.Format("%s\\fcgi-bin\\MonitorSchedule.exe",GetRootPath());


	int n=0;
	BOOL bRet=TRUE;

	while(!::CreateProcess(NULL,
		strCommandLine.GetBuffer(strCommandLine.GetLength()),
		NULL,
		NULL,
		FALSE,
		/*CREATE_NO_WINDOW*/CREATE_NEW_CONSOLE,
		NULL,
		strDir.GetBuffer(strDir.GetLength()),
		&si,pi))
	{
		::Sleep(3000);
		n++;
		if(n==4)
		{
			bRet=FALSE;
			break;
		}

		DWORD dw=GetLastError();
		printf("error:%d\n",dw);
	}

	return bRet;

}
BOOL RunProcess(PROCESS_INFORMATION*pi,CString ProcessName,bool islocal)
{
	CString strCommandLine=_T("");

    STARTUPINFO si;
  //  PROCESS_INFORMATION pi;
	CString strDir=GetRootPath();
	strDir+="\\fcgi-bin\\";

	DWORD dcid=::GetCurrentProcessId();

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( pi, sizeof(PROCESS_INFORMATION) );

	if(islocal)
		strCommandLine.Format("%s",(LPCSTR)ProcessName);
	else
		strCommandLine.Format("%s\\fcgi-bin\\%s",GetRootPath(),(LPCSTR)ProcessName);

	printf("commandline:%s\n",strCommandLine.GetBuffer(0));

	int n=0;
	BOOL bRet=TRUE;

	while(!::CreateProcess(NULL,
		strCommandLine.GetBuffer(strCommandLine.GetLength()),
		NULL,
		NULL,
		FALSE,
		/*CREATE_NO_WINDOW*/CREATE_NEW_CONSOLE,
		NULL,
		strDir.GetBuffer(strDir.GetLength()),
		&si,pi))
	{
		::Sleep(3000);
		n++;
		if(n==4)
		{
			bRet=FALSE;
			printf("Run process:%s failed,error id:%d\n",(LPCSTR)ProcessName,GetLastError());
			break;
		}

		DWORD dw=GetLastError();
		printf("error:%d\n",dw);
	}

	return bRet;

}

BOOL RunRefresh(string queuename,string label,string address)
{
	CString strCommandLine=_T("");

    STARTUPINFO si;
  //  PROCESS_INFORMATION pi;
	CString strDir=GetRootPath();
	strDir+="\\fcgi-bin\\";


	PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(PROCESS_INFORMATION) );


	strCommandLine.Format("%s\\fcgi-bin\\MonitorSchedule.exe %s %s %s",GetRootPath(),queuename.c_str(),label.c_str(),address.c_str());

	printf("commandline:%s\n",strCommandLine.GetBuffer());

	int n=0;
	BOOL bRet=TRUE;

	if(!::CreateProcess(NULL,
		strCommandLine.GetBuffer(strCommandLine.GetLength()),
		NULL,
		NULL,
		FALSE,
		/*CREATE_NO_WINDOW*/CREATE_NEW_CONSOLE,
		NULL,
		strDir.GetBuffer(strDir.GetLength()),
		&si,&pi))
	{
		
		DWORD dw=GetLastError();
		printf("error:%d\n",dw);

		bRet=FALSE;
	}

	return bRet;


}

CString GetRootPath()
{
	string path=::GetSiteViewRootPath();
	return path.c_str();

	return "d:\\v70";
//	return ::FuncGetInstallRootPath();

/*	CString strRet = _T("");
	HKEY hKey = NULL;
	LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,KEY_PATH,0,KEY_READ,&hKey);
	if(lRet != ERROR_SUCCESS)
		return strRet;

	
	LPTSTR  lpName = NULL,
 			lpValue = NULL;
	CString strSubKey = _T(""),
 			strValKey = _T("");
	DWORD   dwIndex = 0,
			dwS1 = 255,
			dwS2 = 255,
			dwType = REG_SZ;
	BOOL    bFindFlag = FALSE;
	char szAppRoot[255] = {0};
	while(lRet == ERROR_SUCCESS)
	{
		lpName = strSubKey.GetBuffer(dwS1);
		lpValue = strValKey.GetBuffer(dwS2);
		lRet = RegEnumValue(hKey, dwIndex++, lpName , &dwS1,NULL, &dwType ,(LPBYTE)lpValue, &dwS2);
		dwS1 = 255;
		dwS2 = 255;
		if(lRet == ERROR_NO_MORE_ITEMS)
			break;
		if(strSubKey.CompareNoCase(KEY_NAME) == 0)
		{
			bFindFlag = TRUE;
			strcpy(szAppRoot,strValKey);
			break;
		}
	}

	RegCloseKey(hKey);
	
	if(bFindFlag)
	{
		strRet.Format("%s",szAppRoot);
	}

	return strRet;*/


}
CString GetConditonString(void)
{
/*	CString strRet = _T("");
	HKEY hKey = NULL;
	LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,KEY_PATH,0,KEY_READ,&hKey);
	if(lRet != ERROR_SUCCESS)
		return strRet;

	
	LPTSTR  lpName = NULL,
 			lpValue = NULL;
	CString strSubKey = _T(""),
 			strValKey = _T("");
	DWORD   dwIndex = 0,
			dwS1 = 255,
			dwS2 = 255,
			dwType = REG_SZ;
	BOOL    bFindFlag = FALSE;
	char szAppRoot[255] = {0};
	while(lRet == ERROR_SUCCESS)
	{
		lpName = strSubKey.GetBuffer(dwS1);
		lpValue = strValKey.GetBuffer(dwS2);
		lRet = RegEnumValue(hKey, dwIndex++, lpName , &dwS1,NULL, &dwType ,(LPBYTE)lpValue, &dwS2);
		dwS1 = 255;
		dwS2 = 255;
		if(lRet == ERROR_NO_MORE_ITEMS)
			break;
		if(strSubKey.CompareNoCase("SeKey") == 0)
		{
			bFindFlag = TRUE;
			strcpy(szAppRoot,strValKey);
			break;
		}
	}

	RegCloseKey(hKey);
	
	if(bFindFlag)
	{
		strRet.Format("%s",szAppRoot);
	}*/

	CString strPath=_T(""),
		strText=_T(""),
		strRet=_T(""),
		strTemp=_T("");

	char szbuf[256]={0};
	char szguid[256]={0};
	char szcondition[256]={0};
	::GetSystemDirectory(szbuf,255);
	strPath.Format("%s\\comguid.dll",szbuf);
	puts(strPath);
	CStdioFile file;
	if(!file.Open((LPCTSTR)strPath,CFile::modeRead,NULL))
		return strRet;
	file.ReadString(strText);
	file.Close();
	if(strText.IsEmpty())
		return strRet;
	strPath.Format("%s\\comdf.dll",szbuf);
	if(!file.Open((LPCTSTR)strPath,CFile::modeRead,NULL))
		return strRet;

	file.ReadString(strTemp);
	if(strTemp.IsEmpty())
		return strRet;

/*	if(!DECRYPT((LPCSTR)strText,szguid))
		return "no";

	if(!DECRYPT((LPCSTR)strTemp,szcondition))
		return "no";
*/
	
	strTemp=szcondition;
	if(strTemp.Find(szguid,0)<0)
		return "no";
	strRet=strTemp.Right(strTemp.GetLength()-strTemp.Find("&",0)-1);
	puts(strTemp);
	puts(strRet);

	return strRet;

}
BOOL Check(void)
{
/*
	return TRUE;

	CString strCondition=_T("");
	strCondition=GetConditonString();
	if(strCondition.IsEmpty()||(strCondition.CompareNoCase("yes")==0))
		return TRUE;
	else
		return FALSE;
	*/

	//---------------------------------------------------------------
	// 修改内容：修改认证方式，通过lic认证
	// 修改人：邹晓
	// 修改日期：2009.03.12
	//---------------------------------------------------------------
	SvDog svdog;
	DATA_DOG dd_ecc;
	if( !svdog.SvReadDog("ECC", &dd_ecc) )
	{
		char szStopTime[32] = {0};
		sprintf( szStopTime, "%s", dd_ecc.StopDate );
		
		char szTemp[32] = {0};
		int nStopYear(0), nStopMonth(0), nStopDay(0);

		strncpy( szTemp, szStopTime, 4 );
		nStopYear = atoi(szTemp);
		strncpy( szTemp, szStopTime+4, 2 );
		szTemp[2] = '\0';
		nStopMonth = atoi(szTemp);
		strncpy( szTemp, szStopTime+6, 2 );
		szTemp[2] = '\0';
		nStopDay = atoi(szTemp);

		TTime stopTime( nStopYear, nStopMonth, nStopDay, 0, 0, 0 );

		if( stopTime >= TTime::GetCurrentTimeEx() )
		{
			puts("测试通过(dog)！\n");
			AddToEventLog( "测试通过(dog)！\n" );
			return true;
		}
		else
		{
			puts("超过有效使用期了(dog)！\n");
			AddToEventLog( "超过有效使用期了(dog)！\n" );
			sprintf(szTemp , "到期时间：%d-%d-%d" , nStopYear , nStopMonth , nStopDay);
			AddToEventLog(szTemp);
			return false;
		}
	}
	else
	{
		bool bSoftDog = false;
		unsigned long nNowTime = ::GetTickCount();
		unsigned long nSpan = ( nNowTime - g_nLastCheckTime ) / 1000;
		g_nLastCheckTime = nNowTime;
// 保存lic中的信息，用于判断lic是否发生变化
		static char szLastPoints[32] = {0};
		static char szLastDevices[32] = {0};
		static char szLastStartDate[32] = {0};
		static char szLastDelayDays[32] = {0};
		char szLicenseName[1024] = {0};
		sprintf( szLicenseName, "%s\\fcgi-bin\\SiteViewECC.lic", ( GetSiteViewRootPath() ).c_str() );
		char szCheckName[] = "SiteViewECC";	
		CDllCall dllInvoke;
		char szLic[20480] = {0};
//----------------------------------------------
// 获取lic信息
//----------------------------------------------
		if( !dllInvoke.GetLicString( szLicenseName, (char*)&szLic ) )
		{
			AddToEventLog( "无法获取授权数据!\n" );
			return false;
		}
		LICENSEDATA licData;
		if( !dllInvoke.Decrypt2Data( (char*)&szLic, &licData ) )
		{
//----------------dy2010-10-12换了lic，客户端读的还是以前的general.ini，还是可以登陆-----------------------
			Des des;
			std::string lasttime = "0";
			char strDesLastTime[1024]={0};
			des.Encrypt(lasttime.c_str(),strDesLastTime);
			WriteIniFileString("license", "lasttime", strDesLastTime,  "general.ini");
//------------------------------------------------------------------------------------------------	
			AddToEventLog( "无法解密授权数据!\n" );
			return false;
		}
		else
		{
			//char szTemp1[100] = {0};
			//sprintf( szTemp1, "licData.Delaydays: %d", licData.Delaydays );
			//AddToEventLog(szTemp1);
			
			if(licData.Isfinal==1)
			{
				AddToEventLog("true");
//-----------------------增加一个字段Isfinal,写入general.ini供客户端读------------------------------------				
				Des des;
				char out[256] = {0};
				char szIsfinal[32] = {0};
				sprintf(szIsfinal, "%d", licData.Isfinal);
				if( des.Encrypt( szIsfinal, out ) )
				{
					WriteIniFileString( "license", "Isfinal", out, "general.ini" );
				}

				if (WriteIniFileString("license" , "Version" , "正式版" ,"general.ini"))
				{
					AddToEventLog("1Version true");
				}
				else
				{
					AddToEventLog("1Version false");
				}
//----------------------------------------------------------2010-09-02
				bSoftDog = true;

				string	beginTime	= licData.StartDate;
				int		bYear		= atoi(beginTime.substr(0,4).c_str());
				int		bMonth		= atoi(beginTime.substr(4 ,2).c_str());
				int		bDay		= atoi(beginTime.substr(6,2).c_str());

				CTime time1(bYear , bMonth , bDay , 0 , 0 , 0);
				time_t bTime = time1.GetTime() + licData.Delaydays * 24*60*60;				
				TTime stopTime(bTime);
				if(stopTime >= TTime::GetCurrentTimeEx())
				{
					puts("测试通过(soft dog)！\n");
					AddToEventLog( "测试通过(soft dog)！\n" );
				}
				else
				{
					puts("超过有效使用期了(soft dog)！\n");
					AddToEventLog( "超过有效使用期了(soft dog)！\n" );
					return false;
				}
			}
			else
			{
//-----------------------增加一个字段Version-------------------------2010-09-02段毅			
				if (WriteIniFileString("license" , "Version" , "测试版" ,"general.ini"))
				{
					AddToEventLog("0Version true");
				}
				else
				{
					AddToEventLog("0Version false");
				}
//-----------------------增加一个字段Version--------------------------2010-09-02段毅	
			}
				char szTemp[1024] = {0};
				sprintf( szTemp, "ProductName: %s", licData.ProductName );
				sprintf( szTemp, "%s\nSysFeatures: %s", szTemp, licData.SysFeatures );
				sprintf( szTemp, "%s\nPoints : %d", szTemp, licData.Points );
				sprintf( szTemp, "%s\nDevices : %d", szTemp, licData.Devices );
				sprintf( szTemp, "%s\nStartDate : %s", szTemp, licData.StartDate );
				sprintf( szTemp, "%s\nTotalTime: %ld", szTemp, licData.TotalTime );
				sprintf( szTemp, "%s\nDelaydays : %d", szTemp, licData.Delaydays );
				sprintf( szTemp, "%s\nDelayTimes : %d", szTemp, licData.DelayTimes );
				sprintf( szTemp, "%s\nIsfinal : %d\n", szTemp, licData.Isfinal );
			    AddToEventLog( szTemp );
				char szPoints[32] = {0};
				char szDevices[32] = {0};
				char szStartDate[32] = {0};
				char szDelayDays[32] = {0};
				sprintf( szPoints, "%d", licData.Points );
				AddToEventLog("SiteView.lic中的点数");
				AddToEventLog(szPoints);
				sprintf( szDevices, "%d", licData.Devices );
				sprintf( szStartDate, "%s", licData.StartDate );
				sprintf( szDelayDays, "%d", licData.Delaydays );
				//bin.liu
	            string strstarttimeold = GetIniFileString( "license", "starttime", "",  "general.ini" );
                Des des;
				char out[256] = {0};


				if( strcmp(szPoints, szLastPoints )			|| 
					strcmp(szDevices, szLastDevices )		||
					strcmp(szStartDate, szLastStartDate )	||
					strcmp(szDelayDays, szLastDelayDays ) 
					)
				{
					AddToEventLog( szLastPoints );
					AddToEventLog( szPoints );
					AddToEventLog( szLastDevices );
					AddToEventLog( szDevices );
					AddToEventLog( szLastStartDate );
					AddToEventLog( szStartDate );
					AddToEventLog( szLastDelayDays );
					AddToEventLog( szDelayDays );
// 保存历史数据
					strcpy( szLastPoints, szPoints );
					strcpy( szLastDevices, szDevices );
					strcpy( szLastStartDate, szStartDate );
					strcpy( szLastDelayDays, szDelayDays );
					
					if( des.Encrypt( szPoints, out ) )
					{
						WriteIniFileString( "license", "point", out, "general.ini" );
					}
					memset( out, 0, 256 );
					if( des.Encrypt( szDevices, out ) )
					{
						WriteIniFileString( "license", "nw", out, "general.ini" );
					}
					char szTemp[32] = {0};
					strncpy( szTemp, szStartDate, 4 );
					strcat( szTemp, "-" );
					strncat( szTemp, szStartDate+4, 2 );
					strcat( szTemp, "-" );
					strncat( szTemp, szStartDate+6, 2 );
					memset( out, 0, 256 );
					if( des.Encrypt( szTemp, out ) )
					{
						WriteIniFileString( "license", "starttime", out, "general.ini" );
					}
				   
				   //AddToEventLog( strlasttimeold.c_str() );
					memset( out, 0, 256 );
					if( des.Encrypt( szDelayDays, out ) )
					{
						WriteIniFileString( "license", "lasttime", out, "general.ini" );
					}
					
				
				}

						//bin.liu 2012-5-3
					if(strcmp(licData.StartDate,"20120420")==0)
					{
						  int sstian=GetIniFileInt("license", "30tian", 90, "general.ini");
							if(sstian==90)
							{
								if(licData.Isfinal!=1)
								{
									
										CTime tmSCan = CTime::GetCurrentTime();
										CString szTime = tmSCan.Format("%Y-%m-%d");  
										AddToEventLog( szTime);
										memset( out, 0, 256 );
										if( des.Encrypt( szTime, out ) )
										{
											DeleteIniFileKey("license", "starttime", "general.ini");
											WriteIniFileString( "license", "starttime", out, "general.ini" );
										}
										memset( out, 0, 256 );
										if( des.Encrypt( "30", out ) )
										{
											DeleteIniFileKey("license", "lasttime", "general.ini");
											WriteIniFileString( "license", "lasttime", out, "general.ini" );
										}
										WriteIniFileInt("license", "30tian", 30, "general.ini");
										AddToEventLog( "30天试用初始化!\n" );
										return true;
									
								}

							}else
							{
										
										WriteIniFileString( "license", "starttime", strstarttimeold, "general.ini" );
										memset( out, 0, 256 );
										if( des.Encrypt( "30", out ) )
										{
											DeleteIniFileKey("license", "lasttime", "general.ini");
											WriteIniFileString( "license", "lasttime", out, "general.ini" );
										}
										AddToEventLog( "30天试用ok!\n" );
										return true;
							}
							
					}
					//--------------------------------bin.liu------------------------------
//			}

		}
//----------------------------------------------
// 认证
//----------------------------------------------
		if( !dllInvoke.GetLicString( szLicenseName, (char*)&szLic ) )
		{
			AddToEventLog( "无法获取授权数据!\n" );
			return false;
		}

//added by lifeng.wang 2010-1-27 在中组部项目定制添加的机器码部分，如果机器码中不包含这个，则作普通处理，否则，视为中组部定制的ECC
		const char * strZZB = "IST:0000000000000000,MAC:001D099A4610";
		char defbuf[512] = {0};
		
		string strLicStr = licData.SysFeatures;
		AddToEventLog("SiteView.Lic");
		AddToEventLog(strLicStr.c_str());

		int n_ZZB_Pos = strLicStr.find(strZZB , 0);			//中组部标误码在机器码中出现的位置
		char b[128] = {0};
		sprintf(b , "n_ZZB_Pos=%d" , n_ZZB_Pos);
		AddToEventLog(b);
		if(n_ZZB_Pos >= 0)
		{
			AddToEventLog("Customer中组部");
			if (WriteIniFileString("license" , "Customer" , "中组部" ,"general.ini"))
			{
				AddToEventLog("Customer true");
			}
			else
			{
				AddToEventLog("Customer false");
			}
			 
			
//			return true;
		}
		else
		{
			AddToEventLog("Customer other");
			DeleteIniFileKey("license" , "Customer", "general.ini");
		}
		int nRet = -1;
		if(bSoftDog)
		{
			AddToEventLog("不带机器码校验");
			//nRet = dllInvoke.IsValidLicFile( szLicenseName, szCheckName, (char*)&szLic, nSpan, false );		//不带机器码校验
			nRet = dllInvoke.IsValidLicFile( szLicenseName, "", (char*)&szLic, nSpan, false );  //不验证模块了
		}
		else
		{
			AddToEventLog("带机器码校验");
			//nRet = dllInvoke.IsValidLicFile( szLicenseName, szCheckName, (char*)&szLic, nSpan, true );		//带机器码校验
			nRet = dllInvoke.IsValidLicFile( szLicenseName, "", (char*)&szLic, nSpan, true );
		}

		if( nRet == 0 )
		{
			AddToEventLog( "测试通过，校验机器码成功！\n" );
			return true;
		}
		else
		{
//----------------dy2010-10-12换了lic，客户端读的还是以前的general.ini，还是可以登陆-------------------------------------------------------
			Des des;
			std::string lasttime = "0";
			char strDesLastTime[1024]={0};
			des.Encrypt(lasttime.c_str(),strDesLastTime);
			WriteIniFileString("license", "lasttime", strDesLastTime,  "general.ini");
//--------------------------------------------------------------------------------

			char szTemp[1024] = {0};
			sprintf( szTemp, "测试不通过: error:%d!\n", nRet );			
			sprintf( szTemp, "%s\nProductName: %s", szTemp, licData.ProductName );
			sprintf( szTemp, "%s\nSysFeatures: %s", szTemp, licData.SysFeatures );
			sprintf( szTemp, "%s\nPoints : %d", szTemp, licData.Points );
			sprintf( szTemp, "%s\nDevices : %d", szTemp, licData.Devices );
			sprintf( szTemp, "%s\nStartDate : %s", szTemp, licData.StartDate );
			sprintf( szTemp, "%s\nTotalTime: %ld", szTemp, licData.TotalTime );
			sprintf( szTemp, "%s\nDelaydays : %d", szTemp, licData.Delaydays );
			sprintf( szTemp, "%s\nDelayTimes : %d\n", szTemp, licData.DelayTimes );
			AddToEventLog( szTemp );
			return false;
		}
	}	
}

DWORD GetProcessMemSize(DWORD ProcessID)
{
   char szProcessName[MAX_PATH] = "unknown";
	char szProcessPath[MAX_PATH] = "unknown";
	DWORD dwSize = 0;
    // Get a handle to the process.

    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, ProcessID );

    // Get the process name.

    if (NULL != hProcess )
    {
 //       HMODULE hMod;
  //      DWORD cbNeeded;

 //       if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
   //          &cbNeeded) )
     //   {
//            GetModuleBaseName( hProcess, hMod, szProcessName, 
  //                             sizeof(szProcessName) );
			PROCESS_MEMORY_COUNTERS pmc;
			if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
			{
				dwSize = pmc.WorkingSetSize/(1024*1024);
			}
 //       }
    }

    CloseHandle( hProcess );

	return dwSize;
	


}
