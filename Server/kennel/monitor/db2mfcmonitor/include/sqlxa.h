/******************************************************************************
*
* Source File Name = SQLXA.H
*
* (C) COPYRIGHT International Business Machines Corp. 1993, 1995
* All Rights Reserved
* Licensed Materials - Property of IBM
*
* US Government Users Restricted Rights - Use, duplication or
* disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
*
* Function = Include File defining:
*              XA Interface - Constants
*              XA Interface - Data Structures
*              XA Interface - Function Prototypes
*
 * Operating System = Common C Include File
*
******************************************************************************/

#ifndef SQL_H_SQLXA
   #define SQL_H_SQLXA             /* Permit duplicate Includes */

#include "sqlsystm.h"              /* Provides _SQLOLDCHAR      */
#include "db2ApiDf.h"              /* Provides db2 api literals */

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(DB2OS2) || defined(DB2NT))
#if defined _WIN64
#pragma pack(8)
#else
#pragma pack(4)
#endif
#elif (defined(DB2WIN))
#pragma pack(2)
#elif (defined(DB2MAC))
#if (defined(DB2PPC))
#pragma align 4
#elif (defined(DB268K))
#pragma align 2
#endif
#elif (defined(DB2HP) || defined(DB2SNI))

#elif (defined(DB2SUN) && defined(__xlC__))
#pragma options align=natural
#elif (defined(DB2AIX) && defined(__64BIT__))
#pragma options align=natural
#elif (defined(DB2AIX))
#pragma options align=power
#endif

/*
* Define an alias for the XID structure defined by X/Open.
* This avoids conflicts with the XID defined by X11 (Xwindows).
*/

#define SQLXA_XIDDATASIZE   128    /* size in bytes           */

#define SQLXA_XID_FORMATID  0x53514C20      /* "SQL " */
#define SQLXA_XID_F2PC_FORMATID 0x46325043  /* "F2PC" */           //@d267738rlz
#define SQLXA_XID_BQLEN     0
#define SQLXA_XID_APPLIDLEN 25
#define SQLXA_XID_SEQNUM    4
#define SQLXA_XID_GTLEN     SQL_DBNAME_SZ+1+SQLXA_XID_APPLIDLEN+SQLXA_XID_SEQNUM+1

#define SQLXA_XID_TSLEN     4
#define SQLXA_XID_LOGIDLEN  4
#define SQLXA_XID_LOGIDPOS  SQL_DBNAME_SZ+1+SQLXA_XID_TSLEN
#define SQLXA_XID_TIDPOS    SQLXA_XID_LOGIDPOS+SQLXA_XID_LOGIDLEN
#define SQLXA_XID_TIDLEN    6

#define SQLXA_XID_GTRID_LEN   17                                   //@d275243iis
#define SQLXA_XID_BQUAL_LEN   6                                    //@d275243iis


struct sqlxa_xid_t {
  sqlint32 formatID;                   /* format identifier       */
  sqlint32 gtrid_length;               /* value from 1 through 64 */
  sqlint32 bqual_length;               /* value from 1 through 64 */
  char data[SQLXA_XIDDATASIZE];
  } ;

typedef struct sqlxa_xid_t SQLXA_XID;

/* the following macro can generate a NULLXID */
#define SQLXA_NULLXID( x ) \
{ (x).formatID     = -1; \
(x).gtrid_length = -1; \
(x).bqual_length = -1; \
(x).data[0] = 0; }

/* provide a test for the NULLXID */
/* note that we only test the formatID! -> a simplification */
#define SQLXA_XID_IS_NULL( x ) ( (x).formatID     == -1 )

/* provide a test for Microsoft DTC */
#define SQLXA_XID_IS_MSDTC( x ) ( (x).formatID    == 0x00445443 ) /*@d76530pkt*/

/* provide a test for Encina Server */
#define SQLXA_XID_IS_ENCINA( x ) ( (x).formatID   == 113577 )

/* provide a test for SYSTEM_NU Server */
#define SQLXA_XID_IS_SYSTEM_NU( x ) ( (x).formatID   == 33 )

/* provide a test for DB2 coordinated transaction */
#define SQLXA_XID_IS_DB2( x ) ( (x).formatID   == SQLXA_XID_FORMATID )

/* provide a test for Federated coordinated transaction              @d267738rlz*/
#define SQLXA_XID_IS_F2PC( x ) ( (x).formatID == SQLXA_XID_F2PC_FORMATID ) //@d267738rlz

/* provide a test for DB2 coordinated transaction coming from V7 client @d267738rlz */
#define SQLXA_XID_SQL_V7(xid) ( SQLXA_XID_IS_DB2(xid) && ((xid)->bqual_length == 0))  //@d267738rlz

