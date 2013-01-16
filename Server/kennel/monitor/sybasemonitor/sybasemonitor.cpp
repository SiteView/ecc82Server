// SybaseMonitor.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "SybaseMonitor.h"
#include "SybaseMonitor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include	<fstream>
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
// CSybaseMonitorApp

BEGIN_MESSAGE_MAP(CSybaseMonitorApp, CWinApp)
	//{{AFX_MSG_MAP(CSybaseMonitorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSybaseMonitorApp construction

CSybaseMonitorApp::CSybaseMonitorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSybaseMonitorApp object
CSybaseMonitorApp theApp;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//子函数

BOOL OpenDatabase(CDatabase *pDatabase,CString sConnectString);
void CloseDatabase(CDatabase *pDatabase);
BOOL ExeSql(CDatabase *pDatabase,CString sSql);
BOOL ExePro(CString sSql,CString sDsnName,CString sUser,CString sPwd);

BOOL GetDeadLock(CDatabase *pDatabase,char* szRet);
BOOL GetAllDataSpace(CDatabase *pDatabase,char* szRet);
BOOL GetAllDataName(CDatabase *pDatabase,CStringList &paramList,char *szReturn, int& nSize);
BOOL GetDataSpace(CDatabase *pDatabase,char* szRet,CString DatabaseName);
BOOL SpMonitor(CDatabase *pDatabase,CString sDsnName,CString sUser,CString sPwd,char* szRet);

BOOL GetLogSpace(CDatabase *pDatabase,char* szRet,CString DatabaseName);//yi.duan 2010-12-30 兼容12.5以下的版本
CTime StrToDate(CString sDate);
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//外部函数
//得到死锁的信息

int PrintLog(const char * strParas)
{
//#ifndef _DEBUG
//	return 0;
//#endif
	char timebuf[64],datebuf[64],tempbuf[1024];
	_strtime(timebuf);
	_strdate(datebuf);
	sprintf(tempbuf,"%s-%s",datebuf,timebuf);
	ofstream mystream;
	mystream.open("sybaselog.log",ios::app);
	mystream<<tempbuf<<"\t"<<strParas<<endl;
	mystream.close();
	return 0;
}
/**/
extern "C" _declspec (dllexport) BOOL DeadLock_C(char* InParam, char* szRet)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	CString strDsnName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	/*
	strDsnName = GetValueFromList("_DsnName",lstParas);
	strUserAccount = GetValueFromList("_UserAccount",lstParas);	
	strPassWord = GetValueFromList("_PassWord",lstParas);	
	*/
	strDsnName = "SybaseMonitor";
	strUserAccount = "sa";	
	strPassWord = "";	

	if(strDsnName.IsEmpty())
	{
		sprintf(szRet,"error=%", FuncGetStringFromIDS("IDS_DSN_NOT_Null")); //DSN不能为空");
		return FALSE;
	}
	if(strUserAccount.IsEmpty())
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_User_Not_Null")); //User不能为空");
		return FALSE;
	}

	CDatabase *pDatabase = NULL;
	pDatabase = new CDatabase();

	CString sConnectString;
	sConnectString = "DSN=";
	sConnectString += strDsnName;
	
	sConnectString += ";UID=";
	sConnectString += strUserAccount;

	sConnectString += ";PWD=";
	sConnectString += strPassWord;

	//打开数据库
	if(!OpenDatabase(pDatabase,sConnectString))
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Open_Database_Failed"));//打开数据库失败");
		return FALSE;
	}

	//得到所有数据空间信息
	if(!GetDeadLock(pDatabase,szRet))
	{		
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Database_Query_Failed")); // 数据库查询失败");
		return FALSE;
	}

	//Sleep(6000);

	//关闭数据库
	CloseDatabase(pDatabase);

	if (szRet=="")
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Cannot_Get_Data")); //不能采集数据");
		return FALSE;
	}


		
	return TRUE;

}

extern "C" _declspec (dllexport) BOOL DeadLock(const char * strParas, char * szRet, int& nSize)
//(CStringList& lstParas, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CString strDsnName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);
	
	strDsnName = GetValueFromList("_DsnName",lstParas);
	strUserAccount = GetValueFromList("_UserAccount",lstParas);	
	strPassWord = GetValueFromList("_PassWord",lstParas);	

	if(strDsnName.IsEmpty())
	{
		sprintf(szRet,"error=%s",  FuncGetStringFromIDS("IDS_DSN_NOT_Null")); //DSN不能为空");
		return FALSE;
	}
	if(strUserAccount.IsEmpty())
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_User_Not_Null")); //User不能为空");
		return FALSE;
	}

	CDatabase *pDatabase = NULL;
	pDatabase = new CDatabase();

	CString sConnectString;
	sConnectString = "DSN=";
	sConnectString += strDsnName;
	
	sConnectString += ";UID=";
	sConnectString += strUserAccount;

	sConnectString += ";PWD=";
	sConnectString += strPassWord;

	//打开数据库
	if(!OpenDatabase(pDatabase,sConnectString))
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Open_Database_Failed"));//打开数据库失败");
		return FALSE;
	}

	//得到所有数据空间信息
	if(!GetDeadLock(pDatabase,szRet))
	{		
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Database_Query_Failed")); //数据库查询失败");
		return FALSE;
	}

	//Sleep(6000);

	//关闭数据库
	CloseDatabase(pDatabase);

	if (szRet=="")
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Cannot_Get_Data")); //不能采集数据");
		return FALSE;
	}
	CString strInput;
	strInput =szRet;
	MakeCharByString(szRet,nSize,strInput);
/*
    char * strEnd = szRet;
	while(*strEnd)
	{
		PrintLog(strEnd);
		strEnd+=strlen(strEnd)+1;
	}
*/
	return TRUE;

}

