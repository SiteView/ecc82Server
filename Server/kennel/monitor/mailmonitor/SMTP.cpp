// SMTP.cpp: implementation of the CSMTP class.
//
//////////////////////////////////////////////////////////////////////

#include "SMTP.h"
#include "MimeCode.h"

#include "../base/SVDefines.h"
#include "base64.h"
#include "Mime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "global.h"
extern int PrintLog(const char * strParas);
// Static member initializers
//

// Note: the order of the entries is important.
//       They must be synchronized with eResponse entries. 
CSMTP::response_code CSMTP::response_table[] =
{
	// GENERIC_SUCCESS
    {  250, "SMTP server error" },

	// CONNECT_SUCCESS
	{ 220, "SMTP server not available" },

	// DATA_SUCCESS
	{ 354, "SMTP server not ready for data" },

	// QUIT_SUCCESS
	{ 221,  "SMTP server didn't terminate session" }, 

	// NEED AUTH
	{ 334,  "SMTP server not ready for auth" }, 

	// AUTH SUCCESS
	{ 235,  "SMTP server auth success" }, 
};

    
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSMTP::CSMTP( const char * szSMTPServerName, unsigned int nPort, unsigned int nTimeout)
{
//	ASSERT( szSMTPServerName != NULL );
	//AfxSocketInit();
    SV_NetInit();
	m_sSMTPServerHostName = szSMTPServerName;
	m_nPort = nPort;
	m_bConnected = false;
	m_sError =  "OK";
	response_buf = NULL;
    m_nTimeout = nTimeout;
}

CSMTP::~CSMTP()
{
	Disconnect();
}

const char * CSMTP::GetServerHostName()
{
	return m_sSMTPServerHostName.c_str();
}

