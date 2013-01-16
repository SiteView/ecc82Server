#include "svapi.h"
#include <StringMap.h>
#include <MonitorTemplet.h>
#include "QueryData.h"
#include "ObjCache.h"

SVAPI_API
OBJECT	Cache_GetMonitorTemplet(int id)
{
	return ObjectCache::FastGetMonitorTPL(id);
}


SVAPI_API 
OBJECT	GetMonitorTemplet(int id,string user,string addr)
{
	if(id<=0)
		return INVALID_VALUE;
	if(user.empty()||addr.empty())
		return INVALID_VALUE;

	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;


	SVDBQUERY querybuf={0};
	char qs[20]={0};
	sprintf(qs,"%d",id);
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_GET;
	querybuf.datatype=S_MONITORTEMPLET;
	strcpy(querybuf.qstr,qs);
	strcpy(querybuf.idcuser,user.c_str());

    QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;
	if(qd.Query(&querybuf,(void **)&pdata,len,addr))
	{
		MonitorTemplet *pmtp=new MonitorTemplet();
		if(pmtp)
		{
			OBJECT ret=INVALID_VALUE;
			if(pdata!=NULL)
			{
				try{
					if(pmtp->CreateObjectByRawData(pdata,len))
						ret=reinterpret_cast<OBJECT>(pmtp);
					if(pdata!=NULL)
				    	delete [] pdata;
				}catch(...)
				{
					return INVALID_VALUE;
				}
				return ret;
			}
		}
	}
	
	return INVALID_VALUE;
}
SVAPI_API 
bool GetAllMonitorTempletInfo(PAIRLIST &retlist,string infoname,string user,string addr)
{
	if(user.empty()||addr.empty())
		return false;
	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;


	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_INFO;
	querybuf.datatype=S_MONITORTEMPLET;
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
MAPNODE	GetMTMainAttribNode(OBJECT mtobj)
{
	if(mtobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{
		MonitorTemplet *pmtp=reinterpret_cast<MonitorTemplet *>(mtobj);
		MAPNODE ret=reinterpret_cast<MAPNODE>(&(pmtp->GetProperty()));
		return ret;
	}catch(...)
	{
		return INVALID_VALUE;
	}

	return INVALID_VALUE;
}

SVAPI_API 
bool	FindMTParameterFirst(OBJECT mtobj,LISTITEM &item)
{
	if(mtobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{

		MonitorTemplet *pmtp=reinterpret_cast<MonitorTemplet *>(mtobj);
		STRMAPLIST &slist=pmtp->GetParameter();
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
bool	FindMTAdvanceParameterFirst(OBJECT mtobj,LISTITEM &item)
{
	if(mtobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{

		MonitorTemplet *pmtp=reinterpret_cast<MonitorTemplet *>(mtobj);
		STRMAPLIST &slist=pmtp->GetAdvanceParameter();
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
bool	FindMTReturnFirst(OBJECT mtobj,LISTITEM &item)
{
	if(mtobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{

		MonitorTemplet *pmtp=reinterpret_cast<MonitorTemplet *>(mtobj);
		STRMAPLIST &slist=pmtp->GetReturn();
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
MAPNODE	GetMTErrorAlertCondition(OBJECT mtobj)
{
	if(mtobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{

		MonitorTemplet *pmtp=reinterpret_cast<MonitorTemplet *>(mtobj);
		StringMap **pmap=pmtp->GetAlertCondition();
		if(pmap[0]==NULL)
			pmap[0]=new StringMap();

		MAPNODE ret=reinterpret_cast<MAPNODE>(pmap[0]);
		return ret;

	}catch(...)
	{
		return INVALID_VALUE;
	}

	return INVALID_VALUE;
}

SVAPI_API 
MAPNODE GetMTWarningAlertCondition(OBJECT mtobj)
{
	if(mtobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{

		MonitorTemplet *pmtp=reinterpret_cast<MonitorTemplet *>(mtobj);
		StringMap **pmap=pmtp->GetAlertCondition();
		if(pmap[1]==NULL)
			pmap[1]=new StringMap();
		MAPNODE ret=reinterpret_cast<MAPNODE>(pmap[1]);
		return ret;

	}catch(...)
	{
		return INVALID_VALUE;
	}

	return INVALID_VALUE;

}

SVAPI_API 
MAPNODE	GetMTGoodAlertCondition(OBJECT mtobj)
{
	if(mtobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{

		MonitorTemplet *pmtp=reinterpret_cast<MonitorTemplet *>(mtobj);
		StringMap **pmap=pmtp->GetAlertCondition();
		if(pmap[2]==NULL)
			pmap[2]=new StringMap();
		MAPNODE ret=reinterpret_cast<MAPNODE>(pmap[2]);
		return ret;

	}catch(...)
	{
		return INVALID_VALUE;
	}

	return INVALID_VALUE;
}

SVAPI_API
OBJECT CreateMonitorTemplet(int id)
{
	try{
		MonitorTemplet *pmt=new MonitorTemplet();
		if(pmt==NULL)
			return INVALID_VALUE;
		pmt->PutID(id);
		OBJECT ret=reinterpret_cast<OBJECT>(pmt);
		return ret;


	}catch(...)
	{
		return INVALID_VALUE;
	}

	return INVALID_VALUE;

}

SVAPI_API
void CloseMonitorTemplet(OBJECT &obj)
{
	if(obj==INVALID_VALUE)
		return;
	try{
		MonitorTemplet *pmt=reinterpret_cast<MonitorTemplet *>(obj);
		delete pmt;
		obj=INVALID_VALUE;
	}catch(...)
	{
		;
	}
}


/*
SVAPI_API 
bool	AddMTParameterItem(LISTITEM &item)
{
	return true;
}

SVAPI_API 
bool	AddAdvanceParameterItem(LISTITEM &item)
{
	return true;
}

SVAPI_API 
bool	AddAlertConditionItem(LISTITEM &item)
{
	return true;
}
*/

SVAPI_API
bool DeleteMonitorTemplet(int id,string user,string addr)
{
	if(id<=0)
		return false;
	if(user.empty()||addr.empty())
		return false;

	SVDBQUERY querybuf={0};
	char qs[20]={0};
	sprintf(qs,"%d",id);
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_DELETE;
	querybuf.datatype=S_MONITORTEMPLET;
	strcpy(querybuf.qstr,qs);
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
bool	SubmitMonitorTemplet(OBJECT mtobj,string user,string addr)
{
	if(mtobj==INVALID_VALUE)
		return false;
	try{
		MonitorTemplet *pmtp=reinterpret_cast<MonitorTemplet *>(mtobj);
		S_UINT len=pmtp->GetRawDataSize();
		svutil::buffer buf;
		if(!buf.checksize(len))
			return false;
		if(pmtp->GetRawData(buf,len)==NULL)
			return false;

		SVDBQUERY querybuf={0};
		char qs[20]={0};
		sprintf(qs,"%d",pmtp->GetID());
		querybuf.len = sizeof(SVDBQUERY);
		querybuf.querytype=QUERY_UPDATE;
		querybuf.datatype=S_MONITORTEMPLET;
		strcpy(querybuf.qstr,qs);
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
OBJECT CloneMonitorTemplet(OBJECT mtobj,int targetid)
{
	if(mtobj==INVALID_VALUE)
		return INVALID_VALUE;
	try{
		MonitorTemplet *pmtp=reinterpret_cast<MonitorTemplet *>(mtobj);
		S_UINT len=pmtp->GetRawDataSize();
		svutil::buffer buf;
		if(!buf.checksize(len))
			return INVALID_VALUE;
		if(pmtp->GetRawData(buf,len)==NULL)
			return INVALID_VALUE;

		MonitorTemplet *pclone=new MonitorTemplet();
		if(pclone)
		{
			if(pclone->CreateObjectByRawData(buf,len))
			{
				pclone->PutID(targetid);
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