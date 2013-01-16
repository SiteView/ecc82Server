#include "util.h"
#ifdef	WIN32
#include "windows.h"

#define KEY_PATH		"SOFTWARE\\SiteView\\SiteViewECC"
#define KEY_PATH_CORE	"SOFTWARE\\SiteView\\SiteViewCore"
#define KEY_NAME		"root_path_7"
#endif
#include "../libutil/Time.h"


std::string TrimSpace(const std::string & input)
{
	string ret=input;
	try{
					
		while( ret.find(" ")==0 )
			ret.erase(0,1);
		if(ret.empty())
			return ret;
		std::string::size_type lastpos;
		while( ret.rfind(" ")==(lastpos=(ret.size()-1)) )
			ret.erase(lastpos,1);
	}
	catch(...)
	{
		ret= input;
	}
	return ret;
}

std::string TruncateToUId(std::string id)
{
	std::string uid=id;
	std::string::size_type pos1=id.find(".");
	if(pos1 != std::string::npos)
	{
		std::string::size_type pos2=id.find(".",pos1+1);
		if(pos2 != std::string::npos)
			uid= id.erase(pos2);
	}
	return uid;
}

bool MoreThan2Dot(const std::string & id)
{
	std::string::size_type pos1=id.find(".");
	if(pos1 != std::string::npos)
	{
		std::string::size_type pos2=id.find(".",pos1+1);
		if(pos2 != std::string::npos)
		{
			if( id.find(".",pos2+1) != std::string::npos )
				return true;
		}
	}
	return false;
}


word GetParentID(word id)
{
	string str=id.getword();
	if(str.empty())
		return id;

	int pos=0;
	if((pos=str.rfind(IDSEPARATOR))<0)
		return id;

	string sub=str.substr(0,pos);
	return sub;

}
int GetIndexByID(word id)
{
	string str=id.getword();
	if(str.empty())
		return -1;
	int pos=0;
	if((pos=str.rfind(IDSEPARATOR))<0)
		return atoi(id.getword());
	string sub=str.substr(pos+1);

	return atoi(sub.c_str());

}

bool IsTopID(word id)
{
	word pid=GetParentID(id);
	return pid==id;
}

word GetTopID(word id)
{
	string str=id;
	if(str.empty())
		return id;

	int pos=0;
	if((pos=str.find(IDSEPARATOR))<0)
		return id;
	string sub=str.substr(0,pos);

	return sub;

}

word MakeIDByIndex(word pid,int index)
{
	char buf[512]={0};
	string str=pid.getword();
	if(str.empty())
	   sprintf(buf,"%d",index);
	else
	   sprintf(buf,"%s.%d",pid.getword(),index);
	return buf;
}

void outputRegeditKey(void)
{
	string path= GetRootPathByKey(KEY_PATH_CORE);
	if( !path.empty() )
		cout<<"    regedit key for svdb : "<<KEY_PATH_CORE<<"\\"<<KEY_NAME<<endl;
	else
	{
		path= GetRootPathByKey(KEY_PATH);
		if( !path.empty() )
			cout<<"    regedit key for svdb : "<<KEY_PATH<<"\\"<<KEY_NAME<<endl;
		else
			cout<<"    regedit key for svdb is invalid ! "<<endl;
	}

}

std::string GetRootPathFromKeyCore()
{
	string path= GetRootPathByKey(KEY_PATH_CORE);
	if(path.empty())
		path= GetRootPath();
	return path;
}

std::string GetRootPath()
{
	return GetRootPathByKey(KEY_PATH);
}

