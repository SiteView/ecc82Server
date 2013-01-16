
#include "Ping.h"
#include "Utils.h"

#include "..\..\base\funcGeneral.h"

BOOL PING_MONITOR(char *server, long msec, int bytesize, BOOL bVipPing, char *custpath, char *szReturn)
{
 	SOCKET	  rawSocket;
 	struct    sockaddr_in saDest;
 	struct    sockaddr_in saSrc;
 	LPHOSTENT lpHost;
 	int		  i, nRet, nBytes, nSeq;
 	long	  dwTimeSent, m_laddr;
 	u_char    cTTL;

	struct rTime{
		int time;
		int flag;
	} rtime[PACKET_COUNT];

	for(i = 0;i < PACKET_COUNT;i ++)
	{
		rtime[i].time = 0;
		rtime[i].flag = 0;
	}

	rawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if(rawSocket == INVALID_SOCKET)
	{
		// Create Socket Error
		sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Ping_1%>"));//<%IDS_Ping_1%>
		return FALSE;
	}

	m_laddr = inet_addr(server);
	if (m_laddr == INADDR_NONE)
	{
		lpHost = gethostbyname(server);
		if (lpHost == NULL)
 		{
			// DNS Resolve Error
			sprintf(szReturn, "error=%s", ::FuncGetStringFromIDS("<%IDS_Ping_2%>"));//<%IDS_Ping_2%>
			return FALSE;
		}
		memcpy(&(saDest.sin_addr),lpHost->h_addr,lpHost->h_length);
		saDest.sin_family = lpHost->h_addrtype;
	}
	else
	{
		saDest.sin_addr.s_addr =m_laddr;
		saDest.sin_family = AF_INET;
	}

	if(bVipPing)
	{
		char szStatus[256] = {0};
		char szVipFile[256] = {0};
		char *addr = inet_ntoa(saDest.sin_addr);

		sprintf(szVipFile, "%s\\groups\\vip.ini", FuncGetInstallPath());
		GetPrivateProfileString(addr, "status", "", szStatus, sizeof(szStatus), szVipFile);
		if(!strncmp(szStatus, "off", 3))
		{
			// Host Has Shutdown
			sprintf(szReturn, "pre-status=4$error=%s", ::FuncGetStringFromIDS("<%IDS_Ping_3%>"));//<%IDS_Ping_3%>
			return FALSE;
		}
	}

 	saDest.sin_port = 0;

	for(i = 0;i < PACKET_COUNT;i ++)
	{
		int Count = 0;
		nRet = SendEchoRequest(rawSocket, &saDest, bytesize, i);
w: 		nRet = WaitForEchoReply(rawSocket, msec);
		if(nRet <= 0)
		{
			/*
			sprintf(szReturn, "status=%d$", 300);
			return FALSE;
			*/
			continue;
		}
		if (nRet > 0)
 		{
 			// Receive reply
			u_char	ictype = 0, 
					iccode = 0;
 			dwTimeSent = RecvEchoReply(rawSocket, &saSrc, &cTTL, &nBytes, &nSeq, &ictype, &iccode);
			if(dwTimeSent == -1 || nSeq != i)
			{
				Count ++;
				if(Count < 3) goto w; else continue;
			}

			// Calculate elapsed time
			rtime[i].time = GetTickCount() - dwTimeSent;
			rtime[i].flag = 1;
 		}
	}

	long	total = 0, num = 0;
	for(i = 0;i < PACKET_COUNT;i ++)
	{
		if(rtime[i].flag)
		{
			total += rtime[i].time;
			num ++;
		}
	}

	if(num == 0)
		sprintf(szReturn, "status=%d$", 300);
	else
		sprintf(szReturn, "status=%d$roundTripTime=%.0f$packetsGoodPercent=%.2f$", 
				200, (float)total / num, (float)num / PACKET_COUNT * 100);

	return TRUE;
}

