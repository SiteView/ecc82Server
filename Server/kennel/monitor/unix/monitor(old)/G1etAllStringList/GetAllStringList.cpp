// GetAllStringList.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GetAllStringList.h"
#include "..\..\base\funcGeneral.h"
#include <list>
#include "../../opens/boost/regex.hpp"

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

void WriteToLog(CString str);
BOOL GetBStrigByList(CStringList &lstText,CStringList &lstValue, CString&strRet);
BOOL GetStringlistByMark(CStringList &lst,CString strContent, CString strMark);
int GetAllDynData(CString strDLLName,CString strFuncName,char *strParam, char* strRet);

CString GetRootPath()
{
	return FuncGetInstallRootPath();

}
void PaserCon( );
void SendTrap();
void cal1(int x0,int x1, int y0, int y1)
{
	float result;
	result =8*(y1-y0)/(x1-x0)/1024;
	printf( "result is %.2f",result);
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
	cal1(1157696331 ,1157696717 , 2203333652 ,2221543059);

	char strParam[1024]={0};
	char strRet[1024]={0};

	sprintf(strParam,"/dev/mapper/VolGroup00-LogVol00###/$$$/dev/sda1###/boot$$$none###/dev/shm$$$");
	printf("aaaa\n");
	


   //PaserCon();
	//SendTrap();
	int nr=GetAllDynData("C:\\Program Files\\Apache Group\\apache2\\fcgi-bin\\monitor.dll"
		,"LOGFILE",strParam,strRet);
//	CString  rootpath= FuncGetInstallPath();
//	puts(rootpath);
	//int nr=GetAllDynData("d:\\v70\\MonitorManager\\SnmpMonitor.dll","SNMPDone",strParam,strRet);

	
	return nRetCode;
}
void sss()
{

}

void SendTrap()
{
	typedef BOOL (*SNMPINFO)(char* ip, char* trapip, int nPort,char *comm,char *srcoid, char *payoid,char * payvalue) ;
	HMODULE hm=::LoadLibrary("snmptrap.dll");
	if(hm)
	{
		SNMPINFO pfunc=(SNMPINFO)::GetProcAddress(hm,"SendTrapInfo");
		if(pfunc)
		{
			int nRet =pfunc("10.3.22.33","10.3.47.251",   162,   "public",
				"1.3.6.1.6.3.1.1.5.1",
				//"1.3.6.1.2.1.11.0.3",
				"1.3.6.1.4.1.11.2.16.2","ÓÎÁú¿Æ¼¼ test trap" );

		}
		FreeLibrary(hm);

	}
	


}
bool writechar( const char * pInput )
{
	const char * p;
	int nSize;
	p=pInput;
	while(*p!='\0')
	{
		nSize =strlen(p);
		if( nSize>0 )
		{	
			
			printf("%s\n" ,p);
		}
		p=p+nSize+1;
	}

	return true;
}
int GetAllDynData(CString strDLLName,CString strFuncName,char  *strParam, char * strRet)
{
		HMODULE hm=::LoadLibrary(strDLLName);
	if(hm)
	{
		PEXECMONITOR pfunc=(PEXECMONITOR)::GetProcAddress(hm,strFuncName);
		if(pfunc)
		{
			char a[4096]={0};
			char b[10000]={0};
				char aa[1024]={0};
				int bs=10000;

				char *p;
					FILE *fp;
				fp= fopen("in2.txt","r");
				if(fp==NULL) return -1;
				p=a;
				 while( fgets(aa,1023,fp)!=NULL)
				 {
					strcat(p,aa);
					p=p+strlen(p);
					*(p-1)=0;
					
					 
				 }
				fclose(fp);


				memset(b,0,10000);
				if(!(*pfunc)(a,b,bs))
				{

				}


				//printf("%s",b);
				writechar(b);
		
		}
		FreeLibrary(hm);
	}else{
		DWORD dwErr= GetLastError();
		printf("%d",dwErr);
	}
	return 0;
}


void ParserContent( char *  content,int &lTotal, int & lMatch ,char *matchstr )
{
	char *p,*pMatch;
	p=(char *) content;
	pMatch =content;
	lTotal=0;
	lMatch =0;
	//matchstr;
	
    while (*p!=0)
	{
		if(*p=='\n')
		{
			*p=0;
			lTotal++;
			static const boost::regex e(matchstr);
			std::string strMatch;
			strMatch =pMatch;
			bool bMatch= regex_match(strMatch, e);
			if(bMatch)
				lMatch++;	
			pMatch=p+1;

		}
		p++;

	}
}
void PaserCon( )
{
	char a[4096]={0};
	char b[4000]={0};
	char aa[1024]={0};
	int bs=4000;

	char *p;
		FILE *fp;
	fp= fopen("in2.txt","r");
	if(fp==NULL) return ;
	p=a;
	while( fgets(aa,1023,fp)!=NULL)
	{
	strcat(p,aa);
	p=p+strlen(p);
//	*(p-1)=0;
	
		
	}
	fclose(fp);

	int ltotal;
	int lmatch;
	ParserContent(a,ltotal,lmatch,"sv_dependscond\\s");

}