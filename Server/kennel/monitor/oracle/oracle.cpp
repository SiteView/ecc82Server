// Oracle.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Oracle.h"
#include <iostream>
#include <fstream>
#include <map>
#include <string>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifdef WIN32
#pragma warning (disable : 4267)
#endif
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
// COracleApp

BEGIN_MESSAGE_MAP(COracleApp, CWinApp)
	//{{AFX_MSG_MAP(COracleApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COracleApp construction

COracleApp::COracleApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only COracleApp object

COracleApp theApp;

#define		__ServiceName__					"_DsnName="
//#define		__ServiceName__					"_ServiceName="
#define		__USERACCOUNT__					"_UserAccount="
#define		__PASSWORD__					"_PassWord="
#define		__CONNECTTIMEOUT__				"_ConnTimeout="
#define		__QUERYTIMEOUT__				"_QueryTimeout="
#define		__TABLESPACENAME__				"_TableSpaceName="
#define		__PROCESSNAME__					"_ProcessName="
#define     __SQL__                         "_SQL="
#define     __TEMPLATEID__                  "_TemplateID="
#define     __MONITORID__                  "_MonitorID="

char  szoutFile[256]={0};
BOOL GetResult( CString & strBuf );
void WriteResultFile(const char* chMsg, const char* filename, int nFlag);
void insert();
void select();
BOOL DBrlogon( CString strConn, otl_connect & pdb,char * szReturn);
BOOL DB_TABLESPACENAME(char *dbconn, char *uid, char *pwd,   char * strReturn, int& nSize);					   
BOOL DB_PROCESSNAME(char *dbconn, char *uid, char *pwd,  char* szReturn,	   int nSize);
BOOL ConnectedOracleMachines(char *dbconn, char *uid, char *pwd, char * strReturn, int& nSize);
void PrintDebugString(string szMsg)
{
#ifdef WIN32
    OutputDebugString("Oracle.dll-- ");
    OutputDebugString(szMsg.c_str());
    OutputDebugString(" --\n");
#endif
}

int PrintLog(const char * strMsg, const char * strFileName)
{
#ifndef _DEBUG
	return 0;
#endif
	char datebuf[128],timebuf[128],tempbuf[40960];
	_tzset();
	_strdate(datebuf);
	_strtime(timebuf);
	sprintf(tempbuf,"%s-%s",datebuf,timebuf);

	char strFile[128] = {0};
	sprintf(strFile,"%s.log",strFileName);

	ofstream filestream;
//	filestream.open("oraclemonitor.log",ios::app);
	filestream.open(strFile, ios::app);
	filestream<<tempbuf<<"\t"<<strMsg<<endl;
	filestream.close();
	return 0;

}
//获取传入参项值的函数。
char * GetParaValue(const char * strParas, const char * strObj)
{
	static char strValue[1024] = {0};
	CStringList strList;
	MakeStringListByChar(strList, strParas);
	POSITION pos = strList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = strList.GetNext(pos);
		if(strTemp.Find(strObj) == 0)
		{
			sprintf(strValue,"%s",(strTemp.Right(strTemp.GetLength() - strlen(strObj))).GetBuffer(1));
		}
	}

	return strValue;
}

extern "C" __declspec(dllexport)
BOOL	TNSPing(CStringList& paramList, TCHAR* szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	POSITION pos = paramList.FindIndex(0);
	CString strServiceName = _T("");
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find("_servername=", 0) == 0)
		{
			strServiceName = strTemp.Right(strTemp.GetLength() - strlen("_servername="));
		}
		else if(strTemp.Find("_DsnName=" , 0) == 0)
		{
			strServiceName = strTemp.Right(strTemp.GetLength() - strlen("_DsnName="));
		}
	}

	if( strServiceName.IsEmpty() )
	{
		sprintf( szReturn , "error=%s", FuncGetStringFromIDS("SV_ORACLE",
            "ORACLE_SERVERNAME_NOT_FOUND"));
  		return FALSE;
	}
	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );

	CFileException ex;
	CFile File;
	CString strFile("");

	char chParam[_MAX_PATH] = {0};
	sprintf( chParam, "tnsping %s", strServiceName);
#if _DEBUG
	strFile.Format("%s\\temp\\tnsping_%d.txt", FuncGetInstallPath(), 
        GetCurrentProcessId());
#else
	strFile.Format("%s\\MonitorManager\\temp\\tnsping_%d.txt", FuncGetInstallPath(), 
        GetCurrentProcessId());
#endif
	if( !File.Open(strFile,CFile::modeCreate|CFile::modeReadWrite , &ex) )
	{
		sprintf( szReturn , "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_CREATE_FILE_FAILE"));
		return FALSE;
	}

	si.dwFlags		=	 STARTF_USESTDHANDLES			;
	si.hStdOutput   =	 (HANDLE)  File.m_hFile			;


	// Start the child process. 
	if( !CreateProcess( NULL, // No module name (use command line). 
		chParam,			  // Command line. 
		NULL,				  // Process handle not inheritable. 
		NULL,                 // Thread handle not inheritable. 
		TRUE,			      // Set handle inheritance to FALSE. 
		0,				      // No creation flags. 
		NULL,			      // Use parent's environment block. 
		NULL,			      // Use parent's starting directory. 
		&si,			      // Pointer to STARTUPINFO structure.
		&pi )			      // Pointer to PROCESS_INFORMATION structure.
	) 
	{
		sprintf( szReturn, "error=%s%d$", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_PROCESS_FAILED"), GetLastError());
		return FALSE;
	}

	// Wait until child process exits.
	WaitForSingleObject( pi.hProcess, 1000*60 );
	File.Close();

	// Close process and thread handles. 
	CloseHandle( pi.hProcess );
	CloseHandle( pi.hThread  );

	CString strParam("");
	if ( GetResult(strParam) )
	{
		CString strTime = _T("");
		if (strParam.Find("（" , 0) != -1)
		{
			strTime = strParam.Right( strParam.GetLength()-strParam.Find("（" , 0) );
		}
		else
		{
			strTime = strParam;
		}

		char chTime[_MAX_PATH] = {0};
		if (strTime == strParam)
		{
			sprintf( szReturn , "stats=%s$time=0$", strParam);
		}
		else
		{
			strncpy( chTime, strTime.Right( strTime.GetLength() -2 ), strTime.GetLength()-9 );

			sprintf( szReturn , "stats=%s$time=%d$", strParam.Left( strParam.Find("（",0)), atol(chTime) );
		}		
		
		return TRUE;
	}
	else
	{
		sprintf( szReturn , "error=%s", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_PARSER_STRING_FAILED"));
		return FALSE;
	}
}

BOOL GetResult( CString & strBuf )
{
	char  chBuf[_MAX_PATH] = {0};
	FILE * fp = NULL;
	int iShift = 0;

	CString strTmpFile = _T("");
#if _DEBUG
	strTmpFile.Format("%s\\temp\\tnsping_%d.txt", FuncGetInstallPath(),
        GetCurrentProcessId());
#else
	strTmpFile.Format("%s\\MonitorManager\\temp\\tnsping_%d.txt", FuncGetInstallPath(),
        GetCurrentProcessId());
#endif
	if( (fp=fopen(strTmpFile, "r")) != NULL ) 
	{
		while( !feof(fp) )
		{
			fgets( chBuf, _MAX_PATH, fp );
//			if( iShift++ == 6 )
//			{
//				strBuf.Format( "%s", chBuf );
//				break;
//			}
		}
		strBuf.Format( "%s", chBuf );
		fclose(fp);
	}
	DeleteFile(strTmpFile);

	return !strBuf.IsEmpty();
}

extern "C" __declspec(dllexport)
//BOOL	TestDBConnect(CStringList& paramList, TCHAR* szReturn)
BOOL TestDBConnect(const char * strParas, char * szReturn , int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strUser = _T("");
	CString strPwd = _T("");
	CString strService = _T("");
	CString strLog = _T("");
	
	CStringList paramList;
	MakeStringListByChar(paramList , strParas);

	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTmp = _T("");
		strTmp = paramList.GetNext(pos);
		if (0 == strTmp.Find(__USERACCOUNT__))
		{
			strUser = strTmp.Right(strTmp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if (0 == strTmp.Find(__PASSWORD__))
		{
			strPwd = strTmp.Right(strTmp.GetLength() - strlen(__PASSWORD__));
		}
		else if (0 == strTmp.Find(__ServiceName__))
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen(__ServiceName__));
		}
		else if(strTmp.Find("_ServiceName=") == 0)
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen("_ServiceName="));
		}
	}

	strLog.Format("_ServiceName=%s _UserAccount=%s _PassWord=%s", 
		strService, strUser, strPwd);

	if (strService.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_ORACLE",
            "ORACLE_SERVERNAME_NOT_FOUND"));
		return FALSE;
	}

	otl_connect db;
	CString strConn = _T("");

	otl_conn::initialize();
//	strConn.Format("%s/%s@%s", strUser, strPwd, strService);
	strConn.Format("UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;", strUser, strPwd, strService);

	try
	{
		int nBegin = GetTickCount();
//		db.rlogon(strConn.GetBuffer(strConn.GetLength()),1);
		db.rlogon(strConn.GetBuffer(1));		
		int nEnd = GetTickCount();
		sprintf(szReturn, "status=OK$time(毫秒)=%d", nEnd-nBegin);
	}
	catch(otl_exception& p)
	{ // intercept OTL exceptions
//		cerr<<p.msg<<endl; // print out error message
//		cerr<<p.stm_text<<endl; // print out SQL that caused the error
//		cerr<<p.var_info<<endl; // print out the variable that caused the error
		sprintf(szReturn, "error=%s\r\n%s$", p.msg, p.var_info);
		strLog.Format("TestDBConnect ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
		db.logoff(); // disconnect from ODBC
		return FALSE;
	}
	CString strInput=szReturn;
	MakeCharByString(szReturn,nSize,strInput);
	db.logoff(); // disconnect from ODBC
	
	return TRUE;
}

extern "C" __declspec(dllexport)
BOOL	TestSQLExec(CStringList& paramList, TCHAR* szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strUser = _T("");
	CString strPwd = _T("");
	CString strService = _T("");
	CString strLog = _T("");
	
	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTmp = _T("");
		strTmp = paramList.GetNext(pos);
		if (0 == strTmp.Find(__USERACCOUNT__))
		{
			strUser = strTmp.Right(strTmp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if (0 == strTmp.Find(__PASSWORD__))
		{
			strPwd = strTmp.Right(strTmp.GetLength() - strlen(__PASSWORD__));
		}
		else if (0 == strTmp.Find(__ServiceName__))
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen(__ServiceName__));
		}
		else if(strTmp.Find("_ServiceName=") == 0)
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen("_ServiceName="));
		}
	}

	strLog.Format("_ServiceName=%s _UserAccount=%s _PassWord=%s", 
		strService, strUser, strPwd);

	if (strService.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_ORACLE",
            "ORACLE_SERVERNAME_NOT_FOUND"));
		return FALSE;
	}

	otl_connect db;
	CString strConn = _T("");

	otl_conn::initialize();
//	strConn.Format("%s/%s@%s", strUser, strPwd, strService);
	strConn.Format("UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;", strUser, strPwd, strService);

	try
	{
		int nBegin = GetTickCount();
		db.rlogon(strConn.GetBuffer(1));		
		otl_stream i(50, // buffer size
              "select * from dual",
                 // SELECT statement
              db // connect object
             ); 		
		int nEnd = GetTickCount();
		sprintf(szReturn, "status=OK$time=%d$", nEnd-nBegin);
	}
	catch(otl_exception& p)
	{ // intercept OTL exceptions
//		cerr<<p.msg<<endl; // print out error message
//		cerr<<p.stm_text<<endl; // print out SQL that caused the error
//		cerr<<p.var_info<<endl; // print out the variable that caused the error
		sprintf(szReturn, "error=%s\r\n%s$", p.msg, p.var_info);
		strLog.Format("TestSQLExec ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
		db.logoff(); // disconnect from ODBC
		return FALSE;
	}
	db.logoff(); // disconnect from ODBC
	
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////

extern "C" __declspec(dllexport)
//BOOL	GetOracleInfo(CStringList& paramList, TCHAR* szReturn)
BOOL	GetOracleInfo(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	char * strMonitorId = GetParaValue(strParas,"_MonitorID=");
	char strFileName[128] = {0};
	sprintf(strFileName,"Oracle_GetOracleInfo_%s",strMonitorId);

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	CString strUser = _T("");
	CString strPwd = _T("");
	CString strService = _T("");
	CString strLog = _T("");
	
	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTmp = _T("");
		strTmp = paramList.GetNext(pos);
		if (0 == strTmp.Find(__USERACCOUNT__))
		{
			strUser = strTmp.Right(strTmp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if (0 == strTmp.Find(__PASSWORD__))
		{
			strPwd = strTmp.Right(strTmp.GetLength() - strlen(__PASSWORD__));
		}
		else if (0 == strTmp.Find(__ServiceName__))
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen(__ServiceName__));
		}
		else if(strTmp.Find("_ServiceName=") == 0)
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen("_ServiceName="));
		}
	}

	strLog.Format("_ServiceName=%s _UserAccount=%s _PassWord=%s", 
		strService, strUser, strPwd);

	otl_connect db;
	CString strConn = _T("");

	otl_conn::initialize();
