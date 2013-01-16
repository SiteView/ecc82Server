
#include "stdafx.h"
#include "SnmpUtils.h"

struct tagInterfaceInfo
{
	int			ifIndex;
	char		ifDescr[1024];
	int			ifType;
	int			ifMtu;
	Gauge		ifSpeed;
	char		ifPhysAddress[64];
	int			ifAdminStatus;
	int			ifOperStatus;
	TimeTicks	ifLastChange;
	char		ifOther[256];
	struct tagInterfaceInfo *next;
};

BOOL GetInterfaces(char *server, int port, char *community, char *custpath, CString &strBuffer);
BOOL MallocInterfaceList(int nInterfaces, struct tagInterfaceInfo **pIfList);
struct tagInterfaceInfo *ReturnIfOfIndex(struct tagInterfaceInfo *pIfList, int index);
void StoreValues(struct tagInterfaceInfo *pInterface, int Item, struct snval *psnval, int ret);
BOOL cisco_GetInterfaces(char *server, int port, char *community, char *custpath, CString &strBuffer);
BOOL INTERFACE_STATUS_MONITOR(char *server, int port, char *community, int index, int timeout, 
			int rdelay, CString strMonitor, char *cpath, char *szReturn);
BOOL INTERFACE_ERRORDROP_MONITOR(char *server, int port, char *community, int index, 
			int timeout, int rdelay, CString strMonitor, char *cpath, char *szReturn);
