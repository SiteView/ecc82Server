
#include "Utils.h"
#include "Base64.h"
#include "math.h"
#include <iostream>

using namespace std;

#include "base\funcGeneral.h"

HINSTANCE g_hInstance = NULL;
HINSTANCE g_hInstance2 = NULL;

#ifdef WIN32
#pragma warning (disable : 4267)
#endif

int WriteLog(const char* str);

bool WriteLog(CString strFileName,const CString strLog);


//int WSA_Init()
//{
//	WORD wVersionRequested;  
//	WSADATA wsaData; 
//	int err; 
//	wVersionRequested = MAKEWORD(1, 1); 
//
//	err = WSAStartup(wVersionRequested, &wsaData); 
//
//	if (err != 0) 
//		return -1; 
//
//	if ( LOBYTE( wsaData.wVersion ) != 1 || 
//	   HIBYTE( wsaData.wVersion ) != 1 ) 
//	{ 
//		WSACleanup(); 
//		return -2;
//	}
//
//	return 0;
//}
//
//void WSA_Free()
//{
//	WSACleanup();
//}


void WriteLogFile1(const char* szMsg)
{
    FILE *stream;
    char chTime[50] = {0};
    //打开文件
#if _DEBUG
    stream=fopen(FuncGetInstallRootPath() + "\\Temp\\Monitor.log","a");
#else
    stream=fopen(FuncGetInstallRootPath() + "\\Monitormanager\\Temp\\Monitor.log","a");
#endif
    if(stream == NULL)
        return;//打开文件失败，直接返回
   	SYSTEMTIME time;
	//取系统当前时间
	GetLocalTime(&time);
    //sprintf(chTime ,"%02d年-%02d月-%02d日 %02d时-%02d分-%02d秒 ",
    sprintf(chTime ,"%dY-%dM-%dD %dH-%dM-%dS ",
				//time.wYear - 2000,
                time.wYear,
				time.wMonth,
				time.wDay,
				time.wHour,
				time.wMinute,
				time.wSecond
			);
    fputs(chTime , stream);
	fputs(szMsg,stream);
    fputs("\n",stream);
	fclose(stream);
}


int connect_timeo(int sockfd,const struct sockaddr *saptr,int salen,int nsec)
{
	int n;
	int error;
	int len;
	fd_set rset,wset;
	struct timeval tv;
	
	u_long val = 1;
	ioctlsocket(sockfd,FIONBIO,&val);

	error = 0;

	if((n = connect(sockfd,saptr,salen)) < 0)
	{
		int nErrNum = WSAGetLastError();
//		if(nErrNum == 10035)
//			goto done;

//		if(WSAGetLastError() != WSAEWOULDBLOCK) 
//		if(nErrNum != WSAEWOULDBLOCK)
		if(nErrNum !=  10035)
		{	
			shutdown(sockfd, 0x02);
			closesocket(sockfd);
			return -1;
		}
	}	


	if(n == 0)
		goto done;

	FD_ZERO(&rset);
	FD_SET(sockfd,&rset);
	wset = rset;
	tv.tv_sec = nsec;
	tv.tv_usec = 0;
	printf("zzzzzzzzzz\r\n");
	if((n = select(sockfd + 1,&rset,&wset,NULL,nsec?&tv:NULL)) == 0)
	{
		shutdown(sockfd, 0x02);
		closesocket(sockfd);
		WSASetLastError(WSAETIMEDOUT);
		return -1;
	}
printf("xxxxxxxxxxx\r\n");
	if(FD_ISSET(sockfd,&rset) || FD_ISSET(sockfd,&wset))
	{
		len = sizeof(error);
		if(getsockopt(sockfd,SOL_SOCKET,SO_ERROR,(char *)&error,&len) < 0)
		{
			shutdown(sockfd, 0x02);
			closesocket(sockfd);
			return -1;
		}
	}			
	else
	{
		//printf("select error: sockfd not set");
		shutdown(sockfd, 0x02);
		closesocket(sockfd);
		return -1;
	}		

done:
	val = 0;
	ioctlsocket(sockfd,FIONBIO,&val);
	if(error)
	{
		shutdown(sockfd, 0x02);
		closesocket(sockfd);
		WSASetLastError(error);
		return -1;
	}	
	return 0;
}

int RESPONSE_WAIT(SOCKET s, int nsec)
{
	struct timeval Timeout;
	fd_set readfds;

	readfds.fd_count = 1;
	readfds.fd_array[0] = s;
	Timeout.tv_sec = nsec;
    Timeout.tv_usec = 0;
	return(select(1, &readfds, NULL, NULL, &Timeout));
}

