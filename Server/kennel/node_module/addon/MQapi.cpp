#include "svdbapi.h"
#include "QueryData.h"
#include <StringMap.h>
#include <QueueRecord.h>
#include <util.h>

SVAPI_API
bool CreateQueue(string queuename,int type,string user,string addr)
{
	if(queuename.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_QUEUE;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_CREATEQUEUE;
	strcpy(query.qstr,queuename.c_str());
	query.datalen=type;

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;

	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		if(pdata!=NULL)
		{
			if(len>0)
			{
				LPSVDBRESULT pret=(LPSVDBRESULT)pdata;
				if(pret->state==SVDB_OK)
				{
					delete [] pdata;
					return true;
				}
			}
			delete [] pdata;
			return false;
		}
	}

	return false;

}

SVAPI_API
bool PushMessage(string queuename,string label,const char *data,unsigned int datalen,string user,string addr)
{
	if(queuename.empty()||(data==NULL)||(datalen<=0)||user.empty()||addr.empty())
		return false;

	if(label.empty())
		label="defaultlabel";
	svutil::TTime ct=svutil::TTime::GetCurrentTimeEx();
	QueueRecord qr(label,ct,data,datalen);
	S_UINT dlen=qr.GetRawDataSize();

	svutil::buffer buf;
	if(!buf.checksize(dlen))
		return false;

	if(!qr.GetRawData(buf,dlen))
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_QUEUE;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_SENDMESSAGE;
	strcpy(query.qstr,queuename.c_str());
	query.datalen=dlen;

	QueryData qd;
	char *pdata=NULL;
	SVDBRESULT ret={0};

	if(!qd.Update(&query,buf,dlen,&ret,addr))
		return false;
	return (ret.state==SVDB_OK);
}


SVAPI_API
bool PushStringMessage(string queuename,string label,string content,string user,string addr)
{
	S_UINT csize= content.size()+ 1;
	svutil::buffer buf;
	if(!buf.checksize(csize))
		return false;
	buf.zerobuf();
	char * tempchar= buf;
	memmove(tempchar,content.c_str(),content.size());
	return PushMessage(queuename, label, tempchar, csize, user, addr);
}


SVAPI_API
bool PopMessage(string queuename,string &label,svutil::TTime &createtime,char *databuf,unsigned int &buflen,unsigned int timer,string user,string addr)
{
	if(queuename.empty()||(buflen<=0)||user.empty()||addr.empty())
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_QUEUE;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_POPMESSAGE;
	strcpy(query.qstr,queuename.c_str());
	query.datalen=(int)timer;

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;



	if(qd.Query(&query,(void **)&pdata,len,addr))
	{

		if(pdata!=NULL)
		{
			if(len<sizeof(QUEUEHEAD))
			{
				delete [] pdata;
				return false;
			}
			QueueRecord qrd;
			const char *pt=pdata+sizeof(QUEUEHEAD);
			if(!qrd.CreateObjectByRawData(pt,len-sizeof(QUEUEHEAD)))
			{
				delete [] pdata;
				return false;
			}

			if(buflen<qrd.GetDataLen())
			{
				delete [] pdata;
				return false;
			}
			buflen=qrd.GetDataLen();

			memmove(databuf,qrd.GetData(),qrd.GetDataLen());        
			label=qrd.GetLabel();
			createtime=qrd.GetCreateTime();
			delete [] pdata;
			return true;

		}

	}

	return false;
}

SVAPI_API
bool BlockPopMessage(string queuename,string &label,svutil::TTime &createtime,char *databuf,unsigned int &buflen,string user,string addr)
{
	if(queuename.empty()||(buflen<=0)||user.empty()||addr.empty())
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_QUEUE;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_BLOCKPOP;
	strcpy(query.qstr,queuename.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;


	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		if(pdata!=NULL)
		{
			if(len<sizeof(QUEUEHEAD))
			{
				delete [] pdata;
				return false;
			}
			QueueRecord qrd;
			const char *pt=pdata+sizeof(QUEUEHEAD);
			if(!qrd.CreateObjectByRawData(pt,len-sizeof(QUEUEHEAD)))
			{
				delete [] pdata;
				return false;
			}

			if(buflen<qrd.GetDataLen())
			{
				delete [] pdata;
				return false;
			}
			buflen=qrd.GetDataLen();

			memmove(databuf,qrd.GetData(),qrd.GetDataLen());        
			label=qrd.GetLabel();
			createtime=qrd.GetCreateTime();
			delete [] pdata;
			return true;

		}

	}

	return false;
}

