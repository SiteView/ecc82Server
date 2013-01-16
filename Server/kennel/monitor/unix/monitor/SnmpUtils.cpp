
#include "SnmpUtils.h"
#include "Utils.h"

#ifdef WIN32
#pragma warning (disable : 4267)
#endif

struct oid system_oid = {{0x2b, 6, 1, 2, 1, 1, 0}, 7, "system", ASN1_OCTSTR, ""};

// RFC1213-MIB Variables
#define		RFC1213_VARS			174
struct oid rfc1213_oids[RFC1213_VARS] = {
	// *** System Group ***
	/* A textual description of the entity.  This value should include the full 
	   name and version identification of the system's hardware type, software 
	   operating-system, and networking software.  It is mandatory that this only 
	   contain printable ASCII characters. */
	{{0x2b, 6, 1, 2, 1, 1, 1}, 7, "system.sysDescr", ASN1_OCTSTR, ""}, 
	/* The vendor's authoritative identification of the network management 
	   subsystem contained in the entity.  This value is allocated within the SMI
	   enterprises subtree (1.3.6.1.4.1) and provides an easy and unambiguous 
	   means for determining `what kind of box' is being managed. For example, if
	   vendor `Flintstones, Inc.' was assigned the subtree 1.3.6.1.4.1.4242, it 
	   could assign the identifier 1.3.6.1.4.1.4242.1.1 to its `Fred Router'. */
	{{0x2b, 6, 1, 2, 1, 1, 2}, 7, "system.sysObjectID", ASN1_OBJID, ""}, 
	/* The time (in hundredths of a second) since the network management portion 
	   of the system was last re-initialized. */
	{{0x2b, 6, 1, 2, 1, 1, 3}, 7, "system.sysUpTime", ASN1_TIMETICKS, ""}, 
	/* The textual identification of the contact person for this managed node, 
	   together with information on how to contact this person. */
	{{0x2b, 6, 1, 2, 1, 1, 4}, 7, "system.sysContact", ASN1_OCTSTR, ""}, 
	/* n administratively-assigned name for this managed node.  By convention, 
	   this is the node's fully-qualified domain name. */
	{{0x2b, 6, 1, 2, 1, 1, 5}, 7, "system.sysName", ASN1_OCTSTR, ""}, 
	/* The physical location of this node (e.g., `telephone closet, 3rd floor'). */
	{{0x2b, 6, 1, 2, 1, 1, 6}, 7, "system.sysLocation", ASN1_OCTSTR, ""}, 
	/* A value which indicates the set of services that this entity primarily 
	   offers.

       The value is a sum.  This sum initially takes the value zero, Then, for 
	   each layer, L, in the range 1 through 7, that this node performs 
	   transactions for, 2 raised to (L - 1) is added to the sum.  For 
	   example, a node which performs primarily routing functions would have a 
	   value of 4 (2^(3-1)).  In contrast, a node which is a host offering 
	   application services would have a value of 72 (2^(4-1) + 2^(7-1)).  Note 
	   that in the context of the Internet suite of protocols, values should be 
	   calculated accordingly:

       layer  functionality
           1  physical (e.g., repeaters)
           2  datalink/subnetwork (e.g., bridges)
           3  internet (e.g., IP gateways)
           4  end-to-end  (e.g., IP hosts)
           7  applications (e.g., mail relays)

        For systems including OSI protocols, layers 5 and 6 may also be counted. */
	{{0x2b, 6, 1, 2, 1, 1, 7}, 7, "system.sysServices", ASN1_INT, ""}, 

