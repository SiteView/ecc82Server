
#include "NetScreen.h"
#include "SnmpUtils.h"

#include "base\funcGeneral.h"



#define		NETSCREENRES_VARS			11
struct oid NS_res_oids[NETSCREENRES_VARS] = {
	/* Average System CPU utilization in percentage. */
	{{0x2b, 6, 1, 4, 1, 3224, 16, 1, 1}, 9, "nsResCpuAvg", ASN1_INT, ""}, 
	/* Last one minute CPU utilization in percentage. */
	{{0x2b, 6, 1, 4, 1, 3224, 16, 1, 2}, 9, "nsResCpuLast1Min", ASN1_INT, ""}, 
	/* Last five minutes CPU utilization in percentage. */
	{{0x2b, 6, 1, 4, 1, 3224, 16, 1, 3}, 9, "nsResCpuLast5Min", ASN1_INT, ""}, 
	/* Last fifteen minutes CPU utilization in percentage. */
	{{0x2b, 6, 1, 4, 1, 3224, 16, 1, 4}, 9, "nsResCpuLast15Min", ASN1_INT, ""}, 

	/* Memory allocated. */
	{{0x2b, 6, 1, 4, 1, 3224, 16, 2, 1}, 9, "nsResMemAllocate", ASN1_INT, ""}, 
	/* Memory left. */
	{{0x2b, 6, 1, 4, 1, 3224, 16, 2, 2}, 9, "nsResMemLeft", ASN1_INT, ""}, 
	/* Memory fragment. */
	{{0x2b, 6, 1, 4, 1, 3224, 16, 2, 3}, 9, "nsResMemFrag", ASN1_INT, ""}, 

	/* Active session number. */
	{{0x2b, 6, 1, 4, 1, 3224, 16, 3, 1}, 9, "nsResSessActive", ASN1_INT, ""}, 
	/* Allocate session number. */
	{{0x2b, 6, 1, 4, 1, 3224, 16, 3, 2}, 9, "nsResSessAllocate", ASN1_INT, ""}, 
	/* Maxium session number system can afford. */
	{{0x2b, 6, 1, 4, 1, 3224, 16, 3, 3}, 9, "nsResSessMaxium", ASN1_INT, ""}, 
	/* Failed session allocation counters. */
	{{0x2b, 6, 1, 4, 1, 3224, 16, 3, 4}, 9, "nsResSessFailed", ASN1_INT, ""}
};

//bool WriteLog(CString strFileName, const CString strLog);

BOOL NETSCREENRES_MONITOR(char *server, int port, char *community, CString strIndex, int retrydealy, 
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
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_1%>"));//<%IDS_Dynamo_1%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_2%>"));//<%IDS_Dynamo_2%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_3%>"));//<%IDS_Dynamo_3%>

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

	for(i = 0;i < NETSCREENRES_VARS;i ++)
		for(j = 0;j < index_id_c;j ++)
			NS_res_oids[i].id[NS_res_oids[i].len++] = index_id[j];

	struct oid *poid = NULL, *ptmpoid = NULL;
	poid = ptmpoid = &NS_res_oids[0];
	ptmpoid->next = &NS_res_oids[4];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_res_oids[5];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_res_oids[7];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_res_oids[8];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_res_oids[9];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_res_oids[10];
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
			sprintf(szReturn, "%s=", "nsResCpuAvg");
			break;
		case 2:
			sprintf(szReturn, "%s%s=", szReturn, "nsResMemAllocate");
			break;
		case 3:
			sprintf(szReturn, "%s%s=", szReturn, "nsResMemLeft");
			break;
		case 4:
			sprintf(szReturn, "%s%s=", szReturn, "nsResSessActive");
			break;
		case 5:
			sprintf(szReturn, "%s%s=", szReturn, "nsResSessAllocate");
			break;
		case 6:
			sprintf(szReturn, "%s%s=", szReturn, "nsResSessMaxium");
			break;
		case 7:
			sprintf(szReturn, "%s%s=", szReturn, "nsResSessFailed");
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

