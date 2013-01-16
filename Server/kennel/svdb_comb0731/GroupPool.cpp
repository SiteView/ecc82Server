#include "grouppool.h"

GroupPool::GroupPool(void)
{
	m_hashtablesize=hashtablesize;
	m_groups.resetsize(m_hashtablesize);
	m_changed=false;
	needToDel=true;

}
GroupPool::GroupPool(word filepath):PoolBase2(filepath)
{
	m_hashtablesize=hashtablesize;
	m_groups.resetsize(m_hashtablesize);
	m_changed=false;
	needToDel=true;
}


GroupPool::~GroupPool(void)
{
	if(IdcUser::BoolToExit)
		return ;

	if(!needToDel)
		return;

	if(m_changed)
		Submit();

	GROUPMAP::iterator it;
	while(m_groups.findnext(it))
	{
		Group *pg=(*it).getvalue();
		if(pg)
			delete pg;
	}

}
bool GroupPool::Load(void)
{
	S_UINT len=0;
	try{

		{
			ost::ThreadFile fl(m_FilePath.getword()); // can creat new file without any data
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

bool GroupPool::SubmitToFile(string fileName, GroupPool * dataPool )
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
		puts("Submit GroupPool Exception");
		return false;
	}
	return true;
}

#include<map>

bool GroupPool::Submit(std::string modifyid)
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
		cout<<"ModifyId:\""<<modifyid.c_str()<<"\"   ";

		std::map<string,GROUPMAP> idcGroups;
		std::map<string,GROUPMAP>::iterator mit;

		GROUPMAP::iterator git;
		while(m_groups.findnext(git))
		{
			string gid= (*git).getkey().getword();
			string uid= TruncateToUId( gid );
			if( !modifyid.empty() && uid.compare(mUid)!=0 )    
				continue;

			mit= idcGroups.find(uid);
			if(mit==idcGroups.end())
			{
				GROUPMAP gmap;
				idcGroups.insert(make_pair(uid,gmap));
				mit= idcGroups.find(uid);

			}
			if(mit==idcGroups.end())
				continue;
			(mit->second).insert(gid,(*git).getvalue());
		}

		for(mit=idcGroups.begin(); mit!=idcGroups.end(); ++mit)
		{  
			string fname= IdcUser::RootPath; fname += "/data/idc_data/"; fname+= mit->first ; 
			IdcUser tempuser;
			tempuser.CreatDir(fname.c_str());
			fname+= "/Group.data";

			GroupPool temppool(mit->second);
			cout<<fname.c_str();
			if( !SubmitToFile(fname.c_str(), &temppool) )  
				continue;
			cout<<" -- updated!"<<endl;
		}
	}

	m_changed=false;
	return true;

}


bool GroupPool::LoadFile(string fileName)
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
		puts("Load GroupPool Exception");
		return false;
	}


	return	false;

}


S_UINT	GroupPool::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);

	len+=tlen;        //m_tablesize size;
	len+=tlen;		  //total monitor count;

	GROUPMAP::iterator it;
	while(m_groups.findnext(it))
	{
		len+=tlen;			//len size;
		len+=(*it).getvalue()->GetRawDataSize();

	}

	return len;

}
char*	GroupPool::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;

	if(bufsize<GetRawDataSize())
		return NULL;

	S_UINT len=0,tlen=sizeof(S_UINT);
	char *pt=lpbuf;

	memmove(pt,&m_hashtablesize,tlen);
	pt+=tlen;

	len=m_groups.size();
	memmove(pt,&len,tlen);
	pt+=tlen;

	GROUPMAP::iterator it;
	while(m_groups.findnext(it))
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
BOOL	GroupPool::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;
		
	S_UINT len=0,tlen=sizeof(S_UINT);

	try{

		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;

		memmove(&m_hashtablesize,pt,tlen);
		pt+=tlen;

		m_groups.resetsize(m_hashtablesize);


		S_UINT count=0;
		memmove(&count,pt,tlen);
		pt+=tlen;

		Group *pm=NULL;
		word id;
		for(int i=0;i<count;i++)
		{
			memmove(&len,pt,tlen);
			pt+=tlen;
			pm=new Group();
			if(!pm)
				return false;

			if(!pm->CreateObjectByRawData(pt,len))
				return false;
			pt+=len;
			id=pm->GetID();
			m_groups.insert(id,pm);
			NewVersion(id);

		}
	}catch(...)
	{
		return false;
	}

	return true;
}

bool GroupPool::push(Group *pm)
{
	ost::MutexLock lock(m_UpdateLock);

	if(!pm)
		return false;
	word id=pm->GetID();

	Group **p=m_groups.find(id);
	if(p!=NULL)
		delete (*p);
	m_groups[id]=pm;
	NewVersion(id);
	cout<<"Latest group "<<id.getword()<<" version is: "<<GetVersion(id)<<endl;

	m_changed=true;
	return true;

}
bool GroupPool::PushData(const char *buf,S_UINT len)
{
	ost::MutexLock lock(m_UpdateLock);
	Group *pm=new Group();
	if(pm==NULL)
		return false;
	if(!pm->CreateObjectByRawData(buf,len))
		return false;
	return push(pm);

}

bool GroupPool::GetGroupData(word id,char *buf,S_UINT &len)
{
	Group **pm=m_groups.find(id);
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

bool GroupPool::DeleteGroup(word id)
{
	ost::MutexLock lock(m_UpdateLock);
	Group **pm=m_groups.find(id);
	if(pm==NULL)
		return false;

	delete (*pm);

	if(m_groups.erase(id))
	{
		m_mversion.erase(id.getword());
		m_changed=true;
		return true;
	}

   return false;


}


