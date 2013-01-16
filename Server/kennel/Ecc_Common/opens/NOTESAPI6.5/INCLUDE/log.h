#ifdef __cplusplus
extern "C" {
#endif

/* Log Routine Definitions */

#ifndef	LOG_DEFS
#define	LOG_DEFS


#ifndef NSF_DATA_DEFS
#include "nsfdata.h"					/* Get DBHANDLE */
#endif

#define LOG_EVENT_ENTRY					1	
#define LOG_EVENT_ENTRY_NO_TIMESTAMP	2
#define LOG_EVENT_ENTRY_NO_CONSOLE		3
#define LOG_ACTIVITY_ENTRY				4
#define MAX_RESERVED_LOG_ENTRY		4

/*	Public log flags. */

#define	LOG_LEAVE_LOCKED				0x0002
#define	LOG_AUTO_ROLLOVER				0x0004
#define	LOG_TO_CONSOLE					0x0008
#define	LOG_ADD_TIMESTAMP				0x0010
#define	LOG_FORMATTED_TEXT				0x0020	/* Text is already formatted */
#define LOG_ITEM_NONSUMMARY 			0x0040  /* Make the Item non summary */
STATUS LNPUBLIC LogCreateEntry (char far *FormName, WORD far *retLogEntry);
STATUS LNPUBLIC LogCompleteEntry (WORD LogEntry);
void   LNPUBLIC LogCloseEntry (WORD LogEntry);
STATUS LNPUBLIC LogAddTimeItem (WORD LogEntry, WORD Flags, char far *ItemName, TIMEDATE far *Time);
STATUS LNPUBLIC LogAddNumberItem (WORD LogEntry, WORD Flags, char far *ItemName, NUMBER far *Number);
STATUS LNPUBLIC LogAddTextlistItem (WORD LogEntry, WORD Flags, char far *ItemName, char far *Text);
STATUS LNVARARGS  LogAddText (WORD LogEntry, WORD Flags, char far *ItemName, HMODULE hModule, STATUS Message, char far *FormatString,...);
STATUS LNVARARGS  LogEvent (STATUS StringID, HMODULE hModule, STATUS AdditionalErrorCode,...);
STATUS LNVARARGS  LogEventText (char far *String, HMODULE hModule, STATUS AdditionalErrorCode,...);
STATUS LNVARARGS  LogEventTextNoConsole (char far *String, HMODULE hModule, STATUS AdditionalErrorCode,...);

/* Public activity logging API */

typedef struct
	{
	NOTEID 	NoteID;	
	DWORD 	RecordPos;
	}ACTIVITYSTREAMPOSITION;

/* Public activity logging API flags */

/* Flag which indicates that a record is a checkpointed record */
#define ACTFLAGS_CHECKPOINTED_RECORD	0x00000001
/* Indicates the final record in a series of checkpointed records */
#define ACTFLAGS_CHECKPOINT_FINAL		0x00000002
/* Causes LogEnumActivityStream to pass only the last instance of a checkpointed record */
#define ACTFLAGS_COLLAPSE_CHECKPOINTS	0x00000004
/* Passed via the stream callback function flags argument. Indicates that the record
   was truncated. Current record size limit is 20K*/
#define ACTFLAGS_RECORD_TRUNCATED		0x00000008



typedef STATUS (LNCALLBACKPTR ACTIVITYSTREAMACTION)(
	const char *DescName, 
	DWORD DescIdx,
	DWORD Flags,
	WORD  PrimaryKey,
   	const TIMEDATE* TimeStamp,
	void *pActivityRecord,
	void *pUserData);

STATUS LNPUBLIC LogOpenActivityStream(
	void** pStreamCtx,
	const char* ServerName,
	const char* LogPath, 
	const char* ActivityName,
	DWORD Flags,
	const TIMEDATE_PAIR* DateRange);
	

STATUS LNPUBLIC LogEnumActivityStream(
	void* pStreamCtx, 
	ACTIVITYSTREAMACTION ActionRoutine,
	void* pUserData,
	ACTIVITYSTREAMPOSITION* Position,
	DWORD PositionSize);

STATUS LNPUBLIC LogSetActivityStreamPosition(void* pStreamCtx, const ACTIVITYSTREAMPOSITION* Position, DWORD PositionSize);

STATUS LNPUBLIC LogCloseActivityStream(void* pStreamCtx);

#endif


#ifdef __cplusplus
}
#endif

