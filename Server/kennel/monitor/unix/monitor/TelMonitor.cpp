
#include "TelMonitor.h"
#include "Utils.h"
#include <fstream>
#include "base\funcGeneral.h"

//int		term_index = 0;
//char	databuffer[BUF_SIZE] = {0};
//int		datalen = 0;

//char	codebuf[256];
//unsigned char codeptr;
#ifdef WIN32
#pragma warning (disable : 4267)
#endif

extern void WriteLogFile(const char* szMsg);

int WriteLog( const char* str );

struct OL ol[] = {
	{TOPT_ECHO,   ddww_echo,  NULL},
	{TOPT_SUPP,   ddww_supp,  NULL},
	{TOPT_TERM,   ddww_term,  sbproc_term},
	{TOPT_ERROR,  ddww_error, NULL}
};

struct TERMINAL terminal[NUM_TERMINALS] = {
	{ "NVT", nvt }, 
	{ "ANSI", ansi }
};

struct CODEC codec[NUM_CODEC] = {
	{'m',ansi_set_screen_attribute},
	{'H',ansi_set_position},
	{'K',ansi_erase_line},
	{'J',ansi_erase_screen},
	{'A',ansi_move_up},
	{0,0}
};

//◊øÕ˚Ãÿ¿˝
BOOL MonitorValue_zw(char *server, int port, char *uid, char *pwd, 
				  char *proxy_server, int proxy_port, char *proxy_uid, char *proxy_pwd, 
				  char *logprompt, char *pwdprompt, char *prompts, 
				  char *inifile, char* szTotalMem, int mtype, 
                  char *extraparam, char *custpath, char *szReturn)
{
	int nTime=0;
	LPGDATA pgd=new GDATA;
	HANDLE hMutex=NULL;
	CString strMutex=_T(""),strTemp=_T("");

LOOP:
	memset(pgd,0,sizeof(GDATA));
	int		ret = 0;
	int		hSocket = 0;
	BOOL	bResult = TRUE;
	char	iniFile[255] = {0};
	char	cmd[255] = {0};

//    CString strlog=_T("");
//	strlog.Format("%s\\monitormanager\\outlog.txt",::FuncGetInstallPath());
/*	FILE *pf=fopen(strlog,"a+");
	if(pf)
	{
		fputs("dddddd",pf);
	}*/

	/*************************************
	 * For Reason Of Prompt
	 */
	int		c = 0;
	char	*ca = prompts, *cb = NULL;
	char	arprompt[PR_NUM][256];
	memset(arprompt, 0, sizeof(arprompt));

	while(cb = strchr(ca, 0x20))
	{
		if(ca == cb) 
		{
			ca ++;
			continue;
		}

		strncpy(arprompt[c++], ca, cb - ca);
		ca = cb + 1;
	}

	strcpy(arprompt[c++], ca);
	/***********************************/


	hSocket = telnet_init(server, port, proxy_server, proxy_port, proxy_uid, proxy_pwd);
	if(hSocket <= 0)
	{
		if(hSocket == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_PARSER_SERVER_FAILED"));    //  <%IDS_TelMonitor_1%>
		else if(hSocket == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
            "SOCKET_COMMUNICATE_ERROR"));   //  <%IDS_TelMonitor_2%>
		else if(hSocket == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX",
            "UNIX_TIMEOUT_ERROR"));     //  <%IDS_TelMonitor_3%>
		else if(hSocket == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_PROXY",
            "PROXY_CONNECT_FAILED"));     //  <%IDS_TelMonitor_4%>
		else if(hSocket == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_PROXY",
            "PROXY_SERVER_ERROR"));        //  <%IDS_TelMonitor_5%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_UNKNOWN_ERROR"));     //  <%IDS_TelMonitor_6%>

		bResult = FALSE;
		goto err;
	}


	if((ret = telnet_connect(hSocket, uid, pwd, logprompt, pwdprompt, arprompt,pgd)) < 0)
	{
		if(ret == -1)
		{
			if(nTime<3)
			{
				nTime++;
				Sleep(3000);
				goto LOOP;
			}else
			{

			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
                "SOCKET_RECV_DATA_TIMEOUT"));    //  <%IDS_TelMonitor_7%>
			}
		}
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
                "SOCKET_RECV_DATA_ERROR"));     //  <%IDS_TelMonitor_8%>
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
                "UNIX_SERVER_SYSTEM_PROMPT_NOT_FOUND"));     //  <%IDS_TelMonitor_9%>
		else if(ret == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
                "UNIX_SERVER_LOGIN_PROMPT_NOT_FOUND"));   //  <%IDS_TelMonitor_10%>
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
                "UNIX_SERVER_PWD_PROMPT_NOT_FOUND"));   //  <%IDS_TelMonitor_11%>
		else if(ret == -6)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
                "SOCKET_RECV_DATA_TIMEOUT"));   //  <%IDS_TelMonitor_12%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
                "UNIX_SERVER_USER_PWD_ERROR"));     //  <%IDS_TelMonitor_13%>

		bResult = FALSE;
		goto err;

	}



	GetIniFileName(inifile, iniFile);

/*	if(pf)		
		fputs("zzzz",pf);*/
//	fclose(pf);
/*	strMutex=iniFile;
	strTemp=strMutex.Right(strMutex.GetLength()-strMutex.ReverseFind('\\')-1);

//	strMutex.Format("Global\\%s",strTemp.Left(strTemp.GetLength()-4));
	strMutex.Format("sitviewdragon-%s",strTemp.Left(strTemp.GetLength()-4));
	puts(strMutex);

	hMutex=::CreateMutex(NULL,FALSE,strMutex);
//	hMutex=::CreateMutex(NULL,FALSE,"siteview-kkkkkkk");
	if(hMutex==NULL)
	{
		
		sprintf(szReturn,"error=¥¥Ω®ª•≥‚≥ˆ¥Ì!ew=%d",GetLastError());
		bResult=FALSE;
		goto err;
	}
	if(::WaitForSingleObject(hMutex,100000)==WAIT_TIMEOUT)
	{
		sprintf(szReturn,"error=≤¢∑¢≥¨ ±$");
		puts("≤¢∑¢≥¨ ±");
		bResult=FALSE;
		goto err;
	}*/

	try
    {
		if(!RETURN_COMMAND(mtype, iniFile, cmd))
		{
			printf("type=%d", mtype);
            printf("inifile=%s", inifile);
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
                "BASIC_READ_CMDFILE_FAILD"));    //  <%IDS_TelMonitor_14%>
            printf(szReturn);

			bResult = FALSE;
			goto err;
		}
	}catch(...)
	{
//		::ReleaseMutex(hMutex);
		bResult=FALSE;
		goto err;
	}

//	::ReleaseMutex(hMutex);



	if(ca = strchr(cmd, '<')) 
	{
		strcpy(ca, extraparam);
		*(ca + strlen(extraparam)) = 0;
	}

	memset(pgd,0,sizeof(GDATA));


	if((ret = telnet_command(hSocket, cmd,pgd)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
                "SOCKET_READ_DATA_ERROR"));    //  <%IDS_TelMonitor_15%>
		bResult = FALSE;
		goto err;
	}

//	printf("telnet return:%s\n",pgd->databuffer);

//	sprintf(szReturn,"error=%s",pgd->databuffer);

//		FILE *pf=fopen("loglog.txt","a+");
//	fputs("zzzz",pf);

