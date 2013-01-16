#include "stdafx.h"
#include "utilsnap.h"
#include "utilemb.h"
#include <fstream>

using namespace std;

extern  int item[32];

typedef struct std::list<char*> StringList;

int GetDatabaseSnapshot(CStringList &paramList, struct sqlma *ma_ptr, char* szReturn);
int InstAttach(char *nodeName,char *user,char *pswd);
int InstDetach();

int PrintLog(const char * strReceive)
{
	char datebuf[128]={0},timebuf[128]={0},tempbuf[1024*10]={0};
	_strtime(timebuf);
	_strdate(datebuf);
	sprintf(tempbuf,"%s-%s",datebuf,timebuf);
	ofstream FileStream;
	FileStream.open("DB2Monitor.log",ios::app);
	FileStream<<tempbuf<<"\t"<<strReceive<<endl;
	FileStream.close();
	return 0;
}

bool MakeStringListByChar(StringList& pList, const char * pInput )
{
	const char * p;
	int nSize;
	p=pInput;
	while(*p!='\0')
	{
		nSize =(int)strlen(p);
		if( nSize>0 )
		{	
			//pList.AddHead(p);
			pList.push_back((char*)p);

		}
		p=p+nSize+1;
	}

	return true;
}

int GetCharLength(const char * pInput)
{
	const char * p;
	int nSize = 0;
	p=pInput;
	while(*p!='\0')
	{
		nSize += (int)strlen(p) + 1;
		p += strlen(p)+1;
	}

	 return nSize;
}

bool MakeCharByString(char *pOut,int &nOutSize,CString strInput )
{
	 char *p;
	
	int nSize=strInput.GetLength();
	if(nSize+2 <nOutSize)
	{
		strcpy(pOut,strInput.GetBuffer(strInput.GetLength()));
	}else return false;
	p=pOut;
	//printf("%d\n",nSize);23028830 13602067678 王波
	for(int i=0;i<nSize;i++)
	{
		if(*p=='$') 	
			*p='\0';
		p++;
	}
	nOutSize=nSize+1;
	return true;
	
}

char *FindStrValue(const char *strParas, CString str)
{
	char *pValue = NULL;
	string m_TempStr;

	std::list<char*> strList;
	MakeStringListByChar(strList, strParas);
	std::list<char *>::iterator pos = strList.begin();

	 while(pos != strList.end())
	{
		//CString strTemp = strList.GetNext(pos);
		char * strTemp = *pos;
		std::string strTemp1 = *pos;
		int m_Fpos = 0;
		
		if((m_Fpos = (int)strTemp1.find(str, 0)) >= 0)
		{
			m_TempStr = strTemp1.substr( m_Fpos + strlen(str)+1, strTemp1.size() - strlen(str) - 1); 
			pValue=(char*)malloc(m_TempStr.size()+1);
			strcpy(pValue, m_TempStr.c_str());
			
		}
		pos++;
	}

	return pValue;
	
}

BOOL UnixCommand(const char *strParas, char * szReturn, int & nSize, CStringList &paramList, struct sqlma *ma_ptr)
//BOOL UnixCommand(char *strParas, char* szReturn)
{

	 CString m_str;
	 BOOL flag=FALSE;
	 int strpos=0;

	 CString m_flag;

	 CString m_strRight;
	 string m_TempStr;

	 char *nodeName=NULL;
	 char *dbname=NULL;
	 char *user=NULL;
	 char *pswd=NULL;

	 int iRet=-1;

/*	for debug zjw
	std::list<char*> paramList;
	MakeStringListByChar(paramList, strParas);
	std::list<char *>::iterator pos = paramList.begin();

	 while(pos != paramList.end())
	{
		char * strTemp = *pos;
		strcpy(szReturn, strTemp);
		strcat(szReturn, " for debug zjw");
		OutputDebugString(szReturn);
		pos++;
	}
	 return true;
*/

	nodeName = FindStrValue(strParas, "_MachineName");
	dbname = FindStrValue(strParas, "DatabaseDir");
	user = FindStrValue(strParas, "_UserAccount");
	pswd = FindStrValue(strParas, "_PassWord");

	 OutputDebugString("---------start InstAttach() call---------------\n");
	try
	{
		iRet=InstAttach(nodeName,user,pswd);
		if (iRet != 0)
		{
			sprintf(szReturn,"error=Instance Node Connect Failure!");
			return FALSE;
		}
	}
	catch(...)
	{
		sprintf(szReturn, "error=instattach error!");
		return FALSE;
	}

	try
	{
		Snapshot snapshot;

		// turn on all the monitor switches
		iRet = snapshot.TurnOnAllMonitorSwitches();
		if (iRet != 0) return FALSE;

		// update UTIL_IMPACT_LIM parameter
		iRet = snapshot.UpdateUtilImpactLim();
		if (iRet != 0) return FALSE;

		DbConn(dbname, user, pswd);
		GetDatabaseSnapshot(paramList, ma_ptr, szReturn);
	}
	catch(...)
	{
		 sprintf(szReturn, "error=getsnapshot error!");
		 return FALSE;
	}

	// detach from the local or remote instance
	InstDetach();

	return TRUE;
}