#define		NETSCREENSERVICE_VARS			10

enum ServiceCategoryE
{
	SERVICECATEGORY_REMOTE = 1, 
    SERVICECATEGORY_EMAIL = 2, 
    SERVICECATEGORY_INFOSEEK = 3, 
    SERVICECATEGORY_SECURITY = 4, 
    SERVICECATEGORY_OTHER = 5 
};

enum ServiceTransProtoE
{
	SERVICETRANSPROTO_TCP = 7, 
	SERVICETRANSPROTO_UDP = 16 
};

enum ServiceFlagE
{
	PRE_DEFINE = 0, 
    USR_DEFINE = 1 
};

struct oid NS_service_oids[NETSCREENSERVICE_VARS] = {
	/* A unique value for each address. Its value ranges between 0 and 65535 and 
	   may not be contingous. */
	{{0x2b, 6, 1, 4, 1, 3224, 13, 1, 1, 1}, 10, "nsServiceIndex", ASN1_INT, ""}, 
	/* Service name. */
	{{0x2b, 6, 1, 4, 1, 3224, 13, 1, 1, 2}, 10, "nsServiceName", ASN1_OCTSTR, ""}, 
	/* Category this service belongs to. */
	{{0x2b, 6, 1, 4, 1, 3224, 13, 1, 1, 3}, 10, "nsServiceCategory", ASN1_INT, ""}, 
	/* Service trans protocol. 6 means tcp; 17 means udp */
	{{0x2b, 6, 1, 4, 1, 3224, 13, 1, 1, 4}, 10, "nsServiceTransProto", ASN1_INT, ""}, 
	/* The low source port number associateing with service. */
	{{0x2b, 6, 1, 4, 1, 3224, 13, 1, 1, 5}, 10, "nsServiceSrcPortLow", ASN1_INT, ""}, 
	/* The high source port number associateing with service. */
	{{0x2b, 6, 1, 4, 1, 3224, 13, 1, 1, 6}, 10, "nsServiceSrcPortHigh", ASN1_INT, ""}, 
	/* The low destination port number associateing with service. */
	{{0x2b, 6, 1, 4, 1, 3224, 13, 1, 1, 7}, 10, "nsServiceDstPortLow", ASN1_INT, ""}, 
	/* The high source port number associateing with service. */
	{{0x2b, 6, 1, 4, 1, 3224, 13, 1, 1, 8}, 10, "nsServiceDstPortHigh", ASN1_INT, ""}, 
	/* Service flag used to indicate if the service is a pre-defined one or a 
	   custom one. */
	{{0x2b, 6, 1, 4, 1, 3224, 13, 1, 1, 9}, 10, "nsServiceFlag", ASN1_INT, ""}, 
	/* Virtual system this configuration belongs to. */
	{{0x2b, 6, 1, 4, 1, 3224, 13, 1, 1, 10}, 10, "nsServiceVsys", ASN1_INT, ""}
};

BOOL NETSCREENSERVICE_MONITOR(char *server, int port, char *community, CString strIndex, int retrydealy, 
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
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_1%>"));//<%IDS_Dynamo_1%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_2%>"));//<%IDS_Dynamo_2%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_3%>"));//<%IDS_Dynamo_3%>

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

	for(i = 0;i < NETSCREENSERVICE_VARS;i ++)
		for(j = 0;j < index_id_c;j ++)
			NS_service_oids[i].id[NS_service_oids[i].len++] = index_id[j];

	struct oid *poid = NULL, *ptmpoid = NULL;
	poid = ptmpoid = &NS_service_oids[0];
	ptmpoid->next = &NS_service_oids[1];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_service_oids[2];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_service_oids[3];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_service_oids[4];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_service_oids[5];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_service_oids[6];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_service_oids[7];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_service_oids[8];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_service_oids[9];
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
			sprintf(szReturn, "%s=", "nsServiceIndex");
			break;
		case 2:
			sprintf(szReturn, "%s%s=", szReturn, "nsServiceName");
			break;
		case 3:
			sprintf(szReturn, "%s%s=", szReturn, "nsServiceCategory");
			break;
		case 4:
			sprintf(szReturn, "%s%s=", szReturn, "nsServiceTransProto");
			break;
		case 5:
			sprintf(szReturn, "%s%s=", szReturn, "nsServiceSrcPortLow");
			break;
		case 6:
			sprintf(szReturn, "%s%s=", szReturn, "nsServiceSrcPortHigh");
			break;
		case 7:
			sprintf(szReturn, "%s%s=", szReturn, "nsServiceDstPortLow");
			break;
		case 8:
			sprintf(szReturn, "%s%s=", szReturn, "nsServiceDstPortHigh");
			break;
		case 9:
			sprintf(szReturn, "%s%s=", szReturn, "nsServiceFlag");
			break;
		case 10:
			sprintf(szReturn, "%s%s=", szReturn, "nsServiceVsys");
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