//	strConn.Format("%s/%s@%s", strUser, strPwd, strService);
	//strConn.Format("UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;", strUser, strPwd, strService);
	
	strConn.Format("Provider=OraOLEDB.Oracle;UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;", strUser, strPwd, strService);
	//WriteResultFile(strConn,szoutFile,1);
	try
	{
		CString strSQL = _T("");
		//CString strSQLFilePath = _T("");
		//strSQLFilePath.Format("%s\\groups\\sql.ini", FuncGetInstallPath());
		//CString strOracleRec;
		//strOracleRec.Format("%s\\groups\\oracle.log", FuncGetInstallPath());
		
		int nBegin = GetTickCount();
		//db.rlogon(strConn.GetBuffer(1));	
		if(FALSE==DBrlogon(strConn,db,szReturn)) 
		{
			return FALSE;
		}

//printf("1");
		//游标数
		strSQL.Empty();
		//GetPrivateProfileString("cursors", "cursors", "", strSQL.GetBuffer(4096), 4096, strSQLFilePath);
		//strSQL.Format("%s", strSQL);
		strSQL="select count(*) from v$open_cursor";
		otl_stream i(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		int nTotalCursor = 0;
		i>>nTotalCursor;
		i.close();
//printf("2");		
		//取当前实例的session数
		strSQL.Empty();
		//GetPrivateProfileString("sessions", "sessions", "", strSQL.GetBuffer(4096), 4096, strSQLFilePath);
		//strSQL.Format("%s", strSQL);
		strSQL="select count(SID) from v$session";
		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		int nTotalSession = 0;
		i>>nTotalSession;
		i.close();
//printf("3");
		//每秒事务数
		int nStartCommitNum = 0,
			nEndCommitNum = 0;
		int nStartRollBackNum = 0,
			nEndRollBackNum = 0;

		strSQL.Empty();
		//GetPrivateProfileString("usercommits", "usercommits", "", strSQL.GetBuffer(4096), 4096, strSQLFilePath);
		//strSQL.Format("%s", strSQL);
		strSQL="select value,name from v$sysstat  where name='user commits'";

		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		i>>nStartCommitNum;
		i.close();
		
		strSQL.Empty();
		//GetPrivateProfileString("userrollbacks", "userrollbacks", "", strSQL.GetBuffer(4096), 4096, strSQLFilePath);
		strSQL="select value,name from v$sysstat  where name='user rollbacks'";

		//strSQL.Format("%s", strSQL);

		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		i>>nStartRollBackNum;
		i.close();

		Sleep(2000);

		strSQL.Empty();
		//GetPrivateProfileString("usercommits", "usercommits", "", strSQL.GetBuffer(4096), 4096, strSQLFilePath);
		//strSQL.Format("%s", strSQL);
		strSQL="select value,name from v$sysstat  where name='user commits'";
		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		i>>nEndCommitNum;
		i.close();

		strSQL.Empty();
		//GetPrivateProfileString("userrollbacks", "userrollbacks", "", strSQL.GetBuffer(4096), 4096, strSQLFilePath);
		//strSQL.Format("%s", strSQL);
		strSQL="select value,name from v$sysstat  where name='user rollbacks'";
		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement	
              db // connect object
             );
		i>>nEndRollBackNum;
		i.close();
		int nTotalTransaction = (nEndCommitNum+nEndRollBackNum-nStartCommitNum-nStartRollBackNum)/2;
		
//printf("4");
		//数据库锁数量
		strSQL.Empty();
		//GetPrivateProfileString("locks", "locks", "", strSQL.GetBuffer(4096), 4096, strSQLFilePath);
		//strSQL.Format("%s", strSQL);
		strSQL="SELECT COUNT(*) FROM SYS.V_$LOCK WHERE TYPE IN ('UL','TM','TX')";

		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		int nTotalLock = 0;
		i>>nTotalLock;
		i.close();
//printf("5");
		//是否有死锁
		strSQL.Empty();
		//GetPrivateProfileString("deadlocks", "deadlocks", "", strSQL.GetBuffer(4096), 4096, strSQLFilePath);
		//strSQL.Format("%s", strSQL);
		strSQL="select count(*) from v$locked_object";
		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );

		//CString strlastOra;

		//GetPrivateProfileString("deadlocks", "deadlocks", "", lastOra, 255, strOracleRec);

		//int nlastOra =GetPrivateProfileInt(strService,"deadlocks",-1,strOracleRec);
		int nDeadLock = 0;
		i>>nDeadLock;
		//strlastOra.Format("%d",nDeadLock);
		//WritePrivateProfileString(strService,"deadlocks",strlastOra.GetBuffer(strlastOra.GetLength()),strOracleRec);

		//if(nlastOra ==-1) nDeadLock =0;
		//else nDeadLock=nDeadLock-nlastOra;

		i.close();
//printf("6");
		//共享池连接数
		
		//buffer cache hit rate
		strSQL.Empty();
		//GetPrivateProfileString("bufhitrate", "bufhitrate", "", strSQL.GetBuffer(4096), 4096, strSQLFilePath);
		//strSQL.Format("%s", strSQL);
		strSQL="select 100*(a.value+b.value-c.value) / (a.value+b.value)  from v$sysstat a, ";
		strSQL+=" v$sysstat b, v$sysstat c where a.name='db block gets' and b.name='consistent gets' and ";
		strSQL+=" c.name='physical reads'";
		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		float fBufferHitRate = 0.0;
		i>>fBufferHitRate;
		i.close();
//printf("7");
		//library cache hit rate
		strSQL.Empty();
		//GetPrivateProfileString("libhitrate", "libhitrate", "", strSQL.GetBuffer(4096), 4096, strSQLFilePath);
		//strSQL.Format("%s", strSQL);
		strSQL=" Select 100*sum(pins -reloads)/sum(pins) from v$librarycache";
		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		float fLibHitRate = 0.0;
		i>>fLibHitRate;
		i.close();
		/*
//printf("8");
		//总进程内存利用率
		strSQL.Empty();
		//GetPrivateProfileString("procmem", "procmem", "", strSQL.GetBuffer(4096), 4096, strSQLFilePath);
		//strSQL.Format("%s", strSQL);
		strSQL=" select 100*sum(PGA_USED_MEM)/sum(PGA_ALLOC_MEM) from v$process";
		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		float fProcMemPre = 0.0;
		i>>fProcMemPre;
		i.close();
		*/
//printf("9");		
		int nEnd = GetTickCount();

		/*sprintf(szReturn, "SV_Cursor=%d$SV_Session=%d$SV_Transaction=%d$SV_Lock=%d$" \
			"SV_DeadLock=%d$SV_BufHitRate=%f$SV_LibHitRate=%f$SV_fProcMemPre=%f$SV_time=%d$", 
			nTotalCursor, nTotalSession, nTotalTransaction, nTotalLock,
			nDeadLock, fBufferHitRate, fLibHitRate, fProcMemPre, (nEnd-nBegin)/1000);
			*/
		sprintf(szReturn, "SV_Cursor=%d$SV_Session=%d$SV_Transaction=%d$SV_Lock=%d$" \
			"SV_DeadLock=%d$SV_BufHitRate=%f$SV_LibHitRate=%f$SV_time=%d$", 
			nTotalCursor, nTotalSession, nTotalTransaction, nTotalLock,
			nDeadLock, fBufferHitRate, fLibHitRate,  (nEnd-nBegin)/1000);
		CString strInput ;
		strInput =szReturn;
		MakeCharByString(szReturn,nSize,strInput);
	}
	catch(otl_exception& p)
	{ // intercept OTL exceptions
//		cerr<<p.msg<<endl; // print out error message
//		cerr<<p.stm_text<<endl; // print out SQL that caused the error
//		cerr<<p.var_info<<endl; // print out the variable that caused the error
		sprintf(szReturn, "error=%s,%s$", p.msg, p.var_info);
		strLog.Format("GetOracleInfo ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
		db.logoff(); // disconnect from ODBC
		return FALSE;
	}
	db.logoff(); // disconnect from ODBC

	return TRUE;
}


//数据库表空间监测
extern "C" __declspec(dllexport) 
BOOL OracleDB(const char * strParas, char * szReturn, int& nSize)
//(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	char * strMonitorId = GetParaValue(strParas,"_MonitorID=");
	char strFileName[128] = {0};
	sprintf(strFileName,"Oracle_OracleDB_%s",strMonitorId);

	CString		strDBConn = _T(""), 
				strQuery = _T("");

	CString		strDBMatch = _T(""), 
				strDBUser = _T(""), 
				strDBPass = _T(""), 
				strDBDriver = _T(""), 
				strTableSpaceName= _T(""), 
				strQFile = _T("");
	CString strLog = _T("");
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__ServiceName__) == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__ServiceName__));
		}

		else if(strTemp.Find(__USERACCOUNT__) == 0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__) == 0)
		{
			strDBPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__TABLESPACENAME__) == 0) 
		{
			strTableSpaceName=strTemp.Right(strTemp.GetLength() - strlen(__TABLESPACENAME__));			
		}
		else if(strTemp.Find("_ServiceName=") == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen("_ServiceName="));
		}
	}

	strLog.Format("_ServiceName=%s _UserAccount=%s _PassWord=%s strTableSpaceName=%s", 
		strDBConn, strDBUser, strDBPass, strTableSpaceName);

	if(strDBConn.IsEmpty()) 
	{
		//IDS_ORACLEPERF_1
		sprintf(szReturn,"error=%s", FuncGetStringFromIDS("SV_DATABASE",
            "DATABASE_NAME_NULL"));
		return FALSE;
	}
	
	otl_connect db;
	CString strConn = _T("");

	otl_conn::initialize();
//	strConn.Format("%s/%s@%s", strDBUser, strDBPass, strDBConn);
	//UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;  UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;;QTO=F;APA=T;
	//strConn.Format("UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;", strDBUser, strDBPass, strDBConn);
	strConn.Format("UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;PLSQLRSet=1;", strDBUser, strDBPass, strDBConn);
//	puts(strConn);
	try
	{
		db.rlogon(strConn.GetBuffer(1));
		//CString strSQLFilePath = _T("");
		//strSQLFilePath.Format("%s\\groups\\sql.ini", FuncGetInstallPath());
		CString strSQL = _T("");
		//GetPrivateProfileString("TabList", "TabList", "", strSQL.GetBuffer(4096), 4096, strSQLFilePath);

		//strSQL="SELECT A.BYTES/1024/1024 TOTAL, (A.BYTES-C.bytes)/1024/1024 USED, C.BYTES/1024/1024 FREE, (A.BYTES-C.bytes)*100/A.BYTES USED_PERCENT, ";
		//strSQL+=" (C.BYTES*100)/A.BYTES FREE_PERCENT, A.TABLESPACE_NAME FROM SYS.SM$TS_AVAIL A, SYS.SM$TS_FREE C WHERE A.TABLESPACE_NAME=C.TABLESPACE_NAME";

  //      //strSQL.Format("%s", strSQL);

		//strSQL += " and C.TABLESPACE_NAME='";
		//strSQL += strTableSpaceName;
		//strSQL += "'";

		strSQL.Format("SELECT  case v1.contents when 'TEMPORARY' then nvl(v4.bytes, 0) / 1024 / 1024 \
			else nvl(v2.bytes, 0) / 1024 / 1024 end as TotalMB,\
			case v1.contents when 'TEMPORARY' then nvl(v5.bytes, 0) / 1024 / 1024 \
			else (nvl(v2.bytes, 0) - nvl(v3.bytes, 0)) / 1024 / 1024 end as UsedMB,\
			case v1.contents when 'TEMPORARY' then nvl(nvl(v5.bytes, 0) / v4.bytes * 100, 0) \
			else nvl((nvl(v2.bytes, 0) - nvl(v3.bytes, 0)) / v2.bytes * 100, 0) end as UsedPer,\
			case v1.contents when 'TEMPORARY' then (nvl(v4.bytes, 0) - nvl(v5.bytes, 0)) / 1024 / 1024 \
			else nvl(v3.bytes, 0) / 1024 / 1024 end as FreeMB,\
			case v1.contents when 'TEMPORARY' then nvl((nvl(v4.bytes, 0) - nvl(v5.bytes, 0)) / v4.bytes * 100, 0) \
			else nvl(nvl(v3.bytes, 0) / v2.bytes * 100, 0) end as FreePer \
			FROM sys.dba_tablespaces v1,\
			(select sum(bytes) bytes from dba_data_files where tablespace_name = '%s') v2,\
			(select sum(bytes) bytes from dba_free_space where tablespace_name = '%s') v3,\
			(select sum(bytes) bytes from dba_temp_files where tablespace_name = '%s') v4,\
			(select sum(bytes_cached) bytes from v$temp_extent_pool where tablespace_name = '%s') v5 \
			where v1.tablespace_name = '%s'", strTableSpaceName, strTableSpaceName, strTableSpaceName, strTableSpaceName, strTableSpaceName);

		printf(" strSQL = %s\n", strSQL.GetBuffer(strSQL.GetLength()));
		strSQL.Replace("\t", NULL);

		printf("strSQL.Replace strSQL = %s\n", strSQL.GetBuffer(strSQL.GetLength()));
		OutputDebugString(strSQL);

		otl_stream i(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		double	fTotal = 0.0,
				fUsed = 0.0,
				fFree = 0.0,
				fUsedPer = 0.0,
				fFreePer = 0.0;
		//i>>fTotal>>fUsed>>fFree>>fUsedPer>>fFreePer;
		//fFreePer =100-fUsedPer;
		//fFree=fTotal-fUsed;
		i>>fTotal>>fUsed>>fUsedPer>>fFree>>fFreePer;

		i.close();
		sprintf(szReturn, "Total=%f$Used=%f$UsedPercent=%f$FreePercent=%f$FreeSpace=%f$", fTotal, fUsed, fUsedPer, fFreePer, fFree);
		printf("szRuturn = %s\n", szReturn);
		CString strInput ;
		strInput =szReturn;
		MakeCharByString(szReturn,nSize,strInput);
	}
	catch(otl_exception& p)
	{
//		intercept OTL exceptions
//		cerr<<p.msg<<endl; // print out error message
//		cerr<<p.stm_text<<endl; // print out SQL that caused the error
//		cerr<<p.var_info<<endl; // print out the variable that caused the error
		printf("error=%s\r\n%s$", p.msg, p.var_info);
		strLog.Format("OracleDB ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
		db.logoff(); // disconnect from ODBC
		CString sReturn;
		sReturn.Format("error=%s:%s$",p.msg,p.var_info);
		sReturn.Replace("\r\n",",");
		sprintf(szReturn,"%s",sReturn);
		db.logoff();
		return FALSE;
	}
	catch(...)
	{
		sprintf(szReturn,"error=error occured,error number is %d",::GetLastError());
		return FALSE;
	}
	db.logoff(); // disconnect from ODBC
	
	return TRUE;
}

//数据库表空间监测 OracleDB 8i 通用10
extern "C" __declspec(dllexport) 
BOOL OracleDBTableSpaceName(const char * strParas, char * szReturn, int& nSize)

{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	char * strMonitorId = GetParaValue(strParas,"_MonitorID=");
	char strFileName[128] = {0};
	sprintf(strFileName,"Oracle_OracleDB_%s",strMonitorId);

	CString		strDBConn = _T(""), 
		strQuery = _T("");

	CString		strDBMatch = _T(""), 
		strDBUser = _T(""), 
		strDBPass = _T(""), 
		strDBDriver = _T(""), 
		strTableSpaceName= _T(""), 
		strQFile = _T("");
	CString strLog = _T("");
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__ServiceName__) == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__ServiceName__));
		}

		else if(strTemp.Find(__USERACCOUNT__) == 0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__) == 0)
		{
			strDBPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__TABLESPACENAME__) == 0) 
		{
			strTableSpaceName=strTemp.Right(strTemp.GetLength() - strlen(__TABLESPACENAME__));			
		}
		else if(strTemp.Find("_ServiceName=") == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen("_ServiceName="));
		}
	}

	strLog.Format("_ServiceName=%s _UserAccount=%s _PassWord=%s strTableSpaceName=%s", 
		strDBConn, strDBUser, strDBPass, strTableSpaceName);


	if(strDBConn.IsEmpty()) 
	{
		//IDS_ORACLEPERF_1
		sprintf(szReturn,"error=%s", FuncGetStringFromIDS("SV_DATABASE",
			"DATABASE_NAME_NULL"));
		return FALSE;
	}

	otl_connect db;
	CString strConn = _T("");

	otl_conn::initialize();

	strConn.Format("UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;PLSQLRSet=1;", strDBUser, strDBPass, strDBConn);

	try
	{
		db.rlogon(strConn.GetBuffer(1));
		
		CString strSQL = _T("");
		

		strSQL.Format("SELECT  max_m, count_blocks free_blk_cnt,sum_free_m, 100*sum_free_m / sum_m \
						AS pct_free FROM ( SELECT tablespace_name,sum(bytes) / 1024 / 1024 AS sum_m \
						FROM dba_data_files GROUP BY tablespace_name), ( SELECT tablespace_name AS fs_ts_name, max(bytes) / 1024 / 1024 AS max_m, count(blocks) \
						AS count_blocks, sum(bytes / 1024 / 1024) AS sum_free_m FROM dba_free_space \
						GROUP BY tablespace_name ) WHERE tablespace_name = fs_ts_name and tablespace_name = '%s' order by pct_free", strTableSpaceName);

		strSQL.Replace("\t", NULL);
		OutputDebugString(strSQL);
		printf("strSQL = %s\n", strSQL.GetBuffer(strSQL.GetLength()));

		otl_stream i(2000, // buffer size
			strSQL.GetBuffer(0),
			// SELECT statement
			db // connect object
			);
		double	fTotal = 0.0,
			fUsed = 0.0,
			fFree = 0.0,
			fUsedPer = 0.0,
			fFreePer = 0.0,

			fMAX_M = 0.0,
			fFREE_BLK_CNT = 0.0,
			fSUM_FREE_M = 0.0,
			fPCT_FREE = 0.0;



		i>>fMAX_M>>fFREE_BLK_CNT>>fSUM_FREE_M>>fPCT_FREE;
			
		

		i.close();

		fTotal   = fSUM_FREE_M / fPCT_FREE * 100;
		fUsed    = fTotal - fSUM_FREE_M;
		fUsedPer = 100.0 - fPCT_FREE;
		fFreePer = fPCT_FREE;
		fFree    = fSUM_FREE_M;

		printf("Max_m = %f, FREE_BLK_CNT = %f, SUM_FREE_M = %f, PCT_FREE = %f\n", fTotal, fUsed, fUsedPer, fFree);
		//printf("close csResult = %s lenth = %d\n", szTableSpaceName, abc.length());

		//char * strEnd = csResult;
		//while(*strEnd)
		//{
		//	printf("Oracle = %s\n", strEnd);
		//	strEnd += strlen(strEnd) +1;
		//}

		sprintf(szReturn, "Total=%f$Used=%f$UsedPercent=%f$FreePercent=%f$FreeSpace=%f$", fTotal, fUsed, fUsedPer, fFreePer, fFree);
		printf("szReturn = %s\n", szReturn);
		CString strInput ;
		strInput =szReturn;
		MakeCharByString(szReturn,nSize,strInput);
	}
	catch(otl_exception& p)
	{
		//		intercept OTL exceptions
		//		cerr<<p.msg<<endl; // print out error message
		//		cerr<<p.stm_text<<endl; // print out SQL that caused the error
		//		cerr<<p.var_info<<endl; // print out the variable that caused the error
		printf("error=%s\r\n%s$", p.msg, p.var_info);
		strLog.Format("OracleDB ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
		db.logoff(); // disconnect from ODBC
		CString sReturn;
		sReturn.Format("error=%s:%s$",p.msg,p.var_info);
		sReturn.Replace("\r\n",",");
		sprintf(szReturn,"%s",sReturn);
		db.logoff();
		return FALSE;
	}
	catch(...)
	{
		sprintf(szReturn,"error=error occured,error number is %d",::GetLastError());
		return FALSE;
	}
	db.logoff(); // disconnect from ODBC

	return TRUE;
}
//占用时间最长的sql语句 dy2010-09-17
extern "C" __declspec(dllexport) 
BOOL OracleMaxTimeSqlText(const char * strParas, char * szReturn, int& nSize)

