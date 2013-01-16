
#include "Foundry.h"
#include "SnmpUtils.h"

#include "base\funcGeneral.h"

/* The statistics collection of 1 second CPU utilization. */
struct oid snAgGblCpuUtil1SecAvg = {{0x2b, 6, 1, 4, 1, 1991, 1, 1, 2, 1, 50}, 11, "", ASN1_GAUGE, ""};
/* The statistics collection of 5 second CPU utilization. */
struct oid snAgGblCpuUtil5SecAvg = {{0x2b, 6, 1, 4, 1, 1991, 1, 1, 2, 1, 51}, 11, "", ASN1_GAUGE, ""};
/* The statistics collection of 1 minute CPU utilization. */
struct oid snAgGblCpuUtil1MinAvg = {{0x2b, 6, 1, 4, 1, 1991, 1, 1, 2, 1, 52}, 11, "", ASN1_GAUGE, ""};

/* -- System CPU Utilization Table. */
struct oid snAgentCpuUtilTable = {{0x2b, 6, 1, 4, 1, 1991, 1, 1, 2, 11, 1}, 11, "", 0, ""};

/* The slot number of module which contains the cpu. */
struct oid snAgentCpuUtilSlotNum = {{0x2b, 6, 1, 4, 1, 1991, 1, 1, 2, 11, 1, 1, 1}, 13, "", ASN1_INT, ""};
/* The id of cpu. For non-VM1/WSM management module, there is one CPU. For VM1/WSM 
   there's one management CPU and three slave CPUs. The management CPU could be 
   turned off. For POS and ATM there's no management CPU but  two slave CPUs. Id for 
   management cpu is 1. Value of 2 or greater are for slave CPUs. */
struct oid snAgentCpuUtilCpuId = {{0x2b, 6, 1, 4, 1, 1991, 1, 1, 2, 11, 1, 1, 2}, 13, "", ASN1_INT, ""};
/* The value, in seconds, for this utilization. For both management and slave CPU, 
   we display utilization for 1 sec, 5 sec, 60 sec and 300 sec interval. */
struct oid snAgentCpuUtilInterval = {{0x2b, 6, 1, 4, 1, 1991, 1, 1, 2, 11, 1, 1, 3}, 13, "", ASN1_INT, ""};
/* The statistical CPU utilization in units of one-hundredth of a percent. */
struct oid snAgentCpuUtilValue = {{0x2b, 6, 1, 4, 1, 1991, 1, 1, 2, 11, 1, 1, 4}, 13, "", ASN1_GAUGE, ""};

BOOL FOUNDRYCPUS_MONITOR(char *server, int port, char *community, char *cpath, char *szReturn)
{
	BOOL		bResult = TRUE;
	struct		oid	*poid = NULL, *proid = NULL, *pnextoid = NULL;
	struct		snval *psnval = NULL, *ptmpsnval = NULL;

	int s;
	struct sockaddr_in serv;

	if(MakeSocket(server, port, s, serv) < 0)
	{
		return FALSE;
	}

	poid = (struct oid *)malloc(sizeof(struct oid));
	proid = (struct oid *)malloc(sizeof(struct oid));
	psnval = (struct snval *)malloc(sizeof(struct snval));
	
	if(poid == NULL || proid == NULL || psnval == NULL)
	{
		bResult = FALSE;
		goto err;
	}

	memset(poid, 0, sizeof(struct oid));
	memset(proid, 0, sizeof(struct oid));
	memset(psnval, 0, sizeof(struct snval));

	memcpy(poid, &snAgentCpuUtilTable, sizeof(struct oid));
	if(SN_NEXT_VALUE(s, serv, community, 1, poid, proid, psnval) < 0)
	{
		bResult = FALSE;
		goto err;
	}

	while(!compare_oid(*proid, snAgentCpuUtilSlotNum))
	{
		sprintf(szReturn, "%s%d$$$", szReturn, psnval->sv_val.sv_int);

		memcpy(poid, proid, sizeof(struct oid));
		memset(proid, 0, sizeof(struct oid));
		memset(psnval, 0, sizeof(struct snval));
		if(SN_NEXT_VALUE(s, serv, community, 1, poid, proid, psnval) < 0)
		{
			bResult = FALSE;
			goto err;
		}
	}

err:
	if(poid) { free(poid); poid = NULL; }
	if(proid) { free(proid); proid = NULL; }
	if(psnval) { free(psnval); psnval = NULL; }

	return bResult;
}

