#ifndef  DRAGONFLOW_BACKUPRECORD
#define DRAGONFLOW_BACKUPRECORD

#include "threadex.h"
#include <svdbapi.h>
#include "backuprecordDamon.h"

class CBackupRecordDamon;

class CTableInfo
{
public:
	CTableInfo():local_tplId(-1),center_tplId(-1),created(false)
	{
	}

	CTableInfo(int in_local_tplId, int in_center_tplId, bool in_created)
	{
		local_tplId= in_local_tplId;
		center_tplId= in_center_tplId;
		created= in_created;
	}

	~CTableInfo()
	{
	}

	int  local_tplId;
	int  center_tplId;
	bool created;
};



class CBackupRecord :
	public ThreadEx
{
public:
	CBackupRecord(void * pDB, CBackupRecordDamon * pDamon);
	virtual ~CBackupRecord();
	virtual void run(void);
	void toExit(void);

	bool AppendRecord(int mTplId,string tablename,const char *pdata,int datalen);

private:
	bool MoveOneStep();
	bool AppendMassRecord(std::list<SingelRecord> & listrcd,string user,string addr);
	bool tryGetAllResourceInfo();

	int BackupThenClearAllRecords(std::list<SingelRecord> & inrcd, int & size);
	bool CacheRecords(string tablename,const char *pdata,int datalen);
	void SwapWillCreateTables(std::map<string,CTableInfo> & tables);
	std::map<string,CTableInfo> PreCreateTable(std::map<string,CTableInfo> & tables);
	void InitTablesInfo(bool earliest=true);

	bool CacheSaver(std::list<SingelRecord> & listrcd, int infoSize);
	bool CacheInLocalDisk(std::list<SingelRecord> & listrcd, std::map<string,CTableInfo> & tables, int infoSize);
	bool CheckCacheStuff();
	bool SendOutCache(int messagecount, int infoSize);
	bool DeleteOneCacheData(int & count, bool show=true);
	bool FilterRefusedRecord(std::list<SingelRecord> & listrcd);

	void setVisit(std::string key, time_t time);

	std::map<string,CTableInfo> m_tables_info;
	std::map<string,CTableInfo> m_tables_willcreate;
	std::list<SingelRecord> m_cache_recods;
	ost::Mutex m_records_Lock;

	void *m_pDB;
	CBackupRecordDamon * m_pDamon;
	bool m_connectable;
	bool m_cachedStuff;
	bool m_willSleep;
	bool m_exitAnyway;
};

#endif
