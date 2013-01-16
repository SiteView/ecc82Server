#ifndef SVDB_ENTITYEX_
#define SVDB_ENTITYEX_

#include "Port.h"
#include "EntityRelation.h"


class EntityEx
{
public:
	EntityEx(void);
	~EntityEx(void);

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	//ȡentity GUID
	word	GetIdentifier() const
	{
		return m_Identifier;
	}
	std::string GetIdentifierStr(void)  const 
	{
		std::string ret(m_Identifier.getword());
		return ret;
	}
	void PutIdentifier(word id)
	{
		m_Identifier=id;
	}

	word GetDescription() const
	{
		return m_Description;
	}
	std::string GetDescriptionStr(void)  const 
	{
		std::string ret(m_Description.getword());
		return ret;
	}

	void PutDescription(word Des)
	{
		m_Description=Des;
	}

	word GetType() const
	{
		return m_Type;
	}
	std::string GetTypeStr(void)  const 
	{
		std::string ret(m_Type.getword());
		return ret;
	}

	void PutType(word Type)
	{
		m_Type=Type;
	}

	word GetEccEntityID() const
	{
		return m_EccEntityID;
	}
	std::string GetEccEntityIDStr(void)  const 
	{
		std::string ret(m_EccEntityID.getword());
		return ret;
	}

	void PutEccEntityID(word EccEntityID)
	{
		m_EccEntityID=EccEntityID;
	}

	StringMap &GetProperty()
	{
		return m_Property;
	}

	ENTITYRELATIONLIST &GetRelations(void) { return m_Relations; }
	PORTMAP &GetPorts(void) { return m_Ports; }

	void DisplayAllData()
	{
		puts("**********************************EntityEx:begin*****************************");
		printf("Identifier:%s\n",(char *)m_Identifier);
		printf("Description:%s\n",(char *)m_Description);
		printf("Type:%s\n",(char *)m_Type);
		printf("EccEntityID:%s\n",(char *)m_EccEntityID);
		puts("----------------------------------Property---------------------------------");
		StringMap::iterator sit;
		while(m_Property.findnext(sit))
			printf("%s=%s\n",(*sit).getkey().getword(),(*sit).getvalue().getword());
		puts("----------------------------------Relations--------------------------------");
		ENTITYRELATIONLIST::iterator it;
		for(it=m_Relations.begin();it!=m_Relations.end();it++)
			(*it)->DisplayAllData();
		puts("-----------------------------------Ports-----------------------------------");
		PORTMAP::iterator pit;
		while(m_Ports.findnext(pit))
			(*pit).getvalue()->DisplayAllData();

		puts("*********************************EntityEx:end*****************************");
	}


protected:
	word	m_Identifier;
	word	m_Description;
	word	m_Type;
	word	m_EccEntityID;

	StringMap m_Property;

	ENTITYRELATIONLIST	m_Relations;
	PORTMAP	m_Ports;
};

typedef svutil::hashtable< svutil::word,EntityEx * >	ENTITYEXMAP;

#endif
