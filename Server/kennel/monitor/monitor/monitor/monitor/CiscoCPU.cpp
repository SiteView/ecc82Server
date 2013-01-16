
#include "CiscoCPU.h"
#include "SnmpUtils.h"

#include <Base\funcGeneral.h>

struct oid cisco_cpu_oid = {{0x2b, 6, 1, 4, 1, 9, 2, 1, 56}, 9, "cisco.local.lsystem.busyPer", ASN1_INT, ""};
struct oid cisco_memory_oid = {{0x2b, 6, 1, 4, 1, 9, 2, 1, 8}, 9, "cisco.local.lsystem.freeMem", ASN1_INT, ""};

BOOL CISCOCPU_MONITOR(char *server, int port, CString strIndex, char *community, 
					  int timeout, int nrdelay, char *custpath, char *szReturn)
{
	int s;
	struct sockaddr_in serv;

	int i = 0, ret = 0;
	int count = 1;
	struct snval *ptmp = NULL;
	struct snval *ptmpnext = NULL;
	int nBusy=0;
	float fFreeSpace=0.0;

	if((ret = MakeSocket(server, port, s, serv)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_1%>"));//<%IDS_CiscoCPU_1%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_2%>"));//<%IDS_CiscoCPU_2%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_3%>"));//<%IDS_CiscoCPU_3%>

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
		cisco_cpu_oid.id[cisco_cpu_oid.len++] = index_id[i];
		cisco_memory_oid.id[cisco_memory_oid.len++] = index_id[i];
	}

	struct oid *poid = NULL, *ptmpoid = NULL;
	poid = ptmpoid = &cisco_cpu_oid;
	ptmpoid->next = &cisco_memory_oid;
	ptmpoid = ptmpoid->next;

	struct snval *psnval = NULL;
	if((ret = SN_VALUES(s, serv, community, 1, poid, &psnval)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_4%>"));//<%IDS_CiscoCPU_4%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_5%>"));//<%IDS_CiscoCPU_5%>
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_6%>"));//<%IDS_CiscoCPU_6%>
		else if(ret == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_7%>"));//<%IDS_CiscoCPU_7%>
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_8%>"));//<%IDS_CiscoCPU_8%>
		else if(ret == -6)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_9%>"));//<%IDS_CiscoCPU_9%>
		else if(ret == -7)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_10%>"));//<%IDS_CiscoCPU_10%>
		else if(ret == -8)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_11%>"));//<%IDS_CiscoCPU_11%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_12%>"));//<%IDS_CiscoCPU_12%>

		goto w;
	}

	ptmp = psnval;
	ptmpnext = NULL;

	while(ptmp)
	{
	/*		switch(count)
		{
		
		case 1:
			sprintf(szReturn, "%s=", "utilization");
			break;
		case 2:
			sprintf(szReturn, "%s%s=", szReturn, "freeSpace");
			break;
		}
		*/
/*
		switch(ptmp->sv_type)
		{
		case ASN1_INT:
			if (1==count)
				sprintf(szReturn, "%s%d$", szReturn, ptmp->sv_val.sv_int);
			if (2==count)
				sprintf(szReturn, "%s%d$", szReturn, ptmp->sv_val.sv_int/1024/8);
			break;
		case ASN1_COUNTER:
		case ASN1_GAUGE:
		case ASN1_TIMETICKS:
		case ASN1_COUNTER64:
		case ASN1_UINTEGER32:
			sprintf(szReturn, "%s%u$", szReturn, ptmp->sv_val.sv_int);
			break;
		default:
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_13%>"));//<%IDS_CiscoCPU_13%>
			ret = -1;
			goto w;
		}
*/
		switch(ptmp->sv_type)
		{
		case ASN1_INT:
			if (count==1) nBusy= ptmp->sv_val.sv_int;
			if (count==2) fFreeSpace= (float)ptmp->sv_val.sv_int;

			 
			//sprintf(szReturn, "%s%d$", szReturn, ptmp->sv_val.sv_int);
			break;
		case ASN1_COUNTER:
		case ASN1_GAUGE:
		case ASN1_TIMETICKS:
		case ASN1_COUNTER64:
		case ASN1_UINTEGER32:
			 if (count==1) nBusy= ptmp->sv_val.sv_int;
			 if (count==2) fFreeSpace= (float)ptmp->sv_val.sv_int;

			break;
		default:
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_13%>"));//<%IDS_CiscoCPU_13%>
			ret = -1;
			goto w;
		}
		count ++;
		ptmp = ptmp->next;
	}

