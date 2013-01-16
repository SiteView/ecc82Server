#include "ThreadContrl.h"

extern Util *putil;
extern CString g_strRootPath;

#ifdef	WIN32
#define		WM_CHECKTASKS		WM_USER+100
#define		WM_CHECKSINGLETASK	WM_USER+101
#else
#define		WM_CHECKTASKS		100
#define		WM_CHECKSINGLETASK	101
#endif


void WriteLog( const char* str );

ThreadContrl::ThreadContrl()
{
	for(int i=0;i<MAXCOUNT;i++)
		m_Threads[i]=NULL;

	m_nThreadCount=0;
	m_pTaskQueueManager=NULL;
	m_isThreadPoolFull=FALSE;
	m_pOption=NULL;


}

ThreadContrl::~ThreadContrl()
{

}

BOOL ThreadContrl::Init()
{
	m_pTaskQueueManager=new QueuesManager(m_pSchMain);
	if(m_pTaskQueueManager==NULL)
		throw MSException("Error : Create QueueManager failed");
	m_pTaskQueueManager->Init();

	SetThreadName("Task check thread");
	m_pOption=m_pSchMain->m_pOption;

	return AddNewThreads(/*WORKCOUNT+Univ::pluscount*/70)>0;

}

int ThreadContrl::AddNewThreads(int count)
{
	if(count+m_nThreadCount>=MAXCOUNT)
		count=MAXCOUNT-m_nThreadCount;

	int nold=m_nThreadCount;
	CString str;

	MonitorThread *pmt = NULL;
	for(int i=0;i<count;i++)
	{
	    pmt=new MonitorThread();
		pmt->SetThreadContrl(this);
		if(!pmt)
			continue;
		str.Format("Work thread %d",m_nThreadCount);
		pmt->SetThreadName((LPCSTR)str);
		m_Threads[m_nThreadCount]=pmt;
//		m_IdleThreadsQueue.AddTail(pmt);
		m_IdleThreadsQueue.push_back(pmt);
		if(!pmt->Start())
		{
			delete pmt;
			continue;
		}
		m_nThreadCount++; 
	}

//	::Sleep(1000);  //�ʱ,ȴ�����.
        ThreadEx::sleep(1000);

	return m_nThreadCount-nold;


}

ThreadEx * ThreadContrl::GetIdleThread()
{
//	Lock lock(m_ThreadMutex);
	MutexLock lock(m_ThreadMutex);

	if(m_IdleThreadsQueue.size()>0)
	{
		ThreadEx *pte=m_IdleThreadsQueue.front();
		m_IdleThreadsQueue.pop_front();
		return pte;
	}
	else
	{
		if(AddNewThreads(2)==0)
			return NULL;
		return GetIdleThread();
	}

}

BOOL ThreadContrl::AddTask(Monitor *pMonitor)
{ 
//	if(m_hThread==NULL)
//		throw MSException("Error : NO ThreadContrl thread");

	CString strError="";
	if(pMonitor==NULL)
		throw MSException("Monitor is empty");

	//bool ist=false;

	//if(stricmp(pMonitor->GetMonitorClass().getText(),"telnet")==0)
	//{
	//	char buf[100]={0};
	//	sprintf(buf,"////////////////class:%s telnet monitor:%s//////////////\n",pMonitor->GetMonitorClass().getText(),
	//		pMonitor->GetMonitorID());

	//	ist=true;

	//	//putil->ErrorLog(buf);
	//}

	if(pMonitor->GetSkipCount()>=MAXSKIPCOUNT)
	{
		strError.Format("SkipCount > %d, MonitorID:%s, this monitor will be deleted. ",MAXSKIPCOUNT,pMonitor->GetMonitorID());
		m_pSchMain->DeleteMonitorV70(pMonitor->GetMonitorID());
		string strTemp = strError.getText();
		WriteLog( strTemp.c_str() );
		//putil->ErrorLog(strError);
		//throw MSException((LPCSTR)strError);
		return false;
	}

	if(pMonitor->GetRunning())
	{
		pMonitor->SetSkipCount(pMonitor->GetSkipCount()+1);
		strError.Format("Monitor skip count:%d,Monitor ID:%s",pMonitor->GetSkipCount(),pMonitor->GetMonitorID());
		string strTemp = strError.getText();
		WriteLog( strTemp.c_str() );
		//putil->ErrorLog(strError);

	}else{
		if(!CheckSubsequent(pMonitor))
		{
			//if(ist)
			//	//putil->ErrorLog("will Start Monitor");
			StartMonitor(pMonitor);
	//		StartMonitor(PopMonitorInQueue(pMonitor));
		}
		else
		{
			//if(ist)
			//	//putil->ErrorLog("Enter queue");
			AddQueue(pMonitor);
		}
	}


	return TRUE;

}

