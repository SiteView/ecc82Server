#include "svdbapi.h"
#include "QueryData.h"
#include <RecordSet.h>
#include <util.h>

#include <stlini.h>
static OBJECT g_resource=INVALID_VALUE;

SVAPI_API
bool BuildDynByData(const char *data,unsigned int len,SVDYN &dyn)
{
	bool bret=false;
	try{
		bret=builddyn(data,len,dyn,true);
	}catch(...)
	{
		return false;
	}

	return bret;


}

SVAPI_API 
bool GetSVDYN(string monitorid,SVDYN &dyn,string user,string addr)
{
	if(monitorid.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_DB;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_GETDYN;
	strcpy(query.qstr,monitorid.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;

	bool bret=false;

	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		if(pdata!=NULL)
		{
			try{
				bret=builddyn(pdata,len,dyn,true);

				delete [] pdata;
				return bret;

			}catch(...)
			{
				return false;
			}
		}

	}

	return false;
}

SVAPI_API
bool GetSVDYNNODisplayString(string monitorid,SVDYN &dyn,string user,string addr)
{
	if(monitorid.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_DB;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_GETDYN;
	strcpy(query.qstr,monitorid.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;

	bool bret=false;

	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		if(pdata!=NULL)
		{
			try{
				bret=builddyn(pdata,len,dyn,false);

				delete [] pdata;
				return bret;

			}catch(...)
			{
				return false;
			}
		}

	}

	return false;

}

bool MakeErrorRecord(char *buf,int &buflen,int state,string dstr)
{
	if(buf==NULL)
		return false;
	if(buflen<sizeof(RecordHead)+dstr.size()+1)
		return false;

	((RecordHead *)buf)->state=state;
	((RecordHead *)buf)->datalen=dstr.size()+1;
	((RecordHead *)buf)->createtime=svutil::TTime::GetCurrentTimeEx();

	char *pt=buf+sizeof(RecordHead);
	strcpy(pt,dstr.c_str());
	pt+=dstr.size()+1;
	pt[0]='\0';
	buflen=pt-buf;

	return true;

}

SVAPI_API
bool SetDYN(string monitorid,int type,string user,string addr)
{
	if(monitorid.empty())
		return false;

	string error="";
	if(type==Record::STATUS_DISABLE)
		error="¼à²âÆ÷±»½ûÖ¹";
	else if(type==Record::STATUS_NULL)
		error="ÆôÓÃ¼à²â";
	else
		return false;

	if(g_resource==INVALID_VALUE)
	{
		g_resource=::LoadResource("default",addr);
	}
	if(g_resource!=INVALID_VALUE)
	{

		MAPNODE map=GetResourceNode(g_resource);
		if(map!=INVALID_VALUE)
		{
			string svalue="";
			if(type==Record::STATUS_DISABLE)
			{
				if(FindNodeValue(map,"IDS_MonitorDisabled",svalue))
					error=svalue;
			}else if(type==Record::STATUS_NULL)
			{
				if(FindNodeValue(map,"IDS_ENABLE_MONITOR",svalue))
					error=svalue;

			}
		}

	}



	char buf[256]={0};
	int blen=256;
	if(MakeErrorRecord(buf,blen,type,error))
	{
		if(blen+error.size()+1>256)
			return false;

		char *pt=buf+blen;
		strcpy(pt,error.c_str());
		blen+=error.size()+1;
		return ::AppendRecord(monitorid,buf,blen,user,addr);
	}

	return false;
    
}

/////////////////////////////////DB///////////////////////////

SVAPI_API
RECORDSET QueryRecords(string monitorid,TTimeSpan recenttimespan,string user,string addr)
{
	if(monitorid.empty()||user.empty()||addr.empty())
		return INVALID_VALUE;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_DB;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_GET;
	strcpy(query.qstr,monitorid.c_str());
	query.datalen=sizeof(DBQUERY);

	DBQUERY dbquery={0};
	dbquery.len=sizeof(DBQUERY);
	dbquery.type=B_RECENTSPAN;
	dbquery.span=recenttimespan;
	
	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;



	if(qd.Query(&query,&dbquery,(void **)&pdata,len,addr))
	{
		if(g_resource==INVALID_VALUE)
		{
			g_resource=::LoadResource("default",addr);
		}
		if(g_resource==INVALID_VALUE)
		{
			delete [] pdata;
			return INVALID_VALUE;
		}
		MAPNODE mrd=GetResourceNode(g_resource);
		if(mrd==INVALID_VALUE)
		{
			delete [] pdata;
			return INVALID_VALUE;
		}

		RecordSet *prst = new RecordSet();
		if(prst)
		{
			if(pdata)
			{
				RECORDSET ret=INVALID_VALUE;
				try{
					StringMap *smap=reinterpret_cast< StringMap * >(mrd);
					if(prst->CreateObjectByRawData(smap,pdata,len))
						ret=reinterpret_cast<RECORDSET>(prst);
					delete [] pdata;
				}catch(...)
				{
					return INVALID_VALUE;
				}

				return ret;
			}
		}


	}

	return INVALID_VALUE;

}

SVAPI_API
bool DeleteRecords(string monitorid,svutil::TTime before,string user,string addr)
{
	if(monitorid.empty()||user.empty()||addr.empty())
		return false;
	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_DB;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_DELETERECORDS;
	strcpy(query.qstr,monitorid.c_str());
	query.datalen=sizeof(DBQUERY);


	DBQUERY dbquery={0};
	dbquery.len=sizeof(DBQUERY);
	dbquery.type=B_SPAN;
	dbquery.begin=before;
//	dbquery.end=end;

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;


	if(qd.Query(&query,&dbquery,(void **)&pdata,len,addr))
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
		}
	
	}

	return false;


}

