//#pragma once

#ifndef  DRAGONFLOW_GROUPS
#define DRAGONFLOW_GROUPS

#include "MonitorSchedule.h"

#include "Entity.h"	
#include "GroupsItem.h"



class Groups
{
public:
	BOOL InitAllEntityActiveMonitorArray(int nCount);
	GroupsItem * GetSingleGroupByID(const char *GroupID);
	Entity * GetEntityByID(const char *EntityID);
	Groups();
	virtual ~Groups();

	CGroupsItemList & GetGroupsList(void)
	{
		return m_GroupsList;
	}
	CEntityList & GetEntityList(void)
	{
		return m_EntityList;
	}
	const char * GetSEID(void) const
	{
		return m_SEID;
	}
	void SetSEID(const char *seid)
	{
		if(seid)
			strcpy(m_SEID,seid);
	}

	int GetGroupsCount(void)
	{
//		return m_GroupsList.GetCount();
		return (int)m_GroupsList.size();
	}
	int GetEntitysCount(void)
	{
//		return m_EntityList.GetCount();
		return (int)m_EntityList.size();
	}

	bool ToTextMap(STRINGMAP &strmap);
	bool FromTextMap(STRINGMAP &strmap);

private:
	char m_SEID[10];
	CGroupsItemList m_GroupsList;
	CEntityList m_EntityList;
};

#endif
