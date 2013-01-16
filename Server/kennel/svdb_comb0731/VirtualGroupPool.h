#ifndef SVDB_VIRTUALGROUPPOOL_
#define SVDB_VIRTUALGROUPPOOL_

#include "PoolBase.h"
#include "SerialBase.h"
#include "svdbtype.h"
#include "VirtualGroup.h"
#include <cc++/file.h>

class VirtualGroupPool :
		public PoolBase,public SerialBase
{
public:
	VirtualGroupPool(void);
	VirtualGroupPool(word filepath);
	~VirtualGroupPool(void);

	enum { hashtablesize=127 };

	bool Load(void);
	bool Submit();

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	bool push(VirtualGroup *pm);
	bool PushData(const char *buf,S_UINT len);

	VirtualGroup *GetVirtualGroup(word id)
	{
		VirtualGroup **pm=m_data.find(id);
		if(pm==NULL)
			return NULL;
		m_changed=true;
		return *pm;
	}

	bool GetVirtualGroupData(word id,char *buf,S_UINT &len);

	bool Find(word id)
	{
		VirtualGroup **pe=m_data.find(id);
		return pe!=NULL;
	}

	bool DeleteVirtualGroup(word id);

	bool GetInfo(word infoname,StringMap &map)
	{
		VIRTUALGROUPMAP::iterator it;
		while(m_data.findnext(it))
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
		VIRTUALGROUPMAP::iterator it;
		bool flag=false;
		while(m_data.findnext(it))
		{
			if((*it).getkey().empty())
				continue;

			flag=false;
			if(key.empty())
			{
				flag=true;
			}else if(key.compare("Label")==0)
			{
				if(value.compare((*it).getvalue()->GetLabel().getword())==0)
					flag=true;
			}else if(key.compare("Description")==0)
			{
				if(value.compare((*it).getvalue()->GetDescription().getword())==0)
					flag=true;
			}

			if(flag)
		    	idlist.push_back((*it).getkey().getword());
		}


		return true;
	}

	bool GetInfoByProperty(string key,string value,std::list<string> &idlist)
	{
		VIRTUALGROUPMAP::iterator it;
		bool flag=false;
		while(m_data.findnext(it))
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
		puts("**********************************VirtualGroupPool:begin*****************************");
		printf("hashtable size:%d\n",m_hashtablesize);
		puts("----------------------------------------data-------------------------------------");
		VIRTUALGROUPMAP::iterator it;
		while(m_data.findnext(it))
			(*it).getvalue()->DisplayAllData();
		puts("**********************************VirtualGroupPool:end*******************************");
		

	}

protected:

	VIRTUALGROUPMAP m_data;
	S_UINT			m_hashtablesize;


};

#endif
