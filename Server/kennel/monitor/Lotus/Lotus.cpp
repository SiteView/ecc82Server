// Lotus.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Lotus.h"

#define NT 1;
#include <lapicinc.h>

#include <global.h>
#include <nsfdb.h>
#include <nsfsearc.h>
#include <osfile.h>
#include <names.h>
#include <nsferr.h>
#include <textlist.h>
#include <ods.h>
#include <odstypes.h>

#include <nsfdata.h>
#include <nsfdb.h>
#include <nsfnote.h>
#include <editods.h>
#include <stdnames.h>
#include <osmem.h>
#include <osmisc.h>
#include <ostime.h>
#include <oserr.h>
#include <nsfobjec.h>
#include <misc.h>
#include <osfile.h>
#include <kfm.h>                        /* SECKFMGetUserName */
     

#include <nsf.h>                        /* MAXUSERNAME, etc. */
#include <osenv.h>                      /* OSGetEnvironmentString */
#include <mail.h>                       /* MAIL_SENDTO_ITEM_NUM, etc. */
#include <mailserv.h>                   /* MailOpenMessageFile */
#include <neterr.h>                     /* ERR_NO_NETBIOS */
#include <clerr.h>                      /* ERR_SERVER_NOT_RESPONDING */


#include <log.h>
#include <lapiplat.h>
#include <osmem.h>
#include <addin.h>
#include <fstream>

using namespace std;

#include "stats.h"
//#include "C:\\notesapi\\include\\stats.h"

#include "NoteMail.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SENDMAILERROR 110
//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CLotusApp

BEGIN_MESSAGE_MAP(CLotusApp, CWinApp)
	//{{AFX_MSG_MAP(CLotusApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLotusApp construction

CLotusApp::CLotusApp()
{
	// TODO: add construction code here
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLotusApp object

CLotusApp theApp;

BOOL GetValue(char *Context,char *Name,char *Value,int nSpace, char *flag,int n);
BOOL CutFlag(char *src,char flag);

STATUS NF_Command(char *server ,char* szcommand,char * szResult);
STATUS NF_Statics(char *server ,char *Facility,char *StatName,char * szResult);
void WritePassword(const char * szPwd);

//NFLIST
STATUS LNPUBLIC file_action (void *, SEARCH_MATCH *, ITEM_TABLE *);
STATUS LNPUBLIC print_file_summary (ITEM_TABLE *, char*);
void PrintAPIError (STATUS error, char* szReturn);
VOID print_usage (void);
BOOL NSF_List(CString strServer, CString strDirectory, 
					   CStringList &lstTexts,
					   CStringList &lstValues);

STATUS LNPUBLIC file_action_list
            (void *szReturn,
            SEARCH_MATCH far *pSearchMatch,
            ITEM_TABLE *summary_info);
STATUS LNPUBLIC print_file_summary_list (ITEM_TABLE *summary, void* szReturn);



//调试 lish
bool WriteLog(CString strFileName,const CString strLog)
{
	char szDataBuf[128]		= _T("");
	char szTimeBuf[128]		= _T("");
	char szTempBuf[1024*10] = _T("");
	_strdate(szDataBuf);
	_strtime(szTimeBuf);
	sprintf(szTempBuf,"%s-%s",szDataBuf,szTimeBuf);

	CString szFileName="";
	szFileName.Format("%s.log",strFileName);

	ofstream stFileStream;
	stFileStream.open(szFileName, ios::app);
	stFileStream<<szTempBuf<<"\t"<<strLog<<endl;
	stFileStream.close();
	return true;
}


extern "C" _declspec (dllexport) BOOL SendMail(CStringList& lstParas, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strServerName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CString strMailFile=_T("");
	
	strServerName = GetValueFromList("_ServerName",lstParas);
	strUserAccount = GetValueFromList("_UserAccount",lstParas);	
	strPassWord = GetValueFromList("_PassWord",lstParas);
	strMailFile = GetValueFromList("_MailFile",lstParas);	
		
	WritePassword(strPassWord);
	
	//------------------开始发送------------------------------------------------------------------
	HANDLE hRetInfo;
	//char hRetInfo[MAX_CONSOLE_CMD_LEN];
	STATUS nError;
	char *pBuffer;
	STATUS      error = NOERROR;            /* return status from API calls */

	error = NotesInitExtended (0, NULL);
    if (error)
    {
        return FALSE;
    }

	WritePassword(strPassWord);

	char *szSendTo = strUserAccount.GetBuffer(strUserAccount.GetLength());
	char *szCopyTo = "";
	char *szFrom = "";
	char *szSubject = "mail";
	char *szBody = "mail";

	STATUS  status = SendMailMainProc(szSendTo,szCopyTo,szFrom,szSubject,szBody);

	
	NotesTerm();

	if(status != 0)
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_SendFailed"));//发送失败");
		return 0;
	}

	//------------------开始接收----------------------------------------------------------------
	char *szReadServerName = strServerName.GetBuffer(strServerName.GetLength());
	char *szReadMailFile = strMailFile.GetBuffer(strMailFile.GetLength());;
	
	try
	{
		status = readmail(szReadServerName,szReadMailFile);
	}
	catch(...)
	{
		NotesTerm();
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_ReceivedFailed"));//接收失败");
		return 0;
	}

	NotesTerm();

	if(status != 0)
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_ReceivedFailed"));//接收失败");
		return 0;
	}

	sprintf(szRet,"reslut=Succeed");

	return TRUE;
}

