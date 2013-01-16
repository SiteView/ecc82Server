// WeatherMonitor.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "WeatherMonitor.h"
#include <iostream>
#include <fstream>

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
// CWeatherMonitorApp

BEGIN_MESSAGE_MAP(CWeatherMonitorApp, CWinApp)
	//{{AFX_MSG_MAP(CWeatherMonitorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWeatherMonitorApp construction

CWeatherMonitorApp::CWeatherMonitorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWeatherMonitorApp object

CWeatherMonitorApp theApp;


#define		__ServiceName__					"_ServiceName="
#define		__USERACCOUNT__					"_UserAccount="
#define		__PASSWORD__					"_PassWord="
#define		__CONNECTTIMEOUT__				"_ConnTimeout="
#define		__QUERYTIMEOUT__				"_QueryTimeout="
#define		__TABLESPACENAME__				"_TableSpaceName="
#define		__PROCESSNAME__					"_ProcessName="
#define     __SQL__                         "_SQL="
#define     __TEMPLATEID__                  "_TemplateID="
#define     __MONITORID__                  "_MonitorID="
#define		__FILENAME__					"_FileName="
#define		__TIMEAREA__					"_TimeArea="

char  szoutFile[256]={0};
BOOL GetResult( CString & strBuf );
void WriteResultFile(const char* chMsg, const char* filename, int nFlag);
void insert();
void select();
BOOL DBrlogon( CString strConn, otl_connect* pdb,char * szReturn);
BOOL DB_TABLESPACENAME(char *dbconn, char *uid, char *pwd,   char * strReturn, int& nSize);
					   
BOOL DB_PROCESSNAME(char *dbconn, char *uid, char *pwd,  char* szReturn,	   int nSize);

#include <string>

using namespace std;
/**/
int PrintLog(const char * str)
{
#ifndef _DEBUG
	return 0;
#endif
	char timebuf[128],datebuf[128],tembuf[4096];
	_strtime(timebuf);
	_strdate(datebuf);
	ofstream tempstream;
	sprintf(tembuf,"%s-%s",datebuf,timebuf);
	tempstream.open("WeatherMonitor.log",ios::app);
	tempstream<<tembuf<<"\t"<<str<<endl;
	tempstream.close();
	return 0;
}

/****************************************************************************
	�������Ƽ������ȡ�������ĺ���GetProList
****************************************************************************/
extern "C" __declspec(dllexport)
BOOL GetProList(const char * strParas, char *szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	 char *strSize=new char[10240];
	sprintf(strSize,"Size=%d",nSize);
	//PrintLog(strSize);
/*
	const char * strTMP=strParas;
	while(*strTMP)
	{
		PrintLog(strTMP);
		strTMP+=strlen(strTMP)+1;
	}
*/
	
	CString		strDBConn = _T("");
	CString		strDBMatch = _T(""), 
				strDBUser = _T(""), 
				strDBPWD = _T(""), 
				strDBDriver = _T(""), 
				strTimeArea = _T(""),
				strQFile = _T("");
	long		intMonitorID;
	CString strLog = _T("");
///////////////////////////////////////////////////////////////////////////////////////////////////

/*	const char * temPtr = strParas;
	while(*temPtr)
	{
		PrintLog(temPtr);
		temPtr+=strlen(temPtr)+1;
	}
*/


///////////////////////////////////////////////////////////////////////////////////////////////////
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
			strDBPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__TEMPLATEID__)==0)
		{
			intMonitorID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TEMPLATEID__)).GetBuffer(1));
		}
		else if(strTemp.Find(__TIMEAREA__)==0)
		{
			strTimeArea = strTemp.Right(strTemp.GetLength() - strlen(__TIMEAREA__));
		}
	}

	strLog.Format("_ServiceName=%s _UserAccount=%s _PassWord=%s", 
		strDBConn, strDBUser, strDBPWD);	
	if(strDBConn.IsEmpty())
	{
		return false;
	}
//////////////////////////////////////////////////////////////////////////////////////////////
	otl_connect db;										//����һ�����Ӷ���
	CString strConn = _T("");
	otl_conn::initialize();								//OTL��ʼ��
	strConn.Format("UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;",strDBUser,strDBPWD,strDBConn); 
	string strQuery;
	switch(intMonitorID)
	{
	case 701:
		strQuery = "SELECT distinct PROCESS_NAME FROM MONITOR_INSTANTDISTR_STATE";
		break;
	case 702:
		strQuery = "SELECT distinct DBPROCESS_NAME FROM MONITOR_DBPROCESS_TREND";
		break;
//	case 703:
//		strQuery = "SELECT distinct FILENAME FROM MONITOR_CITYFORECAST WHERE itime>((select max(itime) from monitor_cityforecast)-"+strTimeArea+"/24/60)";
//		break;
	case 704:
		strQuery = "SELECT DISTINCT DBPROCESS_NAME FROM MONITOR_DBPROCESS_STATE";
		break;
	case 705:
		strQuery = "select process_name||targetip from monitor_instantdistr_trend";
		break;
//	case 706:
//		strQuery = "select distinct(type||vti) from MONITOR_INDEXFORECAST";
//		break;
	case 707:
		strQuery = "select code||fname from MONITOR_PRODUCT_ARRIVEINFO";
		break;
	case 708:
		strQuery = "select SERVER_IP||DIRECTORY from MONITOR_SERVERFILES_TREND where directory not like '/disk2/upload/wis158/%'";
		break;
	}


//	strQuery.Format("%s","SELECT distinct PROCESS_NAME FROM MONITOR_INSTANTDISTR_STATE");
	try
	{
		db.rlogon(strConn.GetBuffer(1));				//���ӵ�ODBC����Դ��
		otl_stream i(100,								// buffer size
			strQuery.c_str(),									// SELECT statement
              db										// connect object
             );
		char * szTableSpaceName = new char[nSize];
//		char szTableSpaceName[409600] = {0};				//���Խ��ջ�ȡ�����ݿ���и���������
		CString strName = _T("");
		int j=0;
		CString strTMP=_T("");
		while(!i.eof())
		{
			
			i>>szTableSpaceName;//>>nPid;
			strName.Format("%s",szTableSpaceName);
//			strName.Format("%s(PID:%d)", szTableSpaceName, nPid);
/*			if(j>=0)
			{
				sprintf(szReturn,"%s=%s$",strName,strName);
				szReturn+= 2* strlen(strName)+2;
			}
			j++;
*/
			sprintf(szReturn,"%s=%s$",strName,strName);
			strTMP+=szReturn;
		}
		sprintf(szReturn,"%s",strTMP.GetBuffer(1));
		CString strInput=szReturn;
		MakeCharByString(szReturn,nSize,strInput);
		i.close();
		delete szTableSpaceName;
		szTableSpaceName=NULL;

	}
	catch(otl_exception& p)
	{ 
		printf("error=%s\r\n%s", p.msg, p.var_info);
		strLog.Format("DB_PROCESSNAME ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
		db.logoff();											// disconnect from ODBC
		sprintf(szReturn,"error=%s:%s",p.msg,p.var_info);
		db.logoff();
		return false;
	}
/*
	char * strEnd=szReturn;
	while(*strEnd)
	{
		PrintLog(strEnd);
		strEnd+=strlen(strEnd)+1;
	}
*/
	delete strSize;
	strSize=NULL;
	db.logoff();
	return true;
}

