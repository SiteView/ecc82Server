
#include "FTP.h"
#include "Utils.h"

#include "..\..\base\funcGeneral.h"

SOCKET		ctrlSocket;
SOCKET		dataSocket;

DWORD		beginTickCount = 0;
DWORD		endTickCount = 0;
DWORD		recvTickCount = 0;
DWORD		recvBytes = 0;

char		remote_host[64] = {0};
int			remote_port = 0;

char		local_host[64] = {0};
int			local_port = 0;

int			passive_mode = 0;

BOOL FTP_MONITOR(char *server, int port, char *file, char *uid, char *pwd, 
				 int timeout, int ptype, char *pserver, int pport, 
				 char *puid, char *ppwd, char *match, char *custpath, char *szReturn)
{
	ctrlSocket = socket(AF_INET, SOCK_STREAM, 0);
	dataSocket = socket(AF_INET, SOCK_STREAM, 0);

	srand((unsigned)time(NULL));

	beginTickCount = GetTickCount();
	if(OpenControlChannel(server, port, uid, pwd, timeout, file, ptype, pserver, pport, puid, ppwd, szReturn))
	{
		return FALSE;
	}

	if(OpenDataChannel(file, timeout, ptype, pserver, pport, puid, ppwd, szReturn))
	{
		return FALSE;
	}

	QuitFromServer();
	
	endTickCount = GetTickCount();

	sprintf(szReturn, "roundTripTime=%.3f$bytes=%ld$", (endTickCount - beginTickCount) / 1000.0, recvBytes);
	if(recvTickCount == 0) recvTickCount = 1;
	sprintf(szReturn, "%skbytesPerSec=%.3f$", szReturn, (recvBytes / 1000.0) / (recvTickCount / 1000.0));

	return TRUE;
}

int OpenControlChannel(char *hostname, int port, char *username, char *password, int timeout, char *retrfile, 
					   int ptype, char *pserver, int pport, char *puid, char *ppwd, char *szReturn)
{
	struct	sockaddr_in	sa;
	struct	hostent		*hp;
	char	buffer[1024] = {0};
	int		ret = 0;
	CString	strCmd = _T("");
	CString strMessage = _T("");

	if(*pserver)
	{
		// Use Proxy
		switch(ptype)
		{
		case 0:
			// Common Proxy
			hp = gethostbyname(pserver);
			if(!hp)
			{
				sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_FTP_1%>"));//<%IDS_FTP_1%>
				return 1;
			}

			memcpy(&sa.sin_addr,hp->h_addr_list[0],hp->h_length);
			sa.sin_family = AF_INET;
			sa.sin_port=htons(pport);
			if (connect_timeo(ctrlSocket, (struct sockaddr *)&sa, sizeof(sa), timeout))
			{
				shutdown(ctrlSocket, 0x02);
				closesocket(ctrlSocket);
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_2%>"));//<%IDS_FTP_2%>
				return 1;
			}
			break;
		case 1:
			// Socks5 Proxy
			if(socks5_protocol(ctrlSocket, hostname, port, pserver, pport, puid, ppwd, timeout))
			{
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_3%>"));//<%IDS_FTP_3%>
				return 1;
			}
			break;
		default:
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_4%>"));//<%IDS_FTP_4%>
			return 1;
		}
	}
	else
	{
		// Direct Connect
		hp = gethostbyname(hostname);
		if(!hp)	
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_5%>"));//<%IDS_FTP_5%>
			return 1;
		}

		memcpy(&sa.sin_addr,hp->h_addr_list[0],hp->h_length);
		sa.sin_family = AF_INET;
		sa.sin_port=htons(port);
		if (connect_timeo(ctrlSocket, (struct sockaddr *)&sa, sizeof(sa), timeout))
		{
			shutdown(ctrlSocket, 0x02);
			closesocket(ctrlSocket);
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_6%>"));//<%IDS_FTP_6%>
			return 1;
		}
	}

	CString	retMsg = _T("");
	if(WaitVersionInfo(ctrlSocket, timeout))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_7%>"));//<%IDS_FTP_7%>
		return 1;
	}

	// Send USER Command
	if(*pserver && ptype == 0)
		strCmd.Format("USER %s@%s\r\n", username, hostname);
	else
		strCmd.Format("USER %s\r\n", username);
	ret = FTP_COMMAND(ctrlSocket, strCmd, strMessage);
	if(ret != 2 && ret != 3)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_8%>"));//<%IDS_FTP_8%>
		return 1;
	}

	// Send PASS Command
	strCmd.Format("PASS %s\r\n", password);
	ret = FTP_COMMAND(ctrlSocket, strCmd, strMessage);
	if(ret != 2 && ret != 3)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_9%>"));//<%IDS_FTP_9%>
		return 1;
	}

	// Binary Mode
	strCmd = "TYPE I\r\n";
	ret = FTP_COMMAND(ctrlSocket, strCmd, strMessage);
	if(ret != 2 && ret != 3)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_10%>"));//<%IDS_FTP_10%>
		return 1;
	}

	if(passive_mode)
	{
		// Passive Mode
		if(EnterPassiveMode())
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_11%>"));//<%IDS_FTP_11%>
			return 1;
		}
	}
	else
	{
		int Count = 0;
		while(1)
		{
			local_port = rand() % 55536 + 10000;
			
			sa.sin_family = AF_INET;
			sa.sin_addr.s_addr = htonl(INADDR_ANY);
			sa.sin_port=htons(local_port);
			if(bind(dataSocket, (struct sockaddr *)&sa, sizeof(sa)) == SOCKET_ERROR)
			{
				if(Count < 5)
				{
					Count ++;
					continue;
				}
				else
				{
					sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_12%>"));//<%IDS_FTP_12%>
					return 1;
				}
			}

			if(listen(dataSocket, 10) == SOCKET_ERROR)
			{
				if(Count < 5)
				{
					Count ++;
					continue;
				}
				else
				{
					sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_13%>"));//<%IDS_FTP_13%>
					return 1;
				}
			}

			break;
		}

		struct sockaddr_in lsa;
		int salen = sizeof(lsa);
		if(getsockname(ctrlSocket, (struct sockaddr *)&lsa, &salen) == SOCKET_ERROR)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_14%>"));//<%IDS_FTP_14%>
			return 1;
		}
		strcpy(local_host, inet_ntoa(lsa.sin_addr));
		strCmd.Format("PORT %s,%d,%d\r\n", local_host, local_port/256, local_port%256);
		strCmd.Replace('.', ',');
		ret = FTP_COMMAND(ctrlSocket, strCmd, strMessage);
		if(ret < 0 || ret != 2)
		{
			if(!passive_mode)
			{
				//closesocket(dataSocket);
				passive_mode = 1;
				if(EnterPassiveMode())
				{
					sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_15%>"));//<%IDS_FTP_15%>
					return 1;
				}
			}
		}
	}

	strCmd.Format("RETR %s\r\n", retrfile);
	WriteString(ctrlSocket, strCmd);
	ret = ReadString2(ctrlSocket,buffer);
	if(ret != 1 && ret != 2 && ret != 3 && ret != 9)
	{
		if(ret == 5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_16%>"));//<%IDS_FTP_16%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_17%>"));//<%IDS_FTP_17%>
		return 1;
	}

	return 0;
}

