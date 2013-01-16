#include ".\msgqueue.h"

MSGQueue::MSGQueue(void)
{
	m_maxcount=maxmsg;
	m_event.reset();
	m_usinglist=false;
	m_tail=0;
	m_head=0;

}

MSGQueue::~MSGQueue(void)
{
}

int MSGQueue::SendMsg(long type,void *ptr,int flags)
{
	if((m_msglist.size()>=m_maxcount)&&m_usinglist)
		return QUEUEFULL;
	if(m_usinglist)
	{
		PTMSG pmsg=new TMSG;
		pmsg->type=type;
		pmsg->pdata=(void *)ptr;

		MutexLock lock(m_mutex);
		m_msglist.push_back(pmsg);
		m_event.signal();
		return QUEUEOK;
	}else
	{
		MutexLock lock(m_mutex);
		int count=m_tail>=m_head?m_tail-m_head : m_tail+initcount-m_head;
		if(count==initcount-1)
			return QUEUEFULL;
		m_tail=(m_tail==initcount-1) ? m_tail=1 : m_tail+1;
		m_msgs[m_tail].type=type;
		m_msgs[m_tail].pdata=ptr;
		m_event.signal();
		return QUEUEOK;
	}

}

int MSGQueue::RecMsg(PTMSG pmsg,int flags)
{
	if((m_msglist.size()==0)&&m_usinglist)
	{
		if(flags==1)
		{
			WaitMsg();
			if(m_msglist.size()==0)
				return QUEUEERROR;
		}else
			return QUEUEEMPTY;
	}
	if(m_usinglist)
	{
		MutexLock lock(m_mutex);
		if(m_msglist.size()==0)
			return QUEUEEMPTY;

		PTMSG &ptm=m_msglist.front();
		pmsg=ptm;
		m_msglist.pop_front();
		if(m_msglist.size()==0)
			m_event.reset();

		return QUEUEOK;
	}else
	{
		if((m_tail==m_head)&&(m_head==0))
		{
			if(flags==1)
			{
				WaitMsg();
			}else
				return QUEUEEMPTY;
		}
		MutexLock lock(m_mutex);
		if((m_tail==m_head)&&(m_head==0))
		{
			return QUEUEEMPTY;
		}
		if(m_head==0)
			m_head=1;
		pmsg->type=m_msgs[m_head].type;
		pmsg->pdata=m_msgs[m_head].pdata;
		if(m_tail==m_head)
		{
			m_tail=0;
			m_head=0;
			m_event.reset();
		}else
			m_head=(m_head==initcount-1) ? 1 : m_head+1;

		return QUEUEOK;

	}
}
