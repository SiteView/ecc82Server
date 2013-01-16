// TuxedoGet.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "TuxedoGet.h"
#include "TuxedoMonitorFunc.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

BOOL  GetMachineList(const char *strParas, char * szReturn, int & nSize);
BOOL GetMachine(const char *strParas, char * szReturn, int & nSize);

BOOL GetServerList(const char *strParas, char * szReturn, int & nSize);
BOOL GetServer(const char *strParas, char * szReturn, int & nSize);

BOOL GetServiceList(const char *strParas, char * szReturn, int & nSize);
BOOL GetService(const char *strParas, char * szReturn, int & nSize);

void GetQueueList();
void GetQueue(CString strTaskParam);

BOOL GetMsgList(const char *strParas, char * szReturn, int & nSize);
BOOL GetMsg(const char *strParas, char * szReturn, int & nSize);

BOOL GetClientList(const char *strParas, char * szReturn, int & nSize);
BOOL GetClient(const char *strParas, char * szReturn, int & nSize);

BOOL GetWSHList(const char *strParas, char * szReturn, int & nSize);
BOOL GetWSH(const char *strParas, char * szReturn, int & nSize);


BOOL GetWSHAver(CString strMonitorID,long numblockq,long rcvdbyt,long rcvdnum,long sentbyt,long sentnum,
				long &numblockq_per_second,long &rcvdbyt_per_second,long &rcvdnum_per_second,long &sentbyt_per_second,long &sentnum_per_second);
BOOL GetMachineAver(CString strMonitorID,long wkcompleted,long wkinitiated,long &wkcompleted_per_second,long &wkinitiated_per_second);
BOOL GetServerAver(CString strMonitorID,long numreq,long numtrancmt,long &numreq_per_second,long &numtrancmt_per_second);

void GetAll();
void write(char * szPath,void *buff,int size);
void read(char * szPath,void *buff,int size);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int WriteLog(const char* str)
{

	char timeBuf[128], dateBuf[128], wyt[4096];
	
	_tzset();
	
	_strtime( timeBuf );
	_strdate( dateBuf );
	
    sprintf(wyt, "%s %s\t", dateBuf, timeBuf );
	
	FILE* log;
		
	log = fopen("TuxedoMonitor.log", "a+");
	
	if(log != NULL)
	{
		strcat(wyt, str);
		fprintf(log, "%s\n", wyt);
		fclose(log);
	}
	
	return 0;
}


