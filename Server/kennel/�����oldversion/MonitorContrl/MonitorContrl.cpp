// MonitorContrl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MonitorContrl.h"
#include "winsvc.h"
//#include "..\base\funcGeneral.h"
#include <svdbapi.h>
#include <libutil/Time.h>
#include <libutil/buffer.h>
#include <base/SVHttp.h>
#include "PSAPI.h"
#pragma comment( lib, "PSAPI.LIB" )


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

#define DEBUG_T

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
BOOL RunProcess(PROCESS_INFORMATION*pi,CString ProcessName);

DWORD GetProcessMemSize(DWORD ProcessID);

HANDLE g_hCom=NULL;
HANDLE g_hReLoadConfig=NULL;
HANDLE g_hExit=NULL;
HANDLE g_hReloadEvent=NULL;

PROCESS_INFORMATION g_pi;
PROCESS_INFORMATION g_alertpi;
PROCESS_INFORMATION g_reportpi;


long	g_lSubRestartTime=0;   //��ʱ���������̵�ʱ����(����);
UINT	g_uCheckTime=0;        //�ͼ�����ͨѶ��ʱ����(����); 
UINT	g_uExitTimeOut=1;      //��ֹ�ӽ���ʱҪ�ȴ���ʱ��(����);
CString  g_strMessage=_T("");
long	g_lCheckTime=5;       //����Ƿ���ڵ�ʱ����(����)
int		g_iStartType=1;       //1Ϊ����MonitorExecutent.exe,2Ϊ����MonitorSchedule.exe
DWORD   g_uMaxMemory=500;     //���������ռ�õ�����ڴ���(�����ڴ浥λM);
CString g_strDBAddress="localhost"; //���з�������ַ;
int		g_seid=1;					//seid;

CString g_AlertProcessName="AlertServer.exe";
CString g_ReportProcessName="reportgenerate.exe";
CString g_strRefreshQueueName="SiteView70_RefreshInfo";

char *g_account=NULL;
char *g_password=NULL;
string g_mcPName="";

const char *g_ServiceDName="SiteView_Watch";
const char *g_ServiceName="SiteView_Watch";

///
CString rootPath;
BOOL RunExecuteBat(CString path, CString filename);
BOOL FileExists(char *filename);

CString g_startItsm	= "start.bat";
CString g_stopItsm	= "stop.bat";

//CString g_startWeb813	= "net start 'SiteViewEcc813WEB'";
//CString g_stopIWeb813	= "net stop 'SiteViewEcc813WEB'";
CString g_startWeb813	= "startweb813.bat";
CString g_stopWeb813	= "stopweb813.bat";

CString g_strofbizurl="http://localhost:18081/cxf/eccservices/eccapi?wsdl";
int g_ofbizheartbeat=10;
CString g_strweb813url="http://127.0.0.1:8181/ecc/";
int g_web813heartbeat=10;

PROCESS_INFORMATION xfire_pi;

