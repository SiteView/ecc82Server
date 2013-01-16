
#include "Interfaces.h"

#include "base\funcGeneral.h"

struct oid	interfaces_ifnumber_oid = {{0x2b, 6, 1, 2, 1, 2, 1, 0}, 8};
struct oid	interfaces_iftable_oid = {{0x2b, 6, 1, 2, 1, 2, 2}, 7};
struct oid	interfaces_ifother_oid = {{0x2b, 6, 1, 2, 1, 31, 1, 1, 1, 1}, 10};

extern struct oid ifDescr_oid;
extern struct oid ifType_oid;
extern struct oid ifMtu_oid;
extern struct oid ifSpeed_oid;
extern struct oid ifPhysAddress_oid;
extern struct oid ifAdminStatus_oid;
extern struct oid ifLastChange_oid;
extern struct oid ifOperStatus_oid;
//extern struct oid ifInDiscards_oid;
extern struct oid ifInErrors_oid;
//extern struct oid ifOutDiscards_oid;
extern struct oid ifOutErrors_oid;

BOOL GetInterfaces(char *server, int port, char *community, char *custpath, CString &strBuffer)
{
	int s;
	struct sockaddr_in serv;
	int i = 0, ret = 0, ifNumber = 0;

	strBuffer = _T("");

	if(MakeSocket(server, port, s, serv) < 0)
		return FALSE;

	struct snval snvalr;
	memset(&snvalr, 0, sizeof(struct snval));
	SN_VALUE(s, serv, community, 1, &interfaces_ifnumber_oid, &snvalr);
	if(snvalr.sv_type != ASN1_INT)
		return FALSE;

	ifNumber = snvalr.sv_val.sv_int;
	if(ifNumber == 0)
		return FALSE;

	struct tagInterfaceInfo *pIfList = NULL;
	if(!MallocInterfaceList(ifNumber, &pIfList))
		return FALSE;

	struct oid *poid = (struct oid *)malloc(sizeof(struct oid));
	memset(poid, 0, sizeof(struct oid));
	struct snval *psnval = (struct snval *)malloc(sizeof(struct snval));
	memset(psnval, 0, sizeof(struct snval));

	for(i = 0;i < ifNumber;i ++)
	{
		struct tagInterfaceInfo *pInterface = ReturnIfOfIndex(pIfList, i);
		if(i == 0)
		{
			ret = SN_NEXT_VALUE(s, serv, community, 1, &interfaces_iftable_oid, poid, psnval);
			
			StoreValues(pInterface, 0, psnval, ret);
			printf("interface is %d\n", pInterface->ifIndex);
		}
		else
		{
			ret = SN_NEXT_VALUE(s, serv, community, 1, poid, poid, psnval);
			StoreValues(pInterface, 0, psnval, ret);
			printf("interface is %d\n", pInterface->ifIndex);
		}
	}

	struct oid	OID;
	struct tagInterfaceInfo *pInterface = pIfList;
	while(pInterface)
	{
		OID = ifDescr_oid;
		OID.id[OID.len ++] = pInterface->ifIndex;
		ret = SN_VALUE(s, serv, community, 1, &OID, psnval);
		StoreValues(pInterface, 1, psnval, ret);

		OID = ifType_oid;
		OID.id[OID.len ++] = pInterface->ifIndex;
		ret = SN_VALUE(s, serv, community, 1, &OID, psnval);
		StoreValues(pInterface, 2, psnval, ret);
/*
		OID = ifMtu_oid;
		OID.id[OID.len ++] = pInterface->ifIndex;
		ret = SN_VALUE(s, serv, community, &OID, psnval);
		StoreValues(pInterface, 3, psnval, ret);
*/
/*
		OID = ifSpeed_oid;
		OID.id[OID.len ++] = pInterface->ifIndex;
		ret = SN_VALUE(s, serv, community, &OID, psnval);
		StoreValues(pInterface, 4, psnval, ret);
*/
/*
		OID = ifPhysAddress_oid;
		OID.id[OID.len ++] = pInterface->ifIndex;
		ret = SN_VALUE(s, serv, community, &OID, psnval);
		StoreValues(pInterface, 5, psnval, ret);
*/
/*
		OID = ifAdminStatus_oid;
		OID.id[OID.len ++] = pInterface->ifIndex;
		ret = SN_VALUE(s, serv, community, &OID, psnval);
		StoreValues(pInterface, 6, psnval, ret);
*/
		OID = ifOperStatus_oid;
		OID.id[OID.len ++] = pInterface->ifIndex;
		ret = SN_VALUE(s, serv, community, 1, &OID, psnval);
		StoreValues(pInterface, 7, psnval, ret);
/*
		OID = ifLastChange_oid;
		OID.id[OID.len ++] = pInterface->ifIndex;
		ret = SN_VALUE(s, serv, community, &OID, psnval);
		StoreValues(pInterface, 8, psnval, ret);
*/
		OID = interfaces_ifother_oid;
		OID.id[OID.len ++] = pInterface->ifIndex;
		ret = SN_VALUE(s, serv, community, 1, &OID, psnval);
		StoreValues(pInterface, 9, psnval, ret);

		pInterface = pInterface->next;
	}

	if(poid) free(poid);
	if(psnval) free(psnval);

	pInterface = pIfList;
	while(pInterface)
	{
		char *ifType = NULL;
		ifType = pInterface->ifType == 1 ? "other" : 
				pInterface->ifType == 2 ? "regular1822" :
				pInterface->ifType == 3 ? "hdh1822":
				pInterface->ifType == 4 ? "ddn-x25":
				pInterface->ifType == 5 ? "rfc877-x25":
				pInterface->ifType == 6 ? "ethernet-csmacd":
				pInterface->ifType == 7 ? "iso88023-csmacd":
				pInterface->ifType == 8 ? "iso88024-tokenBus":
				pInterface->ifType == 9 ? "iso88025-tokenRing":
				pInterface->ifType == 10 ? "iso88026-man":
				pInterface->ifType == 11? "starLan":
				pInterface->ifType == 12 ? "proteon-10Mbit":
				pInterface->ifType == 13 ? "proteon-80Mbit":
				pInterface->ifType == 14 ? "hyperchannel":
				pInterface->ifType == 15 ? "fddi":
				pInterface->ifType == 16 ? "lapb":
				pInterface->ifType == 17 ? "sdlc":
				pInterface->ifType == 18 ? "ds1":
				pInterface->ifType == 19 ? "e1":
				pInterface->ifType == 20 ? "basicISDN":
				pInterface->ifType == 21 ? "primaryISDN":
				pInterface->ifType == 22 ? "propPointToPointSerial":
				pInterface->ifType == 23 ? "ppp":
				pInterface->ifType == 24 ? "softwareLoopback":
				pInterface->ifType == 25 ? "eon":
				pInterface->ifType == 26 ? "ethernet-3Mbit":
				pInterface->ifType == 27 ? "nsip":
				pInterface->ifType == 28 ? "slip":
				pInterface->ifType == 29 ? "ultra":
				pInterface->ifType == 30 ? "ds3":
				pInterface->ifType == 31 ? "sip":
				pInterface->ifType == 32 ? "frame-relay": "unknown";

		char *ifOperState = NULL;
		ifOperState =	pInterface->ifOperStatus == 1 ? "up":
						pInterface->ifOperStatus == 2 ? "down":
						pInterface->ifOperStatus == 3 ? "testing" : "unknown";

		char szReturn[1024] = {0};
		sprintf(szReturn, "%s%d=%d_%s_%s_%s_%s$$$", szReturn, pInterface->ifIndex,pInterface->ifIndex, pInterface->ifDescr, ifType, ifOperState, pInterface->ifOther);
		strBuffer += szReturn;
#if 0
		FILE *fp = fopen(".\\interface.log", "a+");
		if(fp != NULL)
		{
			fprintf(fp, "%s\r\n", szReturn);
			fclose(fp);
		}
#endif
		pInterface = pInterface->next;
	}

	struct tagInterfaceInfo *pIf = pIfList;
	struct tagInterfaceInfo *pIf2 = pIf->next;
	while(pIf)
	{
		free(pIf);
		pIf = pIf2;
		if(!pIf) break;
		pIf2 = pIf2->next;
	}

	return TRUE;
}