/****************************************************************************
	��ʱ�ַ����̼����InstantDistrStateMonitor
****************************************************************************/
extern "C" __declspec(dllexport)
BOOL InstantDistriStateMonitor(char * strParas, char * szReturn, int& nSize)
{
	//AfxGetStaticModuleState()��ָ��ǰ��ģ��״̬��
	AFX_MANAGE_STATE(AfxGetStaticModuleState());			//AFX_MANAGE_STATE(p)�����������Ǳ���ģ��״̬����ǰ�������ý�����ԭģ���״̬�Զ��ָ���	

	CString		strDBConn = _T(""), 
				strQuery = _T("");
	CString		strDBMatch = _T(""), 
				strDBUser = _T(""), 
				strDBPWD = _T(""), 
				strDBDriver = _T(""), 
				strQFile = _T("");
	CString		strProcessName = _T("");
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
			strDBPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROCESSNAME__) ==0)
		{
			strProcessName = strTemp.Right(strTemp.GetLength() - strlen(__PROCESSNAME__));
		}
	}
	if(strDBConn.IsEmpty())
	{
		sprintf(szReturn,"error=%s","�������ݿ�ʧ�ܣ�");
		return false;
	}
	strLog.Format("_ServiceName=%s _UserAccount=%s _PassWord=%s",strDBConn, strDBUser, strDBPWD);	


//////////////////////////////����MONITOR_INSTANTDISTR_STATE��������ֶεı�������///////////////////////////////////////////

	string processName;				//��������
	otl_datetime startTime;			//����ʱ��
	string sourcePath;				//����Ŀ¼
	string action;					//���嶯��
	string targetIP;				//Ŀ���ַ
	string ftpUser;					//�����û�
	string targetUser;				//Ŀ��·��
	otl_datetime finishTime;		//����ʱ��
	string result;					//�������
	string exception;				//�쳣����
	string getResult;				//��¼����ֵ
	char beginTime[200]={0};
	char endTime[200]={0};
	string strSQL = "select * from monitor_instantdistr_state where process_name like '%"+strProcessName+"%'and starttime = (select max(starttime) from monitor_instantdistr_state where process_name like '%"+strProcessName+"%')";


//////////////////////////////����MONITOR_INSTANTDISTR_STATE��������ֶεı�������///////////////////////////////////////////


////////////////////////////////////////////////////////���ݿ��������///////////////////////////////////////////////////////
	otl_connect db;				//�������ݿ����Ӷ���
	CString strConn = _T("");
	strConn.Format("Provider=OraOLEDB.Oracle;UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;",strDBUser,strDBPWD,strDBConn);
	otl_connect::otl_initialize();		//OTL��ʼ��
	int failNum=0;						//ͳ��RESULT��fail�Ĵ�����
	int recordNum=0;					//ͳ�Ƹý��̼�¼����
	char temStr[200]={""};
	
	try
	{
		db.rlogon(strConn.GetBuffer(1));	//����һ������
		otl_stream o(1,strSQL.c_str(),db);	//ִ��SQL��ѯ
//		PrintLog(strSQL.c_str());
		while(!o.eof())
		{
			o>>processName>>startTime>>sourcePath>>action>>targetIP>>ftpUser>>targetUser>>finishTime>>result>>exception;
			sprintf(beginTime,"%d-%02d-%02d %2d:%2d:%2d",startTime.year,startTime.month,startTime.day,startTime.hour,startTime.minute,startTime.second);
			sprintf(endTime,"%d-%02d-%02d %2d:%2d:%2d",finishTime.year,finishTime.month,finishTime.day,finishTime.hour,finishTime.minute,finishTime.second);
			if(result.find("fail")!=-1)
			{
				getResult+="{��������="+processName+
					",��ʼʱ��="+beginTime+
					",Դ·��="+sourcePath+
					",ACTION="+action+
					",Ŀ��IP="+targetIP+
					",FTP�˺�="+ftpUser+									
					",Ŀ���˺�="+targetUser+
					",���ʱ��="+endTime+
					",������="+result+
					",�쳣���="+exception+
					"},";
				failNum++;
			}
			recordNum++;
//			sprintf(temStr,"%d",recordNum);
//			PrintLog(temStr);
				
		}
		if(failNum==0)
		{
			sprintf(szReturn ,"result=%d$getResult=%sһ��������",failNum,strProcessName);
		}
		else
		{
			sprintf(szReturn ,"result=%d$getResult=%s",failNum,getResult.c_str());
		}
		char * temptr=strstr(szReturn,"$");
		* temptr='\0';
/*
		char *ptr=szReturn;
		while(*ptr)
		{
			PrintLog(ptr);
			ptr+=strlen(ptr)+1;
		}
*/
	}
	catch(otl_exception& p )
	{
		sprintf(szReturn, "error=%s,%s$", p.msg, p.var_info);
		strLog.Format("GetOracleSelect ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
		db.logoff(); // disconnect from ODBC
		//PrintLog(szReturn);
		return false;
	}
	db.logoff();
	return true;
}

/****************************************************************************
	�������Ƽ����InstantDistrTrendMonitor
****************************************************************************/