w:	if(psnval)
	{
		ptmp = psnval;
		if(ptmp) ptmpnext = ptmp->next;
		while(ptmp)
		{
			free(ptmp);
			ptmp = ptmpnext;
			if(ptmp) ptmpnext = ptmp->next;
		}
	}
      
	if(strlen(szReturn)==0) 
		 sprintf(szReturn,"utilization=%d$$freeSpace=%.2f$",
				nBusy,fFreeSpace/1024/1024/8);


	return !ret;
}

struct oid huawei_cpu_oid = {{0x2b,6,1,4,1,2011,6,3,4,1,2,0,0,0}, 14, "cisco.local.lsystem.busyPer", ASN1_INT, ""};
struct oid huawei_memory_oid = {{0x2b,6,1,4,1,2011,6,3,5,1,1,3,0,0,1}, 15, "cisco.local.lsystem.freeMem", ASN1_INT, ""};
struct oid huawei_totalmemory_oid = {{0x2b,6,1,4,1,2011,6,3,5,1,1,2,0,0,1}, 15, "cisco.local.lsystem.TotalMem", ASN1_INT, ""};

struct oid huawei_8016_cpu_oid = {{0x2b,6,1,4,1,2011,2,17,1,2,3,1,1,4,17}, 15, "cisco.local.lsystem.busyPer", ASN1_INT, ""};
//内存使用率
struct oid huawei_8016_memory_oid = {{0x2b,6,1,4,1,2011,2,17,1,2,3,1,1,5,17}, 15, "cisco.local.lsystem.mempercentused", ASN1_INT, ""};


//type 型号
BOOL HUAWEISWITCH_CPU(char *server, int port, CString strIndex, char *community, 
					  int timeout, int nrdelay, char *custpath,  CString  strType,
					  char *szReturn)
{
	int s;
	struct sockaddr_in serv;
	int nBusy=0;
	int nPercentUsed=0;


	int i = 0, ret = 0;
	int count = 1;
	struct snval *ptmp = NULL;
	struct snval *ptmpnext = NULL;

	if((ret = MakeSocket(server, port, s, serv)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_1%>"));//<%IDS_CiscoCPU_1%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_2%>"));//<%IDS_CiscoCPU_2%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_3%>"));//<%IDS_CiscoCPU_3%>

		return FALSE;
	}

/*	long index_id[32];
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
		cisco_cpu_oid.id[cisco_cpu_oid.len++] = index_id[i];
		cisco_memory_oid.id[cisco_memory_oid.len++] = index_id[i];
	}*/
	struct oid *poid = NULL, *ptmpoid = NULL;
	if(strType="8016")
	{
		poid = ptmpoid = &huawei_8016_cpu_oid;
		ptmpoid->next = &huawei_8016_memory_oid;
		ptmpoid = ptmpoid->next;
	}	
	struct snval *psnval = NULL;
	if((ret = SN_VALUES(s, serv, community, 1, poid, &psnval)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_4%>"));//<%IDS_CiscoCPU_4%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_5%>"));//<%IDS_CiscoCPU_5%>
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_6%>"));//<%IDS_CiscoCPU_6%>
		else if(ret == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_7%>"));//<%IDS_CiscoCPU_7%>
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_8%>"));//<%IDS_CiscoCPU_8%>
		else if(ret == -6)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_9%>"));//<%IDS_CiscoCPU_9%>
		else if(ret == -7)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_10%>"));//<%IDS_CiscoCPU_10%>
		else if(ret == -8)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_11%>"));//<%IDS_CiscoCPU_11%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_12%>"));//<%IDS_CiscoCPU_12%>

		goto w;
	}

	ptmp = psnval;
	ptmpnext = NULL;

	while(ptmp)
	{
		/*
		switch(count)
		{
		case 1:
			sprintf(szReturn, "%s=", "utilization");
			break;
		case 2:
			sprintf(szReturn, "%s%s=", szReturn, "freeSpace");
			break;
		}*/

		switch(ptmp->sv_type)
		{
		case ASN1_INT:
			if (count==1) nBusy= ptmp->sv_val.sv_int;
			if (count==2) nPercentUsed= ptmp->sv_val.sv_int;

			 
			//sprintf(szReturn, "%s%d$", szReturn, ptmp->sv_val.sv_int);
			break;
		case ASN1_COUNTER:
		case ASN1_GAUGE:
		case ASN1_TIMETICKS:
		case ASN1_COUNTER64:
		case ASN1_UINTEGER32:
			 if (count==1) nBusy= ptmp->sv_val.sv_int;
			 if (count==2) nPercentUsed= ptmp->sv_val.sv_int;

			break;
		default:
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_13%>"));//<%IDS_CiscoCPU_13%>
			ret = -1;
			goto w;
		}

		count ++;
		ptmp = ptmp->next;
	}