	// *** Interfaces Group ***
	/* The number of network interfaces (regardless of their current state) present 
	   on this system. */
	{{0x2b, 6, 1, 2, 1, 2, 1}, 7, "interfaces.ifNumber", ASN1_INT, ""}, 
	/* A unique value for each interface.  Its value ranges between 1 and the value 
	   of ifNumber.  The value for each interface must remain constant at least 
	   from one re-initialization of the entity's network management system to the 
	   next reinitialization. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 1}, 9, "interfaces.ifTable.ifEntry.ifIndex", ASN1_INT, ""}, 
	/* A textual string containing information about the interface.  This string 
	   should include the name of the manufacturer, the product name and the version
	   of the hardware interface. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 2}, 9, "interfaces.ifTable.ifEntry.ifDescr", ASN1_OCTSTR, ""}, 
	/* The type of interface, distinguished according to the physical/link 
	   protocol(s) immediately `below' the network layer in the protocol stack. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 3}, 9, "interfaces.ifTable.ifEntry.ifType", DFF1_IFTYPE, ""}, 
	/* The size of the largest datagram which can be sent/received on the 
	   interface, specified in octets.  For interfaces that are used for 
	   transmitting network datagrams, this is the size of the largest network 
	   datagram that can be sent on the interface. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 4}, 9, "interfaces.ifTable.ifEntry.ifMtu", ASN1_INT, ""}, 
	/* An estimate of the interface's current bandwidth in bits per second.  For 
	   interfaces which do not vary in bandwidth or for those where no accurate
	   estimation can be made, this object should contain the nominal bandwidth. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 5}, 9, "interfaces.ifTable.ifEntry.ifSpeed", ASN1_GAUGE, ""}, 
	/* The interface's address at the protocol layer immediately `below' the 
	   network layer in the protocol stack.  For interfaces which do not have
	   such an address (e.g., a serial line), this object should contain an octet 
	   string of zero length. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 6}, 9, "interfaces.ifTable.ifEntry.ifPhysAddress", DFF1_PHYSADDR, ""}, 
	/* The desired state of the interface.  The testing(3) state indicates that 
	   no operational packets can be passed. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 7}, 9, "interfaces.ifTable.ifEntry.ifAdminStatus", DFF1_ADMINSTATUS, ""}, 
	/* The current operational state of the interface. The testing(3) state 
	   indicates that no operational packets can be passed. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 8}, 9, "interfaces.ifTable.ifEntry.ifOperStatus", DFF1_OPERSTATUS, ""}, 
	/* The value of sysUpTime at the time the interface entered its current 
	   operational state.  If the current state was entered prior to the last 
	   reinitialization of the local network management subsystem, then this object 
	   contains a zero value. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 9}, 9, "interfaces.ifTable.ifEntry.ifLastChange", ASN1_TIMETICKS, ""}, 
	/* The total number of octets received on the interface, including framing 
	   characters. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 10}, 9, "interfaces.ifTable.ifEntry.ifInOctets", ASN1_COUNTER, ""}, 
	/* The number of subnetwork-unicast packets delivered to a higher-layer 
	   protocol. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 11}, 9, "interfaces.ifTable.ifEntry.ifInUcastPkts", ASN1_COUNTER, ""}, 
	/* The number of non-unicast (i.e., subnetwork-broadcast or subnetwork-
	   multicast) packets delivered to a higher-layer protocol. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 12}, 9, "interfaces.ifTable.ifEntry.ifInNUcastPkts", ASN1_COUNTER, ""}, 
	/* The number of inbound packets which were chosen to be discarded even though 
	   no errors had been detected to prevent their being deliverable to a higher-
	   layer protocol.  One possible reason for discarding such a packet could be to 
	   free up buffer space. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 13}, 9, "interfaces.ifTable.ifEntry.ifInDiscards", ASN1_COUNTER, ""}, 
	/* The number of inbound packets that contained errors preventing them from 
	   being deliverable to a higher-layer protocol. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 14}, 9, "interfaces.ifTable.ifEntry.ifInErrors", ASN1_COUNTER, ""}, 
	/* The number of packets received via the interface which were discarded because 
	   of an unknown or unsupported protocol. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 15}, 9, "interfaces.ifTable.ifEntry.ifInUnknownProtos", ASN1_COUNTER, ""}, 
	/* The total number of octets transmitted out of the interface, including 
	   framing characters. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 16}, 9, "interfaces.ifTable.ifEntry.ifOutOctets", ASN1_COUNTER, ""}, 
	/* The total number of packets that higher-level protocols requested be 
	   transmitted to a subnetwork-unicast address, including those that were 
	   discarded or not sent. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 17}, 9, "interfaces.ifTable.ifEntry.ifOutUcastPkts", ASN1_COUNTER, ""}, 
	/* The total number of packets that higher-level protocols requested be 
	   transmitted to a non-unicast (i.e., a subnetwork-broadcast or subnetwork-
	   multicast) address, including those that were discarded or not sent. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 18}, 9, "interfaces.ifTable.ifEntry.ifOutNUcastPkts", ASN1_COUNTER, ""}, 
	/* The number of outbound packets which were chosen to be discarded even though 
	   no errors had been detected to prevent their being transmitted.  One possible 
	   reason for discarding such a packet could be to free up buffer space. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 19}, 9, "interfaces.ifTable.ifEntry.ifOutDiscards", ASN1_COUNTER, ""}, 
	/* The number of outbound packets that could not be transmitted because of 
	   errors. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 20}, 9, "interfaces.ifTable.ifEntry.ifOutErrors", ASN1_COUNTER, ""}, 
	/* The length of the output packet queue (in packets). */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 21}, 9, "interfaces.ifTable.ifEntry.ifOutQLen", ASN1_GAUGE, ""}, 
	/* A reference to MIB definitions specific to the particular media being used 
	   to realize the interface.  For example, if the interface is realized by an 
	   ethernet, then the value of this object refers to a document defining objects 
	   specific to ethernet.  If this information is not present, its value should 
	   be set to the OBJECT IDENTIFIER { 0 0 }, which is a syntatically valid object 
	   identifier, and any conformant implementation of ASN.1 and BER must be able 
	   to generate and recognize this value. */
	{{0x2b, 6, 1, 2, 1, 2, 2, 1, 22}, 9, "interfaces.ifTable.ifEntry.ifSpecific", ASN1_OBJID, ""}, 

	// *** At Group ***
	/* The interface on which this entry's equivalence is effective.  The interface 
	   identified by a particular value of this index is the same interface as 
	   identified by the same value of ifIndex. */
	{{0x2b, 6, 1, 2, 1, 3, 1, 1, 1}, 9, "at.atTable.atEntry.atIfIndex", ASN1_INT, ""}, 
	/* The media-dependent `physical' address.

       Setting this object to a null string (one of zero length) has the effect of 
	   invaliding the corresponding entry in the atTable object.  That is, it 
	   effectively dissasociates the interface identified with said entry from the 
	   mapping identified with said entry.  It is an implementation-specific matter 
	   as to whether the agent removes an invalidated entry from the table. 
	   Accordingly, management stations must be prepared to receive tabular 
	   information from agents that corresponds to entries not currently in use.
       Proper interpretation of such entries requires examination of the relevant 
	   atPhysAddress object. */
	{{0x2b, 6, 1, 2, 1, 3, 1, 1, 2}, 9, "at.atTable.atEntry.atPhysAddress", DFF1_PHYSADDR, ""}, 
	/* The NetworkAddress (e.g., the IP address) corresponding to the media-
	   dependent `physical' address. */
	{{0x2b, 6, 1, 2, 1, 3, 1, 1, 3}, 9, "at.atTable.atEntry.atNetAddress", ASN1_IPADDR, ""}, 

	// *** Ip Group ***
	/* The indication of whether this entity is acting as an IP gateway in respect 
	   to the forwarding of datagrams received by, but not addressed to, this 
	   entity.  IP gateways forward datagrams.  IP hosts do not (except those 
	   source-routed via the host).

       Note that for some managed nodes, this object may take on only a subset of 
	   the values possible. Accordingly, it is appropriate for an agent to return 
	   a `badValue' response if a management station attempts to change this object 
	   to an inappropriate value. */
	{{0x2b, 6, 1, 2, 1, 4, 1}, 7, "ip.ipForwarding", DFF1_IPFORWARDING, ""}, 
	/* The default value inserted into the Time-To-Live field of the IP header of 
	   datagrams originated at this entity, whenever a TTL value is not supplied
       by the transport layer protocol. */
	{{0x2b, 6, 1, 2, 1, 4, 2}, 7, "ip.ipDefaultTTL", ASN1_INT, ""}, 
	/* The total number of input datagrams received from interfaces, including 
	   those received in error. */
	{{0x2b, 6, 1, 2, 1, 4, 3}, 7, "ip.ipInReceives", ASN1_COUNTER, ""}, 
	/* The number of input datagrams discarded due to errors in their IP headers, 
	   including bad checksums, version number mismatch, other format errors, 
	   time-to-live exceeded, errors discovered in processing their IP options, 
	   etc. */
	{{0x2b, 6, 1, 2, 1, 4, 4}, 7, "ip.ipInHdrErrors", ASN1_COUNTER, ""}, 
	/* The number of input datagrams discarded because the IP address in their IP 
	   header's destination field was not a valid address to be received at this 
	   entity.  This count includes invalid addresses (e.g., 0.0.0.0) and addresses 
	   of unsupported Classes (e.g., Class E).  For entities which are not IP 
	   Gateways and therefore do not forward datagrams, this counter includes 
	   datagrams discarded because the destination address was not a local 
	   address. */
	{{0x2b, 6, 1, 2, 1, 4, 5}, 7, "ip.ipInAddrErrors", ASN1_COUNTER, ""}, 
	/* The number of input datagrams for which this entity was not their final IP 
	   destination, as a result of which an attempt was made to find a route to 
	   forward them to that final destination. In entities which do not act as IP 
	   Gateways, this counter will include only those packets which were Source-
	   Routed via this entity, and the Source-Route option processing was 
	   successful. */
	{{0x2b, 6, 1, 2, 1, 4, 6}, 7, "ip.ipForwDataGrams", ASN1_COUNTER, ""}, 
	/* The number of locally-addressed datagrams received successfully but discarded 
	   because of an unknown or unsupported protocol. */
	{{0x2b, 6, 1, 2, 1, 4, 7}, 7, "ip.ipInUnknowProtos", ASN1_COUNTER, ""}, 
	/* The number of input IP datagrams for which no problems were encountered to 
	   prevent their continued processing, but which were discarded (e.g., for lack 
	   of buffer space).  Note that this counter does not include any datagrams 
	   discarded while awaiting re-assembly. */
	{{0x2b, 6, 1, 2, 1, 4, 8}, 7, "ip.ipInDiscards", ASN1_COUNTER, ""}, 
	/* The total number of input datagrams successfully delivered to IP user-
	   protocols (including ICMP). */
	{{0x2b, 6, 1, 2, 1, 4, 9}, 7, "ip.ipInDelivers", ASN1_COUNTER, ""}, 
	/* The total number of IP datagrams which local IP user-protocols (including 
	   ICMP) supplied to IP in requests for transmission.  Note that this counter 
       does not include any datagrams counted in ipForwDatagrams. */
	{{0x2b, 6, 1, 2, 1, 4, 10}, 7, "ip.ipOutRequests", ASN1_COUNTER, ""}, 
	/* The number of output IP datagrams for which no problem was encountered to 
	   prevent their transmission to their destination, but which were discarded 
	   (e.g., for lack of buffer space).  Note that this counter would include 
	   datagrams counted in ipForwDatagrams if any such packets met this 
	   (discretionary) discard criterion. */
	{{0x2b, 6, 1, 2, 1, 4, 11}, 7, "ip.ipOutDiscards", ASN1_COUNTER, ""}, 
	/* The number of IP datagrams discarded because no route could be found to 
	   transmit them to their destination.  Note that this counter includes any 
	   packets counted in ipForwDatagrams which meet this `no-route' criterion. 
	   Note that this includes any datagarms which a host cannot route because 
	   all of its default gateways are down. */
	{{0x2b, 6, 1, 2, 1, 4, 12}, 7, "ip.ipOutNoRoutes", ASN1_COUNTER, ""}, 
	/* The maximum number of seconds which received fragments are held while they 
	   are awaiting reassembly at this entity. */
	{{0x2b, 6, 1, 2, 1, 4, 13}, 7, "ip.ipReasmTimeout", ASN1_INT, ""}, 
	/* The number of IP fragments received which needed to be reassembled at this 
	   entity. */
	{{0x2b, 6, 1, 2, 1, 4, 14}, 7, "ip.ipReasmReqds", ASN1_COUNTER, ""}, 
	/* The number of IP datagrams successfully reassembled. */
	{{0x2b, 6, 1, 2, 1, 4, 15}, 7, "ip.ipReasmOKs", ASN1_COUNTER, ""}, 
	/* The number of failures detected by the IP reassembly algorithm (for whatever 
	   reason: timed out, errors, etc).  Note that this is not necessarily a count 
	   of discarded IP fragments since some algorithms (notably the algorithm in 
	   RFC 815) can lose track of the number of fragments by combining them as they 
	   are received. */
	{{0x2b, 6, 1, 2, 1, 4, 16}, 7, "ip.ipReasmFails", ASN1_COUNTER, ""}, 
	/* The number of IP datagrams that have been successfully fragmented at this 
	   entity. */
	{{0x2b, 6, 1, 2, 1, 4, 17}, 7, "ip.ipFragOKs", ASN1_COUNTER, ""}, 
	/* The number of IP datagrams that have been discarded because they needed to 
	   be fragmented at this entity but could not be, e.g., because their Don't 
	   Fragment flag was set. */
	{{0x2b, 6, 1, 2, 1, 4, 18}, 7, "ip.ipFragFails", ASN1_COUNTER, ""}, 
	/* The number of IP datagram fragments that have been generated as a result of 
	   fragmentation at this entity. */
	{{0x2b, 6, 1, 2, 1, 4, 19}, 7, "ip.ipFragCreates", ASN1_COUNTER, ""}, 

	/* The IP address to which this entry's addressing information pertains. */
	{{0x2b, 6, 1, 2, 1, 4, 20, 1, 1}, 9, "ip.ipAddrTable.ipAddrEntry.ipAdEntAddr", ASN1_IPADDR, ""}, 
	/* The index value which uniquely identifies the interface to which this entry 
	   is applicable.  The interface identified by a particular value of this index 
	   is the same interface as identified by the same value of ifIndex. */
	{{0x2b, 6, 1, 2, 1, 4, 20, 1, 2}, 9, "ip.ipAddrTable.ipAddrEntry.ipAdEntIfIndex", ASN1_INT, ""}, 
	/* The subnet mask associated with the IP address of this entry.  The value of 
	   the mask is an IP address with all the network bits set to 1 and all the 
	   hosts bits set to 0. */
	{{0x2b, 6, 1, 2, 1, 4, 20, 1, 3}, 9, "ip.ipAddrTable.ipAddrEntry.ipAdEntNetMask", ASN1_IPADDR, ""}, 
	/* The value of the least-significant bit in the IP broadcast address used for 
	   sending datagrams on the (logical) interface associated with the IP address 
	   of this entry.  For example, when the Internet standard all-ones broadcast 
	   address is used, the value will be 1.  This value applies to both the subnet 
	   and network broadcasts addresses used by the entity on this (logical) 
	   interface. */
	{{0x2b, 6, 1, 2, 1, 4, 20, 1, 4}, 9, "ip.ipAddrTable.ipAddrEntry.ipAdEntBcastAddr", ASN1_INT, ""}, 
	/* The size of the largest IP datagram which this entity can re-assemble from 
	   incoming IP fragmented datagrams received on this interface. */
	{{0x2b, 6, 1, 2, 1, 4, 20, 1, 5}, 9, "ip.ipAddrTable.ipAddrEntry.ipAdEntReasmMaxSize", ASN1_INT, ""}, 

	/* The destination IP address of this route.  An entry with a value of 0.0.0.0 
	   is considered a default route.  Multiple routes to a single destination can 
	   appear in the table, but access to such multiple entries is dependent on the 
	   table-access mechanisms defined by the network management protocol in use. */
	{{0x2b, 6, 1, 2, 1, 4, 21, 1, 1}, 9, "ip.ipRouteTable.ipRouteEntry.ipRouteDest", ASN1_IPADDR, ""}, 
	/* The index value which uniquely identifies the local interface through which 
	   the next hop of this route should be reached.  The interface identified by 
	   a particular value of this index is the same interface as identified by the 
	   same value of ifIndex. */
	{{0x2b, 6, 1, 2, 1, 4, 21, 1, 2}, 9, "ip.ipRouteTable.ipRouteEntry.ipRouteIfIndex", ASN1_INT, ""}, 
	/* The primary routing metric for this route.  The semantics of this metric are 
	   determined by the routing-protocol specified in the route's ipRouteProto 
	   value.  If this metric is not used, its value should be set to -1. */
	{{0x2b, 6, 1, 2, 1, 4, 21, 1, 3}, 9, "ip.ipRouteTable.ipRouteEntry.ipRouteMetric1", ASN1_INT, ""}, 
	/* An alternate routing metric for this route.  The semantics of this metric are 
	   determined by the routing-protocol specified in the route's ipRouteProto 
	   value.  If this metric is not used, its value should be set to -1. */
	{{0x2b, 6, 1, 2, 1, 4, 21, 1, 4}, 9, "ip.ipRouteTable.ipRouteEntry.ipRouteMetric2", ASN1_INT, ""}, 
	/* An alternate routing metric for this route.  The semantics of this metric are 
	   determined by the routing-protocol specified in the route's ipRouteProto 
	   value.  If this metric is not used, its value should be set to -1. */
	{{0x2b, 6, 1, 2, 1, 4, 21, 1, 5}, 9, "ip.ipRouteTable.ipRouteEntry.ipRouteMetric3", ASN1_INT, ""}, 
	/* An alternate routing metric for this route.  The semantics of this metric are 
	   determined by the routing-protocol specified in the route's ipRouteProto 
	   value.  If this metric is not used, its value should be set to -1. */
	{{0x2b, 6, 1, 2, 1, 4, 21, 1, 6}, 9, "ip.ipRouteTable.ipRouteEntry.ipRouteMetric4", ASN1_INT, ""}, 
	/* The IP address of the next hop of this route. (In the case of a route bound 
	   to an interface which is realized via a broadcast media, the value of this 
	   field is the agent's IP address on that interface.) */
	{{0x2b, 6, 1, 2, 1, 4, 21, 1, 7}, 9, "ip.ipRouteTable.ipRouteEntry.ipRouteNextHop", ASN1_IPADDR, ""}, 
	/* The type of route.  Note that the values direct(3) and indirect(4) refer to 
	   the notion of direct and indirect routing in the IP architecture.

       Setting this object to the value invalid(2) has the effect of invalidating 
	   the corresponding entry in the ipRouteTable object.  That is, it effectively 
	   dissasociates the destination identified with said entry from the route 
	   identified with said entry.  It is an implementation-specific matter as to 
	   whether the agent removes an invalidated entry from the table. Accordingly, 
	   management stations must be prepared to receive tabular information from 
	   agents that corresponds to entries not currently in use. Proper interpretation 
	   of such entries requires examination of the relevant ipRouteType object. */
	{{0x2b, 6, 1, 2, 1, 4, 21, 1, 8}, 9, "ip.ipRouteTable.ipRouteEntry.ipRouteType", DFF1_IPROUTETYPE, ""}, 
	/* The routing mechanism via which this route was learned.  Inclusion of values 
	   for gateway routing protocols is not intended to imply that hosts should 
	   support those protocols. */
	{{0x2b, 6, 1, 2, 1, 4, 21, 1, 9}, 9, "ip.ipRouteTable.ipRouteEntry.ipRouteProto", DFF1_IPROUTEPROTO, ""}, 
	/* The number of seconds since this route was last updated or otherwise 
	   determined to be correct. Note that no semantics of `too old' can be implied 
       except through knowledge of the routing protocol by which the route was 
	   learned. */
	{{0x2b, 6, 1, 2, 1, 4, 21, 1, 10}, 9, "ip.ipRouteTable.ipRouteEntry.ipRouteAge", ASN1_INT, ""}, 
	/* Indicate the mask to be logical-ANDed with the destination address before 
	   being compared to the value in the ipRouteDest field.  For those systems 
	   that do not support arbitrary subnet masks, an agent constructs the value 
	   of the ipRouteMask by determining whether the value of the correspondent 
	   ipRouteDest field belong to a class-A, B, or C network, and then using one 
	   of:

           mask           network
           255.0.0.0      class-A
           255.255.0.0    class-B
           255.255.255.0  class-C

       If the value of the ipRouteDest is 0.0.0.0 (a default route), then the mask 
	   value is also 0.0.0.0.  It should be noted that all IP routing subsystems 
	   implicitly use this mechanism. */
	{{0x2b, 6, 1, 2, 1, 4, 21, 1, 11}, 9, "ip.ipRouteTable.ipRouteEntry.ipRouteMask", ASN1_IPADDR, ""}, 
	/* An alternate routing metric for this route.  The semantics of this metric are 
	   determined by the routing-protocol specified in the route's ipRouteProto 
	   value.  If this metric is not used, its value should be set to -1. */
	{{0x2b, 6, 1, 2, 1, 4, 21, 1, 12}, 9, "ip.ipRouteTable.ipRouteEntry.ipRouteMetric5", ASN1_INT, ""}, 
	/* A reference to MIB definitions specific to the particular routing protocol 
	   which is responsible for this route, as determined by the value specified in 
	   the route's ipRouteProto value.  If this information is not present, its 
	   value should be set to the OBJECT IDENTIFIER { 0 0 }, which is a syntatically 
	   valid object identifier, and any conformant implementation of ASN.1 and BER 
	   must be able to generate and recognize this value. */
	{{0x2b, 6, 1, 2, 1, 4, 21, 1, 13}, 9, "ip.ipRouteTable.ipRouteEntry.ipRouteInfo", ASN1_OBJID, ""}, 

	/* The interface on which this entry's equivalence is effective.  The interface 
	   identified by a particular value of this index is the same interface as 
	   identified by the same value of ifIndex. */
	{{0x2b, 6, 1, 2, 1, 4, 22, 1, 1}, 9, "ip.ipNetToMediaTable.ipNetToMediaEntry.ipNetToMediaIfIndex", ASN1_INT, ""}, 
	/* The media-dependent `physical' address. */
	{{0x2b, 6, 1, 2, 1, 4, 22, 1, 2}, 9, "ip.ipNetToMediaTable.ipNetToMediaEntry.ipNetToMediaPhysAddress", DFF1_PHYSADDR, ""}, 
	/* The IpAddress corresponding to the media-dependent `physical' address. */
	{{0x2b, 6, 1, 2, 1, 4, 22, 1, 3}, 9, "ip.ipNetToMediaTable.ipNetToMediaEntry.ipNetToMediaNetAddress", ASN1_IPADDR, ""}, 
	/* The type of mapping.

       Setting this object to the value invalid(2) has the effect of invalidating 
	   the corresponding entry in the ipNetToMediaTable.  That is, it effectively 
	   dissasociates the interface identified with said entry from the mapping 
	   identified with said entry. It is an implementation-specific matter as to 
	   whether the agent removes an invalidated entry from the table.  Accordingly, 
	   management stations must be prepared to receive tabular information from 
	   agents that corresponds to entries not currently in use. Proper interpretation 
	   of such entries requires examination of the relevant ipNetToMediaType 
	   object. */
	{{0x2b, 6, 1, 2, 1, 4, 22, 1, 4}, 9, "ip.ipNetToMediaTable.ipNetToMediaEntry.ipNetToMediaType", DFF1_NETTOMEDIATYPE, ""}, 

	/* The number of routing entries which were chosen to be discarded even though 
	   they are valid.  One possible reason for discarding such an entry could be 
	   to free-up buffer space for other routing entries. */
	{{0x2b, 6, 1, 2, 1, 4, 23}, 7, "ip.ipRoutingDiscards", ASN1_COUNTER, ""}, 

	// *** Icmp Group ***
	/* The total number of ICMP messages which the entity received.  Note that this 
	   counter includes all those counted by icmpInErrors. */
	{{0x2b, 6, 1, 2, 1, 5, 1}, 7, "icmp.icmpInMsgs", ASN1_COUNTER, ""}, 
	/* The number of ICMP messages which the entity received but determined as 
	   having ICMP-specific errors (bad ICMP checksums, bad length, etc.). */
	{{0x2b, 6, 1, 2, 1, 5, 2}, 7, "icmp.icmpInErrors", ASN1_COUNTER, ""}, 
	/* The number of ICMP Destination Unreachable messages received. */
	{{0x2b, 6, 1, 2, 1, 5, 3}, 7, "icmp.icmpInDestUnreachs", ASN1_COUNTER, ""}, 
	/* The number of ICMP Time Exceeded messages received. */
	{{0x2b, 6, 1, 2, 1, 5, 4}, 7, "icmp.icmpInTimeExcds", ASN1_COUNTER, ""}, 
	/* The number of ICMP Parameter Problem messages received. */
	{{0x2b, 6, 1, 2, 1, 5, 5}, 7, "icmp.icmpInParmProbs", ASN1_COUNTER, ""}, 
	/* The number of ICMP Source Quench messages received. */
	{{0x2b, 6, 1, 2, 1, 5, 6}, 7, "icmp.icmpInSrcQuenchs", ASN1_COUNTER, ""}, 
	/* The number of ICMP Redirect messages received. */
	{{0x2b, 6, 1, 2, 1, 5, 7}, 7, "icmp.icmpInRedirects", ASN1_COUNTER, ""}, 
	/* The number of ICMP Echo (request) messages received. */
	{{0x2b, 6, 1, 2, 1, 5, 8}, 7, "icmp.icmpInEchos", ASN1_COUNTER, ""}, 
	/* The number of ICMP Echo Reply messages received. */
	{{0x2b, 6, 1, 2, 1, 5, 9}, 7, "icmp.icmpInEchoReps", ASN1_COUNTER, ""}, 
	/* The number of ICMP Timestamp (request) messages received. */
	{{0x2b, 6, 1, 2, 1, 5, 10}, 7, "icmp.icmpInTimeStamps", ASN1_COUNTER, ""}, 
	/* The number of ICMP Timestamp Reply messages received. */
	{{0x2b, 6, 1, 2, 1, 5, 11}, 7, "icmp.icmpInTimeStampReps", ASN1_COUNTER, ""}, 
	/* The number of ICMP Address Mask Request messages received. */
	{{0x2b, 6, 1, 2, 1, 5, 12}, 7, "icmp.icmpInAddrMasks", ASN1_COUNTER, ""}, 
	/* The number of ICMP Address Mask Reply messages received. */
	{{0x2b, 6, 1, 2, 1, 5, 13}, 7, "icmp.icmpInAddrMaskReps", ASN1_COUNTER, ""}, 
	/* The total number of ICMP messages which this entity attempted to send.  
	   Note that this counter includes all those counted by icmpOutErrors. */
	{{0x2b, 6, 1, 2, 1, 5, 14}, 7, "icmp.icmpOutMsgs", ASN1_COUNTER, ""}, 
	/* The number of ICMP messages which this entity did not send due to problems 
	   discovered within ICMP such as a lack of buffers.  This value should not 
	   include errors discovered outside the ICMP layer such as the inability of 
	   IP to route the resultant datagram.  In some implementations there may be 
	   no types of error which contribute to this counter's value. */
	{{0x2b, 6, 1, 2, 1, 5, 15}, 7, "icmp.icmpOutErrors", ASN1_COUNTER, ""}, 
	/* The number of ICMP Destination Unreachable messages sent. */
	{{0x2b, 6, 1, 2, 1, 5, 16}, 7, "icmp.icmpOutDestUnreachs", ASN1_COUNTER, ""}, 
	/* The number of ICMP Time Exceeded messages sent. */
	{{0x2b, 6, 1, 2, 1, 5, 17}, 7, "icmp.icmpOutTimeExcds", ASN1_COUNTER, ""}, 
	/* The number of ICMP Parameter Problem messages sent. */
	{{0x2b, 6, 1, 2, 1, 5, 18}, 7, "icmp.icmpOutParmProbs", ASN1_COUNTER, ""}, 
	/* The number of ICMP Source Quench messages sent. */
	{{0x2b, 6, 1, 2, 1, 5, 19}, 7, "icmp.icmpOutSrcQuenchs", ASN1_COUNTER, ""}, 
	/* The number of ICMP Redirect messages sent.  For a host, this object will 
	   always be zero, since hosts do not send redirects. */
	{{0x2b, 6, 1, 2, 1, 5, 20}, 7, "icmp.icmpOutRedirects", ASN1_COUNTER, ""}, 
	/* The number of ICMP Echo (request) messages sent. */
	{{0x2b, 6, 1, 2, 1, 5, 21}, 7, "icmp.icmpOutEchos", ASN1_COUNTER, ""}, 
	/* The number of ICMP Echo Reply messages sent. */
	{{0x2b, 6, 1, 2, 1, 5, 22}, 7, "icmp.icmpOutEchoReps", ASN1_COUNTER, ""}, 
	/* The number of ICMP Timestamp (request) messages sent. */
	{{0x2b, 6, 1, 2, 1, 5, 23}, 7, "icmp.icmpOutTimeStamps", ASN1_COUNTER, ""}, 
	/* The number of ICMP Timestamp Reply messages sent. */
	{{0x2b, 6, 1, 2, 1, 5, 24}, 7, "icmp.icmpOutTimeStampReps", ASN1_COUNTER, ""}, 
	/* The number of ICMP Address Mask Request messages sent. */
	{{0x2b, 6, 1, 2, 1, 5, 25}, 7, "icmp.icmpOutAddrMasks", ASN1_COUNTER, ""}, 
	/* The number of ICMP Address Mask Reply messages sent. */
	{{0x2b, 6, 1, 2, 1, 5, 26}, 7, "icmp.icmpOutAddrMaskReps", ASN1_COUNTER, ""}, 

	// *** Tcp Group ***
	/* The algorithm used to determine the timeout value used for retransmitting 
	   unacknowledged octets. */
	{{0x2b, 6, 1, 2, 1, 6, 1}, 7, "tcp.tcpRtoAlgorithm", DFF1_TCPRTOALGORITH, ""}, 
	/* The minimum value permitted by a TCP implementation for the retransmission 
	   timeout, measured in milliseconds.  More refined semantics for objects of 
	   this type depend upon the algorithm used to determine the retransmission 
	   timeout. In particular, when the timeout algorithm is rsre(3), an object of 
	   this type has the semantics of the LBOUND quantity described in RFC 793. */
	{{0x2b, 6, 1, 2, 1, 6, 2}, 7, "tcp.tcpRtoMin", ASN1_INT, ""}, 
	/* The maximum value permitted by a TCP implementation for the retransmission 
	   timeout, measured in milliseconds.  More refined semantics for objects of 
	   this type depend upon the algorithm used to determine the retransmission 
	   timeout. In particular, when the timeout algorithm is rsre(3), an object of 
	   this type has the semantics of the UBOUND quantity described in RFC 793. */
	{{0x2b, 6, 1, 2, 1, 6, 3}, 7, "tcp.tcpRtoMax", ASN1_INT, ""}, 
	/* The limit on the total number of TCP connections the entity can support.  
	   In entities where the maximum number of connections is dynamic, this object 
	   should contain the value -1. */
	{{0x2b, 6, 1, 2, 1, 6, 4}, 7, "tcp.tcpMaxConn", ASN1_INT, ""}, 
	/* The number of times TCP connections have made a direct transition to the 
	   SYN-SENT state from the CLOSED state. */
	{{0x2b, 6, 1, 2, 1, 6, 5}, 7, "tcp.tcpActiveOpens", ASN1_COUNTER, ""}, 
	/* The number of times TCP connections have made a direct transition to the 
	   SYN-RCVD state from the LISTEN state. */
	{{0x2b, 6, 1, 2, 1, 6, 6}, 7, "tcp.tcpPassiveOpens", ASN1_COUNTER, ""}, 
	/* The number of times TCP connections have made a direct transition to the 
	   CLOSED state from either the SYN-SENT state or the SYN-RCVD state, plus the 
	   number of times TCP connections have made a direct transition to the LISTEN 
	   state from the SYN-RCVD state. */
	{{0x2b, 6, 1, 2, 1, 6, 7}, 7, "tcp.tcpAttemptFails", ASN1_COUNTER, ""}, 
	/* The number of times TCP connections have made a direct transition to the 
	   CLOSED state from either the ESTABLISHED state or the CLOSE-WAIT state. */
	{{0x2b, 6, 1, 2, 1, 6, 8}, 7, "tcp.tcpEstabResets", ASN1_COUNTER, ""}, 
	/* The number of TCP connections for which the current state is either 
	   ESTABLISHED or CLOSE-WAIT. */
	{{0x2b, 6, 1, 2, 1, 6, 9}, 7, "tcp.tcpCurrEstab", ASN1_GAUGE, ""}, 
	/* The total number of segments received, including those received in error. 
	   This count includes segments received on currently established connections. */
	{{0x2b, 6, 1, 2, 1, 6, 10}, 7, "tcp.tcpInSegs", ASN1_COUNTER, ""}, 
	/* The total number of segments sent, including those on current connections 
	   but excluding those containing only retransmitted octets. */
	{{0x2b, 6, 1, 2, 1, 6, 11}, 7, "tcp.tcpOutSegs", ASN1_COUNTER, ""}, 
	/* The total number of segments retransmitted - that is, the number of TCP 
	   segments transmitted containing one or more previously transmitted octets. */
	{{0x2b, 6, 1, 2, 1, 6, 12}, 7, "tcp.tcpRetransSegs", ASN1_COUNTER, ""}, 

	/* The state of this TCP connection.

       The only value which may be set by a management station is deleteTCB(12). 
	   Accordingly, it is appropriate for an agent to return a `badValue' response 
	   if a management station attempts to set this object to any other value.

       If a management station sets this object to the value deleteTCB(12), then 
	   this has the effect of deleting the TCB (as defined in RFC 793) of the 
       corresponding connection on the managed node, resulting in immediate 
	   termination of the connection.

       As an implementation-specific option, a RST segment may be sent from the 
	   managed node to the other TCP endpoint (note however that RST segments
       are not sent reliably). */
	{{0x2b, 6, 1, 2, 1, 6, 13, 1, 1}, 9, "tcp.tcpConnTable.tcpConnEntry.tcpConnState", DFF1_TCPCONNSTATE, ""}, 
	/* The local IP address for this TCP connection. In the case of a connection in 
	   the listen state which is willing to accept connections for any IP interface 
	   associated with the node, the value 0.0.0.0 is used. */
	{{0x2b, 6, 1, 2, 1, 6, 13, 1, 2}, 9, "tcp.tcpConnTable.tcpConnEntry.tcpConnLocalAddress", ASN1_IPADDR, ""}, 
	/* The local port number for this TCP connection. */
	{{0x2b, 6, 1, 2, 1, 6, 13, 1, 3}, 9, "tcp.tcpConnTable.tcpConnEntry.tcpConnLocalPort", ASN1_INT, ""}, 
	/* he remote IP address for this TCP connection. */
	{{0x2b, 6, 1, 2, 1, 6, 13, 1, 4}, 9, "tcp.tcpConnTable.tcpConnEntry.tcpConnRemAddress", ASN1_IPADDR, ""}, 
	/* The remote port number for this TCP connection. */
	{{0x2b, 6, 1, 2, 1, 6, 13, 1, 5}, 9, "tcp.tcpConnTable.tcpConnEntry.tcpConnRemPort", ASN1_INT, ""}, 
	
	/* The total number of segments received in error (e.g., bad TCP checksums). */
	{{0x2b, 6, 1, 2, 1, 6, 14}, 7, "tcp.tcpInErrs", ASN1_COUNTER, ""}, 
	/* The number of TCP segments sent containing the RST flag. */
	{{0x2b, 6, 1, 2, 1, 6, 15}, 7, "tcp.tcpOutRsts", ASN1_COUNTER, ""}, 

	// *** Udp Group ***
	/* The total number of UDP datagrams delivered to UDP users. */
	{{0x2b, 6, 1, 2, 1, 7, 1}, 7, "udp.udpInDataGrams", ASN1_COUNTER, ""}, 
	/* The total number of received UDP datagrams for which there was no application 
	   at the destination port. */
	{{0x2b, 6, 1, 2, 1, 7, 2}, 7, "udp.udpInNoPorts", ASN1_COUNTER, ""}, 
	/* The number of received UDP datagrams that could not be delivered for reasons 
	   other than the lack of an application at the destination port. */
	{{0x2b, 6, 1, 2, 1, 7, 3}, 7, "udp.udpInErrors", ASN1_COUNTER, ""}, 
	/* The total number of UDP datagrams sent from this entity. */
	{{0x2b, 6, 1, 2, 1, 7, 4}, 7, "udp.udpOutDataGrams", ASN1_COUNTER, ""}, 

	/* The local IP address for this UDP listener.  In the case of a UDP listener 
	   which is willing to accept datagrams for any IP interface associated with 
	   the node, the value 0.0.0.0 is used. */
	{{0x2b, 6, 1, 2, 1, 7, 5, 1, 1}, 9, "udp.udpTable.udpEntry.udpLocalAddress", ASN1_IPADDR, ""}, 
	/* The local port number for this UDP listener. */
	{{0x2b, 6, 1, 2, 1, 7, 5, 1, 2}, 9, "udp.udpTable.udpEntry.udpLocalPort", ASN1_INT, ""}, 

	// *** Egp Group ***
	/* The number of EGP messages received without error. */
	{{0x2b, 6, 1, 2, 1, 8, 1}, 7, "egp.egpInMsgs", ASN1_COUNTER, ""}, 
	/* The number of EGP messages received that proved to be in error. */
	{{0x2b, 6, 1, 2, 1, 8, 2}, 7, "egp.egpInErrors", ASN1_COUNTER, ""}, 
	/* The total number of locally generated EGP messages. */
	{{0x2b, 6, 1, 2, 1, 8, 3}, 7, "egp.egpOutMsgs", ASN1_COUNTER, ""}, 
	/* The number of locally generated EGP messages not sent due to resource 
	   limitations within an EGP entity. */
	{{0x2b, 6, 1, 2, 1, 8, 4}, 7, "egp.egpOutErrors", ASN1_COUNTER, ""}, 

	/* The EGP state of the local system with respect to this entry's EGP neighbor. 
	   Each EGP state is represented by a value that is one greater than the 
	   numerical value associated with said state in RFC 904. */
	{{0x2b, 6, 1, 2, 1, 8, 5, 1, 1}, 9, "egp.egpNeighTable.egpNeighEntry.egpNeighState", DFF1_EGPNEIGHSTATE, ""}, 
	/* The IP address of this entry's EGP neighbor. */
	{{0x2b, 6, 1, 2, 1, 8, 5, 1, 2}, 9, "egp.egpNeighTable.egpNeighEntry.egpNeighAddr", ASN1_IPADDR, ""}, 
	/* The autonomous system of this EGP peer.  Zero should be specified if the 
	   autonomous system number of the neighbor is not yet known. */
	{{0x2b, 6, 1, 2, 1, 8, 5, 1, 3}, 9, "egp.egpNeighTable.egpNeighEntry.egpNeighAs", ASN1_INT, ""}, 
	/* The number of EGP messages received without error from this EGP peer. */
	{{0x2b, 6, 1, 2, 1, 8, 5, 1, 4}, 9, "egp.egpNeighTable.egpNeighEntry.egpNeighInMsgs", ASN1_COUNTER, ""}, 
	/* The number of EGP messages received from this EGP peer that proved to be in 
	   error (e.g., bad EGP checksum). */
	{{0x2b, 6, 1, 2, 1, 8, 5, 1, 5}, 9, "egp.egpNeighTable.egpNeighEntry.egpNeighInErrs", ASN1_COUNTER, ""}, 
	/* The number of locally generated EGP messages to this EGP peer. */
	{{0x2b, 6, 1, 2, 1, 8, 5, 1, 6}, 9, "egp.egpNeighTable.egpNeighEntry.egpNeighOutMsgs", ASN1_COUNTER, ""}, 
	/* The number of locally generated EGP messages not sent to this EGP peer due 
	   to resource limitations within an EGP entity. */
	{{0x2b, 6, 1, 2, 1, 8, 5, 1, 7}, 9, "egp.egpNeighTable.egpNeighEntry.egpNeighOutErrs", ASN1_COUNTER, ""}, 
	/* The number of EGP-defined error messages received from this EGP peer. */
	{{0x2b, 6, 1, 2, 1, 8, 5, 1, 8}, 9, "egp.egpNeighTable.egpNeighEntry.egpNeighInErrMsgs", ASN1_COUNTER, ""}, 
	/* The number of EGP-defined error messages sent to this EGP peer. */
	{{0x2b, 6, 1, 2, 1, 8, 5, 1, 9}, 9, "egp.egpNeighTable.egpNeighEntry.egpNeighOutErrMsgs", ASN1_COUNTER, ""}, 
	/* The number of EGP state transitions to the UP state with this EGP peer. */
	{{0x2b, 6, 1, 2, 1, 8, 5, 1, 10}, 9, "egp.egpNeighTable.egpNeighEntry.egpNeighStateUps", ASN1_COUNTER, ""}, 
	/* The number of EGP state transitions from the UP state to any other state 
	   with this EGP peer. */
	{{0x2b, 6, 1, 2, 1, 8, 5, 1, 11}, 9, "egp.egpNeighTable.egpNeighEntry.egpNeighStateDowns", ASN1_COUNTER, ""}, 
	/* The interval between EGP Hello command retransmissions (in hundredths of a 
	   second). This represents the t1 timer as defined in RFC 904. */
	{{0x2b, 6, 1, 2, 1, 8, 5, 1, 12}, 9, "egp.egpNeighTable.egpNeighEntry.egpNeighIntervalHello", ASN1_INT, ""}, 
	/* The interval between EGP poll command retransmissions (in hundredths of a 
	   second).  This represents the t3 timer as defined in RFC 904. */
	{{0x2b, 6, 1, 2, 1, 8, 5, 1, 13}, 9, "egp.egpNeighTable.egpNeighEntry.egpNeighIntervalPoll", ASN1_INT, ""}, 
	/* The polling mode of this EGP entity, either passive or active. */
	{{0x2b, 6, 1, 2, 1, 8, 5, 1, 14}, 9, "egp.egpNeighTable.egpNeighEntry.egpNeighMode", DFF1_EGPNEIGHMODE, ""}, 
	/* A control variable used to trigger operator-initiated Start and Stop events. 
	   When read, this variable always returns the most recent value that 
	   egpNeighEventTrigger was set to.  If it has not been set since the last 
	   initialization of the network management subsystem on the node, it returns 
	   a value of `stop'.

       When set, this variable causes a Start or Stop event on the specified 
	   neighbor, as specified on pages 8-10 of RFC 904.  Briefly, a Start event 
	   causes an Idle peer to begin neighbor acquisition and a non-Idle peer to 
	   reinitiate neighbor acquisition.  A stop event causes a non-Idle peer to 
	   return to the Idle state until a Start event occurs, either via 
	   egpNeighEventTrigger or otherwise. */
	{{0x2b, 6, 1, 2, 1, 8, 5, 1, 15}, 9, "egp.egpNeighTable.egpNeighEntry.egpNeighEventTrigger", DFF1_EGPNEIGHEVENTT, ""}, 

	/* The autonomous system number of this EGP entity. */
	{{0x2b, 6, 1, 2, 1, 8, 6}, 7, "egp.egpAs", ASN1_INT, ""}, 

	// *** Snmp Group ***
	/* The total number of Messages delivered to the SNMP entity from the transport 
	   service. */
	{{0x2b, 6, 1, 2, 1, 11, 1}, 7, "snmp.snmpInPkts", ASN1_COUNTER, ""}, 
	/* The total number of SNMP Messages which were passed from the SNMP protocol 
	   entity to the transport service. */
	{{0x2b, 6, 1, 2, 1, 11, 2}, 7, "snmp.snmpOutPkts", ASN1_COUNTER, ""}, 
	/* The total number of SNMP Messages which were delivered to the SNMP protocol 
	   entity and were for an unsupported SNMP version. */
	{{0x2b, 6, 1, 2, 1, 11, 3}, 7, "snmp.snmpInBadVersions", ASN1_COUNTER, ""}, 
	/* The total number of SNMP Messages delivered to the SNMP protocol entity 
	   which used a SNMP community name not known to said entity. */
	{{0x2b, 6, 1, 2, 1, 11, 4}, 7, "snmp.snmpInBadCommunityNames", ASN1_COUNTER, ""}, 
	/* The total number of SNMP Messages delivered to the SNMP protocol entity 
	   which represented an SNMP operation which was not allowed by the SNMP
	   community named in the Message. */
	{{0x2b, 6, 1, 2, 1, 11, 5}, 7, "snmp.snmpInBadCommunityUses", ASN1_COUNTER, ""}, 
	/* The total number of ASN.1 or BER errors encountered by the SNMP protocol 
	   entity when decoding received SNMP Messages. */
	{{0x2b, 6, 1, 2, 1, 11, 6}, 7, "snmp.snmpInASNParseErrs", ASN1_COUNTER, ""}, 
	/* The total number of SNMP PDUs which were delivered to the SNMP protocol 
	   entity and for which the value of the error-status field is `tooBig'. */
	{{0x2b, 6, 1, 2, 1, 11, 8}, 7, "snmp.snmpInTooBigs", ASN1_COUNTER, ""}, 
	/* The total number of SNMP PDUs which were delivered to the SNMP protocol 
	   entity and for which the value of the error-status field is `noSuchName'. */
	{{0x2b, 6, 1, 2, 1, 11, 9}, 7, "snmp.snmpInNoSuchNames", ASN1_COUNTER, ""}, 
	/* The total number of SNMP PDUs which were delivered to the SNMP protocol 
	   entity and for which the value of the error-status field is `badValue'. */
	{{0x2b, 6, 1, 2, 1, 11, 10}, 7, "snmp.snmpInBadValues", ASN1_COUNTER, ""}, 
	/* The total number valid SNMP PDUs which were delivered to the SNMP protocol 
	   entity and for which the value of the error-status field is `readOnly'.  It 
	   should be noted that it is a protocol error to generate an SNMP PDU which
       contains the value `readOnly' in the error-status field, as such this object 
	   is provided as a means of detecting incorrect implementations of the SNMP. */
	{{0x2b, 6, 1, 2, 1, 11, 11}, 7, "snmp.snmpInReadOnlys", ASN1_COUNTER, ""}, 
	/* The total number of SNMP PDUs which were delivered to the SNMP protocol 
	   entity and for which the value of the error-status field is `genErr'. */
	{{0x2b, 6, 1, 2, 1, 11, 12}, 7, "snmp.snmpInGenErrs", ASN1_COUNTER, ""}, 
	/* The total number of MIB objects which have been retrieved successfully by 
	   the SNMP protocol entity as the result of receiving valid SNMP Get-Request
       and Get-Next PDUs. */
	{{0x2b, 6, 1, 2, 1, 11, 13}, 7, "snmp.snmpInTotalReqVars", ASN1_COUNTER, ""}, 
	/* The total number of MIB objects which have been altered successfully by the 
	   SNMP protocol entity as the result of receiving valid SNMP Set-Request 
	   PDUs. */
	{{0x2b, 6, 1, 2, 1, 11, 14}, 7, "snmp.snmpInTotalSetVars", ASN1_COUNTER, ""}, 
	/* The total number of SNMP Get-Request PDUs which have been accepted and 
	   processed by the SNMP protocol entity. */
	{{0x2b, 6, 1, 2, 1, 11, 15}, 7, "snmp.snmpInGetRequests", ASN1_COUNTER, ""}, 
	/* The total number of SNMP Get-Next PDUs which have been accepted and processed 
	   by the SNMP protocol entity. */
	{{0x2b, 6, 1, 2, 1, 11, 16}, 7, "snmp.snmpInGetNexts", ASN1_COUNTER, ""}, 
	/* The total number of SNMP Set-Request PDUs which have been accepted and 
	   processed by the SNMP protocol entity. */
	{{0x2b, 6, 1, 2, 1, 11, 17}, 7, "snmp.snmpInSetRequests", ASN1_COUNTER, ""}, 
	/* The total number of SNMP Get-Response PDUs which have been accepted and 
	   processed by the SNMP protocol entity. */
	{{0x2b, 6, 1, 2, 1, 11, 18}, 7, "snmp.snmpInGetResponses", ASN1_COUNTER, ""}, 
	/* The total number of SNMP Trap PDUs which have been accepted and processed 
	   by the SNMP protocol entity. */
	{{0x2b, 6, 1, 2, 1, 11, 19}, 7, "snmp.snmpInTraps", ASN1_COUNTER, ""}, 
	/* The total number of SNMP PDUs which were generated by the SNMP protocol 
	   entity and for which the value of the error-status field is `tooBig.' */
	{{0x2b, 6, 1, 2, 1, 11, 20}, 7, "snmp.snmpOutTooBigs", ASN1_COUNTER, ""}, 
	/* The total number of SNMP PDUs which were generated by the SNMP protocol 
	   entity and for which the value of the error-status is `noSuchName'. */
	{{0x2b, 6, 1, 2, 1, 11, 21}, 7, "snmp.snmpOutNoSuchNames", ASN1_COUNTER, ""}, 
	/* The total number of SNMP PDUs which were generated by the SNMP protocol 
	   entity and for which the value of the error-status field is `badValue'. */
	{{0x2b, 6, 1, 2, 1, 11, 22}, 7, "snmp.snmpOutBadValues", ASN1_COUNTER, ""}, 
	/* The total number of SNMP PDUs which were generated by the SNMP protocol 
	   entity and for which the value of the error-status field is `genErr'. */
	{{0x2b, 6, 1, 2, 1, 11, 24}, 7, "snmp.snmpOutGenErrs", ASN1_COUNTER, ""}, 
	/* The total number of SNMP Get-Request PDUs which have been generated by the 
	   SNMP protocol entity. */
	{{0x2b, 6, 1, 2, 1, 11, 25}, 7, "snmp.snmpOutGetRequests", ASN1_COUNTER, ""}, 
	/* The total number of SNMP Get-Next PDUs which have been generated by the 
	   SNMP protocol entity. */
	{{0x2b, 6, 1, 2, 1, 11, 26}, 7, "snmp.snmpOutGetNexts", ASN1_COUNTER, ""}, 
	/* The total number of SNMP Set-Request PDUs which have been generated by the 
	   SNMP protocol entity. */
	{{0x2b, 6, 1, 2, 1, 11, 27}, 7, "snmp.snmpOutSetRequests", ASN1_COUNTER, ""}, 
	/* The total number of SNMP Get-Response PDUs which have been generated by the 
	   SNMP protocol entity. */
	{{0x2b, 6, 1, 2, 1, 11, 28}, 7, "snmp.snmpOutGetResponses", ASN1_COUNTER, ""}, 
	/* The total number of SNMP Trap PDUs which have been generated by the SNMP 
	   protocol entity. */
	{{0x2b, 6, 1, 2, 1, 11, 29}, 7, "snmp.snmpOutTraps", ASN1_COUNTER, ""}, 
	/* Indicates whether the SNMP agent process is permitted to generate 
	   authentication-failure traps.  The value of this object overrides any 
	   configuration information; as such, it provides a means whereby all 
	   authentication-failure traps may be disabled.

       Note that it is strongly recommended that this object be stored in 
	   non-volatile memory so that it remains constant between re-initializations 
	   of the network management system. */
	{{0x2b, 6, 1, 2, 1, 11, 30}, 7, "snmp.snmpEnableAuthenTraps", DFF1_SNMPENABLETRAP, ""}
};

