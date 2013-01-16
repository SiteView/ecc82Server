//#pragma once
#ifndef		DRAGONFLOW_SUBSEQUENTITEM
#define		DRAGONFLOW_SUBSEQUENTITEM

#include <cc++/thread.h>
#include "MonitorSchedule.h"

class SubsequentItem
{
public:
	SubsequentItem(void);
	~SubsequentItem(void);

	CString m_strClass;
	int		m_nPerEntity;
	int		m_nTotal;
	int GetCurrentTotal(void)
	{
		return m_nCurrentTotal;
	}
	void AdjustRef(BOOL isA)
	{
		MutexLock lock(m_mutex);
		isA ? m_nCurrentTotal++ : m_nCurrentTotal--;
	}
	void SetCurrentTotal(int n)
	{
		MutexLock lock(m_mutex);
		m_nCurrentTotal=n;
	}

private:
	int m_nCurrentTotal;
	Mutex m_mutex;

};

typedef std::list<SubsequentItem *>  CSubsequentItemList;


#endif
