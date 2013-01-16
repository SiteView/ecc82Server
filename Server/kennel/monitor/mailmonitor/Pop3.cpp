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
#include "pop3.h"

#include "global.h"
#include "base64.h"
#include "../base/SVDefines.h"
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

string CPop3Message::GetHeaderItem(const string& sName, int nItem) const
{
    //Value which will be returned by this function
    string sField;

    //Get the message header (add an extra "\r\n" at the
    //begining to aid in the parsing)  
    string sHeader("\r\n");
    sHeader += GetHeader();
    string sUpCaseHeader(sHeader);
    strupr(const_cast<char*>(sUpCaseHeader.c_str()));

    string sUpCaseName(sName);
    strupr(const_cast<char*>(sUpCaseName.c_str()));

    //find the specified line in the header
    string sfind("\r\n" + sUpCaseName + ":");
    int nfindLength = static_cast<int>(sfind.length());
    int nfindStart = static_cast<int>(sUpCaseHeader.find(sfind));
    int nfind = nfindStart;
    for (int i=0; i<nItem; i++) 
    {
        //Get ready for the next loop around
        sUpCaseHeader = sUpCaseHeader.substr(nfind + sfind.length());
        nfind = static_cast<int>(sUpCaseHeader.find(sfind));

        if (nfind == -1)
            return ""; //Not found
        else
            nfindStart += (nfind + nfindLength);
    }

    if (nfindStart != -1)
        nfindStart += (3 + static_cast<int>(sName.length()));
    if (nfindStart != -1)
    {
        bool bFoundEnd = false;
        int i = nfindStart;
        int nLength = static_cast<int>(sHeader.length());
        do
        {
            //Examine the current 3 characters
            unsigned char c1 = '\0';
            if (i < nLength)
                c1 = sHeader[i];

            unsigned char c2 = '\0';
            if (i < (nLength-1))
                c2 = sHeader[i+1];

            unsigned char c3 = '\0';
            if (i < (nLength-2))
                c3 = sHeader[i+2];

            //Have we found the terminator
            if ((c1 == '\0') || ((c1 == '\r') && (c2 == '\n') 
                && (c3 != ' ') && c3 != '\t'))
            {
                bFoundEnd = true;
            }
            else
            {
                //Move onto the next character  
                ++i;
            }
        }while (!bFoundEnd);
        sField = sHeader.substr(nfindStart, i - nfindStart);

        //Remove any embedded "\r\n" sequences from the field
        int nEOL = static_cast<int>(sField.find("\r\n"));
        while (nEOL != -1)
        {
            sField = sField.substr(0, nEOL) + sField.substr(nEOL + 2);
            nEOL = static_cast<int>(sField.find("\r\n"));
        }

        //Replace any embedded "\t" sequences with spaces
        int nTab = static_cast<int>(sField.find('\t'));
        while (nTab != -1)
        {
            sField = sField.substr(0, nTab) + ' ' + sField.substr(nTab + 1);
            nTab = static_cast<int>(sField.find('\t'));
        }

        //Remove any leading or trailing white space from the Field Body
        sField = strtriml(sField.c_str());
		sField = strtrimr(sField.c_str());
    }

    return sField;
}

string CPop3Message::GetHeader() const
{
    //Value which will be returned by this function
    string sHeader;

    //find the divider between the header and body
    string sMessage(m_pszMessage);
    int nfind = static_cast<int>(sMessage.find("\r\n\r\n"));
    if (nfind != -1)
        sHeader = sMessage.substr(0, nfind);
    else
    {
        //No divider, then assume all the text is the header
        sHeader = sMessage;
    }

    return sHeader;
}

char* CPop3Message::GetRawBody() const
{
    char* pszStartBody = strstr(GetMessageText(), "\r\n\r\n");
    if (pszStartBody == NULL) 
        return NULL;
    else 
        return pszStartBody + 4;
}

string CPop3Message::GetBody() const
{
    string sBody;
    const char* pszBody = GetRawBody();
    if (pszBody)
        sBody = pszBody;
    return sBody;
}

