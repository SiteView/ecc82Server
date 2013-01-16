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
	BOOL    Connect(LPCTSTR pszHostName, LPCTSTR pszUser, LPCTSTR pszPassword, int nPort = 143);
	BOOL    Disconnect();
	BOOL    Noop();
	BOOL	Capability();
	BOOL	Login(LPCTSTR pszUid, LPCTSTR pszPwd);
	BOOL	Logout();
	BOOL	Select(CString strBox);
	BOOL	Retrieve(int nMsg, CIMAP4Message& message);
	BOOL	Delete(int nMsg);
	BOOL	GetMessageSize(int nMsg, DWORD& dwSize);
	LPSTR	GetFirstCharInResponse(LPSTR pszData) const;

	CString	GetLastError();
public:
	CMailSocket		m_imap;
	int				m_nCmdIndex;
	BOOL			m_bConnected;
	DWORD			m_dwTimeout;
	CString			m_sLastCommandResponse;

	enum eState
	{
		NON_AUTHENTICATED = 0,
		AUTHENTICATED, 
		SELECTED, 
		LOGOUT, 
	} m_nState;

protected:
	virtual BOOL ReadCommandResponse(CString strTerminal);
	virtual BOOL ReadReturnResponse(CIMAP4Message& message, DWORD dwSize, LPTSTR pszTerminal);
	virtual BOOL ReadResponse(LPSTR pszBuffer, int nInitialBufSize, LPSTR pszTerminator, 
		LPSTR* ppszOverFlowBuffer, int nGrowBy=4096);
};

#endif // !defined(AFX_IMAP4CONNECTION_H__D29086B4_2FE6_488B_836B_5E85C53F178B__INCLUDED_)
