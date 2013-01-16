
#include "Traffic.h"
#include "Utils.h"

#include "base\funcGeneral.h"

/* Interfaces Group Table Identifier*/
struct oid ifIndex_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 1}, 9};
struct oid ifDescr_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 2}, 9};
struct oid ifType_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 3}, 9};
struct oid ifMtu_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 4}, 9};
struct oid ifSpeed_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 5}, 9};
struct oid ifPhysAddress_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 6}, 9};
struct oid ifAdminStatus_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 7}, 9};
struct oid ifOperStatus_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 8}, 9};
struct oid ifLastChange_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 9}, 9};
//struct oid ifInOctets_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 10}, 9};
struct oid ifInUcastPkts_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 11}, 9};
struct oid ifInNUcastPkts_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 12}, 9};
//struct oid ifInDiscards_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 13}, 9};
struct oid ifInErrors_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 14}, 9};
struct oid ifInUnknownProtos_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 15}, 9};
//struct oid ifOutOctets_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 16}, 9};
struct oid ifOutUcastPkts_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 17}, 9};
struct oid ifOutNUcastPkts_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 18}, 9};
//struct oid ifOutDiscards_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 19}, 9};
struct oid ifOutErrors_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 20}, 9};
struct oid ifOutQLen_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 21}, 9};
struct oid ifSpecific_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 22}, 9};


void WriteLogFile(CString strMonitorID, CString strMsg)
{
    FILE *stream;
    char chTime[50] = {0};
    //打开文件
    stream=fopen(FuncGetInstallRootPath() + "\\Temp\\" +strMonitorID + ".log","a");
    if(stream == NULL)
        return;//打开文件失败，直接返回   	
    SYSTEMTIME time;
	//取系统当前时间
	GetLocalTime(&time);
    sprintf(chTime ,"%02dY-%02dM-%02dD %02dH-%02dM-%02dS ",
				time.wYear - 2000,
				time.wMonth,
				time.wDay,
				time.wHour,
				time.wMinute,
				time.wSecond
			);
    fputs(chTime , stream);
	fputs(strMsg.GetBuffer(strMsg.GetLength()),stream);
    fputs("\n",stream);
	fclose(stream);
}

