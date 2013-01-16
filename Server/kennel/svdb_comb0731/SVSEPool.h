#ifndef	SVDB_SVSEPOOL_
#define	SVDB_SVSEPOOL_

#include "PoolBase.h"
#include "SerialBase.h"
#include "svdbtype.h"
#include "SVSE.h"
#include <cc++/file.h>
#include <vector>


class SVSEPool :
	public PoolBase2,public SerialBase
{
public:
	SVSEPool(void);
	~SVSEPool(void);

	SVSEPool(word filepath):PoolBase2(filepath)
	{
		m_data.resize(initsize);
		m_count=initsize;
		m_loaded=false;
		m_changed=false;
		m_CurrentID=1;

		for(int i=0;i<initsize;i++)
			m_data[i]=NULL;
	}

	enum{ initsize=30, maxsize=100};

	word GetNextSubID(S_UINT seid=1)
	{
		MutexLock lock(m_UpdateLock);
		if(m_data[seid]==NULL)
		  return "";
		m_changed=true;
		return m_data[seid]->GetNextSubID();

	}

	bool Find(S_UINT id)
	{
		if((id>=m_count)||(id<1))
			return false;

		if(m_data[id]==NULL)
			return false;
		return true;

	}

	SVSE *GetSVSE(S_UINT id)
	{
		if(!Find(id))
			return NULL;
		m_changed=true;
		return m_data[id];

	}
	S_UINT GetNextSEID(void)
	{
		ost::MutexLock lock(m_UpdateLock);
		return ++m_CurrentID;

	}


	bool push(SVSE *psvse)
	{
		ost::MutexLock lock(m_UpdateLock);
		if(psvse==NULL)
			return false;
		int n=psvse->GetID();
		if((n<=0)||(n>maxsize))
			return false;
		if(n>=m_count)
		{
			m_count=(m_count==n) ? m_count+1 : n;
			m_data.resize(m_count,NULL);
		}
		if(m_data[n]!=NULL)
			delete m_data[n];
		m_data[n]=psvse;
		NewVersion(n);
		cout<<"Latest svse "<<n<<" version is: "<<GetVersion(n)<<endl;

		m_changed=true;
		return true;

	}

	bool PushData(const char *buf,S_UINT len)
	{
		ost::MutexLock lock(m_UpdateLock);

		SVSE *ps=new SVSE();
		if(ps==NULL)
			return false;
		if(!ps->CreateObjectByRawData(buf,len))
			return false;

		return push(ps);

	}

	bool GetSVSEData(S_UINT id,char *buf,S_UINT &len);

	bool DeleteSVSED(S_UINT id)
	{
		if(!Find(id))
			return false;
		if(m_data[id]!=NULL)
		{
			delete m_data[id];
			m_data[id]=NULL;
			m_changed=true;
		}
		m_mversion_int.erase(id);
		return true;
	}

	bool PutSubGroup(S_UINT seid,word subid)
	{
		MutexLock lock(m_UpdateLock);

		if(Find(seid))
		{
			NewVersion(seid);
			m_data[seid]->GetSubGroups().push_back(subid);
			m_changed=true;
			return true;

		}
		return false;


	}

	bool PutSubEntity(S_UINT seid,word entityid)
	{
		MutexLock lock(m_UpdateLock);

		if(Find(seid))
		{
			NewVersion(seid);
			m_data[seid]->GetSubEntitys().push_back(entityid);
			m_changed=true;
			return true;

		}
		return false;
	}
    
	bool InitSVSEPool()
	{
		if(m_data[1]!=NULL)
			return false;
		SVSE *pse=new SVSE();
		if(!pse)
			return false;
		pse->PutID(1);
		pse->PutLabel("localhost");
		m_data[1]=pse;
		m_changed=true;
		Submit();
		return true;

	}

	bool Load(void);
	bool Submit(std::string modifyid="");

	S_UINT	GetRawDataSize(void);
	char*	GetRawData(char *lpbuf,S_UINT bufsize);
	BOOL	CreateObjectByRawData(const char *lpbuf,S_UINT bufsize);

	bool GetInfo(word infoname,StringMap &map)
	{
		bool getver(false);
		string infomation(infoname.getword());
		if(infomation.compare("ObjectVersion")==0)
			getver=true;

		char id[100]={0};
		for(int i=0;i<m_count;i++)
		{
			if(m_data[i]!=NULL)
			{
				sprintf(id,"%d",m_data[i]->GetID());
				if(getver)
				{
					char charver[256];
					sprintf(charver,"%d",GetVersion(i));
					map[id]=charver;
					continue;
				}

				map[id]=m_data[i]->GetLabel();
			}
		}


		return true;

	}

	bool DeleteSubEntityID(word id)
	{
		MutexLock lock(m_UpdateLock);
		if(!Find(atoi(::GetParentID(id))))
			return false;

		NewVersion(atoi(GetParentID(id)));
		if(m_data[atoi(GetParentID(id))]->DeleteSubEntityID(id))
			m_changed=true;
		else
			return false;
		return true;
	}
	bool DeleteSubGroupID(word id)
	{
		MutexLock lock(m_UpdateLock);
		if(!Find(atoi(GetParentID(id))))
			return false;

		NewVersion(atoi(GetParentID(id)));
		if(m_data[atoi(GetParentID(id))]->DeleteSubGroupID(id))
			m_changed=true;
		else
			return false;
		return true;
	}

	void DisplayAllData(void)
	{
		puts("************************************Begin********************************");
		printf("Sapce size :%d\n",m_count);
		puts("--------------------------------------------------------------------------");
		for(int i=0;i<m_count;i++)
		{
			if(m_data[i]!=NULL)
				m_data[i]->DisplayAllData();
		}

	}

	S_UINT GetCurrentID()
	{
		return m_CurrentID;
	}




protected:
	std::vector<SVSE *> m_data;
	S_UINT	m_count;
	S_UINT	m_CurrentID;

};

#endif
