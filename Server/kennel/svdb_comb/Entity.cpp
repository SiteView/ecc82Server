#include "Entity.h"

Entity::Entity(void)
{
	m_CurrentID=0;
	m_ID="";
}

Entity::~Entity(void)
{
}

S_UINT	Entity::GetRawDataSize(bool onlyLocked)
{
	S_UINT len=0,tlen=sizeof(S_UINT);
	len+=tlen;		//m_CurrentID;

	len+=strlen(m_ID)+1;	//m_ID;

	len+=tlen;				//m_Property size;
	len+=m_Property.GetRawDataSize();

	len+=tlen;				//m_Monitors count;
	WORDLIST::iterator it;
	for(it=m_Monitors.begin();it!=m_Monitors.end();it++)
	{
		if(onlyLocked && !IdcUser::WillTeleBackup( (*it).getword()) )
			continue;
		len+=strlen(*it)+1;
	}

	return len;


}
char*	Entity::GetRawData(char *lpbuf,S_UINT bufsize, bool onlyLocked)
{
	if(lpbuf==NULL)
		return NULL;
	if(bufsize<GetRawDataSize(onlyLocked))
		return NULL;

	S_UINT len=0,tlen=sizeof(S_UINT);
	
	char *pt=lpbuf;
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


	len=m_Monitors.size();
	if(onlyLocked)
	{
		S_UINT tempcount=0;
		WORDLIST::iterator tempit;
		for(tempit=m_Monitors.begin();tempit!=m_Monitors.end();tempit++)
		{
			if(IdcUser::WillTeleBackup( (*tempit).getword()) )
				++tempcount;
		}
		len= tempcount;
	}
	memmove(pt,&len,tlen);
	pt+=tlen;

	WORDLIST::iterator it;
	for(it=m_Monitors.begin();it!=m_Monitors.end();it++)
	{
		if(onlyLocked && !IdcUser::WillTeleBackup( (*it).getword()) )
			continue;
		strcpy(pt,(*it).getword());
		pt+=strlen((*it).getword());
		pt[0]='\0';
		pt++;	
	}

    return lpbuf;    

}
BOOL	Entity::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
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

		const char *wt=NULL;
		for(int i=0;i<len;i++)
		{
			wt=pt;
			pt+=strlen(wt)+1;
			if(pt>pend)
				return false;
			m_Monitors.push_back(wt);

		}
	}catch(...)
	{
		return false;
	}

	return true;

}