/*
int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;
	
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}

	CString		strServIp = _T(""), 
				strServPort = _T(""), 
				strTaskType = _T(""),
				strTaskParam = _T("");

	strServIp = argv[1];
	strServPort = argv[2];
	strTaskType = argv[3];
	strTaskParam = argv[4];

	CString temp = "WSNADDR=//"+strServIp+":"+strServPort;
	OutputDebugString("--------WSNADDR output----------\n");
	OutputDebugString(temp.GetBuffer(temp.GetLength()));
	OutputDebugString("\n");
	char strServerip[100]={0};
	strcpy(strServerip,temp);

	//设置机器环境
	tuxputenv(strServerip);
	tuxputenv("WSENVFILE=");
	
	OutputDebugString("-------tuxedoget start----\n");
	//机器
	if(strTaskType == "T_MACHINE")
	{
		if(strTaskParam == "null")
			GetMachineList();
		else
			GetMachine(strTaskParam);
	}

	//服务
	else if(strTaskType == "T_SERVER")
	{
		OutputDebugString("-------tuxedoget t_server output----\n");
		if(strTaskParam == "null")
			GetServerList();
		else
			GetServer(strTaskParam);
	}
	
	//服务器
	else if(strTaskType == "T_SERVER")
	{
		OutputDebugString("-------tuxedoget t_server output----\n");
		if(strTaskParam == "null")
			GetServerList();
		else
			GetServer(strTaskParam);
	}

	//服务
	else if(strTaskType == "T_SERVICE")
	{
		if(strTaskParam == "null")
			GetServiceList();
		else
			GetService(strTaskParam);
	}

	//服务队列
	else if(strTaskType == "T_QUEUE")
	{
		if(strTaskParam == "null")
			GetQueueList();
		else
			GetQueue(strTaskParam);
	}
	
	//消息队列
	else if(strTaskType == "T_MSG")
	{
		if(strTaskParam == "null")
			GetMsgList();
		else
			GetMsg(strTaskParam);
	}

	//Client
	else if(strTaskType == "T_CLIENT")
	{
		if(strTaskParam == "null")
			GetClientList();
		else
			GetClient(strTaskParam);
	}

	//WSH
	else if(strTaskType == "T_WSH")
	{
		if(strTaskParam == "null")
			GetWSHList();
		else
			GetWSH(strTaskParam);
	}

	return nRetCode;
}
*/
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//测试
void GetAll()
{
	char strServerip[100] = "WSNADDR=//192.168.5.112:8899";
	tuxputenv(strServerip);

	//tpalloc
	long blen;
	FBFR32	*ibuf;
	ibuf = (FBFR32 *)tpalloc(FMLTYPE32,NULL,0);

	//fchg
	Fchg32(ibuf,TA_OPERATION,0,"GET",0);
	
	//Fchg32(ibuf,TA_CLASS,0,"T_DOMAIN",0);
	//Fchg32(ibuf,TA_CLASS,0,"T_MACHINE",0);
	
	//Fchg32(ibuf,TA_CLASS,0,"T_SERVER",0);
	//Fchg32(ibuf,TA_CLASS,0,"T_SVCGRP",0);
	//Fchg32(ibuf,TA_CLASS,0,"T_SERVICE",0);

	Fchg32(ibuf,TA_CLASS,0,"T_WSL",0);
	//Fchg32(ibuf,TA_CLASS,0,"T_WSH",0);
	 
	//Fchg32(ibuf,TA_CLASS,0,"T_TLOG",0);
	//Fchg32(ibuf,TA_CLASS,0,"T_ULOG",0);

	//Fchg32(ibuf,TA_CLASS,0,"T_TLISTEN",0);
	//Fchg32(ibuf,TA_CLASS,0,"T_CLIENT",0);
	
	//Fchg32(ibuf,TA_CLASS,0,"T_TRANSACTION",0);
	//Fchg32(ibuf,TA_CLASS,0,"T_GROUP",0);

	//Fchg32(ibuf,TA_CLASS,0,"T_QUEUE",0);
	//Fchg32(ibuf,TA_CLASS,0,"T_BRIDGE",0); 
	//Fchg32(ibuf,TA_CLASS,0,"T_CONN",0); 
	//Fchg32(ibuf,TA_CLASS,0,"T_DEVICE",0);
	//Fchg32(ibuf,TA_CLASS,0,"T_FACTORY",0);
	//Fchg32(ibuf,TA_CLASS,0,"T_IFQUEUE",0);
	//Fchg32(ibuf,TA_CLASS,0,"T_INTERFACE",0);
	//Fchg32(ibuf,TA_CLASS,0,"T_MSG",0);
	//Fchg32(ibuf,TA_CLASS,0,"T_NETGROUP",0);
	//Fchg32(ibuf,TA_CLASS,0,"NETMAP",0);
	//Fchg32(ibuf,TA_CLASS,0,"T_SERVERCTXT",0);
	

	//tpcall
	if (tpcall(".TMIB",(char *)ibuf,0,(char **)&ibuf,&blen,0)== -1){
		printf("tpcall(.TMIB) failed:%s\n",tpstrerror(tperrno));
		//Fprint32(ibuf);
		return;
	}

	//print
//	Fprint32(ibuf);

	//exit
	tpfree((char *)ibuf);
	tpterm();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//机器操作

//得到机器参数(strTaskParam == 机器名称)
BOOL GetMachine(const char *strParas, char * szReturn, int & nSize)
{
	FBFR32	*buff = NULL;
	buff = (FBFR32 *)tpalloc(FMLTYPE32,NULL,1024);

	//fchg
	Fchg32(buff,TA_OPERATION,0,"GET",0);
	Fchg32(buff,TA_CLASS,0,"T_MACHINE",0);
	
	long lFilter=TA_PMID;
	//Fchg32(buff, TA_FILTER, 0, (char *)&lFilter, 0);	
	//tpcall
		
	CString		strServIp = FindStrValue(strParas, "_ServerIp");
	CString		strServPort = FindStrValue(strParas, "_ServerPort");
	CString strServerip = "WSNADDR=//"+strServIp+":"+strServPort;

	::Sleep(100);

	//设置机器环境
	tuxputenv((LPTSTR)(LPCTSTR)strServerip);
	tuxputenv("WSENVFILE=");

	/*连接TUXEDO */
	if(tpinit((TPINIT*)NULL)==-1) 
	{
		strcpy(szReturn,"error=init error$");
		return FALSE;
	}

	WriteLog("=========== Machine =============");
	WriteLog(strServerip.GetBuffer());
	WriteLog(tuxgetenv("WSNADDR"));


	long Len = 0;
	if (tpcall(".TMIB",(char *)buff,0,(char **)&buff,&Len,0)== -1)
	{
		CString strReturn="";
		strReturn.Format("tpcall(.TMIB) failed:%s\n",tpstrerror(tperrno));
		//Fprint32(buff);
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,strReturn);
		return FALSE;
	}


	//print
	//Fprint32(buff);

	long Num;
	Num = Foccur32(buff,lFilter);
	if(Num<1)
	{
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,"error=Get data error$");
		return FALSE;
	}

	//print
	//Fprint32(buff);
	char machinename[256]={0};
	char state[256]={0};
	char role[256]={0};
	long numtran=0;
	long numtrancmt=0;
	long wkcompleted = 0;
	long wkinitiated = 0;

	long wkcompleted_per_second = 0;
	long wkinitiated_per_second = 0;

	CString strTaskParam = FindStrValue(strParas, "_TaskParam");
	CString strMonitorID = FindStrValue(strParas, "_MonitorID");

    for(int i=0;i<Num;i++)
	{
		 memset(state,0,256);

		 Fget32(buff,TA_PMID,i,machinename,NULL);

		 char strTemp[1024];
		 sprintf(strTemp,"getmachinename=%s,need=%s",machinename,strTaskParam.GetBuffer());

		 WriteLog(strTemp);

		if(_stricmp(machinename,strTaskParam)==0)
		{
			Fget32(buff,TA_STATE,i,state,NULL);
			Fget32(buff,TA_ROLE,i,role,NULL);

			Fget32(buff,TA_NUMTRAN,i,(char *)&numtran,NULL);
			Fget32(buff,TA_NUMTRANCMT,i,(char *)&numtrancmt,NULL);

			Fget32(buff,TA_WKCOMPLETED,i,(char *)&wkcompleted,NULL);
			Fget32(buff,TA_WKINITIATED,i,(char *)&wkinitiated,NULL);

			//wk参数处理
			if(!GetMachineAver(strMonitorID, wkcompleted,wkinitiated,wkcompleted_per_second,wkinitiated_per_second))
			{
				Sleep(1000);		
				GetMachine(strParas, szReturn, nSize);
			}

			CString temp;
			CString text;

			temp.Format("role=%s$",role);
			text = temp;

			temp.Format("state=%s$",state);
			text += temp;
			temp.Format("numtran=%d$",numtran);
			text += temp;
			temp.Format("numtrancmt=%d$",numtrancmt);
			text += temp;
			temp.Format("wkcompleted=%d$",wkcompleted);
			text += temp;
			temp.Format("wkinitiated=%d$",wkinitiated);
			text += temp;
	
			temp.Format("wkcompleted_per_second=%d$",wkcompleted_per_second);
			text += temp;
			temp.Format("wkinitiated_per_second=%d$",wkinitiated_per_second);
			text += temp;

			//加标志
			//text = "FileSplitFlag"+text;
			//text += "EndSplitFlag";

			strcpy(szReturn,text);

			tpfree((char *)buff);
		    tpterm();
			return TRUE;
		}
	}

	strcpy(szReturn,"error=no data$");
	tpfree((char *)buff);
	tpterm();
	return FALSE;
}

//得到机器列表

BOOL  GetMachineList(const char *strParas, char * szReturn, int & nSize)
{

	if(tpinit((TPINIT*)NULL)==-1) /*连接TUXEDO */
	{
		strcpy(szReturn,"error=init error$");
		return FALSE;
	}

	//tpalloc
	FBFR32	*buff;
	buff = (FBFR32 *)tpalloc(FMLTYPE32,NULL,0);

	//fchg
	Fchg32(buff,TA_OPERATION,0,"GET",0);
	Fchg32(buff,TA_CLASS,0,"T_MACHINE",0);
	
	long lFilter=TA_PMID;
	Fchg32(buff, TA_FILTER, 0, (char *)&lFilter, 0);


	//tpcall
	long Len;
	if (tpcall(".TMIB",(char *)buff,0,(char **)&buff,&Len,0)== -1)
	{
		CString strReturn="";
		strReturn.Format("error=tpcall(.TMIB) failed:%s\n",tpstrerror(tperrno));
		//Fprint32(buff);
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,strReturn);
		return FALSE;
	}

	//print
	//Fprint32(buff);

	long Num;
	Num = Foccur32(buff,lFilter);
	if(Num<1)
	{
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,"error=Get data error$");
		return FALSE;
	}

	
	CString text;
	CString Temp;
	for(int i=0; i<Num; i++)
	{
		char temp[256]={0};
		Fget32(buff,lFilter,i,temp,NULL);

		Temp = temp;
		Temp += "=";
		Temp += temp;
		Temp += "$";
		//Temp = "machinename="+ Temp1 + "*@";

		text += Temp;
	}
	//加标志
	//text = "FileSplitFlag"+text;
	//text += "EndSplitFlag";

	strcpy(szReturn,text);

	//exit
	tpfree((char *)buff);
	tpterm();

	return TRUE;

}

