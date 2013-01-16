#include "TelnetCisco.h"
#include "fstream"
#pragma comment(lib,"Ws2_32.lib")
using namespace std;

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



void WriteLog( const char* str );
bool WriteLog(CString strFileName,const CString strLog);

bool SaveFile(const char* strFileName,const char* strLog)
{
	char szFileName[256]={0};
	char szFileDirectory[MAX_PATH]={0};
	//sprintf(szFileName,"%s.txt",strFileName);
	HANDLE hFile;
	hFile = CreateFile(strFileName,GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ ,NULL,CREATE_ALWAYS, 0,NULL);
	if (hFile)
	{
		DWORD dw;
		BOOL bRet(FALSE);
		bRet =  WriteFile(hFile,strLog,strlen(strLog),&dw, NULL);
		if (bRet)
		{
			cout << "strFileName:" << strFileName<<endl;
			cout << "Write Success"<<endl;
		}
		
		CloseHandle(hFile);
	}
	else
	{
		return false;
	}

	return true;
}


void telnet_protocol(SOCKET server,unsigned char code,LPGDATA pgd)
{
	//cout << "--------------- telnet_protocol start ---------------"<<endl;
	//cout <<"code =" << code <<endl;
	//cout << pgd->term_index <<endl;
	//These vars are the finite state
	static int state = state_data;
	static _verb verb = verb_sb;
	static LPDATAPROC DataProc = terminal[(pgd->term_index==NUM_TERMINALS)?(NUM_TERMINALS-1):pgd->term_index].termproc;
	//cout << code <<endl;
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

	CString strLog="";
	CString strFile ="";
	strFile.Format("Cisco_%d",sockfd);
	
	//WriteLog(strFile,"====================== connect_timeo start ===================");
	ioctlsocket(sockfd,FIONBIO,&val);

	error = 0;
	if((n = connect(sockfd,saptr,salen)) < 0)
	{
		int nErrNum = WSAGetLastError();
		cout <<"nErrNum = " <<nErrNum <<endl;
		if(nErrNum !=  10035)
		{	
			shutdown(sockfd, 0x02);
			closesocket(sockfd);
			strLog.Format("connect_timeo connect failed! nErrNum=%d",nErrNum);
			WriteLog(strFile,strLog);
			return -1;
		}
	}	
	
	//cout<<"n = " << n <<endl;
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
	
	//cout << "n = "<<n<<endl;

	if(FD_ISSET(sockfd,&rset) || FD_ISSET(sockfd,&wset))
	{
		len = sizeof(error);
		if(getsockopt(sockfd,SOL_SOCKET,SO_ERROR,(char *)&error,&len) < 0)
		{
			shutdown(sockfd, 0x02);
			closesocket(sockfd);
			strLog.Format("getsockopt is failed! error=%d",error);
			WriteLog(strFile,strLog);
			return -1;
		}
		//cout << error <<endl;
	}			
	else
	{
		//printf("select error: sockfd not set");
		shutdown(sockfd, 0x02);
		closesocket(sockfd);
		strLog.Format("select error: sockfd not set");
		WriteLog(strFile,strLog);
		return -1;
	}		

done:
	val = 0;
	ioctlsocket(sockfd,FIONBIO,&val);
	if(error)
	{
		strLog.Format("error=%d",error);
		WriteLog(strFile,strLog);
		shutdown(sockfd, 0x02);
		closesocket(sockfd);
		WSASetLastError(error);
		strLog.Format("WSASetLastError! error=%d",error);
		WriteLog(strFile,strLog);
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
	return (select(1, &readfds, NULL, NULL, &Timeout));

/*	struct timeval tv;
	fd_set rset;

	FD_ZERO(&rset);
	FD_SET(s,&rset);
	tv.tv_sec = nsec;
	tv.tv_usec = 0;

	return select(s + 1,&rset,NULL,NULL, &tv);*/
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

int TelnetInit(const char *server, int port)
{
	unsigned long	ip;
	hostent			*ent;
	sockaddr_in		sa;
	SOCKET			hSocket;
	int count = 0;
	//char* server="192.168.0.251";
	ent = gethostbyname(server);
	if(ent == NULL) 
		return -1;
	ip = *(unsigned long*)(ent->h_addr);

	sa.sin_family = AF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr = *(in_addr*)&ip;

	hSocket = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);
	//cout << "hSocket = " << hSocket <<endl;
	if (hSocket == INVALID_SOCKET) 
		return -2;
	if(connect_timeo(hSocket, (sockaddr*)&sa, sizeof(sockaddr), 9) < 0)
	{	
		return -3;
	}
	cout << "------------ connect_timeo --------------"<<endl;
	return hSocket;
}

void WriteLog( const char* str );


int TelnetConnect(SOCKET hSocket, const char* szPwd,const char* pwdPrompt,const char* cmdPrompt, LPGDATA pgd )
{
	int		n = 0;
	char	buffer[4096] = {0};
	char	*scan = NULL;
	int		count = 0;
	int		telnetprogress = 0;
	memset(pgd->databuffer, 0, BUF_SIZE);
	pgd->datalen = 0;
	int nTime=0;
	
	while(1)
	{
LOOP:
		memset(buffer, 0, 4096);
		if (RESPONSE_WAIT(hSocket, READ_TIMEOUT) <= 0)
		{
			if ((telnetprogress ==1) && nTime < 3)
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
		if (n == SOCKET_ERROR)
		{
			return -2; //SOCKET_ERROR
		}

		buffer[n] = 0;
		scan = buffer;
		while(n--)telnet_protocol(hSocket, *scan++,pgd);

		switch (telnetprogress)
		{
		case 0:
			if (pwdPrompt)
			{
				if (strstr(pgd->databuffer,pwdPrompt))//确认登陆提示符，发送密码
				{ 
					char	bufPwd[255] = {0};
					int nLen = strlen(szPwd);
					if (nLen == 0)
					{
						return -7; //登录失败：密码为空
					}
					strcpy(bufPwd, szPwd);
					strcat(bufPwd,"\r\n");
					send(hSocket, bufPwd, strlen(bufPwd), 0);
					send(hSocket,"\r\n",2,0);
					nTime = 0;
					count = 0;
					memset(pgd->databuffer, 0, BUF_SIZE);
					pgd->datalen = 0;
					telnetprogress ++;
					count = 0;
				}

			}
			else
			{
				return -3; //登陆失败：登陆提示符为空
			}
			break;
		case 1:
			//if (strstr(pgd->databuffer, pwdPrompt ))
			//{
			//	memset(pgd->databuffer, 0, BUF_SIZE);
			//	pgd->datalen = 0;
			//	return -7; //登陆失败：密码错误
			//}
			if (cmdPrompt)
			{
				if (strstr(pgd->databuffer,cmdPrompt))//确认命令提示符
				{ 
					char	bufPwd[255] = {0};
					nTime = 0;
					count = 0;
					telnetprogress ++;
					memset(pgd->databuffer, 0, BUF_SIZE);
					pgd->datalen = 0;
					return 0;
				}
			}
			else
			{
				return -4; //登陆失败：命令提示符为空
			}
			break;

			if (++count > 80)
			{
				if (telnetprogress == 0)
				{
					return -5; //登陆失败：查找登陆提示符
				}
				else if (telnetprogress == 1)
				{
					return -6; //登陆失败：查找命令提示符
				}
			}
		}

	}



	return 0;
}

int TelnetCommand(SOCKET hSocket, const char* szCMD, LPGDATA pgd)
{
	int		n = 0;
	char	buffer[4096] = {0};
	char	*scan = NULL;
	int		nTime =0;

	sprintf(buffer,"%s\r\n", szCMD);
	send(hSocket,buffer, strlen(buffer),0);
LOOP:
	memset(pgd->databuffer, 0, BUF_SIZE);
	pgd->datalen = 0;
	memset(buffer,0,4096);
	if(RESPONSE_WAIT(hSocket, READ_TIMEOUT) <= 0)
	{
		if(nTime<3)
		{
			char buf[10]={0};
			sprintf(buf,"\r\n");
			send(hSocket, buf, strlen(buf),0);
			nTime++;
			Sleep(1000);
			goto LOOP;
		}

		return -1;
	} 
	Sleep(1000);
	n = recv(hSocket, buffer, 4095, 0);


	if(n == SOCKET_ERROR)
	{
		int wserror;
		wserror=WSAGetLastError();
		return -2;
	}
	else if (n == 0)
	{
		return -3;
	}
	buffer[n] = 0;
	scan = buffer;
	while(n--)
		telnet_protocol(hSocket, *scan++,pgd);
	/*while(RESPONSE_WAIT(hSocket, READ_TIMEOUT))
	{
		memset(buffer, 0 , 4096);
		Sleep(1000);
		n = recv(hSocket, buffer, 4095, 0);
		if(n == SOCKET_ERROR) 
			return -2;
		buffer[n] = 0;
		scan = buffer;
		while(n--) telnet_protocol(hSocket, *scan++,pgd);

		if (strstr(pgd->databuffer, "--More--"))
		{
			return strlen(pgd->databuffer);
		}
	}*/
	return 0;
}


int TelnetProcess(SOCKET hSocket,const char* szPwd,const char* pwdPrompt,const char* szCMD, const char* cmdPrompt, LPGDATA pgd)
{
	int		n = 0;
	char	buffer[4096] = {0};
	char	*scan = NULL;
	int		count = 0;
	memset(pgd->databuffer, 0, BUF_SIZE);
	pgd->datalen = 0;
	int telnetprogress=0;
	int nTime=0;

	CString strFile = szCMD;
	char* p = strFile.GetBuffer();
	int len = strFile.GetLength();
	for (int i=0; i< len; i++)
	{
		if (*p == '/')
		{
			*p ='_';
		}
		p ++;
	}

	cout << "--------------- TelnetProcess Start ----------------"<<endl;

	cout << "CMD:"<<szCMD<<endl;
//	while(1)
//	{
//LOOP:
//		memset(buffer, 0, 4096);
//		if (RESPONSE_WAIT(hSocket, READ_TIMEOUT) <= 0)
//		{
//			if ((telnetprogress ==1) && nTime < 3)
//			{
//				char buf[10]={0};
//				sprintf(buf,"\r\n");
//				send(hSocket, buf, strlen(buf),0);
//				nTime++;
//				goto LOOP;
//			}
//			return -1;
//		}
//		Sleep(500);
//		n = recv(hSocket, buffer, 4095, 0);
//		if (n == SOCKET_ERROR)
//		{
//			return -2; //SOCKET_ERROR
//		}
//
//		buffer[n] = 0;
//		scan = buffer;
//		while(n--)telnet_protocol(hSocket, *scan++,pgd);
//
//		cout << "pgd:"<<pgd->databuffer <<endl;
//
//		switch (telnetprogress)
//		{
//		case 0:
//			if (pwdPrompt)
//			{
//				if (strstr(pgd->databuffer,pwdPrompt))//确认登陆提示符，发送密码
//				{ 
//					char	bufPwd[255] = {0};
//
//					int nLen = strlen(szPwd);
//					if (nLen == 0)
//					{
//						return -7; //登录失败：密码为空
//					}
//					strcpy(bufPwd, szPwd);
//					strcat(bufPwd,"\r\n");
//					send(hSocket, bufPwd, strlen(bufPwd), 0);
//					//send(hSocket,"\r\n",2,0);
//					nTime = 0;
//					count = 0;
//					memset(pgd->databuffer, 0, BUF_SIZE);
//					pgd->datalen = 0;
//					telnetprogress ++;
//					count = 0;
//					cout << "密码："<<szPwd<<endl;
//					cout << "确认登陆提示符成功，发送密码"<<endl;
//				}
//
//			}
//			else
//			{
//				return -3; //登陆失败：登陆提示符为空
//			}
//			break;
//		case 1:
//			//if (strstr(pgd->databuffer, pwdPrompt ))
//			//{
//			//	memset(pgd->databuffer, 0, BUF_SIZE);
//			//	pgd->datalen = 0;
//			//	return -7; //登陆失败：密码错误
//			//}
//			if (cmdPrompt)
//			{
//				if (strstr(pgd->databuffer,cmdPrompt))//确认命令提示符
//				{ 
//					char	bufPwd[255] = {0};
//					nTime = 0;
//					count = 0;
//					telnetprogress ++;
//					cout << "确认命令提示符成功"<<endl;
//					memset(pgd->databuffer, 0, BUF_SIZE);
//					pgd->datalen = 0;
//					goto CMD;
//				}
//			}
//			else
//			{
//				return -4; //登陆失败：命令提示符为空
//			}
//			break;
//
//			if (++count > 80)
//			{
//				if (telnetprogress == 0)
//				{
//					return -5; //登陆失败：查找登陆提示符
//				}
//				else if (telnetprogress == 1)
//				{
//					return -6; //登陆失败：查找命令提示符
//				}
//			}
//		}
//
//	}
//
//CMD:
//	char bufCMD[1024] = {0};
//	sprintf(buffer,"%s\r\n", szCMD);
//	send(hSocket, buffer, strlen(buffer),0);
//	cout<< "buffer"<<buffer <<endl;
//	/*strcpy(bufCMD, szCMD);
//	for(int nS=0;nS<strlen(bufCMD);nS++)
//	{
//		Sleep(20);
//		send(hSocket, &bufCMD[nS], 1,0);	
//	}
//	send(hSocket,"\r\n",2,0);*/
//	memset(pgd->databuffer, 0, BUF_SIZE);
//	pgd->datalen = 0;
//	
//
//
//	while(RESPONSE_WAIT(hSocket, READ_TIMEOUT))
//	{
//		cout << "开始接收数据"<<endl;
//		Sleep(2000);
//		memset( buffer,0,4096);
//		n = recv(hSocket,buffer, 4095, 0);
//		if (n=SOCKET_ERROR)
//		{
//			cout << "SOCKET_ERROR" <<endl;
//			return -2;
//		}
//		if (n == 0)
//		{
//			cout << "没有数据" <<endl;
//			return -2;
//		}
//		buffer[n] = 0;
//		scan = buffer;
//
//		while(n--) 
//			telnet_protocol(hSocket, *scan++,pgd);
//		if (strstr(pgd->databuffer,cmdPrompt))
//		{
//			cout << "pgd->databuffer:"<<pgd->databuffer <<endl;
//			return 0;
//		}
//		send(hSocket,"\r\n",2,0);
//
//	}

	//return 0;




	//接受登陆提示
LOOP_LOGIN: //等待登陆
	memset(buffer, 0, 4096);
	if(RESPONSE_WAIT(hSocket, READ_TIMEOUT) <= 0)
	{
		//cout << "------------------- RESPONSE_WAIT Start ----------------"<<endl;

		if(nTime<3)
		{
			char buf[10]={0};
			sprintf(buf,"\r\n");
			send(hSocket, buf, strlen(buf),0);
			nTime++;
			goto LOOP_LOGIN;
		}
		
		return -1; //等待超时
	} 
	Sleep(1500);
	n = recv(hSocket, buffer, 4095, 0);
	
	if(n == SOCKET_ERROR)
	{
		int wserror;
		wserror=WSAGetLastError();
		//CString stmp;
		//stmp.Format("telnet connect failed --error code =%d",wserror);
		//WriteLogFile1(stmp);
		return -2; //SOCKET_ERROR
	}
	else if (n==0)
	{
		WriteLog(strFile, "LOOP_LOGIN 过程关闭" );
		return -8;
	}
	buffer[n] = 0;
	scan = buffer;
	//cout << "buffer len = "<< n <<endl;
	//cout << buffer <<endl;
	while(n--)telnet_protocol(hSocket, *scan++,pgd);

	//cout<< pgd->databuffer <<endl;

	//确认登陆提示数据
	if(pwdPrompt)
	{
		if (strstr(pgd->databuffer,pwdPrompt))//确认登陆提示符，发送密码
		{ 
			char	bufPwd[255] = {0};
			int nLen = strlen(szPwd);
			if (nLen == 0)
			{
				return -7;
			}

			strcpy(bufPwd, szPwd);
			strcat(bufPwd,"\r\n");
			send(hSocket, bufPwd, strlen(bufPwd), 0);
			/*for(int nS=0;nS<strlen(bufUser);nS++)
			{
			Sleep(20);
			send(hSocket, &bufUser[nS], 1,0);	
			}*/
			send(hSocket,"\r\n",2,0);
			nTime = 0;
			count = 0;
			memset(pgd->databuffer, 0, BUF_SIZE);
			pgd->datalen = 0;

		}
		else
		{
			WriteLog(pgd->databuffer);
			//WriteLog(buffer);
			return -3; //登陆提示符确认失败
		}
	}
	else
	{
		//WriteLogFile("Read Data Failed from Server(1)");
		return -4; //登陆提示符为空
	}

	//接收命令提示
LOOP_PWD: //发送 密码成功，等待接收数据
	memset(buffer, 0, 4096);
	if(RESPONSE_WAIT(hSocket, READ_TIMEOUT) <= 0)
	{
		if(nTime<3)
		{
			char buf[10]={0};
			sprintf(buf,"\r\n");
			send(hSocket, buf, strlen(buf),0);
			nTime++;
			goto LOOP_PWD;
		}

		return -1;
	} 
	Sleep(500);
	n = recv(hSocket, buffer, 4095, 0);
	
	if(n == SOCKET_ERROR)
	{
		int wserror;
		wserror=WSAGetLastError();
		//CString stmp;
		//stmp.Format("telnet connect failed --error code =%d",wserror);
		//WriteLogFile1(stmp);
		return -2;
	}
	else if (n==0)
	{
		WriteLog(strFile, "LOOP_PWD 过程关闭" );
		return -8;
	}
	buffer[n] = 0;
	scan = buffer;
	memset(pgd->databuffer, 0, BUF_SIZE);
	while(n--)telnet_protocol(hSocket, *scan++,pgd);
	//cout<< pgd->databuffer <<endl;

	//确认命令提示数据
	if(cmdPrompt)
	{
		if (strstr(pgd->databuffer,cmdPrompt))//确认命令提示符，发送查询。
		{
			//char	szCMD[255] = "show int FastEthernet0/14";
			char  bufCMD[1024] ={0};
			strcpy(bufCMD, szCMD);
			for(int nS=0;nS<strlen(bufCMD);nS++)
			{
				Sleep(20);
				send(hSocket, &bufCMD[nS], 1,0);	
			}
			send(hSocket,"\r\n",2,0);
			nTime = 0;
			count = 0;
			memset(pgd->databuffer, 0, BUF_SIZE);
			pgd->datalen = 0;
		}
		else
		{
			return -5;//命令提示符确认失败
		}
	}
	else
	{
		//WriteLogFile("Read Data Failed from Server(1)");
		return -6; //命令提示符为空
	}

LOOP_CMD: //发送查询成功， 等待接收数据 
	memset(buffer, 0, 4096);
	if(RESPONSE_WAIT(hSocket, READ_TIMEOUT) <= 0)
	{
		if(nTime<3)
		{
			char buf[10]={0};
			sprintf(buf,"\r\n");
			send(hSocket, buf, strlen(buf),0);
			nTime++;
			goto LOOP_CMD;
		}

		return -1;
	} 
	Sleep(2000);
	n = recv(hSocket, buffer, 4095, 0);

	if(n == SOCKET_ERROR)
	{
		int wserror;
		wserror=WSAGetLastError();
		return -2;
	}
	else if (n == 0)
	{
		WriteLog(strFile, "LOOP_CMD 过程关闭" );
		return -8;
	}
	buffer[n] = 0;
	scan = buffer;
	//	cout << buffer << endl;
	memset(pgd->databuffer, 0, BUF_SIZE);
	while(n--)
		telnet_protocol(hSocket, *scan++,pgd);
	//cout<< pgd->databuffer <<endl;
	//cout << pgd->databuffer << endl;
	
	cout << "--------------- TelnetProcess End ----------------"<<endl;
	
	return 0;
}



//bool MonitorParse(char* databuf, char* szCMD , CStringList& pList, char* szReturn  )
//{
//	CString strData = databuf;
//	CString strFind="";
//	CString strVal = "";
//	int nPos =-1, nPrePos =-1;
//
//	if (strstr(databuf, szCMD))
//	{
//		strFind.Format("%s is",szCMD);
//		nPos = strData.Find(strFind,0);
//		if (nPos == -1) //接口状态不存在
//		{
//			return false;
//		}
//		nPrePos = nPos + strFind.GetLength();
//		nPos = strData.Find(",",nPrePos);
//		strVal = strData.Mid(nPrePos, nPos - nPrePos);
//		strVal.TrimLeft();
//		strVal.TrimRight();
//		pList.AddTail(strVal);
//
//		strFind.Format("line protocol is");//连接状态
//		nPos = strData.Find(strFind, nPos);
//		if(nPos == -1)
//		{
//			return false;
//		}
//		nPrePos = nPos + strFind.GetLength();
//		nPos = strData.Find("\r\n",nPrePos);
//		strVal = strData.Mid(nPrePos, nPos - nPrePos);
//		strVal.TrimLeft();
//		strVal.TrimRight();
//		pList.AddTail(strVal);
//
//		strFind.Format("packets input,");//接收字节
//		nPos = strData.Find(strFind,nPos);
//		if(nPos == -1)
//		{
//			return false;
//		}
//		nPrePos = nPos + strFind.GetLength();
//		nPos = strData.Find("bytes",nPrePos);
//		strVal = strData.Mid(nPrePos, nPos - nPrePos);
//		strVal.TrimLeft();
//		strVal.TrimRight();
//		pList.AddTail(strVal);
//
//		strFind.Format("packets output,");//发送字节
//		nPos = strData.Find(strFind,nPos);
//		if(nPos == -1)
//		{
//			return false;
//		}
//		nPrePos = nPos + strFind.GetLength();
//		nPos = strData.Find("bytes",nPrePos);
//		strVal = strData.Mid(nPrePos, nPos - nPrePos);
//		strVal.TrimLeft();
//		strVal.TrimRight();
//		pList.AddTail(strVal);
//
//	}
//	else
//	{
//		// sprintf(szReturn,"error");
//		return false;
//	}
//	return true;
//}

//
//

int TelnetProcessNew(SOCKET hSocket,const char* szPwd,const char* pwdPrompt,const char* szCMD, const char* cmdPrompt, const char* szFileName ,LPGDATA pgd)
{
	int		n = 0;
	char	buffer[4096] = {0};
	char	*scan = NULL;
	int		count = 0;
	memset(pgd->databuffer, 0, BUF_SIZE);
	pgd->datalen = 0;
	int nTime=0;

	//接受登陆提示
LOOP_LOGIN: //等待登陆
	memset(buffer, 0, 4096);
	if(RESPONSE_WAIT(hSocket, READ_TIMEOUT) <= 0)
	{
		if(nTime<3)
		{
			char buf[10]={0};
			sprintf(buf,"\r\n");
			send(hSocket, buf, strlen(buf),0);
			nTime++;
			goto LOOP_LOGIN;
		}
		return -1; //等待超时
	} 
	Sleep(500);
	n = recv(hSocket, buffer, 4095, 0);
	if(n == SOCKET_ERROR)
	{
		int wserror;
		wserror=WSAGetLastError();
		//CString stmp;
		//stmp.Format("telnet connect failed --error code =%d",wserror);
		//WriteLogFile1(stmp);
		return -2; //SOCKET_ERROR
	}
	buffer[n] = 0;
	scan = buffer;
	while(n--)telnet_protocol(hSocket, *scan++,pgd);

	//	cout<< pgd->databuffer <<endl;

	//确认登陆提示数据
	if(pwdPrompt)
	{
		if (strstr(pgd->databuffer,pwdPrompt))//确认登陆提示符，发送密码
		{ 
			char	bufPwd[255] = {0};
			strcpy(bufPwd, szPwd);
			strcat(bufPwd,"\r\n");
			send(hSocket, bufPwd, strlen(bufPwd), 0);
			/*for(int nS=0;nS<strlen(bufUser);nS++)
			{
			Sleep(20);
			send(hSocket, &bufUser[nS], 1,0);	
			}*/
			send(hSocket,"\r\n",2,0);
			nTime = 0;
			count = 0;
			memset(pgd->databuffer, 0, BUF_SIZE);
			pgd->datalen = 0;

		}
		else
		{
			return -3; //登陆提示符确认失败
		}
	}
	else
	{
		//WriteLogFile("Read Data Failed from Server(1)");
		return -4; //登陆提示符为空
	}

	//接收命令提示
LOOP_PWD: //发送 密码成功，等待接收数据
	memset(buffer, 0, 4096);
	if(RESPONSE_WAIT(hSocket, READ_TIMEOUT) <= 0)
	{
		if(nTime<3)
		{
			char buf[10]={0};
			sprintf(buf,"\r\n");
			send(hSocket, buf, strlen(buf),0);
			nTime++;
			goto LOOP_PWD;
		}

		return -1;
	} 
	Sleep(500);
	n = recv(hSocket, buffer, 4095, 0);

	if(n == SOCKET_ERROR)
	{
		int wserror;
		wserror=WSAGetLastError();
		//CString stmp;
		//stmp.Format("telnet connect failed --error code =%d",wserror);
		//WriteLogFile1(stmp);
		return -2;
	}
	buffer[n] = 0;
	scan = buffer;
	while(n--)telnet_protocol(hSocket, *scan++,pgd);

	//确认命令提示数据
	if(cmdPrompt)
	{
		if (strstr(pgd->databuffer,cmdPrompt))//确认命令提示符，发送查询。
		{
			//char	szCMD[255] = "show int FastEthernet0/14";
			char  bufCMD[1024] ={0};
			strcpy(bufCMD, szCMD);
			for(int nS=0;nS<strlen(bufCMD);nS++)
			{
				Sleep(20);
				send(hSocket, &bufCMD[nS], 1,0);	
			}
			send(hSocket,"\r\n",2,0);
			nTime = 0;
			count = 0;
			memset(pgd->databuffer, 0, BUF_SIZE);
			pgd->datalen = 0;
		}
		else
		{
			return -5;//命令提示符确认失败
		}
	}
	else
	{
		//WriteLogFile("Read Data Failed from Server(1)");
		return -6; //命令提示符为空
	}

LOOP_CMD: //发送查询成功， 等待接收数据 
	memset(buffer, 0, 4096);
	if(RESPONSE_WAIT(hSocket, READ_TIMEOUT) <= 0)
	{
		if(nTime<3)
		{
			char buf[10]={0};
			sprintf(buf,"\r\n");
			send(hSocket, buf, strlen(buf),0);
			nTime++;
			goto LOOP_CMD;
		}

		return -1;
	} 
	Sleep(1000);
	n = recv(hSocket, buffer, 4095, 0);

	if(n == SOCKET_ERROR)
	{
		int wserror;
		wserror=WSAGetLastError();
		return -2;
	}
	buffer[n] = 0;
	scan = buffer;
	while(n--)
		telnet_protocol(hSocket, *scan++,pgd);
	//	cout << pgd->databuffer << endl;
	CString strAll = "";
	strAll += pgd->databuffer;
	char szTemp[10]={0};
	sprintf(szTemp,"\r\n");
	send(hSocket, szTemp, strlen(szTemp),0);

	while(RESPONSE_WAIT(hSocket, READ_TIMEOUT))
	{
		memset(buffer, 0 , 4096);
		memset(pgd->databuffer,0, BUF_SIZE);
		pgd->datalen = 0;		
		Sleep(10);
		n = recv(hSocket, buffer, 4095, 0);
		if(n == SOCKET_ERROR) 
			return -2;
		buffer[n] = 0;
		scan = buffer;
		while(n--) 
			telnet_protocol(hSocket, *scan++,pgd);
		strAll += pgd->databuffer;
		if (strstr(pgd->databuffer,cmdPrompt))
		{
			SaveFile(szFileName,strAll);

			return 0;
		}
		send(hSocket, szTemp, strlen(szTemp),0);



	}


	return 0;

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
		fprintf(ffile1, "%s\n", str);
		fclose(ffile1);
	}	
	return 0;
}






