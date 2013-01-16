


#include "backuprecord.h"

#include <svdbtype.h>
#include <util.h>
#include "QueryData.h"
#include <RecordSet.h>
#include "DB.h"

CBackupRecord::CBackupRecord(void * pDB, CBackupRecordDamon * pDamon)
{
	m_pDamon= pDamon;
	m_pDB= pDB;
	m_connectable= false;
	m_cachedStuff= true;
	m_willSleep= true;
	m_exitAnyway= false;
}

CBackupRecord::~CBackupRecord()
{
	
}

void CBackupRecord::toExit()
{
	m_exitAnyway= true;
	this->exit();
	ThreadEx::sleep(5000);	
	MoveOneStep();
}

void CBackupRecord::run()
{
	m_exitAnyway= false;
	try{
		DB* pDB= static_cast<DB *>(m_pDB);
		pDB->CreateQueue(IdcUser::CacheQueueName, 1);
	}
	catch(...)
	{
	}
	ThreadEx::sleep(1000);	

	string text(" === BackupRecord InitTablesInfo ... ");
	puts(text.c_str());
	OutputDebugString(text.c_str());
	InitTablesInfo();

	//int count=1;
	while(true)
	{
		if(m_exitAnyway)
			break;

		text= " === BackupRecord run ... ";
		OutputDebugString(text.c_str());

		clock_t time1= clock();
		MoveOneStep();

		if(m_exitAnyway)
			break;
		if(m_willSleep)
			ThreadEx::sleep(IdcUser::msBackup);	
		else
			ThreadEx::sleep(2000);	

		//cout<<" ------- will sleep: "<<count * 360 <<"s "<<endl;
		//ThreadEx::sleep( count * 360 * 1000);	
		//cout<<" ------- sleep is done!  "<<endl;
		//++count;
	}
}


bool CBackupRecord::MoveOneStep()
{
	std::list<SingelRecord> listrcd;
	int size(0);
	int count= BackupThenClearAllRecords(listrcd,size);
	if( count>0 )
	{
		char text[1024]={0};
		sprintf(text," == Tele-backup %d records done, tables_info.size: %d\n",count,size);
		cout<<text;
		OutputDebugString(text);
	} 
	return true;
}


bool CBackupRecord::AppendRecord(int mTplId,string tablename,const char *pdata,int datalen)
{
	try{
		if(tablename.empty() || pdata==NULL || datalen==0)
			return false;
		if(!IdcUser::WillTeleBackup(tablename))
			return true;

		if(IdcUser::RecordsAutoBackup && IdcUser::IsAnLocalSEId(tablename) )
		{
			ost::MutexLock lock(m_records_Lock);
			std::map<string,CTableInfo>::iterator it= m_tables_info.find(tablename);
			if( it!=m_tables_info.end() )
			{
				it->second.local_tplId= mTplId;
				int ctid= it->second.center_tplId;
				if( ctid!=-1 && ctid!=mTplId )
				{
					char text[1024]={0};
					sprintf(text," == Refuse to tele-backup record \"%s\", for local/tele monitor_type: %d/%d\n",tablename.c_str(),mTplId,ctid);
					cout<<text;
					//OutputDebugString(text);
					return false;
				}
			}
			else
				m_tables_willcreate.insert(std::make_pair(tablename,CTableInfo(mTplId,-1,false)));
			return CacheRecords(tablename,pdata,datalen);
		}

		return true;
	}
	catch(...)
	{
		printf(" Exception in CBackupRecord::AppendRecord ! ");
		return false;
	}
}