// 
extern "C" _declspec (dllexport) BOOL Mem(const char * strParas, char * szRet, int& nSize)
//(CStringList& lstParas, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CString strServerName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");

	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);
	
	strServerName = GetValueFromList("_ServerName",lstParas);
	strUserAccount = GetValueFromList("_UserAccount",lstParas);	
	strPassWord = GetValueFromList("_PassWord",lstParas);	

	

	char szResult[1024*10]={0};
	char szServer[1024]={0};
	strcpy(szServer,strServerName);
	//得到内存


	HANDLE hhandle =CreateMutex(NULL,FALSE,"Global\\SiteView-Lotus-Montior");

	if(hhandle ==NULL)
	{
		DWORD nlast;
		nlast =GetLastError();
		printf("begin wait  last error is %d\n",nlast);
		sprintf(szRet,"error=CreateMutex Failed code is %d ",nlast);
		return FALSE;

	}
	

	WaitForSingleObject(hhandle,INFINITE);
	
	puts("run ... ");

	WritePassword(strPassWord);
	STATUS nStatus=NF_Command(szServer,"show stat mem",szResult);
	
	
	puts("ReleaseMutex ");
	ReleaseMutex(hhandle);
	
	CloseHandle(hhandle);
	

	if(nStatus != NOERROR)
	{
		sprintf(szRet,"error=%s %d ", FuncGetStringFromIDS("IDS_RightPoor") , nStatus);//权限不足 %d " , nStatus);

		return FALSE;
	}

	//display 
	//printf("%s",szResult);


	//Mem.Free
	char value[64]={0};
	GetValue(szResult,"Mem.Free",value,3,"\n",1);
	CutFlag(value,',');
	double dValue  = atof(value);

	dValue = dValue/1024;
	dValue= dValue/1024;

	sprintf(szRet, "%sFree=%.2f$", szRet,dValue);


	//Mem.Allocated
//	char value[64]={0};
	memset(value,0,64);
	GetValue(szResult,"Mem.Allocated",value,3,"\n",0);
	CutFlag(value,',');
//	double 
	dValue  = atof(value);

	dValue = dValue/1024;
	dValue= dValue/1024;

	sprintf(szRet, "%sAllocTotal=%.2f$", szRet,dValue);


	//Mem.Allocated.Process
	memset(value,0,64);
	GetValue(szResult,"Mem.Allocated.Process",value,3,"\n",0);
	CutFlag(value,',');
	dValue  = atof(value);

	dValue = dValue/1024;
	dValue= dValue/1024;

	sprintf(szRet, "%sAllocProcess=%.2f$", szRet,dValue);

	//Mem.Allocated.Shared
	memset(value,0,64);
	GetValue(szResult,"Mem.Allocated.Shared",value,3,"\n",0);
	CutFlag(value,',');
	dValue  = atof(value);

	dValue = dValue/1024;
	dValue= dValue/1024;

	sprintf(szRet, "%sAllocShared=%.2f$", szRet,dValue);

	//Mem.PhysicalRAM
	memset(value,0,64);
	GetValue(szResult,"Mem.PhysicalRAM",value,3,"\n",0);
	CutFlag(value,',');
	dValue  = atof(value);

	dValue = dValue/1024;
	dValue= dValue/1024;

	sprintf(szRet, "%sPhysicalRAM=%.2f$", szRet,dValue);

	//Mem.Availability
	memset(value,0,64);
	GetValue(szResult,"Mem.Availability",value,3,"\n",0);
	sprintf(szRet, "%sAvailability=%s$", szRet,value);
	
	CString strInput;
	strInput =szRet;
	MakeCharByString(szRet,nSize,strInput);

	return TRUE;

}

extern "C" _declspec (dllexport) BOOL Disks(const char * strParas, char * szRet, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CString strServerName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");

	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);

	strServerName = GetValueFromList("_ServerName",lstParas);
	strUserAccount = GetValueFromList("_UserAccount",lstParas);	
	strPassWord = GetValueFromList("_PassWord",lstParas);	
/*
	strServerName = "test/cert";
	strUserAccount = "admin";
	strPassWord = "admin";
	WritePassword(strPassWord);
*/

	char szResult[1024*10]={0};
	char szServer[1024]={0};
	strcpy(szServer,strServerName);


	HANDLE hhandle =CreateMutex(NULL,FALSE,"Global\\SiteView-Lotus-Montior");
	if(hhandle ==NULL)
	{
		DWORD nlast;
		nlast =GetLastError();
		printf("begin wait  last error is %d\n",nlast);
		sprintf(szRet,"error=CreateMutex Failed code is %d ",nlast);
		return FALSE;

	}

	WaitForSingleObject(hhandle,INFINITE);
	puts("run ... ");
	WritePassword(strPassWord);
	//得到磁盘 
	STATUS nStatus = NF_Command(szServer,"show stat Disk",szResult);
	puts("ReleaseMutex ");
	ReleaseMutex(hhandle);
	CloseHandle(hhandle);


	if(nStatus != NOERROR)
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_RightPoor"));//权限不足");

		return FALSE;
	}
	//display
	//printf("%s",szResult);

	//Free
	char value[64]={0};
	GetValue(szResult,"Free",value,3,"\n",1);
	CutFlag(value,',');
	double dValue  = atof(value);
	dValue = dValue/1024;
	dValue= dValue/1024;
	sprintf(szRet, "%sFreeDisks=%.2f$", szRet,dValue);

	//Size
	memset(value,0,64);
	GetValue(szResult,"Size",value,3,"\n",1);
	CutFlag(value,',');
	dValue  = atof(value);
	dValue = dValue/1024;
	dValue= dValue/1024;
	sprintf(szRet, "%sSizeDisks=%.2f$", szRet,dValue);

	//Type
	memset(value,0,64);
	GetValue(szResult,"Type",value,3,"\n",1);
	sprintf(szRet, "%sTypeDisks=%s$", szRet,value);

	//Fixed
	memset(value,0,64);
	GetValue(szResult,"Fixed",value,3,"\n",1);
	sprintf(szRet, "%sFixedDisks=%s$", szRet,value);

	//Remote
	memset(value,0,64);
	GetValue(szResult,"Fixed",value,3,"\n",1);
	sprintf(szRet, "%sRemoteDisks=%s$", szRet,value);

	CString strInput;
	strInput =szRet;
	MakeCharByString(szRet,nSize,strInput);

	return TRUE;

}

