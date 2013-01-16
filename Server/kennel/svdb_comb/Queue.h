#ifndef SVDB_QUEUE_
#define SVDB_QUEUE_
#include "Table.h"
#include "QueueRecord.h"

//struct QueueHead{
//
//	QueueHead()
//	{
//		m_prerecord.m_page=-1;
//		m_prerecord.m_pos=-1;
//
//		m_nextrecord.m_page=-1;
//		m_nextrecord.m_pos=-1;
//		m_level=1;
//		m_datalen=0;
//	}
//	~QueueHead()
//	{
//		;
//	}
//
//	svutil::TTime m_createtime;
//	S_UINT			m_datalen;
//	RECORDPOS			m_prerecord;
//	RECORDPOS			m_nextrecord;
//	int			m_level;
//};
//
//typedef struct QueueHead QUEUEHEAD, *LPQUEUEHEAD;

class Queue : public Table
{
public:
	Queue(void);
	~Queue(void);

	enum{
		commonqueue=1,
		tempqueue=2,
		priorityqueue=3,
		temppriorityqueue=4
	};

	enum level{
		lowlevel=0,
		commonlevel=1,
		highlevel=2	
	};

	bool Append(const char * data,S_UINT len,int level=commonlevel);
	int PopTop(svutil::buffer &buf,S_UINT &datalen,bool block=false,S_UINT timer=0);
	int Peek(svutil::buffer &buf,S_UINT &datalen,bool block=false,S_UINT timer=0);
	bool QueryAllRecord(svutil::buffer &buf,int &datalen);
	bool GetQueueAllMessageLabels(std::list<string> & retlist);
	int  GetMessageData(std::list<string> & retlist, int & page, int & pos);
	bool GetLabelData(std::list<string> & retlist, const char * pt,S_UINT len);

	S_UINT GetCount(void)
	{
		return m_recordcount;
	}


	S_UINT GetRecordCount()
	{
		return m_recordcount;
	}

	bool Clear();

	bool FlushData();
	int GetRawDataSize();
	bool CreateObjByPage(Page *pgage);
	std::string m_queuename;
	bool DeleteData();


private:

	int PopTopRecord(char *buf,S_UINT &buflen);
	int PeekTopRecord(char *buf,S_UINT &buflen);


	bool SetNext(RECORDPOS src,RECORDPOS pos);
	bool SetNext(int srcpage,int srcpos,int page,int pos);

	ost::Event m_event;

	int	m_queuetype;
	S_UINT	m_recordcount;
	RECORDPOS	m_top;
	RECORDPOS	m_last;

	RECORDPOS	m_highlast;
	RECORDPOS	m_commonlast;
    

};

typedef svutil::hashtable<svutil::word,Queue *> QUEUEMAP;


#endif
