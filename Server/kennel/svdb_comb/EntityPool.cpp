#include "entitypool.h"
#include "Des.h"

EntityPool::EntityPool(void)
{
	m_hashtablesize=hashtablesize;
	m_entitys.resetsize(m_hashtablesize);
	m_hide_entitys.resetsize(m_hashtablesize);
	m_changed=false;
	needToDel=true;
}

EntityPool::EntityPool(word filepath):PoolBase2(filepath)
{
	m_hashtablesize=hashtablesize;
	m_entitys.resetsize(m_hashtablesize);
	m_hide_entitys.resetsize(m_hashtablesize);
	m_changed=false;
	needToDel=true;
}


EntityPool::~EntityPool(void)
{
	if(IdcUser::BoolToExit)
		return ;

	if(!needToDel)
		return;

	if(m_changed)
		Submit();

	ENTITYMAP::iterator it;
	while(m_entitys.findnext(it))
	{
		Entity *pe=(*it).getvalue();
		if(pe)
			delete pe;
	}

	ENTITYMAP::iterator hit;
	while(m_hide_entitys.findnext(hit))
	{
		Entity *pe=(*hit).getvalue();
		if(pe)
			delete pe;
	}
}


int EntityPool::QueryMassEntity(string pid, const char *data,S_UINT datalen, std::list<SingelRecord> & listrcd )
{
	StringMap smap(577);
	try{
		smap.CreateObjectByRawData(data,datalen);
	}
	catch(...)
	{
		printf("Exception in EntityPool::QueryMassEntity 1 \n");
	}

	bool isdefault(false);
	if(pid.compare("default")==0)
		isdefault=true;
	else
		pid+=".";

	string mid;
	ENTITYMAP::iterator it;
	while(m_entitys.findnext(it))
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
			Entity **pm=m_entitys.find(key);
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
			printf("Exception in EntityPool::QueryMassEntity 2 \n");
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


bool EntityPool::SubmitToFile(string fileName, EntityPool * dataPool )
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
		puts("Submit EntityPool Exception");
		return false;
	}
	return true;
}

#include<map>

bool EntityPool::Submit(std::string modifyid)
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
		if( mUid.compare(modifyid)==0 ) 
			mUid="top";
		cout<<"ModifyId:\""<<modifyid.c_str()<<"\"   ";

		std::map<string,ENTITYMAP> idcEntity;
		std::map<string,ENTITYMAP>::iterator mit;

		ENTITYMAP::iterator git;
		while(m_entitys.findnext(git))
		{
			string gid= (*git).getkey().getword();
			string uid= TruncateToUId( gid );
			if( uid.compare(gid)==0 ) 
				uid="top";

			if( !modifyid.empty() && uid!=mUid )  
				continue;

			mit= idcEntity.find(uid);
			if(mit==idcEntity.end())
			{
				ENTITYMAP gmap;
				idcEntity.insert(make_pair(uid,gmap));
				mit= idcEntity.find(uid);

			}
			if(mit==idcEntity.end())
				continue;
			(mit->second).insert(gid,(*git).getvalue());
		}

		for(mit=idcEntity.begin(); mit!=idcEntity.end(); ++mit)
		{   
			string fname= IdcUser::RootPath; fname += "/data/idc_data/"; 
			if( (mit->first)!="top" )
				fname+= mit->first + "/" ; 
			fname+= "Entity.data";

			EntityPool temppool(mit->second);
			cout<<fname.c_str();
			if( !SubmitToFile(fname.c_str(), &temppool) )  
				continue;
			cout<<" -- updated!"<<endl;
		}
	}

	m_changed=false;
	return true;

}


bool EntityPool::LoadFile(string fileName)
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
		puts("Load EntityPool Exception");
		return false;
	}


	return	false;

}



bool EntityPool::Load(void)
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


bool	EntityPool::GetBackupData(std::list<SingelRecord> & listrcd)
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
	rcd.monitorid= "entity_"+IdcUser::GetLocalSEIdStr();
	rcd.data= tdata;
	rcd.datalen= datalen;

	EntityPool mp;
	if(!mp.CreateObjectByRawData(tdata, datalen))
		return false;

	listrcd.push_back(rcd);
	return true;
}

