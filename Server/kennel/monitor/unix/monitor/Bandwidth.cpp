
#include "Bandwidth.h"
#include "SnmpUtils.h"

#include "base\funcGeneral.h"

struct oid ifSpeed		= {{0x2b, 6, 1, 2, 1, 2, 2, 1, 5},  9, "ifSpeed",	  ASN1_GAUGE,   ""};
struct oid ifInOctets	= {{0x2b, 6, 1, 2, 1, 2, 2, 1, 10}, 9, "ifInOctets",  ASN1_COUNTER, ""};
struct oid ifOutOctets	= {{0x2b, 6, 1, 2, 1, 2, 2, 1, 16}, 9, "ifOutOctets", ASN1_COUNTER, ""};

/*******************************************************************************
 *	For half-duplex: 
 *								  (¦¤ifInOctets + ¦¤ifOutOctets) * 8 * 100
 *		Bandwidth Utilization	= -----------------------------------------
 *												  ¦¤nSeconds
 *
 *	For full-duplex: 
 *								  max(¦¤ifInOctets, ¦¤ifOutOctets) * 8 * 100
 *		Bandwidth Utilization	= -------------------------------------------
 *												  ¦¤nSeconds
 *
 *	Or
 *								  ¦¤ifInOctets * 8 * 100
 *		Input Utilization		= -----------------------
 *										¦¤nSeconds
 *
 *								  ¦¤OutInOctets * 8 * 100
 *		Output Utilization		= -----------------------
 *										¦¤nSeconds
 */

BOOL BANDWIDTH_MONITOR(char *server, int port, char *community, int index, 
					   int rdelay, int timeout, char *cus_path, char *szReturn)
{
	int s;
	struct sockaddr_in serv;

	int i, ret = 0;
	int count = 1;
	struct snval *ptmp = NULL;
	struct snval *ptmpnext = NULL;

	if((ret = MakeSocket(server, port, s, serv)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_1%>"));//<%IDS_Dynamo_1%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_2%>"));//<%IDS_Dynamo_2%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_3%>"));//<%IDS_Dynamo_3%>

		return FALSE;
	}

	ifSpeed.id[ifSpeed.len ++] = index;
	ifInOctets.id[ifInOctets.len ++] = index;
	ifOutOctets.id[ifOutOctets.len ++] = index;

	struct oid *poid = NULL, *ptmpoid = NULL;
	poid = ptmpoid = &ifSpeed;
	ptmpoid->next = &ifInOctets;
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &ifOutOctets;
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
			sprintf(szReturn, "%s=", "ifSpeed");
			break;
		case 2:
			sprintf(szReturn, "%s%s=", szReturn, "ifInOctets");
			break;
		case 3:
			sprintf(szReturn, "%s%s=", szReturn, "ifOutOctets");
			break;
		}

		switch(ptmp->sv_type)
		{
		case ASN1_SEQ:
			break;
		case ASN1_INT:
			sprintf(szReturn, "%s%d$", szReturn, ptmp->sv_val.sv_int);
			break;
		case ASN1_OCTSTR:
			sprintf(szReturn, "%s%s$", szReturn, ptmp->sv_val.sv_str.sv_str);
			break;
		case ASN1_NULL:
			sprintf(szReturn, "%s%s$", szReturn, ptmp->sv_val.sv_str.sv_str);
			break;
		case ASN1_OBJID:
			for(i = 0;i < ptmp->sv_val.sv_oid.len;i ++)
				if(i == 0)
					sprintf(szReturn, "%s%s", szReturn, "1.3.");
				else if(i == ptmp->sv_val.sv_oid.len - 1)
					sprintf(szReturn, "%s%d$", szReturn, ptmp->sv_val.sv_oid.id[i]);
				else
					sprintf(szReturn, "%s%d.", szReturn, ptmp->sv_val.sv_oid.id[i]);
			break;
		case ASN1_IPADDR:
			sprintf(szReturn, "%s%d.%d.%d.%d$", szReturn, 
					ptmp->sv_val.sv_ipaddr[0], ptmp->sv_val.sv_ipaddr[1], 
					ptmp->sv_val.sv_ipaddr[2], ptmp->sv_val.sv_ipaddr[3]);
			break;
		case ASN1_COUNTER:
		case ASN1_GAUGE:
		case ASN1_TIMETICKS:
		case ASN1_COUNTER64:
		case ASN1_UINTEGER32:
			sprintf(szReturn, "%s%u$", szReturn, ptmp->sv_val.sv_int);
			break;
		}

		count ++;
		ptmp = ptmp->next;
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
