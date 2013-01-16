
#ifndef SVDB_TOPOLOGYCHART_
#define SVDB_TOPOLOGYCHART_

#include "SerialBase.h"
#include "svdbtype.h"
#include "StringMap.h"
#include "util.h"
#include "Edge.h"
#include "libutil/time.h"


class TopologyChart :
	public SerialBase
{
public:
	TopologyChart(void);
	~TopologyChart(void);

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	word GetID(void) { return m_ID; }
	void PutID(word id) { m_ID=id; }
	std::string GetIDStr(void) 
	{
		std::string ret(m_ID.getword());
		return ret;
	}

	word GetDescription(void) { return m_Description; }
	void PutDescription( word Des ) { m_Description = Des; }
	std::string GetDescriptionStr(void) 
	{
		std::string ret(m_Description.getword());
		return ret;
	}

	svutil::TTime GetCreateTime(void) { return m_CreateTime; }
	void PutCreateTime(svutil::TTime tm) { m_CreateTime=tm; }

	StringMap &GetProperty(void) { return m_Property; }

	EDGELIST & GetEdges(void) { return m_Edges; }
	void DisplayAllData()
	{
		puts("**********************************TopologyChart:begin*****************************");
		printf("ID:%s\n",(char *)m_ID);
		printf("Create time:%s\n",m_CreateTime.Format().c_str());
		printf("Description:%s\n",(char *)m_Description);
		puts("------------------------------------Propert---------------------------------------");
		StringMap::iterator sit;
		while(m_Property.findnext(sit))
			printf("%s=%s\n",(*sit).getkey().getword(),(*sit).getvalue().getword());
		puts("------------------------------------Edges-----------------------------------------");
		EDGELIST::iterator it;
		for(it=m_Edges.begin();it!=m_Edges.end();it++)
			(*it)->DisplayAllData();
		puts("**********************************TopologyChart:end*******************************");

	}

    

protected:
	word	m_ID;
	svutil::TTime	m_CreateTime;
	word	m_Description;
	StringMap	m_Property;

	EDGELIST	m_Edges;

};

typedef	svutil::hashtable< svutil::word,TopologyChart * >	TOPOLOGYCHARTMAP;

#endif
