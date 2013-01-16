
#ifndef		SVDB_TABLELOCALCACHE_H_
#define		SVDB_TABLELOCALCACHE_H_

#include <cc++/thread.h>

#include "svbase.h"
#include "svclient.h"

using ost::Mutex;

class SvTableCache
{
	SvTableCache(void);
	~SvTableCache(void);

public:
	static bool ClearAllCacheData();

	static CSvTable * GetDataFromServer(CTableName tname, string & estr);
	static CSvTable * GetDataFromCache(CTableName tname, string & estr);

	static bool RefreshBatchData(const std::set<CTableName> & tnames, string & estr);
	static bool RefreshDB(string dbname, string & estr);

	static bool SubmitData(CSvTable * ptable, string message, string & estr);	
	static bool SubmitAllCacheData(string message, string & estr);

private:

	static CSvTable * JustGetData(CTableName tname, string & estr);

	static map<CTableName, CSvTable * > m_alldata;
	static Mutex m_LockClientCache;

};


#endif