//#pragma once

#ifndef		DRAGONFLOW_MONITOR
#define		DRAGONFLOW_MONITOR

#include "MonitorSchedule.h"

#include "ReturnData.h"	
#include "StateCondition.h"
#include "SubsequentItem.h"
#include <svdbapi.h>
#include "Task.h"

class Entity;

class Monitor
{
public:
	bool CalculateNextRunTime(void);
	bool CalculateNextRunTime(CTime time);
	void CalculateErrorFrequency(bool ise);
	BOOL SetParameterList(CStringList&lstParameter);
	BOOL SetReturnList(CReturnDataList &lst);
	CTime m_LastRunTime;
	Monitor();
	virtual ~Monitor();

	enum MonitorState{
	STATUS_OK		= 1,
	STATUS_WARNING	= 2,
	STATUS_ERROR	= 3,
	STATUS_DISABLE	= 4,
	STATUS_BAD		= 5,
	STATUS_NULL=0
	};

	enum { 
		monitoridlen=100,
		defaultfrequency=5,
		entitytypelen=100
	};

	enum EntityType{
		windows=0,
		unix=1,
		other=3
	};

	enum {
		subsequentnull=0,
		subsequentyes=1,
		subsequentno=2
	};

	enum {
		groupdependnull=0,
		groupdependyes=1,
		groupdependno=2
	};


	BOOL isDelete;

	int GetLastState(void){
		if(m_LastState==STATUS_NULL)
		{
			SVDYN dyn;
			if(::GetSVDYNNODisplayString(m_MonitorID,dyn))
			{
				m_LastState=dyn.m_state;
				//char buf[100]={0};
				//sprintf(buf,"last state:%d",m_LastState);
				//Util::ErrorLog(buf);
				
			}
		}

		return m_LastState; 
	}
	void SetLastState( int nState)
	{
		m_LastState=nState;
	}

	void SetMonitorType(int type){ m_MonitorType=type; }
	void SetLibrary(CString strLibrary){ m_Library=strLibrary;}
	void SetLibrary(const char *pLibrary){
		if(pLibrary)
			m_Library=pLibrary;
	}
	void SetProcess(CString strProcess){ m_Process=strProcess;}
	void SetProcess(const char *pProcess){ m_Process=pProcess;}
	void SetMonitorID(const char *pMonitorID)
	{
		if(pMonitorID)
			strcpy(m_MonitorID,pMonitorID);
	}

/*	void SetParentID(const char * pParentID)
	{
		if(pParentID)
			strcpy(m_ParentID,pParentID);
	}
	void SetSEID(const char *pSEID)
	{
		if(pSEID)
			strcpy(m_SEID,pSEID);
	}*/

	void SetRunning(BOOL is){ m_Running=is; }
	BOOL GetRunning(void){ return m_Running; }
	void SetFrequency(int frequency)
	{
		if(frequency>0)
			m_Frequency=frequency;
		else
			m_Frequency=defaultfrequency;
	}

	void SetMonitorClass(const char * pMonitorClass)
	{
		if(pMonitorClass)
			m_MonitorClass=pMonitorClass;
	}
	void SetEntityType(const char *pEntityType)
	{
		if(pEntityType)
			strcpy(m_EntityType,pEntityType);
	}

	void SetSkipCount(int count)
	{
		if(count>=0)
			m_nSkipCount=count;
	}

	int GetSkipCount(void)
	{
		return m_nSkipCount;
	}

	void SetNextRunTime(CTime time)
	{
		m_NextRunTime=time;
	}
	CTime GetNextRunTime(void)
	{
		return m_NextRunTime;
	}

	void SetDisable(bool is)
	{
		m_isDisable=is;
	}
	bool GetDisable(void)
	{
		return m_isDisable;
	}

	int GetMonitorType(void){ return m_MonitorType;}
	int GetFrequency(void){ return m_Frequency;}
	CString GetMonitorClass(void)
	{
		return m_MonitorClass;
	}

	const char* GetMonitorID() const
	{
		return m_MonitorID;
	}
	string GetParentID() const
	{
		return ::FindParentID(m_MonitorID);
	}

	string GetSEID() const
	{
		return ::FindSEID(m_MonitorID);
	}

	const char* GetEntityType() const
	{
		return m_EntityType;
	}

	CString GetLibrary()
	{
		return m_Library;
	}
	CString GetProcess()
	{
		return m_Process;
	}

	CStringList& GetParameterList(void)
	{
		return m_ParamList;
	}

