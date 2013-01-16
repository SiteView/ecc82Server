#include "svapi.h"
#include <StringMap.h>
#include <util.h>
#include "QueryData.h"
#include "stlini.h"

unsigned short g_port=8600;


SVAPI_API 
bool SetSvdbAddrByFile(string addr)
{
	if(addr.empty())
	{
		IdcUser::svdbHostAddr.clear();
		return false;
	}
	INIFile inif=LoadIni(addr.c_str());

	string svalue=GetIniSetting(inif,"svapi","SvdbHostAddr","");
	if(!svalue.empty())
	{
		IdcUser::svdbHostAddr= svalue;
		return true;
	}
	IdcUser::svdbHostAddr.clear();
	return false;
}


SVAPI_API 
string GetSvdbAddr(void)
{
	return IdcUser::svdbHostAddr;
}


SVAPI_API 
bool SetCgiVersionByFile(string addr)
{
	if(addr.empty())
	{
		IdcUser::cgiVersion.clear();
		return false;
	}
	INIFile inif=LoadIni(addr.c_str());

	string svalue=GetIniSetting(inif,"cgiversion","cgiversion","");
	if(!svalue.empty())
	{
		IdcUser::cgiVersion= svalue;
		return true;
	}
	IdcUser::cgiVersion.clear();
	return false;
}


SVAPI_API 
string GetCgiVersion(void)
{
	return IdcUser::cgiVersion;
}


SVAPI_API
void SetServerPort(unsigned short port)
{
	g_port=port;
}

SVAPI_API
unsigned short GetServerPort()
{
	return g_port;
}


SVAPI_API 
bool	EnumNodeAttrib(MAPNODE node,PAIRLIST &retlist)
{
	if(node==INVALID_VALUE)
		return false;

	try{
		StringMap *pmap= reinterpret_cast< StringMap * >(node);
		SVPAIR pair;
		StringMap::iterator it;
		while(pmap->findnext(it))
		{
			pair.name=(*it).getkey().getword();
			pair.value=(*it).getvalue().getword();
			retlist.push_back(pair);
		}
	}catch(...)
	{
		return false;
	}


	return true;

}

SVAPI_API 
unsigned int GetNodeSize(MAPNODE node)
{
	if(node==INVALID_VALUE)
		return 0;

	try{
		StringMap *pmap= reinterpret_cast< StringMap * >(node);
		return pmap->size();
	}catch(...)
	{
		return 0;
	}

	return 0;
}

SVAPI_API 
MAPNODE FindNext(const LISTITEM &item)
{
	if((item.iterator==INVALID_VALUE)||(item.list==INVALID_VALUE))
		return INVALID_VALUE;
	try{

		STRMAPLIST *plist=reinterpret_cast< STRMAPLIST * >(item.list);
		STRMAPLIST::iterator *it=reinterpret_cast< STRMAPLIST::iterator * >(item.iterator);
//		if((*it)!=plist->begin())
//			(*it)++;

		if((*it)!=plist->end())
		{
			MAPNODE ret=reinterpret_cast<MAPNODE>((*(*it)));
			(*it)++;
			return ret;
		}else{

//			item.iterator=INVALID_VALUE;
//			item.list=INVALID_VALUE;
			return INVALID_VALUE;
		}

	}catch(...)
	{
		return INVALID_VALUE;
	}

	return INVALID_VALUE;
}

SVAPI_API 
bool	ResetListItem(const LISTITEM &item)
{
	if((item.iterator==INVALID_VALUE)||(item.list==INVALID_VALUE))
		return false;
	try{

		STRMAPLIST *plist=reinterpret_cast< STRMAPLIST * >(item.list);
		STRMAPLIST::iterator *it=reinterpret_cast< STRMAPLIST::iterator * >(item.iterator);
		(*it)=plist->begin();
		return true;
	}catch(...)
	{
		return false;
	}


}

