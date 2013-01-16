// DnsMonitor.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "DnsMonitor.h"
#include <fstream>

#include ".\base\funcGeneral.h"
#include ".\base\Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CDnsMonitorApp

BEGIN_MESSAGE_MAP(CDnsMonitorApp, CWinApp)
	//{{AFX_MSG_MAP(CDnsMonitorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDnsMonitorApp construction

CDnsMonitorApp::CDnsMonitorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDnsMonitorApp object

CDnsMonitorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDnsMonitorApp initialization

//DEL BOOL CDnsMonitorApp::InitInstance()
//DEL {
//DEL 	if (!AfxSocketInit())
//DEL 	{
//DEL 		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
//DEL 		return FALSE;
//DEL 	}
//DEL 
//DEL 	return TRUE;
//DEL }

BOOL DNS_MONITOR(char *server, int port, char *domain, CStringList &lstAddress, 
				 char *custpath, char *szReturn);
void DNSMessageParser(u_char *buff, int len);
int VerifyDNS(u_char *result, int len, CStringList &lstAddress);

void WriteLogFile(char* chMsg);

void WriteLogFile(char* chMsg)
{
/*
    FILE *stream;
    char chTime[50] = {0};
    //打开文件
#if _DEBUG
    stream=fopen(FuncGetInstallRootPath() + "\\Temp\\DNSMonitor.log","a");
#else
    stream=fopen(FuncGetInstallRootPath() + "\\MonitorManager\\Temp\\DNSMonitor.log","a");
#endif    
    if(stream == NULL)
        return;//打开文件失败，直接返回
   	SYSTEMTIME time;
	//取系统当前时间
	GetLocalTime(&time);
    //sprintf(chTime ,"%02d年-%02d月-%02d日 %02d时-%02d分-%02d秒 ",
    sprintf(chTime ,"%dY-%dM-%d D %dH-%dM-%dS ",
				//time.wYear - 2000,
                time.wYear,
				time.wMonth,
				time.wDay,
				time.wHour,
				time.wMinute,
				time.wSecond
			);
    fputs(chTime , stream);
	fputs(chMsg,stream);
    fputs("\n",stream);
	fclose(stream);
*/
}

int PrintLog(const char * strMsg)
{
//#ifndef _DEBUG
//	return 1;
//#endif
	char timebuf[128] = {0};
	char datebuf[128] = {0};
	char tempbuf[1024] = {0};

	_strtime(timebuf);
	_strdate(datebuf);
	sprintf(tempbuf,"%s-%s",datebuf,timebuf);

	ofstream FileStream;
	FileStream.open("DNSMonitor.log",ios::app);
	FileStream<<tempbuf<<"\t"<<strMsg<<endl;
	FileStream.close();

	return 0;
}
BOOL DNS_MONITOR(char *server, int port, char *domain, CStringList &lstAddress, 
				 char *custpath, char *szReturn)
{
	u_char reply[1000];
	SOCKET udpSocket;
    struct sockaddr_in serv;
	CTime time1,time2;
	unsigned int n;
//	int n;
	int dw, ret, Count = 0, iCount = 0;
	LONG	dwBeginTime, dwElapseTime;

	char strTempInfo[1024] = {0};

	u_char request[1000] =
				{ 
				0x01, 0x01, 0x01, 0x00,
				0x00, 0x01, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00
				};
	Count = 12;

	for(n = 0;n < (int)strlen(domain);n ++)
	{
		if(domain[n] != '.')
		{
			iCount ++;
			request[Count + iCount] = domain[n];
		}
		else
		{
			request[Count] = iCount;
			Count = Count + iCount + 1;
			iCount = 0;
		}
	}
	request[Count] = iCount;
	Count = Count + iCount + 1;
	request[Count] = 0x00;
	request[Count + 1] = 0x00;
	request[Count + 2] = 0x01;
	request[Count + 3] = 0x00;
	request[Count + 4] = 0x01;
	Count += 5;

	udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if(udpSocket == INVALID_SOCKET)
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_SOCKET",
            "SOCKET_CREATE_FAILED"));
		return FALSE;
	}

	struct hostent *hp;
	hp = gethostbyname(server);
	if(!hp)
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_DNS",
            "DNS_DOMAIN_NAME_PARSE_ERROR"));
		return FALSE;
	}
	int i =0 ;
	char *ip;
	while(hp->h_addr_list[i] != NULL)
	{
		ip = inet_ntoa (*(struct in_addr *)hp->h_addr_list[i]);
		printf("IP=%s\nh_length=%d\n",ip,hp->h_length);
		i++;
	}
	memcpy(&serv.sin_addr,hp->h_addr_list[0],hp->h_length);
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);
	
	
	dwBeginTime = GetTickCount();
    n = sendto(udpSocket, (char *)request, Count, 0, (struct sockaddr *)&serv, sizeof(serv));


	if(n == SOCKET_ERROR)
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_SOCKET",
            "SOCKET_SEND_DATA_ERROR"));
		return FALSE;
	}
	int nRound = 0;
