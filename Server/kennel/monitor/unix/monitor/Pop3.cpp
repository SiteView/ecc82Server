/*
Module : POP3.CPP
Purpose: Implementation for a MFC class encapsulation of the POP3 protocol
Created: PJN / 04-05-1998
History: PJN / 27-06-1998 1) Fixed a potential buffer overflow problem in Delete
                          and Retrieve functions when a large message number was
                          specified.
                          2) Improve the ReadResponse code by a) passing the 
                          readability check onto the scoket class and b) Sleeping
                          for 100 ms prior to looping around waiting for new 
                          response data
                          3) Classes are now fully Unicode compliant. Unicode
                          build configurations are also now included.
                          4) Now supports the TOP POP3 command which can be
                          issued using the GetHeader function.

         PJN / 04-01-1999 1) Properly UNICODE enabled the code

         PJN / 22-02-1999 1) Improved the reading of responses back from the server by implementing
                          a growable receive buffer
                          2) timeout is now 60 seconds when building for debug
                          3) Code now yields its time slice while waiting for a timeout
                          4) Made most functions virtual to help end user usage

         PJN / 25-03-1999 1) Fixed memory leak in the CPop3Connection::ReadReturnResponse function.
                          2) Now sleeps for 250 ms instead of yielding the time slice. This helps 
                          reduce CPU usage when waiting for data to arrive in the socket

         PJN / 15-06-1999 1) Added functions to return the message body, header or a particular
                          header field of a message
                          2) Tidied up some of the TRACE messages which the code generates

         PJN / 16-06-1999 1) Fixed a bug in the GetHeaderItem function which was causing a header
                          item which appeared at the begining of the header fields to fail to be 
                          parsed incorrectly.

         PJN / 27-06-1999 1) Fixed a bug in the GetHeaderItem function when a header spanned 
                          multiple lines as is allowed by RFC 822

         PJN / 29-06-1999 1) Another improvement to GetHeaderItem. When will it end <g>. Originally 
                          this was reported as a bug but upon further investigation it turns out that
                          the message which was causing the problems had embedded tabs in the header. 
                          This is discouraged by the RFC which refers to mail headers (RFC 822). 
                          The code has been enhanced to handle this case. Thanks to Chris Bishop 
                          for spotting this.
                          2) Fix for a bug in GetHeaderItem where I accidently was using "=" instead of
                          "==". Thanks to Angelini Fabio for spotting this.

         PJN / 05-07-1999 1) Addition of the following functions:

                          i)   CPop3Message::GetReplyTo
                          ii)  CPop3Message::GetRawBody      
                          iii) CPop3Message::GetSubject                 
	                        iv)  CPop3Message::GetFrom                    
	                        v)   CPop3Message::GetDate                    

                          2) GetHeaderItem function now uses case insensitive searching
                          3) GetHeaderItem now allows you to search for the "n'th" header of a specified type

         PJN / 24-08-99   1) Fixed a bug whereby the function GetHeader was sometimes failing when it
                          was called when the message was retrieved using the "TOP" command.
                             

Copyright (c) 1998 - 1999 by PJ Naughter.  
All rights reserved.

*/

//////////////// Includes ////////////////////////////////////////////
#include "stdafx.h"
#include "pop3.h"

#include "..\..\base\funcGeneral.h"

//////////////// Macros //////////////////////////////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//////////////// Implementation //////////////////////////////////////
CPop3Message::CPop3Message()
{
	m_pszMessage = NULL;
}

CPop3Message::~CPop3Message()
{
	if (m_pszMessage)
	{
		delete [] m_pszMessage;
		m_pszMessage = NULL;
	}
}

