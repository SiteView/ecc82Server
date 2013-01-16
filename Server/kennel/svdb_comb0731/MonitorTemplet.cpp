#include "monitortemplet.h"

MonitorTemplet::MonitorTemplet(void)
{
	m_ID=0;
	for(int i=0;i<3;i++)
	{
		m_AlertCondition[i]=new StringMap();
	}

}

MonitorTemplet::~MonitorTemplet(void)
{
	STRMAPLIST::iterator it;
	for(it=m_Parameter.begin();it!=m_Parameter.end();it++)
		delete (*it);

	for(it=m_AdvanceParameter.begin();it!=m_AdvanceParameter.end();it++)
		delete (*it);

	for(it=m_Return.begin();it!=m_Return.end();it++)
		delete (*it);

	for(int i=0;i<3;i++)
		if(m_AlertCondition[i]!=NULL)
			delete m_AlertCondition[i];


}

S_UINT MonitorTemplet::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);
	len+=sizeof(int);  //m_id

	len+=tlen;   //m_Property data length space
	len+=m_Property.GetRawDataSize();	//m_Property data space
	

//	(S_UINT)m_Parameter.size();
	len+=tlen;	//m_Parameter data length space

	STRMAPLIST::iterator it;
	for(it=m_Parameter.begin();it!=m_Parameter.end();it++)
	{
		len+=tlen;
		len+=(*it)->GetRawDataSize();
	}

//	(S_UINT)m_AdvanceParameter.size();
	len+=tlen;

	for(it=m_AdvanceParameter.begin();it!=m_AdvanceParameter.end();it++)
	{
		len+=tlen;
		len+=(*it)->GetRawDataSize();
	}

//	(S_UINT)m_Return.size();
	len+=tlen;

	for(it=m_Return.begin();it!=m_Return.end();it++)
	{
		len+=tlen;
		len+=(*it)->GetRawDataSize();
	}

	for(int i=0;i<3;i++)
	{
		len+=tlen;
		len+=m_AlertCondition[i]->GetRawDataSize();
	}

	return len;
}
char* MonitorTemplet::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;

	if(bufsize<GetRawDataSize())
		return NULL;

	S_UINT len=0,tlen=sizeof(S_UINT);

/////////////////////////////////////////

	char *pt=lpbuf;
	memmove(pt,&m_ID,sizeof(int));
	pt+=sizeof(int);
////////////////////////////////////

	len=m_Property.GetRawDataSize();
	memmove(pt,&len,tlen);
	pt+=tlen;

	if(!m_Property.GetRawData(pt,len))
		return NULL;
	pt+=len;
/////////////////////////////////////////////////////

	len=(S_UINT)m_Parameter.size();
	memmove(pt,&len,tlen);
	pt+=tlen;


	STRMAPLIST::iterator it;
	for(it=m_Parameter.begin();it!=m_Parameter.end();it++)
	{

		len=(*it)->GetRawDataSize();
		memmove(pt,&len,tlen);
		pt+=tlen;

		if(!(*it)->GetRawData(pt,len))
			return NULL;
		pt+=len;

	}


/////////////////////////////////////////////////////////////


	len=(S_UINT)m_AdvanceParameter.size();
	memmove(pt,&len,tlen);
	pt+=tlen;



	for(it=m_AdvanceParameter.begin();it!=m_AdvanceParameter.end();it++)
	{
		len=(*it)->GetRawDataSize();
		memmove(pt,&len,tlen);
		pt+=tlen;

		if(!(*it)->GetRawData(pt,len))
			return NULL;
		pt+=len;
		
	}

//////////////////////////////////////////////////////////////////////


	len=(S_UINT)m_Return.size();
	memmove(pt,&len,tlen);
	pt+=tlen;

	for(it=m_Return.begin();it!=m_Return.end();it++)
	{
		len=(*it)->GetRawDataSize();
		memmove(pt,&len,tlen);
		pt+=tlen;

		if(!(*it)->GetRawData(pt,len))
			return NULL;
		pt+=len;

	}

	
/////////////////////////////////////////////////////////////////////

	for(int i=0;i<3;i++)
	{
		len=m_AlertCondition[i]->GetRawDataSize();
		memmove(pt,&len,tlen);
		pt+=tlen;

		if(!m_AlertCondition[i]->GetRawData(pt,len))
			return NULL;

		pt+=len;
	}