/* provide a test for DB2 coordinated transaction */
#define SQLXA_XID_IS_DB2_DRDA( x ) ( (x).formatID   == 0x44524441 )

/* provide XID header size ...sum of size of  formatID,gtrid_size,bqual_size */
#define XID_HEADER_SIZE 12

/*
   F2PC XID will always have the format of                           @d267738rlz
     - gtrid 25                                                      @d267738rlz
     - bqual 0                                                       @d267738rlz
*/
#define SQLXA_F2PC_GTRID       25                                  //@d267738rlz
#define SQLXA_F2PC_BQUAL        0                                  //@d267738rlz
#define SQLXA_F2PC_XIDDATASIZE (SQLXA_F2PC_GTRID + SQLXA_F2PC_BQUAL)  //@d267738rlz
#define SQLXA_F2PC_XID_SIZE    (XID_HEADER_SIZE + SQLXA_F2PC_XIDDATASIZE)  //@d267738rlz

/* when we make a copy of F2PC XID, it can be done more efficiently  @d267738rlz
   because we know the exact size.                                   @d267738rlz
*/
#define SQLXA_COPY_F2PC_XIDS(_targetPtr, _sourcePtr) \
    sqlomcpy(_targetPtr, _sourcePtr, SQLXA_F2PC_XID_SIZE);         //@d267738rlz

/* provide true XID size */
#define XID_ACTUAL_SIZE(p_xid) ( XID_HEADER_SIZE + \
                                 (p_xid)->gtrid_length + \
                                 (p_xid)->bqual_length )

/*
* Compare xidA and xidB, for equality.
* Returns "true" if compared to NULLXID by checking the formatID
* Returns "true" (ie. non-zero) if xid's are the same.
* This macro expands to an expression and is therefore suitable for use in
* statements like "while" or "if"
*/

#define XIDEQUAL(xidA, xidB) ( (((xidA).formatID == -1) &&                    \
                                ((xidB).formatID == -1)) ? 1 :                \
                               ((xidA).formatID    ==(xidB).formatID)     &&  \
                               ((xidA).gtrid_length==(xidB).gtrid_length) &&  \
                               ((xidA).bqual_length==(xidB).bqual_length) &&  \
                               ( memcmp((xidA).data, (xidB).data,             \
                                        (int)(xidA).gtrid_length +            \
                                        (int)(xidA).bqual_length) == 0) )

/*
* Compare xidA and xidB, for LOOSE equality:
* 1. (Neither xid can be NULL) and
* 2. (formatIDs must match) and
* 3. (gtrid lengths must match) and
* 4. (gtrid contents must match) and
* 5. ((bqual lengths must differ) or (bqual contents must differ))
*/
#define XIDLSLYCPLD(xidA, xidB) (((xidA).formatID != -1) &&                       \
                                 ((xidA).formatID != SQLXA_XID_FORMATID) &&       \
                                 ((xidA).formatID == (xidB).formatID) &&          \
                                 ((xidA).gtrid_length == (xidB).gtrid_length) &&  \
                                 (memcmp((xidA).data, (xidB).data,                \
                                         (int)(xidA).gtrid_length) == 0) &&       \
                                 (((xidA).bqual_length != (xidB).bqual_length) || \
                                  (memcmp((xidA).data, (xidB).data,               \
                                          (int)(xidA).gtrid_length +              \
                                          (int)(xidA).bqual_length) != 0)))


/*
* Copy xidB into xidA
* -> a macro is not really needed here, since the C language supports
* structure assignments (and this should be efficient on any platform).
*/

#define XIDCPY( xidA, xidB )  (xidA) = (xidB);

/* define a TRUE/FALSE pair for boolean fields in XA structures */
#define SQLXA_TRUE    1
#define SQLXA_FALSE   0

/* define all possible states of transactions returned by "recover" */
#define SQLXA_TS_PREP       'P'  /* prepared                        */
#define SQLXA_TS_HCOM       'X'  /* heuristically committed         */
#define SQLXA_TS_HROL       'Y'  /* heuristically rolled back       */
#define SQLXA_TS_END        'E'  /* idle -> xa_end has been issued  */
#define SQLXA_TS_MACK       'M'  /* missing commit acknowledgement  */
#define SQLXA_TS_MRACK      'R'  /* missing rollback acknowledgement*/

