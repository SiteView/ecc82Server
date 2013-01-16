// IMAP4Connection.cpp: implementation of the CIMAP4Connection class.
//
//////////////////////////////////////////////////////////////////////
#include "IMAP4Connection.h"
#include "../base/SVDefines.h"


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

	m_bConnected = false;
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

bool CIMAP4Connection::Connect(const char* pszHostName, const char* pszUser, const char* pszPassword, int nPort)
{
	//For correct operation of the T2A macro, see MFC Tech Note 59
	//USES_CONVERSION;

	//Create the socket
	if (!m_imap.Create())
	{
		m_sLastCommandResponse = GetResourceValue("IDS_IMAP4_CONNECTION_INIT_FAILED");//"IMAP ³õÊ¼»¯Ê§°Ü";//IDS_IMAP4_CONNECTION_INIT_FAILED
		return false;
	}

	//Connect to the IMAP4 Host
	if (!m_imap.Connect(pszHostName, nPort))
	{
		m_sLastCommandResponse = GetResourceValue("IDS_IMAP4_CONNECTION_CONNECT_FAILED");//"IMAP Á¬½Ó·þÎñÆ÷Ê§°Ü£¬Çë¼ì²é·þÎñÆ÷µØÖ·ºÍ¶Ë¿ÚºÅ¡£";//IDS_IMAP4_CONNECTION_CONNECT_FAILED
		return false;
	}
	else
	{
		//We're now connected !!
		m_bConnected = true;
		
		//check the response
		if (!ReadCommandResponse(("*")))
		{
            m_sLastCommandResponse = GetResourceValue("IDS_IMAP4_CONNECT_READ_RESPONSE_FAILED");//"IMAP ¶ÁÈ¡»ØÓ¦Êý¾ÝÊ§°Ü";//IDS_IMAP4_CONNECT_READ_RESPONSE_FAILED
			Disconnect();
			return false;
		}

		m_nState = NON_AUTHENTICATED;

		bool bResult = Login(pszUser, pszPassword);
		if(!bResult)
		{
			m_sLastCommandResponse = GetResourceValue("IDS_IMAP4_CONNECTION_LOGIN_FAILED");//"IMAP µÇÂ½ÓÃ»§»òÕßÃÜÂë´íÎó£¬µÇÂ½Ê§°Ü£¡";//IDS_IMAP4_CONNECTION_LOGIN_FAILED
			Disconnect();
			return false;
		}

		m_nState = AUTHENTICATED;

		return true;
	}
}

bool CIMAP4Connection::ReadCommandResponse(string strTerminal)
{
	char* pszOverFlowBuffer = NULL;
	char sBuf[1000];

	bool bSuccess = ReadResponse(sBuf, 1000, (strTerminal.c_str()), &pszOverFlowBuffer);
	if (pszOverFlowBuffer)
		delete [] pszOverFlowBuffer;
	
    //m_szResponse = sBuf;
	return bSuccess;
}

