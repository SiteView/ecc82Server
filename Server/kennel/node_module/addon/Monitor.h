#ifndef	SVDB_MONITOR_
#define	SVDB_MONITOR_

#include "SerialBase.h"
#include "svdbtype.h"
#include "StringMap.h"


class Monitor :
	public SerialBase
{
public:
	Monitor(void);
	~Monitor(void);
	word	GetID(){return m_ID;}
	void	PutID(word id){ m_ID=id;}

	StringMap &GetProperty(void){ return m_Property;}
	StringMap &GetParameters(void){ return m_Parameters;}
	StringMap &GetADParameters(void){ return m_ADParameters;}
	StringMap *GetAlertCondition(void) {return m_AlertCondition;};

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	void	DisplayAllData(void)
	{
		puts("***************************Begin*****************************");
		printf("MonitorID:%s\n",(char *)m_ID);
		puts("--------------------------Property---------------------------");
		StringMap::iterator it;
		while(m_Property.findnext(it))
			printf("%s=%s\n",(*it).getkey().getword(),(*it).getvalue().getword());
		puts("-------------------------Parameter---------------------------");
		it.clear();
		while(m_Parameters.findnext(it))
			printf("%s=%s\n",(*it).getkey().getword(),(*it).getvalue().getword());
		puts("------------------------ADParameter--------------------------");
		it.clear();
		while(m_ADParameters.findnext(it))
			printf("%s=%s\n",(*it).getkey().getword(),(*it).getvalue().getword());
		puts("------------------------AlertCondition------------------------");
		it.clear();
		for(int i=0;i<3;i++)
		{
			switch(i)
			{
			case 0:
				puts("//////////////////////Error alert condition///////////");
				break;
			case 1:
				puts("//////////////////////Warning alert condition///////////");
				break;
			case 2:
				puts("//////////////////////good alert condition///////////");
				break;
			}
			it.clear();
			while(m_AlertCondition[i].findnext(it))
				printf("%s=%s\n",(*it).getkey().getword(),(*it).getvalue().getword());
		}

		puts("***************************End********************************");



	}


protected:
	word		m_ID;
	StringMap	m_Property;
	StringMap	m_Parameters;
	StringMap	m_ADParameters;
	StringMap	m_AlertCondition[3];

};

#endif
