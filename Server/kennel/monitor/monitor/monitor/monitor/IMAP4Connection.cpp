// IMAP4Connection.cpp: implementation of the CIMAP4Connection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mail.h"
#include "IMAP4Connection.h"

#include "..\..\base\funcGeneral.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIMAP4Connection::CIMAP4Connection()
{
	m_nCmdIndex = 0;
	m_nState = NON_AUTHENTICATED;

	m_bConnected = FALSE;
#ifdef _DEBUG
	m_dwTimeout = 60000; //default timeout of 60 seconds when debugging
#else
	m_dwTimeout = 2000;  //default timeout of 2 seconds for normal release code
#endif
}

CIMAP4Connection::~CIMAP4Connection()
{
	if (m_bConnected)
		Disconnect();
}

BOOL CIMAP4Connection::Connect(LPCTSTR pszHostName, LPCTSTR pszUser, LPCTSTR pszPassword, int nPort)
{
	//For correct operation of the T2A macro, see MFC Tech Note 59
	USES_CONVERSION;

	//Create the socket
	if (!m_imap.Create())
	{
		m_sLastCommandResponse = FuncGetStringFromIDS("<%IDS_IMAP4Connection_1%>");//<%IDS_IMAP4Connection_1%>
		return FALSE;
	}

	//Connect to the IMAP4 Host
	if (!m_imap.Connect(pszHostName, nPort))
	{
		m_sLastCommandResponse = FuncGetStringFromIDS("<%IDS_IMAP4Connection_2%>");//<%IDS_IMAP4Connection_2%>
		return FALSE;
	}
	else
	{
		//We're now connected !!
		m_bConnected = TRUE;
		
		//check the response
		if (!ReadCommandResponse(_T("*")))
		{
			m_sLastCommandResponse = FuncGetStringFromIDS("<%IDS_IMAP4Connection_3%>");//<%IDS_IMAP4Connection_3%>
			Disconnect();
			return FALSE;
		}

		m_nState = NON_AUTHENTICATED;

		BOOL bResult = Login(pszUser, pszPassword);
		if(!bResult)
		{
			m_sLastCommandResponse = FuncGetStringFromIDS("<%IDS_IMAP4Connection_4%>");//<%IDS_IMAP4Connection_4%>
			Disconnect();
			return FALSE;
		}

		m_nState = AUTHENTICATED;

		return TRUE;
	}
}

BOOL CIMAP4Connection::ReadCommandResponse(CString strTerminal)
{
	LPSTR pszOverFlowBuffer = NULL;
	char sBuf[1000];

	BOOL bSuccess = ReadResponse(sBuf, 1000, strTerminal.GetBuffer(strTerminal.GetLength()), &pszOverFlowBuffer);
	if (pszOverFlowBuffer)
		delete [] pszOverFlowBuffer;
	
	return bSuccess;
}

BOOL CIMAP4Connection::ReadResponse(LPSTR pszBuffer, int nInitialBufSize, LPSTR pszTerminator, LPSTR* ppszOverFlowBuffer, int nGrowBy)
{
	//must have been created first
	if(!m_bConnected)
		return FALSE;
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
		if (!m_imap.IsReadible(bReadible))
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
		int nData = m_imap.Receive(pszRecvBuffer+nReceived, nBufRemaining);
		
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
	BOOL bSuccess = FALSE;
	char sOK[20] = {0};
	sprintf(sOK, "%s OK", pszTerminator);
	bSuccess = (strstr(pszRecvBuffer, sOK) != NULL);
	
	if (!bSuccess)
	{
		SetLastError(WSAEPROTONOSUPPORT);
		m_sLastCommandResponse = pszRecvBuffer; //Hive away the last command reponse
	}
	
	m_nState = AUTHENTICATED;
	return bSuccess;
}

BOOL CIMAP4Connection::Disconnect()
{   
	BOOL bSuccess = FALSE;      
	
	//disconnect from the IMAP4 server if connected 
	if (m_bConnected)
	{
		char sBuf[20] = {0};
		CMDLINE(sBuf, m_nCmdIndex, "LOGOUT");
		int nCmdLength = strlen(sBuf);
		if (!m_imap.Send(sBuf, nCmdLength))
			TRACE(_T("Failed to send the QUIT command to the POP3 server\n"));
		
		//Check the reponse
		CMDINDEX(sBuf, m_nCmdIndex);
		bSuccess = ReadCommandResponse(sBuf);
		
		//Reset all the state variables
		m_bConnected = FALSE;
	}
	else
		TRACE(_T("CIMAP4Connection, Already disconnected\n"));
	
	//free up our socket
	m_imap.Close();
	
	return bSuccess;
}

