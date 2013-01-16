#include "svnnmapi.h"
#include <StringMap.h>
#include <TopologyChart.h>
#include "QueryData.h"

SVAPI_API
OBJECT GetTopologyChart(string TopologyID,string user,string addr)
{
	if(TopologyID.empty()||user.empty()||addr.empty())
		return INVALID_VALUE;
	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_TOPOLOGYCHART;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_GET;
	strcpy(query.qstr,TopologyID.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;

	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		TopologyChart *pe=new TopologyChart();
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
OBJECT CreateTopologyChart(string TopologyID)
{
	if(TopologyID.empty())
		return INVALID_VALUE;

	try{
		TopologyChart *pe=new TopologyChart();
		if(pe==NULL)
			return INVALID_VALUE;
		pe->PutID(TopologyID);
		OBJECT ret=reinterpret_cast<OBJECT>(pe);
		return ret;
	}catch(...)
	{
		return INVALID_VALUE;
	}

	return INVALID_VALUE;

}

SVAPI_API
MAPNODE GetTopologyChartMainAttribNode(OBJECT topologyobj)
{
	if(topologyobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{
		TopologyChart *pe=reinterpret_cast<TopologyChart *>(topologyobj);
		MAPNODE ret=reinterpret_cast<MAPNODE>(&(pe->GetProperty()));
		return ret;

	}catch(...)
	{
		return INVALID_VALUE;
	}

		return INVALID_VALUE;

}

SVAPI_API
bool DeleteTopologyChart(string topologyid,string user,string addr)
{
	if(topologyid.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_DELETE;
	querybuf.datatype=S_TOPOLOGYCHART;
	strcpy(querybuf.qstr,topologyid.c_str());
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
bool CloseTopologyChart(OBJECT &topologyobj)
{
	if(topologyobj==INVALID_VALUE)
		return false;
	try{
		TopologyChart *pe=reinterpret_cast<TopologyChart *>(topologyobj);
		if(pe)
			delete pe;
		topologyobj=INVALID_VALUE;
	}catch(...)
	{
		return false;
	}

	return true;
}

SVAPI_API
bool SubmitTopologyChart(OBJECT topologyobj,string user,string addr)
{
	if(topologyobj==INVALID_VALUE)
		return false;
	if(user.empty()||addr.empty())
		return false;
	try{
		TopologyChart *pmtp=reinterpret_cast<TopologyChart *>(topologyobj);
		if(pmtp->GetID().empty())
		{
			printf("ID of TopologyChart is empty. svapi error.");
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
		querybuf.datatype=S_TOPOLOGYCHART;
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
bool GetAllTopologyChartInfo(PAIRLIST &retlist,string infoname,string user,string addr)
{
	if(user.empty()||addr.empty())
		return false;

	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;


	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_INFO;
	querybuf.datatype=S_TOPOLOGYCHART;
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



bool GetAllTopologyChartInfoFrom(string what, string key,string value,std::list<string> &idlist,string user,string addr)
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
	querybuf.datatype=S_TOPOLOGYCHART;
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
bool GetAllTopologyChartInfoInWords(string key,string value,std::list<string> &idlist,string user,string addr)
{
	return GetAllTopologyChartInfoFrom("word",key,value,idlist,user,addr);
}

SVAPI_API 
bool GetAllTopologyChartInfoInProperty(string key,string value,std::list<string> &idlist,string user,string addr)
{
	return GetAllTopologyChartInfoFrom("property",key,value,idlist,user,addr);
}

