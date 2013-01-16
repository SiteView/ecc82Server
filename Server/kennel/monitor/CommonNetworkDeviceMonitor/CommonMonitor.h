#include <string>
#include <iostream>
#include <list>
#include <map>

using namespace std;

class clsOID;

typedef std::list<string> StringList;
typedef struct std::list<char*> pStringList;
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
#define		 SV_OID 						 "_OID" 
#define		 SV_GETMODE 					 "_GETMODE"


#define     COUNTER_MAX    4294976295
#define		CPU_MONITOR					1001	//CPU
#define		MEMORY_MONITOR				1002	//�ڴ�
#define		DISK_MONITOR				1003	//����
#define		DISKS_MONITOR				1004	//�����б�
#define		PROCESS_MONITOR				1005	//����
#define		PROCESSES_MONITOR			1006	//�����б�
#define		DEVICE_MONITOR				1007	//�豸
#define		DEVICES_MONITOR				1008	//�豸�б�
#define		FILE_MONITOR				1009	//�ļ�
#define		FILES_MONITOR				1010	//�ļ��б�
#define		SCRIPTS_MONITOR				1011	//�ű�
#define		ERRLOG_MONITOR				1012	//������־
#define		CICS_MONITOR				1013	//CICS������
#define		CICSES_MONITOR				1014	//CICS�������б�
/////////////////////////////////////////////////////////////////////////////