//得到Monitor信息
extern "C" _declspec (dllexport) BOOL SpMonitor(const char * strParas, char * szRet, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CString strDsnName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);
	
	strDsnName = GetValueFromList("_DsnName",lstParas);
	strUserAccount = GetValueFromList("_UserAccount",lstParas);	
	strPassWord = GetValueFromList("_PassWord",lstParas);	

	if(strDsnName.IsEmpty())
	{
		sprintf(szRet,"error=%s",  FuncGetStringFromIDS("IDS_DSN_NOT_Null")); //DSN不能为空");
		return FALSE;
	}
	if(strUserAccount.IsEmpty())
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_User_Not_Null")); //User不能为空");
		return FALSE;
	}

	CDatabase *pDatabase = NULL;
	pDatabase = new CDatabase();

	CString sConnectString;
	sConnectString = "DSN=";
	sConnectString += strDsnName;
	
	sConnectString += ";UID=";
	sConnectString += strUserAccount;

	sConnectString += ";PWD=";
	sConnectString += strPassWord;



	//打开数据库
	if(!OpenDatabase(pDatabase,sConnectString))
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Open_Database_Failed"));//打开数据库失败");
		return FALSE;
	}

	
	//ExeSql(pDatabase,"call sp_monitor");

	//得到Monitor信息
	if(!SpMonitor(pDatabase,strDsnName,strUserAccount,strPassWord,szRet))
	{
		puts(szRet);

		if(strlen(szRet)==0)
			sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Database_Query_Failed")); //数据库查询失败");
		return FALSE;
	}

	//Sleep(6000);

	//关闭数据库
	CloseDatabase(pDatabase);

	if (szRet=="")
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Cannot_Get_Data")); //不能采集数据");
		return FALSE;
	}
	CString strInput;
	strInput =szRet;
	MakeCharByString(szRet,nSize,strInput);
	
		
	return TRUE;

}

//得到SABYLE数据库空间利用情况
extern "C" _declspec (dllexport) BOOL GetAllDataSpace(const char * strParas, char * szRet, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	
	CString strDsnName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);

	
	
	
	strDsnName = GetValueFromList("_DsnName",lstParas);
	strUserAccount = GetValueFromList("_UserAccount",lstParas);	
	strPassWord = GetValueFromList("_PassWord",lstParas);	

	if(strDsnName.IsEmpty())
	{
		sprintf(szRet,"error=%s",  FuncGetStringFromIDS("IDS_DSN_NOT_Null")); //DSN不能为空");
		return FALSE;
	}
	if(strUserAccount.IsEmpty())
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_User_Not_Null")); //User不能为空");
		return FALSE;
	}

	CDatabase *pDatabase = NULL;
	pDatabase = new CDatabase();

	CString sConnectString;
	sConnectString = "DSN=";
	sConnectString += strDsnName;
	
	sConnectString += ";UID=";
	sConnectString += strUserAccount;

	sConnectString += ";PWD=";
	sConnectString += strPassWord;

	//打开数据库
	if(!OpenDatabase(pDatabase,sConnectString))
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Open_Database_Failed"));//打开数据库失败");
		return FALSE;
	}

	//得到所有数据空间信息
	if(!GetAllDataSpace(pDatabase,szRet))
	{		
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Database_Query_Failed")); //数据库查询失败");
		return FALSE;
	}

	//Sleep(6000);

	//关闭数据库
	CloseDatabase(pDatabase);

	if (szRet=="")
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Cannot_Get_Data")); //不能采集数据");
		return FALSE;
	}
	CString strInput;
	strInput =szRet;
	MakeCharByString(szRet,nSize,strInput);

		
	return TRUE;

}

extern "C" _declspec (dllexport) 
BOOL GetDataSpace_C(char* InParam, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CString strDsnName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CString strDatabaseName=_T("");
	
	strDsnName = "SybaseMonitor";
	strUserAccount = "sa";	
	strPassWord = "";
	strDatabaseName = "master";	

	if(strDsnName.IsEmpty())
	{
		sprintf(szRet,"error=%s",  FuncGetStringFromIDS("IDS_DSN_NOT_Null")); //DSN不能为空");
		return FALSE;
	}
	if(strUserAccount.IsEmpty())
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_User_Not_Null")); //User不能为空");
		return FALSE;
	}
	if(strDatabaseName.IsEmpty())
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_DBNameNotNull")); //DatabaseName不能为空");
		return FALSE;
	}

	CDatabase *pDatabase = NULL;
	pDatabase = new CDatabase();

	CString sConnectString;
	sConnectString = "DSN=";
	sConnectString += strDsnName;
	
	sConnectString += ";UID=";
	sConnectString += strUserAccount;

	sConnectString += ";PWD=";
	sConnectString += strPassWord;

	//打开数据库
	if(!OpenDatabase(pDatabase,sConnectString))
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Open_Database_Failed"));//打开数据库失败");
		return FALSE;
	}

	if(!GetDataSpace(pDatabase,szRet,strDatabaseName))
	{		
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Database_Query_Failed")); //数据库查询失败");
		return FALSE;
	}

	//关闭数据库
	CloseDatabase(pDatabase);

	if (szRet=="")
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Cannot_Get_Data")); //不能采集数据");
		return FALSE;
	}
		
	return TRUE;
}
//得到SABYLE数据库库名
extern "C" _declspec (dllexport) 
BOOL GetAllDataName(const char * strParas, char * szReturn, int& nSize)
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	OutputDebugString("--------------Start GetAllDataName() call-----------------\n");
	PrintLog("========获取数据库表名的输入参数========\n");
	const char * strReceive = strParas;
	while(*strReceive)
	{
		PrintLog(strReceive);
		strReceive+=strlen(strReceive)+1;
	}
	PrintLog("\n");
	PrintLog("========获取数据库表名的输入参数结束========\n\n");
	


	CStringList paramList;

	MakeStringListByChar(paramList,strParas);
	
	/*
	lstTexts.AddTail("master");
	lstValues.AddTail("master");
	return TRUE;
	*/

	CString strDsnName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	
	strDsnName = GetValueFromList("_DsnName",paramList);
	strUserAccount = GetValueFromList("_UserAccount",paramList);	
	strPassWord = GetValueFromList("_PassWord",paramList);	

	
	if(strDsnName.IsEmpty())
	{
		//sprintf(szRet,"error=DSN不能为空");
		return FALSE;
	}
	if(strUserAccount.IsEmpty())
	{
		//sprintf(szRet,"error=User不能为空");
		return FALSE;
	}

	CDatabase *pDatabase = NULL;
	pDatabase = new CDatabase();

	CString sConnectString;
	sConnectString = "DSN=";
	sConnectString += strDsnName;
	
	sConnectString += ";UID=";
	sConnectString += strUserAccount;

	sConnectString += ";PWD=";
	sConnectString += strPassWord;

	//sConnectString = _T("DSN=Medical_Database;UID=sa;PWD=");

	//打开数据库
	if(!OpenDatabase(pDatabase,sConnectString))
	{
		PrintLog("连接数据库失败！");
		//sprintf(szRet,"error=打开数据库失败");
	
		return FALSE;
	}

	//得到所有数据库名称
	if(!GetAllDataName(pDatabase,paramList, szReturn, nSize))
	{
		PrintLog("获取所有数据库名称失败！");
		//sprintf(szRet,"error=数据库查询失败");
		return FALSE;
	}

	//Sleep(6000);

	//关闭数据库
	CloseDatabase(pDatabase);

	PrintLog("========返回值列表头========\n");
	char * strEnd = szReturn;
	while(*strEnd)
	{
		PrintLog(strEnd);
		strEnd+=strlen(strEnd)+1;
	}
	PrintLog("\n");
	PrintLog("========已到达返回值列表末尾！========\n\n");



	return TRUE;
}