extern "C" __declspec(dllexport)
BOOL InstantDistrTrendMonitor(char * strParas , char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());//����ģ�鵱ǰ״̬�����˺������ý����󣬻ָ�ԭģ���״̬
	CString strDBConn = _T("");
	CString strDBUser = _T("");
	CString strDBPWD = _T("");
	CString strLog = _T("");
	CString strSQL = _T("");
	CString strProName = _T("");
	CString strIP = _T("");
	CString strReceive = _T("");
	
	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp=paramList.GetNext(pos);
		if(strTemp.Find(__ServiceName__)==0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength()-(int)strlen(__ServiceName__));
		}
		else if(strTemp.Find(__USERACCOUNT__)==0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__)==0)
		{
			strDBPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROCESSNAME__)==0)
		{
			strReceive = strTemp.Right(strTemp.GetLength() - strlen(__PROCESSNAME__));
		}
	}
	strProName = strReceive.Left(strReceive.Find(' '));							//�ӽ��յ��ַ���������ȡ������
	strIP = strReceive.Right(strReceive.GetLength() - strReceive.Find('.')+3);	//�ӽ��յ��ַ���������ȡIP��ַ
	if(strIP.Find(' ')==0)
	{
		strIP = strIP.Left(strIP.Delete(0,1));
		if(strIP.Find(' ')==0)
		{
			strIP = strIP.Left(strIP.Delete(0,1));
		}
	}
	
	if(strDBConn.IsEmpty())
	{
		sprintf(szReturn,"error=%s","�������ݿ�ʧ�ܣ�");
		return false;
	}
	strLog.Format("_ServiceName=%s _UserAccount=%s _PassWord=%s",strDBConn, strDBUser, strDBPWD);

////////////////////////////����������ݿ����м�¼�ĸ�����////////////////////////////////////////////

	string	processName;		//��������
	string	targetIP;			//Ŀ��IP
	string	ftpUser;			//FTP�˺�
	string	status;				//״̬
	long		num_ten = 0;		//10�����ڴ�����ļ�����
	long		num_twenty = 0;		//20�����ڴ�����ļ�����
	long		num_thirty = 0;		//30�����ڴ�����ļ�����
	long		num_forty = 0;		//40�����ڴ�����ļ�����
	long		num_fifty = 0;		//50�����ڴ�����ļ�����
	long		num_sixty = 0;		//60�����ڴ�����ļ�����
	long		totalNum = 0;		//������ļ���������

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	otl_connect db;				//����OTL���Ӷ���
	otl_connect::otl_initialize();	//OTL��ʼ��
	CString strConn = _T("");
	strConn.Format("Provider=OraOLEDB.Oracle;UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;",strDBUser,strDBPWD,strDBConn);
	strSQL.Format("SELECT * FROM MONITOR_INSTANTDISTR_TREND WHERE PROCESS_NAME='%s' AND TARGETIP like '%s'",strProName.GetBuffer(1),strIP.GetBuffer(1));
	//strSQL.Format("%s","select * from MONITOR_INSTANTDISTR_TREND");
	try
	{
		db.rlogon(strConn.GetBuffer(1));	//��������
		otl_stream o(1,strSQL,db);			//ִ�в�ѯ���
		while(!o.eof())
		{
			o>>processName>>targetIP>>ftpUser>>status>>num_ten>>num_twenty>>num_thirty>>num_forty>>num_fifty>>num_sixty>>totalNum;
			sprintf(szReturn,"processName=%s$targetIP=%s$ftpUser=%s$status=%s$num_ten=%ld$num_twenty=%ld$num_thirty=%ld$num_forty=%ld$num_fifty=%ld$num_sixty=%ld$totalNum=%ld$",processName.c_str(),targetIP.c_str(),ftpUser.c_str(),status.c_str(),num_ten,num_twenty,num_thirty,num_forty,num_fifty,num_sixty,totalNum);
			if(totalNum==0)
			{
				sprintf(szReturn,"resutl=%s has been error",processName);
			}
		}
	}

	catch(otl_exception& p)
	{
		sprintf(szReturn, "error=%s,%s$", p.msg, p.var_info);
		strLog.Format("GetOracleSelect ExceptionMsg=%s ExceptionInfo=%s", p.msg, p.var_info);
		db.logoff();
		return false;
	}

	db.logoff();
	int nOutSize = 4096;
	CString strInput = szReturn;
	MakeCharByString(szReturn, nOutSize, strInput);
/*
	const char * temPtr = szReturn;
	while(*temPtr)
	{
		PrintLog(temPtr);
		temPtr+=strlen(temPtr)+1;
	}

*/
	return true;
}
/****************************************************************************
	����Ԥ�������CityForecastMonitor
****************************************************************************/

extern "C" __declspec(dllexport)
BOOL CityForecastMonitor(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
/*
	const char * pcStr=strParas;
	while(*pcStr)
	{
		PrintLog(pcStr);
		pcStr+=strlen(pcStr)+1;
	}
*/

	CStringList	paramList;
	CString		strDBConn = _T("");
	CString		strDBUser = _T("");
	CString		strDBPWD = _T("");
	CString		strFileName = _T("");
	CString		strLog = _T("");
	CString		strSQL = _T("");
	CString		strTimeArea = _T("");

	MakeStringListByChar(paramList,strParas);
	POSITION	pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString	strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__ServiceName__)==0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__ServiceName__));
		}
		else if(strTemp.Find(__USERACCOUNT__)==0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__)==0)
		{
			strDBPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
/*
		else if(strTemp.Find(__FILENAME__)==0)
		{
			strFileName = strTemp.Right(strTemp.GetLength() - strlen(__FILENAME__));
		}
*/
		else if(strTemp.Find(__TIMEAREA__)==0)
		{
			strTimeArea = strTemp.Right(strTemp.GetLength() - strlen(__TIMEAREA__));
		}
        
	}
	if(strDBConn.IsEmpty())
	{
		sprintf(szReturn, "%s","�������ݿ�ʧ�ܣ�");
		return false;
	}
	strLog.Format("_ServiceName=%s_UserAccount=%s_PassWord=%s",strDBConn,strDBUser,strDBPWD);
////////////////////////////����������ݿ����м�¼�ĸ�����////////////////////////////////////////////

	string	type;		//Ԥ������
	otl_datetime itime;	//���ʱ��
	string	fcdate;		//Ԥ��ʱ��-������
	string	fchh;		//Ԥ��ʱ��-ʱ
	long	vti;		//ʱЧ
	long	citynumber;	//Ԥ�����и���
	string	datasource;	//������Դ
	string	filename;	//�ļ�����
	long	errstation;	//�쳣վ����
	char cInTimes[128]=_T("");	
	otl_datetime MaxItime;	//������ʱ��
	char result[4096]=_T("");
	string detail=_T("");
	int nTotalCityNum=0;
	int nTotalErrStation=0;
	CString strResult=_T("");

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

    otl_connect	db;		//����һ��OTL����
	otl_connect::otl_initialize();	//OTL��ʼ��
	CString	strConn = _T("");	//���������ַ���
