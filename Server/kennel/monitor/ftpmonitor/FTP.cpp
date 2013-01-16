
#include "stdafx.h"
#include "FTP.h"
#include <Base\funcGeneral.h>

CFtpParam::CFtpParam()
{
	beginTickCount = 0;
	endTickCount = 0;
	recvTickCount = 0;
	recvBytes = 0;

//	remote_host[64] = {0};
	remote_port = 0;

//	local_host[64] = {0};
	local_port = 0;

	passive_mode = 0;
}

CFtpParam::~CFtpParam()
{

}

BOOL FTP_MONITOR(char *server, int port, char *file, char *uid, char *pwd, 
				 int timeout, int ptype, char *pserver, int pport, 
				 char *puid, char *ppwd, char *match, char *custpath, int passive_mode, char *szReturn)
{
	CFtpParam ftpParam;

	ftpParam.passive_mode = passive_mode;
	ftpParam.ctrlSocket = socket(AF_INET, SOCK_STREAM, 0);
	ftpParam.dataSocket = socket(AF_INET, SOCK_STREAM, 0);

	srand((unsigned)time(NULL));

	ftpParam.beginTickCount = GetTickCount();
	if(OpenControlChannel(server, port, uid, pwd, timeout, file, ptype, pserver, pport, puid, ppwd, szReturn, ftpParam))
	{
		return FALSE;
	}

    if(strlen(file) > 0)
    {
	    if(OpenDataChannel(file, timeout, ptype, pserver, pport, puid, ppwd, szReturn, ftpParam))
	    {
		    return FALSE;
	    }
    }

	QuitFromServer(ftpParam);
	
	ftpParam.endTickCount = GetTickCount();

	sprintf(szReturn, "roundTripTime=%.3f$bytes=%ld$", (ftpParam.endTickCount - ftpParam.beginTickCount) / 1000.0, ftpParam.recvBytes);
	if(ftpParam.recvTickCount == 0) ftpParam.recvTickCount = 1;
	sprintf(szReturn, "%skbytesPerSec=%.3f$", szReturn, (ftpParam.recvBytes / 1000.0) / (ftpParam.recvTickCount / 1000.0));

	return TRUE;
}