int SendEchoRequest(SOCKET s, LPSOCKADDR_IN lpstToAddr, int reqSize, int nSeq)
{
	static ECHOREQUEST echoReq;
	static nId = 1;
	int nRet;

	// Fill in echo request
	echoReq.icmpHdr.Type		= ICMP_ECHOREQ;
	echoReq.icmpHdr.Code		= 0;
	echoReq.icmpHdr.Checksum	= 0;
	echoReq.icmpHdr.ID			= (u_short)GetCurrentProcessId();
	echoReq.icmpHdr.Seq			= nSeq;

	// Fill in some data to send
	for (nRet = 0; nRet < reqSize; nRet++)
		echoReq.cData[nRet] = ' '+nRet;

	// Save tick count when sent
	echoReq.dwTime				= GetTickCount();

	// Put data in packet and compute checksum
	echoReq.icmpHdr.Checksum = in_cksum((u_short *)&echoReq, sizeof(ECHOREQUEST));

	// Send the echo request  								  
	nRet = sendto(s,						/* socket */
				 (LPSTR)&echoReq,			/* buffer */
				 sizeof(ECHOREQUEST),
				 0,							/* flags */
				 (LPSOCKADDR)lpstToAddr, /* destination */
				 sizeof(SOCKADDR_IN));   /* address length */

	return (nRet);
}

int WaitForEchoReply(SOCKET s, long msec)
{
	long	lnsec = msec / 1000;
	long	lmsec = msec % 1000;

	struct timeval Timeout;
	fd_set readfds;

	readfds.fd_count = 1;
	readfds.fd_array[0] = s;
	Timeout.tv_sec = lnsec;
    Timeout.tv_usec = lmsec;
	return(select(1, &readfds, NULL, NULL, &Timeout));
}

long RecvEchoReply(SOCKET s, LPSOCKADDR_IN lpsaFrom, u_char *pTTL, int *dataLen, int *nSeq, u_char *ictype, u_char *iccode)
{
	ECHOREPLY echoReply;
	int nRet;
	int nAddrLen = sizeof(struct sockaddr_in);

	// Receive the echo reply	
	nRet = recvfrom(s,					// socket
					(LPSTR)&echoReply,	// buffer
					sizeof(ECHOREPLY),	// size of buffer
					0,					// flags
					(LPSOCKADDR)lpsaFrom,	// From address
					&nAddrLen);			// pointer to address len

	// Check return value
	if(echoReply.echoRequest.icmpHdr.ID != (u_short)GetCurrentProcessId())
		return -1;
	// return time sent and IP TTL
	*ictype = echoReply.echoRequest.icmpHdr.Type;
	*iccode = echoReply.echoRequest.icmpHdr.Code;
	*pTTL = echoReply.ipHdr.TTL;
	*dataLen = strlen(echoReply.echoRequest.cData);
	*nSeq = echoReply.echoRequest.icmpHdr.Seq;

	return(echoReply.echoRequest.dwTime);   		
}

u_short in_cksum(u_short *addr, int len)
{
	register int nleft = len;
	register u_short *w = addr;
	register u_short answer;
	register int sum = 0;

	/*
	 *  Our algorithm is simple, using a 32 bit accumulator (sum),
	 *  we add sequential 16 bit words to it, and at the end, fold
	 *  back all the carry bits from the top 16 bits into the lower
	 *  16 bits.
	 */
	while( nleft > 1 )  {
		sum += *w++;
		nleft -= 2;
	}

	/* mop up an odd byte, if necessary */
	if( nleft == 1 ) {
		u_short	u = 0;

		*(u_char *)(&u) = *(u_char *)w ;
		sum += u;
	}

	/*
	 * add back carry outs from top 16 bits to low 16 bits
	 */
	sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
	sum += (sum >> 16);			/* add carry */
	answer = ~sum;				/* truncate to 16 bits */
	return (answer);
}