int OpenDataChannel(char *retrfile, int timeout, int ptype, char *pserver, int pport, char *puid, char *ppwd, char *szReturn)
{
	CString	strCmd = _T("");
	CString	retMsg = _T("");
	char	buffer[1024] = {0};
	SOCKET dataChannel;

	if(passive_mode)
	{
		dataChannel = socket(AF_INET, SOCK_STREAM, 0);
		if(dataChannel == SOCKET_ERROR)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_18%>"));//<%IDS_FTP_18%>
			return 1;
		}

		if(*pserver && ptype == 1)
		{
			if(socks5_protocol(dataChannel, remote_host, remote_port, 
						pserver, pport, puid, ppwd, timeout))
			{
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_19%>"));//<%IDS_FTP_19%>
				return 1;
			}
		}
		else
		{
			struct sockaddr_in rsa;
			rsa.sin_addr.s_addr = inet_addr(remote_host);
			rsa.sin_family = AF_INET;
			rsa.sin_port = htons(remote_port);
			if(connect_timeo(dataChannel, (struct sockaddr *)&rsa, sizeof(rsa), timeout))
			{
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_20%>"));//<%IDS_FTP_20%>
				return 1;
			}
		}
	}
	else
	{
		struct sockaddr_in rsa;
		int rsalen = sizeof(rsa);
		dataChannel = accept(dataSocket, (struct sockaddr *)&rsa, &rsalen);
		if(dataChannel == INVALID_SOCKET)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_FTP_21%>"));//<%IDS_FTP_21%>
			return 1;
		}
	}
#ifdef SAVEASFILE
	CFile datafile;
	if(!datafile.Open("C:\\download\\download", CFile::modeWrite|CFile::modeCreate)) 
	{
		return 1;
	}