struct oid cisco_iftable_oid = 
{{0x2b, 6, 1, 4, 1, 9, 2, 2, 1}, 9,  "ifTable", ASN1_COUNTER, ""};

/*
struct oid cisco_iftable_oid = 
{{0x2b, 6, 1, 2, 1, 2, 2}, 7,  "ifTable", ASN1_COUNTER, ""};
*/

BOOL cisco_GetInterfaces(char *server, int port, char *community, char *custpath, CString &strBuffer)
{
	int s, ret = 0;
	struct sockaddr_in serv;

	strBuffer = _T("");

	if(MakeSocket(server, port, s, serv) < 0)
		return FALSE;

	struct oid *poid = (struct oid *)malloc(sizeof(struct oid));
	memset(poid, 0, sizeof(struct oid));
	struct snval *psnval = (struct snval *)malloc(sizeof(struct snval));
	memset(psnval, 0, sizeof(struct snval));

	ret = SN_NEXT_VALUE(s, serv, community, 1, &cisco_iftable_oid, poid, psnval);
	if(ret)
		return FALSE;
	else
	{
		if(compare_oid(*poid, cisco_iftable_oid))
			return FALSE;
		else
		{
			char szReturn[1024] = {0};
			sprintf(szReturn, "%d$$$", psnval->sv_val.sv_int);
			strBuffer += szReturn;
		}
	}

	struct oid soid;
	memset(&soid, 0, sizeof(struct oid));
	for(int i = 0;i < poid->len - 1;i ++)
		soid.id[soid.len++] = poid->id[i];

	while(1)
	{
		ret = SN_NEXT_VALUE(s, serv, community, 1, poid, poid, psnval);
		if(ret)
			break;
		else
		{
			if(compare_oid(*poid, soid))
				break;
			else
			{
				char szReturn[1024] = {0};
				sprintf(szReturn, "%s%d$$$", szReturn, psnval->sv_val.sv_int);
				strBuffer += szReturn;
			}
		}
	}

	return TRUE;
}

