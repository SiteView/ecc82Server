//handling time based and collected log data
// the size of the data can be very big

#include "DB.h"
#include "QueueRecord.h"
#include "RecordSet.h"

DB::DB(void)
{
	m_tables.resetsize(inittablemapsize);
	m_queues.resetsize(initqueuemapsize);
	m_dbfilename="";
	m_dbheadfile="";
	m_pagepool=NULL;
	m_headpool=NULL;
	m_dbh=NULL;
	m_pMain=NULL;
}

DB::DB(string dbfilename,string dbheadfile)
{
	m_dbfilename=dbfilename;
	m_dbheadfile=dbheadfile;
	m_pagepool=NULL;
	m_headpool=NULL;
	m_dbh=NULL;
	m_pMain=NULL;

	m_tables.resetsize(inittablemapsize);
	m_queues.resetsize(initqueuemapsize);

}


DB::~DB(void)
{
	FlushData();
	if(m_pagepool)
		delete m_pagepool;
	if(m_headpool)
		delete m_headpool;

}

bool DB::Load()
{
	if(m_headpool!=NULL)
		return false;
	if(m_pagepool!=NULL)
		return false;
	m_headpool = new PagePool();
	if(m_headpool==NULL)
		return false;

	if(!m_headpool->LoadPage(m_dbheadfile.c_str()))
		return false;

	Page *phead=m_headpool->Get(0);
	if(phead==NULL)
		return false;
	if(phead->m_data==NULL)
		return false;

	m_dbh=(DBhead*)phead->m_data;

	m_pagepool=new PagePool();
	if(m_pagepool==NULL)
		return false;

	//Ϊ��ȥ�� .db ��Ŀ¼·���Ĺ����� ȥ��������3��
	//if(!m_pagepool->LoadPage(m_dbh->m_dbfilename))
	//	return false;
	//this->m_dbfilename=m_dbh->m_dbfilename;
	if(!m_pagepool->LoadPage(this->m_dbfilename.c_str()))
		return false;

	m_headpool->Put(phead,false,false);

	for(int i=1;i<m_headpool->GetPagePoolHead().m_PageCount;i++)
	{
		Page *pt=m_headpool->Get(i,false);
		if(pt==NULL)
			return false;
		PageHead *ppagehead=pt->GetPageHead();
		if(ppagehead==NULL)
			return false;
		if((ppagehead->m_unused==1)||(ppagehead->m_badpage==1))
		{
//			m_headpool->Put(pt,false,true);
			m_headpool->PutFree(pt);
			continue;
		}

		Table *ptable=new Table();
		if(!ptable)
			return false;

		if(!ptable->CreateObjByPage(pt))
			return false;
		if(!m_headpool->Put(pt,false,true))
			return false;

		if(ptable->m_type.m_monitorid.empty())
			return false;

		ptable->m_pagepool=m_pagepool;

		if(!m_tables.insert(ptable->m_type.m_monitorid,ptable))
			return false;

	}


	

	return true;
}
int DB::LoadEx()
{
	if(m_headpool!=NULL)
		return false;
	if(m_pagepool!=NULL)
		return false;
	m_headpool = new PagePool();
	if(m_headpool==NULL)
		return false;

	int lint= m_headpool->LoadPageEx(m_dbheadfile.c_str());
	if( lint!=1 )
	{
		delete m_headpool;
		m_headpool=NULL;
		return lint;
	}


	Page *phead=m_headpool->Get(0);
	if(phead==NULL)
		return false;
	if(phead->m_data==NULL)
		return false;

	m_dbh=(DBhead*)phead->m_data;


	m_pagepool=new PagePool();
	if(m_pagepool==NULL)
		return false;

	//Ϊ��ȥ�� .db ��Ŀ¼·���Ĺ����� ȥ��������3��
	//if(!m_pagepool->LoadPageEx(m_dbh->m_dbfilename))
	//	return false;
	//this->m_dbfilename=m_dbh->m_dbfilename;
	lint= m_pagepool->LoadPageEx(this->m_dbfilename.c_str());
	if(lint!=1)
		return lint;

	m_headpool->Put(phead,false,false);

	int totalcount= m_headpool->GetPagePoolHead().m_TotalCount;
	cout<<" Total tab count: "<<totalcount<<endl;
	for(int i=1;i<totalcount;i++)
	{
		Page *pt=m_headpool->Get(i,false);
		if(pt==NULL)
			return false;
		PageHead *ppagehead=pt->GetPageHead();
		if(ppagehead==NULL)
			return false;
		if((ppagehead->m_unused==1)||(ppagehead->m_badpage==1))
		{
//			m_headpool->Put(pt,false,true);
			m_headpool->PutFree(pt);
			continue;
		}

		if(ppagehead->m_pagetype==PageHead::tableheadpagetype)
		{

			Table *ptable=new Table();
			if(!ptable)
			{
				m_headpool->Put(pt,false,true);
				return false;
			}

			if(!ptable->CreateObjByPage(pt))
			{
				delete ptable;
				return false;
			}
			if(!m_headpool->Put(pt,false,true))
			{
				delete ptable;
				return false;
			}

			if(ptable->m_type.m_monitorid.empty())
			{
				delete ptable;
				return false;
			}

			ptable->m_pagepool=m_pagepool;

			if(!m_tables.insert(ptable->m_type.m_monitorid,ptable))
			{
				delete ptable;
				return false;
			}
		}else if(ppagehead->m_pagetype==PageHead::queueheadpagetype)
		{
			Queue *pqueue=new Queue();

			if(pqueue==NULL)
			{
				m_headpool->Put(pt,false,true);
				return false;

			}

			if(!pqueue->CreateObjByPage(pt))
			{
				delete pqueue;
				return false;
			}
			if(!m_headpool->Put(pt,false,true))
			{
				delete pqueue;
				return false;
			}

			if(pqueue->m_queuename.empty())
			{
				delete pqueue;
				return false;

			}
			pqueue->m_pagepool=m_pagepool;

			if(!m_queues.insert(pqueue->m_queuename,pqueue))
			{
				delete pqueue;
				return false;
			}

		}else{

			m_headpool->Put(pt,false,true);

		}

	}

	return true;

}
bool DB::FlushData()
{
	if(m_headpool==NULL)
		return false;

	TABLEMAP::iterator it;
	while(m_tables.findnext(it))
	{
		(*it).getvalue()->FlushData();
	}

	return true;
}
bool DB::CreateNew(string dbheadfile,string dbfilename,int pagesize,int pagecount)
{
	if(m_headpool!=NULL)
		return false;
	m_headpool=new PagePool(1024*2,1000);
	if(!m_headpool)
		return false;
	this->m_dbfilename=dbfilename;
	this->m_dbheadfile=dbheadfile;
	if(!m_headpool->CreatePage(dbheadfile.c_str(),true))
		return false;

	Page *ppt=m_headpool->Get(0);
	if(ppt==NULL)
		return false;

	DBhead *pdh=(DBhead *)ppt->m_data;
	pdh->m_currentid=0;
	pdh->m_monitorcount=0;
	pdh->m_version=1;
	strcpy(pdh->m_dbfilename,dbfilename.c_str());

	m_headpool->Put(ppt,true,true);

	m_pagepool =new PagePool(pagesize,pagecount);
	if(!m_pagepool)
		return false;
	if(!m_pagepool->CreatePage(dbfilename.c_str(),true))
		return false;

	return true;

}
bool DB::CreateNewEx(string dbheadfiletitle,string dbfiletitle,int pagesize,int pagecount,int perfilesize)
{
	if(m_headpool!=NULL)
		return false;
	m_headpool=new PagePool();   //���������ȥ������������Ч
	if(!m_headpool)
		return false;
	m_dbfilename=dbheadfiletitle;
	m_dbheadfile=dbfiletitle;
	if(!m_headpool->CreatePagePool(1000,dbheadfiletitle,perfilesize,pagesize))
	{
		puts("Create head pool failed");
		return false;
	}

	Page *ppt=m_headpool->Get(0);
	if(ppt==NULL)
		return false;

	DBhead *pdh=(DBhead *)ppt->m_data;
	pdh->m_currentid=0;
	pdh->m_monitorcount=0;
	pdh->m_version=1;
	strcpy(pdh->m_dbfilename,dbfiletitle.c_str());

	m_headpool->Put(ppt,true,true);

	m_pagepool =new PagePool();
	if(!m_pagepool)
		return false;
	if(!m_pagepool->CreatePagePool(pagecount,dbfiletitle,perfilesize,pagesize))
	{
		puts("Create page pool failed");
		return false;
	}

	return true;

}

