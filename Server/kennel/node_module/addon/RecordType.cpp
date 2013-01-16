#include "RecordType.h"

S_UINT RecordType::GetRawDataSize()
{
	S_UINT len=0;
	len+=sizeof(int);
	len+=strlen(m_monitorid.c_str())+1;

	int count=m_data.size();
	for(int i=0;i<count;i++)
	{
		len+=sizeof(DataType);
	}

	return len;
}

bool RecordType::GetRawData(char *buf,S_UINT buflen)
{
	if(buf==NULL)
		return false;

	if(buflen<GetRawDataSize())
		return false;
	char *pt=buf;
	memmove(pt,&m_monitortype,sizeof(int));
	pt+=sizeof(int);
	strcpy(pt,m_monitorid.c_str());
	pt+=strlen(m_monitorid.c_str());
	pt[0]='\0';
	pt++;

	int count=m_data.size();
	memmove(pt,&count,sizeof(int));
	pt+=sizeof(int);

	for(int i=0;i<count;i++)
	{
		DataType *pdt=m_data[i];
		if(pdt==NULL)
			return false;
		memmove(pt,pdt,sizeof(DataType));
		pt+=sizeof(DataType);
	}
	return true;
}

bool RecordType::CreateObjectByRawData(const char *buf,S_UINT buflen)
{
	if(buf==NULL)
		return false;
	const char *pt=buf;
	try{

		memmove(&m_monitortype,pt,sizeof(int));
		pt+=sizeof(int);
		m_monitorid=pt;
		pt+=strlen(m_monitorid.c_str())+1;
		int count=0;
		memmove(&count,pt,sizeof(int));
		pt+=sizeof(int);
		DataType *pdt=NULL;
		for(int i=0;i<count;i++)
		{
			pdt=new DataType();
			if(pdt==NULL)
				return false;
			memmove(pdt,pt,sizeof(DataType));
			pt+=sizeof(DataType);
			m_data.push_back(pdt);
		}

	}catch(...)
	{
		return false;
	}

	return true;

}