#ifndef	SVDB_THREADCONTRL_
#define SVDB_THREADCONTRL_
#include "WorkThread.h"
//#include "SvdbMain.h"

class SvdbMain;

class ThreadContrl
{
public:
	ThreadContrl(void);
	~ThreadContrl(void);

	void toExit(void);

#ifdef USEPIPE
	enum { initcount=5,maxcount=MAXSAMEPIPECOUNT };
#else
	enum { initcount=5,maxcount=150 };
#endif

	bool Init(SvdbMain *psvm);
	void AddToIdleThread(ThreadEx *pThread);
#ifdef WIN32
	bool AddTask(HANDLE hd);
#endif 

	bool AddSocketTask(SOCKET sock);
	bool m_isSocket;

protected:
	int CreateNewThreads(int count);
	ThreadEx * ThreadContrl::GetIdleThread();


	int	m_ThreadCount;
	THREADLIST	m_IdleThreads;
	ThreadEx    *m_Threads[maxcount];
	ost::Mutex m_IdleThreadMutex;

	SvdbMain	*m_pMain;

	bool exitanyway;
	

};

#endif;
