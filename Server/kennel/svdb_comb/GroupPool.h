#ifndef	SVDB_GROUPPOOL_
#define SVDB_GROUPPOOL_

#include "PoolBase.h"
#include "SerialBase.h"
#include "svdbtype.h"
#include "Group.h"
#include <cc++/file.h>

typedef svutil::hashtable<word,Group *> GROUPMAP;

class GroupPool :
	public PoolBase2,public SerialBase2
{
public:
	GroupPool(void);
	GroupPool(word filepath);
	GroupPool(GROUPMAP & inputgmap)
	{
		m_hashtablesize=hashtablesize;
		m_groups.resetsize(m_hashtablesize);
		m_changed=false;

		needToDel=false;
		GROUPMAP::iterator git;
		while(inputgmap.findnext(git))
			m_groups.insert((*git).getkey().getword(),(*git).getvalue());
	}
	~GroupPool(void);

	enum{ hashtablesize=1277 };

	bool Load(void);
	bool Submit(std::string modifyid="");

	bool LoadFile(string fileName);
	bool SubmitToFile(string fileName, GroupPool * dataPool );

	bool	GetBackupData(std::list<SingelRecord> & listrcd);
	S_UINT	GetRawDataSize(bool onlyLocked= false);
	char*	GetRawData(char *lpbuf,S_UINT bufsize, bool onlyLocked= false);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);
	bool	UpdateConfig(string sestr, const char *data, S_UINT datalen);


	bool push(Group *pm);
	bool PushData(const char *buf,S_UINT len);

	Group *GetGroup(word id)
	{
		Group **pm=m_groups.find(id);
		if(pm==NULL)
			return NULL;
		//m_changed=true;
		return *pm;
	}

	word GetNextSubID(word groupid)
	{
		MutexLock lock(m_UpdateLock);

		Group *pg=GetGroup(groupid);
		if(pg!=NULL)
		{
			m_changed=true;
			return pg->GetNextSubID();
		}
		else
			return "";

	}

	bool PutSubGroup(word groupid,word subid)
	{
		MutexLock lock(m_UpdateLock);

		Group *pg=GetGroup(groupid);
		if(pg!=NULL)
		{
			NewVersion(groupid);
			pg->GetSubGroups().push_back(subid);
			m_changed=true;
			return true;
		}
		
		return false;


	}

	bool PutSubEntity(word groupid,word entityid)
	{
		MutexLock lock(m_UpdateLock);

		Group *pg=GetGroup(groupid);
		if(pg!=NULL)
		{
			NewVersion(groupid);
			pg->GetEntitys().push_back(entityid);
		    m_changed=true;
			return true;
		}
		return false;

	}

	bool GetGroupData(word id,char *buf,S_UINT &len);

	bool Find(word id)
	{
		Group **pe=m_groups.find(id);
		return pe!=NULL;
	}


	bool DeleteGroup(word id);

	bool GetInfo(word infoname,string pid,StringMap &map,bool onlyson=false)
	{
		ost::MutexLock lock(m_UpdateLock);
		bool getver(false);
		string infomation(infoname.getword());
		if(infomation.compare("ObjectVersion")==0)
			getver=true;

		bool isdefault(false);
		if(pid.compare("default")==0)
			isdefault=true;
		else
			pid+=".";

		string mid;
		GROUPMAP::iterator it;
		while(m_groups.findnext(it))
		{
			if((*it).getkey().empty())
				continue;

			const char * key = (*it).getkey().getword();
			mid=key;
			if( !isdefault && mid.find(pid)!=0)
				continue;

			if(onlyson && mid.find(".",pid.size())!=std::string::npos )
				continue;

			if(getver)
			{
				char charver[256];
				sprintf(charver,"%d",GetVersion(key));
				map[key]=charver;
				continue;
			}

			StringMap &tm=(*it).getvalue()->GetProperty();
			if(tm.find(infoname)!=NULL)
				map[key]=tm[infoname];
			else
				map[key]="";


		}
		
		return true;
	}

	bool DeleteSubEntityID(word id)
	{
		MutexLock lock(m_UpdateLock);
		word groupid=::GetParentID(id);
		Group **pe=m_groups.find(groupid);
		if(pe==NULL)
			return false;

		NewVersion(groupid);
		if((*pe)->DeleteSubEntityID(id))
			m_changed=true;
		else
			return false;
		return true;
	}
	bool DeleteSubGroupID(word id)
	{
		MutexLock lock(m_UpdateLock);
		word groupid=::GetParentID(id);
		Group **pe=m_groups.find(groupid);
		if(pe==NULL)
			return false;

		NewVersion(groupid);
		if((*pe)->DeleteSubGroupID(id))
			m_changed=true;
		else
			return false;
		return true;
	}

	void DisplayAllData(void)
	{
		puts("------------------------------------------Begin----------------------------------------");
		printf("Hash table size :%d\n",m_hashtablesize);
		puts("---------------------------------------------------------------------------------------");
		GROUPMAP::iterator it;
		while(m_groups.findnext(it))
		{
			(*it).getvalue()->DisplayAllData();
		}
	}

	bool UpdateNnmEntityParentGid();
	string GetNnmEntityParentGid(string type);
	bool CheckIdIsNnmEntityParent(string id);

	GROUPMAP & GetMemberData()
	{
		return m_groups;
	}

protected:
	Mutex	m_NnmEntityParentGid_Lock;

	GROUPMAP m_groups;
	GROUPMAP m_hide_groups;

	S_UINT	m_hashtablesize;
	bool needToDel;

};

#endif
