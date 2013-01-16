
/*	Notes Query On-Disk Structure Definitions
	Record format used in saved queries and actions. */


#ifndef QUERYODS_DEFS
#define QUERYODS_DEFS

#ifndef ODS_DEFS
#include "ods.h"
#endif

/*	Query header structure. The on-disk format for a query consists of
	a query header structures followed by a number of query term
	structures. */

#define QUERY_FLAG_INCLUDEDESCENDANTS	0x00000001	/*	Include all children */

typedef struct
	{
	BSIG Header;
	DWORD dwFlags;						/*	Flags for query */
	} CDQUERYHEADER;

/*	Query Text Term structure. This term represents a simple string
	in the Notes Full Text Search Query Syntax */

#define TEXTTERM_FLAG_RAW		0x00000001	/*	String is a Notes Full Text Search Query String */
#define TEXTTERM_FLAG_VERITY	0x00000002	/*	String is in Verity Syntax */
#define TEXTTERM_FLAG_AND		0x00000004	/*	String is comma-separated list of words; AND assumed */
#define TEXTTERM_FLAG_ACCRUE	0x00000008	/*	String is comma-separated list of words; ACCRUE assumed */
#define TEXTTERM_FLAG_NEAR		0x00000010	/*	String is comma-separated list of words; NEAR assumed */
#define TEXTTERM_FLAG_PLAINTEXT	0x00000020	/*	This object is displayed as plain text */

#define MAXTEXTTERMCOUNT		10

typedef struct
	{
	WSIG Header;
	DWORD dwFlags;						/*	flags for this string */
	DWORD dwLength[MAXTEXTTERMCOUNT];	/*	length of strings */
										/*	strings follow */
	} CDQUERYTEXTTERM;

/*	Query by Field structure. */

#define QUERYBYFIELD_FLAG_BYDATE	0x00000001	/*	Search on modified and created date */
#define QUERYBYFIELD_FLAG_BYAUTHOR	0x00000002	/*	Search by author */

#define QUERYBYFIELD_OP_GREATER			1	/*	Greater than value */
#define QUERYBYFIELD_OP_LESS			2	/*	Less than value */
#define QUERYBYFIELD_OP_NOTEQUAL		3	/*	Not equal to value */
#define QUERYBYFIELD_OP_BETWEEN			4	/*	Between Date1 and Date2 */
#define QUERYBYFIELD_OP_NOTWITHIN		5	/*	Not between Date1 and Date2 */
#define QUERYBYFIELD_OP_EQUAL			6	/*	Equal to value */
#define QUERYBYFIELD_OP_CONTAINS		7	/*	Contains value */
#define QUERYBYFIELD_OP_INTHELAST		8	/*	In the last n days */
#define QUERYBYFIELD_OP_INTHENEXT		9	/*	In the next n days */
#define QUERYBYFIELD_OP_OLDERTHAN		10	/*	Older than n days */
#define QUERYBYFIELD_OP_DUEIN			11	/*	Due more than n days from now */
#define QUERYBYFIELD_OP_DOESNOTCONTAIN	12	/*	Does not contain */

typedef struct
	{
	WSIG Header;
	DWORD dwFlags;				/*	QUERYBYFIELD_FLAG_XXX */
	WORD wDataType;				/*	data type of field to search */
	WORD wOperator;				/*	operation QUERYBYFIELD_OP_???? */
	TIMEDATE Date1;				/*	first operand */
	TIMEDATE Date2;				/*	second operand */
	ALIGNED_NUMBER Number1;			/*	first operand */
	ALIGNED_NUMBER Number2;			/*	second operand */
	WORD wFieldNameLen;			/*	length of field name */
	WORD wValueLen;				/*	length of value */
								/*	field name follows */
								/*	value follows */
	} CDQUERYBYFIELD;

/*	Formula query */

#define QUERYFORMULA_FLAG_PLAINTEXT	0x00000001	/*	Show formula as plain text */

typedef struct
	{
	WSIG Header;
	DWORD dwFlags;				/*	flags */
	WORD wFormulaLen;			/*	Length of formula */
	} CDQUERYFORMULA;

/*	Query by Form */
	
typedef struct
	{
	WSIG Header;
	DWORD dwFlags;
	WORD wFieldCount;			/*	Number of fields that follow */
	WORD wFormNameLen;			/*	Length of form name */

								/*	ODS_ASSISTFIELDSTRUCTs follow */
								/*	Form name follows */
	} CDQUERYBYFORM;