{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	char * strMonitorId = GetParaValue(strParas,"_MonitorID=");
	char strFileName[128] = {0};
	sprintf(strFileName,"Oracle_OracleDB_%s",strMonitorId);

	CString		strDBConn = _T(""), 
		strQuery = _T("");

	CString		strDBMatch = _T(""), 
		strDBUser = _T(""), 
		strDBPass = _T(""), 
		strDBDriver = _T(""), 
		strTableSpaceName= _T(""), 
		strQFile = _T("");
	CString strLog = _T("");
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__ServiceName__) == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__ServiceName__));
		}

		else if(strTemp.Find(__USERACCOUNT__) == 0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__) == 0)
		{
			strDBPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find("_ServiceName=") == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen("_ServiceName="));
		}
	}

	strLog.Format("_ServiceName=%s _UserAccount=%s _PassWord=%s strTableSpaceName=%s", 
		strDBConn, strDBUser, strDBPass, strTableSpaceName);


	if(strDBConn.IsEmpty()) 
	{
		//IDS_ORACLEPERF_1
		sprintf(szReturn,"error=%s", FuncGetStringFromIDS("SV_DATABASE",
			"DATABASE_NAME_NULL"));
		return FALSE;
	}

	otl_connect db;
	CString strConn = _T("");

	otl_conn::initialize();

	strConn.Format("UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;PLSQLRSet=1;", strDBUser, strDBPass, strDBConn);

	try
	{
		db.rlogon(strConn.GetBuffer(1));

		CString strSQL = _T("");


		strSQL.Format("select * from (select elapsed_time,SQL_TEXT from v$sql order by elapsed_time desc) where rownum=1");

		strSQL.Replace("\t", NULL);
		OutputDebugString(strSQL);
		printf("strSQL = %s\n", strSQL.GetBuffer(strSQL.GetLength()));

		otl_stream i(2000, // buffer size
			strSQL.GetBuffer(0),
			// SELECT statement
			db // connect object
			);
		char sqlText[1024] = {0};
		double elapsed_time = 0;

		i>> elapsed_time >> sqlText;

		i.close();

		string strTextShort = sqlText;
		if (strTextShort.length()>100)
		{
			char  sqlTextShort[110] = {0};  
			strncpy(sqlTextShort, sqlText, 100); //dy:2010-09-26只取前100个,太长了MonitorSchedule解析不了
			strcat(sqlTextShort, "...");
			sprintf(szReturn, "ELAPSED_TIME=%f$SQL_Text=%s$", elapsed_time/1000000,sqlTextShort);
		}
		else
		{
			sprintf(szReturn, "ELAPSED_TIME=%f$SQL_Text=%s$", elapsed_time/1000000,strTextShort.c_str());
		}
		//elapsed_time = elapsed_time / 1000000; //转换为秒(S)
		printf("szReturn = %s\n", szReturn);
		CString strInput ;
		strInput =szReturn;
		MakeCharByString(szReturn,nSize,strInput);
	}
	catch(otl_exception& p)
	{
		//		intercept OTL exceptions
		//		cerr<<p.msg<<endl; // print out error message
		//		cerr<<p.stm_text<<endl; // print out SQL that caused the error
		//		cerr<<p.var_info<<endl; // print out the variable that caused the error
		printf("error=%s\r\n%s$", p.msg, p.var_info);
		strLog.Format("OracleDB ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
		db.logoff(); // disconnect from ODBC
		CString sReturn;
		sReturn.Format("error=%s:%s$",p.msg,p.var_info);
		sReturn.Replace("\r\n",",");
		sprintf(szReturn,"%s",sReturn);
		db.logoff();
		return FALSE;
	}
	catch(...)
	{
		sprintf(szReturn,"error=error occured,error number is %d",::GetLastError());
		return FALSE;
	}
	db.logoff(); // disconnect from ODBC

	return TRUE;
}


bool MakeCharByStringDollar(char *pOut,int &nOutSize,CString strInput );//SQL语句里面有$符号，用这个解析

//西塔 5分钟全网流量 //yi.duan 2011-05-05 add
extern "C" __declspec(dllexport) 
BOOL allNetRateFiveMin(const char * strParas, char * szReturn, int& nSize)

{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	char * strMonitorId = GetParaValue(strParas,"_MonitorID=");
	char strFileName[128] = {0};
	sprintf(strFileName,"Oracle_OracleDB_%s",strMonitorId);

	CString		strDBConn = _T(""), 
		strQuery = _T("");

	CString		strDBMatch = _T(""), 
		strDBUser = _T(""), 
		strDBPass = _T(""), 
		strDBDriver = _T(""), 
		strTableSpaceName= _T(""), 
		strQFile = _T("");
	CString strLog = _T("");
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__ServiceName__) == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__ServiceName__));
		}

		else if(strTemp.Find(__USERACCOUNT__) == 0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__) == 0)
		{
			strDBPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find("_ServiceName=") == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen("_ServiceName="));
		}
	}

	strLog.Format("_ServiceName=%s _UserAccount=%s _PassWord=%s strTableSpaceName=%s", 
		strDBConn, strDBUser, strDBPass, strTableSpaceName);


	if(strDBConn.IsEmpty()) 
	{
		//IDS_ORACLEPERF_1
		sprintf(szReturn,"error=%s", FuncGetStringFromIDS("SV_DATABASE",
			"DATABASE_NAME_NULL"));
		return FALSE;
	}

	otl_connect db;
	CString strConn = _T("");

	otl_conn::initialize();

	strConn.Format("UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;PLSQLRSet=1;", strDBUser, strDBPass, strDBConn);

	try
	{
		db.rlogon(strConn.GetBuffer(1));

		CString strSQL = _T("");
		CString strCurTime = _T("");
		CString strOldTime = _T("");

		CTime    curTime;
		CTime    oldTime;

		curTime = CTime::GetCurrentTime();
		oldTime = curTime - CTimeSpan(0,0,5,0); 
;
		strCurTime    = curTime.Format("%Y-%m-%d %H:%M:%S");
		strOldTime    = oldTime.Format("%Y-%m-%d %H:%M:%S");

		printf("strCurTime = %s\n", strCurTime.GetBuffer(strCurTime.GetLength()));
		printf("strOldTime = %s\n", strOldTime.GetBuffer(strOldTime.GetLength()));

		strSQL.Format("SELECT Sum(stat_total_bytes), report_sec FROM tana.frt_stat_total WHERE  report_sec >=To_Date('%s', 'YYYY-MM-DD HH24:MI:SS') AND report_sec <= To_Date('%s', 'YYYY-MM-DD HH24:MI:SS')  group by report_sec order by  report_sec desc ;", strOldTime, strCurTime );

		printf("strSQL = %s\n", strSQL.GetBuffer(strSQL.GetLength()));

		otl_stream i(2000, // buffer size
			strSQL.GetBuffer(0),
			// SELECT statement
			db // connect object
			);
	
		int total_byte = 0;

		i>>  total_byte;

		i.close();

		sprintf(szReturn, "total_bytes=%d$", total_byte);

		printf("szReturn = %s\n", szReturn);
		CString strInput ;
		strInput =szReturn;
		MakeCharByString(szReturn,nSize,strInput);
		
	}
	catch(otl_exception& p)
	{
		printf("error=%s\r\n%s$", p.msg, p.var_info);
		strLog.Format("OracleDB ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
		db.logoff(); // disconnect from ODBC
		CString sReturn;
		sReturn.Format("error=%s:%s$",p.msg,p.var_info);
		sReturn.Replace("\r\n",",");
		sprintf(szReturn,"%s",sReturn);
		db.logoff();
		return FALSE;
	}
	catch(...)
	{
		sprintf(szReturn,"error=error occured,error number is %d",::GetLastError());
		return FALSE;
	}
	db.logoff(); // disconnect from ODBC

	return TRUE;
}

////占用资源最多的sql语句(其实这里是内存最多) dy2010-09-17
BOOL OracleMaxResourcesSqlText(const char * strParas, char * szReturn, int& nSize)

{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	char * strMonitorId = GetParaValue(strParas,"_MonitorID=");
	char strFileName[128] = {0};
	sprintf(strFileName,"Oracle_OracleDB_%s",strMonitorId);

	CString		strDBConn = _T(""), 
		strQuery = _T("");

	CString		strDBMatch = _T(""), 
		strDBUser = _T(""), 
		strDBPass = _T(""), 
		strDBDriver = _T(""), 
		strTableSpaceName= _T(""), 
		strQFile = _T("");
	CString strLog = _T("");
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__ServiceName__) == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__ServiceName__));
		}

		else if(strTemp.Find(__USERACCOUNT__) == 0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__) == 0)
		{
			strDBPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find("_ServiceName=") == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen("_ServiceName="));
		}
	}

	strLog.Format("_ServiceName=%s _UserAccount=%s _PassWord=%s strTableSpaceName=%s", 
		strDBConn, strDBUser, strDBPass, strTableSpaceName);


	if(strDBConn.IsEmpty()) 
	{
		//IDS_ORACLEPERF_1
		sprintf(szReturn,"error=%s", FuncGetStringFromIDS("SV_DATABASE",
			"DATABASE_NAME_NULL"));
		return FALSE;
	}

	otl_connect db;
	CString strConn = _T("");

	otl_conn::initialize();

	strConn.Format("UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;PLSQLRSet=1;", strDBUser, strDBPass, strDBConn);

	try
	{
		db.rlogon(strConn.GetBuffer(1));

		CString strSQL = _T("");


		//strSQL.Format("select * from (select SQL_TEXT from v$sql order by SHARABLE_MEM desc) where rownum=1");
		strSQL.Format("select * from (select SQL_TEXT,(PERSISTENT_MEM+RUNTIME_MEM+SHARABLE_MEM) Memory from v$sql order by (PERSISTENT_MEM+RUNTIME_MEM+SHARABLE_MEM) desc) where rownum=1 ;");

		strSQL.Replace("\t", NULL);
		OutputDebugString(strSQL);
		printf("strSQL = %s\n", strSQL.GetBuffer(strSQL.GetLength()));

		otl_stream i(2000, // buffer size
			strSQL.GetBuffer(0),
			// SELECT statement
			db // connect object
			);
		char   sqlText[10240] = {0};
		double memory = 0;

		i>> sqlText >> memory;

		i.close();

		string strTextShort = sqlText;
		if (strTextShort.length()>100)
		{
			char  sqlTextShort[110] = {0};  
			strncpy(sqlTextShort, sqlText,100); //dy:2010-09-26只取前100个,太长了MonitorSchedule解析不了
			strcat(sqlTextShort, "...");
			sprintf(szReturn, "SQL_Text=%s^MEMORY=%f^",sqlTextShort, memory/1024);
		}
		else
		{
			sprintf(szReturn, "SQL_Text=%s^MEMORY=%f^",strTextShort.c_str(), memory/1024);
		}
		printf("szReturn = %s\n", szReturn);
		CString strInput ;
		strInput =szReturn;
		MakeCharByStringDollar(szReturn,nSize,strInput);

	}
	catch(otl_exception& p)
	{
		//		intercept OTL exceptions
		//		cerr<<p.msg<<endl; // print out error message
		//		cerr<<p.stm_text<<endl; // print out SQL that caused the error
		//		cerr<<p.var_info<<endl; // print out the variable that caused the error
		printf("error=%s\r\n%s$", p.msg, p.var_info);
		strLog.Format("OracleDB ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
		db.logoff(); // disconnect from ODBC
		CString sReturn;
		sReturn.Format("error=%s:%s$",p.msg,p.var_info);
		sReturn.Replace("\r\n",",");
		sprintf(szReturn,"%s",sReturn);
		db.logoff();
		return FALSE;
	}
	catch(...)
	{
		sprintf(szReturn,"error=error occured,error number is %d",::GetLastError());
		return FALSE;
	}
	db.logoff(); // disconnect from ODBC

	return TRUE;
}

