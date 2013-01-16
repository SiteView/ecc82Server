#include "svapi.h"
#include <StringMap.h>
#include <Entity.h>
#include "QueryData.h"
#include "ObjCache.h"


SVAPI_API
OBJECT	CacheRefresh_GetEntity(string entityid)
{
	return ObjectCache::RefreshFastGetEntity(entityid);
}

SVAPI_API
OBJECT	Cache_GetEntity(string entityid)
{
	return ObjectCache::FastGetEntity(entityid);
}

OBJECT	GetEntityWithVer(string entityid,string user,string addr,S_UINT & ver)
{
	if(entityid.empty()||user.empty()||addr.empty())
		return INVALID_VALUE;
	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_ENTITY;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_GET;
	strcpy(query.qstr,entityid.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;

	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		Entity *pe=new Entity();
		if(pe)
		{
			OBJECT ret=INVALID_VALUE;
			try{
				if(pe->CreateObjectByRawData(pdata+sizeof(S_UINT),len-sizeof(S_UINT)))
					ret=reinterpret_cast<OBJECT>(pe);
				memmove(&ver,pdata,sizeof(S_UINT));
			}catch(...)
			{
				if(pdata!=NULL)
					delete [] pdata;
				return INVALID_VALUE;
			}
			if(pdata!=NULL)
				delete [] pdata;
			return ret;

		}
	}
	if(pdata!=NULL)
		delete [] pdata;
	return INVALID_VALUE;

}

SVAPI_API
OBJECT	GetEntity(string entityid,string user,string addr)
{
	if(entityid.empty()||user.empty()||addr.empty())
		return INVALID_VALUE;
	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;

	S_UINT ver(0);
	return GetEntityWithVer(entityid,user,addr,ver);
}

SVAPI_API 
bool GetAllEntitysInfo(PAIRLIST &retlist,string infoname,string user,string addr)
{
	if(user.empty()||addr.empty())
		return false;

	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;


	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_INFO;
	querybuf.datatype=S_ENTITY;
	strcpy(querybuf.qstr,infoname.c_str());
	strcpy(querybuf.idcuser,user.c_str());

    QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;
	if(qd.Query(&querybuf,(void **)&pdata,len,addr))
	{
		StringMap *pmtp=new StringMap();
		if(pmtp)
		{
			MAPNODE ret=INVALID_VALUE;
			if(pdata!=NULL)
			{
				try{
					if(pmtp->CreateObjectByRawData(pdata,len))
					{
	     				delete [] pdata;
						ret=reinterpret_cast<MAPNODE>(pmtp);
						bool bret=::EnumNodeAttrib(ret,retlist);
						delete pmtp;
						return bret;
					}
				}catch(...)
				{
					return false;
				}
			    delete [] pdata;
			}
			delete pmtp;
		}
	}
	
	return false;
}


SVAPI_API
OBJECT CreateEntity()
{
	try{
		Entity *pe=new Entity();
		if(pe==NULL)
			return INVALID_VALUE;
		OBJECT ret=reinterpret_cast<OBJECT>(pe);
		return ret;
	}catch(...)
	{
		return INVALID_VALUE;
	}

	return INVALID_VALUE;

}

