#ifndef SVDB_ENTITYRELATION_
#define SVDB_ENTITYRELATION_

#include "SerialBase.h"
#include "svdbtype.h"
#include "StringMap.h"
#include "util.h"
#include <list>


class EntityRelation :
	public SerialBase
{
public:
	EntityRelation(void);
	~EntityRelation(void);

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	word GetType(void) const
	{ 
		return m_Type; 
	}
	void PutType(word type){ m_Type=type; }
	std::string GetTypeStr(void)  const 
	{
		std::string ret(m_Type.getword());
		return ret;
	}

	word GetDescription(void) const {return m_Description; }
	void PutDescription(word Des) { m_Description=Des; }
	std::string GetDescriptionStr(void)  const 
	{
		std::string ret(m_Description.getword());
		return ret;
	}

	word GetPeerID(void) const { return m_PeerID; }
	void PutPeerID(word PeerID) { m_PeerID = PeerID; }
	std::string GetPeerIDStr(void)  const 
	{
		std::string ret(m_PeerID.getword());
		return ret;
	}

	word GetLocalPortID(void) const { return m_LocalPortID; }
	void PutLocalPortID(word LocalPortID) { m_LocalPortID = LocalPortID; }
	std::string GetLocalPortIDStr(void)  const 
	{
		std::string ret(m_LocalPortID.getword());
		return ret;
	}

	StringMap &GetProperty() { return m_Property; }

	void DisplayAllData()
	{
		puts("**********************************EntityRelation:begin*****************************");
		printf("Type:%s\n",(char *)m_Type);
		printf("Description:%s\n",(char *)m_Description);
		printf("PeerID:%s\n",(char *)m_PeerID);
		printf("LocalPortID:%s\n",(char *)m_LocalPortID);
		puts("-------------------------------------Property-------------------------------------");
		StringMap::iterator sit;
		while(m_Property.findnext(sit))
			printf("%s=%s\n",(*sit).getkey().getword(),(*sit).getvalue().getword());
		puts("*********************************EntityRelation:end*********************************");

	}

protected:
	word	m_Type;
	word	m_Description;
	word	m_PeerID;
	word	m_LocalPortID;
	StringMap	m_Property;


};

typedef std::list< EntityRelation * >	ENTITYRELATIONLIST;


#endif
