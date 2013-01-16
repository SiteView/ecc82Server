//SVTelnet.cpp
#include "SVTelnet.h"
#include <scsvapi/svapi.h>

const int   NUL     = 0;
const int   BEL     = 7;
const int   BS      = 8;
const int   HT      = 9;
const int   LF      = 10;
const int   VT      = 11;
const int   FF      = 12;
const int   CR      = 13;
const int   SE      = 240;
const int   NOP     = 241;
const int   DM      = 242;
const int   BRK     = 243;
const int   IP      = 244;
const int   AO      = 245;
const int   AYT     = 246;
const int   EC      = 247;
const int   EL      = 248;
const int   GA      = 249;
const int   SB      = 250;
const int   WILL    = 251;
const int   WONT    = 252;
const int   DO      = 253;
const int   DONT    = 254;
const int   IAC     = 255;

const char  C_STA[] = "DF-CMD-STA";
const char  C_END[] = "DF-CMD-END";

const int   NUM_TERMINALS   = 2;
const int   NUM_CODEC       = 6;
const int   READ_TIMEOUT    = 20;
const int   CONNECT_TIMEOUT = 10;
///----------------------------------------------------------------------
#define			PLINK_PATH		"plink.exe"
#define			CALL_TIMEOUT	20

enum SiteView_ECC_STATE
{
    SiteView_ECC_OK							=   0,
    SiteView_ECC_Timeout					=	1,
    SiteView_ECC_CreateOutputThread			=   2,
    SiteView_ECC_CreateErrorOutputThread	=	3,
    SiteView_ECC_CreateInputThread			=   4,
    SiteView_ECC_WriteToStandardOutput		=   5,
    SiteView_ECC_OpenConnection				=   6,
    SiteView_ECC_OpenCommandFile			=	7,
    SiteView_ECC_NotEnoughSpace				=   8,
    SiteView_ECC_InitialiseWinSock			=   9,
    SiteView_ECC_WinSockVersionIncompatible	=   10,
    SiteView_ECC_FatalError					=   11,
    SiteView_ECC_UnknownError				=	12
};

//extern void WriteLogFile(char* );

//-----------------------------------------------------------------------

typedef void(*LPDATAPROC)(SOCKET, unsigned char data,LPGDATA pgd);

struct OL ol[] = 
{
	{TOPT_ECHO,   ddww_echo,  NULL},
	{TOPT_SUPP,   ddww_supp,  NULL},
	{TOPT_TERM,   ddww_term,  sbproc_term},
	{TOPT_ERROR,  ddww_error, NULL}
};

struct TERMINAL terminal[NUM_TERMINALS] =
{
	{ "NVT",    nvt  }, 
	{ "ANSI",   ansi },
};


