
/*	foldman.h - Public interface to folder routines. */


#ifndef FOLDMAN_DEFS
#define FOLDMAN_DEFS

#ifndef DESIGN_DEFS
#include "design.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


STATUS LNPUBLIC FolderCreate
			(
			DBHANDLE	hDataDB,
			DBHANDLE	hFolderDB,
			NOTEID		FormatNoteID,
			DBHANDLE	hFormatDB,
			char far *	pszName,
			WORD		wNameLen,
			DESIGN_TYPE	FolderType,	
			DWORD		dwFlags,
			NOTEID far * pNoteID
			);

STATUS LNPUBLIC FolderCopy
			(
			DBHANDLE	hDataDB,
			DBHANDLE	hFolderDB,
			NOTEID		FolderNoteID,
			char far *	pszName,
			WORD		wNameLen,
			DWORD		dwFlags,
			NOTEID far * pNewNoteID
			);


STATUS LNPUBLIC FolderDocRemove
			(
			DBHANDLE	hDataDB,
			DBHANDLE	hFolderDB,
			NOTEID		FolderNoteID,
			HANDLE 		hTable,
			DWORD		dwFlags
			);

STATUS LNPUBLIC FolderDocAdd
			(
			DBHANDLE	hDataDB,
			DBHANDLE	hFolderDB,
			NOTEID		FolderNoteID,
			HANDLE 		hTable,
			DWORD		dwFlags
			);

STATUS LNPUBLIC FolderDocRemoveAll
			(
			DBHANDLE	hDataDB,
			DBHANDLE	hFolderDB,
			NOTEID		FolderNoteID,
			DWORD		dwFlags
			);
			
STATUS LNPUBLIC FolderDocCount
			(
			DBHANDLE	hDataDB,
			DBHANDLE	hFolderDB,
			NOTEID		FolderNoteID,
			DWORD		dwFlags,
			DWORD far *	pdwNumDocs
			);


STATUS LNPUBLIC FolderDelete
			(
			DBHANDLE	hDataDB,
			DBHANDLE	hFolderDB,
			NOTEID		FolderNoteID,
			DWORD		dwFlags
			);

STATUS LNPUBLIC FolderMove
			(
			DBHANDLE	hDataDB,
			DBHANDLE	hFolderDB,
			NOTEID		FolderNoteID,
			DBHANDLE	hParentDB,
			NOTEID		ParentNoteID,
			DWORD		dwFlags
			);


STATUS LNPUBLIC FolderRename
			(
			DBHANDLE	hDataDB,
			DBHANDLE	hFolderDB,
			NOTEID		FolderNoteID,
			char far *	pszName,
			WORD		wNameLen,
			DWORD		dwFlags
			);



#ifdef __cplusplus
}
#endif

#endif /* FOLDMAN_DEFS */


