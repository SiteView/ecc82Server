#ifndef	SVDB_RESOURCE_
#define	SVDB_RESOURCE_

#include "SerialBase.h"
#include "svdbtype.h"
#include "StringMap.h"


class Resource :
	public SerialBase
{
public:
	Resource(void);
	~Resource(void);
	Resource(StringMap & inputmap,string keys);

	word	GetLanguageType(void){ return m_Language;}
	void	PutLanguageType(word lang){ m_Language=lang;}

	StringMap	&GetResourceDict(void) { return m_ResourceDict;}

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	void	DisplayAllData(void)
	{
		puts("***********************************Begin**************************************");
		printf("Resource language :%s\n",(char *)m_Language);
		puts("-------------------------------Resource dict--------------------------------");
		StringMap::iterator it;
		while(m_ResourceDict.findnext(it))
			printf("%s=%s\n",(*it).getkey().getword(),(*it).getvalue().getword());
		puts("************************************End***************************************");
	}

protected:
	word	m_Language;
	StringMap	m_ResourceDict;

};

#endif