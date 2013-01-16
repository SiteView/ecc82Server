#include "TopologyChart.h"

TopologyChart::TopologyChart(void)
{
}

TopologyChart::~TopologyChart(void)
{
	EDGELIST::iterator it;
	for(it=m_Edges.begin();it!=m_Edges.end();it++)
		if( (*it)!=NULL )
			delete (*it);
}

S_UINT	TopologyChart::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);
	len+=tlen;  //datalen;

	try{
		len+=(S_UINT)strlen(m_ID)+1;
		len+=(S_UINT)strlen(m_Description)+1;

		len+=sizeof(svutil::TTime);

		len+=tlen;
		len+=m_Property.GetRawDataSize();


		len+=tlen;	//m_Edges.size()
		EDGELIST::iterator it;
		bool bnotend=true;
		while(bnotend)
		{
			bnotend=false;
			for(it=m_Edges.begin();it!=m_Edges.end();it++)
			{
				if( (*it)==NULL )
				{
					m_Edges.erase(it);
					bnotend=true;
					break;
				}
			}
		}
		for(it=m_Edges.begin();it!=m_Edges.end();it++)
		{
			if( (*it)==NULL )
				continue;
			len+=tlen;
			len+=(*it)->GetRawDataSize();
		}
	}
	catch(...)
	{
		printf("Exception to TopologyChart::GetRawDataSize \n");
	}
    
	return len;

}
char*	TopologyChart::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;

	try{
		if(bufsize<GetRawDataSize())
			return NULL;
		S_UINT len=0,tlen=sizeof(S_UINT);
		char *pt=lpbuf;

		len=GetRawDataSize();
		memmove(pt,&len,tlen);
		pt+=tlen;

		if(m_ID.empty())
			return NULL;

		strcpy(pt,m_ID);
		pt+=strlen(m_ID)+1;

		strcpy(pt,m_Description);
		pt+=strlen(m_Description)+1;

		memmove(pt,&m_CreateTime,sizeof(svutil::TTime));
		pt+=sizeof(svutil::TTime);

		len=m_Property.GetRawDataSize();
		memmove(pt,&len,tlen);
		pt+=tlen;

		if(m_Property.GetRawData(pt,len)==NULL)
			return NULL;

		pt+=len;


		S_UINT count=(S_UINT)m_Edges.size();
		memmove(pt,&count,tlen);
		pt+=tlen;

		EDGELIST::iterator it;
		for(it=m_Edges.begin();it!=m_Edges.end();it++)
		{
			if( (*it)==NULL )
				continue;
			len= (*it)->GetRawDataSize();
			memmove(pt,&len,tlen);
			pt+=tlen;

			if((*it)->GetRawData(pt,len)==NULL)
				return false;

			pt+=len;
		}
	}catch(...)
	{
		printf("Exception to TopologyChart::GetRawData \n");
		return NULL;
	}

	return lpbuf;
}
BOOL	TopologyChart::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;
		
	S_UINT len=0,tlen=sizeof(S_UINT);

	const char *pend=lpbuf+bufsize;
	const char *pt=lpbuf;

	try{
		memmove(&len,pt,tlen);
		if(len!=bufsize)
			return false;
		pt+=tlen;

        m_ID=pt;
		pt+=strlen(m_ID)+1;

		m_Description=pt;
		pt+=strlen(m_Description)+1;

		memmove(&m_CreateTime,pt,sizeof(svutil::TTime));
		pt+=sizeof(svutil::TTime);

		memmove(&len,pt,tlen);
		pt+=tlen;

		if(m_Property.CreateObjectByRawData(pt,len)==NULL)
			return false;

		pt+=len;


		S_UINT count=0;
		memmove(&count,pt,tlen);
		pt+=tlen;
		
		Edge *pEdge=NULL;
		for(S_UINT i=0;i<count;i++)
		{
			memmove(&len,pt,tlen);
			pt+=tlen;

			pEdge=new Edge();
			if(pEdge==NULL)
				return false;

			if(!pEdge->CreateObjectByRawData(pt,len))
			{
				delete pEdge;
				return false;
			}
			pt+=len;

			m_Edges.push_back(pEdge);
		}


	}catch(...)
	{
		cout<<"Exception to TopologyChart::CreateObjectByRawData"<<endl;
		return false;
	}

	return true;
}