int a1writeoid(unsigned char *packp, struct oid *oidp)
{
	register u_char	*pp;
	int				i;
	u_char			*objidp, *lenp;

	pp = packp;
	lenp = pp++;
	objidp = pp;

	for(i = 0; i < oidp->len; i ++)
		if(oidp->id[i] < CHAR_HIBIT)	/* short form */
			*pp++ = oidp->id[i];
	
		else if(oidp->id[i] < BYTE2_HIBIT)
		{
			*pp++ = (u_char)(oidp->id[i] >> 7) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] & ~CHAR_HIBIT);
		}
		else if(oidp->id[i] < BYTE3_HIBIT)
		{
			*pp++ = (u_char)(oidp->id[i] >> 14) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] >> 7) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] & ~CHAR_HIBIT);
		}
		/*else if(oidp->id[i] < BYTE4_HIBIT)
		{
			*pp++ = (u_char)(oidp->id[i] >> 21) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] >> 14) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] >> 7) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] & ~CHAR_HIBIT);
		}*/
		else
		{
			*pp++ = (u_char)(oidp->id[i] >> 28) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] >> 21) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] >> 14) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] >> 7) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] & ~CHAR_HIBIT);
		}
	
	*lenp = pp - objidp;
	return pp-packp;
}

/*int a1writeoid(unsigned char *packp, struct oid *oidp)
{
	register u_char	*pp;
	int				i;
	u_char			*objidp, *lenp;

	pp = packp;
	lenp = pp++;
	objidp = pp;

	for(i = 0; i < oidp->len; i ++)
		if(oidp->id[i] < CHAR_HIBIT)	/* short form */
