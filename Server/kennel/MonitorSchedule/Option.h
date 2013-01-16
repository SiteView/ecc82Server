#ifndef DRAGONFLOW_OPTION_H
#define DRAGONFLOW_OPTION_H

#include "MonitorSchedule.h"

class Option
{
public:
	Option(void);
	~Option(void);

	bool LoadOption();

	bool m_isDemo;
	std::string m_PreLoadLibrary;
	std::string m_DemoDLL;
	std::string m_DemoFunction;
	int	m_seid;
	string m_ServerAddress;
	int m_checktime;
	bool m_UseLocalBuffer;
};

#endif