BOOL	g_Disabled=FALSE;
BOOL g_SubProcessState=TRUE;

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

			RunExecuteBat(rootPath+"\\itsm\\", g_stopItsm);	// ���� itsmĿ¼�е� stop.bat

			AddToEventLog("�����˳�����ֹ�ӽ���");

 
            if(!SetServiceStatus(SVS_ServiceStatusHandle, &SVS_ServiceStatus))
            { 
				g_strMessage.Format("���÷���״̬����: %ld\r\n", GetLastError());
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
		g_strMessage.Format("���÷���״̬����: %ld\r\n", GetLastError());
	
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

		g_strMessage = _T("�����ʼ��ʧ��!\r\n");
		AddToErrorLog(g_strMessage);
        return; 
    } 


    SVS_ServiceStatus.dwCurrentState       = SERVICE_RUNNING; 
    SVS_ServiceStatus.dwCheckPoint         = 0; 
    SVS_ServiceStatus.dwWaitHint           = 0; 
 
    if (!SetServiceStatus (SVS_ServiceStatusHandle, &SVS_ServiceStatus)) 
    { 
		g_strMessage.Format("���÷���״̬���� %ld\r\n", GetLastError());
		AddToErrorLog(g_strMessage);
		return;
    }

	HANDLE m_handle=GetCurrentProcess();
	DWORD iPriority=GetPriorityClass(m_handle);
    SetPriorityClass(m_handle,HIGH_PRIORITY_CLASS);
    HANDLE m_thread=GetCurrentThread();
	SetThreadPriority(m_thread,THREAD_PRIORITY_ABOVE_NORMAL);

	// ���� itsmĿ¼�е� start.bat
	RunExecuteBat(rootPath+"\\itsm\\", g_startItsm);	

	AddToEventLog("���������ɹ�!");

	while(!Check())
	{
		::Sleep(g_lCheckTime*1000*60);
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
		//			puts("ԭ��:");

		puts("\t Open 'SCManger' failed");
		return false;
	}

	SC_HANDLE hsc=::OpenService(hsm,g_ServiceName,DELETE);
	if(hsc==NULL)
	{
		puts("Remove service failed");
		//			puts("ԭ��:");

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
	
	rootPath = GetRootPath();
	
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
		}else if(::_stricmp(argv[1],"-install")==0)
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
		puts("dddddd");
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
				 AddToEventLog("�����ؼ��̳߳ɹ�");
				 ::CloseHandle(hd);
				 break;
			 }
			 else if(hd==NULL&&i==5)
			 {
				 AddToErrorLog("�����ؼ��߳�ʧ��!");
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

DWORD WINAPI WatchOfbizService(LPVOID lpParam )
{
	 while(true)
	 {
		SVHttp http;
		http.SetUrl(g_strofbizurl);
		//svHttp.SetTimeOut();
		if(http.SendRequest())
		{
			CString strResponse = "";
			strResponse = http.GetResponseText();

			if(strResponse.GetLength() > 0)
			{
				//puts(strResponse);
				puts("ofbiz service keep alive");
				Sleep(g_ofbizheartbeat*60*1000);
				continue;
			}
		}
		else
		{
			puts("ofbiz service die and reset");
			puts(rootPath+"\\itsm\\");
			// ���� itsmĿ¼�е� stop.bat
			RunExecuteBat(rootPath+"\\itsm\\", g_stopItsm);	
			Sleep(20*1000);
			// ���� itsmĿ¼�е� start.bat
			RunExecuteBat(rootPath+"\\itsm\\", g_startItsm);	
			//Sleep(20*1000);
		}

		Sleep(g_ofbizheartbeat*60*1000);
	 }

	return 1;
}

DWORD WINAPI WatchWeb813(LPVOID lpParam )
{
	 while(true)
	 {
		SVHttp http;
		http.SetUrl(g_strweb813url);
		if(http.SendRequest())
		{
			CString strResponse = "";
			strResponse = http.GetResponseText();

			if(strResponse.GetLength() > 0)
			{
				//puts(strResponse);
				puts("web813 keep alive");
				Sleep(g_web813heartbeat*60*1000);
				continue;
			}
		}
		else
		{
			puts("web813 die and reset");

			// ���� WebECC8.1.3Ŀ¼�е� stop.bat
			RunExecuteBat(rootPath+"\\WebECC8.1.3\\", g_stopWeb813);	
			Sleep(20*1000);
			// ���� WebECC8.1.3Ŀ¼�е� start.bat
			RunExecuteBat(rootPath+"\\WebECC8.1.3\\", g_startWeb813);
			//Sleep(20*1000);
		}

		Sleep(g_web813heartbeat*60*1000);
	 }

	return 1;
}

DWORD WINAPI WatchWeb7(LPVOID lpParam )
{
	 while(true)
	 {
		SVHttp http;
		http.SetUrl("http://localhost:18081/cxf/eccservices/eccapi?wsdl");
		if(http.SendRequest())
		{
			CString strResponse = "";
			strResponse = http.GetResponseText();

			if(strResponse.GetLength() > 0)
			{
				//puts(strResponse);
				puts("web7 keep alive");
				Sleep(180*1000);
				continue;
			}
		}
		else
		{
			puts("web7 die and reset");
			puts(rootPath+"\\itsm\\");
			// ���� itsmĿ¼�е� stop.bat
			RunExecuteBat(rootPath+"\\itsm\\", g_stopItsm);	
			Sleep(20*1000);
			// ���� itsmĿ¼�е� start.bat
			RunExecuteBat(rootPath+"\\itsm\\", g_startItsm);	
		}

		Sleep(60*1000);
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
			lastchecktime=curtime;
			if(!Check())
			{
				puts("ֹͣ���!");
				g_Disabled=TRUE;
				goto EXIT;
			}
			g_Disabled=FALSE;

			if(GetProcessMemSize(g_pi.dwProcessId)>g_uMaxMemory)
			{
				puts("�ڴ�����˳�");
				goto EXIT;
			}



		}

		if(st.GetTotalMinutes()>=g_lSubRestartTime)
		{
			AddToEventLog("���趨����ʱ��");
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
				AddToEventLog("�����˳���ʱ��ǿ���޳�!");
				::TerminateProcess(g_pi.hProcess,2);
			}

		}

		if(!g_Disabled)
		{
			
			::Sleep(g_uCheckTime*60*1000);

			::SetEvent(g_hCom);
			::PostThreadMessage(g_pi.dwThreadId,WM_COMMUNICATION,(WPARAM)0,(LPARAM)0);
			::Sleep(2000);
			
			if(::WaitForSingleObject(g_hCom,1)==WAIT_TIMEOUT)
				continue;
			else
			{
				AddToEventLog("ͨ��ʧ�ܽ�ǿ���޳�����");
				::TerminateProcess(g_pi.hProcess,1);
				::Sleep(20000);
				::SetEvent(g_pi.hProcess);
			}
		}else
			Sleep(60*1000);
			
			
	}



	return 1;
}


