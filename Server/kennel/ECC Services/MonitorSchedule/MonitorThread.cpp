#include "MonitorThread.h"
#include "Entity.h"
#include "ThreadContrl.h"
#include "MakeRecord.h"
#include <svdbapi.h>
#include "ProcessMonitorData.h"
#include <vector>

#ifndef WIN32
#include <dlfcn.h>
#else
//#include <windows.h>
#include <objbase.h>
#include <excpt.h>
#endif

extern Util *putil;
extern CString g_strRootPath;

extern std::string g_RefreshQueueName;
extern std::string g_QueueAddr;
extern string g_ServerHost;

extern int gnTotal;
void WriteLog( const char* str );


//sxf 2008-12-8
void String2Array(std::vector<std::string> &vOut,string strIn,string strDiv)
{
	int pos1= 0,pos2 = 0;

	while(1)
	{
		pos2 = strIn.find(strDiv, pos1);

		if (pos2 < 0)
		{
			vOut.push_back(strIn.substr(pos1, strIn.length() -pos1));
			break;
		}
		
		vOut.push_back(strIn.substr(pos1, pos2 -pos1));
		
		pos1 = pos2 +1;
		pos2 = pos1;

	}
	
}


MonitorThread::MonitorThread():ThreadEx()
{
//	m_hEvent=::CreateEvent(NULL,TRUE,FALSE,NULL);
	SetThreadName("Work thread");
	InitData();

}

MonitorThread::~MonitorThread()
{ 
/*	if(m_hEvent)
		::CloseHandle(m_hEvent);
*/

}


void MonitorThread::run()
{
	//DWORD dret;
	CString strError="";
	InitData();

#ifdef WIN32

	if(FAILED(::CoInitialize(NULL)))
	{
		strError.Format("CoInitialize failed when create thread:%s",m_ThreadName);
		//putil->ErrorLog((LPCSTR)strError);
		return;
	}
	

#endif

	Monitor *pMonitor=NULL;

	printf("In Thread :%s\n",this->m_ThreadName);

	try{

		while(TRUE)
		{
			//		dret=::WaitForSingleObject(m_hEvent,INFINITE);
			//		if(dret==WAIT_FAILED)

			if(!m_Event.wait())
			{
				strError="MonitorThread wait failed";
				throw MSException((LPCSTR)strError);
			}
			if(m_Monitor==NULL)
			{
				//			 ::ResetEvent(m_hEvent);
				m_Event.reset();
				m_pThreadContrl->AddToIdleThread(this);
				continue;
			}

			/*
			if(!InitSocket())
				throw MSException("Init socket failed");
			*/


			/*		BOOL ist=FALSE;
			ist=(m_Monitor->GetMonitorClass().CompareNoCase("WMI")==0);

			if(ist)
			printf("��%s,Class is :%s\n",GetThreadName(),m_Monitor->GetMonitorClass());*/


			m_nRunCount=0;
			m_StartTime=CTime::GetCurrentTimeEx();

			try{
				RunMonitor();
			}catch(MSException &e)
			{
				pMonitor->SetSkipCount(pMonitor->GetSkipCount()+1);
				char errlog[2048]={0};
				sprintf(errlog,"%s,%s,MonitorID:%s, SkipCount: %d. ",this->m_ThreadName,e.GetDescription(),m_Monitor->GetMonitorID(),pMonitor->GetSkipCount());
				//strError.Format("%s,%s,MonitorID:%s",this->m_ThreadName,e.GetDescription(),m_Monitor->GetMonitorID());
			//	//putil->ErrorLog(strError);
				//putil->ErrorLog(errlog);
				WriteLog( errlog );
			}catch(...)
			{
				pMonitor->SetSkipCount(pMonitor->GetSkipCount()+1);
				char errlog[2048]={0};
				sprintf(errlog,"Executent monitor exception,MonitorID:%s, will set skipcount +1. ",m_Monitor->GetMonitorID());
				//strError.Format("Executent monitor exception,MonitorID:%s",m_Monitor->GetMonitorID());
				////putil->ErrorLog(strError);
				//putil->ErrorLog(errlog);
				WriteLog( errlog );
			}
			int oldstate=m_Monitor->GetLastState();
			if(oldstate!=m_MonitorState)
			{
				if((m_MonitorState==Monitor::STATUS_ERROR)||(m_MonitorState==Monitor::STATUS_BAD))
					m_Monitor->CalculateErrorFrequency(true);
				else if((oldstate==Monitor::STATUS_ERROR)||(oldstate==Monitor::STATUS_BAD))
					m_Monitor->CalculateErrorFrequency(false);

			}

			m_Monitor->SetLastState(m_MonitorState);


			m_pThreadContrl->AdjustRetByMonitor(m_Monitor,FALSE);

			m_Monitor->SetRunning(FALSE);

			pMonitor=m_Monitor;
			InitData();
			//		::ResetEvent(m_hEvent);
			m_Event.reset();
			m_pThreadContrl->AddToIdleThread(this);

			m_pThreadContrl->CheckTaskQueueByMonitor(pMonitor);

			/*
			gnTotal++;
			char szTemp[1024] = {0};
			sprintf( szTemp, "线程：%d, 监测器ID：%s, 次数：%d\n ", this->m_ThreadName, m_Monitor->GetMonitorID(), gnTotal );
			WriteLog( szTemp );
			cout << szTemp << endl;
			*/


		}
	}catch(...)
	{
		char errlog[2048]={0};
		sprintf(errlog,"Thread  exit by exception, thread:%s, MonitorID:%s",m_ThreadName, m_Monitor->GetMonitorID() );

	//	strError.Format("Thread  exit by exception, thread:%s",m_ThreadName);
	//	//putil->ErrorLog((LPCSTR)strError);
		//putil->ErrorLog(errlog);
		WriteLog( errlog );
		return;

	}


#ifdef WIN32

	::CoUninitialize();
#endif



}

int filter(unsigned int code, struct _EXCEPTION_POINTERS *ep)
{
	puts("in filter");
	return EXCEPTION_EXECUTE_HANDLER;

}

bool MonitorThread::Run_Dll(LPFUNC func,int buflen)
{
	bool bret=false;

	__try{

		bret=(*func)(m_InBuf,m_RetBuf,buflen);
	}
	__except(filter(GetExceptionCode(), GetExceptionInformation()))
	{
		throw MSException("__try find --RunMonitor-Execute monitor happen exception");

	}


	return bret;
}


