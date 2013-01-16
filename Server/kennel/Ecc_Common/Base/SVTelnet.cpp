//SVTelnet.cpp
#include "SVTelnet.h"
#define		NUL					0
#define		BEL					7
#define		BS					8
#define		HT					9
#define		LF					10
#define		VT					11
#define		FF					12
#define		CR					13
#define		SE					240
#define		NOP					241
#define		DM					242
#define		BRK					243
#define		IP					244
#define		AO					245
#define		AYT					246
#define		EC					247
#define		EL					248
#define		GA					249
#define		SB					250
#define		WILL				251
#define		WONT				252
#define		DO					253
#define		DONT				254
#define		IAC					255

#define		C_STA				"DF-CMD-STA"
#define		C_END				"DF-CMD-END"

#define		NUM_TERMINALS		2
#define		NUM_CODEC			6
#define		READ_TIMEOUT		20
#define		CONNECT_TIMEOUT		10
///----------------------------------------------------------------------
#define			PLINK_PATH		"plink.exe"
#define			CALL_TIMEOUT	20

#define			DragonFlow_OK							0
#define			DragonFlow_Timeout						1
#define			DragonFlow_CreateOutputThread			2
#define			DragonFlow_CreateErrorOutputThread		3
#define			DragonFlow_CreateInputThread			4
#define			DragonFlow_WriteToStandardOutput		5
#define			DragonFlow_OpenConnection				6
#define			DragonFlow_OpenCommandFile				7
#define			DragonFlow_NotEnoughSpace				8
#define			DragonFlow_InitialiseWinSock			9
#define			DragonFlow_WinSockVersionIncompatible	10
#define			DragonFlow_FatalError					11
#define			DragonFlow_UnknownError					12

//extern void WriteLogFile(char* );

//-----------------------------------------------------------------------

typedef void(*LPDATAPROC)(SOCKET, unsigned char data,LPGDATA pgd);



struct OL ol[] = {
	{TOPT_ECHO,   ddww_echo,  NULL},
	{TOPT_SUPP,   ddww_supp,  NULL},
	{TOPT_TERM,   ddww_term,  sbproc_term},
	{TOPT_ERROR,  ddww_error, NULL}
};

struct TERMINAL terminal[NUM_TERMINALS] = {
	{ "NVT", nvt }, 
	{ "ANSI", ansi },
};


struct CODEC codec[NUM_CODEC] = {
	{'m',ansi_set_screen_attribute},
	{'H',ansi_set_position},
	{'K',ansi_erase_line},
	{'J',ansi_erase_screen},
	{'A',ansi_move_up},
	{0,0}
	};


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


void ansi_set_screen_attribute(char* buffer)
{

}

void ansi_erase_line(char* buffer)
{
}

void ansi_set_position(char* buffer)
{
}

void ansi_erase_screen(char* buffer)
{

}

void ansi_move_up(char* buffer)
{

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

	int i=0;

	static _ansi_state state = as_normal;
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
		if(data > 64)
		{
			pgd->codebuf[pgd->codeptr] = 0;

			for(; codec[i].cmd && codec[i].cmd != data; i++);
			if(codec[i].proc)
				codec[i].proc(pgd->codebuf);

#ifdef _DEBUG
			else
			{
				char buf[256];
				wsprintf(buf,"Unknown Ansi code:'%c' (%s)\n",data,pgd->codebuf);
				OutputDebugString(buf);
			}
#endif
			state = as_normal;
		}
		else
			pgd->codebuf[pgd->codeptr++] = data;
		break;
	}
}


int SVUnixOpMonitor::connect_timeo(int sockfd,const struct sockaddr *saptr,int salen,int nsec)
{
	int n;
	int error;
	socklen_t len;
	fd_set rset,wset;
	struct timeval tv;
	
	u_long val = 1;
	error = 0;

	
#ifdef 	WIN32
	ioctlsocket(sockfd,FIONBIO,&val);
	if((n = connect(sockfd,saptr,salen)) < 0)
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK) 
		{
			return -1;
		}
	}	
#else
	if((n = connect(sockfd,saptr,salen)) == -1)
	{
		return -1;
	}
