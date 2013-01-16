#include "GeneralPool.h"

GeneralPool::GeneralPool(void)
{
	m_hashtablesize=hashtablesize;
	m_data.resetsize(m_hashtablesize);
	m_changed=false;

}

GeneralPool::GeneralPool(word filepath):PoolBase(filepath)
{
	m_hashtablesize=hashtablesize;
	m_data.resetsize(m_hashtablesize);
	m_changed=false;	
}

GeneralPool::~GeneralPool(void)
{
	if(m_changed)
		Submit();

	COMMONDATAMAP::iterator it;
	while(m_data.findnext(it))
	{
		Section *ps=(*it).getvalue();
		if(ps)
			delete ps;
	}

}

S_UINT	GeneralPool::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);

	len+=tlen;			//m_hashtablesize
	len+=tlen;			//data count;

	WORDLIST::iterator wit;
	Section **pdata=NULL;
	for(wit=m_SectionOrder.begin();wit!=m_SectionOrder.end();wit++)
	{
		pdata=m_data.find((*wit));
		if(pdata!=NULL)
		{
			len+=tlen;   //data len;
			len+=(*pdata)->GetRawDataSize();

		}
	}

/*	
	COMMONDATAMAP::iterator it;
	while(m_data.findnext(it))
	{
		len+=tlen;			//len size;
		len+=(*it).getvalue()->GetRawDataSize();

	}
*/
	return len;


}
char*	GeneralPool::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;

	if(bufsize<GetRawDataSize())
		return NULL;

	S_UINT len=0,tlen=sizeof(S_UINT);
	char *pt=lpbuf;

	memmove(pt,&m_hashtablesize,tlen);
	pt+=tlen;

//	len=m_data.size();
	len=m_SectionOrder.size();
	memmove(pt,&len,tlen);
	pt+=tlen;

	WORDLIST::iterator wit;
	Section **pdata=NULL;
	for(wit=m_SectionOrder.begin();wit!=m_SectionOrder.end();wit++)
	{
		pdata=m_data.find((*wit));
		if(pdata!=NULL)
		{
			len=(*pdata)->GetRawDataSize();
			memmove(pt,&len,tlen);
			pt+=tlen;

			if(!(*pdata)->GetRawData(pt,len))
				return NULL;
			pt+=len;

		}
	}

/*
	COMMONDATAMAP::iterator it;
	while(m_data.findnext(it))
	{
		len=(*it).getvalue()->GetRawDataSize();
		memmove(pt,&len,tlen);
		pt+=tlen;

		if(!(*it).getvalue()->GetRawData(pt,len))
			return NULL;
		pt+=len;
	}
*/
    
	return lpbuf;
}
BOOL	GeneralPool::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;
		
	S_UINT len=0,tlen=sizeof(S_UINT);

	try{

		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;

		memmove(&m_hashtablesize,pt,tlen);
		pt+=tlen;

		if(m_hashtablesize<=0)
			return false;

		m_data.resetsize(m_hashtablesize);

		S_UINT count=0;

		memmove(&count,pt,tlen);
		pt+=tlen;

		Section *ps=NULL;

		for(S_UINT i=0;i<count;i++)
		{
			memmove(&len,pt,tlen);
			pt+=tlen;
			ps=new Section();
			if(!ps)
				return false;
			if(!ps->CreateObjectByRawData(pt,len))
				return false;
			m_data.insert(ps->GetSection(),ps);
			m_SectionOrder.push_back(ps->GetSection());
			pt+=len;
		}

	}catch(...)
	{
		return false;
	}

	return true;
}

bool GeneralPool::Load(void)
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

		printf("Load GeneralPool Exception,Errorid:%d,filename:%s",::GetLastError(),m_FilePath.getword());
		return false;
	}


	return	false;

}
bool GeneralPool::Submit()
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
	
		printf("Submit GeneralPool Exception,Errorid:%d,filename:%s\n",::GetLastError(),m_FilePath.getword());
		return false;
	}

	m_changed=false;
	return true;
}
bool GeneralPool::WriteData(word section,word key,void *data,S_UINT len)
{
	MutexLock lock(m_UpdateLock);

	if(!Find(section))
	{
		Section *ps=new Section();
		if(!ps)
			return false;

		ps->PutSection(section);

		if(!ps->Push(key,(const char *)data,len))
			return false;
		m_data[section]=ps;
		m_SectionOrder.push_back(section);
		m_changed=true;
		return true;
	}else
	{
		m_changed=true;
		return m_data[section]->Push(key,(const char *)data,len);
	}

	return true;
}
bool GeneralPool::GetData(word section,word key,void *data,S_UINT &len)
{
	if(!Find(section))
		return false;
	void *pdata=NULL;
	S_UINT dlen=0;
	if(!m_data[section]->Pop(key,(void **)&pdata,dlen))
		return false;
    
	if(data==NULL)
	{
		len=dlen;
		return true;
	}
	if(len<dlen)
		return false;
	memcpy(data,pdata,dlen);
	return true;
}

bool GeneralPool::WriteString(word section, word key, word str)
{
	MutexLock lock(m_UpdateLock);

	if(!Find(section))
	{
		Section *ps=new Section();
		if(!ps)
			return false;
		ps->PutSection(section);
		if(!ps->Push(key,str))
			return false;
		m_data[section]=ps;
		m_SectionOrder.push_back(section);
		m_changed=true;
		return true;
	}else
	{
		m_changed=true;
		return m_data[section]->Push(key,str);
	}

	return true;
}
word GeneralPool::GetString(word section, word key,word defaultret)
{
	if(!Find(section))
		return defaultret;

	word str;
	if(!m_data[section]->Pop(key,str))
		return defaultret;
	return str;
}

bool GeneralPool::WriteInt(word section,word key,int value)
{
	MutexLock lock(m_UpdateLock);

	if(!Find(section))
	{
		Section *ps=new Section();
		if(!ps)
			return false;
		ps->PutSection(section);
		if(!ps->Push(key,value))
			return false;
		m_data[section]=ps;
		m_SectionOrder.push_back(section);
		m_changed=true;
		return true;
	}else
	{
		m_changed=true;
		return m_data[section]->Push(key,value);
	}

	return true;
}
int GeneralPool::GetInt(word section,word key, int defaultret)
{
	if(!Find(section))
		return defaultret;
	int value=0;
	if(!m_data[section]->Pop(key,value))
    	return defaultret;
	return value;
}

bool GeneralPool::GetSectionsName(std::list<string> &sections)
{
	WORDLIST::iterator it;
	for(it=m_SectionOrder.begin();it!=m_SectionOrder.end();it++)
		sections.push_back((*it).getword());

/*	COMMONDATAMAP::iterator it;
	while(m_data.findnext(it))
	{
		sections.push_back((*it).getkey().getword());
	}
*/
	return true;
}
bool GeneralPool::GetKeysName(word section,std::list<string> &keylist)
{
	if(!Find(section))
		return false;

	return m_data[section]->GetKeys(keylist);
}

int  GeneralPool::GetValueTypeBySectionAndKey(word section,word key)
{
	if(!Find(section))
		return -1;

	return m_data[section]->GetValueTypeByKey(key);

}



