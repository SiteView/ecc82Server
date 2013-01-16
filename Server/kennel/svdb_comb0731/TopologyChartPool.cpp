#include "TopologyChartPool.h"
#include <cc++/file.h>


TopologyChartPool::TopologyChartPool(void)
{
	m_hashtablesize=hashtablesize;
	m_data.resetsize(m_hashtablesize);
	m_changed=false;

}
TopologyChartPool::TopologyChartPool(word filepath):PoolBase(filepath)
{
	m_hashtablesize=hashtablesize;
	m_data.resetsize(m_hashtablesize);
	m_changed=false;

}


TopologyChartPool::~TopologyChartPool(void)
{
	if(m_changed)
		Submit();

	TOPOLOGYCHARTMAP::iterator it;
	while(m_data.findnext(it))
	{
		TopologyChart *pe=(*it).getvalue();
		if(pe)
			delete pe;
	}


}
bool TopologyChartPool::Load(void)
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
			return CreateObjectByRawData(p,len);
		}


	}catch(...)
	{
		puts("Load GroupPool Exception");
		return false;
	}


	return	false;
}
bool TopologyChartPool::Submit()
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
		puts("Submit TopologyChartPool Exception");
		return false;
	}

	m_changed=false;
	return true;
}

S_UINT	TopologyChartPool::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);
	len+=tlen;

	len+=tlen;        //m_tablesize size;
	len+=tlen;		  //total chart count;

	TOPOLOGYCHARTMAP::iterator it;
	while(m_data.findnext(it))
	{
		len+=tlen;
		len+=(*it).getvalue()->GetRawDataSize();
	}

	return len;
}
char*	TopologyChartPool::GetRawData(char *lpbuf,S_UINT bufsize)
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

		memmove(pt,&m_hashtablesize,tlen);
		pt+=tlen;

		S_UINT count=m_data.size();
		memmove(pt,&count,tlen);
		pt+=tlen;

		TOPOLOGYCHARTMAP::iterator it;
		while(m_data.findnext(it))
		{
			if((*it).getvalue()->GetID().empty())
				continue;

			len=(*it).getvalue()->GetRawDataSize();
			memmove(pt,&len,tlen);
			pt+=tlen;

			if((*it).getvalue()->GetRawData(pt,len)==NULL)
				return NULL;
			pt+=len;
		}
		return lpbuf;

	}catch(...)
	{
		printf("\nException to TopologyChartPool::GetRawData \n");
		return false;
	}
}
BOOL	TopologyChartPool::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
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

		memmove(&m_hashtablesize,pt,tlen);
		pt+=tlen;

		m_data.resetsize(m_hashtablesize);

		S_UINT count=0;
        memmove(&count,pt,tlen);
		pt+=tlen;

		TopologyChart *ptc=NULL;
		for(S_UINT i=0;i<count;i++)
		{
			memmove(&len,pt,tlen);
			pt+=tlen;

			ptc=new TopologyChart();
			if(!ptc->CreateObjectByRawData(pt,len))
				return false;
			pt+=len;

			word id=ptc->GetID();
			m_data[id]=ptc;

		}

	}catch(...)
	{
		printf("\nException to TopologyChartPool::CreateObjectByRawData\n");
		return false;
	}

	return true;
}

bool TopologyChartPool::push(TopologyChart *pm)
{
	ost::MutexLock lock(m_UpdateLock);

	if(!pm)
		return false;
	word id=pm->GetID();
	if(id.empty())
		return false;

	TopologyChart **p=m_data.find(id);
	if(p!=NULL)
		delete (*p);
	m_data[id]=pm;

	m_changed=true;
	return true;


}
bool TopologyChartPool::PushData(const char *buf,S_UINT len)
{
	ost::MutexLock lock(m_UpdateLock);
	TopologyChart *pm=new TopologyChart();
	if(pm==NULL)
		return false;
	if(!pm->CreateObjectByRawData(buf,len))
		return false;
	return push(pm);


}

bool TopologyChartPool::GetTopologyChartData(word id,char *buf,S_UINT &len)
{
	TopologyChart **pm=m_data.find(id);
	if(pm==NULL)
		return false;
	S_UINT size=(*pm)->GetRawDataSize();
	if(buf==NULL)
	{
		len=size;
		return true;
	}

	if(len<size)
		return false;
	return ((*pm)->GetRawData(buf,size)!=NULL);

}

bool TopologyChartPool::DeleteTopologyChart(word id)
{
	ost::MutexLock lock(m_UpdateLock);
	TopologyChart **pm=m_data.find(id);
	if(pm==NULL)
		return false;
	delete (*pm);
   if(m_data.erase(id))
   {
	   	m_changed=true;
		return true;
   }
   return false;

}