/*			*pp++ = oidp->id[i];
		else if(oidp->id[i] < BYTE2_HIBIT)
		{
			*pp++ = (u_char)(oidp->id[i] >> 7) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] & ~CHAR_HIBIT);
		}
		else if(oidp->id[i] < BYTE3_HIBIT)
		{
			*pp++ = (u_char)(oidp->id[i] >> 14) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] >> 7) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] & ~CHAR_HIBIT);
		}
		else if(oidp->id[i] < BYTE4_HIBIT)
		{
			*pp++ = (u_char)(oidp->id[i] >> 21) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] >> 14) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] >> 7) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] & ~CHAR_HIBIT);
		}
		else
		{
			*pp++ = (u_char)(oidp->id[i] >> 28) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] >> 21) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] >> 14) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] >> 7) | CHAR_HIBIT;
			*pp++ = (u_char)(oidp->id[i] & ~CHAR_HIBIT);
		}
	
	*lenp = pp - objidp;
	return pp-packp;
}*/

int a1writelen(u_char *pp, int len)
{
	/* if len < 128 then use short form */
	if(len < CHAR_HIBIT)
	{
		*pp = len;
		return 1;
	}

	/* use long form, where bit 7 = 1, and bits 6 - 0 encode the
		number of subsequent octets that specify the length */
	if(len <= 255)
	{
		*pp++ = CHAR_HIBIT | 1;
		*pp = len & 0xFF;
		return 2;
	}
	/* else, assume len <= 65535 (2^16 - 1) */
	*pp++ = CHAR_HIBIT | 2;
	*pp++ = len >> CHAR_BITS;
	*pp = len & 0xFF;
	return 3;
}