void ThreadContrl::run()
{

#ifdef WIN32

	MSG msg;
	Monitor*pMonitor=NULL;

	m_ThreadID=::GetCurrentThreadId();

//	::SetThreadPriority(this->m_hThread,THREAD_PRIORITY_HIGHEST);
	try{
		
		while(TRUE)
		{
			if(::PeekMessage(&msg,NULL,0,0,PM_REMOVE))
			{
				switch(msg.message)
				{
				case WM_CHECKTASKS:
					CheckAllTaskQueue();
					break;
				case WM_CHECKSINGLETASK:
					////putil->ErrorLog("Get check single task");
					pMonitor=(Monitor*)msg.lParam;
				//	ProcessSingleTask(pMonitor);
					ProcessSingleTaskEx(pMonitor);
					break;
				default:
					TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}
				
				
			}else
				if(!::WaitMessage())
					throw MSException("Contrl thread exit");
		}
		
	}catch(MSException &e)
	{
		CString strError="";
		strError.Format("ThreadContrl dispatch thread exit by :%s",e.Description);
		//putil->ErrorLog((LPCSTR)strError);
		ExitProcess(3);
	}catch(...)
	{
		CString strError="";
		strError.Format("ThreadContrl dispatch thread exit by :%d",::GetLastError());
		//putil->ErrorLog((LPCSTR)strError);
		ExitProcess(4);

	}

#else

	TMSG tmsg;
	Monitor *pMonitor=NULL;

	try{

		while(m_MSGQueue.RecMsg(&tmsg)==0)
		{
			switch(tmsg.type)
			{
			case WM_CHECKTASKS:
				CheckAllTaskQueue();
				break;
			case WM_CHECKSINGLETASK:
				pMonitor=(Monitor*)tmsg.pdata;
				ProcessSingleTaskEx(pMonitor);
				break;
			}
		}

	}catch(MSException &e)
	{
		CString strError="";
		strError.Format("ThreadContrl dispatch thread exit by :%s",e.Description);
		//putil->ErrorLog((LPCSTR)strError);
		//ExitProcess(3);
		::exit(1);
	}


#endif


}