/* possible indoubt states in Parallel Edition ONLY other than the above */
#define SQLXA_TS_PHC        'A'  /* prepared and heur. committed */
#define SQLXA_TS_PHR        'B'  /* prepared and heur. rolled back */
#define SQLXA_TS_HCHR       'C'  /* heur. committed and heur. rolled back */
#define SQLXA_TS_PHCHR      'D'  /* prepared, heur. committed and heur. rolled back */

#define SQLXA_EXE_ALL_NODES  1   /* execute the request on all the nodes in MPP environment */
#define SQLXA_EXE_THIS_NODE  0   /* execute the request on the node it is issued from */

/* various transaction originators */
#define SQLXA_ORIG_PE        0   /* transaction originated by DB2 in MPP environment*/
#define SQLXA_ORIG_XA        1   /* transaction originated by XA */
#define SQLXA_ORIG_FXA       2   /* transaction originated by F2PC */

/*
* The following structure is used to describe the current state
* of an indoubt (ie "prepared") or heuristically completed transaction.
*
* We need to beef up the explanations of the structure elements,
* so that they can appear in the user doc -> CLP will be providing
* the info basically verbatim to the user.
*
* The char string areas will always be right padded with blanks.
*/

#define SQLXA_DBNAME_SZ         8
#define SQLXA_APPLID_SZ         32
#define SQLXA_SEQ_SZ            4
#define SQLXA_OLD_USERID_SZ     8
#define SQLXA_USERID_SZ         SQL_USERID_SZ
#define SQLXA_PASSWD_SZ         8

typedef struct sqlxa_recover_t {
   sqluint32      timestamp;
   SQLXA_XID      xid;
   _SQLOLDCHAR    dbalias[SQLXA_DBNAME_SZ];
   _SQLOLDCHAR    applid[SQLXA_APPLID_SZ];
   _SQLOLDCHAR    sequence_no[SQLXA_SEQ_SZ];
   _SQLOLDCHAR    auth_id[SQLXA_OLD_USERID_SZ];
   char           log_full;                    /* SQLXA_TRUE/FALSE */
   char           connected;                   /* SQLXA_TRUE/FALSE */
   char           indoubt_status;              /* SQLXA_TS_xxx     */
   char           originator;                  /* SQLXA_ORIG_PE/XA */
   char           reserved[8];                 /* set to zeroes    */
   } SQLXA_RECOVER;

/**** reason codes for the SQL_RC_W997 general XA warning code */
#define SQLXAER_REASON_DEADLOCK  1  /* returned when XA_END clears a deadlock */
#define SQLXAER_REASON_RDONLYCOM 35 /* trans read-only and has been committed */

/**** reason codes for the SQL_RC_E998 general XA error sqlcode */
#define SQLXAER_REASON_ASYNC    1 /* asynch operation already outstanding     */
#define SQLXAER_REASON_RMERR    2 /* an RM error occurred                     */
#define SQLXAER_REASON_NOTA     3 /* XID not recognized                       */
#define SQLXAER_REASON_INVAL    4 /* invalid parameters                       */
#define SQLXAER_REASON_PROTO    5 /* routine called in improper context       */
#define SQLXAER_REASON_RMFAIL   6 /* RM unavailable                           */
#define SQLXAER_REASON_DUPID    7 /* the XID already exists                   */
#define SQLXAER_REASON_OUTSIDE  8 /* RM doing work outside global transaction */
#define SQLXAER_REASON_AXREG    9 /* axreg failed                             */
#define SQLXAER_REASON_1LUW    10 /* trying to start new trans while suspended*/
#define SQLXAER_REASON_NOJOIN  11 /* can't joing work of existing transaction */
#define SQLXAER_REASON_AXUNREG 12 /* axunreg failed                           */
#define SQLXAER_REASON_BADAX   13 /* ax interface failure (unresolved symbol) */
#define SQLXAER_REASON_DTCXATMDOWN 14 /* Enlist TM with DTC has failed        */
#define SQLXAER_REASON_DTCNOTRANSACTION 15 /*AXREG with DTC has failed        */
#define SQLXAER_REASON_NOHEUR  35 /* heuristic operaion invalid for non-XA db */
#define SQLXAER_REASON_BADXID  36 /* XID not knowne by the RM                 */
#define SQLXAER_REASON_HEURCOM 37 /* trans already heuristically commited     */
#define SQLXAER_REASON_HEURROL 38 /* trans already heuristically rolled back  */
#define SQLXAER_REASON_NOTINDT 39 /* not an indoubt transaction               */
#define SQLXAER_REASON_RBONLY  40 /* Rollback only is allowed                 */
#define SQLXAER_REASON_HEURFAL 41 /* heuristic commit fails because of node   */
                                  /* failure                                  */