S_UINT	EntityPool::GetRawDataSize(bool onlyLocked)
{
	S_UINT len=0,tlen=sizeof(S_UINT);

	len+=tlen;        //m_tablesize size;
	len+=tlen;		  //total monitor count;

	ENTITYMAP::iterator it;
	while(m_entitys.findnext(it))
	{
		if(onlyLocked && !IdcUser::WillTeleBackup((*it).getvalue()->GetID().getword()))
			continue;
		len+=tlen;			//len size;
		len+=(*it).getvalue()->GetRawDataSize(onlyLocked);

	}
	if(onlyLocked)
		return len;

	ENTITYMAP::iterator hit;
	while(m_hide_entitys.findnext(hit))
	{
		len+=tlen;
		len+=(*hit).getvalue()->GetRawDataSize();

	}
	return len;
}
char*	EntityPool::GetRawData(char *lpbuf,S_UINT bufsize, bool onlyLocked)
{
	if(lpbuf==NULL)
		return NULL;

	if(bufsize<GetRawDataSize(onlyLocked))
		return NULL;

	S_UINT len=0,tlen=sizeof(S_UINT);
	char *pt=lpbuf;

	memmove(pt,&m_hashtablesize,tlen);
	pt+=tlen;

	len=m_entitys.size();
	if(onlyLocked)
	{
		S_UINT tempcount=0;
		ENTITYMAP::iterator tempit;
		while(m_entitys.findnext(tempit))
		{
			if(IdcUser::WillTeleBackup((*tempit).getvalue()->GetID().getword()))
				++tempcount;
		}
		len= tempcount;
	}

	if(!onlyLocked)
		len+= m_hide_entitys.size();
	memmove(pt,&len,tlen);
	pt+=tlen;

	ENTITYMAP::iterator it;
	while(m_entitys.findnext(it))
	{
		if(onlyLocked && !IdcUser::WillTeleBackup((*it).getvalue()->GetID().getword()))
			continue;
		len=(*it).getvalue()->GetRawDataSize(onlyLocked);
		memmove(pt,&len,tlen);
		pt+=tlen;

		if((*it).getvalue()->GetRawData(pt,len,onlyLocked)==NULL)
			return NULL;

		pt+=len;
	}
	if(onlyLocked)
		return lpbuf;

	ENTITYMAP::iterator hit;
	while(m_hide_entitys.findnext(hit))
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
BOOL EntityPool::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;
		
	S_UINT len=0,tlen=sizeof(S_UINT);

	const char *pend=lpbuf+bufsize;
	const char *pt=lpbuf;
	try{

		memmove(&m_hashtablesize,pt,tlen);
		pt+=tlen;

		m_entitys.resetsize(m_hashtablesize);
		m_hide_entitys.resetsize(m_hashtablesize);

		S_UINT count=0;
		memmove(&count,pt,tlen);
		pt+=tlen;

		Entity *pm=NULL;
		word id;
		for(int i=0;i<count;i++)
		{
			memmove(&len,pt,tlen);
			pt+=tlen;
			pm=new Entity();
			if(!pm)
				return false;

			if(!pm->CreateObjectByRawData(pt,len))
				return false;
			pt+=len;
			id=pm->GetID();

			NewVersion(id);
			if(IdcUser::SELocked && !IdcUser::IsAnLocalSEId(id.getword()))
			{
				m_hide_entitys.insert(id,pm);
				continue;
			}
			m_entitys.insert(id,pm);
			
		}
	}catch(...)
	{
		return false;
	}

	return true;
}


