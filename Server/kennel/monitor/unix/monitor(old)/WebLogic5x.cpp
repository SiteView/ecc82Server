
#include "WebLogic5x.h"
#include "SnmpUtils.h"

#include "..\..\base\funcGeneral.h"

#define		BEA_VARS			20
struct oid bea_oids[BEA_VARS] = {
	/* State of the Weblogic Server, as inferred by the snmp agent. Up implies 
	   the agent can contact the server; down implies the agent cannot contact 
	   the server. */
	{{0x2b, 6, 1, 4, 1, 140, 600, 20, 1, 20}, 10, "ServerState", ASN1_INT, ""},				// up(1), down(2)
	/* This value is true if client logins are enabled on the server. */
	{{0x2b, 6, 1, 4, 1, 140, 600, 20, 1, 25}, 10, "LoginEnabled", ASN1_INT, ""},			// true(1), false(2)
	/* Maximum heap size for this server, in KB. */
	{{0x2b, 6, 1, 4, 1, 140, 600, 20, 1, 60}, 10, "MaxHeapSpace", ASN1_INT, ""}, 
	/* Percentage of heap space currently in use in the server. */
	{{0x2b, 6, 1, 4, 1, 140, 600, 20, 1, 65}, 10, "HeapUsedPct", ASN1_INT, ""},				// (0..100)
	/* Current length of the server execute queue. */
	{{0x2b, 6, 1, 4, 1, 140, 600, 20, 1, 70}, 10, "QueueLength", ASN1_INT, ""}, 
	/* Current throughput of execute queue, expressed as number of requests 
	   processed per second. */
	{{0x2b, 6, 1, 4, 1, 140, 600, 20, 1, 75}, 10, "QueueThroughput", ASN1_INT, ""}, 
	/* Total number of EJB deployment units known to the server. */
	{{0x2b, 6, 1, 4, 1, 140, 600, 20, 1, 80}, 10, "NumEJBDeployment", ASN1_INT, ""}, 
	/* Total number of EJB beans actively deployed on the server. */
	{{0x2b, 6, 1, 4, 1, 140, 600, 20, 1, 85}, 10, "NumEJBBeansDeployed", ASN1_INT, ""}, 
	
	/* Port number */
	{{0x2b, 6, 1, 4, 1, 140, 600, 30, 1, 20}, 10, "ListenPort", ASN1_INT, ""}, 
	/* True if admin requests are allowed on this (protocol,ipAddr,port), 
	   else false. */
	{{0x2b, 6, 1, 4, 1, 140, 600, 30, 1, 25}, 10, "ListenAdminOK", ASN1_INT, ""},			// true(1), false(2)
	/* listening if the (protocol,ipAddr,port) is enabled on the server; 
	   notListening if it is not. The server may be listening but not 
	   accepting new clients if its serverLoginEnable state is false. 
	   In this case existing clients will continue to function but new ones 
	   will not. */
	{{0x2b, 6, 1, 4, 1, 140, 600, 30, 1, 30}, 10, "ListenState", ASN1_INT, ""},				// listening(1), notListening(2)
	
	/* A type of java means the cpElement is one of the elements in the normal 
	   java classpath. A type of weblogic means the cpElement is one of the 
	   elements in the weblogic.class.path. A type of servlet means the cpElement 
	   is one of the elements in the dynamic servlet classpath. */
	{{0x2b, 6, 1, 4, 1, 140, 600, 40, 1, 10}, 10, "CPType", ASN1_INT, ""},					// java(1), weblogic(2), servlet(3)
	/* Position of element within its path. The index starts at 1. */
	{{0x2b, 6, 1, 4, 1, 140, 600, 40, 1, 16}, 10, "CPIndex", ASN1_INT, ""},					// (1..64)
	
