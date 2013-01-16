#include "svapi.h"
#include <StringMap.h>
#include <Resource.h>
#include "QueryData.h"
#include "ObjCache.h"

SVAPI_API
OBJECT	Cache_LoadResourceByKeys(string needkeys, string language)
{
	return ObjectCache::FastLoadResourceByKeys(needkeys, language);
}


SVAPI_API 
OBJECT LoadResourceByKeys( string needkeys, string language,string addr)
{
	if(language.empty()||addr.empty()||needkeys.empty())
		return INVALID_VALUE;

	QueryData qd;
	char *pdata=NULL;
	S_UINT rlen=0;

	S_UINT len=0;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_FAST_GET;
	querybuf.datatype=S_RESOURCE;
	strcpy(querybuf.qstr,language.c_str());

	INIQUERY iquery={0};
	iquery.len=sizeof(INIQUERY);
	iquery.datatype=D_STRING;
	iquery.datalen=needkeys.size()+1;

	len+=sizeof(INIQUERY);
	len+=needkeys.size()+1;

	querybuf.datalen=len;

	buffer buf;
	if(!buf.checksize(len))
		return INVALID_VALUE;
	char *pt=buf.getbuffer();
	memcpy(pt,&iquery,sizeof(INIQUERY));
	pt+=sizeof(INIQUERY);
	strcpy(pt,needkeys.c_str());
	pt+=needkeys.size();
	pt[0]='\0';


	if(qd.Query(&querybuf,buf,len,(void **)&pdata,rlen,addr))
	{
		Resource *prc=new Resource();
		if(prc)
		{
			OBJECT ret=INVALID_VALUE;
			try{
				if(prc->CreateObjectByRawData(pdata,rlen))
					ret=reinterpret_cast<OBJECT>(prc);
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
OBJECT Cache_LoadResource(string language)
{
	return ObjectCache::FastLoadResource(language);
}
	

SVAPI_API 
OBJECT LoadResource(string language,string addr)
{
	if(language.empty()||addr.empty())
		return INVALID_VALUE;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_RESOURCE;
	query.querytype=QUERY_GET;
	strcpy(query.qstr,language.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;

	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		Resource *prc=new Resource();
		if(prc)
		{
			OBJECT ret=INVALID_VALUE;
			try{
				if(prc->CreateObjectByRawData(pdata,len))
					ret=reinterpret_cast<OBJECT>(prc);
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
bool GetAllResourceInfo(PAIRLIST &retlist,string addr)
{
	if(addr.empty())
		return false;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_INFO;
	querybuf.datatype=S_RESOURCE;
//	strcpy(querybuf.idcuser,user.c_str());

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
OBJECT CreateResource(string language)
{
	try{
		Resource *prc=new Resource();
		prc->PutLanguageType(language);
		if(prc==NULL)
			return INVALID_VALUE;
		OBJECT ret=reinterpret_cast<OBJECT>(prc);
		return ret;
	}catch(...)
	{
		return INVALID_VALUE;
	}

	return INVALID_VALUE;

}

SVAPI_API 
MAPNODE GetResourceNode(OBJECT rcobj)
{
	if(rcobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{
		Resource *prc=reinterpret_cast<Resource *>(rcobj);
		MAPNODE ret=reinterpret_cast<MAPNODE>(&(prc->GetResourceDict()));
		return ret;

	}catch(...)
	{
		return INVALID_VALUE;
	}
	return INVALID_VALUE;

}

SVAPI_API 
bool	EnumResourceAttrib(OBJECT rcobj,PAIRLIST &retlist)
{
	if(rcobj==INVALID_VALUE)
		return false;
	try{

		Resource *prc=reinterpret_cast<Resource *>(rcobj);
		MAPNODE ret=reinterpret_cast<MAPNODE>(&(prc->GetResourceDict()));
		return EnumNodeAttrib(ret,retlist);

	}catch(...)
	{
		return false;
	}
	return false;

}

SVAPI_API
bool	CloseResource(OBJECT &rcobj)
{
	if(rcobj==INVALID_VALUE)
		return false;
	try{

		Resource *prc=reinterpret_cast<Resource *>(rcobj);
		if(prc)
			delete prc;
		rcobj=INVALID_VALUE;

	}catch(...)
	{
		return false;
	}
	return true;;

}

SVAPI_API
bool	DeleteResource(string language,string addr)
{
	if(language.empty()||addr.empty())
		return false;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_DELETE;
	querybuf.datatype=S_RESOURCE;
	strcpy(querybuf.qstr,language.c_str());

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
bool	SubmitResource(OBJECT rcobj,string addr)
{
	if(rcobj==INVALID_VALUE)
		return false;
	if(addr.empty())
		return false;
	try{
		Resource *pmtp=reinterpret_cast<Resource *>(rcobj);
		string strlang=pmtp->GetLanguageType().getword();
		if(strlang.empty())
			return false;

		S_UINT len=pmtp->GetRawDataSize();
		svutil::buffer buf;
		if(!buf.checksize(len))
			return false;
		if(pmtp->GetRawData(buf,len)==NULL)
			return false;

		SVDBQUERY querybuf={0};
		querybuf.len = sizeof(SVDBQUERY);
		querybuf.querytype=QUERY_UPDATE;
		querybuf.datatype=S_RESOURCE;
		strcpy(querybuf.qstr,pmtp->GetLanguageType().getword());
		querybuf.datalen=len;

		QueryData qd;
		SVDBRESULT ret={0};
		return qd.Update(&querybuf,buf,len,&ret,addr);
	}catch(...)
	{
		return false;
	}


	return true;

}