/*	if(::WaitForSingleObject(hMutex,100000)==WAIT_TIMEOUT)
	{
		sprintf(szReturn,"error=≤¢∑¢≥¨ ±$");
		puts("≤¢∑¢≥¨ ±");
		bResult=FALSE;
		goto err;
	}*/

	try{
		
		if(!MONITOR_PARSE_zw(mtype, pgd->databuffer, inifile, extraparam, szTotalMem, szReturn))
		{
//			printf("parse error\n");
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
                "BASIC_PARSER_STRING_FAILED"));   //  <%IDS_TelMonitor_16%>
			bResult = FALSE;
			goto err;
		}
	}catch(...)
	{
//		::ReleaseMutex(hMutex);
		bResult=FALSE;
		goto err;
	}
//	::ReleaseMutex(hMutex);

//	sprintf(szReturn,"error=test$");

err:
/*	if(pf)		
		fclose(pf);*/
//	::CloseHandle(hMutex);

	delete pgd;
	shutdown(hSocket, 0x02);
	closesocket(hSocket);
	return bResult;
}

BOOL UnixPerfMonitorValue(char *server, int port, char *uid, char *pwd, 
		char *proxy_server, int proxy_port, char *proxy_uid, char *proxy_pwd, 
		char *logprompt, char *pwdprompt, char *prompts, char *inifile, 
		char *custpath, char *szReturn)
{
	int		ret = 0;
	int		hSocket = 0;
	BOOL	bResult = TRUE;
	char	iniFile[255] = {0};
	char	AllDataBuffer[20 * 1024] = {0};
	char	cmd[1024] = {0};

	/*************************************
	 * For Reason Of Prompt
	 */
	int		c = 0;
	char	*ca = prompts, *cb = NULL;
	char	arprompt[PR_NUM][256];
	memset(arprompt, 0, sizeof(arprompt));

	LPGDATA pgd=new GDATA;
	memset(pgd,0,sizeof(GDATA));


	while(cb = strchr(ca, 0x20))
	{
		if(ca == cb) 
		{
			ca ++;
			continue;
		}

		strncpy(arprompt[c++], ca, cb - ca);
		ca = cb + 1;
	}

	strcpy(arprompt[c++], ca);
	/***********************************/

	hSocket = telnet_init(server, port, proxy_server, proxy_port, proxy_uid, proxy_pwd);
	if(hSocket <= 0)
	{
		if(hSocket == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_1%>"));    //  <%IDS_TelMonitor_1%>
		else if(hSocket == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_2%>"));   //  <%IDS_TelMonitor_2%>
		else if(hSocket == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_3%>"));     //  <%IDS_TelMonitor_3%>
		else if(hSocket == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_4%>"));     //  <%IDS_TelMonitor_4%>
		else if(hSocket == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_5%>"));        //  <%IDS_TelMonitor_5%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_6%>"));     //  <%IDS_TelMonitor_6%>

		bResult = FALSE;
		goto err;
	}

	if((ret = telnet_connect(hSocket, uid, pwd, logprompt, pwdprompt, arprompt,pgd)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_7%>"));    //  <%IDS_TelMonitor_7%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_8%>"));     //  <%IDS_TelMonitor_8%>
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_9%>"));     //  <%IDS_TelMonitor_9%>
		else if(ret == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_10%>"));   //  <%IDS_TelMonitor_10%>
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_11%>"));   //  <%IDS_TelMonitor_11%>
		else if(ret == -6)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_12%>"));   //  <%IDS_TelMonitor_12%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_13%>"));     //  <%IDS_TelMonitor_13%>

		bResult = FALSE;
		goto err;

	}

	GetIniFileName(inifile, iniFile);
	
	memset(AllDataBuffer, 0, sizeof(AllDataBuffer));
	
	// CPU Data
	strncpy(AllDataBuffer, "SiteViewCPU_Start\r\n", 19);

	if(RETURN_COMMAND(CPU_TYPE_MONITOR, iniFile, cmd))
	{
		if((ret = telnet_command(hSocket, cmd,pgd)) >= 0)
		{
			strncat(AllDataBuffer, pgd->databuffer, pgd->datalen);
		}
	}

	strncat(AllDataBuffer, "SiteViewCPU_End\r\n", 17);

	// Memory Data
	strncat(AllDataBuffer, "SiteViewMemory_Start\r\n", 22);

	if(RETURN_COMMAND(MEMORY_TYPE_MONITOR, iniFile, cmd))
	{
		if((ret = telnet_command(hSocket, cmd,pgd)) >= 0)
		{
			strncat(AllDataBuffer, pgd->databuffer, pgd->datalen);
		}
	}

	strncat(AllDataBuffer, "SiteViewMemory_End\r\n", 20);

	// Disk Data
	strncat(AllDataBuffer, "SiteViewDisk_Start\r\n", 20);

	if(RETURN_COMMAND(DISKS_TYPE_MONITOR, iniFile, cmd))
	{
		if((ret = telnet_command(hSocket, cmd,pgd)) >= 0)
		{
			strncat(AllDataBuffer, pgd->databuffer, pgd->datalen);
		}
	}

	strncat(AllDataBuffer, "SiteViewDisk_End\r\n", 18);

	MONITOR_PARSE(0, AllDataBuffer, inifile, NULL, szReturn);

err:
	delete pgd;
	shutdown(hSocket, 0x02);
	closesocket(hSocket);
	return bResult;	
}

SOCKET telnet_init(char *server, int port, char *proxy_server, int proxy_port, char *proxy_uid, char *proxy_pwd)
{
	unsigned long	ip;
	hostent			*ent;
	sockaddr_in		sa;
	SOCKET			hSocket;
	
	if(*proxy_server)
	{	
		ent = gethostbyname(proxy_server);
		if(ent == NULL)
			return -4;
		
		memcpy(&sa.sin_addr,ent->h_addr_list[0],ent->h_length);
		sa.sin_family = AF_INET;
		sa.sin_port = htons(proxy_port);
	
		hSocket = socket(AF_INET,SOCK_STREAM,0);
		if (hSocket == INVALID_SOCKET) 
			return -2;

		if(connect_timeo(hSocket, (sockaddr*)&sa, sizeof(sockaddr), CONNECT_TIMEOUT) < 0)
		//if(connect(hSocket, (struct sockaddr*)&sa, sizeof(sa)) < 0)
			return -4;

		unsigned char request[22];
		int request_len = 0;
		unsigned char response[23];
		int response_len = 0;
    
		request[0] = 0x05; // SOCKS version 5
		request[1] = 0x03; // number of auth procotols we recognize
		// auth protocols
		request[2] = 0x00; // no authentication required
		// compliant implementations MUST implement GSSAPI
		// and SHOULD implement username/password
		// TODO: we don't
		request[3] = 0x01; // GSSAPI
		request[4] = 0x02; // username/password
    
		request_len = 2 + request[1];
		int write_len = send(hSocket, (const char *)request, request_len, 0);
		if (write_len != request_len) 
			return -5;
    
		// get the server's response
		response_len = 22;
		response_len = recv(hSocket, (char *)response, response_len, 0);
    
		if (response_len <= 0) 
			return -5;
    
		if (response[0] != 0x05) 
			// it's a either not SOCKS or not our version
			return -5;

		switch (response[1]) 
		{
			case 0x00:
				// no auth
				break;
			case 0x01:
				// GSSAPI
				// TODO: implement
				return -5;
			case 0x02:
				{
				// username/password
				// TODO: implement
				int i = 0;
				request[0] = 0x01;
				if(*proxy_uid)
				{
					request[1] = strlen(proxy_uid);
					for(i = 0;i < (int)strlen(proxy_uid);i ++)
						request[2+i] = proxy_uid[i];
				}
				else
				{
					request[1] = 0x01;
					request[2] = 0x00;
				}
				request_len = 2 + i;

				if(*proxy_pwd)
				{
					request[request_len++] = strlen(proxy_pwd);
					for(i = 0;i < (int)strlen(proxy_pwd);i ++)
						request[request_len+i] = proxy_pwd[i];
					request_len += strlen(proxy_pwd);
				}
				else
				{
					request[request_len++] = 0x01;
					request[request_len++] = 0x00;
				}

				write_len = send(hSocket, (const char *)request, request_len, 0);
				response_len = 22;
				response_len = recv(hSocket, (char *)response, response_len, 0);
				if(response[0] != 1)
					return -5;
				if(response[1] != 0)
					return -5;
				}
				break;
			default:
				// unrecognized auth method
				return -5;
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
			ent = gethostbyname(server);
			if (ent == NULL)
				return -1;

			strcpy(ipaddr, inet_ntoa(*((struct in_addr *)ent->h_addr)));
		}
		else
		{
			strcpy(ipaddr, server);
		}
		
		if(sscanf(ipaddr, "%d.%d.%d.%d", &request[4], &request[5], &request[6], &request[7]) != 4)
			return -3;

		// destination port
		unsigned int destPort = htons(port);
    
		request[request_len-1] = (unsigned char)(destPort >> 8);
		request[request_len-2] = (unsigned char)destPort;
    
		if (send(hSocket, (const char *)request, request_len, 0) != request_len) {
			// bad write
			return -5;
		}
    
		response_len = 22;
		response_len = recv(hSocket, (char *)response, response_len, 0);
		if (response_len <= 0) {
			// bad read
			return -5;
		}
    
		if (response[0] != 0x05) {
			// bad response
			return -5;
		}
    
		if (response[1] != 0x00) {
			// SOCKS server failed to connect
			return -5;
		}
	}
	else
	{
		ent = gethostbyname(server);
		if(ent == NULL) 
			return -1;
		
		ip = *(unsigned long*)(ent->h_addr);

		sa.sin_family = AF_INET;
		sa.sin_port = htons(port);
		sa.sin_addr = *(in_addr*)&ip;

		hSocket = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
		if (hSocket == INVALID_SOCKET) 
			return -2;

		if(connect_timeo(hSocket, (sockaddr*)&sa, sizeof(sockaddr), CONNECT_TIMEOUT) < 0)
		//if(connect(hSocket, (sockaddr *)&sa, sizeof(sockaddr)) < 0)
			return -3;
	}
	
	return hSocket;
}

int telnet_connect(SOCKET hSocket, char *uid, char *pwd, 
				   char *logprompt, char *pwdprompt, char arprompt[PR_NUM][256],
                   LPGDATA pgd)
{
	OutputDebugString("=============== telnet_connect:Login ================\n");

	int		n = 0;
	char	buffer[4096] = {0};
	char	*scan = NULL;
	int		telnetprogress = 0;
	int		count = 0;
	memset(pgd->databuffer, 0, BUF_SIZE);
	pgd->datalen = 0;
	int nTime=0;

	

	while(1)
	{
LOOP:
		memset(buffer, 0, 4096);
		if(RESPONSE_WAIT(hSocket, READ_TIMEOUT) <= 0)
		{
			if((telnetprogress==2)&&(nTime<3))
			{
				char buf[10]={0};
				sprintf(buf,"\r\n");
				send(hSocket, buf, strlen(buf),0);
				nTime++;
				goto LOOP;
			}

			return -1;
		} 
		Sleep(500);
		n = recv(hSocket, buffer, 4095, 0);
		
		//OutputDebugString("-----telnet_connect second receive----------\n");
		//OutputDebugString(buffer);
		//OutputDebugString("\n");
		//WriteLog( "Ω” ‹µΩ ˝æ›£∫" );
		//WriteLog( buffer );

		if(n == SOCKET_ERROR)
		{
			int wserror;
			wserror=WSAGetLastError();
			CString stmp;
			stmp.Format("telnet connect failed --error code =%d",wserror);
			WriteLogFile1(stmp);
			return -2;
		}
		buffer[n] = 0;
		scan = buffer;
		while(n--)telnet_protocol(hSocket, *scan++,pgd);
		switch(telnetprogress)
		{
		case 0:
            if(logprompt)
            {
			    if(strstr(pgd->databuffer, logprompt))
			    {
				    char	bufUser[255] = {0};
				    strcpy(bufUser, uid);
					for(int nS=0;nS<strlen(bufUser);nS++)
					{
						Sleep(20);
						send(hSocket, &bufUser[nS], 1,0);	
					}
					 send(hSocket,"\r\n",2,0);

//				    strcat(bufUser, "\r\n");
					
//				    Sleep(100);			// delay
//				    send(hSocket, bufUser, strlen(bufUser),0);


				    telnetprogress ++;
				    count = 0;
				    memset(pgd->databuffer, 0, BUF_SIZE);
				    pgd->datalen = 0;
			    }
            }
            else
            {
                WriteLogFile("Read Data Failed from Server(1)");
                return -7;
            }
			break;
		case 1:
            if(pwdprompt)
            {
			    if(strstr(pgd->databuffer, pwdprompt))
			    {
				    char	bufPwd[255] = {0};
				    strcpy(bufPwd, pwd);
				    strcat(bufPwd, "\r\n");
				    Sleep(100);			// delay
				    send(hSocket, bufPwd, strlen(bufPwd), 0);
				    telnetprogress ++;
				    count = 0;
				    memset(pgd->databuffer, 0, BUF_SIZE);
				    pgd->datalen = 0;
			    }
            }
            else
            {
                WriteLogFile("Read Data Failed from Server(2)");
                return -7;
            }
			break;
		case 2:
			if(strstr(pgd->databuffer, "ogin incorrect"))
			{
				memset(pgd->databuffer, 0, BUF_SIZE);
				pgd->datalen = 0;
				return -6;
			}

			int		i = 0;
			char	prompt[256];
//			telnet_setlinewidth(hSocket, 0x00, 0xef);
			while(i < PR_NUM && *arprompt[i])
			{
				memset(prompt, 0, sizeof(prompt));
				strcpy(prompt, arprompt[i]);
				if(strstr(pgd->databuffer, prompt))
				{
					memset(pgd->databuffer, 0, BUF_SIZE);
					pgd->datalen = 0;
					return 0;
				}
				i ++;
			}
			
			break;
		}
		if(++count > 80)
		{
			if(telnetprogress == 0)
				return -3;
			if(telnetprogress == 1) 
				return -4;
			if(telnetprogress == 2) 
				return -5;
		}
        //∏ƒ±‰ƒ¨»œµƒtelnet––µƒ≥§∂»
//        telnet_setlinewidth(hSocket, 0x00, 0xef);
	}
}

int telnet_setlinewidth(SOCKET hSocket, BYTE HighByte, BYTE LowByte)
{
	
    BYTE SB_NAWS[9];
	/*
    SB_NAWS[0] = 0xff;
    SB_NAWS[1] = 0xfa;
    SB_NAWS[2] = 0x1f;
    SB_NAWS[3] = HighByte;
    SB_NAWS[4] = LowByte;
    SB_NAWS[5] = 0x00;
    SB_NAWS[6] = 0xef;
    SB_NAWS[7] = 0xff;
    SB_NAWS[8] = 0xf0;
	*/
	SB_NAWS[0] = 0xff;
    SB_NAWS[1] = 0xfb;
    SB_NAWS[2] = 0x1f;

	send(hSocket, (char*)SB_NAWS, 3, 0);
	Sleep(500);

	SB_NAWS[0] = 0xff;
    SB_NAWS[1] = 0xfa;
    SB_NAWS[2] = 0x1f;
    SB_NAWS[3] = 0x00;
    SB_NAWS[4] = 0xA0;
    SB_NAWS[5] = 0x00;
    SB_NAWS[6] = 0xa0;
    SB_NAWS[7] = 0xff;
    SB_NAWS[8] = 0xf0;
    send(hSocket, (char*)SB_NAWS, 9, 0);
    return 1;
}

extern "C" __declspec(dllexport)
int telnet_connect(SOCKET hSocket, char *uid, char *pwd, 
				   char *logprompt, char *pwdprompt, char arprompt[PR_NUM][256],
                   LPGDATA pgd, struct Tel_Param *pParam)
{
	OutputDebugString("=============== telnet_connect:Login ================\n");
	//OutputDebugString("------telnet_connect() call start---------\n");
	puts("=================================================================================");

	WriteLog("=============== telnet_connect:Login ================\n");
	
	int		n = 0;
	char	buffer[4096] = {0};
	char	*scan = NULL;
	int		telnetprogress = 0;
	int		count = 0;
	memset(pgd->databuffer, 0, BUF_SIZE);
	pgd->datalen = 0;
    pParam->state = state_data;
	pParam->verb= verb_sb;
	pParam->DataProc = terminal[(pgd->term_index==NUM_TERMINALS)?(NUM_TERMINALS-1):pgd->term_index].termproc;
	int nTime=0;
	while(1)
	{
LOOP:
		memset(buffer, 0, 4096);
		if(RESPONSE_WAIT(hSocket, READ_TIMEOUT) <= 0)
		{
			if((telnetprogress==2)&&(nTime<3))
			{
				char buf[10]={0};
				sprintf(buf,"\r\n");
				send(hSocket, buf, strlen(buf),0);
				nTime++;
				goto LOOP;
			}

			return -1;
		} 
		//Sleep(500);
		n = recv(hSocket, buffer, 4095, 0);
		
		OutputDebugString("----------telnet receive buffer----------\n");
		OutputDebugString(buffer);
		OutputDebugString("\n");

		WriteLog( "Ω” ‹µΩ ˝æ›£∫" );
		WriteLog( buffer );

		if(n == SOCKET_ERROR)
		{
			int wserror;
			wserror=WSAGetLastError();
			CString stmp;
			stmp.Format("telnet connect failed --error code =%d",wserror);
			WriteLogFile1(stmp);
			return -2;
		}

		char szTemp[128] = {0};
			
		buffer[n] = 0;
		scan = buffer;
		while(n--)
		{
			sprintf( szTemp, "data=%c, state=%d", *scan, pParam->state );
			WriteLog( szTemp );

			telnet_protocol(hSocket, *scan++,pgd, pParam);
		}

		WriteLog( "Ω‚Œˆ∫Ûµƒ ˝æ›£∫" );
		WriteLog( pgd->databuffer );

		WriteLog( "∆•≈‰≤Œ ˝£∫" );
		WriteLog( logprompt );
		WriteLog( pwdprompt );



		switch(telnetprogress)
		{
		case 0:
            if(logprompt)
            {
			    if(strstr(pgd->databuffer, logprompt))
			    {
				    
					char	bufUser[255] = {0};
				    strcpy(bufUser, uid);
					//---wangpeng
					for(int nS=0;nS<strlen(bufUser);nS++)
					{
						Sleep(20);
						send(hSocket, &bufUser[nS], 1,0);	
					}
					 send(hSocket,"\r\n",2,0);

//				    strcat(bufUser, "\r\n");
//				    Sleep(100);			// delay
//				    send(hSocket, bufUser, strlen(bufUser),0);
				    telnetprogress ++;
				    count = 0;
				    memset(pgd->databuffer, 0, BUF_SIZE);
				    pgd->datalen = 0;

					WriteLog( "”√ªß√˚∆•≈‰≥…π¶£°" );
			    }
            }
            else
            {
                WriteLogFile("Read Data Failed from Server(1)");
                return -7;
            }
			break;
		case 1:
            if(pwdprompt)
            {
			    if(strstr(pgd->databuffer, pwdprompt))
			    {
				    char	bufPwd[255] = {0};
				    strcpy(bufPwd, pwd);

					//---wangpeng
					for(int nS=0;nS<strlen(bufPwd);nS++)
					{
						Sleep(20);
						send(hSocket, &bufPwd[nS], 1,0);	
					}
					 send(hSocket,"\r\n",2,0);
					//---------Ω· ¯------------------
//				    strcat(bufPwd, "\r\n");
//				    Sleep(100);			// delay
//				    send(hSocket, bufPwd, strlen(bufPwd), 0);
				    telnetprogress ++;
				    count = 0;
				    memset(pgd->databuffer, 0, BUF_SIZE);
				    pgd->datalen = 0;

					WriteLog( "√‹¬Î∆•≈‰≥…π¶£°" );
			    }
            }
            else
            {
                WriteLogFile("Read Data Failed from Server(2)");
                return -7;
            }
			break;
		case 2:
			if(strstr(pgd->databuffer, "ogin incorrect"))
			{
				memset(pgd->databuffer, 0, BUF_SIZE);
				pgd->datalen = 0;
				return -6;
			}
			telnet_setlinewidth(hSocket, 0x00, 0xef);
			

			WriteLog( " ’µΩÃ· æ∑˚£∫" );
			WriteLog( pgd->databuffer );


			int		i = 0;
			char	prompt[256];
			while(i < PR_NUM && *arprompt[i])
			{
				memset(prompt, 0, sizeof(prompt));
				strcpy(prompt, arprompt[i]);

				WriteLog( "∆•≈‰Ã· æ∑˚£∫" );
				WriteLog( prompt );

				if(strstr(pgd->databuffer, prompt))
				{
//					puts(pgd->databuffer);
					memset(pgd->databuffer, 0, BUF_SIZE);
					pgd->datalen = 0;
					
					return 0;
				}
				i ++;
			}
			if(strstr(pgd->databuffer, "TERM = (ansi)"))
			{
				puts("TERM = (ansi)");
				char	TERM[255] = {0};
				strcpy(TERM, "\r\n");
				Sleep(100);			// delay
				send(hSocket, TERM, 2, 0);
				puts("TERM = (ansi)");
				memset(pgd->databuffer, 0, BUF_SIZE);
				pgd->datalen = 0;
			
			}
			break;
		}
		if(++count > 80)
		{
			if(telnetprogress == 0)
				return -3;
			if(telnetprogress == 1) 
				return -4;
			if(telnetprogress == 2) 
				return -5;
		}
        //∏ƒ±‰ƒ¨»œµƒtelnet––µƒ≥§∂»
//        telnet_setlinewidth(hSocket, 0x00, 0xef);
	}
}

int telnet_command(SOCKET hSocket, char *cmd,LPGDATA pgd, 
                     struct Tel_Param *pParam)
{
	//OutputDebugString("=============== telnet_command:GetData ==============\n");

	WriteLog("\n\n============== telnet_command:GetData =================");


	int		n = 0;
	char	buffer[102400] = {0};
	char	*scan = NULL;
	char	*ca = NULL, *cb = NULL;

	sprintf(buffer, "echo %s;%s;echo %s\r\n", C_STA, cmd, C_END);

	//OutputDebugString(buffer); //zjw

	WriteLog("cmd=");
	WriteLog(buffer);


    if(strlen(buffer) >= 102400)
    {
        char chMsg[256];
        sprintf(chMsg, "Command length is:%d", strlen(buffer));
        WriteLogFile(chMsg);
    }
	send(hSocket, buffer, strlen(buffer), 0);

	memset(pgd->databuffer, 0, BUF_SIZE);
	pgd->datalen = 0;
	while(RESPONSE_WAIT(hSocket, READ_TIMEOUT))
	{
        memset(buffer, 0 , 102400);
		n = recv(hSocket, buffer, 102400, 0);
		if(n == SOCKET_ERROR) 
		{
			WriteLog("SOCKET_ERROR");
			return -2;
		}
		if(n == 0) 
		{
			WriteLog("n=0");
			return -2;
		}
		buffer[n] = 0;
		scan = buffer;

		WriteLog("----------------------------");
		WriteLog("buffer:");
		WriteLog(buffer);
		WriteLog("----------------------------");

		while(n--) telnet_protocol(hSocket, *scan++,pgd, pParam);

		WriteLog("----------------------------");
		WriteLog("bufferParser:");
		WriteLog(pgd->databuffer);
		WriteLog("----------------------------");

		char match_1[20] = {0}, match_2[20] = {0};
		sprintf(match_1, "\r\n%s", C_STA);
		sprintf(match_2, "\r\n%s", C_END);
		if(ca = strstr(pgd->databuffer, match_1)) 
		{
			if(cb = strstr(pgd->databuffer, match_2))
			{
				ca += strlen(C_STA) + 4;
				cb += 2;
				strncpy(pgd->databuffer, ca, cb - ca);
				pgd->databuffer[cb - ca] = 0;
				break;
			}
		}
	}
	
	WriteLog("data=");
	WriteLog(pgd->databuffer);

	//OutputDebugString("data=");
	//OutputDebugString(pgd->databuffer);
	//OutputDebugString("\n");

	
	
//	puts(pgd->databuffer);
	return strlen(pgd->databuffer);
}

void telnet_protocol(SOCKET server,unsigned char code,LPGDATA pgd, 
                     struct Tel_Param *pParam)
{
	//Decide what to do (state based)
	switch(pParam->state)
	{
	case state_data:
		switch(code)
		{
		case IAC: pParam->state = state_code; break;
		default:
            pParam->DataProc(server,code,pgd);          
		}
		break;
	case state_code:
		pParam->state = state_data;
		switch(code)
		{
		// State transition back to data
		case IAC: 
			pParam->DataProc(server,code,pgd);
			break;
		// Code state transitions back to data
		case SE:
			pParam->DataProc = terminal[(pgd->term_index==NUM_TERMINALS)?(NUM_TERMINALS-1):pgd->term_index].termproc;
			break;
		case NOP:
			break;
		case DM:
			break;
		case BRK:
			break;
		case IP:
			break;
		case AO:
			break;
		case AYT:
			break;
		case EC:
			break;
		case EL:
			break;
		case GA:
			break;
		// Transitions to option state
		case SB:
			pParam->verb = verb_sb;
			pParam->state = state_option;
			break;
		case WILL:
			pParam->verb = verb_will;
			pParam->state = state_option;
			break;
		case WONT:
			pParam->verb = verb_wont;
			pParam->state = state_option;
			break;
		case DO:
			pParam->verb = verb_do;
			pParam->state = state_option;
			break;
		case DONT:
			pParam->verb = verb_dont;
			pParam->state = state_option;
		    break;
		}
		break;
	case state_option:
		pParam->state = state_data;

		//Find the option entry
		for(
			int i = 0;
			ol[i].option != TOPT_ERROR && ol[i].option != code;
			i++);

		//Do some verb specific stuff
		if(pParam->verb == verb_sb)
			pParam->DataProc = ol[i].DataProc;
		else
			ol[i].OptionProc(server, pParam->verb, (_option)code);
		break;
	}
}

void telnet_protocol(SOCKET server,unsigned char code,LPGDATA pgd)
{
	//These vars are the finite state
	static int state = state_data;
	static _verb verb = verb_sb;
	static LPDATAPROC DataProc = terminal[(pgd->term_index==NUM_TERMINALS)?(NUM_TERMINALS-1):pgd->term_index].termproc;

    int nType = 0;
	//Decide what to do (state based)
	switch(state)
	{
	case state_data:
		switch(code)
		{
		case IAC: state = state_code; break;
		default:
            DataProc(server,code,pgd);        
		}
		break;
	case state_code:
		state = state_data;
		switch(code)
		{
		// State transition back to data
		case IAC: 
			DataProc(server,code,pgd);
			break;
		// Code state transitions back to data
		case SE:
			DataProc = terminal[(pgd->term_index==NUM_TERMINALS)?(NUM_TERMINALS-1):pgd->term_index].termproc;
			break;
		case NOP:
			break;
		case DM:
			break;
		case BRK:
			break;
		case IP:
			break;
		case AO:
			break;
		case AYT:
			break;
		case EC:
			break;
		case EL:
			break;
		case GA:
			break;
		// Transitions to option state
		case SB:
			verb = verb_sb;
			state = state_option;
			break;
		case WILL:
			verb = verb_will;
			state = state_option;
			break;
		case WONT:
			verb = verb_wont;
			state = state_option;
			break;
		case DO:
			verb = verb_do;
			state = state_option;
			break;
		case DONT:
			verb = verb_dont;
			state = state_option;
		break;
		}
		break;
	case state_option:
		state = state_data;

		//Find the option entry
		for(
			int i = 0;
			ol[i].option != TOPT_ERROR && ol[i].option != code;
			i++);

		//Do some verb specific stuff
		if(verb == verb_sb)
			DataProc = ol[i].DataProc;
		else
			ol[i].OptionProc(server,verb,(_option)code);
		break;
	}
}

int telnet_command(SOCKET hSocket, char *cmd,LPGDATA pgd)
{

	OutputDebugString("=============== telnet_command:end ==============\n");

	WriteLog( "=============== telnet_command:end ==============" );

	int		n = 0;
	char	buffer[4096] = {0};
	char	*scan = NULL;
	char	*ca = NULL, *cb = NULL;

	sprintf(buffer, "echo %s;%s;echo %s\r\n", C_STA, cmd, C_END);
	OutputDebugString("cmd=");
	OutputDebugString(buffer);
	OutputDebugString("\n");

	WriteLog( "cmd=" );
	WriteLog( buffer );


    if(strlen(buffer) >= 4096)
    {
        char chMsg[256];
        sprintf(chMsg, "Command length is:%d", strlen(buffer));
        WriteLogFile(chMsg);
    }
	send(hSocket, buffer, strlen(buffer), 0);

	memset(pgd->databuffer, 0, BUF_SIZE);
	pgd->datalen = 0;
	while(RESPONSE_WAIT(hSocket, READ_TIMEOUT))
	{
        memset(buffer, 0 , 4096);
		n = recv(hSocket, buffer, 4095, 0);
		if(n == SOCKET_ERROR) return -2;
		if(n == 0) return -2;
		buffer[n] = 0;
		scan = buffer;

		while(n--) telnet_protocol(hSocket, *scan++,pgd);

		char match_1[20] = {0}, match_2[20] = {0};
		sprintf(match_1, "\r\n%s", C_STA);
		sprintf(match_2, "%s", C_END);
		if(ca = strstr(pgd->databuffer, match_1)) 
		{
			if(cb = strstr(ca, match_2))
			{
				ca += strlen(C_STA) + 4;
//				puts(ca);
				cb += 2;
//				int a=ca-cb;
//				printf("===========%d\n",a);
				strncpy(pgd->databuffer, ca, cb - ca);
				pgd->databuffer[cb - ca] = 0;
				break;
			}
		}
	}

	OutputDebugString("data=");
	OutputDebugString(pgd->databuffer);
	OutputDebugString("\n");

	WriteLog( "data=" );
	WriteLog( pgd->databuffer );

	return strlen(pgd->databuffer);
}

void noreply(SOCKET server, _verb verb, _option option)
{
	unsigned char buf[3];
	buf[0] = IAC;
	buf[1] = (verb==verb_do)?WONT:(verb==verb_dont)?WILL:(verb==verb_will)?DONT:DO;
	buf[2] = (unsigned char)option;
	send(server, (char*)buf, 3, 0);
}

void yesreply(SOCKET server, _verb verb, _option option)
{
	unsigned char buf[3];
	buf[0] = IAC;
	buf[1] = (verb==verb_do)?WILL:(verb==verb_dont)?WONT:(verb==verb_will)?DO:DONT;
	buf[2] = (unsigned char)option;
	send(server, (char*)buf, 3, 0);
}

void ddww_echo(SOCKET server, _verb verb, _option option)
{
	switch(verb)
	{
	case verb_will: // server wants to echo stuff
		break;
	case verb_wont: // server don't want to echo
		break;
	case verb_do:   // server wants me to loopback
		noreply(server,verb,option);
		return;
	case verb_dont: // server doesn't want me to echo
		break;        // don't bother to reply - I don't
	}

	yesreply(server,verb,option);
}

void ddww_supp(SOCKET server, _verb verb, _option option)
{
	switch(verb)
	{
	case verb_will: // server wants to suppress GA's
		break;
	case verb_wont: // server wants to send GA's 
		break;
	case verb_do:   // server wants me to suppress GA's
		break;
	case verb_dont: // server wants me to send GA's
		break;
	}
}

void ddww_term(SOCKET server, _verb verb, _option option)
{
	switch(verb)
	{
	case verb_will:
		noreply(server,verb,option); // I don't want terminal info
		break;
	case verb_wont:
		//dat be cool - its not going to send. no need to confirm
		break;
	case verb_do:
		yesreply(server,verb,option); //I'll send it when asked
		break;
	case verb_dont://Ok - I won't
		break;
	}
}

void sbproc_term(SOCKET server, unsigned char data,LPGDATA pgd)
{
	if(data == SB_TERM_SEND)
	{
		if(pgd->term_index == NUM_TERMINALS)
			pgd->term_index = 0;
		else
			pgd->term_index++;
		char buf[16]; //pls limit 
		buf[0] = (char)IAC;
		buf[1] = (char)SB;
		buf[2] = (char)TOPT_TERM;
		buf[3] = (char)SB_TERM_IS;
		lstrcpy(&buf[4],terminal[(pgd->term_index==NUM_TERMINALS)?(NUM_TERMINALS-1):pgd->term_index].name);
		int nlen = lstrlen(&buf[4]);
		buf[4+nlen] = (char)IAC;
		buf[5+nlen] = (char)SE;
		send(server,buf,4+nlen+2,0);
	}
}

void ddww_error(SOCKET server, _verb verb, _option option)
{
	switch(verb)
	{
	case verb_will: 
		noreply(server,verb,option);
		break;
	case verb_wont:
		return;
	case verb_do:
		noreply(server,verb,option);
		break;
	case verb_dont:
		return;
	}
}

void nvt(SOCKET server, unsigned char data,LPGDATA pgd)
{
	switch(data)
	{
	case 0:  //eat null codes.
		break;
	default: //Send all else to the console.
		//WriteConsole(stdout1,&data,1,&z,NULL);
		pgd->databuffer[pgd->datalen++] = data;
		break;
	}
}

void ansi(SOCKET server,unsigned char data,LPGDATA pgd)
{
	
	static _ansi_state state = as_normal;

	char szTemp[100] = {0};
	//sprintf( szTemp, "ansi: data=%c, state=%d", data, state );
	//WriteLog( szTemp );

	switch( state)
	{
	case as_normal:
		switch(data)
		{
		case 0:  //eat null codes.
			break;
		case 27: //ANSI esc.
			state = as_esc;
			break;
		default: //Send all else to the console.
			pgd->databuffer[pgd->datalen++] = data;
		break;
		}
		break;
	case as_esc:
		state = as_esc1;
		pgd->codeptr=0;
		pgd->codebuf[pgd->codeptr] = 0;
		break;
	case as_esc1:
		if( data > 34 )
		{
			pgd->codebuf[pgd->codeptr] = 0;
			for(int i=0; codec[i].cmd && codec[i].cmd != data; i++);
			if(codec[i].proc)
				codec[i].proc(pgd->codebuf);
#ifdef _DEBUG
			else
			{
				char buf[256];
				wsprintf(buf,"Unknown Ansi code:'%c' (%s)\n",data,pgd->codebuf);
				//OutputDebugString(buf);
			}
#endif
			state = as_normal;
		}
		else
			pgd->codebuf[pgd->codeptr++] = data;
		break;
	}
}

void ansi_set_screen_attribute(char* buffer)
{
/*
  while(*buffer)
  {
    switch(*buffer++)
    {
    case '0': //Normal
      sa = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
      break;
    case '1': //Hign Intensity
      sa |= FOREGROUND_INTENSITY;
      break;
    case '4': //Underscore
      break;
    case '5': //Blink.
      sa |= BACKGROUND_INTENSITY;
      break;
    case '7':
      sa = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
      break;
    case '8':
      sa = 0;
      break;
    case '3':
      sa = sa & (BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED | BACKGROUND_INTENSITY) |
        (*buffer & 1)?FOREGROUND_RED:0 |
        (*buffer & 2)?FOREGROUND_GREEN:0 |
        (*buffer & 4)?FOREGROUND_BLUE:0;
      if(*buffer)
        buffer++;
      break;
    case '6':
      sa = sa & (FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY) |
        (*buffer & 1)?BACKGROUND_RED:0 |
        (*buffer & 2)?BACKGROUND_GREEN:0 |
        (*buffer & 4)?BACKGROUND_BLUE:0;
      if(*buffer)
        buffer++;
      break;
    }
    if(*buffer && *buffer == ';')
      buffer++;
  }
  SetConsoleTextAttribute(stdout1,sa);
*/
}

void ansi_erase_line(char* buffer)
{
/*
  int act = 0;
  while(*buffer)
  {
    act = (*buffer++) - '0';
  }

  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(stdout1,&csbi);

  COORD pos;
  DWORD n;

  switch(act)
  {
  case 0: //erase to end of line
    pos.X = csbi.dwCursorPosition.X;
    pos.Y = csbi.dwCursorPosition.Y;
    n = csbi.dwSize.X - csbi.dwCursorPosition.X;
    break;
  case 1: //erase from beginning
    pos.X = 0;
    pos.Y = csbi.dwCursorPosition.Y;
    n = csbi.dwCursorPosition.X;
    break;
  case 2: // erase whole line
    pos.X = 0;
    pos.Y = csbi.dwCursorPosition.Y;
    n = csbi.dwSize.X;
    break;
  }

  DWORD w;
  FillConsoleOutputCharacter(stdout1,' ',n,pos,&w);
*/
}

void ansi_set_position(char* buffer)
{
/*
  COORD pos = {0,0};

  // Grab line
  while(*buffer && *buffer != ';')
    pos.Y = pos.Y*10 + *buffer++ - '0';

  if(*buffer)
    buffer++;

  // Grab y
  while(*buffer && *buffer != ';')
    pos.X = pos.X*10 + *buffer++ - '0';

  (pos.X)?pos.X--:0;
  (pos.Y)?pos.Y--:0;

  SetConsoleCursorPosition(stdout1,pos);
*/ 
}

void ansi_erase_screen(char* buffer)
{
/*
  int act = 0;
  while(*buffer)
  {
    act = (*buffer++) - '0';
  }

  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(stdout1,&csbi);

  COORD pos;
  DWORD n;

  switch(act)
  {
  case 0:
    pos.X = csbi.dwCursorPosition.X;
    pos.Y = csbi.dwCursorPosition.Y;
    n = csbi.dwSize.X*csbi.dwSize.Y;
    break;
  case 2:
    pos.X = 0;
    pos.Y = 0;
    n = csbi.dwSize.X*csbi.dwSize.Y;
    break;
  }

  DWORD w;
  FillConsoleOutputCharacter(stdout1,' ',n,pos,&w);
  SetConsoleCursorPosition(stdout1,pos);
*/
}

void ansi_move_up(char* buffer)
{
/*
  int cnt = *buffer?0:1;
  while(*buffer)
  {
    cnt = cnt*10 + (*buffer++) - '0';
  }

  COORD pos;

  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(stdout1,&csbi);

  pos.X = csbi.dwCursorPosition.X;
  pos.Y = ((csbi.dwCursorPosition.Y-cnt)>=0)?(csbi.dwCursorPosition.Y-cnt):0;

  SetConsoleCursorPosition(stdout1,pos);
*/
}

//------------------------nan.shen-----------------------//
int putintofile(char* str1,int in1 ,char * str2,int in2 ,char * str3,char *str4)
{	
	FILE* ffile;		
	ffile = fopen("D:\\testmonitor\\test.txt", "a+");
	
	if(ffile != NULL)
	{
		fprintf(ffile, "%s\n", str1);
		fprintf(ffile, "%d\n", in1);
		fprintf(ffile, "%s\n", str2);
		fprintf(ffile, "%d\n", in2);
		fprintf(ffile, "%s\n", str3);
		fprintf(ffile, "%s\n", str4);

		fclose(ffile);
	}	
	return 0;
}
//------------------------nan.shen-----------------------//
int putintofile1(char* str)
{	
	FILE* ffile1;		
	ffile1 = fopen("D:\\testmonitor\\test1.txt", "a+");
	
	if(ffile1 != NULL)
	{
		fprintf(ffile1, "%d\n", str);
		fclose(ffile1);
	}	
	return 0;
}
//------------------------nan.shen-----------------------//
bool MonitorValue(char *server, int port, char *uid, char *pwd, 
				  char *proxy_server, int proxy_port, char *proxy_uid, char *proxy_pwd, 
				  char *logprompt, char *pwdprompt, char *prompts, 
				  char *inifile, int mtype, char *extraparam, char *custpath, char *szReturn)
{
	WriteLog( "================= MonitorValue ================" );
	WriteLog( extraparam );

	int nTime=0;
	LPGDATA pgd=new GDATA;
	HANDLE hMutex=NULL;
	CString strMutex=_T(""),strTemp=_T("");

    Tel_Param param;/////////////////////////////

    if(pgd == NULL)
    {
        sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC", 
            "BASIC_MALLOC_MEMORY_FAILE"));//∑÷≈‰ƒ⁄¥Ê ß∞‹
        return false;
    }

