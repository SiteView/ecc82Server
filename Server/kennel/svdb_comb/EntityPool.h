#ifndef	SVDB_ENTITYPOOL_
#define SVDB_ENTITYPOOL_

#include "PoolBase.h"
#include "SerialBase.h"
#include "svdbtype.h"
#include "Entity.h"
#include <cc++/file.h>

typedef svutil::hashtable<word,Entity *> ENTITYMAP;

class EntityPool :
	public PoolBase2,public SerialBase2
{
public:
	EntityPool(void);
	EntityPool(word filepath);
	EntityPool(ENTITYMAP & inputgmap)
	{
		m_hashtablesize=hashtablesize;
		m_entitys.resetsize(m_hashtablesize);
		m_changed=false;

		needToDel=false;
		ENTITYMAP::iterator git;
		while(inputgmap.findnext(git))
			m_entitys.insert((*git).getkey().getword(),(*git).getvalue());
	}
	~EntityPool(void);

	enum{ hashtablesize=1277 };

	bool Load(void);
	bool Submit(std::string modifyid="");

	bool LoadFile(string fileName);
	bool SubmitToFile(string fileName, EntityPool * dataPool );

	bool	GetBackupData(std::list<SingelRecord> & listrcd);
	S_UINT	GetRawDataSize(bool onlyLocked= false);
	char*	GetRawData(char *lpbuf,S_UINT bufsize, bool onlyLocked= false);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);
	bool	UpdateConfig(string sestr, const char *data, S_UINT datalen);


	bool push(Entity *pm);
	bool PushData(const char *buf,S_UINT len);

	Entity *GetEntity(word id)
	{
		Entity **pm=m_entitys.find(id);
		if(pm==NULL)
			return NULL;
		//m_changed=true;
		return *pm;
	}
	int QueryMassEntity(string pid, const char *data,S_UINT datalen, std::list<SingelRecord> & listrcd );

	bool GetEntityData(word id,char *buf,S_UINT &len);
	bool SaveDataFromNnmEntity(string id, string exid, StringMap & smap);

	word GetNextSubID(word entityid)
	{
		MutexLock lock(m_UpdateLock);
		Entity *pe=GetEntity(entityid);
		if(pe)
		{
			m_changed=true;
			return pe->GetNextMonitorID();
		}
		return "";
	}

	bool PutSubMonitor(word entityid,word monitorid)
	{
		MutexLock lock(m_UpdateLock);
		Entity *pe=GetEntity(entityid);
		if(pe)
		{
			NewVersion(entityid);
			pe->GetMonitors().push_back(monitorid);
			m_changed=true;
			return true;
		}
		return false;

	}

	bool Find(word id)
	{
		Entity **pe=m_entitys.find(id);
		return pe!=NULL;
	}

	bool DeleteEntity(word id);

	void DisplayAllData(void)
	{
		puts("------------------------------------------Begin----------------------------------------");
		printf("Hash table size :%d\n",m_hashtablesize);
		puts("---------------------------------------------------------------------------------------");
		ENTITYMAP::iterator it;
		while(m_entitys.findnext(it))
		{
			(*it).getvalue()->DisplayAllData();
		}
	}

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
		ENTITYMAP::iterator it;
		while(m_entitys.findnext(it))
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

	bool DeleteSubMonitorID(word id)
	{
		MutexLock lock(m_UpdateLock);
		word entityid=::GetParentID(id);
		Entity **pe=m_entitys.find(entityid);
		if(pe==NULL)
			return false;

		NewVersion(entityid);
		if((*pe)->DeleteSubMonitorID(id))
			m_changed=true;
		else
			return false;
		return true;
	}

	ENTITYMAP & GetMemberData()
	{
		return m_entitys;
	}

protected:
	ENTITYMAP m_entitys;
	ENTITYMAP m_hide_entitys;

	S_UINT	m_hashtablesize;
	bool needToDel;

};

#endif
