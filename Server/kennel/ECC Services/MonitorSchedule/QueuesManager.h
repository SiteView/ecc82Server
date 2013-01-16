#ifndef  DRAGONFLOW_QUEUESMANAGER
#define DRAGONFLOW_QUEUESMANAGER

#include "MonitorSchedule.h"

#include "QueueItem.h"
#include "MonitorSchMain.h"

#define		MAXQUEUE	100


class QueuesManager
{
public:
	void ListAllQueueInfo(void);
	BOOL Push(Monitor *pMonitor,BOOL isTail=TRUE);
	Monitor* Pop(int QueueIndex);
	BOOL Push(Monitor *pMonitor,int QueueIndex,BOOL isTail);
	Monitor* Pop(const char *ClassName);
	int GetQueueIndexByClass(const char *ClassName);
	BOOL Push(Monitor *pMonitor,const char *ClassName,BOOL isTail=TRUE);
	BOOL Init(void);
	QueuesManager();
	QueuesManager(MonitorSchMain *pSchMain)
	{
		m_nQueueCount=0;
		for(int i=0;i<MAXQUEUE;i++)
			m_Queue[i]=NULL;
		
		m_pSchMain=pSchMain;
	}
	virtual ~QueuesManager();
	void SetSchMain(MonitorSchMain *pSchMain)
	{
		m_pSchMain=pSchMain;
	}

    int	GetQueueCount(void)
	{
		return m_nQueueCount;
	}

private:
	BOOL CreateMainQueue(void);
	BOOL CreateQueueBySubsequent(void);
	MonitorSchMain * m_pSchMain;
	QueueItem * m_Queue[MAXQUEUE];
	int m_nQueueCount;
};

#endif