bool DB::InsertTable(string monitorid,int monitortype)
{
	if(monitorid.empty()||monitortype<1)
		return false;

	if(m_tables.find(monitorid)!=NULL)
	{
		printf("Table has existed: %s\n",monitorid.c_str());
		return false;
	}

	if((m_pagepool==NULL)||(m_headpool==NULL))
		return false;



	Table *ptable=new Table();
	if(!ptable)
		return false;

	ptable->m_hasindex=0;

	Page *firstpage=m_pagepool->GetFree();
	if(firstpage==NULL)
	{
		if(!m_pagepool->AddNewPagesEx(1000))
		{
			puts("Add new page failed");
			return false;
		}
		firstpage=m_pagepool->GetFree();
		if(firstpage==NULL)
		{
			puts("Get data free page failed");
			return false;
		}
	}
	ptable->m_firstpage=firstpage->m_pos;
	ptable->m_currentpage=ptable->m_firstpage;
	m_pagepool->Put(firstpage,true,false);

	ptable->m_type.m_monitorid=monitorid;
	ptable->m_type.m_monitortype=monitortype;

	/***************************************************
	��Ҫ��monitortemplet������������Ĵ���ֻ��ģ��
	****************************************************/
	DataType *pdtt = new DataType();
	pdtt->type=DataType::inttype;
	strcpy(pdtt->label,"CPU������");
	strcpy(pdtt->unit,"%%");
	strcpy(pdtt->name,"utilize");
	ptable->m_type.m_data.push_back(pdtt);

	pdtt = new DataType();

	pdtt->type=DataType::stringtype;
	strcpy(pdtt->label,"����");
//	strcpy(pdtt->unit,"%%");
	strcpy(pdtt->name,"description");
	ptable->m_type.m_data.push_back(pdtt);

	pdtt = new DataType();

	pdtt->type=DataType::floattype;
	strcpy(pdtt->label,"����һ��floatֵ");
	strcpy(pdtt->unit,"Ԫ");
	strcpy(pdtt->name,"floatvalue");
	ptable->m_type.m_data.push_back(pdtt);

	/**********************************************************/



	ptable->m_createtime=svutil::TTime::GetCurrentTimeEx();
	ptable->m_page=m_headpool->GetFree();
	if(ptable->m_page==NULL)
	{
		if(m_headpool->GetPagePoolHead().m_TotalCount<MAXTABLE)
		{
			int flen=MAXTABLE-m_headpool->GetPagePoolHead().m_TotalCount;
			int count=flen > 10 ? 10 : flen;
			if(!m_headpool->AddNewPagesEx(count))
			{
				printf("Add new page failed");
				delete ptable;
				return false;
			}
			ptable->m_page=m_headpool->GetFree();
			if(ptable->m_page==NULL)
			{
				puts("Get page failed");
				return false;
			}
		}
		else
		{
			printf("Too many table, %d >= %d \n",m_headpool->GetPagePoolHead().m_TotalCount, MAXTABLE);
			return false;
		}
	}
	ptable->m_page->GetPageHead()->m_pagetype=PageHead::tableheadpagetype;
	m_headpool->Put(ptable->m_page,false,false);
	ptable->m_pagepool=m_pagepool;

	if(!ptable->FlushData())
	{
		puts("flash data failed");
	}

	m_tables[monitorid]=ptable;

	return true;

}
bool  DB::CreateQueue(string queuename,int queuetype)
{
	if(queuename.empty()||queuetype<1)
		return false;

	
	ost::MutexLock queuelock(m_queuemutex);
	if(m_queues.find(queuename)!=NULL)
	{
		printf("Queue: %s has existed.\n",queuename.c_str());
		return false;
	}
	

	if((m_pagepool==NULL)||(m_headpool==NULL))
		return false;

	Queue *pqueue = new Queue();
	if(pqueue==NULL)
		return false;

	pqueue->m_hasindex=0;

	Page *firstpage=m_pagepool->GetFreeEx();
	if(firstpage==NULL)
	{
		puts("Get data free page failed");
		delete pqueue;
		return false;
	}

	pqueue->m_firstpage=firstpage->m_pos;
	pqueue->m_currentpage=pqueue->m_firstpage;
	firstpage->GetPageHead()->m_pagetype=PageHead::queuerecordpagetype;

	m_pagepool->Put(firstpage,true,false);
	pqueue->m_queuename=queuename;

	pqueue->m_createtime=svutil::TTime::GetCurrentTimeEx();
	pqueue->m_page=m_headpool->GetFreeEx();
	if(pqueue->m_page==NULL)
	{
		puts("Create queue -- Get free page failed");
		delete pqueue;
		return false;
	}

	pqueue->m_page->GetPageHead()->m_pagetype=PageHead::queueheadpagetype;

	m_headpool->Put(pqueue->m_page,true,false);
	pqueue->m_pagepool=m_pagepool;

	if(!pqueue->FlushData())
	{
		puts("flash data failed");
	}

	{
//		ost::MutexLock queuelock(m_queuemutex);
		m_queues[queuename]=pqueue;
	}

	return true;



}
bool DB::InsertTableEx(string monitorid,int monitortype)
{
	if(monitorid.empty()||monitortype<1)
		return false;

	{
		ost::MutexLock tablelock(m_tablemutex);
		if(m_tables.find(monitorid)!=NULL)
		{
			printf("Table has existed: %s\n",monitorid.c_str());
			return false;
		}
	}

	if((m_pagepool==NULL)||(m_headpool==NULL))
		return false;

	if(m_pMain==NULL)
		return false;
	if(m_pMain->m_pMT==NULL)
		return false;

	if(!m_pMain->m_pMT->Find(monitortype))
		return false;

	MonitorTemplet *pmt=m_pMain->m_pMT->GetMonitorTemplet(monitortype,true);
	if(pmt==NULL)
		return false;



	Table *ptable=new Table();
	if(!ptable)
		return false;

	ptable->m_hasindex=0;

	Page *firstpage=m_pagepool->GetFree();
	if(firstpage==NULL)
	{
		if(!m_pagepool->AddNewPagesEx(1000))
		{
			puts("Add new page failed");
			return false;
		}
		firstpage=m_pagepool->GetFree();
		if(firstpage==NULL)
		{
			puts("Get data free page failed");
			return false;
		}
	}
	ptable->m_firstpage=firstpage->m_pos;
	ptable->m_currentpage=ptable->m_firstpage;
	firstpage->GetPageHead()->m_pagetype=PageHead::recordpagetype;
	m_pagepool->Put(firstpage,true,false);

	ptable->m_type.m_monitorid=monitorid;
	ptable->m_type.m_monitortype=monitortype;


	STRMAPLIST maplist=pmt->GetReturn();
	STRMAPLIST::iterator it;
    
	for(it=maplist.begin();it!=maplist.end();it++)
	{
		DataType *pdtt = new DataType();
		word *pv=NULL;
		if((pv=(*it)->find("sv_label"))!=NULL)
			sprintf(pdtt->label,"%s",pv->getword());
        pv=NULL;
		if((pv=(*it)->find("sv_name"))!=NULL)
			sprintf(pdtt->name,"%s",pv->getword());

		if((pv=(*it)->find("sv_unit"))!=NULL)
			sprintf(pdtt->unit,"%s",pv->getword());

		if((pv=(*it)->find("sv_type"))!=NULL)
			pdtt->type=(DataType::valuetype)ConvertType(pv->getword());

		ptable->m_type.m_data.push_back(pdtt);

	}

	ptable->m_createtime=svutil::TTime::GetCurrentTimeEx();
	ptable->m_page=m_headpool->GetFree();
	if(ptable->m_page==NULL)
	{
		if(m_headpool->GetPagePoolHead().m_TotalCount<MAXTABLE)
		{
			int flen=MAXTABLE-m_headpool->GetPagePoolHead().m_TotalCount;
			int count=flen > 10 ? 10 : flen;
			if(!m_headpool->AddNewPagesEx(count))
			{
				printf("Add new page failed");
				delete ptable;
				return false;
			}
			ptable->m_page=m_headpool->GetFree();
			if(ptable->m_page==NULL)
			{
				printf("InsertTableEx Get page failed page id:get free page\n");
				puts("Get page failed");
				return false;
			}
		}
		else
		{
			printf("Too many table, %d >= %d \n",m_headpool->GetPagePoolHead().m_TotalCount, MAXTABLE);
			return false;
		}
	}
	ptable->m_page->GetPageHead()->m_pagetype=PageHead::tableheadpagetype;
	m_headpool->Put(ptable->m_page,true,false);
	ptable->m_pagepool=m_pagepool;

	if(!ptable->FlushData())
	{
		puts("flash data failed");
	}

	{
		ost::MutexLock tablelock(m_tablemutex);
		m_tables[monitorid]=ptable;
	}


	return true;
}
bool DB::InsertRecord(string monitorid,int state,char *data,int datalen)
{
	Table **ptable=NULL;
	{
		ost::MutexLock tablelock(m_tablemutex);
		ptable=m_tables.find(monitorid);
	}

	if(ptable==NULL)
	{
		printf("Table no exist: %s\n",monitorid.c_str());
		return false;
	}

	bool nret=false;
	{
		ost::MutexLock((*ptable)->m_mutex);
		nret=(*ptable)->InsertRecord(state,data,datalen);
	}

	return nret;

}
int DB::QueryRecordByTime(string monitorid,svutil::TTime begin,svutil::TTime end,char *buf,int &buflen)
{
	Table **ptable=NULL;
	{
		ost::MutexLock tablelock(m_tablemutex);
		ptable=m_tables.find(monitorid);
	}

	if(ptable==NULL)
	{
		printf("Table no exist: %s\n",monitorid.c_str());
		return TABLENOEXIST;
	}

	S_UINT len=buflen;
	if(!(*ptable)->BuildRecordType(buf,len))
		return -3;

	char *pt=buf+len;

	char *pst=pt+sizeof(S_UINT);

	int blen=buflen-len-sizeof(S_UINT);
	int ret=0;
	{
		ost::MutexLock((*ptable)->m_mutex);
		ret=(*ptable)->QueryRecordByTime(begin,end,pst,blen);
	}
	if(ret>=0)
		memmove(pt,&ret,sizeof(S_UINT));
	buflen=blen+len+sizeof(S_UINT);
	return ret;

//	return (*ptable)->QueryRecordByTime(begin,end,buf,buflen);

}
int DB::QueryRecordByCount(string monitorid,S_UINT count,svutil::buffer &buf,int &datalen)
{
	Table **ptable=NULL;
	{
		ost::MutexLock tablelock(m_tablemutex);
		ptable=m_tables.find(monitorid);
	}

	if(ptable==NULL)
	{
		printf("Table no exist: %s\n",monitorid.c_str());
		return TABLENOEXIST;
	}

	char *pbuf=buf;
	S_UINT len=buf.size();
	if(!(*ptable)->BuildRecordType(pbuf,len))
		return -3;

//	char *pt=pbuf+len;

//	char *pst=pt+sizeof(S_UINT);

	int headlen=len+sizeof(S_UINT);
	int blen=0;
	int ret=0;

	{
		ost::MutexLock((*ptable)->m_mutex);
		ret=(*ptable)->QueryRecordByCount(count,buf,headlen,blen);
	}

	if(ret>=0)
	{
		char *pt=buf.getbuffer();
		pt+=len;
		memmove(pt,&ret,sizeof(S_UINT));
	}
	datalen=blen+len+sizeof(S_UINT);
	return ret;


}