CString CPop3Message::GetHeaderItem(const CString& sName, int nItem) const
{
	//Value which will be returned by this function
	CString sField;
	
	//Get the message header (add an extra "\r\n" at the
	//begining to aid in the parsing)  
	CString sHeader(_T("\r\n"));
	sHeader += GetHeader();
	CString sUpCaseHeader(sHeader);
	sUpCaseHeader.MakeUpper();
	
	CString sUpCaseName(sName);
	sUpCaseName.MakeUpper();
	
	//Find the specified line in the header
	CString sFind(CString(_T("\r\n")) + sUpCaseName + _T(":"));
	int nFindLength = sFind.GetLength();
	int nFindStart = sUpCaseHeader.Find(sFind);
	int nFind = nFindStart;
	for (int i=0; i<nItem; i++) 
	{
		//Get ready for the next loop around
		sUpCaseHeader = sUpCaseHeader.Right(sUpCaseHeader.GetLength() - nFind - nFindLength);
		nFind = sUpCaseHeader.Find(sFind);
		
		if (nFind == -1)
			return _T(""); //Not found
		else
			nFindStart += (nFind + nFindLength);
	}
	
	if (nFindStart != -1)
		nFindStart += (3 + sName.GetLength());
	if (nFindStart != -1)
	{
		BOOL bFoundEnd = FALSE;
		int i = nFindStart;
		int nLength = sHeader.GetLength();
		do
		{
			//Examine the current 3 characters
			TCHAR c1 = _T('\0');
			if (i < nLength)
				c1 = sHeader[i];
			TCHAR c2 = _T('\0');
			if (i < (nLength-1))
				c2 = sHeader[i+1];
			TCHAR c3 = _T('\0');
			if (i < (nLength-2))
				c3 = sHeader[i+2];
			
			//Have we found the terminator
			if ((c1 == _T('\0')) ||
				((c1 == _T('\r')) && (c2 == _T('\n')) && (c3 != _T(' ')) && c3 != _T('\t')))
			{
				bFoundEnd = TRUE;
			}
			else
			{
				//Move onto the next character  
				++i;
			}
		}
		while (!bFoundEnd);
		sField = sHeader.Mid(nFindStart, i - nFindStart);
		
		//Remove any embedded "\r\n" sequences from the field
		int nEOL = sField.Find(_T("\r\n"));
		while (nEOL != -1)
		{
			sField = sField.Left(nEOL) + sField.Right(sField.GetLength() - nEOL - 2);
			nEOL = sField.Find(_T("\r\n"));
		}
		
		//Replace any embedded "\t" sequences with spaces
		int nTab = sField.Find(_T('\t'));
		while (nTab != -1)
		{
			sField = sField.Left(nTab) + _T(' ') + sField.Right(sField.GetLength() - nTab - 1);
			nTab = sField.Find(_T('\t'));
		}
		
		//Remove any leading or trailing white space from the Field Body
		sField.TrimLeft();
		sField.TrimRight();
	}
	
	return sField;
}

CString CPop3Message::GetHeader() const
{
	//Value which will be returned by this function
	CString sHeader;
	
	//Find the divider between the header and body
	CString sMessage(m_pszMessage);
	int nFind = sMessage.Find(_T("\r\n\r\n"));
	if (nFind != -1)
		sHeader = sMessage.Left(nFind);
	else
	{
		//No divider, then assume all the text is the header
		sHeader = sMessage;
	}
	
	return sHeader;
}

LPCSTR CPop3Message::GetRawBody() const
{
	char* pszStartBody = strstr(GetMessageText(), "\r\n\r\n");
	if (pszStartBody == NULL) 
		return NULL;
	else 
		return pszStartBody + 4;
}

CString CPop3Message::GetBody() const
{
	CString sBody;
	LPCSTR pszBody = GetRawBody();
	if (pszBody)
		sBody = pszBody;
	return sBody;
}

CString CPop3Message::GetReplyTo() const
{
	CString sRet = GetHeaderItem("Reply-To");
	if (sRet.IsEmpty())
	{
		sRet = GetFrom();
		if (sRet.IsEmpty())
		{
			sRet = GetHeaderItem(_T("Sender"));
			if (sRet.IsEmpty())
				sRet = GetHeaderItem(_T("Return-Path"));
		}
	}
	return sRet;
}






CMailSocket::CMailSocket()
{
	m_hSocket = INVALID_SOCKET; //default to an invalid scoket descriptor
}

CMailSocket::~CMailSocket()
{
	Close();
}

BOOL CMailSocket::Create()
{
	m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
	return (m_hSocket != INVALID_SOCKET);
}

BOOL CMailSocket::Connect(LPCTSTR pszHostAddress, int nPort)
{
	//For correct operation of the T2A macro, see MFC Tech Note 59
	USES_CONVERSION;
	
	//must have been created first
	if(m_hSocket == INVALID_SOCKET)
		return FALSE;
	
	//Determine if the address is in dotted notation
	SOCKADDR_IN sockAddr;
	ZeroMemory(&sockAddr, sizeof(sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons((u_short)nPort);
	char* pszAsciiHostAddress = T2A((LPTSTR) pszHostAddress);
	sockAddr.sin_addr.s_addr = inet_addr(pszAsciiHostAddress);
	
	//If the address is not dotted notation, then do a DNS 
	//lookup of it.
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
	
	//Call the protected version which takes an address 
	//in the form of a standard C style struct.
	return Connect((SOCKADDR*)&sockAddr, sizeof(sockAddr));
}

BOOL CMailSocket::Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{
	return (connect(m_hSocket, lpSockAddr, nSockAddrLen) != SOCKET_ERROR);
}

BOOL CMailSocket::Send(LPCSTR pszBuf, int nBuf)
{
	//must have been created first
	if(m_hSocket == INVALID_SOCKET)
		return FALSE;
	
	return (send(m_hSocket, pszBuf, nBuf, 0) != SOCKET_ERROR);
}

int CMailSocket::Receive(LPSTR pszBuf, int nBuf)
{
	//must have been created first
	if(m_hSocket == INVALID_SOCKET)
		return FALSE;
	
	return recv(m_hSocket, pszBuf, nBuf, 0); 
}

void CMailSocket::Close()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		shutdown(m_hSocket, 0x02);
		VERIFY(SOCKET_ERROR != closesocket(m_hSocket));
		m_hSocket = INVALID_SOCKET;
	}
}