#endif


	if(n == 0)
		goto done;

	FD_ZERO(&rset);
	FD_SET(sockfd,&rset);
	wset = rset;
	tv.tv_sec = nsec;
	tv.tv_usec = 0;

	if((n = select(sockfd + 1,&rset,&wset,NULL,nsec?&tv:NULL)) == 0)
	{
		shutdown(sockfd, 0x02);
		closesocket(sockfd);
#ifdef WIN32
		WSASetLastError(WSAETIMEDOUT);
#endif
		return -1;
	}

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
		return -1;
	}		

done:
	val = 0;
#ifdef WIN32
	ioctlsocket(sockfd,FIONBIO,&val);
#endif
	if(error)
	{
		shutdown(sockfd, 0x02);
		closesocket(sockfd);
#ifdef WIN32
		WSASetLastError(error);
#endif
		return -1;
	}	
	return 0;
}


int SVUnixOpMonitor::RESPONSE_WAIT(SOCKET s, int nsec)
{
	struct timeval Timeout;
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(s,&readfds);
//	readfds.fd_count = 1;
//	readfds.fd_array[0] = s;
	Timeout.tv_sec = nsec;
    Timeout.tv_usec = 0;
	return(select(s+1, &readfds, NULL, NULL, &Timeout));
}

int SVUnixOpMonitor::telnetExecCmd(char * strCmd)
{
	SetCmd(strCmd);
	//return telnet();
	return  telnet_command();

}
int SVUnixOpMonitor::telnetOpen()
{
	int nResult;

	nResult=telnetinit();
	if(SVFAILED(nResult)) return nResult;
	for(int nLoop=0;nLoop<2;nLoop++  )
	{
		nResult=telnet_connect();

		if(!SVFAILED(nResult)) break;
	}
	if(SVFAILED(nResult)) return nResult;
	/*
	nResult=telnet_command();
	if(SVFAILED(nResult)) return nResult;*/
	return TELNETSUCCESS;

}
BOOL SVUnixOpMonitor::init()
{
	return TRUE;
}
SVUnixOpMonitor::SVUnixOpMonitor()
{
	hSocket=0;
	pgd=NULL;
	pgd=new GDATA;
	//if(pgd==NULL) return FALSE;
	memset(pgd,0,sizeof(GDATA));
	strUser=NULL;//用户名
	strUserPrompt=NULL;//login提示符
	strPwd=NULL;//密码
	strPwdPrompt=NULL;//密码提示符
	strCmd=NULL;	    //命令
	strRemoteHost=NULL;//远程主机
	strLoginPrompt=NULL; //登陆提示符
    state = state_data;
	verb = verb_sb;
//	return TRUE;
		
}
SVUnixOpMonitor::~SVUnixOpMonitor() 
{
	if (hSocket!=0) {
		telnetclose();
	}
	if(strUserPrompt!=NULL)
			free(strUserPrompt);
	 if(strUser!=NULL)
			free (strUser);
	 if(strCmd!=NULL)
			free(strCmd);
	 if(strRemoteHost!=NULL)
			free(strRemoteHost);
	 if(strPwd!=NULL)
			free(strPwd);
	 if(strPwdPrompt!=NULL)
			free( strPwdPrompt);
	 if(strLoginPrompt!=NULL)
			free(strLoginPrompt);

	 if(pgd!=NULL) delete pgd;
	 
	 //return TRUE;

}
/*
BOOL SVUnixOpMonitor::init(  CString RemoteHost,int Port,
		CString User,  CString Pwd, 
		CString UserPrompt,  CString PwdPrompt, CString LoginPrompt)
{
	if(!init()) return FALSE;
	SetUser(User.GetBuffer(User.GetLength()));
	SetUserPrompt(UserPrompt.GetBuffer(User.GetLength()));
	SetPwd(Pwd.GetBuffer(Pwd.GetLength()));
	SetPwdPrompt(PwdPrompt.GetBuffer(PwdPrompt.GetLength()));
	SetRemoteHost(RemoteHost.GetBuffer(RemoteHost.GetLength()));
	SetPort(Port);
	
	SetLoginPrompt(LoginPrompt.GetBuffer(LoginPrompt.GetLength()));
	return TRUE;	
}
*/

bool SVUnixOpMonitor::init( string RemoteHost,int Port,
		string User,  string Pwd, 
		string UserPrompt,  string PwdPrompt, string LoginPrompt)
{
	if(!init()) return FALSE;
	SetUser(User.c_str());
	SetUserPrompt(UserPrompt.c_str());
	SetPwd(Pwd.c_str());
	SetPwdPrompt(PwdPrompt.c_str());
	SetRemoteHost(RemoteHost.c_str());
	SetPort(Port);
	
	SetLoginPrompt(LoginPrompt.c_str());
	return true;	
}