bool DB::QueryRecordCount(string monitorid, int & count)
{
	Table **ptable=NULL;
	{
		ost::MutexLock tablelock(m_tablemutex);
		ptable=m_tables.find(monitorid);
	}

	if(ptable==NULL)
	{
		printf("Table no exist: %s\n",monitorid.c_str());
		return false;
	}

	bool ret=false;
	try{
		{
			ost::MutexLock((*ptable)->m_mutex);
			ret=(*ptable)->QueryRecordCount(monitorid,count);
		}
	}catch(...)
	{
	}

	return ret;
}

int DB::QueryRecordByTimeEx(string monitorid,svutil::TTime begin,svutil::TTime end,svutil::buffer &buf,int &datalen)
{
	Table **ptable=NULL;
	{
		ost::MutexLock tablelock(m_tablemutex);
		ptable=m_tables.find(monitorid);
	}

	if(ptable==NULL)
	{
		printf("Table no exist: %s\n",monitorid.c_str());
		return TABLENOEXIST;
	}

	char *pbuf=buf;
	S_UINT len=buf.size();
	if(!(*ptable)->BuildRecordType(pbuf,len))
		return -3;

//	char *pt=pbuf+len;

//	char *pst=pt+sizeof(S_UINT);

	int headlen=len+sizeof(S_UINT);
	int blen=0;
	int ret=0;

	{
		ost::MutexLock((*ptable)->m_mutex);
		ret=(*ptable)->QueryRecordByTime(begin,end,buf,headlen,blen);
	}

	if(ret>=0)
	{
		char *pt=buf.getbuffer();
		pt+=len;
		memmove(pt,&ret,sizeof(S_UINT));
	}
	datalen=blen+len+sizeof(S_UINT);
	return ret;


}

