// TongLineStatus.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"


#include <fstream>
#include <list>
#include <map>
using namespace std;

typedef std::list<string> StringList;
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



bool WriteLog(char* strFileName,const char* strLog)
{
	return false;
	char szDataBuf[128]		= _T("");
	char szTimeBuf[128]		= _T("");
	char szTempBuf[1024*10] = _T("");
	//_strdate(szDataBuf);
	//_strtime(szTimeBuf);
	//sprintf(szTempBuf,"%s-%s",szDataBuf,szTimeBuf);

	char szFileName[256]={0};
	char szFileDirectory[MAX_PATH]={};
	//GetCurrentDirectory(MAX_PATH, szFileDirectory);
	//szFileName.Format("%s\\TestLog",szFileDirectory);
	//CreateDirectory(szFileName,NULL);
	//szFileName.Format("%s\\%s.log",szFileName,strFileName);
	//szFileName.Format("%s.log",strFileName);
	sprintf(szFileName,"%s.log",strFileName);

	ofstream stFileStream;
	stFileStream.open(szFileName, ios::app);
	stFileStream<<strLog<<endl;
	stFileStream.close();
	return true;
}


bool MakeStringListByChar(CStringList& pList, const char * pInput )
{
	const char * p;
	int nSize;
	p=pInput;
	while(*p!='\0')
	{
		nSize = static_cast<int>(strlen(p));
		if( nSize>0 )
		{	
			pList.AddHead(p);
			//	printf(p);
		}
		p=p+nSize+1;
	}

	return true;
}
bool MakeCharByString(char *pOut,int &nOutSize,CString strInput )
{
	char *p;

	int nSize=strInput.GetLength();
	if(nSize+2 <nOutSize)
	{
		strcpy(pOut,strInput.GetBuffer(strInput.GetLength()));
	}
	else 
		return false;
	p=pOut;
	//printf("%d\n",nSize);23028830 13602067678 王波
	for(int i=0;i<nSize;i++)
	{
		if(*p=='$') 	
			*p='\0';
		p++;
	}
	nOutSize=nSize+1;
	return true;

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

int WSA_Init()
{
	//return 0;
	WORD wVersionRequested;  
	WSADATA wsaData; 
	int err; 
	wVersionRequested = MAKEWORD(1, 1); 

	err = WSAStartup(wVersionRequested, &wsaData); 

	if (err != 0) 
		return -1; 

	if ( LOBYTE( wsaData.wVersion ) != 1 || 
		HIBYTE( wsaData.wVersion ) != 1 ) 
	{ 
		WSACleanup(); 
		return -2;
	}

	return 0;
}

void WSA_Free()
{
	//return;
	WSACleanup();
}


int TelnetInit(char *server, int port)
{
	unsigned long	ip;
	hostent			*ent;
	sockaddr_in		sa;
	SOCKET			hSocket;
	//char* server="192.168.0.251";
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

	return hSocket;
}
int TelnetConnect(SOCKET hSocket, char *uid, char *pwd, 
				  char *logprompt, char *pwdprompt, char arprompt[PR_NUM][256],
				  LPGDATA pgd)
{
	//OutputDebugString("=============== telnet_connect:Login ================\n");

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

		if(n == SOCKET_ERROR)
		{
			int wserror;
			wserror=WSAGetLastError();
			CString stmp;
			stmp.Format("telnet connect failed --error code =%d",wserror);
			//WriteLogFile1(stmp);
			return -2;
		}
		buffer[n] = 0;
		scan = buffer;
		//memset(pgd->databuffer, 0, BUF_SIZE);
		//pgd->datalen = 0;
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
					memset(pgd->databuffer, 0, BUF_SIZE);
					pgd->datalen = 0;
					telnetprogress ++;
					count = 0;

				}
			}
			else
			{
				//WriteLogFile("Read Data Failed from Server(1)");
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
				//	WriteLogFile("Read Data Failed from Server(2)");
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
			while(i < PR_NUM && *arprompt[i])
			{
				memset(prompt, 0, sizeof(prompt));
				strcpy(prompt, arprompt[i]);
				if(strstr(pgd->databuffer, prompt))
				{
					cout << pgd->databuffer <<endl;
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
		//改变默认的telnet行的长度
		//        telnet_setlinewidth(hSocket, 0x00, 0xef);
	}
}

int TelnetCommand(SOCKET hSocket, char *cmd,LPGDATA pgd)
{
	int		n = 0;
	char	buffer[4096] = {0};
	char	*scan = NULL;
	char	*ca = NULL, *cb = NULL;

	sprintf(buffer, "echo %s;%s;echo %s\r\n", C_STA, cmd, C_END);
	//sprintf(buffer,"%s",cmd);

	
	if(strlen(buffer) >= 4096)
	{
		char chMsg[256];
		sprintf(chMsg, "Command length is:%d", strlen(buffer));
		cout << chMsg <<endl;
	}
	send(hSocket, buffer, strlen(buffer), 0);

	memset(pgd->databuffer, 0, BUF_SIZE);
	pgd->datalen = 0;
	//int n=0;
	CString strLog="";
	char szKill[32]={0};
	while(RESPONSE_WAIT(hSocket, READ_TIMEOUT))
	{
		memset(buffer, 0 , 4096);
		Sleep(8000);
		n = recv(hSocket, buffer, 4095, 0);
		if(n == SOCKET_ERROR) return -2;
		if(n == 0) return -2;
		buffer[n] = 0;
		scan = buffer;

		while(n--) telnet_protocol(hSocket, *scan++,pgd);

		char szTemp =0x7F;
		sprintf(szKill,"^C");
		send(hSocket,&szTemp,1,0);
		send (hSocket,"\r\n",2,0);


		char match_1[20] = {0}, match_2[20] = {0};
		sprintf(match_1, "\r\n%s", C_STA);
		sprintf(match_2, "%s", C_END);
		//strLog.Format("--------------------- pgd->databuffer %d ------------------------",n);
		//cout << "--------------------- pgd->databuffer ------------------------"<<endl;
		//cout << pgd->databuffer <<endl;
		//cout << "--------------------- pgd->databuffer ------------------------"<<endl; 
		if(ca = strstr(pgd->databuffer, match_1)) 
		{
			if(cb = strstr(ca, match_2))
			{
				ca += strlen(C_STA) + 4;
				//				puts(ca);
				//cout << "ca:\n" <<ca <<endl;
				//cb += 2;
				//				int a=ca-cb;
				//				printf("===========%d\n",a);
				strncpy(pgd->databuffer, ca, cb - ca);
				pgd->databuffer[cb - ca] = 0;
				break;
			}
		}
	}

	cout << pgd->databuffer <<endl;

	return strlen(pgd->databuffer);
}

bool TLQStatuParse(char* databuf, char* szReturn )
{

	CString strFind="";
	CString strData= databuf;
	int nConPos = -1;
	int nDisPos = -1;
	strFind = _T("CONNECT");
	nConPos = strData.Find(strFind, 0);

	strFind = _T("DISCONN");
	nDisPos = strData.Find(strFind,0);

	if ((nDisPos == -1) && (nConPos == -1))
	{
		return false ;
	}

	if (nConPos > nDisPos)
	{
		sprintf(szReturn, "ConnectStatu=Connect$");
	}
	else
	{
		sprintf(szReturn,"ConnectStatu=Disconn$");
	}
	int nSize = 1024;
	CString strOut = szReturn;
	MakeCharByString(szReturn,nSize,strOut);
	return true;
}

bool TelMonitor(char* strServer, int nPort, char* strUser, 
				char* strPWD, char* LogPrompt,char* PwdPrompt,
				char* strPrompt, char* strCMD, char* szReturn )
{
	
	/*************************************
	* For Reason Of Prompt
	*/
	int		c = 0;
	char	*ca = strPrompt;
	char	*cb = NULL;
	char	arprompt[PR_NUM][256];
	memset(arprompt, 0, sizeof(arprompt));
	cout << "--------- arprompt Start ---------------"<< endl;
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
	cout << "--------- arprompt End ---------------"<< endl;

	/***********************************/
	SOCKET	hSocket;
	int nRet = 0;
	bool bResult = true;
	LPGDATA pgd=new GDATA;
	int nTime = 0;
	if (pgd == NULL)
	{
		sprintf(szReturn,"error=内存分配失败");
		return false;
	}
LOOP:
	memset(pgd,0,sizeof(GDATA));
	cout << "--------- TelnetInit Start ---------------"<< endl;
	nRet = TelnetInit(strServer,nPort);
	cout <<"hSocket = " << hSocket << endl;
	cout << "--------- TelnetInit End ---------------"<< endl;
	if (nRet <= 0)
	{
		if(nRet == -1)
		{
			sprintf(szReturn , "error=连接失败：域名解析");
		}
		else if (nRet == -2)
		{
			sprintf(szReturn,"error=连接失败：通信初始化");
		}
		else if (nRet == -3)
		{
			sprintf(szReturn,"error=连接失败：连接超时");
		}
		else
		{
			sprintf(szReturn,"error=连接失败：未知错误");
		}
		bResult =false;
		goto ERR;
	}
	hSocket = nRet;
	cout << "--------- TelnetConnect Start ---------------"<< endl;
	nRet = TelnetConnect(hSocket,strUser,strPWD,LogPrompt,PwdPrompt,arprompt, pgd);	
	cout <<"nRet = " << nRet << endl;
	cout << "--------- TelnetConnect End ---------------"<< endl;
	if (nRet < 0)
	{
		if (nRet == -1)
		{
			if (nTime <3)
			{
				nTime ++;
				Sleep( 3000);
				goto LOOP;
			}
			else
			{
				sprintf(szReturn,"error=登录失败：读数据超时");
			}
		}
		else if (nRet == -2)
		{
			sprintf(szReturn,"error=登录失败：读数据错误");
		}
		else if (nRet == -3)
		{
			sprintf(szReturn,"error=登录失败：查找提示符");
		}
		else if (nRet == -4)
		{
			sprintf(szReturn,"error=登录失败：查找登录提示符");
		}
		else if (nRet == -5)
		{
			sprintf(szReturn,"error=登录失败：查找密码提示符");
		}
		else if (nRet == -6)
		{
			sprintf(szReturn,"error=登录失败：用户不存在或用户密码错误");
		}
		else
		{
			sprintf(szReturn,"error=登录失败：未知错误");
		}
		bResult = false;
		goto ERR;
	}
 
	cout << "--------- TelnetCommand Start ---------------"<< endl;

	nRet = TelnetCommand(hSocket, strCMD , pgd);
	cout << "--------- TelnetCommand Start ---------------"<< endl;
	if (nRet < 0)
	{
		sprintf(szReturn,"error=发送命令失败");
		bResult = false;
		goto ERR;
	}
	
	bResult = TLQStatuParse(pgd->databuffer, szReturn);
	if ( !bResult )
	{
		sprintf(szReturn,"error=TLQ状态错误");
	}



ERR:
	delete pgd;
	shutdown(hSocket, 0x02);
	closesocket(hSocket);
	return bResult;
}




extern "C" __declspec(dllexport) 
bool TLQConnectStatus(const char *strParas,  char *szReturn,int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString		strServer = _T(""),
				strUID = _T(""),
				strPWD = _T(""),
				strPrompt = _T("# $ >"),
				strLogPrompt = _T("ogin:"),
				strPwdPrompt = _T("assword:"),
				//strCMD = _T("/root/TLQ63/bin/tlqstat -l");
				strCMD = _T("/oracle/TLQ63/TLQ63/bin/tlqstat -l");
	int			nPort = 23; 


	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);


	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServer = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUID = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROMPT__, 0) == 0)
		{
			strPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PROMPT__));
		}
		else if(strTemp.Find(__LOGINPROMPT__, 0) == 0)
		{
			strLogPrompt = strTemp.Right(strTemp.GetLength() - strlen(__LOGINPROMPT__));
		}
		else if(strTemp.Find(__PASSWORDPROMPT__, 0) == 0)
		{
			strPwdPrompt = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORDPROMPT__));
		}
		
	}

	if(strServer.IsEmpty())
	{
		sprintf(szReturn, "error=缺少服务器");//缺少服务器"
		return false;
	}
	if(nPort == 0)
	{
		sprintf(szReturn, "error=输入端口错误");//"输入端口错误"
		return false;
	}
	if(strUID.IsEmpty())
	{
		sprintf(szReturn, "error=缺少用户名");//"缺少用户名"
		return false;
	}

	if(strLogPrompt.IsEmpty() || strPwdPrompt.IsEmpty() || strPrompt.IsEmpty())
	{
		sprintf(szReturn, "error=输入登录提示符、密码提示符或提示符");//"输入登录提示符、密码提示符或提示符"
		return false;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=通信初始化错误");//"通信初始化错误"
		return false;
	}

	strPrompt.TrimLeft();
	strPrompt.TrimRight();
	bool bRet(false);
	try
	{
		bRet = TelMonitor(	strServer.GetBuffer(), 
							nPort, 
							strUID.GetBuffer(),
							strPWD.GetBuffer(),
							strLogPrompt.GetBuffer(),
							strPwdPrompt.GetBuffer(),
							strPrompt.GetBuffer(),
							strCMD.GetBuffer(),
							szReturn );
	}
	catch(...)
	{
		sprintf( szReturn ,"error=监测器异常");
	}
	WSA_Free();
	return bRet;
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
				//wsprintf(buf,"Unknown Ansi code:'%c' (%s)\n",data,pgd->codebuf);
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