BOOL GetUrlData(char *url, int timeout, char *uid, char *pwd, 
				char *pserver, int pport, char *puid, char *ppwd, 
				int dlen, char *szReturn)
{
	BOOL bResult = TRUE;
	CInternetSession *psession = NULL;
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;

	if(*pserver)
	{
		char	pbuff[256] = {0};
		sprintf(pbuff, "%s:%d", pserver, pport);
		psession = new CInternetSession("WinInet", 1, 
								INTERNET_OPEN_TYPE_PROXY, pbuff, NULL, 0);
	}
	else
	{
		psession = new CInternetSession("WinInet");
	}

	try
	{
		CString strServerName;
		CString strObject;
		INTERNET_PORT nPort;
		DWORD dwServiceType;

		if (!AfxParseURL(url, dwServiceType, strServerName, strObject, nPort) ||
			dwServiceType != INTERNET_SERVICE_HTTP)
		{
			sprintf(szReturn, "%s", FuncGetStringFromIDS("SV_URL",
                "URL_TYPE_ERROR"));//<%IDS_Utils_1%>
			return FALSE;
		}

		pServer = psession->GetHttpConnection(strServerName, nPort);

		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
			strObject, NULL, 1, NULL, NULL, INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT);
		if(*puid)
		{
			CString	User_Pass = _T("");
			User_Pass.Format("%s:%s", puid, ppwd);

			CString	strOutput = _T("Proxy-Authorization: Basic ");

			CBase64	*pBase64 = new CBase64();
			strOutput.Insert(strOutput.GetLength(), pBase64->Encode(User_Pass, User_Pass.GetLength()));
			delete pBase64;
			pBase64 = NULL;
			
			pFile->AddRequestHeaders(strOutput);
		}

		pFile->SendRequest();

		DWORD dwRet;
		pFile->QueryInfoStatusCode(dwRet);
		if(dwRet == HTTP_STATUS_DENIED)
		{
			CString	User_Pass = _T("");
			User_Pass.Insert(User_Pass.GetLength(), uid);
			User_Pass.Insert(User_Pass.GetLength(), ':');
			User_Pass.Insert(User_Pass.GetLength(), pwd);

			CString	strOutput = _T("Authorization: Basic ");

			CBase64	*pBase64 = new CBase64();
			strOutput.Insert(strOutput.GetLength(), pBase64->Encode(User_Pass, User_Pass.GetLength()));
			delete pBase64;
			pBase64 = NULL;
			
			pFile->AddRequestHeaders(strOutput);
			pFile->SendRequest();
			pFile->QueryInfoStatusCode(dwRet);
		}

		if(dwRet == 200)
		{
			if(dlen == 0)
			{
				pFile->Read(szReturn, 1024);
			}
			else
			{
				int nLast = 0;
				char buf[100 * 1024] = {0};
next:			int n = pFile->Read(buf, 100 * 1024 - nLast);
				if(n == 0) goto end;
				buf[n] = 0;
				strncat(szReturn, buf, n);
				if(n < 100 * 1024 - nLast)
				{
					nLast += n;
					goto next;
				}
			}
		}
		else
		{
			sprintf(szReturn, "%ld", dwRet);
			bResult = FALSE;
		}

end:	if(pFile) pFile->Close();
		if(pServer) pServer->Close();
	}
	catch (CInternetException* pEx)
	{
		TCHAR szErr[1024];
		pEx->GetErrorMessage(szErr, 1024);

		sprintf(szReturn, "%s", szErr);
		
		bResult = FALSE;

		pEx->Delete();
	}

	if (pFile != NULL)
		delete pFile;
	if (pServer != NULL)
		delete pServer;
	psession->Close();
	delete psession;

	return bResult;
}

BOOL GetIniFileName(const char *FileName, char *g_IniFileName)
{	
/*
    BOOL    bFindFlag = TRUE;
    CString strFileName = _T("");
#if _DEBUG
    strFileName.Format("%s\\templates.os\\%s", FuncGetInstallRootPath(), FileName);
#else
    strFileName.Format("%s\\MonitorManager\\templates.os\\%s",FuncGetInstallRootPath(), FileName);
#endif
//    printf("Config File is:%s\n", strFileName);
    strcpy(g_IniFileName, (LPCTSTR)strFileName);
	*/
	strcpy(g_IniFileName,FileName);
	return true;
//	return bFindFlag;
}

PARSER *ReturnParser(int nMonitorType)
{
	CString strPath=_T("");
#if _DEBUG
    strPath.Format("%s\\fcgi-bin\\AimParser.dll", FuncGetInstallPath());
#else
	strPath.Format("%s\\fcgi-bin\\AimParser.dll",::FuncGetInstallPath());
#endif
	g_hInstance = ::LoadLibrary(strPath);



	if (g_hInstance) 
	{
//		printf("load ok \n");
		switch(nMonitorType)
		{
		case HP_CPU_TYPE_MONITOR:
			return (PARSER *)::GetProcAddress(g_hInstance, "HP_CpuParser");

		case CPU_TYPE_MONITOR:
			return (PARSER *)::GetProcAddress(g_hInstance, "CpuParser");
		case DISK_TYPE_MONITOR:
			return (PARSER *)::GetProcAddress(g_hInstance, "DiskParser");
		case DISKS_TYPE_MONITOR:
			return (PARSER *)::GetProcAddress(g_hInstance, "DisksParser");
		case MEMORY_TYPE_MONITOR:
			return (PARSER *)::GetProcAddress(g_hInstance, "MemoryParser");
		case 0:
			return (PARSER *)::GetProcAddress(g_hInstance, "UnixPerfDisksParser");
		default:
			break;
		}
	}
//		printf("load failed!\n");

	return NULL;
}

PARSER2 *ReturnParser2(int nMonitorType)
{
	CString strPath=_T("");
	strPath.Format("%s\\fcgi-bin\\AimParser.dll",::FuncGetInstallPath());

	g_hInstance2 = ::LoadLibrary(strPath);
	if (g_hInstance2) 
	{
		switch(nMonitorType)
		{
		case SERVICE_TYPE_MONITOR:
			return (PARSER2 *)::GetProcAddress(g_hInstance2, "ServiceParser");
		default:
			break;
		}
	}

	return NULL;
}
//yi.duan 2012-2-21 卓望
BOOL MONITOR_PARSE_Process(char* matchStr,char *databuffer, char *inifile, char *extraparam, char *szReturn)
{
	HINSTANCE g_hIn=NULL;
	CString strPath=_T("");
	PARSER3 *pParser = NULL;

	strPath.Format("%s\\fcgi-bin\\AimParser.dll", FuncGetInstallPath());
	g_hIn = ::LoadLibrary(strPath);

	pParser=(PARSER3 *)GetProcAddress(g_hIn, "ServiceParserFilter");

	if(pParser)
	{
		BOOL bResult = (*pParser)(matchStr,databuffer, szReturn, extraparam, inifile);
		if(g_hIn) ::FreeLibrary(g_hIn);

		return bResult;
	}

	return FALSE;
}

