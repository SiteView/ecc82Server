#ifdef __cplusplus
extern "C" {
#endif


#ifndef NSF_OBJECT_DEFS
#define NSF_OBJECT_DEFS

#ifndef NSF_DEFS
#include "nsfdata.h"
#endif


/* Note Storage File Object Package Definitions */

/*	Available across the network */

#if !defined(NSFDbAllocObject)
STATUS LNPUBLIC NSFDbAllocObject (DBHANDLE hDB, DWORD dwSize,
								WORD Class,
								WORD Privileges,
								DWORD far *retObjectID);
#endif


#if !defined(NSFDbReallocObject)
STATUS LNPUBLIC NSFDbReallocObject (DBHANDLE hDB, DWORD ObjectID, DWORD NewSize);
#endif

#if !defined(NSFDbFreeObject)
STATUS LNPUBLIC NSFDbFreeObject (DBHANDLE hDB, DWORD ObjectID);
#endif

#if !defined(NSFDbGetObjectSize)
STATUS LNPUBLIC NSFDbGetObjectSize (DBHANDLE hDB, DWORD ObjectID, WORD ObjectType,
								DWORD far *retSize,
								WORD far *retClass,
								WORD far *retPrivileges);
#endif

#if !defined(NSFDbReadObject)
STATUS LNPUBLIC NSFDbReadObject (DBHANDLE hDB, DWORD ObjectID,
								DWORD Offset, DWORD Length,
								HANDLE far *rethBuffer);
#endif

#if !defined(NSFDbWriteObject)
STATUS LNPUBLIC NSFDbWriteObject (DBHANDLE hDB, DWORD ObjectID,
								HANDLE hBuffer,
								DWORD Offset, DWORD Length);
#endif

/* End of Note Storage File Object Package */

#endif


#ifdef __cplusplus
}
#endif