/***************************************************************
ֻ���Զ��һ���̶߳���ֱ����ReloadConfig.exe
��g_hReLoadConfigֱ�Ӹ�MonitorExecult.exe
ͨ����Ϊ�˽������õ���չ�������ۻ����صȡ�
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
		  AddToEventLog("�������¼��������ļ�!");

	  }

	  return 1;

}

DWORD WINAPI CheckRefreshThread( LPVOID lpParam )
{
	string label;
	svutil::TTime ct;
	unsigned int len=0;
	string oldlabel;

	int n=0;

	if(!::CreateQueue(g_strRefreshQueueName.GetBuffer(0),1,"default",g_strDBAddress.GetBuffer(0)))
		puts("Create queue failed");

	::ClearQueueMessage(g_strRefreshQueueName.GetBuffer(0),"default",g_strDBAddress.GetBuffer(0));

	printf("Refresh queue:%s,addr:%s\n",g_strRefreshQueueName.GetBuffer(0),g_strDBAddress.GetBuffer(0));

	while(true)
	{
		MQRECORD mrd=::BlockPeekMQMessage(g_strRefreshQueueName.GetBuffer(0),"default",g_strDBAddress.GetBuffer(0));
		if(mrd==INVALID_VALUE)
		{
			printf("queue name:%s,address:%s\n",g_strRefreshQueueName.GetBuffer(0),g_strDBAddress.GetBuffer(0));
			puts("Peek failed");
			::Sleep(10*1000);
			continue;
		}
		if(!::GetMessageData(mrd,label,ct,NULL,len))
		{
			puts("Get message data failed");
			::CloseMQRecord(mrd);
			continue;
		}
		if(label.compare("DYNPARAM")==0)
		{
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
			puts("run failed");
			char buf[256]={0};
			sprintf(buf,"Start monitorschedule.exe failed");
			::PushMessage(label,"Refresh_ERROR",buf,strlen(buf)+1,"default",(LPCSTR)g_strDBAddress);
			::ClearQueueMessage(g_strRefreshQueueName.GetBuffer(0),"default",g_strDBAddress.GetBuffer(0));
		}else
			puts("run ok");

		oldlabel=label;

		::Sleep(3*1000);   //delay

	}

}

BOOL Init(void)
{

	g_hCom=::CreateEvent(NULL,TRUE,FALSE,"Global\\Siteview-Communictions");
	if(g_hCom==NULL)
	{
		AddToErrorLog("�������ӽ���ͨ���¼�ʧ��\n");
		return FALSE;
	}
	if(::GetLastError()==ERROR_ALREADY_EXISTS)
	{
		AddToErrorLog("�ý����Ѿ�����!");
		return FALSE;
	}
/*	g_hReLoadConfig=::CreateEvent(NULL,TRUE,FALSE,"Global\\Siteview-ReloadConfigEvent");

	if(g_hReLoadConfig==NULL)
	{
		AddToErrorLog("�������¼��������ļ��¼�ʧ��!");
		return FALSE;
	}
	g_hExit=::CreateEvent(NULL,TRUE,FALSE,"Global\\Siteview-SubExit");

	if(g_hExit==NULL)
	{
		AddToErrorLog("�����ӽ����˳��¼�ʧ��!");
		return FALSE;
	}

	g_hReloadEvent=::CreateEvent(NULL,TRUE,FALSE,"Global\\Siteview-ReloadEvent");

	if(g_hReloadEvent==NULL)
	{
		AddToErrorLog("���������ļ��¼�ʧ��!");
		return FALSE;
	}*/

	g_SubProcessState=TRUE;

	CString strPath=_T("");
	strPath.Format("%s\\fcgi-bin\\mc.config",GetRootPath());

	printf(strPath+"\n");
	
	g_lSubRestartTime=::GetPrivateProfileInt("information","RestartTime",240,strPath);

	if(g_lSubRestartTime==0)
	{
		AddToErrorLog("�����ļ��д�!");
		return FALSE;
	}
	printf("Restart time %d\n",g_lSubRestartTime);

	g_uCheckTime=::GetPrivateProfileInt("information","CheckTime",2,strPath);

	if(g_uCheckTime==0)
	{
		AddToErrorLog("�����ļ��д�!");
		return FALSE;
	}
	printf("Check time :%d\n",g_uCheckTime);

	g_uExitTimeOut=::GetPrivateProfileInt("information","ExitTimeOut",1,strPath);