bool ThreadContrl::CheckSubsequent(Monitor *pMonitor,bool &bTotal,bool &bPer)
{
	bTotal=false;
	bPer=false;

	int nTotal=0,nPer=0;
	if(pMonitor->GetSubsequent()==Monitor::subsequentno)
		return false;

	SubsequentItem*pItem=pMonitor->GetSubsequentItem();
	if(pItem==NULL)
	{
		Subsequent *pSub=m_pSchMain->GetSubsequtent();
		if(!pSub->GetValueByMonitor(pMonitor,nTotal,nPer))
			return false;
		pItem=pMonitor->GetSubsequentItem();
	}else
	{
		nTotal=pItem->m_nTotal;
		nPer=pItem->m_nPerEntity;
	}


	if(nTotal<=pItem->GetCurrentTotal())
	{
		bTotal=true;
		return true;
	}


	Entity*pEntity=pMonitor->GetEntity();
	if(pEntity==NULL)
	{
		string eid=pMonitor->GetParentID();
		pEntity=m_pSchMain->GetGroups()->GetEntityByID(eid.c_str());
		if(pEntity==NULL)
			return false;
		pMonitor->SetEntity(pEntity);
		

	}

/*	if(pMonitor->GetMonitorClass().CompareNoCase("wmi")==0)
	{
		int n=m_pTaskQueueManager->GetQueueIndexByClass("wmi");
     	printf("Entity active :%d,Entity ID:%s\n",pEntity->GetActiveMonitorByIndex(n),pEntity->GetEntityID());
	}*/

	if(pMonitor->GetQueueIndex()>=0)
	{
		if( nPer<=pEntity->GetActiveMonitorByIndex(pMonitor->GetQueueIndex()))
		{
			bPer=true;
			return true;
		}
	}
	else
	{
		int n=m_pTaskQueueManager->GetQueueIndexByClass((LPCSTR)pMonitor->GetMonitorClass());
		pMonitor->SetQueueIndex(n);
		if(nPer<=pEntity->GetActiveMonitorByIndex(n))
		{
			bPer=true;
			return true;
		}else
			return false;
	}

	return false;


}

BOOL ThreadContrl::CheckSubsequent(Monitor *pMonitor)
{

	int nTotal=0,nPer=0;
	if(pMonitor->GetSubsequent()==Monitor::subsequentno)
		return FALSE;

	SubsequentItem*pItem=pMonitor->GetSubsequentItem();
	if(pItem==NULL)
	{
		Subsequent *pSub=m_pSchMain->GetSubsequtent();
		if(!pSub->GetValueByMonitor(pMonitor,nTotal,nPer))
			return FALSE;
		pItem=pMonitor->GetSubsequentItem();
	}else
	{
		nTotal=pItem->m_nTotal;
		nPer=pItem->m_nPerEntity;
	}

	//if(pMonitor->GetMonitorClass().CompareNoCase("Lotus")==0)
	//{
	//	char buf[200]={0};
	//	sprintf(buf,"Total :%d,per:%d,curt:%d\n",nTotal,nPer,pItem->GetCurrentTotal());
	//	//putil->ErrorLog(buf);
	//}


	if(nTotal<=pItem->GetCurrentTotal())
		return TRUE;


	Entity*pEntity=pMonitor->GetEntity();
	if(pEntity==NULL)
	{
		string eid=pMonitor->GetParentID();
		printf("eid is:%s\n",eid.c_str());
		pEntity=m_pSchMain->GetGroups()->GetEntityByID(eid.c_str());
		if(pEntity==NULL)
		{
			puts("eeeeeeeeeeeeeeeeeeeeeeeeee");
			return FALSE;
		}
		pMonitor->SetEntity(pEntity);
		

	}

	//if(pMonitor->GetMonitorClass().CompareNoCase("Lotus")==0)
	//{
	//	char ebuf[100]={0};
	//	int n=m_pTaskQueueManager->GetQueueIndexByClass("telnet");
 //    	sprintf(ebuf,"Entity active :%d,Entity ID:%s\n",pEntity->GetActiveMonitorByIndex(n),pEntity->GetEntityID());
	//	//putil->ErrorLog(ebuf);
	//}

	if(pMonitor->GetQueueIndex()>=0)
		return nPer<=pEntity->GetActiveMonitorByIndex(pMonitor->GetQueueIndex());
	else
	{
		int n=m_pTaskQueueManager->GetQueueIndexByClass((LPCSTR)pMonitor->GetMonitorClass());
		pMonitor->SetQueueIndex(n);
		return nPer<=pEntity->GetActiveMonitorByIndex(n);
	}

	return FALSE;


}

void ThreadContrl::StartMonitor(Monitor *pMonitor)
{
	MonitorThread *pThread=(MonitorThread*)GetIdleThread();
	if(pThread==NULL)
	{
		m_isThreadPoolFull=TRUE;
		AddQueue(pMonitor);
		return ;
	}
	pThread->ExecuteMonitor(pMonitor);


}