#define		NETSCREENSERVICEGROUP_VARS			5
struct oid NS_servicegroup_oids[NETSCREENSERVICEGROUP_VARS] = {
	/* A unique value for each group. Its value ranges between 0 and 65535 and may 
	   not be contingous. */
	{{0x2b, 6, 1, 4, 1, 3224, 13, 2, 1, 1}, 10, "nsServiceGroupIndex", ASN1_INT, ""}, 
	{{0x2b, 6, 1, 4, 1, 3224, 13, 2, 1, 2}, 10, "nsServiceGroupName", ASN1_OCTSTR, ""}, 
	{{0x2b, 6, 1, 4, 1, 3224, 13, 2, 1, 3}, 10, "nsServiceGroupMember", ASN1_INT, ""}, 
	{{0x2b, 6, 1, 4, 1, 3224, 13, 2, 1, 4}, 10, "nsServiceGroupComment", ASN1_OCTSTR, ""}, 
	{{0x2b, 6, 1, 4, 1, 3224, 13, 2, 1, 5}, 10, "nsServiceGroupVsys", ASN1_OCTSTR, ""}
};

BOOL NETSCREENSERVICEGROUP_MONITOR(char *server, int port, char *community, CString strIndex, int retrydealy, 
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
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_1%>"));//<%IDS_Dynamo_1%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_2%>"));//<%IDS_Dynamo_2%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_3%>"));//<%IDS_Dynamo_3%>

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

	for(i = 0;i < NETSCREENSERVICEGROUP_VARS;i ++)
		for(j = 0;j < index_id_c;j ++)
			NS_servicegroup_oids[i].id[NS_servicegroup_oids[i].len++] = index_id[j];

	struct oid *poid = NULL, *ptmpoid = NULL;
	poid = ptmpoid = &NS_servicegroup_oids[0];
	ptmpoid->next = &NS_servicegroup_oids[1];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_servicegroup_oids[2];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_servicegroup_oids[3];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_servicegroup_oids[4];
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
			sprintf(szReturn, "%s=", "nsServiceGroupIndex");
			break;
		case 2:
			sprintf(szReturn, "%s%s=", szReturn, "nsServiceGroupName");
			break;
		case 3:
			sprintf(szReturn, "%s%s=", szReturn, "nsServiceGroupMember");
			break;
		case 4:
			sprintf(szReturn, "%s%s=", szReturn, "nsServiceGroupComment");
			break;
		case 5:
			sprintf(szReturn, "%s%s=", szReturn, "nsServiceGroupVsys");
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