BOOL TRAFFIC_MONITOR(char *server, int port, char *community, int ifindex, 
					 int timeout, int retrydelay, CString strMonitorID, 
					 char *custpath, char *szReturn)
{
	int s;
	struct sockaddr_in serv;

	int		i = 0, ret = 0;
	int		count = 1;
	struct	snval *ptmp = NULL;
	struct	snval *ptmpnext = NULL;

	long	ifOperStatus = 0;
	Gauge	ifSpeed = 0;
	Counter ifInOctets = 0, ifOutOctets = 0;
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
	oid ifInOctets_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 10}, 9};
	oid ifOutOctets_oid = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 16}, 9};
	ifInOctets_oid.id[ifInOctets_oid.len++] = ifindex;
	poid = ptmpoid = &ifInOctets_oid;
	ifOutOctets_oid.id[ifOutOctets_oid.len++] = ifindex;
	ptmpoid->next = &ifOutOctets_oid;
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
			case ASN1_INT:
			case ASN1_COUNTER:
			case ASN1_GAUGE:
			case ASN1_TIMETICKS:
			case ASN1_COUNTER64:
			case ASN1_UINTEGER32:
				ifInOctets = ptmp->sv_val.sv_int;
				break;
			default:
				ifInOctets = 0;
				break;
			}
			break;
		case 2:
			switch(ptmp->sv_type)
			{
			case ASN1_INT:
			case ASN1_COUNTER:
			case ASN1_GAUGE:
			case ASN1_TIMETICKS:
			case ASN1_COUNTER64:
			case ASN1_UINTEGER32:
				ifOutOctets = ptmp->sv_val.sv_int;
				break;
			default:
				ifOutOctets = 0;
				break;
			}
			break;
		}
		count ++;
		ptmp = ptmp->next;
	}

	// computer the traffic 
	{
	char ciniFile[256] = {0}, 
		 cinifile[256] = {0};
	char cifIndex[256] = {0};
	sprintf(cinifile, "snmp_%s_%s.ini", server, strMonitorID);
	sprintf(ciniFile, "%s\\%s", custpath, cinifile);
	sprintf(cifIndex, "ifIndex_%ld", ifindex);

	long	ifTime = 0;
	Counter inOctets = 0, outOctets = 0;

	CTime curTime = CTime::GetCurrentTime();

	ifTime = DFN_GetPrivateProfileInt(cifIndex, "ifTime", -99, ciniFile);
	inOctets = DFN_GetPrivateProfileInt(cifIndex, "ifInOctets", -99, ciniFile);
	outOctets = DFN_GetPrivateProfileInt(cifIndex, "ifOutOctets", -99, ciniFile);

	if(ifTime == -99 || inOctets == -99 || outOctets == -99)
	{
		CString strSection = _T("");
		strSection.Format("ifIndex_%ld", ifindex);

		curTime = CTime::GetCurrentTime();
		CString strMsg = _T("");
		strMsg.Format("%ld", curTime.GetTime());
		::DFN_WritePrivateProfileString((LPCTSTR)strSection, "ifTime", (LPCTSTR)strMsg, ciniFile);

		strMsg.Format("%ld", ifInOctets);
		::DFN_WritePrivateProfileString((LPCTSTR)strSection, "ifInOctets", (LPCTSTR)strMsg, ciniFile);
		strMsg.Format("%ld", ifOutOctets);
		::DFN_WritePrivateProfileString((LPCTSTR)strSection, "ifOutOctets", (LPCTSTR)strMsg, ciniFile);
		WriteLogFile(strMonitorID, "读配置文件失败");
	}
	else
	{
		float ifInOctetsRate = 0.0, ifOutOctetsRate = 0.0;
		Counter	InOctets = 0, OutOctets = 0;

		CTime bTime(ifTime);
		curTime = CTime::GetCurrentTime();
		CTimeSpan spanTime = curTime - bTime;
		long lInterval = spanTime.GetTotalSeconds();
		
		if(lInterval == 0) lInterval = 1;
		if(lInterval <= 12 * 60 * 60 && lInterval >= 0)
		{
			InOctets = (unsigned int)(ifInOctets - inOctets < 0 ? (COUNTER_MAX - inOctets + ifInOctets) : (ifInOctets - inOctets));
			//ifInOctetsRate = (float)InOctets / lInterval;
			//k
			ifInOctetsRate = (float)InOctets / lInterval/1024;

			OutOctets = (unsigned int)(ifOutOctets - outOctets < 0 ? (COUNTER_MAX - outOctets + ifOutOctets) : (ifOutOctets - outOctets));
			// wang peng /1024 =k
            //Edit By Kevin.Yang PM: 14:55 2004.10.21
            //Reason: 根据张鹏所提供的资料目前Cisio的MIF库中单位为KB，因此在
            //得到的数据后不再需要除以1024
			ifOutOctetsRate = (float)OutOctets / lInterval /1024;
			//ifOutOctetsRate = (float)OutOctets / lInterval;

			sprintf(szReturn, 
					"ifIndex=%ld$ifInOctetsRate=%.2f$ifOutOctetsRate=%.2f$",
					ifindex, 
					ifInOctetsRate * 8, 
					ifOutOctetsRate * 8);
		}
		else
		{
			CString strLog;
			strLog.Format("时间总秒数%d", lInterval);
//			WriteLogFile(strMonitorID, strLog);		
		}
//		WriteLogFile(strMonitorID, szReturn);
		//if(strlen(szReturn) == 0)
		//{
		//	printf("%d年%d月%d日 %d时%d分%d秒 \n%d年%d月%d日 %d时%d分%d秒 \n",
		//		curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), 
		//		curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond(),
		//		bTime.GetYear(), bTime.GetMonth(), bTime.GetDay(),
		//		bTime.GetHour(), bTime.GetMinute(), bTime.GetSecond());
		//}
		char buff[256] = {0};
		memset(buff, 0, 256);
		ltoa(curTime.GetTime(), buff, 10);
		DFN_WritePrivateProfileString(cifIndex, "ifTime", buff, ciniFile);
		memset(buff, 0, 256);
		ltoa(ifInOctets, buff, 10);
		DFN_WritePrivateProfileString(cifIndex, "ifInOctets", buff, ciniFile);
		memset(buff, 0, 256);
		ltoa(ifOutOctets, buff, 10);
		DFN_WritePrivateProfileString(cifIndex, "ifOutOctets", buff, ciniFile);
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

/////////////////////////////////////////////////////////////////////////////////
void StoreValues(CInterfaceTable *pifTable, int Item, struct snval *psnval, int ret)
{
	int	k = 0;

	switch(Item)
	{
	case IFINDEX:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || 
			psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS
			|| psnval->sv_type == ASN1_COUNTER))
		{
			pifTable->ifIndex = psnval->sv_val.sv_int;
		}
		else
			pifTable->ifIndex = ret;
		break;
	case IFDESCR:
		if(ret == 0 && psnval->sv_type == ASN1_OCTSTR)
			for(k = 0;k < psnval->sv_val.sv_str.sv_len;k ++)
				pifTable->ifDescr += *(psnval->sv_val.sv_str.sv_str + k);
		else
			pifTable->ifDescr.Format("%d", ret);
		break;
	case IFTYPE:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pifTable->ifType = psnval->sv_val.sv_int;
		else 
			pifTable->ifType = ret;
		break;
	case IFMTU:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pifTable->ifMtu = psnval->sv_val.sv_int;
		else
			pifTable->ifMtu = ret;
		break;
	case IFSPEED:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pifTable->ifSpeed = psnval->sv_val.sv_int;
		else
			pifTable->ifSpeed = ret;
		break;
	case IFPHYSADDRESS:
		if(ret == 0 && psnval->sv_type == ASN1_OCTSTR)
			if(psnval->sv_val.sv_str.sv_len < 6)
				pifTable->ifPhysAddress = _T("");
			else
			{
				pifTable->ifPhysAddress.Format("%x-%x-%x-%x-%x-%x", 
						*(psnval->sv_val.sv_str.sv_str + 0) & 0xFF, 
						*(psnval->sv_val.sv_str.sv_str + 1) & 0xFF, 
						*(psnval->sv_val.sv_str.sv_str + 2) & 0xFF, 
						*(psnval->sv_val.sv_str.sv_str + 3) & 0xFF, 
						*(psnval->sv_val.sv_str.sv_str + 4) & 0xFF, 
						*(psnval->sv_val.sv_str.sv_str + 5) & 0xFF); 
			}
		else
			pifTable->ifPhysAddress.Format("%d", ret);
		break;
	case IFADMINSTATUS:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pifTable->ifAdminStatus = psnval->sv_val.sv_int;
		else
			pifTable->ifAdminStatus = ret;
		break;
	case IFOPERSTATUS:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pifTable->ifOperStatus = psnval->sv_val.sv_int;
		else
			pifTable->ifOperStatus = ret;
		break;
	case IFLASTCHANGE:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pifTable->ifLastChange = psnval->sv_val.sv_int;
		else
			pifTable->ifLastChange = 0;
		break;
	case IFINOCTETS:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pifTable->ifInOctets = psnval->sv_val.sv_int;
		else
			pifTable->ifInOctets = ret;
		break;
	case IFINUCASTPKTS:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pifTable->ifInUcastPkts = psnval->sv_val.sv_int;
		else
			pifTable->ifInUcastPkts = ret;
		break;
	case IFINNUCASTPKTS:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pifTable->ifInNUcastPkts = psnval->sv_val.sv_int;
		else
			pifTable->ifInNUcastPkts = ret;
		break;
	case IFINDISCARDS:
		if(ret = 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pifTable->ifInDiscards = psnval->sv_val.sv_int;
		else
			pifTable->ifInDiscards = ret;
		break;
	case IFINERRORS:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pifTable->ifInErrors = psnval->sv_val.sv_int;
		else
			pifTable->ifInErrors = ret;
		break;
	case IFINUNKNOWNPROTOS:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pifTable->ifInUnknownProtos = psnval->sv_val.sv_int;
		else
			pifTable->ifInUnknownProtos = ret;
		break;
	case IFOUTOCTETS:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pifTable->ifOutOctets = psnval->sv_val.sv_int;
		else
			pifTable->ifOutOctets = ret;
		break;
	case IFOUTUCASTPKTS:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pifTable->ifOutUcastPkts = psnval->sv_val.sv_int;
		else
			pifTable->ifOutUcastPkts = ret;
		break;
	case IFOUTNUCASTPKTS:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pifTable->ifOutNUcastPkts = psnval->sv_val.sv_int;
		else
			pifTable->ifOutNUcastPkts = ret;
		break;
	case IFOUTDISCARDS:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pifTable->ifOutDiscards = psnval->sv_val.sv_int;
		else
			pifTable->ifOutDiscards = ret;
		break;
	case IFOUTERRORS:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS || psnval->sv_type == ASN1_COUNTER))
			pifTable->ifOutErrors = psnval->sv_val.sv_int;
		else
			pifTable->ifOutErrors = ret;
		break;
	case IFOUTQLEN:
		if(ret == 0 && (psnval->sv_type == ASN1_INT || psnval->sv_type == ASN1_GAUGE || psnval->sv_type == ASN1_TIMETICKS && psnval->sv_type == ASN1_COUNTER))
			pifTable->ifOutQLen = psnval->sv_val.sv_int;
		else
			pifTable->ifOutQLen = ret;
		break;
	case IFSPECIFIC:
		if(ret == 0 && psnval->sv_type == ASN1_OBJID)
			for(k = 0;k < psnval->sv_val.sv_oid.len;k ++)
			{
				char buf[1024] = {0};
				_itoa(*(psnval->sv_val.sv_oid.id + k), buf, 10);
				if(k == psnval->sv_val.sv_oid.len - 1)
					pifTable->ifSpecific += buf;
				else if(k == 0 && !strcmp(buf, "43"))
				{
					pifTable->ifSpecific += "1.3.";
				}
				else
				{
					pifTable->ifSpecific += buf;
					pifTable->ifSpecific += '.';
				}
			}
		else
			pifTable->ifSpecific.Format("%d", ret);
		break;
	}
}

//////////////////////////////////////////////////////////////////////
// CInterfaceTable Member Functions
//////////////////////////////////////////////////////////////////////
CInterfaceTable::CInterfaceTable()
{
	ifIndex = 0;
	ifDescr = _T("");
	ifType = 0;
	ifMtu = 0;
	ifSpeed = 0;
	ifPhysAddress = _T("");
	ifAdminStatus = 0;
	ifOperStatus = 0;
	ifLastChange = 0;
	ifInOctets = 0;
	ifInUcastPkts = 0;
	ifInNUcastPkts = 0;
	ifInDiscards = 0;
	ifInErrors = 0;
	ifInUnknownProtos = 0;
	ifOutOctets = 0;
	ifOutUcastPkts = 0;
	ifOutNUcastPkts = 0;
	ifOutDiscards = 0;
	ifOutErrors = 0;
	ifOutQLen = 0;
	ifSpecific = _T("");

	strOther = _T("");
}

CInterfaceTable::~CInterfaceTable()
{

}

void AddToTrafficLog(LPCTSTR msg)
{
	FILE *fp = fopen(".\\traffic.log", "a+");
	if(fp != NULL)
	{
		fprintf(fp, "%s\r\n", msg);
		fclose(fp);
	}
}