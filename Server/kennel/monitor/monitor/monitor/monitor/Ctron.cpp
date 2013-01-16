
#include "Ctron.h"
#include "SnmpUtils.h"

#include <Base\funcGeneral.h>

struct oid capCPUTable = {{0x2b, 6, 1, 4, 1, 52, 2501, 1, 270, 2, 1}, 11, "", 0, ""};
struct oid capCPUCurrentUtilization = {{0x2b, 6, 1, 4, 1, 52, 2501, 1, 270, 2, 1, 1, 2, 1}, 14, "", ASN1_INT, ""};

BOOL CTRONSSRCPU_MONITOR(char *server, int port, char *community, int rdelay, 
						 int timeout, char *cpath, char *szReturn)
{
	int		s;
	struct	sockaddr_in serv;

	int		ret = 0;
	BOOL	bResult = TRUE;
	struct	oid *poid = NULL, *proid = NULL;
	struct	snval *psnval = NULL;

	*szReturn = 0;

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

	poid = (struct oid *)malloc(sizeof(struct oid));
	proid = (struct oid *)malloc(sizeof(struct oid));
	psnval = (struct snval *)malloc(sizeof(struct snval));
	if(poid == NULL || proid == NULL || psnval == NULL)
	{
		bResult = FALSE;
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CTRON_01%>"));	// 内存溢出错误
		goto err;
	}

	memset(poid, 0, sizeof(struct oid));
	memset(proid, 0, sizeof(struct oid));
	memset(psnval, 0, sizeof(struct snval));

	memcpy(poid, &capCPUTable, sizeof(struct oid));
	while(SN_NEXT_VALUE(s, serv, community, 1, poid, proid, psnval) == 0)
	{
		if(compare_oid(*proid, capCPUCurrentUtilization) || psnval->sv_type != ASN1_INT ||
					psnval->sv_val.sv_int > 100 || psnval->sv_val.sv_int < 0)
		{
			break;
		}

		sprintf(szReturn, "%sUtilization=%d$", szReturn, psnval->sv_val.sv_int);

		memcpy(poid, proid, sizeof(struct oid));
		memset(proid, 0, sizeof(struct oid));
		memset(psnval, 0, sizeof(struct snval));
	}

	if(!*szReturn)
	{
		bResult = FALSE;
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CTRON_02%>"));	// 设备无此性能参数
	}

err:
	if(poid) { free(poid); poid = NULL; }
	if(proid) { free(proid); proid = NULL; }
	if(psnval) { free(psnval); psnval = NULL; }
	
	return bResult;
}

struct oid etherStatsPkts64Octets = {{0x2b, 6, 1, 2, 1, 16, 1, 1, 1, 14}, 10, "", ASN1_COUNTER, ""};
struct oid etherStatsPkts65to127Octets = {{0x2b, 6, 1, 2, 1, 16, 1, 1, 1, 15}, 10, "", ASN1_COUNTER, ""};

