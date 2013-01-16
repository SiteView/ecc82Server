// IODll.cpp : 定义 DLL 应用程序的入口点。
//
#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>

#pragma comment(lib,"ssleay32.lib")
#pragma comment(lib,"libeay32.lib")

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

