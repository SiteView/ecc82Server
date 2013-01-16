//===========================================================================
//
//	Module:
//		EXTPWD.C - Source for extension manager password override program
//
//	Description:
//		The purpose of this sample is to illustrate the work of the
//		extension manager.  Note that this sample is a modified
//		version of the CAPI EXTPWD Sample which uses dialog box to
//		obtain Notes password instead of reading it from a text file.
//
//	Syntax:
//		See Readme.txt for more information.
//
//===========================================================================

/* "C" header files */
#include <stdio.h>
#include <string.h>

/* Notes API header files */
#include <global.h>
#include <extmgr.h>
#include <bsafeerr.h>
#include <nsferr.h>
#include <names.h>
#include <osfile.h>

/* Platform header files */
#if defined (NT)
#include <windows.h>
#elif defined (OS2_2x)
#include <stdio.h>
#include <os2.h>
#endif

/* Program header files */
#include "extpwd.h"

/*
 * Global Data
 */

HEMREGISTRATION hHandler = 0;

/*
 *  Architecture
 *  ------------
 *
 *  An Extension Manager DLL contains the following major components:
 *
 *    1)  DLL startup and shutdown (in platform DLL file, e.g., extwin.c):
 *        Under Win32: the -DllEntryPoint function, here DllMain
 *        Under Win16: LibMain & WEP
 *
 *    2)  Notes entry point at ordinal 1:  MainEntryPoint
 *
 *    3)  Extension manager callback:  ExtHandler
 */

/* Extension manager entry point */
DLL_EXPORT_PREFIX STATUS LNPUBLIC DLL_EXPORT_INFIX MainEntryPoint( void )
{
STATUS  status;
char    msgBuf [256];

	status = EMRegister(EM_GETPASSWORD, EM_REG_BEFORE, ExtHandler, 0, &hHandler);

	if (NOERROR != status)
	{
#if defined (NT)
  wsprintf (msgBuf, "Could not register extension handler - status: 0x%lX",status);
#else
  sprintf (msgBuf, "Could not register extension handler - status: 0x%lX",status);
#endif
	}

	return (status);
}

/* Deregister extensions */
DLL_EXPORT_PREFIX STATUS LNPUBLIC DLL_EXPORT_INFIX ExtClear (
	void
) {
	STATUS  status;

	if (0 != hHandler)
		status = EMDeregister (hHandler);
	else
		status = NOERROR;

	return (status);
}

/* Master callback handler */
STATUS LNCALLBACK ExtHandler (
	EMRECORD far *	pRecord
) {
	VARARG_PTR  pArgs;

    /* Function arguments */
	DWORD         MaxPwdLen;
	DWORD far *   retLength;
	char far *    retPassword;


	if (EM_GETPASSWORD != pRecord->EId)
		return (ERR_EM_CONTINUE);

    /* Fetch the arguments */
	        pArgs = pRecord->Ap;
		MaxPwdLen = VARARG_GET (pArgs, DWORD);
        retLength = VARARG_GET (pArgs, DWORD far *);
      retPassword = VARARG_GET (pArgs, char far *);


		/* Use the current password */
	if ((NULL != retLength)
		&& (NULL != retPassword))
	{		
		FILE  *fptr;
		int   len;
		char  line[MAX_ID_PASSWORD], DataPath[MAXPATH];


        /* Get the local Notes Data directory */

		len = OSGetDataDirectory(DataPath);


#if defined(DOS) || defined(OS2) || defined(NT)  
   strcat(DataPath, "\\");
#elif defined(UNIX)
  strcat(DataPath, "/");
#endif
  strcat (DataPath, "password.txt");

  /* Open the password file from the local Notes Data directory */

  if ((fptr = fopen(DataPath, "r"))==NULL)
	  return (ERR_EM_CONTINUE);
  else
  {
    /* Copy the password and strip of any character return if any. */
		
	fgets (line, MAX_ID_PASSWORD, fptr);
	len = strlen(line);
	if ('\n' == line[len-1])
	{
		len--;
		line[len] = '\0';
	}
	*retLength = len;
	memcpy (retPassword, line, len);
  }
  fclose (fptr);
  return (ERR_BSAFE_EXTERNAL_PASSWORD);
	}
	return (ERR_EM_CONTINUE);
}


