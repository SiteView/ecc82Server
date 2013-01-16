//#pragma once
#ifndef	SVDB_ENTITY_
#define	SVDB_ENTITY_

#include "SerialBase.h"
#include "svdbtype.h"
#include "StringMap.h"
#include "util.h"


class Entity :
	public SerialBase2
{
public:
	Entity(void);
	~Entity(void);

	word	GetID(void){ return m_ID;}
	void	PutID(word id){ m_ID=id;}

	StringMap	&GetProperty(){ return m_Property;}
	WORDLIST	&GetMonitors(){ return m_Monitors;}


	S_UINT	GetCurrentIndex(){return m_CurrentID;}
	void	AdjustIndex(int count){ 
		m_CurrentID+=count;
		m_CurrentID=m_CurrentID<0 ? 0 : m_CurrentID;
	}

	S_UINT	GetRawDataSize( bool onlyLocked= false);
	char*	GetRawData(char *lpbuf,S_UINT bufsize, bool onlyLocked= false);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	word GetNextMonitorID()
	{
		return ::MakeIDByIndex(m_ID,++m_CurrentID);
	/*	char id[256]={0};
		sprintf(id,"%s.%d",m_ID.getword,++m_CurrentID);
		return id;*/
	}

	void DisplayAllData(void)
	{
		puts("************************Begin*************************");
		printf("Entity id:%s\n",(char *)m_ID);
		printf("Current index:%d\n",m_CurrentID);
		puts("----------------------Property------------------------");
		StringMap::iterator it;
		while(m_Property.findnext(it))
			printf("%s=%s\n",(*it).getkey().getword(),(*it).getvalue().getword());
		puts("----------------------Monitors------------------------");
		WORDLIST::iterator itw;
		for(itw=m_Monitors.begin();itw!=m_Monitors.end();itw++)
			printf("%s\n",(*itw).getword());

		puts("***********************End*****************************");
	}

	bool DeleteSubMonitorID(word id)
	{
		WORDLIST::iterator it;
		for(it=m_Monitors.begin();it!=m_Monitors.end();it++)
		{
			if((*it)==id)
			{
				m_Monitors.erase(it);
				return true;
			}
		}
		return false;
	}


protected:
	S_UINT	m_CurrentID;
	word	m_ID;
	StringMap	m_Property;
	WORDLIST	m_Monitors;

};

#endif