SVAPI_API 
bool	FindNodeValue(MAPNODE node,string name,string &value)
{
	if(node==INVALID_VALUE)
		return false;

	try{

		StringMap *pmap= reinterpret_cast< StringMap * >(node);
		word *pv=NULL;
		if((pv=pmap->find(name.c_str()))==NULL)
			return false;
		else
		{
			value=pv->getword();
			return true;
		}
		    
	}catch(...)
	{
		return false;
	}

	return true;
}

SVAPI_API 
bool	DeleteNodeAttrib(MAPNODE node,string name)
{
	if(node==INVALID_VALUE)
		return false;
	try{

		StringMap *pmap= reinterpret_cast< StringMap * >(node);
		return pmap->erase(name.c_str());
	}catch(...)
	{
		return false;
	}

	return true;
}

SVAPI_API 
bool	AddNodeAttrib(MAPNODE node,const SVPAIR &data)
{
	if(node==INVALID_VALUE)
		return false;
	try{

		StringMap *pmap= reinterpret_cast< StringMap * >(node);
		pmap->insert(data.name.c_str(),data.value.c_str());
		return true;

	}catch(...)
	{
		return false;
	}

	return true;
}

SVAPI_API 
bool	AddNodeAttrib(MAPNODE node,string name,string value)
{
	if(node==INVALID_VALUE)
		return false;
	try{

		StringMap *pmap= reinterpret_cast< StringMap * >(node);
		pmap->insert(name.c_str(),value.c_str());
		return true;

	}catch(...)
	{
		return false;
	}

	return true;
}

/*
LISTITEM
*/

SVAPI_API 
unsigned int GetListItemSize(const LISTITEM &item)
{
	if(item.list==INVALID_VALUE)
		return 0;
	try{

		STRMAPLIST *plist=reinterpret_cast< STRMAPLIST * >(item.list);
	//	STRMAPLIST::iterator *it=reinterpret_cast< STRMAPLIST::iterator * >(item.iterator);
		return plist->size();
	}catch(...)
	{
		return 0;
	}

	return 0;
}

SVAPI_API 
bool	DeleteItem(const LISTITEM &item)
{
	if((item.list==INVALID_VALUE)||(item.iterator==INVALID_VALUE))
		return false;
	try{

		STRMAPLIST *plist=reinterpret_cast< STRMAPLIST * >(item.list);
		STRMAPLIST::iterator *it=reinterpret_cast< STRMAPLIST::iterator * >(item.iterator);
		if((*it)!=plist->begin())
			(*it)--;

		if((*it)==plist->end())
			return false;
	
		(*it)=plist->erase((*it));
		return true;

	}catch(...)
	{
		return false;
	}



	return true;
}

SVAPI_API 
MAPNODE	CreateNewMapNode()
{
	StringMap *pmap=new StringMap();
	if(pmap==NULL)
		return INVALID_VALUE;
	MAPNODE ret=reinterpret_cast<MAPNODE>(pmap);
	return ret;
}

SVAPI_API 
MAPNODE	GetItemNode(const LISTITEM &item)
{
	if((item.iterator==INVALID_VALUE)||(item.list==INVALID_VALUE))
		return INVALID_VALUE;
	try{

		STRMAPLIST *plist=reinterpret_cast< STRMAPLIST * >(item.list);
		STRMAPLIST::iterator *it=reinterpret_cast< STRMAPLIST::iterator * >(item.iterator);
		if((*it)!=plist->end())
		{
			MAPNODE ret=reinterpret_cast<MAPNODE>((*(*it)));
			return ret;
		}
	}catch(...)
	{
		return INVALID_VALUE;
	}

	return INVALID_VALUE;
}