//得到SABYLE数据库空间利用情况
extern "C" _declspec (dllexport) BOOL GetDataSpace(const char * strParas, char * szRet, int& nSize)
//(CStringList& lstParas, char* szRet)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	CString strDsnName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CString strDatabaseName=_T("");
	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);
	
	strDsnName = GetValueFromList("_DsnName",lstParas);
	strUserAccount = GetValueFromList("_UserAccount",lstParas);	
	strPassWord = GetValueFromList("_PassWord",lstParas);
	strDatabaseName = GetValueFromList("_DatabaseName",lstParas);	

	if(strDsnName.IsEmpty())
	{
		sprintf(szRet,"error=%s",  FuncGetStringFromIDS("IDS_DSN_NOT_Null")); //DSN不能为空");
		return FALSE;
	}
	if(strUserAccount.IsEmpty())
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_User_Not_Null")); //User不能为空");
		return FALSE;
	}
	if(strDatabaseName.IsEmpty())
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_DBNameNotNull")); //DatabaseName不能为空");
		return FALSE;
	}

	CDatabase *pDatabase = NULL;
	pDatabase = new CDatabase();

	CString sConnectString;
	sConnectString = "DSN=";
	sConnectString += strDsnName;
	
	sConnectString += ";UID=";
	sConnectString += strUserAccount;

	sConnectString += ";PWD=";
	sConnectString += strPassWord;

	//打开数据库
	if(!OpenDatabase(pDatabase,sConnectString))
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Open_Database_Failed"));//打开数据库失败");
		return FALSE;
	}

	if(!GetDataSpace(pDatabase,szRet,strDatabaseName))
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Database_Query_Failed")); //数据库查询失败");
		return FALSE;
	}

	//关闭数据库
	CloseDatabase(pDatabase);

	if (szRet=="")
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Cannot_Get_Data")); //不能采集数据");
		return FALSE;
	}else{
		CString strInput;
		strInput =szRet;
		MakeCharByString(szRet,nSize,strInput);
	}
		
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//得到死锁的信息
BOOL GetDeadLock(CDatabase *pDatabase,char* szRet)
{
	CString sSQL;

	//yi.duan 2010-12-30 添加==0,否则出现查询失败
	sSQL = "select spid,hostname,blocked, time_blocked from sysprocesses where  blocked>=0 order by time_blocked DESC";

	CString spid         = _T("");
	CString hostname     = _T("");
	CString blocked      = _T("");
	CString time_blocked = _T("");

	CRecordset *pRecordset;
	pRecordset = new CRecordset(pDatabase);

	try
	{
		pRecordset->Open(CRecordset::snapshot,sSQL);			//获取记录集快照
		//pRecordset->Open(CRecordset::dynamic,sSQL);
	}

	catch(...)
	{
		delete pRecordset;
		return FALSE;
	}
	
	int Count = 0;
	long MaxTime = 0;
	CString BlockedID;
	CString BlockedID_Buff[10];
	int WaitProcNum[10];
	long MaxWaitTime[10];

	for(int i=0; i<10; i++)
	{
		WaitProcNum[i] = 0;
		MaxWaitTime[i] = 0;
	}

	if(!pRecordset->IsBOF())
	{
		//pRecordset->MoveFirst();

		while(!pRecordset->IsEOF())
		{

			pRecordset->GetFieldValue("spid",spid);
			pRecordset->GetFieldValue("hostname",hostname);	
			pRecordset->GetFieldValue("blocked",blocked);
			pRecordset->GetFieldValue("time_blocked",time_blocked);

			//找出前10个最大的死锁
			printf("sDeadLockNum=%d\n", Count);
			if(Count<10)
			{
				BOOL Flag = FALSE;

				for(int i=0; i<Count; i++)
				{
					if(BlockedID_Buff[i] == blocked)
					{
						Flag = TRUE;
						WaitProcNum[i]++;

						if(atol(time_blocked)>MaxWaitTime[i])
							MaxWaitTime[i] = atol(time_blocked);

						break;
					}
				}

				if(!Flag)
				{
					BlockedID_Buff[Count] = blocked;
					WaitProcNum[Count]++;
					MaxWaitTime[Count] = atol(time_blocked);
					Count++;
				}
			}

			//取最大的死锁
			if(atol(time_blocked)>MaxTime)
			{
				MaxTime = atol(time_blocked);
				BlockedID = blocked;
			}

			pRecordset->MoveNext();
		}
	}

	pRecordset->Close();
	delete pRecordset;

	if(Count>0 && BlockedID_Buff[0] != "" && BlockedID != "")//yi.duan 2010-12-30 添加后面2个判断条件
	{
		
		sprintf(szRet, "%sDeadLockProID=%s$", szRet,BlockedID);
		sprintf(szRet, "%sDeadLockTime=%d$", szRet,MaxTime);
		sprintf(szRet, "%sDeadLockNum=%d$", szRet,Count);
		sprintf(szRet, "%sDeadLockInfo=", szRet);

		

		for(int i=0; i<Count; i++)
		{
			sprintf(szRet, "%sDeadLockProID:%s", szRet,BlockedID_Buff[i]);
			sprintf(szRet, "%sWaitProcNum:%d", szRet,WaitProcNum[i]);
			sprintf(szRet, "%sDeadLockTime:%d\n", szRet,MaxWaitTime[i]);

		}
		sprintf(szRet, "%s$", szRet);

		return TRUE;
	}
	else
	{
	
		sprintf(szRet, "%sDeadLockProID=NULL$", szRet);
		sprintf(szRet, "%sDeadLockTime=0$", szRet);
		sprintf(szRet, "%sDeadLockNum=0$", szRet);
		sprintf(szRet, "%sDeadLockInfo=0$", szRet);
		return TRUE;

	}

	return FALSE;
}