int GetDatabaseSnapshot(CStringList &paramList, struct sqlma *ma_ptr, char* szReturn)
{
	OutputDebugString("--------------start call GetDatabaseSnapshot()-----------------\n");
  int rc = 0;                   
  Snapshot snapshot;

  rc = snapshot.GetSnapshot(paramList,ma_ptr,szReturn);

  return rc;
} 

int InstAttach(char *nodeName, char *user, char *pswd)
{
   struct sqlca sqlca;

   int iRet;

   CMutex mutex(FALSE, "connsqleatin", NULL);
 
   CSingleLock sl(&mutex); 

   sl.Lock(); 

   Sleep(1000);
 
   if(!(GetLastError() == ERROR_NOT_LOGGED_ON))
   {
      sqleatin(nodeName, user, pswd, &sqlca);
   }
    
   if(sqlca.sqlcode<0)
   {
	  iRet=-1;
   }
   else if(sqlca.sqlcode==0)
   {
  	  iRet=0;
   }
   else
   {
	  iRet=1;
   }
   sl.Unlock(); 

   return iRet;
} 

int InstDetach()
{
	struct sqlca sqlca;
	int iRet;

	// detach from an instance
	sqledtin(&sqlca);
	
	if(sqlca.sqlcode<0)
	{
		iRet=-1;
	}
	else if(sqlca.sqlcode==0)
	{
  		iRet=0;
	}
	else
	{
		iRet=1;
	}

	return iRet;
} //InAttach::InstDetach

//__declspec(dllexport)
extern "C" __declspec(dllexport) 
BOOL UnixCommand_Connects(const char *strParas, char * szReturn, int & nSize)
{
	CStringList m_CList;

	unsigned int obj_num = 2;    
	struct sqlma *ma_ptr = NULL;  
	unsigned int ma_sz;          

	ma_sz = SQLMASIZE(obj_num);
	ma_ptr = (struct sqlma *) malloc(ma_sz);
	if ( ma_ptr == NULL)
	{
		return FALSE;
	}

	memset(ma_ptr, '\0', ma_sz);
	ma_ptr->obj_num = obj_num;
	ma_ptr->obj_var[0].obj_type = SQLMA_DB2;
	ma_ptr->obj_var[1].obj_type = SQLMA_DBASE_ALL;
	
	m_CList.AddHead("SQLM_ELM_TOTAL_CONS");
	m_CList.AddHead("SQLM_ELM_LOCAL_CONS");
	m_CList.AddHead("SQLM_ELM_REM_CONS_IN");
	return UnixCommand(strParas, szReturn, nSize, m_CList, ma_ptr);
}

extern "C" __declspec(dllexport) 
BOOL UnixCommand_Agents(const char *strParas, char * szReturn, int & nSize)
{
	CStringList m_CList;

	unsigned int obj_num = 2;    
	struct sqlma *ma_ptr = NULL;  
	unsigned int ma_sz;          
	
	ma_sz = SQLMASIZE(obj_num);
	ma_ptr = (struct sqlma *) malloc(ma_sz);
	if ( ma_ptr == NULL)
	{
		return FALSE;
	}

	memset(ma_ptr, '\0', ma_sz);
	ma_ptr->obj_num = obj_num;
	ma_ptr->obj_var[0].obj_type = SQLMA_DB2;
	ma_ptr->obj_var[1].obj_type = SQLMA_APPL_ALL;

	m_CList.AddHead("SQLM_ELM_AGENTS_REGISTERED");
	m_CList.AddHead("SQLM_ELM_IDLE_AGENTS");
	m_CList.AddHead("SQLM_ELM_NUM_AGENTS");
	m_CList.AddHead("SQLM_ELM_AGENTS_WAITING_TOP");
	
	return UnixCommand(strParas, szReturn, nSize, m_CList, ma_ptr);
}

