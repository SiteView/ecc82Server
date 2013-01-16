#include ".\statecondition.h"

StateCondition::StateCondition(void)
{
}

StateCondition::~StateCondition(void)
{
	CStateConditionItemList::iterator it;
	for(it=m_ConditionList.begin();it!=m_ConditionList.end();it++)
	{
		delete (*it);
	}
	m_ConditionList.clear();

}
