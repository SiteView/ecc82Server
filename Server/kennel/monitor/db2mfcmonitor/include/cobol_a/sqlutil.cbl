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
       77  SQLUM-ROLLFWD             PIC S9(4) COMP-5 VALUE 1.
      * End roll-forward recovery
       77  SQLUM-STOP                PIC S9(4) COMP-5 VALUE 2.
      * End roll-forward recovery
       77  SQLUM-COMPLETE            PIC S9(4) COMP-5 VALUE 2.
      * Roll forward to requested point and end roll-forward recovery
       77  SQLUM-ROLLFWD-STOP        PIC S9(4) COMP-5 VALUE 3.
      * Roll forward to requested point and end roll-forward recovery
       77  SQLUM-ROLLFWD-COMPLETE    PIC S9(4) COMP-5 VALUE 3.
      * Query nextArcFile, firstDeleteArc, lastDeleteArc, lastCommitTime
       77  SQLUM-QUERY               PIC S9(4) COMP-5 VALUE 4.
      * Cancel current rollforward
       77  SQLUM-CANCEL              PIC S9(4) COMP-5 VALUE 8.
      * Parameter check
       77  SQLUM-PARM-CHECK          PIC S9(4) COMP-5 VALUE 10.
      * Continue load recovery.
       77  SQLUM-LOADREC-CONTINUE    PIC S9(4) COMP-5 VALUE 11.
      * Terminate load recovery.
       77  SQLUM-LOADREC-TERMINATE   PIC S9(4) COMP-5 VALUE 12.
      * Terminate device.Caller action codes 31 - 50 reserved for internal
      * use.
       77  SQLUM-LOADREC-DEVICE-TERMINATE PIC S9(4) COMP-5 VALUE 13.

      * Constants used for Rollforward
      * Roll forward until STOP specified
       77  SQLUM-INFINITY-TIMESTAMP  PIC X(26) value "9999-12-31-23.59.5
      -    "9.999999".
      * Length of Archive File name
       77  SQLUM-ARCHIVE-FILE-LEN    PIC S9(4) COMP-5 VALUE 12.
      * Length of ISO format Timestamp
       77  SQLUM-TIMESTAMP-LEN       PIC S9(4) COMP-5 VALUE 26.

      * Roll forward input version values
      * current rollforward input version
       77  SQLUM-RFWD-VERSION        PIC S9(4) COMP-5 VALUE 5.

      * Roll forward allNodeFlag values
      * submit to all nodes in the node list
       77  SQLURF-NODE-LIST          PIC S9(4) COMP-5 VALUE 0.
      * submit to all nodes in the nodeconfiguration file
       77  SQLURF-ALL-NODES          PIC S9(4) COMP-5 VALUE 1.
      * submit to all nodes except the onesspecified by the nodelist
      * parameter
       77  SQLURF-ALL-EXCEPT         PIC S9(4) COMP-5 VALUE 2.
      * submit to catalog node only
       77  SQLURF-CAT-NODE-ONLY      PIC S9(4) COMP-5 VALUE 3.

      * Roll forward query status codes
      * cannot query node
       77  SQLURFQ-NOT-AVAILABLE     PIC S9(4) COMP-5 VALUE 0.
      * rollforward is not pending
       77  SQLURFQ-NOT-RFW-PENDING   PIC S9(4) COMP-5 VALUE 1.
      * database rollforward pending
       77  SQLURFQ-DB-RFW-PENDING    PIC S9(4) COMP-5 VALUE 2.
      * tablespace rollforward pending
       77  SQLURFQ-TBL-RFW-PENDING   PIC S9(4) COMP-5 VALUE 3.
      * database rollforward in progress
       77  SQLURFQ-DB-RFW-IN-PROGRESS PIC S9(4) COMP-5 VALUE 4.
      * tablespace rollforward in progress
       77  SQLURFQ-TBL-RFW-IN-PROGRESS PIC S9(4) COMP-5 VALUE 5.
      * database rollforward processing STOP
       77  SQLURFQ-DB-RFW-STOPPING   PIC S9(4) COMP-5 VALUE 6.
      * tablespace rollforward processing STOP
       77  SQLURFQ-TBL-RFW-STOPPING  PIC S9(4) COMP-5 VALUE 7.

      * Rollforward connect mode
      * Offline rollforward mode
       77  SQLUM-OFFLINE             PIC S9(4) COMP-5 VALUE 0.
      * Online rollforward mode
       77  SQLUM-ONLINE              PIC S9(4) COMP-5 VALUE 1.

      * Backup mode
      * Offline backup mode
       77  SQLUB-OFFLINE             PIC S9(4) COMP-5 VALUE 0.
      * Online backup mode
       77  SQLUB-ONLINE              PIC S9(4) COMP-5 VALUE 1.
      * Split mirror backup
       77  SQLUB-SPLITMIRROR         PIC S9(4) COMP-5 VALUE 16.
      * Backup modes
       77  SQLUB-MODE-MASK           PIC S9(4) COMP-5 VALUE 15.
      * Backup options
       77  SQLUB-OPT-MASK            PIC S9(4) COMP-5 VALUE 0.

      * Backup type
       77  SQLUB-DB                  PIC S9(4) COMP-5 VALUE 0.
       77  SQLUB-FULL                PIC S9(4) COMP-5 VALUE 0.
       77  SQLUB-TABLESPACE          PIC S9(4) COMP-5 VALUE 3.
       77  SQLUB-INCREMENTAL         PIC S9(4) COMP-5 VALUE 16.
       77  SQLUB-DELTA               PIC S9(4) COMP-5 VALUE 48.
       77  SQLUB-INC-LEVEL-MASK      PIC S9(9) COMP-5 VALUE 65280.

      * Backup type masks
      * Determines if full/tbsp bckp.
       77  SQLUB-TYPE-MASK           PIC S9(4) COMP-5 VALUE 15.
      * Determines if incr/delta bckp.
       77  SQLUB-INCR-MASK           PIC S9(4) COMP-5 VALUE 240.

      * Backup calling action values
      * initial call
       77  SQLUB-BACKUP              PIC S9(4) COMP-5 VALUE 0.
      * Subsequent call
       77  SQLUB-CONTINUE            PIC S9(4) COMP-5 VALUE 1.
      * Terminating call
       77  SQLUB-TERMINATE           PIC S9(4) COMP-5 VALUE 2.
      * Initial call to run without prompting
       77  SQLUB-NOINTERRUPT         PIC S9(4) COMP-5 VALUE 3.
      * End media device.
       77  SQLUB-DEVICE-TERMINATE    PIC S9(4) COMP-5 VALUE 9.
      * for parameter check and initialization.
       77  SQLUB-PARM-CHECK          PIC S9(4) COMP-5 VALUE 10.
      * for parameter check and initialization.
       77  SQLUB-PARM-CHECK-INIT-AND-WAIT PIC S9(4) COMP-5 VALUE 10.
      * for parameter checking only.
       77  SQLUB-PARM-CHECK-ONLY     PIC S9(4) COMP-5 VALUE 11.

      * Restore rollforward mode
       77  SQLUD-ROLLFWD             PIC S9(4) COMP-5 VALUE 0.
       77  SQLUD-NOROLLFWD           PIC S9(4) COMP-5 VALUE 1.

      * Restore mode
      * Offline restore mode
       77  SQLUD-OFFLINE             PIC S9(4) COMP-5 VALUE 0.
      * Online restore mode
       77  SQLUD-ONLINE              PIC S9(4) COMP-5 VALUE 1.

      * Datalink mode
       77  SQLUD-DATALINK            PIC S9(4) COMP-5 VALUE 0.
       77  SQLUD-NODATALINK          PIC S9(4) COMP-5 VALUE 1.

      * Restore calling action values
      * initial call
       77  SQLUD-RESTORE             PIC S9(4) COMP-5 VALUE 0.
      * Subsequent call
       77  SQLUD-CONTINUE            PIC S9(4) COMP-5 VALUE 1.
      * Terminating call
       77  SQLUD-TERMINATE           PIC S9(4) COMP-5 VALUE 2.
      * Initial call to run without prompting
       77  SQLUD-NOINTERRUPT         PIC S9(4) COMP-5 VALUE 3.
      * End media device.
       77  SQLUD-DEVICE-TERMINATE    PIC S9(4) COMP-5 VALUE 9.
      * for parameter check and initialization.
       77  SQLUD-PARM-CHECK          PIC S9(4) COMP-5 VALUE 10.
      * for parameter check and initialization.
       77  SQLUD-PARM-CHECK-INIT-AND-WAIT PIC S9(4) COMP-5 VALUE 10.
      * for parameter checking only.
       77  SQLUD-PARM-CHECK-ONLY     PIC S9(4) COMP-5 VALUE 11.
      * list of tablespaces/containers.
       77  SQLUD-RESTORE-TABLESPACE-LIST PIC S9(4) COMP-5 VALUE 12.
      * abort incremental restore
       77  SQLUD-TERMINATE-INCREMENTAL PIC S9(4) COMP-5 VALUE 13.
      * initial call, tablespace container redefinition requested
       77  SQLUD-RESTORE-STORDEF     PIC S9(4) COMP-5 VALUE 100.
      * initial call, tablespace container redefinition requested without
      * prompting
       77  SQLUD-STORDEF-NOINTERRUPT PIC S9(4) COMP-5 VALUE 101.

      * Restore type
       77  SQLUD-DB                  PIC S9(4) COMP-5 VALUE 0.
       77  SQLUD-FULL                PIC S9(4) COMP-5 VALUE 0.
       77  SQLUD-ONLINE-TABLESPACE   PIC S9(4) COMP-5 VALUE 3.
       77  SQLUD-TABLESPACE          PIC S9(4) COMP-5 VALUE 4.
       77  SQLUD-HISTORY             PIC S9(4) COMP-5 VALUE 5.
       77  SQLUD-INCREMENTAL         PIC S9(4) COMP-5 VALUE 16.
       77  SQLUD-AUTOMATIC           PIC S9(4) COMP-5 VALUE 256.

      * The following are not supported in V2.1.0
       77  SQLUD-TABLESPACE-TBL      PIC S9(4) COMP-5 VALUE 6.
       77  SQLUD-HIST-TBLSPACE-TBL   PIC S9(4) COMP-5 VALUE 7.

      * Restore type masks
      * Determines if full/tbsp restore
       77  SQLUD-TYPE-MASK           PIC S9(4) COMP-5 VALUE 15.
      * Determines if incr. restore
       77  SQLUD-INCR-MASK           PIC S9(4) COMP-5 VALUE 240.

      * Backup/Restore Definitions
      * length of database alias name
       77  SQLU-ALIAS-SZ             PIC S9(4) COMP-5 VALUE 8.
      * Maximum length of userid
       77  SQLU-USERID-LEN           PIC S9(4) COMP-5 VALUE 1024.
      * length of password
       77  SQLU-PWD-LEN              PIC S9(4) COMP-5 VALUE 18.
      * length of the backup timestamp in yyyymmddhhmmss format
       77  SQLU-TIME-STAMP-LEN       PIC S9(4) COMP-5 VALUE 14.

      * length of filename of backup image:
      * dbalias.type.db2instance.node.timestamp.seq
       77  SQLU-BACKUP-NAME-SZ       PIC S9(4) COMP-5 VALUE 44.

      * length of directory where backup file will reside:dir - '/' -
      * filename
       77  SQLU-BACKUP-DIR-SZ        PIC S9(4) COMP-5 VALUE 978.

      * Data map separator for LOAD utility;  Partition Key Identifier
       77  SQL-HEADER-DATA-SEPARATOR PIC X(9) value "===PDB===".
       77  SQLU-PARTITIONKEY         PIC X(4) value "Key:".

      * Parameters for Backup/Restore old api's
      * User exit used, not SD interface
       77  SQLU-NSD-ID               PIC X(1) value "0".

      * Parameters for SQLUHPRN force option
       77  SQLUH-NO-FORCE            PIC S9(4) COMP-5 VALUE 0.
       77  SQLUH-FORCE               PIC S9(4) COMP-5 VALUE 1.

      * Parameters for SQLUHOPS caller action
       77  SQLUH-LIST-BACKUP         PIC S9(4) COMP-5 VALUE 0.
       77  SQLUH-LIST-HISTORY        PIC S9(4) COMP-5 VALUE 1.
       77  SQLUH-LIST-ADM-BACKUP     PIC S9(4) COMP-5 VALUE 2.
       77  SQLUH-LIST-ADM-ROLLFORWARD PIC S9(4) COMP-5 VALUE 3.
       77  SQLUH-LIST-ADM-RUNSTATS   PIC S9(4) COMP-5 VALUE 4.
       77  SQLUH-LIST-ADM-REORG      PIC S9(4) COMP-5 VALUE 5.
       77  SQLUH-LIST-ADM-ALT-TABLESPACE PIC S9(4) COMP-5 VALUE 6.
       77  SQLUH-LIST-ADM-DROPPED-TABLE PIC S9(4) COMP-5 VALUE 7.
       77  SQLUH-LIST-ADM-LOAD       PIC S9(4) COMP-5 VALUE 8.
       77  SQLUH-LIST-ADM-HISTORY    PIC S9(4) COMP-5 VALUE 9.

      * Parameters for SQLUHGNE caller action
       77  SQLUH-GET-NEXT-ENTRY      PIC S9(4) COMP-5 VALUE 0.
       77  SQLUH-GET-DDL             PIC S9(4) COMP-5 VALUE 1.
       77  SQLUH-GET-NEXT-ENTRY-DDL  PIC S9(4) COMP-5 VALUE 2.

      * History File structure sizes
       77  SQLUH-OP-SZ               PIC S9(4) COMP-5 VALUE 1.
       77  SQLUH-OBJ-SZ              PIC S9(4) COMP-5 VALUE 1.
       77  SQLUH-OBJPART-SZ          PIC S9(4) COMP-5 VALUE 17.
       77  SQLUH-OPTYPE-SZ           PIC S9(4) COMP-5 VALUE 1.
       77  SQLUH-DEVTYPE-SZ          PIC S9(4) COMP-5 VALUE 1.
       77  SQLUH-FIRSTLOG-SZ         PIC S9(4) COMP-5 VALUE 12.
       77  SQLUH-LASTLOG-SZ          PIC S9(4) COMP-5 VALUE 12.
       77  SQLUH-BACKID-SZ           PIC S9(4) COMP-5 VALUE 14.
       77  SQLUH-TCREATE-SZ          PIC S9(4) COMP-5 VALUE 8.
       77  SQLUH-TNAME-SZ            PIC S9(4) COMP-5 VALUE 18.
       77  SQLUH-NUMTABLESPACE-SZ    PIC S9(4) COMP-5 VALUE 5.
       77  SQLUH-LOC-SZ              PIC S9(4) COMP-5 VALUE 255.
       77  SQLUH-COMMENT-SZ          PIC S9(4) COMP-5 VALUE 30.
       77  SQLUH-TABLESPACENAME-SZ   PIC S9(4) COMP-5 VALUE 18.
       77  SQLUH-TIMESTAMP-SZ        PIC S9(4) COMP-5 VALUE 14.
       77  SQLUH-ID-SZ               PIC S9(4) COMP-5 VALUE 24.

      * Parameters for RUNSTATS
      * TABLEOPT = Table w/o Indexes
       77  SQL-STATS-TABLE           PIC X(1) value "T".
      * TABLEOPT = Table and Indexes
       77  SQL-STATS-BOTH            PIC X(1) value "B".
      * TABLEOPT = Indexes w/o Table
       77  SQL-STATS-INDEX           PIC X(1) value "I".
      * TABLEOPT = Table and dist stats
       77  SQL-STATS-EXTTABLE-ONLY   PIC X(1) value "D".
      * TABLEOPT = Table and dist stats and basic indexes
       77  SQL-STATS-EXTTABLE-INDEX  PIC X(1) value "E".
      * TABLEOPT = Ext stats for indexes only
       77  SQL-STATS-EXTINDEX-ONLY   PIC X(1) value "X".
      * TABLEOPT = Ext stats for indexes and basic table stats
       77  SQL-STATS-EXTINDEX-TABLE  PIC X(1) value "Y".
      * TABLEOPT = Ext stats for indexes and table with dist stats
       77  SQL-STATS-ALL             PIC X(1) value "A".
      * SHAREOPT = Reference
       77  SQL-STATS-REF             PIC X(1) value "R".
      * TABLEOPT - statistics are to be gathered using the profile - Load
      * only
       77  SQLU-STATS-USE-PROFILE    PIC X(1) value "P".
      * SHAREOPT = Change
       77  SQL-STATS-CHG             PIC X(1) value "C".

      * TABLEOPT - no statistics are to be gathered - Load only
       77  SQLU-STATS-NONE           PIC X(1) value " ".

      * Parameters for selection method in sqldcol
      * Names
       77  SQL-METH-N                PIC S9(4) COMP-5 VALUE 78.
      * Positional
       77  SQL-METH-P                PIC S9(4) COMP-5 VALUE 80.
      * Default
       77  SQL-METH-D                PIC S9(4) COMP-5 VALUE 68.
      * Locations
       77  SQL-METH-L                PIC S9(4) COMP-5 VALUE 76.

      * File Types for Import/Export
       77  SQL-IXF                   PIC X(3) value "IXF".
       77  SQL-WSF                   PIC X(3) value "WSF".
       77  SQL-DEL                   PIC X(3) value "DEL".
       77  SQL-ASC                   PIC X(3) value "ASC".
       77  SQL-CURSOR                PIC X(6) value "CURSOR".
      * load/unload
       77  SQL-DB2CS                 PIC X(5) value "DB2CS".

      * Import/Export/Load/Unload calling action values
      * Initial call
       77  SQLU-INITIAL              PIC S9(4) COMP-5 VALUE 0.
      * Subsequent call
       77  SQLU-CONTINUE             PIC S9(4) COMP-5 VALUE 1.
      * Terminating call
       77  SQLU-TERMINATE            PIC S9(4) COMP-5 VALUE 2.
      * Initial call - Load/Unload only
       77  SQLU-NOINTERRUPT          PIC S9(4) COMP-5 VALUE 3.
      * Abort call - Load only
       77  SQLU-ABORT                PIC S9(4) COMP-5 VALUE 4.
      * Restart call - Load only
       77  SQLU-RESTART              PIC S9(4) COMP-5 VALUE 5.
      * Subsequent call - Load/Unload only
       77  SQLU-DEVICE-TERMINATE     PIC S9(4) COMP-5 VALUE 9.

      * Actions for quiesce tablespaces for table
       77  SQLU-QUIESCEMODE-SHARE    PIC S9(4) COMP-5 VALUE 0.
       77  SQLU-QUIESCEMODE-INTENT-UPDATE PIC S9(4) COMP-5 VALUE 1.
       77  SQLU-QUIESCEMODE-EXCLUSIVE PIC S9(4) COMP-5 VALUE 2.
       77  SQLU-QUIESCEMODE-RESET    PIC S9(4) COMP-5 VALUE 9.
       77  SQLU-QUIESCEMODE-RESET-OWNED PIC S9(4) COMP-5 VALUE 10.

      *************************************************************************
      * Equates for Configuration Utilities
      **************************************************************************

      * Token equates for SQLFupd structure for Database Manager config file
       77  SQLF-KTN-RQRIOBLK         PIC S9(4) COMP-5 VALUE 1.
       77  SQLF-KTN-NUMDB            PIC S9(4) COMP-5 VALUE 6.
       77  SQLF-KTN-NNAME            PIC S9(4) COMP-5 VALUE 7.
       77  SQLF-KTN-MAXAGENTS        PIC S9(4) COMP-5 VALUE 12.
       77  SQLF-KTN-MAXCAGENTS       PIC S9(4) COMP-5 VALUE 13.
       77  SQLF-KTN-ASLHEAPSZ        PIC S9(4) COMP-5 VALUE 15.
       77  SQLF-KTN-BACKBUFSZ        PIC S9(4) COMP-5 VALUE 18.
       77  SQLF-KTN-RESTBUFSZ        PIC S9(4) COMP-5 VALUE 19.
       77  SQLF-KTN-INDEXREC         PIC S9(4) COMP-5 VALUE 20.
       77  SQLF-KTN-SHEAPTHRES       PIC S9(4) COMP-5 VALUE 21.
       77  SQLF-KTN-MAXDARI          PIC S9(4) COMP-5 VALUE 80.
       77  SQLF-KTN-FENCED-POOL      PIC S9(4) COMP-5 VALUE 80.
       77  SQLF-KTN-KEEPDARI         PIC S9(4) COMP-5 VALUE 81.
       77  SQLF-KTN-KEEPFENCED       PIC S9(4) COMP-5 VALUE 81.
       77  SQLF-KTN-SVCENAME         PIC S9(4) COMP-5 VALUE 24.
       77  SQLF-KTN-TPNAME           PIC S9(4) COMP-5 VALUE 25.
       77  SQLF-KTN-AGENTPRI         PIC S9(4) COMP-5 VALUE 26.
       77  SQLF-KTN-DFTDBPATH        PIC S9(4) COMP-5 VALUE 27.
      * You may manipulate the bits of SQLF_KTN_DFT_MONSWITCHES or you can
      * enable disable, or query the current setting through the individual
      * tokens
       77  SQLF-KTN-DFT-MONSWITCHES  PIC S9(4) COMP-5 VALUE 29.
      * Bit 1 of SQLF_KTN_DFT_MONSWITCHES
       77  SQLF-KTN-DFT-MON-UOW      PIC S9(4) COMP-5 VALUE 30.
      * Bit 2 of SQLF_KTN_DFT_MONSWITCHES
       77  SQLF-KTN-DFT-MON-STMT     PIC S9(4) COMP-5 VALUE 31.
      * Bit 3 of SQLF_KTN_DFT_MONSWITCHES
       77  SQLF-KTN-DFT-MON-TABLE    PIC S9(4) COMP-5 VALUE 32.
      * Bit 4 of SQLF_KTN_DFT_MONSWITCHES
       77  SQLF-KTN-DFT-MON-BUFPOOL  PIC S9(4) COMP-5 VALUE 33.
      * Bit 5 of SQLF_KTN_DFT_MONSWITCHES
       77  SQLF-KTN-DFT-MON-LOCK     PIC S9(4) COMP-5 VALUE 34.
      * Bit 6 of SQLF_KTN_DFT_MONSWITCHES
       77  SQLF-KTN-DFT-MON-SORT     PIC S9(4) COMP-5 VALUE 35.
      * Bit 7 of SQLF_KTN_DFT_MONSWITCHES
       77  SQLF-KTN-DFT-MON-TIMESTAMP PIC S9(4) COMP-5 VALUE 36.
       77  SQLF-KTN-DFT-ACCOUNT-STR  PIC S9(4) COMP-5 VALUE 28.
       77  SQLF-KTN-SYSADM-GROUP     PIC S9(4) COMP-5 VALUE 39.
       77  SQLF-KTN-DIR-CACHE        PIC S9(4) COMP-5 VALUE 40.
       77  SQLF-KTN-DRDA-HEAP-SZ     PIC S9(4) COMP-5 VALUE 41.
       77  SQLF-KTN-CPUSPEED         PIC S9(4) COMP-5 VALUE 42.
       77  SQLF-KTN-MIN-PRIV-MEM     PIC S9(4) COMP-5 VALUE 43.
       77  SQLF-KTN-PRIV-MEM-THRESH  PIC S9(4) COMP-5 VALUE 44.
       77  SQLF-KTN-MAXTOTFILOP      PIC S9(4) COMP-5 VALUE 45.
       77  SQLF-KTN-FILESERVER       PIC S9(4) COMP-5 VALUE 47.
       77  SQLF-KTN-OBJECTNAME       PIC S9(4) COMP-5 VALUE 48.
       77  SQLF-KTN-QUERY-HEAP-SZ    PIC S9(4) COMP-5 VALUE 49.
       77  SQLF-KTN-AGENT-STACK-SZ   PIC S9(4) COMP-5 VALUE 61.
       77  SQLF-KTN-SYSMAINT-GROUP   PIC S9(4) COMP-5 VALUE 62.
       77  SQLF-KTN-SYSCTRL-GROUP    PIC S9(4) COMP-5 VALUE 63.
       77  SQLF-KTN-DIAGLEVEL        PIC S9(4) COMP-5 VALUE 64.
       77  SQLF-KTN-DIAGPATH         PIC S9(4) COMP-5 VALUE 65.
       77  SQLF-KTN-TP-MON-NAME      PIC S9(4) COMP-5 VALUE 66.
       77  SQLF-KTN-TM-DATABASE      PIC S9(4) COMP-5 VALUE 67.
       77  SQLF-KTN-RESYNC-INTERVAL  PIC S9(4) COMP-5 VALUE 68.
       77  SQLF-KTN-UDF-MEM-SZ       PIC S9(4) COMP-5 VALUE 69.
       77  SQLF-KTN-IPX-SOCKET       PIC S9(4) COMP-5 VALUE 71.
       77  SQLF-KTN-DOS-RQRIOBLK     PIC S9(4) COMP-5 VALUE 72.
       77  SQLF-KTN-DIR-TYPE         PIC S9(4) COMP-5 VALUE 73.
       77  SQLF-KTN-DIR-PATH-NAME    PIC S9(4) COMP-5 VALUE 74.
       77  SQLF-KTN-DIR-OBJ-NAME     PIC S9(4) COMP-5 VALUE 75.
       77  SQLF-KTN-ROUTE-OBJ-NAME   PIC S9(4) COMP-5 VALUE 76.
       77  SQLF-KTN-DFT-CLIENT-COMM  PIC S9(4) COMP-5 VALUE 77.
       77  SQLF-KTN-DFT-CLIENT-ADPT  PIC S9(4) COMP-5 VALUE 82.
       77  SQLF-KTN-AUTHENTICATION   PIC S9(4) COMP-5 VALUE 78.
       77  SQLF-KTN-MON-HEAP-SZ      PIC S9(4) COMP-5 VALUE 79.
       77  SQLF-KTN-SPM-LOG-FILE-SZ  PIC S9(4) COMP-5 VALUE 90.
       77  SQLF-KTN-SPM-MAX-RESYNC   PIC S9(4) COMP-5 VALUE 91.
       77  SQLF-KTN-SPM-NAME         PIC S9(4) COMP-5 VALUE 92.
       77  SQLF-KTN-NUM-INITAGENTS   PIC S9(4) COMP-5 VALUE 500.
       77  SQLF-KTN-MAX-COORDAGENTS  PIC S9(4) COMP-5 VALUE 501.
       77  SQLF-KTN-NUM-POOLAGENTS   PIC S9(4) COMP-5 VALUE 502.
       77  SQLF-KTN-FCM-NUM-BUFFERS  PIC S9(4) COMP-5 VALUE 503.
       77  SQLF-KTN-FCM-NUM-RQB      PIC S9(4) COMP-5 VALUE 504.
       77  SQLF-KTN-FCM-NUM-CONNECT  PIC S9(4) COMP-5 VALUE 505.
       77  SQLF-KTN-FCM-NUM-ANCHORS  PIC S9(4) COMP-5 VALUE 506.
       77  SQLF-KTN-CONN-ELAPSE      PIC S9(4) COMP-5 VALUE 508.
       77  SQLF-KTN-MAX-CONNRETRIES  PIC S9(4) COMP-5 VALUE 509.
       77  SQLF-KTN-MAX-TIME-DIFF    PIC S9(4) COMP-5 VALUE 510.
       77  SQLF-KTN-START-STOP-TIME  PIC S9(4) COMP-5 VALUE 511.
       77  SQLF-KTN-TRUST-ALLCLNTS   PIC S9(4) COMP-5 VALUE 301.
       77  SQLF-KTN-TRUST-CLNTAUTH   PIC S9(4) COMP-5 VALUE 302.
       77  SQLF-KTN-MAX-QUERYDEGREE  PIC S9(4) COMP-5 VALUE 303.
       77  SQLF-KTN-DISCOVER         PIC S9(4) COMP-5 VALUE 304.
       77  SQLF-KTN-DISCOVER-COMM    PIC S9(4) COMP-5 VALUE 305.
       77  SQLF-KTN-INTRA-PARALLEL   PIC S9(4) COMP-5 VALUE 306.
       77  SQLF-KTN-COMM-BANDWIDTH   PIC S9(4) COMP-5 VALUE 307.
       77  SQLF-KTN-DISCOVER-INST    PIC S9(4) COMP-5 VALUE 308.
       77  SQLF-KTN-SS-LOGON         PIC S9(4) COMP-5 VALUE 309.
       77  SQLF-KTN-JAVA-HEAP-SZ     PIC S9(4) COMP-5 VALUE 310.
       77  SQLF-KTN-JDK11-PATH       PIC S9(4) COMP-5 VALUE 311.
       77  SQLF-KTN-JDK-PATH         PIC S9(4) COMP-5 VALUE 311.
       77  SQLF-KTN-AUDIT-BUF-SZ     PIC S9(4) COMP-5 VALUE 312.
       77  SQLF-KTN-SPM-LOG-PATH     PIC S9(4) COMP-5 VALUE 313.
       77  SQLF-KTN-CATALOG-NOAUTH   PIC S9(4) COMP-5 VALUE 314.
       77  SQLF-KTN-NUM-INITDARIS    PIC S9(4) COMP-5 VALUE 601.
       77  SQLF-KTN-NUM-INITFENCED   PIC S9(4) COMP-5 VALUE 601.
       77  SQLF-KTN-INITDARI-JVM     PIC S9(4) COMP-5 VALUE 602.
       77  SQLF-KTN-INITFENCED-JVM   PIC S9(4) COMP-5 VALUE 602.
       77  SQLF-KTN-DATALINKS        PIC S9(4) COMP-5 VALUE 603.
       77  SQLF-KTN-FEDERATED        PIC S9(4) COMP-5 VALUE 604.
       77  SQLF-KTN-NOTIFYLEVEL      PIC S9(4) COMP-5 VALUE 605.
       77  SQLF-KTN-MAX-LOGICAGENTS  PIC S9(4) COMP-5 VALUE 701.
       77  SQLF-KTN-DF-ENCRYPT-LIST  PIC S9(4) COMP-5 VALUE 801.
       77  SQLF-KTN-MAX-CONNECTIONS  PIC S9(4) COMP-5 VALUE 802.
       77  SQLF-KTN-INSTANCE-MEMORY  PIC S9(4) COMP-5 VALUE 803.
       77  SQLF-KTN-HEALTH-MON       PIC S9(4) COMP-5 VALUE 804.
       77  SQLF-KTN-USE-SNA-AUTH     PIC S9(4) COMP-5 VALUE 805.
       77  SQLF-KTN-FED-NOAUTH       PIC S9(4) COMP-5 VALUE 806.
       77  SQLF-KTN-UTIL-IMPACT-LIM  PIC S9(4) COMP-5 VALUE 807.
       77  SQLF-KTN-SRV-PLUGIN-MODE  PIC S9(4) COMP-5 VALUE 809.
       77  SQLF-KTN-GROUP-PLUGIN     PIC S9(4) COMP-5 VALUE 810.
       77  SQLF-KTN-CLNT-PW-PLUGIN   PIC S9(4) COMP-5 VALUE 811.
       77  SQLF-KTN-CLNT-KRB-PLUGIN  PIC S9(4) COMP-5 VALUE 812.
       77  SQLF-KTN-SRVCON-PW-PLUGIN PIC S9(4) COMP-5 VALUE 813.
       77  SQLF-KTN-SRVCON-GSSPLUGIN-LIST PIC S9(4) COMP-5 VALUE 814.
       77  SQLF-KTN-SRVCON-AUTH      PIC S9(4) COMP-5 VALUE 815.
       77  SQLF-KTN-LOCAL-GSSPLUGIN  PIC S9(4) COMP-5 VALUE 816.
       77  SQLF-KTN-SYSMON-GROUP     PIC S9(4) COMP-5 VALUE 808.
       77  SQLF-KTN-SYSADM-GRP       PIC S9(4) COMP-5 VALUE 844.
       77  SQLF-KTN-SYSMAINT-GRP     PIC S9(4) COMP-5 VALUE 845.
       77  SQLF-KTN-SYSCTRL-GRP      PIC S9(4) COMP-5 VALUE 846.
       77  SQLF-KTN-SYSMON-GRP       PIC S9(4) COMP-5 VALUE 847.
       77  SQLF-KTN-FCM-PARALLELISM  PIC S9(4) COMP-5 VALUE 848.
      * tokens for informational parameters
       77  SQLF-KTN-NODETYPE         PIC S9(4) COMP-5 VALUE 100.
       77  SQLF-KTN-RELEASE          PIC S9(4) COMP-5 VALUE 101.
       77  SQLF-KTN-NOTOKEN          PIC S9(4) COMP-5 VALUE 200.

      *************************************************************************
      * defines for database manager configuration parameter values
      **************************************************************************
      * equates for directory type (for dir_type configuration parameter)
       77  SQLF-DIRTYPE-NONE         PIC S9(4) COMP-5 VALUE 0.
       77  SQLF-DIRTYPE-DCE          PIC S9(4) COMP-5 VALUE 1.

      * Nodetype equates
       77  SQLF-NT-STANDALONE        PIC S9(4) COMP-5 VALUE 0.
       77  SQLF-NT-SERVER            PIC S9(4) COMP-5 VALUE 1.
       77  SQLF-NT-REQUESTOR         PIC S9(4) COMP-5 VALUE 2.
       77  SQLF-NT-STAND-REQ         PIC S9(4) COMP-5 VALUE 3.
       77  SQLF-NT-MPP               PIC S9(4) COMP-5 VALUE 4.
       77  SQLF-NT-SATELLITE         PIC S9(4) COMP-5 VALUE 5.
      * Note: SQLF_NT_DCS is not a possible instance nodetype.  It
      * represents a host database server
      * reserved for host database server
       77  SQLF-NT-DCS               PIC S9(4) COMP-5 VALUE 6.
      * Note: SQLF_NT_DAS is not a possible instance nodetype.  It
      * represents an ADMIN server
      * reserved for ADMIN server
       77  SQLF-NT-DAS               PIC S9(4) COMP-5 VALUE 7.

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
       77  SQLF-ENABLE-MON-UOW       PIC S9(4) COMP-5 VALUE 1.
      * xxxx xxxx xxxx xx1x
       77  SQLF-ENABLE-MON-STMT      PIC S9(4) COMP-5 VALUE 2.
      * xxxx xxxx xxxx x1xx
       77  SQLF-ENABLE-MON-TABLE     PIC S9(4) COMP-5 VALUE 4.
      * xxxx xxxx xxxx 1xxx
       77  SQLF-ENABLE-MON-BUFPOOL   PIC S9(4) COMP-5 VALUE 8.
      * xxxx xxxx xxx1 xxxx
       77  SQLF-ENABLE-MON-LOCK      PIC S9(4) COMP-5 VALUE 16.
      * xxxx xxxx xx1x xxxx
       77  SQLF-ENABLE-MON-SORT      PIC S9(4) COMP-5 VALUE 32.
      * xxxx xxxx x1xx xxxx
       77  SQLF-ENABLE-MON-TIMESTAMP PIC S9(4) COMP-5 VALUE 64.

      * Communication Protocols
       77  SQLF-PROTOCOL-TCPIP       PIC X(5) value "TCPIP".
       77  SQLF-PROTOCOL-APPC        PIC X(4) value "APPC".
       77  SQLF-PROTOCOL-NETB        PIC X(7) value "NETBIOS".
       77  SQLF-PROTOCOL-IPXSPX      PIC X(6) value "IPXSPX".

      * Admin. server discover definitions (discover parameter)
       77  SQLF-DSCVR-KNOWN          PIC S9(4) COMP-5 VALUE 1.
       77  SQLF-DSCVR-SEARCH         PIC S9(4) COMP-5 VALUE 2.

      * Constants for trusted client options
       77  SQLF-TRUST-ALLCLNTS-NO    PIC S9(4) COMP-5 VALUE 0.
       77  SQLF-TRUST-ALLCLNTS-YES   PIC S9(4) COMP-5 VALUE 1.
       77  SQLF-TRUST-ALLCLNTS-DRDAONLY PIC S9(4) COMP-5 VALUE 2.

      * Token equates for SQLFupd structure for Database configuration file
       77  SQLF-DBTN-BUFF-PAGE       PIC S9(4) COMP-5 VALUE 90.
       77  SQLF-DBTN-MAXFILOP        PIC S9(4) COMP-5 VALUE 3.
       77  SQLF-DBTN-SOFTMAX         PIC S9(4) COMP-5 VALUE 5.
       77  SQLF-DBTN-MAXAPPLS        PIC S9(4) COMP-5 VALUE 6.
       77  SQLF-DBTN-APPLHEAPSZ      PIC S9(4) COMP-5 VALUE 51.
       77  SQLF-DBTN-DB-HEAP         PIC S9(4) COMP-5 VALUE 58.
       77  SQLF-DBTN-DLCHKTIME       PIC S9(4) COMP-5 VALUE 9.
       77  SQLF-DBTN-SORT-HEAP       PIC S9(4) COMP-5 VALUE 52.
       77  SQLF-DBTN-MAXLOCKS        PIC S9(4) COMP-5 VALUE 15.
       77  SQLF-DBTN-LOGPRIMARY      PIC S9(4) COMP-5 VALUE 16.
       77  SQLF-DBTN-LOGSECOND       PIC S9(4) COMP-5 VALUE 17.
       77  SQLF-DBTN-LOGFIL-SIZ      PIC S9(4) COMP-5 VALUE 92.
       77  SQLF-DBTN-STMTHEAP        PIC S9(4) COMP-5 VALUE 53.
       77  SQLF-DBTN-NEWLOGPATH      PIC S9(4) COMP-5 VALUE 20.
       77  SQLF-DBTN-COPY-PROTECT    PIC S9(4) COMP-5 VALUE 22.
       77  SQLF-DBTN-LOG-RETAIN      PIC S9(4) COMP-5 VALUE 23.
       77  SQLF-DBTN-USER-EXIT       PIC S9(4) COMP-5 VALUE 24.
       77  SQLF-DBTN-AUTO-RESTART    PIC S9(4) COMP-5 VALUE 25.
       77  SQLF-DBTN-INDEXREC        PIC S9(4) COMP-5 VALUE 30.
       77  SQLF-DBTN-PCKCACHESZ      PIC S9(4) COMP-5 VALUE 31.
       77  SQLF-DBTN-MINCOMMIT       PIC S9(4) COMP-5 VALUE 32.
       77  SQLF-DBTN-LOGBUFSZ        PIC S9(4) COMP-5 VALUE 33.
       77  SQLF-DBTN-LOCKTIMEOUT     PIC S9(4) COMP-5 VALUE 34.
       77  SQLF-DBTN-INDEXSORT       PIC S9(4) COMP-5 VALUE 35.
       77  SQLF-DBTN-NUM-FREQVALUES  PIC S9(4) COMP-5 VALUE 36.
       77  SQLF-DBTN-NUM-IOCLEANERS  PIC S9(4) COMP-5 VALUE 37.
       77  SQLF-DBTN-CHNGPGS-THRESH  PIC S9(4) COMP-5 VALUE 38.
       77  SQLF-DBTN-NUM-IOSERVERS   PIC S9(4) COMP-5 VALUE 39.
       77  SQLF-DBTN-DFT-PREFETCH-SZ PIC S9(4) COMP-5 VALUE 40.
       77  SQLF-DBTN-SEQDETECT       PIC S9(4) COMP-5 VALUE 41.
       77  SQLF-DBTN-DFT-LOADREC-SES PIC S9(4) COMP-5 VALUE 42.
       77  SQLF-DBTN-REC-HIS-RETENTN PIC S9(4) COMP-5 VALUE 43.
       77  SQLF-DBTN-STAT-HEAP-SZ    PIC S9(4) COMP-5 VALUE 45.
      * token for directory services
       77  SQLF-DBTN-DIR-OBJ-NAME    PIC S9(4) COMP-5 VALUE 46.
       77  SQLF-DBTN-AVG-APPLS       PIC S9(4) COMP-5 VALUE 47.
       77  SQLF-DBTN-NUM-QUANTILES   PIC S9(4) COMP-5 VALUE 48.
       77  SQLF-DBTN-DFT-EXTENT-SZ   PIC S9(4) COMP-5 VALUE 54.
       77  SQLF-DBTN-UTIL-HEAP-SZ    PIC S9(4) COMP-5 VALUE 55.
       77  SQLF-DBTN-CATALOGCACHE-SZ PIC S9(4) COMP-5 VALUE 56.
       77  SQLF-DBTN-DFT-QUERYOPT    PIC S9(4) COMP-5 VALUE 57.
       77  SQLF-DBTN-APP-CTL-HEAP-SZ PIC S9(4) COMP-5 VALUE 500.
       77  SQLF-DBTN-TSM-PASSWORD    PIC S9(4) COMP-5 VALUE 501.
       77  SQLF-DBTN-PCKCACHE-SZ     PIC S9(4) COMP-5 VALUE 505.
       77  SQLF-DBTN-MULTIPAGE-ALLOC PIC S9(4) COMP-5 VALUE 506.
       77  SQLF-DBTN-DFT-DEGREE      PIC S9(4) COMP-5 VALUE 301.
       77  SQLF-DBTN-ESTORE-SEG-SZ   PIC S9(4) COMP-5 VALUE 303.
       77  SQLF-DBTN-NUM-ESTORE-SEGS PIC S9(4) COMP-5 VALUE 304.
       77  SQLF-DBTN-TSM-OWNER       PIC S9(4) COMP-5 VALUE 305.
       77  SQLF-DBTN-TSM-NODENAME    PIC S9(4) COMP-5 VALUE 306.
       77  SQLF-DBTN-TSM-MGMTCLASS   PIC S9(4) COMP-5 VALUE 307.
       77  SQLF-DBTN-DISCOVER        PIC S9(4) COMP-5 VALUE 308.
       77  SQLF-DBTN-DFT-SQLMATHWARN PIC S9(4) COMP-5 VALUE 309.
       77  SQLF-DBTN-DL-EXPINT       PIC S9(4) COMP-5 VALUE 350.
       77  SQLF-DBTN-DL-NUM-COPIES   PIC S9(4) COMP-5 VALUE 351.
       77  SQLF-DBTN-DL-TIME-DROP    PIC S9(4) COMP-5 VALUE 353.
       77  SQLF-DBTN-NUM-DB-BACKUPS  PIC S9(4) COMP-5 VALUE 601.
       77  SQLF-DBTN-DL-TOKEN        PIC S9(4) COMP-5 VALUE 602.
       77  SQLF-DBTN-DL-UPPER        PIC S9(4) COMP-5 VALUE 603.
       77  SQLF-DBTN-DL-WT-IEXPINT   PIC S9(4) COMP-5 VALUE 354.
       77  SQLF-DBTN-DYN-QUERY-MGMT  PIC S9(4) COMP-5 VALUE 604.
       77  SQLF-DBTN-MIN-DEC-DIV-3   PIC S9(4) COMP-5 VALUE 605.
       77  SQLF-DBTN-DFT-REFRESH-AGE PIC S9(4) COMP-5 VALUE 702.
       77  SQLF-DBTN-LOCK-LIST       PIC S9(4) COMP-5 VALUE 704.
       77  SQLF-DBTN-TRACKMOD        PIC S9(4) COMP-5 VALUE 703.
       77  SQLF-DBTN-APPGROUP-MEM-SZ PIC S9(4) COMP-5 VALUE 800.
       77  SQLF-DBTN-GROUPHEAP-RATIO PIC S9(4) COMP-5 VALUE 801.
       77  SQLF-DBTN-SHEAPTHRES-SHR  PIC S9(4) COMP-5 VALUE 802.
       77  SQLF-DBTN-DATABASE-MEMORY PIC S9(4) COMP-5 VALUE 803.
       77  SQLF-DBTN-BLK-LOG-DSK-FUL PIC S9(4) COMP-5 VALUE 804.
       77  SQLF-DBTN-OVERFLOWLOGPATH PIC S9(4) COMP-5 VALUE 805.
       77  SQLF-DBTN-MIRRORLOGPATH   PIC S9(4) COMP-5 VALUE 806.
       77  SQLF-DBTN-MAX-LOG         PIC S9(4) COMP-5 VALUE 807.
       77  SQLF-DBTN-NUM-LOG-SPAN    PIC S9(4) COMP-5 VALUE 808.
       77  SQLF-DBTN-ALT-COLLATE     PIC S9(4) COMP-5 VALUE 809.
       77  SQLF-DBTN-HADR-DB-ROLE    PIC S9(4) COMP-5 VALUE 810.
       77  SQLF-DBTN-HADR-LOCAL-HOST PIC S9(4) COMP-5 VALUE 811.
       77  SQLF-DBTN-HADR-LOCAL-SVC  PIC S9(4) COMP-5 VALUE 812.
       77  SQLF-DBTN-HADR-REMOTE-HOST PIC S9(4) COMP-5 VALUE 813.
       77  SQLF-DBTN-HADR-REMOTE-SVC PIC S9(4) COMP-5 VALUE 814.
       77  SQLF-DBTN-HADR-REMOTE-INST PIC S9(4) COMP-5 VALUE 815.
       77  SQLF-DBTN-HADR-TIMEOUT    PIC S9(4) COMP-5 VALUE 816.
       77  SQLF-DBTN-HADR-SYNCMODE   PIC S9(4) COMP-5 VALUE 817.
       77  SQLF-DBTN-LOGINDEXBUILD   PIC S9(4) COMP-5 VALUE 818.
       77  SQLF-DBTN-SHEAPTHRES      PIC S9(4) COMP-5 VALUE 820.
       77  SQLF-DBTN-STMT-HEAP       PIC S9(4) COMP-5 VALUE 821.
       77  SQLF-DBTN-LOGARCHMETH1    PIC S9(4) COMP-5 VALUE 822.
       77  SQLF-DBTN-LOGARCHMETH2    PIC S9(4) COMP-5 VALUE 823.
       77  SQLF-DBTN-LOGARCHOPT1     PIC S9(4) COMP-5 VALUE 824.
       77  SQLF-DBTN-LOGARCHOPT2     PIC S9(4) COMP-5 VALUE 825.
       77  SQLF-DBTN-FAILARCHPATH    PIC S9(4) COMP-5 VALUE 826.
       77  SQLF-DBTN-NUMARCHRETRY    PIC S9(4) COMP-5 VALUE 827.
       77  SQLF-DBTN-ARCHRETRYDELAY  PIC S9(4) COMP-5 VALUE 828.
       77  SQLF-DBTN-VENDOROPT       PIC S9(4) COMP-5 VALUE 829.
       77  SQLF-DBTN-AUTONOMIC-SWITCHES PIC S9(4) COMP-5 VALUE 830.
       77  SQLF-DBTN-AUTO-MAINT      PIC S9(4) COMP-5 VALUE 831.
       77  SQLF-DBTN-AUTO-MAINT-EFF  PIC S9(4) COMP-5 VALUE 832.
       77  SQLF-DBTN-AUTO-DB-BACKUP  PIC S9(4) COMP-5 VALUE 833.
       77  SQLF-DBTN-AUTO-DB-BACKUP-EFF PIC S9(4) COMP-5 VALUE 834.
       77  SQLF-DBTN-AUTO-TBL-MAINT  PIC S9(4) COMP-5 VALUE 835.
       77  SQLF-DBTN-AUTO-TBL-MAINT-EFF PIC S9(4) COMP-5 VALUE 836.
       77  SQLF-DBTN-AUTO-RUNSTATS   PIC S9(4) COMP-5 VALUE 837.
       77  SQLF-DBTN-AUTO-RUNSTATS-EFF PIC S9(4) COMP-5 VALUE 838.
       77  SQLF-DBTN-AUTO-STATS-PROF PIC S9(4) COMP-5 VALUE 839.
       77  SQLF-DBTN-AUTO-STATS-PROF-EFF PIC S9(4) COMP-5 VALUE 840.
       77  SQLF-DBTN-AUTO-PROF-UPD   PIC S9(4) COMP-5 VALUE 844.
       77  SQLF-DBTN-AUTO-PROF-UPD-EFF PIC S9(4) COMP-5 VALUE 845.
       77  SQLF-DBTN-PAGESIZE        PIC S9(4) COMP-5 VALUE 846.
       77  SQLF-DBTN-AUTO-REORG      PIC S9(4) COMP-5 VALUE 841.
       77  SQLF-DBTN-AUTO-REORG-EFF  PIC S9(4) COMP-5 VALUE 842.
       77  SQLF-DBTN-DFT-MTTB-TYPES  PIC S9(4) COMP-5 VALUE 843.
      * tokens for informational (non-modifiable) parameters
       77  SQLF-DBTN-COLLATE-INFO    PIC S9(4) COMP-5 VALUE 44.
       77  SQLF-DBTN-COUNTRY         PIC S9(4) COMP-5 VALUE 100.
       77  SQLF-DBTN-TERRITORY-CODE  PIC S9(4) COMP-5 VALUE 100.
       77  SQLF-DBTN-CODEPAGE        PIC S9(4) COMP-5 VALUE 101.
       77  SQLF-DBTN-RELEASE         PIC S9(4) COMP-5 VALUE 102.
       77  SQLF-DBTN-LOGPATH         PIC S9(4) COMP-5 VALUE 103.
       77  SQLF-DBTN-LOGHEAD         PIC S9(4) COMP-5 VALUE 105.
       77  SQLF-DBTN-CONSISTENT      PIC S9(4) COMP-5 VALUE 111.
       77  SQLF-DBTN-BACKUP-PENDING  PIC S9(4) COMP-5 VALUE 112.
       77  SQLF-DBTN-ROLLFWD-PENDING PIC S9(4) COMP-5 VALUE 113.
       77  SQLF-DBTN-LOG-RETAIN-STATUS PIC S9(4) COMP-5 VALUE 114.
       77  SQLF-DBTN-USER-EXIT-STATUS PIC S9(4) COMP-5 VALUE 115.
       77  SQLF-DBTN-CODESET         PIC S9(4) COMP-5 VALUE 120.
       77  SQLF-DBTN-TERRITORY       PIC S9(4) COMP-5 VALUE 121.
       77  SQLF-DBTN-NUMSEGS         PIC S9(4) COMP-5 VALUE 122.
       77  SQLF-DBTN-DATABASE-LEVEL  PIC S9(4) COMP-5 VALUE 124.
       77  SQLF-DBTN-NOTOKEN         PIC S9(4) COMP-5 VALUE 200.
       77  SQLF-DBTN-RESTORE-PENDING PIC S9(4) COMP-5 VALUE 503.

      * defines for database configuration parameter values

      * Generic Disable/Enable values for configuration parameters
       77  SQLF-DISABLE              PIC S9(4) COMP-5 VALUE 0.
       77  SQLF-ENABLE               PIC S9(4) COMP-5 VALUE 1.

      * Index recreate type (INDEXREC values)
       77  SQLF-INX-REC-SYSTEM       PIC S9(4) COMP-5 VALUE 0.
       77  SQLF-INX-REC-REFERENCE    PIC S9(4) COMP-5 VALUE 1.
       77  SQLF-INX-REC-RESTART      PIC S9(4) COMP-5 VALUE 2.
       77  SQLF-INX-REC-RESTART-NO-REDO PIC S9(4) COMP-5 VALUE 3.
       77  SQLF-INX-REC-ACCESS-NO-REDO PIC S9(4) COMP-5 VALUE 4.

      * Possible values for: SQLF_DBTN_ROLLFWD_PENDING are:
      * SQLF_DISABLE, SQLF_ENABLE, and SQLF_ROLLFWD_TABLESPACE
      * The third state of rollfwd_pending
       77  SQLF-ROLLFWD-TABLESPACE   PIC S9(4) COMP-5 VALUE 2.

      * Defines for SQLF_DBTN_LOG_RETAIN and SQLF_DBTN_LOG_RETAIN_STATUS
      * Not configured with LOGRETAIN
       77  SQLF-LOGRETAIN-DISABLE    PIC S9(4) COMP-5 VALUE 0.
      * Log file retained for recovery
       77  SQLF-LOGRETAIN-RECOVERY   PIC S9(4) COMP-5 VALUE 1.
      * Log file retained for CAPTURE, will be deleted after CAPTURE
      * finished processing
       77  SQLF-LOGRETAIN-CAPTURE    PIC S9(4) COMP-5 VALUE 2.

      *************************************************************************
      * HADR - High Availability Data Recovery parameters
      **************************************************************************
      * Internet std for hostname is 255
       77  SQLF-HADR-HOSTNAME-SZ     PIC S9(4) COMP-5 VALUE 255.
      * Internet std for svcename is 40
       77  SQLF-HADR-SVCENAME-SZ     PIC S9(4) COMP-5 VALUE 40.
      * HADR instance name
       77  SQLF-HADR-INSTANCE-SZ     PIC S9(4) COMP-5 VALUE 8.

      *************************************************************************
      * HADR_DB_ROLE values
      **************************************************************************
      * Not an HADR database
       77  SQLF-HADR-DB-ROLE-STANDARD PIC S9(4) COMP-5 VALUE 0.
      * An HADR primary db
       77  SQLF-HADR-DB-ROLE-PRIMARY PIC S9(4) COMP-5 VALUE 1.
      * An HADR standby db
       77  SQLF-HADR-DB-ROLE-STANDBY PIC S9(4) COMP-5 VALUE 2.

      *************************************************************************
      * HADR_SYNCMODE values
      **************************************************************************
      * Synchronous mode
       77  SQLF-HADR-SYNCMODE-SYNC   PIC S9(4) COMP-5 VALUE 0.
      * Near-synchronous mode
       77  SQLF-HADR-SYNCMODE-NEARSYNC PIC S9(4) COMP-5 VALUE 1.
      * Asynchronous mode
       77  SQLF-HADR-SYNCMODE-ASYNC  PIC S9(4) COMP-5 VALUE 2.

      *************************************************************************
      * DFT_MTTB_TYPES values
      **************************************************************************
      * SYSTEM
       77  SQLF-DFT-MTTB-TYPES-SYSTEM PIC S9(4) COMP-5 VALUE 1.
      * USER
       77  SQLF-DFT-MTTB-TYPES-USER  PIC S9(4) COMP-5 VALUE 2.
      * NONE
       77  SQLF-DFT-MTTB-TYPES-NONE  PIC S9(4) COMP-5 VALUE 4.
      * ALL
       77  SQLF-DFT-MTTB-TYPES-ALL   PIC S9(4) COMP-5 VALUE 8.
      * FEDERATED_TOOL
       77  SQLF-DFT-MTTB-TYPES-FED-TOOL PIC S9(4) COMP-5 VALUE 16.

      *************************************************************************
      * Constants for configuration parameter maximum lengths
      **************************************************************************
      * length of syncpoint manager name
       77  SQL-SPM-NAME-SZ           PIC S9(4) COMP-5 VALUE 8.
      * max length of log parms
       77  SQL-LOGPATH-SZ            PIC S9(4) COMP-5 VALUE 242.
      * max length of log file name
       77  SQL-LOGFILE-NAME-SZ       PIC S9(4) COMP-5 VALUE 12.
      * max length of db path parm
       77  SQL-DBPATH-SZ             PIC S9(4) COMP-5 VALUE 215.
      * size of dataflow encrypt list
       77  SQL-DFENCRYPTLIST-SZ      PIC S9(4) COMP-5 VALUE 24.
      * max length of diagpath parm
       77  SQL-FFDCPATH-SZ           PIC S9(4) COMP-5 VALUE 215.
      * max filename size (see SQLZ_FNAME_SZ)
       77  SQL-FILENAME-SZ           PIC S9(4) COMP-5 VALUE 254.

      * Constants for types of Data Redistribution operation
      * Uniform redistribution
       77  SQLUT-UNIFORM             PIC X(1) value "U".
      * Using target partitioning map
       77  SQLUT-TARGETMAP           PIC X(1) value "T".
      * Resuming a stopped data redist.
       77  SQLUT-CONTINUE            PIC X(1) value "C".
      * Rolling back a stopped data redist.
       77  SQLUT-ROLLBACK            PIC X(1) value "R".
      * LOAD and UNLOAD defines
      * Automatic Restart
       77  SQLU-AUTOMATIC-RESTART    PIC X(1) value "A".
      * Reserved for backward compability
       77  SQLU-LOAD-PHASE           PIC X(1) value "L".
      * Reserved for backward compability
       77  SQLU-BUILD-PHASE          PIC X(1) value "B".
      * Reserved for backward compability
       77  SQLU-DELETE-PHASE         PIC X(1) value "D".



      * Access Level Defines

      * No access to table during load or import
       77  SQLU-ALLOW-NO-ACCESS      PIC S9(4) COMP-5 VALUE 0.
      * Read access to table during load
       77  SQLU-ALLOW-READ-ACCESS    PIC S9(4) COMP-5 VALUE 1.
      * Write access to table during import
       77  SQLU-ALLOW-WRITE-ACCESS   PIC S9(4) COMP-5 VALUE 2.

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

       77  SQLUIMPT-IN-SIZE          PIC S9(4) COMP-5 VALUE 12.
       77  SQLUIMPT-OUT-SIZE         PIC S9(4) COMP-5 VALUE 28.
       77  SQLUEXPT-OUT-SIZE         PIC S9(4) COMP-5 VALUE 8.

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

       77  SQLULOAD-IN-SIZE          PIC S9(4) COMP-5 VALUE 40.
       77  SQLULOAD-OUT-SIZE         PIC S9(4) COMP-5 VALUE 28.
       77  SQLUUNLD-IN-SIZE          PIC S9(4) COMP-5 VALUE 24.
       77  SQLUUNLD-OUT-SIZE         PIC S9(4) COMP-5 VALUE 8.

      * Structures for Configuration Utility APIs.

       01 SQL-FUPD.
           05 SQL-TOKEN              PIC 9(4) COMP-5.
           05 FILLER                 PIC X(2).
           05 SQL-VALUE-PTR          USAGE IS POINTER.

      * Constants and structure used by sqlgadau and sqluadau.
       77  SQL-USER-AUTHORIZED       PIC S9(4) COMP-5 VALUE 1.
       77  SQL-USER-UNAUTHORIZED     PIC S9(4) COMP-5 VALUE 0.
       77  SQL-AUTHORIZATION-SIZE-OLD PIC S9(4) COMP-5 VALUE 22.
       77  SQL-AUTHORIZATION-SIZE-V2 PIC S9(4) COMP-5 VALUE 34.
       77  SQL-AUTHORIZATION-SIZE-V3 PIC S9(4) COMP-5 VALUE 38.
       77  SQL-AUTHORIZATION-SIZE-V7 PIC S9(4) COMP-5 VALUE 42.
       77  SQL-AUTHORIZATION-SIZE-V8 PIC S9(4) COMP-5 VALUE 58.
       77  SQL-AUTHORIZATION-SIZE    PIC S9(4) COMP-5 VALUE 62.

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
       77  SQLU-MAX-TBS-NAME-LEN     PIC S9(4) COMP-5 VALUE 18.
       77  SQLU-DB-DIR-LEN           PIC S9(4) COMP-5 VALUE 215.
       77  SQLU-SHR-LIB-LEN          PIC S9(4) COMP-5 VALUE 255.
       77  SQLU-MEDIA-LOCATION-LEN   PIC S9(4) COMP-5 VALUE 255.
       77  SQLU-APPLID-LEN           PIC S9(4) COMP-5 VALUE 32.
       77  SQLU-MAX-SESSIONS         PIC S9(4) COMP-5 VALUE 999.

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
       77  SQLU-LOCAL-MEDIA          PIC X(1) value "L".
      * remote file/device/named pipe
       77  SQLU-SERVER-LOCATION      PIC X(1) value "S".
      * local file/device/named pipe
       77  SQLU-CLIENT-LOCATION      PIC X(1) value "C".
      * SQL Statement
       77  SQLU-SQL-STMT             PIC X(1) value "Q".
      * Tivoli Storage Manager
       77  SQLU-TSM-MEDIA            PIC X(1) value "A".
      * X/Open XBSA interface
       77  SQLU-XBSA-MEDIA           PIC X(1) value "X".
      * vendor library
       77  SQLU-OTHER-MEDIA          PIC X(1) value "O".
      * user exit
       77  SQLU-USER-EXIT            PIC X(1) value "U".
      * Generated only by vendors
       77  SQLU-DISK-MEDIA           PIC X(1) value "D".
      * Generated only by vendors
       77  SQLU-DISKETTE-MEDIA       PIC X(1) value "K".
      * Generated internally by DB2
       77  SQLU-NULL-MEDIA           PIC X(1) value "N".
      * Generated only by vendors
       77  SQLU-TAPE-MEDIA           PIC X(1) value "T".
      * Generated only by vendors
       77  SQLU-PIPE-MEDIA           PIC X(1) value "P".

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
       77  SQLDZ-MAX-FS-NAME-LEN     PIC S9(4) COMP-5 VALUE 256.
      * JFS support for Datalinks
       77  SQL-NATIVE-DLFF           PIC S9(4) COMP-5 VALUE 0.
      * DFS support for Datalinks
       77  SQL-DFS-DLFF              PIC S9(4) COMP-5 VALUE 1.
      * Active Datalinks Manager
       77  SQL-ACTIVE-DLFM           PIC S9(4) COMP-5 VALUE 0.
      * Deleted Datalinks Manager
       77  SQL-INACTIVE-DLFM         PIC S9(4) COMP-5 VALUE 1.
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
       77  SQLDZ-SIZE                PIC S9(4) COMP-5 VALUE 1.
      * Returns sqludz_serverinfo entries for all defined File Manager
      * Servers
       77  SQLDZ-LIST                PIC S9(4) COMP-5 VALUE 2.
      * Adds File Manager Servers to datalink.cfg config file
       77  SQLDZ-ADD                 PIC S9(4) COMP-5 VALUE 3.
      * Updates File Manager Servers in datalink.cfg config file
       77  SQLDZ-UPDATE              PIC S9(4) COMP-5 VALUE 4.
      * Deletes File Manager Servers from datalink.cfg config file
       77  SQLDZ-DELETE              PIC S9(4) COMP-5 VALUE 5.
      * Definitions needed for the sqlb api calls
      * 
      * nb: 'tbs' is an abbreviaton for 'TABLESPACE'

      * length of structure version
       77  SQLB-SVERSION-SIZE        PIC S9(4) COMP-5 VALUE 8.
      * maximum length of a table space name
       77  SQLB-MAX-TBS-NAME-SZ      PIC S9(4) COMP-5 VALUE 128.
      * NB: this constant is set to the maximum size that is likely to be
      * needed in future.  Don't confuse this with SQLU_MAX_TBS_NAME_LEN
      * which is set to the current maximum size (18).

      * Externally visibile flags of a table space
      * storage manager class
      * System   Managed Space
       77  SQLB-TBS-SMS              PIC S9(4) COMP-5 VALUE 1.
      * Database Managed Space
       77  SQLB-TBS-DMS              PIC S9(4) COMP-5 VALUE 2.
      * table space contents
      * regular contents
       77  SQLB-TBS-ANY              PIC S9(4) COMP-5 VALUE 0.
      * long field data
       77  SQLB-TBS-LONG             PIC S9(4) COMP-5 VALUE 32.
      * system temp data
       77  SQLB-TBS-SYSTMP           PIC S9(4) COMP-5 VALUE 16.
      * user temp data
       77  SQLB-TBS-USRTMP           PIC S9(4) COMP-5 VALUE 64.
      * kept for compatibility
       77  SQLB-TBS-TMP              PIC S9(4) COMP-5 VALUE 16.
      * other flags values
      * temp table space is reserved for completion of reorg recovery
       77  SQLB-TBS-TMP-RECOVERY     PIC S9(9) COMP-5 VALUE 131072.

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
       77  SQLB-NORMAL               PIC S9(4) COMP-5 VALUE 0.
      * Quiesced: SHARE
       77  SQLB-QUIESCED-SHARE       PIC S9(4) COMP-5 VALUE 1.
      * Quiesced: UPDATE
       77  SQLB-QUIESCED-UPDATE      PIC S9(4) COMP-5 VALUE 2.
      * Quiesced: EXCLUSIVE
       77  SQLB-QUIESCED-EXCLUSIVE   PIC S9(4) COMP-5 VALUE 4.
      * Load pending
       77  SQLB-LOAD-PENDING         PIC S9(4) COMP-5 VALUE 8.
      * Delete pending
       77  SQLB-DELETE-PENDING       PIC S9(4) COMP-5 VALUE 16.
      * Backup pending
       77  SQLB-BACKUP-PENDING       PIC S9(4) COMP-5 VALUE 32.
      * Roll forward in progress
       77  SQLB-ROLLFORWARD-IN-PROGRESS PIC S9(4) COMP-5 VALUE 64.
      * Roll forward pending
       77  SQLB-ROLLFORWARD-PENDING  PIC S9(4) COMP-5 VALUE 128.
      * Restore pending
       77  SQLB-RESTORE-PENDING      PIC S9(4) COMP-5 VALUE 256.
      * Recovery pending (use SQLB_RESTORE_PENDING)
       77  SQLB-RECOVERY-PENDING     PIC S9(4) COMP-5 VALUE 256.
      * Disable pending
       77  SQLB-DISABLE-PENDING      PIC S9(4) COMP-5 VALUE 512.
      * Reorg in progress
       77  SQLB-REORG-IN-PROGRESS    PIC S9(4) COMP-5 VALUE 1024.
      * Backup in progress
       77  SQLB-BACKUP-IN-PROGRESS   PIC S9(4) COMP-5 VALUE 2048.
      * storage must be defined
       77  SQLB-STORDEF-PENDING      PIC S9(4) COMP-5 VALUE 4096.
      * Restore in progress
       77  SQLB-RESTORE-IN-PROGRESS  PIC S9(4) COMP-5 VALUE 8192.
      * Offline and not accessible
       77  SQLB-OFFLINE              PIC S9(9) COMP-5 VALUE 16384.
      * Only action is to drop
       77  SQLB-DROP-PENDING         PIC S9(9) COMP-5 VALUE 32768.
      * No write is allowed
       77  SQLB-SUSPEND-WRITE        PIC S9(9) COMP-5 VALUE 65536.
      * Load in progress
       77  SQLB-LOAD-IN-PROGRESS     PIC S9(9) COMP-5 VALUE 131072.
      * storage may be defined
       77  SQLB-STORDEF-ALLOWED      PIC S9(9) COMP-5 VALUE 33554432.
      * storDef is in 'final' state
       77  SQLB-STORDEF-FINAL-VERSION PIC S9(9) COMP-5 VALUE 67108864.
      * storDef was changed prior to rollforward
       77  SQLB-STORDEF-CHANGED      PIC S9(9) COMP-5 VALUE 134217728.
      * dms rebalancer is active
       77  SQLB-REBAL-IN-PROGRESS    PIC S9(9) COMP-5 VALUE 268435456.
      * TBS deletion in progress
       77  SQLB-PSTAT-DELETION       PIC S9(9) COMP-5 VALUE 536870912.
      * TBS creation in progress
      * Value of SQLB-PSTAT-CREATION is 1073741824
       01  SQLB-PSTAT-CREATION-INIT PIC X(4) VALUE x"40000000".
       01  SQLB-PSTAT-CREATION REDEFINES
           SQLB-PSTAT-CREATION-INIT PIC S9(9) COMP-5.

      * For service use only.
       77  SQLB-STATE-SET            PIC S9(4) COMP-5 VALUE 8.

      * provide information about the quiescers of a tablespace

      * Maximum number of quiescers
       77  SQLB-MAX-QUIESCERS        PIC S9(4) COMP-5 VALUE 5.
      * SQLB_TBSPQRY_DATA version id
       77  SQLB-TBSPQRY-DATA-ID      PIC X(8) value "TBSQ0030".

      * Provide info about a tableSpace in a database

       77  SQLB-TBSPQRY-DATA-SIZE    PIC S9(4) COMP-5 VALUE 300.
      * container types
      * SMS: directory path specs
       77  SQLB-CONT-PATH            PIC S9(4) COMP-5 VALUE 0.
      * DMS: a 'raw' or 'device' spec, tag size = one page
       77  SQLB-CONT-DISK-PAGE-TAG   PIC S9(4) COMP-5 VALUE 1.
      * DMS: a 'cooked file' -> file spec, tag size = one page
       77  SQLB-CONT-FILE-PAGE-TAG   PIC S9(4) COMP-5 VALUE 2.
      * DMS: a 'raw' or 'device' spec, tag size = one extent
       77  SQLB-CONT-DISK-EXTENT-TAG PIC S9(4) COMP-5 VALUE 5.
      * DMS: a 'cooked file' -> file spec, tag size = one extent
       77  SQLB-CONT-FILE-EXTENT-TAG PIC S9(4) COMP-5 VALUE 6.

      * pre-version 8 container types - kept for compatibility
       77  SQLB-CONT-DISK            PIC S9(4) COMP-5 VALUE 1.
       77  SQLB-CONT-FILE            PIC S9(4) COMP-5 VALUE 2.
       77  SQLB-CONT-STRIPED-DISK    PIC S9(4) COMP-5 VALUE 5.
       77  SQLB-CONT-STRIPED-FILE    PIC S9(4) COMP-5 VALUE 6.

      * provide information about the containers on which tablespaces are
      * based

       77  SQLB-TBSCONTQRY-DATA-SIZE PIC S9(4) COMP-5 VALUE 292.
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
       77  SQLB-OPEN-TBS-ALL         PIC S9(4) COMP-5 VALUE 0.
      * all tablespaces my agent is restoring
       77  SQLB-OPEN-TBS-RESTORE     PIC S9(4) COMP-5 VALUE 1.
      * all tablespaces where storage definition is allowed
       77  SQLB-OPEN-TBS-STORDEF     PIC S9(4) COMP-5 VALUE 2.
      * reserved
       77  SQLB-OPEN-TBS-RESERVED    PIC S9(4) COMP-5 VALUE 4.

      * constant used in place of tablespace id to indicate all table spaces
      * in db
       77  SQLB-ALL-TABLESPACES      PIC S9(9) COMP-5 VALUE 65534.

      * constants used for reserved parameters
       77  SQLB-RESERVED1            PIC S9(9) COMP-5 VALUE 65533.
       77  SQLB-RESERVED2            PIC S9(9) COMP-5 VALUE 65534.


       77  SQLB-SET-CONT-INIT-STATE  PIC S9(4) COMP-5 VALUE 0.
       77  SQLB-SET-CONT-FINAL-STATE PIC S9(4) COMP-5 VALUE 1.





      * SQL Return Codes in SQLCODE for UTILITY/CONFIGURATION Commands

      * If using the symbolic constants SQL-RC-OK and SQL-RC-INVALID-SQLCA
      * ensure sql.cbl is included.

      * ROLL FORWARD Return Codes - more in the 4900's

      * DB not enabled for Roll Fwd
       77  SQLU-RC-RFNOTEN           PIC S9(4) COMP-5 VALUE -1260.
      * Roll Forward is not Pending
       77  SQLU-RC-RFNOTP            PIC S9(4) COMP-5 VALUE -1261.
      * Bad Point in Time specified
       77  SQLU-RC-BADPIT            PIC S9(4) COMP-5 VALUE -1262.
      * Invalid Log Extent file
       77  SQLU-RC-INVEXT            PIC S9(4) COMP-5 VALUE -1263.
      * Log Extent file does not belong to DB
       77  SQLU-RC-NOTEXT            PIC S9(4) COMP-5 VALUE -1264.
      * Log extent file is incorrect version
       77  SQLU-RC-IVREXT            PIC S9(4) COMP-5 VALUE -1265.
      * Time specified is before previous roll-forward time
       77  SQLU-RC-PRTIME            PIC S9(4) COMP-5 VALUE -1266.
      * User exit encountered an error (other than retry) while attempting
      * to retrieve a log extent file for roll forward
       77  SQLU-RC-UEXIT-ERR         PIC S9(4) COMP-5 VALUE -1268.
      * Rollforward by tablespace in progress.
       77  SQLU-RC-RFINPRG           PIC S9(4) COMP-5 VALUE -1269.
      * Roll forward complete but I/O errors encountered
       77  SQLU-RC-RCVIOERR          PIC S9(4) COMP-5 VALUE 1271.
      * Rollforward by tablespace has stopped.
       77  SQLU-RC-RFSTOP            PIC S9(4) COMP-5 VALUE -1272.
      * Missing log extent file.
       77  SQLU-RC-MISSING-EXT       PIC S9(4) COMP-5 VALUE -1273.
      * Invalid stop time for tablespace rollforward.
       77  SQLU-RC-INVTSP-STOPTIME   PIC S9(4) COMP-5 VALUE -1274.
      * invalid stop time specified
       77  SQLU-INVRFR-STOPTIME      PIC S9(4) COMP-5 VALUE -1275.
      * invalid time to issue a stop
       77  SQLU-INVRFR-STOP          PIC S9(4) COMP-5 VALUE -1276.
      * Restore found one or more containers are inaccessable
       77  SQLUD-INACCESSABLE-CONTAINER PIC S9(4) COMP-5 VALUE 1277.
      * Roll forward complete but rollback required
       77  SQLU-RC-TBSROLLBK         PIC S9(4) COMP-5 VALUE 1278.
      * Restart complete but not all invalid  indexes were recreated
       77  SQL-RC-RECREATE-INDEXES   PIC S9(4) COMP-5 VALUE 1279.
      * invalid stop time specified for tablespace rollforward
       77  SQLU-INVRFR-TBSPITTIME    PIC S9(4) COMP-5 VALUE -1280.

      * invalid client comm. protocols
       77  SQLF-RC-INV-CLIENT-COMM   PIC S9(4) COMP-5 VALUE -1290.
      * invalid directory services field
       77  SQLF-RC-INV-DIR-FIELD     PIC S9(4) COMP-5 VALUE -1296.

      * hostname in node dir unknown
       77  SQLCC-RC-UNKNOWN-HOST     PIC S9(4) COMP-5 VALUE -1336.
      * service name in node directory is unknown
       77  SQLCC-RC-UNKNOWN-SERVICE  PIC S9(4) COMP-5 VALUE -1337.
      * CPI-C symdestname is unknown
       77  SQLCC-RC-UNKNOWN-SYM-DEST-NAME PIC S9(4) COMP-5 VALUE -1338.
      * wrkstn name (nname)
       77  SQLCC-RC-NNAME-NOTFOUND   PIC S9(4) COMP-5 VALUE -1341.
      * SOCKS env vars not found
       77  SQLCC-RC-NO-SOCKS-ENV-VAR PIC S9(4) COMP-5 VALUE -1460.
      * socket address in use
       77  SQLCC-RC-SOCKADDR-IN-USE  PIC S9(4) COMP-5 VALUE -5040.
      * general failure in servercommunications support
       77  SQLCC-RC-COMM-SUPPORT-FAILED PIC S9(4) COMP-5 VALUE -5042.

      * invalid discover type
       77  SQLF-RC-INV-DISCOVER-TYPE PIC S9(4) COMP-5 VALUE -1480.
      * invalid discover comm
       77  SQLF-RC-INV-DISCOVER-COMM PIC S9(4) COMP-5 VALUE -1481.

      * buffpage may be ignored
       77  SQLF-WARNING-BUFFPAGE     PIC S9(4) COMP-5 VALUE 1482.

      * GENERAL UTILITY Return Codes

      * Server does not support functionality
       77  SQLU-FUNCTION-NOT-SUPPORTED PIC S9(4) COMP-5 VALUE -1651.
      * Group not defined on DLM
       77  SQLU-RECONCILE-GROUP-UNDEFINED PIC S9(4) COMP-5 VALUE 1161.
      * DLM down during exception
       77  SQLU-RECONCILE-EXCP-DLM-DOWN PIC S9(4) COMP-5 VALUE 1162.
      * DLM down during backup
       77  SQLU-BACKUP-DLM-DOWN      PIC S9(4) COMP-5 VALUE 1196.

      * Invalid output drive
       77  SQLU-BAD-DRIVE            PIC S9(4) COMP-5 VALUE -2000.
      * Backup terminated by user
       77  SQLU-USER-TERM            PIC S9(4) COMP-5 VALUE -2001.
      * database not local
       77  SQLU-NOT-LOCAL            PIC S9(4) COMP-5 VALUE -2002.
      * base op system error
       77  SQLU-DOS-ERROR            PIC S9(4) COMP-5 VALUE -2003.
      * SQL error occurred
       77  SQLU-SQL-ERROR            PIC S9(4) COMP-5 VALUE -2004.
      * read wrong # of bytes
       77  SQLU-READ-ERROR           PIC S9(4) COMP-5 VALUE -2005.
      * too few buffers for pagesize
       77  SQLU-BUFF-TOO-SMALL       PIC S9(4) COMP-5 VALUE -2007.
      * call out of sequence
       77  SQLU-INVALID-ACTION       PIC S9(4) COMP-5 VALUE -2008.
      * insufficient memory
       77  SQLU-INSUFF-MEMORY        PIC S9(4) COMP-5 VALUE -2009.
      * error in Start Using
       77  SQLU-STRD-ERROR           PIC S9(4) COMP-5 VALUE -2010.
      * error in Stop Using
       77  SQLU-STPD-ERROR           PIC S9(4) COMP-5 VALUE -2011.
      * directory Scan error
       77  SQLU-DIR-ERROR            PIC S9(4) COMP-5 VALUE -2013.
      * invalid database name
       77  SQLU-INVALID-DBNAME       PIC S9(4) COMP-5 VALUE -2015.
      * invalid path in environment
       77  SQLU-INVALID-PATH         PIC S9(4) COMP-5 VALUE -2016.
      * Start Session failed
       77  SQLU-START-SESSION        PIC S9(4) COMP-5 VALUE -2017.
      * invalid authorizations
       77  SQLU-INVALID-AUTHS        PIC S9(4) COMP-5 VALUE -2018.
      * auto-binding failed
       77  SQLU-AUTOBIND             PIC S9(4) COMP-5 VALUE -2019.
      * conflict after auto-bind control file
       77  SQLU-TIMESTAMP            PIC S9(4) COMP-5 VALUE -2020.
      * I/O error in accessing the log
       77  SQLU-IO-ERROR-LFH         PIC S9(4) COMP-5 VALUE -2023.
      * I/O error in accessing the Backup/Restore flag file
       77  SQLU-IO-ERROR-BRG         PIC S9(4) COMP-5 VALUE -2024.
      * System I/O error occurred
       77  SQLU-IO-ERROR             PIC S9(4) COMP-5 VALUE -2025.
      * error in PAUSE the started DB
       77  SQLU-PAUSE-ERROR          PIC S9(4) COMP-5 VALUE -2026.
      * error in CONTINUE the stoped DB
       77  SQLU-CONT-ERROR           PIC S9(4) COMP-5 VALUE -2027.
      * interruption installing error
       77  SQLU-INT-INST-ERR         PIC S9(4) COMP-5 VALUE -2028.
      * user exit returned non-zero rc
       77  SQLU-UEXIT-RC             PIC S9(4) COMP-5 VALUE -2029.

      * warning to mount tape
       77  SQLU-FIRST-TAPE-WARNING   PIC S9(4) COMP-5 VALUE 2031.
      * parameter to utility incorrect
       77  SQLU-INVALID-PARM         PIC S9(4) COMP-5 VALUE -2032.
      * TSM reported error
       77  SQLU-TSM-ERROR            PIC S9(4) COMP-5 VALUE -2033.
      * address of parameter incorrect
       77  SQLU-INVALID-PARM-ADDRESS PIC S9(4) COMP-5 VALUE -2034.
      * error during nointerrupt action
       77  SQLU-NOINT-ERROR          PIC S9(4) COMP-5 VALUE -2035.
      * directory does not exist
       77  SQLU-PATH-ERROR           PIC S9(4) COMP-5 VALUE -2036.
      * unable to load TSM
       77  SQLU-LOAD-TSM-ERROR       PIC S9(4) COMP-5 VALUE -2037.
      * database system error
       77  SQLU-DBSYSTEM-ERROR       PIC S9(4) COMP-5 VALUE -2038.
      * application terminated
       77  SQLU-NO-APP-ERROR         PIC S9(4) COMP-5 VALUE -2039.
      * alias parameter error
       77  SQLU-ALIAS-ERROR          PIC S9(4) COMP-5 VALUE -2040.
      * buff_size parameter error
       77  SQLU-BUFFSIZE-ERROR       PIC S9(4) COMP-5 VALUE -2041.
      * I/O error during change tape
       77  SQLU-IO-WARNING           PIC S9(4) COMP-5 VALUE 2042.
      * Spawn child process error
       77  SQLU-SPAWN-EDU-ERROR      PIC S9(4) COMP-5 VALUE -2043.
      * Message queue error
       77  SQLU-QUEUE-ERROR          PIC S9(4) COMP-5 VALUE -2044.
      * Object access error
       77  SQLU-OBJECT-ACCESS-ERROR  PIC S9(4) COMP-5 VALUE -2048.
      * Bad backup image
       77  SQLU-CORRUPT-IMAGE-ERROR  PIC S9(4) COMP-5 VALUE -2054.
      * Unable to access memory set
       77  SQLU-MEMORY-ACCESS-ERROR  PIC S9(4) COMP-5 VALUE -2055.
      * device path point to unknown device type
       77  SQLU-UNKNOWN-MEDIA-TYPE   PIC S9(4) COMP-5 VALUE -2056.
      * device or file already opened by other process
       77  SQLU-MEDIA-CANNOT-BE-SHARED PIC S9(4) COMP-5 VALUE -2057.
      * End of tape or file encountered during read
       77  SQLU-END-OF-MEDIA-WARNING PIC S9(4) COMP-5 VALUE 2058.
      * Device is full during write
       77  SQLU-DEVICE-FULL-WARNING  PIC S9(4) COMP-5 VALUE 2059.
      * Empty device or file not found during read
       77  SQLU-MEDIA-EMPTY-WARNING  PIC S9(4) COMP-5 VALUE 2060.
      * Access denied due to authority level
       77  SQLU-MEDIA-ACCESS-DENIED  PIC S9(4) COMP-5 VALUE -2061.
      * Access error
       77  SQLU-MEDIA-ACCESS-ERROR   PIC S9(4) COMP-5 VALUE -2062.
      * Terminate last Media IO.
       77  SQLU-TERM-LAST-MEDIA-WARNING PIC S9(4) COMP-5 VALUE 2065.
      * Invalid tablespace name
       77  SQLU-BAD-TABLESPACE-NAME  PIC S9(4) COMP-5 VALUE -2066.
      * Could not locate media header in backup or copy image
       77  SQLU-NO-MEDIA-HEADER      PIC S9(4) COMP-5 VALUE -2068.
      * Mismatch alias name from media
       77  SQLU-INCORRECT-DBALIAS    PIC S9(4) COMP-5 VALUE -2069.
      * Mismatch timestamp read from media
       77  SQLU-INCORRECT-TIMESTAMP  PIC S9(4) COMP-5 VALUE -2070.

      * Shared library access error
       77  SQLU-SHR-LIB-ACCESS-ERROR PIC S9(4) COMP-5 VALUE -2071.
      * Bind shared library error
       77  SQLU-BIND-SHR-LIB-ERROR   PIC S9(4) COMP-5 VALUE -2072.

      * Error at DB2 or DLFM
       77  SQLU-DATALINK-INTERNAL-ERROR PIC S9(4) COMP-5 VALUE -2073.
      * Error at DB2 end
       77  SQLU-DATALINK-DB-ERROR    PIC S9(4) COMP-5 VALUE -2074.
      * Error at DLFM end
       77  SQLU-DATALINK-DLFM-ERROR  PIC S9(4) COMP-5 VALUE -2075.
      * Unable to register DLM Server
       77  SQLU-REGISTER-DLFM-WARNING PIC S9(4) COMP-5 VALUE 2076.
      * DLMs down during reconcile
       77  SQLU-RECONCILE-DLM-PEND-WARN PIC S9(4) COMP-5 VALUE 2077.
      * Shared library vendor API error
       77  SQLU-SHR-LIB-VENDOR-API-ERROR PIC S9(4) COMP-5 VALUE -2079.
      * Out of sequence tablespace restore
       77  SQLUD-TSP-RESTORE-OUT-OF-SEQ PIC S9(4) COMP-5 VALUE -2154.

      * ARCHIVE LOG Return Codes
      * Archive log error.
       77  SQLU-ARCHIVELOG-ERROR     PIC S9(4) COMP-5 VALUE -1259.
      * Database is not in recoverable mode.
       77  SQLU-ARCHIVELOG-NONRECOV-DB PIC S9(4) COMP-5 VALUE -2417.

      * HISTORY TABLE Return Codes
      * Changes were made to historyfile during update.
       77  SQLUH-SCAN-UPDATED-WARNING PIC S9(4) COMP-5 VALUE 2155.
      * MAX # open scans exceeded
       77  SQLUH-MAX-SCANS-EXCEEDED  PIC S9(4) COMP-5 VALUE -2157.
      * History file is unfixable
       77  SQLUH-DAMAGED-FILE        PIC S9(4) COMP-5 VALUE -2161.
      * A log file does not have read/write permission
       77  SQLU-INV-PERM-LOGFILE     PIC S9(4) COMP-5 VALUE -2162.
      * No suitable backup image could be found for use in the recover
      * operation
       77  SQLU-RECOVER-NO-IMAGE-FOUND PIC S9(4) COMP-5 VALUE -2163.
      * RECOVER could not locate a history file
       77  SQLU-RECOVER-FILE-NOT-FOUND PIC S9(4) COMP-5 VALUE -2164.
      * number of tablespaces changed
       77  SQLUH-SQLUHINFO-VARS-WARNING PIC S9(4) COMP-5 VALUE 2165.
      * RECOVER failed because database does not exist
       77  SQLU-RECOVER-DB-NOT-FOUND PIC S9(4) COMP-5 VALUE -2166.
      * Duplicate timestamp found
       77  SQLUH-DUPLICATE-ENTRY     PIC S9(4) COMP-5 VALUE -2170.
      * Entry not found on update
       77  SQLUH-ENTRY-NOT-FOUND     PIC S9(4) COMP-5 VALUE -2171.
      * Access history file warning
       77  SQLU-ACCESS-HIST-WARNING  PIC S9(4) COMP-5 VALUE 2172.

      * PRUNE command return codes
      * PRUNE LOGFILE not allowed in this database configuration
       77  SQLU-PRUNE-LOG-NOT-ALLOWED PIC S9(4) COMP-5 VALUE -1206.

      * REORGANIZE TABLE Return Codes
      * invalid authid on index name
       77  SQLUR-INVALID-AUTHID      PIC S9(4) COMP-5 VALUE -2200.
      * invalid tablename syntax
       77  SQLUR-INVALID-TABLENAME   PIC S9(4) COMP-5 VALUE -2203.
      * invalid tablename syntax
       77  SQLUR-INVALID-INDEXNAME   PIC S9(4) COMP-5 VALUE -2204.
      * index doesn't exist
       77  SQLUR-INDEX-NOT-FOUND     PIC S9(4) COMP-5 VALUE -2205.
      * invalid filepath pointer
       77  SQLUR-INVALID-FILEPATH    PIC S9(4) COMP-5 VALUE -2207.
      * invalid tablespace pointer
       77  SQLUR-INVALID-TABLESPACE  PIC S9(4) COMP-5 VALUE -2208.
      * table does not exist
       77  SQLUR-TABLE-NOT-FOUND     PIC S9(4) COMP-5 VALUE -2211.
      * cannot reorg a view
       77  SQLUR-VIEW-ERROR          PIC S9(4) COMP-5 VALUE -2212.
      * Incorrect tablespace type
       77  SQLUR-INCORRECT-TABLESPACE PIC S9(4) COMP-5 VALUE -2213.
      * insuffic authority
       77  SQLUR-INSAUTH             PIC S9(4) COMP-5 VALUE -2214.
      * SQL error commiting prev work
       77  SQLUR-SQLERR-COMPREV      PIC S9(4) COMP-5 VALUE -2215.
      * SQL error during reorganization
       77  SQLUR-SQLERR-REORG        PIC S9(4) COMP-5 VALUE -2216.
      * Invalid temp tablespace for reorg table
       77  SQLUR-INV-TEMP            PIC S9(4) COMP-5 VALUE -2217.
      * Incompatible options specified for reorg table
       77  SQLUR-INCOMPAT-OPTS       PIC S9(4) COMP-5 VALUE -2218.
      * Invalid action for reorg table INPLACE
       77  SQLUR-INV-ACTION          PIC S9(4) COMP-5 VALUE -2219.

      * RUN STATISTICS Return Codes
      * invalid authid
       77  SQLUS-INVALID-AUTHID      PIC S9(4) COMP-5 VALUE -2300.
      * invalid table name
       77  SQLUS-INVALID-TABLE-NAME  PIC S9(4) COMP-5 VALUE -2301.
      * invalid index pointer
       77  SQLUS-INVALID-INDEX-LIST  PIC S9(4) COMP-5 VALUE -2302.
      * statsopt parameter invalid
       77  SQLUS-INVALID-STATS-OPT   PIC S9(4) COMP-5 VALUE -2303.
      * sharelev parameter invalid
       77  SQLUS-INVALID-SHARE-LEV   PIC S9(4) COMP-5 VALUE -2304.
      * table specified is a view
       77  SQLUS-VIEWS-NOT-ALLOWED   PIC S9(4) COMP-5 VALUE -2305.
      * object doesn't exist
       77  SQLUS-OBJ-DOES-NOT-EXIST  PIC S9(4) COMP-5 VALUE -2306.
      * system table not allowed
       77  SQLUS-SYS-TABLE-NOT-ALLOWED PIC S9(4) COMP-5 VALUE -2307.
      * index authid invalid
       77  SQLUS-INVALID-INDEX-AUTHID PIC S9(4) COMP-5 VALUE -2308.
      * index name invalid
       77  SQLUS-INVALID-INDEX-NAME  PIC S9(4) COMP-5 VALUE -2309.
      * error running statistics
       77  SQLUS-ERROR-STAT          PIC S9(4) COMP-5 VALUE -2310.
      * insuffic authority for runstats
       77  SQLUS-INSAUTH             PIC S9(4) COMP-5 VALUE -2311.
      * statistics heap is too small
       77  SQLUS-STATS-HEAP-TOO-SMALL PIC S9(4) COMP-5 VALUE -2312.
      * incomplete statistics collected
       77  SQLUS-PARTIAL-SUCCESS     PIC S9(4) COMP-5 VALUE 2313.
      * Statistics are in an inconsistent state
       77  SQLUS-INCONSISTENT-STATS  PIC S9(4) COMP-5 VALUE 2314.

      * Rst DB turn off roll fwd pend, old OS/2 API only
       77  SQLUD-RST-NOROLLFWD       PIC S9(4) COMP-5 VALUE 3.
      * Old OS/2 Backup calling action values
      * backup entire database
       77  SQLU-BACK-ALL             PIC S9(4) COMP-5 VALUE 0.
      * backup changes only
       77  SQLU-BACK-CHANGES         PIC S9(4) COMP-5 VALUE 1.
      * quiesce during backup
       77  SQLU-BACK-QUIESCE         PIC S9(4) COMP-5 VALUE 2048.
      * New UOW return error immed
       77  SQLU-NEW-UOW-RETURN-ERROR PIC S9(4) COMP-5 VALUE 1024.

      * pause or Continue or migration error
       77  SQLU-DBM-ERROR            PIC S9(4) COMP-5 VALUE -2014.
      * Wrong diskette inserted
       77  SQLU-WRONG-DISKETTE       PIC S9(4) COMP-5 VALUE -2021.
      * a specific fixed disk is full
       77  SQLU-DISK-FULL            PIC S9(4) COMP-5 VALUE -2030.

      * invalid type
       77  SQLUB-BAD-TYPE            PIC S9(4) COMP-5 VALUE -2400.
      * type must be database
       77  SQLUB-TYPE-MUSTBE-DB      PIC S9(4) COMP-5 VALUE -2401.
      * insufficient auth for backup
       77  SQLUB-USER-NOT-AUTH       PIC S9(4) COMP-5 VALUE -2403.
      * The target disk/diskette is full
       77  SQLUB-MEDIA-FULL          PIC S9(4) COMP-5 VALUE -2404.
      * The Backup can't run because B/R flag file existed and read I/O
      * error occurred
       77  SQLUB-BR-INPROG           PIC S9(4) COMP-5 VALUE -2407.
      * Backup process is successful, but unable to delete BRG file
       77  SQLUB-DEL-BRG-ERR         PIC S9(4) COMP-5 VALUE 2408.
      * 'change-only' is only applied to the most recent base image
       77  SQLUB-WRONG-BASE-IMAGE    PIC S9(4) COMP-5 VALUE -2409.
      * change only, backup not allowed to user exit
       77  SQLUB-NO-CHG-NSD          PIC S9(4) COMP-5 VALUE -2410.
      * I/O error during backup
       77  SQLUB-IO-ERROR            PIC S9(4) COMP-5 VALUE -2411.
      * Log not truncated during backup
       77  SQLUB-LOG-NOT-TRUNC       PIC S9(4) COMP-5 VALUE 2425.
      * Restore utility not bound
       77  SQLUD-BIND-WARNING        PIC S9(4) COMP-5 VALUE 2507.
      * error dropping database
       77  SQLUD-DROP-ERROR          PIC S9(4) COMP-5 VALUE -2511.

      * BACKUP Return Codes
      * The backup can't run because the Restore is in progress
       77  SQLUB-RST-IN-PROG         PIC S9(4) COMP-5 VALUE -2405.
      * The Backup can't run because roll forward is pending
       77  SQLUB-ROLLFWD-PENDING     PIC S9(4) COMP-5 VALUE -2406.
      * data page encountered during backup is corrupted
       77  SQLUB-CORRUPT-PAGE        PIC S9(4) COMP-5 VALUE -2412.
      * retain req'd for online backup
       77  SQLUB-LOGRETAIN-ONLINE-ERROR PIC S9(4) COMP-5 VALUE -2413.
      * no space for control info
       77  SQLUB-INSUF-SPACE-WARNING PIC S9(4) COMP-5 VALUE 2414.
      * tape full, mount another
       77  SQLUB-NEXT-TAPE-WARNING   PIC S9(4) COMP-5 VALUE 2416.
      * database does not exist
       77  SQLUB-DBASE-DOES-NOT-EXIST PIC S9(4) COMP-5 VALUE -2418.
      * disk full during backup
       77  SQLUB-DISK-FULL-ERROR     PIC S9(4) COMP-5 VALUE -2419.

      * first tape cannot hold header
       77  SQLUB-FIRST-TAPE-ERROR    PIC S9(4) COMP-5 VALUE -2420.
      * retain req'd for tbs backup
       77  SQLUB-LOGRETAIN-TBS-ERROR PIC S9(4) COMP-5 VALUE -2421.
      * the backup API used does not support this format of database
       77  SQLUB-DB-UNSUPPORTED-BY-API PIC S9(4) COMP-5 VALUE -2422.
      * A required index is missing during an offline backup
       77  SQLUB-MISSING-INDEX       PIC S9(4) COMP-5 VALUE -2423.
      * Copy operation at DLFMend is still in progress
       77  SQLUB-COPY-IN-PROGRESS    PIC S9(4) COMP-5 VALUE -2424.
      * TRACKMOD req'd for incremental backup
       77  SQLUB-TRACKMOD-INCR-ERROR PIC S9(4) COMP-5 VALUE -2426.
      * Saved library is missing an entry point
       77  SQLUB-MISSING-ENTRYPOINT  PIC S9(4) COMP-5 VALUE -2427.
      * Failed to backup requested logfile
       77  SQLUB-BACKUP-LOGS-ERROR   PIC S9(4) COMP-5 VALUE -2428.

      * RESTORE Return Codes
      * can't read restored database
       77  SQLUD-CANNOT-RESTORE      PIC S9(4) COMP-5 VALUE -2501.
      * error reading backup diskette
       77  SQLUD-DISKETTE-ERROR      PIC S9(4) COMP-5 VALUE -2502.
      * wrong backup diskette
       77  SQLUD-WRONG-DATABASE      PIC S9(4) COMP-5 VALUE -2503.
      * prompt for backup diskette
       77  SQLUD-DISKETTE-PROMPT     PIC S9(4) COMP-5 VALUE 2504.
      * warn that drop will be done
       77  SQLUD-DROP-WARNING        PIC S9(4) COMP-5 VALUE 2505.
      * Restore worked, but not cleanup
       77  SQLUD-DATABASE-WARNING    PIC S9(4) COMP-5 VALUE 2506.
      * timestamp incorrectly specified
       77  SQLUD-INVALID-TIMESTAMP   PIC S9(4) COMP-5 VALUE -2508.
      * invalid database drive
       77  SQLUD-INVALID-DBDRV       PIC S9(4) COMP-5 VALUE -2509.
      * semaphore error
       77  SQLUD-SEM-ERROR           PIC S9(4) COMP-5 VALUE -2510.
      * error creating database
       77  SQLUD-CREATE-ERROR        PIC S9(4) COMP-5 VALUE -2512.
      * error renaming database
       77  SQLUD-REN-ERROR           PIC S9(4) COMP-5 VALUE -2513.
      * restored database wrong version
       77  SQLUD-BAD-VERSION         PIC S9(4) COMP-5 VALUE -2514.
      * insuffic authority to restore
       77  SQLUD-INSAUTH             PIC S9(4) COMP-5 VALUE -2515.
      * a database active
       77  SQLUD-DBACT               PIC S9(4) COMP-5 VALUE -2516.
      * database migrated
       77  SQLUD-MIGRATED            PIC S9(4) COMP-5 VALUE 2517.
      * error in restoring DB config.
       77  SQLUD-RST-DBCONG-ERR      PIC S9(4) COMP-5 VALUE -2518.
      * error in migrating the database
       77  SQLUD-MIGRATE-ERROR       PIC S9(4) COMP-5 VALUE -2519.
      * DBCON file is restored using the backup version
       77  SQLUD-DBCON-WARN          PIC S9(4) COMP-5 VALUE 2520.
      * Restore process is successful, but unable to delete BRG file
       77  SQLUD-DEL-BRG-ERR         PIC S9(4) COMP-5 VALUE 2521.
      * more than one file match
       77  SQLUD-TOO-MANY-BACKUP-FILES PIC S9(4) COMP-5 VALUE -2522.
      * corrupted backup image
       77  SQLUD-MEDIA-CORRUPT       PIC S9(4) COMP-5 VALUE -2530.
      * image of wrong database
       77  SQLUD-WRGIMAGE-ERROR      PIC S9(4) COMP-5 VALUE -2532.
      * image of wrong database
       77  SQLUD-WRGIMAGE-WARNING    PIC S9(4) COMP-5 VALUE 2533.
      * seq number of backup incorrect
       77  SQLUD-WRONGSEQ-WARNING    PIC S9(4) COMP-5 VALUE 2536.
      * roll forward required
       77  SQLUD-MUST-ROLLFWD        PIC S9(4) COMP-5 VALUE -2537.
      * end of file reached unexpectedly
       77  SQLUD-UNEXPECT-EOF-ERROR  PIC S9(4) COMP-5 VALUE -2538.
      * noint type restore had warning
       77  SQLUD-NOINT-WARNING       PIC S9(4) COMP-5 VALUE 2540.
      * unable to close backup file
       77  SQLUD-CLOSE-MEDIA-WARNING PIC S9(4) COMP-5 VALUE 2541.
      * no backup file match found
       77  SQLUD-NO-BACKUP-FILE-MATCH PIC S9(4) COMP-5 VALUE -2542.
      * invalid directory for new dbase
       77  SQLUD-DB-DIR-ERROR        PIC S9(4) COMP-5 VALUE -2543.
      * disk full during restore
       77  SQLUD-DISK-FULL-ERROR     PIC S9(4) COMP-5 VALUE -2544.
      * restore requires first image first
       77  SQLUD-NOT-FIRST-IMAGE     PIC S9(4) COMP-5 VALUE -2546.
      * cannot restore online backup from a previous release
       77  SQLUD-OLD-ONLINE-IMAGE-ERROR PIC S9(4) COMP-5 VALUE -2547.
      * backup has diff codepage from disk DB
       77  SQLUD-IMAGE-DB-CP-MISMATCH PIC S9(4) COMP-5 VALUE -2548.
      * all tablespaces skipped during restore
       77  SQLUD-ALL-TBLSPACES-SKIPPED PIC S9(4) COMP-5 VALUE -2549.
      * backup from a different node
       77  SQLUD-WRONG-NODE          PIC S9(4) COMP-5 VALUE -2550.
      * backup of a database with a different catalog node
       77  SQLUD-WRONG-CAT-NODE      PIC S9(4) COMP-5 VALUE -2551.
      * Report filename length >255
       77  SQLUD-REPORTFILE-ERROR    PIC S9(4) COMP-5 VALUE -2552.
      * Reconcile Genereic Error
       77  SQLU-RECONCILE-GENERIC-ERROR PIC S9(4) COMP-5 VALUE -2554.

      *************************************************************************
      *  the meaning of the following 8 warnings :
      *        A -> database alias
      *        N -> database name
      *        S -> database seed
      *        0 -> target db value DOES NOT matches backup image value
      *        1 -> target db value matches backup image value
      * 
      **************************************************************************
       77  SQLUD-A0N0S0-WARNING      PIC S9(4) COMP-5 VALUE 2529.
       77  SQLUD-A0N0S1-WARNING      PIC S9(4) COMP-5 VALUE 2528.
       77  SQLUD-A0N1S0-WARNING      PIC S9(4) COMP-5 VALUE 2525.
       77  SQLUD-A0N1S1-WARNING      PIC S9(4) COMP-5 VALUE 2524.
       77  SQLUD-A1N0S0-WARNING      PIC S9(4) COMP-5 VALUE 2527.
       77  SQLUD-A1N0S1-WARNING      PIC S9(4) COMP-5 VALUE 2526.
       77  SQLUD-A1N1S0-WARNING      PIC S9(4) COMP-5 VALUE 2523.
       77  SQLUD-A1N1S1-WARNING      PIC S9(4) COMP-5 VALUE 2539.
      * restore tablespace to a non original database.
       77  SQLUD-TBLSP-TO-OTHER-DB   PIC S9(4) COMP-5 VALUE -2560.
      * restore tablespace to a new data base.
       77  SQLUD-TBLSP-TO-NEW-DB     PIC S9(4) COMP-5 VALUE 2561.
      * restore tablespace from full backup image.
       77  SQLUD-TBLSP-FR-FULL-BKUP  PIC S9(4) COMP-5 VALUE -2562.
      * Not all tablespases were restored
       77  SQLUD-NOTALL-TBS-RESTORED PIC S9(4) COMP-5 VALUE 2563.
      * Mismatch between restore image and database restored to
       77  SQLUD-DB-MISMATCH         PIC S9(4) COMP-5 VALUE 2565.
      * Tables in tablespaces are in DRP/RNP state
       77  SQLUD-TBS-DATALINK-PENDING PIC S9(4) COMP-5 VALUE 2566.
      * Image is being restored on the wrong platform
       77  SQLUD-WRONG-PLATFORM      PIC S9(4) COMP-5 VALUE -2570.
      * Problem on first media write
       77  SQLU-HEADER-WRITE-ERR     PIC S9(4) COMP-5 VALUE 2045.
      * Complete incremental history not available
       77  SQLUD-INCR-HISTORY-ERROR  PIC S9(4) COMP-5 VALUE -2571.
      * Incremental restore out of sequence
       77  SQLUD-INCR-RESTORE-OUT-OF-SEQ PIC S9(4) COMP-5 VALUE -2572.
      * Incremental image being restored non-incrementally
       77  SQLUD-NON-INCR-RESTORE    PIC S9(4) COMP-5 VALUE -2573.
      * Incremental image being restored is newer than target image
       77  SQLUD-INCR-TOO-NEW        PIC S9(4) COMP-5 VALUE -2574.
      * Incremental image being restored is older than previous image
       77  SQLUD-INCR-TOO-OLD        PIC S9(4) COMP-5 VALUE -2575.
      * Incremental restore is missing "incremental" clause
       77  SQLUD-MISSING-INCR-CLAUSE PIC S9(4) COMP-5 VALUE -2576.
      * No decompression library was found for this restore operation
       77  SQLUD-NO-DECOMPR-LIBRARY  PIC S9(4) COMP-5 VALUE -2577.
      * Specified object was not found in restore image
       77  SQLUD-OBJ-NOT-FOUND       PIC S9(4) COMP-5 VALUE -2578.
      * Restore completed but error extracting logs.
       77  SQLUD-LOGS-RESTORE-WARNING PIC S9(4) COMP-5 VALUE 2580.
      * Restore only logfiles failed.
       77  SQLUD-LOGS-RESTORE-ERROR  PIC S9(4) COMP-5 VALUE -2581.

      * sqlgadau and sqluadau Return codes
      * sql_authorizations parm is bad
       77  SQLUA-BAD-INPUT-PARAMETER PIC S9(4) COMP-5 VALUE -2600.

      * DB2SPLIT Return codes
      * invalid command line options
       77  SQLUSP-CMD-LINE-OPT-ERR   PIC S9(4) COMP-5 VALUE -2701.
      * fail to open config file
       77  SQLUSP-OPEN-CFG-FILE-ERR  PIC S9(4) COMP-5 VALUE -2702.
      * fail to open log file
       77  SQLUSP-OPEN-LOG-FILE-ERR  PIC S9(4) COMP-5 VALUE -2703.
      * fail to open input data file
       77  SQLUSP-OPEN-IN-DATA-FILE-ERR PIC S9(4) COMP-5 VALUE -2704.
      * fail to open input partition map file
       77  SQLUSP-OPEN-INPUT-MAP-FILE-ERR PIC S9(4) COMP-5 VALUE -2705.
      * fail to open output partition map file
       77  SQLUSP-OPEN-OUTMAP-FILE-ERR PIC S9(4) COMP-5 VALUE -2706.
      * fail to open distribution file
       77  SQLUSP-OPEN-DIST-FILE-ERR PIC S9(4) COMP-5 VALUE -2707.
      * fail to open output data file
       77  SQLUSP-OPEN-OUTDATA-FILE-ERR PIC S9(4) COMP-5 VALUE -2708.
      * syntax error in config file
       77  SQLUSP-CFG-SYNTAX-ERR     PIC S9(4) COMP-5 VALUE -2709.
      * invalid keyword in config file
       77  SQLUSP-INVALID-CFG-KEYWORD PIC S9(4) COMP-5 VALUE -2710.
      * column delimiter can't be a blank
       77  SQLUSP-INVALID-COL-DELIMITER PIC S9(4) COMP-5 VALUE -2711.
      * string delimiter can't be a period
       77  SQLUSP-INVALID-STR-DELIMITER PIC S9(4) COMP-5 VALUE -2712.
      * invalid run type in config file
       77  SQLUSP-INVALID-RUNTYPE    PIC S9(4) COMP-5 VALUE -2713.
      * invalid Message Level in config file
       77  SQLUSP-INVALID-MSG-LEVEL  PIC S9(4) COMP-5 VALUE -2714.
      * invalid Check Level in config file
       77  SQLUSP-INVALID-CHK-LEVEL  PIC S9(4) COMP-5 VALUE -2715.
      * record length out of range
       77  SQLUSP-INVALID-REC-LEN    PIC S9(4) COMP-5 VALUE -2716.
      * invalid node specification
       77  SQLUSP-INVALID-NODE       PIC S9(4) COMP-5 VALUE -2717.
      * invalid output node specification
       77  SQLUSP-INVALID-OUTPUTNODE PIC S9(4) COMP-5 VALUE -2718.
      * invalid output type
       77  SQLUSP-INVALID-OUTPUTTYPE PIC S9(4) COMP-5 VALUE -2719.
      * too many partitioning keys
       77  SQLUSP-TOO-MANY-PTITN-KEYS PIC S9(4) COMP-5 VALUE -2720.
      * invalid partition key specification
       77  SQLUSP-INVALID-PTITN-KEYS PIC S9(4) COMP-5 VALUE -2721.
      * invalid log file specification
       77  SQLUSP-INVALID-LOG-FILE   PIC S9(4) COMP-5 VALUE -2722.
      * invalid trace specification
       77  SQLUSP-INVALID-TRACE      PIC S9(4) COMP-5 VALUE -2723.
      * specify one and only one: MAPFILI or NODE
       77  SQLUSP-NODE-ERR           PIC S9(4) COMP-5 VALUE -2724.
      * Output partition map is needed
       77  SQLUSP-NO-OUTMAP          PIC S9(4) COMP-5 VALUE -2725.
      * no partitioning key defined
       77  SQLUSP-NO-PTITN-KEY       PIC S9(4) COMP-5 VALUE -2726.
      * key exceeds record length
       77  SQLUSP-KEY-OUT-RANGE      PIC S9(4) COMP-5 VALUE -2727.
      * output node list is not a subset of node list
       77  SQLUSP-NODE-NOT-EXISTED   PIC S9(4) COMP-5 VALUE -2728.
      * invalid data entry in input map
       77  SQLUSP-INPUT-MAP-ERR      PIC S9(4) COMP-5 VALUE -2729.
      * error writing header of out data file
       77  SQLUSP-WRITE-HEAD-ERR     PIC S9(4) COMP-5 VALUE -2730.
      * error processing input data file
       77  SQLUSP-DATA-READ-ERR      PIC S9(4) COMP-5 VALUE -2731.
      * binary data if VMMVS
       77  SQLUSP-DATA-BIN-ERR       PIC S9(4) COMP-5 VALUE -2732.
      * run type not specified
       77  SQLUSP-NO-RUNTYPE         PIC S9(4) COMP-5 VALUE -2733.
      * 32kLimit definition error
       77  SQLUSP-32KLIMIT-DEF-ERR   PIC S9(4) COMP-5 VALUE -2734.
      * discard empty record
       77  SQLUSP-DISCARD-REC-WARN   PIC S9(4) COMP-5 VALUE 2735.
      * error from sqlugrpi or sqlugrpn
       77  SQLUSP-GRPI-ERR           PIC S9(4) COMP-5 VALUE -2736.
      * error writing data file
       77  SQLUSP-DATA-WRITE-ERR     PIC S9(4) COMP-5 VALUE -2737.
      * data is truncated in writing
       77  SQLUSP-DATA-WRITE-WARN    PIC S9(4) COMP-5 VALUE 2738.
      * reclen must be defined for BIN
       77  SQLUSP-BIN-NO-RECLEN      PIC S9(4) COMP-5 VALUE -2739.
      * FLOAT is not supported for DEL/ASC
       77  SQLUSP-FLOAT-NOT-ALLOWED  PIC S9(4) COMP-5 VALUE -2740.
      * invalid file type
       77  SQLUSP-FILETYPE-DEF-ERR   PIC S9(4) COMP-5 VALUE -2741.
      * decimal len not match its precision
       77  SQLUSP-DECIMAL-LEN-NOT-MATCH PIC S9(4) COMP-5 VALUE -2742.
      * len not match for binary type data
       77  SQLUSP-DATA-LEN-NOT-MATCH PIC S9(4) COMP-5 VALUE -2743.
      * illegal filename in cfg file
       77  SQLUSP-ILLEGAL-FILENAME   PIC S9(4) COMP-5 VALUE -2744.
      * Invalid NEWLINE flag in cfg file
       77  SQLUSP-NEWLINE-DEF-ERR    PIC S9(4) COMP-5 VALUE -2745.
      * Incomplete record in input data file
       77  SQLUSP-INCOMPLETE-RECORD  PIC S9(4) COMP-5 VALUE -2746.
      * ASC record must be no longer than 32K
       77  SQLUSP-RECORD-TOO-LONG    PIC S9(4) COMP-5 VALUE -2747.
      * ASC record not long enough
       77  SQLUSP-RECORD-TOO-SHORT   PIC S9(4) COMP-5 VALUE -2748.
      * partition key not in the first 32k bytes of the record.
       77  SQLUSP-KEY-NOT-IN-32K     PIC S9(4) COMP-5 VALUE -2749.
      * line too long in cfg file
       77  SQLUSP-CFG-LINE-TOO-LONG  PIC S9(4) COMP-5 VALUE -2750.
      * expected reclen not matching actual reclen
       77  SQLUSP-REC-LEN-ERR        PIC S9(4) COMP-5 VALUE -2751.
      * invalid codepage specification
       77  SQLUSP-INVALID-CODEPAGE   PIC S9(4) COMP-5 VALUE -2752.
      * failed to get application CP
       77  SQLUSP-APP-CODEPAGE-ERR   PIC S9(4) COMP-5 VALUE -2753.
      * codepages not convertable
       77  SQLUSP-CODEPAGE-NOTABLE   PIC S9(4) COMP-5 VALUE -2754.
      * codepage-related delimiter error
       77  SQLUSP-DELIMITER-ERROR    PIC S9(4) COMP-5 VALUE -2755.
      * error converting data to DB CP
       77  SQLUSP-CP-DATA-TO-DB      PIC S9(4) COMP-5 VALUE -2756.
      * binary numerics not allowed in EBCDIC data
       77  SQLUSP-EBCDIC-NO-BIN      PIC S9(4) COMP-5 VALUE -2757.
      * DB2GPMAP Return Codes
      * Specify only tbl or nodegrp
       77  SQLUGPMAP-TBL-AND-NDGRP   PIC S9(4) COMP-5 VALUE -2761.
      * fail to find DB install path
       77  SQLUGPMAP-NO-INST-PATH    PIC S9(4) COMP-5 VALUE -2762.
      * tbl not found
       77  SQLUGPMAP-TBL-NOT-FOUND   PIC S9(4) COMP-5 VALUE -2763.
      * nodegrp not found
       77  SQLUGPMAP-NODEGRP-NOT-FOUND PIC S9(4) COMP-5 VALUE -2764.
      * fail to open file
       77  SQLUGPMAP-OPEN-OUTMAPFILE-WARN PIC S9(4) COMP-5 VALUE 2765.
      * incorrect ptitn map size
       77  SQLUGPMAP-PTITN-MAP-ERR   PIC S9(4) COMP-5 VALUE -2766.
      * invalid cmd line option
       77  SQLUGPMAP-INVALID-CMD-OPT PIC S9(4) COMP-5 VALUE -2767.
      * IMPORT/EXPORT Return codes
      * error opening output file
       77  SQLUE-DFO                 PIC S9(4) COMP-5 VALUE -3001.
      * i/o error writing output file
       77  SQLUE-IOE                 PIC S9(4) COMP-5 VALUE -3002.
      * i/o error closing output file
       77  SQLUE-CLS                 PIC S9(4) COMP-5 VALUE -3003.
      * invalid filetype parameter
       77  SQLUE-IFT                 PIC S9(4) COMP-5 VALUE -3004.
      * function interrupted
       77  SQLUE-CBI                 PIC S9(4) COMP-5 VALUE -3005.
      * i/o error opening message file
       77  SQLUE-MFO                 PIC S9(4) COMP-5 VALUE -3006.
      * i/o error writing message file
       77  SQLUE-MFW                 PIC S9(4) COMP-5 VALUE -3007.
      * start using database failed
       77  SQLUE-STA                 PIC S9(4) COMP-5 VALUE -3008.
      * invalid tcolstrg
       77  SQLUE-STR                 PIC S9(4) COMP-5 VALUE -3009.
      * invalid dcoldata
       77  SQLUE-COL                 PIC S9(4) COMP-5 VALUE -3010.

      * memory allocation error
       77  SQLUE-MEM                 PIC S9(4) COMP-5 VALUE -3011.
      * system error
       77  SQLUE-SYSERR              PIC S9(4) COMP-5 VALUE -3012.
      * invalid filetmod
       77  SQLUE-FTMOD               PIC S9(4) COMP-5 VALUE -3013.
      * failure on closing message file
       77  SQLUE-MFC                 PIC S9(4) COMP-5 VALUE -3014.
      * SQL error occurred
       77  SQLUE-SQLERR              PIC S9(4) COMP-5 VALUE -3015.
      * no keywords found
       77  SQLUE-FMODNK              PIC S9(4) COMP-5 VALUE -3016.
      * invalid delimiter or duplicate
       77  SQLUE-FMODID              PIC S9(4) COMP-5 VALUE -3017.
      * decimal used for char delimiter
       77  SQLUE-FMODDEC             PIC S9(4) COMP-5 VALUE -3018.
      * no tcolstrg
       77  SQLUE-NTS                 PIC S9(4) COMP-5 VALUE -3019.
      * insufficient authority for exp.
       77  SQLUE-RC-INSAUTH          PIC S9(4) COMP-5 VALUE -3020.

      * insufficient authority for imp.
       77  SQLUI-RC-INSAUTH          PIC S9(4) COMP-5 VALUE -3021.
      * SQL error on input string
       77  SQLUE-SQL-PREP-ERR        PIC S9(4) COMP-5 VALUE -3022.
      * invalid database name
       77  SQLUE-DATABASE            PIC S9(4) COMP-5 VALUE -3023.
      * invalid datafile
       77  SQLUE-DATAFILE            PIC S9(4) COMP-5 VALUE -3025.
      * invalid msgfile
       77  SQLUE-MSGFILE             PIC S9(4) COMP-5 VALUE -3026.
      * Export method indicator not n/d
       77  SQLUE-DCOLMETH            PIC S9(4) COMP-5 VALUE -3028.
      * filetype is null
       77  SQLUE-NUL-FTYPE           PIC S9(4) COMP-5 VALUE -3029.

      * error opening input data file
       77  SQLUI-DFO                 PIC S9(4) COMP-5 VALUE -3030.
      * i/o error reading input file
       77  SQLUI-IOE                 PIC S9(4) COMP-5 VALUE -3031.
      * Import method not n/d/p
       77  SQLUI-DCOLMETH            PIC S9(4) COMP-5 VALUE -3032.
      * invalid insert in tcolstrg
       77  SQLUI-TINSERT             PIC S9(4) COMP-5 VALUE -3033.
      * invalid into in tcolstrg
       77  SQLUI-TINTO               PIC S9(4) COMP-5 VALUE -3034.
      * invalid tablename in tcolstrg
       77  SQLUI-TABLENAME           PIC S9(4) COMP-5 VALUE -3035.
      * close paren not in tcolstrg
       77  SQLUI-CPAREN              PIC S9(4) COMP-5 VALUE -3036.
      * SQL error on insert string
       77  SQLUE-SQL-PREP-INSERT     PIC S9(4) COMP-5 VALUE -3037.
      * tcolstrg invalid
       77  SQLUI-TCOLJUNK            PIC S9(4) COMP-5 VALUE -3038.
      * load parallelism reduced
       77  SQLU-REDUCE-CPUPAR        PIC S9(4) COMP-5 VALUE 3039.
      * lob file error
       77  SQLUE-LOBFILE-ERROR       PIC S9(4) COMP-5 VALUE -3040.

      * LINKTYPE is not URL
       77  SQLUI-DL-ILLEGAL-LINKTYPE PIC S9(4) COMP-5 VALUE -3042.
      * dl_specification invalid
       77  SQLUI-DL-COL-JUNK         PIC S9(4) COMP-5 VALUE -3043.
      * multiple prefix decl per col
       77  SQLUI-DL-COL-DUP-PREFIX   PIC S9(4) COMP-5 VALUE -3044.

      * invalid dcol position for CSV
       77  SQLUIC-BAD-DCOL-POS       PIC S9(4) COMP-5 VALUE -3045.
      * non-default dcol and no cols
       77  SQLUI-NONDEF-DCOL-NOCOLS  PIC S9(4) COMP-5 VALUE -3046.
      * dcolinfo has invalid method
       77  SQLUI-BAD-DCOL-METH       PIC S9(4) COMP-5 VALUE -3047.
      * non nullable column
       77  SQLUI-NODCOL-FOR-NONNULL-DBCOL PIC S9(4) COMP-5 VALUE -3048.
      * unsupported column type
       77  SQLUIC-UNSUPTYP-NONULLS   PIC S9(4) COMP-5 VALUE -3049.

      * conversion for cdpg
       77  SQLUII-CONVERSION         PIC S9(4) COMP-5 VALUE 3050.
      * too many rows for WSF export
       77  SQLUE-ROWCT-TOOBIG        PIC S9(4) COMP-5 VALUE -3053.
      * eof reading first rec in IXF
       77  SQLUII-HEOF               PIC S9(4) COMP-5 VALUE -3054.
      * length of 'H' rec not numeric
       77  SQLUII-HLEN-CONV          PIC S9(4) COMP-5 VALUE -3055.
      * first record too short
       77  SQLUII-HLEN-SHORT         PIC S9(4) COMP-5 VALUE -3056.
      * first IXF rec is not 'H'
       77  SQLUII-HTYP               PIC S9(4) COMP-5 VALUE -3057.
      * no IXF identifier in 'H'
       77  SQLUII-HID                PIC S9(4) COMP-5 VALUE -3058.
      * invalid version field in 'H'
       77  SQLUII-HVERS              PIC S9(4) COMP-5 VALUE -3059.

      * HCNT in 'H' not numeric
       77  SQLUII-HCNT               PIC S9(4) COMP-5 VALUE -3060.
      * SBCP in 'H' not numeric
       77  SQLUII-HSBCP-BAD          PIC S9(4) COMP-5 VALUE -3061.
      * DBCP in 'H' not numeric
       77  SQLUII-HDBCP-BAD          PIC S9(4) COMP-5 VALUE -3062.
      * 'H' SBCP not compat w/data SBCP
       77  SQLUII-HSBCP-CMP          PIC S9(4) COMP-5 VALUE -3063.
      * 'H' DBCP not compat w/data DBCP
       77  SQLUII-HDBCP-CMP          PIC S9(4) COMP-5 VALUE -3064.
      * can't get codepages
       77  SQLUII-DB-CODEPG          PIC S9(4) COMP-5 VALUE -3065.
      * eof reading/looking for 'T' rec
       77  SQLUII-TEOF               PIC S9(4) COMP-5 VALUE -3066.
      * length of 'T' rec not numeric
       77  SQLUII-TLEN-CONV          PIC S9(4) COMP-5 VALUE -3067.
      * 'T' record is too short
       77  SQLUII-TLEN-SHORT         PIC S9(4) COMP-5 VALUE -3068.
      * first non-'A' rec not 'T' rec
       77  SQLUII-TTYP               PIC S9(4) COMP-5 VALUE -3069.

      * invalid rec length of 'A' rec
       77  SQLUII-ALEN-BAD           PIC S9(4) COMP-5 VALUE -3070.
      * invalid data convention in 'T'
       77  SQLUII-TCONV              PIC S9(4) COMP-5 VALUE -3071.
      * invalid data format in 'T'
       77  SQLUII-TFORM              PIC S9(4) COMP-5 VALUE -3072.
      * invalid machine form in 'T'
       77  SQLUII-TMFRM              PIC S9(4) COMP-5 VALUE -3073.
      * invalid data location in 'T'
       77  SQLUII-TLOC               PIC S9(4) COMP-5 VALUE -3074.
      * 'C' rec cnt in 'T' not numeric
       77  SQLUII-TCCNT              PIC S9(4) COMP-5 VALUE -3075.
      * name len fld in 'T' not numeric
       77  SQLUII-TNAML              PIC S9(4) COMP-5 VALUE -3076.
      * too many 'C' records
       77  SQLUII-CCNT-HIGH          PIC S9(4) COMP-5 VALUE -3077.
      * length of 'A' rec not numeric
       77  SQLUII-ALEN-CONV          PIC S9(4) COMP-5 VALUE -3078.
      * length of 'C' rec not numeric
       77  SQLUII-CLEN-CONV          PIC S9(4) COMP-5 VALUE -3079.

      * 'C' record is too short
       77  SQLUII-CLEN-SHORT         PIC S9(4) COMP-5 VALUE -3080.
      * wrong rec type / 'C' expected
       77  SQLUII-CTYP               PIC S9(4) COMP-5 VALUE -3081.
      * EOF while processing 'C' recs
       77  SQLUII-CEOF               PIC S9(4) COMP-5 VALUE -3082.
      * 'D' rec id field not numeric
       77  SQLUII-CDRID              PIC S9(4) COMP-5 VALUE -3083.
      * 'D' rec posn field not numeric
       77  SQLUII-CPOSN              PIC S9(4) COMP-5 VALUE -3084.
      * 'D' id/position not consistent
       77  SQLUII-CIDPOS             PIC S9(4) COMP-5 VALUE -3085.
      * IXF column does not exist
       77  SQLUII-NOCREC-FOR-NONNULL-DBCO PIC S9(4) COMP-5 VALUE -3086.
      * IXF column not valid
       77  SQLUII-INVCREC-NONNULL-DBCOL PIC S9(4) COMP-5 VALUE -3087.
      * IXF column not compatible
       77  SQLUII-CRECCOMP-NONNULL-DBCOL PIC S9(4) COMP-5 VALUE -3088.
      * wrong rec type / 'D' expected
       77  SQLUII-DTYP               PIC S9(4) COMP-5 VALUE -3089.

      * length of 'D' rec not numeric
       77  SQLUII-DLEN-CONV          PIC S9(4) COMP-5 VALUE -3090.
      * length of 'D' rec not valid
       77  SQLUII-DLEN-RANGE         PIC S9(4) COMP-5 VALUE -3091.
      * invalid id field in 'D' rec
       77  SQLUII-DID                PIC S9(4) COMP-5 VALUE -3092.
      * import file not valid WSF
       77  SQLUIW-IFILE-INV          PIC S9(4) COMP-5 VALUE -3093.
      * DOS non-nullable name not found
       77  SQLUIW-NNCOL-LOST         PIC S9(4) COMP-5 VALUE -3094.
      * col position out of range
       77  SQLUIW-PCOL-INV           PIC S9(4) COMP-5 VALUE -3095.
      * unsup col type - not nullable
       77  SQLUIW-UCOLTYP-NONUL      PIC S9(4) COMP-5 VALUE -3096.
      * record length invalid
       77  SQLUIW-RECLEN-INV         PIC S9(4) COMP-5 VALUE -3097.
      * row number out of range
       77  SQLUIW-INROW-INV          PIC S9(4) COMP-5 VALUE -3098.
      * col number out of range
       77  SQLUIW-INCOL-INV          PIC S9(4) COMP-5 VALUE -3099.

      * column longer than 254 chars
       77  SQLUE-COL-TOOBIG          PIC S9(4) COMP-5 VALUE 3100.
      * column has char delimiter
       77  SQLUE-DATA-CHARDEL        PIC S9(4) COMP-5 VALUE 3101.
      * dcol column nbr > tcol number
       77  SQLUE-DCNUM-HIGH          PIC S9(4) COMP-5 VALUE 3102.
      * dcol column nbr < tcol number
       77  SQLUE-DCNUM-LOW           PIC S9(4) COMP-5 VALUE 3103.
      * error formatting a message
       77  SQLUE-MFE                 PIC S9(4) COMP-5 VALUE 3106.
      * warning message issued
       77  SQLUE-WARNING             PIC S9(4) COMP-5 VALUE 3107.

      * file not linked
       77  SQLUI-DLFM-LINK           PIC S9(4) COMP-5 VALUE 3108.

      * extra database cols
       77  SQLUI-FEWER-DCOLS-DBCOLS-NULLE PIC S9(4) COMP-5 VALUE 3112.
      * column will be nulled
       77  SQLUIC-UNSUPTYP-NULLABLE  PIC S9(4) COMP-5 VALUE 3113.
      * character ignored
       77  SQLUIC-IGNORED-CHAR       PIC S9(4) COMP-5 VALUE 3114.
      * input CSV field too long
       77  SQLUIC-FIELD-TOO-LONG     PIC S9(4) COMP-5 VALUE 3115.
      * field value missing
       77  SQLUIC-CF-REQFIELD-MISSING PIC S9(4) COMP-5 VALUE 3116.
      * non NULL found for GENERATED ALWAYWS col
       77  SQLUIC-CF-GENALWAYS-NOTNULL PIC S9(4) COMP-5 VALUE 3550.
      * smallint field nulled
       77  SQLUIC-CFINT2-NULLED      PIC S9(4) COMP-5 VALUE 3117.
      * smallint field error
       77  SQLUIC-CFINT2-ROWREJ      PIC S9(4) COMP-5 VALUE 3118.
      * int field nulled
       77  SQLUIC-CFINT4-NULLED      PIC S9(4) COMP-5 VALUE 3119.

      * int field error
       77  SQLUIC-CFINT4-ROWREJ      PIC S9(4) COMP-5 VALUE 3120.
      * float field nulled
       77  SQLUIC-CFFLOAT-NULLED     PIC S9(4) COMP-5 VALUE 3121.
      * float field error
       77  SQLUIC-CFFLOAT-ROWREJ     PIC S9(4) COMP-5 VALUE 3122.
      * decimal field nulled
       77  SQLUIC-CFDEC-NULLED       PIC S9(4) COMP-5 VALUE 3123.
      * decimal field error
       77  SQLUIC-CFDEC-ROWREJ       PIC S9(4) COMP-5 VALUE 3124.
      * char field truncated
       77  SQLUIC-CFTRUNC            PIC S9(4) COMP-5 VALUE 3125.

      * char field nulled
       77  SQLUIC-CFCHAR-NULLED      PIC S9(4) COMP-5 VALUE 3126.
      * char field error
       77  SQLUIC-CFCHAR-ROWREJ      PIC S9(4) COMP-5 VALUE 3127.
      * date field truncated
       77  SQLUIC-CFDATETRUNC        PIC S9(4) COMP-5 VALUE 3128.
      * date/time/stamp field padded
       77  SQLUIC-CFDTPAD            PIC S9(4) COMP-5 VALUE 3129.

      * time field truncated
       77  SQLUIC-CFTIMETRUNC        PIC S9(4) COMP-5 VALUE 3130.
      * stamp field truncated
       77  SQLUIC-CFSTAMPTRUNC       PIC S9(4) COMP-5 VALUE 3131.
      * char field truncated
       77  SQLUE-TRUNCATE            PIC S9(4) COMP-5 VALUE 3132.
      * Datalink field nulled
       77  SQLUI-DATALINK-NULLED     PIC S9(4) COMP-5 VALUE 3133.
      * Datalink field error
       77  SQLUI-DATALINK-ROWREJ     PIC S9(4) COMP-5 VALUE 3134.
      * Too many METHOD cols
       77  SQLU-DCOL-TOO-MANY        PIC S9(4) COMP-5 VALUE -3135.
      * not enough columns
       77  SQLUIC-ROWTOOSHORT        PIC S9(4) COMP-5 VALUE 3137.
      * end of input data file
       77  SQLUIC-EOF-IN-CHARDELS    PIC S9(4) COMP-5 VALUE 3138.
      * stop using database failed
       77  SQLUE-SQLSTPDB-ERR        PIC S9(4) COMP-5 VALUE 3139.

      * WSF label too big
       77  SQLUE-WSFLAB-LEN          PIC S9(4) COMP-5 VALUE 3142.
      * WSF char type length too long
       77  SQLUE-CHAR-MTRUNC         PIC S9(4) COMP-5 VALUE 3143.
      * WSF char type length too long
       77  SQLUE-CHAR-WTRUNC         PIC S9(4) COMP-5 VALUE 3144.
      * WSF char truncated at 240
       77  SQLUE-CHAR-ITRUNC         PIC S9(4) COMP-5 VALUE 3145.
      * row count exceeds 2048
       77  SQLUE-ROWCT-LARGE         PIC S9(4) COMP-5 VALUE 3147.
      * row not inserted
       77  SQLUE-3148                PIC S9(4) COMP-5 VALUE 3148.

      * 'H' hcnt not equal 'T' rec ccnt
       77  SQLUII-TCNTCMP            PIC S9(4) COMP-5 VALUE 3154.
      * invalid name length in 'C' rec
       77  SQLUII-CNAML              PIC S9(4) COMP-5 VALUE 3155.
      * invalid null field in 'C' rec
       77  SQLUII-CNULL              PIC S9(4) COMP-5 VALUE 3156.
      * invalid type field in 'C' rec
       77  SQLUII-CTYPE              PIC S9(4) COMP-5 VALUE 3157.
      * invalid SBCP field in 'C' rec
       77  SQLUII-CSBCP              PIC S9(4) COMP-5 VALUE 3158.
      * invalid DBCP field in 'C' rec
       77  SQLUII-CDBCP              PIC S9(4) COMP-5 VALUE 3159.

      * invalid col len fld in 'C' rec
       77  SQLUII-CLENG              PIC S9(4) COMP-5 VALUE 3160.
      * invalid precision in 'C' rec
       77  SQLUII-CPREC              PIC S9(4) COMP-5 VALUE 3161.
      * invalid scale field in 'C' rec
       77  SQLUII-CSCAL              PIC S9(4) COMP-5 VALUE 3162.
      * use 00008 for float col length
       77  SQLUII-CFLOAT-BLANKLENG   PIC S9(4) COMP-5 VALUE 3163.
      * invalid float col len in 'C'.
       77  SQLUII-CFLOAT-BADLENG     PIC S9(4) COMP-5 VALUE 3164.
      * 'C' record has invalid type
       77  SQLUII-CUTYPE             PIC S9(4) COMP-5 VALUE 3165.
      * IXF col does not exist
       77  SQLUII-NOCREC-FOR-NULL-DBCOL PIC S9(4) COMP-5 VALUE 3166.
      * IXF col is invalid
       77  SQLUII-INVCREC-FOR-NULL-DBCOL PIC S9(4) COMP-5 VALUE 3167.
      * IXF col not compatible
       77  SQLUII-CRECCOMP-NULL-DBCOL PIC S9(4) COMP-5 VALUE 3168.

      * EOF found in row of data
       77  SQLUII-DEOF-INROW         PIC S9(4) COMP-5 VALUE 3170.
      * non-label cell in hdr row
       77  SQLUIW-NONLAB-HDR         PIC S9(4) COMP-5 VALUE 3171.
      * nullable colnam not found
       77  SQLUIW-NCOL-LOST          PIC S9(4) COMP-5 VALUE 3172.
      * unsup col type - nullable
       77  SQLUIW-UCOLTYP-NULL       PIC S9(4) COMP-5 VALUE 3174.
      * wrong rec type for db col
       77  SQLUIW-INTYPE-INV         PIC S9(4) COMP-5 VALUE 3175.
      * date value out of range
       77  SQLUIW-DATE-INV           PIC S9(4) COMP-5 VALUE 3176.
      * time value out of range
       77  SQLUIW-TIME-INV           PIC S9(4) COMP-5 VALUE 3177.
      * int rec for time invalid
       77  SQLUIW-INTIME-INV         PIC S9(4) COMP-5 VALUE 3178.
      * no data / non nullable col
       77  SQLUIW-NODATA-NNULL       PIC S9(4) COMP-5 VALUE 3179.

      * insert diskette request
       77  SQLUE-INSERT-DISK         PIC S9(4) COMP-5 VALUE 3180.
      * file ended before AE rec
       77  SQLUII-AE-NOTFOUND        PIC S9(4) COMP-5 VALUE 3181.
      * retry to insert diskette
       77  SQLUII-INSERT-DISK-RETRY  PIC S9(4) COMP-5 VALUE 3182.
      * mult del o'rides/no blanks
       77  SQLUEC-NOBLANK-B4KW       PIC S9(4) COMP-5 VALUE 3183.
      * row of previous warning
       77  SQLUI-PREVMESG-ROWNO      PIC S9(4) COMP-5 VALUE 3185.
      * log full inserting row
       77  SQLUI-LOGFULL-INSWARN     PIC S9(4) COMP-5 VALUE 3186.
      * error creating index
       77  SQLUI-INDEX-WARN          PIC S9(4) COMP-5 VALUE 3187.
      * error truncating table
       77  SQLUI-TRUNCATE-TABLE      PIC S9(4) COMP-5 VALUE -3188.

      * invalid INDEXIXF option
       77  SQLUI-INDEXIXF            PIC S9(4) COMP-5 VALUE -3190.
      * data not fit modifier
       77  SQLUE-INVALID-DATE-DATA   PIC S9(4) COMP-5 VALUE 3191.
      * invalid user date modifier
       77  SQLUE-INVALID-DATE-SPEC   PIC S9(4) COMP-5 VALUE -3192.
      * cannot import to this view
       77  SQLUI-VIEW-ERROR          PIC S9(4) COMP-5 VALUE -3193.
      * cannot import system table
       77  SQLUI-SYSTBL-ERROR        PIC S9(4) COMP-5 VALUE -3194.
      * not enough space
       77  SQLUE-RETRY-DISK          PIC S9(4) COMP-5 VALUE 3195.
      * input file not found
       77  SQLUI-IN-NOTFD            PIC S9(4) COMP-5 VALUE -3196.
      * import/export in use
       77  SQLUI-IMPBUSY             PIC S9(4) COMP-5 VALUE -3197.

      * cant replace parent table
       77  SQLUI-REPL-PAR            PIC S9(4) COMP-5 VALUE -3201.
      * cant update without PK's
       77  SQLUI-IUOPT-NOPK          PIC S9(4) COMP-5 VALUE -3203.
      * cant update views
       77  SQLUI-IUOPT-NOVIEW        PIC S9(4) COMP-5 VALUE -3204.
      * cant replace ref cons view
       77  SQLUI-VIEW-REF            PIC S9(4) COMP-5 VALUE -3205.
      * cant replace subquery view
       77  SQLUI-VIEW-SQUERY         PIC S9(4) COMP-5 VALUE -3206.

      * Invalid table list
       77  SQLU-INVALID-TABLES-LIST  PIC S9(4) COMP-5 VALUE -3207.
      * Import Typed-table to Reg
       77  SQLU-TYPED-IMPORT-INTO-REGULAR PIC S9(4) COMP-5 VALUE 3208.
      * Cannot rename sub-table/attr
       77  SQLU-CANT-RENAM-SUBTAB-OR-ATTR PIC S9(4) COMP-5 VALUE -3209.
      * Options incompatible w/ hier
       77  SQLU-INCOMPATIBLE-HIERARCHY PIC S9(4) COMP-5 VALUE -3210.
      * Load doesn't supp RT
       77  SQLU-LOAD-DOESNT-SUPP-RT  PIC S9(4) COMP-5 VALUE -3211.
      * not supported Load Terminate operation
       77  SQLU-NOSUPP-LD-TERM-OP    PIC S9(4) COMP-5 VALUE -3212.

      * Load indexing mode
       77  SQLU-INXMODE-INFO         PIC S9(4) COMP-5 VALUE 3213.
      * Deferred indexing, but unique inx
       77  SQLU-INXMODE-DEFBUTUNIQUE PIC S9(4) COMP-5 VALUE -3214.
      * Load incrmental indexing + DMS + copy + same TS
       77  SQLU-INXMODE-DMS-RESTRICTION PIC S9(4) COMP-5 VALUE 3215.
      * Load incrmental indexing but inx invalid
       77  SQLU-INXMODE-INC-BUTBADINX PIC S9(4) COMP-5 VALUE 3216.
      * Load incrmental indexing but not insert
       77  SQLU-INXMODE-INC-BUTNOTINSERT PIC S9(4) COMP-5 VALUE 3217.
      * Index file is damaged/missing
       77  SQLU-INDEX-FILE-MISSING   PIC S9(4) COMP-5 VALUE -3218.
      * Load unable to turn off constraints
       77  SQLU-CONSTRAINTS-OFF-FAILED PIC S9(4) COMP-5 VALUE -3219.

      * AIX req next file
       77  SQLUE-PROVIDE-FILE-PART   PIC S9(4) COMP-5 VALUE 3220.
      * start commit
       77  SQLUI-START-COMMIT        PIC S9(4) COMP-5 VALUE 3221.
      * finish commit
       77  SQLUI-FINISH-COMMIT       PIC S9(4) COMP-5 VALUE 3222.
      * bad input parms
       77  SQLUI-BAD-STRUCT-PARM     PIC S9(4) COMP-5 VALUE -3223.
      * restartcnt too big
       77  SQLUI-SKIPPED-ALL-ROWS    PIC S9(4) COMP-5 VALUE -3225.
      * map special token to user record
       77  SQLU-WHICH-USER-RECORD    PIC S9(4) COMP-5 VALUE 3227.
      * Datalink table, deferred indexing not allowed on load
       77  SQLU-DL-AND-DEFERRED-INX  PIC S9(4) COMP-5 VALUE -3228.
      * compound=x error
       77  SQLUI-COMPOUND-ERR        PIC S9(4) COMP-5 VALUE -3250.

      * row/col sequence invalid
       77  SQLUIW-RCSEQ-INV          PIC S9(4) COMP-5 VALUE -3300.
      * BOF in middle of WSF file
       77  SQLUIW-BOF-INV            PIC S9(4) COMP-5 VALUE -3301.
      * unexpected EOF
       77  SQLUIW-EARLY-EOF          PIC S9(4) COMP-5 VALUE -3302.
      * filetype not ixf
       77  SQLUI-IXFONLY             PIC S9(4) COMP-5 VALUE -3303.
      * table does not exist
       77  SQLUI-DELTABLE            PIC S9(4) COMP-5 VALUE -3304.
      * table already exists
       77  SQLUI-CREATE-ERR          PIC S9(4) COMP-5 VALUE -3305.
      * SQL error during insert
       77  SQLUI-EXECUTE-ERR         PIC S9(4) COMP-5 VALUE -3306.
      * incomplete col info
       77  SQLUI-INC-COL             PIC S9(4) COMP-5 VALUE -3307.
      * codepage mismatch
       77  SQLUI-CP-MISMATCH         PIC S9(4) COMP-5 VALUE -3308.
      * double byte data found
       77  SQLUI-DBLDATA             PIC S9(4) COMP-5 VALUE -3309.

      * unrec col type
       77  SQLUI-UNREC-CTYPE         PIC S9(4) COMP-5 VALUE -3310.
      * invalid IXF column
       77  SQLUI-INVCREC-FOR-CREATE  PIC S9(4) COMP-5 VALUE -3310.

      * disk full - OS/2
       77  SQLUE-DISK-FULL-DB2OS2    PIC S9(4) COMP-5 VALUE -3313.
      * disk full - Windows NT
       77  SQLUE-DISK-FULL-DB2NT     PIC S9(4) COMP-5 VALUE -3313.
      * disk full - DOS
       77  SQLUE-DISK-FULL-DB2DOS    PIC S9(4) COMP-5 VALUE -3313.
      * disk full - Windows
       77  SQLUE-DISK-FULL-DB2WIN    PIC S9(4) COMP-5 VALUE -3313.
      * disk full - AIX
       77  SQLUE-DISK-FULL-DB2AIX    PIC S9(9) COMP-5 VALUE -10018.
      * disk full - MacOS
       77  SQLUE-DISK-FULL-DB2MAC    PIC S9(4) COMP-5 VALUE -3313.

       77  SQLUE-DISK-FULL           PIC S9(4) COMP-5 VALUE -3313.

      * 'A' data/ time not as 'H'.
       77  SQLUII-ASTAMP-NOMATCH     PIC S9(4) COMP-5 VALUE -3314.
      * invalid volume info
       77  SQLUII-ACREC-BADVOL       PIC S9(4) COMP-5 VALUE -3315.
      * error closing IXF file
       77  SQLUII-CLOSE-NOTLAST      PIC S9(4) COMP-5 VALUE -3316.
      * conflict in filetmod
       77  SQLUW-FTMOD-INV           PIC S9(4) COMP-5 VALUE -3317.
      * keyword repeated/filetmod
       77  SQLUEC-DUP-KEYWORD        PIC S9(4) COMP-5 VALUE -3318.
      * error creating table
       77  SQLUI-ERR-CREATETAB       PIC S9(4) COMP-5 VALUE -3319.

      * keyword at end of filetmod
       77  SQLUEC-NOROOM-AFTERKW     PIC S9(4) COMP-5 VALUE -3320.
      * circular log full
       77  SQLUI-LOGFULL-INSERR      PIC S9(4) COMP-5 VALUE -3321.
      * semaphore error
       77  SQLUE-SEM-ERROR           PIC S9(4) COMP-5 VALUE -3322.
      * column type invalid
       77  SQLUE-INVCOLTYPE          PIC S9(4) COMP-5 VALUE -3324.
      * ignored WSF row
       77  SQLUEW-IGNORED-ROW        PIC S9(4) COMP-5 VALUE 3325.
      * column list invalid
       77  SQLUI-COL-LIST            PIC S9(4) COMP-5 VALUE -3326.
      * system error
       77  SQLUEI-SYSERROR           PIC S9(4) COMP-5 VALUE -3327.
      * Error importing to nickname
       77  SQLUI-NICKNAME-ERR        PIC S9(9) COMP-5 VALUE -27999.
      * Some metadata will not be saved to IXF on Export
       77  SQLUE-NO-IXF-INFO         PIC S9(9) COMP-5 VALUE 27984.

      * odd leng char -> graphic
       77  SQLUII-ODD2GRAPH          PIC S9(4) COMP-5 VALUE 3330.
      * permission denied
       77  SQLUE-OEACCESS            PIC S9(4) COMP-5 VALUE -3331.
      * too many files open
       77  SQLUE-OEMFILE             PIC S9(4) COMP-5 VALUE -3332.
      * no such file or directory
       77  SQLUE-OENOENT             PIC S9(4) COMP-5 VALUE -3333.
      * not enough memory
       77  SQLUE-OENOMEM             PIC S9(4) COMP-5 VALUE -3334.
      * no space left
       77  SQLUE-OENOSPC             PIC S9(4) COMP-5 VALUE -3335.
      * read access load conditions not met
       77  SQLU-READ-ACCESS-NOT-ALLOWED PIC S9(4) COMP-5 VALUE -3340.
      * use tablespace incorrect
       77  SQLU-INVALID-USE-TABLESPACE PIC S9(4) COMP-5 VALUE -3341.
      * insufficient authority to issue lock with force
       77  SQLU-LOCK-WITH-FORCE-DENIED PIC S9(4) COMP-5 VALUE -3342.
      * cant load restart after rollforward
       77  SQLU-NO-RSTART-POST-RLFWARD PIC S9(4) COMP-5 VALUE -3343.
      * use tablespace warning message
       77  SQLU-USE-TABLESPACE-WARNING PIC S9(4) COMP-5 VALUE 3346.

      * invalid method for ASC
       77  SQLUIA-BAD-DCOL-METH      PIC S9(4) COMP-5 VALUE -3400.
      * invalid import method
       77  SQLUI-DCOLM-ALL           PIC S9(4) COMP-5 VALUE -3401.
      * zeroes as begin/end
       77  SQLUIA-NULLLOC            PIC S9(4) COMP-5 VALUE -3402.
      * invalid pair
       77  SQLUIA-LOCPAIR            PIC S9(4) COMP-5 VALUE -3403.
      * invalid pair for number
       77  SQLUIA-LOCNUM             PIC S9(4) COMP-5 VALUE -3404.
      * invalid pair for date
       77  SQLUIA-LOCDATE            PIC S9(4) COMP-5 VALUE -3405.
      * invalid pair for time
       77  SQLUIA-LOCTIME            PIC S9(4) COMP-5 VALUE -3406.
      * invalid pair for timestamp
       77  SQLUIA-LOCSTAMP           PIC S9(4) COMP-5 VALUE -3407.
      * pair defines long field
       77  SQLUIA-LOCLONG            PIC S9(4) COMP-5 VALUE 3408.
      * pair defines short field
       77  SQLUIA-LOCSHORT           PIC S9(4) COMP-5 VALUE 3409.
      * invalid pair for graphic
       77  SQLUIA-LOCODD             PIC S9(4) COMP-5 VALUE -3410.
      * value not graphic--null
       77  SQLUIA-CFGRAPH-NULLED     PIC S9(4) COMP-5 VALUE 3411.
      * value not graphic--not null
       77  SQLUIA-CFGRAPH-ROWREJ     PIC S9(4) COMP-5 VALUE 3412.
      * field too short--nulled
       77  SQLUIA-SHORTFLDNULLED     PIC S9(4) COMP-5 VALUE 3413.
       77  SQLU-NO-LIFEBOAT          PIC S9(4) COMP-5 VALUE -3414.
      * CPCV failed--null
       77  SQLUIA-CFCPCV-NULLED      PIC S9(4) COMP-5 VALUE 3415.
      * CPCV failed--not null
       77  SQLUIA-CFCPCV-ROWREJ      PIC S9(4) COMP-5 VALUE 3416.
      * Modified by NOCHARDEL usage warning
       77  SQLU-NOCHARDEL-WARNING    PIC S9(4) COMP-5 VALUE 3418.
      * Vendor sort for collating type is unsupported, using default db2
      * sort
       77  SQLU-VENDOR-SORT-IGNORED  PIC S9(4) COMP-5 VALUE 3419.

      * number of warnings hit threshold
       77  SQLU-TOO-MANY-WARNINGS    PIC S9(4) COMP-5 VALUE -3502.
      * number of rows hit threshold
       77  SQLU-ROWCNT               PIC S9(4) COMP-5 VALUE 3503.
      * reclen > 32767
       77  SQLULA-INVALID-RECLEN     PIC S9(4) COMP-5 VALUE -3505.
      * null ind value not Y or N
       77  SQLULA-NULLIND-IGNORED    PIC S9(4) COMP-5 VALUE 3506.
      * nullind column is invalid
       77  SQLUI-NULLIND             PIC S9(4) COMP-5 VALUE -3507.
      * file access error during load
       77  SQLUL-FILE-ERROR          PIC S9(4) COMP-5 VALUE -3508.
      * num of row deleted after load
       77  SQLUL-NUM-ROW-DELETED     PIC S9(4) COMP-5 VALUE 3509.
      * work directory is invalid
       77  SQLU-SORT-WORK-DIR-ERROR  PIC S9(4) COMP-5 VALUE -3510.
      * lobfile missing but nullable col
       77  SQLU-NB-LOBFILE-MISSING   PIC S9(4) COMP-5 VALUE 3511.
      * lobfile missing, nonnullable col
       77  SQLU-NNB-LOBFILE-MISSING  PIC S9(4) COMP-5 VALUE 3512.
      * codepage doesn't match db
       77  SQLUL-UNMATCHED-CODEPAGE  PIC S9(4) COMP-5 VALUE -3513.
      * system error with reason code
       77  SQLUL-SYSERR-WITH-REASON  PIC S9(4) COMP-5 VALUE -3514.
      * unexpected rec in db2cs
       77  SQLUL-UNEXPECTED-RECORD   PIC S9(4) COMP-5 VALUE -3517.
      * coltype incompatible for db2cs
       77  SQLUL-INCOMPATIBLE-TABLE  PIC S9(4) COMP-5 VALUE -3518.
      * missing file
       77  SQLUL-FILE-NOT-FOUND      PIC S9(4) COMP-5 VALUE -3521.
      * copy spec, no logretain/userexit
       77  SQLUL-COPY-LOGRETAIN-OFF  PIC S9(4) COMP-5 VALUE -3522.
      * no messages to be retrieved
       77  SQLUL-NO-MESSAGES         PIC S9(4) COMP-5 VALUE 3523.
      * freespace option invalid
       77  SQLUL-FREESPACE-OPT       PIC S9(4) COMP-5 VALUE -3524.
      * incompatible options
       77  SQLU-INCOMPAT-OPT         PIC S9(4) COMP-5 VALUE -3525.
      * modifier incompatible with load options
       77  SQLU-MOD-INCOMPAT-WITH-OPT PIC S9(4) COMP-5 VALUE -3526.
      * invalid codepage
       77  SQLULA-INVALID-CODEPAGE   PIC S9(4) COMP-5 VALUE -3527.
      * delimiter may be converted from app to DB
       77  SQLUL-DELIMITER-CONV-W    PIC S9(4) COMP-5 VALUE 3528.
      * Unsupported data type
       77  SQLUL-UNSUPPORTED-DATA-TYPE PIC S9(4) COMP-5 VALUE -3529.
      * Load index creation parameter no longer supported
       77  SQLUL-OBSOLETETE-SORT-PARM PIC S9(4) COMP-5 VALUE 3535.
      * Generated override warning
       77  SQLUL-GENERATED-OVERRIDE  PIC S9(4) COMP-5 VALUE 3551.
      * mount new tape
       77  SQLU-NEXT-TAPE-WARNING    PIC S9(4) COMP-5 VALUE 3700.
      * no lobs/longs but lobpath nonull
       77  SQLU-LOBPATHS-IGNORED     PIC S9(4) COMP-5 VALUE 3701.
      * device error but ignored
       77  SQLU-DEVICE-IGNORED       PIC S9(4) COMP-5 VALUE 3702.
      * invalid number of buffers
       77  SQLU-NUM-BUFFERS          PIC S9(4) COMP-5 VALUE -3704.
      * invalid load/unload buffer size
       77  SQLU-BUFFER-SIZE-ERROR    PIC S9(4) COMP-5 VALUE -3705.
      * copy target full
       77  SQLUL-DISK-FULL           PIC S9(4) COMP-5 VALUE -3706.
      * invalid sort buffer size
       77  SQLU-SORT-BUFFSIZE-ERROR  PIC S9(4) COMP-5 VALUE -3707.
      * Hash table is full
       77  SQLUE-NOSPACE-IN-HASH     PIC S9(4) COMP-5 VALUE -3708.

      * Load / unload / load recovery SQLCODES
       77  SQLU-OPEN-COPY-LOC-FILE-ERROR PIC S9(4) COMP-5 VALUE -3783.
       77  SQLU-INV-COPY-LOC-FILE-INPUT PIC S9(4) COMP-5 VALUE -3784.
       77  SQLU-LOAD-RECOVERY-FAILED PIC S9(4) COMP-5 VALUE -3785.
       77  SQLU-INVALID-PARM-WARNING PIC S9(4) COMP-5 VALUE 3798.
       77  SQLU-LOAD-RECOVERY-PENDING PIC S9(4) COMP-5 VALUE 3799.

      * load recovery - copy location input error type
       77  SQLU-KEYWORD-CODE         PIC S9(4) COMP-5 VALUE 1.
       77  SQLU-OVERRIDE-CODE        PIC S9(4) COMP-5 VALUE 2.
       77  SQLU-UNEXPECTED-EOF-CODE  PIC S9(4) COMP-5 VALUE 3.
       77  SQLU-IOERROR-CODE         PIC S9(4) COMP-5 VALUE 4.

      * load recovery - Different Load recovery options
       77  SQLU-RECOVERABLE-LOAD     PIC S9(4) COMP-5 VALUE 0.
       77  SQLU-NON-RECOVERABLE-LOAD PIC S9(4) COMP-5 VALUE 1.

      * Loadapi SQLCODES
       77  SQLU-INVALID-QUIESCEMODE  PIC S9(4) COMP-5 VALUE -3802.
       77  SQLU-INVALID-INDEX        PIC S9(4) COMP-5 VALUE -3804.
       77  SQLU-INVALID-LOADAPI-ACTION PIC S9(4) COMP-5 VALUE -3805.
       77  SQLU-CONSTRAINTS-NOT-OFF  PIC S9(4) COMP-5 VALUE -3806.

      * Export SQLCODES
      * Export message
       77  SQLUE-MSG                 PIC S9(4) COMP-5 VALUE -3999.

      * Roll-Forward Recovery SQLCODES
      * invalid parameter
       77  SQLU-INVALID-PARAM        PIC S9(4) COMP-5 VALUE -4904.
      * invalid parameter range
       77  SQLU-INVALID-RANGE        PIC S9(4) COMP-5 VALUE -4905.
      * invalid tablespace set
       77  SQLUM-INVALID-TPS-SET     PIC S9(4) COMP-5 VALUE -4906.
      * tables set to check pending state
       77  SQLUM-CHECK-PENDING-SET   PIC S9(4) COMP-5 VALUE 4907.
      * tablespace not ready to roll forward
       77  SQLUM-TSP-NOT-READY       PIC S9(4) COMP-5 VALUE -4908.
      * Tables in tablespaces are in DRP/RNP state
       77  SQLU-RFR-DATALINK-PENDING PIC S9(4) COMP-5 VALUE 4909.
      * Invalid overflow log path
       77  SQLU-INVALID-OFLOGPATH    PIC S9(4) COMP-5 VALUE -4910.
      * missing log files
       77  SQLU-RC-MISSING-LOGFILES  PIC S9(4) COMP-5 VALUE -4970.
      * log truncation failed
       77  SQLU-RC-LOG-TRUNCATED     PIC S9(4) COMP-5 VALUE -4971.
      * log path full
       77  SQLU-RC-LOGPATH-FULL      PIC S9(4) COMP-5 VALUE -4972.
      * log mismatch with catalog node
       77  SQLU-RC-LOG-MISMATCH      PIC S9(4) COMP-5 VALUE -4973.
      * query status warning
       77  SQLU-RC-QRY-ERR-WARN      PIC S9(4) COMP-5 VALUE 4974.
      * rollforward canceled
       77  SQLU-RC-CANCELED-WARN     PIC S9(4) COMP-5 VALUE 4975.
      * not on catalog node
       77  SQLU-RC-NOT-ON-CATALOG-NODE PIC S9(4) COMP-5 VALUE -4976.
      * bad export directory
       77  SQLU-RC-EXPORT-DIR        PIC S9(4) COMP-5 VALUE -4977.
      * bad dropped table recovery option
       77  SQLU-RC-DROPPED-TBL       PIC S9(4) COMP-5 VALUE -4978.
      * error while exporting table data
       77  SQLU-RC-EXPORT-DATA       PIC S9(4) COMP-5 VALUE 4979.
      * Corrupt Log file
       77  SQLU-RC-LOGFILE-CORRUPT   PIC S9(4) COMP-5 VALUE -4980.

      * Configuration SQLCODES
      * only SYSADM_GROUP can change db2 configuration file
       77  SQLF-RC-SYSAUTH           PIC S9(4) COMP-5 VALUE -5001.
      * system error
       77  SQLF-RC-SYSERR            PIC S9(4) COMP-5 VALUE -5005.
      * path name error
       77  SQLF-RC-PATHNAME          PIC S9(4) COMP-5 VALUE -5010.
      * invalid node name
       77  SQLF-RC-INVNODENAME       PIC S9(4) COMP-5 VALUE -5020.
      * invalid release number
       77  SQLF-RC-REL               PIC S9(4) COMP-5 VALUE -5030.
      * database needs migration; release number is back-level
       77  SQLF-RC-NEEDMIG           PIC S9(4) COMP-5 VALUE -5035.
      * insufficient memory to support stack switching
       77  SQLF-RC-INSMEM            PIC S9(4) COMP-5 VALUE -5047.
      * invalid db2 configuration file
       77  SQLF-RC-SYSCSUM           PIC S9(4) COMP-5 VALUE -5050.
      * invalid db configuration file
       77  SQLF-RC-DBCSUM            PIC S9(4) COMP-5 VALUE -5055.
      * invalid token parameter
       77  SQLF-RC-INVTKN            PIC S9(4) COMP-5 VALUE -5060.
      * invalid ptr to sqlfupd
       77  SQLF-RC-INVTKN-STRUCT     PIC S9(4) COMP-5 VALUE -5061.
      * invalid token ptr value
       77  SQLF-RC-INVTKN-PTR        PIC S9(4) COMP-5 VALUE -5062.
      * warning - truncated result due to obsolete db cfg token
       77  SQLF-RC-OLD-DB-CFG-TKN-TRUNC PIC S9(4) COMP-5 VALUE 5066.
      * invalid count parameter
       77  SQLF-RC-CNTINV            PIC S9(4) COMP-5 VALUE -5070.
      * invalid logging db cfg parameter
       77  SQLF-RC-INVLOGCFG         PIC S9(4) COMP-5 VALUE -5099.
      * invalid new log path - use SQLF_RC_INVLOGCFG instead
       77  SQLF-RC-INVNEWLOGP        PIC S9(4) COMP-5 VALUE -5099.

      * invalid bit value - must be 0 or 1
       77  SQLF-RC-INV-BIT-VALUE     PIC S9(4) COMP-5 VALUE -5112.
      * set alt_collate on unicode db not allowed
       77  SQLF-RC-ALT-COLLATE       PIC S9(4) COMP-5 VALUE -5113.

      * invalid DB configuration details
       77  SQLF-RC-INVDETS           PIC S9(4) COMP-5 VALUE -5121.
      * database is copy protected
       77  SQLF-RC-PROTECT           PIC S9(4) COMP-5 VALUE -5122.
      * I/O Error with log header
       77  SQLF-RC-LOGIO             PIC S9(4) COMP-5 VALUE -5123.
      * invalid db2 config file entry
       77  SQLF-RC-INV-DBMENT        PIC S9(4) COMP-5 VALUE -5126.
      * integer out of range
       77  SQLF-RC-INV-RANGE         PIC S9(4) COMP-5 VALUE -5130.
      * integer out of range (-1)
       77  SQLF-RC-INV-RANGE-2       PIC S9(4) COMP-5 VALUE -5131.
      * string null or too long
       77  SQLF-RC-INV-STRING        PIC S9(4) COMP-5 VALUE -5132.
      * char/int not in set
       77  SQLF-RC-INV-SET           PIC S9(4) COMP-5 VALUE -5133.
      * tpname not valid
       77  SQLF-RC-INVTPNAME         PIC S9(4) COMP-5 VALUE -5134.
      * dftdbpath not valid
       77  SQLF-RC-INV-DBPATH        PIC S9(4) COMP-5 VALUE -5136.
      * diagpath not valid
       77  SQLF-RC-INV-DIAGPATH      PIC S9(4) COMP-5 VALUE -5137.
      * invalid agent priority
       77  SQLF-RC-INV-AGENTPRI      PIC S9(4) COMP-5 VALUE -5131.
      * out of range - value too small
       77  SQLF-RC-INV-RANGE-TOO-SMALL PIC S9(4) COMP-5 VALUE -5150.
      * out of range - value too small (-1 is allowed)
       77  SQLF-RC-INV-RANGE-TOO-SMALL-2 PIC S9(4) COMP-5 VALUE -5151.
      * out of range - value too large
       77  SQLF-RC-INV-RANGE-TOO-BIG PIC S9(4) COMP-5 VALUE -5152.
      * out of range - condition violated
       77  SQLF-RC-INV-RANGE-CONDITION PIC S9(4) COMP-5 VALUE -5153.
      * authentication must be CLIENT for trust_allcnts = NO or
      * trust_clntauth = SERVER
       77  SQLF-RC-INV-AUTH-TRUST    PIC S9(4) COMP-5 VALUE -5154.
      * sortheap performance warning
       77  SQLF-RC-SORTHEAP-PERF     PIC S9(4) COMP-5 VALUE 5155.

      * Data Redistribution Return Codes
      * nodegroup name is undefined
       77  SQLUT-NGNAME-UNDEF        PIC S9(4) COMP-5 VALUE -204.
      * node is a duplicate node
       77  SQLUT-NODE-DUP            PIC S9(4) COMP-5 VALUE -265.
      * node is undefined
       77  SQLUT-NODE-UNDEF          PIC S9(4) COMP-5 VALUE -266.
      * max no. of part. map reached
       77  SQLUT-OVER-MAX-PARTNO     PIC S9(4) COMP-5 VALUE -269.
      * redist undefined for sys obj.
       77  SQLUT-REDIST-UNDEF        PIC S9(4) COMP-5 VALUE -607.
      * insufficient authority
       77  SQLUT-INSAUTH             PIC S9(4) COMP-5 VALUE -1092.
      * file or dir access denied
       77  SQLUT-ACCESS-DENIED       PIC S9(4) COMP-5 VALUE -1326.
      * for tablespaces on a node
       77  SQLUT-NO-CONTAINERS       PIC S9(4) COMP-5 VALUE -1755.
      * invalid authorizations -2018
       77  SQLUT-INVALID-AUTHS       PIC S9(4) COMP-5 VALUE -2018.
      * parm to utility incorrect -2032
       77  SQLUT-INVALID-PARM        PIC S9(4) COMP-5 VALUE -2032.
      * addr of parm incorrect -2034
       77  SQLUT-INVALID-PARM-ADDRESS PIC S9(4) COMP-5 VALUE -2034.
      * function interruption -3005
       77  SQLUT-CBI                 PIC S9(4) COMP-5 VALUE -3005.
      * redist failed - no part key
       77  SQLUT-REDIST-NO-PARTKEY   PIC S9(4) COMP-5 VALUE -6047.
      * error found in the input file
       77  SQLUT-ERR-IN-FILE         PIC S9(4) COMP-5 VALUE -6053.
      * redistribution not performed
       77  SQLUT-RC-REDIST-ERR       PIC S9(4) COMP-5 VALUE -6056.
      * Log Extent file size changed
       77  SQLU-DIFF-LOG-SIZE        PIC S9(4) COMP-5 VALUE -6063.
      * error during data redistbution
       77  SQLUT-ERROR-REDIST        PIC S9(4) COMP-5 VALUE -6064.
      * error writing output file
       77  SQLUT-WRT-OUT-FILE        PIC S9(4) COMP-5 VALUE -6065.
      * Load Header Return Codes
      * Invalid partition map
       77  SQLU-PARTITIONMAP         PIC S9(4) COMP-5 VALUE -6100.
      * Invalid node number
       77  SQLU-NODE-NUMBER          PIC S9(4) COMP-5 VALUE -6101.
      * Parameter reserved for future
       77  SQLU-FUTURE-PARM          PIC S9(4) COMP-5 VALUE -6102.
      * Unexpected load system error
       77  SQLU-LOAD-SYSERR          PIC S9(4) COMP-5 VALUE -6103.
      * Load does not support indices
       77  SQLU-NO-INDICES           PIC S9(4) COMP-5 VALUE -6104.
      * Load complete - backup NOW !
       77  SQLU-LOAD-COMPLETE        PIC S9(4) COMP-5 VALUE -6105.
      * Invalid use of NOHEADER
       77  SQLU-NOHEADER             PIC S9(4) COMP-5 VALUE -6106.
      * Invalid partitioning key
       77  SQLU-PARTITION-KEY        PIC S9(4) COMP-5 VALUE -6107.
      * Wrong number of partition keys
       77  SQLU-PARTITION-KEY-NUM    PIC S9(4) COMP-5 VALUE -6108.
      * Unexpected partitioning key
       77  SQLU-PARTITION-KEY-NAME   PIC S9(4) COMP-5 VALUE -6109.
      * Unexpected partition key type
       77  SQLU-PARTITION-KEY-TYPE   PIC S9(4) COMP-5 VALUE -6110.
      * Repository for obsolete Return Codes

      * wrote wrong # of bytes
       77  SQLU-WRITE-ERROR          PIC S9(4) COMP-5 VALUE -2006.
      * error in Start Using
       77  SQLU-CONNECT-ERROR        PIC S9(4) COMP-5 VALUE -2010.
      * error in ints
       77  SQLU-INT-ERROR            PIC S9(4) COMP-5 VALUE -2012.
      * ADSM reported error
       77  SQLU-ADSM-ERROR           PIC S9(4) COMP-5 VALUE -2033.
      * unable to load ADSM
       77  SQLU-LOAD-ADSM-ERROR      PIC S9(4) COMP-5 VALUE -2037.
      * media header not present
       77  SQLUD-NO-MHEADER-ERROR    PIC S9(4) COMP-5 VALUE -2531.
      * media header missing
       77  SQLUD-NO-MHEADER-WARNING  PIC S9(4) COMP-5 VALUE 2534.
      * another tape mount required
       77  SQLUD-NEXT-TAPE-WARNING   PIC S9(4) COMP-5 VALUE 2535.
      * waiting for TSM server to access data on removable media
       77  SQLUD-TSM-MOUNT-WAIT      PIC S9(4) COMP-5 VALUE 2545.
      * waiting for ADSM server to access data on removable media
       77  SQLUD-ADSM-MOUNT-WAIT     PIC S9(4) COMP-5 VALUE 2545.

      *************************************************************************
      * Configuration parameter obsolete return codes defines -  
      * Some configuration parameters had specific out of range return
      * codes; these have been replaced by generic out of range messages
      * In these cases the old token names for the specific return codes
      * are given, but the values are replaced by the new values returned 
      * when out of range.
      **************************************************************************

      * only SYSADM can changedatabase configuration file
       77  SQLF-RC-DBAUTH            PIC S9(4) COMP-5 VALUE -5002.
      * invalid # of concurrent db
       77  SQLF-RC-INVNDB            PIC S9(4) COMP-5 VALUE -5130.
      * invalid req I/O blk size
       77  SQLF-RC-INVRIO            PIC S9(4) COMP-5 VALUE -5130.
      * invalid serv I/O blk size
       77  SQLF-RC-INVSIO            PIC S9(4) COMP-5 VALUE -5015.
      * invalid communications heap
       77  SQLF-RC-INVCHEAP          PIC S9(4) COMP-5 VALUE -5016.
      * invalid remote services heap
       77  SQLF-RC-INVRSHEAP         PIC S9(4) COMP-5 VALUE -5017.
      * invalid sort heap threshold
       77  SQLF-RC-INVSHPTHR         PIC S9(4) COMP-5 VALUE -5130.
      * invalid DB2COMM value
       77  SQLCC-RC-BAD-DB2COMM      PIC S9(4) COMP-5 VALUE -5036.
      * service name not definein db2 config file
       77  SQLCC-RC-NO-SERV-IN-DBMCFG PIC S9(4) COMP-5 VALUE -5037.
      * service name not found in/etc/services file
       77  SQLCC-RC-SERV-NOT-FOUND   PIC S9(4) COMP-5 VALUE -5038.
      * interrupt port not found in/etc/services file
       77  SQLCC-RC-INT-PORT-NOT-FOUND PIC S9(4) COMP-5 VALUE -5039.
      * trans program name not definedin db2 configuration file
       77  SQLCC-RC-NO-TPN-IN-DBMCFG PIC S9(4) COMP-5 VALUE -5041.
      * invalid # of locklist
       77  SQLF-RC-INVNLL            PIC S9(4) COMP-5 VALUE -5130.
      * invalid # bufr pool pages
       77  SQLF-RC-INVNBP            PIC S9(4) COMP-5 VALUE -5130.
      * invalid # of DB files open
       77  SQLF-RC-INVNDBF           PIC S9(4) COMP-5 VALUE -5130.
      * invalid soft check point value
       77  SQLF-RC-INVSCP-DB2OS2     PIC S9(4) COMP-5 VALUE -5130.
      * invalid soft check point value
       77  SQLF-RC-INVSCP-DB2AIX     PIC S9(4) COMP-5 VALUE -5130.
      * invalid soft check point value
       77  SQLF-RC-INVSCP            PIC S9(4) COMP-5 VALUE -5130.
      * invalid # of active appls
       77  SQLF-RC-INVNAP            PIC S9(4) COMP-5 VALUE -5130.
      * invalid application heapsz
       77  SQLF-RC-INVAHP            PIC S9(4) COMP-5 VALUE -5130.
      * invalid database heap size
       77  SQLF-RC-INVDHP            PIC S9(4) COMP-5 VALUE -5130.
      * invalid deadlock detection
       77  SQLF-RC-INVDLT            PIC S9(4) COMP-5 VALUE -5130.
      * invalid # of total files openper application
       77  SQLF-RC-INVTAF            PIC S9(4) COMP-5 VALUE -5130.
      * invalid sortlist heap
       77  SQLF-RC-INVSHP            PIC S9(4) COMP-5 VALUE -5130.
      * invalid maxlocks per application
       77  SQLF-RC-INVMAL            PIC S9(4) COMP-5 VALUE -5130.
      * invalid statement heap
       77  SQLF-RC-INVSTMHP          PIC S9(4) COMP-5 VALUE -5130.
      * invalid number primary log files
       77  SQLF-RC-INVLOGPRIM        PIC S9(4) COMP-5 VALUE -5130.
      * invalid number of secondary log files
       77  SQLF-RC-INVLOG2ND         PIC S9(4) COMP-5 VALUE -5130.
      * invalid log file size
       77  SQLF-RC-INVLOGFSZ         PIC S9(4) COMP-5 VALUE -5130.
      * incompatible file open parmeter
       77  SQLF-RC-INVDB2            PIC S9(4) COMP-5 VALUE -5102.
      * no DB's / requestor only
       77  SQLF-RC-INVK2             PIC S9(4) COMP-5 VALUE -5104.
      * standalone nodetype does notsupport nodename
       77  SQLF-RC-INVK3             PIC S9(4) COMP-5 VALUE -5126.
      * remote workstation has alreadybeen configured
       77  SQLF-RC-RWS-EXIST         PIC S9(4) COMP-5 VALUE -5106.
      * <authid> does not haveauthority to add or drop a remote workstation
       77  SQLF-RC-RWS-SYSADM        PIC S9(4) COMP-5 VALUE -5107.
      * remote workstation has notbeen previously setup using sqlarws
       77  SQLF-RC-RWS-NOT-EXIST     PIC S9(4) COMP-5 VALUE -5108.
      * machine name is missing ors too long.
       77  SQLF-RC-RWS-MACHINENAME   PIC S9(4) COMP-5 VALUE -5109.
      * configuration option is invalid
       77  SQLF-RC-RWS-INV-OPT       PIC S9(4) COMP-5 VALUE -5110.
      * environment variableDB2WKSTPROF is not defined
       77  SQLF-RC-ENV-VAR-NOTDEF    PIC S9(4) COMP-5 VALUE -5111.
      * incompatible buffer pool and maximum # of applications
       77  SQLF-RC-INVDB3            PIC S9(4) COMP-5 VALUE -5146.
      * invalid QUERY_HEAP_SZ
       77  SQLF-RC-INV-QUERY-HEAP-SZ PIC S9(4) COMP-5 VALUE -5143.
      * out of range - limited by a parm
       77  SQLF-RC-INV-RANGE-3       PIC S9(4) COMP-5 VALUE -5144.
      * out of range - maximum limited by an expression
       77  SQLF-RC-INV-RANGE-MAX-EXPR PIC S9(4) COMP-5 VALUE -5144.
      * out of range - maximum limited by an expression (range includes -1)
       77  SQLF-RC-INV-RANGE-MAX-EXPR-2 PIC S9(4) COMP-5 VALUE -5145.
      * out of range - minimum limited by an expression
       77  SQLF-RC-INV-RANGE-MIN-EXPR PIC S9(4) COMP-5 VALUE -5146.
      * out of range - minimum limited by an expression (range includes -1)
       77  SQLF-RC-INV-RANGE-MIN-EXPR-2 PIC S9(4) COMP-5 VALUE -5147.
      * not current db2 configuration
       77  SQLF-RC-KCON              PIC S9(4) COMP-5 VALUE -5025.
      * invalid initial log size
       77  SQLF-RC-INVILF            PIC S9(4) COMP-5 VALUE -5083.
      * invalid logfile extention
       77  SQLF-RC-INVLFE            PIC S9(4) COMP-5 VALUE -5091.
      * invalid # of log extention
       77  SQLF-RC-INVNLE            PIC S9(4) COMP-5 VALUE -5092.
      * incompatible logfile parameter
       77  SQLF-RC-INVDB1            PIC S9(4) COMP-5 VALUE -5101.
      * both R1 & R3 Log parameters may not be modified
       77  SQLF-RC-LF-1-3            PIC S9(4) COMP-5 VALUE -5120.
      * maxappls*maxlocks too low
       77  SQLF-RC-LOW-APPLS-AND-LOCKS PIC S9(4) COMP-5 VALUE -5135.
      * To many items on update or get
       77  SQLF-RC-MAX-ITEMS-EXCEEDED PIC S9(4) COMP-5 VALUE -5139.
      * invalid AVG_APPLS
       77  SQLF-RC-INV-AVG-APPLS     PIC S9(4) COMP-5 VALUE -5141.
      * invalid system flag
       77  SQLF-RC-INVSYSIDX         PIC S9(4) COMP-5 VALUE -5021.
      * invalid database flag
       77  SQLF-RC-INVDBIDX          PIC S9(4) COMP-5 VALUE -5022.
      * invalid sysadm_group
       77  SQLF-RC-INVSYSADM         PIC S9(4) COMP-5 VALUE -5028.
      * invalid node type
       77  SQLF-RC-INVNT             PIC S9(4) COMP-5 VALUE -5065.
      * interrupt received
       77  SQLF-RC-CNTBRK            PIC S9(4) COMP-5 VALUE -5075.
      * invalid authentication
       77  SQLF-RC-INV-AUTHENTICATION PIC S9(4) COMP-5 VALUE -5140.
      * invalid trust_allclnts
       77  SQLF-RC-INV-TRUST-ALLCLNTS PIC S9(4) COMP-5 VALUE -5156.
      * db2atld restartcount warning
       77  SQLU-ATLD-RESTARTCOUNT-WARN PIC S9(4) COMP-5 VALUE 6500.
      * db2atld savecount error
       77  SQLU-ATLD-SAVECOUNT-ERROR PIC S9(4) COMP-5 VALUE -6532.
      * db2atld restartcount error
       77  SQLU-ATLD-RESTARTCOUNT-ERROR PIC S9(4) COMP-5 VALUE -6533.
      * db2atld does not need splitting
       77  SQLU-ATLD-SPLIT-NOT-NEEDED PIC S9(4) COMP-5 VALUE -6535.
      * db2atld unable to open pmap
       77  SQLU-PMAP-OPEN-ERR        PIC S9(4) COMP-5 VALUE -6550.
      * db2atld failed to write pmap
       77  SQLU-PMAP-WRITE-ERR       PIC S9(4) COMP-5 VALUE -6551.
      * db2atld failed to open temp file
       77  SQLU-TEMPCFG-OPEN-ERR     PIC S9(4) COMP-5 VALUE -6552.
      * db2atld failed to write to tmp file
       77  SQLU-TEMPCFG-WRITE-ERR    PIC S9(4) COMP-5 VALUE -6553.
      * db2atld failed to spawn remote child
       77  SQLU-REXEC-ERR            PIC S9(4) COMP-5 VALUE -6554.
      * db2atld comm error
       77  SQLU-ATLD-COMM-ERR        PIC S9(4) COMP-5 VALUE -6555.
      * db2atld partial record found
       77  SQLU-ATLD-PARTIAL-REC     PIC S9(4) COMP-5 VALUE 6556.
      * db2atld failed to get default node
       77  SQLU-GET-DEFAULT-NODE-ERR PIC S9(4) COMP-5 VALUE -6557.
      * db2atld unable to get cur working dir
       77  SQLU-GET-CURDIR-ERR       PIC S9(4) COMP-5 VALUE -6558.
      * db2atld incorrect usage
       77  SQLU-ATLD-BAD-CLP-ERR     PIC S9(4) COMP-5 VALUE -6559.
      * db2atld split node incorrect
       77  SQLU-BAD-SPLIT-NODE       PIC S9(4) COMP-5 VALUE -6560.
      * db2atld invalid load node
       77  SQLU-BAD-LOAD-NODE        PIC S9(4) COMP-5 VALUE -6561.
      * db2atld failed to get instance
       77  SQLU-GET-INSTANCE-ERR     PIC S9(4) COMP-5 VALUE -6562.
      * db2atld failed to get cur UID
       77  SQLU-GET-UID-ERR          PIC S9(4) COMP-5 VALUE -6563.
      * db2atld invalid password
       77  SQLU-BAD-PASSWORD         PIC S9(4) COMP-5 VALUE -6564.
      * db2atld help msg
       77  SQLU-ATLD-HELP            PIC S9(4) COMP-5 VALUE 6565.
      * db2atld load command not specified
       77  SQLU-MISSING-LOAD-COMMAND PIC S9(4) COMP-5 VALUE -6566.
      * db2atld option specified twice
       77  SQLU-ATLD-DUP-OPTION      PIC S9(4) COMP-5 VALUE -6567.
      * db2atld starting all the load jobs
       77  SQLU-STARTING-ALL-LOADS   PIC S9(4) COMP-5 VALUE 6568.
      * db2atld starting all the splitters
       77  SQLU-STARTING-ALL-SPLITTERS PIC S9(4) COMP-5 VALUE 6569.
      * db2atld waiting for splitters to finish
       77  SQLU-WAITING-FOR-SPLITTERS PIC S9(4) COMP-5 VALUE 6570.
      * db2atld waiting for load to complete
       77  SQLU-WAITING-FOR-LOADS    PIC S9(4) COMP-5 VALUE 6571.
      * db2atld load has started
       77  SQLU-LOAD-STARTED         PIC S9(4) COMP-5 VALUE 6572.
      * db2atld splitters has finished
       77  SQLU-SPLIT-RESULT         PIC S9(4) COMP-5 VALUE 6573.
      * db2atld Bytes read thus far
       77  SQLU-DATA-READ-STATUS     PIC S9(4) COMP-5 VALUE 6574.
      * db2atld total size of data read
       77  SQLU-TOTAL-DATA-READ      PIC S9(4) COMP-5 VALUE 6575.
      * db2atld threading error
       77  SQLU-THREAD-ERR           PIC S9(4) COMP-5 VALUE -6576.
      * rowcount not supported in this mode
       77  SQLU-ATLD-ROWCOUNT-ERROR  PIC S9(9) COMP-5 VALUE -27961.
      * too many splitters
       77  SQLU-ATLD-TOO-MANY-SPLITTERS PIC S9(9) COMP-5 VALUE -27991.
      * invalid DB2_LOAD_COPY_NO_OVERRIDE reg variable
       77  SQLU-COPYNO-OVERRIDE-INVALID PIC S9(9) COMP-5 VALUE -27965.
      * COPY NO was overridden warning
       77  SQLU-COPYNO-OVERRIDE-WARNING PIC S9(9) COMP-5 VALUE 27966.
      * COPY NO was overriden with default (nonrecov)
       77  SQLU-COPYNO-OVERRIDE-DFLTWARN PIC S9(9) COMP-5 VALUE 27967.
      * Special Register Default value truncated.
       77  SQLU-SPEC-REGSTR-TRUNC-WARN PIC S9(9) COMP-5 VALUE 27994.

      * Flush Table API Return Codes
      * Table not at this node
       77  SQLUF-TABLE-NOT-AT-NODE   PIC S9(4) COMP-5 VALUE -6024.

      * sqlugrpi, sqlugrpn, and sqlugtpi return codes
      * Invalid authid
       77  SQLUG-INVALID-AUTHID      PIC S9(4) COMP-5 VALUE -2300.
      * invalid table
       77  SQLUG-INVALID-TABLE-NAME  PIC S9(4) COMP-5 VALUE -2301.
      * Interrupt
       77  SQLUG-CBI                 PIC S9(4) COMP-5 VALUE -3005.
      * Table not exist
       77  SQLUG-TABLE-NOT-FOUND     PIC S9(4) COMP-5 VALUE -2211.
      * Insufficient authorization
       77  SQLUG-RC-INSAUTH          PIC S9(4) COMP-5 VALUE -6023.
      * No partitioning key
       77  SQLUG-NULL-PARTKEY        PIC S9(4) COMP-5 VALUE -6038.
      * Nulls not allowed
       77  SQLUG-NULL-NOTALLOWED     PIC S9(4) COMP-5 VALUE -6039.
      * decimal format conflict
       77  SQLUG-DECIMAL-FORMAT-CONFLICT PIC S9(4) COMP-5 VALUE -2755.
      * Invalid syntax
       77  SQLUG-INVALID-SYNTAX      PIC S9(4) COMP-5 VALUE -6044.
      * Invalid datatype
       77  SQLUG-INVALID-DATATYPE    PIC S9(4) COMP-5 VALUE -6045.
      * Invalid dynamic query mgmt flag
       77  SQLF-RC-INVALID-DYNQUERYMGMT PIC S9(9) COMP-5 VALUE -29000.

      * Load error codes continued
      * Load restart/terminate is not necessary
       77  SQLU-INV-RESTART-TERMINATE PIC S9(9) COMP-5 VALUE -27902.
      * Invalid input parameter for partitioned database load
       77  SQLU-PARTLOAD-BAD-PARAMETER PIC S9(9) COMP-5 VALUE -27959.
      * Invalid PART_FILE_LOCATION specified for source type CURSOR
       77  SQLU-PARTLOAD-PART-FILE-CURSOR PIC S9(9) COMP-5 VALUE -27960.
      * Invalid rowcount specified for partitioned database load
       77  SQLU-PARTLOAD-BAD-ROWCOUNT PIC S9(9) COMP-5 VALUE -27961.
      * Load partition violations detected
       77  SQLU-PARTITION-VIOLATIONS PIC S9(9) COMP-5 VALUE 27990.
      * Partitioning map was found, but load mode is not load_only
       77  SQLU-PARTITIONING-MAP-FOUND PIC S9(9) COMP-5 VALUE -27992.

      * Utility control error codes
      * Invalid priority
       77  SQLUTH-INVALID-PRIORITY   PIC S9(4) COMP-5 VALUE -1152.
      * Utility not found
       77  SQLUTH-UTILITY-NOT-FOUND  PIC S9(4) COMP-5 VALUE -1153.
      * Utility does not support throttling
       77  SQLUTH-NO-THROTTLE-SUPPORT PIC S9(4) COMP-5 VALUE -1154.

      * obsolete database manager configuration parameter tokens
       77  SQLF-KTN-SVRIOBLK         PIC S9(4) COMP-5 VALUE 3.
       77  SQLF-KTN-SQLENSEG         PIC S9(4) COMP-5 VALUE 5.
       77  SQLF-KTN-COMHEAPSZ        PIC S9(4) COMP-5 VALUE 8.
       77  SQLF-KTN-RSHEAPSZ         PIC S9(4) COMP-5 VALUE 9.
       77  SQLF-KTN-NUMRC            PIC S9(4) COMP-5 VALUE 10.
       77  SQLF-KTN-CUINTERVAL       PIC S9(4) COMP-5 VALUE 14.
       77  SQLF-KTN-SQLSTMTSZ        PIC S9(4) COMP-5 VALUE 17.
       77  SQLF-KTN-COMHEAPSZ-P      PIC S9(4) COMP-5 VALUE 50.
       77  SQLF-KTN-RSHEAPSZ-P       PIC S9(4) COMP-5 VALUE 51.
       77  SQLF-KTN-IPX-FILESERVER   PIC S9(4) COMP-5 VALUE 47.
       77  SQLF-KTN-IPX-OBJECTNAME   PIC S9(4) COMP-5 VALUE 48.
       77  SQLF-KTN-ADSM-PASSWORD    PIC S9(4) COMP-5 VALUE 93.
       77  SQLF-KTN-ADSM-NODENAME    PIC S9(4) COMP-5 VALUE 94.
       77  SQLF-KTN-ADSM-OWNER       PIC S9(4) COMP-5 VALUE 95.
       77  SQLF-KTN-MAX-IDLEAGENTS   PIC S9(4) COMP-5 VALUE 60.

      * obsolete database configuration parameter tokens
       77  SQLF-DBTN-LOGFILE         PIC S9(4) COMP-5 VALUE 4.
       77  SQLF-DBTN-MAXTOTFILOP     PIC S9(4) COMP-5 VALUE 10.
       77  SQLF-DBTN-LOGEXT          PIC S9(4) COMP-5 VALUE 12.
       77  SQLF-DBTN-LOGMAXEXT       PIC S9(4) COMP-5 VALUE 13.
       77  SQLF-DBTN-AGENTHEAP       PIC S9(4) COMP-5 VALUE 14.
       77  SQLF-DBTN-SEGPAGES        PIC S9(4) COMP-5 VALUE 123.
       77  SQLF-DBTN-BUFFPAGE        PIC S9(4) COMP-5 VALUE 2.
       77  SQLF-DBTN-SORTHEAP        PIC S9(4) COMP-5 VALUE 91.
       77  SQLF-DBTN-SORTHEAPSZ-P    PIC S9(4) COMP-5 VALUE 52.
       77  SQLF-DBTN-DBHEAP          PIC S9(4) COMP-5 VALUE 50.
       77  SQLF-DBTN-DBHEAP-P        PIC S9(4) COMP-5 VALUE 50.
       77  SQLF-DBTN-APPLHEAPSZ-P    PIC S9(4) COMP-5 VALUE 51.
       77  SQLF-DBTN-STMTHEAPSZ-P    PIC S9(4) COMP-5 VALUE 53.
       77  SQLF-DBTN-LOGFILSIZ       PIC S9(4) COMP-5 VALUE 18.
       77  SQLF-DBTN-MULTIPGAL       PIC S9(4) COMP-5 VALUE 504.
       77  SQLF-DBTN-ADSM-PASSWD     PIC S9(4) COMP-5 VALUE 501.
       77  SQLF-DBTN-NEXTACTIVE      PIC S9(4) COMP-5 VALUE 107.
       77  SQLF-DBTN-DL-NUM-BACKUP   PIC S9(4) COMP-5 VALUE 352.
       77  SQLF-DBTN-DETS            PIC S9(4) COMP-5 VALUE 21.
       77  SQLF-DBTN-INTFLAGS        PIC S9(4) COMP-5 VALUE 104.
       77  SQLF-DBTN-ADSM-PASSWORD   PIC S9(4) COMP-5 VALUE 501.
       77  SQLF-DBTN-ADSM-OWNER      PIC S9(4) COMP-5 VALUE 305.
       77  SQLF-DBTN-ADSM-NODENAME   PIC S9(4) COMP-5 VALUE 306.
       77  SQLF-DBTN-ADSM-MGMTCLASS  PIC S9(4) COMP-5 VALUE 307.
       77  SQLF-DBTN-LOCKLIST        PIC S9(4) COMP-5 VALUE 1.


      * obsolete config parm assorted defines
      * Default maxcagents
       77  SQLFDMCA                  PIC S9(4) COMP-5 VALUE -1.
      * Default maxdari
       77  SQLFDMXD                  PIC S9(4) COMP-5 VALUE -1.
      * Default agentpri
       77  SQLFDAPR                  PIC S9(4) COMP-5 VALUE -1.
      * keepdari - no
       77  SQLFMKPD                  PIC S9(4) COMP-5 VALUE 0.
      * keepdari - yes
       77  SQLFXKPD                  PIC S9(4) COMP-5 VALUE 1.

      * obsolete defines for SQLF_DBTN_DETS
      * xxxx xxxx xxxx xxx0
       77  SQLF-COPY-NOPROT          PIC S9(4) COMP-5 VALUE 0.
      * xxxx xxxx xxxx xxx1
       77  SQLF-COPY-PROTECT         PIC S9(4) COMP-5 VALUE 1.
      * xxxx xxxx xxxx xx1x
       77  SQLF-ENABLE-LOG-RETAIN    PIC S9(4) COMP-5 VALUE 2.
      * xxxx xxxx xxxx x1xx
       77  SQLF-ENABLE-USER-EXIT     PIC S9(4) COMP-5 VALUE 4.
      * xxxx xxxx xxxx 1xxx
       77  SQLF-ENABLE-AUTO-RESTART  PIC S9(4) COMP-5 VALUE 8.
      * xxxx xxxx xxx1 xxxx
       77  SQLF-ENABLE-LOGRETAIN-CAPTURE PIC S9(4) COMP-5 VALUE 16.
      * xxxx xxxx xx1x xxxx
       77  SQLF-ENABLE-TRACKMOD      PIC S9(4) COMP-5 VALUE 32.

      * obsolete defines for SQLF_DBTN_INTFLAGS
      * xxxx xxxx xxxx xxx1
       77  SQLF-CONSISTENT           PIC S9(4) COMP-5 VALUE 1.
      * xxxx xxxx xxxx x1xx
       77  SQLF-BACKUP-PENDING       PIC S9(4) COMP-5 VALUE 4.
      * xxxx xxxx xxxx 1xxx
       77  SQLF-ROLLFWD-PENDING      PIC S9(4) COMP-5 VALUE 8.
      * xxxx xxxx xxx1 xxxx
       77  SQLF-LOG-RETAIN           PIC S9(4) COMP-5 VALUE 16.
      * xxxx xxxx xx1x xxxx
       77  SQLF-USER-EXIT            PIC S9(4) COMP-5 VALUE 32.
      * xxxx xxxx x1xx xxxx
       77  SQLF-TBS-ROLLFWD          PIC S9(4) COMP-5 VALUE 64.
      * xxxx xxxx 1xxx xxxx
       77  SQLF-RESTORE-PENDING      PIC S9(4) COMP-5 VALUE 128.

      * Obsolete media types
      * ADSM
       77  SQLU-ADSM-MEDIA           PIC X(1) value "A".


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

       77  SQLB-TBSQRY-DATA-SIZE     PIC S9(4) COMP-5 VALUE 176.
      * No action at all
       77  DB2-DB-SUSPEND-NONE       PIC S9(4) COMP-5 VALUE 0.
      * Set write suspend for database
       77  DB2-DB-SUSPEND-WRITE      PIC S9(4) COMP-5 VALUE 1.
      * Set write resume for database
       77  DB2-DB-RESUME-WRITE       PIC S9(4) COMP-5 VALUE 2.
      * Set write suspend for tablespaces
       77  DB2-TS-SUSPEND-WRITE      PIC S9(4) COMP-5 VALUE 10.
      * Set write resume for tablespaces
       77  DB2-TS-RESUME-WRITE       PIC S9(4) COMP-5 VALUE 20.
      * invalid into in tcolstrg
       77  SQL-SLQUI-TINTO           PIC S9(4) COMP-5 VALUE -3034.
      *************************************************************************
      * 
      * End of obsolete functions and symbols
      * 
      **************************************************************************
