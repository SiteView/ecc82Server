#include "MonitortempletPool.h"

using namespace ost;

MonitorTempletPool::MonitorTempletPool()
{
	m_data.resize(initsize);
	m_count=initsize;
	m_loaded=false;
	m_changed=false;
	isorig= false;

	for(int i=0;i<initsize;i++)
		m_data[i]=NULL;
}

MonitorTempletPool::~MonitorTempletPool(void)
{
	if(m_changed)
		Submit();

	for(int i=0;i<m_count;i++)
	{
		if(m_data[i]!=NULL)
			delete m_data[i];
	}
}

bool MonitorTempletPool::Load(void)
{
	S_UINT len=0;
	try{

		{//a Common C++, used for multi-thread file-based DB access
			ost::ThreadFile fl(m_FilePath.getword());
			len =fl.getCapacity();
		}

		if(len<=0)
			return false;
	

		//map the file to the memory
		ost::MappedFile file(m_FilePath.getword(),0,0,ost::File::accessReadWrite);
		char*p=(char *)file.fetch(0,0);
		if(p)
		{
			return CreateObjectByRawData(p,len);
		}


	}catch(...)
	{
		puts("Load MonitorTempletPool Exception");
		return false;
	}


	return	false;


}
bool MonitorTempletPool::Submit()
{
	ost::MutexLock lock(m_UpdateLock);

	if(IdcUser::AutoResolveIDS && !isorig)
		return true;

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
		puts("Submit MonitorTempletPool Exception");
		return false;
	}

	m_changed=false;
	return true;
}

S_UINT	MonitorTempletPool::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);

	len+=tlen;                  //m_count;
	len+=tlen;					//real count;
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
char*	MonitorTempletPool::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;

	if(bufsize<GetRawDataSize())
		return NULL;

	S_UINT len=0,tlen=sizeof(S_UINT);
	char *pt=lpbuf;

	memmove(pt,&m_count,tlen);
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
BOOL	MonitorTempletPool::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;

	S_UINT len=0,tlen=sizeof(S_UINT);

	try{

		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;

		memmove(&m_count,pt,tlen);
		pt+=tlen;
		m_data.resize(m_count);

		S_UINT count=0;
		memmove(&count,pt,tlen);
		pt+=tlen;

		S_UINT index=0;
		MonitorTemplet *pm=NULL;

		for(S_UINT i=0;i<count;i++)
		{
			memmove(&index,pt,tlen);
			pt+=tlen;
			memmove(&len,pt,tlen);
			pt+=tlen;
			pm=new MonitorTemplet();
			if(pm==NULL)
				return false;
			if(!pm->CreateObjectByRawData(pt,len))
				return false;
			pt+=len;

			m_data[index]=pm;

		}
	}catch(...)
	{
		return false;
	}



	return true;

}
bool MonitorTempletPool::GetMonitorTempletData(int id,char *buf,S_UINT &len)
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

bool MonitorTempletPool::PushData(const char *buf,S_UINT len, Resource *m_pLanguage)
{
	ost::MutexLock lock(m_UpdateLock);

	MonitorTemplet *pm=new MonitorTemplet();
	if(pm==NULL)
		return false;
	if(!pm->CreateObjectByRawData(buf,len))
		return false;
	bool ret=push(pm);
	if(m_pLanguage != NULL)
		SaveNewIds(pm->GetID(), m_pLanguage);
	return ret;
}

bool MonitorTempletPool::GetInfo(word infoname,StringMap &map)
{
	char id[100]={0};
	for(int i=0;i<m_count;i++)
	{
		if(m_data[i]!=NULL)
		{
			sprintf(id,"%d",m_data[i]->GetID());
			StringMap &tm=m_data[i]->GetProperty();
			if(tm.find(infoname)!=NULL)
				map[id]=tm[infoname];
			else
				map[id]="";
		}
	}


	return true;

}


bool MonitorTempletPool::ResolveIds(Resource *m_pLanguage)
{
	if(m_pLanguage==NULL)
	{
		cout<<"resource ForTpl is NULL ."<<endl;
		return false;
	}
	for(int i=0;i<m_count;i++)
	{
		if(m_data[i]!=NULL)
			m_data[i]->ResolveIds(m_pLanguage);
	}
	return true;
}


bool MonitorTempletPool::SaveNewIds(int id, Resource *m_pLanguage)
{
	if(m_pLanguage==NULL)
	{
		cout<<"resource ForTpl is NULL ."<<endl;
		return false;
	}
	if( !IdcUser::AutoResolveIDS || !isorig)
		return false;

	if(m_data[id]==NULL)
		return false;

	m_data[id]->SaveNewIds(m_pLanguage);

	return true;
}



