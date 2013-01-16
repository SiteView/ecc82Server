// svapi.cpp : Defines the entry point for the DLL application.
//
#include "svapi.h"
#include "svdbtype.h"

#ifdef WIN32
#include "windows.h"
#endif
//
//#pragma data_seg(".siteviewSvdbHostAddrshare") 
//char svdbHostAddr[128]={0};
//#pragma data_seg() 
//
//#pragma comment(linker,"/section:.siteviewSvdbHostAddrshare,rws")  


#ifdef	WIN32

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif




