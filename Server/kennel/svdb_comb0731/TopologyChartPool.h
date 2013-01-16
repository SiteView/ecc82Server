#ifndef SVDB_TOPOLOGYCHARTPOOL_
#define SVDB_TOPOLOGYCHARTPOOL_

#include "PoolBase.h"
#include "SerialBase.h"
#include "svdbtype.h"

#include "TopologyChart.h"

class TopologyChartPool :
	public PoolBase,public SerialBase
{
public:
	enum { hashtablesize=127 };

	TopologyChartPool(void);
	TopologyChartPool(word filepath);
	~TopologyChartPool(void);

	bool Load(void);
	bool Submit();

	bool push(TopologyChart *pm);
	bool PushData(const char *buf,S_UINT len);

	TopologyChart *GetTopologyChart(word id)
	{
		TopologyChart **pm=m_data.find(id);
		if(pm==NULL)
			return NULL;
		m_changed=true;
		return *pm;
	}

	bool GetTopologyChartData(word id,char *buf,S_UINT &len);

	bool Find(word id)
	{
		TopologyChart **pe=m_data.find(id);
		return pe!=NULL;
	}


	bool DeleteTopologyChart(word id);

	bool GetInfo(word infoname,StringMap &map)
	{
		TOPOLOGYCHARTMAP::iterator it;
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
		TOPOLOGYCHARTMAP::iterator it;
		bool flag=false;
		while(m_data.findnext(it))
		{
			if((*it).getkey().empty())
				continue;

			flag=false;
			if(key.empty())
			{
				flag=true;
			}else if(key.compare("CreateTime")==0)
			{
				if(value.compare((*it).getvalue()->GetCreateTime().Format().c_str())==0)
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
		TOPOLOGYCHARTMAP::iterator it;
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


	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	void DisplayAllData()
	{
		puts("**********************************TopologyChartPool:begin*****************************");
		printf("hashtable size:%d\n",m_hashtablesize);
		puts("----------------------------------------data-------------------------------------");
		TOPOLOGYCHARTMAP::iterator it;
		while(m_data.findnext(it))
			(*it).getvalue()->DisplayAllData();
		puts("**********************************TopologyChartPool:end*******************************");
		

	}


protected:
	TOPOLOGYCHARTMAP m_data;
	S_UINT			m_hashtablesize;
};

#endif