///////////////////////////////////////////////////////////////////

	return lpbuf;
}
BOOL MonitorTemplet::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;

	S_UINT len=0,tlen=sizeof(S_UINT);

	try{
		const char *pt=lpbuf;
		const char *pend=lpbuf+bufsize;

		memmove(&m_ID,pt,sizeof(int));
		pt+=sizeof(int);

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

		////// parsing parameters /////////////////////////////////////////////

		memmove(&len,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;

		S_UINT nt=0;

		for(S_UINT i=0;i<len;i++)
		{
			memmove(&nt,pt,tlen);
			pt+=tlen;
			if(pt>pend)
				return false;


			StringMap *psm=new StringMap();
			if(!psm->CreateObjectByRawData(pt,nt))
				return false;
			pt+=nt;
			if(pt>pend)
				return false;


			m_Parameter.push_back(psm);

		}

		///////parsing adv. parameters //////////////////////////////////////////

		memmove(&len,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;


		for(S_UINT i=0;i<len;i++)
		{
			memmove(&nt,pt,tlen);
			pt+=tlen;
			if(pt>pend)
				return false;


			StringMap *psm=new StringMap();
			if(!psm->CreateObjectByRawData(pt,nt))
				return false;
			pt+=nt;
			if(pt>pend)
				return false;


			m_AdvanceParameter.push_back(psm);

		}
		//////////////////////////////////////////////////////////////

		memmove(&len,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;


		for(S_UINT i=0;i<len;i++)
		{
			memmove(&nt,pt,tlen);
			pt+=tlen;
			if(pt>pend)
				return false;

			StringMap *psm=new StringMap();
			if(!psm->CreateObjectByRawData(pt,nt))
				return false;
			pt+=nt;
			if(pt>pend)
				return false;

			m_Return.push_back(psm);

		}
		////////////////////////////////////////////////////////////////////

		for(int i=0;i<3;i++)
		{
			memmove(&nt,pt,tlen);
			pt+=tlen;
			if(pt>pend)
				return false;

			StringMap *psm=new StringMap();
			if(!psm->CreateObjectByRawData(pt,nt))
				return false;
			pt+=nt;
			if(pt>pend)
				return false;

			if(m_AlertCondition[i]!=NULL)
				delete m_AlertCondition[i];

			m_AlertCondition[i]=psm;

		}
	}catch(...)
	{
		return false;
	}



	return true;
}

bool MonitorTemplet::ResolveIds(Resource *m_pLanguage)
{
	if(m_pLanguage==NULL)
		return false;

	StringMap & lang= m_pLanguage->GetResourceDict();
	if( lang.size()==0 )
		return false;
	
	StringMap::iterator it;
	while(m_Property.findnext(it))
	{
		word *pv=NULL;
		if( ( pv=lang.find((*it).getvalue()) )!=NULL )
		{
			word temp(*pv);
			(*it).setvalue(temp);
		}
	}

	STRMAPLIST::iterator itl;
	for(itl=m_Parameter.begin();itl!=m_Parameter.end();itl++)
	{
		it.clear();
		while((*itl)->findnext(it))
		{
			word *pv=NULL;
			if( ( pv=lang.find((*it).getvalue()) )!=NULL )
			{
				word temp(*pv);
				(*it).setvalue(temp);
			}
		}
	}

	for(itl=m_AdvanceParameter.begin();itl!=m_AdvanceParameter.end();itl++)
	{
		it.clear();
		while((*itl)->findnext(it))
		{
			word *pv=NULL;
			if( ( pv=lang.find((*it).getvalue()) )!=NULL )
			{
				word temp(*pv);
				(*it).setvalue(temp);
			}
		}		
	}

	for(int i=0;i<3;i++)
	{
		it.clear();
		while(m_AlertCondition[i]->findnext(it))
		{
			word *pv=NULL;
			if( ( pv=lang.find((*it).getvalue()) )!=NULL )
			{
				word temp(*pv);
				(*it).setvalue(temp);
			}
		}
	}


	for(itl=m_Return.begin();itl!=m_Return.end();itl++)
	{
		it.clear();
		while((*itl)->findnext(it))
		{
			word *pv=NULL;
			if( ( pv=lang.find((*it).getvalue()) )!=NULL )
			{
				word temp(*pv);
				(*it).setvalue(temp);
			}
		}		
	}

	return true;
}



bool MonitorTemplet::SaveNewIds(Resource *m_pLanguage)
{
	if(m_pLanguage==NULL)
		return false;

	StringMap::iterator it;
	IdcUser tempuser;

	while(m_Property.findnext(it))
	{
		string value= (*it).getvalue().getword();
		string newvalue= tempuser.CreatChineseIDS((*it).getkey().getword(),value,m_pLanguage);
		if( value.compare(newvalue)==0 )
			continue;
		(*it).setvalue(newvalue.c_str());
	}

	STRMAPLIST::iterator itl;
	for(itl=m_Parameter.begin();itl!=m_Parameter.end();itl++)
	{
		it.clear();
		while((*itl)->findnext(it))
		{
			string value= (*it).getvalue().getword();
			string newvalue= tempuser.CreatChineseIDS((*it).getkey().getword(),value,m_pLanguage);
			if( value.compare(newvalue)==0 )
				continue;
			(*it).setvalue(newvalue.c_str());
		}
	}

	for(itl=m_AdvanceParameter.begin();itl!=m_AdvanceParameter.end();itl++)
	{
		it.clear();
		while((*itl)->findnext(it))
		{
			string value= (*it).getvalue().getword();
			string newvalue= tempuser.CreatChineseIDS((*it).getkey().getword(),value,m_pLanguage);
			if( value.compare(newvalue)==0 )
				continue;
			(*it).setvalue(newvalue.c_str());	
		}
	}

	for(int i=0;i<3;i++)
	{
		it.clear();
		while(m_AlertCondition[i]->findnext(it))
		{
			string value= (*it).getvalue().getword();
			string newvalue= tempuser.CreatChineseIDS((*it).getkey().getword(),value,m_pLanguage);
			if( value.compare(newvalue)==0 )
				continue;
			(*it).setvalue(newvalue.c_str());	
		}		
	}

	for(itl=m_Return.begin();itl!=m_Return.end();itl++)
	{
		it.clear();
		while((*itl)->findnext(it))
		{
			string value= (*it).getvalue().getword();
			string newvalue= tempuser.CreatChineseIDS((*it).getkey().getword(),value,m_pLanguage);
			if( value.compare(newvalue)==0 )
				continue;
			(*it).setvalue(newvalue.c_str());	
		}		
	}

	return true;
}
