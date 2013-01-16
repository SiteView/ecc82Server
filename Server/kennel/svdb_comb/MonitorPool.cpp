#include "MonitorPool.h"


MonitorPool::MonitorPool(void)
{
	m_hashtablesize=hashtablesize;
	m_monitors.resetsize(m_hashtablesize);
	m_hide_monitors.resetsize(m_hashtablesize);

	m_changed=false;
	needToDel=true;

}
MonitorPool::MonitorPool(word filepath):PoolBase2(filepath)
{
	m_hashtablesize=hashtablesize;
	m_monitors.resetsize(m_hashtablesize);
	m_hide_monitors.resetsize(m_hashtablesize);

	m_changed=false;
	needToDel=true;

}

MonitorPool::~MonitorPool(void)
{
	if(IdcUser::BoolToExit)
		return ;

	if(!needToDel)
		return;

	if(m_changed)
		Submit();

	MONITORMAP::iterator it;
	while(m_monitors.findnext(it))
	{
		Monitor *pm=(*it).getvalue();
		if(pm)
			delete pm;
	}
	MONITORMAP::iterator hit;
	while(m_hide_monitors.findnext(hit))
	{
		Monitor *pm=(*hit).getvalue();
		if(pm)
			delete pm;
	}
}

int MonitorPool::QueryMassMonitor(string pid, const char *data,S_UINT datalen, std::list<SingelRecord> & listrcd )
{
	StringMap smap(577);
	try{
		smap.CreateObjectByRawData(data,datalen);
	}
	catch(...)
	{
		printf("Exception in MonitorPool::QueryMassMonitor 1 \n");
	}

	bool isdefault(false);
	if(pid.compare("default")==0)
		isdefault=true;
	else
		pid+=".";

	string mid;
	MONITORMAP::iterator it;
	while(m_monitors.findnext(it))
	{
		const char * key = (*it).getkey().getword();
		mid=key;
		if( !isdefault && mid.find(pid)!=0)
			continue;

		word *pv=NULL;
		S_UINT sver = GetVersion(key);
		if( (pv=smap.find(key))!=NULL )
		{
			S_UINT ver= atoi( (*pv).getword() );
			smap.erase(key);
			if( ver== sver )
				continue;
		}

		char * tempchar=NULL;
		SingelRecord rcd;
		try{
			Monitor **pm=m_monitors.find(key);
			if(pm==NULL)
				continue;
			S_UINT size=(*pm)->GetRawDataSize();

			rcd.monitorid= key;
			rcd.datalen= size+sizeof(S_UINT);
			tempchar= new char[size+sizeof(S_UINT)];
			memmove(tempchar,&sver,sizeof(S_UINT));
			if( (*pm)->GetRawData(tempchar+sizeof(S_UINT), size)==NULL )
			{
				if(tempchar!=NULL)
					delete [] tempchar;
				continue;
			}
			rcd.data= tempchar;
		}
		catch(...)
		{
			printf("Exception in MonitorPool::QueryMassMonitor 2 \n");
			if(tempchar!=NULL)
				delete [] tempchar;
			continue;
		}
		listrcd.push_back(rcd);
	}
	StringMap::iterator mit;
	while(smap.findnext(mit))
	{
		SingelRecord rcd;
		rcd.monitorid= (*mit).getkey().getword();
		rcd.data= NULL;
		rcd.datalen= 0;
		listrcd.push_back(rcd);
	}
	return listrcd.size();
}

bool MonitorPool::SubmitToFile(string fileName, MonitorPool * dataPool )
{
	S_UINT len=dataPool->GetRawDataSize();
	try{
		ost::MappedFile file(fileName.c_str(),ost::File::accessReadWrite,len);
		char *p=(char *)file.fetch(0,0);
		if(p)
		{
			if(dataPool->GetRawData(p,len)==NULL)
				return false;
		}

	}catch(...)
	{
		puts("Submit MonitorPool Exception");
		return false;
	}
	return true;
}

#include<map>

bool MonitorPool::Submit(std::string modifyid)
{
	ost::MutexLock lock(m_UpdateLock); 
	if(!m_changed)
		return true;
	if(m_FilePath.empty())
		return false;

	if( !IdcUser::EnableIdc )
	{  //for non-idc
		if( !SubmitToFile(m_FilePath.getword(), this) )
			return false;
	}
	else
	{  // for idc
		string mUid= TruncateToUId( modifyid );
		if( !MoreThan2Dot(modifyid) )  
			mUid="top";
		cout<<"ModifyId:\""<<modifyid.c_str()<<"\"   ";

		std::map<string,MONITORMAP> idcMoni;
		std::map<string,MONITORMAP>::iterator mit;

		MONITORMAP::iterator git;
		while(m_monitors.findnext(git))
		{
			string gid= (*git).getkey().getword();
			string uid= TruncateToUId( gid );

			if( !MoreThan2Dot(gid) ) 
				uid="top";

			if( !modifyid.empty() && uid!=mUid )  
				continue;

			mit= idcMoni.find(uid);
			if(mit==idcMoni.end())
			{
				MONITORMAP gmap;
				idcMoni.insert(make_pair(uid,gmap));
				mit= idcMoni.find(uid);

			}
			if(mit==idcMoni.end())
				continue;
			(mit->second).insert(gid,(*git).getvalue());
		}

		for(mit=idcMoni.begin(); mit!=idcMoni.end(); ++mit)
		{   
			string fname= IdcUser::RootPath; fname += "/data/idc_data/"; 
			if( (mit->first)!="top" )
				fname+= mit->first + "/" ; 
			fname+= "Monitor.data";

			MonitorPool temppool(mit->second);
			cout<<fname.c_str()<<endl;
			if( !SubmitToFile(fname.c_str(), &temppool) )  
				continue;
			cout<<" -- updated!"<<endl;
		}
	}

	m_changed=false;
	return true;

}