#define SQLXAER_REASON_NO_XA_SUPP 43 /* XA not support by remote server       */
#define SQLXAER_REASON_DISALLOW_LCT 225 /* disallow operations in Loosely Coupled Transaction */
#define SQLXAER_REASON_DO_ROLL 226 /* heuristic rollback is performed */
#define SQLXAER_REASON_UNKNOWN 227 /* transaction state is unknown */
#define SQLXAER_REASON_CURSORS_EXIST 228 /* cursors still existed */
#define SQLXAER_REASON_NOT_FIRST_STMT 229 /* not the first stmt */
#define SQLXAER_REASON_DO_COMMIT 230 /* heuristic commit is performed */  //@dXXXrfp


                                                                 /*db126486djm*/
/**** subcodes for the SQL_RC_E998 general XA error sqlcode  reason code 4 (SQLXAER_REASON_INVAL) */
#define SQLXAER_SUBCODE_XAINFO_INVALID                 1
#define SQLXAER_SUBCODE_DBNAME_TOO_LONG                2
#define SQLXAER_SUBCODE_USERID_TOO_LONG                3
#define SQLXAER_SUBCODE_PASSWD_TOO_LONG                4
#define SQLXAER_SUBCODE_USERID_NO_PASSWD               5
#define SQLXAER_SUBCODE_PASSWD_NO_USERID               6
#define SQLXAER_SUBCODE_TOO_MANY_PARMS                 7
#define SQLXAER_SUBCODE_RMID_NOT_MATCH_DBNAME          8
#define SQLXAER_SUBCODE_DBNAME_NOT_SPECIFIED           9                         /* +SAM */
#define SQLXAER_SUBCODE_INVALID_EXE_TYPE               10
#define SQLXAER_SUBCODE_INVALID_TMFLAGS                14


/**** subcodes for the SQL_RC_E998 general XA error sqlcode reason code 9 (SQLXAER_REASON_AXREG) */
#define SQLXAER_SUBCODE_JOINING_XID_NOT_FOUND          1
#define SQLXAER_SUBCODE_AXLIB_LOAD_FAIL                2
#define SQLXAER_SUBCODE_TP_MON_NAME_AXLIB_NOT_FOUND    3

/**** subcodes for the SQL_RC_E998 general XA error sqlcode reason code 13 (SQLXAER_REASON_BADAX) */
#define SQLXAER_SUBCODE_AX_REG_NOT_FOUND               1
#define SQLXAER_SUBCODE_AX_UNREG_NOT_FOUND             2


/**** subcodes for the SQL_RC_E998 general XA error sqlcode reason code ?? */
#define SQLXAER_SUBCODE_INVALID_EXE_TYPE               10
#define SQLXAER_SUBCODE_SUSPEND_CURSOR                 11
#define SQLXAER_SUBCODE_AX_LIB                         12
#define SQLXAER_SUBCODE_TP_MON                         13
                                                                 /*de126486djm*/
/**** reason codes for the SQL_RC_E30090 general XA error code */
#define SQLXAER_REASON_READONLY    1  /* update issued against readonly database */
#define SQLXAER_REASON_BADAPI      2  /* This API not allowed                    */
#define SQLXAER_REASON_CANTHOLD    3  /* HELD CURSOR not allowed                 */
#define SQLXAER_REASON_BADAPI_STR "2" /* This API not allowed - str version @d213813rfp  */

/*** prototypes for the heuristic API's in sqlxapi.c *************/

extern int SQL_API_FN sqlxphqr(    /* query existing indoubt transactions  */
   int                  exe_type,
   SQLXA_RECOVER        **ppIndoubtData,
   sqlint32             *pNumIndoubts,
   struct sqlca         *pSqlca
   );

extern int SQL_API_FN sqlxphcm(    /* heuristically commit a transaction   */
   int                  exe_type,
   SQLXA_XID            *pTransId,
   struct sqlca         *pSqlca
   );

extern int SQL_API_FN sqlxphrl(    /* heuristically rollback a transaction */
   int                  exe_type,
   SQLXA_XID            *pTransId,
   struct sqlca         *pSqlca
   );

extern int SQL_API_FN sqlxhfrg(    /* heuristically forget a transaction   */
   SQLXA_XID            *pTransId,
   struct sqlca         *pSqlca
   );