/*	Query by Folder */

#define QUERYBYFOLDER_FLAG_PRIVATE		0x00000001	/*	Folder is private */

typedef struct
	{
	WSIG Header;
	DWORD dwFlags;				/*	Flags */
	WORD wFolderNameLen;		/*	Length of folder name */

								/*	Folder name follows */
	} CDQUERYBYFOLDER;

/*	Query using form */

typedef struct
	{
	WSIG Header;
	DWORD dwFlags;				/*	Flags */

								/*	LIST structure follows */
	} CDQUERYUSESFORM;

/*	Query by Verity Topic */

typedef struct
	{
	WSIG Header;

	DWORD dwFlags;				/*	Flags */
	WORD wTopicQueryLen;		/*	Length of topic query */
	WORD wDataLen;				/*	Length of data */
								/*	Topic query follows */
								/*	Data follows */
	} CDQUERYTOPIC;

/*	Action header structure. The on-disk format for actions consist of
	an action header structure followed by a number of action term
	structures. */

typedef struct
	{
	BSIG Header;
	} CDACTIONHEADER;

/*	Modify Field Action. This action type modifies a single field. */

#define MODIFYFIELD_FLAG_REPLACE	0x00000001	/*	Replace field value */
#define MODIFYFIELD_FLAG_APPEND		0x00000002	/*	Append field value */

typedef struct
	{
	WSIG Header;
	DWORD dwFlags;				/*	flags MODIFYFIELD_FLAG_??? */
	WORD wFieldNameLen;			/*	Length of field name to modify */
	WORD wValueLen;				/*	Length of new value */

								/*	Field name follows */
								/*	Value follows */
	} CDACTIONMODIFYFIELD;

/*	Reply Action. */

#define ACTIONREPLY_FLAG_REPLYTOALL		0x00000001	/*	Reply to all (otherwise, just to sender) */
#define ACTIONREPLY_FLAG_INCLUDEDOC		0x00000002	/*	Include copy of document */
#define ACTIONREPLY_FLAG_SAVEMAIL		0x00000004	/*	Save copy */
#define ACTIONREPLY_FLAG_NOAGENTREPLY	0x00000008	/*	Do not reply to agent-generated mail */
#define ACTIONREPLY_FLAG_REPLYONCE		0x00000010	/*	Only reply once per sender */

typedef struct
	{
	WSIG Header;
	DWORD dwFlags;
	WORD wBodyLen;				/*	Length of body text */

								/*	Body text follows */
	} CDACTIONREPLY;

/*	Formula Action. */

#define ACTIONFORMULA_FLAG_SELECTDOCS	0x00000001	/*	Select documents */
#define ACTIONFORMULA_FLAG_NEWCOPY		0x00000002	/*	Create a new copy before modifying */

typedef struct
	{
	WSIG Header;
	DWORD dwFlags;
	WORD wFormulaLen;			/*	Length of formula */
								/*	Formula follows */
	} CDACTIONFORMULA;

/*	LotusScript */

typedef struct
	{
	WSIG Header;
	DWORD dwFlags;
	DWORD dwScriptLen;			/*	Length of script */
								/*	Script follows */
	} CDACTIONLOTUSSCRIPT;

/*	Send mail */

#define ACTIONSENDMAIL_FIELDCOUNT			5
#define ACTIONSENDMAIL_TOFIELD				0
#define ACTIONSENDMAIL_CCFIELD				1
#define ACTIONSENDMAIL_BCCFIELD				2
#define ACTIONSENDMAIL_SUBJECTFIELD			3
#define ACTIONSENDMAIL_BODYFIELD			4

#define ACTIONSENDMAIL_FLAG_INCLUDEDOC		0x00000001	/*	Include matching document */
#define ACTIONSENDMAIL_FLAG_INCLUDELINK		0x00000002	/*	Include doclink to document */
#define ACTIONSENDMAIL_FLAG_SAVEMAIL		0x00000004	/*	save copy */
#define ACTIONSENDMAIL_FLAG_TOFORMULA		0x00000008	/*	To field is a formula */
#define ACTIONSENDMAIL_FLAG_CCFORMULA		0x00000010	/*	cc field is a formula */
#define ACTIONSENDMAIL_FLAG_BCCFORMULA		0x00000020	/*	bcc field is a formula */
#define ACTIONSENDMAIL_FLAG_SUBJECTFORMULA	0x00000040	/*	Subject field is a formula */

