
#include "ObjCache.h"
#include "QueryData.h"
#include "util.h"
#include <Monitor.h>
#include <Entity.h>

string ObjectCache::svdbuser("default");
string ObjectCache::svdbaddr("localhost");

map<int, OBJECT>	ObjectCache::MonitorTPL;
map<string, OBJECT> ObjectCache::EntityTPL;
map<string, OBJECT> ObjectCache::EntityGroup;

map<string, OBJECT> ObjectCache::Res;
map<string,map<string, OBJECT> > ObjectCache::Reskey;

map<string, VerOBJECT> ObjectCache::Svses;
map<string, VerOBJECT> ObjectCache::Monitors;
map<string, VerOBJECT> ObjectCache::Entities;
map<string, VerOBJECT> ObjectCache::Groups;

map<string, SingelRecord> ObjectCache::Dyns;

Mutex ObjectCache::m_LockMonitorTPL;
Mutex ObjectCache::m_LockEntityTPL;
Mutex ObjectCache::m_LockEntityGroup;
Mutex ObjectCache::m_LockRes;
Mutex ObjectCache::m_LockReskey;
Mutex ObjectCache::m_LockSvses;
Mutex ObjectCache::m_LockMonitors;
Mutex ObjectCache::m_LockEntities;
Mutex ObjectCache::m_LockGroups;
Mutex ObjectCache::m_LockDyns;


OBJECT ObjectCache::FastGetSVSE(string id)
{
	map<string, VerOBJECT>::iterator it= Svses.find(id);
	if( it != Svses.end() )
	{
		if( (it->second).obj != INVALID_VALUE )
		{
			if( CheckLatest(it->second,S_SVSE,id) ) 
				return (it->second).obj;
		}
	}
	
	ost::MutexLock lock(m_LockSvses);
	if( it != Svses.end() )
	{
		if( (it->second).obj!= INVALID_VALUE )
			CloseSVSE( (it->second).obj );
		Svses.erase(it);
	}

	S_UINT ver;
	OBJECT obj= GetSVSEWithVer( id, svdbuser, svdbaddr, ver );
	if( obj==INVALID_VALUE )
		return INVALID_VALUE;
	
	char charver[256];
	sprintf(charver,"%d",ver);

	VerOBJECT vobj;
	vobj.ver=charver;
	vobj.obj=obj;

	Svses.insert(make_pair(id, vobj));
	return obj;
}


OBJECT ObjectCache::RefreshFastGetMonitor(string id)
{
	map<string, VerOBJECT>::iterator it= Monitors.find(id);
	if( it != Monitors.end() )
	{
		if( (it->second).obj != INVALID_VALUE )
			return (it->second).obj;
	}
	
	ost::MutexLock lock(m_LockMonitors);
	S_UINT ver;
	OBJECT obj= GetMonitorWithVer( id, svdbuser, svdbaddr, ver );
	if( obj==INVALID_VALUE )
		return INVALID_VALUE;
	
	char charver[256];
	sprintf(charver,"%d",ver);

	VerOBJECT vobj;
	vobj.ver=charver;
	vobj.obj=obj;

	Monitors.insert(make_pair(id, vobj));
	return obj;
}


OBJECT ObjectCache::FastGetMonitor(string id)
{
	map<string, VerOBJECT>::iterator it= Monitors.find(id);
	if( it != Monitors.end() )
	{
		if( (it->second).obj != INVALID_VALUE )
		{
			if( CheckLatest(it->second,S_MONITOR,id) ) 
				return (it->second).obj;
		}
	}
	
	ost::MutexLock lock(m_LockMonitors);
	if( it != Monitors.end() )
	{
		if( (it->second).obj!= INVALID_VALUE )
			CloseMonitor( (it->second).obj );
		Monitors.erase(it);
	}

	S_UINT ver;
	OBJECT obj= GetMonitorWithVer( id, svdbuser, svdbaddr, ver );
	if( obj==INVALID_VALUE )
		return INVALID_VALUE;
	
	char charver[256];
	sprintf(charver,"%d",ver);

	VerOBJECT vobj;
	vobj.ver=charver;
	vobj.obj=obj;

	Monitors.insert(make_pair(id, vobj));
	return obj;
}