bool CSMTP::Connect(const int &nNeedAuth, const int &nNeedSPA, const char * strvUsername, const char * strvPassword)
{
	char strTemp[1024]={0};
	sprintf(strTemp,"nNeedAuth=%d",nNeedAuth);
	PrintLog(strTemp);
	string sHello;
	//TCHAR local_host[ 80 ];	// Warning: arbitrary size
	if( m_bConnected )
		return true;
	
	try
	{
		// This will be deleted in Disconnect();
		response_buf = new char[ RESPONSE_BUFFER_SIZE ];
        response_buf[0] = '\0';

		// I can't count on all class users' applications
		// to have exception-throwing operator-new implementations,
		// so I'll soul-kiss the ones that don't.
		if( response_buf == NULL )
		{
			m_sError = "Not enough memory";
			return false;
		}
	}
	catch(...)
	{
		response_buf = NULL;
		m_sError = "Not enough memory";
		return false;
	}

    m_wsSMTPServer = static_cast<int>(socket(AF_INET, SOCK_STREAM, 0));
	if(m_wsSMTPServer == INVALID_SOCKET)
	{
		m_sError = "Unable to create the socket.";
		PrintLog(m_sError.c_str());
		delete[] response_buf;
		response_buf = NULL;
		return false;
	}

	struct hostent *hp;
	struct sockaddr_in serv;
	hp = gethostbyname(GetServerHostName());
	if(hp == NULL)
	{
		m_sError = "Unable to connect to server";
        cout << m_sError.c_str() << endl;
		PrintLog(m_sError.c_str());
		closesocket(m_wsSMTPServer);
		delete[] response_buf;
		response_buf = NULL;
		return false;
	}

	memcpy(&serv.sin_addr, hp->h_addr_list[0],hp->h_length);
	serv.sin_family = AF_INET;
	serv.sin_port=htons(GetPort());

	if (connect_timeo(m_wsSMTPServer, (struct sockaddr*)&serv, sizeof(serv), m_nTimeout)<0)
	{
		m_sError =  "Unable to connect to server";
		PrintLog(m_sError.c_str());
		closesocket(m_wsSMTPServer);
		delete[] response_buf;
		response_buf = NULL;
		return false;
	}

	if( !get_response( CONNECT_SUCCESS ) )
	{
		m_sError = "Server didn't respond.";
        cout << m_sError.c_str() << endl;
		PrintLog(m_sError.c_str());
		closesocket(m_wsSMTPServer);
		delete[] response_buf;
		response_buf = NULL;
		return false;
	}

	TCHAR szComputerName[80]={0};
	unsigned long ulSize = 80;
	GetComputerName(szComputerName, &ulSize);
	HOSTENT *ht;
	ht = gethostbyname(szComputerName);
	sHello = "EHLO ";
    sHello += ht->h_name;
    sHello += "\r\n";
    cout << sHello.c_str() << endl;
	send(m_wsSMTPServer, sHello.c_str(), static_cast<int>(sHello.length()), 0);
	if( !get_response( GENERIC_SUCCESS ) )
	{
		sHello = "HELO ";
        sHello += ht->h_name;
        sHello += "\r\n";
        cout << sHello.c_str() << endl;
	    send(m_wsSMTPServer, sHello.c_str(), static_cast<int>(sHello.length()), 0);
		if( !get_response( GENERIC_SUCCESS ) )
		{
			closesocket(m_wsSMTPServer);
			delete response_buf;
			response_buf = NULL;
			return false;
		}
	}

    if(nNeedSPA == 1)
        return NTLM_Login(strvUsername, strvPassword, szComputerName);
    
    if(nNeedAuth == 0)
    {
        m_bConnected = true;
        return true;
    }

	if(strvUsername && strvPassword)
	{
		string sAuth = "";

		sAuth = "AUTH LOGIN\r\n";
        cout << sAuth.c_str() << endl;
		send(m_wsSMTPServer, sAuth.c_str(), static_cast<int>(sAuth.length()), 0);
		PrintLog(sAuth.c_str());
		if( !get_response( AUTH_RESPONSE ) )
		{
			PrintLog(response_buf);
			closesocket(m_wsSMTPServer);
			delete response_buf;
			response_buf = NULL;
			return false;
		}

        char *pBase64 = new char[strlen(strvUsername) * 2 + 4];
		Base64Encode(pBase64, strvUsername, static_cast<int>(strlen(strvUsername)));
        sAuth = pBase64;
        sAuth += "\r\n";
        delete pBase64;
        pBase64 = NULL;
        cout << sAuth.c_str() << endl;
		PrintLog(sAuth.c_str());
		send(m_wsSMTPServer, sAuth.c_str(), static_cast<int>(sAuth.length()), 0);
		PrintLog(sAuth.c_str());
		if( !get_response( AUTH_RESPONSE ) )
		{
			PrintLog(response_buf);
			closesocket(m_wsSMTPServer);			
			delete[] response_buf;
			response_buf = NULL;
			return false;
		}
        
        pBase64 = new char[strlen(strvPassword) * 2 + 4];
		Base64Encode(pBase64, strvPassword, static_cast<int>(strlen(strvPassword)));
        sAuth = pBase64;
        sAuth += "\r\n";
        delete pBase64;
        pBase64 = NULL;
        cout << sAuth.c_str() << endl;
		send(m_wsSMTPServer, sAuth.c_str(), static_cast<int>(sAuth.length()), 0);
		PrintLog(sAuth.c_str());
		if( !get_response( AUTH_SUCCESS ) )
		{
			PrintLog(response_buf);
			closesocket(m_wsSMTPServer);
			delete[] response_buf;
			response_buf = NULL;
			return false;
		}
	}

	m_bConnected = true;
	return true;
}

