// AddrTable.cpp: implementation of the CAddrTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AddrTable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAddrTable::CAddrTable()
{
	m_strAdEntAddr = _T("");
	m_nAdEntIfIndex = 0;
	m_strAdEntNetMask = _T("");
	m_nAdEntBcaseAddr = 0;
	m_nAdEntReasmMaxSize = 0;

}

CAddrTable::~CAddrTable()
{

}

/****************************************************************************
 *	Get Ip Addresses From a Router
 *
 *	Call WSA_Init() Before Call This Function And Call WSA_Free After This
 *	Function End.
 */
BOOL GetAddrInfo(char *server, int port, char *community, CAddrTableList &lstAddrTable)
{
	int				s, ret = 0;
	BOOL			bResult = TRUE;
	struct			sockaddr_in serv;
	POSITION		pos = NULL;
	struct oid		*pioid = NULL, *pitmpoid = NULL;
	struct snval	*pisnval = NULL;
	struct snval	*ptmp = NULL, *ptmpnext = NULL;

	if(MakeSocket(server, port, s, serv) < 0)
	{
		return FALSE;
	}

	struct oid *poid = (struct oid *)malloc(sizeof(struct oid));
	memset(poid, 0, sizeof(struct oid));
	struct snval *psnval = (struct snval *)malloc(sizeof(struct snval));
	memset(psnval, 0, sizeof(struct snval));

	ret = SN_NEXT_VALUE(s, serv, community, 1, &ipAddrTable_oid, poid, psnval);
	if(ret)
	{
		bResult = FALSE;
		goto err;
	}
	else
	{
		if(compare_oid(*poid, ipAddrTable_oid))
		{
			bResult = FALSE;
			goto err;
		}
		else
		{
			if(psnval->sv_type != ASN1_IPADDR)
			{
				bResult = FALSE;
				goto err;
			}

			CAddrTable *pAddrTable = new CAddrTable();
			pAddrTable->m_strAdEntAddr.Format("%u.%u.%u.%u", 
				psnval->sv_val.sv_ipaddr[0] & 0xFF, psnval->sv_val.sv_ipaddr[1] & 0xFF, 
				psnval->sv_val.sv_ipaddr[2] & 0xFF, psnval->sv_val.sv_ipaddr[3] & 0xFF);

			lstAddrTable.AddTail(pAddrTable);
		}
	}

	while(1)
	{
		ret = SN_NEXT_VALUE(s, serv, community, 1, poid, poid, psnval);
		if(ret)
			break;
		else
		{
			if(compare_oid(*poid, ipAdEntAddr_oid))
				break;
			else
			{
				if(psnval->sv_type != ASN1_IPADDR)
					break;

				CAddrTable *pAddrTable = new CAddrTable();
				pAddrTable->m_strAdEntAddr.Format("%u.%u.%u.%u", 
					psnval->sv_val.sv_ipaddr[0] & 0xFF, psnval->sv_val.sv_ipaddr[1] & 0xFF, 
					psnval->sv_val.sv_ipaddr[2] & 0xFF, psnval->sv_val.sv_ipaddr[3] & 0xFF);

				lstAddrTable.AddTail(pAddrTable);
			}
		}
	}

	pos = lstAddrTable.GetHeadPosition();
	while(pos)
	{
		CAddrTable *pAddrTable = lstAddrTable.GetNext(pos);
		
		u_char addr_1 = 0, addr_2 = 0, addr_3 = 0, addr_4 = 0;
		if(sscanf(pAddrTable->m_strAdEntAddr, "%d.%d.%d.%d", &addr_1, &addr_2, &addr_3, &addr_4) != 4)
			continue;

		struct oid ipAdEntIfIndex = ipAdEntIfIndex_oid;
		ipAdEntIfIndex.id[ipAdEntIfIndex.len ++] = addr_1;
		ipAdEntIfIndex.id[ipAdEntIfIndex.len ++] = addr_2;
		ipAdEntIfIndex.id[ipAdEntIfIndex.len ++] = addr_3;
		ipAdEntIfIndex.id[ipAdEntIfIndex.len ++] = addr_4;

		struct oid ipAdEntNetMask = ipAdEntNetMask_oid;
		ipAdEntNetMask.id[ipAdEntNetMask.len ++] = addr_1;
		ipAdEntNetMask.id[ipAdEntNetMask.len ++] = addr_2;
		ipAdEntNetMask.id[ipAdEntNetMask.len ++] = addr_3;
		ipAdEntNetMask.id[ipAdEntNetMask.len ++] = addr_4;

		struct oid ipAdEntBcastAddr = ipAdEntBcastAddr_oid;
		ipAdEntBcastAddr.id[ipAdEntBcastAddr.len ++] = addr_1;
		ipAdEntBcastAddr.id[ipAdEntBcastAddr.len ++] = addr_2;
		ipAdEntBcastAddr.id[ipAdEntBcastAddr.len ++] = addr_3;
		ipAdEntBcastAddr.id[ipAdEntBcastAddr.len ++] = addr_4;

		struct oid ipAdEntReasmMaxSize = ipAdEntReasmMaxSize_oid;
		ipAdEntReasmMaxSize.id[ipAdEntReasmMaxSize.len ++] = addr_1;
		ipAdEntReasmMaxSize.id[ipAdEntReasmMaxSize.len ++] = addr_2;
		ipAdEntReasmMaxSize.id[ipAdEntReasmMaxSize.len ++] = addr_3;
		ipAdEntReasmMaxSize.id[ipAdEntReasmMaxSize.len ++] = addr_4;

		pioid = pitmpoid = &ipAdEntIfIndex;
		pitmpoid->next = &ipAdEntNetMask;
		pitmpoid = pitmpoid->next;
		pitmpoid->next = &ipAdEntBcastAddr;
		pitmpoid = pitmpoid->next;
		pitmpoid->next = &ipAdEntReasmMaxSize;
		pitmpoid = pitmpoid->next;

		ptmp = pisnval, ptmpnext = NULL;
		if(ptmp) ptmpnext = ptmp->next;
		while(ptmp)
		{
			free(ptmp);
			ptmp = ptmpnext;
			if(ptmp) ptmpnext = ptmp->next;
		}
		pisnval = NULL;
		
		if((ret = SN_VALUES(s, serv, community, 1, pioid, &pisnval)) < 0)
		{
			bResult = FALSE;
			goto err;
		}

		int		count = 0;
		struct	snval *ptmp = pisnval;
		struct	snval *ptmpnext = NULL;
		while(ptmp)
		{
			switch(count)
			{
			case 0:
				pAddrTable->m_nAdEntIfIndex = ptmp->sv_val.sv_int;
				break;
			case 1:
				pAddrTable->m_strAdEntNetMask.Format("%u.%u.%u.%u", 
					ptmp->sv_val.sv_ipaddr[0] & 0xFF, ptmp->sv_val.sv_ipaddr[1] & 0xFF, 
					ptmp->sv_val.sv_ipaddr[2] & 0xFF, ptmp->sv_val.sv_ipaddr[3] & 0xFF);
				break;
			case 2:
				pAddrTable->m_nAdEntBcaseAddr = ptmp->sv_val.sv_int;
				break;
			case 3:
				pAddrTable->m_nAdEntReasmMaxSize = ptmp->sv_val.sv_int;
				break;
			}

			count ++;
			ptmp = ptmp->next;
		}
	}

err:
	if(poid) free(poid);
	if(psnval) free(psnval);
	ptmp = pisnval, ptmpnext = NULL;
	if(ptmp) ptmpnext = ptmp->next;
	while(ptmp)
	{
		free(ptmp);
		ptmp = ptmpnext;
		if(ptmp) ptmpnext = ptmp->next;
	}

	return bResult;
}