BOOL MONITOR_PARSE(int monitortype, char *databuffer, char *inifile, char *extraparam, char *szReturn)
{
	WriteLog("============== MONITOR_PARSE =================");

	cout << "--------------- MONITOR_PARSE Start --------------"<<endl;

	cout << "inifile:" << inifile << endl;
	cout << "monitortype:" << monitortype << endl;

	PARSER *pParser = NULL;
	PARSER2 *pParser2 = NULL;
	HINSTANCE g_hIn=NULL;

	CString strPath=_T("");

	CString strFile=_T("MONITOR_PARSE");


    //strPath.Format("%s\\fcgi-bin\\AimParser.dll", FuncGetInstallPath());
	strPath.Format("%s\\fcgi-bin\\AimParser.dll", FuncGetInstallPath());
	//cout << "strPath:"<<strPath <<endl;

	//mtx???
	//strPath = "C:\\SITEVIEW6\\SiteView.DataCollection.WebService\\MonitorManager\\AimParser.dll";

	g_hIn = ::LoadLibrary(strPath);

//	printf("path:%s\n",strPath);

	if(!g_hIn)
	{
//		puts("调用AimParser.dll动态库出错!");
		printf("%s",FuncGetStringFromIDS("IDS_TransferDllError"));

		return FALSE;
	}
	if(monitortype > 0)
	{
		switch(monitortype)
		{
		case CPU_TYPE_MONITOR:
			pParser=(PARSER *)::GetProcAddress(g_hIn, "CpuParser");
			//cout << "--------------- CpuParser -----------------" <<endl;
			break;
		case DISK_TYPE_MONITOR:
			pParser=(PARSER *)::GetProcAddress(g_hIn, "DiskParser");break;
		case DISKS_TYPE_MONITOR:
			pParser=(PARSER *)::GetProcAddress(g_hIn, "DisksParser");break;
		case DISK_ACTIVITY:
			pParser=(PARSER *)::GetProcAddress(g_hIn, "DiskActParser");break;
		case DISK_ACTIVITY_MONITOR:
			pParser= (PARSER *)::GetProcAddress(g_hIn, "DisksActParser"); break;			
		case MEMORY_TYPE_MONITOR:
			pParser=(PARSER *)::GetProcAddress(g_hIn, "MemoryParser");break;

		case SWAP_TYPE_MONITOR://dy Swap分区
			pParser=(PARSER *)::GetProcAddress(g_hIn, "SwapParser");break;

		case PMEMORY_TYPE_MONITOR:
			pParser=(PARSER *)::GetProcAddress(g_hIn, "PMemoryParser");break;


		// 增加两个用于获取aix和solaris物理内存信息（ 邹晓 2009.02.25 ）
		case TOTAL_PMEMORY:
			pParser=(PARSER *)::GetProcAddress(g_hIn, "PMemoryParser");break;
		case FREE_PMEMORY:
			pParser=(PARSER *)::GetProcAddress(g_hIn, "PMemoryParser");break;


		case HP_CPU_TYPE_MONITOR:
			pParser=(PARSER *)::GetProcAddress(g_hIn, "HP_CpuParser");
			break;

		case SERVICE_TYPE_MONITOR:
			pParser2=(PARSER2 *)GetProcAddress(g_hIn, "ServiceParser");
//			pParser2 = ReturnParser2(monitortype);
			if(pParser2)
			{
				BOOL bResult = (*pParser2)(databuffer, monitortype, szReturn, extraparam, inifile);
				if(g_hIn) ::FreeLibrary(g_hIn);
//				if(g_hInstance2) ::FreeLibrary(g_hInstance2);

				/*

				WriteLog("szReturn=");
				WriteLog(szReturn);


				char* pstrTemp1=strstr(szReturn,"#");
				//WriteLog("pstrTemp1=");
				//WriteLog(pstrTemp1);
				char* pstrTemp2=strstr(szReturn,"$");
				//WriteLog("pstrTemp2=");
				//WriteLog(pstrTemp2);

				if(pstrTemp1!=NULL&&pstrTemp2!=NULL)
				{
					while(*pstrTemp1=='#')
						pstrTemp1++;

					//WriteLog("pstrTemp1=");
					//WriteLog(pstrTemp1);

					int n=pstrTemp2-pstrTemp1;
					if(n>0)
					{
                        char* pstrComp=new char[n+1];

						memcpy(pstrComp,pstrTemp1,n);
						*(pstrComp+n)='\0';

						//WriteLog("pstrComp=");
						//WriteLog(pstrComp);

						if(!strcmp(pstrComp,"挂接在"))
						{
							while(*pstrTemp2=='$')
								pstrTemp2++;

							strcpy(szReturn,pstrTemp2);
						}
					}
				}
				*/

				return bResult;
			}
			break;

		// 增加一个用于获取aix用户占用CPU信息（ 邹晓 2009.04.08 ）
		case USER_CPU_MONITOR:
			{
				WriteLog( "USER_CPU_MONITOR" );
				pParser=(PARSER *)::GetProcAddress( g_hIn, "UserCPUParser" );
				if(pParser2)
				{
					BOOL bResult = (*pParser2)(databuffer, monitortype, szReturn, extraparam, inifile);
					if(g_hIn) ::FreeLibrary(g_hIn);
					return bResult;
				}
				break;
			}


		//增加多核CPU监测 获取cpu序号（lish 2009.11.04）
		case MULCPUS_TYPE_MONITOR:
			{
				pParser=(PARSER *)::GetProcAddress(g_hIn, "MulCPUSParser");
				if (pParser)
				{
					WriteLog(strFile,databuffer);
				}
				break;
			}
		//增加多核CPU监测 获取cpu利用率（lish 2009.11.05）
		case MULCPU_TYPE_MONITOR:
			{
				//pParser2=(PARSER2 *)GetProcAddress(g_hIn, "MulCPUParser"); //（lish 2009.11.05）
				pParser2=(PARSER2 *)GetProcAddress(g_hIn, "CPUsParser");  //duanYi 2010.05.24
				if (pParser2)
				{
					BOOL bResult = (*pParser2)(databuffer, monitortype, szReturn, extraparam, inifile);
					if(g_hIn) ::FreeLibrary(g_hIn);
				
					return bResult;

				}
				break;
			}

		case MEMORYIO_TYPE_MONITOR:
			{
				pParser = (PARSER *) ::GetProcAddress(g_hIn,"MemoryIOParser");
				if (pParser)
				{
					WriteLog(strFile,databuffer);
				}
				break;
			}
		case SYSTEM_RUN_MONITOR:
			{
				pParser = (PARSER *) ::GetProcAddress(g_hIn,"SystemTimeParser");
			}
			break;

		case PROCESS_TYPE_MONITOR:
			{
				pParser2=(PARSER2 *)::GetProcAddress(g_hIn, "ProcessParser");
				if (pParser2)
				{
					cout << "---------------- GetProcAddress ProcessParser is success! -------------------"<<endl;
					BOOL bResult = (*pParser2)(databuffer, monitortype, szReturn, extraparam, inifile);
					if(g_hIn) ::FreeLibrary(g_hIn);
					return bResult;
				}
				break;
			}

		case PRONUM_TYPE_MONITOR:
			{
				pParser = (PARSER *) ::GetProcAddress(g_hIn, "ProcNumParser");
			}
			break;

		case DISKIO_TYPE_MONITOR:
			{
				pParser = (PARSER *) ::GetProcAddress(g_hIn,"DISKIOParser");
				if (pParser)
				{
					cout << "-------------- GetProcAddress DISKIOParser is success! ----------------- "<<endl;
				}

			}break;
		case ARCSDE_USER_MONITOR: //yi.duan 国家绘测局添加acesde 连接状态
			{
				pParser = (PARSER *) ::GetProcAddress(g_hIn,"arcsdeUserParser");
				if (pParser)
				{
					cout << "-------------- GetProcAddress arcsdeUserParser is success! ----------------- "<<endl;
				}

			}break;

		case AIX_ERRPT_LIST_MONITOR:
			pParser2=(PARSER2 *)GetProcAddress(g_hIn, "AixErrptListParser");
			//			pParser2 = ReturnParser2(monitortype);
			if(pParser2)
			{
				BOOL bResult = (*pParser2)(databuffer, monitortype, szReturn, extraparam, inifile);
				if(g_hIn) ::FreeLibrary(g_hIn);
		

				return bResult;
			}
			break;

		default:
			break;
		}

		if(pParser)
		{
			//puts("Get func address ok");
			BOOL bResult = (*pParser)(databuffer, monitortype, szReturn, inifile);
			if(g_hIn) ::FreeLibrary(g_hIn);

			/*
			WriteLog("szReturn=");
			WriteLog(szReturn);

			char* pstrTemp1=strstr(szReturn,"#");
			//WriteLog("pstrTemp1=");
			//WriteLog(pstrTemp1);
			char* pstrTemp2=strstr(szReturn,"$");
			//WriteLog("pstrTemp2=");
			//WriteLog(pstrTemp2);

			if(pstrTemp1!=NULL&&pstrTemp2!=NULL)
			{
				while(*pstrTemp1=='#')
					pstrTemp1++;
				//WriteLog("pstrTemp1=");
				//WriteLog(pstrTemp1);

				int n=pstrTemp2-pstrTemp1;
				if(n>0)
				{

					char* pstrComp=new char[n+1];
					memcpy(pstrComp,pstrTemp1,n);
					*(pstrComp+n)='\0';

					//WriteLog("pstrComp=");
					//WriteLog(pstrComp);

					if(!strcmp(pstrComp,"挂接在"))
					{
						while(*pstrTemp2=='$')
							pstrTemp2++;

						strcpy(szReturn,pstrTemp2);
					}
				}
			}
			*/

			printf("inifile = %s\n", inifile);
			WriteLog("ReturnValue=");
			printf("ReturnValue =");
			
			char* pstrReturn=szReturn;
			while(*pstrReturn)
			{
				WriteLog(pstrReturn);
				printf("ReturnValue = %s\r\n", pstrReturn);
				pstrReturn+=strlen(pstrReturn)+1;
				printf("strlen ReturnValue = %s\r\n", pstrReturn);
			}

			return bResult;
		}
	}
	else
	{
		// For UnixPerf Monitor
		char	*ca = NULL, *cb = NULL;
		char	szCpuData[8192] = {0}, 
				szMemoryData[8192] = {0}, 
				szDiskData[8192] = {0};

		// CPU Parser
		strncat(szReturn, "_cpu=", 5);

		ca = strstr(databuffer, "SiteViewCPU_Start");
		cb = strstr(databuffer, "SiteViewCPU_End");
		if(ca && cb)
		{
			ca = ca + 17;
			while(*ca == '\r' || *ca == '\n') ca ++;
			strncpy(szCpuData, ca, cb - ca);

			pParser = ReturnParser(CPU_TYPE_MONITOR);
			if(pParser)
			{
				if(!(*pParser)(szCpuData, CPU_TYPE_MONITOR, szReturn, inifile))
				{
					sprintf(szReturn, "%serror=%s", szReturn, FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));
				}
			}
			else
			{
				sprintf(szReturn, "%serror=%s", szReturn, FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));
			}

		}
		else
		{
			sprintf(szReturn, "%serror=%s", szReturn, FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));
		}

		strncat(szReturn, "@@@", 3);

		// Memory Parser
		strncat(szReturn, "_mem=", 5);

		ca = strstr(databuffer, "SiteViewMemory_Start");
		cb = strstr(databuffer, "SiteViewMemory_End");
		if(ca && cb)
		{
			ca = ca + 20;
			while(*ca == '\r' || *ca == '\n') ca ++;
			strncpy(szMemoryData, ca, cb - ca);

			pParser = ReturnParser(MEMORY_TYPE_MONITOR);
			if(pParser)
			{
				if(!(*pParser)(szMemoryData, MEMORY_TYPE_MONITOR, szReturn, inifile))
				{
					sprintf(szReturn, "%serror=%s", szReturn, FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));
				}
			}
			else
			{
				sprintf(szReturn, "%serror=%s", szReturn, FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));
			}
		}
		else
		{
			sprintf(szReturn, "%serror=%s", szReturn, FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));
		}

		strncat(szReturn, "@@@", 3);

		// Disks Parser
		strncat(szReturn, "_disk=", 6);

		ca = strstr(databuffer, "SiteViewDisk_Start");
		cb = strstr(databuffer, "SiteViewDisk_End");
		if(ca && cb)
		{
			ca = ca + 18;
			while(*ca == '\r' || *ca == '\n') ca ++;
			strncpy(szDiskData, ca, cb - ca);

			pParser = ReturnParser(0);
			if(pParser)
			{
				if(!(*pParser)(szDiskData, DISKS_TYPE_MONITOR, szReturn, inifile))
				{
					sprintf(szReturn, "%serror=%s", szReturn, FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));
				}
			}
			else
			{
				sprintf(szReturn, "%serror=%s", szReturn, FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));
			}
		}
		else
		{
			sprintf(szReturn, "%serror=%s", szReturn, FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));
		}
	}

	strncat(szReturn, "@@@", 3);
	if(g_hIn) ::FreeLibrary(g_hIn);

	if(g_hInstance) ::FreeLibrary(g_hInstance);
	if(g_hInstance2) ::FreeLibrary(g_hInstance2);

	return FALSE;
}

