#include "svapi.h"
#include <StringMap.h>
#include <EntityGroup.h>
#include "QueryData.h"
#include "ObjCache.h"

SVAPI_API
OBJECT	Cache_GetEntityGroup(string entitygroupid)
{
	return ObjectCache::FastGetEntityGroup(entitygroupid);
}

SVAPI_API
OBJECT GetEntityGroup(string entitygroupid,string user,string addr)
{
	if(entitygroupid.empty()||user.empty()||addr.empty())
		return INVALID_VALUE;
	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_ENTITYGROUP;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_GET;
	strcpy(query.qstr,entitygroupid.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;

	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		EntityGroup *pe=new EntityGroup();
		if(pe)
		{
			OBJECT ret=INVALID_VALUE;
			try{
				if(pe->CreateObjectByRawData(pdata,len))
					ret=reinterpret_cast<OBJECT>(pe);
				delete [] pdata;
			}catch(...)
			{
				return INVALID_VALUE;
			}
			return ret;

		}
	}
	return INVALID_VALUE;
}

SVAPI_API
OBJECT CreateEntityGroup(string entitygroupid)
{
	if(entitygroupid.empty())
		return INVALID_VALUE;
	try{
		EntityGroup *pe=new EntityGroup();
		pe->PutID(entitygroupid);
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
MAPNODE GetEntityGroupMainAttribNode(OBJECT egobj)
{
	if(egobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{
		EntityGroup *pe=reinterpret_cast<EntityGroup *>(egobj);
		MAPNODE ret=reinterpret_cast<MAPNODE>(&(pe->GetProperty()));
		return ret;

	}catch(...)
	{
		return INVALID_VALUE;
	}

	return INVALID_VALUE;

}

SVAPI_API
bool GetSubEntityTempletIDByEG(std::list<string> &etlist,OBJECT egobj)
{
	if(egobj==INVALID_VALUE)
		return false;
	try{
		EntityGroup *pe=reinterpret_cast<EntityGroup *>(egobj);
		WORDLIST &wl=pe->GetEntityKinds();
		WORDLIST::iterator it;
		for(it=wl.begin();it!=wl.end();it++)
			etlist.push_back((*it).getword());

		return true;

	}catch(...)
	{
		return false;
	}

	return false;


}

SVAPI_API
bool DeleteEntityGroup(string entitygroupid,string user,string addr)
{
	if(entitygroupid.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_DELETE;
	querybuf.datatype=S_ENTITYGROUP;
	strcpy(querybuf.qstr,entitygroupid.c_str());
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
bool AddSubEntityTempletIDToEG(OBJECT egobj,string entitytempletid)
{
	if(egobj==INVALID_VALUE)
		return false;
	try{
		EntityGroup *pe=reinterpret_cast<EntityGroup *>(egobj);
		WORDLIST &wl=pe->GetEntityKinds();
		wl.push_back(entitytempletid.c_str());

	}catch(...)
	{
		return false;
	}

	return true;

}

SVAPI_API
bool DeleteSubEntityTempletIDInEG(OBJECT egobj,string entitytempletid)
{
	if(egobj==INVALID_VALUE)
		return false;
	try{
		EntityGroup *pe=reinterpret_cast<EntityGroup *>(egobj);
		return pe->DeleteSubEntityKindsID(entitytempletid);
	}catch(...)
	{
		return false;
	}

	return false;

}

SVAPI_API
bool SubmitEntityGroup(OBJECT egobj,string user,string addr)
{
	if(egobj==INVALID_VALUE)
		return false;
	if(user.empty()||addr.empty())
		return false;
	if(user.size()>MAXUSERLEN)
		return false;

	try{
		EntityGroup *pmtp=reinterpret_cast<EntityGroup *>(egobj);
		S_UINT len=pmtp->GetRawDataSize();
		svutil::buffer buf;
		if(!buf.checksize(len))
			return false;
		if(pmtp->GetRawData(buf,len)==NULL)
			return false;

		SVDBQUERY querybuf={0};
		querybuf.len = sizeof(SVDBQUERY);
		querybuf.querytype=QUERY_UPDATE;
		querybuf.datatype=S_ENTITYGROUP;
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

SVAPI_API
bool CloseEntityGroup(OBJECT &egobj)
{
	if(egobj==INVALID_VALUE)
		return false;
	try{
		EntityGroup *pe=reinterpret_cast<EntityGroup *>(egobj);
		if(pe)
			delete pe;
		egobj=INVALID_VALUE;
	}catch(...)
	{
		return false;
	}

	return true;

}

SVAPI_API 
bool GetAllEntityGroups(PAIRLIST &retlist,string infoname,string user,string addr)
{
	if(user.empty()||addr.empty())
		return false;

	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;


	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_INFO;
	querybuf.datatype=S_ENTITYGROUP;
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
OBJECT CloneEntityGroup(OBJECT egobj,string entitygroupid)
{
	if(egobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{
		EntityGroup *pmtp=reinterpret_cast<EntityGroup *>(egobj);
		S_UINT len=pmtp->GetRawDataSize();
		svutil::buffer buf;
		if(!buf.checksize(len))
			return INVALID_VALUE;
		if(pmtp->GetRawData(buf,len)==NULL)
			return INVALID_VALUE;

		EntityGroup *pclone=new EntityGroup();
		if(pclone)
		{
			if(pclone->CreateObjectByRawData(buf,len))
			{
				WORDLIST &wl=pclone->GetEntityKinds();
				wl.clear();               
				pclone->PutID(entitygroupid);
				OBJECT ret=reinterpret_cast<OBJECT>(pclone);
				return ret;
			}
		}
	
		
	}catch(...)
	{
		return INVALID_VALUE;
	}

	return INVALID_VALUE;
}