extern "C" __declspec(dllexport) 
BOOL UnixCommand_DataBase(const char *strParas, char * szReturn, int & nSize)
{
	//GetTimeZoneSec(strParas, szReturn, nSize);

	CStringList m_CList;
	unsigned int obj_num = 2;    
	struct sqlma *ma_ptr = NULL;  
	unsigned int ma_sz;          
	
	ma_sz = SQLMASIZE(obj_num);
	ma_ptr = (struct sqlma *) malloc(ma_sz);
	if ( ma_ptr == NULL)
	{
		return FALSE;
	}

	memset(ma_ptr, '\0', ma_sz);
	ma_ptr->obj_num = obj_num;
	ma_ptr->obj_var[0].obj_type = SQLMA_DB2;
	char *dbname=NULL;
	dbname = FindStrValue(strParas, "DatabaseDir");
	ma_ptr->obj_var[1].obj_type = SQLMA_DBASE;
	strncpy((char *)ma_ptr->obj_var[1].object, dbname, SQLM_OBJECT_SZ);

	m_CList.AddHead("SQLM_ELM_SERVER_INSTANCE_NAME");
	m_CList.AddHead("SQLM_ELM_DB_NAME");
	m_CList.AddHead("SQLM_ELM_DB_PATH");
	m_CList.AddHead("SQLM_ELM_INPUT_DB_ALIAS");
	m_CList.AddHead("SQLM_ELM_DB_STATUS");
	m_CList.AddHead("SQLM_ELM_DB_CONN_TIME");
	m_CList.AddHead("SQLM_ELM_TIME_ZONE_DISP");
	m_CList.AddHead("SQLM_ELM_DEADLOCKS");
	m_CList.AddHead("SQLM_ELM_TOTAL_LOG_USED");
	m_CList.AddHead("SQLM_ELM_TOTAL_LOG_AVAILABLE");
	m_CList.AddHead("SQLM_ELM_SORT_OVERFLOWS");
	
	BOOL bRet = UnixCommand(strParas, szReturn, nSize, m_CList, ma_ptr);
	if(bRet)
	{
		unsigned int unInt = atoi(FindStrValue(szReturn, "SQLM_ELM_DB_CONN_TIME"));
		unInt += atoi(FindStrValue(szReturn, "SQLM_ELM_TIME_ZONE_DISP"));
		COleDateTimeSpan tSpan;
		tSpan.SetDateTimeSpan(0,0,0,unInt);
		
		COleDateTime tConnTime = COleDateTime::COleDateTime(1970,1,1,0,0,0)  + tSpan;
		
		CString strReturn="";
		strReturn.Format("SQLM_ELM_SERVER_INSTANCE_NAME=%s$SQLM_ELM_DB_NAME=%s$SQLM_ELM_DB_PATH=%s$\
			SQLM_ELM_INPUT_DB_ALIAS=%s$SQLM_ELM_DB_STATUS=%s$SQLM_ELM_DB_CONN_TIME=%s$SQLM_ELM_DEADLOCKS=%s$\
			SQLM_ELM_TOTAL_LOG_USED=%s$SQLM_ELM_TOTAL_LOG_AVAILABLE=%s$SQLM_ELM_SORT_OVERFLOWS=%s$", 
			FindStrValue(szReturn, "SQLM_ELM_SERVER_INSTANCE_NAME"),FindStrValue(szReturn, "SQLM_ELM_DB_NAME"),
			FindStrValue(szReturn, "SQLM_ELM_DB_PATH"),FindStrValue(szReturn, "SQLM_ELM_INPUT_DB_ALIAS"),
			FindStrValue(szReturn, "SQLM_ELM_DB_STATUS"),tConnTime.Format("%Y-%m-%d %H:%M:%S"), 
			FindStrValue(szReturn, "SQLM_ELM_DEADLOCKS"),FindStrValue(szReturn, "SQLM_ELM_TOTAL_LOG_USED"),
			FindStrValue(szReturn, "SQLM_ELM_TOTAL_LOG_AVAILABLE"),FindStrValue(szReturn, "SQLM_ELM_SORT_OVERFLOWS"));
		strReturn.Replace("\t", NULL);
		strcpy(szReturn,strReturn);
	}

	CString strOutRet;
	strOutRet =szReturn;
	nSize = 2048;
	MakeCharByString(szReturn,nSize,strOutRet);
	return bRet;
}

