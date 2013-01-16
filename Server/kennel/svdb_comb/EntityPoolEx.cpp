#include "EntityPoolEx.h"
#include "WorkThread.h"
#include "Des.h"

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
	if(!m_changed)
		return true;

	ost::MutexLock lock(m_UpdateLock);
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

			string ecceid= ptc->GetEccEntityID();
			std::map<string,string>::iterator it= EidToExid.find(ecceid);
			if(it!=EidToExid.end())
				it->second= id.getword();
			else
				EidToExid.insert(std::make_pair(ecceid,id.getword()));

		}

	}catch(...)
	{
		printf("\nException to EntityPoolEx::CreateObjectByRawData \n");
		return false;
	}

	return true;

}

bool EntityPoolEx::CreateNnmEntityParentGroup(SvdbMain *pMain)
{		
	try{
		std::map<string,string> HadId, tempHadId;

		StringMap smap(577);
		pMain->m_pGroup->GetInfo(IdcUser::nnmEntityParentKey,"default",smap);

		StringMap::iterator sit;
		while(smap.findnext(sit))
		{
			for(std::map<string,string>::iterator mit= IdcUser::nnmEntityParentValue.begin(); mit!=IdcUser::nnmEntityParentValue.end(); ++mit)
			{
				if( (mit->second).compare((*sit).getvalue().getword())==0 )
					HadId.insert(std::make_pair(mit->first,mit->second));
			}
		}

		for(int time=0; time<=1; ++time)
		{
			int count(0);
			for(std::map<string,string>::iterator mit= IdcUser::nnmEntityParentValue.begin(); mit!=IdcUser::nnmEntityParentValue.end(); ++mit)
			{
				++count;
				if(time==0 && count==1)
					continue;
				if(time==1 && count==2)
					continue;

				std::map<string,string>::iterator mit2= HadId.find(mit->first);
				if(mit2==HadId.end())
				{
					string nnmEntityParentSvname= mit->second;

					Group *pg=new Group();
					if(pg==NULL)
						return false;
					if(pMain==NULL)
					{
						cout<<" pMain==NULL in Create "<<nnmEntityParentSvname.c_str()<<endl;
						return false;
					}
					pg->GetProperty()[IdcUser::nnmEntityParentKey.c_str()]= nnmEntityParentSvname;
					pg->GetProperty()["sv_name"]= nnmEntityParentSvname;
					pg->GetProperty()["sv_description"]= nnmEntityParentSvname;

					WorkThread wt;
					wt.SetSvdbMain(pMain);
					string newid;
					if(!wt.AddNewGroup(pg, newid, IdcUser::GetFirstSEId()))
					{
						delete pg;
						cout<<" Failed to AddNewGroup when Create "<<nnmEntityParentSvname.c_str()<<endl;
						return false;
					}
					cout<<"  Auto-created: "<<nnmEntityParentSvname.c_str()<<"="<<newid.c_str()<<endl;
				}
				if(time==0 && count==2)
					break;
			}
		}
		pMain->m_pGroup->UpdateNnmEntityParentGid();
	}
	catch(...)
	{
		cout<<" Exception in EntityPoolEx::CreateNnmEntityParentGroup() "<<endl;
		return false;
	}
	return true;
}

