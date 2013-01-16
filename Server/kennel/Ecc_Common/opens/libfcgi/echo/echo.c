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
#include "fcgi_config_x86.h"

#include <stdlib.h>

#include <memory.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <Windows.h>
//#undef _WIN32

#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif


#include "fcgi_stdio.h"
#include <time.h>
#include <string.h>
#include <windows.h>


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

	SYSTEMTIME mytime;
	nSvsid=0;
	
//WritePrivateProfileString("aa","aa1",pidEventName,"c:\\echo.ini");	
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
				//sscanf(wcookie ,"svsid=%d",&nSvsid);
				p=strstr(wcookie,"svsid=");
				if(p!=NULL)
				{
					sscanf(p,"svsid=%d",&nSvsid);
				}

				//Sleep(5000);
			}
		}
		
		

		GetLocalTime(&mytime);
		
		
				
		FCGI_printf("Content-type: text/html\r\n"
			//printf("Content-type: text/html\r\n"
			"Set-Cookie: pid=%d&svsid=%d\r\n"
			"\r\n"
			"<title>FastCGI echo</title>"
			"<h1>FastCGI echo</h1>\n"
			"Time is %d-%d-%d"
			"wangpeng Request number %d,  Process ID: %d<p>\n"
			"<h1>event name is %s<h1><p>"
			"<h1>Cookie is %s  ,sessionid is %d</h1><p>",
			GetCurrentProcessId(),
			nSvsid,
			mytime.wHour, mytime.wMinute,mytime.wSecond, 
			++count, GetCurrentProcessId(),
			pidEventName,wcookie,nSvsid);
		printf("abc");
	//	PrintEnv("Request environment", environ);
	//	PrintEnv("Initial environment", initialEnv);
		//return TRUE;
		FCGI_Finish();
		return TRUE;
	}else
	{
	//	WritePrivateProfileString("aa","aa","false","c:\\echo.ini");
		return FALSE;
	}
	
	
}
#define  SITEVIEW

int main (int argc,char *argv[])
{
    

#ifdef SITEVIEW
	char pidEventName[256];
	HANDLE hevnt;
	DWORD ret;
	int nTest;
	//char tmp[256];
	char *p,*p1;
	char testcokie[256];
	sprintf(testcokie,"%s","pid=0&svsid=1147335476&sdsfsd=454");
	p=strstr(testcokie,"svsid=");
	if(p!=NULL)
	{
		sscanf(p,"svsid=%d",&nTest);
	}
	
//	nTest=0;

	//sscanf("this is a test\n", "%*1s%s", tmp);

		
	//sscanf(testcokie,"%s[^svsid=]%d",tmp,&nTest);

//	return;
	
	
	sprintf(pidEventName,"SiteView-Cgi-Event-%d",GetCurrentProcessId());	
	//sprintf(pidEventName,"aaa",GetCurrentProcessId());	
	//printf(pidEventName);
	hevnt= CreateEvent(NULL,TRUE,FALSE,pidEventName);
	ResetEvent(hevnt);
	

	while(TRUE)
	{
		

		WriteLog(pidEventName);		
		WriteLog("  wait for hevent\n");		
		
		//WaitForSingleObject(hevnt,INFINITE);
		ret= WaitForSingleObject(hevnt,60000);
		if(ret==WAIT_TIMEOUT)  break;
		ResetEvent(hevnt);
		WriteLog("  wait hevent success\n");		

		if(!AcceptPrintf(pidEventName)) break;
		
			
	}
#else
	MSG msg;
	DWORD a =GetCurrentThreadId();
	printf("%d\r\n",a);
	while(TRUE)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			WritePrivateProfileString("aa","aa","0dfd","c:\\echo.ini");
			
			switch(msg.message)
				{
				case WM_CHECKTASKS:
					AcceptPrintf();
					break;
				
				default:
					TranslateMessage(&msg);
					 DispatchMessage(&msg);
				}
	
		}else{
			if(!WaitMessage())  break;
				//throw MSException("Contrl thread exit");
		}
	}
	
#endif		
		
    return 0;
}