BOOL MallocInterfaceList(int nInterfaces, struct tagInterfaceInfo **pIfList)
{
	int i = 0;
	struct tagInterfaceInfo *pIf = NULL;
	struct tagInterfaceInfo *pIf2 = NULL;

	while(nInterfaces --)
	{
		pIf2 = (struct tagInterfaceInfo *)malloc(sizeof(struct tagInterfaceInfo));
		if(!pIf2)
		{
			pIf = *pIfList;
			while(pIf)
			{
				free(pIf);
				pIf = pIf->next;
			}

			return FALSE;
		}

		memset(pIf2, 0, sizeof(struct tagInterfaceInfo));
		if(i == 0) 
			*pIfList = pIf = pIf2;
		else
		{
			pIf->next = pIf2;
			pIf = pIf->next;
		}

		i ++;
	}

	return TRUE;
}

struct tagInterfaceInfo *ReturnIfOfIndex(struct tagInterfaceInfo *pIfList, int index)
{
	int i = 0;
	struct tagInterfaceInfo *pInterface = pIfList;
	while(i < index && pInterface)
	{
		pInterface = pInterface->next;

		i ++;
	}

	return pInterface;
}

void StoreValues(struct tagInterfaceInfo *pInterface, int Item, struct snval *psnval, int ret)
{
	int	k = 0;

	switch(Item)
	{
	case IFINDEX:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pInterface->ifIndex = psnval->sv_val.sv_int;
		else
			pInterface->ifIndex = ret;
		break;
	case IFDESCR:
		if(ret == 0 && psnval->sv_type == ASN1_OCTSTR)
			for(k = 0;k < psnval->sv_val.sv_str.sv_len;k ++)
				pInterface->ifDescr[k] = *(psnval->sv_val.sv_str.sv_str + k);
		else
			sprintf(pInterface->ifDescr, "%d", ret);
		break;
	case IFTYPE:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pInterface->ifType = psnval->sv_val.sv_int;
		else 
			pInterface->ifType = ret;
		break;
	case IFMTU:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pInterface->ifMtu = psnval->sv_val.sv_int;
		else
			pInterface->ifMtu = ret;
		break;
	case IFSPEED:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pInterface->ifSpeed = psnval->sv_val.sv_int;
		else
			pInterface->ifSpeed = ret;
		break;
	case IFPHYSADDRESS:
		if(ret == 0 && psnval->sv_type == ASN1_OCTSTR)
			if(psnval->sv_val.sv_str.sv_len < 6)
				memset(pInterface->ifPhysAddress, 0, 64);
			else
			{
				sprintf(pInterface->ifPhysAddress, "%x-%x-%x-%x-%x-%x", 
						*(psnval->sv_val.sv_str.sv_str + 0) & 0xFF, 
						*(psnval->sv_val.sv_str.sv_str + 1) & 0xFF, 
						*(psnval->sv_val.sv_str.sv_str + 2) & 0xFF, 
						*(psnval->sv_val.sv_str.sv_str + 3) & 0xFF, 
						*(psnval->sv_val.sv_str.sv_str + 4) & 0xFF, 
						*(psnval->sv_val.sv_str.sv_str + 5) & 0xFF); 
			}
		else
			sprintf(pInterface->ifPhysAddress, "%d", ret);
		break;
	case IFADMINSTATUS:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pInterface->ifAdminStatus = psnval->sv_val.sv_int;
		else
			pInterface->ifAdminStatus = ret;
		break;
	case IFOPERSTATUS:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pInterface->ifOperStatus = psnval->sv_val.sv_int;
		else
			pInterface->ifOperStatus = ret;
		break;
	case IFLASTCHANGE:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pInterface->ifLastChange = psnval->sv_val.sv_int;
		else
			pInterface->ifLastChange = ret;
		break;
	case 9:		// For Other
		if(ret == 0 && psnval->sv_type == ASN1_OCTSTR)
			strncpy(pInterface->ifOther, psnval->sv_val.sv_str.sv_str, psnval->sv_val.sv_str.sv_len);
		else
			memset(pInterface->ifOther, 0, sizeof(pInterface->ifOther));
		break;
	}
}

