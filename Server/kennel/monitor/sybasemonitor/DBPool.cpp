// DBPool.cpp: implementation of the DBPool class.
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "DBPool.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern void WriteLogFile(CString);

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
		hResult=m_pConn->Open(ConnStr,"","",NULL);
		//m_pConn->Open("Provider=OraOLEDB.Oracle.1;Password=MON;Persist Security Info=True;User ID=MON;Data Source=MON",
		//	"","",NULL);
		if(FAILED(hResult)) 
		{
			//连接失败，返回
			return 23;
		}
		
		//创建Command对象
		hResult=m_pCommand.CreateInstance(__uuidof(Command));
		if(FAILED(hResult)) 
		{
			//如果创建失败，返回
			return 22;
		}

        //设置活动连接
		m_pCommand->ActiveConnection = m_pConn;
		//设置命令字类型
		m_pCommand->CommandType = adCmdText;
		
    }
    catch(_com_error e)
	{
		//出现运行时错误
		_bstr_t bstrErr=e.Description();		
		
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
	 long nCurrentTime =(long)tC.GetTotalSeconds();
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
	  return dValue;
}

int DBPool::CalBufferManager(char* szReturn)
{
	if(m_pRS==NULL) return 0;
	m_pRS->MoveFirst();
	CString strName;
	CString strTemp;
	int nValue;
	int x,x1;
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
				||(strName.CompareNoCase("Checkpoint_pages_sec")==0)
				)
			{

				int nType =0;
				nValue=m_pRS->Fields->GetItem(_variant_t("cntr_value"))->Value.intVal;
				nType =m_pRS->Fields->GetItem(_variant_t("cntr_type"))->Value.intVal;
				if(nType ==272696320)
				{
					double dValue =GetJshValue(nValue,strName);
					  strTemp.Format("%s=%.2f$",strName,dValue);
				}
				else		strTemp.Format("%s=%d$",strName,nValue);

				strcat(szReturn,strTemp.GetBuffer(strTemp.GetLength()));
				
			}
			



		}
		else{
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
		if((strName.CompareNoCase("Connection_Memory")==0)|(strName.CompareNoCase("Total_Server_Memory")==0)||
				(strName.CompareNoCase("Memory_Grants_Outstanding")==0)||(strName.CompareNoCase("Optimizer_Memory")==0)
				||(strName.CompareNoCase("SQL_Cache_Memory")==0)
				)
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
		if(nType ==272696320)
		{
			double dValue =GetJshValue(nValue,strName);
			  strTemp.Format("%s=%.2f$",strName,dValue);
		}
		else		strTemp.Format("%s=%d$",strName,nValue);
		




		//strTemp.Format("%s=%d$",strName,nValue);
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
				)
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
		m_pRS->MoveNext();
	}
	puts(szReturn);
	return 1;
}