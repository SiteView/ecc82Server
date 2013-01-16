// DBPool.h: interface for the DBPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBPOOL_H__96F68960_EA7B_4A78_8E39_BC914793F997__INCLUDED_)
#define AFX_DBPOOL_H__96F68960_EA7B_4A78_8E39_BC914793F997__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class DBPool  
{
public:
	void CloseDB();
	DBPool();
	virtual ~DBPool();
	int InsertDB(char*);
    int QueryDB(char*);
	int QueryDBDyn(char *chSQL, char *index);//2011-11-30 yi.duan add 获取下拉
	int QueryExecute(char *chSQL);
protected:
	int PoolInit();
	
public:
	void RSMoveNext();
	void RSMoveFirst();
	void RSClose();
	int SetCMDText(char * CMDStr);
	int IsEOF();
	int CMDExecute();
	int GetCount();
	_variant_t GetItem(char * Item);
	_variant_t GetSelectItem(int x,int y);
	_variant_t GetSelectItem(int x1,int x2,CString realval);
	int ConnectToDB(char * ConnStr);
	int CalBufferManager(char* szReturn);
	int CalMemoryManager(char* szReturn);
	int CalGeneralStatistics(char* szReturn);
	int CalCacheStatistics(char* szReturn);
	int CalStaticManager(char* szReturn);
	int CalCacheStatistics_2005(char* szReturn);
	int CalLock_2005(char* szReturn);//2011-11-26 yi.duan add lock状况

	int DatabaseNameDyn_2005(char* szReturn);//2011-11-30 yi.duan add 获取所有数据名(下拉)
	int DatabaseSpace_2005(char* szReturn);//2011-11-30 yi.duan add 获取数据库空间状况(下拉)

	int CalDatabases(char* szReturn);//2011-11-30 yi.duan add 事务及日志
	int CalIndex(char* szReturn);//2011-11-30 yi.duan add 索引
	

    char szRootPath[256];
	double GetJshValue(int nValue,CString strRealName);
	char dsn[256];
protected:

	_ConnectionPtr m_pConn;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRS;
};

#endif // !defined(AFX_DBPOOL_H__96F68960_EA7B_4A78_8E39_BC914793F997__INCLUDED_)
