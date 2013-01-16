
#include "CheckPoint.h"
#include "SnmpUtils.h"

#include "base\funcGeneral.h"

#define		CHECKPOINT_VARS			8
struct oid checkpoint_oids[CHECKPOINT_VARS] = {
	/* The state of the fw module. */
	{{0x2b, 6, 1, 4, 1, 2620, 1, 1, 1}, 9, "ModuleState", ASN1_OCTSTR, ""}, 
	/* The number of rejected  packets. */
	{{0x2b, 6, 1, 4, 1, 2620, 1, 1, 5}, 9, "Rejected", ASN1_INT, ""}, 
	/* The number of dropped packets. */
	{{0x2b, 6, 1, 4, 1, 2620, 1, 1, 6}, 9, "Dropped", ASN1_INT, ""}, 
	/* The number of logged packets. */
	{{0x2b, 6, 1, 4, 1, 2620, 1, 1, 7}, 9, "Logged", ASN1_INT, ""}, 
	/* FireWall-1 Major Version. */
	{{0x2b, 6, 1, 4, 1, 2620, 1, 1, 8}, 9, "Major", ASN1_INT, ""}, 
	/* FireWall-1 Minor Version. */
	{{0x2b, 6, 1, 4, 1, 2620, 1, 1, 9}, 9, "Minor", ASN1_INT, ""}, 
	/* FireWall-1 Product. */
	{{0x2b, 6, 1, 4, 1, 2620, 1, 1, 10}, 9, "Product", ASN1_INT, ""}, 
	/* A string containing the last snmp trap sent via fw */
	{{0x2b, 6, 1, 4, 1, 2620, 1, 1, 11}, 9, "PointEvent", ASN1_OCTSTR, ""}
};

BOOL CHECKPOINT_MONITOR(char *server, int port, char *community, CString strIndex, 
						int retrydelay, int timeout, char *custpath, char *szReturn)
{
	int s;
	struct sockaddr_in serv;

	int i = 0, j = 0, ret = 0;
	int count = 1;
	struct snval *ptmp = NULL;
	struct snval *ptmpnext = NULL;

	if((ret = MakeSocket(server, port, s, serv)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CheckPoint_1%>"));//<%IDS_CheckPoint_1%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CheckPoint_2%>"));//<%IDS_CheckPoint_2%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CheckPoint_3%>"));//<%IDS_CheckPoint_3%>

		return FALSE;
	}

	long index_id[32];
	int	 index_id_c = 0;
	while(1)
	{
		i = strIndex.Find('.', 0);
		if(i > 0)
			index_id[index_id_c++] = atoi(strIndex);
		else if(i < 0 && strIndex.GetLength() > 0)
		{
			index_id[index_id_c++] = atoi(strIndex);
			break;
		}
		else
			break;

		strIndex.Delete(0, i + 1);
	}

	for(i = 0;i < CHECKPOINT_VARS;i ++)
		for(j = 0;j < index_id_c;j ++)
			checkpoint_oids[i].id[checkpoint_oids[i].len++] = index_id[j];

	struct oid *poid = NULL, *ptmpoid = NULL;
	poid = ptmpoid = &checkpoint_oids[1];
	ptmpoid->next = &checkpoint_oids[2];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &checkpoint_oids[3];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &checkpoint_oids[4];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &checkpoint_oids[5];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &checkpoint_oids[6];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &checkpoint_oids[7];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &checkpoint_oids[0];
	ptmpoid = ptmpoid->next;

	struct snval *psnval = NULL;
	if((ret = SN_VALUES(s, serv, community, 1, poid, &psnval)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CheckPoint_4%>"));//<%IDS_CheckPoint_4%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CheckPoint_5%>"));//<%IDS_CheckPoint_5%>
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CheckPoint_6%>"));//<%IDS_CheckPoint_6%>
		else if(ret == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CheckPoint_7%>"));//<%IDS_CheckPoint_7%>
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CheckPoint_8%>"));//<%IDS_CheckPoint_8%>
		else if(ret == -6)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CheckPoint_9%>"));//<%IDS_CheckPoint_9%>
		else if(ret == -7)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CheckPoint_10%>"));//<%IDS_CheckPoint_10%>
		else if(ret == -8)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CheckPoint_11%>"));//<%IDS_CheckPoint_11%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CheckPoint_12%>"));//<%IDS_CheckPoint_12%>

		goto w;
	}

	ptmp = psnval;
	ptmpnext = NULL;
	while(ptmp)
	{
		switch(count)
		{
		case 1:
			sprintf(szReturn, "%s=", "Rejected");
			break;
		case 2:
			sprintf(szReturn, "%s%s=", szReturn, "Dropped");
			break;
		case 3:
			sprintf(szReturn, "%s%s=", szReturn, "Logged");
			break;
		case 4:
			sprintf(szReturn, "%s%s=", szReturn, "Major");
			break;
		case 5:
			sprintf(szReturn, "%s%s=", szReturn, "Minor");
			break;
		case 6:
			sprintf(szReturn, "%s%s=", szReturn, "Product");
			break;
		case 7:
			sprintf(szReturn, "%s%s=", szReturn, "PointEvent");
			break;
		case 8:
			sprintf(szReturn, "%s%s=", szReturn, "ModuleState");
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
