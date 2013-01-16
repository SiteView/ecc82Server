#include "ResourcePool.h"

ResourcePool::ResourcePool(void)
{
	m_hashtablesize=hashtablesize;
	m_resources.resetsize(m_hashtablesize);
	m_changed=false;

}

ResourcePool::~ResourcePool(void)
{
	RESOURCEMAP::iterator it;
	while(m_resources.findnext(it))
	{
		Resource *prc=(*it).getvalue();
		if(prc!=NULL)
			delete prc;
	}
}

bool ResourcePool::Load(void)
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
		puts("Load ResourcePool Exception");
		return false;
	}


	return	false;
}
bool ResourcePool::Submit()
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
		puts("Submit ResourcePool Exception");
		return false;
	}

	m_changed=false;
	return true;
}


bool ResourcePool::SubmitWithIds(const std::map<string,string> & allids )
{
	ost::MutexLock lock(m_UpdateLock);
	
	std::map<string,string>::const_iterator mit;
	if(!allids.empty() && IdcUser::m_pLanguage!=NULL)
	{  //	Ìí¼Ó newids µ½ this  ÖÐ
		Resource * temp_pLanguage= (Resource *)IdcUser::m_pLanguage;
		if(temp_pLanguage!=NULL)
		{
			StringMap & lang= temp_pLanguage->GetResourceDict();
			for(mit=allids.begin();  mit!=allids.end(); ++mit)
				lang.insert(mit->first.c_str(), mit->second.c_str());
		}
	}

	cout<<"   resource.data SubmitWithIds is done!"<<endl;

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
		puts("Submit ResourcePool Exception");
		return false;
	}

	m_changed=false;
	return true;
}

S_UINT ResourcePool::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);

	len+=tlen;        //m_tablesize size;
	len+=tlen;		  //total  count;

	RESOURCEMAP::iterator it;
	while(m_resources.findnext(it))
	{
		len+=tlen;			//len size;
		len+=(*it).getvalue()->GetRawDataSize();

	}

	return len;

}
char* ResourcePool::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;

	if(bufsize<GetRawDataSize())
		return NULL;

	S_UINT len=0,tlen=sizeof(S_UINT);
	char *pt=lpbuf;

	memmove(pt,&m_hashtablesize,tlen);
	pt+=tlen;

	len=m_resources.size();
	memmove(pt,&len,tlen);
	pt+=tlen;

	RESOURCEMAP::iterator it;
	while(m_resources.findnext(it))
	{
		len=(*it).getvalue()->GetRawDataSize();
		memmove(pt,&len,tlen);
		pt+=tlen;

		if((*it).getvalue()->GetRawData(pt,len)==NULL)
			return NULL;

		pt+=len;
	}

	return lpbuf;
}
BOOL ResourcePool::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;
		
	S_UINT len=0,tlen=sizeof(S_UINT);

	try{

		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;

		memmove(&m_hashtablesize,pt,tlen);
		pt+=tlen;

		m_resources.resetsize(m_hashtablesize);


		S_UINT count=0;
		memmove(&count,pt,tlen);
		pt+=tlen;

		Resource *pm=NULL;
		word id;
		for(int i=0;i<count;i++)
		{
			memmove(&len,pt,tlen);
			pt+=tlen;
			pm=new Resource();
			if(!pm)
				return false;

			if(!pm->CreateObjectByRawData(pt,len))
				return false;
			pt+=len;
			id=pm->GetLanguageType();
			m_resources.insert(id,pm);

		}
	}catch(...)
	{
		return false;
	}

	return true;
}
bool ResourcePool::push(Resource *pm)
{
	ost::MutexLock lock(m_UpdateLock);

	if(!pm)
		return false;
	word id=pm->GetLanguageType();

	Resource **p=m_resources.find(id);
	if(p!=NULL)
		delete (*p);
	m_resources[id]=pm;

	m_changed=true;
	return true;

}
bool ResourcePool::PushData(const char *buf,S_UINT len)
{
	ost::MutexLock lock(m_UpdateLock);
	Resource *pm=new Resource();
	if(pm==NULL)
		return false;
	if(!pm->CreateObjectByRawData(buf,len))
		return false;
	return push(pm);

}
bool ResourcePool::DeleteResource(word language)
{
	ost::MutexLock lock(m_UpdateLock);
	Resource **pm=m_resources.find(language);
	if(pm==NULL)
		return false;

	delete (*pm);

   if(m_resources.erase(language))
   {
	   	m_changed=true;
		return true;
   }

   return false;

}