void MonitorThread::RunMonitor()
{
	m_nRunCount++;

	CString strError="";

	if(m_Monitor==NULL)
		throw MSException("Monitor is NULL");

	CString strDLLName="",
		strFuncName="";

	if(m_pThreadContrl->m_pOption->m_isDemo)
	{
		puts("=================Demo mode=========================");
		strDLLName.Format("%s/fcgi-bin/%s",(char *)g_strRootPath,m_pThreadContrl->m_pOption->m_DemoDLL.c_str());
		strFuncName=m_pThreadContrl->m_pOption->m_DemoFunction.c_str();

	}else
	{
		strDLLName.Format("%s/fcgi-bin/%s",(char *)g_strRootPath,(char *)m_Monitor->GetLibrary());
		strFuncName=m_Monitor->GetProcess();
	}

	if(strFuncName.empty())
	{
		throw MSException("RunMonitor-Function name is empty");
	}

	if(m_Monitor->m_isRefresh)
	{
		puts("-----------------Load library-------------------------");
		printf("Library name:%s\n",(char *)strDLLName);
		printf("Function name:%s\n",(char *)strFuncName);
		puts("-----------------End load library---------------------");
		puts("\n-----------------Input parameter----------------------");

		CStringList&lst=m_Monitor->GetParameterList();

		CStringList::iterator it;
		for(it=lst.begin();it!=lst.end();it++)
			printf("%s\n",(*it).c_str());

		puts("----------------End input parameter-------------------");
	}


#ifdef	WIN32

	HMODULE hm=::LoadLibrary(strDLLName);
	if(!hm)
	{
		strError.Format("RunMonitor-Load library failed,DLL name:%s",(char *)strDLLName);
		throw MSException((LPCSTR)strError);
	}

#ifndef SVVER70

	GatherData *pGetData=(GatherData*)::GetProcAddress(hm,(LPCSTR)strFuncName);
	if(!pGetData)
	{
		::FreeLibrary(hm);
		throw MSException("RunMonitor-GetProcAddress failed");
	}

/*	CStringList&lst=m_Monitor->GetParameterList();
	POSITION pos=lst.GetHeadPosition();
	while(pos)
		printf("%s\n",lst.GetNext(pos));

	puts(strDLLName);
	puts(strFuncName);*/

	try{
		CStringList&lstParam=m_Monitor->GetParameterList();
		(*pGetData)(lstParam,m_RetBuf);
	}catch(...)
	{
		::FreeLibrary(hm);
		throw MSException("RunMonitor happened exception_1");
	}

#else

	LPFUNC pfunc = (LPFUNC)GetProcAddress(hm,(LPCSTR)strFuncName);
	if(!pfunc)
	{
		::FreeLibrary(hm);
		throw MSException("RunMonitor-GetProcAddress failed");

	}


	int iLen=MakeInBuf();
	int buflen=RETBUFCOUNT;

	if(!m_Monitor->m_isRunInProcess)
	{

		try{
			if(m_pThreadContrl->m_pOption->m_isDemo)
			{
				WriteLog( "demo!" );
				//MutexLock lock(m_pThreadContrl->m_pSchMain->m_DemoDllMutex);
				(*pfunc)(m_InBuf,m_RetBuf,buflen);
			}
			else
			{
				(*pfunc)(m_InBuf,m_RetBuf,buflen);
				/*
				int nType = m_Monitor->GetMonitorType();
				if( nType == 41)
                    //sprintf( m_RetBuf, "ProcessCount=1\0ThreadCount=38\0PercentProcessorTime=0.00\0WorkingSet=23516.00\0MemUtilization=1.12\0PrivateBytes=21016.00\0VirUtilization=1.00\0" );
					memmove( m_RetBuf, "ProcessCount=1\0ThreadCount=46\0PercentProcessorTime=3.13\0WorkingSet=25596.00\0MemUtilization=1.22\0PrivateBytes=22204.00\0VirUtilization=1.06\0",
							 strlen( "ProcessCount=1$ThreadCount=46$PercentProcessorTime=3.13$WorkingSet=25596.00$MemUtilization=1.22$PrivateBytes=22204.00$VirUtilization=1.06$" ) );
				else if( nType == 14 )
                    //sprintf( m_RetBuf, "Started=1\0State=38\0Status=0.00\0Processes=2\0" );
					memmove( m_RetBuf, "Processes=0\0Started=False\0ProcessName=NA\0State=Stopped\0Status=OK\0", 
							 strlen( "Processes=0$Started=False$ProcessName=NA$State=Stopped$Status=OK$" ) );
				else if( nType == 10 )
                    //sprintf( m_RetBuf, "utilization=1\0detailutilization=38\0" );
					memmove( m_RetBuf, "detailutilization=CPU0:49,CPU1:69\0utilization=59\0", strlen( "etailutilization=CPU0:49,CPU1:69$utilization=59$" ) );
				else if( nType == 12 )
                   //sprintf( m_RetBuf, "percentUsed=1\0Mbfree=38\0TotalMemory=0.00\0PhyMemUsage=23516.00\0FreePhyMem=1.12\0TotalPhyMem=21016.00\0pagesPerSec=1.00\0" );
					memmove( m_RetBuf, "TotalPhyMem=511.45\0TotalMemory=1254.03\0FreePhyMem=148.99\0Mbfree=760.45\0PhyMemUsage=70.87\0percentUsed=39.36\0pagesPerSec=0.00\0", 
							 strlen( "TotalPhyMem=511.45$TotalMemory=1254.03$FreePhyMem=148.99$Mbfree=760.45$PhyMemUsage=70.87$percentUsed=39.36$pagesPerSec=0.00$" ) );
				else if( nType == 11 )
                   //sprintf( m_RetBuf, "percentFull=1\0Mbfree=38\0TotalSize=0.00\0" );
				   memmove( m_RetBuf, "percentFull=27.40\0Mbfree=7426.61\0TotalSize=10228.86\0", 
							strlen( "percentFull=27.40$Mbfree=7426.61$TotalSize=10228.86$" ) );
				else
					sprintf( m_RetBuf, "error=no data!\0" );
				*/

			}
			//Run_Dll(pfunc,buflen);

		}catch(MSException &e)
		{
		//	//putil->ErrorLog(strError);
			::FreeLibrary(hm);
			throw MSException(e.GetDescription());
		}catch(...)
		{
			::FreeLibrary(hm);
			throw MSException("RunMonitor happened exception_2");
		}
	}else{
     	::FreeLibrary(hm);
		RunInProcess(iLen,strDLLName.getText(),strFuncName.getText());
	}

#endif

	::FreeLibrary(hm);

	char szTemp[1024] = {0};
	char* pszTemp = szTemp;

	sprintf( szTemp, "monitorID:%s,returnBuffer:", m_Monitor->GetMonitorID() );
	pszTemp = m_RetBuf;
	while( *pszTemp != '\0' )
	{
		strcat( szTemp, pszTemp );
		strcat( szTemp, "$" );

		pszTemp += strlen( pszTemp ) + 1;
	}

	OutputDebugString( szTemp );
	OutputDebugString( "\n" );

	WriteLog( szTemp );

#else
   void *dp;
   char *error;

   dp = dlopen(strDLLName, RTLD_LAZY);
   
   if(dp == NULL)
   {
   		error=dlerror();
   		strError.Format("RunMonitor-Load library failed,DLL name:%s,Error:%s",(char *)strDLLName,error);
		throw MSException((LPCSTR)strError);
   }

   GatherData *pGetData = (GatherData*)dlsym(dp, strFuncName);

   error = dlerror();

   if (error)
   {
	    dlclose(dp);
	   throw MSException("RunMonitor-GetProcAddress failed");
   }

	try{
		CStringList&lstParam=m_Monitor->GetParameterList();
		
	        CStringList::const_iterator it;
		it=lstParam.begin();
		while(it!=lstParam.end())
		{
		   puts(*it++);
		}
		
	
		(*pGetData)(lstParam,m_RetBuf);

	}catch(...)
	{
	//	::FreeLibrary(hm);
	    //    dlclose(dp);
		throw MSException("RunMonitor happened exception_3");
	}
	
	//puts(m_RetBuf);

	 dlclose(dp);



#endif


/*	puts("\n#########################################################");
	printf("Result is:%s\n",m_RetBuf);
	puts("############################end##########################");
*/
#ifdef SVVER70
	 PaserResultV70();
#else
	PaserResult();
#endif
	
	try{

     	ParserMonitorState();

	}catch(...)
	{
		m_MonitorState=Monitor::STATUS_BAD;
		printf(".....%s.....\n",(char *)m_strDisplay);
		m_strDisplay="Error:Set state error";
#ifdef SVVER70
		ProcessResultV70();
#else
		ProcessResult();
#endif
		return ;

	}
	if(m_MonitorState==Monitor::STATUS_BAD)
	{
		if((m_Monitor->GetCheckError())&&(m_nRunCount<2))
		{
			ClearResult();
			RunMonitor();
			return;
		}
	}
	
	
	//if((m_MonitorState==Monitor::STATUS_ERROR)&&(m_nRunCount<2)&&(strcmp(m_Monitor->GetEntityType(),"NetworkEquipment")==0))
	//{
	//	ClearResult();
	//	RunMonitor();
	//	return;
	//}

#ifdef SVVER70
	ProcessResultV70();
#else
	ProcessResult();
#endif
	
	m_Monitor->SetSkipCount(0);
}

