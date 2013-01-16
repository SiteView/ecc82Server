

#include "tableCache.h"

#include "../svdbtype.h"
#include "util.h"
#include "stlini.h"

#include "svclient.h"
#include "connection.h"
#include "somefunc.h"
#include "clientcomm.h"





map<CTableName, CSvTable * > SvTableCache::m_alldata;
Mutex SvTableCache::m_LockClientCache;

SvTableCache::SvTableCache(void)
{

}

SvTableCache::~SvTableCache(void)
{

}

bool SvTableCache::ClearAllCacheData()
{
	for(map<CTableName, CSvTable * >::iterator it= m_alldata.begin(); it!=m_alldata.end(); ++it)
		if(it->second)
			delete it->second;
	m_alldata.clear();
	return true;
}

CSvTable * SvTableCache::JustGetData(CTableName tname, string & estr)
{
	if(tname.isEmpty())
	{
		estr+= " CTableName from client is empty;   ";
		return false;
	}
	MapStrMap fmap;
	PutReturnMapStrMap(fmap,"inwhat",	"dowhat",		"GetDataFromServer");
	PutReturnMapStrMap(fmap,"inwhat",	"dbName",		tname.dbName);
	PutReturnMapStrMap(fmap,"inwhat",	"tableName",	tname.tableName);

	CSvTable * ptable= NULL;
	char * retp(NULL);
	int rlen(0); 
	CConnect con;
	if( con.ConnectDataBase(fmap,"", retp, rlen, estr) )
	{
		ptable= new CSvTable();
		if(ptable!=NULL)
		{
			CSvTableComm * pcomm=  reinterpret_cast<CSvTableComm*>(ptable);  
			if( !pcomm->CreateByRawData(retp,rlen) )
			{
				estr+= "   Failed to create return object;   ";
				delete ptable;
				ptable=NULL;
			}
		}
		else
			estr+= "  new an object,that==NULL ;  ";
	}
	else
		estr+= "   ConnectDataBase return false;   ";

	if(retp!=NULL)
		delete [] retp;

	return ptable;
}


CSvTable * SvTableCache::GetDataFromCache(CTableName tname, string & estr)
{
	map<CTableName, CSvTable * >::iterator it= m_alldata.find(tname);
	if( it!=m_alldata.end() )
		return it->second;

	CSvTable * ctable= JustGetData(tname, estr);
	if(ctable==NULL)
		return NULL;

	ost::MutexLock lock(m_LockClientCache);
	it= m_alldata.find(tname);
	if( it!=m_alldata.end() )
	{
		delete it->second;
		it->second= ctable;
	}
	else
		m_alldata.insert(std::make_pair(tname,ctable));

	return ctable;
}


CSvTable * SvTableCache::GetDataFromServer(CTableName tname, string & estr)
{
	map<CTableName, CSvTable * >::iterator it= m_alldata.find(tname);
	if( it!=m_alldata.end() )
	{
		if(it->second!=NULL)
		{
			string estr2;
			if(it->second->IsLatestVersion(estr2))
			{
				estr+= " Local table: "+tname.tableName+" is latest;   ";
				return it->second;
			}
		}
	}

	ost::MutexLock lock(m_LockClientCache);
	if( it!=m_alldata.end() )
	{
		if(it->second!=NULL)
			delete it->second;
		m_alldata.erase(it);
		estr+= " Re-get the latest table: "+tname.tableName+" from server ;   ";
	}

	CSvTable * ctable= JustGetData(tname, estr);
	if(ctable==NULL)
		return NULL;

	it= m_alldata.find(tname);
	if( it!=m_alldata.end() )
	{
		delete it->second;
		it->second= ctable;
	}
	else
		m_alldata.insert(std::make_pair(tname,ctable));

	return ctable;
}



