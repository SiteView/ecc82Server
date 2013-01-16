#include "svnnmapi.h"
#include <StringMap.h>
#include <VirtualGroup.h>
#include "QueryData.h"

SVAPI_API
OBJECT GetVirtualGroup(string VirtualGroupID,string user,string addr)
{
	if(VirtualGroupID.empty()||user.empty()||addr.empty())
		return INVALID_VALUE;
	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_VIRTUALGROUP;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_GET;
	strcpy(query.qstr,VirtualGroupID.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;

	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		VirtualGroup *pe=new VirtualGroup();
		if(pe)
		{
			OBJECT ret=INVALID_VALUE;
			try{
				if(pe->CreateObjectByRawData(pdata,len))
					ret=reinterpret_cast<OBJECT>(pe);
			}catch(...)
			{
				return INVALID_VALUE;
			}
			delete [] pdata;
			return ret;

		}
	}

	return INVALID_VALUE;
}

SVAPI_API
OBJECT CreateVirtualGroup(string VirtualGroupID)
{
	if(VirtualGroupID.empty())
		return INVALID_VALUE;

	try{
		VirtualGroup *pe=new VirtualGroup();
		if(pe==NULL)
			return INVALID_VALUE;
		pe->PutID(VirtualGroupID);
		OBJECT ret=reinterpret_cast<OBJECT>(pe);
		return ret;
	}catch(...)
	{
		return INVALID_VALUE;
	}

	return INVALID_VALUE;
}

SVAPI_API
MAPNODE GetVirtualGroupMainAttribNode(OBJECT virtualgroupobj)
{
	if(virtualgroupobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{
		VirtualGroup *pe=reinterpret_cast<VirtualGroup *>(virtualgroupobj);
		MAPNODE ret=reinterpret_cast<MAPNODE>(&(pe->GetProperty()));
		return ret;

	}catch(...)
	{
		return INVALID_VALUE;
	}

		return INVALID_VALUE;
}

SVAPI_API
bool DeleteVirtualGroup(string virtualgroupid,string user,string addr)
{
	if(virtualgroupid.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_DELETE;
	querybuf.datatype=S_VIRTUALGROUP;
	strcpy(querybuf.qstr,virtualgroupid.c_str());
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
bool CloseVirtualGroup(OBJECT &virtualgroupobj)
{
	if(virtualgroupobj==INVALID_VALUE)
		return false;
	try{
		VirtualGroup *pe=reinterpret_cast<VirtualGroup *>(virtualgroupobj);
		if(pe)
			delete pe;
		virtualgroupobj=INVALID_VALUE;
	}catch(...)
	{
		return false;
	}

	return true;

}

SVAPI_API
bool SubmitVirtualGroup(OBJECT virtualgroupobj,string user,string addr)
{
	if(virtualgroupobj==INVALID_VALUE)
		return false;
	if(user.empty()||addr.empty())
		return false;
	try{
		VirtualGroup *pmtp=reinterpret_cast<VirtualGroup *>(virtualgroupobj);
		if(pmtp->GetID().empty())
		{
			printf("ID of VirtualGroup is empty. svapi error.");
			return false;
		}

		S_UINT len=pmtp->GetRawDataSize();
		svutil::buffer buf;
		if(!buf.checksize(len))
			return false;
		if(pmtp->GetRawData(buf,len)==NULL)
			return false;

		SVDBQUERY querybuf={0};
		querybuf.len = sizeof(SVDBQUERY);
		querybuf.querytype=QUERY_UPDATE;
		querybuf.datatype=S_VIRTUALGROUP;
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
bool GetAllVirtualGroupInfo(PAIRLIST &retlist,string infoname,string user,string addr)
{
	if(user.empty()||addr.empty())
		return false;

	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;


	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_INFO;
	querybuf.datatype=S_VIRTUALGROUP;
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


bool GetAllVirtualGroupInfoFrom(string what, string key,string value,std::list<string> &idlist,string user,string addr)
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
	querybuf.datatype=S_VIRTUALGROUP;
	strcpy(querybuf.qstr,key.c_str());
	strcpy(querybuf.idcuser,what.c_str());

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

SVAPI_API 
bool GetAllVirtualGroupInfoInWords(string key,string value,std::list<string> &idlist,string user,string addr)
{
	return GetAllVirtualGroupInfoFrom("word",key,value,idlist,user,addr);
}

SVAPI_API 
bool GetAllVirtualGroupInfoInProperty(string key,string value,std::list<string> &idlist,string user,string addr)
{
	return GetAllVirtualGroupInfoFrom("property",key,value,idlist,user,addr);
}

