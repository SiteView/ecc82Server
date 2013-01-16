#include "svsepool.h"

SVSEPool::SVSEPool(void)
{
	m_data.resize(initsize);
	m_count=initsize;
	m_loaded=false;
	m_changed=false;
	m_CurrentID=1;

	for(int i=0;i<initsize;i++)
		m_data[i]=NULL;

}

SVSEPool::~SVSEPool(void)
{
	if(IdcUser::BoolToExit)
		return ;

	if(m_changed)
		Submit();

	for(int i=0;i<m_count;i++)
	{
		if(m_data[i]!=NULL)
			delete m_data[i];
	}

}

bool SVSEPool::Load(void)
{
	S_UINT len=0;
	try{

		{
			ost::ThreadFile fl(m_FilePath.getword());
			len =fl.getCapacity();
		}

		if(len<=0)
			return false;
	

		ost::MappedFile file(m_FilePath.getword(),0,0,ost::File::accessReadWrite);
		char*p=(char *)file.fetch(0,0);
		if(p)
		{
			return (CreateObjectByRawData(p,len)!=NULL);
		}


	}catch(...)
	{
		puts("Load SVSEPool Exception");
		return false;
	}


	return	false;

}
bool SVSEPool::Submit(std::string modifyid)
{
	ost::MutexLock lock(m_UpdateLock);

	if(!m_changed)
		return true;
	S_UINT len=this->GetRawDataSize();
	try{
		ost::MappedFile file(m_FilePath.getword(),ost::File::accessReadWrite,len);
		char *p=(char *)file.fetch(0,0);
		if(p)
		{
			if(this->GetRawData(p,len)==NULL)
				return false;
		}

	}catch(...)
	{
		puts("Submit SVSEPool Exception");
		return false;
	}

	m_changed=false;
	return true;

}

S_UINT SVSEPool::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);

	len+=tlen;                  //m_count;
	len+=tlen;					//real count;
	len+=tlen;					//m_CurrentID;
	for(S_UINT i=0;i<m_count;i++)
	{
		if(m_data[i]!=NULL)
		{
			len+=tlen;						//index space;
			len+=tlen;								//data size space;
			len+=m_data[i]->GetRawDataSize();		//data space;
		}
	}

	return len;

}
char* SVSEPool::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;

	if(bufsize<GetRawDataSize())
		return NULL;

	S_UINT len=0,tlen=sizeof(S_UINT);
	char *pt=lpbuf;

	memmove(pt,&m_count,tlen);
	pt+=tlen;

	memmove(pt,&m_CurrentID,tlen);
	pt+=tlen;

	S_UINT count=0;

	for(S_UINT n=0;n<m_count;n++)
	{
		if(m_data[n]!=NULL)
			count++;
	}

	memmove(pt,&count,tlen);
	pt+=tlen;

	for(S_UINT i=0;i<m_count;i++)
	{
		if(m_data[i]!=NULL)
		{
			memmove(pt,&i,tlen);
			pt+=tlen;
			len=m_data[i]->GetRawDataSize();
			memmove(pt,&len,tlen);
			pt+=tlen;

			if(m_data[i]->GetRawData(pt,len)==NULL)
				return NULL;
			pt+=len;
		}

	}

	return lpbuf;
}
BOOL SVSEPool::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;

	S_UINT len=0,tlen=sizeof(S_UINT);

	try{
		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;

		memmove(&m_count,pt,tlen);
		pt+=tlen;
		if(m_count<=0)
			return false;
		m_data.resize(m_count);

		memmove(&m_CurrentID,pt,tlen);
		pt+=tlen;

		S_UINT count=0;
		memmove(&count,pt,tlen);
		pt+=tlen;

		S_UINT index=0;
		SVSE *ps=NULL;

		for(S_UINT i=0;i<count;i++)
		{
			memmove(&index,pt,tlen);
			pt+=tlen;
			memmove(&len,pt,tlen);
			pt+=tlen;
			ps=new SVSE();
			if(ps==NULL)
				return false;
			if(!ps->CreateObjectByRawData(pt,len))
				return false;
			pt+=len;

			m_data[index]=ps;
			NewVersion(index);

		}
	}catch(...)
	{
		return false;
	}

	return true;
}

bool SVSEPool::GetSVSEData(S_UINT id,char *buf,S_UINT &len)
{
	if(!Find(id))
		return false;

	if(buf==NULL)
	{
		len=m_data[id]->GetRawDataSize();
		return true;
	}

	S_UINT size=0;
	size=m_data[id]->GetRawDataSize();
	if(len<size)
		return false;
	len=size;
	if(!m_data[id]->GetRawData(buf,len))
		return false;
    
	return true;

}

