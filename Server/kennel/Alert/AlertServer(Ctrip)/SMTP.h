// SMTP.h: interface for the CSMTP class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __SMTP_H__
#define __SMTP_H__

#include "MailMessage.h"
#include "Base64.h"

#define SMTP_PORT 25		// Standard port for SMTP servers
#define RESPONSE_BUFFER_SIZE 1024
#define	CONNECT_TIMEOUT		60

class CSMTP  
{
public:
	CSMTP( LPCTSTR szSMTPServerName, UINT nPort = SMTP_PORT );
	virtual ~CSMTP();

	void SetServerProperties( LPCTSTR szSMTPServerName, UINT nPort = SMTP_PORT );
	CString GetLastError();
	UINT GetPort();
	BOOL Disconnect();
	BOOL Connect(int nNeedAuth, CString vUid, CString vPwd);
	int connect_timeo(int sockfd,const struct sockaddr *saptr,int salen,int nsec);
	virtual BOOL FormatMailMessage( CMailMessage* msg );
	BOOL SendMessage( CMailMessage* msg );
	CString GetServerHostName();

   void Cancel(); // <JFO>

private:
	BOOL get_response( UINT response_expected );
	CString cook_body( CMailMessage* msg );

	CString m_sError;
	BOOL m_bConnected;
	UINT m_nPort;
	CString m_sSMTPServerHostName;
	int m_wsSMTPServer;

protected:
	virtual BOOL transmit_message( CMailMessage* msg );

//
// Helper Code
//
	struct response_code
	{
		UINT nResponse;		// Response we're looking for
		TCHAR* sMessage;	// Error message if we don't get it
	};

	enum eResponse
	{
		GENERIC_SUCCESS = 0,
		CONNECT_SUCCESS,
		DATA_SUCCESS,
		QUIT_SUCCESS,
		// Include any others here
		AUTH_RESPONSE,
		AUTH_SUCCESS,
		LAST_RESPONSE	// Do not add entries past this one
	};
	TCHAR *response_buf;
	static response_code response_table[];
};

#endif // !defined(AFX_SMTP_H__7652DDC5_6F17_4A7C_8D59_0D2EC7F68E32__INCLUDED_)
