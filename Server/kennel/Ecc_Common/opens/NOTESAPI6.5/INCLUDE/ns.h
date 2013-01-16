#ifdef __cplusplus
extern "C" {
#endif


/*  Name Server Public Definitions */

#ifndef NS_DEFS
#define NS_DEFS


/* function templates */

STATUS LNPUBLIC NSGetServerList (char far *pPortName, HANDLE far *retServerTextList);

/* Begin APIs for Cluster support */

#define	CLUSTER_LOOKUP_NOCACHE				0x00000001	/* don't use cluster name cache */
#define	CLUSTER_LOOKUP_CACHEONLY			0x00000002	/* only use cluster name cache */
STATUS LNPUBLIC NSGetServerClusterMates ( 
	const char far *pServerName,
    DWORD dwFlags,
	HANDLE far *phList);
STATUS LNPUBLIC NSPingServer( 
	char far *pServerName,
	DWORD far *pdwIndex,
	HANDLE far *phList);

#endif



#ifdef __cplusplus
}
#endif
