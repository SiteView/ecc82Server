#include "EntityRelation.h"

EntityRelation::EntityRelation(void)
{
}

EntityRelation::~EntityRelation(void)
{
}
S_UINT	EntityRelation::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);

	len+=tlen;  //datalen;
	len+=(S_UINT)strlen(m_Type)+1;
	len+=(S_UINT)strlen(m_Description)+1;
	len+=(S_UINT)strlen(m_PeerID)+1;
	len+=(S_UINT)strlen(m_LocalPortID)+1;

	len+=tlen;		//m_Property len
	len+=m_Property.GetRawDataSize();


	return len;
}
char*	EntityRelation::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;

	try{

		if(bufsize<GetRawDataSize())
			return NULL;
		S_UINT len=0,tlen=sizeof(S_UINT);
		char *pt=lpbuf;

		len=GetRawDataSize();
		memmove(pt,&len,tlen);
		pt+=tlen;

		strcpy(pt,m_Type);
		pt+=strlen(m_Type)+1;

		strcpy(pt,m_Description);
		pt+=strlen(m_Description)+1;

		strcpy(pt,m_PeerID);
		pt+=strlen(m_PeerID)+1;

		strcpy(pt,m_LocalPortID);
		pt+=strlen(m_LocalPortID)+1;

		len=m_Property.GetRawDataSize();
		memmove(pt,&len,tlen);
		pt+=tlen;

		if(m_Property.GetRawData(pt,len)==NULL)
			return NULL;
		pt+=len;

	}catch(...)
	{
		printf("\nException to EntityRelation::GetRawData \n");
		return NULL;
	}
	return lpbuf;
}
BOOL	EntityRelation::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;

	try{		
		S_UINT len=0,tlen=sizeof(S_UINT);

		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;

		memmove(&len,pt,tlen);
		if(len!=bufsize)
			return false;
		pt+=tlen;

		m_Type=pt;
		pt+=strlen(m_Type)+1;
		if(pt>pend)
			return false;

		m_Description=pt;
		pt+=strlen(m_Description)+1;
		if(pt>pend)
			return false;

		m_PeerID=pt;
		pt+=strlen(m_PeerID)+1;
		if(pt>pend)
			return false;

		m_LocalPortID=pt;
		pt+=strlen(m_LocalPortID)+1;
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

	}catch(...)
	{
		printf("Exception to EntityRelation::CreateObjectByRawData \n");
		return false;
	}

	return true;
}
