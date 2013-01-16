/**********************************************************************
*
*  OCO SOURCE MATERIALS
*
*  IBM CONFIDENTIAL (IBM CONFIDENTIAL-RESTRICTED when combined with the
*                    Aggregated OCO Source Modules for this Program)
*
*  Source File Name = (%W%)
*
*  Descriptive Name = Satellite Client definitions
*
*  Copyright = nnnnnnn (c) Copyright IBM Corp 1998, 1998
*              Licensed Materials - Program Property of IBM
*
*  Function = Header file for Satellite Control Server : External
*
*  Dependencies =
*
*  Restrictions =
*
*  Operating System =
*
*  Defect Activity:
*  defect#   DATE    WHO DESCRIPTION
*  =====================================================================
*   101401 10/02/99  rah Build on NT.
*   101887 18/02/99  rah Add "sync" APIs to testdrv.
*   102357 22/02/99  rah Add db2SetSyncSession API.
*   102586 23/02/99  rah Add remaining sync session APIs.
*   102825 02/03/99  rah Add script execution capability.
*   106107 23/03/99  rah semi-externalize db2UpdateSatelliteProgress for ASNSAT
*   109827 12/04/99   al Map/remap progress constants
*   108240 09/04/99  rah sync app rework.
*   111424 19/04/99  rah Catalog DB rework.
*   111780 21/04/99  rah Sync progress rework.
*   112665 26/04/99  rah sync 'start' function broken
*   115868 13/05/99  rah test mode doesn't recognize DB DIR NOT FOUND (-1013)
*   115382 18/05/99  rah Update Sync progress should require message as input
*   115382 18/05/99  rah Update Sync progress should require message as input
*   116776 19/05/99  rah Added sym defs for Satellite sync progress in db2ApiDf
*   118304 31/05/99  rah SQLCA not filled in correctly for all SYNC error cases
*   118436 01/06/99  rah sync app: Pop-up "Log in" dialog for -551
*   120876 09/07/99  rah Return -3968 if script fails
*   138560 20/06/00  hht Added a get nlv utility macro
*   155262 30/01/01  sak Added defines for user id length and id,pwd invalid errors
*
*
*  Last Changed =    %E%   %U%
*
***********************************************************************/

#ifndef __DB2SYNC_H__
#define __DB2SYNC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <ctype.h>                                                    /*@d102825rah*/
#include <sqlsystm.h>                  /* System dependent defines  */
#include <sql.h>                       /* Includes sqlsystm & sqlca */
#include <db2ApiDf.h>                  /* Externalized APIs         */


#ifndef SQLWINT
/******************************************************************************
** Satellite Synchronization APIs and related definitions
*******************************************************************************/

/* Satellite Synchronization Definitions                                      */
#define SQL_SYNCSESSIONID_SZ 18

/* Stages of synchronization progress: Normal Mode                               @d106107rah*/
#define DB2SYNC_SYNCMODE_PHASE_INIT             10                             /*@d115382rah*/
#define DB2SYNC_SYNCMODE_PHASE_EXECPREVSCRIPTS  20                             /*@d115382rah*/
#define DB2SYNC_SYNCMODE_PHASE_SENDPREVRESULTS  30                             /*@d115382rah*/
#define DB2SYNC_SYNCMODE_PHASE_SENTPREVRESULTS  40                             /*@d115382rah*/
#define DB2SYNC_SYNCMODE_PHASE_RETRIEVEBATCHES  50                             /*@d115382rah*/
#define DB2SYNC_SYNCMODE_PHASE_EXECUTESCRIPTS   60                             /*@d115382rah*/
#define DB2SYNC_SYNCMODE_PHASE_SENDRESULTS      70                             /*@d115382rah*/
#define DB2SYNC_SYNCMODE_PHASE_SENTRESULTS      80                             /*@d115382rah*/
#define DB2SYNC_SYNCMODE_PHASE_DONE             90                             /*@d115382rah*/