std::string GetRootPathByKey(string key)
{

	string strpath="";
	
	try{

#ifdef WIN32
	HKEY hKey = NULL;
	LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, key.c_str() ,0,KEY_READ,&hKey);
	if(lRet != ERROR_SUCCESS)
		return strpath;
	
	LPTSTR  lpName = NULL,
 			lpValue = NULL;
	TCHAR	strSubKey[1024]={0};
	TCHAR	strValKey[1024]={0};
	DWORD   dwIndex = 0,
			dwS1 = 1024,
			dwS2 = 1024,
			dwType = REG_SZ;
	BOOL    bFindFlag = FALSE;
	TCHAR szAppRoot[1024] = {0};
	while(lRet == ERROR_SUCCESS)
	{
		lpName = (LPTSTR)strSubKey;
		lpValue = (LPTSTR)strValKey;
		lRet = RegEnumValue(hKey, dwIndex++, lpName , &dwS1,NULL, &dwType ,(LPBYTE)lpValue, &dwS2);
		dwS1 = 1024;
		dwS2 = 1024;
		if(lRet == ERROR_NO_MORE_ITEMS)
			break;
		if(::_stricmp(strSubKey,KEY_NAME)==0)
		{
			bFindFlag = TRUE;
			strcpy(szAppRoot,strValKey);
			break;
		}
	}

	RegCloseKey(hKey);
	
	if(bFindFlag)
	{
		strpath=szAppRoot;
	}

#else
	strpath="root/sitview70";

#endif
	int pos=strpath.rfind("\\");
	if(pos==strpath.size()-1)
		strpath.erase(pos,1);
	
	}catch(...)
	{
	}
	
	return strpath;
}


void OutputForestMap(ForestMap & fmap)
{
	ForestMap::iterator fit;
	NodeData::iterator nit;
	int count(0);
	for(fit=fmap.begin(); fit!=fmap.end();  ++fit)
	{
		cout<<"\n "<<++count<<"th NodeData:  \""<<fit->first.c_str()<<"\""<<endl;
		for(nit=fit->second.begin(); nit!=fit->second.end(); ++nit)
			cout<<"         \""<<nit->first.c_str()<<"\" = \""<<nit->second.c_str()<<"\""<<endl;
	}
	cout<<"\n  ForestMap size: "<<fmap.size()<<"\n\n\n\n"<<endl;
}


S_UINT	GetForestMapRawDataSize(ForestMap & fmap)
{
	S_UINT len=0;

	try{
		std::list<SingelRecord> listrcd;
		for(ForestMap::iterator fit=fmap.begin(); fit!=fmap.end(); ++fit)
		{
			SingelRecord r1;
			r1.datalen= GetNodeDataRawDataSize(fit->second) ;
			r1.data= NULL; 
			r1.monitorid= fit->first;

			listrcd.push_back(r1);
		}
		len= GetMassRecordListRawDataSize(listrcd);
	}
	catch(...)
	{
		printf(" Exception in GetForestMapRawDataSize. ");
		len= 0;
	}
	return len;
}

char*   GetForestMapRawData(ForestMap & fmap,char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;

	try{
		S_UINT totalsize= GetForestMapRawDataSize(fmap);
		if(bufsize<totalsize)
			return NULL;

		S_UINT len=0,tlen=sizeof(S_UINT);
		char *pt=lpbuf;
		char *data(NULL);

		memmove(pt,&totalsize,tlen);
		pt+=tlen;

		len=fmap.size();
		memmove(pt,&len,tlen);
		pt+=tlen;

		data= pt;
		S_UINT nlen=0;
		std::list<SingelRecord> listrcd;
		for(ForestMap::iterator fit=fmap.begin(); fit!=fmap.end(); ++fit)
		{
			SingelRecord r1;
			nlen= GetNodeDataRawDataSize(fit->second) ;

			data+= strlen(fit->first.c_str()) + 1;
			data+= tlen;
			r1.data= GetNodeDataRawData(fit->second, data, nlen); 
			data+= nlen;
			
			r1.monitorid= fit->first;
			r1.datalen= nlen;

			listrcd.push_back(r1);
		}

		for(std::list<SingelRecord>::iterator it=listrcd.begin();it!=listrcd.end();it++)
		{
			len=strlen((it->monitorid).c_str());
			strcpy(pt,(it->monitorid).c_str());
			pt+=len;
			pt[0]='\0';
			pt++;

			memmove(pt,&(it->datalen),tlen);
			pt+=tlen;

			if( (it->data)!=NULL )
				memmove(pt,it->data,it->datalen);
			pt+=it->datalen;
		}
	}catch(...)
	{
		cout<<"Exception to GetForestMapRawData."<<endl;
		return NULL;
	}

	return lpbuf;
}