extern "C" __declspec(dllexport)
BOOL	GetSessions(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	char * strMonitorId = GetParaValue(strParas,"_MonitorID=");
	char strFileName[128] = {0};
	sprintf(strFileName,"Oracle_GetSessions_%s",strMonitorId);

	CString strUser = _T("");
	CString strPwd = _T("");
	CString strService = _T("");
	CString strLog = _T("");
	CString strMonitorID = _T("");

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	
	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTmp = _T("");
		strTmp = paramList.GetNext(pos);
		if (0 == strTmp.Find(__USERACCOUNT__))
		{
			strUser = strTmp.Right(strTmp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if (0 == strTmp.Find(__PASSWORD__))
		{
			strPwd = strTmp.Right(strTmp.GetLength() - strlen(__PASSWORD__));
		}
		else if (0 == strTmp.Find(__ServiceName__))
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen(__ServiceName__));
		}
		else if(strTmp.Find(__MONITORID__) == 0) 
		{
			strMonitorID=strTmp.Right(strTmp.GetLength() - strlen(__MONITORID__));			
		}
		else if(strTmp.Find("_ServiceName=") == 0)
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen("_ServiceName="));
		}
	}

	CString strPath, strTime;
	strPath.Format("..\\data\\TmpIniFile\\ora_%s.ini", strMonitorID);
	COleDateTime tCurrTime =  COleDateTime::GetCurrentTime();
	strTime = tCurrTime.Format("%Y-%m-%d %H:%M:%S");

	strLog.Format("_ServiceName=%s _UserAccount=%s _PassWord=%s", 
		strService, strUser, strPwd);
	//WriteLog(strLog, 220);

	if (strService.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_ORACLE",
            "ORACLE_SERVERNAME_NOT_FOUND"));
		return FALSE;
	}

	otl_connect db;
	CString strConn = _T("");

	otl_conn::initialize();
//	strConn.Format("%s/%s@%s", strUser, strPwd, strService);
	strConn.Format("UID=%s;PWD=%s;DSN=%s", strUser, strPwd, strService);

	try
	{
		char pOldValue[256];
		COleDateTime tTime;
		BOOL bFlag = TRUE;
		db.rlogon(strConn.GetBuffer(1));
		CString strSQL = _T("");
		CString strSQLFilePath = _T("");
		//strSQLFilePath.Format("%s\\groups\\sql.ini", FuncGetInstallPath());
		//GetPrivateProfileString("Connections", "Connections", "", strSQL.GetBuffer(4096), 4096, strSQLFilePath);
		//strSQL.Format("%s", strSQL);
		//strSQL.Format("SELECT A.BYTES/1024/1024 TOTAL, \
		//	B.BYTES/1024/1024 USED, C.BYTES/1024/1024 FREE, (B.BYTES*100)/A.BYTES USED_PERCENT, \
		//	(C.BYTES*100)/A.BYTES FREE_PERCENT, A.TABLESPACE_NAME FROM SYS.SM$TS_AVAIL A, \
		//	SYS.SM$TS_USED B,SYS.SM$TS_FREE C \
		//	WHERE A.TABLESPACE_NAME=B.TABLESPACE_NAME \
		//	AND A.TABLESPACE_NAME=C.TABLESPACE_NAME");
		strSQL.Format("select value from v$sysstat where name='logons current'");


		otl_stream i(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		int nSessionSum = 0;
		i>>nSessionSum;

		strSQL.Empty();
		strSQL="select value from v$sysstat where name='logons cumulative'";
		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		unsigned int nLogonsCum = 0;
		i>>nLogonsCum;
		
		int nLogonsCum1m = -1;
		float fInterval = -1;
		int nSub = -1;
		GetPrivateProfileString("LogonsCum", "Time", "-1", pOldValue, 256, strPath);
		if(tTime.ParseDateTime(pOldValue))
		{
			GetPrivateProfileString("LogonsCum", "HistoryValue", "-1", pOldValue, 256, strPath);
			fInterval = (tCurrTime - tTime).GetTotalSeconds()/60;
			nSub = nLogonsCum - atoi(pOldValue);
			nLogonsCum1m = nSub/fInterval;
		}
		else if(atoi(pOldValue) != -1)
			return FALSE;

		if(nSub < 0 || fInterval < 0)
			bFlag = FALSE;		

		i.close();
		
		//写历史数据
		CString strTemp;
		WritePrivateProfileString("LogonsCum", "Time", strTime, strPath);
		strTemp.Format("%u", nLogonsCum);
		WritePrivateProfileString("LogonsCum", "HistoryValue", strTemp, strPath);

		if(bFlag)
		{
			sprintf(szReturn, "SV_sessions=%d$LogonsCum=%d$LogonsCum1m=%d$", nSessionSum, nLogonsCum, nLogonsCum1m);
			CString strInput ;
			strInput =szReturn;
			MakeCharByString(szReturn,nSize,strInput);
		}
		else
		{
			Sleep(1000);		
			GetSessions(strParas, szReturn, nSize);
		}
	}
	catch(otl_exception& p)
	{ // intercept OTL exceptions
//		cerr<<p.msg<<endl; // print out error message
//		cerr<<p.stm_text<<endl; // print out SQL that caused the error
//		cerr<<p.var_info<<endl; // print out the variable that caused the error
		sprintf(szReturn, "error=%s\r\n%s", p.msg, p.var_info);
		strLog.Format("GetSessions ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
//		WriteLog(strLog, 220);
		db.logoff(); // disconnect from ODBC
		return FALSE;
	}
	db.logoff(); // disconnect from ODBC
	
	return TRUE;
}

extern "C" __declspec(dllexport) 
BOOL OracleTableName(const char * strParas, char * strReturn, int& nSize)
					 //CStringList &paramList, CStringList &lstTexts, CStringList &lstValues)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	PrintLog(">>>>>>>>获取表空间名称<<<<<<<<" , "Oracle");
	const char * strReceive = strParas;
	PrintLog("传入的参数", "Oracle");
	while(*strReceive)
	{
		PrintLog(strReceive, "Oracle");
		strReceive += strlen(strReceive) + 1;
	}

	CString		strDBConn = _T(""), 
				strQuery = _T("");

	CString		strDBMatch = _T(""), 
				strDBUser = _T(""), 
				strDBPass = _T(""), 
				strDBDriver = _T(""), 
				strQFile = _T("");
	CString strLog = _T("");

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

//	WriteResultFile("===================================",szoutFile,1);
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{

		CString strTemp = paramList.GetNext(pos);
		//WriteResultFile(strTemp,szoutFile,1);
		if(strTemp.Find(__ServiceName__) == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__ServiceName__));
//			WriteResultFile(strDBConn,szoutFile,1);
		}

		else if(strTemp.Find(__USERACCOUNT__) == 0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		//	WriteResultFile(strDBUser,szoutFile,1);
		}
		else if(strTemp.Find(__PASSWORD__) == 0)
		{
			strDBPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
//			WriteResultFile(strDBPass,szoutFile,1);
		}
		else if(strTemp.Find("_ServiceName=") == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen("_ServiceName="));
		}
	}

//	WriteResultFile("===================================",szoutFile,1);
	
	strLog.Format("_ServiceName=%s _UserAccount=%s _PassWord=%s", 
		strDBConn, strDBUser, strDBPass);

//	WriteResultFile(strLog,szoutFile,1) ;
	if(strDBConn.IsEmpty())
	{
		//IDS_ORACLEPERF_1
		//sprintf(szReturn,"error=%s", FuncGetStringFromIDS("SV_DATABASE",
        //    "DATABASE_NAME_NULL"));
		return FALSE;
	}
//	WriteResultFile(strLog,szoutFile,1) ;

	BOOL bResult = DB_TABLESPACENAME(strDBConn.GetBuffer(strDBConn.GetLength()), 
							strDBUser.GetBuffer(strDBUser.GetLength()), 
							strDBPass.GetBuffer(strDBPass.GetLength()), 
							strReturn,
							nSize);

	return bResult ;
	
}

BOOL DB_TABLESPACENAME(char *dbconn, char *uid, char *pwd, char * strReturn, int& nSize)
					   //CStringList &lstTexts,		   CStringList &lstValues)
{
	PrintLog("********DB_TABLESPACENAME Start********", "Oracle");
	otl_connect db;
	CString strConn = _T("");
	CString strLog = _T("");
	otl_conn::initialize();
//	strConn.Format("%s/%s@%s", uid, pwd, dbconn);


	strConn.Format("UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;", uid, pwd, dbconn);
	try
	{
		db.rlogon(strConn.GetBuffer(1));
		PrintLog("连上数据库，准备获取表空间名" , "Oracle");
		otl_stream i(100, // buffer size
              "SELECT tablespace_name FROM DBA_TABLESPACES",
                 // SELECT statement
              db // connect object
             );
		PrintLog("已经获取到表空间名，保存到输出流中" , "Oracle");
		char szTableSpaceName[256] = {0};
		char* p=strReturn;
		while(!i.eof())
		{
			i>>szTableSpaceName;
			//lstTexts.AddTail(szTableSpaceName);
			//lstValues.AddTail(szTableSpaceName);
			sprintf(p,"%s=%s",szTableSpaceName,szTableSpaceName);
			p+= 2* strlen(szTableSpaceName)+2;
		}
		char * strEnd = p;
		PrintLog("获得的表空间名" , "Oracle");
		while(*strEnd)
		{
			PrintLog(strEnd, "Oracle");
			strEnd += strlen(strEnd) +1;
		}
		i.close();
	}
	catch(otl_exception& p)
	{ 
// intercept OTL exceptions
//		cerr<<p.msg<<endl; // print out error message
//		cerr<<p.stm_text<<endl; // print out SQL that caused the error
//		cerr<<p.var_info<<endl; // print out the variable that caused the error
		printf("error=%s\r\n%s", p.msg, p.var_info);
		strLog.Format("DB_TABLESPACENAME ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
		db.logoff(); // disconnect from ODBC
		sprintf(strReturn,"%s=%s",p.msg,p.var_info);
		//lstTexts.AddTail(p.msg);
		//lstValues.AddTail(p.var_info);
		
		
		return FALSE;
	}
	db.logoff(); // disconnect from ODBC

			
	return TRUE;
}

extern "C" __declspec(dllexport) 
BOOL OracleProcName(const char * strParas, char * strReturn, int& nSize)
//(CStringList &paramList, CStringList &lstTexts, CStringList &lstValues)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	PrintLog(">>>>>>>>Oracle进程名<<<<<<<<" , "Oracle");
	const char * strReceive = strParas;
	PrintLog("传入的参数", "Oracle");
	while(*strReceive)
	{
		PrintLog(strReceive, "Oracle");
		strReceive += strlen(strReceive) + 1;
	}

	CString		strDBConn = _T(""), 
				strQuery = _T("");

	CString		strDBMatch = _T(""), 
				strDBUser = _T(""), 
				strDBPass = _T(""), 
				strDBDriver = _T(""), 
				strQFile = _T("");
	CString strLog = _T("");

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);


	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__ServiceName__) == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__ServiceName__));
		}

		else if(strTemp.Find(__USERACCOUNT__) == 0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__) == 0)
		{
			strDBPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find("_ServiceName=") == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen("_ServiceName="));
		}
	}

	strLog.Format("_ServiceName=%s _UserAccount=%s _PassWord=%s", 
		strDBConn, strDBUser, strDBPass);

	if(strDBConn.IsEmpty())
	{
		//IDS_ORACLEPERF_1
		//sprintf(szReturn,"error=%s", FuncGetStringFromIDS("SV_DATABASE",
        //    "DATABASE_NAME_NULL"));
		return FALSE;
	}

    PrintDebugString(strLog.GetBuffer(0));


	BOOL bResult = DB_PROCESSNAME(strDBConn.GetBuffer(strDBConn.GetLength()), 
							strDBUser.GetBuffer(strDBUser.GetLength()), 
							strDBPass.GetBuffer(strDBPass.GetLength()), 
							strReturn,
							nSize);
	return bResult ;
	
}

BOOL DB_PROCESSNAME(char *dbconn, char *uid, char *pwd,     char* szReturn,	   int nSize)
{
	otl_connect db;
	CString strConn = _T("");
	CString strLog = _T("");

    PrintDebugString("otl_conn initialize!");

	otl_conn::initialize();
//	strConn.Format("%s/%s@%s", uid, pwd, dbconn);
	strConn.Format("UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;", uid, pwd, dbconn);
	try
	{
        PrintDebugString(strConn.GetBuffer(0));

        
		db.rlogon(strConn.GetBuffer(1));
		otl_stream i(100, // buffer size
              "SELECT Program, pid FROM v$process",
                 // SELECT statement
              db // connect object
             );
		char szTableSpaceName[256] = {0};
		int	nPid = 0;
		CString strName = _T("");
		char *p;
		int j=0;
		p=szReturn;
	
		int nRecordNum = 0;
        PrintDebugString("get data succ!");
		while(!i.eof())
		{
			if(nRecordNum > 120)
				break;
			
			i>>szTableSpaceName>>nPid;
			strName.Format("%s(PID:%d)", szTableSpaceName, nPid);

			if(j>0)
			{
				sprintf(p,"%s=%s",strName,strName);
				//printf("%s\n", p);
				p+= 2* strlen(strName)+2;
			}
			j++;
			nRecordNum ++;
			//lstTexts.AddTail(strName);
			//lstValues.AddTail(strName);
		}
		
        PrintDebugString("close recordset");
		i.close();
		char * strEnd = p;
		PrintLog("传入的参数", "Oracle");
		while(*strEnd)
		{
			PrintLog(strEnd, "Oracle");
			strEnd += strlen(strEnd) + 1;
		}

/*
		/////////////////////////////去头去尾,chenxingang
		if((!lstTexts.IsEmpty())&&(!lstValues.IsEmpty()))
		{
			lstTexts.RemoveHead();
			lstValues.RemoveHead();
		}
		
		if((!lstTexts.IsEmpty())&&(!lstValues.IsEmpty()))
		{
			lstTexts.RemoveTail();
			lstValues.RemoveTail();
		}
		////////////////////////////////
		*/
	}
	catch(otl_exception& p)
	{ // intercept OTL exceptions
//		cerr<<p.msg<<endl; // print out error message
//		cerr<<p.stm_text<<endl; // print out SQL that caused the error
//		cerr<<p.var_info<<endl; // print out the variable that caused the error
		printf("error=%s\r\n%s", p.msg, p.var_info);
		strLog.Format("DB_PROCESSNAME ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
		db.logoff(); // disconnect from ODBC
		sprintf(szReturn,"error=%s:%s",p.msg,p.var_info);
//		lstTexts.AddTail(p.msg);
//		lstValues.AddTail(p.var_info);
		return FALSE;
	}
	db.logoff(); // disconnect from ODBC
			
	return TRUE;
}

//进程内存利用率
extern "C" __declspec(dllexport)
BOOL GetProcMemUsePercent(const char * strParas, char * szReturn, int& nSize)
//(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	char * strMonitorId = GetParaValue(strParas,"_MonitorID=");
	char strFileName[128] = {0};
	sprintf(strFileName,"Oracle_GetProcMemUsePercent_%s",strMonitorId);

	/*for debug zjw
	CStringList strlist;
	MakeStringListByChar(strlist, strParas);
	POSITION pos1=NULL;
	pos1 = strlist.GetHeadPosition();
	CString strItem = "";

	CString str, sTime;
	sTime = COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
	while(pos1 != NULL)
	{
		strItem = strlist.GetNext(pos1);
		strItem += " for debug zjw";
		ofstream fout("orapara.txt",ios::app);
		fout << strItem <<"\r\n"; 
		fout << flush; 
		fout.close(); 
	}
	ofstream fout("orapara.txt",ios::app);
	fout <<"\r\n"; 
	fout << flush; 
	fout.close(); 
	*/

	CString		strDBConn = _T(""), 
				strQuery = _T("");
	CString strLog = _T("");

	CString		strDBMatch = _T(""), 
				strDBUser = _T(""), 
				strDBPass = _T(""), 
				strDBDriver = _T(""), 
				strProcName= _T(""), 
				strQFile = _T(""),
				strMonitorID = _T("");

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__ServiceName__) == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__ServiceName__));
		}

		else if(strTemp.Find(__USERACCOUNT__) == 0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__) == 0)
		{
			strDBPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROCESSNAME__) == 0) 
		{
			strProcName=strTemp.Right(strTemp.GetLength() - strlen(__PROCESSNAME__));			
		}
		else if(strTemp.Find(__MONITORID__) == 0) 
		{
			strMonitorID=strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__));			
		}
		else if(strTemp.Find("_ServiceName=") == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen("_ServiceName="));
		}
	}

	CString strPath, strTime;
	strPath.Format("..\\data\\TmpIniFile\\ora_%s.ini", strMonitorID);
	COleDateTime tCurrTime =  COleDateTime::GetCurrentTime();
	strTime = tCurrTime.Format("%Y-%m-%d %H:%M:%S");

	strLog.Format("_ServiceName=%s _UserAccount=%s _PassWord=%s", 
		strDBConn, strDBUser, strDBPass);


	if(strDBConn.IsEmpty())
	{
		//IDS_ORACLEPERF_1
		sprintf(szReturn,"error=%s", FuncGetStringFromIDS("SV_DATABASE",
            "DATABASE_NAME_NULL"));
		return FALSE;
	}
	//if (strProcName.IsEmpty())
	//{
	//	sprintf(szReturn,"error=%s",FuncGetStringFromIDS("SV_BASIC",
 //           "BASIC_NOT_SELECT_MONITOR_PROCESS"));
	//	return FALSE;
	//}

	//CString strProcessName = _T("");
	//strProcessName = strProcName.Mid(strProcName.Find(":")+1, strProcName.GetLength()-strProcName.Find(":")-2);
	
	otl_connect db;
	CString strConn = _T("");

	otl_conn::initialize();