bool	EntityPool::UpdateConfig(string sestr, const char *data,S_UINT datalen)
{
	try{
		if(!IdcUser::AcceptConfigIncoming || sestr.empty())
			return false;

		ost::MutexLock lock(m_UpdateLock);
		sestr= ","+sestr+",";
		EntityPool mp;
		if(!mp.CreateObjectByRawData(data, datalen))
			return false;

		ENTITYMAP mDel;
		mDel.resetsize(m_hashtablesize);
		ENTITYMAP::iterator git;
		while(m_entitys.findnext(git))
		{
			string id= (*git).getkey().getword();
			string seid= GetTopID(id).getword();
			if(sestr.find(","+seid+",")==std::string::npos)
				continue;
			if(!IdcUser::IsAnLocalSEId(id))
				continue;

			Entity *pIncome= mp.GetEntity(id.c_str());
			if(pIncome==NULL)
				mDel.insert(id,NULL);
		}
			
		ENTITYMAP & ingroups= mp.GetMemberData();
		ENTITYMAP::iterator git3;
		while(ingroups.findnext(git3))
		{
			string id= (*git3).getkey().getword();
			if(!IdcUser::IsAnLocalSEId(id))
				continue;

			Entity **p= m_entitys.find(id);
			if(p!=NULL)
				delete (*p);
			m_entitys[id]= (*git3).getvalue();
			NewVersion(id);

			(*git3).setvalue(NULL);
		}

		ENTITYMAP::iterator git2;
		while(mDel.findnext(git2))
		{
			string id= (*git2).getkey().getword();
			Entity **pm=m_entitys.find(id);
			if(pm!=NULL)
				delete (*pm);

			if(m_entitys.erase(id))
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


bool EntityPool::push(Entity *pm)
{
	ost::MutexLock lock(m_UpdateLock);

	if(!pm)
		return false;
	word id=pm->GetID();

	if(IdcUser::SELocked && !IdcUser::IsAnLocalSEId(id.getword()))
		return false;

	Entity **p=m_entitys.find(id);
	if(p!=NULL)
		delete (*p);
	m_entitys[id]=pm;
	NewVersion(id);
	cout<<"Latest entity "<<id.getword()<<" version is: "<<GetVersion(id)<<endl;

	m_changed=true;
	return true;
}
bool EntityPool::PushData(const char *buf,S_UINT len)
{
	ost::MutexLock lock(m_UpdateLock);
	Entity *pm=new Entity();
	if(pm==NULL)
		return false;
	if(!pm->CreateObjectByRawData(buf,len))
		return false;
	return push(pm);

}

bool EntityPool::GetEntityData(word id,char *buf,S_UINT &len)
{
	Entity **pm=m_entitys.find(id);
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
bool EntityPool::DeleteEntity(word id)
{
	ost::MutexLock lock(m_UpdateLock);
	Entity **pm=m_entitys.find(id);
	if(pm!=NULL)
		delete (*pm);
   if(m_entitys.erase(id))
   {
	   m_mversion.erase(id.getword());
	   m_changed=true;
	   return true;
   }
   return false;

}

bool EntityPool::SaveDataFromNnmEntity(string id, string exid, StringMap & smap)
{
	ost::MutexLock lock(m_UpdateLock);
	Entity ** pm= m_entitys.find(id.c_str());
	if(pm==NULL||*pm==NULL)
		return false;
	m_changed=true;

	string Community;
	bool hasIt(false);
	StringMap::iterator sit;
	while(smap.findnext(sit))
	{
		string key= (*sit).getkey().getword();
		if(key.compare("_Community")!=0)
			(*pm)->GetProperty()[(*sit).getkey()]=(*sit).getvalue();
		else
		{
			hasIt= true;
			Community= (*sit).getvalue().getword();
		}
	}

	if(!Community.empty())
	{
		Des mydes;
		char out[1024]={0};
		try{
			if( mydes.Encrypt(Community.c_str(), out) )
				Community= out;	
		}catch(...)
		{
		}
	}
	if(hasIt)
		(*pm)->GetProperty()["_Community"]= Community.c_str();

	NewVersion(id);
	cout<<"Ecc entity updated by nnm entity, id/version/nnm entity: "<<id.c_str()<<" / "<<GetVersion(id)<<" / "<<exid.c_str()<<endl;
	return true;
}