bool CIMAP4Connection::ReadResponse(char* pszBuffer, int nInitialBufSize, const char* pszTerminator, char** ppszOverFlowBuffer, int nGrowBy)
{
	//must have been created first
	if(!m_bConnected)
		return false;
	//The local variables which will receive the data
	char* pszRecvBuffer = pszBuffer;
	int nBufSize = nInitialBufSize;
	
	//retrieve the reponse using until we
	//get the terminator or a timeout occurs
	bool bFoundTerminator = false;
	int nReceived = 0;
	unsigned long dwStartTicks = ::GetTickCount();
	while (!bFoundTerminator)
	{
		//Has the timeout occured
		if ((::GetTickCount() - dwStartTicks) >	m_dwTimeout)
		{
			pszRecvBuffer[nReceived] = '\0';
			SetLastError(WSAETIMEDOUT);
			m_sLastCommandResponse = pszRecvBuffer; //Hive away the last command reponse
			return false;
		}
		
		//check the socket for readability
		bool bReadible;
		if (!m_imap.IsReadible(bReadible))
		{
			pszRecvBuffer[nReceived] = '\0';
			m_sLastCommandResponse = pszRecvBuffer; //Hive away the last command reponse
			return false;
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
			return false; 
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
				char* pszNewBuf = new char[nBufSize];
				
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
	bool bSuccess = false;
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

bool CIMAP4Connection::Disconnect()
{   
	bool bSuccess = false;      
	
	//disconnect from the IMAP4 server if connected 
	if (m_bConnected)
	{
		char sBuf[20] = {0};
		CMDLINE(sBuf, m_nCmdIndex, "LOGOUT");
		int nCmdLength = static_cast<int>(strlen(sBuf));
		if (!m_imap.Send(sBuf, nCmdLength))
			cout << "Failed to send the QUIT command to the POP3 server\n" << endl;
		
		//Check the reponse
		CMDINDEX(sBuf, m_nCmdIndex);
		bSuccess = ReadCommandResponse(sBuf);
		
		//Reset all the state variables
		m_bConnected = false;
	}
	else
		cout << "CIMAP4Connection, Already disconnected\n" << endl;;
	
	//free up our socket
	m_imap.Close();
	
	return bSuccess;
}

bool CIMAP4Connection::Noop()
{
	//Must be connected to perform a "NOOP"
	if(!m_bConnected)
		return false;
	
	//Send the NOOP command
	char sBuf[10];
	CMDLINE(sBuf, m_nCmdIndex, "NOOP");
	int nCmdLength = static_cast<int>(strlen(sBuf));
	if (!m_imap.Send(sBuf, nCmdLength))
	{
		m_sLastCommandResponse = GetResourceValue("IDS_IMAP4_CONNECTION_SEND_NOOP_FAILED");//"IMAP  ·¢ËÍNOOPÃüÁî×ÖÊ§°Ü";//IDS_IMAP4_CONNECTION_SEND_NOOP_FAILED
		return false;
	}
	
	//And check the response
	CMDINDEX(sBuf, m_nCmdIndex);
	return ReadCommandResponse(sBuf);
}

bool CIMAP4Connection::Capability()
{
	//Must be connected to perform a "NOOP"
	if(!m_bConnected)
		return false;
	
	//Send the NOOP command
	char sBuf[10];
	CMDLINE(sBuf, m_nCmdIndex, "CAPABILITY");
	int nCmdLength = static_cast<int>(strlen(sBuf));
	if (!m_imap.Send(sBuf, nCmdLength))
	{
		m_sLastCommandResponse = GetResourceValue("IDS_IMAP4_CONNECTION_SEND_CAPABILITY_FAILED");//"IMAP ·¢ËÍCAPABILITYÃüÁî×ÖÊ§°Ü";//IDS_IMAP4_CONNECTION_SEND_CAPABILITY_FAILED
		return false;
	}
	
	//And check the response
	CMDINDEX(sBuf, m_nCmdIndex);
	return ReadCommandResponse(sBuf);
}

bool CIMAP4Connection::Logout()
{
	if(!m_bConnected)
		return false;
	
	char sBuf[10];
	CMDLINE(sBuf, m_nCmdIndex, "LOGOUT");
	int nCmdLength = static_cast<int>(strlen(sBuf));
	if (!m_imap.Send(sBuf, nCmdLength))
	{
		m_sLastCommandResponse = GetResourceValue("IDS_IMAP4_CONNECTION_SEND_LOGOUT_FAILED");//"IMAP ·¢ËÍLOGOUTÃüÁî×ÖÊ§°Ü";//IDS_IMAP4_CONNECTION_SEND_LOGOUT_FAILED
		return false;
	}
	
	//And check the response
	CMDINDEX(sBuf, m_nCmdIndex);
	return ReadCommandResponse(sBuf);
}

bool CIMAP4Connection::Login(const char* pszUid, const char* pszPwd)
{
	if(!m_bConnected)
		return false;
	if(m_nState != NON_AUTHENTICATED)
		return false;
	
	char sBuf[64];
	string strCmd = ("LOGIN ");
	strCmd += pszUid;
	strCmd += " ";
	strCmd += pszPwd;
	CMDLINE(sBuf, m_nCmdIndex, strCmd.c_str());
	int nCmdLength = static_cast<int>(strlen(sBuf));
	if (!m_imap.Send(sBuf, nCmdLength))
	{
		cout << "Failed to send the NOOP command to the IMAP4 server\n" << endl;
		return false;
	}
	
	//And check the response
	CMDINDEX(sBuf, m_nCmdIndex);
	return ReadCommandResponse(sBuf);
}

bool CIMAP4Connection::Select(string strBoxName)
{
	if(!m_bConnected)
		return false;

	if(m_nState != AUTHENTICATED)
		return false;
	
	char sBuf[64];
	string strCmd = ("SELECT ");
	strCmd += strBoxName;
	CMDLINE(sBuf, m_nCmdIndex, strCmd.c_str());
	int nCmdLength = static_cast<int>(strlen(sBuf));
	if (!m_imap.Send(sBuf, nCmdLength))
	{
		m_sLastCommandResponse = GetResourceValue("IDS_IMAP4_CONNECTION_SEND_SELECT_FAILED");//"IMAP ·¢ËÍSELECTÃüÁî×ÖÊ§°Ü";//IDS_IMAP4_CONNECTION_SEND_SELECT_FAILED
		return false;
	}
	
	//And check the response
	CMDINDEX(sBuf, m_nCmdIndex);
	bool bResult = ReadCommandResponse(sBuf);
	if(!bResult)
	{
		m_sLastCommandResponse = GetResourceValue("IDS_IMAP4_CONNECT_READ_RESPONSE_FAILED");//"IMAP ¶ÁÈ¡»ØÓ¦Êý¾ÝÊ§°Ü";//IDS_IMAP4_CONNECT_READ_RESPONSE_FAILED
		return false;
	}

	m_nState = SELECTED;

	return true;
}

bool CIMAP4Connection::Retrieve(int nMsg, CIMAP4Message& message)
{
	if(!m_bConnected)
		return false;

	if(m_nState != SELECTED)
		return false;

	unsigned long dwSize;
	if (GetMessageSize(nMsg, dwSize))
	{
		char sBuf[20];
		char sCmd[64];
		sprintf(sBuf, "FETCH %d BODY[TEXT]", nMsg);
		CMDLINE(sCmd, m_nCmdIndex, sBuf);
		int nCmdLength = static_cast<int>(strlen(sCmd));
		if (!m_imap.Send(sCmd, nCmdLength))
		{
			cout << "Failed to send the RETR command to the POP3 server\n" << endl;
			return false;
		}
		
		//And check the command
		CMDINDEX(sBuf, m_nCmdIndex);
		return ReadReturnResponse(message, dwSize, sBuf);
	}
	else
		return false;
}

bool CIMAP4Connection::Delete(int nMsg)
{
	return true;
}

bool CIMAP4Connection::ReadReturnResponse(CIMAP4Message& message, unsigned long dwSize, char* pszTerminal)
{
	if(!m_bConnected)
		return false;
	
	char* pszOverFlowBuffer = NULL;
	int nSize = dwSize + 100;
	char* sBuf = new char[nSize];
	char* sMessageBuf = sBuf;
	if (!ReadResponse(sBuf, nSize, pszTerminal, &pszOverFlowBuffer, 32000))
	{
		delete [] sBuf;
		if (pszOverFlowBuffer)
			delete [] pszOverFlowBuffer;
		
		cout << "Error retrieving the RETR response" << endl;
		return false;
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
		
		return false;
	}
	else
	{  
		sMessageBuf[ca - sMessageBuf] = '\0';
		//remove the first line which contains the +OK from the message
		char* pszFirst = GetFirstCharInResponse(sMessageBuf);
		//VERIFY(pszFirst);
		
		//transfer the message contents to the message class
		int nMessageSize = static_cast<int>(sMessageBuf - pszFirst) + static_cast<int>(strlen(sMessageBuf));
		
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
	
	return true; 
}

bool CIMAP4Connection::GetMessageSize(int nMsg, unsigned long& dwSize)
{
	bool bSuccess = true;

	char	sBuf[64];
	char	sCmd[64];
	sprintf(sBuf, "FETCH %d RFC822.SIZE", nMsg);
	CMDLINE(sCmd, m_nCmdIndex, sBuf);
	int nCmdLength = static_cast<int>(strlen(sCmd));

	if (!m_imap.Send(sCmd, nCmdLength))
	{
		cout << "Failed to send the FETCH command to the IMAP4 server\n" << endl;
		return false;
	}

	char* pszOverFlowBuffer = NULL;
	char sBuff[1000];
	char sTer[20];
	char* sMessageBuf = sBuff;
	CMDINDEX(sTer, m_nCmdIndex);
	if (!ReadResponse(sBuff, 1000, sTer, &pszOverFlowBuffer, 32000))
	{
		if (pszOverFlowBuffer)
			delete [] pszOverFlowBuffer;
		
		cout << "Error retrieving the RETR response" << endl;
		return false;
	}
	if (pszOverFlowBuffer)
		sMessageBuf = pszOverFlowBuffer;

	char sMatch[64];
	sprintf(sMatch, "%s OK", sTer);
	if(strstr(sMessageBuf, sMatch) == NULL)
	{
		if (pszOverFlowBuffer)
			delete [] pszOverFlowBuffer;
		
		return false;
	}
	else
	{  
		char *ca = strstr(sMessageBuf, "RFC822.SIZE");
		if(ca) dwSize = atol(ca + 11);
		return true;
	}
}

char* CIMAP4Connection::GetFirstCharInResponse(char* pszData) const
{
	while ((*pszData != '\n') && *pszData)
		++pszData;
	
	//skip over the "\n" onto the next line
	if (*pszData)
		++pszData;
	
	return pszData;
}

string	CIMAP4Connection::GetLastError()
{
	return m_sLastCommandResponse;
}