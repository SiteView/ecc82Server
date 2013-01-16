
#include "WebLogic7x.h"
#include "SnmpUtils.h"

#include <Base\funcGeneral.h>

// -- JmsRuntimeMBean
struct oid jmsRuntimeTable = {{0x2b, 6, 1, 4, 1, 140, 625, 240}, 8, "", 0, ""};

struct oid jmsRuntimeIndex = {{0x2b, 6, 1, 4, 1, 140, 625, 240, 1, 1}, 10, "", ASN1_OCTSTR, ""};
struct oid jmsRuntimeObjectName = {{0x2b, 6, 1, 4, 1, 140, 625, 240, 1, 5}, 10, "", ASN1_OCTSTR, ""};
struct oid jmsRuntimeType = {{0x2b, 6, 1, 4, 1, 140, 625, 240, 1, 10}, 10, "", ASN1_OCTSTR, ""};
struct oid jmsRuntimeName = {{0x2b, 6, 1, 4, 1, 140, 625, 240, 1, 15}, 10, "", ASN1_OCTSTR, ""};
struct oid jmsRuntimeParent = {{0x2b, 6, 1, 4, 1, 140, 625, 240, 1, 20}, 10, "", ASN1_OCTSTR, ""};
struct oid jmsRuntimeConnections = {{0x2b, 6, 1, 4, 1, 140, 625, 240, 1, 25}, 10, "", ASN1_OCTSTR, ""};
struct oid jmsRuntimeConnectionsCurrentCount = {{0x2b, 6, 1, 4, 1, 140, 625, 240, 1, 30}, 10, "", ASN1_INT, ""};
struct oid jmsRuntimeConnectionsHighCount = {{0x2b, 6, 1, 4, 1, 140, 625, 240, 1, 35}, 10, "", ASN1_INT, ""};
struct oid jmsRuntimeConnectionsTotalCount = {{0x2b, 6, 1, 4, 1, 140, 625, 240, 1, 40}, 10, "", ASN1_INT, ""};
struct oid jmsRuntimeJMSServers = {{0x2b, 6, 1, 4, 1, 140, 625, 240, 1, 45}, 10, "", ASN1_OCTSTR, ""};
struct oid jmsRuntimeJMSServersCurrentCount = {{0x2b, 6, 1, 4, 1, 140, 625, 240, 1, 50}, 10, "", ASN1_INT, ""};
struct oid jmsRuntimeJMSServersHighCount = {{0x2b, 6, 1, 4, 1, 140, 625, 240, 1, 55}, 10, "", ASN1_INT, ""};
struct oid jmsRuntimeJMSServersTotalCount = {{0x2b, 6, 1, 4, 1, 140, 625, 240, 1, 60}, 10, "", ASN1_INT, ""};
//=====================================================================================

BOOL WEBLOGIC7X_MONITOR(char *server, int port, char *community, CString strIndex, 
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

	struct oid *poid = NULL, *ptmpoid = NULL;
	for(j = 0;j < index_id_c;j ++)
		jmsRuntimeConnectionsCurrentCount.id[jmsRuntimeConnectionsCurrentCount.len ++] = index_id[j];
	poid = ptmpoid = &jmsRuntimeConnectionsCurrentCount;
	for(j = 0;j < index_id_c;j ++)
		jmsRuntimeConnectionsHighCount.id[jmsRuntimeConnectionsHighCount.len ++] = index_id[j];
	ptmpoid->next = &jmsRuntimeConnectionsHighCount;
	ptmpoid = ptmpoid->next;
	for(j = 0;j < index_id_c;j ++)
		jmsRuntimeConnectionsTotalCount.id[jmsRuntimeConnectionsTotalCount.len ++] = index_id[j];
	ptmpoid->next = &jmsRuntimeConnectionsTotalCount;
	ptmpoid = ptmpoid->next;
	for(j = 0;j < index_id_c;j ++)
		jmsRuntimeJMSServersCurrentCount.id[jmsRuntimeJMSServersCurrentCount.len ++] = index_id[j];
	ptmpoid->next = &jmsRuntimeJMSServersCurrentCount;
	ptmpoid = ptmpoid->next;
	for(j = 0;j < index_id_c;j ++)
		jmsRuntimeJMSServersHighCount.id[jmsRuntimeJMSServersHighCount.len ++] = index_id[j];
	ptmpoid->next = &jmsRuntimeJMSServersHighCount;
	ptmpoid = ptmpoid->next;
	for(j = 0;j < index_id_c;j ++)
		jmsRuntimeJMSServersTotalCount.id[jmsRuntimeJMSServersTotalCount.len ++] = index_id[j];
	ptmpoid->next = &jmsRuntimeJMSServersTotalCount;
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
			sprintf(szReturn, "%s=", "jmsRuntimeConnectionsCurrentCount");
			break;
		case 2:
			sprintf(szReturn, "%s%s=", szReturn, "jmsRuntimeConnectionsHighCount");
			break;
		case 3:
			sprintf(szReturn, "%s%s=", szReturn, "jmsRuntimeConnectionsTotalCount");
			break;
		case 4:
			sprintf(szReturn, "%s%s=", szReturn, "jmsRuntimeJMSServersCurrentCount");
			break;
		case 5:
			sprintf(szReturn, "%s%s=", szReturn, "jmsRuntimeJMSServersHighCount");
			break;
		case 6:
			sprintf(szReturn, "%s%s=", szReturn, "jmsRuntimeJMSServersTotalCount");
			break;
		}

		switch(ptmp->sv_type)
		{
		case ASN1_INT:
			sprintf(szReturn, "%s%d$", szReturn, ptmp->sv_val.sv_int);
			break;
		default:
			ret = 1;
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_WebLogic7x_01%>"));
			goto w;
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