extern "C" _declspec (dllexport) BOOL Task(const char * strParas, char * szRet, int& nSize)
//(CStringList& lstParas, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CString strServerName=_T("");
	CString strUserAccount=_T("");	CString strPassWord=_T("");

	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);	
	strServerName = GetValueFromList("_ServerName",lstParas);
	strUserAccount = GetValueFromList("_UserAccount",lstParas);	
	strPassWord = GetValueFromList("_PassWord",lstParas);	
/*  
	strServerName = "test/cert";
	strUserAccount = "admin";
	strPassWord = "admin";
	WritePassword(strPassWord);
*/
	

	char szResult[1024*10]={0};
	char szServer[1024]={0};
	strcpy(szServer,strServerName);

	//得到任务
	
	HANDLE hhandle =CreateMutex(NULL,FALSE,"Global\\SiteView-Lotus-Montior");

	if(hhandle ==NULL)
	{
		DWORD nlast;
		nlast =GetLastError();
		printf("begin wait  last error is %d\n",nlast);
		sprintf(szRet,"error=CreateMutex Failed code is %d ",nlast);
		return FALSE;

	}

	WaitForSingleObject(hhandle,INFINITE);
	
	puts("run ... ");
	WritePassword(strPassWord);
	STATUS nStatus=NF_Command(szServer,"show stat  server",szResult);

	if(nStatus != NOERROR)
	{
		sprintf(szRet,"error=%s %d", FuncGetStringFromIDS("IDS_RightPoor"),nStatus);//权限不足 %d",nStatus  );
		puts("ReleaseMutex ");
		ReleaseMutex(hhandle);
	
		CloseHandle(hhandle);

		return FALSE;
	}

	//display 
	//printf("%s",szResult);

	//Task count
	char value[64]={0};

	GetValue(szResult,"Tasks",value,3,"\n",0);
	double dValue  = atof(value);
	sprintf(szRet, "%sTaskCount=%.0f$", szRet,dValue);


	memset(szResult,1024*10,0);

	nStatus=NF_Command(szServer,"show tasks",szResult);

	if(nStatus != NOERROR)
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_RightPoor"));//权限不足");
		puts("ReleaseMutex ");
		ReleaseMutex(hhandle);
		
		CloseHandle(hhandle);

		return FALSE;
	}

	puts("ReleaseMutex ");
	ReleaseMutex(hhandle);
	
	CloseHandle(hhandle);



	memset(value,0,64);
	GetValue(szResult,"Router",value,3,"\n",0);
	sprintf(szRet, "%sRouterStatus=%s$", szRet,value);

	memset(value,0,64);
	GetValue(szResult,"Replicator",value,3,"\n",0);
	sprintf(szRet, "%sRepicatorStatus=%s$", szRet,value);

	memset(value,0,64);
	GetValue(szResult,"Event Monitor",value,3,"\n",0);
	sprintf(szRet, "%sEventStatus=%s$", szRet,value);


	/*
	//TransPerMinute
	memset(value,0,64);
	GetValue(szResult,"Server.Trans.PerMinute",value,3,"\n",0);
	sprintf(szRet, "%sTransPerMinute=%s$", szRet,value);

	//UsersPeak
	memset(value,0,64);
	GetValue(szResult,"Server.Users.Peak",value,3,"\n",0);
	sprintf(szRet, "%sUsersPeak=%s$", szRet,value);
*/

	CString strInput;
	strInput =szRet;
	MakeCharByString(szRet,nSize,strInput);


	return TRUE;

}

extern "C" _declspec (dllexport) BOOL Mail(const char * strParas, char * szRet, int& nSize)
//(CStringList& lstParas, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CString strServerName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");

	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);
	strServerName = GetValueFromList("_ServerName",lstParas);
	strUserAccount = GetValueFromList("_UserAccount",lstParas);	
	strPassWord = GetValueFromList("_PassWord",lstParas);	
