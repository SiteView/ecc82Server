
#include "News.h"

#include "base\funcGeneral.h"


BOOL NEWS_MONITOR(char *server, int port, CStringList &lstGroups, char *uid, char *pwd, 
				  int timeout, char *connaddr, char *custpath, char *szReturn)
{
	DWORD dwBeginTime = 0, dwEndTime = 0;

	dwBeginTime = ::GetTickCount();

	CNNTPConnection nntp(timeout);

	if(!nntp.Connect(server, uid, pwd, port))
	{
		sprintf(szReturn, "error=%s", nntp.m_strErrorMsg);
		return FALSE;
	}

	int nArticles = 0;
	POSITION pos = lstGroups.FindIndex(0);
	while(pos)
	{
		int n = 0;
		CString strGroup = lstGroups.GetNext(pos);
		if(!nntp.Group((LPCTSTR)strGroup, n))
		{
			sprintf(szReturn, "error=%s", nntp.m_strErrorMsg);
			return FALSE;
		}

		nArticles += n;
	}

	nntp.Disconnect();

	dwEndTime = ::GetTickCount();

	sprintf(szReturn, "Articles=%d$roundTripTime=%ld$", 
				nArticles, dwEndTime - dwBeginTime);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// CNNTPSocket Member Functions
//////////////////////////////////////////////////////////////////////

CNNTPSocket::CNNTPSocket()
{

}

CNNTPSocket::~CNNTPSocket()
{

}

int CNNTPSocket::connect_timeo(int sockfd,const struct sockaddr *saptr,int salen,int nsec)
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

BOOL CNNTPSocket::Create()
{
	m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
	return (m_hSocket != INVALID_SOCKET);
}

BOOL CNNTPSocket::Connect(LPCTSTR pszHostAddress, int nPort)
{
	USES_CONVERSION;
	
	if(m_hSocket == INVALID_SOCKET)
		return FALSE;
	
	SOCKADDR_IN sockAddr;
	ZeroMemory(&sockAddr, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons((u_short)nPort);
	char* pszAsciiHostAddress = T2A((LPTSTR) pszHostAddress);
	sockAddr.sin_addr.s_addr = inet_addr(pszAsciiHostAddress);
	
	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname(pszAsciiHostAddress);
		if (lphost != NULL)
			sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
			return FALSE;
		}
	}
	
	return Connect((SOCKADDR*)&sockAddr, sizeof(sockAddr));
}

BOOL CNNTPSocket::Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{
	return (connect(m_hSocket, lpSockAddr, nSockAddrLen) != SOCKET_ERROR);
	//return (connect_timeo(m_hSocket, lpSockAddr, nSockAddrLen, nsec) == 0)
}

BOOL CNNTPSocket::Send(LPCSTR pszBuf, int nBuf)
{
	if(m_hSocket == INVALID_SOCKET)
		return FALSE;
	
	return (send(m_hSocket, pszBuf, nBuf, 0) != SOCKET_ERROR);
}

int CNNTPSocket::Receive(LPSTR pszBuf, int nBuf)
{
	if(m_hSocket == INVALID_SOCKET)
		return FALSE;
	
	return recv(m_hSocket, pszBuf, nBuf, 0); 
}

void CNNTPSocket::Close()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		shutdown(m_hSocket, 0x02);
		closesocket(m_hSocket);
		m_hSocket = INVALID_SOCKET;
	}
}

BOOL CNNTPSocket::IsReadible(BOOL& bReadible, int nTimeout)
{
	timeval timeout = {nTimeout, 0};
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(m_hSocket, &fds);
	int nStatus = select(0, &fds, NULL, NULL, &timeout);
	if (nStatus == SOCKET_ERROR)
	{
		return FALSE;
	}
	else
	{
		bReadible = !(nStatus == 0);
		return TRUE;
	}
}



//////////////////////////////////////////////////////////////////////
// CNNTPConnection Member Functions
//////////////////////////////////////////////////////////////////////

CNNTPConnection::CNNTPConnection(int nTimeout)
{
	m_bConnected = FALSE;
	m_nTimeout = nTimeout;
	m_strErrorMsg = _T("OK");
}

CNNTPConnection::~CNNTPConnection()
{
	
}

BOOL CNNTPConnection::LoginUser(LPCTSTR pszUid)
{
	if(!m_bConnected)
		return FALSE;

	CString	strCmd = _T("AUTHINFO USER ");
	strCmd += pszUid;
	strCmd += _T("\r\n");

	if(!m_nntp.Send(strCmd, strCmd.GetLength()))
	{
		m_strErrorMsg = FuncGetStringFromIDS("<%IDS_News_1%>");//<%IDS_News_1%>_T("写数据错误")
		return FALSE;
	}

	CString strInfo = _T("");
	int nRes = CommandResponse(strInfo);
	if(nRes != 2 && nRes != 3)
	{
		m_strErrorMsg = strInfo;
		return FALSE;
	}

	return TRUE;
}

