/* Calendaring and Scheduling structures */

#ifndef SCHEDULE_DEFS
#define SCHEDULE_DEFS

#ifdef __cplusplus
extern "C" {
#endif


#ifndef NSF_DEFS
#include "nsfdata.h"    /* for UNID */
#endif

#ifndef MQ_DEFS
#include "mq.h"
#endif

/* These are magic cookies */

typedef HANDLE          HCNTNR;         /* Handle to a container */
typedef DWORD           HCNTNROBJ;      /* Handle to an object in a container */
#define NULLCNTNROBJ    0L              /* NULL container  object handle */
#define HSCHEDULE       HCNTNROBJ       /* Handle to a schedule */

typedef struct {
    DWORD           reserved[8];
    DBID            dbReplicaID;        /* Users mail file replica ID */
    TIMEDATE_PAIR   Interval;           /* Events etc. are in this interval */
    DWORD           dwErrGateway;       /* Gateway error retrieving this schedule */
    STATUS          error;              /* Error retrieving this schedule */
    WORD            wReserved;          /* Unused at this time */
    WORD            wOwnerNameSize;     /* Size of owner name (includes term.)*/
                                        /* Followed by owner name */
} SCHEDULE;

/* Functions on containers */

STATUS LNPUBLIC SchContainer_New( HCNTNR FAR *rethCntnr );

void LNPUBLIC SchContainer_Free( HCNTNR hCntnr );

STATUS LNPUBLIC SchContainer_FindSchedule(
        HCNTNR              hCntnr, 
        char FAR            *pszUserName, 
        STATUS FAR          *retErrStatus, 
        DWORD FAR           *retdwErrGateway, 
        HSCHEDULE FAR       *retvbObj, 
        SCHEDULE FAR * FAR  *retpSched );

STATUS LNPUBLIC SchContainer_GetFirstSchedule(
        HANDLE              hCntnr, 
        HSCHEDULE FAR       *rethObj, 
        SCHEDULE FAR * FAR  *retpSchedule );

STATUS LNPUBLIC SchContainer_GetNextSchedule(
        HANDLE              hCntnr, 
        HSCHEDULE           hCurSchedule, 
        HSCHEDULE FAR       *rethNextSchedule, 
        SCHEDULE FAR * FAR  *retpNextSchedule );

/* Functions on schedules */

STATUS LNPUBLIC Schedule_Access(
        HCNTNR          hCntnr, 
        HSCHEDULE       hSched, 
        SCHEDULE FAR    **pretSched );

STATUS LNPUBLIC Schedule_ExtractBusyTimeRange(
        HCNTNR              hCntnr, 
        HCNTNROBJ           hSchedObj, 
        const UNID FAR      *punidIgnore,  
        TIMEDATE_PAIR FAR   *pInterval, 
        DWORD FAR           *retdwSize, 
        HANDLE FAR          *rethRange, 
        HCNTNROBJ           *rethMoreCtx );

STATUS LNPUBLIC Schedule_ExtractMoreBusyTimeRange(
        HCNTNR          hCntnr, 
        HCNTNROBJ       hMoreCtx, 
        const UNID      *punidIgnore,  
        TIMEDATE_PAIR   *pInterval, 
        DWORD           *retdwSize, 
        HANDLE          *rethRange, 
        HCNTNROBJ       *rethMore );

STATUS LNPUBLIC Schedule_ExtractFreeTimeRange(
        HCNTNR              hCntnr, 
        HCNTNROBJ           hSchedObj, 
        const UNID FAR      *punidIgnore, 
        BOOL                fFindFirstFit, 
        WORD                wDuration,
        TIMEDATE_PAIR FAR   *pInterval, 
        DWORD FAR           *retdwSize, 
        HANDLE FAR          *rethRange );

STATUS LNPUBLIC Schedule_ExtractSchedList(
        HCNTNR              hCntnr, 
        HCNTNROBJ           hSchedObj,
        TIMEDATE_PAIR FAR   *pInterval, 
        DWORD FAR           *retdwSize, 
        HANDLE FAR          *rethSchedList, 
        HCNTNROBJ FAR       *rethMore );

STATUS LNPUBLIC Schedule_ExtractMoreSchedList(
        HCNTNR              hCntnr, 
        HCNTNROBJ           hMoreCtx, 
        TIMEDATE_PAIR FAR   *pInterval, 
        DWORD FAR           *retdwSize, 
        HANDLE FAR          *rethSchedList, 
        HCNTNROBJ FAR       *rethMore );


STATUS LNPUBLIC Schedule_GetFirstDetails(
        HCNTNR                          hCntnr, 
        HSCHEDULE                       hSchedObj, 
        HCNTNROBJ                       *rethDetailObj, 
        SCHED_DETAIL_LIST FAR * FAR     *retpDetail );

STATUS LNPUBLIC Schedule_GetNextDetails(
        HCNTNR                          hCntnr, 
        HCNTNROBJ                       hDetailObj, 
        HCNTNROBJ                       *rethNextDetailObj, 
        SCHED_DETAIL_LIST FAR * FAR     *retpNextDetail );

/* Schedule Query APIs */

/* Option flags for scheduling */

#define SCHRQST_COMPOSITE       0x0001L /* Return composite sched */
#define SCHRQST_EACHPERSON      0x0002L /* Return each person's sched */
#define SCHRQST_LOCAL           0x0004L /* Do only local lookup */
#define SCHRQST_FORCEREMOTE     0x0008L /* Force remote lookups even if workstation based mail */
#define SCHRQST_EXTFORMAT       0x0020L /* Get busytime in the SCHED_ENTRY_EXT 
                                        ** format instead of the normal 
                                        ** SCHED_ENTRY format from preR6
                                        ** Note: Use does not guarantee that
                                        ** data will be in SCHED_ENTRY_EXT
                                        ** format since we do not convert data
                                        ** from preR6 servers.
                                        */

STATUS LNPUBLIC SchFreeTimeSearch(
        const UNID FAR      *pApptUnid,
        TIMEDATE FAR        *pApptOrigDate,
        WORD                fFindFirstFit,
        DWORD               dwReserved,         
        TIMEDATE_PAIR FAR   *pInterval,
        WORD                Duration,
        LIST FAR            *pNames,
        HANDLE FAR          *rethRange );

STATUS LNPUBLIC SchRetrieve(
        const UNID FAR      *pApptUnid,
        TIMEDATE FAR        *pApptOrigDate,
        DWORD               dwOptions,
        TIMEDATE_PAIR FAR   *pInterval,
        LIST FAR            *pNames,
        HCNTNR FAR          *rethCntnr,
        void FAR            *MustBeNull1, 
        void FAR            *MustBeNull2,
        void FAR * FAR      *MustBeNull3 );


STATUS LNPUBLIC SchSrvRetrieve(
        LIST FAR            *pClientNames,
        const UNID FAR      *pApptUnid,
        TIMEDATE FAR        *pApptOrigDate,
        DWORD               dwOptions,
        TIMEDATE_PAIR FAR   *pInterval,
        LIST FAR            *pNames,
        HCNTNR FAR          *rethCntnr );

STATUS LNPUBLIC SchSrvRetrieveExt(
        LIST FAR            *pClientNames,
        const UNID FAR      *pApptUnid,
        TIMEDATE FAR        *pApptOrigDate,
        DWORD               dwOptions,
        TIMEDATE_PAIR FAR   *pInterval,
        LIST FAR            *pNames,
        LIST FAR            *pDetails,
        LIST FAR            *piCalList,
        char FAR            *pszProxyUserName,
        char FAR            *pszProxyPassword,
        HCNTNR FAR          *rethCntnr );

#ifdef __cplusplus
}
#endif

#endif /* SCHEDULE_DEFS */