BOOL MONITOR_PARSE_zw(int monitortype, char *databuffer, char *inifile, char *extraparam, char* szTotalMem, char *szReturn)
{
	PARSER *pParser = NULL;
    PARSER_ZW *pParser_zw  = NULL;
	PARSER2 *pParser2 = NULL;
	HINSTANCE g_hIn=NULL;

	CString strPath=_T("");
#if _DEBUG
    strPath.Format("%s\\cgi-bin\\AimParser.dll", FuncGetInstallPath());
#else
	strPath.Format("%s\\fcgi-bin\\AimParser.dll",::FuncGetInstallPath());
#endif
	g_hIn = ::LoadLibrary(strPath);

//	printf("path:%s\n",strPath);

	if(!g_hIn)
	{
//		puts("调用AimParser.dll动态库出错!");
		printf("%s",FuncGetStringFromIDS("IDS_TransferDllError"));
		return FALSE;
	}


	if(monitortype > 0)
	{
		switch(monitortype)
		{
		case HP_CPU_TYPE_MONITOR:
			pParser=(PARSER *)::GetProcAddress(g_hIn, "HP_CpuParser");break;

		case CPU_TYPE_MONITOR:
			pParser=(PARSER *)::GetProcAddress(g_hIn, "CpuParser");break;
		case DISK_TYPE_MONITOR:
			pParser=(PARSER *)::GetProcAddress(g_hIn, "DiskParser");break;
		case DISKS_TYPE_MONITOR:
			pParser=(PARSER *)::GetProcAddress(g_hIn, "DisksParser");break;
		case MEMORY_TYPE_MONITOR:
			pParser_zw=(PARSER_ZW *)::GetProcAddress(g_hIn, "MemoryParser_zw");break;
/*			pParser = ReturnParser(monitortype);
			if(pParser)
			{
				BOOL bResult = (*pParser)(databuffer, monitortype, szReturn, inifile);
				if(g_hInstance) ::FreeLibrary(g_hInstance);
				if(g_hInstance2) ::FreeLibrary(g_hInstance2);
				return bResult;
			}
			break;*/
		case SERVICE_TYPE_MONITOR:
			pParser2=(PARSER2 *)GetProcAddress(g_hIn, "ServiceParser");
//			pParser2 = ReturnParser2(monitortype);
			if(pParser2)
			{
				BOOL bResult = (*pParser2)(databuffer, monitortype, szReturn, extraparam, inifile);
				if(g_hIn) ::FreeLibrary(g_hIn);
//				if(g_hInstance2) ::FreeLibrary(g_hInstance2);
				return bResult;
			}
			break;
		default:
			break;
		}
		if(pParser_zw)
		{
			BOOL bResult = (*pParser_zw)(databuffer, monitortype, szReturn, inifile, szTotalMem);
			if(g_hIn) ::FreeLibrary(g_hIn);
			return bResult;
		}
	}
	else
	{
		// For UnixPerf Monitor
		char	*ca = NULL, *cb = NULL;
		char	szCpuData[8192] = {0}, 
				szMemoryData[8192] = {0}, 
				szDiskData[8192] = {0};

		// CPU Parser
		strncat(szReturn, "_cpu=", 5);

		ca = strstr(databuffer, "SiteViewCPU_Start");
		cb = strstr(databuffer, "SiteViewCPU_End");
		if(ca && cb)
		{
			ca = ca + 17;
			while(*ca == '\r' || *ca == '\n') ca ++;
			strncpy(szCpuData, ca, cb - ca);

			pParser = ReturnParser(CPU_TYPE_MONITOR);
			if(pParser)
			{
				if(!(*pParser)(szCpuData, CPU_TYPE_MONITOR, szReturn, inifile))
				{
					sprintf(szReturn, "%serror=%s", szReturn, FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));
				}
			}
			else
			{
				sprintf(szReturn, "%serror=%s", szReturn, FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));
			}

		}
		else
		{
			sprintf(szReturn, "%serror=%s", szReturn, FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));
		}

		strncat(szReturn, "@@@", 3);

		// Memory Parser
		strncat(szReturn, "_mem=", 5);

		ca = strstr(databuffer, "SiteViewMemory_Start");
		cb = strstr(databuffer, "SiteViewMemory_End");
		if(ca && cb)
		{
			ca = ca + 20;
			while(*ca == '\r' || *ca == '\n') ca ++;
			strncpy(szMemoryData, ca, cb - ca);

			pParser = ReturnParser(MEMORY_TYPE_MONITOR);
			if(pParser)
			{
				if(!(*pParser)(szMemoryData, MEMORY_TYPE_MONITOR, szReturn, inifile))
				{
					sprintf(szReturn, "%serror=%s", szReturn, FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));
				}
			}
			else
			{
				sprintf(szReturn, "%serror=%s", szReturn, FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));
			}
		}
		else
		{
			sprintf(szReturn, "%serror=%s", szReturn, FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));
		}

		strncat(szReturn, "@@@", 3);

		// Disks Parser
		strncat(szReturn, "_disk=", 6);

		ca = strstr(databuffer, "SiteViewDisk_Start");
		cb = strstr(databuffer, "SiteViewDisk_End");
		if(ca && cb)
		{
			ca = ca + 18;
			while(*ca == '\r' || *ca == '\n') ca ++;
			strncpy(szDiskData, ca, cb - ca);

			pParser = ReturnParser(0);
			if(pParser)
			{
				if(!(*pParser)(szDiskData, DISKS_TYPE_MONITOR, szReturn, inifile))
				{
					sprintf(szReturn, "%serror=%s", szReturn, FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));
				}
			}
			else
			{
				sprintf(szReturn, "%serror=%s", szReturn, FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));
			}
		}
		else
		{
			sprintf(szReturn, "%serror=%s", szReturn, FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));
		}
	}

	strncat(szReturn, "@@@", 3);
	if(g_hIn) ::FreeLibrary(g_hIn);

	if(g_hInstance) ::FreeLibrary(g_hInstance);
	if(g_hInstance2) ::FreeLibrary(g_hInstance2);

	return FALSE;
}