OBJECT ObjectCache::FastGetGroup(string id)
{
	map<string, VerOBJECT>::iterator it= Groups.find(id);
	if( it != Groups.end() )
	{
		if( (it->second).obj != INVALID_VALUE )
		{
			if( CheckLatest(it->second,S_GROUP,id) ) 
				return (it->second).obj;
		}
	}
	
	ost::MutexLock lock(m_LockGroups);
	if( it != Groups.end() )
	{
		if( (it->second).obj!= INVALID_VALUE )
			CloseGroup( (it->second).obj );
		Groups.erase(it);
	}

	S_UINT ver;
	OBJECT obj= GetGroupWithVer( id, svdbuser, svdbaddr, ver );
	if( obj==INVALID_VALUE )
		return INVALID_VALUE;
	
	char charver[256];
	sprintf(charver,"%d",ver);

	VerOBJECT vobj;
	vobj.ver=charver;
	vobj.obj=obj;

	Groups.insert(make_pair(id, vobj));
	return obj;
}

OBJECT ObjectCache::RefreshFastGetEntity(string id)
{
	map<string, VerOBJECT>::iterator it= Entities.find(id);
	if( it != Entities.end() )
	{
		if( (it->second).obj != INVALID_VALUE )
			return (it->second).obj;
	}

	ost::MutexLock lock(m_LockEntities);
	S_UINT ver;
	OBJECT obj= GetEntityWithVer( id, svdbuser, svdbaddr, ver );
	if( obj==INVALID_VALUE )
		return INVALID_VALUE;
	
	char charver[256];
	sprintf(charver,"%d",ver);

	VerOBJECT vobj;
	vobj.ver=charver;
	vobj.obj=obj;

	Entities.insert(make_pair(id, vobj));
	return obj;
}

OBJECT ObjectCache::FastGetEntity(string id)
{
	map<string, VerOBJECT>::iterator it= Entities.find(id);
	if( it != Entities.end() )
	{
		if( (it->second).obj != INVALID_VALUE )
		{
			if( CheckLatest(it->second,S_ENTITY,id) ) 
				return (it->second).obj;
		}
	}
	
	ost::MutexLock lock(m_LockEntities);
	if( it != Entities.end() )
	{
		if( (it->second).obj!= INVALID_VALUE )
			CloseEntity( (it->second).obj );
		Entities.erase(it);
	}

	S_UINT ver;
	OBJECT obj= GetEntityWithVer( id, svdbuser, svdbaddr, ver );
	if( obj==INVALID_VALUE )
		return INVALID_VALUE;
	
	char charver[256];
	sprintf(charver,"%d",ver);

	VerOBJECT vobj;
	vobj.ver=charver;
	vobj.obj=obj;

	Entities.insert(make_pair(id, vobj));
	return obj;
}



OBJECT ObjectCache::FastGetMonitorTPL(int id)
{
	map<int,OBJECT>::iterator it= MonitorTPL.find(id);
	if( it != MonitorTPL.end() )
		return it->second;
	
	OBJECT obj= GetMonitorTemplet( id, svdbuser, svdbaddr );
	if( obj==INVALID_VALUE )
		return INVALID_VALUE;
	
	ost::MutexLock lock(m_LockMonitorTPL);
	MonitorTPL.insert(make_pair(id, obj));
	return obj;
}

OBJECT ObjectCache::FastGetEntityTPL(string id)
{
	map<string, OBJECT>::iterator it= EntityTPL.find(id);
	if( it != EntityTPL.end() )
		return it->second;
	
	OBJECT obj= GetEntityTemplet( id, svdbuser, svdbaddr );
	if( obj==INVALID_VALUE )
		return INVALID_VALUE;
	
	ost::MutexLock lock(m_LockEntityTPL);
	EntityTPL.insert(make_pair(id, obj));
	return obj;
}