int DB::QueryDynWithLatestRCD(string monitorid,char *buf,S_UINT & buflen,char * & buf2,S_UINT & buflen2, string & monitorTplId, svutil::TTime intime)
{
	Table **ptable=NULL;
	{
		ost::MutexLock tablelock(m_tablemutex);
		ptable=m_tables.find(monitorid);
	}

	if(ptable==NULL)
	{
		printf("Table no exist: %s\n",monitorid.c_str());
		return TABLENOEXIST;
	}

	int ret=0;
	{
		ost::MutexLock((*ptable)->m_mutex);

		ret=(*ptable)->QueryDyn(buf,buflen);
		if(ret!=0||buflen==0)
			return ret;
		svutil::TTime time2;
		memmove(&time2, buf, sizeof(svutil::TTime));
		if(intime.GetTime()!=0)
		{
			if(time2.GetTime()==0 || intime >= time2 )
				return -7;
		}

		int mid=(*ptable)->m_type.m_monitortype;
		char tplid[128]={0};
		sprintf(tplid,"%d",mid);
		monitorTplId= tplid;

		if( !(*ptable)->GetLatestRCD(buf2,buflen2) )
		{
			svutil::buffer tbuf;
			if(!tbuf.checksize(QBUFFERLEN))
				return -9;

			int ret2(1);
			char * tbuf2(NULL);
			S_UINT tbuflen2(0);
			int headlen(0);
			int blen=0;

			char *pbuf=tbuf;
			S_UINT len=tbuf.size();
			if(!(*ptable)->BuildRecordType(pbuf,len))
				ret2=0;
			else
			{
				headlen=len+sizeof(S_UINT);
				ret2=(*ptable)->QueryRecordByCount(1,tbuf,headlen,blen);
				RecordHead *prht=(RecordHead *)(tbuf+headlen);

				if( ret2<1|| blen<=sizeof(RecordHead)||(prht->state==Record::STATUS_BAD)||(prht->state==Record::STATUS_NULL)||(prht->state==Record::STATUS_DISABLE))
					ret2= 0;
			}

			if( ret2<1)
			{
				tbuf2= NULL;
				tbuflen2= 0;
			}
			else
			{
				tbuf2= tbuf+headlen+sizeof(RecordHead);
				tbuflen2= blen-sizeof(RecordHead);
			}
			(*ptable)->SetLatestRCD(tbuf2, tbuflen2);
			(*ptable)->GetLatestRCD(buf2,buflen2);
		}
	}
	return ret;
}


