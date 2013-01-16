#include "svapi.h"
#include <StringMap.h>
#include <SVSE.h>
#include "QueryData.h"
#include "ObjCache.h"


SVAPI_API 
OBJECT	Cache_GetSVSE(string id)
{
	return ObjectCache::FastGetSVSE(id);
}

OBJECT	GetSVSEWithVer(string id,string user,string addr,S_UINT & ver)
{
	if(atoi(id.c_str())<=0)
		return INVALID_VALUE;
	if(user.empty()||addr.empty())
		return INVALID_VALUE;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_GET;
	querybuf.datatype=S_SVSE;
	strcpy(querybuf.qstr,id.c_str());
	strcpy(querybuf.idcuser,user.c_str());

    QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;
	if(qd.Query(&querybuf,(void **)&pdata,len,addr))
	{
		SVSE *pmtp=new SVSE();
		if(pmtp)
		{
			OBJECT ret=INVALID_VALUE;
			if(pdata!=NULL)
			{
				try{
					if(pmtp->CreateObjectByRawData(pdata+sizeof(S_UINT),len-sizeof(S_UINT)))
						ret=reinterpret_cast<OBJECT>(pmtp);
					memmove(&ver,pdata,sizeof(S_UINT));
					if(pdata!=NULL)
						delete [] pdata;
				}catch(...)
				{
					if(pdata!=NULL)
						delete [] pdata;
					return INVALID_VALUE;
				}
				return ret;
			}
		}
	}
	if(pdata!=NULL)
		delete [] pdata;
	return INVALID_VALUE;
}


SVAPI_API 
OBJECT	GetSVSE(string id,string user,string addr)
{
	if(atoi(id.c_str())<=0)
		return INVALID_VALUE;
	if(user.empty()||addr.empty())
		return INVALID_VALUE;

	S_UINT ver(0);
	return GetSVSEWithVer(id, user, addr, ver);
}

SVAPI_API 
bool GetAllSVSEInfo(PAIRLIST &retlist,string user,string addr)
{
	return GetAllSVSEObjInfo(retlist,"",user,addr);
}


SVAPI_API 
bool GetAllSVSEObjInfo(PAIRLIST &retlist,string infoname,string user,string addr)
{
	if(user.empty()||addr.empty())
		return false;

	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;

	if(infoname.size()>MAXQUEREYSTRINGLEN)
		return INVALID_VALUE;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_INFO;
	querybuf.datatype=S_SVSE;
	strcpy(querybuf.idcuser,user.c_str());
	strcpy(querybuf.qstr,infoname.c_str());

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
OBJECT	CreateSVSE(string label)
{
	try{
		SVSE *pmt=new SVSE();
		pmt->PutLabel(label);
		if(pmt==NULL)
			return INVALID_VALUE;
		OBJECT ret=reinterpret_cast<OBJECT>(pmt);
		return ret;

	}catch(...)
	{
		return INVALID_VALUE;
	}

	return INVALID_VALUE;

}

SVAPI_API
bool PutSVSELabel(OBJECT svseobj,string label)
{
	if(svseobj==INVALID_VALUE)
		return false;
	try{

		SVSE *pmtp=reinterpret_cast<SVSE *>(svseobj);
		pmtp->PutLabel(label);
		
	}catch(...)
	{
       return false;
	}
      return true;;

}

SVAPI_API	
string GetSVSELabel(OBJECT svseobj)
{
	if(svseobj==INVALID_VALUE)
		return "";
	try{

		SVSE *pmtp=reinterpret_cast<SVSE *>(svseobj);
		return pmtp->GetLabel().getword();
		
	}catch(...)
	{
       return "";
	}
      return "";

}

SVAPI_API
bool GetSubGroupsIDBySE(OBJECT svseobj,std::list<string> &idlist)
{
	if(svseobj==INVALID_VALUE)
		return false;
	try{

		SVSE *pmtp=reinterpret_cast<SVSE *>(svseobj);
		WORDLIST gl=pmtp->GetSubGroups();
		WORDLIST::iterator it;
		for(it=gl.begin();it!=gl.end();it++)
			idlist.push_back((*it).getword());
		
	}catch(...)
	{
       return false;
	}
     return true;

}
SVAPI_API
bool GetSubGroupsIDBySEEx(OBJECT svseobj,std::list<char *> &idlist)
{
	if(svseobj==INVALID_VALUE)
		return false;
	try{

		SVSE *pmtp=reinterpret_cast<SVSE *>(svseobj);
		WORDLIST gl=pmtp->GetSubGroups();
		WORDLIST::iterator it;
		for(it=gl.begin();it!=gl.end();it++)
			idlist.push_back(_strdup((*it).getword()));
		
	}catch(...)
	{
       return false;
	}
     return true;

}


SVAPI_API
bool GetSubEntitysIDBySE(OBJECT svseobj,std::list<string> &idlist)
{
	if(svseobj==INVALID_VALUE)
		return false;
	try{

		SVSE *pmtp=reinterpret_cast<SVSE *>(svseobj);
		WORDLIST gl=pmtp->GetSubEntitys();
		WORDLIST::iterator it;
		for(it=gl.begin();it!=gl.end();it++)
			idlist.push_back((*it).getword());
		
	}catch(...)
	{
       return false;
	}
     return true;

}

SVAPI_API
bool DeleteSVSE(string id,string user,string addr)
{
	if(id.empty()||addr.empty()||user.empty())
		return false;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_DELETE;
	querybuf.datatype=S_SVSE;
	strcpy(querybuf.qstr,id.c_str());
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
bool CloseSVSE(OBJECT &svseobj)
{
	if(svseobj==INVALID_VALUE)
		return false;
	try{

		SVSE *pmtp=reinterpret_cast<SVSE *>(svseobj);
		if(pmtp)
			delete pmtp;

		svseobj=INVALID_VALUE;
	}catch(...)
	{
		return false;
	}

	return true;
}

SVAPI_API
string AddNewSVSE(OBJECT svseobj,string user,string addr)
{
	if(svseobj==INVALID_VALUE)
		return "";
	if(user.empty()||addr.empty())
		return "";

	try{
		SVSE *pmtp=reinterpret_cast<SVSE *>(svseobj);
		S_UINT len=pmtp->GetRawDataSize();
		svutil::buffer buf;
		if(!buf.checksize(len))
			return "";
		if(pmtp->GetRawData(buf,len)==NULL)
			return "";

		SVDBQUERY querybuf={0};
		querybuf.len = sizeof(SVDBQUERY);
		querybuf.querytype=QUERY_ADDNEW;
		querybuf.datatype=S_SVSE;
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
	return "";
}

SVAPI_API
bool SubmitSVSE(OBJECT svseobj,string user,string addr)
{
	if(svseobj==INVALID_VALUE)
		return false;
	if(user.empty()||addr.empty())
		return false;
	try{
		SVSE *pmtp=reinterpret_cast<SVSE *>(svseobj);
		S_UINT len=pmtp->GetRawDataSize();
		svutil::buffer buf;
		if(!buf.checksize(len))
			return false;
		if(pmtp->GetRawData(buf,len)==NULL)
			return false;

		SVDBQUERY querybuf={0};
		querybuf.len = sizeof(SVDBQUERY);
		querybuf.querytype=QUERY_UPDATE;
		querybuf.datatype=S_SVSE;
		sprintf(querybuf.qstr,"%d",pmtp->GetID());
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