SVAPI_API 
bool	InsertItemToList(const LISTITEM &listi,const MAPNODE item)
{
	if((listi.iterator==INVALID_VALUE)||(item==INVALID_VALUE))
		return false;
	try{

		STRMAPLIST *plist=reinterpret_cast< STRMAPLIST * >(listi.list);
		STRMAPLIST::iterator *it=reinterpret_cast< STRMAPLIST::iterator * >(listi.iterator);
		StringMap *pmap=reinterpret_cast< StringMap *>(item);
		if((*it)!=plist->begin())
			(*it)--;
		plist->insert((*it),pmap);
		return true;
	}catch(...)
	{
		return false;
	}
	return true;

}

SVAPI_API
bool	AddItemToList(const LISTITEM &listi,const MAPNODE item)
{
	if((listi.iterator==INVALID_VALUE)||(item==INVALID_VALUE))
		return false;
	try{

		STRMAPLIST *plist=reinterpret_cast< STRMAPLIST * >(listi.list);
		StringMap *pmap=reinterpret_cast< StringMap *>(item);
		plist->push_back(pmap);
		return true;
	}catch(...)
	{
		return false;
	}
	return true;

}


SVAPI_API
bool	CreateNewItemList(LISTITEM&listi)
{
	return false;
}

SVAPI_API
void	ReleaseItemList(const LISTITEM &listi)
{
	if(listi.list==INVALID_VALUE)
		return;
	if(listi.iterator==INVALID_VALUE)
		return;
	try{
		STRMAPLIST::iterator *it=reinterpret_cast< STRMAPLIST::iterator * >(listi.iterator);
		delete it;
	}catch(...)
	{
		;
	}

}


/*
OBJECT
*/

SVAPI_API 
void	CloseMapNodeObject(OBJECT &obj)
{
	if(obj==INVALID_VALUE)
		return;
	try{

		StringMap *p= reinterpret_cast< StringMap * >(obj);
		if(p)
			delete p;
		obj=INVALID_VALUE;

	}catch(...)
	{
		puts("Close error");
	}

}

/*
通用相关
*/



SVAPI_API 
string	GetSiteViewRootPath()
{
	return ::GetRootPath();
}

SVAPI_API
string	FindParentID(string id)
{
	return ::GetParentID(id).getword();
}

SVAPI_API
int	FindIndexByID(string id)
{
	return ::GetIndexByID(id);
}

SVAPI_API
bool IsSVSEID(string id)
{
	return ::IsTopID(id);
}

SVAPI_API
string FindSEID(string id)
{
	return ::GetTopID(id).getword();
}


SVAPI_API
bool WriteIniFileString(string section,string key,string str,string filename,string addr,string user)
{
	if(section.empty()||key.empty()||filename.empty()||addr.empty())
		return false;
	if((section.size()>MAXSECTIONLEN)||(key.size()>MAXKEYLEN))
		return false;

	S_UINT len=0;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_UPDATE;
	querybuf.datatype=S_INIFILE;
	strcpy(querybuf.qstr,filename.c_str());
	strcpy(querybuf.idcuser,user.c_str());

	INIQUERY iquery={0};
	iquery.len=sizeof(INIQUERY);
	strcpy(iquery.section,section.c_str());
	strcpy(iquery.key,key.c_str());
	iquery.datatype=D_STRING;
	iquery.datalen=str.size()+1;


	len+=sizeof(INIQUERY);
	len+=str.size()+1;

	querybuf.datalen=len;

	buffer buf;
	if(!buf.checksize(len))
		return false;
	char *pt=buf.getbuffer();
	memcpy(pt,&iquery,sizeof(INIQUERY));
	pt+=sizeof(INIQUERY);
	strcpy(pt,str.c_str());
	pt+=str.size();
	pt[0]='\0';

	QueryData qd;
	SVDBRESULT ret={0};
	return qd.Update(&querybuf,buf,len,&ret,addr);
}