BOOL CMailSocket::IsReadible(BOOL& bReadible)
{
	timeval timeout = {0, 0};
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






CPop3Connection::CPop3Connection()
{
	m_nNumberOfMails = 0;
	m_bListRetrieved = FALSE;
	m_bStatRetrieved = FALSE;
	m_bUIDLRetrieved = FALSE;
	m_msgSizes.RemoveAll();
	m_bConnected = FALSE;
#ifdef _DEBUG
	m_dwTimeout = 60000; //default timeout of 60 seconds when debugging
#else
	m_dwTimeout = 2000;  //default timeout of 2 seconds for normal release code
#endif
}

CPop3Connection::~CPop3Connection()
{
	if (m_bConnected)
		Disconnect();
}

BOOL CPop3Connection::Connect(LPCTSTR pszHostName, LPCTSTR pszUser, LPCTSTR pszPassword, int nPort)
{
	//For correct operation of the T2A macro, see MFC Tech Note 59
	USES_CONVERSION;

	//Create the socket
	if (!m_pop.Create())
	{
		//m_sLastCommandResponse = _T("通信初始化失败");//<%IDS_Pop3_1%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_1%>");//<%IDS_Pop3_1%>
		return FALSE;
	}

	//Connect to the POP3 Host
	if (!m_pop.Connect(pszHostName, nPort))
	{
		//m_sLastCommandResponse = _T("主机连接失败");//<%IDS_Pop3_2%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_2%>");
		return FALSE;
	}
	else
	{
		//We're now connected !!
		m_bConnected = TRUE;
		
		//check the response
		if (!ReadCommandResponse())
		{
			//m_sLastCommandResponse = _T("读取初始信息超时");//<%IDS_Pop3_3%>
			m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_3%>");
			Disconnect();
			return FALSE;
		}
		
		//Send the POP3 username and check the response
		char sBuf[128];
		char* pszAsciiUser = T2A((LPTSTR) pszUser);
		if(strlen(pszAsciiUser) >= 100)
		{
			//m_sLastCommandResponse = _T("用户名太长");//<%IDS_Pop3_4%>
			m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_4%>");
			return FALSE;
		}
		
		sprintf(sBuf, "USER %s\r\n", pszAsciiUser);
		int nCmdLength = strlen(sBuf);
		
		if (!m_pop.Send(sBuf, nCmdLength))
		{
			TRACE(_T("Failed to send the USER command to the POP3 server\n"));
			//m_sLastCommandResponse = _T("发送USER命令失败");//<%IDS_Pop3_5%>
			m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_5%>");
			Disconnect();
			return FALSE;
		}
		if (!ReadCommandResponse())
		{
			TRACE(_T("Failed while connected to read a USER command response from the POP3 server\n"));
			//m_sLastCommandResponse = _T("发送USER命令超时");
			Disconnect();
			return FALSE;
		} 

		//Send the POP3 password and check the response
		char* pszAsciiPassword = T2A((LPTSTR) pszPassword);
		if(strlen(pszAsciiPassword) >= 100)
		{
			//m_sLastCommandResponse = _T("密码太长");//<%IDS_Pop3_6%>
			m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_6%>");
			return FALSE;
		}
		sprintf(sBuf, "PASS %s\r\n", pszAsciiPassword);
		nCmdLength = strlen(sBuf);
		if (!m_pop.Send(sBuf, nCmdLength))
		{
			TRACE(_T("Failed to send the PASS command to the POP3 server\n"));
			//m_sLastCommandResponse = _T("发送PASS命令失败");//<%IDS_Pop3_7%>
			m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_7%>");
			Disconnect();
			return FALSE;
		}
		if (!ReadCommandResponse())
		{
			TRACE(_T("Failed while connected to read a PASS command response from the POP3 server\n"));
			//m_sLastCommandResponse = _T("发送PASS命令超时");
			Disconnect();
			return FALSE;
		}
		
		return TRUE;
	}
}

BOOL CPop3Connection::Disconnect()
{          
	//disconnect from the POP3 server if connected 
	if (m_bConnected)
	{
		char sBuf[10];
		strcpy(sBuf, "QUIT\r\n");
		int nCmdLength = strlen(sBuf);
		if (!m_pop.Send(sBuf, nCmdLength))
			TRACE(_T("Failed to send the QUIT command to the POP3 server\n"));
		
		//Check the reponse
		//bSuccess = ReadCommandResponse();
		
		//Reset all the state variables
		m_bConnected = FALSE;
		m_bListRetrieved = FALSE;
		m_bStatRetrieved = FALSE;
		m_bUIDLRetrieved = FALSE;
	}
	else
		TRACE(_T("CPop3Connection, Already disconnected\n"));
	
	//free up our socket
	m_pop.Close();
	
	return TRUE;
}

BOOL CPop3Connection::Delete(int nMsg)
{
	//Must be connected to perform a delete
	if(!m_bConnected)
	{
		//m_sLastCommandResponse = _T("通信未建立");//<%IDS_Pop3_8%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_8%>");
		return FALSE;
	}
	
	//if we haven't executed the LIST command then do it now
	if (!m_bListRetrieved)
		List();
	
	//Send the DELE command along with the message ID
	char sBuf[20];
	sprintf(sBuf, "DELE %d\r\n", nMsg);
	int nCmdLength = strlen(sBuf);
	if (!m_pop.Send(sBuf, nCmdLength))
	{
		TRACE(_T("Failed to send the DELE command to the POP3 server\n"));
		//m_sLastCommandResponse = _T("发送DELE命令失败");//<%IDS_Pop3_9%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_9%>");
		return FALSE;
	}
	
	return ReadCommandResponse();
}

BOOL CPop3Connection::Statistics(int& nNumberOfMails, int& nTotalMailSize)
{
	//Must be connected to perform a "STAT"
	if(!m_bConnected)
	{
		//m_sLastCommandResponse = _T("通信未建立");//<%IDS_Pop3_10%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_10%>");
		return FALSE;
	}
	
	//Send the STAT command
	char sBuf[10];
	strcpy(sBuf, "STAT\r\n");
	int nCmdLength = strlen(sBuf);
	if (!m_pop.Send(sBuf, nCmdLength))
	{
		TRACE(_T("Failed to send the STAT command to the POP3 server\n"));
		//m_sLastCommandResponse = _T("发送STAT命令失败");//<%IDS_Pop3_11%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_11%>");
		return FALSE;
	}
	
	return ReadStatResponse(nNumberOfMails, nTotalMailSize);
}

BOOL CPop3Connection::GetMessageSize(int nMsg, DWORD& dwSize)
{
	BOOL bSuccess = TRUE;

	//if we haven't executed the LIST command then do it now
	if (!m_bListRetrieved)
		bSuccess = List();

	//nMsg must be in the correct range
	if(nMsg <= 0 || nMsg > m_msgSizes.GetSize())
	{
		//m_sLastCommandResponse = _T("无效邮件");//<%IDS_Pop3_12%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_12%>");
		return FALSE;
	}
	
	//retrieve the size from the message size array
	dwSize = m_msgSizes.GetAt(nMsg - 1);
	
	return bSuccess;
}

BOOL CPop3Connection::GetMessageID(int nMsg, CString& sID)
{
	BOOL bSuccess = TRUE;
	
	//if we haven't executed the UIDL command then do it now
	if (!m_bUIDLRetrieved)
		bSuccess = UIDL();
	
	//nMsg must be in the correct range
	if(nMsg <= 0 || nMsg <= m_msgIDs.GetSize())
	{
		//m_sLastCommandResponse = _T("无效邮件");//<%IDS_Pop3_13%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_13%>");
		return FALSE;
	}
	
	//retrieve the size from the message size array
	sID = m_msgIDs.GetAt(nMsg - 1);
	
	return bSuccess;
}

BOOL CPop3Connection::List()
{
	//Must be connected to perform a "LIST"
	if(!m_bConnected)
	{
		//m_sLastCommandResponse = _T("通信未建立");//<%IDS_Pop3_14%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_14%>");
		return FALSE;
	}
	
	//if we haven't executed the STAT command then do it now
	int nNumberOfMails = m_nNumberOfMails;
	int nTotalMailSize;
	if (!m_bStatRetrieved)
	{
		if (!Statistics(nNumberOfMails, nTotalMailSize))
		{
			//m_sLastCommandResponse = _T("发送STAT命令失败");//<%IDS_Pop3_15%>
			m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_15%>");
			return FALSE;
		}
		else
			m_bStatRetrieved = TRUE;
	}
	
	//Send the LIST command
	char sBuf[10];
	strcpy(sBuf, "LIST\r\n");
	int nCmdLength = strlen(sBuf);
	if (!m_pop.Send(sBuf, nCmdLength))
	{
		TRACE(_T("Failed to send the LIST command to the POP3 server\n"));
		//m_sLastCommandResponse = _T("发送LIST命令失败");//<%IDS_Pop3_16%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_16%>");
		return FALSE;
	}
	//And check the response
	m_bListRetrieved = ReadListResponse(nNumberOfMails);
	return m_bListRetrieved;
}

BOOL CPop3Connection::UIDL()
{
	//Must be connected to perform a "UIDL"
	if(!m_bConnected)
	{
		//m_sLastCommandResponse = _T("通信未建立");//<%IDS_Pop3_17%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_17%>");
		return FALSE;
	}
	
	//if we haven't executed the STAT command then do it now
	int nNumberOfMails = m_nNumberOfMails;
	int nTotalMailSize;
	if (!m_bStatRetrieved)
	{
		if (!Statistics(nNumberOfMails, nTotalMailSize))
		{
			m_sLastCommandResponse = _T("发送STAT命令失败");//<%IDS_Pop3_18%>
			return FALSE;
		}
		else
			m_bStatRetrieved = TRUE;
	}
	
	//Send the UIDL command
	char sBuf[10];
	strcpy(sBuf, "UIDL\r\n");
	int nCmdLength = strlen(sBuf);
	if (!m_pop.Send(sBuf, nCmdLength))
	{
		TRACE(_T("Failed to send the UIDL command to the POP3 server\n"));
		//m_sLastCommandResponse = _T("发送UIDL命令失败");//<%IDS_Pop3_19%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_19%>");
		return FALSE;
	}
	//And check the response
	m_bUIDLRetrieved = ReadUIDLResponse(nNumberOfMails);
	return m_bUIDLRetrieved;
}

BOOL CPop3Connection::Reset()
{
	//Must be connected to perform a "RSET"
	if(!m_bConnected)
	{
		//m_sLastCommandResponse = _T("通信未建立");//<%IDS_Pop3_20%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_20%>");
		return FALSE;
	}
	
	//Send the RSET command
	char sBuf[10];
	strcpy(sBuf, "RSET\r\n");
	int nCmdLength = strlen(sBuf);
	if (!m_pop.Send(sBuf, nCmdLength))
	{
		TRACE(_T("Failed to send the RSET command to the POP3 server\n"));
		//m_sLastCommandResponse = _T("发送RSET命令失败");//<%IDS_Pop3_21%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_21%>");
		return FALSE;
	}
	
	//And check the command
	return ReadCommandResponse();
}

BOOL CPop3Connection::Noop()
{
	//Must be connected to perform a "NOOP"
	if(!m_bConnected)
	{
		//m_sLastCommandResponse = _T("通信未建立");//<%IDS_Pop3_22%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_22%>");
		return FALSE;
	}
	
	//Send the NOOP command
	char sBuf[10];
	strcpy(sBuf, "NOOP\r\n");
	int nCmdLength = strlen(sBuf);
	if (!m_pop.Send(sBuf, nCmdLength))
	{
		TRACE(_T("Failed to send the NOOP command to the POP3 server\n"));
		//m_sLastCommandResponse = _T("发送NOOP命令失败");//<%IDS_Pop3_23%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_23%>");
		return FALSE;
	}
	
	//And check the response
	return ReadCommandResponse();
}

BOOL CPop3Connection::Retrieve(int nMsg, CPop3Message& message)
{
	//Must be connected to retrieve a message
	if(!m_bConnected)
	{
		//m_sLastCommandResponse = _T("通信未建立");//<%IDS_Pop3_24%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_24%>");
		return FALSE;
	}
	
	//work out the size of the message to retrieve
	DWORD dwSize;
	if (GetMessageSize(nMsg, dwSize))
	{
		//Send the RETR command
		char sBuf[20];
		sprintf(sBuf, "RETR %d\r\n", nMsg);	
		int nCmdLength = strlen(sBuf);
		if (!m_pop.Send(sBuf, nCmdLength))
		{
			TRACE(_T("Failed to send the RETR command to the POP3 server\n"));
			//m_sLastCommandResponse = _T("发送RETR命令失败");//<%IDS_Pop3_25%>
			m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_25%>");
			return FALSE;
		}
		
		//And check the command
		return ReadReturnResponse(message, dwSize);
	}
	else
		return FALSE;
}

BOOL CPop3Connection::Top(int nMsg, CString &strMessage)
{
	//Must be connected to retrieve a message
	if(!m_bConnected)
	{
		//m_sLastCommandResponse = _T("通信未建立");//<%IDS_Pop3_26%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_26%>");//<%IDS_Pop3_26%>
		return FALSE;
	}
	
	//Send the RETR command
	char sBuf[20];
	sprintf(sBuf, "TOP %d 50\r\n", nMsg);	
	int nCmdLength = strlen(sBuf);
	if (!m_pop.Send(sBuf, nCmdLength))
	{
		TRACE(_T("Failed to send the RETR command to the POP3 server\n"));
		//m_sLastCommandResponse = _T("发送TOP命令失败");//<%IDS_Pop3_27%>
		m_sLastCommandResponse = FuncGetStringFromIDS("<%IDS_Pop3_27%>");
		return FALSE;
	}
		
	//And check the command
	LPSTR pszOverFlowBuffer = NULL;
	char szBuffer[256] = {0};
	if(!ReadResponse(szBuffer, 70, "\r\n.\r\n", &pszOverFlowBuffer))
	{
		if (pszOverFlowBuffer)
			delete [] pszOverFlowBuffer;
		
		TRACE(_T("Error retrieving the TOP response"));
		return FALSE;
	}

	if(!pszOverFlowBuffer)
		return FALSE;

	strMessage = pszOverFlowBuffer;
	delete pszOverFlowBuffer;
	return TRUE;
}

BOOL CPop3Connection::GetMessageHeader(int nMsg, CPop3Message& message)
{
	// Must be connected to retrieve a message
	if(!m_bConnected)
	{
		//m_sLastCommandResponse = _T("通信未建立");//<%IDS_Pop3_28%>
		m_sLastCommandResponse = FuncGetStringFromIDS("<%IDS_Pop3_28%>");
		return FALSE;
	}
	
	// make sure the message actually exists
	DWORD dwSize;
	if (GetMessageSize(nMsg, dwSize))
	{
		// Send the TOP command
		char sBuf[16];
		sprintf(sBuf, "TOP %d 0\r\n", nMsg);
		int nCmdLength = strlen(sBuf);
		if (!m_pop.Send(sBuf, nCmdLength))
		{
			TRACE(_T("Failed to send the TOP command to the POP3 server\n"));
			//m_sLastCommandResponse = _T("发送TOP命令失败");//<%IDS_Pop3_29%>
			m_sLastCommandResponse = FuncGetStringFromIDS("<%IDS_Pop3_29%>");
			return FALSE;
		}
		
		// And check the command
		return ReadReturnResponse(message, dwSize);
	}
	else
		return FALSE;
}

BOOL CPop3Connection::ReadCommandResponse()
{
	LPSTR pszOverFlowBuffer = NULL;
	char sBuf[1000] = {0};
	BOOL bSuccess = ReadResponse(sBuf, 1000, "\r\n", &pszOverFlowBuffer);
	if (pszOverFlowBuffer)
		delete [] pszOverFlowBuffer;
	
	return bSuccess;
}

LPSTR CPop3Connection::GetFirstCharInResponse(LPSTR pszData) const
{
	while ((*pszData != '\n') && *pszData)
		++pszData;
	
	//skip over the "\n" onto the next line
	if (*pszData)
		++pszData;
	
	return pszData;
}

BOOL CPop3Connection::ReadResponse(LPSTR pszBuffer, int nInitialBufSize, LPSTR pszTerminator, LPSTR* ppszOverFlowBuffer, int nGrowBy)
{
	//must have been created first
	if(!m_bConnected)
	{
		//m_sLastCommandResponse = _T("通信未建立");//<%IDS_Pop3_30%>
		m_sLastCommandResponse = FuncGetStringFromIDS("<%IDS_Pop3_30%>");
		return FALSE;
	}
	
	//The local variables which will receive the data
	LPSTR pszRecvBuffer = pszBuffer;
	int nBufSize = nInitialBufSize;
	
	//retrieve the reponse using until we
	//get the terminator or a timeout occurs
	BOOL bFoundTerminator = FALSE;
	int nReceived = 0;
	DWORD dwStartTicks = ::GetTickCount();
	while (!bFoundTerminator)
	{
		//Has the timeout occured
		if ((::GetTickCount() - dwStartTicks) >	m_dwTimeout)
		{
			pszRecvBuffer[nReceived] = '\0';
			SetLastError(WSAETIMEDOUT);
			m_sLastCommandResponse = pszRecvBuffer; //Hive away the last command reponse
			return FALSE;
		}
		
		//check the socket for readability
		BOOL bReadible;
		if (!m_pop.IsReadible(bReadible))
		{
			pszRecvBuffer[nReceived] = '\0';
			m_sLastCommandResponse = pszRecvBuffer; //Hive away the last command reponse
			return FALSE;
		}
		else if (!bReadible) //no data to receive, just loop around
		{
			Sleep(250); //Sleep for a while before we loop around again
			continue;
		}
		
		//receive the data from the socket
		int nBufRemaining = nBufSize-nReceived-1; //Allows allow one space for the NULL terminator
		if (nBufRemaining<0)
			nBufRemaining = 0;
		int nData = m_pop.Receive(pszRecvBuffer+nReceived, nBufRemaining);
		
		//Reset the idle timeout if data was received
		if (nData)
		{
			dwStartTicks = ::GetTickCount();
			
			//Increment the count of data received
			nReceived += nData;							   
		}
		
		//If an error occurred receiving the data
		if (nData == SOCKET_ERROR)
		{
			//NULL terminate the data received
			if (pszRecvBuffer)
				pszBuffer[nReceived] = '\0';
			
			m_sLastCommandResponse = pszRecvBuffer; //Hive away the last command reponse
			return FALSE; 
		}
		else
		{
			//NULL terminate the data received
			if (pszRecvBuffer)
				pszRecvBuffer[nReceived] = '\0';
			
			if (nBufRemaining-nData == 0) //No space left in the current buffer
			{
				//Allocate the new receive buffer
				nBufSize += nGrowBy; //Grow the buffer by the specified amount
				LPSTR pszNewBuf = new char[nBufSize];
				
				//copy the old contents over to the new buffer and assign 
				//the new buffer to the local variable used for retreiving 
				//from the socket
				if (pszRecvBuffer)
					strcpy(pszNewBuf, pszRecvBuffer);
				pszRecvBuffer = pszNewBuf;
				
				//delete the old buffer if it was allocated
				if (*ppszOverFlowBuffer)
					delete [] *ppszOverFlowBuffer;
				
				//Remember the overflow buffer for the next time around
				*ppszOverFlowBuffer = pszNewBuf;        
			}
		}
		
		//Check to see if the terminator character(s) have been found
		bFoundTerminator = (strstr(pszRecvBuffer, pszTerminator) != NULL);
	}
	
	//Remove the terminator from the response data
	pszRecvBuffer[nReceived - strlen(pszTerminator)] = '\0';
	
	//determine if the response is an error
	BOOL bSuccess = (strnicmp(pszRecvBuffer,"+OK", 3) == 0);
	
	if (!bSuccess)
	{
		SetLastError(WSAEPROTONOSUPPORT);
		m_sLastCommandResponse = pszRecvBuffer; //Hive away the last command reponse
	}
	
	return bSuccess;
}

BOOL CPop3Connection::ReadReturnResponse(CPop3Message& message, DWORD dwSize)
{
	//Must be connected to perform a "RETR"
	if(!m_bConnected)
	{
		//m_sLastCommandResponse = _T("通信未建立");//<%IDS_Pop3_31%>
		m_sLastCommandResponse = FuncGetStringFromIDS("<%IDS_Pop3_31%>");//<%IDS_Pop3_31%>
		return FALSE;
	}
	
	//Retrieve the message body
	LPSTR pszOverFlowBuffer = NULL;
	int nSize = dwSize + 100;
	char* sBuf = new char[nSize];
	char* sMessageBuf = sBuf;
	if (!ReadResponse(sBuf, nSize, "\r\n.\r\n", &pszOverFlowBuffer, 32000))
	{
		delete [] sBuf;
		if (pszOverFlowBuffer)
			delete [] pszOverFlowBuffer;
		
		TRACE(_T("Error retrieving the RETR response"));
		return FALSE;
	}
	if (pszOverFlowBuffer)
		sMessageBuf = pszOverFlowBuffer;
	
	//determine if the response is an error
	if (strnicmp(sMessageBuf,"+OK", 3) != 0)
	{
		delete [] sBuf;
		if (pszOverFlowBuffer)
			delete [] pszOverFlowBuffer;
		
		SetLastError(WSAEPROTONOSUPPORT);
		TRACE(_T("POP3 server did not respond correctly to the RETR response\n"));
		return FALSE;
	}
	else
	{  
		//remove the first line which contains the +OK from the message
		char* pszFirst = GetFirstCharInResponse(sMessageBuf);
		VERIFY(pszFirst);
		
		//transfer the message contents to the message class
		int nMessageSize = sMessageBuf - pszFirst + strlen(sMessageBuf);
		
		// Do we already have memory allocated? If so, destroy it!
		if (message.m_pszMessage)
		{
			delete [] message.m_pszMessage;
			message.m_pszMessage = NULL;
		}
		
		message.m_pszMessage = new char[nMessageSize + 1];
		memcpy(message.m_pszMessage, pszFirst, nMessageSize);
		message.m_pszMessage[nMessageSize] = '\0';
	}
	delete [] sBuf;
	if (pszOverFlowBuffer)
		delete [] pszOverFlowBuffer;
	
	return TRUE; 
}

BOOL CPop3Connection::ReadListResponse(int nNumberOfMails)
{
	//Must be connected to perform a "LIST"
	if(!m_bConnected)
	{
		//m_sLastCommandResponse = _T("通信未建立");//<%IDS_Pop3_32%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_32%>");//<%IDS_Pop3_32%>
		return FALSE;
	}
	
	//retrieve the reponse
	LPSTR pszOverFlowBuffer = NULL;
	int nSize = 14 * nNumberOfMails + 100;
	char* sBuf = new char[nSize];
	char* sMessageBuf = sBuf;
	if (!ReadResponse(sBuf, nSize, "\r\n.\r\n", &pszOverFlowBuffer))
	{
		delete [] sBuf;
		if (pszOverFlowBuffer)
			delete [] pszOverFlowBuffer;
		
		TRACE(_T("Error retrieving the LIST response from the POP3 server"));
		return FALSE;
	}
	if (pszOverFlowBuffer)
		sMessageBuf = pszOverFlowBuffer;
	
	//determine if the response is an error
	if (strnicmp(sMessageBuf,"+OK", 3) != 0)
	{
		delete [] sBuf;
		if (pszOverFlowBuffer)
			delete [] pszOverFlowBuffer;
		
		SetLastError(WSAEPROTONOSUPPORT);
		TRACE(_T("POP3 server did not respond correctly to the LIST response\n"));
		return FALSE;
	}
	else
	{  
		//Retrieve the message sizes and put them
		//into the m_msgSizes array
		m_msgSizes.RemoveAll();
		m_msgSizes.SetSize(0, nNumberOfMails);
		
		//then parse the LIST response
		char* pszSize = GetFirstCharInResponse(sMessageBuf);
		VERIFY(pszSize);
		for (; *pszSize != '.'; pszSize++)
			if (*pszSize == '\t' || *pszSize == ' ')
				m_msgSizes.Add(atoi(pszSize));
	}
	delete [] sBuf;
	if (pszOverFlowBuffer)
		delete [] pszOverFlowBuffer;
	
	return TRUE; 
}

BOOL CPop3Connection::ReadUIDLResponse(int nNumberOfMails)
{
	//Must be connected to perform a "LIST"
	if(!m_bConnected)
	{
		//m_sLastCommandResponse = _T("通信未建立");//<%IDS_Pop3_33%>
		m_sLastCommandResponse = ::FuncGetStringFromIDS("<%IDS_Pop3_33%>");//<%IDS_Pop3_33%>
		return FALSE;
	}
	
	//retrieve the reponse
	LPSTR pszOverFlowBuffer = NULL;
	int nSize = 14 * nNumberOfMails + 100;
	char* sBuf = new char[nSize];
	char* sMessageBuf = sBuf;
	if (!ReadResponse(sBuf, nSize, "\r\n.\r\n", &pszOverFlowBuffer))
	{
		delete [] sBuf;
		if (pszOverFlowBuffer)
			delete [] pszOverFlowBuffer;
		
		TRACE(_T("Error retrieving the UIDL response from the POP3 server"));
		return FALSE;
	}
	if (pszOverFlowBuffer)
		sMessageBuf = pszOverFlowBuffer;
	
	//determine if the response is an error
	if (strnicmp(sMessageBuf,"+OK", 3) != 0)
	{
		delete [] sBuf;
		if (pszOverFlowBuffer)
			delete [] pszOverFlowBuffer;
		
		SetLastError(WSAEPROTONOSUPPORT);
		TRACE(_T("POP3 server did not respond correctly to the UIDL response\n"));
		return FALSE;
	}
	else
	{  
		//Retrieve the message ID's and put them
		//into the m_msgIDs array
		m_msgIDs.RemoveAll();
		m_msgIDs.SetSize(0, nNumberOfMails);
		
		//then parse the UIDL response
		char* pszSize = GetFirstCharInResponse(sMessageBuf);
		VERIFY(pszSize);
		for (; *pszSize != '.'; pszSize++)
		{
			char* pszBegin = pszSize;
			while (*pszSize != '\n' && *pszSize != '.')
			{
				++pszSize;
			}
			if (*pszSize == '.')
				continue;
			
			char sMsg[15];
			char sID[1000];
			*pszSize = '\0';
			sscanf(pszBegin, "%s %s", sMsg, sID);
			
			m_msgIDs.Add(CString(sID));
		}
	}
	delete [] sBuf;
	if (pszOverFlowBuffer)
		delete [] pszOverFlowBuffer;
	
	return TRUE; 
}

BOOL CPop3Connection::ReadStatResponse(int& nNumberOfMails, int& nTotalMailSize)
{
	//Must be connected to perform a "STAT"
	if(!m_bConnected)
		return FALSE;
	
	//retrieve the reponse
	LPSTR pszOverFlowBuffer = NULL;
	char sBuf[100];
	char* sMessageBuf = sBuf;
	if (!ReadResponse(sBuf, 100, "\r\n", &pszOverFlowBuffer))
	{
		if (pszOverFlowBuffer)
			delete [] pszOverFlowBuffer;
		
		TRACE(_T("Error retrieving the STAT response from the POP3 server"));
		return FALSE;
	}
	if (pszOverFlowBuffer)
		sMessageBuf = pszOverFlowBuffer;
	
	//determine if the response is an error
	if (strncmp(sMessageBuf,"+OK", 3) != 0)
	{
		TRACE(_T("POP3 server did not respond correctly to the STAT response\n"));
		return FALSE;
	}
	else
	{                                          
		//Parse out the Number of Mails and Total mail size values
		BOOL bGetNumber = TRUE;
		for (char* pszNum=sMessageBuf; *pszNum!='\0'; pszNum++)
		{
			if (*pszNum=='\t' || *pszNum==' ')
			{						
				if (bGetNumber)
				{
					nNumberOfMails = atoi(pszNum);
					m_nNumberOfMails = nNumberOfMails;
					bGetNumber = FALSE;
				}
				else
				{
					nTotalMailSize = atoi(pszNum);
					return TRUE;
				}
			}
		}
	}
	if (pszOverFlowBuffer)
		delete [] pszOverFlowBuffer;
	
	return FALSE; 
}

CString CPop3Connection::GetLastError()
{
	return m_sLastCommandResponse;
}