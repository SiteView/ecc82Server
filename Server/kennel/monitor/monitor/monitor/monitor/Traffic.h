
/* Control Variables Display Or Not */
#define		IFINDEX_EXIST			0
#define		IFDESCR_EXIST			1
#define		IFTYPE_EXIST			2
//#define		IFMTU_EXIST				3
#define		IFSPEED_EXIST			4
//#define		IFPHYSADDRESS_EXIST		5
//#define		IFADMINSTATUS_EXIST		6
#define		IFOPERSTATUS_EXIST		7
#define		IFLASTCHANGE_EXIST		8
#define		IFINOCTETS_EXIST		9
//#define		IFINUCASTPKTS_EXIST		10
//#define		IFINNUCASTPKTS_EXIST	11
//#define		IFINDISCARDS_EXIST		12
#define		IFINERRORS_EXIST		13
//#define		IFINUNKNOWNPROTOS_EXIST	14
#define		IFOUTOCTETS_EXIST		15
//#define		IFOUTUCASTPKTS_EXIST	16
//#define		IFOUTNUCASTPKTS_EXIST	17
//#define		IFOUTDISCARDS_EXIST		18
#define		IFOUTERRORS_EXIST		19
//#define		IFOUTQLEN_EXIST			20
//#define		IFSPECIFIC_EXIST		21

#include "stdafx.h"
#include "SnmpUtils.h"

class CInterfaceTable : public CObject  
{
public:
	CInterfaceTable();
	virtual ~CInterfaceTable();

public:
	int			ifIndex;
	CString		ifDescr;
	int			ifType;
	int			ifMtu;
	Gauge		ifSpeed;
	CString		ifPhysAddress;
	int			ifAdminStatus;
	int			ifOperStatus;
	TimeTicks	ifLastChange;
	Counter		ifInOctets;
	Counter		ifInUcastPkts;
	Counter		ifInNUcastPkts;
	Counter		ifInDiscards;
	Counter		ifInErrors;
	Counter		ifInUnknownProtos;
	Counter		ifOutOctets;
	Counter		ifOutUcastPkts;
	Counter		ifOutNUcastPkts;
	Counter		ifOutDiscards;
	Counter		ifOutErrors;
	Gauge		ifOutQLen;
	CString		ifSpecific;

	CString		strOther;

public:
	enum ifTypeE 
	{
		TYPE_OTHER = 1, 
	    REGULAR1822, 
	    HDH1822, 
	    DDN_X25, 
	    RFC877_X25, 
	    ETHERNET_CSMACD, 
	    ISO88023_CSMACD, 
	    ISO88024_TOKENBUS, 
	    ISO88025_TOKENRING, 
	    ISO88026_MAN, 
	    STARLAN, 
	    PROTEON_10MBIT, 
	    PROTEON_80MBIT, 
	    HYPERCHANNEL, 
	    FDDI, 
	    LAPB, 
	    SDLC, 
	    DS1, 
	    E1, 
	    BASICISDN, 
	    PRIMARYISDN, 
	    PROPPOINTTOPOINTSERIAL, 
	    PPP, 
	    SOFTWARELOOPBACK, 
	    EON, 
	    ETHERNET_3MBIT, 
	    NSIP, 
	    SLIP, 
	    ULTRA, 
	    DS3, 
	    SIP, 
	    FRAME_RELAY 
	};

	enum ifAdminStatusE 
	{
		ADMINSTATUS_UP = 1, 
		ADMINSTATUS_DOWN, 
		ADMINSTATUS_TESTING
	};

	enum ifOperStatusE 
	{
		OPERSTATUS_UP = 1, 
		OPERSTATUS_DOWN, 
		OPERSTATUS_TESTING
	};
};

typedef CTypedPtrList< CObList, CInterfaceTable * > CInterfaceTableList;

BOOL TRAFFIC_MONITOR(char *server, int port, char *community, int ifindex, 
					 int timeout, int retrydelay, CString strMonitorID, 
					 char *custpath, char *szReturn);
BOOL CTRAFFIC_MONITOR(char *server, int port, char *community, int ifindex, 
					  int timeout, int retrydelay, char *custpath, char *szReturn);
void StoreValues(CInterfaceTable *pifTable, int Item, struct snval *psnval, int ret);
void AddToTrafficLog(LPCTSTR msg);