BOOL SVUnixOpMonitor::init(  char * RemoteHost,int Port,
		char * User,  char * Pwd, 
		char * UserPrompt,  char * PwdPrompt, char * LoginPrompt)
{
	if(!init()) return FALSE;
	SetUser(User);
	SetUserPrompt(UserPrompt);
	SetPwd(Pwd);
	SetPwdPrompt(PwdPrompt);
	SetRemoteHost(RemoteHost);
	SetPort(Port);
	
	SetLoginPrompt(LoginPrompt);
	return TRUE;	
}

BOOL SVUnixOpMonitor::telnetclose()
{
	if (hSocket!=0) {
		telnetExecCmd("exit");
		Sleep(500);
		shutdown(hSocket, 0x02);
		closesocket(hSocket);
		hSocket =0;
	}

	return TRUE;	
}

BOOL SVUnixOpMonitor::SVFAILED(int nResult)
{
	if(TELNETSUCCESS==nResult) return FALSE;
	else return TRUE;
}


int SVUnixOpMonitor::telnetinit()
{
	unsigned long	ip;
	hostent			*ent;
	sockaddr_in		sa;
//	SOCKET			hSocket;

	ent = gethostbyname(strRemoteHost);
	if(ent == NULL) 
		return HOSTERROR;

	ip = *(unsigned long*)(ent->h_addr);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(nPort);
	sa.sin_addr = *(in_addr*)&ip;

	hSocket = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (hSocket == INVALID_SOCKET) 
		return SOCKETINITERROR;

	if(connect_timeo(hSocket, (sockaddr*)&sa, sizeof(sockaddr), CONNECT_TIMEOUT) < 0)
		return CONNECTTIMEOUTERROR;

	return TELNETSUCCESS;
}

int SVUnixOpMonitor::telnet_setlinewidth(BYTE HighByte, BYTE LowByte)
{
    BYTE SB_NAWS[9];
    SB_NAWS[0] = 0xff;
    SB_NAWS[1] = 0xfa;
    SB_NAWS[2] = 0x1f;
    SB_NAWS[3] = HighByte;
    SB_NAWS[4] = LowByte;
    SB_NAWS[5] = 0x00;
    SB_NAWS[6] = 0xef;
    SB_NAWS[7] = 0xff;
    SB_NAWS[8] = 0xf0;
    send(hSocket, (char*)SB_NAWS, 9, 0);
    return 1;
}
int SVUnixOpMonitor::telnet_connect()
{
	int		n = 0;
	char	buffer[4096] = {0};
	char	*scan = NULL;
	int		telnetprogress = 0;
	int		count = 0;

	memset(pgd->databuffer, 0, TELNET_BUF_SIZE);
	pgd->datalen = 0;
	int nTime=0;
    DataProc = terminal[(pgd->term_index==NUM_TERMINALS)?(NUM_TERMINALS-1):pgd->term_index].termproc;
	while(1)
	{
LOOP:
		memset(buffer, 0, 4096);

		if(RESPONSE_WAIT(hSocket, READ_TIMEOUT) <= 0)
		{
			if(nTime<1)
			{
				char buf[10]={0};
				sprintf(buf,"\r\n");
				send(hSocket, buf, strlen(buf),0);
				nTime++;
				goto LOOP;
			}

			if(telnetprogress == 0)
				return FISTCONNECTERROR;
			if(telnetprogress == 1) 
				return USERLOGINERROR;
			if(telnetprogress == 2) 
				return PWDLOGINERROR;

		}



		n = recv(hSocket, buffer, 4096, 0);

		if(n == SOCKET_ERROR)
			return LOGINSOCKETERROR;

		buffer[n] = 0;

		scan = buffer;
		while(n--)
		{
			telnet_protocol(hSocket, *scan++,pgd);
			//TRACE("%c", *scan);
		}

		switch(telnetprogress)
		{
		case 0:

			if(strstr(pgd->databuffer, strUserPrompt))
			{

				char	buf[255] = {0};
				strcpy(buf, strUser);

				for(int nS=0;nS<strlen(buf);nS++)
				{

						Sleep(20);
						send(hSocket, &buf[nS], 1,0);	
				}
				send(hSocket,"\r\n",2,0);

//				strcat(buf, "\r\n");
//				Sleep(100);			// delay
//				send(hSocket, buf, strlen(buf),0);
				telnetprogress ++;
				count = 0;
				memset(pgd->databuffer, 0, TELNET_BUF_SIZE);
				pgd->datalen = 0;
			}
			break;
		case 1:
			if(strstr(pgd->databuffer, strPwdPrompt))
			{
				char	buf[255] = {0};
				strcpy(buf, strPwd);
				//---wangpeng
				for(int nS=0;nS<strlen(buf);nS++)
				{
					Sleep(20);
					send(hSocket, &buf[nS], 1,0);	
				}
				 send(hSocket,"\r\n",2,0);
				//---------结束------------------
				telnetprogress ++;
				count = 0;
				memset(pgd->databuffer, 0, TELNET_BUF_SIZE);
				pgd->datalen = 0;
			}
			break;
		case 2:
			if(strstr(pgd->databuffer, "ogin incorrect"))
			{
				memset(pgd->databuffer, 0, TELNET_BUF_SIZE);
				pgd->datalen = 0;
				return LOGININCORRECTERROR;
			}

			int		i = 0;
			char	prompt[256];
			while(i < PR_NUM && *arprompt[i])
			{
				memset(prompt, 0, sizeof(prompt));
				strcpy(prompt, arprompt[i]);

				if(strstr(pgd->databuffer, prompt))
				{
					memset(pgd->databuffer, 0, TELNET_BUF_SIZE);
					pgd->datalen = 0;
					return TELNETSUCCESS;
				}

				i ++;
			}
			break;
		}

		if(++count > 80)
		{
			if(telnetprogress == 0)
				return FISTCONNECTERROR;
			if(telnetprogress == 1) 
				return USERLOGINERROR;
			if(telnetprogress == 2) 
				return PWDLOGINERROR;
		}
		//改变默认的telnet行的长度
        telnet_setlinewidth(0x00, 0xef);
	}

}