int DB::QueryDyn(string monitorid,char *buf,S_UINT&buflen)
{
	Table **ptable=NULL;
	{
		ost::MutexLock tablelock(m_tablemutex);
		ptable=m_tables.find(monitorid);
	}

	if(ptable==NULL)
	{
		printf("Table no exist: %s\n",monitorid.c_str());
		return TABLENOEXIST;
	}

	int ret=0;
	{
		ost::MutexLock((*ptable)->m_mutex);
		ret=(*ptable)->QueryDyn(buf,buflen);
	}
	return ret;
}

int DB::QueryMassDyn(string pid, std::map <string, svutil::TTime> & times, std::list<SingelRecord> & listrcd )
{
	std::map <string, svutil::TTime>::iterator mit;
	string tableid;
	pid+=".";

	TABLEMAP::iterator tit;
	while(m_tables.findnext(tit))
	{
		const char * key = (*tit).getkey().getword();
		tableid=key;
		if( tableid.find(pid)!=0 )
			continue;

		svutil::buffer buf;
		S_UINT size(0);

		try{
			if(QueryDyn(tableid,NULL,size)==0)
			{
				if(!buf.checksize(size))
					continue;

				char * buf2(NULL);
				S_UINT size2(0);
				string mtplid;
								
				svutil::TTime intime;
				mit= times.find(tableid);
				if(mit!=times.end())
					intime= mit->second;

				int get=QueryDynWithLatestRCD(tableid, buf,size, buf2,size2,mtplid,intime);
				if(get==0)
				{
					std::list<SingelRecord> templistrcd;
					SingelRecord r1;
					r1.datalen= size;
					r1.monitorid= tableid;
					r1.data= buf;
					templistrcd.push_back(r1);

					SingelRecord r2;
					r2.datalen= size2;
					r2.monitorid= mtplid;
					r2.data= buf2;
					templistrcd.push_back(r2);

					S_UINT templen= GetMassRecordListRawDataSize(templistrcd);
					char * tempchar= new char[templen];
					if(tempchar==NULL)
						continue;
					tempchar= GetMassRecordListRawData(templistrcd,tempchar,templen); 
					if(tempchar==NULL)
						continue;

					SingelRecord rcd;
					rcd.monitorid= tableid;
					rcd.datalen= templen;
					rcd.data= tempchar;

					listrcd.push_back(rcd);
				}
			}
		}
		catch(...)
		{
			printf("Exception in DB::QueryMassDyn \n");
		}
	}

	return listrcd.size();
}