bool SvTableCache::RefreshBatchData(const std::set<CTableName> & tnames, string & estr)
{
	if(tnames.empty())
	{
		estr+= " CTableNames from client is empty;   ";
		return false;
	}

	MapStrMap fmap;
	PutReturnMapStrMap(fmap,"inwhat",  "dowhat", "RefreshBatchData");
	map<CTableName, CSvTable * >::iterator ttit;
	for(std::set<CTableName>::const_iterator it= tnames.begin(); it!=tnames.end(); ++it)
	{
		string section= it->dbName+it->tableName;
		PutReturnMapStrMap(fmap, section, it->tableName, it->dbName);
		ttit= m_alldata.find(*it);
		if( ttit!=m_alldata.end() )
		{
			CSvTable * ctable= ttit->second;
			if(ctable!=NULL)
			{
				if(!ctable->IsModified())
					PutReturnMapStrMap(fmap, section, ctable->GetVersion(), section+"version");
			}
		}
	}

	char * retp(NULL);
	int rlen(0); 
	CConnect con;

	try{
		if( !con.ConnectDataBase(fmap,"", retp, rlen, estr) )
			estr+= "   ConnectDataBase return false;   ";

		map<CTableName, CSvTable * > tempalldata;
		std::list<SingelRecord> listrcd;
		if( CreateMassRecordListByRawData(listrcd, retp, rlen) )
		{
			for(std::list<SingelRecord>::iterator lit= listrcd.begin(); lit!= listrcd.end(); ++lit )
			{
				if(lit->data==NULL && lit->datalen==0)
				{
					try{
						string dbname= lit->monitorid;
						string::size_type pos= dbname.find(".");
						if(pos==string::npos)
							continue;

						CTableName tname(dbname.substr(0,pos),dbname.substr(pos+1,string::npos));
						tempalldata.insert(std::make_pair(tname,(CSvTable *)NULL));
						cout<<"  table to delete in client cache:";
						tname.Display();
					}
					catch(...)
					{
						cout<<" Exception in RefreshBatchData; "<<endl;
					}
					continue;
				}

				CSvTable * ptable= new CSvTable();
				if(ptable!=NULL)
				{
					CSvTableComm * pcomm=  reinterpret_cast<CSvTableComm*>(ptable);  
					if( pcomm->CreateByRawData(lit->data, lit->datalen) )
						tempalldata.insert(std::make_pair(ptable->GetTableName(), ptable));
					else
						delete ptable;
				}
			}
		}
		else
			estr+= "   Failed to create batch return object;   ";

		if(retp!=NULL)
		{
			delete [] retp;
			retp= NULL;
		}

		if(!tempalldata.empty())
		{
			ost::MutexLock lock(m_LockClientCache);
			map<CTableName, CSvTable * >::iterator it;
			for(map<CTableName, CSvTable * >::iterator tempit= tempalldata.begin(); tempit!=tempalldata.end(); ++tempit)
			{
				it= m_alldata.find(tempit->first);
				if( it!=m_alldata.end() )
				{
					if(tempit->second)
					{
						if( it->second )
							delete it->second;
						it->second= tempit->second;
					}
					else
					{
						if( it->second )
							delete it->second;
						m_alldata.erase(it);
					}
				}
				else
					m_alldata.insert(std::make_pair(tempit->first, tempit->second));
			}
			char tempchar[128]={0};
			sprintf(tempchar,"%d",tempalldata.size());
			string text= tempchar;
			estr+= "  Succeeded to get "+text+" tables ;  ";
		}
		else
			estr+= "   Didn't get any table in RefreshBatchData;  ";
	}
	catch(...)
	{
		if(retp!=NULL)
			delete [] retp;
		estr+= "   Exception in RefreshBatchData;  ";
	}
	return true;
}


bool SvTableCache::RefreshDB(string dbname, string & estr)
{
	if(dbname.empty())
	{
		estr+= "  Input database name is empty;   ";
		return false;
	}

	CSvDataBase cdb(dbname);
	set<CTableName> tnames;
	if( !cdb.GetAllTableName(tnames,estr) )
		return false;

	for(map<CTableName, CSvTable * >::iterator it= m_alldata.begin(); it!=m_alldata.end(); ++it)
	{
		if(it->first.dbName.compare(dbname)==0)
			tnames.insert(it->first);
	}

	if( tnames.empty() )
		return true;

	return RefreshBatchData(tnames,estr);
}