//子函数
BOOL SpMonitor(CDatabase *pDatabase,CString sDsnName,CString sUser,CString sPwd,char* szRet)
{
	
	CString sSQL;

	sSQL = "select lastrun,cpu_busy,io_busy,idle,pack_received,pack_sent,pack_errors,total_read,total_write,total_errors,connections from spt_monitor";

	CString seconds, lastrun,cpu_busy,io_busy,idle,pack_received,pack_sent,pack_errors,total_read,total_write,total_errors,connections;

	CRecordset *pRecordset;
	pRecordset = new CRecordset(pDatabase);					//用pDatabase来初始化pRecordset

	try
	{
		pRecordset->Open(CRecordset::snapshot,sSQL);
		char buf[256] ={0};

		//pRecordset->Open(CRecordset::dynamic, sSQL);
	}

	catch(...)
	{
		
		delete pRecordset;
		pRecordset=NULL;
		return FALSE;
	}
	
	int i = 0;
	double used = 0;
	
	if(!pRecordset->IsBOF())
	{
		
		//pRecordset->MoveFirst();

		while(!pRecordset->IsEOF())
		{			
			pRecordset->GetFieldValue("lastrun",lastrun);
			
			OutputDebugString("-------------first lastrun---------------\n");
			OutputDebugString(lastrun);
			OutputDebugString("\n");

			pRecordset->GetFieldValue("cpu_busy",cpu_busy);
			
			//OutputDebugString("-------------first cpu_busy----------------\n");
			//OutputDebugString(cpu_busy);
			//OutputDebugString("\n");

			pRecordset->GetFieldValue("io_busy",io_busy);	
			
			//OutputDebugString("-------------first io_busy----------------\n");
			//OutputDebugString(io_busy);
			//OutputDebugString("\n");
			
			pRecordset->GetFieldValue("idle",idle);
			//OutputDebugString("-------------first idle----------------\n");
			//OutputDebugString(idle);
			//OutputDebugString("\n");

			pRecordset->GetFieldValue("pack_received",pack_received);
			//OutputDebugString("-------------first pack_received----------------\n");
			//OutputDebugString(pack_received);
			//OutputDebugString("\n");

			pRecordset->GetFieldValue("pack_sent",pack_sent);
			//OutputDebugString("-------------first pack_sent----------------\n");
			//OutputDebugString(pack_sent);
			//OutputDebugString("\n");

			pRecordset->GetFieldValue("pack_errors",pack_errors);
			//OutputDebugString("-------------first pack_errors----------------\n");
			//OutputDebugString(pack_errors);
			//OutputDebugString("\n");

			pRecordset->GetFieldValue("total_read",total_read);
			//OutputDebugString("-------------first total_read----------------\n");
			//OutputDebugString(total_read);
			//OutputDebugString("\n");
			pRecordset->GetFieldValue("total_write",total_write);
			//OutputDebugString("-------------first total_write----------------\n");
			//OutputDebugString(total_write);
			//OutputDebugString("\n");
			pRecordset->GetFieldValue("total_errors",total_errors);
			//OutputDebugString("-------------first total_errors----------------\n");
			//OutputDebugString(total_errors);
			//OutputDebugString("\n");
			pRecordset->GetFieldValue("connections",connections);				
			pRecordset->MoveNext();
			i++;
		}
	}

	pRecordset->Close();
	delete pRecordset;
	pRecordset=NULL;

	if(i<=0)
	{
		sprintf(szRet,"error=%s","连接数据库失败！");
		return FALSE;
	}	

	char buffer[1024] = {0};	
	/*GetPrivateProfileString("sybase", 
		"seconds",
		"",
		buffer,
		sizeof(buffer) - 1,
		(LPCTSTR)"log.ini");
		*/

	OutputDebugString("------------start dfn_getprivateprofilestring call----------\n");
	DFN_GetPrivateProfileString( "sybase",  "seconds", "", buffer,  sizeof(buffer) - 1,  "logfile.ini");	//从logfile.ini中读取seconds的值保存到buffer中
	OutputDebugString("------------dfn_getprivateprofilestring call finish----------\n");


	//-----------------------------------------------
	//执行存储过程得到新的spt_monitor数据
	ExePro("sp_monitor()",sDsnName,sUser,sPwd);				//感觉这句毫无意义，执行成功反正一个真值，但却并没有将执行值传递给任何字符串中
	CTime tm = CTime::GetCurrentTime();						//获取当前系统时间
	int nHour = tm.GetHour();
	int nMinute = tm.GetMinute();
	int nSecond = tm.GetSecond();
	

	int numSecond = nHour*3600 + nMinute*60 + nSecond;

	int nSeconds = numSecond - atoi(buffer);	

	char buf[256];
	sprintf(buf, "%d", numSecond);							//将当前系统时间传化成秒保存在buf中

/*	WritePrivateProfileString ("sybase", 
                              "seconds", 
								buf,                       
                              "./log.ini"); 
*/

	OutputDebugString("------------start dfn_writeprivateprofilestring call----------\n");
	DFN_WritePrivateProfileString(
		"sybase",
		"seconds",
		buf,
		"logfile.ini");					//将当前系统时间以秒为单位保存在logfile.ini中，比如现在是13：08，则保存值为：13*3600+0*60+8=36908
	OutputDebugString("------------dfn_writeprivateprofilestring call finish----------\n");


	//-----------------------------------------------


	sSQL = "select lastrun,cpu_busy,io_busy,idle,pack_received,pack_sent,pack_errors,total_read,total_write,total_errors,connections from spt_monitor";

	CString seconds1, lastrun1,cpu_busy1,io_busy1,idle1,pack_received1,pack_sent1,pack_errors1,total_read1,total_write1,total_errors1,connections1;

	pRecordset = new CRecordset(pDatabase);

	try
	{
		pRecordset->Open(CRecordset::snapshot,sSQL);
		//pRecordset->Open(CRecordset::dynamic,sSQL);
	}

	catch(...)
	{
		delete pRecordset;
		return FALSE;
	}
	
	if(!pRecordset->IsBOF())
	{
		//pRecordset->MoveFirst();

		while(!pRecordset->IsEOF())
		{

			pRecordset->GetFieldValue("lastrun",lastrun1);
			OutputDebugString("-------------second lastrun-----------------\n");
			OutputDebugString(lastrun1);
			OutputDebugString("\n");
			pRecordset->GetFieldValue("cpu_busy",cpu_busy1);
			//OutputDebugString("-------------second cpu_busy1-----------------\n");
			//OutputDebugString(cpu_busy1);
			//OutputDebugString("\n");
			pRecordset->GetFieldValue("io_busy",io_busy1);	
			//OutputDebugString("-------------second io_busy1-----------------\n");
			//OutputDebugString(io_busy1);
			//OutputDebugString("\n");
			pRecordset->GetFieldValue("idle",idle1);
			//OutputDebugString("-------------second idle1-----------------\n");
			//OutputDebugString(idle1);
			//OutputDebugString("\n");
			pRecordset->GetFieldValue("pack_received",pack_received1);
			//OutputDebugString("-------------second received1-----------------\n");
			//OutputDebugString(pack_received1);
			//OutputDebugString("\n");
			pRecordset->GetFieldValue("pack_sent",pack_sent1);
			//OutputDebugString("-------------second pack_sent1-----------------\n");
			//OutputDebugString(pack_sent1);
			//OutputDebugString("\n");
			pRecordset->GetFieldValue("pack_errors",pack_errors1);
			//OutputDebugString("-------------second pack_errors1-----------------\n");
			//OutputDebugString(pack_errors1);
			//OutputDebugString("\n");
			pRecordset->GetFieldValue("total_read",total_read1);
			//OutputDebugString("-------------second total_read1-----------------\n");
			//OutputDebugString(total_read1);
			//OutputDebugString("\n");
			pRecordset->GetFieldValue("total_write",total_write1);
			//OutputDebugString("-------------second total_write1-----------------\n");
			//OutputDebugString(total_write1);
			//OutputDebugString("\n");
			pRecordset->GetFieldValue("total_errors",total_errors1);
			//OutputDebugString("-------------second total_errors1-----------------\n");
			//OutputDebugString(total_errors1);
			//OutputDebugString("\n");
			pRecordset->GetFieldValue("connections",connections1);					
			//OutputDebugString("-------------second connections1-----------------\n");
			//OutputDebugString(connections1);
			//OutputDebugString("\n");
			pRecordset->MoveNext();
			i++;
		}
	}

	

	pRecordset->Close();
	delete pRecordset;
	pRecordset=NULL;

	if(i<=0)
	{
		return FALSE;
	}

	//-----------------------------------------------------------------------------------
	//得到两次时间间隔以分为单位
	//CTime t = StrToDate(lastrun);
	//CTime t1 = StrToDate(lastrun1);
	//CTimeSpan ts=t1-t;


	
	
	if(nSeconds <= 0)
	{
		sprintf(szRet, "error=%s", FuncGetStringFromIDS("IDS_RefreshTimeSmall"));
		return FALSE;
	}

	//OutputDebugStr
	int Minutes = abs(nSeconds);


	//int Minutes = ts.GetMinutes();
	if(Minutes<=0)
	{
		puts("---------------Minutes <= 0--------------\n");
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_RefreshTimeSmall")); //刷新间隔小于1分钟");
		return FALSE;
	}
	

	//计算出每分钟的吞吐量

	double per_cpu_busy = atof(cpu_busy1) - atof(cpu_busy);
	per_cpu_busy = per_cpu_busy/Minutes;

	double per_io_busy = atof(io_busy1) - atof(io_busy);
	per_io_busy = per_io_busy/Minutes;

	double per_idle = atof(idle1) - atof(idle);
	per_idle = per_idle/Minutes;

	double per_pack_received = atof(pack_received1) - atof(pack_received);
	per_pack_received = per_pack_received/Minutes;

	double per_pack_sent = atof(pack_sent1) - atof(pack_sent);
	per_pack_sent = per_pack_sent/Minutes;

	double per_pack_errors = atof(pack_errors1) - atof(pack_errors);
	per_pack_errors = per_pack_errors/Minutes;

	double per_total_read = atof(total_read1) - atof(total_read);
	per_total_read = per_total_read/Minutes;

	double per_total_write = atof(total_write1) - atof(total_write);
	per_total_write = per_total_write/Minutes;

	double per_total_errors = atof(total_errors1) - atof(total_errors);
	per_total_errors = per_total_errors/Minutes;

	double per_connections = atof(connections1) - atof(connections);
	per_connections = per_connections/Minutes;

	//sprintf(szRet, "%sseconds=%d$", szRet,Minutes);
	sprintf(szRet, "%sper_cpu_busy=%.2f$", szRet,per_cpu_busy);
	sprintf(szRet, "%sper_io_busy=%.2f$", szRet,per_io_busy);
	sprintf(szRet, "%sper_idle=%.2f$", szRet,per_idle);
	sprintf(szRet, "%sper_pack_received=%.2f$", szRet,per_pack_received);

	sprintf(szRet, "%sper_pack_sent=%.2f$", szRet,per_pack_sent);
	sprintf(szRet, "%sper_pack_errors=%.2f$", szRet,per_pack_errors);
	sprintf(szRet, "%sper_total_read=%.2f$", szRet,per_total_read);
	sprintf(szRet, "%sper_total_write=%.2f$", szRet,per_total_write);
	sprintf(szRet, "%sper_total_errors=%.2f$", szRet,per_total_errors);
	sprintf(szRet, "%sper_connections=%.2f$", szRet,per_connections);

	return TRUE;


}

