#ifndef	SVDB_WATCHTHREAD_
#define	SVDB_WATCHTHREAD_

#include "svdbtype.h"
#include "ThreadEx.h"
#include "SvdbMain.h"
#include "ThreadContrl.h"

class WatchThread :
	public ThreadEx
{
public:
	WatchThread(void);
	~WatchThread(void);

	void toExit(void)
	{
		//cout<<"WatchThread::toExit"<<endl;
		this->exit();
	}

	bool Init();
	void run(void);

	SvdbMain *m_pMain;
protected:
	ThreadContrl *m_pTC;

};
#endif