//	strConn.Format("%s/%s@%s", strDBUser, strDBPass, strDBConn);
	strConn.Format("UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;", strDBUser, strDBPass, strDBConn);
	try
	{
		char pOldValue[256];
		COleDateTime tTime;
		BOOL bFlag = TRUE;
		db.rlogon(strConn.GetBuffer(1));		
		CString strSQL = _T("");
		strSQL.Format("select 100*sum(PGA_USED_MEM)/sum(PGA_ALLOC_MEM) from v$process");// where PID=%s", strProcessName);
		try
		{
			otl_stream i(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		}
		catch(otl_exception& p)
		{
			strSQL.Empty();
			strSQL="select (select sum(value) from sys.v_$sesstat where statistic#=20)/\
			(select (select count(*) from sys.v_$session)*(2048576+a.value+b.value)   pga_size \
			from v$parameter a,v$parameter b where a.name = 'sort_area_size' and b.name = 'hash_area_size')*100 from dual";
			strSQL.Replace("\t", NULL);
		}

		otl_stream i(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		float fProcMemPer = 0.0;
		i>>fProcMemPer;

		strSQL.Empty();

		strSQL="select sum(value) total_physical_io from v$sysstat where name in ('physical reads','physical reads direct',\
		'physical reads direct (lob)','physical writes','physical writes direct','physical writes direct (lob)',\
		'physical writes non checkpoint')";
		strSQL.Replace("\t", NULL);

		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		unsigned  int nTotalPhysicalIO = 0;
		i>>nTotalPhysicalIO;

		int nPhysicalIO1m = -1;
		float fInterval = -1;
		int nSub = -1;
		GetPrivateProfileString("TotalPhysicalIO", "Time", "-1", pOldValue, 256, strPath);
		if(tTime.ParseDateTime(pOldValue))
		{
			GetPrivateProfileString("TotalPhysicalIO", "HistoryValue", "-1", pOldValue, 256, strPath);
			fInterval = (tCurrTime - tTime).GetTotalSeconds()/60;
			nSub = nTotalPhysicalIO - atoi(pOldValue);
			nPhysicalIO1m = nSub/fInterval;
		}
		else if(atoi(pOldValue) != -1)
		{
			return FALSE;
		}

		if(nSub < 0 || fInterval < 0)
			bFlag = FALSE;		

		strSQL.Empty();
		strSQL="select sum(value) total_logical_io from v$sysstat where name in ('db block gets','db block changes',\
		'consistent gets','consistent changes')";
		strSQL.Replace("\t", NULL);
	
		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		unsigned int nTotalLogicalIO = 0;
		i>>nTotalLogicalIO;

		int nLogicalIO1m = -1;
		GetPrivateProfileString("TotalLogicalIO", "Time", "-1", pOldValue, 256, strPath);
		if(tTime.ParseDateTime(pOldValue))
		{
			GetPrivateProfileString("TotalLogicalIO", "HistoryValue", "-1", pOldValue, 256, strPath);
			fInterval = (tCurrTime - tTime).GetTotalSeconds()/60;
			nSub = nTotalLogicalIO - atoi(pOldValue);
			nLogicalIO1m = nSub/fInterval;
		}
		else if(atoi(pOldValue) != -1)
		{
			return FALSE;
		}

		if(nSub < 0 || fInterval < 0)
			bFlag = FALSE;		

		strSQL.Empty();
		strSQL="select value from v$sysstat where name='sorts (disk)'";

		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		unsigned  int nSortsDisk = 0;
		i>>nSortsDisk;
		
		int nSortsDisk1m = -1;
		GetPrivateProfileString("SortsDisk", "Time", "-1", pOldValue, 256, strPath);
		if(tTime.ParseDateTime(pOldValue))
		{
			GetPrivateProfileString("SortsDisk", "HistoryValue", "-1", pOldValue, 256, strPath);
			float fInterval = (tCurrTime - tTime).GetTotalSeconds()/60;
			nSub = nSortsDisk - atoi(pOldValue);
			nSortsDisk1m = nSub/fInterval;
		}
		else if(atoi(pOldValue) != -1)
		{
			return FALSE;
		}

		if(nSub < 0)
			bFlag = FALSE;		

		strSQL.Empty();
		strSQL="select value from v$sysstat where name='sorts (memory)'";

		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		unsigned int nSortsMem = 0;
		i>>nSortsMem;

		int nSortsMem1m = -1;
		GetPrivateProfileString("SortsMem", "Time", "-1", pOldValue, 256, strPath);
		if(tTime.ParseDateTime(pOldValue))
		{
			GetPrivateProfileString("SortsMem", "HistoryValue", "-1", pOldValue, 256, strPath);
			float fInterval = (tCurrTime - tTime).GetTotalSeconds()/60;
			nSub = nSortsMem - atoi(pOldValue);
			nSortsMem1m = nSub/fInterval;
		}
		else if(atoi(pOldValue) != -1)
		{
			return FALSE;
		}

		if(nSub < 0 || fInterval < 0)
			bFlag = FALSE;		

		strSQL.Empty();
		strSQL="select value from v$sysstat where name='user commits'";

		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		unsigned  int nUserCommits = 0;
		i>>nUserCommits;
		
		int nUserCommits1m = -1;
		GetPrivateProfileString("UserCommits", "Time", "-1", pOldValue, 256, strPath);
		if(tTime.ParseDateTime(pOldValue))
		{
			GetPrivateProfileString("UserCommits", "HistoryValue", "-1", pOldValue, 256, strPath);
			float fInterval = (tCurrTime - tTime).GetTotalSeconds()/60;
			nSub = nUserCommits - atoi(pOldValue);
			nUserCommits1m = nSub/fInterval;
		}
		else if(atoi(pOldValue) != -1)
		{
			return FALSE;
		}

		if(nSub < 0 || fInterval < 0)
			bFlag = FALSE;		

		strSQL.Empty();
		strSQL="select value from v$sysstat where name='user rollbacks'";

		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		unsigned int nUserRollbacks = -1;
		i>>nUserRollbacks;

		int nUserRollbacks1m = -1;
		GetPrivateProfileString("UserRollbacks", "Time", "-1", pOldValue, 256, strPath);
		if(tTime.ParseDateTime(pOldValue))
		{
			GetPrivateProfileString("UserRollbacks", "HistoryValue", "-1", pOldValue, 256, strPath);
			float fInterval = (tCurrTime - tTime).GetTotalSeconds()/60;
			nSub = nUserRollbacks - atoi(pOldValue);
			nUserRollbacks1m = nSub/fInterval;
		}
		else if(atoi(pOldValue) != -1)
		{
			return FALSE;
		}

		if(nSub < 0 || fInterval < 0)
			bFlag = FALSE;		

		i.close();

		//写历史数据
		CString strTemp;
		WritePrivateProfileString("TotalPhysicalIO", "Time", strTime, strPath);
		strTemp.Format("%u", nTotalPhysicalIO);
		WritePrivateProfileString("TotalPhysicalIO", "HistoryValue", strTemp, strPath);

		WritePrivateProfileString("TotalLogicalIO", "Time", strTime, strPath);
		strTemp.Format("%u", nTotalLogicalIO);
		WritePrivateProfileString("TotalLogicalIO", "HistoryValue", strTemp, strPath);

		WritePrivateProfileString("SortsDisk", "Time", strTime, strPath);	
		strTemp.Format("%u", nSortsDisk);
		WritePrivateProfileString("SortsDisk", "HistoryValue", strTemp, strPath);

		WritePrivateProfileString("SortsMem", "Time", strTime, strPath);
		strTemp.Format("%u", nSortsMem);
		WritePrivateProfileString("SortsMem", "HistoryValue", strTemp, strPath);

		WritePrivateProfileString("UserCommits", "Time", strTime, strPath);	
		strTemp.Format("%u", nUserCommits);
		WritePrivateProfileString("UserCommits", "HistoryValue", strTemp, strPath);

		WritePrivateProfileString("UserRollbacks", "Time", strTime, strPath);
		strTemp.Format("%u", nUserRollbacks);
		WritePrivateProfileString("UserRollbacks", "HistoryValue", strTemp, strPath);

		if(bFlag)
		{
			CString strReturn;
			strReturn.Format("ProcMemPer=%.2f$TotalPhysicalIO=%d$PhysicalIO1m=%d$TotalLogicalIO=%d$LogicalIO1m=%d$\
				SortsDisk=%d$SortsDisk1m=%d$SortsMem=%d$SortsMem1m=%d$UserCommits=%d$UserCommits1m=%d$\
				UserRollbacks=%d$UserRollbacks1m=%d$", 
				fProcMemPer, nTotalPhysicalIO, nPhysicalIO1m, nTotalLogicalIO,nLogicalIO1m,nSortsDisk,nSortsDisk1m,
				nSortsMem,nSortsMem1m,nUserCommits,nUserCommits1m,nUserRollbacks,nUserRollbacks1m);
			strReturn.Replace("\t", NULL);
			strcpy(szReturn, strReturn);
			CString strInput ;
			strInput =szReturn;
			MakeCharByString(szReturn,nSize,strInput);
			db.logoff();
		}
		else
		{
			Sleep(1000);		
			GetProcMemUsePercent(strParas, szReturn, nSize);
		}
	}
	catch(otl_exception& p)
	{ 
//		intercept OTL exceptions
//		cerr<<p.msg<<endl; // print out error message
//		cerr<<p.stm_text<<endl; // print out SQL that caused the error
//		cerr<<p.var_info<<endl; // print out the variable that caused the error
		printf("error=%s\r\n%s$", p.msg, p.var_info);
		strLog.Format("GetProcMemUsePercent ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
		db.logoff(); // disconnect from ODBC
		CString sReturn;
		sReturn.Format("error=%s:%s$",p.msg,p.var_info);
		sReturn.Replace("\r\n",",");
		sprintf(szReturn,"%s",sReturn);
		return FALSE;
	}
	db.logoff(); // disconnect from ODBC
	
	return TRUE;
}


int GetRetMum(int nTplId)
{
    char szTplFile[MAX_PATH] = {0};
    sprintf(szTplFile, "%s\\templates.monitor\\%d.tpl", FuncGetInstallPath(), nTplId);

    int nNum = 0;
    nNum = GetPrivateProfileInt("properties", "_num", 0, szTplFile);
    return nNum;
}

void GetRetType(int nTplId, char* szSession, char* szType)
{
    char szTplFile[MAX_PATH] = {0};
    sprintf(szTplFile, "%s\\templates.monitor\\%d.tpl", FuncGetInstallPath(), nTplId);

    GetPrivateProfileString(szSession, "_type", "", szType, 16, szTplFile);
}

void GetRetName(int nTplId, char* szSession, char* szName)
{
    char szTplFile[MAX_PATH] = {0};
    sprintf(szTplFile, "%s\\templates.monitor\\%d.tpl", FuncGetInstallPath(), nTplId);

    GetPrivateProfileString(szSession, "_name", "", szName, 128, szTplFile);
}

/********************************************************************************/
/*																				*/
/*                                Oracle SGA监测器                              */
/*																				*/
/********************************************************************************/
extern "C" __declspec(dllexport)
BOOL SGAMonitor(const char * strParas, char * szReturn, int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL Ret=FALSE;
	char * strMonitorId = GetParaValue(strParas,"_MonitorID=");
	char strFileName[128] = {0};
	sprintf(strFileName,"Oracle_SGAMonitor_%s",strMonitorId);


	CString		strDBConn = _T("");
	CString		strDBUserName = _T("");
	CString		strDBPWD = _T("");
	CString		strLog = _T("");

	CStringList strList;
	MakeStringListByChar(strList , strParas);
	POSITION	pos = strList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = strList.GetNext(pos);
		if(strTemp.Find(__ServiceName__)==0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__ServiceName__));
		}
		else if(strTemp.Find(__USERACCOUNT__)==0)
		{
			strDBUserName = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__)==0)
		{
			strDBPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find("_ServiceName=") == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen("_ServiceName="));
		}
	}


	if(strDBConn.IsEmpty())
	{
		sprintf(szReturn,"error=%s","获取DSN为空！");
		return FALSE;
	}

	strLog.Format("_ServiceName=%s _USERACCOUNT=%s _PASSWORD=%s",strDBConn,strDBUserName,strDBPWD);