#define		NETSCREENSERVICEGROUPMEMBER_VARS			4
struct oid NS_servicegroupmember_oids[NETSCREENSERVICEGROUPMEMBER_VARS] = {
	/* A unique value for each group. Its value ranges between 0 and 65535 and may 
	   not be contingous. */
	{{0x2b, 6, 1, 4, 1, 3224, 13, 3, 1, 1}, 10, "nsServiceGrpMemberIndex", ASN1_INT, ""}, 
	{{0x2b, 6, 1, 4, 1, 3224, 13, 3, 1, 2}, 10, "nsServiceGrpName", ASN1_OCTSTR, ""}, 
	{{0x2b, 6, 1, 4, 1, 3224, 13, 3, 1, 3}, 10, "nsServiceGroupMemberName", ASN1_OCTSTR, ""}, 
	{{0x2b, 6, 1, 4, 1, 3224, 13, 3, 1, 4}, 10, "nsServiceGroupMemberVsys", ASN1_OCTSTR, ""}, 
};

BOOL NETSCREENSERVICEGROUPMEMBER_MONITOR(char *server, int port, char *community, CString strIndex, int retrydealy, 
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
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_1%>"));//<%IDS_Dynamo_1%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_2%>"));//<%IDS_Dynamo_2%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_3%>"));//<%IDS_Dynamo_3%>

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

	for(i = 0;i < NETSCREENSERVICEGROUPMEMBER_VARS;i ++)
		for(j = 0;j < index_id_c;j ++)
			NS_servicegroupmember_oids[i].id[NS_servicegroupmember_oids[i].len++] = index_id[j];

	struct oid *poid = NULL, *ptmpoid = NULL;
	poid = ptmpoid = &NS_servicegroupmember_oids[0];
	ptmpoid->next = &NS_servicegroupmember_oids[1];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_servicegroupmember_oids[2];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_servicegroupmember_oids[3];
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
			sprintf(szReturn, "%s=", "nsServiceGrpMemberIndex");
			break;
		case 2:
			sprintf(szReturn, "%s%s=", szReturn, "nsServiceGrpName");
			break;
		case 3:
			sprintf(szReturn, "%s%s=", szReturn, "nsServiceGroupMemberName");
			break;
		case 4:
			sprintf(szReturn, "%s%s=", szReturn, "nsServiceGroupMemberVsys");
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

