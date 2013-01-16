#include "EntityPoolEx.h"

EntityPoolEx::EntityPoolEx(void)
{
	m_hashtablesize=hashtablesize;
	m_entityexs.resetsize(m_hashtablesize);
	m_changed=false;


}

EntityPoolEx::EntityPoolEx(word filepath):PoolBase(filepath)
{
	m_hashtablesize=hashtablesize;
	m_entityexs.resetsize(m_hashtablesize);
	m_changed=false;
}


EntityPoolEx::~EntityPoolEx(void)
{
	if(m_changed)
		Submit();

	ENTITYEXMAP::iterator it;
	while(m_entityexs.findnext(it))
	{
		EntityEx *pe=(*it).getvalue();
		if(pe)
			delete pe;
	}

}

bool EntityPoolEx::Load(void)
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
bool EntityPoolEx::Submit()
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

S_UINT	EntityPoolEx::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);
	len+=tlen;

	len+=tlen;        //m_tablesize size;
	len+=tlen;		  //total entitys count;

	ENTITYEXMAP::iterator it;
	while(m_entityexs.findnext(it))
	{
		len+=tlen;			//len size;
		len+=(*it).getvalue()->GetRawDataSize();

	}

	return len;


}
char*	EntityPoolEx::GetRawData(char *lpbuf,S_UINT bufsize)
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

		S_UINT count=m_entityexs.size();
		memmove(pt,&count,tlen);
		pt+=tlen;

		ENTITYEXMAP::iterator it;
		while(m_entityexs.findnext(it))
		{
			if((*it).getvalue()->GetIdentifier().empty())
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
		printf("\nException to EntityPoolEx::GetRawData \n");
		return false;
	}

}
BOOL	EntityPoolEx::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
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

		m_entityexs.resetsize(m_hashtablesize);

		S_UINT count=0;
        memmove(&count,pt,tlen);
		pt+=tlen;

		EntityEx *ptc=NULL;
		for(S_UINT i=0;i<count;i++)
		{
			memmove(&len,pt,tlen);
			pt+=tlen;

			ptc=new EntityEx();
			if(!ptc->CreateObjectByRawData(pt,len))
				return false;
			pt+=len;

			word id=ptc->GetIdentifier();
			m_entityexs[id]=ptc;

		}

	}catch(...)
	{
		printf("\nException to EntityPoolEx::CreateObjectByRawData \n");
		return false;
	}

	return true;

}

bool EntityPoolEx::push(EntityEx *pm)
{
	ost::MutexLock lock(m_UpdateLock);

	if(!pm)
		return false;
	word id=pm->GetIdentifier();
	if(id.empty())
		return false;

	EntityEx **p=m_entityexs.find(id);
	if(p!=NULL)
		delete (*p);
	m_entityexs[id]=pm;

	m_changed=true;
	return true;

}
bool EntityPoolEx::PushData(const char *buf,S_UINT len)
{
	ost::MutexLock lock(m_UpdateLock);
	EntityEx *pm=new EntityEx();
	if(pm==NULL)
		return false;
	if(!pm->CreateObjectByRawData(buf,len))
		return false;
	return push(pm);

}
bool EntityPoolEx::GetEntityExData(word id,char *buf,S_UINT &len)
{
	EntityEx **pm=m_entityexs.find(id);
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
bool EntityPoolEx::DeleteEntityEx(word id)
{
	ost::MutexLock lock(m_UpdateLock);
	EntityEx **pm=m_entityexs.find(id);
	if(pm==NULL)
		return false;
	delete (*pm);
   if(m_entityexs.erase(id))
   {
	   	m_changed=true;
		return true;
   }
   return false;

}