SVAPI_API
RECORDSET QueryRecords(string monitorid,int count,string user,string addr)
{
	if(monitorid.empty()||user.empty()||addr.empty())
		return INVALID_VALUE;

	if(count<=0)
		return INVALID_VALUE;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_DB;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_GETDATABYCOUNT;
	strcpy(query.qstr,monitorid.c_str());
	query.datalen=count;

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;


	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		if(g_resource==INVALID_VALUE)
		{
			g_resource=::LoadResource("default",addr);
		}
		if(g_resource==INVALID_VALUE)
		{
			delete [] pdata;
			return INVALID_VALUE;
		}
		MAPNODE mrd=GetResourceNode(g_resource);
		if(mrd==INVALID_VALUE)
		{
			delete [] pdata;
			return INVALID_VALUE;
		}

		RecordSet *prst = new RecordSet();
		if(prst)
		{
			if(pdata!=NULL)
			{
				RECORDSET ret=INVALID_VALUE;
				try{
					StringMap *smap=reinterpret_cast< StringMap * >(mrd);
					if(prst->CreateObjectByRawData(smap,pdata,len))
						ret=reinterpret_cast<RECORDSET>(prst);
					delete [] pdata;
				}catch(...)
				{
					return INVALID_VALUE;
				}

				return ret;
			}
		}


	}

	return INVALID_VALUE;






}

SVAPI_API
RECORDSET QueryRecords(string monitorid,TTime begin,TTime end,string user,string addr)
{
	if(monitorid.empty()||user.empty()||addr.empty())
		return INVALID_VALUE;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_DB;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_GET;
	strcpy(query.qstr,monitorid.c_str());
	query.datalen=sizeof(DBQUERY);


	DBQUERY dbquery={0};
	dbquery.len=sizeof(DBQUERY);
	dbquery.type=B_SPAN;
	dbquery.begin=begin;
	dbquery.end=end;
	
	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;


	if(qd.Query(&query,&dbquery,(void **)&pdata,len,addr))
	{
		if(g_resource==INVALID_VALUE)
		{
			g_resource=::LoadResource("default",addr);
		}
		if(g_resource==INVALID_VALUE)
		{
			delete [] pdata;
			return INVALID_VALUE;
		}
		MAPNODE mrd=GetResourceNode(g_resource);
		if(mrd==INVALID_VALUE)
		{
			delete [] pdata;
			return INVALID_VALUE;
		}
		RecordSet *prst = new RecordSet();
		if(prst)
		{
			if(pdata!=NULL)
			{
				RECORDSET ret=INVALID_VALUE;
				try{
					StringMap *smap=reinterpret_cast< StringMap * >(mrd);
					if(prst->CreateObjectByRawData(smap,pdata,len))
						ret=reinterpret_cast<RECORDSET>(prst);
					delete [] pdata;
				}catch(...)
				{
					return INVALID_VALUE;
				}

				return ret;
			}
		}


	}

	return INVALID_VALUE;
}


