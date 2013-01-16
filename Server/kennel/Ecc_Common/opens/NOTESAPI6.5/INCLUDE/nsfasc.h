
/* nsfasc.h - NSF Note OLE Associated Items API */

#ifndef NSF_ASCAPI_DEFS
#define NSF_ASCAPI_DEFS

#ifdef __cplusplus
extern "C" {
#endif

#define	ASSOCITEM_IFEXISTS_FAIL			0
#define	ASSOCITEM_IFEXISTS_OVERWRITE	1
#define	ASSOCITEM_IFEXISTS_USEEXISTING	2



/*	
	API Utility functions used to associate, disassociate and get $FILEs
	associated with an OLE2 object.
*/


/******************************************************************************
 *
 *	NSFNoteOLEAssociateFileToObject
 *
 *	Description:
 *		Associate a Notes attachment ($FILE) with a Notes OLE object.  The 
 *		association can be made using an existing associated $FILE or can create
 *		a new $FILE from a system file and then create the association. 
 *
 *		If there is no file spec, but there is an attachment name and the
 *		attachment exists, use existing attachment for association.
 *
 *	Arguments:
 *		hNote
 *			Handle of the OLE object's note.
 *		pszObjName
 *			Ptr to name of the OLE object.  This is the name of the OLE object 
 *			in the note.
 *		pszFileSpec
 *			Ptr to the file specification of the file to associate with the object.
 *			If =NULL, attachment already exists.  See NSFNoteAttachFile's file_name
 *			argument for details.
 *		pszAttachmentName
 *			Ptr to the name of the attachment to associate.
 *		wIfExists
 *			=NOTEASSOCITEM_IFEXISTS_FAIL, if attachment ($FILE) already exists 
 *				as an associated item on any OLE object	in the note, return an error
 *			=NOTEASSOCITEM_IFEXISTS_OVERWRITE, if attachment ($FILE) already exists
 *				as an associated item on any OLE object	in the note, overwrite it 
 *				with the specified file
 *			=NOTEASSOCITEM_IFEXISTS_USEEXISTING, if attachment ($FILE) already exists
 *				as an associated item on any OLE object	in the note, use the existing 
 *				attachment and ignore the specified file
 *		dwFlags
 *			Currently not used.
 *		pdwId
 *			Ptr to the location which upon return contains the Id of the
 *			associated $FILE.  If =NULL, no Id is returned.
 *
 *	Return
 *		Status
 *
 *********************************************************************************/

STATUS LNPUBLIC NSFNoteOLEAssociateFileToObject(
								NOTEHANDLE	hNote,
								char *		pszObjName,
								char *		pszFileSpec,
								char *		pszAttachmentName,
								WORD		wIfExists,
								DWORD		dwFlags,
								DWORD *		pdwId);

								
/******************************************************************************
 *
 *	NSFNoteOLEDisassociateFileFromObject
 *
 *	Description:
 *		Disassociate a Notes attachment ($FILE) from a Notes OLE object.
 *
 *	Arguments:
 *		hNote
 *			Handle of the OLE object's note.
 *		pszObjName
 *			Ptr to name of the OLE object.  This is the name of the OLE object 
 *			in the notes.
 *		pszAttachmentName
 *			Ptr to the name of the attachment to disassociate.
 *
 *	Return
 *		Status
 *
 *********************************************************************************/

STATUS LNPUBLIC  NSFNoteOLEDisassociateFileFromObject(
								NOTEHANDLE	hNote,
								char *		pszObjName,
								char *		pszAttachmentName);


/******************************************************************************
 *
 *	NSFNoteOLEGetAssociateFileOnObject
 *
 *	Description:
 *		Get a specified associated Notes attachment ($FILE) on a Notes OLE object.
 *		The name returned is in OSMemAlloc'd memory.  It is the caller's
 *		responsibility to free this memory.
 *
 *	Arguments:
 *		hNote
 *			Handle of the OLE object's note.
 *		pszObjName
 *			Ptr to name of the OLE object.  This is the name of the OLE object 
 *			in the note.
 *		phszAttachmentName
 *			Ptr to the location where the handle to the name of the attachment
 *			is placed upon return.
 *		dwId
 *			Id of the attachment to get.  Ids start at zero.
 *
 *	Return
 *		Status
 *
 *********************************************************************************/

STATUS LNPUBLIC  NSFNoteOLEGetAssociateFileOnObject(
								NOTEHANDLE		hNote,
								char *			pszObjName,
								HANDLE far *	ppszAttachmentName,
								DWORD			dwId);

#ifdef __cplusplus
}
#endif

#endif /* NSF_ASCAPI_DEFS */

