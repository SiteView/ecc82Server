#include "VirtualGroupPool.h"

VirtualGroupPool::VirtualGroupPool(void)
{
	m_hashtablesize=hashtablesize;
	m_data.resetsize(m_hashtablesize);
	m_changed=false;

}
VirtualGroupPool::VirtualGroupPool(word filepath):PoolBase(filepath)
{
	m_hashtablesize=hashtablesize;
	m_data.resetsize(m_hashtablesize);
	m_changed=false;
}


VirtualGroupPool::~VirtualGroupPool(void)
{
	if(m_changed)
		Submit();

	VIRTUALGROUPMAP::iterator it;
	while(m_data.findnext(it))
	{
		VirtualGroup *pe=(*it).getvalue();
		if(pe)
			delete pe;
	}

}

bool VirtualGroupPool::Load(void)
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
		puts("Load EntityPool Exception");
		return false;
	}


	return	false;
}
bool VirtualGroupPool::Submit()
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
		puts("Submit EntityPool Exception");
		return false;
	}

	m_changed=false;
	return true;
}

S_UINT	VirtualGroupPool::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);
	len+=tlen;

	len+=tlen;        //m_tablesize size;
	len+=tlen;		  //total data count;

	VIRTUALGROUPMAP::iterator it;
	while(m_data.findnext(it))
	{
		len+=tlen;			//len size;
		len+=(*it).getvalue()->GetRawDataSize();

	}

	return len;
}
char*	VirtualGroupPool::GetRawData(char *lpbuf,S_UINT bufsize)
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

	memmove(pt,&m_hashtablesize,tlen);
	pt+=tlen;

	S_UINT count=m_data.size();
	memmove(pt,&count,tlen);
	pt+=tlen;

	VIRTUALGROUPMAP::iterator it;
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

}
BOOL	VirtualGroupPool::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
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

		VirtualGroup *ptc=NULL;
		for(S_UINT i=0;i<count;i++)
		{
			memmove(&len,pt,tlen);
			pt+=tlen;

			ptc=new VirtualGroup();
			if(!ptc->CreateObjectByRawData(pt,len))
				return false;
			pt+=len;

			word id=ptc->GetID();
			m_data[id]=ptc;

		}

	}catch(...)
	{
		return false;
	}

	return true;
}

bool VirtualGroupPool::push(VirtualGroup *pm)
{
	ost::MutexLock lock(m_UpdateLock);

	if(!pm)
		return false;
	word id=pm->GetID();
	if(id.empty())
		return false;

	VirtualGroup **p=m_data.find(id);
	if(p!=NULL)
		delete (*p);
	m_data[id]=pm;

	m_changed=true;
	return true;
}
bool VirtualGroupPool::PushData(const char *buf,S_UINT len)
{
	ost::MutexLock lock(m_UpdateLock);
	VirtualGroup *pm=new VirtualGroup();
	if(pm==NULL)
		return false;
	if(!pm->CreateObjectByRawData(buf,len))
		return false;
	return push(pm);

}

bool VirtualGroupPool::GetVirtualGroupData(word id,char *buf,S_UINT &len)
{
	VirtualGroup **pm=m_data.find(id);
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

bool VirtualGroupPool::DeleteVirtualGroup(word id)
{
	ost::MutexLock lock(m_UpdateLock);
	VirtualGroup **pm=m_data.find(id);
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



