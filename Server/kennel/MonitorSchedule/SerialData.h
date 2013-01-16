#ifndef	DRAGONFLOW_SERIALDATA_H
#define DRAGONFLOW_SERIALDATA_H

#include "MonitorSchedule.h"

#include "Monitor.h"
#include "Task.h"
#include "Groups.h"
#include "Subsequent.h"


using namespace std;


class SerialData
{
public:
	~SerialData(void);

	SerialData(string filename)
	{
		m_filename=filename;
	}

	int Serial(CMonitorList &MonitorList,Groups *pGroups,Subsequent *pSubsequent,TASKMAP &TaskMap,bool out);

private:
	bool SerialStringMap(STRINGMAP &strmap,FILE *pf);
	bool CovertStringToMap(string str,STRINGMAP &smap);
	int Serial(FILE *pf,CMonitorList &MonitorList,Groups *pGroups,Subsequent *pSubsequent,TASKMAP &TaskMap,bool out);
	string m_filename;

};

#endif