//得到数据库空间占用情况
BOOL GetAllDataSpace(CDatabase *pDatabase,char* szRet)
{
	CString sSQL;

	sSQL = "select sysdatabases.name  as name,sysusages.size as total,sysusages.unreservedpgs as free from sysdatabases ,sysusages where sysdatabases .dbid = sysusages.dbid";

	CString name,total,free;

	CRecordset *pRecordset;
	pRecordset = new CRecordset(pDatabase);

	try
	{
		pRecordset->Open(CRecordset::snapshot,sSQL);
		//pRecordset->Open(CRecordset::dynamic,sSQL);
	}

	catch(...)
	{
		delete pRecordset;
		return FALSE;
	}
	
	int i = 0;
	double used = 0;
	if(!pRecordset->IsBOF())
	{
		//pRecordset->MoveFirst();

		while(!pRecordset->IsEOF())
		{

			pRecordset->GetFieldValue("name",name);
			pRecordset->GetFieldValue("total",total);	
			pRecordset->GetFieldValue("free",free);
			
			used = atof(total) - atof(free);

			double usedPercent = used/atof(total);
			usedPercent = usedPercent*100;
			
			sprintf(szRet, "%sname=%s$", szRet,name);
			sprintf(szRet, "%stotal=%.2f$", szRet,atof(total)/512);
			sprintf(szRet, "%sfreeSpace=%.2f$", szRet,atof(free)/512);
			sprintf(szRet, "%sused=%.2f$", szRet,used/512);
			sprintf(szRet, "%sfreePercent=%.2f$", szRet,100-usedPercent);
			sprintf(szRet, "%susedPercent=%.2f$", szRet,usedPercent);

			pRecordset->MoveNext();
			i++;
		}
	}

	

	pRecordset->Close();
	delete pRecordset;

	if(i>0)
		return TRUE;
	else
		return FALSE;
}

