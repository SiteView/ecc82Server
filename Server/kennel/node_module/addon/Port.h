#ifndef SVDB_PORT_H
#define SVDB_PORT_H

#include "SerialBase.h"
#include "svdbtype.h"
#include "StringMap.h"
#include "util.h"


class Port :
	public SerialBase
{
public:
	Port(void);
	~Port(void);

	word GetID() const
	{
		return m_ID;
	}
	std::string GetIDStr(void)  const 
	{
		std::string ret(m_ID.getword());
		return ret;
	}

	void PutID(word id)
	{
		m_ID=id;
	}


	word GetPeerPortID() const
	{
		return m_PeerPortID;
	}
	std::string GetPeerPortIDStr(void)  const 
	{
		std::string ret(m_PeerPortID.getword());
		return ret;
	}

	void PutPeerPortID(word PeerPortID)
	{
		m_PeerPortID=PeerPortID;
	}

	word GetMonitorID() const
	{
		return m_MonitorID;
	}
	std::string GetMonitorIDStr(void)  const 
	{
		std::string ret(m_MonitorID.getword());
		return ret;
	}

	void PutMonitorID(word MonitorID)
	{
		m_MonitorID=MonitorID;
	}

	StringMap &GetProperty(void)
	{
		return m_Property;
	}

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	void DisplayAllData()
	{
		puts("*************************************Port:begin**********************************");
		printf("ID:%s\n",(char *)m_ID);
		printf("PeerPortID:%s\n",(char *)m_PeerPortID);
		printf("MonitorID:%s\n",(char *)m_MonitorID);
		puts("-------------------------------------Property------------------------------------");
		StringMap::iterator sit;
		while(m_Property.findnext(sit))
			printf("%s=%s\n",(*sit).getkey().getword(),(*sit).getvalue().getword());
		puts("*************************************Port:end************************************");

	}


protected:
	word	m_ID;
	word	m_PeerPortID;
	word	m_MonitorID;
	StringMap	m_Property;
	
};

typedef svutil::hashtable<svutil::word,Port * >	PORTMAP;


#endif
