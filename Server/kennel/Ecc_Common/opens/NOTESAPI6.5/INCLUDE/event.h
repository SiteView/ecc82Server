#ifndef EVENT_DEFS
#define EVENT_DEFS

#ifdef __cplusplus
extern "C" {
#endif


#ifndef POOL_DEFS
#include "pool.h"					/* need BLOCKID */
#endif

#ifndef NAME_DEFS
#include "names.h"					/* need MAXUSERNAME */
#endif

#ifndef MISC_DEFS
#include "misc.h"					/* need MAXSPRINTF */
#endif

#ifndef NSF_DATA_DEFS
#include "nsfdata.h"					/* need MAXSPRINTF */
#endif

STATUS LNPUBLIC EventQueueAlloc(char far * QueueName);
/*
*	EventQueueAlloc - Create an event queue with the given name.  If one
*			already exists, return an error.
*
*	Inputs:
*		QueueName - ASCIIZ name of queue to create (32 chars including NULL MAX)
*
*	Outputs:
*		If queue with that name does not already exist, creates the queue,
*		else returns error.
*
*/
	/*
	Each event consumer calls EventQueueAlloc at startup to create a event
	queue with a specific name to receive events.
	*/


void LNPUBLIC EventQueueFree(char far * QueueName);
/*
*	EventQueueFree - destroys the queue and deallocates the memory it used.
*
* 	Inputs:
*		QueueName - ASCIIZ name of queue to destroy
*
*	Outputs:
*		none
*/
	/*
	Called at shutdown time by each event consumer.
	*/


STATUS LNPUBLIC EventQueuePut(char far *QueueName,
								char far *OriginatingServer,
								WORD Type,
								WORD Severity,
								TIMEDATE far *EventTime,
								WORD FormatSpecifier,
								WORD EventDataLength,
								void far *EventSpecificData);
/*
*	EventQueuePut - puts a event into a queue.  
*
*
* Inputs:
*	QueueName - (ASCIIZ) name of queue to receive this event
*	OriginatingServer - (ASCIIZ) name of server where event
*						 occured (if, NULL, uses the current server name)
*	Type - one of: 	EVT_COMM
*					EVT_SECURITY
*					EVT_MAIL
*					EVT_RESOURCE
*					EVT_MISC
*					EVT_ALARM
*					EVT_SERVER
*					EVT_UNKNOWN
*
*	Severity - one of:
*					SEV_FATAL
*					SEV_FAILURE
*					SEV_WARNING1
*					SEV_WARNING2
*					SEV_NORMAL
*					SEV_UNKNOWN
*
*	EventTime - event's temporal locus
*	FormatSpecifier - format of data in EventSpecificData
*	EventDataLength - number of bytes in EventSpecificData
*	EventSpecificData - event info
*
* Outputs:
*	Event is placed in the specified queue.
*	(routine status)
*
*/
	/*
	Event producers call this routine whenever an event occurs that
	anyone may be interested in.  If no event consumer has requested
	notification of a particular event, the event is discarded.
	*/


STATUS LNPUBLIC EventQueueGet(char far *QueueName, HANDLE far *rethEvent);
/*
*	EventQueueGet	 - removes an event from a queue and returns the
*			handle to it's object.  It is the caller's responsibility
*			to free it when through.
*
* Inputs:
*	QueueName - name of queue to search for events
*
* Outputs:
*	*hEvent - handle to event object.  NULLHANDLE if queue is empty.
*	(retstatus) - ERR_EVTQUEUE_EMPTY if empty queue, else NOERROR
*				if something dequeued
*/
	/*
	Event consumers call this routine to dequeue any events
	presently in their queue.  If the queue is empty, the routine
	returns ERR_EVTQUEUE_EMPTY.  Else, it returns NOERROR, and
	stuffs the output parameter with the handle to the dequeued event.
	*/

STATUS LNPUBLIC EventRegisterEventRequest(WORD EventType, WORD EventSeverity, char far *QueueName, char far *DestName);
/*
*	EventRegisterEventRequest
*
*	Inputs:
*		EventType - type of event to notify of
*		EventSeverity - severity of event to notify of
*		QueueName - name of queue that desires notification
*		DestName - name of person/database to address event to
*
*	Outputs:
*		(none)
*		routine = status
*/
	/*
	At registration time, an event consumer calls this routine
	once for each class and type of event that it is interested in.
	*/


STATUS LNPUBLIC EventDeregisterEventRequest(WORD EventType, WORD EventSeverity, char far *QueueName);
/*
*	EventDeregisterEventRequest
*
*	Inputs:
*		EventType - type of event to discontinue notification of
*		EventSeverity - severity of event to discontinue notification of
*		QueueName - name of queue that desires no longer desires notification
*
*	Outputs:
*		(none)
*		routine = status
*/
	/*
	Called by process to discontinue notification of particular events
	*/

BOOL LNPUBLIC EventGetDestNameEx(WORD EventType, WORD EventSeverity, char *ErrorCode, char *ErrorText, char *QueueName, char *DestName, WORD DestNameSize);
BOOL LNPUBLIC EventGetDestName(WORD EventType, WORD Severity, char far *QueueName, char far *DestName, WORD DestNameSize);
/*
*	
*
*	Inputs:
*		EventType - type of event 
*		EventSeverity - severity of event 
*		QueueName - name of queue that desires information
*		DestName - buffer to receive name of person/database to address event to
*		DestNameSize - size of ret buffer
*
*	Outputs:
*		DestName - contains name of destination person/database
*		routine = TRUE if dest name is set
*/
	/*
	Called by a process to obtain the destination for these events for this
	queue.  For mail, would return a user or group name.  For logging, would
	return a database name, or a server and database name, etc.
	*/



/*	flags for EventQueuePut	*/
#define	EVT_UNKNOWN		0
#define	EVT_COMM		1
#define	EVT_SECURITY	2
#define	EVT_MAIL		3
#define	EVT_REPLICA		4
#define	EVT_RESOURCE	5
#define	EVT_MISC		6
#define	EVT_SERVER		7
#define	EVT_ALARM		8
#define	EVT_UPDATE		9
#define	EVT_DATABASE	10
#define	EVT_NETWORK		11
#define	EVT_COMPILER	12
#define	EVT_ROUTER		13
#define	EVT_AGENT		14
#define	EVT_CLIENT		15
#define	EVT_ADDIN		16
#define	EVT_ADMINP		17
#define	EVT_WEB_CODE	18
#define	EVT_NNTP_CODE	19
#define	EVT_FTP_CODE	20
#define	EVT_CODE		21
#define	EVT_LDAP		22
#define	EVT_MONITOR		23
#define	MAX_TYPE		24

/*	event type names	*/
#define	UNKNOWN_NAME    "Unknown"						   
#define	COMM_NAME	    "Comm"							   
#define	SECURE_NAME	    "Security"						   
#define	MAIL_NAME	    "Mail"							   
#define	REPLICA_NAME	"Replica"						   
#define	RESOURCE_NAME	"Resource"						   
#define	MISC_NAME	    "Misc"							   
#define	SERVER_NAME	    "Server"						   
#define	ALARM_NAME	    "Statistic"						   
#define	UPDATE_NAME	    "Update"						   
#define	DATABASE_NAME	"DataBase"						   
#define	NETWORK_NAME	"Network"						   
#define	COMPILER_NAME	"Compiler"						   
#define	ROUTER_NAME	    "Router"						   
#define	AGENT_NAME	    "Agent"							   
#define	CLIENT_NAME	    "Client"						   
#define	ADDIN_NAME	    "Addin"							   
#define	ADMINP_NAME	    "AdminP"						   
#define	WEB_NAME	    "Web"							   
#define	NNTP_NAME	    "News"							   
#define	FTP_NAME	    "Ftp"							   
#define	CODE_NAME	    "ErrorCode"						   
#define	LDAP_NAME	    "LDAP"						   
#define	MONITOR_NAME	"Monitor"						   
														   
/*	Severity FLAGS	*/
#define	SEV_UNKNOWN		0
#define	SEV_FATAL		1
#define	SEV_FAILURE		2
#define	SEV_WARNING1	3
#define	SEV_WARNING2	4
#define	SEV_NORMAL		5
#define	MAX_SEVERITY	7

#define	FATAL_NAME		"Fatal"
#define	FAILURE_NAME	"Failure"
#define	WARNING1_NAME	"Warning (high)"
#define	WARNING2_NAME	"Warning (low)"
#define	NORMAL_NAME		"Normal"

/*	FormatSpecifier FLAGS	*/

#define	FMT_UNKNOWN			0
#define	FMT_TEXT			1
#define	FMT_ERROR_CODE		2
#define	FMT_ERROR_MSG		3

/*	Version field values specified in following structure */

#define EVENT_VERSION	2

/*	Event structure	*/

typedef struct 	{
		DWORD Links[3];				/* Reserved - used to link this struct onto queues */
		char OriginatingServerName[MAXUSERNAME]; /* Server name (only if event relayed to another server) */
		WORD Version;				/* EVENT_VERSION */
		WORD ErrorCode;				/* Error code (any values) associated with this event */
		WORD AdditionalErrorCode;	/* Additional Error code (any values) associated with this event */
		WORD AddinNameLength;		/* Length of AddinName at end of structure */
		WORD Type;					/* EVT_xxx */
		WORD Severity;				/* SEV_xxx */
		TIMEDATE EventTime;			/* Time/date event was generated */
		WORD FormatSpecifier;		/* FMT_xxx (format of event data which follows) */
		WORD EventDataLength;		/* Length of event data which follows */
		BYTE EventSpecificData;		/* (First byte of) Event Data which follows... */
									/* (AddinName string follows...) */
		} EVENT_DATA;


#define EVENT_MAIN_QUEUE_NAME	"EventDispatcher"
#define EVENT_SNMP_QUEUE_NAME	"TRAP"



#ifdef __cplusplus
}

#endif

#endif