bool	CreateForestMapByRawData(ForestMap & fmap,const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;

	if(bufsize<sizeof(S_UINT))
		return false;
		
	S_UINT len=0,tlen=sizeof(S_UINT);

	try{
		fmap.clear();

		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;
		memmove(&len,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;

		if(len!=bufsize)
		{
			puts("Raw data of ForestMap error");
			return false;
		}

		S_UINT count;
		memmove(&count,pt,tlen);
		pt+=tlen;


		std::string str;
		S_UINT datalen(0);
		const char * data(NULL);

		for(S_UINT i=0;i<count;i++)
		{
			str=pt;
			pt+=strlen(str.c_str())+1;
			if(pt>pend)
				return true;

			memmove(&datalen,pt,tlen);
			pt+=tlen;
			if(pt>pend)
				return true;

			data=pt;

			pt+=datalen;
			if(pt>pend)
				return true;

			NodeData ndata;
			if( CreateNodeDataByRawData(ndata, data, datalen) )
				fmap.insert(std::make_pair( str,ndata ));	
		}
	}catch(...)
	{
		cout<<"Exception to CreateForestMapByRawData."<<endl;
		return false;
	}  
	return true;
}

void OutputNodeData(NodeData & ndata)
{
	NodeData::iterator nit;
	int count(0);

	for(nit= ndata.begin(); nit!= ndata.end(); ++nit)
	{
		cout<<"         \""<<nit->first.c_str()<<"\" = \""<<nit->second.c_str()<<"\""<<endl;
	}

	cout<<"\n  NodeData size: "<<ndata.size()<<"\n\n\n\n"<<endl;
}

S_UINT GetNodeDataRawDataSize(NodeData & ndata)
{
	S_UINT len=0,tlen=sizeof(S_UINT);
	len+=tlen + tlen;	
	NodeData::iterator it;
	for(it=ndata.begin();it!=ndata.end();it++)
	{
		len+=(S_UINT)strlen(it->first.c_str())+1;
		len+=(S_UINT)strlen(it->second.c_str())+1;
	}

	return len;
}

char*   GetNodeDataRawData(NodeData & ndata,char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;

	try{
		S_UINT totalsize=GetNodeDataRawDataSize(ndata);
		if(bufsize<totalsize)
			return NULL;

		S_UINT count=0,tlen=sizeof(S_UINT);
		char *pt=lpbuf;

		memmove(pt,&totalsize,tlen);
		pt+=tlen;

		count=ndata.size();
		memmove(pt,&count,tlen);
		pt+=tlen;

		NodeData::iterator it;
		S_UINT len=tlen*2;
		for(it=ndata.begin();it!=ndata.end();it++)
		{
			const char *p=it->first.c_str();
			len+=(S_UINT)strlen(p)+1;
			if(len<=bufsize)
			{
				strcpy(pt,p);
				pt+=(S_UINT)strlen(p);
				pt[0]='\0';
				pt++;
			}else
				return NULL;
			p=it->second.c_str();

			len+=(S_UINT)strlen(p)+1;
			if(len<=bufsize)
			{
				strcpy(pt,p);
				pt+=(S_UINT)strlen(p);
				pt[0]='\0';
				pt++;
			}else
				return NULL;

		}

		return lpbuf;
	}
	catch(...)
	{
		printf("\n Exception to GetNodeDataRawData \n");
		return NULL;
	}
}

bool	CreateNodeDataByRawData(NodeData & ndata,const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;

	if(bufsize<sizeof(S_UINT))
		return false;
		
	S_UINT len=0,tlen=sizeof(S_UINT);

	try{

		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;
		memmove(&len,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;

		if(len!=bufsize)
		{
			puts("Raw data of NodeData error");
			return false;
		}

		S_UINT count;
		memmove(&count,pt,tlen);
		pt+=tlen;


		std::string str1,str2;
		S_UINT datalen;
		char * data;

		for(S_UINT i=0;i<count;i++)
		{
			str1=pt;
			pt+=strlen(str1.c_str())+1;
			if(pt>pend)
				return true;

			str2=pt;
			pt+=strlen(str2.c_str())+1;
			if(pt>pend)
				return true;

			ndata.insert(std::make_pair(str1,str2));
		}
	}catch(...)
	{
		cout<<"Exception to CreateNodeDataByRawData."<<endl;
		return false;
	}
    
	return true;
}

S_UINT GetMassRecordListRawDataSize(std::list<SingelRecord> & listrcd)
{
	S_UINT len=0,tlen=sizeof(S_UINT);
	len+=tlen + tlen;	//size;
	std::list<SingelRecord>::iterator it;
	for(it=listrcd.begin();it!=listrcd.end();it++)
	{
		len+=strlen((it->monitorid).c_str())+1;
		len+=sizeof(unsigned int);
		len+=it->datalen;
	}

	return len;

}

char*   GetMassRecordListRawData(std::list<SingelRecord> & listrcd,char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;

	try{
		S_UINT totalsize=GetMassRecordListRawDataSize(listrcd);
		if(bufsize<totalsize)
			return NULL;

		S_UINT len=0,tlen=sizeof(S_UINT);
		char *pt=lpbuf;

		memmove(pt,&totalsize,tlen);
		pt+=tlen;

		len=listrcd.size();
		memmove(pt,&len,tlen);
		pt+=tlen;

		std::list<SingelRecord>::iterator it;
		for(it=listrcd.begin();it!=listrcd.end();it++)
		{
			len=strlen((it->monitorid).c_str());
			strcpy(pt,(it->monitorid).c_str());
			pt+=len;
			pt[0]='\0';
			pt++;

			memmove(pt,&(it->datalen),tlen);
			pt+=tlen;

			if( (it->data)!=NULL && it->datalen )
				memmove(pt,it->data,it->datalen);
			pt+=it->datalen;
		}
	}catch(...)
	{
		cout<<"Exception to GetMassRecordListRawData."<<endl;
		return NULL;
	}

	return lpbuf;
}

bool	CreateMassRecordListByRawData(std::list<SingelRecord> & listrcd,const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;

	if(bufsize<sizeof(S_UINT))
		return false;
		
	S_UINT len=0,tlen=sizeof(S_UINT);

	try{

		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;
		memmove(&len,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;

		if(len!=bufsize)
		{
			puts("Raw data error");
			return false;
		}

		S_UINT count;
		memmove(&count,pt,tlen);
		pt+=tlen;


		std::string str;
		S_UINT datalen;
		char * data;

		for(S_UINT i=0;i<count;i++)
		{
			str=pt;
			pt+=strlen(str.c_str())+1;
			if(pt>pend)
				return true;

			memmove(&datalen,pt,tlen);
			pt+=tlen;
			if(pt>pend)
				return true;

			SingelRecord arcd;
			arcd.monitorid=str;
			arcd.datalen=datalen;
			if(datalen)
				arcd.data=pt;

			pt+=datalen;
			if(pt>pend)
				return true;

			listrcd.push_back(arcd);
		}
	}catch(...)
	{
		cout<<"Exception to CreateMassRecordListByRawData."<<endl;
		return false;
	}
    
	return true;
}

S_UINT GetStrListRawDataSize(std::list<std::string> &strlist)
{
	S_UINT len=0,tlen=sizeof(S_UINT);
	len+=tlen;	//size;
	std::list<std::string>::iterator it;
	for(it=strlist.begin();it!=strlist.end();it++)
		len+=strlen((*it).c_str())+1;

	return len;

}

char*   GetStrListRawData(std::list<std::string> &strlist,char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;
	if(bufsize<GetStrListRawDataSize(strlist))
		return NULL;

	S_UINT len=0,tlen=sizeof(S_UINT);
	char *pt=lpbuf;
	
	len=strlist.size();
	memmove(pt,&len,tlen);
	pt+=tlen;

	std::list<std::string>::iterator it;
	for(it=strlist.begin();it!=strlist.end();it++)
	{
		len=strlen((*it).c_str());
		strcpy(pt,(*it).c_str());
		pt+=len;
		pt[0]='\0';
		pt++;
	}

	return lpbuf;
}

bool	CreateStrListByRawData(std::list<std::string> &strlist,const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;
		
	S_UINT len=0,tlen=sizeof(S_UINT);

	try{

		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;
		memmove(&len,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;

		std::string str;

		for(S_UINT i=0;i<len;i++)
		{
			str=pt;
			pt+=strlen(str.c_str())+1;
			if(pt>pend)
				return false;
			strlist.push_back(str);
		}
	}catch(...)
	{
		return false;
	}
    
	return true;
}

char *buildbuffer(int data,char *pt,int buflen)
{
	if(pt==NULL)
		return NULL;
	if(buflen<sizeof(int))
		return NULL;
	memmove(pt,&data,sizeof(int));
	pt+=sizeof(int);
	return pt;
}
char *buildbuffer(float data,char *pt,int buflen)
{
	if(pt==NULL)
		return NULL;
	if(buflen<sizeof(float))
		return NULL;
	memmove(pt,&data,sizeof(float));
	pt+=sizeof(float);
	return pt;

}

char *buildbuffer(std::string data,char *pt,int buflen)
{
	if(pt==NULL)
		return NULL;
	if(buflen<data.size()+1)
		return NULL;
	strcpy(pt,data.c_str());
	pt+=data.size();
	pt[0]='\0';
	pt++;
	return pt;

}

int parsebuffer(const char *buf,int buflen,int &ret)
{
	if((buf==NULL)||(buflen<sizeof(int)))
		return 0;
	const char *pt=buf;
	memmove(&ret,pt,sizeof(int));
	return sizeof(int);
}
int parsebuffer(const char *buf,int buflen,float &ret)
{
	if((buf==NULL)||(buflen<sizeof(float)))
		return 0;
	const char *pt=buf;
	memmove(&ret,pt,sizeof(float));
	return sizeof(float);
}
int parsebuffer(const char *buf,int buflen,std::string &ret)
{
	if((buf==NULL)||(buflen<1))
		return 0;
	const char *pt=buf;
	ret=pt;
	return strlen(ret.c_str())+1;
}
int ConvertType(std::string strtype)
{
	if(strtype.compare("String")==0)
	    return 3;
	if(strtype.compare("Int")==0)
		return 1;
	if(strtype.compare("Float")==0)
		return 2;
	return 0;
}

void AddToErrorLog(string error)
{
	char buf[1024]={0};
	string mse=IdcUser::ProcessID;
	mse+="  ";
	mse+=svutil::TTime::GetCurrentTimeEx().Format();
	mse+="\t";
	mse+=error;
	mse+="\r\n";
	sprintf(buf,"%s/data/svdblog.log",GetRootPathFromKeyCore().c_str());
	FILE *pf=fopen(buf,"a+");
	if(pf)
	{
		fputs(mse.c_str(),pf);
		fclose(pf);
	}
}


bool builddyn(const char *buf,int datalen,SVDYN &dyn,bool hasstr)
{
	int len=sizeof(SVDYN)-sizeof(int);
	if(datalen<sizeof(SVDYN)-sizeof(int))
		return false;

    const char *pt=buf;
	memmove(&dyn.m_time,pt,sizeof(TTime));
	pt+=sizeof(TTime);
	memmove(&dyn.m_state,pt,sizeof(int));
	pt+=sizeof(int);
	memmove(&dyn.m_keeplaststatetime,pt,sizeof(TTimeSpan));
	pt+=sizeof(TTimeSpan);
	memmove(&dyn.m_laststatekeeptimes,pt,sizeof(unsigned int));
	pt+=sizeof(unsigned int);
	if(hasstr)
	{
	   dyn.m_displaystr=_strdup(pt);
	}

	return true;
}