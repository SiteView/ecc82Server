#ifndef  DRAGONFLOW_RETURNVALUE
#define DRAGONFLOW_RETURNVALUE

#include "MonitorSchedule.h"
#include "ReturnData.h"

#define		RETVALUEMAXCOUNT		1024*2


class ReturnValue
{
public:
	void Clear(void);
	ReturnData * m_pReturnData;
	ReturnValue();
	virtual ~ReturnValue();

	union val{
		int		nVal;
		float	fVal;
		char	szVal[RETVALUEMAXCOUNT];
	}m_value;
};

typedef std::list<ReturnValue *>  CReturnValueList;


#endif