bool CSMTP::NTLM_Login(const char *pszUsername, const char *pszPassword, const char * pszComputerName)
{
	if(pszUsername && pszPassword)
	{
		string sAuth = "";
        sAuth = "AUTH NTLM\r\n";
        cout << sAuth.c_str() << endl;
		send(m_wsSMTPServer, sAuth.c_str(), static_cast<int>(sAuth.length()), 0);
		if( !get_response( AUTH_RESPONSE ) )
		{
			closesocket(m_wsSMTPServer);
			delete response_buf;
			response_buf = NULL;
			return false;
		}

        tSmbNtlmAuthRequest request;
        tSmbNtlmAuthChallenge challenge;
        tSmbNtlmAuthResponse response;

        buildEmptySmbNtlmAuthRequest(&request);
        char *pBase64 = new char[static_cast<int>(SmbLength(&request)*2) + 4];
		Base64Encode(pBase64, (char*)&request, static_cast<int>(SmbLength(&request)));
        sAuth = pBase64;
        sAuth += "\r\n";
        delete pBase64;
        pBase64 = NULL;
        cout << sAuth.c_str() << endl;
		send(m_wsSMTPServer, sAuth.c_str(), static_cast<int>(sAuth.length()), 0);
		if( !get_response( AUTH_RESPONSE ) )
		{
			closesocket(m_wsSMTPServer);			
			delete[] response_buf;
			response_buf = NULL;
			return false;
		}        
        
        string szReponse = response_buf;
        szReponse = szReponse.substr(4);
        int nSize = static_cast<int>(szReponse.length());
        char *pszReponse = new char[nSize + 1];
        int nLen = Base64Decode(pszReponse, szReponse.c_str(), nSize); 
        memcpy(&challenge, pszReponse, nLen);
        buildNtlmAuthResponse (&challenge, &response, pszUsername, pszPassword, pszComputerName);
        delete []pszReponse;
        pszReponse = NULL;
        pBase64 = new char[SmbLength(&response)*2 + 4];
		Base64Encode(pBase64, (char*)&response, static_cast<int>(SmbLength(&response)));
        sAuth = pBase64;
        sAuth += "\r\n";
        delete pBase64;
        pBase64 = NULL;
        cout << sAuth.c_str() << endl;
		send(m_wsSMTPServer, sAuth.c_str(), static_cast<int>(sAuth.length()), 0);
		if( !get_response( AUTH_SUCCESS ) )
		{
			closesocket(m_wsSMTPServer);
			delete[] response_buf;
			response_buf = NULL;
			return false;
		}
	}
    m_bConnected = true;
    return true;
}

bool CSMTP::Disconnect()
{
	bool ret;
	if( !m_bConnected )
		return true;
//---------------------------------------------------------
/*
	// Disconnect gracefully from the server and close the socket
	CString sQuit = ( "QUIT\r\n" );
	m_wsSMTPServer.Send( (LPCTSTR)sQuit, sQuit.GetLength() );

	// No need to check return value here.
	// If it fails, the message is available with GetLastError
	ret = get_response( QUIT_SUCCESS );
	m_wsSMTPServer.Close();
*/
	// Disconnect gracefully from the server and close the socket
	string sQuit = "QUIT\r\n";
	send(m_wsSMTPServer, sQuit.c_str(), static_cast<int>(sQuit.length()), 0);

	// No need to check return value here.
	// If it fails, the message is available with GetLastError
	ret = get_response( QUIT_SUCCESS );
	closesocket(m_wsSMTPServer);
//-----------------------------------------------------------
	if( response_buf != NULL )
	{
		delete[] response_buf;
		response_buf = NULL;
	}

	m_bConnected = false;
	return ret;
}

const unsigned int CSMTP::GetPort()
{
	return m_nPort;
}

const char * CSMTP::GetLastError()
{
	return m_sError.c_str();
}

bool CSMTP::SendMail(CMimeMessage& msg)
{
	if( ! transmit_message( msg ) )
	{
        cout << "Send e-mail failed" << endl;
		return false;
	}
	return true;
}
/*
BOOL CSMTP::FormatMailMessage( CMailMessage* msg )
{
	ASSERT( msg != NULL );
   if( msg->GetNumRecipients() == 0 )
	{
		m_sError = ( "No Recipients" );
		return FALSE;
	}
	msg->FormatMessage();
	return TRUE;
}
*/
void CSMTP::SetServerProperties( const char * szSMTPServerName, unsigned int nPort)
{
//	ASSERT( szSMTPServerName != NULL );
	// Needs to be safe in non-debug too
	if( szSMTPServerName == NULL )
		return;
	m_sSMTPServerHostName = szSMTPServerName;
	m_nPort = nPort;
}

