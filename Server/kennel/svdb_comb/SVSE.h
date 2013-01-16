
#ifndef	SVDB_SVSE_
#define SVDB_SVSE_

#include "SerialBase.h"
#include "svdbtype.h"
#include "StringMap.h"
#include "util.h"


class SVSE :
	public SerialBase2
{
public:
	SVSE(void);
	~SVSE(void);

	word GetNextSubID()
	{
		char buf[100]={0};
		sprintf(buf,"%d",m_ID);
		return ::MakeIDByIndex(buf,++m_CurrentID);
	}

	word GetIDWord()
	{
		char buf[100]={0};
		sprintf(buf,"%d",m_ID);
		return buf;
	}

	S_UINT GetID()
	{
		return m_ID;
	}
	void PutID(S_UINT id)
	{
		m_ID=id;
	}

	WORDLIST &GetSubGroups()
	{
		return m_SubGroups;
	}

	WORDLIST &GetSubEntitys()
	{
		return m_SubEntitys;
	}

	S_UINT	GetRawDataSize(bool onlyLocked= false);
	char*	GetRawData(char *lpbuf,S_UINT bufsize,bool onlyLocked= false);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	void DisplayAllData()
	{
		puts("*************************************Begin***********************************");
		printf("ID :%d\n",m_ID);
		puts("----------------------------------Label--------------------------------------");
		printf("Label:%s\n",m_Label.getword());
		puts("---------------------------------Current index-------------------------------");
		printf("Current index:%d\n",m_CurrentID);
		puts("---------------------------------Sub groups----------------------------------");
		WORDLIST::iterator it;
		for(it=m_SubGroups.begin();it!=m_SubGroups.end();it++)
			printf("Sub group:%s\n",(*it).getword());
		puts("---------------------------------Sub entitys---------------------------------");
		for(it=m_SubEntitys.begin();it!=m_SubEntitys.end();it++)
			printf("Sub entity:%s\n",(*it).getword());
		puts("------------------------------------End--------------------------------------");

	
	}

	svutil::word GetLabel(void)
	{
		return m_Label;
	}

	void PutLabel(word label)
	{
		m_Label=label;
	}

	bool DeleteSubGroupID(word id)
	{
		WORDLIST::iterator it;
		for(it=m_SubGroups.begin();it!=m_SubGroups.end();it++)
		{
			if((*it)==id)
			{
				m_SubGroups.erase(it);
				return true;
			}
		}
		return false;
	}

	bool DeleteSubEntityID(word id)
	{
		WORDLIST::iterator it;
		for(it=m_SubEntitys.begin();it!=m_SubEntitys.end();it++)
		{
			if((*it)==id)
			{
				m_SubEntitys.erase(it);
				return true;
			}
		}
		return false;
	}




protected:
	S_UINT	m_ID;
	S_UINT	m_CurrentID;
	WORDLIST	m_SubGroups;
	WORDLIST	m_SubEntitys;
	svutil::word	m_Label;
};

#endif
