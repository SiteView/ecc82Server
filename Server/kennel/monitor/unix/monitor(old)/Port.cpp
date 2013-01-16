
#include "Port.h"

#include "..\..\base\funcGeneral.h"

BOOL PORT_MONITOR(char *server, int port, char *sendstring, char *matchstring, 
				  int timeout, char *pserver, int pport, char *puid, char *ppwd, 
				  char *custpath, char *szReturn)
{
	DWORD	dwBeginTime = 0,
			dwEndTime = 0;
	int		n = 0, ret = 0;
	SOCKET	s = 0;

	dwBeginTime = GetTickCount();

	// Create Socket
	s = socket(AF_INET,SOCK_STREAM,0);
	if (s == INVALID_SOCKET)
	{
		// Create Socket Error
		sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Port_1%>"));//<%IDS_Port_1%>
		return FALSE;
	}

	if(*pserver)
	{
		if(socks5_protocol(s, server, port, pserver, pport, puid, ppwd, timeout))
		{
			// Sock5 Proxy Error
			sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Port_2%>"));//<%IDS_Port_2%>
			return FALSE;
		}
	}
	else
	{
		struct sockaddr_in	serv;
		struct hostent *hp;

		hp = gethostbyname(server);
		if(hp == NULL)	
		{
			// DNS Resolve Error
			sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Port_3%>"));//<%IDS_Port_3%>
			return FALSE;
		}

		// Connect to the host specialed nonblocking
		memcpy(&serv.sin_addr, hp->h_addr_list[0], hp->h_length);
		serv.sin_family = hp->h_addrtype;
		serv.sin_port=htons(port);
		if (connect_timeo(s, (struct sockaddr *)&serv, sizeof(serv), timeout))
		{
			shutdown(s, 0x02);
			closesocket(s);
			// Server Port Connect Error
			sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Port_4%>"));//<%IDS_Port_4%>
			return FALSE;
		}
	}

	if(*sendstring)
	{
		CString strSend = sendstring;
		strSend.Replace("\\r", "\r");
		strSend.Replace("\\n", "\n");

		n = send(s, strSend, strSend.GetLength(), 0);
		if(n == SOCKET_ERROR)
		{
			// Send Data Error
			sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Port_5%>"));//<%IDS_Port_5%>
			shutdown(s, 0x02);
			closesocket(s);
			return FALSE;
		}
	}

	if(*matchstring)
	{
		char reply[8192] = {0};
		ret = RESPONSE_WAIT(s, timeout);
		if(ret > 0)
		{
			memset(reply, 0, 8192);
			n = recv(s, reply, 8192, 0);
			if(n <= 0)
			{
				shutdown(s, 0x02);
				closesocket(s);
				// Content Match Error
				sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Port_6%>"));//<%IDS_Port_6%>
				return FALSE;
			}

			if(!strstr(reply, matchstring))
			{
				shutdown(s, 0x02);
				closesocket(s);
				// Content Match Error
				sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Port_7%>"));//<%IDS_Port_7%>
				return FALSE;
			}
		}
		else if(ret == 0)
		{
			shutdown(s, 0x02);
			closesocket(s);
			// Read Data Timeout
			sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Port_8%>"));//<%IDS_Port_8%>
			return FALSE;
		}
		else
		{
			shutdown(s, 0x02);
			closesocket(s);
			// Read Data Error
			sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Port_9%>"));//<%IDS_Port_9%>
			return FALSE;
		}
	}

	shutdown(s, 0x02);
	closesocket(s);

	dwEndTime = GetTickCount();
	sprintf(szReturn, "TotalTime=%ld$", dwEndTime - dwBeginTime);

	return TRUE;
}

/*
BOOL PORT_MONITOR(char *server, int port, char *sendstring, char *matchstring, 
				  int timeout, char *pserver, int pport, char *puid, char *ppwd, 
				  char *szReturn)
{
	DWORD		dwBeginTime = 0,
				dwEndTime = 0,
				dwTotalTime = 0;

	dwBeginTime = GetTickCount();
	if(!TcpConnectPort(server, port, sendstring, matchstring, timeout, szReturn))
	{
		dwEndTime = GetTickCount();
		dwTotalTime = dwEndTime - dwBeginTime;
		sprintf(szReturn, "status=200$TotalTime=%ld$", dwTotalTime);
		return TRUE;
	}

	return FALSE;
}
*/

int TcpConnectPort(char *Host, int Port, char *SendStr, char *MatchStr, int Timeout, char *szReturn)
{
	struct sockaddr_in	serv;
	LPHOSTENT lpHost;
	int	tcpSocket, n = 0, ret = 0;
	char request[100] = {0};
	char reply[100] = {0};

	if(*SendStr)
	{
		if(strlen(SendStr) <= 100)
			strcpy(request, SendStr);
		else
			strncpy(request, SendStr, 100);
	}

	lpHost = gethostbyname(Host);
	if(lpHost == NULL)	
	{
		// DNS Resolve Error
		sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Port_10%>"));//<%IDS_Port_10%>
		return -1;
	}

	// Create Socket
	tcpSocket = socket(AF_INET,SOCK_STREAM,0);
	if (tcpSocket < 0)
	{
		// Create Socket Error
		sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Port_11%>"));//<%IDS_Port_11%>
		return -1;
	}

	// Connect to the host specialed nonblocking
	memcpy(&serv.sin_addr,lpHost->h_addr_list[0],lpHost->h_length);
	serv.sin_family = lpHost->h_addrtype;
	serv.sin_port=htons(Port);
	if (connect_timeo(tcpSocket, (struct sockaddr *)&serv, sizeof(serv), 10))
	{
		shutdown(tcpSocket, 0x02);
		closesocket(tcpSocket);
		// Server Port Connect Error
		sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Port_12%>"));//<%IDS_Port_12%>
		return -1;
	}
	
	if(*request)
	{
		n = send(tcpSocket, request, strlen(request), 0);
		if(n == SOCKET_ERROR)
		{
			// Send Data Error
			sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Port_13%>"));//<%IDS_Port_13%>
			shutdown(tcpSocket, 0x02);
			closesocket(tcpSocket);
			return -1;
		}
	}

	if(*MatchStr)
	{
		ret = RESPONSE_WAIT(tcpSocket, Timeout);
		if(ret > 0)
		{
			n = recv(tcpSocket, reply, 100, 0);
			if(n == SOCKET_ERROR)
			{
				shutdown(tcpSocket, 0x02);
				closesocket(tcpSocket);
				// Read Data Error
				sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Port_14%>"));//<%IDS_Port_14%>
				return -1;
			}
			if(n == 0)
			{
				shutdown(tcpSocket, 0x02);
				closesocket(tcpSocket);
				// Content Match Error
				sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Port_15%>"));//<%IDS_Port_15%>
				return -1;
			}
			if(!strstr(reply, MatchStr))
			{
				shutdown(tcpSocket, 0x02);
				closesocket(tcpSocket);
				// Content Match Error
				sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Port_16%>"));//<%IDS_Port_16%>
				return -1;
			}
		}
		else
		{
			shutdown(tcpSocket, 0x02);
			closesocket(tcpSocket);
			// Read Data Error
			sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Port_17%>"));//<%IDS_Port_17%>
			return -1;
		}
	}

	shutdown(tcpSocket, 0x02);
	closesocket(tcpSocket);
	return 0;
}