// MySQLMonitor.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "MySQLMonitor.h"
#include <Base/funcGeneral.h>
//#include "..\..\base\funcGeneral.h"
#include <mysql.h>
#include <fstream>
#include <map>
#include<stdio.h> 
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#import "progid:WbemScripting.SWbemLocator" named_guids

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

int PrintLog(const char * strReceive)
{
	char timebuf[128]={0},datebuf[128]={0},tempbuf[1024*10]={0};
	_strtime(timebuf);
	_strdate(datebuf);
	sprintf(tempbuf,"%s-%s",datebuf,timebuf);
	ofstream filestream;
	filestream.open("MySQLMonitor.log",ios::app);
	filestream<<tempbuf<<"\t"<<strReceive<<endl;
	filestream.close();
	return 0;
}
extern "C" __declspec(dllexport)
BOOL MySqlMem(const char * strParas, char * szReturn, int& nSize)
{
	CString strReturn = "",
			strHost = _T(""), 
			strUser = _T(""), //UserName
			strPwd = _T("");// Password

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

    POSITION pos = paramList.GetHeadPosition();
	while(pos)
    {
        CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{//Get Host
			strHost = strTemp.Right(strTemp.GetLength() - (int)strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__WINUSERACCOUNT__, 0) == 0)
		{//Get Username
			strUser = strTemp.Right(strTemp.GetLength() - (int)strlen(__WINUSERACCOUNT__));
		}
        else if(strTemp.Find(__WINPASSWORD__, 0) == 0)
		{//Get password
			strPwd = strTemp.Right(strTemp.GetLength() - (int)strlen(__WINPASSWORD__));
		}
    }

	//ofstream fout("mysqltest.txt",ios::app);
	//fout << strHost<< ","<<strUser<< ","<<strPwd<< ","<<"\r\n"; 
	//fout << flush;
	//fout.close(); 
	CoInitialize(NULL);

	try 
	{ 
		WbemScripting::ISWbemLocatorPtr locator; 
		locator.CreateInstance(WbemScripting::CLSID_SWbemLocator);
		
		if (locator != NULL) 
		{ 
			WbemScripting::ISWbemServicesPtr services;
			if(IsLocalHost(strHost))
				services = locator->ConnectServer(".","root\\cimv2","","","","",0,NULL);
			else
				services = locator->ConnectServer((_bstr_t)strHost,"root\\cimv2",(_bstr_t)strUser,(_bstr_t)strPwd,"","",0,NULL);

			WbemScripting::ISWbemObjectSetPtr objects = services->InstancesOf("Win32_LogicalMemoryConfiguration", 0x10, NULL);
			IEnumVARIANTPtr obj_enum = objects->Get_NewEnum(); 
			ULONG fetched; 
			VARIANT var; 
			int nTotalPhyMem, nTotalVirMem;
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("TotalPhysicalMemory",0);
				_variant_t value = prop->GetValue();
				nTotalPhyMem = (int)value;
				prop = properties->Item("TotalVirtualMemory",0);
				value = prop->GetValue();
				nTotalVirMem = (int)value;
			} 

			_bstr_t com; 
			com="SELECT * FROM Win32_PerfRawData_PerfProc_Process WHERE NAME='mysqld-nt' or NAME='mysqld'"; 
			objects = services->ExecQuery(com,"WQL",0x10,NULL); 
			obj_enum = objects->Get_NewEnum(); 
			while (obj_enum->Next(1,&var,&fetched) == S_OK) 
			{ 
				WbemScripting::ISWbemObjectPtr object = var;
				WbemScripting::ISWbemPropertySetPtr properties = object->Properties_;
				WbemScripting::ISWbemPropertyPtr prop = properties->Item("WorkingSet",0);
				_variant_t value = prop->GetValue();
				int nPhyMem = (int)value/1024;
				prop = properties->Item("PrivateBytes",0);
				value = prop->GetValue();
				int nVirMem = (int)value/1024;
				float fMySqlPhyMemUsage, fMySqlVirMemUsage;
				fMySqlPhyMemUsage = (float)nPhyMem/(float)nTotalPhyMem*100;
				fMySqlVirMemUsage = (float)nVirMem/(float)nTotalVirMem*100;
				strReturn.Format("MySqlPhyMem=%d$MySqlVirMem=%d$MySqlPhyMemUsage=%.2f$MySqlVirMemUsage=%.2f$TotalPhyMem=%d$TotalVirMem=%d$", 
					nPhyMem, nVirMem, fMySqlPhyMemUsage, fMySqlVirMemUsage, nTotalPhyMem, nTotalVirMem);
			} 
			if(strReturn == "")
			{
				sprintf(szReturn, "error=MySql is not run on this server!");
				return FALSE;
			}
		} 
	} 
	catch (_com_error err) 
	{ 
		char buf[200] = {0};
		IErrorInfo * ei = err.ErrorInfo();
		BSTR strDesEI;
		ei->GetDescription(&strDesEI);
		sprintf(szReturn, "error=Error ocured:%x: %s", (unsigned)err.Error(),_com_util::ConvertBSTRToString(strDesEI));
		return FALSE;
	} 
	catch(...) 
	{ 
		sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
		return FALSE;
	} 
	
	strcpy(szReturn,strReturn);
	CString strOutRet;
	strOutRet =szReturn;
	nSize = 2048;
	MakeCharByString(szReturn,nSize,strOutRet);	 
	
	CoUninitialize();

	return TRUE;
}

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
			strHost = strTemp.Right(strTemp.GetLength() - (int)strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{//Get Port
			nPort = atoi(strTemp.Right(strTemp.GetLength() - (int)strlen(__PORT__)));
		}
        else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{//Get Username
			strUser = strTemp.Right(strTemp.GetLength() - (int)strlen(__USERACCOUNT__));
		}
        else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{//Get password
			strPwd = strTemp.Right(strTemp.GetLength() - (int)strlen(__PASSWORD__));
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
	printf("----------%s--%s--%s--------\n",strHost,strUser,strPwd);

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

        pszT = mysql_stat( myData ) ;		//mysql_stat(MYSQL * myData)取得目录数据库系统状况

        if(pszT)
        {
            Parser(pszT, strHost.GetBuffer(strHost.GetLength()), szReturn);
//			mysql_free_result(pszT);
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
/********************************************************
			MySQL进程监测器ProcessInfo
********************************************************/
extern "C" __declspec(dllexport)
BOOL ProInfo(const char * strParas , char * szReturn , int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	bool bRet = false;
	CString strHost = _T("");
	CString strUserName = _T("");
	CString strPWD = _T("");
	int nProID=0;		//保存传进来的进程ID号
	int nPort = 3306;	//MySQL的默认端口号是3306

	MYSQL_RES * res;	//MYSQL数据库的记录集指针
	MYSQL_ROW row;		
	MYSQL	*mysql;	//接收MySQL数据的对象
	CStringList paramList;
	MakeStringListByChar(paramList, strParas);
	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0)==0)
		{
			strHost = strTemp.Right(strTemp.GetLength() - (int)strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0)==0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - (int)strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__ , 0)==0)
		{
			strUserName = strTemp.Right(strTemp.GetLength() - (int)strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__ , 0)==0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - (int)strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROID__, 0)==0)
		{
			nProID = atoi(strTemp.Right(strTemp.GetLength() - (int)strlen(__PROID__)));
		}
	}

	if(strHost.IsEmpty())
	{
		sprintf(szReturn,"error = %s",FuncGetStringFromIDS("SV_MYSQL","MYSQL_SERVER_HOST_IS_NULL"));
		return false;
	}
	if(nPort<=0)
	{
		sprintf(szReturn,"error = %s",FuncGetStringFromIDS("SV_MYSQL","MYSQL_SERVER_PORT_IS_BAD"));
		return false;
	}
	unsigned int i=0;
	CString strTMP = _T("");
	//////////////////////////////////////////////////

	int nID = 0;						//MYSQL线程标识，你要kill一个语句的时候很有用
	CString strDBUser = _T("");			//user列，显示当前用户，如果不是root，这个命令就只显示你权限范围内的sql语句
	CString strDBHost = _T("");			//host列，显示这个语句是从哪个ip的哪个端口上发出的
	CString strDatabase = _T("");		//db列，显示这个进程目前连接的是哪个数据库
	CString strCommand = _T("");		//command列，显示当前连接的执行的命令，一般就是休眠（sleep），查询（query），连接（connect）。
	int nTime = 0;						//time列，此这个状态持续的时间，单位是秒
	CString strState = _T("");			//state列，显示使用当前连接的sql语句的状态
	CString strInfo = _T("");			//info列，显示这个sql语句，因为长度有限，所以长的sql语句就显示不全
	unsigned long nCurrentID = 0;		//当前线程ID

	int nTotalConn=0;					//连接总数
	map<CString,int> hostMap;
	map<CString,int>::const_iterator ptrHost;
	typedef pair <CString,int> hostPair;

	//////////////////////////////////////////////////

	if((mysql = mysql_init((MYSQL   *)NULL)) && mysql_real_connect(mysql,strHost, strUserName, strPWD, NULL, nPort,  NULL, 0))
	{
//		res = mysql_list_processes(mysql);
		CString strCommand=_T("");
		mysql_query(mysql,"show processlist");
		res = mysql_store_result(mysql);
		nCurrentID = mysql_thread_id(mysql);	//获取当前线程ID
		strCommand.Format("kill %d",nCurrentID);
		while(row = mysql_fetch_row(res))
		{	
			nID = atoi(row[0]);
			strDBUser = row[1];
			strDBHost = row[2];
			strDatabase = row[3];
			strCommand = row[4];
			nTime = atoi(row[5]);
			strState = row[6];
			strInfo = row[7];
			strDBHost=strDBHost.Left(strDBHost.Find(':'));		//擦掉获得的主机端口号
			ptrHost = hostMap.find(strDBHost.GetBuffer(strDBHost.GetLength()));

			if(nID!=nCurrentID)
			{
				if(ptrHost==hostMap.end())
				{
					hostMap.insert(hostPair(strDBHost.GetBuffer(strDBHost.GetLength()),1));
				}																						//用MAP保存主机名，并且将主机出现次数统计在键值中
				else
				{
					hostMap[strDBHost.GetBuffer(strDBHost.GetLength())]++;
				}
			}
/*
			else
			{
				PrintLog(row[0]);
				PrintLog(row[2]);
			}
*/			
			nTotalConn++;
		}

		nTotalConn=nTotalConn-1;		//减去自身的连接
		for(ptrHost=hostMap.begin();ptrHost!=hostMap.end();ptrHost++)
		{
			strTMP.Format("%s%s的连接数是%d,",strTMP.GetBuffer(strTMP.GetLength()),ptrHost->first,ptrHost->second);
		}
		strTMP.Delete(strTMP.GetLength()-1,1);			//去除返回值后多余的一个“,”
		sprintf(szReturn,"nTotalConn=%d$result=%s",nTotalConn,strTMP.GetBuffer(strTMP.GetLength()));

		CString strInput ;
		strInput =szReturn;
		MakeCharByString(szReturn,nSize,strInput);
		mysql_free_result(res);				//释放结果集使用的内存。
		mysql_close(mysql);					// 关闭服务器连接。
		bRet = TRUE;

	}
	else
	{
		sprintf(szReturn, "error=%s(%s:%d)$", FuncGetStringFromIDS("SV_MYSQL",
            "MYSQL_CONNECT_DATABASE_FAILED"), strHost, nPort);
		fprintf(stderr, "Failed to connect to database: Error: %s\n",
          mysql_error(mysql));

        mysql_close( mysql ) ;
        return FALSE;
	}