//	strConn.Format("Provider=OraOLEDB.Oracle;UID=%s;PWD=%s;DSN=%S;QTO=F;APA=T;",strDBUser,strPWD,strDBConn);
	strConn.Format("Provider=OraOLEDB.Oracle;UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T;",strDBUser,strDBPWD,strDBConn);
//	strSQL.Format("%s","select * from monitor_cityforecast where FILENAME like '%"+strFileName+"%' and itime>((select max(itime) from monitor_cityforecast)-"+strTimeArea+"/24/60)");
	strSQL.Format("%s","select * from monitor_cityforecast where itime>((select max(itime) from monitor_cityforecast)-"+strTimeArea+"/24/60)");
    
	PrintLog(strSQL.GetBuffer(strSQL.GetLength()));
	try
	{
		db.rlogon(strConn.GetBuffer(1));
		otl_stream	o(1,strSQL.GetBuffer(1),db);
		int RowNum=0;
		while(!o.eof())
		{
			o>>type>>itime>>fcdate>>fchh>>vti>>citynumber>>datasource>>filename>>errstation;			
			sprintf(cInTimes,"%d-%d-%d %d:%d:%d",itime.year,itime.month,itime.day,itime.hour,itime.minute,itime.second);
			sprintf(szReturn,",type=%s,itime=%s,fcdate=%s,fchh=%s,vti=%ld,citynumber=%ld,datasource=%s,filename=%s,errstation=%ld",type.c_str(),cInTimes,fcdate.c_str(),fchh.c_str(),vti,citynumber,datasource.c_str(),filename.c_str(),errstation);
			//sprintf(result,"{Ԥ������=%s,���ʱ��=%s,Ԥ��ʱ��=%s,Ԥ��ʱ��=%d,ʱЧ=%d,������Դ=%s,�ļ�����=%s,}",type.c_str(),cInTimes,fcdate.c_str(),atoi(fchh.c_str()),vti,datasource.c_str(),filename.c_str());
			nTotalCityNum+=citynumber;
			nTotalErrStation+=errstation;
			RowNum++;
			if(errstation!=0)
			{
				strResult.Format("%s,%s",strResult.GetBuffer(strResult.GetLength()),szReturn);
			}
//			PrintLog(szReturn);
			
		}
		if(RowNum==0)
		{
			sprintf(szReturn,"error=%s","����Ԥ�����޼�¼��!");
			return false;
		}
		nTotalCityNum=nTotalCityNum/RowNum;
		sprintf(szReturn,"result=%s$citynumber=%d$errstation=%d$",strResult.GetBuffer(strResult.GetLength()),nTotalCityNum,nTotalErrStation);
		if(nTotalErrStation==0)
		{
			sprintf(szReturn,"result=%s$citynumber=%d$errstation=%d$","û����վ��¼",nTotalCityNum,nTotalErrStation);
		}
		if(strlen(szReturn)>=4096)
		{
			sprintf(szReturn,"result=%scitynumber=%d$TotalErrStation=%d$","��վ��¼̫����",nTotalCityNum,nTotalErrStation);
		}

		//detail+=result;

	}
	catch(otl_exception& p)
	{
		sprintf(szReturn,"error=%s,%s\0",p.msg,p.var_info);
		strLog.Format("GetOracleSelect Exception=%s,ExceptionInfo=%s",p.msg,p.var_info);
		db.logoff();
		return false;
	}
	db.logoff();
	//sprintf(szReturn,"citynumber=%d$errstation=%d$detail=%s$",citynumber,errstation,detail.c_str());
	CString strInput = szReturn;	
	MakeCharByString(szReturn,nSize,strInput);
/*
	PrintLog("------------------------------------");
	const char * temPtr = szReturn;
	while(*temPtr)
	{
		PrintLog(temPtr);
		temPtr+=strlen(temPtr)+1;
	}
*/
	return true;
}


/****************************************************************************
	���ݿ⴦�����1Сʱ��״̬��DbProcessStateMonitor
****************************************************************************/
extern "C" __declspec(dllexport)
BOOL DbProcessStateMonitor(const char *strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
/*
	const char * strReceive=strParas;
	while(*strReceive)
	{
		PrintLog(strReceive);
		strReceive+=strlen(strReceive)+1;
	}
*/	
	CStringList	paramList;
	CString		strDBConn = _T("");
	CString		strDBUser = _T("");
	CString		strDBPWD = _T("");
	CString		strDBProcessName = _T("");
	CString		strLog = _T("");
	CString		strSQL = _T("");

	MakeStringListByChar(paramList,strParas);
	POSITION	pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__ServiceName__)==0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__ServiceName__));
		}
		else if(strTemp.Find(__USERACCOUNT__)==0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__)==0)
		{
			strDBPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROCESSNAME__)==0)
		{
			strDBProcessName = strTemp.Right(strTemp.GetLength() - strlen(__PROCESSNAME__));
		}
	}
	if(strDBConn.IsEmpty())
	{
		sprintf(szReturn,"error=%s","�������ݿ�ʧ�ܣ�");
		return false;
	}
	strLog.Format("_ServiceName=%s_UserAccount=%s_PassWord=%s",strDBConn,strDBUser,strDBPWD);

////////////////////////////����������ݿ����м�¼�ĸ�����////////////////////////////////////////////
	string	dbprocess_name=_T("");		//DB�����������
	otl_datetime	starttime;	//������ʼʱ��
	string	rsource=_T("");			//������Դ
	string	action=_T("");				//���嶯��
	otl_datetime	finishtime;	//�������ʱ��
	string	result=_T("");				//�������
	string	exception=_T("");			//�쳣����
	char cBeginTime[128] = {0};
	char cEndTime[128] = {0};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	otl_connect	db;
	otl_connect::otl_initialize();
	CString		strConn = _T("");
	strConn.Format("Provider=OraOLEDB.Oracle;UID=%s;PWD=%s;DSN=%s;QTO=F;APA=F",strDBUser,strDBPWD,strDBConn);
	strSQL.Format("%s","select * from MONITOR_DBPROCESS_STATE where DBPROCESS_NAME like '%"+strDBProcessName+"%' and starttime=(select max(starttime) from MONITOR_DBPROCESS_STATE where DBPROCESS_NAME like '%"+strDBProcessName+"%')");
