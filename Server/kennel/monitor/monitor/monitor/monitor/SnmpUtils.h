#ifndef __SNMPUTILS_H__
#define	__SNMPUTILS_H__

#include "stdafx.h"

typedef		unsigned int		Counter;
typedef		unsigned int		Gauge;
typedef		unsigned int		TimeTicks;

#define		COUNTER_MAX			4294967296

/* ASN.1 object types */
#define		ASN1_SEQ			0x30		/* sequence object			*/
#define		ASN1_INT			0x02		/* integer					*/
#define		ASN1_BITSTR			0x03		/* bit string				*/
#define		ASN1_OCTSTR			0x04		/* octet string				*/
#define		ASN1_NULL			0x05		/* null						*/
#define		ASN1_OBJID			0x06		/* object identifier		*/
#define		DFF1_PHYSADDR		0xA0		/* Sub Data Type of OCTSTR	*/
#define		DFF1_IFTYPE			0xA1		/* Sub Data Type of INT		*/
#define		DFF1_ADMINSTATUS	0xA2		/* ...						*/
#define		DFF1_OPERSTATUS		0xA3		/* ...						*/
#define		DFF1_IPFORWARDING	0xA4		/* ...						*/
#define		DFF1_IPROUTETYPE	0xA5		/* ...						*/
#define		DFF1_IPROUTEPROTO	0xA6		/* ...						*/
#define		DFF1_NETTOMEDIATYPE	0xA7		/* ...						*/
#define		DFF1_TCPRTOALGORITH	0xA8		/* ...						*/
#define		DFF1_TCPCONNSTATE	0xA9		/* ...						*/
#define		DFF1_EGPNEIGHSTATE	0xAA		/* ...						*/
#define		DFF1_EGPNEIGHMODE	0xAB		/* ...						*/
#define		DFF1_EGPNEIGHEVENTT	0xAC		/* ...						*/
#define		DFF1_SNMPENABLETRAP	0xAD		/* ...						*/
#define		ASN1_IPADDR			0x40		/* ip address				*/
#define		ASN1_COUNTER		0x41		/* counter					*/
#define		ASN1_GAUGE			0x42		/* gauge					*/
#define		ASN1_TIMETICKS		0x43		/* time ticks				*/

#define		ASN1_OPAQUE			0x44
#define		ASN1_NSAPADDRESS	0x45
#define		ASN1_COUNTER64		0x46
#define		ASN1_UINTEGER32		0x47

/* Protocol Data Unit types -- SNMP specific */
#define		PDU_GET				0xA0		/* get request				*/
#define		PDU_GETN			0xA1		/* get-next request			*/
#define		PDU_RESP			0xA2		/* response					*/
#define		PDU_SET				0xA3		/* set request				*/
#define		PDU_TRAP			0xA4		/* trap message				*/
#define		PDU_GETBULK			0xA5		/* get bulk request(SNMPv2)	*/
#define		PDU_INFORM			0xA6		/* inform request(SNMPv2)	*/
#define		PDU_TRAP_V2			0xA7		/* trap message(SNMPv2)		*/

/* Constants used for conversion of objects to/from ASN.1 notation */
#define		CHAR_BITS			8			/* number of bits per char	*/
#define		CHAR_HIBIT			0x80		/* octet with the high bit set */
#define		BYTE2_HIBIT			0x8000		/* 2 bytes with high bit set */
#define		BYTE3_HIBIT			0x800000	/* 3 bytes with high bit set */
#define		BYTE4_HIBIT			0x80000000	/* 4 bytes with high bit set */

#define		A1_SIGNED(x)		((x) == ASN1_INT)

#define		SNMP_RETRY_TIMES	3
#define		SNMP_WAIT_TIMEOUT	10
#define		SNMP_VERSION		1
#define		SMAXOBJID			256

struct oid
{
	unsigned int	id[SMAXOBJID];
	int				len;
	char			name[64];
	int				type;
	char			descr[1];

	struct oid		*next;
};

struct snval
{
	unsigned char	sv_type;
	union {
		long sv_int;
		struct {
			char sv_str[1024];
			int	sv_len;
		} sv_str;
		struct oid	sv_oid;
		char sv_ipaddr[64];
	} sv_val;

	struct snval *next;
};

/* Interfaces Group */
enum eifGroup{
	IFINDEX = 0, 
	IFDESCR, 
	IFTYPE,
	IFMTU,
	IFSPEED,
	IFPHYSADDRESS,
	IFADMINSTATUS,
	IFOPERSTATUS,
	IFLASTCHANGE,
	IFINOCTETS,
	IFINUCASTPKTS,
	IFINNUCASTPKTS,
	IFINDISCARDS,
	IFINERRORS,
	IFINUNKNOWNPROTOS,
	IFOUTOCTETS,
	IFOUTUCASTPKTS,
	IFOUTNUCASTPKTS,
	IFOUTDISCARDS,
	IFOUTERRORS,
	IFOUTQLEN,
	IFSPECIFIC
};

int a1writeoid(unsigned char *packp, struct oid *oidp);
int a1writelen(u_char *pp, int len);
int a1writeint(int val, u_char *buffp, int a1type);
int a1readlen(unsigned char *pack, int *lenlen);
int a1readoid(unsigned char *pack, int objidlen, struct oid *objid);
int a1readint(u_char *pack, int len);

int MakeSocket(char *server, int port, int &s, struct sockaddr_in &serv);
void AddToResult(struct snval **ppsnval, struct snval *val);

int SN_VALUE(int s, struct	sockaddr_in serv, char *community, int request_id, struct oid *oidp, struct snval *psnval);
int SN_NEXT_VALUE(int s, struct sockaddr_in serv, char *community, int request_id, struct oid *oidp, struct oid *oidr, struct snval *psnval);
int SN_VALUES(int s, struct sockaddr_in serv, char *community, int request_id, struct oid *oidp, struct snval **ppsnval);
int SN_SET_VALUES(int s, struct sockaddr_in serv, char *community, int request_id, struct oid *oidp, struct snval *psnval);
int SN_TRAP(int s, struct sockaddr_in serv, struct oid *pobject, char *community, 
			int g_trapid, int s_trapid, char *message);

void SN_PARSE(u_char *pack, int palen, int *index, int *field, int *errstatus, struct oid *poid, struct snval *psnval);
void SN_PARSE_VALUES(u_char *pack, int palen, int *index, int *field, int *errstatus, struct oid *poid, struct snval **ppsnval);

struct oid *VerifyObject(char *object);
struct oid *findout_oid(struct oid *poid);
int compare_oid(struct oid s_oid, struct oid d_oid);

#endif