// IMAP4Connection.h: interface for the CIMAP4Connection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAP4CONNECTION_H__D29086B4_2FE6_488B_836B_5E85C53F178B__INCLUDED_)
#define AFX_IMAP4CONNECTION_H__D29086B4_2FE6_488B_836B_5E85C53F178B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Pop3.h"
#include "IMAP4Message.h"


#include <iostream>
#include <string>

using namespace std;

#define	CMDLINE(buff, index, cmd)							\
	index ++;												\
	if(index < 10)											\
		sprintf(buff, "a00%d %s\r\n", index, cmd);			\
	else if(index < 100)									\
		sprintf(buff, "a0%d %s\r\n", index, cmd);			\
	else													\
		sprintf(buff, "a%d %s\r\n", index, cmd);			\

#define	CMDINDEX(buff, index)								\
	if(index < 10)											\
		sprintf(buff, "a00%d", index);						\
	else if(index < 100)									\
		sprintf(buff, "a0%d", index);						\
	else													\
		sprintf(buff, "a%d", index);

class CIMAP4Connection  
{
public:
	CIMAP4Connection();
	virtual ~CIMAP4Connection();

//Methods
	bool    Connect(const char* pszHostName, const char* pszUser, const char* pszPassword, int nPort = 143);
	bool    Disconnect();
	bool    Noop();
	bool	Capability();
	bool	Login(const char* pszUid, const char* pszPwd);
	bool	Logout();
	bool	Select(string strBox);
	bool	Retrieve(int nMsg, CIMAP4Message& message);
	bool	Delete(int nMsg);
	bool	GetMessageSize(int nMsg, unsigned long& dwSize);
	char*	GetFirstCharInResponse(char* pszData) const;

	string	GetLastError();
public:
	CPop3Socket		m_imap;
	int				m_nCmdIndex;
	bool			m_bConnected;
	unsigned long	m_dwTimeout;
	string			m_sLastCommandResponse;

	enum eState
	{
		NON_AUTHENTICATED = 0,
		AUTHENTICATED, 
		SELECTED, 
		LOGOUT, 
	} m_nState;

protected:
	virtual bool ReadCommandResponse(string strTerminal);
	virtual bool ReadReturnResponse(CIMAP4Message& message, unsigned long dwSize, char* pszTerminal);
	virtual bool ReadResponse(char* pszBuffer, int nInitialBufSize, const char* pszTerminator, 
		char** ppszOverFlowBuffer, int nGrowBy=4096);
};

#endif // !defined(AFX_IMAP4CONNECTION_H__D29086B4_2FE6_488B_836B_5E85C53F178B__INCLUDED_)
