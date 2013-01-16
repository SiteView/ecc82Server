#ifndef  DRAGONFLOW_COMMUNICATIONTHREAD
#define DRAGONFLOW_COMMUNICATIONTHREAD

#include "threadex.h"

class MonitorSchMain;


class CommunicationThread :
	public ThreadEx
{
public:
	void SetSchMain(MonitorSchMain *pSchMain);
	virtual void run(void);
	CommunicationThread();
	CommunicationThread(MonitorSchMain *pSchMain)
	{
		m_pSchMain=pSchMain;
	}
	virtual ~CommunicationThread();

private:
	MonitorSchMain * m_pSchMain;
};

#endif
