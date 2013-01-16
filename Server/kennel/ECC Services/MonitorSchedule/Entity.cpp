#include "Entity.h"

Entity::Entity()
{
	m_nCurrentActiveMonitor=0;
	m_pActiveMonitors=NULL;
	m_nMonitorTypeCount=0;
	m_DependsCondition=0;

	m_Depend[0]='\0';
	m_EntityID[0]='\0';
	m_IPAdress[0]='\0';
	m_SystemType[0]='\0';
//	m_ParentID[0]='\0';


}

Entity::~Entity()
{
	if(m_pActiveMonitors)
		delete [] m_pActiveMonitors;


}

BOOL Entity::InitActiveMonitorsArray(int count)
{
	if(count<=0)
		return FALSE;

	m_nMonitorTypeCount=count;
	m_pActiveMonitors=new int[count];
	if(m_pActiveMonitors==NULL)
		return FALSE;
	memset(m_pActiveMonitors,0,count*sizeof(int));
	return TRUE;

}

void Entity::SetMonitorTypeCount(int count)
{
	m_nMonitorTypeCount=count;

}

void Entity::AdjustActiveMonitorRef(int index, BOOL isA)
{
	if((!m_pActiveMonitors)||(index>m_nMonitorTypeCount)||(index<0))
		return;

	MutexLock lock(m_mutex);
	isA ? m_pActiveMonitors[index]++ : m_pActiveMonitors[index]--;

}