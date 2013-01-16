#include "Port.h"

Port::Port(void)
{
}

Port::~Port(void)
{
}

S_UINT	Port::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);

	try{
		len+=tlen;  //datalen;
		len+=(S_UINT)strlen(m_ID)+1;	//ID len;
		len+=(S_UINT)strlen(m_PeerPortID)+1;
		len+=(S_UINT)strlen(m_MonitorID)+1;

		len+=tlen;
		len+=m_Property.GetRawDataSize();
	}
	catch(...)
	{
		printf("Exeption to Port::GetRawDataSize\n ");
	}
	return len;
}

char* Port::GetRawData(char *lpbuf,S_UINT bufsize)
{
	try{
		if(lpbuf==NULL)
			return NULL;
		if(bufsize<GetRawDataSize())
			return NULL;
		S_UINT len=0,tlen=sizeof(S_UINT);
		char *pt=lpbuf;

		len=GetRawDataSize();
		memmove(pt,&len,tlen);
		pt+=tlen;

		strcpy(pt,m_ID);
		pt+=strlen(m_ID)+1;

		strcpy(pt,m_PeerPortID);
		pt+=strlen(m_PeerPortID)+1;

		strcpy(pt,m_MonitorID);
		pt+=strlen(m_MonitorID)+1;

		len=m_Property.GetRawDataSize();
		memmove(pt,&len,tlen);
		pt+=tlen;

		if(m_Property.GetRawData(pt,len)==NULL)
			return NULL;
		pt+=len;

		return lpbuf;

	}catch(...)
	{
		printf("\n Exception to Port::GetRawData \n");
		return NULL;
	}
}

BOOL Port::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
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
		if(pt>pend)
			return false;

		m_ID=pt;
		pt+=strlen(m_ID)+1;

		if(pt>pend)
			return false;

		m_PeerPortID=pt;
		pt+=strlen(m_PeerPortID)+1;

		if(pt>pend)
			return false;

		m_MonitorID=pt;
		pt+=strlen(m_MonitorID)+1;

		if(pt>pend)
			return false;

		memmove(&len,pt,tlen);
		pt+=tlen;

		if(!m_Property.CreateObjectByRawData(pt,len))
			return false;

		pt+=len;

	}catch(...)
	{
		printf("\n Exception to Port::CreateObjectByRawData \n");
		return false;
	}


	return true;
}

