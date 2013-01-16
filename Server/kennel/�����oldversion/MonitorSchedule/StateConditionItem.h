//#pragma once

#ifndef  DRAGONFLOW_STATECONDITIONITEM
#define DRAGONFLOW_STATECONDITIONITEM

#include "MonitorSchedule.h"

class StateConditionItem
{
public:
	int m_ItemID;
	CString m_ParamName;
	char	m_Operator[10];
	CString m_ParamValue;


	StateConditionItem(void);
	~StateConditionItem(void);
};

typedef std::list<StateConditionItem *> CStateConditionItemList;

#endif
