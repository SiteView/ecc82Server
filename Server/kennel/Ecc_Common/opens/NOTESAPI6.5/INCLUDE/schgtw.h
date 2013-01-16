#ifndef SCHGTW_DEFS
#define SCHGTW_DEFS

#ifndef SCHEDULE_DEFS
#include "schedule.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SCHMSG_INPUTQ_PREFIX    "MQ$ICSGTW"
#define SCHMSG_OUTPUTQ_PREFIX   "MQ$OCSGTW"

/* For R6 we extended the Message Queue info so we are creating
** a new gateway name for Calendar Connector (CalConn) to use
** so that old ones will still exist and not cause any crashes
** if they attempt API calls.
*/

#define NOTESSCHEDGATEWAY       "NOTESSCHEDGATEWAY"
#define NOTESSCHEDGATEWAYV2     "NOTESSCHEDGATEWAYV2"

/* Message priority */

#define SCHMSG_RQST_PRIORITY    0x0001
#define SCHMSG_REPLY_PRIORITY   SCHMSG_RQST_PRIORITY

/* CS (Calendaring and Scheduling) function request codes  */

#define SCHFUNC_RQST_SCHED        0x0001L    /* request schedules */

/* Request/Reply msg structure */

typedef struct {                    /* preceded by CSOBJ_HEADER */
    DWORD       dwFlags;            /* Flags defined as follows */
    WORD        wFunction;          /* function name */
#define SCHMSG_EndOfTransaction     0x00000001L    /* last buffer in message */
#define SCHMSG_LastBuffer           0x00000001L    /* last buffer in message */
#define SCHMSG_ReplyToRqst          0x00010000L
    HCNTNR      hMsgCntnr;          /* handle to a container object */
    STATUS      error;
    TIMEDATE    tdRequestQueued;    /* time request was queued */
    DWORD       spare[5];
} SCHMSG;                    

/* SCH reply/request objects */

typedef struct {
    DWORD           dwOptions;      /* SCHRQST_xxx option codes (see schedule.h)*/
    WORD            wNumHops;       /* Number of hops so far */
    TIMEDATE_PAIR   Interval;       /* Interval */
    UNID            ApptUnid;       /* UNID of appt to ignore busytime for */
    TIMEDATE        ApptOrigDate;   /* (Orginizer 2.x) the orig date of ignored appt */
    DWORD           reserved[10];
    WORD            wClientNamesSize;/* Queryer's name length */
    WORD            wServerNameSize;/* Notes server name length (includes term.) */
/*    Followed by Client name doing the query */
/*    Followed by name of Notes server to forward this request to (NULL terminated). */
} RQST_SCHED_OBJ;

/* For R6 we added a few more items that need to be sent to the gateway /
** connector so in order to be backwards compatable for non-Notes gateways
** we have to define a new request format that will be used for
** non-Notes gateways (at least for the short term).
**
** We have a bit of a dilemma on how to best deal with foreign gateways that 
** may OR may not be hip to our enhanced queries.  Since many gateways lag
** our releases a bit, for the short term we will send Gateways the
** older RQST_SCHED_OBJ format and will use the RQST_SCHED_OBJ2 format
** for Notes gateways!
*/
typedef struct {
    DWORD           dwOptions;      /* SCHRQST_xxx option codes (see schedule.h)*/
    WORD            wNumHops;       /* Number of hops so far */
    TIMEDATE_PAIR   Interval;       /* Interval */
    UNID            ApptUnid;       /* UNID of appt to ignore busytime for */
    TIMEDATE        ApptOrigDate;   /* (Orginizer 2.x) the orig date of ignored appt */
    DWORD           reserved[10];
    WORD            wClientNamesSize;/* Queryer's name length */
    WORD            wServerNameSize;/* Notes server name length (includes term.) */
    /* The rest is new as of Build 173 */
    DWORD           dwRequestHdrLen;    /* Size of this request header */
    DWORD           dwDetailListSize;   /* Size of detailed item list  */
    DWORD           dwiCalListSize;     /* Size of iCalendar list */
    DWORD           dwWebUserNameLen;   /* Length of Web User Name to Follow */
    DWORD           dwWebPasswordLen;   /* Length of Web Password to Follow  */
    DWORD           Spare[10];          /* Keep a bunch of extra space - BAK */
/* Followed by:
**  List of Client names (or proxy) doing the query (may be NULL),
**  Name of Notes server to forward this request to (NULL terminated).
**  List of item names to get detailed information from (may be NULL),
**  List of names/URLs pairs to get iCalendar schedules from (may be NULL)
**  Web User Name to send to authenticating proxy, if any (may be NULL),
**  Web Password to send to authentiating prox, if any (may be NULL)
**
** Note that the dwRequestHdrLen is used so that if we need to expand
** the SCHED_REQ size in the future for some reason, the older (as of R6)
** receiver can parse what they know and skip the newer stuff that MUST
** ALWAYS be appended to the structure (but that comes before the expected 
** data listed above!!)
*/
} RQST_SCHED_OBJ2;

/* Functions on containers */

STATUS LNPUBLIC SchContainer_FreeRequest( 
    HANDLE      hCntnr, 
    HCNTNROBJ   hRqst );

STATUS LNPUBLIC SchContainer_GetRequest(
    HANDLE          hCntnr, 
    HCNTNROBJ FAR   *rethRqst, 
    void FAR * FAR  *retpRqst, 
    LIST FAR * FAR  *retpNameList, 
    LIST FAR * FAR  *repClientNames, 
    char FAR * FAR  *retpNS );

STATUS LNPUBLIC SchContainer_GetRequestExt(
    HANDLE          hCntnr, 
	WORD			wVersion,
    HCNTNROBJ FAR   *rethRqst, 
    void FAR * FAR  *retpRqst, 
    LIST FAR * FAR  *retpNameList, 
    LIST FAR * FAR  *repClientNames, 
    char FAR * FAR  *retpNS,
	LIST FAR * FAR  *retpDetails,
	LIST FAR * FAR  *retpiCal,
	char FAR * FAR  *retpAuthUsername,
	char FAR * FAR  *retpAuthPassword );

/* Functions on message queues */

STATUS LNPUBLIC SchMsgQHandles_New(
    char FAR        *szServerName, 
    BOOL            fCreate,
    MQHANDLE FAR    *rethInputQ, 
    MQHANDLE FAR    *rethOutputQ );

void LNPUBLIC SchMsgQHandles_Free(
    MQHANDLE    hInputQ, 
    MQHANDLE    hOutputQ );

/* Functions on containers */

STATUS LNPUBLIC SchContainer_DupHandle(
    HCNTNR      hOrigCntnr, 
    HCNTNR FAR  *phDup );

/* Functions on schedules */

STATUS LNPUBLIC Schedule_Free(
    HCNTNR      hCntnr, 
    HSCHEDULE   hSched );
    
STATUS LNPUBLIC Schedule_NewFromSchedList(
    HCNTNR              hCntnr, 
    TIMEDATE_PAIR FAR   *pInterval, 
    SCHED_LIST FAR      *pSchedList,
    char FAR            *pUsername,
    STATUS              errStatus, 
    DWORD               dwErrGateway,
    HSCHEDULE FAR       *rethSched );


STATUS LNPUBLIC Schedule_AddSchedList(
    HCNTNR              hCntnr, 
    HCNTNROBJ           hSched,
    TIMEDATE_PAIR FAR   *pInterval, 
    SCHED_LIST FAR      *pSchedList,
    HCNTNROBJ FAR       *rethSchedList );


#ifdef __cplusplus
}
#endif
#endif /* SCHGTW_DEFS */
