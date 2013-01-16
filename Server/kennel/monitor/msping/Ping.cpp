
#include "Ping.h"
#include <iostream>
using namespace std;
#include <base\funcGeneral.h>

extern int PrintLog(const char * strInfo);

BOOL PING_MONITOR(char *server, long msec, int bytesize, int nSendNums,BOOL bVipPing, char *custpath,
				  char *szReturn,int& nSize)
{
		char strTemp[1024]={0};


    
    // Load the ICMP.DLL
    HINSTANCE hIcmp = LoadLibrary("ICMP.DLL");
    if (hIcmp == 0) {
		PrintLog("装载ICMP.dll失败！");
        sprintf(szReturn,"error=%s",FuncGetStringFromIDS("SV_BASIC",
            "BASIC_LOAD_DLL_FAILED"));
		PrintLog(szReturn);
        return FALSE;
    }

    // Look up an IP address for the given host name
    struct hostent* phe;
    if ((phe = gethostbyname(server)) == 0) {		
		PrintLog("获取主机信息");

		sprintf(szReturn,"error=%s",FuncGetStringFromIDS("SV_BASIC",
            "BASIC_PARSER_SERVER_FAILED"));
		PrintLog(szReturn);
        
        return FALSE;
    }

    // Get handles to the functions inside ICMP.DLL that we'll need
    typedef HANDLE (WINAPI* pfnHV)(VOID);
    typedef BOOL (WINAPI* pfnBH)(HANDLE);
    typedef DWORD (WINAPI* pfnDHDPWPipPDD)(HANDLE, DWORD, LPVOID, WORD,
            PIP_OPTION_INFORMATION, LPVOID, DWORD, DWORD); // evil, no?
    pfnHV pIcmpCreateFile;
    pfnBH pIcmpCloseHandle;
    pfnDHDPWPipPDD pIcmpSendEcho;
    pIcmpCreateFile = (pfnHV)GetProcAddress(hIcmp,
            "IcmpCreateFile");
    pIcmpCloseHandle = (pfnBH)GetProcAddress(hIcmp,
            "IcmpCloseHandle");
    pIcmpSendEcho = (pfnDHDPWPipPDD)GetProcAddress(hIcmp,
            "IcmpSendEcho");
    if ((pIcmpCreateFile == 0) || (pIcmpCloseHandle == 0) || 
            (pIcmpSendEcho == 0)) {

		sprintf(szReturn,"error=%s",FuncGetStringFromIDS("SV_BASIC",
            "BASIC_GET_PROC_ADDRESS_FAILED"));
		PrintLog(szReturn);
        
        return 4;
    }

    // Open the ping service
    HANDLE hIP = pIcmpCreateFile();
    if (hIP == INVALID_HANDLE_VALUE) {
		sprintf(szReturn,"error=%s",FuncGetStringFromIDS("SV_PING",
            "PING_CREATE_ICMP_FILE_FAILED"));
        PrintLog(szReturn);
        return FALSE;
    }
   
    // Build ping packet
//    char acPingBuffer[64];
	char * acPingBuffer = new char[bytesize];
    memset(acPingBuffer, 'A', bytesize);//sizeof(acPingBuffer)); //\xAA
    PIP_ECHO_REPLY pIpe = (PIP_ECHO_REPLY)GlobalAlloc(
            GMEM_FIXED | GMEM_ZEROINIT,
            sizeof(IP_ECHO_REPLY) + bytesize);//sizeof(acPingBuffer));
    if (pIpe == 0) {
		sprintf(szReturn,"error=%s",FuncGetStringFromIDS("SV_BASIC",
            "BASIC_GOLBAL_ALLOC_FAILED"));

		::CloseHandle(hIP);
         PrintLog(szReturn);
		 delete [] acPingBuffer;
        return FALSE;
    }
    pIpe->Data = acPingBuffer;
    pIpe->DataSize = bytesize;//sizeof(acPingBuffer);      

    // Send the ping packet
	//CTime a=CTime::GetCurrentTime();
	//printf("%d:%d",a.GetMinute(),a.GetSecond());

	int nRTT=0;
	int nSuc =0;

	if(nSendNums<=0)
		nSendNums = 1;

	for(int i=0;i<nSendNums;i++)
	{
        
		DWORD dwStatus = pIcmpSendEcho(hIP, *((DWORD*)phe->h_addr_list[0]), 
				acPingBuffer, bytesize, NULL, pIpe,//sizeof(acPingBuffer), NULL, pIpe, 
				sizeof(IP_ECHO_REPLY) + bytesize,msec);//1000);//sizeof(acPingBuffer), 1000);
		cout<<"bytesize="<<bytesize<<endl;
        
		if (pIpe->Status == 0) 
        {
			nSuc++;
			nRTT+=	pIpe->RoundTripTime	;
		}
	}

//	char strTemp[1024]={0};
//	sprintf(strTemp,"nSuc=%d",nSuc);
//	PrintLog(strTemp);

	if(nSuc == 0)
		sprintf(szReturn, "status=%d$roundTripTime=%.0f$packetsGoodPercent=%.2f$", 300,0,0);
	else
		sprintf(szReturn, "status=%d$roundTripTime=%.2f$packetsGoodPercent=%.2f$", 
				200, (float)nRTT / (float)nSendNums, (float)nSuc / nSendNums * 100);

	CString strInput ;
	strInput =szReturn;
	MakeCharByString(szReturn,nSize,strInput);
		

    // Shut down...
	::CloseHandle(hIP);
    GlobalFree(pIpe);
    FreeLibrary(hIcmp);

	delete [] acPingBuffer;
    return TRUE;
}