void ThreadContrl::AddQueue(Monitor *pMonitor)
{
	m_pTaskQueueManager->Push(pMonitor);
//	::PostThreadMessage(m_ThreadID,WM_CHECKSINGLETASK,0,(LPARAM)pMonitor);

}
Monitor *ThreadContrl::PopMonitorInQueue(Monitor *pMonitor)
{
	Monitor *pmt=NULL;

	if(pMonitor->GetQueueIndex()>=0)
		pmt=m_pTaskQueueManager->Pop(pMonitor->GetQueueIndex());
	else
		pmt=m_pTaskQueueManager->Pop((LPCSTR)pMonitor->GetMonitorClass());

	if(pmt!=NULL)
	{
		AddQueue(pMonitor);
		return pmt;
	}

	return pMonitor;
}

BOOL ThreadContrl::ProcessSingleTaskEx(Monitor *pMonitor)
{
	//char ebuf[256]={0};
	//sprintf(ebuf,"In processST,class:%s\n",(LPCSTR)pMonitor->GetMonitorClass());
	////putil->ErrorLog(ebuf);

	if(m_isThreadPoolFull)
	{
		CheckAllTaskQueue();
		return TRUE;
	}

	Monitor *pTa=NULL;
	if(pMonitor->GetQueueIndex()>=0)
   	    pTa=m_pTaskQueueManager->Pop(pMonitor->GetQueueIndex());
	else
		pTa=m_pTaskQueueManager->Pop((LPCSTR)pMonitor->GetMonitorClass());

	Monitor *ptemp=NULL;

	bool bTotal=false;
	bool bPer=false;


	while(pTa!=NULL)
	{
		if(ptemp==pTa)
		{
			////putil->ErrorLog("---loop over exit-----");
			m_pTaskQueueManager->Push(pTa,FALSE);
			break;
		}

		if(ptemp==NULL)
	    	ptemp=pTa;

		//sprintf(ebuf,"Get Monitor id:%s,class:%s\n",pTa->GetMonitorID(),(LPCSTR)pMonitor->GetMonitorClass());
  //   	//putil->ErrorLog(ebuf);

		if((!CheckSubsequent(pTa,bTotal,bPer))&&(!pTa->GetRunning()))
		{
			MonitorThread *pThread=(MonitorThread*)GetIdleThread();
			if(pThread==NULL)
			{
				m_isThreadPoolFull=TRUE;
				m_pTaskQueueManager->Push(pTa,FALSE);
				////putil->ErrorLog("NO idle thread");
				return TRUE;
			}else
			{
				if(pTa==NULL)
				{
					return FALSE;
				}
				if(pTa==ptemp)
					ptemp=NULL;

				////putil->ErrorLog("Execute Monitor");
				pThread->ExecuteMonitor(pTa);
			}
		}else
		{
			////putil->ErrorLog("Monitor subsequent contrl");
			if(bTotal)
			{
		    	////putil->ErrorLog("Monitor subsequent total full");
				m_pTaskQueueManager->Push(pTa,FALSE);
				return TRUE;
			}
			m_pTaskQueueManager->Push(pTa);				

		}

		if(pTa->GetQueueIndex()>=0)
			pTa=m_pTaskQueueManager->Pop(pTa->GetQueueIndex());
		else
			pTa=m_pTaskQueueManager->Pop((LPCSTR)pTa->GetMonitorClass());

		////putil->ErrorLog("Loop ...");


	}

	////putil->ErrorLog("....check return.....");

	return TRUE;

}

