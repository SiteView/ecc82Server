#include "Edge.h"

Edge::Edge(void)
{
}

Edge::~Edge(void)
{
}

S_UINT	Edge::GetRawDataSize(void)
{

	S_UINT len=0,tlen=sizeof(S_UINT);
	len+=tlen;  //datalen;

	len+=(S_UINT)strlen(m_LeftPortID)+1;
	len+=(S_UINT)strlen(m_RightPortID)+1;
	len+=(S_UINT)strlen(m_DependMonitorID)+1;

	len+=tlen;
	len+=m_Property.GetRawDataSize();


	return len;
}
char*	Edge::GetRawData(char *lpbuf,S_UINT bufsize)
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

	strcpy(pt,m_LeftPortID);
	pt+=strlen(m_LeftPortID)+1;

	strcpy(pt,m_RightPortID);
	pt+=strlen(m_RightPortID)+1;

	strcpy(pt,m_DependMonitorID);
	pt+=strlen(m_DependMonitorID)+1;

	len=m_Property.GetRawDataSize();
	memmove(pt,&len,tlen);
	pt+=tlen;

	if(m_Property.GetRawData(pt,len)==NULL)
		return NULL;

	pt+=len;

	return lpbuf;
}
BOOL	Edge::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
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

		m_LeftPortID=pt;
		pt+=strlen(m_LeftPortID)+1;

		m_RightPortID=pt;
		pt+=strlen(m_RightPortID)+1;

		m_DependMonitorID=pt;
		pt+=strlen(m_DependMonitorID)+1;


		memmove(&len,pt,tlen);
		pt+=tlen;

		if(m_Property.CreateObjectByRawData(pt,len)==NULL)
			return false;

	}catch(...)
	{
		return false;
	}

	return true;
}

