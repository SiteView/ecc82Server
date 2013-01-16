// SMTP.cpp: implementation of the CSMTP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SMTP.h"

#include "..\..\base\funcGeneral.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
// Static member initializers
//

// Note: the order of the entries is important.
//       They must be synchronized with eResponse entries. 
CSMTP::response_code CSMTP::response_table[] =
{
	// GENERIC_SUCCESS
	{ 250, _T( "SMTP server error" ) },

	// CONNECT_SUCCESS
	{ 220, _T( "SMTP server not available" ) },

	// DATA_SUCCESS
	{ 354, _T( "SMTP server not ready for data" ) },

	// QUIT_SUCCESS
	{ 221, _T( "SMTP server didn't terminate session" ) }, 

	// NEED AUTH
	{ 334, _T( "SMTP server not ready for auth" ) }, 

	// AUTH SUCCESS
	{ 235, _T( "SMTP server auth success" ) }, 
};


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSMTP::CSMTP( LPCTSTR szSMTPServerName, UINT nPort )
{
	m_sSMTPServerHostName = szSMTPServerName;
	m_nPort = nPort;
	m_bConnected = FALSE;
	//m_sError = _T( "正常" );//<%IDS_SMTP_1%>
	m_sError = ::FuncGetStringFromIDS("<%IDS_Pop3_25%>");
	response_buf = NULL;
}

CSMTP::~CSMTP()
{
	Disconnect();
}

CString CSMTP::GetServerHostName()
{
	return m_sSMTPServerHostName;
}

