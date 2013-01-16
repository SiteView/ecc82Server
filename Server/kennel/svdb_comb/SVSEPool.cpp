#include "svsepool.h"

SVSEPool::SVSEPool(void)
{
	m_data.resize(initsize);
	m_hide_data.resize(maxsize);

	m_count=initsize;
	m_loaded=false;
	m_changed=false;
	m_CurrentID=1;

	for(int i=0;i<initsize;i++)
		m_data[i]=NULL;
	for(int i=0;i<maxsize;i++)
		m_hide_data[i]=NULL;
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
	for(int i=0;i<maxsize;i++)
	{
		if(m_hide_data[i]!=NULL)
			delete m_hide_data[i];
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
	if(m_FilePath.empty())
		return false;

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


bool	SVSEPool::GetBackupData(std::list<SingelRecord> & listrcd)
{
	ost::MutexLock lock(m_UpdateLock);

	int datalen= GetRawDataSize(true);
	char * tdata(NULL);
	char * pdata(NULL);
	try{
		pdata=new char[datalen];
		if(datalen==0 || pdata==NULL)
			return false;
		tdata= GetRawData(pdata, datalen, true);
		if(tdata==NULL)
		{
			delete [] pdata;
			return false;
		}
	}
	catch(...)
	{
		delete [] pdata;
		return false;
	}
	SingelRecord rcd;
	rcd.monitorid= "svse_"+IdcUser::GetLocalSEIdStr();
	rcd.data= tdata;
	rcd.datalen= datalen;

	SVSEPool mp;
	if(!mp.CreateObjectByRawData(tdata, datalen))
		return false;

	listrcd.push_back(rcd);
	return true;
}


S_UINT SVSEPool::GetRawDataSize(bool onlyLocked)
{
	S_UINT len=0,tlen=sizeof(S_UINT);

	len+=tlen;                  //m_count;
	len+=tlen;					//real count;
	len+=tlen;					//m_CurrentID;
	for(S_UINT i=0;i<m_count;i++)
	{
		if(m_data[i]!=NULL)
		{
			if(onlyLocked && !IdcUser::WillTeleBackup(m_data[i]->GetIDWord().getword()))
				continue;

			len+=tlen;						//index space;
			len+=tlen;								//data size space;
			len+=m_data[i]->GetRawDataSize(onlyLocked);		//data space;
		}
	}
	if(onlyLocked)
		return len;

	for(int i=0;i<maxsize;i++)
	{
		if(m_hide_data[i]!=NULL)
		{
			len+=tlen;						
			len+=tlen;								
			len+=m_hide_data[i]->GetRawDataSize();	
		}
	}
	return len;

}
char* SVSEPool::GetRawData(char *lpbuf,S_UINT bufsize, bool onlyLocked)
{
	if(lpbuf==NULL)
		return NULL;

	if(bufsize<GetRawDataSize(onlyLocked))
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
		{
			if(onlyLocked && !IdcUser::WillTeleBackup(m_data[n]->GetIDWord().getword()))
				continue;
			count++;
		}
	}
	if(!onlyLocked)
	{
		for(int i=0;i<maxsize;i++)
		{
			if(m_hide_data[i]!=NULL)
				count++;
		}
	}

	memmove(pt,&count,tlen);
	pt+=tlen;

	for(S_UINT i=0;i<m_count;i++)
	{
		if(m_data[i]!=NULL)
		{
			if(onlyLocked && !IdcUser::WillTeleBackup(m_data[i]->GetIDWord().getword()))
				continue;

			memmove(pt,&i,tlen);
			pt+=tlen;
			len=m_data[i]->GetRawDataSize(onlyLocked);
			memmove(pt,&len,tlen);
			pt+=tlen;

			if(m_data[i]->GetRawData(pt,len,onlyLocked)==NULL)
				return NULL;
			pt+=len;
		}
	}
	if(onlyLocked)
		return lpbuf;

	for(int i=0;i<maxsize;i++)
	{
		if(m_hide_data[i]!=NULL)
		{
			memmove(pt,&i,tlen);
			pt+=tlen;
			len=m_hide_data[i]->GetRawDataSize();
			memmove(pt,&len,tlen);
			pt+=tlen;

			if(m_hide_data[i]->GetRawData(pt,len)==NULL)
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
		m_hide_data.resize(maxsize);

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

			NewVersion(index);

			if(IdcUser::SELocked && !IdcUser::IsAnLocalSEId(ps->GetID()))
			{
				m_hide_data[index]=ps;
				continue;
			}

			m_data[index]=ps;
			IdcUser::set_InitLoadSEId.insert(ps->GetID());
		}
	}catch(...)
	{
		return false;
	}

	return true;
}


bool	SVSEPool::UpdateConfig(const char *data,S_UINT datalen)
{
	try{
		if(!IdcUser::AcceptConfigIncoming)
			return false;

		ost::MutexLock lock(m_UpdateLock);
		SVSEPool mp;
		if(!mp.CreateObjectByRawData(data, datalen))
			return false;

		for(int i=0;i<m_count;i++)
		{
			if(!IdcUser::IsAnLocalSEId(i) || !Find(i))
				continue;

			SVSE *pIncome= mp.GetSVSE(i);
			if(pIncome!=NULL)
				push(pIncome);

			mp.GetMemberData()[i]= NULL;
		}

		m_changed=true;
		return Submit();
	}
	catch(...)
	{
		return false;
	}
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