int MonitorThread::MakeInBuf()
{
     CStringList&lstParam=m_Monitor->GetParameterList();

	 CStringList::iterator it;
	 int len=0;
	 for(it=lstParam.begin();it!=lstParam.end();it++)
	 {
		 len+=(*it).size()+1;
	 }

	 len++;
//	 printf("len:%d,lst size:%d\n",len,lstParam.size());

	 if(!m_InBuf.checksize(len))
	 {
		 throw MSException("Malloc memmory failed for inbuf");
		 return 0;
	 }

//	 printf("buf length:%d\n",m_InBuf.size());

	 char *pt=m_InBuf;
	 for(it=lstParam.begin();it!=lstParam.end();it++)
	 {
		 strcpy(pt,(*it).c_str());
		 pt+=(*it).size()+1;
	 }

//	 printf("data size:%d\n",pt-(char*)m_InBuf);

	 return len;
}

void MonitorThread::InitData()
{
	m_Monitor=NULL;
	memset(m_RetBuf,0,RETBUFCOUNT);
	m_InBuf.zerobuf();
	m_RVmap.clear();
	m_RetValueCount=0;
	m_MonitorState=Monitor::STATUS_NULL;
	m_nRunCount=0;
	m_strDisplay.clear();

	CReturnValueList::iterator it;
	for(it=m_RetValueList.begin();it!=m_RetValueList.end();it++)
		delete *it;
	m_RetValueList.clear();


/*	while(!m_RetValueList.IsEmpty())
	{
		ReturnValue*pv=m_RetValueList.RemoveTail();
		delete pv;
	}
	*/

	for(int i=0;i<retcount;i++)
	{
		m_RetValues[i].Clear();
	}

}
BOOL MonitorThread::PaserResultV70(void)
{
	m_RVmap.clear();

	if(!BulidStringMap(m_RVmap,m_RetBuf))
	{
		throw MSException("Build return data failed");
		return false;
	}

	if(m_Monitor->m_isRefresh)
	{
		puts("\n------------------------Return value-----------------------");

		STRINGMAP::iterator itm;
		while(m_RVmap.findnext(itm))
			printf("%s=%s\n",(*itm).getkey().getword(),(*itm).getvalue().getword());

		puts("-------------------------End return value--------------------");
	}


	if(m_RVmap.size()==0)
	{
		m_MonitorState=Monitor::STATUS_BAD;
		throw MSException("Return value is empty");
	}

	svutil::word *pv=m_RVmap.find("error");
	if(pv!=NULL)
	{
		m_MonitorState=Monitor::STATUS_BAD;
		m_strDisplay=pv->getword();

		m_RVmap.clear();

		return true;

	}

	CReturnDataList &LstRet=m_Monitor->GetReutrnList();
	m_RetValueCount=(int)LstRet.size();

	ReturnValue*prv=NULL;
	BOOL bv=m_RetValueCount>retcount;
	CString strDisplay="";

	char szDisplay[5*1024] = {0};


	int n=0;
	ReturnData*prd=NULL;
	CReturnDataList::iterator it;
	svutil::word *pvalue=NULL;

	for(it=LstRet.begin();it!=LstRet.end();it++)
	{
		prd=(*it);
		if(bv)
		   prv=new ReturnValue;
		else
			prv=m_RetValues+n;

		prv->m_pReturnData=prd;

		pvalue=m_RVmap.find((LPCSTR)prd->m_Name);
		if(pvalue==NULL)
		{
			printf("name :%s\n",(LPCSTR)prd->m_Name);

			m_RVmap.clear();

			throw MSException("ParserResult return value is bad, it's name: "+prd->m_Name);
		}

		if(stricmp(prd->m_Type,"Int")==0)
		{
			prv->m_value.nVal=atoi(pvalue->getword());

			//strDisplay.Format("%s=%d, ",(char *)prd->m_Label,prv->m_value.nVal);
			sprintf( szDisplay, "%s=%d, ", (char *)prd->m_Label,prv->m_value.nVal );
//			printf("label===%s\n",prd->m_Label);
		}else if(stricmp(prd->m_Type,"Float")==0)
		{
			prv->m_value.fVal=atof(pvalue->getword());
			//strDisplay.Format("%s=%0.2f, ",(char *)prd->m_Label,prv->m_value.fVal);
			sprintf( szDisplay, "%s=%0.2f, ",(char *)prd->m_Label,prv->m_value.fVal);
//			printf("label===%s\n",prd->m_Label);
		}else if(stricmp(prd->m_Type,"String")==0)
		{
			if(strlen(pvalue->getword())>RETVALUEMAXCOUNT-1)
				throw MSException("Return string value too big");
			strcpy(prv->m_value.szVal,pvalue->getword());
			//strDisplay.Format("%s=%s, ",(char *)prd->m_Label,prv->m_value.szVal);
			sprintf( szDisplay, "%s=%s, ",(char *)prd->m_Label,prv->m_value.szVal);
//			printf("label===%s\n",prd->m_Label);
		}else
		{
			if(bv)
				delete prv;

			m_RVmap.clear();

			throw MSException("ParserResult error, unknown type name");
		}
		if(bv)
			m_RetValueList.push_back(prv);

		n++;

		//m_strDisplay+=strDisplay;

		m_strDisplay += szDisplay;

	}

	m_RVmap.clear();

	return TRUE;

}


