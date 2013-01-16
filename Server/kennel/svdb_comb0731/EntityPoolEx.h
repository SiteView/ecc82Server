#ifndef SVDB_ENTITYPOOLEX_
#define SVDB_ENTITYPOOLEX_

#include "PoolBase.h"
#include "SerialBase.h"
#include "svdbtype.h"
#include "EntityEx.h"
#include <cc++/file.h>



class EntityPoolEx :
	public PoolBase,public SerialBase
{
public:
	EntityPoolEx(void);
	EntityPoolEx(word filepath);

	~EntityPoolEx(void);

	enum{ hashtablesize=1277 };

	bool Load(void);
	bool Submit();

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	bool push(EntityEx *pm);
	bool PushData(const char *buf,S_UINT len);

	EntityEx *GetEntityEx(word id)
	{
		EntityEx **pm=m_entityexs.find(id);
		if(pm==NULL)
			return NULL;
		m_changed=true;
		return *pm;
	}

	bool GetEntityExData(word id,char *buf,S_UINT &len);

	bool Find(word id)
	{
		EntityEx **pe=m_entityexs.find(id);
		return pe!=NULL;
	}

	bool DeleteEntityEx(word id);

	bool GetInfo(word infoname,StringMap &map)
	{
		ENTITYEXMAP::iterator it;
		while(m_entityexs.findnext(it))
		{
			if((*it).getkey().empty())
				continue;

			StringMap &tm=(*it).getvalue()->GetProperty();
			if(tm.find(infoname)!=NULL)
				map[(*it).getkey().getword()]=tm[infoname];
			else
				map[(*it).getkey().getword()]="";
		}
		
		return true;
	}

	bool GetInfoByKey(string key,string value,std::list<string> &idlist)
	{
		ENTITYEXMAP::iterator it;
		bool flag=false;
		while(m_entityexs.findnext(it))
		{
			if((*it).getkey().empty())
				continue;

			flag=false;
			if(key.empty())
			{
				flag=true;
			}else if(key.compare("Type")==0)
			{
				if(value.compare((*it).getvalue()->GetType().getword())==0)
					flag=true;
			}else if(key.compare("Description")==0)
			{
				if(value.compare((*it).getvalue()->GetDescription().getword())==0)
					flag=true;
			}else if(key.compare("EccEntityID")==0)
			{
				if(value.compare((*it).getvalue()->GetEccEntityID().getword())==0)
					flag=true;
			}

			if(flag)
		    	idlist.push_back((*it).getkey().getword());
		}


		return true;
	}

	bool GetInfoByProperty(string key,string value,std::list<string> &idlist)
	{
		ENTITYEXMAP::iterator it;
		bool flag=false;
		while(m_entityexs.findnext(it))
		{
			if((*it).getkey().empty())
				continue;

			flag=false;
			StringMap &smap=(*it).getvalue()->GetProperty();
			if(smap.find(key)!=NULL)
				if(value.compare(smap[key].getword())==0)
					flag=true;

			if(flag)
		    	idlist.push_back((*it).getkey().getword());

		}

		return true;
	}

	void DisplayAllData()
	{
		puts("**********************************EntityPoolEx:begin*****************************");
		printf("hashtable size:%d\n",m_hashtablesize);
		puts("----------------------------------------data-------------------------------------");
		ENTITYEXMAP::iterator it;
		while(m_entityexs.findnext(it))
			(*it).getvalue()->DisplayAllData();
		puts("**********************************EntityPoolEx:end*******************************");
	}

protected:

	ENTITYEXMAP m_entityexs;
	S_UINT	m_hashtablesize;
};

#endif
