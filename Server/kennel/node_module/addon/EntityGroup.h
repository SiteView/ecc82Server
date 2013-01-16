#ifndef	SVDB_SERIALBASE_
#define	SVDB_SERIALBASE_

#include "SerialBase.h"
#include "svdbtype.h"
#include "StringMap.h"
#include "util.h"
#include "Resource.h"
#include "ResourcePool.h"

class EntityGroup :
	public SerialBase
{
public:
	EntityGroup(void);
	~EntityGroup(void);

	word	GetID(){ return m_ID;}
	void	PutID(word id) { m_ID=id;}

	StringMap	&GetProperty(){ return m_Property;}
	WORDLIST	&GetEntityKinds(){ return m_SubEntityKinds;}


	bool DeleteSubEntityKindsID(word id)
	{
		WORDLIST::iterator it;
		for(it=m_SubEntityKinds.begin();it!=m_SubEntityKinds.end();it++)
		{
			if((*it)==id)
			{
				m_SubEntityKinds.erase(it);
				return true;
			}
		}
		return false;
	}

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	void DisplayAllData(void)
	{
		puts("************************Begin*************************");
		printf("EntityGroup id:%s\n",(char *)m_ID);
		puts("----------------------Property------------------------");
		StringMap::iterator it;
		while(m_Property.findnext(it))
			printf("%s=%s\n",(*it).getkey().getword(),(*it).getvalue().getword());
		puts("----------------------EntityKinds------------------------");
		WORDLIST::iterator itw;
		for(itw=m_SubEntityKinds.begin();itw!=m_SubEntityKinds.end();itw++)
			printf("%s\n",(*itw).getword());

		puts("***********************End*****************************");
	}


	bool ResolveIds(Resource *m_pLanguage); //非线程安全
	bool SaveNewIds(Resource *m_pLanguage); 

protected:
    word		m_ID;
	StringMap	m_Property;
	WORDLIST	m_SubEntityKinds;
};

#endif
