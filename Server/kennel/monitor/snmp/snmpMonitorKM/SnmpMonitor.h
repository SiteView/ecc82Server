#include <string>
#include <iostream>
#include <list>
#include <map>

using namespace std;

class clsOID;

typedef std::list<string> StringList;
/////////////////////////////////////////////////////////////////////////////
// CSnmpMonitorApp
// See SnmpMonitor.cpp for the implementation of this class
//
//typedef list<string *> CStringList;

class CSnmpMonitorApp
{
public:
	CSnmpMonitorApp();

	virtual bool InitInstance();
};

/////////////////////////////////////////////////////////////////////////////
//extern "C" __declspec(dllexport)  int SNMPList(StringList &paramList, StringList &lstTexts,
//              StringList &lstValues);
extern "C" __declspec(dllexport)  int SNMPList(const char * strParas, char * szReturn, int& nSize);

void MakeLabel(StringList &lsTemp, StringList &lsLable, char* strFixLable ,int nSelSub = 0, int nSubSize = 0);

extern "C" __declspec(dllexport)  int SNMPDone(const char * strParas, char * szReturn, int& nSize);

extern "C" __declspec(dllexport)  int NetworkSetTest(const char *inFileName,const char *outFileName);

#define		__COMMUNITY__					"_Community="
#define		__SERVERPORT__					"_ServerPort="
#define		__INTERFACEINDEX__				"_InterfaceIndex="
#define     __SELVALUE__					"_SelValue="
#define		__GROUPID__						"_GroupID="  
#define		__MONITORID__					"_MonitorID="
#define		__TPLID__						"_TemplateID="
//#define     __TPLID__                       "_templateid="
#define		__MATCHCONT__					"_matchcont="
//#define		__MACHINENAME__					"_ServerPort="
#define		__NETSETTYPE__					"_netsettype="
#define		__INDEX__						"_index="
#define     __DISKINDEX__                   "_DiskIndex="
#define     __MACHINENAME__                 "_MachineName="
#define     __PORT__                        "_Port="
#define		__REQUIREID__					"_RequireID="
#define		__SETVALUE__					"_SetValue="
#define		__DISKINTERFACEINDEX__			"_DiskIndex="
#define		__DOWNTIME__					"_downtime="
/////////////////////////////////////////////////////////////////////////////
