
#ifdef __cplusplus
extern "C" {
#endif


/*	This header file contains data structures and definitions used by
	OLE servers to perform Notes Field Exchange (FX).

	HOW TO USE THIS PROTOCOL WITH NOTES

	As an OLE server, first register yourself in the OLE registry during setup,
	and add the special SetDataFormat and RequestDataFormat keys (see below).  
	This will indicate to Notes that your OLE server is capable of receipt of 
	the special document information message (NotesDocInfo) and (optionally) 
	capable of receiving unsolicited requests for the updated hNote (NoteshNote)
	and NotesDocActions.  

	Notes will issue an OleSetData (which in turn invokes the server's SetData method) 
	with the doc info data block at the following  points in execution:

		1) Upon the initial instantiation of an object, regardless of the verb
		2) Upon initial creation of the object (OleCreate)
		3) During a document access mode change (i.e. from read-only to edit or from
			edit to read-only).

	Upon notification of the OLE_CLOSED event from the OLE server, Notes will 
	apply items that have been added to the hFXNote to the
	real document's hNote, and will re-calc the document fields so they will be saved
	with the document. Notes will apply the fields only if the the following two conditions
	are met: 1) Notes has received an OLE_CHANGED notification prior to receipt of the 
	OLE_CLOSED, and 2) items exist on the hFXNote.  BOTH CONDITIONS 1 AND 2 MUST 
	BE MET FOR NOTES TO MODIFY THE FIELDS.

	With respect to the NoteshNote RequestDataMessage, this message is used
	when Notes is requesting the FX OLE server for an update of the shared fields, 
	and is used in the following two circumstances:

		1) 	When a user issues a "File/Update" command from the OLE server
			application.

		2) 	If a Notes user closes (with Save) a Notes document in which
			an active OLE session is established with an FX server.



	API PROGRAM BEHAVIOR AND RECOMMENDATIONS
	
	As an API program that is also an OLE server, you should not issue a NotesInit
	call during normal activation if you are being launched "- embedding".  Because
	NotesInit() yields to the Windows kernel, the synchronous return of control to the
	OLE 1 Client DLL when Notes yields will cause the OLE 1 client DLL to think that the
	OLE server task activation has failed, thus returning failure back to the Ole client
	task (Notes).  The recommended behavior is to have an
	explicitly loaded "buffer" DLL (which issues the NotesInit) in the OLE server which
	is implicitly linked against Notes DLL's.  This "buffer" DLL should be invoked upon 
	receipt of the SetData message which contains the Notes doc info.


	ACCESSING THE DATABASE USING THE NOTES API (NSF)

	The Notes data buffer contains two NSF Note Handles, namely hNote and hFXNote.
	The hNote is an actual live hNote that the Notes Editor is using to access the
	document, and is to be treated as READ-ONLY, and is ONLY VALID FOR THE DURATION OF
	THE SetData method in the OLE server.  Under no circumstances should another
	task write to this hNote, or unpredictable bad things can happen.  The OLE server is
	free to use this hNote for reading items during the SetData method and should make 
	copies of the items to the hFXNote for writing.  The hFXNote is an EMPTY note which has been
	associated with the Database to which the hNote is associated.  The purpose of the
	hFXNote is a place holder for all items that the OLE server wants to modify,
	append, or delete to/from the document's note. Note that your OLE server can safely issue any
	item-level (non-I/O) calls to the hFXNote, but will CRASH on any database I/O
	functions unless an NSF context is first established for the task.  An API program should
	use the following NSF functions to associate (map for I/O) it's task with NSF:

		** Get DB handle from existing hFXNote from doc info message  **
		NSFNoteGetInfo(hFXNote, _NOTE_DB, &hMyHandleToDB)

		** Reopen the database to establish a context to NSF **
		if (error = NSFDbReopen(hMyHandleToDB, &hMyTaskDB))
			handle error

		** Associate the temp note's DB for the task  **
		NSFNoteSetInfo(hFXNote, _NOTE_DB, &hMyTaskDB)
				:
		do whatever you want with the hNote
				:
      
		** Close the DB to unmap yourself from the DB  **
		NSFDbClose(hMyTaskDB)

	None of the above code is necessary if the server is simply doing NSF Item level
	manipulation.

	NSF Items can be added or modified from the hFXNote using the NSFItemAppend             
	function.  If the OLE server wants to DELETE an item from the document,  the
	item should be added using the NSF data type TYPE_UNAVAILABLE.  When the Notes
	editor encounters an item in the hFXNote whose type is TYPE_UNAVAILABLE, the
	corresponding item in the real document's hNote will be deleted.
	

	PROCESSING THE NOTESHNOTE DATA REQUEST 

	When Notes requests the NOTESHNOTE data format from an FX OLE server,
	Notes is asking the server for a NEW hNote containing the fields which
	have been updated by the server.  To ensure mutual exclusion of NSF
	data structures, the FX OLE server MUST provide a new copy of the
	hNote every time Notes asks for this data.  Notes will dispose of 
	both the hNote and the memory block allocated for this message; the 
	server gives ownership of this data completely to Notes.

	The FX OLE server should use the Notes NSF API function	
	NSFNoteCopy to copy its current copy of its "active" hNote 
	to the one it will hand back to Notes.


	IMPORTANT NOTE: The OLE server object must return the NoteshNote data
					format in the EnumFormats method.  Failure
					to support this format during the Enum will prevent
					the GetData method from being called by the OLE server
					DLL.


	PROCESSING THE NOTESDOCACTION DATA REQUEST (Notesflow)

	A new FX feature for Notes V4 is the NotesFlow (tm) protocol, which publishes Notes
	document Form actions to the OLE server, and allows the OLE server to Notifiy
	Notes when a document action has been chosen by the user.  To enable an OLE server
	for Notesflow, perform the following steps:

		Register your application to receive Notes doc actions by registering the
		NotesDocAction in your application's RequestDataFormats section of the 
		system registry.

		When your application receives the NotesDocInfo message, examine the DocFlags
		to see if this message can contain document actions.  If it does, then 
		acquire the actions from the message, and display the actions somewhere
		in your application's UI.  Commonly used locations for these commands 
		could be a top level menu item, a hierarchical menu item, or a popup menu
		bound to a button: the choice is yours.

		Save the actions list and the Notes-defined command ID's associated with these
		actions somewhere in your program's data, to be used later when the user 
		executes one of these actions.

		When the user executes one of these actions from your application's UI, 
		you need to Notify Notes that this action was executed.  This can be done
		in one of two ways depending if your server application is an OLE 1 or
		OLE 2 server:

			OLE 1 Server:
				Issue an OLE_UPDATE to Notes, using the same notification that
				you would use if the user had issued a "File\Update" command.
				This will trigger Notes into asking your application for
				the NotesDocAction data via the OLE RequestData/GetData 
				mechanism.  When asked for this data format, respond with the
				command that the user has chosen, by filling in the 
				NOTES_EXECUTE_DOCACTION_MSG structure.  

				If Notes asks you for this message and the user has NOT selected
				a command, just return NULLHANDLE (this is an indicator that 
				NO command has been chosen, and Notes may ask you this at various
				times, so be ready to accept it anytime.).

			OLE 2 Server:
				Notes will set up an OLE Advise (via an IAdviseSink) using an
				OLE IDataObject associated with OLE Embedded Object, using the
				NotesDocAction clipboard data format.  When the user executes one
				of the Actions in the OLE server's UI, the OLE server simply has
				to Notify Notes adviseSink::OnDataChange method, providing Notes with
				the NOTES_EXECUTE_DOCACTION_MSG during the notification.  



*/

#ifndef OLENOTES_DEFS
#define OLENOTES_DEFS

#ifndef NSF_DATA_DEFS
#include "nsfdata.h"  	/*  for NOTELINK definition */
#endif

#ifndef NAME_DEFS
#include "names.h"		/* for MAXUSERNAME */
#endif

/* 
	Format for the NOTES_DOC_INFO_MSG. This is the format that Notes uses
	in the registration database to determine if an OLE server is capable
	of receiving this format of data.  If an OLE server supports this message,
	the following Key must be put in the registration database under
	Windows:

	<ObjectClass>\Protocol\StdFileEditing\SetDataFormats=NotesDocInfo


	Format for the NOTES_HNOTE message. This is the format that Notes uses
	in the registration database to determine if an OLE server is capable
	of rendering a message of this data format.  If an OLE server supports 
	this format, the following Key must be put in the registration database 
	under Windows:

	<ObjectClass>\Protocol\StdFileEditing\RequestDataFormats=NoteshNote


	The Macintosh registration database works differently than Windows
	in that apps cannot just make up their own keys and use them. So
	to work within the limits of this scheme, the only way for apps
	to generate unique strings is to map the formats into verb
	strings, using the "SetDataFormats"  or "RequestDataFormats" as
	the PROTOCOL hierarchy name rather than StdFileEditing:

	
	<ObjectClass>\Protocol\SetDataFormats\Verb\<verbnum>=NotesDocInfo

	<ObjectClass>\Protocol\RequestDataFormats\Verb\<verbnum>=NoteshNote


*/


/* This is the Windows clipboard format string registered by the Notes
	workstation for the NOTES_DOC_INFO_MSG */

#define NOTES_DOCINFO_CLIP_FORMAT	"NotesDocInfo"

/* Resource type used on the MAC for OleSetData for the NOTES_DOC_INFO_MSG */

#define RES_TYPE_DOCINFO_CLIP_FORMAT 'NTDI'  

/*	Version/platform info for this structure.  */

#define NOTES_DOC_INFO_VERSION1		0	/* First version of this structure */

/* 	This is the data object passed from Notes to OLE servers via 
	OleSetData(), using the registered format "NotesDocInfo", defined
	above.  Notes allocates this structure in global shared memory
	so the OLE server can access it and the OLE server is always responsible
	for deallocating the memory object (as with any handle passed to server
	via its SetData() method. */

typedef struct
	{
	WORD 		Version;			/* Version of this structure */
	NOTELINK 	DocLink;		/* Document in which OLE object resides */	
	WORD 		DocOpenMode;		/* DOC_OPENMODE_??? flags below */
	WORD 		DocFlags;			/* DOC_FLAGS_??? flags below*/
	WORD 		UserAccess;   		/* USER_ACCESS_??? flags below */
	HWND 		hDocWnd;			/* Notes document window handle */
	char 		UserName[MAXUSERNAME]; /* Notes user name */
	NOTEHANDLE 	hNote;		/* Handle to document's note: THIS IS READ ONLY! its lifetime is only for duration of the SetData method */
	NOTEHANDLE 	hFXNote;		/* Empty note, associated with document's DB. A server should use this for writing items to be applied to hNote */
	WORD  		cNumDocActions;	/* Count of doc actions, if any. Only passed to server if registered to support NotesDocAction GetData() */
	HANDLE 		hDocActions;	    /* Notes OSMemAlloc() handle to DOC_ACTION_INFO */
	DWORD 		dwUnused[2];
	} NOTES_DOC_INFO_MSG;






/* Document open mode flags */

#define DOC_OPENMODE_READONLY		0x0001	/* Read Only */
#define DOC_OPENMODE_EDIT			0x0002  /* Edit mode */

/* Other document state information */

#define DOC_FLAGS_EDITFORM			0x0001	/* Editing a form */
#define DOC_FLAGS_COMPOSE_NEW		0x0002	/* Composing a new document (doc not yet saved) */
											/* The flags below are necessary for interpreting hWnd */
											/* Note: if none of these flags is set, assume */
											/* the Win16 version of Notes */
#define DOC_FLAGS_WIN32				0x0004  /* Windows 32-bit version of Notes */
#define DOC_FLAGS_MAC				0x0008  /* Mac version of Notes */


#define USER_ACCESS_EDIT			0x0001	/* User has edit capabilities. */


/*	This is the structure of the Notes Document Actions passed through the
	hDocActions handle in the NOTES_DOC_INFO_MSG to those OLE objects registered
	as supporting the NotesDocAction SetData() format.   It is an array of the 
	following data structure, whose count is provided in the NOTES_DOC_INFO_MSG.
	This data block must be deallocated by the OLE server. */

#define ACTION_NAME_SIZE 128

typedef struct
	{
	int ActionID;	/*	Notes magic cookie identifying this action */
	WORD NameLength;
	char Name[ACTION_NAME_SIZE]; /* Action name in LMBCS character set */
	DWORD Flags; 	/* ACTION_FLAG_??? below */
	}DOC_ACTION_INFO;

#define ACTION_FLAG_HIDE		0x00000001	/* Don't show in UI.  Only used for some reserved actions */

/*	Here are a list of reserved actions.  Note that all actions in the 
	negative number range are "reserved" actions which have specified 
	semantics. */

#define DOC_ACTION_MAIL				-1	/*  Mail the document */
#define DOC_ACTION_CONSOLIDATE		-2  /*	Perform a consolidation */



/* This is the Windows clipboard format string registered by the Notes
	workstation for the NOTES_HNOTE and NOTES_DOCACTION request data/GetData message
	Also added 10/31 for V4, NotesDocActionDone completion notification (SetData) */

#define NOTES_HNOTE_CLIP_FORMAT	"NoteshNote"
#define NOTES_DOCACTION_CLIP_FORMAT "NotesDocAction"
#define NOTES_DOCACTION_DONE_CLIP_FORMAT  "NotesDocActionDone"

/* Resource type used on the MAC for OleRequestData for the NOTES_HNOTE & NOTES_DOCACTION request */

#define RES_TYPE_HNOTE_CLIP_FORMAT 'NTHN'  
#define RES_TYPE_DOCACTION_CLIP_FORMAT 'NTDA'  
#define RES_TYPE_DOCACTION_DONE_CLIP_FORMAT 'NTAD'  


/* 	Here is the format of the message initialized by the OLE FX server
	when receiving a request by Notes */

typedef struct
	{
	NOTEHANDLE hNote;		/* Handle to a copy of the server's updated hNote */
	DWORD Unused[3];		/* Future use */
	} NOTES_HNOTE_MSG;


/*	Here is the format of the message supplied by the OLE FX server to notify
	Notes to execute a specified Document Action.  The document action was
	passed from Notes in the NOTES_DOC_INFO_MSG.  */

typedef struct
	{
	int ActionID;			/*	Doc action ID for Notes to process */
	DWORD Flags;
	DWORD Unused[3];		/* Future use */
	} NOTES_EXECUTE_DOCACTION_MSG;


/*	Message passed to OLE FX server upon the completion of an executed Doc Action */

typedef struct
	{
	int ActionID;			/*	Doc action ID which has completed execution */
	DWORD ExecuteStatus;	/*	Execution status, 0 = SUCCESS, Non-zero internal failure. */
	DWORD Unused[3];		/* Future use */
	} NOTES_DOCACTION_DONE_MSG;





/* If you want your OLE object to receive the NOTES_DOC_INFO_MSG message when
	the Notes document containing your objects is printed, then your 
	application must handle an OleActivate() using the predefined 
	printing verb named "Print" (for USA), or translated to the localized
	country translation, which matches that of the translation used 
	by Notes.  Notes actually stores the localized string "Print" in its
	string resources, and does not actually used the constant below anywhere
	in compiled code.  However, its defined below as a mere convienience to
	Notes API developers as a reference. */
	

#define NOTES_OLE_PRINT_VERB_USA	"Print"



#endif

#ifdef __cplusplus
}
#endif