BOOL CTRONETHERSTATUS_MONITOR(char *server, int port, char *community, CString strIndex, 
			int rdelay, int timeout, int ngid, int nmid, char *cpath, char *szReturn)
{
	int		s;
	struct	sockaddr_in serv;

	int		i = 0, j = 0, ret = 0;
	int		count = 1;
	struct	snval *ptmp = NULL;
	struct	snval *ptmpnext = NULL;

	u_int	_64Octets = 0, _65to127Octets = 0;
	u_int	_Curr64Octets = 0, _Curr65to127Octets = 0;

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

	for(i = 0;i < index_id_c;i ++)
	{
		etherStatsPkts64Octets.id[etherStatsPkts64Octets.len++] = index_id[i];
		etherStatsPkts65to127Octets.id[etherStatsPkts65to127Octets.len ++] = index_id[i];
	}

	struct oid *poid = NULL, *ptmpoid = NULL;
	poid = ptmpoid = &etherStatsPkts64Octets;
	ptmpoid->next = &etherStatsPkts65to127Octets;
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

	_Curr64Octets = psnval->sv_val.sv_int;
	_Curr65to127Octets = psnval->next->sv_val.sv_int;

	{
	char ciniFile[256] = {0}, 
		 cinifile[256] = {0};
	char cifIndex[256] = {0};
	sprintf(cinifile, "ctron_%s_%d_%d.ini", server, ngid, nmid);
	sprintf(ciniFile, "%s\\%s", cpath, cinifile);
	sprintf(cifIndex, "ifIndex_%s", strIndex);

	long	ifTime = 0;
	CTime	curTime = CTime::GetCurrentTime();

	ifTime = DFN_GetPrivateProfileInt(cifIndex, "ifTime", 0, ciniFile);
	_64Octets = DFN_GetPrivateProfileInt(cifIndex, "_64Octets", 0, ciniFile);
	_65to127Octets = DFN_GetPrivateProfileInt(cifIndex, "_65to127Octets", 0, ciniFile);

	if(ifTime == 0 && _64Octets == 0 && _65to127Octets == 0)
	{
		CString strSection = _T("");
		strSection.Format("ifIndex_%s", strIndex);

		CString strMsg = _T("");
		strMsg.Format("%ld", curTime.GetTime());
		::DFN_WritePrivateProfileString((LPCTSTR)strSection, "ifTime", (LPCTSTR)strMsg, ciniFile);

		strMsg.Format("%ld", _Curr64Octets);
		::DFN_WritePrivateProfileString((LPCTSTR)strSection, "_64Octets", (LPCTSTR)strMsg, ciniFile);

		strMsg.Format("%ld", _Curr65to127Octets);
		::DFN_WritePrivateProfileString((LPCTSTR)strSection, "_65to127Octets", (LPCTSTR)strMsg, ciniFile);
	}
	else
	{
		float	_Rate64Octets = 0.0, _Rate65to127Octets = 0.0;
		Counter	_Delta64Octets = 0, _Delta65to127Octets = 0;

		CTime bTime(ifTime);
		CTimeSpan spanTime = curTime - bTime;
		long lInterval = spanTime.GetTotalSeconds();
		
		if(lInterval == 0) lInterval = 1;
		if(lInterval <= 12 * 60 * 60 && lInterval > 0)
		{
			_Delta64Octets = (unsigned int)(_Curr64Octets - _64Octets < 0 ? (COUNTER_MAX - _64Octets + _Curr64Octets) : (_Curr64Octets - _64Octets));
			_Rate64Octets = (float)_Delta64Octets / lInterval;

			_Delta65to127Octets = (unsigned int)(_Curr65to127Octets - _65to127Octets < 0 ? (COUNTER_MAX - _65to127Octets + _Curr65to127Octets) : (_Curr65to127Octets - _65to127Octets));
			_Rate65to127Octets = (float)_Delta65to127Octets / lInterval;

			sprintf(szReturn, "_Rate64Octets=%.2f$_Rate65to127Octets=%.2f$",
					_Rate64Octets, _Rate65to127Octets);
		}

		char buff[256] = {0};
		memset(buff, 0, 256);
		ltoa(curTime.GetTime(), buff, 10);
		DFN_WritePrivateProfileString(cifIndex, "ifTime", buff, ciniFile);
		memset(buff, 0, 256);
		ltoa(_Curr64Octets, buff, 10);
		DFN_WritePrivateProfileString(cifIndex, "_64Octets", buff, ciniFile);
		memset(buff, 0, 256);
		ltoa(_Curr65to127Octets, buff, 10);
		DFN_WritePrivateProfileString(cifIndex, "_65to127Octets", buff, ciniFile);
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


struct oid etherStatsTable = {{0x2b, 6, 1, 2, 1, 16, 1, 1}, 8, "", 0, ""};
struct oid etherStatsIndex = {{0x2b, 6, 1, 2, 1, 16, 1, 1, 1, 1}, 10, "", ASN1_INT, ""};
struct oid etherStatsDataSource = {{0x2b, 6, 1, 2, 1, 16, 1, 1, 1, 2}, 10, "", ASN1_OBJID, ""};
struct oid ifDescr = {{0x2b, 6, 1, 2, 1, 2, 2, 1, 2}, 9, "", ASN1_OCTSTR, ""};

BOOL CtronEtherIndexs(char *server, int port, char *community, int rdelay, 
					  int timeout, char *cpath, CString &strBuffer)
{
	int		s;
	struct	sockaddr_in serv;

	int		nEther = 0;
	int		nEthers[256] = {0};
	struct	oid nEtherDs[256];
	int		nIfIndex[256] = {0};
	char	IfDescr[256][64] = {0};

	BOOL	bResult = TRUE;
	int		i = 0;
	struct	oid *poid = NULL, *proid = NULL;
	struct	snval *psnval = NULL;

	strBuffer = _T("");

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

	memcpy(poid, &etherStatsTable, sizeof(struct oid));
	memset(proid, 0, sizeof(struct oid));
	memset(psnval, 0, sizeof(struct snval));

	// Get Rmon Index
	while(SN_NEXT_VALUE(s, serv, community, 1, poid, proid, psnval) == 0)
	{
		if(compare_oid(*proid, etherStatsIndex) || psnval->sv_type != ASN1_INT)
		{
			break;
		}
		
		nEthers[nEther ++] = psnval->sv_val.sv_int;
		if(nEther >= 256)
		{
			break;
		}

		memcpy(poid, proid, sizeof(struct oid));
		memset(proid, 0, sizeof(struct oid));
		memset(psnval, 0, sizeof(struct snval));
	}

	if(nEther == 0)
	{
		bResult = FALSE;
		goto err;
	}

	// Get Rmon Data Source
	for(i = 0;i < nEther;i ++)
	{
		struct oid ds = etherStatsDataSource;
		ds.id[ds.len ++] = nEthers[i];

		memset(psnval, 0, sizeof(struct snval));
		if(SN_VALUE(s, serv, community, 1, &ds, psnval) < 0)
		{
			bResult = FALSE;
			goto err;
		}

		if(psnval->sv_type != ASN1_OBJID)
		{
			bResult = FALSE;
			goto err;
		}

		nEtherDs[i] = psnval->sv_val.sv_oid;
	}

	// Get Interface Index
	for(i = 0;i < nEther;i ++)
	{
		memset(psnval, 0, sizeof(struct snval));
		if(SN_VALUE(s, serv, community, 1, &nEtherDs[i], psnval) < 0)
		{
			bResult = FALSE;
			goto err;
		}

		if(psnval->sv_type != ASN1_INT)
		{
			bResult = FALSE;
			goto err;
		}

		nIfIndex[i] = psnval->sv_val.sv_int;
	}

	// Get Interface Description
	for(i = 0;i < nEther;i ++)
	{
		struct oid descr = ifDescr;
		descr.id[descr.len ++] = nIfIndex[i];

		memset(psnval, 0, sizeof(struct snval));
		if(SN_VALUE(s, serv, community, 1, &descr, psnval) < 0)
		{
			bResult = FALSE;
			goto err;
		}

		if(psnval->sv_type != ASN1_OCTSTR)
		{
			bResult = FALSE;
			goto err;
		}

		strncpy(IfDescr[i], psnval->sv_val.sv_str.sv_str, psnval->sv_val.sv_str.sv_len);
	}

	// Organize Output
	for(i = 0;i < nEther;i ++)
	{
		char szBuffer[1024] = {0};
		sprintf(szBuffer, "%s%d###%s###$$$", szBuffer, nEthers[i], IfDescr[i]);
		strBuffer += szBuffer;
	}

err:
	if(poid) { free(poid); poid = NULL; }
	if(proid) { free(proid); proid = NULL; }
	if(psnval) { free(psnval); psnval = NULL; }

	return bResult;
}