//保存本次数据并计算平均值
BOOL GetMachineAver(CString strMonitorID, long wkcompleted,long wkinitiated,long &wkcompleted_per_second,long &wkinitiated_per_second)
{
	long buff[3] = {0}; 

	CString strPath;
	#if _DEBUG  
		strPath.Format(".\\MachineParam_%s.ini", strMonitorID);
	#else  
		strPath.Format("..\\data\\TmpIniFile\\MachineParam_%s.ini", strMonitorID);
	#endif  


	read(strPath.GetBuffer(strPath.GetLength()), buff,sizeof(long));

	//得到上一次
	long Interval0 = buff[0];
	long wkcompleted0 = buff[1];
	long wkinitiated0 = buff[2];

	//保存这次的数据
	CTime t1(1970,7,1,0,0,0);
	CTime t2 = CTime::GetCurrentTime();
	CTimeSpan ts = t2 - t1; 
	long Interval1 = ts.GetTotalSeconds();

	buff[0] = Interval1;
	buff[1] = wkcompleted;
	buff[2] = wkinitiated;

	write(strPath.GetBuffer(strPath.GetLength()),buff,sizeof(long));

	//判断是否已经保存过数据
	if(Interval0 <= 0)
		return FALSE;

	//两次时间间隔
	long Interval = Interval1 - Interval0;
	if(Interval <= 0)
		return FALSE;
	
	//两次数据差
	wkcompleted -= wkcompleted0;
	wkinitiated -= wkinitiated0;
	
	if(wkcompleted > 0)
		wkcompleted_per_second = wkcompleted/Interval;

	if(wkinitiated > 0)
		wkinitiated_per_second = wkinitiated/Interval;
	

	return TRUE;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//服务器操作


BOOL GetServer(const char *strParas, char * szReturn, int & nSize)
{
	OutputDebugString("-----------start GetServer() call----------\n");
	FBFR32	*buff = NULL;
	buff = (FBFR32 *)tpalloc(FMLTYPE32,NULL,1024);

	//fchg
	Fchg32(buff,TA_OPERATION,0,"GET",0);
	Fchg32(buff,TA_CLASS,0,"T_SERVER",0);
	
	long lFilter=TA_SERVERNAME;
	//Fchg32(buff, TA_FILTER, 0, (char *)&lFilter, 0);	
	//tpcall
		
	CString		strServIp = FindStrValue(strParas, "_ServerIp");
	CString		strServPort = FindStrValue(strParas, "_ServerPort");
	CString strServerip = "WSNADDR=//"+strServIp+":"+strServPort;

	::Sleep(100);

	//设置机器环境
	tuxputenv((LPTSTR)(LPCTSTR)strServerip);
	tuxputenv("WSENVFILE=");

	/*连接TUXEDO */
	if(tpinit((TPINIT*)NULL)==-1) 
	{
		strcpy(szReturn,"error=init error$");
		return FALSE;
	}

	WriteLog("=========== Server =============");
	WriteLog(strServerip.GetBuffer());
	WriteLog(tuxgetenv("WSNADDR"));


	long Len = 0;
	if (tpcall(".TMIB",(char *)buff,0,(char **)&buff,&Len,0)== -1)
	{
		CString strReturn="";
		strReturn.Format("tpcall(.TMIB) failed:%s\n",tpstrerror(tperrno));
		//Fprint32(buff);
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,strReturn);
		return FALSE;
	}

	//print
	//Fprint32(buff);

	long Num;
	Num = Foccur32(buff,lFilter);
	if(Num<1)
	{
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,"error=Get data error$");
		return FALSE;
	}

	//print
	//Fprint32(buff);
	char Servername[256]={0};
	char state[256]={0};
	char clopt[256]={0};
	long numreq=0;
	long numtrancmt=0;
	long curconv = 0;

	long numreq_per_second = 0;
	long numtrancmt_per_second = 0;

	CString strTaskParam = FindStrValue(strParas, "_TaskParam");
	CString strMonitorID = FindStrValue(strParas, "_MonitorID");

    for(int i=0;i<Num;i++)
	{
		 memset(state,0,256);

		 Fget32(buff,TA_SERVERNAME,i,Servername,NULL);
		
		 OutputDebugString("---------Server name output---------\n");
		 char dbuf[256] = {0};
		 sprintf(dbuf, "Servername:%s strTaskParam:%s\n", Servername, strTaskParam.GetBuffer(strTaskParam.GetLength()));
		 OutputDebugString(dbuf);
		 OutputDebugString("\n");

		 char strTemp[1024];
		 sprintf(strTemp,"getServername=%s,need=%s",Servername,strTaskParam.GetBuffer());

		 WriteLog(strTemp);

		
		if(_stricmp(Servername,strTaskParam)==0)
		{
			OutputDebugString("--------find servername---------\n");
			Fget32(buff,TA_STATE,i,state,NULL);
			Fget32(buff,TA_TOTREQC,i,(char *)&numreq,NULL);
			Fget32(buff,TA_TOTWORKL,i,(char *)&numtrancmt,NULL);
			Fget32(buff,TA_CURCONV,i,(char *)&curconv,NULL);
			//wk参数处理
			if(!GetServerAver(strMonitorID,numreq,numtrancmt,numreq_per_second,numtrancmt_per_second))
			{
				Sleep(1000);		
				GetServer(strParas, szReturn, nSize);
			}

			CString temp;
			CString text;

			temp.Format("state=%s$",state);
			text += temp;
			temp.Format("numreq=%d$",numreq);
			text += temp;
			temp.Format("numtrancmt=%d$",numtrancmt);
			text += temp;
			temp.Format("numreq_per_second=%d$",numreq_per_second);
			text += temp;
			temp.Format("numtrancmt_per_second=%d$",numtrancmt_per_second);
			text += temp;
			temp.Format("curconv=%d",curconv);
			text += temp;

			//加标志
			//text = "FileSplitFlag"+text;
			//text += "EndSplitFlag";

			OutputDebugString("-----------getserver return string----------\n");
			OutputDebugString(text.GetBuffer(text.GetLength()));
			strcpy(szReturn,text);

			tpfree((char *)buff);
		    tpterm();
			return TRUE;
		}
	}

	strcpy(szReturn,"error=no data$");
	tpfree((char *)buff);
	tpterm();
	return FALSE;

}