//    CString FieldName[10];
	char FieldName[1024]={0};
	double value[10]={0};
	otl_connect db;
	CString	strConn;
	strConn.Format("UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;",strDBUserName,strDBPWD,strDBConn);

	otl_conn::initialize();
	try
	{
		db.rlogon(strConn.GetBuffer(1));
		otl_stream i(2000,"select * from v$sga",db);
		double FixedSize=0,VariableSize=0,DatabaseBuffers=0,RedoBuffers=0;
		int n =0;
		while(!i.eof())
		{
//			i>>FieldName[n].GetBuffer(FieldName[n].GetLength())>>value[n];
			i>>FieldName>>value[n];
			cout<<FieldName<<"="<<value[n]<<endl;
			n++;
		}
		i.close();

		FixedSize = value[0];
		VariableSize = value[1];
		DatabaseBuffers =value[2];
		RedoBuffers = value[3];
		FixedSize = FixedSize/1024/1024;
		VariableSize = VariableSize/1024/1024;
		DatabaseBuffers =DatabaseBuffers/1024/1024;
		RedoBuffers = RedoBuffers/1024/1024;

		sprintf(szReturn,"FixedSize=%f$VariableSize=%f$DatabaseBuffers=%f$RedoBuffers=%f$",FixedSize,VariableSize,DatabaseBuffers,RedoBuffers);
		CString strInput = szReturn;
		MakeCharByString(szReturn,nSize,strInput);
		Ret = TRUE;
		
	}
	catch(otl_exception& e)
	{
		strLog.Format("OracleGeneral ExceptionMSG=%s ExceptionInfo=%s",e.msg,e.var_info);
		CString sReturn;
		sReturn.Format("error=%s:%s",e.msg,e.var_info);
		sReturn.Replace("\r\t",",");
		sprintf(szReturn,"%s",sReturn.GetBuffer(1));
		Ret = FALSE;
	}
	catch(...)
	{

		sprintf(szReturn,"error=%s，异常代号是：%d","出现未知错误！",GetLastError());
		Ret = FALSE;
	}
	db.logoff();

	return Ret;
}

extern "C" __declspec(dllexport) 
BOOL OracleGeneral(const char * strParas, char * szReturn, int& nSize)
//(CStringList &paramList, char *szReturn)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strDBConn = _T(""), 
				strQuery = _T("");
	CString     strLog = _T("");

	CString		strDBMatch = _T(""), 
				strDBUser = _T(""), 
				strDBPass = _T(""), 
				strDBDriver = _T(""), 
				strProcName= _T(""), 
				strQFile = _T("");
    int nTplId = 0;

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__ServiceName__) == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__ServiceName__));
		}

		else if(strTemp.Find(__USERACCOUNT__) == 0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__) == 0)
		{
			strDBPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__SQL__) == 0) 
		{
			strQuery=strTemp.Right(strTemp.GetLength() - strlen(__SQL__));
		}
        else if(strTemp.Find(__TEMPLATEID__) == 0) 
		{
			nTplId=atoi(strTemp.Right(strTemp.GetLength() - strlen(__TEMPLATEID__)).GetBuffer(0));
		}
		else if(strTemp.Find("_ServiceName=") == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen("_ServiceName="));
		}
	}

	strLog.Format("_ServiceName=%s _UserAccount=%s _PassWord=%s", 
		strDBConn, strDBUser, strDBPass);
	WriteLog(strLog, 223);

	if(strDBConn.IsEmpty())
	{
		//IDS_ORACLEPERF_1
		sprintf(szReturn,"error=%s", FuncGetStringFromIDS("SV_DATABASE",
            "DATABASE_NAME_NULL"));
		return FALSE;
	}
	
	otl_connect db;
	CString strConn = _T("");

	otl_conn::initialize();
//	strConn.Format("%s/%s@%s", strDBUser, strDBPass, strDBConn);
	strConn.Format("UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;", strDBUser, strDBPass, strDBConn);
	try
	{
		db.rlogon(strConn.GetBuffer(1));		
		otl_stream i(200, // buffer size
              strQuery.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		
        //从配置文件中取得返回值个数
        int nRets = GetRetMum(nTplId);

        //根据返回值个数，循环取得返回值类型、字段名称
        char    szSession[32] = {0};
        char    szType[16] = {0};
        char    szName[128] = {0};
        string  strReturn;
        char    szTmp[1024] = {0};
        double  lValue = 0;
        char    szValue[1024] = {0};

        for (int j=1; j<=nRets; j++)
        {
            sprintf(szSession, "property%d", j);
            GetRetType(nTplId, szSession, szType); 
            GetRetName(nTplId, szSession, szName);
            if (0 == stricmp("int", szType))
            {
                i>>lValue;
                sprintf(szTmp, "%.0f", lValue);
                strReturn += szName;
                strReturn += "=";
                strReturn += szTmp;
                strReturn += "$";
            }
            else if (0 == stricmp("float", szType))
            {
                i>>lValue;
                strReturn += szName;
                strReturn += "=";
                sprintf(szTmp, "%.2f", lValue);
                strReturn += szTmp;
                strReturn += "$";
            }
            else if (0 == stricmp("string", szType))
            {
                i>>szValue;
                strReturn += szName;
                strReturn += "=";
                sprintf(szTmp, "%s", szValue);
                strReturn += szTmp;
                strReturn += "$";
            }
            else
            {
            }
        }
        
		i.close();
        sprintf(szReturn, "%s", strReturn);
		CString strInput = strReturn.c_str();
		MakeCharByString(szReturn,nSize,strInput);
	}
	catch(otl_exception& p)
	{ // intercept OTL exceptions
//		cerr<<p.msg<<endl; // print out error message
//		cerr<<p.stm_text<<endl; // print out SQL that caused the error
//		cerr<<p.var_info<<endl; // print out the variable that caused the error
		printf("error=%s\r\n%s$", p.msg, p.var_info);
		strLog.Format("OracleGeneral ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
		WriteLog(strLog, 223);
		db.logoff(); // disconnect from ODBC
		CString sReturn;
		sReturn.Format("error=%s:%s$",p.msg,p.var_info);
		sReturn.Replace("\r\n",",");
		sprintf(szReturn,"%s",sReturn);
		return FALSE;
	}
	db.logoff(); // disconnect from ODBC
	
	return TRUE;
}

/****************************************************************************
	Export Function Of DatabaseSourceNames (Fetch DSN)
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL SYSTEMDSN(const char * strParas, char * strReturn, int& nSize)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//PrintLog("SYSTEMDSN Start" , "OracleMonitor.log");
	//const char * strTemp = strParas;
	//while(*strTemp)
	//{
	//	PrintLog(strTemp , "OracleMonitor.log");
	//	strTemp += strlen(strTemp)+1;
	//}

	HKEY hKey = NULL;
	LONG lRet = NULL;	
	lRet = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\ODBC\\ODBC.INI\\ODBC Data Sources", 0, KEY_QUERY_VALUE , &hKey);
	//lRet = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\ODBC\\ODBC.INI\\ODBC Data Sources", 0, KEY_ALL_ACCESS|KEY_WOW64_64KEY , &hKey);

	char *p =strReturn;
	if (lRet == ERROR_SUCCESS) {
		// TODO: 
		DWORD dwIndex = 0;
		while (1) {
			char szValueName[512] = {0};
			DWORD dwSizeValue = sizeof(szValueName) - 1;
			
			char szVal[512] = {0};
			DWORD len = 512;

			//lRet = RegEnumValue(hKey, dwIndex, szValueName, &dwSizeValue, NULL, NULL, NULL, NULL);				
			lRet = RegEnumValue(hKey, dwIndex, szValueName, &dwSizeValue, NULL, NULL, (LPBYTE)szVal, &len);
			if (lRet != ERROR_SUCCESS)
				break;

			if(strstr(szVal, "Oracle") != NULL)
			{
			sprintf(p,"%s=%s",szValueName,szValueName);
			p+= 2* strlen(szValueName)+2;
			}
			//lstTexts.AddTail(szValueName);
			//lstValues.AddTail(szValueName);
			dwIndex++;
		}
		RegCloseKey(hKey);
	}

	//char *strEnd = strReturn;
	//while(*strEnd)
	//{
	//	PrintLog(strEnd , "OracleMonitor.log");
	//	strEnd += strlen(strEnd)+1;
	//}
	//PrintLog("SYSTEMDSN End" , "OracleMonitor.log");
	return TRUE;
	
}

BOOL SYSTEMDSN64(const char * strParas, char * strReturn, int& nSize, char *strFlag )
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HKEY hKey = NULL;
	LONG lRet = NULL;	

	lRet = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\ODBC\\ODBC.INI\\ODBC Data Sources", 0, KEY_ALL_ACCESS|KEY_WOW64_64KEY , &hKey);

	char *p =strReturn;

	if (lRet == ERROR_SUCCESS)
	{
		// TODO: 
		DWORD dwIndex = 0;
		while (1)
		{
			char szValueName[512] = {0};
			DWORD dwSizeValue = sizeof(szValueName) - 1;

			char szVal[512] = {0};
			DWORD len = 512;

			lRet = RegEnumValue(hKey, dwIndex, szValueName, &dwSizeValue, NULL, NULL, (LPBYTE)szVal, &len);

			if (lRet != ERROR_SUCCESS)
				break;


			if(strstr(szVal, "Oracle")==NULL)
			{			
				sprintf(p,"%s=%s",szValueName,szValueName);
				p+= 2* strlen(szValueName)+2;

			}
			dwIndex++;
		}
		RegCloseKey(hKey);
	}
	return TRUE;
}


extern "C" __declspec(dllexport) 
BOOL ODBCTest(const char *inFileName,const char *outFileName)
{
	FILE *stream;
    char chTime[250] = {0};
    //打开文件

	strcpy(szoutFile,outFileName);

	stream=fopen(inFileName,"r");

//	WriteResultFile(inFileName,outFileName,0);

	CString strTemp,aa;
	CStringList paramlist,lsValue,lsText;
	while(fgets(chTime,249,stream)!=NULL)
	{
		strTemp=chTime;
		strTemp.Replace("\n","");
		aa+=strTemp;
		paramlist.AddHead(strTemp);
	//	WriteResultFile(chTime,outFileName,1);
	}
	
	char szReturn[2014]={0};
	//GetOracleInfo(paramlist,szReturn);
	//OracleTableName(paramlist,);
	
	WriteResultFile(FuncGetStringFromIDS("SV_ORACLE","SHOWTABLENAME"),szoutFile,1);
	POSITION pos;
	pos =lsText.GetHeadPosition();
	while(pos!=NULL)
	{
		strTemp=lsText.GetNext(pos);
		WriteResultFile(strTemp,outFileName,1);
		
	}

	WriteResultFile(szReturn,outFileName,1);
	
	return TRUE;
}

void WriteResultFile(const char* chMsg, const char* filename, int nFlag)
{
	
    FILE *stream;
    char chTime[50] = {0};
    //打开文件
    if (nFlag == 0)
        stream=fopen(filename,"w");
    else if(nFlag ==1)
        stream=fopen(filename,"a");
	fputs(chMsg,stream);
    fputs("\n",stream);
	fclose(stream);
	
}


//BOOL DBrlogon( CString strConn, otl_connect* pdb,char * szReturn)
BOOL DBrlogon( CString strConn, otl_connect &pdb,char * szReturn)
{
	
	int i =0;
	try
	{
//LOOP:
		i++;
		int nBegin = GetTickCount();
//		pdb->rlogon(strConn.GetBuffer(1));
		pdb.rlogon(strConn.GetBuffer(strConn.GetLength()));
		int nEnd = GetTickCount();
		sprintf(szReturn, "status=OK$time=%d", nEnd-nBegin);
		
	}
	catch(otl_exception& p)
	{ 
		/*
		if(i<2) {
			//Sleep(5000);
			//goto LOOP;
		}*/
		sprintf(szReturn, "error=%s\r\n%s$", p.msg, p.var_info);
		//strLog.Format("TestDBConnect ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
		pdb.logoff();
//		pdb->logoff(); // disconnect from ODBC
		return FALSE;
	}
	return TRUE;
}