BOOL ThreadContrl::ProcessSingleTask(Monitor *pMonitor)
{
	char ebuf[256]={0};
	sprintf(ebuf,"In processST,class:%s\n",(LPCSTR)pMonitor->GetMonitorClass());
	//putil->ErrorLog(ebuf);

	if(m_isThreadPoolFull)
	{
		CheckAllTaskQueue();
		return TRUE;
	}

	Monitor *pTa=NULL;
	if(pMonitor->GetQueueIndex()>=0)
   	    pTa=m_pTaskQueueManager->Pop(pMonitor->GetQueueIndex());
	else
		pTa=m_pTaskQueueManager->Pop((LPCSTR)pMonitor->GetMonitorClass());

	while(pTa!=NULL)
	{
		sprintf(ebuf,"Get Monitor id:%s,class:%s\n",pTa->GetMonitorID(),(LPCSTR)pMonitor->GetMonitorClass());
     	//putil->ErrorLog(ebuf);

		if((!CheckSubsequent(pTa))&&(!pTa->GetRunning()))
		{
			MonitorThread *pThread=(MonitorThread*)GetIdleThread();
			if(pThread==NULL)
			{
				m_isThreadPoolFull=TRUE;
				m_pTaskQueueManager->Push(pTa,FALSE);
				//putil->ErrorLog("NO idle thread");
				return TRUE;
			}else
			{
				if(pTa==NULL)
				{
					return FALSE;
				}
				pThread->ExecuteMonitor(pTa);
				//putil->ErrorLog("Execute Monitor");
			}
		}else
		{
			if(pTa->GetRunning())
			{
				//putil->ErrorLog("Monitor is running");

				m_pTaskQueueManager->Push(pTa);
				Monitor *ptem=pTa;
				if(pTa->GetQueueIndex()>=0)
					pTa=m_pTaskQueueManager->Pop(pTa->GetQueueIndex());
				else
					pTa=m_pTaskQueueManager->Pop((LPCSTR)pTa->GetMonitorClass());

				if(pTa==ptem)
				{
					//putil->ErrorLog("Same monitor");
					return TRUE;
				}

				//putil->ErrorLog("different monitor");

				continue;
				
			}
			else
			{
		    	//putil->ErrorLog("Subsequent contrl disable");
				m_pTaskQueueManager->Push(pTa,FALSE);
		    	return TRUE;
			}

		}

		if(pTa->GetQueueIndex()>=0)
			pTa=m_pTaskQueueManager->Pop(pTa->GetQueueIndex());
		else
			pTa=m_pTaskQueueManager->Pop((LPCSTR)pTa->GetMonitorClass());

	}

	return TRUE;


}

void ThreadContrl::CheckTaskQueueByMonitor(Monitor *pMonitor)
{
#ifdef	WIN32
	//char buf[100]={0};
	//sprintf(buf,"will post message threadid:%d\n",m_ThreadID);
	////putil->ErrorLog(buf);
	::PostThreadMessage(m_ThreadID,WM_CHECKSINGLETASK,0,(LPARAM)pMonitor);
#else
	m_MSGQueue.SendMsg(WM_CHECKSINGLETASK,(void *)pMonitor);
#endif

}

BOOL ThreadContrl::ProcessSingleTaskQueueByIndex(int QueueIndex)
{
	Monitor *pTa=NULL;
    pTa=m_pTaskQueueManager->Pop(QueueIndex);
	while(pTa!=NULL)
	{
		if(!CheckSubsequent(pTa))
		{
			MonitorThread *pThread=(MonitorThread*)GetIdleThread();
			if(pThread==NULL)
			{
				m_isThreadPoolFull=TRUE;
				m_pTaskQueueManager->Push(pTa,FALSE);
				return TRUE;
			}else
				pThread->ExecuteMonitor(pTa);
		}else
		{
           	m_pTaskQueueManager->Push(pTa,FALSE);
			return TRUE;
		}
		
		pTa=m_pTaskQueueManager->Pop(QueueIndex);


	}

	return TRUE;


}

void ThreadContrl::CheckAllTaskQueue()
{
	m_isThreadPoolFull=FALSE;
     for(int i=0;i<m_pTaskQueueManager->GetQueueCount();i++)
		 ProcessSingleTaskQueueByIndex(i);

}