extern "C" __declspec (dllexport)
BOOL RETURN_COMMAND(int monitortype, char *inifile, char *cmd)
{	
	WriteLog("\n\n============== RETURN_COMMAND =================");

	//OutputDebugString("=============== RETURN_COMMAND ================\n");

	WriteLog("inifile=");
	WriteLog(inifile);
	if(monitortype > 0)
	{
	//	char	*appname;
		char appname[255]={0};
		
	//	memset(appname,0,255);
		switch(monitortype)
		{
		case HP_CPU_TYPE_MONITOR:
			strcpy(appname, "HP_CPU");
		case CPU_TYPE_MONITOR:
			strcpy(appname, "cpu");
			//cout << "appname:" << appname <<endl;
			break;
		case DISK_TYPE_MONITOR:
			strcpy(appname, "disk");
			break;
		case DISKS_TYPE_MONITOR:
			strcpy(appname, "disks");
			break;
		case DISK_ACTIVITY:
			strcpy(appname, "diskact");
			break;
		case DISK_ACTIVITY_MONITOR:
			strcpy(appname, "diskList");
			break;
		case MEMORY_TYPE_MONITOR:
			strcpy(appname, "memory");
			break;
		case PMEMORY_TYPE_MONITOR:
			strcpy(appname, "physicalmemory");
			break;

		// 增加两个用于获取aix和solaris物理内存信息（ 邹晓 2009.02.25 ）
		case TOTAL_PMEMORY:
			strcpy( appname, "totalPhysicalMemory" );
			break;
		case FREE_PMEMORY:
			strcpy( appname, "freePhysicalMemory" );
			break;


		case SERVICE_TYPE_MONITOR:
			strcpy(appname, "service");
			break;

		// 增加一个用于获取aix用户占用CPU信息（ 邹晓 2009.04.08 ）
		case USER_CPU_MONITOR:
			strcpy(appname, "usercpu");
			break;

		case FILE_TYPE_MONITOR:
			strcpy(appname, "file");
			break;
		
		case MULCPUS_TYPE_MONITOR:
			strcpy(appname,"mulcpus");
			break;

		case MULCPU_TYPE_MONITOR:
			strcpy(appname,"mulcpus");
			break;

		case MEMORYIO_TYPE_MONITOR:
			strcpy(appname,"memoryio");
			break;
		case SYSTEM_RUN_MONITOR:
			strcpy(appname,"systime");
			break;
		case PROCESS_TYPE_MONITOR:
			strcpy(appname,"process");
			break;
		case PRONUM_TYPE_MONITOR:
			strcpy(appname,"process");
			break;
		case DISKIO_TYPE_MONITOR:
			strcpy(appname,"diskio");

			cout << "diskio:" << appname << endl;
			break;
		case SWAP_TYPE_MONITOR:
			strcpy(appname,"swap");

			cout << "swap:" << appname << endl;
			break;
		case ARCSDE_USER_MONITOR: //yi.duan 2010-12-08 国家绘测局ArcsdeUser
			strcpy(appname,"arcsdeUser");
			cout << "arcsdeUser:" << appname << endl;
			break;

		case AIX_ERRPT_LIST_MONITOR: //yi.duan 2011-06-14 易通科技 aix errpt
			strcpy(appname,"errpt");
			cout << "errpt:" << appname << endl;
			break;

		default:
			return FALSE;
		}
		char key[256];
		sprintf(key,"%s_command",appname);
		
		//if(appname)
		//	delete(appname);
		

		if(DFN_GetPrivateProfileString(inifile, key, "", cmd, 255, "oscmd.ini") <= 0)
		{
				//tmp.Format("faile ini -- %s--%s-%s--",appname,cmd,inifile);
				//WriteLogFile1(tmp);
				OutputDebugString("Get command failed!");
				return FALSE;			
		}

		//cout << "cmd:" <<cmd <<endl;
		WriteLog("cmd=");
		WriteLog(cmd);

		//OutputDebugString("cmd=");
		//OutputDebugString(cmd);
		//OutputDebugString("\n");


	}
	else
	{
		// For UnixPerf Monitor
		DWORD	dw = 0;
		char	szBuffer[256] = {0};
		
		strncpy(cmd, "echo SiteViewCPU_Start;", 23);
		memset(szBuffer, 0, sizeof(szBuffer));
		dw = DFN_GetPrivateProfileString("CPU", "command", "", szBuffer, sizeof(szBuffer), inifile);
		strncat(cmd, szBuffer, dw);
		strncat(cmd, ";", 1);
		strncat(cmd, "echo SiteViewCPU_End;", 21);

		strncat(cmd, "echo SiteViewMemory_Start;", 26);
		memset(szBuffer, 0, sizeof(szBuffer));
		dw = DFN_GetPrivateProfileString("Memory", "command", "", szBuffer, sizeof(szBuffer), inifile);
		strncat(cmd, szBuffer, dw);
		strncat(cmd, ";", 1);
		strncat(cmd, "echo SiteViewMemory_End;", 24);

		strncat(cmd, "echo SiteViewphysicalMemory_Start;", 34);
		memset(szBuffer, 0, sizeof(szBuffer));
		dw = DFN_GetPrivateProfileString("physicalMemory", "command", "", szBuffer, sizeof(szBuffer), inifile);
		strncat(cmd, szBuffer, dw);
		strncat(cmd, ";", 1);
		strncat(cmd, "echo SiteViewphysicalMemory_End;", 32);

		strncat(cmd, "echo SiteViewDisk_Start;", 24);
		memset(szBuffer, 0, sizeof(szBuffer));
		dw = DFN_GetPrivateProfileString("Disks", "command", "", szBuffer, sizeof(szBuffer), inifile);
		strncat(cmd, szBuffer, dw);
		strncat(cmd, ";", 1);
		strncat(cmd, "echo SiteViewDisk_End;", 22);

	}
	return TRUE;
}

