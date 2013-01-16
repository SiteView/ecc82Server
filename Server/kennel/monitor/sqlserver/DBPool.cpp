// DBPool.cpp: implementation of the DBPool class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "DBPool.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern void WriteLogFile(CString);
extern int PrintLog(const char * strReceive);

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
DBPool::DBPool()
{
	CoInitialize(NULL);	
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
DBPool::~DBPool()
{
	CoUninitialize();//释放
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
int DBPool::PoolInit()
{
	try
	{		
		//创建Recordset对象	
		HRESULT hResult;
		//WriteLog("Create Command \n");
		hResult=m_pCommand.CreateInstance(__uuidof(Command) , NULL);
		if(FAILED(hResult)) 
		{
			//WriteLog("Create Command Error\n");
			//如果创建失败，返回
			return -3;
		}
		//创建数据库连接对象
	
		hResult=m_pConn.CreateInstance(__uuidof(Connection) , NULL);
		//WriteLog("Create DB Connection \n");
		if(FAILED(hResult)) 
		{
			//WriteLog("Create DB Connection Error\n");
			//如果创建失败，返回
			return -2;
		}
	}
	catch(_com_error e)
	{
		_bstr_t bstrErr=e.Description();
		CString strErrInfo;
		strErrInfo.Format("Error Information :%s\n", (LPCSTR)bstrErr);
		//WriteLog(strErrInfo);
		return -1;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
int DBPool::ConnectToDB(char * ConnStr)
{
	try
	{
		//创建数据库连接对象        
		HRESULT hResult=m_pConn.CreateInstance(__uuidof(Connection) , NULL);
		if(FAILED(hResult)) 
		{
			//如果创建失败，返回
			return 20;
		}		
		//创建Recordset对象
		hResult=m_pRS.CreateInstance(__uuidof(Recordset));
		if(FAILED(hResult)) 
		{
			//如果创建失败，返回
			return 21;
		}
		//连接数据库
		puts(ConnStr);
		
		hResult=m_pConn->Open(ConnStr,"","",NULL);
		//m_pConn->Open("Provider=OraOLEDB.Oracle.1;Password=MON;Persist Security Info=True;User ID=MON;Data Source=MON",
		//	"","",NULL);
		if(FAILED(hResult)) 
		{
			puts("connect failed ");
			//连接失败，返回
			return 23;
		}
		
		//创建Command对象
		/*
		hResult=m_pCommand.CreateInstance(__uuidof(Command));
		if(FAILED(hResult)) 
		{
			//如果创建失败，返回
			puts("Create Command failed ");
			return 22;
		}

        //设置活动连接
		m_pCommand->ActiveConnection = m_pConn;
		//设置命令字类型
		m_pCommand->CommandType = adCmdText;
		*/
		
    }
    catch(_com_error e)
	{
		//出现运行时错误
		_bstr_t bstrErr=e.Description();
		CString strErrInfo;
		strErrInfo.Format("Error Information :%s\n", (LPCSTR)bstrErr);
//		PrintLog(strErrInfo.GetBuffer(1));
		puts(strErrInfo);
		
		//返回
		return 10;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////			//////////////////////////////
int DBPool::QueryDB(char *chSQL)
{
	try 
	{
		COleVariant vtOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);
		 m_pRS=m_pConn->Execute(_bstr_t(chSQL),&vtOptional,-1);
		 //printf("\r\n");
		while(!m_pRS->EndOfFile)
		{
			CString s(m_pRS->Fields->GetItem(_variant_t("counter_name"))->Value.bstrVal);

			int a=m_pRS->Fields->GetItem(_variant_t("cntr_value"))->Value.intVal;
			//CString s1(m_pRS->Fields->GetItem(_variant_t("cntr_value"))->Value.);
			
			m_pRS->MoveNext();
			s.TrimRight(" ");
			s.Replace(" ","_");
			s.Replace("/","_");
		//	printf("%s=%d\r\n",s,a);
		}

	}catch(_com_error e)
	{
		_bstr_t bstrErr=e.Description();
		CString strErrInfo;
		strErrInfo.Format("Error Information :%s\n", (LPCSTR)bstrErr);
		puts(strErrInfo);
		//WriteLog(strErrInfo);
		return -1;
	}
	return 0;
}

int DBPool::QueryDBDyn(char *chSQL, char *index)
{
	try 
	{
		COleVariant vtOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);
		m_pRS=m_pConn->Execute(_bstr_t(chSQL),&vtOptional,-1);
		//printf("\r\n");
		while(!m_pRS->EndOfFile)
		{
			CString s(m_pRS->Fields->GetItem(_variant_t(index))->Value.bstrVal);

			m_pRS->MoveNext();
			s.TrimRight(" ");
			s.Replace(" ","_");
			s.Replace("/","_");
			//	printf("%s=%d\r\n",s,a);
		}

	}catch(_com_error e)
	{
		_bstr_t bstrErr=e.Description();
		CString strErrInfo;
		strErrInfo.Format("Error Information :%s\n", (LPCSTR)bstrErr);
		puts(strErrInfo);
		//WriteLog(strErrInfo);
		return -1;
	}
	return 0;
}

int DBPool::QueryExecute(char *chSQL)
{
	try 
	{
		printf("QueryExecute sql = %s\n", chSQL);
		COleVariant vtOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);
		m_pRS=m_pConn->Execute(_bstr_t(chSQL),&vtOptional,-1);

	}catch(_com_error e)
	{
		_bstr_t bstrErr=e.Description();
		CString strErrInfo;
		strErrInfo.Format("Error Information :%s\n", (LPCSTR)bstrErr);
		puts(strErrInfo);
		//WriteLog(strErrInfo);
		return -1;
	}
	return 0;
}


/////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////			//////////////////////////////
int DBPool::InsertDB(char *chSQL)
{
	try 
	{
		COleVariant vtOptional((long)DISP_E_PARAMNOTFOUND,VT_ERROR);
		m_pConn->Execute(_bstr_t(chSQL),&vtOptional,-1);

	}catch(_com_error e)
	{
		_bstr_t bstrErr=e.Description();
		CString strErrInfo;
		strErrInfo.Format("Error Information :%s\n", (LPCSTR)bstrErr);
		//WriteLog(strErrInfo);
		return -1;
	}
	/*
	try
	{
		//设置查询命令
		m_pCommand->CommandText = chSQL;
		//执行查询命令
		m_pCommand->Execute(NULL , NULL , NULL);
	}
	catch(_com_error e)
	{
		_bstr_t bstrErr=e.Description();
		CString strErrInfo;
		strErrInfo.Format("Error Information :%s\n", (LPCSTR)bstrErr);
		//WriteLog(strErrInfo);
		return -1;
	}
	*/
	return 0;
}

void DBPool::CloseDB()
{
	m_pCommand.Detach();
	m_pConn->Close();
	m_pConn.Detach();
}

_variant_t DBPool::GetItem(char *Item)
{
	//取得结果集Item的值
	_variant_t vt;
	try
	{		
		vt=m_pRS->Fields->GetItem(Item)->GetValue();		
	}
	catch(_com_error e)
	{
		_bstr_t bstrErr=e.Description();
	}
	return vt;
}

int DBPool::CMDExecute()
{
	try
	{
		//查询存储过程
		m_pRS = m_pCommand->Execute(NULL, NULL,NULL);
	}
	catch(_com_error e)
	{
		//出现运行错误
		_bstr_t bstrErr=e.Description();
//		ShowMsg(bstrErr);
		return 26;
	}
	return 0;
}

int DBPool::IsEOF()
{
	return m_pRS->EndOfFile;
}

int DBPool::SetCMDText(char *CMDStr)
{
	try
	{
		//设置查询所使用的存储过程
		m_pCommand->CommandText=CMDStr;
		//m_pCommand->CommandType= adCmdText;
	}
	catch(_com_error e)
	{
		//出现运行错误
		_bstr_t bstrErr=e.Description();
//		ShowMsg(bstrErr);
		return 25;
	}
	return 0;
}

void DBPool::RSClose()
{
	m_pRS->Close();
}

void DBPool::RSMoveNext()
{
	m_pRS->MoveNext();
}

int DBPool::GetCount()
{
	int nCount = 0;
	while(!m_pRS->EndOfFile)
	{
		nCount ++;
		m_pRS->MoveNext();
	}
	if(!m_pRS->EndOfFile)
		m_pRS->MoveFirst();
	return nCount;
}

void DBPool::RSMoveFirst()
{
	m_pRS->MoveFirst();
}

_variant_t DBPool::GetSelectItem(int x1,int x2,CString realval)
{
	m_pRS->MoveFirst();

	_variant_t  vIndex,vt;
	while(!m_pRS->EndOfFile)
	{
		m_pRS->MoveNext();
		vIndex=m_pRS->Fields->GetItem(_variant_t("x1"))->Value;
		CString str(vIndex.bstrVal);
		if (str==realval)
		{
			vt=m_pRS->Fields->GetItem(_variant_t("x2"))->Value;
		}
		
				
	}
	return vt;
}

_variant_t DBPool::GetSelectItem(int x,int y)
{
	m_pRS->MoveFirst();
	for (int i=0;i<y;i++)
	{
		m_pRS->MoveNext();
		if(m_pRS->EndOfFile)  break;
	}
	_variant_t  vIndex,vt;
	
	if(!m_pRS->EndOfFile )
		vt=m_pRS->Fields->GetItem(_variant_t("x"))->Value;
	else  vt=_variant_t("0");

	return vt;
}

double DBPool::GetJshValue(int nValue ,CString strRealName)
{
	CString strName;
	
	CString stream;

	strName.Format("%s--%s",strRealName,dsn);

	#if _DEBUG
	  stream.Format("%s\\Temp\\SQLMonitor.ini",szRootPath);
	#else
	  stream.Format("%s\\Monitormanager\\Temp\\SQLMonitor.ini",szRootPath);
	#endif

	 int lastvalue ;
	 lastvalue=GetPrivateProfileInt(strName,"lastvalue",-1,(LPCSTR)stream);

	 int lasttime;
	 lasttime =GetPrivateProfileInt(strName,"lasttime",-1,(LPCSTR)stream);
	 CTime tNow =CTime::GetCurrentTime();
	 CTime tBase =CTime(2004,1,1,0,0,0,-1);
	 CTimeSpan tC;
	 tC=tNow-tBase;
	 long nCurrentTime =tC.GetTotalSeconds();
	 CString wValue;
	 wValue.Format("%d",nCurrentTime);
	 WritePrivateProfileString(strName,"lasttime",wValue.GetBuffer(wValue.GetLength()),(LPCSTR)stream);
	 wValue.Format("%d",nValue);
	 WritePrivateProfileString(strName,"lastvalue",wValue.GetBuffer(wValue.GetLength()),(LPCSTR)stream);
	
	 if(lastvalue==-1) nValue=0;
	  nValue=nValue-lastvalue;
	  if(nValue<0)  nValue=0;
	  double dValue ;
	  dValue =(double )nValue/(nCurrentTime-lasttime);
	  if(dValue<0) dValue =0;
	  return dValue;
}

int DBPool::CalBufferManager(char* szReturn)
{
	if(m_pRS==NULL) return 0;
	m_pRS->MoveFirst();
	CString strName;
	CString strTemp;
	
	double  nValue = 0;
	double  x  = 0;
	double  x1 = 0;
	while(!m_pRS->EndOfFile)
	{
		CString strName(m_pRS->Fields->GetItem(_variant_t("counter_name"))->Value.bstrVal);
		strName.TrimRight(" ");
		strName.Replace(" ", "_");
		strName.Replace("/", "_");

		if((strName.CompareNoCase("Buffer_cache_hit_ratio")!=0)&&(strName.CompareNoCase("Buffer_cache_hit_ratio_base")!=0))
		{
			if((strName.CompareNoCase("Page_reads_sec")==0)||(strName.CompareNoCase("Page_writes_sec")==0)||
				(strName.CompareNoCase("Page_life_expectancy")==0)||(strName.CompareNoCase("Lazy_writes_sec")==0)
				||(strName.CompareNoCase("Checkpoint_pages_sec")==0) || (strName.CompareNoCase("Free_pages")==0)
				|| (strName.CompareNoCase("Total_pages")==0))//yi.duan add: Free pages and Total pages 2010-11-30
			{

				int nType =0;
				nValue=m_pRS->Fields->GetItem(_variant_t("cntr_value"))->Value.intVal;
				nType =m_pRS->Fields->GetItem(_variant_t("cntr_type"))->Value.intVal;
			
				if(nType ==272696320)
				{
					double dValue =GetJshValue(nValue,strName);
					  strTemp.Format("%s=%.2f$",strName,dValue);
				}
				else		strTemp.Format("%s=%.2f$",strName,nValue);

				strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
				
			}
		}
		else
		{
			if(strName=="Buffer_cache_hit_ratio")
				x=m_pRS->Fields->GetItem(_variant_t("cntr_value"))->Value.intVal;
			if(strName=="Buffer_cache_hit_ratio_base")
			{
				x1=m_pRS->Fields->GetItem(_variant_t("cntr_value"))->Value.intVal;
				int y =100*x/x1;
				strTemp.Format("Buffer_cache_hit_ratio=%d$",y);
				strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
				
			}
		}
	
		printf("strName = %s\n", strName.GetBuffer(strName.GetLength()));
		printf("nValue = %f\n", nValue);
		m_pRS->MoveNext();
	}
	puts(szReturn);
	return 1;
}
int DBPool::CalMemoryManager(char* szReturn)
{
	m_pRS->MoveFirst();
	CString strName;
	CString strTemp;
	int nValue;

	while(!m_pRS->EndOfFile)
	{
		CString strName(m_pRS->Fields->GetItem(_variant_t("counter_name"))->Value.bstrVal);
		strName.TrimRight(" ");
		strName.Replace(" ", "_");
		strName.Replace("_(KB)", "");
		if((strName.CompareNoCase("Connection_Memory")==0)||(strName.CompareNoCase("Total_Server_Memory")==0)||
				(strName.CompareNoCase("Memory_Grants_Outstanding")==0)||(strName.CompareNoCase("Optimizer_Memory")==0)
				||(strName.CompareNoCase("SQL_Cache_Memory")==0) || (strName.CompareNoCase("Lock_Memory")==0)
				||(strName.CompareNoCase("Target_Server_Memory")==0) || (strName.CompareNoCase("Target_Server_Memory(KB)")==0))
			{

				nValue=m_pRS->Fields->GetItem(_variant_t("cntr_value"))->Value.intVal;
				int nType=0;
				nType =m_pRS->Fields->GetItem(_variant_t("cntr_type"))->Value.intVal;
				if(nType ==272696320)
				{
					double dValue =GetJshValue(nValue,strName);
					  strTemp.Format("%s=%.2f$",strName,dValue);
				}
				else		strTemp.Format("%s=%d$",strName,nValue);

				//strTemp.Format("%s=%d$",strName,nValue);
				strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
			}
			printf("strName = %s\n", strName.GetBuffer(strName.GetLength()));
			printf("nValue = %d\n", nValue);
		m_pRS->MoveNext();
	}
	puts(szReturn);
	return 1;
}
int DBPool::CalGeneralStatistics(char* szReturn)
{
	m_pRS->MoveFirst();
	CString strName;
	CString strTemp;
	int nValue;
	while(!m_pRS->EndOfFile)
	{
		CString strName(m_pRS->Fields->GetItem(_variant_t("counter_name"))->Value.bstrVal);
		strName.TrimRight(" ");
		strName.Replace(" ", "_");
		strName.Replace("/", "_");
		nValue=m_pRS->Fields->GetItem(_variant_t("cntr_value"))->Value.intVal;

		int nType=0;
		nType =m_pRS->Fields->GetItem(_variant_t("cntr_type"))->Value.intVal;
		if(nType ==272696320)
		{
			double dValue =GetJshValue(nValue,strName);
			  strTemp.Format("%s=%.2f$",strName,dValue);
		}
		else		strTemp.Format("%s=%d$",strName,nValue);
		
				
		
		strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		m_pRS->MoveNext();
	}
	puts(szReturn);
	return 1;
}
int DBPool::CalCacheStatistics(char* szReturn)
{
	m_pRS->MoveFirst();

	CString       strName;
	CString       strTemp;
	unsigned long nValue = 0;
	unsigned long      x = 0;
	unsigned long     x1 = 0;

	while(!m_pRS->EndOfFile)
	{
		CString strName(m_pRS->Fields->GetItem(_variant_t("counter_name"))->Value.bstrVal);


		strName.TrimRight(" ");
		strName.Replace(" ", "_");
		strName.Replace("/", "_");

		nValue=m_pRS->Fields->GetItem(_variant_t("cntr_value"))->Value.intVal;

		if(strName.CompareNoCase("Cache_Hit_Ratio")==0) 
		{
//yi.duan{------------------------------------------------------
			if (nValue<0)              //dy+: 如果取得值小于0,重新取值
			{
				m_pRS->MoveFirst();
				continue;
			}
//yi.duan------------------------------------------------------}
			x=nValue;
			m_pRS->MoveNext();	
			continue;
		}

		if(strName.CompareNoCase("Cache_Hit_Ratio_Base")==0) 
		{
			x1=nValue;

//yi.duan{------------------------------------------------------
			if (nValue<0 || x>x1)            //yi.duan+: 如果取得值小于0,重新取值
			{
				m_pRS->MoveFirst();
				continue;
			}
//yi.duan------------------------------------------------------}
			double   dTempA = x;               //dy+
			double   dTempB = x1;			   //dy+	
			unsigned int    iTemp;             //dy+
			
			iTemp = dTempA*100/dTempB;         //dy+ 

			strTemp.Format("%s=%d$", strName, iTemp);    //dy+
			//strTemp.Format("%s=%d$", strName, x*100/x1);  //dy:原来的,long型*100可能溢出，故用double型计算
			strcat(szReturn, strTemp.GetBuffer(strTemp.GetLength()));
			m_pRS->MoveNext();
			continue;
		}
		
		int nType=0;
		nType =m_pRS->Fields->GetItem(_variant_t("cntr_type"))->Value.intVal;
		if(nType ==272696320)
		{
			double dValue =GetJshValue(nValue,strName);
			strTemp.Format("%s=%.2f$",strName,dValue);
		}
		else
		{
			strTemp.Format("%s=%d$",strName,nValue);
		}		
		//strTemp.Format("%s=%d$",strName,nValue);
		strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		m_pRS->MoveNext();
	}

	puts(szReturn);
	return 1;

}

int DBPool::CalCacheStatistics_2005(char * szReturn)
{
	m_pRS->MoveFirst();

	CString strName;
	CString strTemp;
	int nValue,x,x1;
	while(!m_pRS->EndOfFile)
	{
		CString strName(m_pRS->Fields->GetItem(_variant_t("counter_name"))->Value.bstrVal);
		strName.TrimRight(" ");
		strName.Replace(" ", "_");
		strName.Replace("/", "_");
		nValue=m_pRS->Fields->GetItem(_variant_t("cntr_value"))->Value.intVal;
	
		if(strName.CompareNoCase("Cache_Hit_Ratio")==0) 
		{
			x=nValue;
			m_pRS->MoveNext();	
			continue;
		}
		if(strName.CompareNoCase("Cache_Hit_Ratio_Base")==0) 
		{
			x1=nValue;
			strTemp.Format("%s=%d$",strName,x*100/x1);
			strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
			m_pRS->MoveNext();
			continue;
		}
		
		int nType=0;
		nType =m_pRS->Fields->GetItem(_variant_t("cntr_type"))->Value.intVal;
/*
		if(nType==65792)
		{
			double dValue =GetJshValue(nValue,strName);
			  strTemp.Format("%s=%.2f$",strName,dValue);
		}
		else	
*/
		strTemp.Format("%s=%d$",strName,nValue);
		
		strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		m_pRS->MoveNext();
	}
	puts(szReturn);
	return 1;

}
int DBPool::CalStaticManager(char* szReturn)
{
	m_pRS->MoveFirst();
	CString strName;
	CString strTemp;
	int nValue;

	while(!m_pRS->EndOfFile)
	{
		CString strName(m_pRS->Fields->GetItem(_variant_t("counter_name"))->Value.bstrVal);
		strName.TrimRight(" ");
		strName.Replace(" ", "_");
		strName.Replace("/", "_");
		strName.Replace("-", "_");
		if((strName.CompareNoCase("Batch_Requests_sec")==0)|(strName.CompareNoCase("Auto_Param_Attempts_sec")==0)||
				(strName.CompareNoCase("SQL_Compilations_sec")==0)||(strName.CompareNoCase("SQL_Re_Compilations_sec")==0)
				||(strName.CompareNoCase("Batch_Requests_sec")==0))
			{

				nValue=m_pRS->Fields->GetItem(_variant_t("cntr_value"))->Value.intVal;

				int nType=0;
				nType =m_pRS->Fields->GetItem(_variant_t("cntr_type"))->Value.intVal;
/*				if(nType ==272696320)
				{
					double dValue =GetJshValue(nValue,strName);
					  strTemp.Format("%s=%.2f$",strName,dValue);
				}
				else		*/
					strTemp.Format("%s=%d$",strName.GetBuffer(0),nValue);



				//strTemp.Format("%s=%d$",strName,nValue);
				strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
			}
		m_pRS->MoveNext();
	}
	puts(szReturn);
	return 1;
}
//yi.duan 2010-11-30 lock 状况
int DBPool::CalLock_2005(char * szReturn)
{
	m_pRS->MoveFirst();

	CString strName;
	CString strTemp;
	unsigned long nValue = 0;

	while(!m_pRS->EndOfFile)
	{
		strName = m_pRS->Fields->GetItem(_variant_t("counter_name"))->Value.bstrVal;
		strName.TrimRight(" ");
		strName.Replace(" ", "_");
		strName.Replace("/", "_");
		strName.Replace("_(ms)", "");
		strName.Replace("(timeout_>_0)", "");
		
		nValue=m_pRS->Fields->GetItem(_variant_t("cntr_value"))->Value.intVal;
		//nValue=m_pRS->Fields->GetItem(_variant_t("cntr_value"))->Value.dblVal;


		//每个导致等待的锁请求的平均等待时间（毫秒）。
		if(strName.CompareNoCase("Average_Wait_Time")==0) 
		{
			strTemp.Format("%s=%d$",strName,nValue);
			strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		}
		//锁管理器每秒请求的新锁和锁转换数。
		else if(strName.CompareNoCase("Lock_Requests_sec")==0) 
		{
			strTemp.Format("%s=%d$",strName,nValue);
			strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		}
		//每秒超时的锁请求数，但不包括对 NOWAIT 锁的请求。
		else if(strName.CompareNoCase("Lock_Timeouts__sec")==0) 
		{
			strTemp.Format("%s=%d$",strName,nValue);
			strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		}
		//每秒超时的锁请求数，包括对 NOWAIT 锁的请求。
		else if(strName.CompareNoCase("Lock_Timeouts_sec")==0) 
		{
			strTemp.Format("%s=%d$",strName,nValue);
			strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		}
		//锁在最后一秒内的总等待时间（毫秒）。
		else if(strName.CompareNoCase("Lock_Wait_Time")==0) 
		{
			strTemp.Format("%s=%d$",strName,nValue);
			strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		}
		//每秒要求调用者等待的锁请求数。
		else if(strName.CompareNoCase("Lock_Waits_sec")==0) 
		{
			strTemp.Format("%s=%d$",strName,nValue);
			strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		}
		//每秒导致死锁的锁请求数。
		else if(strName.CompareNoCase("Number_of_Deadlocks_sec")==0) 
		{
			strTemp.Format("%s=%d$",strName, nValue);
			strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		}

		printf("strName = %s\n", strName.GetBuffer(strName.GetLength()));
		printf("nValue = %d\n", nValue);
		m_pRS->MoveNext();
	}
	printf("szReturn = %s\n", szReturn);

	return 1;
}

int DBPool::DatabaseNameDyn_2005(char * szReturn)
{
	m_pRS->MoveFirst();

	CString strName;
	CString strTemp;
	unsigned long nValue = 0;
	char *p = szReturn;

	while(!m_pRS->EndOfFile)
	{
		strName = m_pRS->Fields->GetItem(_variant_t("name"))->Value.bstrVal;
		strName.TrimRight(" ");
		strName.Replace(" ", "_");
		strName.Replace("/", "_");
		
		printf("strName = %s\n", strName.GetBuffer(strName.GetLength()));

		sprintf(p,"%s=%s",strName,strName);
		p+= 2* strlen(strName)+2;
		m_pRS->MoveNext();
	}
	//puts(szReturn);
	return 1;
}

int DBPool::DatabaseSpace_2005(char* szReturn)
{
	m_pRS->MoveFirst();

	CString strName;
	CString strTemp;
	unsigned long nValue = 0;
	char *p = szReturn;
	unsigned long nDataFileSize = 0;
	unsigned long nLogSizeSize = 0;

	while(!m_pRS->EndOfFile)
	{
		strName = m_pRS->Fields->GetItem(_variant_t("counter_name"))->Value.bstrVal;
		strName.TrimRight(" ");
		strName.Replace(" ", "_");
		strName.Replace("/", "_");
		strName.Replace("_(KB)", "");
		strName.Replace("(s)", "");
		nValue=m_pRS->Fields->GetItem(_variant_t("cntr_value"))->Value.intVal;
	
		if(strName.CompareNoCase("Data_File_Size")==0) 
		{
			nDataFileSize = nValue;
			strTemp.Format("%s=%d$",strName,nValue);
			strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		}
		else if(strName.CompareNoCase("Log_File_Size")==0) 
		{
			nLogSizeSize = nValue;
			strTemp.Format("%s=%d$",strName,nValue);
			strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		}
		
		printf("strName = %s\n", strName.GetBuffer(strName.GetLength()));
		printf("nValue = %d\n", nValue);
	
		m_pRS->MoveNext();
	}
	strTemp.Format("%s=%d$","DataBase_Size",nDataFileSize+nLogSizeSize);  //添加数据库占硬盘的空间
	strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
	//puts(szReturn);
	return 1;
}

int DBPool::CalDatabases(char* szReturn)//2011-11-30 yi.duan add 事务及日志
{
	m_pRS->MoveFirst();

	CString strName;
	CString strTemp;
	char *p = szReturn;
	unsigned long nValue = 0;

	while(!m_pRS->EndOfFile)
	{
		strName = m_pRS->Fields->GetItem(_variant_t("counter_name"))->Value.bstrVal;
		strName.TrimRight(" ");
		strName.Replace(" ", "_");
		strName.Replace("/", "_");
		strName.Replace("_(KB)", "");
		strName.Replace("(s)", "");
		//nValue=m_pRS->Fields->GetItem(_variant_t("cntr_value"))->Value.intVal;
		nValue=m_pRS->Fields->GetItem(_variant_t("cntr_value"))->Value.dblVal;

		if(strName.CompareNoCase("Active_Transactions")==0) 
		{
			strTemp.Format("%s=%d$",strName,nValue);
			strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		}
		else if(strName.CompareNoCase("Data_File_Size")==0) 
		{
			strTemp.Format("%s=%d$",strName,nValue);
			strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		}
		else if(strName.CompareNoCase("Percent_Log_Used")==0) 
		{
			strTemp.Format("%s=%d$",strName,nValue);
			strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		}
		else if(strName.CompareNoCase("Transactions_sec")==0) 
		{
			strTemp.Format("%s=%d$",strName,nValue);
			strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		}

		printf("strName = %s\n", strName.GetBuffer(strName.GetLength()));
		printf("nValue = %d\n", nValue);

		m_pRS->MoveNext();
	}
	//puts(szReturn);
	return 1;
}

int DBPool::CalIndex(char* szReturn)//2011-12-01 yi.duan add 索引
{
	m_pRS->MoveFirst();

	CString strName;
	CString strTemp;
	//unsigned long nValue = 0;
	INT64 nValue = 0;
	char *p = szReturn;

	while(!m_pRS->EndOfFile)
	{
		strName = m_pRS->Fields->GetItem(_variant_t("counter_name"))->Value.bstrVal;
		strName.TrimRight(" ");
		strName.Replace(" ", "_");
		strName.Replace("/", "_");
		strName.Replace("_(KB)", "");
		strName.Replace("(s)", "");
		nValue=m_pRS->Fields->GetItem(_variant_t("cntr_value"))->Value.intVal;

		if(strName.CompareNoCase("Forwarded_Records_sec")==0) 
		{
			strTemp.Format("%s=%d$",strName,nValue);
			strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		}
		else if(strName.CompareNoCase("Full_Scans_sec")==0) 
		{
			strTemp.Format("%s=%d$",strName,nValue);
			strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		}
		else if(strName.CompareNoCase("Page_Splits_sec")==0) 
		{
			strTemp.Format("%s=%d$",strName,nValue);
			strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		}
		else if(strName.CompareNoCase("Index_Searches_sec")==0) 
		{
			strTemp.Format("%s=%d$",strName,nValue);
			strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
		}

		printf("strName = %s\n", strName.GetBuffer(strName.GetLength()));
		printf("nValue = %d\n", nValue);

		m_pRS->MoveNext();
	}
	//puts(szReturn);
	return 1;
}