extern "C" __declspec(dllexport) 
BOOL UnixCommand_Transation(const char *strParas, char * szReturn, int & nSize)
{
	CStringList m_CList;
	unsigned int obj_num = 2;    
	struct sqlma *ma_ptr = NULL;  
	unsigned int ma_sz;          
	
	ma_sz = SQLMASIZE(obj_num);
	ma_ptr = (struct sqlma *) malloc(ma_sz);
	if ( ma_ptr == NULL)
	{
		return FALSE;
	}

	memset(ma_ptr, '\0', ma_sz);
	ma_ptr->obj_num = obj_num;
	ma_ptr->obj_var[0].obj_type = SQLMA_DB2;
	ma_ptr->obj_var[1].obj_type = SQLMA_DBASE_ALL;

	m_CList.AddHead("SQLM_ELM_FAILED_SQL_STMTS");
	m_CList.AddHead("SQLM_ELM_DYNAMIC_SQL_STMTS");
	m_CList.AddHead("SQLM_ELM_STATIC_SQL_STMTS");
	m_CList.AddHead("SQLM_ELM_COMMIT_SQL_STMTS");
	m_CList.AddHead("SQLM_ELM_INT_COMMITS");
	m_CList.AddHead("SQLM_ELM_ROLLBACK_SQL_STMTS");
	m_CList.AddHead("SQLM_ELM_INT_ROLLBACKS");
	
	BOOL bRet = UnixCommand(strParas, szReturn, nSize, m_CList, ma_ptr);
	if(bRet)
	{
		int nFailedSqlStmts = atoi(FindStrValue(szReturn, "SQLM_ELM_FAILED_SQL_STMTS"));
		int nDynamicSqlStmts = atoi(FindStrValue(szReturn, "SQLM_ELM_DYNAMIC_SQL_STMTS"));
		int nStaticSqlStmts = atoi(FindStrValue(szReturn, "SQLM_ELM_STATIC_SQL_STMTS"));
		int nCommitSqlStmts = atoi(FindStrValue(szReturn, "SQLM_ELM_COMMIT_SQL_STMTS"));
		int nIntCommits = atoi(FindStrValue(szReturn, "SQLM_ELM_INT_COMMITS"));
		int nRollbackSqlStmts = atoi(FindStrValue(szReturn, "SQLM_ELM_ROLLBACK_SQL_STMTS"));
		int nIntRollbacks = atoi(FindStrValue(szReturn, "SQLM_ELM_INT_ROLLBACKS"));

		CString strReturn="";
		strReturn.Format("SQL_ELM_SUCCESS_QUERY=%d$SQL_ELM_FAILURE_QUERY=%d$SQL_ELM_UNITS_OF_WORK=%d$", 
			nCommitSqlStmts - nFailedSqlStmts, nFailedSqlStmts, 
			nCommitSqlStmts + nIntCommits + nRollbackSqlStmts + nIntRollbacks);
		
		strcpy(szReturn,strReturn);
	}

	CString strOutRet;
	strOutRet =szReturn;
	nSize = 2048;
	MakeCharByString(szReturn,nSize,strOutRet);	 
	return bRet;
}