SVAPI_API
bool WriteIniFileInt(string section,string key,int value,string filename,string addr,string user)
{
	if(section.empty()||key.empty()||filename.empty()||addr.empty())
		return false;
	if((section.size()>MAXSECTIONLEN)||(key.size()>MAXKEYLEN))
		return false;

	S_UINT len=0;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_UPDATE;
	querybuf.datatype=S_INIFILE;
	strcpy(querybuf.qstr,filename.c_str());
	strcpy(querybuf.idcuser,user.c_str());

	INIQUERY iquery={0};
	iquery.len=sizeof(INIQUERY);
	strcpy(iquery.section,section.c_str());
	strcpy(iquery.key,key.c_str());
	iquery.datatype=D_INT;
	iquery.datalen=sizeof(int);


	len+=sizeof(INIQUERY);
	len+=sizeof(int);

	querybuf.datalen=len;

	buffer buf;
	if(!buf.checksize(len))
		return false;
	char *pt=buf.getbuffer();
	memcpy(pt,&iquery,sizeof(INIQUERY));
	pt+=sizeof(INIQUERY);
	memmove(pt,&value,sizeof(int));

	QueryData qd;
	SVDBRESULT ret={0};
	return qd.Update(&querybuf,buf,len,&ret,addr);
}

SVAPI_API
bool WriteIniFileStruct(string section,string key, void *data,unsigned int len,string filename,string addr,string user)
{
	if(section.empty()||key.empty()||filename.empty()||addr.empty())
		return false;
	if((section.size()>MAXSECTIONLEN)||(key.size()>MAXKEYLEN))
		return false;

	S_UINT dlen=0;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_UPDATE;
	querybuf.datatype=S_INIFILE;
	strcpy(querybuf.qstr,filename.c_str());
	strcpy(querybuf.idcuser,user.c_str());

	INIQUERY iquery={0};
	iquery.len=sizeof(INIQUERY);
	strcpy(iquery.section,section.c_str());
	strcpy(iquery.key,key.c_str());
	iquery.datatype=D_BINARY;
	iquery.datalen=len;

	dlen+=sizeof(INIQUERY);
	dlen+=len;

	querybuf.datalen=dlen;

	buffer buf;
	if(!buf.checksize(dlen))
		return false;
	char *pt=buf.getbuffer();
	memcpy(pt,&iquery,sizeof(INIQUERY));
	pt+=sizeof(INIQUERY);
	memmove(pt,data,len);

	QueryData qd;
	SVDBRESULT ret={0};
	return qd.Update(&querybuf,buf,dlen,&ret,addr);
}

SVAPI_API
string GetIniFileString(string section,string key,string defaultret,string filename,string addr,string user)
{
	if(section.empty()||key.empty()||filename.empty()||addr.empty())
		return defaultret;
	if((section.size()>MAXSECTIONLEN)||(key.size()>MAXKEYLEN))
		return defaultret;

	S_UINT len=0;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_GET;
	querybuf.datatype=S_INIFILE;
	strcpy(querybuf.qstr,filename.c_str());
	querybuf.datalen=sizeof(INIQUERY);

	strcpy(querybuf.idcuser,user.c_str());

	INIQUERY iquery={0};
	iquery.len=sizeof(INIQUERY);
	strcpy(iquery.section,section.c_str());
	strcpy(iquery.key,key.c_str());
	iquery.datatype=D_STRING;
	if(defaultret.size()>MAXDEFAULT)
	{
		string substr=defaultret.substr(0,MAXDEFAULT-1);
		sprintf(iquery.defaultret,"%s",substr.c_str());
	}else
		sprintf(iquery.defaultret,"%s",defaultret.c_str());

	QueryData qd;
	S_UINT rlen=0;
	char *pdata=NULL;

	if(qd.Query(&querybuf,&iquery,(void **)&pdata,rlen,addr))
	{
		if(pdata!=NULL)
		{
		//	buffer buf;
			string strret=pdata;
			delete [] pdata;
			return strret;
		}
	}

	return defaultret;
}