SVAPI_API
bool QueryRecordCount(string monitorid,int & count,string user,string addr)
{
	if(monitorid.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_RCD_COUNT;
	querybuf.datatype=S_DB;
	strcpy(querybuf.qstr,monitorid.c_str());
	querybuf.datalen=sizeof(DBQUERY);


	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;

	bool bret=false;
	count=0;

	if(qd.Query(&querybuf,(void **)&pdata,len,addr))
	{
		if((pdata!=NULL)&&(len==sizeof(int)))
		{
			int nret=0;
			memmove(&nret,pdata,sizeof(int));
			delete [] pdata;
			count= nret;
			return true;
		}
		if(pdata!=NULL)
			delete [] pdata;
	}

	return false;
}


SVAPI_API
bool GetRecordCount(RECORDSET rset,size_t &count)
{
	if(rset==INVALID_VALUE)
		return false;


	try{
	     RecordSet *prs=reinterpret_cast<RecordSet *>(rset);
		 count=prs->m_records.size();
	}catch(...)
	{
		return false;
	}
	return true;
}

SVAPI_API
bool GetReocrdSetField(RECORDSET rset,std::list<string> &fieldlist)
{
	if(rset==INVALID_VALUE)
		return false;


	try{
	     RecordSet *prs=reinterpret_cast<RecordSet *>(rset);
		 int size=prs->m_type.m_data.size();
		 for(int i=0;i<size;i++)
		 {
			 DataType *ptype=prs->m_type.m_data[i];
			 if(ptype)
			 {
				 fieldlist.push_back(ptype->name);
			 }
		 }
		 
	}catch(...)
	{
		return false;
	}
	return true;
}

SVAPI_API
bool GetReocrdSetMonitorType(RECORDSET rset,int &type)
{
	if(rset==INVALID_VALUE)
		return false;


	try{
	     RecordSet *prs=reinterpret_cast<RecordSet *>(rset);
		 type=prs->m_type.m_monitortype;
		 
	}catch(...)
	{
		return false;
	}
	return true;
}

SVAPI_API
bool GetRecordSetMonitorID(RECORDSET rset,string &monitorid)
{
	if(rset==INVALID_VALUE)
		return false;


	try{
	     RecordSet *prs=reinterpret_cast<RecordSet *>(rset);
		 monitorid=prs->m_type.m_monitorid;
		 
	}catch(...)
	{
		return false;
	}
	return true;
}

SVAPI_API 
bool FindRecordFirst(RECORDSET rset,LISTITEM &item)
{
	if(rset==INVALID_VALUE)
		return false;
	try{
	     RecordSet *prs=reinterpret_cast<RecordSet *>(rset);
		 item.list=reinterpret_cast<OBJECT>(&prs->m_records);
		 RECORDLIST::iterator *pit = new RECORDLIST::iterator();
		 if(pit==NULL)
			 return false;
		 (*pit)=prs->m_records.begin();
		 item.iterator=reinterpret_cast<OBJECT>(pit);
		 
	}catch(...)
	{
		return false;
	}
	return true;
}

SVAPI_API
RECORD FindNextRecord(LISTITEM &item)
{
	if((item.iterator==INVALID_VALUE)||(item.list==INVALID_VALUE))
		return INVALID_VALUE;

	try{

		RECORDLIST *plist=reinterpret_cast< RECORDLIST * >(item.list);
		RECORDLIST::iterator *it=reinterpret_cast< RECORDLIST::iterator * >(item.iterator);

		if((*it)!=plist->end())
		{
			RECORD ret=reinterpret_cast<RECORD>((*(*it)));
			(*it)++;
			return ret;
		}
	}catch(...)
	{
		return INVALID_VALUE;
	}


	return INVALID_VALUE;
}

SVAPI_API
RECORD FindPreReocrd(LISTITEM &item)
{
	if((item.iterator==INVALID_VALUE)||(item.list==INVALID_VALUE))
		return INVALID_VALUE;
	try{

		RECORDLIST *plist=reinterpret_cast< RECORDLIST * >(item.list);
		RECORDLIST::iterator *it=reinterpret_cast< RECORDLIST::iterator * >(item.iterator);

		if((*it)!=plist->begin())
		{
			(*it)--;
			RECORD ret=reinterpret_cast<RECORD>((*(*it)));
			return ret;
		}
	}catch(...)
	{
		return INVALID_VALUE;
	}


	return INVALID_VALUE;
}

SVAPI_API
void ReleaseRecordList(LISTITEM &item)
{
	if(item.list==INVALID_VALUE)
		return;
	if(item.iterator==INVALID_VALUE)
		return;
	try{
		RECORDLIST::iterator *it=reinterpret_cast< RECORDLIST::iterator * >(item.iterator);
		delete it;
	}catch(...)
	{
		;
	}
	item.iterator=INVALID_VALUE;
	item.list=INVALID_VALUE;


}

SVAPI_API
bool ResetRecordList(LISTITEM &item)
{
	if((item.iterator==INVALID_VALUE)||(item.list==INVALID_VALUE))
		return false;
	try{

		RECORDLIST *plist=reinterpret_cast< RECORDLIST * >(item.list);
		RECORDLIST::iterator *it=reinterpret_cast< RECORDLIST::iterator * >(item.iterator);
		(*it)=plist->begin();
		return true;
	}catch(...)
	{
		return false;
	}

	return false;

}


SVAPI_API
bool GetRecordState(RECORD rd,int &state)
{
	if(rd==INVALID_VALUE)
		return false;
	try{
		Record *prd=reinterpret_cast< Record * >(rd);
		state=prd->GetState();

	}catch(...)
	{
		return false;
	}
	return true;
}

SVAPI_API
bool GetRecordCreateTime(RECORD rd,TTime &tm)
{
	if(rd==INVALID_VALUE)
		return false;
	try{
		Record *prd=reinterpret_cast< Record * >(rd);
		tm=prd->GetCreateTime();

	}catch(...)
	{
		return false;
	}
	return true;
}

SVAPI_API
bool GetRecordValueByField(RECORD rd,string Field,int &type,int &state,int &iv,float &fv,string &sv)
{
	if(rd==INVALID_VALUE)
		return false;
	try{
		Record *prd=reinterpret_cast< Record * >(rd);
		state=prd->GetState();

		if((state==Record::STATUS_BAD)||(state==Record::STATUS_NULL)||(state==Record::STATUS_DISABLE))
		{
			sv=prd->GetErrorDescription();
			type=DataType::stringtype;
			return true;
		}

		RecordValue rdv;
		if(!prd->GetValue(Field,rdv))
			return false;
		type=rdv.vt;
		if(rdv.vt==DataType::inttype)
			iv=(int)rdv;
		else if(rdv.vt==DataType::floattype)
			fv=(float)rdv;
		else if(rdv.vt==DataType::stringtype)
			sv=(string)rdv;
		else
			return false;
        

	}catch(...)
	{
		return false;
	}
	return true;
}

SVAPI_API
bool GetRecordDisplayString(RECORD rd,int &state,string &dstr)
{
	if(rd==INVALID_VALUE)
		return false;
	try{
		Record *prd=reinterpret_cast< Record * >(rd);
		state=prd->GetState();
		if((state==Record::STATUS_NULL)||(state==Record::STATUS_BAD)||(state==Record::STATUS_DISABLE))
			dstr=prd->GetErrorDescription();
		else
			dstr=prd->GetDisplayStr();
	}catch(...)
	{
		return false;
	}


	return true;
}

SVAPI_API
void CloseRecordSet(RECORDSET &rset)
{
	if(rset==INVALID_VALUE)
		return ;
	try{
	     RecordSet *prs=reinterpret_cast<RecordSet *>(rset);
		 if(prs)
			 delete prs;
		 rset=INVALID_VALUE;
	}catch(...)
	{
		;
	}

}

///////////////////////////

SVAPI_API
bool GetAllTableNames(std::list<string> &namelist,string user,string addr)
{
	if(user.empty()||addr.empty())
		return false;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_GETTABLENAMES;
	querybuf.datatype=S_DB;
//	strcpy(querybuf.qstr,filename.c_str());
	querybuf.datalen=0;

//	strcpy(querybuf.idcuser,user.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;

	bool bret=false;

	if(qd.Query(&querybuf,(void **)&pdata,len,addr))
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
bool InsertTable(string monitorid,int monitortype,string user,string addr)
{
	if(monitorid.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_DB;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_INSERTTABLE;
	strcpy(query.qstr,monitorid.c_str());
	query.datalen=monitortype;

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
bool DeleteTable(string monitorid,string user,string addr)
{
	if(monitorid.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_DB;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_DELETETABLE;
	strcpy(query.qstr,monitorid.c_str());

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
		}
	}

	return false;
}

////////////////////////////

SVAPI_API
bool AppendRecord(string monitorid,const char *data,unsigned int len,string user,string addr)
{
	if(monitorid.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_DB;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_APPENDRECORD;
	strcpy(query.qstr,monitorid.c_str());
	query.datalen=len;

	QueryData qd;
	char *pdata=NULL;
	SVDBRESULT ret={0};

	return qd.Update(&query,data,len,&ret,addr);
}



SVAPI_API
bool AppendMassRecord(std::list<SingelRecord> & listrcd,string user,string addr)
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

	return qd.Update(&query,data,len,&ret,addr);
}


////////////////////////////

SVAPI_API
bool SetLogKeepDays(int days,string user,string addr)
{
	string rootpath=GetRootPath();
	if(rootpath.empty())
	{
		puts("Get root path failed");
		return false;
	}

	char configpath[1024]={0};
	sprintf(configpath,"%s/data/svdbconfig.ini",rootpath.c_str());
	string svalue;
	INIFile inif=LoadIni(configpath);

	char value[100]={0};
	sprintf(value,"%d",days);
	PutIniSetting(inif,"svdb","DataKeepDays",value);
	SaveIni(inif,configpath);
	return true;
}

SVAPI_API
bool GetLogKeepDays(int &days,string user,string addr)
{
	string rootpath=GetRootPath();
	if(rootpath.empty())
	{
		puts("Get root path failed");
		return false;
	}

	char configpath[1024]={0};
	sprintf(configpath,"%s/data/svdbconfig.ini",rootpath.c_str());

	string svalue;

	INIFile inif=LoadIni(configpath);

	svalue=GetIniSetting(inif,"svdb","DataKeepDays","");
	if(!svalue.empty())
		days=atoi(svalue.c_str());
	else
	{
		days=0;
		return false;
	}

	return true;
}
SVAPI_API
bool isDemoMode()
{
	string rootpath=GetRootPath();
	if(rootpath.empty())
	{
		puts("Get root path failed");
		return false;
	}

	char configpath[1024]={0};
	sprintf(configpath,"%s/fcgi-bin/mc.config",rootpath.c_str());

	string svalue;

	INIFile inif=LoadIni(configpath);

	svalue=GetIniSetting(inif,"MonitorSchedule","DemoMode","");
	if(!svalue.empty())
	{
		if(svalue.compare("true")==0)
			return true;
	}

	return false;
}
SVAPI_API
string GetMasterAddress()
{
	string rootpath=GetRootPath();
	if(rootpath.empty())
	{
		puts("Get root path failed");
		return "";
	}

	char configpath[1024]={0};
	sprintf(configpath,"%s/fcgi-bin/mc.config",rootpath.c_str());

	string svalue="localhost";

	INIFile inif=LoadIni(configpath);

	svalue=GetIniSetting(inif,"MonitorSchedule","ServerHost","");
	if(!svalue.empty())
	{
		return svalue;
	}

	return svalue;


}