#endif
	DWORD	recvBegin = GetTickCount();
	while(1) 
	{
		int iRead = 0;
		char	response[1025] = {0};
		iRead = recv(dataChannel, response, 1024, 0);
		if(!iRead || iRead == SOCKET_ERROR)
			break;
		else
		{
#ifdef SAVEASFILE
			datafile.Write(response, iRead);
#endif
			recvBytes += iRead;
		}
	}
	recvTickCount = GetTickCount() - recvBegin;
#ifdef SAVEASFILE
	datafile.Close();
#endif

	return 0;
}

void QuitFromServer()
{
	//CString retMsg = _T("");
	//ExecuteCommand(ctrlSocket, "QUIT\r\n", &retMsg);
	shutdown(ctrlSocket, 0x02);
	closesocket(ctrlSocket);
	shutdown(dataSocket, 0x02);
	closesocket(dataSocket);
}

int WaitVersionInfo(SOCKET s, int nSec)
{
	int		n = 0;
	char	*ca = NULL, *cb = NULL;
	char	buffer[1024] = {0};

	memset(buffer, 0, 1024);
	ca = buffer;
	while(RESPONSE_WAIT(s, nSec))
	{
		char	response[1025] = {0};
		n = recv(s, response, 1024, 0);
		if(n <= 0)
			return 1;

		response[n] = 0;
		strncat(buffer, response, n);

w:		if(isdigit(*ca) && isdigit(*(ca+1)) && isdigit(*(ca+2)) && *(ca+3) == ' ')
		{
			if(*ca - 0x30 == 0x02)
				return 0;

			return 1;
		}
		else
		{
			if(cb = strstr(ca, "\r\n"))
			{
				ca = cb + 2;
				goto w;
			}
		}
	}
	/*
	char	buffer[1024] = {0};

	if(RESPONSE_WAIT(s, nSec))
	{
		int n = recv(s, buffer, 1024, 0);
		if(n <= 0)
			return 1;

		return 0;
	}
	*/
	return 1;
}

int FTP_COMMAND(SOCKET s, CString strCmd, CString &strMessage)
{
	int		iRead = 0;
	char	buff[1024] = {0};
	int		iSent = 0;
	char	*ca = NULL, *cb = NULL;
	
	iSent = send(s, strCmd, strCmd.GetLength(), 0);
	if(iSent == SOCKET_ERROR)
		return -1;

	memset(buff, 0, 1024);
	ca = buff;
	while(RESPONSE_WAIT(s, 20))
	{
		char	response[1025] = {0};
		iRead = recv(s, response, 1024, 0);
		if(iRead == SOCKET_ERROR)
			return -1;
		if(iRead == 0)
			return -1;
		response[iRead] = 0;
		strncat(buff, response, iRead);

w:		if(isdigit(*ca) && isdigit(*(ca+1)) && isdigit(*(ca+2)) && *(ca+3) == ' ')
		{
			strMessage = buff;
			return *ca - 0x30;
		}
		else
		{
			if(cb = strstr(ca, "\r\n"))
			{
				ca = cb + 2;
				goto w;
			}
			else if(iRead == 0)
			{
				return -1;
			}
		}
	}

	return -1;
}

int EnterPassiveMode()
{
	CString strCmd = "PASV\r\n";
	CString strMessage = _T("");
	
	if(FTP_COMMAND(ctrlSocket, strCmd, strMessage) < 0)
		return 1;

	int index = 0, index2 = 0;
	index = strMessage.Find('(', 0);
	index2 = strMessage.Find(')', 0);
	CString strMid = strMessage.Mid(index + 1, index2 - index - 1);
	int s1, s2, s3, s4, s5, s6;
	if(sscanf(strMid, "%d,%d,%d,%d,%d,%d", &s1, &s2, &s3, &s4, &s5, &s6) != 6)
		return 1;

	sprintf(remote_host, "%d.%d.%d.%d", s1, s2, s3, s4);
	remote_port = s5 * 256 + s6;

	return 0;
}

int WriteString(SOCKET s, CString sendStr)
{
	int iSent = send(s, sendStr, sendStr.GetLength(), 0);
	return iSent;
}

int ReadString2(SOCKET s, char *buffer)
{
	int		iRead = 0;
	char	response[1025] = {0};

	memset(buffer, 0, 1024);
	int ret = RESPONSE_WAIT(s, 2);
	if(ret > 0)
	{
		iRead = recv(s, response, 1024, 0);
		if(iRead <= 0)
		{
			return -1;
		}
		response[iRead] = 0;

		strncpy(buffer, response, iRead);

		return *buffer - 0x30;
	}
	else if(ret == 0)
	{
		strcpy(buffer, "Timeout");
		return 9;
	}
	else
	{
		strcpy(buffer, "Select Error");
		return 10;
	}
}

