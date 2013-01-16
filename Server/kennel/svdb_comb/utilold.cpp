#include "util.h"
#ifdef	WIN32
#include "windows.h"

#define KEY_PATH		"SOFTWARE\\SiteView\\SiteViewECC"
#define KEY_NAME		"root_path_7"
#endif
#include "libutil/Time.h"


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
	cout<<"    regedit key: "<<KEY_PATH<<"\\"<<KEY_NAME<<endl;
}

std::string GetRootPath()
{
	string strpath="";
	
	try{

#ifdef WIN32
	HKEY hKey = NULL;
	LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,KEY_PATH,0,KEY_READ,&hKey);
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

			if( (it->data)!=NULL )
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
	if((buf==NULL)||(buflen<sizeof(int)))
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
	string mse=svutil::TTime::GetCurrentTimeEx().Format();
	mse+="\t";
	mse+=error;
	mse+="\r\n";
	sprintf(buf,"%s/data/svdblog.log",GetRootPath().c_str());
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