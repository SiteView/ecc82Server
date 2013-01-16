#include "svapi.h"
#include <StringMap.h>
#include <Section.h>
#include "QueryData.h"

SVAPI_API
OBJECT GetTask(string taskname,string user,string addr)
{
	if(taskname.empty()||user.empty()||addr.empty())
		return INVALID_VALUE;
	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;

	SVDBQUERY query={0};
	query.len=sizeof(SVDBQUERY);
	query.datatype=S_TASK;
	strcpy(query.idcuser,user.c_str());
	query.querytype=QUERY_GET;
	strcpy(query.qstr,taskname.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;

	if(qd.Query(&query,(void **)&pdata,len,addr))
	{
		Section *pe=new Section();
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
OBJECT CreateTask(string taskname)
{
	try{
		Section *pe=new Section();
		if(pe==NULL)
			return INVALID_VALUE;
		pe->PutSection(taskname);
		OBJECT ret=reinterpret_cast<OBJECT>(pe);
		return ret;
	}catch(...)
	{
		return INVALID_VALUE;
	}

	return INVALID_VALUE;
}

SVAPI_API
string GetTaskValue(string key,OBJECT taskobj)
{
	if(taskobj==INVALID_VALUE)
		return "";
	if(key.empty())
		return "";
	try{
		Section *pe=reinterpret_cast<Section *>(taskobj);
		word str;
		if(!pe->Pop(key,str))
			return "";
		return str.getword();

	}catch(...)
	{
		return "";
	}

	return "";
}

SVAPI_API
bool  SetTaskValue(string key,string value,OBJECT taskobj)
{
	if(taskobj==INVALID_VALUE)
		return false;
	if(key.empty())
		return false;
	try{
		Section *pe=reinterpret_cast<Section *>(taskobj);

		return pe->Push(key,value);

	}catch(...)
	{
		return false;
	}

	return false;
}

SVAPI_API
bool  DeleteTaskKey(string key,OBJECT taskobj)
{
	if(taskobj==INVALID_VALUE)
		return false;
	if(key.empty())
		return false;
	try{
		Section *pe=reinterpret_cast<Section *>(taskobj);

		return pe->Delete(key);

	}catch(...)
	{
		return false;
	}


	return false;
}

SVAPI_API
bool  DeleteTask(string taskname,string user,string addr)
{
	if(taskname.empty()||user.empty()||addr.empty())
		return false;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_DELETE;
	querybuf.datatype=S_TASK;
	strcpy(querybuf.qstr,taskname.c_str());
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
bool GetAllTaskName(std::list<string> &tasknamelist,string user,string addr)
{
	if(user.empty()||addr.empty())
		return false;

	if(user.size()>MAXUSERLEN)
		return INVALID_VALUE;


	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_INFO;
	querybuf.datatype=S_TASK;
//	strcpy(querybuf.qstr,infoname.c_str());
	strcpy(querybuf.idcuser,user.c_str());

	QueryData qd;
	S_UINT len=0;
	char *pdata=NULL;
	if(qd.Query(&querybuf,(void **)&pdata,len,addr))
	{
		if(pdata!=NULL)
		{
			try{

				tasknamelist.clear();
				if(::CreateStrListByRawData(tasknamelist,pdata,len))
				{
					delete [] pdata;
					return true;
				}
			}catch(...)
			{
				delete [] pdata;
				return false;
			}
			delete [] pdata;
		}
	}

	return false;
}

SVAPI_API
bool GetAllTaskKeyName(std::list<string> &keynamelist,OBJECT taskobj)
{
	if(taskobj==INVALID_VALUE)
		return false;

	try{
		Section *pmtp=reinterpret_cast<Section *>(taskobj);
		pmtp->GetKeys(keynamelist);
	}catch(...)
	{
		return false;
	}

	return true;

    
}

SVAPI_API
bool SubmitTask(OBJECT taskobj,string user,string addr)
{
	if(taskobj==INVALID_VALUE)
		return false;
	if(user.empty()||addr.empty())
		return false;
	try{
		Section *pmtp=reinterpret_cast<Section *>(taskobj);
		S_UINT len=pmtp->GetRawDataSize();
		svutil::buffer buf;
		if(!buf.checksize(len))
			return false;
		if(pmtp->GetRawData(buf,len)==NULL)
			return false;

		SVDBQUERY querybuf={0};
		querybuf.len = sizeof(SVDBQUERY);
		querybuf.querytype=QUERY_UPDATE;
		querybuf.datatype=S_TASK;
		strcpy(querybuf.qstr,pmtp->GetSection().getword());
		querybuf.datalen=len;
		strcpy(querybuf.idcuser,user.c_str());

		QueryData qd;
		SVDBRESULT ret={0};
		return qd.Update(&querybuf,buf,len,&ret,addr);
	}catch(...)
	{
		return false;
	}
	return false;
}
SVAPI_API
void CloseTask(OBJECT &taskobj)
{
	if(taskobj==INVALID_VALUE)
		return ;
	try{
		Section *pmtp=reinterpret_cast<Section *>(taskobj);
		if(pmtp)
			delete pmtp;
		taskobj=INVALID_VALUE;
	}catch(...)
	{
		return;
	}

	return;


}

SVAPI_API
bool EditTask(OBJECT taskobj,string taskname,string user,string addr)
{
	if(taskobj==INVALID_VALUE)
		return false;
	if(user.empty()||addr.empty())
		return false;

	if(taskname.empty())
		return SubmitTask(taskobj,user,addr);

	try{
		Section *pmtp=reinterpret_cast<Section *>(taskobj);
		if(taskname.compare(pmtp->GetSection().getword())==0)
			return SubmitTask(taskobj,user,addr);
		if(pmtp->Find("$newtaskname3721$"))
			return false;
		if(!pmtp->Push("$newsection3721$",taskname))
			return false;
		S_UINT len=pmtp->GetRawDataSize();
		svutil::buffer buf;
		if(!buf.checksize(len))
		{
			pmtp->Delete("$newsection3721$");
			return false;
		}
		if(pmtp->GetRawData(buf,len)==NULL)
		{
			pmtp->Delete("$newsection3721$");
			return false;
		}

		pmtp->Delete("$newsection3721$");

		SVDBQUERY querybuf={0};
		querybuf.len = sizeof(SVDBQUERY);
		querybuf.querytype=QUERY_EDIT;
		querybuf.datatype=S_TASK;
		strcpy(querybuf.qstr,pmtp->GetSection().getword());
		querybuf.datalen=len;
		strcpy(querybuf.idcuser,user.c_str());

		QueryData qd;
		SVDBRESULT ret={0};
		return qd.Update(&querybuf,buf,len,&ret,addr);
	}catch(...)
	{
		return false;
	}
	return false;

}

