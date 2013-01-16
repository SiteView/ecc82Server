//#pragma once
#ifndef		DRAGONFLOW_RETURNDATA
#define		DRAGONFLOW_RETURNDATA

#include "MonitorSchedule.h"

class ReturnData
{
public:

	char m_Type[8];
	char m_Unit[20];
	CString m_Name;
	CString m_Label;


	ReturnData(void);
	~ReturnData(void);
};

typedef std::list<ReturnData *>  CReturnDataList;

#endif