/*
CString CSMTP::cook_body(CMailMessage * msg)
{
	ASSERT( msg != NULL );
	CString sTemp;
	CString sCooked = ( "" );
	LPTSTR szBad = ( "\r\n.\r\n" );
	LPTSTR szGood = ( "\r\n..\r\n" );
	int nPos;
	int nStart = 0;
	int nBadLength = strlen( szBad );
	sTemp = msg->m_sBody;
	if( sTemp.Left( 3 ) == ( ".\r\n" ) )
		sTemp = ( "." ) + sTemp;
	//
	// This is a little inefficient because it beings a search
	// at the beginning of the string each time. This was
	// the only thing I could think of that handled ALL variations.
	// In particular, the sequence "\r\n.\r\n.\r\n" is troublesome. 
	// (Even CStringEx's FindReplace wouldn't handle that situation
	// with the global flag set.)
	//
	while( (nPos = sTemp.Find( szBad )) > -1 )
	{
		sCooked = sTemp.Mid( nStart, nPos );
		sCooked += szGood;
		sTemp = sCooked + sTemp.Right( sTemp.GetLength() - (nPos + nBadLength) );
	}
	return sTemp;
}
*/

bool CSMTP::transmit_message(CMimeMessage& msg)
{
	PrintLog("========transmit_message========\n");
	char strTemp[1024]={0};
	string sFrom;
	string sTo;
	string sTemp;
	string sEmail;

	if( !m_bConnected )
	{
		m_sError =  "Must be connected";
        cout << "Must be connected" << endl;
		return false;
	}

	// Send the MAIL command
	//
	sFrom = "MAIL From: <";
    sFrom += msg.GetFrom();
    sFrom += ">\r\n";

    cout << sFrom.c_str() << endl;
	send(m_wsSMTPServer, sFrom.c_str(), static_cast<int>(sFrom.length()), 0);
//------------------------------------------------------------
	if( !get_response( GENERIC_SUCCESS ) )
	{
		sprintf(strTemp,"send [%s] failer!",sFrom.c_str());
		PrintLog(strTemp);
		return false;
	}
    
    sTo = "RCPT TO: <";
    sTo += msg.GetTo();
    sTo += ">\r\n";

    cout << sTo.c_str() << endl;
    send(m_wsSMTPServer, sTo.c_str(), static_cast<int>(sTo.length()), 0);
    //---------------------------------------------------------
	if( !get_response( GENERIC_SUCCESS ) )
	{		
		sprintf(strTemp,"send [%s] failer!",sTo.c_str());
		PrintLog(strTemp);		
		return false;
	}

	// Send the DATA command
	sTemp = "DATA\r\n";

    cout << sTemp.c_str() << endl;
	send(m_wsSMTPServer, sTemp.c_str(), static_cast<int>(sTemp.length()), 0);
	//--------------------------------------------------------
	if( !get_response( DATA_SUCCESS ) )
	{
		sprintf(strTemp,"send [%s] failer!",sTemp.c_str());
		PrintLog(strTemp);	
		return false;
	}
    int nSize = msg.GetLength();
	char *pBuff = new char[nSize];
    msg.Store(pBuff, nSize);
    cout << pBuff << endl;
	send(m_wsSMTPServer, pBuff, static_cast<int>(strlen(pBuff)), 0);
	PrintLog(pBuff);
    delete pBuff;
	//-------------------------------------------------------

	// Signal end of data
	//
	sTemp = "\r\n.\r\n";
    cout << sTemp.c_str() << endl;
	send(m_wsSMTPServer, sTemp.c_str(), static_cast<int>(sTemp.length()), 0);
	//-------------------------------------------------------
	if( !get_response( GENERIC_SUCCESS ) )
	{		
		sprintf(strTemp,"send [%s] failer!",sTemp.c_str());
		PrintLog(strTemp);	
		return false;
	}
	PrintLog("========transmit_message End========\n");
	return true;
}

