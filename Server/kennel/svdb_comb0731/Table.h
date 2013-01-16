#ifndef SVDB_TABLE_
#define	SVDB_TABLE_

#include "svdbtype.h"

#include <vector>
#include "libutil/time.h"
#include "libutil/hashtable.h"
#include "libutil/buffer.h"
#include <cc++/thread.h>

#include "Page.h"
#include "PagePool.h"
#include "RecordType.h"

//struct RecordPos{
//	RecordPos()
//	{
//		m_page=-1;
//		m_pos=-1;
//	}
//	RecordPos(const struct RecordPos &pp)
//	{
//		m_page=pp.m_page;
//		m_pos=pp.m_pos;
//	}
//
//	RecordPos & operator=(const struct RecordPos &pp)
//	{
//		m_page=pp.m_page;
//		m_pos=pp.m_pos;
//		return *this;
//	}
//
//	bool operator==(const struct RecordPos &rp)
//	{
//		return ((m_page==rp.m_page)&&(m_pos==rp.m_pos));
//	}
//
//	int m_page;
//	int m_pos;
//
//};
//
//typedef struct RecordPos	RECORDPOS, *LPRECORDPOS;


class Table
{
public:
	Table(void);
	~Table(void);

	int m_firstpage;
	int	m_currentpage;
	svutil::TTime m_createtime;
	int m_hasindex;

	RecordType m_type;

	PagePool *m_pagepool;
	Page *m_page;

	ost::Mutex m_mutex;



	bool InsertRecord(int state,char *data,int len);
	int AppendRecord(const char *rawdata,S_UINT datalen);
	int QueryRecordByTime(svutil::TTime begin,svutil::TTime end,char *buf,int &buflen);
	int QueryRecordByTime(svutil::TTime begin,svutil::TTime end,svutil::buffer &buf,int headlen,int &datalen);
	int QueryRecordByCount(S_UINT count,svutil::buffer &buf,int headlen,int &datalen);
	bool QueryRecordCount(string monitorid, int & count);

	bool DeleteData();
	bool DeleteData(svutil::TTime begin,svutil::TTime end);
	bool DeleteData(svutil::TTime before);
	int QueryDyn(char *buf,S_UINT &buflen);
	int QueryDynNOStr(char *buf,S_UINT &buflen);
	bool CreateObjByPage(Page *pgage);
	bool FlushData();
	bool BuildRecordType(char *buf,S_UINT &buflen);

	bool GetAlertInfoData(char *buf,S_UINT &buflen);


protected:

	int GetRawDataSize();
	Page *GetPreRecordPage(Page *ppage);
	Page *GetNextPage(Page *ppage);

	Page *GetFreePage();


	bool FreePage(Page *ppage);
	bool FreePage(Page *ppage,svutil::TTime begin,svutil::TTime end);
	bool FreePage(Page *ppage,svutil::TTime before);

	int GetSingleRecord(Page *ppage,int pos,char *buf,int buflen);

private:


	std::vector<DataType *> m_indextype;

	/**********************************/
	svutil::TTime m_time;
	int	 m_state;
	svutil::TTime m_laststatechangetime;
	unsigned int m_laststatekeeptimes;
	string m_displaystr;
    /**********************************/

	S_UINT GetDynSize(void);

};

typedef svutil::hashtable<svutil::word,Table *> TABLEMAP;


#endif