w:	if(psnval)
	{
		ptmp = psnval;
		if(ptmp) ptmpnext = ptmp->next;
		while(ptmp)
		{
			free(ptmp);
			ptmp = ptmpnext;
			if(ptmp) ptmpnext = ptmp->next;
		}
	}
	
	if(strlen(szReturn)==0) 
		 sprintf(szReturn,"utilization=%d$$percentused=%d$",
				nBusy,nPercentUsed);

	return !ret;
}
//--------------------------------------------------------
BOOL HUAWEICPU_MONITOR(char *server, int port, CString strIndex, char *community, 
					  int timeout, int nrdelay, char *custpath, char *szReturn)
{
	int s;
	struct sockaddr_in serv;
	int nBusy=0;
	float nFreeSpace=0.0;
	float nTotalMem=0.0;

	int i = 0, ret = 0;
	int count = 1;
	struct snval *ptmp = NULL;
	struct snval *ptmpnext = NULL;

	if((ret = MakeSocket(server, port, s, serv)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_1%>"));//<%IDS_CiscoCPU_1%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_2%>"));//<%IDS_CiscoCPU_2%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_3%>"));//<%IDS_CiscoCPU_3%>

		return FALSE;
	}

	struct oid *poid = NULL, *ptmpoid = NULL;
	poid = ptmpoid = &huawei_cpu_oid;
	ptmpoid->next = &huawei_memory_oid;
	ptmpoid = ptmpoid->next;

	ptmpoid->next =	&huawei_totalmemory_oid;
	ptmpoid = ptmpoid->next;




	
	struct snval *psnval = NULL;
	if((ret = SN_VALUES(s, serv, community, 1, poid, &psnval)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_4%>"));//<%IDS_CiscoCPU_4%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_5%>"));//<%IDS_CiscoCPU_5%>
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_6%>"));//<%IDS_CiscoCPU_6%>
		else if(ret == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_7%>"));//<%IDS_CiscoCPU_7%>
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_8%>"));//<%IDS_CiscoCPU_8%>
		else if(ret == -6)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_9%>"));//<%IDS_CiscoCPU_9%>
		else if(ret == -7)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_10%>"));//<%IDS_CiscoCPU_10%>
		else if(ret == -8)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_11%>"));//<%IDS_CiscoCPU_11%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_12%>"));//<%IDS_CiscoCPU_12%>

		goto w;
	}

	ptmp = psnval;
	ptmpnext = NULL;

	while(ptmp)
	{
		/*
		switch(count)
		{
		case 1:
			sprintf(szReturn, "%s=", "utilization");
			break;
		case 2:
			sprintf(szReturn, "%s%s=", szReturn, "freeSpace");
			break;
		}*/

		switch(ptmp->sv_type)
		{
		case ASN1_INT:
			if (count==1) nBusy= ptmp->sv_val.sv_int;
			if (count==2) nFreeSpace= (float)ptmp->sv_val.sv_int;
			if (count==3) nTotalMem= (float)ptmp->sv_val.sv_int;
			 
			//sprintf(szReturn, "%s%d$", szReturn, ptmp->sv_val.sv_int);
			break;
		case ASN1_COUNTER:
		case ASN1_GAUGE:
		case ASN1_TIMETICKS:
		case ASN1_COUNTER64:
		case ASN1_UINTEGER32:
			 if (count==1) nBusy= ptmp->sv_val.sv_int;
			 if (count==2) nFreeSpace= (float)ptmp->sv_val.sv_int;
			 if (count==3) nTotalMem= (float)ptmp->sv_val.sv_int;
			 
			//sprintf(szReturn, "%s%u$", szReturn, ptmp->sv_val.sv_int);
			break;
		default:
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoCPU_13%>"));//<%IDS_CiscoCPU_13%>
			ret = -1;
			goto w;
		}

		count ++;
		ptmp = ptmp->next;
	}

w:	if(psnval)
	{
		ptmp = psnval;
		if(ptmp) ptmpnext = ptmp->next;
		while(ptmp)
		{
			free(ptmp);
			ptmp = ptmpnext;
			if(ptmp) ptmpnext = ptmp->next;
		}
	}
	nFreeSpace=nFreeSpace/1048576;
	nTotalMem=nTotalMem/1048576;
	
	if(strlen(szReturn)==0) 
		 sprintf(szReturn,"utilization=%d$freeSpace=%.2f$totalmem=%.2f$percentused=%.2f$",
				nBusy,nFreeSpace,nTotalMem,((nTotalMem-nFreeSpace)/nTotalMem)*100);

	return !ret;
}


