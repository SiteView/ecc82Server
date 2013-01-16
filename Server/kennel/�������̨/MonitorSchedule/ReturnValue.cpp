#include ".\returnvalue.h"

ReturnValue::ReturnValue()
{
	Clear();

}

ReturnValue::~ReturnValue()
{
	m_pReturnData=NULL;

}

void ReturnValue::Clear()
{
	m_pReturnData=NULL;
	memset(&m_value,0,sizeof(m_value));

}