//得到服务列表
BOOL GetServerList(const char *strParas, char * szReturn, int & nSize)
{
	if(tpinit((TPINIT*)NULL)==-1) /*连接TUXEDO */
	{
		strcpy(szReturn,"error=init error$");
		return FALSE;

	}

	//tpalloc
	FBFR32	*buff;
	buff = (FBFR32 *)tpalloc(FMLTYPE32,NULL,0);

	//fchg
	Fchg32(buff,TA_OPERATION,0,"GET",0);
	Fchg32(buff,TA_CLASS,0,"T_SERVER",0);
	
	long lFilter=TA_SERVERNAME;
	Fchg32(buff, TA_FILTER, 0, (char *)&lFilter, 0);

	//tpcall
	long Len;
	if (tpcall(".TMIB",(char *)buff,0,(char **)&buff,&Len,0)== -1)
	{
		CString strReturn="";
		strReturn.Format("tpcall(.TMIB) failed:%s\n",tpstrerror(tperrno));
		//Fprint32(buff);
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,strReturn);
		return FALSE;
	}

	long Num;
	Num = Foccur32(buff,lFilter);
	if(Num<1)
	{
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,"error=Get data error$");
		return FALSE;
	}

	CString text;
	CString Temp;
	CString Temp1;
	for(int i=0; i<Num; i++)
	{
		char temp[256]={0};
		Fget32(buff,lFilter,i,temp,NULL);

		Temp1 = temp;
		Temp = temp;
		Temp += "=";
		Temp += Temp1;
		Temp += "$";
		//Temp = "Servername="+ Temp1 + "*@";

		text += Temp;
	}
	//加标志
	//text = "FileSplitFlag"+text;
	//text += "EndSplitFlag";

	strcpy(szReturn,text);

	//exit
	tpfree((char *)buff);
	tpterm();

	return TRUE;
}

//保存本次数据并计算平均值
BOOL GetServerAver(CString strMonitorID, long numreq,long numtrancmt,long &numreq_per_second,long &numtrancmt_per_second)
{
	long buff[3] = {0}; 

	CString strPath;
	#if _DEBUG  
		strPath.Format(".\\ServerParam_%s.ini", strMonitorID);
	#else  
		strPath.Format("..\\data\\TmpIniFile\\ServerParam_%s.ini", strMonitorID);
	#endif  

	read(strPath.GetBuffer(strPath.GetLength()),buff,sizeof(long));

	//得到上一次
	long Interval0 = buff[0];
	long numreq0 = buff[1];
	long numtrancmt0 = buff[2];

	//保存这次的数据
	CTime t1(1970,7,1,0,0,0);
	CTime t2 = CTime::GetCurrentTime();
	CTimeSpan ts = t2 - t1; 
	long Interval1 = ts.GetTotalSeconds();

	buff[0] = Interval1;
	buff[1] = numreq;
	buff[2] = numtrancmt;

	write(strPath.GetBuffer(strPath.GetLength()),buff,sizeof(long));

	//判断是否已经保存过数据
	if(Interval0 <= 0)
		return FALSE;

	//两次时间间隔
	long Interval = Interval1 - Interval0;
	if(Interval <= 0)
		return FALSE;
	
	//两次数据差
	numreq -= numreq0;
	numtrancmt -= numtrancmt0;
	
	//计算平均值
	if(numreq > 0)
		numreq_per_second = numreq/Interval;

	if(numtrancmt > 0)
		numtrancmt_per_second = numtrancmt/Interval;


	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//服务操作
BOOL GetService(const char *strParas, char * szReturn, int & nSize)
{
	FBFR32	*buff = NULL;
	buff = (FBFR32 *)tpalloc(FMLTYPE32,NULL,1024);

	//fchg
	Fchg32(buff,TA_OPERATION,0,"GET",0);
	Fchg32(buff,TA_CLASS,0,"T_SERVICE",0);
	
	long lFilter=TA_SERVICENAME;
	//Fchg32(buff, TA_FILTER, 0, (char *)&lFilter, 0);	
	//tpcall
	

	CString		strServIp = FindStrValue(strParas, "_ServerIp");
	CString		strServPort = FindStrValue(strParas, "_ServerPort");
	CString strServerip = "WSNADDR=//"+strServIp+":"+strServPort;

	::Sleep(100);

	//设置机器环境
	tuxputenv((LPTSTR)(LPCTSTR)strServerip);
	tuxputenv("WSENVFILE=");

	/*连接TUXEDO */
	if(tpinit((TPINIT*)NULL)==-1) 
	{
		strcpy(szReturn,"error=init error$");
		return FALSE;
	}

	WriteLog("============ Service ============");
	WriteLog(strServerip.GetBuffer());
	WriteLog(tuxgetenv("WSNADDR"));

	long Len = 0;
	if (tpcall(".TMIB",(char *)buff,0,(char **)&buff,&Len,0)== -1)
	{
		CString strReturn="";
		strReturn.Format("tpcall(.TMIB) failed:%s\n",tpstrerror(tperrno));
		//Fprint32(buff);
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,strReturn);
		return FALSE;
	}

	//print
	//Fprint32(buff);

	long Num;
	Num = Foccur32(buff,lFilter);
	if(Num<1)
	{
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,"error=Get data error$");
		return FALSE;
	}

	//print
	//Fprint32(buff);
	char Servicename[256]={0};
	char state[256]={0};
	long timeout =0;

	long numreq_per_second = 0;
	long numtrancmt_per_second = 0;

	CString strTaskParam = FindStrValue(strParas, "_TaskParam");

    for(int i=0;i<Num;i++)
	{
		 memset(state,0,256);

		 Fget32(buff,TA_SERVICENAME,i,Servicename,NULL);

		 char strTemp[1024];
		 sprintf(strTemp,"getServicename=%s,need=%s",Servicename,strTaskParam.GetBuffer());

		 WriteLog(strTemp);


		if(_stricmp(Servicename,strTaskParam)==0)
		{
			Fget32(buff,TA_STATE,i,state,NULL);
			Fget32(buff,TA_SVCTIMEOUT,i,(char *)&timeout,NULL);
			
			CString temp;
			CString text;

			temp.Format("state=%s$",state);
			text += temp;
			temp.Format("timeout=%d$",timeout);
			text += temp;

			//加标志
			//text = "FileSplitFlag"+text;
			//text += "EndSplitFlag";

			strcpy(szReturn,text);

			tpfree((char *)buff);
		  tpterm();
			return TRUE;
		}
	}

	strcpy(szReturn,"error=no data$");
	tpfree((char *)buff);
	tpterm();
	return FALSE;

}

