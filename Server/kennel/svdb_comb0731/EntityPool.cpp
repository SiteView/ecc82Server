#include "entitypool.h"

EntityPool::EntityPool(void)
{
	m_hashtablesize=hashtablesize;
	m_entitys.resetsize(m_hashtablesize);
	m_changed=false;
	needToDel=true;
}

EntityPool::EntityPool(word filepath):PoolBase2(filepath)
{
	m_hashtablesize=hashtablesize;
	m_entitys.resetsize(m_hashtablesize);
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


S_UINT	EntityPool::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);

	len+=tlen;        //m_tablesize size;
	len+=tlen;		  //total monitor count;

	ENTITYMAP::iterator it;
	while(m_entitys.findnext(it))
	{
		len+=tlen;			//len size;
		len+=(*it).getvalue()->GetRawDataSize();

	}

	return len;
}
char*	EntityPool::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;

	if(bufsize<GetRawDataSize())
		return NULL;

	S_UINT len=0,tlen=sizeof(S_UINT);
	char *pt=lpbuf;

	memmove(pt,&m_hashtablesize,tlen);
	pt+=tlen;

	len=m_entitys.size();
	memmove(pt,&len,tlen);
	pt+=tlen;

	ENTITYMAP::iterator it;
	while(m_entitys.findnext(it))
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
			m_entitys.insert(id,pm);
			NewVersion(id);

		}
	}catch(...)
	{
		return false;
	}

	return true;
}

bool EntityPool::push(Entity *pm)
{
	ost::MutexLock lock(m_UpdateLock);

	if(!pm)
		return false;
	word id=pm->GetID();

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
	if(pm==NULL)
		return false;
	delete (*pm);
   if(m_entitys.erase(id))
   {
	   m_mversion.erase(id.getword());
	   m_changed=true;
	   return true;
   }
   return false;

}