OBJECT ObjectCache::FastGetEntityGroup(string id)
{
	map<string, OBJECT>::iterator it= EntityGroup.find(id);
	if( it != EntityGroup.end() )
		return it->second;
	
	OBJECT obj= GetEntityGroup( id, svdbuser, svdbaddr );
	if( obj==INVALID_VALUE )
		return INVALID_VALUE;
	
	ost::MutexLock lock(m_LockEntityGroup);
	EntityGroup.insert(make_pair(id, obj));
	return obj;
}

OBJECT ObjectCache::FastLoadResource(string lan)
{
	map<string, OBJECT>::iterator it= Res.find(lan);
	if( it != Res.end() )
		return it->second;
	
	OBJECT obj= LoadResource( lan, svdbaddr );
	if( obj==INVALID_VALUE )
		return INVALID_VALUE;
	
	ost::MutexLock lock(m_LockRes);
	Res.insert(make_pair(lan, obj));
	return obj;
}

OBJECT ObjectCache::FastLoadResourceByKeys(string keys,string lan)
{
	map<string,map<string, OBJECT> >::iterator mit= Reskey.find(lan);
	map<string,OBJECT>::iterator it;
	if( mit != Reskey.end() )
	{
		it= (mit->second).find(keys);
		if( it!=(mit->second).end() )
			return it->second;
	}

	OBJECT obj= LoadResourceByKeys( keys, lan, svdbaddr );
	if( obj==INVALID_VALUE )
		return INVALID_VALUE;
	
	ost::MutexLock lock(m_LockReskey);
	if( mit == Reskey.end() )
	{
		map<string, OBJECT> newkeys;
		Reskey.insert(make_pair(lan,newkeys));
		mit= Reskey.find(lan);
		if( mit == Reskey.end() )
			return obj;
	}
	(mit->second).insert(make_pair(keys, obj));
	return obj;
}


bool ObjectCache::RefreshMonitors(string pid)
{
	if(Monitors.empty())
		return true;

	PAIRLIST retlist; PAIRLIST::iterator lit;
	if( !GetAllMonitorsInfo(retlist,"ObjectVersion",pid,svdbaddr) )
		return false;

	map<string, VerOBJECT>::iterator it;
	for( lit=retlist.begin();lit!=retlist.end();lit++)
	{
		it= Monitors.find(lit->name);
		if( it != Monitors.end() )
		{
			if( (it->second).ver.compare(lit->value) != 0 )
			{
				if( (it->second).obj!= INVALID_VALUE )
					CloseMonitor( (it->second).obj );
				ost::MutexLock lock(m_LockMonitors);
				Monitors.erase(it);
			}
		}
	}
    return true;
}

bool ObjectCache::RefreshEntities(string pid)
{
	if(Entities.empty())
		return true;

	PAIRLIST retlist; PAIRLIST::iterator lit;
	if( !GetAllEntitysInfo(retlist,"ObjectVersion",pid,svdbaddr) )
		return false;

	map<string, VerOBJECT>::iterator it;
	for( lit=retlist.begin();lit!=retlist.end();lit++)
	{
		it= Entities.find(lit->name);
		if( it != Entities.end() )
		{
			if( (it->second).ver.compare(lit->value) != 0 )
			{
				if( (it->second).obj!= INVALID_VALUE )
					CloseEntity( (it->second).obj );
				ost::MutexLock lock(m_LockEntities);
				Entities.erase(it);
			}
		}
	}
    return true;
}