BOOL FOUNDRYCPU2_MONITOR(char *server, int port, char *community, CString strIndex, 
			int rdelay, int timeout, char *cpath, char *szReturn)
{
	int s;
	struct sockaddr_in serv;

	int i = 0, ret = 0;
	BOOL bResult = TRUE;
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

	for(i = 0;i < index_id_c;i ++)
	{
		snAgGblCpuUtil1SecAvg.id[snAgGblCpuUtil1SecAvg.len ++] = index_id[i];
		snAgGblCpuUtil5SecAvg.id[snAgGblCpuUtil5SecAvg.len ++] = index_id[i];
		snAgGblCpuUtil1MinAvg.id[snAgGblCpuUtil1MinAvg.len ++] = index_id[i];
	}

	struct oid *poid = NULL, *ptmpoid = NULL;
	poid = ptmpoid = &snAgGblCpuUtil1SecAvg;
	ptmpoid->next = &snAgGblCpuUtil5SecAvg;
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &snAgGblCpuUtil1MinAvg;
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

		bResult = FALSE;
		goto w;
	}

	ptmp = psnval;
	ptmpnext = NULL;
	while(ptmp)
	{
		switch(count)
		{
		case 1:
			sprintf(szReturn, "%s=", "snAgGblCpuUtil1SecAvg");
			break;
		case 2:
			sprintf(szReturn, "%s%s=", szReturn, "snAgGblCpuUtil5SecAvg");
			break;
		case 3:
			sprintf(szReturn, "%s%s=", szReturn, "snAgGblCpuUtil1MinAvg");
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

	return bResult;
}

BOOL FOUNDRYCPU_MONITOR(char *server, int port, char *community, CString strIndex, 
			int rdelay, int timeout, char *cpath, char *szReturn)
{
	int s;
	struct sockaddr_in serv;

	int i = 0, ret = 0;
	BOOL bResult = TRUE;
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

	for(i = 0;i < index_id_c;i ++)
	{
		snAgentCpuUtilSlotNum.id[snAgentCpuUtilSlotNum.len ++] = index_id[i];
		snAgentCpuUtilCpuId.id[snAgentCpuUtilCpuId.len ++] = index_id[i];
		snAgentCpuUtilInterval.id[snAgentCpuUtilInterval.len ++] = index_id[i];
		snAgentCpuUtilValue.id[snAgentCpuUtilValue.len ++] = index_id[i];
	}

	struct oid *poid = NULL, *ptmpoid = NULL;
	poid = ptmpoid = &snAgentCpuUtilSlotNum;
	ptmpoid->next = &snAgentCpuUtilCpuId;
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &snAgentCpuUtilInterval;
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &snAgentCpuUtilValue;
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

		bResult = FALSE;
		goto w;
	}

	ptmp = psnval;
	ptmpnext = NULL;
	while(ptmp)
	{
		switch(count)
		{
		case 1:
			sprintf(szReturn, "%s=", "snAgentCpuUtilSlotNum");
			break;
		case 2:
			sprintf(szReturn, "%s%s=", szReturn, "snAgentCpuUtilCpuId");
			break;
		case 3:
			sprintf(szReturn, "%s%s=", szReturn, "snAgentCpuUtilInterval");
			break;
		case 4:
			sprintf(szReturn, "%s%s=", szReturn, "snAgentCpuUtilValue");
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

	return bResult;
}

/* The system dynamic memory utilization, in unit of percentage. */
struct oid snAgGblDynMemUtil = {{0x2b, 6, 1, 4, 1, 1991, 1, 1, 2, 1, 53}, 11, "", ASN1_GAUGE, ""};
/* The total amount of system dynamic memory, in number of bytes. */
struct oid snAgGblDynMemTotal = {{0x2b, 6, 1, 4, 1, 1991, 1, 1, 2, 1, 54}, 11, "", ASN1_INT, ""};
/* The free amount of system dynamic memory, in number of bytes. */
struct oid snAgGblDynMemFree = {{0x2b, 6, 1, 4, 1, 1991, 1, 1, 2, 1, 55}, 11, "", ASN1_INT, ""};

BOOL FOUNDRYMEM_MONITOR(char *server, int port, char *community, CString strIndex, 
			int rdelay, int timeout, char *cpath, char *szReturn)
{
	int s;
	struct sockaddr_in serv;

	int i = 0, ret = 0;
	BOOL bResult = TRUE;
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

	for(i = 0;i < index_id_c;i ++)
	{
		snAgGblDynMemUtil.id[snAgGblDynMemUtil.len ++] = index_id[i];
		snAgGblDynMemTotal.id[snAgGblDynMemTotal.len ++] = index_id[i];
		snAgGblDynMemFree.id[snAgGblDynMemFree.len ++] = index_id[i];
	}

	struct oid *poid = NULL, *ptmpoid = NULL;
	poid = ptmpoid = &snAgGblDynMemUtil;
	ptmpoid->next = &snAgGblDynMemTotal;
	ptmpoid = ptmpoid->next;
	ptmpoid->next = &snAgGblDynMemFree;
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

		bResult = FALSE;
		goto w;
	}

	ptmp = psnval;
	ptmpnext = NULL;
	while(ptmp)
	{
		switch(count)
		{
		case 1:
			sprintf(szReturn, "%s=", "snAgGblDynMemUtil");
			break;
		case 2:
			sprintf(szReturn, "%s%s=", szReturn, "snAgGblDynMemTotal");
			break;
		case 3:
			sprintf(szReturn, "%s%s=", szReturn, "snAgGblDynMemFree");
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

	return bResult;
}

// --  image and config file down/up load 
struct oid snAgTftpServerIp = {{0x2b, 6, 1, 4, 1, 1991, 1, 1, 2, 1, 5}, 11, "", ASN1_IPADDR, ""};
struct oid snAgImgFname = {{0x2b, 6, 1, 4, 1, 1991, 1, 1, 2, 1, 6}, 11, "", ASN1_OCTSTR, ""};
struct oid snAgImgLoad = {{0x2b, 6, 1, 4, 1, 1991, 1, 1, 2, 1, 7}, 11, "", ASN1_INT, ""};
struct oid snAgCfgFname = {{0x2b, 6, 1, 4, 1, 1991, 1, 1, 2, 1, 8}, 11, "", ASN1_OCTSTR, ""};
struct oid snAgCfgLoad = {{0x2b, 6, 1, 4, 1, 1991, 1, 1, 2, 1, 9}, 11, "", ASN1_INT, ""};

enum snAgImgLoadE
{
	IMG_NORMAL = 1, 
	IMG_FLASHPREPAREREADFAILURE = 2, 
	IMG_FLASHREADERROR = 3, 
	IMG_FLASHPREPAREWRITEFAILURE = 4, 
	IMG_FLASHWRITEERROR = 5, 
	IMG_TFTPTIMEOUTERROR = 6, 
	IMG_TFTPOUTOFBUFFERSPACE = 7, 
	IMG_TFTPBUSY = 8, 
	IMG_TFTPREMOTEOTHERERRORS = 9, 
	IMG_TFTPREMOTENOFILE = 10, 
	IMG_TFTPREMOTEBADACCESS = 11, 
	IMG_TFTPREMOTEDISKFULL = 12, 
	IMG_TFTPREMOTEBADOPERATION = 13, 
	IMG_TFTPREMOTEBADID = 14, 
	IMG_TFTPREMOTEFILEEXISTS = 15, 
	IMG_TFTPREMOTENOUSER = 16, 
	IMG_OPERATIONERROR = 17, 
	IMG_LOADING = 18, 
	IMG_UPLOADMPPRIMARY = 19, 
	IMG_DOWNLOADMPPRIMARY = 20, 
	IMG_UPLOADMPSECONDARY = 21, 
	IMG_DOWNLOADMPSECONDARY = 22, 
	IMG_TFTPWRONGFILETYPE = 23, 
	IMG_DOWNLOADSPPRIMARY = 24, 
	IMG_DOWNLOADSPSECONDARY = 25

};

enum snAgCfgLoadE
{
	CFG_NORMAL = 1, 
	CFG_FLASHPREPAREREADFAILURE = 2, 
	CFG_FLASHREADERROR = 3, 
	CFG_FLASHPREPAREWRITEFAILURE = 4, 
	CFG_FLASHWRITEERROR = 5, 
	CFG_TFTPTIMEOUTERROR = 6, 
	CFG_TFTPOUTOFBUFFERSPACE = 7, 
	CFG_TFTPBUSY = 8, 
	CFG_TFTPREMOTEOTHERERRORS = 9, 
	CFG_TFTPREMOTENOFILE = 10, 
	CFG_TFTPREMOTEBADACCESS = 11, 
	CFG_TFTPREMOTEDISKFULL = 12, 
	CFG_TFTPREMOTEBADOPERATION = 13, 
	CFG_TFTPREMOTEBADID = 14, 
	CFG_TFTPREMOTEFILEEXISTS = 15, 
	CFG_TFTPREMOTENOUSER = 16, 
	CFG_OPERATIONERROR =17, 
	CFG_LOADING = 18, 
	CFG_UPLOADFROMFLASHTOSERVER = 20, 
	CFG_DOWNLOADTOFLASHFROMSERVER = 21, 
	CFG_UPLOADFROMDRAMTOSERVER = 22, 
	CFG_DOWNLOADTODRAMFROMSERVER = 23, 
	CFG_UPLOADFROMFLASHTONMS = 24, 
	CFG_DOWNLOADTOFLASHFROMNMS = 25, 
	CFG_UPLOADFROMDRAMTONMS = 26, 
	CFG_DOWNLOADTODRAMFROMNMS = 27, 
	CFG_OPERATIONDONEWITHNMS = 28, 
	CFG_TFTPWRONGFILETYPE = 29, 
	CFG_DOWNLOADTODRAMFROMSERVEROVERWRITE = 30

};