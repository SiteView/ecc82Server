#include "Groups.h"

Groups::Groups()
{
	memset(m_SEID,0,10);

}

Groups::~Groups()
{

	CEntityList::iterator it;
	if(!m_EntityList.empty())
	{
		for(it=m_EntityList.begin();it!=m_EntityList.end();it++)
		{
			if((*it)!=NULL)
		    	delete (*it);
		}
		m_EntityList.clear();
	}

	CGroupsItemList::iterator itt;
	if(!m_GroupsList.empty())
	{
		for(itt=m_GroupsList.begin();itt!=m_GroupsList.end();itt++)
		{
			delete (*itt);
		}
		m_GroupsList.clear();
	}


/*	while(!m_EntityList.IsEmpty())
	{
		Entity*pe=m_EntityList.RemoveTail();
		delete pe;
	}
	while(!m_GroupsList.IsEmpty())
	{
		GroupsItem*pItem=m_GroupsList.RemoveTail();
		delete pItem;
	}
	*/

}

Entity * Groups::GetEntityByID(const char *EntityID)
{
	if(m_EntityList.empty())
		return NULL;

	CEntityList::iterator it;
	for(it=m_EntityList.begin();it!=m_EntityList.end();it++)
	{
		if(stricmp((*it)->GetEntityID(),EntityID)==0)
			return *it;
	}
	return NULL;

/*	POSITION pos = m_EntityList.GetHeadPosition();
	while(pos)
	{
		Entity *pEntity=m_EntityList.GetNext(pos);
		if(_stricmp(pEntity->GetEntityID(),EntityID)==0)
			return pEntity;
	}

	return NULL;*/
}

GroupsItem * Groups::GetSingleGroupByID(const char *GroupID)
{
	if(m_GroupsList.empty())
		return NULL;

	CGroupsItemList::iterator it;
	for(it=m_GroupsList.begin();it!=m_GroupsList.end();it++)
	{
		if(stricmp((*it)->GetGroupID(),GroupID)==0)
			return *it;
	}

	return NULL;

/*	POSITION pos = m_GroupsList.GetHeadPosition();
	while(pos)
	{
		GroupsItem *pGroup=m_GroupsList.GetNext(pos);
		if(_stricmp(pGroup->GetGroupID(),GroupID)==0)
			return pGroup;
	}
	return NULL;
	*/

}