void ThreadContrl::AddToIdleThread(ThreadEx *pThread)
{
	MutexLock lock(m_ThreadMutex);
//	m_IdleThreadsQueue.AddTail(pThread);
	m_IdleThreadsQueue.push_back(pThread);

}

void ThreadContrl::PrintTaskQueueInfo()
{

	m_pTaskQueueManager->ListAllQueueInfo();

	char szTemp[1024] = {0};
	sprintf( szTemp, "total:%d, work:%d \n", m_nThreadCount, m_nThreadCount-m_IdleThreadsQueue.size() );
	puts(szTemp);
	OutputDebugString(szTemp);
	WriteLog( szTemp );

}

//调整并发控制
void ThreadContrl::AdjustRetByMonitor(Monitor *pMonitor,BOOL isA)
{
	if(pMonitor->GetSubsequentItem()!=NULL)
	{
		pMonitor->GetSubsequentItem()->AdjustRef(isA);
    	if((pMonitor->GetEntity()!=NULL))
		{
			if(pMonitor->GetQueueIndex()>=0)
		    	pMonitor->GetEntity()->AdjustActiveMonitorRef(pMonitor->GetQueueIndex(),isA);
			else
			{
				int n=m_pTaskQueueManager->GetQueueIndexByClass((LPCSTR)pMonitor->GetMonitorClass());
				pMonitor->GetEntity()->AdjustActiveMonitorRef(n,isA);
				pMonitor->SetQueueIndex(n);
			}
		}
	}

}


void ThreadContrl::ExecuteMonitor(Monitor *pMonitor)
{
	if(pMonitor==NULL)
	  return ;
    
     MonitorThread *pThread=(MonitorThread*)GetIdleThread();
     if(pThread==NULL)
     {
        throw MSException("Get Idle thread error");
     }
     pMonitor->m_isRefresh=true;
     
     puts("*****Start monitor...*****");
     pThread->ExecuteMonitor(pMonitor);
     
     int i=0;
     while(pMonitor->GetRunning()&&i<120)
     {
         ThreadEx::sleep(1000);
	     i++;
     }
     
     if(pMonitor->GetRunning())
       puts("*****Timeout*****");
     else
       puts("*****Success*****");
     
   
}




/*!
    \fn ThreadContrl::InitRefresh(void)
 */
bool ThreadContrl::InitRefresh(int nthreads)
{
	if(nthreads<1)
	 return false;
	 
      SetThreadName("Task check thread");
      return AddNewThreads(nthreads)>0;
}


/*!
    \fn ThreadContrl::RefreshMonitors(CMonitorList &lstMonitor)
 */
void ThreadContrl::RefreshMonitors(CMonitorList &lstMonitor)
{
   if(lstMonitor.size()==1)
   {
   	puts("Enter");
      Monitor *pm=lstMonitor.front();
      ExecuteMonitor(pm);
      lstMonitor.pop_front();
      delete pm;
      
      return ;
      
   }
   CMonitorList::iterator it;
   for(it=lstMonitor.begin();it!=lstMonitor.end();it++)
   {
	Monitor *pM=*it;
	pM->m_isRefresh=true;
	while(1)
	{
 		 MonitorThread *pThread=(MonitorThread*)GetIdleThread();
		  if(pThread)
		  {
 			   pThread->ExecuteMonitor(pM);
 			   break;
		  }
 		 else
		  {
 			   ThreadEx::sleep(1000);
  			  continue;
 		 }
	}

      
   }
   
   //wait for monitor complete
   
   BOOL hasRuning;
   for(int n=0;n<300;n++)
   {
   	hasRuning=TRUE;
	for(it=lstMonitor.begin();it!=lstMonitor.end();it++)
	{
		if((*it)->GetRunning())
		{
			hasRuning=FALSE;
			break;
		}
	}
	if(hasRuning)
	  break;
	ThreadEx::sleep(1000);
    }
   
}