//得到服务列表
BOOL GetServiceList(const char *strParas, char * szReturn, int & nSize)
{	
	if(tpinit((TPINIT*)NULL)==-1) /*连接TUXEDO */
	{
		strcpy(szReturn,"error=init error$");
		return FALSE;
	}

	//tpalloc
	FBFR32	*buff;
	buff = (FBFR32 *)tpalloc(FMLTYPE32,NULL,0);

	//fchg
	Fchg32(buff,TA_OPERATION,0,"GET",0);
	Fchg32(buff,TA_CLASS,0,"T_SERVICE",0);
	
	long lFilter=TA_SERVICENAME;
	Fchg32(buff, TA_FILTER, 0, (char *)&lFilter, 0);

	//tpcall
	long Len;
	if (tpcall(".TMIB",(char *)buff,0,(char **)&buff,&Len,0)== -1)
	{
		CString strReturn="";
		strReturn.Format("tpcall(.TMIB) failed:%s\n",tpstrerror(tperrno));
		//Fprint32(buff);
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,strReturn);
		return FALSE;
	}

	long Num;
	Num = Foccur32(buff,lFilter);
	if(Num<1)
	{
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,"error=Get data error$");
		return FALSE;
	}

	CString text;
	CString Temp;
	CString Temp1;
	for(int i=0; i<Num; i++)
	{
		char temp[256]={0};
		Fget32(buff,lFilter,i,temp,NULL);

		Temp1 = temp;
		Temp = temp;
		Temp += "=";
		Temp += Temp1;
		Temp += "$";
		//Temp = "Servicename="+ Temp1 + "*@";

		text += Temp;
	}
	//加标志
	//text = "FileSplitFlag"+text;
	//text += "EndSplitFlag";

	strcpy(szReturn,text);

	//exit
	tpfree((char *)buff);
	tpterm();

	return TRUE;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//服务队列操作

//得到机器参数(strTaskParam == 机器名称)
void GetQueue(CString strTaskParam)
{
	FBFR32	*buff = NULL;
	buff = (FBFR32 *)tpalloc(FMLTYPE32,NULL,1024);

	//fchg
	Fchg32(buff,TA_OPERATION,0,"GET",0);
	Fchg32(buff,TA_CLASS,0,"T_QUEUE",0);
	
	long lFilter=TA_SERVERNAME;
	//Fchg32(buff, TA_FILTER, 0, (char *)&lFilter, 0);	
	//tpcall
		
	/*连接TUXEDO */
	if(tpinit((TPINIT*)NULL)==-1) 
	{
		puts("error=init error$");
		//exit(1);
	}

	long Len = 0;
	if (tpcall(".TMIB",(char *)buff,0,(char **)&buff,&Len,0)== -1)
	{
		printf("tpcall(.TMIB) failed:%s\n",tpstrerror(tperrno));
		tpfree((char *)buff);
		//Fprint32(buff);
		return;
	}

	//print
	//Fprint32(buff);

	long Num;
	Num = Foccur32(buff,lFilter);
	if(Num<1)
	{
		tpfree((char *)buff);
		tpterm();
		printf("error=Get data error\n");
		return;
	}

	//print
	//Fprint32(buff);
	char Queuename[256]={0};
	char state[256]={0};
	long totnqueued=0;
	long totwkqueued=0;

    for(int i=0;i<Num;i++)
	{
		 memset(state,0,256);

		 Fget32(buff,TA_SERVERNAME,i,Queuename,NULL);

		if(_stricmp(Queuename,strTaskParam)==0)
		{
			Fget32(buff,TA_STATE,i,state,NULL);
			Fget32(buff,TA_TOTNQUEUED,i,(char *)&totnqueued,NULL);
			Fget32(buff,TA_TOTWKQUEUED,i,(char *)&totwkqueued,NULL);

			CString temp;
			CString text;

			temp.Format("state=%s$",state);
			text += temp;
			temp.Format("totnqueued=%d$",totnqueued);
			text += temp;
			temp.Format("totwkqueued=%d$",totwkqueued);

			//加标志
			text = "FileSplitFlag"+text;
			text += "EndSplitFlag";

			printf(text);

			tpfree((char *)buff);
		    tpterm();
			return;
		}
	}

	puts("error=no data$");
	tpfree((char *)buff);
	tpterm();

}

//得到服务列表
void GetQueueList()
{
	if(tpinit((TPINIT*)NULL)==-1) /*连接TUXEDO */
	{
		puts("error=init error$");
		//exit(1);
	}

	//tpalloc
	FBFR32	*buff;
	buff = (FBFR32 *)tpalloc(FMLTYPE32,NULL,0);

	//fchg
	Fchg32(buff,TA_OPERATION,0,"GET",0);
	Fchg32(buff,TA_CLASS,0,"T_QUEUE",0);
	
	long lFilter=TA_SERVERNAME;
	Fchg32(buff, TA_FILTER, 0, (char *)&lFilter, 0);

	//tpcall
	long Len;
	if (tpcall(".TMIB",(char *)buff,0,(char **)&buff,&Len,0)== -1)
	{
		printf("tpcall(.TMIB) failed:%s\n",tpstrerror(tperrno));
		//Fprint32(buff);
		tpfree((char *)buff);
		tpterm();
		return;
	}

	long Num;
	Num = Foccur32(buff,lFilter);
	if(Num<1)
	{
		tpfree((char *)buff);
		tpterm();
		printf("error=Get data error\n");
		return;
	}

	CString text;
	CString Temp;
	CString Temp1;
	for(int i=0; i<Num; i++)
	{
		char temp[256]={0};
		Fget32(buff,lFilter,i,temp,NULL);

		Temp1 = temp;
		Temp = temp;
		Temp += "=";
		Temp += Temp1;
		Temp += "*@";
		//Temp = "Queuename="+ Temp1 + "*@";

		text += Temp;
	}
	//加标志
	text = "FileSplitFlag"+text;
	text += "EndSplitFlag";

	printf(text);

	//exit
	tpfree((char *)buff);
	tpterm();

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//消息队列操作

//得到机器参数(strTaskParam == 机器名称)
BOOL GetMsg(const char *strParas, char * szReturn, int & nSize)
{
	FBFR32	*buff = NULL;
	buff = (FBFR32 *)tpalloc(FMLTYPE32,NULL,1024);

	//fchg
	Fchg32(buff,TA_OPERATION,0,"GET",0);
	Fchg32(buff,TA_CLASS,0,"T_MSG",0);
	
	long lFilter=TA_MSGID;
	//Fchg32(buff, TA_FILTER, 0, (char *)&lFilter, 0);	
	//tpcall
	
	CString		strServIp = FindStrValue(strParas, "_ServerIp");
	CString		strServPort = FindStrValue(strParas, "_ServerPort");
	CString strServerip = "WSNADDR=//"+strServIp+":"+strServPort;

	::Sleep(100);

	//设置机器环境
	tuxputenv((LPTSTR)(LPCTSTR)strServerip);
	tuxputenv("WSENVFILE=");

	/*连接TUXEDO */
	if(tpinit((TPINIT*)NULL)==-1) 
	{
		strcpy(szReturn,"error=init error$");
		return FALSE;
	}

	WriteLog("=========== Msg =============");
	WriteLog(strServerip.GetBuffer());
	WriteLog(tuxgetenv("WSNADDR"));

	long Len = 0;
	if (tpcall(".TMIB",(char *)buff,0,(char **)&buff,&Len,0)== -1)
	{
		CString strReturn="";
		strReturn.Format("tpcall(.TMIB) failed:%s\n",tpstrerror(tperrno));
		//Fprint32(buff);
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,strReturn);
		return FALSE;
	}

	//print
	//Fprint32(buff);

	long Num;
	Num = Foccur32(buff,lFilter);
	if(Num<1)
	{
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,"error=Get data error$");
		return FALSE;
	}

	//print
	//Fprint32(buff);
	long MsgId = 0;
	CString sMsgId = "";
	char state[256]={0};
	long qbytes=0;
	long cbytes = 0;
	long qnum=0;

	CString strTaskParam = FindStrValue(strParas, "_TaskParam");

    for(int i=0;i<Num;i++)
	{
		memset(state,0,256);

		Fget32(buff,lFilter,i,(char *)&MsgId,NULL);
		sMsgId.Format("%d",MsgId);

		char strTemp[1024];
		 sprintf(strTemp,"getMsgId=%s,need=%s",sMsgId.GetBuffer(),strTaskParam.GetBuffer());

		 WriteLog(strTemp);


		if(_stricmp(sMsgId,strTaskParam)==0)
		{
			Fget32(buff,TA_STATE,i,state,NULL);
			Fget32(buff,TA_MSG_CBYTES,i,(char *)&cbytes,NULL);
			Fget32(buff,TA_MSG_QBYTES,i,(char *)&qbytes,NULL);
			Fget32(buff,TA_MSG_QNUM,i,(char *)&qnum,NULL);

			CString temp;
			CString text;

			temp.Format("state=%s$",state);
			text += temp;
			temp.Format("qbytes=%d$",qbytes);
			text += temp;
			temp.Format("cbytes=%d$",cbytes);
			text += temp;
			temp.Format("qnum=%d$",qnum);
			text += temp;

			//加标志
			//text = "FileSplitFlag"+text;
			//text += "EndSplitFlag";

			strcpy(szReturn,text);

			tpfree((char *)buff);
			tpterm();
			return TRUE;
		}
	}

	strcpy(szReturn,"error=no data$");
	tpfree((char *)buff);
	tpterm();
	return FALSE;

}

