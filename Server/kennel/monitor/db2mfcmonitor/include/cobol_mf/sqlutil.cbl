      *************************************************************************
      * 
      * Source File Name: SQLUTIL
      * 
      * (C) COPYRIGHT International Business Machines Corp. 1987, 2004
      * All Rights Reserved
      * Licensed Materials - Property of IBM
      * 
      * US Government Users Restricted Rights - Use, duplication or
      * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
      * 
      * Function = Include File defining:
      *              Utility / Configuration Commands - Constants
      *              Utility / Configuration Commands - Data Structures
      *              Utility / Configuration Commands - Function Prototypes
      *              Utility / Configuration Commands - Labels for SQLCODES
      * 
      * Operating System: Windows NT
      * 
      **************************************************************************
      * Interface Parameters/Structures/Return Codes

      * Parameters for Rollforward caller action
      * Roll forward to requested point
       78  SQLUM-ROLLFWD             VALUE    1.
      * End roll-forward recovery
       78  SQLUM-STOP                VALUE    2.
      * End roll-forward recovery
       78  SQLUM-COMPLETE            VALUE    2.
      * Roll forward to requested point and end roll-forward recovery
       78  SQLUM-ROLLFWD-STOP        VALUE    3.
      * Roll forward to requested point and end roll-forward recovery
       78  SQLUM-ROLLFWD-COMPLETE    VALUE    3.
      * Query nextArcFile, firstDeleteArc, lastDeleteArc, lastCommitTime
       78  SQLUM-QUERY               VALUE    4.
      * Cancel current rollforward
       78  SQLUM-CANCEL              VALUE    8.
      * Parameter check
       78  SQLUM-PARM-CHECK          VALUE    10.
      * Continue load recovery.
       78  SQLUM-LOADREC-CONTINUE    VALUE    11.
      * Terminate load recovery.
       78  SQLUM-LOADREC-TERMINATE   VALUE    12.
      * Terminate device.Caller action codes 31 - 50 reserved for internal
      * use.
       78  SQLUM-LOADREC-DEVICE-TERMINATE VALUE 13.

      * Constants used for Rollforward
      * Roll forward until STOP specified
       78  SQLUM-INFINITY-TIMESTAMP  value    "9999-12-31-23.59.59.99999
      -    "9".
      * Length of Archive File name
       78  SQLUM-ARCHIVE-FILE-LEN    VALUE    12.
      * Length of ISO format Timestamp
       78  SQLUM-TIMESTAMP-LEN       VALUE    26.

      * Roll forward input version values
      * current rollforward input version
       78  SQLUM-RFWD-VERSION        VALUE    5.

      * Roll forward allNodeFlag values
      * submit to all nodes in the node list
       78  SQLURF-NODE-LIST          VALUE    0.
      * submit to all nodes in the nodeconfiguration file
       78  SQLURF-ALL-NODES          VALUE    1.
      * submit to all nodes except the onesspecified by the nodelist
      * parameter
       78  SQLURF-ALL-EXCEPT         VALUE    2.
      * submit to catalog node only
       78  SQLURF-CAT-NODE-ONLY      VALUE    3.

      * Roll forward query status codes
      * cannot query node
       78  SQLURFQ-NOT-AVAILABLE     VALUE    0.
      * rollforward is not pending
       78  SQLURFQ-NOT-RFW-PENDING   VALUE    1.
      * database rollforward pending
       78  SQLURFQ-DB-RFW-PENDING    VALUE    2.
      * tablespace rollforward pending
       78  SQLURFQ-TBL-RFW-PENDING   VALUE    3.
      * database rollforward in progress
       78  SQLURFQ-DB-RFW-IN-PROGRESS VALUE   4.
      * tablespace rollforward in progress
       78  SQLURFQ-TBL-RFW-IN-PROGRESS VALUE  5.
      * database rollforward processing STOP
       78  SQLURFQ-DB-RFW-STOPPING   VALUE    6.
      * tablespace rollforward processing STOP
       78  SQLURFQ-TBL-RFW-STOPPING  VALUE    7.

      * Rollforward connect mode
      * Offline rollforward mode
       78  SQLUM-OFFLINE             VALUE    0.
      * Online rollforward mode
       78  SQLUM-ONLINE              VALUE    1.

      * Backup mode
      * Offline backup mode
       78  SQLUB-OFFLINE             VALUE    0.
      * Online backup mode
       78  SQLUB-ONLINE              VALUE    1.
      * Split mirror backup
       78  SQLUB-SPLITMIRROR         VALUE    16.
      * Backup modes
       78  SQLUB-MODE-MASK           VALUE    15.
      * Backup options
       78  SQLUB-OPT-MASK            VALUE    0.

      * Backup type
       78  SQLUB-DB                  VALUE    0.
       78  SQLUB-FULL                VALUE    0.
       78  SQLUB-TABLESPACE          VALUE    3.
       78  SQLUB-INCREMENTAL         VALUE    16.
       78  SQLUB-DELTA               VALUE    48.
       78  SQLUB-INC-LEVEL-MASK      VALUE    65280.

      * Backup type masks
      * Determines if full/tbsp bckp.
       78  SQLUB-TYPE-MASK           VALUE    15.
      * Determines if incr/delta bckp.
       78  SQLUB-INCR-MASK           VALUE    240.

      * Backup calling action values
      * initial call
       78  SQLUB-BACKUP              VALUE    0.
      * Subsequent call
       78  SQLUB-CONTINUE            VALUE    1.
      * Terminating call
       78  SQLUB-TERMINATE           VALUE    2.
      * Initial call to run without prompting
       78  SQLUB-NOINTERRUPT         VALUE    3.
      * End media device.
       78  SQLUB-DEVICE-TERMINATE    VALUE    9.
      * for parameter check and initialization.
       78  SQLUB-PARM-CHECK          VALUE    10.
      * for parameter check and initialization.
       78  SQLUB-PARM-CHECK-INIT-AND-WAIT VALUE 10.
      * for parameter checking only.
       78  SQLUB-PARM-CHECK-ONLY     VALUE    11.

      * Restore rollforward mode
       78  SQLUD-ROLLFWD             VALUE    0.
       78  SQLUD-NOROLLFWD           VALUE    1.

      * Restore mode
      * Offline restore mode
       78  SQLUD-OFFLINE             VALUE    0.
      * Online restore mode
       78  SQLUD-ONLINE              VALUE    1.

      * Datalink mode
       78  SQLUD-DATALINK            VALUE    0.
       78  SQLUD-NODATALINK          VALUE    1.

      * Restore calling action values
      * initial call
       78  SQLUD-RESTORE             VALUE    0.
      * Subsequent call
       78  SQLUD-CONTINUE            VALUE    1.
      * Terminating call
       78  SQLUD-TERMINATE           VALUE    2.
      * Initial call to run without prompting
       78  SQLUD-NOINTERRUPT         VALUE    3.
      * End media device.
       78  SQLUD-DEVICE-TERMINATE    VALUE    9.
      * for parameter check and initialization.
       78  SQLUD-PARM-CHECK          VALUE    10.
      * for parameter check and initialization.
       78  SQLUD-PARM-CHECK-INIT-AND-WAIT VALUE 10.
      * for parameter checking only.
       78  SQLUD-PARM-CHECK-ONLY     VALUE    11.
      * list of tablespaces/containers.
       78  SQLUD-RESTORE-TABLESPACE-LIST VALUE 12.
      * abort incremental restore
       78  SQLUD-TERMINATE-INCREMENTAL VALUE  13.
      * initial call, tablespace container redefinition requested
       78  SQLUD-RESTORE-STORDEF     VALUE    100.
      * initial call, tablespace container redefinition requested without
      * prompting
       78  SQLUD-STORDEF-NOINTERRUPT VALUE    101.

      * Restore type
       78  SQLUD-DB                  VALUE    0.
       78  SQLUD-FULL                VALUE    0.
       78  SQLUD-ONLINE-TABLESPACE   VALUE    3.
       78  SQLUD-TABLESPACE          VALUE    4.
       78  SQLUD-HISTORY             VALUE    5.
       78  SQLUD-INCREMENTAL         VALUE    16.
       78  SQLUD-AUTOMATIC           VALUE    256.

      * The following are not supported in V2.1.0
       78  SQLUD-TABLESPACE-TBL      VALUE    6.
       78  SQLUD-HIST-TBLSPACE-TBL   VALUE    7.

      * Restore type masks
      * Determines if full/tbsp restore
       78  SQLUD-TYPE-MASK           VALUE    15.
      * Determines if incr. restore
       78  SQLUD-INCR-MASK           VALUE    240.

      * Backup/Restore Definitions
      * length of database alias name
       78  SQLU-ALIAS-SZ             VALUE    8.
      * Maximum length of userid
       78  SQLU-USERID-LEN           VALUE    1024.
      * length of password
       78  SQLU-PWD-LEN              VALUE    18.
      * length of the backup timestamp in yyyymmddhhmmss format
       78  SQLU-TIME-STAMP-LEN       VALUE    14.

      * length of filename of backup image:
      * dbalias.type.db2instance.node.timestamp.seq
       78  SQLU-BACKUP-NAME-SZ       value    44.

      * length of directory where backup file will reside:dir - '/' -
      * filename
       78  SQLU-BACKUP-DIR-SZ        value    978.

      * Data map separator for LOAD utility;  Partition Key Identifier
       78  SQL-HEADER-DATA-SEPARATOR value    "===PDB===".
       78  SQLU-PARTITIONKEY         value    "Key:".

      * Parameters for Backup/Restore old api's
      * User exit used, not SD interface
       78  SQLU-NSD-ID               value    "0".

      * Parameters for SQLUHPRN force option
       78  SQLUH-NO-FORCE            VALUE    0.
       78  SQLUH-FORCE               VALUE    1.

      * Parameters for SQLUHOPS caller action
       78  SQLUH-LIST-BACKUP         VALUE    0.
       78  SQLUH-LIST-HISTORY        VALUE    1.
       78  SQLUH-LIST-ADM-BACKUP     VALUE    2.
       78  SQLUH-LIST-ADM-ROLLFORWARD VALUE   3.
       78  SQLUH-LIST-ADM-RUNSTATS   VALUE    4.
       78  SQLUH-LIST-ADM-REORG      VALUE    5.
       78  SQLUH-LIST-ADM-ALT-TABLESPACE VALUE 6.
       78  SQLUH-LIST-ADM-DROPPED-TABLE VALUE 7.
       78  SQLUH-LIST-ADM-LOAD       VALUE    8.
       78  SQLUH-LIST-ADM-HISTORY    VALUE    9.

      * Parameters for SQLUHGNE caller action
       78  SQLUH-GET-NEXT-ENTRY      VALUE    0.
       78  SQLUH-GET-DDL             VALUE    1.
       78  SQLUH-GET-NEXT-ENTRY-DDL  VALUE    2.

      * History File structure sizes
       78  SQLUH-OP-SZ               VALUE    1.
       78  SQLUH-OBJ-SZ              VALUE    1.
       78  SQLUH-OBJPART-SZ          VALUE    17.
       78  SQLUH-OPTYPE-SZ           VALUE    1.
       78  SQLUH-DEVTYPE-SZ          VALUE    1.
       78  SQLUH-FIRSTLOG-SZ         VALUE    12.
       78  SQLUH-LASTLOG-SZ          VALUE    12.
       78  SQLUH-BACKID-SZ           VALUE    14.
       78  SQLUH-TCREATE-SZ          VALUE    8.
       78  SQLUH-TNAME-SZ            VALUE    18.
       78  SQLUH-NUMTABLESPACE-SZ    VALUE    5.
       78  SQLUH-LOC-SZ              VALUE    255.
       78  SQLUH-COMMENT-SZ          VALUE    30.
       78  SQLUH-TABLESPACENAME-SZ   VALUE    18.
       78  SQLUH-TIMESTAMP-SZ        VALUE    14.
       78  SQLUH-ID-SZ               VALUE    24.

      * Parameters for RUNSTATS
      * TABLEOPT = Table w/o Indexes
       78  SQL-STATS-TABLE           value    "T".
      * TABLEOPT = Table and Indexes
       78  SQL-STATS-BOTH            value    "B".
      * TABLEOPT = Indexes w/o Table
       78  SQL-STATS-INDEX           value    "I".
      * TABLEOPT = Table and dist stats
       78  SQL-STATS-EXTTABLE-ONLY   value    "D".
      * TABLEOPT = Table and dist stats and basic indexes
       78  SQL-STATS-EXTTABLE-INDEX  value    "E".
      * TABLEOPT = Ext stats for indexes only
       78  SQL-STATS-EXTINDEX-ONLY   value    "X".
      * TABLEOPT = Ext stats for indexes and basic table stats
       78  SQL-STATS-EXTINDEX-TABLE  value    "Y".
      * TABLEOPT = Ext stats for indexes and table with dist stats
       78  SQL-STATS-ALL             value    "A".
      * SHAREOPT = Reference
       78  SQL-STATS-REF             value    "R".
      * TABLEOPT - statistics are to be gathered using the profile - Load
      * only
       78  SQLU-STATS-USE-PROFILE    value    "P".
      * SHAREOPT = Change
       78  SQL-STATS-CHG             value    "C".

      * TABLEOPT - no statistics are to be gathered - Load only
       78  SQLU-STATS-NONE           value    " ".

      * Parameters for selection method in sqldcol
      * Names
       78  SQL-METH-N                VALUE    78.
      * Positional
       78  SQL-METH-P                VALUE    80.
      * Default
       78  SQL-METH-D                VALUE    68.
      * Locations
       78  SQL-METH-L                VALUE    76.

      * File Types for Import/Export
       78  SQL-IXF                   value    "IXF".
       78  SQL-WSF                   value    "WSF".
       78  SQL-DEL                   value    "DEL".
       78  SQL-ASC                   value    "ASC".
       78  SQL-CURSOR                value    "CURSOR".
      * load/unload
       78  SQL-DB2CS                 value    "DB2CS".

      * Import/Export/Load/Unload calling action values
      * Initial call
       78  SQLU-INITIAL              VALUE    0.
      * Subsequent call
       78  SQLU-CONTINUE             VALUE    1.
      * Terminating call
       78  SQLU-TERMINATE            VALUE    2.
      * Initial call - Load/Unload only
       78  SQLU-NOINTERRUPT          VALUE    3.
      * Abort call - Load only
       78  SQLU-ABORT                VALUE    4.
      * Restart call - Load only
       78  SQLU-RESTART              VALUE    5.
      * Subsequent call - Load/Unload only
       78  SQLU-DEVICE-TERMINATE     VALUE    9.

      * Actions for quiesce tablespaces for table
       78  SQLU-QUIESCEMODE-SHARE    VALUE    0.
       78  SQLU-QUIESCEMODE-INTENT-UPDATE VALUE 1.
       78  SQLU-QUIESCEMODE-EXCLUSIVE VALUE   2.
       78  SQLU-QUIESCEMODE-RESET    VALUE    9.
       78  SQLU-QUIESCEMODE-RESET-OWNED VALUE 10.

      *************************************************************************
      * Equates for Configuration Utilities
      **************************************************************************

      * Token equates for SQLFupd structure for Database Manager config file
       78  SQLF-KTN-RQRIOBLK         VALUE    1.
       78  SQLF-KTN-NUMDB            VALUE    6.
       78  SQLF-KTN-NNAME            VALUE    7.
       78  SQLF-KTN-MAXAGENTS        VALUE    12.
       78  SQLF-KTN-MAXCAGENTS       VALUE    13.
       78  SQLF-KTN-ASLHEAPSZ        VALUE    15.
       78  SQLF-KTN-BACKBUFSZ        VALUE    18.
       78  SQLF-KTN-RESTBUFSZ        VALUE    19.
       78  SQLF-KTN-INDEXREC         VALUE    20.
       78  SQLF-KTN-SHEAPTHRES       VALUE    21.
       78  SQLF-KTN-MAXDARI          VALUE    80.
       78  SQLF-KTN-FENCED-POOL      VALUE    80.
       78  SQLF-KTN-KEEPDARI         VALUE    81.
       78  SQLF-KTN-KEEPFENCED       VALUE    81.
       78  SQLF-KTN-SVCENAME         VALUE    24.
       78  SQLF-KTN-TPNAME           VALUE    25.
       78  SQLF-KTN-AGENTPRI         VALUE    26.
       78  SQLF-KTN-DFTDBPATH        VALUE    27.
      * You may manipulate the bits of SQLF_KTN_DFT_MONSWITCHES or you can
      * enable disable, or query the current setting through the individual
      * tokens
       78  SQLF-KTN-DFT-MONSWITCHES  VALUE    29.
      * Bit 1 of SQLF_KTN_DFT_MONSWITCHES
       78  SQLF-KTN-DFT-MON-UOW      VALUE    30.
      * Bit 2 of SQLF_KTN_DFT_MONSWITCHES
       78  SQLF-KTN-DFT-MON-STMT     VALUE    31.
      * Bit 3 of SQLF_KTN_DFT_MONSWITCHES
       78  SQLF-KTN-DFT-MON-TABLE    VALUE    32.
      * Bit 4 of SQLF_KTN_DFT_MONSWITCHES
       78  SQLF-KTN-DFT-MON-BUFPOOL  VALUE    33.
      * Bit 5 of SQLF_KTN_DFT_MONSWITCHES
       78  SQLF-KTN-DFT-MON-LOCK     VALUE    34.
      * Bit 6 of SQLF_KTN_DFT_MONSWITCHES
       78  SQLF-KTN-DFT-MON-SORT     VALUE    35.
      * Bit 7 of SQLF_KTN_DFT_MONSWITCHES
       78  SQLF-KTN-DFT-MON-TIMESTAMP VALUE   36.
       78  SQLF-KTN-DFT-ACCOUNT-STR  VALUE    28.
       78  SQLF-KTN-SYSADM-GROUP     VALUE    39.
       78  SQLF-KTN-DIR-CACHE        VALUE    40.
       78  SQLF-KTN-DRDA-HEAP-SZ     VALUE    41.
       78  SQLF-KTN-CPUSPEED         VALUE    42.
       78  SQLF-KTN-MIN-PRIV-MEM     VALUE    43.
       78  SQLF-KTN-PRIV-MEM-THRESH  VALUE    44.
       78  SQLF-KTN-MAXTOTFILOP      VALUE    45.
       78  SQLF-KTN-FILESERVER       VALUE    47.
       78  SQLF-KTN-OBJECTNAME       VALUE    48.
       78  SQLF-KTN-QUERY-HEAP-SZ    VALUE    49.
       78  SQLF-KTN-AGENT-STACK-SZ   VALUE    61.
       78  SQLF-KTN-SYSMAINT-GROUP   VALUE    62.
       78  SQLF-KTN-SYSCTRL-GROUP    VALUE    63.
       78  SQLF-KTN-DIAGLEVEL        VALUE    64.
       78  SQLF-KTN-DIAGPATH         VALUE    65.
       78  SQLF-KTN-TP-MON-NAME      VALUE    66.
       78  SQLF-KTN-TM-DATABASE      VALUE    67.
       78  SQLF-KTN-RESYNC-INTERVAL  VALUE    68.
       78  SQLF-KTN-UDF-MEM-SZ       VALUE    69.
       78  SQLF-KTN-IPX-SOCKET       VALUE    71.
       78  SQLF-KTN-DOS-RQRIOBLK     VALUE    72.
       78  SQLF-KTN-DIR-TYPE         VALUE    73.
       78  SQLF-KTN-DIR-PATH-NAME    VALUE    74.
       78  SQLF-KTN-DIR-OBJ-NAME     VALUE    75.
       78  SQLF-KTN-ROUTE-OBJ-NAME   VALUE    76.
       78  SQLF-KTN-DFT-CLIENT-COMM  VALUE    77.
       78  SQLF-KTN-DFT-CLIENT-ADPT  VALUE    82.
       78  SQLF-KTN-AUTHENTICATION   VALUE    78.
       78  SQLF-KTN-MON-HEAP-SZ      VALUE    79.
       78  SQLF-KTN-SPM-LOG-FILE-SZ  VALUE    90.
       78  SQLF-KTN-SPM-MAX-RESYNC   VALUE    91.
       78  SQLF-KTN-SPM-NAME         VALUE    92.
       78  SQLF-KTN-NUM-INITAGENTS   VALUE    500.
       78  SQLF-KTN-MAX-COORDAGENTS  VALUE    501.
       78  SQLF-KTN-NUM-POOLAGENTS   VALUE    502.
       78  SQLF-KTN-FCM-NUM-BUFFERS  VALUE    503.
       78  SQLF-KTN-FCM-NUM-RQB      VALUE    504.
       78  SQLF-KTN-FCM-NUM-CONNECT  VALUE    505.
       78  SQLF-KTN-FCM-NUM-ANCHORS  VALUE    506.
       78  SQLF-KTN-CONN-ELAPSE      VALUE    508.
       78  SQLF-KTN-MAX-CONNRETRIES  VALUE    509.
       78  SQLF-KTN-MAX-TIME-DIFF    VALUE    510.
       78  SQLF-KTN-START-STOP-TIME  VALUE    511.
       78  SQLF-KTN-TRUST-ALLCLNTS   VALUE    301.
       78  SQLF-KTN-TRUST-CLNTAUTH   VALUE    302.
       78  SQLF-KTN-MAX-QUERYDEGREE  VALUE    303.
       78  SQLF-KTN-DISCOVER         VALUE    304.
       78  SQLF-KTN-DISCOVER-COMM    VALUE    305.
       78  SQLF-KTN-INTRA-PARALLEL   VALUE    306.
       78  SQLF-KTN-COMM-BANDWIDTH   VALUE    307.
       78  SQLF-KTN-DISCOVER-INST    VALUE    308.
       78  SQLF-KTN-SS-LOGON         VALUE    309.
       78  SQLF-KTN-JAVA-HEAP-SZ     VALUE    310.
       78  SQLF-KTN-JDK11-PATH       VALUE    311.
       78  SQLF-KTN-JDK-PATH         VALUE    311.
       78  SQLF-KTN-AUDIT-BUF-SZ     VALUE    312.
       78  SQLF-KTN-SPM-LOG-PATH     VALUE    313.
       78  SQLF-KTN-CATALOG-NOAUTH   VALUE    314.
       78  SQLF-KTN-NUM-INITDARIS    VALUE    601.
       78  SQLF-KTN-NUM-INITFENCED   VALUE    601.
       78  SQLF-KTN-INITDARI-JVM     VALUE    602.
       78  SQLF-KTN-INITFENCED-JVM   VALUE    602.
       78  SQLF-KTN-DATALINKS        VALUE    603.
       78  SQLF-KTN-FEDERATED        VALUE    604.
       78  SQLF-KTN-NOTIFYLEVEL      VALUE    605.
       78  SQLF-KTN-MAX-LOGICAGENTS  VALUE    701.
       78  SQLF-KTN-DF-ENCRYPT-LIST  VALUE    801.
       78  SQLF-KTN-MAX-CONNECTIONS  VALUE    802.
       78  SQLF-KTN-INSTANCE-MEMORY  VALUE    803.
       78  SQLF-KTN-HEALTH-MON       VALUE    804.
       78  SQLF-KTN-USE-SNA-AUTH     VALUE    805.
       78  SQLF-KTN-FED-NOAUTH       VALUE    806.
       78  SQLF-KTN-UTIL-IMPACT-LIM  VALUE    807.
       78  SQLF-KTN-SRV-PLUGIN-MODE  VALUE    809.
       78  SQLF-KTN-GROUP-PLUGIN     VALUE    810.
       78  SQLF-KTN-CLNT-PW-PLUGIN   VALUE    811.
       78  SQLF-KTN-CLNT-KRB-PLUGIN  VALUE    812.
       78  SQLF-KTN-SRVCON-PW-PLUGIN VALUE    813.
       78  SQLF-KTN-SRVCON-GSSPLUGIN-LIST VALUE 814.
       78  SQLF-KTN-SRVCON-AUTH      VALUE    815.
       78  SQLF-KTN-LOCAL-GSSPLUGIN  VALUE    816.
       78  SQLF-KTN-SYSMON-GROUP     VALUE    808.
       78  SQLF-KTN-SYSADM-GRP       VALUE    844.
       78  SQLF-KTN-SYSMAINT-GRP     VALUE    845.
       78  SQLF-KTN-SYSCTRL-GRP      VALUE    846.
       78  SQLF-KTN-SYSMON-GRP       VALUE    847.
       78  SQLF-KTN-FCM-PARALLELISM  VALUE    848.
      * tokens for informational parameters
       78  SQLF-KTN-NODETYPE         VALUE    100.
       78  SQLF-KTN-RELEASE          VALUE    101.
       78  SQLF-KTN-NOTOKEN          VALUE    200.

      *************************************************************************
      * defines for database manager configuration parameter values
      **************************************************************************
      * equates for directory type (for dir_type configuration parameter)
       78  SQLF-DIRTYPE-NONE         VALUE    0.
       78  SQLF-DIRTYPE-DCE          VALUE    1.

      * Nodetype equates
       78  SQLF-NT-STANDALONE        VALUE    0.
       78  SQLF-NT-SERVER            VALUE    1.
       78  SQLF-NT-REQUESTOR         VALUE    2.
       78  SQLF-NT-STAND-REQ         VALUE    3.
       78  SQLF-NT-MPP               VALUE    4.
       78  SQLF-NT-SATELLITE         VALUE    5.
      * Note: SQLF_NT_DCS is not a possible instance nodetype.  It
      * represents a host database server
      * reserved for host database server
       78  SQLF-NT-DCS               VALUE    6.
      * Note: SQLF_NT_DAS is not a possible instance nodetype.  It
      * represents an ADMIN server
      * reserved for ADMIN server
       78  SQLF-NT-DAS               VALUE    7.

      *************************************************************************
      * Defines for Database Monitor Switches - SQLF_KTN_DFT_MONSWITCHES:
      * The following meanings apply when the bits are set on:
      * SQLF_ENABLE_MON_UOW       -
      * SQLF_ENABLE_MON_STMT      -
      * SQLF_ENABLE_MON_TABLE     -
      * SQLF_ENABLE_MON_BUFPOOL   -
      * SQLF_ENABLE_MON_LOCK      -
      * SQLF_ENABLE_MON_SORT      -
      * SQLF_ENABLE_MON_TIMESTAMP -
      **************************************************************************
      * xxxx xxxx xxxx xxx1
       78  SQLF-ENABLE-MON-UOW       VALUE    1.
      * xxxx xxxx xxxx xx1x
       78  SQLF-ENABLE-MON-STMT      VALUE    2.
      * xxxx xxxx xxxx x1xx
       78  SQLF-ENABLE-MON-TABLE     VALUE    4.
      * xxxx xxxx xxxx 1xxx
       78  SQLF-ENABLE-MON-BUFPOOL   VALUE    8.
      * xxxx xxxx xxx1 xxxx
       78  SQLF-ENABLE-MON-LOCK      VALUE    16.
      * xxxx xxxx xx1x xxxx
       78  SQLF-ENABLE-MON-SORT      VALUE    32.
      * xxxx xxxx x1xx xxxx
       78  SQLF-ENABLE-MON-TIMESTAMP VALUE    64.

      * Communication Protocols
       78  SQLF-PROTOCOL-TCPIP       value    "TCPIP".
       78  SQLF-PROTOCOL-APPC        value    "APPC".
       78  SQLF-PROTOCOL-NETB        value    "NETBIOS".
       78  SQLF-PROTOCOL-IPXSPX      value    "IPXSPX".

      * Admin. server discover definitions (discover parameter)
       78  SQLF-DSCVR-KNOWN          VALUE    1.
       78  SQLF-DSCVR-SEARCH         VALUE    2.

      * Constants for trusted client options
       78  SQLF-TRUST-ALLCLNTS-NO    VALUE    0.
       78  SQLF-TRUST-ALLCLNTS-YES   VALUE    1.
       78  SQLF-TRUST-ALLCLNTS-DRDAONLY VALUE 2.

      * Token equates for SQLFupd structure for Database configuration file
       78  SQLF-DBTN-BUFF-PAGE       VALUE    90.
       78  SQLF-DBTN-MAXFILOP        VALUE    3.
       78  SQLF-DBTN-SOFTMAX         VALUE    5.
       78  SQLF-DBTN-MAXAPPLS        VALUE    6.
       78  SQLF-DBTN-APPLHEAPSZ      VALUE    51.
       78  SQLF-DBTN-DB-HEAP         VALUE    58.
       78  SQLF-DBTN-DLCHKTIME       VALUE    9.
       78  SQLF-DBTN-SORT-HEAP       VALUE    52.
       78  SQLF-DBTN-MAXLOCKS        VALUE    15.
       78  SQLF-DBTN-LOGPRIMARY      VALUE    16.
       78  SQLF-DBTN-LOGSECOND       VALUE    17.
       78  SQLF-DBTN-LOGFIL-SIZ      VALUE    92.
       78  SQLF-DBTN-STMTHEAP        VALUE    53.
       78  SQLF-DBTN-NEWLOGPATH      VALUE    20.
       78  SQLF-DBTN-COPY-PROTECT    VALUE    22.
       78  SQLF-DBTN-LOG-RETAIN      VALUE    23.
       78  SQLF-DBTN-USER-EXIT       VALUE    24.
       78  SQLF-DBTN-AUTO-RESTART    VALUE    25.
       78  SQLF-DBTN-INDEXREC        VALUE    30.
       78  SQLF-DBTN-PCKCACHESZ      VALUE    31.
       78  SQLF-DBTN-MINCOMMIT       VALUE    32.
       78  SQLF-DBTN-LOGBUFSZ        VALUE    33.
       78  SQLF-DBTN-LOCKTIMEOUT     VALUE    34.
       78  SQLF-DBTN-INDEXSORT       VALUE    35.
       78  SQLF-DBTN-NUM-FREQVALUES  VALUE    36.
       78  SQLF-DBTN-NUM-IOCLEANERS  VALUE    37.
       78  SQLF-DBTN-CHNGPGS-THRESH  VALUE    38.
       78  SQLF-DBTN-NUM-IOSERVERS   VALUE    39.
       78  SQLF-DBTN-DFT-PREFETCH-SZ VALUE    40.
       78  SQLF-DBTN-SEQDETECT       VALUE    41.
       78  SQLF-DBTN-DFT-LOADREC-SES VALUE    42.
       78  SQLF-DBTN-REC-HIS-RETENTN VALUE    43.
       78  SQLF-DBTN-STAT-HEAP-SZ    VALUE    45.
      * token for directory services
       78  SQLF-DBTN-DIR-OBJ-NAME    VALUE    46.
       78  SQLF-DBTN-AVG-APPLS       VALUE    47.
       78  SQLF-DBTN-NUM-QUANTILES   VALUE    48.
       78  SQLF-DBTN-DFT-EXTENT-SZ   VALUE    54.
       78  SQLF-DBTN-UTIL-HEAP-SZ    VALUE    55.
       78  SQLF-DBTN-CATALOGCACHE-SZ VALUE    56.
       78  SQLF-DBTN-DFT-QUERYOPT    VALUE    57.
       78  SQLF-DBTN-APP-CTL-HEAP-SZ VALUE    500.
       78  SQLF-DBTN-TSM-PASSWORD    VALUE    501.
       78  SQLF-DBTN-PCKCACHE-SZ     VALUE    505.
       78  SQLF-DBTN-MULTIPAGE-ALLOC VALUE    506.
       78  SQLF-DBTN-DFT-DEGREE      VALUE    301.
       78  SQLF-DBTN-ESTORE-SEG-SZ   VALUE    303.
       78  SQLF-DBTN-NUM-ESTORE-SEGS VALUE    304.
       78  SQLF-DBTN-TSM-OWNER       VALUE    305.
       78  SQLF-DBTN-TSM-NODENAME    VALUE    306.
       78  SQLF-DBTN-TSM-MGMTCLASS   VALUE    307.
       78  SQLF-DBTN-DISCOVER        VALUE    308.
       78  SQLF-DBTN-DFT-SQLMATHWARN VALUE    309.
       78  SQLF-DBTN-DL-EXPINT       VALUE    350.
       78  SQLF-DBTN-DL-NUM-COPIES   VALUE    351.
       78  SQLF-DBTN-DL-TIME-DROP    VALUE    353.
       78  SQLF-DBTN-NUM-DB-BACKUPS  VALUE    601.
       78  SQLF-DBTN-DL-TOKEN        VALUE    602.
       78  SQLF-DBTN-DL-UPPER        VALUE    603.
       78  SQLF-DBTN-DL-WT-IEXPINT   VALUE    354.
       78  SQLF-DBTN-DYN-QUERY-MGMT  VALUE    604.
       78  SQLF-DBTN-MIN-DEC-DIV-3   VALUE    605.
       78  SQLF-DBTN-DFT-REFRESH-AGE VALUE    702.
       78  SQLF-DBTN-LOCK-LIST       VALUE    704.
       78  SQLF-DBTN-TRACKMOD        VALUE    703.
       78  SQLF-DBTN-APPGROUP-MEM-SZ VALUE    800.
       78  SQLF-DBTN-GROUPHEAP-RATIO VALUE    801.
       78  SQLF-DBTN-SHEAPTHRES-SHR  VALUE    802.
       78  SQLF-DBTN-DATABASE-MEMORY VALUE    803.
       78  SQLF-DBTN-BLK-LOG-DSK-FUL VALUE    804.
       78  SQLF-DBTN-OVERFLOWLOGPATH VALUE    805.
       78  SQLF-DBTN-MIRRORLOGPATH   VALUE    806.
       78  SQLF-DBTN-MAX-LOG         VALUE    807.
       78  SQLF-DBTN-NUM-LOG-SPAN    VALUE    808.
       78  SQLF-DBTN-ALT-COLLATE     VALUE    809.
       78  SQLF-DBTN-HADR-DB-ROLE    VALUE    810.
       78  SQLF-DBTN-HADR-LOCAL-HOST VALUE    811.
       78  SQLF-DBTN-HADR-LOCAL-SVC  VALUE    812.
       78  SQLF-DBTN-HADR-REMOTE-HOST VALUE   813.
       78  SQLF-DBTN-HADR-REMOTE-SVC VALUE    814.
       78  SQLF-DBTN-HADR-REMOTE-INST VALUE   815.
       78  SQLF-DBTN-HADR-TIMEOUT    VALUE    816.
       78  SQLF-DBTN-HADR-SYNCMODE   VALUE    817.
       78  SQLF-DBTN-LOGINDEXBUILD   VALUE    818.
       78  SQLF-DBTN-SHEAPTHRES      VALUE    820.
       78  SQLF-DBTN-STMT-HEAP       VALUE    821.
       78  SQLF-DBTN-LOGARCHMETH1    VALUE    822.
       78  SQLF-DBTN-LOGARCHMETH2    VALUE    823.
       78  SQLF-DBTN-LOGARCHOPT1     VALUE    824.
       78  SQLF-DBTN-LOGARCHOPT2     VALUE    825.
       78  SQLF-DBTN-FAILARCHPATH    VALUE    826.
       78  SQLF-DBTN-NUMARCHRETRY    VALUE    827.
       78  SQLF-DBTN-ARCHRETRYDELAY  VALUE    828.
       78  SQLF-DBTN-VENDOROPT       VALUE    829.
       78  SQLF-DBTN-AUTONOMIC-SWITCHES VALUE 830.
       78  SQLF-DBTN-AUTO-MAINT      VALUE    831.
       78  SQLF-DBTN-AUTO-MAINT-EFF  VALUE    832.
       78  SQLF-DBTN-AUTO-DB-BACKUP  VALUE    833.
       78  SQLF-DBTN-AUTO-DB-BACKUP-EFF VALUE 834.
       78  SQLF-DBTN-AUTO-TBL-MAINT  VALUE    835.
       78  SQLF-DBTN-AUTO-TBL-MAINT-EFF VALUE 836.
       78  SQLF-DBTN-AUTO-RUNSTATS   VALUE    837.
       78  SQLF-DBTN-AUTO-RUNSTATS-EFF VALUE  838.
       78  SQLF-DBTN-AUTO-STATS-PROF VALUE    839.
       78  SQLF-DBTN-AUTO-STATS-PROF-EFF VALUE 840.
       78  SQLF-DBTN-AUTO-PROF-UPD   VALUE    844.
       78  SQLF-DBTN-AUTO-PROF-UPD-EFF VALUE  845.
       78  SQLF-DBTN-PAGESIZE        VALUE    846.
       78  SQLF-DBTN-AUTO-REORG      VALUE    841.
       78  SQLF-DBTN-AUTO-REORG-EFF  VALUE    842.
       78  SQLF-DBTN-DFT-MTTB-TYPES  VALUE    843.
      * tokens for informational (non-modifiable) parameters
       78  SQLF-DBTN-COLLATE-INFO    VALUE    44.
       78  SQLF-DBTN-COUNTRY         VALUE    100.
       78  SQLF-DBTN-TERRITORY-CODE  VALUE    100.
       78  SQLF-DBTN-CODEPAGE        VALUE    101.
       78  SQLF-DBTN-RELEASE         VALUE    102.
       78  SQLF-DBTN-LOGPATH         VALUE    103.
       78  SQLF-DBTN-LOGHEAD         VALUE    105.
       78  SQLF-DBTN-CONSISTENT      VALUE    111.
       78  SQLF-DBTN-BACKUP-PENDING  VALUE    112.
       78  SQLF-DBTN-ROLLFWD-PENDING VALUE    113.
       78  SQLF-DBTN-LOG-RETAIN-STATUS VALUE  114.
       78  SQLF-DBTN-USER-EXIT-STATUS VALUE   115.
       78  SQLF-DBTN-CODESET         VALUE    120.
       78  SQLF-DBTN-TERRITORY       VALUE    121.
       78  SQLF-DBTN-NUMSEGS         VALUE    122.
       78  SQLF-DBTN-DATABASE-LEVEL  VALUE    124.
       78  SQLF-DBTN-NOTOKEN         VALUE    200.
       78  SQLF-DBTN-RESTORE-PENDING VALUE    503.

      * defines for database configuration parameter values

      * Generic Disable/Enable values for configuration parameters
       78  SQLF-DISABLE              VALUE    0.
       78  SQLF-ENABLE               VALUE    1.

      * Index recreate type (INDEXREC values)
       78  SQLF-INX-REC-SYSTEM       VALUE    0.
       78  SQLF-INX-REC-REFERENCE    VALUE    1.
       78  SQLF-INX-REC-RESTART      VALUE    2.
       78  SQLF-INX-REC-RESTART-NO-REDO VALUE 3.
       78  SQLF-INX-REC-ACCESS-NO-REDO VALUE  4.

      * Possible values for: SQLF_DBTN_ROLLFWD_PENDING are:
      * SQLF_DISABLE, SQLF_ENABLE, and SQLF_ROLLFWD_TABLESPACE
      * The third state of rollfwd_pending
       78  SQLF-ROLLFWD-TABLESPACE   VALUE    2.

      * Defines for SQLF_DBTN_LOG_RETAIN and SQLF_DBTN_LOG_RETAIN_STATUS
      * Not configured with LOGRETAIN
       78  SQLF-LOGRETAIN-DISABLE    VALUE    0.
      * Log file retained for recovery
       78  SQLF-LOGRETAIN-RECOVERY   VALUE    1.
      * Log file retained for CAPTURE, will be deleted after CAPTURE
      * finished processing
       78  SQLF-LOGRETAIN-CAPTURE    VALUE    2.

      *************************************************************************
      * HADR - High Availability Data Recovery parameters
      **************************************************************************
      * Internet std for hostname is 255
       78  SQLF-HADR-HOSTNAME-SZ     VALUE    255.
      * Internet std for svcename is 40
       78  SQLF-HADR-SVCENAME-SZ     VALUE    40.
      * HADR instance name
       78  SQLF-HADR-INSTANCE-SZ     VALUE    8.

      *************************************************************************
      * HADR_DB_ROLE values
      **************************************************************************
      * Not an HADR database
       78  SQLF-HADR-DB-ROLE-STANDARD VALUE   0.
      * An HADR primary db
       78  SQLF-HADR-DB-ROLE-PRIMARY VALUE    1.
      * An HADR standby db
       78  SQLF-HADR-DB-ROLE-STANDBY VALUE    2.

      *************************************************************************
      * HADR_SYNCMODE values
      **************************************************************************
      * Synchronous mode
       78  SQLF-HADR-SYNCMODE-SYNC   VALUE    0.
      * Near-synchronous mode
       78  SQLF-HADR-SYNCMODE-NEARSYNC VALUE  1.
      * Asynchronous mode
       78  SQLF-HADR-SYNCMODE-ASYNC  VALUE    2.

      *************************************************************************
      * DFT_MTTB_TYPES values
      **************************************************************************
      * SYSTEM
       78  SQLF-DFT-MTTB-TYPES-SYSTEM VALUE   1.
      * USER
       78  SQLF-DFT-MTTB-TYPES-USER  VALUE    2.
      * NONE
       78  SQLF-DFT-MTTB-TYPES-NONE  VALUE    4.
      * ALL
       78  SQLF-DFT-MTTB-TYPES-ALL   VALUE    8.
      * FEDERATED_TOOL
       78  SQLF-DFT-MTTB-TYPES-FED-TOOL VALUE 16.

      *************************************************************************
      * Constants for configuration parameter maximum lengths
      **************************************************************************
      * length of syncpoint manager name
       78  SQL-SPM-NAME-SZ           VALUE    8.
      * max length of log parms
       78  SQL-LOGPATH-SZ            VALUE    242.
      * max length of log file name
       78  SQL-LOGFILE-NAME-SZ       VALUE    12.
      * max length of db path parm
       78  SQL-DBPATH-SZ             VALUE    215.
      * size of dataflow encrypt list
       78  SQL-DFENCRYPTLIST-SZ      VALUE    24.
      * max length of diagpath parm
       78  SQL-FFDCPATH-SZ           VALUE    215.
      * max filename size (see SQLZ_FNAME_SZ)
       78  SQL-FILENAME-SZ           VALUE    254.

      * Constants for types of Data Redistribution operation
      * Uniform redistribution
       78  SQLUT-UNIFORM             value    "U".
      * Using target partitioning map
       78  SQLUT-TARGETMAP           value    "T".
      * Resuming a stopped data redist.
       78  SQLUT-CONTINUE            value    "C".
      * Rolling back a stopped data redist.
       78  SQLUT-ROLLBACK            value    "R".
      * LOAD and UNLOAD defines
      * Automatic Restart
       78  SQLU-AUTOMATIC-RESTART    value    "A".
      * Reserved for backward compability
       78  SQLU-LOAD-PHASE           value    "L".
      * Reserved for backward compability
       78  SQLU-BUILD-PHASE          value    "B".
      * Reserved for backward compability
       78  SQLU-DELETE-PHASE         value    "D".



      * Access Level Defines

      * No access to table during load or import
       78  SQLU-ALLOW-NO-ACCESS      VALUE    0.
      * Read access to table during load
       78  SQLU-ALLOW-READ-ACCESS    VALUE    1.
      * Write access to table during import
       78  SQLU-ALLOW-WRITE-ACCESS   VALUE    2.

       01 SQL-DCOLDATA.
           05 SQL-DCOLMETH           PIC S9(4) COMP-5.
           05 SQL-DCOLNUM            PIC S9(4) COMP-5.
           05 SQLDCOLN OCCURS 0 TO 255 TIMES DEPENDING ON SQL-DCOLNUM.
               10 SQL-DCOLNLEN       PIC S9(4) COMP-5.
               10 FILLER             PIC X(2).
               10 SQL-DCOLN-PTR      USAGE IS POINTER.

      *  Note that SQL-FILETMOD-DATA is defined to have 256 characters.
      *  256 is an arbitrary maximum that should be large enough.
       01 SQL-FILETMOD.
           05 SQL-FILETMOD-LEN PIC 9(4) COMP-5 VALUE 0.
           05 SQL-FILETMOD-DATA PIC X(256) VALUE SPACES.


      *  Note that SQL-TCOLSTRG-DATA is defined to have 256 characters.
      *  256 is an arbitrary maximum that should be large enough.
       01 SQL-TCOLSTRG.
           05 SQL-TCOLSTRG-LEN PIC 9(4) COMP-5 VALUE 0.
           05 SQL-TCOLSTRG-DATA PIC X(256) VALUE SPACES.

      * Structure for Pairs of Locations for ASC files
       01 SQL-LOCTAB.
      * actual size must be dcolnum
           05 SQL-LOC-PAIR OCCURS 1 TIMES.
               10 SQL-BEGIN-LOC      PIC S9(4) COMP-5.
               10 SQL-END-LOC        PIC S9(4) COMP-5.

      * Structures for Input and Output Data for Import and Export

       01 SQL-UIMPT-IN.
           05 SQL-SIZE-OF-UIMPT-IN   PIC 9(9) COMP-5 VALUE 12.
           05 SQL-COMMITCNT          PIC 9(9) COMP-5 VALUE 0.
           05 SQL-RESTARTCNT         PIC 9(9) COMP-5 VALUE 0.

       01 SQL-UIMPT-OUT.
           05 SQL-SIZE-OF-UIMPT-OUT  PIC 9(9) COMP-5 VALUE 28.
           05 SQL-ROWSREAD           PIC 9(9) COMP-5 VALUE 0.
           05 SQL-ROWSSKIPPED        PIC 9(9) COMP-5 VALUE 0.
           05 SQL-ROWSINSERTED       PIC 9(9) COMP-5 VALUE 0.
           05 SQL-ROWSUPDATED        PIC 9(9) COMP-5 VALUE 0.
           05 SQL-ROWSREJECTED       PIC 9(9) COMP-5 VALUE 0.
           05 SQL-ROWSCOMMITTED      PIC 9(9) COMP-5 VALUE 0.

       01 SQL-UEXPT-OUT.
           05 SQL-SIZE-OF-UEXPT-OUT  PIC 9(9) COMP-5 VALUE 8.
           05 SQL-ROWSEXPORTED       PIC 9(9) COMP-5 VALUE 0.

       78  SQLUIMPT-IN-SIZE          value    12.
       78  SQLUIMPT-OUT-SIZE         value    28.
       78  SQLUEXPT-OUT-SIZE         value    8.

      * Structures for Input and Output Data for Load and Unload

       01 SQLULOAD-IN.
           05 SQL-SIZE-OF-STRUCT     PIC 9(9) COMP-5 VALUE 40.
           05 SQL-SAVECNT            PIC 9(9) COMP-5.
           05 SQL-RESTARTCOUNT       PIC 9(9) COMP-5.
           05 SQL-ROWCNT             PIC 9(9) COMP-5.
           05 SQL-WARNINGCNT         PIC 9(9) COMP-5.
           05 SQL-DATA-BUFFER-SIZE   PIC 9(9) COMP-5.
           05 SQL-SORT-BUFFER-SIZE   PIC 9(9) COMP-5.
           05 SQL-HOLD-QUIESCE       PIC 9(4) COMP-5.
           05 SQL-RESTARTPHASE       PIC X.
           05 SQL-STATSOPT           PIC X.
           05 SQL-CPU-PARALLELISM    PIC 9(4) COMP-5.
           05 SQL-DISK-PARALLELISM   PIC 9(4) COMP-5.
           05 SQL-NON-RECOVERABLE    PIC 9(4) COMP-5.
           05 SQL-INDEXING-MODE      PIC 9(4) COMP-5.

       01 SQLULOAD-OUT.
           05 SQL-SIZE-OF-STRUCT     PIC 9(9) COMP-5 VALUE 28.
           05 SQL-ROWS-READ          PIC 9(9) COMP-5.
           05 SQL-ROWS-SKIPPED       PIC 9(9) COMP-5.
           05 SQL-ROWS-LOADED        PIC 9(9) COMP-5.
           05 SQL-ROWS-REJECTED      PIC 9(9) COMP-5.
           05 SQL-ROWS-DELETED       PIC 9(9) COMP-5.
           05 SQL-ROWS-COMMITTED     PIC 9(9) COMP-5.

       01 SQLUUNLD-IN.
           05 SQL-SIZE-OF-STRUCT     PIC 9(9) COMP-5 VALUE 24.
           05 SQL-DATA-BUFFER-SIZE   PIC 9(9) COMP-5.
           05 SQL-SORT-BUFFER-SIZE   PIC 9(9) COMP-5.
           05 SQL-HOLD-QUIESCE       PIC 9(4) COMP-5.
           05 SQL-INTENT-UPDATE      PIC 9(4) COMP-5.
           05 SQL-SERIAL-WRITE       PIC 9(4) COMP-5.
           05 SQL-NUM-BUFFERS        PIC 9(4) COMP-5.
           05 SQL-REORG-LOBS         PIC 9(4) COMP-5.
           05 SQL-FILLER             PIC 9(4) COMP-5.

       01 SQLUUNLD-OUT.
           05 SQL-SIZE-OF-STRUCT     PIC 9(9) COMP-5 VALUE 8.
           05 SQL-ROWS-UNLOADED      PIC 9(9) COMP-5.

       78  SQLULOAD-IN-SIZE          value    40.
       78  SQLULOAD-OUT-SIZE         value    28.
       78  SQLUUNLD-IN-SIZE          value    24.
       78  SQLUUNLD-OUT-SIZE         value    8.

      * Structures for Configuration Utility APIs.

       01 SQL-FUPD.
           05 SQL-TOKEN              PIC 9(4) COMP-5.
           05 FILLER                 PIC X(2).
           05 SQL-VALUE-PTR          USAGE IS POINTER.

      * Constants and structure used by sqlgadau and sqluadau.
       78  SQL-USER-AUTHORIZED       VALUE    1.
       78  SQL-USER-UNAUTHORIZED     VALUE    0.
       78  SQL-AUTHORIZATION-SIZE-OLD VALUE   22.
       78  SQL-AUTHORIZATION-SIZE-V2 VALUE    34.
       78  SQL-AUTHORIZATION-SIZE-V3 VALUE    38.
       78  SQL-AUTHORIZATION-SIZE-V7 VALUE    42.
       78  SQL-AUTHORIZATION-SIZE-V8 VALUE    58.
       78  SQL-AUTHORIZATION-SIZE    VALUE    62.

       01 SQL-AUTHORIZATIONS.
           05 SQL-AUTHORIZATIONS-LEN PIC S9(4) COMP-5.
           05 SQL-SYSADM-AUTH        PIC S9(4) COMP-5.
           05 SQL-DBADM-AUTH         PIC S9(4) COMP-5.
           05 SQL-CREATETAB-AUTH     PIC S9(4) COMP-5.
           05 SQL-BINDADD-AUTH       PIC S9(4) COMP-5.
           05 SQL-CONNECT-AUTH       PIC S9(4) COMP-5.
           05 SQL-SYSADM-GRP-AUTH    PIC S9(4) COMP-5.
           05 SQL-DBADM-GRP-AUTH     PIC S9(4) COMP-5.
           05 SQL-CREATETAB-GRP-AUTH PIC S9(4) COMP-5.
           05 SQL-BINDADD-GRP-AUTH   PIC S9(4) COMP-5.
           05 SQL-CONNECT-GRP-AUTH   PIC S9(4) COMP-5.
           05 SQL-SYSCTRL-AUTH       PIC S9(4) COMP-5.
           05 SQL-SYSCTRL-GRP-AUTH   PIC S9(4) COMP-5.
           05 SQL-SYSMAINT-AUTH      PIC S9(4) COMP-5.
           05 SQL-SYSMAINT-GRP-AUTH  PIC S9(4) COMP-5.
           05 SQL-CREATE-NOT-FENC-AUTH PIC S9(4) COMP-5.
           05 SQL-CREATE-NOT-FENC-GRP-AUTH PIC S9(4) COMP-5.
           05 SQL-IMPLICIT-SCHEMA-AUTH PIC S9(4) COMP-5.
           05 SQL-IMPLICIT-SCHEMA-GRP-AUTH PIC S9(4) COMP-5.
           05 SQL-LOAD-AUTH          PIC S9(4) COMP-5.
           05 SQL-LOAD-GRP-AUTH      PIC S9(4) COMP-5.
           05 SQL-CREATE-EXT-RTN-AUTH PIC S9(4) COMP-5.
           05 SQL-CREATE-EXT-RTN-GRP-AUTH PIC S9(4) COMP-5.
           05 SQL-QUIESCE-CONNECT-AUTH PIC S9(4) COMP-5.
           05 SQL-QUIESCE-CONNECT-GRP-AUTH PIC S9(4) COMP-5.
           05 SQL-SECURITY-ADMIN-AUTH PIC S9(4) COMP-5.
           05 SQL-SECURITY-ADMIN-GRP-AUTH PIC S9(4) COMP-5.
           05 SQL-LIBRARY-ADMIN-AUTH PIC S9(4) COMP-5.
           05 SQL-LIBRARY-ADMIN-GRP-AUTH PIC S9(4) COMP-5.
           05 SQL-SYSMON-AUTH        PIC S9(4) COMP-5.
           05 SQL-SYSMON-GRP-AUTH    PIC S9(4) COMP-5.

      * Constants and structure used for backup and restore
       78  SQLU-MAX-TBS-NAME-LEN     VALUE    18.
       78  SQLU-DB-DIR-LEN           VALUE    215.
       78  SQLU-SHR-LIB-LEN          VALUE    255.
       78  SQLU-MEDIA-LOCATION-LEN   VALUE    255.
       78  SQLU-APPLID-LEN           VALUE    32.
       78  SQLU-MAX-SESSIONS         VALUE    999.

       01 SQLU-TABLESPACE-ENTRY.
           05 SQL-TBSP-LEN           PIC 9(9) COMP-5.
           05 SQL-TABLESPACE-ENTRY   PIC X(18).
           05 FILLER                 PIC X.
           05 SQL-FILLER             PIC X(1).

       01 SQLU-TABLESPACE-BKRST-LIST.
           05 SQL-NUM-ENTRY          PIC S9(9) COMP-5.
           05 SQL-TABLESPACE         USAGE IS POINTER.

       01 SQLU-MEDIA-ENTRY.
           05 SQL-MEDENT-LEN         PIC 9(9) COMP-5.
           05 SQL-MEDIA-ENTRY        PIC X(215).
           05 FILLER                 PIC X.

       01 SQLU-VENDOR.
           05 SQL-SHRLIB-LEN         PIC 9(9) COMP-5.
           05 SQL-SHR-LIB            PIC X(255).
           05 FILLER                 PIC X.
           05 SQL-FILENAME-LEN       PIC 9(9) COMP-5.
           05 SQL-FILENAME           PIC X(255).
           05 FILLER                 PIC X.

       01 SQLU-LOCATION-ENTRY.
           05 SQL-LOCATION-LEN       PIC 9(9) COMP-5.
           05 SQL-LOCATION-ENTRY     PIC X(255).
           05 FILLER                 PIC X.

       01 SQLU-STATEMENT-ENTRY.
           05 SQL-STATEMENT-LEN      PIC 9(9) COMP-5.
           05 SQL-STATEMENT-ENTRY    USAGE IS POINTER.

       01 SQLU-HIST-FILE.
      * node number
           05 SQL-NODENUM            PIC S9(4) COMP-5.
      * length of filename
           05 SQL-FILENAMELEN        PIC 9(4) COMP-5.
      * name of the history file
           05 SQL-FILENAME           PIC X(254).
           05 FILLER                 PIC X.
           05 FILLER                 PIC X(1).

       01 SQLURF-NEWLOGPATH.
      * node number
           05 SQL-NODENUM            PIC S9(4) COMP-5.
      * length of new logpath
           05 SQL-PATHLEN            PIC 9(4) COMP-5.
      * new overflow log path
           05 SQL-LOGPATH            PIC X(254).
           05 FILLER                 PIC X.
           05 FILLER                 PIC X(1).

       01 SQLURF-INFO.
      * node number
           05 SQL-NODENUM            PIC S9(4) COMP-5.
           05 FILLER                 PIC X(2).
      * state information
           05 SQL-STATE              PIC S9(9) COMP-5.
      * next archive file needed
           05 SQL-NEXTARCLOG         PIC X(12).
           05 FILLER                 PIC X.
      * first archive file to be deleted
           05 SQL-FIRSTARCDEL        PIC X(12).
           05 FILLER                 PIC X.
      * last archive file to be deleted
           05 SQL-LASTARCDEL         PIC X(12).
           05 FILLER                 PIC X.
      * returned timestamp
           05 SQL-LASTCOMMIT         PIC X(26).
           05 FILLER                 PIC X.
           05 FILLER                 PIC X(2).

       01 SQLURF-STAT.
           05 SQL-NUMNODES           PIC S9(9) COMP-5.
           05 SQL-NODE-INFO OCCURS 1000 TIMES.
      * node number
               10 SQL-NODENUM        PIC S9(4) COMP-5.
               10 FILLER             PIC X(2).
      * state information
               10 SQL-STATE          PIC S9(9) COMP-5.
      * next archive file needed
               10 SQL-NEXTARCLOG     PIC X(12).
               10 FILLER             PIC X.
      * first archive file to be deleted
               10 SQL-FIRSTARCDEL    PIC X(12).
               10 FILLER             PIC X.
      * last archive file to be deleted
               10 SQL-LASTARCDEL     PIC X(12).
               10 FILLER             PIC X.
      * returned timestamp
               10 SQL-LASTCOMMIT     PIC X(26).
               10 FILLER             PIC X.
               10 FILLER             PIC X(2).

       01 SQL-RFWD-INPUT.
      * rollforward version
           05 SQL-VERSION            PIC 9(9) COMP-5.
      * database alias
           05 SQL-DBALIAS            USAGE IS POINTER.
      * action
           05 SQL-CALLERACTION       PIC 9(4) COMP-5.
           05 FILLER                 PIC X(2).
      * stop time
           05 SQL-STOPTIME           USAGE IS POINTER.
      * user name
           05 SQL-USERNAME           USAGE IS POINTER.
      * password
           05 SQL-PASSWORD           USAGE IS POINTER.
      * overflow log path
           05 SQL-OVERFLOWLOGPATH    USAGE IS POINTER.
      * no. of changed overflow log paths
           05 SQL-NUMCHANGE          PIC 9(4) COMP-5.
           05 FILLER                 PIC X(2).
      * changed overflow log paths
           05 SQL-P-CHNG-LOG-OVRFLW  USAGE IS POINTER.
      * connect mode
           05 SQL-CONNECTMODE        PIC 9(4) COMP-5.
           05 FILLER                 PIC X(2).
      * tablespace list
           05 SQL-P-TABLESPACE-LIST  USAGE IS POINTER.
      * all node flag
           05 SQL-ALLNODEFLAG        PIC S9(4) COMP-5.
      * size of nodelist
           05 SQL-NUMNODES           PIC S9(4) COMP-5.
      * list of node numbers
           05 SQL-NODELIST           USAGE IS POINTER.
      * size of pNodeInfo in rfwd_output
           05 SQL-NUMNODEINFO        PIC S9(4) COMP-5.
      * not used
           05 SQL-DLMODE             PIC 9(4) COMP-5.
      * not used
           05 SQL-REPORTFILE         USAGE IS POINTER.
      * dropped table id
           05 SQL-DROPPEDTBLID       USAGE IS POINTER.
      * dropped table export directory
           05 SQL-EXPORTDIR          USAGE IS POINTER.

       01 SQL-GRFWD-INPUT.
      * length in bytes of DbAlias
           05 SQL-DBALIASLEN         PIC 9(4) COMP-5.
      * length in bytes of StopTime
           05 SQL-STOPTIMELEN        PIC 9(4) COMP-5.
      * length in bytes of UserName
           05 SQL-USERNAMELEN        PIC 9(4) COMP-5.
      * length in bytes of Password
           05 SQL-PASSWORDLEN        PIC 9(4) COMP-5.
      * length in bytes of OverflowLogPath
           05 SQL-OVRLOGPATHLEN      PIC 9(4) COMP-5.
      * length in bytes of ReportFile
           05 SQL-REPORTFILELEN      PIC 9(4) COMP-5.
      * rollforward version
           05 SQL-VERSION            PIC 9(9) COMP-5.
      * database alias
           05 SQL-DBALIAS            USAGE IS POINTER.
      * action
           05 SQL-CALLERACTION       PIC 9(4) COMP-5.
           05 FILLER                 PIC X(2).
      * stop time
           05 SQL-STOPTIME           USAGE IS POINTER.
      * user name
           05 SQL-USERNAME           USAGE IS POINTER.
      * password
           05 SQL-PASSWORD           USAGE IS POINTER.
      * overflow log path
           05 SQL-OVERFLOWLOGPATH    USAGE IS POINTER.
      * no. of changed overflow log paths
           05 SQL-NUMCHANGE          PIC 9(4) COMP-5.
           05 FILLER                 PIC X(2).
      * changed overflow log paths
           05 SQL-P-CHNG-LOG-OVRFLW  USAGE IS POINTER.
      * connect mode
           05 SQL-CONNECTMODE        PIC 9(4) COMP-5.
           05 FILLER                 PIC X(2).
      * tablespace list
           05 SQL-P-TABLESPACE-LIST  USAGE IS POINTER.
      * all node flag
           05 SQL-ALLNODEFLAG        PIC S9(4) COMP-5.
      * size of nodelist
           05 SQL-NUMNODES           PIC S9(4) COMP-5.
      * list of node numbers
           05 SQL-NODELIST           USAGE IS POINTER.
      * size of pNodeInfo in rfwd_output
           05 SQL-NUMNODEINFO        PIC S9(4) COMP-5.
      * not used
           05 SQL-DL                 PIC 9(4) COMP-5.
      * not used
           05 SQL-REPORTFILE         USAGE IS POINTER.
      * dropped table id
           05 SQL-DROPPEDTBLID       USAGE IS POINTER.
      * dropped table export directory
           05 SQL-EXPORTDIR          USAGE IS POINTER.

       01 SQL-RFWD-OUTPUT.
      * application id
           05 SQL-APPLID             USAGE IS POINTER.
      * number of replies received
           05 SQL-NUMREPLIES         USAGE IS POINTER.
      * node reply info
           05 SQL-P-NODE-INFO        USAGE IS POINTER.

      * Media types
      * path/device
       78  SQLU-LOCAL-MEDIA          value    "L".
      * remote file/device/named pipe
       78  SQLU-SERVER-LOCATION      value    "S".
      * local file/device/named pipe
       78  SQLU-CLIENT-LOCATION      value    "C".
      * SQL Statement
       78  SQLU-SQL-STMT             value    "Q".
      * Tivoli Storage Manager
       78  SQLU-TSM-MEDIA            value    "A".
      * X/Open XBSA interface
       78  SQLU-XBSA-MEDIA           value    "X".
      * vendor library
       78  SQLU-OTHER-MEDIA          value    "O".
      * user exit
       78  SQLU-USER-EXIT            value    "U".
      * Generated only by vendors
       78  SQLU-DISK-MEDIA           value    "D".
      * Generated only by vendors
       78  SQLU-DISKETTE-MEDIA       value    "K".
      * Generated internally by DB2
       78  SQLU-NULL-MEDIA           value    "N".
      * Generated only by vendors
       78  SQLU-TAPE-MEDIA           value    "T".
      * Generated only by vendors
       78  SQLU-PIPE-MEDIA           value    "P".

       01 SQLU-MEDIA-LIST.
           05 SQL-MEDIA-TYPE         PIC X.
           05 SQL-FILLER             PIC X(3).
           05 SQL-SESSIONS           PIC S9(9) COMP-5.
           05 SQL-TARGET.
      * For media_type L - for BACKUP, RESTORE and LOAD copytarget,
      * workdirectory, lobpaths, IMPORT and EXPORT lobpaths
               10 SQL-MEDIA          USAGE IS POINTER.
      * For media_type O
               10 SQL-VENDOR         REDEFINES SQL-MEDIA
                                     USAGE IS POINTER.
      * For media_type S - for LOAD and LOAD recovery
      * For media type C - for EXPORT lobfile
               10 SQL-LOCATION       REDEFINES SQL-MEDIA
                                     USAGE IS POINTER.
      * For media_type S - for LOAD and LOAD recovery
               10 SQL-P-STATEMENT    REDEFINES SQL-MEDIA
                                     USAGE IS POINTER.

      * Struct used by sqlugtpi, sqlugrpi and sqlugrpn to store partitioning
      * information
       01 SQLUPI.
      * Length of partitioning map
           05 SQL-PMAPLEN            PIC 9(4) COMP-5.
      * Partitioning map
           05 SQL-PMAP               PIC S9(4) COMP-5 OCCURS 4096 TIMES.
      * # of used SQLPARTKEY elements
           05 SQLUPI-SQLD            PIC 9(4) COMP-5.
      * Partitioning Keys
           05 SQLPARTKEY OCCURS 500 TIMES.
      * Date Type of Key
               10 SQLUPI-TYPE        PIC 9(4) COMP-5.
      * Data Length of Key
               10 SQLUPI-LEN         PIC 9(4) COMP-5.

       01 SQLUHTSP.
      * tablespace name
           05 SQL-TABLESPACE-NAME    PIC X(18).
           05 FILLER                 PIC X.
           05 SQL-FILLER             PIC X.

       01 SQLUHADM.
      * Completion time of operation
           05 SQL-END-TIME           PIC X(14).
           05 FILLER                 PIC X.
      * Unique object identifier
           05 SQL-ID                 PIC X(24).
           05 FILLER                 PIC X.
      * Result SQLCA from event
           05 SQL-EVENT-SQLCA.
               10 SQL-EVENT-SQLCAID  PIC X(8).
               10 SQL-EVENT-SQLABC   PIC S9(9) COMP-5.
               10 SQL-EVENT-SQLCODE  PIC S9(9) COMP-5.
               10 SQL-EVENT-SQLERRML PIC S9(4) COMP-5.
               10 SQL-EVENT-SQLERRMC PIC X(70).
               10 SQL-EVENT-SQLERRP  PIC X(8).
               10 SQL-EVENT-SQLERRD  PIC S9(9) COMP-5 OCCURS 6 TIMES.
               10 SQL-EVENT-SQLWARN  PIC X(11).
               10 SQL-EVENT-SQLSTATE PIC X(5).
      * Command text
           05 SQL-COMMAND.
               10 SQL-LENGTH         PIC S9(4) COMP-5.
               10 SQL-DATA           PIC X(1).
               10 FILLER             PIC X(1).

       01 SQLUHINFO.
      * Eye catcher = 'SQLUHINF'
           05 SQLUHINFOID            PIC X(8).
      * SQLUHINFO  in bytes=(   )
           05 SQLUHINFOBC            PIC S9(9) COMP-5.
      * # of tablespace elements available
           05 SQLH-SQLN              PIC S9(4) COMP-5.
      * # of used tablespace elements
           05 SQLH-SQLD              PIC S9(4) COMP-5.
      * Type of event
           05 SQL-OPERATION          PIC X(1).
           05 FILLER                 PIC X.
      * Granularity of operation
           05 SQL-OBJECT             PIC X(1).
           05 FILLER                 PIC X.
      * Start time + sequence number.
           05 SQL-OBJECT-PART        PIC X(17).
           05 FILLER                 PIC X.
      * More details of operation
           05 SQL-OPTYPE             PIC X(1).
           05 FILLER                 PIC X.
           05 SQL-DEVICE-TYPE        PIC X(1).
           05 FILLER                 PIC X.
           05 SQL-FIRST-LOG          PIC X(12).
           05 FILLER                 PIC X.
           05 SQL-LAST-LOG           PIC X(12).
           05 FILLER                 PIC X.
           05 SQL-BACKUP-ID          PIC X(14).
           05 FILLER                 PIC X.
           05 SQL-TABLE-CREATOR      PIC X(8).
           05 FILLER                 PIC X.
           05 SQL-TABLE-NAME         PIC X(18).
           05 FILLER                 PIC X.
           05 SQL-NUM-OF-TABLESPACES PIC X(5).
           05 FILLER                 PIC X.
           05 SQL-LOCATION           PIC X(255).
           05 FILLER                 PIC X.
           05 SQL-COMMENT            PIC X(30).
           05 FILLER                 PIC X.
           05 SQL-TABLESPACE OCCURS 1 TIMES.
      * tablespace name
               10 SQL-TABLESPACE-NAME PIC X(18).
               10 FILLER             PIC X.
               10 SQL-FILLER         PIC X.

      *  structure and defines for sqldzcfg API
      * 
      * nb: 'tbs' is an abbreviaton for 'TABLESPACE'

      * max len of File Manager server name
       78  SQLDZ-MAX-FS-NAME-LEN     VALUE    256.
      * JFS support for Datalinks
       78  SQL-NATIVE-DLFF           VALUE    0.
      * DFS support for Datalinks
       78  SQL-DFS-DLFF              VALUE    1.
      * Active Datalinks Manager
       78  SQL-ACTIVE-DLFM           VALUE    0.
      * Deleted Datalinks Manager
       78  SQL-INACTIVE-DLFM         VALUE    1.
       01 SQLDZ-SERVERINFO.
      * DLFM TCPIP hostname
           05 SQL-FILE-SERVER        PIC X(256).
           05 FILLER                 PIC X.
           05 FILLER                 PIC X(3).
      * TCPIP Port Number
           05 SQL-FILE-SERVER-PORT   PIC S9(9) COMP-5.
      * File Manager Type
           05 SQL-FILE-SERVER-TYPE   PIC S9(9) COMP-5.
      * File Manager Status
           05 SQL-FILE-SERVER-STATUS PIC S9(9) COMP-5.
      * DLFM Instance Name
           05 SQL-FILE-SERVER-INSTANCE PIC X(8).
           05 FILLER                 PIC X.
           05 FILLER                 PIC X(3).

      * Returns number of defined File Manager Servers
       78  SQLDZ-SIZE                VALUE    1.
      * Returns sqludz_serverinfo entries for all defined File Manager
      * Servers
       78  SQLDZ-LIST                VALUE    2.
      * Adds File Manager Servers to datalink.cfg config file
       78  SQLDZ-ADD                 VALUE    3.
      * Updates File Manager Servers in datalink.cfg config file
       78  SQLDZ-UPDATE              VALUE    4.
      * Deletes File Manager Servers from datalink.cfg config file
       78  SQLDZ-DELETE              VALUE    5.
      * Definitions needed for the sqlb api calls
      * 
      * nb: 'tbs' is an abbreviaton for 'TABLESPACE'

      * length of structure version
       78  SQLB-SVERSION-SIZE        VALUE    8.
      * maximum length of a table space name
       78  SQLB-MAX-TBS-NAME-SZ      VALUE    128.
      * NB: this constant is set to the maximum size that is likely to be
      * needed in future.  Don't confuse this with SQLU_MAX_TBS_NAME_LEN
      * which is set to the current maximum size (18).

      * Externally visibile flags of a table space
      * storage manager class
      * System   Managed Space
       78  SQLB-TBS-SMS              VALUE    1.
      * Database Managed Space
       78  SQLB-TBS-DMS              VALUE    2.
      * table space contents
      * regular contents
       78  SQLB-TBS-ANY              VALUE    0.
      * long field data
       78  SQLB-TBS-LONG             VALUE    32.
      * system temp data
       78  SQLB-TBS-SYSTMP           VALUE    16.
      * user temp data
       78  SQLB-TBS-USRTMP           VALUE    64.
      * kept for compatibility
       78  SQLB-TBS-TMP              value    16.
      * other flags values
      * temp table space is reserved for completion of reorg recovery
       78  SQLB-TBS-TMP-RECOVERY     VALUE    131072.

      * The externally visible state of a table space is composed of the
      * hexadecimal sum of various of the following state values.  For
      * example if the state is 'quiesced exclusive' and 'load pending', the
      * value would be 0x0004 + 0x0008 which is 0x000c.
      * 
      * You may use the tool db2tbst to do the reverse calculation (from
      * 0x000c to 'load pending' and 'quiesced exclusive', for example)
      * which is more complex. In our example, execute:
      * 
      *  db2tbst 0x000c
      * 
      * which yields:
      * 
      * State = Quiesced Exclusive
      *       + Load Pending
      * Normal
       78  SQLB-NORMAL               VALUE    0.
      * Quiesced: SHARE
       78  SQLB-QUIESCED-SHARE       VALUE    1.
      * Quiesced: UPDATE
       78  SQLB-QUIESCED-UPDATE      VALUE    2.
      * Quiesced: EXCLUSIVE
       78  SQLB-QUIESCED-EXCLUSIVE   VALUE    4.
      * Load pending
       78  SQLB-LOAD-PENDING         VALUE    8.
      * Delete pending
       78  SQLB-DELETE-PENDING       VALUE    16.
      * Backup pending
       78  SQLB-BACKUP-PENDING       VALUE    32.
      * Roll forward in progress
       78  SQLB-ROLLFORWARD-IN-PROGRESS VALUE 64.
      * Roll forward pending
       78  SQLB-ROLLFORWARD-PENDING  VALUE    128.
      * Restore pending
       78  SQLB-RESTORE-PENDING      VALUE    256.
      * Recovery pending (use SQLB_RESTORE_PENDING)
       78  SQLB-RECOVERY-PENDING     VALUE    256.
      * Disable pending
       78  SQLB-DISABLE-PENDING      VALUE    512.
      * Reorg in progress
       78  SQLB-REORG-IN-PROGRESS    VALUE    1024.
      * Backup in progress
       78  SQLB-BACKUP-IN-PROGRESS   VALUE    2048.
      * storage must be defined
       78  SQLB-STORDEF-PENDING      VALUE    4096.
      * Restore in progress
       78  SQLB-RESTORE-IN-PROGRESS  VALUE    8192.
      * Offline and not accessible
       78  SQLB-OFFLINE              VALUE    16384.
      * Only action is to drop
       78  SQLB-DROP-PENDING         VALUE    32768.
      * No write is allowed
       78  SQLB-SUSPEND-WRITE        VALUE    65536.
      * Load in progress
       78  SQLB-LOAD-IN-PROGRESS     VALUE    131072.
      * storage may be defined
       78  SQLB-STORDEF-ALLOWED      VALUE    33554432.
      * storDef is in 'final' state
       78  SQLB-STORDEF-FINAL-VERSION VALUE   67108864.
      * storDef was changed prior to rollforward
       78  SQLB-STORDEF-CHANGED      VALUE    134217728.
      * dms rebalancer is active
       78  SQLB-REBAL-IN-PROGRESS    VALUE    268435456.
      * TBS deletion in progress
       78  SQLB-PSTAT-DELETION       VALUE    536870912.
      * TBS creation in progress
       78  SQLB-PSTAT-CREATION       VALUE    1073741824.

      * For service use only.
       78  SQLB-STATE-SET            VALUE    8.

      * provide information about the quiescers of a tablespace

      * Maximum number of quiescers
       78  SQLB-MAX-QUIESCERS        VALUE    5.
      * SQLB_TBSPQRY_DATA version id
       78  SQLB-TBSPQRY-DATA-ID      value    "TBSQ0030".

      * Provide info about a tableSpace in a database

       78  SQLB-TBSPQRY-DATA-SIZE    value    300.
      * container types
      * SMS: directory path specs
       78  SQLB-CONT-PATH            VALUE    0.
      * DMS: a 'raw' or 'device' spec, tag size = one page
       78  SQLB-CONT-DISK-PAGE-TAG   VALUE    1.
      * DMS: a 'cooked file' -> file spec, tag size = one page
       78  SQLB-CONT-FILE-PAGE-TAG   VALUE    2.
      * DMS: a 'raw' or 'device' spec, tag size = one extent
       78  SQLB-CONT-DISK-EXTENT-TAG VALUE    5.
      * DMS: a 'cooked file' -> file spec, tag size = one extent
       78  SQLB-CONT-FILE-EXTENT-TAG VALUE    6.

      * pre-version 8 container types - kept for compatibility
       78  SQLB-CONT-DISK            value    1.
       78  SQLB-CONT-FILE            value    2.
       78  SQLB-CONT-STRIPED-DISK    value    5.
       78  SQLB-CONT-STRIPED-FILE    value    6.

      * provide information about the containers on which tablespaces are
      * based

       78  SQLB-TBSCONTQRY-DATA-SIZE value    292.
      * provide statistics on a tablespace
      * BETA NOTE: currently, we only provide stats on DMS tablespaces
      * SRSFIX: determine what SMS support to ship with V2
       01 SQLB-TBS-STATS.
      * DMS : Pages specified by CREATE TABLESPACE.
      * SMS : Pages of all files in the directories.
           05 SQL-TOTAL-PAGES        PIC 9(9) COMP-5.
      * DMS : totalPages - overhead
      * SMS : same as totalPages
           05 SQL-USEABLE-PAGES      PIC 9(9) COMP-5.
      * DMS : used and allocated pages in the pool
      * SMS : same as totalPages
           05 SQL-USED-PAGES         PIC 9(9) COMP-5.
      * DMS : useablePages - usedPages
      * SMS : N/A
           05 SQL-FREE-PAGES         PIC 9(9) COMP-5.
      * DMS : high water mark of used page number
      * SMS : N/A
           05 SQL-HIGH-WATER-MARK    PIC 9(9) COMP-5.

      * all tablespaces in database
       78  SQLB-OPEN-TBS-ALL         VALUE    0.
      * all tablespaces my agent is restoring
       78  SQLB-OPEN-TBS-RESTORE     VALUE    1.
      * all tablespaces where storage definition is allowed
       78  SQLB-OPEN-TBS-STORDEF     VALUE    2.
      * reserved
       78  SQLB-OPEN-TBS-RESERVED    VALUE    4.

      * constant used in place of tablespace id to indicate all table spaces
      * in db
       78  SQLB-ALL-TABLESPACES      VALUE    65534.

      * constants used for reserved parameters
       78  SQLB-RESERVED1            VALUE    65533.
       78  SQLB-RESERVED2            VALUE    65534.


       78  SQLB-SET-CONT-INIT-STATE  VALUE    0.
       78  SQLB-SET-CONT-FINAL-STATE VALUE    1.





      * SQL Return Codes in SQLCODE for UTILITY/CONFIGURATION Commands

      * If using the symbolic constants SQL-RC-OK and SQL-RC-INVALID-SQLCA
      * ensure sql.cbl is included.

      * ROLL FORWARD Return Codes - more in the 4900's

      * DB not enabled for Roll Fwd
       78  SQLU-RC-RFNOTEN           VALUE    -1260.
      * Roll Forward is not Pending
       78  SQLU-RC-RFNOTP            VALUE    -1261.
      * Bad Point in Time specified
       78  SQLU-RC-BADPIT            VALUE    -1262.
      * Invalid Log Extent file
       78  SQLU-RC-INVEXT            VALUE    -1263.
      * Log Extent file does not belong to DB
       78  SQLU-RC-NOTEXT            VALUE    -1264.
      * Log extent file is incorrect version
       78  SQLU-RC-IVREXT            VALUE    -1265.
      * Time specified is before previous roll-forward time
       78  SQLU-RC-PRTIME            VALUE    -1266.
      * User exit encountered an error (other than retry) while attempting
      * to retrieve a log extent file for roll forward
       78  SQLU-RC-UEXIT-ERR         VALUE    -1268.
      * Rollforward by tablespace in progress.
       78  SQLU-RC-RFINPRG           VALUE    -1269.
      * Roll forward complete but I/O errors encountered
       78  SQLU-RC-RCVIOERR          VALUE    1271.
      * Rollforward by tablespace has stopped.
       78  SQLU-RC-RFSTOP            VALUE    -1272.
      * Missing log extent file.
       78  SQLU-RC-MISSING-EXT       VALUE    -1273.
      * Invalid stop time for tablespace rollforward.
       78  SQLU-RC-INVTSP-STOPTIME   VALUE    -1274.
      * invalid stop time specified
       78  SQLU-INVRFR-STOPTIME      VALUE    -1275.
      * invalid time to issue a stop
       78  SQLU-INVRFR-STOP          VALUE    -1276.
      * Restore found one or more containers are inaccessable
       78  SQLUD-INACCESSABLE-CONTAINER VALUE 1277.
      * Roll forward complete but rollback required
       78  SQLU-RC-TBSROLLBK         VALUE    1278.
      * Restart complete but not all invalid  indexes were recreated
       78  SQL-RC-RECREATE-INDEXES   VALUE    1279.
      * invalid stop time specified for tablespace rollforward
       78  SQLU-INVRFR-TBSPITTIME    VALUE    -1280.

      * invalid client comm. protocols
       78  SQLF-RC-INV-CLIENT-COMM   VALUE    -1290.
      * invalid directory services field
       78  SQLF-RC-INV-DIR-FIELD     VALUE    -1296.

      * hostname in node dir unknown
       78  SQLCC-RC-UNKNOWN-HOST     VALUE    -1336.
      * service name in node directory is unknown
       78  SQLCC-RC-UNKNOWN-SERVICE  VALUE    -1337.
      * CPI-C symdestname is unknown
       78  SQLCC-RC-UNKNOWN-SYM-DEST-NAME VALUE -1338.
      * wrkstn name (nname)
       78  SQLCC-RC-NNAME-NOTFOUND   VALUE    -1341.
      * SOCKS env vars not found
       78  SQLCC-RC-NO-SOCKS-ENV-VAR VALUE    -1460.
      * socket address in use
       78  SQLCC-RC-SOCKADDR-IN-USE  VALUE    -5040.
      * general failure in servercommunications support
       78  SQLCC-RC-COMM-SUPPORT-FAILED VALUE -5042.

      * invalid discover type
       78  SQLF-RC-INV-DISCOVER-TYPE VALUE    -1480.
      * invalid discover comm
       78  SQLF-RC-INV-DISCOVER-COMM VALUE    -1481.

      * buffpage may be ignored
       78  SQLF-WARNING-BUFFPAGE     VALUE    1482.

      * GENERAL UTILITY Return Codes

      * Server does not support functionality
       78  SQLU-FUNCTION-NOT-SUPPORTED VALUE  -1651.
      * Group not defined on DLM
       78  SQLU-RECONCILE-GROUP-UNDEFINED VALUE 1161.
      * DLM down during exception
       78  SQLU-RECONCILE-EXCP-DLM-DOWN VALUE 1162.
      * DLM down during backup
       78  SQLU-BACKUP-DLM-DOWN      VALUE    1196.

      * Invalid output drive
       78  SQLU-BAD-DRIVE            VALUE    -2000.
      * Backup terminated by user
       78  SQLU-USER-TERM            VALUE    -2001.
      * database not local
       78  SQLU-NOT-LOCAL            VALUE    -2002.
      * base op system error
       78  SQLU-DOS-ERROR            VALUE    -2003.
      * SQL error occurred
       78  SQLU-SQL-ERROR            VALUE    -2004.
      * read wrong # of bytes
       78  SQLU-READ-ERROR           VALUE    -2005.
      * too few buffers for pagesize
       78  SQLU-BUFF-TOO-SMALL       VALUE    -2007.
      * call out of sequence
       78  SQLU-INVALID-ACTION       VALUE    -2008.
      * insufficient memory
       78  SQLU-INSUFF-MEMORY        VALUE    -2009.
      * error in Start Using
       78  SQLU-STRD-ERROR           VALUE    -2010.
      * error in Stop Using
       78  SQLU-STPD-ERROR           VALUE    -2011.
      * directory Scan error
       78  SQLU-DIR-ERROR            VALUE    -2013.
      * invalid database name
       78  SQLU-INVALID-DBNAME       VALUE    -2015.
      * invalid path in environment
       78  SQLU-INVALID-PATH         VALUE    -2016.
      * Start Session failed
       78  SQLU-START-SESSION        VALUE    -2017.
      * invalid authorizations
       78  SQLU-INVALID-AUTHS        VALUE    -2018.
      * auto-binding failed
       78  SQLU-AUTOBIND             VALUE    -2019.
      * conflict after auto-bind control file
       78  SQLU-TIMESTAMP            VALUE    -2020.
      * I/O error in accessing the log
       78  SQLU-IO-ERROR-LFH         VALUE    -2023.
      * I/O error in accessing the Backup/Restore flag file
       78  SQLU-IO-ERROR-BRG         VALUE    -2024.
      * System I/O error occurred
       78  SQLU-IO-ERROR             VALUE    -2025.
      * error in PAUSE the started DB
       78  SQLU-PAUSE-ERROR          VALUE    -2026.
      * error in CONTINUE the stoped DB
       78  SQLU-CONT-ERROR           VALUE    -2027.
      * interruption installing error
       78  SQLU-INT-INST-ERR         VALUE    -2028.
      * user exit returned non-zero rc
       78  SQLU-UEXIT-RC             VALUE    -2029.

      * warning to mount tape
       78  SQLU-FIRST-TAPE-WARNING   VALUE    2031.
      * parameter to utility incorrect
       78  SQLU-INVALID-PARM         VALUE    -2032.
      * TSM reported error
       78  SQLU-TSM-ERROR            VALUE    -2033.
      * address of parameter incorrect
       78  SQLU-INVALID-PARM-ADDRESS VALUE    -2034.
      * error during nointerrupt action
       78  SQLU-NOINT-ERROR          VALUE    -2035.
      * directory does not exist
       78  SQLU-PATH-ERROR           VALUE    -2036.
      * unable to load TSM
       78  SQLU-LOAD-TSM-ERROR       VALUE    -2037.
      * database system error
       78  SQLU-DBSYSTEM-ERROR       VALUE    -2038.
      * application terminated
       78  SQLU-NO-APP-ERROR         VALUE    -2039.
      * alias parameter error
       78  SQLU-ALIAS-ERROR          VALUE    -2040.
      * buff_size parameter error
       78  SQLU-BUFFSIZE-ERROR       VALUE    -2041.
      * I/O error during change tape
       78  SQLU-IO-WARNING           VALUE    2042.
      * Spawn child process error
       78  SQLU-SPAWN-EDU-ERROR      VALUE    -2043.
      * Message queue error
       78  SQLU-QUEUE-ERROR          VALUE    -2044.
      * Object access error
       78  SQLU-OBJECT-ACCESS-ERROR  VALUE    -2048.
      * Bad backup image
       78  SQLU-CORRUPT-IMAGE-ERROR  VALUE    -2054.
      * Unable to access memory set
       78  SQLU-MEMORY-ACCESS-ERROR  VALUE    -2055.
      * device path point to unknown device type
       78  SQLU-UNKNOWN-MEDIA-TYPE   VALUE    -2056.
      * device or file already opened by other process
       78  SQLU-MEDIA-CANNOT-BE-SHARED VALUE  -2057.
      * End of tape or file encountered during read
       78  SQLU-END-OF-MEDIA-WARNING VALUE    2058.
      * Device is full during write
       78  SQLU-DEVICE-FULL-WARNING  VALUE    2059.
      * Empty device or file not found during read
       78  SQLU-MEDIA-EMPTY-WARNING  VALUE    2060.
      * Access denied due to authority level
       78  SQLU-MEDIA-ACCESS-DENIED  VALUE    -2061.
      * Access error
       78  SQLU-MEDIA-ACCESS-ERROR   VALUE    -2062.
      * Terminate last Media IO.
       78  SQLU-TERM-LAST-MEDIA-WARNING VALUE 2065.
      * Invalid tablespace name
       78  SQLU-BAD-TABLESPACE-NAME  VALUE    -2066.
      * Could not locate media header in backup or copy image
       78  SQLU-NO-MEDIA-HEADER      VALUE    -2068.
      * Mismatch alias name from media
       78  SQLU-INCORRECT-DBALIAS    VALUE    -2069.
      * Mismatch timestamp read from media
       78  SQLU-INCORRECT-TIMESTAMP  VALUE    -2070.

      * Shared library access error
       78  SQLU-SHR-LIB-ACCESS-ERROR VALUE    -2071.
      * Bind shared library error
       78  SQLU-BIND-SHR-LIB-ERROR   VALUE    -2072.

      * Error at DB2 or DLFM
       78  SQLU-DATALINK-INTERNAL-ERROR VALUE -2073.
      * Error at DB2 end
       78  SQLU-DATALINK-DB-ERROR    VALUE    -2074.
      * Error at DLFM end
       78  SQLU-DATALINK-DLFM-ERROR  VALUE    -2075.
      * Unable to register DLM Server
       78  SQLU-REGISTER-DLFM-WARNING VALUE   2076.
      * DLMs down during reconcile
       78  SQLU-RECONCILE-DLM-PEND-WARN VALUE 2077.
      * Shared library vendor API error
       78  SQLU-SHR-LIB-VENDOR-API-ERROR VALUE -2079.
      * Out of sequence tablespace restore
       78  SQLUD-TSP-RESTORE-OUT-OF-SEQ VALUE -2154.

      * ARCHIVE LOG Return Codes
      * Archive log error.
       78  SQLU-ARCHIVELOG-ERROR     VALUE    -1259.
      * Database is not in recoverable mode.
       78  SQLU-ARCHIVELOG-NONRECOV-DB VALUE  -2417.

      * HISTORY TABLE Return Codes
      * Changes were made to historyfile during update.
       78  SQLUH-SCAN-UPDATED-WARNING VALUE   2155.
      * MAX # open scans exceeded
       78  SQLUH-MAX-SCANS-EXCEEDED  VALUE    -2157.
      * History file is unfixable
       78  SQLUH-DAMAGED-FILE        VALUE    -2161.
      * A log file does not have read/write permission
       78  SQLU-INV-PERM-LOGFILE     VALUE    -2162.
      * No suitable backup image could be found for use in the recover
      * operation
       78  SQLU-RECOVER-NO-IMAGE-FOUND VALUE  -2163.
      * RECOVER could not locate a history file
       78  SQLU-RECOVER-FILE-NOT-FOUND VALUE  -2164.
      * number of tablespaces changed
       78  SQLUH-SQLUHINFO-VARS-WARNING VALUE 2165.
      * RECOVER failed because database does not exist
       78  SQLU-RECOVER-DB-NOT-FOUND VALUE    -2166.
      * Duplicate timestamp found
       78  SQLUH-DUPLICATE-ENTRY     VALUE    -2170.
      * Entry not found on update
       78  SQLUH-ENTRY-NOT-FOUND     VALUE    -2171.
      * Access history file warning
       78  SQLU-ACCESS-HIST-WARNING  VALUE    2172.

      * PRUNE command return codes
      * PRUNE LOGFILE not allowed in this database configuration
       78  SQLU-PRUNE-LOG-NOT-ALLOWED VALUE   -1206.

      * REORGANIZE TABLE Return Codes
      * invalid authid on index name
       78  SQLUR-INVALID-AUTHID      VALUE    -2200.
      * invalid tablename syntax
       78  SQLUR-INVALID-TABLENAME   VALUE    -2203.
      * invalid tablename syntax
       78  SQLUR-INVALID-INDEXNAME   VALUE    -2204.
      * index doesn't exist
       78  SQLUR-INDEX-NOT-FOUND     VALUE    -2205.
      * invalid filepath pointer
       78  SQLUR-INVALID-FILEPATH    VALUE    -2207.
      * invalid tablespace pointer
       78  SQLUR-INVALID-TABLESPACE  VALUE    -2208.
      * table does not exist
       78  SQLUR-TABLE-NOT-FOUND     VALUE    -2211.
      * cannot reorg a view
       78  SQLUR-VIEW-ERROR          VALUE    -2212.
      * Incorrect tablespace type
       78  SQLUR-INCORRECT-TABLESPACE VALUE   -2213.
      * insuffic authority
       78  SQLUR-INSAUTH             VALUE    -2214.
      * SQL error commiting prev work
       78  SQLUR-SQLERR-COMPREV      VALUE    -2215.
      * SQL error during reorganization
       78  SQLUR-SQLERR-REORG        VALUE    -2216.
      * Invalid temp tablespace for reorg table
       78  SQLUR-INV-TEMP            VALUE    -2217.
      * Incompatible options specified for reorg table
       78  SQLUR-INCOMPAT-OPTS       VALUE    -2218.
      * Invalid action for reorg table INPLACE
       78  SQLUR-INV-ACTION          VALUE    -2219.

      * RUN STATISTICS Return Codes
      * invalid authid
       78  SQLUS-INVALID-AUTHID      VALUE    -2300.
      * invalid table name
       78  SQLUS-INVALID-TABLE-NAME  VALUE    -2301.
      * invalid index pointer
       78  SQLUS-INVALID-INDEX-LIST  VALUE    -2302.
      * statsopt parameter invalid
       78  SQLUS-INVALID-STATS-OPT   VALUE    -2303.
      * sharelev parameter invalid
       78  SQLUS-INVALID-SHARE-LEV   VALUE    -2304.
      * table specified is a view
       78  SQLUS-VIEWS-NOT-ALLOWED   VALUE    -2305.
      * object doesn't exist
       78  SQLUS-OBJ-DOES-NOT-EXIST  VALUE    -2306.
      * system table not allowed
       78  SQLUS-SYS-TABLE-NOT-ALLOWED VALUE  -2307.
      * index authid invalid
       78  SQLUS-INVALID-INDEX-AUTHID VALUE   -2308.
      * index name invalid
       78  SQLUS-INVALID-INDEX-NAME  VALUE    -2309.
      * error running statistics
       78  SQLUS-ERROR-STAT          VALUE    -2310.
      * insuffic authority for runstats
       78  SQLUS-INSAUTH             VALUE    -2311.
      * statistics heap is too small
       78  SQLUS-STATS-HEAP-TOO-SMALL VALUE   -2312.
      * incomplete statistics collected
       78  SQLUS-PARTIAL-SUCCESS     VALUE    2313.
      * Statistics are in an inconsistent state
       78  SQLUS-INCONSISTENT-STATS  VALUE    2314.

      * Rst DB turn off roll fwd pend, old OS/2 API only
       78  SQLUD-RST-NOROLLFWD       VALUE    3.
      * Old OS/2 Backup calling action values
      * backup entire database
       78  SQLU-BACK-ALL             VALUE    0.
      * backup changes only
       78  SQLU-BACK-CHANGES         VALUE    1.
      * quiesce during backup
       78  SQLU-BACK-QUIESCE         VALUE    2048.
      * New UOW return error immed
       78  SQLU-NEW-UOW-RETURN-ERROR VALUE    1024.

      * pause or Continue or migration error
       78  SQLU-DBM-ERROR            VALUE    -2014.
      * Wrong diskette inserted
       78  SQLU-WRONG-DISKETTE       VALUE    -2021.
      * a specific fixed disk is full
       78  SQLU-DISK-FULL            VALUE    -2030.

      * invalid type
       78  SQLUB-BAD-TYPE            VALUE    -2400.
      * type must be database
       78  SQLUB-TYPE-MUSTBE-DB      VALUE    -2401.
      * insufficient auth for backup
       78  SQLUB-USER-NOT-AUTH       VALUE    -2403.
      * The target disk/diskette is full
       78  SQLUB-MEDIA-FULL          VALUE    -2404.
      * The Backup can't run because B/R flag file existed and read I/O
      * error occurred
       78  SQLUB-BR-INPROG           VALUE    -2407.
      * Backup process is successful, but unable to delete BRG file
       78  SQLUB-DEL-BRG-ERR         VALUE    2408.
      * 'change-only' is only applied to the most recent base image
       78  SQLUB-WRONG-BASE-IMAGE    VALUE    -2409.
      * change only, backup not allowed to user exit
       78  SQLUB-NO-CHG-NSD          VALUE    -2410.
      * I/O error during backup
       78  SQLUB-IO-ERROR            VALUE    -2411.
      * Log not truncated during backup
       78  SQLUB-LOG-NOT-TRUNC       VALUE    2425.
      * Restore utility not bound
       78  SQLUD-BIND-WARNING        VALUE    2507.
      * error dropping database
       78  SQLUD-DROP-ERROR          VALUE    -2511.

      * BACKUP Return Codes
      * The backup can't run because the Restore is in progress
       78  SQLUB-RST-IN-PROG         VALUE    -2405.
      * The Backup can't run because roll forward is pending
       78  SQLUB-ROLLFWD-PENDING     VALUE    -2406.
      * data page encountered during backup is corrupted
       78  SQLUB-CORRUPT-PAGE        VALUE    -2412.
      * retain req'd for online backup
       78  SQLUB-LOGRETAIN-ONLINE-ERROR VALUE -2413.
      * no space for control info
       78  SQLUB-INSUF-SPACE-WARNING VALUE    2414.
      * tape full, mount another
       78  SQLUB-NEXT-TAPE-WARNING   VALUE    2416.
      * database does not exist
       78  SQLUB-DBASE-DOES-NOT-EXIST VALUE   -2418.
      * disk full during backup
       78  SQLUB-DISK-FULL-ERROR     VALUE    -2419.

      * first tape cannot hold header
       78  SQLUB-FIRST-TAPE-ERROR    VALUE    -2420.
      * retain req'd for tbs backup
       78  SQLUB-LOGRETAIN-TBS-ERROR VALUE    -2421.
      * the backup API used does not support this format of database
       78  SQLUB-DB-UNSUPPORTED-BY-API VALUE  -2422.
      * A required index is missing during an offline backup
       78  SQLUB-MISSING-INDEX       VALUE    -2423.
      * Copy operation at DLFMend is still in progress
       78  SQLUB-COPY-IN-PROGRESS    VALUE    -2424.
      * TRACKMOD req'd for incremental backup
       78  SQLUB-TRACKMOD-INCR-ERROR VALUE    -2426.
      * Saved library is missing an entry point
       78  SQLUB-MISSING-ENTRYPOINT  VALUE    -2427.
      * Failed to backup requested logfile
       78  SQLUB-BACKUP-LOGS-ERROR   VALUE    -2428.

      * RESTORE Return Codes
      * can't read restored database
       78  SQLUD-CANNOT-RESTORE      VALUE    -2501.
      * error reading backup diskette
       78  SQLUD-DISKETTE-ERROR      VALUE    -2502.
      * wrong backup diskette
       78  SQLUD-WRONG-DATABASE      VALUE    -2503.
      * prompt for backup diskette
       78  SQLUD-DISKETTE-PROMPT     VALUE    2504.
      * warn that drop will be done
       78  SQLUD-DROP-WARNING        VALUE    2505.
      * Restore worked, but not cleanup
       78  SQLUD-DATABASE-WARNING    VALUE    2506.
      * timestamp incorrectly specified
       78  SQLUD-INVALID-TIMESTAMP   VALUE    -2508.
      * invalid database drive
       78  SQLUD-INVALID-DBDRV       VALUE    -2509.
      * semaphore error
       78  SQLUD-SEM-ERROR           VALUE    -2510.
      * error creating database
       78  SQLUD-CREATE-ERROR        VALUE    -2512.
      * error renaming database
       78  SQLUD-REN-ERROR           VALUE    -2513.
      * restored database wrong version
       78  SQLUD-BAD-VERSION         VALUE    -2514.
      * insuffic authority to restore
       78  SQLUD-INSAUTH             VALUE    -2515.
      * a database active
       78  SQLUD-DBACT               VALUE    -2516.
      * database migrated
       78  SQLUD-MIGRATED            VALUE    2517.
      * error in restoring DB config.
       78  SQLUD-RST-DBCONG-ERR      VALUE    -2518.
      * error in migrating the database
       78  SQLUD-MIGRATE-ERROR       VALUE    -2519.
      * DBCON file is restored using the backup version
       78  SQLUD-DBCON-WARN          VALUE    2520.
      * Restore process is successful, but unable to delete BRG file
       78  SQLUD-DEL-BRG-ERR         VALUE    2521.
      * more than one file match
       78  SQLUD-TOO-MANY-BACKUP-FILES VALUE  -2522.
      * corrupted backup image
       78  SQLUD-MEDIA-CORRUPT       VALUE    -2530.
      * image of wrong database
       78  SQLUD-WRGIMAGE-ERROR      VALUE    -2532.
      * image of wrong database
       78  SQLUD-WRGIMAGE-WARNING    VALUE    2533.
      * seq number of backup incorrect
       78  SQLUD-WRONGSEQ-WARNING    VALUE    2536.
      * roll forward required
       78  SQLUD-MUST-ROLLFWD        VALUE    -2537.
      * end of file reached unexpectedly
       78  SQLUD-UNEXPECT-EOF-ERROR  VALUE    -2538.
      * noint type restore had warning
       78  SQLUD-NOINT-WARNING       VALUE    2540.
      * unable to close backup file
       78  SQLUD-CLOSE-MEDIA-WARNING VALUE    2541.
      * no backup file match found
       78  SQLUD-NO-BACKUP-FILE-MATCH VALUE   -2542.
      * invalid directory for new dbase
       78  SQLUD-DB-DIR-ERROR        VALUE    -2543.
      * disk full during restore
       78  SQLUD-DISK-FULL-ERROR     VALUE    -2544.
      * restore requires first image first
       78  SQLUD-NOT-FIRST-IMAGE     VALUE    -2546.
      * cannot restore online backup from a previous release
       78  SQLUD-OLD-ONLINE-IMAGE-ERROR VALUE -2547.
      * backup has diff codepage from disk DB
       78  SQLUD-IMAGE-DB-CP-MISMATCH VALUE   -2548.
      * all tablespaces skipped during restore
       78  SQLUD-ALL-TBLSPACES-SKIPPED VALUE  -2549.
      * backup from a different node
       78  SQLUD-WRONG-NODE          VALUE    -2550.
      * backup of a database with a different catalog node
       78  SQLUD-WRONG-CAT-NODE      VALUE    -2551.
      * Report filename length >255
       78  SQLUD-REPORTFILE-ERROR    VALUE    -2552.
      * Reconcile Genereic Error
       78  SQLU-RECONCILE-GENERIC-ERROR VALUE -2554.

      *************************************************************************
      *  the meaning of the following 8 warnings :
      *        A -> database alias
      *        N -> database name
      *        S -> database seed
      *        0 -> target db value DOES NOT matches backup image value
      *        1 -> target db value matches backup image value
      * 
      **************************************************************************
       78  SQLUD-A0N0S0-WARNING      VALUE    2529.
       78  SQLUD-A0N0S1-WARNING      VALUE    2528.
       78  SQLUD-A0N1S0-WARNING      VALUE    2525.
       78  SQLUD-A0N1S1-WARNING      VALUE    2524.
       78  SQLUD-A1N0S0-WARNING      VALUE    2527.
       78  SQLUD-A1N0S1-WARNING      VALUE    2526.
       78  SQLUD-A1N1S0-WARNING      VALUE    2523.
       78  SQLUD-A1N1S1-WARNING      VALUE    2539.
      * restore tablespace to a non original database.
       78  SQLUD-TBLSP-TO-OTHER-DB   VALUE    -2560.
      * restore tablespace to a new data base.
       78  SQLUD-TBLSP-TO-NEW-DB     VALUE    2561.
      * restore tablespace from full backup image.
       78  SQLUD-TBLSP-FR-FULL-BKUP  VALUE    -2562.
      * Not all tablespases were restored
       78  SQLUD-NOTALL-TBS-RESTORED VALUE    2563.
      * Mismatch between restore image and database restored to
       78  SQLUD-DB-MISMATCH         VALUE    2565.
      * Tables in tablespaces are in DRP/RNP state
       78  SQLUD-TBS-DATALINK-PENDING VALUE   2566.
      * Image is being restored on the wrong platform
       78  SQLUD-WRONG-PLATFORM      VALUE    -2570.
      * Problem on first media write
       78  SQLU-HEADER-WRITE-ERR     VALUE    2045.
      * Complete incremental history not available
       78  SQLUD-INCR-HISTORY-ERROR  VALUE    -2571.
      * Incremental restore out of sequence
       78  SQLUD-INCR-RESTORE-OUT-OF-SEQ VALUE -2572.
      * Incremental image being restored non-incrementally
       78  SQLUD-NON-INCR-RESTORE    VALUE    -2573.
      * Incremental image being restored is newer than target image
       78  SQLUD-INCR-TOO-NEW        VALUE    -2574.
      * Incremental image being restored is older than previous image
       78  SQLUD-INCR-TOO-OLD        VALUE    -2575.
      * Incremental restore is missing "incremental" clause
       78  SQLUD-MISSING-INCR-CLAUSE VALUE    -2576.
      * No decompression library was found for this restore operation
       78  SQLUD-NO-DECOMPR-LIBRARY  VALUE    -2577.
      * Specified object was not found in restore image
       78  SQLUD-OBJ-NOT-FOUND       VALUE    -2578.
      * Restore completed but error extracting logs.
       78  SQLUD-LOGS-RESTORE-WARNING VALUE   2580.
      * Restore only logfiles failed.
       78  SQLUD-LOGS-RESTORE-ERROR  VALUE    -2581.

      * sqlgadau and sqluadau Return codes
      * sql_authorizations parm is bad
       78  SQLUA-BAD-INPUT-PARAMETER VALUE    -2600.

      * DB2SPLIT Return codes
      * invalid command line options
       78  SQLUSP-CMD-LINE-OPT-ERR   VALUE    -2701.
      * fail to open config file
       78  SQLUSP-OPEN-CFG-FILE-ERR  VALUE    -2702.
      * fail to open log file
       78  SQLUSP-OPEN-LOG-FILE-ERR  VALUE    -2703.
      * fail to open input data file
       78  SQLUSP-OPEN-IN-DATA-FILE-ERR VALUE -2704.
      * fail to open input partition map file
       78  SQLUSP-OPEN-INPUT-MAP-FILE-ERR VALUE -2705.
      * fail to open output partition map file
       78  SQLUSP-OPEN-OUTMAP-FILE-ERR VALUE  -2706.
      * fail to open distribution file
       78  SQLUSP-OPEN-DIST-FILE-ERR VALUE    -2707.
      * fail to open output data file
       78  SQLUSP-OPEN-OUTDATA-FILE-ERR VALUE -2708.
      * syntax error in config file
       78  SQLUSP-CFG-SYNTAX-ERR     VALUE    -2709.
      * invalid keyword in config file
       78  SQLUSP-INVALID-CFG-KEYWORD VALUE   -2710.
      * column delimiter can't be a blank
       78  SQLUSP-INVALID-COL-DELIMITER VALUE -2711.
      * string delimiter can't be a period
       78  SQLUSP-INVALID-STR-DELIMITER VALUE -2712.
      * invalid run type in config file
       78  SQLUSP-INVALID-RUNTYPE    VALUE    -2713.
      * invalid Message Level in config file
       78  SQLUSP-INVALID-MSG-LEVEL  VALUE    -2714.
      * invalid Check Level in config file
       78  SQLUSP-INVALID-CHK-LEVEL  VALUE    -2715.
      * record length out of range
       78  SQLUSP-INVALID-REC-LEN    VALUE    -2716.
      * invalid node specification
       78  SQLUSP-INVALID-NODE       VALUE    -2717.
      * invalid output node specification
       78  SQLUSP-INVALID-OUTPUTNODE VALUE    -2718.
      * invalid output type
       78  SQLUSP-INVALID-OUTPUTTYPE VALUE    -2719.
      * too many partitioning keys
       78  SQLUSP-TOO-MANY-PTITN-KEYS VALUE   -2720.
      * invalid partition key specification
       78  SQLUSP-INVALID-PTITN-KEYS VALUE    -2721.
      * invalid log file specification
       78  SQLUSP-INVALID-LOG-FILE   VALUE    -2722.
      * invalid trace specification
       78  SQLUSP-INVALID-TRACE      VALUE    -2723.
      * specify one and only one: MAPFILI or NODE
       78  SQLUSP-NODE-ERR           VALUE    -2724.
      * Output partition map is needed
       78  SQLUSP-NO-OUTMAP          VALUE    -2725.
      * no partitioning key defined
       78  SQLUSP-NO-PTITN-KEY       VALUE    -2726.
      * key exceeds record length
       78  SQLUSP-KEY-OUT-RANGE      VALUE    -2727.
      * output node list is not a subset of node list
       78  SQLUSP-NODE-NOT-EXISTED   VALUE    -2728.
      * invalid data entry in input map
       78  SQLUSP-INPUT-MAP-ERR      VALUE    -2729.
      * error writing header of out data file
       78  SQLUSP-WRITE-HEAD-ERR     VALUE    -2730.
      * error processing input data file
       78  SQLUSP-DATA-READ-ERR      VALUE    -2731.
      * binary data if VMMVS
       78  SQLUSP-DATA-BIN-ERR       VALUE    -2732.
      * run type not specified
       78  SQLUSP-NO-RUNTYPE         VALUE    -2733.
      * 32kLimit definition error
       78  SQLUSP-32KLIMIT-DEF-ERR   VALUE    -2734.
      * discard empty record
       78  SQLUSP-DISCARD-REC-WARN   VALUE    2735.
      * error from sqlugrpi or sqlugrpn
       78  SQLUSP-GRPI-ERR           VALUE    -2736.
      * error writing data file
       78  SQLUSP-DATA-WRITE-ERR     VALUE    -2737.
      * data is truncated in writing
       78  SQLUSP-DATA-WRITE-WARN    VALUE    2738.
      * reclen must be defined for BIN
       78  SQLUSP-BIN-NO-RECLEN      VALUE    -2739.
      * FLOAT is not supported for DEL/ASC
       78  SQLUSP-FLOAT-NOT-ALLOWED  VALUE    -2740.
      * invalid file type
       78  SQLUSP-FILETYPE-DEF-ERR   VALUE    -2741.
      * decimal len not match its precision
       78  SQLUSP-DECIMAL-LEN-NOT-MATCH VALUE -2742.
      * len not match for binary type data
       78  SQLUSP-DATA-LEN-NOT-MATCH VALUE    -2743.
      * illegal filename in cfg file
       78  SQLUSP-ILLEGAL-FILENAME   VALUE    -2744.
      * Invalid NEWLINE flag in cfg file
       78  SQLUSP-NEWLINE-DEF-ERR    VALUE    -2745.
      * Incomplete record in input data file
       78  SQLUSP-INCOMPLETE-RECORD  VALUE    -2746.
      * ASC record must be no longer than 32K
       78  SQLUSP-RECORD-TOO-LONG    VALUE    -2747.
      * ASC record not long enough
       78  SQLUSP-RECORD-TOO-SHORT   VALUE    -2748.
      * partition key not in the first 32k bytes of the record.
       78  SQLUSP-KEY-NOT-IN-32K     VALUE    -2749.
      * line too long in cfg file
       78  SQLUSP-CFG-LINE-TOO-LONG  VALUE    -2750.
      * expected reclen not matching actual reclen
       78  SQLUSP-REC-LEN-ERR        VALUE    -2751.
      * invalid codepage specification
       78  SQLUSP-INVALID-CODEPAGE   VALUE    -2752.
      * failed to get application CP
       78  SQLUSP-APP-CODEPAGE-ERR   VALUE    -2753.
      * codepages not convertable
       78  SQLUSP-CODEPAGE-NOTABLE   VALUE    -2754.
      * codepage-related delimiter error
       78  SQLUSP-DELIMITER-ERROR    VALUE    -2755.
      * error converting data to DB CP
       78  SQLUSP-CP-DATA-TO-DB      VALUE    -2756.
      * binary numerics not allowed in EBCDIC data
       78  SQLUSP-EBCDIC-NO-BIN      VALUE    -2757.
      * DB2GPMAP Return Codes
      * Specify only tbl or nodegrp
       78  SQLUGPMAP-TBL-AND-NDGRP   VALUE    -2761.
      * fail to find DB install path
       78  SQLUGPMAP-NO-INST-PATH    VALUE    -2762.
      * tbl not found
       78  SQLUGPMAP-TBL-NOT-FOUND   VALUE    -2763.
      * nodegrp not found
       78  SQLUGPMAP-NODEGRP-NOT-FOUND VALUE  -2764.
      * fail to open file
       78  SQLUGPMAP-OPEN-OUTMAPFILE-WARN VALUE 2765.
      * incorrect ptitn map size
       78  SQLUGPMAP-PTITN-MAP-ERR   VALUE    -2766.
      * invalid cmd line option
       78  SQLUGPMAP-INVALID-CMD-OPT VALUE    -2767.
      * IMPORT/EXPORT Return codes
      * error opening output file
       78  SQLUE-DFO                 VALUE    -3001.
      * i/o error writing output file
       78  SQLUE-IOE                 VALUE    -3002.
      * i/o error closing output file
       78  SQLUE-CLS                 VALUE    -3003.
      * invalid filetype parameter
       78  SQLUE-IFT                 VALUE    -3004.
      * function interrupted
       78  SQLUE-CBI                 VALUE    -3005.
      * i/o error opening message file
       78  SQLUE-MFO                 VALUE    -3006.
      * i/o error writing message file
       78  SQLUE-MFW                 VALUE    -3007.
      * start using database failed
       78  SQLUE-STA                 VALUE    -3008.
      * invalid tcolstrg
       78  SQLUE-STR                 VALUE    -3009.
      * invalid dcoldata
       78  SQLUE-COL                 VALUE    -3010.

      * memory allocation error
       78  SQLUE-MEM                 VALUE    -3011.
      * system error
       78  SQLUE-SYSERR              VALUE    -3012.
      * invalid filetmod
       78  SQLUE-FTMOD               VALUE    -3013.
      * failure on closing message file
       78  SQLUE-MFC                 VALUE    -3014.
      * SQL error occurred
       78  SQLUE-SQLERR              VALUE    -3015.
      * no keywords found
       78  SQLUE-FMODNK              VALUE    -3016.
      * invalid delimiter or duplicate
       78  SQLUE-FMODID              VALUE    -3017.
      * decimal used for char delimiter
       78  SQLUE-FMODDEC             VALUE    -3018.
      * no tcolstrg
       78  SQLUE-NTS                 VALUE    -3019.
      * insufficient authority for exp.
       78  SQLUE-RC-INSAUTH          VALUE    -3020.

      * insufficient authority for imp.
       78  SQLUI-RC-INSAUTH          VALUE    -3021.
      * SQL error on input string
       78  SQLUE-SQL-PREP-ERR        VALUE    -3022.
      * invalid database name
       78  SQLUE-DATABASE            VALUE    -3023.
      * invalid datafile
       78  SQLUE-DATAFILE            VALUE    -3025.
      * invalid msgfile
       78  SQLUE-MSGFILE             VALUE    -3026.
      * Export method indicator not n/d
       78  SQLUE-DCOLMETH            VALUE    -3028.
      * filetype is null
       78  SQLUE-NUL-FTYPE           VALUE    -3029.

      * error opening input data file
       78  SQLUI-DFO                 VALUE    -3030.
      * i/o error reading input file
       78  SQLUI-IOE                 VALUE    -3031.
      * Import method not n/d/p
       78  SQLUI-DCOLMETH            VALUE    -3032.
      * invalid insert in tcolstrg
       78  SQLUI-TINSERT             VALUE    -3033.
      * invalid into in tcolstrg
       78  SQLUI-TINTO               VALUE    -3034.
      * invalid tablename in tcolstrg
       78  SQLUI-TABLENAME           VALUE    -3035.
      * close paren not in tcolstrg
       78  SQLUI-CPAREN              VALUE    -3036.
      * SQL error on insert string
       78  SQLUE-SQL-PREP-INSERT     VALUE    -3037.
      * tcolstrg invalid
       78  SQLUI-TCOLJUNK            VALUE    -3038.
      * load parallelism reduced
       78  SQLU-REDUCE-CPUPAR        VALUE    3039.
      * lob file error
       78  SQLUE-LOBFILE-ERROR       VALUE    -3040.

      * LINKTYPE is not URL
       78  SQLUI-DL-ILLEGAL-LINKTYPE VALUE    -3042.
      * dl_specification invalid
       78  SQLUI-DL-COL-JUNK         VALUE    -3043.
      * multiple prefix decl per col
       78  SQLUI-DL-COL-DUP-PREFIX   VALUE    -3044.

      * invalid dcol position for CSV
       78  SQLUIC-BAD-DCOL-POS       VALUE    -3045.
      * non-default dcol and no cols
       78  SQLUI-NONDEF-DCOL-NOCOLS  VALUE    -3046.
      * dcolinfo has invalid method
       78  SQLUI-BAD-DCOL-METH       VALUE    -3047.
      * non nullable column
       78  SQLUI-NODCOL-FOR-NONNULL-DBCOL VALUE -3048.
      * unsupported column type
       78  SQLUIC-UNSUPTYP-NONULLS   VALUE    -3049.

      * conversion for cdpg
       78  SQLUII-CONVERSION         VALUE    3050.
      * too many rows for WSF export
       78  SQLUE-ROWCT-TOOBIG        VALUE    -3053.
      * eof reading first rec in IXF
       78  SQLUII-HEOF               VALUE    -3054.
      * length of 'H' rec not numeric
       78  SQLUII-HLEN-CONV          VALUE    -3055.
      * first record too short
       78  SQLUII-HLEN-SHORT         VALUE    -3056.
      * first IXF rec is not 'H'
       78  SQLUII-HTYP               VALUE    -3057.
      * no IXF identifier in 'H'
       78  SQLUII-HID                VALUE    -3058.
      * invalid version field in 'H'
       78  SQLUII-HVERS              VALUE    -3059.

      * HCNT in 'H' not numeric
       78  SQLUII-HCNT               VALUE    -3060.
      * SBCP in 'H' not numeric
       78  SQLUII-HSBCP-BAD          VALUE    -3061.
      * DBCP in 'H' not numeric
       78  SQLUII-HDBCP-BAD          VALUE    -3062.
      * 'H' SBCP not compat w/data SBCP
       78  SQLUII-HSBCP-CMP          VALUE    -3063.
      * 'H' DBCP not compat w/data DBCP
       78  SQLUII-HDBCP-CMP          VALUE    -3064.
      * can't get codepages
       78  SQLUII-DB-CODEPG          VALUE    -3065.
      * eof reading/looking for 'T' rec
       78  SQLUII-TEOF               VALUE    -3066.
      * length of 'T' rec not numeric
       78  SQLUII-TLEN-CONV          VALUE    -3067.
      * 'T' record is too short
       78  SQLUII-TLEN-SHORT         VALUE    -3068.
      * first non-'A' rec not 'T' rec
       78  SQLUII-TTYP               VALUE    -3069.

      * invalid rec length of 'A' rec
       78  SQLUII-ALEN-BAD           VALUE    -3070.
      * invalid data convention in 'T'
       78  SQLUII-TCONV              VALUE    -3071.
      * invalid data format in 'T'
       78  SQLUII-TFORM              VALUE    -3072.
      * invalid machine form in 'T'
       78  SQLUII-TMFRM              VALUE    -3073.
      * invalid data location in 'T'
       78  SQLUII-TLOC               VALUE    -3074.
      * 'C' rec cnt in 'T' not numeric
       78  SQLUII-TCCNT              VALUE    -3075.
      * name len fld in 'T' not numeric
       78  SQLUII-TNAML              VALUE    -3076.
      * too many 'C' records
       78  SQLUII-CCNT-HIGH          VALUE    -3077.
      * length of 'A' rec not numeric
       78  SQLUII-ALEN-CONV          VALUE    -3078.
      * length of 'C' rec not numeric
       78  SQLUII-CLEN-CONV          VALUE    -3079.

      * 'C' record is too short
       78  SQLUII-CLEN-SHORT         VALUE    -3080.
      * wrong rec type / 'C' expected
       78  SQLUII-CTYP               VALUE    -3081.
      * EOF while processing 'C' recs
       78  SQLUII-CEOF               VALUE    -3082.
      * 'D' rec id field not numeric
       78  SQLUII-CDRID              VALUE    -3083.
      * 'D' rec posn field not numeric
       78  SQLUII-CPOSN              VALUE    -3084.
      * 'D' id/position not consistent
       78  SQLUII-CIDPOS             VALUE    -3085.
      * IXF column does not exist
       78  SQLUII-NOCREC-FOR-NONNULL-DBCO VALUE -3086.
      * IXF column not valid
       78  SQLUII-INVCREC-NONNULL-DBCOL VALUE -3087.
      * IXF column not compatible
       78  SQLUII-CRECCOMP-NONNULL-DBCOL VALUE -3088.
      * wrong rec type / 'D' expected
       78  SQLUII-DTYP               VALUE    -3089.

      * length of 'D' rec not numeric
       78  SQLUII-DLEN-CONV          VALUE    -3090.
      * length of 'D' rec not valid
       78  SQLUII-DLEN-RANGE         VALUE    -3091.
      * invalid id field in 'D' rec
       78  SQLUII-DID                VALUE    -3092.
      * import file not valid WSF
       78  SQLUIW-IFILE-INV          VALUE    -3093.
      * DOS non-nullable name not found
       78  SQLUIW-NNCOL-LOST         VALUE    -3094.
      * col position out of range
       78  SQLUIW-PCOL-INV           VALUE    -3095.
      * unsup col type - not nullable
       78  SQLUIW-UCOLTYP-NONUL      VALUE    -3096.
      * record length invalid
       78  SQLUIW-RECLEN-INV         VALUE    -3097.
      * row number out of range
       78  SQLUIW-INROW-INV          VALUE    -3098.
      * col number out of range
       78  SQLUIW-INCOL-INV          VALUE    -3099.

      * column longer than 254 chars
       78  SQLUE-COL-TOOBIG          VALUE    3100.
      * column has char delimiter
       78  SQLUE-DATA-CHARDEL        VALUE    3101.
      * dcol column nbr > tcol number
       78  SQLUE-DCNUM-HIGH          VALUE    3102.
      * dcol column nbr < tcol number
       78  SQLUE-DCNUM-LOW           VALUE    3103.
      * error formatting a message
       78  SQLUE-MFE                 VALUE    3106.
      * warning message issued
       78  SQLUE-WARNING             VALUE    3107.

      * file not linked
       78  SQLUI-DLFM-LINK           VALUE    3108.

      * extra database cols
       78  SQLUI-FEWER-DCOLS-DBCOLS-NULLE VALUE 3112.
      * column will be nulled
       78  SQLUIC-UNSUPTYP-NULLABLE  VALUE    3113.
      * character ignored
       78  SQLUIC-IGNORED-CHAR       VALUE    3114.
      * input CSV field too long
       78  SQLUIC-FIELD-TOO-LONG     VALUE    3115.
      * field value missing
       78  SQLUIC-CF-REQFIELD-MISSING VALUE   3116.
      * non NULL found for GENERATED ALWAYWS col
       78  SQLUIC-CF-GENALWAYS-NOTNULL VALUE  3550.
      * smallint field nulled
       78  SQLUIC-CFINT2-NULLED      VALUE    3117.
      * smallint field error
       78  SQLUIC-CFINT2-ROWREJ      VALUE    3118.
      * int field nulled
       78  SQLUIC-CFINT4-NULLED      VALUE    3119.

      * int field error
       78  SQLUIC-CFINT4-ROWREJ      VALUE    3120.
      * float field nulled
       78  SQLUIC-CFFLOAT-NULLED     VALUE    3121.
      * float field error
       78  SQLUIC-CFFLOAT-ROWREJ     VALUE    3122.
      * decimal field nulled
       78  SQLUIC-CFDEC-NULLED       VALUE    3123.
      * decimal field error
       78  SQLUIC-CFDEC-ROWREJ       VALUE    3124.
      * char field truncated
       78  SQLUIC-CFTRUNC            VALUE    3125.

      * char field nulled
       78  SQLUIC-CFCHAR-NULLED      VALUE    3126.
      * char field error
       78  SQLUIC-CFCHAR-ROWREJ      VALUE    3127.
      * date field truncated
       78  SQLUIC-CFDATETRUNC        VALUE    3128.
      * date/time/stamp field padded
       78  SQLUIC-CFDTPAD            VALUE    3129.

      * time field truncated
       78  SQLUIC-CFTIMETRUNC        VALUE    3130.
      * stamp field truncated
       78  SQLUIC-CFSTAMPTRUNC       VALUE    3131.
      * char field truncated
       78  SQLUE-TRUNCATE            VALUE    3132.
      * Datalink field nulled
       78  SQLUI-DATALINK-NULLED     VALUE    3133.
      * Datalink field error
       78  SQLUI-DATALINK-ROWREJ     VALUE    3134.
      * Too many METHOD cols
       78  SQLU-DCOL-TOO-MANY        VALUE    -3135.
      * not enough columns
       78  SQLUIC-ROWTOOSHORT        VALUE    3137.
      * end of input data file
       78  SQLUIC-EOF-IN-CHARDELS    VALUE    3138.
      * stop using database failed
       78  SQLUE-SQLSTPDB-ERR        VALUE    3139.

      * WSF label too big
       78  SQLUE-WSFLAB-LEN          VALUE    3142.
      * WSF char type length too long
       78  SQLUE-CHAR-MTRUNC         VALUE    3143.
      * WSF char type length too long
       78  SQLUE-CHAR-WTRUNC         VALUE    3144.
      * WSF char truncated at 240
       78  SQLUE-CHAR-ITRUNC         VALUE    3145.
      * row count exceeds 2048
       78  SQLUE-ROWCT-LARGE         VALUE    3147.
      * row not inserted
       78  SQLUE-3148                VALUE    3148.

      * 'H' hcnt not equal 'T' rec ccnt
       78  SQLUII-TCNTCMP            VALUE    3154.
      * invalid name length in 'C' rec
       78  SQLUII-CNAML              VALUE    3155.
      * invalid null field in 'C' rec
       78  SQLUII-CNULL              VALUE    3156.
      * invalid type field in 'C' rec
       78  SQLUII-CTYPE              VALUE    3157.
      * invalid SBCP field in 'C' rec
       78  SQLUII-CSBCP              VALUE    3158.
      * invalid DBCP field in 'C' rec
       78  SQLUII-CDBCP              VALUE    3159.

      * invalid col len fld in 'C' rec
       78  SQLUII-CLENG              VALUE    3160.
      * invalid precision in 'C' rec
       78  SQLUII-CPREC              VALUE    3161.
      * invalid scale field in 'C' rec
       78  SQLUII-CSCAL              VALUE    3162.
      * use 00008 for float col length
       78  SQLUII-CFLOAT-BLANKLENG   VALUE    3163.
      * invalid float col len in 'C'.
       78  SQLUII-CFLOAT-BADLENG     VALUE    3164.
      * 'C' record has invalid type
       78  SQLUII-CUTYPE             VALUE    3165.
      * IXF col does not exist
       78  SQLUII-NOCREC-FOR-NULL-DBCOL VALUE 3166.
      * IXF col is invalid
       78  SQLUII-INVCREC-FOR-NULL-DBCOL VALUE 3167.
      * IXF col not compatible
       78  SQLUII-CRECCOMP-NULL-DBCOL VALUE   3168.

      * EOF found in row of data
       78  SQLUII-DEOF-INROW         VALUE    3170.
      * non-label cell in hdr row
       78  SQLUIW-NONLAB-HDR         VALUE    3171.
      * nullable colnam not found
       78  SQLUIW-NCOL-LOST          VALUE    3172.
      * unsup col type - nullable
       78  SQLUIW-UCOLTYP-NULL       VALUE    3174.
      * wrong rec type for db col
       78  SQLUIW-INTYPE-INV         VALUE    3175.
      * date value out of range
       78  SQLUIW-DATE-INV           VALUE    3176.
      * time value out of range
       78  SQLUIW-TIME-INV           VALUE    3177.
      * int rec for time invalid
       78  SQLUIW-INTIME-INV         VALUE    3178.
      * no data / non nullable col
       78  SQLUIW-NODATA-NNULL       VALUE    3179.

      * insert diskette request
       78  SQLUE-INSERT-DISK         VALUE    3180.
      * file ended before AE rec
       78  SQLUII-AE-NOTFOUND        VALUE    3181.
      * retry to insert diskette
       78  SQLUII-INSERT-DISK-RETRY  VALUE    3182.
      * mult del o'rides/no blanks
       78  SQLUEC-NOBLANK-B4KW       VALUE    3183.
      * row of previous warning
       78  SQLUI-PREVMESG-ROWNO      VALUE    3185.
      * log full inserting row
       78  SQLUI-LOGFULL-INSWARN     VALUE    3186.
      * error creating index
       78  SQLUI-INDEX-WARN          VALUE    3187.
      * error truncating table
       78  SQLUI-TRUNCATE-TABLE      VALUE    -3188.

      * invalid INDEXIXF option
       78  SQLUI-INDEXIXF            VALUE    -3190.
      * data not fit modifier
       78  SQLUE-INVALID-DATE-DATA   VALUE    3191.
      * invalid user date modifier
       78  SQLUE-INVALID-DATE-SPEC   VALUE    -3192.
      * cannot import to this view
       78  SQLUI-VIEW-ERROR          VALUE    -3193.
      * cannot import system table
       78  SQLUI-SYSTBL-ERROR        VALUE    -3194.
      * not enough space
       78  SQLUE-RETRY-DISK          VALUE    3195.
      * input file not found
       78  SQLUI-IN-NOTFD            VALUE    -3196.
      * import/export in use
       78  SQLUI-IMPBUSY             VALUE    -3197.

      * cant replace parent table
       78  SQLUI-REPL-PAR            VALUE    -3201.
      * cant update without PK's
       78  SQLUI-IUOPT-NOPK          VALUE    -3203.
      * cant update views
       78  SQLUI-IUOPT-NOVIEW        VALUE    -3204.
      * cant replace ref cons view
       78  SQLUI-VIEW-REF            VALUE    -3205.
      * cant replace subquery view
       78  SQLUI-VIEW-SQUERY         VALUE    -3206.

      * Invalid table list
       78  SQLU-INVALID-TABLES-LIST  VALUE    -3207.
      * Import Typed-table to Reg
       78  SQLU-TYPED-IMPORT-INTO-REGULAR VALUE 3208.
      * Cannot rename sub-table/attr
       78  SQLU-CANT-RENAM-SUBTAB-OR-ATTR VALUE -3209.
      * Options incompatible w/ hier
       78  SQLU-INCOMPATIBLE-HIERARCHY VALUE  -3210.
      * Load doesn't supp RT
       78  SQLU-LOAD-DOESNT-SUPP-RT  VALUE    -3211.
      * not supported Load Terminate operation
       78  SQLU-NOSUPP-LD-TERM-OP    VALUE    -3212.

      * Load indexing mode
       78  SQLU-INXMODE-INFO         VALUE    3213.
      * Deferred indexing, but unique inx
       78  SQLU-INXMODE-DEFBUTUNIQUE VALUE    -3214.
      * Load incrmental indexing + DMS + copy + same TS
       78  SQLU-INXMODE-DMS-RESTRICTION VALUE 3215.
      * Load incrmental indexing but inx invalid
       78  SQLU-INXMODE-INC-BUTBADINX VALUE   3216.
      * Load incrmental indexing but not insert
       78  SQLU-INXMODE-INC-BUTNOTINSERT VALUE 3217.
      * Index file is damaged/missing
       78  SQLU-INDEX-FILE-MISSING   VALUE    -3218.
      * Load unable to turn off constraints
       78  SQLU-CONSTRAINTS-OFF-FAILED VALUE  -3219.

      * AIX req next file
       78  SQLUE-PROVIDE-FILE-PART   VALUE    3220.
      * start commit
       78  SQLUI-START-COMMIT        VALUE    3221.
      * finish commit
       78  SQLUI-FINISH-COMMIT       VALUE    3222.
      * bad input parms
       78  SQLUI-BAD-STRUCT-PARM     VALUE    -3223.
      * restartcnt too big
       78  SQLUI-SKIPPED-ALL-ROWS    VALUE    -3225.
      * map special token to user record
       78  SQLU-WHICH-USER-RECORD    VALUE    3227.
      * Datalink table, deferred indexing not allowed on load
       78  SQLU-DL-AND-DEFERRED-INX  VALUE    -3228.
      * compound=x error
       78  SQLUI-COMPOUND-ERR        VALUE    -3250.

      * row/col sequence invalid
       78  SQLUIW-RCSEQ-INV          VALUE    -3300.
      * BOF in middle of WSF file
       78  SQLUIW-BOF-INV            VALUE    -3301.
      * unexpected EOF
       78  SQLUIW-EARLY-EOF          VALUE    -3302.
      * filetype not ixf
       78  SQLUI-IXFONLY             VALUE    -3303.
      * table does not exist
       78  SQLUI-DELTABLE            VALUE    -3304.
      * table already exists
       78  SQLUI-CREATE-ERR          VALUE    -3305.
      * SQL error during insert
       78  SQLUI-EXECUTE-ERR         VALUE    -3306.
      * incomplete col info
       78  SQLUI-INC-COL             VALUE    -3307.
      * codepage mismatch
       78  SQLUI-CP-MISMATCH         VALUE    -3308.
      * double byte data found
       78  SQLUI-DBLDATA             VALUE    -3309.

      * unrec col type
       78  SQLUI-UNREC-CTYPE         VALUE    -3310.
      * invalid IXF column
       78  SQLUI-INVCREC-FOR-CREATE  VALUE    -3310.

      * disk full - OS/2
       78  SQLUE-DISK-FULL-DB2OS2    VALUE    -3313.
      * disk full - Windows NT
       78  SQLUE-DISK-FULL-DB2NT     VALUE    -3313.
      * disk full - DOS
       78  SQLUE-DISK-FULL-DB2DOS    VALUE    -3313.
      * disk full - Windows
       78  SQLUE-DISK-FULL-DB2WIN    VALUE    -3313.
      * disk full - AIX
       78  SQLUE-DISK-FULL-DB2AIX    VALUE    -10018.
      * disk full - MacOS
       78  SQLUE-DISK-FULL-DB2MAC    VALUE    -3313.

       78  SQLUE-DISK-FULL           value    -3313.

      * 'A' data/ time not as 'H'.
       78  SQLUII-ASTAMP-NOMATCH     VALUE    -3314.
      * invalid volume info
       78  SQLUII-ACREC-BADVOL       VALUE    -3315.
      * error closing IXF file
       78  SQLUII-CLOSE-NOTLAST      VALUE    -3316.
      * conflict in filetmod
       78  SQLUW-FTMOD-INV           VALUE    -3317.
      * keyword repeated/filetmod
       78  SQLUEC-DUP-KEYWORD        VALUE    -3318.
      * error creating table
       78  SQLUI-ERR-CREATETAB       VALUE    -3319.

      * keyword at end of filetmod
       78  SQLUEC-NOROOM-AFTERKW     VALUE    -3320.
      * circular log full
       78  SQLUI-LOGFULL-INSERR      VALUE    -3321.
      * semaphore error
       78  SQLUE-SEM-ERROR           VALUE    -3322.
      * column type invalid
       78  SQLUE-INVCOLTYPE          VALUE    -3324.
      * ignored WSF row
       78  SQLUEW-IGNORED-ROW        VALUE    3325.
      * column list invalid
       78  SQLUI-COL-LIST            VALUE    -3326.
      * system error
       78  SQLUEI-SYSERROR           VALUE    -3327.
      * Error importing to nickname
       78  SQLUI-NICKNAME-ERR        VALUE    -27999.
      * Some metadata will not be saved to IXF on Export
       78  SQLUE-NO-IXF-INFO         VALUE    27984.

      * odd leng char -> graphic
       78  SQLUII-ODD2GRAPH          VALUE    3330.
      * permission denied
       78  SQLUE-OEACCESS            VALUE    -3331.
      * too many files open
       78  SQLUE-OEMFILE             VALUE    -3332.
      * no such file or directory
       78  SQLUE-OENOENT             VALUE    -3333.
      * not enough memory
       78  SQLUE-OENOMEM             VALUE    -3334.
      * no space left
       78  SQLUE-OENOSPC             VALUE    -3335.
      * read access load conditions not met
       78  SQLU-READ-ACCESS-NOT-ALLOWED VALUE -3340.
      * use tablespace incorrect
       78  SQLU-INVALID-USE-TABLESPACE VALUE  -3341.
      * insufficient authority to issue lock with force
       78  SQLU-LOCK-WITH-FORCE-DENIED VALUE  -3342.
      * cant load restart after rollforward
       78  SQLU-NO-RSTART-POST-RLFWARD VALUE  -3343.
      * use tablespace warning message
       78  SQLU-USE-TABLESPACE-WARNING VALUE  3346.

      * invalid method for ASC
       78  SQLUIA-BAD-DCOL-METH      VALUE    -3400.
      * invalid import method
       78  SQLUI-DCOLM-ALL           VALUE    -3401.
      * zeroes as begin/end
       78  SQLUIA-NULLLOC            VALUE    -3402.
      * invalid pair
       78  SQLUIA-LOCPAIR            VALUE    -3403.
      * invalid pair for number
       78  SQLUIA-LOCNUM             VALUE    -3404.
      * invalid pair for date
       78  SQLUIA-LOCDATE            VALUE    -3405.
      * invalid pair for time
       78  SQLUIA-LOCTIME            VALUE    -3406.
      * invalid pair for timestamp
       78  SQLUIA-LOCSTAMP           VALUE    -3407.
      * pair defines long field
       78  SQLUIA-LOCLONG            VALUE    3408.
      * pair defines short field
       78  SQLUIA-LOCSHORT           VALUE    3409.
      * invalid pair for graphic
       78  SQLUIA-LOCODD             VALUE    -3410.
      * value not graphic--null
       78  SQLUIA-CFGRAPH-NULLED     VALUE    3411.
      * value not graphic--not null
       78  SQLUIA-CFGRAPH-ROWREJ     VALUE    3412.
      * field too short--nulled
       78  SQLUIA-SHORTFLDNULLED     VALUE    3413.
       78  SQLU-NO-LIFEBOAT          VALUE    -3414.
      * CPCV failed--null
       78  SQLUIA-CFCPCV-NULLED      VALUE    3415.
      * CPCV failed--not null
       78  SQLUIA-CFCPCV-ROWREJ      VALUE    3416.
      * Modified by NOCHARDEL usage warning
       78  SQLU-NOCHARDEL-WARNING    VALUE    3418.
      * Vendor sort for collating type is unsupported, using default db2
      * sort
       78  SQLU-VENDOR-SORT-IGNORED  VALUE    3419.

      * number of warnings hit threshold
       78  SQLU-TOO-MANY-WARNINGS    VALUE    -3502.
      * number of rows hit threshold
       78  SQLU-ROWCNT               VALUE    3503.
      * reclen > 32767
       78  SQLULA-INVALID-RECLEN     VALUE    -3505.
      * null ind value not Y or N
       78  SQLULA-NULLIND-IGNORED    VALUE    3506.
      * nullind column is invalid
       78  SQLUI-NULLIND             VALUE    -3507.
      * file access error during load
       78  SQLUL-FILE-ERROR          VALUE    -3508.
      * num of row deleted after load
       78  SQLUL-NUM-ROW-DELETED     VALUE    3509.
      * work directory is invalid
       78  SQLU-SORT-WORK-DIR-ERROR  VALUE    -3510.
      * lobfile missing but nullable col
       78  SQLU-NB-LOBFILE-MISSING   VALUE    3511.
      * lobfile missing, nonnullable col
       78  SQLU-NNB-LOBFILE-MISSING  VALUE    3512.
      * codepage doesn't match db
       78  SQLUL-UNMATCHED-CODEPAGE  VALUE    -3513.
      * system error with reason code
       78  SQLUL-SYSERR-WITH-REASON  VALUE    -3514.
      * unexpected rec in db2cs
       78  SQLUL-UNEXPECTED-RECORD   VALUE    -3517.
      * coltype incompatible for db2cs
       78  SQLUL-INCOMPATIBLE-TABLE  VALUE    -3518.
      * missing file
       78  SQLUL-FILE-NOT-FOUND      VALUE    -3521.
      * copy spec, no logretain/userexit
       78  SQLUL-COPY-LOGRETAIN-OFF  VALUE    -3522.
      * no messages to be retrieved
       78  SQLUL-NO-MESSAGES         VALUE    3523.
      * freespace option invalid
       78  SQLUL-FREESPACE-OPT       VALUE    -3524.
      * incompatible options
       78  SQLU-INCOMPAT-OPT         VALUE    -3525.
      * modifier incompatible with load options
       78  SQLU-MOD-INCOMPAT-WITH-OPT VALUE   -3526.
      * invalid codepage
       78  SQLULA-INVALID-CODEPAGE   VALUE    -3527.
      * delimiter may be converted from app to DB
       78  SQLUL-DELIMITER-CONV-W    VALUE    3528.
      * Unsupported data type
       78  SQLUL-UNSUPPORTED-DATA-TYPE VALUE  -3529.
      * Load index creation parameter no longer supported
       78  SQLUL-OBSOLETETE-SORT-PARM VALUE   3535.
      * Generated override warning
       78  SQLUL-GENERATED-OVERRIDE  VALUE    3551.
      * mount new tape
       78  SQLU-NEXT-TAPE-WARNING    VALUE    3700.
      * no lobs/longs but lobpath nonull
       78  SQLU-LOBPATHS-IGNORED     VALUE    3701.
      * device error but ignored
       78  SQLU-DEVICE-IGNORED       VALUE    3702.
      * invalid number of buffers
       78  SQLU-NUM-BUFFERS          VALUE    -3704.
      * invalid load/unload buffer size
       78  SQLU-BUFFER-SIZE-ERROR    VALUE    -3705.
      * copy target full
       78  SQLUL-DISK-FULL           VALUE    -3706.
      * invalid sort buffer size
       78  SQLU-SORT-BUFFSIZE-ERROR  VALUE    -3707.
      * Hash table is full
       78  SQLUE-NOSPACE-IN-HASH     VALUE    -3708.

      * Load / unload / load recovery SQLCODES
       78  SQLU-OPEN-COPY-LOC-FILE-ERROR VALUE -3783.
       78  SQLU-INV-COPY-LOC-FILE-INPUT VALUE -3784.
       78  SQLU-LOAD-RECOVERY-FAILED VALUE    -3785.
       78  SQLU-INVALID-PARM-WARNING VALUE    3798.
       78  SQLU-LOAD-RECOVERY-PENDING VALUE   3799.

      * load recovery - copy location input error type
       78  SQLU-KEYWORD-CODE         VALUE    1.
       78  SQLU-OVERRIDE-CODE        VALUE    2.
       78  SQLU-UNEXPECTED-EOF-CODE  VALUE    3.
       78  SQLU-IOERROR-CODE         VALUE    4.

      * load recovery - Different Load recovery options
       78  SQLU-RECOVERABLE-LOAD     VALUE    0.
       78  SQLU-NON-RECOVERABLE-LOAD VALUE    1.

      * Loadapi SQLCODES
       78  SQLU-INVALID-QUIESCEMODE  VALUE    -3802.
       78  SQLU-INVALID-INDEX        VALUE    -3804.
       78  SQLU-INVALID-LOADAPI-ACTION VALUE  -3805.
       78  SQLU-CONSTRAINTS-NOT-OFF  VALUE    -3806.

      * Export SQLCODES
      * Export message
       78  SQLUE-MSG                 VALUE    -3999.

      * Roll-Forward Recovery SQLCODES
      * invalid parameter
       78  SQLU-INVALID-PARAM        VALUE    -4904.
      * invalid parameter range
       78  SQLU-INVALID-RANGE        VALUE    -4905.
      * invalid tablespace set
       78  SQLUM-INVALID-TPS-SET     VALUE    -4906.
      * tables set to check pending state
       78  SQLUM-CHECK-PENDING-SET   VALUE    4907.
      * tablespace not ready to roll forward
       78  SQLUM-TSP-NOT-READY       VALUE    -4908.
      * Tables in tablespaces are in DRP/RNP state
       78  SQLU-RFR-DATALINK-PENDING VALUE    4909.
      * Invalid overflow log path
       78  SQLU-INVALID-OFLOGPATH    VALUE    -4910.
      * missing log files
       78  SQLU-RC-MISSING-LOGFILES  VALUE    -4970.
      * log truncation failed
       78  SQLU-RC-LOG-TRUNCATED     VALUE    -4971.
      * log path full
       78  SQLU-RC-LOGPATH-FULL      VALUE    -4972.
      * log mismatch with catalog node
       78  SQLU-RC-LOG-MISMATCH      VALUE    -4973.
      * query status warning
       78  SQLU-RC-QRY-ERR-WARN      VALUE    4974.
      * rollforward canceled
       78  SQLU-RC-CANCELED-WARN     VALUE    4975.
      * not on catalog node
       78  SQLU-RC-NOT-ON-CATALOG-NODE VALUE  -4976.
      * bad export directory
       78  SQLU-RC-EXPORT-DIR        VALUE    -4977.
      * bad dropped table recovery option
       78  SQLU-RC-DROPPED-TBL       VALUE    -4978.
      * error while exporting table data
       78  SQLU-RC-EXPORT-DATA       VALUE    4979.
      * Corrupt Log file
       78  SQLU-RC-LOGFILE-CORRUPT   VALUE    -4980.

      * Configuration SQLCODES
      * only SYSADM_GROUP can change db2 configuration file
       78  SQLF-RC-SYSAUTH           VALUE    -5001.
      * system error
       78  SQLF-RC-SYSERR            VALUE    -5005.
      * path name error
       78  SQLF-RC-PATHNAME          VALUE    -5010.
      * invalid node name
       78  SQLF-RC-INVNODENAME       VALUE    -5020.
      * invalid release number
       78  SQLF-RC-REL               VALUE    -5030.
      * database needs migration; release number is back-level
       78  SQLF-RC-NEEDMIG           VALUE    -5035.
      * insufficient memory to support stack switching
       78  SQLF-RC-INSMEM            VALUE    -5047.
      * invalid db2 configuration file
       78  SQLF-RC-SYSCSUM           VALUE    -5050.
      * invalid db configuration file
       78  SQLF-RC-DBCSUM            VALUE    -5055.
      * invalid token parameter
       78  SQLF-RC-INVTKN            VALUE    -5060.
      * invalid ptr to sqlfupd
       78  SQLF-RC-INVTKN-STRUCT     VALUE    -5061.
      * invalid token ptr value
       78  SQLF-RC-INVTKN-PTR        VALUE    -5062.
      * warning - truncated result due to obsolete db cfg token
       78  SQLF-RC-OLD-DB-CFG-TKN-TRUNC VALUE 5066.
      * invalid count parameter
       78  SQLF-RC-CNTINV            VALUE    -5070.
      * invalid logging db cfg parameter
       78  SQLF-RC-INVLOGCFG         VALUE    -5099.
      * invalid new log path - use SQLF_RC_INVLOGCFG instead
       78  SQLF-RC-INVNEWLOGP        VALUE    -5099.

      * invalid bit value - must be 0 or 1
       78  SQLF-RC-INV-BIT-VALUE     VALUE    -5112.
      * set alt_collate on unicode db not allowed
       78  SQLF-RC-ALT-COLLATE       VALUE    -5113.

      * invalid DB configuration details
       78  SQLF-RC-INVDETS           VALUE    -5121.
      * database is copy protected
       78  SQLF-RC-PROTECT           VALUE    -5122.
      * I/O Error with log header
       78  SQLF-RC-LOGIO             VALUE    -5123.
      * invalid db2 config file entry
       78  SQLF-RC-INV-DBMENT        VALUE    -5126.
      * integer out of range
       78  SQLF-RC-INV-RANGE         VALUE    -5130.
      * integer out of range (-1)
       78  SQLF-RC-INV-RANGE-2       VALUE    -5131.
      * string null or too long
       78  SQLF-RC-INV-STRING        VALUE    -5132.
      * char/int not in set
       78  SQLF-RC-INV-SET           VALUE    -5133.
      * tpname not valid
       78  SQLF-RC-INVTPNAME         VALUE    -5134.
      * dftdbpath not valid
       78  SQLF-RC-INV-DBPATH        VALUE    -5136.
      * diagpath not valid
       78  SQLF-RC-INV-DIAGPATH      VALUE    -5137.
      * invalid agent priority
       78  SQLF-RC-INV-AGENTPRI      VALUE    -5131.
      * out of range - value too small
       78  SQLF-RC-INV-RANGE-TOO-SMALL VALUE  -5150.
      * out of range - value too small (-1 is allowed)
       78  SQLF-RC-INV-RANGE-TOO-SMALL-2 VALUE -5151.
      * out of range - value too large
       78  SQLF-RC-INV-RANGE-TOO-BIG VALUE    -5152.
      * out of range - condition violated
       78  SQLF-RC-INV-RANGE-CONDITION VALUE  -5153.
      * authentication must be CLIENT for trust_allcnts = NO or
      * trust_clntauth = SERVER
       78  SQLF-RC-INV-AUTH-TRUST    VALUE    -5154.
      * sortheap performance warning
       78  SQLF-RC-SORTHEAP-PERF     VALUE    5155.

      * Data Redistribution Return Codes
      * nodegroup name is undefined
       78  SQLUT-NGNAME-UNDEF        VALUE    -204.
      * node is a duplicate node
       78  SQLUT-NODE-DUP            VALUE    -265.
      * node is undefined
       78  SQLUT-NODE-UNDEF          VALUE    -266.
      * max no. of part. map reached
       78  SQLUT-OVER-MAX-PARTNO     VALUE    -269.
      * redist undefined for sys obj.
       78  SQLUT-REDIST-UNDEF        VALUE    -607.
      * insufficient authority
       78  SQLUT-INSAUTH             VALUE    -1092.
      * file or dir access denied
       78  SQLUT-ACCESS-DENIED       VALUE    -1326.
      * for tablespaces on a node
       78  SQLUT-NO-CONTAINERS       VALUE    -1755.
      * invalid authorizations -2018
       78  SQLUT-INVALID-AUTHS       value    -2018.
      * parm to utility incorrect -2032
       78  SQLUT-INVALID-PARM        value    -2032.
      * addr of parm incorrect -2034
       78  SQLUT-INVALID-PARM-ADDRESS value   -2034.
      * function interruption -3005
       78  SQLUT-CBI                 value    -3005.
      * redist failed - no part key
       78  SQLUT-REDIST-NO-PARTKEY   VALUE    -6047.
      * error found in the input file
       78  SQLUT-ERR-IN-FILE         VALUE    -6053.
      * redistribution not performed
       78  SQLUT-RC-REDIST-ERR       VALUE    -6056.
      * Log Extent file size changed
       78  SQLU-DIFF-LOG-SIZE        VALUE    -6063.
      * error during data redistbution
       78  SQLUT-ERROR-REDIST        VALUE    -6064.
      * error writing output file
       78  SQLUT-WRT-OUT-FILE        VALUE    -6065.
      * Load Header Return Codes
      * Invalid partition map
       78  SQLU-PARTITIONMAP         VALUE    -6100.
      * Invalid node number
       78  SQLU-NODE-NUMBER          VALUE    -6101.
      * Parameter reserved for future
       78  SQLU-FUTURE-PARM          VALUE    -6102.
      * Unexpected load system error
       78  SQLU-LOAD-SYSERR          VALUE    -6103.
      * Load does not support indices
       78  SQLU-NO-INDICES           VALUE    -6104.
      * Load complete - backup NOW !
       78  SQLU-LOAD-COMPLETE        VALUE    -6105.
      * Invalid use of NOHEADER
       78  SQLU-NOHEADER             VALUE    -6106.
      * Invalid partitioning key
       78  SQLU-PARTITION-KEY        VALUE    -6107.
      * Wrong number of partition keys
       78  SQLU-PARTITION-KEY-NUM    VALUE    -6108.
      * Unexpected partitioning key
       78  SQLU-PARTITION-KEY-NAME   VALUE    -6109.
      * Unexpected partition key type
       78  SQLU-PARTITION-KEY-TYPE   VALUE    -6110.
      * Repository for obsolete Return Codes

      * wrote wrong # of bytes
       78  SQLU-WRITE-ERROR          VALUE    -2006.
      * error in Start Using
       78  SQLU-CONNECT-ERROR        VALUE    -2010.
      * error in ints
       78  SQLU-INT-ERROR            VALUE    -2012.
      * ADSM reported error
       78  SQLU-ADSM-ERROR           value    -2033.
      * unable to load ADSM
       78  SQLU-LOAD-ADSM-ERROR      value    -2037.
      * media header not present
       78  SQLUD-NO-MHEADER-ERROR    VALUE    -2531.
      * media header missing
       78  SQLUD-NO-MHEADER-WARNING  VALUE    2534.
      * another tape mount required
       78  SQLUD-NEXT-TAPE-WARNING   VALUE    2535.
      * waiting for TSM server to access data on removable media
       78  SQLUD-TSM-MOUNT-WAIT      VALUE    2545.
      * waiting for ADSM server to access data on removable media
       78  SQLUD-ADSM-MOUNT-WAIT     value    2545.

      *************************************************************************
      * Configuration parameter obsolete return codes defines -  
      * Some configuration parameters had specific out of range return
      * codes; these have been replaced by generic out of range messages
      * In these cases the old token names for the specific return codes
      * are given, but the values are replaced by the new values returned 
      * when out of range.
      **************************************************************************

      * only SYSADM can changedatabase configuration file
       78  SQLF-RC-DBAUTH            VALUE    -5002.
      * invalid # of concurrent db
       78  SQLF-RC-INVNDB            VALUE    -5130.
      * invalid req I/O blk size
       78  SQLF-RC-INVRIO            VALUE    -5130.
      * invalid serv I/O blk size
       78  SQLF-RC-INVSIO            VALUE    -5015.
      * invalid communications heap
       78  SQLF-RC-INVCHEAP          VALUE    -5016.
      * invalid remote services heap
       78  SQLF-RC-INVRSHEAP         VALUE    -5017.
      * invalid sort heap threshold
       78  SQLF-RC-INVSHPTHR         VALUE    -5130.
      * invalid DB2COMM value
       78  SQLCC-RC-BAD-DB2COMM      VALUE    -5036.
      * service name not definein db2 config file
       78  SQLCC-RC-NO-SERV-IN-DBMCFG VALUE   -5037.
      * service name not found in/etc/services file
       78  SQLCC-RC-SERV-NOT-FOUND   VALUE    -5038.
      * interrupt port not found in/etc/services file
       78  SQLCC-RC-INT-PORT-NOT-FOUND VALUE  -5039.
      * trans program name not definedin db2 configuration file
       78  SQLCC-RC-NO-TPN-IN-DBMCFG VALUE    -5041.
      * invalid # of locklist
       78  SQLF-RC-INVNLL            VALUE    -5130.
      * invalid # bufr pool pages
       78  SQLF-RC-INVNBP            VALUE    -5130.
      * invalid # of DB files open
       78  SQLF-RC-INVNDBF           VALUE    -5130.
      * invalid soft check point value
       78  SQLF-RC-INVSCP-DB2OS2     VALUE    -5130.
      * invalid soft check point value
       78  SQLF-RC-INVSCP-DB2AIX     VALUE    -5130.
      * invalid soft check point value
       78  SQLF-RC-INVSCP            VALUE    -5130.
      * invalid # of active appls
       78  SQLF-RC-INVNAP            VALUE    -5130.
      * invalid application heapsz
       78  SQLF-RC-INVAHP            VALUE    -5130.
      * invalid database heap size
       78  SQLF-RC-INVDHP            VALUE    -5130.
      * invalid deadlock detection
       78  SQLF-RC-INVDLT            VALUE    -5130.
      * invalid # of total files openper application
       78  SQLF-RC-INVTAF            VALUE    -5130.
      * invalid sortlist heap
       78  SQLF-RC-INVSHP            VALUE    -5130.
      * invalid maxlocks per application
       78  SQLF-RC-INVMAL            VALUE    -5130.
      * invalid statement heap
       78  SQLF-RC-INVSTMHP          VALUE    -5130.
      * invalid number primary log files
       78  SQLF-RC-INVLOGPRIM        VALUE    -5130.
      * invalid number of secondary log files
       78  SQLF-RC-INVLOG2ND         VALUE    -5130.
      * invalid log file size
       78  SQLF-RC-INVLOGFSZ         VALUE    -5130.
      * incompatible file open parmeter
       78  SQLF-RC-INVDB2            VALUE    -5102.
      * no DB's / requestor only
       78  SQLF-RC-INVK2             VALUE    -5104.
      * standalone nodetype does notsupport nodename
       78  SQLF-RC-INVK3             VALUE    -5126.
      * remote workstation has alreadybeen configured
       78  SQLF-RC-RWS-EXIST         VALUE    -5106.
      * <authid> does not haveauthority to add or drop a remote workstation
       78  SQLF-RC-RWS-SYSADM        VALUE    -5107.
      * remote workstation has notbeen previously setup using sqlarws
       78  SQLF-RC-RWS-NOT-EXIST     VALUE    -5108.
      * machine name is missing ors too long.
       78  SQLF-RC-RWS-MACHINENAME   VALUE    -5109.
      * configuration option is invalid
       78  SQLF-RC-RWS-INV-OPT       VALUE    -5110.
      * environment variableDB2WKSTPROF is not defined
       78  SQLF-RC-ENV-VAR-NOTDEF    VALUE    -5111.
      * incompatible buffer pool and maximum # of applications
       78  SQLF-RC-INVDB3            VALUE    -5146.
      * invalid QUERY_HEAP_SZ
       78  SQLF-RC-INV-QUERY-HEAP-SZ VALUE    -5143.
      * out of range - limited by a parm
       78  SQLF-RC-INV-RANGE-3       VALUE    -5144.
      * out of range - maximum limited by an expression
       78  SQLF-RC-INV-RANGE-MAX-EXPR VALUE   -5144.
      * out of range - maximum limited by an expression (range includes -1)
       78  SQLF-RC-INV-RANGE-MAX-EXPR-2 VALUE -5145.
      * out of range - minimum limited by an expression
       78  SQLF-RC-INV-RANGE-MIN-EXPR VALUE   -5146.
      * out of range - minimum limited by an expression (range includes -1)
       78  SQLF-RC-INV-RANGE-MIN-EXPR-2 VALUE -5147.
      * not current db2 configuration
       78  SQLF-RC-KCON              VALUE    -5025.
      * invalid initial log size
       78  SQLF-RC-INVILF            VALUE    -5083.
      * invalid logfile extention
       78  SQLF-RC-INVLFE            VALUE    -5091.
      * invalid # of log extention
       78  SQLF-RC-INVNLE            VALUE    -5092.
      * incompatible logfile parameter
       78  SQLF-RC-INVDB1            VALUE    -5101.
      * both R1 & R3 Log parameters may not be modified
       78  SQLF-RC-LF-1-3            VALUE    -5120.
      * maxappls*maxlocks too low
       78  SQLF-RC-LOW-APPLS-AND-LOCKS VALUE  -5135.
      * To many items on update or get
       78  SQLF-RC-MAX-ITEMS-EXCEEDED VALUE   -5139.
      * invalid AVG_APPLS
       78  SQLF-RC-INV-AVG-APPLS     VALUE    -5141.
      * invalid system flag
       78  SQLF-RC-INVSYSIDX         VALUE    -5021.
      * invalid database flag
       78  SQLF-RC-INVDBIDX          VALUE    -5022.
      * invalid sysadm_group
       78  SQLF-RC-INVSYSADM         VALUE    -5028.
      * invalid node type
       78  SQLF-RC-INVNT             VALUE    -5065.
      * interrupt received
       78  SQLF-RC-CNTBRK            VALUE    -5075.
      * invalid authentication
       78  SQLF-RC-INV-AUTHENTICATION VALUE   -5140.
      * invalid trust_allclnts
       78  SQLF-RC-INV-TRUST-ALLCLNTS VALUE   -5156.
      * db2atld restartcount warning
       78  SQLU-ATLD-RESTARTCOUNT-WARN VALUE  6500.
      * db2atld savecount error
       78  SQLU-ATLD-SAVECOUNT-ERROR VALUE    -6532.
      * db2atld restartcount error
       78  SQLU-ATLD-RESTARTCOUNT-ERROR VALUE -6533.
      * db2atld does not need splitting
       78  SQLU-ATLD-SPLIT-NOT-NEEDED VALUE   -6535.
      * db2atld unable to open pmap
       78  SQLU-PMAP-OPEN-ERR        VALUE    -6550.
      * db2atld failed to write pmap
       78  SQLU-PMAP-WRITE-ERR       VALUE    -6551.
      * db2atld failed to open temp file
       78  SQLU-TEMPCFG-OPEN-ERR     VALUE    -6552.
      * db2atld failed to write to tmp file
       78  SQLU-TEMPCFG-WRITE-ERR    VALUE    -6553.
      * db2atld failed to spawn remote child
       78  SQLU-REXEC-ERR            VALUE    -6554.
      * db2atld comm error
       78  SQLU-ATLD-COMM-ERR        VALUE    -6555.
      * db2atld partial record found
       78  SQLU-ATLD-PARTIAL-REC     VALUE    6556.
      * db2atld failed to get default node
       78  SQLU-GET-DEFAULT-NODE-ERR VALUE    -6557.
      * db2atld unable to get cur working dir
       78  SQLU-GET-CURDIR-ERR       VALUE    -6558.
      * db2atld incorrect usage
       78  SQLU-ATLD-BAD-CLP-ERR     VALUE    -6559.
      * db2atld split node incorrect
       78  SQLU-BAD-SPLIT-NODE       VALUE    -6560.
      * db2atld invalid load node
       78  SQLU-BAD-LOAD-NODE        VALUE    -6561.
      * db2atld failed to get instance
       78  SQLU-GET-INSTANCE-ERR     VALUE    -6562.
      * db2atld failed to get cur UID
       78  SQLU-GET-UID-ERR          VALUE    -6563.
      * db2atld invalid password
       78  SQLU-BAD-PASSWORD         VALUE    -6564.
      * db2atld help msg
       78  SQLU-ATLD-HELP            VALUE    6565.
      * db2atld load command not specified
       78  SQLU-MISSING-LOAD-COMMAND VALUE    -6566.
      * db2atld option specified twice
       78  SQLU-ATLD-DUP-OPTION      VALUE    -6567.
      * db2atld starting all the load jobs
       78  SQLU-STARTING-ALL-LOADS   VALUE    6568.
      * db2atld starting all the splitters
       78  SQLU-STARTING-ALL-SPLITTERS VALUE  6569.
      * db2atld waiting for splitters to finish
       78  SQLU-WAITING-FOR-SPLITTERS VALUE   6570.
      * db2atld waiting for load to complete
       78  SQLU-WAITING-FOR-LOADS    VALUE    6571.
      * db2atld load has started
       78  SQLU-LOAD-STARTED         VALUE    6572.
      * db2atld splitters has finished
       78  SQLU-SPLIT-RESULT         VALUE    6573.
      * db2atld Bytes read thus far
       78  SQLU-DATA-READ-STATUS     VALUE    6574.
      * db2atld total size of data read
       78  SQLU-TOTAL-DATA-READ      VALUE    6575.
      * db2atld threading error
       78  SQLU-THREAD-ERR           VALUE    -6576.
      * rowcount not supported in this mode
       78  SQLU-ATLD-ROWCOUNT-ERROR  VALUE    -27961.
      * too many splitters
       78  SQLU-ATLD-TOO-MANY-SPLITTERS VALUE -27991.
      * invalid DB2_LOAD_COPY_NO_OVERRIDE reg variable
       78  SQLU-COPYNO-OVERRIDE-INVALID VALUE -27965.
      * COPY NO was overridden warning
       78  SQLU-COPYNO-OVERRIDE-WARNING VALUE 27966.
      * COPY NO was overriden with default (nonrecov)
       78  SQLU-COPYNO-OVERRIDE-DFLTWARN VALUE 27967.
      * Special Register Default value truncated.
       78  SQLU-SPEC-REGSTR-TRUNC-WARN VALUE  27994.

      * Flush Table API Return Codes
      * Table not at this node
       78  SQLUF-TABLE-NOT-AT-NODE   VALUE    -6024.

      * sqlugrpi, sqlugrpn, and sqlugtpi return codes
      * Invalid authid
       78  SQLUG-INVALID-AUTHID      value    -2300.
      * invalid table
       78  SQLUG-INVALID-TABLE-NAME  value    -2301.
      * Interrupt
       78  SQLUG-CBI                 value    -3005.
      * Table not exist
       78  SQLUG-TABLE-NOT-FOUND     value    -2211.
      * Insufficient authorization
       78  SQLUG-RC-INSAUTH          VALUE    -6023.
      * No partitioning key
       78  SQLUG-NULL-PARTKEY        VALUE    -6038.
      * Nulls not allowed
       78  SQLUG-NULL-NOTALLOWED     VALUE    -6039.
      * decimal format conflict
       78  SQLUG-DECIMAL-FORMAT-CONFLICT VALUE -2755.
      * Invalid syntax
       78  SQLUG-INVALID-SYNTAX      VALUE    -6044.
      * Invalid datatype
       78  SQLUG-INVALID-DATATYPE    VALUE    -6045.
      * Invalid dynamic query mgmt flag
       78  SQLF-RC-INVALID-DYNQUERYMGMT VALUE -29000.

      * Load error codes continued
      * Load restart/terminate is not necessary
       78  SQLU-INV-RESTART-TERMINATE VALUE   -27902.
      * Invalid input parameter for partitioned database load
       78  SQLU-PARTLOAD-BAD-PARAMETER VALUE  -27959.
      * Invalid PART_FILE_LOCATION specified for source type CURSOR
       78  SQLU-PARTLOAD-PART-FILE-CURSOR VALUE -27960.
      * Invalid rowcount specified for partitioned database load
       78  SQLU-PARTLOAD-BAD-ROWCOUNT VALUE   -27961.
      * Load partition violations detected
       78  SQLU-PARTITION-VIOLATIONS VALUE    27990.
      * Partitioning map was found, but load mode is not load_only
       78  SQLU-PARTITIONING-MAP-FOUND VALUE  -27992.

      * Utility control error codes
      * Invalid priority
       78  SQLUTH-INVALID-PRIORITY   VALUE    -1152.
      * Utility not found
       78  SQLUTH-UTILITY-NOT-FOUND  VALUE    -1153.
      * Utility does not support throttling
       78  SQLUTH-NO-THROTTLE-SUPPORT VALUE   -1154.

      * obsolete database manager configuration parameter tokens
       78  SQLF-KTN-SVRIOBLK         VALUE    3.
       78  SQLF-KTN-SQLENSEG         VALUE    5.
       78  SQLF-KTN-COMHEAPSZ        VALUE    8.
       78  SQLF-KTN-RSHEAPSZ         VALUE    9.
       78  SQLF-KTN-NUMRC            VALUE    10.
       78  SQLF-KTN-CUINTERVAL       VALUE    14.
       78  SQLF-KTN-SQLSTMTSZ        VALUE    17.
       78  SQLF-KTN-COMHEAPSZ-P      VALUE    50.
       78  SQLF-KTN-RSHEAPSZ-P       VALUE    51.
       78  SQLF-KTN-IPX-FILESERVER   value    47.
       78  SQLF-KTN-IPX-OBJECTNAME   value    48.
       78  SQLF-KTN-ADSM-PASSWORD    VALUE    93.
       78  SQLF-KTN-ADSM-NODENAME    VALUE    94.
       78  SQLF-KTN-ADSM-OWNER       VALUE    95.
       78  SQLF-KTN-MAX-IDLEAGENTS   VALUE    60.

      * obsolete database configuration parameter tokens
       78  SQLF-DBTN-LOGFILE         VALUE    4.
       78  SQLF-DBTN-MAXTOTFILOP     VALUE    10.
       78  SQLF-DBTN-LOGEXT          VALUE    12.
       78  SQLF-DBTN-LOGMAXEXT       VALUE    13.
       78  SQLF-DBTN-AGENTHEAP       VALUE    14.
       78  SQLF-DBTN-SEGPAGES        VALUE    123.
       78  SQLF-DBTN-BUFFPAGE        VALUE    2.
       78  SQLF-DBTN-SORTHEAP        VALUE    91.
       78  SQLF-DBTN-SORTHEAPSZ-P    value    52.
       78  SQLF-DBTN-DBHEAP          VALUE    50.
       78  SQLF-DBTN-DBHEAP-P        value    50.
       78  SQLF-DBTN-APPLHEAPSZ-P    value    51.
       78  SQLF-DBTN-STMTHEAPSZ-P    value    53.
       78  SQLF-DBTN-LOGFILSIZ       VALUE    18.
       78  SQLF-DBTN-MULTIPGAL       VALUE    504.
       78  SQLF-DBTN-ADSM-PASSWD     VALUE    501.
       78  SQLF-DBTN-NEXTACTIVE      VALUE    107.
       78  SQLF-DBTN-DL-NUM-BACKUP   VALUE    352.
       78  SQLF-DBTN-DETS            VALUE    21.
       78  SQLF-DBTN-INTFLAGS        VALUE    104.
       78  SQLF-DBTN-ADSM-PASSWORD   value    501.
       78  SQLF-DBTN-ADSM-OWNER      value    305.
       78  SQLF-DBTN-ADSM-NODENAME   value    306.
       78  SQLF-DBTN-ADSM-MGMTCLASS  value    307.
       78  SQLF-DBTN-LOCKLIST        VALUE    1.


      * obsolete config parm assorted defines
      * Default maxcagents
       78  SQLFDMCA                  VALUE    -1.
      * Default maxdari
       78  SQLFDMXD                  VALUE    -1.
      * Default agentpri
       78  SQLFDAPR                  VALUE    -1.
      * keepdari - no
       78  SQLFMKPD                  VALUE    0.
      * keepdari - yes
       78  SQLFXKPD                  VALUE    1.

      * obsolete defines for SQLF_DBTN_DETS
      * xxxx xxxx xxxx xxx0
       78  SQLF-COPY-NOPROT          VALUE    0.
      * xxxx xxxx xxxx xxx1
       78  SQLF-COPY-PROTECT         VALUE    1.
      * xxxx xxxx xxxx xx1x
       78  SQLF-ENABLE-LOG-RETAIN    VALUE    2.
      * xxxx xxxx xxxx x1xx
       78  SQLF-ENABLE-USER-EXIT     VALUE    4.
      * xxxx xxxx xxxx 1xxx
       78  SQLF-ENABLE-AUTO-RESTART  VALUE    8.
      * xxxx xxxx xxx1 xxxx
       78  SQLF-ENABLE-LOGRETAIN-CAPTURE VALUE 16.
      * xxxx xxxx xx1x xxxx
       78  SQLF-ENABLE-TRACKMOD      VALUE    32.

      * obsolete defines for SQLF_DBTN_INTFLAGS
      * xxxx xxxx xxxx xxx1
       78  SQLF-CONSISTENT           VALUE    1.
      * xxxx xxxx xxxx x1xx
       78  SQLF-BACKUP-PENDING       VALUE    4.
      * xxxx xxxx xxxx 1xxx
       78  SQLF-ROLLFWD-PENDING      VALUE    8.
      * xxxx xxxx xxx1 xxxx
       78  SQLF-LOG-RETAIN           VALUE    16.
      * xxxx xxxx xx1x xxxx
       78  SQLF-USER-EXIT            VALUE    32.
      * xxxx xxxx x1xx xxxx
       78  SQLF-TBS-ROLLFWD          VALUE    64.
      * xxxx xxxx 1xxx xxxx
       78  SQLF-RESTORE-PENDING      VALUE    128.

      * Obsolete media types
      * ADSM
       78  SQLU-ADSM-MEDIA           value    "A".


      * Repository for obsolete prototypes

      *************************************************************************
      * 
      * The following functions and symbols are obsolete and may not be supported
      * in future releases. The obsolete functions are provided for backward compatibility
      * and exported from DB2API.LIB. All applications should be migrated to use new APIs.
      * Note: Some of the function parameters may be NO-OP.
      * 
      **************************************************************************
      * Provide info about a tableSpace in a database

       78  SQLB-TBSQRY-DATA-SIZE     value    176.
      * No action at all
       78  DB2-DB-SUSPEND-NONE       VALUE    0.
      * Set write suspend for database
       78  DB2-DB-SUSPEND-WRITE      VALUE    1.
      * Set write resume for database
       78  DB2-DB-RESUME-WRITE       VALUE    2.
      * Set write suspend for tablespaces
       78  DB2-TS-SUSPEND-WRITE      VALUE    10.
      * Set write resume for tablespaces
       78  DB2-TS-RESUME-WRITE       VALUE    20.
      * invalid into in tcolstrg
       78  SQL-SLQUI-TINTO           value    -3034.
      *************************************************************************
      * 
      * End of obsolete functions and symbols
      * 
      **************************************************************************
