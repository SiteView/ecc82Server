#include "Section.h"

Section::Section(void)
{
	m_hashtablesize=hashtablesize;
	m_data.resetsize(hashtablesize);
}

Section::~Section(void)
{
	SECTIONDATA::iterator it;
	while(m_data.findnext(it))
	{
		CommonData *pd=(*it).getvalue();
		if(pd)
	    	delete pd;
	}
	m_data.clear();
	m_KeyOrder.clear();
}

S_UINT	CommonData::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);

	len+=sizeof(int);	//m_DataType;
	len+=tlen;			//m_DataLen;
	len+=m_DataLen;		//m_data;

	return len;
}
char*	CommonData::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;
	if(bufsize<GetRawDataSize())
		return NULL;
	S_UINT len=0,tlen=sizeof(S_UINT);	
	char *pt=lpbuf;

	memmove(pt,&m_DataType,sizeof(int));
	pt+=sizeof(int);

	memmove(pt,&m_DataLen,tlen);
	pt+=tlen;

	memmove(pt,m_data,m_DataLen);
	pt+=m_DataLen;

	return lpbuf;
}
BOOL	CommonData::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return false;

	S_UINT len=0,tlen=sizeof(S_UINT);

	try{

		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;

		memmove(&m_DataType,pt,sizeof(int));
		pt+=sizeof(int);
		if(pt>pend)
			return false;

		memmove(&m_DataLen,pt,tlen);
		pt+=tlen;
		if(pt>pend)
			return false;
		if(!PutData(pt,m_DataLen,m_DataType))
			return false;
		pt+=m_DataLen;
		if(pt>pend)
			return false;

	}catch(...)
	{
		return false;
	}


	return true;
}

S_UINT	Section::GetRawDataSize(void)
{
	S_UINT len=0,tlen=sizeof(S_UINT);

	len+=tlen;		//m_tablesize;
	len+=strlen(m_SectionName)+1;
	len+=tlen;		//data count;

	WORDLIST::iterator wit;
	CommonData **pdata=NULL;
	for(wit=m_KeyOrder.begin();wit!=m_KeyOrder.end();wit++)
	{
		pdata=m_data.find((*wit));
		if(pdata!=NULL)
		{
			len+=strlen((*wit).getword())+1;
			len+=tlen;   //data len;
			len+=(*pdata)->GetRawDataSize();

		}
	}


/*	SECTIONDATA::iterator it;
	while(m_data.findnext(it))
	{
		len+=strlen((*it).getkey().getword())+1;
		len+=tlen;   //data len;
		len+=(*it).getvalue()->GetRawDataSize();
	}
*/
	return len;
}
char*	Section::GetRawData(char *lpbuf,S_UINT bufsize)
{
	if(lpbuf==NULL)
		return NULL;
	if(bufsize<GetRawDataSize())
		return NULL;
	S_UINT len=0,tlen=sizeof(S_UINT);	
	char *pt=lpbuf;
	memmove(pt,&m_hashtablesize,tlen);
	pt+=tlen;
	strcpy(pt,m_SectionName);
	pt+=strlen(m_SectionName);
	pt[0]='\0';
	pt++;

//	len=m_data.size();
	len=m_KeyOrder.size();
	memmove(pt,&len,tlen);
	pt+=tlen;

	WORDLIST::iterator wit;
	CommonData **pdata=NULL;
	for(wit=m_KeyOrder.begin();wit!=m_KeyOrder.end();wit++)
	{
		pdata=m_data.find((*wit));
		if(pdata!=NULL)
		{
			strcpy(pt,(*wit).getword());
			pt+=strlen((*wit).getword());
			pt[0]='\0';
			pt++;

			len=(*pdata)->GetRawDataSize();
			memmove(pt,&len,tlen);
			pt+=tlen;
			if(!(*pdata)->GetRawData(pt,len))
				return false;
			pt+=len;
		}
	}

/*
    SECTIONDATA::iterator it;
	while(m_data.findnext(it))
	{
		strcpy(pt,(*it).getkey().getword());
		pt+=strlen((*it).getkey().getword());
		pt[0]='\0';
		pt++;

		len=(*it).getvalue()->GetRawDataSize();
		memmove(pt,&len,tlen);
		pt+=tlen;
		if(!(*it).getvalue()->GetRawData(pt,len))
			return false;
		pt+=len;
	}
*/

	return lpbuf;

}
BOOL	Section::CreateObjectByRawData(const char *lpbuf,S_UINT bufsize)
{

	if(lpbuf==NULL)
		return false;

	S_UINT len=0,tlen=sizeof(S_UINT);

	try{

		const char *pend=lpbuf+bufsize;
		const char *pt=lpbuf;

		memmove(&m_hashtablesize,pt,tlen);
		m_data.resetsize(m_hashtablesize);
		pt+=tlen;
		if(pt>pend)
			return false;
        
		m_SectionName=pt;
		pt+=strlen(m_SectionName)+1;
		if(pt>pend)
			return false;

		S_UINT count=0;

		memmove(&count,pt,tlen);
		pt+=tlen;
		word name="";
		CommonData *pdata=NULL;
		for(S_UINT i=0;i<count;i++)
		{
			name=pt;
			pt+=strlen(name)+1;
			if(pt>pend)
				return false;

			memmove(&len,pt,tlen);
			pt+=tlen;
			if(pt>pend)
				return false;

			pdata=new CommonData();
			if(!pdata)
				return false;

			if(!pdata->CreateObjectByRawData(pt,len))
				return false;
			m_data.insert(name,pdata);
			m_KeyOrder.push_back(name);
			pt+=len;
			if(pt>pend)
				return false;
		}

	}catch(...)
	{
		return false;
	}

	return true;

}


