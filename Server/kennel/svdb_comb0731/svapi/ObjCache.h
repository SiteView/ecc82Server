
#ifndef		SVDB_OBJECTLOCALCACHE_H_
#define		SVDB_OBJECTLOCALCACHE_H_

#include <cc++/thread.h>

#include <StringMap.h>
#include "svdbtype.h"
#include "svapi.h"
#include "svdbapi.h"
#include <string>
#include <map>
using std::map;
using std::string;
using ost::Mutex;

class VerOBJECT
{
public:
	VerOBJECT():ver(""),obj(INVALID_VALUE){}
	~VerOBJECT(){}

	string ver;
	OBJECT obj;
};

class ObjectCache
{
	ObjectCache(void){};
	~ObjectCache(void){};

public:
	static OBJECT FastGetMonitorTPL(int id);
	static OBJECT FastGetEntityTPL(string id);
	static OBJECT FastGetEntityGroup(string id);

	static OBJECT FastLoadResource(string lan);
	static OBJECT FastLoadResourceByKeys(string keys,string lan);


	static OBJECT FastGetSVSE(string id);
	static OBJECT FastGetMonitor(string id);
	static OBJECT FastGetGroup(string id);
	static OBJECT FastGetEntity(string id);

	static bool RefreshMonitors(string pid);
	static bool RefreshEntities(string pid);
	static OBJECT RefreshFastGetMonitor(string id);
	static OBJECT RefreshFastGetEntity(string id);

	static bool RefreshLatestRecords(string pid, ForestMap & fmap, int command, bool need);
	static bool FastGetSVDYN(string id, SVDYN &dyn);
	static bool RefreshSVDYNs(string pid);
	static bool QuerySVDYNs( string pid, std::list<SingelRecord> & listrcd, std::list<SingelRecord> & listrcd_out );
	static bool RefreshMonitors_new(string pid);
	static bool QueryMassMonitor( string pid, StringMap & smap, std::list<SingelRecord> & listrcd_out );
	static bool RefreshEntities_new(string pid);
	static bool QueryMassEntity( string pid, StringMap & smap, std::list<SingelRecord> & listrcd_out );

	static void SetUserAddr(string user,string addr);

private:
	static bool SetLatestRecords(std::list<SingelRecord> & listrcdin, ForestMap & fmap, int command);

	static bool CheckLatest(VerOBJECT & vobj, int dtype, string id);

	static string svdbuser;
	static string svdbaddr;

	static map<int, OBJECT> MonitorTPL;
	static map<string, OBJECT> EntityTPL;
	static map<string, OBJECT> EntityGroup;

	static map<string, OBJECT> Res;
	static map<string,map<string, OBJECT> > Reskey;


	static map<string, VerOBJECT> Svses;
	static map<string, VerOBJECT> Monitors;
	static map<string, VerOBJECT> Entities;
	static map<string, VerOBJECT> Groups;

	static map<string, SingelRecord> Dyns;


	static Mutex m_LockMonitorTPL;
	static Mutex m_LockEntityTPL;
	static Mutex m_LockEntityGroup;
	static Mutex m_LockRes;
	static Mutex m_LockReskey;
	static Mutex m_LockSvses;
	static Mutex m_LockMonitors;
	static Mutex m_LockEntities;
	static Mutex m_LockGroups;
	static Mutex m_LockDyns;

};

OBJECT	GetSVSEWithVer(string id,string user,string addr,S_UINT & ver);
OBJECT	GetGroupWithVer(string groupid,string user,string addr,S_UINT & ver);
OBJECT	GetEntityWithVer(string entityid,string user,string addr,S_UINT & ver);
OBJECT	GetMonitorWithVer(string monitorid,string user,string addr,S_UINT & ver);

#endif