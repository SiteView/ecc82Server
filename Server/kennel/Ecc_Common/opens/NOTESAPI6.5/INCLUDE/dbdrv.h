#ifdef __cplusplus
extern "C" {
#endif

/*
	External Database Driver Definitions
*/

#ifndef DBDRV_DEFS
#define DBDRV_DEFS

#ifndef NSF_DATA_DEFS
#include "nsfdata.h"					/* DBID */
#endif


/*	Class name definitions */

#define MAX_DBDRV_NAME	5			/* Max. length of database driver class name */
#define	DBDRV_PREFIX "DB"			/* pre-pended to database driver name */

/*	Database function definitions common to all databases, although
	the actual arguments may differ for any given function. */

#define DB_LOOKUP 	0				/* look something up in a database */
#define DB_COLUMN 	1				/* return an entire column from a database */
#define	DB_DBEXT	2				/* Extended function */

/*	Driver vectors */

typedef void far * HDBDSESSION;		/* DBD session handle */
typedef STATUS (LNCALLBACKPTR DBOPENBYIDPROC)(DBID ReplicaID, char far *FileTitle, HANDLE hNames, DBHANDLE far *rethDB);

typedef struct dbvec
	{
	char		ClassName[MAX_DBDRV_NAME+1];/* name of driver class + '\0' */
	HMODULE		hModule;					/* hModule of loaded module */

	/*	Do per-process initialization routine.  This is called just after
		the LoadLibrary, and is the first entry point in the library.
		When this entry point is called, all of the other entry point
		vectors are filled in by this routine. */

	STATUS (LNCALLBACKPTR Init)(struct dbvec far *vec);

	/*	Per-process termination routine, ASSUMING that all open sessions
		for this context have been closed by the time this is called.  This
		is called just prior to the FreeLibrary. */

	STATUS (LNCALLBACKPTR Term)(struct dbvec far *vec);

	/*	Open a session.  Any databases opened as a side-effect of Functions
		performed on this session will gather up their context in the
		hSession returned by this routine. */

	STATUS (LNCALLBACKPTR Open)(struct dbvec far *vec, HDBDSESSION far *rethSession);

	/*	Close a session, and as a side-effect all databases whose context
		has been built up in hSession. */

	STATUS (LNCALLBACKPTR Close)(struct dbvec far *vec, HDBDSESSION hSession);

	/*	Set auxiliary context, used principally when called from Desk */

	STATUS (LNCALLBACKPTR SetOpenContext)(struct dbvec far *vec, HDBDSESSION hSession, char far *DefaultDbName, DBOPENBYIDPROC Proc, HANDLE hNames, DWORD hParentWnd);

	/*	Perform a function on a session.  If any databases must be opened
		as a side-effect of this function, gather context into hSession
		so that it may be later deallocated/closed in Close. */

	STATUS (LNCALLBACKPTR Function)(struct dbvec far *vec, HDBDSESSION hSession,
							WORD Function,
							WORD argc, DWORD far *argl, void far * far *argv,
							HANDLE far *rethResult, DWORD far *retResultLength);

	/*	Flags */

	FLAG fUpdateIfModified:1;		/* TRUE if we want UpdateCollections if modified */

	} DBVEC;


#endif	/* DBDRV_DEFS */


#ifdef __cplusplus
}
#endif

