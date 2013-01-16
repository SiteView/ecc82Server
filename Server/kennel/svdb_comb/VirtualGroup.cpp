#include "VirtualGroup.h"

VirtualGroup::VirtualGroup(void)
{
}

VirtualGroup::~VirtualGroup(void)
{
}

S_UINT	VirtualGroup::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);
	len+=tlen;  //datalen;

	len+=(S_UINT)strlen(m_ID)+1;
	len+=(S_UINT)strlen(m_Label)+1;
	len+=(S_UINT)strlen(m_Description)+1;
	len+=tlen;	//m_Property.size();
	len+=m_Property.GetRawDataSize();

	len+=tlen;	//m_EntityList.size();
	WORDLIST::iterator it;
	for(it=m_EntityList.begin();it!=m_EntityList.end();it++)
	{
		len+=(S_UINT)strlen((*it).getword())+1;
	}

	return len;


}
char*	VirtualGroup::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;
	if(bufsize<GetRawDataSize())
		return NULL;
	S_UINT len=0,tlen=sizeof(S_UINT);
	char *pt=lpbuf;

	len=GetRawDataSize();
	memmove(pt,&len,tlen);
	pt+=tlen;

	if(m_ID.empty())
		return NULL;

	strcpy(pt,m_ID);
	pt+=strlen(m_ID)+1;

	strcpy(pt,m_Label);
	pt+=strlen(m_Label)+1;

	strcpy(pt,m_Description);
	pt+=strlen(m_Description)+1;

	len=m_Property.GetRawDataSize();
	memmove(pt,&len,tlen);
	pt+=tlen;

	if(m_Property.GetRawData(pt,len)==NULL)
		return NULL;
	pt+=len;

	S_UINT count=(S_UINT)m_EntityList.size();
	memmove(pt,&count,tlen);
	pt+=tlen;

	WORDLIST::iterator it;
	for(it=m_EntityList.begin();it!=m_EntityList.end();it++)
	{
		strcpy(pt,(*it).getword());
		pt+=strlen((*it).getword());
		pt[0]='\0';
		pt++;
	}


	return lpbuf;
}
BOOL	VirtualGroup::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;
		
	S_UINT len=0,tlen=sizeof(S_UINT);

	const char *pend=lpbuf+bufsize;
	const char *pt=lpbuf;

	try{
		memmove(&len,pt,tlen);
		if(len!=bufsize)
			return false;
		pt+=tlen;

		m_ID=pt;
		pt+=strlen(m_ID)+1;

		m_Label=pt;
		pt+=strlen(m_Label)+1;

		m_Description=pt;
		pt+=strlen(m_Description)+1;

		memmove(&len,pt,tlen);
		pt+=tlen;

		if(!m_Property.CreateObjectByRawData(pt,len))
			return false;
		pt+=len;

		S_UINT count=0;
		memmove(&count,pt,tlen);
		pt+=tlen;

		svutil::word wd;

		for(S_UINT i=0;i<count;i++)
		{
			wd=pt;
			pt+=strlen(wd)+1;
			if(pt>pend)
				return false;

			m_EntityList.push_back(wd);
			
		}

	}catch(...)
	{
		return false;
	}

	return true;
}