/*
	const char * strEnd=szReturn;
	while(*strEnd)
	{
		PrintLog(strEnd);
		strEnd+=strlen(strEnd)+1;
	}
*/
	return bRet;	
}

extern "C" __declspec(dllexport)
BOOL ProList(const char * strParas , char * szReturn , int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	bool bRet = false;
	CString strHost = _T("");
	CString strUserName = _T("");
	CString strPWD = _T("");
	int nPort = 3306;	//MySQL的默认端口号是3306

	MYSQL_RES * res;	//MYSQL数据库的记录集指针
	MYSQL_ROW row;		
	MYSQL	*mysql;	//接收MySQL数据的对象
	CStringList paramList;
	MakeStringListByChar(paramList, strParas);
	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0)==0)
		{
			strHost = strTemp.Right(strTemp.GetLength() - (int)strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0)==0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - (int)strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__ , 0)==0)
		{
			strUserName = strTemp.Right(strTemp.GetLength() - (int)strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__ , 0)==0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - (int)strlen(__PASSWORD__));
		}
	}

	if(strHost.IsEmpty())
	{
		sprintf(szReturn,"error = %s",FuncGetStringFromIDS("SV_MYSQL","MYSQL_SERVER_HOST_IS_NULL"));
		return false;
	}
	if(nPort<=0)
	{
		sprintf(szReturn,"error = %s",FuncGetStringFromIDS("SV_MYSQL","MYSQL_SERVER_PORT_IS_BAD"));
		return false;
	}
	unsigned int i=0;
	CString strTMP = _T("");
	//////////////////////////////////////////////////

	int nID =0;							//MYSQL线程标识，你要kill一个语句的时候很有用
	unsigned long nCurrentID = 0;

	//////////////////////////////////////////////////



	if((mysql = mysql_init((MYSQL   *)NULL)) && mysql_real_connect(mysql,strHost, strUserName, strPWD, NULL, nPort,  NULL, 0))
	{
		res = mysql_list_processes(mysql);
		nCurrentID = mysql_thread_id(mysql);    //获取当前线程ID号。
		while(row = mysql_fetch_row(res))
		{
			
			nID = atoi(row[0]);
			if(nID==nCurrentID)
			{
				break;
			}

			sprintf(szReturn,"%d=%d$",nID,nID);
			strTMP+=szReturn;
			
		}
		sprintf(szReturn,"%s$",strTMP.GetBuffer(1));
		CString strInput ;
		strInput =szReturn;
		MakeCharByString(szReturn,nSize,strInput);

		mysql_free_result(res);		//关闭查询的结果集
		mysql_close(mysql);

		bRet = TRUE;
	}
	else
	{
		sprintf(szReturn, "error=%s(%s:%d)$", FuncGetStringFromIDS("SV_MYSQL",
            "MYSQL_CONNECT_DATABASE_FAILED"), strHost, nPort);
		fprintf(stderr, "Failed to connect to database: Error: %s\n",
          mysql_error(mysql));

        mysql_close( mysql ) ;
        return FALSE;
	}
