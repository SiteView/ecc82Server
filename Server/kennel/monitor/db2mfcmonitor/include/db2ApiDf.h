/******************************************************************************
** 
** Source File Name: db2ApiDf
** 
** (C) COPYRIGHT International Business Machines Corp. 1987, 1997
** All Rights Reserved
** Licensed Materials - Property of IBM
** 
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
** 
** Function = Include File defining:
**              Environment Commands - Constants
**              Environment Commands - Structures
**              Environment Commands - Function Prototypes
**              Environment Commands - Labels for SQLCODES
** 
** Operating System:  Windows NT
** 
*******************************************************************************/
#ifndef SQL_H_DB2APIDF
#define SQL_H_DB2APIDF

#ifdef __cplusplus
extern "C" {
#endif

/* Note: _SQLOLDCHAR defaults to 'char'.  See sqlsystm.h for details.         */

#include "sqlsystm.h"                  /* System dependent defines  */

/* Required Include Files */

#include "sql.h"                       /* Includes sqlsystm & sqlca */
#include "sqlda.h"
#include "sqlmon.h"
#include "sqlaprep.h"
#include "sqlutil.h"
#include <stddef.h>                    /* needed for offsetof */

#ifdef _WIN64
#pragma pack(8)
#else
#pragma pack(4)
#endif

/* Version, Release PTF identifier                                            */
#define db2Version6Prior SQL_RELPRE6                 /* Pre Version 6.1.0.0   */
#define db2Version610    SQL_REL6100                 /* Version 6.1.0.0       */
#define db2Version710    SQL_REL7100                 /* Version 7.1.0.0       */
#define db2Version810    SQL_REL8100                 /* Version 8.1.0.0       */
#define db2Version812    SQL_REL8102                 /* Version 8.1.0.2       */
#define db2Version814    SQL_REL8104                 /* Version 8.1.0.4       */
#define db2Version820    SQL_REL8200                 /* Version 8.2.0.0       */
#define db2Version822    SQL_REL8202                 /* Version 8.2.0.2       */

/* Type definition names                                                      */
typedef sqluint64 db2Uint64;                         /* 64 bit unsigned int  */
typedef sqluint32 db2Uint32;                         /* 32 bit unsigned int   */
typedef sqlint32  db2int32;                          /* 32 bit signed int     */
typedef sqluint16 db2Uint16;                         /* 16 bit unsigned int   */
typedef sqlint16  db2int16;                          /* 16 bit signed int     */
typedef sqlint8   db2int8;                           /*  8 bit unsigned int   */
typedef SQL_PDB_NODE_TYPE db2NodeType;               /* Node                  */
typedef SQL_PDB_PORT_TYPE db2PortType;               /* Port                  */

/******************************************************************************
** Structure db2Char
** 
** Description:
**     pioData  -- Pointer to character data.
**              -- If NULL, no data will be returned.
** 
**     iLength  -- Input:  Size of the pData buffer.
** 
**     oLength  -- Output: Length of valid data in pData.
**              -- If the output data was truncated, this will be set to
**                 the complete size before truncation
*******************************************************************************/
typedef SQL_STRUCTURE db2Char
{
   char            *pioData;           /* Character data                      */
   db2Uint32       iLength;            /* I:  Length of pioData               */
   db2Uint32       oLength;            /* O:  Untruncated length of data      */
} db2Char;



/******************************************************************************
** Database and Database Manager Configuration
** Structures, Constants, and Function Prototypes
*******************************************************************************/

#define db2CfgDatabase                 1         /* act on database cfg, or   */
#define db2CfgDatabaseManager          2         /* act on database manager   */
                                                 /* cfg                       */
#define db2CfgImmediate                4         /* get/set current values,   */
                                                 /* or                        */
#define db2CfgDelayed                  8         /* get/set delayed values    */
#define db2CfgGetDefaults              64        /* get default values        */
#define db2CfgReset                    64        /* set to default values     */
                                                 /* (reset)                   */
#define db2CfgMaxParam                 64        /* maximum number of params  */
                                                 /* in db2Cfg paramArray      */


/******************************************************************************
** Constants describing a single configuration parameter
*******************************************************************************/
#define db2CfgParamAutomatic           1         /* let db2 set this value    */
#define db2CfgAutoOnly                 16        /* set to auto using         */
                                                 /* current                   */


/******************************************************************************
** Structure for a single Configuration Parameter
*******************************************************************************/
typedef SQL_STRUCTURE db2CfgParam
{
   db2Uint32                           token;    /* Parameter Identifier      */
   char                                *ptrvalue; /* Parameter value          */
   db2Uint32                           flags;    /* flags for this parameter  */
} db2CfgParam;

/******************************************************************************
** Main structure for db2CfgSet() and db2CfgGet()
*******************************************************************************/
typedef SQL_STRUCTURE db2Cfg
{
   db2Uint32                           numItems; /* Number of configuration   */
                                                 /* parameters in the         */
                                                 /* following array           */
   struct db2CfgParam                  *paramArray; /* Array of cfg           */
                                                 /* parameters                */
   db2Uint32                           flags;    /* Various properties        */
   char                                *dbname;  /* Database Name, if needed  */
} db2Cfg;

/******************************************************************************
** API for Setting/Reading the Configuration Parameters
*******************************************************************************/
SQL_API_RC SQL_API_FN                            /* Set/Reset Config Params   */
  db2CfgSet (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

SQL_API_RC SQL_API_FN                            /* Read Config Params        */
  db2CfgGet (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/******************************************************************************
** Structure (generic) for a single Configuration Parameter
*******************************************************************************/
typedef SQL_STRUCTURE db2gCfgParam
{
   db2Uint32                           token;    /* Parameter Identifier      */
   db2Uint32                           ptrvalue_len; /* length of ptrvalue    */
   char                                *ptrvalue; /* Parameter value          */
   db2Uint32                           flags;    /* flags for this parameter  */
} db2gCfgParam;

/******************************************************************************
** Main (generic) structure for db2CfgSet() and db2CfgGet()
*******************************************************************************/
typedef SQL_STRUCTURE db2gCfg
{
   db2Uint32                           numItems; /* Number of configuration   */
                                                 /* parameters in the         */
                                                 /* following array           */
   struct db2gCfgParam                 *paramArray; /* Array of cfg           */
                                                 /* parameters                */
   db2Uint32                           flags;    /* Various properties        */
   db2Uint32                           dbname_len; /* length of dbname        */
   char                                *dbname;  /* Database Name, if needed  */
} db2gCfg;

/******************************************************************************
** (Generic) API for Setting/Reading the Configuration Parameters
*******************************************************************************/
SQL_API_RC SQL_API_FN                            /* Set/Reset Config Params   */
  db2gCfgSet (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

SQL_API_RC SQL_API_FN                            /* Read Config Params        */
  db2gCfgGet (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */




#define SQL_CS_SYSTEM_NLSCHAR          -8        /* SYSTEM table + NLS        */
                                                 /* function                  */
#define SQL_CS_USER_NLSCHAR            -9        /* USER table + NLS          */
                                                 /* function                  */
/******************************************************************************
** Node List Operations Constants
*******************************************************************************/
#define DB2_NODE_LIST        0            /* submit to all nodes in the node  */
                                          /* list                             */
#define DB2_ALL_NODES        1            /* submit to all nodes in the node  */
                                          /* configuration file               */
#define DB2_ALL_EXCEPT       2            /* submit to all nodes except the   */
                                          /* ones specified by the nodelist   */
                                          /* parameter                        */
#define DB2_CAT_NODE_ONLY    3            /* submit to catalog node only      */
/******************************************************************************
** Load Query Structures and Constants
*******************************************************************************/

/* Possible values for "iStringType"                                          */
#define DB2LOADQUERY_TABLENAME         0         /* Tablename specified       */
#define DB2LOADQUERY_TEMPPATHNAME      1

/* Possible values for "iShowLoadMessages"                                    */
#define DB2LOADQUERY_SHOW_ALL_MSGS     0         /* Show ALL LOAD messages    */
#define DB2LOADQUERY_SHOW_NO_MSGS      1         /* Show NO LOAD messages     */
#define DB2LOADQUERY_SHOW_NEW_MSGS     2         /* Show only NEW LOAD        */
                                                 /* messages                  */

/* Possible values for "oWhichPhase"                                          */
#define DB2LOADQUERY_LOAD_PHASE        1         /* Load Phase                */
#define DB2LOADQUERY_BUILD_PHASE       2         /* Build Phase               */
#define DB2LOADQUERY_DELETE_PHASE      3         /* Delete Phase              */
#define DB2LOADQUERY_INDEXCOPY_PHASE   4         /* Index Copy Phase          */

/* Possible values for "oTablestate"                                          */
#define DB2LOADQUERY_NORMAL            0x0       /* Normal tablestate         */
#define DB2LOADQUERY_CHECK_PENDING     0x1       /* Table in check pending    */
                                                 /* state                     */
#define DB2LOADQUERY_LOAD_IN_PROGRESS  0x2       /* Load in progress on       */
                                                 /* table                     */
#define DB2LOADQUERY_READ_ACCESS       0x4       /* Table in read access      */
                                                 /* only state                */
#define DB2LOADQUERY_NOTAVAILABLE      0x8       /* Table in unavailable      */
                                                 /* state                     */
#define DB2LOADQUERY_NO_LOAD_RESTART   0x10      /* Table not in load         */
                                                 /* restartable state         */
#define DB2LOADQUERY_UNCHANGED         0x20      /* Table state has not       */
                                                 /* changed                   */
#define DB2LOADQUERY_LOAD_PENDING      0x40      /* Table pending completion  */
                                                 /* of load                   */
#define DB2LOADQUERY_NO_DATA_MOVEMENT  0x80      /* Table in no data          */
                                                 /* movement state            */
#define DB2LOADQUERY_RECONCILE_PENDING 0x100     /* Table in datalink         */
                                                 /* reconcile pending state   */
#define DB2LOADQUERY_RECONCILE_NOT_POS 0x200     /* Table in datalink         */
                                                 /* reconcile not possible    */
                                                 /* state                     */
#define DB2LOADQUERY_TYPE1_INDEXES     0x400     /* Table has type-1 indexes  */

/* Load Query Output Structure                                                */
typedef SQL_STRUCTURE db2LoadQueryOutputStruct
{
   db2Uint32                           oRowsRead; /* Rows Read                */
   db2Uint32                           oRowsSkipped; /* Rows Skipped          */
   db2Uint32                           oRowsCommitted; /* Rows Committed      */
   db2Uint32                           oRowsLoaded; /* Rows Loaded            */
   db2Uint32                           oRowsRejected; /* Rows Rejected        */
   db2Uint32                           oRowsDeleted; /* Rows Deleted          */
   db2Uint32                           oCurrentIndex; /* Current Index        */
                                                 /* (BUILD PHASE)             */
   db2Uint32                           oNumTotalIndexes; /* Total # of        */
                                                 /* Indexes to build (BUILD   */
                                                 /* PHASE)                    */
   db2Uint32                           oCurrentMPPNode; /* Node being         */
                                                 /* queried (MPP only)        */
   db2Uint32                           oLoadRestarted; /* Load Restart        */
                                                 /* indicator                 */
   db2Uint32                           oWhichPhase; /* Phase of queried Load  */
   db2Uint32                           oWarningCount; /* Warning Count        */
   db2Uint32                           oTableState; /* Table State            */
} db2LoadQueryOutputStruct;

/* Load Query Parameter Structure                                             */
typedef SQL_STRUCTURE db2LoadQueryStruct
{
   db2Uint32                           iStringType; /* Type of piString       */
   char                                *piString; /* Name to query            */
   db2Uint32                           iShowLoadMessages; /* Level of Load    */
                                                 /* message reporting         */
   struct db2LoadQueryOutputStruct     *poOutputStruct; /* Load Query Output  */
                                                 /* Structure                 */
   char                                *piLocalMessageFile; /* Message File   */
} db2LoadQueryStruct;

SQL_API_RC SQL_API_FN                            /* Load Query                */
  db2LoadQuery (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/* Generic Load Query Parameter Structure                                     */
typedef SQL_STRUCTURE db2gLoadQueryStruct
{
   db2Uint32                           iStringType; /* Type of piString       */
   db2Uint32                           iStringLen; /* Length in bytes of      */
                                                 /* piString                  */
   char                                *piString; /* Name to query            */
   db2Uint32                           iShowLoadMessages; /* Level of Load    */
                                                 /* message reporting         */
   struct db2LoadQueryOutputStruct     *poOutputStruct; /* Load Query Output  */
                                                 /* Structure                 */
   db2Uint32                           iLocalMessageFileLen; /* Length in     */
                                                 /* bytes of message file     */
                                                 /* variable                  */
   char                                *piLocalMessageFile; /* Message File   */
} db2gLoadQueryStruct;

SQL_API_RC SQL_API_FN                            /* Generic Load Query        */
  db2gLoadQuery (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/******************************************************************************
** Load Structures and Constants
*******************************************************************************/

/* General db2Load constants                                                  */
#define DB2LOAD_MAX_HOSTNAME_SZ        255       /* Max size of               */
                                                 /* db2PartLoadIn -           */
                                                 /* iHostName                 */
#define DB2LOAD_MAX_FILETRANSFERCMD_SZ 254       /* Max size of               */
                                                 /* db2PartLoadIn -           */
                                                 /* iFileTransferCmd          */
#define DB2LOAD_MAX_PATH_SZ            260       /* Max size of Load path     */
                                                 /* strings                   */

/* Load input structure                                                       */
typedef SQL_STRUCTURE db2LoadIn
{
   db2Uint64                           iRowcount; /* Row count                */
   db2Uint64                           iRestartcount; /* Restart count        */
   char                                *piUseTablespace; /* Alternative       */
                                                 /* Tablespace to rebuild     */
                                                 /* index                     */
   db2Uint32                           iSavecount; /* Save count              */
   db2Uint32                           iDataBufferSize; /* Data buffer        */
   db2Uint32                           iSortBufferSize; /* Sort buffer (for   */
                                                 /* vendor load sort)         */
   db2Uint32                           iWarningcount; /* Warning count        */
   db2Uint16                           iHoldQuiesce; /* Hold quiesce between  */
                                                 /* loads                     */
   db2Uint16                           iCpuParallelism; /* CPU parallelism    */
   db2Uint16                           iDiskParallelism; /* Disk parallelism  */
   db2Uint16                           iNonrecoverable; /* Non-recoverable    */
                                                 /* load                      */
   db2Uint16                           iIndexingMode; /* Indexing mode        */
   db2Uint16                           iAccessLevel; /* Access Level          */
   db2Uint16                           iLockWithForce; /* Lock With Force     */
   db2Uint16                           iCheckPending; /* Check Pending        */
                                                 /* Option                    */
   char                                iRestartphase; /* Restart phase        */
   char                                iStatsOpt; /* Statistics option        */
} db2LoadIn;

/* Load output structure                                                      */
typedef SQL_STRUCTURE db2LoadOut
{
   db2Uint64                           oRowsRead; /* Number of rows read      */
   db2Uint64                           oRowsSkipped; /* Number of rows        */
                                                 /* skipped                   */
   db2Uint64                           oRowsLoaded; /* Number of rows loaded  */
   db2Uint64                           oRowsRejected; /* Number of rows       */
                                                 /* rejected                  */
   db2Uint64                           oRowsDeleted; /* Number of rows        */
                                                 /* deleted                   */
   db2Uint64                           oRowsCommitted; /* Number of rows      */
                                                 /* committed                 */
} db2LoadOut;

/*                                                                            */
/* Partitioned Load structures and constants. These structures are only       */
/* needed when loading into partitioned databases                             */
/*                                                                            */

/* Load node list structure                                                   */
typedef SQL_STRUCTURE db2LoadNodeList
{
   SQL_PDB_NODE_TYPE                   *piNodeList; /* Array of nodes         */
   db2Uint16                           iNumNodes; /* Number of nodes          */
} db2LoadNodeList;

/* Load port range structure                                                  */
typedef SQL_STRUCTURE db2LoadPortRange
{
   db2Uint16                           iPortMin; /* Minimum port number       */
   db2Uint16                           iPortMax; /* Maximum port number       */
} db2LoadPortRange;

/* Possible values for db2PartLoadInfoIn - iMode                              */
#define DB2LOAD_PARTITION_AND_LOAD     0         /* Partition and Load mode   */
#define DB2LOAD_PARTITION_ONLY         1         /* Partition only mode       */
#define DB2LOAD_LOAD_ONLY              2         /* Load only mode            */
#define DB2LOAD_LOAD_ONLY_VERIFY_PART  3         /* Load file with no         */
                                                 /* partition header          */
#define DB2LOAD_ANALYZE                4         /* Generate optimal          */
                                                 /* partition map             */

/* Possible values for db2PartLoadInfoIn - iIsolatePartErrs                   */
#define DB2LOAD_SETUP_ERRS_ONLY        0         /* Setup errors only         */
#define DB2LOAD_LOAD_ERRS_ONLY         1         /* Load errors only          */
#define DB2LOAD_SETUP_AND_LOAD_ERRS    2         /* Setup and Load errors     */
#define DB2LOAD_NO_ISOLATION           3         /* No partition error        */
                                                 /* isolation                 */

/* Partitioned Load input structure                                           */
/*   -- assign NULL to a field to indicate default value is desired           */
typedef SQL_STRUCTURE db2PartLoadIn
{
   char                                *piHostname; /* Hostname for           */
                                                 /* iFileTransferCmd          */
                                                 /* parameter                 */
   char                                *piFileTransferCmd; /* File transfer   */
                                                 /* command                   */
   char                                *piPartFileLocation; /* Partition      */
                                                 /* file location             */
   struct db2LoadNodeList              *piOutputNodes; /* Output nodes        */
   struct db2LoadNodeList              *piPartitioningNodes; /* Partitioning  */
                                                 /* nodes                     */
   db2Uint16                           *piMode;  /* Partitioned Load mode     */
   db2Uint16                           *piMaxNumPartAgents; /* Max number of  */
                                                 /* partitioning agents       */
   db2Uint16                           *piIsolatePartErrs; /* Partition       */
                                                 /* error isolation mode      */
   db2Uint16                           *piStatusInterval; /* Status report    */
                                                 /* interval                  */
   struct db2LoadPortRange             *piPortRange; /* Port number range     */
   db2Uint16                           *piCheckTruncation; /* Check for       */
                                                 /* record truncation         */
   char                                *piMapFileInput; /* Map file input     */
   char                                *piMapFileOutput; /* Map file output   */
   db2Uint16                           *piTrace; /* Number of records to      */
                                                 /* trace                     */
   db2Uint16                           *piNewline; /* Check for newlines at   */
                                                 /* end of ASC records        */
   char                                *piDistfile; /* Partition              */
                                                 /* distribution output file  */
   db2Uint16                           *piOmitHeader; /* Don't generate       */
                                                 /* partition header in       */
                                                 /* output file               */
   SQL_PDB_NODE_TYPE                   *piRunStatDBPartNum; /* runstat node   */
} db2PartLoadIn;

/* Possible values for db2LoadAgentInfo - oAgentType                          */
#define DB2LOAD_LOAD_AGENT             0         /* Load agent (one per       */
                                                 /* output node, not used     */
                                                 /* for PARTITION_ONLY and    */
                                                 /* ANALYZE modes)            */
#define DB2LOAD_PARTITIONING_AGENT     1         /* Partitioning agent (one   */
                                                 /* per partitioning node,    */
                                                 /* not used for LOAD_ONLY    */
                                                 /* and LOAD_ONLY_VERIFY      */
                                                 /* PART modes)               */
#define DB2LOAD_PRE_PARTITIONING_AGENT 2         /* Pre-partitioning agent    */
                                                 /* (one at coordinator       */
                                                 /* node, not used for LOAD   */
                                                 /* ONLY and LOAD_ONLY        */
                                                 /* VERIFY_PART modes)        */
#define DB2LOAD_FILE_TRANSFER_AGENT    3         /* File transfer agent       */
                                                 /* (only for FILE_TRANSFER   */
                                                 /* CMD option)               */
#define DB2LOAD_LOAD_TO_FILE_AGENT     4         /* For PARTITION_ONLY mode   */
                                                 /* (one per output node)     */

/* Load agent info structure.  Load will generate an agent info structure     */
/* for each agent working on behalf of the Load command                       */
typedef SQL_STRUCTURE db2LoadAgentInfo
{
   db2int32                            oSqlcode; /* Agent sqlcode             */
   db2Uint32                           oTableState; /* Table state (only      */
                                                 /* relevant for agents of    */
                                                 /* type DB2LOAD_LOAD_AGENT)  */
   SQL_PDB_NODE_TYPE                   oNodeNum; /* Node on which agent       */
                                                 /* executed                  */
   db2Uint16                           oAgentType; /* Agent type (see above   */
                                                 /* for possible values)      */
} db2LoadAgentInfo;

/* Partitioned Load output structure                                          */
typedef SQL_STRUCTURE db2PartLoadOut
{
   db2Uint64                           oRowsRdPartAgents; /* Rows read by     */
                                                 /* partitioning agents       */
   db2Uint64                           oRowsRejPartAgents; /* Rows rejected   */
                                                 /* by partitioning agents    */
   db2Uint64                           oRowsPartitioned; /* Rows partitioned  */
                                                 /* by partitioning agents    */
   struct db2LoadAgentInfo             *poAgentInfoList; /* Node output info  */
                                                 /* list                      */
   db2Uint32                           iMaxAgentInfoEntries; /* Max number    */
                                                 /* of agent info entries     */
                                                 /* allocated by user for     */
                                                 /* poAgentInfoList. This     */
                                                 /* should at least include   */
                                                 /* space for the             */
                                                 /* partitioning agents,      */
                                                 /* load agents, and one pre  */
                                                 /* partitioning agent        */
   db2Uint32                           oNumAgentInfoEntries; /* Number of     */
                                                 /* agent info entries        */
                                                 /* produced by load.  The    */
                                                 /* number of entries in      */
                                                 /* poAgentInfoList is        */
                                                 /* min(iMaxAgentInfoEntries  */
                                                 /*  oNumAgentInfoEntries).   */
} db2PartLoadOut;

/*                                                                            */
/* db2Load parameter structure                                                */
/*   -- For non-partitioned database loads, set piLoadInfoIn and              */
/* poLoadInfoOut to NULL                                                      */
/*   -- For partitioned database loads, set piLoadInfoIn to NULL to request   */
/* default values for all partitioned database load options                   */
/*                                                                            */
typedef SQL_STRUCTURE db2LoadStruct
{
   struct sqlu_media_list              *piSourceList; /* List of input        */
                                                 /* source names (files,      */
                                                 /* pipes, etc.)              */
   struct sqlu_media_list              *piLobPathList; /* Lob file paths      */
   struct sqldcol                      *piDataDescriptor; /* Data descriptor  */
                                                 /* list                      */
   struct sqlchar                      *piActionString; /* Load action        */
                                                 /* string                    */
   char                                *piFileType; /* File type (ASC, DEL,   */
                                                 /* IXF, WSF, etc.)           */
   struct sqlchar                      *piFileTypeMod; /* File type modifier  */
                                                 /* string                    */
   char                                *piLocalMsgFileName; /* Message        */
                                                 /* filename                  */
   char                                *piTempFilesPath; /* Temporary files   */
                                                 /* path                      */
   struct sqlu_media_list              *piVendorSortWorkPaths; /* Vendor      */
                                                 /* Sort work directories     */
   struct sqlu_media_list              *piCopyTargetList; /* List of Load     */
                                                 /* copy targets              */
   db2int32                            *piNullIndicators; /* Null indicators  */
   struct db2LoadIn                    *piLoadInfoIn; /* Load input           */
                                                 /* structure                 */
   struct db2LoadOut                   *poLoadInfoOut; /* Load output         */
                                                 /* structure                 */
   struct db2PartLoadIn                *piPartLoadInfoIn; /* Partitioned      */
                                                 /* Load input structure      */
   struct db2PartLoadOut               *poPartLoadInfoOut; /* Partitioned     */
                                                 /* Load output structure     */
   db2int16                            iCallerAction; /* Caller action        */
} db2LoadStruct;

/* db2Load - API                                                              */
SQL_API_RC SQL_API_FN                            /* Load                      */
  db2Load (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/*                                                                            */
/* db2gLoad structures                                                        */
/*                                                                            */

/* Generic Load input structure                                               */
typedef SQL_STRUCTURE db2gLoadIn
{
   db2Uint64                           iRowcount; /* Row count                */
   db2Uint64                           iRestartcount; /* Restart count        */
   char                                *piUseTablespace; /* Alternative       */
                                                 /* Tablespace to rebuild     */
                                                 /* index                     */
   db2Uint32                           iSavecount; /* Save count              */
   db2Uint32                           iDataBufferSize; /* Data buffer        */
   db2Uint32                           iSortBufferSize; /* Sort buffer (for   */
                                                 /* vendor load sort)         */
   db2Uint32                           iWarningcount; /* Warning count        */
   db2Uint16                           iHoldQuiesce; /* Hold quiesce between  */
                                                 /* loads                     */
   db2Uint16                           iCpuParallelism; /* CPU parallelism    */
   db2Uint16                           iDiskParallelism; /* Disk parallelism  */
   db2Uint16                           iNonrecoverable; /* Non-recoverable    */
                                                 /* load                      */
   db2Uint16                           iIndexingMode; /* Indexing mode        */
   db2Uint16                           iAccessLevel; /* Access Level          */
   db2Uint16                           iLockWithForce; /* Lock With Force     */
   db2Uint16                           iCheckPending; /* Check Pending        */
                                                 /* Option                    */
   char                                iRestartphase; /* Restart phase        */
   char                                iStatsOpt; /* Statistics option        */
   db2Uint16                           iUseTablespaceLen; /* Length of        */
                                                 /* piUseTablespace string    */
} db2gLoadIn;

/* Generic Partitioned Load input structure                                   */
typedef SQL_STRUCTURE db2gPartLoadIn
{
   char                                *piHostname; /* Hostname for           */
                                                 /* iFileTransferCmd          */
                                                 /* parameter                 */
   char                                *piFileTransferCmd; /* File transfer   */
                                                 /* command                   */
   char                                *piPartFileLocation; /* Partition      */
                                                 /* file location             */
   struct db2LoadNodeList              *piOutputNodes; /* Output nodes        */
   struct db2LoadNodeList              *piPartitioningNodes; /* Partitioning  */
                                                 /* nodes                     */
   db2Uint16                           *piMode;  /* Partitioned Load mode     */
   db2Uint16                           *piMaxNumPartAgents; /* Max number of  */
                                                 /* partitioning agents       */
   db2Uint16                           *piIsolatePartErrs; /* Partition       */
                                                 /* error isolation mode      */
   db2Uint16                           *piStatusInterval; /* Status report    */
                                                 /* interval                  */
   struct db2LoadPortRange             *piPortRange; /* Port number range     */
   db2Uint16                           *piCheckTruncation; /* Check for       */
                                                 /* record truncation         */
   char                                *piMapFileInput; /* Map file input     */
   char                                *piMapFileOutput; /* Map file output   */
   db2Uint16                           *piTrace; /* Number of records to      */
                                                 /* trace                     */
   db2Uint16                           *piNewline; /* Check for newlines at   */
                                                 /* end of ASC records        */
   char                                *piDistfile; /* Partition              */
                                                 /* distribution output file  */
   db2Uint16                           *piOmitHeader; /* Don't generate       */
                                                 /* partition header in       */
                                                 /* output file               */
   void                                *piReserved1; /* Reserved parameter 1  */
   db2Uint16                           iHostnameLen; /* Length of iHostname   */
                                                 /* string                    */
   db2Uint16                           iFileTransferLen; /* Length of         */
                                                 /* iFileTransferCmd string   */
   db2Uint16                           iPartFileLocLen; /* Length of          */
                                                 /* iPartFileLocation string  */
   db2Uint16                           iMapFileInputLen; /* Length of         */
                                                 /* iMapFileInput string      */
   db2Uint16                           iMapFileOutputLen; /* Length of        */
                                                 /* iMapFileOutput string     */
   db2Uint16                           iDistfileLen; /* Length of iDistfile   */
                                                 /* string                    */
} db2gPartLoadIn;

/* db2gLoad parameter structure                                               */
typedef SQL_STRUCTURE db2gLoadStruct
{
   struct sqlu_media_list              *piSourceList; /* List of input        */
                                                 /* source names (files,      */
                                                 /* pipes, etc.)              */
   struct sqlu_media_list              *piLobPathList; /* Lob file paths      */
   struct sqldcol                      *piDataDescriptor; /* Data descriptor  */
                                                 /* list                      */
   struct sqlchar                      *piActionString; /* Load action        */
                                                 /* string                    */
   char                                *piFileType; /* File type (ASC, DEL,   */
                                                 /* IXF, WSF, etc.)           */
   struct sqlchar                      *piFileTypeMod; /* File type modifier  */
                                                 /* string                    */
   char                                *piLocalMsgFileName; /* Message        */
                                                 /* filename                  */
   char                                *piTempFilesPath; /* Temporary files   */
                                                 /* path                      */
   struct sqlu_media_list              *piVendorSortWorkPaths; /* Vendor      */
                                                 /* Sort work directories     */
   struct sqlu_media_list              *piCopyTargetList; /* List of Load     */
                                                 /* copy targets              */
   db2int32                            *piNullIndicators; /* Null indicators  */
   struct db2gLoadIn                   *piLoadInfoIn; /* Load input           */
                                                 /* structure                 */
   struct db2LoadOut                   *poLoadInfoOut; /* Load output         */
                                                 /* structure                 */
   struct db2gPartLoadIn               *piPartLoadInfoIn; /* Partitioned      */
                                                 /* Load input structure      */
   struct db2PartLoadOut               *poPartLoadInfoOut; /* Partitioned     */
                                                 /* Load output structure     */
   db2int16                            iCallerAction; /* Caller action        */
   db2Uint16                           iFileTypeLen; /* Length of iFileType   */
                                                 /* string                    */
   db2Uint16                           iLocalMsgFileLen; /* Length of         */
                                                 /* iLocalMsgFileName string  */
   db2Uint16                           iTempFilesPathLen; /* Length of        */
                                                 /* iTempFilesPath string     */
} db2gLoadStruct;

/* db2Load - Generic API                                                      */
SQL_API_RC SQL_API_FN                            /* Load                      */
  db2gLoad (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/******************************************************************************
** Import Structures and Constants
*******************************************************************************/

#define DB2IMPORT_LOCKTIMEOUT          0         /* Respect db cfg            */
                                                 /* LOCKTIMEOUT               */
#define DB2IMPORT_NO_LOCKTIMEOUT       1         /* Override db cfg           */
                                                 /* LOCKTIMEOUT               */
#define DB2IMPORT_COMMIT_AUTO          -1        /* Use automatic             */
                                                 /* commitcount               */

/* Import input structure                                                     */
typedef SQL_STRUCTURE db2ImportIn
{
   db2Uint64                           iRowcount; /* Row count                */
   db2Uint64                           iRestartcount; /* Restart count        */
   db2Uint64                           iSkipcount; /* Skip count              */
   db2int32                            *piCommitcount; /* Commit count        */
   db2Uint32                           iWarningcount; /* Warning count        */
   db2Uint16                           iNoTimeout; /* No lock timeout         */
   db2Uint16                           iAccessLevel; /* Access level          */
} db2ImportIn;

/* Import output structure                                                    */
typedef SQL_STRUCTURE db2ImportOut
{
   db2Uint64                           oRowsRead; /* Rows read                */
   db2Uint64                           oRowsSkipped; /* Rows skipped          */
   db2Uint64                           oRowsInserted; /* Rows inserted        */
   db2Uint64                           oRowsUpdated; /* Rows updated          */
   db2Uint64                           oRowsRejected; /* Rows rejected        */
   db2Uint64                           oRowsCommitted; /* Rows committed      */
} db2ImportOut;

/* db2Import parameter structure                                              */
typedef SQL_STRUCTURE db2ImportStruct
{
   char                                *piDataFileName; /* Data file name     */
   struct sqlu_media_list              *piLobPathList; /* Lob file paths      */
   struct sqldcol                      *piDataDescriptor; /* Data descriptor  */
                                                 /* list                      */
   struct sqlchar                      *piActionString; /* Import action      */
                                                 /* string                    */
   char                                *piFileType; /* File type (ASC, DEL,   */
                                                 /* IXF, WSF, etc.)           */
   struct sqlchar                      *piFileTypeMod; /* File type modifier  */
                                                 /* string                    */
   char                                *piMsgFileName; /* Message filename    */
   db2int16                            iCallerAction; /* Caller action        */
   struct db2ImportIn                  *piImportInfoIn; /* Import input       */
                                                 /* structure                 */
   struct db2ImportOut                 *poImportInfoOut; /* Import output     */
                                                 /* structure                 */
   db2int32                            *piNullIndicators; /* Null indicators  */
} db2ImportStruct;

/* db2Import - API                                                            */
SQL_API_RC SQL_API_FN                            /* Import                    */
  db2Import (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/* Generic Import input structure                                             */
typedef SQL_STRUCTURE db2gImportIn
{
   db2Uint64                           iRowcount; /* Row count                */
   db2Uint64                           iRestartcount; /* Restart count        */
   db2Uint64                           iSkipcount; /* Skip count              */
   db2int32                            *piCommitcount; /* Commit count        */
   db2Uint32                           iWarningcount; /* Warning count        */
   db2Uint16                           iNoTimeout; /* No lock timeout         */
   db2Uint16                           iAccessLevel; /* Access level          */
} db2gImportIn;

/* Generic Import output structure                                            */
typedef SQL_STRUCTURE db2gImportOut
{
   db2Uint64                           oRowsRead; /* Rows read                */
   db2Uint64                           oRowsSkipped; /* Rows skipped          */
   db2Uint64                           oRowsInserted; /* Rows inserted        */
   db2Uint64                           oRowsUpdated; /* Rows updated          */
   db2Uint64                           oRowsRejected; /* Rows rejected        */
   db2Uint64                           oRowsCommitted; /* Rows committed      */
} db2gImportOut;

/* db2gImport parameter structure                                             */
typedef SQL_STRUCTURE db2gImportStruct
{
   char                                *piDataFileName; /* Data file name     */
   struct sqlu_media_list              *piLobPathList; /* Lob file paths      */
   struct sqldcol                      *piDataDescriptor; /* Data descriptor  */
                                                 /* list                      */
   struct sqlchar                      *piActionString; /* Import action      */
                                                 /* string                    */
   char                                *piFileType; /* File type (ASC, DEL,   */
                                                 /* IXF, WSF, etc.)           */
   struct sqlchar                      *piFileTypeMod; /* File type modifier  */
                                                 /* string                    */
   char                                *piMsgFileName; /* Message filename    */
   db2int16                            iCallerAction; /* Caller action        */
   struct db2gImportIn                 *piImportInfoIn; /* Import input       */
                                                 /* structure                 */
   struct db2gImportOut                *poImportInfoOut; /* Import output     */
                                                 /* structure                 */
   db2int32                            *piNullIndicators; /* Null indicators  */
   db2Uint16                           iDataFileNameLen; /* Length of         */
                                                 /* iDataFileName string      */
   db2Uint16                           iFileTypeLen; /* Length of iFileType   */
                                                 /* string                    */
   db2Uint16                           iMsgFileNameLen; /* Length of          */
                                                 /* iMsgFileName string       */
} db2gImportStruct;

/* db2gImport - Generic API                                                   */
SQL_API_RC SQL_API_FN                            /* Import                    */
  db2gImport (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/******************************************************************************
** Export Structures and Constants
*******************************************************************************/

/* Export output structure                                                    */
typedef SQL_STRUCTURE db2ExportOut
{
   db2Uint64                           oRowsExported; /* Rows Exported        */
} db2ExportOut;

/* db2Export parameter structure                                              */
typedef SQL_STRUCTURE db2ExportStruct
{
   char                                *piDataFileName; /* Data file name     */
   struct sqlu_media_list              *piLobPathList; /* Lob file paths      */
   struct sqlu_media_list              *piLobFileList; /* Lob file lists      */
   struct sqldcol                      *piDataDescriptor; /* Data descriptor  */
                                                 /* list                      */
   struct sqllob                       *piActionString; /* Export action      */
                                                 /* string                    */
   char                                *piFileType; /* File type (ASC, DEL,   */
                                                 /* IXF, WSF, etc.)           */
   struct sqlchar                      *piFileTypeMod; /* File type modifier  */
                                                 /* string                    */
   char                                *piMsgFileName; /* Message filename    */
   db2int16                            iCallerAction; /* Caller action        */
   struct db2ExportOut                 *poExportInfoOut; /* Export output     */
                                                 /* structure                 */
} db2ExportStruct;

/* db2Export - API                                                            */
SQL_API_RC SQL_API_FN                            /* Export                    */
  db2Export (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/* db2gExport parameter structure                                             */
typedef SQL_STRUCTURE db2gExportStruct
{
   char                                *piDataFileName; /* Data file name     */
   struct sqlu_media_list              *piLobPathList; /* Lob file paths      */
   struct sqlu_media_list              *piLobFileList; /* Lob file files      */
   struct sqldcol                      *piDataDescriptor; /* Data descriptor  */
                                                 /* list                      */
   struct sqllob                       *piActionString; /* Export action      */
                                                 /* string                    */
   char                                *piFileType; /* File type (ASC, DEL,   */
                                                 /* IXF, WSF, etc.)           */
   struct sqlchar                      *piFileTypeMod; /* File type modifier  */
                                                 /* string                    */
   char                                *piMsgFileName; /* Message filename    */
   db2int16                            iCallerAction; /* Caller action        */
   struct db2ExportOut                 *poExportInfoOut; /* Export output     */
                                                 /* structure                 */
   db2Uint16                           iDataFileNameLen; /* Length of         */
                                                 /* piDataFileName string     */
   db2Uint16                           iFileTypeLen; /* Length of piFileType  */
                                                 /* string                    */
   db2Uint16                           iMsgFileNameLen; /* Length of          */
                                                 /* piMsgFileName string      */
} db2gExportStruct;

/* db2gExport - Generic API                                                   */
SQL_API_RC SQL_API_FN                            /* Export                    */
  db2gExport (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/******************************************************************************
** Open History Scan Definitions
*******************************************************************************/

/* Values for db2HistoryOpen - iCallerAction                                  */
#define DB2HISTORY_LIST_HISTORY        0
#define DB2HISTORY_LIST_BACKUP         1
#define DB2HISTORY_LIST_ROLLFORWARD    2
#define DB2HISTORY_LIST_REORG          4
#define DB2HISTORY_LIST_ALT_TABLESPACE 5
#define DB2HISTORY_LIST_DROPPED_TABLE  6
#define DB2HISTORY_LIST_LOAD           7
#define DB2HISTORY_LIST_REN_TABLESPACE 8
#define DB2HISTORY_LIST_CRT_TABLESPACE 9
#define DB2HISTORY_LIST_ARCHIVE_LOG    10

/* db2HistoryOpen input struct                                                */
typedef SQL_STRUCTURE db2HistoryOpenStruct
{
   char                                *piDatabaseAlias; /* DB to fetch       */
                                                 /* history from              */
   char                                *piTimestamp; /* Since this timestamp  */
   char                                *piObjectName; /* Entries containing   */
                                                 /* this object               */
   db2Uint32                           oNumRows; /* # of entries matching     */
                                                 /* search                    */
   db2Uint32                           oMaxTbspaces; /* Max. # of tablespace  */
                                                 /* names stored with ANY     */
                                                 /* history entry             */
   db2Uint16                           iCallerAction; /* Caller action        */
   db2Uint16                           oHandle;  /* Handle for this scan      */
} db2HistoryOpenStruct;

/* db2HistoryOpenScan - API                                                   */
SQL_API_RC SQL_API_FN                  /* Open a history file scan            */
  db2HistoryOpenScan (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* In/out parameters                   */
       struct sqlca * pSqlca);         /* SQLCA                               */

/* db2gHistoryOpen input struct                                               */
typedef SQL_STRUCTURE db2gHistoryOpenStruct
{
   char                                *piDatabaseAlias; /* DB to fetch       */
                                                 /* history from              */
   char                                *piTimestamp; /* Since this timestamp  */
   char                                *piObjectName; /* Entries containing   */
                                                 /* this object               */
   db2Uint32                           iAliasLen; /* Length of database       */
                                                 /* alias string              */
   db2Uint32                           iTimestampLen; /* Length of timestamp  */
                                                 /* string                    */
   db2Uint32                           iObjectNameLen; /* Length of object    */
                                                 /* name string               */
   db2Uint32                           oNumRows; /* # of entries matching     */
                                                 /* search                    */
   db2Uint32                           oMaxTbspaces; /* Max. # of tablespace  */
                                                 /* names stored with ANY     */
                                                 /* history entry             */
   db2Uint16                           iCallerAction; /* Caller action        */
   db2Uint16                           oHandle;  /* Handle for this scan      */
} db2gHistoryOpenStruct;

/* db2gHistoryOpenScan - Generic API                                          */
SQL_API_RC SQL_API_FN                  /* Open a history file scan            */
  db2gHistoryOpenScan (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* In/out parameters                   */
       struct sqlca * pSqlca);         /* SQLCA                               */

/******************************************************************************
** Fetch Next History Entry Definitions
*******************************************************************************/

/* Structure db2HistoryEID                                                    */
typedef SQL_STRUCTURE db2HistoryEID
{
   SQL_PDB_NODE_TYPE                   ioNode;   /* Node number               */
   db2Uint32                           ioHID;    /* Local history file entry  */
                                                 /* ID                        */
} db2HistoryEID;

/* Max Sizes of db2HistoryDataStruct db2Char fields                           */
/* NOTE:  These lengths do not include NULL terminators                       */
#define DB2HISTORY_TIMESTAMP_SZ        14
#define DB2HISTORY_SEQNUM_SZ           3
#define DB2HISTORY_OBJPART_SZ          (DB2HISTORY_TIMESTAMP_SZ + DB2HISTORY_SEQNUM_SZ)
#define DB2HISTORY_LOGFILE_SZ          12
#define DB2HISTORY_ID_SZ               24
#define DB2HISTORY_TABLE_QUAL_SZ       30
#define DB2HISTORY_TABLE_NAME_SZ       128
#define DB2HISTORY_LOCATION_SZ         255
#define DB2HISTORY_COMMENT_SZ          30
#define DB2HISTORY_COMMAND_SZ          SQL_MAX_STMT_SIZ

/* Structure db2HistoryData                                                   */
/*                                                                            */
/* Description:                                                               */
/*      ioHistDataID      -- Eye catcher, must be set to "SQLUHINF"           */
/*                                                                            */
/*      oObjectPart       -- Start time + sequence number                     */
/*                        -- DB2HISTORY_OBJPART_SZ                            */
/*                                                                            */
/*      oEndTime          -- Completion timestamp                             */
/*                        -- DB2HISTORY_TIMESTAMP_SZ                          */
/*                                                                            */
/*      oFirstLog         -- First log file referrenced by event              */
/*                        -- DB2HISTORY_LOGFILE_SZ                            */
/*                                                                            */
/*      oLastLog          -- Last log file referrenced by event               */
/*                        -- DB2HISTORY_LOGFILE_SZ                            */
/*                                                                            */
/*      oID               -- Backup ID (timestamp), or unique table ID        */
/*                        -- DB2HISTORY_ID_SZ                                 */
/*                                                                            */
/*      oTableQualifier   -- Table qualifier                                  */
/*                        -- DB2HISTORY_TABLE_QUAL_SZ                         */
/*                                                                            */
/*      oTableName        -- Table name                                       */
/*                        -- DB2HISTORY_TABLE_NAME_SZ                         */
/*                                                                            */
/*      oLocation         -- Location of object used or produced by event     */
/*                        -- DB2HISTORY_LOCATION_SZ                           */
/*                                                                            */
/*      oComment          -- Text comment                                     */
/*                        -- DB2HISTORY_COMMENT_SZ                            */
/*                                                                            */
/*      oCommandText      -- Command text, or DDL                             */
/*                        -- DB2HISTORY_COMMAND_SZ                            */
/*                                                                            */
/*      oLastLSN          -- Last Log Sequence Number                         */
/*      poEventSQLCA      -- SQLCA returned at event completion               */
/*      poTablespace      -- LIST of tablespace names                         */
/*      iNumTablespaces   -- # of available entries in the poTablespace list  */
/*      oNumTablespaces   -- # of used entries in the poTablespace list       */
/*      oEID              -- Unique history entry ID                          */
/*      oOperation        -- Event type:        DB2HISTORY_TYPE_*             */
/*      oObject           -- Event granularity: DB2HISTORY_GRAN_*             */
/*      oOptype           -- Event details:     DB2HISTORY_OPTYPE_*           */
/*      oStatus           -- Entry status:      DB2HISTORY_STATUS_*           */
/*      oDeviceType       -- Type of oLocation: DB2_MEDIA_*                   */

typedef SQL_STRUCTURE db2HistoryData
{
   char                                ioHistDataID[8]; /* Eye catcher =      */
                                                 /* 'SQLUHINF'                */
   db2Char                             oObjectPart; /* Start time + sequence  */
                                                 /* number                    */
   db2Char                             oEndTime; /* Completion time of event  */
   db2Char                             oFirstLog; /* First log file           */
   db2Char                             oLastLog; /* Last log file             */
   db2Char                             oID;      /* Backup ID, or unique      */
                                                 /* table ID                  */
   db2Char                             oTableQualifier; /* Table qualifier    */
   db2Char                             oTableName; /* Table name              */
   db2Char                             oLocation; /* Location of referrenced  */
                                                 /* object                    */
   db2Char                             oComment; /* Text comment              */
   db2Char                             oCommandText; /* Command text          */
   SQLU_LSN                            oLastLSN; /* Last log sequence number  */
   db2HistoryEID                       oEID;     /* Unique entry ID           */
   struct sqlca                        *poEventSQLCA; /* Resulting SQLCA of   */
                                                 /* event                     */
   struct db2Char                      *poTablespace; /* List of tablespace   */
                                                 /* names                     */
   db2Uint32                           iNumTablespaces; /* I: # of available  */
                                                 /* poTablespace fields       */
   db2Uint32                           oNumTablespaces; /* O: # of used       */
                                                 /* poTablespace fields       */
   char                                oOperation; /* Event type              */
   char                                oObject;  /* Operation granularity     */
   char                                oOptype;  /* Operation details         */
   char                                oStatus;  /* Status of entry           */
   char                                oDeviceType; /* Device type of         */
                                                 /* 'location'                */
} db2HistoryData;

/* Values for db2HistoryGetEntry - iCallerAction                              */
#define DB2HISTORY_GET_ENTRY 0     /* Next entry, without command data        */
#define DB2HISTORY_GET_DDL   1     /* ONLY command data from previous fetch   */
#define DB2HISTORY_GET_ALL   2     /* Next entry, with all data               */

/* db2HistoryGetEntry input struct                                            */
typedef SQL_STRUCTURE db2HistoryGetEntryStruct
{
   struct db2HistoryData               *pioHistData; /* History entry data    */
                                                 /* area                      */
   db2Uint16                           iHandle;  /* History scan handle       */
   db2Uint16                           iCallerAction; /* Caller action        */
} db2HistoryGetEntryStruct;

/* db2HistoryGetEntry - API                                                   */
SQL_API_RC SQL_API_FN                  /* Fetch the next history file entry   */
  db2HistoryGetEntry (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* In/out parameters                   */
       struct sqlca * pSqlca);         /* SQLCA                               */

/* db2gHistoryGetEntry - Generic API                                          */
SQL_API_RC SQL_API_FN                  /* Fetch the next history file entry   */
  db2gHistoryGetEntry (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* In/out parameters                   */
       struct sqlca * pSqlca);         /* SQLCA                               */

/* History Operations                                                         */
#define DB2HISTORY_OP_CRT_TABLESPACE   'A'
#define DB2HISTORY_OP_BACKUP           'B'
#define DB2HISTORY_OP_LOAD_COPY        'C'
#define DB2HISTORY_OP_DROPPED_TABLE    'D'
#define DB2HISTORY_OP_ROLLFWD          'F'
#define DB2HISTORY_OP_REORG            'G'
#define DB2HISTORY_OP_LOAD             'L'
#define DB2HISTORY_OP_REN_TABLESPACE   'N'
#define DB2HISTORY_OP_DROP_TABLESPACE  'O'
#define DB2HISTORY_OP_QUIESCE          'Q'
#define DB2HISTORY_OP_RESTORE          'R'
#define DB2HISTORY_OP_ALT_TABLESPACE   'T'
#define DB2HISTORY_OP_UNLOAD           'U'
#define DB2HISTORY_OP_ARCHIVE_LOG      'X'

/* History Objects                                                            */
#define DB2HISTORY_GRAN_DB             'D'
#define DB2HISTORY_GRAN_TBS            'P'
#define DB2HISTORY_GRAN_TABLE          'T'
#define DB2HISTORY_GRAN_INDEX          'I'

/* Backup Optypes                                                             */
#define DB2HISTORY_OPTYPE_OFFLINE      'F'
#define DB2HISTORY_OPTYPE_ONLINE       'N'
#define DB2HISTORY_OPTYPE_INCR_OFFLINE 'I'
#define DB2HISTORY_OPTYPE_INCR_ONLINE  'O'
#define DB2HISTORY_OPTYPE_DELT_OFFLINE 'D'
#define DB2HISTORY_OPTYPE_DELT_ONLINE  'E'

/* Quiesce Optypes                                                            */
#define DB2HISTORY_OPTYPE_SHARE        'S'
#define DB2HISTORY_OPTYPE_UPDATE       'U'
#define DB2HISTORY_OPTYPE_EXCL         'X'
#define DB2HISTORY_OPTYPE_RESET        'Z'

/* Rollforward Optypes                                                        */
#define DB2HISTORY_OPTYPE_EOL          'E'
#define DB2HISTORY_OPTYPE_PIT          'P'

/* Load Optypes                                                               */
#define DB2HISTORY_OPTYPE_INSERT       'I'
#define DB2HISTORY_OPTYPE_REPLACE      'R'

/* Alter Tablespace Optypes                                                   */
#define DB2HISTORY_OPTYPE_ADD_CONT     'C'
#define DB2HISTORY_OPTYPE_REB          'R'

/* Archive Log Optypes                                                        */
#define DB2HISTORY_OPTYPE_PRIMARY      'P'
#define DB2HISTORY_OPTYPE_MIRROR       'M'
#define DB2HISTORY_OPTYPE_ARCHFAIL     'F'
#define DB2HISTORY_OPTYPE_ARCH1        '1'
#define DB2HISTORY_OPTYPE_ARCH2        '2'

/* History Entry Status Flags                                                 */
#define DB2HISTORY_STATUS_ACTIVE       'A'
#define DB2HISTORY_STATUS_INACTIVE     'I'
#define DB2HISTORY_STATUS_EXPIRED      'E'
#define DB2HISTORY_STATUS_DELETED      'D'
#define DB2HISTORY_STATUS_NC           'N'
#define DB2HISTORY_STATUS_INCMP_ACTV   'a'
#define DB2HISTORY_STATUS_INCMP_INACTV 'i'

/******************************************************************************
** Close History Scan Definitions
*******************************************************************************/

/* db2HistoryCloseScan - API                                                  */
SQL_API_RC SQL_API_FN                  /* Close a history file scan           */
  db2HistoryCloseScan (
       db2Uint32 versionNumber,        /* Database version number             */
       void * piHandle,                /* History scan handle                 */
       struct sqlca * pSqlca);         /* SQLCA                               */


/* db2gHistoryCloseScan - Generic API                                         */
SQL_API_RC SQL_API_FN                  /* Close a history file scan           */
  db2gHistoryCloseScan (
       db2Uint32 versionNumber,        /* Database version number             */
       void * piHandle,                /* History scan handle                 */
       struct sqlca * pSqlca);         /* SQLCA                               */

/******************************************************************************
** Update History Entry Definitions
*******************************************************************************/

/* Update history API input struct                                            */
typedef SQL_STRUCTURE db2HistoryUpdateStruct
{
   char                                *piNewLocation;
   char                                *piNewDeviceType;
   char                                *piNewComment;
   char                                *piNewStatus;
   db2HistoryEID                       iEID;     /* ID of the entry to be     */
                                                 /* updated                   */
} db2HistoryUpdateStruct;

/* db2HistoryUpdate - API                                                     */
SQL_API_RC SQL_API_FN                  /* Update history entry data           */
  db2HistoryUpdate (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* Input parameters                    */
       struct sqlca * pSqlca);         /* SQLCA                               */

/* Update history API input struct                                            */
typedef SQL_STRUCTURE db2gHistoryUpdateStruct
{
   char                                *piNewLocation;
   char                                *piNewDeviceType;
   char                                *piNewComment;
   char                                *piNewStatus;
   db2Uint32                           iNewLocationLen;
   db2Uint32                           iNewDeviceLen;
   db2Uint32                           iNewCommentLen;
   db2Uint32                           iNewStatusLen;
   db2HistoryEID                       iEID;     /* ID of the entry to be     */
                                                 /* updated                   */
} db2gHistoryUpdateStruct;

/* db2gHistoryUpdate - Generic API                                            */
SQL_API_RC SQL_API_FN                  /* Update history entry data           */
  db2gHistoryUpdate (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* Input parameters                    */
       struct sqlca * pSqlca);         /* SQLCA                               */

/******************************************************************************
** Precompiler API Related Definitions
*******************************************************************************/

/* Compile SQL Parmeter Structure                                             */
typedef struct db2CompileSqlStruct
{
   db2Uint32                           *piSqlStmtLen; /* SQL statement text   */
                                                 /* length                    */
   char                                *piSqlStmt; /* SQL statement text      */
   db2Uint32                           *piLineNum; /* Source Line Number      */
   struct sqla_flaginfo                *pioFlagInfo; /* SQL Flagger           */
                                                 /* information               */
   struct sqla_tokens                  *pioTokenIdArray; /* Token ID Array    */
   struct sqla_tasks                   *poTaskArray; /* Task Array            */
   db2Uint16                           *poSectionNum; /* Section Number       */
   db2Uint16                           *poSqlStmtType; /* Type of SQL         */
                                                 /* statement                 */
   char                                *poBuffer1; /* 256 byte string buffer  */
                                                 /* 1                         */
   char                                *poBuffer2; /* 256 byte string buffer  */
                                                 /* 2                         */
   char                                *poBuffer3; /* 256 byte string buffer  */
                                                 /* 3                         */
   void                                *pioReserved; /* Reserved              */
} db2CompileSqlStruct;

/* Generic Compile SQL Parmeter Structure                                     */
typedef struct db2gCompileSqlStruct
{
   db2Uint32                           *piSqlStmtLen; /* SQL statement text   */
                                                 /* length                    */
   char                                *piSqlStmt; /* SQL statement text      */
   db2Uint32                           *piLineNum; /* Source Line Number      */
   struct sqla_flaginfo                *pioFlagInfo; /* SQL Flagger           */
                                                 /* information               */
   struct sqla_tokens                  *pioTokenIdArray; /* Token ID Array    */
   struct sqla_tasks                   *poTaskArray; /* Task Array            */
   db2Uint16                           *poSectionNum; /* Section Number       */
   db2Uint16                           *poSqlStmtType; /* Type of SQL         */
                                                 /* statement                 */
   char                                *poBuffer1; /* 256 byte string buffer  */
                                                 /* 1                         */
   char                                *poBuffer2; /* 256 byte string buffer  */
                                                 /* 2                         */
   char                                *poBuffer3; /* 256 byte string buffer  */
                                                 /* 3                         */
   void                                *pioReserved; /* Reserved              */
} db2gCompileSqlStruct;

/* Compile SQL API                                                            */
SQL_API_RC SQL_API_FN                            /* Compile an SQL statement  */
  db2CompileSql (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/* Generic Compile SQL API                                                    */
SQL_API_RC SQL_API_FN                            /* Compile an SQL statement  */
  db2gCompileSql (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/* Initialization Parmeter Structure                                          */
typedef struct db2InitStruct
{
   db2Uint16                           *piProgramNameLength; /* Program name  */
                                                 /* length                    */
   char                                *piProgramName; /* Program name        */
   db2Uint16                           *piDbNameLength; /* Database name      */
                                                 /* length                    */
   char                                *piDbName; /* Database name            */
   db2Uint16                           *piDbPasswordLength; /* Password       */
                                                 /* length                    */
   char                                *piDbPassword; /* Password             */
   db2Uint16                           *piBindNameLength; /* Bindfile name    */
                                                 /* length                    */
   char                                *piBindName; /* Bindfile name          */
   struct sqla_options                 *piOptionsArray; /* Precompiler        */
                                                 /* options array             */
   db2Uint16                           *piPidLength; /* Precompiler program   */
                                                 /* ID buffer length          */
   struct sqla_program_id              *poPrecompilerPid; /* Precompiler      */
                                                 /* program ID                */
} db2InitStruct;

/* Generic Initialization Parmeter Structure                                  */
typedef struct db2gInitStruct
{
   db2Uint16                           *piProgramNameLength; /* Program name  */
                                                 /* length                    */
   char                                *piProgramName; /* Program name        */
   db2Uint16                           *piDbNameLength; /* Database name      */
                                                 /* length                    */
   char                                *piDbName; /* Database name            */
   db2Uint16                           *piDbPasswordLength; /* Password       */
                                                 /* length                    */
   char                                *piDbPassword; /* Password             */
   db2Uint16                           *piBindNameLength; /* Bindfile name    */
                                                 /* length                    */
   char                                *piBindName; /* Bindfile name          */
   struct sqla_options                 *piOptionsArray; /* Precompiler        */
                                                 /* options array             */
   db2Uint16                           *piPidLength; /* Precompiler program   */
                                                 /* ID buffer length          */
   struct sqla_program_id              *poPrecompilerPid; /* Precompiler      */
                                                 /* program ID                */
} db2gInitStruct;

/* Initialization API                                                         */
SQL_API_RC SQL_API_FN                            /* Initialize an access      */
                                                 /* plan                      */
  db2Initialize (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/* Generic Initialization SQL API                                             */
SQL_API_RC SQL_API_FN                            /* Initialize an access      */
                                                 /* plan                      */
  db2gInitialize (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/******************************************************************************
** Reorg Definitions
*******************************************************************************/

/* Reorg type defines                                                         */
#define DB2REORG_OBJ_TABLE_OFFLINE 1      /* Define for reorg table offline   */
#define DB2REORG_OBJ_TABLE_INPLACE 2      /* Define for reorg table inplace   */
#define DB2REORG_OBJ_INDEXESALL   4       /* Define for reorg indexes all     */

/* Defines for reorg flags parameter                                          */
#define DB2REORG_OPTION_NONE      0x0     /* Take default action              */

/* Offline Reorg Options                                                      */
#define DB2REORG_LONGLOB          0x1     /* Reorg long fields and lobs       */
#define DB2REORG_CHOOSE_TEMP      0x2     /* System to choose temp            */
                                          /* tablespace for reorg work        */
#define DB2REORG_INDEXSCAN        0x4     /* Recluster utilizing index scan   */
#define DB2REORG_ALLOW_NONE       0x8     /* No read or write access to       */
                                          /* table                            */

/* Online Reorg Options                                                       */
#define DB2REORG_START_ONLINE     0x10000 /* Start online reorg               */
#define DB2REORG_PAUSE_ONLINE     0x20000 /* Pause an existing online reorg   */
#define DB2REORG_STOP_ONLINE      0x40000 /* Stop an existing online reorg    */
#define DB2REORG_RESUME_ONLINE    0x80000 /* Resume a paused online reorg     */
#define DB2REORG_NOTRUNCATE_ONLINE 0x100000 /* Do not perform table           */
                                          /* truncation                       */
#define DB2REORG_ALLOW_WRITE      0x10000000 /* Read and write access to      */
                                          /* table                            */
#define DB2REORG_ALLOW_READ       0x20000000 /* Allow only read access to     */
                                          /* table                            */

/* Online Reorg Indexes All Options                                           */
#define DB2REORG_REORG_TYPE1      0x200000 /* Reorg type1 index only          */
#define DB2REORG_CLEANUP_NONE     0x400000 /* No cleanup is required          */
#define DB2REORG_CLEANUP_ALL      0x800000 /* Cleanup pages and keys          */
#define DB2REORG_CLEANUP_PAGES    0x1000000 /* Cleanup pages only             */
#define DB2REORG_CONVERT_NONE     0x2000000 /* No convert is required         */
#define DB2REORG_CONVERT          0x4000000 /* Convert to type2               */
#define DB2REORG_CONVERT_TYPE1    0x8000000 /* Convert to type1               */

/* MPP allNodeFlag values                                                     */
#define DB2REORG_NODE_LIST        DB2_NODE_LIST
#define DB2REORG_ALL_NODES        DB2_ALL_NODES
#define DB2REORG_ALL_EXCEPT       DB2_ALL_EXCEPT

/* Reorg Table input struct                                                   */
typedef SQL_STRUCTURE db2ReorgTable
{
  char              *pTableName;      /* Name of table to reorganize          */
  char              *pOrderByIndex;   /* Index to order table by              */
  char              *pSysTempSpace;   /* Tablespace to create temp objects    */
                                      /* in                                   */
} db2ReorgTable;


/* Reorg Indexes all input struct                                             */
typedef SQL_STRUCTURE db2ReorgIndexesAll
{
  char              *pTableName;      /* Name of table to reorganize indexes  */
                                      /* on                                   */
} db2ReorgIndexesAll;


/* Reorg object struct                                                        */
union db2ReorgObject
{
  struct db2ReorgTable             tableStruct;            /* Table struct    */
  struct db2ReorgIndexesAll        indexesAllStruct;       /* Indexes struct  */
};


/* Reorg object struct                                                        */
typedef SQL_STRUCTURE db2ReorgStruct
{
  db2Uint32                   reorgType;    /* Type - Table or Indexes        */
  db2Uint32                   reorgFlags;   /* Reorg options - DB2REORG_xxxx  */
  db2int32                    nodeListFlag; /* Which nodes reorg applies to   */
  db2Uint32                   numNodes;     /* Number of nodes in nodelist    */
                                            /* array                          */
  SQL_PDB_NODE_TYPE           *pNodeList;   /* Pointer to array of node       */
                                            /* numbers                        */
  union db2ReorgObject        reorgObject;  /* Table or Index struct          */
} db2ReorgStruct;


/* Reorg Table API                                                            */
SQL_API_RC SQL_API_FN                        /* Reorganize Table              */
  db2Reorg (
             db2Uint32 versionNumber,        /* Version Number                */
             void * pReorgStruct,            /* Reorg Table Info              */
             struct sqlca * pSqlca);         /* SQLCA                         */


/* Generic Reorg Table API input struct                                       */
typedef SQL_STRUCTURE db2gReorgTable
{
  db2Uint32  tableNameLen;        /* length in bytes of pTableName            */
  char       *pTableName;         /* Name of table to reorganize              */
  db2Uint32  orderByIndexLen;     /* length in bytes of pOrderByIndex         */
  char       *pOrderByIndex;      /* Index to order table by                  */
  db2Uint32  sysTempSpaceLen;     /* length in bytes of pSysTempSpace         */
  char       *pSysTempSpace;      /* Tablespace to create temp objects in     */
} db2gReorgTable;


/* Reorg Indexes all input struct                                             */
typedef SQL_STRUCTURE db2gReorgIndexesAll
{
  db2Uint32  tableNameLen;        /* length in bytes of pTableName            */
  char       *pTableName;         /* Name of table to reorganize indexes on   */
} db2gReorgIndexesAll;


/* Reorg object struct                                                        */
union db2gReorgObject
{
  struct db2gReorgTable            tableStruct;            /* Table struct    */
  struct db2gReorgIndexesAll       indexesAllStruct;       /* Indexes struct  */
};


/* Reorg nodelist array                                                       */
typedef SQL_STRUCTURE db2gReorgNodes
{
  SQL_PDB_NODE_TYPE                nodeNum[SQL_PDB_MAX_NUM_NODE]; /* Node     */
                                                           /* number          */
} db2gReorgNodes;


/* Reorg object struct                                                        */
typedef SQL_STRUCTURE db2gReorgStruct
{
  db2Uint32                   reorgType;    /* Type - Table or Indexes        */
  db2Uint32                   reorgFlags;   /* Reorg options - DB2REORG_xxxx  */
  db2int32                    nodeListFlag; /* Which nodes reorg applies to   */
  db2Uint32                   numNodes;     /* Number of nodes in nodelist    */
                                            /* array                          */
  SQL_PDB_NODE_TYPE           *pNodeList;   /* Pointer to array of node       */
                                            /* numbers                        */
  union db2gReorgObject       reorgObject;  /* Table or Index struct          */
} db2gReorgStruct;

/* Generic Reorg Table API                                                    */
SQL_API_RC SQL_API_FN                        /* Reorganize Table              */
  db2gReorg (
              db2Uint32 versionNumber,       /* Database version number       */
              void * pReorgStruct,           /* Input parameters              */
              struct sqlca * pSqlca);        /* SQLCA                         */


/******************************************************************************
** db2Runstats Definitions
*******************************************************************************/

/* Function Prototype for db2Runstats API                                     */

SQL_API_RC SQL_API_FN                        /* Collect Statistics            */
  db2Runstats (
                db2Uint32 versionNumber,     /* database version number       */
                void * data,                 /* input parameters              */
                struct sqlca * pSqlca);      /* output SQLCA                  */

/* Options for iRunstatsFlags                                                 */
#define DB2RUNSTATS_ALL_COLUMNS 0x1         /* Gather stats on all columns    */
#define DB2RUNSTATS_KEY_COLUMNS 0x2         /* Gather stats on key columns    */
#define DB2RUNSTATS_ALL_INDEXES 0x4         /* Gather stats on all indexes    */
#define DB2RUNSTATS_DISTRIBUTION 0x8        /* Gather distribution stats on   */
                                            /* either all columns or key      */
                                            /* columns                        */
#define DB2RUNSTATS_EXT_INDEX 0x10          /* Gather extended index stats    */
#define DB2RUNSTATS_EXT_INDEX_SAMPLED 0x20  /* Gather sampled extended index  */
                                            /* stats                          */
#define DB2RUNSTATS_USE_PROFILE 0x40        /* Gather stats using profile     */
#define DB2RUNSTATS_SET_PROFILE 0x80        /* Gather stats and set the       */
                                            /* profile                        */
#define DB2RUNSTATS_SET_PROFILE_ONLY 0x100  /* Set the stats profile only     */
#define DB2RUNSTATS_ALLOW_READ 0x200        /* Allow others to only read      */
                                            /* table while Runstats is in     */
                                            /* progress                       */
#define DB2RUNSTATS_ALL_DBPARTITIONS 0x400  /* Gather stats on all DB         */
                                            /* partitions                     */
#define DB2RUNSTATS_UPDATE_PROFILE 0x800    /* Gather statistics and update   */
                                            /* statistics profile             */
#define DB2RUNSTATS_UPDA_PROFILE_ONLY 0x1000 /* Update statistics profile     */
                                            /* without gathering statistics   */
#define DB2RUNSTATS_SAMPLING_SYSTEM 0x2000  /* Page Level Sampling            */
#define DB2RUNSTATS_SAMPLING_REPEAT 0x4000  /* Generate Repeatable Sample     */

/* Options for iColumnFlags                                                   */
#define DB2RUNSTATS_COLUMN_LIKE_STATS 0x1   /* Gather stats for LIKE          */
                                            /* predicates                     */

/* Default options                                                            */
#define DB2RUNSTATS_PARALLELISM_DFLT 0      /* Default level of parallelism   */
#define DB2RUNSTATS_SAMPLING_DFLT 1.000000e+002 /* Default percentage of      */
                                            /* pages sampled (100%)           */


/*  Structures for db2Runstats API                                            */

/* Columns to Collect Stats on                                                */
typedef SQL_STRUCTURE db2ColumnData
{
  unsigned char               *piColumnName;
  db2int16                    iColumnFlags;
} db2ColumnData;

/* Columns to Collect Distribution Stats on                                   */
typedef SQL_STRUCTURE db2ColumnDistData
{
  unsigned char               *piColumnName;
  db2int16                    iNumFreqValues;
  db2int16                    iNumQuantiles;
} db2ColumnDistData;

/* Columns Groups to Collect Stats on                                         */
typedef SQL_STRUCTURE db2ColumnGrpData
{
  unsigned char               **piGroupColumnNames;
  db2int16                    iGroupSize;
  db2int16                    iNumFreqValues;
  db2int16                    iNumQuantiles;
} db2ColumnGrpData;

/* Struct containing db2Runstats options                                      */
typedef SQL_STRUCTURE db2RunstatsData
{
  double                      iSamplingOption;
  unsigned char               *piTablename;
  struct db2ColumnData        **piColumnList;
  struct db2ColumnDistData    **piColumnDistributionList;
  struct db2ColumnGrpData     **piColumnGroupList;
  unsigned char               **piIndexList;
  db2Uint32                   iRunstatsFlags;
  db2int16                    iNumColumns;
  db2int16                    iNumColdist;
  db2int16                    iNumColGroups;
  db2int16                    iNumIndexes;
  db2int16                    iParallelismOption;
  db2int16                    iTableDefaultFreqValues;
  db2int16                    iTableDefaultQuantiles;
  db2Uint32                   iSamplingRepeatable;
  db2Uint32                   iUtilImpactPriority;
} db2RunstatsData;


/* Function Prototype for db2gRunstats API                                    */

SQL_API_RC SQL_API_FN                        /* Collect Statistics            */
  db2gRunstats (
                db2Uint32 versionNumber,     /* database version number       */
                void * data,                 /* input parameters              */
                struct sqlca * pSqlca);      /* output SQLCA                  */

/*  Structures for db2gRunstats API                                           */

/* Columns to Collect Stats on                                                */
typedef SQL_STRUCTURE db2gColumnData
{
  unsigned char               *piColumnName;
  db2Uint16                   iColumnNameLen;
  db2int16                    iColumnFlags;
} db2gColumnData;

/* Columns to Collect Distribution Stats on                                   */
typedef SQL_STRUCTURE db2gColumnDistData
{
  unsigned char               *piColumnName;
  db2Uint16                   iColumnNameLen;
  db2int16                    iNumFreqValues;
  db2int16                    iNumQuantiles;
} db2gColumnDistData;

/* Columns Groups to Collect Stats on                                         */
typedef SQL_STRUCTURE db2gColumnGrpData
{
  unsigned char               **piGroupColumnNames;
  db2Uint16                   *piGroupColumnNamesLen;
  db2int16                    iGroupSize;
  db2int16                    iNumFreqValues;
  db2int16                    iNumQuantiles;
} db2gColumnGrpData;

/* Struct containing db2gRunstats options                                     */
typedef SQL_STRUCTURE db2gRunstatsData
{
  double                      iSamplingOption;
  unsigned char               *piTablename;
  struct db2gColumnData       **piColumnList;
  struct db2gColumnDistData   **piColumnDistributionList;
  struct db2gColumnGrpData    **piColumnGroupList;
  unsigned char               **piIndexList;
  db2Uint16                   *piIndexNamesLen;
  db2Uint32                   iRunstatsFlags;
  db2Uint16                   iTablenameLen;
  db2int16                    iNumColumns;
  db2int16                    iNumColdist;
  db2int16                    iNumColGroups;
  db2int16                    iNumIndexes;
  db2int16                    iParallelismOption;
  db2int16                    iTableDefaultFreqValues;
  db2int16                    iTableDefaultQuantiles;
  db2Uint32                   iSamplingRepeatable;
  db2Uint32                   iUtilImpactPriority;
} db2gRunstatsData;



/******************************************************************************
** Prune Database History Definitions
*******************************************************************************/

/* Values for db2Prune - iCallerAction                                        */
#define DB2PRUNE_ACTION_HISTORY        1         /* Remove history file       */
                                                 /* entries                   */
#define DB2PRUNE_ACTION_LOG            2         /* Remove log files from     */
                                                 /* active log path           */

/* Values for db2Prune - iOptions                                             */
#define DB2PRUNE_OPTION_NONE           0x0       /* Force removal of last     */
                                                 /* backup                    */
#define DB2PRUNE_OPTION_FORCE          0x1       /* Force removal of last     */
                                                 /* backup                    */
#define DB2PRUNE_OPTION_LSNSTRING      0x2       /* piString is an LSN        */
                                                 /* string, used by DB2PRUNE  */
                                                 /* ACTION_LOG                */
#define DB2PRUNE_OPTION_DELETE         0x4       /* Delete pruned files       */

/* Prune API input struct                                                     */
typedef SQL_STRUCTURE db2PruneStruct
{
  char                        *piString;    /* Timestamp, or lastLSN          */
  db2HistoryEID               iEID;         /* Prune history since this EID   */
  db2Uint32                   iAction;      /* What to prune                  */
  db2Uint32                   iOptions;     /* Options relevent to            */
                                            /* particular iAction             */
} db2PruneStruct;

SQL_API_RC SQL_API_FN                  /* Prune history / log                 */
  db2Prune (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* Input parameters                    */
       struct sqlca * pSqlca);         /* SQLCA                               */

/* Generic Prune API input struct                                             */
typedef SQL_STRUCTURE db2gPruneStruct
{
  db2Uint32                   iStringLen;   /* length in bytes of piString    */
  char                        *piString;    /* Timestamp, or lastLSN          */
  db2HistoryEID               iEID;         /* Prune history since this EID   */
  db2Uint32                   iAction;      /* What to prune                  */
  db2Uint32                   iOptions;     /* Options relevent to            */
                                            /* particular iAction             */
} db2gPruneStruct;

SQL_API_RC SQL_API_FN                  /* Generic Prune history / log         */
  db2gPrune (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* Input parameters                    */
       struct sqlca * pSqlca);         /* SQLCA                               */


/* Backup Options                                                             */
#define DB2BACKUP_DB          0x0
#define DB2BACKUP_TABLESPACE  0x3
#define DB2BACKUP_INCREMENTAL 0x10
#define DB2BACKUP_DELTA       0x30
#define DB2BACKUP_OFFLINE     0x0
#define DB2BACKUP_ONLINE      0x100
#define DB2BACKUP_COMPRESS    0x400         /* Compress backup image          */
#define DB2BACKUP_INCLUDE_COMPR_LIB 0x0     /* Include compression lib in     */
                                            /* backup image                   */
#define DB2BACKUP_EXCLUDE_COMPR_LIB 0x800   /* Do not include compression     */
                                            /* lib in backup image            */
#define DB2BACKUP_INCLUDE_LOGS 0x1000       /* Include log files in online    */
                                            /* backup image                   */
#define DB2BACKUP_EXCLUDE_LOGS 0x2000       /* Exclude log files from backup  */
                                            /* image                          */

/* Backup Caller Actions                                                      */
#define DB2BACKUP_BACKUP      0
#define DB2BACKUP_CONTINUE    1
#define DB2BACKUP_TERMINATE   2
#define DB2BACKUP_NOINTERRUPT 3
#define DB2BACKUP_DEVICE_TERMINATE 9
#define DB2BACKUP_PARM_CHK    10
#define DB2BACKUP_PARM_CHK_INIT_WAIT 10
#define DB2BACKUP_PARM_CHK_ONLY 11

/* Restore Options                                                            */
#define DB2RESTORE_DB         0x0
#define DB2RESTORE_TABLESPACE 0x4
#define DB2RESTORE_HISTORY    0x5
#define DB2RESTORE_COMPR_LIB  0x6           /* Restore only compression       */
                                            /* library                        */
#define DB2RESTORE_LOGS       0x7
#define DB2RESTORE_INCREMENTAL 0x10
#define DB2RESTORE_OFFLINE    0x0
#define DB2RESTORE_ONLINE     0x40
#define DB2RESTORE_DATALINK   0x0
#define DB2RESTORE_NODATALINK 0x80
#define DB2RESTORE_AUTOMATIC  0x100
#define DB2RESTORE_ROLLFWD    0x0
#define DB2RESTORE_NOROLLFWD  0x200
#define DB2RESTORE_NOREPLACE_HISTORY 0x0
#define DB2RESTORE_REPLACE_HISTORY 0x400

/* Restore Caller Actions                                                     */
#define DB2RESTORE_RESTORE    0
#define DB2RESTORE_CONTINUE   1
#define DB2RESTORE_TERMINATE  2
#define DB2RESTORE_NOINTERRUPT 3
#define DB2RESTORE_DEVICE_TERMINATE 9
#define DB2RESTORE_PARM_CHK   10
#define DB2RESTORE_PARM_CHK_INIT_WAIT 10
#define DB2RESTORE_PARM_CHK_ONLY 11
#define DB2RESTORE_TERMINATE_INCRE 13
#define DB2RESTORE_RESTORE_STORDEF 100
#define DB2RESTORE_STORDEF_NOINTERRUPT 101

/******************************************************************************
** Backup Definitions
*******************************************************************************/

/* Tablespace List Structure                                                  */
typedef SQL_STRUCTURE db2TablespaceStruct
{
  char                        **tablespaces;
  db2Uint32                   numTablespaces;
} db2TablespaceStruct;

/* Media List Structure                                                       */
typedef SQL_STRUCTURE db2MediaListStruct
{
  char                        **locations;
  db2Uint32                   numLocations;
  char                        locationType;
} db2MediaListStruct;

/* Backup Structure                                                           */
typedef SQL_STRUCTURE db2BackupStruct
{
  char                        *piDBAlias;
  char                        oApplicationId[SQLU_APPLID_LEN+1];
  char                        oTimestamp[SQLU_TIME_STAMP_LEN+1];
  struct db2TablespaceStruct  *piTablespaceList;
  struct db2MediaListStruct   *piMediaList;
  char                        *piUsername;
  char                        *piPassword;
  void                        *piVendorOptions;
  db2Uint32                   iVendorOptionsSize;
  db2Uint32                   oBackupSize;
  db2Uint32                   iCallerAction;
  db2Uint32                   iBufferSize;
  db2Uint32                   iNumBuffers;
  db2Uint32                   iParallelism;
  db2Uint32                   iOptions;
  db2Uint32                   iUtilImpactPriority; /* Throttle Parameter      */
  char                        *piComprLibrary; /* Name of compression         */
                                            /* library                        */
  void                        *piComprOptions; /* Vendor-specific options     */
                                            /* for compression plug-in        */
  db2Uint32                   iComprOptionsSize; /* Size of piComprOptions    */
                                            /* block                          */
} db2BackupStruct;

/* Backup API                                                                 */
SQL_API_RC SQL_API_FN
  db2Backup (
       db2Uint32 versionNumber,
       void * pDB2BackupStruct,
       struct sqlca * pSqlca);

/******************************************************************************
** Generic Backup Definitions
*******************************************************************************/

/* Generic Tablespace List Structure                                          */
typedef SQL_STRUCTURE db2gTablespaceStruct
{
  struct db2Char              *tablespaces;
  db2Uint32                   numTablespaces;
} db2gTablespaceStruct;

/* Generic Media List Structure                                               */
typedef SQL_STRUCTURE db2gMediaListStruct
{
  struct db2Char              *locations;
  db2Uint32                   numLocations;
  char                        locationType;
} db2gMediaListStruct;

/* Generic Backup Structure                                                   */
typedef SQL_STRUCTURE db2gBackupStruct
{
  char                        *piDBAlias;
  db2Uint32                   iDBAliasLen;
  char                        *poApplicationId;
  db2Uint32                   iApplicationIdLen;
  char                        *poTimestamp;
  db2Uint32                   iTimestampLen;
  struct db2gTablespaceStruct *piTablespaceList;
  struct db2gMediaListStruct  *piMediaList;
  char                        *piUsername;
  db2Uint32                   iUsernameLen;
  char                        *piPassword;
  db2Uint32                   iPasswordLen;
  void                        *piVendorOptions;
  db2Uint32                   iVendorOptionsSize;
  db2Uint32                   oBackupSize;
  db2Uint32                   iCallerAction;
  db2Uint32                   iBufferSize;
  db2Uint32                   iNumBuffers;
  db2Uint32                   iParallelism;
  db2Uint32                   iOptions;
  db2Uint32                   iUtilImpactPriority; /* Throttle Parameter      */
  char                        *piComprLibrary; /* Name of compression         */
                                            /* library                        */
  db2Uint32                   iComprLibraryLen; /* Length of compression      */
                                            /* library name                   */
  void                        *piComprOptions; /* Vendor-specific options     */
                                            /* for compression plug-in        */
  db2Uint32                   iComprOptionsSize; /* Size of piComprOptions    */
                                            /* block                          */
} db2gBackupStruct;

/* Generic Backup API                                                         */
SQL_API_RC SQL_API_FN
  db2gBackup (
       db2Uint32 versionNumber,
       void * pDB2gBackupStruct,
       struct sqlca * pSqlca);

/******************************************************************************
** Restore Definitions
*******************************************************************************/

/* Storage Paths Structure                                                    */
typedef SQL_STRUCTURE db2StoragePathsStruct
{
  char                        **storagePaths;
  db2Uint32                   numStoragePaths;
} db2StoragePathsStruct;

/* Restore Structure                                                          */
typedef SQL_STRUCTURE db2RestoreStruct
{
  char                        *piSourceDBAlias;
  char                        *piTargetDBAlias;
  char                        oApplicationId[SQLU_APPLID_LEN+1];
  char                        *piTimestamp;
  char                        *piTargetDBPath;
  char                        *piReportFile;
  struct db2TablespaceStruct  *piTablespaceList;
  struct db2MediaListStruct   *piMediaList;
  char                        *piUsername;
  char                        *piPassword;
  char                        *piNewLogPath;
  void                        *piVendorOptions;
  db2Uint32                   iVendorOptionsSize;
  db2Uint32                   iParallelism;
  db2Uint32                   iBufferSize;
  db2Uint32                   iNumBuffers;
  db2Uint32                   iCallerAction;
  db2Uint32                   iOptions;
  char                        *piComprLibrary; /* Name of compression         */
                                            /* library                        */
  void                        *piComprOptions; /* Vendor-specific options     */
                                            /* for compression plug-in        */
  db2Uint32                   iComprOptionsSize; /* Size of piComprOptions    */
                                            /* block                          */
  char                        *piLogTarget;
  struct db2StoragePathsStruct *piStoragePaths;
} db2RestoreStruct;

/* Restore API                                                                */
SQL_API_RC SQL_API_FN
  db2Restore (
       db2Uint32 versionNumber,
       void * pDB2RestoreStruct,
       struct sqlca * pSqlca);

/******************************************************************************
** Generic Restore Definitions
*******************************************************************************/

/* Generic Storage Paths Structure                                            */
typedef SQL_STRUCTURE db2gStoragePathsStruct
{
  struct db2Char              *storagePaths;
  db2Uint32                   numStoragePaths;
} db2gStoragePathsStruct;

/* Generic Restore Structure                                                  */
typedef SQL_STRUCTURE db2gRestoreStruct
{
  char                        *piSourceDBAlias;
  db2Uint32                   iSourceDBAliasLen;
  char                        *piTargetDBAlias;
  db2Uint32                   iTargetDBAliasLen;
  char                        *poApplicationId;
  db2Uint32                   iApplicationIdLen;
  char                        *piTimestamp;
  db2Uint32                   iTimestampLen;
  char                        *piTargetDBPath;
  db2Uint32                   iTargetDBPathLen;
  char                        *piReportFile;
  db2Uint32                   iReportFileLen;
  struct db2gTablespaceStruct *piTablespaceList;
  struct db2gMediaListStruct  *piMediaList;
  char                        *piUsername;
  db2Uint32                   iUsernameLen;
  char                        *piPassword;
  db2Uint32                   iPasswordLen;
  char                        *piNewLogPath;
  db2Uint32                   iNewLogPathLen;
  void                        *piVendorOptions;
  db2Uint32                   iVendorOptionsSize;
  db2Uint32                   iParallelism;
  db2Uint32                   iBufferSize;
  db2Uint32                   iNumBuffers;
  db2Uint32                   iCallerAction;
  db2Uint32                   iOptions;
  char                        *piComprLibrary; /* Name of compression         */
                                            /* library                        */
  db2Uint32                   iComprLibraryLen; /* Length of compression      */
                                            /* library name                   */
  void                        *piComprOptions; /* Vendor-specific options     */
                                            /* for compression plug-in        */
  db2Uint32                   iComprOptionsSize; /* Size of piComprOptions    */
                                            /* block                          */
  char                        *piLogTarget;
  db2Uint32                   iLogTargetLen;
  struct db2gStoragePathsStruct *piStoragePaths;
} db2gRestoreStruct;

/* Generic Restore API                                                        */
SQL_API_RC SQL_API_FN
  db2gRestore (
       db2Uint32 versionNumber,
       void * pDB2gRestoreStruct,
       struct sqlca * pSqlca);

/******************************************************************************
** Recover Definitions
*******************************************************************************/

/* Recover Structure                                                          */
typedef SQL_STRUCTURE db2RecoverStruct
{
  char                        *piSourceDBAlias;
  char                        *piUsername;
  char                        *piPassword;
  db2Uint32                   iRecoverCallerAction;
  db2Uint32                   iOptions;
  sqlint32                    *poNumReplies; /* number of replies received    */
  struct sqlurf_info          *poNodeInfo;  /* node reply info                */
  char                        *piStopTime;  /* Stop time                      */
  char                        *piOverflowLogPath; /* Overflow log path        */
  db2Uint32                   iNumChngLgOvrflw; /* Number of changed          */
                                            /* overflow log paths             */
  struct sqlurf_newlogpath    *piChngLogOvrflw; /* Changed overflow log       */
                                            /* paths                          */
  db2int32                    iAllNodeFlag; /* Flag indicating how            */
                                            /* piNodeList should be used      */
  db2int32                    iNumNodes;    /* Number of nodes in piNodeList  */
  SQL_PDB_NODE_TYPE           *piNodeList;  /* Node list                      */
  db2int32                    iNumNodeInfo; /* Size of piNodeInfo in          */
                                            /* db2RfwdOutput                  */
  char                        *piHistoryFile; /* History file                 */
  db2Uint32                   iNumChngHistoryFile; /* Number of history       */
                                            /* files in list                  */
  struct sqlu_histFile        *piChngHistoryFile; /* list of history files    */
  char                        *piComprLibrary; /* Name of compression         */
                                            /* library                        */
  void                        *piComprOptions; /* Vendor-specific options     */
                                            /* for compression plug-in        */
  db2Uint32                   iComprOptionsSize; /* Size of piComprOptions    */
                                            /* block                          */
} db2RecoverStruct;

typedef SQL_STRUCTURE db2gRecoverStruct
{
  char                        *piSourceDBAlias;
  db2Uint32                   iSourceDBAliasLen; /* Length in bytes of        */
                                            /* piSourceDBAlias                */
  char                        *piUserName;
  db2Uint32                   iUserNameLen; /* Length in bytes of piUsername  */
  char                        *piPassword;
  db2Uint32                   iPasswordLen; /* Length in bytes of piPassword  */
  db2Uint32                   iRecoverCallerAction;
  db2Uint32                   iOptions;
  sqlint32                    *poNumReplies; /* number of replies received    */
  struct sqlurf_info          *poNodeInfo;  /* node reply info                */
  char                        *piStopTime;  /* Stop time                      */
  db2Uint32                   iStopTimeLen; /* Length in bytes of piStopTime  */
  char                        *piOverflowLogPath; /* Overflow log path        */
  db2Uint32                   iOverflowLogPathLen; /* Length in bytes of      */
                                            /* piOverflowLogPath              */
  db2Uint32                   iNumChngLgOvrflw; /* Number of changed          */
                                            /* overflow log paths             */
  struct sqlurf_newlogpath    *piChngLogOvrflw; /* Changed overflow log       */
                                            /* paths                          */
  db2int32                    iAllNodeFlag; /* Flag indicating how            */
                                            /* piNodeList should be used      */
  db2int32                    iNumNodes;    /* Number of nodes in piNodeList  */
  SQL_PDB_NODE_TYPE           *piNodeList;  /* Node list                      */
  db2int32                    iNumNodeInfo; /* Size of piNodeInfo in          */
                                            /* db2RfwdOutput                  */
  char                        *piHistoryFile; /* History file                 */
  db2Uint32                   iHistoryFileLen; /* Length in bytes of          */
                                            /* piHistoryFile                  */
  db2Uint32                   iNumChngHistoryFile; /* Number of history       */
                                            /* files in list                  */
  struct sqlu_histFile        *piChngHistoryFile; /* list of history files    */
  char                        *piComprLibrary; /* Name of compression         */
                                            /* library                        */
  db2Uint32                   iComprLibraryLen; /* Length of compression      */
                                            /* library name                   */
  void                        *piComprOptions; /* Vendor-specific options     */
                                            /* for compression plug-in        */
  db2Uint32                   iComprOptionsSize; /* Size of piComprOptions    */
                                            /* block                          */
} db2gRecoverStruct;

/* Recover API                                                                */
SQL_API_RC SQL_API_FN
  db2Recover (
       db2Uint32 versionNumber,
       void * pDB2RecovStruct,
       struct sqlca * pSqlca);

SQL_API_RC SQL_API_FN                  /* Generic Recover                     */
  db2gRecover (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pDB2gRecoverStruct,      /* Input parameters                    */
       struct sqlca * pSqlca);         /* SQLCA                               */

/******************************************************************************
** Rollforward Definitions
*******************************************************************************/

/* Values for db2Rollforward - CallerAction                                   */
#define DB2ROLLFORWARD_RFWD            1         /* Rollforward to requested  */
                                                 /* point                     */
#define DB2ROLLFORWARD_STOP            2         /* End rollforward recovery  */
#define DB2ROLLFORWARD_COMPLETE        2         /* End rollforward recovery  */
#define DB2ROLLFORWARD_RFWD_STOP       3         /* Rollforward  to           */
                                                 /* requested point and end   */
                                                 /* rollforward recovery      */
#define DB2ROLLFORWARD_RFWD_COMPLETE   3         /* Rollforward  to           */
                                                 /* requested point and end   */
                                                 /* rollforward recovery      */
#define DB2ROLLFORWARD_QUERY           4         /* Query nextarclog,         */
                                                 /* firstarcdel, lastarcdel,  */
                                                 /* lastcommit                */
#define DB2ROLLFORWARD_CANCEL          8         /* Cancel current            */
                                                 /* rollforward               */
#define DB2ROLLFORWARD_PARM_CHECK      10        /* Parameter check           */
#define DB2ROLLFORWARD_LOADREC_CONT    11        /* Continue load recovery    */
#define DB2ROLLFORWARD_LOADREC_TERM    12        /* Terminate load recovery   */
#define DB2ROLLFORWARD_DEVICE_TERM     13        /* Terminate device          */

/* Values for db2Rollforward - ConnectMode                                    */
#define DB2ROLLFORWARD_OFFLINE         0         /* Offline rollforward mode  */
#define DB2ROLLFORWARD_ONLINE          1         /* Online rollforward mode   */

/* Values for db2Rollforward - RollforwardFlags                               */
#define DB2ROLLFORWARD_EMPTY_FLAG      0         /* No flags specified        */
#define DB2ROLLFORWARD_LOCAL_TIME      1         /* Interpret stop time as    */
                                                 /* local time, not GMT       */
#define DB2ROLLFORWARD_NO_RETRIEVE     2         /* Do not attempt to         */
                                                 /* retrieve log files via    */
                                                 /* userexit                  */

/* Length of arrays used by db2Rollforward                                    */
#define DB2_ISO_TIMESTAMP_LEN          26        /* Length of ISO format      */
                                                 /* timestamp                 */
#define DB2_LOGFILE_NAME_LEN           12        /* Length of logfile name    */
#define DB2_LOGPATH_LEN                242       /* Maximum length of a       */
                                                 /* logfile path              */

/* Rollforward Input Structure                                                */
typedef SQL_STRUCTURE db2RfwdInputStruct
{
  sqluint32                   iVersion;     /* Rollforward version            */
  char                        *piDbAlias;   /* Database alias                 */
  db2Uint32                   iCallerAction; /* Rollforward action            */
  char                        *piStopTime;  /* Stop time                      */
  char                        *piUserName;  /* User name to execute under     */
  char                        *piPassword;  /* Password                       */
  char                        *piOverflowLogPath; /* Overflow log path        */
  db2Uint32                   iNumChngLgOvrflw; /* Number of changed          */
                                            /* overflow log paths             */
  struct sqlurf_newlogpath    *piChngLogOvrflw; /* Changed overflow log       */
                                            /* paths                          */
  db2Uint32                   iConnectMode; /* Connect mode                   */
  struct sqlu_tablespace_bkrst_list *piTablespaceList; /* List of             */
                                            /* tablespaces to rollforward     */
  db2int32                    iAllNodeFlag; /* Flag indicating how            */
                                            /* piNodeList should be used      */
  db2int32                    iNumNodes;    /* Number of nodes in piNodeList  */
  SQL_PDB_NODE_TYPE           *piNodeList;  /* Node list                      */
  db2int32                    iNumNodeInfo; /* Size of piNodeInfo in          */
                                            /* db2RfwdOutput                  */
  char                        *piDroppedTblID; /* Dropped table id            */
  char                        *piExportDir; /* Dropped table export           */
                                            /* directory                      */
  db2Uint32                   iRollforwardFlags; /* Rollforward flags         */
} db2RfwdInputStruct;

/* Generic Rollforward Input Structure                                        */
typedef SQL_STRUCTURE db2gRfwdInputStruct
{
  db2Uint32                   iDbAliasLen;  /* Length in bytes of piDbAlias   */
  db2Uint32                   iStopTimeLen; /* Length in bytes of piStopTime  */
  db2Uint32                   iUserNameLen; /* Length in bytes of piUserName  */
  db2Uint32                   iPasswordLen; /* Length in bytes of piPassword  */
  db2Uint32                   iOvrflwLogPathLen; /* Length in bytes of        */
                                            /* piOverflowLogPath              */
  db2Uint32                   iDroppedTblIDLen; /* Length in bytes of         */
                                            /* piDroppedTblID                 */
  db2Uint32                   iExportDirLen; /* Length in bytes of            */
                                            /* piExportDir                    */
  sqluint32                   iVersion;     /* Rollforward version            */
  char                        *piDbAlias;   /* Database alias                 */
  db2Uint32                   iCallerAction; /* Rollforward action            */
  char                        *piStopTime;  /* Stop time                      */
  char                        *piUserName;  /* User name to execute under     */
  char                        *piPassword;  /* Password                       */
  char                        *piOverflowLogPath; /* Overflow log path        */
  db2Uint32                   iNumChngLgOvrflw; /* Number of changed          */
                                            /* overflow log paths             */
  struct sqlurf_newlogpath    *piChngLogOvrflw; /* Changed overflow log       */
                                            /* paths                          */
  db2Uint32                   iConnectMode; /* Connect mode                   */
  struct sqlu_tablespace_bkrst_list *piTablespaceList; /* List of             */
                                            /* tablespaces to rollforward     */
  db2int32                    iAllNodeFlag; /* Flag indicating how piNodeist  */
                                            /* should be used                 */
  db2int32                    iNumNodes;    /* Number of nodes in piNodeList  */
  SQL_PDB_NODE_TYPE           *piNodeList;  /* Node list                      */
  db2int32                    iNumNodeInfo; /* Size of piNodeInfo in          */
                                            /* db2RfwdOutput                  */
  char                        *piDroppedTblID; /* Dropped table id            */
  char                        *piExportDir; /* Dropped table export           */
                                            /* directory                      */
  db2Uint32                   iRollforwardFlags; /* Rollforward flags         */
} db2gRfwdInputStruct;

/* Rollforward Output Structure                                               */
typedef SQL_STRUCTURE db2RfwdOutputStruct
{
  char                        *poApplicationId; /* application id             */
  sqlint32                    *poNumReplies; /* number of replies received    */
  struct sqlurf_info          *poNodeInfo;  /* node reply info                */
} db2RfwdOutputStruct;

/* Rollforward API Input Structure                                            */
typedef SQL_STRUCTURE db2RollforwardStruct
{
  struct db2RfwdInputStruct   *piRfwdInput; /* rollforward input structure    */
  struct db2RfwdOutputStruct  *poRfwdOutput; /* rollforward output structure  */
} db2RollforwardStruct;

/* Generic Rollforward API Structure                                          */
typedef SQL_STRUCTURE db2gRollforwardStruct
{
  struct db2gRfwdInputStruct  *piRfwdInput; /* generic rollforward input      */
                                            /* structure                      */
  struct db2RfwdOutputStruct  *poRfwdOutput; /* rollforward output structure  */
} db2gRollforwardStruct;

SQL_API_RC SQL_API_FN                  /* Rollforward                         */
  db2Rollforward (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pDB2RollforwardStruct,   /* Input parameters                    */
       struct sqlca * pSqlca);         /* SQLCA                               */

SQL_API_RC SQL_API_FN                  /* Generic Rollforward                 */
  db2gRollforward (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pDB2gRollforwardStruct,  /* Input parameters                    */
       struct sqlca * pSqlca);         /* SQLCA                               */

/******************************************************************************
** High Availability Disaster Recovery (HADR) Definitions
*******************************************************************************/

/* Values for iByForce                                                        */
#define DB2HADR_NO_FORCE               0         /* Do not perform START or   */
                                                 /* TAKEOVER HADR operation   */
                                                 /* by force                  */
#define DB2HADR_FORCE                  1         /* Do perform START or       */
                                                 /* TAKEOVER HADR operation   */
                                                 /* by force                  */

/* Values for iDbRole                                                         */
#define DB2HADR_DB_ROLE_PRIMARY        1         /* An HADR Primary database  */
#define DB2HADR_DB_ROLE_STANDBY        2         /* An HADR Standby database  */

/******************************************************************************
** High Availability Disaster Recovery (HADR) API Definitions
*******************************************************************************/

/* Start HADR API Input Structure                                             */
typedef SQL_STRUCTURE db2HADRStartStruct
{
   char                                *piDbAlias; /* Database alias          */
   char                                *piUserName; /* User name to execute   */
                                                 /* command under             */
   char                                *piPassword; /* Password for username  */
   db2Uint32                           iDbRole;  /* Primary db or standby     */
   db2Uint16                           iByForce; /* Start HADR by force       */
} db2HADRStartStruct;

SQL_API_RC SQL_API_FN                  /* Start HADR                          */
  db2HADRStart (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* Input parameters                    */
       struct sqlca * pSqlca);         /* SQLCA                               */

/* Generic Start HADR API Input Struct                                        */
typedef SQL_STRUCTURE db2gHADRStartStruct
{
   char                                *piDbAlias; /* Database alias          */
   db2Uint32                           iAliasLen; /* Length of database       */
                                                 /* alias string              */
   char                                *piUserName; /* User name to execute   */
                                                 /* command under             */
   db2Uint32                           iUserNameLen; /* Length of username    */
                                                 /* field                     */
   char                                *piPassword; /* Password for username  */
   db2Uint32                           iPasswordLen; /* Length of password    */
                                                 /* field                     */
   db2Uint32                           iDbRole;  /* Primary db or standby     */
   db2Uint16                           iByForce; /* Start HADR by force       */
} db2gHADRStartStruct;

SQL_API_RC SQL_API_FN                  /* Generic Start HADR                  */
  db2gHADRStart (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* Input parameters                    */
       struct sqlca * pSqlca);         /* SQLCA                               */


/* Stop HADR API Input Structure                                              */
typedef SQL_STRUCTURE db2HADRStopStruct
{
   char                                *piDbAlias; /* Database alias          */
   char                                *piUserName; /* User name to execute   */
                                                 /* command under             */
   char                                *piPassword; /* Password for username  */
} db2HADRStopStruct;

SQL_API_RC SQL_API_FN                  /* Stop HADR                           */
  db2HADRStop (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* Input parameters                    */
       struct sqlca * pSqlca);         /* SQLCA                               */

/* Generic Stop HADR API Input Struct                                         */
typedef SQL_STRUCTURE db2gHADRStopStruct
{
   char                                *piDbAlias; /* Database alias          */
   db2Uint32                           iAliasLen; /* Length of database       */
                                                 /* alias string              */
   char                                *piUserName; /* User name to execute   */
                                                 /* command under             */
   db2Uint32                           iUserNameLen; /* Length of username    */
                                                 /* field                     */
   char                                *piPassword; /* Password for username  */
   db2Uint32                           iPasswordLen; /* Length of password    */
                                                 /* field                     */
} db2gHADRStopStruct;

SQL_API_RC SQL_API_FN                  /* Generic Stop HADR                   */
  db2gHADRStop (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* Input parameters                    */
       struct sqlca * pSqlca);         /* SQLCA                               */


/* Takeover HADR API Input Structure                                          */
typedef SQL_STRUCTURE db2HADRTakeoverStruct
{
   char                                *piDbAlias; /* Database alias          */
   char                                *piUserName; /* User name to execute   */
                                                 /* command under             */
   char                                *piPassword; /* Password for username  */
   db2Uint16                           iByForce; /* Takeover HADR by force    */
} db2HADRTakeoverStruct;

SQL_API_RC SQL_API_FN                  /* Takeover HADR                       */
  db2HADRTakeover (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* Input parameters                    */
       struct sqlca * pSqlca);         /* SQLCA                               */

/* Generic Takeover HADR API Input Struct                                     */
typedef SQL_STRUCTURE db2gHADRTakeoverStruct
{
   char                                *piDbAlias; /* Database alias          */
   db2Uint32                           iAliasLen; /* Length of database       */
                                                 /* alias string              */
   char                                *piUserName; /* User name to execute   */
                                                 /* command under             */
   db2Uint32                           iUserNameLen; /* Length of username    */
                                                 /* field                     */
   char                                *piPassword; /* Password for username  */
   db2Uint32                           iPasswordLen; /* Length of password    */
                                                 /* field                     */
   db2Uint16                           iByForce; /* Takeover HADR by force    */
} db2gHADRTakeoverStruct;

SQL_API_RC SQL_API_FN                  /* Generic Takeover HADR               */
  db2gHADRTakeover (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* Input parameters                    */
       struct sqlca * pSqlca);         /* SQLCA                               */

/******************************************************************************
** Archive Active Log Definitions
*******************************************************************************/

/* Values for db2ArchiveLog - iAllNodeFlag                                    */
#define DB2ARCHIVELOG_NODE_LIST        DB2_NODE_LIST
#define DB2ARCHIVELOG_ALL_NODES        DB2_ALL_NODES
#define DB2ARCHIVELOG_ALL_EXCEPT       DB2_ALL_EXCEPT

/* Archive Active Log API Input Structure                                     */
typedef SQL_STRUCTURE db2ArchiveLogStruct
{
   char                                *piDatabaseAlias; /* Alias of          */
                                                 /* database to archive log   */
                                                 /* for                       */
   char                                *piUserName; /* User name to execute   */
                                                 /* command under             */
   char                                *piPassword; /* Password for username  */
   db2Uint16                           iAllNodeFlag; /* Flag indicating how   */
                                                 /* the iNodeList should be   */
                                                 /* used                      */
   db2Uint16                           iNumNodes; /* Number of nodes in       */
                                                 /* piNodeList array          */
   SQL_PDB_NODE_TYPE                   *piNodeList; /* Node list              */
   db2Uint32                           iOptions; /* Future use                */
} db2ArchiveLogStruct;

SQL_API_RC SQL_API_FN                  /* Archive Active Log                  */
  db2ArchiveLog (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pDB2ArchiveLogStruct,    /* Input parameters                    */
       struct sqlca * pSqlca);         /* SQLCA                               */

/* Generic Archive Active Log API Input Struct                                */
typedef SQL_STRUCTURE db2gArchiveLogStruct
{
   db2Uint32                           iAliasLen; /* Length of database       */
                                                 /* alias field               */
   db2Uint32                           iUserNameLen; /* Length of username    */
                                                 /* field                     */
   db2Uint32                           iPasswordLen; /* Length of password    */
                                                 /* field                     */
   char                                *piDatabaseAlias; /* Alias of          */
                                                 /* database to archive log   */
                                                 /* for                       */
   char                                *piUserName; /* User name to execute   */
                                                 /* command under             */
   char                                *piPassword; /* Password for username  */
   db2Uint16                           iAllNodeFlag; /* Flag indicating how   */
                                                 /* the iNodeList should be   */
                                                 /* used                      */
   db2Uint16                           iNumNodes; /* Number of nodes in       */
                                                 /* piNodeList array          */
   SQL_PDB_NODE_TYPE                   *piNodeList; /* Node list              */
   db2Uint32                           iOptions; /* Future use                */
} db2gArchiveLogStruct;

SQL_API_RC SQL_API_FN                  /* Generic Archive Active Log          */
  db2gArchiveLog (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pDB2ArchiveLogStruct,    /* Input parameters                    */
       struct sqlca * pSqlca);         /* SQLCA                               */

/******************************************************************************
** Asynchronous Read Log Definitions
*******************************************************************************/

/* Values for db2ReadLog - iCallerAction                                      */
#define DB2READLOG_READ                1         /* Read the database log     */
#define DB2READLOG_READ_SINGLE         2         /* Read a single log record  */
#define DB2READLOG_QUERY               3         /* Query the database log    */

/* Values for db2ReadLog - iFilterOption                                      */
#define DB2READLOG_FILTER_OFF          0         /* Read all log records      */
#define DB2READLOG_FILTER_ON           1         /* Read only propagatable    */
                                                 /* records                   */

/* Asynchronous Read Log API Info Output Structure                            */
typedef SQL_STRUCTURE db2TimeOfLog
{
   db2Uint32                           seconds;  /* Timestamp in seconds      */
   db2Uint32                           accuracy; /* High accuracy timestamp   */
} db2TimeOfLog;

typedef SQL_STRUCTURE db2ReadLogInfoStruct
{
   SQLU_LSN                            initialLSN; /* Initial LSN at first    */
                                                 /* connect                   */
   SQLU_LSN                            firstReadLSN; /* First LSN read on     */
                                                 /* call                      */
   SQLU_LSN                            nextStartLSN; /* Next LSN caller       */
                                                 /* should read               */
   db2Uint32                           logRecsWritten; /* Number of log       */
                                                 /* records returned          */
   db2Uint32                           logBytesWritten; /* Number of log      */
                                                 /* bytes returned            */
   SQLU_LSN                            firstReusedLSN; /* First LSN reused    */
                                                 /* due to restore            */
                                                 /* rollforward               */
   db2Uint32                           timeOfLSNReuse; /* Timestamp for when  */
                                                 /* LSN reuse occurred        */
   db2TimeOfLog                        currentTimeValue; /* Current time      */
} db2ReadLogInfoStruct;

/* Asynchronous Read Log API Input Structure                                  */
typedef SQL_STRUCTURE db2ReadLogStruct
{
   db2Uint32                           iCallerAction; /* Type of read         */
                                                 /* required                  */
   SQLU_LSN                            *piStartLSN; /* Start read log         */
                                                 /* sequence number           */
   SQLU_LSN                            *piEndLSN; /* End read log sequence    */
                                                 /* number                    */
   char                                *poLogBuffer; /* User buffer           */
   db2Uint32                           iLogBufferSize; /* Size of user        */
                                                 /* buffer                    */
   db2Uint32                           iFilterOption; /* Set filtering level  */
   db2ReadLogInfoStruct                *poReadLogInfo; /* Read log            */
                                                 /* information structure     */
} db2ReadLogStruct;

SQL_API_RC SQL_API_FN                  /* Asynchronous Read Log               */
  db2ReadLog (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pDB2ReadLogStruct,       /* Input parameters                    */
       struct sqlca * pSqlca);         /* SQLCA                               */


/******************************************************************************
** Standalone Read Log API Definitions
*******************************************************************************/

/* Values for db2ReadLogNoConn - iRetrieveLogs                                */
#define DB2READLOG_RETRIEVE_OFF        0         /* Do not retrieve missing   */
                                                 /* logs                      */
#define DB2READLOG_RETRIEVE_LOGPATH    1         /* Retrieve logs to logpath  */
#define DB2READLOG_RETRIEVE_OVERFLOW   2         /* Retrieve logs to          */
                                                 /* overflow logpath          */

/* Standalone Read Log API Initialization Input Structure                     */
typedef SQL_STRUCTURE db2ReadLogNoConnInitStruct
{
   db2Uint32                           iFilterOption; /* Set filtering level  */
   char                                *piLogFilePath; /* Path to log files   */
                                                 /* to be read                */
   char                                *piOverflowLogPath; /* Secondary path  */
                                                 /* to log files              */
   db2Uint32                           iRetrieveLogs; /* Retrieve logs        */
                                                 /* options                   */
   char                                *piDatabaseName; /* Name of the        */
                                                 /* source database           */
   char                                *piNodeName; /* Name of the source     */
                                                 /* node                      */
   db2Uint32                           iReadLogMemoryLimit; /* Number of      */
                                                 /* bytes of memory the API   */
                                                 /* may allocate internally   */
   char                                **poReadLogMemPtr; /* Pointer to API   */
                                                 /* allocate memory           */
} db2ReadLogNoConnInitStruct;

SQL_API_RC SQL_API_FN                  /* Standalone Read Log Initialization  */
                                       /* API                                 */
  db2ReadLogNoConnInit (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pDB2ReadLogNoConnInitStruct, /* Input parameters                */
       struct sqlca * pSqlca);         /* SQLCA                               */

/* Standalone Read Log API Info Output Structure                              */
typedef SQL_STRUCTURE db2ReadLogNoConnInfoStruct
{
   SQLU_LSN                            firstAvailableLSN; /* First available  */
                                                 /* LSN in logpath            */
   SQLU_LSN                            firstReadLSN; /* First LSN read on     */
                                                 /* call                      */
   SQLU_LSN                            nextStartLSN; /* Next LSN caller       */
                                                 /* should read               */
   db2Uint32                           logRecsWritten; /* Number of log       */
                                                 /* records returned          */
   db2Uint32                           logBytesWritten; /* Number of log      */
                                                 /* bytes returned            */
   db2Uint32                           lastLogFullyRead; /* Last log read to  */
                                                 /* completion on call        */
   db2TimeOfLog                        currentTimeValue; /* Current time      */
} db2ReadLogNoConnInfoStruct;

/* Standalone Read Log API Input Structure                                    */
typedef SQL_STRUCTURE db2ReadLogNoConnStruct
{
   db2Uint32                           iCallerAction; /* Type of read         */
                                                 /* required                  */
   SQLU_LSN                            *piStartLSN; /* Start read log         */
                                                 /* sequence number           */
   SQLU_LSN                            *piEndLSN; /* End read log sequence    */
                                                 /* number                    */
   char                                *poLogBuffer; /* User buffer           */
   db2Uint32                           iLogBufferSize; /* Size of user        */
                                                 /* buffer                    */
   char                                *piReadLogMemPtr; /* Pointer to API    */
                                                 /* allocated memory          */
   db2ReadLogNoConnInfoStruct          *poReadLogInfo; /* Read log            */
                                                 /* information structure     */
} db2ReadLogNoConnStruct;

SQL_API_RC SQL_API_FN                  /* Standalone Read Log API             */
  db2ReadLogNoConn (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pDB2ReadLogNoConnStruct, /* Input parameters                    */
       struct sqlca * pSqlca);         /* SQLCA                               */

/* Standalone Read Log API Termination Input Structure                        */
typedef SQL_STRUCTURE db2ReadLogNoConnTermStruct
{
   char                                **poReadLogMemPtr; /* Pointer to API   */
                                                 /* allocated memory          */
} db2ReadLogNoConnTermStruct;

SQL_API_RC SQL_API_FN                  /* Standalone Read Log Termination     */
                                       /* API                                 */
  db2ReadLogNoConnTerm (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pDB2ReadLogNoConnTermStruct, /* Input parameters                */
       struct sqlca * pSqlca);         /* SQLCA                               */


/* Ping Database Parameter Structure                                          */
typedef SQL_STRUCTURE db2DatabasePingStruct
{
   char                                iDbAlias[SQL_ALIAS_SZ + 1]; /* Input:  */
                                                 /* Database alias -          */
                                                 /* reserved                  */
   db2int32                            RequestPacketSz; /* Input: size of     */
                                                 /* request packet            */
   db2int32                            ResponsePacketSz; /* Input: size of    */
                                                 /* response packet           */
   db2Uint16                           iNumIterations; /* Input: Number of    */
                                                 /* iterations                */
   db2Uint32                           *poElapsedTime; /* Output: Array of    */
                                                 /* elapsed times             */
                                                 /* (microsec), contains      */
                                                 /* iNumIterations elements   */
} db2DatabasePingStruct;

SQL_API_RC SQL_API_FN                  /* Ping Database                       */
  db2DatabasePing (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* In/out parameters                   */
       struct sqlca * pSqlca);         /* SQLCA                               */

/******************************************************************************
** Generic Ping Database Parameter Structure 
*******************************************************************************/
typedef SQL_STRUCTURE db2gDatabasePingStruct
{
   db2Uint16                           iDbAliasLength; /* Input: Db alias     */
                                                 /* name length - reserved    */
   char                                iDbAlias[SQL_ALIAS_SZ + 1]; /* Input:  */
                                                 /* Database alias -          */
                                                 /* reserved                  */
   db2int32                            RequestPacketSz; /* Input: size of     */
                                                 /* request packet            */
   db2int32                            ResponsePacketSz; /* Input: size of    */
                                                 /* response packet           */
   db2Uint16                           iNumIterations; /* Input: Number of    */
                                                 /* iterations                */
   db2Uint32                           *poElapsedTime; /* Output: Array of    */
                                                 /* elapsed times             */
                                                 /* (microsec), contains      */
                                                 /* iNumIterations elements   */
} db2gDatabasePingStruct;

SQL_API_RC SQL_API_FN                  /* Generic Ping Database               */
  db2gDatabasePing (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* In/out parameters                   */
       struct sqlca * pSqlca);         /* SQLCA                               */

/* Convert LSN from internal representation to ascii hex string.              */
/*      _inLsn  - char * that points to the LSN in db2 internal b-byte        */
/*                binary format.                                              */
/*      _outStr - char * to store the output in hex string format.  It        */
/*                should be big enough to store at least 13 bytes.            */

#define db2LsnToString(_inLsn, _outStr) \
         sprintf(_outStr, \
                 "%2.2X%2.2X%2.2X%2.2X%2.2X%2.2X", \
                 (_inLsn).lsnChar[0], (_inLsn).lsnChar[1], \
                 (_inLsn).lsnChar[2], (_inLsn).lsnChar[3], \
                 (_inLsn).lsnChar[4], (_inLsn).lsnChar[5])

/* Convert Monitor Data Interface Structure                                   */
typedef SQL_STRUCTURE db2ConvMonStreamData
{
   void                                *poTarget; /* Pointer to target        */
                                                 /* structure                 */
   struct sqlm_header_info             *piSource; /* Pointer to V6 stream     */
   db2Uint32                           iTargetType; /* Target structure type  */
   db2Uint32                           iTargetSize; /* Space allocated for    */
                                                 /* target                    */
   db2Uint32                           iSourceType; /* Type of source stream  */
} db2ConvMonStreamData;

SQL_API_RC SQL_API_FN                            /* Convert monitor stream    */
  db2ConvMonStream (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

SQL_API_RC SQL_API_FN                            /* Convert monitor stream    */
  db2gConvMonStream (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/* Get Snapshot Data Interface Structure                                      */
typedef SQL_STRUCTURE db2GetSnapshotData
{
   struct sqlma                        *piSqlmaData; /* Pointer to monitor    */
                                                 /* area                      */
   struct sqlm_collected               *poCollectedData; /* Pointer to        */
                                                 /* collected data            */
   void                                *poBuffer; /* Pointer to output        */
                                                 /* buffer                    */
   db2Uint32                           iVersion; /* Snapshot version          */
   db2Uint32                           iBufferSize; /* Size of output buffer  */
   db2Uint32                           iStoreResult; /* Write to file flag    */
   db2int32                            iNodeNumber; /* Target node            */
   db2Uint32                           *poOutputFormat; /* Pointer to output  */
                                                 /* format indicator          */
   db2Uint32                           iSnapshotClass; /* Class qualifier     */
                                                 /* for snapshot              */
} db2GetSnapshotData;

typedef SQL_STRUCTURE db2gGetSnapshotData
{
   struct sqlma                        *piSqlmaData; /* Pointer to monitor    */
                                                 /* area                      */
   struct sqlm_collected               *poCollectedData; /* Pointer to        */
                                                 /* collected data            */
   void                                *poBuffer; /* Pointer to output        */
                                                 /* buffer                    */
   db2Uint32                           iVersion; /* Snapshot version          */
   db2Uint32                           iBufferSize; /* Size of output buffer  */
   db2Uint32                           iStoreResult; /* Write to file flag    */
   db2int32                            iNodeNumber; /* Target node            */
   db2Uint32                           *poOutputFormat; /* Pointer to output  */
                                                 /* format indicator          */
   db2Uint32                           iSnapshotClass; /* Class qualifier     */
                                                 /* for snapshot              */
} db2gGetSnapshotData;

SQL_API_RC SQL_API_FN                            /* Get snapshot              */
  db2GetSnapshot (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

SQL_API_RC SQL_API_FN                            /* Get snapshot              */
  db2gGetSnapshot (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/* Get Snapshot Size Data Interface Structure                                 */
typedef SQL_STRUCTURE db2GetSnapshotSizeData
{
   struct sqlma                        *piSqlmaData; /* Pointer to monitor    */
                                                 /* area                      */
   sqluint32                           *poBufferSize; /* Pointer to output    */
                                                 /* buffer                    */
   db2Uint32                           iVersion; /* Snapshot version          */
   db2int32                            iNodeNumber; /* Target node            */
   db2Uint32                           iSnapshotClass; /* Class qualifier     */
                                                 /* for snapshot              */
} db2GetSnapshotSizeData;

typedef SQL_STRUCTURE db2gGetSnapshotSizeData
{
   struct sqlma                        *piSqlmaData; /* Pointer to monitor    */
                                                 /* area                      */
   sqluint32                           *poBufferSize; /* Pointer to output    */
                                                 /* buffer                    */
   db2Uint32                           iVersion; /* Snapshot version          */
   db2int32                            iNodeNumber; /* Target node            */
   db2Uint32                           iSnapshotClass; /* Class qualifier     */
                                                 /* for snapshot              */
} db2gGetSnapshotSizeData;

SQL_API_RC SQL_API_FN                            /* Get snapshot size         */
  db2GetSnapshotSize (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

SQL_API_RC SQL_API_FN                            /* Get snapshot size         */
  db2gGetSnapshotSize (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/* Monitor Switches Data Interface Structure                                  */
typedef SQL_STRUCTURE db2MonitorSwitchesData
{
   struct sqlm_recording_group         *piGroupStates; /* Pointer to group    */
                                                 /* states                    */
   void                                *poBuffer; /* Pointer to output        */
                                                 /* buffer                    */
   db2Uint32                           iBufferSize; /* Size of output buffer  */
   db2Uint32                           iReturnData; /* Return data flag       */
   db2Uint32                           iVersion; /* Snapshot version          */
   db2int32                            iNodeNumber; /* Target node            */
   db2Uint32                           *poOutputFormat; /* Pointer to output  */
                                                 /* format indicator          */
} db2MonitorSwitchesData;

typedef SQL_STRUCTURE db2gMonitorSwitchesData
{
   struct sqlm_recording_group         *piGroupStates; /* Pointer to group    */
                                                 /* states                    */
   void                                *poBuffer; /* Pointer to output        */
                                                 /* buffer                    */
   db2Uint32                           iBufferSize; /* Size of output buffer  */
   db2Uint32                           iReturnData; /* Return data flag       */
   db2Uint32                           iVersion; /* Snapshot version          */
   db2int32                            iNodeNumber; /* Target node            */
   db2Uint32                           *poOutputFormat; /* Pointer to output  */
                                                 /* format indicator          */
} db2gMonitorSwitchesData;

SQL_API_RC SQL_API_FN                            /* Get snapshot size         */
  db2MonitorSwitches (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

SQL_API_RC SQL_API_FN                            /* Get snapshot size         */
  db2gMonitorSwitches (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/* Monitor Reset Data Interface Structure                                     */
typedef SQL_STRUCTURE db2ResetMonitorData
{
   db2Uint32                           iResetAll; /* Reset value flag         */
   char                                *piDbAlias; /* Database alias          */
   db2Uint32                           iVersion; /* Snapshot version          */
   db2int32                            iNodeNumber; /* Target node            */
} db2ResetMonitorData;

/* Monitor Reset Data General Interface Structure                             */
typedef SQL_STRUCTURE db2gResetMonitorData
{
   db2Uint32                           iResetAll; /* Reset value flag         */
   char                                *piDbAlias; /* Database alias          */
   db2Uint32                           iDbAliasLength; /* Length of DB alias  */
   db2Uint32                           iVersion; /* Snapshot version          */
   db2int32                            iNodeNumber; /* Target node            */
} db2gResetMonitorData;

SQL_API_RC SQL_API_FN                            /* Get snapshot size         */
  db2ResetMonitor (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

SQL_API_RC SQL_API_FN                            /* Get snapshot size         */
  db2gResetMonitor (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/* Restart Database Structure                                                 */
typedef struct db2RestartDbStruct
{
   char                                *piDatabaseName; /* Database name or   */
                                                 /* alias                     */
   char                                *piUserId; /* User Id                  */
   char                                *piPassword; /* Password               */
   char                                *piTablespaceNames; /* Droppending     */
                                                 /* tablespace name list      */
   db2int32                            iOption;  /* Suspend or resume         */
} db2RestartDbStruct;

SQL_API_RC SQL_API_FN                            /* Database Restart          */
  db2DatabaseRestart (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

typedef struct db2gRestartDbStruct
{
   db2Uint32                           iDatabaseNameLen; /* Length in bytes   */
                                                 /* of piDatabaseName         */
   db2Uint32                           iUserIdLen; /* Length in bytes of      */
                                                 /* piUserId                  */
   db2Uint32                           iPasswordLen; /* Length in bytes of    */
                                                 /* piPassword                */
   db2Uint32                           iTablespaceNamesLen; /* Length in      */
                                                 /* bytes of                  */
                                                 /* piTablespaceNames         */
   char                                *piDatabaseName; /* Database name or   */
                                                 /* alias                     */
   char                                *piUserId; /* User Id                  */
   char                                *piPassword; /* Password               */
   char                                *piTablespaceNames; /* Tablespace      */
                                                 /* name list                 */
} db2gRestartDbStruct;

SQL_API_RC SQL_API_FN                            /* Generic Database Restart  */
  db2gDatabaseRestart (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/******************************************************************************
** Satellite Synchronization APIs and related definitions
*******************************************************************************/

/* Maximum length of a sync session ID                                        */
#define SQL_SYNCSESSIONID_SZ           18

/* Phases of a synchronization session, as                                    */
/* returned in db2QuerySatelliteProgressStruct.oStep                          */
#define DB2SYNC_SYNCMODE_PHASE_INIT    10
#define DB2SYNC_SYNCMODE_PHASE_EXECPREVSCRIPTS 20
#define DB2SYNC_SYNCMODE_PHASE_SENDPREVRESULTS 30
#define DB2SYNC_SYNCMODE_PHASE_SENTPREVRESULTS 40
#define DB2SYNC_SYNCMODE_PHASE_RETRIEVEBATCHES 50
#define DB2SYNC_SYNCMODE_PHASE_EXECUTESCRIPTS 60
#define DB2SYNC_SYNCMODE_PHASE_SENDRESULTS 70
#define DB2SYNC_SYNCMODE_PHASE_SENTRESULTS 80
#define DB2SYNC_SYNCMODE_PHASE_DONE    90

/* Phases of a synchronization test session, as                               */
/* returned in db2QuerySatelliteProgressStruct.oStep                          */
#define DB2SYNC_TESTMODE_PHASE_INIT    100
#define DB2SYNC_TESTMODE_PHASE_CONNTODB 110
#define DB2SYNC_TESTMODE_PHASE_DISCONNDB 120
#define DB2SYNC_TESTMODE_PHASE_DONE    130


/* Input parameters for the Set Synchronization Session API                   */
typedef struct db2SetSyncSessionStruct
{
   char                                *piSyncSessionID; /* ID for sync       */
                                                 /* session                   */
} db2SetSyncSessionStruct;

SQL_API_RC SQL_API_FN                            /* Set sync session for a    */
                                                 /* satellite                 */
  db2SetSyncSession (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* Input parameters          */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/* Output parameters for the Get Synchronization Session API                  */
typedef struct db2GetSyncSessionStruct
{
   char                                *poSyncSessionID; /* ID for sync       */
                                                 /* session                   */
} db2GetSyncSessionStruct;

SQL_API_RC SQL_API_FN                            /* Get sync session ID for   */
                                                 /* a satellite               */
  db2GetSyncSession (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* Output parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/* Output parameters for the Query Satellite Progress API                     */
typedef struct db2QuerySatelliteProgressStruct
{
   db2int32                            oStep;    /* current step of           */
                                                 /* synchronization           */
   db2int32                            oSubstep; /* substep of the current    */
                                                 /* step                      */
   db2int32                            oNumSubsteps; /* total number of       */
                                                 /* substeps                  */
   db2int32                            oScriptStep; /* step of current        */
                                                 /* script substep            */
   db2int32                            oNumScriptSteps; /* total number of    */
                                                 /* script steps              */
   char                                *poDescription; /* description of      */
                                                 /* step                      */
   char                                *poError; /* error text, if available  */
   char                                *poProgressLog; /* contents of         */
                                                 /* progress log              */
} db2QuerySatelliteProgressStruct;

SQL_API_RC SQL_API_FN                            /* Query Satellite Progress  */
  db2QuerySatelliteProgress (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* Ouput parameters          */
   struct sqlca * pSqlca);                       /* SQLCA                     */

SQL_API_RC SQL_API_FN                            /* Test Satellite's ability  */
                                                 /* to synchronize            */
  db2SyncSatelliteTest (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* Input/Ouput parameters    */
   struct sqlca * pSqlca);                       /* SQLCA                     */

SQL_API_RC SQL_API_FN                            /* Synchronize Satellite     */
  db2SyncSatellite (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* Input/Ouput parameters    */
   struct sqlca * pSqlca);                       /* SQLCA                     */

SQL_API_RC SQL_API_FN                            /* Stop Satellite's          */
                                                 /* Synchronization Session   */
  db2SyncSatelliteStop (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* Input/Ouput parameters    */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/* Administrative Message Logging Defines/Structure                           */

#define DB2LOG_SEVERE_ERROR 1
#define DB2LOG_ERROR        2
#define DB2LOG_WARNING      3
#define DB2LOG_INFORMATION  4

#define STRING_MSG          1
#define BINARY_MSG          2

typedef struct db2AdminMsgWriteStruct
{
   db2Uint32                           iMsgType; /* Message Type              */
   db2Uint32                           iComponent; /* Component id            */
   db2Uint32                           iFunction; /* Function id              */
   db2Uint32                           iProbeID; /* Numeric probe point       */
   char                                *piData_title; /* Title describing     */
                                                 /* data                      */
   void                                *piData;  /* Data                      */
   db2Uint32                           iDataLen; /* Length of binary data     */
   db2Uint32                           iError_type; /* Severity of message    */
} db2AdminMsgWriteStruct;

/* Administrative Message Logging                                             */

SQL_API_RC SQL_API_FN                            /* Administrative Message    */
                                                 /* Logging                   */
  db2AdminMsgWrite (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

typedef SQL_STRUCTURE db2LdapProtocolInfo
{
   char                                iType;
   char                                *piHostName;
   char                                *piServiceName;
   char                                *piNetbiosName;
   char                                *piNetworkId;
   char                                *piPartnerLU;
   char                                *piTPName;
   char                                *piMode;
   unsigned short                      iSecurityType;
   char                                *piLanAdapterAddress;
   char                                *piChangePasswordLU;
   char                                *piIpxAddress;
} db2LdapProtocolInfo;

typedef SQL_STRUCTURE db2LdapRegisterStruct
{
   char                                *piNodeName;
   char                                *piComputer;
   char                                *piInstance;
   unsigned short                      iNodeType;
   unsigned short                      iOsType;
   db2LdapProtocolInfo                 iProtocol;
   char                                *piComment;
   char                                *piBindDN;
   char                                *piPassword;
} db2LdapRegisterStruct;

SQL_API_RC SQL_API_FN                            /* Register DB2 server       */
  db2LdapRegister (
   db2Uint32 versionNumber,                      /* Version Number            */
   void * pParmStruct,                           /* Register Info             */
   struct sqlca * pSqlca);                       /* SQLCA                     */

typedef SQL_STRUCTURE db2LdapUpdateStruct
{
   char                                *piNodeName;
   char                                *piComment;
   unsigned short                      iNodeType;
   db2LdapProtocolInfo                 iProtocol;
   char                                *piBindDN;
   char                                *piPassword;
} db2LdapUpdateStruct;

SQL_API_RC SQL_API_FN                            /* Update DB2 server         */
  db2LdapUpdate (
   db2Uint32 versionNumber,                      /* Version Number            */
   void * pParmStruct,                           /* Update Info               */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/******************************************************************************
** Update LDAP Alternate Server Parameter Structure 
*******************************************************************************/
typedef SQL_STRUCTURE db2LdapUpdateAltServerStruct
{
   char                                *piDbAlias; /* Input: database alias   */
   char                                *piNode;  /* Input: node name          */
   char                                *piGWNode; /* Input: gateway node      */
                                                 /* name                      */
   char                                *piBindDN; /* Input: bind distinguish  */
                                                 /* name                      */
   char                                *piPassword; /* input: bind password   */
} db2LdapUpdateAltServerStruct;

/******************************************************************************
** Update LDAP Alternate Server API 
*******************************************************************************/
SQL_API_RC SQL_API_FN                            /* Update LDAP Alternate     */
                                                 /* Server                    */
  db2LdapUpdateAlternateServerForDB (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

typedef SQL_STRUCTURE db2LdapDeregisterStruct
{
   char                                *piNodeName;
   char                                *piBindDN;
   char                                *piPassword;
} db2LdapDeregisterStruct;

SQL_API_RC SQL_API_FN                            /* Deregister DB2 server     */
  db2LdapDeregister (
   db2Uint32 versionNumber,                      /* Version Number            */
   void * pParmStruct,                           /* DeRegister Info           */
   struct sqlca * pSqlca);                       /* SQLCA                     */

typedef SQL_STRUCTURE db2LdapCatalogNodeStruct
{
   char                                *piAlias;
   char                                *piNodeName;
   char                                *piBindDN;
   char                                *piPassword;
} db2LdapCatalogNodeStruct;

SQL_API_RC SQL_API_FN                            /* Catalog LDAP node         */
  db2LdapCatalogNode (
   db2Uint32 versionNumber,                      /* Version Number            */
   void * pParmStruct,                           /* Catalog Node Info         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

typedef SQL_STRUCTURE db2LdapUncatalogNodeStruct
{
   char                                *piAlias;
   char                                *piBindDN;
   char                                *piPassword;
} db2LdapUncatalogNodeStruct;

SQL_API_RC SQL_API_FN                            /* Uncatalog LDAP node       */
  db2LdapUncatalogNode (
   db2Uint32 versionNumber,                      /* Version Number            */
   void * pParmStruct,                           /* UnCatalog Node Info       */
   struct sqlca * pSqlca);                       /* SQLCA                     */

typedef SQL_STRUCTURE db2LdapCatalogDatabaseStruct
{
   char                                *piAlias;
   char                                *piDatabaseName;
   char                                *piComment;
   char                                *piNodeName;
   char                                *piGWNodeName;
   char                                *piParameters;
   char                                *piARLibrary;
   unsigned short                      iAuthentication;
   char                                *piDCEPrincipalName;
   char                                *piBindDN;
   char                                *piPassword;
} db2LdapCatalogDatabaseStruct;

SQL_API_RC SQL_API_FN                            /* Catalog LDAP database     */
  db2LdapCatalogDatabase (
   db2Uint32 versionNumber,                      /* Version Number            */
   void * pParmStruct,                           /* Catalog Database Info     */
   struct sqlca * pSqlca);                       /* SQLCA                     */

typedef SQL_STRUCTURE db2LdapUncatalogDatabaseStruct
{
   char                                *piAlias;
   char                                *piBindDN;
   char                                *piPassword;
} db2LdapUncatalogDatabaseStruct;

SQL_API_RC SQL_API_FN                            /* Uncatalog LDAP database   */
  db2LdapUncatalogDatabase (
   db2Uint32 versionNumber,                      /* Version Number            */
   void * pParmStruct,                           /* UnCatalog Database Info   */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/* Suspend I/O Database Structure                                             */
typedef struct db2SetWriteDbStruct
{
   db2int32                            iOption;  /* Suspend or resume         */
   char                                *piTablespaceNames; /* Suspending      */
                                                 /* tablespace name list:     */
                                                 /* empty for all             */
                                                 /* Tablespaces               */
} db2SetWriteDbStruct;

SQL_API_RC SQL_API_FN                            /* Set Write Suspend/Resume  */
                                                 /* For DB                    */
  db2SetWriteForDB (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */


/******************************************************************************
** Inspect Database Definitions
*******************************************************************************/

/* Inspect API structure                                                      */
typedef SQL_STRUCTURE db2InspectStruct
{
   char                                *piTablespaceName; /* Tablespace       */
                                                 /* name; null terminated     */
   char                                *piTableName; /* Table name; null      */
                                                 /* terminated                */
   char                                *piSchemaName; /* Schema name; null    */
                                                 /* terminated                */
   char                                *piResultsName; /* Results Output      */
                                                 /* file name; null           */
                                                 /* terminated                */
   char                                *piDataFileName; /* Data file name;    */
                                                 /* null terminated           */
   SQL_PDB_NODE_TYPE                   *piNodeList; /* Array of nodes         */
   db2Uint32                           iAction;  /* Action                    */
   db2int32                            iTablespaceID; /* Tablespace ID        */
   db2int32                            iObjectID; /* Object ID                */
   db2Uint32                           iFirstPage; /* Page number of first    */
                                                 /* page                      */
   db2Uint32                           iNumberOfPages; /* Number of pages     */
   db2Uint32                           iFormatType; /* Format type            */
   db2Uint32                           iOptions; /* Options                   */
   db2Uint32                           iBeginCheckOption; /* Begin checking   */
                                                 /* option                    */
   db2int32                            iLimitErrorReported; /* Number of      */
                                                 /* pages in error in object  */
                                                 /* to limit reporting of     */
   db2Uint16                           iObjectErrorState; /* Object error     */
                                                 /* state option              */
   db2Uint16                           iCatalogToTablespace; /* Catalog to    */
                                                 /* tablespace consistency    */
                                                 /* option                    */
   db2Uint16                           iKeepResultfile; /* Keep result file   */
   db2Uint16                           iAllNodeFlag; /* all node flag         */
   db2Uint16                           iNumNodes; /* Number of nodes          */
   db2Uint16                           iLevelObjectData; /* Processing level  */
                                                 /* for Data object           */
   db2Uint16                           iLevelObjectIndex; /* Processing       */
                                                 /* level for Index object    */
   db2Uint16                           iLevelObjectLong; /* Processing level  */
                                                 /* for Long object           */
   db2Uint16                           iLevelObjectLOB; /* Processing level   */
                                                 /* for LOB object            */
   db2Uint16                           iLevelObjectBlkMap; /* Processing      */
                                                 /* level for Block map       */
                                                 /* object                    */
   db2Uint16                           iLevelExtentMap; /* Processing level   */
                                                 /* for Extent map            */
} db2InspectStruct;


/* Generic Inspect API structure                                              */
typedef SQL_STRUCTURE db2gInspectStruct
{
   char                                *piTablespaceName; /* Tablespace name  */
   char                                *piTableName; /* Table name            */
   char                                *piSchemaName; /* Schema name          */
   char                                *piResultsName; /* Results Output      */
                                                 /* file name                 */
   char                                *piDataFileName; /* Data file name     */
   SQL_PDB_NODE_TYPE                   *piNodeList; /* Array of nodes         */
   db2Uint32                           iResultsNameLength; /* Results Output  */
                                                 /* file name length          */
   db2Uint32                           iDataFileNameLength; /* Data file      */
                                                 /* name length               */
   db2Uint32                           iTablespaceNameLength; /* Tablespace   */
                                                 /* name length               */
   db2Uint32                           iTableNameLength; /* Table name        */
                                                 /* length                    */
   db2Uint32                           iSchemaNameLength; /* Schema name      */
                                                 /* length                    */
   db2Uint32                           iAction;  /* Action                    */
   db2int32                            iTablespaceID; /* Tablespace ID        */
   db2int32                            iObjectID; /* Object ID                */
   db2Uint32                           iFirstPage; /* Page number of first    */
                                                 /* page                      */
   db2Uint32                           iNumberOfPages; /* Number of pages     */
   db2Uint32                           iFormatType; /* Format type            */
   db2Uint32                           iOptions; /* Options                   */
   db2Uint32                           iBeginCheckOption; /* Begin checking   */
                                                 /* option                    */
   db2int32                            iLimitErrorReported; /* Number of      */
                                                 /* pages in error in object  */
                                                 /* to limit reporting of     */
   db2Uint16                           iObjectErrorState; /* Object error     */
                                                 /* state option              */
   db2Uint16                           iCatalogToTablespace; /* Catalog to    */
                                                 /* tablespace consistency    */
                                                 /* option                    */
   db2Uint16                           iKeepResultfile; /* Keep result file   */
   db2Uint16                           iAllNodeFlag; /* all node flag         */
   db2Uint16                           iNumNodes; /* Number of nodes          */
   db2Uint16                           iLevelObjectData; /* Processing level  */
                                                 /* for Data object           */
   db2Uint16                           iLevelObjectIndex; /* Processing       */
                                                 /* level for Index object    */
   db2Uint16                           iLevelObjectLong; /* Processing level  */
                                                 /* for Long object           */
   db2Uint16                           iLevelObjectLOB; /* Processing level   */
                                                 /* for LOB object            */
   db2Uint16                           iLevelObjectBlkMap; /* Processing      */
                                                 /* level for Block map       */
                                                 /* object                    */
   db2Uint16                           iLevelExtentMap; /* Processing level   */
                                                 /* for Extent map            */
} db2gInspectStruct;


/* Values for db2Inspect - Action constants                                   */
#define DB2INSPECT_ACTION_CHECK        1
#define DB2INSPECT_ACTION_FORMAT       65536

/* Values for db2Inspect - iAction                                            */
/* DB2INSPECT_ACT_CHECK_RESTART   - Inspect check restart                     */
/* DB2INSPECT_ACT_CHECK_DB        - Inspect DB                                */
/* DB2INSPECT_ACT_CHECK_TABSPACE  - Inspect tablespace                        */
/* DB2INSPECT_ACT_CHECK_TABLE     - Inspect table                             */
/* DB2INSPECT_ACT_CHECK_DATA      - Inspect data object                       */
/* DB2INSPECT_ACT_CHECK_INDEX     - Inspect index object                      */
/* DB2INSPECT_ACT_CHECK_BLOCKMAP  - Inspect block map object                  */
/* DB2INSPECT_ACT_FORMAT_PAGE     - Format page in tablespace                 */
/* DB2INSPECT_ACT_FORMAT_EXT_MAP  - Format extent map                         */
/* DB2INSPECT_ACT_FORMAT_DATA     - Format data page                          */
/* DB2INSPECT_ACT_FORMAT_INDEX    - Format index page                         */
/* DB2INSPECT_ACT_FORMAT_BLOCKMAP - Format block map page                     */
#define DB2INSPECT_ACT_CHECK_RESTART   (DB2INSPECT_ACTION_CHECK+1) 
#define DB2INSPECT_ACT_CHECK_DB        (DB2INSPECT_ACTION_CHECK+2) 
#define DB2INSPECT_ACT_CHECK_TABSPACE  (DB2INSPECT_ACTION_CHECK+3) 
#define DB2INSPECT_ACT_CHECK_TABLE     (DB2INSPECT_ACTION_CHECK+4) 
#define DB2INSPECT_ACT_CHECK_DATA      (DB2INSPECT_ACTION_CHECK+5) 
#define DB2INSPECT_ACT_CHECK_INDEX     (DB2INSPECT_ACTION_CHECK+6) 
#define DB2INSPECT_ACT_CHECK_BLOCKMAP  (DB2INSPECT_ACTION_CHECK+7) 
#define DB2INSPECT_ACT_CHECK_08        (DB2INSPECT_ACTION_CHECK+8) 
#define DB2INSPECT_ACT_CHECK_09        (DB2INSPECT_ACTION_CHECK+9) 
#define DB2INSPECT_ACT_CHECK_10        (DB2INSPECT_ACTION_CHECK+10) 
#define DB2INSPECT_ACT_FORMAT_PAGE     (DB2INSPECT_ACTION_FORMAT+1) 
#define DB2INSPECT_ACT_FORMAT_EXT_MAP  (DB2INSPECT_ACTION_FORMAT+2) 
#define DB2INSPECT_ACT_FORMAT_DATA     (DB2INSPECT_ACTION_FORMAT+3) 
#define DB2INSPECT_ACT_FORMAT_INDEX    (DB2INSPECT_ACTION_FORMAT+4) 
#define DB2INSPECT_ACT_FORMAT_BLOCKMAP (DB2INSPECT_ACTION_FORMAT+5) 
#define DB2INSPECT_ACT_FORMAT_06       (DB2INSPECT_ACTION_FORMAT+6) 
#define DB2INSPECT_ACT_FORMAT_07       (DB2INSPECT_ACTION_FORMAT+7) 
#define DB2INSPECT_ACT_FORMAT_08       (DB2INSPECT_ACTION_FORMAT+8) 
#define DB2INSPECT_ACT_FORMAT_09       (DB2INSPECT_ACTION_FORMAT+9) 
#define DB2INSPECT_ACT_FORMAT_10       (DB2INSPECT_ACTION_FORMAT+10) 

/* Values for db2Inspect - iFormatType                                        */
#define DB2INSPECT_FORMAT_TYPE_HEX     1
#define DB2INSPECT_FORMAT_TYPE_BRIEF   2
#define DB2INSPECT_FORMAT_TYPE_DETAIL  3
#define DB2INSPECT_FORMAT_TYPE_DEL     4

/* Values for db2Inspect - iOptions                                           */
#define DB2INSPECT_OPTS_PAGE_LOGICAL   0         /* Format - 1st page number  */
                                                 /* logical                   */
#define DB2INSPECT_OPTS_PAGE_PHYSICAL  1         /* Format - 1st page number  */
                                                 /* physical                  */

/* Values for db2Inspect - iBeginCheckOption                                  */
#define DB2INSPECT_BEGIN_FROM_START    0x0       /* Begin from the start.     */
#define DB2INSPECT_BEGIN_TSPID         0x1       /* Begin check on            */
                                                 /* tablespace ID.            */
#define DB2INSPECT_BEGIN_OBJID         0x2       /* Begin check on object     */
                                                 /* ID.                       */
#define DB2INSPECT_BEGIN_TSPID_OBJID   (DB2INSPECT_BEGIN_TSPID | DB2INSPECT_BEGIN_OBJID)

/* Values for db2Inspect - iLimitErrorReported                                */
#define DB2INSPECT_LIMIT_ERROR_DEFAULT 0         /* limit errors reported to  */
                                                 /* default number            */
#define DB2INSPECT_LIMIT_ERROR_ALL     -1        /* no limit to errors        */
                                                 /* reported                  */

/* Values for db2Inspect - iObjectErrorState                                  */
#define DB2INSPECT_ERROR_STATE_NORMAL  0         /* process object only in    */
                                                 /* normal state              */
#define DB2INSPECT_ERROR_STATE_ALL     1         /* process object in any     */
                                                 /* state                     */

/* Values for db2Inspect - iCatalogToTablespace                               */
#define DB2INSPECT_CAT_TO_TABSP_NONE   0         /* Catalog to tablespace     */
                                                 /* consistency not           */
                                                 /* requested                 */
#define DB2INSPECT_CAT_TO_TABSP_YES    1         /* Catalog to tablespace     */
                                                 /* consistency requested     */

/* Values for db2Inspect - iKeepResultfile                                    */
#define DB2INSPECT_RESFILE_CLEANUP     0         /* Clean up result file      */
                                                 /* when no error             */
#define DB2INSPECT_RESFILE_KEEP_ALWAYS 1         /* Always keep result file   */

/* Values for db2Inspect - Level constants                                    */
#define DB2INSPECT_LEVEL_DEFAULT       0
#define DB2INSPECT_LEVEL_NONE          1
#define DB2INSPECT_LEVEL_NORMAL        DB2INSPECT_LEVEL_DEFAULT
#define DB2INSPECT_LEVEL_LOW           16384
#define DB2INSPECT_LEVEL_HIGH          49152

/* Values for db2Inspect - iNumberOfPages                                     */
#define DB2INSPECT_NUMPAGES_TO_THE_END 0         /* To the last page          */

/* Inspect API                                                                */
SQL_API_RC SQL_API_FN                            /* DB2 Inspect               */
  db2Inspect (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */

/* Generic Inspect API                                                        */
SQL_API_RC SQL_API_FN                            /* DB2 Inspect               */
  db2gInspect (
   db2Uint32 versionNumber,                      /* Database version number   */
   void * pParmStruct,                           /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */



/******************************************************************************
** Utility Control API
*******************************************************************************/

/* Utility Throttling Constants                                               */
#define UTIL_IMPACT_PRIORITY_DFLT      50        /* Default throttling        */
                                                 /* priority                  */

/* Utility Control Structure                                                  */
typedef SQL_STRUCTURE db2UtilityControlStruct
{
   db2Uint32                           iID;      /* Utility id                */
   db2Uint32                           iAttribute; /* Attribute to modify     */
   void                                *pioValue; /* Attribute data           */
} db2UtilityControlStruct;

/* Valid utility attributes                                                   */
#define DB2UTILCTRL_PRIORITY_ATTRIB    1         /* Modify utility priority   */

/* Utility Control API                                                        */
SQL_API_RC SQL_API_FN                            /* Modify attributes of      */
                                                 /* active utilities          */
  db2UtilityControl (
   db2Uint32 version,                            /* Database version number   */
   void * pUtilityControlStruct,                 /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */


/* Generic Utility Control Structure                                          */
typedef SQL_STRUCTURE db2gUtilityControlStruct
{
   db2Uint32                           iID;      /* Utility id                */
   db2Uint32                           iAttribute; /* Attribute to modify     */
   void                                *pioValue; /* Attribute data           */
} db2gUtilityControlStruct;


/* Generic Utility Control API                                                */
SQL_API_RC SQL_API_FN                            /* Modify attributes of      */
                                                 /* active utilities          */
  db2gUtilityControl (
   db2Uint32 version,                            /* Database version number   */
   void * pgUtilityControlStruct,                /* In/out parameters         */
   struct sqlca * pSqlca);                       /* SQLCA                     */


/******************************************************************************
** Quiesce Definitions
*******************************************************************************/

/* Database Quiesce Struct                                                    */
typedef SQL_STRUCTURE db2DbQuiesceStruct
{
  char              *piDatabaseName;  /* Database to be quiesced              */
  db2Uint32         iImmediate;       /* Immediate or deferred                */
  db2Uint32         iForce;           /* Force Connections                    */
  db2Uint32         iTimeout;         /* Defer timeout in minutes             */
} db2DbQuiesceStruct;


/* Database Quiesce API                                                       */
SQL_API_RC SQL_API_FN                        /* Database Quiesce              */
  db2DatabaseQuiesce (
             db2Uint32 versionNumber,        /* Version Number                */
             void * pParmStruct,             /* Database Quiesce Info         */
             struct sqlca * pSqlca);         /* SQLCA                         */


/* Generic Database Quiesce Struct                                            */
typedef SQL_STRUCTURE db2gDbQuiesceStruct
{
  db2Uint32         iDatabaseNameLen; /* Database Name Length                 */
  char              *piDatabaseName;  /* Database to be quiesced              */
  db2Uint32         iImmediate;       /* Immediate or deferred                */
  db2Uint32         iForce;           /* Force Connections                    */
  db2Uint32         iTimeout;         /* Defer timeout in minutes             */
} db2gDbQuiesceStruct;


/* Generic Database Quiesce API                                               */
SQL_API_RC SQL_API_FN                        /* Generic Database Quiesce      */
  db2gDatabaseQuiesce (
             db2Uint32 versionNumber,        /* Version Number                */
             void * pParmStruct,             /* Database Quiesce Info         */
             struct sqlca * pSqlca);         /* SQLCA                         */


/* Database Unquiesce Struct                                                  */
typedef SQL_STRUCTURE db2DbUnquiesceStruct
{
             char          *piDatabaseName;  /* Database to be unquiesced     */
} db2DbUnquiesceStruct;


/* Database Unquiesce API                                                     */
SQL_API_RC SQL_API_FN                        /* Database unquiesce            */
  db2DatabaseUnquiesce (
             db2Uint32 versionNumber,        /* Version Number                */
             void * pParmStruct,             /* Database Unquiesce Info       */
             struct sqlca * pSqlca);         /* SQLCA                         */


/* Generic Database Unquiesce Struct                                          */
typedef SQL_STRUCTURE db2gDbUnquiesceStruct
{
             db2Uint32     iDatabaseNameLen; /* Database Name Length          */
             char          *piDatabaseName;  /* Database to be unquiesced     */
} db2gDbUnquiesceStruct;


/* Generic Database Unquiesce API                                             */
SQL_API_RC SQL_API_FN                        /* Database unquiesce            */
  db2gDatabaseUnquiesce (
             db2Uint32 versionNumber,        /* Version Number                */
             void * pParmStruct,             /* Database Unquiesce Info       */
             struct sqlca * pSqlca);         /* SQLCA                         */


/* Instance Quiesce Struct                                                    */
typedef SQL_STRUCTURE db2InsQuiesceStruct
{
             char          *piInstanceName;  /* Instance to be quiesced       */
             char          *piUserId;        /* Quiesce User Id               */
             char          *piGroupId;       /* Quiesce Group Id              */
             db2Uint32     iImmediate;       /* Immediate or deferred         */
             db2Uint32     iForce;           /* Force Connections             */
             db2Uint32     iTimeout;         /* Defer timeout in minutes      */
} db2InsQuiesceStruct;


/* Instance Quiesce API                                                       */
SQL_API_RC SQL_API_FN                        /* Instance Quiesce              */
  db2InstanceQuiesce (
             db2Uint32 versionNumber,        /* Version Number                */
             void * pParmStruct,             /* Instance Quiesce Info         */
             struct sqlca * pSqlca);         /* SQLCA                         */


/* Generic Instance Quiesce Struct                                            */
typedef SQL_STRUCTURE db2gInsQuiesceStruct
{
             db2Uint32     iInstanceNameLen; /* InstanceName Length           */
             char          *piInstanceName;  /* Instance to be quiesced       */
             db2Uint32     iUserIdLen;       /* UserId Length                 */
             char          *piUserId;        /* Quiesce User Id               */
             db2Uint32     iGroupIdLen;      /* GroupId Length                */
             char          *piGroupId;       /* Quiesce Group Id              */
             db2Uint32     iImmediate;       /* Immediate or deferred         */
             db2Uint32     iForce;           /* Force Connections             */
             db2Uint32     iTimeout;         /* Defer timeout in minutes      */
} db2gInsQuiesceStruct;


/* Generic Instance Quiesce API                                               */
SQL_API_RC SQL_API_FN                        /* Generic Instance Quiesce      */
  db2gInstanceQuiesce (
             db2Uint32 versionNumber,        /* Version Number                */
             void * pParmStruct,             /* Instance Quiesce Info         */
             struct sqlca * pSqlca);         /* SQLCA                         */


/* Instance Unquiesce Struct                                                  */
typedef SQL_STRUCTURE db2InsUnquiesceStruct
{
             char          *piInstanceName;  /* Instance to be unquiesced     */
} db2InsUnquiesceStruct;


/* Instance Unquiesce API                                                     */
SQL_API_RC SQL_API_FN                        /* Instance Unquiesce            */
  db2InstanceUnquiesce (
             db2Uint32 versionNumber,        /* Version Number                */
             void * pParmStruct,             /* Instance Unquiesce Info       */
             struct sqlca * pSqlca);         /* SQLCA                         */


/* Generic Instance Unquiesce Struct                                          */
typedef SQL_STRUCTURE db2gInsUnquiesceStruct
{
             db2Uint32     iInstanceNameLen; /* InstanceName Length           */
             char          *piInstanceName;  /* Instance to be unquiesced     */
} db2gInsUnquiesceStruct;


/* Generic Instance Unquiesce API                                             */
SQL_API_RC SQL_API_FN                        /* Generic Instance Unquiesce    */
  db2gInstanceUnquiesce (
             db2Uint32 versionNumber,        /* Version Number                */
             void * pParmStruct,             /* Instance Unquiesce Info       */
             struct sqlca * pSqlca);         /* SQLCA                         */


/* db2DasCommData                                                             */
typedef SQL_STRUCTURE db2DasCommData
{
             db2int8       iCommParam;       /* Flag for Nodename or          */
                                             /* Hostname specified            */
             char          *piNodeOrHostName; /* Node or HostName             */
             char          *piUserId;        /* User id                       */
             char          *piUserPw;        /* User password                 */
} db2DasCommData;


/* db2gDasCommData                                                            */
typedef SQL_STRUCTURE db2gDasCommData
{
             db2int8       iCommParam;       /* Flag for Nodename or          */
                                             /* Hostname specified            */
             db2Uint32     iNodeOrHostNameLen; /* Node or HostName            */
             char          *piNodeOrHostName; /* Node or HostName             */
             db2Uint32     iUserIdLen;       /* iUserIdLen                    */
             char          *piUserId;        /* User id                       */
             db2Uint32     iUserPwLen;       /* iUserPwLen                    */
             char          *piUserPw;        /* User password                 */
} db2gDasCommData;


/* db2QuiesceStartStruct                                                      */
typedef SQL_STRUCTURE db2QuiesceStartStruct
{
             db2int8       iIsQRequested;    /* is Quiesce requested?         */
             char          *piQUsrName;      /* Quiesce user name             */
             char          *piQGrpName;      /* Quiesce group name            */
             db2int8       iIsQUsrGrpDef;    /* is UserGroup Defined?         */
} db2QuiesceStartStruct;


/* db2gQuiesceStartStruct                                                     */
typedef SQL_STRUCTURE db2gQuiesceStartStruct
{
             db2int8       iIsQRequested;    /* is Quiesce requested?         */
             db2Uint32     iQUsrNameLen;     /* iQUserNameLen                 */
             char          *piQUsrName;      /* Quiesce user name             */
             db2Uint32     iQGrpNameLen;     /* iQGrpNameLen                  */
             char          *piQGrpName;      /* Quiesce group name            */
             db2int8       iIsQUsrGrpDef;    /* is UserGroup Defined?         */
} db2gQuiesceStartStruct;


/* db2StartOptionsStruct                                                      */
typedef SQL_STRUCTURE db2StartOptionsStruct
{
             db2Uint32     iIsProfile;       /* is Profile specified?         */
             char          *piProfile;       /* Profile to be executed prior  */
                                             /* to a start                    */
             db2Uint32     iIsNodeNum;       /* is Node number specified?     */
             db2NodeType   iNodeNum;         /* Node number                   */
             db2Uint32     iOption;          /* Start option                  */
             db2Uint32     iIsHostName;      /* is Hostname specified?        */
             char          *piHostName;      /* Hostname                      */
             db2Uint32     iIsPort;          /* is Port specified?            */
             db2PortType   iPort;            /* Port                          */
             db2Uint32     iIsNetName;       /* is Netname specified?         */
             char          *piNetName;       /* Netname                       */
             db2Uint32     iTblspaceType;    /* Addnode tablespace type       */
             db2NodeType   iTblspaceNode;    /* Addnode tablespace node       */
             db2Uint32     iIsComputer;      /* is Computername               */
                                             /* specified(for Windows)?       */
             char          *piComputer;      /* Computer name                 */
             char          *piUserName;      /* Logon account user name       */
             char          *piPassword;      /* Logon account password        */
             db2QuiesceStartStruct  iQuiesceOpts; /* Node number              */
} db2StartOptionsStruct;


/* db2gStartOptionsStruct                                                     */
typedef SQL_STRUCTURE db2gStartOptionsStruct
{
             db2Uint32     iIsProfile;       /* is Profile specified?         */
             char          *piProfile;       /* Profile to be executed prior  */
                                             /* to a start                    */
             db2Uint32     iIsNodeNum;       /* is Node number specified?     */
             db2NodeType   iNodeNum;         /* Node number                   */
             db2Uint32     iOption;          /* Start option                  */
             db2Uint32     iIsHostName;      /* is Hostname specified?        */
             char          *piHostName;      /* Hostname                      */
             db2Uint32     iIsPort;          /* is Port specified?            */
             db2PortType   iPort;            /* Port                          */
             db2Uint32     iIsNetName;       /* is Netname specified?         */
             char          *piNetName;       /* Netname                       */
             db2Uint32     iTblspaceType;    /* Addnode tablespace type       */
             db2NodeType   iTblspaceNode;    /* Addnode tablespace node       */
             db2Uint32     iIsComputer;      /* is Computername               */
                                             /* specified(for Windows)?       */
             char          *piComputer;      /* Computer name                 */
             char          *piUserName;      /* Logon account user name       */
             char          *piPassword;      /* Logon account password        */
             db2gQuiesceStartStruct  iQuiesceOpts; /* Node number             */
} db2gStartOptionsStruct;


/* db2InstanceStartStruct                                                     */
typedef SQL_STRUCTURE db2InstanceStartStruct
{
             db2int8       iIsRemote;        /* is it a remote start?         */
             char          *piRemoteInstName; /* The name of the remote       */
                                             /* instance to be started.       */
             db2DasCommData * piCommData;    /* Remote start structure for    */
                                             /* DAS                           */
             db2StartOptionsStruct * piStartOpts; /* db2StartOptions          */
                                             /* structure                     */
} db2InstanceStartStruct;


/* db2gInstanceStStruct                                                       */
typedef SQL_STRUCTURE db2gInstanceStStruct
{
             db2int8       iIsRemote;        /* is it a remote start?         */
             db2Uint32     iRemoteInstLen;   /* iRemoteInstLen                */
             char          *piRemoteInstName; /* The name of the remote       */
                                             /* instance to be started.       */
             db2gDasCommData * piCommData;   /* Remote start structure for    */
                                             /* DAS                           */
             db2gStartOptionsStruct * piStartOpts; /* db2gStartOptions        */
                                             /* structure                     */
} db2gInstanceStStruct;



/* Instance Start API                                                         */
SQL_API_RC SQL_API_FN                        /* Instance Start                */
  db2InstanceStart (
             db2Uint32 versionNumber,        /* Version Number                */
             void * pParmStruct,             /* Start Options                 */
             struct sqlca * pSqlca);         /* SQLCA                         */


/* Generic Instance Start API                                                 */
SQL_API_RC SQL_API_FN                        /* Generic Instance Start        */
  db2gInstanceStart (
             db2Uint32 versionNumber,        /* Version Number                */
             void * pParmStruct,             /* Start Options                 */
             struct sqlca * pSqlca);         /* SQLCA                         */


/* db2StopOptionsStruct                                                       */
typedef SQL_STRUCTURE db2StopOptionsStruct
{
             db2Uint32     iIsProfile;       /* is Profile specified?         */
             char          *piProfile;       /* Profile                       */
             db2Uint32     iIsNodeNum;       /* is Node number specified?     */
             db2NodeType   iNodeNum;         /* Node number                   */
             db2Uint32     iStopOption;      /* Stop option (drop/force)      */
             db2Uint32     iCallerac;        /* if stop option is DROP NODE,  */
                                             /* this could be                 */
                                             /* DROP,CONTINUE,TERMINATE       */
} db2StopOptionsStruct;

/* db2InstanceStopStruct                                                      */
typedef SQL_STRUCTURE db2InstanceStopStruct
{
             db2int8       iIsRemote;        /* is it a remote start?         */
             char          *piRemoteInstName; /* The name of the remote       */
                                             /* instance to be started.       */
             db2DasCommData * piCommData;    /* Remote start structure for    */
                                             /* DAS                           */
             db2StopOptionsStruct * piStopOpts; /* db2StopOptions structure   */
} db2InstanceStopStruct;

/* db2gInstanceStopStruct                                                     */
typedef SQL_STRUCTURE db2gInstanceStopStruct
{
             db2int8       iIsRemote;        /* is it a remote start?         */
             db2Uint32     iRemoteInstLen;   /* iRemoteInstLen                */
             char          *piRemoteInstName; /* The name of the remote       */
                                             /* instance to be started.       */
             db2gDasCommData * piCommData;   /* Remote start structure for    */
                                             /* DAS                           */
             db2StopOptionsStruct * piStopOpts; /* db2StopOptions structure   */
} db2gInstanceStopStruct;


/* Instance Stop API                                                          */
SQL_API_RC SQL_API_FN                        /* Instance Stop                 */
  db2InstanceStop (
             db2Uint32 versionNumber,        /* Version Number                */
             void * pParmStruct,             /* Stop Options                  */
             struct sqlca * pSqlca);         /* SQLCA                         */


/* Generic Instance Stop API                                                  */
SQL_API_RC SQL_API_FN                        /* Generic Instance Stop         */
  db2gInstanceStop (
             db2Uint32 versionNumber,        /* Version Number                */
             void * pParmStruct,             /* Stop Options                  */
             struct sqlca * pSqlca);         /* SQLCA                         */




/******************************************************************************
** Health Monitor Alert Configuration
** Structures, Constants, and Function Prototypes
*******************************************************************************/

/******************************************************************************
** Type of DB2 objects for which one can configure health alerts
** (db2GetAlertCfgData.iObjType)
*******************************************************************************/
#define DB2ALERTCFG_OBJTYPE_DBM         0    /* DBM alert cfg                 */
#define DB2ALERTCFG_OBJTYPE_DATABASE    1    /* Alert cfg for a DB            */
#define DB2ALERTCFG_OBJTYPE_TABLESPACE  2    /* Alert cfg for a TS            */
#define DB2ALERTCFG_OBJTYPE_TS_CONTAINER 3   /* Alert cfg for a TSC           */

#define DB2ALERTCFG_OBJTYPE_DATABASES   1001 /* Alert cfg for all DBs         */
#define DB2ALERTCFG_OBJTYPE_TABLESPACES 1002 /* Alert cfg for all TSs         */
#define DB2ALERTCFG_OBJTYPE_TS_CONTAINERS 1003 /* Alert cfg for a TSCs        */

/******************************************************************************
** Type of DB2 defaults the user wants the health alerts for
** (db2GetAlertCfgData.iDefault)
*******************************************************************************/
#define DB2ALERTCFG_NOT_DEFAULT         0    /* Defaults not requested.       */
                                             /* Current Settings will be      */
                                             /* returned.                     */
#define DB2ALERTCFG_CURRENT_DEFAULT     1    /* The current default settings  */
                                             /* for an object of that type    */
                                             /* will be returned.(Those are   */
                                             /* user configurable)            */

/******************************************************************************
** Types of scripts supported as alert actions (db2AlertScriptAction.scriptType)
*******************************************************************************/
#define DB2ALERTCFG_SCRIPTTYPE_DB2CMD   0x8001 /* DB2 Command script          */
#define DB2ALERTCFG_SCRIPTTYPE_OS       0x8002 /* Operating System script     */

/******************************************************************************
** Alert attributes that can be configured (db2AlertAttrib.attribID)
*******************************************************************************/
#define DB2ALERTCFG_ALARM               0    /* alarm threshold               */
#define DB2ALERTCFG_WARNING             1    /* warning threshold             */
#define DB2ALERTCFG_SENSITIVITY         2    /* Min time in threshold zone    */
#define DB2ALERTCFG_ACTIONS_ENABLED     3    /* Threshold breach actionable?  */
#define DB2ALERTCFG_THRESHOLD_CHECK     4    /* Evaluate thresholds or        */
                                             /* state?                        */

/******************************************************************************
** Script action attributes that can be configured (db2AlertAttrib.attribID)
*******************************************************************************/
#define DB2ALERTCFG_SCRIPT_TYPE         0    /* Type of action script         */
#define DB2ALERTCFG_SCRIPT_WORKINGDIR   1    /* Script's working directory    */
#define DB2ALERTCFG_SCRIPT_STMTTERMCHAR 2    /* Statement termination char    */
#define DB2ALERTCFG_SCRIPT_CMDLINEPARMS 3    /* Command-line parameters       */
#define DB2ALERTCFG_SCRIPT_USERID       4    /* Script execution userid       */
#define DB2ALERTCFG_SCRIPT_PASSWORD     5    /* Script execution password     */
#define DB2ALERTCFG_SCRIPT_NAME         6    /* Script Name                   */
#define DB2ALERTCFG_SCRIPT_CONDITION    7    /* Script condition              */
#define DB2ALERTCFG_SCRIPT_HOSTNAME     8    /* host name to run script on    */

/******************************************************************************
** Task action attributes that can be configured (db2AlertAttrib.attribID)
*******************************************************************************/
#define DB2ALERTCFG_TASK_USERID         4    /* Task execution userid         */
#define DB2ALERTCFG_TASK_PASSWORD       5    /* Task execution password       */
#define DB2ALERTCFG_TASK_NAME           6    /* Task Name                     */
#define DB2ALERTCFG_TASK_CONDITION      7    /* Task condition                */
#define DB2ALERTCFG_TASK_HOSTNAME       8    /* host name to find a task      */

/******************************************************************************
** Types of alert actions 
** (db2AlertActionDelete.actionType)
** (db2AlertActionUpdate.actionType)
** (db2AlertActionNew.actionType)
*******************************************************************************/
#define DB2ALERTCFG_ACTIONTYPE_SCRIPT   0x8000 /* Alert action is a script    */
#define DB2ALERTCFG_ACTIONTYPE_TASK     0x4000 /* Alert action is a task      */

/******************************************************************************
** Conditions for scripts and tasks execution
** (db2AlertTaskAction.condition) and (db2AlertScriptAction.condition).
** For state based health indicators, use the states returned
** by the health snapshot present in sqlmon.h
*******************************************************************************/
#define DB2ALERTCFG_CONDITION_ALL       0xffffffff /* Run script/task on any  */
                                             /* alarm or non normal state     */
#define DB2ALERTCFG_CONDITION_WARNING   0xfffffffe /* Run script/task on      */
                                             /* warning                       */
#define DB2ALERTCFG_CONDITION_ALARM     0xfffffffd /* Run script/task on      */
                                             /* ALARM                         */

/******************************************************************************
** Values returned in (db2GetAlertCfgInd.oDefault) for alert configuration default.
*******************************************************************************/
#define DB2ALERTCFG_INHERIT_DEFAULT_NO  0x0  /* Configuration not inherited   */
                                             /* from default.                 */
#define DB2ALERTCFG_INHERIT_DEFAULT_YES 0x1  /* Configuration inherited from  */
                                             /* default.                      */
#define DB2ALERTCFG_INHERIT_DEFAULT_UNKNOWN 0x2 /* Unknown if configuration   */
                                             /* is inherited from default     */
                                             /* (8.1 servers only)            */


/******************************************************************************
** Structure for returning alert configuration using the db2GetAlertCfg API
*******************************************************************************/
typedef SQL_STRUCTURE db2GetAlertCfgData
{
   db2Uint32                 iObjType;      /* DB2 object type                */
   char                      *piObjName;    /* Name of DB2 object.            */
                                            /* For DB2ALERTCFG_OBJTYPE_TS     */
                                            /* CONTAINER, this name is        */
                                            /* defined as <tablespace         */
                                            /* Name>.<Table Space Container   */
                                            /* Name> .For all other objects,  */
                                            /* this name is simply the        */
                                            /* actual object name.            */
   db2Uint32                 iDefault;      /* Return default configuration   */
                                            /* for an object of type          */
                                            /* iObjType.                      */
                                            /*                                */
                                            /* To get the current             */
                                            /* configuration, set to          */
                                            /* DB2ALERTCFG_NOT_DEFAULT.       */
                                            /*                                */
                                            /* To get the default             */
                                            /* configuration, set to          */
                                            /* DB2ALERTCFG_CURRENT_DEFAULT.   */
                                            /*                                */
   char                      *piDbName;     /* Database alias                 */
   db2Uint32                 ioNumIndicators; /* Array length of pIndicators  */
   struct db2GetAlertCfgInd  *pioIndicators; /* Input:                        */
                                            /*                                */
                                            /* To get the configuration of    */
                                            /* all HI set to NULL.            */
                                            /*                                */
                                            /* To get the configuration of a  */
                                            /* specific HI, pass an array of  */
                                            /* db2GetAlertCfgInd and set the  */
                                            /* indicatorID fields to desired  */
                                            /* HIs and other fields to 0 or   */
                                            /* NULL                           */
                                            /*                                */
                                            /* Output:                        */
                                            /*                                */
                                            /* Array of cfg per indicator     */
                                            /* allocated by the API.          */
} db2GetAlertCfgData;


/******************************************************************************
** Structure for defining the parameters of a Task action
*******************************************************************************/
typedef SQL_STRUCTURE db2AlertTaskAction
{
   char                      *pTaskName;    /* Name of the task.              */
                                            /* This should be a NULL          */
                                            /* terminated string of the       */
                                            /* form: <task ID Number>.<task   */
                                            /* Suffix Number>                 */
   db2Uint32                 condition;     /* Condition needed to run task   */
   char                      *pUserID;      /* User acct under which task     */
                                            /* runs                           */
   char                      *pPassword;    /* Password for pUserID           */
   char                      *pHostName;    /* Host name where to find the    */
                                            /* task                           */
} db2AlertTaskAction;


/******************************************************************************
** Structure for defining the parameters of a script action
*******************************************************************************/
typedef SQL_STRUCTURE db2AlertScriptAction
{
   db2Uint32                 scriptType;    /* DB2 or OS script               */
   db2Uint32                 condition;     /* Condition needed to run        */
                                            /* script                         */
   char                      *pPathName;    /* Absolute pathname of script    */
   char                      *pWorkingDir;  /* Dir in which script runs       */
   char                      *pCmdLineParms; /* Cmd-line parms for OS         */
                                            /* scripts only                   */
   char                      stmtTermChar;  /* Char that terminates stmts in  */
                                            /* DB2 Cmd script                 */
   char                      *pUserID;      /* User acct under which script   */
                                            /* runs                           */
   char                      *pPassword;    /* Password for pUserID           */
   char                      *pHostName;    /* Host name where to find and    */
                                            /* run script                     */
} db2AlertScriptAction;


/******************************************************************************
** Structure containing configuration parameters for a single health indicator
*******************************************************************************/
typedef SQL_STRUCTURE db2GetAlertCfgInd
{
   db2Uint32                 ioIndicatorID; /* Health indicator identifier    */
   sqlint64                  oAlarm;        /* alarm threshold                */
   sqlint64                  oWarning;      /* warning threshold              */
   db2Uint32                 oSensitivity;  /* Min time within threshold      */
                                            /* region before alert            */
   char                      *poFormula;    /* Formula to computer health     */
                                            /* indicator                      */
   db2Uint32                 oActionEnabled; /* Alert actions enabled?        */
   db2Uint32                 oCheckThresholds; /* Threshold checking on?      */
   db2Uint32                 oNumTaskActions; /* Number of task actions       */
   struct db2AlertTaskAction *poTaskActions; /* Array of task actions         */
   db2Uint32                 oNumScriptActions; /* Number of script actions   */
   struct db2AlertScriptAction *poScriptActions; /* Array of script actions   */
   db2Uint32                 oDefault;      /* Indicate if setting returned   */
                                            /* is inherited from default.     */
} db2GetAlertCfgInd;

/******************************************************************************
** API for Getting Alert Configuration.
** 
** Set pParmStruct to a db2GetAlertCfgData pointer and
** always call db2GetAlertCfgFree() to free the memory
** allocated in db2GetAlertCfgData.pIndicators once
** it is no longer needed.
** 
** To get the configuration for all health indicators:
**    - Set db2GetAlertCfgData.numIndicators to 0.
**    - Set db2GetAlertCfgData.pIndicators to NULL.
** 
** To get the configuration for a list of specific health indicators:
**    - Set db2GetAlertCfgData.pIndicators to an array of
**      db2GetAlertCfgInd which have only the indicatorID field set.
**    - Set db2GetAlertCfgData.numIndicators to the length of the
**      pIndicators array.
**    In this case, the db2GetAlertCfgInd array you
**    submitted will be replaced by a new array.
*******************************************************************************/
SQL_API_RC SQL_API_FN                       /* Get Alert Cfg                  */
  db2GetAlertCfg (
   db2Uint32 versionNumber,                 /* Database version number        */
   void * pParmStruct,                      /* In/out parameters              */
   struct sqlca * pSqlca);                  /* SQLCA                          */

/******************************************************************************
** API for freeing memory returned by db2GetAlertCfg()
**  in db2GetAlertCfgData.pIndicators and its substructures.
*******************************************************************************/
SQL_API_RC SQL_API_FN                       /* Get Alert Cfg Free.            */
  db2GetAlertCfgFree (
   db2Uint32 versionNumber,                 /* Database version number        */
   void * pParmStruct,                      /* In/out parameters              */
   struct sqlca * pSqlca);                  /* SQLCA                          */


/******************************************************************************
** Structure for resetting alert configuration using the db2ResetAlertCfg API
*******************************************************************************/
typedef SQL_STRUCTURE db2ResetAlertCfgData
{
   db2Uint32                 iObjType;      /* DB2 object type                */
   char                      *piObjName;    /* Name of DB2 object             */
   char                      *piDbName;     /* Database alias                 */
   db2Uint32                 iIndicatorID;  /* Health indicator identifier.   */
                                            /* Leave 0 to reset all health    */
                                            /* indicators for the specified   */
                                            /* object.                        */
} db2ResetAlertCfgData;


/******************************************************************************
** API for Resetting Alert Configuration
*******************************************************************************/
SQL_API_RC SQL_API_FN                       /* Reset Alert Cfg                */
  db2ResetAlertCfg (
   db2Uint32 versionNumber,                 /* Database version number        */
   void * pParmStruct,                      /* In/out parameters              */
   struct sqlca * pSqlca);                  /* SQLCA                          */


/******************************************************************************
** Structure for setting a single alert attribute using db2UpdateAlertCfg API
*******************************************************************************/
typedef SQL_STRUCTURE db2AlertAttrib
{
   db2Uint32                 iAttribID;     /* Attribute identifier           */
   char                      *piAttribValue; /* Value for the attribute       */
} db2AlertAttrib;


/******************************************************************************
** Structure for deleting an alert action
*******************************************************************************/
typedef SQL_STRUCTURE db2AlertActionDelete
{
   db2Uint32                 iActionType;   /* Type of alert action           */
   char                      *piName;       /* task name or script pathname   */
   db2Uint32                 iCondition;    /* Condition associated with      */
                                            /* task or script                 */
} db2AlertActionDelete;


/******************************************************************************
** Structure for defining an alert action update
*******************************************************************************/
typedef SQL_STRUCTURE db2AlertActionUpdate
{
   db2Uint32                 iActionType;   /* Type of alert action           */
   char                      *piActionName; /* Current Task or Script name    */
                                            /* to update                      */
   db2Uint32                 iCondition;    /* Condition associated with      */
                                            /* task or script                 */
   db2Uint32                 iNumParmUpdates; /* Number of parameters to      */
                                            /* update                         */
   struct db2AlertAttrib     *piParmUpdates; /* Array of parameters to        */
                                            /* update                         */
} db2AlertActionUpdate;


/******************************************************************************
** Structure for defining a new alert action
*******************************************************************************/
typedef SQL_STRUCTURE db2AlertActionNew
{
   db2Uint32                 iActionType;   /* Type of alert action.          */
                                            /*  Only the Attribs for that     */
                                            /* type of script will be used.   */
   struct db2AlertScriptAction *piScriptAttribs; /* Attributes of script to   */
                                            /* be created.                    */
                                            /* Use when actionType is         */
                                            /* DB2ALERTCFG_ACTIONTYPE_SCRIPT  */
   struct db2AlertTaskAction *piTaskAttribs; /* Attributes of task to be      */
                                            /* created.                       */
                                            /* Use when actionType is         */
                                            /* DB2ALERTCFG_ACTIONTYPE_TASK    */
} db2AlertActionNew;


/******************************************************************************
** Structure for Updating alert configuration using the db2UpdateAlertCfg API
*******************************************************************************/
typedef SQL_STRUCTURE db2UpdateAlertCfgData
{
   db2Uint32                 iObjType;      /* DB2 object type                */
   char                      *piObjName;    /* Name of DB2 object             */
   char                      *piDbName;     /* Database alias                 */
   db2Uint32                 iIndicatorID;  /* Health indicator to update     */
   db2Uint32                 iNumIndAttribUpdates; /* ind attrib updates      */
   struct db2AlertAttrib     *piIndAttribUpdates; /* Array of updates         */
   db2Uint32                 iNumActionUpdates; /* Num actions to update      */
   struct db2AlertActionUpdate *piActionUpdates; /* action updates            */
   db2Uint32                 iNumActionDeletes; /* Num actions to delete      */
   struct db2AlertActionDelete *piActionDeletes; /* action deletes            */
   db2Uint32                 iNumNewActions; /* Number of new actions         */
   struct db2AlertActionNew  *piNewActions; /* Array of new actions           */
} db2UpdateAlertCfgData;


/******************************************************************************
** API for setting and updating Alert Configuration
*******************************************************************************/
SQL_API_RC SQL_API_FN                       /* Set Alert Cfg                  */
  db2UpdateAlertCfg (
   db2Uint32 versionNumber,                 /* Database version number        */
   void * pParmStruct,                      /* In/out parameters              */
   struct sqlca * pSqlca);                  /* SQLCA                          */





/******************************************************************************
** Health Monitor Recommendation
** Structures, Constants, and Function Prototypes
*******************************************************************************/

/******************************************************************************
** Type of DB2 objects on which health alerts can fire
** (db2GetRecommendationsData.iObjType)
*******************************************************************************/
#define DB2HEALTH_OBJTYPE_DBM           0    /* DBM health indicator          */
#define DB2HEALTH_OBJTYPE_DATABASE      1    /* DB health indicator           */
#define DB2HEALTH_OBJTYPE_TABLESPACE    2    /* TS health indicator           */
#define DB2HEALTH_OBJTYPE_TS_CONTAINER  3    /* TSC health indicator          */

/******************************************************************************
** Version ID of the schema used for health recommendation XML documents.
** (db2GetRecommendationsData.iSchemaVersion)
*******************************************************************************/
#define DB2HEALTH_RECSCHEMA_VERSION8_2  8020000 /* Health recommendation XML  */
                                             /* schema version                */

/******************************************************************************
** Structure for querying recommendations for a health indicator
*******************************************************************************/
typedef SQL_STRUCTURE db2GetRecommendationsData
{
   db2Uint32                            iSchemaVersion; /* Version ID of      */
                                             /* schema used for health        */
                                             /* recommendation XML document   */
   db2Uint32                            iNodeNumber; /* The node number       */
                                             /* where the health indicator    */
                                             /* fired                         */
   db2Uint32                            iIndicatorID; /* Health indicator     */
                                             /* identifier                    */
   db2Uint32                            iObjType; /* DB2 object type          */
   char                                 *piObjName; /* Name of DB2 object     */
   char                                 *piDbName; /* Database alias          */
   char                                 *poRecommendation; /* Health          */
                                             /* recommendation XML document   */
} db2GetRecommendationsData;

/******************************************************************************
** API for Getting Health Recommendations.
** 
** Set pParmStruct to a db2GetRecommendationsData pointer and
** always call db2GetRecommendationsFree() to free the memory
** allocated in db2GetRecommendationsData.poRecommendion once
** it is no longer needed.
** 
** To get recommendations on all nodes for a health indicator
** that has entered an alter state:
**    - Set db2GetRecommendationsData.iNodeNumber to SQLM_ALL_NODES.
** To get recommendations for a health indicator on the current node:
**    - Set db2GetRecommendationsData.iNodeNumber to SQLM_CURRENT_NODE.
*******************************************************************************/
SQL_API_RC SQL_API_FN                        /* Get Health Recommendations    */
  db2GetRecommendations (
   db2Uint32 versionNumber,                  /* Database version number       */
   void * pParmStruct,                       /* In/out parameters             */
   struct sqlca * pSqlca);                   /* SQLCA                         */

/******************************************************************************
** API for freeing memory returned by db2GetRecommendations()
**  in db2GetRecommendationsData.poRecommendation.
*******************************************************************************/
SQL_API_RC SQL_API_FN                        /* Get Health Recommendations    */
                                             /* Free.                         */
  db2GetRecommendationsFree (
   db2Uint32 versionNumber,                  /* Database version number       */
   void * pParmStruct,                       /* In/out parameters             */
   struct sqlca * pSqlca);                   /* SQLCA                         */

/******************************************************************************
** Contact Management
** Structures, Constants, and Function Prototypes
*******************************************************************************/

/******************************************************************************
**  General contact macros.
*******************************************************************************/
#define DB2CONTACT_MAX_SZ               128  /* maximum contact/contact       */
                                             /* group name size               */

/******************************************************************************
** Ways in which to be contacted (db2ContactData.type)
*******************************************************************************/
#define DB2CONTACT_EMAIL                0    /* Contact via e-mail            */
#define DB2CONTACT_PAGE                 1    /* Contact via page              */

/******************************************************************************
** Contact attributes that can be configured (db2ContactAttrib.attribID)
*******************************************************************************/
#define DB2CONTACT_ADDRESS              0    /* Contact address               */
#define DB2CONTACT_TYPE                 1    /* e-mail or page                */
#define DB2CONTACT_MAXPAGELEN           2    /* max pg len in chars           */
#define DB2CONTACT_DESCRIPTION          3    /* contact description           */

/******************************************************************************
** Can either have a single contact or a group of contacts
** used by(db2ContactTypeData.contactType)
*******************************************************************************/
#define DB2CONTACT_SINGLE               0    /* Single contact                */
#define DB2CONTACT_GROUP                1    /* Group of contacts             */


/******************************************************************************
** Structure for defining the attributes of a contact
** Used by db2GetContactsData.poContacts 
*******************************************************************************/
typedef SQL_STRUCTURE db2ContactData
{
   char                      *pName;        /* Contact name                   */
   db2Uint32                 type;          /* contact by e-mail, page        */
   char                      *pAddress;     /* e-mail or page address         */
   db2Uint32                 maxPageLength; /* Max number of characters       */
   char                      *pDescription; /* Contact description            */
} db2ContactData;

/******************************************************************************
** Structure for defining the attributes of a contact
** Used by db2AddContact.pParmStruct
*******************************************************************************/
typedef SQL_STRUCTURE db2AddContactData
{
   char                      *piUserid;     /* User ID                        */
   char                      *piPassword;   /* Password                       */
   char                      *piName;       /* Contact name                   */
   db2Uint32                 iType;         /* contact by e-mail, page        */
   char                      *piAddress;    /* e-mail or page address         */
   db2Uint32                 iMaxPageLength; /* Max number of characters      */
   char                      *piDescription; /* Contact description           */
} db2AddContactData;


/******************************************************************************
** Structure for Getting contact information using the db2GetContacts API
*******************************************************************************/
typedef SQL_STRUCTURE db2GetContactsData
{
   db2Uint32                 ioNumContacts; /* Number of contacts gotten      */
   struct db2ContactData     *poContacts;   /* Retrieved contacts             */
} db2GetContactsData;


/******************************************************************************
** API for Getting Contacts
*******************************************************************************/
SQL_API_RC SQL_API_FN                       /* Get contacts                   */
  db2GetContacts (
   db2Uint32 versionNumber,                 /* Database version number        */
   void * pParmStruct,                      /* In/out parameters              */
   struct sqlca * pSqlca);                  /* SQLCA                          */


/******************************************************************************
** Structure for specifying what contact to drop, used by 
** db2DropContact, db2DropContactGroup APIs
*******************************************************************************/
typedef SQL_STRUCTURE db2DropContactData
{
   char                      *piUserid;     /* User ID                        */
   char                      *piPassword;   /* Password                       */
   char                      *piName;       /* Contact name                   */
} db2DropContactData;


/******************************************************************************
** API for Dropping a contact
*******************************************************************************/
SQL_API_RC SQL_API_FN                       /* Drop a contact                 */
  db2DropContact (
   db2Uint32 versionNumber,                 /* Database version number        */
   void * pParmStruct,                      /* In/out parameters              */
   struct sqlca * pSqlca);                  /* SQLCA                          */


/******************************************************************************
** API for Adding a contact
*******************************************************************************/
SQL_API_RC SQL_API_FN                       /* Add a contact                  */
  db2AddContact (
   db2Uint32 versionNumber,                 /* Database version number        */
   void * pParmStruct,                      /* In/out parameters              */
   struct sqlca * pSqlca);                  /* SQLCA                          */


/******************************************************************************
** Structure for setting a single contact attribute using db2UpdateContact API
*******************************************************************************/
typedef SQL_STRUCTURE db2ContactAttrib
{
   db2Uint32                 iAttribID;     /* Attribute identifier           */
   char                      *piAttribValue; /* Value for the attribute       */
} db2ContactAttrib;


/******************************************************************************
** Structure for specifying contact info to update, used by db2UpdateContact API
*******************************************************************************/
typedef SQL_STRUCTURE db2UpdateContactData
{
   char                      *piUserid;     /* User ID                        */
   char                      *piPassword;   /* Password                       */
   char                      *piContactName; /* Contact name                  */
   db2Uint32                 iNumAttribsUpdated; /* contact attrib updates    */
   struct db2ContactAttrib   *piAttribs;    /* Attribute updates              */
} db2UpdateContactData;


/******************************************************************************
** API for updating a contact
*******************************************************************************/
SQL_API_RC SQL_API_FN                       /* Update a contact               */
  db2UpdateContact (
   db2Uint32 versionNumber,                 /* Database version number        */
   void * pParmStruct,                      /* In/out parameters              */
   struct sqlca * pSqlca);                  /* SQLCA                          */


/******************************************************************************
** Structure for specifying either a single or group contact
*******************************************************************************/
typedef SQL_STRUCTURE db2ContactTypeData
{
   db2Uint32                 contactType;   /* Contact or group               */
   char                      *pName;        /* Contact or group name          */
} db2ContactTypeData;


/******************************************************************************
** Structure for getting contact group info, used by
** db2GetContactGroup APIs
*******************************************************************************/
typedef SQL_STRUCTURE db2ContactGroupData
{
   char                      *pGroupName;   /* Contact group name             */
   char                      *pDescription; /* Contact group description      */
   db2Uint32                 numContacts;   /* Number of contacts in group    */
   struct db2ContactTypeData *pContacts;    /* Contacts in group              */
} db2ContactGroupData;

/******************************************************************************
** Structure for adding contact group info, used by
**  db2AddContactGroup APIs
*******************************************************************************/
typedef SQL_STRUCTURE db2AddContactGroupData
{
   char                      *piUserid;     /* User id                        */
   char                      *piPassword;   /* Password                       */
   char                      *piGroupName;  /* Contact group name             */
   char                      *piDescription; /* Contact group description     */
   db2Uint32                 iNumContacts;  /* Number of contacts in group    */
   struct db2ContactTypeData *piContacts;   /* Contacts in group              */
} db2AddContactGroupData;


/******************************************************************************
** API for retrieving a contact group
*******************************************************************************/
SQL_API_RC SQL_API_FN                       /* Retrieve a contact group       */
  db2GetContactGroup (
   db2Uint32 versionNumber,                 /* Database version number        */
   void * pParmStruct,                      /* In/out parameters              */
   struct sqlca * pSqlca);                  /* SQLCA                          */


/******************************************************************************
** Structure for specifying info on a contact group
*******************************************************************************/
typedef SQL_STRUCTURE db2ContactGroupDesc
{
   char                      *poName;       /* Group name                     */
   char                      *poDescription; /* Group description             */
} db2ContactGroupDesc;


/******************************************************************************
** Structure for getting contact groups, used by db2GetContactGroups API
*******************************************************************************/
typedef SQL_STRUCTURE db2GetContactGroupsData
{
   db2Uint32                 ioNumGroups;   /* Number of groups gotten        */
   struct db2ContactGroupDesc *poGroups;    /* List of groups                 */
} db2GetContactGroupsData;


/******************************************************************************
** API for retrieving contact groups
*******************************************************************************/
SQL_API_RC SQL_API_FN                       /* Retrieve contact groups        */
  db2GetContactGroups (
   db2Uint32 versionNumber,                 /* Database version number        */
   void * pParmStruct,                      /* In/out parameters              */
   struct sqlca * pSqlca);                  /* SQLCA                          */


/******************************************************************************
** API for dropping a contact group
*******************************************************************************/
SQL_API_RC SQL_API_FN                       /* Remove a contact group         */
  db2DropContactGroup (
   db2Uint32 versionNumber,                 /* Database version number        */
   void * pParmStruct,                      /* In/out parameters              */
   struct sqlca * pSqlca);                  /* SQLCA                          */


/******************************************************************************
** API for adding a new contact group
*******************************************************************************/
SQL_API_RC SQL_API_FN                       /* Add a new contact group        */
  db2AddContactGroup (
   db2Uint32 versionNumber,                 /* Database version number        */
   void * pParmStruct,                      /* In/out parameters              */
   struct sqlca * pSqlca);                  /* SQLCA                          */


/******************************************************************************
** Structure for specifying updates to a contact group, used by db2UpdateContactGroup API
*******************************************************************************/
typedef SQL_STRUCTURE db2UpdateContactGroupData
{
   char                      *piUserid;     /* User ID                        */
   char                      *piPassword;   /* Password                       */
   char                      *piGroupName;  /* Group name                     */
   db2Uint32                 iNumNewContacts; /* Number of new contacts       */
   struct db2ContactTypeData *piNewContacts; /* Contacts to be added          */
   db2Uint32                 iNumDroppedContacts; /* Number of contacts       */
                                            /* dropped                        */
   struct db2ContactTypeData *piDroppedContacts; /* Contacts to be dropped    */
   char                      *piNewDescription; /* New group description      */
} db2UpdateContactGroupData;


/******************************************************************************
** API for updating a contact group
*******************************************************************************/
SQL_API_RC SQL_API_FN                       /* Update a contact group         */
  db2UpdateContactGroup (
   db2Uint32 versionNumber,                 /* Database version number        */
   void * pParmStruct,                      /* In/out parameters              */
   struct sqlca * pSqlca);                  /* SQLCA                          */



/******************************************************************************
** Health Notification Contact List Management
** Structures, Constants, and Function Prototypes
*******************************************************************************/

/******************************************************************************
** Types of notification contact list updates
** (db2HealthNotificationListUpdate.updateType)
*******************************************************************************/
#define DB2HEALTHNOTIFICATIONLIST_ADD   0    /* add a contact                 */
#define DB2HEALTHNOTIFICATIONLIST_DROP  1    /* drop a contact                */


/******************************************************************************
** Structure for retrieving notification contact list, used
** by db2GetHealthNotificationList API
*******************************************************************************/
typedef SQL_STRUCTURE db2GetHealthNotificationListData
{
   db2Uint32                 ioNumContacts; /* Number of contacts in list     */
   struct db2ContactTypeData *poContacts;   /* Contacts in notification list  */
} db2GetHealthNotificationListData;


/******************************************************************************
** API for retrieving health notification contact list
** 
** The parameter pParmStruct accepts a pointer to a
** db2GetHealthNotificationListData structure which must contain
** a sufficiently large array of db2ContactTypeData.
** Every db2ContactTypeData in the array must have a pre-allocated
** pName buffer of size DB2CONTACT_MAX_SZ+1. If the array specified
** in poContacts is too small then an error will be returned with
** the required array length returned in oNumContacts. Call with
** poContacts = NULL and oNumContacts = 0 to receive the current
** number of db2ContactTypeData to submit.
*******************************************************************************/
SQL_API_RC SQL_API_FN                       /* Retrieve Notification List     */
  db2GetHealthNotificationList (
   db2Uint32 versionNumber,                 /* Database version number        */
   void * pParmStruct,                      /* In/out parameters              */
   struct sqlca * pSqlca);                  /* SQLCA                          */


/******************************************************************************
** Structure for specifying an update to the notification contact list
*******************************************************************************/
typedef SQL_STRUCTURE db2HealthNotificationListUpdate
{
   db2Uint32                 iUpdateType;   /* add or drop                    */
   struct db2ContactTypeData *piContact;    /* Contact to add/drop            */
} db2HealthNotificationListUpdate;


/******************************************************************************
** Structure specifying updates to the health notification contact list,
** Used by db2UpdateHealthNotificationList API
*******************************************************************************/
typedef SQL_STRUCTURE db2UpdateHealthNotificationListData
{
   db2Uint32                 iNumUpdates;   /* Number of list updates         */
   struct db2HealthNotificationListUpdate *piUpdates; /* List updates         */
} db2UpdateHealthNotificationListData;


/******************************************************************************
** API for updating health notification contact list.
** 
** The parameter pParmStruct accepts a pointer to a 
** db2UpdateHealthNotificationListData which in turn must contain an
** array of db2HealthNotificationListUpdate with the update information.
*******************************************************************************/
SQL_API_RC SQL_API_FN                       /* Updates Notification List      */
  db2UpdateHealthNotificationList (
   db2Uint32 versionNumber,                 /* Database version number        */
   void * pParmStruct,                      /* In/out parameters              */
   struct sqlca * pSqlca);                  /* SQLCA                          */

/* Get Node Number Interface Structure                                        */
typedef SQL_STRUCTURE db2GetNodeInfoData
{
   db2int32                  *poMyNodeNumber; /* Pointer to output node       */
                                            /* number                         */
   db2int32                  *poCurNumNodes; /* Pointer to number of nodes    */
} db2GetNodeInfoData;

SQL_API_RC SQL_API_FN                       /* Get node information           */
  db2GetNodeInfo (
   db2Uint32 versionNumber,                 /* Database version number        */
   void * pParmStruct,                      /* In/out parameters              */
   struct sqlca * pSqlca);                  /* SQLCA                          */

/******************************************************************************
** Update Alternate Server Parameter Structure 
*******************************************************************************/
typedef SQL_STRUCTURE db2UpdateAltServerStruct
{
   char                      *piDbAlias;    /* Input: database alias          */
   char                      *piHostName;   /* Input: host name               */
   char                      *piPort;       /* Input: port number             */
} db2UpdateAltServerStruct;

/******************************************************************************
** Update Alternate Server API 
*******************************************************************************/
SQL_API_RC SQL_API_FN                  /* Update Alternate Server             */
  db2UpdateAlternateServerForDB (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* In/out parameters                   */
       struct sqlca * pSqlca);         /* SQLCA                               */

/******************************************************************************
** Generic Update Alternate Server Structure 
*******************************************************************************/
typedef SQL_STRUCTURE db2gUpdateAltServerStruct
{
   db2Uint32                 iDbAlias_len;  /* Input: database alias length   */
   char                      *piDbAlias;    /* Input: database alias          */
   db2Uint32                 iHostName_len; /* Input: host name length        */
   char                      *piHostName;   /* Input: host name               */
   db2Uint32                 iPort_len;     /* Input: port number length      */
   char                      *piPort;       /* Input: port number             */
} db2gUpdateAltServerStruct;

/******************************************************************************
** Generic Update Alternate Server API 
*******************************************************************************/
SQL_API_RC SQL_API_FN                  /* Generic Update Alternate Server     */
  db2gUpdateAlternateServerForDB (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* In/out parameters                   */
       struct sqlca * pSqlca);         /* SQLCA                               */

/******************************************************************************
** Open Database Directory Scan Parameter Structure 
*******************************************************************************/
typedef SQL_STRUCTURE db2DbDirOpenScanStruct
{
   char                      *piPath;       /* Input: drive/path              */
   db2Uint16                 oHandle;       /* Output: handle                 */
   db2Uint16                 oNumEntries;   /* Output: count                  */
} db2DbDirOpenScanStruct;

/******************************************************************************
** Open Database Directory Scan API 
*******************************************************************************/
SQL_API_RC SQL_API_FN                  /* Open Database Directory Scan        */
  db2DbDirOpenScan (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* In/out parameters                   */
       struct sqlca * pSqlca);         /* SQLCA                               */

/******************************************************************************
** Generic Open Database Directory Scan Parameter Structure 
*******************************************************************************/
typedef SQL_STRUCTURE db2gDbDirOpenScanStruct
{
   db2Uint32                 iPath_len;     /* Input: drive/path length       */
   char                      *piPath;       /* Input: drive/path              */
   db2Uint16                 oHandle;       /* Output: handle                 */
   db2Uint16                 oNumEntries;   /* Output: count                  */
} db2gDbDirOpenScanStruct;

/******************************************************************************
** Generic Open Database Directory Scan API 
*******************************************************************************/
SQL_API_RC SQL_API_FN                  /* Generic Open Database Directory     */
                                       /* Scan                                */
  db2gDbDirOpenScan (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* In/out parameters                   */
       struct sqlca * pSqlca);         /* SQLCA                               */

/******************************************************************************
** Get Next Database Directory Entry Data Structure 
*******************************************************************************/
SQL_STRUCTURE db2DbDirInfo
{
   _SQLOLDCHAR               alias[SQL_ALIAS_SZ]; /* Alias name               */
   _SQLOLDCHAR               dbname[SQL_DBNAME_SZ]; /* Database name          */
   _SQLOLDCHAR               drive[SQL_DRIVE_SZ]; /* Drive/Path               */
   _SQLOLDCHAR               intname[SQL_INAME_SZ]; /* Database subdirectory  */
   _SQLOLDCHAR               nodename[SQL_NNAME_SZ]; /* Node name             */
   _SQLOLDCHAR               dbtype[SQL_DBTYP_SZ]; /* Release information     */
   _SQLOLDCHAR               comment[SQL_CMT_SZ]; /* Comment                  */
   short                     com_codepage;  /* Code page of comment           */
   _SQLOLDCHAR               type;          /* Entry type - defines above     */
   unsigned short            authentication; /* Authentication type           */
   char                      glbdbname[SQL_DIR_NAME_SZ]; /* Global database   */
                                            /* name                           */
   _SQLOLDCHAR               dceprincipal[SQL_DCEPRIN_SZ]; /* dce principal   */
                                            /* bin string                     */
   short                     cat_nodenum;   /* Catalog node number            */
   short                     nodenum;       /* Node number                    */
   _SQLOLDCHAR               althostname[SQL_HOSTNAME_SZ]; /* Alternate       */
                                            /* hostname                       */
   _SQLOLDCHAR               altportnumber[SQL_SERVICE_NAME_SZ]; /* Port      */
                                            /* number                         */
};

/******************************************************************************
** Get Next Database Directory Entry Parameter Structure 
*******************************************************************************/
typedef SQL_STRUCTURE db2DbDirNextEntryStruct
{
   db2Uint16                 iHandle;       /* Input: handle                  */
   struct db2DbDirInfo       *poDbDirEntry; /* Output: buffer                 */
} db2DbDirNextEntryStruct;

/******************************************************************************
** Get Next Database Directory Entry API 
*******************************************************************************/
SQL_API_RC SQL_API_FN                  /* Get Next Database Directory Entry   */
  db2DbDirGetNextEntry (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* In/out parameters                   */
       struct sqlca * pSqlca);         /* SQLCA                               */

/******************************************************************************
** Generic Get Next Database Directory Entry Parameter Structure 
*******************************************************************************/
typedef SQL_STRUCTURE db2gDbDirNextEntryStruct
{
   db2Uint16                 iHandle;       /* Input: handle                  */
   struct db2DbDirInfo       *poDbDirEntry; /* Output: buffer                 */
} db2gDbDirNextEntryStruct;

/******************************************************************************
** Generic Get Next Database Directory Entry API 
*******************************************************************************/
SQL_API_RC SQL_API_FN                  /* Generic Get Next Database           */
                                       /* Directory Entry                     */
  db2gDbDirGetNextEntry (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* In/out parameters                   */
       struct sqlca * pSqlca);         /* SQLCA                               */

/******************************************************************************
** Close Database Directory Scan Parameter Structure 
*******************************************************************************/
typedef SQL_STRUCTURE db2DbDirCloseScanStruct
{
   db2Uint16                 iHandle;       /* Input: handle                  */
} db2DbDirCloseScanStruct;

/******************************************************************************
** Close Database Directory Scan API 
*******************************************************************************/
SQL_API_RC SQL_API_FN                  /* Close Database Directory Scan       */
  db2DbDirCloseScan (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* In/out parameters                   */
       struct sqlca * pSqlca);         /* SQLCA                               */

/******************************************************************************
** Generic Close Database Directory Scan Parameter Structure 
*******************************************************************************/
typedef SQL_STRUCTURE db2gDbDirCloseScanStruct
{
   db2Uint16                 iHandle;       /* Input: handle                  */
} db2gDbDirCloseScanStruct;

/******************************************************************************
** Generic Close Database Directory Scan API 
*******************************************************************************/
SQL_API_RC SQL_API_FN                  /* Generic Close Database Directory    */
                                       /* Scan                                */
  db2gDbDirCloseScan (
       db2Uint32 versionNumber,        /* Database version number             */
       void * pParmStruct,             /* In/out parameters                   */
       struct sqlca * pSqlca);         /* SQLCA                               */

/******************************************************************************
** db2QpGetUserInformation
*******************************************************************************/

/* db2QpGetUserInfo Structure                                                 */
typedef SQL_STRUCTURE db2QpGetUserInfoStruct
{
  char              *poReplyBuffer;   /* Output:Information buffer provided   */
                                      /* by caller                            */
  db2Uint32         ioReplyBufSize;   /* Input/Output: Reply buffer size      */
                                      /* Filled buffer size                   */
  db2Uint32         oStatus;          /* Output: Status of QP call            */
} db2QpGetUserInfoStruct;


/* db2QpGetUserInformation API                                                */
SQL_API_RC SQL_API_FN                        /* Get QP user information       */
  db2QpGetUserInformation (
             db2Uint32 versionNumber,        /* Version Number                */
             void * pParmStruct,             /* db2QpGetUserInfoStruct to be  */
                                             /* returned                      */
             struct sqlca * pSqlca);         /* SQLCA                         */


#pragma pack()

#ifdef __cplusplus 
}
#endif

#endif /* SQL_H_DB2APIDF */
