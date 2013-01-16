
#include "F5.h"
#include "SnmpUtils.h"

#include "..\..\base\funcGeneral.h"

#define		LOADBAL_VARS			8
struct oid f5_oids[LOADBAL_VARS] = {
	/* The uptime of this device -- not the uptime of the SNMP agent. */
	{{0x2b, 6, 1, 4, 1, 3375, 1, 1, 50}, 9, "uptime", ASN1_TIMETICKS, ""}, 
	/* The number of current connections. */
	{{0x2b, 6, 1, 4, 1, 3375, 1, 1, 52}, 9, "concur", ASN1_INT, ""}, 
	/* The total count of all packets read off the network. */
	{{0x2b, 6, 1, 4, 1, 3375, 1, 1, 54}, 9, "pktsin", ASN1_COUNTER, ""}, 
	/* The total count of all packets written to the network. */
	{{0x2b, 6, 1, 4, 1, 3375, 1, 1, 55}, 9, "pktsout", ASN1_COUNTER, ""}, 
	/* The count of port denies that has occuried. */
	{{0x2b, 6, 1, 4, 1, 3375, 1, 1, 58}, 9, "portdeny", ASN1_COUNTER, ""}, 
	/* The count of inbound packets that have been dropped. */
	{{0x2b, 6, 1, 4, 1, 3375, 1, 1, 59}, 9, "droppedin", ASN1_COUNTER, ""}, 
	/* The count of outbound packets that have been dropped. */
	{{0x2b, 6, 1, 4, 1, 3375, 1, 1, 60}, 9, "droppedout", ASN1_COUNTER, ""}, 
	/* Memory used by the load balancer kernel. */
	{{0x2b, 6, 1, 4, 1, 3375, 1, 1, 77}, 9, "memoryUsed", ASN1_INT, ""}, 
};

BOOL F5_MONITOR(char *server, int port, char *community, CString strIndex, int rdelay, 
				int timeout, char *custpath, char *szReturn)
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
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_F5_1%>"));//<%IDS_F5_1%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_F5_2%>"));//<%IDS_F5_2%>
		else
			sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_F5_3%>"));//<%IDS_F5_3%>

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

	for(i = 0;i < LOADBAL_VARS;i ++)
		for(j = 0;j < index_id_c;j ++)
			f5_oids[i].id[f5_oids[i].len++] = index_id[j];

	struct oid *poid = NULL, *ptmpoid = NULL;
	poid = ptmpoid = &f5_oids[0];
	ptmpoid->next = &f5_oids[1];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &f5_oids[2];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &f5_oids[3];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &f5_oids[4];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &f5_oids[5];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &f5_oids[6];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &f5_oids[7];
	ptmpoid = ptmpoid->next;

	struct snval *psnval = NULL;
	if((ret = SN_VALUES(s, serv, community, 1, poid, &psnval)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_F5_4%>"));//<%IDS_F5_4%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_F5_5%>"));//<%IDS_F5_5%>
		else if(ret == -3)
			sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_F5_6%>"));//<%IDS_F5_6%>
		else if(ret == -4)
			sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_F5_7%>"));//<%IDS_F5_7%>
		else if(ret == -5)
			sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_F5_8%>"));//<%IDS_F5_8%>
		else if(ret == -6)
			sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_F5_9%>"));//<%IDS_F5_9%>
		else if(ret == -7)
			sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_F5_10%>"));//<%IDS_F5_10%>
		else if(ret == -8)
			sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_F5_11%>"));//<%IDS_F5_11%>
		else
			sprintf(szReturn, "error=%s",  FuncGetStringFromIDS("<%IDS_F5_12%>"));//<%IDS_F5_12%>

		goto w;
	}

	ptmp = psnval;
	ptmpnext = NULL;
	while(ptmp)
	{
		switch(count)
		{
		case 1:
			sprintf(szReturn, "%s=", "uptime");
			break;
		case 2:
			sprintf(szReturn, "%s%s=", szReturn, "concur");
			break;
		case 3:
			sprintf(szReturn, "%s%s=", szReturn, "pktsin");
			break;
		case 4:
			sprintf(szReturn, "%s%s=", szReturn, "pktsout");
			break;
		case 5:
			sprintf(szReturn, "%s%s=", szReturn, "portdeny");
			break;
		case 6:
			sprintf(szReturn, "%s%s=", szReturn, "droppedin");
			break;
		case 7:
			sprintf(szReturn, "%s%s=", szReturn, "droppedout");
			break;
		case 8:
			sprintf(szReturn, "%s%s=", szReturn, "memoryUsed");
			break;
		}

		switch(ptmp->sv_type)
		{
		case ASN1_INT:
			sprintf(szReturn, "%s%d$", szReturn, ptmp->sv_val.sv_int);
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