/*
	const char * strEnd = szReturn;
	while(*strEnd)
	{
		PrintLog(strEnd);
		strEnd+=strlen(strEnd)+1;
	}
*/
	return TRUE;	
}

/********************************************************
			MySQL从服务器同步状态监测器SlaveStatus
/********************************************************/

extern "C" __declspec(dllexport)
BOOL SlaveStatus(const char * strParas , char * szReturn, int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;
	CString strHost = _T("");
	CString strUserName = _T("");
	CString strPWD = _T("");
	int nPort = 3306;

	MYSQL_RES * res;	//MYSQL数据库的记录集指针
	MYSQL_ROW row;		
	MYSQL	*mysql;	//接收MySQL数据的对象
	CStringList paramList;
	MakeStringListByChar(paramList, strParas);
	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0)==0)
		{
			strHost = strTemp.Right(strTemp.GetLength() - (int)strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0)==0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - (int)strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__ , 0)==0)
		{
			strUserName = strTemp.Right(strTemp.GetLength() - (int)strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__ , 0)==0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - (int)strlen(__PASSWORD__));
		}
	}

	if(strHost.IsEmpty())
	{
		sprintf(szReturn,"error=%s",FuncGetStringFromIDS("SV_MYSQL","MYSQL_SERVER_HOST_IS_NULL"));
		return false;
	}
	if(nPort<=0)
	{
		sprintf(szReturn,"error=%s",FuncGetStringFromIDS("SV_MYSQL","MYSQL_SERVER_PORT_IS_BAD"));
		return false;
	}
	unsigned int i=0;
	CString strTMP = _T("");
	//////////////////////////////////////////////////

	CString strSlaveIOState = _T("");
	CString strMasterHost = _T("");
	CString strMasterUser = _T("");
	CString strSlaveIORunning = _T("");						//Slave是否在运行
	CString strSlaveSQLRunning = _T("");
	int nMasterPort = 3306;
	CString strSecondsBehindMaster = _T("");				//最后一次同步时间

	//////////////////////////////////////////////////


	if((mysql = mysql_init((MYSQL   *)NULL)) && mysql_real_connect(mysql,strHost, strUserName, strPWD, NULL, nPort,  NULL, 0))
	{
		mysql_query(mysql, "show slave status");
		res = mysql_store_result(mysql);

		while(row = mysql_fetch_row(res))
		{
/*			
			PrintLog(row[1]);
			PrintLog(row[2]);
			PrintLog(row[3]);

			for (i=0; i<mysql_num_fields(res); i++)
			{
				PrintLog(row[i]);
			}
*/
			strSlaveIOState = row[0];
			strMasterHost = row[1];
			strMasterUser = row[2];
			nMasterPort = atoi(row[3]);
			strSlaveIORunning =row[10];
			strSlaveSQLRunning = row[11];
			strSecondsBehindMaster = row[32];
			
	
		
		}
		if(strSlaveIOState.IsEmpty())
			strSlaveIOState="null";
		if(strMasterHost.IsEmpty())
			strMasterHost="null";
		if(strMasterUser.IsEmpty())
			strMasterUser="null";
		if(strSlaveIORunning.IsEmpty())
			strSlaveIORunning="null";
		sprintf(szReturn,"SlaveIOState=%s$MasterHost=%s$MasterUser=%s$MasterPort=%d$SlaveIORunning=%s$SlaveSQLRunning=%s$SecondsBehindMaster=%s$",
			strSlaveIOState.GetBuffer(1),
			strMasterHost.GetBuffer(1),
			strMasterUser.GetBuffer(1),
			nMasterPort,
			strSlaveIORunning.GetBuffer(1),
			strSlaveSQLRunning.GetBuffer(1),
			strSecondsBehindMaster.GetBuffer(1));

		CString strInput ;
		strInput =szReturn;
		MakeCharByString(szReturn,nSize,strInput);
				
		mysql_free_result(res);
		bRet = TRUE;
	}
	else
	{
		sprintf(szReturn, "error=%s(%s:%d)$", FuncGetStringFromIDS("SV_MYSQL",
            "MYSQL_CONNECT_DATABASE_FAILED"), strHost, nPort);
		fprintf(stderr, "Failed to connect to database: Error: %s\n",
          mysql_error(mysql));

        mysql_close( mysql ) ;
        return FALSE;
	}

	mysql_close(mysql);
