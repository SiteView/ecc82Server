// MySQLMonitor.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "MySQLMonitor.h"
#include "..\..\base\funcGeneral.h"
#include <mysql.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
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
// CMySQLMonitorApp

BEGIN_MESSAGE_MAP(CMySQLMonitorApp, CWinApp)
	//{{AFX_MSG_MAP(CMySQLMonitorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySQLMonitorApp construction

CMySQLMonitorApp::CMySQLMonitorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMySQLMonitorApp object

CMySQLMonitorApp theApp;


extern "C" __declspec(dllexport)
BOOL MySQL(const char * strParas, char * szReturn, int& nSize)
//(CStringList &paramList, char *szReturn)
{
    BOOL bRet = TRUE;
    CString strHost = _T(""), //MySqL Server
        //strDBName = _T("mysql"),//Database Name,default mysql
        strUser = _T(""), //UserName
        strPwd = _T("");// Password
    int nPort = 3306;//MySQL Server Port,default 3306
/////////////////////////////////////////////////////////////////////////////
    const  char   *pszT;//Temp string
    MYSQL		* myData ;//Access Mysql objcect
/////////////////////////////////////////////////////////////////////////////
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

    POSITION pos = paramList.GetHeadPosition();
    while(pos)
    {
        CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{//Get Host
			strHost = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{//Get Port
			nPort = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PORT__)));
		}
        else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{//Get Username
			strUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
        else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{//Get password
			strPwd = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
        /*else if(strTemp.Find(__DBNAME__, 0) == 0)
		{//Get database name
			strDBName = strTemp.Right(strTemp.GetLength() - strlen(__DBNAME__));
		}*/
    }

    if(strHost.IsEmpty())
    {//host is empty
        sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_MYSQL",
            "MYSQL_SERVER_HOST_IS_NULL"));
        return FALSE;
    }
    if(nPort <= 0)
    {//Invalid port
        sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_MYSQL",
            "MYSQL_SERVER_PORT_IS_BAD"));
        return FALSE;
    }

    if((myData = mysql_init((MYSQL*) 0)) && 
       mysql_real_connect( myData, strHost, strUser, strPwd, NULL, nPort,
			   NULL, 0 ) )
    {
        /*if ( mysql_select_db( myData, strDBName ) < 0 ) 
        {
            sprintf(szReturn, "error=%s(%s)$", FuncGetStringFromIDS("SV_MYSQL",
                "MYSQL_SELECT_DATABASE_FAILED"), strDBName);
	        //printf( "Can't select the %s database !\n", strDBName) ;
	        mysql_close( myData ) ;
	        return FALSE;
        }*/
        pszT = mysql_stat( myData ) ;
        if(pszT)
        {
            Parser(pszT, strHost.GetBuffer(strHost.GetLength()), szReturn);
			CString strInput ;
			strInput =szReturn;
			MakeCharByString(szReturn,nSize,strInput);
				

        }
        else
        {
            sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_MYSQL",
                "MYSQL_QUERY_STAT_FAILED"));
            mysql_close( myData ) ;
            return FALSE;
        }
    }
    else 
    {
        sprintf(szReturn, "error=%s(%s:%d)$", FuncGetStringFromIDS("SV_MYSQL",
            "MYSQL_CONNECT_DATABASE_FAILED"), strHost, nPort);
        //printf( "Can't connect to the mysql server on port %d !\n",
	    //    MYSQL_PORT ) ;
		fprintf(stderr, "Failed to connect to database: Error: %s\n",
          mysql_error(myData));

        mysql_close( myData ) ;
        return FALSE;
    }
    mysql_close( myData ) ;
    return TRUE;
}

void Parser(const char* szResult, const char *chHost, char *szReturn)
{
    int nThread = 0, nQueryCount = 0, nOldQueryCount = 0,
        nSlowQuery = 0, nOpenTable = 0;
    char *ca = strstr(szResult, "Threads: ");
    if(ca)
    {
        sscanf(ca + 9, "%d", &nThread);
    }
    ca = strstr(szResult, "Questions: ");
    if(ca)
    {
        sscanf(ca + 11, "%d", &nQueryCount);        
    }
    ca = strstr(szResult, "Slow queries: ");
    if(ca)
    {
        sscanf(ca + 14, "%d", &nSlowQuery);        
    }
    ca = strstr(szResult, "Open tables: ");
    if(ca)
    {
        sscanf(ca + 13, "%d", &nOpenTable);
    }

    char chTempFile[MAX_BUFF_LEN] = {0};
    char chValue[MAX_BUFF_LEN] = {0};
#if _DEBUG
    sprintf(chTempFile, "%s\\temp\\mysql_monitor.ini", FuncGetInstallRootPath());
#else
    sprintf(chTempFile, "%s\\monitormanager\\temp\\mysql_monitor.ini", 
        FuncGetInstallRootPath());
#endif

    nOldQueryCount = GetPrivateProfileInt(chHost, "Questions", 0, chTempFile);
    sprintf(chValue, "%d", nQueryCount);
    WritePrivateProfileString(chHost, "Questions", chValue, chTempFile);

    sprintf(szReturn, "thread=%d$querycount=%d$slowquery=%d$opentable=%d$",
        nThread, nQueryCount-nOldQueryCount, nSlowQuery, nOpenTable);
}