UdpSelect:	
	ret = RESPONSE_WAIT(udpSocket, DNS_TIMEOUT);
	DWORD dwEndTime;

	printf("ret=%d\n", ret);

	if(ret > 0)
	{
		dw = sizeof(serv);
		if((n = recvfrom(udpSocket, (char *)reply, 1000, 0, (struct sockaddr *)&serv, &dw)) < 0)
		{
            sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_SOCKET",
                "SOCKET_RECV_DATA_ERROR"));
			return FALSE;
		}
		
		int n = (int)(reply[3] & 0x0F);
		printf("reply=%s\n",reply);
		printf("n=%d\n",n);


		switch(reply[3] & 0x0F)
		{
		case 0:
			break;
		case 1:
			// Malformed DNS Request Packet
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_DNS",
                "DNS_SERVER_NAME_CANNOT_PARSER"));
			return FALSE;
			//break;
		case 2:
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_DNS",
                "DNS_SERVER_NAME_CANNOT_PARSER"));
			return FALSE;
		case 3:
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_DNS",
                "DNS_SERVER_NAME_CANNOT_PARSER"));
			return FALSE;
		}

		if(reply[0] == request[0] && reply[1] == request[1])
		{
			if(reply[6] * 256 + reply[7] == 0)
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_DNS",
                    "DNS_SERVER_NAME_CANNOT_PARSER"));
				return FALSE;
			}

			//DNSMessageParser(reply, n);

			if(lstAddress.GetCount() != 0)
			{
				ret = VerifyDNS(reply, n, lstAddress);
				if(ret == 0)
				{
					dwEndTime = GetTickCount();
//					dwElapseTime = GetTickCount() - dwBeginTime;
					dwElapseTime = dwEndTime - dwBeginTime;
					sprintf(szReturn, "roundTripTime=%ld$", dwElapseTime);

					return TRUE;
				}
				else if(ret == -1)
				{
					sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_SOCKET",
                        "SOCKET_CHECK_IP_ERROR"));
					return FALSE;
				}
				else if(ret == -2)
				{
					sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_SOCKET",
                        "SOCKET_CHECK_IP_ERROR")); 
					return FALSE;
				}
				else
				{
					sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_SOCKET",
                        "SOCKET_UNKNOWN_ERROR"));
					return FALSE;
				}
			}
			else
			{
				dwEndTime = GetTickCount();
//				dwElapseTime = GetTickCount() - dwBeginTime;
				dwElapseTime = dwEndTime - dwBeginTime;
				sprintf(szReturn, "roundTripTime=%ld$", dwElapseTime);
			}
		}
		else
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_SOCKET",
                "SOCKET_DATA_TYPE_ERROR"));//<%IDS_DNS_13%>
			return FALSE;
		}
	}
	else
	{
		if(ret == 0)
		{
			if(nRound >=5)
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_SOCKET",
					"SOCKET_RECV_DATA_TIMEOUT"));	//<%IDS_DNS_14%>
				return FALSE;
			}
			else
			{
				Sleep(1000);
				goto UdpSelect;
			}
		}
		else
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_SOCKET",
                "SOCKET_RECV_DATA_ERROR"));//<%IDS_DNS_15%>
			return FALSE;
		}
	}

	return TRUE;
}