/*
	char * strEnd = szReturn;
	while(*strEnd)
	{
		PrintLog(strEnd);
		strEnd+=strlen(strEnd)+1;
	}
*/
	return bRet;
}

/********************************************************
			MySQL服务器状态监测器Status
/********************************************************/

extern "C" __declspec(dllexport)
BOOL Status(const char * strParas , char * szReturn, int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = false;
	return bRet;
}
/********************************************************
			MySQL服务器内存监测器Memory
/********************************************************/
extern "C" __declspec(dllexport)
BOOL MemoryStatus(const char * strParas , char * szReturn, int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	bool bRet = FALSE;
	CString strHost = _T("");
	CString strUserName = _T("");
	CString strPWD = _T("");
	int nPort = 3306;		//MySQL的默认端口号

	MYSQL * mysql;			//MySQL对象
	MYSQL_RES * res;		//MySQL记录集指针
	MYSQL_ROW row;		//MySQL行指针

	CStringList strList;
	MakeStringListByChar(strList, strParas);
	POSITION pos=strList.GetHeadPosition();
	while(pos)
	{
		CString strTemp=strList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__)==0)
		{
			strHost = strTemp.Right(strTemp.GetLength() - (int)strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__USERACCOUNT__)==0)
		{
			strUserName = strTemp.Right(strTemp.GetLength() - (int)strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__)==0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - (int)strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PORT__)==0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - (int)strlen(__PORT__)));
		}
	}
	if(strHost.IsEmpty())
	{
		sprintf(szReturn,"error=%s$",FuncGetStringFromIDS("SV_MYSQL","MYSQL_SERVER_HOST_IS_NULL"));
		return FALSE;
	}
	if(nPort<=0)
	{
		sprintf(szReturn,"error=%s$",FuncGetStringFromIDS("SV_MYSQL","MYSQL_SERVER_PORT_IS_BAD"));
		return FALSE;
	}
	////////////////////////////////////////////////

	int Innodb_page_size=0;					//页大小（单位：字节）
	int Innodb_buffer_pool_pages_total =0;	//缓冲池总大小（单位：页数）
	int Innodb_buffer_pool_pages_free = 0;	//空页数（单位：页）
	int Innodb_buffer_pool_pages_data = 0;	//包含数据的页（单位：页）
	int Innodb_buffer_pool_pages_dirty =0;	//当前的脏页（单位：页），脏页是指该页包含尚未写磁盘的修改
	int Innodb_buffer_pool_pages_latched = 0;	//在InnoDB缓冲池中锁定的页数。这是当前正读或写或由于其它原因不能清空或删除的页数
	unsigned int nCurrentID=0;

	////////////////////////////////////////////////
	
	if((mysql = mysql_init((MYSQL *)NULL)) && mysql_real_connect(mysql, strHost, strUserName, strPWD, NULL, nPort, NULL, 0))
	{
		mysql_query(mysql,"show status");
		res = mysql_store_result(mysql);

		while(row = mysql_fetch_row(res))
		{
			if(!strcmp(row[0], "Innodb_page_size"))
			{
				Innodb_page_size=atoi(row[1]);
				Innodb_page_size=Innodb_page_size/1024;		//将页的单位从字节转化为Kb
				
			}
			else if(!strcmp(row[0], "Innodb_buffer_pool_pages_total"))
			{
				Innodb_buffer_pool_pages_total = atoi(row[1]);
				
			}
			else if(!strcmp(row[0], "Innodb_buffer_pool_pages_free"))
			{
				Innodb_buffer_pool_pages_free = atoi(row[1]);
			}
			else if(!strcmp(row[0], "Innodb_buffer_pool_pages_data"))
			{
				Innodb_buffer_pool_pages_data = atoi(row[1]);
			}
			else if(!strcmp(row[0], "Innodb_buffer_pool_pages_dirty"))
			{
				Innodb_buffer_pool_pages_dirty = atoi(row[1]);
			}
			else if(!strcmp(row[0], "Innodb_buffer_pool_pages_latched"))
			{
				Innodb_buffer_pool_pages_latched = atoi(row[1]);
			}
			
		}
		sprintf(szReturn,"PageSize=%d$TotalPages=%d$FreePages=%d$DataPages=%d$DirtyPages=%d$LatchedPages=%d$",
			Innodb_page_size,
			Innodb_buffer_pool_pages_total,
			Innodb_buffer_pool_pages_free,
			Innodb_buffer_pool_pages_data,
			Innodb_buffer_pool_pages_dirty,
			Innodb_buffer_pool_pages_latched);
		CString strInput ;
		strInput =szReturn;
		MakeCharByString(szReturn,nSize,strInput);
		mysql_free_result(res);
		bRet = TRUE;


	}
	else
	{
		sprintf(szReturn, "error=%s(%s:%d)$", FuncGetStringFromIDS("SV_MYSQL",
            "MYSQL_CONNECT_DATABASE_FAILED"), strHost, nPort);
		fprintf(stderr, "Failed to connect to database: Error: %s\n",
          mysql_error(mysql));

        mysql_close( mysql ) ;
        return FALSE;
	}
