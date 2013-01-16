#include "EntityGroupPool.h"

EntityGroupPool::EntityGroupPool(void)
{
	isorig= false;
}

EntityGroupPool::~EntityGroupPool(void)
{
	if(m_changed)
		Submit();

	ENTITYGROUPMAP::iterator it;
	while(m_data.findnext(it))
	{
		EntityGroup *pt=(*it).getvalue();
		if(pt)
			delete pt;
	}

}

EntityGroupPool::EntityGroupPool(word filepath,bool orig):PoolBase(filepath)
{
	isorig= orig;
}

bool EntityGroupPool::Load(void)
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
		puts("Load EntityGroupPool Exception");
		return false;
	}


	return	false;
}
bool EntityGroupPool::Submit()
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
		puts("Submit EntityGroupPool Exception");
		return false;
	}

	m_changed=false;
	return true;
}

S_UINT	EntityGroupPool::GetRawDataSize(void)
{

	S_UINT len=0,tlen=sizeof(S_UINT);

	len+=tlen;		//m_data count;

	ENTITYGROUPMAP::iterator it;
	while(m_data.findnext(it))
	{
		len+=tlen;		//data size;
		len+=(*it).getvalue()->GetRawDataSize();
	}

	return len;


}
char*	EntityGroupPool::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;

	if(bufsize<GetRawDataSize())
		return NULL;

	S_UINT len=0,tlen=sizeof(S_UINT);
	char *pt=lpbuf;

	len=m_data.size();
	memmove(pt,&len,tlen);
	pt+=tlen;

	ENTITYGROUPMAP::iterator it;
	while(m_data.findnext(it))
	{
		len=(*it).getvalue()->GetRawDataSize();
		memmove(pt,&len,tlen);
		pt+=tlen;
		if(!(*it).getvalue()->GetRawData(pt,len))
			return false;
		pt+=len;
	}

	return lpbuf;


}
BOOL	EntityGroupPool::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;
		
	S_UINT len=0,tlen=sizeof(S_UINT);

	try{

		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;

		S_UINT count=0;
		memmove(&count,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;

		EntityGroup *peg=NULL;
		for(S_UINT i=0;i<count;i++)
		{
			memmove(&len,pt,tlen);
			pt+=tlen;
			if(pt>pend)
				return false;

			peg=new EntityGroup();
			if(!peg->CreateObjectByRawData(pt,len))
				return false;
			m_data.insert(peg->GetID(),peg);

			pt+=len;
			if(pt>pend)
				return false;

		}

	}catch(...)
	{
		return false;
	}

	return true;;
}

bool EntityGroupPool::Push(EntityGroup *pm)
{
	if(pm==NULL)
		return false;
	MutexLock lock(m_UpdateLock);

	EntityGroup** ps=m_data.find(pm->GetID());
	if(ps!=NULL)
		delete (*ps);
	m_data[pm->GetID()]=pm;
	m_changed=true;
	return true;

}
bool EntityGroupPool::Push(const char *buf,S_UINT len, Resource *m_pLanguage)
{
	MutexLock lock(m_UpdateLock);
	if(buf==NULL)
		return false;
	if(len<=0)
		return false;
	EntityGroup *ps=new EntityGroup();
	if(ps)
	{
		if(ps->CreateObjectByRawData(buf,len))
		{
			m_changed=true;
			bool ret=Push(ps);
			if(m_pLanguage != NULL)
				SaveNewIds(ps->GetID().getword(), m_pLanguage);
			return ret;
		}
	}
	return false;

}



bool EntityGroupPool::ResolveIds(Resource *m_pLanguage)
{
	if(m_pLanguage==NULL)
	{
		cout<<"resource ForTpl is NULL ."<<endl;
		return false;
	}

	ENTITYGROUPMAP::iterator it;
	while(m_data.findnext(it))
		(*it).getvalue()->ResolveIds(m_pLanguage);
	return true;
}


bool EntityGroupPool::SaveNewIds(string id, Resource *m_pLanguage)
{
	if(m_pLanguage==NULL)
	{
		cout<<"resource ForTpl is NULL ."<<endl;
		return false;
	}
	if( !IdcUser::AutoResolveIDS || !isorig)
		return false;

	EntityGroup * temppoint= GetEntityGroup(id.c_str());
	if(temppoint==NULL)
		return false;

	temppoint->SaveNewIds(m_pLanguage);

	return true;
}
