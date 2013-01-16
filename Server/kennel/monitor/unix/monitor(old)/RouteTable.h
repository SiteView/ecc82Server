// RouteTable.h: interface for the CRouteTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROUTETABLE_H__77E8D165_FD0C_4EEB_AFB7_5FC38942137D__INCLUDED_)
#define AFX_ROUTETABLE_H__77E8D165_FD0C_4EEB_AFB7_5FC38942137D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SnmpUtils.h"

/* ipRouteTable */
/* This entity's IP Routing table. */
struct oid ipRouteTable_oid			= {{0x2b, 6, 1, 2, 1, 4, 21}, 7, "ipRouteTable", 0, ""};

/* The destination IP address of this route. An entry with a value of 0.0.0.0 is 
   considered a default route. Multiple routes to a single destination can appear 
   in the table, but access to such multiple entries is dependent on the table-access 
   mechanisms defined by the network management protocol in use. */
struct oid ipRouteDest_oid			= {{0x2b, 6, 1, 2, 1, 4, 21, 1, 1}, 9, "ipRouteDest", ASN1_IPADDR, ""};
/* The index value which uniquely identifies the local interface through which the 
   next hop of this route should be reached. The interface identified by a particular 
   value of this index is the same interface as identified by the same value of ifIndex. */
struct oid ipRouteIfIndex_oid		= {{0x2b, 6, 1, 2, 1, 4, 21, 1, 2}, 9, "ipRouteIfIndex", ASN1_INT, ""};
/* The primary routing metric for this route. The semantics of this metric are 
   determined by the routing-protocol specified in the route's ipRouteProto value. 
   If this metric is not used, its value should be set to -1. */
struct oid ipRouteMetric1_oid		= {{0x2b, 6, 1, 2, 1, 4, 21, 1, 3}, 9, "ipRouteMetric1", ASN1_INT, ""};
/* An alternate routing metric for this route. The semantics of this metric are 
   determined by the routing-protocol specified in the route's ipRouteProto value. 
   If this metric is not used, its value should be set to -1. */
struct oid ipRouteMetric2_oid		= {{0x2b, 6, 1, 2, 1, 4, 21, 1, 4}, 9, "ipRouteMetric2", ASN1_INT, ""};
/* An alternate routing metric for this route. The semantics of this metric are 
   determined by the routing-protocol specified in the route's ipRouteProto value. 
   If this metric is not used, its value should be set to -1. */
struct oid ipRouteMetric3_oid		= {{0x2b, 6, 1, 2, 1, 4, 21, 1, 5}, 9, "ipRouteMetric3", ASN1_INT, ""};
/* An alternate routing metric for this route. The semantics of this metric are 
   determined by the routing-protocol specified in the route's ipRouteProto value. 
   If this metric is not used, its value should be set to -1. */
struct oid ipRouteMetric4_oid		= {{0x2b, 6, 1, 2, 1, 4, 21, 1, 6}, 9, "ipRouteMetric4", ASN1_INT, ""};
/* The IP address of the next hop of this route. (In the case of a route bound to an 
   interface which is realized via a broadcast media, the value of this field is the 
   agent's IP address on that interface.) */
struct oid ipRouteNextHop_oid		= {{0x2b, 6, 1, 2, 1, 4, 21, 1, 7}, 9, "ipRouteNextHop", ASN1_IPADDR, ""};
/* The type of route. Note that the values direct(3) and indirect(4) refer to the 
   notion of direct and indirect routing in the IP architecture.

   Setting this object to the value invalid(2) has the effect of invalidating the 
   corresponding entry in the ipRouteTable object.  That is, it effectively 
   dissasociates the destination identified with said entry from the route 
   identified with said entry. It is an implementation-specific matter as to whether 
   the agent removes an invalidated entry from the table. Accordingly, management 
   stations must be prepared to receive tabular information from agents that 
   corresponds to entries not currently in use. Proper interpretation of such 
   entries requires examination of the relevant ipRouteType object. */
struct oid ipRouteType_oid			= {{0x2b, 6, 1, 2, 1, 4, 21, 1, 8}, 9, "ipRouteType", ASN1_INT, ""};
/* The routing mechanism via which this route was learned. Inclusion of values for 
   gateway routing protocols is not intended to imply that hosts should support 
   those protocols. */
struct oid ipRouteProto_oid			= {{0x2b, 6, 1, 2, 1, 4, 21, 1, 9}, 9, "ipRouteProto", ASN1_INT, ""};
/* The number of seconds since this route was last updated or otherwise determined 
   to be correct. Note that no semantics of `too old' can be implied except through 
   knowledge of the routing protocol by which the route was learned. */
struct oid ipRouteAge_oid			= {{0x2b, 6, 1, 2, 1, 4, 21, 1, 10}, 9, "ipRouteAge", ASN1_INT, ""};
/* Indicate the mask to be logical-ANDed with the destination address before being 
   compared to the value in the ipRouteDest field.  For those systems that do not 
   support arbitrary subnet masks, an agent constructs the value of the ipRouteMask 
   by determining whether the value of the correspondent ipRouteDest field belong 
   to a class-A, B, or C network, and then using one of:

		mask           network
        255.0.0.0      class-A
        255.255.0.0    class-B
        255.255.255.0  class-C

   If the value of the ipRouteDest is 0.0.0.0 (a default route), then the mask value 
   is also 0.0.0.0.  It should be noted that all IP routing subsystems implicitly 
   use this mechanism. */
struct oid ipRouteMask_oid			= {{0x2b, 6, 1, 2, 1, 4, 21, 1, 11}, 9, "ipRouteMask", ASN1_IPADDR, ""};
/* An alternate routing metric for this route. The semantics of this metric are 
   determined by the routing-protocol specified in the route's ipRouteProto value. 
   If this metric is not used, its value should be set to -1. */
struct oid ipRouteMetric5_oid		= {{0x2b, 6, 1, 2, 1, 4, 21, 1, 12}, 9, "ipRouteMetric5", ASN1_INT, ""};

class CRouteTable : public CObject
{
public:
	CRouteTable();
	virtual ~CRouteTable();

public:
	CString		m_strRouteDest;
	int			m_nRouteIfIndex;
	int			m_nRouteMetric1;
	int			m_nRouteMetric2;
	int			m_nRouteMetric3;
	int			m_nRouteMetric4;
	CString		m_strRouteNextHop;
	int			m_nRouteType;
	int			m_nRouteProto;
	int			m_nRouteAge;
	CString		m_strRouteMask;
	int			m_nRouteMetric5;

public:
	enum ipRouteTypeE 
	{
		TYPE_OTHER = 1, 
		INVALID, 
		DIRECT, 
		INDIRECT
	};

	enum ipRouteProtoE 
	{
		PROTO_OTHER = 1, 
		LOCAL, 
		NETMGMT, 
		ICMP, 
		EGP, 
		GGP, 
		HELLO, 
		RIP, 
		IS_IS, 
		ES_IS, 
		CISCOIGRP, 
		BBNSPFIGP, 
		OSPF, 
		BGP
	};
};

typedef CTypedPtrList< CObList, CRouteTable * > CRouteTableList;

#endif // !defined(AFX_ROUTETABLE_H__77E8D165_FD0C_4EEB_AFB7_5FC38942137D__INCLUDED_)
