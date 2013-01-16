
#include "RemotePing.h"
#include "SnmpUtils.h"

#include "base\funcGeneral.h"


enum CiscoNetworkProtocolE
{
	IP = 1, 
	DECNET = 2, 
	PUP = 3, 
	CHAOS = 4, 
	XNS = 5, 
	X121 = 6, 
	APPLETALK = 7, 
	CLNS = 8, 
	LAT = 9, 
	VINES = 10, 
	CONS = 11, 
	APOLLO = 12, 
	STUN = 13, 
	NOVELL = 14, 
	QLLC = 15, 
	SNAPSHOT = 16, 
	ATMILMI = 17, 
	BSTUN = 18, 
	X25PVC = 19, 
	UNKNOWN = 65535

};

enum ciscoPingEntryStatusE
{
	ACTIVE = 1, 
	NOTINSERVICE, 
	NOTREADY, 
	CREATEANDGO, 
	CREATEANDWAIT, 
	DESTROY 

};

BOOL REMOTEPING_MONITOR(char *server, int port, char *community, char *pingserver, int bytes, 
			int timeout, int gid, int mid, char *cpath, char *szReturn)
{


/* MIB-2 System Oids */
struct oid sysObjectID = {{0x2b, 6, 1, 2, 1, 1, 2}, 7, "system.sysObjectID", ASN1_OBJID, ""};
struct oid sysName = {{0x2b, 6, 1, 2, 1, 1, 5}, 7, "system.sysName", ASN1_OCTSTR, ""};

/* Cisco Ping Oids */
struct oid CiscoCompany = {{0x2b, 6, 1, 4, 1, 9}, 6, "enterprise.cisco", 0, ""};

struct oid ciscoPingTable = {{0x2b, 6, 1, 4, 1, 9, 9, 16, 1, 1}, 10, "ciscoPingTable", 0, ""};

struct oid ciscoPingSerialNumber = {{0x2b, 6, 1, 4, 1, 9, 9, 16, 1, 1, 1, 1}, 12, "ciscoPingSerialNumber", ASN1_INT, ""};
struct oid ciscoPingProtocol = {{0x2b, 6, 1, 4, 1, 9, 9, 16, 1, 1, 1, 2}, 12, "ciscoPingProtocol", ASN1_INT, ""};
struct oid ciscoPingAddress = {{0x2b, 6, 1, 4, 1, 9, 9, 16, 1, 1, 1, 3}, 12, "ciscoPingAddress", ASN1_IPADDR, ""};
struct oid ciscoPingPacketCount = {{0x2b, 6, 1, 4, 1, 9, 9, 16, 1, 1, 1, 4}, 12, "ciscoPingPacketCount", ASN1_INT, ""};
struct oid ciscoPingPacketSize = {{0x2b, 6, 1, 4, 1, 9, 9, 16, 1, 1, 1, 5}, 12, "ciscoPingPacketSize", ASN1_INT, ""};
struct oid ciscoPingPacketTimeout = {{0x2b, 6, 1, 4, 1, 9, 9, 16, 1, 1, 1, 6}, 12, "ciscoPingPacketTimeout", ASN1_INT, ""};
struct oid ciscoPingDelay = {{0x2b, 6, 1, 4, 1, 9, 9, 16, 1, 1, 1, 7}, 12, "ciscoPingDelay", ASN1_INT, ""};
struct oid ciscoPingTrapOnCompletion = {{0x2b, 6, 1, 4, 1, 9, 9, 16, 1, 1, 1, 8}, 12, "ciscoPingTrapOnCompletion", ASN1_INT, ""};
struct oid ciscoPingSentPackets = {{0x2b, 6, 1, 4, 1, 9, 9, 16, 1, 1, 1, 9}, 12, "ciscoPingSentPackets", ASN1_COUNTER, ""};
struct oid ciscoPingReceivedPackets = {{0x2b, 6, 1, 4, 1, 9, 9, 16, 1, 1, 1, 10}, 12, "ciscoPingReceivedPackets", ASN1_COUNTER, ""};
struct oid ciscoPingMinRtt = {{0x2b, 6, 1, 4, 1, 9, 9, 16, 1, 1, 1, 11}, 12, "ciscoPingMinRtt", ASN1_INT, ""};
struct oid ciscoPingAvgRtt = {{0x2b, 6, 1, 4, 1, 9, 9, 16, 1, 1, 1, 12}, 12, "ciscoPingAvgRtt", ASN1_INT, ""};
struct oid ciscoPingMaxRtt = {{0x2b, 6, 1, 4, 1, 9, 9, 16, 1, 1, 1, 13}, 12, "ciscoPingMaxRtt", ASN1_INT, ""};
struct oid ciscoPingCompleted = {{0x2b, 6, 1, 4, 1, 9, 9, 16, 1, 1, 1, 14}, 12, "ciscoPingCompleted", ASN1_INT, ""};
struct oid ciscoPingEntryOwner = {{0x2b, 6, 1, 4, 1, 9, 9, 16, 1, 1, 1, 15}, 12, "ciscoPingEntryOwner", ASN1_OCTSTR, ""};
struct oid ciscoPingEntryStatus = {{0x2b, 6, 1, 4, 1, 9, 9, 16, 1, 1, 1, 16}, 12, "ciscoPingEntryStatus", ASN1_INT, ""};
struct oid ciscoPingVrfName = {{0x2b, 6, 1, 4, 1, 9, 9, 16, 1, 1, 1, 17}, 12, "ciscoPingVrfName", ASN1_OCTSTR, ""};

/* IBM Rping OIDs */
struct oid IbmCompany = {{0x2b, 6, 1, 4, 1, 2}, 6, "enterprise.ibm", 0, ""};

struct oid nv6saIcmpEchoReq = {{0x2b, 6, 1, 4, 1, 2, 6, 4, 3, 1}, 10, "nv6saIcmpEchoReq", ASN1_INT, ""};


	int		s, ret = 0, count = 1;
	struct	sockaddr_in serv;

	u_char	reqid = 1;
	int		addid = 0;

	int	packetSent = 0, packetReceived = 0;
	int	minRtt = 0, avgRtt = 0, maxRtt = 0;

	struct	oid	*poid = NULL;
	struct	oid *ptmpoid = NULL;

	struct	snval *ptmp = NULL;
	struct	snval *ptmpnext = NULL;
	struct	snval *psnval = NULL;

	long	laddr = 0;
	u_char	ipaddr[4] = {0};

	srand((unsigned)time(NULL));
	addid = rand();

	laddr = inet_addr(pingserver);
	if (laddr == INADDR_NONE)
	{
		struct hostent *hp = gethostbyname(pingserver);
		if (hp == NULL)
 		{
			sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Ping_2%>"));//<%IDS_Ping_2%>
			return FALSE;
		}

		sscanf(inet_ntoa(*((struct in_addr *)hp->h_addr)), "%u.%u.%u.%u", 
			&ipaddr[0], &ipaddr[1], &ipaddr[2], &ipaddr[3]);
	}
	else
	{
		sscanf(pingserver, "%u.%u.%u.%u", 
			&ipaddr[0], &ipaddr[1], &ipaddr[2], &ipaddr[3]);
	}

	if(MakeSocket(server, port, s, serv) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_01%>"));		// 建立通信过程失败
		return FALSE;
	}

	struct oid		oidr;
	struct snval	snvalr;

	memset(&oidr, 0, sizeof(struct oid));
	oidr = sysObjectID;
	oidr.id[oidr.len ++] = 0;
	memset(&snvalr, 0, sizeof(struct snval));
	if((ret = SN_VALUE(s, serv, community, reqid ++, &oidr, &snvalr)) < 0)
	{
		sprintf(szReturn, "error=%s (%d)$", FuncGetStringFromIDS("<%IDS_RemotePing_02%>"), ret);		// 通信过程失败 (sysObjectID)
		return FALSE;
	}

	if(snvalr.sv_type != ASN1_OBJID)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_03%>"));		// 请确认路由器支持正确的MIB
		return FALSE;
	}

	if(!compare_oid(snvalr.sv_val.sv_oid, IbmCompany))
	{
		memset(&oidr, 0, sizeof(struct oid));
		oidr = nv6saIcmpEchoReq;
		oidr.id[oidr.len ++] = bytes;
		oidr.id[oidr.len ++] = timeout;
		oidr.id[oidr.len ++] = ipaddr[0];
		oidr.id[oidr.len ++] = ipaddr[1];
		oidr.id[oidr.len ++] = ipaddr[2];
		oidr.id[oidr.len ++] = ipaddr[3];

		memset(&snvalr, 0, sizeof(struct snval));
		if((ret = SN_VALUE(s, serv, community, reqid ++, &oidr, &snvalr)) < 0)
		{
			sprintf(szReturn, "error=%s (%d)$", FuncGetStringFromIDS("<%IDS_RemotePing_02%>"), ret);		// 通信过程失败 (sysObjectID)
			return FALSE;
		}

		if(snvalr.sv_type != ASN1_INT)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_03%>"));		// 请确认路由器支持正确的MIB
			return FALSE;
		}

		switch(snvalr.sv_val.sv_int)
		{
		case -1:
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_18%>"));		// internal error
			return FALSE;
		case -2:
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_19%>"));		// request timed out
			//sprintf(szReturn, "error=%s", "request timed out");		// 
			return FALSE;
		case -3:
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_20%>"));		// echo reply is not the correct reply
			return FALSE;
		case -4:
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_21%>"));		// packet size is too large
			return FALSE;
		case -5:
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_22%>"));		// timeout is invalid
			return FALSE;
		default:
			sprintf(szReturn, "roundTripTime=%d$", snvalr.sv_val.sv_int);
			return TRUE;
		}
	}
	else if(compare_oid(snvalr.sv_val.sv_oid, CiscoCompany))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_04%>"));		// 请确认是否为CISCO路由器
		return FALSE;
	}

	
	memset(&oidr, 0, sizeof(struct oid));
	oidr = sysName;
	oidr.id[oidr.len ++] = 0;
	memset(&snvalr, 0, sizeof(struct snval));
	ret = SN_VALUE(s, serv, community, reqid ++, &oidr, &snvalr);


	memset(&oidr, 0, sizeof(struct oid));
	oidr = ciscoPingEntryStatus;
	oidr.id[oidr.len ++] = addid;
	memset(&snvalr, 0, sizeof(struct snval));
	snvalr.sv_type = ASN1_INT;
	snvalr.sv_val.sv_int = 0x06;
	if((ret = SN_SET_VALUES(s, serv, community, reqid ++, &oidr, &snvalr)) < 0)
	{
		//printf("teststests:%d \n",ret);
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_05%>"));		// 通信过程失败 (CiscoPingEntryStatus)
		return FALSE;
	}



	memset(&oidr, 0, sizeof(struct oid));
	oidr = ciscoPingEntryStatus;
	oidr.id[oidr.len ++] = addid;
	memset(&snvalr, 0, sizeof(struct snval));
	snvalr.sv_type = ASN1_INT;
	snvalr.sv_val.sv_int = 0x05;
	if((ret = SN_SET_VALUES(s, serv, community, reqid ++, &oidr, &snvalr)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_05%>"));		// 通信过程失败 (CiscoPingEntryStatus)
		return FALSE;
	}

	char	hostname[256] = {"0"};
	char	ipaddress[64] = {"0"};
	char	entryowner[1024] = {0};
	if(!gethostname(hostname, sizeof(hostname)))
	{
		struct hostent *hp = NULL;
		if((hp = gethostbyname(hostname)) != NULL)
		{
			strcpy(ipaddress, inet_ntoa(*((struct in_addr *)hp->h_addr)));
		}
	}
	sprintf(entryowner, "DragonFlow.Net Proxy Ping On %s (%s)", hostname, ipaddress);

	memset(&oidr, 0, sizeof(struct oid));
	oidr = ciscoPingEntryOwner;
	oidr.id[oidr.len ++] = addid;
	memset(&snvalr, 0, sizeof(struct snval));
	snvalr.sv_type = ASN1_OCTSTR;
	snvalr.sv_val.sv_str.sv_len = strlen(entryowner);
	strncpy(snvalr.sv_val.sv_str.sv_str, entryowner, strlen(entryowner));
	if((ret = SN_SET_VALUES(s, serv, community, reqid ++, &oidr, &snvalr)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_06%>"));		// 通信过程失败 (CiscoPingEntryOwner)
		return FALSE;
	}

	/* ciscoPingProtocol */
	memset(&oidr, 0, sizeof(struct oid));
	oidr = ciscoPingProtocol;
	oidr.id[oidr.len ++] = addid;
	memset(&snvalr, 0, sizeof(struct snval));
	snvalr.sv_type = ASN1_INT;
	snvalr.sv_val.sv_int = 0x01;
	if((ret = SN_SET_VALUES(s, serv, community, reqid ++, &oidr, &snvalr)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_07%>"));		// 通信过程失败 (CiscoPingProtocol)
		return FALSE;
	}

	/* ciscoPingPacketSize */
	memset(&oidr, 0, sizeof(struct oid));
	oidr = ciscoPingPacketSize;
	oidr.id[oidr.len ++] = addid;
	memset(&snvalr, 0, sizeof(struct snval));
	snvalr.sv_type = ASN1_INT;
	snvalr.sv_val.sv_int = bytes;
	if((ret = SN_SET_VALUES(s, serv, community, reqid ++, &oidr, &snvalr)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_08%>"));		// 通信过程失败 [CiscoPingPacketSize]
		return FALSE;
	}

	/* ciscoPingPacketCount */
	memset(&oidr, 0, sizeof(struct oid));
	oidr = ciscoPingPacketCount;
	oidr.id[oidr.len ++] = addid;
	memset(&snvalr, 0, sizeof(struct snval));
	snvalr.sv_type = ASN1_INT;
	snvalr.sv_val.sv_int = 0x04;
	if((ret = SN_SET_VALUES(s, serv, community, reqid ++, &oidr, &snvalr)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_09%>"));		// 通信过程失败 [CiscoPingPacketCount]
		return FALSE;
	}

	/* ciscoPingPacketTimeout */
	memset(&oidr, 0, sizeof(struct oid));
	oidr = ciscoPingPacketTimeout;
	oidr.id[oidr.len ++] = addid;
	memset(&snvalr, 0, sizeof(struct snval));
	snvalr.sv_type = ASN1_INT;
	snvalr.sv_val.sv_int = timeout;
	if((ret = SN_SET_VALUES(s, serv, community, reqid ++, &oidr, &snvalr)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_16%>"));		// 通信过程失败 [CiscoPingPacketTimeout]
		return FALSE;
	}

	/* ciscoPingDelay */
	memset(&oidr, 0, sizeof(struct oid));
	oidr = ciscoPingDelay;
	oidr.id[oidr.len ++] = addid;
	memset(&snvalr, 0, sizeof(struct snval));
	snvalr.sv_type = ASN1_INT;
	snvalr.sv_val.sv_int = 0;
	if((ret = SN_SET_VALUES(s, serv, community, reqid ++, &oidr, &snvalr)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_10%>"));		// 通信过程失败 [CiscoPingDelay]
		return FALSE;
	}

	/* ciscoPingAddress */
	memset(&oidr, 0, sizeof(struct oid));
	oidr = ciscoPingAddress;
	oidr.id[oidr.len ++] = addid;
	memset(&snvalr, 0, sizeof(struct snval));
	snvalr.sv_type = ASN1_OCTSTR;
	snvalr.sv_val.sv_str.sv_len = 4;
	snvalr.sv_val.sv_str.sv_str[0] = ipaddr[0];
	snvalr.sv_val.sv_str.sv_str[1] = ipaddr[1];
	snvalr.sv_val.sv_str.sv_str[2] = ipaddr[2];
	snvalr.sv_val.sv_str.sv_str[3] = ipaddr[3];
	if((ret = SN_SET_VALUES(s, serv, community, reqid ++, &oidr, &snvalr)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_11%>"));		// 通信过程失败 [CiscoPingAddress]
		return FALSE;
	}

	/* ciscoPingEntryStatus */
	memset(&oidr, 0, sizeof(struct oid));
	oidr = ciscoPingEntryStatus;
	oidr.id[oidr.len ++] = addid;
	memset(&snvalr, 0, sizeof(struct snval));
	snvalr.sv_type = ASN1_INT;
	snvalr.sv_val.sv_int = 0x01;
	if((ret = SN_SET_VALUES(s, serv, community, reqid ++, &oidr, &snvalr)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_12%>"));		// 通信过程失败 [CiscoPingEntryStatus]
		return FALSE;
	}

next:

	

	/* ciscoPingCompleted */
	memset(&oidr, 0, sizeof(struct oid));
	oidr = ciscoPingCompleted;
	oidr.id[oidr.len ++] = addid;
	memset(&snvalr, 0, sizeof(struct snval));
	if((ret = SN_VALUE(s, serv, community, reqid ++, &oidr, &snvalr)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_13%>"));		// 通信过程失败 [CiscoPingCompleted]
		return FALSE;
	}

//	if(snvalr.sv_type != ASN1_INT)
//	{
//		return FALSE;
//	}

	if(snvalr.sv_val.sv_int == 2)
	{
		Sleep(1000);
		goto next;
	}

	if(snvalr.sv_val.sv_int != 1)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_14%>"));		// 所取变量的值未知
		ret = 1;
		goto w;
	}

	poid = NULL, ptmpoid = NULL;
	ciscoPingSentPackets.id[ciscoPingSentPackets.len++] = addid;
	poid = ptmpoid = &ciscoPingSentPackets;
	ciscoPingReceivedPackets.id[ciscoPingReceivedPackets.len++] = addid;
	ptmpoid->next = &ciscoPingReceivedPackets;
	ptmpoid = ptmpoid->next;
	ciscoPingMinRtt.id[ciscoPingMinRtt.len++] = addid;
	ptmpoid->next = &ciscoPingMinRtt;
	ptmpoid = ptmpoid->next;
	ciscoPingAvgRtt.id[ciscoPingAvgRtt.len++] = addid;
	ptmpoid->next = &ciscoPingAvgRtt;
	ptmpoid = ptmpoid->next;
	ciscoPingMaxRtt.id[ciscoPingMaxRtt.len++] = addid;
	ptmpoid->next = &ciscoPingMaxRtt;
	ptmpoid = ptmpoid->next;

	ptmp = NULL;
	ptmpnext = NULL;
	psnval = NULL;
	Sleep(timeout);
	if((ret = SN_VALUES(s, serv, community, 1, poid, &psnval)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_15%>"));		// 网络连接超时
		sprintf(szReturn, "error=%d$", ret);		// 网络连接超时

		//OutputDebugString("szReturn=");
		//OutputDebugString(szReturn);
		//OutputDebugString("\n");

		ret = 1;
		goto w;
	}

	ptmp = psnval;
	ptmpnext = NULL;
	while(ptmp)
	{
		switch(count)
		{
		case 1:
			switch(ptmp->sv_type)
			{
			case ASN1_COUNTER:
				packetSent = ptmp->sv_val.sv_int;
				break;
			default:
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_17%>"));	// 所取变量数据类型错误
				ret = 1;
				goto w;
			}
			break;
		case 2:
			switch(ptmp->sv_type)
			{
			case ASN1_COUNTER:
				packetReceived = ptmp->sv_val.sv_int;
				break;
			default:
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_17%>"));	// 所取变量数据类型错误
				ret = 1;
				goto w;
			}
			break;
		case 3:
			switch(ptmp->sv_type)
			{
			case ASN1_INT:
				minRtt = ptmp->sv_val.sv_int;
				break;
			default:
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_17%>"));	// 所取变量数据类型错误
				ret = 1;
				goto w;
			}
			break;
		case 4:
			switch(ptmp->sv_type)
			{
			case ASN1_INT:
				avgRtt = ptmp->sv_val.sv_int;
				break;
			default:
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_17%>"));	// 所取变量数据类型错误
				ret = 1;
				goto w;
			}
			break;
		case 5:
			switch(ptmp->sv_type)
			{
			case ASN1_INT:
				maxRtt = ptmp->sv_val.sv_int;
				break;
			default:
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_RemotePing_17%>"));	// 所取变量数据类型错误
				ret = 1;
				goto w;
			}
			break;
		}

		count ++;
		ptmp = ptmp->next;
	}

	sprintf(szReturn, "roundTripTime=%.0f$packetsGoodPercent=%.2f$", 
		(float)avgRtt, (float)packetReceived / packetSent * 100);
//	sprintf(szReturn, "packetSent=%ld$packetReceived=%ld$minRtt=%d$avgRtt=%d$maxRtt=%d$", 
//			packetSent, packetReceived, minRtt, avgRtt, maxRtt);

w:	ptmp = psnval;
	if(ptmp) ptmpnext = ptmp->next;
	while(ptmp)
	{
		free(ptmp);
		ptmp = ptmpnext;
		if(ptmp) ptmpnext = ptmp->next;
	}

	return !ret;
}
