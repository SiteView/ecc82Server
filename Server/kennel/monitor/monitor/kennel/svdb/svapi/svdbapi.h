/*************************************************
*  @file svdbapi.h
*  author :		chen xin gang
*  Copyright (C) 2005-2006 dragonflow corp.
*
*************************************************/


#ifndef	SITEVIEW_SVDBAPI_
#define	SITEVIEW_SVDBAPI_

#include "svapi.h"

typedef int BOOL;

#include "../libutil/Time.h"

using namespace chen;

//typedef unsigned long	RECORD;
//typedef unsigned long	RECORDSET;
//
//typedef unsigned long	MQRECORD;

typedef void*	RECORD;
typedef void*	RECORDSET;

typedef void*	MQRECORD;

struct sv_dyn{
	sv_dyn()
	{
		m_state=0;
		m_laststatekeeptimes=0;
		m_displaystr=NULL;
	}
	~sv_dyn()
	{
		if(m_displaystr!=NULL)
			free(m_displaystr);
	}

	TTime m_time;
	int	 m_state;
	TTimeSpan m_keeplaststatetime;
	unsigned int m_laststatekeeptimes;
	char *m_displaystr;

};

typedef struct sv_dyn	SVDYN;

///////////////////////////////////svdyn/////////////////////

SVAPI_API 
bool GetSVDYN(string monitorid,SVDYN &dyn,string user="default",string addr="localhost");

SVAPI_API
bool BuildDynByData(const char *data,unsigned int len,SVDYN &dyn);

SVAPI_API
bool GetSVDYNNODisplayString(string monitorid,SVDYN &dyn,string user="default",string addr="localhost");

SVAPI_API
bool SetDYN(string monitorid,int type,string user="default",string addr="localhost");

/////////////////////////////////DB///////////////////////////

SVAPI_API
RECORDSET QueryRecords(string monitorid,TTimeSpan recenttimespan,string user="default",string addr="localhost");

SVAPI_API
RECORDSET QueryRecords(string monitorid,TTime begin,TTime end,string user="default",string addr="localhost");

SVAPI_API
RECORDSET QueryRecords(string monitorid,int count,string user="default",string addr="localhost");


SVAPI_API
bool  GetRecordCount(RECORDSET rset,size_t &count);

SVAPI_API
bool GetReocrdSetField(RECORDSET rset,std::list<string> &fieldlist);

SVAPI_API
bool GetReocrdSetMonitorType(RECORDSET rset,int &type);

SVAPI_API
bool GetRecordSetMonitorID(RECORDSET rset,string &monitorid);

SVAPI_API 
bool FindRecordFirst(RECORDSET rset,LISTITEM &item);

SVAPI_API
RECORD FindNextRecord(LISTITEM &item);

SVAPI_API
void ReleaseRecordList(LISTITEM &item);

SVAPI_API
bool ResetRecordList(LISTITEM &item);

SVAPI_API
RECORD FindPreReocrd(LISTITEM &item);

SVAPI_API
bool GetRecordState(RECORD rd,int &state);

SVAPI_API
bool GetRecordCreateTime(RECORD rd,TTime &tm);

SVAPI_API
bool GetRecordValueByField(RECORD rd,string Field,int &type,int &state,int &iv,float &fv,string &sv);

SVAPI_API
bool GetRecordDisplayString(RECORD rd,int &state,string &dstr);

SVAPI_API
void CloseRecordSet(RECORDSET &rset);

SVAPI_API
bool DeleteRecords(string monitorid,chen::TTime before,string user="default",string addr="localhost");

///////////////////////////

SVAPI_API
bool GetAllTableNames(std::list<string> &namelist,string user="default",string addr="localhost");

SVAPI_API
bool InsertTable(string monitorid,int monitortype,string user="default",string addr="localhost");

SVAPI_API
bool DeleteTable(string monitorid,string user="default",string addr="localhost");

////////////////////////////

SVAPI_API
bool AppendRecord(string monitorid,const char *data,unsigned int len,string user="default",string addr="localhost");

////////////////////////////

SVAPI_API
bool SetLogKeepDays(int days,string user="default",string addr="localhost");

SVAPI_API
bool GetLogKeepDays(int &days,string user="default",string addr="localhost");


///////////////////////////////////////////////Message queue////////////////////////////////////////////////////////////////

SVAPI_API
bool CreateQueue(string queuename,int type=1,string user="default",string addr="localhost");

SVAPI_API
bool PushMessage(string queuename,string label,const char *data,unsigned int datalen,string user="default",string addr="localhost");

SVAPI_API
bool PopMessage(string queuename,string &label,chen::TTime &createtime,char *databuf,unsigned int &buflen,unsigned int timer=0,string user="default",string addr="localhost");

SVAPI_API
bool BlockPopMessage(string queuename,string &label,chen::TTime &createtime,char *databuf,unsigned int &buflen,string user="default",string addr="localhost");

SVAPI_API
MQRECORD PopMessage(string queuename,unsigned int timer=0,string user="default",string addr="localhost");

SVAPI_API
MQRECORD BlockPopMessage(string queuename,string user="default",string addr="localhost");

SVAPI_API
bool PeekMQMessage(string queuename,string &label,chen::TTime &createtime,char *databuf,unsigned int &buflen,unsigned int timer=0,string user="default",string addr="localhost");

SVAPI_API
bool BlockPeekMQMessage(string queuename,string &label,chen::TTime &createtime,char *databuf,unsigned int &buflen,string user="default",string addr="localhost");

SVAPI_API
MQRECORD PeekMQMessage(string queuename,unsigned int timer=0,string user="default",string addr="localhost");

SVAPI_API
MQRECORD BlockPeekMQMessage(string queuename,string user="default",string addr="localhost");

SVAPI_API
bool GetMessageData(MQRECORD record,string &label,chen::TTime &createtime,char *databuf,unsigned int &buflen);

SVAPI_API
bool CloseMQRecord(MQRECORD &record);

SVAPI_API
bool GetMQRecordCount(string queuename,unsigned int &count,string user="default",string addr="localhost");

SVAPI_API
bool GetAllQueueNames(std::list<string> &namelist,string user="default",string addr="localhost");

SVAPI_API
bool DeleteQueue(string queuename,string user="default",string addr="localhost");

SVAPI_API
bool ClearQueueMessage(string queuename,string user="default",string addr="localhost");


#endif