void DNSMessageParser(u_char *buff, int len)
{
	int i = 0;
	int flen = 0;
	u_char *ptr = buff;

	for(i = 0;i < len;i ++)
	{
		if( i % 16 == 0) printf("\r\n");
		printf("%2.2x ", buff[i]);
	}
	printf("\r\n\r\n");

	// Identification
	printf("Identification: %2.2x%2.2x\r\n", *ptr, *(ptr + 1));
	ptr += 2, flen += 2;

	// Parameter
	printf("Parameter: %2.2x%2.2x\r\n", *ptr, *(ptr + 1));
	ptr += 2, flen += 2;

	// Number Of Questions
	printf("Question Number: %d\r\n", *ptr * 256 + *(ptr + 1));
	ptr += 2, flen += 2;

	// Number Of Answers
	printf("Answer Number: %d\r\n", *ptr * 256 + *(ptr + 1));
	ptr += 2, flen += 2;

	// Number Of Authority
	printf("Authority Number: %d\r\n", *ptr * 256 + *(ptr + 1));
	ptr += 2, flen += 2;

	// Number Of Additional
	printf("Additional Number: %d\r\n\r\n", *ptr * 256 + *(ptr + 1));
	ptr += 2, flen += 2;

	// Question Section
	printf("Question Section: \r\n");
	printf("	Query Domain Name: ");
	while(*ptr && ((*ptr & 0xC0) != 0xC0))
	{
		for(i = 0;i < *ptr;i ++)
			printf("%c", *(ptr + 1 + i));

		flen += 1 + *ptr, ptr += 1 + *ptr;
		if(*ptr && ((*ptr & 0xC0) != 0xC0))
			printf("%c", '.');
	}
	printf("\r\n");
	ptr ++, flen ++;

	printf("	Query Type: %d\r\n", *ptr * 256 + *(ptr + 1));
	ptr += 2, flen += 2;

	printf("	Query Class: %d\r\n\r\n", *ptr * 256 + *(ptr + 1));
	ptr += 2, flen += 2;

	// Answer Section
	printf("Answer Section: \r\n");
	printf("	Resource Domain name: ");
	if((*ptr & 0xC0) == 0xC0)
	{
		flen += (*ptr & 0x3F) * 0xFF + *(ptr + 1);
		ptr += (*ptr & 0x3F) * 0xFF + *(ptr + 1);
		while(*ptr && ((*ptr & 0xC0) != 0xC0))
		{
			for(i = 0;i < *ptr;i ++)
				printf("%c", *(ptr + 1 + i));

			flen += 1 + *ptr, ptr += 1 + *ptr;
			if(*ptr && ((*ptr & 0xC0) != 0xC0))
				printf("%c", '.');
		}
	}
	else
	{
		while(*ptr && ((*ptr & 0xC0) != 0xC0))
		{
			for(i = 0;i < *ptr;i ++)
				printf("%c", *(ptr + 1 + i));

			flen += 1 + *ptr, ptr += 1 + *ptr;
			if(*ptr && ((*ptr & 0xC0) != 0xC0))
				printf("%c", '.');
		}
	}
	printf("\r\n\r\n");

	if(*ptr == 0xC0) ptr += 2, flen += 2;
	if(!*ptr) ptr ++, flen ++;

	u_char st_1 = *ptr;
	u_char st_2 = *(ptr + 1);
	while(*ptr == st_1 && *(ptr + 1) == st_2)
	{
		ptr += 2, flen += 2;

		printf("	Resource Type: %d\r\n", *ptr * 256 + *(ptr + 1));
		ptr += 2, flen += 2;

		printf("	Resource Class: %d\r\n", *ptr * 256 + *(ptr + 1));
		ptr += 2, flen += 2;

		printf("	Resource Time To Live: %ld\r\n", *ptr * 256 * 256 * 256 + *(ptr + 1) * 256 * 256 + *(ptr + 2) * 256 + *(ptr + 3));
		ptr += 4, flen += 4;

		printf("	Resource Data Length: %d\r\n", *ptr * 256 + *(ptr + 1));
		ptr += 2, flen += 2;

		printf("	Resource Data: %d.%d.%d.%d\r\n\r\n", *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3));
		ptr += 4, flen += 4;
	}

	// Authority Section
	printf("Authority Section: \r\n");

	st_1 = *ptr;
	st_2 = *(ptr + 1);
	while(*ptr == st_1 && *(ptr + 1) == st_2)
	{
		ptr += 2, flen += 2;

		int type = *ptr * 256 + *(ptr + 1);
		printf("	Resource Type: %d\r\n", type);
		ptr += 2, flen += 2;

		printf("	Resource Class: %d\r\n", *ptr * 256 + *(ptr + 1));
		ptr += 2, flen += 2;

		printf("	Resource Time To Live: %ld\r\n", *ptr * 256 * 256 * 256 + *(ptr + 1) * 256 * 256 + *(ptr + 2) * 256 + *(ptr + 3));
		ptr += 4, flen += 4;

		int rlen = *ptr * 256 + *(ptr + 1);
		printf("	Resource Data Length: %d\r\n", rlen);
		ptr += 2, flen += 2;

		printf("	Resource Data: ");
		switch(type)
		{
		case 1:
			printf("%d.%d.%d.%d", *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3));
			ptr += rlen, flen += rlen;
			break;
		case 2:
			while(*ptr && ((*ptr & 0xC0) != 0xC0))
			{
				for(i = 0;i < *ptr;i ++)
					printf("%c", *(ptr + 1 + i));

				flen += 1 + *ptr, ptr += 1 + *ptr;
				if(*ptr && ((*ptr & 0xC0) != 0xC0))
					printf("%c", '.');
			}

			if(*ptr == 0xC0) ptr += 2, flen += 2;
			if(!*ptr) ptr ++, flen ++;
			break;
		default:
			for(i = 0;i < rlen;i ++)
				printf("%c", *(ptr + i));
			ptr += rlen, flen += rlen;
			break;
		}

		printf("\r\n\r\n");
		
	}

	// Additional Informatoin Section
	printf("Additional Information Section: \r\n");

	while(flen < len)
	{
		ptr += 2, flen += 2;

		int type = *ptr * 256 + *(ptr + 1);
		printf("	Resource Type: %d\r\n", type);
		ptr += 2, flen += 2;

		printf("	Resource Class: %d\r\n", *ptr * 256 + *(ptr + 1));
		ptr += 2, flen += 2;

		printf("	Resource Time To Live: %ld\r\n", *ptr * 256 * 256 * 256 + *(ptr + 1) * 256 * 256 + *(ptr + 2) * 256 + *(ptr + 3));
		ptr += 4, flen += 4;

		int rlen = *ptr * 256 + *(ptr + 1);
		printf("	Resource Data Length: %d\r\n", rlen);
		ptr += 2, flen += 2;

		printf("	Resource Data: ");
		switch(type)
		{
		case 1:
			printf("%d.%d.%d.%d", *ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3));
			ptr += rlen, flen += rlen;
			break;
		case 2:
			while(*ptr && ((*ptr & 0xC0) != 0xC0))
			{
				for(i = 0;i < *ptr;i ++)
					printf("%c", *(ptr + 1 + i));

				flen += 1 + *ptr, ptr += 1 + *ptr;
				if(*ptr && ((*ptr & 0xC0) != 0xC0))
					printf("%c", '.');
			}

			if(*ptr == 0xC0) ptr += 2, flen += 2;
			if(!*ptr) ptr ++, flen ++;
			break;
		default:
			for(i = 0;i < rlen;i ++)
				printf("%c", *(ptr + i));
			ptr += rlen, flen += rlen;
			break;
		}

		printf("\r\n\r\n");
	}
}

