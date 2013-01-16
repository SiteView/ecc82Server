#ifndef  DRAGONFLOW_GROUPSITEM
#define DRAGONFLOW_GROUPSITEM

#include "MonitorSchedule.h"

class GroupsItem
{
public:
	GroupsItem();
	virtual ~GroupsItem();

	void SetGroupID(const char *GroupID)
	{
		if(GroupID&&(strlen(GroupID)<GUIDLEN))
			strcpy(m_GroupID,GroupID);
		else
			throw MSException("Invalid parameter <GroupID>");
	}
	void SetParentID(const char *ParentID)
	{
		if(ParentID&&(strlen(ParentID)<GUIDLEN))
			strcpy(m_ParentID,ParentID);
		else
			throw MSException("Invalid parameter <ParentID>");
	}
	void SetDepedSon(const char *DependSon)
	{
		if(DependSon&&(strlen(DependSon)<GUIDLEN))
			strcpy(m_DependSon,DependSon);
		else
			throw MSException("Invalid parameter <DependSon>");
	}

	const char * GetGroupID(void) const
	{
		return m_GroupID;
	}
	const char * GetParentID(void) const
	{
		return m_ParentID;
	}
	const char * GetDependSon(void) const
	{
		return m_DependSon;
	}
	void SetDependsCondition(int n)
	{
		m_DependsCondition=n;
	}
	int GetDependsCondition(void)
	{
		return m_DependsCondition;
	}

	

private:
	char m_GroupID[GUIDLEN];
	char m_ParentID[GUIDLEN];
	char m_DependSon[GUIDLEN];
	int  m_DependsCondition;
};

typedef std::list<GroupsItem *>  CGroupsItemList;


#endif