bool MonitorPool::LoadFile(string fileName)
{
	S_UINT len=0;
	try{

		{
			ost::ThreadFile fl(fileName.c_str());
			len =fl.getCapacity();
		}

		if(len<=0)
			return false;
	

		ost::MappedFile file(fileName.c_str(),0,0,ost::File::accessReadWrite);
		char*p=(char *)file.fetch(0,0);
		if(p)
		{
			return CreateObjectByRawData(p,len);
		}


	}catch(...)
	{
		puts("Load MonitorPool Exception");
		return false;
	}


	return	false;

}


bool MonitorPool::Load(void)
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
		puts("Load MonitorPool Exception");
		return false;
	}


	return	false;
}

bool	MonitorPool::GetBackupData(std::list<SingelRecord> & listrcd)
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
		printf(" Exception in MonitorPool::GetBackupData!  ");
		delete [] pdata;
		return false;
	}
	SingelRecord rcd;
	rcd.monitorid= "monitor_"+IdcUser::GetLocalSEIdStr();
	rcd.data= tdata;
	rcd.datalen= datalen;

	MonitorPool mp;
	if(!mp.CreateObjectByRawData(tdata, datalen))
		return false;

	listrcd.push_back(rcd);
	return true;
}

S_UINT	MonitorPool::GetRawDataSize(bool onlyLocked)
{
	S_UINT len=0,tlen=sizeof(S_UINT);

	len+=tlen;        //m_tablesize size;
	len+=tlen;		  //total monitor count;

	MONITORMAP::iterator it;
	while(m_monitors.findnext(it))
	{
		if(onlyLocked && !IdcUser::WillTeleBackup((*it).getvalue()->GetID().getword()))
			continue;
		len+=tlen;			//len size;
		len+=(*it).getvalue()->GetRawDataSize();

	}
	if(onlyLocked)
		return len;

	MONITORMAP::iterator hit;
	while(m_hide_monitors.findnext(hit))
	{
		len+=tlen;	
		len+=(*hit).getvalue()->GetRawDataSize();

	}
	return len;
}
char*	MonitorPool::GetRawData(char *lpbuf,S_UINT bufsize, bool onlyLocked)
{
	if(lpbuf==NULL)
		return NULL;

	if(bufsize<GetRawDataSize(onlyLocked))
		return NULL;

	S_UINT len=0,tlen=sizeof(S_UINT);
	char *pt=lpbuf;

	memmove(pt,&m_hashtablesize,tlen);
	pt+=tlen;

	len=m_monitors.size();
	if(onlyLocked)
	{
		S_UINT tempcount=0;
		MONITORMAP::iterator tempit;
		while(m_monitors.findnext(tempit))
		{
			if(IdcUser::WillTeleBackup((*tempit).getvalue()->GetID().getword()))
				++tempcount;
		}
		len= tempcount;
	}

	if(!onlyLocked)
		len+= m_hide_monitors.size();
	memmove(pt,&len,tlen);
	pt+=tlen;

	MONITORMAP::iterator it;
	while(m_monitors.findnext(it))
	{
		if(onlyLocked && !IdcUser::WillTeleBackup((*it).getvalue()->GetID().getword()))
			continue;

		len=(*it).getvalue()->GetRawDataSize();
		memmove(pt,&len,tlen);
		pt+=tlen;

		if((*it).getvalue()->GetRawData(pt,len)==NULL)
			return NULL;

		pt+=len;
	}
	if(onlyLocked)
		return lpbuf;

	MONITORMAP::iterator hit;
	while(m_hide_monitors.findnext(hit))
	{
		len=(*hit).getvalue()->GetRawDataSize();
		memmove(pt,&len,tlen);
		pt+=tlen;

		if((*hit).getvalue()->GetRawData(pt,len)==NULL)
			return NULL;

		pt+=len;
	}

	return lpbuf;
}