BOOL MonitorThread::PaserResult()
{
	m_strDisplay.clear();
	CString strContent=m_RetBuf;
	CString strDisplay="";


	strContent.Replace("\r","");
	strContent.Replace("\n","");

	if(strContent.empty())
	{
		m_MonitorState=Monitor::STATUS_ERROR;
		throw MSException("Return value is empty");
	}

	if(strContent.Find("error=",0)>-1)
	{
		m_MonitorState=Monitor::STATUS_ERROR;
		GetProperty("error=", strContent, strDisplay);
		m_strDisplay=strDisplay;
		return TRUE;

	}

	CReturnDataList &LstRet=m_Monitor->GetReutrnList();
	m_RetValueCount=(int)LstRet.size();

	ReturnValue*prv=NULL;
	BOOL bv=m_RetValueCount>retcount;
	CString strSection="";

//	POSITION pos=LstRet.GetHeadPosition();
	int n=0;
	ReturnData*prd=NULL;
	CReturnDataList::iterator it;
	for(it=LstRet.begin();it!=LstRet.end();it++)
//	while(pos)
	{
//		prd=LstRet.GetNext(pos);
		prd=(*it);
		if(bv)
		   prv=new ReturnValue;
		else
			prv=m_RetValues+n;

		strSection=prd->m_Name;
		strSection+="=";

		prv->m_pReturnData=prd;

		if(stricmp(prd->m_Type,"Int")==0)
		{
			GetProperty(strSection,strContent,prv->m_value.nVal);
			strDisplay.Format("%s=%d, ",(char *)prd->m_Label,prv->m_value.nVal);
//			printf("label===%s\n",prd->m_Label);
		}else if(stricmp(prd->m_Type,"Float")==0)
		{
			GetProperty(strSection,strContent,prv->m_value.fVal);
			strDisplay.Format("%s=%0.2f, ",(char *)prd->m_Label,prv->m_value.fVal);
//			printf("label===%s\n",prd->m_Label);
		}else if(stricmp(prd->m_Type,"String")==0)
		{
			GetProperty(strSection,strContent,prv->m_value.szVal);
			strDisplay.Format("%s=%s, ",(char *)prd->m_Label,prv->m_value.szVal);
//			printf("label===%s\n",prd->m_Label);
		}else
		{
			if(bv)
				delete prv;
			throw MSException("ParserResult error unknown type name");
		}
		if(bv)
			m_RetValueList.push_front(prv);

		n++;

		m_strDisplay+=strDisplay;

	}

	return TRUE;

}

BOOL MonitorThread::GetProperty(const char *szProperty, const CString strSource, CString &strRet)
{
	BOOL bRet = FALSE;

	CString strTemp;
	strTemp=strSource;


	int nLeftIndex = strTemp.Find(szProperty, 0);
	if (nLeftIndex == -1)
		return bRet;

	int nRightIndex = strTemp.Find("$", nLeftIndex);
	if (nRightIndex == -1)
		return bRet;

	int nLength = strlen(szProperty);

	CString str = strTemp.Mid(nLeftIndex + nLength , nRightIndex - nLeftIndex - nLength);
	strRet = str;

	bRet = TRUE;
	return bRet;


}

BOOL MonitorThread::GetProperty(const char *szProperty, const CString strSource, char *psret)
{
	BOOL bRet = FALSE;
	CString strTemp;
	strTemp=strSource;


	int nLeftIndex = strTemp.Find(szProperty, 0);
	if (nLeftIndex == -1)
		return bRet;

	int nRightIndex = strTemp.Find("$", nLeftIndex);
	if (nRightIndex == -1)
		return bRet;

	int nLength = (int)strlen(szProperty);

	CString str = strTemp.Mid(nLeftIndex + nLength , nRightIndex - nLeftIndex - nLength);
	int size=str.GetLength();
	size=(size>=RETVALUEMAXCOUNT) ?RETVALUEMAXCOUNT-1:size;
	str=str.Left(size);
	sprintf(psret,"%s",str.GetBuffer(size));

	bRet = TRUE;
	return bRet;


}

BOOL MonitorThread::GetProperty(const char *szProperty, const CString strSource, float &fRet)
{
	BOOL bRet = FALSE;

	CString strTemp;
	strTemp=strSource;


	int nLeftIndex = strTemp.Find(szProperty, 0);
	if (nLeftIndex == -1)
		return bRet;

	int nRightIndex = strTemp.Find("$", nLeftIndex);
	if (nRightIndex == -1)
		return bRet;

	int nLength = (int)strlen(szProperty);

	CString str = strTemp.Mid(nLeftIndex + nLength, nRightIndex - nLeftIndex - nLength);
	fRet= atof(str);

	bRet = TRUE;
	return bRet;

}

BOOL MonitorThread::GetProperty(const char *szProperty, const CString strSource, int &nRet)
{
	BOOL bRet = FALSE;

	CString strTemp;
	strTemp=strSource;


	int nLeftIndex = strTemp.Find(szProperty, 0);
	if (nLeftIndex == -1)
		return bRet;

	int nRightIndex = strTemp.Find("$", nLeftIndex);
	if (nRightIndex == -1)
		return bRet;

	int nLength = (int)strlen(szProperty);

	CString str = strTemp.Mid(nLeftIndex + nLength , nRightIndex - nLeftIndex - nLength);
	nRet= atoi((LPCTSTR)str);

	bRet = TRUE;
	return bRet;



}