bool SvTableCache::SubmitData(CSvTable * ptable, string message, string & estr)
{
	if(ptable==NULL)
	{
		estr+= "  Your parameter ptable is NULL;  ";
		return false;
	}
	if(message.empty())
	{
		estr+= "  Your parameter message is empty;  ";
		return false;
	}
	if(!ptable->IsModified())
	{
		estr+= "  There isn't any modification of this table;  ";
		return true;
	}
	CSvTableComm * pcomm=  reinterpret_cast<CSvTableComm*>(ptable); 
	CTableName tname(ptable->GetTableName());

	MapStrMap fmap;
	PutReturnMapStrMap(fmap,"inwhat",	"dowhat",		"SubmitTable");
	PutReturnMapStrMap(fmap,"inwhat",	"dbName",		tname.dbName);
	PutReturnMapStrMap(fmap,"inwhat",	"tableName",	tname.tableName);
	PutReturnMapStrMap(fmap,"inwhat",	"message",		message);

	S_UINT dlen= pcomm->GetRawDataSize();
	char * data= new char[dlen];
	if(data==NULL)
	{
		estr+= " new an object (to SubmitTable), that==NULL;  ";
		return false;
	}
	char * tdata= pcomm->GetRawData(data,dlen);
	if(tdata==NULL)
	{
		estr+= " Failed to GetRawData to SubmitTable;  ";
		delete [] data;
		return false;
	}

	std::list<SingelRecord> listrcd;
	SingelRecord r1;
	r1.data= tdata;
	r1.datalen= dlen;
	r1.monitorid= tname.tableName;
	listrcd.push_back(r1);

	char * retp(NULL);
	int rlen(0); 
	CConnect con;
	bool bret= con.ConnectDataBaseCommit(fmap,listrcd,"", retp, rlen, estr);
	if(retp!=NULL)
		delete [] retp;	

	if(!bret)
		return false;

	pcomm->SetSubmitted();
	return true;
}

bool SvTableCache::SubmitAllCacheData(string message, string & estr)
{
	if(message.empty())
	{
		estr+= "  Your parameter message is empty;  ";
		return false;
	}

	MapStrMap fmap;
	PutReturnMapStrMap(fmap,"inwhat",	"dowhat",		"SubmitAllCacheData");
	PutReturnMapStrMap(fmap,"inwhat",	"message",		message);

	std::list<SingelRecord> listrcd;
	for(map<CTableName, CSvTable * >::iterator it= m_alldata.begin(); it!=m_alldata.end(); ++it)
	{
		CSvTable * ptable= it->second;
		if(ptable==NULL)
			continue;
		if(!ptable->IsModified())
			continue;

		CTableName tname(ptable->GetTableName());
		if(tname.isEmpty())
		{
			tname.Display();
			continue;
		}

		CSvTableComm * pcomm=  reinterpret_cast<CSvTableComm*>(ptable); 
		S_UINT dlen= pcomm->GetRawDataSize();
		char * data= new char[dlen];
		if(data==NULL)
		{
			estr+= " new an object (to SubmitTable), that==NULL;  ";
			continue;
		}
		char * tdata= pcomm->GetRawData(data,dlen);
		if(tdata==NULL)
		{
			estr+= " Failed to GetRawData to SubmitTable;  ";
			delete [] data;
			continue;
		}

		SingelRecord r1;
		r1.data= tdata;
		r1.datalen= dlen;
		r1.monitorid= tname.tableName;
		listrcd.push_back(r1);
	}
	if(listrcd.empty())
	{
		estr+= "  There isn't any table modified;  ";
		return true;
	}
	else
	{
		char tempchar[128]={0};
		sprintf(tempchar,"%d",listrcd.size());
		string text= tempchar;
		estr+= "  Submit "+text+" tables to server;  ";		
	}

	char * retp(NULL);
	int rlen(0); 
	CConnect con;
	bool bret= con.ConnectDataBaseCommit(fmap,listrcd,"", retp, rlen, estr);

	if(retp!=NULL)
		delete [] retp;	

	if(!bret)
		return false;

	for(map<CTableName, CSvTable * >::iterator it= m_alldata.begin(); it!=m_alldata.end(); ++it)
	{
		CSvTable * ptable= it->second;
		if(ptable==NULL)
			continue;
		CSvTableComm * pcomm=  reinterpret_cast<CSvTableComm*>(ptable); 
		pcomm->SetSubmitted();
	}
	return true;
}




