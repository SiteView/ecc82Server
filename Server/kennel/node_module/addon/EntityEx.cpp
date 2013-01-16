#include "EntityEx.h"

EntityEx::EntityEx(void)
{
}

EntityEx::~EntityEx(void)
{
	ENTITYRELATIONLIST::iterator it;
	for(it=m_Relations.begin();it!=m_Relations.end();it++)
		if( (*it)!=NULL )
			delete (*it);
	PORTMAP::iterator pit;
	while(m_Ports.findnext(pit))
		if( (*pit).getvalue() != NULL )
			delete ((*pit).getvalue());
}

S_UINT	EntityEx::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);
	len+=tlen;  //datalen;

	try{
		len+=(S_UINT)strlen(m_Identifier)+1;
		len+=(S_UINT)strlen(m_Description)+1;
		len+=(S_UINT)strlen(m_Type)+1;
		len+=(S_UINT)strlen(m_EccEntityID)+1;

		len+=tlen;	//m_Property.size();
		len+=m_Property.GetRawDataSize(); 

		len+=tlen;		//m_Relations.size();

		ENTITYRELATIONLIST::iterator it;
		bool bnotend=true;
		while(bnotend)
		{
			bnotend=false;
			for(it=m_Relations.begin();it!=m_Relations.end();it++)
			{
				if( (*it)==NULL )
				{
					m_Relations.erase(it);
					bnotend=true;
					break;
				}
			}
		}
		for(it=m_Relations.begin();it!=m_Relations.end();it++)
		{
			if( (*it)==NULL )
				continue;
			len+=tlen;
			len+=(*it)->GetRawDataSize();
		}

		len+=tlen;	//Ports size;
		PORTMAP::iterator itp;
		bnotend=true;
		while(bnotend)
		{
			bnotend=false;
			itp.clear();
			while(m_Ports.findnext(itp))
			{
				bool todel=false;
				try{
					if( (*itp).getvalue()==NULL )
						todel=true;
					else
						(*itp).getvalue()->GetRawDataSize();
				}
				catch(...)
				{
					printf("Exception to Port: \"%s\" , svapi will abandon it!\n",(*itp).getkey().getword());
					todel=true;
				}
				if(todel)
				{
					m_Ports.erase( (*itp).getkey() );
					bnotend=true;
					break;
				}
			}
		}
		itp.clear();
		while(m_Ports.findnext(itp))
		{
			if((*itp).getvalue() ==NULL)
				continue;
			len+=tlen;
			len+=(*itp).getvalue()->GetRawDataSize();
		}
	}
	catch(...)
	{
		printf("Exception to EntityEx::GetRawDataSize \n");
	}
	return len;
}
char* EntityEx::GetRawData(char *lpbuf,S_UINT bufsize)
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

		if(m_Identifier.empty())
			return NULL;

		strcpy(pt,m_Identifier);
		pt+=strlen(m_Identifier)+1;

		strcpy(pt,m_Description);
		pt+=strlen(m_Description)+1;

		strcpy(pt,m_Type);
		pt+=strlen(m_Type)+1;

		strcpy(pt,m_EccEntityID);
		pt+=strlen(m_EccEntityID)+1;

		len=(S_UINT)m_Property.GetRawDataSize();
		memmove(pt,&len,tlen);
		pt+=tlen;
		if(m_Property.GetRawData(pt,len)==NULL)
			return NULL;
		pt+=len;


		len=(S_UINT)m_Relations.size();
		memmove(pt,&len,tlen);
		pt+=tlen;

		ENTITYRELATIONLIST::iterator it;
		for(it=m_Relations.begin();it!=m_Relations.end();it++)
		{
			if( (*it)==NULL )
				continue;
			len=(*it)->GetRawDataSize();
			memmove(pt,&len,tlen);
			pt+=tlen;

			if((*it)->GetRawData(pt,len)==NULL)
				return NULL;
			pt+=len;
		}

		len=m_Ports.size();
		memmove(pt,&len,tlen);
		pt+=tlen;

		PORTMAP::iterator itp;
		while(m_Ports.findnext(itp))
		{
			if( (*itp).getvalue()==NULL )
				continue;
			len=(*itp).getvalue()->GetRawDataSize();
			memmove(pt,&len,tlen);
			pt+=tlen;

			if((*itp).getvalue()->GetRawData(pt,len)==NULL)
				return NULL;

			pt+=len;
		}

	}catch(...)
	{
		printf("Exception to EntityEx::GetRawData \n");
		return NULL;
	}

	return lpbuf;
}
BOOL EntityEx::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
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

		m_Identifier=pt;
		pt+=strlen(m_Identifier)+1;

		m_Description=pt;
		pt+=strlen(m_Description)+1;

		m_Type=pt;
		pt+=strlen(m_Type)+1;

		m_EccEntityID=pt;
		pt+=strlen(m_EccEntityID)+1;

		memmove(&len,pt,tlen);
		pt+=tlen;
		if(!m_Property.CreateObjectByRawData(pt,len))
			return false;
		pt+=len;

		S_UINT count=0;
		memmove(&count,pt,tlen);
		pt+=tlen;

		EntityRelation *pRelation=NULL;
		S_UINT i=0;

		for(i=0;i<count;i++)
		{
			memmove(&len,pt,tlen);
			pt+=tlen;

			pRelation=new EntityRelation();
			if(pRelation==NULL)
				return false;

			if(!pRelation->CreateObjectByRawData(pt,len))
			{
				delete pRelation;
				return false;
			}

			pt+=len;

			m_Relations.push_back(pRelation);

		}

        memmove(&count,pt,tlen);
		pt+=tlen;

		Port *pPort=NULL;
		for(i=0;i<count;i++)
		{
			memmove(&len,pt,tlen);
			pt+=tlen;

			pPort=new Port();
			if(pPort==NULL)
				return false;

			if(!pPort->CreateObjectByRawData(pt,len))
			{
				delete pPort;
				return false;
			}

			pt+=len;

			word id=pPort->GetID();
			if(id.empty())
				return false;
			m_Ports[id]=pPort;
		}

	}catch(...)
	{
		printf("Exception to EntityEx::CreateObjectByRawData \n");
		return false;
	}

	return true;
}
