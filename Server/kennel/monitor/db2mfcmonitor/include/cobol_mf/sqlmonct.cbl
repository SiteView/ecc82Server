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
       78 SQLM-DBMON-VERSION1 VALUE 1.

       78 SQLM-DBMON-VERSION2 VALUE 2.

       78 SQLM-DBMON-VERSION5 VALUE 3.

       78 SQLM-DBMON-VERSION5-2 VALUE 4.

       78 SQLM-DBMON-VERSION6 VALUE 5.

       78 SQLM-DBMON-VERSION7 VALUE 6.

       78 SQLM-DBMON-VERSION8 VALUE 7.

       78 SQLM-CURRENT-VERSION VALUE 7.


      *****************************************************************************
      * Snapshot class qualifiers (This indicates how the sqlma is to be           *
      * interpreted)                                                               *
      *****************************************************************************

       78 SQLM-CLASS-DEFAULT VALUE 0.

       78 SQLM-CLASS-HEALTH VALUE 1.

       78 SQLM-CLASS-HEALTH-WITH-DETAIL VALUE 2.

       78 SQLM-CLASS-MAX VALUE 2.


      *****************************************************************************
      * Snapshot requests (sqlm_obj_struct.obj_type in sqlma)                      *
      *****************************************************************************

      * Database Manager snapshot *
       78 SQLMA-DB2 VALUE 1.


      * List of connections *
       78 SQLMA-DBASE-APPLINFO VALUE 7.

       78 SQLMA-APPLINFO-ALL VALUE 11.

       78 SQLMA-DCS-APPLINFO-ALL VALUE 12.


      * Database snapshot *
       78 SQLMA-DBASE VALUE 2.

       78 SQLMA-DBASE-ALL VALUE 9.

       78 SQLMA-DCS-DBASE VALUE 34.

       78 SQLMA-DCS-DBASE-ALL VALUE 35.

       78 SQLMA-DBASE-REMOTE VALUE 37.

       78 SQLMA-DBASE-REMOTE-ALL VALUE 38.


      * Application snapshot *
       78 SQLMA-APPL-ALL VALUE 10.

       78 SQLMA-APPL VALUE 3.

       78 SQLMA-AGENT-ID VALUE 4.

       78 SQLMA-DBASE-APPLS VALUE 6.

       78 SQLMA-DCS-APPL-ALL VALUE 30.

       78 SQLMA-DCS-APPL VALUE 31.

       78 SQLMA-DCS-APPL-HANDLE VALUE 32.

       78 SQLMA-DCS-DBASE-APPLS VALUE 33.

       78 SQLMA-DBASE-APPLS-REMOTE VALUE 39.

       78 SQLMA-APPL-REMOTE-ALL VALUE 40.


      * Table snapshot *
       78 SQLMA-DBASE-TABLES VALUE 5.


      * Tablespace snapshot *
       78 SQLMA-DBASE-TABLESPACES VALUE 13.


      * Snapshot for locks *
       78 SQLMA-APPL-LOCKS VALUE 20.

       78 SQLMA-APPL-LOCKS-AGENT-ID VALUE 21.

       78 SQLMA-DBASE-LOCKS VALUE 8.


      * Bufferpool snapshot *
       78 SQLMA-DBASE-BUFFERPOOLS VALUE 22.

       78 SQLMA-BUFFERPOOLS-ALL VALUE 23.


      * Dynamic sql snapshot *
       78 SQLMA-DYNAMIC-SQL VALUE 36.


      *****************************************************************************
      * Snapshot output structure type identifiers                                 *
      *                                                                            *
      * (info_type element in the sqlm_xxx structures that are returned in the     *
      * sqlmonss() output buffer.)                                                 *
      *****************************************************************************

       78 SQLM-DB2-SS VALUE "1".

       78 SQLM-FCM-SS VALUE "E".

       78 SQLM-NODE-SS VALUE "G".

       78 SQLM-APPLINFO-SS VALUE "4".

       78 SQLM-DCS-APPLINFO-SS VALUE "A".

       78 SQLM-DBASE-SS VALUE "2".

       78 SQLM-TABLE-HEADER-SS VALUE "5".

       78 SQLM-TABLE-SS VALUE "6".

       78 SQLM-DBASE-LOCK-SS VALUE "7".

       78 SQLM-APPL-LOCK-SS VALUE "8".

       78 SQLM-LOCK-SS VALUE "9".

       78 SQLM-TABLESPACE-HEADER-SS VALUE "B".

       78 SQLM-TABLESPACE-SS VALUE "C".

       78 SQLM-APPL-SS VALUE "3".

       78 SQLM-DBASE-ROLLFWD-SS VALUE "H".

       78 SQLM-DBASE-ROLLFWD-TS-SS VALUE "I".

       78 SQLM-BUFFERPOOL-SS VALUE "J".

       78 SQLM-LOCK-WAIT-SS VALUE "K".

       78 SQLM-STMT-SS VALUE "L".

       78 SQLM-SUBSECTION-SS VALUE "M".

       78 SQLM-AGENT-SS VALUE "N".

       78 SQLM-DCS-APPL-SS VALUE "O".

       78 SQLM-DCS-APPLID-SS VALUE "P".

       78 SQLM-DCS-APPL-STATS-SS VALUE "Q".

       78 SQLM-APPL-XID-SS VALUE "R".

       78 SQLM-TPMON-INFO-SS VALUE "S".

       78 SQLM-DCS-STMT-SS VALUE "T".

       78 SQLM-DCS-DBASE-SS VALUE "U".

       78 SQLM-COLLECTED-SS VALUE "V".

       78 SQLM-DBASE-REMOTE-SS VALUE "W".

       78 SQLM-APPL-REMOTE-SS VALUE "X".


      *****************************************************************************
      * stream type ids                                                            *
      *****************************************************************************
       78 SQLM-STREAM-SNAPSHOT VALUE 1.

       78 SQLM-STREAM-EVMON VALUE 2.


      *****************************************************************************
      * size constants                                                             *
      *****************************************************************************

       78 SQLM-APPLID-SZ VALUE 32.

       78 SQLM-SEQ-SZ VALUE 4.

       78 SQLM-SH-IDENT-SZ VALUE 8.

       78 SQLM-IDENT-SZ VALUE 20.

       78 SQLM-LG-IDENT-SZ VALUE 32.

       78 SQLM-OBJECT-SZ VALUE 36.

       78 SQLM-DBPATH-SZ VALUE 256.

       78 SQLM-COMM-ADDR-SZ VALUE 32.

       78 SQLM-SERVERNAME-SZ VALUE 128.

       78 SQLM-MESSAGE-SZ VALUE 128.

       78 SQLM-STORAGE-PATH-SZ VALUE 190.


      *****************************************************************************
      * Information group state switches (sqlmon and sqlmonss functions)           *
      *****************************************************************************

       78 SQLM-OFF VALUE 0.

       78 SQLM-ON VALUE 1.

       78 SQLM-HOLD VALUE 2.


      *****************************************************************************
      * flag constant for sqlmrset()                                               *
      *****************************************************************************
       78 SQLM-UDB-DBASE VALUE 0.

       78 SQLM-UDB-ALL VALUE 1.

       78 SQLM-DCS-DBASE VALUE 2.

       78 SQLM-DCS-ALL VALUE 3.

       78 SQLM-APM VALUE 4.


      *****************************************************************************
      * Monitor switches                                                           *
      *****************************************************************************

       78 SQLM-UOW-SW VALUE 1.

       78 SQLM-STATEMENT-SW VALUE 2.

       78 SQLM-TABLE-SW VALUE 3.

       78 SQLM-BUFFER-POOL-SW VALUE 4.

       78 SQLM-LOCK-SW VALUE 5.

       78 SQLM-SORT-SW VALUE 6.

       78 SQLM-TIMESTAMP-SW VALUE 7.


       78 SQLM-NUM-GROUPS VALUE 7.

       78 SQLM-NUM-GROUPS-PREV8 VALUE 6.


      *****************************************************************************
      * Statement Types (stmt_type)                                                *
      *****************************************************************************
       78 SQLM-STATIC VALUE 1.

       78 SQLM-DYNAMIC VALUE 2.

       78 SQLM-NON-STMT VALUE 3.

       78 SQLM-STMT-TYPE-UNKNOWN VALUE 4.


      *****************************************************************************
      * Statement Operation Types (stmt_operation)                                 *
      *****************************************************************************
      * SQL operations *
       78 SQLM-PREPARE VALUE 1.

       78 SQLM-EXECUTE VALUE 2.

       78 SQLM-EXECUTE-IMMEDIATE VALUE 3.

       78 SQLM-OPEN VALUE 4.

       78 SQLM-FETCH VALUE 5.

       78 SQLM-CLOSE VALUE 6.

       78 SQLM-DESCRIBE VALUE 7.

       78 SQLM-STATIC-COMMIT VALUE 8.

       78 SQLM-STATIC-ROLLBACK VALUE 9.

       78 SQLM-FREE-LOCATOR VALUE 10.

       78 SQLM-PREP-COMMIT VALUE 11.

       78 SQLM-CALL VALUE 12.

       78 SQLM-SELECT VALUE 15.

       78 SQLM-PREP-OPEN VALUE 16.

       78 SQLM-PREP-EXEC VALUE 17.

       78 SQLM-COMPILE VALUE 18.

       78 SQLM-SET VALUE 19.


      * non-SQL operations *
       78 SQLM-RUNSTATS VALUE 20.

       78 SQLM-REORG VALUE 21.

       78 SQLM-REBIND VALUE 22.

       78 SQLM-REDIST VALUE 23.

       78 SQLM-GETTA VALUE 24.

       78 SQLM-GETAA VALUE 25.


      *****************************************************************************
      * Table Types (table_type)                                                   *
      *****************************************************************************
       78 SQLM-USER-TABLE VALUE 1.

       78 SQLM-DROPPED-TABLE VALUE 2.

       78 SQLM-TEMP-TABLE VALUE 3.

       78 SQLM-CATALOG-TABLE VALUE 4.

       78 SQLM-REORG-TABLE VALUE 5.


      *****************************************************************************
      * lock modes (lock_mode)                                                     *
      *****************************************************************************

       78 SQLM-LNON VALUE 0.

       78 SQLM-LOIS VALUE 1.

       78 SQLM-LOIX VALUE 2.

       78 SQLM-LOOS VALUE 3.

       78 SQLM-LSIX VALUE 4.

       78 SQLM-LOOX VALUE 5.

       78 SQLM-LOIN VALUE 6.

       78 SQLM-LOOZ VALUE 7.

       78 SQLM-LOOU VALUE 8.

       78 SQLM-LONS VALUE 9.

       78 SQLM-LONX VALUE 10.

       78 SQLM-LOOW VALUE 11.

       78 SQLM-LONW VALUE 12.


      *****************************************************************************
      * subsection status (ss_status)                                              *
      *****************************************************************************

       78 SQLM-SSEXEC VALUE 1.

       78 SQLM-SSTQ-WAIT-TO-RCV VALUE 2.

       78 SQLM-SSTQ-WAIT-TO-SEND VALUE 3.

       78 SQLM-SSCOMPLETED VALUE 4.


      *****************************************************************************
      * application status (appl_status)                                           *
      *****************************************************************************

       78 SQLM-CONNECTPEND VALUE 1.

       78 SQLM-CONNECTED VALUE 2.

       78 SQLM-UOWEXEC VALUE 3.

       78 SQLM-UOWWAIT VALUE 4.

       78 SQLM-LOCKWAIT VALUE 5.

       78 SQLM-COMMIT-ACT VALUE 6.

       78 SQLM-ROLLBACK-ACT VALUE 7.

       78 SQLM-RECOMP VALUE 8.

       78 SQLM-COMP VALUE 9.

       78 SQLM-INTR VALUE 10.

       78 SQLM-DISCONNECTPEND VALUE 11.

       78 SQLM-TPREP VALUE 12.

       78 SQLM-THCOMT VALUE 13.

       78 SQLM-THABRT VALUE 14.

       78 SQLM-TEND VALUE 15.

       78 SQLM-CREATE-DB VALUE 16.

       78 SQLM-RESTART VALUE 17.

       78 SQLM-RESTORE VALUE 18.

       78 SQLM-BACKUP VALUE 19.

       78 SQLM-LOAD VALUE 20.

       78 SQLM-UNLOAD VALUE 21.

       78 SQLM-IOERROR-WAIT VALUE 22.

       78 SQLM-QUIESCE-TABLESPACE VALUE 23.

       78 SQLM-WAITFOR-REMOTE VALUE 24.

       78 SQLM-REMOTE-RQST VALUE 25.

       78 SQLM-DECOUPLED VALUE 26.

       78 SQLM-ROLLBACK-TO-SAVEPOINT VALUE 27.


      *****************************************************************************
      * DCS application status (dcs_appl_status)                                   *
      *****************************************************************************

       78 SQLM-DCS-CONNECTPEND-OUTBOUND VALUE 101.

       78 SQLM-DCS-UOWWAIT-OUTBOUND VALUE 102.

       78 SQLM-DCS-UOWWAIT-INBOUND VALUE 103.

      * or gateway waiting for request*

      *****************************************************************************
      * DCS transmission chain groupings (num_transmissions_group)                 *
      *****************************************************************************

       78 SQLM-DCS-TRANS-GROUP-2 VALUE 1.

       78 SQLM-DCS-TRANS-GROUP-3TO7 VALUE 2.

       78 SQLM-DCS-TRANS-GROUP-8TO15 VALUE 3.

       78 SQLM-DCS-TRANS-GROUP-16TO64 VALUE 4.

       78 SQLM-DCS-TRANS-GROUP-GT64 VALUE 5.


      *****************************************************************************
      * Concentrator agent statuses                                                *
      *****************************************************************************
       78 SQLM-AGENT-ASSOCIATED VALUE 1.

       78 SQLM-AGENT-NOT-ASSOCIATED VALUE 2.


      *****************************************************************************
      * database status (db_status)                                                *
      *****************************************************************************

       78 SQLM-DB-ACTIVE VALUE 0.

       78 SQLM-DB-QUIESCE-PEND VALUE 1.

       78 SQLM-DB-QUIESCED VALUE 2.

       78 SQLM-DB-ROLLFWD VALUE 3.


      *****************************************************************************
      * rollforward types (rf_type)                                                *
      *****************************************************************************
       78 SQLM-ROLLFWD-TYP-DB VALUE 0.

       78 SQLM-ROLLFWD-TYP-TSPACE VALUE 1.

       78 SQLM-NO-ROLLFWD VALUE 2.


      *****************************************************************************
      * Rollforward status (rf_status)                                             *
      *****************************************************************************
       78 SQLM-ROLLFWD-STATUS-REDO VALUE 0.

       78 SQLM-ROLLFWD-STATUS-UNDO VALUE 1.

       78 SQLM-ROLLFWD-STATUS-SUCCESS VALUE 2.

       78 SQLM-ROLLFWD-STATUS-ERROR VALUE 3.

       78 SQLM-ROLLFWD-STATUS-USR-INTR VALUE 4.


      *****************************************************************************
      * tablespace types                                                           *
      *****************************************************************************
       78 SQLM-TABLESPACE-TYP-DMS VALUE 0.

       78 SQLM-TABLESPACE-TYP-SMS VALUE 1.


      *****************************************************************************
      * tablespace content type                                                    *
      *****************************************************************************
       78 SQLM-TABLESPACE-CONTENT-ANY VALUE 0.

       78 SQLM-TABLESPACE-CONTENT-LONG VALUE 1.

      * The full name of the monitor element is SQLM_TABLESPACE_CONTENT_SYSTEMP
       78 SQLM-TBSP-CONTENT-SYSTEMP VALUE 2.

      * The full name of the monitor element is SQLM_TABLESPACE_CONTENT_USRTEMP
       78 SQLM-TBSP-CONTENT-USRTEMP VALUE 3.


      *****************************************************************************
      *  reorg table defines                                                       *
      *****************************************************************************

       78 SQLM-REORG-STARTED VALUE 1.

       78 SQLM-REORG-PAUSED VALUE 2.

       78 SQLM-REORG-STOPPED VALUE 3.

       78 SQLM-REORG-COMPLETED VALUE 4.

       78 SQLM-REORG-TRUNCATE VALUE 5.


       78 SQLM-REORG-SORT VALUE 1.

       78 SQLM-REORG-BUILD VALUE 2.

       78 SQLM-REORG-REPLACE VALUE 3.

       78 SQLM-REORG-INDEX-RECREATE VALUE 4.


       78 SQLM-REORG-SUCCESS VALUE 0.

       78 SQLM-REORG-FAIL VALUE -1.


      *****************************************************************************
      * rebalancer modes                                                           *
      *****************************************************************************
       78 SQLM-TABLESPACE-NO-REBAL VALUE 0.

       78 SQLM-TABLESPACE-FWD-REBAL VALUE 1.

       78 SQLM-TABLESPACE-REV-REBAL VALUE 2.


      *****************************************************************************
      * Memory Heaps                                                               *
      *****************************************************************************
       78 SQLM-HEAP-APPLICATION VALUE 1.

       78 SQLM-HEAP-DATABASE VALUE 2.

       78 SQLM-HEAP-APPL-CONTROL VALUE 3.

       78 SQLM-HEAP-LOCK-MGR VALUE 4.

       78 SQLM-HEAP-UTILITY VALUE 5.

       78 SQLM-HEAP-STATISTICS VALUE 6.

       78 SQLM-HEAP-PACKAGE-CACHE VALUE 7.

       78 SQLM-HEAP-CAT-CACHE VALUE 8.

       78 SQLM-HEAP-DFM VALUE 9.

       78 SQLM-HEAP-QUERY VALUE 10.

       78 SQLM-HEAP-MONITOR VALUE 11.

       78 SQLM-HEAP-STATEMENT VALUE 12.

       78 SQLM-HEAP-FCMBP VALUE 13.

       78 SQLM-HEAP-IMPORT-POOL VALUE 14.

       78 SQLM-HEAP-OTHER VALUE 15.

       78 SQLM-HEAP-BP VALUE 16.

       78 SQLM-HEAP-APP-GROUP VALUE 17.

       78 SQLM-HEAP-SHARED-SORT VALUE 18.

       78 SQLM-HEAP-MIN VALUE 1.

       78 SQLM-HEAP-MAX VALUE 18.


      *****************************************************************************
      * database manager status  (db2_status)                                      *
      *****************************************************************************

       78 SQLM-DB2-ACTIVE VALUE 0.

       78 SQLM-DB2-QUIESCE-PEND VALUE 1.

       78 SQLM-DB2-QUIESCED VALUE 2.


      *****************************************************************************
      * UOW completion status (uow_comp_status)                                    *
      *****************************************************************************

       78 SQLM-UOWCOMMIT VALUE 1.

       78 SQLM-UOWROLLBACK VALUE 2.

       78 SQLM-UOWDEADLOCK VALUE 3.

       78 SQLM-UOWABEND VALUE 4.

       78 SQLM-APPL-END VALUE 5.

       78 SQLM-UOWLOCKTIMEOUT VALUE 6.

       78 SQLM-UOWUNKNOWN VALUE 7.


      *****************************************************************************
      * lock attributes (lock_attributes)                                         *
      *****************************************************************************

       78 SQLM-LOCKATTR-WAIT-FOR-AVAIL VALUE   00000001 .

       78 SQLM-LOCKATTR-ESCALATED VALUE   00000002 .

       78 SQLM-LOCKATTR-RR-IN-BLOCK VALUE   00000004 .

       78 SQLM-LOCKATTR-INSERT VALUE   00000008 .

       78 SQLM-LOCKATTR-RR VALUE   00000010 .

       78 SQLM-LOCKATTR-UPDATE-DELETE VALUE   00000020 .

       78 SQLM-LOCKATTR-ALLOW-NEW VALUE   00000040 .

       78 SQLM-LOCKATTR-NEW-REQUEST VALUE   00000080 .

       78 SQLM-LOCKATTR-DELETE-IN-BLOCK VALUE   00000000 .


      *****************************************************************************
      * lock release flags (lock_release_flags)                                    *
      *****************************************************************************

       78 SQLM-LOCKRELFLAGS-SQLCOMPILER VALUE   80000000 .

       78 SQLM-LOCKRELFLAGS-UNTRACKED VALUE   40000000 .


      *****************************************************************************
      * lock object type (lock_object_type)                                        *
      *****************************************************************************

       78 SQLM-TABLE-LOCK VALUE 1.

       78 SQLM-ROW-LOCK VALUE 2.

       78 SQLM-INTERNAL-LOCK VALUE 3.

       78 SQLM-TABLESPACE-LOCK VALUE 4.

       78 SQLM-EOT-LOCK VALUE 5.

       78 SQLM-KEYVALUE-LOCK VALUE 6.

       78 SQLM-SYSBOOT-LOCK VALUE 7.

       78 SQLM-INTERNALP-LOCK VALUE 8.

       78 SQLM-INTERNALV-LOCK VALUE 9.

       78 SQLM-INTERNALS-LOCK VALUE 10.

       78 SQLM-INTERNALJ-LOCK VALUE 11.

       78 SQLM-INTERNALL-LOCK VALUE 12.

       78 SQLM-INTERNALC-LOCK VALUE 13.

       78 SQLM-INTERNALB-LOCK VALUE 14.

       78 SQLM-INTERNALO-LOCK VALUE 15.

       78 SQLM-INTERNALT-LOCK VALUE 16.

       78 SQLM-INTERNALQ-LOCK VALUE 17.

       78 SQLM-INPLACE-REORG-LOCK VALUE 18.

       78 SQLM-BLOCK-LOCK VALUE 19.


      *****************************************************************************
      * lock status (lock_status)                                                  *
      *****************************************************************************

       78 SQLM-LRBGRNT VALUE 1.

       78 SQLM-LRBCONV VALUE 2.


      *****************************************************************************
      * client communications protocols (client_protocol)                          *
      *****************************************************************************
       78 SQLM-PROT-APPC VALUE 0.

       78 SQLM-PROT-NETBIOS VALUE 1.

       78 SQLM-PROT-APPN VALUE 2.

       78 SQLM-PROT-TCPIP VALUE 3.

       78 SQLM-PROT-CPIC VALUE 4.

       78 SQLM-PROT-IPXSPX VALUE 5.

       78 SQLM-PROT-LOCAL VALUE 6.

       78 SQLM-PROT-NPIPE VALUE 7.


      *****************************************************************************
      * Operating Systems (client_platform and server_platform)                    *
      *****************************************************************************
       78 SQLM-PLATFORM-UNKNOWN VALUE 0.

       78 SQLM-PLATFORM-OS2 VALUE 1.

       78 SQLM-PLATFORM-DOS VALUE 2.

       78 SQLM-PLATFORM-WINDOWS VALUE 3.

       78 SQLM-PLATFORM-AIX VALUE 4.

       78 SQLM-PLATFORM-NT VALUE 5.

       78 SQLM-PLATFORM-HP VALUE 6.

       78 SQLM-PLATFORM-SUN VALUE 7.

       78 SQLM-PLATFORM-MVS-DRDA VALUE 8.

       78 SQLM-PLATFORM-AS400-DRDA VALUE 9.

       78 SQLM-PLATFORM-VM-DRDA VALUE 10.

       78 SQLM-PLATFORM-VSE-DRDA VALUE 11.

       78 SQLM-PLATFORM-UNKNOWN-DRDA VALUE 12.

       78 SQLM-PLATFORM-SNI VALUE 13.

       78 SQLM-PLATFORM-MAC VALUE 14.

       78 SQLM-PLATFORM-WINDOWS95 VALUE 15.

       78 SQLM-PLATFORM-SCO VALUE 16.

       78 SQLM-PLATFORM-SGI VALUE 17.

       78 SQLM-PLATFORM-LINUX VALUE 18.

       78 SQLM-PLATFORM-DYNIX VALUE 19.

       78 SQLM-PLATFORM-AIX64 VALUE 20.

       78 SQLM-PLATFORM-SUN64 VALUE 21.

       78 SQLM-PLATFORM-HP64 VALUE 22.

       78 SQLM-PLATFORM-NT64 VALUE 23.

       78 SQLM-PLATFORM-LINUX390 VALUE 24.

       78 SQLM-PLATFORM-LINUXZ64 VALUE 25.

       78 SQLM-PLATFORM-LINUXIA64 VALUE 26.

       78 SQLM-PLATFORM-LINUXPPC VALUE 27.

       78 SQLM-PLATFORM-LINUXPPC64 VALUE 28.

       78 SQLM-PLATFORM-OS390 VALUE 29.

       78 SQLM-PLATFORM-LINUXX8664 VALUE 30.

       78 SQLM-PLATFORM-HPIA VALUE 31.

       78 SQLM-PLATFORM-HPIA64 VALUE 32.


      *****************************************************************************
      * Operating System Application Priority Type (appl_priority_type)            *
      *****************************************************************************
       78 SQLM-FIXED-PRIORITY VALUE 1.

       78 SQLM-DYNAMIC-PRIORITY VALUE 2.


      *****************************************************************************
      * Isolation levels                                                           *
      *****************************************************************************
       78 SQLM-ISOLATION-LEVEL-NONE VALUE 0.

       78 SQLM-ISOLATION-LEVEL-UR VALUE 1.

       78 SQLM-ISOLATION-LEVEL-CS VALUE 2.

       78 SQLM-ISOLATION-LEVEL-RS VALUE 3.

       78 SQLM-ISOLATION-LEVEL-RR VALUE 4.


      *****************************************************************************
      * Event Monitor Record Type Identifiers  (sqlm_event_rec_header.type)        *
      *                                                                            *
      * Used to read the records in a trace produced by an Event Monitor           *
      *****************************************************************************
       78 SQLM-EVENT-DB VALUE 1.

       78 SQLM-EVENT-CONN VALUE 2.

       78 SQLM-EVENT-TABLE VALUE 3.

       78 SQLM-EVENT-STMT VALUE 4.

       78 SQLM-EVENT-STMTTEXT VALUE 5.

       78 SQLM-EVENT-XACT VALUE 6.

       78 SQLM-EVENT-DEADLOCK VALUE 7.

       78 SQLM-EVENT-DLCONN VALUE 8.

       78 SQLM-EVENT-TABLESPACE VALUE 9.

       78 SQLM-EVENT-DBHEADER VALUE 10.

       78 SQLM-EVENT-START VALUE 11.

       78 SQLM-EVENT-CONNHEADER VALUE 12.

       78 SQLM-EVENT-OVERFLOW VALUE 13.

       78 SQLM-EVENT-BUFFERPOOL VALUE 14.

       78 SQLM-EVENT-SUBSECTION VALUE 15.

       78 SQLM-EVENT-LOG-HEADER VALUE 16.

       78 SQLM-EVENT-DBMEMUSE VALUE 17.

       78 SQLM-EVENT-CONNMEMUSE VALUE 18.

       78 SQLM-EVENT-STMT-HISTORY VALUE 19.

       78 SQLM-EVENT-DATA-VALUE VALUE 20.

       78 SQLM-EVENT-MAX VALUE 20.

      * the list                            *

      *****************************************************************************
      * Event Monitor Byte Order Identifiers (byte_order)                          *
      *****************************************************************************
       78 SQLM-LITTLE-ENDIAN VALUE 0.

       78 SQLM-BIG-ENDIAN VALUE -1.


      *****************************************************************************
      * Database Monitor Error Constants (sqlcode)                                 *
      *****************************************************************************
       78 SQLM-RC-INV-PTR VALUE -1601.

       78 SQLM-RC-INV-OBJ-TYPE VALUE -1602.

       78 SQLM-RC-NOT-SPECIFIED VALUE -1603.

       78 SQLM-RC-NOT-NULL-TERM VALUE -1604.

       78 SQLM-RC-DBNOACT VALUE 1605.

       78 SQLM-RC-BUFFER-FULL VALUE 1606.

       78 SQLM-RC-NOMEM VALUE -1607.

       78 SQLM-RC-DUPLICATE-DB VALUE 1608.

       78 SQLM-RC-REMOTE-DB VALUE -1609.

       78 SQLM-RC-INV-VALUE VALUE -1610.

       78 SQLM-RC-NO-DATA VALUE 1611.

       78 SQLM-RC-TOO-MANY-OBJECTS VALUE -1623.

       78 SQLM-RC-MULTIPLE-NODES VALUE -1624.


      *****************************************************************************
      * FCM Connection Status (connection_status)                                  *
      *****************************************************************************
       78 SQLM-FCM-CONNECT-INACTIVE VALUE 0.

       78 SQLM-FCM-CONNECT-ACTIVE VALUE 1.

       78 SQLM-FCM-CONNECT-CONGESTED VALUE 2.


      *****************************************************************************
      * Database location relative to snapshot application (db_location)           *
      *****************************************************************************
       78 SQLM-LOCAL VALUE 1.

       78 SQLM-REMOTE VALUE 2.


      *****************************************************************************
      * Node location definitions                                                  *
      *****************************************************************************
       78 SQLM-CURRENT-NODE VALUE -1.

       78 SQLM-ALL-NODES VALUE -2.


      *****************************************************************************
      * Log space usage definitions                                                *
      *****************************************************************************
       78 SQLM-LOGSPACE-INFINITE
                            VALUE -1.


      *****************************************************************************
      * Log file number definitions                                                *
      *****************************************************************************
       78 SQLM-LOGFILE-NUM-UNKNOWN VALUE -1.


      *****************************************************************************
      * Utility types associated with SQLM_ELM_UTILITY_TYPE                        *
      *****************************************************************************
       78 SQLM-UTILITY-REBALANCE VALUE 0.

       78 SQLM-UTILITY-BACKUP VALUE 1.

       78 SQLM-UTILITY-RUNSTATS VALUE 2.

       78 SQLM-UTILITY-REORG VALUE 3.

       78 SQLM-UTILITY-RESTORE VALUE 4.

       78 SQLM-UTILITY-CRASH-RECOVERY VALUE 5.

      * The full name of the monitor element is SQLM_UTILITY_ROLLFORWARD_RECOVERY
       78 SQLM-UTL-ROLLFWD-RCVRY VALUE 6.

       78 SQLM-UTILITY-LOAD VALUE 7.

      * The full name of the monitor element is SQLM_UTILITY_RESTART_RECREATE_INDEX
       78 SQLM-UTL-RESTART-INDEX VALUE 8.


      *****************************************************************************
      * Utility priority of value 0                                                *
      *****************************************************************************
       78 SQLM-UTILITY-UNTHROTTLED VALUE 0.


      *****************************************************************************
      * Progress information work metric values                                    *
      *****************************************************************************
       78 SQLM-WORK-METRIC-NOT-SUPPORT VALUE 0.

       78 SQLM-WORK-METRIC-BYTES VALUE 1.

       78 SQLM-WORK-METRIC-EXTENTS VALUE 2.

       78 SQLM-WORK-METRIC-ROWS VALUE 3.

       78 SQLM-WORK-METRIC-PAGES VALUE 4.

       78 SQLM-WORK-METRIC-INDEXES VALUE 5.


      *****************************************************************************
      * Current HADR roles of the database                                         *
      *****************************************************************************
       78 SQLM-HADR-ROLE-STANDARD VALUE 0.

       78 SQLM-HADR-ROLE-PRIMARY VALUE 1.

       78 SQLM-HADR-ROLE-STANDBY VALUE 2.


      *****************************************************************************
      * State of HADR subsystem                                                    *
      *****************************************************************************
       78 SQLM-HADR-STATE-DISCONNECTED VALUE 0.

       78 SQLM-HADR-STATE-LOCAL-CATCHUP VALUE 1.

       78 SQLM-HADR-STATE-REM-CATCH-PEND VALUE 2.

       78 SQLM-HADR-STATE-REM-CATCHUP VALUE 3.

       78 SQLM-HADR-STATE-PEER VALUE 4.


      *****************************************************************************
      * HADR synchronization mode                                                  *
      *****************************************************************************
       78 SQLM-HADR-SYNCMODE-ASYNC VALUE 0.

       78 SQLM-HADR-SYNCMODE-NEARSYNC VALUE 1.

       78 SQLM-HADR-SYNCMODE-SYNC VALUE 2.


      *****************************************************************************
      * Status of HADR primary-standby connection                                  *
      *****************************************************************************
       78 SQLM-HADR-CONN-CONNECTED VALUE 0.

       78 SQLM-HADR-CONN-CONGESTED VALUE 1.

       78 SQLM-HADR-CONN-DISCONNECTED VALUE 2.


      *****************************************************************************
      * Snapshot Monitoring defines (db2GetSnapshot, etc) for self-describing      *
      * datastream                                                                 *
      * Used by all versions of snapshot v6 and later.                             *
      *****************************************************************************
      *****************************************************************************
      * Defines for snapshot output stream format                                  *
      *****************************************************************************
       78 SQLM-STREAM-STATIC-FORMAT VALUE 0.

       78 SQLM-STREAM-DYNAMIC-FORMAT VALUE 1.


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
       78 SQLM-TYPE-HEADER VALUE 1.


      ****************Base data elements*******************************************
       78 SQLM-TYPE-STRING VALUE 50.

       78 SQLM-TYPE-U8BIT VALUE 51.

       78 SQLM-TYPE-8BIT VALUE 52.

       78 SQLM-TYPE-16BIT VALUE 53.

       78 SQLM-TYPE-U16BIT VALUE 54.

       78 SQLM-TYPE-32BIT VALUE 55.

       78 SQLM-TYPE-U32BIT VALUE 56.

       78 SQLM-TYPE-U64BIT VALUE 57.

       78 SQLM-TYPE-64BIT VALUE 58.

       78 SQLM-TYPE-HANDLE VALUE 59.


      *****************************************************************************
      * Monitor data elements (reside in the "element" field of the                *
      * sqlm_header_data                                                           *
      *****************************************************************************
      * The following group of defines relate to logical groupings of snapshot     *
      * information. They do not represent data elements, but rather groups of     *
      * data elements                                                              *
      *****************************************************************************
       78 SQLM-ELM-DB2 VALUE 1.

       78 SQLM-ELM-FCM VALUE 2.

       78 SQLM-ELM-FCM-NODE VALUE 3.

       78 SQLM-ELM-APPL-INFO VALUE 4.

       78 SQLM-ELM-APPL VALUE 5.

       78 SQLM-ELM-DCS-APPL-INFO VALUE 6.

       78 SQLM-ELM-DCS-APPL VALUE 7.

       78 SQLM-ELM-DCS-STMT VALUE 8.

       78 SQLM-ELM-SUBSECTION VALUE 9.

       78 SQLM-ELM-AGENT VALUE 10.

       78 SQLM-ELM-LOCK-WAIT VALUE 11.

       78 SQLM-ELM-DCS-DBASE VALUE 12.

       78 SQLM-ELM-DBASE VALUE 13.

       78 SQLM-ELM-ROLLFORWARD VALUE 14.

       78 SQLM-ELM-TABLE VALUE 15.

       78 SQLM-ELM-LOCK VALUE 16.

       78 SQLM-ELM-TABLESPACE VALUE 17.

       78 SQLM-ELM-BUFFERPOOL VALUE 18.

       78 SQLM-ELM-DYNSQL VALUE 19.

       78 SQLM-ELM-COLLECTED VALUE 20.

       78 SQLM-ELM-SWITCH-LIST VALUE 21.

       78 SQLM-ELM-UOW-SW VALUE 22.

       78 SQLM-ELM-STATEMENT-SW VALUE 23.

       78 SQLM-ELM-TABLE-SW VALUE 24.

       78 SQLM-ELM-BUFFPOOL-SW VALUE 25.

       78 SQLM-ELM-LOCK-SW VALUE 26.

       78 SQLM-ELM-SORT-SW VALUE 27.

       78 SQLM-ELM-TABLE-LIST VALUE 28.

       78 SQLM-ELM-TABLESPACE-LIST VALUE 29.

       78 SQLM-ELM-DYNSQL-LIST VALUE 30.

       78 SQLM-ELM-APPL-LOCK-LIST VALUE 31.

       78 SQLM-ELM-DB-LOCK-LIST VALUE 32.

       78 SQLM-ELM-STMT VALUE 33.

       78 SQLM-ELM-DBASE-REMOTE VALUE 34.

       78 SQLM-ELM-APPL-REMOTE VALUE 35.

       78 SQLM-ELM-APPL-ID-INFO VALUE 36.

       78 SQLM-ELM-STMT-TRANSMISSIONS VALUE 37.

       78 SQLM-ELM-TIMESTAMP-SW VALUE 38.

       78 SQLM-ELM-TABLE-REORG VALUE 39.

       78 SQLM-ELM-MEMORY-POOL VALUE 40.

       78 SQLM-ELM-TABLESPACE-QUIESCER VALUE 41.

       78 SQLM-ELM-TABLESPACE-CONTAINER VALUE 42.

       78 SQLM-ELM-TABLESPACE-RANGE VALUE 43.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_RANGE_CONTAINER
       78 SQLM-ELM-TBSP-RANGE-CONT VALUE 44.

       78 SQLM-ELM-TABLESPACE-NODEINFO VALUE 45.

       78 SQLM-ELM-HEALTH-INDICATOR VALUE 46.

       78 SQLM-ELM-HEALTH-INDICATOR-HIST VALUE 47.

       78 SQLM-ELM-BUFFERPOOL-NODEINFO VALUE 48.

       78 SQLM-ELM-UTILITY VALUE 49.

       78 SQLM-ELM-HI-OBJ-LIST VALUE 50.

       78 SQLM-ELM-HI-OBJ-LIST-HIST VALUE 51.

       78 SQLM-ELM-PROGRESS VALUE 52.

       78 SQLM-ELM-PROGRESS-LIST VALUE 53.

       78 SQLM-ELM-HADR VALUE 54.

       78 SQLM-ELM-DETAIL-LOG VALUE 55.

       78 SQLM-ELM-ROLLBACK-PROGRESS VALUE 56.

       78 SQLM-ELM-DB-STORAGE-GROUP VALUE 57.


       78 SQLM-MAX-LOGICAL-ELEMENT VALUE 57.


      *****************************************************************************
      * The following group of defines relate to logical groupings of event monitor*
      * information. They do not represent data elements, but rather groups of     *
      * data elements                                                              *
      *****************************************************************************
       78 SQLM-ELM-EVENT-DB VALUE 100.

       78 SQLM-ELM-EVENT-CONN VALUE 101.

       78 SQLM-ELM-EVENT-TABLE VALUE 102.

       78 SQLM-ELM-EVENT-STMT VALUE 103.

       78 SQLM-ELM-EVENT-XACT VALUE 104.

       78 SQLM-ELM-EVENT-DEADLOCK VALUE 105.

       78 SQLM-ELM-EVENT-DLCONN VALUE 106.

       78 SQLM-ELM-EVENT-TABLESPACE VALUE 107.

       78 SQLM-ELM-EVENT-DBHEADER VALUE 108.

       78 SQLM-ELM-EVENT-START VALUE 109.

       78 SQLM-ELM-EVENT-CONNHEADER VALUE 110.

       78 SQLM-ELM-EVENT-OVERFLOW VALUE 111.

       78 SQLM-ELM-EVENT-BUFFERPOOL VALUE 112.

       78 SQLM-ELM-EVENT-SUBSECTION VALUE 113.

       78 SQLM-ELM-EVENT-LOG-HEADER VALUE 114.

       78 SQLM-ELM-EVENT-CONTROL VALUE 115.

       78 SQLM-ELM-EVENT-LOCK-LIST VALUE 116.

       78 SQLM-ELM-EVENT-DETAILED-DLCONN VALUE 117.

       78 SQLM-ELM-EVENT-CONNMEMUSE VALUE 118.

       78 SQLM-ELM-EVENT-DBMEMUSE VALUE 119.

       78 SQLM-ELM-EVENT-STMT-HISTORY VALUE 120.

       78 SQLM-ELM-EVENT-DATA-VALUE VALUE 121.


      *****************************************************************************
      * Timestamps are returned as logic data elements with seconds and            *
      * microseconds returned in seperate base elements                            *
      *****************************************************************************
       78 SQLM-MIN-TIME-STAMP VALUE 200.


       78 SQLM-ELM-TIME-STAMP VALUE 200.

       78 SQLM-ELM-STATUS-CHANGE-TIME VALUE 201.

       78 SQLM-ELM-GW-CON-TIME VALUE 202.

       78 SQLM-ELM-PREV-UOW-STOP-TIME VALUE 203.

       78 SQLM-ELM-UOW-START-TIME VALUE 204.

       78 SQLM-ELM-UOW-STOP-TIME VALUE 205.

       78 SQLM-ELM-STMT-START VALUE 206.

       78 SQLM-ELM-STMT-STOP VALUE 207.

       78 SQLM-ELM-LAST-RESET VALUE 208.

       78 SQLM-ELM-DB2START-TIME VALUE 209.

       78 SQLM-ELM-DB-CONN-TIME VALUE 210.

       78 SQLM-ELM-LAST-BACKUP VALUE 211.

       78 SQLM-ELM-LOCK-WAIT-START-TIME VALUE 212.

       78 SQLM-ELM-APPL-CON-TIME VALUE 213.

       78 SQLM-ELM-CONN-COMPLETE-TIME VALUE 214.

       78 SQLM-ELM-DISCONN-TIME VALUE 215.

       78 SQLM-ELM-EVENT-TIME VALUE 216.

       78 SQLM-ELM-START-TIME VALUE 217.

       78 SQLM-ELM-STOP-TIME VALUE 218.

       78 SQLM-ELM-RF-TIMESTAMP VALUE 219.

       78 SQLM-ELM-CONN-TIME VALUE 220.

       78 SQLM-ELM-FIRST-OVERFLOW-TIME VALUE 221.

       78 SQLM-ELM-LAST-OVERFLOW-TIME VALUE 222.

       78 SQLM-ELM-GW-EXEC-TIME VALUE 223.

       78 SQLM-ELM-AGENT-USR-CPU-TIME VALUE 224.

       78 SQLM-ELM-AGENT-SYS-CPU-TIME VALUE 225.

       78 SQLM-ELM-SS-USR-CPU-TIME VALUE 226.

       78 SQLM-ELM-SS-SYS-CPU-TIME VALUE 227.

       78 SQLM-ELM-USER-CPU-TIME VALUE 228.

       78 SQLM-ELM-TOTAL-EXEC-TIME VALUE 229.

       78 SQLM-ELM-SWITCH-SET-TIME VALUE 230.

       78 SQLM-ELM-ELAPSED-EXEC-TIME VALUE 231.

       78 SQLM-ELM-SELECT-TIME VALUE 232.

       78 SQLM-ELM-INSERT-TIME VALUE 233.

       78 SQLM-ELM-UPDATE-TIME VALUE 234.

       78 SQLM-ELM-DELETE-TIME VALUE 235.

       78 SQLM-ELM-CREATE-NICKNAME-TIME VALUE 236.

       78 SQLM-ELM-PASSTHRU-TIME VALUE 237.

       78 SQLM-ELM-STORED-PROC-TIME VALUE 238.

       78 SQLM-ELM-REMOTE-LOCK-TIME VALUE 239.

       78 SQLM-ELM-NETWORK-TIME-TOP VALUE 240.

       78 SQLM-ELM-NETWORK-TIME-BOTTOM VALUE 241.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_REBALANCER_START_TIME
       78 SQLM-ELM-TBSP-REB-START-TIME VALUE 242.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_REBALANCER_RESTART_TIME
       78 SQLM-ELM-TBSP-REB-RESTART-TIME VALUE 243.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_MIN_RECOVERY_TIME
       78 SQLM-ELM-TBSP-MIN-RCVRY-TIME VALUE 244.

       78 SQLM-ELM-HI-TIMESTAMP VALUE 245.

       78 SQLM-MAX-TIME-STAMP VALUE 245.


      *****************************************************************************
      * Actual database monitor elements                                           *
      *****************************************************************************
       78 SQLM-ELM-SECONDS VALUE 300.

       78 SQLM-ELM-MICROSEC VALUE 301.

       78 SQLM-ELM-AGENT-ID VALUE 302.

       78 SQLM-ELM-SERVER-DB2-TYPE VALUE 303.

       78 SQLM-ELM-SERVER-PRDID VALUE 304.

       78 SQLM-ELM-SERVER-NNAME VALUE 305.

       78 SQLM-ELM-SERVER-INSTANCE-NAME VALUE 306.

       78 SQLM-ELM-NODE-NUMBER VALUE 307.

       78 SQLM-ELM-TIME-ZONE-DISP VALUE 308.

       78 SQLM-ELM-SERVER-VERSION VALUE 309.

       78 SQLM-ELM-APPL-STATUS VALUE 310.

       78 SQLM-ELM-CODEPAGE-ID VALUE 311.

       78 SQLM-ELM-STMT-TEXT VALUE 312.

       78 SQLM-ELM-APPL-NAME VALUE 313.

       78 SQLM-ELM-APPL-ID VALUE 314.

       78 SQLM-ELM-SEQUENCE-NO VALUE 315.

       78 SQLM-ELM-AUTH-ID VALUE 316.

       78 SQLM-ELM-PRIMARY-AUTH-ID VALUE 316.

       78 SQLM-ELM-CLIENT-NNAME VALUE 317.

       78 SQLM-ELM-CLIENT-PRDID VALUE 318.

       78 SQLM-ELM-INPUT-DB-ALIAS VALUE 319.

       78 SQLM-ELM-CLIENT-DB-ALIAS VALUE 320.

       78 SQLM-ELM-DB-NAME VALUE 321.

       78 SQLM-ELM-DB-PATH VALUE 322.

       78 SQLM-ELM-NUM-ASSOC-AGENTS VALUE 323.

       78 SQLM-ELM-COORD-NODE-NUM VALUE 324.

       78 SQLM-ELM-AUTHORITY-LVL VALUE 325.

       78 SQLM-ELM-EXECUTION-ID VALUE 326.

       78 SQLM-ELM-CORR-TOKEN VALUE 327.

       78 SQLM-ELM-CLIENT-PID VALUE 328.

       78 SQLM-ELM-CLIENT-PLATFORM VALUE 329.

       78 SQLM-ELM-CLIENT-PROTOCOL VALUE 330.

       78 SQLM-ELM-COUNTRY-CODE VALUE 331.

       78 SQLM-ELM-TERRITORY-CODE VALUE 331.

       78 SQLM-ELM-COORD-AGENT-PID VALUE 332.

       78 SQLM-ELM-GW-DB-ALIAS VALUE 333.

       78 SQLM-ELM-OUTBOUND-COMM-ADDRESS VALUE 334.

       78 SQLM-ELM-INBOUND-COMM-ADDRESS VALUE 335.

      * The full name of the monitor element is SQLM_ELM_OUTBOUND_COMM_PROTOCOL
       78 SQLM-ELM-OUTB-COMM-PROTO VALUE 336.

       78 SQLM-ELM-DCS-DB-NAME VALUE 337.

       78 SQLM-ELM-HOST-DB-NAME VALUE 338.

       78 SQLM-ELM-HOST-PRDID VALUE 339.

       78 SQLM-ELM-OUTBOUND-APPL-ID VALUE 340.

       78 SQLM-ELM-OUTBOUND-SEQUENCE-NO VALUE 341.

       78 SQLM-ELM-DCS-APPL-STATUS VALUE 342.

       78 SQLM-ELM-HOST-CCSID VALUE 343.

       78 SQLM-ELM-OUTPUT-STATE VALUE 344.

       78 SQLM-ELM-COUNT VALUE 345.

       78 SQLM-ELM-ROWS-SELECTED VALUE 346.

       78 SQLM-ELM-SQL-STMTS VALUE 347.

       78 SQLM-ELM-FAILED-SQL-STMTS VALUE 348.

       78 SQLM-ELM-COMMIT-SQL-STMTS VALUE 349.

       78 SQLM-ELM-ROLLBACK-SQL-STMTS VALUE 350.

      * The full name of the monitor element is SQLM_ELM_INBOUND_BYTES_RECEIVED
       78 SQLM-ELM-INBOUND-BYT-RECV VALUE 351.

       78 SQLM-ELM-OUTBOUND-BYTES-SENT VALUE 352.

      * The full name of the monitor element is SQLM_ELM_OUTBOUND_BYTES_RECEIVED
       78 SQLM-ELM-OUTB-BYT-RECV VALUE 353.

       78 SQLM-ELM-INBOUND-BYTES-SENT VALUE 354.

       78 SQLM-ELM-STMT-OPERATION VALUE 355.

       78 SQLM-ELM-SECTION-NUMBER VALUE 356.

       78 SQLM-ELM-LOCK-NODE VALUE 357.

       78 SQLM-ELM-CREATOR VALUE 358.

       78 SQLM-ELM-PACKAGE-NAME VALUE 359.

       78 SQLM-ELM-APPL-IDLE-TIME VALUE 360.

       78 SQLM-ELM-OPEN-CURSORS VALUE 361.

       78 SQLM-ELM-UOW-COMP-STATUS VALUE 362.

      * The full name of the monitor element is SQLM_ELM_SEQUENCE_NO_HOLDING_LK
       78 SQLM-ELM-SEQ-NO-HOLDING-LK VALUE 363.

       78 SQLM-ELM-ROLLED-BACK-AGENT-ID VALUE 364.

       78 SQLM-ELM-ROLLED-BACK-APPL-ID VALUE 365.

      * The full name of the monitor element is SQLM_ELM_ROLLED_BACK_SEQUENCE_NO
       78 SQLM-ELM-R0L-BACK-SEQ-NO VALUE 366.

       78 SQLM-ELM-XID VALUE 367.

       78 SQLM-ELM-TPMON-CLIENT-USERID VALUE 368.

       78 SQLM-ELM-TPMON-CLIENT-WKSTN VALUE 369.

       78 SQLM-ELM-TPMON-CLIENT-APP VALUE 370.

       78 SQLM-ELM-TPMON-ACC-STR VALUE 371.

       78 SQLM-ELM-QUERY-COST-ESTIMATE VALUE 372.

       78 SQLM-ELM-QUERY-CARD-ESTIMATE VALUE 373.

       78 SQLM-ELM-FETCH-COUNT VALUE 374.

       78 SQLM-ELM-GW-TOTAL-CONS VALUE 375.

       78 SQLM-ELM-GW-CUR-CONS VALUE 376.

       78 SQLM-ELM-GW-CONS-WAIT-HOST VALUE 377.

       78 SQLM-ELM-GW-CONS-WAIT-CLIENT VALUE 378.

       78 SQLM-ELM-GW-CONNECTIONS-TOP VALUE 379.

       78 SQLM-ELM-SORT-HEAP-ALLOCATED VALUE 380.

       78 SQLM-ELM-POST-THRESHOLD-SORTS VALUE 381.

       78 SQLM-ELM-PIPED-SORTS-REQUESTED VALUE 382.

       78 SQLM-ELM-PIPED-SORTS-ACCEPTED VALUE 383.

       78 SQLM-ELM-DL-CONNS VALUE 384.

       78 SQLM-ELM-REM-CONS-IN VALUE 385.

       78 SQLM-ELM-REM-CONS-IN-EXEC VALUE 386.

       78 SQLM-ELM-LOCAL-CONS VALUE 387.

       78 SQLM-ELM-LOCAL-CONS-IN-EXEC VALUE 388.

       78 SQLM-ELM-CON-LOCAL-DBASES VALUE 389.

       78 SQLM-ELM-AGENTS-REGISTERED VALUE 390.

      * The full name of the monitor element is SQLM_ELM_AGENTS_WAITING_ON_TOKEN
       78 SQLM-ELM-AGNT-WAIT-ON-TOKEN VALUE 391.

       78 SQLM-ELM-DB2-STATUS VALUE 392.

       78 SQLM-ELM-AGENTS-REGISTERED-TOP VALUE 393.

       78 SQLM-ELM-AGENTS-WAITING-TOP VALUE 394.

       78 SQLM-ELM-COMM-PRIVATE-MEM VALUE 395.

       78 SQLM-ELM-IDLE-AGENTS VALUE 396.

       78 SQLM-ELM-AGENTS-FROM-POOL VALUE 397.

      * The full name of the monitor element is SQLM_ELM_AGENTS_CREATED_EMPTY_POOL
       78 SQLM-ELM-AGNT-CREATED-EMPTY-PL VALUE 398.

       78 SQLM-ELM-AGENTS-TOP VALUE 399.

       78 SQLM-ELM-COORD-AGENTS-TOP VALUE 400.

       78 SQLM-ELM-MAX-AGENT-OVERFLOWS VALUE 401.

       78 SQLM-ELM-AGENTS-STOLEN VALUE 402.

       78 SQLM-ELM-PRODUCT-NAME VALUE 403.

       78 SQLM-ELM-COMPONENT-ID VALUE 404.

       78 SQLM-ELM-SERVICE-LEVEL VALUE 405.

      * The full name of the monitor element is SQLM_ELM_POST_THRESHOLD_HASH_JOINS
       78 SQLM-ELM-POST-THRE-HASH-JOINS VALUE 406.

       78 SQLM-ELM-BUFF-FREE VALUE 407.

       78 SQLM-ELM-BUFF-FREE-BOTTOM VALUE 408.

       78 SQLM-ELM-MA-FREE VALUE 409.

       78 SQLM-ELM-MA-FREE-BOTTOM VALUE 410.

       78 SQLM-ELM-CE-FREE VALUE 411.

       78 SQLM-ELM-CE-FREE-BOTTOM VALUE 412.

       78 SQLM-ELM-RB-FREE VALUE 413.

       78 SQLM-ELM-RB-FREE-BOTTOM VALUE 414.

       78 SQLM-ELM-CONNECTION-STATUS VALUE 416.

       78 SQLM-ELM-TOTAL-BUFFERS-SENT VALUE 417.

       78 SQLM-ELM-TOTAL-BUFFERS-RCVD VALUE 418.

       78 SQLM-ELM-LOCKS-HELD VALUE 419.

       78 SQLM-ELM-LOCK-WAITS VALUE 420.

       78 SQLM-ELM-LOCK-WAIT-TIME VALUE 421.

       78 SQLM-ELM-LOCK-LIST-IN-USE VALUE 422.

       78 SQLM-ELM-DEADLOCKS VALUE 423.

       78 SQLM-ELM-LOCK-ESCALS VALUE 424.

       78 SQLM-ELM-X-LOCK-ESCALS VALUE 425.

       78 SQLM-ELM-LOCKS-WAITING VALUE 426.

       78 SQLM-ELM-TOTAL-SORTS VALUE 427.

       78 SQLM-ELM-TOTAL-SORT-TIME VALUE 428.

       78 SQLM-ELM-SORT-OVERFLOWS VALUE 429.

       78 SQLM-ELM-ACTIVE-SORTS VALUE 430.

       78 SQLM-ELM-POOL-DATA-L-READS VALUE 431.

       78 SQLM-ELM-POOL-DATA-P-READS VALUE 432.

       78 SQLM-ELM-POOL-DATA-WRITES VALUE 433.

       78 SQLM-ELM-POOL-INDEX-L-READS VALUE 434.

       78 SQLM-ELM-POOL-INDEX-P-READS VALUE 435.

       78 SQLM-ELM-POOL-INDEX-WRITES VALUE 436.

       78 SQLM-ELM-POOL-READ-TIME VALUE 437.

       78 SQLM-ELM-POOL-WRITE-TIME VALUE 438.

       78 SQLM-ELM-FILES-CLOSED VALUE 439.

       78 SQLM-ELM-DYNAMIC-SQL-STMTS VALUE 440.

       78 SQLM-ELM-STATIC-SQL-STMTS VALUE 441.

       78 SQLM-ELM-SELECT-SQL-STMTS VALUE 442.

       78 SQLM-ELM-DDL-SQL-STMTS VALUE 443.

       78 SQLM-ELM-UID-SQL-STMTS VALUE 444.

       78 SQLM-ELM-INT-AUTO-REBINDS VALUE 445.

       78 SQLM-ELM-INT-ROWS-DELETED VALUE 446.

       78 SQLM-ELM-INT-ROWS-UPDATED VALUE 447.

       78 SQLM-ELM-INT-COMMITS VALUE 448.

       78 SQLM-ELM-INT-ROLLBACKS VALUE 449.

      * The full name of the monitor element is SQLM_ELM_INT_DEADLOCK_ROLLBACKS
       78 SQLM-ELM-INT-DEADLOCK-RBCK VALUE 450.

       78 SQLM-ELM-ROWS-DELETED VALUE 451.

       78 SQLM-ELM-ROWS-INSERTED VALUE 452.

       78 SQLM-ELM-ROWS-UPDATED VALUE 453.

       78 SQLM-ELM-BINDS-PRECOMPILES VALUE 454.

       78 SQLM-ELM-LOCKS-HELD-TOP VALUE 455.

      * The full name of the monitor element is SQLM_ELM_NUM_NODES_IN_DB2_INSTANCE
       78 SQLM-ELM-NUM-NODE-IN-DB2-INST VALUE 456.

       78 SQLM-ELM-TOTAL-CONS VALUE 457.

       78 SQLM-ELM-APPLS-CUR-CONS VALUE 458.

       78 SQLM-ELM-APPLS-IN-DB2 VALUE 459.

       78 SQLM-ELM-SEC-LOG-USED-TOP VALUE 460.

       78 SQLM-ELM-TOT-LOG-USED-TOP VALUE 461.

       78 SQLM-ELM-SEC-LOGS-ALLOCATED VALUE 462.

      * The full name of the monitor element is SQLM_ELM_POOL_ASYNC_INDEX_READS
       78 SQLM-ELM-PL-ASYNC-IDX-RD VALUE 463.

       78 SQLM-ELM-POOL-DATA-TO-ESTORE VALUE 464.

       78 SQLM-ELM-POOL-INDEX-TO-ESTORE VALUE 465.

      * The full name of the monitor element is SQLM_ELM_POOL_INDEX_FROM_ESTORE
       78 SQLM-ELM-PL-IDX-FROM-ESTORE VALUE 466.

       78 SQLM-ELM-POOL-DATA-FROM-ESTORE VALUE 467.

       78 SQLM-ELM-DB-STATUS VALUE 468.

       78 SQLM-ELM-LOCK-TIMEOUTS VALUE 469.

       78 SQLM-ELM-CONNECTIONS-TOP VALUE 470.

       78 SQLM-ELM-DB-HEAP-TOP VALUE 471.

       78 SQLM-ELM-POOL-ASYNC-DATA-READS VALUE 472.

      * The full name of the monitor element is SQLM_ELM_POOL_ASYNC_DATA_WRITES
       78 SQLM-ELM-PL-ASYNC-DATA-WR VALUE 473.

      * The full name of the monitor element is SQLM_ELM_POOL_ASYNC_INDEX_WRITES
       78 SQLM-ELM-PL-ASYNC-IDX-WR VALUE 474.

       78 SQLM-ELM-POOL-ASYNC-READ-TIME VALUE 475.

       78 SQLM-ELM-POOL-ASYNC-WRITE-TIME VALUE 476.

      * The full name of the monitor element is SQLM_ELM_POOL_ASYNC_DATA_READ_REQS
       78 SQLM-ELM-PL-ASYNC-DATA-RD-REQS VALUE 477.

       78 SQLM-ELM-POOL-LSN-GAP-CLNS VALUE 478.

      * The full name of the monitor element is SQLM_ELM_POOL_DRTY_PG_STEAL_CLNS
       78 SQLM-ELM-PL-DRTY-PG-STEAL-CLNS VALUE 479.

      * The full name of the monitor element is SQLM_ELM_POOL_DRTY_PG_THRSH_CLNS
       78 SQLM-ELM-PL-DRTY-PG-THRSH-CLNS VALUE 480.

       78 SQLM-ELM-DIRECT-READS VALUE 481.

       78 SQLM-ELM-DIRECT-WRITES VALUE 482.

       78 SQLM-ELM-DIRECT-READ-REQS VALUE 483.

       78 SQLM-ELM-DIRECT-WRITE-REQS VALUE 484.

       78 SQLM-ELM-DIRECT-READ-TIME VALUE 485.

       78 SQLM-ELM-DIRECT-WRITE-TIME VALUE 486.

       78 SQLM-ELM-INT-ROWS-INSERTED VALUE 487.

       78 SQLM-ELM-LOG-READS VALUE 488.

       78 SQLM-ELM-LOG-WRITES VALUE 489.

       78 SQLM-ELM-PKG-CACHE-LOOKUPS VALUE 490.

       78 SQLM-ELM-PKG-CACHE-INSERTS VALUE 491.

       78 SQLM-ELM-CAT-CACHE-LOOKUPS VALUE 492.

       78 SQLM-ELM-CAT-CACHE-INSERTS VALUE 493.

       78 SQLM-ELM-CAT-CACHE-OVERFLOWS VALUE 494.

       78 SQLM-ELM-CAT-CACHE-HEAP-FULL VALUE 495.

       78 SQLM-ELM-CATALOG-NODE VALUE 496.

       78 SQLM-ELM-TOTAL-SEC-CONS VALUE 497.

       78 SQLM-ELM-DB-LOCATION VALUE 498.

       78 SQLM-ELM-SERVER-PLATFORM VALUE 499.

       78 SQLM-ELM-CATALOG-NODE-NAME VALUE 500.

       78 SQLM-ELM-PREFETCH-WAIT-TIME VALUE 501.

       78 SQLM-ELM-APPL-SECTION-LOOKUPS VALUE 502.

       78 SQLM-ELM-APPL-SECTION-INSERTS VALUE 503.

       78 SQLM-ELM-TOTAL-HASH-JOINS VALUE 504.

       78 SQLM-ELM-TOTAL-HASH-LOOPS VALUE 505.

       78 SQLM-ELM-HASH-JOIN-OVERFLOWS VALUE 506.

      * The full name of the monitor element is SQLM_ELM_HASH_JOIN_SMALL_OVERFLOWS
       78 SQLM-ELM-HASH-JOIN-SMALL-OWFLW VALUE 507.

       78 SQLM-ELM-UOW-LOCK-WAIT-TIME VALUE 508.

       78 SQLM-ELM-STMT-TYPE VALUE 509.

       78 SQLM-ELM-CURSOR-NAME VALUE 510.

       78 SQLM-ELM-UOW-LOG-SPACE-USED VALUE 511.

       78 SQLM-ELM-OPEN-REM-CURS VALUE 512.

       78 SQLM-ELM-OPEN-REM-CURS-BLK VALUE 513.

       78 SQLM-ELM-REJ-CURS-BLK VALUE 514.

       78 SQLM-ELM-ACC-CURS-BLK VALUE 515.

       78 SQLM-ELM-VERSION VALUE 516.

       78 SQLM-ELM-EVENT-MONITOR-NAME VALUE 517.

       78 SQLM-ELM-SQL-REQS-SINCE-COMMIT VALUE 518.

       78 SQLM-ELM-BYTE-ORDER VALUE 520.

       78 SQLM-ELM-PREP-TIME-WORST VALUE 521.

       78 SQLM-ELM-ROWS-READ VALUE 522.

       78 SQLM-ELM-ROWS-WRITTEN VALUE 523.

       78 SQLM-ELM-OPEN-LOC-CURS VALUE 524.

       78 SQLM-ELM-OPEN-LOC-CURS-BLK VALUE 525.

       78 SQLM-ELM-COORD-NODE VALUE 526.

       78 SQLM-ELM-NUM-AGENTS VALUE 527.

       78 SQLM-ELM-ASSOCIATED-AGENTS-TOP VALUE 528.

       78 SQLM-ELM-APPL-PRIORITY VALUE 529.

       78 SQLM-ELM-APPL-PRIORITY-TYPE VALUE 530.

       78 SQLM-ELM-DEGREE-PARALLELISM VALUE 531.

       78 SQLM-ELM-STMT-SORTS VALUE 532.

       78 SQLM-ELM-STMT-USR-CPU-TIME VALUE 533.

       78 SQLM-ELM-STMT-SYS-CPU-TIME VALUE 534.

       78 SQLM-ELM-SS-NUMBER VALUE 535.

       78 SQLM-ELM-SS-STATUS VALUE 536.

       78 SQLM-ELM-SS-NODE-NUMBER VALUE 537.

       78 SQLM-ELM-SS-EXEC-TIME VALUE 538.

       78 SQLM-ELM-PREP-TIME-BEST VALUE 539.

       78 SQLM-ELM-NUM-COMPILATIONS VALUE 540.

       78 SQLM-ELM-TQ-NODE-WAITED-FOR VALUE 541.

       78 SQLM-ELM-TQ-WAIT-FOR-ANY VALUE 542.

       78 SQLM-ELM-TQ-ID-WAITING-ON VALUE 543.

       78 SQLM-ELM-TQ-TOT-SEND-SPILLS VALUE 544.

       78 SQLM-ELM-TQ-CUR-SEND-SPILLS VALUE 545.

       78 SQLM-ELM-TQ-MAX-SEND-SPILLS VALUE 546.

       78 SQLM-ELM-TQ-ROWS-READ VALUE 547.

       78 SQLM-ELM-TQ-ROWS-WRITTEN VALUE 548.

       78 SQLM-ELM-AGENT-PID VALUE 549.

       78 SQLM-ELM-LOCK-ESCALATION VALUE 550.

       78 SQLM-ELM-SUBSECTION-NUMBER VALUE 551.

       78 SQLM-ELM-LOCK-MODE VALUE 552.

       78 SQLM-ELM-LOCK-OBJECT-TYPE VALUE 553.

       78 SQLM-ELM-NUM-EXECUTIONS VALUE 554.

       78 SQLM-ELM-TABLE-NAME VALUE 555.

       78 SQLM-ELM-TABLE-SCHEMA VALUE 556.

       78 SQLM-ELM-TABLESPACE-NAME VALUE 557.

       78 SQLM-ELM-AGENT-ID-HOLDING-LK VALUE 558.

       78 SQLM-ELM-APPL-ID-HOLDING-LK VALUE 559.

       78 SQLM-ELM-TABLE-FILE-ID VALUE 561.

       78 SQLM-ELM-TABLE-TYPE VALUE 562.

       78 SQLM-ELM-OVERFLOW-ACCESSES VALUE 563.

       78 SQLM-ELM-PAGE-REORGS VALUE 564.

       78 SQLM-ELM-SQLCABC VALUE 565.

       78 SQLM-ELM-LOCK-STATUS VALUE 566.

       78 SQLM-ELM-LOCK-OBJECT-NAME VALUE 567.

       78 SQLM-ELM-RF-TYPE VALUE 568.

       78 SQLM-ELM-RF-LOG-NUM VALUE 569.

       78 SQLM-ELM-RF-STATUS VALUE 570.

       78 SQLM-ELM-TS-NAME VALUE 571.

       78 SQLM-ELM-BP-NAME VALUE 572.

       78 SQLM-ELM-STMT-NODE-NUMBER VALUE 573.

       78 SQLM-ELM-PARTIAL-RECORD VALUE 574.

       78 SQLM-ELM-SYSTEM-CPU-TIME VALUE 575.

       78 SQLM-ELM-SQLCA VALUE 576.

       78 SQLM-ELM-SQLCODE VALUE 577.

       78 SQLM-ELM-SQLERRML VALUE 578.

       78 SQLM-ELM-SQLERRMC VALUE 579.

       78 SQLM-ELM-SQLERRP VALUE 580.

       78 SQLM-ELM-SQLERRD VALUE 581.

       78 SQLM-ELM-SQLWARN VALUE 582.

       78 SQLM-ELM-SQLSTATE VALUE 583.

       78 SQLM-ELM-UOW-STATUS VALUE 584.

       78 SQLM-ELM-TOTAL-SYS-CPU-TIME VALUE 585.

       78 SQLM-ELM-TOTAL-USR-CPU-TIME VALUE 586.

       78 SQLM-ELM-LOCK-MODE-REQUESTED VALUE 587.

       78 SQLM-ELM-INACTIVE-GW-AGENTS VALUE 588.

       78 SQLM-ELM-NUM-GW-CONN-SWITCHES VALUE 589.

       78 SQLM-ELM-GW-COMM-ERRORS VALUE 590.

       78 SQLM-ELM-GW-COMM-ERROR-TIME VALUE 591.

       78 SQLM-ELM-GW-CON-START-TIME VALUE 592.

       78 SQLM-ELM-CON-RESPONSE-TIME VALUE 593.

       78 SQLM-ELM-CON-ELAPSED-TIME VALUE 594.

       78 SQLM-ELM-HOST-RESPONSE-TIME VALUE 595.

      * The full name of the monitor element is SQLM_ELM_PKG_CACHE_NUM_OVERFLOWS
       78 SQLM-ELM-PKG-CACHE-NUM-OWFLW VALUE 596.

       78 SQLM-ELM-PKG-CACHE-SIZE-TOP VALUE 597.

       78 SQLM-ELM-APPL-ID-OLDEST-XACT VALUE 598.

       78 SQLM-ELM-TOTAL-LOG-USED VALUE 599.

       78 SQLM-ELM-TOTAL-LOG-AVAILABLE VALUE 600.

       78 SQLM-ELM-STMT-ELAPSED-TIME VALUE 601.

       78 SQLM-ELM-UOW-ELAPSED-TIME VALUE 602.

       78 SQLM-ELM-SQLCAID VALUE 603.

      * The full name of the monitor element is SQLM_ELM_SMALLEST_LOG_AVAIL_NODE
       78 SQLM-ELM-SMALLEST-LOG-AVL-NODE VALUE 604.

       78 SQLM-ELM-DISCONNECTS VALUE 605.

       78 SQLM-ELM-CREATE-NICKNAME VALUE 606.

       78 SQLM-ELM-PASSTHRUS VALUE 607.

       78 SQLM-ELM-STORED-PROCS VALUE 608.

       78 SQLM-ELM-SP-ROWS-SELECTED VALUE 609.

       78 SQLM-ELM-DATASOURCE-NAME VALUE 610.

       78 SQLM-ELM-REMOTE-LOCKS VALUE 611.

       78 SQLM-ELM-BLOCKING-CURSOR VALUE 612.

      * The full name of the monitor element is SQLM_ELM_OUTBOUND_BLOCKING_CURSOR
       78 SQLM-ELM-OUTB-BLOCKING-CURSOR VALUE 613.

       78 SQLM-ELM-INSERT-SQL-STMTS VALUE 614.

       78 SQLM-ELM-UPDATE-SQL-STMTS VALUE 615.

       78 SQLM-ELM-DELETE-SQL-STMTS VALUE 616.

       78 SQLM-ELM-UNREAD-PREFETCH-PAGES VALUE 617.

       78 SQLM-ELM-AGENT-STATUS VALUE 618.

       78 SQLM-ELM-NUM-TRANSMISSIONS VALUE 619.

      * The full name of the monitor element is SQLM_ELM_OUTBOUND_BYTES_SENT_TOP
       78 SQLM-ELM-OUTB-BYT-SENT-TOP VALUE 620.

      * The full name of the monitor element is SQLM_ELM_OUTBOUND_BYTES_RECEIVED_TOP
       78 SQLM-ELM-OUTB-BYT-RECV-TOP VALUE 621.

      * The full name of the monitor element is SQLM_ELM_OUTBOUND_BYTES_SENT_BOTTOM
       78 SQLM-ELM-OUTB-BYT-SENT-BOT VALUE 622.

      * The full name of the monitor element is SQLM_ELM_OUTBOUND_BYTES_RECEIVED_BOTTOM
       78 SQLM-ELM-OUTB-BYT-RECV-BOT VALUE 623.


      *  The following elements represent counts of the number of transmissions    *
      *  whose size in bytes is less than or equal to the number in the name.      *
       78 SQLM-ELM-MAX-DATA-SENT-128 VALUE 624.

       78 SQLM-ELM-MAX-DATA-SENT-256 VALUE 625.

       78 SQLM-ELM-MAX-DATA-SENT-512 VALUE 626.

       78 SQLM-ELM-MAX-DATA-SENT-1024 VALUE 627.

       78 SQLM-ELM-MAX-DATA-SENT-2048 VALUE 628.

       78 SQLM-ELM-MAX-DATA-SENT-4096 VALUE 629.

       78 SQLM-ELM-MAX-DATA-SENT-8192 VALUE 630.

       78 SQLM-ELM-MAX-DATA-SENT-16384 VALUE 631.

       78 SQLM-ELM-MAX-DATA-SENT-31999 VALUE 632.

       78 SQLM-ELM-MAX-DATA-SENT-64000 VALUE 633.

       78 SQLM-ELM-MAX-DATA-SENT-GT64000 VALUE 634.

       78 SQLM-ELM-MAX-DATA-RECEIVED-128 VALUE 635.

       78 SQLM-ELM-MAX-DATA-RECEIVED-256 VALUE 636.

       78 SQLM-ELM-MAX-DATA-RECEIVED-512 VALUE 637.

      * The full name of the monitor element is SQLM_ELM_MAX_DATA_RECEIVED_1024
       78 SQLM-ELM-MAX-DATA-RECV-1024 VALUE 638.

      * The full name of the monitor element is SQLM_ELM_MAX_DATA_RECEIVED_2048
       78 SQLM-ELM-MAX-DATA-RECV-2048 VALUE 639.

      * The full name of the monitor element is SQLM_ELM_MAX_DATA_RECEIVED_4096
       78 SQLM-ELM-MAX-DATA-RECV-4096 VALUE 640.

      * The full name of the monitor element is SQLM_ELM_MAX_DATA_RECEIVED_8192
       78 SQLM-ELM-MAX-DATA-RECV-8192 VALUE 641.

      * The full name of the monitor element is SQLM_ELM_MAX_DATA_RECEIVED_16384
       78 SQLM-ELM-MAX-DATA-RECV-16384 VALUE 642.

      * The full name of the monitor element is SQLM_ELM_MAX_DATA_RECEIVED_31999
       78 SQLM-ELM-MAX-DATA-RECV-31999 VALUE 643.

      * The full name of the monitor element is SQLM_ELM_MAX_DATA_RECEIVED_64000
       78 SQLM-ELM-MAX-DATA-RECV-64000 VALUE 644.

      * The full name of the monitor element is SQLM_ELM_MAX_DATA_RECEIVED_GT64000
       78 SQLM-ELM-MAX-DATA-RECV-GT64000 VALUE 645.


      *  The following elements represent counts of the number of transmissions    *
      *  whose network transmission time is less than the time (in milliseconds)   *
      *  in the name.                                                              *
       78 SQLM-ELM-MAX-TIME-2-MS VALUE 646.

       78 SQLM-ELM-MAX-TIME-4-MS VALUE 647.

       78 SQLM-ELM-MAX-TIME-8-MS VALUE 648.

       78 SQLM-ELM-MAX-TIME-16-MS VALUE 649.

       78 SQLM-ELM-MAX-TIME-32-MS VALUE 650.

       78 SQLM-ELM-MAX-TIME-GT32-MS VALUE 651.


      * Deadlock Identification elements.                                          *
       78 SQLM-ELM-DEADLOCK-ID VALUE 652.

       78 SQLM-ELM-DEADLOCK-NODE VALUE 653.


      * Deadlock Participant numbers                                               *
       78 SQLM-ELM-PARTICIPANT-NO VALUE 654.

      * The full name of the monitor element is SQLM_ELM_PARTICIPANT_NO_HOLDING_LK
       78 SQLM-ELM-PARTCP-NO-HOLDING-LK VALUE 655.

      * The full name of the monitor element is SQLM_ELM_ROLLED_BACK_PARTICIPANT_NO
       78 SQLM-ELM-R0L-BACK-PARTCP-NO VALUE 656.


      * Please make these 6 element always consecutive, sqlmetart.C relys on this  *
       78 SQLM-ELM-SQLERRD1 VALUE 657.

       78 SQLM-ELM-SQLERRD2 VALUE 658.

       78 SQLM-ELM-SQLERRD3 VALUE 659.

       78 SQLM-ELM-SQLERRD4 VALUE 660.

       78 SQLM-ELM-SQLERRD5 VALUE 661.

       78 SQLM-ELM-SQLERRD6 VALUE 662.


      * Event monitor new elements added  *
       78 SQLM-ELM-EVMON-ACTIVATES VALUE 663.

       78 SQLM-ELM-EVMON-FLUSHES VALUE 664.

       78 SQLM-ELM-SQL-REQ-ID VALUE 665.


      * New elements added for WTT control table                                    *
       78 SQLM-ELM-MESSAGE VALUE 666.

       78 SQLM-ELM-MESSAGE-TIME VALUE 667.


      * Elements for dynamic bufferpool                                            *
       78 SQLM-ELM-VECTORED-IOS VALUE 668.

      * The full name of the monitor element is SQLM_ELM_PAGES_FROM_VECTORED_IOS
       78 SQLM-ELM-PG-FROM-VECT-IOS VALUE 669.

       78 SQLM-ELM-BLOCK-IOS VALUE 670.

       78 SQLM-ELM-PAGES-FROM-BLOCK-IOS VALUE 671.

       78 SQLM-ELM-PHYSICAL-PAGE-MAPS VALUE 672.


      * Specifies the number of locks in the detailed dlconn lock list             *
       78 SQLM-ELM-LOCKS-IN-LIST VALUE 673.


      * Elements for table reorg                                                   *
       78 SQLM-ELM-REORG-PHASE VALUE 674.

       78 SQLM-ELM-REORG-MAX-PHASE VALUE 675.

       78 SQLM-ELM-REORG-CURRENT-COUNTER VALUE 676.

       78 SQLM-ELM-REORG-MAX-COUNTER VALUE 677.

       78 SQLM-ELM-REORG-TYPE VALUE 678.

       78 SQLM-ELM-REORG-STATUS VALUE 679.

       78 SQLM-ELM-REORG-COMPLETION VALUE 680.

       78 SQLM-ELM-REORG-START VALUE 681.

       78 SQLM-ELM-REORG-END VALUE 682.

       78 SQLM-ELM-REORG-PHASE-START VALUE 683.

       78 SQLM-ELM-REORG-INDEX-ID VALUE 684.

       78 SQLM-ELM-REORG-TBSPC-ID VALUE 685.


      * Pool memory elements                                                        *
       78 SQLM-ELM-POOL-ID VALUE 686.

       78 SQLM-ELM-POOL-CUR-SIZE VALUE 687.

       78 SQLM-ELM-POOL-CONFIG-SIZE VALUE 688.

       78 SQLM-ELM-POOL-MAX-SIZE VALUE 688.

       78 SQLM-ELM-POOL-WATERMARK VALUE 689.


      * Elements for tablespace containers                                         *
       78 SQLM-ELM-TABLESPACE-ID VALUE 690.

       78 SQLM-ELM-TABLESPACE-TYPE VALUE 691.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_CONTENT_TYPE
       78 SQLM-ELM-TBSP-CONTENT-TYPE VALUE 692.

       78 SQLM-ELM-TABLESPACE-STATE VALUE 693.

       78 SQLM-ELM-TABLESPACE-PAGE-SIZE VALUE 694.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_EXTENT_SIZE
       78 SQLM-ELM-TBSP-EXT-SIZE VALUE 695.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_PREFETCH_SIZE
       78 SQLM-ELM-TBSP-PREFETCH-SIZE VALUE 696.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_CUR_POOL_ID
       78 SQLM-ELM-TBSP-CUR-PL-ID VALUE 697.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_NEXT_POOL_ID
       78 SQLM-ELM-TBSP-NEXT-PL-ID VALUE 698.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_TOTAL_PAGES
       78 SQLM-ELM-TBSP-TOTAL-PG VALUE 699.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_USABLE_PAGES
       78 SQLM-ELM-TBSP-USABLE-PG VALUE 700.

       78 SQLM-ELM-TABLESPACE-USED-PAGES VALUE 701.

       78 SQLM-ELM-TABLESPACE-FREE-PAGES VALUE 702.

       78 SQLM-ELM-TABLESPACE-PAGE-TOP VALUE 703.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_PENDING_FREE_PAGES
       78 SQLM-ELM-TBSP-PENDING-FREE-PG VALUE 704.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_REBALANCER_MODE
       78 SQLM-ELM-TBSP-REB-MODE VALUE 705.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_REBALANCER_EXTENTS_REMAINING
       78 SQLM-ELM-TBSP-REB-EXT-REM VALUE 706.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_REBALANCER_EXTENTS_PROCESSED
       78 SQLM-ELM-TBSP-REB-EXT-PRO VALUE 707.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_REBALANCER_LAST_EXTENT_MOVED
       78 SQLM-ELM-TBSP-REB-LAST-EXT-MVD VALUE 708.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_REBALANCER_PRIORITY
       78 SQLM-ELM-TBSP-REB-PRIORITY VALUE 709.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_NUM_QUIESCERS
       78 SQLM-ELM-TBSP-NUM-QUIESCERS VALUE 710.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_STATE_CHANGE_OBJECT_ID
       78 SQLM-ELM-TBSP-ST-CHG-OBJ-ID VALUE 711.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_STATE_CHANGE_TS_ID
       78 SQLM-ELM-TBSP-ST-CHG-TS-ID VALUE 712.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_NUM_CONTAINERS
       78 SQLM-ELM-TBSP-NUM-CONTAINERS VALUE 713.

       78 SQLM-ELM-TABLESPACE-NUM-RANGES VALUE 714.

       78 SQLM-ELM-QUIESCER-STATE VALUE 715.

       78 SQLM-ELM-QUIESCER-AGENT-ID VALUE 716.

       78 SQLM-ELM-QUIESCER-TS-ID VALUE 717.

       78 SQLM-ELM-QUIESCER-OBJ-ID VALUE 718.

       78 SQLM-ELM-QUIESCER-AUTH-ID VALUE 719.

       78 SQLM-ELM-CONTAINER-ID VALUE 720.

       78 SQLM-ELM-CONTAINER-TYPE VALUE 721.

       78 SQLM-ELM-CONTAINER-TOTAL-PAGES VALUE 722.

      * The full name of the monitor element is SQLM_ELM_CONTAINER_USABLE_PAGES
       78 SQLM-ELM-CONT-USABLE-PG VALUE 723.

       78 SQLM-ELM-CONTAINER-STRIPE-SET VALUE 724.

       78 SQLM-ELM-CONTAINER-ACCESSIBLE VALUE 725.

       78 SQLM-ELM-CONTAINER-NAME VALUE 726.

      * The full name of the monitor element is SQLM_ELM_RANGE_STRIPE_SET_NUMBER
       78 SQLM-ELM-RANGE-STRIPE-SET-NUM VALUE 727.

       78 SQLM-ELM-RANGE-NUMBER VALUE 728.

       78 SQLM-ELM-RANGE-OFFSET VALUE 729.

       78 SQLM-ELM-RANGE-MAX-PAGE-NUMBER VALUE 730.

       78 SQLM-ELM-RANGE-MAX-EXTENT VALUE 731.

       78 SQLM-ELM-RANGE-START-STRIPE VALUE 732.

       78 SQLM-ELM-RANGE-END-STRIPE VALUE 733.

       78 SQLM-ELM-RANGE-ADJUSTMENT VALUE 734.

       78 SQLM-ELM-RANGE-NUM-CONTAINERS VALUE 735.

       78 SQLM-ELM-RANGE-CONTAINER-ID VALUE 736.


       78 SQLM-ELM-CONSISTENCY-TOKEN VALUE 737.

       78 SQLM-ELM-PACKAGE-VERSION-ID VALUE 738.


       78 SQLM-ELM-LOCK-NAME VALUE 739.

       78 SQLM-ELM-LOCK-COUNT VALUE 740.

       78 SQLM-ELM-LOCK-HOLD-COUNT VALUE 741.

       78 SQLM-ELM-LOCK-ATTRIBUTES VALUE 742.

       78 SQLM-ELM-LOCK-RELEASE-FLAGS VALUE 743.

       78 SQLM-ELM-LOCK-CURRENT-MODE VALUE 744.

       78 SQLM-ELM-TABLESPACE-FS-CACHING VALUE 745.


      *****************************************************************************
      * Dynamic Bufferpool                                                         *
      *****************************************************************************
       78 SQLM-ELM-BP-TBSP-USE-COUNT VALUE 751.

      * The full name of the monitor element is SQLM_ELM_BP_PAGES_LEFT_TO_REMOVE
       78 SQLM-ELM-BP-PG-LEFT-TO-REMOVE VALUE 752.

       78 SQLM-ELM-BP-CUR-BUFFSZ VALUE 753.

       78 SQLM-ELM-BP-NEW-BUFFSZ VALUE 754.


       78 SQLM-ELM-SORT-HEAP-TOP VALUE 755.

      * The full name of the monitor element is SQLM_ELM_SORT_SHRHEAP_ALLOCATED
       78 SQLM-ELM-SRT-SHRHEAP-ALLC VALUE 756.

       78 SQLM-ELM-SORT-SHRHEAP-TOP VALUE 757.


      *****************************************************************************
      * APM Interaction                                                            *
      *****************************************************************************
      * The full name of the monitor element is SQLM_ELM_SHR_WORKSPACE_SIZE_TOP
       78 SQLM-ELM-SHR-WKSPC-SIZE-TOP VALUE 758.

      * The full name of the monitor element is SQLM_ELM_SHR_WORKSPACE_NUM_OVERFLOWS
       78 SQLM-ELM-SHR-WKSPC-NUM-OWFLW VALUE 759.

      * The full name of the monitor element is SQLM_ELM_SHR_WORKSPACE_SECTION_LOOKUPS
       78 SQLM-ELM-SHR-WKSPC-SEC-LKP VALUE 760.

      * The full name of the monitor element is SQLM_ELM_SHR_WORKSPACE_SECTION_INSERTS
       78 SQLM-ELM-SHR-WKSPC-SEC-INS VALUE 761.

      * The full name of the monitor element is SQLM_ELM_PRIV_WORKSPACE_SIZE_TOP
       78 SQLM-ELM-PRIV-WKSPC-SIZE-TOP VALUE 762.

      * The full name of the monitor element is SQLM_ELM_PRIV_WORKSPACE_NUM_OVERFLOWS
       78 SQLM-ELM-PRIV-WKSPC-NUM-OWFLW VALUE 763.

      * The full name of the monitor element is SQLM_ELM_PRIV_WORKSPACE_SECTION_LOOKUPS
       78 SQLM-ELM-PRIV-WKSPC-SEC-LKP VALUE 764.

      * The full name of the monitor element is SQLM_ELM_PRIV_WORKSPACE_SECTION_INSERTS
       78 SQLM-ELM-PRIV-WKSPC-SEC-INS VALUE 765.


      *****************************************************************************
      * Catalog Cache                                                              *
      *****************************************************************************
       78 SQLM-ELM-CAT-CACHE-SIZE-TOP VALUE 766.


      *****************************************************************************
      * Partition Number for EEE WTT evmon                                         *
      *****************************************************************************
       78 SQLM-ELM-PARTITION-NUMBER VALUE 767.

      * The full name of the monitor element is SQLM_ELM_NUM_TRANSMISSIONS_GROUP
       78 SQLM-ELM-NUM-TRMS-GROUP VALUE 768.

       78 SQLM-ELM-NUM-INDOUBT-TRANS VALUE 769.


      *****************************************************************************
      * Information for active utilities                                           *
      *****************************************************************************
       78 SQLM-ELM-UTILITY-DBNAME VALUE 770.

       78 SQLM-ELM-UTILITY-ID VALUE 771.

       78 SQLM-ELM-UTILITY-TYPE VALUE 772.

       78 SQLM-ELM-UTILITY-PRIORITY VALUE 773.

       78 SQLM-ELM-UTILITY-START-TIME VALUE 774.

       78 SQLM-ELM-UTILITY-DESCRIPTION VALUE 775.


      * The full name of the monitor element is SQLM_ELM_POOL_ASYNC_INDEX_READ_REQS
       78 SQLM-ELM-PL-ASYNC-IDX-RD-REQS VALUE 776.

       78 SQLM-ELM-SESSION-AUTH-ID VALUE 777.


       78 SQLM-ELM-SQL-CHAINS VALUE 778.


      * The full name of the monitor element is SQLM_ELM_POOL_TEMP_DATA_L_READS
       78 SQLM-ELM-PL-TMP-DATA-L-RD VALUE 779.

      * The full name of the monitor element is SQLM_ELM_POOL_TEMP_DATA_P_READS
       78 SQLM-ELM-PL-TMP-DATA-P-RD VALUE 780.

      * The full name of the monitor element is SQLM_ELM_POOL_TEMP_INDEX_L_READS
       78 SQLM-ELM-PL-TMP-IDX-L-RD VALUE 781.

      * The full name of the monitor element is SQLM_ELM_POOL_TEMP_INDEX_P_READS
       78 SQLM-ELM-PL-TMP-IDX-P-RD VALUE 782.


       78 SQLM-ELM-MAX-TIME-1-MS VALUE 783.

       78 SQLM-ELM-MAX-TIME-100-MS VALUE 784.

       78 SQLM-ELM-MAX-TIME-500-MS VALUE 785.

       78 SQLM-ELM-MAX-TIME-GT500-MS VALUE 786.


      * The full name of the monitor element is SQLM_ELM_LOG_TO_REDO_FOR_RECOVERY
       78 SQLM-ELM-LOG-TO-REDO-FOR-RCVRY VALUE 787.

      * redone during recovery       *
       78 SQLM-ELM-POOL-NO-VICTIM-BUFFER VALUE 788.

      * buffer available from hate   *
      * list                         *
      * The full name of the monitor element is SQLM_ELM_LOG_HELD_BY_DIRTY_PAGES
       78 SQLM-ELM-LOG-HELD-BY-DIRTY-PG VALUE 789.

      * by minbuf                    *

      *****************************************************************************
      *   Information for the progress of utilities                                *
      *****************************************************************************
       78 SQLM-ELM-PROGRESS-DESCRIPTION VALUE 790.

       78 SQLM-ELM-PROGRESS-START-TIME VALUE 791.

       78 SQLM-ELM-PROGRESS-WORK-METRIC VALUE 792.

       78 SQLM-ELM-PROGRESS-TOTAL-UNITS VALUE 793.

      * The full name of the monitor element is SQLM_ELM_PROGRESS_COMPLETED_UNITS
       78 SQLM-ELM-PROG-COMP-UNIT VALUE 794.

       78 SQLM-ELM-PROGRESS-SEQ-NUM VALUE 795.

      * The full name of the monitor element is SQLM_ELM_PROGRESS_LIST_CUR_SEQ_NUM
       78 SQLM-PROG-LST-CUR-SEQN VALUE 796.


      *****************************************************************************
      * Information for HADR                                                       *
      *****************************************************************************
       78 SQLM-ELM-HADR-ROLE VALUE 797.

       78 SQLM-ELM-HADR-STATE VALUE 798.

       78 SQLM-ELM-HADR-SYNCMODE VALUE 799.

       78 SQLM-ELM-HADR-CONNECT-STATUS VALUE 800.

       78 SQLM-ELM-HADR-CONNECT-TIME VALUE 801.

       78 SQLM-ELM-HADR-HEARTBEAT VALUE 802.

       78 SQLM-ELM-HADR-LOCAL-HOST VALUE 803.

       78 SQLM-ELM-HADR-LOCAL-SERVICE VALUE 804.

       78 SQLM-ELM-HADR-REMOTE-HOST VALUE 805.

       78 SQLM-ELM-HADR-REMOTE-SERVICE VALUE 806.

       78 SQLM-ELM-HADR-TIMEOUT VALUE 807.

       78 SQLM-ELM-HADR-PRIMARY-LOG-FILE VALUE 808.

       78 SQLM-ELM-HADR-PRIMARY-LOG-PAGE VALUE 809.

       78 SQLM-ELM-HADR-PRIMARY-LOG-LSN VALUE 810.

       78 SQLM-ELM-HADR-STANDBY-LOG-FILE VALUE 811.

       78 SQLM-ELM-HADR-STANDBY-LOG-PAGE VALUE 812.

       78 SQLM-ELM-HADR-STANDBY-LOG-LSN VALUE 813.

       78 SQLM-ELM-HADR-LOG-GAP VALUE 814.

       78 SQLM-ELM-HADR-REMOTE-INSTANCE VALUE 815.


      *****************************************************************************
      * Additional elements added in 8.2                                           *
      *****************************************************************************
       78 SQLM-ELM-DATA-OBJECT-PAGES VALUE 816.

      * base data                   *
       78 SQLM-ELM-INDEX-OBJECT-PAGES VALUE 817.

      * base data                   *
       78 SQLM-ELM-LOB-OBJECT-PAGES VALUE 818.

       78 SQLM-ELM-LONG-OBJECT-PAGES VALUE 819.

      * data                        *

       78 SQLM-ELM-LOCK-TIMEOUT-VAL VALUE 820.


      *****************************************************************************
      *  Log elements v8.2                                                         *
      *****************************************************************************
       78 SQLM-ELM-LOG-WRITE-TIME VALUE 821.

       78 SQLM-ELM-LOG-READ-TIME VALUE 822.

       78 SQLM-ELM-NUM-LOG-WRITE-IO VALUE 823.

       78 SQLM-ELM-NUM-LOG-READ-IO VALUE 824.

       78 SQLM-ELM-NUM-LOG-PART-PAGE-IO VALUE 825.

       78 SQLM-ELM-NUM-LOG-BUFF-FULL VALUE 826.

       78 SQLM-ELM-NUM-LOG-DATA-IN-BUFF VALUE 827.

       78 SQLM-ELM-LOG-FILE-NUM-FIRST VALUE 828.

       78 SQLM-ELM-LOG-FILE-NUM-LAST VALUE 829.

       78 SQLM-ELM-LOG-FILE-NUM-CURR VALUE 830.

       78 SQLM-ELM-LOG-FILE-ARCHIVE VALUE 831.


       78 SQLM-ELM-NANOSEC VALUE 832.


      *****************************************************************************
      *  New elements for enhanced deadlock event monitor with statement history   *
      *****************************************************************************
       78 SQLM-ELM-STMT-HISTORY-ID VALUE 833.

       78 SQLM-ELM-STMT-LOCK-TIMEOUT VALUE 834.

       78 SQLM-ELM-STMT-ISOLATION VALUE 835.

       78 SQLM-ELM-COMP-ENV-DESC VALUE 836.

      * compilation environment     *
       78 SQLM-ELM-STMT-VALUE-TYPE VALUE 837.

       78 SQLM-ELM-STMT-VALUE-ISREOPT VALUE 838.

       78 SQLM-ELM-STMT-VALUE-ISNULL VALUE 839.

       78 SQLM-ELM-STMT-VALUE-DATA VALUE 840.

       78 SQLM-ELM-STMT-VALUE-INDEX VALUE 841.

       78 SQLM-ELM-STMT-FIRST-USE-TIME VALUE 842.

       78 SQLM-ELM-STMT-LAST-USE-TIME VALUE 843.

       78 SQLM-ELM-STMT-NEST-LEVEL VALUE 844.

       78 SQLM-ELM-STMT-INVOCATION-ID VALUE 845.

       78 SQLM-ELM-STMT-QUERY-ID VALUE 846.

       78 SQLM-ELM-STMT-SOURCE-ID VALUE 847.

       78 SQLM-ELM-STMT-PKGCACHE-ID VALUE 848.

       78 SQLM-ELM-INACT-STMTHIST-SZ VALUE 849.


      *****************************************************************************
      * New elements for Single point of storage management                        *
      *****************************************************************************
       78 SQLM-ELM-NUM-DB-STORAGE-PATHS VALUE 850.

       78 SQLM-ELM-DB-STORAGE-PATH VALUE 851.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_INITIAL_SIZE
       78 SQLM-ELM-TBSP-INITIAL-SIZE VALUE 852.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_CURRENT_SIZE
       78 SQLM-ELM-TBSP-CURRENT-SIZE VALUE 853.

       78 SQLM-ELM-TABLESPACE-MAX-SIZE VALUE 854.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_INCREASE_SIZE_PERCENT
       78 SQLM-ELM-TBSP-INCR-SIZE-PCT VALUE 855.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_INCREASE_SIZE
       78 SQLM-ELM-TBSP-INCR-SIZE VALUE 856.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_LAST_RESIZE_TIME
       78 SQLM-ELM-TBSP-LAST-RESIZE-TIME VALUE 857.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_USING_AUTO_STORAGE
       78 SQLM-ELM-TBSP-USE-AUTO-STORAGE VALUE 858.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_AUTO_RESIZE_ENABLED
       78 SQLM-ELM-TBSP-AUTO-RSZ-ENABLED VALUE 859.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_LAST_RESIZE_FAILED
       78 SQLM-ELM-TBSP-LAST-RESIZE-FAIL VALUE 860.


       78 SQLM-ELM-BP-ID VALUE 861.


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
       78 SQLM-ELM-HI-ID VALUE 55001.

       78 SQLM-ELM-HI-VALUE VALUE 55002.

       78 SQLM-ELM-HI-ALERT-STATE VALUE 55003.

      * The full name of the monitor element is SQLM_ELM_DB2_ROLLED_UP_ALERT_STATE
       78 SQLM-ELM-DB2-R0L-UP-ALRT-ST VALUE 55004.

      * The full name of the monitor element is SQLM_ELM_DATABASE_ROLLED_UP_ALERT_STATE
       78 SQLM-ELM-DB-R0L-UP-ALRT-ST VALUE 55005.

      * The full name of the monitor element is SQLM_ELM_TABLESPACE_ROLLED_UP_ALERT_STATE
       78 SQLM-ELM-TBSP-R0L-UP-ALRT-ST VALUE 55006.

      * The full name of the monitor element is SQLM_ELM_CONTAINER_ROLLED_UP_ALERT_STATE
       78 SQLM-ELM-CONT-R0L-UP-ALRT-ST VALUE 55007.

       78 SQLM-ELM-HI-OBJ-NAME VALUE 55008.

       78 SQLM-ELM-HI-OBJ-DETAIL VALUE 55009.

       78 SQLM-ELM-HI-OBJ-STATE VALUE 55010.


      * the following are for health snapshot with detail only                     *
       78 SQLM-ELM-HI-FORMULA VALUE 55101.

       78 SQLM-ELM-HI-ADDITIONAL-INFO VALUE 55102.


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
       78 SQLM-HI-DB2-STATE VALUE 1.

      * The full name of the monitor element is SQLM_HI_DB2_PRIVATE_SORT_MEM_UTILIZATION
       78 SQLM-HI-DB2-PRV-SRT-M-UTL VALUE 2.

      * The full name of the monitor element is SQLM_HI_DB2_MAX_PRIVATE_SORT_MEM_UTILIZATION
       78 SQLM-HI-DB2-MAX-PRV-SRT-M-UTL VALUE 3.

      * The full name of the monitor element is SQLM_HI_DB2_MON_HEAP_UTILIZATION
       78 SQLM-HI-DB2-MON-HEAP-UTL VALUE 4.


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
       78 SQLM-HI-DATABASE-STATE VALUE 1001.

      * The full name of the monitor element is SQLM_HI_DATABASE_SHARED_SORT_MEM_UTILIZATION
       78 SQLM-HI-DB-SHR-SRT-M-UTL VALUE 1002.

       78 SQLM-HI-DATABASE-SPILLED-SORTS VALUE 1003.

      * The full name of the monitor element is SQLM_HI_DATABASE_MAX_SHARED_SORT_MEM_UTILIZATION
       78 SQLM-HI-DB-MAX-SHR-SRT-M-UTL VALUE 1004.

      * The full name of the monitor element is SQLM_HI_DATABASE_LOG_UTILIZATION
       78 SQLM-HI-DB-LOG-UTL VALUE 1005.

      * The full name of the monitor element is SQLM_HI_DATABASE_LOG_FILESYSTEM_UTILIZATION
       78 SQLM-HI-DB-LOG-FILESYSTEM-UTL VALUE 1006.

       78 SQLM-HI-DATABASE-DEADLOCK-RATE VALUE 1007.

      * The full name of the monitor element is SQLM_HI_DATABASE_LOCKLIST_UTILIZATION
       78 SQLM-HI-DB-LOCKLIST-UTL VALUE 1008.

      * The full name of the monitor element is SQLM_HI_DATABASE_LOCK_ESCALATION_RATE
       78 SQLM-HI-DB-LOCK-ESCL-RATE VALUE 1009.

      * The full name of the monitor element is SQLM_HI_DATABASE_APPS_WAITING_ON_LOCKS
       78 SQLM-HI-DB-APPS-WAIT-ON-LOCKS VALUE 1010.

      * The full name of the monitor element is SQLM_HI_DATABASE_PKGCACHE_HITRATIO
       78 SQLM-HI-DB-PKGCACHE-HITRATIO VALUE 1011.

      * The full name of the monitor element is SQLM_HI_DATABASE_CATCACHE_HITRATIO
       78 SQLM-HI-DB-CATCACHE-HITRATIO VALUE 1012.

      * The full name of the monitor element is SQLM_HI_DATABASE_SHRWORKSPACE_HITRATIO
       78 SQLM-HI-DB-SHRWKSPC-HITRATIO VALUE 1013.

      * The full name of the monitor element is SQLM_HI_DATABASE_HEAP_UTILIZATION
       78 SQLM-HI-DB-HEAP-UTL VALUE 1014.

       78 SQLM-HI-DATABASE-TB-REORG-REQ VALUE 1015.

      * The full name of the monitor element is SQLM_HI_DATABASE_HADR_OP_STATUS
       78 SQLM-HI-DATABASE-HADR-OPSTATUS VALUE 1016.

       78 SQLM-HI-DATABASE-HADR-DELAY VALUE 1017.

       78 SQLM-HI-DATABASE-BACKUP-REQ VALUE 1018.

      * The full name of the monitor element is SQLM_HI_DATABASE_FED_NICKNAME_STATUS
       78 SQLM-HI-DB-FED-NICKNAME-STATUS VALUE 1019.

      * The full name of the monitor element is SQLM_HI_DATABASE_FED_SERVER_STATUS
       78 SQLM-HI-DB-FED-SERVER-STATUS VALUE 1020.

       78 SQLM-HI-DATABASE-TB-STATS-REQ VALUE 1022.

      * The full name of the monitor element is SQLM_HI_DATABASE_AUTO_STORAGE_UTILIZATION
       78 SQLM-HI-DB-AUTO-STG-UTIL VALUE 1023.


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
       78 SQLM-HI-TABLESPACE-STATE VALUE 2001.

       78 SQLM-HI-TABLESPACE-UTILIZATION VALUE 2002.

      * The full name of the monitor element is SQLM_HI_TABLESPACE_AUTO_RESIZE_STATUS
       78 SQLM-HI-TBSP-AUTO-RESIZE-ST VALUE 2003.

      * The full name of the monitor element is SQLM_HI_AUTO_RESIZE_TABLESPACE_UTILIZATION
       78 SQLM-HI-AUTO-RS-TBSP-UTIL VALUE 2004.


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
       78 SQLM-HI-TBSP-CONT-ST VALUE 3001.

      * The full name of the monitor element is SQLM_HI_TABLESPACE_CONTAINER_UTILIZATION
       78 SQLM-HI-TBSP-CONT-UTL VALUE 3002.


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

       78 SQLM-HI-ALERT-STATE-UNKNOWN VALUE 0.

       78 SQLM-HI-ALERT-STATE-NORMAL VALUE 1.

       78 SQLM-HI-ALERT-STATE-ATTENTION VALUE 2.

       78 SQLM-HI-ALERT-STATE-WARNING VALUE 3.

       78 SQLM-HI-ALERT-STATE-ALARM VALUE 4.

       78 SQLM-HI-ALERT-STATE-AUTOMATED VALUE 5.

      * The full name of the monitor element is SQLM_HI_ALERT_STATE_AUTOMATE_FAILED
       78 SQLM-HIALRT-STATE-AUTO-FAIL VALUE 6.


      *****************************************************************************
      * Health Indicator specific values                                           *
      *****************************************************************************

      * these are possible values for SQLM_HI_DATABASE_BACKUP_REQ                  *
       78 SQLM-HI-VAL-NO-DB-BACKUP-REQ VALUE 0.

       78 SQLM-HI-VAL-DB-BACKUP-REQ VALUE 1.

      * The full name of the monitor element is SQLM_HI_VAL_AUTO_DB_BACKUP_FAILED
       78 SQLM-HIVAL-AUTO-DBBCKP-FAIL VALUE 2.


      * these are the possible values for SQLM_HI_DATABASE_TB_REORG_REQ            *
       78 SQLM-HI-VAL-NO-REORG-REQ VALUE 0.

       78 SQLM-HI-VAL-REORG-REQ VALUE 1.


      * these are the possible values for SQLM_HI_DATABASE_TB_STATS_REQ            *
       78 SQLM-HI-VAL-NO-STATS-REQ VALUE 0.

       78 SQLM-HI-VAL-STATS-REQ VALUE 1.


      * nickname status (fed_nickname_status)                                      *
       78 SQLM-FED-NN-VALID VALUE 0.

       78 SQLM-FED-NN-INVALID VALUE 1.


      * federated server status (fed_server_status)                                *
       78 SQLM-FED-SERVER-AVAILABLE VALUE 0.

       78 SQLM-FED-SERVER-UNAVAILABLE VALUE 1.


      *****************************************************************************
      * Health snapshot option flags. These flags can be used in the agent_id      *
      * field of the sqlm_obj_struct passed in the sqlma. The flag is used in      *
      * conjunction with the snapshot classes SQLM_CLASS_HEALTH and                *
      * SQLM_CLASS_HEALTH_WITH_DETAIL  to determine what additional health snapshot*
      * information to return in the snapshot stream                               *
      *****************************************************************************

       78 SQLM-HMON-OPT-COLL-FULL VALUE   00000001 .


      *****************************************************************************
      * NOTE: 60000 - 65535 reserved for internal IBM use                          *
      *****************************************************************************

      * End Of Monitor Definitions *

      **********************************************************************
      * Database Monitor Structure Size Constants
      * DON'T USE THESE CONSTANTS. THEY ARE PROVIDED FOR COMPATIBILITY ONLY
      **********************************************************************

       78 SQLM-DB2-SZ                VALUE 100.
      *structure SQLM-DB2

       78 SQLM-DBASE-SZ              VALUE 548.
      *structure SQLM-DBASE

       78 SQLM-APPL-SZ               VALUE 880.
      *structure SQLM-APPL

       78 SQLM-APPLINFO-SZ           VALUE 492.
      *structure SQLM-APPLINFO

       78 SQLM-DCS-APPLINFO-SZ       VALUE 260.
      *structure SQLM-DCS-APPLINFO

       78 SQLM-TABLE-HEADER-SZ       VALUE 356.
      *structure SQLM-TABLE-HEADER

       78 SQLM-TABLE-SZ              VALUE 60.
      *structure SQLM-TABLE

       78 SQLM-DBASE-LOCK-SZ         VALUE 352.
      *structure SQLM-DBASE-LOCK

       78 SQLM-APPL-LOCK-SZ          VALUE 216.
      *structure SQLM-APPL-LOCK

       78 SQLM-LOCK-SZ               VALUE 60.
      *structure SQLM-LOCK

       78 SQLM-TIMESTAMP-SZ          VALUE 8.
      *structure SQLM-TIMESTAMP

       78 SQLM-COLLECTED-SZ          VALUE 232.
      *structure SQLM-COLLECTED

      **********************************************************************
      * Database Monitor Data Structures
      **********************************************************************

      **********************************************************************
      *  Dynamic (V6) Monitor Stream structures
      **********************************************************************
       01 SQLM-HEADER-INFO.
          05 SQLM-SIZE              PIC 9(9) COMP-5.
          05 SQLM-TYPE                   PIC 9(4) COMP-5.
          05 ELEMENT                PIC 9(4) COMP-5.
        
       01 SQLM-HEADER-DATA.
          05 HEADER.
             10 SQLM-SIZE           PIC 9(9) COMP-5.
             10 SQLM-TYPE           PIC 9(4) COMP-5.
             10 ELEMENT             PIC 9(4) COMP-5.
          05 SQLM-DATA              PIC X(1).

      **********************************************************************
      *  SQL Monitor Area
      **********************************************************************

       01 SQLMA.
          05 OBJ-NUM                 PIC 9(9) COMP-5.
          05 OBJ-VAR OCCURS 0 TO 100 TIMES DEPENDING ON OBJ-NUM.
             10 AGENT-ID             PIC 9(9) COMP-5.
             10 OBJ-TYPE             PIC 9(9) COMP-5.
             10 SQLMA-OBJECT         PIC X(36).

      **********************************************************************
      * time stamp data structure
      **********************************************************************

       01 SQLM-TIMESTAMP.
          05 SECONDS                 PIC 9(9) COMP-5.
          05 MICROSEC                PIC 9(9) COMP-5.

      **********************************************************************
      * Elpased time data structure  (For CPU times)
      **********************************************************************

       01 SQLM-TIME.
          05 SECONDS                 PIC 9(9) COMP-5.
          05 MICROSEC                PIC 9(9) COMP-5.

      **********************************************************************
      * State information for each monitor group
      **********************************************************************

       01 SQLM-RECORDING-GROUP OCCURS 7 TIMES.
          05 INPUT-STATE             PIC 9(9) COMP-5.
          05 OUTPUT-STATE            PIC 9(9) COMP-5.
          05 START-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.

      **********************************************************************
      * collection count data structure returned from get monitor info
      **********************************************************************

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
          05 SERVER-PRDID            PIC X(20).
          05 SERVER-NNAME            PIC X(20).
          05 SERVER-INSTANCE-NAME    PIC X(20).
          05 RESERVED                PIC X(22).
          05 NODE-NUMBER             PIC 9(5) COMP-5.
          05 TIME-ZONE-DISP          PIC S9(9) COMP-5.
          05 NUM-TOP-LEVEL-STRUCTS   PIC 9(9) COMP-5.
          05 TABLESPACE-DATABASES    PIC 9(9) COMP-5.
          05 SERVER-VERSION          PIC 9(9) COMP-5.