void ObjectCache::SetUserAddr(string user,string addr)
{
	svdbuser= user;
	svdbaddr= addr;
	
	for(map<int,OBJECT>::iterator it= MonitorTPL.begin(); it!=MonitorTPL.end(); ++it)
		CloseMonitorTemplet( it->second );
	MonitorTPL.clear();

	for(map<string,OBJECT>::iterator it= EntityTPL.begin(); it!=EntityTPL.end(); ++it)
		CloseEntityTemplet( it->second );
	EntityTPL.clear();

	for(map<string,OBJECT>::iterator it= EntityGroup.begin(); it!=EntityGroup.end(); ++it)
		CloseEntityGroup( it->second );
	EntityGroup.clear();

	for(map<string,OBJECT>::iterator it= Res.begin(); it!=Res.end(); ++it)
		CloseResource( it->second );
	Res.clear();

	for(map<string,map<string, OBJECT> >::iterator mit= Reskey.begin(); mit!=Reskey.end(); ++mit)
	{
		for(map<string,OBJECT>::iterator it= (mit->second).begin(); it!= (mit->second).end(); ++it)
			CloseResource( it->second );
		mit->second.clear();
	}
	Reskey.clear();

	for(map<string,VerOBJECT>::iterator it= Monitors.begin(); it!=Monitors.end(); ++it)
		CloseMonitor( (it->second).obj );
	Monitors.clear();

	for(map<string,VerOBJECT>::iterator it= Entities.begin(); it!=Entities.end(); ++it)
		CloseEntity( (it->second).obj );
	Entities.clear();

	for(map<string,VerOBJECT>::iterator it= Groups.begin(); it!=Groups.end(); ++it)
		CloseGroup( (it->second).obj );
	Groups.clear();

	for(map<string,VerOBJECT>::iterator it= Svses.begin(); it!=Svses.end(); ++it)
		CloseSVSE( (it->second).obj );
	Svses.clear();

	for(map<string, SingelRecord>::iterator it= Dyns.begin(); it!=Dyns.end(); ++it)
	{
		if( it->second.data != NULL )
			delete [] (it->second.data);
	}
	Dyns.clear();

}

bool ObjectCache::CheckLatest(VerOBJECT & vobj, int dtype, string id)
{
	if(vobj.obj==INVALID_VALUE)
		return false;
	if(id.size()>=MAXQUEREYSTRINGLEN)
		return false;
	if(vobj.ver.size()>=MAXUSERLEN)
		return false;

	try{
		SVDBQUERY querybuf={0};
		querybuf.len = sizeof(SVDBQUERY);
		querybuf.querytype=QUERY_LATEST;
		querybuf.datatype= dtype;
		querybuf.datalen=0;
		strcpy(querybuf.idcuser,	vobj.ver.c_str());
		strcpy(querybuf.qstr,		id.c_str());

		QueryData qd;
		S_UINT len=0;
		char *pdata=NULL;
		if(qd.Query(&querybuf,(void **)&pdata,len,svdbaddr))
		{
			if(pdata)
			{
				if(len>0)
				{
					int *pret=(int*)pdata;
					if(*pret==SVDB_OK)
					{
						delete [] pdata;
						return true;
					}
				}
				delete [] pdata;
			}
		}
	}catch(...)
	{
		return false;
	}

	return false;
}


bool ObjectCache::FastGetSVDYN(string id,SVDYN &dyn)
{
	map<string, SingelRecord>::iterator it= Dyns.find(id);
	if( it!=Dyns.end() && (it->second.data)!=NULL )
	{
		bool bret(false);
		try{
			bret=builddyn(it->second.data,it->second.datalen,dyn,true);
			if(bret)
				return true;
		}catch(...)
		{
			printf("Exception to builddyn in Cache_GetSVDYN.");
		}
	}
	return false;
}


bool ObjectCache::QuerySVDYNs( string pid, std::list<SingelRecord> & listrcd, std::list<SingelRecord> & listrcd_out )
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


	if(qd.Query(&querybuf,buf,len,(void **)&pdata,rlen,svdbaddr))
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
					char * tempchar= new char[lit->datalen];
					memmove(tempchar,lit->data,lit->datalen);
					rcd.data= tempchar;

					listrcd_out.push_back(rcd);
				}
				return true;
			}
		}catch(...)
		{
			printf("Exception to QuerySVDYNs in RefreshSVDYNs.");
		}
	}
	if(pdata!=NULL)
		delete [] pdata;
	return false;
}


