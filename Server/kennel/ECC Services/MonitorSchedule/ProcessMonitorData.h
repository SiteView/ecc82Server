#ifndef DRAGONFLOW_PROCESSMONITORDATA_H
#define DRAGONFLOW_PROCESSMONITORDATA_H

#include "MonitorSchedule.h"


class ProcessMonitorData
{
public:
	ProcessMonitorData(void);
	~ProcessMonitorData(void);

	int m_datalen;
	char *m_data;
	int m_state;

	string m_dllname;
	string m_funname;

	int GetRawDataSize(void);
	bool GetRawData(char *databuf,int len);
	bool CreateObjectByRawData(const char *data,int len);
	void Clear();

	
};

#endif