bool EntityPoolEx::push(EntityEx *pm,string & exid,SvdbMain *pMain)
{
	ost::MutexLock lock(m_UpdateLock);

	if(!pm)
		return false;
	word id=pm->GetIdentifier();
	if(id.empty())
		return false;
	exid= id.getword();

	EntityEx **p=m_entityexs.find(id);
	if(p!=NULL && *p!=NULL)
	{
		string oldecceid= (*p)->GetEccEntityID().getword();
		std::map<string,string>::iterator oldit= EidToExid.find(oldecceid);
		if(oldit!=EidToExid.end())
			EidToExid.erase(oldit);
	}
	if(p!=NULL)
		UndoInvalidUpdateOfEccEntityId(*p,pm,pMain);

	if(p!=NULL)
		delete (*p);
	m_entityexs[id]=pm;
	m_changed=true;

	string ecceid= pm->GetEccEntityID();
	std::map<string,string>::iterator it= EidToExid.find(ecceid);
	if(it!=EidToExid.end())
		it->second= exid;
	else
		EidToExid.insert(std::make_pair(ecceid,exid));

	if(!IdcUser::nnmEntityParentOk)
		CreateNnmEntityParentGroup(pMain);

	return true;

}
bool EntityPoolEx::PushData(const char *buf,S_UINT len,string & exid,SvdbMain *pMain)
{
	ost::MutexLock lock(m_UpdateLock);
	EntityEx *pm=new EntityEx();
	if(pm==NULL)
		return false;
	if(!pm->CreateObjectByRawData(buf,len))
		return false;
	return push(pm,exid,pMain);

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
	if(pm==NULL||(*pm)==NULL)
		return false;
	
	string ecceid= (*pm)->GetEccEntityID();
	delete (*pm);

   if(m_entityexs.erase(id))
   {
	   m_changed=true;

	   std::map<string,string>::iterator it= EidToExid.find(ecceid);
	   if(it!=EidToExid.end())
		   EidToExid.erase(it);

	   return true;
   }
   return false;

}

bool EntityPoolEx::SaveDataFromEccEntity(string id, StringMap & smap)
{
	ost::MutexLock lock(m_UpdateLock);
	EntityEx ** ex= m_entityexs.find(id.c_str());
	if(ex==NULL||*ex==NULL)
		return false;
	m_changed=true;

	string Community;
	bool hasIt(false);
	StringMap::iterator sit;
	while(smap.findnext(sit))
	{
		string key= (*sit).getkey().getword();
		if(key.compare("_Community")!=0)
			(*ex)->GetProperty()[(*sit).getkey()]=(*sit).getvalue();
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
			if( mydes.Decrypt(Community.c_str(), out) )
				Community= out;	
		}catch(...)
		{
		}
	}
	if(hasIt)
		(*ex)->GetProperty()["_Community"]= Community.c_str();

	cout<<"\""<<id.c_str()<<"\" nnm entity updated by ecc entity"<<endl;
	return true;
}

bool EntityPoolEx::SaveEccEntityId(string exid, string ecceid)
{
	ost::MutexLock lock(m_UpdateLock);
	EntityEx ** ex= m_entityexs.find(exid.c_str());
	if(ex==NULL||*ex==NULL)
		return false;
	m_changed=true;

	string oldecceid= (*ex)->GetEccEntityID().getword();
	std::map<string,string>::iterator oldit= EidToExid.find(oldecceid);
	if(oldit!=EidToExid.end())
		EidToExid.erase(oldit);

	(*ex)->PutEccEntityID(ecceid.c_str());

	std::map<string,string>::iterator it= EidToExid.find(ecceid);
	if(it!=EidToExid.end())
		it->second= exid;
	else
		EidToExid.insert(std::make_pair(ecceid,exid));

	return true;
}

bool EntityPoolEx::UndoInvalidUpdateOfEccEntityId(EntityEx * pOld, EntityEx * pNew, SvdbMain *pMain)
{
	if(pOld==NULL || pNew==NULL || pMain==NULL)
		return false;

	string oldeid= pOld->GetEccEntityID();
	string neweid= pNew->GetEccEntityID();

	Entity * olden= pMain->m_pEntity->GetEntity(oldeid.c_str());
	Entity * newen= pMain->m_pEntity->GetEntity(neweid.c_str());

	if(olden!=NULL && newen==NULL)
	{
		m_changed=true;
		pNew->PutEccEntityID(oldeid.c_str());
	}
	return true;
}

string EntityPoolEx::GetNnmEidByEccEid(string ecceid)
{
	std::map<string,string>::iterator it= EidToExid.find(ecceid);
	if(it==EidToExid.end())
		return "";
	
	return it->second;
}