//得到服务列表
BOOL GetMsgList(const char *strParas, char * szReturn, int & nSize)
{
	if(tpinit((TPINIT*)NULL)==-1) /*连接TUXEDO */
	{
		strcpy(szReturn,"error=init error$");
		return FALSE;
	}

	//tpalloc
	FBFR32	*buff;
	buff = (FBFR32 *)tpalloc(FMLTYPE32,NULL,0);

	//fchg
	Fchg32(buff,TA_OPERATION,0,"GET",0);
	Fchg32(buff,TA_CLASS,0,"T_MSG",0);
	
	long lFilter=TA_MSGID;
	Fchg32(buff, TA_FILTER, 0, (char *)&lFilter, 0);

	//tpcall
	long Len;
	if (tpcall(".TMIB",(char *)buff,0,(char **)&buff,&Len,0)== -1)
	{
		CString strReturn="";
		strReturn.Format("tpcall(.TMIB) failed:%s\n",tpstrerror(tperrno));
		//Fprint32(buff);
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,strReturn);
		return FALSE;
	}

	long Num;
	Num = Foccur32(buff,lFilter);
	if(Num<1)
	{
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,"error=Get data error$");
		return FALSE;
	}

	CString text;
	CString Temp;
	CString Temp1;
	for(int i=0; i<Num; i++)
	{
		long temp = 0;

		Fget32(buff,lFilter,i,(char *)&temp,NULL);

		Temp1.Format("%d",temp);
		Temp = Temp1;
		Temp += "=";
		Temp += Temp1;
		Temp += "$";
	//	Temp = "msgname="+ Temp1 + "*@";

		text += Temp;
	}
	//加标志
	//text = "FileSplitFlag"+text;
	//text += "EndSplitFlag";

	strcpy(szReturn,text);

	//exit
	tpfree((char *)buff);
	tpterm();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Client操作

//得到机器参数(strTaskParam == 机器名称)
BOOL GetClient(const char *strParas, char * szReturn, int & nSize)
{
	FBFR32	*buff = NULL;
	buff = (FBFR32 *)tpalloc(FMLTYPE32,NULL,1024);

	//fchg
	Fchg32(buff,TA_OPERATION,0,"GET",0);
	Fchg32(buff,TA_CLASS,0,"T_CLIENT",0);
	
	long lFilter=TA_CLTNAME;
	//Fchg32(buff, TA_FILTER, 0, (char *)&lFilter, 0);	
	//tpcall
	
	CString		strServIp = FindStrValue(strParas, "_ServerIp");
	CString		strServPort = FindStrValue(strParas, "_ServerPort");
	CString strServerip = "WSNADDR=//"+strServIp+":"+strServPort;

	::Sleep(100);

	//设置机器环境
	tuxputenv((LPTSTR)(LPCTSTR)strServerip);
	tuxputenv("WSENVFILE=");

	/*连接TUXEDO */
	if(tpinit((TPINIT*)NULL)==-1) 
	{
		strcpy(szReturn,"error=init error$");
		return FALSE;
	}

	WriteLog("=========== Client =============");
	WriteLog(strServerip.GetBuffer());
	WriteLog(tuxgetenv("WSNADDR"));

	long Len = 0;
	if (tpcall(".TMIB",(char *)buff,0,(char **)&buff,&Len,0)== -1)
	{
		CString strReturn="";
		strReturn.Format("tpcall(.TMIB) failed:%s\n",tpstrerror(tperrno));
		//Fprint32(buff);
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,strReturn);
		return FALSE;
	}

	//print
	//Fprint32(buff);

	long Num;
	Num = Foccur32(buff,lFilter);
	if(Num<1)
	{
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,"error=Get data error$");
		return FALSE;
	}

	//print
	//Fprint32(buff);
	char clientname[256]={0};
	char state[256]={0};
	long numreq=0;
	long numtrancmt = 0;

	CString strTaskParam = FindStrValue(strParas, "_TaskParam");

    for(int i=0;i<Num;i++)
	{
		memset(state,0,256);

		Fget32(buff,lFilter,i,clientname,NULL);

		char strTemp[1024];
		sprintf(strTemp,"getclientname=%s,need=%s",clientname,strTaskParam.GetBuffer());

		WriteLog(strTemp);


		if(_stricmp(clientname,strTaskParam)==0)
		{
			Fget32(buff,TA_STATE,i,state,NULL);
			Fget32(buff,TA_NUMREQ,i,(char *)&numreq,NULL);
			Fget32(buff,TA_NUMTRANCMT,i,(char *)&numtrancmt,NULL);

			CString temp;
			CString text;

			temp.Format("state=%s$",state);
			text += temp;
			temp.Format("numreq=%d$",numreq);
			text += temp;
			temp.Format("numtrancmt=%d$",numtrancmt);
			text += temp;

			//加标志
			//text = "FileSplitFlag"+text;
			//text += "EndSplitFlag";

			strcpy(szReturn,text);

			tpfree((char *)buff);
			tpterm();
			return TRUE;
		}
	}

	strcpy(szReturn,"error=no data$");
	tpfree((char *)buff);
	tpterm();
	return FALSE;

}