int CBackupRecord::BackupThenClearAllRecords(std::list<SingelRecord> & inrcd, int & size)
{
	try{
			setVisit("t00", svutil::TTime::GetCurrentTimeEx().GetTime());
		inrcd.clear();
		m_cachedStuff= CheckCacheStuff();
			setVisit("t01", svutil::TTime::GetCurrentTimeEx().GetTime());
		std::map<string,CTableInfo> createT;
		{
		ost::MutexLock lock(m_records_Lock);
			setVisit("t02", svutil::TTime::GetCurrentTimeEx().GetTime());
		size= m_tables_info.size();
		m_cache_recods.swap(inrcd);
		if(inrcd.empty() && !m_cachedStuff)
		{
			m_willSleep= true;
			return 0;
		}
		SwapWillCreateTables(createT);
		}

			setVisit("t03", svutil::TTime::GetCurrentTimeEx().GetTime());
		bool ret(false);
		if(m_connectable && !m_cachedStuff && inrcd.size()<2000)
		{
			m_willSleep= true;
			std::map<string,CTableInfo> tempT;
				setVisit("t04", svutil::TTime::GetCurrentTimeEx().GetTime());
			tempT.swap(PreCreateTable(createT));
				setVisit("t05", svutil::TTime::GetCurrentTimeEx().GetTime());
			ret= AppendMassRecord(inrcd,"default",IdcUser::CenterAdress);
				setVisit("t06", svutil::TTime::GetCurrentTimeEx().GetTime());
			if(ret)
				createT.swap(tempT);
		}

		if(!ret)
		{
				setVisit("t07", svutil::TTime::GetCurrentTimeEx().GetTime());
			CacheInLocalDisk(inrcd, createT, size);
				setVisit("t08", svutil::TTime::GetCurrentTimeEx().GetTime());
			for(std::list<SingelRecord>::iterator it=inrcd.begin();  it!=inrcd.end(); ++it)
				if( (it->data)!=NULL )
					delete [] it->data;

				setVisit("t09", svutil::TTime::GetCurrentTimeEx().GetTime());
			bool tryresource= tryGetAllResourceInfo();
			if(tryresource)
				SendOutCache(600, size);
			setVisit("t10", svutil::TTime::GetCurrentTimeEx().GetTime());
			m_connectable= tryresource;
			return 0;
		}

		for(std::list<SingelRecord>::iterator it=inrcd.begin();  it!=inrcd.end(); ++it)
			if( (it->data)!=NULL )
				delete [] it->data;
		setVisit("t11", svutil::TTime::GetCurrentTimeEx().GetTime());
		{
		ost::MutexLock lock(m_records_Lock);
		for(std::map<string,CTableInfo>::iterator it= createT.begin(); it!=createT.end(); ++it)
			m_tables_info.insert(*it);
		size= m_tables_info.size();
		}
		setVisit("t12", svutil::TTime::GetCurrentTimeEx().GetTime());
		m_connectable= true;
		return inrcd.size();
	}
	catch(...)
	{
		printf(" ** Exception in CBackupRecord::BackupThenClearAllRecords ! ");
		OutputDebugString(" ** Exception in CBackupRecord::BackupThenClearAllRecords ! ");
		return -1;
	}
}


bool CBackupRecord::CacheRecords(string tablename,const char *pdata,int datalen)
{
	char * tdata(NULL);
	try{
		tdata=new char[datalen];
		if(datalen==0 || tdata==NULL || pdata==NULL)
			return false;
		memmove(tdata,pdata,datalen);
	}
	catch(...)
	{
		return false;
	}
	SingelRecord rcd;
	rcd.monitorid= tablename;
	rcd.data= tdata;
	rcd.datalen= datalen;

	m_cache_recods.push_back(rcd);
	return true;
}


bool CBackupRecord::AppendMassRecord(std::list<SingelRecord> & listrcd,string user,string addr)
{
	if(listrcd.empty()||user.empty()||addr.empty())
		return false;

	unsigned int len= GetMassRecordListRawDataSize(listrcd);
	svutil::buffer buf;
	if(!buf.checksize(len))
		return false;

	const char *data= GetMassRecordListRawData(listrcd,buf,len); 
	if(data==NULL)
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_DB;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_APPENDMASSRECORD;
	query.datalen=len;

	QueryData qd;
	char *pdata=NULL;
	SVDBRESULT ret={0};

	return qd.UpdateWithTimeOut(&query,data,len,&ret,addr);
}


//connectable ,		need time 0.8ms
//unconnectable ,	need time 21s
bool CBackupRecord::tryGetAllResourceInfo()
{
	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_INFO;
	querybuf.datatype=S_RESOURCE;

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;
	bool ret=qd.QueryWithTimeOut(&querybuf, (void **)&pdata, len, IdcUser::CenterAdress);
	if(pdata!=NULL)
		delete [] pdata;

	return ret;
}


