/*
Module : POP3.H
Purpose: Defines the interface for a MFC class encapsulation of the POP3 protocol
Created: PJN / 04-05-1998
History: None

Copyright (c) 1998 - 1999 by PJ Naughter.  
All rights reserved.

*/


/////////////////////////////// Defines ///////////////////////////////////////
#ifndef __POP3_H__
#define __POP3_H__

#ifndef __AFXTEMPL_H__
#pragma message("POP3 classes require afxtempl.h in your PCH")                                                                                
#endif

#ifndef _WINSOCKAPI_
#pragma message("POP3 classes require afxsock.h or winsock.h in your PCH")
#endif
  
/////////////////////////////// Classes ///////////////////////////////////////
////// forward declaration
class CPop3Connection;

//Encapsulation of a POP3 message
class CPop3Message
{
public:
//Constructors / Destructors
  CPop3Message();
  ~CPop3Message();

//Methods
  LPCSTR GetMessageText() const { return m_pszMessage; };
  CString GetHeader() const;
  CString GetHeaderItem(const CString& sName, int nItem = 0) const;
  CString GetBody() const;
	LPCSTR  GetRawBody() const;
	CString GetSubject() const { return GetHeaderItem(_T("Subject")); }
	CString GetFrom() const		 { return GetHeaderItem(_T("From")); }
	CString GetTo() const { return GetHeaderItem(_T("To")); }
	CString GetDate() const		 { return GetHeaderItem(_T("Date")); }
	CString GetReplyTo() const;
	CString GetContentType() const { return GetHeaderItem(_T("Content-Type")); }

//protected:
  char* m_pszMessage;
  friend class CPop3Connection;
};


//Simple Socket wrapper class
class CMailSocket
{
public:
//Constructors / Destructors
  CMailSocket();
  ~CMailSocket();

//methods
  BOOL  Create();
  BOOL  Connect(LPCTSTR pszHostAddress, int nPort = 110);
  BOOL  Send(LPCSTR pszBuf, int nBuf);
  void  Close();
  int   Receive(LPSTR pszBuf, int nBuf);
  BOOL  IsReadible(BOOL& bReadible);

protected:
  BOOL   Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen);
  SOCKET m_hSocket;

	friend class CPop3Connection;
};
  

//The main class which encapsulates the POP3 connection
class CPop3Connection
{
public:
//Constructors / Destructors
  CPop3Connection();
  ~CPop3Connection();

//Methods
  BOOL    Connect(LPCTSTR pszHostName, LPCTSTR pszUser, LPCTSTR pszPassword, int nPort = 110);
  BOOL    Disconnect();
  BOOL    Statistics(int& nNumberOfMails, int& nTotalMailSize);
  BOOL    Delete(int nMsg);
  BOOL    GetMessageSize(int nMsg, DWORD& dwSize);
  BOOL    GetMessageID(int nMsg, CString& sID);
  BOOL    Retrieve(int nMsg, CPop3Message& message);
  BOOL    GetMessageHeader(int nMsg, CPop3Message& message);
  BOOL    Reset();
  BOOL    UIDL();
  BOOL    Noop();
  BOOL	  Top(int nMsg, CString &strMessage);
  CString GetLastCommandResponse() const { return m_sLastCommandResponse; };
  DWORD   GetTimeout() const { return m_dwTimeout; };
  void    SetTimeout(DWORD dwTimeout) { m_dwTimeout = dwTimeout; };

  CString GetLastError();
protected:
  virtual BOOL ReadStatResponse(int& nNumberOfMails, int& nTotalMailSize);
  virtual BOOL ReadCommandResponse();
  virtual BOOL ReadListResponse(int nNumberOfMails);
  virtual BOOL ReadUIDLResponse(int nNumberOfMails);
  virtual BOOL ReadReturnResponse(CPop3Message& message, DWORD dwSize);
  virtual BOOL ReadResponse(LPSTR pszBuffer, int nInitialBufSize, LPSTR pszTerminator, 
                            LPSTR* ppszOverFlowBuffer, int nGrowBy=4096);
  BOOL  List();
  LPSTR GetFirstCharInResponse(LPSTR pszData) const;

  CMailSocket  m_pop;
  int          m_nNumberOfMails;
  BOOL         m_bListRetrieved;
  BOOL         m_bStatRetrieved;
  BOOL         m_bUIDLRetrieved;
  CDWordArray  m_msgSizes;
  CStringArray m_msgIDs;
  BOOL         m_bConnected;
  CString      m_sLastCommandResponse;
  DWORD        m_dwTimeout;
};


#endif //__POP3_H__