//得到服务列表
BOOL GetClientList(const char *strParas, char * szReturn, int & nSize)
{
	if(tpinit((TPINIT*)NULL)==-1) /*连接TUXEDO */
	{
		strcpy(szReturn,"error=init error$");
		return FALSE;
	}

	//tpalloc
	FBFR32	*buff;
	buff = (FBFR32 *)tpalloc(FMLTYPE32,NULL,0);

	//fchg
	Fchg32(buff,TA_OPERATION,0,"GET",0);
	Fchg32(buff,TA_CLASS,0,"T_CLIENT",0);
	
	long lFilter=TA_CLTNAME;
	Fchg32(buff, TA_FILTER, 0, (char *)&lFilter, 0);

	//tpcall
	long Len;
	if (tpcall(".TMIB",(char *)buff,0,(char **)&buff,&Len,0)== -1)
	{
		CString strReturn="";
		strReturn.Format("tpcall(.TMIB) failed:%s\n",tpstrerror(tperrno));
		//Fprint32(buff);
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,strReturn);
		return FALSE;
	}
	

	long Num;
	Num = Foccur32(buff,lFilter);
	if(Num<1)
	{
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,"error=Get data error$");
		return FALSE;
	}

	CString text;
	CString Temp;
	CString Temp1;
	for(int i=0; i<Num; i++)
	{
		char temp[256]={0};

		Fget32(buff,lFilter,i,temp,NULL);
		if(strcmp(temp, "") != 0)
		{
			Temp1 = temp;
			Temp = temp;
			Temp += "=";
			Temp += Temp1;
			Temp += "$";
			//Temp = "clientname="+ Temp1 + "*@";

			text += Temp;
		}
	}
	//加标志
	//text = "FileSplitFlag"+text;
	//text += "EndSplitFlag";

	strcpy(szReturn,text);

	//exit
	tpfree((char *)buff);
	tpterm();
	return TRUE;

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//WSH操作

BOOL GetWSH(const char *strParas, char * szReturn, int & nSize)
{
	FBFR32	*buff = NULL;
	buff = (FBFR32 *)tpalloc(FMLTYPE32,NULL,1024);

	//fchg
	Fchg32(buff,TA_OPERATION,0,"GET",0);
	Fchg32(buff,TA_CLASS,0,"T_WSH",0);
	
	long lFilter = TA_PID;
	//Fchg32(buff, TA_FILTER, 0, (char *)&lFilter, 0);	
	//tpcall
	

	CString		strServIp = FindStrValue(strParas, "_ServerIp");
	CString		strServPort = FindStrValue(strParas, "_ServerPort");
	CString strServerip = "WSNADDR=//"+strServIp+":"+strServPort;

	::Sleep(100);

	//设置机器环境
	tuxputenv((LPTSTR)(LPCTSTR)strServerip);
	tuxputenv("WSENVFILE=");

	/*连接TUXEDO */
	if(tpinit((TPINIT*)NULL)==-1) 
	{
		strcpy(szReturn,"error=init error$");
		return FALSE;
	}

	WriteLog("============ WSH ============");
	WriteLog(strServerip.GetBuffer());
	WriteLog(tuxgetenv("WSNADDR"));

	long Len = 0;
	if (tpcall(".TMIB",(char *)buff,0,(char **)&buff,&Len,0)== -1)
	{
		CString strReturn="";
		strReturn.Format("tpcall(.TMIB) failed:%s\n",tpstrerror(tperrno));
		//Fprint32(buff);
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,strReturn);
		return FALSE;
	}

	//print
	//Fprint32(buff);

	long Num;
	Num = Foccur32(buff,lFilter);
	if(Num<1)
	{
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,"error=Get data error$");
		return FALSE;
	}

	//print
	//Fprint32(buff);
	char WSHname[256]={0};
	char state[256]={0};

	long numblockq = 0;
	long rcvdbyt = 0;
	long rcvdnum = 0;
	long sentbyt = 0;
	long sentnum = 0;

	long numblockq_per_second = 0;
	long rcvdbyt_per_second = 0;
	long rcvdnum_per_second = 0;
	long sentbyt_per_second = 0;
	long sentnum_per_second = 0;

	CString strTaskParam = FindStrValue(strParas, "_TaskParam");
	CString strMonitorID = FindStrValue(strParas, "_MonitorID");

    for(int i=0;i<Num;i++)
	{
		memset(state,0,256);

		CString sTemp;

		long temp = 0;
		Fget32(buff,lFilter,i,(char *)&temp,NULL);
		sTemp.Format("%d",temp);

		char strTemp[1024];
		 sprintf(strTemp,"gettemp=%s,need=%s",sTemp.GetBuffer(),strTaskParam.GetBuffer());

		 WriteLog(strTemp);

		if(_stricmp(sTemp,strTaskParam)==0)
		{
			Fget32(buff,TA_STATE,i,state,NULL);

			Fget32(buff,TA_NUMBLOCKQ,i,(char *)&numblockq,NULL);
			Fget32(buff,TA_RCVDBYT,i,(char *)&rcvdbyt,NULL);
			Fget32(buff,TA_RCVDNUM,i,(char *)&rcvdnum,NULL);
			Fget32(buff,TA_SENTBYT,i,(char *)&sentbyt,NULL);
			Fget32(buff,TA_SENTNUM,i,(char *)&sentnum,NULL);

			//wk参数处理
			if(!GetWSHAver(strMonitorID,numblockq,rcvdbyt,rcvdnum,sentbyt,sentnum,
				numblockq_per_second,rcvdbyt_per_second,rcvdnum_per_second,sentbyt_per_second,sentnum_per_second))
			{
				Sleep(1000);		
				GetWSH(strParas, szReturn, nSize);
			}

			CString temp;
			CString text;

			temp.Format("state=%s$",state);
			text += temp;
			
			temp.Format("numblockq=%d$",numblockq);
			text += temp;
			temp.Format("rcvdbyt=%d$",rcvdbyt);
			text += temp;
			temp.Format("rcvdnum=%d$",rcvdnum);
			text += temp;
			temp.Format("sentbyt=%d$",sentbyt);
			text += temp;
			temp.Format("sentnum=%d$",sentnum);
			text += temp;


			temp.Format("numblockq_per_second=%d$",numblockq_per_second);
			text += temp;
			temp.Format("rcvdbyt_per_second=%d$",rcvdbyt_per_second);
			text += temp;
			temp.Format("rcvdnum_per_second=%d$",rcvdnum_per_second);
			text += temp;
			temp.Format("sentbyt_per_second=%d$",sentbyt_per_second);
			text += temp;
			temp.Format("sentnum_per_second=%d$",sentnum_per_second);
			text += temp;

			//加标志
			//text = "FileSplitFlag"+text;
			//text += "EndSplitFlag";

			strcpy(szReturn,text);

			tpfree((char *)buff);
		  tpterm();
			return TRUE;
		}
	}

	strcpy(szReturn,"error=no data$");
	tpfree((char *)buff);
	tpterm();
	return FALSE;

}

