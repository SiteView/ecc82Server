
#include "stdafx.h"
#include "emailalert.h"
#include "mysendmail.h"
using namespace SiteView_MySenderMail;

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

void WriteLog( const char* str )
{
	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	char szLogFile[] = "emailAlert.log";

	// 判断文件大小：在不打开文件的情况下实现
	/**/
	struct _stat buf;
	if( _stat( szLogFile, &buf ) == 0 )
	{
		if( buf.st_size > 10000*1024 )
		{
			FILE *log = fopen( szLogFile, "w" );
			if( log != NULL )
				fclose( log );
		}
	}
	


	FILE *log = fopen( szLogFile,"a+");
	if( log != NULL )
	{
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fclose( log );
	}

}
 
DLL_EXPORT bool DoAlert(const char * szContent, 
                        const char * szDest /* = NULL  */,
                        const char * szSubject /* = NULL */)
{
    bool bRet = SendEmail("mail.dragonflow.com","jiewen.zhang@dragonflow.com",
		szDest,szSubject,szContent,"jiewen.zhang@dragonflow.com","197010");
/*    CSMTP smtp("mail.dragonflow.com");
    if(smtp.Connect("kevin.yang@dragonflow.com", "kevin.yang"))
    {
        CMimeMessage mail;
        mail.SetFrom("wanli.wu@dragonflow.com");
        mail.SetTo(szDest);
        mail.SetSubject(szSubject);
        mail.SetText(szContent);
        bRet = smtp.SendMail(mail);
        smtp.Disconnect();
    }
 */   
    return bRet;
}


DLL_EXPORT bool SendEmail(const char *pszServer, const char *pszMailfrom, 
                          const char *pszMailTo, const char *pszSubject,
                          const char * pszMailContent, const char *pszUser, 
                          const char *pszPassword)
{
	WriteLog("=========== SendEmail ==========");
	WriteLog( pszServer );
	WriteLog( pszMailfrom );
	WriteLog( pszMailTo );
	WriteLog( pszSubject );
	WriteLog( pszMailContent );
	WriteLog( pszUser );
	WriteLog( pszPassword );


    bool bRet = false;
 	MySendMail* mail = MySendMail::GetInstance();
/*	strcpy(mail->content,pszMailContent);
	strcpy(mail->mailSubject,pszSubject);
	strcpy(mail->password,pszPassword);
	strcpy(mail->user,pszUser);
	strcpy(mail->receiverAddress,pszMailfrom);
	strcpy(mail->receiverName ,"Customer");
	strcpy(mail->senderAddress,pszMailTo);
	strcpy(mail->sendName, "SiteView");
	strcpy(mail->smptAddress,pszServer);

	bRet=mail->SendMail ();
*/
	mail->content=(char *)pszMailContent;
	mail->mailSubject=(char *)pszSubject;
	mail->password=(char *)pszPassword;
	mail->user=(char *)pszUser;
	mail->receiverAddress=(char *)pszMailTo;
	mail->receiverName="Customer";
	mail->senderAddress=(char *)pszMailfrom;
	mail->sendName= "SiteView";
	mail->smptAddress=(char *)pszServer;

	bRet=mail->SendMail ();


/*
    CSMTP smtp(pszServer);
    if(smtp.Connect(pszUser, pszPassword))
    {
        CMimeMessage mail;
        mail.SetFrom(pszMailfrom);
        mail.SetTo(pszMailTo);
        mail.SetSubject(pszSubject);
        mail.SetText(pszMailContent);
        bRet = smtp.SendMail(mail);
        smtp.Disconnect();
    }
    else
    {
#if WIN32
        OutputDebugString("Connect failed\n");
        OutputDebugString("Error information: ");
        OutputDebugString(smtp.GetLastError());
        OutputDebugString("\n");
#endif
    }
 */   
    return bRet;
}