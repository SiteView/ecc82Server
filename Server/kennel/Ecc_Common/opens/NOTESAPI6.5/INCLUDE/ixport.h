#ifdef __cplusplus
extern "C" {
#endif

/*	Useful definitions for import/export .EXEs */

#ifndef IXPORT_DEFS
#define IXPORT_DEFS

#ifndef IXEDIT_DEFS
#include "ixedit.h"
#endif
#ifndef IX_DEFS
#include "ixview.h"
#endif
#ifndef OSSIGNAL_DEFS
#include "ossignal.h"
#endif

/*	Define the input/output arguments to an import/export module. */


typedef STATUS (LNCALLBACKPTR IXENTRYPROC)(
							void far *IXContext,	/* See IXEDIT.H or IXVIEW.H */
							WORD Flags,				/* IXFLAG_xxx */
							HMODULE hModule,
							char far *AltLibraryName,
							char far *PathName);	/* File to act upon */

/* Flags passed to all imports/exports */

#define IXFLAG_FIRST		0x01		/* First time thru flag */
#define IXFLAG_LAST			0x02		/* Last time thru flag */
#define IXFLAG_APPEND		0x04		/* For exports, Append to output file */



/*	Macros for functions which are actually vectored calls to the main
	Notes process.
*/


#define IXPostMessage(str) \
			(OSGetSignalHandler(OS_SIGNAL_MESSAGE) ? \
				(*(OSSIGMSGPROC) OSGetSignalHandler(OS_SIGNAL_MESSAGE))(str, OSMESSAGETYPE_POST_NOSERVER) : \
				NOERROR)


/*	Definitions of the flags word which follows the description field
	in the notes.ini import/export flag. */

/*	Allow files to be appended on export */
#define IXFLAG_ALLOW_APPEND				0x01

/*	For doc exports, used a Composite Data scratch file. If not set, 
	uses a CD scratch buffer. All version 1 exports use a scratch
	buffer, Version 2 exports may use a scratch file. */

#define IXFLAG_DOC_EXPORT_USE_CD_FILE	0x02

#endif



#ifdef __cplusplus
}
#endif

