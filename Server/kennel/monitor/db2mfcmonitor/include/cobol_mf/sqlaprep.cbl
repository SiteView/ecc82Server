      *************************************************************************
      * 
      * Source File Name: SQLAPREP
      * 
      * (C) COPYRIGHT International Business Machines Corp. 1988, 1997
      * All Rights Reserved
      * Licensed Materials - Property of IBM
      * 
      * US Government Users Restricted Rights - Use, duplication or
      * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
      * 
      * Function = Include File defining:
      *              Global equates, structures, and prototypes for
      *              Precompiler and Runtime Command Interfaces
      * 
      * Operating System =  Windows NT
      * 
      **************************************************************************
      * Values used for option[n].type, an input parameter of SQLAINIT.
      * Option: date/time format
       78  SQLA-FORMAT               value    1.
      * Option: access plan creation
       78  SQLA-ACCESS-PLAN          VALUE    2.
      * Option: bind file creation
       78  SQLA-BIND-FILE            VALUE    3.
      * Option: isolation level
       78  SQLA-ISOLATION            value    4.
      * Option: record blocking
       78  SQLA-BLOCK                value    5.
      * Flagger option
       78  SQLA-FLAG-OPT             value    8.
      * Option: standards compliance
       78  SQLA-STDS-LEVEL           VALUE    7.
      * C NULL required option
       78  SQLA-CNULREQD-OPT         value    11.
      * Generic option
       78  SQLA-GENERIC-OPT          value    12.
      * Option for Deferred Prepare
       78  SQLA-DEFERRED-PREPARE-OPT value    15.
      * Level of a module
       78  SQLA-LEVEL-OPT            value    30.
      * Package collection id
       78  SQLA-COLLECTION-OPT       value    31.
      * Package version id
       78  SQLA-VERSION-OPT          value    32.
      * Package owner auth id
       78  SQLA-OWNER-OPT            value    33.
      * Synonym for owner
       78  SQLA-SCHEMA-OPT           value    33.
      * Authorization identifier that is to be used as qualifier for
      * unqualified objects
       78  SQLA-QUALIFIER-OPT        value    34.
      * Synonym for qualifier.
       78  SQLA-CATALOG-OPT          value    34.
      * Package description
       78  SQLA-TEXT-OPT             value    35.
      * Indicates when object validation occurs
       78  SQLA-VALIDATE-OPT         value    40.
      * Determines whether information will be produced about how the SQL
      * statements in a package will be executed
       78  SQLA-EXPLAIN-OPT          value    41.
      * Indicates whether a package is to be added or replaced
       78  SQLA-ACTION-OPT           value    42.
      * Indicates under what conditions a package will be created
       78  SQLA-SQLERROR-OPT         value    25.
      * Replaces a specific version of a package
       78  SQLA-REPLVER-OPT          value    44.
      * Indicates whether EXECUTE authorities are to be preserved when a
      * package is replaced
       78  SQLA-RETAIN-OPT           value    45.
      * Indicates whether resources are released at each COMMIT or when the
      * application terminates
       78  SQLA-RELEASE-OPT          value    46.
      * Specifies whether or not the query is executed using I/O parallelism
       78  SQLA-DEGREE-OPT           value    47.
      * Designates whether an apostrophe or quote will be used as a string
      * delimiter
       78  SQLA-STRDEL-OPT           value    50.
      * Designates whether a period or comma will be used as a decimal point
      * indicator
       78  SQLA-DECDEL-OPT           value    51.
      * Designates default character subtype that is to be used for column
      * definitions in the CREATE and ALTER TABLE SQL statements
       78  SQLA-CHARSUB-OPT          value    55.
      * Designates what CCSID will be used for single byte characters for
      * character column definitions without a specific CCSID clause in the
      * CREATE and ALTER TABLE SQL statements
       78  SQLA-CCSIDS-OPT           value    56.
      * Designates what CCSID will be used for mixed byte characters for
      * character column definitions without a specific CCSID clause in the
      * CREATE and ALTER TABLE SQL statements
       78  SQLA-CCSIDM-OPT           value    57.
      * Designates what CCSID will be used for double byte characters for
      * character column definitions without a specific CCSID clause in the
      * CREATE and ALTER TABLE SQL statements
       78  SQLA-CCSIDG-OPT           value    58.
      * Specifies the maximum precision to be used in decimal arithmetic
      * operations
       78  SQLA-DEC-OPT              value    59.
      * Specifies whether one or multiple connections can exist at the same
      * time.
       78  SQLA-CONNECT-OPT          value    16.
      * Specifies the set of rules used for connection to multiple databases
      * within a single unit of work
       78  SQLA-SQLRULES-OPT         value    17.
      * Specifies which databases disconnected when a COMMIT or ROLLBACK is
      * issued.
       78  SQLA-DISCONNECT-OPT       value    18.
      * Specifies what syncpoint option (for example one or two phase) will
      * be used.
       78  SQLA-SYNCPOINT-OPT        value    19.
      * Specifies which authorization identifier to use when dynamic SQL in
      * a package is executed.
       78  SQLA-DYNAMICRULES-OPT     value    61.
      * Buffers VALUE inserts - bind and precompile option for DB2/PE
      * servers only
       78  SQLA-INSERT               value    62.
      * Capture explain snapshot - bind and precompile option
       78  SQLA-EXPLSNAP-OPT         value    63.
      * Path for user-defined function resolution - bind and precompile
      * option
       78  SQLA-FUNCTION-PATH        value    64.
      * Disable prepare-time SQL warnings - bind and precompile option
       78  SQLA-SQLWARN-OPT          value    65.
      * Set query optimization class -bind and precompile option
       78  SQLA-QUERYOPT-OPT         value    66.
      * Target compiler -precompile option
       78  SQLA-TARGET-OPT           value    67.
      * Name of precompiler output file - precompile option
       78  SQLA-PREP-OUTPUT-OPT      value    68.
      * Internal use only
       78  SQLA-TOKEN-USE-INITIALIZED-OPT value 1000.
      * Internal use only
       78  SQLA-USE-LONG-LABELS      value    1001.
      * Call resolution option -precompile option
       78  SQLA-CALL-RES-OPT         value    85.
      * If static cursors are to be treated as READONLY
       78  SQLA-STATICREADONLY-OPT   value    87.
      * The optimization profile name
       78  SQLA-OPTPROFILE-OPT       value    88.

      * Values used for option[n].val when option[n].type is SQLA_FORMAT in
      * the Precompiler Option Array.
      * Default date/time format
       78  SQLA-POA-DEF              VALUE    0.
      * USA date/time format
       78  SQLA-POA-USA              VALUE    1.
      * European date/time format
       78  SQLA-POA-EUR              VALUE    2.
      * International date/time format
       78  SQLA-POA-ISO              VALUE    3.
      * Japanese date/time format
       78  SQLA-POA-JIS              VALUE    4.
      * Local date/time format
       78  SQLA-POA-LOC              VALUE    5.

      * Values used for option[n].val when option[n].type is
      * SQLA_ACCESS_PLAN in
      * the Precompiler Option Array.
      * Create an access plan
       78  SQLA-CREATE-PLAN          VALUE    1.
      * Do not create an access plan
       78  SQLA-NO-PLAN              VALUE    0.
      * Set ocb->syntax to TRUE
       78  SQLA-NO-PLAN-SYNTAX       VALUE    3.

      * Values used for option[n].val when option[n].type is SQLA_BIND_FILE
      * in
      * the Precompiler Option Array.
      * Create a bind file
       78  SQLA-CREATE-BIND-FILE     VALUE    1.
      * Do not create a bind file
       78  SQLA-NO-BIND-FILE         VALUE    0.

      * Values used for option[n].val when option[n].type is SQLA_ISOLATION
      * in
      * the Precompiler Option Array.
      * RR isolation level
       78  SQLA-REPEATABLE-READ      value    0.
      * CS Isolation level
       78  SQLA-CURSOR-STABILITY     value    1.
      * UR Isolation level
       78  SQLA-UNCOMMITTED-READ     value    2.
      * RS Isolation level
       78  SQLA-READ-STABILITY       value    3.
      * NC Isolation level
       78  SQLA-NO-COMMIT            value    4.

      * Values used for option[n].val when option[n].type is SQLA_BLOCK in
      * the Precompiler Option Array.
       78  SQLA-BLOCK-UNAMBIG        value    0.
       78  SQLA-BLOCK-ALL            value    1.
       78  SQLA-NO-BLOCK             value    2.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_LEVEL_OPT
       78  SQLA-MAX-LEVEL            value    8.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_COLLECTION_OPT
       78  SQLA-MAX-COLLECTION       value    30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_VERSION_OPT
       78  SQLA-MAX-VERSION          value    254.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_OWNER_OPT
       78  SQLA-MAX-OWNER            value    30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_SCHEMA_OPT
       78  SQLA-MAX-SCHEMA           value    30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_QUALIFIER_OPT
       78  SQLA-MAX-QUALIFIER        value    30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_CATALOG_OPT
       78  SQLA-MAX-CATALOG          value    30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_TEXT_OPT
       78  SQLA-MAX-TEXT             value    255.

      * Values used for option[n].val when option[n].type is
      * SQLA_VALIDATE_OPT.
      * Validate objects during BIND
       78  SQLA-VALIDATE-BIND        value    0.
      * Validate objects during execution
       78  SQLA-VALIDATE-RUN         value    1.

      * Values used for option[n].val when option[n].type is
      * SQLA_EXPLAIN_OPT.
      * No EXPLAIN output produced
       78  SQLA-EXPLAIN-NO           value    0.
      * EXPLAIN output produced
       78  SQLA-EXPLAIN-YES          value    1.
      * EXPLAIN output produced for all static and dynamic statements.
       78  SQLA-EXPLAIN-ALL          value    2.
      * EXPLAIN output produced for static reoptimizable statements.
       78  SQLA-EXPLAIN-REOPT        value    3.

      * Values used for option[n].val when option[n].type is
      * SQLA_ACTION_OPT.
      * Package is added
       78  SQLA-ACTION-ADD           value    0.
      * Package is replaced
       78  SQLA-ACTION-REPLACE       value    1.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_REPLVER_OPT
       78  SQLA-MAX-REPLVER          value    254.

      * Values used for option[n].val when option[n].type is
      * SQLA_SQLERROR_OPT.
      * Do not create a package if errors are encountered
       78  SQLA-SQLERROR-NOPACKAGE   value    0.
      * Do not create a package
       78  SQLA-SQLERROR-CHECK       value    1.
      * Create a package even if errors are encountered
       78  SQLA-SQLERROR-CONTINUE    value    2.

      * Values used for option[n].val when option[n].type is
      * SQLA_RETAIN_OPT.
      * Do not preserve EXECUTE authorities when a package is replaced
       78  SQLA-RETAIN-NO            value    0.
      * Preserve EXECUTE authorities when a package is replaced
       78  SQLA-RETAIN-YES           value    1.

      * Values used for option[n].val when option[n].type is
      * SQLA_RELEASE_OPT.
      * Release resources at COMMIT
       78  SQLA-RELEASE-COMMIT       value    0.
      * Release resources when a program terminates
       78  SQLA-RELEASE-DEALLOCATE   value    1.

      * Values used for option[n].val when option[n].type is
      * SQLA_STRDEL_OPT.
      * Apostrophe string delimiter
       78  SQLA-STRDEL-APOSTROPHE    value    0.
      * Quote string delimiter
       78  SQLA-STRDEL-QUOTE         value    1.

      * Values used for option[n].val when option[n].type is
      * SQLA_DECDEL_OPT.
      * Period is used as a decimal point indicator in decimal and floating
      * point literals
       78  SQLA-DECDEL-PERIOD        value    0.
      * Comma is used as a decimal point indicator in decimal and floating
      * point literals
       78  SQLA-DECDEL-COMMA         value    1.

      * Values used for option[n].val when option[n].type is
      * SQLA_CHARSUB_OPT.
      * Use the target system defined default for all new character columns
      * for which an explicit subtype is not specified
       78  SQLA-CHARSUB-DEFAULT      value    0.
      * Use the bit character subtype for all new character columns for
      * which an explicit subtype is not specified
       78  SQLA-CHARSUB-BIT          value    1.
      * Use the SBCS character subtype for all new character columns for
      * which an explicit subtype is not specified
       78  SQLA-CHARSUB-SBCS         value    2.
      * Use the mixed character subtype for all new character columns for
      * which an explicit subtype is not specified
       78  SQLA-CHARSUB-MIXED        value    3.

      * Values used for option[n].val when option[n].type is SQLA_DEC_OPT.
      * 15 bit precision is used in decimal arithmetic operations
       78  SQLA-DEC-15               value    15.
      * 31 bit precision is used in decimal arithmetic operations
       78  SQLA-DEC-31               value    31.

      * Values used for option[n].val when option[n].type is
      * SQLA_DEGREE_OPT.
      * Prohibits parallel I/O operations
       78  SQLA-DEGREE-1             value    1.
      * Allows the target database system to determine the degree of
      * parallel I/O operations
       78  SQLA-DEGREE-ANY           value    0.
      * Maximum value
       78  SQLA-MAX-DEGREE-VAL       value    32767.

      * Values used for option[n].val when option[n].type is
      * SQLA_CNULREQD_OPT in the Precompiler Option array
      * C NULL is not required
       78  SQLA-CNULREQD-NO          value    0.
      * C NULL is not required
       78  SQLA-CNULREQD-YES         value    1.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_GENERIC_OPT
       78  SQLA-MAX-GENERIC          value    1023.

      * Values used for option[n].val when option[n].type is
      * SQLA_DEFERRED_PREPARE_OPT.
      * Enable Deferred Prepare except for a PREPARE...INTO statement which
      * contains parameter markers.
       78  SQLA-DEFERRED-PREPARE-YES value    1.
      * Disable Deferred Prepare
       78  SQLA-DEFERRED-PREPARE-NO  value    2.
      * Same as YES except that a PREPARE...INTO which contains parameter
      * markers will also be deferred
       78  SQLA-DEFERRED-PREPARE-ALL value    3.

      * Values used for option[n].val when option[n].type is
      * SQLA_CONNECT_OPT.
      * Indicates that only one connection to a database can exist at any
      * given time.
       78  SQLA-CONNECT-1            value    1.
      * Indicates that multiple connections can exist simultaneously, with
      * one being active and the others dormant.
       78  SQLA-CONNECT-2            value    2.

      * Values used for option[n].val when option[n].type is SQLA_RULES_OPT.
      * Indicates that CONNECT TO can be used to make a dormant connection
      * the current connection.
       78  SQLA-RULES-DB2            value    1.
      * Indicates that CONNECT TO is not valid for making a dormant
      * connection current,and SET CONNECTION must be used instead.
       78  SQLA-RULES-STD            value    2.
      * Values used for option[n].val when option[n].type is
      * SQLA_DISCONNECT_OPT.
      * Indicates that all connections marked by the RELEASE statement will
      * be the only connections released when a COMMIT or ROLLBACK is
      * issued.
       78  SQLA-DISCONNECT-EXPL      value    1.
      * Indicates that all connections which don't have open WITH HOLD
      * cursors will be the only connections released when a COMMIT or
      * ROLLBACK is issued.
       78  SQLA-DISCONNECT-COND      value    2.
      * Indicates that all connections will be released when a COMMIT or
      * ROLLBACK is issued.
       78  SQLA-DISCONNECT-AUTO      value    3.

      * Values used for option[n].val when option[n].type is
      * SQLA_SYNCPOINT_OPT.
      * Do not use a transaction manager to perform two phase commit,but
      * enforce that there is only one database is updated when multiple
      * databases are accessed within a single transaction.
       78  SQLA-SYNC-ONEPHASE        value    1.
      * Use a transaction manager to coordinate two phase commit.
       78  SQLA-SYNC-TWOPHASE        value    2.
      * No two-phase commit and no single updater enforcement.
       78  SQLA-SYNC-NONE            value    0.

      * Values used for option[n].val when option[n].type is
      * SQLA_DYNAMICRULES_OPT.
      * Dynamic SQL in package will use authid of person running the package
       78  SQLA-DYNAMICRULES-RUN     value    0.
      * Dynamic SQL in package will use authid of person who owns the
      * package
       78  SQLA-DYNAMICRULES-BIND    value    1.
      * Dynamic SQL in UDF or stored procedure will use authid of invoker of
      * UDF or stored procedure
       78  SQLA-DYNAMICRULES-INVOKE  value    2.
      * Dynamic SQL in UDF or stored procedure will use authid of definer of
      * UDF or stored procedure
       78  SQLA-DYNAMICRULES-DEFINE  value    3.
      * Dynamic SQL in routine will use authid of definer of routine
       78  SQLA-DYNAMICRULES-DEFINEBIND value 5.
      * Dynamic SQL in routine will use authid of definer of routine 
       78  SQLA-DYNAMICRULES-DEFINERUN value  3.
      * Dynamic SQL in routine will use authid of invoker of routine
       78  SQLA-DYNAMICRULES-INVOKEBIND value 4.
      * Dynamic SQL in routine will use authid of invoker of routine 
       78  SQLA-DYNAMICRULES-INVOKERUN value  2.

      * Values used for option[n].val when option[n].type is SQLA_FLAG_OPT
      * inthe Precompiler Option Array.
       78  SQLA-DB2V23-SYNTAX        value    4.
       78  SQLA-DB2V31-SYNTAX        value    5.
       78  SQLA-DB2V41-SYNTAX        value    6.
       78  SQLA-FIPS-SYNTAX          value    7.

      * Values used for option[n].val when option[n].type is
      * SQLA_EXPLSNAP_OPT in the Precompiler Option array
      * Do not save explain snapshot
       78  SQLA-EXPLSNAP-NO          value    0.
      * Save explain snapshot
       78  SQLA-EXPLSNAP-YES         value    1.
      * Save explain snapshot for all static and dynamic statements
       78  SQLA-EXPLSNAP-ALL         value    2.
      * Save explain snapshot for static reoptimizable statements
       78  SQLA-EXPLSNAP-REOPT       value    3.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_FUNCTION_PATH
       78  SQLA-MAX-FUNCPATH         value    254.

      * Values used for option[n].val when option[n].type is
      * SQLA_SQLWARN_OPT in the Precompiler Option array
      * Suppress prepare-time warning SQLCODEs
       78  SQLA-SQLWARN-NO           value    0.
      * Permit prepare-time warning SQLCODEs
       78  SQLA-SQLWARN-YES          value    1.

      * Values used for option[n].val when option[n].type is
      * SQLA_QUERYOPT_OPT in the Precompiler Option array
      * Class 0 query optimization
       78  SQLA-QUERYOPT-0           value    0.
      * Class 1 query optimization
       78  SQLA-QUERYOPT-1           value    1.
      * Class 2 query optimization
       78  SQLA-QUERYOPT-2           value    2.
      * Class 3 query optimization
       78  SQLA-QUERYOPT-3           value    3.
      * Class 5 query optimization
       78  SQLA-QUERYOPT-5           value    5.
      * Class 7 query optimization
       78  SQLA-QUERYOPT-7           value    7.
      * Class 9 query optimization
       78  SQLA-QUERYOPT-9           value    9.

      * Values used for option[n].val when option[n].type is SQLA_INSERT_OPT
      * Do not buffer VALUE inserts
       78  SQLA-INSERT-DEF           value    0.
      * Buffer VALUE inserts
       78  SQLA-INSERT-BUF           value    1.

      * Maximum target string length in option[n].val when option[n].type is
      * SQLA_TARGET_OPT
       78  SQLA-MAX-TARGET-LEN       value    32.

      * Values used for option[n].val when option[n].type is SQLA_STDS_LEVEL
      * inthe Precompiler Option Array.
      * No compliance required
       78  SQLA-STND-NONE            VALUE    0.
      * Comply with MIA standard
       78  SQLA-STND-MIA             VALUE    1.
      * Comply with SQL92E standard
       78  SQLA-STND-SQL92E          VALUE    2.

      * Values used for option[n].val when option[n].type is
      * SQLA_STATICREADONLY_OPT in the Precompiler Option Array.
      * Static cursors take on attributes as would normally be generated
      * given the statement text and the LANGLEVEL option
       78  SQLA-STATICRO-NO          value    0.
      * Any static cursor that does not contain the FOR UPDATE or FOR READ
      * ONLY clause will be considered READ ONLY.
       78  SQLA-STATICRO-YES         value    1.
      * Values used for location, an input parameter of SQLAAHVR.
      * Add host var found in a declare section
       78  SQLA-DECLARE-SECT         VALUE    0.
      * Add host var found in an SQL statement
       78  SQLA-SQL-STMT             VALUE    1.

      * Values assigned to task[n].func, an output parameter of SQLACMPL.
      * Generate a call to sqlastrt
       78  SQLA-START                VALUE    0.
      * Begin or end parsing hvar declarations
       78  SQLA-DECLARE              VALUE    1.
      * Generate code for an SQLCA or SQLDA
       78  SQLA-INCLUDE              VALUE    2.
      * Allocate an input SQLDA using SQLAALOC
       78  SQLA-ALLOC-INPUT          VALUE    3.
      * Allocate an output SQLDA using SQLAALOC
       78  SQLA-ALLOC-OUTPUT         VALUE    4.
      * Register a host variable using SQLASETS
       78  SQLA-SETS                 VALUE    5.
      * Register an input user-defined SQLDA
       78  SQLA-USDA-INPUT           VALUE    6.
      * Register an output user-defined SQLDA
       78  SQLA-USDA-OUTPUT          VALUE    7.
      * Generate a call to SQLACALL
       78  SQLA-CALL                 VALUE    8.
      * Generate a call to SQLADLOC
       78  SQLA-DEALLOC              VALUE    9.
      * Generate a call to SQLASTOP
       78  SQLA-STOP                 VALUE    10.
      * Generate code for WHENEVER SQLERROR
       78  SQLA-SQLERROR             VALUE    11.
      * Generate code for WHENEVER SQLWARNING
       78  SQLA-SQLWARNING           VALUE    12.
      * Generate code for WHENEVER NOT_FOUND
       78  SQLA-NOT-FOUND            VALUE    13.
      * Process an INCLUDE text-file stmt
       78  SQLA-INC-TEXTFILE         VALUE    14.
      * Begin Compound SQL block
       78  SQLA-BEGIN-COMPOUND       VALUE    15.
      * Generate a call to SQLACMPD
       78  SQLA-CMPD                 VALUE    16.
      * Generate a conditional branch around subsequent Compound SQL
      * substatements.
       78  SQLA-CMPD-TEST            VALUE    17.
      * Generate a target label for gotos from cmpd_test (optional)
       78  SQLA-CMPD-MARK            VALUE    18.
      * Request the outer precompiler to get the next triggered SQL
      * statement
       78  SQLA-NEXT-SUBSTATEMENT    VALUE    19.
      * Generate code to copy SQLCODE/SQLSTATE
       78  SQLA-SQLCODE-COPY         VALUE    20.
      * Request the outer precompiler to get the next stored procedure SQL
      * statement
       78  SQLA-NEXT-STOREDPROC-STMT VALUE    21.
      * Generate code for ASSOCIATE LOCATOR 
       78  SQLA-ASSOCIATE-LOCATOR    VALUE    22.
      * Save last called procedure informationor generate code for ASSOCIATE
      * LOCATOR referring to the last called procedure
       78  SQLA-ASSOCIATE-PREV-PROC  VALUE    23.
      * Generate code for ALLOCATE CURSOR
       78  SQLA-ALLOCATE-CURSOR      VALUE    24.

      * Values returned in task[n].val when task[n].func is SQLA_DECLARE.
      * This is an output parameter of SQLACMPL.
       78  SQLA-BEGIN                VALUE    0.
       78  SQLA-END                  VALUE    1.

      * Values returned in task[n].val when task[n].func is SQLA_INCLUDE.
      * This is an output parameter of SQLACMPL.
       78  SQLA-SQLCA                VALUE    10.
       78  SQLA-SQLDA                VALUE    11.

      * Values returned in task[n].val when task[n].func is SQLA_CALL.
      * This is an output parameter of SQLACMPL.
       78  SQLA-CLOSE                VALUE    20.
       78  SQLA-COMMIT               VALUE    21.
       78  SQLA-DESCRIBE             VALUE    22.
       78  SQLA-EXECIMMD             VALUE    23.
       78  SQLA-EXECUTE              VALUE    24.
       78  SQLA-FETCH                VALUE    25.
       78  SQLA-OPEN                 VALUE    26.
       78  SQLA-PREPARE              VALUE    27.
       78  SQLA-ROLLBACK             VALUE    28.
       78  SQLA-CONNECT              VALUE    29.
      * Atomic Compound SQL call
       78  SQLA-CMPDEXEC             VALUE    30.
      * SQL free locators call
       78  SQLA-FREE-LOB-LOCATORS    VALUE    37.
      * Not Atomic Compound SQL call
       78  SQLA-CMPDEXEC-NA          VALUE    38.
       78  SQLA-DUOW                 VALUE    40.
      * Close Cursor with Release
       78  SQLA-CLOSE-REL            VALUE    41.
      * SP SQL Deferred call
       78  SQLA-CALLPROC             VALUE    42.
      * SP Deferred call
       78  SQLA-CALLPROC-DEFERRED    VALUE    42.
      * Set Current Packageset
       78  SQLA-SET-CURRENT-PKGSET   VALUE    43.
       78  SQLA-EXECIMMD-ASYNC       VALUE    44.
       78  SQLA-EXECUTE-ASYNC        VALUE    45.
       78  SQLA-FETCH-ASYNC          VALUE    46.
       78  SQLA-OPEN-ASYNC           VALUE    47.
       78  SQLA-CALLPROC-ASYNC       VALUE    48.
       78  SQLA-CMPDEXEC-ASYNC       VALUE    49.
       78  SQLA-DESCRIBE-ASYNC       VALUE    50.
       78  SQLA-PREPARE-ASYNC        VALUE    51.
       78  SQLA-EXECIMMD-ASYNC-COMMIT VALUE   52.
       78  SQLA-EXECUTE-ASYNC-COMMIT VALUE    53.
       78  SQLA-CMPDEXEC-ASYNC-COMMIT VALUE   54.
       78  SQLA-EXECIMMD-COMMIT      VALUE    55.
       78  SQLA-EXECUTE-COMMIT       VALUE    56.
       78  SQLA-CMPDEXEC-COMMIT      VALUE    57.
       78  SQLA-CMPDEXEC-NA-ASYNC    VALUE    58.
       78  SQLA-CMPDEXEC-NA-COMMIT   VALUE    59.
       78  SQLA-CMPDEXEC-NA-DIAG     VALUE    60.
       78  SQLA-CMPDEXEC-NA-ASYNC-COMMIT VALUE 61.
       78  SQLA-CMPDEXEC-NA-ASYNC-DIAG VALUE  62.
       78  SQLA-CMPDEXEC-NA-COMMIT-DIAG VALUE 63.
       78  SQLA-CMPDEXEC-NA-A-C-D    VALUE    64.
       78  SQLA-CLOSE-OPEN           VALUE    65.
       78  SQLA-CLOSE-OPEN-ASYNC     VALUE    66.
       78  SQLA-CLOSEREL-OPEN-ASYNC  VALUE    67.
       78  SQLA-CLOSEREL-OPEN        VALUE    68.
       78  SQLA-SETOPTION            VALUE    69.
       78  SQLA-DESCRIBE-CURSOR      VALUE    70.
       78  SQLA-DESCRIBE-CURSOR-ASYNC VALUE   71.
       78  SQLA-ARRAYINPUT           VALUE    72.
       78  SQLA-ARRAYINPUT-ASYNC     VALUE    73.
       78  SQLA-GETINFO              VALUE    74.
       78  SQLA-OPEN-WITH-RETURN     VALUE    75.
       78  SQLA-OPEN-WITH-RET-TO-CLIENT VALUE 76.
       78  SQLA-CALLPROC-NEW         VALUE    77.
       78  SQLA-CALLPROC-IMMED       VALUE    77.
       78  SQLA-DESCRIBE-INPUT       VALUE    78.
       78  SQLA-PREP-DESCRIBE-INPUT  VALUE    79.
       78  SQLA-EXECUTE-FREE-LOB-LOCATORS VALUE 80.
      * Set Current Package Path
       78  SQLA-SET-CURRENT-PKG-PATH VALUE    81.
      * The following two constants are for internal use only.  Do not use
      * them as values for sqlacall or similar API calls.
       78  SQLA-ALLOCATE             VALUE    82.
       78  SQLA-ASSOCIATE            VALUE    83.

      * Values returned in task[n].val when task[n].func is
      * SQLA_SQLCODE_COPY.
      * This is an output parameter of SQLACMPL.
      * Copy SQLCODE and/or SQLSTATE
       78  SQLA-SQLCODE              VALUE    1.
      * Copy PREP errors
       78  SQLA-SQLCODE-PREP-ERROR   VALUE    2.

      * Values assigned to token[n].use, an output parameter of SQLACMPL.
      * Input host var, no indicator var
       78  SQLA-INPUT-HVAR           VALUE    0.
      * Input host var followed by indicator var
       78  SQLA-INPUT-WITH-IND       VALUE    1.
      * Output host var, no indicator var
       78  SQLA-OUTPUT-HVAR          VALUE    2.
      * Output host var followed by indicator var
       78  SQLA-OUTPUT-WITH-IND      VALUE    3.
      * Indicator variable
       78  SQLA-INDICATOR            VALUE    4.
      * SQLDA structure
       78  SQLA-USER-SQLDA           VALUE    5.
      * Host variable type does not match use
       78  SQLA-INVALID-USE          VALUE    6.
      * Host variable token ID is not valid
       78  SQLA-INVALID-ID           VALUE    7.
      * Literal string
       78  SQLA-LITERAL              VALUE    8.
      * Host var was one of several fields expanded from a structure
      * reference
       78  SQLA-MULTIPLE-STRUCT-FIELD VALUE   9.
      * Host var was either atomic or a single field from a structure
      * reference
       78  SQLA-ATOMIC-FIELD         VALUE    10.

      * Values assigned to stmt_type, an output parameter of SQLACMPL.
       78  SQLA-TYPE-NULL-CONNECT    VALUE    0.
       78  SQLA-TYPE-CONN-EXCL-NO-PWD VALUE   1.
       78  SQLA-TYPE-CONN-EXCL-PWD   VALUE    2.
       78  SQLA-TYPE-CONN-EXCL-NO-PWD-SN VALUE 91.
       78  SQLA-TYPE-CONN-EXCL-PWD-SN VALUE   92.
       78  SQLA-TYPE-CONNECT-RESET   VALUE    3.
       78  SQLA-TYPE-CONN-SHARE-NO-PWD VALUE  4.
       78  SQLA-TYPE-CONN-SHARE-PWD  VALUE    5.
       78  SQLA-TYPE-CONNECT-USERID  VALUE    6.
       78  SQLA-TYPE-BEGIN           VALUE    10.
       78  SQLA-TYPE-BEGIN-COMPOUND  VALUE    12.
       78  SQLA-TYPE-CALLPROC        VALUE    13.
       78  SQLA-TYPE-CLOSE           VALUE    15.
       78  SQLA-TYPE-COMMIT          VALUE    20.
       78  SQLA-TYPE-CONNECT-1       VALUE    21.
       78  SQLA-TYPE-CONNECT-2       VALUE    22.
       78  SQLA-TYPE-DISCONNECT-DBNAME VALUE  23.
       78  SQLA-TYPE-DISCONNECT-CURRENT VALUE 24.
       78  SQLA-TYPE-DISCONNECT-ALL  VALUE    25.
       78  SQLA-TYPE-RELEASE-DBNAME  VALUE    26.
       78  SQLA-TYPE-RELEASE-CURRENT VALUE    27.
       78  SQLA-TYPE-RELEASE-ALL     VALUE    28.
       78  SQLA-TYPE-SET-CONNECTION  VALUE    29.
       78  SQLA-TYPE-DECLARE-SELECT  VALUE    30.
       78  SQLA-TYPE-DECLARE-SNAME   VALUE    40.
       78  SQLA-TYPE-ALLOCATE-CURSOR VALUE    45.
       78  SQLA-TYPE-DESCRIBE        VALUE    50.
       78  SQLA-TYPE-DESCRIBE-CURSOR VALUE    55.
       78  SQLA-TYPE-END             VALUE    60.
       78  SQLA-TYPE-END-COMPOUND    VALUE    65.
       78  SQLA-TYPE-EXECUTE         VALUE    70.
       78  SQLA-TYPE-EXECUTE-IMM     VALUE    80.
       78  SQLA-TYPE-FETCH           VALUE    90.
       78  SQLA-TYPE-FREE-LOB-LOCATOR VALUE   95.
       78  SQLA-TYPE-INCLUDE-SQLCA   VALUE    100.
       78  SQLA-TYPE-INCLUDE-SQLDA   VALUE    110.
       78  SQLA-TYPE-INCLUDE-TEXTFILE VALUE   115.
       78  SQLA-TYPE-OPEN            VALUE    120.
       78  SQLA-TYPE-PREPARE         VALUE    130.
       78  SQLA-TYPE-ROLLBACK        VALUE    140.
       78  SQLA-TYPE-SELECT          VALUE    150.
       78  SQLA-TYPE-SELECT-INTO     VALUE    160.
       78  SQLA-TYPE-SET-CURRENT-PKGSET VALUE 163.
       78  SQLA-TYPE-WHEN-NOT-FOUND  VALUE    170.
       78  SQLA-TYPE-WHEN-SQLERROR   VALUE    180.
       78  SQLA-TYPE-WHEN-SQLWARNING VALUE    190.
       78  SQLA-TYPE-SET-CURRENT-PKG-PATH VALUE 195.
       78  SQLA-TYPE-OTHER           VALUE    200.

      * Values used for term_option, an input parameter of SQLAFINI.
      * Save the access plan and/or bind file
       78  SQLA-SAVE                 VALUE    1.
      * Discard the access plan and/or bind file
       78  SQLA-DISCARD              VALUE    0.

      * Miscellaneous Constants

      * size of SQLAINIT file name buffer
       78  SQL-MAX-FILE-NAME-LENGTH  VALUE    128.

      * size of SQLACMPL output buffers
       78  SQL-CMPL-BUFSIZ           VALUE    256.
      * length of precompiler pid for V6.1 or earlier
       78  SQL-PID-LENGTH-V610       VALUE    40.
      * length of precompiler pid
       78  SQL-PID-LENGTH            VALUE    162.
      * minimum length of precompiler pid
       78  SQL-MIN-PID-LENGTH        VALUE    34.
      * Message array length same as SQLH_MAXMSGS
       78  SQLA-FLAG-MAXMSGS         VALUE    10.
      * Current SQL Flagger version
       78  SQLA-FLAG-VERSION         VALUE    1.

      * Return codes from Precompiler Services and Runtime Services
      * functions.
      * sqlca pointer/length invalid
       78  SQLA-SQLCA-BAD            VALUE    -1.
      * check sqlca completion status
       78  SQLA-CHECK-SQLCA          VALUE    0.

      * Values returned in sqlca.sqlcode by Precompiler or Runtime Services
       78  SQLA-RC-OK                value    0.
       78  SQLA-RC-OPTION-IGNORED    VALUE    20.
       78  SQLA-RC-CHAR-BAD          value    -7.
       78  SQLA-RC-STRING-NOT-TERMINATED value -10.
       78  SQLA-RC-EMPTY-DEL-IDENT   value    -13.
       78  SQLA-RC-BFILE-OPEN-ERROR  VALUE    -31.
       78  SQLA-RC-BFILE-DISK-ERROR  VALUE    -32.
       78  SQLA-RC-SECTION-LIMIT     VALUE    -51.
       78  SQLA-RC-MEMORY-BAD        VALUE    -83.
       78  SQLA-RC-SNAME-DUP         VALUE    -85.
       78  SQLA-RC-NO-STRUCT         VALUE    -87.
       78  SQLA-RC-AMBIG-HOSTVAR     VALUE    -88.
       78  SQLA-RC-STMT-LIMIT        value    -101.
       78  SQLA-RC-NUMBER-BAD        value    -103.
       78  SQLA-RC-STMT-SYNTAX-BAD   value    -104.
      * DBCS only
       78  SQLA-RC-GSTRING-BAD       value    -105.
       78  SQLA-RC-IDENTIFIER-LIMIT  value    -107.
       78  SQLA-RC-QUALIFIER-BAD     value    -108.
       78  SQLA-RC-DDSBAD            value    -142.
       78  SQLA-RC-DDSIGN            value    143.
       78  SQLA-RC-DDSWRN            value    143.
       78  SQLA-RC-STMT-IGNORED      value    143.
       78  SQLA-RC-KEYWORD-BAD       value    -199.
       78  SQLA-RC-HVAR-NOT-DEC      VALUE    -306.
       78  SQLA-RC-HVAR-DUP-NAME     VALUE    -307.
       78  SQLA-RC-HVAR-LIMIT        VALUE    -308.
       78  SQLA-RC-STMT-HVAR-LIMIT   VALUE    -310.
       78  SQLA-RC-HVAR-USE-BAD      VALUE    -324.
       78  SQLA-RC-CURSOR-DUP        VALUE    -505.
       78  SQLA-RC-STMT-MODIFY-ALL   VALUE    513.
       78  SQLA-RC-INVALID-TRIGGER-STMT value -797.
       78  SQLA-RC-SQLDA-SQLD-ERR    value    -804.
       78  SQLA-RC-SQLVAR-TYPE-ERR   value    -804.
       78  SQLA-RC-DISK-FULL         value    -968.
       78  SQLA-RC-NODE-FAILURE      value    -1229.
       78  SQLA-RC-CMPD-NESTED       VALUE    -4010.
       78  SQLA-RC-CMPD-INVALID-STMT VALUE    -4011.
       78  SQLA-RC-CMPD-INVALID-COMMIT VALUE  -4012.
       78  SQLA-RC-CMPD-INVALID-END  VALUE    -4013.
       78  SQLA-RC-INVALID-CMPD-SYNTAX VALUE  -4014.
       78  SQLA-RC-FATAL-ERROR       VALUE    -4901.
       78  SQLA-RC-PARM-CHARS-BAD    VALUE    -4902.
       78  SQLA-RC-PARM-LENGTH-BAD   value    -4903.
       78  SQLA-RC-PARM-POINTER-BAD  value    -4904.
       78  SQLA-RC-PARM-RANGE-BAD    value    -4905.
       78  SQLA-RC-HVAR-SQLTYPE-BAD  VALUE    -4911.
       78  SQLA-RC-HVAR-SQLLEN-BAD   VALUE    -4912.
       78  SQLA-RC-HVAR-TOKEN-ID-DUP VALUE    -4913.
       78  SQLA-RC-HVAR-TOKEN-ID-BAD VALUE    -4914.
       78  SQLA-RC-INIT-DUP          VALUE    -4915.
       78  SQLA-RC-INIT-REQUIRED     VALUE    -4916.
       78  SQLA-RC-OPTION-BAD        value    -4917.
       78  SQLA-RC-TERM-OPTION-BAD   VALUE    -4918.
       78  SQLA-RC-TASK-ARRAY-LIMIT  VALUE    -4919.
       78  SQLA-RC-TOKEN-ARRAY-LIMIT VALUE    -4920.
       78  SQLA-RC-OPTION-GEN-INVALID value   -30104.
      * Prep version of -109
       78  SQLA-RC-STMT-CLAUSE-BAD   VALUE    -4940.
      * Prep version of -198
       78  SQLA-RC-STMT-BLANK        VALUE    -4941.
      * Prep version of -303
       78  SQLA-RC-SELECT-HVAR-TYPE-BAD VALUE -4942.
      * Prep version of -313
       78  SQLA-RC-SELECT-LIST-BAD   VALUE    4943.
      * Prep version of -407
       78  SQLA-RC-COLUMN-NOT-NULLABLE VALUE  -4944.
      * Prep version of -418
       78  SQLA-RC-STMT-MARKER-BAD   VALUE    -4945.
      * Prep version of -504
       78  SQLA-RC-CURSOR-NOT-DECLARED VALUE  -4946.
       78  SQLA-RC-NO-USERDA-IN-CMPD VALUE    -4950.
       78  SQLA-RC-SQLDA-ID-BAD      VALUE    -4951.
       78  SQLA-RC-SQLVAR-INDEX-BAD  VALUE    -4952.
       78  SQLA-RC-CALL-TYPE-BAD     VALUE    -4953.
       78  SQLA-RC-SECTION-BAD       VALUE    -4954.

      * Temporary value used in sqlca.sqlcode after an SQLAALOC function.
      * Runtime code can check sqlca.sqlcode to determine whether or not the
      * SQLASETV function needs to be called to initialize the sqlda sqlvar
      * elements.
      * sqlvars already initialized
       78  SQLA-RC-SQLVARS-SET       VALUE    4959.

      * Errors that cause Precompiler Services to enter the fatal error
      * state. After encountering one of these types of errors, Precompiler
      * Services requires an SQLAFINI and SQLAINIT call before it will
      * continue.

       78  SQLA-RC-INV-INSERT        value    -803.
       78  SQLA-RC-INV-SQLDA-ADR     value    -822.
       78  SQLA-RC-SYS-ERROR         value    -902.
       78  SQLA-RC-DEADLOCK-ERR      value    -911.
       78  SQLA-RC-FAT-SYS-ERR       value    -912.
       78  SQLA-RC-TOO-MANY-LKS      value    -930.
       78  SQLA-RC-STORAGE-ERR       value    -954.
       78  SQLA-RC-DB-HEAP-ERR       value    -956.
       78  SQLA-RC-TOOMANY-OFLS      value    -958.
       78  SQLA-RC-TOOMANY-FILES     value    -960.
       78  SQLA-RC-LOG-FULL          value    -964.
       78  SQLA-RC-FILE-SYS-FULL     value    -968.
       78  SQLA-RC-READ-ONLY-FIL     value    -970.

       78  SQLA-RC-INCORRECT-DSK     VALUE    -972.
       78  SQLA-RC-DB-DRV-LOCKED     VALUE    -974.
       78  SQLA-RC-DRV-DOOR-OPEN     VALUE    -976.
       78  SQLA-RC-DISK-WRT-PRO      VALUE    -978.
       78  SQLA-RC-FILESYSTEM-ACCESS VALUE    -10019.

       78  SQLA-RC-DISK-ERROR        value    -980.
       78  SQLA-RC-DISK-ERR          value    -982.
       78  SQLA-RC-COMM-RB-ERR       value    -984.
       78  SQLA-RC-CAT-FILE-ERR      value    -985.
       78  SQLA-RC-TAB-FILE-ERR      value    -986.
       78  SQLA-RC-INDEX-ERR         value    -990.
       78  SQLA-RC-REL-NUM-BAD       value    -992.
       78  SQLA-RC-AGENT-GONE        value    -1224.
      * Prep version of -952
       78  SQLA-RC-CTRL-BREAK        VALUE    -4994.
      * Territory info not found
       78  SQLA-RC-CODEPAGE-BAD      VALUE    -4995.
      * Prep version of -1046
       78  SQLA-RC-SQLUSER-BAD       VALUE    -4997.
      * Prep version of -1049
       78  SQLA-RC-DB-DISCONNECTED   VALUE    -4998.
      * Precompiler Services err
       78  SQLA-RC-INTERNAL-ERR      VALUE    -4999.

      *************************************************************************
      * Target API constants for sqlaopts()
      **************************************************************************
      * Precompiler Services API (sqlainit)
       78  SQLAO-PREP-SVCS-API       VALUE    0.
      * Precompile API (sqlaprep)
       78  SQLAO-PREP-API            VALUE    1.
      * Binder API (sqlabndx)
       78  SQLAO-BIND-API            VALUE    2.

      *************************************************************************
      * Precompiler Services Structures
      **************************************************************************

      * Precompiler Option Array.  Used with SQLAINIT.

       01 SQLA-OPTIONS.
           05 SQLA-OPTIONS-HEADER.
              10 SQLA-OPTIONS-ALLOCATED   PIC 9(9) COMP-5.
              10 SQLA-OPTIONS-USED        PIC 9(9) COMP-5.
           05 SQLA-OPTION OCCURS 1 TO 10 
                 DEPENDING ON SQLA-OPTIONS-ALLOCATED.
              10 SQLA-OPTIONS-TYPE        PIC 9(9) COMP-5.
              10 SQLA-OPTIONS-VAL         PIC 9(9) COMP-5.

      * Program ID.  Used by Precompiler Services and Runtime Services
       01 SQL-SQLA-PROGRAM-ID-V610.
           05 SQL-LENGTH             PIC 9(4) COMP-5.
           05 SQL-RP-REL-NUM         PIC 9(4) COMP-5.
           05 SQL-DB-REL-NUM         PIC 9(4) COMP-5.
           05 SQL-BF-REL-NUM         PIC 9(4) COMP-5.
           05 SQLUSER                PIC X(8).
           05 SQL-PLANNAME           PIC X(8).
           05 SQL-CONTOKEN           PIC X(8).
           05 SQL-BUFFER             PIC X(8).

       01 SQL-SQLA-PROGRAM-ID.
           05 SQL-LENGTH             PIC 9(4) COMP-5.
           05 SQL-RP-REL-NUM         PIC 9(4) COMP-5.
           05 SQL-DB-REL-NUM         PIC 9(4) COMP-5.
           05 SQL-BF-REL-NUM         PIC 9(4) COMP-5.
           05 SQLUSER-LEN            PIC 9(4) COMP-5.
           05 SQLUSER                PIC X(128).
           05 SQL-PLANNAME           PIC X(8).
           05 SQL-CONTOKEN           PIC X(8).
           05 SQL-BUFFER             PIC X(8).

       01 SQL-SQLA-PROGRAM-ID.
           05 SQL-LENGTH             PIC 9(4) COMP-5.
           05 SQL-RP-REL-NUM         PIC 9(4) COMP-5.
           05 SQL-DB-REL-NUM         PIC 9(4) COMP-5.
           05 SQL-BF-REL-NUM         PIC 9(4) COMP-5.
           05 SQL-PLANNAME           PIC X(8).
           05 SQL-CONTOKEN           PIC X(8).
           05 SQL-BUFFER             PIC X(8).
           05 SQLUSER-LEN            PIC 9(4) COMP-5.
           05 SQLUSER                PIC X(128).

      * Indexes into the buffer field, showing the information it provides
       78  SQLA-PGMID-STD            VALUE    1.
       78  SQLA-PGMID-CONNECT        VALUE    2.
       78  SQLA-PGMID-SQLRULES       VALUE    3.
       78  SQLA-PGMID-DISCONNECT     VALUE    4.
       78  SQLA-PGMID-SYNCPOINT      VALUE    5.
       78  SQLA-PGMID-NETBIOS        VALUE    6.
       78  SQLA-PGMID-DEFERRED-PREPARE VALUE  7.
       78  SQLA-INT-TO-ASCII-DIFF    VALUE    48.

      * Number of DUOW connection options
       78  SQLA-PGMID-CONNOPTS       VALUE    6.

      * Precompiler Task Array.  Used with SQLACMPL.

      * Assume maximum number of tasks ranges between 0 and 32
       01 SQLA-TASKS.
          05 SQLA-TASKS-HEADER.
             10 SQLA-TASKS-ALLOCATED   PIC 9(9) COMP-5.
             10 SQLA-TASKS-USED        PIC 9(9) COMP-5.
          05 SQLA-TASK OCCURS 0 TO 32 
                DEPENDING ON SQLA-TASKS-ALLOCATED.
             10 SQLA-TASKS-FUNC        PIC 9(9) COMP-5.
             10 SQLA-TASKS-VAL         PIC 9(9) COMP-5.

      * Token return structure.  Used to return length and offset of
      * a token within an SQL statement.  May be overlaid on top of
      * the val member of the task array or on top of the id member
      * of the token id array.
       01 SQLA-RETURN-TOKEN.
          05 SQLA-RETURN-OFFSET       PIC 9(4) COMP-5.
          05 SQLA-RETURN-LENGTH       PIC 9(4) COMP-5.

      * Precompiler Token ID Array.  Used with SQLACMPL.

      * Assume maximum number of tokens ranges between 0 and 64
       01 SQLA-TOKENS.
          05 SQLA-TOKENS-HEADER.
             10 SQLA-TOKENS-ALLOCATED   PIC 9(9) COMP-5.
             10 SQLA-TOKENS-USED        PIC 9(9) COMP-5.
          05 SQLA-TOKEN OCCURS 0 TO 64 
                DEPENDING ON SQLA-TOKENS-ALLOCATED.
             10 SQLA-TOKENS-ID          PIC 9(9) COMP-5.
             10 SQLA-TOKENS-USE         PIC 9(9) COMP-5.

      * Flagger information area. Used with SQLACMPL and SQLGCMPL.
       01 SQLA-FLAGINFO.
          05 SQLFLAG-VERSION            PIC 9(4) COMP-5.  
          05 FILLER                     PIC X(2).
          05 SQLFLAG-MSGS. 
             10 SQLFLAG-MSGS-COUNT      PIC 9(4) COMP-5.
             10 FILLER                  PIC X(2).
             10 SQLFLAG-MSGS-SQLCA OCCURS 10 TIMES.
                20 SQLFLAG-SQLCAID pic X(8) value "SQLCA   ".
                20 SQLFLAG-SQLCABC pic S9(9) comp-5 value 136.
                20 SQLFLAG-SQLCODE pic S9(9) comp-5.
                20 SQLFLAG-SQLERRM.
                   49 SQLFLAG-SQLERRML PIC S9(4) comp-5.
                   49 SQLFLAG-SQLERRMC PIC X(70).
                20 SQLFLAG-SQLERRP PIC X(8).
                20 SQLFLAG-SQLERRD OCCURS 6 TIMES PIC S9(9) comp-5.
                20 SQLFLAG-SQLWARN.
                   30 SQLFLAG-SQLWARN0 PIC X.
                   30 SQLFLAG-SQLWARN1 PIC X.
                   30 SQLFLAG-SQLWARN2 PIC X.
                   30 SQLFLAG-SQLWARN3 PIC X.
                   30 SQLFLAG-SQLWARN4 PIC X.
                   30 SQLFLAG-SQLWARN5 PIC X.
                   30 SQLFLAG-SQLWARN6 PIC X.
                   30 SQLFLAG-SQLWARN7 PIC X.
                   30 SQLFLAG-SQLWARN8 PIC X.
                   30 SQLFLAG-SQLWARN9 PIC X.
                   30 SQLFLAG-SQLWARNA PIC X.
                20 SQLFLAG-SQLSTATE PIC X(5).