SVAPI_API
MQRECORD PopMessage(string queuename,unsigned int timer,string user,string addr)
{
	if(queuename.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_QUEUE;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_POPMESSAGE;
	strcpy(query.qstr,queuename.c_str());
	query.datalen=(int)timer;

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;


	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		if(pdata!=NULL)
		{
			if(len<sizeof(QUEUEHEAD))
			{
				delete [] pdata;
				return false;
			}

			QueueRecord *pqrd=new QueueRecord();
			if(pqrd)
			{
				MQRECORD ret=INVALID_VALUE;

				const char *pt=pdata+sizeof(QUEUEHEAD);
				try{
					if(pqrd->CreateObjectByRawData(pt,len-sizeof(QUEUEHEAD)))
						ret=reinterpret_cast<MQRECORD>(pqrd);
					delete [] pdata;
				}catch(...)
				{
					return INVALID_VALUE;
				}

				return ret;
			}

			delete [] pdata;
		}

	}


	return INVALID_VALUE;
}

SVAPI_API
MQRECORD BlockPopMessage(string queuename,string user,string addr)
{
	if(queuename.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_QUEUE;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_BLOCKPOP;
	strcpy(query.qstr,queuename.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;


	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		if(pdata!=NULL)
		{
			if(len<sizeof(QUEUEHEAD))
			{
				delete [] pdata;
				return false;
			}

			QueueRecord *pqrd=new QueueRecord();
			if(pqrd)
			{
				MQRECORD ret=INVALID_VALUE;
				const char *pt=pdata+sizeof(QUEUEHEAD);
				try{
					if(pqrd->CreateObjectByRawData(pt,len-sizeof(QUEUEHEAD)))
						ret=reinterpret_cast<MQRECORD>(pqrd);
					delete [] pdata;
				}catch(...)
				{
					return INVALID_VALUE;
				}

				return ret;
			}

			delete [] pdata;
		}

	}


	return INVALID_VALUE;
}

SVAPI_API
bool PeekMQMessage(string queuename,string &label,svutil::TTime &createtime,char *databuf,unsigned int &buflen,unsigned int timer,string user,string addr)
{
	if(queuename.empty()||(buflen<=0)||user.empty()||addr.empty())
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_QUEUE;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_PEEKMESSAGE;
	strcpy(query.qstr,queuename.c_str());
	query.datalen=(int)timer;

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;



	if(qd.Query(&query,(void **)&pdata,len,addr))
	{

		if(pdata!=NULL)
		{
			if(len<sizeof(QUEUEHEAD))
			{
				delete [] pdata;
				return false;
			}
			QueueRecord qrd;
			const char *pt=pdata+sizeof(QUEUEHEAD);
			if(!qrd.CreateObjectByRawData(pt,len-sizeof(QUEUEHEAD)))
			{
				delete [] pdata;
				return false;
			}

			if(buflen<qrd.GetDataLen())
			{
				delete [] pdata;
				return false;
			}
			buflen=qrd.GetDataLen();

			memmove(databuf,qrd.GetData(),qrd.GetDataLen());        
			label=qrd.GetLabel();
			createtime=qrd.GetCreateTime();
			delete [] pdata;
			return true;

		}

	}

	return false;
}

SVAPI_API
bool BlockPeekMQMessage(string queuename,string &label,svutil::TTime &createtime,char *databuf,unsigned int &buflen,string user,string addr)
{
	if(queuename.empty()||(buflen<=0)||user.empty()||addr.empty())
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_QUEUE;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_BLOCKPEEK;
	strcpy(query.qstr,queuename.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;


	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		if(pdata!=NULL)
		{
			if(len<sizeof(QUEUEHEAD))
			{
				delete [] pdata;
				return false;
			}
			QueueRecord qrd;
			const char *pt=pdata+sizeof(QUEUEHEAD);
			if(!qrd.CreateObjectByRawData(pt,len-sizeof(QUEUEHEAD)))
			{
				delete [] pdata;
				return false;
			}

			if(buflen<qrd.GetDataLen())
			{
				delete [] pdata;
				return false;
			}
			buflen=qrd.GetDataLen();

			memmove(databuf,qrd.GetData(),qrd.GetDataLen());        
			label=qrd.GetLabel();
			createtime=qrd.GetCreateTime();
			delete [] pdata;
			return true;

		}

	}

	return false;

}

SVAPI_API
MQRECORD PeekMQMessage(string queuename,unsigned int timer,string user,string addr)
{
	if(queuename.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_QUEUE;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_PEEKMESSAGE;
	strcpy(query.qstr,queuename.c_str());
	query.datalen=(int)timer;

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;


	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		if(pdata!=NULL)
		{
			if(len<sizeof(QUEUEHEAD))
			{
				delete [] pdata;
				return false;
			}

			QueueRecord *pqrd=new QueueRecord();
			if(pqrd)
			{
				MQRECORD ret=INVALID_VALUE;

				const char *pt=pdata+sizeof(QUEUEHEAD);
				try{
					if(pqrd->CreateObjectByRawData(pt,len-sizeof(QUEUEHEAD)))
						ret=reinterpret_cast<MQRECORD>(pqrd);
					delete [] pdata;
				}catch(...)
				{
					return INVALID_VALUE;
				}

				return ret;
			}

			delete [] pdata;
		}

	}


	return INVALID_VALUE;
}

SVAPI_API
MQRECORD BlockPeekMQMessage(string queuename,string user,string addr)
{
	if(queuename.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_QUEUE;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_BLOCKPEEK;
	strcpy(query.qstr,queuename.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;


	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		if(pdata!=NULL)
		{
			if(len<sizeof(QUEUEHEAD))
			{
				delete [] pdata;
				return false;
			}

			QueueRecord *pqrd=new QueueRecord();
			if(pqrd)
			{
				MQRECORD ret=INVALID_VALUE;
				const char *pt=pdata+sizeof(QUEUEHEAD);
				try{
					if(pqrd->CreateObjectByRawData(pt,len-sizeof(QUEUEHEAD)))
						ret=reinterpret_cast<MQRECORD>(pqrd);
					delete [] pdata;
				}catch(...)
				{
					return INVALID_VALUE;
				}

				return ret;
			}

			delete [] pdata;
		}

	}


	return INVALID_VALUE;

}


SVAPI_API
bool GetMessageData(MQRECORD record,string &label,svutil::TTime &createtime,char *databuf,unsigned int &buflen)
{
	if(record==INVALID_VALUE)
		return false;

	try{
	     QueueRecord *pqr=reinterpret_cast<QueueRecord *>(record);
		 label=pqr->GetLabel();
		 createtime=pqr->GetCreateTime();
		 if(databuf==NULL)
		 {
	    	 buflen=pqr->GetDataLen();
			 return true;
		 }
		 if(buflen<pqr->GetDataLen())
			 return false;
		 memmove(databuf,pqr->GetData(),pqr->GetDataLen());

	}catch(...)
	{
		return false;
	}
	return true;
}

SVAPI_API
bool CloseMQRecord(MQRECORD &record)
{
	if(record==INVALID_VALUE)
		return true;
	try{
	     QueueRecord *pqr=reinterpret_cast<QueueRecord *>(record);
		 if(pqr!=NULL)
			 delete pqr;
		 record=INVALID_VALUE;
	}catch(...)
	{
		return false;
	}

	return true;
}

SVAPI_API
bool GetMQRecordCount(string queuename,unsigned int &count,string user,string addr)
{
	if(queuename.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_QUEUE;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_GETRECCOUNT;
	strcpy(query.qstr,queuename.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;


	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		if(pdata!=NULL)
		{
			if(len!=(sizeof(int)+sizeof(S_UINT)))
			{
				delete [] pdata;
				return false;
			}
			int nret=0;
			S_UINT rcount=0;
			const char *pt=(const char *)pdata;
			memmove(&nret,pt,sizeof(int));
			if(nret!=QUEUE_OK)
			{
				delete [] pdata;
				return false;
			}
			pt+=sizeof(int);
			memmove(&rcount,pt,sizeof(S_UINT));
			count=rcount;
			delete [] pdata;
			return true;
		}

	}

	return false;
}

SVAPI_API
bool GetAllQueueNames(std::list<string> &namelist,string user,string addr)
{
	if(user.empty()||addr.empty())
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_QUEUE;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_GETQUEUENAMES;

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;


	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		try{
			if(pdata)
			{
				namelist.clear();
				if(CreateStrListByRawData(namelist,pdata,len))
				{
					delete [] pdata;
					return true;
				}
				delete [] pdata;
			}

		}catch(...)
		{
			return false;
		}

	}

	return false;
}

SVAPI_API
bool DeleteQueue(string queuename,string user,string addr)
{
	if(queuename.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_QUEUE;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_DELETEQUEUE;
	strcpy(query.qstr,queuename.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;

	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		if(len>0)
		{
			LPSVDBRESULT pret=(LPSVDBRESULT)pdata;
			if(pret->state==SVDB_OK)
			{
				delete [] pdata;
				return true;
			}
		}
		delete [] pdata;
	}

	return false;
}

SVAPI_API
bool ClearQueueMessage(string queuename,string user,string addr)
{
	if(queuename.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_QUEUE;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_CLEARQUEUE;
	strcpy(query.qstr,queuename.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;

	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		if(len>0)
		{
			LPSVDBRESULT pret=(LPSVDBRESULT)pdata;
			if(pret->state==SVDB_OK)
			{
				delete [] pdata;
				return true;
			}
		}
		delete [] pdata;
	}

	return false;

}

SVAPI_API
bool GetAllMessageLabels(string queuename,std::list<string> &retlist,string user,string addr)
{
	if(queuename.empty()||user.empty()||addr.empty())
		return false;

	if(queuename.size()>MAXQUEREYSTRINGLEN)
		return false;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_GET_LABELS;
	querybuf.datatype=S_QUEUE;
	strcpy(querybuf.qstr,queuename.c_str());
	strcpy(querybuf.idcuser,user.c_str());

    QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;
	if(qd.Query(&querybuf,(void **)&pdata,len,addr))
	{
		if(pdata!=NULL)
		{
			try{

				retlist.clear();
				if(::CreateStrListByRawData(retlist,pdata,len))
				{
					delete [] pdata;
					return true;
				}
			}catch(...)
			{
				delete [] pdata;
				return false;
			}
			delete [] pdata;
		}
	}
	
	return false;
}