BOOL CSMTP::Connect(int nNeedAuth, CString vUid, CString vPwd)
{
	CString sHello;
	TCHAR local_host[ 80 ];	// Warning: arbitrary size
	if( m_bConnected )
		return TRUE;
	
	try
	{
		// This will be deleted in Disconnect();
		response_buf = new TCHAR[ RESPONSE_BUFFER_SIZE ];

		// I can't count on all class users' applications
		// to have exception-throwing operator-new implementations,
		// so I'll soul-kiss the ones that don't.
		if( response_buf == NULL )
		{
			//m_sError = _T( "内存溢出" );//<%IDS_SMTP_2%>
			m_sError = ::FuncGetStringFromIDS("<%IDS_SMTP_2%>");
			return FALSE;
		}
	}
	catch( CException *e )
	{
		response_buf = NULL;
		//m_sError = _T( "内存溢出" );//<%IDS_SMTP_3%>
		m_sError = ::FuncGetStringFromIDS("<%IDS_SMTP_3%>");
		delete e;
		return FALSE;
	}

	m_wsSMTPServer = socket(AF_INET, SOCK_STREAM, 0);
	if(m_wsSMTPServer == INVALID_SOCKET)
	{
		//m_sError = _T( "通信建立失败" );//<%IDS_SMTP_4%>
		m_sError = ::FuncGetStringFromIDS("<%IDS_SMTP_4%>");
		delete response_buf;
		response_buf = NULL;
		return FALSE;
	}

	struct hostent *hp;
	struct sockaddr_in serv;
	hp = gethostbyname(GetServerHostName());
	if(hp == NULL)
	{
		//m_sError = _T( "域名解析失败" );//<%IDS_SMTP_5%>
		m_sError = ::FuncGetStringFromIDS("<%IDS_SMTP_5%>");
		shutdown(m_wsSMTPServer, 0x02);
		closesocket(m_wsSMTPServer);
		delete response_buf;
		response_buf = NULL;
		return FALSE;
	}

	int i = 0;
	while(1)
	{
		if(hp->h_addr_list[i])
			memcpy(&serv.sin_addr,hp->h_addr_list[i],hp->h_length);
		else
		{
			//m_sError = _T( "主机连接失败" );//<%IDS_SMTP_6%>
			m_sError = ::FuncGetStringFromIDS("<%IDS_SMTP_6%>");
			shutdown(m_wsSMTPServer, 0x02);
			closesocket(m_wsSMTPServer);
			delete response_buf;
			response_buf = NULL;
			return FALSE;
		}
		serv.sin_family = AF_INET;
		serv.sin_port=htons(GetPort());
//		if (connect(m_wsSMTPServer, (struct sockaddr*)&serv, sizeof(serv)) != SOCKET_ERROR)
		if (connect_timeo(m_wsSMTPServer, (struct sockaddr*)&serv, sizeof(serv), CONNECT_TIMEOUT) == 0)
		{
			break;
		}
		i ++;
	}

	if( !get_response( CONNECT_SUCCESS ) )
	{
		//m_sError = _T( "主机无响应" );//<%IDS_SMTP_7%>
		m_sError = ::FuncGetStringFromIDS("<%IDS_SMTP_7%>");
		shutdown(m_wsSMTPServer, 0x02);
		closesocket(m_wsSMTPServer);
		delete response_buf;
		response_buf = NULL;
		return FALSE;
	}

	gethostname( local_host, 80 );
	if(nNeedAuth)
		sHello.Format( _T( "EHLO %s\r\n" ), local_host );
	else
		sHello.Format( _T( "HELO %s\r\n" ), local_host );
	send(m_wsSMTPServer, (LPCTSTR)sHello, sHello.GetLength(), 0);
	if( !get_response( GENERIC_SUCCESS ) )
	{
		//m_sError = _T("发送HELO命令失败");//<%IDS_SMTP_8%>
		m_sError = ::FuncGetStringFromIDS("<%IDS_SMTP_8%>");
		shutdown(m_wsSMTPServer, 0x02);
		closesocket(m_wsSMTPServer);
		delete response_buf;
		response_buf = NULL;
		return FALSE;
	}
	//get_response( GENERIC_SUCCESS ) ;
/*
	if( !get_response( GENERIC_SUCCESS ) )
	{
		//m_sError = _T("发送HELO命令失败");//<%IDS_SMTP_8%>
		m_sError = ::FuncGetStringFromIDS("<%IDS_SMTP_8%>");
		shutdown(m_wsSMTPServer, 0x02);
		closesocket(m_wsSMTPServer);
		delete response_buf;
		response_buf = NULL;
		return FALSE;
	}
	*/
	if(nNeedAuth)
	{
		CString sAuth = _T("");
		CBase64 *pBase64 = new CBase64();

		sAuth = _T("AUTH LOGIN\r\n");
		send(m_wsSMTPServer, (LPCTSTR)sAuth, sAuth.GetLength(), 0);
		if( !get_response( AUTH_RESPONSE ) )
		{
			//m_sError = _T("身份验证失败");//<%IDS_SMTP_9%>
			m_sError = ::FuncGetStringFromIDS("<%IDS_SMTP_9%>");
			shutdown(m_wsSMTPServer, 0x02);
			closesocket(m_wsSMTPServer);
			delete pBase64;
			pBase64 = NULL;
			delete response_buf;
			response_buf = NULL;
			return FALSE;
		}
		//get_response( AUTH_RESPONSE );

		sAuth.Format(_T("%s\r\n"), pBase64->Encode((LPCTSTR)vUid, vUid.GetLength()));
		send(m_wsSMTPServer, (LPCTSTR)sAuth, sAuth.GetLength(), 0);
		if( !get_response( AUTH_RESPONSE ) )
		{
			//m_sError = _T("发送身份验证用户名失败");//<%IDS_SMTP_10%>
			m_sError = ::FuncGetStringFromIDS("<%IDS_SMTP_10%>");
			shutdown(m_wsSMTPServer, 0x02);
			closesocket(m_wsSMTPServer);
			delete pBase64;
			pBase64 = NULL;
			delete response_buf;
			response_buf = NULL;
			return FALSE;
		}

		sAuth.Format(_T("%s\r\n"), pBase64->Encode((LPCTSTR)vPwd, vPwd.GetLength()));
		send(m_wsSMTPServer, (LPCTSTR)sAuth, sAuth.GetLength(), 0);
		if( !get_response( AUTH_SUCCESS ) )
		{
			//m_sError = _T("发送身份验证密码失败");//<%IDS_SMTP_11%>
			m_sError = ::FuncGetStringFromIDS("<%IDS_SMTP_11%>");
			shutdown(m_wsSMTPServer, 0x02);
			closesocket(m_wsSMTPServer);
			delete pBase64;
			pBase64 = NULL;
			delete response_buf;
			response_buf = NULL;
			return FALSE;
		}

		delete pBase64;
		pBase64 = NULL;
	}

	m_bConnected = TRUE;
	return TRUE;
}

