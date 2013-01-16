// RouteTable.cpp: implementation of the CRouteTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RouteTable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRouteTable::CRouteTable()
{
	m_strRouteDest = _T("");
	m_nRouteIfIndex = 0;
	m_nRouteMetric1 = 0;
	m_nRouteMetric2 = 0;
	m_nRouteMetric3 = 0;
	m_nRouteMetric4 = 0;
	m_strRouteNextHop = _T("");
	m_nRouteType = 0;
	m_nRouteProto = 0;
	m_nRouteAge = 0;
	m_strRouteMask = _T("");
	m_nRouteMetric5 = 0;

}

CRouteTable::~CRouteTable()
{

}

/****************************************************************************
 *	Get Route Information From a Router
 *
 *	Call WSA_Init() Before Call This Function And Call WSA_Free After This
 *	Function End.
 *
 *	Please Delete Default Route(dest = 0.0.0.0) Or Host Route(netmask=
 *	255.255.255.255) From lstRouteTable
 *
 */
BOOL GetRouteInfo(char *server, int port, char *community, CRouteTableList &lstRouteTable)
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

	ret = SN_NEXT_VALUE(s, serv, community, 1, &ipRouteTable_oid, poid, psnval);
	if(ret)
	{
		bResult = FALSE;
		goto err;
	}
	else
	{
		if(compare_oid(*poid, ipRouteTable_oid))
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

			CRouteTable *pRouteTable = new CRouteTable();
			pRouteTable->m_strRouteDest.Format("%u.%u.%u.%u", 
				psnval->sv_val.sv_ipaddr[0] & 0xFF, psnval->sv_val.sv_ipaddr[1] & 0xFF, 
				psnval->sv_val.sv_ipaddr[2] & 0xFF, psnval->sv_val.sv_ipaddr[3] & 0xFF);

			lstRouteTable.AddTail(pRouteTable);
		}
	}

	while(1)
	{
		ret = SN_NEXT_VALUE(s, serv, community, 1, poid, poid, psnval);
		if(ret)
			break;
		else
		{
			if(compare_oid(*poid, ipRouteDest_oid))
				break;
			else
			{
				if(psnval->sv_type != ASN1_IPADDR)
					break;

				CRouteTable *pRouteTable = new CRouteTable();
				pRouteTable->m_strRouteDest.Format("%u.%u.%u.%u", 
					psnval->sv_val.sv_ipaddr[0] & 0xFF, psnval->sv_val.sv_ipaddr[1] & 0xFF, 
					psnval->sv_val.sv_ipaddr[2] & 0xFF, psnval->sv_val.sv_ipaddr[3] & 0xFF);

				lstRouteTable.AddTail(pRouteTable);
			}
		}
	}

	pos = lstRouteTable.GetHeadPosition();
	while(pos)
	{
		CRouteTable *pRouteTable = lstRouteTable.GetNext(pos);
		
		u_char addr_1 = 0, addr_2 = 0, addr_3 = 0, addr_4 = 0;
		if(sscanf(pRouteTable->m_strRouteDest, "%d.%d.%d.%d", &addr_1, &addr_2, &addr_3, &addr_4) != 4)
			continue;

		struct oid ipRouteIfIndex = ipRouteIfIndex_oid;
		ipRouteIfIndex.id[ipRouteIfIndex.len ++] = addr_1;
		ipRouteIfIndex.id[ipRouteIfIndex.len ++] = addr_2;
		ipRouteIfIndex.id[ipRouteIfIndex.len ++] = addr_3;
		ipRouteIfIndex.id[ipRouteIfIndex.len ++] = addr_4;

		struct oid ipRouteMetric1 = ipRouteMetric1_oid;
		ipRouteMetric1.id[ipRouteMetric1.len ++] = addr_1;
		ipRouteMetric1.id[ipRouteMetric1.len ++] = addr_2;
		ipRouteMetric1.id[ipRouteMetric1.len ++] = addr_3;
		ipRouteMetric1.id[ipRouteMetric1.len ++] = addr_4;

		struct oid ipRouteMetric2 = ipRouteMetric2_oid;
		ipRouteMetric2.id[ipRouteMetric2.len ++] = addr_1;
		ipRouteMetric2.id[ipRouteMetric2.len ++] = addr_2;
		ipRouteMetric2.id[ipRouteMetric2.len ++] = addr_3;
		ipRouteMetric2.id[ipRouteMetric2.len ++] = addr_4;

		struct oid ipRouteMetric3 = ipRouteMetric3_oid;
		ipRouteMetric3.id[ipRouteMetric3.len ++] = addr_1;
		ipRouteMetric3.id[ipRouteMetric3.len ++] = addr_2;
		ipRouteMetric3.id[ipRouteMetric3.len ++] = addr_3;
		ipRouteMetric3.id[ipRouteMetric3.len ++] = addr_4;

		struct oid ipRouteMetric4 = ipRouteMetric4_oid;
		ipRouteMetric4.id[ipRouteMetric4.len ++] = addr_1;
		ipRouteMetric4.id[ipRouteMetric4.len ++] = addr_2;
		ipRouteMetric4.id[ipRouteMetric4.len ++] = addr_3;
		ipRouteMetric4.id[ipRouteMetric4.len ++] = addr_4;

		struct oid ipRouteNextHop = ipRouteNextHop_oid;
		ipRouteNextHop.id[ipRouteNextHop.len ++] = addr_1;
		ipRouteNextHop.id[ipRouteNextHop.len ++] = addr_2;
		ipRouteNextHop.id[ipRouteNextHop.len ++] = addr_3;
		ipRouteNextHop.id[ipRouteNextHop.len ++] = addr_4;

		struct oid ipRouteType = ipRouteType_oid;
		ipRouteType.id[ipRouteType.len ++] = addr_1;
		ipRouteType.id[ipRouteType.len ++] = addr_2;
		ipRouteType.id[ipRouteType.len ++] = addr_3;
		ipRouteType.id[ipRouteType.len ++] = addr_4;

		struct oid ipRouteProto = ipRouteProto_oid;
		ipRouteProto.id[ipRouteProto.len ++] = addr_1;
		ipRouteProto.id[ipRouteProto.len ++] = addr_2;
		ipRouteProto.id[ipRouteProto.len ++] = addr_3;
		ipRouteProto.id[ipRouteProto.len ++] = addr_4;

		struct oid ipRouteAge = ipRouteAge_oid;
		ipRouteAge.id[ipRouteAge.len ++] = addr_1;
		ipRouteAge.id[ipRouteAge.len ++] = addr_2;
		ipRouteAge.id[ipRouteAge.len ++] = addr_3;
		ipRouteAge.id[ipRouteAge.len ++] = addr_4;

		struct oid ipRouteMask = ipRouteMask_oid;
		ipRouteMask.id[ipRouteMask.len ++] = addr_1;
		ipRouteMask.id[ipRouteMask.len ++] = addr_2;
		ipRouteMask.id[ipRouteMask.len ++] = addr_3;
		ipRouteMask.id[ipRouteMask.len ++] = addr_4;

//		struct oid ipRouteMetric5 = ipRouteMetric5_oid;
//		ipRouteMetric5.id[ipRouteMetric5.len ++] = addr_1;
//		ipRouteMetric5.id[ipRouteMetric5.len ++] = addr_2;
//		ipRouteMetric5.id[ipRouteMetric5.len ++] = addr_3;
//		ipRouteMetric5.id[ipRouteMetric5.len ++] = addr_4;

		pioid = pitmpoid = &ipRouteIfIndex;
		pitmpoid->next = &ipRouteMetric1;
		pitmpoid = pitmpoid->next;
		pitmpoid->next = &ipRouteMetric2;
		pitmpoid = pitmpoid->next;
		pitmpoid->next = &ipRouteMetric3;
		pitmpoid = pitmpoid->next;
		pitmpoid->next = &ipRouteMetric4;
		pitmpoid = pitmpoid->next;
		pitmpoid->next = &ipRouteNextHop;
		pitmpoid = pitmpoid->next;
		pitmpoid->next = &ipRouteType;
		pitmpoid = pitmpoid->next;
		pitmpoid->next = &ipRouteProto;
		pitmpoid = pitmpoid->next;
		pitmpoid->next = &ipRouteAge;
		pitmpoid = pitmpoid->next;
		pitmpoid->next = &ipRouteMask;
		pitmpoid = pitmpoid->next;
//		pitmpoid->next = &ipRouteMetric5;
//		pitmpoid = pitmpoid->next;

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
				pRouteTable->m_nRouteIfIndex = ptmp->sv_val.sv_int;
				break;
			case 1:
				pRouteTable->m_nRouteMetric1 = ptmp->sv_val.sv_int;
				break;
			case 2:
				pRouteTable->m_nRouteMetric2 = ptmp->sv_val.sv_int;
				break;
			case 3:
				pRouteTable->m_nRouteMetric3 = ptmp->sv_val.sv_int;
				break;
			case 4:
				pRouteTable->m_nRouteMetric4 = ptmp->sv_val.sv_int;
				break;
			case 5:
				pRouteTable->m_strRouteNextHop.Format("%u.%u.%u.%u", 
					ptmp->sv_val.sv_ipaddr[0] & 0xFF, ptmp->sv_val.sv_ipaddr[1] & 0xFF, 
					ptmp->sv_val.sv_ipaddr[2] & 0xFF, ptmp->sv_val.sv_ipaddr[3] & 0xFF);
				break;
			case 6:
				pRouteTable->m_nRouteType = ptmp->sv_val.sv_int;
				break;
			case 7:
				pRouteTable->m_nRouteProto = ptmp->sv_val.sv_int;
				break;
			case 8:
				pRouteTable->m_nRouteAge = ptmp->sv_val.sv_int;
				break;
			case 9:
				pRouteTable->m_strRouteMask.Format("%u.%u.%u.%u", 
					ptmp->sv_val.sv_ipaddr[0] & 0xFF, ptmp->sv_val.sv_ipaddr[1] & 0xFF, 
					ptmp->sv_val.sv_ipaddr[2] & 0xFF, ptmp->sv_val.sv_ipaddr[3] & 0xFF);
				break;
//			case 10:
//				pRouteInfo->m_nRouteMetric5 = psnval->sv_val.sv_int;
//				break;
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