int OpenControlChannel(char *hostname, int port, char *username, char *password, int timeout, char *retrfile, 
					   int ptype, char *pserver, int pport, char *puid, char *ppwd, char *szReturn, CFtpParam& ftpParam)
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
				sprintf(szReturn, "error=%s$",  FuncGetStringFromIDS("SV_PROXY",
                    "PROXY_DOMAIN_NAME_PARSER_ERROR"));//<%IDS_FTP_1%>
				return 1;
			}

			memcpy(&sa.sin_addr,hp->h_addr_list[0],hp->h_length);
			sa.sin_family = AF_INET;
			sa.sin_port=htons(pport);
			if (connect_timeo(ftpParam.ctrlSocket, (struct sockaddr *)&sa, sizeof(sa), timeout))
			{
				shutdown(ftpParam.ctrlSocket, 0x02);
				closesocket(ftpParam.ctrlSocket);
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_PROXY",
                    "PROXY_CONNECT_FAILED"));//<%IDS_FTP_2%>
				return 1;
			}
			break;
		case 1:
			// Socks5 Proxy
			if(socks5_protocol(ftpParam.ctrlSocket, hostname, port, pserver, pport, puid, ppwd, timeout))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_PROXY",
                    "PROXY_SERVER_ERROR"));//<%IDS_FTP_3%>
				return 1;
			}
			break;
		default:
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
                "FTP_UNKNOWN_PROXY_TYPE"));//<%IDS_FTP_4%>
			return 1;
		}
	}
	else
	{
		// Direct Connect
		hp = gethostbyname(hostname);
		if(!hp)	
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_DNS",
                "DNS_DOMAIN_NAME_PARSE_ERROR"));//<%IDS_FTP_5%>
			return 1;
		}

		memcpy(&sa.sin_addr,hp->h_addr_list[0],hp->h_length);
		sa.sin_family = AF_INET;
		sa.sin_port=htons(port);
		if (connect_timeo(ftpParam.ctrlSocket, (struct sockaddr *)&sa, sizeof(sa), timeout))
		{
			shutdown(ftpParam.ctrlSocket, 0x02);
			closesocket(ftpParam.ctrlSocket);
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
                "FTP_CONNET_FAILED"));//<%IDS_FTP_6%>
			return 1;
		}
	}

	CString	retMsg = _T("");
	if(WaitVersionInfo(ftpParam.ctrlSocket, timeout))
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
            "FTP_WAIT_INIT_INFO_TIMEOUT"));//<%IDS_FTP_7%>
		return 1;
	}

	// Send USER Command
	if(*pserver && ptype == 0)
		strCmd.Format("USER %s@%s\r\n", username, hostname);
	else
		strCmd.Format("USER %s\r\n", username);
	ret = FTP_COMMAND(ftpParam.ctrlSocket, strCmd, strMessage);
	if(ret != 2 && ret != 3)
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
            "FTP_SEND_USERNAME_FAILED"));//<%IDS_FTP_8%>
		return 1;
	}

	// Send PASS Command
	strCmd.Format("PASS %s\r\n", password);
	ret = FTP_COMMAND(ftpParam.ctrlSocket, strCmd, strMessage);
	if(ret != 2 && ret != 3)
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
            "FTP_SEND_PWD_FAILED"));//<%IDS_FTP_9%>
		return 1;
	}

	// Binary Mode
	strCmd = "TYPE I\r\n";
	ret = FTP_COMMAND(ftpParam.ctrlSocket, strCmd, strMessage);
	if(ret != 2 && ret != 3)
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
            "FTP_APPLY_BINARY_MODE_ERROR"));//<%IDS_FTP_10%>
		return 1;
	}

	if(ftpParam.passive_mode)
	{
		// Passive Mode
		if(EnterPassiveMode(ftpParam))
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
                "FTP_APPLY_PASSIVITY_MODE_ERROR"));//<%IDS_FTP_11%>
			return 1;
		}
	}
	else
	{
		int Count = 0;
		while(1)
		{
			ftpParam.local_port = rand() % 55536 + 10000;
			
			sa.sin_family = AF_INET;
			sa.sin_addr.s_addr = htonl(INADDR_ANY);
			sa.sin_port=htons(ftpParam.local_port);
			if(bind(ftpParam.dataSocket, (struct sockaddr *)&sa, sizeof(sa)) == SOCKET_ERROR)
			{
				if(Count < 5)
				{
					Count ++;
					continue;
				}
				else
				{
					sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
                        "FTP_LOCATE_BIND_ERROR"));//<%IDS_FTP_12%>
					return 1;
				}
			}

			if(listen(ftpParam.dataSocket, 10) == SOCKET_ERROR)
			{
				if(Count < 5)
				{
					Count ++;
					continue;
				}
				else
				{
					sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
                        "FTP_LOCATE_CREATE_COMMUNICATE_ERROR"));//<%IDS_FTP_13%>
					return 1;
				}
			}

			break;
		}

		struct sockaddr_in lsa;
		int salen = sizeof(lsa);
		if(getsockname(ftpParam.ctrlSocket, (struct sockaddr *)&lsa, &salen) == SOCKET_ERROR)
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
                "FTP_GET_LOCATE_INFO_ERROR"));//<%IDS_FTP_14%>
			return 1;
		}
		strcpy(ftpParam.local_host, inet_ntoa(lsa.sin_addr));
		strCmd.Format("PORT %s,%d,%d\r\n", ftpParam.local_host, ftpParam.local_port/256, ftpParam.local_port%256);
		strCmd.Replace('.', ',');
		ret = FTP_COMMAND(ftpParam.ctrlSocket, strCmd, strMessage);
		if(ret < 0 || ret != 2)
		{
			if(!ftpParam.passive_mode)
			{
				//closesocket(dataSocket);
				ftpParam.passive_mode = 1;
				if(EnterPassiveMode(ftpParam))
				{
					sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
                        "FTP_APPLY_PASSIVITY_MODE_ERROR"));//<%IDS_FTP_15%>
					return 1;
				}
			}
		}
	}

    if(strlen(retrfile) > 0)
    {
	    strCmd.Format("RETR %s\r\n", retrfile);
	    WriteString(ftpParam.ctrlSocket, strCmd);
	    ret = ReadString2(ftpParam.ctrlSocket,buffer);
	    if(ret != 1 && ret != 2 && ret != 3 && ret != 9)
	    {
		    if(ret == 5)
			    sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
                "FTP_FILE_NOT_FOUND"));//<%IDS_FTP_16%>
		    else
			    sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
                "FTP_UNKNOWN_ERROR"));//<%IDS_FTP_17%>
		    return 1;
	    }
    }

	return 0;
}