string CPop3Message::GetReplyTo() const
{
    string sRet = GetHeaderItem("Reply-To");
    if (sRet.empty())
    {
        sRet = GetFrom();
        if (sRet.empty())
        {
            sRet = GetHeaderItem("Sender");
            if (sRet.empty())
                sRet = GetHeaderItem("Return-Path");
        }
    }
    return sRet;
}


CPop3Socket::CPop3Socket()
{
    m_hSocket = INVALID_SOCKET; //default to an invalid scoket descriptor
}

CPop3Socket::~CPop3Socket()
{
    Close();
}

bool CPop3Socket::Create()
{
    m_hSocket = socket(AF_INET, SOCK_STREAM, 0);
    return (m_hSocket != INVALID_SOCKET);
}

bool CPop3Socket::Connect(const char* pszHostAddress, int nPort)
{
    //For correct operation of the T2A macro, see MFC Tech Note 59
    //USES_CONVERSION;

    //must have been created first
    //ASSERT(m_hSocket != INVALID_SOCKET);

    //Determine if the address is in dotted notation
    SOCKADDR_IN sockAddr;
    ZeroMemory(&sockAddr, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons((u_short)nPort);
    //char* pszAsciiHostAddress = T2A((LPTSTR) pszHostAddress);
    sockAddr.sin_addr.s_addr = inet_addr(pszHostAddress);

    //If the address is not dotted notation, then do a DNS 
    //lookup of it.
    if (sockAddr.sin_addr.s_addr == INADDR_NONE)
    {
        LPHOSTENT lphost;
        lphost = gethostbyname(pszHostAddress);
        if (lphost != NULL)
            sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
        else
        {
            WSASetLastError(WSAEINVAL); 
            return false;
        }
    }

	//Call the protected version which takes an address 
	//in the form of a standard C style struct.
	return Connect((SOCKADDR*)&sockAddr, sizeof(sockAddr));
}

bool CPop3Socket::Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen)
{
    return (connect(m_hSocket, lpSockAddr, nSockAddrLen) != SOCKET_ERROR);
}

bool CPop3Socket::Send(const char* pszBuf, int nBuf)
{
    //must have been created first
    //ASSERT(m_hSocket != INVALID_SOCKET);

    return (send(m_hSocket, pszBuf, nBuf, 0) != SOCKET_ERROR);
}

int CPop3Socket::Receive(char* pszBuf, int nBuf)
{
    //must have been created first
    //ASSERT(m_hSocket != INVALID_SOCKET);

    return recv(m_hSocket, pszBuf, nBuf, 0); 
}

void CPop3Socket::Close()
{
    if (m_hSocket != INVALID_SOCKET)
    {
        //VERIFY(SOCKET_ERROR != closesocket(m_hSocket));
        closesocket(m_hSocket);
        m_hSocket = INVALID_SOCKET;
    }
}

bool CPop3Socket::IsReadible(bool& bReadible)
{
    timeval timeout = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(m_hSocket, &fds);
    int nStatus = select(0, &fds, NULL, NULL, &timeout);
    if (nStatus == SOCKET_ERROR)
    {
        return false;
    }
    else
    {
        bReadible = !(nStatus == 0);
        return true;
    }
}