//得到数据库空间占用情况
BOOL GetDataSpace(CDatabase *pDatabase,char* szRet,CString DatabaseName)
{
	CString sSQL;

	sSQL = "select sysdatabases.name  as name,sysusages.size as total,sysusages.unreservedpgs as free from sysdatabases ,sysusages where sysdatabases .dbid = sysusages.dbid";

	CRecordset *pRecordset;
	pRecordset = new CRecordset(pDatabase);

	try
	{
		pRecordset->Open(CRecordset::snapshot,sSQL);
		//pRecordset->Open(CRecordset::dynamic,sSQL);
	}

	catch(...)
	{
		delete pRecordset;
		return FALSE;
	}
	
	double total = 0;
	double free = 0;
	double used = 0;

	CString sName,sTotal,sFree;
	if(!pRecordset->IsBOF())
	{
		//pRecordset->MoveFirst();

		while(!pRecordset->IsEOF())
		{
			pRecordset->GetFieldValue("name",sName);
			pRecordset->GetFieldValue("total",sTotal);	
			pRecordset->GetFieldValue("free",sFree);

			if(DatabaseName == sName)
			{
				total += atof(sTotal);
				free += atof(sFree);
			}

			pRecordset->MoveNext();
		}
	}

	used = total - free;

	double usedPercent = 0;
	if(total > 0)
		usedPercent = used/total;

	usedPercent = usedPercent*100;
	
	sprintf(szRet, "%stotal=%.2f$", szRet,total/512);
	sprintf(szRet, "%sfreeSpace=%.2f$", szRet,free/512);
	sprintf(szRet, "%sused=%.2f$", szRet,used/512);
	sprintf(szRet, "%sfreePercent=%.2f$", szRet,100-usedPercent);
	sprintf(szRet, "%susedPercent=%.2f$", szRet,usedPercent);


	pRecordset->Close();
	delete pRecordset;
	return TRUE;
}

//得到数据库名称
BOOL GetAllDataName(CDatabase *pDatabase,CStringList &paramList, char *szReturn, int& nSize)
{
	OutputDebugString("--------------GetAllDataName Other call()------------------\n");
	CString sSQL;

	sSQL = "select sysdatabases.name  as name from sysdatabases ,sysusages where sysdatabases .dbid = sysusages.dbid";

	CString name;

	CRecordset *pRecordset;
	pRecordset = new CRecordset(pDatabase);

	try
	{
		pRecordset->Open(CRecordset::snapshot,sSQL);
		//pRecordset->Open(CRecordset::dynamic,sSQL);
	}

	catch(...)
	{
		delete pRecordset;
		return FALSE;
	}
	
	int i = 0;
	if(!pRecordset->IsBOF())
	{
		//pRecordset->MoveFirst();

		char *p =szReturn;
		while(!pRecordset->IsEOF())
		{

			pRecordset->GetFieldValue("name",name);
		
			//sprintf(szRet, "%sDataName=%s*@", szRet,name);
			/*lstTexts.AddTail(name);
			lstValues.AddTail(name);
*/
			sprintf(p,"%s=%s",name,name);
			p+= 2* strlen(name)+2;
			pRecordset->MoveNext();
			i++;
		}
	}

	pRecordset->Close();
	delete pRecordset;

	if(i>0)
		return TRUE;
	else
		return FALSE;
}

