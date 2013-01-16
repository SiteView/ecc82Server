
#include "stdafx.h"

#define		DHCP_SERVER_PORT			67
#define		DHCP_CLIENT_PORT			68
#define		DHCP_MULTICAST_ADDRESS		"255.255.255.255"
#define		DHCP_LOCAL_ADDRESS			"0.0.0.0"

typedef RPC_STATUS (RPC_ENTRY *lpUuidCreateSequential)(UUID *Uuid);

typedef struct DHCP_MESSAGE {
	u_char	op;
	u_char	htype;
	u_char	hlen;
	u_char	hops;
	u_char	xid[4];
	u_char	secs[2];
	u_char	flags[2];
	u_char	ciaddr[4];
	u_char	yiaddr[4];
	u_char	siaddr[4];
	u_char	giaddr[4];
	u_char	ch_addr[16];
	u_char	sname[64];
	u_char	file[128];
	struct options {
		u_char opstr[312];
		int oplen;
	} opts;

} DHCPMessage, *LPDHCPMessage;

BOOL DHCP_MONITOR(int timeout, char *re_addr, char *custpath, char *szReturn);
int topacket(LPDHCPMessage pdm, u_char *packet);
int tostruct(LPDHCPMessage pdm, u_char *packet, int packlen);
BOOL GetMacAddress(u_char *mac);
