#include "Task.h"

Task::Task(void)
{
}

Task::~Task(void)
{
}
bool Task::ToTextMap(STRINGMAP &strmap)
{
	char buf[65]={0};

	strmap["taskname"]=m_taskname;
	strmap["type"]=itoa(m_type,buf,10);
	int i=0;
	char bkey[256]={0};
	int n=0;
	int j=0;
	for(i=0;i<7;i++)
	{
		sprintf(bkey,"week_%d",i);
		strmap[bkey]=m_week[i].m_enable ? "true" : "false";
		sprintf(bkey,"week_%d_tasksize",i);
		n=(int)m_week[i].m_task.size();
		strmap[bkey]=itoa(n,buf,10);
		for(j=0;j<n;j++)
		{
			sprintf(bkey,"week_%d_task_%d_beginhour",i,j);
			strmap[bkey]=itoa(m_week[i].m_task[j].m_beginhour,buf,10);

			sprintf(bkey,"week_%d_task_%d_beginminute",i,j);
			strmap[bkey]=itoa(m_week[i].m_task[j].m_beginminute,buf,10);

			sprintf(bkey,"week_%d_task_%d_endhour",i,j);
			strmap[bkey]=itoa(m_week[i].m_task[j].m_endhour,buf,10);

			sprintf(bkey,"week_%d_task_%d_endminute",i,j);
			strmap[bkey]=itoa(m_week[i].m_task[j].m_endminute,buf,10);
		}
	}

	return true;
}
bool Task::FromTextMap(STRINGMAP &strmap)
{
	svutil::word *pstr=NULL;
	if((pstr=strmap.find("taskname"))==NULL)
		return false;

	m_taskname=pstr->getword();
	if((pstr=strmap.find("type"))==NULL)
		return false;

	m_type=atoi(pstr->getword());
	char bkey[256]={0};

	int n=0;
	int size=0;
	int j=0;
	for(int i=0;i<7;i++)
	{
		sprintf(bkey,"week_%d",i);
		if((pstr=strmap.find(bkey))==NULL)
			return false;
		m_week[i].m_enable=strcmp(pstr->getword(),"true")==0;

		sprintf(bkey,"week_%d_tasksize",i);
		if((pstr=strmap.find(bkey))==NULL)
			return false;
		size=atoi(pstr->getword());
		m_week[i].m_task.resize(size);
		for(j=0;j<size;j++)
		{
			sprintf(bkey,"week_%d_task_%d_beginhour",i,j);
			if((pstr=strmap.find(bkey))==NULL)
				return false;
			m_week[i].m_task[j].m_beginhour=atoi(pstr->getword());

			sprintf(bkey,"week_%d_task_%d_beginminute",i,j);
			if((pstr=strmap.find(bkey))==NULL)
				return false;
			m_week[i].m_task[j].m_beginminute=atoi(pstr->getword());

			sprintf(bkey,"week_%d_task_%d_endhour",i,j);
			if((pstr=strmap.find(bkey))==NULL)
				return false;
			m_week[i].m_task[j].m_endhour=atoi(pstr->getword());

			sprintf(bkey,"week_%d_task_%d_endminute",i,j);
			if((pstr=strmap.find(bkey))==NULL)
				return false;
			m_week[i].m_task[j].m_endminute=atoi(pstr->getword());

		}
		
	}





	return true;
}
