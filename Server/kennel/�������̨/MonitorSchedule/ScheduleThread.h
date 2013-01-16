//#pragma once
#ifndef  DRAGONFLOW_SCHEDULETHREAD
#define DRAGONFLOW_SCHEDULETHREAD

#include "threadex.h"
#include "MonitorSchMain.h"
#include "ThreadContrl.h"


class ScheduleThread :
	public ThreadEx
{
public:
	BOOL Init(void);
	virtual void run(void);
	ScheduleThread();
	ScheduleThread(MonitorSchMain *pSchMain):ThreadEx()
	{
		m_pSchMain=pSchMain;
		
	}

	void SetSchMain(MonitorSchMain *pSchMain)
	{
		m_pSchMain=pSchMain;
	}
	virtual ~ScheduleThread();

protected:
	BOOL InitMonitorTime(void);
	ThreadContrl* m_pThreadContrl;
	MonitorSchMain * m_pSchMain;
};

#endif
