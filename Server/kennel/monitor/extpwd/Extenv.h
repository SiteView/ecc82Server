/*
 *  extenv.h - Environment header file for Extension Manager password
 *             program.
 *
 *  This file declares the platform-specific functions.
 */

#ifndef _EXTENV_H_
#define _EXTENV_H_

/*
 *  Function Declarations
 */

#if defined (NT)

    /* Win32 DLL Startup/Shutdown routine */
	DLL_EXPORT_PREFIX BOOL WINAPI DllMain (
		HANDLE hInst,
		ULONG  whyCalled,
		LPVOID lpReserved
	);

#elif defined (DOSW16)

    /* Win16 DLL startup */
	int CALLBACK DllMain (
		HINSTANCE  hInst,
		WORD       wDataSeg,
		WORD       cbHeapSize,
		LPSTR      lpszCmdLine
	);

    /* Win16 DLL shutdown */
	int CALLBACK WEP (
		int nExitType
	);

#elif defined (OS2_2x)

    /* OS/2 DLL Startup/Shutdown routine */
	ULONG DLL_EXPORT_INFIX _DLL_InitTerm (
        HMODULE hModule,
		ULONG   dwFlag
	);

#endif

#endif