//	strSQL.Format("select * from MONITOR_DBPROCESS_STATE where DBPROCESS_NAME like '%s' and starttime=(select max(starttime) from MONITOR_DBPROCESS_STATE where DBPROCESS_NAME like '%s')",strDBProcessName,strDBProcessName);


	try
	{
		db.rlogon(strConn.GetBuffer(1));
		otl_stream o(1,strSQL,db);
		while(!o.eof())
		{
			o>>dbprocess_name>>starttime>>rsource>>action>>finishtime>>result>>exception;
			sprintf(cBeginTime,"%d-%d-%d %d:%d:%d",starttime.year,starttime.month,starttime.day,starttime.hour,starttime.minute,starttime.second);
			sprintf(cEndTime,"%d-%d-%d %d:%d:%d",finishtime.year,finishtime.month,finishtime.day,finishtime.hour,finishtime.minute,finishtime.second);
			sprintf(szReturn,"ProcessName=%s$StartTime=%s$Resource=%s$Action=%s$FinishTime=%s$Result=%s$Exception=%s$",dbprocess_name.c_str(),cBeginTime,rsource.c_str(),action.c_str(),cEndTime,result.c_str(),exception.c_str());
		}
	}
	catch(otl_exception& p)
	{
		sprintf(szReturn,"error=%s,%s\0",p.msg,p.var_info);
		strLog.Format("GetOracleSelect Exception=%s ExceptionInfo=%s",p.msg,p.var_info);
		db.logoff();
		return false;
	}

	db.logoff();


	int nOutSize=4096;
	CString strInput=szReturn;
	MakeCharByString(szReturn,nOutSize,strInput);
/*	
	const char * tempStr=szReturn;
	while(*tempStr)
	{
		PrintLog(tempStr);
		tempStr+=strlen(tempStr)+1;
	}
	PrintLog("================================");

*/
	return true;
}

/****************************************************************************
	���ݿ⴦�����1Сʱ�����Ƽ���� DBProcessTrendMonitor
****************************************************************************/
extern "C" __declspec(dllexport)
BOOL DBProcessTrendMonitor(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	

	CStringList paramList;
	CString		strDBConn = _T("");
	CString		strDBUser = _T("");
	CString		strDBPWD = _T("");
	CString		strLog = _T("");
	CString		strSQL = _T("");
	CString		strProName = _T("");
	MakeStringListByChar(paramList,strParas);

	POSITION pos=paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp=paramList.GetNext(pos);
		if(strTemp.Find(__ServiceName__)==0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__ServiceName__));
		}
		else if(strTemp.Find(__USERACCOUNT__)==0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__)==0)
		{
			strDBPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROCESSNAME__)==0)
		{
			strProName = strTemp.Right(strTemp.GetLength() - strlen(__PROCESSNAME__));
		}
	}
	if(strDBConn.IsEmpty())
	{
		sprintf(szReturn,"error=%s","�������ݿ�ʧ�ܣ�");
		return false;
	}
	strLog.Format("_ServiceName=%s_UserAccount=%s,_Password=%s",strDBConn,strDBUser,strDBPWD);

////////////////////////////����������ݿ����м�¼�ĸ�����////////////////////////////////////////////
	string	dbprocess_name;	//DB�����������
	string	status;			//���ݴ���״̬�����������桢����Σ��
	long	ten_minutes;		//10���Ӵ�����ļ�����
	long	twenty_minutes;		//20���Ӵ�����ļ�����
	long thirty_minutes;		//30���Ӵ�����ļ�����
	long forty_minutes;		//40���Ӵ�����ļ�����
	long fifty_minutes;		//50���Ӵ�����ļ�����
	long sixty_minutes;		//60���Ӵ�����ļ�����
	long total;				//1Сʱ�ۼƴ�����ļ�����
	char detail[4098]={0};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	otl_connect db;
	otl_connect::otl_initialize();
	CString	strConn = _T("");
	strConn.Format("Provider=OraOLEDB.Oracle;UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T",strDBUser,strDBPWD,strDBConn);
	strSQL.Format("%s","select * from MONITOR_DBPROCESS_TREND where DBPROCESS_NAME like '%"+strProName+"%'");
	try
	{
		db.rlogon(strConn.GetBuffer(1));
		otl_stream	o(1,strSQL,db);
		while(!o.eof())
		{
			o>>dbprocess_name>>status>>ten_minutes>>twenty_minutes>>thirty_minutes>>forty_minutes>>fifty_minutes>>sixty_minutes>>total;			

		}
//		sprintf(detail,"������=%s,״̬=%s,10���Ӵ�����ļ�����=%d,20���Ӵ�����ļ�����=%d,30���Ӵ�����ļ�����=%d,40���Ӵ�����ļ�����=%d,50���Ӵ�����ļ�����=%d,60���Ӵ�����ļ�����=%d,1Сʱ�ۼƴ�����ļ�����=%d",dbprocess_name.c_str(),status.c_str(),ten_minutes,twenty_minutes,thirty_minutes,forty_minutes,fifty_minutes,sixty_minutes,total);
		sprintf(szReturn,"ProcessName=%s$Status=%s$tenMin=%d$twentyMin=%d$thirtyMin=%d$fortyMin=%d$fiftyMin=%d$sixtyMin=%d$Total=%d$",dbprocess_name.c_str(),status.c_str(),ten_minutes,twenty_minutes,thirty_minutes,forty_minutes,fifty_minutes,sixty_minutes,total);

//		sprintf(szReturn,"detail=%s$status=%s",detail,status.c_str());

	}
	catch(otl_exception& p)
	{
		sprintf(szReturn,"error=%s,%s\0",p.msg,p.var_info);
		strLog.Format("GetOracleSelect Exception=%s ExceptionInfo=%s",p.msg,p.var_info);
		db.logoff();
		return false;
	}
	catch(...)
	{
		sprintf(szReturn,"error=%s","����δ֪����");
		db.logoff();
		return false;
	}
	db.logoff();
	int nOutSize=(int)strlen(szReturn)+3;
	CString csInput=szReturn;
	MakeCharByString(szReturn,nOutSize,csInput);
//	char * temp=strstr(szReturn,"$");
//	* temp='\0';

/*
	const char * temPtr = szReturn;
	while(*temPtr)
	{
		PrintLog(temPtr);
		temPtr+=strlen(temPtr)+1;
	}
*/	
	return true;
}