LOOP:
	memset(pgd,0,sizeof(GDATA));
	int		ret = 0;
	int		hSocket = 0;
	bool	bResult = true;
	char	iniFile[255] = {0};
	char	cmd[255] = {0};
	/*************************************
	 * For Reason Of Prompt
	 */
	int		c = 0;
	char	*ca = prompts, *cb = NULL;
	char	arprompt[PR_NUM][256];
	memset(arprompt, 0, sizeof(arprompt));
	while(cb = strchr(ca, 0x20))
	{
		if(ca == cb) 
		{
			ca ++;
			continue;
		}
		
		strncpy(arprompt[c++], ca, cb - ca);
		ca = cb + 1;
	}   
	
	strcpy(arprompt[c++], ca);
	/***********************************/
    ///////////////////////////////////////////////////
    //Telnet init	
	hSocket = telnet_init(server, port, proxy_server, proxy_port, proxy_uid, proxy_pwd);
	
	if(hSocket <= 0)
	{
		if(hSocket == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_PARSER_SERVER_FAILED"));    //  <%IDS_TelMonitor_1%>
		else if(hSocket == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
            "SOCKET_COMMUNICATE_ERROR"));   //  <%IDS_TelMonitor_2%>
		else if(hSocket == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX",
            "UNIX_TIMEOUT_ERROR"));     //  <%IDS_TelMonitor_3%>
		else if(hSocket == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_PROXY",
            "PROXY_CONNECT_FAILED"));     //  <%IDS_TelMonitor_4%>
		else if(hSocket == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_PROXY",
            "PROXY_SERVER_ERROR"));        //  <%IDS_TelMonitor_5%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX_SERVER",
            "UNIX_SERVER_UNKNOWN_ERROR"));     //  <%IDS_TelMonitor_6%>
		bResult = false;
		goto err;
	}
    //////////////////////////////////////////////////////////
    //Connect Server by Telnet Type
	// real connect 

	ret = telnet_connect(hSocket, uid, pwd, logprompt, 
        pwdprompt, arprompt,pgd , &param);

	//if((ret = telnet_connect(hSocket, uid, pwd, logprompt, 
 //       pwdprompt, arprompt,pgd , &param)) < 0)
	if( ret < 0)
	{
		

		if(ret == -1)//If Recv data time out
		{
			

			if(nTime<3)
			{//If loop's times less than three
				nTime++;//loop's times add
				Sleep(3000);//Sleep three second
				goto LOOP;//Go to Loop label
			}
            else
			{//
			    sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
                    "SOCKET_RECV_DATA_TIMEOUT"));    //  <%IDS_TelMonitor_7%>
			}
		}
		else if(ret == -2)//If recv data error
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
                "SOCKET_RECV_DATA_ERROR"));     //  <%IDS_TelMonitor_8%>
		else if(ret == -3)//prompr not found
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
                "UNIX_SERVER_SYSTEM_PROMPT_NOT_FOUND"));     //  <%IDS_TelMonitor_9%>
		else if(ret == -4)//login prompt not found
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
                "UNIX_SERVER_LOGIN_PROMPT_NOT_FOUND"));   //  <%IDS_TelMonitor_10%>
		else if(ret == -5)//password prompt not found
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
                "UNIX_SERVER_PWD_PROMPT_NOT_FOUND"));   //  <%IDS_TelMonitor_11%>
		else if(ret == -6)//recv data time out
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
                //"SOCKET_RECV_DATA_TIMEOUT"));   //  <%IDS_TelMonitor_12%>
				"<%IDS_TelMonitor_12%>"));
        else if(ret == -7)
            sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
            "SOCKET_SEND_DATA_ERROR"));
		else//
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
                "UNIX_SERVER_USER_PWD_ERROR"));     //  <%IDS_TelMonitor_13%>
		bResult = false;
		goto err;

	}


    //////////////////////////////////////////////////
    //Make OS config file
	GetIniFileName(inifile, iniFile);
    //////////////////////////////////////////////////

	char szTemp[1024] = {0};
	sprintf( szTemp, "mtype=%d,iniFile=%s", mtype, iniFile );
	WriteLog( szTemp );


	//-----------------------------------------------------------------------------------------------------------
	// ∏¸∏ƒƒ⁄»›£∫’Î∂‘”Îaix”ÎsolarisµƒŒÔ¿Ìƒ⁄¥Ê–≈œ¢≤…»°¡Ω≤ΩªÒ»°£®prtconf£∫◊‹ŒÔ¿Ìƒ⁄¥Ê£¨vmstat£∫ £”‡ŒÔ¿Ìƒ⁄¥Ê£©
	// ∏¸∏ƒ»À£∫◊ﬁœ˛
	// ∏¸∏ƒ ±º‰£∫2009.02.25
	//-----------------------------------------------------------------------------------------------------------
	if( ( mtype == PMEMORY_TYPE_MONITOR ) && 
		( ( strcmp( iniFile, "AIX" ) ==0 ) || 
		  ( strcmp( iniFile, "Sun" ) == 0 ) || 
		  ( strcmp( iniFile, "sun_china" ) == 0 ) ) )
	{
		float fTotalMemory(0.0);
		float fFreeMemory(0.0);
		float fPercentUsed(0.0);

		char buffer[1024] = {0};

		//  ◊œ»ªÒ»°◊‹ŒÔ¿Ìƒ⁄¥Ê
		try
		{
			if(!RETURN_COMMAND( TOTAL_PMEMORY, iniFile, cmd ) )
			{
				sprintf( szReturn, "error=%s", FuncGetStringFromIDS( "SV_BASIC",
					"BASIC_READ_CMDFILE_FAILD" ) );// <%IDS_TelMonitor_14%>
				printf( szReturn );
				bResult = FALSE;
				goto err;
			}
			//OutputDebugString( cmd );
		}
		catch(...)
		{
			bResult=false;
			goto err;
		}


		if(ca = strchr(cmd, '<')) 
		{
			//OutputDebugString(extraparam);
			strcpy(ca, extraparam);
			*(ca + strlen(extraparam)) = 0;
		}

		memset( pgd,0,sizeof(GDATA) );

		if((ret = telnet_command( hSocket, cmd, pgd, &param ) ) < 0 )
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
					"SOCKET_READ_DATA_ERROR"));// <%IDS_TelMonitor_15%>
			bResult = false;
			goto err;
		}

		try
		{
			//OutputDebugString( pgd->databuffer );
			if( !MONITOR_PARSE( TOTAL_PMEMORY, pgd->databuffer, inifile, extraparam, buffer ) )
			{
				printf("parse error\n");
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
					"BASIC_PARSER_STRING_FAILED"));// <%IDS_TelMonitor_16%>
				bResult = false;
				goto err;
			}
		}
		catch(...)
		{
			bResult=false;
			goto err;
		}

		char* pszTemp = strchr( buffer, '=' ) + 1;

		fTotalMemory =(float)atof( pszTemp );



		// »ª∫ÛªÒ»°ŒÔ¿Ìƒ⁄¥Ê £”‡¡ø
		try
		{
			if(!RETURN_COMMAND( FREE_PMEMORY, iniFile, cmd ) )
			{
				sprintf( szReturn, "error=%s", FuncGetStringFromIDS( "SV_BASIC",
					"BASIC_READ_CMDFILE_FAILD" ) );// <%IDS_TelMonitor_14%>
				printf( szReturn );
				bResult = FALSE;
				goto err;
			}
			//OutputDebugString( cmd );
		}
		catch(...)
		{
			bResult=false;
			goto err;
		}


		if(ca = strchr(cmd, '<')) 
		{
			//OutputDebugString(extraparam);
			strcpy(ca, extraparam);
			*(ca + strlen(extraparam)) = 0;
		}

		memset( pgd,0,sizeof(GDATA) );

		if((ret = telnet_command( hSocket, cmd, pgd, &param ) ) < 0 )
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
					"SOCKET_READ_DATA_ERROR"));// <%IDS_TelMonitor_15%>
			bResult = false;
			goto err;
		}

		try
		{	
			memset( buffer, 0, sizeof( buffer ) );

			//OutputDebugString( pgd->databuffer );
			if( !MONITOR_PARSE( FREE_PMEMORY, pgd->databuffer, inifile, extraparam, buffer ) )
			{
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
					"BASIC_PARSER_STRING_FAILED"));// <%IDS_TelMonitor_16%>
				bResult = false;
				goto err;
			}
		}
		catch(...)
		{
			bResult=false;
			goto err;
		}

		pszTemp = strchr( buffer, '=' ) + 1;

		fFreeMemory =(float)atof( pszTemp );

		if( fTotalMemory > 0 )
		{
            fPercentUsed = ( fTotalMemory - fFreeMemory ) / fTotalMemory;
			fTotalMemory /= ( 1024*1024 );
			fFreeMemory /= ( 1024*1024 );
			fPercentUsed *= 100;
		}

		sprintf( szReturn, "totalMemory=%.2f$freeMemory=%.2f$percentUsed=%.2f$", fTotalMemory, fFreeMemory, fPercentUsed );


	}// Ω· ¯∏¸∏ƒ
	else
	{
		//Get Command from OS config file
		try
		{
			if(!RETURN_COMMAND(mtype, iniFile, cmd))
			{//Read File Failed
				WriteLog( "ªÒ»°√¸¡Ó ß∞‹£°" );
				printf("type=%d", mtype);
				printf("inifile=%s", inifile);
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
					"BASIC_READ_CMDFILE_FAILD"));    //  <%IDS_TelMonitor_14%>
				printf(szReturn);
				bResult = FALSE;
				goto err;
			}
			OutputDebugString(cmd);
		}
		catch(...)
		{
			bResult=false;
			goto err;
		}
			////////////////////////////////////////////////////
	//	::ReleaseMutex(hMutex);
	/*    if(mtype == SERVICE_TYPE_MONITOR)
		{
			if(extraparam)
			{
				int nCmdLen = strlen(cmd);
				memcpy(cmd + nCmdLen, "|grep \"", 7);
				cmd[nCmdLen + 7] = '\0';
				nCmdLen = strlen(cmd);
				memcpy(cmd + nCmdLen, extraparam, strlen(extraparam));
				cmd[nCmdLen + strlen(extraparam)] = '\"';
				cmd[nCmdLen + strlen(extraparam) + 1] = '\0';
				nCmdLen = strlen(cmd);
				char chPara[100] = "|awk 'BEGIN {print \"     UID   PID  PPID   C" \
					"STIME    TTY  TIME CMD\"}{print $0}'\0";
				memcpy(cmd + nCmdLen, chPara, strlen(chPara));
				cmd[nCmdLen + strlen(chPara)] = '\0';
			}
		}
	*/
		//////////////////////////////////////////////////
		//If need additive parameter then Edit Command
		if(ca = strchr(cmd, '<')) 
		{
			OutputDebugString(extraparam);
			strcpy(ca, extraparam);
			*(ca + strlen(extraparam)) = 0;
		}
		memset(pgd,0,sizeof(GDATA));
	//    WriteLogFile(cmd);
		///////////////////////////////////////////////////
		//	puts(cmd);
		///////////////////////////////////////////////////
		//Execute Command 
		if((ret = telnet_command(hSocket, cmd,pgd, &param)) < 0)
		{
			WriteLog( "ªÒ»° ˝æ› ß∞‹£°" );

			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
					"SOCKET_READ_DATA_ERROR"));    //  <%IDS_TelMonitor_15%>
			bResult = false;
			goto err;
		}

	//	puts(pgd->databuffer);
		//strTemp.Format("%s",pgd->databuffer);
		//strTemp.Replace("ÔÔˇ?˙^_ÔÔecho: not found\n","");
		//strcpy(pgd->databuffer,strTemp.GetBuffer(strTemp.GetLength()));
		//puts(pgd->databuffer);
		
		////////////////////////////////////////////////
		//Parser return string by AimParser.dll
		try
		{		
			OutputDebugString(pgd->databuffer); //zjw
			if(!MONITOR_PARSE(mtype, pgd->databuffer, inifile, extraparam, szReturn))
			{
				printf("parse error\n");
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
					"BASIC_PARSER_STRING_FAILED"));   //  <%IDS_TelMonitor_16%>
				WriteLogFile(cmd);
				WriteLogFile(pgd->databuffer);
				bResult = false;
				goto err;
			}
		}
		catch(...)
		{
			bResult=false;
			goto err;
		}
		//WriteLogFile(pgd->databuffer);
	} // Ω· ¯∑«aix∫ÕsolarisŒÔ¿Ìƒ⁄¥Ê–≈œ¢
err:

	telnet_command(hSocket,"exit",pgd);
	Sleep(500);
	
	delete pgd;
	shutdown(hSocket, 0x02);
	closesocket(hSocket);

	//Add By zjw for trace
	//CString str, sTime;
	//sTime = COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
	//ofstream fout("TelnetError.log",ios::app);
	//fout << sTime << ":" << szReturn <<"\r\n"; 
	//fout << flush; 
	//fout.close();

	return bResult;
}