/* Stages of synchronization progress: Test Mode                                 @d115382rah*/
#define DB2SYNC_TESTMODE_PHASE_INIT         100                                /*@d115382rah*/
#define DB2SYNC_TESTMODE_PHASE_CONNTODB     110                                /*@d115382rah*/
#define DB2SYNC_TESTMODE_PHASE_DISCONNDB    120                                /*@d115382rah*/
#define DB2SYNC_TESTMODE_PHASE_DONE         130                                /*@d115382rah*/


/* Input parameters for the Set Synchronization Session API                   */
typedef struct
{
        char                   *piSyncSessionID; /* ID for sync session       */
} db2SetSyncSessionStruct;

SQL_API_RC SQL_API_FN                  /* Set sync session for a satellite    */
  db2SetSyncSession (
        db2Uint32 versionNumber,       /* Database version number             */
        void * pParmStruct,            /* Input parameters                    */
        struct sqlca * pSqlca);        /* SQLCA                               */


/* Output parameters for the Get Synchronization Session API                  */
typedef struct
{
        char                   *poSyncSessionID; /* ID for sync session       */
} db2GetSyncSessionStruct;

SQL_API_RC SQL_API_FN                  /* Get sync session ID for a           */
                                       /* satellite                           */
  db2GetSyncSession (
        db2Uint32 versionNumber,       /* Database version number             */
        void * pParmStruct,            /* Output parameters                   */
        struct sqlca * pSqlca);        /* SQLCA                               */

/* Output parameters for the Query Satellite Progress API                     */
typedef struct
{
        db2int32               oStep;  /* current step of synchronization     */
        db2int32               oSubstep; /* substep of the current step       */
        db2int32               oNumSubsteps; /* total number of substeps      */
        db2int32               oScriptStep; /* step of current script         */
                                       /* substep                             */
        db2int32               oNumScriptSteps; /* total number of script     */
                                       /* steps                               */
        char                   *poDescription; /* description of step         */
        char                   *poError; /* error text, if available          */
        char                   *poProgressLog; /* contents of progress log    */
} db2QuerySatelliteProgressStruct;

SQL_API_RC SQL_API_FN                  /* Query Satellite Progress            */
  db2QuerySatelliteProgress (
        db2Uint32 versionNumber,       /* Database version number             */
        void * pParmStruct,            /* Ouput parameters                    */
        struct sqlca * pSqlca);        /* SQLCA                               */

SQL_API_RC SQL_API_FN                  /* Test Satellite's ability to         */
                                       /* synchronize                         */
  db2SyncSatelliteTest (
        db2Uint32 versionNumber,       /* Database version number             */
        void * pParmStruct,            /* Input/Ouput parameters              */
        struct sqlca * pSqlca);        /* SQLCA                               */

SQL_API_RC SQL_API_FN                  /* Synchronize Satellite               */
  db2SyncSatellite (
        db2Uint32 versionNumber,       /* Database version number             */
        void * pParmStruct,            /* Input/Ouput parameters              */
        struct sqlca * pSqlca);        /* SQLCA                               */

SQL_API_RC SQL_API_FN                  /* Stop Satellite's Synchronization    */
                                       /* Session                             */
  db2SyncSatelliteStop (
        db2Uint32 versionNumber,       /* Database version number             */
        void * pParmStruct,            /* Input/Ouput parameters              */
        struct sqlca * pSqlca);        /* SQLCA                               */
#endif
/* sqlcodes for db2SetSyncSession */
#define SQL_RC_SETSYNCSESS_OK     3942
#define SQL_RC_SYNCSESSIDLEN_ERR -3943
#define SQL_RC_RESETSYNCSESS_OK   3944
#define SQL_RC_GETSYNCSESS_OK     3945
#define SQL_RC_SYNCSESSID_ERR    -3946