typedef struct
	{
	WSIG Header;
	DWORD dwFlags;				/*	flags */

	/*	Lengths for each of the fields */

	WORD wFieldLen[ACTIONSENDMAIL_FIELDCOUNT];

								/*	To field follows */
								/*	cc field follows */
								/*	bcc field follows */
								/*	Subject field follows */
	} CDACTIONSENDMAIL;

/*	copy */

#define ACTIONDBCOPY_FLAG_MOVE				0x00000001	/*	Remove document from original database */

typedef struct
	{
	WSIG Header;
	DWORD dwFlags;				/*	flags */
	WORD wServerLen;			/*	length of server name */
	WORD wDatabaseLen;			/*	length of database filespec */

								/*	server name follows */
								/*	database filespec follows */
	} CDACTIONDBCOPY;

/*	delete */

typedef struct
	{
	BSIG Header;
	DWORD dwFlags;				/*	flags */
	} CDACTIONDELETE;

/*	Read Marks */

typedef struct
	{
	BSIG Header;
	DWORD dwFlags;
	} CDACTIONREADMARKS;

/*	Action by form */

#define ACTIONBYFIELD_OP_REPLACE		1	/*	Replace value */
#define ACTIONBYFIELD_OP_APPEND			2	/*	Append value as list element */
#define ACTIONBYFIELD_OP_REMOVE			3	/*	Remove value from list */

typedef struct
	{
	WORD wTotalLen;				/*	Total length of field structure (includes padding) */
	WORD wOperator;				/*	Query or action operator (ACTIONBYFIELD_OP_?? or QUERYBYFIELD_OP_?? */
	WORD wFieldNameLen;			/*	Field name length */
	WORD wValueLen;				/*	Length of value */
	WORD wValueDataType;		/*	Datatype of value */
	WORD wSpare;
								/*	Field name follows */
								/*	Value follows */
								/*	Padded to WORD boundary */
	} ODS_ASSISTFIELDSTRUCT;

typedef struct
	{
	WSIG Header;
	DWORD dwFlags;
	WORD wFieldCount;			/*	Number of fields that follow */
	WORD wFormNameLen;			/*	Length of form name */

								/*	ODS_ASSISTFIELDSTRUCTs follow */
								/*	Form name follows */
	} CDACTIONBYFORM;

/*	Folder Action */

#define ACTIONFOLDER_FLAG_NEWFOLDER		0x00000001	/*	Create new folder */
#define ACTIONFOLDER_FLAG_PRIVATEFOLDER	0x00000002	/*	Folder is private */

typedef struct
	{
	WSIG Header;
	DWORD dwFlags;				/*	flags */
	WORD wFolderNameLen;		/*	length of folder name */
	WORD wSpare;
								/*	folder name follows */
	} CDACTIONFOLDER;

/*	Newsletter action */

#define ACTIONNEWSLETTER_FLAG_SUMMARY		0x00000001	/*	Summary of docs (with DocLinks) */
#define ACTIONNEWSLETTER_FLAG_GATHER		0x00000002	/*	Gather at least n before mailing */
#define ACTIONNEWSLETTER_FLAG_INCLUDEALL	0x00000004	/*	Include all notes when mailing out multiple notes */

/* 	Newsletter formula flags are same as sendmail flags, so...
   	Do not use the following flag values for newsletter flags.: */

/* #define ACTIONSENDMAIL_FLAG_TOFORMULA		0x00000008	To field is a formula */
/* #define ACTIONSENDMAIL_FLAG_CCFORMULA		0x00000010	cc field is a formula */
/* #define ACTIONSENDMAIL_FLAG_BCCFORMULA		0x00000020	bcc field is a formula */
/* #define ACTIONSENDMAIL_FLAG_SUBJECTFORMULA	0x00000040	Subject field is a formula */

typedef struct
	{
	WSIG Header;
	DWORD dwFlags;				/*	flags */
	DWORD dwGather;				/*	gather at least n documents */
	WORD wViewNameLen;			/*	View to use to display data */
	WORD wSpare;
	WORD wFieldLen[ACTIONSENDMAIL_FIELDCOUNT];

								/*	view name follows */
	} CDACTIONNEWSLETTER;