void SVUnixOpMonitor::telnet_protocol(SOCKET server,unsigned char code,LPGDATA pgd)
{
	int i = 0;
	switch(state)
	{
	case state_data:
		switch(code)
		{
		case IAC: state = state_code; break;
		default: DataProc(server,code,pgd);
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
		for(;ol[i].option != TOPT_ERROR && ol[i].option != code;i++);
		//Do some verb specific stuff
		if(verb == verb_sb)
			DataProc = ol[i].DataProc;
		else
			ol[i].OptionProc(server,verb,(_option)code);
		break;
	}
}

void SVUnixOpMonitor::SetLoginPrompt(const char * LoginPrompt)
{
		if(strLoginPrompt!=NULL)
			free( strLoginPrompt);
		strLoginPrompt =(char *) malloc(strlen(LoginPrompt)+1);
		strcpy(strLoginPrompt ,LoginPrompt);


		const char * prompts;
		//prompts =LoginPrompt.GetBuffer(LoginPrompt.GetLength());
		prompts =LoginPrompt;
		int		c = 0;
		const char	*ca = prompts, *cb = NULL;
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

	}


int SVUnixOpMonitor::telnet_command()
{
	int		n = 0;
	char	buffer[4096] = {0};
	char	*scan = NULL;
	char	*ca = NULL, *cb = NULL;

	sprintf(buffer, "echo %s;%s;echo %s\r\n", C_STA, strCmd, C_END);

	send(hSocket, buffer, strlen(buffer), 0);

	memset(pgd->databuffer, 0, TELNET_BUF_SIZE);
	pgd->datalen = 0;
	while(RESPONSE_WAIT(hSocket, READ_TIMEOUT))
	{
		n = recv(hSocket, buffer, 4095, 0);
		if(n == SOCKET_ERROR) return TELNETCOMMANDERROR;
		if(n == 0) return TELNETCOMMANDERROR;
		buffer[n] = 0;

		scan = buffer;
		while(n--) telnet_protocol(hSocket, *scan++,pgd);
	
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

	pgd->datalen=strlen(pgd->databuffer);
	return TELNETSUCCESS;
}

int SVUnixOpMonitor::SSHCMD(char *strCmd)
{
	SetCmd(strCmd);
	return SSH();	
}
int SVUnixOpMonitor::SSH()
{
/*
	CString strCmdLine;

	strCmdLine.Format("%s \"%s\" \"-ssh\" \"-P\" \"%d\" \"-pw\" \"%s\" \"-l\" \"%s\" \"%s\"  ", PLINK_PATH, 
		strRemoteHost,nPort, strPwd, strUser,  strCmd);
*/
	char strCmdLine[5000] ={0};
	sprintf(strCmdLine,"%s \"%s\" \"-ssh\" \"-P\" \"%d\" \"-pw\" \"%s\" \"-l\" \"%s\" \"%s\"  ", PLINK_PATH, 
		strRemoteHost,nPort, strPwd, strUser,  strCmd);

	int nResult = 0;
	nResult = executeExeFile(strCmdLine);

	return nResult;
}


#ifdef	WIN32
int SVUnixOpMonitor::executeExeFile(char* strCmdLine)
{
	CString strContent;
	HANDLE hOutputRead, hOutputWrite;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	if (CreatePipe(&hOutputRead, &hOutputWrite, &sa, 0)) {
		STARTUPINFO si;
		memset(&si, 0, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdOutput = hOutputWrite;
		si.hStdError = hOutputWrite;
		
		PROCESS_INFORMATION pi;
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));

		if (CreateProcess(NULL, strCmdLine,  NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
			char buffer[TELNET_BUF_SIZE] = {0};

			DWORD bytesRead = 0;

			while(1)
			{
				int index = 0;
				if(ReadFile(hOutputRead, buffer, sizeof(buffer), &bytesRead, NULL))
				{
					buffer[bytesRead] = 0;
					strContent += buffer;
					index = strContent.Find("DragonFlow: ", 0);
					if(index >= 0)
					{
						TerminateProcess(pi.hProcess, 1);
						break;
					}
				}
				else
				{
					TerminateProcess(pi.hProcess, 1);
					break;
				}
			}

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}

		CloseHandle(hOutputRead);
		CloseHandle(hOutputWrite);
	}

	
	int index = 0;
	index = strContent.Find("DragonFlow: OK", 0);
	if(index >= 0)
	{
		strContent.Delete(index, strContent.GetLength() - index);
		strcpy(pgd->databuffer,strContent.GetBuffer(strContent.GetLength()));
		pgd->datalen=strContent.GetLength();

		return TELNETSUCCESS;
	}
	else if(strContent.Find("DragonFlow: Timeout") >= 0)
		return DragonFlow_Timeout	;
	else if(strContent.Find("DragonFlow: CreateOutputThread") >= 0)
		return DragonFlow_CreateOutputThread;
	else if(strContent.Find("DragonFlow: CreateErrorOutputThread") >= 0)
		return DragonFlow_CreateErrorOutputThread;
	else if(strContent.Find("DragonFlow: CreateInputThread") >= 0)
		return DragonFlow_CreateInputThread;
	else if(strContent.Find("DragonFlow: WriteToStandardOutput") >= 0)
		return DragonFlow_WriteToStandardOutput;
	else if(strContent.Find("DragonFlow: OpenConnection") >= 0)
		return DragonFlow_OpenConnection;
	else if(strContent.Find("DragonFlow: OpenCommandFile") >= 0)
		return DragonFlow_OpenCommandFile;
	else if(strContent.Find("DragonFlow: NotEnoughSpace") >= 0)
		return DragonFlow_NotEnoughSpace;
	else if(strContent.Find("DragonFlow: InitialiseWinSock") >= 0)
		return DragonFlow_InitialiseWinSock;
	else if(strContent.Find("DragonFlow: WinSockVersionIncompatible") >= 0)
		return DragonFlow_WinSockVersionIncompatible;
	else if(strContent.Find("DragonFlow: Fatal Error") >= 0)
		return DragonFlow_FatalError;
	else if(strContent.Find("DragonFlow") >= 0)
	{
		int nErrIn=strContent.Find("DragonFlow");
		int nEnd =strContent.Find("\r",nErrIn);
		strContent.Delete(0,nErrIn);
		strContent=strContent.Left(nEnd-nErrIn);		
		return 12;
	}

	else
		return DragonFlow_UnknownError;
}
#else
int SVUnixOpMonitor::executeExeFile(char* strCmdLine)  //empty
{
	return 0;
}
#endif

