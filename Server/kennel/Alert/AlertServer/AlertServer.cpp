// AlertServer.cpp : �������̨Ӧ�ó������ڵ㡣
//�������������
//һ���������Ͷ����ࣺ
//    1�����ݽṹ
//	     a��Id���������ͱ�������Ĳ���������ʱ��ȡ�
//    2�������ⲿDll�Է��ͱ������麯����
//    3����¼������־�ĺ�����
//�����������������ࣺ�ʼ������š��ű�������
//    1����д�����ⲿDll�Է��ͱ������麯����
//���������¼���
//    1�����ݽṹ
//	     a��MonitorId��EventType��EventCount�ȡ�
//�ġ��������������ࣺ
//    1�����ݽṹ
//	      a��AlertTargetList��
//	      b���������ơ����������ȡ�
//		  c������������
//    2������������Էֽ����ȷ��AlertTargetList�ĺ�����
//    3������MonitorId�������¼��ж��Ƿ�ƥ�䷢���������麯����
//    4�����ɱ������Ͷ�����麯����
//    5�����ݱ���������������ֹͣ�����ȡ�
//�塢�����������������ࣺ�ʼ������š��ű�������
//    1����д����MonitorId�������¼��ж��Ƿ�ƥ�䷢���������麯����
//    2����д���ɱ������Ͷ�����麯����
//�����������ࣺ
//    1�����ݽṹ
//		a��һ��AlertBaseObjList, ����Ϊ�������������ࡣ
//		b���ĸ�WaitSendAlertObjList, ����Ϊ�������Ͷ����ࡣ
//		c��һ������EventList, ����Ϊ�����¼��ࡣ
//		d���̣߳����롢�����
//    2����������
//		a����alert.ini�Գ�ʼ��AlertBaseObjList�� ���ֽ����ȷ��AlertTargetList��
//		b��ѭ����ȡ����Event��EventList��
//		c��ѭ������AlertBaseObjList��EventList���Eventƥ�䣬������AlertSendObj�����͵�WaitSendAlertObjList��
//		d��ѭ����WaitSendAlertObjList���ͳ�ȥ�� ά��WaitSendAlertObjList����¼��־��
//�ߡ�������
//    1����װ�ɷ�����ࡣ
//    2���뱨����־��ѯ��ص��ࡣ
//�ˡ�����������ֹͣ�������������⡣
//�š�ִ�нű�ʱ�̶��ű����û��Զ���ű���ִ�з�ʽ��һ����
//ʮ�����ű����Ķ˿����øı�ʱ֪ͨ������ ���������°󶨶˿ڣ�����������ini����
//ʮһ������ƻ��ļ������������-------�ܸ��ӵ�-------��һ��ͳһ�߼����жϣ� ���뿪�˸��Ӷȡ�
//ʮ�����¼������Խ�������������⡣
//ʮ����1��������2���澯��3������4������5����ֹ��
//ʮ�ġ�Alert.ini�ļ������ı������AlertBaseObjList��

#include "stdafx.h"
#include "AlertMain.h"
#include "./base/funcGeneral.h"
#include <WinSvc.h>
#include <direct.h>
#include "svdb/svapi/svapi.h"

VOID  WINAPI StartAlertService(DWORD argc, LPTSTR *argv);//��������
VOID  WINAPI sch_ServiceCtrlHandler(DWORD opcode);//�������
BOOL  InstallService(CString strServiceName);//��װ����
BOOL  StopService(CString strServiceName);//ֹͣ����
BOOL  InitService();//��ʼ�������߳�

void WriteLogFile(CString strMsg);//д��־�ļ�
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
 *	�������ܣ�������ں���  
 *  ���������argv[1]Ϊ-install�ǰ�װ����-removeʱɾ������argv[2]��װʱΪ�������̨��ʾ��������
    ɾ��ʱΪע��������ƣ��˳���ΪAlertService
 *  �����������
 *  ����ֵ��
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
 *	�������ܣ�������������
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
	
	//InitService();//����ʵ�ʹ����߳�
	Run();
}

/*
 *	�������ܣ�������ƺ���
 *  ���������������Opcode
 *  �������: ��
 *  ����ֵ����
 */
VOID WINAPI sch_ServiceCtrlHandler(DWORD Opcode) 
{ 
    switch(Opcode) 
    { 
        case SERVICE_CONTROL_PAUSE: //��ͣ����
        // Do whatever it takes to pause here. 
            sch_ServiceStatus.dwCurrentState = SERVICE_PAUSED; 
			//AddToLog("Pause Service\r\n");
            break; 
 
        case SERVICE_CONTROL_CONTINUE: //��������
        // Do whatever it takes to continue here. 
            sch_ServiceStatus.dwCurrentState = SERVICE_RUNNING; 

			//AddToLog("Continue Service\r\n");
            break; 
 
        case SERVICE_CONTROL_STOP: //ֹͣ����
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
 *	�������ܣ���װ������
 *  ���������strServiceNameΪ�ڷ������̨����ʾ������
 *  �����������
 *  ����ֵ������������ȷ����TRUE�����򷵻�FALSE
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
 *	�������ܣ�ɾ��������
 *  ����������������̨����ʾ�ķ�������
 *  �����������
 *  ����ֵ��  �ɹ�����TRUE������FALSE
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
    //���ļ�
    stream=fopen(FuncGetInstallRootPath() + "\\AlertServer.log","a");
    if(stream == NULL)
        return ;
   	SYSTEMTIME time;
	//ȡϵͳ��ǰʱ��
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
// *	�������ܣ����������߳�
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