/******************************************************************************
** List existing indoubt transactions
*******************************************************************************/
typedef SQL_STRUCTURE db2XaRecoverStruct{
   sqluint32      timestamp;                   /* Time when indoubt state entered */
   SQLXA_XID      xid;                         /* XA Transaction Identifier */
   char           dbalias[SQLXA_DBNAME_SZ];    /* Database alias */
   char           applid[SQLXA_APPLID_SZ];     /* Application Identifier */
   char           sequence_no[SQLXA_SEQ_SZ];   /* Sequence Number */
   char           auth_id[SQLXA_USERID_SZ];    /* Authorisation ID  */
   char           log_full;                    /* Log full         */
   char           connected;                   /* Is application connected ? */
   char           indoubt_status;              /* Indoubt status  */
   char           originator;                  /* XA or DB2 EEE transaction */
   char           reserved[8];                 /* Reserved         */
   } db2XaRecoverStruct;

/* List Indoubt Transaction API struct                                        */
typedef SQL_STRUCTURE db2XaListIndTransStruct
{
   db2XaRecoverStruct * piIndoubtData;         /* Indoubt Data buffer pointer */
   db2Uint32            iIndoubtDataLen;       /* Indoubt Data buffer length */
   db2Uint32            oNumIndoubtsReturned;  /* Number of indoubts returned */
   db2Uint32            oNumIndoubtsTotal;     /* Total number of indoubts  */
   db2Uint32            oReqBufferLen;         /* Required buffer length    */
} db2XaListIndTransStruct;

/* db2XaListIndTrans - API                                                    */
SQL_API_RC SQL_API_FN                  /* List indoubt transactions           */
  db2XaListIndTrans (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* Input parameters                    */
       struct sqlca * pSqlca);         /* SQLCA                               */

/******************************************************************************
** Get Information for Resource Manager(rmid)
*******************************************************************************/
typedef SQL_STRUCTURE db2XaGetInfoStruct
{
   db2int32             iRmid;                /* rmid to get information for   */
   struct sqlca         oLastSqlca;           /* sqlca for last XA API call    */
} db2XaGetInfoStruct;

SQL_API_RC SQL_API_FN
  db2XaGetInfo(db2Uint32 versionNumber,    /* Database version number         */
               void * pParmStruct,         /* In/out parameters               */
               struct sqlca * pSqlca);     /* SQLCA                           */

/*** map old api to new ones ***/
#define sqlxhqry(_ppIndoubtData, _pNumIndoubts, _pSqlca) \
        sqlxphqr(SQLXA_EXE_THIS_NODE, _ppIndoubtData, _pNumIndoubts, _pSqlca)

#define sqlxhcom(_pTransId, _pSqlca) \
        sqlxphcm(SQLXA_EXE_THIS_NODE, _pTransId, _pSqlca)

#define sqlxhrol(_pTransId, _pSqlca) \
        sqlxphrl(SQLXA_EXE_THIS_NODE, _pTransId, _pSqlca)

                                                                    /*@fb33569rjp*/
/*
 * The following structure is used to describe the current state
 * of a DRDA indoubt transaction.
 */

#define SQLCSPQY_DBNAME_SZ       8
#define SQLCSPQY_LUWID_SZ       35
#define SQLCSPQY_LUNAME_SZ      17
#define SQLCSPQY_APPLID_SZ      32

#define SQLCSPQY_AR             'R'
#define SQLCSPQY_AS             'S'

#define SQLCSPQY_STATUS_COM     'C'
#define SQLCSPQY_STATUS_RBK     'R'
#define SQLCSPQY_STATUS_IDB     'I'
#define SQLCSPQY_STATUS_HCM     'X'
#define SQLCSPQY_STATUS_HRB     'Y'

typedef struct sqlcspqy_indoubt_t {
   SQLXA_XID    xid;
   char         luwid[SQLCSPQY_LUWID_SZ+1];
   char         corrtok[SQLCSPQY_APPLID_SZ+1];
   char         partner[SQLCSPQY_LUNAME_SZ+1];
   char         dbname[SQLCSPQY_DBNAME_SZ+1];
   char         dbalias[SQLCSPQY_DBNAME_SZ+1];
   char         role;
   char         uow_status;
   char         partner_status;
} SQLCSPQY_INDOUBT;

/* function prototype for list DRDA indoubt transactions */

extern int SQL_API_FN sqlcspqy(SQLCSPQY_INDOUBT     **indoubt_data,
                               sqlint32              *indoubt_count,
                               struct sqlca          *sqlca);

#if (defined(DB2OS2) || defined(DB2NT) || defined(DB2WIN))
#pragma pack()
#elif (defined(DB2MAC))
#pragma align
#elif (defined(DB2HP) || defined(DB2SNI))

#elif (defined(DB2AIX) || (defined(DB2SUN) && defined(__xlC__)))
#pragma options align=reset
#endif

#ifdef __cplusplus
}
#endif

#endif /* SQL_H_SQLXA */
