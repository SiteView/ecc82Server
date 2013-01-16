#include "MakeRecord.h"

MakeRecord::MakeRecord(void)
{
	m_buf=NULL;
	m_currentlen=0;
	m_buflen=0;

}

MakeRecord::~MakeRecord(void)
{
}

MakeRecord::MakeRecord(char *buf,int buflen,int state)
{
	m_buf=buf;
	m_buflen=buflen;


	((RecordHead *)m_buf)->state=state;
	((RecordHead *)m_buf)->createtime=TTime::GetCurrentTimeEx();

	m_currentlen=sizeof(RecordHead);

}
bool MakeRecord::SetData(char *buf,int buflen,int state)
{
	if((m_buf==NULL)||(buflen<sizeof(RecordHead)))
			return false;
	m_buf=buf;
	m_buflen=buflen;


	((RecordHead *)m_buf)->state=state;
	((RecordHead *)m_buf)->createtime=TTime::GetCurrentTimeEx();

	m_currentlen=sizeof(RecordHead);

	return true;

}
MakeRecord::MakeRecord(char *buf,int buflen,int state,svutil::TTime tm)
{

	m_buf=buf;
	m_buflen=buflen;

	((RecordHead *)m_buf)->state=state;
	((RecordHead *)m_buf)->createtime=tm;

	m_currentlen=sizeof(RecordHead);

}

bool MakeRecord::MakeBuf(string type,int iv,float fv,string sv)
{
	if(m_buf==NULL)
		return false;
	char *pt=m_buf+m_currentlen;

	if(stricmp(type.c_str(),"Int")==0)
	{
		if((pt=this->buildbuffer(iv,pt,m_buflen-m_currentlen))==NULL)
			return false;
	}else if(stricmp(type.c_str(),"Float")==0)
	{
		if((pt=this->buildbuffer(fv,pt,m_buflen-m_currentlen))==NULL)
			return false;
	}else if(stricmp(type.c_str(),"String")==0)
	{
		if((pt=this->buildbuffer(sv,pt,m_buflen-m_currentlen))==NULL)
			return false;
	}else
		return false;

	m_currentlen=pt-m_buf;
	((RecordHead *)m_buf)->datalen=m_currentlen-sizeof(RecordHead);

	return true;

}
bool MakeRecord::MakeBuf(ReturnValue *prv)
{
	if(m_buf==NULL)
		return false;
	char *pt=m_buf+m_currentlen;

	if(stricmp(prv->m_pReturnData->m_Type,"Int")==0)
	{
//		printf("Int value:%d\n",prv->m_value.nVal);
		if((pt=this->buildbuffer(prv->m_value.nVal,pt,m_buflen-m_currentlen))==NULL)
			return false;
	}else if(stricmp(prv->m_pReturnData->m_Type,"Float")==0)
	{
//		printf("Float value:%0.2f\n",prv->m_value.fVal);
		if((pt=this->buildbuffer(prv->m_value.fVal,pt,m_buflen-m_currentlen))==NULL)
			return false;
	}else if(stricmp(prv->m_pReturnData->m_Type,"String")==0)
	{
//		printf("String value:%s\n",prv->m_value.szVal);
		string str=prv->m_value.szVal;
		if((pt=this->buildbuffer(str,pt,m_buflen-m_currentlen))==NULL)
			return false;
	}else
		return false;

	m_currentlen=pt-m_buf;
	((RecordHead *)m_buf)->datalen=m_currentlen-sizeof(RecordHead);


	return true;


}
bool MakeRecord::MakeError(int state,string error)
{

//	printf("buflen:%d,prv type:%s\n",m_buflen,error.c_str());

	if(m_buf==NULL)
		return false;
	((RecordHead *)m_buf)->state=state;
	char *pt=m_buf+m_currentlen;
	if((pt=this->buildbuffer(error,pt,m_buflen-m_currentlen))==NULL)
		return false;
	m_currentlen=pt-m_buf;
	((RecordHead *)m_buf)->datalen=m_currentlen-sizeof(RecordHead);

	return true;

}

char *MakeRecord::buildbuffer(int data,char *pt,int buflen)
{
	if(pt==NULL)
		return NULL;
	if(buflen<sizeof(int))
		return NULL;
	memmove(pt,&data,sizeof(int));
	pt+=sizeof(int);
	return pt;
}
char *MakeRecord::buildbuffer(float data,char *pt,int buflen)
{
	if(pt==NULL)
		return NULL;
	if(buflen<sizeof(float))
		return NULL;
	memmove(pt,&data,sizeof(float));
	pt+=sizeof(float);

	return pt;

}

char *MakeRecord::buildbuffer(std::string data,char *pt,int buflen)
{
	if(pt==NULL)
		return NULL;
	if(buflen<data.size()+1)
		return NULL;
	strcpy(pt,data.c_str());
	pt+=data.size();
	pt[0]='\0';
	pt++;
	return pt;

}