extern "C" __declspec(dllexport) 
BOOL UnixCommand_Tablespace(const char *strParas, char * szReturn, int & nSize)
{
	CStringList m_CList;
	unsigned int obj_num = 1;    
	struct sqlma *ma_ptr = NULL;  
	unsigned int ma_sz;          
	
	ma_sz = SQLMASIZE(obj_num);
	ma_ptr = (struct sqlma *) malloc(ma_sz);
	if ( ma_ptr == NULL)
	{
		return FALSE;
	}

	memset(ma_ptr, '\0', ma_sz);
	ma_ptr->obj_num = obj_num;
	char *dbname=NULL;
	dbname = FindStrValue(strParas, "DatabaseDir");
	ma_ptr->obj_var[0].obj_type = SQLMA_DBASE_TABLESPACES;
	strncpy((char *)ma_ptr->obj_var[0].object, dbname, SQLM_OBJECT_SZ);

	m_CList.AddHead("SQLM_ELM_TABLESPACE_EXTENT_SIZE");
	m_CList.AddHead("SQLM_ELM_TABLESPACE_PREFETCH_SIZE");
	m_CList.AddHead("SQLM_ELM_TABLESPACE_PAGE_SIZE");
	m_CList.AddHead("SQLM_ELM_TABLESPACE_CUR_POOL_ID");
	m_CList.AddHead("SQLM_ELM_TABLESPACE_NEXT_POOL_ID");
	m_CList.AddHead("SQLM_ELM_TABLESPACE_TOTAL_PAGES");
	m_CList.AddHead("SQLM_ELM_TABLESPACE_USED_PAGES");

	BOOL bRet = UnixCommand(strParas, szReturn, nSize, m_CList, ma_ptr);
	if(bRet)
	{
		float fTotalPages = (float)atof(FindStrValue(szReturn, "SQLM_ELM_TABLESPACE_TOTAL_PAGES"));
		float fUsedPages = (float)atof(FindStrValue(szReturn, "SQLM_ELM_TABLESPACE_USED_PAGES"));
		float fUseRate = (float)fUsedPages*100/fTotalPages;
		
		CString str;
		str.Format("SQLM_ELM_TABLESPACE_USED_RATE=%.2f", fUseRate);
		memcpy(szReturn + GetCharLength(szReturn), str.GetBuffer(), str.GetLength());
	}
	return bRet;
}

extern "C" __declspec(dllexport) 
BOOL UnixCommand_Buffer(const char *strParas, char * szReturn, int & nSize)
{
	CStringList m_CList;
	unsigned int obj_num = 1;    
	struct sqlma *ma_ptr = NULL;  
	unsigned int ma_sz;          
	
	ma_sz = SQLMASIZE(obj_num);
	ma_ptr = (struct sqlma *) malloc(ma_sz);
	if ( ma_ptr == NULL)
	{
		return FALSE;
	}

	memset(ma_ptr, '\0', ma_sz);
	ma_ptr->obj_num = obj_num;
	char *dbname=NULL;
	dbname = FindStrValue(strParas, "DatabaseDir");
	ma_ptr->obj_var[0].obj_type = SQLMA_BUFFERPOOLS_ALL;
	//ma_ptr->obj_var[1].obj_type = SQLMA_DBASE_BUFFERPOOLS;
	//strncpy((char *)ma_ptr->obj_var[1].object, dbname, SQLM_OBJECT_SZ);

	m_CList.AddHead("SQLM_ELM_BUFFERPOOL");
	m_CList.AddHead("SQLM_ELM_POOL_DATA_L_READS");
	m_CList.AddHead("SQLM_ELM_POOL_DATA_P_READS");
	m_CList.AddHead("SQLM_ELM_POOL_INDEX_L_READS");
	m_CList.AddHead("SQLM_ELM_POOL_INDEX_P_READS");
	m_CList.AddHead("SQLM_ELM_DIRECT_READS");
	m_CList.AddHead("SQLM_ELM_DIRECT_WRITES");
	m_CList.AddHead("SQLM_ELM_BP_CUR_BUFFSZ");
	
	BOOL bRet = UnixCommand(strParas, szReturn, nSize, m_CList, ma_ptr);
	if(bRet)
	{
		float fDataLReads = (float)atof(FindStrValue(szReturn, "SQLM_ELM_POOL_DATA_L_READS"));
		float fDataPReads = (float)atof(FindStrValue(szReturn, "SQLM_ELM_POOL_DATA_P_READS"));
		float fIndexLReads = (float)atof(FindStrValue(szReturn, "SQLM_ELM_POOL_INDEX_L_READS"));
		float fIndexPReads = (float)atof(FindStrValue(szReturn, "SQLM_ELM_POOL_INDEX_P_READS"));
		int nDirectReads = atoi(FindStrValue(szReturn, "SQLM_ELM_DIRECT_READS"));
		int nDirectWrites = atoi(FindStrValue(szReturn, "SQLM_ELM_DIRECT_WRITES"));
		int nBufSize = atoi(FindStrValue(szReturn, "SQLM_ELM_BP_CUR_BUFFSZ"));

		CString strReturn="";
		if(fDataLReads > 0 && fIndexLReads > 0)
		{
			float fDataRet = 100-fDataPReads*100/fDataLReads;
			float fIndexRet = 100-fIndexPReads*100/fIndexLReads;
			float fPoolRet = 100 - (fDataPReads + fIndexPReads)*100/(fDataLReads + fIndexLReads);
			strReturn.Format("SQLM_ELM_BUFFER_POOL_HIT_RATIO=%.2f$SQLM_ELM_INDEX_PAGE_HIT_RATIO=%.2f$SQLM_ELM_DATA_HIT_RATIO=%.2f$SQLM_ELM_DIRECT_READS=%d$SQLM_ELM_DIRECT_WRITES=%d$", 
							fPoolRet, fIndexRet, fDataRet, nDirectReads, nDirectWrites);
		}
		else
			strReturn.Format("SQLM_ELM_BUFFER_POOL_HIT_RATIO=NOT USE$SQLM_ELM_INDEX_PAGE_HIT_RATIO=NOT USE$SQLM_ELM_DATA_HIT_RATIO=NOT USE$SQLM_ELM_DIRECT_READS=%d$SQLM_ELM_DIRECT_WRITES=%d$", 
							nDirectReads, nDirectWrites);
		
		strcpy(szReturn,strReturn);
	}

	CString strOutRet;
	strOutRet =szReturn;
	nSize = 2048;
	MakeCharByString(szReturn,nSize,strOutRet);	 
	return bRet;
}

