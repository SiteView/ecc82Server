#ifndef SVDB_GENERALPOOL_
#define SVDB_GENERALPOOL_

#include "PoolBase.h"
#include "SerialBase.h"
#include "svdbtype.h"
#include "Section.h"
#include <cc++/file.h>
#include "libutil/Time.h"

typedef svutil::hashtable<word,Section *> COMMONDATAMAP;

class GeneralPool :
	public PoolBase,public SerialBase
{
public:
	GeneralPool(void);
	GeneralPool(word filepath);
	~GeneralPool(void);

	enum { hashtablesize=127 };

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	bool Load(void);
	bool Submit();

	bool Find(word section)
	{
		Section **ps=m_data.find(section);
		return ps!=NULL;
	}

	bool WriteData(word section,word key,void *data,S_UINT len);
    bool GetData(word section,word key,void *data,S_UINT &len);
    
	bool WriteString(word section, word key, word str);
	word GetString(word section, word key,word defaultret="");

	bool WriteInt(word section,word key,int value);
	int GetInt(word section,word key, int defaultret=0);

	bool GetSectionsName(std::list<string> &sections);
	bool GetKeysName(word section,std::list<string> &keylist);

	int  GetValueTypeBySectionAndKey(word section,word key);

	bool DeleteKey(word section,word key)
	{
		MutexLock lock(m_UpdateLock);
		Section **ps=m_data.find(section);
		if(ps==NULL)
			return false;

		m_changed=true;

		return (*ps)->Delete(key);
		
	}
	bool DeleteSection(word section)
	{
		MutexLock lock(m_UpdateLock);
		Section **ps=m_data.find(section);
		if(ps==NULL)
			return false;
        delete (*ps);
        DeleteSectionInSectionOrder(section);
		m_changed=true;
		return m_data.erase(section);
	}

	Section *GetSection(word section)
	{
		if(!Find(section))
			return NULL;
		m_changed=true;
		return m_data[section];
	}
	bool GetSection(word section,char *buf,S_UINT &len)
	{
		if(!Find(section))
			return false;
		if(buf==NULL)
		{
			len=m_data[section]->GetRawDataSize();
			return true;
		}

		if(m_data[section]->GetRawData(buf,len)==NULL)
			return false;
		return true;
	}
	bool Push(Section *psection)
	{
		MutexLock lock(m_UpdateLock);
		if(psection==NULL)
			return false;

		Section** ps=m_data.find(psection->GetSection());
		if(ps!=NULL)
		{
			delete (*ps);
			//		DeleteSectionInSectionOrder(psection->GetSection());
			m_data[psection->GetSection()]=psection;

		}else
		{
			m_data[psection->GetSection()]=psection;
			m_SectionOrder.push_back(psection->GetSection());
		}
		m_changed=true;
		return true;
	}

	bool EditSectionName(word oldname,word newname)
	{
		if(oldname==newname)
			return true;

		MutexLock lock(m_UpdateLock);
		Section **ps=m_data.find(oldname);
		if(ps==NULL)
			return false;
		if(Find(newname))
			return false;

		(*ps)->PutSection(newname);
		Section *ptem=(*ps);
		m_data.erase(oldname);

		m_data[newname]=ptem;

		ReplaceSectionInSectionOrder(oldname,newname);
		m_changed=true;
		return true;
		
	}

	bool EditKeyName(word section,word oldkey,word newkey)
	{
		if(Find(section))
			return m_data[section]->EditKeyName(oldkey,newkey);
		return false;
	}

	bool Push(const char *buf,S_UINT len)
	{
		MutexLock lock(m_UpdateLock);
		if(buf==NULL)
			return false;
		if(len<=0)
			return false;
		Section *ps=new Section();
		if(ps)
		{
			if(ps->CreateObjectByRawData(buf,len))
			{
				m_changed=true;
				return Push(ps);
			}
		}
		return false;
	}

	bool Push(const char *buf,S_UINT len,string &sname)
	{
		MutexLock lock(m_UpdateLock);
		sname="";

		if(buf==NULL)
			return false;
		if(len<=0)
			return false;
		Section *ps=new Section();
		if(ps)
		{
			if(ps->CreateObjectByRawData(buf,len))
			{
				m_changed=true;
				sname=ps->GetSection().getword();
				return Push(ps);
			}
		}
		return false;

	}

	Mutex	m_UseMutex;
	svutil::TTime		m_LastAccessTime;


protected:
	bool ReplaceSectionInSectionOrder(word oldsection,word newsection)
	{
		WORDLIST::iterator it;
		for(it=m_SectionOrder.begin();it!=m_SectionOrder.end();it++)
		{
			if((*it)==oldsection)
			{
				m_SectionOrder.insert(it,newsection);
				return DeleteSectionInSectionOrder(oldsection);
			}
		}
		return false;

	}
	bool DeleteSectionInSectionOrder(word section)
	{
		WORDLIST::iterator it;
		for(it=m_SectionOrder.begin();it!=m_SectionOrder.end();it++)
		{
			if((*it)==section)
			{
				m_SectionOrder.erase(it);
				return true;
			}
		}
		return false;

	}

	WORDLIST		m_SectionOrder;
	COMMONDATAMAP	m_data;
	S_UINT			m_hashtablesize;

};

#endif
