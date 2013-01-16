#ifndef SVDB_EDGE_H
#define SVDB_EDGE_H

#include "SerialBase.h"
#include "svdbtype.h"
#include "StringMap.h"
#include "util.h"
#include <list>

//comment by FanLin:网络边界，用于生成拓扑图
class Edge :
	public SerialBase
{
public:
	Edge(void);
	~Edge(void);

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	word GetLeftPortID(void) { return m_LeftPortID; }
	void PutLeftPortID(word LeftPortID) { m_LeftPortID=LeftPortID; }
	std::string GetLeftPortIDStr(void) 
	{
		std::string ret(m_LeftPortID.getword());
		return ret;
	}

	word GetRightPortID(void) { return m_RightPortID; }
	void PutRightPortID(word RightPortID) { m_RightPortID=RightPortID;}
	std::string GetRightPortIDStr(void) 
	{
		std::string ret(m_RightPortID.getword());
		return ret;
	}

	StringMap &GetProperty(void) { return m_Property; }

	void PutDependMonitorID(word DependMonitorId) { m_DependMonitorID=DependMonitorId;}
	word GetDependMonitorID(void) { return m_DependMonitorID; }
	std::string GetDependMonitorIDStr(void) 
	{
		std::string ret(m_DependMonitorID.getword());
		return ret;
	}

	void DisplayAllData()
	{
		puts("**********************************Edge:begin*****************************");
		printf("LeftPortID:%s\n",(char *)m_LeftPortID);
		printf("RightPortID:%s\n",(char *)m_RightPortID);
		printf("DependMonitorID:%s\n",(char *)m_DependMonitorID);
		puts("--------------------------------Property-----------------------------------");
		StringMap::iterator sit;
		while(m_Property.findnext(sit))
			printf("%s=%s\n",(*sit).getkey().getword(),(*sit).getvalue().getword());
		puts("*********************************Edge:end*********************************");
	}

protected:
	word	m_LeftPortID;
	word	m_RightPortID;
	word	m_DependMonitorID;
	StringMap	m_Property;
};

typedef std::list<Edge *>	EDGELIST;

#endif