BOOL CNNTPConnection::LoginPass(LPCTSTR pszPwd)
{
	if(!m_bConnected)
		return FALSE;

	CString	strCmd = _T("AUTHINFO PASS ");
	strCmd += pszPwd;
	strCmd += _T("\r\n");

	if(!m_nntp.Send(strCmd, strCmd.GetLength()))
	{
		m_strErrorMsg = FuncGetStringFromIDS("<%IDS_News_2%>");//<%IDS_News_2%>_T("写数据错误")
		return FALSE;
	}

	CString strInfo = _T("");
	int nRes = CommandResponse(strInfo);
	if(nRes != 2 && nRes != 3)
	{
		m_strErrorMsg = strInfo;
		return FALSE;
	}

	return TRUE;
}

BOOL CNNTPConnection::Group(LPCTSTR pszGroup, int &nArticles)
{
	if(!m_bConnected)
		return FALSE;
	
	CString strCmd = _T("GROUP ");
	strCmd += pszGroup;
	strCmd += _T("\r\n");

	if(!m_nntp.Send(strCmd, strCmd.GetLength()))
	{
		m_strErrorMsg = FuncGetStringFromIDS("<%IDS_News_3%>");//<%IDS_News_3%>_T("写数据错误")
		return FALSE;
	}

	CString strInfo = _T("");
	int nRes = CommandResponse(strInfo);
	if(nRes != 2)
	{
		m_strErrorMsg = strInfo;
		return FALSE;
	}

	strInfo.Delete(0, 3);
	nArticles = atoi(strInfo);
	return TRUE;
}

BOOL CNNTPConnection::Connect(LPCTSTR pszHostName, LPCTSTR pszUser, LPCTSTR pszPassword, int nPort)
{
	USES_CONVERSION;

	if (!m_nntp.Create())
	{
		m_strErrorMsg = FuncGetStringFromIDS("<%IDS_News_4%>");//<%IDS_News_4%>_T("通信初始化失败")
		return FALSE;
	}

	if (!m_nntp.Connect(pszHostName, nPort))
	{
		m_strErrorMsg = FuncGetStringFromIDS("<%IDS_News_5%>");//<%IDS_News_5%>_T("主机连接失败")
		return FALSE;
	}
	else
	{
		m_bConnected = TRUE;
		
		CString	strInfo = _T("");
		int nRes = CommandResponse(strInfo);
		if(nRes != 2)
		{
			m_strErrorMsg = strInfo;
			Disconnect();
			return FALSE;
		}

		if(*pszUser)
		{
			if(!LoginUser(pszUser))
			{
				m_strErrorMsg = FuncGetStringFromIDS("<%IDS_News_6%>");//<%IDS_News_6%>_T("登录失败")
				Disconnect();
				return FALSE;
			}

			if(!LoginUser(pszPassword))
			{
				m_strErrorMsg = FuncGetStringFromIDS("<%IDS_News_7%>");//<%IDS_News_7%>_T("登录失败")
				Disconnect();
				return FALSE;
			}
		}

		return TRUE;
	}
}

int CNNTPConnection::CommandResponse(CString &strInfo)
{
	char szResponse[1024] = {0};

	while(1)
	{
		BOOL bReadible = FALSE;
		if (!m_nntp.IsReadible(bReadible, m_nTimeout))
		{
			m_strErrorMsg = FuncGetStringFromIDS("<%IDS_News_8%>");//<%IDS_News_8%>_T("读数据错误")
			return 0;
		}
		else if (!bReadible)
		{
			m_strErrorMsg = FuncGetStringFromIDS("<%IDS_News_9%>");//<%IDS_News_9%>_T("读数据超时")
			return 0;
		}
		else
		{
			char szTemp[1024] = {0};
			int nData = m_nntp.Receive(szTemp, sizeof(szTemp));
			if(nData == SOCKET_ERROR)
			{
				m_strErrorMsg = FuncGetStringFromIDS("<%IDS_News_10%>");//<%IDS_News_10%>_T("读数据错误")
				return 0;
			}
			szTemp[nData] = 0;
			strncat(szResponse, szTemp, nData);
			char *ca = NULL;
			if(ca = strstr(szResponse, "\r\n"))
			{
				szResponse[ca - szResponse] = 0;
				break;
			}
		}
	}

	strInfo = szResponse;
	return szResponse[0] - 0x30;
}

BOOL CNNTPConnection::Disconnect()
{
	int nRes = 0;
	BOOL bSuccess = FALSE;      
	
	if (m_bConnected)
	{
		CString	strCmd = _T("QUIT\r\n");
		if (!m_nntp.Send(strCmd, strCmd.GetLength()))
			TRACE(_T("Failed to send the QUIT command to the POP3 server\n"));
		
		CString strInfo = _T("");
		nRes = CommandResponse(strInfo);
		
		m_bConnected = FALSE;
	}
	else
		TRACE(_T("CNNTPConnection, Already disconnected\n"));
	
	m_nntp.Close();
	
	return nRes == 2;
}