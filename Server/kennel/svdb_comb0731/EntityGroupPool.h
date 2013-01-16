#ifndef	SVDB_ENTITYGROUPPOOL_
#define	SVDB_ENTITYGROUPPOOL_

#include "PoolBase.h"
#include "SerialBase.h"
#include "svdbtype.h"
#include "EntityGroup.h"
#include <cc++/file.h>

typedef svutil::hashtable<word,EntityGroup *> ENTITYGROUPMAP;

class EntityGroupPool : 
	public PoolBase,public SerialBase
{
public:
	EntityGroupPool(void);
	EntityGroupPool(word filepath,bool orig=false);
	~EntityGroupPool(void);

	bool Load(void);
	bool Submit();

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	bool Push(EntityGroup *pm);
	bool Push(const char *buf,S_UINT len, Resource *m_pLanguage=NULL);

	bool Find(word id)
	{
		EntityGroup **peg=m_data.find(id);
		return peg!=NULL;
	}

	EntityGroup *GetEntityGroup(word id)
	{
		EntityGroup **peg=m_data.find(id);
		if(peg!=NULL)
		{
			m_changed=true;
			return (*peg);
		}
		return NULL;
	}

	bool GetEntityGroup(word id,char *buf,S_UINT &len)
	{
		EntityGroup **pm=m_data.find(id);
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

	bool DeleteEntityGroup(word id)
	{
		ost::MutexLock lock(m_UpdateLock);
		EntityGroup **pm=m_data.find(id);
		if(pm==NULL)
			return false;

		delete (*pm);

		if(m_data.erase(id))
		{
			m_changed=true;
			return true;
		}

		return false;

	}

	bool GetInfo(word infoname,StringMap &map)
	{
		ENTITYGROUPMAP::iterator it;
		while(m_data.findnext(it))
		{
			StringMap &tm=(*it).getvalue()->GetProperty();
			if(tm.find(infoname)!=NULL)
				map[(*it).getkey().getword()]=tm[infoname];
			else
				map[(*it).getkey().getword()]="";


		}
		
		return true;
	}


	void DisplayAllData(void)
	{
		puts("**********************************Begin*************************************");
		ENTITYGROUPMAP::iterator it;
		while(m_data.findnext(it))
			(*it).getvalue()->DisplayAllData();
		puts("**********************************End***************************************");

	}

	bool ResolveIds(Resource *m_pLanguage);//非线程安全函数
	bool SaveNewIds(string id,Resource *m_pLanguage);

protected:
	ENTITYGROUPMAP	m_data;
 
	bool isorig;  
};

#endif