/*
	const char * strEnd = szReturn;
	while(*strEnd)
	{
		PrintLog(strEnd);
		strEnd+=strlen(strEnd)+1;
	}
*/
	mysql_close(mysql);
	return TRUE;
}
/***************************************************
		解析器函数Parser(解析szResult的值)
***************************************************/
void Parser(const char* szResult, const char *chHost, char *szReturn)
{
    int nUptime = 0, nThread = 0, nQueryCount = 0, nQueryCount1m = -1,
        nSlowQuery = 0, nFlushTable = 0, nOpenTable = 0;
	float	fQuery1s = 0;
   
	char *ca = strstr(szResult, "Uptime: ");
    if(ca)
    {
        sscanf(ca + 8, "%d", &nUptime);
    }
	ca = strstr(szResult, "Threads: ");
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
	ca = strstr(szResult, "Flush tables: ");
    if(ca)
    {
        sscanf(ca + 14, "%d", &nFlushTable);
    }
    ca = strstr(szResult, "Open tables: ");
    if(ca)
    {
        sscanf(ca + 13, "%d", &nOpenTable);
    }
	ca = strstr(szResult, "Queries per second avg: ");
    if(ca)
    {
        sscanf(ca + 24, "%f", &fQuery1s);
    }

	sprintf(szReturn, "uptime=%d$thread=%d$querycount=%d$slowquery=%d$flushtable=%d$opentable=%d$querys1s=%.3f$",
			nUptime, nThread, nQueryCount, nSlowQuery, nFlushTable, nOpenTable, fQuery1s);

}