bool QuerySVDYNs( string pid, std::list<SingelRecord> & listrcd, std::list<SingelRecord> & listrcd_out )
{
	unsigned int tlen= GetMassRecordListRawDataSize(listrcd);
	svutil::buffer tbuf;
	if(!tbuf.checksize(tlen))
		return false;

	const char *data= GetMassRecordListRawData(listrcd,tbuf,tlen); 
	if(data==NULL)
		return false;

	QueryData qd;
	char *pdata=NULL;
	S_UINT rlen=0;

	S_UINT len=0;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_MASSDYN;
	querybuf.datatype=S_DB;
	strcpy(querybuf.qstr,pid.c_str());

	INIQUERY iquery={0};
	iquery.len=sizeof(INIQUERY);
	iquery.datatype=D_STRING;
	iquery.datalen=tlen;

	len+=sizeof(INIQUERY);
	len+=tlen;

	querybuf.datalen=len;

	buffer buf;
	if(!buf.checksize(len))
		return INVALID_VALUE;
	char *pt=buf.getbuffer();
	memcpy(pt,&iquery,sizeof(INIQUERY));
	pt+=sizeof(INIQUERY);
	memmove(pt,data,tlen);


	if(qd.QueryWithTimeOut(&querybuf,buf,len,(void **)&pdata,rlen,IdcUser::CenterAdress))
	{
		try{
			std::list<SingelRecord> listrcd2;
			std::list<SingelRecord>::iterator lit;
			if( CreateMassRecordListByRawData(listrcd2,pdata,rlen) )
			{
				for(lit=listrcd2.begin(); lit!=listrcd2.end(); ++lit)
				{
					SingelRecord rcd;
					rcd.monitorid= lit->monitorid;
					rcd.datalen= lit->datalen;
					if(lit->datalen)
					{
						char * tempchar= new char[lit->datalen];
						memmove(tempchar,lit->data,lit->datalen);
						rcd.data= tempchar;
					}
					else
						rcd.data= NULL;

					listrcd_out.push_back(rcd);
				}
				if(pdata!=NULL)
					delete [] pdata;
				return true;
			}
		}catch(...)
		{
			printf("Exception to QuerySVDYNs in CBackupRecord. \n");
		}
	}
	if(pdata!=NULL)
		delete [] pdata;
	if(rlen==0)
		return true;
	return false;
}


bool GetMonitorTplIdOfCenterTables(string pid, NodeData & ndata)
{
	if(pid.empty())
		return false;
	if(pid.compare("default")==0)
		return false;
	if(pid.size()>MAXQUEREYSTRINGLEN)
		return false;

	std::list<SingelRecord> listrcd;
	std::list<SingelRecord> listrcd_out;
	if( !QuerySVDYNs(pid,listrcd,listrcd_out) )
		return false;
	if(listrcd_out.empty())
		return true;

	for(std::list<SingelRecord>::iterator litin=listrcd_out.begin(); litin!=listrcd_out.end(); ++litin)
	{
		try{
			std::list<SingelRecord> listrcd;
			if( !CreateMassRecordListByRawData(listrcd,litin->data,litin->datalen) )
				continue;
			std::list<SingelRecord>::iterator lit= listrcd.begin();
			if( (lit== listrcd.end()) || (++lit== listrcd.end()) )
				continue;

			ndata.insert(std::make_pair(litin->monitorid, lit->monitorid));
		}catch(...)
		{
			printf("Exception to GetMonitorTplIdOfCenterTables in CBackupRecord .\n");
		}
		if( litin->data!=NULL )
			delete [] litin->data;
	}
	return true;
}


