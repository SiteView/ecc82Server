#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>


#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#include <Iphlpapi.h>
#include <stdio.h>
#include <Iptypes.h>
#pragma comment(lib,"Iphlpapi.lib")

#else	
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if.h>
#endif



long mac_addr_sys ( unsigned char *addr)
{
/* implementation for Linux */
#ifdef WIN32
	char szMac[20];
	PIP_ADAPTER_INFO pAdapterInfo;
	PIP_ADAPTER_INFO pAdapter = NULL;
	DWORD dwRetVal = 0;
	unsigned long ulOutBufLen;

	pAdapterInfo = (IP_ADAPTER_INFO *) malloc( sizeof(IP_ADAPTER_INFO) );
	ulOutBufLen = sizeof(IP_ADAPTER_INFO);

	addr[0] = 0x0;
	if (GetAdaptersInfo( pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO *) malloc (ulOutBufLen); 
	}

	if ((dwRetVal = GetAdaptersInfo( pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		pAdapter = pAdapterInfo;
		while (pAdapter) {
			memset(szMac, 0, 20);
			sprintf(szMac, "%02X-%02X-%02X-%02X-%02X-%02X;", 
				pAdapter->Address[0], 
				pAdapter->Address[1],
				pAdapter->Address[2],
				pAdapter->Address[3],
				pAdapter->Address[4],
				pAdapter->Address[5]);
			strcat(addr, szMac);
			pAdapter = pAdapter->Next;
		}
	}
	else {
		return 0;
	}
#else 
    struct ifreq ifr;
    struct ifreq *IFR;
    struct ifconf ifc;
    unsigned int uNICCount = 0;
    char buf[1024];
    char szbuff[16];
    struct sockaddr_in *pAddr;
    int s, i,j;
    int ok = 0;

    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s==-1) {
        return 0;
    }
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;
    ioctl(s, SIOCGIFCONF, &ifc);
    uNICCount = ifc.ifc_len / sizeof(struct ifreq);
   // printf("NIC total is : %d\n", uNICCount);
    
    IFR = ifc.ifc_req;
    strcpy(addr,"");	
    for (i = 0;i < uNICCount; i++,IFR++)
    {	
        strcpy(ifr.ifr_name, IFR->ifr_name);
	//printf( "%d NIC %s ",i,IFR->ifr_name);	
	if (ioctl(s, SIOCGIFFLAGS, &ifr) == 0)
	{
            if (! (ifr.ifr_flags & IFF_LOOPBACK)) 
	    {
		if (ioctl(s, SIOCGIFHWADDR, &ifr) == 0) 
		{    			
			sprintf(szbuff,"%.2X-%.2X-%.2X-%.2X-%.2X-%.2X;",
			(unsigned char)ifr.ifr_hwaddr.sa_data[0],
			(unsigned char)ifr.ifr_hwaddr.sa_data[1],
			(unsigned char)ifr.ifr_hwaddr.sa_data[2],
			(unsigned char)ifr.ifr_hwaddr.sa_data[3],
			(unsigned char)ifr.ifr_hwaddr.sa_data[4],
			(unsigned char)ifr.ifr_hwaddr.sa_data[5]);	
                        strcat(addr,szbuff);			
			ok = 1;			 
		}
		else
		{
			close(s);
			return 0;
		}
	    }
	}	
    }
    close(s);  
    if (ok==0) return 0;

#endif
/* Not implemented platforms */
    return 1;
}

/***********************************************************************/
/*
 * Main (only for testing)
 */
#ifdef MAIN
int main( int argc, char **argv)
{
    long stat;
    int i;
    u_char addrs[124];

    stat = mac_addr_sys( addrs);
     printf("%s\n", addrs);
    //~ if (0 == stat) {
        //~ printf( "MAC address = ");
        //~ for (i=0; i<6; ++i) {
            //~ printf("%2.2x", addr[i]);
        //~ }
        //~ printf( "\n");
    //~ }
    //~ else {
        //~ fprintf( stderr, "can't get MAC address\n");
        //~ exit( 1);
    //~ }
    return 0;
}
#endif
