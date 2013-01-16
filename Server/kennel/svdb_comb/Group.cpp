#include "Group.h"

Group::Group(void)
{
	m_CurrentID=0;
}

Group::~Group(void)
{
}

S_UINT Group::GetRawDataSize( bool onlyLocked)
{
	S_UINT len=0,tlen=sizeof(S_UINT);

	len+=tlen;

	len+=strlen(m_ID)+1;

	len+=tlen;
	len+=m_Property.GetRawDataSize();

	//m_SubGroups.size();
	len+=tlen;
	WORDLIST::iterator it;
	for(it=m_SubGroups.begin();it!=m_SubGroups.end();it++)
	{
		if(onlyLocked && !IdcUser::WillTeleBackup( (*it).getword()) )
			continue;
		len+=strlen((*it))+1;
	}

	len+=tlen;
	for(it=m_Entitys.begin();it!=m_Entitys.end();it++)
	{
		if(onlyLocked && !IdcUser::WillTeleBackup( (*it).getword()) )
			continue;
		len+=strlen((*it))+1;
	}

	return len;
}
char* Group::GetRawData(char *lpbuf,S_UINT bufsize, bool onlyLocked)
{
	if(lpbuf==NULL)
		return NULL;

	if(bufsize<GetRawDataSize(onlyLocked))
		return NULL;

	char *pt=lpbuf;
	S_UINT len=0,tlen=sizeof(S_UINT);

	memmove(pt,&m_CurrentID,tlen);
	pt+=tlen;

	strcpy(pt,m_ID);
	pt+=strlen(m_ID);
	pt[0]='\0';
	pt++;

	len=m_Property.GetRawDataSize();
	memmove(pt,&len,tlen);
	pt+=tlen;
	if(m_Property.GetRawData(pt,len)==NULL)
		return NULL;
	pt+=len;

	len=m_SubGroups.size();
	if(onlyLocked)
	{
		S_UINT tempcount=0;
		WORDLIST::iterator tempit;
		for(tempit=m_SubGroups.begin();tempit!=m_SubGroups.end();tempit++)
		{
			if(IdcUser::WillTeleBackup( (*tempit).getword()) )
				++tempcount;
		}
		len= tempcount;
	}

	memmove(pt,&len,tlen);
	pt+=tlen;

	WORDLIST::iterator it;
	for(it=m_SubGroups.begin();it!=m_SubGroups.end();it++)
	{
		if(onlyLocked && !IdcUser::WillTeleBackup( (*it).getword()) )
			continue;
		len=strlen((*it));
		strcpy(pt,(*it).getword());
		pt+=len;
		pt[0]='\0';
		pt++;
	}

	len=m_Entitys.size();
	if(onlyLocked)
	{
		S_UINT tempcount=0;
		WORDLIST::iterator tempit;
		for(tempit=m_Entitys.begin();tempit!=m_Entitys.end();tempit++)
		{
			if(IdcUser::WillTeleBackup( (*tempit).getword()) )
				++tempcount;
		}
		len= tempcount;
	}

	memmove(pt,&len,tlen);
	pt+=tlen;

	for(it=m_Entitys.begin();it!=m_Entitys.end();it++)
	{
		if(onlyLocked && !IdcUser::WillTeleBackup( (*it).getword()) )
			continue;
		len=strlen((*it));
		strcpy(pt,(*it).getword());
		pt+=len;
		pt[0]='\0';
		pt++;
	}

	return lpbuf;
}
BOOL Group::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;
		
	S_UINT len=0,tlen=sizeof(S_UINT);

	try{

		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;

		memmove(&m_CurrentID,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;


		m_ID=pt;
		pt+=strlen(m_ID)+1;
		if(pt>pend)
			return false;

		memmove(&len,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;
		if(!m_Property.CreateObjectByRawData(pt,len))
			return false;
		pt+=len;
		if(pt>pend)
			return false;

		memmove(&len,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;
		const char* wt=NULL;

		for(int i=0;i<len;i++)
		{
			wt=pt;
			pt+=strlen(wt)+1;
			if(pt>pend)
				return false;
			m_SubGroups.push_back(wt);
		}

		memmove(&len,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;

		for(i=0;i<len;i++)
		{
			wt=pt;
			pt+=strlen(wt)+1;
			if(pt>pend)
				return false;
			m_Entitys.push_back(wt);

		}
	}catch(...)
	{
		return false;
	}




	return true;
}