BOOL CIMAP4Connection::Noop()
{
	//Must be connected to perform a "NOOP"
	if(!m_bConnected)
		return FALSE;
	
	//Send the NOOP command
	char sBuf[10];
	CMDLINE(sBuf, m_nCmdIndex, "NOOP");
	int nCmdLength = strlen(sBuf);
	if (!m_imap.Send(sBuf, nCmdLength))
	{
		m_sLastCommandResponse = FuncGetStringFromIDS("<%IDS_IMAP4Connection_5%>");//<%IDS_IMAP4Connection_5%>
		return FALSE;
	}
	
	//And check the response
	CMDINDEX(sBuf, m_nCmdIndex);
	return ReadCommandResponse(sBuf);
}

BOOL CIMAP4Connection::Capability()
{
	//Must be connected to perform a "NOOP"
	if(!m_bConnected)
		return FALSE;
	
	//Send the NOOP command
	char sBuf[10];
	CMDLINE(sBuf, m_nCmdIndex, "CAPABILITY");
	int nCmdLength = strlen(sBuf);
	if (!m_imap.Send(sBuf, nCmdLength))
	{
		m_sLastCommandResponse =FuncGetStringFromIDS("<%IDS_IMAP4Connection_6%>");//<%IDS_IMAP4Connection_6%>
		return FALSE;
	}
	
	//And check the response
	CMDINDEX(sBuf, m_nCmdIndex);
	return ReadCommandResponse(sBuf);
}

BOOL CIMAP4Connection::Logout()
{
	if(!m_bConnected)
		return FALSE;
	
	char sBuf[10];
	CMDLINE(sBuf, m_nCmdIndex, "LOGOUT");
	int nCmdLength = strlen(sBuf);
	if (!m_imap.Send(sBuf, nCmdLength))
	{
		m_sLastCommandResponse = FuncGetStringFromIDS("<%IDS_IMAP4Connection_7%>");//<%IDS_IMAP4Connection_7%>
		return FALSE;
	}
	
	//And check the response
	CMDINDEX(sBuf, m_nCmdIndex);
	return ReadCommandResponse(sBuf);
}

BOOL CIMAP4Connection::Login(LPCTSTR pszUid, LPCTSTR pszPwd)
{
	if(!m_bConnected)
		return FALSE;
	if(m_nState != NON_AUTHENTICATED)
		return FALSE;
	
	char sBuf[64];
	CString strCmd = _T("LOGIN ");
	strCmd += pszUid;
	strCmd += " ";
	strCmd += pszPwd;
	CMDLINE(sBuf, m_nCmdIndex, strCmd.GetBuffer(strCmd.GetLength()));
	int nCmdLength = strlen(sBuf);
	if (!m_imap.Send(sBuf, nCmdLength))
	{
		TRACE(_T("Failed to send the NOOP command to the IMAP4 server\n"));
		return FALSE;
	}
	
	//And check the response
	CMDINDEX(sBuf, m_nCmdIndex);
	return ReadCommandResponse(sBuf);
}

BOOL CIMAP4Connection::Select(CString strBoxName)
{
	if(!m_bConnected)
		return FALSE;

	if(m_nState != AUTHENTICATED)
		return FALSE;
	
	char sBuf[64];
	CString strCmd = _T("SELECT ");
	strCmd += strBoxName;
	CMDLINE(sBuf, m_nCmdIndex, strCmd.GetBuffer(strCmd.GetLength()));
	int nCmdLength = strlen(sBuf);
	if (!m_imap.Send(sBuf, nCmdLength))
	{
		m_sLastCommandResponse = FuncGetStringFromIDS("<%IDS_IMAP4Connection_8%>");//<%IDS_IMAP4Connection_8%>
		return FALSE;
	}
	
	//And check the response
	CMDINDEX(sBuf, m_nCmdIndex);
	BOOL bResult = ReadCommandResponse(sBuf);
	if(!bResult)
	{
		m_sLastCommandResponse = FuncGetStringFromIDS("<%IDS_IMAP4Connection_9%>");//<%IDS_IMAP4Connection_9%>
		return FALSE;
	}

	m_nState = SELECTED;

	return TRUE;
}

