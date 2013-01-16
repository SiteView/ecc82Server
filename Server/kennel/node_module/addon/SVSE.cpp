#include "SVSE.h"

SVSE::SVSE(void)
{
	m_CurrentID=0;
	m_ID=1;
}

SVSE::~SVSE(void)
{
}

S_UINT SVSE::GetRawDataSize(bool onlyLocked)
{
	S_UINT len=0,tlen=sizeof(S_UINT);
	len+=tlen;		//m_currentid;
	len+=tlen;		//m_ID;
	len+=strlen(m_Label.getword())+1;

    len+=tlen;		//sub group count size
	WORDLIST::iterator it;
	for(it=m_SubGroups.begin();it!=m_SubGroups.end();it++)
	{
		if(onlyLocked && !IdcUser::WillTeleBackup( (*it).getword()) )
			continue;
		len+=strlen((*it))+1;
	}


	len+=tlen;
	for(it=m_SubEntitys.begin();it!=m_SubEntitys.end();it++)
	{
		if(onlyLocked && !IdcUser::WillTeleBackup( (*it).getword()) )
			continue;
		len+=strlen((*it))+1;
	}


	return len;

}
char* SVSE::GetRawData(char *lpbuf,S_UINT bufsize,bool onlyLocked)
{
	if(lpbuf==NULL)
		return NULL;

	if(bufsize<GetRawDataSize(onlyLocked))
		return NULL;

	char *pt=lpbuf;
	S_UINT len=0,tlen=sizeof(S_UINT);

	memmove(pt,&m_CurrentID,tlen);
	pt+=tlen;

	memmove(pt,&m_ID,tlen);
	pt+=tlen;

	strcpy(pt,m_Label.getword());
	pt+=strlen(m_Label.getword());
	pt[0]='\0';
	pt++;

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
		len=strlen((*it).getword());
		strcpy(pt,(*it).getword());
		pt+=len;
		pt[0]='\0';
		pt++;
	}

	len=m_SubEntitys.size();
	if(onlyLocked)
	{
		S_UINT tempcount=0;
		WORDLIST::iterator tempit;
		for(tempit=m_SubEntitys.begin();tempit!=m_SubEntitys.end();tempit++)
		{
			if(IdcUser::WillTeleBackup( (*tempit).getword()) )
				++tempcount;
		}
		len= tempcount;
	}

	memmove(pt,&len,tlen);
	pt+=tlen;

	for(it=m_SubEntitys.begin();it!=m_SubEntitys.end();it++)
	{
		if(onlyLocked && !IdcUser::WillTeleBackup( (*it).getword()) )
			continue;
		len=strlen((*it).getword());
		strcpy(pt,(*it).getword());
		pt+=len;
		pt[0]='\0';
		pt++;


	}


	return lpbuf;

}
BOOL SVSE::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
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

		memmove(&m_ID,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;

		m_Label=pt;
		pt+=strlen(m_Label)+1;
		if(pt>pend)
			return false;

		const char* wt=NULL;

		memmove(&len,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;

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

		for(int i=0;i<len;i++)
		{
			wt=pt;
			pt+=strlen(wt)+1;
			if(pt>pend)
				return false;
			m_SubEntitys.push_back(wt);

		}
	}catch(...)
	{
		return false;
	}

	return true;
}