int a1writeint(int val, u_char *buffp, int a1type)
{
	unsigned		tmp;
	register u_char *bp;
	u_char			num[10];
	int				numbytes = 0;

	bp = buffp;
	tmp = val;
	if (A1_SIGNED(a1type) && val < 0)
		tmp = -val;

	int v1 = tmp / 256;
	int v2 = tmp % 256;
	while(v1)
	{
		tmp = v1;

		num[numbytes++] = v2;

		v1 = tmp / 256;
		v2 = tmp % 256;
	}
	num[numbytes++] = v2;
	if(v2 >= (unsigned)CHAR_HIBIT)
	{
		num[numbytes++] = 0x00;
	}

	*bp++ = numbytes;
	for(int i = numbytes - 1;i >= 0;i --)
		*bp++ = num[i];

	return bp - buffp;
}
/*
int a1writeint(int val, u_char *buffp, int a1type)
{
	unsigned		tmp, numbytes;
	register u_char *bp;

	bp = buffp;
	tmp = val;
	if (A1_SIGNED(a1type) && val < 0)
		tmp = -val;

	if(tmp < (unsigned)CHAR_HIBIT)
	{
		*bp++ = numbytes = (u_char)1;
		*bp++ = (u_char)val;
	}
	else if(tmp < (unsigned)BYTE2_HIBIT)
	{
		*bp++ = numbytes = (u_char)2;
		*bp++ = (u_char)(val >> 7) | CHAR_HIBIT;
		*bp++ = (u_char)(val & ~CHAR_HIBIT);
	}
	else if(tmp < (unsigned)BYTE3_HIBIT)
	{
		*bp++ = numbytes = (u_char)3;
		*bp++ = (u_char)(val >> 14) | CHAR_HIBIT;
		*bp++ = (u_char)(val >> 7) | CHAR_HIBIT;
		*bp++ = (u_char)(val & ~CHAR_HIBIT);
	}
	else if(tmp < (unsigned)BYTE4_HIBIT)
	{
		*bp++ = numbytes = (u_char)4;
		*bp++ = (u_char)(val >> 21) | CHAR_HIBIT;
		*bp++ = (u_char)(val >> 14) | CHAR_HIBIT;
		*bp++ = (u_char)(val >> 7) | CHAR_HIBIT;
		*bp++ = (u_char)(val & ~CHAR_HIBIT);
	}
	else
	{
		*bp++ = (u_char)5;
		*bp++ = (u_char)0;
		numbytes = 4;
		*bp++ = (u_char)(val >> 28) | CHAR_HIBIT;
		*bp++ = (u_char)(val >> 21) | CHAR_HIBIT;
		*bp++ = (u_char)(val >> 14) | CHAR_HIBIT;
		*bp++ = (u_char)(val >> 7) | CHAR_HIBIT;
		*bp++ = (u_char)(val & ~CHAR_HIBIT);
	}

	//strncpy((char *)bp, ((char *)&val) + (sizeof(int) - numbytes), numbytes);
	return bp - buffp;
}
*/
int a1readlen(unsigned char *pack, int *lenlen)
{
	int		totlen;
	int		i;

	if(!((*pack) & CHAR_HIBIT))
	{
		*lenlen = 1;
		return (*pack) & ~CHAR_HIBIT;
	}

	*lenlen = (*pack++ & ~CHAR_HIBIT) + 1;

	for(i = 0, totlen = 0; i < (*lenlen) - 1; i++)
		totlen = (totlen << CHAR_BITS) | (int)*pack++;

	return totlen;
}

int a1readoid(unsigned char *pack, int objidlen, struct oid *objid)
{
	int		val;
	u_char	*pp;

	objid->len = 0;
	pp = pack;

	for(; pp < pack + objidlen; objid->len++)
	{
		if(!(*pp & CHAR_HIBIT))
		{
			objid->id[objid->len] = *pp++;
			continue;
		}
		/*
		 * using long form, where bits 6 - 0 of each
		 * octet are used; (bit 7 == 0) ==> last octet
		 */
		val = 0;
		do
			val = (val << 7) | (int)(*pp & ~CHAR_HIBIT);
		while(*pp++ & CHAR_HIBIT);
		objid->id[objid->len] = val;
	}

	return 0;
}

int a1readint(u_char *pack, int len)
{
	int	i = 0, j = 0;
	int	tot = 0;

	for(i = len - 1, j = 0;i >= 0;i --, j ++)
	{
		tot += (int)(pack[j] * pow(0x100, i));
	}

	return tot;
}
/*
int a1readint(u_char *pack, int len)
{
	register int	tot;
	u_char			neg;
	int				tlen;

	if((tlen = len) > sizeof(int))
		return 0;
	tot = *pack & ~CHAR_HIBIT;
	neg = *pack & CHAR_HIBIT;
	for(tlen--, pack++; tlen > 0; tlen--, pack++)
		tot = (tot << CHAR_BITS) | (int)*pack;
	if(neg)
		tot -= (1 << ((len * CHAR_BITS) - 1));

	return tot;
}
*/
int SN_VALUE(int s, struct sockaddr_in serv, char *community, int request_id, struct oid *oidp, struct snval *psnval)
{
	u_char				request[1000];
	u_char				reply[1000];
	int					i, tmp, count = 0;

	// SNMP Version Information
	u_char	ver[20] = {0};
	int		verlen = 0;
	ver[0] = ASN1_INT;
	verlen = 1 + a1writeint(SNMP_VERSION - 1, ver + 1, ASN1_INT);
	// SNMP Community Information
	u_char	comm[255] = {0};
	int		commlen = 0;
	comm[0] = ASN1_OCTSTR;
	tmp = a1writelen(comm + 1, strlen(community));
	strncpy((char *)(comm + 1 + tmp), community, strlen(community));
	commlen = 1 + tmp + strlen(community);

	// SNMP Request Identifier
	u_char	reqid[20] = {0};
	int		reqidlen = 0;
	reqid[0] = ASN1_INT;
	tmp = a1writeint(request_id, reqid + 1, ASN1_INT);
	reqidlen += 1, reqidlen += tmp;
	// SNMP Error Status
	u_char	errstatus[20] = {0};
	int		errstatuslen = 0;
	errstatus[0] = ASN1_INT;
	errstatuslen = 1 + a1writeint(0, errstatus + 1, ASN1_INT);

	// SNMP Error Index
	u_char	errindex[20] = {0};
	int		errindexlen = 0;
	errindex[0] = ASN1_INT;
	errindexlen = 1 + a1writeint(0, errindex + 1, ASN1_INT);

	// SNMP Request Object
	u_char	object[255] = {0};
	int		objlen = 0;
	object[0] = ASN1_OBJID;
	objlen += 1;
	tmp = a1writeoid(object + 1, oidp);
	objlen += tmp;

	// SNMP Value
	u_char	value[255] = {0};
	int		valuelen = 0;
	value[0] = ASN1_NULL;
	a1writelen(value + 1, 0);
	valuelen = 2;

	// SNMP Object-Value
	u_char	objval[255] = {0};
	int		objvallen = 0;
	objval[0] = ASN1_SEQ;
	tmp = a1writelen(objval + 1, objlen + 2);
	objvallen += 1, objvallen += tmp;
	for(i = 0;i < objlen;i ++)
		objval[objvallen ++] = object[i];
	for(i = 0;i < valuelen;i ++)
		objval[objvallen ++] = value[i];

	// SNMP Objects
	u_char	objects[255] = {0};
	int		objectslen = 0;
	objects[0] = ASN1_SEQ;
	tmp = a1writelen(objects + 1, objvallen);
	objectslen += 1, objectslen += tmp;
	for(i = 0; i < objvallen; i ++)
		objects[objectslen ++] = objval[i];

	// SNMP PDU Type Information
	u_char	pdu[255] = {0};
	int		pdulen = 0;
	pdu[0] = PDU_GET;
	tmp = a1writelen(pdu + 1, reqidlen + errstatuslen + errindexlen + objectslen);
	pdulen += 1, pdulen += tmp;
	for(i = 0;i < reqidlen;i ++)
		pdu[pdulen ++] = reqid[i];
	for(i = 0;i < errstatuslen;i ++)
		pdu[pdulen ++] = errstatus[i];
	for(i = 0;i < errindexlen;i ++)
		pdu[pdulen ++] = errindex[i];
	for(i = 0;i < objectslen;i ++)
		pdu[pdulen ++] = objects[i];

	// SNMP Request
	int		requestlen = 0;
	request[0] = ASN1_SEQ;
	tmp = a1writelen(request + 1, verlen + commlen + pdulen);
	requestlen += 1, requestlen += tmp;
	for(i = 0;i < verlen;i ++)
		request[requestlen++] = ver[i];
	for(i = 0;i < commlen;i ++)
		request[requestlen++] = comm[i];
	for(i = 0;i < pdulen;i ++)
		request[requestlen ++] = pdu[i];

	int dw, n;
w:	n = sendto(s, (char *)request, requestlen, 0, (struct sockaddr *)&serv, sizeof(serv));
	if(n == SOCKET_ERROR)
	{
		return -7;
	}
	
	if(RESPONSE_WAIT(s, SNMP_WAIT_TIMEOUT) > 0)
	{
		dw = sizeof(serv);
		n = recvfrom(s, (char *)reply, 1000, 0, (struct sockaddr *)&serv, &dw);
		if(n == SOCKET_ERROR || n == 0)
		{
			return -8;
		}
	
		int index = 0, field = 0, errstat = 0;
		SN_PARSE(reply, n, &index, &field, &errstat, NULL, psnval);

		return -errstat;
	}
	else
	{
		count ++;
		if(count < SNMP_RETRY_TIMES) goto w;

		return -6;
	}
}

