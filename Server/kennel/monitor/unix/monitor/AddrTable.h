// AddrTable.h: interface for the CAddrTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADDRTABLE_H__448962EF_D1DF_498A_9243_52808DF814FA__INCLUDED_)
#define AFX_ADDRTABLE_H__448962EF_D1DF_498A_9243_52808DF814FA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SnmpUtils.h"

/* ipAddrTable */
/* The table of addressing information relevant to this entity's IP addresses. */
struct oid ipAddrTable_oid			= {{0x2b, 6, 1, 2, 1, 4, 20}, 7, "ipAddrTable", 0, ""};

/* The IP address to which this entry's addressing information pertains. */
struct oid ipAdEntAddr_oid			= {{0x2b, 6, 1, 2, 1, 4, 20, 1, 1}, 9, "ipAdEntAddr", ASN1_IPADDR, ""};
/* The index value which uniquely identifies the interface to which this entry is 
   applicable. The interface identified by a particular value of this index is the 
   same interface as identified by the same value of ifIndex. */
struct oid ipAdEntIfIndex_oid		= {{0x2b, 6, 1, 2, 1, 4, 20, 1, 2}, 9, "ipAdEntIfIndex", ASN1_INT, ""};
/* The subnet mask associated with the IP address of this entry. The value of the 
   mask is an IP address with all the network bits set to 1 and all the hosts bits 
   set to 0. */
struct oid ipAdEntNetMask_oid		= {{0x2b, 6, 1, 2, 1, 4, 20, 1, 3}, 9, "ipAdEntNetMask", ASN1_IPADDR, ""};
/* The value of the least-significant bit in the IP broadcast address used for 
   sending datagrams on the (logical) interface associated with the IP address of 
   this entry. For example, when the Internet standard all-ones broadcast address 
   is used, the value will be 1.  This value applies to both the subnet and network 
   broadcasts addresses used by the entity on this (logical) interface. */
struct oid ipAdEntBcastAddr_oid		= {{0x2b, 6, 1, 2, 1, 4, 20, 1, 4}, 9, "ipAdEntBcastAddr", ASN1_IPADDR, ""};
/* The size of the largest IP datagram which this entity can re-assemble from 
   incoming IP fragmented datagrams received on this interface. */
struct oid ipAdEntReasmMaxSize_oid	= {{0x2b, 6, 1, 2, 1, 4, 20, 1, 5}, 9, "ipAdEntReasmMaxSize", ASN1_INT, ""};

class CAddrTable : public CObject
{
public:
	CAddrTable();
	virtual ~CAddrTable();

public:
	CString		m_strAdEntAddr;
	int			m_nAdEntIfIndex;
	CString		m_strAdEntNetMask;
	int			m_nAdEntBcaseAddr;
	int			m_nAdEntReasmMaxSize;

};

typedef CTypedPtrList< CObList, CAddrTable * > CAddrTableList;

#endif // !defined(AFX_ADDRTABLE_H__448962EF_D1DF_498A_9243_52808DF814FA__INCLUDED_)
