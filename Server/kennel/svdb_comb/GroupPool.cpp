#include "grouppool.h"

GroupPool::GroupPool(void)
{
	m_hashtablesize=hashtablesize;
	m_groups.resetsize(m_hashtablesize);
	m_hide_groups.resetsize(m_hashtablesize);
	m_changed=false;
	needToDel=true;

}
GroupPool::GroupPool(word filepath):PoolBase2(filepath)
{
	m_hashtablesize=hashtablesize;
	m_groups.resetsize(m_hashtablesize);
	m_hide_groups.resetsize(m_hashtablesize);
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

	GROUPMAP::iterator hit;
	while(m_hide_groups.findnext(hit))
	{
		Group *pg=(*hit).getvalue();
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


bool	GroupPool::GetBackupData(std::list<SingelRecord> & listrcd)
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
	rcd.monitorid= "group_"+IdcUser::GetLocalSEIdStr();
	rcd.data= tdata;
	rcd.datalen= datalen;

	GroupPool mp;
	if(!mp.CreateObjectByRawData(tdata, datalen))
		return false;

	listrcd.push_back(rcd);
	return true;
}


S_UINT	GroupPool::GetRawDataSize(bool onlyLocked)
{
	S_UINT len=0,tlen=sizeof(S_UINT);

	len+=tlen;        //m_tablesize size;
	len+=tlen;		  //total monitor count;

	GROUPMAP::iterator it;
	while(m_groups.findnext(it))
	{
		if(onlyLocked && !IdcUser::WillTeleBackup((*it).getvalue()->GetID().getword()))
			continue;
		len+=tlen;			//len size;
		len+=(*it).getvalue()->GetRawDataSize(onlyLocked);

	}
	if(onlyLocked)
		return len;

	GROUPMAP::iterator hit;
	while(m_hide_groups.findnext(hit))
	{
		len+=tlen;			//len size;
		len+=(*hit).getvalue()->GetRawDataSize();
	}
	return len;

}
char*	GroupPool::GetRawData(char *lpbuf,S_UINT bufsize, bool onlyLocked)
{
	if(lpbuf==NULL)
		return NULL;

	if(bufsize<GetRawDataSize(onlyLocked))
		return NULL;

	S_UINT len=0,tlen=sizeof(S_UINT);
	char *pt=lpbuf;

	memmove(pt,&m_hashtablesize,tlen);
	pt+=tlen;

	len=m_groups.size();
	if(onlyLocked)
	{
		S_UINT tempcount=0;
		GROUPMAP::iterator tempit;
		while(m_groups.findnext(tempit))
		{
			if(IdcUser::WillTeleBackup((*tempit).getvalue()->GetID().getword()))
				++tempcount;
		}
		len= tempcount;
	}

	if(!onlyLocked)
		len+= m_hide_groups.size();
	memmove(pt,&len,tlen);
	pt+=tlen;

	GROUPMAP::iterator it;
	while(m_groups.findnext(it))
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

	GROUPMAP::iterator hit;
	while(m_hide_groups.findnext(hit))
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
		m_hide_groups.resetsize(m_hashtablesize);

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

			NewVersion(id);

			if(IdcUser::SELocked && !IdcUser::IsAnLocalSEId(id.getword()))
			{
				m_hide_groups.insert(id,pm);
				continue;
			}

			m_groups.insert(id,pm);


		}
	}catch(...)
	{
		return false;
	}

	return true;
}


