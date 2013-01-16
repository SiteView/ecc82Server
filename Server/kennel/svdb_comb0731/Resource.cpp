#include "resource.h"
#include "util.h"

Resource::Resource(void)
{
}

Resource::~Resource(void)
{
}

#include <sstream>
Resource::Resource(StringMap & inputmap,string keys)
{
	string dvalue=keys;
	for(string::size_type index=0; index !=dvalue.size(); ++index)
		if(dvalue[index]==',') dvalue[index]='\n';

	std::istringstream input_temp(dvalue);
	string tempkey;

	set<string> keyset; set<string>::iterator it;

	try{
		while(  std::getline(input_temp,tempkey)  )
		{
			tempkey=TrimSpace(tempkey);
			word *pv=NULL;
			if(!tempkey.empty() && ( pv=inputmap.find(tempkey.c_str()) )!=NULL)
				m_ResourceDict.insert(tempkey.c_str(), *pv);
		}
	}
	catch(...)
	{
		m_ResourceDict.clear();
		cout<<"Exception to creat fast-query-resource by keys."<<endl;
	}
}


S_UINT	Resource::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);

	len+=strlen(m_Language)+1;

	len+=tlen;
	len+=m_ResourceDict.GetRawDataSize();

	return len;


}
char*	Resource::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;
	if(bufsize<GetRawDataSize())
		return NULL;
	S_UINT len=0,tlen=sizeof(S_UINT);
	char *pt=lpbuf;
	strcpy(pt,m_Language);
	pt+=strlen(m_Language);
	pt[0]='\0';
	pt++;

	len=m_ResourceDict.GetRawDataSize();
	memmove(pt,&len,tlen);
	pt+=tlen;

	if(m_ResourceDict.GetRawData(pt,len)==NULL)
		return NULL;
	pt+=len;

	return lpbuf;


}
BOOL	Resource::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{
    if(lpbuf==NULL)
		return false;

	S_UINT len=0,tlen=sizeof(S_UINT);

	try{
		const char *pt=lpbuf;
		const char *pend=lpbuf+bufsize;

		m_Language=pt;
		pt+=strlen(m_Language)+1;
		if(pt>pend)
			return false;

		memmove(&len,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;

		if(!m_ResourceDict.CreateObjectByRawData(pt,len))
			return false;
		pt+=len;
	}catch(...)
	{
		return false;
	}


	return true;

}

