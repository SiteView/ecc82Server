//#pragma once
#ifndef	SVDB_MONITORTEMPLET_
#define	SVDB_MONITORTEMPLET_

#include "SerialBase.h"
#include "svdbtype.h"
#include "StringMap.h"
#include "Resource.h"
#include "ResourcePool.h"
#include <list>

class MonitorTemplet :
	public SerialBase
{
public:
	MonitorTemplet(void);
	~MonitorTemplet(void);

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	int GetID(void){ return m_ID;}
	void PutID(int id) { m_ID=id;}

	StringMap &GetProperty(){return m_Property;}

	STRMAPLIST &GetParameter() { return m_Parameter;}
	STRMAPLIST &GetAdvanceParameter(){return m_AdvanceParameter;}
	STRMAPLIST &GetReturn() {return m_Return;}
	StringMap ** GetAlertCondition(){ return m_AlertCondition;}

	void DisplayAllData(void)
	{
		puts("***************begin***************");
		printf("MonitorTemplet id:%d\n",m_ID);

		puts("--------------Property----------------");
		StringMap::iterator it;
		while(m_Property.findnext(it))
			printf("%s=%s\n",(char *)(*it).getkey(),(char*)(*it).getvalue());
		puts("--------------Parameter---------------");
		STRMAPLIST::iterator itl;
		for(itl=m_Parameter.begin();itl!=m_Parameter.end();itl++)
		{
			puts("/////sub item//////");
			it.clear();
			while((*itl)->findnext(it))
				printf("%s=%s\n",(char *)(*it).getkey(),(char*)(*it).getvalue());
		}

		puts("---------------AdvanceParameter----------");
		for(itl=m_AdvanceParameter.begin();itl!=m_AdvanceParameter.end();itl++)
		{
			puts("//////sub item/////");
			it.clear();
			while((*itl)->findnext(it))
				printf("%s=%s\n",(char *)(*it).getkey(),(char*)(*it).getvalue());		
		}

		puts("--------------AlertCondition-------------");
		for(int i=0;i<3;i++)
		{
			puts("///////sub item/////");
			it.clear();
			while(m_AlertCondition[i]->findnext(it))
				printf("%s=%s\n",(char *)(*it).getkey(),(char*)(*it).getvalue());		

		}

		puts("--------------Return list----------------");
		
		for(itl=m_Return.begin();itl!=m_Return.end();itl++)
		{
			puts("////////sub item/////");
			it.clear();
			while((*itl)->findnext(it))
				printf("%s=%s\n",(char *)(*it).getkey(),(char*)(*it).getvalue());		

		}

		puts("*********************end***********************");

	}

	bool ResolveIds(Resource *m_pLanguage); //非线程安全
	bool SaveNewIds(Resource *m_pLanguage);  

protected:

	int m_ID;

	StringMap	m_Property;

	STRMAPLIST	m_Parameter;

	STRMAPLIST m_AdvanceParameter;

	StringMap * m_AlertCondition[3];

	STRMAPLIST m_Return;

};


#endif