void MonitorThread::ParserMonitorState()
{
	if(m_MonitorState==Monitor::STATUS_BAD)
		return;

	if(CheckStateByType(Error))
	{
		m_MonitorState=Monitor::STATUS_ERROR;
		return;
	}else if(CheckStateByType(Warning))
	{
		m_MonitorState=Monitor::STATUS_WARNING;
		return;
	}else if(CheckStateByType(Normal))
	{
		m_MonitorState=Monitor::STATUS_OK;
		return;
	}else{
		m_MonitorState=Monitor::STATUS_BAD;
		puts("No state be match");
		throw MSException("Parser monitor state error");
	}

}

BOOL MonitorThread::CheckStateByType(int Type)
{
	if(m_MonitorState==Monitor::STATUS_ERROR)
	{
		if(Type==Error)
			return TRUE;
		else
			return FALSE;
	}

/*	CString strCondition=_T(""),
		strErrorOperator=_T(""),
		strTemp=_T("");*/

	BOOL bRet=FALSE;
	CString strTemp="";

	StateCondition **PSC=m_Monitor->GetStateCondition();



	StateCondition *pSt=NULL;
	for(int i=0;i<3;i++)
	{
		if(PSC[i]->m_Type==Type)
		{
			pSt=PSC[i];
			break;
		}
	}

	if(pSt==NULL) return FALSE;

	

	CStringList lstOperator,lstID;
	int nret=ParserExpression(pSt->m_Expression,lstOperator,lstID);
	if(nret<0)
		return FALSE;
	
	if(nret==0)
	{
//		strTemp=lstID.RemoveTail();
		strTemp=lstID.back();
		lstID.pop_back();
		if(strTemp.empty())
		{
		        Util::FreeStringList(lstOperator);
			Util::FreeStringList(lstID);
			return FALSE;
		}
		Util::FreeStringList(lstOperator);
		Util::FreeStringList(lstID);
		puts("dddd2");
		return CheckSingleItemState(pSt,strTemp.getValue());
	}

	if(nret>0)
	{
		BOOL bTem=FALSE;
		BOOL bLast=FALSE;
		CStringList::iterator it,it2;

		it=lstID.begin();
		it2=lstOperator.begin();

	//	POSITION pos=lstID.GetHeadPosition();
	//	POSITION pos2=lstOperator.GetHeadPosition();
		if(it!=lstID.end())
//		if(pos)
		{
//			strTemp=lstID.GetNext(pos);
			strTemp=*it++;
			bRet=CheckSingleItemState(pSt,atoi(strTemp.GetBuffer(strTemp.GetLength())));
			if(it!=lstID.end())
			{
		//		strTemp=lstID.GetNext(pos);
				strTemp=*it++;
				bTem=CheckSingleItemState(pSt,atoi(strTemp.GetBuffer(strTemp.GetLength())));
				if(it2!=lstOperator.end())
				{
			//		strTemp=lstOperator.GetNext(pos2);
					strTemp=*it2++;
					if(strTemp.CompareNoCase("or")==0)
						bLast=bTem||bRet;
					else if(strTemp.CompareNoCase("and")==0)
						bLast=bTem&&bRet;
					else
						bLast=FALSE;
				}else
				{
				        Util::FreeStringList(lstOperator);
					Util::FreeStringList(lstID);
					return FALSE;
				}
			}else
			{
			
				Util::FreeStringList(lstOperator);
				Util::FreeStringList(lstID);
				return FALSE;
			}
			
			while(it!=lstID.end())
			{
				//strTemp=lstID.GetNext(pos);
				strTemp=*it++;
				bTem=CheckSingleItemState(pSt,atoi(strTemp.GetBuffer(strTemp.GetLength())));
				if(it2==lstOperator.end())
				{
					Util::FreeStringList(lstOperator);
					Util::FreeStringList(lstID);
					return FALSE;
				}
			//	strTemp=lstOperator.GetNext(pos2);
				strTemp=*it2++;
				if(strTemp.CompareNoCase("or")==0)
					bLast=bLast||bTem;
				else if(strTemp.CompareNoCase("and")==0)
					bLast=bLast&&bTem;
				else
					bLast=FALSE;
			}
			
			Util::FreeStringList(lstOperator);
			Util::FreeStringList(lstID);
 
			return bLast;


		}else
		{
			Util::FreeStringList(lstOperator);
			Util::FreeStringList(lstID);
			return FALSE;
		}
	}
	
	Util::FreeStringList(lstOperator);
	Util::FreeStringList(lstID);
	return FALSE;



}

/*BOOL MonitorThread::CheckStateByType(int Type)
{
	if(m_MonitorState==Monitor::STATUS_ERROR)
	{
		if(Type==Error)
			return TRUE;
		else
			return FALSE;
	}

//	CString strCondition=_T(""),
//		strErrorOperator=_T(""),
//		strTemp=_T("");

	BOOL bRet=FALSE;
	CString strTemp="";

	StateCondition **PSC=m_Monitor->GetStateCondition();

	StateCondition *pSt=NULL;
	for(int i=0;i<3;i++)
	{
		if(PSC[i]->m_Type==Type)
		{
			pSt=PSC[i];
			break;
		}
	}

	if(pSt==NULL) return FALSE;
	

	CStringList lstOperator,lstID;
	int nret=ParserExpression(pSt->m_Expression,lstOperator,lstID);
	if(nret<0)
		return FALSE;
	
	if(nret==0)
	{
//		strTemp=lstID.RemoveTail();
		strTemp=lstID.back();
		lstID.pop_back();
		if(strTemp.empty())
			return FALSE;
		return CheckSingleItemState(pSt,strTemp.getValue());
	}

	if(nret>0)
	{
		BOOL bTem=FALSE;
		BOOL bLast=FALSE;
		POSITION pos=lstID.GetHeadPosition();
		POSITION pos2=lstOperator.GetHeadPosition();
		if(pos)
		{
			strTemp=lstID.GetNext(pos);
			bRet=CheckSingleItemState(pSt,atoi(strTemp.GetBuffer(strTemp.GetLength())));
			if(pos)
			{
				strTemp=lstID.GetNext(pos);
				bTem=CheckSingleItemState(pSt,atoi(strTemp.GetBuffer(strTemp.GetLength())));
				if(pos2)
				{
					strTemp=lstOperator.GetNext(pos2);
					if(strTemp.CompareNoCase("or")==0)
						bLast=bTem||bRet;
					else if(strTemp.CompareNoCase("and")==0)
						bLast=bTem&&bRet;
					else
						bLast=FALSE;
				}else
					return FALSE;
			}else
				return FALSE;
			
			while(pos)
			{
				strTemp=lstID.GetNext(pos);
				bTem=CheckSingleItemState(pSt,atoi(strTemp.GetBuffer(strTemp.GetLength())));
				if(pos2)
					return FALSE;
				strTemp=lstOperator.GetNext(pos2);
				if(strTemp.CompareNoCase("or")==0)
					bLast=bLast||bTem;
				else if(strTemp.CompareNoCase("and")==0)
					bLast=bLast&&bTem;
				else
					bLast=FALSE;
			}
 
			return bLast;


		}else
			return FALSE;
	}


	return FALSE;



}*/

