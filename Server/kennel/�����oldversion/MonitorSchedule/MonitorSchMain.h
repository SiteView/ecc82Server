#ifndef  DRAGONFLOW_MONITORSCHMAIN
#define DRAGONFLOW_MONITORSCHMAIN

#include "MonitorSchedule.h"
#include "Monitor.h"
#include "LoadConfig.h"
#include "ommunicationThread.h"
#include "Task.h"
#include "Option.h"
#include "appendmassrecord.h"

class ScheduleThread;

class MonitorSchMain
{
public:
	void UpdateDataByFile(LPCSTR FileTitle);
	Monitor* GetMonitorByID(const char *MonitorID);
	bool DeleteMonitorV70(string monitorid);
	Groups *GetGroups(void)
	{
		return m_pGroups;
	}
	BOOL CheckGroupDependState(Monitor *pMonitor,CString &strDependID);
	bool CheckTask(Monitor *pMonitor,CTime ct,bool &isabs);
	CMonitorList & GetMonitorList(void);
	Subsequent* GetSubsequtent(void)
	{
		return m_pSub;
	}
	void Run(void);
	BOOL Init(void);
	MonitorSchMain();
	virtual ~MonitorSchMain();

	bool ProcessConfigChange(string opt,string id);
	bool SerialMonitorData(bool out);

	ost::Mutex	m_MonitorListMutex;
	ost::Mutex	m_DemoDllMutex;
	Option *m_pOption;

	bool m_bDataChange;

private:
	LoadConfig m_lc;
protected:

	bool AddEntityV70(string entityid,bool isEdit);
	bool AddGroupV70(string groupid,bool isEdit);
	bool ReLoadMonitorsV70(string parentid);
	bool ReLoadTaskV70();
	bool InitMonitorsGroupDependV70(string parentid);
	bool InitGroupDependByMonitorChangeV70(Monitor *oldpm,Monitor* newpm,bool isdel);
	bool EntityChangeV70(Entity *oldE,Entity *newE,bool isdel);

	bool AddMonitorV70(string monitorid,bool isEdit);
	static bool ParseOpt(string opt,string &name,string &type);
	void DeleteMonitor(LPCSTR FileName);
	BOOL AddMonitor(LPCSTR FileName,BOOL isEdit);
	void AddEntity(LPCSTR FileName,BOOL isEdit);
	void AddGroup(LPCSTR FileName,BOOL isEdit);
	CommunicationThread * m_pCommThread;
	int GetMonitorLastState(const char *MonitorID);
	ScheduleThread * m_ScheduleThread;
	Subsequent * m_pSub;
	Groups * m_pGroups;
	CMonitorList m_MonitorList;

	CAppendMassRecord * m_pAppendThread;

	TASKMAP	m_TaskMap;
	ost::Mutex	m_TaskMutex;

public:
	bool LoadPreLibrary(void);
};

#endif
