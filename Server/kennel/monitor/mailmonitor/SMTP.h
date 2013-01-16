// SMTP.h: interface for the CSMTP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMTP_H__7652DDC5_6F17_4A7C_8D59_0D2EC7F68E32__INCLUDED_)
#define AFX_SMTP_H__7652DDC5_6F17_4A7C_8D59_0D2EC7F68E32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "MailMessage.h"

#define SMTP_PORT 25		// Standard port for SMTP servers
#define RESPONSE_BUFFER_SIZE 1024
#define	CONNECT_TIMEOUT		5

#include <string>

using namespace std;

class CMimeMessage;
/*
struct response_code
{
	unsigned int nResponse;		// Response we're looking for
	string  szMessage;	// Error message if we don't get it
/*
public:
    response_code(unsigned int nResp, const char * pszMessage)
    {
        nResponse = nResp; szMessage = pszMessage;
    }

};
*/

class CSMTP  
{
public:
	CSMTP( const char * szSMTPServerName, unsigned int nPort = SMTP_PORT, unsigned int nTimeout = CONNECT_TIMEOUT );
	virtual ~CSMTP();

    void SetServerProperties( const char * szSMTPServerName, unsigned int nPort = SMTP_PORT );
    void Cancel(); // <JFO>

	const char * GetServerHostName();	
    const char * GetLastError();
	const unsigned int GetPort();

	bool Disconnect();
	bool Connect(const int &nNeedAuth, const int &nNeedSPA, const char * pszUsername, const char * pszPassword);
	bool SendMail( CMimeMessage& msg );

	int  connect_timeo(int sockfd, const struct sockaddr *saptr, int salen, int nsec);
	//virtual BOOL FormatMailMessage( CMailMessage* msg );

private:
	bool get_response( unsigned int response_expected );

    bool NTLM_Login(const char* pszUsername, const char *pszPWD, const char* pszComputerName);

    int  WaitforReplay(int sockfd, long msec);
	//CString cook_body( CMailMessage* msg );

    string m_sError;
    string m_sSMTPServerHostName;
	
    bool m_bConnected;
	unsigned int m_nPort;
	
	int m_wsSMTPServer;
    int m_nTimeout;
protected:
	bool transmit_message(CMimeMessage& msg );
//
// Helper Code
//
    struct response_code
    {
	    unsigned int nResponse;		// Response we're looking for
	    char *  szMessage;	// Error message if we don't get it
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
	char * response_buf;
	static response_code response_table[];
};

#endif // !defined(AFX_SMTP_H__7652DDC5_6F17_4A7C_8D59_0D2EC7F68E32__INCLUDED_)