/*	Run agent action */

typedef struct
	{
	WSIG Header;
	DWORD dwFlags;				/*	flags */
	WORD wAgentNameLen;			/*	Agent name length */
	WORD wSpare;
								/*	agent name follows */
	} CDACTIONRUNAGENT;

/*	Send document action */

typedef struct
	{
	BSIG Header;
	DWORD dwFlags;
	} CDACTIONSENDDOCUMENT;

/*	Java agent action */

typedef struct
	{
	WSIG Header;
	WORD wClassNameLen;			/*	Agent name length */
	WORD wCodePathLen;
	WORD wFileListBytes;
	WORD wLibraryListBytes;
	WORD wSpare[1];
	DWORD dwSpare[1];
								/*	Strings follows */
	} CDACTIONJAVAAGENT;


/*	Assistant ODS */

#define ASSISTODS_FLAG_HIDDEN			0x00000001	/*	TRUE if manual assistant is hidden */
#define ASSISTODS_FLAG_NOWEEKENDS		0x00000002	/*	Do not run on weekends */
#define ASSISTODS_FLAG_STOREHIGHLIGHTS	0x00000004	/*	TRUE if storing highlights */
#define ASSISTODS_FLAG_MAILANDPASTE		0x00000008	/*	TRUE if this is the V3-style mail and paste macro */
#define ASSISTODS_FLAG_CHOOSEWHENENABLED	0x00000010	/*	TRUE if server to run on should be chosed when enabled */

/*	The assistant schedule is kept in the following structure. Note that
	because there are different types of schedules, the meaning of the
	fields in the structure is sometimes ambiguous. Use the following
	rules:

	When iTriggerType == ASSISTTRIGGER_TYPE_SCHEDULED:
		This trigger type means that the assistant should run based on a
		timed schedule.

		iSearchType: Any of the following are valid: ASSISTSEARCH_TYPE_ALL,
			TYPE_NEW, and TYPE_MODIFIED.

		iIntervalType: This is the type of interval: minutes, days, etc.
			Any of the following are valid: ASSISTINTERVAL_TYPE_MINUTES,
			TYPE_DAYS, TYPE_WEEK, TYPE_MONTH.

		iInterval: This is the interval with which the assistant will run.
			This number is expressed in units of iIntervalType. For example,
			if iIntervalType == TYPE_DAYS, and iInterval is 2, then the
			assistant will execute once every two days.

		dwTime1 and dwTime2: These variables depend on iIntervalType:
			Note: Time of day units are TICKS since midnight GMT,
				  ie, 2am would be 2 * 60 * TICKS_PER_MINUTE

			ASSISTANTINTERVAL_TYPE_MINUTES: dwTime1 and dwTime2 represent the
			starting and ending times of the days during which the assistant
			will run. For example, if iInterval is 30, dwTime1 is 2pm and
			dwTime2 is 11pm, then the assistant will run every 30 minutes
			starting at 2pm until 11pm. If the fNoWeekends flag is set, then
			the assistant will not run on weekends. If dwTime1 is 0 and dwTime2
			is TICKS_IN_DAY, then it means that the agent should run all day.

			ASSISTANTINTERVAL_TYPE_DAYS: dwTime1 represents the time of the
			day at which the assistant will run. If the fNoWeekends flag is set, then
			the assistant will not run on weekends.

			ASSISTANTINTERVAL_TYPE_WEEK: dwTime1 represents the time of the
			day at which the assistant will run; dwTime2 represents the
			day of the week (1-7) on which it will run. For example, if
			dwTime1 is 10pm and dwTime2 is 4, then the assistant will run
			once a week on Wednesday at 10pm. The fNoWeekends flags is
			ignored.

			ASSISTANTINTERVAL_TYPE_MONTH: dwTime1 represents the time of the
			day on which the assistant will run; dwTime2 is the day of the
			month (1-31). For example, if dwTime1 is 5pm and dwTime2 is 
			27, then the assistant will run once per month on the 27th at
			5pm. The fNoWeekends flags is ignored.

	iTriggerType == ASSISTTRIGGER_TYPE_NEWMAIL:
		This tigger type means that the assistant should run whenever new
		mail is delivered, there are no time constraints.

		iSearchType: This parameter is ignored since we always operate on new
			documents.

	iTriggerType == ASSISTTRIGGER_TYPE_DOCUPDATE:
		This tigger type means that the assistant should run whenever a
		data class document is updated (via NSFNoteUpdate) in the 
		indicated database, there are no time constraints.

		iSearchType: This parameter is ignored since we always operate on new
			documents.

	iTriggerType == ASSISTTRIGGER_TYPE_PASTED:
		This trigger type means that the assistant should run when documents
		are pasted (through the UI) into the database.

		iSearchType: This parameter is ignored since we always operate on new
			documents.

	iTriggerType == ASSISTTRIGGER_TYPE_MANUAL:
		This assistant type may only be run through the UI.

		iSearchType: The following are valid: ASSISTSEARCH_TYPE_SELECTED,
			TYPE_VIEW, TYPE_ALL, TYPE_UNREAD, TYPE_PROMPT, TYPE_UI.

*/