bool	GroupPool::UpdateConfig(string sestr, const char *data,S_UINT datalen)
{
	try{
		if(!IdcUser::AcceptConfigIncoming || sestr.empty())
			return false;

		ost::MutexLock lock(m_UpdateLock);
		sestr= ","+sestr+",";
		GroupPool mp;
		if(!mp.CreateObjectByRawData(data, datalen))
			return false;

		GROUPMAP mDel;
		mDel.resetsize(m_hashtablesize);
		GROUPMAP::iterator git;
		while(m_groups.findnext(git))
		{
			string id= (*git).getkey().getword();
			string seid= GetTopID(id).getword();
			if(sestr.find(","+seid+",")==std::string::npos)
				continue;
			if(!IdcUser::IsAnLocalSEId(id))
				continue;

			Group *pIncome= mp.GetGroup(id.c_str());
			if(pIncome==NULL)
				mDel.insert(id,NULL);
		}

		GROUPMAP & ingroups= mp.GetMemberData();
		GROUPMAP::iterator git3;
		while(ingroups.findnext(git3))
		{
			string id= (*git3).getkey().getword();
			if(!IdcUser::IsAnLocalSEId(id))
				continue;

			Group **p= m_groups.find(id);
			if(p!=NULL)
				delete (*p);
			m_groups[id]= (*git3).getvalue();
			NewVersion(id);

			(*git3).setvalue(NULL);
		}

		GROUPMAP::iterator git2;
		while(mDel.findnext(git2))
		{
			string id= (*git2).getkey().getword();
			Group **pm=m_groups.find(id);
			if(pm!=NULL)
				delete (*pm);

			if(m_groups.erase(id))
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


bool GroupPool::push(Group *pm)
{
	ost::MutexLock lock(m_UpdateLock);

	if(!pm)
		return false;
	word id=pm->GetID();

	if(IdcUser::SELocked && !IdcUser::IsAnLocalSEId(id.getword()))
		return false;

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
	if(pm!=NULL)
		delete (*pm);

	if(m_groups.erase(id))
	{
		m_mversion.erase(id.getword());
		m_changed=true;
		return true;
	}

   return false;


}

bool GroupPool::UpdateNnmEntityParentGid()
{
	ost::MutexLock lock(m_NnmEntityParentGid_Lock);

	try{
		StringMap smap(577);
		GetInfo(IdcUser::nnmEntityParentKey,"default",smap);

		IdcUser::nnmEntityParentId.clear();
		bool OkOld= IdcUser::nnmEntityParentOk;
		IdcUser::nnmEntityParentOk= false;
		StringMap::iterator sit;
		while(smap.findnext(sit))
		{
			for(std::map<string,string>::iterator mit= IdcUser::nnmEntityParentValue.begin(); mit!=IdcUser::nnmEntityParentValue.end(); ++mit)
			{
				if( (mit->second).compare((*sit).getvalue().getword())==0 )
				{
					AddToErrorLog(mit->second+"="+(*sit).getkey().getword());
					pair< std::map<string,string>::iterator, bool > pr= IdcUser::nnmEntityParentId.insert(std::make_pair(mit->first,(*sit).getkey().getword()));
					if( !pr.second )
					{
						cout<<mit->second.c_str()<<"="<<(*sit).getkey().getword();
						cout<<"   (系统发现多个\""<<mit->second.c_str()<<"\"，请检查，该设备组只能有一个!)"<<endl;
						AddToErrorLog("   (系统发现多个\""+mit->second+"\"，请检查，该设备组只能有一个!)");
					}
				}
			}
		}

		if(IdcUser::nnmEntityParentId.size()== IdcUser::nnmEntityParentValue.size())
		{
			IdcUser::nnmEntityParentOk= true;
			if( OkOld== false )
			{
				for(std::map<string,string>::iterator mit= IdcUser::nnmEntityParentId.begin(); mit!=IdcUser::nnmEntityParentId.end(); ++mit)
				{
					std::map<string,string>::iterator mit2= IdcUser::nnmEntityParentValue.find(mit->first);
					if(mit2!=IdcUser::nnmEntityParentValue.end())
						cout<<mit2->second.c_str()<<"="<<mit->second.c_str()<<endl;
				}
			}
		}
	}
	catch(...)
	{
		cout<<" Exception in GroupPool::UpdateNnmEntityParentGid() "<<endl;
		return false;
	}
	return IdcUser::nnmEntityParentOk;
}

string GroupPool::GetNnmEntityParentGid(string type)
{
	ost::MutexLock lock(m_NnmEntityParentGid_Lock);
	std::map<string,string>::iterator mit= IdcUser::nnmEntityParentId.find(type);
	if(mit==IdcUser::nnmEntityParentId.end())
		return "";
	else
		return mit->second;
}

bool GroupPool::CheckIdIsNnmEntityParent(string id)
{
	ost::MutexLock lock(m_NnmEntityParentGid_Lock);
	for(std::map<string,string>::iterator mit= IdcUser::nnmEntityParentId.begin(); mit!=IdcUser::nnmEntityParentId.end(); ++mit)
	{
		if(id.compare(mit->second)==0)
			return true;
	}
	return false;
}



