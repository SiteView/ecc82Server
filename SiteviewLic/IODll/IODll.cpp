// IODll.cpp : ���� DLL Ӧ�ó������ڵ㡣
//
#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����
// Windows ͷ�ļ�:
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

