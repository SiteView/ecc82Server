//#pragma once

#ifndef		DRAGONFLOW_ENTITY
#define		DRAGONFLOW_ENTITY

#include <cc++/thread.h>
#include "MonitorSchedule.h"
#include "EntityItem.h"
#include <svapi.h>


class MSException;

class Entity
{
public:

	void AdjustActiveMonitorRef(int index,BOOL isA);
	void SetMonitorTypeCount(int count);
	BOOL InitActiveMonitorsArray(int count);

	Entity(void);
	~Entity(void);

	enum { systemtypelen=30,iplen=30 };

/*	CEntityItemList & GetEntityItemList(void)
	{
		return m_EntityItemList;
	}*/

	void SetEntityID(const char *EntityID)
	{
		if(EntityID&&(strlen(EntityID)<GUIDLEN))
			strcpy(m_EntityID,EntityID);
		else
			throw MSException("Invalid parameter <EntityID>");
	}

/*	void SetParentID(const char *ParentID)
	{
		if(ParentID&&(strlen(ParentID)<GUIDLEN))
			strcpy(m_ParentID,ParentID);
		else
			throw MSException("Invalid parameter <ParentID>");

	}*/

	void SetSystemType(const char *SystemType)
	{
		if(SystemType&&(strlen(SystemType)<systemtypelen))
			strcpy(m_SystemType,SystemType);
		else
			throw MSException("Invalid parameter <SystemType>");

	}
	void SetDepend(const char *Depend)
	{
		if(Depend&&(strlen(Depend)<GUIDLEN))
			strcpy(m_Depend,Depend);
		else
			throw MSException("Invalid parameter <Depend>");

	}
	void SetIPAdress(const char *IPAdress)
	{
		if(IPAdress&&(strlen(IPAdress)<GUIDLEN))
			strcpy(m_IPAdress,IPAdress);
		else
			throw MSException("Invalid parameter <IPAdress>");
	}

	const char *GetEntityID(void) const
	{
		return m_EntityID;
	}
	string GetParentID(void) const
	{
	//	return m_ParentID;
		return ::FindParentID(m_EntityID);
	}
	const char *GetSystemType(void) const
	{
		return m_SystemType;
	}
	const char *GetIPAdress(void) const
	{
		return m_IPAdress;
	}
	const char *GetDepend(void) const
	{
		return m_Depend;
	}
	void SetDependsCondition(int n)
	{
		m_DependsCondition=n;
	}
	int GetDependsCondition(void)
	{
		return m_DependsCondition;
	}

	inline void AdjustActiveMonitorRef(BOOL isA)
	{
		MutexLock lock(m_mutex);
		isA ? m_nCurrentActiveMonitor++ : m_nCurrentActiveMonitor--;
	}
	inline int GetActiveMonitor(void)
	{
		return m_nCurrentActiveMonitor;
	}
	inline void SetActiveMonitor(int n)
	{
		MutexLock lock(m_mutex);
		m_nCurrentActiveMonitor=n;
	}
	inline int GetActiveMonitorByIndex(int index)
	{
		if((!m_pActiveMonitors)||(index>m_nMonitorTypeCount)||(index<0))
			return 0;
		return m_pActiveMonitors[index];
	}

	int GetMonitorTypeCount()
	{
		return m_nMonitorTypeCount;
	}
	int *GetActiveMonitorsAP()
	{
		return m_pActiveMonitors;
	}

	//bool CloneActiveMonitorsAP(int *p,int count)
	//{
	//	if((m_nMonitorTypeCount!=count)||(m_pActiveMonitors==NULL))
	//		return false;
	//	memcpy(
	//}

	bool CopyTempData(const Entity *pe)
	{
		MutexLock lock(m_mutex);
		if(m_pActiveMonitors!=NULL)
		{
			delete [] m_pActiveMonitors;
		}

		m_nMonitorTypeCount=pe->m_nMonitorTypeCount;
		m_nCurrentActiveMonitor=pe->m_nCurrentActiveMonitor;
		m_pActiveMonitors=new int[m_nMonitorTypeCount];

		if(m_pActiveMonitors==NULL)
			return false;

		memset(m_pActiveMonitors,0,m_nMonitorTypeCount*sizeof(int));
		memcpy(m_pActiveMonitors,pe->m_pActiveMonitors,m_nMonitorTypeCount*sizeof(int));
		return true;
	}
	

private:
	int m_nMonitorTypeCount;
	int * m_pActiveMonitors;
	Mutex m_mutex;
	int m_nCurrentActiveMonitor;
	char m_EntityID[GUIDLEN];
//	char m_ParentID[GUIDLEN];
	char m_SystemType[systemtypelen];
	char m_IPAdress[iplen];
	char m_Depend[GUIDLEN];
	int  m_DependsCondition;

	//CEntityItemList m_EntityItemList;

};

typedef std::list<Entity *>  CEntityList;


#endif
