#ifndef	SVDB_THREADEX_
#define	SVDB_THREADEX_

#include "svdbtype.h"
#include <cc++/thread.h>

class ThreadEx :
	public ost::Thread
{
public:
	ThreadEx(void);
	~ThreadEx(void);

	virtual void run(void)=0;
	void SetThreadName(word threadName)
	{
		m_ThreadName=threadName;

	}

	int Start(void)
	{
		return (start()>=0);
	}

protected:
	word	m_ThreadName;
	unsigned int m_ThreadID;

};
typedef std::list<ThreadEx *> THREADLIST;

#endif
