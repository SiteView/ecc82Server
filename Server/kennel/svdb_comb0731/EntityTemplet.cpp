#include "EntityTemplet.h"

EntityTemplet::EntityTemplet(void)
{
}

EntityTemplet::~EntityTemplet(void)
{
	STRMAPLIST::iterator it;
	for(it=m_ContrlList.begin();it!=m_ContrlList.end();it++)
	{
		if((*it)!=NULL)
			delete (*it);
	}
}

S_UINT EntityTemplet::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);
    
	len+=strlen(m_ID)+1;  //m_ID

	len+=tlen;				//m_Property size;
	len+=m_Property.GetRawDataSize();


	len+=tlen;	//m_ContrlList count;
	STRMAPLIST::iterator it;
	for(it=m_ContrlList.begin();it!=m_ContrlList.end();it++)
	{
		len+=tlen;
		len+=(*it)->GetRawDataSize();
	}

	len+=tlen; //m_MonitorKinds count;
	len+=(m_MonitorKinds.size()*sizeof(int));

	return len;

}
char* EntityTemplet::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;

	if(bufsize<GetRawDataSize())
		return NULL;

	S_UINT len=0,tlen=sizeof(S_UINT);
	char *pt=lpbuf;

	strcpy(pt,m_ID);
	pt+=strlen(m_ID);
	pt[0]='\0';
	pt++;

	len=m_Property.GetRawDataSize();
	memmove(pt,&len,tlen);
	pt+=tlen;
	if(m_Property.GetRawData(pt,len)==NULL)
		return NULL;
	pt+=len;


	len=m_ContrlList.size();
	memmove(pt,&len,tlen);
	pt+=tlen;

	STRMAPLIST::iterator it;
	for(it=m_ContrlList.begin();it!=m_ContrlList.end();it++)
	{
		len=(*it)->GetRawDataSize();
		memmove(pt,&len,tlen);
		pt+=tlen;
		if(!(*it)->GetRawData(pt,len))
			return NULL;
		pt+=len;

	}

	len=m_MonitorKinds.size();
	memmove(pt,&len,tlen);
	pt+=tlen;

	int nkind;

	std::list<int>::iterator kit;
	for(kit=m_MonitorKinds.begin();kit!=m_MonitorKinds.end();kit++)
	{
		nkind=(*kit);
		memmove(pt,&nkind,sizeof(int));
		pt+=sizeof(int);
	}

	return lpbuf;


}
BOOL EntityTemplet::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;

	S_UINT len=0,tlen=sizeof(S_UINT);
	try
	{
		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;

		m_ID=pt;
		pt+=strlen(m_ID)+1;
		if(pt>pend)
			return false;

		memmove(&len,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;

		if(!m_Property.CreateObjectByRawData(pt,len))
			return false;
		pt+=len;
		if(pt>pend)
			return false;


		S_UINT count=0;
		memmove(&count,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;

		for(S_UINT i=0;i<count;i++)
		{
			memmove(&len,pt,tlen);
			pt+=tlen;
			if(pt>pend)
				return false;

			StringMap *pmap=new StringMap();
			if(!pmap->CreateObjectByRawData(pt,len))
				return false;
			pt+=len;            
			if(pt>pend)
				return false;
			m_ContrlList.push_back(pmap);

		}

		memmove(&count,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;
		int nkind=0;
		for(i=0;i<count;i++)
		{
			memmove(&nkind,pt,sizeof(int));
			pt+=sizeof(int);
			if(pt>pend)
				return false;
			m_MonitorKinds.push_back(nkind);
		}


	}catch(...)
	{
		return false;
	}

	return true;

}


bool EntityTemplet::ResolveIds(Resource *m_pLanguage)
{
	if(m_pLanguage==NULL)
		return false;

	StringMap & lang= m_pLanguage->GetResourceDict();
	if( lang.size()==0 )
		return false;

	StringMap::iterator itp;
	while(m_Property.findnext(itp))
	{
		word *pv=NULL;
		if( ( pv=lang.find((*itp).getvalue()) )!=NULL )
		{
			word temp(*pv);
			(*itp).setvalue(temp);
		}
	}

	STRMAPLIST::iterator it;
	StringMap::iterator sit;
	for(it=m_ContrlList.begin();it!=m_ContrlList.end();it++)
	{
		sit.clear();
		while((*it)->findnext(sit))
		{
			word *pv=NULL;
			if( ( pv=lang.find((*sit).getvalue()) )!=NULL )
			{
				word temp(*pv);
				(*sit).setvalue(temp);
			}
		}
	}
	return true;
}



bool EntityTemplet::SaveNewIds(Resource *m_pLanguage)
{
	if(m_pLanguage==NULL)
		return false;

	IdcUser tempuser;
	StringMap::iterator itp;
	while(m_Property.findnext(itp))
	{
		string value= (*itp).getvalue().getword();
		string newvalue= tempuser.CreatChineseIDS((*itp).getkey().getword(),value,m_pLanguage);
		if( value.compare(newvalue)==0 )
			continue;
		(*itp).setvalue(newvalue.c_str());
	}

	STRMAPLIST::iterator it;
	StringMap::iterator sit;
	for(it=m_ContrlList.begin();it!=m_ContrlList.end();it++)
	{
		sit.clear();
		while((*it)->findnext(sit))
		{
			string value= (*sit).getvalue().getword();
			string newvalue= tempuser.CreatChineseIDS((*sit).getkey().getword(),value,m_pLanguage);
			if( value.compare(newvalue)==0 )
				continue;
			(*sit).setvalue(newvalue.c_str());
		}
	}

	return true;
}