BOOL INTERFACE_STATUS_MONITOR(char *server, int port, char *community, int index, int timeout, 
			int rdelay, CString strMonitor, char *cpath, char *szReturn)
{
	int s, ret = 0;
	struct sockaddr_in serv;

	if(MakeSocket(server, port, s, serv) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Interfaces_01%>"));		// 建立通信过程失败
		return FALSE;
	}

	struct oid		oidr;
	struct snval	snvalr;
	
	oidr = ifOperStatus_oid;
	oidr.id[oidr.len ++] = index;
	memset(&snvalr, 0, sizeof(struct snval));
	if((ret = SN_VALUE(s, serv, community, 1, &oidr, &snvalr)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_4%>"));//<%IDS_Dynamo_4%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_5%>"));//<%IDS_Dynamo_5%>
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_6%>"));//<%IDS_Dynamo_6%>
		else if(ret == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_7%>"));//<%IDS_Dynamo_7%>
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_8%>"));//<%IDS_Dynamo_8%>
		else if(ret == -6)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_9%>"));//<%IDS_Dynamo_9%>
		else if(ret == -7)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_10%>"));//<%IDS_Dynamo_10%>
		else if(ret == -8)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_11%>"));//<%IDS_Dynamo_11%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_12%>"));//<%IDS_Dynamo_12%>

		return FALSE;
	}

	if(snvalr.sv_type != ASN1_INT)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Interfaces_02%>"));		// 所取变量的数据类型错误
		return FALSE;
	}

	if(snvalr.sv_val.sv_int == 1)
	{
		sprintf(szReturn, "status=%s$", "up");
	}
	else if(snvalr.sv_val.sv_int == 2)
	{
		sprintf(szReturn, "status=%s$", "down");
	}
	else if(snvalr.sv_val.sv_int == 3)
	{
		sprintf(szReturn, "status=%s$", "testing");
	}
	else
	{
		sprintf(szReturn, "status=%s$", "unknown");
	}

	return TRUE;
}