int DB::QueryDynNOStr(string monitorid,char *buf,S_UINT &buflen)
{
	Table **ptable=NULL;
	{
		ost::MutexLock tablelock(m_tablemutex);
		ptable=m_tables.find(monitorid);
	}

	if(ptable==NULL)
	{
		printf("Table no exist: %s\n",monitorid.c_str());
		return TABLENOEXIST;
	}

	int ret=0;
	{
		ost::MutexLock((*ptable)->m_mutex);
		ret=(*ptable)->QueryDynNOStr(buf,buflen);
	}
	return ret;


}

bool DB::DeleteTable(string monitorid)
{

	ost::MutexLock tablelock(m_tablemutex);
	Table **ptable=m_tables.find(monitorid);

	if(ptable==NULL)
	{
		printf("Table no exist: %s\n",monitorid.c_str());
		return false;
	}
	{
		Table *pmytalbe=(*ptable);
		ost::MutexLock(pmytalbe->m_mutex);
		pmytalbe->DeleteData();
		Page *pt=m_headpool->Get(pmytalbe->m_page->m_pos);
		if(pt==NULL)
		{
			puts("pt is NULL");
			return false;
		}
		m_headpool->PutFree(pt);
    	m_tables.erase(monitorid);
	}

//	delete (*ptable);

	return true;


}
bool DB::DeleteRecordsByTime(string monitorid,svutil::TTime before)
{
	Table **ptable=NULL;
	{
		ost::MutexLock tablelock(m_tablemutex);
		ptable=m_tables.find(monitorid);
	}

	if(ptable==NULL)
	{
		printf("Table no exist: %s\n",monitorid.c_str());
		return false;
	}

	bool bret=false;
	{
		ost::MutexLock((*ptable)->m_mutex);
        bret=(*ptable)->DeleteData(before);
	}
	return bret;
}

