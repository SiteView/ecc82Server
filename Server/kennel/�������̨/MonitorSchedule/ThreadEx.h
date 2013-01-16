//#pragma once
#ifndef  DRAGONFLOW_THREADEX
#define DRAGONFLOW_THREADEX

#include <cc++\thread.h>
#include "MonitorSchedule.h"


class ThreadEx :
	public Thread
{
public:
	ThreadEx(void);
	~ThreadEx(void);

	virtual void run(void)=0;


	void SetThreadName(const char *threadName)
	{
		if(m_ThreadName)
			::free(m_ThreadName);
		m_ThreadName=_strdup(threadName);

	}

	int Start(void)
	{
		return (start()>=0);
	}

protected:
	char	*m_ThreadName;


	unsigned int m_ThreadID;
};

typedef std::list<ThreadEx *>  CThreadList;


#endif
