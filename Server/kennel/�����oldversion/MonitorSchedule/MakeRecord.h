#ifndef		DRAGONFLOW_MAKERECORD
#define		DRAGONFLOW_MAKERECORD
#include <iostream>
#include <RecordType.h>
#include "ReturnValue.h"


using namespace	std;

class MakeRecord
{
public:
	MakeRecord(void);
	~MakeRecord(void);
	MakeRecord(char *buf,int buflen,int state);
	MakeRecord(char *buf,int buflen,int state,svutil::TTime tm);
	void SetBuf(char *buf,int buflen)
	{
		if((m_buf==NULL)||(m_buflen<sizeof(RecordHead)))
			return ;

		m_buf=buf;
		m_buflen=buflen;
		
	}
	void SetState(int state)
	{
		if((m_buf==NULL)||(m_buflen<sizeof(RecordHead)))
			return;
		((RecordHead *)m_buf)->state=state;
	}
	void SetTime(svutil::TTime tm)
	{
		if((m_buf==NULL)||(m_buflen<sizeof(RecordHead)))
			return;
		((RecordHead *)m_buf)->createtime=tm;
	}
	bool SetData(char *buf,int buflen,int state);
    
	bool MakeBuf(string type,int iv,float fv,string sv);
	bool MakeError(int state,string error);
	bool MakeBuf(ReturnValue *prv);

	int GetDataSize()
	{
		return m_currentlen;
	}

private:

	char *buildbuffer(int data,char *pt,int buflen);

	char *buildbuffer(float data,char *pt,int buflen);

	char *buildbuffer(std::string data,char *pt,int buflen);

private:
	char *m_buf;
	int	m_currentlen;
	int m_buflen;

};

#endif