bool DB::Init(string dbfilename,SvdbMain *pMain,int PerFileSize,int PageSize)
{
	if(pMain==NULL)
		return false;
	m_pMain=pMain;
	if(dbfilename.empty())
		return false;

	char buf[1024]={0};
	if( IdcUser::EnableIdc )
	{   // for IDC
		sprintf(buf,"%s/data/idc_data/%s_head",pMain->m_pOption->m_rootpath.c_str(),dbfilename.c_str());
	}
	else
	{   // for non-IDC
		sprintf(buf,"%s/data/%s_head",pMain->m_pOption->m_rootpath.c_str(),dbfilename.c_str());
	}
//	sprintf(buf,"%s\\%s_head.db","d:\\v70",dbfilename.c_str());

	printf("DB path:%s\n",buf);

	m_dbheadfile=buf;

	if( IdcUser::EnableIdc )
	{   // for IDC
		sprintf(buf,"%s/data/idc_data/%s_data",pMain->m_pOption->m_rootpath.c_str(),dbfilename.c_str());
	}
	else
	{ // for non-IDC
		sprintf(buf,"%s/data/%s_data",pMain->m_pOption->m_rootpath.c_str(),dbfilename.c_str());
	}
//	sprintf(buf,"%s\\%s_data","d:\\v70",dbfilename.c_str());
	m_dbfilename=buf;

	printf("DB name:%s\n",buf);

	int lint= LoadEx();
	if(lint!=1)
	{
		if(lint==-1)
			return CreateNewEx(m_dbheadfile,m_dbfilename,PageSize,1000);
		else
			return false;
	}

	return true;
}


bool DB::Init(string idcuser,string dbfilename,SvdbMain *pMain,int PerFileSize,int PageSize)
{
	if(pMain==NULL)
		return false;
	m_pMain=pMain;
	if(dbfilename.empty())
		return false;

	char buf[1024]={0};
	sprintf(buf,"%s/data/idc_data/%s/%s_head",pMain->m_pOption->m_rootpath.c_str(),idcuser.c_str(),dbfilename.c_str());
//	sprintf(buf,"%s\\%s_head.db","d:\\v70",dbfilename.c_str());

	//printf("DB path:%s\n",buf);

	m_dbheadfile=buf;
	sprintf(buf,"%s/data/idc_data/%s/%s_data",pMain->m_pOption->m_rootpath.c_str(),idcuser.c_str(),dbfilename.c_str());
//	sprintf(buf,"%s\\%s_data","d:\\v70",dbfilename.c_str());
	m_dbfilename=buf;

	//printf("DB name:%s\n",buf);
	printf("DB of idc user: %s\n",idcuser.c_str());

	int lint= LoadEx();
	if(lint!=1)
	{
		if(lint==-1)
			return CreateNewEx(m_dbheadfile,m_dbfilename,PageSize,1000);
		else
			return false;
	}
	return true;
}

int DB::AppendRecord(string monitorid,const char *data,S_UINT datalen)
{
	Table **ptable=NULL;
	{
		ost::MutexLock tablelock(m_tablemutex);
		ptable=m_tables.find(monitorid);
	}

	if(ptable==NULL)
	{
		printf("Table no exist: %s\n",monitorid.c_str());
		return TABLENOEXIST;
	}
    int nret=0;

	if(IdcUser::RecordsAutoBackup && m_pMain->m_pBackupRecordThread)
		m_pMain->m_pBackupRecordThread->AppendRecord((*ptable)->m_type.m_monitortype ,monitorid, data, datalen);

	{
		ost::MutexLock lock((*ptable)->m_mutex);
		nret=(*ptable)->AppendRecord(data,datalen);
		if(m_pMain->m_pOption->m_enablealert)
		{
			S_UINT blen=0;
			if((*ptable)->GetAlertInfoData(NULL,blen))
			{
				if(blen>0)
				{
					svutil::buffer pbuf;
					if(pbuf.checksize(blen))
					{
						if((*ptable)->GetAlertInfoData(pbuf,blen))
						{
							try{
								QueueRecord qrd(monitorid,svutil::TTime::GetCurrentTimeEx(),(const char*)pbuf,blen);
								S_UINT dlen=qrd.GetRawDataSize();
								if(pbuf.checksize(dlen))
								{
									if(qrd.GetRawData(pbuf,dlen)!=NULL)
									{
										if(!m_pMain->m_pDB->AppendQueueMessage(m_pMain->m_pOption->m_AlertQueueName,pbuf,dlen))
										{
											m_pMain->m_pDB->CreateQueue(m_pMain->m_pOption->m_AlertQueueName);
										}
									}
								}
							}catch(...)
							{
								;
							}
						}
					}
				}
			}
		}

	}
	return nret;
}