BOOL MonitorThread::CheckSingleItemState(/*int Type*/StateCondition *pSt , int ItemID)
{
/*	StateCondition **PSC=m_Monitor->GetStateCondition();
	StateCondition *pSt=NULL;
	for(int i=0;i<3;i++)
	{
		if(PSC[i]->m_Type==Type)
		{
			pSt=PSC[i];
			break;
		}
	}*/

	if(pSt==NULL) return FALSE;
	if(ItemID<=0)
		return FALSE;

	CStateConditionItemList &scl=pSt->GetStateConditionList();
	CStateConditionItemList::iterator it;
	//POSITION pos=scl.GetHeadPosition();
	it=scl.begin();
	StateConditionItem *ptem=NULL;
	//while(pos)
	while(it!=scl.end())
	{
	//	ptem=scl.GetNext(pos);
		ptem=*it++;
		if(ptem->m_ItemID==ItemID)
			break;
	}

	if(ptem==NULL)
		return FALSE;

	if(stricmp(ptem->m_Operator,"always")==0)
	    return TRUE;	
	if(stricmp(ptem->m_Operator,"none")==0)
		return FALSE;

	ReturnValue* pRV=NULL;
//	POSITION rpos=NULL;
	CReturnValueList::iterator itv;
	CString strTemp="";

	for(int n=0;n<m_RetValueCount;n++)
	{
		if(m_RetValueCount>retcount)
		{
			if(n==0)
				itv=m_RetValueList.begin();
		//	rpos=m_RetValueList.FindIndex(n);
		//	pRV=m_RetValueList.GetAt(rpos);
			pRV=*itv++;
		}else
			pRV=m_RetValues+n;

		if(!pRV)
			continue;
		if(pRV->m_pReturnData->m_Name==ptem->m_ParamName)
		{
			if(_stricmp(pRV->m_pReturnData->m_Type,"Int")==0)
				strTemp.Format("%d",pRV->m_value.nVal);
			else if(_stricmp(pRV->m_pReturnData->m_Type,"Float")==0)
				strTemp.Format("%0.2f",pRV->m_value.fVal);
			else if(_stricmp(pRV->m_pReturnData->m_Type,"String")==0)
				strTemp.Format("%s",pRV->m_value.szVal);


			//baseline feature sxf 2008-12-8
			if (m_Monitor->IsUseBaseline())
			{
				CTime tt = CTime::GetCurrentTimeEx();

				int nHour = tt.GetHour();

				CString strBaseline =  ptem->m_ParamValue;
				std::vector<std::string> vBlpts;
				std::string strVal;

				printf("baseline value:%s\n",strBaseline.GetBuffer(strBaseline.GetLength()));

				String2Array(vBlpts, std::string(strBaseline.GetBuffer(strBaseline.GetLength())),",");

				if (vBlpts.size() <= 0) //取基线数据失败
				{
					printf("get baseline data error.\n");
					return FALSE;
				}

				char *cEnd = NULL;
				char szTmp[64];

				double addval = strtod(vBlpts[vBlpts.size() -1].c_str(), &cEnd);

				vBlpts.pop_back();

				if (vBlpts.size() <= 0) //基线为空
				{
					printf("baseline data is empty\n");
					return FALSE;
				}

				long interval = 24 / vBlpts.size(); 

				long nIndex = (nHour-1)/interval;

				printf("nIndex:%d\n", nIndex);

				if (nIndex > vBlpts.size()-1) //基线数据不够
					return FALSE;
				else
					strVal = vBlpts[nIndex];


				if (strVal.compare("-1")==0) //此时刻没有基线数据
				{
					if (pSt->m_Type == StateCondition::Normal)
						return TRUE;
					else
						return FALSE;
				}
				
				double baseVal = strtod(strVal.c_str(), &cEnd);

				//加上附加值
				baseVal += addval;

				sprintf(szTmp, "%.4f", baseVal);

				printf("baseline compare value:%s\n",szTmp);

				return Judge(strTemp.GetBuffer(strTemp.GetLength()),
					szTmp,
					ptem->m_Operator);

				
			}else
			//

			return Judge(strTemp.GetBuffer(strTemp.GetLength()),
				ptem->m_ParamValue.GetBuffer(ptem->m_ParamValue.GetLength()),
				ptem->m_Operator);

		}

	}

	return FALSE;


}

void MonitorThread::ClearResult()
{
	m_MonitorState=Monitor::STATUS_NULL;
	m_RVmap.clear();
	memset(m_RetBuf,0,RETBUFCOUNT);
	m_strDisplay.clear();


/*	while(!m_RetValueList.IsEmpty())
	{
		ReturnValue*pv=m_RetValueList.RemoveTail();
		delete pv;
	}
	*/
	if(m_RetValueList.size()>0)
	{
		CReturnValueList::iterator it;
		for(it=m_RetValueList.begin();it!=m_RetValueList.end();it++)
			delete *it;
		m_RetValueList.clear();
	}

	for(int i=0;i<retcount;i++)
	{
		m_RetValues[i].Clear();
	}


}

