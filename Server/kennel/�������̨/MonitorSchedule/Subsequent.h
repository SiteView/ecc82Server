//#pragma once
#ifndef  DRAGONFLOW_SUBSEQUENT
#define DRAGONFLOW_SUBSEQUENT

#include "SubsequentItem.h"
#include "Monitor.h"


class Subsequent
{
public:
	BOOL GetValueByMonitor(Monitor *pMonitor,int &nTotal,int &nPer);
	BOOL GetValueByClassName(const char *ClassName,int &nTotal,int &nPer);
	Subsequent();
	virtual ~Subsequent();

	CSubsequentItemList & GetSubsequentItemList(void)
	{
		return m_ItemList;
	}

	bool ToTextMap(STRINGMAP &strmap);
	bool FromTextMap(STRINGMAP &strmap);

private:
	CSubsequentItemList m_ItemList;
};


#endif