extern "C" __declspec(dllexport) 
BOOL UnixCommand_Cache(const char *strParas, char * szReturn, int & nSize)
{
	CStringList m_CList;
	unsigned int obj_num = 2;    
	struct sqlma *ma_ptr = NULL;  
	unsigned int ma_sz;          
	
	ma_sz = SQLMASIZE(obj_num);
	ma_ptr = (struct sqlma *) malloc(ma_sz);
	if ( ma_ptr == NULL)
	{
		return FALSE;
	}

	memset(ma_ptr, '\0', ma_sz);
	ma_ptr->obj_num = obj_num;
	char *dbname=NULL;
	dbname = FindStrValue(strParas, "DatabaseDir");
	ma_ptr->obj_var[0].obj_type = SQLMA_DBASE_ALL;
	ma_ptr->obj_var[1].obj_type = SQLMA_BUFFERPOOLS_ALL;
	//ma_ptr->obj_var[1].obj_type = SQLMA_DBASE_BUFFERPOOLS;
	//strncpy((char *)ma_ptr->obj_var[1].object, dbname, SQLM_OBJECT_SZ);

	m_CList.AddHead("SQLM_ELM_PKG_CACHE_INSERTS");
	m_CList.AddHead("SQLM_ELM_PKG_CACHE_LOOKUPS");
	m_CList.AddHead("SQLM_ELM_CAT_CACHE_INSERTS");
	m_CList.AddHead("SQLM_ELM_CAT_CACHE_LOOKUPS");
	
	BOOL bRet = UnixCommand(strParas, szReturn, nSize, m_CList, ma_ptr);
	if(bRet)
	{
		float fPkgInserts = (float)atof(FindStrValue(szReturn, "SQLM_ELM_PKG_CACHE_INSERTS"));
		float fPkgLookups = (float)atof(FindStrValue(szReturn, "SQLM_ELM_PKG_CACHE_LOOKUPS"));
		float fCatInserts = (float)atof(FindStrValue(szReturn, "SQLM_ELM_CAT_CACHE_INSERTS"));
		float fCatLookups = (float)atof(FindStrValue(szReturn, "SQLM_ELM_CAT_CACHE_LOOKUPS"));

		CString strReturn="";
		if(fPkgLookups > 0 && fCatLookups > 0)
		{
			float fPkgRet = 100-fPkgInserts*100/fPkgLookups;
			float fCatRet = 100-fCatInserts*100/fCatLookups;
			strReturn.Format("SQLM_ELM_PACKAGE_CACHE_HIT_RATIO=%.2f$SQLM_ELM_CATALOG_CACHE_HIT_RATIO=%.2f$", 
				fPkgRet, fCatRet);
		}
		else
			strReturn = "SQLM_ELM_PACKAGE_CACHE_HIT_RATIO=NOT USE$SQLM_ELM_CATALOG_CACHE_HIT_RATIO=NOT USE$";
		
		strcpy(szReturn,strReturn);
	}

	CString strOutRet;
	strOutRet =szReturn;
	nSize = 2048;
	MakeCharByString(szReturn,nSize,strOutRet);	 
	return bRet;
}

