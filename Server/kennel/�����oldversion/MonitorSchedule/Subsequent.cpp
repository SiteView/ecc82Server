#include "Subsequent.h"

Subsequent::Subsequent()
{

}

Subsequent::~Subsequent()
{
	CSubsequentItemList::iterator it;
	for(it=m_ItemList.begin();it!=m_ItemList.end();it++)
	{
		delete (*it);
	}
	m_ItemList.clear();

/*	while(!m_ItemList.IsEmpty())
	{
		SubsequentItem*pitem=m_ItemList.RemoveTail();
		delete pitem;
	}
	*/

}

BOOL Subsequent::GetValueByClassName(const char *ClassName, int &nTotal, int &nPer)
{
	CSubsequentItemList::iterator it;
	for(it=m_ItemList.begin();it!=m_ItemList.end();it++)
	{
		if((*it)->m_strClass==ClassName)
		{
			nTotal=(*it)->m_nTotal;
			nPer=(*it)->m_nPerEntity;
			return TRUE;
		}
	}

	return FALSE;


/*	POSITION pos=m_ItemList.GetHeadPosition();
	while(pos)
	{
		SubsequentItem * pItem=m_ItemList.GetNext(pos);
		if(pItem)
		{
			if(pItem->m_strClass.CompareNoCase(ClassName)==0)
			{
				nTotal=pItem->m_nTotal;
				nPer=pItem->m_nPerEntity;
				return TRUE;
			}
		}
	}

	return FALSE;*/

}

BOOL Subsequent::GetValueByMonitor(Monitor *pMonitor, int &nTotal, int &nPer)
{

	CSubsequentItemList::iterator it;
	for(it=m_ItemList.begin();it!=m_ItemList.end();it++)
	{
		if((*it)->m_strClass==pMonitor->GetMonitorClass())
		{
			nTotal=(*it)->m_nTotal;
			nPer=(*it)->m_nPerEntity;
			pMonitor->SetSubsequentItem((*it));
			pMonitor->SetSubsequent(Monitor::subsequentyes);

			return TRUE;
		}
	}

	pMonitor->SetSubsequent(Monitor::subsequentno);
	return FALSE;

/*	POSITION pos=m_ItemList.GetHeadPosition();
	while(pos)
	{
		SubsequentItem * pItem=m_ItemList.GetNext(pos);
		if(pItem)
		{
			if(pItem->m_strClass.CompareNoCase(pMonitor->GetMonitorClass())==0)
			{
				nTotal=pItem->m_nTotal;
				nPer=pItem->m_nPerEntity;
				pMonitor->SetSubsequentItem(pItem);
				pMonitor->SetSubsequent(Monitor::subsequentyes);
				return TRUE;
			}
		}
	}

	pMonitor->SetSubsequent(Monitor::subsequentno);

	return FALSE;*/


}
bool Subsequent::ToTextMap(STRINGMAP &strmap)
{
	char buf[65]={0};
	strmap["ItemListSize"]=_i64toa(m_ItemList.size(),buf,10);
    int n=0;
	char bkey[256]={0};
	CSubsequentItemList::iterator it;
	for(it=m_ItemList.begin();it!=m_ItemList.end();it++)
	{
		sprintf(bkey,"Item_Class_%d",n);
		strmap[bkey]=(*it)->m_strClass.GetBuffer(0);

		sprintf(bkey,"Item_PerEntity_%d",n);
		strmap[bkey]=itoa((*it)->m_nPerEntity,buf,10);

		sprintf(bkey,"Item_Total_%d",n);
		strmap[bkey]=itoa((*it)->m_nTotal,buf,10);

		n++;
	}

	return true;
}
bool Subsequent::FromTextMap(STRINGMAP &strmap)
{
	int n=0;
	svutil::word *pstr=NULL;
	if((pstr=strmap.find("ItemListSize"))==NULL)
		return false;

	n=atoi(pstr->getword());

	char buf[65]={0};
	SubsequentItem *psitem=NULL;
	char bkey[256]={0};

	for(int i=0;i<n;i++)
	{
		psitem=new SubsequentItem();
		sprintf(bkey,"Item_Class_%d",i);
		if((pstr=strmap.find(bkey))==NULL)
		{
			delete psitem;
			return false;
		}
		psitem->m_strClass=pstr->getword();

		sprintf(bkey,"Item_PerEntity_%d",i);
		if((pstr=strmap.find(bkey))==NULL)
		{
			delete psitem;
			return false;
		}

		psitem->m_nPerEntity=atoi(pstr->getword());

		sprintf(bkey,"Item_Total_%d",i);
		if((pstr=strmap.find(bkey))==NULL)
		{
			delete psitem;
			return false;
		}

		psitem->m_nTotal=atoi(pstr->getword());

		m_ItemList.push_back(psitem);

	}

	return true;
}
