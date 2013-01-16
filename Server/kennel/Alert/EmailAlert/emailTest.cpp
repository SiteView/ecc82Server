// emailTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "mysendmail.h"
using namespace SiteView_MySenderMail;


#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

void WriteLog( const char* str )
{
	//puts( str );

	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	char szLogFile[] = "emailAlert.log";

	// 判断文件大小：在不打开文件的情况下实现
	/*
	struct _stat buf;
	if( _stat( szLogFile, &buf ) == 0 )
	{
		if( buf.st_size > 10*1024 )
		{
			FILE *log = fopen( szLogFile, "w" );
			if( log != NULL )
				fclose( log );
		}
	}
	*/


	FILE *log = fopen( szLogFile,"a+");
	if( log != NULL )
	{
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fclose( log );
	}

}


int _tmain(int argc, _TCHAR* argv[])
{
	MySendMail* mail = MySendMail::GetInstance();
	mail->content = "hello world";
	mail->mailSubject = "test";
/*	mail->password = "197010";
	mail->user = "jiewen.zhang@dragonflow.com";
	mail->receiverAddress = "jiewen.zhang@dragonflow.com";
	mail->receiverName = "Customer";
	mail->senderAddress = "jiewen.zhang@dragonflow.com";
	mail->sendName = "SiteView";
	mail->smptAddress = "mail.dragonflow.com";

	mail->SendMail ();
*/

	mail->password = "siteviews1gjkxt!";
	mail->user = "siteviewmonitor";
	mail->receiverAddress = "siteview_test@ctrip.com";
	mail->receiverName = "siteview_test@ctrip.com";
	mail->senderAddress = "siteview_test@ctrip.com";
	mail->sendName = "siteview_test@ctrip.com";
	mail->smptAddress = "192.168.100.242";
	mail->SendMail ();
	return 0;
}