	/* Current number of database connections opened for this pool. */
	{{0x2b, 6, 1, 4, 1, 140, 600, 50, 1, 25}, 10, "JDBCCurrentPoolSize", ASN1_INT, ""}, 
	/* High water mark value for the number of pending connections. */
	{{0x2b, 6, 1, 4, 1, 140, 600, 50, 1, 40}, 10, "JDBCHighwaterConnect", ASN1_INT, ""}, 
	/* Initial number of database connections to be opened for this pool. */
	{{0x2b, 6, 1, 4, 1, 140, 600, 50, 1, 20}, 10, "JDBCInitCapacity", ASN1_INT, ""}, 
	/* High water mark for the wait time of a client in seconds. */
	{{0x2b, 6, 1, 4, 1, 140, 600, 50, 1, 45}, 10, "JDBCHighaterWait", ASN1_INT, ""}, 
	/* Number of pending database connection requests on this pool.  */
	{{0x2b, 6, 1, 4, 1, 140, 600, 50, 1, 35}, 10, "JDBCTotalConn", ASN1_INT, ""}, 
	/* Current number of database connections in use for this pool. */
	{{0x2b, 6, 1, 4, 1, 140, 600, 50, 1, 30}, 10, "JDBCCurrentInUse", ASN1_INT, ""}, 
	/* Maximum number of database conections allowed for this pool. */
	{{0x2b, 6, 1, 4, 1, 140, 600, 50, 1, 15}, 10, "JDBCMaxCapacity", ASN1_INT, ""}
};

BOOL WEBLOGIC5X_MONITOR(char *server, int port, char *community, CString strIndex, 
						int rdelay, int timeout, char *custpath, char *szReturn)
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
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_WebLogic5x_1%>"));//<%IDS_WebLogic5x_1%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s",FuncGetStringFromIDS("<%IDS_WebLogic5x_2%>"));//<%IDS_WebLogic5x_2%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_WebLogic5x_3%>"));//<%IDS_WebLogic5x_3%>

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

	for(i = 0;i < BEA_VARS;i ++)
		for(j = 0;j < index_id_c;j ++)
			bea_oids[i].id[bea_oids[i].len++] = index_id[j];

	struct oid *poid = NULL, *ptmpoid = NULL;
	poid = ptmpoid = &bea_oids[0];
	ptmpoid->next = &bea_oids[1];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &bea_oids[2];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &bea_oids[3];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &bea_oids[4];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &bea_oids[5];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &bea_oids[6];
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &bea_oids[7];
	ptmpoid = ptmpoid->next;

	struct snval *psnval = NULL;
	if((ret = SN_VALUES(s, serv, community, 1, poid, &psnval)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_WebLogic5x_4%>"));//<%IDS_WebLogic5x_4%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_WebLogic5x_5%>"));//<%IDS_WebLogic5x_5%>
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_WebLogic5x_6%>"));//<%IDS_WebLogic5x_6%>
		else if(ret == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_WebLogic5x_7%>"));//<%IDS_WebLogic5x_7%>
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_WebLogic5x_8%>"));//<%IDS_WebLogic5x_8%>
		else if(ret == -6)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_WebLogic5x_9%>"));//<%IDS_WebLogic5x_9%>
		else if(ret == -7)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_WebLogic5x_10%>"));//<%IDS_WebLogic5x_10%>
		else if(ret == -8)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_WebLogic5x_11%>"));//<%IDS_WebLogic5x_11%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_WebLogic5x_12%>"));//<%IDS_WebLogic5x_12%>

		goto w;
	}

	ptmp = psnval;
	ptmpnext = NULL;
	while(ptmp)
	{
		switch(count)
		{
		case 1:
			sprintf(szReturn, "%s=", "ServerState");
			break;
		case 2:
			sprintf(szReturn, "%s%s=", szReturn, "LoginEnabled");
			break;
		case 3:
			sprintf(szReturn, "%s%s=", szReturn, "MaxHeapSpace");
			break;
		case 4:
			sprintf(szReturn, "%s%s=", szReturn, "HeapUsedPct");
			break;
		case 5:
			sprintf(szReturn, "%s%s=", szReturn, "QueueLength");
			break;
		case 6:
			sprintf(szReturn, "%s%s=", szReturn, "QueueThroughput");
			break;
		case 7:
			sprintf(szReturn, "%s%s=", szReturn, "NumEJBDeployment");
			break;
		case 8:
			sprintf(szReturn, "%s%s=", szReturn, "NumEJBBeansDeployed");
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