//	if(g_uExitTimeOut==0)
 //        g_uExitTimeOut=1;

	g_iStartType=::GetPrivateProfileInt("information","StartType",2,strPath);
//	if(g_iStartType==0)
//		g_iStartType=1;

	g_uMaxMemory=::GetPrivateProfileInt("information","MaxMemory",500,strPath);

	if(g_uMaxMemory<20)
		g_uMaxMemory=20;

	char buf[256]={0};
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

	memset(buf,0,256);
	::GetPrivateProfileString("web","ofbizurl","",buf,255,strPath);
	if(strlen(buf)>1)
		g_strofbizurl=buf;

	memset(buf,0,256);
	::GetPrivateProfileString("web","web813url","",buf,255,strPath);
	if(strlen(buf)>1)
		g_strweb813url=buf;

	memset(buf,0,256);
	::GetPrivateProfileString("web","ofbizheartbeat","",buf,255,strPath);
	if(strlen(buf)>0)
		g_ofbizheartbeat=atoi(buf);

	memset(buf,0,256);
	::GetPrivateProfileString("web","web813heartbeat","",buf,255,strPath);
	if(strlen(buf)>0)
		g_web813heartbeat=atoi(buf);

	char strr[1024]={0};
	sprintf(strr,"%s_%d",g_strRefreshQueueName.GetBuffer(1),g_seid);

	g_strRefreshQueueName=strr;


	return TRUE;
}

