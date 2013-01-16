#include "QueueRecord.h"

QueueRecord::QueueRecord(void)
{
	m_level=1;
	m_data=NULL;
	m_datalen=0;
}

QueueRecord::~QueueRecord(void)
{
	if(m_data!=NULL)
		delete [] m_data;

}

QueueRecord::QueueRecord(string label,svutil::TTime ctime,int level)
{
	m_label=label;
	m_createtime=ctime;
	m_level=level;

	m_data=NULL;
	m_datalen=0;

}
QueueRecord::QueueRecord(string label,svutil::TTime ctime,const char *data,S_UINT datalen,int level)
{
	m_label=label;
	m_createtime=ctime;
	m_level=level;

	m_data=NULL;
	m_datalen=0;
	if((data!=NULL)&&(datalen>0))
	{
		m_data=new char[datalen];
		if(m_data!=NULL)
		{
			memmove(m_data,data,datalen);
			m_datalen=datalen;
		}
		else
			m_data=NULL;
	}

}
S_UINT	QueueRecord::GetRawDataSize(void)
{
	S_UINT len=0;
	
	len+=sizeof(S_UINT);  //selfsize
	len+=sizeof(svutil::TTime); //m_createtime
	len+=m_label.size()+1;    //m_label;
	len+=sizeof(int);		//m_level;
	len+=sizeof(S_UINT);	//m_datalen;
	len+=m_datalen;			//m_data;

	return len;


}
char*	QueueRecord::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;

	S_UINT dsize=GetRawDataSize();
	if(bufsize<dsize)
		return NULL;

	S_UINT len=0,tlen=sizeof(S_UINT);
	char *pt=lpbuf;

	memmove(pt,&dsize,sizeof(S_UINT));
	pt+=sizeof(S_UINT);

	memmove(pt,&m_createtime,sizeof(svutil::TTime));
	pt+=sizeof(svutil::TTime);

	strcpy(pt,m_label.c_str());
	pt+=m_label.size();
	pt[0]='\0';
	pt++;

	memmove(pt,&m_level,sizeof(int));
	pt+=sizeof(int);

	memmove(pt,&m_datalen,sizeof(S_UINT));
	pt+=sizeof(S_UINT);

	if(m_data==NULL)
	{
		if(m_datalen!=0)
			return NULL;
	}
	memmove(pt,m_data,m_datalen);
	pt+=m_datalen;

	return lpbuf;

}
BOOL QueueRecord::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;

	if(bufsize<sizeof(S_UINT))
		return false;

	S_UINT len=0,tlen=sizeof(S_UINT);

	try{

		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;

		S_UINT dlen=0;
		memmove(&dlen,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;

		if(dlen!=bufsize)
		{
			puts("Raw data error");
			return false;
		}

		memmove(&m_createtime,pt,sizeof(svutil::TTime));
		pt+=sizeof(svutil::TTime);
		if(pt>pend)
			return false;

		m_label=pt;
		pt+=m_label.size()+1;
		if(pt>pend)
			return false;

		memmove(&m_level,pt,sizeof(int));
		pt+=sizeof(int);

		memmove(&m_datalen,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;
		if(m_datalen>0)
		{
			m_data=new char[m_datalen];
			if(m_data==NULL)
				return false;
			memmove(m_data,pt,m_datalen);
			pt+=m_datalen;
			if(pt>pend)
			{
				delete [] m_data;
				m_data=NULL;
				return false;
			}
		}


	}catch(...)
	{
		return false;
	}

	return true;

}

