//#pragma once
#ifndef		DRAGONFLOW_STATECONDITION
#define		DRAGONFLOW_STATECONDITION

#include "StateConditionItem.h"

class StateCondition
{
public:

	CStateConditionItemList &GetStateConditionList(void)
	{
		return m_ConditionList;
	}

	enum	econ{
		Normal=0x1,
		Warning,
		Error,
	};

	enum econ m_Type;
	CStateConditionItemList m_ConditionList;
	CString m_Expression;

	StateCondition(void);
	~StateCondition(void);
};
#endif
