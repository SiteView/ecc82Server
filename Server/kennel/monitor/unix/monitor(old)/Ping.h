
#include "stdafx.h"

#define		REQ_DATASIZE				256	// Echo Request Data size
#define		PACKET_COUNT				4
#define		ICMP_ECHOREQ				8

// IP Header -- RFC 791
typedef struct tagIPHDR
{
	u_char  VIHL;			// Version and IHL
	u_char	TOS;			// Type Of Service
	short	TotLen;			// Total Length
	short	ID;				// Identification
	short	FlagOff;		// Flags and Fragment Offset
	u_char	TTL;			// Time To Live
	u_char	Protocol;		// Protocol
	u_short	Checksum;		// Checksum
	struct	in_addr iaSrc;	// Internet Address - Source
	struct	in_addr iaDst;	// Internet Address - Destination
} IPHDR, *PIPHDR;

// ICMP Header - RFC 792
typedef struct tagICMPHDR
{
	u_char	Type;			// Type
	u_char	Code;			// Code
	u_short	Checksum;		// Checksum
	u_short	ID;				// Identification
	u_short	Seq;			// Sequence
	char	Data;			// Data
} ICMPHDR, *PICMPHDR;

// ICMP Echo Request
typedef struct tagECHOREQUEST
{
	ICMPHDR icmpHdr;
	DWORD	dwTime;
	char	cData[REQ_DATASIZE];
} ECHOREQUEST, *PECHOREQUEST;

// ICMP Echo Reply
typedef struct tagECHOREPLY
{
	IPHDR	ipHdr;
	ECHOREQUEST	echoRequest;
	char    cFiller[256];
} ECHOREPLY, *PECHOREPLY;


BOOL PING_MONITOR(char *server, long msec, int bytesize, BOOL bVipPing, char *custpath, char *szReturn);
int SendEchoRequest(SOCKET s, LPSOCKADDR_IN lpstToAddr, int reqSize, int nSeq);
u_short in_cksum(u_short *addr, int len);
int WaitForEchoReply(SOCKET s, long msec);
long RecvEchoReply(SOCKET s, LPSOCKADDR_IN lpsaFrom, u_char *pTTL, int *dataLen, 
				   int *nSeq, u_char *ictype, u_char *iccode);