BOOL MonitorThread::Judge(const char *szSource, const char *szDestination, const char *szRelation)
{
	if (!strcmp(szRelation, "==")) {
		float fS = atof(szSource);
		float fD = atof(szDestination);
		return (fS==fD)?TRUE:FALSE;
	}
	else if (!strcmp(szRelation, "!=")) {	
		float fS = atof(szSource);
		float fD = atof(szDestination);
		return (fS!=fD)?TRUE:FALSE;
	}
	else if (!strcmp(szRelation, ">")) {
		float fS = atof(szSource);
		float fD = atof(szDestination);
		return (fS>fD)?TRUE:FALSE;
	}
	else if (!strcmp(szRelation, "<")) {
		float fS = atof(szSource);
		float fD = atof(szDestination);
		return (fS<fD)?TRUE:FALSE;
	}
	else if (!strcmp(szRelation, ">=")) {
		float fS = atof(szSource);
		float fD = atof(szDestination);
		return (fS>=fD)?TRUE:FALSE;
	}
	else if (!strcmp(szRelation, "<=")) {
		float fS = atof(szSource);
		float fD = atof(szDestination);
		return (fS<=fD)?TRUE:FALSE;
	}
	else if (!strcmp(szRelation, "contains")) {
		if (strstr(szSource, szDestination))
			return TRUE;
		else
			return FALSE;
	}
	else if (!strcmp(szRelation, "!contains")) {
		if (strstr(szSource, szDestination))
			return FALSE;
		else
			return TRUE;
	}
	else {
		return FALSE;
	}

}

int MonitorThread::ParserExpression(CString strExpression, CStringList &lstOperator, CStringList &lstID)
{
	if(strExpression.IsEmpty())
		return -1;

	CString strTemp="";

	int pos=0;
	int nb=0;
	int n=0;
	if((pos=strExpression.Find(SEPARATOR,0))<0)
	{
	//	lstID.AddTail(strExpression);
		lstID.push_back(strExpression.getText());
		return 0;
	}

	for(nb=0;pos>-1;nb++)
	{
		strTemp=strExpression.Mid(nb,pos-nb);
//		lstID.AddTail(strTemp);
		lstID.push_back(strTemp.getText());
		pos++;
		nb=strExpression.Find(SEPARATOR,pos);
		if(nb<0)
			return -2;
		strTemp=strExpression.Mid(pos,nb-pos);
//		lstOperator.AddTail(strTemp);
		lstOperator.push_back(strTemp.getText());
		pos=strExpression.Find(SEPARATOR,nb+1);
		n++;
	}

//	lstID.AddTail(strExpression.Right(strExpression.GetLength()-strExpression.ReverseFind(SEPARATOR)-1));
	lstID.push_back(strExpression.Right(strExpression.GetLength()-strExpression.ReverseFind(SEPARATOR)-1).getText());

	return n;

}
bool MonitorThread::BulidStringMap(STRINGMAP &map,const char *buf)
{
	if(buf==NULL)
		return false;

	const char *pt =buf;
	if(pt[0]=='\0')
		return true;

	string str,name,value;
	int pos=0;
	do{
		str=pt;
		if((pos=str.find('=',0))<0)
			return false;
		name=str.substr(0,pos);
		value=str.substr(pos+1,str.size()-pos);
		if(name.empty())
			return false;
		map[name]=value;

		pt+=str.size()+1;	
	}while(pt[0]!='\0');

	return true;
}

void MonitorThread::ProcessResultV70(void)
{

	ReturnValue* pRV=NULL;
	CReturnValueList::iterator itv;
	CString strTemp="";

	char buf[2048]={0};
	int buflen=2048;
	int dlen=0;
	MakeRecord mr(buf,buflen,m_MonitorState);
	
	if((m_MonitorState==Monitor::STATUS_BAD)||(m_MonitorState==Monitor::STATUS_NULL))
	{
	
		if(!mr.MakeError(m_MonitorState,(LPCSTR)m_strDisplay))
		{
			throw MSException("Make record failed");
		}
		dlen=mr.GetDataSize();
	}else{
//		printf("Return count :%d\n",m_RetValueCount);
		for(int n=0;n<m_RetValueCount;n++)
		{
			if(m_RetValueCount>retcount)
			{
				if(n==0)
					itv=m_RetValueList.begin();
				pRV=*itv++;

			}else
				pRV=m_RetValues+n;
			if(pRV==NULL)
			{
				throw MSException("Return value is NULL");
			}
		
		    if(!mr.MakeBuf(pRV))
			{
				throw MSException("Make result failed");
			}

		}
		dlen=mr.GetDataSize();
	}

	//CString strCostTime="";

	//CTime time=CTime::GetCurrentTimeEx();
	//CTimeSpan ts=time-m_StartTime;

	//strCostTime.Format("[Cost time :%ld seconds]",ts.GetTotalSeconds());
	//m_strDisplay+=strCostTime;


	char *pt=buf;
	pt+=dlen;
	if(buflen-dlen<strlen((char *)m_strDisplay)+1)
		throw MSException("Process result -- string too big");
	strcpy(pt,(char *)m_strDisplay);
	dlen+=strlen((char *)m_strDisplay)+1;

	printf("MonitorId:%s,MonitorType:%d,state:%d,dstr:%s\n",m_Monitor->GetMonitorID(),m_Monitor->GetMonitorType(),m_MonitorState,(char *)m_strDisplay);

//	puts((char *)m_strDisplay);
//	const char *ps=m_Monitor->GetMonitorID();
//	puts(ps);
	ClearResult();
	if(m_Monitor->isDelete)
		return ;

	
	if(m_Monitor->m_isRefresh)
		putil->InsertSvdb(m_Monitor->GetMonitorID(),buf,dlen,g_ServerHost);
	else
		putil->InsertSvdb(m_Monitor->GetMonitorID(),buf,dlen);

	if(m_Monitor->m_isRefresh)
	{
		char buf[100]={0};
		sprintf(buf,"%s",m_Monitor->GetMonitorID());
		string text="Refresh ,PushMessage f: \"";
		text+=g_RefreshQueueName + "\"";
		if(!::PushMessage(g_RefreshQueueName,"Refresh_OK",buf,strlen(buf)+1,"default",g_QueueAddr))
			//putil->ErrorLog(text.c_str());
			WriteLog( text.c_str() );
	}	
}

void MonitorThread::ProcessResult()
{
	CString strResult="",
		strCostTime="";

	CTime time=CTime::GetCurrentTimeEx();
	CTimeSpan ts=time-m_StartTime;

	strCostTime.Format("[Cost time :%ld seconds]",ts.GetTotalSeconds());
	m_strDisplay+=strCostTime;


	strResult.Format(2048,"seid=%s$MonitorId=%s$MType=%d$EntityId=%s$MonitorState=%d$FireTime=%s$%sMonitorString=%s",
		m_Monitor->GetSEID().c_str(),
		m_Monitor->GetMonitorID(),
		m_Monitor->GetMonitorType(),
		m_Monitor->GetParentID().c_str(),
		m_MonitorState,
	//	time.Format("%Y-%m-%d %H:%M:%S"),
	    time.Format().c_str(),
		m_RetBuf,
		(char *)m_strDisplay);

		
	puts(strResult);
	puts(m_strDisplay);

/*	puts("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	puts(strResult);
	puts("+++++++++++++++++++++++++++++++++++++end++++++++++++++++++++++++++++++++++++");
*/
/*
	if(m_Monitor->m_isRefresh)
	{
		putil->InsertSVMQ("SiteviewRefresh",strResult.getText(),putil->g_strSession.getText());
		return;
	}
	if(!m_Monitor->isDelete)
        	putil->InsertSVMQ("siteviewlogs",strResult.getText(),"");

*/
	

	
}