CPop3Connection::CPop3Connection()
{
    m_nNumberOfMails = 0;
    m_bListRetrieved = false;
    m_bStatRetrieved = false;
    m_bUIDLRetrieved = false;
    m_msgSizes.clear();
    m_bConnected = false;
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

bool CPop3Connection::Connect(const char* pszHostName, const char* pszUser, const char* pszPassword, 
                            int nNeedSPA, int nPort)
{
    //For correct operation of the T2A macro, see MFC Tech Note 59
    //USES_CONVERSION;

    //Create the socket
    if (!m_Pop.Create())
    {
        m_szLastError = ("Failed to create client socket");
        return false;
    }

    //Connect to the POP3 Host
    if (!m_Pop.Connect(pszHostName, nPort))
    {
        m_szLastError = ("Could not connect to the POP3 Server");
        cout << m_szLastError.c_str() << endl;
        return false;
    }
    else
    {
        //We're now connected !!
        m_bConnected = true;

        //check the response
        if (!ReadCommandResponse())
        {
            m_szLastError = ("Failed while connected to read a command response from the POP3 server\n");
            Disconnect();
            return false;
        }

        if(nNeedSPA == 1)
        {
            string szUser = "", szPwd = "";
            szUser = pszUser;
            szPwd  = pszPassword;
            //szComputer = local_host;
            return NTLM_Login(szUser, szPwd);
        }
        //Send the POP3 username and check the response
        char sBuf[128];
        //char* pszAsciiUser = T2A((LPTSTR) pszUser);
        //ASSERT(strlen(pszAsciiUser) < 100); 
        sprintf(sBuf, "USER %s\r\n", pszUser);
        int nCmdLength = static_cast<int>(strlen(sBuf));
        if (!m_Pop.Send(sBuf, nCmdLength))
        {
            m_szLastError = ("Failed to send the USER command to the POP3 server\n");
            Disconnect();
            return false;
        }
        if (!ReadCommandResponse())
        {
            m_szLastError = ("Failed while connected to read a USER command response from the POP3 server\n");
            Disconnect();
            return false;
        } 

        //Send the POP3 password and check the response
        //char* pszAsciiPassword = T2A((LPTSTR) pszPassword);
        //ASSERT(strlen(pszAsciiPassword) < 100);
        sprintf(sBuf, "PASS %s\r\n", pszPassword);
        nCmdLength = static_cast<int>(strlen(sBuf));
        if (!m_Pop.Send(sBuf, nCmdLength))
        {
            m_szLastError = ("Failed to send the PASS command to the POP3 server\n");
            Disconnect();
            return false;
        }
        if (!ReadCommandResponse())
        {
            m_szLastError = ("Failed while connected to read a PASS command response from the POP3 server\n");
            Disconnect();
            return false;
        }    
        return true;
    }
}

bool CPop3Connection::Disconnect()
{          
    bool bSuccess = false;      

    //disconnect from the POP3 server if connected 
    if (m_bConnected)
    {
        char sBuf[10];
        strcpy(sBuf, "QUIT\r\n");
        int nCmdLength = static_cast<int>(strlen(sBuf));
        if (!m_Pop.Send(sBuf, nCmdLength))
            m_szLastError = ("Failed to send the QUIT command to the POP3 server\n");

        //Check the reponse
        bSuccess = ReadCommandResponse();

        //Reset all the state variables
        m_bConnected = false;
        m_bListRetrieved = false;
        m_bStatRetrieved = false;
        m_bUIDLRetrieved = false;
    }
    else
        m_szLastError = ("CPop3Connection, Already disconnected\n");
 
    //free up our socket
    m_Pop.Close();

    return bSuccess;
}

bool CPop3Connection::Delete(int nMsg)
{
    //Must be connected to perform a delete
    //ASSERT(m_bConnected);

    //if we haven't executed the LIST command then do it now
    if (!m_bListRetrieved)
    List();

    //Send the DELE command along with the message ID
    char sBuf[20];
    sprintf(sBuf, "DELE %d\r\n", nMsg);
    int nCmdLength = static_cast<int>(strlen(sBuf));
    if (!m_Pop.Send(sBuf, nCmdLength))
    {
        m_szLastError = ("Failed to send the DELE command to the POP3 server\n");
        return false;
    }

	return ReadCommandResponse();
}

bool CPop3Connection::Statistics(int& nNumberOfMails, int& nTotalMailSize)
{
    //Must be connected to perform a "STAT"
    //ASSERT(m_bConnected);

    //Send the STAT command
    char sBuf[10];
    strcpy(sBuf, "STAT\r\n");
    int nCmdLength = static_cast<int>(strlen(sBuf));
    if (!m_Pop.Send(sBuf, nCmdLength))
    {
        m_szLastError = ("Failed to send the STAT command to the POP3 server\n");
        return false;
    }

	return ReadStatResponse(nNumberOfMails, nTotalMailSize);
}

bool CPop3Connection::GetMessageSize(int nMsg, unsigned long& dwSize)
{
    bool bSuccess = true;

    //if we haven't executed the LIST command then do it now
    if (!m_bListRetrieved)
        bSuccess = List();

    //nMsg must be in the correct range
    //ASSERT((nMsg > 0) && (nMsg <= m_msgSizes.size()));

    //retrieve the size from the message size array
    dwSize = m_msgSizes[nMsg - 1];

    return bSuccess;
}

bool CPop3Connection::GetMessageID(int nMsg, string& sID)
{
    bool bSuccess = true;

    //if we haven't executed the UIDL command then do it now
    if (!m_bUIDLRetrieved)
        bSuccess = UIDL();

    //nMsg must be in the correct range
    //ASSERT((nMsg > 0) && (nMsg <= m_msgIDs.size()));

    //retrieve the size from the message size array
    sID = m_msgIDs[nMsg - 1];

    return bSuccess;
}

bool CPop3Connection::List()
{
    //Must be connected to perform a "LIST"
    //ASSERT(m_bConnected);

    //if we haven't executed the STAT command then do it now
    int nNumberOfMails = m_nNumberOfMails;
    int nTotalMailSize;
    if (!m_bStatRetrieved)
    {
        if (!Statistics(nNumberOfMails, nTotalMailSize))
            return false;
        else
            m_bStatRetrieved = true;
    }

    //Send the LIST command
    char sBuf[10];
    strcpy(sBuf, "LIST\r\n");
    int nCmdLength = static_cast<int>(strlen(sBuf));
    if (!m_Pop.Send(sBuf, nCmdLength))
    {
        m_szLastError = ("Failed to send the LIST command to the POP3 server\n");
        return false;
    }
    //And check the response
    m_bListRetrieved = ReadListResponse(nNumberOfMails);
    return m_bListRetrieved;
}

bool CPop3Connection::UIDL()
{
    //Must be connected to perform a "UIDL"
    //ASSERT(m_bConnected);

    //if we haven't executed the STAT command then do it now
    int nNumberOfMails = m_nNumberOfMails;
    int nTotalMailSize;
    if (!m_bStatRetrieved)
    {
        if (!Statistics(nNumberOfMails, nTotalMailSize))
            return false;
        else
            m_bStatRetrieved = true;
    }

    //Send the UIDL command
    char sBuf[10];
    strcpy(sBuf, "UIDL\r\n");
    int nCmdLength = static_cast<int>(strlen(sBuf));
    if (!m_Pop.Send(sBuf, nCmdLength))
    {
        m_szLastError = ("Failed to send the UIDL command to the POP3 server\n");
        return false;
    }
    //And check the response
    m_bUIDLRetrieved = ReadUIDLResponse(nNumberOfMails);
    return m_bUIDLRetrieved;
}

bool CPop3Connection::Reset()
{
    //Must be connected to perform a "RSET"
    //ASSERT(m_bConnected);

    //Send the RSET command
    char sBuf[10];
    strcpy(sBuf, "RSET\r\n");
    int nCmdLength = static_cast<int>(strlen(sBuf));
    if (!m_Pop.Send(sBuf, nCmdLength))
    {
        m_szLastError = ("Failed to send the RSET command to the POP3 server\n");
        return false;
    }

    //And check the command
    return ReadCommandResponse();
}

bool CPop3Connection::Noop()
{
    //Must be connected to perform a "NOOP"
    //ASSERT(m_bConnected);

    //Send the NOOP command
    char sBuf[10];
    strcpy(sBuf, "NOOP\r\n");
    int nCmdLength = static_cast<int>(strlen(sBuf));
    if (!m_Pop.Send(sBuf, nCmdLength))
    {
        m_szLastError = ("Failed to send the NOOP command to the POP3 server\n");
        return false;
    }

    //And check the response
    return ReadCommandResponse();
}

bool CPop3Connection::Retrieve(int nMsg, CPop3Message& message)
{
    //Must be connected to retrieve a message
    //ASSERT(m_bConnected);

    //work out the size of the message to retrieve
    unsigned long dwSize;
    if (GetMessageSize(nMsg, dwSize))
    {
        //Send the RETR command
        char sBuf[20];
        sprintf(sBuf, "RETR %d\r\n", nMsg);	
        int nCmdLength = static_cast<int>(strlen(sBuf));
        if (!m_Pop.Send(sBuf, nCmdLength))
        {
            m_szLastError = ("Failed to send the RETR command to the POP3 server\n");
            return false;
        }    
		//And check the command
        return ReadReturnResponse(message, dwSize);
    }
    else
        return false;
}

bool CPop3Connection::GetMessageHeader(int nMsg, CPop3Message& message)
{
    // Must be connected to retrieve a message
    //ASSERT(m_bConnected);

    // make sure the message actually exists
    unsigned long dwSize;
    if (GetMessageSize(nMsg, dwSize))
    {
        // Send the TOP command
        char sBuf[16];
        sprintf(sBuf, "TOP %d 0\r\n", nMsg);
        int nCmdLength = static_cast<int>(strlen(sBuf));
        if (!m_Pop.Send(sBuf, nCmdLength))
        {
            m_szLastError = ("Failed to send the TOP command to the POP3 server\n");
            return false;
        }

        // And check the command
        return ReadReturnResponse(message, dwSize);
    }
    else
        return false;
}

bool CPop3Connection::ReadCommandResponse()
{
    char* pszOverFlowBuffer = NULL;
    char sBuf[1000];
    bool bSuccess = ReadResponse(sBuf, 1000, "\r\n", &pszOverFlowBuffer);
    if (pszOverFlowBuffer)
        delete [] pszOverFlowBuffer;

    m_szResponse = sBuf;
    return bSuccess;
}

char* CPop3Connection::GetFirstCharInResponse(char* pszData) const
{
    while ((*pszData != '\n') && *pszData)
        ++pszData;

    //skip over the "\n" onto the next line
    if (*pszData)
        ++pszData;

    return pszData;
}

bool CPop3Connection::ReadResponse(char* pszBuffer, int nInitialBufSize, 
                                   char* pszTerminator, char** ppszOverFlowBuffer, int nGrowBy)
{
    //ASSERT(ppszOverFlowBuffer);          //Must have a valid string pointer
    //ASSERT(*ppszOverFlowBuffer == NULL); //Initially it must point to a NULL string

    //must have been created first
    //ASSERT(m_bConnected);

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
        if (!m_Pop.IsReadible(bReadible))
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
        int nData = m_Pop.Receive(pszRecvBuffer+nReceived, nBufRemaining);

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
    bool bSuccess = (strnicmp(pszRecvBuffer,"+OK", 3) == 0);

    if (!bSuccess)
    {
        SetLastError(WSAEPROTONOSUPPORT);
        m_sLastCommandResponse = pszRecvBuffer; //Hive away the last command reponse
    }

    return bSuccess;
}

bool CPop3Connection::ReadReturnResponse(CPop3Message& message, unsigned long dwSize)
{
    //Must be connected to perform a "RETR"
    //ASSERT(m_bConnected);

    //Retrieve the message body
    char* pszOverFlowBuffer = NULL;
    int nSize = dwSize + 100;
    char* sBuf = new char[nSize];
    char* sMessageBuf = sBuf;
    if (!ReadResponse(sBuf, nSize, "\r\n.\r\n", &pszOverFlowBuffer, 32000))
    {
        delete [] sBuf;
        if (pszOverFlowBuffer)
        delete [] pszOverFlowBuffer;

        m_szLastError = ("Error retrieving the RETR response");
        return false;
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
        m_szLastError = ("POP3 server did not respond correctly to the RETR response\n");
        return false;
    }
    else
	{  
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

bool CPop3Connection::ReadListResponse(int nNumberOfMails)
{
    //Must be connected to perform a "LIST"
    //ASSERT(m_bConnected);

    //retrieve the reponse
    char* pszOverFlowBuffer = NULL;
    int nSize = 14 * nNumberOfMails + 100;
    char* sBuf = new char[nSize];
    char* sMessageBuf = sBuf;
    if (!ReadResponse(sBuf, nSize, "\r\n.\r\n", &pszOverFlowBuffer))
    {
        delete [] sBuf;
        if (pszOverFlowBuffer)
            delete [] pszOverFlowBuffer;

        m_szLastError = ("Error retrieving the LIST response from the POP3 server");
        return false;
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
        m_szLastError = ("POP3 server did not respond correctly to the LIST response\n");
        return false;
    }
	else
	{  
        //Retrieve the message sizes and put them
        //into the m_msgSizes array
        m_msgSizes.clear();
        //m_msgSizes.SetSize(0, nNumberOfMails);

        //then parse the LIST response
        char* pszSize = GetFirstCharInResponse(sMessageBuf);
        //VERIFY(pszSize);
        for (; *pszSize != '.'; pszSize++)
            if (*pszSize == '\t' || *pszSize == ' ')
                m_msgSizes.push_back(atoi(pszSize));
	}
    delete [] sBuf;
    if (pszOverFlowBuffer)
        delete [] pszOverFlowBuffer;

    return true; 
}

bool CPop3Connection::ReadUIDLResponse(int nNumberOfMails)
{
    //Must be connected to perform a "LIST"
    //ASSERT(m_bConnected);

    //retrieve the reponse
    char* pszOverFlowBuffer = NULL;
    int nSize = 14 * nNumberOfMails + 100;
    char* sBuf = new char[nSize];
    char* sMessageBuf = sBuf;
    if (!ReadResponse(sBuf, nSize, "\r\n.\r\n", &pszOverFlowBuffer))
    {
        delete [] sBuf;
        if (pszOverFlowBuffer)
        delete [] pszOverFlowBuffer;

        m_szLastError = ("Error retrieving the UIDL response from the POP3 server");
        return false;
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
        m_szLastError = ("POP3 server did not respond correctly to the UIDL response\n");
        return false;
    }
	else
	{  
        //Retrieve the message ID's and put them
        //into the m_msgIDs array
        m_msgIDs.clear();
        //m_msgIDs.resize(0, nNumberOfMails);

        //then parse the UIDL response
        char* pszSize = GetFirstCharInResponse(sMessageBuf);
        //VERIFY(pszSize);
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

            m_msgIDs.push_back(string(sID));
        }
	}
    delete [] sBuf;
    if (pszOverFlowBuffer)
        delete [] pszOverFlowBuffer;

  return true; 
}

bool CPop3Connection::ReadStatResponse(int& nNumberOfMails, int& nTotalMailSize)
{
    //Must be connected to perform a "STAT"
    //ASSERT(m_bConnected);

    //retrieve the reponse
    char* pszOverFlowBuffer = NULL;
    char sBuf[100];
    char* sMessageBuf = sBuf;
    if (!ReadResponse(sBuf, 100, "\r\n", &pszOverFlowBuffer))
    {
        if (pszOverFlowBuffer)
            delete [] pszOverFlowBuffer;

        m_szLastError = ("Error retrieving the STAT response from the POP3 server");
        return false;
    }
    if (pszOverFlowBuffer)
        sMessageBuf = pszOverFlowBuffer;

    //determine if the response is an error
    if (strncmp(sMessageBuf,"+OK", 3) != 0)
    {
        m_szLastError = ("POP3 server did not respond correctly to the STAT response\n");
        return false;
    }
    else
    {                                          
        //Parse out the Number of Mails and Total mail size values
        bool bGetNumber = true;
        for (char* pszNum=sMessageBuf; *pszNum!='\0'; pszNum++)
        {
            if (*pszNum=='\t' || *pszNum==' ')
            {						
                if (bGetNumber)
                {
                    nNumberOfMails = atoi(pszNum);
                    m_nNumberOfMails = nNumberOfMails;
                    bGetNumber = false;
				}
				else
				{
				    nTotalMailSize = atoi(pszNum);
				    return true;
				}
			}
		}
	}
    if (pszOverFlowBuffer)
        delete [] pszOverFlowBuffer;

    return false; 
}

string CPop3Connection::GetLastErrorInfor() const
{
    return m_szLastError;
}

bool CPop3Connection::NTLM_Login(string szUser, string szPwd)
{
    string sAuth = "";
    sAuth = "AUTH NTLM\r\n";
    if (!m_Pop.Send(sAuth.c_str(), static_cast<int>(sAuth.length())))
    {
        m_szLastError = ("Failed to send the AUTH NTLM command to the POP3 server\n");
        Disconnect();
        return false;
    }
    if (!ReadCommandResponse())
    {
        m_szLastError = ("Failed while connected to read a AUTH NTLM command response from the POP3 server\n");
        Disconnect();
        return false;
    }

    tSmbNtlmAuthRequest request;
    tSmbNtlmAuthChallenge challenge;
    tSmbNtlmAuthResponse response;

    buildEmptySmbNtlmAuthRequest(&request);
    char *pBase64 = new char[SmbLength(&request)*2 + 4];
    Base64Encode(pBase64, (char*)&request, static_cast<int>(SmbLength(&request)));
    sAuth = pBase64;
    sAuth += "\r\n";
    delete pBase64;
    pBase64 = NULL;
    if (!m_Pop.Send(sAuth.c_str(), static_cast<int>(sAuth.length())))
    {
        m_szLastError = ("Failed to send the USER command to the POP3 server\n");
        Disconnect();
        return false;
    }
    ReadCommandResponse();
    char local_host[81] = {0};
    gethostname( local_host, 80 );
    string szReponse = m_szResponse;
    szReponse = szReponse.substr(2);
    int nSize = static_cast<int>(szReponse.length());
    char *pszReponse = new char[nSize + 1];
    int nLen = Base64Decode(pszReponse, szReponse.c_str(), nSize); 
    memcpy(&challenge, pszReponse, nLen);
    buildNtlmAuthResponse (&challenge, &response, szUser.c_str(), szPwd.c_str(),  local_host);
    delete []pszReponse;
    pszReponse = NULL;
    pBase64 = new char[SmbLength(&response)*2 + 4];
    Base64Encode(pBase64, (char*)&response, static_cast<int>(SmbLength(&response)));
    sAuth = pBase64;
    sAuth += "\r\n";
    delete pBase64;
    pBase64 = NULL;
    if (!m_Pop.Send(sAuth.c_str(), static_cast<int>(sAuth.length())))
    {
        m_szLastError = ("Failed to send the PASS command to the POP3 server\n");
        Disconnect();
        return false;
    }
    if (!ReadCommandResponse())
    {
        m_szLastError = ("Failed while connected to read a PASS command response from the POP3 server\n");
        Disconnect();
        return false;
    } 

    delete pBase64;
    pBase64 = NULL;

    return true;
}

bool CPop3Connection::Top(int nMsg, string &szMessage)
{
	//Must be connected to retrieve a message
	if(!m_bConnected)
	{
		//m_sLastCommandResponse = _T("通信未建立");//<%IDS_Pop3_26%>
		m_szLastError = ::GetResourceValue("<%IDS_Pop3_26%>");//"通信未建立"; //::GetResourceValue("<%IDS_Pop3_26%>");//<%IDS_Pop3_26%>
		return false;
	}
	
	//Send the RETR command
	char sBuf[20];
	sprintf(sBuf, "TOP %d 50\r\n", nMsg);	
	int nCmdLength = static_cast<int>(strlen(sBuf));
	if (!m_Pop.Send(sBuf, nCmdLength))
	{
		m_szLastError = "Failed to send the TOP command to the POP3 server\n";
		//m_sLastCommandResponse = _T("发送TOP命令失败");//<%IDS_Pop3_27%>
		//m_szLastError = "发送TOP命令失败";//GetResourceValue("<%IDS_Pop3_27%>");
		return false;
	}
		
	//And check the command
	char* pszOverFlowBuffer = NULL;
	char szBuffer[256] = {0};
	if(!ReadResponse(szBuffer, 70, "\r\n.\r\n", &pszOverFlowBuffer))
	{
		if (pszOverFlowBuffer)
			delete [] pszOverFlowBuffer;
		
		m_szLastError = "Error retrieving the TOP response";
		return false;
	}

	if(!pszOverFlowBuffer)
		return false;

    //cout << "Top response: " << szMessage.c_str() << endl;
	szMessage = pszOverFlowBuffer;
	delete pszOverFlowBuffer;
	return true;
}
