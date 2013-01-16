#ifndef  DRAGONFLOW_MSGQUEUE
#define DRAGONFLOW_MSGQUEUE

#include <cc++/thread.h>
#include <list>


#ifdef	CCXX_NAMESPACES
using namespace ost;
#endif

using namespace std;


typedef struct tmsg{
	long	type;
	void	*pdata;
}TMSG,*PTMSG;

typedef std::list<PTMSG> TMSGLIST;

#define		QUEUEFULL		-1
#define		QUEUEOK			0
#define		QUEUEEMPTY		-2
#define		QUEUEERROR		-3



class MSGQueue
{
public:
	MSGQueue(void);
	~MSGQueue(void);

	enum MAXMSG{ maxmsg=1000,initcount=200 };
	int SendMsg(long type,void *ptr,int flags=1);

	void Init(bool usinglist) {
		m_usinglist=usinglist;
	}

	int SendMsg(PTMSG pmsg,int flags=1)
	{
		return SendMsg(pmsg->type,pmsg->pdata,flags);
	}

	int RecMsg(PTMSG pmsg,int flags=1);

	bool WaitMsg(void)
	{
		return m_event.wait();
	}

private:
	bool	m_usinglist;
	int		m_head;
	int		m_tail;
	TMSG	m_msgs[initcount];
	long	m_maxcount;
	TMSGLIST	m_msglist;
	Mutex m_mutex;
	Event m_event;

};

#endif
