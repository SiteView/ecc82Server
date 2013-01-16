#include "WatchThread.h"

WatchThread::WatchThread(void)
{
	m_pMain=NULL;
	m_pTC=NULL;
}

WatchThread::~WatchThread(void)
{
	if(m_pTC!=NULL)
		delete m_pTC;
	this->exit();
}

void WatchThread::run(void)
{
#ifdef	WIN32

	if(m_pTC==NULL)
		return;

	puts("In watch thread");


	HANDLE hpipe=NULL,htpipe=NULL;
	BOOL iscon=FALSE;
	hpipe=::CreateNamedPipe(SVDB_PIPENAME,PIPE_ACCESS_DUPLEX|FILE_FLAG_WRITE_THROUGH,PIPE_WAIT|PIPE_TYPE_BYTE,
		MAXSAMEPIPECOUNT,OUTBUFFERSIZE,INBUFFERSIZE,NULL,NULL);
	while(true)
	{
		if(IdcUser::BoolToExit)
			return ;

		if(hpipe==INVALID_HANDLE_VALUE)
		{
			AddToErrorLog("Create pipe failed in watchthread");
			ThreadEx::sleep(1000);
			return;
		}

		puts("Create ok");

		iscon=ConnectNamedPipe(hpipe,NULL) ? TRUE :(::GetLastError()==ERROR_PIPE_CONNECTED);

		puts("connect ok");
		htpipe=hpipe;

		hpipe=::CreateNamedPipe(SVDB_PIPENAME,PIPE_ACCESS_DUPLEX|FILE_FLAG_WRITE_THROUGH,PIPE_WAIT|PIPE_TYPE_BYTE,
			MAXSAMEPIPECOUNT,OUTBUFFERSIZE,INBUFFERSIZE,NULL,NULL);

		if(iscon)
		{
			puts("Connect ok");
			if(!m_pTC->AddTask(htpipe))
			{
				AddToErrorLog("Overrun max number of connect");
				::DisconnectNamedPipe(hpipe);
				::CloseHandle(hpipe);
			}

		}else
		{
			AddToErrorLog("ConnectNamedPipe failed in watchthread");
			::DisconnectNamedPipe(htpipe);
			::CloseHandle(htpipe);
			ThreadEx::sleep(1000);
			continue;

		}
	}



#else

#endif

}
bool WatchThread::Init()
{
	if(m_pMain==NULL)
		return false;

	if(m_pTC!=NULL)
		return false;

	m_pTC=new ThreadContrl();
	if(!m_pTC->Init(m_pMain))
		return false;

	return true;
}
