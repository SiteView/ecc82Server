//#pragma once
#ifndef	SVDB_SVDBMAIN_
#define SVDB_SVDBMAIN_

#include "svdbtype.h"
#include "MonitorTempletPool.h"
#include "MonitorPool.h"
#include "SVSEPool.h"
#include "ResourcePool.h"
#include "GroupPool.h"
#include "EntityPool.h"
#include "GeneralPool.h"
#include "EntityGroupPool.h"
#include "EntityTempletPool.h"
#include "EntityPoolEx.h"
#include "TopologyChartPool.h"
#include "VirtualGroupPool.h"

#include "Option.h"


class ThreadContrl;
class DB;
class WatchThread;
class SocketWatchThread;

typedef svutil::hashtable<svutil::word,GeneralPool * >	INIFILEPOOL;

#include <map>

class SvdbMain
{
public:
	SvdbMain(void);
	~SvdbMain(void);
	bool	Init(void);
	bool	Start(void);
	bool	Quit(void);

	DB *GetDB()
	{
		return m_pDB;
	}

	void toExitListen(void);

	MonitorPool			*m_pMonitor;
	ResourcePool		*m_pResource;
	EntityPool			*m_pEntity;
	GroupPool			*m_pGroup;
	SVSEPool			*m_pSVSE;
	GeneralPool			*m_pTask;
	MonitorTempletPool	*m_pMT;
    EntityGroupPool		*m_pEntityGroup;
	EntityTempletPool	*m_pEntityTemplet;

	MonitorTempletPool	*m_pMT_orig;
    EntityGroupPool		*m_pEntityGroup_orig;
	EntityTempletPool	*m_pEntityTemplet_orig;
	Resource			*m_pLanguage;

	EntityPoolEx		*m_pEntityEx;
	TopologyChartPool	*m_pTopologyChart;
	VirtualGroupPool	*m_pVirtualGroup;

	INIFILEPOOL			m_IniPool;

	DB	*m_pDB;
	std::map<string,DB *> m_pDB_map;//读取是线程安全的,写入是未定义的
	void InsertIntoStdMap(string id, DB * tempDB)//写入要用该函数
	{
		ost::MutexLock lock(m_UpdateLockStdMap);
		m_pDB_map.insert(make_pair(id.c_str(), tempDB));
	}

	Option				*m_pOption;

protected:
	svutil::word	m_RootPath;
	ThreadContrl *m_pTC;
	WatchThread *m_pWT;
	SocketWatchThread *m_pSWT;

	Mutex	m_UpdateLockStdMap;

};

#endif
