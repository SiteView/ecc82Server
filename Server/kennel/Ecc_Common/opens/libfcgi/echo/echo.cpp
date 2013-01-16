/*
 * echo.c --
 *
 *	Produce a page containing all FastCGI inputs
 *
 *
 * Copyright (c) 1996 Open Market, Inc.
 *
 * See the file "LICENSE.TERMS" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 */
#ifndef lint
static const char rcsid[] = "$Id: echo.c,v 1.5 1999/07/28 00:29:37 roberts Exp $";
#endif /* not lint */

//#include "fcgi_config.h"
//#include "fcgi_config_x86.h"










#include <process.h>
#include <string>
#include "fcgi_stdio.h"
#include <time.h>
#include <Windows.h>


using namespace std;
string FuncGetDataByPostMethod();

void WriteLog(char *app)
{
	FILE *fp;
	fp=fopen("c:\\echo.txt","a+");
	fprintf(fp,app);
	fclose(fp);

}

static void PrintEnv(char *label, char **envp)
{
	int i;
    FCGI_printf("%s:<br>\n<pre>\n", label);
 
	for ( i = 0; envp[i]; ++i)
	{
		//putenv(envp[i]);
		FCGI_printf("%s\n", envp[i]);
	
    }
    FCGI_printf("</pre><p>\n");
}
    int count = 0;
BOOL AcceptPrintf(char * pidEventName)
{
	char **initialEnv = environ;
	char  wcookie[256];
	char  *p;
	int   nSvsid;
	std::string strPostData;

	SYSTEMTIME mytime;
	nSvsid=0;

	WriteLog("befor accept");
	if(FCGI_Accept()>=0){

		char *qrystr;
		char szWrite[256];
		sprintf(szWrite,"pid %d  accept data\n",GetCurrentProcessId() );
		WriteLog(szWrite);

		environ=FCGI_GetEnv();
		
		
		qrystr=getenv("HTTP_COOKIE");
		if(qrystr)
		{
			if(strlen(qrystr)>0)
			{
				sprintf(wcookie,"%s",qrystr);
				p=strstr(wcookie,"svsid=");
				if(p!=NULL)
				{
					sscanf(p,"svsid=%d",&nSvsid);
				}
			}
		}
		
		

		GetLocalTime(&mytime);
		
		
				
		FCGI_printf("Content-type: text/html\r\n"
			//printf("Content-type: text/html\r\n"
			"Set-Cookie: svsid=%d\r\n"
			"Set-Cookie: userid=1\r\n\r\n"
			"\r\n"
			"<title>FastCGI echo</title>"
			"<h1>FastCGI echo</h1>\n"
			"Time is %d-%d-%d"
			"wangpeng Request number %d,  Process ID: %d<p>\n"
			"<h1>event name is %s<h1><p>"
			"<h1>Cookie is %s  ,sessionid is %d</h1><p>",
			GetCurrentProcessId(),
			mytime.wHour, mytime.wMinute,mytime.wSecond, 
			++count, GetCurrentProcessId(),
			pidEventName,wcookie,nSvsid);
		printf("abc");
		strPostData=  FuncGetDataByPostMethod();
		FCGI_printf(strPostData.c_str());
		FCGI_Finish();
		return TRUE;
	}else
	{

		return FALSE;
	}
	
	
}
#define  SITEVIEW

int main (int argc,char *argv[])
{
    
	char pidEventName[256];
	HANDLE hCgiEvent;
	DWORD ret;
	int nTest;
	char *p,*p1;

    sprintf(pidEventName,"SiteView-Cgi-Event-%d",GetCurrentProcessId());	
    hCgiEvent = CreateEvent(NULL,TRUE,FALSE,pidEventName);
	
	while(TRUE)
	{
		DWORD ret = WaitForSingleObject(hCgiEvent, 600 * 1000);
		if(ret==WAIT_TIMEOUT)  break;
		ResetEvent(hCgiEvent);

		if(!AcceptPrintf(pidEventName)) break;
	}
		
    return 0;
}


string FuncGetDataByPostMethod()
{
	char *cl;
	int sum;
	
	cl=getenv("CONTENT_LENGTH");
	string strGet = "";	
	
	if (cl)
	{
		sum = atoi(cl) + 1;
		
		char * buffer = NULL;
		buffer = (char *)malloc(sum);
		
		if(!buffer)
			return strGet;
		memset(buffer, 0, sum);
		
		fgets(buffer, sum, stdin);

		strGet = buffer;		
		
		if (buffer)
		{
			free(buffer);
			buffer = NULL;
		}
	}

	return strGet;
}