SVAPI_API
int	GetIniFileInt(string section,string key,int defaultret,string filename,string addr,string user)
{
	if(section.empty()||key.empty()||filename.empty()||addr.empty())
		return defaultret;
	if((section.size()>MAXSECTIONLEN)||(key.size()>MAXKEYLEN))
		return defaultret;

	S_UINT len=0;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_GET;
	querybuf.datatype=S_INIFILE;
	strcpy(querybuf.qstr,filename.c_str());
	querybuf.datalen=sizeof(INIQUERY);

	strcpy(querybuf.idcuser,user.c_str());

	INIQUERY iquery={0};
	iquery.len=sizeof(INIQUERY);
	strcpy(iquery.section,section.c_str());
	strcpy(iquery.key,key.c_str());
	iquery.datatype=D_INT;
	sprintf(iquery.defaultret,"%d",defaultret);

	QueryData qd;
	S_UINT rlen=0;
	char *pdata=NULL;

	if(qd.Query(&querybuf,&iquery,(void **)&pdata,rlen,addr))
	{
		if((pdata!=NULL)&&(rlen==sizeof(int)))
		{
			int nret=0;
			memmove(&nret,pdata,sizeof(int));
			delete [] pdata;
			return nret;
		}
		if(pdata!=NULL)
			delete [] pdata;
	}

	return defaultret;

}

SVAPI_API
bool GetIniFileStruct(string section,string key,void *buf,unsigned int &len,string filename,string addr,string user)
{
	if(section.empty()||key.empty()||filename.empty()||addr.empty())
		return false;
	if((section.size()>MAXSECTIONLEN)||(key.size()>MAXKEYLEN))
		return false;


	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_GET;
	querybuf.datatype=S_INIFILE;
	strcpy(querybuf.qstr,filename.c_str());
	querybuf.datalen=sizeof(INIQUERY);

	strcpy(querybuf.idcuser,user.c_str());

	INIQUERY iquery={0};
	iquery.len=sizeof(INIQUERY);
	strcpy(iquery.section,section.c_str());
	strcpy(iquery.key,key.c_str());
	if(buf!=NULL)
		iquery.datatype=D_BINARY;
	else
		iquery.datatype=D_BINARYLEN;


	QueryData qd;
	S_UINT rlen=0;
	char *pdata=NULL;

	if(qd.Query(&querybuf,&iquery,(void **)&pdata,rlen,addr))
	{
		if(buf==NULL)
		{
			if((pdata!=NULL)&&(rlen==sizeof(unsigned int)))
			{
				memmove(&len,pdata,sizeof(unsigned int));
				delete [] pdata;
				return true;
			}
		}else
		{
			if((pdata!=NULL)&&(rlen>=sizeof(unsigned int)))
			{
				len=rlen;
				memmove(buf,pdata,len);
				delete [] pdata;
				return true;
			}
		}
	}

	return false;
}

SVAPI_API
bool DeleteIniFileSection(string section,string filename,string addr,string user)
{
	if(section.empty()||filename.empty()||addr.empty())
		return false;
	if(section.size()>MAXSECTIONLEN)
		return false;

	S_UINT dlen=0;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_DELETE;
	querybuf.datatype=S_INIFILE;
	strcpy(querybuf.qstr,filename.c_str());
	strcpy(querybuf.idcuser,user.c_str());

	INIQUERY iquery={0};
	iquery.len=sizeof(INIQUERY);
	strcpy(iquery.section,section.c_str());
	//strcpy(iquery.key,key.c_str());

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
}

SVAPI_API
bool DeleteIniFileKey(string section,string key,string filename,string addr,string user)
{
	if(section.empty()||key.empty()||filename.empty()||addr.empty())
		return false;
	if((section.size()>MAXSECTIONLEN)||(key.size()>MAXKEYLEN))
		return false;

	S_UINT dlen=0;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_DELETE;
	querybuf.datatype=S_INIFILE;
	strcpy(querybuf.qstr,filename.c_str());
	strcpy(querybuf.idcuser,user.c_str());

	INIQUERY iquery={0};
	iquery.len=sizeof(INIQUERY);
	strcpy(iquery.section,section.c_str());
	strcpy(iquery.key,key.c_str());

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
	return true;
}