BOOL CIMAP4Connection::Retrieve(int nMsg, CIMAP4Message& message)
{
	if(!m_bConnected)
		return FALSE;

	if(m_nState != SELECTED)
		return FALSE;

	DWORD dwSize;
	if (GetMessageSize(nMsg, dwSize))
	{
		char sBuf[20];
		char sCmd[64];
		sprintf(sBuf, "FETCH %d BODY[TEXT]", nMsg);
		CMDLINE(sCmd, m_nCmdIndex, sBuf);
		int nCmdLength = strlen(sCmd);
		if (!m_imap.Send(sCmd, nCmdLength))
		{
			TRACE(_T("Failed to send the RETR command to the POP3 server\n"));
			return FALSE;
		}
		
		//And check the command
		CMDINDEX(sBuf, m_nCmdIndex);
		return ReadReturnResponse(message, dwSize, sBuf);
	}
	else
		return FALSE;
}

BOOL CIMAP4Connection::Delete(int nMsg)
{
	return TRUE;
}

BOOL CIMAP4Connection::ReadReturnResponse(CIMAP4Message& message, DWORD dwSize, LPTSTR pszTerminal)
{
	if(!m_bConnected)
		return FALSE;
	
	LPSTR pszOverFlowBuffer = NULL;
	int nSize = dwSize + 100;
	char* sBuf = new char[nSize];
	char* sMessageBuf = sBuf;
	if (!ReadResponse(sBuf, nSize, pszTerminal, &pszOverFlowBuffer, 32000))
	{
		delete [] sBuf;
		if (pszOverFlowBuffer)
			delete [] pszOverFlowBuffer;
		
		TRACE(_T("Error retrieving the RETR response"));
		return FALSE;
	}
	if (pszOverFlowBuffer)
		sMessageBuf = pszOverFlowBuffer;
	
	char *ca = NULL;
	char sMatch[64];
	sprintf(sMatch, "%s OK", pszTerminal);
	if((ca = strstr(sMessageBuf, sMatch)) == NULL)
	{
		delete [] sBuf;
		if (pszOverFlowBuffer)
			delete [] pszOverFlowBuffer;
		
		return FALSE;
	}
	else
	{  
		sMessageBuf[ca - sMessageBuf] = '\0';
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

BOOL CIMAP4Connection::GetMessageSize(int nMsg, DWORD& dwSize)
{
	BOOL bSuccess = TRUE;

	char	sBuf[64];
	char	sCmd[64];
	sprintf(sBuf, "FETCH %d RFC822.SIZE", nMsg);
	CMDLINE(sCmd, m_nCmdIndex, sBuf);
	int nCmdLength = strlen(sCmd);

	if (!m_imap.Send(sCmd, nCmdLength))
	{
		TRACE(_T("Failed to send the FETCH command to the IMAP4 server\n"));
		return FALSE;
	}

	LPSTR pszOverFlowBuffer = NULL;
	char sBuff[1000];
	char sTer[20];
	char* sMessageBuf = sBuff;
	CMDINDEX(sTer, m_nCmdIndex);
	if (!ReadResponse(sBuff, 1000, sTer, &pszOverFlowBuffer, 32000))
	{
		if (pszOverFlowBuffer)
			delete [] pszOverFlowBuffer;
		
		TRACE(_T("Error retrieving the RETR response"));
		return FALSE;
	}
	if (pszOverFlowBuffer)
		sMessageBuf = pszOverFlowBuffer;

	char sMatch[64];
	sprintf(sMatch, "%s OK", sTer);
	if(strstr(sMessageBuf, sMatch) == NULL)
	{
		if (pszOverFlowBuffer)
			delete [] pszOverFlowBuffer;
		
		return FALSE;
	}
	else
	{  
		char *ca = strstr(sMessageBuf, "RFC822.SIZE");
		if(ca) dwSize = atol(ca + 11);
		return TRUE;
	}
}

LPSTR CIMAP4Connection::GetFirstCharInResponse(LPSTR pszData) const
{
	while ((*pszData != '\n') && *pszData)
		++pszData;
	
	//skip over the "\n" onto the next line
	if (*pszData)
		++pszData;
	
	return pszData;
}

CString	CIMAP4Connection::GetLastError()
{
	return m_sLastCommandResponse;
}