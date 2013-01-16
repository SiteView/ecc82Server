#include "svapi.h"
#include <StringMap.h>
#include <Monitor.h>
#include "QueryData.h"
#include "ObjCache.h"


SVAPI_API
OBJECT	CacheRefresh_GetMonitor(string monitorid)
{
	return ObjectCache::RefreshFastGetMonitor(monitorid);
}


SVAPI_API
OBJECT	Cache_GetMonitor(string monitorid)
{
	return ObjectCache::FastGetMonitor(monitorid);
}

OBJECT	GetMonitorWithVer(string monitorid,string user,string addr,S_UINT & ver)
{
	if(monitorid.empty()||user.empty()||addr.empty())
		return INVALID_VALUE;
	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_MONITOR;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_GET;
	strcpy(query.qstr,monitorid.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;

	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		Monitor *pm=new Monitor();
		if(pm)
		{
				OBJECT ret=INVALID_VALUE;
				try{
					if(pm->CreateObjectByRawData(pdata+sizeof(S_UINT),len-sizeof(S_UINT)))
						ret=reinterpret_cast<OBJECT>(pm);
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
OBJECT	GetMonitor(string monitorid,string user,string addr)
{
	if(monitorid.empty()||user.empty()||addr.empty())
		return INVALID_VALUE;
	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;
	
	S_UINT ver(0);
	return GetMonitorWithVer(monitorid,user,addr,ver);
}

SVAPI_API 
bool GetAllMonitorsInfo(PAIRLIST &retlist,string infoname,string user,string addr)
{
	if(user.empty()||addr.empty())
		return false;

	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;


	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_INFO;
	querybuf.datatype=S_MONITOR;
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
bool GetMonitorsInfoBySE(string seid,PAIRLIST &retlist,string infoname,string user,string addr)
{
	if(user.empty()||addr.empty()||seid.empty())
		return false;

	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_INFOBYSE;
	querybuf.datatype=S_MONITOR;
	querybuf.datalen=atoi(seid.c_str());
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
OBJECT CreateMonitor(void)
{
	try{
		Monitor *pm=new Monitor();
		if(pm==NULL)
			return INVALID_VALUE;
		OBJECT ret=reinterpret_cast<OBJECT>(pm);
		return ret;


	}catch(...)
	{
		return INVALID_VALUE;
	}

	return INVALID_VALUE;


}

SVAPI_API
MAPNODE GetMonitorMainAttribNode(OBJECT monitorobj)
{
	if(monitorobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{
		Monitor *pm=reinterpret_cast<Monitor *>(monitorobj);
		MAPNODE ret=reinterpret_cast<MAPNODE>(&(pm->GetProperty()));
		return ret;

	}catch(...)
	{
		return INVALID_VALUE;
	}

}

SVAPI_API
MAPNODE GetMonitorParameter(OBJECT monitorobj)
{
	if(monitorobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{
		Monitor *pm=reinterpret_cast<Monitor *>(monitorobj);
		MAPNODE ret=reinterpret_cast<MAPNODE>(&(pm->GetParameters()));
		return ret;

	}catch(...)
	{
		return INVALID_VALUE;
	}

}

SVAPI_API
MAPNODE GetMonitorAdvanceParameterNode(OBJECT monitorobj)
{
	if(monitorobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{
		Monitor *pm=reinterpret_cast<Monitor *>(monitorobj);
		MAPNODE ret=reinterpret_cast<MAPNODE>(&(pm->GetADParameters()));
		return ret;

	}catch(...)
	{
		return INVALID_VALUE;
	}

}

SVAPI_API 
MAPNODE	GetMonitorErrorAlertCondition(OBJECT monitorobj)
{
	if(monitorobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{
		Monitor *pm=reinterpret_cast<Monitor *>(monitorobj);
		StringMap *psm=pm->GetAlertCondition();
		MAPNODE ret=reinterpret_cast<MAPNODE>(&(psm[0]));
		return ret;

	}catch(...)
	{
		return INVALID_VALUE;
	}

}

SVAPI_API 
MAPNODE GetMonitorWarningAlertCondition(OBJECT monitorobj)
{
	if(monitorobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{
		Monitor *pm=reinterpret_cast<Monitor *>(monitorobj);
		StringMap *psm=pm->GetAlertCondition();
		MAPNODE ret=reinterpret_cast<MAPNODE>(&(psm[1]));
		return ret;

	}catch(...)
	{
		return INVALID_VALUE;
	}

}

SVAPI_API 
MAPNODE	GetMonitorGoodAlertCondition(OBJECT monitorobj)
{
	if(monitorobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{
		Monitor *pm=reinterpret_cast<Monitor *>(monitorobj);
		StringMap *psm=pm->GetAlertCondition();
		MAPNODE ret=reinterpret_cast<MAPNODE>(&(psm[2]));
		return ret;

	}catch(...)
	{
		return INVALID_VALUE;
	}

}

SVAPI_API
bool DeleteSVMonitor(string monitorid,string user,string addr)
{
	if(monitorid.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_DELETE;
	querybuf.datatype=S_MONITOR;
	strcpy(querybuf.qstr,monitorid.c_str());
	strcpy(querybuf.idcuser,user.c_str());

    QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;
	if(qd.Query(&querybuf,(void **)&pdata,len,addr))
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

	return false;

}

SVAPI_API
void CloseMonitor(OBJECT &monitorobj)
{
	if(monitorobj==INVALID_VALUE)
		return ;
	try{
		Monitor *pm=reinterpret_cast<Monitor *>(monitorobj);
		if(pm)
			delete pm;
		monitorobj=INVALID_VALUE;
	}catch(...)
	{
		;
	}

}

SVAPI_API 
bool	SubmitMonitor(OBJECT monitorobj,string user,string addr)
{
	if(monitorobj==INVALID_VALUE)
		return false;
	if(user.empty()||addr.empty())
		return false;

	try{
		Monitor *pmtp=reinterpret_cast<Monitor *>(monitorobj);
		S_UINT len=pmtp->GetRawDataSize();
		svutil::buffer buf;
		if(!buf.checksize(len))
			return false;
		if(pmtp->GetRawData(buf,len)==NULL)
			return false;

		SVDBQUERY querybuf={0};
		querybuf.len = sizeof(SVDBQUERY);
		querybuf.querytype=QUERY_UPDATE;
		querybuf.datatype=S_MONITOR;
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

//insert the monitor created by CreateMonitor() into SVDB
SVAPI_API
string AddNewMonitor(OBJECT monitorobj,string entityid,string user,string addr)
{
	if(monitorobj==INVALID_VALUE)
		return "";
	if(user.empty()||addr.empty()||entityid.empty())
		return "";

	try{
		Monitor *pmtp=reinterpret_cast<Monitor *>(monitorobj);
		S_UINT len=pmtp->GetRawDataSize();
		svutil::buffer buf;
		if(!buf.checksize(len))
			return "";
		if(pmtp->GetRawData(buf,len)==NULL)
			return "";

		SVDBQUERY querybuf={0};
		querybuf.len = sizeof(SVDBQUERY);
		querybuf.querytype=QUERY_ADDNEW;
		querybuf.datatype=S_MONITOR;
		strcpy(querybuf.qstr,entityid.c_str());
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



string MonitorCopyGeneral(bool bCreatTable,string srcmonitorid,string objentityid,string user,string addr)
{
	if(srcmonitorid.empty()||objentityid.empty()||user.empty()||addr.empty())
		return "";

	OBJECT monitorobj= GetMonitor(srcmonitorid,user,addr);
	if(monitorobj==INVALID_VALUE)
		return "";

	string nid=AddNewMonitor(monitorobj,objentityid,user,addr);
	if( bCreatTable && !nid.empty() )
	{
		MAPNODE ma= GetMonitorMainAttribNode(monitorobj);
		string value;
		FindNodeValue(ma,"sv_monitortype",value);
		int mid= atoi(value.c_str());
		if(mid>0)
			InsertTable(nid,mid,user,addr);
	}
	CloseMonitor(monitorobj);
	return nid;
}



SVAPI_API
string MonitorCopyAndCreateTable(string srcmonitorid,string objentityid,string user,string addr)
{
	return MonitorCopyGeneral(true,srcmonitorid,objentityid,user,addr);
}


SVAPI_API
string MonitorCopy(string srcmonitorid,string objentityid,string user,string addr)
{
	return MonitorCopyGeneral(false,srcmonitorid,objentityid,user,addr);
}