SVAPI_API
bool GetIniFileSections(std::list<string> &sectionlist,string filename,string addr,string user)
{
	if(filename.empty()||addr.empty())
		return false;
	S_UINT len=0;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_GET;
	querybuf.datatype=S_INIFILE;
	strcpy(querybuf.qstr,filename.c_str());
	querybuf.datalen=sizeof(INIQUERY);

	strcpy(querybuf.idcuser,user.c_str());

	INIQUERY iquery={0};
	iquery.len=sizeof(INIQUERY);
	iquery.datatype=D_SECTIONCOUNT;

	QueryData qd;
	S_UINT rlen=0;
	char *pdata=NULL;

	if(qd.Query(&querybuf,&iquery,(void **)&pdata,rlen,addr))
	{
//		StringMap strm;
		try{
			sectionlist.clear();
			if(::CreateStrListByRawData(sectionlist,pdata,rlen))
			{
			    delete [] pdata;
				return true;
			}
/*			if(strm.CreateObjectByRawData(pdata,rlen))
			{
			    delete pdata;
				StringMap::iterator it;
				while(strm.findnext(it))
					sectionlist.push_back((*it).getkey().getword());
				return true;
			}
*/
			if(pdata!=NULL)
		     	delete [] pdata;

		}catch(...)
		{
			return false;
		}

	}

	return false;
}

SVAPI_API
bool GetIniFileKeys(string section,std::list<string> &keylist,string filename,string addr,string user)
{
	if(filename.empty()||addr.empty()||section.empty())
		return false;
	if(section.size()>MAXSECTIONLEN)
		return false;

	S_UINT len=0;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_GET;
	querybuf.datatype=S_INIFILE;
	strcpy(querybuf.qstr,filename.c_str());
	querybuf.datalen=sizeof(INIQUERY);
	strcpy(querybuf.idcuser,user.c_str());

	INIQUERY iquery={0};
	iquery.len=sizeof(INIQUERY);
	strcpy(iquery.section,section.c_str());
	iquery.datatype=D_KEYCOUNT;

	QueryData qd;
	S_UINT rlen=0;
	char *pdata=NULL;

	if(qd.Query(&querybuf,&iquery,(void **)&pdata,rlen,addr))
	{
//		StringMap strm;
		try{
			keylist.clear();
			if(::CreateStrListByRawData(keylist,pdata,rlen))
			{
			    delete [] pdata;
				return true;
			}

/*			if(strm.CreateObjectByRawData(pdata,rlen))
			{
				delete pdata;

				StringMap::iterator it;
				while(strm.findnext(it))
					keylist.push_back((*it).getkey().getword());
				return true;
			}
*/
			if(pdata)
				delete [] pdata;

		}catch(...)
		{
			return false;
		}

	}

	return false;

}
SVAPI_API
int	GetIniFileValueType(string section,string key,string filename,string addr,string user)
{
	if(filename.empty()||addr.empty()||section.empty())
		return INIVALUE_FAILED;
	if(section.size()>MAXSECTIONLEN)
		return INIVALUE_FAILED;

	if(key.size()>MAXKEYLEN)
		return INIVALUE_FAILED;

	S_UINT len=0;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_GET;
	querybuf.datatype=S_INIFILE;
	strcpy(querybuf.qstr,filename.c_str());
	querybuf.datalen=sizeof(INIQUERY);
	strcpy(querybuf.idcuser,user.c_str());

	INIQUERY iquery={0};
	iquery.len=sizeof(INIQUERY);
	strcpy(iquery.section,section.c_str());
	strcpy(iquery.key,key.c_str());
	iquery.datatype=D_VALUETYPE;

	QueryData qd;
	S_UINT rlen=0;
	char *pdata=NULL;

	if(qd.Query(&querybuf,&iquery,(void **)&pdata,rlen,addr))
	{
		if(pdata)
		{
			if(rlen==sizeof(int))
			{
				int ret=0;
				memmove(&ret,pdata,sizeof(int));
				delete [] pdata;
				return ret;
			}
			delete [] pdata;
		}
	}

	return INIVALUE_FAILED;

}