int SN_NEXT_VALUE(int s, struct sockaddr_in serv, char *community, int request_id, struct oid *oidp, struct oid *oidr, struct snval *psnval)
{
	u_char				request[1000];
	u_char				reply[1000];
	int					i, tmp, count = 0;

	// SNMP Version Information
	u_char	ver[20] = {0};
	int		verlen = 0;
	ver[0] = ASN1_INT;
	verlen = 1 + a1writeint(SNMP_VERSION - 1, ver + 1, ASN1_INT);
	// SNMP Community Information
	u_char	comm[255] = {0};
	int		commlen = 0;
	comm[0] = ASN1_OCTSTR;
	tmp = a1writelen(comm + 1, strlen(community));
	strncpy((char *)(comm + 1 + tmp), community, strlen(community));
	commlen = 1 + tmp + strlen(community);

	// SNMP Request Identifier
	u_char	reqid[20] = {0};
	int		reqidlen = 0;
	reqid[0] = ASN1_INT;
	tmp = a1writeint(request_id, reqid + 1, ASN1_INT);
	reqidlen += 1, reqidlen += tmp;

	// SNMP Error Status
	u_char	errstatus[20] = {0};
	int		errstatuslen = 0;
	errstatus[0] = ASN1_INT;
	errstatuslen = 1 + a1writeint(0, errstatus + 1, ASN1_INT);

	// SNMP Error Index
	u_char	errindex[20] = {0};
	int		errindexlen = 0;
	errindex[0] = ASN1_INT;
	errindexlen = 1 + a1writeint(0, errindex + 1, ASN1_INT);

	// SNMP Request Object
	u_char	object[255] = {0};
	int		objlen = 0;
	object[0] = ASN1_OBJID;
	objlen += 1;
	tmp = a1writeoid(object + 1, oidp);
	objlen += tmp;

	// SNMP Value
	u_char	value[255] = {0};
	int		valuelen = 0;
	value[0] = ASN1_NULL;
	a1writelen(value + 1, 0);
	valuelen = 2;

	// SNMP Object-Value
	u_char	objval[255] = {0};
	int		objvallen = 0;
	objval[0] = ASN1_SEQ;
	tmp = a1writelen(objval + 1, objlen + 2);
	objvallen += 1, objvallen += tmp;
	for(i = 0;i < objlen;i ++)
		objval[objvallen ++] = object[i];
	for(i = 0;i < valuelen;i ++)
		objval[objvallen ++] = value[i];

	// SNMP Objects
	u_char	objects[255] = {0};
	int		objectslen = 0;
	objects[0] = ASN1_SEQ;
	tmp = a1writelen(objects + 1, objvallen);
	objectslen += 1, objectslen += tmp;
	for(i = 0; i < objvallen; i ++)
		objects[objectslen ++] = objval[i];

	// SNMP PDU Type Information
	u_char	pdu[255] = {0};
	int		pdulen = 0;
	pdu[0] = PDU_GETN;
	tmp = a1writelen(pdu + 1, reqidlen + errstatuslen + errindexlen + objectslen);
	pdulen += 1, pdulen += tmp;
	for(i = 0;i < reqidlen;i ++)
		pdu[pdulen ++] = reqid[i];
	for(i = 0;i < errstatuslen;i ++)
		pdu[pdulen ++] = errstatus[i];
	for(i = 0;i < errindexlen;i ++)
		pdu[pdulen ++] = errindex[i];
	for(i = 0;i < objectslen;i ++)
		pdu[pdulen ++] = objects[i];

	// SNMP Request
	int		requestlen = 0;
	request[0] = ASN1_SEQ;
	tmp = a1writelen(request + 1, verlen + commlen + pdulen);
	requestlen += 1, requestlen += tmp;
	for(i = 0;i < verlen;i ++)
		request[requestlen++] = ver[i];
	for(i = 0;i < commlen;i ++)
		request[requestlen++] = comm[i];
	for(i = 0;i < pdulen;i ++)
		request[requestlen ++] = pdu[i];

	int dw, n;
w:	n = sendto(s, (char *)request, requestlen, 0, (struct sockaddr *)&serv, sizeof(serv));
	if(n == SOCKET_ERROR)
	{
		return -7;
	}

	if(RESPONSE_WAIT(s, SNMP_WAIT_TIMEOUT) > 0)
	{
		dw = sizeof(serv);
		n = recvfrom(s, (char *)reply, 1000, 0, (struct sockaddr *)&serv, &dw);
		if(n == SOCKET_ERROR || n == 0)
		{
			return -8;
		}

		int index = 0, field = 0, errstat = 0;
		SN_PARSE(reply, n, &index, &field, &errstat, oidr, psnval);

		return -errstat;
	}
	else
	{
		count ++;
		if(count < SNMP_RETRY_TIMES) goto w;

		return -6;
	}
}

int WriteLog(const char* str);

int SN_VALUES(int s, struct sockaddr_in serv, char *community, int request_id, 
			  struct oid *oidp, struct snval **ppsnval)
{
	u_char				request[1000];
	u_char				reply[1000];
	int					i, tmp, count = 0;
	memset(request, 0 , 1000);
	memset(reply,0 ,1000);
	// SNMP Version Information
	u_char	ver[20] = {0};
	memset(ver, 0 ,20);
	int		verlen = 0;
	ver[0] = ASN1_INT;
	verlen = 1 + a1writeint(SNMP_VERSION - 1, ver + 1, ASN1_INT);
	// SNMP Community Information
	u_char	comm[255] = {0};
	memset(comm, 0 ,255);
	int		commlen = 0;
	comm[0] = ASN1_OCTSTR;
	tmp = a1writelen(comm + 1, strlen(community));
	strncpy((char *)(comm + 1 + tmp), community, strlen(community));
	commlen = 1 + tmp + strlen(community);

	// SNMP Request Identifier
	u_char	reqid[20] = {0};
	memset(reqid, 0, 20);
	int		reqidlen = 0;
	reqid[0] = ASN1_INT;
	tmp = a1writeint(request_id, reqid + 1, ASN1_INT);
	reqidlen += 1, reqidlen += tmp;
	// SNMP Error Status
	u_char	errstatus[20] = {0};
	memset(errstatus, 0, 20);
	int		errstatuslen = 0;
	errstatus[0] = ASN1_INT;
	errstatuslen = 1 + a1writeint(0, errstatus + 1, ASN1_INT);

	// SNMP Error Index
	u_char	errindex[20] = {0};
	memset(errindex, 0, 20);
	int		errindexlen = 0;
	errindex[0] = ASN1_INT;
	errindexlen = 1 + a1writeint(0, errindex + 1, ASN1_INT);

	u_char	objval[1024] = {0};
	memset(objval, 0, 1024);
	int		objvallen = 0;
	struct oid	*poid = oidp;
	while(poid)
	{
		// SNMP Request Object
		u_char	object[1024] = {0};
		memset(object, 0, 1024);
		int		objlen = 0;
		object[objlen++] = ASN1_OBJID;
		objlen += a1writeoid(object + 1, poid);

		// SNMP Value
		object[objlen++] = ASN1_NULL;
		object[objlen++] = (u_char)0;

		// SNMP Object-Value
		objval[objvallen++] = ASN1_SEQ;
		objvallen += a1writelen(objval + objvallen, objlen);
		for(i = 0;i < objlen;i ++)
			objval[objvallen ++] = object[i];

		poid = poid->next;
	}

	// SNMP Objects
	u_char	objects[8192] = {0};
	memset(objects, 0 , 8192);
	int		objectslen = 0;
	objects[0] = ASN1_SEQ;
	tmp = a1writelen(objects + 1, objvallen);
	objectslen += 1, objectslen += tmp;
	for(i = 0; i < objvallen; i ++)
		objects[objectslen ++] = objval[i];

	// SNMP PDU Type Information
	u_char	pdu[8192] = {0};
	memset(pdu, 0, 8192);
	int		pdulen = 0;
	pdu[0] = PDU_GET;
	tmp = a1writelen(pdu + 1, reqidlen + errstatuslen + errindexlen + objectslen);
	pdulen += 1, pdulen += tmp;
	for(i = 0;i < reqidlen;i ++)
		pdu[pdulen ++] = reqid[i];
	for(i = 0;i < errstatuslen;i ++)
		pdu[pdulen ++] = errstatus[i];
	for(i = 0;i < errindexlen;i ++)
		pdu[pdulen ++] = errindex[i];
	for(i = 0;i < objectslen;i ++)
		pdu[pdulen ++] = objects[i];

	// SNMP Request

	int		requestlen = 0;
	request[0] = ASN1_SEQ;
	tmp = a1writelen(request + 1, verlen + commlen + pdulen);
	requestlen += 1, requestlen += tmp;
	for(i = 0;i < verlen;i ++)
		request[requestlen++] = ver[i];
	for(i = 0;i < commlen;i ++)
		request[requestlen++] = comm[i];
	for(i = 0;i < pdulen;i ++)
		request[requestlen ++] = pdu[i];

	int dw, n;

	WriteLog("=============== SN_VALUES ================");
	WriteLog("pdu=");
	WriteLog((char*)pdu);

w:	n = sendto(s, (char *)request, requestlen, 0, (struct sockaddr *)&serv, sizeof(serv));
	if(n == SOCKET_ERROR)
	{
		return -7;
	}
	
	if(RESPONSE_WAIT(s, SNMP_WAIT_TIMEOUT) > 0)
	{
		dw = sizeof(serv);
		n = recvfrom(s, (char *)reply, 1000, 0, (struct sockaddr *)&serv, &dw);

		WriteLog("reply=");
		WriteLog((char*)reply);

		if(n == SOCKET_ERROR || n == 0)
		{
			return -8;
		}

		int index = 0, field = 0, errstat = 0;
		SN_PARSE_VALUES(reply, n, &index, &field, &errstat, NULL, ppsnval);

		char strTempErr[32];
		sprintf(strTempErr,"errstat=%d",errstat);
		WriteLog(strTempErr);


		return -errstat;
	}
	else
	{
		count ++;
		if(count < SNMP_RETRY_TIMES) goto w;

		return -6;
	}
}

int SN_SET_VALUES(int s, struct sockaddr_in serv, char *community, int request_id, struct oid *oidp, struct snval *psnval)
{
	u_char				request[1000];
	u_char				reply[1000];
	int					i, tmp, count = 0;

	// SNMP Version Information
	u_char	ver[20] = {0};
	int		verlen = 0;
	ver[0] = ASN1_INT;
	verlen = 1 + a1writeint(SNMP_VERSION - 1, ver + 1, ASN1_INT);
	// SNMP Community Information
	u_char	comm[255] = {0};
	int		commlen = 0;
	comm[0] = ASN1_OCTSTR;
	tmp = a1writelen(comm + 1, strlen(community));
	strncpy((char *)(comm + 1 + tmp), community, strlen(community));
	commlen = 1 + tmp + strlen(community);

	// SNMP Request Identifier
	u_char	reqid[20] = {0};
	int		reqidlen = 0;
	reqid[0] = ASN1_INT;
	tmp = a1writeint(request_id, reqid + 1, ASN1_INT);
	reqidlen += 1, reqidlen += tmp;
	// SNMP Error Status
	u_char	errstatus[20] = {0};
	int		errstatuslen = 0;
	errstatus[0] = ASN1_INT;
	errstatuslen = 1 + a1writeint(0, errstatus + 1, ASN1_INT);

	// SNMP Error Index
	u_char	errindex[20] = {0};
	int		errindexlen = 0;
	errindex[0] = ASN1_INT;
	errindexlen = 1 + a1writeint(0, errindex + 1, ASN1_INT);

	u_char	objval[1024] = {0};
	int		objvallen = 0;
	struct oid		*poid = oidp;
	struct snval	*pval = psnval;
	while(poid && pval)
	{
		// SNMP Request Object
		u_char	object[255] = {0};
		int		objlen = 0;
		object[objlen++] = ASN1_OBJID;
		objlen += a1writeoid(object + 1, poid);

		// SNMP Value
		object[objlen++] = pval->sv_type;
		switch(pval->sv_type)
		{
		case ASN1_INT:
		case ASN1_COUNTER:
		case ASN1_GAUGE:
		case ASN1_TIMETICKS:
		case ASN1_COUNTER64:
		case ASN1_UINTEGER32:
			tmp = a1writeint(pval->sv_val.sv_int, object + objlen, ASN1_INT);
			objlen += tmp;
			break;
		case ASN1_IPADDR:
			tmp = a1writelen(object + objlen, 4);
			*(object + objlen + tmp) = pval->sv_val.sv_ipaddr[0];
			*(object + objlen + tmp + 1) = pval->sv_val.sv_ipaddr[1];
			*(object + objlen + tmp + 2) = pval->sv_val.sv_ipaddr[2];
			*(object + objlen + tmp + 3) = pval->sv_val.sv_ipaddr[3];
			objlen = objlen + tmp + 4;
			break;
		case ASN1_BITSTR:
			break;
		case ASN1_OCTSTR:
			tmp = a1writelen(object + objlen, pval->sv_val.sv_str.sv_len);
			strncpy((char *)(object + objlen + tmp), pval->sv_val.sv_str.sv_str, pval->sv_val.sv_str.sv_len);
			objlen = objlen + tmp + pval->sv_val.sv_str.sv_len;
			break;
		case ASN1_NULL:
			object[objlen++] = (u_char)0;
			break;
		case ASN1_OBJID:
			objlen += a1writeoid(object + objlen, &pval->sv_val.sv_oid);
			break;
		}

		// SNMP Object-Value
		objval[objvallen++] = ASN1_SEQ;
		objvallen += a1writelen(objval + objvallen, objlen);
		for(i = 0;i < objlen;i ++)
			objval[objvallen ++] = object[i];

		poid = poid->next;
		pval = pval->next;
	}

	// SNMP Objects
	u_char	objects[1024] = {0};
	int		objectslen = 0;
	objects[0] = ASN1_SEQ;
	tmp = a1writelen(objects + 1, objvallen);
	objectslen += 1, objectslen += tmp;
	for(i = 0; i < objvallen; i ++)
		objects[objectslen ++] = objval[i];

	// SNMP PDU Type Information
	u_char	pdu[255] = {0};
	int		pdulen = 0;
	pdu[0] = PDU_SET;
	tmp = a1writelen(pdu + 1, reqidlen + errstatuslen + errindexlen + objectslen);
	pdulen += 1, pdulen += tmp;
	for(i = 0;i < reqidlen;i ++)
		pdu[pdulen ++] = reqid[i];
	for(i = 0;i < errstatuslen;i ++)
		pdu[pdulen ++] = errstatus[i];
	for(i = 0;i < errindexlen;i ++)
		pdu[pdulen ++] = errindex[i];
	for(i = 0;i < objectslen;i ++)
		pdu[pdulen ++] = objects[i];

	// SNMP Request
	int		requestlen = 0;
	request[0] = ASN1_SEQ;
	tmp = a1writelen(request + 1, verlen + commlen + pdulen);
	requestlen += 1, requestlen += tmp;
	for(i = 0;i < verlen;i ++)
		request[requestlen++] = ver[i];
	for(i = 0;i < commlen;i ++)
		request[requestlen++] = comm[i];
	for(i = 0;i < pdulen;i ++)
		request[requestlen ++] = pdu[i];

	int dw, n;
w:	n = sendto(s, (char *)request, requestlen, 0, (struct sockaddr *)&serv, sizeof(serv));
	if(n == SOCKET_ERROR)
	{
		return -7;
	}
	
	if(RESPONSE_WAIT(s, SNMP_WAIT_TIMEOUT) > 0)
	{
		dw = sizeof(serv);
		n = recvfrom(s, (char *)reply, 1000, 0, (struct sockaddr *)&serv, &dw);
		if(n == SOCKET_ERROR || n == 0)
		{
			return -8;
		}

		int index = 0, field = 0, errstat = 0;
		struct snval *prval = NULL;

		SN_PARSE_VALUES(reply, n, &index, &field, &errstat, NULL, &prval);

		struct snval *ptmp = prval;
		struct snval *ptmpnext = NULL;
		if(ptmp) ptmpnext = ptmp->next;
		while(ptmp)
		{
			free(ptmp);
			ptmp = ptmpnext;
			if(ptmp) ptmpnext = ptmp->next;
		}
	
		return -errstat;
	}
	else
	{
		count ++;
		if(count < SNMP_RETRY_TIMES) goto w;

		return -6;
	}
}