BOOL INTERFACE_ERRORDROP_MONITOR(char *server, int port, char *community, int ifindex, 
			int timeout, int rdelay, CString strMonitor, char *cpath, char *szReturn)
{
	int		s;
	struct	sockaddr_in serv;

	int		ret = 0;
	int		count = 1;
	struct	snval *ptmp = NULL;
	struct	snval *ptmpnext = NULL;

	Counter ifInDiscards = 0, ifOutDiscards = 0;
	Counter ifInErrors = 0, ifOutErrors = 0;

	if((ret = MakeSocket(server, port, s, serv)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Traffic_1%>"));
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Traffic_2%>"));
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Traffic_3%>"));

		return FALSE;
	}

	struct oid *poid = NULL, *ptmpoid = NULL;
	oid ifOutDiscards_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 19}, 9};
	oid ifInDiscards_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 13}, 9};
	ifInDiscards_oid.id[ifInDiscards_oid.len++] = ifindex;
	poid = ptmpoid = &ifInDiscards_oid;
	ifInErrors_oid.id[ifInErrors_oid.len++] = ifindex;
	ptmpoid->next = &ifInErrors_oid;
	ptmpoid = ptmpoid->next;
	ifOutDiscards_oid.id[ifOutDiscards_oid.len++] = ifindex;
	ptmpoid->next = &ifOutDiscards_oid;
	ptmpoid = ptmpoid->next;
	ifOutErrors_oid.id[ifOutErrors_oid.len++] = ifindex;
	ptmpoid->next = &ifOutErrors_oid;
	ptmpoid = ptmpoid->next;

	struct snval *psnval = NULL;
	if((ret = SN_VALUES(s, serv, community, 1, poid, &psnval)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_4%>"));//<%IDS_Dynamo_4%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_5%>"));//<%IDS_Dynamo_5%>
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_6%>"));//<%IDS_Dynamo_6%>
		else if(ret == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_7%>"));//<%IDS_Dynamo_7%>
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_8%>"));//<%IDS_Dynamo_8%>
		else if(ret == -6)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_9%>"));//<%IDS_Dynamo_9%>
		else if(ret == -7)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_10%>"));//<%IDS_Dynamo_10%>
		else if(ret == -8)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_11%>"));//<%IDS_Dynamo_11%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_12%>"));//<%IDS_Dynamo_12%>

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
				ifInDiscards = ptmp->sv_val.sv_int;
				break;
			default:
				ifInDiscards = 0;
				break;
			}
			break;
		case 2:
			switch(ptmp->sv_type)
			{
			case ASN1_COUNTER:
				ifInErrors = ptmp->sv_val.sv_int;
				break;
			default:
				ifInErrors = 0;
				break;
			}
			break;
		case 3:
			switch(ptmp->sv_type)
			{
			case ASN1_COUNTER:
				ifOutDiscards = ptmp->sv_val.sv_int;
				break;
			default:
				ifOutDiscards = 0;
				break;
			}
			break;
		case 4:
			switch(ptmp->sv_type)
			{
			case ASN1_COUNTER:
				ifOutErrors = ptmp->sv_val.sv_int;
				break;
			default:
				ifOutErrors = 0;
				break;
			}
			break;
		}

		count ++;
		ptmp = ptmp->next;
	}

	// computer the rate 
	{
	char ciniFile[256] = {0}, 
		 cinifile[256] = {0};
	char cifIndex[256] = {0};
	sprintf(cinifile, "snmp_%s_%s.ini", server, strMonitor);
	sprintf(ciniFile, "%s\\%s", cpath, cinifile);
	sprintf(cifIndex, "ifIndex_%ld", ifindex);

	long	ifTime = 0;
	Counter inDiscards = 0, outDiscards = 0;
	Counter inErrors = 0, outErrors = 0;

	CTime curTime = CTime::GetCurrentTime();

	ifTime = DFN_GetPrivateProfileInt(cifIndex, "ifTime", -99, ciniFile);
	inDiscards = DFN_GetPrivateProfileInt(cifIndex, "ifInDiscards", -99, ciniFile);
	outDiscards = DFN_GetPrivateProfileInt(cifIndex, "ifOutDiscards", -99, ciniFile);
	inErrors = DFN_GetPrivateProfileInt(cifIndex, "ifInErrors", -99, ciniFile);
	outErrors = DFN_GetPrivateProfileInt(cifIndex, "ifOutErrors", -99, ciniFile);

	if(ifTime == -99 || inDiscards == -99 || outDiscards == -99 || inErrors == -99 || outErrors == -99)
	{
		CString strSection = _T("");
		strSection.Format("ifIndex_%ld", ifindex);

		CString strMsg = _T("");
		strMsg.Format("%ld", curTime.GetTime());
		::DFN_WritePrivateProfileString((LPCTSTR)strSection, "ifTime", (LPCTSTR)strMsg, ciniFile);

		strMsg.Format("%ld", ifInDiscards);
		::DFN_WritePrivateProfileString((LPCTSTR)strSection, "ifInDiscards", (LPCTSTR)strMsg, ciniFile);

		strMsg.Format("%ld", ifOutDiscards);
		::DFN_WritePrivateProfileString((LPCTSTR)strSection, "ifOutDiscards", (LPCTSTR)strMsg, ciniFile);

		strMsg.Format("%ld", ifInErrors);
		::DFN_WritePrivateProfileString((LPCTSTR)strSection, "ifInErrors", (LPCTSTR)strMsg, ciniFile);

		strMsg.Format("%ld", ifOutErrors);
		::DFN_WritePrivateProfileString((LPCTSTR)strSection, "ifOutErrors", (LPCTSTR)strMsg, ciniFile);
	}
	else
	{
		float ifInDiscardsRate = 0.0, ifOutDiscardsRate = 0.0;
		float ifInErrorsRate = 0.0, ifOutErrorsRate = 0.0;
		Counter	InDiscards = 0, OutDiscards = 0;
		Counter InErrors = 0, OutErrors = 0;

		CTime bTime(ifTime);
		curTime = CTime::GetCurrentTime();
		CTimeSpan spanTime = curTime - bTime;
		long lInterval = spanTime.GetTotalSeconds();
		
		if(lInterval == 0) lInterval = 1;
		if(lInterval <= 12 * 60 * 60 && lInterval > 0)
		{
			InDiscards = (unsigned int)(ifInDiscards - inDiscards < 0 ? (COUNTER_MAX - inDiscards + ifInDiscards) : (ifInDiscards - inDiscards));
			ifInDiscardsRate = (float)InDiscards / lInterval;

			OutDiscards = (unsigned int)(ifOutDiscards - outDiscards < 0 ? (COUNTER_MAX - outDiscards + ifOutDiscards) : (ifOutDiscards - outDiscards));
			ifOutDiscardsRate = (float)OutDiscards / lInterval;

			InErrors = (unsigned int)(ifInErrors - inErrors < 0 ? (COUNTER_MAX - inErrors + ifInErrors) : (ifInErrors - inErrors));
			ifInErrorsRate = (float)InErrors / lInterval;

			OutErrors = (unsigned int)(ifOutErrors - outErrors < 0 ? (COUNTER_MAX - outErrors + ifOutErrors) : (ifOutErrors - outErrors));
			ifOutErrorsRate = (float)OutErrors / lInterval;

			sprintf(szReturn, 
					"ifInDiscardsRate=%.2f$ifOutDiscardsRate=%.2f$ifInErrorsRate=%.2f$ifOutErrorsRate=%.2f$",
					ifInDiscardsRate * 8, 
					ifOutDiscardsRate * 8, 
					ifInErrorsRate * 8, 
					ifOutErrorsRate * 8);
		}

		char buff[256] = {0};
		memset(buff, 0, 256);
		ltoa(curTime.GetTime(), buff, 10);
		DFN_WritePrivateProfileString(cifIndex, "ifTime", buff, ciniFile);
		memset(buff, 0, 256);
		ltoa(ifInDiscards, buff, 10);
		DFN_WritePrivateProfileString(cifIndex, "ifInDiscards", buff, ciniFile);
		memset(buff, 0, 256);
		ltoa(ifOutDiscards, buff, 10);
		DFN_WritePrivateProfileString(cifIndex, "ifOutDiscards", buff, ciniFile);
		memset(buff, 0, 256);
		ltoa(ifInErrors, buff, 10);
		DFN_WritePrivateProfileString(cifIndex, "ifInErrors", buff, ciniFile);
		memset(buff, 0, 256);
		ltoa(ifOutErrors, buff, 10);
		DFN_WritePrivateProfileString(cifIndex, "ifOutErrors", buff, ciniFile);

	}
	}

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
