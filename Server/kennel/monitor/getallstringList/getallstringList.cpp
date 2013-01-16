// GetAllStringList.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GetAllStringList.h"
#include <Base\funcGeneral.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

#define		SEPARATOR		"@@@"
//#define KEY_PATH		"SOFTWARE\\Aim"
//#define KEY_NAME		"RootPath"

typedef BOOL (*PEXECMONITOR)(   char* ,char * ,int&  );
typedef BOOL (*PEXECMONITOR_NEW)(   char* ,int& ,char * ,int&  );

void WriteToLog(CString str);
BOOL GetBStrigByList(CStringList &lstText,CStringList &lstValue, CString&strRet);
BOOL GetStringlistByMark(CStringList &lst,CString strContent, CString strMark);
int GetAllDynData(CString strDLLName,CString strFuncName,char *strParam, char* strRet);

CString GetRootPath()
{
	return FuncGetInstallRootPath();

}


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	//return 0;
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
    	return nRetCode;
	}

	

	char strParam[1024]={0};
	char strRet[1024]={0};

	//strParam
	int nr=GetAllDynData("C:\\Program Files\\Apache Group\\apache2\\fcgi-bin\\oracle.dll","SYSTEMDSN",strParam,strRet);

	return nRetCode;
}
int GetAllDynData(CString strDLLName,CString strFuncName,char  *strParam, char * strRet)
{
		HMODULE hm=::LoadLibrary(strDLLName);
	if(hm)
	{
		PEXECMONITOR pfunc=(PEXECMONITOR)::GetProcAddress(hm,strFuncName);
		if(pfunc)
		{
			char a[4000]={0};
				char b[10000];
				int bs=10000;
				char *p;
				memset(b,0,10000);
				sprintf(a,"%s","_MachineName=127.0.0.1@_UserAccount=98@_PassWord=jjjjj@_Disk=C:@");
/*
				FILE *fp;
				fp= fopen("C:\\Program Files\\Apache Group\\apache2\\fcgi-bin\\in.txt","r");
				if(fp==NULL) return -1;
				
				fgets(a,4095,fp);
				fclose(fp);
*/
				p=a;

				int length(0);
				while(*p!='\0')
				{
					if(*p=='\n') break;
					if(*p=='@') *p='\0';
					p++;
					length++;
				
				}

				if( strDLLName.Find("dnsmonitor.dll")!=-1 && strFuncName=="GetDiskInfo" )
				{
					PEXECMONITOR_NEW pfunc=(PEXECMONITOR_NEW)::GetProcAddress(hm,strFuncName);
					if( !(*pfunc)(a,length,b,bs)  )
						printf("func error");
				}else pfunc(a,b,bs);



					
		
				p=(char *)b;
				int nSize;
				while(*p!='\0')
				{
					nSize =strlen(p);
					printf("%s\n", p);
					p=p+nSize+1;
				}
				

		
		}
		FreeLibrary(hm);
	}else{
		DWORD dwErr= GetLastError();
		printf("error:%d",dwErr);
	}
	return 0;
}

//int test_DNS()
//{
//
//};