//打开数据库
BOOL OpenDatabase(CDatabase *pDatabase,CString sConnectString)
{
	pDatabase->SetLoginTimeout(1);
	try
	{
		if(!pDatabase->OpenEx(sConnectString))
		{
			delete pDatabase;
			pDatabase = NULL;
			return FALSE;
		}
	}
	catch(CDBException * e1)
	{
		e1->Delete();
		delete pDatabase;
		pDatabase = NULL;
		return FALSE;
	}
	catch(CMemoryException * e2)
	{
		e2->Delete();
		delete pDatabase;
		pDatabase = NULL;
		return FALSE;
	}

	return TRUE;
}

//关闭数据库
void CloseDatabase(CDatabase *pDatabase)
{
	if(pDatabase)
	{

		delete pDatabase;
		pDatabase = NULL;
	}
}

BOOL ExeSql(CDatabase *pDatabase,CString sSql)
{
	if(!pDatabase->BeginTrans())
		return FALSE;

    TRY
	{
	    pDatabase->ExecuteSQL(sSql);
		pDatabase->CommitTrans();
	}

	CATCH(CDBException, e)
	{
		pDatabase->Rollback();
		e->Delete();
		return FALSE;
	}

	END_CATCH

	return TRUE;
}

//COM + ADO执行存储过程
BOOL ExePro(CString sSql,CString sDsnName,CString sUser,CString sPwd)
{
	char chConnect[1023] = {0};
	_ConnectionPtr pConnection;
	_CommandPtr pCommand;
	
	sprintf(chConnect , "UID=%s;PWD=%s;DSN=%s", sUser, sPwd,sDsnName);
	
	OutputDebugString("--------------database connect string---------------\n");
	OutputDebugString(chConnect);
	OutputDebugString("\n");

	if(FAILED(CoInitialize(NULL)))
	{
		OutputDebugString("-------------------CoInitialize() call failure--------------------\n");
		return FALSE; 
	} 
	//CoInitialize(NULL);

	try
	{
		//创建数据库连接对象        
		HRESULT hResult=pConnection.CreateInstance(__uuidof(Connection) , NULL);
		if(FAILED(hResult)) 
		{
			OutputDebugString("---------------sybase connection create instance failure-------------\n");
			return FALSE;	
		}

		//连接数据库
		hResult=pConnection->Open(chConnect,"","",NULL);
		if(FAILED(hResult)) 
		{
			OutputDebugString("--------------sybase connection failure--------------\n");
			return FALSE;	
		}
    }
    catch(_com_error e)
	{
		OutputDebugString("---------------database connection exception---------------\n");
		_bstr_t bstrErr=e.Description();		
		return FALSE;
	}

	//建立一个_ParameterPtr对象。
	pCommand.CreateInstance("ADODB.Command");
	//初试化
	pCommand->ActiveConnection = pConnection;
	//给当前数据库连接给command对象。
	pCommand->CommandType= adCmdStoredProc;
	//设定类型为存储过程。
	pCommand->CommandText= _bstr_t("sp_monitor");
	//设定执行的过程名字。

	try
	{
		pCommand->Execute(NULL,NULL, adCmdStoredProc);//执行命令，取得记录集    

	}
	catch(...)
	{
		OutputDebugString("-----------------command execute exception-----------------\n");
		return FALSE;
	}



    return TRUE;
	
}

//字符串转变为字符串
CTime StrToDate(CString sDate)
{

	int year,month,day,hour,minute,second;

	int Index = sDate.Find('-',0);
	year = atoi(sDate.Left(Index));
	sDate.Delete(0,Index+1);

	Index = sDate.Find('-',0);
	month = atoi(sDate.Left(Index));
	sDate.Delete(0,Index+1);

	Index = sDate.Find(' ',0);
	day = atoi(sDate.Left(Index));
	sDate.Delete(0,Index+1);

	Index = sDate.Find(':',0);
	hour = atoi(sDate.Left(Index));
	sDate.Delete(0,Index+1);
	
	Index = sDate.Find(':',0);
	minute = atoi(sDate.Left(Index));
	sDate.Delete(0,Index+1);

	Index = sDate.Find('.',0);
	second = atoi(sDate.Left(Index));
	sDate.Delete(0,Index+1);

	return CTime(year,month,day,hour,minute,second);
}