void MonitorThread::ExecuteMonitor()
{
	if(m_Monitor==NULL)
		return;

	if(m_Monitor->GetRunning())
		return;
	m_Monitor->SetRunning(TRUE);


/*	if(m_Monitor->GetSubsequentItem()!=NULL)
	{
		m_Monitor->GetSubsequentItem()->AdjustRef(TRUE);
    	if((m_Monitor->GetEntity()!=NULL)&&(m_Monitor->GetMonitorClass().CompareNoCase("Ping")!=0))
			m_Monitor->GetEntity()->AdjustActiveMonitorRef(TRUE);
	}*/

	m_pThreadContrl->AdjustRetByMonitor(m_Monitor,TRUE);


//	::SetEvent(m_hEvent);
	m_Event.signal();

}

void MonitorThread::ExecuteMonitor(Monitor *pMonitor)
{
	if(pMonitor->GetRunning())
		return;
	if(pMonitor->isDelete)
		return;

	m_pThreadContrl->AdjustRetByMonitor(pMonitor,TRUE);

	m_Monitor=pMonitor;
	pMonitor->SetRunning(TRUE);
/*	if(pMonitor->GetSubsequentItem()!=NULL)
	{
		pMonitor->GetSubsequentItem()->AdjustRef(TRUE);
    	if((pMonitor->GetEntity()!=NULL)&&(m_Monitor->GetMonitorClass().CompareNoCase("Ping")!=0))
			pMonitor->GetEntity()->AdjustActiveMonitorRef(TRUE);
	}*/
//	::SetEvent(m_hEvent);
	m_Event.signal();

}

BOOL MonitorThread::InitSocket()
{

#ifdef 	WIN32
	int err=0;

	WSADATA WSAData;
	err=WSAStartup(MAKEWORD(2, 2), &WSAData); 

	if(err!=0)
	{
//		printf("�ʧ�");
		return FALSE;
	}
	if(WSAData.wVersion!=MAKEWORD(2,2))
	{
//		printf("�ʧ�");
		return FALSE;
	}
#endif

	return TRUE;


}

void MonitorThread::CloseSocket()
{
#ifdef	WIN32
		::WSACleanup();
#endif

}

bool MonitorThread::RunInProcess(int iLen,string dllname,string funname)
{
	if(m_Monitor==NULL)
		throw MSException("Monitor is empty in RunInProcess function");

#ifdef WIN32

	DWORD dw=::GetCurrentThreadId();
	char queuename[256]={0};
	sprintf(queuename,"SiteView_RunMonitor_%d",dw);
	::CreateQueue(queuename,2);
	::ClearQueueMessage(queuename);

	ProcessMonitorData pdata;
	pdata.m_data=m_InBuf.getbuffer();
	pdata.m_datalen=iLen;
	pdata.m_dllname=dllname;
	pdata.m_funname=funname;
    
	int len=pdata.GetRawDataSize();
	if(len<=RETBUFCOUNT)
	{
		if(!pdata.GetRawData(m_RetBuf,len))
			throw MSException("Get data failed in RunInProcess function");
		if(!::PushMessage(queuename,"InData",m_RetBuf,len))
			throw MSException("PushMessage failed in RunInProcess function");
	}else
	{
		svutil::buffer ibuf;
		if(!ibuf.checksize(len))
			throw MSException("Malloc memmory failed in RunInProcess function");

		if(!pdata.GetRawData(ibuf,len))
			throw MSException("Get data failed2 in RunInProcess function");
	}
	char strCommandLine[1024]={0};
	sprintf(strCommandLine,"%s\\fcgi-bin\\MonitorSchedule.exe -e %s",(char *)g_strRootPath,queuename);
//	sprintf(strCommandLine,"\"%s\\fcgi-bin\\MonitorSchedule.exe\"",(char *)g_strRootPath);
	PROCESS_INFORMATION pi;
	ZeroMemory( &pi, sizeof(PROCESS_INFORMATION) );
	STARTUPINFO si;
	ZeroMemory( &si, sizeof(STARTUPINFO) );
	si.cb=sizeof(STARTUPINFO);

	char strDir[256]={0};
	sprintf(strDir,"%s\\fcgi-bin\\",(char *)g_strRootPath);
	printf("CommandLine :%s\n",strCommandLine);
	printf("dir :%s\n",strDir);

	if(!::CreateProcess(NULL,
		strCommandLine,
		NULL,
		NULL,
		FALSE,
		/*CREATE_NO_WINDOW*/CREATE_NEW_CONSOLE,
		NULL,
		strDir,
		&si,&pi))
	{
		
		DWORD dw=GetLastError();
		printf("error:%d\n",dw);
		::DeleteQueue(queuename);

		throw MSException("Create process failed in RunInProcess function");
	}

   ::WaitForSingleObject(pi.hProcess,INFINITE);

   MQRECORD mrd=::PopMessage(queuename);
   if(mrd==INVALID_VALUE)
   {
		::DeleteQueue(queuename);
	   throw MSException("Receive messgae failed in RunInProcess function");
   }

   string label="";
   unsigned int blen=0;
   svutil::TTime rt;
   if(!GetMessageData(mrd,label,rt,NULL,blen))
   {
	   ::CloseMQRecord(mrd);
		::DeleteQueue(queuename);
	   throw MSException("GetMessageData failed in RunInProcess function");
   }

   if(label.compare("SiteView_Return")!=0)
   {
	   ::CloseMQRecord(mrd);
		::DeleteQueue(queuename);
	   throw MSException("Return data error in RunInProcess function");
   }

   if(blen>RETBUFCOUNT)
   {
	   ::CloseMQRecord(mrd);
		::DeleteQueue(queuename);
	   throw MSException("Return data too big in RunInProcess function");
   }

   if(!GetMessageData(mrd,label,rt,m_RetBuf,blen))
   {
	   ::CloseMQRecord(mrd);
		::DeleteQueue(queuename);
	   throw MSException("GetMessageData failed2 in RunInProcess function");
   }
   ::CloseMQRecord(mrd);
   ::DeleteQueue(queuename);


#endif

	return true;
}
