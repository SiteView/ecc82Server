#ifndef SVDB_DB_
#define SVDB_DB_

#include "svdbtype.h"

#include "Table.h"
#include "PagePool.h"

#include "Queue.h"

#include "SvdbMain.h"

struct DBhead
{
	DBhead()
	{
		m_version=1;
		m_currentid=0;
		m_monitorcount=0;
		memset(m_dbfilename,0,256);

	}
	int m_version;
	int m_currentid;
	int m_monitorcount;
	char m_dbfilename[256];

};

struct DYN{
	DYN()
	{
		m_state=0;
		m_laststatekeeptimes=0;
		m_displaystr=NULL;
	}
	~DYN()
	{
		if(m_displaystr!=NULL)
			free(m_displaystr);
	}

	svutil::TTime m_time;
	int	 m_state;
	svutil::TTimeSpan m_keeplaststatetime;
	unsigned int m_laststatekeeptimes;
	char *m_displaystr;

};


class DB
{
public:
	DB(void);
	DB(string dbfilename,string dbheadfile);
	~DB(void);

	enum { inittablemapsize = 5771,initqueuemapsize = 1777 };

	bool Quit();

	bool Load();
	bool LoadEx();
	bool CreateNew(string dbheadfile,string dbfilename,int pagesize,int pagecount);
	bool CreateNewEx(string dbheadfile,string dbfiletitle,int pagesize=PAGESIZE,int pagecount=1000,int perfilesize=PERFILESIZE);
	int QueryRecordByTime(string monitorid,svutil::TTime begin,svutil::TTime end,char *buf,int &buflen);
	int QueryRecordByTimeEx(string monitorid,svutil::TTime begin,svutil::TTime end,svutil::buffer &buf,int &datalen);
	int QueryRecordByCount(string monitorid,S_UINT count,svutil::buffer &buf,int &datalen);
	int QueryDyn(string monitorid,char *buf,S_UINT&buflen);
	int QueryMassDyn(string pid, std::map <string, svutil::TTime> & times, std::list<SingelRecord> & listrcd );
	int QueryDynNOStr(string monitorid,char *buf,S_UINT &buflen);
	bool DeleteTable(string monitorid);
	bool DeleteRecordsByTime(string monitorid,svutil::TTime before);
	bool QueryRecordCount(string monitorid, int & count);

	bool GetAllTableNames(std::list<string> &namelist);

	int AppendRecord(string monitorid,const char *data,S_UINT datalen);
	int AppendMassRecord(std::list<SingelRecord> & listrcd);
	bool InsertRecord(string monitorid,int state,char *data,int datalen);
	bool InsertTableEx(string monitorid,int monitortype);
	bool InsertTable(string monitorid,int monitortype);

	bool  CreateQueue(string queuename,int queuetype=1);
	bool  AppendQueueMessage(string queuename,const char *buf,S_UINT buflen);
	int  PopQueueMessage(string queuename,svutil::buffer &buf,S_UINT &datalen,bool block=false,S_UINT timer=0);
	int  PeekQueueMessage(string queuename,svutil::buffer&buf,S_UINT &datalen,bool block=false,S_UINT timer=0);
	int GetQueueRecordCount(string queuename,S_UINT &count);
	int ClearQueueRecords(string queuename);
	int DeleteQueue(string queuename);
	bool GetAllQueueNames(std::list<string> &namelist);
	bool GetQueueAllMessageLabels(word queuename,std::list<string> & retlist);


	bool Init(string dbfilename,SvdbMain *pMain,int PerFileSize=PERFILESIZE,int PageSize=PAGESIZE);
	bool Init(string idcuser,string dbfilename,SvdbMain *pMain,int PerFileSize=PERFILESIZE,int PageSize=PAGESIZE);

	S_UINT Debug_GetFreeDataPage()
	{
		return m_pagepool->GetFreePageCount();
	}

	string	m_dbfilename;
	string	m_dbheadfile;

	PagePool *GetDPagePool()
	{
		return m_pagepool;
	}
	PagePool *GetHPagePool()
	{
		return m_headpool;
	}

protected:

	bool FlushData();
	struct DBhead *m_dbh;

	TABLEMAP m_tables;
	QUEUEMAP m_queues;
	ost::Mutex m_tablemutex;
	ost::Mutex m_queuemutex;
	PagePool *m_pagepool;
	PagePool *m_headpool;

	SvdbMain *m_pMain;


};

#endif
