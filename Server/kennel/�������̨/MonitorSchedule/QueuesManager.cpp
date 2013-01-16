#include "QueuesManager.h"

void WriteLog( const char* str );

QueuesManager::QueuesManager()
{
	m_nQueueCount=0;
	for(int i=0;i<MAXQUEUE;i++)
	    m_Queue[i]=NULL;

	m_pSchMain=NULL;


}

QueuesManager::~QueuesManager()
{

}

BOOL QueuesManager::Init()
{
	if(!m_pSchMain)
		throw MSException("Error : m_pSchMain is NULL when QueueManager initialization");
	CreateMainQueue();
	CreateQueueBySubsequent();

	Groups *pG=m_pSchMain->GetGroups();
	pG->InitAllEntityActiveMonitorArray(m_nQueueCount);

	return TRUE;

}

BOOL QueuesManager::CreateQueueBySubsequent()
{
	Subsequent *pSub=m_pSchMain->GetSubsequtent();
	if(!pSub)
		return FALSE;
	CSubsequentItemList &ItemList=pSub->GetSubsequentItemList();
//	int count=ItemList.GetCount();
	int count=(int)ItemList.size();
	if(count<=0)
		return FALSE;
//	POSITION pos=NULL;
	CSubsequentItemList::iterator it;
	SubsequentItem*pItem=NULL;
	QueueItem *pQueueItem=NULL;

	it=ItemList.begin();
	for(int i=0;(i<count)&&(i<MAXQUEUE-1);i++)
	{
		
//		pos=ItemList.FindIndex(i);
//		pItem=ItemList.GetAt(pos);
		pItem=*it++;

		pQueueItem=new QueueItem(pItem->m_strClass);
		m_Queue[i+1]=pQueueItem;
		m_nQueueCount++;
	}
		
	return TRUE;
}

BOOL QueuesManager::CreateMainQueue()
{
	QueueItem *pQueue=new QueueItem("MainQueueName");
	if(pQueue==NULL)
		throw MSException("Error : Create main queue error");
	m_Queue[0]=pQueue;
	m_nQueueCount++;

	return TRUE;

}

BOOL QueuesManager::Push(Monitor *pMonitor, const char *ClassName, BOOL isTail)
{
	if(!pMonitor)
		return FALSE;
	if(ClassName==NULL)
		m_Queue[0]->Push(pMonitor,isTail);

	int index=GetQueueIndexByClass(ClassName);
	m_Queue[index]->Push(pMonitor,isTail);
	pMonitor->SetQueueIndex(index);


	return TRUE;

}

int QueuesManager::GetQueueIndexByClass(const char *ClassName)
{
	Subsequent *pSub=m_pSchMain->GetSubsequtent();
	if(!pSub)
		return FALSE;
	CSubsequentItemList &ItemList=pSub->GetSubsequentItemList();
//	int count=ItemList.GetCount();
	int count=(int)ItemList.size();
	if(count<=0)
		return FALSE;
//	POSITION pos=NULL;

	CSubsequentItemList::iterator it;

	SubsequentItem*pItem=NULL;
	QueueItem *pQueueItem=NULL;

	it=ItemList.begin();

	for(int i=0;(i<count)&&(i<MAXQUEUE-1);i++)
	{
		
	//	pos=ItemList.FindIndex(i);
	//	pItem=ItemList.GetAt(pos);
		pItem=*it++;
		if(stricmp(ClassName,pItem->m_strClass)==0)
			return i+1;
	}

	return 0;



}

Monitor* QueuesManager::Pop(const char *ClassName)
{
	if(ClassName==NULL)
		return m_Queue[0]->Pop();
	int index=GetQueueIndexByClass(ClassName);
	return m_Queue[index]->Pop();

}

BOOL QueuesManager::Push(Monitor *pMonitor, int QueueIndex,BOOL isTail)
{
	if(!pMonitor)
		return FALSE;
	if((QueueIndex<0)||(QueueIndex>=MAXQUEUE))
		return FALSE;

	m_Queue[QueueIndex]->Push(pMonitor,isTail);

	return TRUE;
	

}

Monitor* QueuesManager::Pop(int QueueIndex)
{
	if((QueueIndex<0)||(QueueIndex>=MAXQUEUE))
		return NULL;
	return m_Queue[QueueIndex]->Pop();

}

BOOL QueuesManager::Push(Monitor *pMonitor,BOOL isTail)
{
	int index=pMonitor->GetQueueIndex();
	if(index<0)
		return Push(pMonitor,(LPCSTR)pMonitor->GetMonitorClass(),isTail);
	if(index>=MAXQUEUE)
		return FALSE;
	m_Queue[index]->Push(pMonitor,isTail);

	return TRUE;


}

void QueuesManager::ListAllQueueInfo()
{
	for(int i=0;i<m_nQueueCount;i++)
	{
//		printf("Queue name:%s--Monitor count:%d\n",m_Queue[i]->GetQueueName(),m_Queue[i]->GetMonitorList().GetCount());
		printf("Queue name:%s--Monitor count:%d\n",m_Queue[i]->GetQueueName(),(int)(m_Queue[i]->GetMonitorList().size()));

		char szTemp[1024] = {0};
		sprintf( szTemp, "Queue name:%s--Monitor count:%d\n", m_Queue[i]->GetQueueName(), (int)(m_Queue[i]->GetMonitorList().size()) );
		WriteLog( szTemp );
	}


}