BOOL	MonitorPool::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;
		
	S_UINT len=0,tlen=sizeof(S_UINT);

	try{

		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;

		memmove(&m_hashtablesize,pt,tlen);
		pt+=tlen;

		m_monitors.resetsize(m_hashtablesize);
		m_hide_monitors.resetsize(m_hashtablesize);

		S_UINT count=0;
		memmove(&count,pt,tlen);
		pt+=tlen;

		Monitor *pm=NULL;
		word id;
		for(int i=0;i<count;i++)
		{
			memmove(&len,pt,tlen);
			pt+=tlen;
			pm=new Monitor();
			if(!pm)
				return false;

			if(!pm->CreateObjectByRawData(pt,len))
				return false;
			pt+=len;
			id=pm->GetID();

			NewVersion(id);
			if(IdcUser::SELocked && !IdcUser::IsAnLocalSEId(id.getword()))
			{
				m_hide_monitors.insert(id,pm);
				continue;
			}
			m_monitors.insert(id,pm);
		}
	}catch(...)
	{
		return false;
	}


	return true;
}

bool	MonitorPool::UpdateConfig(string sestr, const char *data,S_UINT datalen)
{
	try{
		if(!IdcUser::AcceptConfigIncoming || sestr.empty())
			return false;

		ost::MutexLock lock(m_UpdateLock);
		sestr= ","+sestr+",";
		MonitorPool mp;
		if(!mp.CreateObjectByRawData(data, datalen))
			return false;

		MONITORMAP mDel;
		mDel.resetsize(m_hashtablesize);
		MONITORMAP::iterator git;
		while(m_monitors.findnext(git))
		{
			string id= (*git).getkey().getword();
			string seid= GetTopID(id).getword();
			if(sestr.find(","+seid+",")==std::string::npos)
				continue;
			if(!IdcUser::IsAnLocalSEId(id))
				continue;

			Monitor *pIncome= mp.GetMonitor(id.c_str());
			if(pIncome==NULL)
				mDel.insert(id,NULL);
		}
	
		MONITORMAP & ingroups= mp.GetMemberData();
		MONITORMAP::iterator git3;
		while(ingroups.findnext(git3))
		{
			string id= (*git3).getkey().getword();
			if(!IdcUser::IsAnLocalSEId(id))
				continue;

			Monitor **p= m_monitors.find(id);
			if(p!=NULL)
				delete (*p);
			m_monitors[id]= (*git3).getvalue();
			NewVersion(id);

			(*git3).setvalue(NULL);
		}

		MONITORMAP::iterator git2;
		while(mDel.findnext(git2))
		{
			string id= (*git2).getkey().getword();
			Monitor **pm=m_monitors.find(id);
			if(pm!=NULL)
				delete (*pm);

			if(m_monitors.erase(id))
				m_mversion.erase(id);
		}

		m_changed=true;
		return Submit();
	}
	catch(...)
	{
		return false;
	}
}

bool MonitorPool::push(Monitor *pm)
{
	ost::MutexLock lock(m_UpdateLock);

	if(!pm)
		return false;
	word id=pm->GetID();

	if(IdcUser::SELocked && !IdcUser::IsAnLocalSEId(id.getword()))
		return false;

	Monitor **p=m_monitors.find(id);
	if(p!=NULL)
		delete (*p);
	m_monitors[id]=pm;
	NewVersion(id);
	cout<<"Latest monitor "<<id.getword()<<" version is: "<<GetVersion(id)<<endl;

	m_changed=true;
	return true;
	
}

bool MonitorPool::PushData(const char *buf,S_UINT len)
{
	ost::MutexLock lock(m_UpdateLock);
	Monitor *pm=new Monitor();
	if(pm==NULL)
		return false;
	if(!pm->CreateObjectByRawData(buf,len))
		return false;
	return push(pm);

}

bool MonitorPool::GetMonitorData(word id,char *buf,S_UINT &len)
{
	Monitor **pm=m_monitors.find(id);
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

bool MonitorPool::DeleteMonitor(word id)
{
	ost::MutexLock lock(m_UpdateLock);

	Monitor **pm=m_monitors.find(id);
	if(pm!=NULL)
		delete (*pm);

   if(m_monitors.erase(id))
   {
	   m_mversion.erase(id.getword());
	   	m_changed=true;
		return true;
   }

   return false;

}

bool MonitorPool::ReSetBackupId()
{
	if(IdcUser::FullTeleBackup)
		return true;
	std::set<std::string> backupId;

	StringMap smap(577);
	if(GetInfo("sv_telebackup", "default", smap, false))
	{
		StringMap::iterator smit;
		while(smap.findnext(smit))
		{
			try{
				string value= (*smit).getvalue().getword();
				if(value.compare("true")!=0)
					continue;
				string pid= (*smit).getkey().getword();
				backupId.insert(pid);

				std::string::size_type lastpos;	
				while( (lastpos=pid.rfind("."))!=std::string::npos )
				{
					pid.erase(lastpos);
					backupId.insert(pid);
				}
			}
			catch(...)
			{
				continue;
			}
		}
	}

	//printf(" ****  ReSetBackupId, id count: %d\n", backupId.size());
	IdcUser::SetTeleBackupId(backupId);
	return true;
}

