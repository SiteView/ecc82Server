// MonitorContrl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MonitorContrl.h"
#include "winsvc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

#define		DEBUG_T
#define		WM_COMMUNICATION	WM_USER+135

CWinApp theApp;

using namespace std;

BOOL Run(void);
BOOL Init(void);
BOOL AddToErrorLog(CString strError);
BOOL AddToEventLog(CString strMessage);
CString GetRootPath(void);

///
CString rootPath;
BOOL rocExecuteBat(CString path, CString filename);
BOOL FileExists(char *filename);

#define KEY_PATH		"SOFTWARE\\SiteView\\SiteViewECC"
#define KEY_NAME		"root_path_7"
#define szServiceName	"svEccXfireServer"

HANDLE g_hCom=NULL;
HANDLE g_hReLoadConfig=NULL;
HANDLE g_hExit=NULL;
HANDLE g_hReloadEvent=NULL;

PROCESS_INFORMATION xfire_pi;

int		 g_seid=1;					//seid;
CString  g_strMessage=_T("");

CString g_startXfire = "start_xfire.bat";
CString g_stopXfire = "shut_down_xfire.bat";

char	*g_account		= NULL;
char	*g_password		= NULL;
string	g_mcPName		= "";

const char *g_ServiceDName="svEccXfireServer";
const char *g_ServiceName ="svEccXfireServer";