#define ASSISTTRIGGER_TYPE_NONE				0	/*	Unknown or unavailable */
#define ASSISTTRIGGER_TYPE_SCHEDULED		1	/*	According to time schedule */
#define ASSISTTRIGGER_TYPE_NEWMAIL			2	/*	When new mail delivered */
#define ASSISTTRIGGER_TYPE_PASTED			3	/*	When documents pasted into database */
#define ASSISTTRIGGER_TYPE_MANUAL			4	/*	Manually executed */
#define ASSISTTRIGGER_TYPE_DOCUPDATE		5	/*	When doc is updated */
#define ASSISTTRIGGER_TYPE_SYNCHNEWMAIL		6	/*	Synchronous new mail agent executed by router */

#define ASSISTSEARCH_TYPE_NONE				0	/*	Unknown or unavailable */
#define ASSISTSEARCH_TYPE_ALL				1	/*	All documents in database */
#define ASSISTSEARCH_TYPE_NEW				2	/*	New documents since last run */
#define ASSISTSEARCH_TYPE_MODIFIED			3	/*	New or modified docs since last run */
#define ASSISTSEARCH_TYPE_SELECTED			4	/*	Selected documents */
#define ASSISTSEARCH_TYPE_VIEW				5	/*	All documents in view */
#define ASSISTSEARCH_TYPE_UNREAD			6	/*	All unread documents */
#define ASSISTSEARCH_TYPE_PROMPT			7	/*	Prompt user */
#define ASSISTSEARCH_TYPE_UI				8	/*	Works on the selectable object */

#define ASSISTSEARCH_TYPE_COUNT				9	/*	Total number of search types */

#define ASSISTINTERVAL_TYPE_NONE			0	/*	Unknown */
#define ASSISTINTERVAL_TYPE_MINUTES			1
#define ASSISTINTERVAL_TYPE_DAYS			2
#define ASSISTINTERVAL_TYPE_WEEK			3
#define ASSISTINTERVAL_TYPE_MONTH			4

typedef struct
	{
	WORD wVersion;				/*	Structure version */

	WORD wTriggerType;			/*	Type of trigger */
	WORD wSearchType;			/*	Type of search */
	WORD wIntervalType;			/*	Type of interval */
	WORD wInterval;				/*	Interval */
	DWORD dwTime1;				/*	depends on interval type */
	DWORD dwTime2;				/*	depends on interval type */

	TIMEDATE StartTime;			/*	Agent does not run before this time */
	TIMEDATE EndTime;			/*	Agent does not run after this time */

	DWORD dwFlags;

	DWORD dwSpare[16];
	} ODS_ASSISTSTRUCT;

typedef struct
	{
	DWORD dwFlags;				/*	flags */
	WORD wEntries;				/*	Number of entries following */
	WORD wSpare;
								/*	entries follow */
	} ODS_ASSISTRUNOBJECTHEADER;

typedef struct
	{
	DWORD dwLength;
	DWORD dwFlags;
	} ODS_ASSISTRUNOBJECTENTRY;

typedef struct
	{
	TIMEDATE LastRun;			/*	Time of last agent run */
	DWORD dwProcessed;			/*	Number of documents processed on last run */
	TIMEDATE AssistMod;			/*	Time date of last assistant modification */
	TIMEDATE DbID;				/*	DbID on which assistant was last run */
	LONG dwExitCode;			/*	Exit code when assistant was last run */
	DWORD dwSpare[4];
	} ODS_ASSISTRUNINFO;

#endif /* QUERYODS_DEFS */


