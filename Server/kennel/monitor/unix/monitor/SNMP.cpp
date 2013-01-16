
#include "SNMP.h"
#include "SnmpUtils.h"

#include "base\funcGeneral.h"

BOOL SNMP_MONITOR(char *server, int port, char *object, char *community, int timeout, 
				  int retrydelay, char *match, char *custpath, char *szReturn)
{
	int s, ret = 0;
	struct sockaddr_in serv;

	if((ret = MakeSocket(server, port, s, serv)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SNMP_1%>"));    // <%IDS_SNMP_1%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SNMP_2%>"));      //  <%IDS_SNMP_2%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SNMP_3%>"));        //  <%IDS_SNMP_3%>

		return FALSE;
	}

	struct oid *oidp = VerifyObject(object);
	if(!oidp)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SNMP_4%>"));         //  <%IDS_SNMP_4%>
		return FALSE;
	}

	struct snval snval;
	if((ret = SN_VALUE(s, serv, community, 1, oidp, &snval)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SNMP_5%>"));      //  <%IDS_SNMP_5%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SNMP_6%>"));   //  <%IDS_SNMP_6%>
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SNMP_7%>"));     //  <%IDS_SNMP_7%>
		else if(ret == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SNMP_8%>"));      //  <%IDS_SNMP_8%>
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SNMP_9%>"));      //  <%IDS_SNMP_9%>
		else if(ret == -6)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SNMP_10%>"));       //  <%IDS_SNMP_10%>
		else if(ret == -7)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SNMP_11%>"));    //  <%IDS_SNMP_11%>
		else if(ret == -8)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SNMP_12%>"));     //  <%IDS_SNMP_12%>
		else
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SNMP_13%>"));      //  <%IDS_SNMP_13%>

		if(oidp) free(oidp);
		return FALSE;
	}

	int i = 0;
	switch(snval.sv_type)
	{
	case ASN1_INT:
		switch(oidp->type)
		{
		case DFF1_IFTYPE:
			{
			char *iftype = NULL;
			iftype =	snval.sv_val.sv_int == 1 ? "other" : 
						snval.sv_val.sv_int == 2 ? "regular1822" :
						snval.sv_val.sv_int == 3 ? "hdh1822":
						snval.sv_val.sv_int == 4 ? "ddn-x25":
						snval.sv_val.sv_int == 5 ? "rfc877-x25":
						snval.sv_val.sv_int == 6 ? "ethernet-csmacd":
						snval.sv_val.sv_int == 7 ? "iso88023-csmacd":
						snval.sv_val.sv_int == 8 ? "iso88024-tokenBus":
						snval.sv_val.sv_int == 9 ? "iso88025-tokenRing":
						snval.sv_val.sv_int == 10 ? "iso88026-man":
						snval.sv_val.sv_int == 11? "starLan":
						snval.sv_val.sv_int == 12 ? "proteon-10Mbit":
						snval.sv_val.sv_int == 13 ? "proteon-80Mbit":
						snval.sv_val.sv_int == 14 ? "hyperchannel":
						snval.sv_val.sv_int == 15 ? "fddi":
						snval.sv_val.sv_int == 16 ? "lapb":
						snval.sv_val.sv_int == 17 ? "sdlc":
						snval.sv_val.sv_int == 18 ? "ds1":
						snval.sv_val.sv_int == 19 ? "e1":
						snval.sv_val.sv_int == 20 ? "basicISDN":
						snval.sv_val.sv_int == 21 ? "primaryISDN":
						snval.sv_val.sv_int == 22 ? "propPointToPointSerial":
						snval.sv_val.sv_int == 23 ? "ppp":
						snval.sv_val.sv_int == 24 ? "softwareLoopback":
						snval.sv_val.sv_int == 25 ? "eon":
						snval.sv_val.sv_int == 26 ? "ethernet-3Mbit":
						snval.sv_val.sv_int == 27 ? "nsip":
						snval.sv_val.sv_int == 28 ? "slip":
						snval.sv_val.sv_int == 29 ? "ultra":
						snval.sv_val.sv_int == 30 ? "ds3":
						snval.sv_val.sv_int == 31 ? "sip":
						snval.sv_val.sv_int == 32 ? "frame-relay": "unknown";

			//sprintf(szReturn, "%s=%s###%ld$", oidp->name, iftype, snval.sv_val.sv_int);
			sprintf(szReturn, "value=%ld$", snval.sv_val.sv_int);
			}
			break;
		case DFF1_ADMINSTATUS:
			{
			char *adminstatus = NULL;
			adminstatus =	snval.sv_val.sv_int == 1 ? "up" :
							snval.sv_val.sv_int == 2 ? "down" :
							snval.sv_val.sv_int == 3 ? "testing" : "unknown";

			//sprintf(szReturn, "%s=%s###%ld$", oidp->name, adminstatus, snval.sv_val.sv_int);
			sprintf(szReturn, "value=%ld$", snval.sv_val.sv_int);
			}
			break;
		case DFF1_OPERSTATUS:
			{
			char *operstatus = NULL;
			operstatus =	snval.sv_val.sv_int == 1 ? "up" :
							snval.sv_val.sv_int == 2 ? "down" :
							snval.sv_val.sv_int == 3 ? "testing" : "unknown";

			//sprintf(szReturn, "%s=%s###%ld$", oidp->name, operstatus, snval.sv_val.sv_int);
			sprintf(szReturn, "value=%ld$", snval.sv_val.sv_int);
			}
			break;
		case DFF1_IPFORWARDING:
			{
			char *forward = NULL;
			forward =	snval.sv_val.sv_int == 1 ? "forwarding" :
						snval.sv_val.sv_int == 2 ? "not-forwarding" : "unknown";

			//sprintf(szReturn, "%s=%s###%ld$", oidp->name, forward, snval.sv_val.sv_int);
			sprintf(szReturn, "value=%ld$", snval.sv_val.sv_int);
			}
			break;
		case DFF1_IPROUTETYPE:
			{
			char *routetype = NULL;
			routetype =	snval.sv_val.sv_int == 1 ? "other" : 
						snval.sv_val.sv_int == 2 ? "invalid" : 
						snval.sv_val.sv_int == 3 ? "direct" : 
						snval.sv_val.sv_int == 4 ? "indirect" : "unknown";

			//sprintf(szReturn, "%s=%s###%ld$", oidp->name, routetype, snval.sv_val.sv_int);
			sprintf(szReturn, "value=%ld$", snval.sv_val.sv_int);
			}
			break;
		case DFF1_IPROUTEPROTO:
			{
			char *routeproto = NULL;
			routeproto =	snval.sv_val.sv_int == 1 ? "other" : 
							snval.sv_val.sv_int == 2 ? "local" : 
							snval.sv_val.sv_int == 3 ? "netmgmt" : 
							snval.sv_val.sv_int == 4 ? "icmp" : 
							snval.sv_val.sv_int == 5 ? "egp" : 
							snval.sv_val.sv_int == 6 ? "ggp" : 
							snval.sv_val.sv_int == 7 ? "hello" : 
							snval.sv_val.sv_int == 8 ? "rip" : 
							snval.sv_val.sv_int == 9 ? "is-is" : 
							snval.sv_val.sv_int == 10 ? "es-is" : 
							snval.sv_val.sv_int == 11 ? "ciscoIgrp" : 
							snval.sv_val.sv_int == 12 ? "bbnSpfIgp" : 
							snval.sv_val.sv_int == 13 ? "ospf" : 
							snval.sv_val.sv_int == 14 ? "bgp" : "unknown";

			//sprintf(szReturn, "%s=%s###%ld$", oidp->name, routeproto, snval.sv_val.sv_int);
			sprintf(szReturn, "value=%ld$", snval.sv_val.sv_int);
			}
			break;
		case DFF1_NETTOMEDIATYPE:
			{
			char *nettomediatype = NULL;
			nettomediatype =	snval.sv_val.sv_int == 1 ? "other" : 
								snval.sv_val.sv_int == 2 ? "invalid" : 
								snval.sv_val.sv_int == 3 ? "dynamic" : 
								snval.sv_val.sv_int == 4 ? "static" : "unknown";

			//sprintf(szReturn, "%s=%s###%ld$", oidp->name, nettomediatype, snval.sv_val.sv_int);
			sprintf(szReturn, "value=%ld$", snval.sv_val.sv_int);
			}
			break;
		case DFF1_TCPRTOALGORITH:
			{
			char *rtoalgorithm = NULL;
			rtoalgorithm =	snval.sv_val.sv_int == 1 ? "other" :
							snval.sv_val.sv_int == 2 ? "constant" : 
							snval.sv_val.sv_int == 3 ? "rsre" : 
							snval.sv_val.sv_int == 4 ? "vanj" : "unknown";

			//sprintf(szReturn, "%s=%s###%ld$", oidp->name, rtoalgorithm, snval.sv_val.sv_int);
			sprintf(szReturn, "value=%ld$", snval.sv_val.sv_int);
			}
			break;
		case DFF1_TCPCONNSTATE:
			{
			char *connstate = NULL;
			connstate =	snval.sv_val.sv_int == 1 ? "closed" : 
						snval.sv_val.sv_int == 2 ? "listen" : 
						snval.sv_val.sv_int == 3 ? "synSent" : 
						snval.sv_val.sv_int == 4 ? "synReceived" : 
						snval.sv_val.sv_int == 5 ? "established" : 
						snval.sv_val.sv_int == 6 ? "finWait1" : 
						snval.sv_val.sv_int == 7 ? "finWait2" : 
						snval.sv_val.sv_int == 8 ? "closeWait" : 
						snval.sv_val.sv_int == 9 ? "lastAck" : 
						snval.sv_val.sv_int == 10 ? "closing" : 
						snval.sv_val.sv_int == 11 ? "timeWait" : 
						snval.sv_val.sv_int == 12 ? "deleteTCB" : "unknown";

			//sprintf(szReturn, "%s=%s###%ld$", oidp->name, connstate, snval.sv_val.sv_int);
			sprintf(szReturn, "value=%ld$", snval.sv_val.sv_int);
			}
			break;
		case DFF1_EGPNEIGHSTATE:
			{
			char *neighstate = NULL;
			neighstate =	snval.sv_val.sv_int == 1 ? "idle" : 
							snval.sv_val.sv_int == 2 ? "acquisition" : 
							snval.sv_val.sv_int == 3 ? "down" : 
							snval.sv_val.sv_int == 4 ? "up" : 
							snval.sv_val.sv_int == 5 ? "cease" : "unknown";

			//sprintf(szReturn, "%s=%s###%ld$", oidp->name, neighstate, snval.sv_val.sv_int);
			sprintf(szReturn, "value=%ld$", snval.sv_val.sv_int);
			}
			break;
		case DFF1_EGPNEIGHMODE:
			{
			char *neighmode = NULL;
			neighmode = snval.sv_val.sv_int == 1 ? "avtive" : 
						snval.sv_val.sv_int == 2 ? "passive" : "unknown";

			//sprintf(szReturn, "%s=%s###%ld$", oidp->name, neighmode, snval.sv_val.sv_int);
			sprintf(szReturn, "value=%ld$", snval.sv_val.sv_int);
			}
			break;
		case DFF1_EGPNEIGHEVENTT:
			{
			char *neigheventtrigger = NULL;
			neigheventtrigger =	snval.sv_val.sv_int == 1 ? "start" : 
								snval.sv_val.sv_int == 2 ? "stop" : "unknown";

			//sprintf(szReturn, "%s=%s###%ld$", oidp->name, neigheventtrigger, snval.sv_val.sv_int);
			sprintf(szReturn, "value=%ld$", snval.sv_val.sv_int);
			}
			break;
		case DFF1_SNMPENABLETRAP:
			{
			char *enableauthentraps = NULL;
			enableauthentraps =	snval.sv_val.sv_int == 1 ? "enabled" : 
								snval.sv_val.sv_int == 2 ? "disabled" : "unknown";

			//sprintf(szReturn, "%s=%s###%ld$", oidp->name, enableauthentraps, snval.sv_val.sv_int);
			sprintf(szReturn, "value=%ld$", snval.sv_val.sv_int);
			}
			break;
		default:
			//sprintf(szReturn, "%s=%ld$", oidp->name, snval.sv_val.sv_int);
			sprintf(szReturn, "value=%ld$", snval.sv_val.sv_int);
			break;
		}
		if(oidp) free(oidp);
		return TRUE;
	case ASN1_COUNTER:
	case ASN1_GAUGE:
		//sprintf(szReturn, "%s=%ld$", oidp->name, snval.sv_val.sv_int);
		sprintf(szReturn, "value=%u$", snval.sv_val.sv_int);
		if(oidp) free(oidp);
		return TRUE;
	case ASN1_TIMETICKS:
		{
		int days = 0, hours = 0, minutes = 0, secs = 0, psecs = 0;
		days = snval.sv_val.sv_int / (24 * 3600 * 100);
		hours = (snval.sv_val.sv_int % (24 * 3600 * 100)) / (3600 * 100);
		minutes = ((snval.sv_val.sv_int % (24 * 3600 * 100)) % (3600 * 100)) / (60 * 100);
		secs = (((snval.sv_val.sv_int % (24 * 3600 * 100)) % (3600 * 100)) % (60 * 100)) / 100;
		psecs = (((snval.sv_val.sv_int % (24 * 3600 * 100)) % (3600 * 100)) % (60 * 100)) % 100;

		//if(days)
		//	sprintf(szReturn, "%s=%d days, %d:%d:%d.%d###%ld$", oidp->name, days, hours, minutes, secs, psecs, snval.sv_val.sv_int);
		//else
		//	sprintf(szReturn, "%s=%d:%d:%d.%d###%ld$", oidp->name, hours, minutes, secs, psecs, snval.sv_val.sv_int);
		sprintf(szReturn, "value=%u$", snval.sv_val.sv_int);
		if(oidp) free(oidp);
		return TRUE;
		}
	case ASN1_OCTSTR:
		//if(!compare_oid(*oidp, interfaces_ifPhysAddress) || !compare_oid(*oidp, at_atPhysAddress))
		if(oidp->type == DFF1_PHYSADDR && snval.sv_val.sv_str.sv_len == 6)
			//sprintf(szReturn, "%s=%x-%x-%x-%x-%x-%x$", oidp->name, 
			//											*snval.sv_val.sv_str.sv_str & 0xFF, 
			//											*(snval.sv_val.sv_str.sv_str + 1) & 0xFF, 
			//											*(snval.sv_val.sv_str.sv_str + 2) & 0xFF, 
			//											*(snval.sv_val.sv_str.sv_str + 3) & 0xFF, 
			//											*(snval.sv_val.sv_str.sv_str + 4) & 0xFF, 
			//											*(snval.sv_val.sv_str.sv_str + 5) & 0xFF);
			sprintf(szReturn, "value=%x-%x-%x-%x-%x-%x$", 
														*snval.sv_val.sv_str.sv_str & 0xFF, 
														*(snval.sv_val.sv_str.sv_str + 1) & 0xFF, 
														*(snval.sv_val.sv_str.sv_str + 2) & 0xFF, 
														*(snval.sv_val.sv_str.sv_str + 3) & 0xFF, 
														*(snval.sv_val.sv_str.sv_str + 4) & 0xFF, 
														*(snval.sv_val.sv_str.sv_str + 5) & 0xFF);

		else
		{
			//sprintf(szReturn, "%s=", oidp->name);
			//strncat(szReturn, snval.sv_val.sv_str.sv_str, snval.sv_val.sv_str.sv_len);
			//strncat(szReturn, "$", 1);
			sprintf(szReturn, "value=", oidp->name);
			strncat(szReturn, snval.sv_val.sv_str.sv_str, snval.sv_val.sv_str.sv_len);
			strncat(szReturn, "$", 1);

		}
		if(oidp) free(oidp);
		return TRUE;
	case ASN1_NULL:
		//sprintf(szReturn, "%s=%s$", oidp->name, "");
		sprintf(szReturn, "value=%s$", "");
		if(oidp) free(oidp);
		return TRUE;
	case ASN1_OBJID:
		//sprintf(szReturn, "%s=", oidp->name);
		sprintf(szReturn, "%s=", "value");
		if(snval.sv_val.sv_oid.len <= 3)
			sprintf(szReturn, "%s$", szReturn);
		else
			while(i < snval.sv_val.sv_oid.len)
			{
				if(i == 0)
					sprintf(szReturn, "%s1.3.", szReturn);
				else if(i == snval.sv_val.sv_oid.len - 1)
					sprintf(szReturn, "%s%d$", szReturn, snval.sv_val.sv_oid.id[i]);
				else
					sprintf(szReturn, "%s%d.", szReturn, snval.sv_val.sv_oid.id[i]);
		
				i ++;
			}
		if(oidp) free(oidp);
		return TRUE;
	case ASN1_IPADDR:
		//sprintf(szReturn, "%s=%d.%d.%d.%d$",	oidp->name, 
		//										(u_char)*snval.sv_val.sv_ipaddr, 
		//										(u_char)*(snval.sv_val.sv_ipaddr + 1), 
		//										(u_char)*(snval.sv_val.sv_ipaddr + 2), 
		//										(u_char)*(snval.sv_val.sv_ipaddr + 3));
		sprintf(szReturn, "value=%d.%d.%d.%d$",	
												(u_char)*snval.sv_val.sv_ipaddr, 
												(u_char)*(snval.sv_val.sv_ipaddr + 1), 
												(u_char)*(snval.sv_val.sv_ipaddr + 2), 
												(u_char)*(snval.sv_val.sv_ipaddr + 3));
		if(oidp) free(oidp);
		return TRUE;
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SNMP_14%>"));    //  <%IDS_SNMP_14%>
		if(oidp) free(oidp);
		return FALSE;
	}

	return TRUE;
}

