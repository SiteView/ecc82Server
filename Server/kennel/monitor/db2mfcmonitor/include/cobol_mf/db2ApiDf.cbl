      *************************************************************************
      * 
      * Source File Name: db2ApiDf
      * 
      * (C) COPYRIGHT International Business Machines Corp. 1987, 1997
      * All Rights Reserved
      * Licensed Materials - Property of IBM
      * 
      * US Government Users Restricted Rights - Use, duplication or
      * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
      * 
      * Function = Include File defining:
      *              Environment Commands - Constants
      *              Environment Commands - Structures
      *              Environment Commands - Function Prototypes
      *              Environment Commands - Labels for SQLCODES
      * 
      * Operating System:  Windows NT
      * 
      **************************************************************************

      * Version, Release PTF identifier
      * Pre Version 6.1.0.0
       78  DB2VERSION6PRIOR          value    0.
      * Version 6.1.0.0
       78  DB2VERSION610             value    6010000.
      * Version 7.1.0.0
       78  DB2VERSION710             value    7010000.
      * Version 8.1.0.0
       78  DB2VERSION810             value    8010000.
      * Version 8.1.0.2
       78  DB2VERSION812             value    8010200.
      * Version 8.1.0.4
       78  DB2VERSION814             value    8010400.
      * Version 8.2.0.0
       78  DB2VERSION820             value    8020000.
      * Version 8.2.0.2
       78  DB2VERSION822             value    8020200.

      *************************************************************************
      * Structure db2Char
      * 
      * Description:
      *     pioData  -- Pointer to character data.
      *              -- If NULL, no data will be returned.
      * 
      *     iLength  -- Input:  Size of the pData buffer.
      * 
      *     oLength  -- Output: Length of valid data in pData.
      *              -- If the output data was truncated, this will be set to
      *                 the complete size before truncation
      **************************************************************************
       01 DB2CHAR.
      * Character data
           05 DB2-PIO-DATA           USAGE IS POINTER.
      * I:  Length of pioData
           05 DB2-I-LENGTH           PIC 9(9) COMP-5.
      * O:  Untruncated length of data
           05 DB2-O-LENGTH           PIC 9(9) COMP-5.



      *************************************************************************
      * Database and Database Manager Configuration
      * Structures, Constants, and Function Prototypes
      **************************************************************************

      * act on database cfg, or
       78  DB2CFG-DATABASE           VALUE    1.
      * act on database manager cfg
       78  DB2CFG-DATABASE-MANAGER   VALUE    2.
      * get/set current values, or
       78  DB2CFG-IMMEDIATE          VALUE    4.
      * get/set delayed values
       78  DB2CFG-DELAYED            VALUE    8.
      * get default values
       78  DB2CFG-GET-DEFAULTS       VALUE    64.
      * set to default values (reset)
       78  DB2CFG-RESET              VALUE    64.
      * maximum number of params in db2Cfg paramArray
       78  DB2CFG-MAX-PARAM          VALUE    64.


      *************************************************************************
      * Constants describing a single configuration parameter
      **************************************************************************
      * let db2 set this value
       78  DB2CFG-PARAM-AUTOMATIC    VALUE    1.
      * set to auto using current
       78  DB2CFG-AUTO-ONLY          VALUE    16.


      *************************************************************************
      * Structure for a single Configuration Parameter
      **************************************************************************
       01 DB2CFG-PARAM.
      * Parameter Identifier
           05 DB2-TOKEN              PIC 9(9) COMP-5.
      * Parameter value
           05 DB2-PTRVALUE           USAGE IS POINTER.
      * flags for this parameter
           05 DB2-FLAGS              PIC 9(9) COMP-5.

      *************************************************************************
      * Main structure for db2CfgSet() and db2CfgGet()
      **************************************************************************
       01 DB2CFG.
      * Number of configuration parameters in the following array
           05 DB2-NUM-ITEMS          PIC 9(9) COMP-5.
      * Array of cfg parameters
           05 DB2-PARAM-ARRAY        USAGE IS POINTER.
      * Various properties
           05 DB2-FLAGS              PIC 9(9) COMP-5.
      * Database Name, if needed
           05 DB2-DBNAME             USAGE IS POINTER.

      *************************************************************************
      * API for Setting/Reading the Configuration Parameters
      **************************************************************************
      *************************************************************************
      * Structure (generic) for a single Configuration Parameter
      **************************************************************************
       01 DB2G-CFG-PARAM.
      * Parameter Identifier
           05 DB2-TOKEN              PIC 9(9) COMP-5.
      * length of ptrvalue
           05 DB2-PTRVALUE-LEN       PIC 9(9) COMP-5.
      * Parameter value
           05 DB2-PTRVALUE           USAGE IS POINTER.
      * flags for this parameter
           05 DB2-FLAGS              PIC 9(9) COMP-5.

      *************************************************************************
      * Main (generic) structure for db2CfgSet() and db2CfgGet()
      **************************************************************************
       01 DB2G-CFG.
      * Number of configuration parameters in the following array
           05 DB2-NUM-ITEMS          PIC 9(9) COMP-5.
      * Array of cfg parameters
           05 DB2-PARAM-ARRAY        USAGE IS POINTER.
      * Various properties
           05 DB2-FLAGS              PIC 9(9) COMP-5.
      * length of dbname
           05 DB2-DBNAME-LEN         PIC 9(9) COMP-5.
      * Database Name, if needed
           05 DB2-DBNAME             USAGE IS POINTER.

      *************************************************************************
      * (Generic) API for Setting/Reading the Configuration Parameters
      **************************************************************************



      * SYSTEM table + NLS function
       78  SQL-CS-SYSTEM-NLSCHAR     VALUE    -8.
      * USER table + NLS function
       78  SQL-CS-USER-NLSCHAR       VALUE    -9.
      *************************************************************************
      * Node List Operations Constants
      **************************************************************************
      * submit to all nodes in the node list
       78  DB2-NODE-LIST             VALUE    0.
      * submit to all nodes in the node configuration file
       78  DB2-ALL-NODES             VALUE    1.
      * submit to all nodes except the ones specified by the nodelist
      * parameter
       78  DB2-ALL-EXCEPT            VALUE    2.
      * submit to catalog node only
       78  DB2-CAT-NODE-ONLY         VALUE    3.
      *************************************************************************
      * Load Query Structures and Constants
      **************************************************************************

      * Possible values for "iStringType"
      * Tablename specified
       78  DB2LOADQUERY-TABLENAME    VALUE    0.
       78  DB2LOADQUERY-TEMPPATHNAME VALUE    1.

      * Possible values for "iShowLoadMessages"
      * Show ALL LOAD messages
       78  DB2LOADQUERY-SHOW-ALL-MSGS VALUE   0.
      * Show NO LOAD messages
       78  DB2LOADQUERY-SHOW-NO-MSGS VALUE    1.
      * Show only NEW LOAD messages
       78  DB2LOADQUERY-SHOW-NEW-MSGS VALUE   2.

      * Possible values for "oWhichPhase"
      * Load Phase
       78  DB2LOADQUERY-LOAD-PHASE   VALUE    1.
      * Build Phase
       78  DB2LOADQUERY-BUILD-PHASE  VALUE    2.
      * Delete Phase
       78  DB2LOADQUERY-DELETE-PHASE VALUE    3.
      * Index Copy Phase
       78  DB2LOADQUERY-INDEXCOPY-PHASE VALUE 4.

      * Possible values for "oTablestate"
      * Normal tablestate
       78  DB2LOADQUERY-NORMAL       VALUE    0.
      * Table in check pending state
       78  DB2LOADQUERY-CHECK-PENDING VALUE   1.
      * Load in progress on table
       78  DB2LOADQUERY-LOAD-IN-PROGRESS VALUE 2.
      * Table in read access only state
       78  DB2LOADQUERY-READ-ACCESS  VALUE    4.
      * Table in unavailable state
       78  DB2LOADQUERY-NOTAVAILABLE VALUE    8.
      * Table not in load restartable state
       78  DB2LOADQUERY-NO-LOAD-RESTART VALUE 16.
      * Table state has not changed
       78  DB2LOADQUERY-UNCHANGED    VALUE    32.
      * Table pending completion of load
       78  DB2LOADQUERY-LOAD-PENDING VALUE    64.
      * Table in no data movement state
       78  DB2LOADQUERY-NO-DATA-MOVEMENT VALUE 128.
      * Table in datalink reconcile pending state
       78  DB2LOADQUERY-RECONCILE-PENDING VALUE 256.
      * Table in datalink reconcile not possible state
       78  DB2LOADQUERY-RECONCILE-NOT-POS VALUE 512.
      * Table has type-1 indexes
       78  DB2LOADQUERY-TYPE1-INDEXES VALUE   1024.

      * Load Query Output Structure
       01 DB2LOAD-QUERY-OUTPUT-STRUCT.
      * Rows Read
           05 DB2-O-ROWS-READ        PIC 9(9) COMP-5.
      * Rows Skipped
           05 DB2-O-ROWS-SKIPPED     PIC 9(9) COMP-5.
      * Rows Committed
           05 DB2-O-ROWS-COMMITTED   PIC 9(9) COMP-5.
      * Rows Loaded
           05 DB2-O-ROWS-LOADED      PIC 9(9) COMP-5.
      * Rows Rejected
           05 DB2-O-ROWS-REJECTED    PIC 9(9) COMP-5.
      * Rows Deleted
           05 DB2-O-ROWS-DELETED     PIC 9(9) COMP-5.
      * Current Index (BUILD PHASE)
           05 DB2-O-CURRENT-INDEX    PIC 9(9) COMP-5.
      * Total # of Indexes to build (BUILD PHASE)
           05 DB2-O-NUM-TOTAL-INDEXES PIC 9(9) COMP-5.
      * Node being queried (MPP only)
           05 DB2-O-CURRENT-MPPNODE  PIC 9(9) COMP-5.
      * Load Restart indicator
           05 DB2-O-LOAD-RESTARTED   PIC 9(9) COMP-5.
      * Phase of queried Load
           05 DB2-O-WHICH-PHASE      PIC 9(9) COMP-5.
      * Warning Count
           05 DB2-O-WARNING-COUNT    PIC 9(9) COMP-5.
      * Table State
           05 DB2-O-TABLE-STATE      PIC 9(9) COMP-5.

      * Generic Load Query Parameter Structure
       01 DB2G-LOAD-QUERY-STRUCT.
      * Type of piString
           05 DB2-I-STRING-TYPE      PIC 9(9) COMP-5.
      * Length in bytes of piString
           05 DB2-I-STRING-LEN       PIC 9(9) COMP-5.
      * Name to query
           05 DB2-PI-STRING          USAGE IS POINTER.
      * Level of Load message reporting
           05 DB2-I-SHOW-LOAD-MESSAGES PIC 9(9) COMP-5.
      * Load Query Output Structure
           05 DB2-PO-OUTPUT-STRUCT   USAGE IS POINTER.
      * Length in bytes of message file variable
           05 DB2-I-LOCAL-MESSAGE-FILE-LEN PIC 9(9) COMP-5.
      * Message File
           05 DB2-PI-LOCAL-MESSAGE-FILE USAGE IS POINTER.


      *************************************************************************
      * Load Structures and Constants
      **************************************************************************

      * General db2Load constants
      * Max size of db2PartLoadIn - iHostName
       78  DB2LOAD-MAX-HOSTNAME-SZ   VALUE    255.
      * Max size of db2PartLoadIn - iFileTransferCmd
       78  DB2LOAD-MAX-FILETRANSFERCMD-SZ VALUE 254.
      * Max size of Load path strings
       78  DB2LOAD-MAX-PATH-SZ       VALUE    260.

      * Load input structure
       01 DB2LOAD-IN.
      * Row count
           05 DB2-I-ROWCOUNT         PIC 9(18) COMP-5.
      * Restart count
           05 DB2-I-RESTARTCOUNT     PIC 9(18) COMP-5.
      * Alternative Tablespace to rebuild index
           05 DB2-PI-USE-TABLESPACE  USAGE IS POINTER.
      * Save count
           05 DB2-I-SAVECOUNT        PIC 9(9) COMP-5.
      * Data buffer
           05 DB2-I-DATA-BUFFER-SIZE PIC 9(9) COMP-5.
      * Sort buffer (for vendor load sort)
           05 DB2-I-SORT-BUFFER-SIZE PIC 9(9) COMP-5.
      * Warning count
           05 DB2-I-WARNINGCOUNT     PIC 9(9) COMP-5.
      * Hold quiesce between loads
           05 DB2-I-HOLD-QUIESCE     PIC 9(4) COMP-5.
      * CPU parallelism
           05 DB2-I-CPU-PARALLELISM  PIC 9(4) COMP-5.
      * Disk parallelism
           05 DB2-I-DISK-PARALLELISM PIC 9(4) COMP-5.
      * Non-recoverable load
           05 DB2-I-NONRECOVERABLE   PIC 9(4) COMP-5.
      * Indexing mode
           05 DB2-I-INDEXING-MODE    PIC 9(4) COMP-5.
      * Access Level
           05 DB2-I-ACCESS-LEVEL     PIC 9(4) COMP-5.
      * Lock With Force
           05 DB2-I-LOCK-WITH-FORCE  PIC 9(4) COMP-5.
      * Check Pending Option
           05 DB2-I-CHECK-PENDING    PIC 9(4) COMP-5.
      * Restart phase
           05 DB2-I-RESTARTPHASE     PIC X.
      * Statistics option
           05 DB2-I-STATS-OPT        PIC X.
           05 FILLER                 PIC X(2).

      * Load output structure
       01 DB2LOAD-OUT.
      * Number of rows read
           05 DB2-O-ROWS-READ        PIC 9(18) COMP-5.
      * Number of rows skipped
           05 DB2-O-ROWS-SKIPPED     PIC 9(18) COMP-5.
      * Number of rows loaded
           05 DB2-O-ROWS-LOADED      PIC 9(18) COMP-5.
      * Number of rows rejected
           05 DB2-O-ROWS-REJECTED    PIC 9(18) COMP-5.
      * Number of rows deleted
           05 DB2-O-ROWS-DELETED     PIC 9(18) COMP-5.
      * Number of rows committed
           05 DB2-O-ROWS-COMMITTED   PIC 9(18) COMP-5.

      *  
      * Partitioned Load structures and constants. These structures are only
      * needed when loading into partitioned databases
      *  

      * Load node list structure
       01 DB2LOAD-NODE-LIST.
      * Array of nodes
           05 DB2-PI-NODE-LIST       USAGE IS POINTER.
      * Number of nodes
           05 DB2-I-NUM-NODES        PIC 9(4) COMP-5.
           05 FILLER                 PIC X(2).

      * Load port range structure
       01 DB2LOAD-PORT-RANGE.
      * Minimum port number
           05 DB2-I-PORT-MIN         PIC 9(4) COMP-5.
      * Maximum port number
           05 DB2-I-PORT-MAX         PIC 9(4) COMP-5.

      * Possible values for db2PartLoadInfoIn - iMode
      * Partition and Load mode
       78  DB2LOAD-PARTITION-AND-LOAD VALUE   0.
      * Partition only mode
       78  DB2LOAD-PARTITION-ONLY    VALUE    1.
      * Load only mode
       78  DB2LOAD-LOAD-ONLY         VALUE    2.
      * Load file with no partition header
       78  DB2LOAD-LOAD-ONLY-VERIFY-PART VALUE 3.
      * Generate optimal partition map
       78  DB2LOAD-ANALYZE           VALUE    4.

      * Possible values for db2PartLoadInfoIn - iIsolatePartErrs
      * Setup errors only
       78  DB2LOAD-SETUP-ERRS-ONLY   VALUE    0.
      * Load errors only
       78  DB2LOAD-LOAD-ERRS-ONLY    VALUE    1.
      * Setup and Load errors
       78  DB2LOAD-SETUP-AND-LOAD-ERRS VALUE  2.
      * No partition error isolation
       78  DB2LOAD-NO-ISOLATION      VALUE    3.

      * Partitioned Load input structure
      *   -- assign NULL to a field to indicate default value is desired
       01 DB2PART-LOAD-IN.
      * Hostname for iFileTransferCmd parameter
           05 DB2-PI-HOSTNAME        USAGE IS POINTER.
      * File transfer command
           05 DB2-PI-FILE-TRANSFER-CMD USAGE IS POINTER.
      * Partition file location
           05 DB2-PI-PART-FILE-LOCATION USAGE IS POINTER.
      * Output nodes
           05 DB2-PI-OUTPUT-NODES    USAGE IS POINTER.
      * Partitioning nodes
           05 DB2-PI-PARTITIONING-NODES USAGE IS POINTER.
      * Partitioned Load mode
           05 DB2-PI-MODE            USAGE IS POINTER.
      * Max number of partitioning agents
           05 DB2-PI-MAX-NUM-PART-AGENTS USAGE IS POINTER.
      * Partition error isolation mode
           05 DB2-PI-ISOLATE-PART-ERRS USAGE IS POINTER.
      * Status report interval
           05 DB2-PI-STATUS-INTERVAL USAGE IS POINTER.
      * Port number range
           05 DB2-PI-PORT-RANGE      USAGE IS POINTER.
      * Check for record truncation
           05 DB2-PI-CHECK-TRUNCATION USAGE IS POINTER.
      * Map file input
           05 DB2-PI-MAP-FILE-INPUT  USAGE IS POINTER.
      * Map file output
           05 DB2-PI-MAP-FILE-OUTPUT USAGE IS POINTER.
      * Number of records to trace
           05 DB2-PI-TRACE           USAGE IS POINTER.
      * Check for newlines at end of ASC records
           05 DB2-PI-NEWLINE         USAGE IS POINTER.
      * Partition distribution output file
           05 DB2-PI-DISTFILE        USAGE IS POINTER.
      * Don't generate partition header in output file
           05 DB2-PI-OMIT-HEADER     USAGE IS POINTER.
      * runstat node
           05 DB2-PI-RUN-STAT-DBPART-NUM USAGE IS POINTER.

      * Possible values for db2LoadAgentInfo - oAgentType
      * Load agent (one per output node, not used for PARTITION_ONLY and
      * ANALYZE modes)
       78  DB2LOAD-LOAD-AGENT        VALUE    0.
      * Partitioning agent (one per partitioning node, not used for
      * LOAD_ONLY and LOAD_ONLY_VERIFY_PART modes)
       78  DB2LOAD-PARTITIONING-AGENT VALUE   1.
      * Pre-partitioning agent (one at coordinator node, not used for
      * LOAD_ONLY and LOAD_ONLY_VERIFY_PART modes)
       78  DB2LOAD-PRE-PARTITIONING-AGENT VALUE 2.
      * File transfer agent (only for FILE_TRANSFER_CMD option)
       78  DB2LOAD-FILE-TRANSFER-AGENT VALUE  3.
      * For PARTITION_ONLY mode (one per output node)
       78  DB2LOAD-LOAD-TO-FILE-AGENT VALUE   4.

      * Load agent info structure.  Load will generate an agent info
      * structure for each agent working on behalf of the Load command
       01 DB2LOAD-AGENT-INFO.
      * Agent sqlcode
           05 DB2-O-SQLCODE          PIC S9(9) COMP-5.
      * Table state (only relevant for agents of type DB2LOAD_LOAD_AGENT)
           05 DB2-O-TABLE-STATE      PIC 9(9) COMP-5.
      * Node on which agent executed
           05 DB2-O-NODE-NUM         PIC S9(4) COMP-5.
      * Agent type (see above for possible values)
           05 DB2-O-AGENT-TYPE       PIC 9(4) COMP-5.

      * Partitioned Load output structure
       01 DB2PART-LOAD-OUT.
      * Rows read by partitioning agents
           05 DB2-O-ROWS-RD-PART-AGENTS PIC 9(18) COMP-5.
      * Rows rejected by partitioning agents
           05 DB2-O-ROWS-REJ-PART-AGENTS PIC 9(18) COMP-5.
      * Rows partitioned by partitioning agents
           05 DB2-O-ROWS-PARTITIONED PIC 9(18) COMP-5.
      * Node output info list
           05 DB2-PO-AGENT-INFO-LIST USAGE IS POINTER.
      * Max number of agent info entries allocated by user for
      * poAgentInfoList. This should at least include space for the
      * partitioning agents, load agents, and one pre-partitioning agent
           05 DB2-I-MAX-AGENT-INFO-ENTRIES PIC 9(9) COMP-5.
      * Number of agent info entries produced by load.  The number of
      * entries in poAgentInfoList is min(iMaxAgentInfoEntries,
      * oNumAgentInfoEntries).
           05 DB2-O-NUM-AGENT-INFO-ENTRIES PIC 9(9) COMP-5.

      *  
      * db2Load parameter structure
      *   -- For non-partitioned database loads, set piLoadInfoIn and
      * poLoadInfoOut to NULL
      *   -- For partitioned database loads, set piLoadInfoIn to NULL to
      * request default values for all partitioned database load options
      *  
       01 DB2LOAD-STRUCT.
      * List of input source names (files, pipes, etc.)
           05 DB2-PI-SOURCE-LIST     USAGE IS POINTER.
      * Lob file paths
           05 DB2-PI-LOB-PATH-LIST   USAGE IS POINTER.
      * Data descriptor list
           05 DB2-PI-DATA-DESCRIPTOR USAGE IS POINTER.
      * Load action string
           05 DB2-PI-ACTION-STRING   USAGE IS POINTER.
      * File type (ASC, DEL, IXF, WSF, etc.)
           05 DB2-PI-FILE-TYPE       USAGE IS POINTER.
      * File type modifier string
           05 DB2-PI-FILE-TYPE-MOD   USAGE IS POINTER.
      * Message filename
           05 DB2-PI-LOCAL-MSG-FILE-NAME USAGE IS POINTER.
      * Temporary files path
           05 DB2-PI-TEMP-FILES-PATH USAGE IS POINTER.
      * Vendor Sort work directories
           05 DB2-PI-VENDOR-SORT-WORK-PATHS USAGE IS POINTER.
      * List of Load copy targets
           05 DB2-PI-COPY-TARGET-LIST USAGE IS POINTER.
      * Null indicators
           05 DB2-PI-NULL-INDICATORS USAGE IS POINTER.
      * Load input structure
           05 DB2-PI-LOAD-INFO-IN    USAGE IS POINTER.
      * Load output structure
           05 DB2-PO-LOAD-INFO-OUT   USAGE IS POINTER.
      * Partitioned Load input structure
           05 DB2-PI-PART-LOAD-INFO-IN USAGE IS POINTER.
      * Partitioned Load output structure
           05 DB2-PO-PART-LOAD-INFO-OUT USAGE IS POINTER.
      * Caller action
           05 DB2-I-CALLER-ACTION    PIC S9(4) COMP-5.
           05 FILLER                 PIC X(2).

      * db2Load - API
      *  
      * db2gLoad structures
      *  

      * Generic Load input structure
       01 DB2G-LOAD-IN.
      * Row count
           05 DB2-I-ROWCOUNT         PIC 9(18) COMP-5.
      * Restart count
           05 DB2-I-RESTARTCOUNT     PIC 9(18) COMP-5.
      * Alternative Tablespace to rebuild index
           05 DB2-PI-USE-TABLESPACE  USAGE IS POINTER.
      * Save count
           05 DB2-I-SAVECOUNT        PIC 9(9) COMP-5.
      * Data buffer
           05 DB2-I-DATA-BUFFER-SIZE PIC 9(9) COMP-5.
      * Sort buffer (for vendor load sort)
           05 DB2-I-SORT-BUFFER-SIZE PIC 9(9) COMP-5.
      * Warning count
           05 DB2-I-WARNINGCOUNT     PIC 9(9) COMP-5.
      * Hold quiesce between loads
           05 DB2-I-HOLD-QUIESCE     PIC 9(4) COMP-5.
      * CPU parallelism
           05 DB2-I-CPU-PARALLELISM  PIC 9(4) COMP-5.
      * Disk parallelism
           05 DB2-I-DISK-PARALLELISM PIC 9(4) COMP-5.
      * Non-recoverable load
           05 DB2-I-NONRECOVERABLE   PIC 9(4) COMP-5.
      * Indexing mode
           05 DB2-I-INDEXING-MODE    PIC 9(4) COMP-5.
      * Access Level
           05 DB2-I-ACCESS-LEVEL     PIC 9(4) COMP-5.
      * Lock With Force
           05 DB2-I-LOCK-WITH-FORCE  PIC 9(4) COMP-5.
      * Check Pending Option
           05 DB2-I-CHECK-PENDING    PIC 9(4) COMP-5.
      * Restart phase
           05 DB2-I-RESTARTPHASE     PIC X.
      * Statistics option
           05 DB2-I-STATS-OPT        PIC X.
      * Length of piUseTablespace string
           05 DB2-I-USE-TABLESPACE-LEN PIC 9(4) COMP-5.

      * Generic Partitioned Load input structure
       01 DB2G-PART-LOAD-IN.
      * Hostname for iFileTransferCmd parameter
           05 DB2-PI-HOSTNAME        USAGE IS POINTER.
      * File transfer command
           05 DB2-PI-FILE-TRANSFER-CMD USAGE IS POINTER.
      * Partition file location
           05 DB2-PI-PART-FILE-LOCATION USAGE IS POINTER.
      * Output nodes
           05 DB2-PI-OUTPUT-NODES    USAGE IS POINTER.
      * Partitioning nodes
           05 DB2-PI-PARTITIONING-NODES USAGE IS POINTER.
      * Partitioned Load mode
           05 DB2-PI-MODE            USAGE IS POINTER.
      * Max number of partitioning agents
           05 DB2-PI-MAX-NUM-PART-AGENTS USAGE IS POINTER.
      * Partition error isolation mode
           05 DB2-PI-ISOLATE-PART-ERRS USAGE IS POINTER.
      * Status report interval
           05 DB2-PI-STATUS-INTERVAL USAGE IS POINTER.
      * Port number range
           05 DB2-PI-PORT-RANGE      USAGE IS POINTER.
      * Check for record truncation
           05 DB2-PI-CHECK-TRUNCATION USAGE IS POINTER.
      * Map file input
           05 DB2-PI-MAP-FILE-INPUT  USAGE IS POINTER.
      * Map file output
           05 DB2-PI-MAP-FILE-OUTPUT USAGE IS POINTER.
      * Number of records to trace
           05 DB2-PI-TRACE           USAGE IS POINTER.
      * Check for newlines at end of ASC records
           05 DB2-PI-NEWLINE         USAGE IS POINTER.
      * Partition distribution output file
           05 DB2-PI-DISTFILE        USAGE IS POINTER.
      * Don't generate partition header in output file
           05 DB2-PI-OMIT-HEADER     USAGE IS POINTER.
      * Reserved parameter 1
           05 DB2-PI-RESERVED1       USAGE IS POINTER.
      * Length of iHostname string
           05 DB2-I-HOSTNAME-LEN     PIC 9(4) COMP-5.
      * Length of iFileTransferCmd string
           05 DB2-I-FILE-TRANSFER-LEN PIC 9(4) COMP-5.
      * Length of iPartFileLocation string
           05 DB2-I-PART-FILE-LOC-LEN PIC 9(4) COMP-5.
      * Length of iMapFileInput string
           05 DB2-I-MAP-FILE-INPUT-LEN PIC 9(4) COMP-5.
      * Length of iMapFileOutput string
           05 DB2-I-MAP-FILE-OUTPUT-LEN PIC 9(4) COMP-5.
      * Length of iDistfile string
           05 DB2-I-DISTFILE-LEN     PIC 9(4) COMP-5.

      * db2gLoad parameter structure
       01 DB2G-LOAD-STRUCT.
      * List of input source names (files, pipes, etc.)
           05 DB2-PI-SOURCE-LIST     USAGE IS POINTER.
      * Lob file paths
           05 DB2-PI-LOB-PATH-LIST   USAGE IS POINTER.
      * Data descriptor list
           05 DB2-PI-DATA-DESCRIPTOR USAGE IS POINTER.
      * Load action string
           05 DB2-PI-ACTION-STRING   USAGE IS POINTER.
      * File type (ASC, DEL, IXF, WSF, etc.)
           05 DB2-PI-FILE-TYPE       USAGE IS POINTER.
      * File type modifier string
           05 DB2-PI-FILE-TYPE-MOD   USAGE IS POINTER.
      * Message filename
           05 DB2-PI-LOCAL-MSG-FILE-NAME USAGE IS POINTER.
      * Temporary files path
           05 DB2-PI-TEMP-FILES-PATH USAGE IS POINTER.
      * Vendor Sort work directories
           05 DB2-PI-VENDOR-SORT-WORK-PATHS USAGE IS POINTER.
      * List of Load copy targets
           05 DB2-PI-COPY-TARGET-LIST USAGE IS POINTER.
      * Null indicators
           05 DB2-PI-NULL-INDICATORS USAGE IS POINTER.
      * Load input structure
           05 DB2-PI-LOAD-INFO-IN    USAGE IS POINTER.
      * Load output structure
           05 DB2-PO-LOAD-INFO-OUT   USAGE IS POINTER.
      * Partitioned Load input structure
           05 DB2-PI-PART-LOAD-INFO-IN USAGE IS POINTER.
      * Partitioned Load output structure
           05 DB2-PO-PART-LOAD-INFO-OUT USAGE IS POINTER.
      * Caller action
           05 DB2-I-CALLER-ACTION    PIC S9(4) COMP-5.
      * Length of iFileType string
           05 DB2-I-FILE-TYPE-LEN    PIC 9(4) COMP-5.
      * Length of iLocalMsgFileName string
           05 DB2-I-LOCAL-MSG-FILE-LEN PIC 9(4) COMP-5.
      * Length of iTempFilesPath string
           05 DB2-I-TEMP-FILES-PATH-LEN PIC 9(4) COMP-5.

      * db2Load - Generic API

      *************************************************************************
      * Import Structures and Constants
      **************************************************************************

      * Respect db cfg LOCKTIMEOUT
       78  DB2IMPORT-LOCKTIMEOUT     VALUE    0.
      * Override db cfg LOCKTIMEOUT
       78  DB2IMPORT-NO-LOCKTIMEOUT  VALUE    1.
      * Use automatic commitcount
       78  DB2IMPORT-COMMIT-AUTO     VALUE    -1.

      * Generic Import input structure
       01 DB2G-IMPORT-IN.
      * Row count
           05 DB2-I-ROWCOUNT         PIC 9(18) COMP-5.
      * Restart count
           05 DB2-I-RESTARTCOUNT     PIC 9(18) COMP-5.
      * Skip count
           05 DB2-I-SKIPCOUNT        PIC 9(18) COMP-5.
      * Commit count
           05 DB2-PI-COMMITCOUNT     USAGE IS POINTER.
      * Warning count
           05 DB2-I-WARNINGCOUNT     PIC 9(9) COMP-5.
      * No lock timeout
           05 DB2-I-NO-TIMEOUT       PIC 9(4) COMP-5.
      * Access level
           05 DB2-I-ACCESS-LEVEL     PIC 9(4) COMP-5.

      * Generic Import output structure
       01 DB2G-IMPORT-OUT.
      * Rows read
           05 DB2-O-ROWS-READ        PIC 9(18) COMP-5.
      * Rows skipped
           05 DB2-O-ROWS-SKIPPED     PIC 9(18) COMP-5.
      * Rows inserted
           05 DB2-O-ROWS-INSERTED    PIC 9(18) COMP-5.
      * Rows updated
           05 DB2-O-ROWS-UPDATED     PIC 9(18) COMP-5.
      * Rows rejected
           05 DB2-O-ROWS-REJECTED    PIC 9(18) COMP-5.
      * Rows committed
           05 DB2-O-ROWS-COMMITTED   PIC 9(18) COMP-5.

      * db2gImport parameter structure
       01 DB2G-IMPORT-STRUCT.
      * Data file name
           05 DB2-PI-DATA-FILE-NAME  USAGE IS POINTER.
      * Lob file paths
           05 DB2-PI-LOB-PATH-LIST   USAGE IS POINTER.
      * Data descriptor list
           05 DB2-PI-DATA-DESCRIPTOR USAGE IS POINTER.
      * Import action string
           05 DB2-PI-ACTION-STRING   USAGE IS POINTER.
      * File type (ASC, DEL, IXF, WSF, etc.)
           05 DB2-PI-FILE-TYPE       USAGE IS POINTER.
      * File type modifier string
           05 DB2-PI-FILE-TYPE-MOD   USAGE IS POINTER.
      * Message filename
           05 DB2-PI-MSG-FILE-NAME   USAGE IS POINTER.
      * Caller action
           05 DB2-I-CALLER-ACTION    PIC S9(4) COMP-5.
           05 FILLER                 PIC X(2).
      * Import input structure
           05 DB2-PI-IMPORT-INFO-IN  USAGE IS POINTER.
      * Import output structure
           05 DB2-PO-IMPORT-INFO-OUT USAGE IS POINTER.
      * Null indicators
           05 DB2-PI-NULL-INDICATORS USAGE IS POINTER.
      * Length of iDataFileName string
           05 DB2-I-DATA-FILE-NAME-LEN PIC 9(4) COMP-5.
      * Length of iFileType string
           05 DB2-I-FILE-TYPE-LEN    PIC 9(4) COMP-5.
      * Length of iMsgFileName string
           05 DB2-I-MSG-FILE-NAME-LEN PIC 9(4) COMP-5.
           05 FILLER                 PIC X(2).

      * db2gImport - Generic API

      *************************************************************************
      * Export Structures and Constants
      **************************************************************************

      * Export output structure
       01 DB2EXPORT-OUT.
      * Rows Exported
           05 DB2-O-ROWS-EXPORTED    PIC 9(18) COMP-5.

      * db2gExport parameter structure
       01 DB2G-EXPORT-STRUCT.
      * Data file name
           05 DB2-PI-DATA-FILE-NAME  USAGE IS POINTER.
      * Lob file paths
           05 DB2-PI-LOB-PATH-LIST   USAGE IS POINTER.
      * Lob file files
           05 DB2-PI-LOB-FILE-LIST   USAGE IS POINTER.
      * Data descriptor list
           05 DB2-PI-DATA-DESCRIPTOR USAGE IS POINTER.
      * Export action string
           05 DB2-PI-ACTION-STRING   USAGE IS POINTER.
      * File type (ASC, DEL, IXF, WSF, etc.)
           05 DB2-PI-FILE-TYPE       USAGE IS POINTER.
      * File type modifier string
           05 DB2-PI-FILE-TYPE-MOD   USAGE IS POINTER.
      * Message filename
           05 DB2-PI-MSG-FILE-NAME   USAGE IS POINTER.
      * Caller action
           05 DB2-I-CALLER-ACTION    PIC S9(4) COMP-5.
           05 FILLER                 PIC X(2).
      * Export output structure
           05 DB2-PO-EXPORT-INFO-OUT USAGE IS POINTER.
      * Length of piDataFileName string
           05 DB2-I-DATA-FILE-NAME-LEN PIC 9(4) COMP-5.
      * Length of piFileType string
           05 DB2-I-FILE-TYPE-LEN    PIC 9(4) COMP-5.
      * Length of piMsgFileName string
           05 DB2-I-MSG-FILE-NAME-LEN PIC 9(4) COMP-5.
           05 FILLER                 PIC X(2).

      * db2gExport - Generic API

      *************************************************************************
      * Open History Scan Definitions
      **************************************************************************

      * Values for db2HistoryOpen - iCallerAction
       78  DB2HISTORY-LIST-HISTORY   VALUE    0.
       78  DB2HISTORY-LIST-BACKUP    VALUE    1.
       78  DB2HISTORY-LIST-ROLLFORWARD VALUE  2.
       78  DB2HISTORY-LIST-REORG     VALUE    4.
       78  DB2HISTORY-LIST-ALT-TABLESPACE VALUE 5.
       78  DB2HISTORY-LIST-DROPPED-TABLE VALUE 6.
       78  DB2HISTORY-LIST-LOAD      VALUE    7.
       78  DB2HISTORY-LIST-REN-TABLESPACE VALUE 8.
       78  DB2HISTORY-LIST-CRT-TABLESPACE VALUE 9.
       78  DB2HISTORY-LIST-ARCHIVE-LOG VALUE  10.

      * db2gHistoryOpen input struct
       01 DB2G-HISTORY-OPEN-STRUCT.
      * DB to fetch history from
           05 DB2-PI-DATABASE-ALIAS  USAGE IS POINTER.
      * Since this timestamp
           05 DB2-PI-TIMESTAMP       USAGE IS POINTER.
      * Entries containing this object
           05 DB2-PI-OBJECT-NAME     USAGE IS POINTER.
      * Length of database alias string
           05 DB2-I-ALIAS-LEN        PIC 9(9) COMP-5 VALUE 0.
      * Length of timestamp string
           05 DB2-I-TIMESTAMP-LEN    PIC 9(9) COMP-5 VALUE 0.
      * Length of object name string
           05 DB2-I-OBJECT-NAME-LEN  PIC 9(9) COMP-5 VALUE 0.
      * # of entries matching search
           05 DB2-O-NUM-ROWS         PIC 9(9) COMP-5.
      * Max. # of tablespace names stored with ANY history entry
           05 DB2-O-MAX-TBSPACES     PIC 9(9) COMP-5.
      * Caller action
           05 DB2-I-CALLER-ACTION    PIC 9(4) COMP-5.
      * Handle for this scan
           05 DB2-O-HANDLE           PIC 9(4) COMP-5.

      * db2gHistoryOpenScan - Generic API

      *************************************************************************
      * Fetch Next History Entry Definitions
      **************************************************************************

      * Structure db2HistoryEID
       01 DB2HISTORY-EID.
      * Node number
           05 DB2-IO-NODE            PIC S9(4) COMP-5.
           05 FILLER                 PIC X(2).
      * Local history file entry ID
           05 DB2-IO-HID             PIC 9(9) COMP-5.

      * Max Sizes of db2HistoryDataStruct db2Char fields
      * NOTE:  These lengths do not include NULL terminators
       78  DB2HISTORY-TIMESTAMP-SZ   VALUE    14.
       78  DB2HISTORY-SEQNUM-SZ      VALUE    3.
       78  DB2HISTORY-OBJPART-SZ     value    17.
       78  DB2HISTORY-LOGFILE-SZ     VALUE    12.
       78  DB2HISTORY-ID-SZ          VALUE    24.
       78  DB2HISTORY-TABLE-QUAL-SZ  VALUE    30.
       78  DB2HISTORY-TABLE-NAME-SZ  VALUE    128.
       78  DB2HISTORY-LOCATION-SZ    VALUE    255.
       78  DB2HISTORY-COMMENT-SZ     VALUE    30.
       78  DB2HISTORY-COMMAND-SZ     value    2097152.

      * Structure db2HistoryData
      * 
      * Description:
      *      ioHistDataID      -- Eye catcher, must be set to "SQLUHINF"
      * 
      *      oObjectPart       -- Start time + sequence number
      *                        -- DB2HISTORY_OBJPART_SZ
      * 
      *      oEndTime          -- Completion timestamp
      *                        -- DB2HISTORY_TIMESTAMP_SZ
      * 
      *      oFirstLog         -- First log file referrenced by event
      *                        -- DB2HISTORY_LOGFILE_SZ
      * 
      *      oLastLog          -- Last log file referrenced by event
      *                        -- DB2HISTORY_LOGFILE_SZ
      * 
      *      oID               -- Backup ID (timestamp), or unique table ID
      *                        -- DB2HISTORY_ID_SZ
      * 
      *      oTableQualifier   -- Table qualifier
      *                        -- DB2HISTORY_TABLE_QUAL_SZ
      * 
      *      oTableName        -- Table name
      *                        -- DB2HISTORY_TABLE_NAME_SZ
      * 
      *      oLocation         -- Location of object used or produced by
      * event
      *                        -- DB2HISTORY_LOCATION_SZ
      * 
      *      oComment          -- Text comment
      *                        -- DB2HISTORY_COMMENT_SZ
      * 
      *      oCommandText      -- Command text, or DDL
      *                        -- DB2HISTORY_COMMAND_SZ
      * 
      *      oLastLSN          -- Last Log Sequence Number
      *      poEventSQLCA      -- SQLCA returned at event completion
      *      poTablespace      -- LIST of tablespace names
      *      iNumTablespaces   -- # of available entries in the poTablespace
      * list
      *      oNumTablespaces   -- # of used entries in the poTablespace list
      *      oEID              -- Unique history entry ID
      *      oOperation        -- Event type:        DB2HISTORY_TYPE_*
      *      oObject           -- Event granularity: DB2HISTORY_GRAN_*
      *      oOptype           -- Event details:     DB2HISTORY_OPTYPE_*
      *      oStatus           -- Entry status:      DB2HISTORY_STATUS_*
      *      oDeviceType       -- Type of oLocation: DB2_MEDIA_*

      * db2HistoryData structure.
       01 DB2HISTORY-DATA.
           05 DB2-IO-HIST-DATA-ID    PIC X(8).
           05 DB2-O-OBJECT-PART.
               10 DB2-PIO-DATA       USAGE IS POINTER.
               10 DB2-I-LENGTH       PIC 9(9) COMP-5 VALUE 0.
               10 DB2-O-LENGTH       PIC 9(9) COMP-5 VALUE 0.
           05 DB2-O-END-TIME.
               10 DB2-PIO-DATA       USAGE IS POINTER.
               10 DB2-I-LENGTH       PIC 9(9) COMP-5 VALUE 0.
               10 DB2-O-LENGTH       PIC 9(9) COMP-5 VALUE 0.
           05 DB2-O-FIRST-LOG.
               10 DB2-PIO-DATA       USAGE IS POINTER.
               10 DB2-I-LENGTH       PIC 9(9) COMP-5 VALUE 0.
               10 DB2-O-LENGTH       PIC 9(9) COMP-5 VALUE 0.
           05 DB2-O-LAST-LOG.
               10 DB2-PIO-DATA       USAGE IS POINTER.
               10 DB2-I-LENGTH       PIC 9(9) COMP-5 VALUE 0.
               10 DB2-O-LENGTH       PIC 9(9) COMP-5 VALUE 0.
           05 DB2-O-ID.
               10 DB2-PIO-DATA       USAGE IS POINTER.
               10 DB2-I-LENGTH       PIC 9(9) COMP-5 VALUE 0.
               10 DB2-O-LENGTH       PIC 9(9) COMP-5 VALUE 0.
           05 DB2-O-TABLE-QUALIFIER.
               10 DB2-PIO-DATA       USAGE IS POINTER.
               10 DB2-I-LENGTH       PIC 9(9) COMP-5 VALUE 0.
               10 DB2-O-LENGTH       PIC 9(9) COMP-5 VALUE 0.
           05 DB2-O-TABLE-NAME.
               10 DB2-PIO-DATA       USAGE IS POINTER.
               10 DB2-I-LENGTH       PIC 9(9) COMP-5 VALUE 0.
               10 DB2-O-LENGTH       PIC 9(9) COMP-5 VALUE 0.
           05 DB2-O-LOCATION.
               10 DB2-PIO-DATA       USAGE IS POINTER.
               10 DB2-I-LENGTH       PIC 9(9) COMP-5 VALUE 0.
               10 DB2-O-LENGTH       PIC 9(9) COMP-5 VALUE 0.
           05 DB2-O-COMMENT.
               10 DB2-PIO-DATA       USAGE IS POINTER.
               10 DB2-I-LENGTH       PIC 9(9) COMP-5 VALUE 0.
               10 DB2-O-LENGTH       PIC 9(9) COMP-5 VALUE 0.
           05 DB2-O-COMMAND-TEXT.
               10 DB2-PIO-DATA       USAGE IS POINTER.
               10 DB2-I-LENGTH       PIC 9(9) COMP-5 VALUE 0.
               10 DB2-O-LENGTH       PIC 9(9) COMP-5 VALUE 0.
      * Structure SQLU_LSN
           05 DB2-O-LAST-LSN.
               10 SQL-LSN-CHAR       PIC X(6).
               10 SQL-LSN-WORD       REDEFINES SQL-LSN-CHAR
                                     PIC 9(4) COMP-5 OCCURS 3 TIMES.
               10 FILLER             REDEFINES SQL-LSN-CHAR
                                     PIC X(6).
               10 FILLER             PIC X(2).
      * Structure db2HistoryEID
           05 DB2-O-EID.
               10 DB2-IO-NODE        PIC S9(4) COMP-5 VALUE 0.
               10 FILLER             PIC X(2).
               10 DB2-IO-HID         PIC 9(9) COMP-5 VALUE 0.
      * Result SQLCA from event
           05 DB2-PO-EVENT-SQLCA     USAGE IS POINTER.
           05 DB2-PO-TABLESPACE      USAGE IS POINTER.
           05 DB2-I-NUM-TABLESPACES  PIC 9(9) COMP-5 VALUE 0.
           05 DB2-O-NUM-TABLESPACES  PIC 9(9) COMP-5 VALUE 0.
           05 DB2-O-OPERATION        PIC X.
           05 DB2-O-OBJECT           PIC X.
           05 DB2-O-OPTYPE           PIC X.
           05 DB2-O-STATUS           PIC X.
           05 DB2-O-DEVICE-TYPE      PIC X.
           05 FILLER                 PIC X(3).

      * Values for db2HistoryGetEntry - iCallerAction
      * Next entry, without command data
       78  DB2HISTORY-GET-ENTRY      VALUE    0.
      * ONLY command data from previous fetch
       78  DB2HISTORY-GET-DDL        VALUE    1.
      * Next entry, with all data
       78  DB2HISTORY-GET-ALL        VALUE    2.

      * db2HistoryGetEntry input struct
       01 DB2HISTORY-GET-ENTRY-STRUCT.
      * History entry data area
           05 DB2-PIO-HIST-DATA      USAGE IS POINTER.
      * History scan handle
           05 DB2-I-HANDLE           PIC 9(4) COMP-5.
      * Caller action
           05 DB2-I-CALLER-ACTION    PIC 9(4) COMP-5.

      * db2gHistoryGetEntry - Generic API

      * History Operations
       78  DB2HIST-OP-CRT-TABLESPACE value    "A".
       78  DB2HIST-OP-BACKUP         value    "B".
       78  DB2HIST-OP-LOAD-COPY      value    "C".
       78  DB2HIST-OP-DROPPED-TABLE  value    "D".
       78  DB2HIST-OP-ROLLFWD        value    "F".
       78  DB2HIST-OP-REORG          value    "G".
       78  DB2HIST-OP-LOAD           value    "L".
       78  DB2HIST-OP-REN-TABLESPACE value    "N".
       78  DB2HIST-OP-DROP-TABLESPACE value   "O".
       78  DB2HIST-OP-QUIESCE        value    "Q".
       78  DB2HIST-OP-RESTORE        value    "R".
       78  DB2HIST-OP-ALT-TBS        value    "T".
       78  DB2HIST-OP-UNLOAD         value    "U".
       78  DB2HIST-OP-ARCHIVE-LOG    value    "X".

      * History Objects
       78  DB2HISTORY-GRAN-DB        value    "D".
       78  DB2HISTORY-GRAN-TBS       value    "P".
       78  DB2HISTORY-GRAN-TABLE     value    "T".
       78  DB2HISTORY-GRAN-INDEX     value    "I".

      * Backup Optypes
       78  DB2HISTORY-OPTYPE-OFFLINE value    "F".
       78  DB2HISTORY-OPTYPE-ONLINE  value    "N".
       78  DB2HISTORY-OPTYPE-INCR-OFFLINE value "I".
       78  DB2HISTORY-OPTYPE-INCR-ONLINE value "O".
       78  DB2HISTORY-OPTYPE-DELT-OFFLINE value "D".
       78  DB2HISTORY-OPTYPE-DELT-ONLINE value "E".

      * Quiesce Optypes
       78  DB2HISTORY-OPTYPE-SHARE   value    "S".
       78  DB2HISTORY-OPTYPE-UPDATE  value    "U".
       78  DB2HISTORY-OPTYPE-EXCL    value    "X".
       78  DB2HISTORY-OPTYPE-RESET   value    "Z".

      * Rollforward Optypes
       78  DB2HISTORY-OPTYPE-EOL     value    "E".
       78  DB2HISTORY-OPTYPE-PIT     value    "P".

      * Load Optypes
       78  DB2HISTORY-OPTYPE-INSERT  value    "I".
       78  DB2HISTORY-OPTYPE-REPLACE value    "R".

      * Alter Tablespace Optypes
       78  DB2HISTORY-OPTYPE-ADD-CONT value   "C".
       78  DB2HISTORY-OPTYPE-REB     value    "R".

      * Archive Log Optypes
       78  DB2HISTORY-OPTYPE-PRIMARY value    "P".
       78  DB2HISTORY-OPTYPE-MIRROR  value    "M".
       78  DB2HISTORY-OPTYPE-ARCHFAIL value   "F".
       78  DB2HISTORY-OPTYPE-ARCH1   value    "1".
       78  DB2HISTORY-OPTYPE-ARCH2   value    "2".

      * History Entry Status Flags
       78  DB2HISTORY-STATUS-ACTIVE  value    "A".
       78  DB2HISTORY-STATUS-INACTIVE value   "I".
       78  DB2HISTORY-STATUS-EXPIRED value    "E".
       78  DB2HISTORY-STATUS-DELETED value    "D".
       78  DB2HISTORY-STATUS-NC      value    "N".
       78  DB2HISTORY-STATUS-INCMP-ACTV value "a".
       78  DB2HISTORY-STATUS-INCMP-INACTV value "i".


      * db2gHistoryCloseScan - Generic API

      *************************************************************************
      * Update History Entry Definitions
      **************************************************************************

      * Update history API input struct
       01 DB2G-HISTORY-UPDATE-STRUCT.
           05 DB2-PI-NEW-LOCATION    USAGE IS POINTER.
           05 DB2-PI-NEW-DEVICE-TYPE USAGE IS POINTER.
           05 DB2-PI-NEW-COMMENT     USAGE IS POINTER.
           05 DB2-PI-NEW-STATUS      USAGE IS POINTER.
           05 DB2-I-NEW-LOCATION-LEN PIC 9(9) COMP-5 VALUE 0.
           05 DB2-I-NEW-DEVICE-LEN   PIC 9(9) COMP-5 VALUE 0.
           05 DB2-I-NEW-COMMENT-LEN  PIC 9(9) COMP-5 VALUE 0.
           05 DB2-I-NEW-STATUS-LEN   PIC 9(9) COMP-5 VALUE 0.
      * ID of the entry to be updated
           05 DB2-I-EID.
      * Node number
               10 DB2-IO-NODE        PIC S9(4) COMP-5.
               10 FILLER             PIC X(2).
      * Local history file entry ID
               10 DB2-IO-HID         PIC 9(9) COMP-5.

      * db2gHistoryUpdate - Generic API

      *************************************************************************
      * Precompiler API Related Definitions
      **************************************************************************

      * Generic Compile SQL Parmeter Structure
       01 DB2G-COMPILE-SQL-STRUCT.
      * SQL statement text length
           05 DB2-PI-SQL-STMT-LEN    USAGE IS POINTER.
      * SQL statement text
           05 DB2-PI-SQL-STMT        USAGE IS POINTER.
      * Source Line Number
           05 DB2-PI-LINE-NUM        USAGE IS POINTER.
      * SQL Flagger information
           05 DB2-PIO-FLAG-INFO      USAGE IS POINTER.
      * Token ID Array
           05 DB2-PIO-TOKEN-ID-ARRAY USAGE IS POINTER.
      * Task Array
           05 DB2-PO-TASK-ARRAY      USAGE IS POINTER.
      * Section Number
           05 DB2-PO-SECTION-NUM     USAGE IS POINTER.
      * Type of SQL statement
           05 DB2-PO-SQL-STMT-TYPE   USAGE IS POINTER.
      * 256 byte string buffer 1
           05 DB2-PO-BUFFER1         USAGE IS POINTER.
      * 256 byte string buffer 2
           05 DB2-PO-BUFFER2         USAGE IS POINTER.
      * 256 byte string buffer 3
           05 DB2-PO-BUFFER3         USAGE IS POINTER.
      * Reserved
           05 DB2-PIO-RESERVED       USAGE IS POINTER.

      * Generic Initialization Parmeter Structure
       01 DB2G-INIT-STRUCT.
      * Program name length
           05 DB2-PI-PROGRAM-NAME-LENGTH USAGE IS POINTER.
      * Program name
           05 DB2-PI-PROGRAM-NAME    USAGE IS POINTER.
      * Database name length
           05 DB2-PI-DB-NAME-LENGTH  USAGE IS POINTER.
      * Database name
           05 DB2-PI-DB-NAME         USAGE IS POINTER.
      * Password length
           05 DB2-PI-DB-PASSWORD-LENGTH USAGE IS POINTER.
      * Password
           05 DB2-PI-DB-PASSWORD     USAGE IS POINTER.
      * Bindfile name length
           05 DB2-PI-BIND-NAME-LENGTH USAGE IS POINTER.
      * Bindfile name
           05 DB2-PI-BIND-NAME       USAGE IS POINTER.
      * Precompiler options array
           05 DB2-PI-OPTIONS-ARRAY   USAGE IS POINTER.
      * Precompiler program ID buffer length
           05 DB2-PI-PID-LENGTH      USAGE IS POINTER.
      * Precompiler program ID
           05 DB2-PO-PRECOMPILER-PID USAGE IS POINTER.

      *************************************************************************
      * Reorg Definitions
      **************************************************************************

      * Reorg type defines
      * Define for reorg table offline
       78  DB2REORG-OBJ-TABLE-OFFLINE VALUE   1.
      * Define for reorg table inplace
       78  DB2REORG-OBJ-TABLE-INPLACE VALUE   2.
      * Define for reorg indexes all
       78  DB2REORG-OBJ-INDEXESALL   VALUE    4.

      * Defines for reorg flags parameter
      * Take default action
       78  DB2REORG-OPTION-NONE      VALUE    0.

      * Offline Reorg Options
      * Reorg long fields and lobs
       78  DB2REORG-LONGLOB          VALUE    1.
      * System to choose temp tablespace for reorg work
       78  DB2REORG-CHOOSE-TEMP      VALUE    2.
      * Recluster utilizing index scan
       78  DB2REORG-INDEXSCAN        VALUE    4.
      * No read or write access to table
       78  DB2REORG-ALLOW-NONE       VALUE    8.

      * Online Reorg Options
      * Start online reorg
       78  DB2REORG-START-ONLINE     VALUE    65536.
      * Pause an existing online reorg
       78  DB2REORG-PAUSE-ONLINE     VALUE    131072.
      * Stop an existing online reorg
       78  DB2REORG-STOP-ONLINE      VALUE    262144.
      * Resume a paused online reorg
       78  DB2REORG-RESUME-ONLINE    VALUE    524288.
      * Do not perform table truncation
       78  DB2REORG-NOTRUNCATE-ONLINE VALUE   1048576.
      * Read and write access to table
       78  DB2REORG-ALLOW-WRITE      VALUE    268435456.
      * Allow only read access to table
       78  DB2REORG-ALLOW-READ       VALUE    536870912.

      * Online Reorg Indexes All Options
      * Reorg type1 index only
       78  DB2REORG-REORG-TYPE1      VALUE    2097152.
      * No cleanup is required
       78  DB2REORG-CLEANUP-NONE     VALUE    4194304.
      * Cleanup pages and keys
       78  DB2REORG-CLEANUP-ALL      VALUE    8388608.
      * Cleanup pages only
       78  DB2REORG-CLEANUP-PAGES    VALUE    16777216.
      * No convert is required
       78  DB2REORG-CONVERT-NONE     VALUE    33554432.
      * Convert to type2
       78  DB2REORG-CONVERT          VALUE    67108864.
      * Convert to type1
       78  DB2REORG-CONVERT-TYPE1    VALUE    134217728.

      * MPP allNodeFlag values
       78  DB2REORG-NODE-LIST        value    0.
       78  DB2REORG-ALL-NODES        value    1.
       78  DB2REORG-ALL-EXCEPT       value    2.

      * Generic Reorg Table API input struct
       01 DB2G-REORG-TABLE.
      * length in bytes of pTableName
           05 DB2-TABLE-NAME-LEN     PIC 9(9) COMP-5.
      * Name of table to reorganize
           05 DB2-P-TABLE-NAME       USAGE IS POINTER.
      * length in bytes of pOrderByIndex
           05 DB2-ORDER-BY-INDEX-LEN PIC 9(9) COMP-5.
      * Index to order table by
           05 DB2-P-ORDER-BY-INDEX   USAGE IS POINTER.
      * length in bytes of pSysTempSpace
           05 DB2-SYS-TEMP-SPACE-LEN PIC 9(9) COMP-5.
      * Tablespace to create temp objects in
           05 DB2-P-SYS-TEMP-SPACE   USAGE IS POINTER.


      * Reorg Indexes all input struct
       01 DB2G-REORG-INDEXES-ALL.
      * length in bytes of pTableName
           05 DB2-TABLE-NAME-LEN     PIC 9(9) COMP-5.
      * Name of table to reorganize indexes on
           05 DB2-P-TABLE-NAME       USAGE IS POINTER.


      * Reorg nodelist array
       01 DB2G-REORG-NODES.
      * Node number
           05 DB2-NODE-NUM           PIC S9(4) COMP-5 OCCURS 1000 TIMES.


      * Reorg object struct
       01 DB2G-REORG-STRUCT.
      * Type - Table or Indexes
           05 DB2-REORG-TYPE         PIC 9(9) COMP-5.
      * Reorg options - DB2REORG_xxxx
           05 DB2-REORG-FLAGS        PIC 9(9) COMP-5.
      * Which nodes reorg applies to
           05 DB2-NODE-LIST-FLAG     PIC S9(9) COMP-5.
      * Number of nodes in nodelist array
           05 DB2-NUM-NODES          PIC 9(9) COMP-5.
      * Pointer to array of node numbers
           05 DB2-P-NODE-LIST        USAGE IS POINTER.
      * Table or Index struct
           05 DB2-REORG-OBJECT.
      * Table struct
               10 DB2-TABLE-STRUCT.
      * length in bytes of pTableName
                   15 DB2-TABLE-NAME-LEN PIC 9(9) COMP-5.
      * Name of table to reorganize
                   15 DB2-P-TABLE-NAME USAGE IS POINTER.
      * length in bytes of pOrderByIndex
                   15 DB2-ORDER-BY-INDEX-LEN PIC 9(9) COMP-5.
      * Index to order table by
                   15 DB2-P-ORDER-BY-INDEX USAGE IS POINTER.
      * length in bytes of pSysTempSpace
                   15 DB2-SYS-TEMP-SPACE-LEN PIC 9(9) COMP-5.
      * Tablespace to create temp objects in
                   15 DB2-P-SYS-TEMP-SPACE USAGE IS POINTER.
      * Indexes struct
               10 DB2-INDEXES-ALL-STRUCT REDEFINES DB2-TABLE-STRUCT.
      * length in bytes of pTableName
                   15 DB2-TABLE-NAME-LEN PIC 9(9) COMP-5.
      * Name of table to reorganize indexes on
                   15 DB2-P-TABLE-NAME USAGE IS POINTER.

      * Generic Reorg Table API


      *************************************************************************
      * db2Runstats Definitions
      **************************************************************************


      * Options for iRunstatsFlags
      * Gather stats on all columns
       78  DB2RUNSTATS-ALL-COLUMNS   VALUE    1.
      * Gather stats on key columns
       78  DB2RUNSTATS-KEY-COLUMNS   VALUE    2.
      * Gather stats on all indexes
       78  DB2RUNSTATS-ALL-INDEXES   VALUE    4.
      * Gather distribution stats on either all columns or key columns
       78  DB2RUNSTATS-DISTRIBUTION  VALUE    8.
      * Gather extended index stats
       78  DB2RUNSTATS-EXT-INDEX     VALUE    16.
      * Gather sampled extended index stats
       78  DB2RUNSTATS-EXT-INDEX-SAMPLED VALUE 32.
      * Gather stats using profile
       78  DB2RUNSTATS-USE-PROFILE   VALUE    64.
      * Gather stats and set the profile
       78  DB2RUNSTATS-SET-PROFILE   VALUE    128.
      * Set the stats profile only
       78  DB2RUNSTATS-SET-PROFILE-ONLY VALUE 256.
      * Allow others to only read table while Runstats is in progress
       78  DB2RUNSTATS-ALLOW-READ    VALUE    512.
      * Gather stats on all DB partitions
       78  DB2RUNSTATS-ALL-DBPARTITIONS VALUE 1024.
      * Gather statistics and update statistics profile
       78  DB2RUNSTATS-UPDATE-PROFILE VALUE   2048.
      * Update statistics profile without gathering statistics
       78  DB2RUNSTATS-UPDA-PROFILE-ONLY VALUE 4096.
      * Page Level Sampling
       78  DB2RUNSTATS-SAMPLING-SYSTEM VALUE  8192.
      * Generate Repeatable Sample
       78  DB2RUNSTATS-SAMPLING-REPEAT VALUE  16384.

      * Options for iColumnFlags
      * Gather stats for LIKE predicates
       78  DB2RUNSTATS-COLUMN-LIKE-STATS VALUE 1.

      * Default options
      * Default level of parallelism
       78  DB2RUNSTATS-PARALLELISM-DFLT VALUE 0.
      * Default percentage of pages sampled (100%)
       78  DB2RUNSTATS-SAMPLING-DFLT value    1.000000e+02.


      *  Structures for db2gRunstats API

      * Columns to Collect Stats on
       01 DB2G-COLUMN-DATA.
           05 DB2-PI-COLUMN-NAME     USAGE IS POINTER.
           05 DB2-I-COLUMN-NAME-LEN  PIC 9(4) COMP-5.
           05 DB2-I-COLUMN-FLAGS     PIC S9(4) COMP-5.

      * Columns to Collect Distribution Stats on
       01 DB2G-COLUMN-DIST-DATA.
           05 DB2-PI-COLUMN-NAME     USAGE IS POINTER.
           05 DB2-I-COLUMN-NAME-LEN  PIC 9(4) COMP-5.
           05 DB2-I-NUM-FREQ-VALUES  PIC S9(4) COMP-5.
           05 DB2-I-NUM-QUANTILES    PIC S9(4) COMP-5.
           05 FILLER                 PIC X(2).

      * Columns Groups to Collect Stats on
       01 DB2G-COLUMN-GRP-DATA.
           05 DB2-PI-GROUP-COLUMN-NAMES USAGE IS POINTER.
           05 DB2-PI-GROUP-COL-NAMES-LEN USAGE IS POINTER.
           05 DB2-I-GROUP-SIZE       PIC S9(4) COMP-5.
           05 DB2-I-NUM-FREQ-VALUES  PIC S9(4) COMP-5.
           05 DB2-I-NUM-QUANTILES    PIC S9(4) COMP-5.
           05 FILLER                 PIC X(2).

      * Struct containing db2gRunstats options
       01 DB2G-RUNSTATS-DATA.
           05 DB2-I-SAMPLING-OPTION  USAGE COMP-2.
           05 DB2-PI-TABLENAME       USAGE IS POINTER.
           05 DB2-PI-COLUMN-LIST     USAGE IS POINTER.
           05 DB2-PI-COLUMN-DIST-LIST USAGE IS POINTER.
           05 DB2-PI-COLUMN-GROUP-LIST USAGE IS POINTER.
           05 DB2-PI-INDEX-LIST      USAGE IS POINTER.
           05 DB2-PI-INDEX-NAMES-LEN USAGE IS POINTER.
           05 DB2-I-RUNSTATS-FLAGS   PIC 9(9) COMP-5.
           05 DB2-I-TABLENAME-LEN    PIC 9(4) COMP-5.
           05 DB2-I-NUM-COLUMNS      PIC S9(4) COMP-5.
           05 DB2-I-NUM-COLDIST      PIC S9(4) COMP-5.
           05 DB2-I-NUM-COL-GROUPS   PIC S9(4) COMP-5.
           05 DB2-I-NUM-INDEXES      PIC S9(4) COMP-5.
           05 DB2-I-PARALLELISM-OPTION PIC S9(4) COMP-5.
           05 DB2-I-TABLE-DEF-FREQ-VALUES PIC S9(4) COMP-5.
           05 DB2-I-TABLE-DEF-QUANTILES PIC S9(4) COMP-5.
           05 DB2-I-SAMPLING-REPEATABLE PIC 9(9) COMP-5.
           05 DB2-I-UTIL-IMPACT-PRIORITY PIC 9(9) COMP-5.



      *************************************************************************
      * Prune Database History Definitions
      **************************************************************************

      * Values for db2Prune - iCallerAction
      * Remove history file entries
       78  DB2PRUNE-ACTION-HISTORY   VALUE    1.
      * Remove log files from active log path
       78  DB2PRUNE-ACTION-LOG       VALUE    2.

      * Values for db2Prune - iOptions
      * Force removal of last backup
       78  DB2PRUNE-OPTION-NONE      VALUE    0.
      * Force removal of last backup
       78  DB2PRUNE-OPTION-FORCE     VALUE    1.
      * piString is an LSN-string, used by DB2PRUNE_ACTION_LOG
       78  DB2PRUNE-OPTION-LSNSTRING VALUE    2.
      * Delete pruned files
       78  DB2PRUNE-OPTION-DELETE    VALUE    4.

      * Generic Prune API input struct
       01 DB2G-PRUNE-STRUCT.
      * length in bytes of piString
           05 DB2-I-STRING-LEN       PIC 9(9) COMP-5 VALUE 0.
      * Timestamp, or lastLSN
           05 DB2-PI-STRING          USAGE IS POINTER.
      * Prune history since this EID
           05 DB2-I-EID.
      * Node number
               10 DB2-IO-NODE        PIC S9(4) COMP-5.
               10 FILLER             PIC X(2).
      * Local history file entry ID
               10 DB2-IO-HID         PIC 9(9) COMP-5.
      * What to prune
           05 DB2-I-ACTION           PIC 9(9) COMP-5.
      * Options relevent to particular iAction
           05 DB2-I-OPTIONS          PIC 9(9) COMP-5.



      * Backup Options
       78  DB2BACKUP-DB              VALUE    0.
       78  DB2BACKUP-TABLESPACE      VALUE    3.
       78  DB2BACKUP-INCREMENTAL     VALUE    16.
       78  DB2BACKUP-DELTA           VALUE    48.
       78  DB2BACKUP-OFFLINE         VALUE    0.
       78  DB2BACKUP-ONLINE          VALUE    256.
      * Compress backup image
       78  DB2BACKUP-COMPRESS        VALUE    1024.
      * Include compression lib in backup image
       78  DB2BACKUP-INCLUDE-COMPR-LIB VALUE  0.
      * Do not include compression lib in backup image
       78  DB2BACKUP-EXCLUDE-COMPR-LIB VALUE  2048.
      * Include log files in online backup image
       78  DB2BACKUP-INCLUDE-LOGS    VALUE    4096.
      * Exclude log files from backup image
       78  DB2BACKUP-EXCLUDE-LOGS    VALUE    8192.

      * Backup Caller Actions
       78  DB2BACKUP-BACKUP          VALUE    0.
       78  DB2BACKUP-CONTINUE        VALUE    1.
       78  DB2BACKUP-TERMINATE       VALUE    2.
       78  DB2BACKUP-NOINTERRUPT     VALUE    3.
       78  DB2BACKUP-DEVICE-TERMINATE VALUE   9.
       78  DB2BACKUP-PARM-CHK        VALUE    10.
       78  DB2BACKUP-PARM-CHK-INIT-WAIT VALUE 10.
       78  DB2BACKUP-PARM-CHK-ONLY   VALUE    11.

      * Restore Options
       78  DB2RESTORE-DB             VALUE    0.
       78  DB2RESTORE-TABLESPACE     VALUE    4.
       78  DB2RESTORE-HISTORY        VALUE    5.
      * Restore only compression library
       78  DB2RESTORE-COMPR-LIB      VALUE    6.
       78  DB2RESTORE-LOGS           VALUE    7.
       78  DB2RESTORE-INCREMENTAL    VALUE    16.
       78  DB2RESTORE-OFFLINE        VALUE    0.
       78  DB2RESTORE-ONLINE         VALUE    64.
       78  DB2RESTORE-DATALINK       VALUE    0.
       78  DB2RESTORE-NODATALINK     VALUE    128.
       78  DB2RESTORE-AUTOMATIC      VALUE    256.
       78  DB2RESTORE-ROLLFWD        VALUE    0.
       78  DB2RESTORE-NOROLLFWD      VALUE    512.
       78  DB2RESTORE-NOREPLACE-HISTORY VALUE 0.
       78  DB2RESTORE-REPLACE-HISTORY VALUE   1024.

      * Restore Caller Actions
       78  DB2RESTORE-RESTORE        VALUE    0.
       78  DB2RESTORE-CONTINUE       VALUE    1.
       78  DB2RESTORE-TERMINATE      VALUE    2.
       78  DB2RESTORE-NOINTERRUPT    VALUE    3.
       78  DB2RESTORE-DEVICE-TERMINATE VALUE  9.
       78  DB2RESTORE-PARM-CHK       VALUE    10.
       78  DB2RESTORE-PARM-CHK-INIT-WAIT VALUE 10.
       78  DB2RESTORE-PARM-CHK-ONLY  VALUE    11.
       78  DB2RESTORE-TERMINATE-INCRE VALUE   13.
       78  DB2RESTORE-RESTORE-STORDEF VALUE   100.
       78  DB2RESTORE-STORDEF-NOINTERRUPT VALUE 101.

      *************************************************************************
      * Generic Backup Definitions
      **************************************************************************

      * Generic Tablespace List Structure
       01 DB2G-TABLESPACE-STRUCT.
           05 DB2-TABLESPACES        USAGE IS POINTER.
           05 DB2-NUM-TABLESPACES    PIC 9(9) COMP-5.

      * Generic Media List Structure
       01 DB2G-MEDIA-LIST-STRUCT.
           05 DB2-LOCATIONS          USAGE IS POINTER.
           05 DB2-NUM-LOCATIONS      PIC 9(9) COMP-5.
           05 DB2-LOCATION-TYPE      PIC X.
           05 FILLER                 PIC X(3).

      * Generic Backup Structure
       01 DB2G-BACKUP-STRUCT.
           05 DB2-PI-DBALIAS         USAGE IS POINTER.
           05 DB2-I-DBALIAS-LEN      PIC 9(9) COMP-5.
           05 DB2-PO-APPLICATION-ID  USAGE IS POINTER.
           05 DB2-I-APPLICATION-ID-LEN PIC 9(9) COMP-5.
           05 DB2-PO-TIMESTAMP       USAGE IS POINTER.
           05 DB2-I-TIMESTAMP-LEN    PIC 9(9) COMP-5.
           05 DB2-PI-TABLESPACE-LIST USAGE IS POINTER.
           05 DB2-PI-MEDIA-LIST      USAGE IS POINTER.
           05 DB2-PI-USERNAME        USAGE IS POINTER.
           05 DB2-I-USERNAME-LEN     PIC 9(9) COMP-5.
           05 DB2-PI-PASSWORD        USAGE IS POINTER.
           05 DB2-I-PASSWORD-LEN     PIC 9(9) COMP-5.
           05 DB2-PI-VENDOR-OPTIONS  USAGE IS POINTER.
           05 DB2-I-VENDOR-OPTIONS-SIZE PIC 9(9) COMP-5.
           05 DB2-O-BACKUP-SIZE      PIC 9(9) COMP-5.
           05 DB2-I-CALLER-ACTION    PIC 9(9) COMP-5.
           05 DB2-I-BUFFER-SIZE      PIC 9(9) COMP-5.
           05 DB2-I-NUM-BUFFERS      PIC 9(9) COMP-5.
           05 DB2-I-PARALLELISM      PIC 9(9) COMP-5.
           05 DB2-I-OPTIONS          PIC 9(9) COMP-5.
      * Throttle Parameter
           05 DB2-I-UTIL-IMPACT-PRIORITY PIC 9(9) COMP-5.
      * Name of compression library
           05 DB2-PI-COMPR-LIBRARY   USAGE IS POINTER.
      * Length of compression library name
           05 DB2-I-COMPR-LIBRARY-LEN PIC 9(9) COMP-5.
      * Vendor-specific options for compression plug-in
           05 DB2-PI-COMPR-OPTIONS   USAGE IS POINTER.
      * Size of piComprOptions block
           05 DB2-I-COMPR-OPTIONS-SIZE PIC 9(9) COMP-5.

      * Generic Backup API

      *************************************************************************
      * Generic Restore Definitions
      **************************************************************************

      * Generic Storage Paths Structure
       01 DB2G-STORAGE-PATHS-STRUCT.
           05 DB2-STORAGE-PATHS      USAGE IS POINTER.
           05 DB2-NUM-STORAGE-PATHS  PIC 9(9) COMP-5.

      * Generic Restore Structure
       01 DB2G-RESTORE-STRUCT.
           05 DB2-PI-SOURCE-DBALIAS  USAGE IS POINTER.
           05 DB2-I-SOURCE-DBALIAS-LEN PIC 9(9) COMP-5.
           05 DB2-PI-TARGET-DBALIAS  USAGE IS POINTER.
           05 DB2-I-TARGET-DBALIAS-LEN PIC 9(9) COMP-5.
           05 DB2-PO-APPLICATION-ID  USAGE IS POINTER.
           05 DB2-I-APPLICATION-ID-LEN PIC 9(9) COMP-5.
           05 DB2-PI-TIMESTAMP       USAGE IS POINTER.
           05 DB2-I-TIMESTAMP-LEN    PIC 9(9) COMP-5.
           05 DB2-PI-TARGET-DBPATH   USAGE IS POINTER.
           05 DB2-I-TARGET-DBPATH-LEN PIC 9(9) COMP-5.
           05 DB2-PI-REPORT-FILE     USAGE IS POINTER.
           05 DB2-I-REPORT-FILE-LEN  PIC 9(9) COMP-5.
           05 DB2-PI-TABLESPACE-LIST USAGE IS POINTER.
           05 DB2-PI-MEDIA-LIST      USAGE IS POINTER.
           05 DB2-PI-USERNAME        USAGE IS POINTER.
           05 DB2-I-USERNAME-LEN     PIC 9(9) COMP-5.
           05 DB2-PI-PASSWORD        USAGE IS POINTER.
           05 DB2-I-PASSWORD-LEN     PIC 9(9) COMP-5.
           05 DB2-PI-NEW-LOG-PATH    USAGE IS POINTER.
           05 DB2-I-NEW-LOG-PATH-LEN PIC 9(9) COMP-5.
           05 DB2-PI-VENDOR-OPTIONS  USAGE IS POINTER.
           05 DB2-I-VENDOR-OPTIONS-SIZE PIC 9(9) COMP-5.
           05 DB2-I-PARALLELISM      PIC 9(9) COMP-5.
           05 DB2-I-BUFFER-SIZE      PIC 9(9) COMP-5.
           05 DB2-I-NUM-BUFFERS      PIC 9(9) COMP-5.
           05 DB2-I-CALLER-ACTION    PIC 9(9) COMP-5.
           05 DB2-I-OPTIONS          PIC 9(9) COMP-5.
      * Name of compression library
           05 DB2-PI-COMPR-LIBRARY   USAGE IS POINTER.
      * Length of compression library name
           05 DB2-I-COMPR-LIBRARY-LEN PIC 9(9) COMP-5.
      * Vendor-specific options for compression plug-in
           05 DB2-PI-COMPR-OPTIONS   USAGE IS POINTER.
      * Size of piComprOptions block
           05 DB2-I-COMPR-OPTIONS-SIZE PIC 9(9) COMP-5.
           05 DB2-PI-LOG-TARGET      USAGE IS POINTER.
           05 DB2-I-LOG-TARGET-LEN   PIC 9(9) COMP-5.
           05 DB2-PI-STORAGE-PATHS   USAGE IS POINTER.

      * Generic Restore API


      *************************************************************************
      * Rollforward Definitions
      **************************************************************************

      * Values for db2Rollforward - CallerAction
      * Rollforward to requested point
       78  DB2ROLLFORWARD-RFWD       VALUE    1.
      * End rollforward recovery
       78  DB2ROLLFORWARD-STOP       VALUE    2.
      * End rollforward recovery
       78  DB2ROLLFORWARD-COMPLETE   VALUE    2.
      * Rollforward  to requested point and end rollforward recovery
       78  DB2ROLLFORWARD-RFWD-STOP  VALUE    3.
      * Rollforward  to requested point and end rollforward recovery
       78  DB2ROLLFORWARD-RFWD-COMPLETE VALUE 3.
      * Query nextarclog, firstarcdel, lastarcdel, lastcommit
       78  DB2ROLLFORWARD-QUERY      VALUE    4.
      * Cancel current rollforward
       78  DB2ROLLFORWARD-CANCEL     VALUE    8.
      * Parameter check
       78  DB2ROLLFORWARD-PARM-CHECK VALUE    10.
      * Continue load recovery
       78  DB2ROLLFORWARD-LOADREC-CONT VALUE  11.
      * Terminate load recovery
       78  DB2ROLLFORWARD-LOADREC-TERM VALUE  12.
      * Terminate device
       78  DB2ROLLFORWARD-DEVICE-TERM VALUE   13.

      * Values for db2Rollforward - ConnectMode
      * Offline rollforward mode
       78  DB2ROLLFORWARD-OFFLINE    VALUE    0.
      * Online rollforward mode
       78  DB2ROLLFORWARD-ONLINE     VALUE    1.

      * Values for db2Rollforward - RollforwardFlags
      * No flags specified
       78  DB2ROLLFORWARD-EMPTY-FLAG VALUE    0.
      * Interpret stop time as local time, not GMT
       78  DB2ROLLFORWARD-LOCAL-TIME VALUE    1.
      * Do not attempt to retrieve log files via userexit
       78  DB2ROLLFORWARD-NO-RETRIEVE VALUE   2.

      * Length of arrays used by db2Rollforward
      * Length of ISO format timestamp
       78  DB2-ISO-TIMESTAMP-LEN     VALUE    26.
      * Length of logfile name
       78  DB2-LOGFILE-NAME-LEN      VALUE    12.
      * Maximum length of a logfile path
       78  DB2-LOGPATH-LEN           VALUE    242.

      * Generic Rollforward Input Structure
       01 DB2G-RFWD-INPUT-STRUCT.
      * Length in bytes of piDbAlias
           05 SQL-DBALIASLEN         PIC 9(9) COMP-5.
      * Length in bytes of piStopTime
           05 SQL-STOPTIMELEN        PIC 9(9) COMP-5.
      * Length in bytes of piUserName
           05 SQL-USERNAMELEN        PIC 9(9) COMP-5.
      * Length in bytes of piPassword
           05 SQL-PASSWORDLEN        PIC 9(9) COMP-5.
      * Length in bytes of piOverflowLogPath
           05 SQL-OVRLOGPATHLEN      PIC 9(9) COMP-5.
      * Length in bytes of piDroppedTblID
           05 SQL-DROPPEDTBLIDLEN    PIC 9(9) COMP-5.
      * Length in bytes of piExportDir
           05 SQL-EXPORTDIRLEN       PIC 9(9) COMP-5.
      * Rollforward version
           05 SQL-VERSION            PIC 9(9) COMP-5.
      * Database alias
           05 SQL-DBALIAS            USAGE IS POINTER.
      * Rollforward action
           05 SQL-CALLERACTION       PIC 9(9) COMP-5.
      * Stop time
           05 SQL-STOPTIME           USAGE IS POINTER.
      * User name to execute under
           05 SQL-USERNAME           USAGE IS POINTER.
      * Password
           05 SQL-PASSWORD           USAGE IS POINTER.
      * Overflow log path
           05 SQL-OVERFLOWLOGPATH    USAGE IS POINTER.
      * Number of changed overflow log paths
           05 SQL-NUMCHANGE          PIC 9(9) COMP-5.
      * Changed overflow log paths
           05 DB2-PI-CHNG-LOG-OVRFLW USAGE IS POINTER.
      * Connect mode
           05 SQL-CONNECTMODE        PIC 9(9) COMP-5.
      * List of tablespaces to rollforward
           05 DB2-PI-TABLESPACE-LIST USAGE IS POINTER.
      * Flag indicating how piNodeist should be used
           05 SQL-ALLNODEFLAG        PIC S9(9) COMP-5.
      * Number of nodes in piNodeList
           05 SQL-NUMNODES           PIC S9(9) COMP-5.
      * Node list
           05 SQL-NODELIST           USAGE IS POINTER.
      * Size of piNodeInfo in db2RfwdOutput
           05 SQL-NUMNODEINFO        PIC S9(9) COMP-5.
      * Dropped table id
           05 SQL-DROPPEDTBLID       USAGE IS POINTER.
      * Dropped table export directory
           05 SQL-EXPORTDIR          USAGE IS POINTER.
      * Rollforward flags
           05 SQL-ROLLFORWARDFLAGS   PIC 9(9) COMP-5.

      * Rollforward Output Structure
       01 DB2RFWD-OUTPUT-STRUCT.
      * application id
           05 SQL-APPLID             USAGE IS POINTER.
      * number of replies received
           05 SQL-NUMREPLIES         USAGE IS POINTER.
      * node reply info
           05 DB2-PO-NODE-INFO       USAGE IS POINTER.

      * Generic Rollforward API Structure
       01 DB2G-ROLLFORWARD-STRUCT.
      * generic rollforward input structure
           05 DB2-PI-RFWD-INPUT      USAGE IS POINTER.
      * rollforward output structure
           05 DB2-PO-RFWD-OUTPUT     USAGE IS POINTER.


      *************************************************************************
      * High Availability Disaster Recovery (HADR) Definitions
      **************************************************************************

      * Values for iByForce
      * Do not perform START or TAKEOVER HADR operation by force
       78  DB2HADR-NO-FORCE          value    0.
      * Do perform START or TAKEOVER HADR operation by force
       78  DB2HADR-FORCE             value    1.

      * Values for iDbRole
      * An HADR Primary database
       78  DB2HADR-DB-ROLE-PRIMARY   value    1.
      * An HADR Standby database
       78  DB2HADR-DB-ROLE-STANDBY   value    2.

      *************************************************************************
      * High Availability Disaster Recovery (HADR) API Definitions
      **************************************************************************

      * Generic Start HADR API Input Struct
       01 DB2G-HADRSTART-STRUCT.
      * Database alias
           05 DB2-PI-DB-ALIAS        USAGE IS POINTER.
      * Length of database alias string
           05 DB2-I-ALIAS-LEN        PIC 9(9) COMP-5.
      * User name to execute command under
           05 DB2-PI-USER-NAME       USAGE IS POINTER.
      * Length of username field
           05 DB2-I-USER-NAME-LEN    PIC 9(9) COMP-5.
      * Password for username
           05 DB2-PI-PASSWORD        USAGE IS POINTER.
      * Length of password field
           05 DB2-I-PASSWORD-LEN     PIC 9(9) COMP-5.
      * Primary db or standby
           05 DB2-I-DB-ROLE          PIC 9(9) COMP-5.
      * Start HADR by force
           05 DB2-I-BY-FORCE         PIC 9(4) COMP-5.
           05 FILLER                 PIC X(2).



      * Generic Stop HADR API Input Struct
       01 DB2G-HADRSTOP-STRUCT.
      * Database alias
           05 DB2-PI-DB-ALIAS        USAGE IS POINTER.
      * Length of database alias string
           05 DB2-I-ALIAS-LEN        PIC 9(9) COMP-5.
      * User name to execute command under
           05 DB2-PI-USER-NAME       USAGE IS POINTER.
      * Length of username field
           05 DB2-I-USER-NAME-LEN    PIC 9(9) COMP-5.
      * Password for username
           05 DB2-PI-PASSWORD        USAGE IS POINTER.
      * Length of password field
           05 DB2-I-PASSWORD-LEN     PIC 9(9) COMP-5.



      * Generic Takeover HADR API Input Struct
       01 DB2G-HADRTAKEOVER-STRUCT.
      * Database alias
           05 DB2-PI-DB-ALIAS        USAGE IS POINTER.
      * Length of database alias string
           05 DB2-I-ALIAS-LEN        PIC 9(9) COMP-5.
      * User name to execute command under
           05 DB2-PI-USER-NAME       USAGE IS POINTER.
      * Length of username field
           05 DB2-I-USER-NAME-LEN    PIC 9(9) COMP-5.
      * Password for username
           05 DB2-PI-PASSWORD        USAGE IS POINTER.
      * Length of password field
           05 DB2-I-PASSWORD-LEN     PIC 9(9) COMP-5.
      * Takeover HADR by force
           05 DB2-I-BY-FORCE         PIC 9(4) COMP-5.
           05 FILLER                 PIC X(2).


      *************************************************************************
      * Archive Active Log Definitions
      **************************************************************************

      * Values for db2ArchiveLog - iAllNodeFlag
       78  DB2ARCHIVELOG-NODE-LIST   value    0.
       78  DB2ARCHIVELOG-ALL-NODES   value    1.
       78  DB2ARCHIVELOG-ALL-EXCEPT  value    2.

      * Generic Archive Active Log API Input Struct
       01 DB2G-ARCHIVE-LOG-STRUCT.
      * Length of database alias field
           05 DB2-I-ALIAS-LEN        PIC 9(9) COMP-5.
      * Length of username field
           05 DB2-I-USER-NAME-LEN    PIC 9(9) COMP-5.
      * Length of password field
           05 DB2-I-PASSWORD-LEN     PIC 9(9) COMP-5.
      * Alias of database to archive log for
           05 DB2-PI-DATABASE-ALIAS  USAGE IS POINTER.
      * User name to execute command under
           05 DB2-PI-USER-NAME       USAGE IS POINTER.
      * Password for username
           05 DB2-PI-PASSWORD        USAGE IS POINTER.
      * Flag indicating how the iNodeList should be used
           05 DB2-I-ALL-NODE-FLAG    PIC 9(4) COMP-5.
      * Number of nodes in piNodeList array
           05 DB2-I-NUM-NODES        PIC 9(4) COMP-5.
      * Node list
           05 DB2-PI-NODE-LIST       USAGE IS POINTER.
      * Future use
           05 DB2-I-OPTIONS          PIC 9(9) COMP-5.


      *************************************************************************
      * Asynchronous Read Log Definitions
      **************************************************************************

      * Values for db2ReadLog - iCallerAction
      * Read the database log
       78  DB2READLOG-READ           VALUE    1.
      * Read a single log record
       78  DB2READLOG-READ-SINGLE    VALUE    2.
      * Query the database log
       78  DB2READLOG-QUERY          VALUE    3.

      * Values for db2ReadLog - iFilterOption
      * Read all log records
       78  DB2READLOG-FILTER-OFF     VALUE    0.
      * Read only propagatable records
       78  DB2READLOG-FILTER-ON      VALUE    1.

      *************************************************************************
      * Generic Ping Database Parameter Structure 
      **************************************************************************
       01 DB2G-DATABASE-PING-STRUCT.
      * Input: Db alias name length - reserved
           05 DB2-I-DB-ALIAS-LENGTH  PIC 9(4) COMP-5.
      * Input: Database alias - reserved
           05 DB2-I-DB-ALIAS         PIC X(8).
           05 FILLER                 PIC X.
           05 FILLER                 PIC X(1).
      * Input: size of request packet
           05 DB2-REQUEST-PACKET-SZ  PIC S9(9) COMP-5.
      * Input: size of response packet
           05 DB2-RESPONSE-PACKET-SZ PIC S9(9) COMP-5.
      * Input: Number of iterations
           05 DB2-I-NUM-ITERATIONS   PIC 9(4) COMP-5.
           05 FILLER                 PIC X(2).
      * Output: Array of elapsed times (microsec), contains iNumIterations
      * elements
           05 DB2-PO-ELAPSED-TIME    USAGE IS POINTER.


      * Convert Monitor Data Interface Structure
       01 DB2CONV-MON-STREAM-DATA.
      * Pointer to target structure
           05 DB2-PO-TARGET          USAGE IS POINTER.
      * Pointer to V6 stream
           05 DB2-PI-SOURCE          USAGE IS POINTER.
      * Target structure type
           05 DB2-I-TARGET-TYPE      PIC 9(9) COMP-5.
      * Space allocated for target
           05 DB2-I-TARGET-SIZE      PIC 9(9) COMP-5.
      * Type of source stream
           05 DB2-I-SOURCE-TYPE      PIC 9(9) COMP-5.
      * Get Snapshot Data Interface Structure
       01 DB2G-GET-SNAPSHOT-DATA.
      * Pointer to monitor area
           05 DB2-PI-SQLMA-DATA      USAGE IS POINTER.
      * Pointer to collected data
           05 DB2-PO-COLLECTED-DATA  USAGE IS POINTER.
      * Pointer to output buffer
           05 DB2-PO-BUFFER          USAGE IS POINTER.
      * Snapshot version
           05 DB2-I-VERSION          PIC 9(9) COMP-5.
      * Size of output buffer
           05 DB2-I-BUFFER-SIZE      PIC 9(9) COMP-5.
      * Write to file flag
           05 DB2-I-STORE-RESULT     PIC 9(9) COMP-5.
      * Target node
           05 DB2-I-NODE-NUMBER      PIC S9(9) COMP-5.
      * Pointer to output format indicator
           05 DB2-PO-OUTPUT-FORMAT   USAGE IS POINTER.
      * Class qualifier for snapshot
           05 DB2-I-SNAPSHOT-CLASS   PIC 9(9) COMP-5.

      * Get Snapshot Size Data Interface Structure
       01 DB2G-GET-SNAPSHOT-SIZE-DATA.
      * Pointer to monitor area
           05 DB2-PI-SQLMA-DATA      USAGE IS POINTER.
      * Pointer to output buffer
           05 DB2-PO-BUFFER-SIZE     USAGE IS POINTER.
      * Snapshot version
           05 DB2-I-VERSION          PIC 9(9) COMP-5.
      * Target node
           05 DB2-I-NODE-NUMBER      PIC S9(9) COMP-5.
      * Class qualifier for snapshot
           05 DB2-I-SNAPSHOT-CLASS   PIC 9(9) COMP-5.

      * Monitor Switches Data Interface Structure
       01 DB2G-MONITOR-SWITCHES-DATA.
      * Pointer to group states
           05 DB2-PI-GROUP-STATES    USAGE IS POINTER.
      * Pointer to output buffer
           05 DB2-PO-BUFFER          USAGE IS POINTER.
      * Size of output buffer
           05 DB2-I-BUFFER-SIZE      PIC 9(9) COMP-5.
      * Return data flag
           05 DB2-I-RETURN-DATA      PIC 9(9) COMP-5.
      * Snapshot version
           05 DB2-I-VERSION          PIC 9(9) COMP-5.
      * Target node
           05 DB2-I-NODE-NUMBER      PIC S9(9) COMP-5.
      * Pointer to output format indicator
           05 DB2-PO-OUTPUT-FORMAT   USAGE IS POINTER.

      * Monitor Reset Data Interface Structure
      * Monitor Reset Data General Interface Structure
       01 DB2G-RESET-MONITOR-DATA.
      * Reset value flag
           05 DB2-I-RESET-ALL        PIC 9(9) COMP-5.
      * Database alias
           05 DB2-PI-DB-ALIAS        USAGE IS POINTER.
      * Length of DB alias
           05 DB2-I-DB-ALIAS-LENGTH  PIC 9(9) COMP-5.
      * Snapshot version
           05 DB2-I-VERSION          PIC 9(9) COMP-5.
      * Target node
           05 DB2-I-NODE-NUMBER      PIC S9(9) COMP-5.

       01 DB2G-RESTART-DB-STRUCT.
      * Length in bytes of piDatabaseName
           05 DB2-I-DATABASE-NAME-LEN PIC 9(9) COMP-5.
      * Length in bytes of piUserId
           05 DB2-I-USER-ID-LEN      PIC 9(9) COMP-5.
      * Length in bytes of piPassword
           05 DB2-I-PASSWORD-LEN     PIC 9(9) COMP-5.
      * Length in bytes of piTablespaceNames
           05 DB2-I-TABLESPACE-NAMES-LEN PIC 9(9) COMP-5.
      * Database name or alias
           05 DB2-PI-DATABASE-NAME   USAGE IS POINTER.
      * User Id
           05 DB2-PI-USER-ID         USAGE IS POINTER.
      * Password
           05 DB2-PI-PASSWORD        USAGE IS POINTER.
      * Tablespace name list
           05 DB2-PI-TABLESPACE-NAMES USAGE IS POINTER.

      *************************************************************************
      * Update LDAP Alternate Server Parameter Structure 
      **************************************************************************
      *************************************************************************
      * Update LDAP Alternate Server API 
      **************************************************************************

      *************************************************************************
      * Inspect Database Definitions
      **************************************************************************

      * Inspect API structure
       01 DB2INSPECT-STRUCT.
      * Tablespace name; null terminated
           05 DB2-PI-TABLESPACE-NAME USAGE IS POINTER.
      * Table name; null terminated
           05 DB2-PI-TABLE-NAME      USAGE IS POINTER.
      * Schema name; null terminated
           05 DB2-PI-SCHEMA-NAME     USAGE IS POINTER.
      * Results Output file name; null terminated
           05 DB2-PI-RESULTS-NAME    USAGE IS POINTER.
      * Data file name; null terminated
           05 DB2-PI-DATA-FILE-NAME  USAGE IS POINTER.
      * Array of nodes
           05 DB2-PI-NODE-LIST       USAGE IS POINTER.
      * Action
           05 DB2-I-ACTION           PIC 9(9) COMP-5.
      * Tablespace ID
           05 DB2-I-TABLESPACE-ID    PIC S9(9) COMP-5.
      * Object ID
           05 DB2-I-OBJECT-ID        PIC S9(9) COMP-5.
      * Page number of first page
           05 DB2-I-FIRST-PAGE       PIC 9(9) COMP-5.
      * Number of pages
           05 DB2-I-NUMBER-OF-PAGES  PIC 9(9) COMP-5.
      * Format type
           05 DB2-I-FORMAT-TYPE      PIC 9(9) COMP-5.
      * Options
           05 DB2-I-OPTIONS          PIC 9(9) COMP-5.
      * Begin checking option
           05 DB2-I-BEGIN-CHECK-OPTION PIC 9(9) COMP-5.
      * Number of pages in error in object to limit reporting of
           05 DB2-I-LIMIT-ERROR-REPORTED PIC S9(9) COMP-5.
      * Object error state option
           05 DB2-I-OBJECT-ERROR-STATE PIC 9(4) COMP-5.
      * Catalog to tablespace consistency option
           05 DB2-I-CATALOG-TO-TABLESPACE PIC 9(4) COMP-5.
      * Keep result file
           05 DB2-I-KEEP-RESULTFILE  PIC 9(4) COMP-5.
      * all node flag
           05 DB2-I-ALL-NODE-FLAG    PIC 9(4) COMP-5.
      * Number of nodes
           05 DB2-I-NUM-NODES        PIC 9(4) COMP-5.
      * Processing level for Data object
           05 DB2-I-LEVEL-OBJECT-DATA PIC 9(4) COMP-5.
      * Processing level for Index object
           05 DB2-I-LEVEL-OBJECT-INDEX PIC 9(4) COMP-5.
      * Processing level for Long object
           05 DB2-I-LEVEL-OBJECT-LONG PIC 9(4) COMP-5.
      * Processing level for LOB object
           05 DB2-I-LEVEL-OBJECT-LOB PIC 9(4) COMP-5.
      * Processing level for Block map object
           05 DB2-I-LEVEL-OBJECT-BLK-MAP PIC 9(4) COMP-5.
      * Processing level for Extent map
           05 DB2-I-LEVEL-EXTENT-MAP PIC 9(4) COMP-5.
           05 FILLER                 PIC X(2).


      * Generic Inspect API structure
       01 DB2G-INSPECT-STRUCT.
      * Tablespace name
           05 DB2-PI-TABLESPACE-NAME USAGE IS POINTER.
      * Table name
           05 DB2-PI-TABLE-NAME      USAGE IS POINTER.
      * Schema name
           05 DB2-PI-SCHEMA-NAME     USAGE IS POINTER.
      * Results Output file name
           05 DB2-PI-RESULTS-NAME    USAGE IS POINTER.
      * Data file name
           05 DB2-PI-DATA-FILE-NAME  USAGE IS POINTER.
      * Array of nodes
           05 DB2-PI-NODE-LIST       USAGE IS POINTER.
      * Results Output file name length
           05 DB2-I-RESULTS-NAME-LENGTH PIC 9(9) COMP-5.
      * Data file name length
           05 DB2-I-DATA-FILE-NAME-LENGTH PIC 9(9) COMP-5.
      * Tablespace name length
           05 DB2-I-TABLESPACE-NAME-LENGTH PIC 9(9) COMP-5.
      * Table name length
           05 DB2-I-TABLE-NAME-LENGTH PIC 9(9) COMP-5.
      * Schema name length
           05 DB2-I-SCHEMA-NAME-LENGTH PIC 9(9) COMP-5.
      * Action
           05 DB2-I-ACTION           PIC 9(9) COMP-5.
      * Tablespace ID
           05 DB2-I-TABLESPACE-ID    PIC S9(9) COMP-5.
      * Object ID
           05 DB2-I-OBJECT-ID        PIC S9(9) COMP-5.
      * Page number of first page
           05 DB2-I-FIRST-PAGE       PIC 9(9) COMP-5.
      * Number of pages
           05 DB2-I-NUMBER-OF-PAGES  PIC 9(9) COMP-5.
      * Format type
           05 DB2-I-FORMAT-TYPE      PIC 9(9) COMP-5.
      * Options
           05 DB2-I-OPTIONS          PIC 9(9) COMP-5.
      * Begin checking option
           05 DB2-I-BEGIN-CHECK-OPTION PIC 9(9) COMP-5.
      * Number of pages in error in object to limit reporting of
           05 DB2-I-LIMIT-ERROR-REPORTED PIC S9(9) COMP-5.
      * Object error state option
           05 DB2-I-OBJECT-ERROR-STATE PIC 9(4) COMP-5.
      * Catalog to tablespace consistency option
           05 DB2-I-CATALOG-TO-TABLESPACE PIC 9(4) COMP-5.
      * Keep result file
           05 DB2-I-KEEP-RESULTFILE  PIC 9(4) COMP-5.
      * all node flag
           05 DB2-I-ALL-NODE-FLAG    PIC 9(4) COMP-5.
      * Number of nodes
           05 DB2-I-NUM-NODES        PIC 9(4) COMP-5.
      * Processing level for Data object
           05 DB2-I-LEVEL-OBJECT-DATA PIC 9(4) COMP-5.
      * Processing level for Index object
           05 DB2-I-LEVEL-OBJECT-INDEX PIC 9(4) COMP-5.
      * Processing level for Long object
           05 DB2-I-LEVEL-OBJECT-LONG PIC 9(4) COMP-5.
      * Processing level for LOB object
           05 DB2-I-LEVEL-OBJECT-LOB PIC 9(4) COMP-5.
      * Processing level for Block map object
           05 DB2-I-LEVEL-OBJECT-BLK-MAP PIC 9(4) COMP-5.
      * Processing level for Extent map
           05 DB2-I-LEVEL-EXTENT-MAP PIC 9(4) COMP-5.
           05 FILLER                 PIC X(2).


      * Values for db2Inspect - Action constants
       78  DB2INSPECT-ACTION-CHECK   VALUE    1.
       78  DB2INSPECT-ACTION-FORMAT  VALUE    65536.

      * Values for db2Inspect - iAction
      * DB2INSPECT_ACT_CHECK_RESTART   - Inspect check restart    
      * DB2INSPECT_ACT_CHECK_DB        - Inspect DB               
      * DB2INSPECT_ACT_CHECK_TABSPACE  - Inspect tablespace       
      * DB2INSPECT_ACT_CHECK_TABLE     - Inspect table            
      * DB2INSPECT_ACT_CHECK_DATA      - Inspect data object      
      * DB2INSPECT_ACT_CHECK_INDEX     - Inspect index object     
      * DB2INSPECT_ACT_CHECK_BLOCKMAP  - Inspect block map object 
      * DB2INSPECT_ACT_FORMAT_PAGE     - Format page in tablespace
      * DB2INSPECT_ACT_FORMAT_EXT_MAP  - Format extent map        
      * DB2INSPECT_ACT_FORMAT_DATA     - Format data page         
      * DB2INSPECT_ACT_FORMAT_INDEX    - Format index page        
      * DB2INSPECT_ACT_FORMAT_BLOCKMAP - Format block map page    
      * Inspect check restart
       78  DB2INSPECT-ACT-CHECK-RESTART value 2.
      * Inspect DB
       78  DB2INSPECT-ACT-CHECK-DB   value    3.
      * Inspect tablespace
       78  DB2INSPECT-ACT-CHECK-TABSPACE value 4.
      * Inspect table
       78  DB2INSPECT-ACT-CHECK-TABLE value   5.
      * Inspect data object
       78  DB2INSPECT-ACT-CHECK-DATA value    6.
      * Inspect index object
       78  DB2INSPECT-ACT-CHECK-INDEX value   7.
      * Inspect block map object
       78  DB2INSPECT-ACT-CHECK-BLOCKMAP value 8.
      * Reserved
       78  DB2INSPECT-ACT-CHECK-08   value    9.
      * Reserved
       78  DB2INSPECT-ACT-CHECK-09   value    10.
      * Reserved
       78  DB2INSPECT-ACT-CHECK-10   value    11.
      * Format page in tablespace
       78  DB2INSPECT-ACT-FORMAT-PAGE value   65537.
      * Format extent map
       78  DB2INSPECT-ACT-FORMAT-EXT-MAP value 65538.
      * Format data page
       78  DB2INSPECT-ACT-FORMAT-DATA value   65539.
      * Format index page
       78  DB2INSPECT-ACT-FORMAT-INDEX value  65540.
      * Format block map page
       78  DB2INSPECT-ACT-FORMAT-BLOCKMAP value 65541.
      * Reserved
       78  DB2INSPECT-ACT-FORMAT-06  value    65542.
      * Reserved
       78  DB2INSPECT-ACT-FORMAT-07  value    65543.
      * Reserved
       78  DB2INSPECT-ACT-FORMAT-08  value    65544.
      * Reserved
       78  DB2INSPECT-ACT-FORMAT-09  value    65545.
      * Reserved
       78  DB2INSPECT-ACT-FORMAT-10  value    65546.

      * Values for db2Inspect - iFormatType
       78  DB2INSPECT-FORMAT-TYPE-HEX VALUE   1.
       78  DB2INSPECT-FORMAT-TYPE-BRIEF VALUE 2.
       78  DB2INSPECT-FORMAT-TYPE-DETAIL VALUE 3.
       78  DB2INSPECT-FORMAT-TYPE-DEL VALUE   4.

      * Values for db2Inspect - iOptions
      * Format - 1st page number logical
       78  DB2INSPECT-OPTS-PAGE-LOGICAL VALUE 0.
      * Format - 1st page number physical
       78  DB2INSPECT-OPTS-PAGE-PHYSICAL VALUE 1.

      * Values for db2Inspect - iBeginCheckOption
      * Begin from the start.
       78  DB2INSPECT-BEGIN-FROM-START VALUE  0.
      * Begin check on tablespace ID.
       78  DB2INSPECT-BEGIN-TSPID    VALUE    1.
      * Begin check on object ID.
       78  DB2INSPECT-BEGIN-OBJID    VALUE    2.
       78  DB2INSPECT-BEGIN-TSPID-OBJID value 3.

      * Values for db2Inspect - iLimitErrorReported
      * limit errors reported to default number
       78  DB2INSPECT-LIMIT-ERROR-DEFAULT VALUE 0.
      * no limit to errors reported
       78  DB2INSPECT-LIMIT-ERROR-ALL VALUE   -1.

      * Values for db2Inspect - iObjectErrorState
      * process object only in normal state
       78  DB2INSPECT-ERROR-STATE-NORMAL VALUE 0.
      * process object in any state
       78  DB2INSPECT-ERROR-STATE-ALL VALUE   1.

      * Values for db2Inspect - iCatalogToTablespace
      * Catalog to tablespace consistency not requested
       78  DB2INSPECT-CAT-TO-TABSP-NONE VALUE 0.
      * Catalog to tablespace consistency requested
       78  DB2INSPECT-CAT-TO-TABSP-YES VALUE  1.

      * Values for db2Inspect - iKeepResultfile
      * Clean up result file when no error
       78  DB2INSPECT-RESFILE-CLEANUP VALUE   0.
      * Always keep result file
       78  DB2INSPECT-RESFILE-KEEP-ALWAYS VALUE 1.

      * Values for db2Inspect - Level constants
       78  DB2INSPECT-LEVEL-DEFAULT  VALUE    0.
       78  DB2INSPECT-LEVEL-NONE     VALUE    1.
       78  DB2INSPECT-LEVEL-NORMAL   value    0.
       78  DB2INSPECT-LEVEL-LOW      VALUE    16384.
       78  DB2INSPECT-LEVEL-HIGH     VALUE    49152.

      * Values for db2Inspect - iNumberOfPages
      * To the last page
       78  DB2INSPECT-NUMPAGES-TO-THE-END VALUE 0.

      * Inspect API

      * Generic Inspect API



      *************************************************************************
      * Utility Control API
      **************************************************************************

      * Utility Throttling Constants
      * Default throttling priority
       78  SQL-UTIL-IMPACT-PRIORITY-DFLT VALUE 50.

      * Utility Control Structure
       01 DB2UTILITY-CONTROL-STRUCT.
      * Utility id
           05 DB2-I-ID               PIC 9(9) COMP-5.
      * Attribute to modify
           05 DB2-I-ATTRIBUTE        PIC 9(9) COMP-5.
      * Attribute data
           05 DB2-PIO-VALUE          USAGE IS POINTER.

      * Valid utility attributes
      * Modify utility priority
       78  DB2UTILCTRL-PRIORITY-ATTRIB VALUE  1.

      * Utility Control API

      * Generic Utility Control Structure
       01 DB2G-UTILITY-CONTROL-STRUCT.
      * Utility id
           05 DB2-I-ID               PIC 9(9) COMP-5.
      * Attribute to modify
           05 DB2-I-ATTRIBUTE        PIC 9(9) COMP-5.
      * Attribute data
           05 DB2-PIO-VALUE          USAGE IS POINTER.


      * Generic Utility Control API

      *************************************************************************
      * Quiesce Definitions
      **************************************************************************



































      * Get Node Number Interface Structure
      *************************************************************************
      * Update Alternate Server Parameter Structure 
      **************************************************************************
      *************************************************************************
      * Update Alternate Server API 
      **************************************************************************
      *************************************************************************
      * Generic Update Alternate Server Structure 
      **************************************************************************
       01 DB2G-UPDATE-ALT-SERVER-STRUCT.
      * Input: database alias length
           05 DB2-I-DB-ALIAS-LEN     PIC 9(9) COMP-5.
      * Input: database alias
           05 DB2-PI-DB-ALIAS        USAGE IS POINTER.
      * Input: host name length
           05 DB2-I-HOST-NAME-LEN    PIC 9(9) COMP-5.
      * Input: host name
           05 DB2-PI-HOST-NAME       USAGE IS POINTER.
      * Input: port number length
           05 DB2-I-PORT-LEN         PIC 9(9) COMP-5.
      * Input: port number
           05 DB2-PI-PORT            USAGE IS POINTER.

      *************************************************************************
      * Generic Update Alternate Server API 
      **************************************************************************

      *************************************************************************
      * Open Database Directory Scan Parameter Structure 
      **************************************************************************
      *************************************************************************
      * Open Database Directory Scan API 
      **************************************************************************
      *************************************************************************
      * Generic Open Database Directory Scan Parameter Structure 
      **************************************************************************
       01 DB2G-DB-DIR-OPEN-SCAN-STRUCT.
      * Input: drive/path length
           05 DB2-I-PATH-LEN         PIC 9(9) COMP-5.
      * Input: drive/path
           05 DB2-PI-PATH            USAGE IS POINTER.
      * Output: handle
           05 DB2-O-HANDLE           PIC 9(4) COMP-5.
      * Output: count
           05 DB2-O-NUM-ENTRIES      PIC 9(4) COMP-5.

      *************************************************************************
      * Generic Open Database Directory Scan API 
      **************************************************************************

      *************************************************************************
      * Get Next Database Directory Entry Data Structure 
      **************************************************************************
       01 DB2DB-DIR-INFO.
      * Alias name
           05 SQL-ALIAS-N            PIC X(8).
      * Database name
           05 SQL-DBNAME-N           PIC X(8).
      * Drive/Path
           05 SQL-DRIVE-N            PIC X(12).
      * Database subdirectory
           05 SQL-INTNAME-N          PIC X(8).
      * Node name
           05 SQL-NODENAME-N         PIC X(8).
      * Release information
           05 SQL-DBTYPE-N           PIC X(20).
      * Comment
           05 SQL-COMMENT-N          PIC X(30).
      * Code page of comment
           05 SQL-COM-CODEPAGE-N     PIC S9(4) COMP-5.
      * Entry type - defines above
           05 SQL-TYPE-N             PIC X.
           05 FILLER                 PIC X(1).
      * Authentication type
           05 SQL-AUTHENTICATION-N   PIC 9(4) COMP-5.
      * Global database name
           05 SQL-GLBDBNAME-N        PIC X(255).
      * dce principal bin string
           05 SQL-DCEPRINCIPAL-N     PIC X(1024).
           05 FILLER                 PIC X(1).
      * Catalog node number
           05 SQL-CAT-NODENUM-N      PIC S9(4) COMP-5.
      * Node number
           05 SQL-NODENUM-N          PIC S9(4) COMP-5.
      * Alternate hostname
           05 SQL-ALTHOSTNAME-N      PIC X(255).
      * Port number
           05 SQL-ALTPORTNUMBER-N    PIC X(14).
           05 FILLER                 PIC X(1).

      *************************************************************************
      * Get Next Database Directory Entry Parameter Structure 
      **************************************************************************
      *************************************************************************
      * Get Next Database Directory Entry API 
      **************************************************************************
      *************************************************************************
      * Generic Get Next Database Directory Entry Parameter Structure 
      **************************************************************************
       01 DB2G-DB-DIR-NEXT-ENTRY-STRUCT.
      * Input: handle
           05 DB2-I-HANDLE           PIC 9(4) COMP-5.
           05 FILLER                 PIC X(2).
      * Output: buffer
           05 DB2-PO-DB-DIR-ENTRY    USAGE IS POINTER.

      *************************************************************************
      * Generic Get Next Database Directory Entry API 
      **************************************************************************

      *************************************************************************
      * Close Database Directory Scan Parameter Structure 
      **************************************************************************
      *************************************************************************
      * Close Database Directory Scan API 
      **************************************************************************
      *************************************************************************
      * Generic Close Database Directory Scan Parameter Structure 
      **************************************************************************
       01 DB2G-DB-DIR-CLOSE-SCAN-STRUCT.
      * Input: handle
           05 DB2-I-HANDLE           PIC 9(4) COMP-5.

      *************************************************************************
      * Generic Close Database Directory Scan API 
      **************************************************************************

      *************************************************************************
      * db2QpGetUserInformation
      **************************************************************************



