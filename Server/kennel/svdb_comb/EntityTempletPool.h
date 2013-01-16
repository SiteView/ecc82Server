#ifndef SVDB_ENTITYTEMPLETPOOL_
#define SVDB_ENTITYTEMPLETPOOL_

#include "PoolBase.h"
#include "SerialBase.h"
#include "svdbtype.h"
#include "EntityTemplet.h"
#include <cc++/file.h>

typedef svutil::hashtable<word,EntityTemplet *> ENTITYTEMPLETMAP;

class EntityTempletPool :
	public PoolBase,public SerialBase
{
public:
	EntityTempletPool(void);
	EntityTempletPool(word filepath,bool orig=false):PoolBase(filepath)
	{
		m_data.resetsize(hashtablelen);
		isorig= orig;
	}

	~EntityTempletPool(void);

	enum { hashtablelen=127 };

	bool Load(void);
	bool Submit();

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	bool Push(EntityTemplet *pm);
	bool Push(const char *buf,S_UINT len, Resource *m_pLanguage=NULL);

	bool Find(word id)
	{
		EntityTemplet **peg=m_data.find(id);
		return peg!=NULL;
	}

	EntityTemplet *GetEntityTemplet(word id)
	{
		EntityTemplet **peg=m_data.find(id);
		if(peg!=NULL)
		{
			m_changed=true;
			return (*peg);
		}
		return NULL;
	}

	bool GetEntityTemplet(word id,char *buf,S_UINT &len)
	{
		EntityTemplet **pm=m_data.find(id);
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

	bool DeleteEntityTemplet(word id)
	{
		ost::MutexLock lock(m_UpdateLock);
		EntityTemplet **pm=m_data.find(id);
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
		ENTITYTEMPLETMAP::iterator it;
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
		ENTITYTEMPLETMAP::iterator it;
		while(m_data.findnext(it))
			(*it).getvalue()->DisplayAllData();
		puts("**********************************End***************************************");

	}

	bool GetInfoByProperty(string key,string value,std::list<string> &idlist)
	{
		ENTITYTEMPLETMAP::iterator it;
		while(m_data.findnext(it))
		{
			StringMap &smap=(*it).getvalue()->GetProperty();
			svutil::word * myWord= smap.find( key.c_str() );
			if( myWord !=NULL && value.compare(myWord->getword())==0)
				idlist.push_back( (*it).getkey().getword() );
		}
		return true;
	}

	bool ResolveIds(Resource *m_pLanguage);//非线程安全函数
	bool SaveNewIds(string id,Resource *m_pLanguage);

protected:
    ENTITYTEMPLETMAP	m_data;
 
	bool isorig;   
};

#endif