/****************************************************************************
	ָ��Ԥ��������Ϣ�� IndexForecastMonitor
****************************************************************************/
extern "C" __declspec(dllexport)
BOOL IndexForecastMonitor(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
/*
	const char * strTest = strParas;
	while(*strTest)
	{
		PrintLog(strTest);
		strTest+=strlen(strTest)+1;
	}
*/
	CStringList paramList;
	CString		strDBConn = _T("");
	CString		strDBUser = _T("");
	CString		strDBPWD = _T("");
	CString		strSQL = _T("");
	CString		strLog = _T("");
	CString		strReceive = _T("");
	CString		strType = _T("");
	CString		strVTI = _T("");
	MakeStringListByChar(paramList,strParas);
	
	POSITION	pos=paramList.GetHeadPosition();
	while(pos)
	{
		CString	strTemp=paramList.GetNext(pos);
		if(strTemp.Find(__ServiceName__)==0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__ServiceName__));
		}
		else if(strTemp.Find(__USERACCOUNT__)==0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__)==0)
		{
			strDBPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
//		else if(strTemp.Find(__PROCESSNAME__)==0)
//		{
//			strReceive = strTemp.Right(strTemp.GetLength() - strlen(__PROCESSNAME__));
//		}
	}
	if(strDBConn.IsEmpty())
	{
		sprintf(szReturn,"error=%s","�������ݿ�ʧ�ܣ�");
		return false;
	}
	strLog.Format("_ServiceName=%s_UserAccount=%s_PassWord=%s",strDBConn,strDBUser,strDBPWD);
/*
	int i=0;
	i=strReceive.FindOneOf("0123456789");			//���ַ���������ҵ�һ�����ֵ�λ�ã����丳��i
	strType = strReceive.Left(i);					//�ӻ�ȡ���ַ������棬��TYPE��ȡ����
	strVTI = strReceive.Right(strReceive.GetLength() - i);	//�ӻ�ȡ���ַ������棬��vti��ȡ����
	long iVTI = atoi(strVTI.GetBuffer(1));
*/

////////////////////////////����������ݿ����м�¼�ĸ�����////////////////////////////////////////////
    
    string			type;			//ָ������
	otl_datetime	itime;			//���ʱ��
	string			fcdate;			//Ԥ��ʱ��-������
	string			fchh;			//Ԥ��ʱ��-ʱ
	long			vti;			//Ԥ��ʱЧ
	long			citynumber;		//Ԥ�����и���
	string			datasource;		//������Դ
	string			indexfilename;	//�ļ�����
	long			errstation;		//�쳣վ�б�
	char			tempTime[1024]={0};
	int	nErrNum = 0;
	CString strErrRecord = _T("");

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	otl_connect db;
	otl_connect::otl_initialize();
	CString		strConn = _T("");
	strConn.Format("Provider=OraOLEDB.Oracle;UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T",strDBUser,strDBPWD,strDBConn);
	strSQL.Format("%s","select t.* from monitor_indexforecast t, (select type||vti tv, max(itime) mt from monitor_indexforecast group by type||vti) t1 where t.type||t.vti=t1.tv and t.itime=t1.mt");
//	strSQL.Format("select * from monitor_indexforecast where itime=(select max(itime) from MONITOR_INDEXFORECAST  where type like '%s' and vti=%ld)",strType,iVTI);
//	strSQL.Format("%s","select * from MONITOR_INDEXFORECAST where itime=(select max(itime) from MONITOR_INDEXFORECAST)");
	try
	{
		db.rlogon(strConn.GetBuffer(1));
		otl_stream	o(1,strSQL,db);
		while(!o.eof())
		{
			o>>type>>itime>>fcdate>>fchh>>vti>>citynumber>>datasource>>indexfilename>>errstation;
			sprintf(tempTime,"%d/%d/%d-%d:%d:%d",itime.year,itime.month,itime.day,itime.hour,itime.minute,itime.second);
			sprintf(szReturn,"type=%s,itime=%s,fcdate=%s,fchh=%s,vti=%ld,citynumber=%ld,datasource=%s,indexfilename=%s,errstation=%ld",type.c_str(),tempTime,fcdate.c_str(),fchh.c_str(),vti,citynumber,datasource.c_str(),indexfilename.c_str(),errstation);
			if(errstation!=0)
			{
				nErrNum++;
				strErrRecord.Format("%s,%s",strErrRecord.GetBuffer(strErrRecord.GetLength()), szReturn);
			}
		}
	}
	catch(otl_exception & p)
	{
		sprintf(szReturn,"error=%s,%s\0",p.msg,p.var_info);
		strLog.Format("GetOracleSelect Exception=%s ExceptionInfor=%s",p.msg,p.var_info);
		db.logoff();
		return false;
	}

	db.logoff();
	if(strErrRecord.GetLength()>=nSize)
	{
		sprintf(szReturn,"error=%s","Error Information is too large!");
		return FALSE;
	}
	if(nErrNum==0)
	{
		sprintf(szReturn,"result=%s$errstation=%d","����ָ��Ԥ����ʱЧ����",nErrNum);
	}
	else
	{
		sprintf(szReturn,"result=%s$errstation=%d$",strErrRecord.GetBuffer(strErrRecord.GetLength()),nErrNum);
	}
	CString strInput=szReturn;
	MakeCharByString(szReturn, nSize, strInput);
/*
	const char * temPtr = szReturn;
	while(*temPtr)
	{
		PrintLog(temPtr);
		temPtr+=strlen(temPtr)+1;
	}

*/

	return true;
}

/****************************************************************************
	������1Сʱ���ļ�������Ϣ�� ServerFilesTrendMonitor
****************************************************************************/
extern "C" __declspec(dllexport)
BOOL ServerFilesTrendMonitor(const char * strParas, char * szReturn, int&nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
/*
	const char * strTest = strParas;
	while(*strTest)
	{
		PrintLog(strTest);
		strTest+=strlen(strTest)+1;
	}
*/
	CStringList paramList;
	CString		strDBConn = _T("");
	CString		strDBUser = _T("");
	CString		strDBPWD = _T("");
	CString		strLog = _T("");
	CString		strSQL = _T("");
	CString		strProcessName = _T("");
	CString		strServerIP = _T("");
	CString		strDirectory = _T("");
	MakeStringListByChar(paramList, strParas);

	POSITION	pos=paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__ServiceName__)==0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__ServiceName__));
		}
		else if(strTemp.Find(__USERACCOUNT__)==0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__)==0)
		{
			strDBPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROCESSNAME__)==0)
		{
			strProcessName = strTemp.Right(strTemp.GetLength() - strlen(__PROCESSNAME__));
		}
	}
	if(strDBConn.IsEmpty())
	{
		sprintf(szReturn,"error=%s","�������ݿ�ʧ�ܣ�");
		return false;
	}
	strLog.Format("_ServiceName=%s_UserAccount=%s_PassWord=%s",strDBConn,strDBUser,strDBPWD);
	int i=0;
	i=strProcessName.Find(' ');
	strServerIP = strProcessName.Left(i);
	i=strProcessName.Find('/');
	strDirectory = strProcessName.Right(strProcessName.GetLength() - i);

