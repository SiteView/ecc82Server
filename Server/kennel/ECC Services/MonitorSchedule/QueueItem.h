#ifndef  DRAGONFLOW_QUEUEITEM
#define DRAGONFLOW_QUEUEITEM

#include "MonitorSchedule.h"
#include "Monitor.h"


class QueueItem
{
public:
	Monitor * Pop(void);
	void Push(Monitor *pMonitor,BOOL isTail=TRUE);
	QueueItem();
	QueueItem(const char *QueueName){
		m_QueueName=::strdup(QueueName);
	}
	void SetQueueName(const char *QueueName)
	{
		m_QueueName=::strdup(QueueName);
	}
	CMonitorList &GetMonitorList(void)
	{
		return m_MonitorList;
	}
	const char * GetQueueName(void) const
	{
		return m_QueueName;
	}
	virtual ~QueueItem();

private:
	CMonitorList m_MonitorList;
	char* m_QueueName;
	ost::Mutex	m_mutex;
};
#endif