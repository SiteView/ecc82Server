#ifdef __cplusplus
extern "C" {
#endif

#ifndef DBMISC_DEFS
#define DBMISC_DEFS

#ifndef GLOBAL_DEFS
#include "global.h"
#endif

#ifndef NAME_DEFS
#include "names.h"
#endif

#ifndef NSF_DEFS
#include "nsf.h"
#endif

#ifndef ADMINP_DEFS
#include "adminp.h"
#endif

STATUS LNPUBLIC StoredFormAddItems (DBHANDLE hSrcDbHandle,
									HANDLE hSrcNote,
									HANDLE hDstNote,
									BOOL bDoSubforms,
									DWORD dwFlags);

STATUS LNPUBLIC StoredFormRemoveItems(	HANDLE hNote, 
										DWORD dwFlags);

STATUS LNPUBLIC StoredFormAppendSubformToken(	char* pszSubName, 
												WORD wSubNameBufferLen);

/* end ifndef DBMISC_DEFS */
#endif

#ifdef __cplusplus
}
#endif