/* Strip trailing blanks from a string     @d102357rah*/
inline void stripBlanks( char * str, int len )
{
  for (int i=len-1; i>=0; i -= 1)
  {
    if (str[i] == ' ')
      str[i] = '\0';
    else
      break;
  } /*for*/
} /* end of function stripBlanks */

inline void upcase( char * toStr, char * fromStr, int len )
{
  for (int i=0; i<len; i += 1)
    toStr[i] = (char)toupper(fromStr[i]);
} /* end of function upcase */

SQL_API_RC SQL_API_FN db2UpdateSatelliteProgress( int scriptStep, int numScriptSteps ); //@d106107rah

sqlint32 satFillSqlca( struct sqlca * pSqlca,    /*@d108240rah*/
                       sqlint32       sqlcode,   /*@d108240rah*/
                       char         * pFuncname, /*@d108240rah*/
                       int            numTokens, /*@d108240rah*/
                       char         * tokens[],  /*@d108240rah*/
                       char         * addlErrmc, /*@d108240rah*/
                       int            addlErrml  /*@d108240rah*/
                     );                          /*@d108240rah*/

/* miscellaneous definitons */
#define SAT_MAXLEN_SATELLITEID 20
#define SAT_SATTELLITEIDVAR    "DB2SATELLITEID"


/* Message numbers for "normal" mode sync progress                               @d115382rah
   taken from the db2sat.mo message file.                                        @d115382rah*/
#define SYNC_INIT                  23                                          /*@d115382rah*/
#define SYNC_EXEC_PREV_SCRIPTS     28                                          /*@d115382rah*/
#define SYNC_SEND_PREV_RESULTS     14                                          /*@d115382rah*/
#define SYNC_SENT_PREVEXEC_RESULTS 17                                          /*@d115382rah*/
#define SYNC_RETRIEVE_BATCHES      52                                          /*@d115382rah*/
#define SYNC_EXEC_CURRENT_SCRIPTS  29                                          /*@d115382rah*/
#define SYNC_EXECUTE_SCRIPTS       32                                          /*@d115382rah*/
#define SYNC_SEND_EXEC_RESULTS     14                                          /*@d115382rah*/
#define SYNC_SENT_EXEC_RESULTS     15                                          /*@d115382rah*/
#define SYNC_COMPLETE              26                                          /*@d115382rah*/

/* Message numbers for test mode sync progress                                   @d115382rah
   taken from the db2sat.mo message file.                                        @d115382rah*/
#define TEST_INIT                 24    /* SAT msg */                          /*@d109827al*/
#define TEST_DISCONNECT           13    /* SAT msg */                          /*@d109827al*/
#define TEST_CONNECT              10    /* SAT msg */                          /*@d109827al*/
#define TEST_SUCCESSFUL           27    /* SAT msg */                          /*@d109827al*/
#define TEST_ENVERR               -2014 /* SQL msg */                          /*@d115382rah*/

/* Test mode setp/substep/script step is                                         @d115382rah
   not applicable to the current phase.                                          @d115382rah*/
#define STEP_NA -1 /* Not Applicable */                                        /*@d115382rah*/