extern "C" __declspec(dllexport) 
BOOL UnixCommand_Backup(const char *strParas, char * szReturn, int & nSize)
{
	CStringList m_CList;
	unsigned int obj_num = 2;    
	struct sqlma *ma_ptr = NULL;  
	unsigned int ma_sz;          
	
	ma_sz = SQLMASIZE(obj_num);
	ma_ptr = (struct sqlma *) malloc(ma_sz);
	if ( ma_ptr == NULL)
	{
		return FALSE;
	}
	/*
	unsigned __int64 ulHisValue = 0, ulValue = 0,uSubValue = 0;
	ulHisValue = (unsigned __int64)_atoi64("3199935970");
	ulValue = (unsigned __int64)_atoi64("3263263555 ");
	uSubValue = ulValue - ulHisValue;
	*/
	memset(ma_ptr, '\0', ma_sz);
	ma_ptr->obj_num = obj_num;
	ma_ptr->obj_var[0].obj_type = SQLMA_DB2;
	char *dbname=NULL;
	dbname = FindStrValue(strParas, "DatabaseDir");
	ma_ptr->obj_var[1].obj_type = SQLMA_DBASE;
	strncpy((char *)ma_ptr->obj_var[1].object, dbname, SQLM_OBJECT_SZ);

	m_CList.AddHead("SQLM_ELM_LAST_BACKUP");
	m_CList.AddHead("SQLM_ELM_TIME_ZONE_DISP");
	
	BOOL bRet = UnixCommand(strParas, szReturn, nSize, m_CList, ma_ptr);
	if(bRet)
	{
		unsigned int unInt = atoi(FindStrValue(szReturn, "SQLM_ELM_LAST_BACKUP"));
		unInt += atoi(FindStrValue(szReturn, "SQLM_ELM_TIME_ZONE_DISP"));
		COleDateTimeSpan tSpan;
		tSpan.SetDateTimeSpan(0,0,0,unInt);
		
		COleDateTime tBackup = COleDateTime::COleDateTime(1970,1,1,0,0,0)  + tSpan;
		tSpan = COleDateTime::GetCurrentTime() - tBackup;

		CString strReturn="";
		strReturn.Format("SQLM_ELM_LAST_BACKUP=%s$SQLM_ELM_LAST_BACKUP_DAYS=%ld$", tBackup.Format("%Y-%m-%d %H:%M:%S"), tSpan.GetDays());
		strcpy(szReturn,strReturn);
	}

	CString strOutRet;
	strOutRet =szReturn;
	nSize = 2048;
	MakeCharByString(szReturn,nSize,strOutRet);	 
	return bRet;
}

extern "C" __declspec(dllexport)
BOOL TestDB2Conn(const char * strParas , char * szReturn , int &nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	 CString m_str;
	 BOOL flag=FALSE;
	 int strpos=0;

	 CString m_flag;

	 CString m_strRight;
	 string m_TempStr;

	 char *nodeName=NULL;
	 char *dbname=NULL;
	 char *user=NULL;
	 char *pswd=NULL;

	 int iRet=-1;
	 int BeginTime=0,EndTime=0;

	nodeName = FindStrValue(strParas, "_MachineName");		//获取到DB2节点名
	dbname = FindStrValue(strParas, "DatabaseDir");			//获取DB2数据库节点名
	user = FindStrValue(strParas, "_UserAccount");
	pswd = FindStrValue(strParas, "_PassWord");

	try
	{
		BeginTime=GetTickCount();
		iRet=InstAttach(nodeName,user,pswd);	//实例节点连接？
		if (iRet != 0)
		{
			sprintf(szReturn,"error=Instance Node Connect Failure!");
			return FALSE;
		}
		
	}
	catch(...)
	{
		sprintf(szReturn, "error=instattach error!");
		return FALSE;
	}
	InstDetach();
	EndTime=GetTickCount();
	sprintf(szReturn,"status=OK$time(毫秒)=%d$",EndTime-BeginTime);
	CString strInput;
	strInput=szReturn;
	MakeCharByString(szReturn,nSize,strInput);
	return TRUE;

}