#include "monitor.h"

Monitor::Monitor(void)
{
	m_ID="";
}

Monitor::~Monitor(void)
{
}

S_UINT	Monitor::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);

	len+=strlen(m_ID)+1;

	len+=tlen;
	len+=m_Property.GetRawDataSize();

	len+=tlen;
	len+=m_Parameters.GetRawDataSize();

	len+=tlen;
	len+=m_ADParameters.GetRawDataSize();

	for(int i=0;i<3;i++)
	{
		len+=tlen;
		len+=m_AlertCondition[i].GetRawDataSize();
	}

	return len;

}
char*	Monitor::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;
	if(bufsize<GetRawDataSize())
		return NULL;

	S_UINT len=0,tlen=sizeof(S_UINT);
	char *pt=lpbuf;

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

	len=m_Parameters.GetRawDataSize();
	memmove(pt,&len,tlen);
	pt+=tlen;

	if(m_Parameters.GetRawData(pt,len)==NULL)
		return NULL;
	pt+=len;

	len=m_ADParameters.GetRawDataSize();
	memmove(pt,&len,tlen);
	pt+=tlen;

	if(m_ADParameters.GetRawData(pt,len)==NULL)
		return NULL;
	pt+=len;

	for(int i=0;i<3;i++)
	{
		len=m_AlertCondition[i].GetRawDataSize();
		memmove(pt,&len,tlen);
		pt+=tlen;
		if(m_AlertCondition[i].GetRawData(pt,len)==NULL)
			return NULL;
		pt+=len;
	}

	return lpbuf;

}
BOOL	Monitor::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;

	S_UINT len=0,tlen=sizeof(S_UINT);

	try{
		const char *pt=lpbuf;
		const char *pend=lpbuf+bufsize;

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

		if(!m_Parameters.CreateObjectByRawData(pt,len))
			return false;
		pt+=len;
		if(pt>pend)
			return false;


		memmove(&len,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;

		if(!m_ADParameters.CreateObjectByRawData(pt,len))
			return false;
		pt+=len;
		if(pt>pend)
			return false;

		for(int i=0;i<3;i++)
		{
			memmove(&len,pt,tlen);
			pt+=tlen;
			if(pt>pend)
				return false;

			if(!m_AlertCondition[i].CreateObjectByRawData(pt,len))
				return false;
			pt+=len;
			if(pt>pend)
				return false;

		}
	}catch(...)
	{
		return false;
	}

	return true;


}