bool ObjectCache::RefreshSVDYNs(string pid)
{
	if(pid.empty())
		return false;
	if(pid.compare("default")==0)
		return false;
	if(pid.size()>MAXQUEREYSTRINGLEN)
		return false;

	std::list<SingelRecord> listrcd;
	map<string, SingelRecord>::iterator mit;

	for(mit=Dyns.begin(); mit!=Dyns.end(); ++mit)
	{
		if( (mit->first).find(pid+".")!=0 )
			continue;
		SingelRecord rcd;
		rcd.monitorid= mit->first;
		rcd.data= mit->second.data;
		rcd.datalen= sizeof(svutil::TTime);

		listrcd.push_back(rcd);
	}

	std::list<SingelRecord> listrcd_out;
	if( !QuerySVDYNs(pid,listrcd,listrcd_out) )
		return false;
	if(listrcd_out.empty())
		return true;

	ost::MutexLock lock(m_LockDyns);
	std::list<SingelRecord>::iterator lit;
	for(lit=listrcd_out.begin(); lit!=listrcd_out.end(); ++lit)
	{
		mit= Dyns.find(lit->monitorid);
		if(mit==Dyns.end())
		{
			Dyns.insert(std::make_pair(lit->monitorid, *lit));
			continue;
		}

		if( mit->second.data != NULL )
		{
			delete [] (mit->second.data);
			mit->second.data =NULL;
		}
		
		mit->second= *lit;
	}
	return true;
}

bool ObjectCache::QueryMassMonitor( string pid, StringMap & smap, std::list<SingelRecord> & listrcd_out )
{
	unsigned int tlen= smap.GetRawDataSize();
	svutil::buffer tbuf;
	if(!tbuf.checksize(tlen))
		return false;

	const char *data= smap.GetRawData(tbuf, tlen); 
	if(data==NULL)
		return false;

	QueryData qd;
	char *pdata=NULL;
	S_UINT rlen=0;

	S_UINT len=0;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_MASSMONITOR;
	querybuf.datatype=S_MONITOR;
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


	if(qd.Query(&querybuf,buf,len,(void **)&pdata,rlen,svdbaddr))
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
					char * tempchar= new char[lit->datalen];
					memmove(tempchar,lit->data,lit->datalen);
					rcd.data= tempchar;

					listrcd_out.push_back(rcd);
				}
				return true;
			}
		}catch(...)
		{
			printf("Exception to QueryMassMonitor in RefreshMonitors_new.");
		}
	}
	if(pdata!=NULL)
		delete [] pdata;
	return false;
}

bool ObjectCache::RefreshMonitors_new(string pid)
{
	if(pid.empty())
		return false;
	if(pid.size()>MAXQUEREYSTRINGLEN)
		return false;

	bool isdefault(false);
	if(pid.compare("default")==0)
		isdefault=true;

	StringMap smap(577);
	map<string, VerOBJECT>::iterator mit;
	for(mit=Monitors.begin(); mit!=Monitors.end(); ++mit)
	{
		if( !isdefault && (mit->first).find(pid+".")!=0)
			continue;
		smap[mit->first.c_str()]= mit->second.ver.c_str();
	}

	std::list<SingelRecord> listrcd_out;
	if( !QueryMassMonitor(pid,smap,listrcd_out) )
		return false;
	if(listrcd_out.empty())
		return true;

	ost::MutexLock lock(m_LockMonitors);
	std::list<SingelRecord>::iterator lit;
	for(lit=listrcd_out.begin(); lit!=listrcd_out.end(); ++lit)
	{
		if(lit->data == NULL)
			continue;

		S_UINT ver(0);
		OBJECT ret=INVALID_VALUE;
		Monitor *pm=new Monitor();
		if(pm)
		{
			const char * pdata= lit->data;
			try{
				if(pm->CreateObjectByRawData(pdata+sizeof(S_UINT),(lit->datalen)-sizeof(S_UINT)))
					ret=reinterpret_cast<OBJECT>(pm);
				memmove(&ver,pdata,sizeof(S_UINT));
			}catch(...)
			{
				ret=INVALID_VALUE;
			}
		}
		delete [] (lit->data);
		if(ret==INVALID_VALUE)
		{
			if(pm)
				delete pm;
			continue;
		}

		char charver[256];
		sprintf(charver,"%d",ver);

		mit= Monitors.find(lit->monitorid);
		if(mit==Monitors.end())
		{
			VerOBJECT vobj;
			vobj.ver=charver;
			vobj.obj=ret;

			Monitors.insert(make_pair(lit->monitorid, vobj));
			continue;
		}

		if( mit->second.obj != INVALID_VALUE )
			CloseMonitor(mit->second.obj);

		mit->second.ver= charver;
		mit->second.obj= ret;
	}
	return true;
}


