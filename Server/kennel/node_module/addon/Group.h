//#pragma once
#ifndef	SVDB_GROUP_
#define	SVDB_GROUP_

#include "svdbtype.h"
#include "StringMap.h"
#include "util.h"


class Group :
	public SerialBase2
{
public:
	Group(void);
	~Group(void);

	word	GetID(void){ return m_ID;}
	void	PutID(word id){ m_ID=id;}

	S_UINT	GetCurrentIndex(){return m_CurrentID;}
	void	AdjustIndex(int count){ 
		m_CurrentID+=count;
		m_CurrentID=m_CurrentID<0 ? 0 : m_CurrentID;
	}


	StringMap	&GetProperty(void){ return m_Property;}
	WORDLIST	&GetSubGroups(void){ return m_SubGroups;}
	WORDLIST	&GetEntitys(void){ return m_Entitys;}

	word	GetNextSubID()
	{
		return ::MakeIDByIndex(m_ID,++m_CurrentID);

	/*	char buf[256]={0};
		sprintf(buf,"%s.%d",m_ID,++m_CurrentID);
		return buf;
		*/
	}

	S_UINT	GetRawDataSize( bool onlyLocked= false);
	char*	GetRawData(char *lpbuf,S_UINT bufsize, bool onlyLocked= false);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);


	void DisplayAllData(void)
	{
		puts("**************************begin***********************");
		printf("Group ID:%s\n",(char *)m_ID);
		printf("Current index:%d\n",m_CurrentID);
		puts("------------------------Property----------------------");
		StringMap::iterator it;
		while(m_Property.findnext(it))
			printf("%s=%s\n",(char *)(*it).getkey(),(char *)(*it).getvalue());
		puts("-----------------------Sub Groups----------------------");
		WORDLIST::iterator itw;
		for(itw=m_SubGroups.begin();itw!=m_SubGroups.end();itw++)
			printf("Group id:%s\n",(*itw).getword());
		puts("----------------------Sub Entitys----------------------");
		for(itw=m_Entitys.begin();itw!=m_Entitys.end();itw++)
			printf("Entity id:%s\n",(*itw).getword());

		puts("**************************end**************************");
	}

	bool DeleteSubEntityID(word id)
	{
		WORDLIST::iterator it;
		for(it=m_Entitys.begin();it!=m_Entitys.end();it++)
		{
			if((*it)==id)
			{
				m_Entitys.erase(it);
				return true;
			}
		}
		return false;
	}

	bool DeleteSubGroupID(word id)
	{
		WORDLIST::iterator it;
		for(it=m_SubGroups.begin();it!=m_SubGroups.end();it++)
		{
			if((*it)==id)
			{
				m_SubGroups.erase(it);
				return true;
			}
		}
		return false;
	}


protected:  
	S_UINT	m_CurrentID;

	word	m_ID;
	StringMap	m_Property;

	WORDLIST	m_SubGroups;
	WORDLIST	m_Entitys;

};

#endif