//得到服务列表
BOOL GetWSHList(const char *strParas, char * szReturn, int & nSize)
{
	if(tpinit((TPINIT*)NULL)==-1) /*连接TUXEDO */
	{
		strcpy(szReturn,"error=init error$");
		return FALSE;
	}

	//tpalloc
	FBFR32	*buff;
	buff = (FBFR32 *)tpalloc(FMLTYPE32,NULL,0);

	//fchg
	Fchg32(buff,TA_OPERATION,0,"GET",0);
	Fchg32(buff,TA_CLASS,0,"T_WSH",0);
	
	long lFilter=TA_PID;
	//Fchg32(buff, TA_FILTER, 0, (char *)&lFilter, 0);

	//tpcall
	long Len;
	if (tpcall(".TMIB",(char *)buff,0,(char **)&buff,&Len,0)== -1)
	{
		CString strReturn="";
		strReturn.Format("tpcall(.TMIB) failed:%s\n",tpstrerror(tperrno));
		//Fprint32(buff);
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,strReturn);
		return FALSE;
	}

	//Fprint32(buff);
	//return;

	long Num;
	Num = Foccur32(buff,lFilter);
	if(Num<1)
	{
		tpfree((char *)buff);
		tpterm();
		strcpy(szReturn,"error=Get data error$");
		return FALSE;
	}

	CString text;
	CString Temp;
	CString Temp1;
	for(int i=0; i<Num; i++)
	{
		/*
		char temp[256]={0};
		Fget32(buff,lFilter,i,temp,NULL);

		Temp1 = temp;
		Temp = "WSHID="+ Temp1 + "*@";

		text += Temp;
		*/
		long temp = 0;
		Fget32(buff,lFilter,i,(char *)&temp,NULL);

		Temp1.Format("%d",temp);
		Temp = Temp1;
		Temp += "=";
		Temp += Temp1;
		Temp += "$";
		//Temp = "Servername="+ Temp1 + "*@";

		text += Temp;

	}
	//加标志
	//text = "FileSplitFlag"+text;
	//text += "EndSplitFlag";

	strcpy(szReturn,text);

	//exit
	tpfree((char *)buff);
	tpterm();

	return TRUE;

}

//保存本次数据并计算平均值
BOOL GetWSHAver(CString strMonitorID,long numblockq,long rcvdbyt,long rcvdnum,long sentbyt,long sentnum,
				long &numblockq_per_second,long &rcvdbyt_per_second,long &rcvdnum_per_second,long &sentbyt_per_second,long &sentnum_per_second)
{
	long buff[6]= {0}; 

	CString strPath;
	#if _DEBUG  
		strPath.Format(".\\WSHParam_%s.ini", strMonitorID);
	#else  
		strPath.Format("..\\data\\TmpIniFile\\WSHParam_%s.ini", strMonitorID);
	#endif  

	read(strPath.GetBuffer(strPath.GetLength()),buff,sizeof(long));

	//得到上一次
	long Interval0 = buff[0];
	long numblockq0 = buff[1];
	long rcvdbyt0 = buff[2];
	long rcvdnum0 = buff[3];
	long sentbyt0 = buff[4];
	long sentnum0 = buff[5];
	
	//保存这次的数据
	CTime t1(1970,7,1,0,0,0);
	CTime t2 = CTime::GetCurrentTime();
	CTimeSpan ts = t2 - t1; 
	long Interval1 = ts.GetTotalSeconds();

	buff[0] = Interval1;
	buff[1] = numblockq;
	buff[2] = rcvdbyt;
	buff[3] = rcvdnum;
	buff[4] = sentbyt;
	buff[5] = sentnum;

	write(strPath.GetBuffer(strPath.GetLength()),buff,sizeof(long));

	//判断是否已经保存过数据
	if(Interval0 <= 0)
		return FALSE;

	//两次时间间隔
	long Interval = Interval1 - Interval0;
	if(Interval <= 0)
		return FALSE;
	
	//两次数据差
	numblockq -= numblockq0;
	rcvdbyt -= rcvdbyt0;
	rcvdnum -= rcvdnum0;
	sentbyt -= sentbyt0;
	sentnum -= sentnum0;
	
	//计算平均值
	if(numblockq > 0)
		numblockq_per_second = numblockq/Interval;

	if(rcvdbyt > 0)
		rcvdbyt_per_second = rcvdbyt/Interval;

	if(rcvdnum > 0)
		rcvdnum_per_second = rcvdnum/Interval;

	if(sentbyt > 0)
		sentbyt_per_second = sentbyt/Interval;

	if(sentnum > 0)
		sentnum_per_second = sentnum/Interval;


	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//把BUFF写到文件
void write(char * szPath,void *buff,int size)
{
	FILE *fp; 

	/*创建一个二进制文件只写*/ 
	fp=fopen(szPath, "wb"); 
	
	if(!fp)
		return;

	/*将6个long数写入文件中*/ 
	fwrite(buff, size, 6, fp);
	fclose(fp); 
}

//把文件读到BUFF
void read(char * szPath,void *buff,int size)
{
	FILE *fp = NULL; 

	/*打开一个二进制文件只读*/ 
	fp=fopen(szPath, "rb");

	if(!fp)
		return;

	/*判断是否读了6个数*/ 
	if(fread(buff,size, 2, fp)!=2) 
								
	{ 
		/*不到6个数文件结束*/ 
	   if(feof(fp)) 
		 printf("End of file"); 
	   else 
		   /*读数错误*/ 
		 printf("Read error");  
	}

	/*关闭文件*/ 
	fclose(fp);                   
}