BOOL CSMTP::Disconnect()
{
	BOOL ret;
	if( !m_bConnected )
		return TRUE;

	// Disconnect gracefully from the server and close the socket
	CString sQuit = _T( "QUIT\r\n" );
	send(m_wsSMTPServer, (LPCTSTR)sQuit, sQuit.GetLength(), 0);

	// No need to check return value here.
	// If it fails, the message is available with GetLastError
	ret = get_response( QUIT_SUCCESS );
	shutdown(m_wsSMTPServer, 0x02);
	closesocket(m_wsSMTPServer);

	if( response_buf != NULL )
	{
		delete[] response_buf;
		response_buf = NULL;
	}

	m_bConnected = FALSE;
	return ret;
}

UINT CSMTP::GetPort()
{
	return m_nPort;
}

CString CSMTP::GetLastError()
{
	return m_sError;
}

BOOL CSMTP::SendMessage(CMailMessage * msg)
{
	if( !m_bConnected )
	{
		//m_sError = _T( "通信未建立" );//<%IDS_SMTP_12%>
		m_sError = ::FuncGetStringFromIDS("<%IDS_SMTP_12%>");
		return FALSE;
	}
	if( FormatMailMessage( msg ) == FALSE )
	{
		return FALSE;
	}
	if( transmit_message( msg ) == FALSE )
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CSMTP::FormatMailMessage( CMailMessage* msg )
{
   if( msg->GetNumRecipients() == 0 )
	{
		//m_sError = _T( "无收件人地址" );//<%IDS_SMTP_13%>
	   m_sError = ::FuncGetStringFromIDS("<%IDS_SMTP_13%>");
		return FALSE;
	}
	msg->FormatMessage();
	return TRUE;
}

void CSMTP::SetServerProperties( LPCTSTR szSMTPServerName, UINT nPort)
{
	// Needs to be safe in non-debug too
	if( szSMTPServerName == NULL )
		return;
	m_sSMTPServerHostName = szSMTPServerName;
	m_nPort = nPort;
}


CString CSMTP::cook_body(CMailMessage * msg)
{
	CString sTemp;
	CString sCooked = _T( "" );
	LPTSTR szBad = _T( "\r\n.\r\n" );
	LPTSTR szGood = _T( "\r\n..\r\n" );
	int nPos;
	int nStart = 0;
	int nBadLength = strlen( szBad );
	sTemp = msg->m_sBody;
	if( sTemp.Left( 3 ) == _T( ".\r\n" ) )
		sTemp = _T( "." ) + sTemp;
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

BOOL CSMTP::transmit_message(CMailMessage * msg)
{
	puts("transmit message");
	CString sFrom;
	CString sTo;
	CString sTemp;
	CString sEmail;

	if( !m_bConnected )
	{
		//m_sError = _T( "通信未建立" );//<%IDS_SMTP_14%>
		m_sError = ::FuncGetStringFromIDS("<%IDS_SMTP_14%>");
		return FALSE;
	}

	// Send the MAIL command
	//
	sFrom.Format( _T( "MAIL From: <%s>\r\n" ), (LPCTSTR)msg->m_sFrom );
	send(m_wsSMTPServer, (LPCTSTR)sFrom, sFrom.GetLength(), 0);
	if( !get_response( GENERIC_SUCCESS ) )
		return FALSE;
	
	// Send RCPT commands (one for each recipient)
	//
	for( int i = 0; i < msg->GetNumRecipients(); i++ )
	{
		msg->GetRecipient( sEmail, sTemp, i );
		sTo.Format( _T( "RCPT TO: <%s>\r\n" ), (LPCTSTR)sEmail );
		send(m_wsSMTPServer, (LPCTSTR)sTo, sTo.GetLength(), 0);
		get_response( GENERIC_SUCCESS );
	}

   //*** Begin <JFO>
	   for( i = 0; i < msg->GetNumRecipients(CMailMessage::CC); i++ )
	   {
		   msg->GetRecipient( sEmail, sTemp, i, CMailMessage::CC );
		   sTo.Format( _T( "RCPT TO: <%s>\r\n" ), (LPCTSTR)sEmail );
		   send(m_wsSMTPServer, (LPCTSTR)sTo, sTo.GetLength(), 0);
		   get_response( GENERIC_SUCCESS );
	   }

      for( i = 0; i < msg->GetNumRecipients(CMailMessage::BCC); i++ )
	   {
		   msg->GetRecipient( sEmail, sTemp, i, CMailMessage::BCC );
		   sTo.Format( _T( "RCPT TO: <%s>\r\n" ), (LPCTSTR)sEmail );
		   send(m_wsSMTPServer, (LPCTSTR)sTo, sTo.GetLength(), 0);
		   get_response( GENERIC_SUCCESS );
	   }
   //*** End

	// Send the DATA command
	sTemp = _T( "DATA\r\n" );
	send(m_wsSMTPServer, (LPCTSTR)sTemp, sTemp.GetLength(), 0);
	if( !get_response( DATA_SUCCESS ) )
	{
		return FALSE;
	}
	// Send the header
	//
	send(m_wsSMTPServer, (LPCTSTR)msg->m_sHeader, msg->m_sHeader.GetLength(), 0);

	// Send the body
	//
	sTemp = cook_body( msg );
	send(m_wsSMTPServer, (LPCTSTR)sTemp, sTemp.GetLength(), 0);

	// Signal end of data
	//
	sTemp = _T( "\r\n.\r\n" );
	send(m_wsSMTPServer, (LPCTSTR)sTemp, sTemp.GetLength(), 0);
	if( !get_response( GENERIC_SUCCESS ) )
	{
		return FALSE;
	}
	return TRUE;
}



BOOL CSMTP::get_response( UINT response_expected )
{
	int		n = 0;
	char	*ca = response_buf;

	if(response_expected < GENERIC_SUCCESS || response_expected >= LAST_RESPONSE )
	{
		//m_sError = _T("未知错误");//<%IDS_SMTP_15%>
		m_sError = ::FuncGetStringFromIDS("<%IDS_SMTP_15%>");
		return FALSE;
	}

	CString sResponse;
	UINT response;
	response_code* pResp;	// Shorthand

	memset(response_buf, 0, RESPONSE_BUFFER_SIZE);
	
	
l:	if( (n = recv(m_wsSMTPServer, ca, RESPONSE_BUFFER_SIZE, 0)) == SOCKET_ERROR )
	{
		//m_sError = _T( "读数据失败" );//<%IDS_SMTP_16%>
		m_sError = ::FuncGetStringFromIDS("<%IDS_SMTP_16%>");
		return FALSE;
	}

	ca[n] = 0;

	if(ca[n-2] != '\r' || ca[n-1] != '\n')
	{
		ca = ca + n;
		goto l;
	}


	sResponse = response_buf;
	sscanf( (LPCTSTR)sResponse.Left( 3 ), _T( "%d" ), &response );
	pResp = &response_table[ response_expected ];
	
	///puts((LPCTSTR)pResp->sMessage);

	if((response_expected==AUTH_RESPONSE)&&(250==response))
	{
		// puts(sResponse);
		//char ms[] =5;
		//setsockopt(m_wsSMTPServer,SOL_SOCKET,SO_RCVTIMEO,(char *)&ms,2);
		goto l;
	}
	//	puts("------------------------------");

	if( response != pResp->nResponse )
	{
		m_sError.Format( _T( "%d:%s" ), response, (LPCTSTR)pResp->sMessage );
		return FALSE;
	}

	return TRUE;
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
	shutdown(m_wsSMTPServer, 0x02);
   closesocket(m_wsSMTPServer);
   //-------------------------------------------
}

int CSMTP::connect_timeo(int sockfd,const struct sockaddr *saptr,int salen,int nsec)
{
	int n;
	int error;
	int len;
	fd_set rset,wset;
	struct timeval tv;
	
	u_long val = 1;
	ioctlsocket(sockfd,FIONBIO,&val);

	error = 0;
	if((n = connect(sockfd,saptr,salen)) < 0)
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK) 
		{
			return -1;
		}
	}	

	if(n == 0)
		goto done;

	FD_ZERO(&rset);
	FD_SET(sockfd,&rset);
	wset = rset;
	tv.tv_sec = nsec;
	tv.tv_usec = 0;

	if((n = select(sockfd + 1,&rset,&wset,NULL,nsec?&tv:NULL)) == 0)
	{
		shutdown(sockfd, 0x02);
		closesocket(sockfd);
		WSASetLastError(WSAETIMEDOUT);
		return -1;
	}

	if(FD_ISSET(sockfd,&rset) || FD_ISSET(sockfd,&wset))
	{
		len = sizeof(error);
		if(getsockopt(sockfd,SOL_SOCKET,SO_ERROR,(char *)&error,&len) < 0)
		{
			shutdown(sockfd, 0x02);
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
	ioctlsocket(sockfd,FIONBIO,&val);
	if(error)
	{
		shutdown(sockfd, 0x02);
		closesocket(sockfd);
		WSASetLastError(error);
		return -1;
	}	
	return 0;
}