bool CSMTP::get_response( unsigned int response_expected )
{
//	ASSERT( response_expected >= GENERIC_SUCCESS );
//	ASSERT( response_expected < LAST_RESPONSE );

	//CString sResponse;
	PrintLog("========get_response========\n");
	char strTemp[1024]={0};
	unsigned int response;
	response_code* pResp;	// Shorthand
    int nRecv = 0;

    if(WaitforReplay(m_wsSMTPServer, m_nTimeout) > 0)
    {

	    if( (nRecv = recv(m_wsSMTPServer, response_buf, RESPONSE_BUFFER_SIZE, 0)) == SOCKET_ERROR )
	    {
		    m_sError = "Socket Error";
		    return false;
	    }
 //-------------------------------------------------------------------
        response_buf[nRecv] = '\0';
        char chResp[4] ={0} ;
        strncpy(chResp, response_buf, 3);
        chResp[3] = '\0';
	    response = atoi(chResp);///, "%d", &response );
	    pResp = &response_table[ response_expected ];
		sprintf(strTemp,"response=%d",response);
		PrintLog(strTemp);
		sprintf(strTemp,"pResp->nResponse=%d",pResp->nResponse);
		PrintLog(strTemp);
	    if( response != pResp->nResponse )
	    {
            m_sError = response_buf;
            cout << "Error Message:  " << m_sError.c_str() << endl;
			PrintLog(m_sError.c_str());	
			PrintLog("response!=pResp->nResponse");
			PrintLog("========get_response End========\n");
		    return false;
	    }
        else
        {
			PrintLog(response_buf);
            cout << "Success Message:  " << response_buf << endl;
        }
		PrintLog("调用WaitforReplay成功！");
		PrintLog("========get_response End========\n");
	    return true;
    }
	PrintLog("========get_response End========\n");
    return false;
}

void CSMTP::Cancel()
{
   //*** Cancel a blocking call currently in progress.
   //*** During connection, the process cannot be cancelled
   //*** so you will have to wait about the connection has been 
   //*** complete before cancellation.. :0(
	//------------------------------------------
//   m_wsSMTPServer.CancelBlockingCall();
	//------------------------------------------
   //*** Since cancellation is drastically operation and may be
   //*** called "op閞ation draconnienne drastique :0)", we should
   //*** close the socket immediately.
   //-------------------------------------------
   /*
   m_wsSMTPServer.Close();
   */
   closesocket(m_wsSMTPServer);
   //-------------------------------------------
}

int CSMTP::connect_timeo(int sockfd, const struct sockaddr *saptr, int salen, int nsec)
{
	int n;
	int error;
#ifdef WIN32
	int len;
#else
    socklen_t len;
#endif
	fd_set rset,wset;
	struct timeval tv;
	
	u_long val = 1;
#ifdef WIN32
	ioctlsocket(sockfd,FIONBIO,&val);
#endif

	error = 0;
	if((n = connect(sockfd,saptr,salen)) < 0)
	{
#ifdef WIN32
		if(WSAGetLastError() != WSAEWOULDBLOCK) 
		{
			return -1;
		}
#endif
	}	

	if(n == 0)
		goto done;

	FD_ZERO(&rset);
	FD_SET(sockfd,&rset);
	wset = rset;
	tv.tv_sec = nsec;
	tv.tv_usec = 0;

	if((n = select(sockfd + 1,&rset,&wset,NULL, &tv)) == 0)
	{
		closesocket(sockfd);
#ifdef WIN32
		WSASetLastError(WSAETIMEDOUT);
#endif
		return -1;
	}

	if(FD_ISSET(sockfd,&rset) || FD_ISSET(sockfd,&wset))
	{
		len = sizeof(error);
		if(getsockopt(sockfd,SOL_SOCKET,SO_ERROR,(char *)&error,&len) < 0)
		{
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
#ifdef WIN32
	ioctlsocket(sockfd,FIONBIO,&val);
#endif
	if(error)
	{
		closesocket(sockfd);
#ifdef WIN32
		WSASetLastError(error);
#endif
		return -1;
	}	
	return 0;
}

int CSMTP::WaitforReplay(int sockfd, long msec)
{
	struct timeval Timeout;
	fd_set rset, wset;
	FD_ZERO(&rset);
	FD_SET(sockfd, &rset);
	wset = rset;
	Timeout.tv_sec = msec;
    Timeout.tv_usec = 0;
	return(select(sockfd + 1, &rset, &wset, NULL, &Timeout));
}