/*
	strServerName = "test/cert";
	strUserAccount = "admin";
	strPassWord = "admin";
	WritePassword(strPassWord);
*/



	char szResult[1024*10]={0};
	char szServer[1024]={0};
	strcpy(szServer,strServerName);

	//得到MAIL  
	HANDLE hhandle =CreateMutex(NULL,FALSE,"Global\\SiteView-Lotus-Montior");
	if(hhandle ==NULL)
	{
		DWORD nlast;
		nlast =GetLastError();
		printf("begin wait  last error is %d\n",nlast);
		sprintf(szRet,"error=CreateMutex Failed code is %d ",nlast);
		return FALSE;

	}

	WaitForSingleObject(hhandle,INFINITE);
	WritePassword(strPassWord);
	STATUS nStatus =NF_Command(szServer,"show stat  mail",szResult);
	ReleaseMutex(hhandle);
	CloseHandle(hhandle);

	

	if(nStatus != NOERROR)
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_RightPoor"));//权限不足");

		return FALSE;
	}
	//display 
	//printf("%s",szResult);

	//CurrentByteDeliveryRate
	char value[64]={0};

	GetValue(szResult,"CurrentByteDeliveryRate",value,3,"\n",0);
	double dValue  = atof(value);
	sprintf(szRet, "%sDeliveredMailRate=%.0f$", szRet,dValue);

	//CurrentByteTransferRate
	memset(value,0,64);
	GetValue(szResult,"CurrentByteTransferRate",value,3,"\n",0);
	dValue  = atof(value);
	sprintf(szRet, "%sTaskMailTransferRate=%.0f$", szRet,dValue);

	//CurrentMessageDeliveryRate
	memset(value,0,64);
	GetValue(szResult,"CurrentMessageDeliveryRate",value,3,"\n",0);
	 dValue  = atof(value);
	sprintf(szRet, "%sRoutedMailRate=%.0f$", szRet,dValue);

	//Dead
	memset(value,0,64);
	GetValue(szResult,"Dead",value,3,"\n",0);
	dValue  = atof(value);
	sprintf(szRet, "%sDeadmailRate=%.0f$", szRet,dValue);

	//TotalPending
	memset(value,0,64);
	GetValue(szResult,"TotalPending",value,3,"\n",0);
	dValue  = atof(value);
	sprintf(szRet, "%sTotalPending=%.0f$", szRet,dValue);

	//Waiting
	memset(value,0,64);
	GetValue(szResult,"Waiting",value,3,"\n",0);
	dValue  = atof(value);
	sprintf(szRet, "%sWaitingMail=%.0f$", szRet,dValue);

	//WaitingRecipients
	memset(value,0,64);
	GetValue(szResult,"WaitingRecipients",value,3,"\n",0);
	dValue  = atof(value);
	sprintf(szRet, "%sWaitingRecipients=%.0f$", szRet,dValue);
	CString strInput;
	strInput =szRet;
	MakeCharByString(szRet,nSize,strInput);


	return TRUE;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetFile(char *file)
{
	//所查询注册表键值的内容
	struct HKEY__* ReRootKey = HKEY_CURRENT_USER;
	TCHAR *ReSubKey = "Software\\Lotus\\Notes\\Installer";
	TCHAR *ReValueName = "DATADIR";
	char content[256] = {0};			
	HKEY hKey;

	//定义读取数据类型
	DWORD dwType = REG_SZ;		
	DWORD dwLength = 256;


	int Result=0;

	if(RegOpenKeyEx(ReRootKey,ReSubKey,0,KEY_READ,&hKey)==ERROR_SUCCESS)
	{
		if(RegQueryValueEx(hKey,ReValueName,NULL,&dwType,(unsigned char *)content,&dwLength)!=ERROR_SUCCESS)
		{
			//printf("错误：无法查询有关的注册表信息");
			;
		}
		RegCloseKey(hKey);
	}
	else
	{
		//printf("错误：无法打开有关的hKEY");
		;
	}

	if(strlen(content) <=0)
		strcpy(file,"C:\\lotus\\notes\\data\\password.txt");
	else
	{
		strcat(file,content);
		strcat(file,"password.txt");
	}


	return 0;
}

void WritePassword(const char * szPwd)
{
	char file[256] = {0};
	GetFile(file);

	FILE * fp = NULL;

	fp = fopen(file, "w+");
	if (fp) 
	{
		fputs(szPwd, fp);
		//fputs("\r\n", fp);
		fclose(fp);
	}
}

BOOL CutFlag(char *src,char flag)
{
	char value[64] = {0};

	char *tmp = src;
	int len = strlen(tmp);

	int n = 0;
	for(int i=0; i<len; i++)
	{
		if(*tmp != flag)
		{
			value[n] = *tmp;
			n++;
		}

		tmp++;
	}

	memset(src,0,64);
	strcpy(src,value);

	return TRUE;

}

