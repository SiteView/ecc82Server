#include "SocketWatchThread.h"
#include "MyTCPSocket.h"

SocketWatchThread::SocketWatchThread(void)
{
	m_pMain=NULL;
	m_pTC=NULL;
}

SocketWatchThread::~SocketWatchThread(void)
{ 
	if(m_pTC!=NULL)
		delete m_pTC;
	this->exit();

}
bool SocketWatchThread::Init()
{
	if(m_pMain==NULL)
		return false;

	if(m_pTC!=NULL)
		return false;

	m_pTC=new ThreadContrl();
	m_pTC->m_isSocket=true;
	if(!m_pTC->Init(m_pMain))
		return false;

	return true;

}
void SocketWatchThread::run(void)
{
	in_addr ina;
	ina.S_un.S_addr;

	IPV4Address vb;
	vb=htonl(INADDR_ANY);

	try{
		ost::TCPSocket tsocket(vb,this->m_pMain->m_pOption->m_listenport);
		while(tsocket.isPendingConnection(TIMEOUT_INF))
		{
			if(IdcUser::BoolToExit)
				return ;

			SOCKET sock=accept(tsocket.getSocket(),NULL,NULL);

			m_pTC->AddSocketTask(sock);

		}
	}catch(...)
	{
		AddToErrorLog("Socket listen failed in watchthread");
		return;

	}

}