int socks5_protocol(SOCKET s, char *server, int port, char *pserver, int pport, 
					char *puid, char *ppwd, int timeout)
{
	struct hostent *hent;
	unsigned long	ip;
	sockaddr_in		sa;

	hent = gethostbyname(pserver);
	if(hent == NULL)
		return 1;

	memcpy(&sa.sin_addr, hent->h_addr_list[0], hent->h_length);
	sa.sin_family = AF_INET;
	sa.sin_port = htons(pport);

	if(connect_timeo(s, (sockaddr*)&sa, sizeof(sockaddr), timeout) < 0)
		return 1;

	unsigned char request[22];
	int request_len = 0;
	unsigned char response[22];
	int response_len = 0;

	request[0] = 0x05; // SOCKS version 5
	request[1] = 0x03; // number of auth procotols we recognize
	request[2] = 0x00; // no authentication required
	request[3] = 0x01; // GSSAPI
	request[4] = 0x02; // username/password

	request_len = 2 + request[1];
	int write_len = send(s, (const char *)request, request_len, 0);
	if (write_len != request_len) 
		return 1;

	response_len = 22;
	response_len = recv(s, (char *)response, response_len, 0);

	if (response_len <= 0) 
		return 1;

	if (response[0] != 0x05) 
		return 1;

	switch (response[1]) 
	{
		case 0x00:
			// no auth
			break;
		case 0x01:
			// GSSAPI
			return 1;
		case 0x02:
			{
			// username/password
			int i = 0;
			request[0] = 0x01;
			if(*puid)
			{
				request[1] = strlen(puid);
				for(i = 0;i < (int)strlen(puid);i ++)
					request[2+i] = puid[i];
			}
			else
			{
				request[1] = 0x01;
				request[2] = 0x00;
			}
			request_len = 2 + i;

			if(*ppwd)
			{
				request[request_len++] = strlen(ppwd);
				for(i = 0;i < (int)strlen(ppwd);i ++)
					request[request_len+i] = ppwd[i];
				request_len += strlen(ppwd);
			}
			else
			{
				request[request_len++] = 0x01;
				request[request_len++] = 0x00;
			}

			write_len = send(s, (const char *)request, request_len, 0);
			response_len = 22;
			response_len = recv(s, (char *)response, response_len, 0);
			if(response[0] != 1)
				return 1;
			if(response[1] != 0)
				return 1;
			}
			break;
		default:
			return 1;
	}

	// we are now authenticated, so lets tell
	// the server where to connect to
	request_len = 6;

	request[0] = 0x05; // SOCKS version 5
	request[1] = 0x01; // CONNECT command
	request[2] = 0x00; // obligatory reserved field (perfect for MS tampering!)
    
	request[3] = 0x01; // encoding of destination address (1 == IPv4)
	request_len += 4;

	char	ipaddr[20] = {0};
	ip = inet_addr(server);
	if (ip == INADDR_NONE)
	{
		hent = gethostbyname(server);
		if (hent == NULL)
			return 1;

		strcpy(ipaddr, inet_ntoa(*((struct in_addr *)hent->h_addr)));
	}
	else
	{
		strcpy(ipaddr, server);
	}
	
	if(sscanf(ipaddr, "%d.%d.%d.%d", &request[4], &request[5], &request[6], &request[7]) != 4)
		return 1;

	// destination port
	unsigned int destPort = htons(port);

	request[request_len-1] = (unsigned char)(destPort >> 8);
	request[request_len-2] = (unsigned char)destPort;

	if (send(s, (const char *)request, request_len, 0) != request_len)
		return 1;

	response_len = 22;
	response_len = recv(s, (char *)response, response_len, 0);
	if (response_len <= 0)
		return 1;

	if (response[0] != 0x05)
		return 1;

	if (response[1] != 0x00)
		return 1;

	return 0;
}