SVAPI_API
bool EditIniFileSection(string oldsection,string newsection,string filename,string addr,string user)
{
	if(oldsection.empty()||filename.empty()||addr.empty()||newsection.empty())
		return false;
	if((oldsection.size()>MAXSECTIONLEN)||(newsection.size()>MAXSECTIONLEN))
		return false;

	S_UINT dlen=0;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_EDIT;
	querybuf.datatype=S_INIFILE;
	strcpy(querybuf.qstr,filename.c_str());
	strcpy(querybuf.idcuser,user.c_str());

	INIQUERY iquery={0};
	iquery.len=sizeof(INIQUERY);
	strcpy(iquery.section,oldsection.c_str());
	iquery.datatype=D_SECTION;
	iquery.datalen=newsection.size()+1;
	//strcpy(iquery.key,key.c_str());

	dlen+=sizeof(INIQUERY);
	dlen+=newsection.size()+1;

	querybuf.datalen=dlen;

	buffer buf;
	if(!buf.checksize(dlen))
		return false;
	char *pt=buf.getbuffer();
	memcpy(pt,&iquery,sizeof(INIQUERY));
	pt+=sizeof(INIQUERY);
	strcpy(pt,newsection.c_str());
    
	QueryData qd;
	SVDBRESULT ret={0};
	return qd.Update(&querybuf,buf,dlen,&ret,addr);


}

SVAPI_API
bool EditIniFileKey(string section,string oldkey,string newkey,string filename,string addr,string user)
{
	if(section.empty()||filename.empty()||addr.empty())
		return false;
	if((section.size()>MAXSECTIONLEN)||(oldkey.size()>MAXKEYLEN)||(newkey.size()>MAXKEYLEN))
		return false;
	

	S_UINT dlen=0;

	SVDBQUERY querybuf={0};
	querybuf.len = sizeof(SVDBQUERY);
	querybuf.querytype=QUERY_EDIT;
	querybuf.datatype=S_INIFILE;
	strcpy(querybuf.qstr,filename.c_str());
	strcpy(querybuf.idcuser,user.c_str());

	INIQUERY iquery={0};
	iquery.len=sizeof(INIQUERY);
	strcpy(iquery.section,section.c_str());
	strcpy(iquery.key,oldkey.c_str());
	iquery.datatype=D_KEY;
	iquery.datalen=oldkey.size()+1;

	dlen+=sizeof(INIQUERY);
	dlen+=newkey.size()+1;

	querybuf.datalen=dlen;

	buffer buf;
	if(!buf.checksize(dlen))
		return false;
	char *pt=buf.getbuffer();
	memcpy(pt,&iquery,sizeof(INIQUERY));
	pt+=sizeof(INIQUERY);
	strcpy(pt,newkey.c_str());
    
	QueryData qd;
	SVDBRESULT ret={0};
	return qd.Update(&querybuf,buf,dlen,&ret,addr);

}


SVAPI_API
MAPNODE CloneMapNode(MAPNODE node)
{
	if(node==INVALID_VALUE)
		return INVALID_VALUE;
	try{
		StringMap *pmtp=reinterpret_cast<StringMap *>(node);
		S_UINT len=pmtp->GetRawDataSize();
		svutil::buffer buf;
		if(!buf.checksize(len))
			return INVALID_VALUE;
		if(pmtp->GetRawData(buf,len)==NULL)
			return INVALID_VALUE;

		StringMap *pclone=new StringMap();
		if(pclone)
		{
			if(pclone->CreateObjectByRawData(buf,len))
			{
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