//共享内存监测器，监测指标：共享内存命中率、共享内存使用率
extern "C" __declspec(dllexport)
BOOL GetSharedMemoryInfo(const char * strParas , char * strReturn , int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet = FALSE;
	char * strMonitorId = GetParaValue(strParas,"_MonitorID=");

	CString strUser = _T("");
	CString strPwd = _T("");
	CString strService = _T("");
	CString strMonitorID = _T("");

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	
	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTmp = _T("");
		strTmp = paramList.GetNext(pos);
		if (0 == strTmp.Find(__USERACCOUNT__))
		{
			strUser = strTmp.Right(strTmp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if (0 == strTmp.Find(__PASSWORD__))
		{
			strPwd = strTmp.Right(strTmp.GetLength() - strlen(__PASSWORD__));
		}
		else if (0 == strTmp.Find(__ServiceName__))
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen(__ServiceName__));
		}
		else if(strTmp.Find(__MONITORID__) == 0) 
		{
			strMonitorID=strTmp.Right(strTmp.GetLength() - strlen(__MONITORID__));			
		}
		else if(strTmp.Find("_ServiceName=") == 0)
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen("_ServiceName="));
		}
	}
	if (strService.IsEmpty())
	{
		sprintf(strReturn, "error=%s", FuncGetStringFromIDS("SV_ORACLE",
            "ORACLE_SERVERNAME_NOT_FOUND"));
		return FALSE;
	}

	otl_connect db;
	CString strConn = _T("");

	otl_conn::initialize();
	strConn.Format("UID=%s;PWD=%s;DSN=%s", strUser, strPwd, strService);
	try
	{
		db.rlogon(strConn.GetBuffer(1));
		CString strSQL = _T("");
		CString strSQLFilePath = _T("");		
//修改共享池的大小，ALTER SYSTEM SET SHARED_POOL_SIZE = 64M;查看共享SQL区的使用率：select(sum(pins-reloads))/sum(pins)*100 "Library_Cache" from v$librarycache;
		strSQL.Format("%s" , "select sum(pinhits)/sum(pins)*100 \"hit_radio\" from v$librarycache");
		otl_stream i(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		float hit_Radio;
		i>>hit_Radio;			//获取到Oracle共享内存命中率
		cout<<"hit_Radio="<<hit_Radio<<endl;
		i.clean();

//共享内存使用率select Sum(Pins)/(Sum(Pins) + Sum(Reloads)) * 100 "Library_Cache_Hit_Ratio" from V$LibraryCache
		strSQL.Empty();
		strSQL="select(sum(pins-reloads))/sum(pins)*100 \"Library_Cache\" from v$librarycache";
		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		float Library_Cache;
		i>>Library_Cache;
		cout<<"Library_Cache="<<Library_Cache<<endl;
		i.clean();

/******************************************************************************************************
数据字典缓存命中率：
数据字典缓存命中率显示了对数据字典和其他对象的内存读操作所占的百分比。
select (1-(sum(getmisses)/sum(gets))) * 100 "Data_Cache_Hit_Ratio" from v$rowcache; 
大于98%为最佳

库缓存命中率：
库缓存命中率显示了对实际语句和PL/SQL对象的内存读操作所占的百分比。注意，很高的命中率并不总是一件好事。
select Sum(Pins)/(Sum(Pins) + Sum(Reloads)) * 100 "Library_Cache_Hit_Ratio" from V$LibraryCache;
大于98%为最佳
*******************************************************************************************************/
		strSQL.Empty();
		strSQL="select (1-(sum(getmisses)/sum(gets))) * 100 \"Data_Cache_Hit_Ratio\" from v$rowcache";
		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		float DataDictionaryHitRate;
		i>>DataDictionaryHitRate;
		cout<<"DataDictionaryHitRate="<<DataDictionaryHitRate<<endl;
		i.clean();

		strSQL.Empty();
		strSQL="select Sum(Pins)/(Sum(Pins) + Sum(Reloads)) * 100 \"Library_Cache_Hit_Ratio\" from V$LibraryCache";
		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		float LibraryCacheMemoryHitRate;
		i>>LibraryCacheMemoryHitRate;
		cout<<"LibraryCacheMemoryHitRate="<<LibraryCacheMemoryHitRate<<endl;
		i.clean();
//读操作命中率
		strSQL.Empty();
		strSQL="select round((1 - sum(decode(name,'physical reads',value,0))/(sum(decode(name,'db block gets',value,0)) + sum(decode(name,'consistent gets',value,0)))),4) *100 || '%' chitrati from v$sysstat";
		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		float ReadHitRate;
		char otlBuf[128] = {0};
		i>>otlBuf;
		ReadHitRate = atof(otlBuf);
		cout<<"ReadHitRate="<<ReadHitRate<<endl;
		i.clean();
//写操作命中率
		strSQL.Empty();
		strSQL="select round((1 - sum(decode(name,'physical writes',value,0))/(sum(decode(name,'db block gets',value,0)) + sum(decode(name,'consistent gets',value,0))) ),4) *100 || '%' chitrati from v$sysstat";
		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		float WriteHitRate;
		i>>otlBuf;
		WriteHitRate=atof(otlBuf);
		cout<<"WriteHitRate"<<WriteHitRate<<endl;
		i.clean();
//数据库运行状态
		strSQL.Empty();
		strSQL="select status  from v$instance";
		i.open(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		char DataBaseStatus[20] = {0};
		i>>DataBaseStatus;
		WriteHitRate=atof(otlBuf);
		cout<<"WriteHitRate"<<WriteHitRate<<endl;
		sprintf(strReturn , "SharedMemoryHitRate=%.2f$SharedMemoryUsedRate=%.2f$DataDictionaryHitRate=%0.2f$LibraryCacheMemoryHitRate=%.2f$ReadHitRate=%.2f$WriteHitRate=%.2f$DataBaseStatus=%s$" ,
			hit_Radio,
			Library_Cache,
			DataDictionaryHitRate,
			LibraryCacheMemoryHitRate,
			ReadHitRate,
			WriteHitRate,
			DataBaseStatus
			);	
		bRet = TRUE;
	}
	catch(otl_exception &e)
	{
		sprintf(strReturn , "error=%s" , e.msg);
		bRet = FALSE;
	}
	catch(...)
	{
		DWORD dwError = ::GetLastError();
		LPTSTR lpBuffer = NULL;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		sprintf(strReturn , "error=Exception id:%d\terror info:%s" , dwError , lpBuffer);
		::LocalFree(lpBuffer);
		bRet = FALSE;
	}
	db.logoff(); 
	char * strEnd = strReturn;
	while(*strEnd)
	{
		if(*strEnd == '$')
			*strEnd = '\0';
		strEnd++;
	}
	return bRet;
}

/*select a.TABLESPACE tablespace,
       a.SIZE_IN_MB,
       b.FREE_IN_MB,
       b.MAX_IN_MB,
       rpad(ROUND((b.FREE_IN_MB / a.SIZE_IN_MB) * 100, 2), 5) FREE_PCT,
       b.FRAG# FRAGS#
  from dual,
       (select tablespace_name TABLESPACE,
               rpad(round(sum(bytes) / 1024 / 1024, 2), 5) SIZE_IN_MB
          from dba_data_files
         group by tablespace_name) a,
       (select tablespace_name TABLESPACE,
               rpad(round(SUM(BYTES) / 1024 / 1024, 2), 5) FREE_IN_MB,
               rpad(round(MAX(BYTES) / 1024 / 1024, 2), 5) MAX_IN_MB,
               count(1) FRAG#
          from dba_free_space
         group by tablespace_name) b
 WHERE a.tablespace = b.tablespace(+)
表空间碎片率
 select tablespace_name,(100-sqrt(max(blocks)/sum(blocks))*(100/sqrt(sqrt(count(blocks))))) FSFI from dba_free_space group by tablespace_name;
 */
extern "C" __declspec(dllexport)
BOOL GetCrackRateInfo(const char * strParas , char * strReturn , int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet = FALSE;
	char * strMonitorId = GetParaValue(strParas,"_MonitorID=");

	CString strUser = _T("");
	CString strPwd = _T("");
	CString strService = _T("");
	CString strMonitorID = _T("");

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	
	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTmp = _T("");
		strTmp = paramList.GetNext(pos);
		if (0 == strTmp.Find(__USERACCOUNT__))
		{
			strUser = strTmp.Right(strTmp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if (0 == strTmp.Find(__PASSWORD__))
		{
			strPwd = strTmp.Right(strTmp.GetLength() - strlen(__PASSWORD__));
		}
		else if (0 == strTmp.Find(__ServiceName__))
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen(__ServiceName__));
		}
		else if(strTmp.Find(__MONITORID__) == 0) 
		{
			strMonitorID=strTmp.Right(strTmp.GetLength() - strlen(__MONITORID__));			
		}
		else if(strTmp.Find("_ServiceName=") == 0)
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen("_ServiceName="));
		}
	}
	if (strService.IsEmpty())
	{
		sprintf(strReturn, "error=%s", FuncGetStringFromIDS("SV_ORACLE",
            "ORACLE_SERVERNAME_NOT_FOUND"));
		return FALSE;
	}

	otl_connect db;
	CString strConn = _T("");

	otl_conn::initialize();
	strConn.Format("UID=%s;PWD=%s;DSN=%s", strUser, strPwd, strService);
	try
	{
		db.rlogon(strConn.GetBuffer(1));
		CString strSQL = _T("");
		char tempbuf[128] = {0};
		string strTemp;
		strSQL.Format("%s" , "select tablespace_name,(100-sqrt(max(blocks)/sum(blocks))*(100/sqrt(sqrt(count(blocks))))) FSFI from dba_free_space group by tablespace_name");
		otl_stream i(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		float fsfi;//CrackRate;
		char tablespace_name[128] = {0};
		while(!i.eof())
		{
			i>>tablespace_name>>fsfi;			//获取到Oracle表空间碎片率
			sprintf(tempbuf , "%s(%c):%.2f," , tablespace_name , '%',fsfi);
			strTemp += tempbuf;
		}
		strTemp.erase(strTemp.length()- 1, strTemp.length());
		sprintf(strReturn , "CrackRate=%s$" , strTemp.c_str());
		bRet = TRUE;
	}
	catch(otl_exception &e)
	{
		sprintf(strReturn , "error=%s" , e.msg);
		bRet = FALSE;
	}
	catch(...)
	{
		DWORD dwError = ::GetLastError();
		LPTSTR lpBuffer = NULL;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		sprintf(strReturn , "error=Exception id:%d\terror info:%s" , dwError , lpBuffer);
		::LocalFree(lpBuffer);
		bRet = FALSE;
	}
	db.logoff(); 
	char * strEnd = strReturn;
	while(*strEnd)
	{
		if(*strEnd == '$')
			*strEnd = '\0';
		strEnd++;
	}
	return bRet;
}

extern "C" __declspec(dllexport)
BOOL GetRollUsedRateInfo(const char * strParas , char * strReturn , int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet = FALSE;
	char * strMonitorId = GetParaValue(strParas,"_MonitorID=");

	CString strUser = _T("");
	CString strPwd = _T("");
	CString strService = _T("");
	CString strMonitorID = _T("");

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	
	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTmp = _T("");
		strTmp = paramList.GetNext(pos);
		if (0 == strTmp.Find(__USERACCOUNT__))
		{
			strUser = strTmp.Right(strTmp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if (0 == strTmp.Find(__PASSWORD__))
		{
			strPwd = strTmp.Right(strTmp.GetLength() - strlen(__PASSWORD__));
		}
		else if (0 == strTmp.Find(__ServiceName__))
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen(__ServiceName__));
		}
		else if(strTmp.Find(__MONITORID__) == 0) 
		{
			strMonitorID=strTmp.Right(strTmp.GetLength() - strlen(__MONITORID__));			
		}
		else if(strTmp.Find("_ServiceName=") == 0)
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen("_ServiceName="));
		}
	}
	if (strService.IsEmpty())
	{
		sprintf(strReturn, "error=%s", FuncGetStringFromIDS("SV_ORACLE",
            "ORACLE_SERVERNAME_NOT_FOUND"));
		return FALSE;
	}

	otl_connect db;
	CString strConn = _T("");

	otl_conn::initialize();
	strConn.Format("UID=%s;PWD=%s;DSN=%s", strUser, strPwd, strService);
	try
	{
		db.rlogon(strConn.GetBuffer(1));
		CString strSQL = _T("");
		char tempbuf[128] = {0};
		string strTemp;
		strSQL.Format("%s" , "select 100*(1-(UNDOB-UNDO*DBS)/UNDOB) as a FROM (select instance_name  from V$instance),(select nvl(sum(undoblks),0) UNDO from v$undostat where begin_time >(select sysdate - UR/(3600*24) from (select value as UR from v$parameter where name='undo_retention'))), (select  value as DBS from v$parameter where name='db_block_size'),(select sum(bytes) as UNDOB,tablespace_name from dba_data_files where tablespace_name=(select upper(value) as UNDO from v$parameter where name='undo_tablespace') group by tablespace_name)");
		otl_stream i(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		float RollUsedRate;	//RollUsedRate;回滚段使用率
		i>>RollUsedRate;
		i.clean();
//长表扫描率
		strSQL.Empty();
		strSQL = "select 100*((select t2.value from v$sysstat t2 where t2.name ='table scans (long tables)')/(select sum(t1.value) from v$sysstat t1 where t1.name in ('table scans (short tables)','table scans (long tables)'))) as \"LongtableScanRate\" from dual";
		i.open(200,
			strSQL.GetBuffer(1),
			db
			);
		float LongtableScanRate = 0.0;
		i>>LongtableScanRate;
		i.clean();
//日志空间请求率
		strSQL.Empty();
        strSQL = "select round(100 * sum(decode(name, 'redo log space requests', value, 0))/sum(decode(name, 'redo entries', value, 0)), 2) \"Log_Space_Request_Rate\" from v$sysstat where name in ('redo log space requests', 'redo entries')";
		i.open(200,
			strSQL.GetBuffer(1),
			db
			);
		float RedoLogSpaceRequestRate;
		i>>RedoLogSpaceRequestRate;
		i.clean();
//察看一下归档日志的空间大小 select * from v$recovery_file_dest. 				
		strSQL.Empty();
        strSQL = "select space_limit \"Total Size\" , space_used \"Used Size\" from v$recovery_file_dest";
		i.open(200,
			strSQL.GetBuffer(1),
			db
			);
		float LogSpaceSize , UsedLogSpaceSize;
		i>>LogSpaceSize>>UsedLogSpaceSize;
		i.clean();

		sprintf(strReturn , "RollUsedRate=%.2f$LongtableScanRate=%.2f$RedoLogSpaceRequestRate=%.2f$LogSpaceUsedRate=%.2f$" , 
			RollUsedRate,
			LongtableScanRate,
			RedoLogSpaceRequestRate,
			UsedLogSpaceSize/LogSpaceSize*100
			);
		bRet = TRUE;
	}
	catch(otl_exception &e)
	{
		sprintf(strReturn , "error=%s" , e.msg);
		bRet = FALSE;
	}
	catch(...)
	{
		DWORD dwError = ::GetLastError();
		LPTSTR lpBuffer = NULL;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		sprintf(strReturn , "error=Exception id:%d\terror info:%s" , dwError , lpBuffer);
		::LocalFree(lpBuffer);
		bRet = FALSE;
	}
	db.logoff(); 
	char * strEnd = strReturn;
	while(*strEnd)
	{
		if(*strEnd == '$')
			*strEnd = '\0';
		strEnd++;
	}
	return bRet;
}

extern "C" __declspec(dllexport)
BOOL GetUsers(const char * strParas , char * strReturn , int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet = FALSE;
	char * strMonitorId = GetParaValue(strParas,"_MonitorID=");

	CString strUser = _T("");
	CString strPwd = _T("");
	CString strService = _T("");
	CString strMonitorID = _T("");

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	
	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTmp = _T("");
		strTmp = paramList.GetNext(pos);
		if (0 == strTmp.Find(__USERACCOUNT__))
		{
			strUser = strTmp.Right(strTmp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if (0 == strTmp.Find(__PASSWORD__))
		{
			strPwd = strTmp.Right(strTmp.GetLength() - strlen(__PASSWORD__));
		}
		else if (0 == strTmp.Find(__ServiceName__))
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen(__ServiceName__));
		}
		else if(strTmp.Find(__MONITORID__) == 0) 
		{
			strMonitorID=strTmp.Right(strTmp.GetLength() - strlen(__MONITORID__));			
		}
		else if(strTmp.Find("_ServiceName=") == 0)
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen("_ServiceName="));
		}
	}
	if (strService.IsEmpty())
	{
		sprintf(strReturn, "error=%s", FuncGetStringFromIDS("SV_ORACLE",
            "ORACLE_SERVERNAME_NOT_FOUND"));
		return FALSE;
	}

	otl_connect db;
	CString strConn = _T("");

	otl_conn::initialize();
	strConn.Format("UID=%s;PWD=%s;DSN=%s", strUser, strPwd, strService);
	try
	{
		db.rlogon(strConn.GetBuffer(1));
		CString strSQL = _T("");
		char tempbuf[128] = {0};
		string strTemp;
		strSQL.Format("%s" , "select distinct(username) from v$session");
		otl_stream i(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		while(!i.eof())
		{
			memset(tempbuf , '\0' , 128);
			i>>tempbuf;
			strTemp += tempbuf;
			strTemp += '=';
			strTemp += tempbuf;
			strTemp += '$';
		}
		i.clean();
		sprintf(strReturn , "%s" , strTemp.c_str());
		bRet = TRUE;
	}
	catch(otl_exception &e)
	{
		sprintf(strReturn , "error=%s" , e.msg);
		bRet = FALSE;
	}
	catch(...)
	{
		DWORD dwError = ::GetLastError();
		LPTSTR lpBuffer = NULL;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		sprintf(strReturn , "error=Exception id:%d\terror info:%s" , dwError , lpBuffer);
		::LocalFree(lpBuffer);
		bRet = FALSE;
	}
	db.logoff(); 
	char * strEnd = strReturn;
	while(*strEnd)
	{
		if(*strEnd == '$')
			*strEnd = '\0';
		strEnd++;
	}
	return bRet;
}

