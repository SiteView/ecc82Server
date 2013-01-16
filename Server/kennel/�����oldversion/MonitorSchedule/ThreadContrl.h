//#pragma once
#ifndef  DRAGONFLOW_THREADCONTRL
#define DRAGONFLOW_THREADCONTRL

#include "MonitorSchedule.h"

#include "MonitorThread.h"
#include "Monitor.h"
#include "MonitorSchMain.h"
#include "QueuesManager.h"
#include "MSGQueue.h"

#include "ThreadEx.h"
#include "Option.h"

///////////////////////////////////////////////////////////////////////
#define		MAXCOUNT	200
#define		SAFECOUNT	MAXCOUNT-20
#define		WORKCOUNT	30
///////////////////////////////////////////////////////////////////////
#define		MAXSKIPCOUNT	5


class ThreadContrl :
	public ThreadEx
{
public:
	void AdjustRetByMonitor(Monitor *pMonitor,BOOL isA);
	void PrintTaskQueueInfo(void);
	void AddToIdleThread(ThreadEx *pThread);
	void CheckTaskQueueByMonitor(Monitor *pMonitor);
	virtual void run(void);
	void SetSchMain(MonitorSchMain *pSchMain){
		m_pSchMain=pSchMain;
	}
	BOOL AddTask(Monitor * pMonitor);
	BOOL Init(void);
	ThreadContrl();
	ThreadContrl(MonitorSchMain *pSchMain):m_pSchMain(pSchMain)
	{
		for(int i=0;i<MAXCOUNT;i++)
			m_Threads[i]=NULL;
		
		m_nThreadCount=0;
		m_pTaskQueueManager=NULL;
		m_isThreadPoolFull=FALSE;
		m_pOption=pSchMain->m_pOption;
		
	}
	virtual ~ThreadContrl();
    bool InitRefresh(int nthreads);
    void RefreshMonitors(CMonitorList &lstMonitor);
    void ExecuteMonitor(Monitor *pMonitor);

	Option *m_pOption;
	MonitorSchMain *m_pSchMain;
private:
	Mutex m_ThreadMutex;
	void AddQueue(Monitor *pMonitor);
	Monitor *PopMonitorInQueue(Monitor *pMonitor);
	void StartMonitor(Monitor *pMonitor);
	QueuesManager *m_pTaskQueueManager;
	ThreadEx * GetIdleThread(void);
//	CMonitorList m_TaskQueue;
	int AddNewThreads(int count);
	int m_nThreadCount;
	ThreadEx * m_Threads[MAXCOUNT];
	
	CThreadList m_IdleThreadsQueue;
	MSGQueue m_MSGQueue;
protected:
	BOOL m_isThreadPoolFull;
	void CheckAllTaskQueue(void);
	BOOL ProcessSingleTaskQueueByIndex(int QueueIndex);
	BOOL ProcessSingleTask(Monitor *pMonitor);
	BOOL ProcessSingleTaskEx(Monitor *pMonitor);
	BOOL CheckSubsequent(Monitor *pMonitor);
	bool CheckSubsequent(Monitor *pMonitor,bool &bTotal,bool &bPer);
    
};

#endif