bool ObjectCache::QueryMassEntity( string pid, StringMap & smap, std::list<SingelRecord> & listrcd_out )
{
	unsigned int tlen= smap.GetRawDataSize();
	svutil::buffer tbuf;
	if(!tbuf.checksize(tlen))
		return false;

	const char *data= smap.GetRawData(tbuf, tlen); 
	if(data==NULL)
		return false;

	QueryData qd;
	char *pdata=NULL;
	S_UINT rlen=0;

	S_UINT len=0;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_MASSENTITY;
	querybuf.datatype=S_ENTITY;
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


	if(qd.Query(&querybuf,buf,len,(void **)&pdata,rlen,svdbaddr))
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
					char * tempchar= new char[lit->datalen];
					memmove(tempchar,lit->data,lit->datalen);
					rcd.data= tempchar;

					listrcd_out.push_back(rcd);
				}
				return true;
			}
		}catch(...)
		{
			printf("Exception to QueryMassEntity in RefreshEntities_new.");
		}
	}
	if(pdata!=NULL)
		delete [] pdata;
	return false;
}

bool ObjectCache::RefreshEntities_new(string pid)
{
	if(pid.empty())
		return false;
	if(pid.size()>MAXQUEREYSTRINGLEN)
		return false;

	bool isdefault(false);
	if(pid.compare("default")==0)
		isdefault=true;

	StringMap smap(577);
	map<string, VerOBJECT>::iterator mit;
	for(mit=Entities.begin(); mit!=Entities.end(); ++mit)
	{
		if( !isdefault && (mit->first).find(pid+".")!=0)
			continue;
		smap[mit->first.c_str()]= mit->second.ver.c_str();
	}

	std::list<SingelRecord> listrcd_out;
	if( !QueryMassEntity(pid,smap,listrcd_out) )
		return false;
	if(listrcd_out.empty())
		return true;

	ost::MutexLock lock(m_LockEntities);
	std::list<SingelRecord>::iterator lit;
	for(lit=listrcd_out.begin(); lit!=listrcd_out.end(); ++lit)
	{
		if(lit->data == NULL)
			continue;

		S_UINT ver(0);
		OBJECT ret=INVALID_VALUE;
		Entity *pm=new Entity();
		if(pm)
		{
			const char * pdata= lit->data;
			try{
				if(pm->CreateObjectByRawData(pdata+sizeof(S_UINT),(lit->datalen)-sizeof(S_UINT)))
					ret=reinterpret_cast<OBJECT>(pm);
				memmove(&ver,pdata,sizeof(S_UINT));
			}catch(...)
			{
				ret=INVALID_VALUE;
			}
		}
		delete [] (lit->data);
		if(ret==INVALID_VALUE)
		{
			if(pm)
				delete pm;
			continue;
		}

		char charver[256];
		sprintf(charver,"%d",ver);

		mit= Entities.find(lit->monitorid);
		if(mit==Entities.end())
		{
			VerOBJECT vobj;
			vobj.ver=charver;
			vobj.obj=ret;

			Entities.insert(make_pair(lit->monitorid, vobj));
			continue;
		}

		if( mit->second.obj != INVALID_VALUE )
			CloseEntity(mit->second.obj);

		mit->second.ver= charver;
		mit->second.obj= ret;
	}
	return true;
}

SVAPI_API
void SetCacheUserAddr(string user,string addr)
{
	ObjectCache::SetUserAddr( user,addr);
}

SVAPI_API
bool  CacheRefreshMonitors(string parentid)
{
	return ObjectCache::RefreshMonitors_new( parentid );
}

SVAPI_API
bool  CacheRefreshEntities(string parentid)
{
	return ObjectCache::RefreshEntities_new( parentid );
}

SVAPI_API 
bool Cache_GetSVDYN(string monitorid,SVDYN &dyn)
{
	return ObjectCache::FastGetSVDYN( monitorid, dyn );
}

SVAPI_API
bool	CacheRefreshSVDYNs(string parentid)
{
	return ObjectCache::RefreshSVDYNs(parentid);
}