#define		NETSCREENVPN_VARS			28
struct oid NS_vpn_oids[NETSCREENVPN_VARS] = {
	/* A unique value for each active VPN tunnel.  Its value
       ranges between 1 and 65535 and may not be contigous. 
	   Due to the dynamic nature of active VPN tunnels, the index 
	   has no other meaning but a pure index */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 1}, 11, "nsVpnMonIndex", ASN1_INT, ""}, 
	/* The incoming policy ID for which this tunnel is created for.
       -1 means no policy associates with this SA. */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 2}, 11, "nsVpnMonInPlyId", ASN1_INT, ""}, 
	/* The outgoing policy ID for which this tunnel is created for.
       -1 means no policy associates with this SA. */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 3}, 11, "nsVpnMonOutPlyId", ASN1_INT, ""}, 
	/* A textual string containing information about the
       VPN entity from which this tunnel was derived. */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 4}, 11, "nsVpnMonVpnName", ASN1_OCTSTR, ""}, 
	/* A textual string containing information about the
       Virtual system to which this tunnel belongs. This applies
	   to NetScreen 1000 */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 5}, 11, "nsVpnMonVsysName", ASN1_OCTSTR, ""}, 
	/* Protocol type used for the tunnel */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 6}, 11, "nsVpnMonTunnelType", ASN1_INT, ""}, 
	/* a value which identifies a particular algorithm to be
       used to provide secrecy protection for ESP. */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 7}, 11, "nsVpnMonEspEncAlg", ASN1_INT, ""}, 
	/* The ESP Authentication Algorithm used in the IPsec. */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 8}, 11, "nsVpnMonEspAuthAlg", ASN1_INT, ""}, 
	/* a value which identifies a particular algorithm to be
       used to provide integrity protection for AH. */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 9}, 11, "nsVpnMonAhAlg", ASN1_INT, ""}, 
	/* a value which identifies a key exchange protocol
       to be used for the negotiation */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 10}, 11, "nsVpnMonKeyType", ASN1_INT, ""}, 
	/* a value which identifies Phase 1 authentication method */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 11}, 11, "nsVpnMonP1Auth", ASN1_INT, ""}, 
	/* The type of this VPN tunnel, either a dialup or site-to-site */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 12}, 11, "nsVpnMonVpnType", ASN1_INT, ""}, 
	/* The peer Gateway's IP address */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 13}, 11, "nsVpnMonRmtGwIp", ASN1_IPADDR, ""}, 
	/* The peer Gateway's ID */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 14}, 11, "nsVpnMonRmtGwId", ASN1_OCTSTR, ""}, 
	/* The local Gateway's IP address */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 15}, 11, "nsVpnMonMyGwIp", ASN1_IPADDR, ""}, 
	/* The local Gateway's ID */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 16}, 11, "nsVpnMonMyGwId", ASN1_OCTSTR, ""}, 
	/* The SPI for outgoing packets */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 17}, 11, "nsVpnMonOutSpi", ASN1_INT, ""}, 
	/* The SPI for incoming packets */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 18}, 11, "nsVpnMonInSpi", ASN1_INT, ""}, 
	/* The monitoring status, if it is on, an icmp ping will be sent over
	   the tunnel periodically to test the conntivity and latency */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 19}, 11, "nsVpnMonMonState", ASN1_INT, ""}, 
	/* The current tunnel status determined by the icmp ping 
	   if The monitoring status is on. */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 20}, 11, "nsVpnMonTunnelState", ASN1_INT, ""}, 
	/* The IKE's Phase 1 status */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 21}, 11, "nsVpnMonP1State", ASN1_INT, ""}, 
	/* an active Phase 1 sa's time left before re-key.
	   -1 means unlimited lifetime. */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 22}, 11, "nsVpnMonP1LifeTime", ASN1_INT, ""}, 
	/* The IKE's Phase 2 status */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 23}, 11, "nsVpnMonP2State", ASN1_INT, ""}, 
	/* an active Phase 2 sa's time left before re-key.
	   -1 means unlimited life time. */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 24}, 11, "nsVpnMonP2LifeTime", ASN1_INT, ""}, 
	/* an active Phase 2 sa's bytes left before re-key.
	   -1 means unlimited life bytes. */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 25}, 11, "nsVpnMonP2LifeBytes", ASN1_INT, ""}, 
	/* a kind of rolling average of latency, in milliseconds.
	   -1 has no meaning here, which means nsVpnMonDelayAvg 
       has not been calculated yet. */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 26}, 11, "nsVpnMonDelayAvg", ASN1_INT, ""}, 
	/* latency in last sample, in milliseconds.
	   -1 means either vpn tunnel is inactive or vpn tunnel
	   monitor is not turned on. */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 27}, 11, "nsVpnMonDelayLast", ASN1_INT, ""}, 
	/* percentage over 30 samples */
	{{0x2b, 6, 1, 4, 1, 3224, 4, 1, 1, 1, 28}, 11, "nsVpnMonAvail", ASN1_INT, ""}
};

BOOL NETSCREENVPN_MONITOR(char *server, int port, char *community, CString strIndex, int retrydealy, 
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
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_1%>"));//<%IDS_Dynamo_1%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_2%>"));//<%IDS_Dynamo_2%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_3%>"));//<%IDS_Dynamo_3%>

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

	for(i = 0;i < NETSCREENVPN_VARS;i ++)
		for(j = 0;j < index_id_c;j ++)
			NS_vpn_oids[i].id[NS_vpn_oids[i].len++] = index_id[j];

	struct oid *poid = NULL, *ptmpoid = NULL;
	poid = ptmpoid = &NS_vpn_oids[1];
	ptmpoid->next = &NS_vpn_oids[2];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_vpn_oids[3];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_vpn_oids[4];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_vpn_oids[16];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_vpn_oids[17];
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
			sprintf(szReturn, "%s=", "nsVpnMonInPlyId");
			break;
		case 2:
			sprintf(szReturn, "%s%s=", szReturn, "nsVpnMonOutPlyId");
			break;
		case 3:
			sprintf(szReturn, "%s%s=", szReturn, "nsVpnMonVpnName");
			break;
		case 4:
			sprintf(szReturn, "%s%s=", szReturn, "nsVpnMonVsysName");
			break;
		case 5:
			sprintf(szReturn, "%s%s=", szReturn, "nsVpnMonOutSpi");
			break;
		case 6:
			sprintf(szReturn, "%s%s=", szReturn, "nsVpnMonInSpi");
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

