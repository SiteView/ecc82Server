#include "svapi.h"
#include <StringMap.h>
#include <EntityTemplet.h>
#include "QueryData.h"
#include "ObjCache.h"

SVAPI_API
OBJECT	Cache_GetEntityTemplet(string entitytempletid)
{
	return ObjectCache::FastGetEntityTPL(entitytempletid);
}

SVAPI_API
OBJECT GetEntityTemplet(string entitytempletid,string user,string addr)
{
	if(entitytempletid.empty()||user.empty()||addr.empty())
		return INVALID_VALUE;
	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_ENTITYTEMPLET;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_GET;
	strcpy(query.qstr,entitytempletid.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;

	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		EntityTemplet *pe=new EntityTemplet();
		if(pe)
		{
			OBJECT ret=INVALID_VALUE;
			try{
				if(pe->CreateObjectByRawData(pdata,len))
					ret=reinterpret_cast<OBJECT>(pe);
				if(pdata!=NULL)
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
OBJECT CreateEntityTemplet(string entitytempletid)
{
	if(entitytempletid.empty())
		return INVALID_VALUE;
	try{
		EntityTemplet *pe=new EntityTemplet();
		pe->PutID(entitytempletid);
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
bool DeleteEntityTemplet(string entitytempletid,string user,string addr)
{
	if(entitytempletid.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_DELETE;
	querybuf.datatype=S_ENTITYTEMPLET;
	strcpy(querybuf.qstr,entitytempletid.c_str());
	strcpy(querybuf.idcuser,user.c_str());

    QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;
	if(qd.Query(&querybuf,(void **)&pdata,len,addr))
	{
		if(pdata!=NULL)
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
MAPNODE GetEntityTempletMainAttribNode(OBJECT etobj)
{
	if(etobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{
		EntityTemplet *pe=reinterpret_cast<EntityTemplet *>(etobj);
		MAPNODE ret=reinterpret_cast<MAPNODE>(&(pe->GetProperty()));
		return ret;

	}catch(...)
	{
		return INVALID_VALUE;
	}

	return INVALID_VALUE;

}

SVAPI_API 
bool FindETContrlFirst(OBJECT etobj,LISTITEM &item)
{
	if(etobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{

		EntityTemplet *pmtp=reinterpret_cast<EntityTemplet *>(etobj);
		STRMAPLIST &slist=pmtp->GetContrlList();
		item.list=reinterpret_cast<OBJECT>(&slist);
		STRMAPLIST::iterator *pit=new STRMAPLIST::iterator();
		(*pit)=slist.begin();
		item.iterator=reinterpret_cast<OBJECT>(pit);
		return true;

	}catch(...)
	{
		return false;
	}

	return true;

}

SVAPI_API
bool GetSubMonitorTypeByET(OBJECT etobj,std::list<int> &mtlist)
{
	if(etobj==INVALID_VALUE)
		return false;
	try{
		EntityTemplet *pe=reinterpret_cast<EntityTemplet *>(etobj);
		std::list<int> &mid=pe->GetMonitorKinds();

		std::list<int>::iterator it;
		for(it=mid.begin();it!=mid.end();it++)
			mtlist.push_back((*it));

		return true;

	}catch(...)
	{
		return false;
	}

	return false;

}

SVAPI_API
std::list<int> & GetSubMonitorTypeList(OBJECT etobj)
{
	if(etobj==INVALID_VALUE)
	{
		try{
			EntityTemplet *pe=reinterpret_cast<EntityTemplet *>(etobj);
			return pe->GetMonitorKinds();
		}catch(...)
		{
			;
		}
	}


}

SVAPI_API
bool AddSubMonitorTypeToET(OBJECT etobj,int MonitorType)
{
	if(etobj==INVALID_VALUE)
		return false;
	try{
		EntityTemplet *pe=reinterpret_cast<EntityTemplet *>(etobj);
		std::list<int> &mid=pe->GetMonitorKinds();
		mid.push_back(MonitorType);
		return true;

	}catch(...)
	{
		return false;
	}

	return false;


}

SVAPI_API
bool DeleteSubMonitorTypeInET(OBJECT etobj,int MonitorType)
{
	if(etobj==INVALID_VALUE)
		return false;
	try{
		EntityTemplet *pe=reinterpret_cast<EntityTemplet *>(etobj);
		return pe->DeleteMonitorKindsID(MonitorType);

	}catch(...)
	{
		return false;
	}

	return false;

}

SVAPI_API
bool SubmitEntityTemplet(OBJECT etobj,string user,string addr)
{
	if(etobj==INVALID_VALUE)
		return false;
	if(user.empty()||addr.empty())
		return false;
	if(user.size()>MAXUSERLEN)
		return false;

	try{
		EntityTemplet *pmtp=reinterpret_cast<EntityTemplet *>(etobj);
		S_UINT len=pmtp->GetRawDataSize();
		svutil::buffer buf;
		if(!buf.checksize(len))
			return false;
		if(pmtp->GetRawData(buf,len)==NULL)
			return false;

		SVDBQUERY querybuf={0};
		querybuf.len = sizeof(SVDBQUERY);
		querybuf.querytype=QUERY_UPDATE;
		querybuf.datatype=S_ENTITYTEMPLET;
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
bool CloseEntityTemplet(OBJECT &etobj)
{
	if(etobj==INVALID_VALUE)
		return false;
	try{
		EntityTemplet *pe=reinterpret_cast<EntityTemplet *>(etobj);
		if(pe)
			delete pe;
		etobj=INVALID_VALUE;
	}catch(...)
	{
		return false;
	}

	return true;

}

SVAPI_API 
bool GetAllEntityTemplets(PAIRLIST &retlist,string infoname,string user,string addr)
{
	if(user.empty()||addr.empty())
		return false;

	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;


	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_INFO;
	querybuf.datatype=S_ENTITYTEMPLET;
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
OBJECT CloneEntityTemplet(OBJECT etobj,string entitytempletid)
{
	if(etobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{
		EntityTemplet *pmtp=reinterpret_cast<EntityTemplet *>(etobj);
		S_UINT len=pmtp->GetRawDataSize();
		svutil::buffer buf;
		if(!buf.checksize(len))
			return INVALID_VALUE;
		if(pmtp->GetRawData(buf,len)==NULL)
			return INVALID_VALUE;

		EntityTemplet *pclone=new EntityTemplet();
		if(pclone)
		{
			if(pclone->CreateObjectByRawData(buf,len))
			{
				pclone->PutID(entitytempletid);
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

SVAPI_API 
bool GetAllEntityTempletsInProperty(string key,string value,std::list<string> &idlist,string user,string addr)
{
	if(addr.empty())
		return false;
	if(key.size()>MAXQUEREYSTRINGLEN)
		return false;

	QueryData qd;
	char *pdata=NULL;
	S_UINT rlen=0;

	S_UINT len=0;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_FAST_GET;
	querybuf.datatype=S_ENTITYTEMPLET;
	strcpy(querybuf.qstr,key.c_str());

	INIQUERY iquery={0};
	iquery.len=sizeof(INIQUERY);
	iquery.datatype=D_STRING;
	iquery.datalen=value.size()+1;

	len+=sizeof(INIQUERY);
	len+=value.size()+1;

	querybuf.datalen=len;

	buffer buf;
	if(!buf.checksize(len))
		return false;
	char *pt=buf.getbuffer();
	memcpy(pt,&iquery,sizeof(INIQUERY));
	pt+=sizeof(INIQUERY);
	strcpy(pt,value.c_str());
	pt+=value.size();
	pt[0]='\0';

	if(qd.Query(&querybuf,buf,len,(void **)&pdata,rlen,addr))
	{
		try{
			idlist.clear();
			if(::CreateStrListByRawData(idlist,pdata,rlen))
			{
				if(pdata!=NULL)
					delete [] pdata;
				return true;
			}
		}catch(...)
		{
		}
	}
	if(pdata!=NULL)
		delete [] pdata;
	return false;
}