extern "C" __declspec(dllexport)
BOOL GetUsersSessions(const char * strParas , char * strReturn , int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet = FALSE;
	char * strMonitorId = GetParaValue(strParas,"_MonitorID=");

	CString strUser = _T("");
	CString strPwd = _T("");
	CString strService = _T("");
	CString strMonitorID = _T("");
	CString strUserName = _T("");

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	
	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTmp = _T("");
		strTmp = paramList.GetNext(pos);
		if (0 == strTmp.Find(__USERACCOUNT__))
		{
			strUser = strTmp.Right(strTmp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if (0 == strTmp.Find(__PASSWORD__))
		{
			strPwd = strTmp.Right(strTmp.GetLength() - strlen(__PASSWORD__));
		}
		else if (0 == strTmp.Find(__ServiceName__))
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen(__ServiceName__));
		}
		else if(strTmp.Find(__MONITORID__) == 0) 
		{
			strMonitorID=strTmp.Right(strTmp.GetLength() - strlen(__MONITORID__));			
		}
		else if(strTmp.Find("_UserName=") == 0)
		{
			strUserName = strTmp.Right(strTmp.GetLength() - strlen("_UserName="));
		}
		else if(strTmp.Find("_ServiceName=") == 0)
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen("_ServiceName="));
		}
	}
	if (strService.IsEmpty())
	{
		sprintf(strReturn, "error=%s", FuncGetStringFromIDS("SV_ORACLE",
            "ORACLE_SERVERNAME_NOT_FOUND"));
		return FALSE;
	}

	otl_connect db;
	CString strConn = _T("");

	otl_conn::initialize();
	strConn.Format("UID=%s;PWD=%s;DSN=%s", strUser, strPwd, strService);
	try
	{
		db.rlogon(strConn.GetBuffer(1));
		CString strSQL = _T("");
		char tempbuf[128] = {0};
		string strTemp;
		strSQL.Format("select count(*) from v$session where username='%s'" , strUserName.GetBuffer(1));
		otl_stream i(200, // buffer size
              strSQL.GetBuffer(0),
                 // SELECT statement
              db // connect object
             );
		int OracleUserSessionNums;
		i>>OracleUserSessionNums;
		i.clean();
		sprintf(strReturn , "OracleUserSessionNums=%d$" , OracleUserSessionNums);
		bRet = TRUE;
	}
	catch(otl_exception &e)
	{
		sprintf(strReturn , "error=%s" , e.msg);
		bRet = FALSE;
	}
	catch(...)
	{
		DWORD dwError = ::GetLastError();
		LPTSTR lpBuffer = NULL;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		sprintf(strReturn , "error=Exception id:%d\terror info:%s" , dwError , lpBuffer);
		::LocalFree(lpBuffer);
		bRet = FALSE;
	}
	db.logoff(); 
	char * strEnd = strReturn;
	while(*strEnd)
	{
		if(*strEnd == '$')
			*strEnd = '\0';
		strEnd++;
	}
	return bRet;
}
//oracle 死锁监测器 Added by lifeng.wang 2010-4-26
extern "C" __declspec(dllexport)
BOOL LockInfo(const char * strParas , char * strReturn , int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet = FALSE;
	CString strUser = _T("");
	CString strPwd = _T("");
	CString strService = _T("");
	CString strMonitorID = _T("");
	CString strUserName = _T("");

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTmp = _T("");
		strTmp = paramList.GetNext(pos);
		if (0 == strTmp.Find(__USERACCOUNT__))
		{
			strUser = strTmp.Right(strTmp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if (0 == strTmp.Find(__PASSWORD__))
		{
			strPwd = strTmp.Right(strTmp.GetLength() - strlen(__PASSWORD__));
		}
		else if (0 == strTmp.Find(__ServiceName__))
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen(__ServiceName__));
		}
		else if(strTmp.Find(__MONITORID__) == 0) 
		{
			strMonitorID=strTmp.Right(strTmp.GetLength() - strlen(__MONITORID__));			
		}
		else if(strTmp.Find("_UserName=") == 0)
		{
			strUserName = strTmp.Right(strTmp.GetLength() - strlen("_UserName="));
		}
		else if(strTmp.Find("_ServiceName=") == 0)
		{
			strService = strTmp.Right(strTmp.GetLength() - strlen("_ServiceName="));
		}
	}
	if (strService.IsEmpty())
	{
		sprintf(strReturn, "error=%s", FuncGetStringFromIDS("SV_ORACLE",
			"ORACLE_SERVERNAME_NOT_FOUND"));
		return FALSE;
	}

	otl_connect db;
	CString strConn = _T("");

	otl_conn::initialize();
	strConn.Format("UID=%s;PWD=%s;DSN=%s", strUser, strPwd, strService);
	try
	{
		db.rlogon(strConn.GetBuffer(1));
		CString strSQL = _T("");
		char tempbuf[128] = {0};
		string strTemp;
		strSQL.Format("%s", "select * from v$lock where block = 1;");
		otl_stream i(200, // buffer size
			strSQL.GetBuffer(0),
			// SELECT statement
			db // connect object
			);
		int TotalLockNum = 0;		
		map<string, int> LockMap;
		char LockAddr[50] = {0};
		char LockKAddr[50] = {0};
		char LockSid[20] ={0};
		string LockInfo;
		while(!i.eof())
		{
			i>>LockAddr>>LockKAddr>>LockSid;
			if(LockMap.find(LockSid) != LockMap.end())
			{
				LockMap[LockSid] += 1;
			}
			else
			{
				LockMap[LockSid] = 1;
			}
			TotalLockNum++;
		}		
		i.clean();

        if(LockMap.size() == 0)
		{
			LockInfo = "没有死锁";
		}
		else
		{
			map<string, int>::iterator pLockMap = LockMap.begin();
			while(pLockMap != LockMap.end())
			{
				char buf[10] = {0};
				sprintf(buf, " BlockNum:%d," , pLockMap->second);
				LockInfo += pLockMap->first + buf;
				pLockMap++;
			}
		}
		sprintf(strReturn , "LockInfo=%s$TotalLockNum=%d$" ,LockInfo.c_str(), TotalLockNum);
		bRet = TRUE;
	}
	catch(otl_exception &e)
	{
		sprintf(strReturn , "error=%s" , e.msg);
		bRet = FALSE;
	}
	catch(...)
	{
		DWORD dwError = ::GetLastError();
		LPTSTR lpBuffer = NULL;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwError,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		sprintf(strReturn , "error=Exception id:%d\terror info:%s" , dwError , lpBuffer);
		::LocalFree(lpBuffer);
		bRet = FALSE;
	}
	db.logoff(); 
	char * strEnd = strReturn;
	while(*strEnd)
	{
		if(*strEnd == '$')
			*strEnd = '\0';
		strEnd++;
	}
	return bRet;
}


extern "C" __declspec(dllexport) 
BOOL ConnectedOracleMachines(const char * strParas, char * strReturn, int& nSize)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	const char * strReceive = strParas;

	CString		strDBConn = _T(""), 
		strQuery = _T("");

	CString		strDBMatch = _T(""), 
		strDBUser = _T(""), 
		strDBPass = _T(""), 
		strDBDriver = _T(""), 
		strQFile = _T("");
	CString strLog = _T("");

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);


	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{

		CString strTemp = paramList.GetNext(pos);
		
		if(strTemp.Find(__ServiceName__) == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__ServiceName__));
		}
		else if(strTemp.Find(__USERACCOUNT__) == 0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
			
		}
		else if(strTemp.Find(__PASSWORD__) == 0)
		{
			strDBPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find("_ServiceName=") == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen("_ServiceName="));
		}
	}

	strLog.Format("_ServiceName=%s _UserAccount=%s _PassWord=%s", 
		strDBConn, strDBUser, strDBPass);

	if(strDBConn.IsEmpty())
	{
		//IDS_ORACLEPERF_1
		//sprintf(szReturn,"error=%s", FuncGetStringFromIDS("SV_DATABASE",
		//    "DATABASE_NAME_NULL"));
		return FALSE;
	}

	otl_connect db;
	CString strConn = _T("");
	otl_conn::initialize();
	//	strConn.Format("%s/%s@%s", uid, pwd, dbconn);


	CString cReturn = "HostName=";
	strConn.Format("UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;", strDBUser, strDBPass, strDBConn);
	try
	{
		db.rlogon(strConn.GetBuffer(1));
		otl_stream i(100, // buffer size
			"select machine,count(*) from v$session group by machine,username order by  machine;",

			// SELECT statement
			db // connect object
			);
		
		char           szMachineName[100] = {0};
		unsigned  int  oneMachineCount    = 0;
		char           temp[120] = {0};

		while(!i.eof())
		{
			i>>szMachineName>>oneMachineCount;

			if (szMachineName[0]==0)
			{
				strcpy(szMachineName,"UnknowHost");
			}
			sprintf(temp, "%s:%d,", szMachineName,oneMachineCount);
			cReturn += temp;
		}
		i.close();

		cReturn.TrimRight(',');
		cReturn += "$";
	}
	catch(otl_exception& p)
	{ 
		printf("error=%s\r\n%s", p.msg, p.var_info);
		strLog.Format("DB_MachineNAME ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
		db.logoff(); // disconnect from ODBC
		sprintf(strReturn,"%s=%s",p.msg,p.var_info);
	
		return FALSE;
	}
	db.logoff(); // disconnect from ODBC

	strcpy(strReturn,cReturn);
	cout << "strReturn = "<< strReturn << endl;
	CString strOutRet;
	strOutRet =strReturn;
	nSize = 2048;
	MakeCharByString(strReturn,nSize,strOutRet);	 
	
	return TRUE;
}


extern "C" __declspec(dllexport) 
BOOL OracleUserName(const char * strParas, char * strReturn, int& nSize)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	const char * strReceive = strParas;

	CString		strDBConn = _T(""), 
		strQuery = _T("");

	CString		strDBMatch = _T(""), 
		strDBUser = _T(""), 
		strDBPass = _T(""), 
		strDBDriver = _T(""), 
		strQFile = _T("");
	CString strLog = _T("");

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);


	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{

		CString strTemp = paramList.GetNext(pos);

		if(strTemp.Find(__ServiceName__) == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__ServiceName__));
		}
		else if(strTemp.Find(__USERACCOUNT__) == 0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));

		}
		else if(strTemp.Find(__PASSWORD__) == 0)
		{
			strDBPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find("_ServiceName=") == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen("_ServiceName="));
		}
	}

	strLog.Format("_ServiceName=%s _UserAccount=%s _PassWord=%s", 
		strDBConn, strDBUser, strDBPass);

	if(strDBConn.IsEmpty())
	{
		//IDS_ORACLEPERF_1
		//sprintf(szReturn,"error=%s", FuncGetStringFromIDS("SV_DATABASE",
		//    "DATABASE_NAME_NULL"));
		return FALSE;
	}

	otl_connect db;
	CString strConn = _T("");
	otl_conn::initialize();
	//	strConn.Format("%s/%s@%s", uid, pwd, dbconn);

	strConn.Format("UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;", strDBUser, strDBPass, strDBConn);
	try
	{
		db.rlogon(strConn.GetBuffer(1));
		otl_stream i(100, // buffer size
			"select username from dba_users;",
			db // connect object
			);

		char  szUserName[100] = {0};
		char   *p = strReturn;

		while(!i.eof())
		{

			i>>szUserName;

			while(!i.eof())
			{
				i>>szUserName;

				cout << "szUserName = " << szUserName << endl;
				sprintf(p,"%s=%s",szUserName,szUserName);
				p+= 2* strlen(szUserName)+2;
			}
		}
		i.close();

		//cout << "p = " << p << endl;
	}
	catch(otl_exception& p)
	{ 
		printf("error=%s\r\n%s", p.msg, p.var_info);
		strLog.Format("DB_MachineNAME ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
		db.logoff(); // disconnect from ODBC
		sprintf(strReturn,"%s=%s",p.msg,p.var_info);

		return FALSE;
	}
	db.logoff(); // disconnect from ODBC
 
	return TRUE;
}

extern "C" __declspec(dllexport) 
BOOL OracleUserPrivis(const char * strParas, char * szReturn, int& nSize)
//(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strDBConn = _T(""), 
				strDBUser = _T(""), 
				strDBPass = _T(""), 
				strUserName= _T(""), 
				strLog    = _T(""),
				cReturn   = "UserPrivis=";

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__ServiceName__) == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__ServiceName__));
		}

		else if(strTemp.Find(__USERACCOUNT__) == 0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__) == 0)
		{
			strDBPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find("_OracleUserName=") == 0) 
		{
			strUserName=strTemp.Right(strTemp.GetLength() - strlen("_OracleUserName="));			
		}
		else if(strTemp.Find("_ServiceName=") == 0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen("_ServiceName="));
		}
	}

	if(strDBConn.IsEmpty()) 
	{
		sprintf(szReturn,"error=%s", FuncGetStringFromIDS("SV_DATABASE",
			"DATABASE_NAME_NULL"));
		return FALSE;
	}

	otl_connect db;
	CString strConn = _T("");

	otl_conn::initialize();
	strConn.Format("UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;PLSQLRSet=1;", strDBUser, strDBPass, strDBConn);

	try
	{
		db.rlogon(strConn.GetBuffer(1));
		
		CString strSQL = _T("");
		strSQL.Format("select privilege from dba_sys_privs where grantee='%s' union  select privilege from dba_sys_privs where grantee in (select granted_role from dba_role_privs where grantee='%s' );",strUserName,strUserName);

		printf(" strSQL = %s\n", strSQL.GetBuffer(strSQL.GetLength()));
		strSQL.Replace("\t", NULL);
		printf("strSQL.Replace strSQL = %s\n", strSQL.GetBuffer(strSQL.GetLength()));
		
		otl_stream i(200, 
			strSQL.GetBuffer(0),
			db 
			);

		char           szUserPrivis[100] = {0};
		char           temp[50] = {0};

		while(!i.eof())
		{
			i>>szUserPrivis;

			cout << "szUserPrivis = " << szUserPrivis << endl;

			sprintf(temp, "%s,", szUserPrivis);
			cReturn += temp;
		}

		cReturn.TrimRight(',');
		if (cReturn.GetLength()>1000)
		{
			cReturn = cReturn.Left(1000);
			cReturn += "...";
		}
		cReturn += "$";
		i.close();
	}
	catch(otl_exception& p)
	{
		printf("error=%s\r\n%s$", p.msg, p.var_info);
		strLog.Format("OracleDB ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
		db.logoff(); // disconnect from ODBC
		CString sReturn;
		sReturn.Format("error=%s:%s$",p.msg,p.var_info);
		sReturn.Replace("\r\n",",");
		sprintf(szReturn,"%s",sReturn);
		db.logoff();
		return FALSE;
	}
	catch(...)
	{
		sprintf(szReturn,"error=error occured,error number is %d",::GetLastError());
		return FALSE;
	}
	db.logoff(); // disconnect from ODBC

	strcpy(szReturn,cReturn);
	cout << "szReturn = "<< szReturn << endl;
	CString strOutRet;
	strOutRet =szReturn;
	nSize = 2048;
	MakeCharByString(szReturn,nSize,strOutRet);	 

	return TRUE;
}