#define MAXL	200	/* Maximum length of last in & out fields */

double diff(char *a, char *b)
{
    char res[MAXL], *a1, *b1, *r1;
    int c,x,m;
    if (*a == '-' && *b == '-') 
	{
       b1 = b + 1;
       b = a + 1;
       a = b1;
    }
    
    while (!isdigit((int)*a)) a++;
    while (!isdigit((int)*b)) b++;
    a1 = &a[strlen(a)-1];
    m = max(strlen(a),strlen(b));
    r1 = &res[m+1];
    for (b1 = res;b1 <= r1; b1++) *b1 = ' ';
    b1 = &b[strlen(b)-1];
    r1[1] = 0;	/* Null terminate result */
    c = 0;
    for (x=0; x<m; x++) 
	{
		if (a1 >= a && b1 >= b) 
		{
			*r1 = ((*a1 - c) - *b1) + '0';
		} 
		else if (a1 >= a) 
		{
			*r1 = (*a1 - c);
		} 
		else 
		{
			*r1 = ('0' - *b1 - c) + '0';
		}
	
		if (*r1 < '0') 
		{
			*r1 += 10; 
			c=1;
		} 
		else 
		{
			c=0;
		}

		a1--;b1--;r1--;
    }

    if (c) 
	{
		r1 = &res[m+1];
		for (x=0; isdigit((int)*r1) && x<m; x++,r1--)  
		{
			*r1 = ('9' - *r1 + c) + '0';
			if (*r1 > '9') 
			{
				*r1 -= 10; 
				c=1;
			} 
			else 
			{
				c=0;
			}
		}

		return(-atof(res));
    } 
	else
		return(atof(res));
}

