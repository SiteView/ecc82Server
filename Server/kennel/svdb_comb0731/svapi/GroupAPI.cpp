#include "svapi.h"
#include <StringMap.h>
#include <Group.h>
#include "QueryData.h"
#include "ObjCache.h"


SVAPI_API
string  CreatIdcUser(string userindex, string pid, string addr)
{
	string newid("");
	if(userindex.empty()||addr.empty() || pid.empty())
		return newid;

	try{
		S_UINT len=0;
		SVDBQUERY querybuf={0};
		querybuf.len = sizeof(SVDBQUERY);
		querybuf.querytype=QUERY_CREATIDC;
		querybuf.datatype=S_GROUP;
		strcpy(querybuf.idcuser,userindex.c_str());
		strcpy(querybuf.qstr,pid.c_str());
		querybuf.datalen=len;

		char *pdata=NULL;
		QueryData qd;
		if(qd.Query(&querybuf,(void **)&pdata,len,addr))
		{
			if(pdata)
			{
				if(len>0)
				{
					newid= pdata;
					delete [] pdata;
					return newid;
				}
				delete [] pdata;
			}
		}
	}catch(...)
	{
	}
	return newid;
}



SVAPI_API
bool CopyIniFile(string filecopy,string addr,string user)
{
	if(filecopy.empty()||addr.empty()||user.empty())
		return false;
	if( filecopy.size()>MAXQUEREYSTRINGLEN )
		return false;
	if(user.compare("default")==0)
		return false;

	try{
		S_UINT dlen=0;

		SVDBQUERY querybuf={0};
		querybuf.len = sizeof(SVDBQUERY);
		querybuf.querytype=QUERY_COPY_INI;
		querybuf.datatype=S_GROUP;
		strcpy(querybuf.qstr,filecopy.c_str());
		strcpy(querybuf.idcuser,user.c_str());

		INIQUERY iquery={0};
		iquery.len=sizeof(INIQUERY);
		dlen+=sizeof(INIQUERY);

		querybuf.datalen=dlen;

		buffer buf;
		if(!buf.checksize(dlen))
			return false;
		char *pt=buf.getbuffer();
		memcpy(pt,&iquery,sizeof(INIQUERY));

		QueryData qd;
		SVDBRESULT ret={0};
		return qd.Update(&querybuf,buf,dlen,&ret,addr);
	}catch(...)
	{
		return false;
	}

}

SVAPI_API
OBJECT	Cache_GetGroup(string groupid)
{
	return ObjectCache::FastGetGroup(groupid);
}

OBJECT	GetGroupWithVer(string groupid,string user,string addr,S_UINT & ver)
{
	if(groupid.empty()||user.empty()||addr.empty())
		return INVALID_VALUE;
	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_GROUP;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_GET;
	strcpy(query.qstr,groupid.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;

	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		Group *pg=new Group();
		if(pg)
		{
			OBJECT ret=INVALID_VALUE;
			try{
				if(pg->CreateObjectByRawData(pdata+sizeof(S_UINT),len-sizeof(S_UINT)))
					ret=reinterpret_cast<OBJECT>(pg);
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
	if(pdata!=NULL)
		delete [] pdata;
	return INVALID_VALUE;
}


SVAPI_API
OBJECT	GetGroup(string groupid,string user,string addr)
{
	if(groupid.empty()||user.empty()||addr.empty())
		return INVALID_VALUE;
	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;

	S_UINT ver(0);
	return GetGroupWithVer(groupid,user,addr,ver);
}

SVAPI_API 
bool GetAllGroupsInfo(PAIRLIST &retlist,string infoname,string user,string addr)
{
	if(user.empty()||addr.empty())
		return false;

	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;

	//printf("inside GetAllGroupsInfo\n");
	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_INFO;
	querybuf.datatype=S_GROUP;
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
OBJECT CreateGroup(void)
{

	try{
		Group *pg=new Group();
		if(pg==NULL)
			return INVALID_VALUE;
		OBJECT ret=reinterpret_cast<OBJECT>(pg);
		return ret;
	}catch(...)
	{
		return INVALID_VALUE;
	}

	return INVALID_VALUE;

}

SVAPI_API
MAPNODE GetGroupMainAttribNode(OBJECT groupobj)
{
	if(groupobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{
		Group *pg=reinterpret_cast<Group *>(groupobj);
		MAPNODE ret=reinterpret_cast<MAPNODE>(&(pg->GetProperty()));
		return ret;

	}catch(...)
	{
		return INVALID_VALUE;
	}
	return INVALID_VALUE;


}


SVAPI_API
bool DeleteGroup(string groupid,string user,string addr)
{
	if(groupid.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_DELETE;
	querybuf.datatype=S_GROUP;
	strcpy(querybuf.qstr,groupid.c_str());
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
bool CloseGroup(OBJECT &groupobj)
{
	if(groupobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{
		Group *pg=reinterpret_cast<Group *>(groupobj);
		if(pg)
			delete pg;
        groupobj=INVALID_VALUE;
	}catch(...)
	{
		return false;
	}

	return true;

}

SVAPI_API
bool GetSubGroupsIDByGroup(OBJECT groupobj,std::list<string> &idlist)
{
	if(groupobj==INVALID_VALUE)
		return false;
	try{

		Group *pg=reinterpret_cast<Group *>(groupobj);
		
		WORDLIST gl=pg->GetSubGroups();
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
bool GetSubGroupsIDByGroupEx(OBJECT groupobj,std::list<char *> &idlist)
{
	if(groupobj==INVALID_VALUE)
		return false;
	try{

		Group *pg=reinterpret_cast<Group *>(groupobj);
		
		WORDLIST gl=pg->GetSubGroups();
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
bool GetSubEntitysIDByGroup(OBJECT groupobj,std::list<string> &idlist)
{
	if(groupobj==INVALID_VALUE)
		return false;
	try{

		Group *pg=reinterpret_cast<Group *>(groupobj);
		
		WORDLIST gl=pg->GetEntitys();
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
string AddNewGroup(OBJECT groupobj,string pid,string user,string addr)
{
	if(groupobj==INVALID_VALUE)
		return "";
	if(user.empty()||addr.empty()||pid.empty())
		return "";

	try{
		Group *pmtp=reinterpret_cast<Group *>(groupobj);
		S_UINT len=pmtp->GetRawDataSize();
		svutil::buffer buf;
		if(!buf.checksize(len))
			return "";
		if(pmtp->GetRawData(buf,len)==NULL)
			return "";

		SVDBQUERY querybuf={0};
		querybuf.len = sizeof(SVDBQUERY);
		querybuf.querytype=QUERY_ADDNEW;
		querybuf.datatype=S_GROUP;
		strcpy(querybuf.qstr,pid.c_str());
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
bool SubmitGroup(OBJECT groupobj,string user,string addr)
{
	if(groupobj==INVALID_VALUE)
		return false;
	if(user.empty()||addr.empty())
		return false;
	try{
		Group *pmtp=reinterpret_cast<Group *>(groupobj);
		S_UINT len=pmtp->GetRawDataSize();
		svutil::buffer buf;
		if(!buf.checksize(len))
			return false;
		if(pmtp->GetRawData(buf,len)==NULL)
			return false;

		SVDBQUERY querybuf={0};
		querybuf.len = sizeof(SVDBQUERY);
		querybuf.querytype=QUERY_UPDATE;
		querybuf.datatype=S_GROUP;
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