int SN_TRAP(int s, struct sockaddr_in serv, struct oid *pobject, char *community, 
			int g_trapid, int s_trapid, char *message)
{
	u_char				request[1000];
	int					i, tmp, count = 0;

	// SNMP Version Information
	u_char	ver[20] = {0};
	int		verlen = 0;
	ver[0] = ASN1_INT;
	verlen = 1 + a1writeint(SNMP_VERSION - 1, ver + 1, ASN1_INT);

	// SNMP Community Information
	u_char	comm[255] = {0};
	int		commlen = 0;
	comm[0] = ASN1_OCTSTR;
	tmp = a1writelen(comm + 1, strlen(community));
	strncpy((char *)(comm + 1 + tmp), community, strlen(community));
	commlen = 1 + tmp + strlen(community);

	// SNMP Enterprise Information
	u_char	object[255] = {0};
	int		objlen = 0;
	object[0] = ASN1_OBJID;
	objlen += 1;
	tmp = a1writeoid(object + 1, pobject);
	objlen += tmp;

	// Agent Address
	char hname[256] = {0};
	struct hostent *hp = NULL;
	char ipaddr[64] = {0};

	strcpy(ipaddr, "0.0.0.0");
	gethostname(hname, sizeof(hname));
	if(hp = gethostbyname(hname))
		strcpy(ipaddr, inet_ntoa(*((struct in_addr *)hp->h_addr)));

	int addr1, addr2, addr3, addr4;
	sscanf(ipaddr, "%d.%d.%d.%d", &addr1, &addr2, &addr3, &addr4);

	u_char addr[10] = {0};
	int addrlen = 0;
	addr[addrlen++] = ASN1_IPADDR;
	addr[addrlen++] = 0x04;
	addr[addrlen++] = addr1;
	addr[addrlen++] = addr2;
	addr[addrlen++] = addr3;
	addr[addrlen++] = addr4;

	// Trap Type
	u_char	type[20] = {0};
	int		typelen = 0;
	type[0] = ASN1_INT;
	typelen = 1 + a1writeint(g_trapid, type + 1, ASN1_INT);

	// Specifie code
	u_char	scode[20] = {0};
	int		scodelen = 0;
	scode[0] = ASN1_INT;
	if(g_trapid == 6)
		scodelen = 1 + a1writeint(s_trapid, scode + 1, ASN1_INT);
	else
		scodelen = 1 + a1writeint(0, scode + 1, ASN1_INT);

	// Time Stamp
	u_char	stamp[20] = {0};
	int		stamplen = 0;
	stamp[0] = ASN1_TIMETICKS;
	stamplen = 1 + a1writeint(0, stamp + 1, ASN1_INT);

	// SNMP Request Object
	u_char	robject[255] = {0};
	int		robjlen = 0;
	robject[0] = ASN1_OBJID;
	robjlen += 1;
	tmp = a1writeoid(robject + 1, &system_oid);
	robjlen += tmp;

	// SNMP Value
	u_char	value[255] = {0};
	int		valuelen = 0;
	value[0] = ASN1_OCTSTR;
	tmp = a1writelen(value + 1, strlen(message));
	strncpy((char *)(value + 1 + tmp), message, strlen(message));
	valuelen = 1 + tmp + strlen(message);

	// SNMP Object-Value
	u_char	objval[255] = {0};
	int		objvallen = 0;
	objval[0] = ASN1_SEQ;
	tmp = a1writelen(objval + 1, robjlen + valuelen);
	objvallen += 1, objvallen += tmp;
	for(i = 0;i < robjlen;i ++)
		objval[objvallen ++] = robject[i];
	for(i = 0;i < valuelen;i ++)
		objval[objvallen ++] = value[i];

	// SNMP Objects
	u_char	objects[255] = {0};
	int		objectslen = 0;
	objects[0] = ASN1_SEQ;
	tmp = a1writelen(objects + 1, objvallen);
	objectslen += 1 + tmp;
	for(i = 0; i < objvallen; i ++)
		objects[objectslen ++] = objval[i];

	// SNMP PDU Type Information
	u_char	pdu[255] = {0};
	int		pdulen = 0;
	pdu[0] = PDU_TRAP;
	tmp = a1writelen(pdu + 1, objlen + addrlen + typelen + scodelen + stamplen + objectslen);
	pdulen += 1, pdulen += tmp;
	for(i = 0;i < objlen;i ++)
		pdu[pdulen ++] = object[i];
	for(i = 0;i < addrlen;i ++)
		pdu[pdulen ++] = addr[i];
	for(i = 0;i < typelen;i ++)
		pdu[pdulen ++] = type[i];
	for(i = 0;i < scodelen;i ++)
		pdu[pdulen ++] = scode[i];
	for(i = 0;i < stamplen;i ++)
		pdu[pdulen ++] = stamp[i];
	for(i = 0;i < objectslen;i ++)
		pdu[pdulen ++] = objects[i];

	// SNMP Request
	int		requestlen = 0;
	request[0] = ASN1_SEQ;
	tmp = a1writelen(request + 1, verlen + commlen + pdulen);
	requestlen += 1, requestlen += tmp;
	for(i = 0;i < verlen;i ++)
		request[requestlen++] = ver[i];
	for(i = 0;i < commlen;i ++)
		request[requestlen++] = comm[i];
	for(i = 0;i < pdulen;i ++)
		request[requestlen ++] = pdu[i];

	int n = sendto(s, (char *)request, requestlen, 0, (struct sockaddr *)&serv, sizeof(serv));
	if(n == SOCKET_ERROR)
		return -1;

	return 0;
}

void SN_PARSE_VALUES(u_char *pack, int palen, int *index, int *field, int *errstatus, 
					 struct oid *poid, struct snval **ppsnval)
{
	int plen = *index + palen;
	int	lenlen = 0, tmp = 0, i = 0;
	struct snval *val = NULL;

	while(*index < plen)
	{
		switch(pack[*index])
		{
		case ASN1_SEQ:
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			SN_PARSE_VALUES(pack, tmp, index, field, errstatus, poid, ppsnval);
			break;
		case PDU_RESP:
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			(*field) ++;
			SN_PARSE_VALUES(pack, tmp, index, field, errstatus, poid, ppsnval);
			break;
		case ASN1_OCTSTR:
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			(*index) += tmp;
			
			switch(*field)
			{
			case 1:
#ifdef __DEBUG__
				printf("SNMP Community: ");
				for(i = 0;i < tmp; i ++)
					printf("%c", *(pack + (*index) - tmp + i));
				printf("\r\n");
#endif
				break;
			case 0:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				break;
			default:
#ifdef __DEBUG__
				if(*field > 6 && *field % 2)
				for(i = 0;i < tmp; i ++)
					printf("%c", *(pack + (*index) - tmp + i));
				printf("\r\n");
#endif
				if(*field > 6 && *field % 2)
				{
					val = (struct snval *)malloc(sizeof(struct snval));
					memset(val, 0, sizeof(struct snval));
					val->sv_type = ASN1_OCTSTR;
					val->sv_val.sv_str.sv_len = tmp;
					for(i = 0;i < tmp; i ++)
						val->sv_val.sv_str.sv_str[i] = *(pack + (*index) - tmp + i);
					val->sv_val.sv_str.sv_str[tmp] = 0;

					AddToResult(ppsnval, val);
				}
				break;
			}
			(*field) ++;
			
			break;
		case ASN1_NULL:
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;

			switch(*field)
			{
			case 1:
#ifdef __DEBUG__
				printf("SNMP Community: %s\r\n", "");
#endif
				break;
			case 0:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				break;
			default:
#ifdef __DEBUG__
				if(*field > 6 && *field % 2)
					printf("%s\r\n", "NULL");
#endif
				if(*field > 6 && *field % 2)
				{
					val = (struct snval *)malloc(sizeof(struct snval));
					memset(val, 0, sizeof(struct snval));
					val->sv_type = ASN1_NULL;
					val->sv_val.sv_str.sv_len = 0;
					memset(val->sv_val.sv_str.sv_str, 0, 1024);

					AddToResult(ppsnval, val);
				}
				break;
			}
			(*field) ++;

			break;
		case ASN1_OBJID:
			struct oid OID;
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			a1readoid(pack + (*index), tmp, &OID);
			(*index) += tmp;

			switch(*field)
			{
			case 6:
				if(poid)
				{
					poid->len = 0;
					for(i = 0;i < OID.len;i ++)
						poid->id[poid->len ++] = *(OID.id + i);
				}
#ifdef __DEBUG__
				for(i = 0;i < OID.len;i ++)
					if(i == 0)
						printf("%d.%d.", 1, 3);
					else if(i == OID.len - 1)
						printf("%ld: ", OID.id[i]);
					else
						printf("%ld.", OID.id[i]);
#endif
				break;
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				break;
			default:
#ifdef __DEBUG__
				if(*field > 7 && !(*field % 2))
					for(i = 0;i < OID.len;i ++)
						if(i == 0)
							printf("%d.%d.", 1, 3);
						else if(i == OID.len - 1)
							printf("%ld: ", OID.id[i]);
						else
							printf("%ld.", OID.id[i]);
#endif
				if(*field > 6 && *field % 2)
				{
					val = (struct snval *)malloc(sizeof(struct snval));
					memset(val, 0, sizeof(struct snval));
					val->sv_type = ASN1_OBJID;
					val->sv_val.sv_oid = OID;

					AddToResult(ppsnval, val);
				}
				break;
			}
			(*field) ++;

			break;
		case ASN1_IPADDR:
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			(*index) += tmp;

			switch(*field)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				break;
			default:
#ifdef __DEBUG__
				if(*field > 6 && *field % 2)
				{
					for(i = 0;i < tmp;i ++)
						if(i == tmp - 1)
							printf("%d\r\n", *(pack + (*index) - tmp + i));
						else
							printf("%d.", *(pack + (*index) - tmp + i));
				}
#endif
				if(*field > 6 && *field % 2)
				{
					val = (struct snval *)malloc(sizeof(struct snval));
					memset(val, 0, sizeof(struct snval));
					val->sv_type = ASN1_IPADDR;
					for(i = 0;i < tmp;i ++)
						val->sv_val.sv_ipaddr[i] = *(pack + (*index) - tmp + i);

					AddToResult(ppsnval, val);
				}
				break;
			}
			(*field) ++;

			break;
		case ASN1_INT:
			int ivalue;
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			ivalue = a1readint(pack + (*index), tmp);
			(*index) += tmp;

			switch(*field)
			{
			case 0:
#ifdef __DEBUG__
				printf("SNMP Version: %d\r\n", ivalue + 1);
#endif
				break;
			case 2:
				break;
			case 3:
#ifdef __DEBUG__
				printf("SNMP Request Identifier: %d\r\n", ivalue);
#endif
				break;
			case 4:
				*errstatus = ivalue;
#ifdef __DEBUG__
				printf("SNMP Error Status: %d\r\n", ivalue);
#endif
				break;
			case 5:
#ifdef __DEBUG__
				printf("SNMP Error Index: %d\r\n", ivalue);
#endif
				break;
			case 1:
			case 6:
				break;
			default:
#ifdef __DEBUG__
				if(*field > 6 && *field % 2)
					printf("%d\r\n", ivalue);
#endif
				if(*field > 6 && *field % 2)
				{
					val = (struct snval *)malloc(sizeof(struct snval));
					memset(val, 0, sizeof(struct snval));
					val->sv_type = ASN1_INT;
					val->sv_val.sv_int = ivalue;

					AddToResult(ppsnval, val);
				}
				break;
			}
			(*field) ++;
			break;
		case ASN1_COUNTER:
		case ASN1_GAUGE:
		case ASN1_TIMETICKS:
		case ASN1_COUNTER64:
		case ASN1_UINTEGER32:
			long lvalue;
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			lvalue = a1readint(pack + (*index), tmp);
			(*index) += tmp;

			switch(*field)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
				break;
			default:
#ifdef __DEBUG__
				if(*field > 6 && *field % 2)
					printf("%ld\r\n", lvalue);
#endif
				if(*field > 6 && *field % 2)
				{
					val = (struct snval *)malloc(sizeof(struct snval));
					memset(val, 0, sizeof(struct snval));
					val->sv_type = *(pack + (*index) - tmp - lenlen - 1);
					val->sv_val.sv_int = lvalue;

					AddToResult(ppsnval, val);
				}
				break;
			}
			(*field) ++;

			break;
		default:
			*errstatus = -9;
			*index = plen;
			(*field) ++;
			break;
		}
	}
}

void AddToResult(struct snval **ppsnval, struct snval *val)
{
	if(!*ppsnval)
		*ppsnval = val;
	else
	{
		struct snval *ptmpsnval = *ppsnval;
		while(ptmpsnval->next)
			ptmpsnval = ptmpsnval->next;

		ptmpsnval->next = val;
	}
}