#define		NETSCREENATTACK_VARS			13
struct oid NS_attack_oids[NETSCREENATTACK_VARS] = {
	/* unique interface id.	*/
	{{0x2b, 6, 1, 4, 1, 3224, 3, 2, 1, 1}, 10, "nsIdsAttkMonIfIdx", ASN1_INT, ""}, 
	/* according NetScreen's concepts. each interface belongs to one virtual
       system. This attribute displays the virtual system name an interface
       belongs to. */
	{{0x2b, 6, 1, 4, 1, 3224, 3, 2, 1, 2}, 10, "nsIdsAttkMonVsys", ASN1_INT, ""}, 
	/* sync attack packet counter. */
	{{0x2b, 6, 1, 4, 1, 3224, 3, 2, 1, 3}, 10, "nsIdsAttkMonSynAttk", ASN1_INT, ""}, 
	/* tear drop attack packet counter. */
	{{0x2b, 6, 1, 4, 1, 3224, 3, 2, 1, 4}, 10, "nsIdsAttkMonTearDrop", ASN1_INT, ""}, 
	/* source route option attack packet counter. */
	{{0x2b, 6, 1, 4, 1, 3224, 3, 2, 1, 5}, 10, "nsIdsAttkMonSrcRoute", ASN1_INT, ""}, 
	/* ping of death attack packet counter. */
	{{0x2b, 6, 1, 4, 1, 3224, 3, 2, 1, 6}, 10, "nsIdsAttkMonPingDeath", ASN1_INT, ""}, 
	/* address spoofing attack packet counter. */
	{{0x2b, 6, 1, 4, 1, 3224, 3, 2, 1, 7}, 10, "nsIdsAttkMonAddrSpoof", ASN1_INT, ""}, 
	/* land attack packet counter. */
	{{0x2b, 6, 1, 4, 1, 3224, 3, 2, 1, 8}, 10, "nsIdsAttkMonLand", ASN1_INT, ""}, 
	/* icmp flood attack packet counter. */
	{{0x2b, 6, 1, 4, 1, 3224, 3, 2, 1, 9}, 10, "nsIdsAttkMonIcmpFlood", ASN1_INT, ""}, 
	/* udp flood attack packet counter. */
	{{0x2b, 6, 1, 4, 1, 3224, 3, 2, 1, 10}, 10, "nsIdsAttkMonUdpFlood", ASN1_INT, ""}, 
	/* weired netbios attack packet counter. */
	{{0x2b, 6, 1, 4, 1, 3224, 3, 2, 1, 11}, 10, "nsIdsAttkMonWinnuke", ASN1_INT, ""}, 
	/* port scan attempt attack packet counter. */
	{{0x2b, 6, 1, 4, 1, 3224, 3, 2, 1, 12}, 10, "nsIdsAttkMonPortScan", ASN1_INT, ""}, 
	/* address sweep attemp attack packet counter. */
	{{0x2b, 6, 1, 4, 1, 3224, 3, 2, 1, 13}, 10, "nsIdsAttkMonIpSweep", ASN1_INT, ""}
};

