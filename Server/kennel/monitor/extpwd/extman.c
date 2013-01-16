//===========================================================================
//
//  Module:
//		EXTMAN.C - DLL framework for extension manager password override.
//
//  Description:
//     The purpose of this sample is to illustrate the work of the
//     extension manager.  Note that this sample is a modified
//     version of the CAPI EXTPWD Sample which uses dialog box to
//     obtain Notes password instead of reading it from a text file.
//
//     Syntax: See Readme.txt for more information.
//
//===========================================================================

/* Notes header files */

#include <global.h>
#include <extmgr.h>
#include <bsafeerr.h>

/* Sample program header files */

#include "extpwd.h"
#include "extenv.h"

/* DLL MANAGEMENT */

#if defined (_WIN32)

/* Windows header files */

#include <windows.h>

HINSTANCE	hDllInstance = (HINSTANCE) 0;

/* Win32 DLL Startup/Shutdown routine  */

DLL_EXPORT_PREFIX BOOL WINAPI DllMain (HANDLE hInst, ULONG dwReason,
									   LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		/* Save the instance handle */
		hDllInstance = hInst;
		break;
	
	case DLL_PROCESS_DETACH:
		/* Deregister extension manager callbacks */
		ExtClear ();
		break;
	}
	
	return (TRUE);
}


#elif defined (OS2_2x)

/* OS2 header files */
#include <os2.h>

/* Local data */

HMODULE	hDllInstance = (HMODULE) 0;

/* C Run-time startup/shutdown routines */

int _CRT_init (void);
void _CRT_term (void);

/* OS2 DLL Startup/Shutdown routine */

#pragma linkage (_DLL_InitTerm, system)

ULONG DLL_EXPORT_INFIX _DLL_InitTerm (HMODULE hModule, ULONG dwFlag)
{
	switch (dwFlag)
	{
	
	case 0:
		/* Process init.  Initialize run-time library. */
		if (-1 == _CRT_init ())
			return ((ULONG) 0);

		/* Save the instance handle */
		hDllInstance = hModule;
		break;

	case 1:
		/* Process Term. Deregister extension manager callbacks */
		ExtClear ();
		
		/* Shut down run-time library. */
		_CRT_term ();
		break;
	}

	return ((ULONG) 1);
}

#endif