void SN_PARSE(u_char *pack, int palen, int *index, int *field, int *errstatus, struct oid *poid, struct snval *psnval)
{
	OutputDebugString("===================== SN_PARSE ====================\n");

	int plen = *index + palen;
	int	lenlen = 0, tmp = 0, i = 0;

	while(*index < plen)
	{
		switch(pack[*index])
		{
		case ASN1_SEQ:
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			SN_PARSE(pack, tmp, index, field, errstatus, poid, psnval);
			break;
		case PDU_RESP:
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			(*field) ++;
			SN_PARSE(pack, tmp, index, field, errstatus, poid, psnval);
			break;
		case ASN1_OCTSTR:
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			(*index) += tmp;
			
			switch(*field)
			{
			case 1:
				break;
			case 7:
				if(psnval)
				{
					psnval->sv_type = ASN1_OCTSTR;
					psnval->sv_val.sv_str.sv_len = tmp;
					for(i = 0;i < tmp; i ++)
						psnval->sv_val.sv_str.sv_str[i] = *(pack + (*index) - tmp + i);
					psnval->sv_val.sv_str.sv_str[tmp] = 0;
				}
				break;
			case 0:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			default:
				break;
			}
			(*field) ++;
			
			break;
		case ASN1_NULL:
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;

			switch(*field)
			{
			case 1:
				break;
			case 7:
				if(psnval)
				{
					psnval->sv_type = ASN1_NULL;
					psnval->sv_val.sv_str.sv_len = 0;
					memset(psnval->sv_val.sv_str.sv_str, 0, 1024);
				}
				break;
			case 0:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			default:
				break;
			}
			(*field) ++;

			break;
		case ASN1_OBJID:
			struct oid OID;
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			a1readoid(pack + (*index), tmp, &OID);

			OutputDebugString("pack=");
			OutputDebugString((char*)pack);
			OutputDebugString("\n");

			OutputDebugString("Oid=");

			char strTempOid[1024];
			memset(strTempOid,0,1024);
			for(int k=0;k!=OID.len;k++)
			{
				sprintf(strTempOid,"%s,%d",strTempOid,OID.id[k]);
			}

			OutputDebugString(strTempOid);
			OutputDebugString("\n");





			(*index) += tmp;

			switch(*field)
			{
			case 6:
				if(poid)
				{
					poid->len = 0;
					for(i = 0;i < OID.len;i ++)
						poid->id[poid->len ++] = *(OID.id + i);
				}
				break;
			case 7:
				if(psnval)
				{
					psnval->sv_type = ASN1_OBJID;
					psnval->sv_val.sv_oid = OID;
				}
				break;
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			default:
				break;
			}
			(*field) ++;

			break;
		case ASN1_IPADDR:
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			(*index) += tmp;

			switch(*field)
			{
			case 7:
				if(psnval)
				{
					psnval->sv_type = ASN1_IPADDR;
					for(i = 0;i < tmp;i ++)
						psnval->sv_val.sv_ipaddr[i] = *(pack + (*index) - tmp + i);
				}
				break;
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			default:
				break;
			}
			(*field) ++;

			break;
		case ASN1_INT:
			int ivalue;
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			ivalue = a1readint(pack + (*index), tmp);
			(*index) += tmp;

			switch(*field)
			{
			case 0:
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				*errstatus = ivalue;
				break;
			case 5:
				break;
			case 7:
				if(psnval)
				{
					psnval->sv_type = ASN1_INT;
					psnval->sv_val.sv_int = ivalue;
				}
				break;
			case 1:
			case 6:
			default:
				break;
			}
			(*field) ++;
			break;
		case ASN1_COUNTER:
		case ASN1_GAUGE:
		case ASN1_TIMETICKS:
		case ASN1_COUNTER64:
		case ASN1_UINTEGER32:
			long lvalue;
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			lvalue = a1readint(pack + (*index), tmp);
			(*index) += tmp;

			switch(*field)
			{
			case 7:
				if(psnval)
				{
					psnval->sv_type = *(pack + (*index) - tmp - lenlen - 1);
					psnval->sv_val.sv_int = lvalue;
				}
				break;
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			default:
				break;
			}
			(*field) ++;

			break;
		default:
			*errstatus = -9;
			*index = plen;
			(*field) ++;
			break;
		}


	}

	OutputDebugString("psnval->sv_val=");
	char strTempPoid[1024];
	memset(strTempPoid,0,1024);
	for(int n=0;n!=psnval->sv_val.sv_oid.len;n++)
	{
		sprintf(strTempPoid,"%s,%d",strTempPoid,psnval->sv_val.sv_oid.id[n]);
	}

	OutputDebugString(strTempPoid);
	OutputDebugString("\n");
}

void SN_PARSE_VALUES(u_char *pack, int palen, int *index, int *field, int *errstatus, struct oid *poid, struct snval *psnval)
{
	int plen = *index + palen;
	int	lenlen = 0, tmp = 0, i = 0;

	while(*index < plen)
	{
		switch(pack[*index])
		{
		case ASN1_SEQ:
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			SN_PARSE_VALUES(pack, tmp, index, field, errstatus, poid, psnval);
			break;
		case PDU_RESP:
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			(*field) ++;
			SN_PARSE_VALUES(pack, tmp, index, field, errstatus, poid, psnval);
			break;
		case ASN1_OCTSTR:
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			(*index) += tmp;
			
			switch(*field)
			{
			case 1:
#ifdef __DEBUG__
				printf("SNMP Community: ");
				for(i = 0;i < tmp; i ++)
					printf("%c", *(pack + (*index) - tmp + i));
				printf("\r\n");
#endif
				break;
			case 7:
				if(psnval)
				{
					psnval->sv_type = ASN1_OCTSTR;
					psnval->sv_val.sv_str.sv_len = tmp;
					for(i = 0;i < tmp; i ++)
						psnval->sv_val.sv_str.sv_str[i] = *(pack + (*index) - tmp + i);
					psnval->sv_val.sv_str.sv_str[tmp] = 0;
				}
#ifdef __DEBUG__
				for(i = 0;i < tmp; i ++)
					printf("%c", *(pack + (*index) - tmp + i));
				printf("\r\n");
#endif
				break;
			case 0:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			default:
#ifdef __DEBUG__
				if(*field > 7 && *field % 2)
				for(i = 0;i < tmp; i ++)
					printf("%c", *(pack + (*index) - tmp + i));
				printf("\r\n");
#endif
				break;
			}
			(*field) ++;
			
			break;
		case ASN1_NULL:
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;

			switch(*field)
			{
			case 1:
#ifdef __DEBUG__
				printf("SNMP Community: %s\r\n", "NULL");
#endif
				break;
			case 7:
				if(psnval)
				{
					psnval->sv_type = ASN1_NULL;
					psnval->sv_val.sv_str.sv_len = 0;
					memset(psnval->sv_val.sv_str.sv_str, 0, 1024);
				}
#ifdef __DEBUG__
				if(*field > 7 && *field % 2)
					printf("%s\r\n", "NULL");
#endif
				break;
			case 0:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			default:
#ifdef __DEBUG__
				if(*field > 7 && *field % 2)
					printf("%s\r\n", "NULL");
#endif
				break;
			}
			(*field) ++;

			break;
		case ASN1_OBJID:
			struct oid OID;
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			a1readoid(pack + (*index), tmp, &OID);
			(*index) += tmp;

			switch(*field)
			{
			case 6:
				if(poid)
				{
					poid->len = 0;
					for(i = 0;i < OID.len;i ++)
						poid->id[poid->len ++] = *(OID.id + i);
				}
#ifdef __DEBUG__
				for(i = 0;i < OID.len;i ++)
					if(i == 0)
						printf("%d.%d.", 1, 3);
					else if(i == OID.len - 1)
						printf("%ld: ", OID.id[i]);
					else
						printf("%ld.", OID.id[i]);
#endif
				break;
			case 7:
				if(psnval)
				{
					psnval->sv_type = ASN1_OBJID;
					psnval->sv_val.sv_oid = OID;
				}
				break;
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			default:
#ifdef __DEBUG__
				if(*field > 7 && !(*field % 2))
					for(i = 0;i < OID.len;i ++)
						if(i == 0)
							printf("%d.%d.", 1, 3);
						else if(i == OID.len - 1)
							printf("%ld: ", OID.id[i]);
						else
							printf("%ld.", OID.id[i]);
#endif
				break;
			}
			(*field) ++;

			break;
		case ASN1_IPADDR:
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			(*index) += tmp;

			switch(*field)
			{
			case 7:
				if(psnval)
				{
					psnval->sv_type = ASN1_IPADDR;
					for(i = 0;i < tmp;i ++)
						psnval->sv_val.sv_ipaddr[i] = *(pack + (*index) - tmp + i);
				}
				break;
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			default:
				break;
			}
			(*field) ++;

			break;
		case ASN1_INT:
			int ivalue;
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			ivalue = a1readint(pack + (*index), tmp);
			(*index) += tmp;

			switch(*field)
			{
			case 0:
#ifdef __DEBUG__
				printf("SNMP Version: %d\r\n", ivalue + 1);
#endif
				break;
			case 2:
				break;
			case 3:
#ifdef __DEBUG__
				printf("SNMP Request Identifier: %d\r\n", ivalue);
#endif
				break;
			case 4:
				*errstatus = ivalue;
#ifdef __DEBUG__
				printf("SNMP Error Status: %d\r\n", ivalue);
#endif
				break;
			case 5:
#ifdef __DEBUG__
				printf("SNMP Error Index: %d\r\n", ivalue);
#endif
				break;
			case 7:
				if(psnval)
				{
					psnval->sv_type = ASN1_INT;
					psnval->sv_val.sv_int = ivalue;
#ifdef __DEBUG__
					printf("%d\r\n", ivalue);
#endif
				}
				break;
			case 1:
			case 6:
			default:
#ifdef __DEBUG__
				if(*field > 7 && *field % 2)
					printf("%d\r\n", ivalue);
#endif
				break;
			}
			(*field) ++;
			break;
		case ASN1_COUNTER:
		case ASN1_GAUGE:
		case ASN1_TIMETICKS:
		case ASN1_COUNTER64:
		case ASN1_UINTEGER32:
			long lvalue;
			(*index) ++;
			tmp = a1readlen(pack + (*index), &lenlen);
			(*index) += lenlen;
			lvalue = a1readint(pack + (*index), tmp);
			(*index) += tmp;

			switch(*field)
			{
			case 7:
				if(psnval)
				{
					psnval->sv_type = *(pack + (*index) - tmp - lenlen - 1);
					psnval->sv_val.sv_int = lvalue;
				}
#ifdef __DEBUG__
				printf("%d\r\n", lvalue);
#endif
				break;
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			default:
#ifdef __DEBUG__
				if(*field > 7 && *field % 2)
					printf("%ld\r\n", lvalue);
#endif
				break;
			}
			(*field) ++;

			break;
		default:
			*errstatus = -9;
			*index = plen;
			(*field) ++;
			break;
		}
	}
}

int MakeSocket(char *server, int port, int &s, struct sockaddr_in &serv)
{
	struct hostent *hp;

	s = socket(AF_INET, SOCK_DGRAM, 0);
	if(s == INVALID_SOCKET)
		return -1;

	hp = gethostbyname(server);
	if(hp == NULL)
		return -2;

	memcpy(&serv.sin_addr,hp->h_addr_list[0],hp->h_length);
    serv.sin_family = AF_INET;
    serv.sin_port = htons(port);

	return 0;
}

//-----------------------------------------------------------------
struct oid *VerifyObject(char *object)
{
	int		index = 0;
	struct	oid *poid = NULL;
	CString strObject(object);
	CStringList	lstIDs;

	for(int i = 0;i < strObject.GetLength();i ++)
	{
		TCHAR ch = strObject.GetAt(i);
		if(!isdigit(ch) && ch != '.')
			return NULL;
	}

	while((index = strObject.Find('.', 0)) >= 0)
	{
		if(index > 0)
			lstIDs.AddTail(strObject.Left(index));

		strObject.Delete(0, index + 1);
	}

	if(!strObject.IsEmpty())
		lstIDs.AddTail(strObject);

	if(lstIDs.GetCount() <= 2)
		return NULL;

	POSITION pos = lstIDs.FindIndex(1);
	if(atoi(lstIDs.GetHead()) != 1 || atoi(lstIDs.GetAt(pos)) != 3)
		return NULL;

	poid = (struct oid *)malloc(sizeof(struct oid));
	if(!poid) return NULL;
	memset(poid, 0, sizeof(struct oid));

	poid->id[poid->len ++] = 0x2b;

	pos = lstIDs.FindIndex(2);
	while(pos)
		poid->id[poid->len ++] = atoi(lstIDs.GetNext(pos));

/*
	int Count = 0;
	char *ca, *cb;
	struct oid *poid = NULL;

	ca = object;
	cb = strchr(object, '.');
	if(!ca) return NULL;
	if(!cb) return NULL;
	if(atoi(ca) * 40 + atoi(cb + 1) != 0x2b)
		return poid;

	poid = (struct oid *)malloc(sizeof(struct oid));
	if(!poid) return NULL;
	memset(poid, 0, sizeof(struct oid));

	poid->id[poid->len ++] = 0x2b;

	while(1)
	{
		ca = cb + 1;
		cb = strchr(ca, '.');
		if(cb && isdigit(*(cb + 1)))
			poid->id[poid->len ++] = atoi(cb + 1);
		else
			break;
	}
*/
	struct oid *p_oid = findout_oid(poid);
	if(p_oid)
	{
		sprintf(poid->name, "%s", p_oid->name);
		for(int i = p_oid->len;i < poid->len;i ++)
			sprintf(poid->name, "%s.%d", poid->name, poid->id[i]);
		poid->type = p_oid->type;
	}
	else
	{
		sprintf(poid->name, "%d", poid->id[0]);
		for(int i = 1;i < poid->len;i ++)
			sprintf(poid->name, "%s.%d", poid->name, poid->id[i]);
	}

	return poid;
}

struct oid *findout_oid(struct oid *poid)
{
	int i = 0;

	for(i = 0;i < RFC1213_VARS;i ++)
		if(!compare_oid(*poid, rfc1213_oids[i]))
			return &rfc1213_oids[i];

	return NULL;
}

int compare_oid(struct oid s_oid, struct oid d_oid)
{
	int i = 0;
	for(i = 0;i < d_oid.len;i ++)
		if(s_oid.id[i] != d_oid.id[i])
			return 1;

	return 0;
}
