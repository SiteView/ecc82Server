#ifndef	SVDB_VIRTUALGROUP_
#define SVDB_VIRTUALGROUP_

#include "SerialBase.h"
#include "svdbtype.h"
#include "StringMap.h"
#include "util.h"


class VirtualGroup :
	public SerialBase
{
public:
	VirtualGroup(void);
	~VirtualGroup(void);

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	word GetID() { return m_ID; }
	void PutID(word id) { m_ID=id; }

	word GetLabel() { return m_Label;}
	void PutLabel(word label) { m_Label=label; }

	word GetDescription() { return m_Description; }
	void PutDescription(word des) { m_Description=des; }

	StringMap &GetProperty() { return m_Property; }
	WORDLIST &GetEntityList() { return m_EntityList; }

	void DisplayAllData()
	{
		puts("**********************************VirtualGroup:begin*****************************");
		printf("ID:%s\n",(char *)m_ID);
		printf("Label:%s\n",(char *)m_Label);
		printf("Description:%s\n",(char *)m_Description);
		puts("------------------------------------Property----------------------------------------");
		StringMap::iterator sit;
		while(m_Property.findnext(sit))
			printf("%s=%s\n",(*sit).getkey().getword(),(*sit).getvalue().getword());
		puts("-----------------------------------EntityList--------------------------------------");
		WORDLIST::iterator it;
		for(it=m_EntityList.begin();it!=m_EntityList.end();it++)
			puts((char *)(*it).getword());
		puts("***********************************VirtualGroup:end*******************************");

	}


protected:
	word	m_ID;
	word	m_Label;
	word	m_Description;
	StringMap	m_Property;
	WORDLIST	m_EntityList;
};

typedef svutil::hashtable< svutil::word , VirtualGroup * > VIRTUALGROUPMAP;

#endif
