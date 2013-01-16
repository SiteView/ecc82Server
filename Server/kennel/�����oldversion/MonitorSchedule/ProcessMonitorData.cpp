#include "ProcessMonitorData.h"

ProcessMonitorData::ProcessMonitorData(void)
{
	m_datalen=0;
	m_state=0;
	m_data=NULL;

	m_dllname="";
	m_funname="";
}

ProcessMonitorData::~ProcessMonitorData(void)
{
}
int ProcessMonitorData::GetRawDataSize(void)
{
	int len=0;
	len+=sizeof(int);   //raw data size;
	len+=sizeof(int);   //m_datalen;
	len+=sizeof(int);   //m_state;
	len+=m_datalen;     //m_data;
	len+=m_dllname.size()+1; //m_dllname;
	len+=m_funname.size()+1; //m_funname;

	return len;
}
bool ProcessMonitorData::GetRawData(char *databuf,int len)
{
	if(databuf==NULL)
		return false;

	int dlen=GetRawDataSize();
	if(len<dlen)
		return false;

	char *pt=databuf;
	memmove(pt,&dlen,sizeof(int));
	pt+=sizeof(int);
	memmove(pt,&m_state,sizeof(int));
	pt+=sizeof(int);
	memmove(pt,&m_datalen,sizeof(int));
	pt+=sizeof(int);
	memmove(pt,m_data,m_datalen);
	pt+=m_datalen;

	strcpy(pt,m_dllname.c_str());
	pt+=m_dllname.size()+1;

	strcpy(pt,m_funname.c_str());
	pt+=m_funname.size()+1;

	return true;
}
bool ProcessMonitorData::CreateObjectByRawData(const char *data,int len)
{
	if(data==NULL)
		return false;

	if(m_data!=NULL)
		delete [] m_data;

	const char *pt=data;

	int dlen=0;
	memmove(&dlen,pt,sizeof(int));
	if(dlen!=len)
		return false;
	pt+=sizeof(int);

	try{
		memmove(&m_state,pt,sizeof(int));
		pt+=sizeof(int);
		
		int datalen=0;
		memmove(&datalen,pt,sizeof(int));
		pt+=sizeof(int);

		//if(m_datalen<datalen)
		//	return false;

		m_data=new char[datalen];
		if(m_data==NULL)
			return false;
		memset(m_data,0,datalen);

		memmove(m_data,pt,datalen);
		pt+=datalen;

		m_datalen=datalen;

		if(pt[0]!='\0')
		{
			m_dllname=pt;
			pt+=m_dllname.size()+1;
			m_funname=pt;
			pt+=m_funname.size()+1;
		}

	}catch(...)
	{
		return false;
	}

	return true;
}

void ProcessMonitorData::Clear()
{
	if(m_data)
		delete [] m_data;

}