/* sqlcodes for APIs */
#define SQL_RC_OPPRIVNEEDED_ERR          -551                                  //@d118436rah
#define SQL_RC_PKGNOTFOUND_ERR           -805                                  //@d111424rah
#define SQL_RC_PWDINVALID_ERR            -1003                                 //@d155262sak
#define SQL_RC_DBALIASALREADYEXISTS_ERR  -1005                                 //@d111424rah
#define SQL_RC_DBALIASNOTFOUND_ERR       -1013
#define SQL_RC_NODEALREADYEXISTS_ERR     -1018                                 //@d111424rah
#define SQL_RC_DBDIRNOTEXIST_ERR         -1031                                 //@d115868rah
#define SQL_RC_AUTHIDINVALID_ERR         -1046                                 //@d155262sak
#define SQL_RC_NODENOTEXIST_ERR          -1097                                 //@d111780rah
#define SQL_RC_HOSTNOTFOUND_ERR          -1336                                 //@d111424rah
#define SQL_RC_IDPWINVALID_ERR           -1403
#define SQL_RC_NONSEVERE_ERR             -3901
#define SQL_RC_SEVERE_ERR                -3902
#define SQL_RC_SYNCSUCCESSFULL_INF        3910                               //@d120876rah
#define SQL_RC_SYNCSTOPSUCCESSFULL_INF    3912                               //@d118304rah
#define SQL_RC_SYNCSESSNOTACTIVE_INF      3913                               //@d118304rah
#define SQL_RC_INTERRUPT_REC_OK           3914                               //@d118304rah
#define SQL_RC_NOSCRIPTSTOEXEC_WARN       3930                               //@d118304rah
#define SQL_RC_SETSYNCSESS_OK             3942
#define SQL_RC_SYNCSESSIDLEN_ERR         -3943
#define SQL_RC_RESETSYNCSESS_OK           3944
#define SQL_RC_GETSYNCSESS_OK             3945
#define SQL_RC_SYNCSESSID_ERR            -3946
#define SQL_RC_SYNCSESSALREADYACTIVE_ERR -3950                              //@d118304rah
#define SQL_RC_SATIDNOTFOUND_ERR         -3951
#define SQL_RC_SATCTLDBNOTFOUND_ERR      -3955
#define SQL_RC_SYNCSESSFAILED_ERR        -3966
#define SQL_RC_NOPROGINFO_ERR            -3967                              /*@d112665rah*/
#define SQL_RC_SCRIPTFAILED_ERR          -3968                              //@d120876rah
#define SQL_RC_APPVER_NOTEXIST_ERR       -3956
#define SQL_RC_COMMFAILURE_ERR           -30081                             //@d111424rah

/* SATELLITE-specific messages */
#define SAT_RC_SYNCINPROG_WARN        2024

/* reason codes for SQL_RC_NONSEVERE_ERR */
#define SQL_REASONCODE_CONNERR       "1"
#define SQL_REASONCODE_AUTHERR       "2"
#define SQL_REASONCODE_MEMERR        "3"
#define SQL_REASONCODE_STOPQUEERR    "4"                                    //@d118304rah
#define SQL_REASONCODE_SIGNALERR     "5"                                    //@d118304rah

/* reason codes for SQL_RC_SYNCSESSFAILED_ERR */
#define SQL_REASONCODE_MISSAUTHERR   "1"
#define SQL_REASONCODE_MISSSCRIPTS   "2"
#define SQL_REASONCODE_SYSFILESERR   "3"
#define SQL_REASONCODE_SCRIPTSYSERR  "4"

//Utility Macros
/* MACRO:  Integer to Ascii */
#define itoa(i,b) (sprintf(b,"%d",i),b)                                     //@d138560hht
/* MACRO:  Long   to Ascii */                                               //@d138560hht
#define ltoa(i,b) (sprintf(b,"%ld",i),b)                                    //@d138560hht
/* Get NVL compliant message */                                             //@d138560hht
#define SQLMSGFILE            "db2sql.mo"                                   //@d138560hht
#define SATMSGFILE            "db2sat.mo"                                   //@d138560hht
#define GET_NLV_MSG(pSqlca, msgNum, fncName, numTokens, tokens, pDest, szDest, whichMsgFile)    \
if (szDest > 0)                                                                                 \
{                                                                                               \
   satFillSqlca( pSqlca, msgNum, fncName, numTokens, tokens, NULL, 0 );                         \
   (void) sqlaintp_api( (char *)pDest, szDest, SQLO_NOINTP_FORMAT, whichMsgFile, pSqlca );      \
}


#ifdef __cplusplus
}
#endif

#endif /* __DB2SYNC_H__ */
