
#include "stdafx.h"

class CNNTPSocket  
{
public:
	CNNTPSocket();
	virtual ~CNNTPSocket();

public:
	int connect_timeo(int sockfd,const struct sockaddr *saptr,int salen,int nsec);
	BOOL  Create();
	BOOL  Connect(LPCTSTR pszHostAddress, int nPort = 119);
	BOOL  Send(LPCSTR pszBuf, int nBuf);
	void  Close();
	int   Receive(LPSTR pszBuf, int nBuf);
	BOOL  IsReadible(BOOL& bReadible, int nTimeout);

protected:
	BOOL   Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen);
	SOCKET m_hSocket;
};

class CNNTPConnection  
{
public:
	CNNTPConnection(int nTimeout);
	virtual ~CNNTPConnection();

public:
	BOOL LoginUser(LPCTSTR pszUid);
	BOOL LoginPass(LPCTSTR pszPwd);
	BOOL Group(LPCTSTR pszGroup, int &nArticles);
	BOOL Connect(LPCTSTR pszHostName, LPCTSTR pszUser, LPCTSTR pszPassword, int nPort);
	BOOL Disconnect();
	int CommandResponse(CString &strInfo);

public:
	CNNTPSocket		m_nntp;
	BOOL			m_bConnected;
	DWORD			m_nTimeout;
	CString			m_strErrorMsg;
};

BOOL NEWS_MONITOR(char *server, int port, CStringList &lstGroups, char *uid, char *pwd, 
				  int timeout, char *connaddr, char *custpath, char *szReturn);
