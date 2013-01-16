#ifndef	SVDB_ENTITYTEMPLET_
#define	SVDB_ENTITYTEMPLET_

#include "SerialBase.h"
#include "svdbtype.h"
#include "StringMap.h"
#include "util.h"
#include "Resource.h"
#include "ResourcePool.h"

class EntityTemplet : 
	public SerialBase
{
public:
	EntityTemplet(void);
	~EntityTemplet(void);

	word GetID(){ return m_ID; }
	void PutID( word id ){ m_ID=id; }

	StringMap	&GetProperty(){ return m_Property;}

	STRMAPLIST &GetContrlList(void) { return m_ContrlList; }
	std::list<int> &GetMonitorKinds(void) { return m_MonitorKinds; }

	bool DeleteMonitorKindsID(int id)
	{
		std::list<int>::iterator it;
		for(it=m_MonitorKinds.begin();it!=m_MonitorKinds.end();it++)
		{
			if((*it)==id)
			{
				m_MonitorKinds.erase(it);
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
		puts("********************************Begin************************************");
		printf("ID:%s\n",(char *)m_ID);
		puts("*******************************Property**********************************");
		StringMap::iterator itp;
		while(m_Property.findnext(itp))
			printf("%s=%s\n",(*itp).getkey().getword(),(*itp).getvalue().getword());
		puts("-------------------------------Contrl list-------------------------------");
		STRMAPLIST::iterator it;
		StringMap::iterator sit;
		for(it=m_ContrlList.begin();it!=m_ContrlList.end();it++)
		{
			puts("--------------------------sub contrl---------------------------");
			sit.clear();
			while((*it)->findnext(sit))
				printf("%s=%s\n",(*sit).getkey().getword(),(*sit).getvalue().getword());
		}
		puts("-------------------------------Monitor kinds-----------------------------");
		std::list<int>::iterator nit;
		for(nit=m_MonitorKinds.begin();nit!=m_MonitorKinds.end();nit++)
			printf("%d\n",(*nit));

		puts("*********************************End**************************************");

	}

	bool ResolveIds(Resource *m_pLanguage); //非线程安全
	bool SaveNewIds(Resource *m_pLanguage); 

protected:
	word	m_ID;
	StringMap	m_Property;
	STRMAPLIST m_ContrlList;
	std::list<int> m_MonitorKinds;
};

#endif