BOOL NETSCREENATTACK_MONITOR(char *server, int port, char *community, CString strIndex, int retrydealy, 
							 int timeout, char *custpath, char *szReturn)
{
	int s;
	struct sockaddr_in serv;

	int i = 0, j = 0, ret = 0;
	int count = 1;
	struct snval *ptmp = NULL;
	struct snval *ptmpnext = NULL;
	
	CString strFile = "NETSCREENATTACK";
	CString strLog ="";
	//WriteLog(strFile,"--------------- NETSCREENATTACK_MONITOR  Start---------------------");

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

	for(i = 0;i < NETSCREENATTACK_VARS;i ++)
		for(j = 0;j < index_id_c;j ++)
			NS_attack_oids[i].id[NS_attack_oids[i].len++] = index_id[j];

	struct oid *poid = NULL, *ptmpoid = NULL;
	poid = ptmpoid = &NS_attack_oids[2];
	ptmpoid->next = &NS_attack_oids[3];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_attack_oids[4];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_attack_oids[5];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_attack_oids[6];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_attack_oids[7];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_attack_oids[8];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_attack_oids[9];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_attack_oids[10];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_attack_oids[11];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_attack_oids[12];
	ptmpoid = ptmpoid->next;

	struct snval *psnval = NULL;
	if((ret = SN_VALUES(s, serv, community, 1, poid, &psnval)) < 0)
	{
		
		strLog.Format("ret = %d", ret);
		//WriteLog(strFile,strLog);
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
			sprintf(szReturn, "%s=", "nsIdsAttkMonSynAttk");
			break;
		case 2:
			sprintf(szReturn, "%s%s=", szReturn, "nsIdsAttkMonTearDrop");
			break;
		case 3:
			sprintf(szReturn, "%s%s=", szReturn, "nsIdsAttkMonSrcRoute");
			break;
		case 4:
			sprintf(szReturn, "%s%s=", szReturn, "nsIdsAttkMonPingDeath");
			break;
		case 5:
			sprintf(szReturn, "%s%s=", szReturn, "nsIdsAttkMonAddrSpoof");
			break;
		case 6:
			sprintf(szReturn, "%s%s=", szReturn, "nsIdsAttkMonLand");
			break;
		case 7:
			sprintf(szReturn, "%s%s=", szReturn, "nsIdsAttkMonIcmpFlood");
			break;
		case 8:
			sprintf(szReturn, "%s%s=", szReturn, "nsIdsAttkMonUdpFlood");
			break;
		case 9:
			sprintf(szReturn, "%s%s=", szReturn, "nsIdsAttkMonWinnuke");
			break;
		case 10:
			sprintf(szReturn, "%s%s=", szReturn, "nsIdsAttkMonPortScan");
			break;
		case 11:
			sprintf(szReturn, "%s%s=", szReturn, "nsIdsAttkMonIpSweep");
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

	//WriteLog(strFile,"--------------- NETSCREENATTACK_MONITOR  End ---------------------");

	return !ret;
}

#define		NETSCREENPACKDROP_VARS			12
struct oid NS_packdrop_oids[NETSCREENPACKDROP_VARS] = {
	/* Unique interface id, also used as table index. */
	{{0x2b, 6, 1, 4, 1, 3224, 9, 4, 1, 1}, 10, "nsIfMonIfIdx", ASN1_INT, ""}, 
	/* According NetScreen's concepts, each interface belongs to one virtual
       system. This attribute displays the virtual system name an interface
       belongs to. */
	{{0x2b, 6, 1, 4, 1, 3224, 9, 4, 1, 2}, 10, "nsIfMonVsys", ASN1_INT, ""}, 
	/* Packet drop since denied by policy. */
	{{0x2b, 6, 1, 4, 1, 3224, 9, 4, 1, 3}, 10, "nsIfMonPlyDeny", ASN1_COUNTER, ""}, 
	/* Packet drop due to authentication failed. */
	{{0x2b, 6, 1, 4, 1, 3224, 9, 4, 1, 4}, 10, "nsIfMonAuthFail", ASN1_COUNTER, ""}, 
	/* Packet drop due to URL blocking. */
	{{0x2b, 6, 1, 4, 1, 3224, 9, 4, 1, 5}, 10, "nsIfMonUrlBlock", ASN1_COUNTER, ""}, 
	/* packet queue up due to traffic management */
	{{0x2b, 6, 1, 4, 1, 3224, 9, 4, 1, 6}, 10, "nsIfMonTrMngQueue", ASN1_COUNTER, ""}, 
	/* Packet drop due to traffic management */
	{{0x2b, 6, 1, 4, 1, 3224, 9, 4, 1, 7}, 10, "nsIfMonTrMngDrop", ASN1_COUNTER, ""}, 
	/* IPSec enc failed due to sa not available, no ipak, etc */
	{{0x2b, 6, 1, 4, 1, 3224, 9, 4, 1, 8}, 10, "nsIfMonEncFail", ASN1_COUNTER, ""}, 
	/* packet drop due to no sa found for incomig spi */
	{{0x2b, 6, 1, 4, 1, 3224, 9, 4, 1, 9}, 10, "nsIfMonNoSa", ASN1_COUNTER, ""}, 
	/* packet drop due to no policy associated with found sa */
	{{0x2b, 6, 1, 4, 1, 3224, 9, 4, 1, 10}, 10, "nsIfMonNoSaPly", ASN1_COUNTER, ""}, 
	/* packet drop due to sa not active */
	{{0x2b, 6, 1, 4, 1, 3224, 9, 4, 1, 11}, 10, "nsIfMonSaInactive", ASN1_COUNTER, ""}, 
	/* packet drop due to denial by sa policy */
	{{0x2b, 6, 1, 4, 1, 3224, 9, 4, 1, 12}, 10, "nsIfMonSaPolicyDeny", ASN1_COUNTER, ""}
};

BOOL NETSCREENPACKDROP_MONITOR(char *server, int port, char *community, CString strIndex, int retrydealy, 
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
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_1%>"));//<%IDS_Dynamo_1%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_2%>"));//<%IDS_Dynamo_2%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_3%>"));//<%IDS_Dynamo_3%>

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

	for(i = 0;i < NETSCREENPACKDROP_VARS;i ++)
		for(j = 0;j < index_id_c;j ++)
			NS_packdrop_oids[i].id[NS_packdrop_oids[i].len++] = index_id[j];

	struct oid *poid = NULL, *ptmpoid = NULL;
	poid = ptmpoid = &NS_packdrop_oids[2];
	ptmpoid->next = &NS_packdrop_oids[3];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_packdrop_oids[4];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_packdrop_oids[5];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_packdrop_oids[6];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_packdrop_oids[7];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_packdrop_oids[8];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_packdrop_oids[9];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_packdrop_oids[10];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &NS_packdrop_oids[11];
	ptmpoid = ptmpoid->next;

	struct snval *psnval = NULL;
	if((ret = SN_VALUES(s, serv, community, 1, poid, &psnval)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_4%>"));//<%IDS_Dynamo_4%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Dynamo_5%>"));//<%IDS_Dynamo_5%> 所请求变量不存在
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
			sprintf(szReturn, "%s=", "nsIfMonPlyDeny");
			break;
		case 2:
			sprintf(szReturn, "%s%s=", szReturn, "nsIfMonAuthFail");
			break;
		case 3:
			sprintf(szReturn, "%s%s=", szReturn, "nsIfMonUrlBlock");
			break;
		case 4:
			sprintf(szReturn, "%s%s=", szReturn, "nsIfMonTrMngQueue");
			break;
		case 5:
			sprintf(szReturn, "%s%s=", szReturn, "nsIfMonTrMngDrop");
			break;
		case 6:
			sprintf(szReturn, "%s%s=", szReturn, "nsIfMonEncFail");
			break;
		case 7:
			sprintf(szReturn, "%s%s=", szReturn, "nsIfMonNoSa");
			break;
		case 8:
			sprintf(szReturn, "%s%s=", szReturn, "nsIfMonNoSaPly");
			break;
		case 9:
			sprintf(szReturn, "%s%s=", szReturn, "nsIfMonSaInactive");
			break;
		case 10:
			sprintf(szReturn, "%s%s=", szReturn, "nsIfMonSaPolicyDeny");
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

