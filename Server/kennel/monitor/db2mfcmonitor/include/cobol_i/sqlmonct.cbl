      *******************************************************************************
      *
      * Source File Name = SQLMONCT.CBL
      *
      * (C) COPYRIGHT International Business Machines Corp. 1993, 1997
      * All Rights Reserved
      * Licensed Materials - Property of IBM
      *
      * US Government Users Restricted Rights - Use, duplication or
      * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
      *
      * Function = Include File defining:
      *              Database System Monitor - Constants
      *              Database System Monitor - Structures
      *              Database System Monitor - Function Prototypes
      *
      * Operating System: Common COBOL Include File
      *
      * Notes:
      *   structures returned as output to the 'Get Snapshot' function, 'sqlmonss()'
      *   are of the form: 'sqlm_xxxx', for example: sqlm_dbase.
      *
      *   sqlm_xxxx_event structures are logged to a trace by DB2 Event Monitors,
      *   which can be defined and activated using SQL commands (CREATE EVENT
      *   MONITOR, SET EVENT MONITOR).
      *
      ******************************************************************************

      *****************************************************************************
      * Database System Monitor Version (version, server_version)                  *
      *****************************************************************************
       77 SQLM-DBMON-VERSION1
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-DBMON-VERSION2
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-DBMON-VERSION5
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-DBMON-VERSION5-2
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLM-DBMON-VERSION6
           PIC S9(4) COMP-5 VALUE 5.

       77 SQLM-DBMON-VERSION7
           PIC S9(4) COMP-5 VALUE 6.

       77 SQLM-DBMON-VERSION8
           PIC S9(4) COMP-5 VALUE 7.

       77 SQLM-CURRENT-VERSION
           PIC S9(4) COMP-5 VALUE 7.


      *****************************************************************************
      * Snapshot class qualifiers (This indicates how the sqlma is to be           *
      * interpreted)                                                               *
      *****************************************************************************

       77 SQLM-CLASS-DEFAULT
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-CLASS-HEALTH
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-CLASS-HEALTH-WITH-DETAIL
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-CLASS-MAX
           PIC S9(4) COMP-5 VALUE 2.


      *****************************************************************************
      * Snapshot requests (sqlm_obj_struct.obj_type in sqlma)                      *
      *****************************************************************************

      * Database Manager snapshot *
       77 SQLMA-DB2
           PIC S9(4) COMP-5 VALUE 1.


      * List of connections *
       77 SQLMA-DBASE-APPLINFO
           PIC S9(4) COMP-5 VALUE 7.

       77 SQLMA-APPLINFO-ALL
           PIC S9(4) COMP-5 VALUE 11.

       77 SQLMA-DCS-APPLINFO-ALL
           PIC S9(4) COMP-5 VALUE 12.


      * Database snapshot *
       77 SQLMA-DBASE
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLMA-DBASE-ALL
           PIC S9(4) COMP-5 VALUE 9.

       77 SQLMA-DCS-DBASE
           PIC S9(4) COMP-5 VALUE 34.

       77 SQLMA-DCS-DBASE-ALL
           PIC S9(4) COMP-5 VALUE 35.

       77 SQLMA-DBASE-REMOTE
           PIC S9(4) COMP-5 VALUE 37.

       77 SQLMA-DBASE-REMOTE-ALL
           PIC S9(4) COMP-5 VALUE 38.


      * Application snapshot *
       77 SQLMA-APPL-ALL
           PIC S9(4) COMP-5 VALUE 10.

       77 SQLMA-APPL
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLMA-AGENT-ID
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLMA-DBASE-APPLS
           PIC S9(4) COMP-5 VALUE 6.

       77 SQLMA-DCS-APPL-ALL
           PIC S9(4) COMP-5 VALUE 30.

       77 SQLMA-DCS-APPL
           PIC S9(4) COMP-5 VALUE 31.

       77 SQLMA-DCS-APPL-HANDLE
           PIC S9(4) COMP-5 VALUE 32.

       77 SQLMA-DCS-DBASE-APPLS
           PIC S9(4) COMP-5 VALUE 33.

       77 SQLMA-DBASE-APPLS-REMOTE
           PIC S9(4) COMP-5 VALUE 39.

       77 SQLMA-APPL-REMOTE-ALL
           PIC S9(4) COMP-5 VALUE 40.


      * Table snapshot *
       77 SQLMA-DBASE-TABLES
           PIC S9(4) COMP-5 VALUE 5.


      * Tablespace snapshot *
       77 SQLMA-DBASE-TABLESPACES
           PIC S9(4) COMP-5 VALUE 13.


      * Snapshot for locks *
       77 SQLMA-APPL-LOCKS
           PIC S9(4) COMP-5 VALUE 20.

       77 SQLMA-APPL-LOCKS-AGENT-ID
           PIC S9(4) COMP-5 VALUE 21.

       77 SQLMA-DBASE-LOCKS
           PIC S9(4) COMP-5 VALUE 8.


      * Bufferpool snapshot *
       77 SQLMA-DBASE-BUFFERPOOLS
           PIC S9(4) COMP-5 VALUE 22.

       77 SQLMA-BUFFERPOOLS-ALL
           PIC S9(4) COMP-5 VALUE 23.


      * Dynamic sql snapshot *
       77 SQLMA-DYNAMIC-SQL
           PIC S9(4) COMP-5 VALUE 36.


      *****************************************************************************
      * Snapshot output structure type identifiers                                 *
      *                                                                            *
      * (info_type element in the sqlm_xxx structures that are returned in the     *
      * sqlmonss() output buffer.)                                                 *
      *****************************************************************************

       77 SQLM-DB2-SS
           PIC X(1) VALUE "1" USAGE DISPLAY NATIVE.

       77 SQLM-FCM-SS
           PIC X(1) VALUE "E" USAGE DISPLAY NATIVE.

       77 SQLM-NODE-SS
           PIC X(1) VALUE "G" USAGE DISPLAY NATIVE.

       77 SQLM-APPLINFO-SS
           PIC X(1) VALUE "4" USAGE DISPLAY NATIVE.

       77 SQLM-DCS-APPLINFO-SS
           PIC X(1) VALUE "A" USAGE DISPLAY NATIVE.

       77 SQLM-DBASE-SS
           PIC X(1) VALUE "2" USAGE DISPLAY NATIVE.

       77 SQLM-TABLE-HEADER-SS
           PIC X(1) VALUE "5" USAGE DISPLAY NATIVE.

       77 SQLM-TABLE-SS
           PIC X(1) VALUE "6" USAGE DISPLAY NATIVE.

       77 SQLM-DBASE-LOCK-SS
           PIC X(1) VALUE "7" USAGE DISPLAY NATIVE.

       77 SQLM-APPL-LOCK-SS
           PIC X(1) VALUE "8" USAGE DISPLAY NATIVE.

       77 SQLM-LOCK-SS
           PIC X(1) VALUE "9" USAGE DISPLAY NATIVE.

       77 SQLM-TABLESPACE-HEADER-SS
           PIC X(1) VALUE "B" USAGE DISPLAY NATIVE.

       77 SQLM-TABLESPACE-SS
           PIC X(1) VALUE "C" USAGE DISPLAY NATIVE.

       77 SQLM-APPL-SS
           PIC X(1) VALUE "3" USAGE DISPLAY NATIVE.

       77 SQLM-DBASE-ROLLFWD-SS
           PIC X(1) VALUE "H" USAGE DISPLAY NATIVE.

       77 SQLM-DBASE-ROLLFWD-TS-SS
           PIC X(1) VALUE "I" USAGE DISPLAY NATIVE.

       77 SQLM-BUFFERPOOL-SS
           PIC X(1) VALUE "J" USAGE DISPLAY NATIVE.

       77 SQLM-LOCK-WAIT-SS
           PIC X(1) VALUE "K" USAGE DISPLAY NATIVE.

       77 SQLM-STMT-SS
           PIC X(1) VALUE "L" USAGE DISPLAY NATIVE.

       77 SQLM-SUBSECTION-SS
           PIC X(1) VALUE "M" USAGE DISPLAY NATIVE.

       77 SQLM-AGENT-SS
           PIC X(1) VALUE "N" USAGE DISPLAY NATIVE.

       77 SQLM-DCS-APPL-SS
           PIC X(1) VALUE "O" USAGE DISPLAY NATIVE.

       77 SQLM-DCS-APPLID-SS
           PIC X(1) VALUE "P" USAGE DISPLAY NATIVE.

       77 SQLM-DCS-APPL-STATS-SS
           PIC X(1) VALUE "Q" USAGE DISPLAY NATIVE.

       77 SQLM-APPL-XID-SS
           PIC X(1) VALUE "R" USAGE DISPLAY NATIVE.

       77 SQLM-TPMON-INFO-SS
           PIC X(1) VALUE "S" USAGE DISPLAY NATIVE.

       77 SQLM-DCS-STMT-SS
           PIC X(1) VALUE "T" USAGE DISPLAY NATIVE.

       77 SQLM-DCS-DBASE-SS
           PIC X(1) VALUE "U" USAGE DISPLAY NATIVE.

       77 SQLM-COLLECTED-SS
           PIC X(1) VALUE "V" USAGE DISPLAY NATIVE.

       77 SQLM-DBASE-REMOTE-SS
           PIC X(1) VALUE "W" USAGE DISPLAY NATIVE.

       77 SQLM-APPL-REMOTE-SS
           PIC X(1) VALUE "X" USAGE DISPLAY NATIVE.


      *****************************************************************************
      * stream type ids                                                            *
      *****************************************************************************
       77 SQLM-STREAM-SNAPSHOT
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-STREAM-EVMON
           PIC S9(4) COMP-5 VALUE 2.


      *****************************************************************************
      * size constants                                                             *
      *****************************************************************************

       77 SQLM-APPLID-SZ
           PIC S9(4) COMP-5 VALUE 32.

       77 SQLM-SEQ-SZ
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLM-SH-IDENT-SZ
           PIC S9(4) COMP-5 VALUE 8.

       77 SQLM-IDENT-SZ
           PIC S9(4) COMP-5 VALUE 20.

       77 SQLM-LG-IDENT-SZ
           PIC S9(4) COMP-5 VALUE 32.

       77 SQLM-OBJECT-SZ
           PIC S9(4) COMP-5 VALUE 36.

       77 SQLM-DBPATH-SZ
           PIC S9(4) COMP-5 VALUE 256.

       77 SQLM-COMM-ADDR-SZ
           PIC S9(4) COMP-5 VALUE 32.

       77 SQLM-SERVERNAME-SZ
           PIC S9(4) COMP-5 VALUE 128.

       77 SQLM-MESSAGE-SZ
           PIC S9(4) COMP-5 VALUE 128.

       77 SQLM-STORAGE-PATH-SZ
           PIC S9(4) COMP-5 VALUE 190.


      *****************************************************************************
      * Information group state switches (sqlmon and sqlmonss functions)           *
      *****************************************************************************

       77 SQLM-OFF
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-ON
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-HOLD
           PIC S9(4) COMP-5 VALUE 2.


      *****************************************************************************
      * flag constant for sqlmrset()                                               *
      *****************************************************************************
       77 SQLM-UDB-DBASE
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-UDB-ALL
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-DCS-DBASE
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-DCS-ALL
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-APM
           PIC S9(4) COMP-5 VALUE 4.


      *****************************************************************************
      * Monitor switches                                                           *
      *****************************************************************************

       77 SQLM-UOW-SW
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-STATEMENT-SW
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-TABLE-SW
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-BUFFER-POOL-SW
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLM-LOCK-SW
           PIC S9(4) COMP-5 VALUE 5.

       77 SQLM-SORT-SW
           PIC S9(4) COMP-5 VALUE 6.

       77 SQLM-TIMESTAMP-SW
           PIC S9(4) COMP-5 VALUE 7.


       77 SQLM-NUM-GROUPS
           PIC S9(4) COMP-5 VALUE 7.

       77 SQLM-NUM-GROUPS-PREV8
           PIC S9(4) COMP-5 VALUE 6.


      *****************************************************************************
      * Statement Types (stmt_type)                                                *
      *****************************************************************************
       77 SQLM-STATIC
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-DYNAMIC
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-NON-STMT
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-STMT-TYPE-UNKNOWN
           PIC S9(4) COMP-5 VALUE 4.


      *****************************************************************************
      * Statement Operation Types (stmt_operation)                                 *
      *****************************************************************************
      * SQL operations *
       77 SQLM-PREPARE
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-EXECUTE
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-EXECUTE-IMMEDIATE
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-OPEN
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLM-FETCH
           PIC S9(4) COMP-5 VALUE 5.

       77 SQLM-CLOSE
           PIC S9(4) COMP-5 VALUE 6.

       77 SQLM-DESCRIBE
           PIC S9(4) COMP-5 VALUE 7.

       77 SQLM-STATIC-COMMIT
           PIC S9(4) COMP-5 VALUE 8.

       77 SQLM-STATIC-ROLLBACK
           PIC S9(4) COMP-5 VALUE 9.

       77 SQLM-FREE-LOCATOR
           PIC S9(4) COMP-5 VALUE 10.

       77 SQLM-PREP-COMMIT
           PIC S9(4) COMP-5 VALUE 11.

       77 SQLM-CALL
           PIC S9(4) COMP-5 VALUE 12.

       77 SQLM-SELECT
           PIC S9(4) COMP-5 VALUE 15.

       77 SQLM-PREP-OPEN
           PIC S9(4) COMP-5 VALUE 16.

       77 SQLM-PREP-EXEC
           PIC S9(4) COMP-5 VALUE 17.

       77 SQLM-COMPILE
           PIC S9(4) COMP-5 VALUE 18.

       77 SQLM-SET
           PIC S9(4) COMP-5 VALUE 19.


      * non-SQL operations *
       77 SQLM-RUNSTATS
           PIC S9(4) COMP-5 VALUE 20.

       77 SQLM-REORG
           PIC S9(4) COMP-5 VALUE 21.

       77 SQLM-REBIND
           PIC S9(4) COMP-5 VALUE 22.

       77 SQLM-REDIST
           PIC S9(4) COMP-5 VALUE 23.

       77 SQLM-GETTA
           PIC S9(4) COMP-5 VALUE 24.

       77 SQLM-GETAA
           PIC S9(4) COMP-5 VALUE 25.


      *****************************************************************************
      * Table Types (table_type)                                                   *
      *****************************************************************************
       77 SQLM-USER-TABLE
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-DROPPED-TABLE
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-TEMP-TABLE
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-CATALOG-TABLE
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLM-REORG-TABLE
           PIC S9(4) COMP-5 VALUE 5.


      *****************************************************************************
      * lock modes (lock_mode)                                                     *
      *****************************************************************************

       77 SQLM-LNON
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-LOIS
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-LOIX
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-LOOS
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-LSIX
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLM-LOOX
           PIC S9(4) COMP-5 VALUE 5.

       77 SQLM-LOIN
           PIC S9(4) COMP-5 VALUE 6.

       77 SQLM-LOOZ
           PIC S9(4) COMP-5 VALUE 7.

       77 SQLM-LOOU
           PIC S9(4) COMP-5 VALUE 8.

       77 SQLM-LONS
           PIC S9(4) COMP-5 VALUE 9.

       77 SQLM-LONX
           PIC S9(4) COMP-5 VALUE 10.

       77 SQLM-LOOW
           PIC S9(4) COMP-5 VALUE 11.

       77 SQLM-LONW
           PIC S9(4) COMP-5 VALUE 12.


      *****************************************************************************
      * subsection status (ss_status)                                              *
      *****************************************************************************

       77 SQLM-SSEXEC
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-SSTQ-WAIT-TO-RCV
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-SSTQ-WAIT-TO-SEND
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-SSCOMPLETED
           PIC S9(4) COMP-5 VALUE 4.


      *****************************************************************************
      * application status (appl_status)                                           *
      *****************************************************************************

       77 SQLM-CONNECTPEND
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-CONNECTED
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-UOWEXEC
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-UOWWAIT
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLM-LOCKWAIT
           PIC S9(4) COMP-5 VALUE 5.

       77 SQLM-COMMIT-ACT
           PIC S9(4) COMP-5 VALUE 6.

       77 SQLM-ROLLBACK-ACT
           PIC S9(4) COMP-5 VALUE 7.

       77 SQLM-RECOMP
           PIC S9(4) COMP-5 VALUE 8.

       77 SQLM-COMP
           PIC S9(4) COMP-5 VALUE 9.

       77 SQLM-INTR
           PIC S9(4) COMP-5 VALUE 10.

       77 SQLM-DISCONNECTPEND
           PIC S9(4) COMP-5 VALUE 11.

       77 SQLM-TPREP
           PIC S9(4) COMP-5 VALUE 12.

       77 SQLM-THCOMT
           PIC S9(4) COMP-5 VALUE 13.

       77 SQLM-THABRT
           PIC S9(4) COMP-5 VALUE 14.

       77 SQLM-TEND
           PIC S9(4) COMP-5 VALUE 15.

       77 SQLM-CREATE-DB
           PIC S9(4) COMP-5 VALUE 16.

       77 SQLM-RESTART
           PIC S9(4) COMP-5 VALUE 17.

       77 SQLM-RESTORE
           PIC S9(4) COMP-5 VALUE 18.

       77 SQLM-BACKUP
           PIC S9(4) COMP-5 VALUE 19.

       77 SQLM-LOAD
           PIC S9(4) COMP-5 VALUE 20.

       77 SQLM-UNLOAD
           PIC S9(4) COMP-5 VALUE 21.

       77 SQLM-IOERROR-WAIT
           PIC S9(4) COMP-5 VALUE 22.

       77 SQLM-QUIESCE-TABLESPACE
           PIC S9(4) COMP-5 VALUE 23.

       77 SQLM-WAITFOR-REMOTE
           PIC S9(4) COMP-5 VALUE 24.

       77 SQLM-REMOTE-RQST
           PIC S9(4) COMP-5 VALUE 25.

       77 SQLM-DECOUPLED
           PIC S9(4) COMP-5 VALUE 26.

       77 SQLM-ROLLBACK-TO-SAVEPOINT
           PIC S9(4) COMP-5 VALUE 27.


      *****************************************************************************
      * DCS application status (dcs_appl_status)                                   *
      *****************************************************************************

       77 SQLM-DCS-CONNECTPEND-OUTBOUND
           PIC S9(4) COMP-5 VALUE 101.

       77 SQLM-DCS-UOWWAIT-OUTBOUND
           PIC S9(4) COMP-5 VALUE 102.

       77 SQLM-DCS-UOWWAIT-INBOUND
           PIC S9(4) COMP-5 VALUE 103.

      * or gateway waiting for request*

      *****************************************************************************
      * DCS transmission chain groupings (num_transmissions_group)                 *
      *****************************************************************************

       77 SQLM-DCS-TRANS-GROUP-2
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-DCS-TRANS-GROUP-3TO7
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-DCS-TRANS-GROUP-8TO15
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-DCS-TRANS-GROUP-16TO64
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLM-DCS-TRANS-GROUP-GT64
           PIC S9(4) COMP-5 VALUE 5.


      *****************************************************************************
      * Concentrator agent statuses                                                *
      *****************************************************************************
       77 SQLM-AGENT-ASSOCIATED
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-AGENT-NOT-ASSOCIATED
           PIC S9(4) COMP-5 VALUE 2.


      *****************************************************************************
      * database status (db_status)                                                *
      *****************************************************************************

       77 SQLM-DB-ACTIVE
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-DB-QUIESCE-PEND
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-DB-QUIESCED
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-DB-ROLLFWD
           PIC S9(4) COMP-5 VALUE 3.


      *****************************************************************************
      * rollforward types (rf_type)                                                *
      *****************************************************************************
       77 SQLM-ROLLFWD-TYP-DB
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-ROLLFWD-TYP-TSPACE
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-NO-ROLLFWD
           PIC S9(4) COMP-5 VALUE 2.


      *****************************************************************************
      * Rollforward status (rf_status)                                             *
      *****************************************************************************
       77 SQLM-ROLLFWD-STATUS-REDO
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-ROLLFWD-STATUS-UNDO
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-ROLLFWD-STATUS-SUCCESS
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-ROLLFWD-STATUS-ERROR
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-ROLLFWD-STATUS-USR-INTR
           PIC S9(4) COMP-5 VALUE 4.


      *****************************************************************************
      * tablespace types                                                           *
      *****************************************************************************
       77 SQLM-TABLESPACE-TYP-DMS
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-TABLESPACE-TYP-SMS
           PIC S9(4) COMP-5 VALUE 1.


      *****************************************************************************
      * tablespace content type                                                    *
      *****************************************************************************
       77 SQLM-TABLESPACE-CONTENT-ANY
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-TABLESPACE-CONTENT-LONG
           PIC S9(4) COMP-5 VALUE 1.

      * The full name of the monitor element is SQLM_TABLESPACE_CONTENT_SYSTEMP
       77 SQLM-TBSP-CONTENT-SYSTEMP
           PIC S9(4) COMP-5 VALUE 2.

      * The full name of the monitor element is SQLM_TABLESPACE_CONTENT_USRTEMP
       77 SQLM-TBSP-CONTENT-USRTEMP
           PIC S9(4) COMP-5 VALUE 3.


      *****************************************************************************
      *  reorg table defines                                                       *
      *****************************************************************************

       77 SQLM-REORG-STARTED
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-REORG-PAUSED
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-REORG-STOPPED
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-REORG-COMPLETED
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLM-REORG-TRUNCATE
           PIC S9(4) COMP-5 VALUE 5.


       77 SQLM-REORG-SORT
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-REORG-BUILD
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-REORG-REPLACE
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-REORG-INDEX-RECREATE
           PIC S9(4) COMP-5 VALUE 4.


       77 SQLM-REORG-SUCCESS
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-REORG-FAIL
           PIC S9(4) COMP-5 VALUE -1.


      *****************************************************************************
      * rebalancer modes                                                           *
      *****************************************************************************
       77 SQLM-TABLESPACE-NO-REBAL
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-TABLESPACE-FWD-REBAL
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-TABLESPACE-REV-REBAL
           PIC S9(4) COMP-5 VALUE 2.


      *****************************************************************************
      * Memory Heaps                                                               *
      *****************************************************************************
       77 SQLM-HEAP-APPLICATION
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-HEAP-DATABASE
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-HEAP-APPL-CONTROL
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-HEAP-LOCK-MGR
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLM-HEAP-UTILITY
           PIC S9(4) COMP-5 VALUE 5.

       77 SQLM-HEAP-STATISTICS
           PIC S9(4) COMP-5 VALUE 6.

       77 SQLM-HEAP-PACKAGE-CACHE
           PIC S9(4) COMP-5 VALUE 7.

       77 SQLM-HEAP-CAT-CACHE
           PIC S9(4) COMP-5 VALUE 8.

       77 SQLM-HEAP-DFM
           PIC S9(4) COMP-5 VALUE 9.

       77 SQLM-HEAP-QUERY
           PIC S9(4) COMP-5 VALUE 10.

       77 SQLM-HEAP-MONITOR
           PIC S9(4) COMP-5 VALUE 11.

       77 SQLM-HEAP-STATEMENT
           PIC S9(4) COMP-5 VALUE 12.

       77 SQLM-HEAP-FCMBP
           PIC S9(4) COMP-5 VALUE 13.

       77 SQLM-HEAP-IMPORT-POOL
           PIC S9(4) COMP-5 VALUE 14.

       77 SQLM-HEAP-OTHER
           PIC S9(4) COMP-5 VALUE 15.

       77 SQLM-HEAP-BP
           PIC S9(4) COMP-5 VALUE 16.

       77 SQLM-HEAP-APP-GROUP
           PIC S9(4) COMP-5 VALUE 17.

       77 SQLM-HEAP-SHARED-SORT
           PIC S9(4) COMP-5 VALUE 18.

       77 SQLM-HEAP-MIN
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-HEAP-MAX
           PIC S9(4) COMP-5 VALUE 18.


      *****************************************************************************
      * database manager status  (db2_status)                                      *
      *****************************************************************************

       77 SQLM-DB2-ACTIVE
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-DB2-QUIESCE-PEND
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-DB2-QUIESCED
           PIC S9(4) COMP-5 VALUE 2.


      *****************************************************************************
      * UOW completion status (uow_comp_status)                                    *
      *****************************************************************************

       77 SQLM-UOWCOMMIT
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-UOWROLLBACK
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-UOWDEADLOCK
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-UOWABEND
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLM-APPL-END
           PIC S9(4) COMP-5 VALUE 5.

       77 SQLM-UOWLOCKTIMEOUT
           PIC S9(4) COMP-5 VALUE 6.

       77 SQLM-UOWUNKNOWN
           PIC S9(4) COMP-5 VALUE 7.


      *****************************************************************************
      * lock attributes (lock_attributes)                                         *
      *****************************************************************************

       77 SQLM-LOCKATTR-WAIT-FOR-AVAIL
           PIC S9(4) COMP-5 VALUE   00000001 .

       77 SQLM-LOCKATTR-ESCALATED
           PIC S9(4) COMP-5 VALUE   00000002 .

       77 SQLM-LOCKATTR-RR-IN-BLOCK
           PIC S9(4) COMP-5 VALUE   00000004 .

       77 SQLM-LOCKATTR-INSERT
           PIC S9(4) COMP-5 VALUE   00000008 .

       77 SQLM-LOCKATTR-RR
           PIC S9(4) COMP-5 VALUE   00000010 .

       77 SQLM-LOCKATTR-UPDATE-DELETE
           PIC S9(4) COMP-5 VALUE   00000020 .

       77 SQLM-LOCKATTR-ALLOW-NEW
           PIC S9(4) COMP-5 VALUE   00000040 .

       77 SQLM-LOCKATTR-NEW-REQUEST
           PIC S9(4) COMP-5 VALUE   00000080 .

       77 SQLM-LOCKATTR-DELETE-IN-BLOCK
           PIC S9(4) COMP-5 VALUE   00000000 .


      *****************************************************************************
      * lock release flags (lock_release_flags)                                    *
      *****************************************************************************

       77 SQLM-LOCKRELFLAGS-SQLCOMPILER
           PIC S9(5) COMP-5 VALUE   80000000 .

       77 SQLM-LOCKRELFLAGS-UNTRACKED
           PIC S9(5) COMP-5 VALUE   40000000 .


      *****************************************************************************
      * lock object type (lock_object_type)                                        *
      *****************************************************************************

       77 SQLM-TABLE-LOCK
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-ROW-LOCK
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-INTERNAL-LOCK
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-TABLESPACE-LOCK
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLM-EOT-LOCK
           PIC S9(4) COMP-5 VALUE 5.

       77 SQLM-KEYVALUE-LOCK
           PIC S9(4) COMP-5 VALUE 6.

       77 SQLM-SYSBOOT-LOCK
           PIC S9(4) COMP-5 VALUE 7.

       77 SQLM-INTERNALP-LOCK
           PIC S9(4) COMP-5 VALUE 8.

       77 SQLM-INTERNALV-LOCK
           PIC S9(4) COMP-5 VALUE 9.

       77 SQLM-INTERNALS-LOCK
           PIC S9(4) COMP-5 VALUE 10.

       77 SQLM-INTERNALJ-LOCK
           PIC S9(4) COMP-5 VALUE 11.

       77 SQLM-INTERNALL-LOCK
           PIC S9(4) COMP-5 VALUE 12.

       77 SQLM-INTERNALC-LOCK
           PIC S9(4) COMP-5 VALUE 13.

       77 SQLM-INTERNALB-LOCK
           PIC S9(4) COMP-5 VALUE 14.

       77 SQLM-INTERNALO-LOCK
           PIC S9(4) COMP-5 VALUE 15.

       77 SQLM-INTERNALT-LOCK
           PIC S9(4) COMP-5 VALUE 16.

       77 SQLM-INTERNALQ-LOCK
           PIC S9(4) COMP-5 VALUE 17.

       77 SQLM-INPLACE-REORG-LOCK
           PIC S9(4) COMP-5 VALUE 18.

       77 SQLM-BLOCK-LOCK
           PIC S9(4) COMP-5 VALUE 19.


      *****************************************************************************
      * lock status (lock_status)                                                  *
      *****************************************************************************

       77 SQLM-LRBGRNT
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-LRBCONV
           PIC S9(4) COMP-5 VALUE 2.


      *****************************************************************************
      * client communications protocols (client_protocol)                          *
      *****************************************************************************
       77 SQLM-PROT-APPC
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-PROT-NETBIOS
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-PROT-APPN
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-PROT-TCPIP
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-PROT-CPIC
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLM-PROT-IPXSPX
           PIC S9(4) COMP-5 VALUE 5.

       77 SQLM-PROT-LOCAL
           PIC S9(4) COMP-5 VALUE 6.

       77 SQLM-PROT-NPIPE
           PIC S9(4) COMP-5 VALUE 7.


      *****************************************************************************
      * Operating Systems (client_platform and server_platform)                    *
      *****************************************************************************
       77 SQLM-PLATFORM-UNKNOWN
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-PLATFORM-OS2
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-PLATFORM-DOS
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-PLATFORM-WINDOWS
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-PLATFORM-AIX
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLM-PLATFORM-NT
           PIC S9(4) COMP-5 VALUE 5.

       77 SQLM-PLATFORM-HP
           PIC S9(4) COMP-5 VALUE 6.

       77 SQLM-PLATFORM-SUN
           PIC S9(4) COMP-5 VALUE 7.

       77 SQLM-PLATFORM-MVS-DRDA
           PIC S9(4) COMP-5 VALUE 8.

       77 SQLM-PLATFORM-AS400-DRDA
           PIC S9(4) COMP-5 VALUE 9.

       77 SQLM-PLATFORM-VM-DRDA
           PIC S9(4) COMP-5 VALUE 10.

       77 SQLM-PLATFORM-VSE-DRDA
           PIC S9(4) COMP-5 VALUE 11.

       77 SQLM-PLATFORM-UNKNOWN-DRDA
           PIC S9(4) COMP-5 VALUE 12.

       77 SQLM-PLATFORM-SNI
           PIC S9(4) COMP-5 VALUE 13.

       77 SQLM-PLATFORM-MAC
           PIC S9(4) COMP-5 VALUE 14.

       77 SQLM-PLATFORM-WINDOWS95
           PIC S9(4) COMP-5 VALUE 15.

       77 SQLM-PLATFORM-SCO
           PIC S9(4) COMP-5 VALUE 16.

       77 SQLM-PLATFORM-SGI
           PIC S9(4) COMP-5 VALUE 17.

       77 SQLM-PLATFORM-LINUX
           PIC S9(4) COMP-5 VALUE 18.

       77 SQLM-PLATFORM-DYNIX
           PIC S9(4) COMP-5 VALUE 19.

       77 SQLM-PLATFORM-AIX64
           PIC S9(4) COMP-5 VALUE 20.

       77 SQLM-PLATFORM-SUN64
           PIC S9(4) COMP-5 VALUE 21.

       77 SQLM-PLATFORM-HP64
           PIC S9(4) COMP-5 VALUE 22.

       77 SQLM-PLATFORM-NT64
           PIC S9(4) COMP-5 VALUE 23.

       77 SQLM-PLATFORM-LINUX390
           PIC S9(4) COMP-5 VALUE 24.

       77 SQLM-PLATFORM-LINUXZ64
           PIC S9(4) COMP-5 VALUE 25.

       77 SQLM-PLATFORM-LINUXIA64
           PIC S9(4) COMP-5 VALUE 26.

       77 SQLM-PLATFORM-LINUXPPC
           PIC S9(4) COMP-5 VALUE 27.

       77 SQLM-PLATFORM-LINUXPPC64
           PIC S9(4) COMP-5 VALUE 28.

       77 SQLM-PLATFORM-OS390
           PIC S9(4) COMP-5 VALUE 29.

       77 SQLM-PLATFORM-LINUXX8664
           PIC S9(4) COMP-5 VALUE 30.

       77 SQLM-PLATFORM-HPIA
           PIC S9(4) COMP-5 VALUE 31.

       77 SQLM-PLATFORM-HPIA64
           PIC S9(4) COMP-5 VALUE 32.


      *****************************************************************************
      * Operating System Application Priority Type (appl_priority_type)            *
      *****************************************************************************
       77 SQLM-FIXED-PRIORITY
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-DYNAMIC-PRIORITY
           PIC S9(4) COMP-5 VALUE 2.


      *****************************************************************************
      * Isolation levels                                                           *
      *****************************************************************************
       77 SQLM-ISOLATION-LEVEL-NONE
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-ISOLATION-LEVEL-UR
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-ISOLATION-LEVEL-CS
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-ISOLATION-LEVEL-RS
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-ISOLATION-LEVEL-RR
           PIC S9(4) COMP-5 VALUE 4.


      *****************************************************************************
      * Event Monitor Record Type Identifiers  (sqlm_event_rec_header.type)        *
      *                                                                            *
      * Used to read the records in a trace produced by an Event Monitor           *
      *****************************************************************************
       77 SQLM-EVENT-DB
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-EVENT-CONN
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-EVENT-TABLE
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-EVENT-STMT
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLM-EVENT-STMTTEXT
           PIC S9(4) COMP-5 VALUE 5.

       77 SQLM-EVENT-XACT
           PIC S9(4) COMP-5 VALUE 6.

       77 SQLM-EVENT-DEADLOCK
           PIC S9(4) COMP-5 VALUE 7.

       77 SQLM-EVENT-DLCONN
           PIC S9(4) COMP-5 VALUE 8.

       77 SQLM-EVENT-TABLESPACE
           PIC S9(4) COMP-5 VALUE 9.

       77 SQLM-EVENT-DBHEADER
           PIC S9(4) COMP-5 VALUE 10.

       77 SQLM-EVENT-START
           PIC S9(4) COMP-5 VALUE 11.

       77 SQLM-EVENT-CONNHEADER
           PIC S9(4) COMP-5 VALUE 12.

       77 SQLM-EVENT-OVERFLOW
           PIC S9(4) COMP-5 VALUE 13.

       77 SQLM-EVENT-BUFFERPOOL
           PIC S9(4) COMP-5 VALUE 14.

       77 SQLM-EVENT-SUBSECTION
           PIC S9(4) COMP-5 VALUE 15.

       77 SQLM-EVENT-LOG-HEADER
           PIC S9(4) COMP-5 VALUE 16.

       77 SQLM-EVENT-DBMEMUSE
           PIC S9(4) COMP-5 VALUE 17.

       77 SQLM-EVENT-CONNMEMUSE
           PIC S9(4) COMP-5 VALUE 18.

       77 SQLM-EVENT-STMT-HISTORY
           PIC S9(4) COMP-5 VALUE 19.

       77 SQLM-EVENT-DATA-VALUE
           PIC S9(4) COMP-5 VALUE 20.

       77 SQLM-EVENT-MAX
           PIC S9(4) COMP-5 VALUE 20.

      * the list                            *

      *****************************************************************************
      * Event Monitor Byte Order Identifiers (byte_order)                          *
      *****************************************************************************
       77 SQLM-LITTLE-ENDIAN
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-BIG-ENDIAN
           PIC S9(4) COMP-5 VALUE -1.


      *****************************************************************************
      * Database Monitor Error Constants (sqlcode)                                 *
      *****************************************************************************
       77 SQLM-RC-INV-PTR
           PIC S9(4) COMP-5 VALUE -1601.

       77 SQLM-RC-INV-OBJ-TYPE
           PIC S9(4) COMP-5 VALUE -1602.

       77 SQLM-RC-NOT-SPECIFIED
           PIC S9(4) COMP-5 VALUE -1603.

       77 SQLM-RC-NOT-NULL-TERM
           PIC S9(4) COMP-5 VALUE -1604.

       77 SQLM-RC-DBNOACT
           PIC S9(4) COMP-5 VALUE 1605.

       77 SQLM-RC-BUFFER-FULL
           PIC S9(4) COMP-5 VALUE 1606.

       77 SQLM-RC-NOMEM
           PIC S9(4) COMP-5 VALUE -1607.

       77 SQLM-RC-DUPLICATE-DB
           PIC S9(4) COMP-5 VALUE 1608.

       77 SQLM-RC-REMOTE-DB
           PIC S9(4) COMP-5 VALUE -1609.

       77 SQLM-RC-INV-VALUE
           PIC S9(4) COMP-5 VALUE -1610.

       77 SQLM-RC-NO-DATA
           PIC S9(4) COMP-5 VALUE 1611.

       77 SQLM-RC-TOO-MANY-OBJECTS
           PIC S9(4) COMP-5 VALUE -1623.

       77 SQLM-RC-MULTIPLE-NODES
           PIC S9(4) COMP-5 VALUE -1624.


      *****************************************************************************
      * FCM Connection Status (connection_status)                                  *
      *****************************************************************************
       77 SQLM-FCM-CONNECT-INACTIVE
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-FCM-CONNECT-ACTIVE
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-FCM-CONNECT-CONGESTED
           PIC S9(4) COMP-5 VALUE 2.


      *****************************************************************************
      * Database location relative to snapshot application (db_location)           *
      *****************************************************************************
       77 SQLM-LOCAL
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-REMOTE
           PIC S9(4) COMP-5 VALUE 2.


      *****************************************************************************
      * Node location definitions                                                  *
      *****************************************************************************
       77 SQLM-CURRENT-NODE
           PIC S9(4) COMP-5 VALUE -1.

       77 SQLM-ALL-NODES
           PIC S9(4) COMP-5 VALUE -2.


      *****************************************************************************
      * Log space usage definitions                                                *
      *****************************************************************************
       77 SQLM-LOGSPACE-INFINITE
           PIC S9(18) COMP-5 VALUE -1.


      *****************************************************************************
      * Log file number definitions                                                *
      *****************************************************************************
       77 SQLM-LOGFILE-NUM-UNKNOWN
           PIC S9(4) COMP-5 VALUE -1.


      *****************************************************************************
      * Utility types associated with SQLM_ELM_UTILITY_TYPE                        *
      *****************************************************************************
       77 SQLM-UTILITY-REBALANCE
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-UTILITY-BACKUP
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-UTILITY-RUNSTATS
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-UTILITY-REORG
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-UTILITY-RESTORE
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLM-UTILITY-CRASH-RECOVERY
           PIC S9(4) COMP-5 VALUE 5.

      * The full name of the monitor element is SQLM_UTILITY_ROLLFORWARD_RECOVERY
       77 SQLM-UTL-ROLLFWD-RCVRY
           PIC S9(4) COMP-5 VALUE 6.

       77 SQLM-UTILITY-LOAD
           PIC S9(4) COMP-5 VALUE 7.

      * The full name of the monitor element is SQLM_UTILITY_RESTART_RECREATE_INDEX
       77 SQLM-UTL-RESTART-INDEX
           PIC S9(4) COMP-5 VALUE 8.


      *****************************************************************************
      * Utility priority of value 0                                                *
      *****************************************************************************
       77 SQLM-UTILITY-UNTHROTTLED
           PIC S9(4) COMP-5 VALUE 0.


      *****************************************************************************
      * Progress information work metric values                                    *
      *****************************************************************************
       77 SQLM-WORK-METRIC-NOT-SUPPORT
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-WORK-METRIC-BYTES
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-WORK-METRIC-EXTENTS
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-WORK-METRIC-ROWS
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-WORK-METRIC-PAGES
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLM-WORK-METRIC-INDEXES
           PIC S9(4) COMP-5 VALUE 5.


      *****************************************************************************
      * Current HADR roles of the database                                         *
      *****************************************************************************
       77 SQLM-HADR-ROLE-STANDARD
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-HADR-ROLE-PRIMARY
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-HADR-ROLE-STANDBY
           PIC S9(4) COMP-5 VALUE 2.


      *****************************************************************************
      * State of HADR subsystem                                                    *
      *****************************************************************************
       77 SQLM-HADR-STATE-DISCONNECTED
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-HADR-STATE-LOCAL-CATCHUP
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-HADR-STATE-REM-CATCH-PEND
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-HADR-STATE-REM-CATCHUP
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-HADR-STATE-PEER
           PIC S9(4) COMP-5 VALUE 4.


      *****************************************************************************
      * HADR synchronization mode                                                  *
      *****************************************************************************
       77 SQLM-HADR-SYNCMODE-ASYNC
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-HADR-SYNCMODE-NEARSYNC
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-HADR-SYNCMODE-SYNC
           PIC S9(4) COMP-5 VALUE 2.


      *****************************************************************************
      * Status of HADR primary-standby connection                                  *
      *****************************************************************************
       77 SQLM-HADR-CONN-CONNECTED
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-HADR-CONN-CONGESTED
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-HADR-CONN-DISCONNECTED
           PIC S9(4) COMP-5 VALUE 2.


      *****************************************************************************
      * Snapshot Monitoring defines (db2GetSnapshot, etc) for self-describing      *
      * datastream                                                                 *
      * Used by all versions of snapshot v6 and later.                             *
      *****************************************************************************
      *****************************************************************************
      * Defines for snapshot output stream format                                  *
      *****************************************************************************
       77 SQLM-STREAM-STATIC-FORMAT
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-STREAM-DYNAMIC-FORMAT
           PIC S9(4) COMP-5 VALUE 1.


      *****************************************************************************
      * This structure can be used for parsing through the self describing snapshot*
      * data stream. All data in the snapshot and event monitor hiearchy have this *
      * header.                                                                    *
      *                                                                            *
      *         size:         The size (in bytes) of the data stored in the data   *
      *                       element.                                             *
      *                                                                            *
      *         type:         The type of element stored in the data. See the      *
      *                       defines of the form SQLM_TYP_... for values note     *
      *                       that the type HEADER refers to "logical" types,      *
      *                       meaning they do not represent specific data          *
      *                       elements, but rather logical groupings of elements.  *
      *                                                                            *
      *      element:         The element that data holds. See the defines of the  *
      *                       form SQLM_ELM_... for values                         *
      *                                                                            *
      *         data:         The data itself.                                     *
      *                                                                            *
      *****************************************************************************
      *                                                                            *
      * Snapshot data is returned in a heirarchtical fashion. At the bottom of the *
      * hiearchy, are the individual data elements. Higher up in the hiearchy, are *
      * the logical groupings of data elements. What follows is a description of   *
      * the logical data hiearchy. Shown by element values.                        *
      *                                                                            *
      * SQLM_ELM_COLLECTED - Top level logical data member, contains information   *
      *                      relevant to the entire snapshot, and indicates how    *
      *                      many second tier snapshot logical data elements will  *
      *                      follow.                                               *
      * SQLM_ELM_DB2             - DB2 instance information                        *
      * SQLM_ELM_FCM             - FCM information                                 *
      * SQLM_ELM_FCM_NODE        - FCM node information                            *
      * SQLM_ELM_APPL_INFO       - application identifiern                         *
      * SQLM_ELM_APPL            - application information                         *
      * SQLM_ELM_DCS_APPL_INFO   - dcs application identifier                      *
      * SQLM_ELM_DCS_APPL        - dcs application information                     *
      * SQLM_ELM_DCS_STMT        - dcs statement information                       *
      * SQLM_ELM_AGENT           - agent information                               *
      * SQLM_ELM_LOCK_WAIT       - lock waiting information                        *
      * SQLM_ELM_DCS_DBASE       - dcs database information                        *
      * SQLM_ELM_DBASE           - database information                            *
      * SQLM_ELM_ROLLFORWARD     - database rollforward information                *
      * SQLM_ELM_TABLE_LIST      - database wide table information                 *
      * SQLM_ELM_TABLE           - table information                               *
      * SQLM_ELM_LOCK            - lock information                                *
      * SQLM_ELM_TABLESPACE_LIST - database wide tablespace info                   *
      * SQLM_ELM_TABLESPACE      - tablespace information                          *
      * SQLM_ELM_BUFFERPOOL      - bufferpool information                          *
      * SQLM_ELM_DYNSQL          - dynamic sql statement information               *
      * SQLM_ELM_DYNSQL_LIST     - dynamic sql statement list                      *
      * SQLM_ELM_SWITCHES        - a list of the switch state on the server        *
      * SQLM_ELM_APPL_LOCK_LIST  - a list of application locks                     *
      * SQLM_ELM_DB_LOCK_LIST    - a list of database locks                        *
      * SQLM_ELM_STMT            - statement information                           *
      * SQLM_ELM_SUBSECTION      - subsection information                          *
      * SQLM_ELM_DBASE_REMOTE    - remote datasource information for database      *
      * SQLM_ELM_APPL_REMOTE     - remote datasource information for application   *
      * SQLM_ELM_TABLE_REORG     - table reorg data                                *
      * SQLM_ELM_MEMORY_POOL     - memory pool statistics                          *
      * SQLM_ELM_TABLESPACE_QUIESCER        - List of quiescers for a tablespace   *
      * SQLM_ELM_TABLESPACE_CONTAINER       - List of containers for a TS          *
      * SQLM_ELM_TABLESPACE_RANGE           - List of ranges for TS map            *
      * SQLM_ELM_TABLESPACE_RANGE_CONTAINER - List of containers for range         *
      * SQLM_ELM_TABLESPACE_NODEINFO        - Node unique info for a tablespace    *
      * SQLM_ELM_BUFFERPOOL_NODEINFO        - Node unique info for a bufferpool    *
      *                                                                            *
      * SQLM_ELM_COLLECTED --- DB2 - FCM - FCM_NODE                                *
      *                            +- MEMORY_POOL                                  *
      *                            +- SWITCH_LIST +- UOW_SW                        *
      *                                           +- STATEMENT_SW                  *
      *                                           +- LOCK_SW                       *
      *                                           +- BUFFPOOL_SW                   *
      *                                           +- TABLE_SW                      *
      *                                           +- SORT_SW                       *
      *                                           +- TIMESTAMP_SW                  *
      *                     +- APPL_INFO                                           *
      *                     +- DCS_APPL_INFO                                       *
      *                     +- DCS_APPL -- STMT                                    *
      *                                 +- DCS_APPL_INFO                           *
      *                     +- APPL -- AGENT -- MEMORY_POOL                        *
      *                             +- APPLINFO                                    *
      *                             +- LOCK_WAIT                                   *
      *                             +- STMT -  SUBSECTION - AGENT MEMORY_POOL (MPP)*
      *                                     +- AGENT --MEMORY_POOL            (SMP)*
      *                     +- DCS_DBASE                                           *
      *                     +- DBASE -  ROLLFORWARD - TABLESPACE                   *
      *                              +- MEMORY_POOL                                *
      *                     +- TABLE_LIST - TABLE - TABLE_REORG                    *
      *                     +- APPL_LOCK_LIST +-- LOCK                             *
      *                                       +- LOCK_WAIT - LOCK                  *
      *                     +- DB_LOCK_LIST +- LOCK                                *
      *                                     +- LOCK_WAIT - LOCK                    *
      *                                     +- APPL_LOCK_LIST                      *
      *                     +- TABLESPACE_LIST - TABLESPACE                        *
      *                                        +-QUIESCER                          *
      *                                        +-CONTAINER                         *
      *                                        +-RANGE                             *
      *                                        +-RANGE_CONTAINER                   *
      *                                        +-NODEINFO                          *
      *                     +- BUFFERPOOL      --NODEINFO                          *
      *                     +- DYNSQL_LIST - DYNSQL                                *
      *                     +- SWITCH_LIST +- UOW_SW                               *
      *                                    +- STATEMENT_SW                         *
      *                                    +- LOCK_SW                              *
      *                                    +- BUFFPOOL_SW                          *
      *                                    +- TABLE_SW                             *
      *                                    +- SORT_SW                              *
      *                                    +- TIMESTAMP_SW                         *
      *                     +- DBASE_REMOTE                                        *
      *                     +- APPL_REMOTE                                         *
      *                                                                            *
      *****************************************************************************
      *                                                                            *
      * Event monitor data is returned as a series of records of the following     *
      * types:                                                                     *
      * SQLM_ELM_EVENT_DB         - database event                                 *
      * SQLM_ELM_EVENT_CONN       - connection event                               *
      * SQLM_ELM_EVENT_TABLE      - table event                                    *
      * SQLM_ELM_EVENT_STMT       - statement event                                *
      * SQLM_ELM_EVENT_XACT       - transaction event                              *
      * SQLM_ELM_EVENT_DEADLOCK   - deadlock event                                 *
      * SQLM_ELM_EVENT_DLCONN     - deadlocked connection event                    *
      * SQLM_ELM_EVENT_TABLESPACE - tablespace event                               *
      * SQLM_ELM_EVENT_DBHEADER   - database header event                          *
      * SQLM_ELM_EVENT_START      - event monitor start                            *
      * SQLM_ELM_EVENT_CONNHEADER - connection header event                        *
      * SQLM_ELM_EVENT_OVERFLOW   - overflow event                                 *
      * SQLM_ELM_EVENT_BUFFERPOOL - bufferpool event                               *
      * SQLM_ELM_EVENT_SUBSECTION - subsection event                               *
      * SQLM_ELM_EVENT_LOCK_LIST  - lock list for en_dlock                         *
      * SQLM_ELM_EVENT_CONTROL    - control table                                  *
      * SQLM_ELM_EVENT_LOG_HEADER - log start event                                *
      * SQLM_ELM_EVENT_DETAILED_DLCONN - detailed deadlocked connection event      *
      * SQLM_ELM_EVENT_STMT_HISTORY - part of details dlock with history evmon     *
      * SQLM_ELM_EVENT_DATA_VALUE - paramter values corresponding to stmt history  *
      *                                                                            *
      *****************************************************************************

      *****************************************************************************
      * Monitor data types (reside in the "type" field of the sqlm_header_data     *
      *****************************************************************************
      ****************Logical data elements****************************************
       77 SQLM-TYPE-HEADER
           PIC S9(4) COMP-5 VALUE 1.


      ****************Base data elements*******************************************
       77 SQLM-TYPE-STRING
           PIC S9(4) COMP-5 VALUE 50.

       77 SQLM-TYPE-U8BIT
           PIC S9(4) COMP-5 VALUE 51.

       77 SQLM-TYPE-8BIT
           PIC S9(4) COMP-5 VALUE 52.

       77 SQLM-TYPE-16BIT
           PIC S9(4) COMP-5 VALUE 53.

       77 SQLM-TYPE-U16BIT
           PIC S9(4) COMP-5 VALUE 54.

       77 SQLM-TYPE-32BIT
           PIC S9(4) COMP-5 VALUE 55.

       77 SQLM-TYPE-U32BIT
           PIC S9(4) COMP-5 VALUE 56.

       77 SQLM-TYPE-U64BIT
           PIC S9(4) COMP-5 VALUE 57.

       77 SQLM-TYPE-64BIT
           PIC S9(4) COMP-5 VALUE 58.

       77 SQLM-TYPE-HANDLE
           PIC S9(4) COMP-5 VALUE 59.


      *****************************************************************************
      * Monitor data elements (reside in the "element" field of the                *
      * sqlm_header_data                                                           *
      *****************************************************************************
      * The following group of defines relate to logical groupings of snapshot     *
      * information. They do not represent data elements, but rather groups of     *
      * data elements                                                              *
      *****************************************************************************
       77 SQLM-ELM-DB2
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-ELM-FCM
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-ELM-FCM-NODE
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-ELM-APPL-INFO
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLM-ELM-APPL
           PIC S9(4) COMP-5 VALUE 5.

       77 SQLM-ELM-DCS-APPL-INFO
           PIC S9(4) COMP-5 VALUE 6.

       77 SQLM-ELM-DCS-APPL
           PIC S9(4) COMP-5 VALUE 7.

       77 SQLM-ELM-DCS-STMT
           PIC S9(4) COMP-5 VALUE 8.

       77 SQLM-ELM-SUBSECTION
           PIC S9(4) COMP-5 VALUE 9.

       77 SQLM-ELM-AGENT
           PIC S9(4) COMP-5 VALUE 10.

       77 SQLM-ELM-LOCK-WAIT
           PIC S9(4) COMP-5 VALUE 11.

       77 SQLM-ELM-DCS-DBASE
           PIC S9(4) COMP-5 VALUE 12.

       77 SQLM-ELM-DBASE
           PIC S9(4) COMP-5 VALUE 13.

       77 SQLM-ELM-ROLLFORWARD
           PIC S9(4) COMP-5 VALUE 14.

       77 SQLM-ELM-TABLE
           PIC S9(4) COMP-5 VALUE 15.

       77 SQLM-ELM-LOCK
           PIC S9(4) COMP-5 VALUE 16.

       77 SQLM-ELM-TABLESPACE
           PIC S9(4) COMP-5 VALUE 17.

       77 SQLM-ELM-BUFFERPOOL
           PIC S9(4) COMP-5 VALUE 18.

       77 SQLM-ELM-DYNSQL
           PIC S9(4) COMP-5 VALUE 19.

       77 SQLM-ELM-COLLECTED
           PIC S9(4) COMP-5 VALUE 20.

       77 SQLM-ELM-SWITCH-LIST
           PIC S9(4) COMP-5 VALUE 21.

       77 SQLM-ELM-UOW-SW
           PIC S9(4) COMP-5 VALUE 22.

       77 SQLM-ELM-STATEMENT-SW
           PIC S9(4) COMP-5 VALUE 23.

       77 SQLM-ELM-TABLE-SW
           PIC S9(4) COMP-5 VALUE 24.

       77 SQLM-ELM-BUFFPOOL-SW
           PIC S9(4) COMP-5 VALUE 25.

       77 SQLM-ELM-LOCK-SW
           PIC S9(4) COMP-5 VALUE 26.

       77 SQLM-ELM-SORT-SW
           PIC S9(4) COMP-5 VALUE 27.

       77 SQLM-ELM-TABLE-LIST
           PIC S9(4) COMP-5 VALUE 28.

       77 SQLM-ELM-TABLESPACE-LIST
           PIC S9(4) COMP-5 VALUE 29.

       77 SQLM-ELM-DYNSQL-LIST
           PIC S9(4) COMP-5 VALUE 30.

       77 SQLM-ELM-APPL-LOCK-LIST
           PIC S9(4) COMP-5 VALUE 31.

       77 SQLM-ELM-DB-LOCK-LIST
           PIC S9(4) COMP-5 VALUE 32.

       77 SQLM-ELM-STMT
           PIC S9(4) COMP-5 VALUE 33.

       77 SQLM-ELM-DBASE-REMOTE
           PIC S9(4) COMP-5 VALUE 34.

       77 SQLM-ELM-APPL-REMOTE
           PIC S9(4) COMP-5 VALUE 35.

       77 SQLM-ELM-APPL-ID-INFO
           PIC S9(4) COMP-5 VALUE 36.

       77 SQLM-ELM-STMT-TRANSMISSIONS
           PIC S9(4) COMP-5 VALUE 37.

       77 SQLM-ELM-TIMESTAMP-SW
           PIC S9(4) COMP-5 VALUE 38.

       77 SQLM-ELM-TABLE-REORG
           PIC S9(4) COMP-5 VALUE 39.

       77 SQLM-ELM-MEMORY-POOL
           PIC S9(4) COMP-5 VALUE 40.

       77 SQLM-ELM-TABLESPACE-QUIESCER
           PIC S9(4) COMP-5 VALUE 41.

       77 SQLM-ELM-TABLESPACE-CONTAINER
           PIC S9(4) COMP-5 VALUE 42.

       77 SQLM-ELM-TABLESPACE-RANGE
           PIC S9(4) COMP-5 VALUE 43.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_RANGE_CONTAINER
       77 SQLM-ELM-TBSP-RANGE-CONT
           PIC S9(4) COMP-5 VALUE 44.

       77 SQLM-ELM-TABLESPACE-NODEINFO
           PIC S9(4) COMP-5 VALUE 45.

       77 SQLM-ELM-HEALTH-INDICATOR
           PIC S9(4) COMP-5 VALUE 46.

       77 SQLM-ELM-HEALTH-INDICATOR-HIST
           PIC S9(4) COMP-5 VALUE 47.

       77 SQLM-ELM-BUFFERPOOL-NODEINFO
           PIC S9(4) COMP-5 VALUE 48.

       77 SQLM-ELM-UTILITY
           PIC S9(4) COMP-5 VALUE 49.

       77 SQLM-ELM-HI-OBJ-LIST
           PIC S9(4) COMP-5 VALUE 50.

       77 SQLM-ELM-HI-OBJ-LIST-HIST
           PIC S9(4) COMP-5 VALUE 51.

       77 SQLM-ELM-PROGRESS
           PIC S9(4) COMP-5 VALUE 52.

       77 SQLM-ELM-PROGRESS-LIST
           PIC S9(4) COMP-5 VALUE 53.

       77 SQLM-ELM-HADR
           PIC S9(4) COMP-5 VALUE 54.

       77 SQLM-ELM-DETAIL-LOG
           PIC S9(4) COMP-5 VALUE 55.

       77 SQLM-ELM-ROLLBACK-PROGRESS
           PIC S9(4) COMP-5 VALUE 56.

       77 SQLM-ELM-DB-STORAGE-GROUP
           PIC S9(4) COMP-5 VALUE 57.


       77 SQLM-MAX-LOGICAL-ELEMENT
           PIC S9(4) COMP-5 VALUE 57.


      *****************************************************************************
      * The following group of defines relate to logical groupings of event monitor*
      * information. They do not represent data elements, but rather groups of     *
      * data elements                                                              *
      *****************************************************************************
       77 SQLM-ELM-EVENT-DB
           PIC S9(4) COMP-5 VALUE 100.

       77 SQLM-ELM-EVENT-CONN
           PIC S9(4) COMP-5 VALUE 101.

       77 SQLM-ELM-EVENT-TABLE
           PIC S9(4) COMP-5 VALUE 102.

       77 SQLM-ELM-EVENT-STMT
           PIC S9(4) COMP-5 VALUE 103.

       77 SQLM-ELM-EVENT-XACT
           PIC S9(4) COMP-5 VALUE 104.

       77 SQLM-ELM-EVENT-DEADLOCK
           PIC S9(4) COMP-5 VALUE 105.

       77 SQLM-ELM-EVENT-DLCONN
           PIC S9(4) COMP-5 VALUE 106.

       77 SQLM-ELM-EVENT-TABLESPACE
           PIC S9(4) COMP-5 VALUE 107.

       77 SQLM-ELM-EVENT-DBHEADER
           PIC S9(4) COMP-5 VALUE 108.

       77 SQLM-ELM-EVENT-START
           PIC S9(4) COMP-5 VALUE 109.

       77 SQLM-ELM-EVENT-CONNHEADER
           PIC S9(4) COMP-5 VALUE 110.

       77 SQLM-ELM-EVENT-OVERFLOW
           PIC S9(4) COMP-5 VALUE 111.

       77 SQLM-ELM-EVENT-BUFFERPOOL
           PIC S9(4) COMP-5 VALUE 112.

       77 SQLM-ELM-EVENT-SUBSECTION
           PIC S9(4) COMP-5 VALUE 113.

       77 SQLM-ELM-EVENT-LOG-HEADER
           PIC S9(4) COMP-5 VALUE 114.

       77 SQLM-ELM-EVENT-CONTROL
           PIC S9(4) COMP-5 VALUE 115.

       77 SQLM-ELM-EVENT-LOCK-LIST
           PIC S9(4) COMP-5 VALUE 116.

       77 SQLM-ELM-EVENT-DETAILED-DLCONN
           PIC S9(4) COMP-5 VALUE 117.

       77 SQLM-ELM-EVENT-CONNMEMUSE
           PIC S9(4) COMP-5 VALUE 118.

       77 SQLM-ELM-EVENT-DBMEMUSE
           PIC S9(4) COMP-5 VALUE 119.

       77 SQLM-ELM-EVENT-STMT-HISTORY
           PIC S9(4) COMP-5 VALUE 120.

       77 SQLM-ELM-EVENT-DATA-VALUE
           PIC S9(4) COMP-5 VALUE 121.


      *****************************************************************************
      * Timestamps are returned as logic data elements with seconds and            *
      * microseconds returned in seperate base elements                            *
      *****************************************************************************
       77 SQLM-MIN-TIME-STAMP
           PIC S9(4) COMP-5 VALUE 200.


       77 SQLM-ELM-TIME-STAMP
           PIC S9(4) COMP-5 VALUE 200.

       77 SQLM-ELM-STATUS-CHANGE-TIME
           PIC S9(4) COMP-5 VALUE 201.

       77 SQLM-ELM-GW-CON-TIME
           PIC S9(4) COMP-5 VALUE 202.

       77 SQLM-ELM-PREV-UOW-STOP-TIME
           PIC S9(4) COMP-5 VALUE 203.

       77 SQLM-ELM-UOW-START-TIME
           PIC S9(4) COMP-5 VALUE 204.

       77 SQLM-ELM-UOW-STOP-TIME
           PIC S9(4) COMP-5 VALUE 205.

       77 SQLM-ELM-STMT-START
           PIC S9(4) COMP-5 VALUE 206.

       77 SQLM-ELM-STMT-STOP
           PIC S9(4) COMP-5 VALUE 207.

       77 SQLM-ELM-LAST-RESET
           PIC S9(4) COMP-5 VALUE 208.

       77 SQLM-ELM-DB2START-TIME
           PIC S9(4) COMP-5 VALUE 209.

       77 SQLM-ELM-DB-CONN-TIME
           PIC S9(4) COMP-5 VALUE 210.

       77 SQLM-ELM-LAST-BACKUP
           PIC S9(4) COMP-5 VALUE 211.

       77 SQLM-ELM-LOCK-WAIT-START-TIME
           PIC S9(4) COMP-5 VALUE 212.

       77 SQLM-ELM-APPL-CON-TIME
           PIC S9(4) COMP-5 VALUE 213.

       77 SQLM-ELM-CONN-COMPLETE-TIME
           PIC S9(4) COMP-5 VALUE 214.

       77 SQLM-ELM-DISCONN-TIME
           PIC S9(4) COMP-5 VALUE 215.

       77 SQLM-ELM-EVENT-TIME
           PIC S9(4) COMP-5 VALUE 216.

       77 SQLM-ELM-START-TIME
           PIC S9(4) COMP-5 VALUE 217.

       77 SQLM-ELM-STOP-TIME
           PIC S9(4) COMP-5 VALUE 218.

       77 SQLM-ELM-RF-TIMESTAMP
           PIC S9(4) COMP-5 VALUE 219.

       77 SQLM-ELM-CONN-TIME
           PIC S9(4) COMP-5 VALUE 220.

       77 SQLM-ELM-FIRST-OVERFLOW-TIME
           PIC S9(4) COMP-5 VALUE 221.

       77 SQLM-ELM-LAST-OVERFLOW-TIME
           PIC S9(4) COMP-5 VALUE 222.

       77 SQLM-ELM-GW-EXEC-TIME
           PIC S9(4) COMP-5 VALUE 223.

       77 SQLM-ELM-AGENT-USR-CPU-TIME
           PIC S9(4) COMP-5 VALUE 224.

       77 SQLM-ELM-AGENT-SYS-CPU-TIME
           PIC S9(4) COMP-5 VALUE 225.

       77 SQLM-ELM-SS-USR-CPU-TIME
           PIC S9(4) COMP-5 VALUE 226.

       77 SQLM-ELM-SS-SYS-CPU-TIME
           PIC S9(4) COMP-5 VALUE 227.

       77 SQLM-ELM-USER-CPU-TIME
           PIC S9(4) COMP-5 VALUE 228.

       77 SQLM-ELM-TOTAL-EXEC-TIME
           PIC S9(4) COMP-5 VALUE 229.

       77 SQLM-ELM-SWITCH-SET-TIME
           PIC S9(4) COMP-5 VALUE 230.

       77 SQLM-ELM-ELAPSED-EXEC-TIME
           PIC S9(4) COMP-5 VALUE 231.

       77 SQLM-ELM-SELECT-TIME
           PIC S9(4) COMP-5 VALUE 232.

       77 SQLM-ELM-INSERT-TIME
           PIC S9(4) COMP-5 VALUE 233.

       77 SQLM-ELM-UPDATE-TIME
           PIC S9(4) COMP-5 VALUE 234.

       77 SQLM-ELM-DELETE-TIME
           PIC S9(4) COMP-5 VALUE 235.

       77 SQLM-ELM-CREATE-NICKNAME-TIME
           PIC S9(4) COMP-5 VALUE 236.

       77 SQLM-ELM-PASSTHRU-TIME
           PIC S9(4) COMP-5 VALUE 237.

       77 SQLM-ELM-STORED-PROC-TIME
           PIC S9(4) COMP-5 VALUE 238.

       77 SQLM-ELM-REMOTE-LOCK-TIME
           PIC S9(4) COMP-5 VALUE 239.

       77 SQLM-ELM-NETWORK-TIME-TOP
           PIC S9(4) COMP-5 VALUE 240.

       77 SQLM-ELM-NETWORK-TIME-BOTTOM
           PIC S9(4) COMP-5 VALUE 241.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_REBALANCER_START_TIME
       77 SQLM-ELM-TBSP-REB-START-TIME
           PIC S9(4) COMP-5 VALUE 242.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_REBALANCER_RESTART_TIME
       77 SQLM-ELM-TBSP-REB-RESTART-TIME
           PIC S9(4) COMP-5 VALUE 243.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_MIN_RECOVERY_TIME
       77 SQLM-ELM-TBSP-MIN-RCVRY-TIME
           PIC S9(4) COMP-5 VALUE 244.

       77 SQLM-ELM-HI-TIMESTAMP
           PIC S9(4) COMP-5 VALUE 245.

       77 SQLM-MAX-TIME-STAMP
           PIC S9(4) COMP-5 VALUE 245.


      *****************************************************************************
      * Actual database monitor elements                                           *
      *****************************************************************************
       77 SQLM-ELM-SECONDS
           PIC S9(4) COMP-5 VALUE 300.

       77 SQLM-ELM-MICROSEC
           PIC S9(4) COMP-5 VALUE 301.

       77 SQLM-ELM-AGENT-ID
           PIC S9(4) COMP-5 VALUE 302.

       77 SQLM-ELM-SERVER-DB2-TYPE
           PIC S9(4) COMP-5 VALUE 303.

       77 SQLM-ELM-SERVER-PRDID
           PIC S9(4) COMP-5 VALUE 304.

       77 SQLM-ELM-SERVER-NNAME
           PIC S9(4) COMP-5 VALUE 305.

       77 SQLM-ELM-SERVER-INSTANCE-NAME
           PIC S9(4) COMP-5 VALUE 306.

       77 SQLM-ELM-NODE-NUMBER
           PIC S9(4) COMP-5 VALUE 307.

       77 SQLM-ELM-TIME-ZONE-DISP
           PIC S9(4) COMP-5 VALUE 308.

       77 SQLM-ELM-SERVER-VERSION
           PIC S9(4) COMP-5 VALUE 309.

       77 SQLM-ELM-APPL-STATUS
           PIC S9(4) COMP-5 VALUE 310.

       77 SQLM-ELM-CODEPAGE-ID
           PIC S9(4) COMP-5 VALUE 311.

       77 SQLM-ELM-STMT-TEXT
           PIC S9(4) COMP-5 VALUE 312.

       77 SQLM-ELM-APPL-NAME
           PIC S9(4) COMP-5 VALUE 313.

       77 SQLM-ELM-APPL-ID
           PIC S9(4) COMP-5 VALUE 314.

       77 SQLM-ELM-SEQUENCE-NO
           PIC S9(4) COMP-5 VALUE 315.

       77 SQLM-ELM-AUTH-ID
           PIC S9(4) COMP-5 VALUE 316.

       77 SQLM-ELM-PRIMARY-AUTH-ID
           PIC S9(4) COMP-5 VALUE 316.

       77 SQLM-ELM-CLIENT-NNAME
           PIC S9(4) COMP-5 VALUE 317.

       77 SQLM-ELM-CLIENT-PRDID
           PIC S9(4) COMP-5 VALUE 318.

       77 SQLM-ELM-INPUT-DB-ALIAS
           PIC S9(4) COMP-5 VALUE 319.

       77 SQLM-ELM-CLIENT-DB-ALIAS
           PIC S9(4) COMP-5 VALUE 320.

       77 SQLM-ELM-DB-NAME
           PIC S9(4) COMP-5 VALUE 321.

       77 SQLM-ELM-DB-PATH
           PIC S9(4) COMP-5 VALUE 322.

       77 SQLM-ELM-NUM-ASSOC-AGENTS
           PIC S9(4) COMP-5 VALUE 323.

       77 SQLM-ELM-COORD-NODE-NUM
           PIC S9(4) COMP-5 VALUE 324.

       77 SQLM-ELM-AUTHORITY-LVL
           PIC S9(4) COMP-5 VALUE 325.

       77 SQLM-ELM-EXECUTION-ID
           PIC S9(4) COMP-5 VALUE 326.

       77 SQLM-ELM-CORR-TOKEN
           PIC S9(4) COMP-5 VALUE 327.

       77 SQLM-ELM-CLIENT-PID
           PIC S9(4) COMP-5 VALUE 328.

       77 SQLM-ELM-CLIENT-PLATFORM
           PIC S9(4) COMP-5 VALUE 329.

       77 SQLM-ELM-CLIENT-PROTOCOL
           PIC S9(4) COMP-5 VALUE 330.

       77 SQLM-ELM-COUNTRY-CODE
           PIC S9(4) COMP-5 VALUE 331.

       77 SQLM-ELM-TERRITORY-CODE
           PIC S9(4) COMP-5 VALUE 331.

       77 SQLM-ELM-COORD-AGENT-PID
           PIC S9(4) COMP-5 VALUE 332.

       77 SQLM-ELM-GW-DB-ALIAS
           PIC S9(4) COMP-5 VALUE 333.

       77 SQLM-ELM-OUTBOUND-COMM-ADDRESS
           PIC S9(4) COMP-5 VALUE 334.

       77 SQLM-ELM-INBOUND-COMM-ADDRESS
           PIC S9(4) COMP-5 VALUE 335.

      * The full name of the monitor element is SQLM_ELM_OUTBOUND_COMM_PROTOCOL
       77 SQLM-ELM-OUTB-COMM-PROTO
           PIC S9(4) COMP-5 VALUE 336.

       77 SQLM-ELM-DCS-DB-NAME
           PIC S9(4) COMP-5 VALUE 337.

       77 SQLM-ELM-HOST-DB-NAME
           PIC S9(4) COMP-5 VALUE 338.

       77 SQLM-ELM-HOST-PRDID
           PIC S9(4) COMP-5 VALUE 339.

       77 SQLM-ELM-OUTBOUND-APPL-ID
           PIC S9(4) COMP-5 VALUE 340.

       77 SQLM-ELM-OUTBOUND-SEQUENCE-NO
           PIC S9(4) COMP-5 VALUE 341.

       77 SQLM-ELM-DCS-APPL-STATUS
           PIC S9(4) COMP-5 VALUE 342.

       77 SQLM-ELM-HOST-CCSID
           PIC S9(4) COMP-5 VALUE 343.

       77 SQLM-ELM-OUTPUT-STATE
           PIC S9(4) COMP-5 VALUE 344.

       77 SQLM-ELM-COUNT
           PIC S9(4) COMP-5 VALUE 345.

       77 SQLM-ELM-ROWS-SELECTED
           PIC S9(4) COMP-5 VALUE 346.

       77 SQLM-ELM-SQL-STMTS
           PIC S9(4) COMP-5 VALUE 347.

       77 SQLM-ELM-FAILED-SQL-STMTS
           PIC S9(4) COMP-5 VALUE 348.

       77 SQLM-ELM-COMMIT-SQL-STMTS
           PIC S9(4) COMP-5 VALUE 349.

       77 SQLM-ELM-ROLLBACK-SQL-STMTS
           PIC S9(4) COMP-5 VALUE 350.

      * The full name of the monitor element is SQLM_ELM_INBOUND_BYTES_RECEIVED
       77 SQLM-ELM-INBOUND-BYT-RECV
           PIC S9(4) COMP-5 VALUE 351.

       77 SQLM-ELM-OUTBOUND-BYTES-SENT
           PIC S9(4) COMP-5 VALUE 352.

      * The full name of the monitor element is SQLM_ELM_OUTBOUND_BYTES_RECEIVED
       77 SQLM-ELM-OUTB-BYT-RECV
           PIC S9(4) COMP-5 VALUE 353.

       77 SQLM-ELM-INBOUND-BYTES-SENT
           PIC S9(4) COMP-5 VALUE 354.

       77 SQLM-ELM-STMT-OPERATION
           PIC S9(4) COMP-5 VALUE 355.

       77 SQLM-ELM-SECTION-NUMBER
           PIC S9(4) COMP-5 VALUE 356.

       77 SQLM-ELM-LOCK-NODE
           PIC S9(4) COMP-5 VALUE 357.

       77 SQLM-ELM-CREATOR
           PIC S9(4) COMP-5 VALUE 358.

       77 SQLM-ELM-PACKAGE-NAME
           PIC S9(4) COMP-5 VALUE 359.

       77 SQLM-ELM-APPL-IDLE-TIME
           PIC S9(4) COMP-5 VALUE 360.

       77 SQLM-ELM-OPEN-CURSORS
           PIC S9(4) COMP-5 VALUE 361.

       77 SQLM-ELM-UOW-COMP-STATUS
           PIC S9(4) COMP-5 VALUE 362.

      * The full name of the monitor element is SQLM_ELM_SEQUENCE_NO_HOLDING_LK
       77 SQLM-ELM-SEQ-NO-HOLDING-LK
           PIC S9(4) COMP-5 VALUE 363.

       77 SQLM-ELM-ROLLED-BACK-AGENT-ID
           PIC S9(4) COMP-5 VALUE 364.

       77 SQLM-ELM-ROLLED-BACK-APPL-ID
           PIC S9(4) COMP-5 VALUE 365.

      * The full name of the monitor element is SQLM_ELM_ROLLED_BACK_SEQUENCE_NO
       77 SQLM-ELM-R0L-BACK-SEQ-NO
           PIC S9(4) COMP-5 VALUE 366.

       77 SQLM-ELM-XID
           PIC S9(4) COMP-5 VALUE 367.

       77 SQLM-ELM-TPMON-CLIENT-USERID
           PIC S9(4) COMP-5 VALUE 368.

       77 SQLM-ELM-TPMON-CLIENT-WKSTN
           PIC S9(4) COMP-5 VALUE 369.

       77 SQLM-ELM-TPMON-CLIENT-APP
           PIC S9(4) COMP-5 VALUE 370.

       77 SQLM-ELM-TPMON-ACC-STR
           PIC S9(4) COMP-5 VALUE 371.

       77 SQLM-ELM-QUERY-COST-ESTIMATE
           PIC S9(4) COMP-5 VALUE 372.

       77 SQLM-ELM-QUERY-CARD-ESTIMATE
           PIC S9(4) COMP-5 VALUE 373.

       77 SQLM-ELM-FETCH-COUNT
           PIC S9(4) COMP-5 VALUE 374.

       77 SQLM-ELM-GW-TOTAL-CONS
           PIC S9(4) COMP-5 VALUE 375.

       77 SQLM-ELM-GW-CUR-CONS
           PIC S9(4) COMP-5 VALUE 376.

       77 SQLM-ELM-GW-CONS-WAIT-HOST
           PIC S9(4) COMP-5 VALUE 377.

       77 SQLM-ELM-GW-CONS-WAIT-CLIENT
           PIC S9(4) COMP-5 VALUE 378.

       77 SQLM-ELM-GW-CONNECTIONS-TOP
           PIC S9(4) COMP-5 VALUE 379.

       77 SQLM-ELM-SORT-HEAP-ALLOCATED
           PIC S9(4) COMP-5 VALUE 380.

       77 SQLM-ELM-POST-THRESHOLD-SORTS
           PIC S9(4) COMP-5 VALUE 381.

       77 SQLM-ELM-PIPED-SORTS-REQUESTED
           PIC S9(4) COMP-5 VALUE 382.

       77 SQLM-ELM-PIPED-SORTS-ACCEPTED
           PIC S9(4) COMP-5 VALUE 383.

       77 SQLM-ELM-DL-CONNS
           PIC S9(4) COMP-5 VALUE 384.

       77 SQLM-ELM-REM-CONS-IN
           PIC S9(4) COMP-5 VALUE 385.

       77 SQLM-ELM-REM-CONS-IN-EXEC
           PIC S9(4) COMP-5 VALUE 386.

       77 SQLM-ELM-LOCAL-CONS
           PIC S9(4) COMP-5 VALUE 387.

       77 SQLM-ELM-LOCAL-CONS-IN-EXEC
           PIC S9(4) COMP-5 VALUE 388.

       77 SQLM-ELM-CON-LOCAL-DBASES
           PIC S9(4) COMP-5 VALUE 389.

       77 SQLM-ELM-AGENTS-REGISTERED
           PIC S9(4) COMP-5 VALUE 390.

      * The full name of the monitor element is SQLM_ELM_AGENTS_WAITING_ON_TOKEN
       77 SQLM-ELM-AGNT-WAIT-ON-TOKEN
           PIC S9(4) COMP-5 VALUE 391.

       77 SQLM-ELM-DB2-STATUS
           PIC S9(4) COMP-5 VALUE 392.

       77 SQLM-ELM-AGENTS-REGISTERED-TOP
           PIC S9(4) COMP-5 VALUE 393.

       77 SQLM-ELM-AGENTS-WAITING-TOP
           PIC S9(4) COMP-5 VALUE 394.

       77 SQLM-ELM-COMM-PRIVATE-MEM
           PIC S9(4) COMP-5 VALUE 395.

       77 SQLM-ELM-IDLE-AGENTS
           PIC S9(4) COMP-5 VALUE 396.

       77 SQLM-ELM-AGENTS-FROM-POOL
           PIC S9(4) COMP-5 VALUE 397.

      * The full name of the monitor element is SQLM_ELM_AGENTS_CREATED_EMPTY_POOL
       77 SQLM-ELM-AGNT-CREATED-EMPTY-PL
           PIC S9(4) COMP-5 VALUE 398.

       77 SQLM-ELM-AGENTS-TOP
           PIC S9(4) COMP-5 VALUE 399.

       77 SQLM-ELM-COORD-AGENTS-TOP
           PIC S9(4) COMP-5 VALUE 400.

       77 SQLM-ELM-MAX-AGENT-OVERFLOWS
           PIC S9(4) COMP-5 VALUE 401.

       77 SQLM-ELM-AGENTS-STOLEN
           PIC S9(4) COMP-5 VALUE 402.

       77 SQLM-ELM-PRODUCT-NAME
           PIC S9(4) COMP-5 VALUE 403.

       77 SQLM-ELM-COMPONENT-ID
           PIC S9(4) COMP-5 VALUE 404.

       77 SQLM-ELM-SERVICE-LEVEL
           PIC S9(4) COMP-5 VALUE 405.

      * The full name of the monitor element is SQLM_ELM_POST_THRESHOLD_HASH_JOINS
       77 SQLM-ELM-POST-THRE-HASH-JOINS
           PIC S9(4) COMP-5 VALUE 406.

       77 SQLM-ELM-BUFF-FREE
           PIC S9(4) COMP-5 VALUE 407.

       77 SQLM-ELM-BUFF-FREE-BOTTOM
           PIC S9(4) COMP-5 VALUE 408.

       77 SQLM-ELM-MA-FREE
           PIC S9(4) COMP-5 VALUE 409.

       77 SQLM-ELM-MA-FREE-BOTTOM
           PIC S9(4) COMP-5 VALUE 410.

       77 SQLM-ELM-CE-FREE
           PIC S9(4) COMP-5 VALUE 411.

       77 SQLM-ELM-CE-FREE-BOTTOM
           PIC S9(4) COMP-5 VALUE 412.

       77 SQLM-ELM-RB-FREE
           PIC S9(4) COMP-5 VALUE 413.

       77 SQLM-ELM-RB-FREE-BOTTOM
           PIC S9(4) COMP-5 VALUE 414.

       77 SQLM-ELM-CONNECTION-STATUS
           PIC S9(4) COMP-5 VALUE 416.

       77 SQLM-ELM-TOTAL-BUFFERS-SENT
           PIC S9(4) COMP-5 VALUE 417.

       77 SQLM-ELM-TOTAL-BUFFERS-RCVD
           PIC S9(4) COMP-5 VALUE 418.

       77 SQLM-ELM-LOCKS-HELD
           PIC S9(4) COMP-5 VALUE 419.

       77 SQLM-ELM-LOCK-WAITS
           PIC S9(4) COMP-5 VALUE 420.

       77 SQLM-ELM-LOCK-WAIT-TIME
           PIC S9(4) COMP-5 VALUE 421.

       77 SQLM-ELM-LOCK-LIST-IN-USE
           PIC S9(4) COMP-5 VALUE 422.

       77 SQLM-ELM-DEADLOCKS
           PIC S9(4) COMP-5 VALUE 423.

       77 SQLM-ELM-LOCK-ESCALS
           PIC S9(4) COMP-5 VALUE 424.

       77 SQLM-ELM-X-LOCK-ESCALS
           PIC S9(4) COMP-5 VALUE 425.

       77 SQLM-ELM-LOCKS-WAITING
           PIC S9(4) COMP-5 VALUE 426.

       77 SQLM-ELM-TOTAL-SORTS
           PIC S9(4) COMP-5 VALUE 427.

       77 SQLM-ELM-TOTAL-SORT-TIME
           PIC S9(4) COMP-5 VALUE 428.

       77 SQLM-ELM-SORT-OVERFLOWS
           PIC S9(4) COMP-5 VALUE 429.

       77 SQLM-ELM-ACTIVE-SORTS
           PIC S9(4) COMP-5 VALUE 430.

       77 SQLM-ELM-POOL-DATA-L-READS
           PIC S9(4) COMP-5 VALUE 431.

       77 SQLM-ELM-POOL-DATA-P-READS
           PIC S9(4) COMP-5 VALUE 432.

       77 SQLM-ELM-POOL-DATA-WRITES
           PIC S9(4) COMP-5 VALUE 433.

       77 SQLM-ELM-POOL-INDEX-L-READS
           PIC S9(4) COMP-5 VALUE 434.

       77 SQLM-ELM-POOL-INDEX-P-READS
           PIC S9(4) COMP-5 VALUE 435.

       77 SQLM-ELM-POOL-INDEX-WRITES
           PIC S9(4) COMP-5 VALUE 436.

       77 SQLM-ELM-POOL-READ-TIME
           PIC S9(4) COMP-5 VALUE 437.

       77 SQLM-ELM-POOL-WRITE-TIME
           PIC S9(4) COMP-5 VALUE 438.

       77 SQLM-ELM-FILES-CLOSED
           PIC S9(4) COMP-5 VALUE 439.

       77 SQLM-ELM-DYNAMIC-SQL-STMTS
           PIC S9(4) COMP-5 VALUE 440.

       77 SQLM-ELM-STATIC-SQL-STMTS
           PIC S9(4) COMP-5 VALUE 441.

       77 SQLM-ELM-SELECT-SQL-STMTS
           PIC S9(4) COMP-5 VALUE 442.

       77 SQLM-ELM-DDL-SQL-STMTS
           PIC S9(4) COMP-5 VALUE 443.

       77 SQLM-ELM-UID-SQL-STMTS
           PIC S9(4) COMP-5 VALUE 444.

       77 SQLM-ELM-INT-AUTO-REBINDS
           PIC S9(4) COMP-5 VALUE 445.

       77 SQLM-ELM-INT-ROWS-DELETED
           PIC S9(4) COMP-5 VALUE 446.

       77 SQLM-ELM-INT-ROWS-UPDATED
           PIC S9(4) COMP-5 VALUE 447.

       77 SQLM-ELM-INT-COMMITS
           PIC S9(4) COMP-5 VALUE 448.

       77 SQLM-ELM-INT-ROLLBACKS
           PIC S9(4) COMP-5 VALUE 449.

      * The full name of the monitor element is SQLM_ELM_INT_DEADLOCK_ROLLBACKS
       77 SQLM-ELM-INT-DEADLOCK-RBCK
           PIC S9(4) COMP-5 VALUE 450.

       77 SQLM-ELM-ROWS-DELETED
           PIC S9(4) COMP-5 VALUE 451.

       77 SQLM-ELM-ROWS-INSERTED
           PIC S9(4) COMP-5 VALUE 452.

       77 SQLM-ELM-ROWS-UPDATED
           PIC S9(4) COMP-5 VALUE 453.

       77 SQLM-ELM-BINDS-PRECOMPILES
           PIC S9(4) COMP-5 VALUE 454.

       77 SQLM-ELM-LOCKS-HELD-TOP
           PIC S9(4) COMP-5 VALUE 455.

      * The full name of the monitor element is SQLM_ELM_NUM_NODES_IN_DB2_INSTANCE
       77 SQLM-ELM-NUM-NODE-IN-DB2-INST
           PIC S9(4) COMP-5 VALUE 456.

       77 SQLM-ELM-TOTAL-CONS
           PIC S9(4) COMP-5 VALUE 457.

       77 SQLM-ELM-APPLS-CUR-CONS
           PIC S9(4) COMP-5 VALUE 458.

       77 SQLM-ELM-APPLS-IN-DB2
           PIC S9(4) COMP-5 VALUE 459.

       77 SQLM-ELM-SEC-LOG-USED-TOP
           PIC S9(4) COMP-5 VALUE 460.

       77 SQLM-ELM-TOT-LOG-USED-TOP
           PIC S9(4) COMP-5 VALUE 461.

       77 SQLM-ELM-SEC-LOGS-ALLOCATED
           PIC S9(4) COMP-5 VALUE 462.

      * The full name of the monitor element is SQLM_ELM_POOL_ASYNC_INDEX_READS
       77 SQLM-ELM-PL-ASYNC-IDX-RD
           PIC S9(4) COMP-5 VALUE 463.

       77 SQLM-ELM-POOL-DATA-TO-ESTORE
           PIC S9(4) COMP-5 VALUE 464.

       77 SQLM-ELM-POOL-INDEX-TO-ESTORE
           PIC S9(4) COMP-5 VALUE 465.

      * The full name of the monitor element is SQLM_ELM_POOL_INDEX_FROM_ESTORE
       77 SQLM-ELM-PL-IDX-FROM-ESTORE
           PIC S9(4) COMP-5 VALUE 466.

       77 SQLM-ELM-POOL-DATA-FROM-ESTORE
           PIC S9(4) COMP-5 VALUE 467.

       77 SQLM-ELM-DB-STATUS
           PIC S9(4) COMP-5 VALUE 468.

       77 SQLM-ELM-LOCK-TIMEOUTS
           PIC S9(4) COMP-5 VALUE 469.

       77 SQLM-ELM-CONNECTIONS-TOP
           PIC S9(4) COMP-5 VALUE 470.

       77 SQLM-ELM-DB-HEAP-TOP
           PIC S9(4) COMP-5 VALUE 471.

       77 SQLM-ELM-POOL-ASYNC-DATA-READS
           PIC S9(4) COMP-5 VALUE 472.

      * The full name of the monitor element is SQLM_ELM_POOL_ASYNC_DATA_WRITES
       77 SQLM-ELM-PL-ASYNC-DATA-WR
           PIC S9(4) COMP-5 VALUE 473.

      * The full name of the monitor element is SQLM_ELM_POOL_ASYNC_INDEX_WRITES
       77 SQLM-ELM-PL-ASYNC-IDX-WR
           PIC S9(4) COMP-5 VALUE 474.

       77 SQLM-ELM-POOL-ASYNC-READ-TIME
           PIC S9(4) COMP-5 VALUE 475.

       77 SQLM-ELM-POOL-ASYNC-WRITE-TIME
           PIC S9(4) COMP-5 VALUE 476.

      * The full name of the monitor element is SQLM_ELM_POOL_ASYNC_DATA_READ_REQS
       77 SQLM-ELM-PL-ASYNC-DATA-RD-REQS
           PIC S9(4) COMP-5 VALUE 477.

       77 SQLM-ELM-POOL-LSN-GAP-CLNS
           PIC S9(4) COMP-5 VALUE 478.

      * The full name of the monitor element is SQLM_ELM_POOL_DRTY_PG_STEAL_CLNS
       77 SQLM-ELM-PL-DRTY-PG-STEAL-CLNS
           PIC S9(4) COMP-5 VALUE 479.

      * The full name of the monitor element is SQLM_ELM_POOL_DRTY_PG_THRSH_CLNS
       77 SQLM-ELM-PL-DRTY-PG-THRSH-CLNS
           PIC S9(4) COMP-5 VALUE 480.

       77 SQLM-ELM-DIRECT-READS
           PIC S9(4) COMP-5 VALUE 481.

       77 SQLM-ELM-DIRECT-WRITES
           PIC S9(4) COMP-5 VALUE 482.

       77 SQLM-ELM-DIRECT-READ-REQS
           PIC S9(4) COMP-5 VALUE 483.

       77 SQLM-ELM-DIRECT-WRITE-REQS
           PIC S9(4) COMP-5 VALUE 484.

       77 SQLM-ELM-DIRECT-READ-TIME
           PIC S9(4) COMP-5 VALUE 485.

       77 SQLM-ELM-DIRECT-WRITE-TIME
           PIC S9(4) COMP-5 VALUE 486.

       77 SQLM-ELM-INT-ROWS-INSERTED
           PIC S9(4) COMP-5 VALUE 487.

       77 SQLM-ELM-LOG-READS
           PIC S9(4) COMP-5 VALUE 488.

       77 SQLM-ELM-LOG-WRITES
           PIC S9(4) COMP-5 VALUE 489.

       77 SQLM-ELM-PKG-CACHE-LOOKUPS
           PIC S9(4) COMP-5 VALUE 490.

       77 SQLM-ELM-PKG-CACHE-INSERTS
           PIC S9(4) COMP-5 VALUE 491.

       77 SQLM-ELM-CAT-CACHE-LOOKUPS
           PIC S9(4) COMP-5 VALUE 492.

       77 SQLM-ELM-CAT-CACHE-INSERTS
           PIC S9(4) COMP-5 VALUE 493.

       77 SQLM-ELM-CAT-CACHE-OVERFLOWS
           PIC S9(4) COMP-5 VALUE 494.

       77 SQLM-ELM-CAT-CACHE-HEAP-FULL
           PIC S9(4) COMP-5 VALUE 495.

       77 SQLM-ELM-CATALOG-NODE
           PIC S9(4) COMP-5 VALUE 496.

       77 SQLM-ELM-TOTAL-SEC-CONS
           PIC S9(4) COMP-5 VALUE 497.

       77 SQLM-ELM-DB-LOCATION
           PIC S9(4) COMP-5 VALUE 498.

       77 SQLM-ELM-SERVER-PLATFORM
           PIC S9(4) COMP-5 VALUE 499.

       77 SQLM-ELM-CATALOG-NODE-NAME
           PIC S9(4) COMP-5 VALUE 500.

       77 SQLM-ELM-PREFETCH-WAIT-TIME
           PIC S9(4) COMP-5 VALUE 501.

       77 SQLM-ELM-APPL-SECTION-LOOKUPS
           PIC S9(4) COMP-5 VALUE 502.

       77 SQLM-ELM-APPL-SECTION-INSERTS
           PIC S9(4) COMP-5 VALUE 503.

       77 SQLM-ELM-TOTAL-HASH-JOINS
           PIC S9(4) COMP-5 VALUE 504.

       77 SQLM-ELM-TOTAL-HASH-LOOPS
           PIC S9(4) COMP-5 VALUE 505.

       77 SQLM-ELM-HASH-JOIN-OVERFLOWS
           PIC S9(4) COMP-5 VALUE 506.

      * The full name of the monitor element is SQLM_ELM_HASH_JOIN_SMALL_OVERFLOWS
       77 SQLM-ELM-HASH-JOIN-SMALL-OWFLW
           PIC S9(4) COMP-5 VALUE 507.

       77 SQLM-ELM-UOW-LOCK-WAIT-TIME
           PIC S9(4) COMP-5 VALUE 508.

       77 SQLM-ELM-STMT-TYPE
           PIC S9(4) COMP-5 VALUE 509.

       77 SQLM-ELM-CURSOR-NAME
           PIC S9(4) COMP-5 VALUE 510.

       77 SQLM-ELM-UOW-LOG-SPACE-USED
           PIC S9(4) COMP-5 VALUE 511.

       77 SQLM-ELM-OPEN-REM-CURS
           PIC S9(4) COMP-5 VALUE 512.

       77 SQLM-ELM-OPEN-REM-CURS-BLK
           PIC S9(4) COMP-5 VALUE 513.

       77 SQLM-ELM-REJ-CURS-BLK
           PIC S9(4) COMP-5 VALUE 514.

       77 SQLM-ELM-ACC-CURS-BLK
           PIC S9(4) COMP-5 VALUE 515.

       77 SQLM-ELM-VERSION
           PIC S9(4) COMP-5 VALUE 516.

       77 SQLM-ELM-EVENT-MONITOR-NAME
           PIC S9(4) COMP-5 VALUE 517.

       77 SQLM-ELM-SQL-REQS-SINCE-COMMIT
           PIC S9(4) COMP-5 VALUE 518.

       77 SQLM-ELM-BYTE-ORDER
           PIC S9(4) COMP-5 VALUE 520.

       77 SQLM-ELM-PREP-TIME-WORST
           PIC S9(4) COMP-5 VALUE 521.

       77 SQLM-ELM-ROWS-READ
           PIC S9(4) COMP-5 VALUE 522.

       77 SQLM-ELM-ROWS-WRITTEN
           PIC S9(4) COMP-5 VALUE 523.

       77 SQLM-ELM-OPEN-LOC-CURS
           PIC S9(4) COMP-5 VALUE 524.

       77 SQLM-ELM-OPEN-LOC-CURS-BLK
           PIC S9(4) COMP-5 VALUE 525.

       77 SQLM-ELM-COORD-NODE
           PIC S9(4) COMP-5 VALUE 526.

       77 SQLM-ELM-NUM-AGENTS
           PIC S9(4) COMP-5 VALUE 527.

       77 SQLM-ELM-ASSOCIATED-AGENTS-TOP
           PIC S9(4) COMP-5 VALUE 528.

       77 SQLM-ELM-APPL-PRIORITY
           PIC S9(4) COMP-5 VALUE 529.

       77 SQLM-ELM-APPL-PRIORITY-TYPE
           PIC S9(4) COMP-5 VALUE 530.

       77 SQLM-ELM-DEGREE-PARALLELISM
           PIC S9(4) COMP-5 VALUE 531.

       77 SQLM-ELM-STMT-SORTS
           PIC S9(4) COMP-5 VALUE 532.

       77 SQLM-ELM-STMT-USR-CPU-TIME
           PIC S9(4) COMP-5 VALUE 533.

       77 SQLM-ELM-STMT-SYS-CPU-TIME
           PIC S9(4) COMP-5 VALUE 534.

       77 SQLM-ELM-SS-NUMBER
           PIC S9(4) COMP-5 VALUE 535.

       77 SQLM-ELM-SS-STATUS
           PIC S9(4) COMP-5 VALUE 536.

       77 SQLM-ELM-SS-NODE-NUMBER
           PIC S9(4) COMP-5 VALUE 537.

       77 SQLM-ELM-SS-EXEC-TIME
           PIC S9(4) COMP-5 VALUE 538.

       77 SQLM-ELM-PREP-TIME-BEST
           PIC S9(4) COMP-5 VALUE 539.

       77 SQLM-ELM-NUM-COMPILATIONS
           PIC S9(4) COMP-5 VALUE 540.

       77 SQLM-ELM-TQ-NODE-WAITED-FOR
           PIC S9(4) COMP-5 VALUE 541.

       77 SQLM-ELM-TQ-WAIT-FOR-ANY
           PIC S9(4) COMP-5 VALUE 542.

       77 SQLM-ELM-TQ-ID-WAITING-ON
           PIC S9(4) COMP-5 VALUE 543.

       77 SQLM-ELM-TQ-TOT-SEND-SPILLS
           PIC S9(4) COMP-5 VALUE 544.

       77 SQLM-ELM-TQ-CUR-SEND-SPILLS
           PIC S9(4) COMP-5 VALUE 545.

       77 SQLM-ELM-TQ-MAX-SEND-SPILLS
           PIC S9(4) COMP-5 VALUE 546.

       77 SQLM-ELM-TQ-ROWS-READ
           PIC S9(4) COMP-5 VALUE 547.

       77 SQLM-ELM-TQ-ROWS-WRITTEN
           PIC S9(4) COMP-5 VALUE 548.

       77 SQLM-ELM-AGENT-PID
           PIC S9(4) COMP-5 VALUE 549.

       77 SQLM-ELM-LOCK-ESCALATION
           PIC S9(4) COMP-5 VALUE 550.

       77 SQLM-ELM-SUBSECTION-NUMBER
           PIC S9(4) COMP-5 VALUE 551.

       77 SQLM-ELM-LOCK-MODE
           PIC S9(4) COMP-5 VALUE 552.

       77 SQLM-ELM-LOCK-OBJECT-TYPE
           PIC S9(4) COMP-5 VALUE 553.

       77 SQLM-ELM-NUM-EXECUTIONS
           PIC S9(4) COMP-5 VALUE 554.

       77 SQLM-ELM-TABLE-NAME
           PIC S9(4) COMP-5 VALUE 555.

       77 SQLM-ELM-TABLE-SCHEMA
           PIC S9(4) COMP-5 VALUE 556.

       77 SQLM-ELM-TABLESPACE-NAME
           PIC S9(4) COMP-5 VALUE 557.

       77 SQLM-ELM-AGENT-ID-HOLDING-LK
           PIC S9(4) COMP-5 VALUE 558.

       77 SQLM-ELM-APPL-ID-HOLDING-LK
           PIC S9(4) COMP-5 VALUE 559.

       77 SQLM-ELM-TABLE-FILE-ID
           PIC S9(4) COMP-5 VALUE 561.

       77 SQLM-ELM-TABLE-TYPE
           PIC S9(4) COMP-5 VALUE 562.

       77 SQLM-ELM-OVERFLOW-ACCESSES
           PIC S9(4) COMP-5 VALUE 563.

       77 SQLM-ELM-PAGE-REORGS
           PIC S9(4) COMP-5 VALUE 564.

       77 SQLM-ELM-SQLCABC
           PIC S9(4) COMP-5 VALUE 565.

       77 SQLM-ELM-LOCK-STATUS
           PIC S9(4) COMP-5 VALUE 566.

       77 SQLM-ELM-LOCK-OBJECT-NAME
           PIC S9(4) COMP-5 VALUE 567.

       77 SQLM-ELM-RF-TYPE
           PIC S9(4) COMP-5 VALUE 568.

       77 SQLM-ELM-RF-LOG-NUM
           PIC S9(4) COMP-5 VALUE 569.

       77 SQLM-ELM-RF-STATUS
           PIC S9(4) COMP-5 VALUE 570.

       77 SQLM-ELM-TS-NAME
           PIC S9(4) COMP-5 VALUE 571.

       77 SQLM-ELM-BP-NAME
           PIC S9(4) COMP-5 VALUE 572.

       77 SQLM-ELM-STMT-NODE-NUMBER
           PIC S9(4) COMP-5 VALUE 573.

       77 SQLM-ELM-PARTIAL-RECORD
           PIC S9(4) COMP-5 VALUE 574.

       77 SQLM-ELM-SYSTEM-CPU-TIME
           PIC S9(4) COMP-5 VALUE 575.

       77 SQLM-ELM-SQLCA
           PIC S9(4) COMP-5 VALUE 576.

       77 SQLM-ELM-SQLCODE
           PIC S9(4) COMP-5 VALUE 577.

       77 SQLM-ELM-SQLERRML
           PIC S9(4) COMP-5 VALUE 578.

       77 SQLM-ELM-SQLERRMC
           PIC S9(4) COMP-5 VALUE 579.

       77 SQLM-ELM-SQLERRP
           PIC S9(4) COMP-5 VALUE 580.

       77 SQLM-ELM-SQLERRD
           PIC S9(4) COMP-5 VALUE 581.

       77 SQLM-ELM-SQLWARN
           PIC S9(4) COMP-5 VALUE 582.

       77 SQLM-ELM-SQLSTATE
           PIC S9(4) COMP-5 VALUE 583.

       77 SQLM-ELM-UOW-STATUS
           PIC S9(4) COMP-5 VALUE 584.

       77 SQLM-ELM-TOTAL-SYS-CPU-TIME
           PIC S9(4) COMP-5 VALUE 585.

       77 SQLM-ELM-TOTAL-USR-CPU-TIME
           PIC S9(4) COMP-5 VALUE 586.

       77 SQLM-ELM-LOCK-MODE-REQUESTED
           PIC S9(4) COMP-5 VALUE 587.

       77 SQLM-ELM-INACTIVE-GW-AGENTS
           PIC S9(4) COMP-5 VALUE 588.

       77 SQLM-ELM-NUM-GW-CONN-SWITCHES
           PIC S9(4) COMP-5 VALUE 589.

       77 SQLM-ELM-GW-COMM-ERRORS
           PIC S9(4) COMP-5 VALUE 590.

       77 SQLM-ELM-GW-COMM-ERROR-TIME
           PIC S9(4) COMP-5 VALUE 591.

       77 SQLM-ELM-GW-CON-START-TIME
           PIC S9(4) COMP-5 VALUE 592.

       77 SQLM-ELM-CON-RESPONSE-TIME
           PIC S9(4) COMP-5 VALUE 593.

       77 SQLM-ELM-CON-ELAPSED-TIME
           PIC S9(4) COMP-5 VALUE 594.

       77 SQLM-ELM-HOST-RESPONSE-TIME
           PIC S9(4) COMP-5 VALUE 595.

      * The full name of the monitor element is SQLM_ELM_PKG_CACHE_NUM_OVERFLOWS
       77 SQLM-ELM-PKG-CACHE-NUM-OWFLW
           PIC S9(4) COMP-5 VALUE 596.

       77 SQLM-ELM-PKG-CACHE-SIZE-TOP
           PIC S9(4) COMP-5 VALUE 597.

       77 SQLM-ELM-APPL-ID-OLDEST-XACT
           PIC S9(4) COMP-5 VALUE 598.

       77 SQLM-ELM-TOTAL-LOG-USED
           PIC S9(4) COMP-5 VALUE 599.

       77 SQLM-ELM-TOTAL-LOG-AVAILABLE
           PIC S9(4) COMP-5 VALUE 600.

       77 SQLM-ELM-STMT-ELAPSED-TIME
           PIC S9(4) COMP-5 VALUE 601.

       77 SQLM-ELM-UOW-ELAPSED-TIME
           PIC S9(4) COMP-5 VALUE 602.

       77 SQLM-ELM-SQLCAID
           PIC S9(4) COMP-5 VALUE 603.

      * The full name of the monitor element is SQLM_ELM_SMALLEST_LOG_AVAIL_NODE
       77 SQLM-ELM-SMALLEST-LOG-AVL-NODE
           PIC S9(4) COMP-5 VALUE 604.

       77 SQLM-ELM-DISCONNECTS
           PIC S9(4) COMP-5 VALUE 605.

       77 SQLM-ELM-CREATE-NICKNAME
           PIC S9(4) COMP-5 VALUE 606.

       77 SQLM-ELM-PASSTHRUS
           PIC S9(4) COMP-5 VALUE 607.

       77 SQLM-ELM-STORED-PROCS
           PIC S9(4) COMP-5 VALUE 608.

       77 SQLM-ELM-SP-ROWS-SELECTED
           PIC S9(4) COMP-5 VALUE 609.

       77 SQLM-ELM-DATASOURCE-NAME
           PIC S9(4) COMP-5 VALUE 610.

       77 SQLM-ELM-REMOTE-LOCKS
           PIC S9(4) COMP-5 VALUE 611.

       77 SQLM-ELM-BLOCKING-CURSOR
           PIC S9(4) COMP-5 VALUE 612.

      * The full name of the monitor element is SQLM_ELM_OUTBOUND_BLOCKING_CURSOR
       77 SQLM-ELM-OUTB-BLOCKING-CURSOR
           PIC S9(4) COMP-5 VALUE 613.

       77 SQLM-ELM-INSERT-SQL-STMTS
           PIC S9(4) COMP-5 VALUE 614.

       77 SQLM-ELM-UPDATE-SQL-STMTS
           PIC S9(4) COMP-5 VALUE 615.

       77 SQLM-ELM-DELETE-SQL-STMTS
           PIC S9(4) COMP-5 VALUE 616.

       77 SQLM-ELM-UNREAD-PREFETCH-PAGES
           PIC S9(4) COMP-5 VALUE 617.

       77 SQLM-ELM-AGENT-STATUS
           PIC S9(4) COMP-5 VALUE 618.

       77 SQLM-ELM-NUM-TRANSMISSIONS
           PIC S9(4) COMP-5 VALUE 619.

      * The full name of the monitor element is SQLM_ELM_OUTBOUND_BYTES_SENT_TOP
       77 SQLM-ELM-OUTB-BYT-SENT-TOP
           PIC S9(4) COMP-5 VALUE 620.

      * The full name of the monitor element is SQLM_ELM_OUTBOUND_BYTES_RECEIVED_TOP
       77 SQLM-ELM-OUTB-BYT-RECV-TOP
           PIC S9(4) COMP-5 VALUE 621.

      * The full name of the monitor element is SQLM_ELM_OUTBOUND_BYTES_SENT_BOTTOM
       77 SQLM-ELM-OUTB-BYT-SENT-BOT
           PIC S9(4) COMP-5 VALUE 622.

      * The full name of the monitor element is SQLM_ELM_OUTBOUND_BYTES_RECEIVED_BOTTOM
       77 SQLM-ELM-OUTB-BYT-RECV-BOT
           PIC S9(4) COMP-5 VALUE 623.


      *  The following elements represent counts of the number of transmissions    *
      *  whose size in bytes is less than or equal to the number in the name.      *
       77 SQLM-ELM-MAX-DATA-SENT-128
           PIC S9(4) COMP-5 VALUE 624.

       77 SQLM-ELM-MAX-DATA-SENT-256
           PIC S9(4) COMP-5 VALUE 625.

       77 SQLM-ELM-MAX-DATA-SENT-512
           PIC S9(4) COMP-5 VALUE 626.

       77 SQLM-ELM-MAX-DATA-SENT-1024
           PIC S9(4) COMP-5 VALUE 627.

       77 SQLM-ELM-MAX-DATA-SENT-2048
           PIC S9(4) COMP-5 VALUE 628.

       77 SQLM-ELM-MAX-DATA-SENT-4096
           PIC S9(4) COMP-5 VALUE 629.

       77 SQLM-ELM-MAX-DATA-SENT-8192
           PIC S9(4) COMP-5 VALUE 630.

       77 SQLM-ELM-MAX-DATA-SENT-16384
           PIC S9(4) COMP-5 VALUE 631.

       77 SQLM-ELM-MAX-DATA-SENT-31999
           PIC S9(4) COMP-5 VALUE 632.

       77 SQLM-ELM-MAX-DATA-SENT-64000
           PIC S9(4) COMP-5 VALUE 633.

       77 SQLM-ELM-MAX-DATA-SENT-GT64000
           PIC S9(4) COMP-5 VALUE 634.

       77 SQLM-ELM-MAX-DATA-RECEIVED-128
           PIC S9(4) COMP-5 VALUE 635.

       77 SQLM-ELM-MAX-DATA-RECEIVED-256
           PIC S9(4) COMP-5 VALUE 636.

       77 SQLM-ELM-MAX-DATA-RECEIVED-512
           PIC S9(4) COMP-5 VALUE 637.

      * The full name of the monitor element is SQLM_ELM_MAX_DATA_RECEIVED_1024
       77 SQLM-ELM-MAX-DATA-RECV-1024
           PIC S9(4) COMP-5 VALUE 638.

      * The full name of the monitor element is SQLM_ELM_MAX_DATA_RECEIVED_2048
       77 SQLM-ELM-MAX-DATA-RECV-2048
           PIC S9(4) COMP-5 VALUE 639.

      * The full name of the monitor element is SQLM_ELM_MAX_DATA_RECEIVED_4096
       77 SQLM-ELM-MAX-DATA-RECV-4096
           PIC S9(4) COMP-5 VALUE 640.

      * The full name of the monitor element is SQLM_ELM_MAX_DATA_RECEIVED_8192
       77 SQLM-ELM-MAX-DATA-RECV-8192
           PIC S9(4) COMP-5 VALUE 641.

      * The full name of the monitor element is SQLM_ELM_MAX_DATA_RECEIVED_16384
       77 SQLM-ELM-MAX-DATA-RECV-16384
           PIC S9(4) COMP-5 VALUE 642.

      * The full name of the monitor element is SQLM_ELM_MAX_DATA_RECEIVED_31999
       77 SQLM-ELM-MAX-DATA-RECV-31999
           PIC S9(4) COMP-5 VALUE 643.

      * The full name of the monitor element is SQLM_ELM_MAX_DATA_RECEIVED_64000
       77 SQLM-ELM-MAX-DATA-RECV-64000
           PIC S9(4) COMP-5 VALUE 644.

      * The full name of the monitor element is SQLM_ELM_MAX_DATA_RECEIVED_GT64000
       77 SQLM-ELM-MAX-DATA-RECV-GT64000
           PIC S9(4) COMP-5 VALUE 645.


      *  The following elements represent counts of the number of transmissions    *
      *  whose network transmission time is less than the time (in milliseconds)   *
      *  in the name.                                                              *
       77 SQLM-ELM-MAX-TIME-2-MS
           PIC S9(4) COMP-5 VALUE 646.

       77 SQLM-ELM-MAX-TIME-4-MS
           PIC S9(4) COMP-5 VALUE 647.

       77 SQLM-ELM-MAX-TIME-8-MS
           PIC S9(4) COMP-5 VALUE 648.

       77 SQLM-ELM-MAX-TIME-16-MS
           PIC S9(4) COMP-5 VALUE 649.

       77 SQLM-ELM-MAX-TIME-32-MS
           PIC S9(4) COMP-5 VALUE 650.

       77 SQLM-ELM-MAX-TIME-GT32-MS
           PIC S9(4) COMP-5 VALUE 651.


      * Deadlock Identification elements.                                          *
       77 SQLM-ELM-DEADLOCK-ID
           PIC S9(4) COMP-5 VALUE 652.

       77 SQLM-ELM-DEADLOCK-NODE
           PIC S9(4) COMP-5 VALUE 653.


      * Deadlock Participant numbers                                               *
       77 SQLM-ELM-PARTICIPANT-NO
           PIC S9(4) COMP-5 VALUE 654.

      * The full name of the monitor element is SQLM_ELM_PARTICIPANT_NO_HOLDING_LK
       77 SQLM-ELM-PARTCP-NO-HOLDING-LK
           PIC S9(4) COMP-5 VALUE 655.

      * The full name of the monitor element is SQLM_ELM_ROLLED_BACK_PARTICIPANT_NO
       77 SQLM-ELM-R0L-BACK-PARTCP-NO
           PIC S9(4) COMP-5 VALUE 656.


      * Please make these 6 element always consecutive, sqlmetart.C relys on this  *
       77 SQLM-ELM-SQLERRD1
           PIC S9(4) COMP-5 VALUE 657.

       77 SQLM-ELM-SQLERRD2
           PIC S9(4) COMP-5 VALUE 658.

       77 SQLM-ELM-SQLERRD3
           PIC S9(4) COMP-5 VALUE 659.

       77 SQLM-ELM-SQLERRD4
           PIC S9(4) COMP-5 VALUE 660.

       77 SQLM-ELM-SQLERRD5
           PIC S9(4) COMP-5 VALUE 661.

       77 SQLM-ELM-SQLERRD6
           PIC S9(4) COMP-5 VALUE 662.


      * Event monitor new elements added  *
       77 SQLM-ELM-EVMON-ACTIVATES
           PIC S9(4) COMP-5 VALUE 663.

       77 SQLM-ELM-EVMON-FLUSHES
           PIC S9(4) COMP-5 VALUE 664.

       77 SQLM-ELM-SQL-REQ-ID
           PIC S9(4) COMP-5 VALUE 665.


      * New elements added for WTT control table                                    *
       77 SQLM-ELM-MESSAGE
           PIC S9(4) COMP-5 VALUE 666.

       77 SQLM-ELM-MESSAGE-TIME
           PIC S9(4) COMP-5 VALUE 667.


      * Elements for dynamic bufferpool                                            *
       77 SQLM-ELM-VECTORED-IOS
           PIC S9(4) COMP-5 VALUE 668.

      * The full name of the monitor element is SQLM_ELM_PAGES_FROM_VECTORED_IOS
       77 SQLM-ELM-PG-FROM-VECT-IOS
           PIC S9(4) COMP-5 VALUE 669.

       77 SQLM-ELM-BLOCK-IOS
           PIC S9(4) COMP-5 VALUE 670.

       77 SQLM-ELM-PAGES-FROM-BLOCK-IOS
           PIC S9(4) COMP-5 VALUE 671.

       77 SQLM-ELM-PHYSICAL-PAGE-MAPS
           PIC S9(4) COMP-5 VALUE 672.


      * Specifies the number of locks in the detailed dlconn lock list             *
       77 SQLM-ELM-LOCKS-IN-LIST
           PIC S9(4) COMP-5 VALUE 673.


      * Elements for table reorg                                                   *
       77 SQLM-ELM-REORG-PHASE
           PIC S9(4) COMP-5 VALUE 674.

       77 SQLM-ELM-REORG-MAX-PHASE
           PIC S9(4) COMP-5 VALUE 675.

       77 SQLM-ELM-REORG-CURRENT-COUNTER
           PIC S9(4) COMP-5 VALUE 676.

       77 SQLM-ELM-REORG-MAX-COUNTER
           PIC S9(4) COMP-5 VALUE 677.

       77 SQLM-ELM-REORG-TYPE
           PIC S9(4) COMP-5 VALUE 678.

       77 SQLM-ELM-REORG-STATUS
           PIC S9(4) COMP-5 VALUE 679.

       77 SQLM-ELM-REORG-COMPLETION
           PIC S9(4) COMP-5 VALUE 680.

       77 SQLM-ELM-REORG-START
           PIC S9(4) COMP-5 VALUE 681.

       77 SQLM-ELM-REORG-END
           PIC S9(4) COMP-5 VALUE 682.

       77 SQLM-ELM-REORG-PHASE-START
           PIC S9(4) COMP-5 VALUE 683.

       77 SQLM-ELM-REORG-INDEX-ID
           PIC S9(4) COMP-5 VALUE 684.

       77 SQLM-ELM-REORG-TBSPC-ID
           PIC S9(4) COMP-5 VALUE 685.


      * Pool memory elements                                                        *
       77 SQLM-ELM-POOL-ID
           PIC S9(4) COMP-5 VALUE 686.

       77 SQLM-ELM-POOL-CUR-SIZE
           PIC S9(4) COMP-5 VALUE 687.

       77 SQLM-ELM-POOL-CONFIG-SIZE
           PIC S9(4) COMP-5 VALUE 688.

       77 SQLM-ELM-POOL-MAX-SIZE
           PIC S9(4) COMP-5 VALUE 688.

       77 SQLM-ELM-POOL-WATERMARK
           PIC S9(4) COMP-5 VALUE 689.


      * Elements for tablespace containers                                         *
       77 SQLM-ELM-TABLESPACE-ID
           PIC S9(4) COMP-5 VALUE 690.

       77 SQLM-ELM-TABLESPACE-TYPE
           PIC S9(4) COMP-5 VALUE 691.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_CONTENT_TYPE
       77 SQLM-ELM-TBSP-CONTENT-TYPE
           PIC S9(4) COMP-5 VALUE 692.

       77 SQLM-ELM-TABLESPACE-STATE
           PIC S9(4) COMP-5 VALUE 693.

       77 SQLM-ELM-TABLESPACE-PAGE-SIZE
           PIC S9(4) COMP-5 VALUE 694.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_EXTENT_SIZE
       77 SQLM-ELM-TBSP-EXT-SIZE
           PIC S9(4) COMP-5 VALUE 695.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_PREFETCH_SIZE
       77 SQLM-ELM-TBSP-PREFETCH-SIZE
           PIC S9(4) COMP-5 VALUE 696.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_CUR_POOL_ID
       77 SQLM-ELM-TBSP-CUR-PL-ID
           PIC S9(4) COMP-5 VALUE 697.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_NEXT_POOL_ID
       77 SQLM-ELM-TBSP-NEXT-PL-ID
           PIC S9(4) COMP-5 VALUE 698.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_TOTAL_PAGES
       77 SQLM-ELM-TBSP-TOTAL-PG
           PIC S9(4) COMP-5 VALUE 699.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_USABLE_PAGES
       77 SQLM-ELM-TBSP-USABLE-PG
           PIC S9(4) COMP-5 VALUE 700.

       77 SQLM-ELM-TABLESPACE-USED-PAGES
           PIC S9(4) COMP-5 VALUE 701.

       77 SQLM-ELM-TABLESPACE-FREE-PAGES
           PIC S9(4) COMP-5 VALUE 702.

       77 SQLM-ELM-TABLESPACE-PAGE-TOP
           PIC S9(4) COMP-5 VALUE 703.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_PENDING_FREE_PAGES
       77 SQLM-ELM-TBSP-PENDING-FREE-PG
           PIC S9(4) COMP-5 VALUE 704.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_REBALANCER_MODE
       77 SQLM-ELM-TBSP-REB-MODE
           PIC S9(4) COMP-5 VALUE 705.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_REBALANCER_EXTENTS_REMAINING
       77 SQLM-ELM-TBSP-REB-EXT-REM
           PIC S9(4) COMP-5 VALUE 706.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_REBALANCER_EXTENTS_PROCESSED
       77 SQLM-ELM-TBSP-REB-EXT-PRO
           PIC S9(4) COMP-5 VALUE 707.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_REBALANCER_LAST_EXTENT_MOVED
       77 SQLM-ELM-TBSP-REB-LAST-EXT-MVD
           PIC S9(4) COMP-5 VALUE 708.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_REBALANCER_PRIORITY
       77 SQLM-ELM-TBSP-REB-PRIORITY
           PIC S9(4) COMP-5 VALUE 709.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_NUM_QUIESCERS
       77 SQLM-ELM-TBSP-NUM-QUIESCERS
           PIC S9(4) COMP-5 VALUE 710.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_STATE_CHANGE_OBJECT_ID
       77 SQLM-ELM-TBSP-ST-CHG-OBJ-ID
           PIC S9(4) COMP-5 VALUE 711.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_STATE_CHANGE_TS_ID
       77 SQLM-ELM-TBSP-ST-CHG-TS-ID
           PIC S9(4) COMP-5 VALUE 712.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_NUM_CONTAINERS
       77 SQLM-ELM-TBSP-NUM-CONTAINERS
           PIC S9(4) COMP-5 VALUE 713.

       77 SQLM-ELM-TABLESPACE-NUM-RANGES
           PIC S9(4) COMP-5 VALUE 714.

       77 SQLM-ELM-QUIESCER-STATE
           PIC S9(4) COMP-5 VALUE 715.

       77 SQLM-ELM-QUIESCER-AGENT-ID
           PIC S9(4) COMP-5 VALUE 716.

       77 SQLM-ELM-QUIESCER-TS-ID
           PIC S9(4) COMP-5 VALUE 717.

       77 SQLM-ELM-QUIESCER-OBJ-ID
           PIC S9(4) COMP-5 VALUE 718.

       77 SQLM-ELM-QUIESCER-AUTH-ID
           PIC S9(4) COMP-5 VALUE 719.

       77 SQLM-ELM-CONTAINER-ID
           PIC S9(4) COMP-5 VALUE 720.

       77 SQLM-ELM-CONTAINER-TYPE
           PIC S9(4) COMP-5 VALUE 721.

       77 SQLM-ELM-CONTAINER-TOTAL-PAGES
           PIC S9(4) COMP-5 VALUE 722.

      * The full name of the monitor element is SQLM_ELM_CONTAINER_USABLE_PAGES
       77 SQLM-ELM-CONT-USABLE-PG
           PIC S9(4) COMP-5 VALUE 723.

       77 SQLM-ELM-CONTAINER-STRIPE-SET
           PIC S9(4) COMP-5 VALUE 724.

       77 SQLM-ELM-CONTAINER-ACCESSIBLE
           PIC S9(4) COMP-5 VALUE 725.

       77 SQLM-ELM-CONTAINER-NAME
           PIC S9(4) COMP-5 VALUE 726.

      * The full name of the monitor element is SQLM_ELM_RANGE_STRIPE_SET_NUMBER
       77 SQLM-ELM-RANGE-STRIPE-SET-NUM
           PIC S9(4) COMP-5 VALUE 727.

       77 SQLM-ELM-RANGE-NUMBER
           PIC S9(4) COMP-5 VALUE 728.

       77 SQLM-ELM-RANGE-OFFSET
           PIC S9(4) COMP-5 VALUE 729.

       77 SQLM-ELM-RANGE-MAX-PAGE-NUMBER
           PIC S9(4) COMP-5 VALUE 730.

       77 SQLM-ELM-RANGE-MAX-EXTENT
           PIC S9(4) COMP-5 VALUE 731.

       77 SQLM-ELM-RANGE-START-STRIPE
           PIC S9(4) COMP-5 VALUE 732.

       77 SQLM-ELM-RANGE-END-STRIPE
           PIC S9(4) COMP-5 VALUE 733.

       77 SQLM-ELM-RANGE-ADJUSTMENT
           PIC S9(4) COMP-5 VALUE 734.

       77 SQLM-ELM-RANGE-NUM-CONTAINERS
           PIC S9(4) COMP-5 VALUE 735.

       77 SQLM-ELM-RANGE-CONTAINER-ID
           PIC S9(4) COMP-5 VALUE 736.


       77 SQLM-ELM-CONSISTENCY-TOKEN
           PIC S9(4) COMP-5 VALUE 737.

       77 SQLM-ELM-PACKAGE-VERSION-ID
           PIC S9(4) COMP-5 VALUE 738.


       77 SQLM-ELM-LOCK-NAME
           PIC S9(4) COMP-5 VALUE 739.

       77 SQLM-ELM-LOCK-COUNT
           PIC S9(4) COMP-5 VALUE 740.

       77 SQLM-ELM-LOCK-HOLD-COUNT
           PIC S9(4) COMP-5 VALUE 741.

       77 SQLM-ELM-LOCK-ATTRIBUTES
           PIC S9(4) COMP-5 VALUE 742.

       77 SQLM-ELM-LOCK-RELEASE-FLAGS
           PIC S9(4) COMP-5 VALUE 743.

       77 SQLM-ELM-LOCK-CURRENT-MODE
           PIC S9(4) COMP-5 VALUE 744.

       77 SQLM-ELM-TABLESPACE-FS-CACHING
           PIC S9(4) COMP-5 VALUE 745.


      *****************************************************************************
      * Dynamic Bufferpool                                                         *
      *****************************************************************************
       77 SQLM-ELM-BP-TBSP-USE-COUNT
           PIC S9(4) COMP-5 VALUE 751.

      * The full name of the monitor element is SQLM_ELM_BP_PAGES_LEFT_TO_REMOVE
       77 SQLM-ELM-BP-PG-LEFT-TO-REMOVE
           PIC S9(4) COMP-5 VALUE 752.

       77 SQLM-ELM-BP-CUR-BUFFSZ
           PIC S9(4) COMP-5 VALUE 753.

       77 SQLM-ELM-BP-NEW-BUFFSZ
           PIC S9(4) COMP-5 VALUE 754.


       77 SQLM-ELM-SORT-HEAP-TOP
           PIC S9(4) COMP-5 VALUE 755.

      * The full name of the monitor element is SQLM_ELM_SORT_SHRHEAP_ALLOCATED
       77 SQLM-ELM-SRT-SHRHEAP-ALLC
           PIC S9(4) COMP-5 VALUE 756.

       77 SQLM-ELM-SORT-SHRHEAP-TOP
           PIC S9(4) COMP-5 VALUE 757.


      *****************************************************************************
      * APM Interaction                                                            *
      *****************************************************************************
      * The full name of the monitor element is SQLM_ELM_SHR_WORKSPACE_SIZE_TOP
       77 SQLM-ELM-SHR-WKSPC-SIZE-TOP
           PIC S9(4) COMP-5 VALUE 758.

      * The full name of the monitor element is SQLM_ELM_SHR_WORKSPACE_NUM_OVERFLOWS
       77 SQLM-ELM-SHR-WKSPC-NUM-OWFLW
           PIC S9(4) COMP-5 VALUE 759.

      * The full name of the monitor element is SQLM_ELM_SHR_WORKSPACE_SECTION_LOOKUPS
       77 SQLM-ELM-SHR-WKSPC-SEC-LKP
           PIC S9(4) COMP-5 VALUE 760.

      * The full name of the monitor element is SQLM_ELM_SHR_WORKSPACE_SECTION_INSERTS
       77 SQLM-ELM-SHR-WKSPC-SEC-INS
           PIC S9(4) COMP-5 VALUE 761.

      * The full name of the monitor element is SQLM_ELM_PRIV_WORKSPACE_SIZE_TOP
       77 SQLM-ELM-PRIV-WKSPC-SIZE-TOP
           PIC S9(4) COMP-5 VALUE 762.

      * The full name of the monitor element is SQLM_ELM_PRIV_WORKSPACE_NUM_OVERFLOWS
       77 SQLM-ELM-PRIV-WKSPC-NUM-OWFLW
           PIC S9(4) COMP-5 VALUE 763.

      * The full name of the monitor element is SQLM_ELM_PRIV_WORKSPACE_SECTION_LOOKUPS
       77 SQLM-ELM-PRIV-WKSPC-SEC-LKP
           PIC S9(4) COMP-5 VALUE 764.

      * The full name of the monitor element is SQLM_ELM_PRIV_WORKSPACE_SECTION_INSERTS
       77 SQLM-ELM-PRIV-WKSPC-SEC-INS
           PIC S9(4) COMP-5 VALUE 765.


      *****************************************************************************
      * Catalog Cache                                                              *
      *****************************************************************************
       77 SQLM-ELM-CAT-CACHE-SIZE-TOP
           PIC S9(4) COMP-5 VALUE 766.


      *****************************************************************************
      * Partition Number for EEE WTT evmon                                         *
      *****************************************************************************
       77 SQLM-ELM-PARTITION-NUMBER
           PIC S9(4) COMP-5 VALUE 767.

      * The full name of the monitor element is SQLM_ELM_NUM_TRANSMISSIONS_GROUP
       77 SQLM-ELM-NUM-TRMS-GROUP
           PIC S9(4) COMP-5 VALUE 768.

       77 SQLM-ELM-NUM-INDOUBT-TRANS
           PIC S9(4) COMP-5 VALUE 769.


      *****************************************************************************
      * Information for active utilities                                           *
      *****************************************************************************
       77 SQLM-ELM-UTILITY-DBNAME
           PIC S9(4) COMP-5 VALUE 770.

       77 SQLM-ELM-UTILITY-ID
           PIC S9(4) COMP-5 VALUE 771.

       77 SQLM-ELM-UTILITY-TYPE
           PIC S9(4) COMP-5 VALUE 772.

       77 SQLM-ELM-UTILITY-PRIORITY
           PIC S9(4) COMP-5 VALUE 773.

       77 SQLM-ELM-UTILITY-START-TIME
           PIC S9(4) COMP-5 VALUE 774.

       77 SQLM-ELM-UTILITY-DESCRIPTION
           PIC S9(4) COMP-5 VALUE 775.


      * The full name of the monitor element is SQLM_ELM_POOL_ASYNC_INDEX_READ_REQS
       77 SQLM-ELM-PL-ASYNC-IDX-RD-REQS
           PIC S9(4) COMP-5 VALUE 776.

       77 SQLM-ELM-SESSION-AUTH-ID
           PIC S9(4) COMP-5 VALUE 777.


       77 SQLM-ELM-SQL-CHAINS
           PIC S9(4) COMP-5 VALUE 778.


      * The full name of the monitor element is SQLM_ELM_POOL_TEMP_DATA_L_READS
       77 SQLM-ELM-PL-TMP-DATA-L-RD
           PIC S9(4) COMP-5 VALUE 779.

      * The full name of the monitor element is SQLM_ELM_POOL_TEMP_DATA_P_READS
       77 SQLM-ELM-PL-TMP-DATA-P-RD
           PIC S9(4) COMP-5 VALUE 780.

      * The full name of the monitor element is SQLM_ELM_POOL_TEMP_INDEX_L_READS
       77 SQLM-ELM-PL-TMP-IDX-L-RD
           PIC S9(4) COMP-5 VALUE 781.

      * The full name of the monitor element is SQLM_ELM_POOL_TEMP_INDEX_P_READS
       77 SQLM-ELM-PL-TMP-IDX-P-RD
           PIC S9(4) COMP-5 VALUE 782.


       77 SQLM-ELM-MAX-TIME-1-MS
           PIC S9(4) COMP-5 VALUE 783.

       77 SQLM-ELM-MAX-TIME-100-MS
           PIC S9(4) COMP-5 VALUE 784.

       77 SQLM-ELM-MAX-TIME-500-MS
           PIC S9(4) COMP-5 VALUE 785.

       77 SQLM-ELM-MAX-TIME-GT500-MS
           PIC S9(4) COMP-5 VALUE 786.


      * The full name of the monitor element is SQLM_ELM_LOG_TO_REDO_FOR_RECOVERY
       77 SQLM-ELM-LOG-TO-REDO-FOR-RCVRY
           PIC S9(4) COMP-5 VALUE 787.

      * redone during recovery       *
       77 SQLM-ELM-POOL-NO-VICTIM-BUFFER
           PIC S9(4) COMP-5 VALUE 788.

      * buffer available from hate   *
      * list                         *
      * The full name of the monitor element is SQLM_ELM_LOG_HELD_BY_DIRTY_PAGES
       77 SQLM-ELM-LOG-HELD-BY-DIRTY-PG
           PIC S9(4) COMP-5 VALUE 789.

      * by minbuf                    *

      *****************************************************************************
      *   Information for the progress of utilities                                *
      *****************************************************************************
       77 SQLM-ELM-PROGRESS-DESCRIPTION
           PIC S9(4) COMP-5 VALUE 790.

       77 SQLM-ELM-PROGRESS-START-TIME
           PIC S9(4) COMP-5 VALUE 791.

       77 SQLM-ELM-PROGRESS-WORK-METRIC
           PIC S9(4) COMP-5 VALUE 792.

       77 SQLM-ELM-PROGRESS-TOTAL-UNITS
           PIC S9(4) COMP-5 VALUE 793.

      * The full name of the monitor element is SQLM_ELM_PROGRESS_COMPLETED_UNITS
       77 SQLM-ELM-PROG-COMP-UNIT
           PIC S9(4) COMP-5 VALUE 794.

       77 SQLM-ELM-PROGRESS-SEQ-NUM
           PIC S9(4) COMP-5 VALUE 795.

      * The full name of the monitor element is SQLM_ELM_PROGRESS_LIST_CUR_SEQ_NUM
       77 SQLM-PROG-LST-CUR-SEQN
           PIC S9(4) COMP-5 VALUE 796.


      *****************************************************************************
      * Information for HADR                                                       *
      *****************************************************************************
       77 SQLM-ELM-HADR-ROLE
           PIC S9(4) COMP-5 VALUE 797.

       77 SQLM-ELM-HADR-STATE
           PIC S9(4) COMP-5 VALUE 798.

       77 SQLM-ELM-HADR-SYNCMODE
           PIC S9(4) COMP-5 VALUE 799.

       77 SQLM-ELM-HADR-CONNECT-STATUS
           PIC S9(4) COMP-5 VALUE 800.

       77 SQLM-ELM-HADR-CONNECT-TIME
           PIC S9(4) COMP-5 VALUE 801.

       77 SQLM-ELM-HADR-HEARTBEAT
           PIC S9(4) COMP-5 VALUE 802.

       77 SQLM-ELM-HADR-LOCAL-HOST
           PIC S9(4) COMP-5 VALUE 803.

       77 SQLM-ELM-HADR-LOCAL-SERVICE
           PIC S9(4) COMP-5 VALUE 804.

       77 SQLM-ELM-HADR-REMOTE-HOST
           PIC S9(4) COMP-5 VALUE 805.

       77 SQLM-ELM-HADR-REMOTE-SERVICE
           PIC S9(4) COMP-5 VALUE 806.

       77 SQLM-ELM-HADR-TIMEOUT
           PIC S9(4) COMP-5 VALUE 807.

       77 SQLM-ELM-HADR-PRIMARY-LOG-FILE
           PIC S9(4) COMP-5 VALUE 808.

       77 SQLM-ELM-HADR-PRIMARY-LOG-PAGE
           PIC S9(4) COMP-5 VALUE 809.

       77 SQLM-ELM-HADR-PRIMARY-LOG-LSN
           PIC S9(4) COMP-5 VALUE 810.

       77 SQLM-ELM-HADR-STANDBY-LOG-FILE
           PIC S9(4) COMP-5 VALUE 811.

       77 SQLM-ELM-HADR-STANDBY-LOG-PAGE
           PIC S9(4) COMP-5 VALUE 812.

       77 SQLM-ELM-HADR-STANDBY-LOG-LSN
           PIC S9(4) COMP-5 VALUE 813.

       77 SQLM-ELM-HADR-LOG-GAP
           PIC S9(4) COMP-5 VALUE 814.

       77 SQLM-ELM-HADR-REMOTE-INSTANCE
           PIC S9(4) COMP-5 VALUE 815.


      *****************************************************************************
      * Additional elements added in 8.2                                           *
      *****************************************************************************
       77 SQLM-ELM-DATA-OBJECT-PAGES
           PIC S9(4) COMP-5 VALUE 816.

      * base data                   *
       77 SQLM-ELM-INDEX-OBJECT-PAGES
           PIC S9(4) COMP-5 VALUE 817.

      * base data                   *
       77 SQLM-ELM-LOB-OBJECT-PAGES
           PIC S9(4) COMP-5 VALUE 818.

       77 SQLM-ELM-LONG-OBJECT-PAGES
           PIC S9(4) COMP-5 VALUE 819.

      * data                        *

       77 SQLM-ELM-LOCK-TIMEOUT-VAL
           PIC S9(4) COMP-5 VALUE 820.


      *****************************************************************************
      *  Log elements v8.2                                                         *
      *****************************************************************************
       77 SQLM-ELM-LOG-WRITE-TIME
           PIC S9(4) COMP-5 VALUE 821.

       77 SQLM-ELM-LOG-READ-TIME
           PIC S9(4) COMP-5 VALUE 822.

       77 SQLM-ELM-NUM-LOG-WRITE-IO
           PIC S9(4) COMP-5 VALUE 823.

       77 SQLM-ELM-NUM-LOG-READ-IO
           PIC S9(4) COMP-5 VALUE 824.

       77 SQLM-ELM-NUM-LOG-PART-PAGE-IO
           PIC S9(4) COMP-5 VALUE 825.

       77 SQLM-ELM-NUM-LOG-BUFF-FULL
           PIC S9(4) COMP-5 VALUE 826.

       77 SQLM-ELM-NUM-LOG-DATA-IN-BUFF
           PIC S9(4) COMP-5 VALUE 827.

       77 SQLM-ELM-LOG-FILE-NUM-FIRST
           PIC S9(4) COMP-5 VALUE 828.

       77 SQLM-ELM-LOG-FILE-NUM-LAST
           PIC S9(4) COMP-5 VALUE 829.

       77 SQLM-ELM-LOG-FILE-NUM-CURR
           PIC S9(4) COMP-5 VALUE 830.

       77 SQLM-ELM-LOG-FILE-ARCHIVE
           PIC S9(4) COMP-5 VALUE 831.


       77 SQLM-ELM-NANOSEC
           PIC S9(4) COMP-5 VALUE 832.


      *****************************************************************************
      *  New elements for enhanced deadlock event monitor with statement history   *
      *****************************************************************************
       77 SQLM-ELM-STMT-HISTORY-ID
           PIC S9(4) COMP-5 VALUE 833.

       77 SQLM-ELM-STMT-LOCK-TIMEOUT
           PIC S9(4) COMP-5 VALUE 834.

       77 SQLM-ELM-STMT-ISOLATION
           PIC S9(4) COMP-5 VALUE 835.

       77 SQLM-ELM-COMP-ENV-DESC
           PIC S9(4) COMP-5 VALUE 836.

      * compilation environment     *
       77 SQLM-ELM-STMT-VALUE-TYPE
           PIC S9(4) COMP-5 VALUE 837.

       77 SQLM-ELM-STMT-VALUE-ISREOPT
           PIC S9(4) COMP-5 VALUE 838.

       77 SQLM-ELM-STMT-VALUE-ISNULL
           PIC S9(4) COMP-5 VALUE 839.

       77 SQLM-ELM-STMT-VALUE-DATA
           PIC S9(4) COMP-5 VALUE 840.

       77 SQLM-ELM-STMT-VALUE-INDEX
           PIC S9(4) COMP-5 VALUE 841.

       77 SQLM-ELM-STMT-FIRST-USE-TIME
           PIC S9(4) COMP-5 VALUE 842.

       77 SQLM-ELM-STMT-LAST-USE-TIME
           PIC S9(4) COMP-5 VALUE 843.

       77 SQLM-ELM-STMT-NEST-LEVEL
           PIC S9(4) COMP-5 VALUE 844.

       77 SQLM-ELM-STMT-INVOCATION-ID
           PIC S9(4) COMP-5 VALUE 845.

       77 SQLM-ELM-STMT-QUERY-ID
           PIC S9(4) COMP-5 VALUE 846.

       77 SQLM-ELM-STMT-SOURCE-ID
           PIC S9(4) COMP-5 VALUE 847.

       77 SQLM-ELM-STMT-PKGCACHE-ID
           PIC S9(4) COMP-5 VALUE 848.

       77 SQLM-ELM-INACT-STMTHIST-SZ
           PIC S9(4) COMP-5 VALUE 849.


      *****************************************************************************
      * New elements for Single point of storage management                        *
      *****************************************************************************
       77 SQLM-ELM-NUM-DB-STORAGE-PATHS
           PIC S9(4) COMP-5 VALUE 850.

       77 SQLM-ELM-DB-STORAGE-PATH
           PIC S9(4) COMP-5 VALUE 851.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_INITIAL_SIZE
       77 SQLM-ELM-TBSP-INITIAL-SIZE
           PIC S9(4) COMP-5 VALUE 852.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_CURRENT_SIZE
       77 SQLM-ELM-TBSP-CURRENT-SIZE
           PIC S9(4) COMP-5 VALUE 853.

       77 SQLM-ELM-TABLESPACE-MAX-SIZE
           PIC S9(4) COMP-5 VALUE 854.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_INCREASE_SIZE_PERCENT
       77 SQLM-ELM-TBSP-INCR-SIZE-PCT
           PIC S9(4) COMP-5 VALUE 855.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_INCREASE_SIZE
       77 SQLM-ELM-TBSP-INCR-SIZE
           PIC S9(4) COMP-5 VALUE 856.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_LAST_RESIZE_TIME
       77 SQLM-ELM-TBSP-LAST-RESIZE-TIME
           PIC S9(4) COMP-5 VALUE 857.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_USING_AUTO_STORAGE
       77 SQLM-ELM-TBSP-USE-AUTO-STORAGE
           PIC S9(4) COMP-5 VALUE 858.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_AUTO_RESIZE_ENABLED
       77 SQLM-ELM-TBSP-AUTO-RSZ-ENABLED
           PIC S9(4) COMP-5 VALUE 859.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_LAST_RESIZE_FAILED
       77 SQLM-ELM-TBSP-LAST-RESIZE-FAIL
           PIC S9(4) COMP-5 VALUE 860.


       77 SQLM-ELM-BP-ID
           PIC S9(4) COMP-5 VALUE 861.


      *****************************************************************************
      *                                                                            *
      * The following describes the elements found in the SQLM_CLASS_HEALTH        *
      * and SQLM_CLASS_HEALTH_WITH_DETAIL snapshot types. There can be more than   *
      * one SQLM_ELM_HEALTH_INDICATOR and SQLM_ELM_HEALTH_INDICATOR_HIST.          *
      * SQLM_ELM_HEALTH_INDICATOR_HIST is only present in health snapshot with     *
      * details.                                                                   *
      *                                                                            *
      * There will be only one NODEINFO stream for a single partition snapshot. A  *
      * global snapshot will contain a NODEINFO stream for each partition with an  *
      * activate Health Monitor.                                                   *
      *                                                                            *
      *****************************************************************************
      *                                                                            *
      * Typical health snapshot hierarchy:                                         *
      *                                                                            *
      * SQLM_ELM_COLLECTED                                                         *
      *    +- DB2                                                                  *
      *          +- HEALTH_INDICATOR                                               *
      *                +- HEALTH_INDICATOR_HIST (only with details)                *
      *    +- DBASE                                                                *
      *          +- HEALTH_INDICATOR                                               *
      *                +- HEALTH_INDICATOR_HIST (only with details)                *
      *    +- TABLESPACE_LIST                                                      *
      *          +- TABLESPACE                                                     *
      *                +- HEALTH_INDICATOR                                         *
      *                      +- HEALTH_INDICATOR_HIST (only with details)          *
      *                +- NODEINFO (ESE only, otherwise TSCs are here)             *
      *                      +- TABLESPACE_CONTAINER                               *
      *                            +- HEALTH_INDICATOR                             *
      *                               +- HEALTH_INDICATOR_HIST (only with details) *
      *                                                                            *
      *****************************************************************************
      *                                                                            *
      * Hierarchy of elements under SQLM_ELM_HI and SQLM_ELM_HI_HIST:              *
      *                                                                            *
      *  +- HEALTH_INDICATOR                                                       *
      *        SQLM_ELM_HI_ID                                                      *
      *        SQLM_ELM_HI_VALUE                                                   *
      *        +- SQLM_ELM_HI_TIMESTAMP                                            *
      *              SQLM_ELM_SECONDS                                              *
      *              SQLM_ELM_MICROSEC                                             *
      *        SQLM_ELM_HI_ALERT_STATE                                             *
      *        +- SQLM_ELM_HI_OBJ_LIST                                             *
      *              SQLM_ELM_HI_OBJ_NAME                                          *
      *              SQLM_ELM_HI_OBJ_DETAIL                                        *
      *              SQLM_ELM_HI_OBJ_STATE                                         *
      *              +- SQLM_ELM_HI_TIMESTAMP                                      *
      *                    SQLM_ELM_SECONDS                                        *
      *                    SQLM_ELM_MICROSEC                                       *
      *              +- SQLM_ELM_HI_OBJ_LIST_HIST      (only with details)         *
      *                    SQLM_ELM_HI_OBJ_STATE                                   *
      *                    +- SQLM_ELM_HI_TIMESTAMP                                *
      *                          SQLM_ELM_SECONDS                                  *
      *                          SQLM_ELM_MICROSEC                                 *
      *                                                                            *
      * (The following is additional info present in SQLM_CLASS_HEALTH_WITH_DETAIL *
      *  snapshot only.)                                                           *
      *                                                                            *
      *        SQLM_ELM_HI_FORMULA                                                 *
      *        SQLM_ELM_HI_ADDITIONAL_INFO                                         *
      *        +- SQLM_ELM_HEALTH_INDICATOR_HIST                                   *
      *              SQLM_ELM_HI_ID                                                *
      *              SQLM_ELM_HI_VALUE                                             *
      *              +- SQLM_ELM_HI_TIMESTAMP                                      *
      *                    SQLM_ELM_SECONDS                                        *
      *                    SQLM_ELM_MICROSEC                                       *
      *              SQLM_ELM_HI_ALERT_STATE                                       *
      *              SQLM_ELM_HI_FORMULA                                           *
      *              SQLM_ELM_HI_ADDITIONAL_INFO                                   *
      *                                                                            *
      *****************************************************************************

      * Health snapshot only elements. These are not found in the standard         *
      * snapshots but can be present in snapshot with and without details.         *
      * Used under SQLM_ELM_HEALTH INDICATOR and SQLM_ELM_HEALTH_INDICATOR_HIST    *
      * logical group.                                                             *
       77 SQLM-ELM-HI-ID
           PIC S9(5) COMP-5 VALUE 55001.

       77 SQLM-ELM-HI-VALUE
           PIC S9(5) COMP-5 VALUE 55002.

       77 SQLM-ELM-HI-ALERT-STATE
           PIC S9(5) COMP-5 VALUE 55003.

      * The full name of the monitor element is SQLM_ELM_DB2_ROLLED_UP_ALERT_STATE
       77 SQLM-ELM-DB2-R0L-UP-ALRT-ST
           PIC S9(5) COMP-5 VALUE 55004.

      * The full name of the monitor element is SQLM_ELM_DATABASE_ROLLED_UP_ALERT_STATE
       77 SQLM-ELM-DB-R0L-UP-ALRT-ST
           PIC S9(5) COMP-5 VALUE 55005.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_ROLLED_UP_ALERT_STATE
       77 SQLM-ELM-TBSP-R0L-UP-ALRT-ST
           PIC S9(5) COMP-5 VALUE 55006.

      * The full name of the monitor element is SQLM_ELM_CONTAINER_ROLLED_UP_ALERT_STATE
       77 SQLM-ELM-CONT-R0L-UP-ALRT-ST
           PIC S9(5) COMP-5 VALUE 55007.

       77 SQLM-ELM-HI-OBJ-NAME
           PIC S9(5) COMP-5 VALUE 55008.

       77 SQLM-ELM-HI-OBJ-DETAIL
           PIC S9(5) COMP-5 VALUE 55009.

       77 SQLM-ELM-HI-OBJ-STATE
           PIC S9(5) COMP-5 VALUE 55010.


      * the following are for health snapshot with detail only                     *
       77 SQLM-ELM-HI-FORMULA
           PIC S9(5) COMP-5 VALUE 55101.

       77 SQLM-ELM-HI-ADDITIONAL-INFO
           PIC S9(5) COMP-5 VALUE 55102.


      *****************************************************************************
      * The folowing macros define the health indicators and are present in the    *
      * sqlm_header_data.data field when sqlm_header_data.header.element is        *
      * SQLM_ELM_HI_ID.                                                            *
      *****************************************************************************

      *****************************************************************************
      * Health snapshot elements under SQLM_ELM_DB2 logical grouping:              *
      *                                                                            *
      * +- SQLM_ELM_DB2                                                            *
      *       (also present in standard system snapshots)                          *
      *       SQLM_ELM_DB2START_TIME                                               *
      *       SQLM_ELM_NUM_NODES_IN_DB2_INSTANCE                                   *
      *       (new for health snapshot)                                            *
      *       SQLM_ELM_DB2_ROLLED_UP_ALERT_STATE                                   *
      *       SQLM_ELM_HEALTH_INDICATOR                                            *
      *****************************************************************************

      * these are possible values for SQLM_ELM_HEALTH_INDICATOR -- SQLM_ELM_HI_ID  *
      * under the SQLM_ELM_DB2 logical group                                       *
       77 SQLM-HI-DB2-STATE
           PIC S9(4) COMP-5 VALUE 1.

      * The full name of the monitor element is SQLM_HI_DB2_PRIVATE_SORT_MEM_UTILIZATION
       77 SQLM-HI-DB2-PRV-SRT-M-UTL
           PIC S9(4) COMP-5 VALUE 2.

      * The full name of the monitor element is SQLM_HI_DB2_MAX_PRIVATE_SORT_MEM_UTILIZATION
       77 SQLM-HI-DB2-MAX-PRV-SRT-M-UTL
           PIC S9(4) COMP-5 VALUE 3.

      * The full name of the monitor element is SQLM_HI_DB2_MON_HEAP_UTILIZATION
       77 SQLM-HI-DB2-MON-HEAP-UTL
           PIC S9(4) COMP-5 VALUE 4.


      *****************************************************************************
      * Health snapshot elements under SQLM_ELM_DB logical grouping:               *
      *                                                                            *
      * +- SQLM_ELM_DB                                                             *
      *       (also present in standard system snapshots)                          *
      *       SQLM_ELM_DB_LOCATION                                                 *
      *       SQLM_ELM_SERVER_PLATFORM                                             *
      *       SQLM_ELM_INPUT_DB_ALIAS                                              *
      *       SQLM_ELM_DB_NAME                                                     *
      *       SQLM_ELM_DB_PATH                                                     *
      *       (new for health snapshot)                                            *
      *       SQLM_ELM_DATABASE_ROLLED_UP_ALERT_STATE                              *
      *       SQLM_ELM_HEALTH_INDICATOR                                            *
      *****************************************************************************

      * these are possible values for SQLM_ELM_HEALTH_INDICATOR -- SQLM_ELM_HI_ID  *
      * under the SQLM_ELM_DATABASE logical group                                  *
       77 SQLM-HI-DATABASE-STATE
           PIC S9(4) COMP-5 VALUE 1001.

      * The full name of the monitor element is SQLM_HI_DATABASE_SHARED_SORT_MEM_UTILIZATION
       77 SQLM-HI-DB-SHR-SRT-M-UTL
           PIC S9(4) COMP-5 VALUE 1002.

       77 SQLM-HI-DATABASE-SPILLED-SORTS
           PIC S9(4) COMP-5 VALUE 1003.

      * The full name of the monitor element is SQLM_HI_DATABASE_MAX_SHARED_SORT_MEM_UTILIZATION
       77 SQLM-HI-DB-MAX-SHR-SRT-M-UTL
           PIC S9(4) COMP-5 VALUE 1004.

      * The full name of the monitor element is SQLM_HI_DATABASE_LOG_UTILIZATION
       77 SQLM-HI-DB-LOG-UTL
           PIC S9(4) COMP-5 VALUE 1005.

      * The full name of the monitor element is SQLM_HI_DATABASE_LOG_FILESYSTEM_UTILIZATION
       77 SQLM-HI-DB-LOG-FILESYSTEM-UTL
           PIC S9(4) COMP-5 VALUE 1006.

       77 SQLM-HI-DATABASE-DEADLOCK-RATE
           PIC S9(4) COMP-5 VALUE 1007.

      * The full name of the monitor element is SQLM_HI_DATABASE_LOCKLIST_UTILIZATION
       77 SQLM-HI-DB-LOCKLIST-UTL
           PIC S9(4) COMP-5 VALUE 1008.

      * The full name of the monitor element is SQLM_HI_DATABASE_LOCK_ESCALATION_RATE
       77 SQLM-HI-DB-LOCK-ESCL-RATE
           PIC S9(4) COMP-5 VALUE 1009.

      * The full name of the monitor element is SQLM_HI_DATABASE_APPS_WAITING_ON_LOCKS
       77 SQLM-HI-DB-APPS-WAIT-ON-LOCKS
           PIC S9(4) COMP-5 VALUE 1010.

      * The full name of the monitor element is SQLM_HI_DATABASE_PKGCACHE_HITRATIO
       77 SQLM-HI-DB-PKGCACHE-HITRATIO
           PIC S9(4) COMP-5 VALUE 1011.

      * The full name of the monitor element is SQLM_HI_DATABASE_CATCACHE_HITRATIO
       77 SQLM-HI-DB-CATCACHE-HITRATIO
           PIC S9(4) COMP-5 VALUE 1012.

      * The full name of the monitor element is SQLM_HI_DATABASE_SHRWORKSPACE_HITRATIO
       77 SQLM-HI-DB-SHRWKSPC-HITRATIO
           PIC S9(4) COMP-5 VALUE 1013.

      * The full name of the monitor element is SQLM_HI_DATABASE_HEAP_UTILIZATION
       77 SQLM-HI-DB-HEAP-UTL
           PIC S9(4) COMP-5 VALUE 1014.

       77 SQLM-HI-DATABASE-TB-REORG-REQ
           PIC S9(4) COMP-5 VALUE 1015.

      * The full name of the monitor element is SQLM_HI_DATABASE_HADR_OP_STATUS
       77 SQLM-HI-DATABASE-HADR-OPSTATUS
           PIC S9(4) COMP-5 VALUE 1016.

       77 SQLM-HI-DATABASE-HADR-DELAY
           PIC S9(4) COMP-5 VALUE 1017.

       77 SQLM-HI-DATABASE-BACKUP-REQ
           PIC S9(4) COMP-5 VALUE 1018.

      * The full name of the monitor element is SQLM_HI_DATABASE_FED_NICKNAME_STATUS
       77 SQLM-HI-DB-FED-NICKNAME-STATUS
           PIC S9(4) COMP-5 VALUE 1019.

      * The full name of the monitor element is SQLM_HI_DATABASE_FED_SERVER_STATUS
       77 SQLM-HI-DB-FED-SERVER-STATUS
           PIC S9(4) COMP-5 VALUE 1020.

       77 SQLM-HI-DATABASE-TB-STATS-REQ
           PIC S9(4) COMP-5 VALUE 1022.

      * The full name of the monitor element is SQLM_HI_DATABASE_AUTO_STORAGE_UTILIZATION
       77 SQLM-HI-DB-AUTO-STG-UTIL
           PIC S9(4) COMP-5 VALUE 1023.


      *****************************************************************************
      * Health snapshot elements under SQLM_ELM_TABLESPACE_LIST logical grouping:  *
      *                                                                            *
      * +- SQLM_ELM_TABLESPACE_LIST                                                *
      *       (also present in standard system snapshots)                          *
      *       SQLM_ELM_DB_NAME                                                     *
      *       SQLM_ELM_DB_PATH                                                     *
      *       SQLM_ELM_INPUT_DB_ALIAS                                              *
      *       (new for health snapshot)                                            *
      *       //none for now                                                       *
      *****************************************************************************

      *****************************************************************************
      * Health snapshot elements under SQLM_ELM_TABLESPACE logical grouping:       *
      *                                                                            *
      * +- SQLM_ELM_TABLESPACE                                                     *
      *       (also present in standard system snapshots)                          *
      *       SQLM_ELM_TABLESPACE_NAME                                             *
      *       (new for health snapshot)                                            *
      *       SQLM_ELM_TABLESPACE_ROLLED_UP_ALERT_STATE                            *
      *       SQLM_ELM_HEALTH_INDICATOR                                            *
      *****************************************************************************

      * these are possible values for SQLM_ELM_HEALTH_INDICATOR -- SQLM_ELM_HI_ID  *
      * under the SQLM_ELM_TABLESPACE logical group                                *
       77 SQLM-HI-TABLESPACE-STATE
           PIC S9(4) COMP-5 VALUE 2001.

       77 SQLM-HI-TABLESPACE-UTILIZATION
           PIC S9(4) COMP-5 VALUE 2002.

      * The full name of the monitor element is SQLM_HI_TABLESPACE_AUTO_RESIZE_STATUS
       77 SQLM-HI-TBSP-AUTO-RESIZE-ST
           PIC S9(4) COMP-5 VALUE 2003.

      * The full name of the monitor element is SQLM_HI_AUTO_RESIZE_TABLESPACE_UTILIZATION
       77 SQLM-HI-AUTO-RS-TBSP-UTIL
           PIC S9(4) COMP-5 VALUE 2004.


      *****************************************************************************
      * Health snapshot elements under SQLM_ELM_TABLESPACE_CONTAINER logical group *
      *                                                                            *
      * +- SQLM_ELM_TABLESPACE_CONTAINER                                           *
      *       (also present in standard system snapshots)                          *
      *       SQLM_ELM_CONTAINER_NAME                                              *
      *       (new for health snapshot)                                            *
      *       SQLM_ELM_HEALTH_INDICATOR                                            *
      *****************************************************************************

      * these are possible values for SQLM_ELM_HEALTH_INDICATOR -- SQLM_ELM_HI_ID  *
      * under the SQLM_ELM_TABLESPACE_CONTAINER logical group                      *
      * The full name of the monitor element is SQLM_HI_TABLESPACE_CONTAINER_STATE
       77 SQLM-HI-TBSP-CONT-ST
           PIC S9(4) COMP-5 VALUE 3001.

      * The full name of the monitor element is SQLM_HI_TABLESPACE_CONTAINER_UTILIZATION
       77 SQLM-HI-TBSP-CONT-UTL
           PIC S9(4) COMP-5 VALUE 3002.


      *****************************************************************************
      * Health snapshot alert state values.                                        *
      *                                                                            *
      * Those states can be used as values of the following health snapshot        *
      * elements:                                                                  *
      *                                                                            *
      *    SQLM_ELM_HI_ALERT_STATE                                                 *
      *    SQLM_ELM_DB2_ROLLED_UP_ALERT_STATE                                      *
      *    SQLM_ELM_DATABASE_ROLLED_UP_ALERT_STATE                                 *
      *    SQLM_ELM_TABLESPACE_ROLLED_UP_ALERT_STATE                               *
      *    SQLM_ELM_HI_OBJ_STATE                                                   *
      *                                                                            *
      * Keep the values as different bits as we might run actions on combinations  *
      * of those alert states.                                                     *
      * NOTE: Currently, all the AUTOMATED states are only applicable for          *
      *       collection object state. These values are not valid for health       *
      *       indicator states.                                                    *
      *                                                                            *
      *****************************************************************************

       77 SQLM-HI-ALERT-STATE-UNKNOWN
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-HI-ALERT-STATE-NORMAL
           PIC S9(4) COMP-5 VALUE 1.

       77 SQLM-HI-ALERT-STATE-ATTENTION
           PIC S9(4) COMP-5 VALUE 2.

       77 SQLM-HI-ALERT-STATE-WARNING
           PIC S9(4) COMP-5 VALUE 3.

       77 SQLM-HI-ALERT-STATE-ALARM
           PIC S9(4) COMP-5 VALUE 4.

       77 SQLM-HI-ALERT-STATE-AUTOMATED
           PIC S9(4) COMP-5 VALUE 5.

      * The full name of the monitor element is SQLM_HI_ALERT_STATE_AUTOMATE_FAILED
       77 SQLM-HIALRT-STATE-AUTO-FAIL
           PIC S9(4) COMP-5 VALUE 6.


      *****************************************************************************
      * Health Indicator specific values                                           *
      *****************************************************************************

      * these are possible values for SQLM_HI_DATABASE_BACKUP_REQ                  *
       77 SQLM-HI-VAL-NO-DB-BACKUP-REQ
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-HI-VAL-DB-BACKUP-REQ
           PIC S9(4) COMP-5 VALUE 1.

      * The full name of the monitor element is SQLM_HI_VAL_AUTO_DB_BACKUP_FAILED
       77 SQLM-HIVAL-AUTO-DBBCKP-FAIL
           PIC S9(4) COMP-5 VALUE 2.


      * these are the possible values for SQLM_HI_DATABASE_TB_REORG_REQ            *
       77 SQLM-HI-VAL-NO-REORG-REQ
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-HI-VAL-REORG-REQ
           PIC S9(4) COMP-5 VALUE 1.


      * these are the possible values for SQLM_HI_DATABASE_TB_STATS_REQ            *
       77 SQLM-HI-VAL-NO-STATS-REQ
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-HI-VAL-STATS-REQ
           PIC S9(4) COMP-5 VALUE 1.


      * nickname status (fed_nickname_status)                                      *
       77 SQLM-FED-NN-VALID
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-FED-NN-INVALID
           PIC S9(4) COMP-5 VALUE 1.


      * federated server status (fed_server_status)                                *
       77 SQLM-FED-SERVER-AVAILABLE
           PIC S9(4) COMP-5 VALUE 0.

       77 SQLM-FED-SERVER-UNAVAILABLE
           PIC S9(4) COMP-5 VALUE 1.


      *****************************************************************************
      * Health snapshot option flags. These flags can be used in the agent_id      *
      * field of the sqlm_obj_struct passed in the sqlma. The flag is used in      *
      * conjunction with the snapshot classes SQLM_CLASS_HEALTH and                *
      * SQLM_CLASS_HEALTH_WITH_DETAIL  to determine what additional health snapshot*
      * information to return in the snapshot stream                               *
      *****************************************************************************

       77 SQLM-HMON-OPT-COLL-FULL
           PIC S9(4) COMP-5 VALUE   00000001 .


      *****************************************************************************
      * NOTE: 60000 - 65535 reserved for internal IBM use                          *
      *****************************************************************************

      * End Of Monitor Definitions *

      **********************************************************************
      * Database Monitor Structure Size Constants
      * DON'T USE THESE CONSTANTS. THEY ARE PROVIDED FOR COMPATIBILITY ONLY
      **********************************************************************

       77 SQLM-DB2-SZ                PIC S9(4) COMP-5 VALUE 100.
      *structure SQLM-DB2

       77 SQLM-DBASE-SZ              PIC S9(4) COMP-5 VALUE 548.
      *structure SQLM-DBASE

       77 SQLM-APPL-SZ               PIC S9(4) COMP-5 VALUE 880.
      *structure SQLM-APPL

       77 SQLM-APPLINFO-SZ           PIC S9(4) COMP-5 VALUE 492.
      *structure SQLM-APPLINFO

       77 SQLM-DCS-APPLINFO-SZ       PIC S9(4) COMP-5 VALUE 260.
      *structure SQLM-DCS-APPLINFO

       77 SQLM-TABLE-HEADER-SZ       PIC S9(4) COMP-5 VALUE 356.
      *structure SQLM-TABLE-HEADER

       77 SQLM-TABLE-SZ              PIC S9(4) COMP-5 VALUE 60.
      *structure SQLM-TABLE

       77 SQLM-DBASE-LOCK-SZ         PIC S9(4) COMP-5 VALUE 352.
      *structure SQLM-DBASE-LOCK

       77 SQLM-APPL-LOCK-SZ          PIC S9(4) COMP-5 VALUE 216.
      *structure SQLM-APPL-LOCK

       77 SQLM-LOCK-SZ               PIC S9(4) COMP-5 VALUE 60.
      *structure SQLM-LOCK

       77 SQLM-TIMESTAMP-SZ          PIC S9(4) COMP-5 VALUE 8.
      *structure SQLM-TIMESTAMP

       77 SQLM-COLLECTED-SZ          PIC S9(4) COMP-5 VALUE 232.
      *structure SQLM-COLLECTED
      
      ******************************************************************
      * Database Monitor Data Structures
      ******************************************************************

      **********************************************************************
      *  Dynamic (V6) Monitor Stream structures
      **********************************************************************
       01 SQLM-HEADER-INFO.
          05 SQLM-SIZE              PIC 9(9) COMP-5.
          05 SQLM-TYPE              PIC 9(4) COMP-5.
          05 ELEMENT                PIC 9(4) COMP-5.
        
       01 SQLM-HEADER-DATA.
          05 HEADER.
             10 SQLM-SIZE           PIC 9(9) COMP-5.
             10 SQLM-TYPE           PIC 9(4) COMP-5.
             10 ELEMENT             PIC 9(4) COMP-5.
          05 SQLM-DATA                   PIC X(1) USAGE DISPLAY NATIVE.

      ******************************************************************
      *  SQL Monitor Area
      ******************************************************************

       01 SQLMA.
          05 OBJ-NUM                 PIC 9(9) COMP-5.
          05 OBJ-VAR OCCURS 0 TO 100 TIMES DEPENDING ON OBJ-NUM.
             10 AGENT-ID             PIC 9(9) COMP-5.
             10 OBJ-TYPE             PIC 9(9) COMP-5.
             10 SQLMA-OBJECT         PIC X(36) USAGE DISPLAY NATIVE.

      ******************************************************************
      * time stamp data structure
      ******************************************************************

       01 SQLM-TIMESTAMP.
          05 SECONDS                 PIC 9(9) COMP-5.
          05 MICROSEC                PIC 9(9) COMP-5.

      ******************************************************************
      * Elpased time data structure  (For CPU times)
      ******************************************************************

       01 SQLM-TIME.
          05 SECONDS                 PIC 9(9) COMP-5.
          05 MICROSEC                PIC 9(9) COMP-5.

      ******************************************************************
      * State information for each monitor group
      ******************************************************************

       01 SQLM-RECORDING-GROUP.
          05 SQLM-RECORDING-GROUP-STATES OCCURS 7 TIMES.
             10 INPUT-STATE          PIC 9(9) COMP-5.
             10 OUTPUT-STATE         PIC 9(9) COMP-5.
             10 START-TIME.
                15 SECONDS           PIC 9(9) COMP-5.
                15 MICROSEC          PIC 9(9) COMP-5.

      ******************************************************************
      * collection count data structure returned from get monitor info
      ******************************************************************

       01 SQLM-COLLECTED.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 DB2                     PIC 9(9) COMP-5.
          05 DATABASES               PIC 9(9) COMP-5.
          05 TABLE-DATABASES         PIC 9(9) COMP-5.
          05 LOCK-DATABASES          PIC 9(9) COMP-5.
          05 APPLICATIONS            PIC 9(9) COMP-5.
          05 APPLINFOS               PIC 9(9) COMP-5.
          05 DCS-APPLINFOS           PIC 9(9) COMP-5.
          05 SERVER-DB2-TYPE         PIC 9(9) COMP-5.
          05 TIME-STAMP.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 GROUP-STATES OCCURS 6.
             10 INPUT-STATE          PIC 9(9) COMP-5.
             10 OUTPUT-STATE         PIC 9(9) COMP-5.
             10 START-TIME.
                15 SECONDS           PIC 9(9) COMP-5.
                15 MICROSEC          PIC 9(9) COMP-5.
          05 SERVER-PRDID            PIC X(20) USAGE DISPLAY NATIVE.
          05 SERVER-NNAME            PIC X(20) USAGE DISPLAY NATIVE.
          05 SERVER-INSTANCE-NAME    PIC X(20) USAGE DISPLAY NATIVE.
          05 RESERVED                PIC X(22) USAGE DISPLAY NATIVE.
          05 NODE-NUMBER             PIC 9(5) COMP-5.
          05 TIME-ZONE-DISP          PIC S9(9) COMP-5.
          05 NUM-TOP-LEVEL-STRUCTS   PIC 9(9) COMP-5.
          05 TABLESPACE-DATABASES    PIC 9(9) COMP-5.
          05 SERVER-VERSION          PIC 9(9) COMP-5.