////////////////////////////����������ݿ����м�¼�ĸ�����////////////////////////////////////////////

	string	server_IP;		//��������ַ
	string	directory;		//Ŀ¼������·����
	string	receive_status;	//�ļ�����״̬�����������棬Σ�գ�����
	string	process_status;	//�ļ�����״̬�����������棬Σ�գ�����
	long	ten_minutes;		//10�����յ����ļ�����
	long	twenty_minutes;		//20�����յ����ļ�����
	long thirty_minutes;		//30�����յ����ļ�����
	long forty_minutes;		//40�����յ����ļ�����
	long fifty_minutes;		//50�����յ����ļ�����
	long sixty_minutes;		//60�����յ����ļ�����
	long total;				//1Сʱ�ۼ��յ����ļ�����

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	otl_connect	db;
	otl_connect::otl_initialize();
	CString		strConn = _T("");
	strConn.Format("Provider=OcaOLEDB.Oracle;UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T",strDBUser,strDBPWD,strDBConn);
	strSQL.Format("select * from MONITOR_SERVERFILES_TREND where SERVER_IP like '%%%s%%' and DIRECTORY like '%%%s%%'",strServerIP.GetBuffer(1),strDirectory.GetBuffer(1));
	try
	{
		db.rlogon(strConn.GetBuffer(1));
		otl_stream	o(1,strSQL,db);
		while(!o.eof())
		{
			o>>server_IP>>directory>>receive_status>>process_status>>ten_minutes>>twenty_minutes>>thirty_minutes>>forty_minutes>>fifty_minutes>>sixty_minutes>>total;
			sprintf(szReturn,"server_IP=%s$directory=%s$receive_status=%s$process_status=%s$ten_minutes=%ld$twenty_minutes=%ld$thirty_minutes=%ld$forty_minutes=%ld$fifty_minutes=%ld$sixty_minutes=%ld$total=%ld$",
				server_IP.c_str(),directory.c_str(),receive_status.c_str(),process_status.c_str(),ten_minutes,twenty_minutes,thirty_minutes,forty_minutes,fifty_minutes,sixty_minutes,total);
		}
	}
	catch(otl_exception& p)
	{
		sprintf(szReturn,"error=%s %s",p.msg,p.var_info);
		strLog.Format("GetOracleSelect Exception=%s ExceptionInfo=%s",p.msg,p.var_info);
		db.logoff();
		return	false;
	}
	db.logoff();
	int nOutSize=4096;
	CString strInput=szReturn;
	MakeCharByString(szReturn, nOutSize, strInput);
	const char * strEnd = szReturn;
/*
	while(*strEnd)
	{
		PrintLog(strEnd);
		strEnd+=strlen(strEnd)+1;
	}
*/
	return true;
    
		
}

/****************************************************************************
	��Ʒ�������������Ϣ�� ProductArriveInfoMonitor
****************************************************************************/
extern "C" __declspec(dllexport)
BOOL ProductArriveInfoMonitor(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
/*
	const char * strTest = strParas;
	while(*strTest)
	{
		PrintLog(strTest);
		strTest+=strlen(strTest)+1;
	}
*/
	CStringList	paramList;
	CString		strDBConn = _T("");
	CString		strDBUser = _T("");
	CString		strDBPWD = _T("");
	CString		strSQL = _T("");
	CString		strLog = _T("");
	CString		strProcessName = _T("");
	CString		strCode = _T("");
	CString		strFname = _T("");
	MakeStringListByChar(paramList,strParas);

	POSITION	pos=paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__ServiceName__)==0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength() - strlen(__ServiceName__));
		}
		else if(strTemp.Find(__USERACCOUNT__)==0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__)==0)
		{
			strDBPWD = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROCESSNAME__)==0)
		{
			strProcessName = strTemp.Right(strTemp.GetLength() - strlen(__PROCESSNAME__));
		}
	}
	if(strDBConn.IsEmpty())
	{
		sprintf(szReturn,"error=%s","�������ݿ�ʧ�ܣ�");
		return false;
	}
	strLog.Format("_ServiceName=%s_UserAccount=%s_PassWord=%s",strDBConn,strDBUser,strDBPWD);
	int i=0;
	i=strProcessName.Find(' ');
	strCode = strProcessName.Left(i);
	i=strProcessName.FindOneOf("abcdefghijklmnopqrstuvwxyz");
	strFname = strProcessName.Right(strProcessName.GetLength() - i);

////////////////////////////����������ݿ����м�¼�ĸ�����////////////////////////////////////////////

	string	code;				//��Ʒ����
	string	name;				//��Ʒ����
	string	fname;				//�ļ�����
	otl_datetime	base_time;	//��ƷӦ����ʱ��
	otl_datetime	arrive_time;//��Ʒ����ʱ��
	long	arrive_number;		//��Ʒ�������
	string	status;				//״̬
	char strPlanTime[128]={0};
	char strReachTime[128]={0};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	otl_connect db;
	otl_connect::otl_initialize();
	CString	strConn;
	strSQL.Format("select * from MONITOR_PRODUCT_ARRIVEINFO where code like '%%%s%%' and fname like '%%%s%%'",strCode.GetBuffer(1),strFname.GetBuffer(1));
