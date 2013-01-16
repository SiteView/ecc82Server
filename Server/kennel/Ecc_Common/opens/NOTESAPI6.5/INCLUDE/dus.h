#ifndef DUS_DEFS
#define DUS_DEFS

#ifdef __cplusplus
extern "C" {
#endif

/*	Function prototypes and definitions for the Domino Upgrade Services (DUS) SPI.
	Unlike typical Notes API functions, these Service Provider Interface functions
	do NOT exist in the notes library, but are implemented by the
	Domino Upgrade Serice builder to be called by the DUS framework.
	A sample of a Domino Upgrade Service (which implements these functions)
	will be available with the Notes SDK. */

#if defined(ADMIN_PLATFORM) /* This SPI interface only available with Admin Client */

#ifndef	USERREG_DEFS
#include "reg.h"
#endif

/*************************  DEFINITIONS  ***************************/

/* Max size of Name element in DUS_ENTRY struct */
#define DUSMAXFLATNAME 128


/* DUS Init Flags -- set in the StartFlags param of DUSStart. */

#define fDUSDoMailConversion		0x00000001	/* mail exists to convert and should be converted */
#define fDUSGenerateRandPWs			0x00000002	/* generate random passwords if no user password supplied */
#define fDUSAdvancedDlg				0x00000004	/* advanced dialog is available DUSAdvancedDlg is implemented */
#define fDUSUseFullNameProvided		0x00000008	/* use the full name provided as a secondary Notes user name */
#define fDUSAllowEmptyGroups		0x00000010	/* allow addition of empty groups to Notes Address Book */
#define fDUSMailConversionOnly		0x00000020	/* Only convert mail for this migrating user.  The person document
												   and ID file must already exist in the address book in this case */
#define fDUSAddAdminToACL			0x00000080	/* Add the administrator to the mail file access control list during
												   registration mail creation.  This allows mail conversion access to
												   mail file in two pass migration scenarios (directory migrated first,
												   mail converted at a later date). */
#define fDUSOverwritePasswords		0x00000100	/* Generate random password for every user, always overwriting any existing password */

#define fDUSUseFilters				0x00000200	/* (NEW IN R6) See comment to DUSRetrieveFilters below */

/* DUS Entry Flags -- used in 'EntryFlags' for DUS_ENTRY struct */
#define fDUSExternalEntry			0x00010000	/* (NEW IN R6) If flag is set for a group member, it will not be registered in Notes, only added to the group */
#define fDUSIgnoreEntry				0x00020000	/* (NEW IN R6) If flag is set for a group member, it will neither be registered in Notes, nor added to the group */
/* NOTE: fDUSExternalEntry and fDUSIgnoreEntry flags are ignored when filter is executed, or all users/groups are retrieved */


/* Used to write to the ADMINP_GROUP_TYPE field in the group note. */
#define DUS_GROUP_TYPE_MULTI_PURPOSE	"0"
#define DUS_GROUP_TYPE_MAIL_ONLY		"1"
#define DUS_GROUP_TYPE_ACL_ONLY			"2"
#define DUS_GROUP_TYPE_DENY_LIST_ONLY	"3"


/*	The below four flags for extended error processing.
	Used in pErrorLevel param with DUSExtendedErrorText */

/* The below three flags should not be or'd in together, only exclusively.
	For example, if DUS_ERROR_LEVEL_INFO is or'd in, do not or in
	DUS_ERROR_LEVEL_WARNING or DUS_ERROR_LEVEL_ERROR. DUS_ERROR_LEVEL_ERROR
	signals the DUS framework (Notes) to go no farther and to shutdown the DUS */
#define DUS_ERROR_LEVEL_INFO			0x0001
#define DUS_ERROR_LEVEL_WARNING			0x0002
#define DUS_ERROR_LEVEL_ERROR			0x0004
/* Use this flag if the error should not be displayed. */
#define DUS_ERROR_NO_DISPLAY			0x0008

/************************** TYPEDEFS ****************************/

/*	DUS_ENTRY -- Represents one user or group.  An array of these
	structures is allocated by Notes and passed in to the DUS with
	DUSRetrieveUsers and DUSRetrieveGroups.  The DUS fills in the
	data and the array is passed back to Notes,	where the data
	is displayed in the Notes Upgrade UI. */

typedef struct
{
char	Name[DUSMAXFLATNAME+1];	/*	required -- user or group name */
DWORD	ID;						/*	required -- unique value identifying this user or group */
DWORD	EntryFlags;				/*	not to be used by the DUS */
HANDLE	hParentGroupList;		/* 	Optional and rarely used.  Alloced by the DUS or Notes and freed by Notes.
									Contains a Notes LIST of group names to which the entry belongs.
									This can be adjusted by Notes after DUSInitialize, depending
									on administrator choices at the UI, but can be initially set
									by the DUS.  This LIST handle must be created with the
									prefix data type set to FALSE (see ListAllocate) and must
									be passed UNLOCKED back to Notes. */
} DUS_ENTRY, *PDUS_ENTRY;


/* typedef for progress bar callback passed to DUS with DUSInitialize */

typedef void   (LNCALLBACKPTR DUSPROGRESSBARPROC)(DWORD Range, DWORD Position, char *MessageText);
typedef STATUS (LNVARARGS * DUSLOGEVENTPROC)(STATUS StringID, HMODULE hModule, STATUS AdditionalErrorCode,...);


/* Prototypes for DUS SPI functions */

/*************************************************************************
    FUNCTION: 	DUSGetName

    PURPOSE:  	Provide UI strings immediately after dll is loaded.

	INPUTS:		hInstance -- instance handle of this DUS.
				DUSNameBufLen -- size of the DUSNameBuf.
				DUSDescriptionBufLen -- size of the DUSDescriptionBuf.

	OUTPUTS:	DUSNameBuf -- buffer into which the DUS will copy the name
				of the DUS as it will appear in the Foreign Directory Source
				listbox (in the upgrade dialog).
				DUSDescriptionBuf -- buffer into which the DUS will copy a brief
				description of the DUS as it will appear in the upgrade dialog.

    COMMENTS: 	This call notifies the upgrade DLL that the upgrade dialog
    			is being displayed.  It also passes in the instance handle of
    			the DUS, which the DUS may want to store for later DUS operations.
*************************************************************************/
STATUS	LNCALLBACK	DUSGetName(	HMODULE hInstance,
							char *DUSNameBuf,
							WORD DUSNameBufLen,
							char *DUSDescriptionBuf,
							WORD DUSDescriptionBufLen );




/*************************************************************************
    FUNCTION: 	DUSStart

    PURPOSE:  	Notifies the DUS of one of two situations:
    				1)	It has been selected from the foreign directory
		    			source list in the foreign directory dialog or;
					2)	The first of possibly several users retrieved
						from this DUS is about to be registered in Notes.
				It also supplies the DUS framework with a context handle,
				which will be passed back to the DUS with every subsequent
				DUSxxx function.
				finally, it supplies the DUS with flags representing
				UI defaults for the Foreign Directory dialog.

	INPUTS:		hInstance -- Instance handle to this DUS dll.

				hUserNote -- this note handle will only be valid (non-NULL)
				if DUSStart is called just prior to user registration
				(from the user registration dialog, as opposed to from the
				foreign directory dialog).  The hUserNote contains context
				information (represented by the context handle passed back
				to the caller).  The DUS can stores this context information
				in the hUserNote passed in with DUSGetUserInformation.
				When DUSStart is called from the Foreign Directory dialog,
				hUserNote will be NULLHANDLE and the DUS allocates and inits
				a context buffer for the first time.

				DUSProgressBar -- pointer to a callback used by the DUS
				to display progress on potentially length operations, such
				as user retrieval.  This should be retained by the DUS for
				subsequent calls.
			
				DUSLogEvent -- pointer to a callback used by the DUS
				to log progress to the Notes log on any operation, such
				as user retrieval.  This should be retained by the DUS for
				subsequent calls.  The log cannot be written to after DUSStop
				is called.

	OUTPUTS:	pRethContext -- pointer to a context handle the DUS can optionally
				allocate/return to the DUS framework.  On every subsequent call
				to the DUS in the current session, this context handle will be passed
				to the DUS by the DUS framework.  The DUS is responsible for freeing
				this handle in DUSStop and should store the context handle in the
				hUserNote if the DUS needs to convert mail or take pre- or post-
				user registration actions available with DUSRegistrationNotify.


    COMMENTS: 	This function is also where the DUS should put up any secondary UI
				necessary to handle directory specific operations, such as
				retrieving users.  For example, if the administrator will have
				to supply a password to access the foreign directory source,
				the DUS should prompt for it here.

				DUSStart is called once per Foreign Directory dialog session
				(for each DUS) and once per registration session (for each DUS,
				if users from that DUS source are being registered).
*************************************************************************/
STATUS	LNCALLBACK	DUSStart(	HMODULE hInstance,
							HANDLE *pRethContext,
							NOTEHANDLE hUserNote,
							DWORD *pRetInitFlags,
							DUSPROGRESSBARPROC DUSProgressBar,
							DUSLOGEVENTPROC DUSLogEvent);



/****************************************************************************
    FUNCTION:	DUSExtendedErrorText

    Purpose:
    	This call retrieves the text to be displayed in the
		event that a call to the DUS extension DLL fails and the dll
		wishes to return it's own error string.  Any call implemented
		in this dll wanting this callback should return ERR_DUS_EXTENDED_ERROR.

	Inputs:
		hContext -- pointer to DUS specific context.  Alloced/provided by the DUS
					in DUSStart and passed back to the DUS with every DUS function.

		BufferLen -- length of ErrorBuffer

	Outputs:
		ErrorBuffer -- lmbcs string passed to DUS framework for display.

		pErrorLevel -- pointer to one of three error level flags supplied by the DUS
						(see DUS_ERROR_LEVEL_xxx in dus.h).  This value determines
						how the error message box will be displayed.  The three flags
						relating to error level should only be or'd in exclusively (not
						with each other).  A fourth flag, DUS_ERROR_NO_DISPLAY, prevents
						display of the error if or'd in.
*******************************************************************************/
void	LNCALLBACK	DUSExtendedErrorText(	HANDLE hContext,
										char * ErrorBuffer,
										WORD BufferLen,
										WORD *pErrorLevel);




/*************************************************************************
    FUNCTION: 	DUSRetrieveUsers

    Purpose:
    	This function allocates and passes back an array of DUS_ENTRY structs
    	containing information about the users available for upgrade/migration.

	Inputs:
		hContext -- provided by the DUS with DUSStart and passed back
					with every call by the framework.
		StartIndex -- 0L the first time the DUS is called.  If all users
					are not retrieved the first time and subsequent calls
					are necessary, this value is equal to the resume index
					passed back by the DUS on the previous call (*pResumeIndex).
		NumUsersRequested -- number of users that should be provided
							to the caller, if available.

	Outputs:
		pResumeIndex -- Set this value if additional users need to be
			returned to the DUS framework (number of available users exceeds
			the number requested by the framework).  This value will be returned
			to the DUS on the subsequent DUSRetrieveUsers call as the StartIndex.
			When user retrieval is complete the pResumeIndex should be set to 0L.
			The pResumeIndex will always be 0L the first time DUSRetrieveUsers
			is called.
			
		pNumUsersReturned -- points to the number of users returned by the DUS to
							the caller (users returned via the array of DUS_ENTRY
							structs represented	by pRethExternalUsers).  This value
							should be used to allocate an array of DUS_ENTRY structs
							with the size:
								NumUsersRequested * sizeof(DUS_ENTRY)
		pRethExternalUsers -- pointer to the memory handle of an array of DUS_ENTRY
							structs returned to the caller.  The DUS allocs and assigns
							this handle, sets the user information into the array, and
							returns the handle UNLOCKED to the caller. The array should
							contain *pNumUsersReturned structs.  The caller will
							free the memory associated with the handle.
		pRetUserEntrySize -- pointer to the size of the DUS_ENTRY struct (an array of
							which are alloced by the DUS and passed back to the caller)

	Comments:	DUSRetrieveUsers will be continue to be called by the DUS framework
				until the DUS returns a resume index of 0 or the *pNumUsersReturned is
				less than *pNumUsersRequested.

*************************************************************************/
STATUS	LNCALLBACK	DUSRetrieveUsers(	HANDLE hContext,
									DWORD StartIndex,
									DWORD *pResumeIndex,
									DWORD NumUsersRequested,
									DWORD *pNumUsersReturned,
									HANDLE *pRethExternalUsers,
									DWORD *pRetUserEntrySize);



/*************************************************************************
    FUNCTION: 	DUSRetrieveGroups

    Purpose:
    	This function allocates and passes back an array of DUS_ENTRY structs
    	containing information about the users available for upgrade/migration.

	Inputs:
		hContext -- provided by the DUS with DUSStart and passed back
					with every call by the framework.
		StartIndex -- 0L the first time the DUS is called.  If all groups
					are not retrieved the first time and subsequent calls
					are necessary, this value is equal to the resume index
					passed back by the DUS on the previous call (*pResumeIndex).
		NumGroupsRequested -- number of groups that should be provided
							to the caller, if available.

	Outputs:
		pResumeIndex -- Set this value if additional groups need to be
			returned to the DUS framework (number of available groups exceeds
			the number requested by the framework).  This value will be returned
			to the DUS on the subsequent DUSRetrieveGroups call as the StartIndex.
			When group retrieval is complete the pResumeIndex should be set to 0L.
			The pResumeIndex will always be 0L the first time DUSRetrieveGroups
			is called.
			
		pNumGroupsReturned -- points to the number of groups returned by the DUS to
							the caller (groups returned via the array of DUS_ENTRY
							structs represented	by pRethExternalGroups).  This value
							should be used to allocate an array of DUS_ENTRY structs
							with the size:
								NumGroupsRequested * sizeof(DUS_ENTRY)
		pRethExternalGroups -- pointer to the memory handle of an array of DUS_ENTRY
							structs returned to the caller.  The DUS allocs and assigns
							this handle, sets the user information into the array, and
							returns the handle UNLOCKED to the caller. The array should
							contain *pNumUsersReturned structs.  The caller will
							free the memory associated with the handle.
		pRetGroupEntrySize -- pointer to the size of the DUS_ENTRY struct (an array of
							which are alloced by the DUS and passed back to the caller)

	Comments:	DUSRetrieveGroups will be continue to be called by the DUS framework
				until the DUS returns a resume index of 0 or the *pNumGroupsReturned is
				less than *pNumGroupsRequested.

*************************************************************************/
STATUS	LNCALLBACK	DUSRetrieveGroups(	HANDLE hContext,
									DWORD StartIndex,
									DWORD *pResumeIndex,
									DWORD NumGroupsRequested,
									DWORD *pNumGroupsReturned,
									HANDLE *pRethExternalGroups,
									DWORD *pRetGroupEntrySize);








/*************************************************************************
    FUNCTION: 	DUSGetUserInformation

    Comments:
    	This call supplies the upgrade DLL with one of the users selected
		for upgrade and returns full information for this user.

	Inputs:
		hContext -- handle to this DUS's context information,
					initially set in DUSStart.  Passed with every DUS call.
		UserName -- The text originally supplied to the DUS framework with DUSRetrieveUsers.
		hUserNote -- contains the UserName and ID used by the DUS to identify
					the user for which full information is being requested.
					The DUS stores and passes back information to be set
					in the NAB person document via the hUserNote.
		TotalLeftToRead -- This identifies the number of selected users for
					which full information still needs to be returned by the DUS.
					When this value is 1, it should be the last time DUSGetUserInformation
					is called.  This is passed as a courtesy to the DUS, which
					might be able to make retrieval more efficient if it knows:
					1) the number of users selected
					2) when retrieval is complete
	Outputs:
		hUserNote -- contains the UserName and ID used by the DUS to identify
					the user for which full information is being requested.
					The DUS stores and passes back information to be set
					in the NAB person document via the hUserNote.  When the DUS
					is called before/after user registration (DUSRegistrationNotify
					and optionally, DUSConvertMailFile) this hUserNote will be
					passed back to the DUS.  Any information the DUS needs at that
					time should be stored here, such as information represented by
					the hContext handle passed in to the DUS with every DUS function.
					If the DUS writes it's own non-standard "context" information
					to the hUserNote, it should store this info in one or more
					fields prefixed with "Context", such as "ContextPostOfficeInfo".
					This prevents an unintentional name collision with other field
					names in the hUser Note.

					The hUserNote should NOT be updated or closed by the DUS.  The
					DUS framework (caller) writes additional fields to this note and
					will update/close the hUserNote appropriately. 

	Comments:
*************************************************************************/
STATUS	LNCALLBACK	DUSGetUserInformation(	HANDLE hContext,
										char * UserName,
										NOTEHANDLE hUserNote,
										DWORD TotalLeftToRead);





/***********************************************************************
FUNCTION:	DUSGetGroupInformation

Purpose:
	Returns full information about the group for display immediately
	after the administrator has selected the group from the available list. 

Inputs:
	hContext -- pointer to DUS specific context.  Alloced/provided by
	the DUS in DUSStart and passed back to the DUS with every DUS function.

	GroupName -- name of the group selected (originally supplied by the DUS
	with DUSRetrieveGroups).

Output:
	Additional group information in the form of fields written to the hNote.
	The following fields can be supplied:
		GroupType -- type of Notes group, if known (Multi-purpose, mail only, etc.)
					Field name is ADMINP_GROUP_TYPE.

		Description -- description of the group if known (MAIL_LISTDESCRIPTION_ITEM)

		ParentGroupNamesList -- list of the groups parent groups, if applicable.
								Field name is USERREG_DUSPARENTGROUPS_ITEM.

		See stdnames.h for the field names of a group document.

************************************************************************/
STATUS	LNCALLBACK	DUSGetGroupInformation(	HANDLE hContext,
										char * GroupName,
										NOTEHANDLE hGroupNote);




/***********************************************************************
FUNCTION:	DUSGetGroupMembers

Purpose:
	Returns membership information about the group.  Members will also be
	added to the selected list for upgrade, depending on options chosen
	by the administrator in the group dialog.  Called after DUSGetGroupInformation
	(the administrator has selected the group from the available list). 

Inputs:
	hContext -- pointer to DUS specific context.  Alloced/provided by
	the DUS in DUSStart and passed back to the DUS with every DUS function.

	GroupName -- name of the group selected (originally supplied by the DUS
	with DUSRetrieveGroups).

Output:
	Additional group information in the form of fields written to the hNote.

	hUserMembersList -- allocated by the DUS, this is a list containing all
	of the user members of the Group identified by GroupName.

	hGroupMembersList -- allocated by the DUS, this is a list containing all
	of the immediate group members of the Group identified by GroupName.

	These LIST handles must be created with the prefix data type set
	to FALSE (see ListAllocate) and must be passed UNLOCKED back to Notes.
	Notes will take care of deallocation.

Comments:
************************************************************************/
STATUS	LNCALLBACK	DUSGetGroupMembers(	HANDLE hContext,
									char * GroupName,
									NOTEHANDLE hGroupNote,
									HANDLE *phGroupMembersList,
									HANDLE *phUserMembersList);



/***********************************************************************
FUNCTION:	DUSAdvancedDlg

Purpose:
	Allows this dll to put up it's own dialog from the Foreign
	Directory dialog when the "Advanced" button is pushed.

Inputs:
	hContext -- pointer to DUS specific context.  Alloced/provided by
	the DUS in DUSStart and passed back to the DUS with every DUS function.

Output:
	Advanced dialog put up the by DUS.

Comments:
	DUSStart can return a flag which ORs in fDUSAdvancedDlg.
	If it does so, when the "Advanced" button is pressed
	from the foreign Directory dialog, DUSAdvancedDlg will be called.

	Information collected from the dialog can be stored in the buffer
	represented by hContext, but the handle itself should not change.
************************************************************************/
STATUS	LNCALLBACK	DUSAdvancedDlg(	HANDLE hContext);



/***********************************************************************
FUNCTION:	DUSRegistrationNotify

Purpose:
	Notifies the DUS that Notes user registration for the user represented by
	UserName (and originally retrieved from this DUS) is about to start
	or just finished.  This allows the DUS to take pre or post registration
	action to supplement registration-related activity.

Inputs:
	hContext -- pointer to DUS specific context.  Alloced/provided by
	the DUS in DUSStart and passed back to the DUS with every DUS function.

Inputs:
	UserName -- lmbcs string identifying the user being registered,
				originally passed in by the DUS with DUSRetrieveUsers.

	hUserNote -- contains user and DUS specific information.

	bAfterEvent -- FALSE if the notification is just prior to user registration,
					TRUE if the notification is just after.

Comments:
	DUSRegistrationNotify is an optional function, which must be implemented
	but can do nothing if not needed by the DUS.
************************************************************************/
STATUS	LNCALLBACK	DUSRegistrationNotify(	HANDLE hContext,
										char *UserName,
										NOTEHANDLE hUserNote,
										BOOL bAfterEvent);




/*************************************************************************
FUNCTION: 	DUSConvertMailFile

Purpose:
	Converts DUS mail into Notes mail file.

Inputs:
	UserName -- lmbcs string identifying the user being registered,
				originally passed in by the DUS with DUSRetrieveUsers.

	MailFilePath -- lmbcs string identifying the fully qualified mail
					file path (including mail server) of the empty 
					Notes mail file to be converted.

	hUserNote -- contains user and DUS specific information.

	hNewPersonNote -- the Note handle of the person document just created
				by user registration in the Address book.  If the DUS
				modifies this note it must update the note (NSFNoteUpdate),
				but the DUS framework will close the note.

	SignalStatus -- address of a function to display status in the
				Status bar of the admin panel (below the user reg
				dialog).  The DUS can use this proc to apprise the
				administrator of mail conversion progress if desired.

Comments:
 	DUSConvertMailFile is called after the user has been
 	successfully registered and a Notes mail file (empty)
 	has been created.

Outputs:
	Converted mail file (implicit).
*************************************************************************/
STATUS	LNCALLBACK	DUSConvertMailFile( HANDLE hContext,
									char *UserName,
									char *MailFilePath,
									NOTEHANDLE hUserNote,
									NOTEHANDLE	hNewPersonNote,
									REGSIGNALPROC SignalStatus);



/***********************************************************************
FUNCTION:	DUSStop

Purpose:
	Notifies the DUS that the end of this session (started
	by DUSStart and represented by hContext) is over.  Memory
	associated with the context handle (alloced in DUSStart) should
	now be freed.

Inputs:
	hContext -- pointer to DUS specific context.  Alloced/provided by
	the DUS in DUSStart and passed back to the DUS with every DUS function.

Comments:
	DUSStop is called from one of two places:
		1)	If the DUS was started from the foreign Directory dialog,
			DUSStop will be called when that dialog is closed.

		2)	If the DUS was started during user registration because
			a user from this DUS was being registered, DUSStop will
			be called when registration of ALL users in that session
			is complete.
************************************************************************/
STATUS	LNCALLBACK	DUSStop( HANDLE hContext);




/*************************************************************************
	FUNCTION: 	DUSTerm

    Comments:
     	This call notifies the DUS DLL that it is being terminated/unloaded.
		Any alloced memory not already freed should be freed here.
*************************************************************************/
STATUS	LNCALLBACK	DUSTerm( void);





/*************************************************************************
    FUNCTION: 	DUSRetrieveUsersEx (Added in R6)

    Purpose:
    	This function allocates and passes back an array of DUS_ENTRY structs
    	containing information about the users in a group, or in a filter search result.

	Inputs:
		hContext -- provided by the DUS with DUSStart and passed back
					with every call by the framework.
		Filter	 -- filter string in application-defined format that user has 
					selected.
		GroupName -- Name of the group, non-NULL if members of the group are requested.
		hGroupNote -- Group Note handle. 
		StartIndex -- 0L the first time the DUS is called.  If all users
					are not retrieved the first time and subsequent calls
					are necessary, this value is equal to the resume index
					passed back by the DUS on the previous call (*pResumeIndex).
		NumUsersRequested -- number of users that should be provided
							to the caller, if available.

	Outputs:
		pResumeIndex -- Set this value if additional users need to be
			returned to the DUS framework (number of available users exceeds
			the number requested by the framework).  This value will be returned
			to the DUS on the subsequent DUSRetrieveUsersEx call as the StartIndex.
			When user retrieval is complete the pResumeIndex should be set to 0L.
			The pResumeIndex will always be 0L the first time DUSRetrieveUsersEx
			is called.
			
		pNumUsersReturned -- points to the number of users returned by the DUS to
							the caller (users returned via the array of DUS_ENTRY
							structs represented	by pRethExternalUsers).  This value
							should be used to allocate an array of DUS_ENTRY structs
							with the size:
								NumUsersRequested * sizeof(DUS_ENTRY)
		pRethExternalUsers -- pointer to the memory handle of an array of DUS_ENTRY
							structs returned to the caller.  The DUS allocs and assigns
							this handle, sets the user information into the array, and
							returns the handle UNLOCKED to the caller. The array should
							contain *pNumUsersReturned structs.  The caller will
							free the memory associated with the handle.
		pRetUserEntrySize -- pointer to the size of the DUS_ENTRY struct (an array of
							which are alloced by the DUS and passed back to the caller)

	Comments:	- DUSRetrieveUsersEx will be continue to be called by the DUS framework
				until the DUS returns a resume index of 0 or the *pNumUsersReturned is
				less than *pNumUsersRequested.
				- If group members are retrieved, calls will stop if 65K entries are retrieved.
				65K is the limit imposed by Notes on number of group members.
				- if Filter is NULL, and GroupName is valid, return members of a group.
				if both are NULL, return all entries.
*************************************************************************/
STATUS	LNCALLBACK	DUSRetrieveUsersEx(	HANDLE hContext,
									char * Filter,
									char * GroupName,
									NOTEHANDLE hGroupNote,
									DWORD StartIndex,
									DWORD *pResumeIndex,
									DWORD NumUsersRequested,
									DWORD *pNumUsersReturned,
									HANDLE *pRethExternalUsers,
									DWORD *pRetUserEntrySize);



/*************************************************************************
    FUNCTION: 	DUSRetrieveGroupsEx (added in R6)

    Purpose:
    	This function allocates and passes back an array of DUS_ENTRY structs
    	containing information about the groups in a group, or in a filter search result.

	Inputs:
		hContext -- provided by the DUS with DUSStart and passed back
					with every call by the framework.
		Filter	 -- filter string in application-defined format that user has 
					selected.
		GroupName -- Name of the group, non-NULL if members of the group are requested.
		hGroupNote -- Group Note handle. 
		StartIndex -- 0L the first time the DUS is called.  If all groups
					are not retrieved the first time and subsequent calls
					are necessary, this value is equal to the resume index
					passed back by the DUS on the previous call (*pResumeIndex).
		NumGroupsRequested -- number of groups that should be provided
							to the caller, if available.

	Outputs:
		pResumeIndex -- Set this value if additional groups need to be
			returned to the DUS framework (number of available groups exceeds
			the number requested by the framework).  This value will be returned
			to the DUS on the subsequent DUSRetrieveGroupsEx call as the StartIndex.
			When group retrieval is complete the pResumeIndex should be set to 0L.
			The pResumeIndex will always be 0L the first time DUSRetrieveGroupsEx
			is called.
			
		pNumGroupsReturned -- points to the number of groups returned by the DUS to
							the caller (groups returned via the array of DUS_ENTRY
							structs represented	by pRethExternalGroups).  This value
							should be used to allocate an array of DUS_ENTRY structs
							with the size:
								NumGroupsRequested * sizeof(DUS_ENTRY)
		pRethExternalGroups -- pointer to the memory handle of an array of DUS_ENTRY
							structs returned to the caller.  The DUS allocs and assigns
							this handle, sets the user information into the array, and
							returns the handle UNLOCKED to the caller. The array should
							contain *pNumUsersReturned structs.  The caller will
							free the memory associated with the handle.
		pRetGroupEntrySize -- pointer to the size of the DUS_ENTRY struct (an array of
							which are alloced by the DUS and passed back to the caller)

	Comments:	-DUSRetrieveGroupsEx will be continue to be called by the DUS framework
				until the DUS returns a resume index of 0 or the *pNumGroupsReturned is
				less than *pNumGroupsRequested.
				- If group members are retrieved, calls will stop if 65K entries are retrieved.
				65K is the limit imposed by Notes on number of group members.
				- if Filter is NULL, and GroupName is valid, return members of a group.
				if both are NULL, return all entries.

*************************************************************************/
STATUS	LNCALLBACK	DUSRetrieveGroupsEx(	HANDLE hContext,
									char * pFilter,
									char * GroupName,
									NOTEHANDLE hGroupNote,
									DWORD StartIndex,
									DWORD *pResumeIndex,
									DWORD NumGroupsRequested,
									DWORD *pNumGroupsReturned,
									HANDLE *pRethExternalGroups,
									DWORD *pRetGroupEntrySize);


/***********************************************************************
FUNCTION:	DUSRetrieveFilters (new in R6)

Purpose:
	Returns default filter strings 

Inputs:
	hContext -- pointer to DUS specific context.  Alloced/provided by
	the DUS in DUSStart and passed back to the DUS with every DUS function.

Output:
	phDefaultFilters -- allocated by the DUS, this is a list containing default
	filter strings that will be shown in the combobox.

	bAllowType -- flag indicating whether user should be allowed to type 
	other custom filters, or only select from the default ones.

	The LIST handle must be created with the prefix data type set
	to FALSE (see ListAllocate) and must be passed UNLOCKED back to Notes.
	Notes will take care of deallocation.

Comments:
	DUSRetrieveFilters is called after DUSStart, in case when the DUS 
	is selected from the combobox. If fDUSUseFilters flag is passed to DUSStart,
	user will be allowed to type other filters, otherwise, (s)he will only be 
	able to choose among default filters, and options such as "All Users and Groups",
	"All Users" and "All Groups". 
************************************************************************/
STATUS	LNCALLBACK	DUSRetrieveFilters(	HANDLE hContext,
										HANDLE *phDefaultFilters);

#endif /* if defined (NT) */

#ifdef __cplusplus
}
#endif

#endif