bool GetTable(string monitorid)
{
	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_DB;
	strcpy(query.idcuser,"default");
	query.querytype=QUERY_GETDYN;
	strcpy(query.qstr,monitorid.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;

	bool bret= qd.QueryWithTimeOut(&query,(void **)&pdata,len,IdcUser::CenterAdress);
	if(pdata!=NULL)
		delete [] pdata;

	return bret;
}


bool AddTable(string monitorid,int monitortype)
{
	if(monitorid.empty() || monitortype<=0)
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_DB;
	strcpy(query.idcuser,"default");
	query.querytype=QUERY_INSERTTABLE;
	strcpy(query.qstr,monitorid.c_str());
	query.datalen=monitortype;

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;

	if(qd.QueryWithTimeOut(&query,(void **)&pdata,len,IdcUser::CenterAdress))
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
	delete [] pdata;
	return false;
}

void CBackupRecord::SwapWillCreateTables(std::map<string,CTableInfo> & tables)
{
	return m_tables_willcreate.swap(tables);
}

std::map<string,CTableInfo> CBackupRecord::PreCreateTable(std::map<string,CTableInfo> & tables)
{
	std::map<string,CTableInfo> createdT;
	for(std::map<string,CTableInfo>::iterator it=tables.begin(); it!=tables.end(); ++it)
	{
		string tableid= it->first;
		int tplId= it->second.local_tplId;

		std::map<string,CTableInfo>::iterator tit= createdT.find(tableid);
		if( tit!=createdT.end() )
		{
			tit->second.local_tplId= tplId;
			continue;
		}

		if(!it->second.created)
		{
			if( GetTable(tableid) )
			{
				NodeData ndata;
				GetMonitorTplIdOfCenterTables(GetParentID(tableid.c_str()).getword(),ndata);
				for(NodeData::iterator nit=ndata.begin(); nit!=ndata.end(); ++nit)
				{
					//cout<<"  ------  "<<nit->first.c_str()<<" = "<<nit->second.c_str()<<endl;
					createdT.insert(std::make_pair(nit->first, CTableInfo(-1, atoi(nit->second.c_str()), true) ));
				}
				continue;
			}
			if( AddTable(tableid, tplId) )
				createdT.insert(std::make_pair(tableid, CTableInfo(tplId,tplId,true) ));
		}
	}
	return createdT;
}


void CBackupRecord::InitTablesInfo(bool earliest)
{
	static bool initdone(false);
	if(initdone)
		return;

	std::set<int> ses= IdcUser::GetLocalSEId();
	std::map<string,CTableInfo> createdT;

	for(std::set<int>::iterator it= ses.begin(); it!=ses.end(); ++it)
	{
		char id[32]={0};
		sprintf(id,"%d",*it);

		NodeData ndata;
		GetMonitorTplIdOfCenterTables(id, ndata);
		for(NodeData::iterator nit=ndata.begin(); nit!=ndata.end(); ++nit)
		{
			createdT.insert(std::make_pair(nit->first, CTableInfo(-1, atoi(nit->second.c_str()), true) ));
		}
	}
	cout<<" == Tables_info size of BackupRecord Init : "<<createdT.size()<<endl;
	if(createdT.empty())
		return;

	if(earliest)
	{
		ost::MutexLock lock(m_records_Lock);
		m_tables_info.swap(createdT);
		initdone= true;
	}
	else
	{
		ost::MutexLock lock(m_records_Lock);
		for(std::map<string,CTableInfo>::iterator it= createdT.begin(); it!= createdT.end(); ++it)
		{
			std::map<string,CTableInfo>::iterator it2= m_tables_info.find(it->first);
			if(it2==m_tables_info.end())
			{
				m_tables_info.insert(*it);
				continue;
			}
			it2->second.center_tplId= it->second.center_tplId;
			it2->second.created= true;
		}
		initdone= true;
	}
}

bool CBackupRecord::CacheSaver(std::list<SingelRecord> & listrcd, int infoSize)
{
	try{
		if(listrcd.empty())
			return false;
		DB* pDB= static_cast<DB *>(m_pDB);

		unsigned int len= GetMassRecordListRawDataSize(listrcd);
		if(len>=QBUFFERLEN)
		{
			string text= " ** Too big data in CBackupRecord::CacheSaver, should check it!  \n";
			cout<<text.c_str();
			OutputDebugString(text.c_str());
			AddToErrorLog(text);
			return false;
		}
		svutil::buffer buf;
		if(!buf.checksize(len))
			return false;
		const char *data= GetMassRecordListRawData(listrcd,buf,len); 
		if(data==NULL)
			return false;

		svutil::TTime ct=svutil::TTime::GetCurrentTimeEx();
		QueueRecord qr("RecordsData",ct,data,len);
		S_UINT dlen=qr.GetRawDataSize();
		svutil::buffer dbuf;
		if(!dbuf.checksize(dlen))
			return false;
		if(!qr.GetRawData(dbuf,dlen))
			return false;

		for(int i=1; i<=2; ++i)
		{
			if( pDB->AppendQueueMessage(IdcUser::CacheQueueName, dbuf, dlen) )
			{
				char text[1024]={0};
				sprintf(text," == Stored %d records in local disk cache, tables_info.size: %d ",listrcd.size(),infoSize);
				cout<<text;
				OutputDebugString(text);
				return true;
			}
			pDB->CreateQueue(IdcUser::CacheQueueName, 1);
		}
	}
	catch(...)
	{
		printf(" Exception in CBackupRecord::CacheSaver  ! ");
	}
	return false;
}


// mainly 70 bytes/per record 
bool CBackupRecord::CacheInLocalDisk(std::list<SingelRecord> & listrcd, std::map<string,CTableInfo> & tables, int infoSize)
{
	try{
		std::map<string,CTableInfo> tableInfo;
		if(listrcd.empty())
			return false;
		else
		{
			ost::MutexLock lock(m_records_Lock);
			std::map<string,CTableInfo> info(m_tables_info);
			tableInfo.swap(info);
		}
		DB* pDB= static_cast<DB *>(m_pDB);

		for(std::list<SingelRecord>::iterator it=listrcd.begin(); it!=listrcd.end(); ++it)
		{
			int localTplId(0);

			std::map<string,CTableInfo>::iterator mit= tables.find(it->monitorid);
			if(mit!=tables.end())
				localTplId= mit->second.local_tplId;
			else
			{
				mit= tableInfo.find(it->monitorid);
				if(mit!=tableInfo.end())
					localTplId= mit->second.local_tplId;
			}
			if(localTplId==0)
				cout<<"  ** Record \""<<it->monitorid.c_str()<<"\" 's monitor_type is 0 "<<endl;

			char text[32]={0};
			sprintf(text,"_[mType]%d",localTplId);
			it->monitorid += text;
		}

		unsigned int len= GetMassRecordListRawDataSize(listrcd);
		if(len<QBUFFERLEN)
			return CacheSaver(listrcd, infoSize);

		char text[1024]={0};
		sprintf(text," == Will total store %d records (len: %d ) in local disk cache",listrcd.size(),len);
		cout<<text;
		OutputDebugString(text);

		int logsize= listrcd.size();
		int onesize= logsize/(1.25*len/QBUFFERLEN);

		bool reachEnd(false),forceout(false);
		int count(0);
		std::list<SingelRecord>::iterator lit1= listrcd.begin();
		std::list<SingelRecord>::iterator lit2= listrcd.begin();
		while(1)
		{
			for(int index=1; index<=onesize; ++index)
			{
				if(index==1)
					lit2= lit1;
				if(++lit2==listrcd.end())
				{
					reachEnd= true;
					break;
				}
				if( ++count > logsize )
				{
					forceout= true;
					break;
				}
			}
			std::list<SingelRecord> rcd(lit1,lit2,listrcd.get_allocator());
			CacheSaver(rcd, infoSize);
			if(reachEnd || forceout)
				break;
			lit1= lit2;
		}
		if(forceout)
		{
			string text= " ** Wrong in CBackupRecord::CacheInLocalDisk, should check it!  \n";
			cout<<text.c_str();
			OutputDebugString(text.c_str());
			AddToErrorLog(text);
		}
	}
	catch(...)
	{
		printf(" Exception in CBackupRecord::CacheInLocalDisk  ! ");
	}
	return false;
}

bool CBackupRecord::CheckCacheStuff()
{
	try{
		DB* pDB= static_cast<DB *>(m_pDB);
		S_UINT count(0);
		int nret= pDB->GetQueueRecordCount(IdcUser::CacheQueueName, count);
		if(nret!=QUEUE_OK || count==0)
			return false;
		else
			return true;
	}
	catch(...)
	{
		printf(" Exception in CBackupRecord::CheckCacheStuff  ! ");
		return false;
	}
}

bool CBackupRecord::DeleteOneCacheData(int & count, bool show)
{
	DB* pDB= static_cast<DB *>(m_pDB);
	svutil::buffer buf;
	if(!buf.checksize(1024))
		return false;
	S_UINT datalen= 1024;
	int ret=pDB->PopQueueMessage(IdcUser::CacheQueueName, buf, datalen, false, 0);
	if(show && ret==QUEUE_OK )
	{
		string text= " ** Deleted an unable to parse cache RecordsData.\n";
		cout<<text.c_str();
		OutputDebugString(text.c_str());
		AddToErrorLog(text);
	}
	
	if(ret!=QUEUE_OK)
	{
		char text[1024]={0};
		sprintf(text," ** Failed to delete an unable to parse cache RecordsData, %d .\n", ret);
		cout<<text;
		OutputDebugString(text);
		AddToErrorLog(text);
				
		++count;
		if(count>5)
			ret=QUEUE_UNKNOWERROR;
	}

	if(ret==QUEUE_UNKNOWERROR || ret==QUEUE_GETPAGEERROR)
	{
		pDB->DeleteQueue(IdcUser::CacheQueueName);
		pDB->CreateQueue(IdcUser::CacheQueueName, 1);
		char text[1024]={0};
		sprintf(text," ** Unknown error or get page error happened to cache queue, it was just re-created.\n", ret);
		cout<<text;
		OutputDebugString(text);
		AddToErrorLog(text);
	}
	return ret==QUEUE_OK;
}


bool CBackupRecord::SendOutCache(int messagecount, int infoSize)
{
	try{
		clock_t time1=clock(); 
		InitTablesInfo(false);
		m_willSleep= true;

		int count(0), fcount(1), delcount(0);
		DB* pDB= static_cast<DB *>(m_pDB);
		for(int index=1; index<=messagecount; ++index)
		{
			int nret=0;
			svutil::buffer buf;
			if(!buf.checksize(1024))
				continue;
			S_UINT datalen=1024;

			nret= pDB->PeekQueueMessage(IdcUser::CacheQueueName, buf, datalen, false, 0);
			if(nret==QUEUE_EMPTY)
			{
				char text[1024]={0};
				sprintf(text," ** Local cache is empty, %dth work.\n", index);
				cout<<text<<endl;
				OutputDebugString(text);
				break;
			}
			if(nret!=QUEUE_OK || datalen>=QBUFFERLEN || datalen<sizeof(QUEUEHEAD))
			{
				DeleteOneCacheData(delcount);
				continue;
			}

			QueueRecord qrd;
			const char *pt= buf+sizeof(QUEUEHEAD);
			if(!qrd.CreateObjectByRawData(pt, datalen-sizeof(QUEUEHEAD)))
			{
				DeleteOneCacheData(delcount);
				continue;
			}

			string label=qrd.GetLabel();
			std::list<SingelRecord> listrcd;
			if(label.compare("RecordsData")!=0 || !CreateMassRecordListByRawData(listrcd, qrd.GetData(), qrd.GetDataLen()) )
			{
				DeleteOneCacheData(delcount);
				continue;
			}

			FilterRefusedRecord(listrcd);
			
			if( AppendMassRecord(listrcd, "default", IdcUser::CenterAdress) )
			{
				count+= listrcd.size();
				fcount= 1;
				delcount= 0;
				if( !DeleteOneCacheData(delcount, false) )
					cout<<" ** Failed to deleted cache RecordsData, that has sent to backup center already."<<endl;
				else
				{
					char text[1024]={0};
					sprintf(text," == Just sent out %d cache records, %dth work.\n", listrcd.size(), index);
					cout<<text;
					OutputDebugString(text);
				}
			}
			else
			{
				if(!tryGetAllResourceInfo())
					break;
				cout<<" Failed to AppendMassRecord to backup center , "<<fcount<<"th try "<<endl;
				++fcount;
			}
			if(fcount>=3)
			{
				if( DeleteOneCacheData(delcount, false) )
				{
					fcount= 1;
					delcount= 0;
					string text= " ** Abandon a bad cache RecordsData, something wrong in local or backup center.";
					cout<<text.c_str()<<endl;
					OutputDebugString(text.c_str());
					AddToErrorLog(text);
				}
				else
					delcount= 100;
				continue;
			}
			if( (clock()- time1)>(2*IdcUser::msBackup) )
			{
				m_willSleep= false;
				break;
			}
			if(m_exitAnyway)
				break;
			ThreadEx::sleep(10);	
		}
		double runtime= ((double)clock()-time1)/1000;

		char text[1024]={0};
		sprintf(text,"\n == Sent out %d cache records within %.2f s, tables_info.size: %d ", count, runtime, infoSize);
		cout<<text<<endl;
		OutputDebugString(text);
		return true;
	}
	catch(...)
	{
		printf(" Exception in CBackupRecord::SendOutCache  ! ");
		return false;
	}
}


bool CBackupRecord::FilterRefusedRecord(std::list<SingelRecord> & listrcd)
{
	std::map<string,CTableInfo> createT;
	if( !listrcd.empty() )
	{
		ost::MutexLock lock(m_records_Lock);
		for(std::list<SingelRecord>::iterator it=listrcd.begin(); it!=listrcd.end(); ++it)
		{
			int localTplId(0);
			string tablename;
			try{
				string text= it->monitorid;
				string name= text;
				std::string::size_type pos= text.find("_[mType]");
				if(pos!=std::string::npos)
				{
					text= text.substr(pos+8);
					localTplId=	atoi(text.c_str());
					tablename= name.erase(pos);
				}
			}
			catch(...)
			{
				localTplId=0;
			}
			if(localTplId==0 || tablename.empty())
			{
				it->data= NULL;
				it->datalen= 0;
				continue;
			}
			else
			{	
				it->monitorid= tablename;

				std::map<string,CTableInfo>::iterator m_it= m_tables_info.find(tablename);
				if( m_it!=m_tables_info.end() )
				{
					m_it->second.local_tplId= localTplId;
					int ctid= m_it->second.center_tplId;
					if( ctid!=-1 && ctid!=localTplId )
					{
						char text[1024]={0};
						sprintf(text," == Refuse to tele-backup record(cache) \"%s\", for local/tele monitor_type: %d/%d\n",tablename.c_str(),localTplId,ctid);
						cout<<text;
						//OutputDebugString(text);

						it->data= NULL;
						it->datalen= 0;
						continue;
					}
				}
				else
					createT.insert(std::make_pair(tablename,CTableInfo(localTplId,-1,false)));
			}
		}
	}

	std::map<string,CTableInfo> tableInfo;
	std::map<string,CTableInfo> & tempT= PreCreateTable(createT) ;
	if( tempT.empty() )
	{
		ost::MutexLock lock(m_records_Lock);
		for(std::map<string,CTableInfo>::iterator it= tempT.begin(); it!=tempT.end(); ++it)
			m_tables_info.insert(*it);
		std::map<string,CTableInfo> info(m_tables_info);
		tableInfo.swap(info);
	}

	for(std::list<SingelRecord>::iterator it=listrcd.begin(); it!=listrcd.end(); ++it)
	{
		string tablename= it->monitorid;
		if(it->data==NULL || it->datalen==0)
			continue;

		std::map<string,CTableInfo>::iterator m_it= tableInfo.find(tablename);
		if( m_it!=tableInfo.end() )
		{
			int localTplId= m_it->second.local_tplId;
			int ctid= m_it->second.center_tplId;
			if( ctid!=-1 && ctid!=localTplId )
			{
				char text[1024]={0};
				sprintf(text," == Refuse_2 to tele-backup record(cache) \"%s\", for local/tele monitor_type: %d/%d\n",tablename.c_str(),localTplId,ctid);
				cout<<text;
				//OutputDebugString(text);

				it->data= NULL;
				it->datalen= 0;
				continue;
			}
		}
	}
	return true;
}


void CBackupRecord::setVisit(std::string key, time_t time)
{
	try{
		if( m_pDamon==NULL )
			return;
		m_pDamon->setVisit(key, time);
	}
	catch(...)
	{
	}
}




