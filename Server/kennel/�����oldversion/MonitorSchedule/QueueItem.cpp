#include "QueueItem.h" //此处不同

extern Util *putil;


QueueItem::QueueItem()
{
	m_QueueName=NULL;

}

QueueItem::~QueueItem()
{
	if(m_QueueName)
		::free(m_QueueName);

}

void QueueItem::Push(Monitor *pMonitor, BOOL isTail)
{
	ost::MutexLock lock(m_mutex);
	if(pMonitor->m_isInQueue)
	{
		char buf[100]={0};
		sprintf(buf,"%s has in queue\n",pMonitor->GetMonitorID());
		putil->ErrorLog(buf);
		return;
	}

	if(isTail)
		m_MonitorList.push_back(pMonitor);
	else
		m_MonitorList.push_front(pMonitor);
	pMonitor->m_isInQueue=true;

}

Monitor * QueueItem::Pop()
{
	ost::MutexLock lock(m_mutex);

	if(m_MonitorList.size()>0)
	{
		Monitor*pm=m_MonitorList.front(); //此处不同
		m_MonitorList.pop_front();
		pm->m_isInQueue=false;
		return pm; //此处不同
	}else
		return NULL;

/*	if(m_MonitorList.GetCount()>0)
		return m_MonitorList.RemoveHead();
	else
		return NULL;
*/

}