SVAPI_API
MAPNODE GetEntityMainAttribNode(OBJECT entityobj)
{
	if(entityobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{
		Entity *pe=reinterpret_cast<Entity *>(entityobj);
		MAPNODE ret=reinterpret_cast<MAPNODE>(&(pe->GetProperty()));
		return ret;

	}catch(...)
	{
		return INVALID_VALUE;
	}

	return INVALID_VALUE;

}

SVAPI_API
bool GetSubMonitorsIDByEntity(OBJECT entityobj,std::list<string> &monitoridlist)
{
	if(entityobj==INVALID_VALUE)
		return false;
	try{
		Entity *pe=reinterpret_cast<Entity *>(entityobj);
		WORDLIST &monitors=pe->GetMonitors();
		WORDLIST::iterator it;
		for(it=monitors.begin();it!=monitors.end();it++)
		{
			monitoridlist.push_back((*it).getword());
		}

	}catch(...)
	{
		return false;
	}

	return true;


}
/*
SVAPI_API
string GetEntityNextMonitorID(OBJECT entityobj,string user,string addr)
{
	if(entityobj==INVALID_VALUE)
		return "";
	if(user.empty()||addr.empty())
		return "";

	try{
		Entity *pe=reinterpret_cast<Entity *>(entityobj);
		svutil::word id=pe->GetNextMonitorID();
		if(SubmitEntity(entityobj,user,addr))
		{
			return id.getword();
		}
		
	}catch(...)
	{
		return "";
	}
	return "";
    
}
*/

SVAPI_API
bool DeleteEntity(string entityid,string user,string addr)
{
	if(entityid.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_DELETE;
	querybuf.datatype=S_ENTITY;
	strcpy(querybuf.qstr,entityid.c_str());
	strcpy(querybuf.idcuser,user.c_str());

    QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;
	if(qd.Query(&querybuf,(void **)&pdata,len,addr))
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

	return false;

}
SVAPI_API
string AddNewEntity(OBJECT entityobj,string groupid,string user,string addr)
{
	if(entityobj==INVALID_VALUE)
		return "";
	if(user.empty()||addr.empty()||groupid.empty())
		return "";
	try{
		Entity *pmtp=reinterpret_cast<Entity *>(entityobj);
		S_UINT len=pmtp->GetRawDataSize();
		svutil::buffer buf;
		if(!buf.checksize(len))
			return "";
		if(pmtp->GetRawData(buf,len)==NULL)
			return "";

		SVDBQUERY querybuf={0};
		querybuf.len = sizeof(SVDBQUERY);
		querybuf.querytype=QUERY_ADDNEW;
		querybuf.datatype=S_ENTITY;
		strcpy(querybuf.qstr,groupid.c_str());
		querybuf.datalen=len;
		strcpy(querybuf.idcuser,user.c_str());

		QueryData qd;
		SVDBRESULT ret={0};
	   if(!qd.Update(&querybuf,buf,len,&ret,addr))
		   return "";
	   return ret.info;
	}catch(...)
	{
		return "";
	}


	return "";

}


SVAPI_API
bool CloseEntity(OBJECT &entityobj)
{
	if(entityobj==INVALID_VALUE)
		return false;
	try{
		Entity *pe=reinterpret_cast<Entity *>(entityobj);
		if(pe)
			delete pe;
		entityobj=INVALID_VALUE;
	}catch(...)
	{
		return false;
	}

	return true;

}

SVAPI_API
bool SubmitEntity(OBJECT entityobj,string user,string addr)
{
	if(entityobj==INVALID_VALUE)
		return false;
	if(user.empty()||addr.empty())
		return false;
	try{
		Entity *pmtp=reinterpret_cast<Entity *>(entityobj);
		S_UINT len=pmtp->GetRawDataSize();
		svutil::buffer buf;
		if(!buf.checksize(len))
			return false;
		if(pmtp->GetRawData(buf,len)==NULL)
			return false;

		SVDBQUERY querybuf={0};
		querybuf.len = sizeof(SVDBQUERY);
		querybuf.querytype=QUERY_UPDATE;
		querybuf.datatype=S_ENTITY;
		strcpy(querybuf.qstr,pmtp->GetID().getword());
		querybuf.datalen=len;
		strcpy(querybuf.idcuser,user.c_str());

		QueryData qd;
		SVDBRESULT ret={0};
		return qd.Update(&querybuf,buf,len,&ret,addr);
	}catch(...)
	{
		return false;
	}


	return true;


}

string EntityCopyGeneral(bool bCreatTable,string srcentityid,string objgroupid,string user,string addr)
{
	if(srcentityid.empty()||objgroupid.empty()||user.empty()||addr.empty())
		return "";

	OBJECT entityobj= GetEntity(srcentityid,user,addr);
	if(entityobj==INVALID_VALUE)
		return "";

	std::list<string> mlist;

	try{
		Entity *pmtp=reinterpret_cast<Entity *>(entityobj);
		WORDLIST &monitors=pmtp->GetMonitors();
		WORDLIST::iterator it;
		for(it=monitors.begin();it!=monitors.end();it++)
			mlist.push_back((*it).getword());

		monitors.clear();

		pmtp->AdjustIndex(0-pmtp->GetCurrentIndex());

		string eid=AddNewEntity(entityobj,objgroupid,user,addr);
		CloseEntity(entityobj);
		if(eid.empty())
		{
			return "";
		}

		std::list<string>::iterator lit;
		for(lit=mlist.begin();lit!=mlist.end();lit++)
		{
			if( bCreatTable )
				MonitorCopyAndCreateTable((*lit),eid,user,addr);
			else
				MonitorCopy((*lit),eid,user,addr);
		}

		return eid;
		

	}catch(...)
	{
		return "";
	}

	return "";
}


SVAPI_API
string EntityCopyAndCreateTable(string srcentityid,string objgroupid,string user,string addr)
{
	return EntityCopyGeneral(true,srcentityid,objgroupid,user,addr);
}

SVAPI_API
string EntityCopy(string srcentityid,string objgroupid,string user,string addr)
{
	return EntityCopyGeneral(false,srcentityid,objgroupid,user,addr);
}


