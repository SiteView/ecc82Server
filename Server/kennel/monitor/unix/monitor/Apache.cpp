
#include "Apache.h"
#include "Utils.h"

#include "base\funcGeneral.h"

BOOL APACHE_MONITOR(char *url, int timeout, char *uid, char *pwd, 
					char *pserver, int pport, char *puid, char *ppwd, 
					char *custpath, char *szReturn)
{
	float	cpuLoad = 0.0, requestsPerSec = 0.0, bytesPerSec = 0.0, bytesPerReq = 0.0,
        totalkb = 0.0;
    int  nAccessCount = 0;
	int		busyServers = 0, idleServers = 0;

	char msg[1024] = {0};
	BOOL bResult = GetUrlData(url, timeout, uid, pwd, pserver, pport, puid, ppwd, 0, msg);
	if(!bResult)
	{
		sprintf(szReturn, "error=%s", msg);
		return FALSE;
	}

	char *ca = NULL;
	char tmp[256] = {0};
	ca = strstr(msg, "CPULoad:");
	if(ca)
	{
		
		sscanf(ca, "%s %f", tmp, &cpuLoad); 
		
	}
	else
	{
		cpuLoad = 0;

		//sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Apache_1%>"));//<%IDS_Apache_1%>
		//return FALSE;
		
	}

    ca = strstr(msg, "Total Accesses:");
    if(ca)
        sscanf(ca + 15, "%d", &nAccessCount);
    else
    {
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_APACHE",
            "APACHE_DATA_TYPE_ERROR"));//<%IDS_Apache_2%>
		return FALSE;
    }

    ca = strstr(msg, "Total kBytes:");
    if(ca)
        sscanf(ca + 15, "%f", &totalkb);
    else
    {
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_APACHE",
            "APACHE_DATA_TYPE_ERROR"));//<%IDS_Apache_2%>
		return FALSE;
    }
	ca = strstr(msg, "ReqPerSec:");
	if(ca) //ReqPerSec
		sscanf(ca, "%s %f", tmp, &requestsPerSec); 
	else
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_APACHE",
            "APACHE_DATA_TYPE_ERROR"));//<%IDS_Apache_2%>
		return FALSE;
	}



	ca = strstr(msg, "BytesPerSec:");
	if(ca)
		sscanf(ca, "%s %f", tmp, &bytesPerSec); 
	else
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_APACHE",
            "APACHE_DATA_TYPE_ERROR"));//<%IDS_Apache_3%>
		return FALSE;
	}

    ca = strstr(msg, "BytesPerReq:");
    if(ca)
        sscanf(ca , "%s %f", tmp, &bytesPerReq);
    else
    {
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_APACHE",
            "APACHE_DATA_TYPE_ERROR"));//<%IDS_Apache_3%>
		return FALSE;
    }


	ca = strstr(msg, "BusyServers:");
	if(ca)
		sscanf(ca, "%s %d", tmp, &busyServers); 
	else
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_APACHE",
            "APACHE_DATA_TYPE_ERROR"));//<%IDS_Apache_4%>
		return FALSE;
	}

	ca = strstr(msg, "IdleServers:");
	if(ca)
		sscanf(ca, "%s %d", tmp, &idleServers); 
	else
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_APACHE",
            "APACHE_DATA_TYPE_ERROR"));//<%IDS_Apache_5%>
		return FALSE;
	}

	sprintf(szReturn, "cpuLoad=%f$accessCount=%d$reqPerSec=%f$bytesPerSec=%f" \
        "$bytesPerReq=%f$busyServers=%d$idleServers=%d$", 
			cpuLoad, nAccessCount, requestsPerSec*60, bytesPerSec, bytesPerReq, busyServers, idleServers);


	return TRUE;
}

BOOL APACHE_2_0_34_MONITOR(char *url, int timeout, char *uid, char *pwd, 
    	 				   char *pserver, int pport, char *puid, char *ppwd, 
						   char *custpath, char *szReturn)
{
	long	totalAccesses = 0, totalKBytes = 0;
	float	reqPerSec = 0.0, bytesPerSec = 0.0, bytesPerReq;
	int		busyWorkers = 0, idleWorkers = 0;

	char msg[1024] = {0};
	BOOL bResult = GetUrlData(url, timeout, uid, pwd, pserver, pport, puid, ppwd, 0, msg);
	if(!bResult)
	{
		sprintf(szReturn, "error=%s", msg);
		return FALSE;
	}

	char *ca = NULL;

	// Total Accesses: 
	if(ca = strstr(msg, "Total Accesses: "))
		totalAccesses = atol(ca + 16); 
	else
		goto Err;

	// Total kBytes
	if(ca = strstr(msg, "Total kBytes: "))
		totalKBytes = atol(ca + 14); 
	else
		goto Err;

	// ReqPerSec
	if(ca = strstr(msg, "ReqPerSec: "))
		reqPerSec = (float)atof(ca + 11); 
	else
		goto Err;

	// BytesPerSec
	if(ca = strstr(msg, "BytesPerSec: "))
		bytesPerSec = (float)atof(ca + 13); 
	else
		goto Err;

	// BytesPerReq
	if(ca = strstr(msg, "BytesPerReq: "))
		bytesPerReq = (float)atof(ca + 13); 
	else
		goto Err;

	// Busy Workers
	if(ca = strstr(msg, "BusyWorkers: "))
		busyWorkers = atoi(ca + 13); 
	else
		goto Err;

	// Idle Workers
	if(ca = strstr(msg, "IdleWorkers: "))
		idleWorkers = atoi(ca + 13); 
	else
		goto Err;

	sprintf(szReturn, "totalAccesses=%ld$totalKBytes=%ld$reqPerSec=%f$bytesPerSec=%f$bytesPerReq=%f$busyWorkers=%d$idleWorkers=%d$", 
			totalAccesses, totalKBytes, reqPerSec*60, bytesPerSec, bytesPerReq, 
			busyWorkers, idleWorkers);

	return TRUE;

Err:
	sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_SOCKET",
        "SOCKET_DATA_TYPE_ERROR"));//<%IDS_Apache_2%>
	return FALSE;
}