BOOL g_Disabled=FALSE;
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
			AddToEventLog("服务停止...");

			rocExecuteBat(rootPath+"\\xfire\\", g_stopXfire);	// 启动 .bat 文件
			::Sleep(1000);

            SVS_ServiceStatus.dwWin32ExitCode = 0; 
            SVS_ServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
            SVS_ServiceStatus.dwCheckPoint    = 0; 
            SVS_ServiceStatus.dwWaitHint      = 0; 

			g_SubProcessState=FALSE;

			::Sleep(2000);
 
            if(!SetServiceStatus(SVS_ServiceStatusHandle, &SVS_ServiceStatus))
            { 
				g_strMessage.Format("设置服务状态出错: %ld\r\n", GetLastError());
				AddToErrorLog(g_strMessage);
            } 
			
			AddToEventLog("服务停止");
			AddToEventLog("");
            return; 
 
        case SERVICE_CONTROL_INTERROGATE: 
			// Fall through to send current status. 
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
	AddToEventLog("服务启动...");

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

	// 启动 xfire 目录中的 .bat
	rocExecuteBat(rootPath+"\\xfire\\", g_startXfire);	

	Sleep(2000);

    Run();

	AddToEventLog("服务启动成功!");
	AddToEventLog("");

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
	CString buf = GetRootPath();

	buf += "\\fcgi-bin\\svXfireMonitor.exe -service";

	//sprintf(buf,"\"%s\" -service",g_mcPName.c_str());
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

		puts("\t Open 'SCManger' failed");
		return false;
	}

	SC_HANDLE hsc=::OpenService(hsm,g_ServiceName,DELETE);
	if(hsc==NULL)
	{
		puts("Remove service failed");

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
			default:	
				puts("\t Unknown error with 'OpenService'");
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

	g_mcPName=argv[0];

	if(argc>1)
	{
		if(::_stricmp(argv[1],"-service")==0)
		{
			SERVICE_TABLE_ENTRY ste[] = 
			{
				{szServiceName,	SVS_ServiceStart}, 
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
	else
	{
		Run();

		MSG msg;
		while(::GetMessage(&msg,NULL,0,0))
		{
			TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		::Sleep(100000);
	}
	return nRetCode;
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


BOOL Init(void)
{
	g_hCom=::CreateEvent(NULL,TRUE,FALSE,"Global\\Siteview-Communictions");
	if(g_hCom==NULL)
	{
		AddToErrorLog("创建跟子进程通信事件失败\n");
		return FALSE;
	}
	AddToEventLog("创建跟子进程通信事件成功\n");

	if(::GetLastError()==ERROR_ALREADY_EXISTS)
	{
		AddToErrorLog("该进程已经存在!");
		return FALSE;
	}

	g_SubProcessState=TRUE;

	return TRUE;
}

BOOL Run(void)
{
	/*
	if(!Init())
	{
		AddToErrorLog("初始化失败!");
		return FALSE;
	}
	*/
	g_SubProcessState = TRUE;
	
	int i=0;
	
	DWORD dw;

	// xfire
	if(g_seid==1)
	{
		for(i=0;i<1;i++)
		{
			HANDLE ha=::CreateThread(NULL, 0, &WatchXfireProcess, NULL, 0, &dw);
			if(ha)
			{
				AddToEventLog("Start Watch xfire thread succeed");
				::CloseHandle(ha);
				break;
			}
			else if(ha==NULL && i==5)
			{
				AddToErrorLog("Start Watch xfire thread failed");
				return FALSE;
			}
		}
	}
	
	return TRUE;
}

BOOL AddToErrorLog(CString strError)
{
	CString strTemp=_T(""),
		strFile=_T("");
	strFile.Format("%s\\temp\\batMonitorError.log",GetRootPath());

	CTime time=CTime::GetCurrentTime();
	strTemp=time.Format("%Y-%m-%d %H:%M:%S");

	strTemp+="\t"+strError+"\n";

	FILE *fp=NULL;
	fp=::fopen(strFile,"a+");
	if(fp==NULL)
	{
		printf("打开日志文件失败!\n");
		return FALSE;
	}
	::fputs(strTemp,fp);
	::fclose(fp);
	printf("%s",strTemp);
	return TRUE;
}

BOOL AddToEventLog(CString strMessage)
{
	CString strTemp=_T(""),
		strFile=_T("");
	strFile.Format("%s\\temp\\batMonitorEvent.log",GetRootPath());


	CTime time=CTime::GetCurrentTime();
	strTemp=time.Format("%Y-%m-%d %H:%M:%S");

	strTemp+="\t"+strMessage+"\n";
	FILE *fp=::fopen(strFile,"a+");
	if(fp==NULL)
	{
		printf("打开日志文件失败!\n");
		return FALSE;
	}
	::fputs(strTemp,fp);
	::fclose(fp);
	printf("%s",strTemp);
	
	return TRUE;
}

// 检查指定文件是否存在
BOOL FileExists(char *filename)
{
	BOOL Exist;  
	// 最后结果，表示目录是否存在
	if(filename[strlen(filename)]=='\\')
		filename[strlen(filename)-1]='\0';    // 先删除最后的“\”
	
	WIN32_FIND_DATA wfd;                                  // 查找
	
	HANDLE hFind=FindFirstFile(filename,&wfd);                  
	
	if(hFind==INVALID_HANDLE_VALUE)
		Exist=FALSE;						// 没有找到配备，目录肯定不存在
	else
	{
		if(wfd.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)		// 检查找到的结果是否目录
			Exist=TRUE;												// 不是目录
		else
			Exist=FALSE;
		
		FindClose(hFind);
	} 
	
	return(Exist);	
}

///
BOOL rocExecuteBat(CString path, CString filename)
{
	CString strDir;

	strDir = path+filename;
	if( FileExists( strDir.GetBuffer(strDir.GetLength())) == FALSE)
		return FALSE;

	AddToEventLog( "开始运行 " + filename);

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
			AddToEventLog("  > 启动 " + filename +"失败");
			break;
		}

		AddToEventLog("  > 启动 " + filename +"失败");
	}

	if(bRet)
		AddToEventLog("  > 启动 " + filename +" 成功");

	return bRet;
}


// 从注册表取ECC 服务端安装目录
CString GetRootPath()
{
	CString resultStr =  _T("");
	string rootPath = _T("");

	try
	{
		HKEY hKey = NULL;
		LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,KEY_PATH,0,KEY_READ,&hKey);
		if(lRet != ERROR_SUCCESS)
			return resultStr;
		
		LPTSTR  lpName = NULL,
 				lpValue = NULL;
		TCHAR	strSubKey[1024]={0};
		TCHAR	strValKey[1024]={0};
		DWORD   dwIndex = 0,
				dwS1 = 1024,
				dwS2 = 1024,
				dwType = REG_SZ;
		BOOL    bFindFlag = FALSE;
		TCHAR szAppRoot[1024] = {0};
		while(lRet == ERROR_SUCCESS)
		{
			lpName = (LPTSTR)strSubKey;
			lpValue = (LPTSTR)strValKey;
			lRet = RegEnumValue(hKey, dwIndex++, lpName , &dwS1,NULL, &dwType ,(LPBYTE)lpValue, &dwS2);
			dwS1 = 1024;
			dwS2 = 1024;
			if(lRet == ERROR_NO_MORE_ITEMS)
				break;

			if(::_stricmp(strSubKey,KEY_NAME)==0)
			{
				bFindFlag = TRUE;
				rootPath = strValKey;//

				//strcpy(rootPath,strValKey);
				break;
			}
		}

		RegCloseKey(hKey);		
	}
	catch(...)
	{
		return resultStr;
	}
	
	int pos = rootPath.rfind("\\");
	if(pos == rootPath.size()-1)
		rootPath.erase(pos,1);

	resultStr = rootPath.c_str();

	return resultStr;
}