BOOL IsLocalHost(CString sHostName)
{
	sHostName.TrimRight();
	sHostName.MakeLower();
	if(sHostName == "127.0.0.1" || sHostName == "localhost")
		return TRUE;

	WSADATA wsaData;
	char name[155];
	char *ip;
	PHOSTENT hostinfo; 
	if ( WSAStartup( MAKEWORD(2,0), &wsaData ) == 0 ) 
	{ 
		if( gethostname ( name, sizeof(name)) == 0) 
		{ 
			if(sHostName == name)
				return TRUE;
			if((hostinfo = gethostbyname(name)) != NULL) 
			{ //这些就是获得IP的函数
				ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list); 

				if(sHostName == ip)
					return TRUE;
			} 
		} 
		WSACleanup( );
	}
	return FALSE;
}

extern "C" __declspec(dllexport)
BOOL TestMySQLConn(const char * strParas, char * szReturn, int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	BOOL bRet = FALSE;
	CString strHost = _T("");
	CString strUserName = _T("");
	CString strPWD = _T("");
	int nPort = 3306;

//	MYSQL_RES * res;	//MYSQL数据库的记录集指针
//	MYSQL_ROW row;		
	MYSQL	*mysql;	//接收MySQL数据的对象
	CStringList paramList;
	MakeStringListByChar(paramList, strParas);
	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__MACHINENAME__, 0)==0)
		{
			strHost = strTemp.Right(strTemp.GetLength() - (int)strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0)==0)
		{
			nPort = atoi(strTemp.Right(strTemp.GetLength() - (int)strlen(__PORT__)));
		}
		else if(strTemp.Find(__USERACCOUNT__ , 0)==0)
		{
			strUserName = strTemp.Right(strTemp.GetLength() - (int)strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__ , 0)==0)
		{
			strPWD = strTemp.Right(strTemp.GetLength() - (int)strlen(__PASSWORD__));
		}
	}

	if(strHost.IsEmpty())
	{
		sprintf(szReturn,"error=%s",FuncGetStringFromIDS("SV_MYSQL","MYSQL_SERVER_HOST_IS_NULL"));
		return false;
	}
	if(nPort<=0)
	{
		sprintf(szReturn,"error=%s",FuncGetStringFromIDS("SV_MYSQL","MYSQL_SERVER_PORT_IS_BAD"));
		return false;
	}
	unsigned int i=0;
	CString strTMP = _T("");
	//////////////////////////////////////////////////

	int BeginTime=0,EndTime=0;

	//////////////////////////////////////////////////


	try
	{
		BeginTime=::GetTickCount();
		if((mysql = mysql_init((MYSQL   *)NULL)) && mysql_real_connect(mysql,strHost, strUserName, strPWD, NULL, nPort,  NULL, 0))
		{			
			bRet = TRUE;
		}
		else
		{
			sprintf(szReturn, "error=%s(%s:%d)$", FuncGetStringFromIDS("SV_MYSQL",
				"MYSQL_CONNECT_DATABASE_FAILED"), strHost, nPort);
			fprintf(stderr, "Failed to connect to database: Error: %s\n",
				mysql_error(mysql));

			mysql_close( mysql ) ;
			return FALSE;
		}	
		mysql_close(mysql);
		EndTime=::GetTickCount();
		sprintf(szReturn,"status=OK$time(毫秒)=%d$",EndTime-BeginTime);
	}
	catch(...)
	{
		sprintf(szReturn ,"error=%s$",GetLastError());
		bRet = FALSE;
	}

	CString strInput ;
	strInput =szReturn;
	MakeCharByString(szReturn,nSize,strInput);

	return bRet;
}