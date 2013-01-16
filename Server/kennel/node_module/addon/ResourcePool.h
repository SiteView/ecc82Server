#ifndef SVDB_RESOURCEPOOL_
#define SVDB_RESOURCEPOOL_
#include "PoolBase.h"
#include "SerialBase.h"
#include "svdbtype.h"
#include "Resource.h"
#include <cc++/file.h>
#include <map>

typedef svutil::hashtable<word,Resource *> RESOURCEMAP;

class ResourcePool :
	public PoolBase,public SerialBase
{
public:
	ResourcePool(void);
	~ResourcePool(void);

	enum { hashtablesize=127 };

	ResourcePool(word filepath):PoolBase(filepath)
	{
		m_hashtablesize=hashtablesize;
		m_resources.resetsize(m_hashtablesize);
		m_changed=false;

	}




	bool Load(void);
	bool Submit();
	bool SubmitWithIds(const std::map<string,string> & allids );

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	bool push(Resource *pm);
	bool PushData(const char *buf,S_UINT len);

	Resource *GetResource(word language)
	{
		Resource **pm=m_resources.find(language);
		if(pm==NULL)
			return NULL;
		m_changed=true;
		return *pm;

	}

	bool GetResourceData(word language,char *buf,S_UINT &len)
	{
		Resource **pm=m_resources.find(language);
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

	bool GetResourceDataByKeys(string keys,word language,char *buf,S_UINT &len)
	{
		Resource **pm=m_resources.find(language);
		if(pm==NULL)
			return false;
		if(keys.empty())
			return false;

		Resource tempResource( (*pm)->GetResourceDict(),keys );
		S_UINT size=tempResource.GetRawDataSize();
		if(buf==NULL)
		{
			len=size;
			return true;
		}

		if(len<size)
			return false;
		return (tempResource.GetRawData(buf,size)!=NULL);

	}

	bool GetInfo(word infoname,StringMap &map)
	{
		RESOURCEMAP::iterator it;
		while(m_resources.findnext(it))
		{
			map[(*it).getkey().getword()]=(*it).getkey().getword();
		}
		
		return true;
	}


	bool DeleteResource(word language);

	void DisplayAllData(void)
	{
		puts("*************************************Begin**********************************");
		RESOURCEMAP::iterator it;

		while(m_resources.findnext(it))
			(*it).getvalue()->DisplayAllData();

		puts("**************************************End************************************");
	}

	std::map<string,string> newids;
	void InsertIntoNewIds(string key, string value)
	{
		ost::MutexLock lock(m_UpdateLockStdMap);
		newids.insert(make_pair(key.c_str(), value.c_str()));
	}
	void GetThenClearAllIds(std::map<string,string> & retids)
	{
		ost::MutexLock lock(m_UpdateLockStdMap);
		retids.clear();
		newids.swap(retids);
	}

public:
	RESOURCEMAP m_resources;
	S_UINT	m_hashtablesize;

protected:
	Mutex	m_UpdateLockStdMap;

};

#endif