	CReturnDataList& GetReutrnList(void)
	{
		return m_ReturnList;
	}
	StateCondition **GetStateCondition(void)
	{
		return m_StateConditions;
	}
	void SetTempDisable(bool is)
	{
		m_isTempDisable=is;
	}
	bool GetTempDisable()
	{
		return m_isTempDisable;
	}
	bool GetTempDisableStateByTime(CTime time)
	{
		if(m_isTempDisable)
		{
//			printf("TempDisable btime:%s-etime:%s\n",m_beginTDTime.Format().c_str(),m_endTDTime.Format().c_str());
			if((time>=m_beginTDTime)&&(time<=m_endTDTime))
				return true;
		}

		return false;

	}
	void SetBeginTDTime(CTime time)
	{
		m_beginTDTime=time;
	}
	CTime GetBeginTDTime()
	{
		return m_beginTDTime;
	}
	void SetEndTDTime(CTime time)
	{
		m_endTDTime=time;
	}
	CTime GetEndTDTime()
	{
		return m_endTDTime;
	}

	void SetQueueIndex(int index)
	{
		m_nQueueIndex=index;
	}
	int GetQueueIndex(void)
	{
		return m_nQueueIndex;
	}

	void SetSubsequentItem(SubsequentItem *pItem)
	{
		m_pSubsequentItem=pItem;
	}

	SubsequentItem * GetSubsequentItem(void)
	{
		return m_pSubsequentItem;
	}

	void SetSubsequent(int is)
	{
		m_isSubsequent=is;
	}

	int GetSubsequent(void)
	{
		return m_isSubsequent;
	}
	void SetEntity(Entity*pEntity)
	{
		m_pEntity=pEntity;
	}

	Entity * GetEntity(void)
	{
		return m_pEntity;
	}

	void SetGroupDependState(int is)
	{
		m_isGroupDepend=is;
	}
	inline int GetGroupDependState(void)
	{
		return m_isGroupDepend;
	}

	int GetGroupDependCondition(void)
	{
		return m_GroupDependCondition;
	}

	void SetGroupDependCondition(int GroupDependCondition)
	{
		m_GroupDependCondition=GroupDependCondition;
	}

	Monitor *GetGroupDependMonitor(void)
	{
		return m_GroupDependMonitor;
	}

	void SetGroupDependMonitor(Monitor *pMonitor)
	{
		m_GroupDependMonitor=pMonitor;
	}

	void SetTaskName(string taskname)
	{
		m_TaskName=taskname;
	}
	string GetTaskName()
	{
		return m_TaskName;
	}

	int GetTaskType()
	{
		return m_TaskType;
	}
	void SetTaskType(int type)
	{
		m_TaskType=type;
	}

	void SetCheckError(bool isc)
	{
		m_CheckError=isc;

	}

	bool GetCheckError(void)
	{
		return m_CheckError;
	}

	void SetErrorFrequency(int ef)
	{
		m_ErrorFreq=ef;
	}

	int GetErrorFrequency(void)
	{
		return m_ErrorFreq;
	}

	int ToTextMap(STRINGMAP &strmap);
	int FromTextMap(STRINGMAP &strmap);
    
	//sxf baseline 功能 2008-12-8
	bool IsUseBaseline()
	{
		return m_bBaseLine;
	};

	void SetUserBaseline(bool flag)
	{
		m_bBaseLine = flag;
	}
	//

	bool m_isRefresh;
	bool m_isInQueue;
	bool m_isRunInProcess;

protected:
	int m_TaskType;
	int m_LastState;
	CTime m_beginTDTime;
	CTime m_endTDTime;
	bool m_isTempDisable;
	bool m_isDisable;
	CTime m_NextRunTime;

	StateCondition *m_StateConditions[3];
	CReturnDataList m_ReturnList;
	CStringList m_ParamList;
	int m_nSkipCount;

	int m_MonitorType;
	CString m_Library;
	CString m_Process;
	char m_MonitorID[monitoridlen];
//	char m_ParentID[monitoridlen];
//	char m_SEID[monitoridlen];
	char m_EntityType[entitytypelen];
	BOOL	m_Running;
	int		m_Frequency;
	int		m_ErrorFreq;
	bool	m_CheckError;
	CString m_MonitorClass;

	string m_TaskName;

	//sxf baseline 功能 2008-12-8
	bool	m_bBaseLine;
	//

private:
	Entity * m_pEntity;
	int m_isSubsequent;
	SubsequentItem * m_pSubsequentItem;
	int m_nQueueIndex;
	int	m_isGroupDepend;
	int m_GroupDependCondition;
	Monitor *m_GroupDependMonitor;
};

typedef std::list<Monitor *>  CMonitorList;

#endif
