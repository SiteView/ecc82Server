#include "EntityGroup.h"

EntityGroup::EntityGroup(void)
{
	m_ID="";
}

EntityGroup::~EntityGroup(void)
{
}

S_UINT	EntityGroup::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);

	len+=strlen(m_ID)+1;	//m_ID;

	len+=tlen;				//m_Property size;
	len+=m_Property.GetRawDataSize();

	len+=tlen;				//m_SubEntityKinds count;
	WORDLIST::iterator it;
	for(it=m_SubEntityKinds.begin();it!=m_SubEntityKinds.end();it++)
		len+=strlen(*it)+1;

	return len;

}
char*	EntityGroup::GetRawData(char *lpbuf,S_UINT bufsize)
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


	len=m_SubEntityKinds.size();
	memmove(pt,&len,tlen);
	pt+=tlen;

	WORDLIST::iterator it;
	for(it=m_SubEntityKinds.begin();it!=m_SubEntityKinds.end();it++)
	{
		strcpy(pt,(*it).getword());
		pt+=strlen((*it).getword());
		pt[0]='\0';
		pt++;	
	}

    return lpbuf;  
}
BOOL	EntityGroup::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;

	S_UINT len=0,tlen=sizeof(S_UINT);

	try{

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

		memmove(&len,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;

		const char *wt=NULL;
		for(int i=0;i<len;i++)
		{
			wt=pt;
			pt+=strlen(wt)+1;
			if(pt>pend)
				return false;
			m_SubEntityKinds.push_back(wt);

		}
	}catch(...)
	{
		return false;
	}

	return true;

}


bool EntityGroup::ResolveIds(Resource *m_pLanguage)
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
	return true;
}



bool EntityGroup::SaveNewIds(Resource *m_pLanguage)
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
	return true;
}