BOOL Groups::InitAllEntityActiveMonitorArray(int nCount)
{
	if(m_EntityList.empty())
		return TRUE;
	CEntityList::iterator it;
	for(it=m_EntityList.begin();it!=m_EntityList.end();it++)
	{
		(*it)->InitActiveMonitorsArray(nCount);
	}

	return TRUE;

/*	POSITION pos = m_EntityList.GetHeadPosition();
	while(pos)
	{
		Entity *pE=m_EntityList.GetNext(pos);
		pE->InitActiveMonitorsArray(nCount);
	}

	return TRUE;*/
}
bool Groups::FromTextMap(STRINGMAP &strmap)
{
	svutil::word *pstr=NULL;
	
	if((pstr=strmap.find("seid"))!=NULL)
	{
		if(strlen(pstr->getword())>=10)
			return false;

		strcpy(m_SEID,pstr->getword());

	}

	int n=0;
	if((pstr=strmap.find("GroupListSize"))==NULL)
	{
		return false;
	}
	n=atoi(pstr->getword());
	int i=0;
	char bkey[256]={0};
	GroupsItem *pitem=NULL;
	for(i=0;i<n;i++)
	{
		pitem=new GroupsItem();
		if(pitem==NULL)
			return false;

		sprintf(bkey,"Groups_Item_GroupID_%d",i);
		if((pstr=strmap.find(bkey))==NULL)
		{
			delete pitem;
			return false;
		}
		pitem->SetGroupID(pstr->getword());

		sprintf(bkey,"Groups_Item_ParentID_%d",i);
		if((pstr=strmap.find(bkey))==NULL)
		{
			delete pitem;
			return false;
		}
		pitem->SetParentID(pstr->getword());

		sprintf(bkey,"Groups_Item_DependSon_%d",i);
		if((pstr=strmap.find(bkey))==NULL)
		{
			delete pitem;
			return false;
		}

		pitem->SetDepedSon(pstr->getword());

		sprintf(bkey,"Groups_Item_DependsCondition_%d",i);
		if((pstr=strmap.find(bkey))==NULL)
		{
			delete pitem;
			return false;
		}
		pitem->SetDependsCondition(atoi(pstr->getword()));

		m_GroupsList.push_back(pitem);

	}

	if((pstr=strmap.find("EntityListSize"))==NULL)
	{
		return false;
	}
	n=atoi(pstr->getword());
	memset(bkey,0,256);
    Entity *pentity=NULL;

	for(i=0;i<n;i++)
	{
		pentity= new Entity();

		sprintf(bkey,"Entitys_Item_MonitorTypeCount_%d",i);

		if((pstr=strmap.find(bkey))==NULL)
		{
			delete pentity;
			return false;
		}

		pentity->SetMonitorTypeCount(atoi(pstr->getword()));

		sprintf(bkey,"Entitys_Item_EntityID_%d",i);

		if((pstr=strmap.find(bkey))==NULL)
		{
			delete pentity;
			return false;
		}
		pentity->SetEntityID(pstr->getword());

		sprintf(bkey,"Entitys_Item_SystemType_%d",i);

		if((pstr=strmap.find(bkey))==NULL)
		{
			delete pentity;
			return false;
		}
		pentity->SetSystemType(pstr->getword());

		sprintf(bkey,"Entitys_Item_IPAdress_%d",i);

		if((pstr=strmap.find(bkey))==NULL)
		{
			delete pentity;
			return false;
		}
		pentity->SetIPAdress(pstr->getword());


		sprintf(bkey,"Entitys_Item_Depend_%d",i);

		if((pstr=strmap.find(bkey))==NULL)
		{
			delete pentity;
			return false;
		}
		pentity->SetDepend(pstr->getword());

		sprintf(bkey,"Entitys_Item_DependsCondition_%d",i);

		if((pstr=strmap.find(bkey))==NULL)
		{
			delete pentity;
			return false;
		}

		pentity->SetDependsCondition(atoi(pstr->getword()));

		m_EntityList.push_back(pentity);



	}

	return true;
}

bool Groups::ToTextMap(STRINGMAP &strmap)
{
	char buf[65]={0};

	strmap["seid"]=m_SEID;
	strmap["GroupListSize"]=_i64toa(m_GroupsList.size(),buf,10);

	int n=0;
	char bkey[256]={0};

	CGroupsItemList::iterator it;
	for(it=m_GroupsList.begin();it!=m_GroupsList.end();it++)
	{
		sprintf(bkey,"Groups_Item_GroupID_%d",n);
		strmap[bkey]=(*it)->GetGroupID();

		sprintf(bkey,"Groups_Item_ParentID_%d",n);
		strmap[bkey]=(*it)->GetParentID();

		sprintf(bkey,"Groups_Item_DependSon_%d",n);
		strmap[bkey]=(*it)->GetDependSon();

		sprintf(bkey,"Groups_Item_DependsCondition_%d",n);
		strmap[bkey]=itoa((*it)->GetDependsCondition(),buf,10);

		n++;
	}

	strmap["EntityListSize"]=_i64toa(m_EntityList.size(),buf,10);
	n=0;

	CEntityList::iterator ite;
	for(ite=m_EntityList.begin();ite!=m_EntityList.end();ite++)
	{
		sprintf(bkey,"Entitys_Item_MonitorTypeCount_%d",n);
		strmap[bkey]=itoa((*ite)->GetMonitorTypeCount(),buf,10);

		sprintf(bkey,"Entitys_Item_EntityID_%d",n);
		strmap[bkey]=(*ite)->GetEntityID();

		sprintf(bkey,"Entitys_Item_SystemType_%d",n);
		strmap[bkey]=(*ite)->GetSystemType();

		sprintf(bkey,"Entitys_Item_IPAdress_%d",n);
		strmap[bkey]=(*ite)->GetIPAdress();
		
		sprintf(bkey,"Entitys_Item_Depend_%d",n);
		strmap[bkey]=(*ite)->GetDepend();

		sprintf(bkey,"Entitys_Item_DependsCondition_%d",n);
		strmap[bkey]=itoa((*ite)->GetDependsCondition(),buf,10);
		n++;
	}


	return TRUE;
}