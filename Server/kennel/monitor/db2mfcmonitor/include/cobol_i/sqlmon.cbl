      ***********************************************************************
      *
      *  Source File Name = SQLMON.CBL
      *
      *  (C) Copyright IBM Corp. 1993, 1997
      *  All Rights Reserved
      *  Licensed Material - Program Property of IBM
      *
      *  US Government Users Restricted Rights - Use, duplication or
      *  disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
      *
      *  Function = Copy File defining:
      *               DB Monitor Data Structures
      *
      **********************************************************************

      **********************************************************************
      * Database Monitor Snapshot Data Structures
      **********************************************************************

      **********************************************************************
      * Summary Application Information Snapshot data structure
      **********************************************************************

       01 SQLM-APPLINFO.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X(3) USAGE DISPLAY NATIVE.
          05 AGENT-ID                PIC 9(9) COMP-5.
          05 APPL-STATUS             PIC 9(9) COMP-5.
          05 CODEPAGE-ID             PIC 9(9) COMP-5.
          05 STATUS-CHANGE-TIME.
               10 SECONDS              PIC 9(9) COMP-5.
               10 MICROSEC             PIC 9(9) COMP-5.
          05 APPL-NAME               PIC X(20) USAGE DISPLAY NATIVE.
          05 APPL-ID                 PIC X(32) USAGE DISPLAY NATIVE.
          05 SEQUENCE-NO             PIC X(4) USAGE DISPLAY NATIVE.
          05 AUTH-ID                 PIC X(20) USAGE DISPLAY NATIVE.
          05 CLIENT-NNAME            PIC X(20) USAGE DISPLAY NATIVE.
          05 CLIENT-PRDID            PIC X(20) USAGE DISPLAY NATIVE.
          05 INPUT-DB-ALIAS          PIC X(20) USAGE DISPLAY NATIVE.
          05 CLIENT-DB-ALIAS         PIC X(20) USAGE DISPLAY NATIVE.
          05 DB-NAME                 PIC X(20) USAGE DISPLAY NATIVE.
          05 DB-PATH                 PIC X(256) USAGE DISPLAY NATIVE.
          05 RESERVED                PIC X(20) USAGE DISPLAY NATIVE.
          05 NUM-ASSOC-AGENTS        PIC 9(9) COMP-5.
          05 COORD-NODE-NUM          PIC 9(9) COMP-5.
          05 AUTHORITY-LVL           PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2  Client Identification Information
          05 EXECUTION-ID            PIC X(20) USAGE DISPLAY NATIVE.
          05 CORR-TOKEN              PIC X(32) USAGE DISPLAY NATIVE.
          05 CLIENT-PID              PIC 9(9) COMP-5.
          05 CLIENT-PLATFORM         PIC 9(4) COMP-5.
          05 CLIENT-PROTOCOL         PIC 9(4) COMP-5.
          05 COUNTRY-CODE            PIC 9(4) COMP-5.
          05 FILLER                  PIC X(2) USAGE DISPLAY NATIVE.
          05 COORD-AGENT-PID         PIC S9(9) COMP-5.

      **********************************************************************
      * DDCS Application Information Snapshot data structure
      **********************************************************************

       01 SQLM-DCS-APPLINFO.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X(3) USAGE DISPLAY NATIVE.
          05 AGENT-ID                PIC 9(9) COMP-5.
          05 CODEPAGE-ID             PIC 9(9) COMP-5.
          05 APPL-NAME               PIC X(20) USAGE DISPLAY NATIVE.
          05 APPL-ID                 PIC X(32) USAGE DISPLAY NATIVE.
          05 SEQUENCE-NO             PIC X(4) USAGE DISPLAY NATIVE.
          05 AUTH-ID                 PIC X(20) USAGE DISPLAY NATIVE.
          05 CLIENT-NNAME            PIC X(20) USAGE DISPLAY NATIVE.
          05 CLIENT-PRDID            PIC X(20) USAGE DISPLAY NATIVE.
          05 CLIENT-DB-ALIAS         PIC X(20) USAGE DISPLAY NATIVE.
          05 DB-NAME                 PIC X(20) USAGE DISPLAY NATIVE.
          05 HOST-PRDID              PIC X(20) USAGE DISPLAY NATIVE.
          05 OUTBOUND-APPL-ID        PIC X(32) USAGE DISPLAY NATIVE.
          05 OUTBOUND-SEQUENCE-NO    PIC X(4) USAGE DISPLAY NATIVE.
          05 DCS-APPL-STATUS         PIC 9(9) COMP-5.
          05 STATUS-CHANGE-TIME.
            10 SECONDS               PIC 9(9) COMP-5.
            10 MICROSEC              PIC 9(9) COMP-5.
          05 CLIENT-PLATFORM         PIC 9(4) COMP-5. 
          05 CLIENT-PROTOCOL         PIC 9(4) COMP-5.
          05 HOST-CCSID              PIC 9(4) COMP-5.
          05 NUM-SEC-DCS-APP-STRUCTS PIC 9(9) COMP-5.
          05 RESERVED                PIC X(10) USAGE DISPLAY NATIVE.
      
      **********************************************************************
      * DDCS Application ID Information Snapshot data structure
      **********************************************************************
      
       01 SQLM-DCS-APPLID-INFO.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER-A                PIC X(3) USAGE DISPLAY NATIVE.
          05 AGENT-ID                PIC 9(9) COMP-5.
          05 CODEPAGE-ID             PIC 9(9) COMP-5.
          05 CLIENT-PID              PIC 9(9) COMP-5.
          05 APPL-NAME               PIC X(20) USAGE DISPLAY NATIVE.
          05 APPL-ID                 PIC X(32) USAGE DISPLAY NATIVE.
          05 SEQUENCE-NO             PIC X(4) USAGE DISPLAY NATIVE.
          05 AUTH-ID                 PIC X(20) USAGE DISPLAY NATIVE.
          05 CLIENT-NNAME            PIC X(20) USAGE DISPLAY NATIVE.
          05 CLIENT-PRDID            PIC X(20) USAGE DISPLAY NATIVE.
          05 GW-DB-ALIAS             PIC X(20) USAGE DISPLAY NATIVE.
          05 EXECUTION-ID            PIC X(20) USAGE DISPLAY NATIVE.
          05 OUTBOUND-COMM-ADDRESS   PIC X(32) USAGE DISPLAY NATIVE.
          05 INBOUND-COMM-ADDRESS    PIC X(32) USAGE DISPLAY NATIVE.
          05 OUTBOUND-COMM-PROTOCOL  PIC 9(4) COMP-5.
          05 CLIENT-PLATFORM         PIC 9(4) COMP-5.
          05 CLIENT-PROTOCOL         PIC 9(4) COMP-5.
          05 FILLER-B                PIC X(2) USAGE DISPLAY NATIVE.
          05 DCS-DB-NAME             PIC X(20) USAGE DISPLAY NATIVE.
          05 HOST-DB-NAME            PIC X(20) USAGE DISPLAY NATIVE.
          05 HOST-PRDID              PIC X(20) USAGE DISPLAY NATIVE.
          05 OUTBOUND-APPL-ID        PIC X(32) USAGE DISPLAY NATIVE.
          05 OUTBOUND-SEQUENCE-NO    PIC X(4) USAGE DISPLAY NATIVE.
          05 DCS-APPL-STATUS         PIC 9(9) COMP-5.
          05 STATUS-CHANGE-TIME.
              10 SECONDS             PIC 9(9) COMP-5.
              10 MICROSEC            PIC 9(9) COMP-5.
          05 HOST-CCSID              PIC 9(4) COMP-5.
          05 FILLER-C                PIC X(2) USAGE DISPLAY NATIVE.
      
      **********************************************************************
      * DDCS Application Statistics Snapshot data structure
      **********************************************************************

       01 SQLM-DCS-APPL-SNAP-STATS.
          05 SQLM-SIZE                    PIC 9(9) COMP-5.
          05 INFO-TYPE                    PIC X USAGE DISPLAY NATIVE.
          05 FILLER                       PIC X(3) USAGE DISPLAY NATIVE.
          05 SQLM-DCS-APPL-STATS.
              10 GW-CON-TIME.
                  15 SECONDS              PIC 9(9) COMP-5.
                  15 MICROSEC             PIC 9(9) COMP-5.
              10 ROWS-SELECTED            PIC 9(9) COMP-5.
              10 SQL-STMTS                PIC 9(9) COMP-5.
              10 FAILED-SQL-STMTS         PIC 9(9) COMP-5.
              10 COMMIT-SQL-STMTS         PIC 9(9) COMP-5.
              10 ROLLBACK-SQL-STMTS       PIC 9(9) COMP-5.
              10 INBOUND-BYTES-RECEIVED   PIC 9(9) COMP-5.
              10 OUTBOUND-BYTES-SENT      PIC 9(9) COMP-5.
              10 OUTBOUND-BYTES-RECEIVED  PIC 9(9) COMP-5.
              10 INBOUND-BYTES-SENT       PIC 9(9) COMP-5.
               
      **********************************************************************
      * DDCS Application Snapshot data structure
      **********************************************************************

       01 SQLM-DCS-APPL.
          05 SQLM-SIZE                   PIC 9(9) COMP-5.
          05 INFO-TYPE                   PIC X USAGE DISPLAY NATIVE.
          05 FILLER                      PIC X(3) USAGE DISPLAY NATIVE.
          05 NUM-SEC-STRUCTS             PIC 9(9) COMP-5.
          05 STMT-OPERATION              PIC 9(9) COMP-5.
          05 SECTION-NUMBER              PIC 9(9) COMP-5.
          05 STMT-START.
              10 SECONDS                 PIC 9(9) COMP-5.
              10 MICROSEC                PIC 9(9) COMP-5.
          05 STMT-STOP. 
              10 SECONDS                 PIC 9(9) COMP-5.
              10 MICROSEC                PIC 9(9) COMP-5.
          05 CREATOR                     PIC X(20) USAGE DISPLAY NATIVE.
          05 PACKAGE-NAME                PIC X(20) USAGE DISPLAY NATIVE.
          05 APPL-IDLE-TIME              PIC 9(9) COMP-5.
          05 OPEN-CURSORS                PIC 9(9) COMP-5.
          05 UOW-COMP-STATUS             PIC 9(9) COMP-5.
          05 PREV-UOW-STOP-TIME.
              10 SECONDS                 PIC 9(9) COMP-5.
              10 MICROSEC                PIC 9(9) COMP-5.
          05 UOW-START-TIME.
              10 SECONDS                 PIC 9(9) COMP-5.
              10 MICROSEC                PIC 9(9) COMP-5.
          05 UOW-STOP-TIME.
              10 SECONDS                 PIC 9(9) COMP-5.
              10 MICROSEC                PIC 9(9) COMP-5.
          05 INBOUND-BYTES-RECEIVED      PIC 9(9) COMP-5.
          05 OUTBOUND-BYTES-SENT         PIC 9(9) COMP-5.
          05 OUTBOUND-BYTES-RECEIVED     PIC 9(9) COMP-5.
          05 INBOUND-BYTES-SENT          PIC 9(9) COMP-5.
          05 LAST-RESET.
              10 SECONDS                 PIC 9(9) COMP-5.
              10 MICROSEC                PIC 9(9) COMP-5.
                     
      **********************************************************************
      * XID Information - returned on application and DCS appl snapshot
      **********************************************************************
       
       01 SQLM-APPL-XID.
          05 SQLM-SIZE                   PIC 9(9) COMP-5.          
          05 INFO-TYPE                   PIC X USAGE DISPLAY NATIVE.
          05 FILLER                      PIC X(3) USAGE DISPLAY NATIVE.
          05 XID-SIZE                    PIC 9(9) COMP-5.
          05 XID                         PIC X USAGE DISPLAY NATIVE.
      
      **********************************************************************
      * 3-tier client information 
      **********************************************************************
      
       01 SQLM-TPMON-INFO.
          05 SQLM-SIZE                   PIC 9(9) COMP-5.
          05 INFO-TYPE                   PIC X USAGE DISPLAY NATIVE.
          05 FILLER                      PIC X(3) USAGE DISPLAY NATIVE.
          05 TPMON-CLIENT-USERID         PIC X(20) USAGE DISPLAY NATIVE.
          05 TPMON-CLIENT-WKSTN          PIC X(20) USAGE DISPLAY NATIVE.
          05 TPMON-CLIENT-APP            PIC X(20) USAGE DISPLAY NATIVE.
          05 ACC-STR-LENGTH              PIC 9(4) COMP-5.
          05 ACC-STR-OFFSET              PIC 9(4) COMP-5.
      
      **********************************************************************
      * Statement that is running on a host 
      **********************************************************************
      
       01 SQLM-DCS-STMT.
          05 SQLM-SIZE                   PIC 9(9) COMP-5.
          05 INFO-TYPE                   PIC X USAGE DISPLAY NATIVE.
          05 FILLER                      PIC X(3) USAGE DISPLAY NATIVE.
          05 CREATOR                     PIC X(20) USAGE DISPLAY NATIVE.
          05 PACKAGE-NAME                PIC X(20) USAGE DISPLAY NATIVE.
          05 SECTION-NUMBER              PIC 9(9) COMP-5.
          05 QUERY-COST-ESTIMATE         PIC 9(9) COMP-5.
          05 QUERY-CARD-ESTIMATE         PIC 9(9) COMP-5.
          05 STMT-START.    
             10 SECONDS                  PIC 9(9) COMP-5.
             10 MICROSEC                 PIC 9(9) COMP-5.
          05 STMT-STOP.
             10 SECONDS                  PIC 9(9) COMP-5.
             10 MICROSEC                 PIC 9(9) COMP-5.
          05 STMT-OPERATION              PIC 9(9) COMP-5.
          05 FETCH-COUNT                 PIC 9(9) COMP-5.
          05 GW-EXEC-TIME.
             10 SECONDS                  PIC 9(9) COMP-5.
             10 MICROSEC                 PIC 9(9) COMP-5.
          05 INBOUND-BYTES-RECEIVED      PIC 9(9) COMP-5.
          05 OUTBOUND-BYTES-SENT         PIC 9(9) COMP-5.
          05 OUTBOUND-BYTES-RECEIVED     PIC 9(9) COMP-5.
          05 INBOUND-BYTES-SENT          PIC 9(9) COMP-5.
          05 STMT-LENGTH                 PIC 9(4) COMP-5.
          05 STMT-TEXT-OFFSET            PIC 9(4) COMP-5.
      
      **********************************************************************
      * DCS database statistics 
      **********************************************************************
       01 SQLM-DCS-DB.
          05 SQLM-SIZE                   PIC 9(9) COMP-5.
          05 INFO-TYPE                   PIC X USAGE DISPLAY NATIVE.
          05 FILLER                      PIC X(3) USAGE DISPLAY NATIVE.
          05 DCS-DB-NAME                 PIC X(20) USAGE DISPLAY NATIVE.
          05 HOST-DB-NAME                PIC X(20) USAGE DISPLAY NATIVE.
          05 GW-CON-TIME.
             10 SECONDS                  PIC 9(9) COMP-5.
             10 MICROSEC                 PIC 9(9) COMP-5.
          05 LAST-RESET.
             10 SECONDS                  PIC 9(9) COMP-5.
             10 MICROSEC                 PIC 9(9) COMP-5.
          05 SQL-STMTS                   PIC 9(9) COMP-5.
          05 FAILED-SQL-STMTS            PIC 9(9) COMP-5.
          05 COMMIT-SQL-STMTS            PIC 9(9) COMP-5.
          05 ROLLBACK-SQL-STMTS          PIC 9(9) COMP-5.
          05 ROWS-SELECTED               PIC 9(9) COMP-5.
          05 GW-TOTAL-CONS               PIC 9(9) COMP-5.
          05 GW-CUR-CONS                 PIC 9(9) COMP-5.
          05 GW-CONS-WAIT-HOST           PIC 9(9) COMP-5.
          05 GW-CONS-WAIT-CLIENT         PIC 9(9) COMP-5.
          05 GW-CONNECTIONS-TOP          PIC 9(9) COMP-5.
      
      **********************************************************************
      * DB2 Related Snapshot Information
      **********************************************************************
       
       01 SQLM-DB2.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X(3) USAGE DISPLAY NATIVE.

      *   Sorting information
          05 SORT-HEAP-ALLOCATED     PIC 9(9) COMP-5.
          05 POST-THRESHOLD-SORTS    PIC 9(9) COMP-5.
          05 PIPED-SORTS-REQUESTED   PIC 9(9) COMP-5.
          05 PIPED-SORTS-ACCEPTED    PIC 9(9) COMP-5.

      *   Timestamps
          05 DB2START-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 LAST-RESET.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.

      *   Connection information
          05 REM-CONS-IN             PIC 9(9) COMP-5.
          05 REM-CONS-IN-EXEC        PIC 9(9) COMP-5.
          05 LOCAL-CONS              PIC 9(9) COMP-5.
          05 LOCAL-CONS-IN-EXEC      PIC 9(9) COMP-5.
          05 CON-LOCAL-DBASES        PIC 9(9) COMP-5.
          05 AGENTS-REGISTERED       PIC 9(9) COMP-5.
          05 AGENTS-WAITING-ON-TOKEN PIC 9(9) COMP-5.

      *   Server Switch settings - indicate if the DBMS is
      *   collecting monitor data
          05 DBMON-SW-SETTINGS       PIC X OCCURS 6 TIMES
                                     USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X(2) USAGE DISPLAY NATIVE.
          05 DBMON-SW-SET-TIME       PIC 9(9) COMP-5
                                     OCCURS 6 TIMES.

      *   SQLM-DBMON-VERSION2  DB2 Instance Status
          05 DB2-STATUS              PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2  High Water Marks
          05 AGENTS-REGISTERED-TOP   PIC 9(9) COMP-5.
          05 AGENTS-WAITING-TOP      PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2  DB2 Instance Memory Usage Information
          05 COMM-PRIVATE-MEM        PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2  Agent Information
          05 IDLE-AGENTS             PIC 9(9) COMP-5.

      * SQLM_DBMON_VERSION5  Agent Information
          05 AGENTS-FROM-POOL        PIC 9(9) COMP-5.
          05 AGENTS-CREATED-EMPTY-POOL PIC 9(9) COMP-5.
          05 COORD-AGENTS-TOP        PIC 9(9) COMP-5.
          05 MAX-AGENT-OVERFLOWS     PIC 9(9) COMP-5.
          05 AGENTS-STOLEN           PIC 9(9) COMP-5.
          05 NUM-SEC-DBM-STRUCTS     PIC 9(9) COMP-5.
          05 PRODUCT-NAME            PIC X(32) USAGE DISPLAY NATIVE.
          05 COMPONENT-ID            PIC X(20) USAGE DISPLAY NATIVE.
          05 SERVICE-LEVEL           PIC X(8) USAGE DISPLAY NATIVE.

      * SQLM_DBMON_VERSION5_2 DDCS Info
          05 GW-TOTAL-CONS                PIC 9(9) COMP-5.
          05 GW-CUR-CONS                  PIC 9(9) COMP-5.
          05 GW-CONS-WAIT-HOST            PIC 9(9) COMP-5.
          05 GW-CONS-WAIT-CLIENT          PIC 9(9) COMP-5.
          05 GW-POST-THRESHOLD-HASH-JOINS PIC 9(9) COMP-5.

      ***************************************************************
      * Fast Communication Manager (FCM) related Snapshot Information
      ***************************************************************
        01 SQLM-FCM.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X(3) USAGE DISPLAY NATIVE.
          05 BUFF-FREE               PIC 9(9) COMP-5.
          05 BUFF-FREE-BOTTOM        PIC 9(9) COMP-5.
          05 MA-FREE                 PIC 9(9) COMP-5.
          05 MA-FREE-BOTTOM          PIC 9(9) COMP-5.
          05 CE-FREE                 PIC 9(9) COMP-5.
          05 CE-FREE-BOTTOM          PIC 9(9) COMP-5.
          05 RB-FREE                 PIC 9(9) COMP-5.
          05 RB-FREE-BOTTOM          PIC 9(9) COMP-5.
          05 NUMBER-NODES            PIC 9(9) COMP-5.

      *****************************************************************
      * FCM Node Snapshot Information
      *****************************************************************
        01 SQLM-FCM-NODE.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X USAGE DISPLAY NATIVE.
          05 NODE-NUMBER             PIC 9(4) COMP-5.
          05 CONNECTION-STATUS       PIC 9(9) COMP-5.
          05 TOTAL-BUFFERS-SENT      PIC 9(9) COMP-5.
          05 TOTAL-BUFFERS-RCVD      PIC 9(9) COMP-5.

      **********************************************************************
      * Database Related Snapshot Information
      **********************************************************************

       01 SQLM-DBASE.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X(3) USAGE DISPLAY NATIVE.
          05 INPUT-DB-ALIAS          PIC X(20) USAGE DISPLAY NATIVE.
          05 DB-NAME                 PIC X(20) USAGE DISPLAY NATIVE.
          05 DB-PATH                 PIC X(256) USAGE DISPLAY NATIVE.

      *   Locking information
          05 LOCKS-HELD              PIC 9(9) COMP-5.
          05 LOCK-WAITS              PIC 9(9) COMP-5.
          05 LOCK-WAIT-TIME          PIC 9(9) COMP-5.
          05 LOCK-LIST-IN-USE        PIC 9(9) COMP-5.
          05 DEADLOCKS               PIC 9(9) COMP-5.
          05 LOCK-ESCALS             PIC 9(9) COMP-5.
          05 X-LOCK-ESCALS           PIC 9(9) COMP-5.
          05 LOCKS-WAITING           PIC 9(9) COMP-5.

      *   Sorting information
          05 SORT-HEAP-ALLOCATED     PIC 9(9) COMP-5.
          05 TOTAL-SORTS             PIC 9(9) COMP-5.
          05 TOTAL-SORT-TIME         PIC 9(9) COMP-5.
          05 SORT-OVERFLOWS          PIC 9(9) COMP-5.
          05 ACTIVE-SORTS            PIC 9(9) COMP-5.

      *   Buffer Pool information
          05 POOL-DATA-L-READS       PIC 9(9) COMP-5.
          05 POOL-DATA-P-READS       PIC 9(9) COMP-5.
          05 POOL-DATA-WRITES        PIC 9(9) COMP-5.
          05 POOL-INDEX-L-READS      PIC 9(9) COMP-5.
          05 POOL-INDEX-P-READS      PIC 9(9) COMP-5.
          05 POOL-INDEX-WRITES       PIC 9(9) COMP-5.
          05 POOL-READ-TIME          PIC 9(9) COMP-5.
          05 POOL-WRITE-TIME         PIC 9(9) COMP-5.
          05 FILES-CLOSED            PIC 9(9) COMP-5.

      *   SQL Statement counts
          05 COMMIT-SQL-STMTS        PIC 9(9) COMP-5.
          05 ROLLBACK-SQL-STMTS      PIC 9(9) COMP-5.
          05 DYNAMIC-SQL-STMTS       PIC 9(9) COMP-5.
          05 STATIC-SQL-STMTS        PIC 9(9) COMP-5.
          05 FAILED-SQL-STMTS        PIC 9(9) COMP-5.
          05 SELECT-SQL-STMTS        PIC 9(9) COMP-5.
          05 DDL-SQL-STMTS           PIC 9(9) COMP-5.
          05 UID-SQL-STMTS           PIC 9(9) COMP-5.

      *   Internal counts
          05 INT-AUTO-REBINDS        PIC 9(9) COMP-5.
          05 INT-ROWS-DELETED        PIC 9(9) COMP-5.
          05 INT-ROWS-UPDATED        PIC 9(9) COMP-5.
          05 INT-COMMITS             PIC 9(9) COMP-5.
          05 INT-ROLLBACKS           PIC 9(9) COMP-5.
          05 INT-DEADLOCK-ROLLBACKS  PIC 9(9) COMP-5.

      *   Row counts
          05 ROWS-DELETED            PIC 9(9) COMP-5.
          05 ROWS-INSERTED           PIC 9(9) COMP-5.
          05 ROWS-UPDATED            PIC 9(9) COMP-5.
          05 ROWS-SELECTED           PIC 9(9) COMP-5.

      *   Binds/precompiles
          05 BINDS-PRECOMPILES       PIC 9(9) COMP-5.

      *   Timestamps
          05 DB-CONN-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 LAST-RESET.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 LAST-BACKUP.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.

      *   Application connection counters
          05 TOTAL-CONS              PIC 9(9) COMP-5.
          05 APPLS-CUR-CONS          PIC 9(9) COMP-5.
          05 APPLS-IN-DB2            PIC 9(9) COMP-5.

      *   Logging information
          05 SEC-LOG-USED-TOP        PIC 9(9) COMP-5.
          05 TOT-LOG-USED-TOP        PIC 9(9) COMP-5.
          05 SEC-LOGS-ALLOCATED      PIC 9(9) COMP-5.

          05 RESERVED                PIC X(8) USAGE DISPLAY NATIVE.

          05 NUM-SEC-DBASE-STRUCTS   PIC 9(9) COMP-5.

      * SQLM_DBMON_VERSION5  Buffer Pool and I/O Information
          05 POOL-ASYNC-INDEX-READS  PIC 9(9) COMP-5.
          05 POOL-DATA-TO-ESTORE     PIC 9(9) COMP-5.
          05 POOL-INDEX-TO-ESTORE    PIC 9(9) COMP-5.
          05 POOL-INDEX-FROM-ESTORE  PIC 9(9) COMP-5.
          05 POOL-DATA-FROM-ESTORE   PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2  Database Status
          05 SQLM-DB-STATUS          PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2  lock information
          05 LOCK-TIMEOUTS           PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2  High Water Marks
          05 CONNECTIONS-TOP         PIC 9(9) COMP-5.
          05 DB-HEAP-TOP             PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2  Buffer Pool and I/O Information
          05 POOL-ASYNC-DATA-READS     PIC 9(9) COMP-5.
          05 POOL-ASYNC-DATA-WRITES    PIC 9(9) COMP-5.
          05 POOL-ASYNC-INDEX-WRITES   PIC 9(9) COMP-5.
          05 POOL-ASYNC-READ-TIME      PIC 9(9) COMP-5.
          05 POOL-ASYNC-WRITE-TIME     PIC 9(9) COMP-5.
          05 POOL-ASYNC-DATA-READ-REQS PIC 9(9) COMP-5.
          05 POOL-LSN-GAP-CLNS         PIC 9(9) COMP-5.
          05 POOL-DRTY-PG-STEAL-CLNS   PIC 9(9) COMP-5.
          05 POOL-DRTY-PG-THRSH-CLNS   PIC 9(9) COMP-5.
          05 DIRECT-READS              PIC 9(9) COMP-5.
          05 DIRECT-WRITES             PIC 9(9) COMP-5.
          05 DIRECT-READ-REQS          PIC 9(9) COMP-5.
          05 DIRECT-WRITE-REQS         PIC 9(9) COMP-5.
          05 DIRECT-READ-TIME          PIC 9(9) COMP-5.
          05 DIRECT-WRITE-TIME         PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2 Internal counts
          05 INT-ROWS-INSERTED       PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2 Logging Information
          05 LOG-READS               PIC 9(9) COMP-5.
          05 LOG-WRITES              PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2 Package Cache Information
          05 PKG-CACHE-LOOKUPS       PIC 9(9) COMP-5.
          05 PKG-CACHE-INSERTS       PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2 Catalog Cache Information
          05 CAT-CACHE-LOOKUPS       PIC 9(9) COMP-5.
          05 CAT-CACHE-INSERTS       PIC 9(9) COMP-5.
          05 CAT-CACHE-OVERFLOWS     PIC 9(9) COMP-5.
          05 CAT-CACHE-HEAP-FULL     PIC 9(9) COMP-5.

      * SQLM_DBMON_VERSION5
          05 CATALOG-NODE            PIC 9(9) COMP-5.
          05 TOTAL-SEC-CONS          PIC 9(9) COMP-5.
          05 NUM-ASSOC-AGENTS        PIC 9(9) COMP-5.
          05 AGENTS-TOP              PIC 9(9) COMP-5.
          05 COORD-AGENTS-TOP        PIC 9(9) COMP-5.
          05 DB-LOCATION             PIC 9(4) COMP-5.
          05 SERVER-PLATFORM         PIC 9(4) COMP-5.
          05 CATALOG-NODE-NAME       PIC X(32) USAGE DISPLAY NATIVE.
          05 PREFETCH-WAIT-TIME      PIC 9(9) COMP-5.

      * SQLM_DBMON_VERSION5 Application Section Retrieval Information
          05 APPL-SECTION-LOOKUPS    PIC 9(9) COMP-5.
          05 APPL-SECTION-INSERTS    PIC 9(9) COMP-5.

      * SQLM_DBMON_VERSION5_2 Information
          05 TOTAL-HASH-JOINS          PIC 9(9) COMP-5.
          05 TOTAL-HASH-LOOPS          PIC 9(9) COMP-5.
          05 HASH-JOIN-OVERFLOWS       PIC 9(9) COMP-5.
          05 HASH-JOIN-SMALL-OVERFLOWS PIC 9(9) COMP-5.

      **********************************************************************
      * Application Related Snapshot Information
      **********************************************************************

       01 SQLM-APPL.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X(3) USAGE DISPLAY NATIVE.

      *   Identification information
          05 AGENT-ID                PIC 9(9) COMP-5.
          05 APPL-STATUS             PIC 9(9) COMP-5.
          05 CODEPAGE-ID             PIC 9(9) COMP-5.
          05 STATUS-CHANGE-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 APPL-NAME               PIC X(20) USAGE DISPLAY NATIVE.
          05 APPL-ID                 PIC X(32) USAGE DISPLAY NATIVE.
          05 SEQUENCE-NO             PIC X(4) USAGE DISPLAY NATIVE.
          05 AUTH-ID                 PIC X(20) USAGE DISPLAY NATIVE.
          05 CLIENT-NNAME            PIC X(20) USAGE DISPLAY NATIVE.
          05 CLIENT-PRDID            PIC X(20) USAGE DISPLAY NATIVE.
          05 INPUT-DB-ALIAS          PIC X(20) USAGE DISPLAY NATIVE.
          05 CLIENT-DB-ALIAS         PIC X(20) USAGE DISPLAY NATIVE.
          05 DB-NAME                 PIC X(20) USAGE DISPLAY NATIVE.
          05 DB-PATH                 PIC X(256) USAGE DISPLAY NATIVE.
          05 NUM-ASSOC-AGENTS        PIC 9(9) COMP-5.
          05 COORD-NODE-NUM          PIC 9(9) COMP-5.
          05 AUTHORITY-LVL           PIC 9(9) COMP-5.

      *   Locking information
          05 LOCKS-HELD              PIC 9(9) COMP-5.
          05 LOCK-WAITS              PIC 9(9) COMP-5.
          05 LOCK-WAIT-TIME          PIC 9(9) COMP-5.
          05 LOCK-ESCALS             PIC 9(9) COMP-5.
          05 X-LOCK-ESCALS           PIC 9(9) COMP-5.
          05 DEADLOCKS               PIC 9(9) COMP-5.
          05 UOW-LOCK-WAIT-TIME      PIC 9(9) COMP-5.
          05 AGENT-ID-HOLDING-LK     PIC 9(9) COMP-5.
          05 TABLE-FILE-ID           PIC 9(9) COMP-5.
          05 LOCK-MODE               PIC 9(9) COMP-5.
          05 LOCK-OBJECT-TYPE        PIC 9(9) COMP-5.
          05 LOCK-OBJECT-NAME        PIC 9(9) COMP-5.
          05 LOCK-WAIT-START-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 APPL-ID-HOLDING-LK      PIC X(32) USAGE DISPLAY NATIVE.
          05 SEQUENCE-NO-HOLDING-LK  PIC X(4) USAGE DISPLAY NATIVE.

      *   Sorting information
          05 TOTAL-SORTS             PIC 9(9) COMP-5.
          05 TOTAL-SORT-TIME         PIC 9(9) COMP-5.
          05 SORT-OVERFLOWS          PIC 9(9) COMP-5.

      *   Buffer Pool information
          05 POOL-DATA-L-READS       PIC 9(9) COMP-5.
          05 POOL-DATA-P-READS       PIC 9(9) COMP-5.
          05 POOL-DATA-WRITES        PIC 9(9) COMP-5.
          05 POOL-INDEX-L-READS      PIC 9(9) COMP-5.
          05 POOL-INDEX-P-READS      PIC 9(9) COMP-5.
          05 POOL-INDEX-WRITES       PIC 9(9) COMP-5.
          05 POOL-READ-TIME          PIC 9(9) COMP-5.
          05 POOL-WRITE-TIME         PIC 9(9) COMP-5.

      *   SQL Statement counts
          05 COMMIT-SQL-STMTS        PIC 9(9) COMP-5.
          05 ROLLBACK-SQL-STMTS      PIC 9(9) COMP-5.
          05 DYNAMIC-SQL-STMTS       PIC 9(9) COMP-5.
          05 STATIC-SQL-STMTS        PIC 9(9) COMP-5.
          05 FAILED-SQL-STMTS        PIC 9(9) COMP-5.
          05 SELECT-SQL-STMTS        PIC 9(9) COMP-5.
          05 DDL-SQL-STMTS           PIC 9(9) COMP-5.
          05 UID-SQL-STMTS           PIC 9(9) COMP-5.

      *   Internal counts
          05 INT-AUTO-REBINDS        PIC 9(9) COMP-5.
          05 INT-ROWS-DELETED        PIC 9(9) COMP-5.
          05 INT-ROWS-UPDATED        PIC 9(9) COMP-5.
          05 INT-COMMITS             PIC 9(9) COMP-5.
          05 INT-ROLLBACKS           PIC 9(9) COMP-5.
          05 INT-DEADLOCK-ROLLBACKS  PIC 9(9) COMP-5.

      *   Row counts
          05 ROWS-DELETED            PIC 9(9) COMP-5.
          05 ROWS-INSERTED           PIC 9(9) COMP-5.
          05 ROWS-UPDATED            PIC 9(9) COMP-5.
          05 ROWS-SELECTED           PIC 9(9) COMP-5.

      *   Binds/precompiles
          05 BINDS-PRECOMPILES       PIC 9(9) COMP-5.

      *   SQL Statement information
          05 STMT-TYPE               PIC 9(9) COMP-5.
          05 STMT-OPERATION          PIC 9(9) COMP-5.
          05 SECTION-NUMBER          PIC 9(9) COMP-5.
          05 STMT-LENGTH             PIC 9(9) COMP-5.
          05 STMT-SORTS              PIC 9(9) COMP-5.
          05 STMT-START.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 STMT-STOP.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 CURSOR-NAME             PIC X(20) USAGE DISPLAY NATIVE.
          05 CREATOR                 PIC X(20) USAGE DISPLAY NATIVE.
          05 PACKAGE-NAME            PIC X(20) USAGE DISPLAY NATIVE.

      *   Unit of Work information
          05 UOW-LOG-SPACE-USED      PIC 9(9) COMP-5.
          05 UOW-COMP-STATUS         PIC 9(9) COMP-5.
          05 PREV-UOW-STOP-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 UOW-START-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 UOW-STOP-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.

      *   Remote cursor information
          05 OPEN-REM-CURS           PIC 9(9) COMP-5.
          05 OPEN-REM-CURS-BLK       PIC 9(9) COMP-5.
          05 REJ-CURS-BLK            PIC 9(9) COMP-5.
          05 ACC-CURS-BLK            PIC 9(9) COMP-5.

      *   Timestamps
          05 APPL-CON-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 CONN-COMPLETE-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 LAST-RESET.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.

      *   Communication information
          05 CURR-COMM-HEAP-SIZE     PIC 9(9) COMP-5.
          05 MAX-COMM-HEAP-SIZE      PIC 9(9) COMP-5.

      * SQLM_DBMON_VERSION5  Agent CPU times
          05  APPL-IDLE-TIME         PIC 9(9) COMP-5.

      * SQLM-DBMON-VERSION5  Buffer Pool and I/O Information
          05  POOL-DATA-TO-ESTORE    PIC 9(9) COMP-5.
          05  POOL-INDEX-TO-ESTORE   PIC 9(9) COMP-5.
          05  POOL-INDEX-FROM-ESTORE PIC 9(9) COMP-5.
          05  POOL-DATA-FROM-ESTORE  PIC 9(9) COMP-5.

          05  AUTHORITY-LVL          PIC 9(9) COMP-5.
          05  SQL-REQS-SINCE-COMMIT  PIC 9(9) COMP-5.
          05  NUM-SEC-APPL-STRUCTS   PIC 9(9) COMP-5.


      *   SQLM-DBMON-VERSION2  Lock Wait Information
          05 TABLE-NAME              PIC X(20) USAGE DISPLAY NATIVE.
          05 TABLE-SCHEMA            PIC X(20) USAGE DISPLAY NATIVE.
          05 TABLESPACE-NAME         PIC X(20) USAGE DISPLAY NATIVE.

      *   SQLM-DBMON-VERSION2  lock information
          05 LOCK-TIMEOUTS           PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2  Agent CPU times
          05 AGENT-USR-CPU-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 AGENT-SYS-CPU-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2  SQL Statement CPU times
          05 STMT-USR-CPU-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 STMT-SYS-CPU-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2  Internal counts
          05 INT-ROWS-INSERTED       PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2  Row Counts
          05 ROWS-READ               PIC 9(9) COMP-5.
          05 ROWS-WRITTEN            PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2  I/O Information
          05 DIRECT-READS            PIC 9(9) COMP-5.
          05 DIRECT-WRITES           PIC 9(9) COMP-5.
          05 DIRECT-READ-REQS        PIC 9(9) COMP-5.
          05 DIRECT-WRITE-REQS       PIC 9(9) COMP-5.
          05 DIRECT-READ-TIME        PIC 9(9) COMP-5.
          05 DIRECT-WRITE-TIME       PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2 Local Cursor Information
          05 OPEN-LOC-CURS           PIC 9(9) COMP-5.
          05 OPEN-LOC-CURS-BLK       PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2  Client Identification Information
          05 EXECUTION-ID            PIC X(20) USAGE DISPLAY NATIVE.
          05 CORR-TOKEN              PIC X(32) USAGE DISPLAY NATIVE.
          05 CLIENT-PID              PIC 9(9) COMP-5.
          05 CLIENT-PLATFORM         PIC 9(4) COMP-5.
          05 CLIENT-PROTOCOL         PIC 9(4) COMP-5.
          05 COUNTRY-CODE            PIC 9(4) COMP-5.
          05 FILLER2                 PIC X(2) USAGE DISPLAY NATIVE.

      *   SQLM-DBMON-VERSION2 Package Cache Information
          05 PKG-CACHE-LOOKUPS       PIC 9(9) COMP-5.
          05 PKG-CACHE-INSERTS       PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION2 Catalog Cache Information
          05 CAT-CACHE-LOOKUPS       PIC 9(9) COMP-5.
          05 CAT-CACHE-INSERTS       PIC 9(9) COMP-5.
          05 CAT-CACHE-OVERFLOWS     PIC 9(9) COMP-5.
          05 CAT-CACHE-HEAP-FULL     PIC 9(9) COMP-5.

      *   SQLM-DBMON-VERSION5
          05 COORD-NODE              PIC 9(9) COMP-5.
          05 NUM-ASSOC-AGENTS        PIC 9(9) COMP-5.
          05 NUM-AGENTS              PIC 9(9) COMP-5.
          05 AGENTS-STOLEN           PIC 9(9) COMP-5.
          05 ASSOCIATED-AGENTS-TOP   PIC 9(9) COMP-5.

          05 APPL-PRIORITY           PIC S9(9) COMP-5.
          05 APPL-PRIORITY-TYPE      PIC 9(9) COMP-5.
          05 PREFETCH-WAIT-TIME      PIC 9(9) COMP-5.
          05 COORD-AGENT-PID         PIC S9(9) COMP-5.

          05 APPL-SECTION-LOOKUPS      PIC 9(9) COMP-5.
          05 APPL-SECTION-INSERTS      PIC 9(9) COMP-5.
          05 LOCKS-WAITING             PIC 9(9) COMP-5.
          05 OUTBOUND-COMM-ADDRESS     PIC X(32) USAGE DISPLAY NATIVE.
          05 INBOUND-COMM-ADDRESS      PIC X(32) USAGE DISPLAY NATIVE.
          05 OUTBOUND-COMM-PROTOCOL    PIC 9(9) COMP-5.
          05 FILLER3                   PIC X(3) USAGE DISPLAY NATIVE.

      * SQLM-DBMON-VERSION5-2 
          05 TOTAL-HASH-JOINS          PIC 9(9) COMP-5.
          05 TOTAL-HASH-LOOPS          PIC 9(9) COMP-5.
          05 HASH-JOIN-OVERFLOWS       PIC 9(9) COMP-5.
          05 HASH-JOIN-SMALL-OVERFLOWS PIC 9(9) COMP-5.        

       01 SQLM-STMT.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X USAGE DISPLAY NATIVE.
          05 STMT-NODE-NUMBER        PIC 9(4) COMP-5.
          05 STMT-TYPE               PIC 9(9) COMP-5.
          05 STMT-OPERATION          PIC 9(9) COMP-5.
          05 SECTION-NUMBER          PIC 9(9) COMP-5.
          05 QUERY-COST-ESTIMATE     PIC 9(9) COMP-5.
          05 QUERY-CARD-ESTIMATE     PIC 9(9) COMP-5.
          05 DEGREE-PARALLELISM      PIC 9(9) COMP-5.
          05 STMT-SORTS              PIC 9(9) COMP-5.
          05 TOTAL-SORT-TIME         PIC 9(9) COMP-5.
          05 SORT-OVERFLOWS          PIC 9(9) COMP-5.
          05 ROWS-READ               PIC 9(9) COMP-5.
          05 ROWS-WRITTEN            PIC 9(9) COMP-5.
          05 INT-ROWS-DELETED        PIC 9(9) COMP-5.
          05 INT-ROWS-UPDATED        PIC 9(9) COMP-5.
          05 INT-ROWS-INSERTED       PIC 9(9) COMP-5.
          05 FETCH-COUNT             PIC 9(9) COMP-5.
          05 STMT-START.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 STMT-STOP.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 CURSOR-NAME             PIC X(20) USAGE DISPLAY NATIVE.
          05 CREATOR                 PIC X(20) USAGE DISPLAY NATIVE.
          05 PACKAGE-NAME            PIC X(20) USAGE DISPLAY NATIVE.
          05 STMT-USR-CPU-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 STMT-SYS-CPU-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 NUM-SS                  PIC 9(9) COMP-5.
          05 NUM-AGENTS              PIC 9(9) COMP-5.
          05 AGENTS-TOP              PIC 9(9) COMP-5.
          05 STMT-LENGTH             PIC 9(4) COMP-5.
          05 STMT-TEXT-OFFSET        PIC 9(4) COMP-5.

      * SQL Subsection -  returned on application snapshot (SQLMA_APPL)
       01 SQLM-SUBSECTION.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X USAGE DISPLAY NATIVE.
          05 SS-NUMBER               PIC 9(4) COMP-5.
          05 SS-STATUS               PIC 9(4) COMP-5.
          05 SS-NODE-NUMBER          PIC 9(4) COMP-5.
          05 SS-EXEC-TIME            PIC 9(9) COMP-5.
          05 SS-USR-CPU-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 SS-SYS-CPU-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.

          05 TQ-NODE-WAITED-FOR      PIC 9(4) COMP-5.
          05 TQ-WAIT-FOR-ANY         PIC 9(4) COMP-5.
          05 TQ-ID-WAITING-ON        PIC 9(4) COMP-5.
          05 FILLER                  PIC X(2) USAGE DISPLAY NATIVE.
          05 TQ-TOT-SEND-SPILLS      PIC 9(9) COMP-5.
          05 TQ-CUR-SEND-SPILLS      PIC 9(9) COMP-5.
          05 TQ-MAX-SEND-SPILLS      PIC 9(9) COMP-5.
          05 TQ-ROWS-READ            PIC 9(9) COMP-5.
          05 TQ-ROWS-WRITTEN         PIC 9(9) COMP-5.

          05 ROWS-READ               PIC 9(9) COMP-5.
          05 ROWS-WRITTEN            PIC 9(9) COMP-5.

          05 NUM-AGENTS              PIC 9(9) COMP-5.

      ****************************************************************
      * SQL Agent -  returned on application snapshot (SQLMA_APPL)
      ****************************************************************
       01 SQLM-AGENT.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X(3) USAGE DISPLAY NATIVE.
          05 AGENT-PID               PIC S9(9) COMP-5.

      ******************************************************************
      * Lock Wait Snapshot Information
      ******************************************************************
       01 SQLM-LOCK-WAIT.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X(3) USAGE DISPLAY NATIVE.
          05 SUBSECTION-NUMBER       PIC 9(9) COMP-5.
          05 LOCK-MODE               PIC 9(9) COMP-5.
          05 LOCK-OBJECT-TYPE        PIC 9(9) COMP-5.
          05 LOCK-WAIT-START-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 TABLE-NAME              PIC X(20) USAGE DISPLAY NATIVE.
          05 TABLE-SCHEMA            PIC X(20) USAGE DISPLAY NATIVE.
          05 TABLESPACE-NAME         PIC X(20) USAGE DISPLAY NATIVE.
          05 AGENT-ID-HOLDING-LK     PIC 9(9) COMP-5.
          05 APPL-ID-HOLDING-LK      PIC X(32) USAGE DISPLAY NATIVE.

      **********************************************************************
      * Table Related Snapshot Information
      **********************************************************************

      *Table Header
       01 SQLM-TABLE-HEADER.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X(3) USAGE DISPLAY NATIVE.
          05 NUM-TABLES              PIC 9(9) COMP-5.
          05 DB-CONN-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 LAST-RESET.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 INPUT-DB-ALIAS          PIC X(20) USAGE DISPLAY NATIVE.
          05 DB-NAME                 PIC X(20) USAGE DISPLAY NATIVE.
          05 DB-PATH                 PIC X(256) USAGE DISPLAY NATIVE.

      *Individual Table information
       01 SQLM-TABLE.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X(3) USAGE DISPLAY NATIVE.
          05 TABLE-FILE-ID           PIC 9(9) COMP-5.
          05 TABLE-TYPE              PIC 9(9) COMP-5.
          05 ROWS-WRITTEN            PIC 9(9) COMP-5.
          05 ROWS-READ               PIC 9(9) COMP-5.
          05 OVERFLOW-ACCESSES       PIC 9(9) COMP-5.
          05 PAGE-REORGS             PIC 9(9) COMP-5.
          05 RESERVED                PIC X(28) USAGE DISPLAY NATIVE.

      *   SQLM-DBMON-VERSION2  Table Identification
          05 TABLE-NAME              PIC X(20) USAGE DISPLAY NATIVE.
          05 TABLE-SCHEMA            PIC X(20) USAGE DISPLAY NATIVE.

      **********************************************************************
      * Lock Related Snapshot Information
      **********************************************************************

      *Database Lock information
       01 SQLM-DBASE-LOCK.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X(3) USAGE DISPLAY NATIVE.
          05 LOCKS-HELD              PIC 9(9) COMP-5.
          05 APPLS-CUR-CONS          PIC 9(9) COMP-5.
          05 NUM-APPLS               PIC 9(9) COMP-5.
          05 LOCKS-WAITING           PIC 9(9) COMP-5.
          05 INPUT-DB-ALIAS          PIC X(20) USAGE DISPLAY NATIVE.
          05 DB-NAME                 PIC X(20) USAGE DISPLAY NATIVE.
          05 DB-PATH                 PIC X(256) USAGE DISPLAY NATIVE.

      *Application Lock information
       01 SQLM-APPL-LOCK.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X(3) USAGE DISPLAY NATIVE.
          05 AGENT-ID                PIC 9(9) COMP-5.
          05 APPL-STATUS             PIC 9(9) COMP-5.
          05 CODEPAGE-ID             PIC 9(9) COMP-5.
          05 LOCKS-HELD              PIC 9(9) COMP-5.
          05 NUM-LOCKS               PIC 9(9) COMP-5.
          05 STATUS-CHANGE-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 APPL-ID                 PIC X(32) USAGE DISPLAY NATIVE.
          05 SEQUENCE-NO             PIC X(4) USAGE DISPLAY NATIVE.
          05 APPL-NAME               PIC X(20) USAGE DISPLAY NATIVE.
          05 AUTH-ID                 PIC X(20) USAGE DISPLAY NATIVE.
          05 CLIENT-DB-ALIAS         PIC X(20) USAGE DISPLAY NATIVE.

      *   Lock waited on information
          05 AGENT-ID-HOLDING-LK     PIC 9(9) COMP-5.
          05 LOCK-OBJECT-NAME        PIC 9(9) COMP-5.
          05 LOCK-OBJECT-TYPE        PIC 9(9) COMP-5.
          05 TABLE-FILE-ID           PIC 9(9) COMP-5.
          05 APPL-ID-HOLDING-LK      PIC X(32) USAGE DISPLAY NATIVE.
          05 SEQUENCE-NO-HOLDING-LK  PIC X(4) USAGE DISPLAY NATIVE.

          05 RESERVED                PIC X(24) USAGE DISPLAY NATIVE.

          05 LOCK-WAIT-TIME          PIC 9(9) COMP-5.
          05 NUM-LOCKWAITS           PIC 9(9) COMP-5.

          05 TABLE-NAME              PIC X(20) USAGE DISPLAY NATIVE.
          05 TABLE-SCHEMA            PIC X(20) USAGE DISPLAY NATIVE.
          05 TABLESPACE-NAME         PIC X(20) USAGE DISPLAY NATIVE.

      *Individual Lock information
       01 SQLM-LOCK.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X(3) USAGE DISPLAY NATIVE.
          05 TABLE-FILE-ID           PIC 9(9) COMP-5.
          05 LOCK-OBJECT-TYPE        PIC 9(9) COMP-5.
          05 LOCK-MODE               PIC 9(9) COMP-5.
          05 LOCK-STATUS             PIC 9(9) COMP-5.
          05 LOCK-OBJECT-NAME        PIC 9(9) COMP-5.
          05 RESERVED                PIC X(32) USAGE DISPLAY NATIVE.

      *   SQLM-DBMON-VERSION2  Lock Information
          05 TABLE-NAME              PIC X(20) USAGE DISPLAY NATIVE.
          05 TABLE-SCHEMA            PIC X(20) USAGE DISPLAY NATIVE.
          05 TABLESPACE-NAME         PIC X(20) USAGE DISPLAY NATIVE.


      **********************************************************************
      * Tablespace Related Snapshot Information
      **********************************************************************

      * Tablespace Header
       01 SQLM-TABLESPACE-HEADER.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X(3) USAGE DISPLAY NATIVE.
          05 NUM-TABLESPACES         PIC 9(9) COMP-5.
          05 DB-CONN-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 LAST-RESET.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 INPUT-DB-ALIAS          PIC X(20) USAGE DISPLAY NATIVE.
          05 DB-NAME                 PIC X(20) USAGE DISPLAY NATIVE.
          05 DB-PATH                 PIC X(256) USAGE DISPLAY NATIVE.


      * Individual Tablespace information
       01 SQLM-TABLESPACE.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X(3) USAGE DISPLAY NATIVE.
          05 TABLESPACE-NAME         PIC X(20) USAGE DISPLAY NATIVE.

      *   Buffer Pool Information
          05 POOL-DATA-L-READS         PIC 9(9) COMP-5.
          05 POOL-DATA-P-READS         PIC 9(9) COMP-5.
          05 POOL-ASYNC-DATA-READS     PIC 9(9) COMP-5.
          05 POOL-DATA-WRITES          PIC 9(9) COMP-5.
          05 POOL-ASYNC-DATA-WRITES    PIC 9(9) COMP-5.
          05 POOL-INDEX-L-READS        PIC 9(9) COMP-5.
          05 POOL-INDEX-P-READS        PIC 9(9) COMP-5.
          05 POOL-INDEX-WRITES         PIC 9(9) COMP-5.
          05 POOL-ASYNC-INDEX-WRITES   PIC 9(9) COMP-5.
          05 POOL-READ-TIME            PIC 9(9) COMP-5.
          05 POOL-WRITE-TIME           PIC 9(9) COMP-5.
          05 POOL-ASYNC-READ-TIME      PIC 9(9) COMP-5.
          05 POOL-ASYNC-WRITE-TIME     PIC 9(9) COMP-5.
          05 POOL-ASYNC-DATA-READ-REQS PIC 9(9) COMP-5.

      *   Direct I/O information
          05 DIRECT-READS            PIC 9(9) COMP-5.
          05 DIRECT-WRITES           PIC 9(9) COMP-5.
          05 DIRECT-READ-REQS        PIC 9(9) COMP-5.
          05 DIRECT-WRITE-REQS       PIC 9(9) COMP-5.
          05 DIRECT-READ-TIME        PIC 9(9) COMP-5.
          05 DIRECT-WRITE-TIME       PIC 9(9) COMP-5.
          05 FILES-CLOSED            PIC 9(9) COMP-5.

      * SQLM_DBMON_VERSION5  Buffer Pool and I/O Information
          05 POOL-ASYNC-INDEX-READS    PIC 9(9) COMP-5.
          05 POOL-DATA-TO-ESTORE       PIC 9(9) COMP-5.
          05 POOL-INDEX-TO-ESTORE      PIC 9(9) COMP-5.
          05 POOL-INDEX-FROM-ESTORE    PIC 9(9) COMP-5.
          05 POOL-DATA-FROM-ESTORE     PIC 9(9) COMP-5.


       01 SQLM-ROLLFWD-INFO.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X USAGE DISPLAY NATIVE.
          05 NODE-NUMBER             PIC 9(4) COMP-5.
          05 RF-TYPE                 PIC 9(9) COMP-5.
          05 RF-LOG-NUM              PIC 9(9) COMP-5.
          05 RF-STATUS               PIC 9(9) COMP-5.
          05 RF-TIMESTAMP            PIC 9(9) COMP-5.
          05 RF-NUM-TSPACES          PIC 9(9) COMP-5.

       01 SQLM-ROLLFWD-TS-INFO.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X(3) USAGE DISPLAY NATIVE.
          05 TS-NAME                 PIC X(20) USAGE DISPLAY NATIVE.

      ******************************************************************
      * Bufferpool Related Snapshot Information
      ******************************************************************
       01 SQLM-BUFFERPOOL.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 INFO-TYPE               PIC X USAGE DISPLAY NATIVE.
          05 FILLER                  PIC X(3) USAGE DISPLAY NATIVE.
          05 BP-NAME                 PIC X(20) USAGE DISPLAY NATIVE.
          05 INPUT-DB-ALIAS          PIC X(20) USAGE DISPLAY NATIVE.
          05 DB-NAME                 PIC X(20) USAGE DISPLAY NATIVE.
          05 DB-PATH                 PIC X(256) USAGE DISPLAY NATIVE.

          05 POOL-DATA-L-READS       PIC 9(9) COMP-5.
          05 POOL-DATA-P-READS       PIC 9(9) COMP-5.
          05 POOL-DATA-WRITES        PIC 9(9) COMP-5.
          05 POOL-INDEX-L-READS      PIC 9(9) COMP-5.
          05 POOL-INDEX-P-READS      PIC 9(9) COMP-5.
          05 POOL-INDEX-WRITES       PIC 9(9) COMP-5.
          05 POOL-READ-TIME          PIC 9(9) COMP-5.
          05 POOL-WRITE-TIME         PIC 9(9) COMP-5.
          05 FILES-CLOSED            PIC 9(9) COMP-5.

          05 POOL-ASYNC-DATA-READS   PIC 9(9) COMP-5.
          05 POOL-ASYNC-DATA-WRITES  PIC 9(9) COMP-5.
          05 POOL-ASYNC-INDEX-WRITES PIC 9(9) COMP-5.
          05 POOL-ASYNC-READ-TIME    PIC 9(9) COMP-5.
          05 POOL-ASYNC-WRITE-TIME   PIC 9(9) COMP-5.
          05 POOL-ASYNC-DATA-READ-REQS PIC 9(9) COMP-5.
          05 DIRECT-READS            PIC 9(9) COMP-5.
          05 DIRECT-WRITES           PIC 9(9) COMP-5.
          05 DIRECT-READ-REQS        PIC 9(9) COMP-5.
          05 DIRECT-WRITE-REQS       PIC 9(9) COMP-5.
          05 DIRECT-READ-TIME        PIC 9(9) COMP-5.
          05 DIRECT-WRITE-TIME       PIC 9(9) COMP-5.

          05 POOL-ASYNC-INDEX-READS  PIC 9(9) COMP-5.
          05 POOL-DATA-TO-ESTORE     PIC 9(9) COMP-5.
          05 POOL-INDEX-TO-ESTORE    PIC 9(9) COMP-5.
          05 POOL-INDEX-FROM-ESTORE  PIC 9(9) COMP-5.
          05 POOL-DATA-FROM-ESTORE   PIC 9(9) COMP-5.

      **********************************************************************
      * Database Monitor Event Monitor Data Structures
      **********************************************************************
      
      **********************************************************************
      *  Event Log Stream Header 
      **********************************************************************

       01 SQLM-EVENT-LOG-STREAM-HEADER.
          05 BYTE-ORDER             PIC S9(9) COMP-5.
          05 SQLM-SIZE              PIC 9(9) COMP-5.
          05 VERSION                PIC 9(9) COMP-5.

      **********************************************************************
      *  Event Monitor Log Header Record
      **********************************************************************

       01 SQLM-EV-LOG-HEADER.
          05 BYTE-ORDER              PIC S9(9) COMP-5.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 VERSION                 PIC 9(9) COMP-5.
          05 EVENT-MONITOR-NAME      PIC X(20) USAGE DISPLAY NATIVE.
          05 CODEPAGE-ID             PIC 9(4) COMP-5.
          05 COUNTRY-CODE            PIC 9(4) COMP-5.
          05 SERVER-PRDID            PIC X(20) USAGE DISPLAY NATIVE.
          05 SERVER-INSTANCE-NAME    PIC X(20) USAGE DISPLAY NATIVE.
          05 NUM-NODES-IN-DB2-INSTANCE PIC 9(9) COMP-5.

      **********************************************************************
      *  Event Monitor Header Record
      **********************************************************************

       01 SQLM-EVENT-REC-HEADER.
          05 SQLM-SIZE                 PIC 9(9) COMP-5.
          05 EVENT-TYPE                PIC 9(9) COMP-5.

      **********************************************************************
      *  Event Monitor Database Event Record
      **********************************************************************

       01 SQLM-DB-EVENT.
          05 SQLM-SIZE                 PIC 9(9) COMP-5.
          05 EVENT-TYPE                PIC 9(9) COMP-5.

      *   Locking Information
          05 LOCK-WAITS                PIC 9(9) COMP-5.
          05 LOCK-WAIT-TIME            PIC 9(9) COMP-5.
          05 DEADLOCKS                 PIC 9(9) COMP-5.
          05 LOCK-ESCALS               PIC 9(9) COMP-5.
          05 X-LOCK-ESCALS             PIC 9(9) COMP-5.
          05 LOCK-TIMEOUTS             PIC 9(9) COMP-5.

      *   Sorting Information
          05 TOTAL-SORTS               PIC 9(9) COMP-5.
          05 TOTAL-SORT-TIME           PIC 9(9) COMP-5.
          05 SORT-OVERFLOWS            PIC 9(9) COMP-5.

      *   Buffer Pool and I/O Information
          05 POOL-DATA-L-READS         PIC 9(9) COMP-5.
          05 POOL-DATA-P-READS         PIC 9(9) COMP-5.
          05 POOL-ASYNC-DATA-READS     PIC 9(9) COMP-5.
          05 POOL-DATA-WRITES          PIC 9(9) COMP-5.
          05 POOL-ASYNC-DATA-WRITES    PIC 9(9) COMP-5.
          05 POOL-INDEX-L-READS        PIC 9(9) COMP-5.
          05 POOL-INDEX-P-READS        PIC 9(9) COMP-5.
          05 POOL-INDEX-WRITES         PIC 9(9) COMP-5.
          05 POOL-ASYNC-INDEX-WRITES   PIC 9(9) COMP-5.
          05 POOL-READ-TIME            PIC 9(9) COMP-5.
          05 POOL-WRITE-TIME           PIC 9(9) COMP-5.
          05 POOL-ASYNC-READ-TIME      PIC 9(9) COMP-5.
          05 POOL-ASYNC-WRITE-TIME     PIC 9(9) COMP-5.
          05 POOL-ASYNC-DATA-READ-REQS PIC 9(9) COMP-5.
          05 POOL-LSN-GAP-CLNS         PIC 9(9) COMP-5.
          05 POOL-DRTY-PG-STEAL-CLNS   PIC 9(9) COMP-5.
          05 POOL-DRTY-PG-THRSH-CLNS   PIC 9(9) COMP-5.
          05 DIRECT-READS              PIC 9(9) COMP-5.
          05 DIRECT-WRITES             PIC 9(9) COMP-5.
          05 DIRECT-READ-REQS          PIC 9(9) COMP-5.
          05 DIRECT-WRITE-REQS         PIC 9(9) COMP-5.
          05 DIRECT-READ-TIME          PIC 9(9) COMP-5.
          05 DIRECT-WRITE-TIME         PIC 9(9) COMP-5.
          05 FILES-CLOSED              PIC 9(9) COMP-5.

      *   SQL statement counts
          05 COMMIT-SQL-STMTS          PIC 9(9) COMP-5.
          05 ROLLBACK-SQL-STMTS        PIC 9(9) COMP-5.
          05 DYNAMIC-SQL-STMTS         PIC 9(9) COMP-5.
          05 STATIC-SQL-STMTS          PIC 9(9) COMP-5.
          05 FAILED-SQL-STMTS          PIC 9(9) COMP-5.
          05 SELECT-SQL-STMTS          PIC 9(9) COMP-5.
          05 DDL-SQL-STMTS             PIC 9(9) COMP-5.
          05 UID-SQL-STMTS             PIC 9(9) COMP-5.

      *   Internal counts
          05 INT-AUTO-REBINDS          PIC 9(9) COMP-5.
          05 INT-ROWS-DELETED          PIC 9(9) COMP-5.
          05 INT-ROWS-UPDATED          PIC 9(9) COMP-5.
          05 INT-ROWS-INSERTED         PIC 9(9) COMP-5.
          05 INT-COMMITS               PIC 9(9) COMP-5.
          05 INT-ROLLBACKS             PIC 9(9) COMP-5.

      *   Row counts
          05 ROWS-DELETED              PIC 9(9) COMP-5.
          05 ROWS-INSERTED             PIC 9(9) COMP-5.
          05 ROWS-UPDATED              PIC 9(9) COMP-5.
          05 ROWS-SELECTED             PIC 9(9) COMP-5.

      *   Binds/Precompiles
          05 BINDS-PRECOMPILES         PIC 9(9) COMP-5.

      *   Timestamps
          05 DISCONN-TIME.
             10 SECONDS                PIC 9(9) COMP-5.
             10 MICROSEC               PIC 9(9) COMP-5.

      *   Application connection counters
          05 TOTAL-CONS                PIC 9(9) COMP-5.
          05 CONNECTIONS-TOP           PIC 9(9) COMP-5.

      *   Database memory pool high water marks
          05 DB-HEAP-TOP               PIC 9(9) COMP-5.

      *   Logging Information
          05 SEC-LOG-USED-TOP          PIC 9(9) COMP-5.
          05 TOT-LOG-USED-TOP          PIC 9(9) COMP-5.
          05 LOG-READS                 PIC 9(9) COMP-5.
          05 LOG-WRITES                PIC 9(9) COMP-5.

      *   Package Cache Information
          05 PKG-CACHE-LOOKUPS         PIC 9(9) COMP-5.
          05 PKG-CACHE-INSERTS         PIC 9(9) COMP-5.

      *   Catalog Cache Information
          05 CAT-CACHE-LOOKUPS         PIC 9(9) COMP-5.
          05 CAT-CACHE-INSERTS         PIC 9(9) COMP-5.
          05 CAT-CACHE-OVERFLOWS       PIC 9(9) COMP-5.
          05 CAT-CACHE-HEAP-FULL       PIC 9(9) COMP-5.

      * Application Section Retrieval Information
          05 APPL-SECTION-LOOKUPS      PIC 9(9) COMP-5.
          05 APPL-SECTION-INSERTS      PIC 9(9) COMP-5.

      * SQLM_DBMON_VERSION5  Buffer Pool and I/O Information
          05 POOL-ASYNC-INDEX-READS    PIC 9(9) COMP-5.
          05 POOL-DATA-TO-ESTORE       PIC 9(9) COMP-5.
          05 POOL-INDEX-TO-ESTORE      PIC 9(9) COMP-5.
          05 POOL-INDEX-FROM-ESTORE    PIC 9(9) COMP-5.
          05 POOL-DATA-FROM-ESTORE     PIC 9(9) COMP-5.
          05 PREFETCH-WAIT-TIME        PIC 9(9) COMP-5.

      * SQLM_DBMON_VERSION5  Server details
          05 CATALOG-NODE              PIC 9(9) COMP-5.
          05 SERVER-PLATFORM           PIC 9(4) COMP-5.
          05 CATALOG-NODE-NAME         PIC X(20) USAGE DISPLAY NATIVE.

      * SQLM_DBMON_VERSION5_2 Hash join information
          05 TOTAL-HASH-JOINS          PIC 9(9) COMP-5.
          05 TOTAL-HASH-LOOPS          PIC 9(9) COMP-5.
          05 HASH-JOIN-OVERFLOWS       PIC 9(9) COMP-5.
          05 HASH-JOIN-SMALL-OVERFLOWS PIC 9(9) COMP-5.


      **********************************************************************
      *  Event Monitor Connection Event Record
      **********************************************************************

       01 SQLM-CONN-EVENT.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 EVENT-TYPE              PIC 9(9) COMP-5.
          05 APPL-ID                 PIC X(32) USAGE DISPLAY NATIVE.
          05 SEQUENCE-NO             PIC X(4) USAGE DISPLAY NATIVE.

      *   Locking Information
          05 LOCK-WAITS              PIC 9(9) COMP-5.
          05 LOCK-WAIT-TIME          PIC 9(9) COMP-5.
          05 LOCK-ESCALS             PIC 9(9) COMP-5.
          05 X-LOCK-ESCALS           PIC 9(9) COMP-5.
          05 DEADLOCKS               PIC 9(9) COMP-5.
          05 LOCK-TIMEOUTS           PIC 9(9) COMP-5.

      *   Sorting Information
          05 TOTAL-SORTS             PIC 9(9) COMP-5.
          05 TOTAL-SORT-TIME         PIC 9(9) COMP-5.
          05 SORT-OVERFLOWS          PIC 9(9) COMP-5.

      *   Buffer Pool and I/O Information
          05 POOL-DATA-L-READS       PIC 9(9) COMP-5.
          05 POOL-DATA-P-READS       PIC 9(9) COMP-5.
          05 POOL-DATA-WRITES        PIC 9(9) COMP-5.
          05 POOL-INDEX-L-READS      PIC 9(9) COMP-5.
          05 POOL-INDEX-P-READS      PIC 9(9) COMP-5.
          05 POOL-INDEX-WRITES       PIC 9(9) COMP-5.
          05 POOL-READ-TIME          PIC 9(9) COMP-5.
          05 POOL-WRITE-TIME         PIC 9(9) COMP-5.
          05 DIRECT-READS            PIC 9(9) COMP-5.
          05 DIRECT-WRITES           PIC 9(9) COMP-5.
          05 DIRECT-READ-REQS        PIC 9(9) COMP-5.
          05 DIRECT-WRITE-REQS       PIC 9(9) COMP-5.
          05 DIRECT-READ-TIME        PIC 9(9) COMP-5.
          05 DIRECT-WRITE-TIME       PIC 9(9) COMP-5.

      *   SQL statement counts
          05 COMMIT-SQL-STMTS        PIC 9(9) COMP-5.
          05 ROLLBACK-SQL-STMTS      PIC 9(9) COMP-5.
          05 DYNAMIC-SQL-STMTS       PIC 9(9) COMP-5.
          05 STATIC-SQL-STMTS        PIC 9(9) COMP-5.
          05 FAILED-SQL-STMTS        PIC 9(9) COMP-5.
          05 SELECT-SQL-STMTS        PIC 9(9) COMP-5.
          05 DDL-SQL-STMTS           PIC 9(9) COMP-5.
          05 UID-SQL-STMTS           PIC 9(9) COMP-5.

      *   Internal counts
          05 INT-AUTO-REBINDS        PIC 9(9) COMP-5.
          05 INT-ROWS-DELETED        PIC 9(9) COMP-5.
          05 INT-ROWS-UPDATED        PIC 9(9) COMP-5.
          05 INT-ROWS-INSERTED       PIC 9(9) COMP-5.
          05 INT-ROWS-SELECTED       PIC 9(9) COMP-5.
          05 INT-COMMITS             PIC 9(9) COMP-5.
          05 INT-ROLLBACKS           PIC 9(9) COMP-5.
          05 INT-DEADLOCK-ROLLBACKS  PIC 9(9) COMP-5.

      *   Row counts
          05 ROWS-DELETED            PIC 9(9) COMP-5.
          05 ROWS-INSERTED           PIC 9(9) COMP-5.
          05 ROWS-UPDATED            PIC 9(9) COMP-5.
          05 ROWS-SELECTED           PIC 9(9) COMP-5.
          05 ROWS-READ               PIC 9(9) COMP-5.
          05 ROWS-WRITTEN            PIC 9(9) COMP-5.

      *   Binds/Precompiles
          05 BINDS-PRECOMPILES       PIC 9(9) COMP-5.

      *   Blocking Cursor Information
          05 REJ-CURS-BLK            PIC 9(9) COMP-5.
          05 ACC-CURS-BLK            PIC 9(9) COMP-5.

      *   Timestamps
          05 DISCONN-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.

      *   Agent activity
          05 USER-CPU-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 SYSTEM-CPU-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.

      *   Package Cache Information
          05 PKG-CACHE-LOOKUPS       PIC 9(9) COMP-5.
          05 PKG-CACHE-INSERTS       PIC 9(9) COMP-5.

      *   Catalog Cache Information
          05 CAT-CACHE-OVERFLOWS     PIC 9(9) COMP-5.
          05 CAT-CACHE-HEAP-FULL     PIC 9(9) COMP-5.

      * Application Section Retrieval Information
          05 APPL-SECTION-LOOKUPS    PIC 9(9) COMP-5.
          05 APPL-SECTION-INSERTS    PIC 9(9) COMP-5.

      * SQLM_DBMON_VERSION5  Buffer Pool and I/O Information
          05 PREFETCH-WAIT-TIME      PIC 9(9) COMP-5.
          05 POOL-DATA-TO-ESTORE     PIC 9(9) COMP-5.
          05 POOL-INDEX-TO-ESTORE    PIC 9(9) COMP-5.
          05 POOL-INDEX-FROM-ESTORE  PIC 9(9) COMP-5.
          05 POOL-DATA-FROM-ESTORE   PIC 9(9) COMP-5.

      * SQLM_DBMON_VERSION5
          05 AUTHORITY-LVL           PIC 9(9) COMP-5.
          05 COORD-NODE              PIC 9(9) COMP-5.
          05 APPL-PRIORITY           PIC S9(9) COMP-5.
          05 APPL-PRIORITY-TYPE      PIC 9(9) COMP-5.
          05 AGENT-ID                PIC 9(9) COMP-5.
      * SQLM_DBMON_VERSION5_2
          05 TOTAL-HASH-JOINS          PIC 9(9) COMP-5.
          05 TOTAL-HASH-LOOPS          PIC 9(9) COMP-5.
          05 HASH-JOIN-OVERFLOWS       PIC 9(9) COMP-5.
          05 HASH-JOIN-SMALL-OVERFLOWS PIC 9(9) COMP-5.
          05 CAT-CACHE-INSERTS         PIC 9(9) COMP-5.
          05 CAT-CACHE-LOOKUPS         PIC 9(9) COMP-5.

      **********************************************************************
      *  Event Monitor Table Event Record
      **********************************************************************

       01 SQLM-TABLE-EVENT.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 EVENT-TYPE              PIC 9(9) COMP-5.
          05 TABLE-NAME              PIC X(20) USAGE DISPLAY NATIVE.
          05 TABLE-SCHEMA            PIC X(20) USAGE DISPLAY NATIVE.
          05 TABLE-TYPE              PIC 9(9) COMP-5.
          05 ROWS-WRITTEN            PIC 9(9) COMP-5.
          05 ROWS-READ               PIC 9(9) COMP-5.
          05 OVERFLOW-ACCESSES       PIC 9(9) COMP-5.
          05 TABLE-EVENT-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 PAGE-REORGS             PIC 9(9) COMP-5.



      **********************************************************************
      *  Event Monitor Statment and Statment Text Event Records
      **********************************************************************

      *Statement Event
       01 SQLM-STMT-EVENT.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 EVENT-TYPE              PIC 9(9) COMP-5.
          05 APPL-ID                 PIC X(32) USAGE DISPLAY NATIVE.
          05 SEQUENCE-NO             PIC X(4) USAGE DISPLAY NATIVE.
          05 STMT-TYPE               PIC 9(9) COMP-5.
          05 OPERATION               PIC 9(9) COMP-5.
          05 FETCH-COUNT             PIC 9(9) COMP-5.
          05 SECTION-NUMBER          PIC 9(9) COMP-5.
          05 START-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 STOP-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 CURSOR-NAME             PIC X(20) USAGE DISPLAY NATIVE.
          05 CREATOR                 PIC X(20) USAGE DISPLAY NATIVE.
          05 PACKAGE-NAME            PIC X(20) USAGE DISPLAY NATIVE.
          05 USER-CPU-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 SYSTEM-CPU-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 TOTAL-SORTS             PIC 9(9) COMP-5.
          05 TOTAL-SORT-TIME         PIC 9(9) COMP-5.
          05 SORT-OVERFLOWS          PIC 9(9) COMP-5.
          05 ROWS-READ               PIC 9(9) COMP-5.
          05 ROWS-WRITTEN            PIC 9(9) COMP-5.
          05 INT-ROWS-DELETED        PIC 9(9) COMP-5.
          05 INT-ROWS-UPDATED        PIC 9(9) COMP-5.
          05 INT-ROWS-INSERTED       PIC 9(9) COMP-5.
          05 STMT-EVENT-SQLCA.
             10 SQLM-SQLCAID         PIC X(8) USAGE DISPLAY NATIVE.
             10 SQLM-SQLCABC         PIC S9(9) COMP-5.
             10 SQLM-SQLCODE         PIC S9(9) COMP-5.
             10 SQLM-SQLERRM.
                15 SQLM-SQLERRML     PIC S9(4) COMP-5.
                15 SQLM-SQLERRMC     PIC X(70) USAGE DISPLAY NATIVE.
             10 SQLM-SQLERRP         PIC X(8) USAGE DISPLAY NATIVE.
             10 SQLM-SQLERRD        OCCURS 6 TIMES PIC S9(9) COMP-5.
             10 SQLM-SQLWARN.
                15 SQLM-SQLWARN0     PIC X USAGE DISPLAY NATIVE.
                15 SQLM-SQLWARN1     PIC X USAGE DISPLAY NATIVE.
                15 SQLM-SQLWARN2     PIC X USAGE DISPLAY NATIVE.
                15 SQLM-SQLWARN3     PIC X USAGE DISPLAY NATIVE.
                15 SQLM-SQLWARN4     PIC X USAGE DISPLAY NATIVE.
                15 SQLM-SQLWARN5     PIC X USAGE DISPLAY NATIVE.
                15 SQLM-SQLWARN6     PIC X USAGE DISPLAY NATIVE.
                15 SQLM-SQLWARN7     PIC X USAGE DISPLAY NATIVE.
                15 SQLM-SQLWARN8     PIC X USAGE DISPLAY NATIVE.
                15 SQLM-SQLWARN9     PIC X USAGE DISPLAY NATIVE.
                15 SQLM-SQLWARNA     PIC X USAGE DISPLAY NATIVE.
             10 SQLM-SQLSTATE        PIC X(5) USAGE DISPLAY NATIVE.
          05 AGENT-ID                PIC 9(9) COMP-5.
          05 AGENTS-TOP              PIC 9(9) COMP-5.
          05 STMT-LENGTH             PIC 9(4) COMP-5.
          05 STMT-TEXT-OFFSET        PIC 9(4) COMP-5.

      *Statement Text Event
       01 SQLM-STMTTEXT-EVENT.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 EVENT-TYPE              PIC 9(9) COMP-5.
          05 APPL-ID                 PIC X(32) USAGE DISPLAY NATIVE.
          05 SEQUENCE-NO             PIC X(4) USAGE DISPLAY NATIVE.
          05 CREATOR                 PIC X(20) USAGE DISPLAY NATIVE.
          05 PACKAGE-NAME            PIC X(20) USAGE DISPLAY NATIVE.
          05 SECTION-NUMBER          PIC 9(9) COMP-5.
          05 STMT-LENGTH             PIC 9(9) COMP-5.
      *   Note: Variable length statment text follows. The length of the
      *         text can be found from the STMT_LENGTH field.

      *Subsection Event
       01 SQLM-SUBSECTION-EVENT.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 EVENT-TYPE              PIC 9(9) COMP-5.
          05 AGENT-ID                PIC 9(9) COMP-5.
          05 SS-NUMBER               PIC 9(4) COMP-5.
          05 SS-NODE-NUMBER          PIC 9(4) COMP-5.
          05 SS-EXEC-TIME            PIC 9(9) COMP-5.
          05 SS-USR-CPU-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 SS-SYS-CPU-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 TQ-TOT-SEND-SPILLS      PIC 9(9) COMP-5.
          05 TQ-MAX-SEND-SPILLS      PIC 9(9) COMP-5.
          05 TQ-ROWS-READ            PIC 9(9) COMP-5.
          05 TQ-ROWS-WRITTEN         PIC 9(9) COMP-5.
          05 NUM-AGENTS              PIC 9(4) COMP-5.
          05 FILLER                  PIC X(2) USAGE DISPLAY NATIVE.


      **********************************************************************
      *  Event Monitor Transaction Event Record
      **********************************************************************

       01 SQLM-XACTION-EVENT.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 EVENT-TYPE              PIC 9(9) COMP-5.
          05 APPL-ID                 PIC X(32) USAGE DISPLAY NATIVE.
          05 SEQUENCE-NO             PIC X(4) USAGE DISPLAY NATIVE.
          05 LOG-SPACE-USED          PIC 9(9) COMP-5.
          05 COMP-STATUS             PIC 9(9) COMP-5.
          05 LOCK-WAIT-TIME          PIC 9(9) COMP-5.
          05 LOCKS-HELD-TOP          PIC 9(9) COMP-5.
          05 LOCK-ESCALS             PIC 9(9) COMP-5.
          05 X-LOCK-ESCALS           PIC 9(9) COMP-5.
          05 ROWS-READ               PIC 9(9) COMP-5.
          05 ROWS-WRITTEN            PIC 9(9) COMP-5.
          05 USER-CPU-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 SYSTEM-CPU-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 PREV-STOP-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 START-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 STOP-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 AGENT-ID                PIC 9(9) COMP-5.


      **********************************************************************
      *  Event Monitor Deadlock and Deadlocked Connection Event Records
      **********************************************************************

      *Deadlock Event
       01 SQLM-DEADLOCK-EVENT.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 EVENT-TYPE              PIC 9(9) COMP-5.
          05 DL-CONNS                PIC 9(9) COMP-5.
          05 START-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 ROLLED-BACK-APPL-ID     PIC X(32) USAGE DISPLAY NATIVE.
          05 ROLLED-BACK-SEQUENCE-NO PIC X(4) USAGE DISPLAY NATIVE.
          05 ROLLED-BACK-AGENT-ID    PIC 9(9) COMP-5.


      *Deadlocked Application Event
       01 SQLM-DLCONN-EVENT.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 EVENT-TYPE              PIC 9(9) COMP-5.
          05 APPL-ID                 PIC X(32) USAGE DISPLAY NATIVE.
          05 SEQUENCE-NO             PIC X(4) USAGE DISPLAY NATIVE.
          05 APPL-ID-HOLDING-LK      PIC X(32) USAGE DISPLAY NATIVE.
          05 SEQUENCE-NO-HOLDING-LK  PIC X(4) USAGE DISPLAY NATIVE.
          05 TABLE-NAME              PIC X(20) USAGE DISPLAY NATIVE.
          05 TABLE-SCHEMA            PIC X(20) USAGE DISPLAY NATIVE.
          05 TABLESPACE-NAME         PIC X(20) USAGE DISPLAY NATIVE.
          05 LOCK-MODE               PIC 9(9) COMP-5.
          05 LOCK-OBJECT-TYPE        PIC 9(9) COMP-5.
          05 LOCK-OBJECT-NAME        PIC 9(9) COMP-5.
          05 LOCK-WAIT-START-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 START-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 LOCK-NODE               PIC 9(9) COMP-5.
          05 AGENT-ID                PIC 9(9) COMP-5.
          05 LOCK-ESCALATION         PIC X USAGE DISPLAY NATIVE.



      **********************************************************************
      *  Event Monitor Tablespace Event Record
      **********************************************************************

       01 SQLM-TABLESPACE-EVENT.
          05 SQLM-SIZE                 PIC 9(9) COMP-5.
          05 EVENT-TYPE                PIC 9(9) COMP-5.

          05 TABLESPACE-NAME           PIC X(20) USAGE DISPLAY NATIVE.
          05 POOL-DATA-L-READS         PIC 9(9) COMP-5.
          05 POOL-DATA-P-READS         PIC 9(9) COMP-5.
          05 POOL-ASYNC-DATA-READS     PIC 9(9) COMP-5.
          05 POOL-DATA-WRITES          PIC 9(9) COMP-5.
          05 POOL-ASYNC-DATA-WRITES    PIC 9(9) COMP-5.
          05 POOL-INDEX-L-READS        PIC 9(9) COMP-5.
          05 POOL-INDEX-P-READS        PIC 9(9) COMP-5.
          05 POOL-INDEX-WRITES         PIC 9(9) COMP-5.
          05 POOL-ASYNC-INDEX-WRITES   PIC 9(9) COMP-5.
          05 POOL-READ-TIME            PIC 9(9) COMP-5.
          05 POOL-WRITE-TIME           PIC 9(9) COMP-5.
          05 POOL-ASYNC-READ-TIME      PIC 9(9) COMP-5.
          05 POOL-ASYNC-WRITE-TIME     PIC 9(9) COMP-5.
          05 POOL-ASYNC-DATA-READ-REQS PIC 9(9) COMP-5.

          05 DIRECT-READS              PIC 9(9) COMP-5.
          05 DIRECT-WRITES             PIC 9(9) COMP-5.
          05 DIRECT-READ-REQS          PIC 9(9) COMP-5.
          05 DIRECT-WRITE-REQS         PIC 9(9) COMP-5.
          05 DIRECT-READ-TIME          PIC 9(9) COMP-5.
          05 DIRECT-WRITE-TIME         PIC 9(9) COMP-5.

          05 FILES-CLOSED              PIC 9(9) COMP-5.
          05 TS-EVENT-TIME.
             10 SECONDS                PIC 9(9) COMP-5.
             10 MICROSEC               PIC 9(9) COMP-5.

      * SQLM_DBMON_VERSION5  Buffer Pool and I/O Information
          05 POOL-ASYNC-INDEX-READS    PIC 9(9) COMP-5.
          05 POOL-DATA-TO-ESTORE       PIC 9(9) COMP-5.
          05 POOL-INDEX-TO-ESTORE      PIC 9(9) COMP-5.
          05 POOL-INDEX-FROM-ESTORE    PIC 9(9) COMP-5.
          05 POOL-DATA-FROM-ESTORE     PIC 9(9) COMP-5.


      **********************************************************************
      *  Event Monitor Database Header Event Record
      **********************************************************************

       01 SQLM-DBHEADER-EVENT.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 EVENT-TYPE              PIC 9(9) COMP-5.
          05 DB-NAME                 PIC X(20) USAGE DISPLAY NATIVE.
          05 DB-PATH                 PIC X(256) USAGE DISPLAY NATIVE.
          05 CONN-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.


      **********************************************************************
      *  Event Monitor Start Event Record
      **********************************************************************

       01 SQLM-EVMON-START-EVENT.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 EVENT-TYPE              PIC 9(9) COMP-5.
          05 START-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.



      **********************************************************************
      *  Event Monitor Connection Header Event Record
      **********************************************************************

       01 SQLM-CONNHEADER-EVENT.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 EVENT-TYPE              PIC 9(9) COMP-5.
          05 APPL-ID                 PIC X(32) USAGE DISPLAY NATIVE.
          05 SEQUENCE-NO             PIC X(4) USAGE DISPLAY NATIVE.
          05 CORR-TOKEN              PIC X(32) USAGE DISPLAY NATIVE.
          05 APPL-NAME               PIC X(20) USAGE DISPLAY NATIVE.
          05 AUTH-ID                 PIC X(20) USAGE DISPLAY NATIVE.
          05 EXECUTION-ID            PIC X(20) USAGE DISPLAY NATIVE.
          05 CLIENT-NNAME            PIC X(20) USAGE DISPLAY NATIVE.
          05 CLIENT-PRDID            PIC X(20) USAGE DISPLAY NATIVE.
          05 CLIENT-DB-ALIAS         PIC X(20) USAGE DISPLAY NATIVE.
          05 CLIENT-PID              PIC 9(9) COMP-5.
          05 AGENT-ID                PIC 9(9) COMP-5.
          05 CODEPAGE-ID             PIC 9(4) COMP-5.
          05 COUNTRY-CODE            PIC 9(4) COMP-5.
          05 CLIENT-PLATFORM         PIC 9(4) COMP-5.
          05 CLIENT-PROTOCOL         PIC 9(4) COMP-5.
          05 CONN-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 NODE-NUMBER             PIC 9(4) COMP-5.
          05 FILLER                  PIC X(2) USAGE DISPLAY NATIVE.


      **********************************************************************
      *  Event Monitor Overflow Event Record
      **********************************************************************

       01 SQLM-OVERFLOW-EVENT.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 EVENT-TYPE              PIC 9(9) COMP-5.
          05 OVERFLOW-COUNT          PIC 9(9) COMP-5.
          05 FIRST-OVERFLOW-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 LAST-OVERFLOW-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 NODE-NUMBER             PIC 9(4) COMP-5.
          05 FILLER                  PIC X(2) USAGE DISPLAY NATIVE.

      ******************************************************************
      *  Event Monitor Bufferpool Event Record
      ******************************************************************
       01 SQLM-BUFFERPOOL-EVENT.
          05 SQLM-SIZE               PIC 9(9) COMP-5.
          05 EVENT-TYPE              PIC 9(9) COMP-5.
          05 EVENT-TIME.
             10 SECONDS              PIC 9(9) COMP-5.
             10 MICROSEC             PIC 9(9) COMP-5.
          05 BP-NAME                 PIC X(20) USAGE DISPLAY NATIVE.
          05 DB-NAME                 PIC X(20) USAGE DISPLAY NATIVE.
          05 DB-PATH                 PIC X(256) USAGE DISPLAY NATIVE.

          05 POOL-DATA-L-READS       PIC 9(9) COMP-5.
          05 POOL-DATA-P-READS       PIC 9(9) COMP-5.
          05 POOL-DATA-WRITES        PIC 9(9) COMP-5.
          05 POOL-INDEX-L-READS      PIC 9(9) COMP-5.
          05 POOL-INDEX-P-READS      PIC 9(9) COMP-5.
          05 POOL-INDEX-WRITES       PIC 9(9) COMP-5.
          05 POOL-READ-TIME          PIC 9(9) COMP-5.
          05 POOL-WRITE-TIME         PIC 9(9) COMP-5.
          05 FILES-CLOSED            PIC 9(9) COMP-5.

          05 POOL-ASYNC-DATA-READS   PIC 9(9) COMP-5.
          05 POOL-ASYNC-DATA-WRITES  PIC 9(9) COMP-5.
          05 POOL-ASYNC-INDEX-WRITES PIC 9(9) COMP-5.
          05 POOL-ASYNC-READ-TIME    PIC 9(9) COMP-5.
          05 POOL-ASYNC-WRITE-TIME   PIC 9(9) COMP-5.
          05 POOL-ASYNC-DATA-READ-REQS PIC 9(9) COMP-5.
          05 DIRECT-READS            PIC 9(9) COMP-5.
          05 DIRECT-WRITES           PIC 9(9) COMP-5.
          05 DIRECT-READ-REQS        PIC 9(9) COMP-5.
          05 DIRECT-WRITE-REQS       PIC 9(9) COMP-5.
          05 DIRECT-READ-TIME        PIC 9(9) COMP-5.
          05 DIRECT-WRITE-TIME       PIC 9(9) COMP-5.

          05 POOL-ASYNC-INDEX-READS  PIC 9(9) COMP-5.
          05 POOL-DATA-TO-ESTORE     PIC 9(9) COMP-5.
          05 POOL-INDEX-TO-ESTORE    PIC 9(9) COMP-5.
          05 POOL-INDEX-FROM-ESTORE  PIC 9(9) COMP-5.
          05 POOL-DATA-FROM-ESTORE   PIC 9(9) COMP-5.

