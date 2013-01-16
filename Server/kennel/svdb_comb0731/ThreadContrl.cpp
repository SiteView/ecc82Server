#include "threadcontrl.h"
#include "SvdbMain.h"


ThreadContrl::ThreadContrl(void)
{
	m_ThreadCount=0;
	m_isSocket=false;
	exitanyway= false;
}

ThreadContrl::~ThreadContrl(void)
{
}

void ThreadContrl::toExit(void)
{
	//cout<<"ThreadContrl::toExit"<<endl;
	exitanyway=true;
}

bool ThreadContrl::Init(SvdbMain *psvm)
{
	m_pMain=psvm;

	int tempcount=initcount;
	if( m_isSocket )
	{
		tempcount+=IdcUser::SocketTreadPlusCount;
		cout<<"Socket thread count: "<<tempcount<<endl;
	}
	else
	{
		tempcount+=IdcUser::PipeTreadPlusCount;
		cout<<"Pipe thread count: "<<tempcount<<endl;
	}

	if(CreateNewThreads(tempcount)!=tempcount)
		return false;

	return true;
}
int ThreadContrl::CreateNewThreads(int count)
{
	if(exitanyway)
		return 0;

	if(count+m_ThreadCount>=maxcount)
		count=maxcount-m_ThreadCount;

	int len=m_ThreadCount;
	WorkThread *pwt=NULL;
	for(int i=0;i<count;i++)
	{
		pwt=new WorkThread(this,i);
		if(pwt)
		{
	    	pwt->m_isSocket=m_isSocket;
			pwt->SetSvdbMain(m_pMain);

			m_Threads[m_ThreadCount]=pwt;
			m_IdleThreads.push_back(pwt);
			pwt->Start();

			m_ThreadCount++;
		}

	}

	return m_ThreadCount-len;

}

void ThreadContrl::AddToIdleThread(ThreadEx *pThread)
{
	if(exitanyway)
		return ;

	MutexLock lock(m_IdleThreadMutex);
	m_IdleThreads.push_back(pThread);
}
ThreadEx * ThreadContrl::GetIdleThread()
{
	if(exitanyway)
		return NULL;

	MutexLock lock(m_IdleThreadMutex);

	if(m_IdleThreads.size()>0)
	{
		ThreadEx *pte=m_IdleThreads.front();
		m_IdleThreads.pop_front();
		return pte;
	}
	else
	{
		if(CreateNewThreads(2)==0)
			return NULL;
		return GetIdleThread();
	}

	return NULL;
}

#ifdef WIN32

bool ThreadContrl::AddTask(HANDLE hd)
{
	if(exitanyway)
		return false;

	if(hd==NULL)
		return true;

	WorkThread *pt=(WorkThread *)GetIdleThread();
	if(pt==NULL)
	{
		for(int count=1; count<=60;  ++count)
		{
			Sleep(500);
			pt=(WorkThread *)GetIdleThread();
			if(pt != NULL)
				break;
		}
		if(pt==NULL)
		{
			puts("--------------  Failed to get idle pipe work thread,  too heavy job or something wrong!  ----------------");
			AddToErrorLog("--------------  Failed to get idle pipe work thread,  too heavy job or something wrong!  ----------------");
			return false;
		}
	}

	puts("Get ok");

	pt->SetPipeHandle(hd);
	pt->trigger();

	return true;

}

#endif

bool ThreadContrl::AddSocketTask(SOCKET sock)
{
	if(exitanyway)
		return false;

	if(sock==0)
		return true;

	WorkThread *pt=(WorkThread *)GetIdleThread();
	if(pt==NULL)
	{
		for(int count=1; count<=60;  ++count)
		{
			Sleep(500);
			pt=(WorkThread *)GetIdleThread();
			if(pt != NULL)
				break;
		}
		if(pt==NULL)
		{
			puts("--------------  Failed to get idle socket work thread,  too heavy job or something wrong!  ----------------");
			AddToErrorLog("--------------  Failed to get idle pipe work thread,  too heavy job or something wrong!  ----------------");
			return false;
		}
	}


	pt->SetSocket(sock);
	pt->trigger();

	return true;


}