struct CODEC codec[NUM_CODEC] = 
{
	{'m',   ansi_set_screen_attribute},
	{'H',   ansi_set_position},
	{'K',   ansi_erase_line},
	{'J',   ansi_erase_screen},
	{'A',   ansi_move_up},
	{0,     0}
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

void nvt(SOCKET server, unsigned char data, LPGDATA pgd)
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
			for(int i=0; codec[i].cmd && codec[i].cmd != data; i++);
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
		if(WSAGetLastError() != WSAEWOULDBLOCK) 
		{
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

	if((n = select(sockfd + 1,&rset,&wset,NULL,nsec?&tv:NULL)) == 0)
	{
		shutdown(sockfd, 0x02);
		closesocket(sockfd);
		WSASetLastError(WSAETIMEDOUT);
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

int SVUnixOpMonitor::telnetExecCmd(const char * pszCmd)
{
	SetCmd(pszCmd);
	return  telnet_command();
}

int SVUnixOpMonitor::telnetOpen()
{
	int nResult;

	nResult=telnetinit();
	if(SVFAILED(nResult)) return nResult;
	for(int nLoop=0; nLoop<2; nLoop++  )
	{
		nResult=telnet_connect();
		if(!SVFAILED(nResult)) break;
	}
	if(SVFAILED(nResult)) 
        return nResult;

	return TELNETSUCCESS;
}


SVUnixOpMonitor::SVUnixOpMonitor()
{
	hSocket = 0;
	//pgd     = NULL;
	pgd     = new GDATA;
	memset(pgd, 0, sizeof(GDATA));
	m_pszUser         = NULL;     //用户名
	m_pszUserPrompt   = NULL;     //login提示符
	m_pszPwd          = NULL;     //密码
	m_pszPwdPrompt    = NULL;     //密码提示符
	m_pszCmd          = NULL;	    //命令
	m_pszRemoteHost   = NULL;     //远程主机
	m_pszLoginPrompt  = NULL;     //登陆提示符
    m_nState          = state_data;
	verb            = verb_sb;		
}

SVUnixOpMonitor::~SVUnixOpMonitor() 
{
	if (hSocket != 0) 
        telnetclose();

    if(m_pszUserPrompt != NULL)
        delete []m_pszUserPrompt;

    if(m_pszUser != NULL)
        delete []m_pszUser;

    if(m_pszCmd != NULL)
        delete []m_pszCmd;

    if(m_pszRemoteHost != NULL)
        delete []m_pszRemoteHost;

    if(m_pszPwd != NULL)
        delete []m_pszPwd;

    if(m_pszPwdPrompt != NULL)
        delete []m_pszPwdPrompt;

    if(m_pszLoginPrompt != NULL)
        delete []m_pszLoginPrompt;

    if(pgd!=NULL) delete pgd;
}

//BOOL SVUnixOpMonitor::init(  CString RemoteHost,int Port,
//		CString User,  CString Pwd, 
//		CString UserPrompt,  CString PwdPrompt, CString LoginPrompt)
//{
//	if(!init()) return FALSE;
//	SetUser(User.GetBuffer(User.GetLength()));
//	SetUserPrompt(UserPrompt.GetBuffer(User.GetLength()));
//	SetPwd(Pwd.GetBuffer(Pwd.GetLength()));
//	SetPwdPrompt(PwdPrompt.GetBuffer(PwdPrompt.GetLength()));
//	SetRemoteHost(RemoteHost.GetBuffer(RemoteHost.GetLength()));
//	SetPort(Port);
//	
//	SetLoginPrompt(LoginPrompt.GetBuffer(LoginPrompt.GetLength()));
//	return TRUE;	
//}

bool SVUnixOpMonitor::init(const char * pszRemoteHost, const int &nPort, const char * pszUser, 
                const char * pszPwd, const char * pszUserPrompt, const char * pszPwdPrompt,
                const char * pszLoginPrompt)
{
	SetUser(pszUser);
	SetUserPrompt(pszUserPrompt);
	SetPwd(pszPwd);
	SetPwdPrompt(pszPwdPrompt);
	SetRemoteHost(pszRemoteHost);
	SetPort(nPort);	
	SetLoginPrompt(pszLoginPrompt);
	return true;
}

bool SVUnixOpMonitor::telnetclose()
{
	if (hSocket!=0) 
    {
		telnetExecCmd("exit");
		Sleep(500);
		shutdown(hSocket, 0x02);
		closesocket(hSocket);
		hSocket =0;
	}
	return true;	
}

bool SVUnixOpMonitor::SVFAILED(int nResult)
{
	if(TELNETSUCCESS == nResult) 
        return false;
	else 
        return true;
}


int SVUnixOpMonitor::telnetinit()
{
	unsigned long	ip;
	hostent			*ent;
	sockaddr_in		sa;
//	SOCKET			hSocket;

	ent = gethostbyname(m_pszRemoteHost);
	if(ent == NULL) 
		return HOSTERROR;

	ip = *(unsigned long*)(ent->h_addr);

	sa.sin_family   = AF_INET;
	sa.sin_port     = htons(m_nPort);
	sa.sin_addr     = *(in_addr*)&ip;

	hSocket = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (hSocket == INVALID_SOCKET) 
		return SOCKETINITERROR;

	if(connect_timeo(static_cast<int>(hSocket), (sockaddr*)&sa, sizeof(sockaddr), CONNECT_TIMEOUT) < 0)
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
		memset(buffer, 0, sizeof(buffer));
		if(RESPONSE_WAIT(hSocket, READ_TIMEOUT) <= 0)
		{
			if(nTime < 1)
			{
				char buf[10]={0};
				sprintf(buf,"\r\n");
				send(hSocket, buf, static_cast<int>(strlen(buf)) ,0);
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


		n = recv(hSocket, buffer, sizeof(buffer) - 1, 0);
		if(n == SOCKET_ERROR)
			return LOGINSOCKETERROR;
		buffer[n] = 0;

		scan = buffer;
	    //memset(pgd->databuffer, 0, TELNET_BUF_SIZE);
	    //pgd->datalen = 0;

		while(n--)
        {
			telnet_protocol(hSocket, *scan++, pgd);
            //cout << *scan ;
        }

        //cout << pgd->databuffer << endl;
        //cout << m_pszUserPrompt << endl;
		switch(telnetprogress)
		{
		case 0:
            //cout << "login server" << endl;
			if(strstr(pgd->databuffer, m_pszUserPrompt))
			{
                //cout << "send user name" << endl;
				char	buf[255] = {0};
                strcpy(buf, m_pszUser);
                cout << "user name is: " << m_pszUser << endl;
    //            int nSize = strlen(buf);
				//for(int nS = 0; nS < nSize; nS++)
				//{
				//	Sleep(20);
				//	send(hSocket, &buf[nS], 1, 0);	
				//}
				//send(hSocket,"\r\n",2,0);

				strcat(buf, "\r\n");
				Sleep(100);			// delay
				send(hSocket, buf, static_cast<int>(strlen(buf)), 0);
				telnetprogress ++;
				count = 0;
				memset(pgd->databuffer, 0, TELNET_BUF_SIZE);
				pgd->datalen = 0;
			}
			break;
		case 1:
			if(strstr(pgd->databuffer, m_pszPwdPrompt))
			{
				char	buf[255] = {0};
				strcpy(buf, m_pszPwd);
				//---wangpeng
                int nSize = static_cast<int>(strlen(buf));
				for(int nS = 0; nS < nSize; nS++)
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

void SVUnixOpMonitor::telnet_protocol(SOCKET server, unsigned char code, LPGDATA pgd)
{
	//Decide what to do (state based)
	switch(m_nState)
	{
	case state_data:
		switch(code)
		{
		case IAC:
            m_nState = state_code; 
            break;
		default: 
            DataProc(server,code,pgd);
            break;
		}
		break;
	case state_code:
		m_nState = state_data;
		switch(code)
		{
		// State transition back to data
		case IAC: 
			DataProc(server, code, pgd);
			break;
		// Code state transitions back to data
		case SE:
			DataProc = terminal[(pgd->term_index==NUM_TERMINALS)?(NUM_TERMINALS-1):pgd->term_index].termproc;
			break;
		case NOP:
		case DM:
		case BRK:
		case IP:
		case AO:
		case AYT:
		case EC:
		case EL:
		case GA:
			break;
		// Transitions to option state
		case SB:
			verb = verb_sb;
			m_nState = state_option;
			break;
		case WILL:
			verb = verb_will;
			m_nState = state_option;
			break;
		case WONT:
			verb = verb_wont;
			m_nState = state_option;
			break;
		case DO:
			verb = verb_do;
			m_nState = state_option;
			break;
		case DONT:
			verb = verb_dont;
			m_nState = state_option;
		break;
		}
		break;
	case state_option:
		m_nState = state_data;
		//Find the option entry
		for(int i = 0;	ol[i].option != TOPT_ERROR && ol[i].option != code;	i++);
		//Do some verb specific stuff
		if(verb == verb_sb)
			DataProc = ol[i].DataProc;
		else
			ol[i].OptionProc(server,verb,(_option)code);
		break;
	}
}

void SVUnixOpMonitor::SetLoginPrompt(const char * pszLoginPrompt)
{
    if(! pszLoginPrompt)
        return;
	if(m_pszLoginPrompt != NULL)
		delete []m_pszLoginPrompt;
	m_pszLoginPrompt = new char[strlen(pszLoginPrompt) + 1];
    if(m_pszLoginPrompt)
    {
		strcpy(m_pszLoginPrompt , pszLoginPrompt);
		char * prompts;
		//prompts =LoginPrompt.GetBuffer(LoginPrompt.GetLength());
		prompts = m_pszLoginPrompt;
		int		c = 0;
		char	*ca = prompts, *cb = NULL;
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
}


int SVUnixOpMonitor::telnet_command()
{
	int		n = 0;
	char	buffer[4096] = {0};
	char	*scan = NULL;
	char	*ca = NULL, *cb = NULL;

	sprintf(buffer, "echo %s;%s;echo %s\r\n", C_STA, m_pszCmd, C_END);

	send(hSocket, buffer, static_cast<int>(strlen(buffer)), 0);

	memset(pgd->databuffer, 0, TELNET_BUF_SIZE);
	pgd->datalen = 0;
	while(RESPONSE_WAIT(hSocket, READ_TIMEOUT))
	{
		n = recv(hSocket, buffer, 4095, 0);
		if(n == SOCKET_ERROR) return TELNETCOMMANDERROR;
		if(n == 0) return TELNETCOMMANDERROR;
		buffer[n] = 0;

		scan = buffer;
		while(n--) 
            telnet_protocol(hSocket, *scan++,pgd);
	
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

	pgd->datalen = static_cast<int>(strlen(pgd->databuffer));
	return TELNETSUCCESS;
}

int SVUnixOpMonitor::SSHCMD(const char *strCmd)
{
	SetCmd(strCmd);
	return SSH();	
}

int SVUnixOpMonitor::SSH()
{
    char szCmdLine[512] = {0};
	sprintf(szCmdLine, "%s \"%s\" \"-ssh\" \"-P\" \"%d\" \"-pw\" \"%s\" \"-l\" \"%s\" \"%s\"  ", PLINK_PATH, 
		m_pszRemoteHost, m_nPort, m_pszPwd, m_pszUser, m_pszCmd);

//	WriteLogFile(strCmdLine.GetBuffer(strCmdLine.GetLength()));
	int nResult = executeExeFile(szCmdLine);
	return nResult;
}

int SVUnixOpMonitor::executeExeFile(const string &strCmdLine)
{
	string szContent;
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

		if (CreateProcess(NULL, const_cast<char*>(strCmdLine.c_str()), NULL, NULL, TRUE, /*CREATE_NEW_CONSOLE*/CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
			char buffer[TELNET_BUF_SIZE] = {0};

			DWORD bytesRead = 0;

			while(1)
			{
				int index = 0;
				if(ReadFile(hOutputRead, buffer, sizeof(buffer), &bytesRead, NULL))
				{
					buffer[bytesRead] = 0;
					szContent += buffer;
					index = static_cast<int>(szContent.find("DragonFlow: ", 0));
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

	
	int nIndex = 0;
	nIndex = static_cast<int>(szContent.find("DragonFlow: OK", 0));
	if(nIndex >= 0)
	{
		szContent = szContent.substr(0, nIndex);
		strcpy(pgd->databuffer, szContent.c_str());
		pgd->datalen = static_cast<int>(szContent.length());
		return TELNETSUCCESS;
	}

	nIndex = static_cast<int>(szContent.find("DragonFlow: Timeout"));
    if(nIndex >= 0)
		return SiteView_ECC_Timeout;

	nIndex = static_cast<int>(szContent.find("DragonFlow: CreateOutputThread"));
    if(nIndex >= 0)
		return SiteView_ECC_CreateOutputThread;

	nIndex = static_cast<int>(szContent.find("DragonFlow: CreateErrorOutputThread"));
    if(nIndex >= 0)
		return SiteView_ECC_CreateErrorOutputThread;
	
    nIndex = static_cast<int>(szContent.find("DragonFlow: CreateInputThread"));
    if(nIndex >= 0)
		return SiteView_ECC_CreateInputThread;

	nIndex = static_cast<int>(szContent.find("DragonFlow: WriteToStandardOutput"));
    if(nIndex >= 0)
		return SiteView_ECC_WriteToStandardOutput;

	nIndex = static_cast<int>(szContent.find("DragonFlow: OpenConnection"));
    if(nIndex >= 0)
		return SiteView_ECC_OpenConnection;
	
    nIndex = static_cast<int>(szContent.find("DragonFlow: OpenCommandFile"));
    if(nIndex >= 0)
		return SiteView_ECC_OpenCommandFile;

	nIndex = static_cast<int>(szContent.find("DragonFlow: NotEnoughSpace"));
    if(nIndex >= 0)
		return SiteView_ECC_NotEnoughSpace;

	nIndex = static_cast<int>(szContent.find("DragonFlow: InitialiseWinSock"));
    if(nIndex >= 0)
		return SiteView_ECC_InitialiseWinSock;
	
    nIndex = static_cast<int>(szContent.find("DragonFlow: WinSockVersionIncompatible"));
    if(nIndex >= 0)
		return SiteView_ECC_WinSockVersionIncompatible;

	nIndex = static_cast<int>(szContent.find("DragonFlow: Fatal Error"));
    if(nIndex >= 0)
		return SiteView_ECC_FatalError;

	nIndex = static_cast<int>(szContent.find("DragonFlow"));
    if(nIndex >= 0)
	{
		int nErrIn = static_cast<int>(szContent.find("DragonFlow"));
		int nEnd   = static_cast<int>(szContent.find("\r", nErrIn));
		szContent = szContent.substr(nErrIn);
		szContent = szContent.substr(0, nEnd - nErrIn);		
		return 12;
	}
	else
		return SiteView_ECC_UnknownError;
}