/****************************************************************************
	Export Function Of DatabaseSourceNames (Fetch DSN)
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL SYSTEMDSN(const char * strParas, char * strReturn, int& nSize, char *strFlag )
{	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HKEY hKey = NULL;
	LONG lRet = NULL;	
	lRet = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\ODBC\\ODBC.INI\\ODBC Data Sources", 0, KEY_QUERY_VALUE , &hKey);
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

			if(strstr(szVal, "Oracle") ==NULL)
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

	return TRUE;
	
}

extern "C" __declspec(dllexport)
BOOL TestSybaseConn(const char * strParas, char * szRet , int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strDsnName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CStringList lstParas;
	int BeginTime=0,EndTime=0;	//连接耗费时间：EndTime-BeginTime
	MakeStringListByChar(lstParas,strParas);
	
	strDsnName = GetValueFromList("_DsnName",lstParas);
	strUserAccount = GetValueFromList("_UserAccount",lstParas);	
	strPassWord = GetValueFromList("_PassWord",lstParas);	

	if(strDsnName.IsEmpty())
	{
		sprintf(szRet,"error=%s",  FuncGetStringFromIDS("IDS_DSN_NOT_Null")); //DSN不能为空");
		return FALSE;
	}
	if(strUserAccount.IsEmpty())
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_User_Not_Null")); //User不能为空");
		return FALSE;
	}

	CDatabase *pDatabase = NULL;
	pDatabase = new CDatabase();

	CString sConnectString;
	sConnectString = "DSN=";
	sConnectString += strDsnName;
	
	sConnectString += ";UID=";
	sConnectString += strUserAccount;

	sConnectString += ";PWD=";
	sConnectString += strPassWord;



	BeginTime=GetTickCount();
	//打开数据库
	if(!OpenDatabase(pDatabase,sConnectString))
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Open_Database_Failed"));//打开数据库失败");
		return FALSE;
	}
	CloseDatabase(pDatabase);
	EndTime = GetTickCount();
	sprintf(szRet,"status=OK$time(毫秒)=%d",EndTime-BeginTime);
	CString strInput;
	strInput=szRet;
	MakeCharByString(szRet,nSize,strInput);
	return TRUE;
}


//yi.duan add 2010-12-30 兼容12.5以下的版本
//得到数据库日志空间
extern "C" _declspec (dllexport)
BOOL logSpace(const char * strParas, char * szRet, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strDsnName=_T("");
	CString strUserAccount=_T("");
	CString strPassWord=_T("");
	CString strDatabaseName=_T("");
	CStringList lstParas;
	MakeStringListByChar(lstParas,strParas);

	strDsnName = GetValueFromList("_DsnName",lstParas);
	strUserAccount = GetValueFromList("_UserAccount",lstParas);	
	strPassWord = GetValueFromList("_PassWord",lstParas);
	strDatabaseName = GetValueFromList("_DatabaseName",lstParas);	

	if(strDsnName.IsEmpty())
	{
		sprintf(szRet,"error=%s",  FuncGetStringFromIDS("IDS_DSN_NOT_Null")); //DSN不能为空");
		return FALSE;
	}
	if(strUserAccount.IsEmpty())
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_User_Not_Null")); //User不能为空");
		return FALSE;
	}
	if(strDatabaseName.IsEmpty())
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_DBNameNotNull")); //DatabaseName不能为空");
		return FALSE;
	}

	CDatabase *pDatabase = NULL;
	pDatabase = new CDatabase();

	CString sConnectString;
	sConnectString = "DSN=";
	sConnectString += strDsnName;

	sConnectString += ";UID=";
	sConnectString += strUserAccount;

	sConnectString += ";PWD=";
	sConnectString += strPassWord;

	//打开数据库
	if(!OpenDatabase(pDatabase,sConnectString))
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Open_Database_Failed"));//打开数据库失败");
		return FALSE;
	}

	if(!GetLogSpace(pDatabase,szRet,strDatabaseName))
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Database_Query_Failed")); //数据库查询失败");
		return FALSE;
	}

	//关闭数据库
	CloseDatabase(pDatabase);

	if (szRet=="")
	{
		sprintf(szRet,"error=%s", FuncGetStringFromIDS("IDS_Cannot_Get_Data")); //不能采集数据");
		return FALSE;
	}
	else{
		CString strInput;
		strInput =szRet;
		MakeCharByString(szRet,nSize,strInput);
	}

	return TRUE;
}

//得到数据库空间占用情况
BOOL GetLogSpace(CDatabase *pDatabase,char* szRet,CString DatabaseName)
{
	CString sSQL;


	sSQL.Format("select s.name, (select max(convert(int,t.status & 1)) from %s.dbo.systhresholds t where s.segment=t.segment) status, sum(u.size) size, sum(u.size - curunreservedpgs(u.dbid, u.lstart, u.unreservedpgs)) used, @@thresh_hysteresis hysteresis from %s.dbo.syssegments s, master.dbo.sysusages u where 1 = 1 and ((u.segmap / ((s.segment & 1) + 1)) / power(2, (s.segment & 30))) & 1 = 1 and u.dbid = db_id('%s') and s.name like '%%' group by s.name order by 1",
		DatabaseName.GetBuffer(DatabaseName.GetLength()),
		DatabaseName.GetBuffer(DatabaseName.GetLength()),
		DatabaseName.GetBuffer(DatabaseName.GetLength()));
	//sSQL = "select s.name, (select max(convert(int,t.status & 1)) from master.dbo.systhresholds t where s.segment=t.segment) status, sum(u.size) size, sum(u.size - curunreservedpgs(u.dbid, u.lstart, u.unreservedpgs)) used, @@thresh_hysteresis hysteresis from master.dbo.syssegments s, master.dbo.sysusages u where 1 = 1 and ((u.segmap / ((s.segment & 1) + 1)) / power(2, (s.segment & 30))) & 1 = 1 and u.dbid = db_id('master') and s.name like '%' group by s.name order by 1";

	printf("sql = %s\n", sSQL.GetBuffer(sSQL.GetLength()));

	CRecordset *pRecordset;
	pRecordset = new CRecordset(pDatabase);

	try
	{
		pRecordset->Open(CRecordset::snapshot,sSQL);
		//pRecordset->Open(CRecordset::dynamic,sSQL);
	}

	catch(...)
	{
		delete pRecordset;
		return FALSE;
	}

	double total = 0;
	double free = 0;
	double used = 0;

	CString sName,sTotal,sUerd;
	if(!pRecordset->IsBOF())
	{
		//pRecordset->MoveFirst();

		while(!pRecordset->IsEOF())
		{
			pRecordset->GetFieldValue("name",sName);
			printf("sName=%s\n", sName.GetBuffer(sName.GetLength()));
			if("logsegment" == sName)
			{
				pRecordset->GetFieldValue("used",sUerd);	
				pRecordset->GetFieldValue("size",sTotal);
				printf("used=%s\n", sUerd.GetBuffer(sUerd.GetLength()));
				printf("size=%s\n", sTotal.GetBuffer(sTotal.GetLength()));
				total += atof(sTotal);
				used += atof(sUerd);
				break;
			}

			pRecordset->MoveNext();
		}
	}

	double usedPercent = 0;
	if(total > 0)
		usedPercent = used/total;

	usedPercent = usedPercent*100;

	double dLogSpace = used / 512.0;
	double usedPer= 0.0;

	if (total != 0)
	{
		usedPer = static_cast<double>(100 * used) / total;
	}

	sprintf(szRet, "TotalMB=%.2f$UsedPer=%.2f$", dLogSpace, usedPer);

	printf("buf=%s\n",szRet);


	pRecordset->Close();
	delete pRecordset;
	return TRUE;
}


