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
    char szRootPath[256];
	double GetJshValue(int nValue,CString strRealName);
	char dsn[256];
public:

	_ConnectionPtr m_pConn;
	_CommandPtr m_pCommand;
	_RecordsetPtr m_pRS;
};

#endif // !defined(AFX_DBPOOL_H__96F68960_EA7B_4A78_8E39_BC914793F997__INCLUDED_)