BOOL Run(void)
{
	if(!Init())
	{
		AddToErrorLog("��ʼ��ʧ��!");
		return FALSE;
	}

	int i=0;
	
	DWORD dw;
	for(i=0;i<6;i++)
	{
		HANDLE hd=::CreateThread(NULL,0,&CommThreadFunc,NULL,0,&dw);
		if(hd)
		{
			AddToEventLog("Start communication thread succeed");
			::CloseHandle(hd);
			break;
		}
		else if(hd==NULL&&i==5)
		{
			AddToErrorLog("Start communication thread failed");
			return FALSE;
		}
	}

	for(i=0;i<6;i++)
	{
		HANDLE ht=::CreateThread(NULL,0,&ThreadFunc,NULL,0,&dw);
		if(ht)
		{
			AddToEventLog("Start watch MonitorSchedule thread succeed");
			::CloseHandle(ht);
			break;
		}
		else if(ht==NULL&&i==5)
		{
			AddToErrorLog("Start watch MonitorSchedule thread failed");
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

	if(g_seid==1)
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

		for(i=0;i<6;i++)
		{
			HANDLE hrt=::CreateThread(NULL,0,&WatchOfbizService,NULL,0,&dw);
			if(hrt)
			{
				AddToEventLog("Start watch ofbiz service thread succeed");
				::CloseHandle(hrt);
				break;
			}
			else if(hrt==NULL&&i==5)
			{
				AddToErrorLog("Start watch ofbiz service thread failed");
				return FALSE;
			}
		}


		for(i=0;i<6;i++)
		{
			HANDLE hrt=::CreateThread(NULL,0,&WatchWeb813,NULL,0,&dw);
			if(hrt)
			{
				AddToEventLog("Start watch web813 thread succeed");
				::CloseHandle(hrt);
				break;
			}
			else if(hrt==NULL&&i==5)
			{
				AddToErrorLog("Start watch web813 thread failed");
				return FALSE;
			}
		}
	}
	
	/*	for(i=0;i<6;i++)
	{
		HANDLE hr=::CreateThread(NULL,0,&ReloadConfigThreadFunc,NULL,0,&dw);
		if(hr)
		{
			AddToEventLog("�������������ļ��̳߳ɹ�");
			::CloseHandle(hr);
			break;
		}
		else if(hr==NULL&&i==5)
		{
			AddToErrorLog("�������������ļ��߳�ʧ��!");
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
		printf("����־�ļ�ʧ��!\n");
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
//#ifndef DEBUG_T	
	FILE *fp=::fopen(strFile,"a+");
	if(fp==NULL)
	{
		printf("����־�ļ�ʧ��!\n");
		return FALSE;
	}
	::fputs(strTemp,fp);
	::fclose(fp);
//#else
	printf("%s",strTemp);
//#endif
	
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
BOOL RunProcess(PROCESS_INFORMATION*pi,CString ProcessName)
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

//	return "d:\\v70";
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
	return TRUE;

	CString strCondition=_T("");
	strCondition=GetConditonString();
	if(strCondition.IsEmpty()||(strCondition.CompareNoCase("yes")==0))
		return TRUE;
	else
		return FALSE;
	
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

// ���ָ���ļ��Ƿ����
BOOL FileExists(char *filename)
{
	BOOL Exist;  
	// ���������ʾĿ¼�Ƿ����
	if(filename[strlen(filename)]=='\\')
		filename[strlen(filename)-1]='\0';    // ��ɾ�����ġ�\��
	
	WIN32_FIND_DATA wfd;                                  // ����
	
	HANDLE hFind=FindFirstFile(filename,&wfd);                  
	
	if(hFind==INVALID_HANDLE_VALUE)
		Exist=FALSE;						// û���ҵ��䱸��Ŀ¼�϶�������
	else
	{
		if(wfd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)		// ����ҵ��Ľ���Ƿ�Ŀ¼
			Exist=TRUE;												// ����Ŀ¼
		else
			Exist=FALSE;
		
		FindClose(hFind);
	} 
	
	return(Exist);	
}

///
BOOL RunExecuteBat(CString path, CString filename)
{
	CString strDir;

	strDir = path+filename;
	if( FileExists( strDir.GetBuffer(strDir.GetLength())) == FALSE)
		return FALSE;

	AddToEventLog( "��ʼ���� " + filename);

	//strDir = GetRootPath();
	//strDir += "\\xfire\\";

	CString strCommandLine=_T("");
	strCommandLine.Format("%s\\%s", path, filename);

    STARTUPINFO si;

	DWORD dcid=::GetCurrentProcessId();

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &xfire_pi, sizeof(PROCESS_INFORMATION) );

	int n=0;
	BOOL bRet=TRUE;

	while(!::CreateProcess( NULL,
		strCommandLine.GetBuffer(strCommandLine.GetLength()),
		NULL,
		NULL,
		FALSE,
		/*CREATE_NO_WINDOW*/CREATE_NEW_CONSOLE,
		NULL,
		path.GetBuffer(path.GetLength()),
		&si, &xfire_pi))
	{
		::Sleep(3000);
		n++;
		if(n==4)
		{
			bRet = FALSE;
			AddToEventLog("  > ���� " + filename +"ʧ��");
			break;
		}

		AddToEventLog("  > ���� " + filename +"ʧ��");
	}

	if(bRet)
		AddToEventLog("  > ���� " + filename +" �ɹ�");

	return bRet;
}

// 
DWORD WINAPI WatchXfireProcess(LPVOID lpParam )
{
	while(1)
		Sleep(60*1000);

	/*
LOOP:
	 if( g_SubProcessState && (!g_Disabled) )
	 {
		 ::Sleep(2000);
		 DWORD dw=0;
		 for(int i=0;i<1;i++)
		 {
			 HANDLE hd=::CreateThread(NULL,0,&WatchXfireProcess,NULL,0,&dw);
			 if(hd)
			 {
				 AddToEventLog("Start watch xfire thread ok");
				 ::CloseHandle(hd);
				 break;
			 }
			 else if(hd==NULL )	//&& i==5)
			 {
				 AddToErrorLog("Start watch xfire thread failed");
				 break;
			 }
		 }
	 }
	 else
	 {
		 Sleep(60*1000);
		 goto LOOP;
	 }
*/
	return 1;
}