BOOL GetValue(char *Context,char *Name,char *Value,int nSpace, char *flag,int n)
{
	char * tmp = NULL;
	tmp = strstr(Context,Name);
	tmp+= strlen(Name);
	tmp+= nSpace;

	char * pos = strstr(tmp,flag);
	int len = pos - tmp - n;
	strncpy(Value,tmp,len);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
STATUS NF_Command(char *server ,char* szcommand,char * szResult)
{
	HANDLE hRetInfo;
	//char hRetInfo[MAX_CONSOLE_CMD_LEN];
	STATUS nError;
	char *pBuffer;
	STATUS      error = NOERROR;            /* return status from API calls */

	error = NotesInitExtended (0, NULL);
    if (error)
    {
        return FALSE;
    }

	
	nError = NSFRemoteConsole (server, szcommand, &hRetInfo);
	//nError = NSFRemoteConsole ("192.168.5.11","show task", &hRetInfo);
	if (nError != NOERROR)
	{
		
		
		NotesTerm();
	    return (ERR(nError));
	}

	/* Lock the returned buffer handle to obtain a pointer to the
	   text buffer containing the server command information */

	pBuffer = OSLock (char, hRetInfo);

	//printf(pBuffer);
	strcpy(szResult,pBuffer);

	OSUnlock (hRetInfo);

	OSMemFree (hRetInfo);
	NotesTerm();

	return (NOERROR);
}

STATUS NF_Statics(char *server ,char *Facility,char *StatName,char * szResult)
{
	HANDLE hRetInfo;
	char *pBuffer;
	STATUS nError;
	STATUS      error = NOERROR;            /* return status from API Dcalls */
	DWORD dwWord;

	error = NotesInitExtended (0, NULL);
    if (error)
    {
        return FALSE;
    }
	nError = NSFGetServerStats (server, Facility,StatName, &hRetInfo,&dwWord);

	if (nError != NOERROR)
	{
		NotesTerm();
	    return (ERR(nError));
	}

	/* Lock the returned buffer handle to obtain a pointer to the
	   text buffer containing the server command information */

	pBuffer = OSLock (char, hRetInfo);

	printf(pBuffer);
	strcpy(szResult,pBuffer);

	OSUnlock (hRetInfo);

	OSMemFree (hRetInfo);
	NotesTerm();

	return (NOERROR);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern "C" _declspec (dllexport)  
BOOL FileList(const char * strParas, char * szReturn, int& nSize)
//(CStringList &paramList, CStringList &lstTexts, CStringList &lstValues)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString strDirectory = _T("");  
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	 CStringList lstTexts;
	 CStringList lstValues;

	CString strServerName = GetValueFromList("_ServerName",paramList);
	CString strUserAccount = GetValueFromList("_UserAccount",paramList);	
	CString strPassWord = GetValueFromList("_PassWord",paramList);	

	CString strFile = _T("FileList");

	CString strLog  = _T("");

	WriteLog( strFile, "---------------- Call FileList ------------------");

	//strServerName = "192.168.5.11";
	//WritePassword(strPassWord);
	//strDirectory = "G:\\lotus\\domino\\data\\ssyoa";

	HANDLE hhandle =CreateMutex(NULL,FALSE,"Global\\SiteView-Lotus-Montior");
	if(hhandle ==NULL)
	{
		DWORD nlast;
		nlast =GetLastError();
		printf("begin wait  last error is %d\n",nlast);
		sprintf(szReturn,"error=CreateMutex Failed code is %d ",nlast);
		WriteLog(strFile,szReturn);
		return FALSE;

	}

	WaitForSingleObject(hhandle,INFINITE);
	WritePassword(strPassWord);
	//得到FileList 
	BOOL bResult = NSF_List(strServerName, strDirectory, lstTexts, lstValues);
	ReleaseMutex(hhandle);
	CloseHandle(hhandle);



	POSITION pos=NULL;
    pos = lstTexts.GetHeadPosition();
	char * ca;
	ca =szReturn;
	CString strTemp;
    while(pos)
    {
		strTemp=lstTexts.GetNext(pos);
		if(strTemp.Find(".nsf",0)>=0)
		{
			sprintf(ca,"%s=%s",strTemp.GetBuffer(strTemp.GetLength()),strTemp.GetBuffer(strTemp.GetLength()));

			WriteLog(strFile, ca);
			ca=ca+2*strTemp.GetLength()+2;
		}
        //lstValues.AddTail(lstTexts.GetNext(pos));        
    }

	
	WriteLog(strFile, "------------------ FileList Success ------------------");
	


	return bResult ;
	
}

BOOL DBSPace(CString strDBFile, CString strServer,CString strPassWord ,char *szReturn)
{

	

	CString strDirectory = _T("");  
    char        szFullNetPath[MAXPATH] = ""; /* full network path */
    DBHANDLE    dir_handle;                 /* handle for directory */
    STATUS      error = NOERROR;            /* return status from API calls */
    DWORD       dwAvalid = 0;
    DWORD       dwFree = 0;

	WritePassword(strPassWord);

    error = NotesInitExtended (0, NULL);
    if (error)
    {
        sprintf (szReturn, "error=Error initializing Notes.$");
        return FALSE;
    }
    if (_T("") != strDBFile && _T("")!=strDirectory)
    {
        strDirectory += "\\";
        strDirectory += strDBFile;
    }
    else if(_T("") != strDBFile && _T("")==strDirectory)
    {
        strDirectory = strDBFile;
    }
    if(strServer != _T(""))
    {
        /* Compose the full network pathname to the directory. */
        if (error = OSPathNetConstruct(NULL, strServer.GetBuffer(0),
                            strDirectory.GetBuffer(0), szFullNetPath))
	     {
		      PrintAPIError (error, szReturn);
		      NotesTerm();
		      return FALSE;
	     }
    }
    //WriteLog(szFullNetPath);

    /* Open the directory. */
    if (error = NSFDbOpen (szFullNetPath, &dir_handle))
    {
        PrintAPIError (error, szReturn);
        NotesTerm();
        return FALSE;
    }

    if (error = NSFDbSpaceUsage(dir_handle, &dwAvalid, &dwFree))
    {
        PrintAPIError (error, szReturn);
        NotesTerm();
        return FALSE;
    }

//    /* Call NSFSearch to find files in the directory. For each file found,
//    call an action routine. */
//    if (error = NSFSearch (
//        dir_handle,        /* directory handle */
//        NULLHANDLE,        /* selection formula */
//        NULL,              /* title of view in formula */
//        SEARCH_FILETYPE +  /* search for files */
//        SEARCH_SUMMARY,    /* return a summary buffer */
//        FILE_DBANY +       /* find any .NS? file */
//        FILE_DIRS +        /* find subdirectories */
//        FILE_NOUPDIRS,     /* don't find the ".." dir */
//        NULL,              /* starting date */
//        file_action,       /* call for each file found */
//        (void*)szReturn,   /* argument to action routine */
//        NULL))             /* returned ending date (unused) */
//        {
//            PrintAPIError (error, szReturn);
//            NSFDbClose (dir_handle);
//            NotesTerm();
//            return FALSE;
//        }

    /* Close the directory. */
    if (error = NSFDbClose (dir_handle))
    {
        PrintAPIError (error, szReturn);
        NotesTerm();
        return FALSE;
    }

    /* Terminate Domino and Notes. */
    NotesTerm();

	sprintf(szReturn, "Used=%f$Free=%f$FreePer=%f$", 
			dwAvalid/(1024.0*1024), dwFree/(1024.0*1024), 100.0*dwFree/(dwFree+dwAvalid));
	return TRUE;

}


extern "C" __declspec(dllexport)
BOOL    Domino(const char * strParas, char * szReturn, int& nSize)
//(CStringList& paramList, char* szReturn)
{
    
    
	
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	CString strServer = GetValueFromList("_ServerName",paramList);
	CString strUserAccount = GetValueFromList("_UserAccount",paramList);	
	CString strPassWord = GetValueFromList("_PassWord",paramList);	
	CString strDBFile = GetValueFromList("_DBFile",paramList);



	HANDLE hhandle =CreateMutex(NULL,FALSE,"Global\\SiteView-Lotus-Montior");
	if(hhandle ==NULL)
	{
		DWORD nlast;
		nlast =GetLastError();
		printf("begin wait  last error is %d\n",nlast);
		sprintf(szReturn,"error=CreateMutex Failed code is %d ",nlast);
		return FALSE;

	}

	WaitForSingleObject(hhandle,INFINITE);
	
	puts("run ... ");

	 DBSPace(strDBFile,strServer,strPassWord,szReturn);


	puts("ReleaseMutex ");
	ReleaseMutex(hhandle);
	
	CloseHandle(hhandle);
	


	
	

	
	CString strInput;
	strInput =szReturn;
	MakeCharByString(szReturn,nSize,strInput);

    return  TRUE;
}


/************************************************************************

    FUNCTION:   file_action

    PURPOSE:    routine called by NSFSearch for each file found

    INPUTS:
      The first argument to this function is the optional argument
      that we supplied when we called NSFSearch.

      The second argument is supplied by NSFSearch.    It is
      a structure of information about the object that was found.

      The third argument is also supplied by NSFSearch and is
      the summary buffer for this object.

*************************************************************************/

STATUS LNPUBLIC file_action
            (void *szReturn,
            SEARCH_MATCH far *pSearchMatch,
            ITEM_TABLE *summary_info)
{
    STATUS        error;
    SEARCH_MATCH SearchMatch;

    memcpy( (char*)&SearchMatch, (char*)pSearchMatch, sizeof(SEARCH_MATCH) );

/* Skip this object if it does not really match the search criteria (it
is now deleted or modified).  This is not necessary for full searches,
but is shown here in case a starting date was used in the search. */

    if (!(SearchMatch.SERetFlags & SE_FMATCH))
        return (NOERROR);

/* Call a local function to print the file information in the
summary buffer. */

    if (error = print_file_summary (summary_info, (char*)szReturn))
		 return(error);

/* End of subroutine. */

    return (NOERROR);

}

STATUS LNPUBLIC file_action_list
            (void *szReturn,
            SEARCH_MATCH far *pSearchMatch,
            ITEM_TABLE *summary_info)
{
    STATUS        error;
    SEARCH_MATCH SearchMatch;

    memcpy( (char*)&SearchMatch, (char*)pSearchMatch, sizeof(SEARCH_MATCH) );

/* Skip this object if it does not really match the search criteria (it
is now deleted or modified).  This is not necessary for full searches,
but is shown here in case a starting date was used in the search. */

    if (!(SearchMatch.SERetFlags & SE_FMATCH))
        return (NOERROR);

/* Call a local function to print the file information in the
summary buffer. */

    if (error = print_file_summary_list (summary_info, szReturn))
		 return(error);

/* End of subroutine. */

    return (NOERROR);

}

/************************************************************************

    FUNCTION:   print_file_summary

    PURPOSE:    This function prints the items in a file summary buffer.

    DESCRIPTION:
         The information in a summary buffer is structured as follows:

            header (total length of summary + number of items in summary)
            length of name of item #1
            length of item #1 (including data type)
            length of name of item #2
            length of item #2 (including data type)
            length of name of item #3
            length of item #3 (including data type)
            ...
            name of item #1
            data type of item #1 + value of item #1
            name of item #2
            data type of item #2 + value of item #2
            name of item #3
            data type of item #3 + value of item #3
            ....

*************************************************************************/

STATUS LNPUBLIC print_file_summary (ITEM_TABLE *summary, char* szReturn)
{

#define     MAX_ITEMS           30
#define     MAX_TEXT_LEN        1000
#define     DATATYPE_SIZE       sizeof(USHORT)
#define     NAME_LENGTH_SIZE    sizeof(USHORT)
#define     ITEM_LENGTH_SIZE    sizeof(USHORT)
#define     NUMERIC_SIZE        sizeof(FLOAT)
#define     TIME_SIZE           ODSLength(_TIMEDATE)


/* Local variables */

    BYTE       *summary_position;       /* current position in summary */
    ITEM_TABLE item_table;              /* header at start of summary */
    USHORT     item_count;              /* number of items in summary */
    USHORT     name_length[MAX_ITEMS];  /* length of each item name */
    USHORT     item_length[MAX_ITEMS];  /* length of each item */
    char       item_name[MAX_TEXT_LEN]; /* name of a summary item */
    USHORT     datatype;                /* type of item */
    char       item_text[MAX_TEXT_LEN]; /* text of a summary item */
    FLOAT      numeric_item;            /* a numeric item */
    TIMEDATE   time_item;               /* a time/date item */
    WORD       time_string_len;         /* length of ASCII time/date */
    STATUS     error;                   /* return code from API calls */
    USHORT     i;                       /* a counter */


/* Print a blank line to start this display. */

    printf ("\n");

/* Get the header at the beginning of the summary buffer. */

    item_table = *summary;

/* Keep track of where we are in the buffer. */

    summary_position = (BYTE *) summary;

/* Get the number of items in the summary. */

    item_count = item_table.Items;

/* Check boundary of the array */

	 if (item_count > MAX_ITEMS)
	 {
		 printf("ERROR: Number of items has exceeded boundary of defined array.\n");
		 return (0);
	 }

/* Advance the buffer pointer over the header. */

    summary_position += ODSLength(_ITEM_TABLE);

/* Get the length of each item in the summary. */

    for (i=0; i < item_count; i++)
        {
        memcpy (&name_length[i], summary_position, NAME_LENGTH_SIZE);
        summary_position += NAME_LENGTH_SIZE;
        memcpy (&item_length[i], summary_position, ITEM_LENGTH_SIZE);
        summary_position += ITEM_LENGTH_SIZE;
        }

/* Start a loop that extracts each item in the summary. */

    for (i=0; i < item_count; i++)
        {

/* Print the name of the item. */

        memcpy (item_name, summary_position, name_length[i]);
        item_name[name_length[i]] = '\0';
        summary_position += name_length[i];

        printf ("%s:  ", item_name);

/* Get the data type of this item. */

        memcpy (&datatype, summary_position, DATATYPE_SIZE);
        summary_position += DATATYPE_SIZE;

/* Extract the item from the summary and put it in readable
form. The way in which we extract the item depends on its type.
This program handles TEXT, NUMBER, and TIME. */

        switch (datatype)
        {

/* Extract a text item from the summary. */

        case TYPE_TEXT:
            memcpy (item_text,
                summary_position,
                item_length[i] - DATATYPE_SIZE);
            item_text[item_length[i] - DATATYPE_SIZE] = '\0';
            break;

/* Extract a number item from the summary. */

        case TYPE_NUMBER:
            memcpy (&numeric_item, summary_position, NUMERIC_SIZE);
            sprintf (item_text, "%g", numeric_item);
            break;

/* Extract a time/date item from the summary. */

        case TYPE_TIME:

            memcpy (&time_item, summary_position, TIME_SIZE);

            if (error = ConvertTIMEDATEToText (NULL,
                                               NULL,
                                               &time_item,
                                               item_text,
                                               MAXALPHATIMEDATE,
                                               &time_string_len))
            return (ERR(error));

            item_text[time_string_len] = '\0';

            break;

     case TYPE_TEXT_LIST:
        {
          LIST      *pList;
          WORD      list_entry;
          char      *Buffer;
          WORD      text_size;

        memset (item_text,'\0', item_length[i] - DATATYPE_SIZE + 1);
        pList = (LIST *)summary_position;
        for (list_entry = 0; list_entry < pList->ListEntries; list_entry++)
           {
           ListGetText(pList, FALSE, list_entry, &Buffer, &text_size);
           strncat (item_text, Buffer, text_size);
           }
        }
        break;

/* If the summary item is not one of the data types this program
handles. */

        default:
            strcpy (item_text, "(Data type not handled)");
            break;
        }

/* Print the item. */

        printf ("%s\n", item_text);

/* Advance to next item in the summary. */

        summary_position += (item_length[i] - DATATYPE_SIZE);

/* End of loop that is extracting each item in the summary. */

        }

/* End of function */

    return (NOERROR);

}

STATUS LNPUBLIC print_file_summary_list (ITEM_TABLE *summary, void* szReturn)
{
#define     MAX_ITEMS           30
#define     MAX_TEXT_LEN        1000
#define     DATATYPE_SIZE       sizeof(USHORT)
#define     NAME_LENGTH_SIZE    sizeof(USHORT)
#define     ITEM_LENGTH_SIZE    sizeof(USHORT)
#define     NUMERIC_SIZE        sizeof(FLOAT)
#define     TIME_SIZE           ODSLength(_TIMEDATE)


/* Local variables */

    BYTE       *summary_position;       /* current position in summary */
    ITEM_TABLE item_table;              /* header at start of summary */
    USHORT     item_count;              /* number of items in summary */
    USHORT     name_length[MAX_ITEMS];  /* length of each item name */
    USHORT     item_length[MAX_ITEMS];  /* length of each item */
    char       item_name[MAX_TEXT_LEN]; /* name of a summary item */
    USHORT     datatype;                /* type of item */
    char       item_text[MAX_TEXT_LEN]; /* text of a summary item */
    FLOAT      numeric_item;            /* a numeric item */
    TIMEDATE   time_item;               /* a time/date item */
    WORD       time_string_len;         /* length of ASCII time/date */
    STATUS     error;                   /* return code from API calls */
    USHORT     i;                       /* a counter */

    CStringList* lstText;
    lstText = (CStringList*)szReturn;


/* Print a blank line to start this display. */

    printf ("\n");

/* Get the header at the beginning of the summary buffer. */

    item_table = *summary;

/* Keep track of where we are in the buffer. */

    summary_position = (BYTE *) summary;

/* Get the number of items in the summary. */

    item_count = item_table.Items;

/* Check boundary of the array */

	 if (item_count > MAX_ITEMS)
	 {
		 printf("ERROR: Number of items has exceeded boundary of defined array.\n");
		 return (0);
	 }

/* Advance the buffer pointer over the header. */

    summary_position += ODSLength(_ITEM_TABLE);

/* Get the length of each item in the summary. */

    for (i=0; i < item_count; i++)
    {
        memcpy (&name_length[i], summary_position, NAME_LENGTH_SIZE);
        summary_position += NAME_LENGTH_SIZE;
        memcpy (&item_length[i], summary_position, ITEM_LENGTH_SIZE);
        summary_position += ITEM_LENGTH_SIZE;
    }

/* Start a loop that extracts each item in the summary. */
    bool bNeedAdd = false;
    for (i=0; i < item_count; i++)
        {

/* Print the name of the item. */

        memcpy (item_name, summary_position, name_length[i]);
        item_name[name_length[i]] = '\0';
        summary_position += name_length[i];

        printf ("%s:  ", item_name);
        if (0 == strcmp(item_name, "$TITLE"))
        {
            bNeedAdd = true;
        }

/* Get the data type of this item. */

        memcpy (&datatype, summary_position, DATATYPE_SIZE);
        summary_position += DATATYPE_SIZE;

/* Extract the item from the summary and put it in readable
form. The way in which we extract the item depends on its type.
This program handles TEXT, NUMBER, and TIME. */

        switch (datatype)
        {

/* Extract a text item from the summary. */

        case TYPE_TEXT:
            memcpy (item_text,
                summary_position,
                item_length[i] - DATATYPE_SIZE);
            item_text[item_length[i] - DATATYPE_SIZE] = '\0';
            if (bNeedAdd)
            {
                lstText->AddTail(item_text);
                bNeedAdd = false;
            }
            break;

/* Extract a number item from the summary. */

        case TYPE_NUMBER:
            memcpy (&numeric_item, summary_position, NUMERIC_SIZE);
            sprintf (item_text, "%g", numeric_item);
            break;

/* Extract a time/date item from the summary. */

        case TYPE_TIME:

            memcpy (&time_item, summary_position, TIME_SIZE);

            if (error = ConvertTIMEDATEToText (NULL,
                                               NULL,
                                               &time_item,
                                               item_text,
                                               MAXALPHATIMEDATE,
                                               &time_string_len))
            return (ERR(error));

            item_text[time_string_len] = '\0';

            break;

     case TYPE_TEXT_LIST:
        {
          LIST      *pList;
          WORD      list_entry;
          char      *Buffer;
          WORD      text_size;

        memset (item_text,'\0', item_length[i] - DATATYPE_SIZE + 1);
        pList = (LIST *)summary_position;
        for (list_entry = 0; list_entry < pList->ListEntries; list_entry++)
           {
           ListGetText(pList, FALSE, list_entry, &Buffer, &text_size);
           strncat (item_text, Buffer, text_size);
           }
        }
        break;

/* If the summary item is not one of the data types this program
handles. */

        default:
            strcpy (item_text, "(Data type not handled)");
            break;
        }

/* Print the item. */

        printf ("%s\n", item_text);

/* Advance to next item in the summary. */

        summary_position += (item_length[i] - DATATYPE_SIZE);

/* End of loop that is extracting each item in the summary. */

        }

/* End of function */

    return (NOERROR);

}

void PrintAPIError (STATUS api_error, char* szReturn)
{
    STATUS  string_id = ERR(api_error);
    char    error_text[200];
    WORD    text_len;

    /* Get the message for this Lotus C API for Domino and Notes error code
       from the resource string table. */

    text_len = OSLoadString (NULLHANDLE,
                             string_id,
                             error_text,
                             sizeof(error_text));

    /* Print it. */

//    fprintf (stderr, "\n%s\n", error_text);
    if (szReturn)
    {
        sprintf(szReturn, "error=%s", error_text);
    }
    else
    {
        fprintf(stderr, "\n%s\n", error_text);
    }
}


BOOL NSF_List(CString strServer, CString strDirectory, 
					   CStringList &lstTexts,
					   CStringList &lstValues)
{
	char        szFullNetPath[MAXPATH] = ""; /* full network path */
    DBHANDLE    dir_handle;                 /* handle for directory */
    STATUS      error = NOERROR;            /* return status from API calls */


	CString strFile = _T("FileList");
	CString strLog  = _T("");

    error = NotesInitExtended (0, NULL);
    if (error)
    {
		WriteLog(strFile, "------------- NotesInitExtended Error ----------------") ;
        return FALSE;
    }
    
    if (strDirectory != _T(""))
    {
        strcpy(szFullNetPath, strDirectory.GetBuffer(0));
    }
    
    if(strServer != _T(""))
    {
        /* Compose the full network pathname to the directory. */
        if (error = OSPathNetConstruct(NULL, strServer.GetBuffer(0),
                            strDirectory.GetBuffer(0), szFullNetPath))
	     {
		      PrintAPIError (error, NULL);
		      NotesTerm();
			  WriteLog(strFile,"--------------------- OSPathNetConstruct Error ---------------- ");
		      return FALSE;
	     }
    }

	strLog.Format("strDirectory:%s",strDirectory);
	WriteLog(strFile,strLog);
	strLog.Format("szFullNetPath:%s",szFullNetPath);
	WriteLog(strFile,strLog);

    /* Open the directory. */
    if (error = NSFDbOpen (szFullNetPath, &dir_handle))
    {
        PrintAPIError (error, NULL);
        NotesTerm();
		WriteLog(strFile,"--------------------- NSFDbOpen Error ---------------- ");
        return FALSE;
    }

    /* Call NSFSearch to find files in the directory. For each file found,
    call an action routine. */
    if (error = NSFSearch (
        dir_handle,        /* directory handle */
        NULLHANDLE,        /* selection formula */
        NULL,              /* title of view in formula */
        SEARCH_FILETYPE +  /* search for files */
        SEARCH_SUMMARY,    /* return a summary buffer */
        //FILE_DBANY +       /* find any .NS? file */
		FILE_FTANY +       /* NT?, any NTF version */
        FILE_DIRS +        /* find subdirectories */
		FILE_RECURSE +
        FILE_NOUPDIRS,     /* don't find the ".." dir */
        NULL,              /* starting date */
        file_action_list,  /* call for each file found */
        &lstTexts,              /* argument to action routine */
        NULL))             /* returned ending date (unused) */

        {
            PrintAPIError (error, NULL);
            NSFDbClose (dir_handle);
            NotesTerm();
			WriteLog(strFile,"------------------- NSFSearch Eorror ------------------");
            return FALSE;
        }

    /* Close the directory. */
    if (error = NSFDbClose (dir_handle))
    {
        PrintAPIError (error, NULL);
        NotesTerm();
		WriteLog(strFile, "-------------------- NSFDbClose Eorror -------------------");
        return FALSE;
    }

    /* Terminate Domino and Notes. */
    NotesTerm();
    POSITION pos=NULL;
    pos = lstTexts.GetHeadPosition();
	int nCount = lstTexts.GetCount();
	strLog.Format("nCount=%d",nCount);
	WriteLog(strFile,strLog);
	CString strVal = _T("");
	int i=0;
    while(pos)
    {
		i++;
	   strVal = lstTexts.GetNext(pos);
	   strLog.Format("strVal%d:%s", i, strVal);
	   WriteLog( strFile, strLog );
        lstValues.AddTail(strVal);        
    }

	WriteLog(strFile,"---------------------- NSF_List Success ----------------------");
	return TRUE;
}