int get_ipaddrs(char *domain, char ipaddrs[64][20])
{
	int		i = 0;
	struct	hostent *hp;

	memset(ipaddrs, 0, sizeof(ipaddrs));

	if(WSA_Init())
	{
		return 1;
	}

	hp = gethostbyname(domain);
	if(hp == NULL)
	{
		return 1;
	}

	while(1)
	{
		if(hp->h_addr_list[i])
		{
			strcpy(ipaddrs[i], inet_ntoa(*((struct in_addr *)hp->h_addr_list[i])));
		}
		else
		{
			break;
		}

		if(i++ >= 20) 
			break;
	}

	WSA_Free();

	return 0;
}

/********************************************************
 *	standard average: 
 *		v = (a1+a2+....+an)/n 
 * 
 *	standard deviation: 
 *		v^2 = [(a1-a)^2+(a2-a)^2+....+(an-a)^2 ]/n 
 * 
 *	standard variation: 
 *		v = [(a1-a)^2+(a2-a)^2+....+(an-a)^2 ]/n 
 * 
 *	standard error: 
 *		v = [|a1-a|+|a2-a|+....+|an-a|]/n 
 ********************************************************/
double std_avg(double *dl_array, int n)
{
	int		i = 0;
	double	dl_ave = 0.0;

	for(i = 0;i < n;i ++)
		dl_ave += dl_array[i] / n;

	return dl_ave;
}

double std_dev(double *dl_array, int n)
{
	int		i = 0;
	double	dl_ave = 0.0;
	double	dl_tot = 0.0;

	dl_ave = std_avg(dl_array, n);

	for(i = 0;i < n;i ++)
		dl_tot += pow((dl_array[i] - dl_ave), 2) / n;

	return sqrt(dl_tot);
}

double std_var(double *dl_array, int n)
{
	return pow(std_dev(dl_array, n), 2);
}

double std_err(double *dl_array, int n)
{
	int		i = 0;
	double	dl_ave = 0.0;
	double	dl_tot = 0.0;

	dl_ave = std_avg(dl_array, n);

	for(i = 0;i < n;i ++)
		dl_tot += fabs(dl_array[i] - dl_ave) / n;

	return dl_tot;
}

void OsDetect(CString &strOsInfo) 
{
	OSVERSIONINFO	OSversion;
	
	OSversion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	::GetVersionEx(&OSversion);

	switch(OSversion.dwPlatformId)
	{
    case VER_PLATFORM_WIN32s: 
		strOsInfo.Format("Windows %d.%d",OSversion.dwMajorVersion, OSversion.dwMinorVersion);
		break;
	case VER_PLATFORM_WIN32_WINDOWS:
		if(OSversion.dwMinorVersion == 0)
			strOsInfo = "Windows 95";  
		else if(OSversion.dwMinorVersion == 10)  
			strOsInfo = "Windows 98";
		else if(OSversion.dwMinorVersion == 90)  
			strOsInfo = "Windows Me";	
		break;        
	case VER_PLATFORM_WIN32_NT:
		if(OSversion.dwMajorVersion == 5 && OSversion.dwMinorVersion == 0)
			strOsInfo.Format("Windows 2000 With %s",OSversion.szCSDVersion);
		else if(OSversion.dwMajorVersion == 5 && OSversion.dwMinorVersion == 1)
			strOsInfo.Format("Windows XP %s",OSversion.szCSDVersion);
		else if(OSversion.dwMajorVersion <= 4) 	  
			strOsInfo.Format("Windows NT %d.%d with %s",OSversion.dwMajorVersion,
			   OSversion.dwMinorVersion,OSversion.szCSDVersion);
		else	
			// for unknown windows/newest windows version	  
			strOsInfo.Format("Windows %d.%d ",OSversion.dwMajorVersion,
				OSversion.dwMinorVersion);
		break;
	default:
		strOsInfo.Format("%s", "Unknown Operating System");
		break;
	}
}