int OpenDataChannel(char *retrfile, int timeout, int ptype, char *pserver, int pport, char *puid, char *ppwd, char *szReturn, CFtpParam& ftpParam)
{
	CString	strCmd = _T("");
	CString	retMsg = _T("");
	char	buffer[1024] = {0};
	SOCKET dataChannel;

	if(ftpParam.passive_mode)
	{
		dataChannel = socket(AF_INET, SOCK_STREAM, 0);
		if(dataChannel == SOCKET_ERROR)
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
                "FTP_CREATE_COMMUNICATE_ERROR"));//<%IDS_FTP_18%>
			return 1;
		}

		if(*pserver && ptype == 1)
		{
			if(socks5_protocol(dataChannel, ftpParam.remote_host, ftpParam.remote_port, 
						pserver, pport, puid, ppwd, timeout))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
                    "FTP_CREATE_COMMUNICATE_ERROR"));//<%IDS_FTP_19%>
				return 1;
			}
		}
		else
		{
			struct sockaddr_in rsa;
			rsa.sin_addr.s_addr = inet_addr(ftpParam.remote_host);
			rsa.sin_family = AF_INET;
			rsa.sin_port = htons(ftpParam.remote_port);
			if(connect_timeo(dataChannel, (struct sockaddr *)&rsa, sizeof(rsa), timeout))
			{
				sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
                    "FTP_CREATE_COMMUNICATE_ERROR"));//<%IDS_FTP_20%>
				return 1;
			}
		}
	}
	else
	{
		struct sockaddr_in rsa;
		int rsalen = sizeof(rsa);
		dataChannel = accept(ftpParam.dataSocket, (struct sockaddr *)&rsa, &rsalen);
		if(dataChannel == INVALID_SOCKET)
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
                "FTP_CREATE_COMMUNICATE_ERROR"));//<%IDS_FTP_21%>
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
			ftpParam.recvBytes += iRead;
		}
	}
	ftpParam.recvTickCount = GetTickCount() - recvBegin;
#ifdef SAVEASFILE
	datafile.Close();
#endif

	return 0;
}

void QuitFromServer(CFtpParam& ftpParam)
{
	//CString retMsg = _T("");
	//ExecuteCommand(ctrlSocket, "QUIT\r\n", &retMsg);
	shutdown(ftpParam.ctrlSocket, 0x02);
	closesocket(ftpParam.ctrlSocket);
	shutdown(ftpParam.dataSocket, 0x02);
	closesocket(ftpParam.dataSocket);
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

int EnterPassiveMode(CFtpParam& ftpParam)
{
	CString strCmd = "PASV\r\n";
	CString strMessage = _T("");
	
	if(FTP_COMMAND(ftpParam.ctrlSocket, strCmd, strMessage) < 0)
		return 1;

	int index = 0, index2 = 0;
	index = strMessage.Find('(', 0);
	index2 = strMessage.Find(')', 0);
	CString strMid = strMessage.Mid(index + 1, index2 - index - 1);
	int s1, s2, s3, s4, s5, s6;
	if(sscanf(strMid, "%d,%d,%d,%d,%d,%d", &s1, &s2, &s3, &s4, &s5, &s6) != 6)
		return 1;

	sprintf(ftpParam.remote_host, "%d.%d.%d.%d", s1, s2, s3, s4);
	ftpParam.remote_port = s5 * 256 + s6;

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
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_SOCKET",
            "SOCKET_CREATE_FAILED"));
		return FALSE;
	}

	if((hp = gethostbyname(server)) == NULL)
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_DSN",
            "DNS_DOMAIN_NAME_PARSE_ERROR"));
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
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_SOCKET",
            "SOCKET_SEND_DATA_ERROR"));
		return FALSE;
	}

#ifdef SAVEASFILE
	CFile	datafile;
	char	szFile[256] = {0};
	
	sprintf(szFile, ".\\%s", file);
	if(!datafile.Open(szFile, CFile::modeWrite|CFile::modeCreate)) 
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_CREATE_FILE_FAIL"));
		return FALSE;
	}
#endif

	dwbReceive = ::GetTickCount();

	while(1)
	{
		if(RESPONSE_WAIT(s, timeout) <= 0)
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_SOCKET",
                "SOCKET_RECV_DATA_TIMEOUT"));
			return FALSE;
		}

		dw = sizeof(serv);
		n = recvfrom(s, (char *)res, sizeof(res), 0, (struct sockaddr *)&serv, &dw);
		if(n == SOCKET_ERROR || n == 0)
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_SCOKET",
                "SOCKET_RECV_DATA_ERROR"));
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
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_SOCKET",
                "SOCKET_UNKNOWN_ERROR"));
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
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_SOCKET",
                "SOCKET_SEND_DATA_EEROR"));
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