//	strSQL.Format("%s","select * from MONITOR_PRODUCT_ARRIVEINFO");
	strConn.Format("Provider=OcaOLEDB.Olacle;UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T",strDBUser,strDBPWD,strDBConn);
	try
	{
		db.rlogon(strConn.GetBuffer(1));
		otl_stream	o(1,strSQL,db);
		while(!o.eof())
		{
			o>>code>>name>>fname>>base_time>>arrive_time>>arrive_number>>status;
			sprintf(strPlanTime,"%d/%d%d-%d:%d:%d",base_time.year,base_time.month,base_time.day,base_time.hour,base_time.minute,base_time.second);
			sprintf(strReachTime,"%d/%d/%d-%d:%d:%d",arrive_time.year,arrive_time.month,arrive_time.day,arrive_time.hour,arrive_time.minute,arrive_time.second);
			sprintf(szReturn,"code=%s$name=%s$fname=%s$base_time=%s$arrive_time=%s$arrive_number=%ld$status=%s$",code.c_str(),name.c_str(),fname.c_str(),strPlanTime,strReachTime,arrive_number,status.c_str());

		}
	}
	catch(otl_exception& p)
	{
		sprintf(szReturn,"error=%s %s",p.msg,p.var_info);
		strLog.Format("GetOracleSelect Exception=%s ExceptionInfo=%s",p.msg,p.var_info);
		db.logoff();
		return	false;
	}
	db.logoff();
	CString strInput=szReturn;
	int nOutSize=4096;
	MakeCharByString(szReturn, nOutSize, strInput);
	return	true;


}

/****************************************************************************
	��Ʒ������������ProductInfoMonitor
****************************************************************************/
extern "C" __declspec(dllexport)
BOOL ProductInfoMonitor(const char * strParas, char * szReturn, int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList	paramList;
	CString		strDBConn=_T("");
	CString		strDBUser=_T("");
	CString		strDBPWD=_T("");
	CString		strSQL=_T("");
	CString		strLog=_T("");

	MakeStringListByChar(paramList, strParas);
	POSITION pos=paramList.GetHeadPosition();
	while(pos)
	{
		CString	strTemp=paramList.GetNext(pos);
		if(strTemp.Find(__ServiceName__)==0)
		{
			strDBConn = strTemp.Right(strTemp.GetLength()-strlen(__ServiceName__));
		}
		else if(strTemp.Find(__USERACCOUNT__)==0)
		{
			strDBUser = strTemp.Right(strTemp.GetLength()-strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__)==0)
		{
			strDBPWD = strTemp.Right(strTemp.GetLength()-strlen(__PASSWORD__));
		}
	}
	if(strDBConn.IsEmpty())
	{
		sprintf(szReturn,"error=%s","�������ݿ�ʧ�ܣ�");
		return false;
	}
	strLog.Format("_ServiceName=%s_UserAccount=%s_PassWord=%s",strDBConn,strDBUser,strDBPWD);
	
////////////////////////////����������ݿ����м�¼�ĸ�����////////////////////////////////////////////

	string  code=_T("");				//��Ʒ����
	string  name=_T("");				//��Ʒ����
	string  fname=_T("");				//�ļ�����
	otl_datetime	base_time;	//��ƷӦ����ʱ��
	otl_datetime	ptime;		//Ԥ������
	otl_datetime	arrive_time;//��Ʒ����ʱ��
	long	arrive_number;		//��Ʒ�������
	string  status=_T("");				//״̬
	int		nFileNotArrive=0;		//û�е�����ļ�����
	char strPlanTime[128]={0};
	char strPtime[128]={0};		
	char strReachTime[128]={0};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	otl_connect db;
	otl_connect::otl_initialize();
	CString	strConn;
//	strSQL.Format("select * from MONITOR_PRODUCT_ARRIVEINFO where code like '%%%s%%' and fname like '%%%s%%'",strCode.GetBuffer(1),strFname.GetBuffer(1));
	strSQL.Format("%s","select * from MONITOR_PRODUCT_ARRIVEINFO");
//	strSQL.Format("%s","select * from monitor_serverfiles t");
	strConn.Format("Provider=OcaOLEDB.Olacle;UID=%s;PWD=%s;DSN=%s;QTO=F;APA=T",strDBUser,strDBPWD,strDBConn);
	CString strResult=_T("");
	try
	{
		db.rlogon(strConn.GetBuffer(1));
		otl_stream	o(1,strSQL,db);
		if(o.is_null())
		{
			sprintf(szReturn,"result=%s$FileNotArriveNum=%d$","��Ʒȫ�����",0);
			CString strInput=szReturn;
			MakeCharByString(szReturn,nSize,strInput);
			return TRUE;
		}
		while(!o.eof())
		{
			o>>code>>name>>fname>>base_time>>ptime>>arrive_time>>arrive_number>>status;
			sprintf(strPlanTime,"%d/%d%d-%d:%d:%d",base_time.year,base_time.month,base_time.day,base_time.hour,base_time.minute,base_time.second);
			sprintf(strPtime,"%d/%d/%d",ptime.year,ptime.month,ptime.day);
			sprintf(strReachTime,"%d/%d/%d-%d:%d:%d",arrive_time.year,arrive_time.month,arrive_time.day,arrive_time.hour,arrive_time.minute,arrive_time.second);
			sprintf(szReturn,",��Ʒ����=%s,��Ʒ����=%s,�ļ�����=%s,��ƷӦ����ʱ��=%s,Ԥ������=%s,��Ʒ����ʱ��=%s,��Ʒ�������=%ld,״̬=%s",code.c_str(),name.c_str(),fname.c_str(),strPlanTime,strPtime,strReachTime,arrive_number,status.c_str());
			nFileNotArrive++;
			strResult+=szReturn;
			
		}
		if(strResult.GetLength()>=1024)
		{
			sprintf(szReturn,"result=%s$FileNotArriveNum=%d$","��Ʒδ�������Ϣ���ݹ�����",nFileNotArrive);
		}
		else
		{
			sprintf(szReturn,"result=%s$FileNotArriveNum=%d$",strResult,nFileNotArrive);
		}
		
/*
		else if(nFileNotArrive>1)
		{
			sprintf(szReturn,"error=%s","����ļ�δ���£�");
			return false;
		}

		else
		{
			sprintf(szReturn,"error=%s","�ļ���Ϣ����");
		}
*/
	}
	catch(otl_exception& p)
	{
		sprintf(szReturn,"error=%s %s",p.msg,p.var_info);
		strLog.Format("GetOracleSelect Exception=%s ExceptionInfo=%s",p.msg,p.var_info);
		db.logoff();
		return	false;
	}
	catch(...)
	{
		sprintf(szReturn,"error=%s","Unknow Error");
		return false;
	}
	db.logoff();
	CString strInput=szReturn;
	MakeCharByString(szReturn, nSize, strInput);
	return	true;

}

/****************************************************************************
	Export Function Of DatabaseSourceNames (Fetch DSN)
****************************************************************************/
extern "C" __declspec(dllexport) 
BOOL SYSTEMDSN(const char * strParas, char * strReturn, int& nSize)
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

	return TRUE;
	
}

