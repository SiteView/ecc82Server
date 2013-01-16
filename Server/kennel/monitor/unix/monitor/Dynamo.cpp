
#include "Dynamo.h"
#include "SnmpUtils.h"

#include "base\funcGeneral.h"

#define		DYNAMO_VARS			28
struct oid dynamo_oids[DYNAMO_VARS] = {
	/* The total amount of memory currently available for allocating objects, 
	   measured in bytes. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 1, 4}, 9, "sysTotalMem", ASN1_COUNTER, ""}, 
	/* An approximation of the total amount of memory currently available for 
	   future allocated objects, measured in bytes. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 1, 5}, 9, "sysFreeMem", ASN1_COUNTER, ""}, 
	/* The number of system global info messages written. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 1, 6}, 9, "sysNumInfoMsgs", ASN1_COUNTER, ""}, 
	/* The number of system global warning messages written. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 1, 7}, 9, "sysNumWarningMsgs", ASN1_COUNTER, ""}, 
	/* The number of system global error messages written. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 1, 8}, 9, "sysNumErrorMsgs", ASN1_COUNTER, ""}, 

	/* True if this Dynamo is running a load manager. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 2, 1}, 9, "lmIsManager", ASN1_COUNTER, ""}, 
	/* Returns this Dynamo's offset into the list of load managing entities. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 2, 2}, 9, "lmManagerIndex", ASN1_COUNTER, ""},
	/* True if the load manager is an acting primary manager. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 2, 3}, 9, "lmIsPrimaryManager", ASN1_COUNTER, ""}, 
	/* True if the load manager has serviced any connection module requests in 
	   the amount of time set as the connection module polling interval. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 2, 4}, 9, "lmServicingCMs", ASN1_COUNTER, ""}, 
	/* The port of the connection module agent. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 2, 6}, 9, "lmCMLDRPPort", ASN1_COUNTER, ""}, 

	/* A unique value for each pooling service. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 5, 1, 1, 1, 1}, 12, "dbIndex", ASN1_COUNTER, ""}, 
	/* The minimum number of connections pooled. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 5, 1, 1, 3, 1}, 12, "dbMinConn", ASN1_COUNTER, ""}, 
	/* The maximum number of connections pooled. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 5, 1, 1, 4, 1}, 12, "dbMaxConn", ASN1_COUNTER, ""}, 
	/* The maximum number of free pooled connections at a time. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 5, 1, 1, 5, 1}, 12, "dbMaxFreeConn", ASN1_COUNTER, ""}, 
	/* Whether or not the pool is to block on check outs. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 5, 1, 1, 6, 1}, 12, "dbBlocking", ASN1_COUNTER, ""}, 
	/* Returns the number of connections checked out. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 5, 1, 1, 7, 1}, 12, "dbConnOut", ASN1_COUNTER, ""}, 
	/* Returns the number of free connections in the pool.  This number refers to 
	   actually created connections that are not currently checked out.  It does not 
	   include how many more connections are allowed to be created as set by the 
	   maximum number of connections allowed in the pool. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 5, 1, 1, 8, 1}, 12, "dbFreeResources", ASN1_COUNTER, ""}, 
	/* Returns the number of total connections in the pool.  This number refers to 
	   actually created connections and is not an indication of how many more connections 
	   may be created and used in the pool. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 5, 1, 1, 9, 1}, 12, "dbTotalResources", ASN1_COUNTER, ""}, 

	/* The port of the DRP server. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 4, 2}, 9, "drpPort", ASN1_COUNTER, ""}, 
	/* Total number of DRP requests serviced. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 4, 3}, 9, "drpTotalReqsServed", ASN1_COUNTER, ""}, 
	/* Total service time in msecs for all DRP requests. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 4, 4}, 9, "drpTotalReqTime", ASN1_COUNTER, ""}, 
	/* Average service time in msecs for each DRP request. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 4, 5}, 9, "drpAvgReqTime", ASN1_COUNTER, ""}, 
	/* True if this Dynamo is accepting new sessions. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 4, 6}, 9, "drpNewSessions", ASN1_COUNTER, ""}, 
	/* True if this DRP server is available to handle requests. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 4, 7}, 9, "drpAvailable", ASN1_COUNTER, ""}, 

	/* The number of created sessions. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 3, 1}, 9, "stCreatedSessionCnt", ASN1_COUNTER, ""}, 
	/* The number of valid sessions. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 3, 2}, 9, "stValidSessionCnt", ASN1_COUNTER, ""}, 
	/* The number of sessions migrated to server. */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 3, 3}, 9, "stRestoredSessionCnt", ASN1_COUNTER, ""}, 
	/* ... */
	{{0x2b, 6, 1, 4, 1, 2725, 1, 3, 4}, 9, "stDictionaryServerStatus", ASN1_COUNTER, ""}
};

BOOL DYNAMO_MONITOR(char *server, int port, char *community, CString strIndex, int retrydealy, 
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

	for(i = 0;i < DYNAMO_VARS;i ++)
		for(j = 0;j < index_id_c;j ++)
			dynamo_oids[i].id[dynamo_oids[i].len++] = index_id[j];

	struct oid *poid = NULL, *ptmpoid = NULL;
	poid = ptmpoid = &dynamo_oids[0];		// sysTotalMem
	ptmpoid->next = &dynamo_oids[1];		// sysFreeMem
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &dynamo_oids[10];		// dbIndex
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &dynamo_oids[16];		// dbFreeResources
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &dynamo_oids[17];		// dbTotalResources
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &dynamo_oids[21];		// drpAvgReqTime
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &dynamo_oids[24];		// stCreatedSessionCnt
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
			sprintf(szReturn, "%s=", "sysTotalMem");
			break;
		case 2:
			sprintf(szReturn, "%s%s=", szReturn, "sysFreeMem");
			break;
		case 3:
			sprintf(szReturn, "%s%s=", szReturn, "dbIndex");
			break;
		case 4:
			sprintf(szReturn, "%s%s=", szReturn, "dbFreeResources");
			break;
		case 5:
			sprintf(szReturn, "%s%s=", szReturn, "dbTotalResources");
			break;
		case 6:
			sprintf(szReturn, "%s%s=", szReturn, "drpAvgReqTime");
			break;
		case 7:
			sprintf(szReturn, "%s%s=", szReturn, "stCreatedSessionCnt");
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