int DB::AppendMassRecord(std::list<SingelRecord> & listrcd)
{
	std::list<SingelRecord>::iterator it;
	int count(0);
	for(it=listrcd.begin(); it!=listrcd.end(); ++it)
		if( AppendRecord(it->monitorid,it->data,it->datalen)==1 )
			++count;
	return count;
}


bool DB::GetAllTableNames(std::list<string> &namelist)
{
	ost::MutexLock tablelock(m_tablemutex);

	TABLEMAP::iterator it;

	while(m_tables.findnext(it))
		namelist.push_back((*it).getkey().getword());

	return true;

}
bool DB::GetAllQueueNames(std::list<string> &namelist)
{
	ost::MutexLock queuelock(m_queuemutex);
	QUEUEMAP::iterator it;
	while(m_queues.findnext(it))
		namelist.push_back((*it).getkey().getword());

	return true;
}

bool DB::GetQueueAllMessageLabels(word queuename,std::list<string> & retlist)
{
	Queue **pqueue=NULL; 
	{
		ost::MutexLock queuelock(m_queuemutex);
		pqueue = m_queues.find(queuename);
	}
	if(pqueue==NULL)
	{
		puts("Queue no exist");
		return false;
	}
	if( (*pqueue)==NULL )
		return false;

	bool ret=true;
	try{
		ret= (*pqueue)->GetQueueAllMessageLabels(retlist);
	}catch(...)
	{
		cout<<"Exception to get all labels of queue."<<endl;
	}
	if(retlist.empty())
		ret=false;

	return ret;
}

bool DB::AppendQueueMessage(string queuename,const char *buf,S_UINT buflen)
{
	Queue **pqueue=NULL; 
	{
		ost::MutexLock queuelock(m_queuemutex);
		pqueue = m_queues.find(queuename);
	}
	if(pqueue==NULL)
	{
		puts("Queue no exist");
		return false;
	}

	bool nret=false;
	{
		ost::MutexLock lock((*pqueue)->m_mutex);
		nret=(*pqueue)->Append(buf,buflen);
	}

	return nret;

}
int  DB::PeekQueueMessage(string queuename,svutil::buffer&buf,S_UINT &datalen,bool block,S_UINT timer)
{
	Queue **pqueue=NULL;
	{
		ost::MutexLock queuelock(m_queuemutex);
		pqueue = m_queues.find(queuename);
	}
	if(pqueue==NULL)
	{
		puts("Queue no exist");
		return QUEUE_NOEXIST;
	}

	int nret=0;
	{
		nret=(*pqueue)->Peek(buf,datalen,block,timer);
	}

	return nret;

}
int DB::PopQueueMessage(string queuename,svutil::buffer &buf,S_UINT &datalen,bool block,S_UINT timer)
{
	Queue **pqueue=NULL;
	{
		ost::MutexLock queuelock(m_queuemutex);
		pqueue = m_queues.find(queuename);
	}
	if(pqueue==NULL)
	{
		puts("Queue no exist");
		return QUEUE_NOEXIST;
	}

	int nret=0;
	{
		nret=(*pqueue)->PopTop(buf,datalen,block,timer);
	}

	return nret;


}

int DB::GetQueueRecordCount(string queuename,S_UINT &count)
{
	Queue **pqueue=NULL;
	{
		ost::MutexLock queuelock(m_queuemutex);
		pqueue = m_queues.find(queuename);
	}
	if(pqueue==NULL)
	{
		puts("Queue no exist");
		return QUEUE_NOEXIST;
	}

	ost::MutexLock lock((*pqueue)->m_mutex);
	count=(*pqueue)->GetCount();

	return QUEUE_OK;

}
int DB::ClearQueueRecords(string queuename)
{
	Queue **pqueue=NULL;
	{
		ost::MutexLock queuelock(m_queuemutex);
		pqueue = m_queues.find(queuename);
	}
	if(pqueue==NULL)
	{
		puts("Queue no exist");
		return QUEUE_NOEXIST;
	}
	ost::MutexLock lock((*pqueue)->m_mutex);

	(*pqueue)->Clear();

	return QUEUE_OK;

}

int DB::DeleteQueue(string queuename)
{
	ost::MutexLock queuelock(m_queuemutex);
	Queue **pqueue = m_queues.find(queuename);
	if(pqueue==NULL)
	{
		puts("Queue no exist");
		return QUEUE_NOEXIST;
	}
	{
    	ost::MutexLock lock((*pqueue)->m_mutex);
		(*pqueue)->DeleteData();
		Page *pt=m_headpool->Get((*pqueue)->m_page->m_pos);
		if(pt!=NULL)
	     	m_headpool->PutFree(pt);
    	m_queues.erase(queuename);
	}
    
	return QUEUE_OK;

}
bool DB::Quit()
{
	return true;
	
}