int VerifyDNS(u_char *result, int len, CStringList &lstAddress)
{
	int	addr_1 = 0,
		addr_2 = 0,
		addr_3 = 0,
		addr_4 = 0;
	int ret = 0;

	POSITION pos = lstAddress.FindIndex(0);
	while(pos)
	{
		CString strAddress = lstAddress.GetNext(pos);
		for(int i = 0;i < strAddress.GetLength();i ++)
		{
			TCHAR ch = strAddress.GetAt(i);
			if(ch != '.' && !isdigit(ch))
				return -1;
		}

		ret = sscanf(strAddress, "%d.%d.%d.%d", &addr_1, &addr_2, &addr_3, &addr_4);
		if(ret != 4)
		{
			return -1;
		}

		for(i = 0;i <= len - 4;i ++)
		{
			if(	result[i] == addr_1 && result[i + 1] == addr_2 &&
				result[i + 2] == addr_3 && result[i + 3] == addr_4)
				break;
		}

		if(i == len - 3) return -2;
	}

	return 0;
}
/****************************************************************************
	DNS Monitor Export Function
****************************************************************************/
//extern "C" __declspec(dllexport) 
//BOOL DNS(CStringList &paramList, char *szReturn)
extern "C" __declspec(dllexport) 
bool DNS(char* strParas ,char *szReturn ,int nOutSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList paramList;
	MakeStringListByChar(paramList, strParas );

	CString		strDNSServ = _T(""), 
				strDomain = _T("");
	int			nDNSPort = 53;

	CStringList	lstAddress;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strDNSServ = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nDNSPort = atoi(strTemp);
			}
			else
			{
				strDNSServ = strTemp;
			}
		}
		else if(strTemp.Find(__DOMAINNAME__, 0) == 0)
		{
			strDomain = strTemp.Right(strTemp.GetLength() - strlen(__DOMAINNAME__));
		}
		else if(strTemp.Find(__MATCHIP__, 0) == 0)
		{
			int index = 0;
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__MATCHIP__));
			while(1)
			{
				index = strTemp.Find(',', 0);
				if(index > 0)
				{
					lstAddress.AddTail(strTemp.Left(index));
					strTemp.Delete(0, index + 1);
				}
				else if(index == 0)
				{
					strTemp.Delete(0, 1);
				}
				else
				{
					if(strTemp.IsEmpty())
						break;
					else
					{
						lstAddress.AddTail(strTemp);
						break;
					}
				}
			}
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
	}

	if(strDNSServ.IsEmpty())
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_DNS", 
            "DNS_DOMAIN_SERVER_NOT_FOUND"));
		return FALSE;
	}

	if(strDomain.IsEmpty())
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_DNS",
            "DNS_DOMAIN_NAME_NOT_FOUND"));//<%IDS_Monitor_23%>"缺少要查询IP地址的域名"
		return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_24%>"通信初始化错误"
		return FALSE;
	}

	BOOL bResult = DNS_MONITOR(strDNSServ.GetBuffer(strDNSServ.GetLength()), 
							nDNSPort, 
							strDomain.GetBuffer(strDomain.GetLength()), 
							lstAddress, 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							szReturn);


	WSA_Free();


	CString strInput =szReturn;
	MakeCharByString( szReturn,nOutSize,strInput);
    //WriteLogFile(szReturn);

    //if(strlen(szReturn) ==0)
    //{
    //    WriteLogFile("Unknown Error!");
    //}
	return bResult;
}
