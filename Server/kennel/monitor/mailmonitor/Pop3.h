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

//#ifndef __AFXTEMPL_H__
//#pragma message("POP3 classes require afxtempl.h in your PCH")                                                                                
//#endif
//
//#ifndef _WINSOCKAPI_
//#pragma message("POP3 classes require afxsock.h or winsock.h in your PCH")
//#endif

#include <iostream> 
#include <string>
#include <vector>

using namespace std; 

#include "../base/SVDefines.h"
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
    char* GetMessageText() const { return m_pszMessage; };
    char*  GetRawBody() const;
    string GetHeader() const;
    string GetHeaderItem(const string& sName, int nItem = 0) const;
    string GetBody() const;

    string GetSubject() const { return GetHeaderItem("Subject"); }
    string GetFrom() const		 { return GetHeaderItem("From"); }
    string GetDate() const		 { return GetHeaderItem("Date"); }
    string GetReplyTo() const;

    //protected:
    char* m_pszMessage;

    friend class CPop3Connection;
};


//Simple Socket wrapper class
class CPop3Socket
{
public:
    //Constructors / Destructors
    CPop3Socket();
    ~CPop3Socket();

    //methods
    bool  Create();
    bool  Connect(const char* pszHostAddress, int nPort = 110);
    bool  Send(const char* pszBuf, int nBuf);
    void  Close();
    int   Receive(char* pszBuf, int nBuf);
    bool  IsReadible(bool& bReadible);

protected:
    bool   Connect(const SOCKADDR* lpSockAddr, int nSockAddrLen);
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
    bool            Connect(const char* pszHostName, const char* pszUser, const char* pszPassword, 
                            int nNeedSPA, int nPort = 110);
    bool            Disconnect();
    bool            Top(int nMsg, string &szMessage);
    bool            Statistics(int& nNumberOfMails, int& nTotalMailSize);
    bool            Delete(int nMsg);
    bool            GetMessageSize(int nMsg, unsigned long& dwSize);
    bool            GetMessageID(int nMsg, string& sID);
    bool            Retrieve(int nMsg, CPop3Message& message);
    bool            GetMessageHeader(int nMsg, CPop3Message& message);
    bool            Reset();
    bool            UIDL();
    bool            Noop();
    string          GetLastCommandResponse() const { return m_sLastCommandResponse; };
    unsigned long   GetTimeout() const { return m_dwTimeout; };
    void            SetTimeout(unsigned long dwTimeout) { m_dwTimeout = dwTimeout; };
    string          GetLastErrorInfor() const;
protected:
    virtual bool    ReadStatResponse(int& nNumberOfMails, int& nTotalMailSize);
    virtual bool    ReadCommandResponse();
    virtual bool    ReadListResponse(int nNumberOfMails);
    virtual bool    ReadUIDLResponse(int nNumberOfMails);
    virtual bool    ReadReturnResponse(CPop3Message& message, unsigned long dwSize);
    virtual bool    ReadResponse(char* pszBuffer, int nInitialBufSize, char* pszTerminator, 
                        char** ppszOverFlowBuffer, int nGrowBy=4096);
    bool            List();
    char*           GetFirstCharInResponse(char* pszData) const;
    bool            NTLM_Login(string szUser, string szPwd);

    CPop3Socket             m_Pop;
    int                     m_nNumberOfMails;
    bool                    m_bListRetrieved;
    bool                    m_bStatRetrieved;
    bool                    m_bUIDLRetrieved;
    vector<unsigned long>   m_msgSizes;
    vector<string>          m_msgIDs;
    bool                    m_bConnected;
    string                  m_sLastCommandResponse;
    unsigned long           m_dwTimeout;
    string                  m_szLastError;
    string                  m_szResponse;
};


#endif //__POP3_H__