/***************************************************************************
 *
 *	TFTP Server Monitor Functions
 *
 */

BOOL TFTP_MONITOR(char *server, int port, char *file, int timeout, char *custpath, 
				  char *szReturn)
{
	SOCKET	s;
	int		nAckSeq = 0;
	int		nSaveFlag = 0;
	int		datalen = 0;
	DWORD	dwBytes = 0;
	int		i = 0, n = 0, dw = 0;
	struct	hostent *hp = NULL;
	struct	sockaddr_in serv;

	DWORD	dwbTime = 0, dweTime = 0;
	DWORD	dwbReceive = 0, dweReceive = 0;

	int		reqlen = 0;
	u_char	req[1024] = {0};
	u_char	res[1024] = {0};

	if((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		sprintf(szReturn, "error=%s", "Create Socket Error.");
		return FALSE;
	}

	if((hp = gethostbyname(server)) == NULL)
	{
		sprintf(szReturn, "error=%s", "Resolve Hostname Error.");
		return FALSE;
	}

	memcpy(&serv.sin_addr,hp->h_addr_list[0],hp->h_length);
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);

	// RRQ
	req[reqlen ++] = 0x00;
	req[reqlen ++] = 0x01;

	// Filename
	for(i = 0;i < (int)strlen(file);i ++)
		req[reqlen ++] = file[i];
	req[reqlen ++] = 0x00;

	// Mode
	req[reqlen ++] = 'O';
	req[reqlen ++] = 'C';
	req[reqlen ++] = 'T';
	req[reqlen ++] = 'E';
	req[reqlen ++] = 'T';
	req[reqlen ++] = 0x00;

	dwbTime = ::GetTickCount();

	if((n = sendto(s, (char *)req, reqlen, 0, (struct sockaddr *)&serv, sizeof(serv))) == SOCKET_ERROR)
	{
		sprintf(szReturn, "error=%s", "Send Data Error.");
		return FALSE;
	}

#ifdef SAVEASFILE
	CFile	datafile;
	char	szFile[256] = {0};
	
	sprintf(szFile, ".\\%s", file);
	if(!datafile.Open(szFile, CFile::modeWrite|CFile::modeCreate)) 
	{
		sprintf(szReturn, "error=%s", "Local File Create Error.");
		return FALSE;
	}
#endif

	dwbReceive = ::GetTickCount();

	while(1)
	{
		if(RESPONSE_WAIT(s, timeout) <= 0)
		{
			sprintf(szReturn, "error=%s", "Receive Data Timeout.");
			return FALSE;
		}

		dw = sizeof(serv);
		n = recvfrom(s, (char *)res, sizeof(res), 0, (struct sockaddr *)&serv, &dw);
		if(n == SOCKET_ERROR || n == 0)
		{
			sprintf(szReturn, "error=%s", "Receive Data Error.");
			return FALSE;
		}

		int nCode = res[0] * 0x100 + res[1];
		if(nCode == 0x05)
		{
			sprintf(szReturn, "error=%s [%d]$", res + 4, res[2] * 0x100 + res[3]);
			return FALSE;
		}

		if(nCode != 0x03)
		{
			sprintf(szReturn, "error=%s", "Unknown Error.");
			return FALSE;
		}

		datalen = n;

		if(res[2] * 0x100 + res[3] > nAckSeq)
		{
			nAckSeq ++;
			nSaveFlag = 1;
		}

		reqlen = 0;
		req[reqlen ++] = 0x00;
		req[reqlen ++] = 0x04;
		req[reqlen ++] = res[2];
		req[reqlen ++] = res[3];
		if((n = sendto(s, (char *)req, reqlen, 0, (struct sockaddr *)&serv, sizeof(serv))) == SOCKET_ERROR)
		{
			sprintf(szReturn, "error=%s", "Send Data Error.");
			return FALSE;
		}

		if(nSaveFlag) dwBytes += datalen - 4;

#ifdef SAVEASFILE
		if(nSaveFlag) datafile.Write(res + 4, datalen - 4);
#endif
		nSaveFlag = 0;

		if(datalen - 4 < 512) break;
	}
	
#ifdef SAVEASFILE
	datafile.Close();
#endif

	dweTime = ::GetTickCount();
	dweReceive = ::GetTickCount();

	DWORD dwInterval = dweReceive - dwbReceive;
	if(dwInterval == 0) dwInterval = 1;

	sprintf(szReturn, "roundTripTime=%.3f$bytes=%ld$bytesPerSec=%.3f", 
			(dweTime - dwbTime) / 1000.0, dwBytes, (dwBytes / 1024.0) / (dwInterval / 1000.0));

	return TRUE;
}

