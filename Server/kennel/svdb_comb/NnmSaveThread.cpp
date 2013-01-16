#include "NnmSaveThread.h"

NnmSaveThread::NnmSaveThread()
{
	m_pMain=NULL;
}

NnmSaveThread::~NnmSaveThread(void)
{ 
	this->exit();
}

void NnmSaveThread::toExit()
{
	if(!m_pMain->m_pEntityEx->Submit())
		printf("Failed to save nnm entity.");

	if( !IdcUser::EnableIdc )
	{
		if( !m_pMain->m_pEntity->Submit() )
			printf("Failed to save ecc entity.");
	}

	if(!m_pMain->m_pTopologyChart->Submit())
		printf("Failed to save nnm TopologyChart.");
}

bool NnmSaveThread::Init()
{
	if(m_pMain==NULL)
		return false;
	return true;
}
void NnmSaveThread::run(void)
{
	if(m_pMain==NULL)
	{
		printf("nnm Save Thread runs failed.");
		return ;
	}

	while(true)
	{
		ThreadEx::sleep(1000);	
		if(!m_pMain->m_pEntityEx->Submit())
			printf("Failed to save nnm entity(per 2 second).");
		
		if( !IdcUser::EnableIdc )
		{
			if( !m_pMain->m_pEntity->Submit() )
				printf("Failed to save ecc entity(per 2 second).");
		}

		if(!m_pMain->m_pTopologyChart->Submit())
			printf("Failed to save nnm TopologyChart(per 2 second).");
	}
}

