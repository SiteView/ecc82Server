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
       77  SQLA-FORMAT               PIC S9(4) COMP-5 VALUE 1.
      * Option: access plan creation
       77  SQLA-ACCESS-PLAN          PIC S9(4) COMP-5 VALUE 2.
      * Option: bind file creation
       77  SQLA-BIND-FILE            PIC S9(4) COMP-5 VALUE 3.
      * Option: isolation level
       77  SQLA-ISOLATION            PIC S9(4) COMP-5 VALUE 4.
      * Option: record blocking
       77  SQLA-BLOCK                PIC S9(4) COMP-5 VALUE 5.
      * Flagger option
       77  SQLA-FLAG-OPT             PIC S9(4) COMP-5 VALUE 8.
      * Option: standards compliance
       77  SQLA-STDS-LEVEL           PIC S9(4) COMP-5 VALUE 7.
      * C NULL required option
       77  SQLA-CNULREQD-OPT         PIC S9(4) COMP-5 VALUE 11.
      * Generic option
       77  SQLA-GENERIC-OPT          PIC S9(4) COMP-5 VALUE 12.
      * Option for Deferred Prepare
       77  SQLA-DEFERRED-PREPARE-OPT PIC S9(4) COMP-5 VALUE 15.
      * Level of a module
       77  SQLA-LEVEL-OPT            PIC S9(4) COMP-5 VALUE 30.
      * Package collection id
       77  SQLA-COLLECTION-OPT       PIC S9(4) COMP-5 VALUE 31.
      * Package version id
       77  SQLA-VERSION-OPT          PIC S9(4) COMP-5 VALUE 32.
      * Package owner auth id
       77  SQLA-OWNER-OPT            PIC S9(4) COMP-5 VALUE 33.
      * Synonym for owner
       77  SQLA-SCHEMA-OPT           PIC S9(4) COMP-5 VALUE 33.
      * Authorization identifier that is to be used as qualifier for
      * unqualified objects
       77  SQLA-QUALIFIER-OPT        PIC S9(4) COMP-5 VALUE 34.
      * Synonym for qualifier.
       77  SQLA-CATALOG-OPT          PIC S9(4) COMP-5 VALUE 34.
      * Package description
       77  SQLA-TEXT-OPT             PIC S9(4) COMP-5 VALUE 35.
      * Indicates when object validation occurs
       77  SQLA-VALIDATE-OPT         PIC S9(4) COMP-5 VALUE 40.
      * Determines whether information will be produced about how the SQL
      * statements in a package will be executed
       77  SQLA-EXPLAIN-OPT          PIC S9(4) COMP-5 VALUE 41.
      * Indicates whether a package is to be added or replaced
       77  SQLA-ACTION-OPT           PIC S9(4) COMP-5 VALUE 42.
      * Indicates under what conditions a package will be created
       77  SQLA-SQLERROR-OPT         PIC S9(4) COMP-5 VALUE 25.
      * Replaces a specific version of a package
       77  SQLA-REPLVER-OPT          PIC S9(4) COMP-5 VALUE 44.
      * Indicates whether EXECUTE authorities are to be preserved when a
      * package is replaced
       77  SQLA-RETAIN-OPT           PIC S9(4) COMP-5 VALUE 45.
      * Indicates whether resources are released at each COMMIT or when the
      * application terminates
       77  SQLA-RELEASE-OPT          PIC S9(4) COMP-5 VALUE 46.
      * Specifies whether or not the query is executed using I/O parallelism
       77  SQLA-DEGREE-OPT           PIC S9(4) COMP-5 VALUE 47.
      * Designates whether an apostrophe or quote will be used as a string
      * delimiter
       77  SQLA-STRDEL-OPT           PIC S9(4) COMP-5 VALUE 50.
      * Designates whether a period or comma will be used as a decimal point
      * indicator
       77  SQLA-DECDEL-OPT           PIC S9(4) COMP-5 VALUE 51.
      * Designates default character subtype that is to be used for column
      * definitions in the CREATE and ALTER TABLE SQL statements
       77  SQLA-CHARSUB-OPT          PIC S9(4) COMP-5 VALUE 55.
      * Designates what CCSID will be used for single byte characters for
      * character column definitions without a specific CCSID clause in the
      * CREATE and ALTER TABLE SQL statements
       77  SQLA-CCSIDS-OPT           PIC S9(4) COMP-5 VALUE 56.
      * Designates what CCSID will be used for mixed byte characters for
      * character column definitions without a specific CCSID clause in the
      * CREATE and ALTER TABLE SQL statements
       77  SQLA-CCSIDM-OPT           PIC S9(4) COMP-5 VALUE 57.
      * Designates what CCSID will be used for double byte characters for
      * character column definitions without a specific CCSID clause in the
      * CREATE and ALTER TABLE SQL statements
       77  SQLA-CCSIDG-OPT           PIC S9(4) COMP-5 VALUE 58.
      * Specifies the maximum precision to be used in decimal arithmetic
      * operations
       77  SQLA-DEC-OPT              PIC S9(4) COMP-5 VALUE 59.
      * Specifies whether one or multiple connections can exist at the same
      * time.
       77  SQLA-CONNECT-OPT          PIC S9(4) COMP-5 VALUE 16.
      * Specifies the set of rules used for connection to multiple databases
      * within a single unit of work
       77  SQLA-SQLRULES-OPT         PIC S9(4) COMP-5 VALUE 17.
      * Specifies which databases disconnected when a COMMIT or ROLLBACK is
      * issued.
       77  SQLA-DISCONNECT-OPT       PIC S9(4) COMP-5 VALUE 18.
      * Specifies what syncpoint option (for example one or two phase) will
      * be used.
       77  SQLA-SYNCPOINT-OPT        PIC S9(4) COMP-5 VALUE 19.
      * Specifies which authorization identifier to use when dynamic SQL in
      * a package is executed.
       77  SQLA-DYNAMICRULES-OPT     PIC S9(4) COMP-5 VALUE 61.
      * Buffers VALUE inserts - bind and precompile option for DB2/PE
      * servers only
       77  SQLA-INSERT               PIC S9(4) COMP-5 VALUE 62.
      * Capture explain snapshot - bind and precompile option
       77  SQLA-EXPLSNAP-OPT         PIC S9(4) COMP-5 VALUE 63.
      * Path for user-defined function resolution - bind and precompile
      * option
       77  SQLA-FUNCTION-PATH        PIC S9(4) COMP-5 VALUE 64.
      * Disable prepare-time SQL warnings - bind and precompile option
       77  SQLA-SQLWARN-OPT          PIC S9(4) COMP-5 VALUE 65.
      * Set query optimization class -bind and precompile option
       77  SQLA-QUERYOPT-OPT         PIC S9(4) COMP-5 VALUE 66.
      * Target compiler -precompile option
       77  SQLA-TARGET-OPT           PIC S9(4) COMP-5 VALUE 67.
      * Name of precompiler output file - precompile option
       77  SQLA-PREP-OUTPUT-OPT      PIC S9(4) COMP-5 VALUE 68.
      * Internal use only
       77  SQLA-TOKEN-USE-INITIALIZED-OPT PIC S9(4) COMP-5 VALUE 1000.
      * Internal use only
       77  SQLA-USE-LONG-LABELS      PIC S9(4) COMP-5 VALUE 1001.
      * Call resolution option -precompile option
       77  SQLA-CALL-RES-OPT         PIC S9(4) COMP-5 VALUE 85.
      * If static cursors are to be treated as READONLY
       77  SQLA-STATICREADONLY-OPT   PIC S9(4) COMP-5 VALUE 87.
      * The optimization profile name
       77  SQLA-OPTPROFILE-OPT       PIC S9(4) COMP-5 VALUE 88.

      * Values used for option[n].val when option[n].type is SQLA_FORMAT in
      * the Precompiler Option Array.
      * Default date/time format
       77  SQLA-POA-DEF              PIC S9(4) COMP-5 VALUE 0.
      * USA date/time format
       77  SQLA-POA-USA              PIC S9(4) COMP-5 VALUE 1.
      * European date/time format
       77  SQLA-POA-EUR              PIC S9(4) COMP-5 VALUE 2.
      * International date/time format
       77  SQLA-POA-ISO              PIC S9(4) COMP-5 VALUE 3.
      * Japanese date/time format
       77  SQLA-POA-JIS              PIC S9(4) COMP-5 VALUE 4.
      * Local date/time format
       77  SQLA-POA-LOC              PIC S9(4) COMP-5 VALUE 5.

      * Values used for option[n].val when option[n].type is
      * SQLA_ACCESS_PLAN in
      * the Precompiler Option Array.
      * Create an access plan
       77  SQLA-CREATE-PLAN          PIC S9(4) COMP-5 VALUE 1.
      * Do not create an access plan
       77  SQLA-NO-PLAN              PIC S9(4) COMP-5 VALUE 0.
      * Set ocb->syntax to TRUE
       77  SQLA-NO-PLAN-SYNTAX       PIC S9(4) COMP-5 VALUE 3.

      * Values used for option[n].val when option[n].type is SQLA_BIND_FILE
      * in
      * the Precompiler Option Array.
      * Create a bind file
       77  SQLA-CREATE-BIND-FILE     PIC S9(4) COMP-5 VALUE 1.
      * Do not create a bind file
       77  SQLA-NO-BIND-FILE         PIC S9(4) COMP-5 VALUE 0.

      * Values used for option[n].val when option[n].type is SQLA_ISOLATION
      * in
      * the Precompiler Option Array.
      * RR isolation level
       77  SQLA-REPEATABLE-READ      PIC S9(4) COMP-5 VALUE 0.
      * CS Isolation level
       77  SQLA-CURSOR-STABILITY     PIC S9(4) COMP-5 VALUE 1.
      * UR Isolation level
       77  SQLA-UNCOMMITTED-READ     PIC S9(4) COMP-5 VALUE 2.
      * RS Isolation level
       77  SQLA-READ-STABILITY       PIC S9(4) COMP-5 VALUE 3.
      * NC Isolation level
       77  SQLA-NO-COMMIT            PIC S9(4) COMP-5 VALUE 4.

      * Values used for option[n].val when option[n].type is SQLA_BLOCK in
      * the Precompiler Option Array.
       77  SQLA-BLOCK-UNAMBIG        PIC S9(4) COMP-5 VALUE 0.
       77  SQLA-BLOCK-ALL            PIC S9(4) COMP-5 VALUE 1.
       77  SQLA-NO-BLOCK             PIC S9(4) COMP-5 VALUE 2.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_LEVEL_OPT
       77  SQLA-MAX-LEVEL            PIC S9(4) COMP-5 VALUE 8.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_COLLECTION_OPT
       77  SQLA-MAX-COLLECTION       PIC S9(4) COMP-5 VALUE 30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_VERSION_OPT
       77  SQLA-MAX-VERSION          PIC S9(4) COMP-5 VALUE 254.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_OWNER_OPT
       77  SQLA-MAX-OWNER            PIC S9(4) COMP-5 VALUE 30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_SCHEMA_OPT
       77  SQLA-MAX-SCHEMA           PIC S9(4) COMP-5 VALUE 30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_QUALIFIER_OPT
       77  SQLA-MAX-QUALIFIER        PIC S9(4) COMP-5 VALUE 30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_CATALOG_OPT
       77  SQLA-MAX-CATALOG          PIC S9(4) COMP-5 VALUE 30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_TEXT_OPT
       77  SQLA-MAX-TEXT             PIC S9(4) COMP-5 VALUE 255.

      * Values used for option[n].val when option[n].type is
      * SQLA_VALIDATE_OPT.
      * Validate objects during BIND
       77  SQLA-VALIDATE-BIND        PIC S9(4) COMP-5 VALUE 0.
      * Validate objects during execution
       77  SQLA-VALIDATE-RUN         PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is
      * SQLA_EXPLAIN_OPT.
      * No EXPLAIN output produced
       77  SQLA-EXPLAIN-NO           PIC S9(4) COMP-5 VALUE 0.
      * EXPLAIN output produced
       77  SQLA-EXPLAIN-YES          PIC S9(4) COMP-5 VALUE 1.
      * EXPLAIN output produced for all static and dynamic statements.
       77  SQLA-EXPLAIN-ALL          PIC S9(4) COMP-5 VALUE 2.
      * EXPLAIN output produced for static reoptimizable statements.
       77  SQLA-EXPLAIN-REOPT        PIC S9(4) COMP-5 VALUE 3.

      * Values used for option[n].val when option[n].type is
      * SQLA_ACTION_OPT.
      * Package is added
       77  SQLA-ACTION-ADD           PIC S9(4) COMP-5 VALUE 0.
      * Package is replaced
       77  SQLA-ACTION-REPLACE       PIC S9(4) COMP-5 VALUE 1.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_REPLVER_OPT
       77  SQLA-MAX-REPLVER          PIC S9(4) COMP-5 VALUE 254.

      * Values used for option[n].val when option[n].type is
      * SQLA_SQLERROR_OPT.
      * Do not create a package if errors are encountered
       77  SQLA-SQLERROR-NOPACKAGE   PIC S9(4) COMP-5 VALUE 0.
      * Do not create a package
       77  SQLA-SQLERROR-CHECK       PIC S9(4) COMP-5 VALUE 1.
      * Create a package even if errors are encountered
       77  SQLA-SQLERROR-CONTINUE    PIC S9(4) COMP-5 VALUE 2.

      * Values used for option[n].val when option[n].type is
      * SQLA_RETAIN_OPT.
      * Do not preserve EXECUTE authorities when a package is replaced
       77  SQLA-RETAIN-NO            PIC S9(4) COMP-5 VALUE 0.
      * Preserve EXECUTE authorities when a package is replaced
       77  SQLA-RETAIN-YES           PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is
      * SQLA_RELEASE_OPT.
      * Release resources at COMMIT
       77  SQLA-RELEASE-COMMIT       PIC S9(4) COMP-5 VALUE 0.
      * Release resources when a program terminates
       77  SQLA-RELEASE-DEALLOCATE   PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is
      * SQLA_STRDEL_OPT.
      * Apostrophe string delimiter
       77  SQLA-STRDEL-APOSTROPHE    PIC S9(4) COMP-5 VALUE 0.
      * Quote string delimiter
       77  SQLA-STRDEL-QUOTE         PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is
      * SQLA_DECDEL_OPT.
      * Period is used as a decimal point indicator in decimal and floating
      * point literals
       77  SQLA-DECDEL-PERIOD        PIC S9(4) COMP-5 VALUE 0.
      * Comma is used as a decimal point indicator in decimal and floating
      * point literals
       77  SQLA-DECDEL-COMMA         PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is
      * SQLA_CHARSUB_OPT.
      * Use the target system defined default for all new character columns
      * for which an explicit subtype is not specified
       77  SQLA-CHARSUB-DEFAULT      PIC S9(4) COMP-5 VALUE 0.
      * Use the bit character subtype for all new character columns for
      * which an explicit subtype is not specified
       77  SQLA-CHARSUB-BIT          PIC S9(4) COMP-5 VALUE 1.
      * Use the SBCS character subtype for all new character columns for
      * which an explicit subtype is not specified
       77  SQLA-CHARSUB-SBCS         PIC S9(4) COMP-5 VALUE 2.
      * Use the mixed character subtype for all new character columns for
      * which an explicit subtype is not specified
       77  SQLA-CHARSUB-MIXED        PIC S9(4) COMP-5 VALUE 3.

      * Values used for option[n].val when option[n].type is SQLA_DEC_OPT.
      * 15 bit precision is used in decimal arithmetic operations
       77  SQLA-DEC-15               PIC S9(4) COMP-5 VALUE 15.
      * 31 bit precision is used in decimal arithmetic operations
       77  SQLA-DEC-31               PIC S9(4) COMP-5 VALUE 31.

      * Values used for option[n].val when option[n].type is
      * SQLA_DEGREE_OPT.
      * Prohibits parallel I/O operations
       77  SQLA-DEGREE-1             PIC S9(4) COMP-5 VALUE 1.
      * Allows the target database system to determine the degree of
      * parallel I/O operations
       77  SQLA-DEGREE-ANY           PIC S9(4) COMP-5 VALUE 0.
      * Maximum value
       77  SQLA-MAX-DEGREE-VAL       PIC S9(9) COMP-5 VALUE 32767.

      * Values used for option[n].val when option[n].type is
      * SQLA_CNULREQD_OPT in the Precompiler Option array
      * C NULL is not required
       77  SQLA-CNULREQD-NO          PIC S9(4) COMP-5 VALUE 0.
      * C NULL is not required
       77  SQLA-CNULREQD-YES         PIC S9(4) COMP-5 VALUE 1.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_GENERIC_OPT
       77  SQLA-MAX-GENERIC          PIC S9(4) COMP-5 VALUE 1023.

      * Values used for option[n].val when option[n].type is
      * SQLA_DEFERRED_PREPARE_OPT.
      * Enable Deferred Prepare except for a PREPARE...INTO statement which
      * contains parameter markers.
       77  SQLA-DEFERRED-PREPARE-YES PIC S9(4) COMP-5 VALUE 1.
      * Disable Deferred Prepare
       77  SQLA-DEFERRED-PREPARE-NO  PIC S9(4) COMP-5 VALUE 2.
      * Same as YES except that a PREPARE...INTO which contains parameter
      * markers will also be deferred
       77  SQLA-DEFERRED-PREPARE-ALL PIC S9(4) COMP-5 VALUE 3.

      * Values used for option[n].val when option[n].type is
      * SQLA_CONNECT_OPT.
      * Indicates that only one connection to a database can exist at any
      * given time.
       77  SQLA-CONNECT-1            PIC S9(4) COMP-5 VALUE 1.
      * Indicates that multiple connections can exist simultaneously, with
      * one being active and the others dormant.
       77  SQLA-CONNECT-2            PIC S9(4) COMP-5 VALUE 2.

      * Values used for option[n].val when option[n].type is SQLA_RULES_OPT.
      * Indicates that CONNECT TO can be used to make a dormant connection
      * the current connection.
       77  SQLA-RULES-DB2            PIC S9(4) COMP-5 VALUE 1.
      * Indicates that CONNECT TO is not valid for making a dormant
      * connection current,and SET CONNECTION must be used instead.
       77  SQLA-RULES-STD            PIC S9(4) COMP-5 VALUE 2.
      * Values used for option[n].val when option[n].type is
      * SQLA_DISCONNECT_OPT.
      * Indicates that all connections marked by the RELEASE statement will
      * be the only connections released when a COMMIT or ROLLBACK is
      * issued.
       77  SQLA-DISCONNECT-EXPL      PIC S9(4) COMP-5 VALUE 1.
      * Indicates that all connections which don't have open WITH HOLD
      * cursors will be the only connections released when a COMMIT or
      * ROLLBACK is issued.
       77  SQLA-DISCONNECT-COND      PIC S9(4) COMP-5 VALUE 2.
      * Indicates that all connections will be released when a COMMIT or
      * ROLLBACK is issued.
       77  SQLA-DISCONNECT-AUTO      PIC S9(4) COMP-5 VALUE 3.

      * Values used for option[n].val when option[n].type is
      * SQLA_SYNCPOINT_OPT.
      * Do not use a transaction manager to perform two phase commit,but
      * enforce that there is only one database is updated when multiple
      * databases are accessed within a single transaction.
       77  SQLA-SYNC-ONEPHASE        PIC S9(4) COMP-5 VALUE 1.
      * Use a transaction manager to coordinate two phase commit.
       77  SQLA-SYNC-TWOPHASE        PIC S9(4) COMP-5 VALUE 2.
      * No two-phase commit and no single updater enforcement.
       77  SQLA-SYNC-NONE            PIC S9(4) COMP-5 VALUE 0.

      * Values used for option[n].val when option[n].type is
      * SQLA_DYNAMICRULES_OPT.
      * Dynamic SQL in package will use authid of person running the package
       77  SQLA-DYNAMICRULES-RUN     PIC S9(4) COMP-5 VALUE 0.
      * Dynamic SQL in package will use authid of person who owns the
      * package
       77  SQLA-DYNAMICRULES-BIND    PIC S9(4) COMP-5 VALUE 1.
      * Dynamic SQL in UDF or stored procedure will use authid of invoker of
      * UDF or stored procedure
       77  SQLA-DYNAMICRULES-INVOKE  PIC S9(4) COMP-5 VALUE 2.
      * Dynamic SQL in UDF or stored procedure will use authid of definer of
      * UDF or stored procedure
       77  SQLA-DYNAMICRULES-DEFINE  PIC S9(4) COMP-5 VALUE 3.
      * Dynamic SQL in routine will use authid of definer of routine
       77  SQLA-DYNAMICRULES-DEFINEBIND PIC S9(4) COMP-5 VALUE 5.
      * Dynamic SQL in routine will use authid of definer of routine 
       77  SQLA-DYNAMICRULES-DEFINERUN PIC S9(4) COMP-5 VALUE 3.
      * Dynamic SQL in routine will use authid of invoker of routine
       77  SQLA-DYNAMICRULES-INVOKEBIND PIC S9(4) COMP-5 VALUE 4.
      * Dynamic SQL in routine will use authid of invoker of routine 
       77  SQLA-DYNAMICRULES-INVOKERUN PIC S9(4) COMP-5 VALUE 2.

      * Values used for option[n].val when option[n].type is SQLA_FLAG_OPT
      * inthe Precompiler Option Array.
       77  SQLA-DB2V23-SYNTAX        PIC S9(4) COMP-5 VALUE 4.
       77  SQLA-DB2V31-SYNTAX        PIC S9(4) COMP-5 VALUE 5.
       77  SQLA-DB2V41-SYNTAX        PIC S9(4) COMP-5 VALUE 6.
       77  SQLA-FIPS-SYNTAX          PIC S9(4) COMP-5 VALUE 7.

      * Values used for option[n].val when option[n].type is
      * SQLA_EXPLSNAP_OPT in the Precompiler Option array
      * Do not save explain snapshot
       77  SQLA-EXPLSNAP-NO          PIC S9(4) COMP-5 VALUE 0.
      * Save explain snapshot
       77  SQLA-EXPLSNAP-YES         PIC S9(4) COMP-5 VALUE 1.
      * Save explain snapshot for all static and dynamic statements
       77  SQLA-EXPLSNAP-ALL         PIC S9(4) COMP-5 VALUE 2.
      * Save explain snapshot for static reoptimizable statements
       77  SQLA-EXPLSNAP-REOPT       PIC S9(4) COMP-5 VALUE 3.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQLA_FUNCTION_PATH
       77  SQLA-MAX-FUNCPATH         PIC S9(4) COMP-5 VALUE 254.

      * Values used for option[n].val when option[n].type is
      * SQLA_SQLWARN_OPT in the Precompiler Option array
      * Suppress prepare-time warning SQLCODEs
       77  SQLA-SQLWARN-NO           PIC S9(4) COMP-5 VALUE 0.
      * Permit prepare-time warning SQLCODEs
       77  SQLA-SQLWARN-YES          PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is
      * SQLA_QUERYOPT_OPT in the Precompiler Option array
      * Class 0 query optimization
       77  SQLA-QUERYOPT-0           PIC S9(4) COMP-5 VALUE 0.
      * Class 1 query optimization
       77  SQLA-QUERYOPT-1           PIC S9(4) COMP-5 VALUE 1.
      * Class 2 query optimization
       77  SQLA-QUERYOPT-2           PIC S9(4) COMP-5 VALUE 2.
      * Class 3 query optimization
       77  SQLA-QUERYOPT-3           PIC S9(4) COMP-5 VALUE 3.
      * Class 5 query optimization
       77  SQLA-QUERYOPT-5           PIC S9(4) COMP-5 VALUE 5.
      * Class 7 query optimization
       77  SQLA-QUERYOPT-7           PIC S9(4) COMP-5 VALUE 7.
      * Class 9 query optimization
       77  SQLA-QUERYOPT-9           PIC S9(4) COMP-5 VALUE 9.

      * Values used for option[n].val when option[n].type is SQLA_INSERT_OPT
      * Do not buffer VALUE inserts
       77  SQLA-INSERT-DEF           PIC S9(4) COMP-5 VALUE 0.
      * Buffer VALUE inserts
       77  SQLA-INSERT-BUF           PIC S9(4) COMP-5 VALUE 1.

      * Maximum target string length in option[n].val when option[n].type is
      * SQLA_TARGET_OPT
       77  SQLA-MAX-TARGET-LEN       PIC S9(4) COMP-5 VALUE 32.

      * Values used for option[n].val when option[n].type is SQLA_STDS_LEVEL
      * inthe Precompiler Option Array.
      * No compliance required
       77  SQLA-STND-NONE            PIC S9(4) COMP-5 VALUE 0.
      * Comply with MIA standard
       77  SQLA-STND-MIA             PIC S9(4) COMP-5 VALUE 1.
      * Comply with SQL92E standard
       77  SQLA-STND-SQL92E          PIC S9(4) COMP-5 VALUE 2.

      * Values used for option[n].val when option[n].type is
      * SQLA_STATICREADONLY_OPT in the Precompiler Option Array.
      * Static cursors take on attributes as would normally be generated
      * given the statement text and the LANGLEVEL option
       77  SQLA-STATICRO-NO          PIC S9(4) COMP-5 VALUE 0.
      * Any static cursor that does not contain the FOR UPDATE or FOR READ
      * ONLY clause will be considered READ ONLY.
       77  SQLA-STATICRO-YES         PIC S9(4) COMP-5 VALUE 1.
      * Values used for location, an input parameter of SQLAAHVR.
      * Add host var found in a declare section
       77  SQLA-DECLARE-SECT         PIC S9(4) COMP-5 VALUE 0.
      * Add host var found in an SQL statement
       77  SQLA-SQL-STMT             PIC S9(4) COMP-5 VALUE 1.

      * Values assigned to task[n].func, an output parameter of SQLACMPL.
      * Generate a call to sqlastrt
       77  SQLA-START                PIC S9(4) COMP-5 VALUE 0.
      * Begin or end parsing hvar declarations
       77  SQLA-DECLARE              PIC S9(4) COMP-5 VALUE 1.
      * Generate code for an SQLCA or SQLDA
       77  SQLA-INCLUDE              PIC S9(4) COMP-5 VALUE 2.
      * Allocate an input SQLDA using SQLAALOC
       77  SQLA-ALLOC-INPUT          PIC S9(4) COMP-5 VALUE 3.
      * Allocate an output SQLDA using SQLAALOC
       77  SQLA-ALLOC-OUTPUT         PIC S9(4) COMP-5 VALUE 4.
      * Register a host variable using SQLASETS
       77  SQLA-SETS                 PIC S9(4) COMP-5 VALUE 5.
      * Register an input user-defined SQLDA
       77  SQLA-USDA-INPUT           PIC S9(4) COMP-5 VALUE 6.
      * Register an output user-defined SQLDA
       77  SQLA-USDA-OUTPUT          PIC S9(4) COMP-5 VALUE 7.
      * Generate a call to SQLACALL
       77  SQLA-CALL                 PIC S9(4) COMP-5 VALUE 8.
      * Generate a call to SQLADLOC
       77  SQLA-DEALLOC              PIC S9(4) COMP-5 VALUE 9.
      * Generate a call to SQLASTOP
       77  SQLA-STOP                 PIC S9(4) COMP-5 VALUE 10.
      * Generate code for WHENEVER SQLERROR
       77  SQLA-SQLERROR             PIC S9(4) COMP-5 VALUE 11.
      * Generate code for WHENEVER SQLWARNING
       77  SQLA-SQLWARNING           PIC S9(4) COMP-5 VALUE 12.
      * Generate code for WHENEVER NOT_FOUND
       77  SQLA-NOT-FOUND            PIC S9(4) COMP-5 VALUE 13.
      * Process an INCLUDE text-file stmt
       77  SQLA-INC-TEXTFILE         PIC S9(4) COMP-5 VALUE 14.
      * Begin Compound SQL block
       77  SQLA-BEGIN-COMPOUND       PIC S9(4) COMP-5 VALUE 15.
      * Generate a call to SQLACMPD
       77  SQLA-CMPD                 PIC S9(4) COMP-5 VALUE 16.
      * Generate a conditional branch around subsequent Compound SQL
      * substatements.
       77  SQLA-CMPD-TEST            PIC S9(4) COMP-5 VALUE 17.
      * Generate a target label for gotos from cmpd_test (optional)
       77  SQLA-CMPD-MARK            PIC S9(4) COMP-5 VALUE 18.
      * Request the outer precompiler to get the next triggered SQL
      * statement
       77  SQLA-NEXT-SUBSTATEMENT    PIC S9(4) COMP-5 VALUE 19.
      * Generate code to copy SQLCODE/SQLSTATE
       77  SQLA-SQLCODE-COPY         PIC S9(4) COMP-5 VALUE 20.
      * Request the outer precompiler to get the next stored procedure SQL
      * statement
       77  SQLA-NEXT-STOREDPROC-STMT PIC S9(4) COMP-5 VALUE 21.
      * Generate code for ASSOCIATE LOCATOR 
       77  SQLA-ASSOCIATE-LOCATOR    PIC S9(4) COMP-5 VALUE 22.
      * Save last called procedure informationor generate code for ASSOCIATE
      * LOCATOR referring to the last called procedure
       77  SQLA-ASSOCIATE-PREV-PROC  PIC S9(4) COMP-5 VALUE 23.
      * Generate code for ALLOCATE CURSOR
       77  SQLA-ALLOCATE-CURSOR      PIC S9(4) COMP-5 VALUE 24.

      * Values returned in task[n].val when task[n].func is SQLA_DECLARE.
      * This is an output parameter of SQLACMPL.
       77  SQLA-BEGIN                PIC S9(4) COMP-5 VALUE 0.
       77  SQLA-END                  PIC S9(4) COMP-5 VALUE 1.

      * Values returned in task[n].val when task[n].func is SQLA_INCLUDE.
      * This is an output parameter of SQLACMPL.
       77  SQLA-SQLCA                PIC S9(4) COMP-5 VALUE 10.
       77  SQLA-SQLDA                PIC S9(4) COMP-5 VALUE 11.

      * Values returned in task[n].val when task[n].func is SQLA_CALL.
      * This is an output parameter of SQLACMPL.
       77  SQLA-CLOSE                PIC S9(4) COMP-5 VALUE 20.
       77  SQLA-COMMIT               PIC S9(4) COMP-5 VALUE 21.
       77  SQLA-DESCRIBE             PIC S9(4) COMP-5 VALUE 22.
       77  SQLA-EXECIMMD             PIC S9(4) COMP-5 VALUE 23.
       77  SQLA-EXECUTE              PIC S9(4) COMP-5 VALUE 24.
       77  SQLA-FETCH                PIC S9(4) COMP-5 VALUE 25.
       77  SQLA-OPEN                 PIC S9(4) COMP-5 VALUE 26.
       77  SQLA-PREPARE              PIC S9(4) COMP-5 VALUE 27.
       77  SQLA-ROLLBACK             PIC S9(4) COMP-5 VALUE 28.
       77  SQLA-CONNECT              PIC S9(4) COMP-5 VALUE 29.
      * Atomic Compound SQL call
       77  SQLA-CMPDEXEC             PIC S9(4) COMP-5 VALUE 30.
      * SQL free locators call
       77  SQLA-FREE-LOB-LOCATORS    PIC S9(4) COMP-5 VALUE 37.
      * Not Atomic Compound SQL call
       77  SQLA-CMPDEXEC-NA          PIC S9(4) COMP-5 VALUE 38.
       77  SQLA-DUOW                 PIC S9(4) COMP-5 VALUE 40.
      * Close Cursor with Release
       77  SQLA-CLOSE-REL            PIC S9(4) COMP-5 VALUE 41.
      * SP SQL Deferred call
       77  SQLA-CALLPROC             PIC S9(4) COMP-5 VALUE 42.
      * SP Deferred call
       77  SQLA-CALLPROC-DEFERRED    PIC S9(4) COMP-5 VALUE 42.
      * Set Current Packageset
       77  SQLA-SET-CURRENT-PKGSET   PIC S9(4) COMP-5 VALUE 43.
       77  SQLA-EXECIMMD-ASYNC       PIC S9(4) COMP-5 VALUE 44.
       77  SQLA-EXECUTE-ASYNC        PIC S9(4) COMP-5 VALUE 45.
       77  SQLA-FETCH-ASYNC          PIC S9(4) COMP-5 VALUE 46.
       77  SQLA-OPEN-ASYNC           PIC S9(4) COMP-5 VALUE 47.
       77  SQLA-CALLPROC-ASYNC       PIC S9(4) COMP-5 VALUE 48.
       77  SQLA-CMPDEXEC-ASYNC       PIC S9(4) COMP-5 VALUE 49.
       77  SQLA-DESCRIBE-ASYNC       PIC S9(4) COMP-5 VALUE 50.
       77  SQLA-PREPARE-ASYNC        PIC S9(4) COMP-5 VALUE 51.
       77  SQLA-EXECIMMD-ASYNC-COMMIT PIC S9(4) COMP-5 VALUE 52.
       77  SQLA-EXECUTE-ASYNC-COMMIT PIC S9(4) COMP-5 VALUE 53.
       77  SQLA-CMPDEXEC-ASYNC-COMMIT PIC S9(4) COMP-5 VALUE 54.
       77  SQLA-EXECIMMD-COMMIT      PIC S9(4) COMP-5 VALUE 55.
       77  SQLA-EXECUTE-COMMIT       PIC S9(4) COMP-5 VALUE 56.
       77  SQLA-CMPDEXEC-COMMIT      PIC S9(4) COMP-5 VALUE 57.
       77  SQLA-CMPDEXEC-NA-ASYNC    PIC S9(4) COMP-5 VALUE 58.
       77  SQLA-CMPDEXEC-NA-COMMIT   PIC S9(4) COMP-5 VALUE 59.
       77  SQLA-CMPDEXEC-NA-DIAG     PIC S9(4) COMP-5 VALUE 60.
       77  SQLA-CMPDEXEC-NA-ASYNC-COMMIT PIC S9(4) COMP-5 VALUE 61.
       77  SQLA-CMPDEXEC-NA-ASYNC-DIAG PIC S9(4) COMP-5 VALUE 62.
       77  SQLA-CMPDEXEC-NA-COMMIT-DIAG PIC S9(4) COMP-5 VALUE 63.
       77  SQLA-CMPDEXEC-NA-A-C-D    PIC S9(4) COMP-5 VALUE 64.
       77  SQLA-CLOSE-OPEN           PIC S9(4) COMP-5 VALUE 65.
       77  SQLA-CLOSE-OPEN-ASYNC     PIC S9(4) COMP-5 VALUE 66.
       77  SQLA-CLOSEREL-OPEN-ASYNC  PIC S9(4) COMP-5 VALUE 67.
       77  SQLA-CLOSEREL-OPEN        PIC S9(4) COMP-5 VALUE 68.
       77  SQLA-SETOPTION            PIC S9(4) COMP-5 VALUE 69.
       77  SQLA-DESCRIBE-CURSOR      PIC S9(4) COMP-5 VALUE 70.
       77  SQLA-DESCRIBE-CURSOR-ASYNC PIC S9(4) COMP-5 VALUE 71.
       77  SQLA-ARRAYINPUT           PIC S9(4) COMP-5 VALUE 72.
       77  SQLA-ARRAYINPUT-ASYNC     PIC S9(4) COMP-5 VALUE 73.
       77  SQLA-GETINFO              PIC S9(4) COMP-5 VALUE 74.
       77  SQLA-OPEN-WITH-RETURN     PIC S9(4) COMP-5 VALUE 75.
       77  SQLA-OPEN-WITH-RET-TO-CLIENT PIC S9(4) COMP-5 VALUE 76.
       77  SQLA-CALLPROC-NEW         PIC S9(4) COMP-5 VALUE 77.
       77  SQLA-CALLPROC-IMMED       PIC S9(4) COMP-5 VALUE 77.
       77  SQLA-DESCRIBE-INPUT       PIC S9(4) COMP-5 VALUE 78.
       77  SQLA-PREP-DESCRIBE-INPUT  PIC S9(4) COMP-5 VALUE 79.
       77  SQLA-EXECUTE-FREE-LOB-LOCATORS PIC S9(4) COMP-5 VALUE 80.
      * Set Current Package Path
       77  SQLA-SET-CURRENT-PKG-PATH PIC S9(4) COMP-5 VALUE 81.
      * The following two constants are for internal use only.  Do not use
      * them as values for sqlacall or similar API calls.
       77  SQLA-ALLOCATE             PIC S9(4) COMP-5 VALUE 82.
       77  SQLA-ASSOCIATE            PIC S9(4) COMP-5 VALUE 83.

      * Values returned in task[n].val when task[n].func is
      * SQLA_SQLCODE_COPY.
      * This is an output parameter of SQLACMPL.
      * Copy SQLCODE and/or SQLSTATE
       77  SQLA-SQLCODE              PIC S9(4) COMP-5 VALUE 1.
      * Copy PREP errors
       77  SQLA-SQLCODE-PREP-ERROR   PIC S9(4) COMP-5 VALUE 2.

      * Values assigned to token[n].use, an output parameter of SQLACMPL.
      * Input host var, no indicator var
       77  SQLA-INPUT-HVAR           PIC S9(4) COMP-5 VALUE 0.
      * Input host var followed by indicator var
       77  SQLA-INPUT-WITH-IND       PIC S9(4) COMP-5 VALUE 1.
      * Output host var, no indicator var
       77  SQLA-OUTPUT-HVAR          PIC S9(4) COMP-5 VALUE 2.
      * Output host var followed by indicator var
       77  SQLA-OUTPUT-WITH-IND      PIC S9(4) COMP-5 VALUE 3.
      * Indicator variable
       77  SQLA-INDICATOR            PIC S9(4) COMP-5 VALUE 4.
      * SQLDA structure
       77  SQLA-USER-SQLDA           PIC S9(4) COMP-5 VALUE 5.
      * Host variable type does not match use
       77  SQLA-INVALID-USE          PIC S9(4) COMP-5 VALUE 6.
      * Host variable token ID is not valid
       77  SQLA-INVALID-ID           PIC S9(4) COMP-5 VALUE 7.
      * Literal string
       77  SQLA-LITERAL              PIC S9(4) COMP-5 VALUE 8.
      * Host var was one of several fields expanded from a structure
      * reference
       77  SQLA-MULTIPLE-STRUCT-FIELD PIC S9(4) COMP-5 VALUE 9.
      * Host var was either atomic or a single field from a structure
      * reference
       77  SQLA-ATOMIC-FIELD         PIC S9(4) COMP-5 VALUE 10.

      * Values assigned to stmt_type, an output parameter of SQLACMPL.
       77  SQLA-TYPE-NULL-CONNECT    PIC S9(4) COMP-5 VALUE 0.
       77  SQLA-TYPE-CONN-EXCL-NO-PWD PIC S9(4) COMP-5 VALUE 1.
       77  SQLA-TYPE-CONN-EXCL-PWD   PIC S9(4) COMP-5 VALUE 2.
       77  SQLA-TYPE-CONN-EXCL-NO-PWD-SN PIC S9(4) COMP-5 VALUE 91.
       77  SQLA-TYPE-CONN-EXCL-PWD-SN PIC S9(4) COMP-5 VALUE 92.
       77  SQLA-TYPE-CONNECT-RESET   PIC S9(4) COMP-5 VALUE 3.
       77  SQLA-TYPE-CONN-SHARE-NO-PWD PIC S9(4) COMP-5 VALUE 4.
       77  SQLA-TYPE-CONN-SHARE-PWD  PIC S9(4) COMP-5 VALUE 5.
       77  SQLA-TYPE-CONNECT-USERID  PIC S9(4) COMP-5 VALUE 6.
       77  SQLA-TYPE-BEGIN           PIC S9(4) COMP-5 VALUE 10.
       77  SQLA-TYPE-BEGIN-COMPOUND  PIC S9(4) COMP-5 VALUE 12.
       77  SQLA-TYPE-CALLPROC        PIC S9(4) COMP-5 VALUE 13.
       77  SQLA-TYPE-CLOSE           PIC S9(4) COMP-5 VALUE 15.
       77  SQLA-TYPE-COMMIT          PIC S9(4) COMP-5 VALUE 20.
       77  SQLA-TYPE-CONNECT-1       PIC S9(4) COMP-5 VALUE 21.
       77  SQLA-TYPE-CONNECT-2       PIC S9(4) COMP-5 VALUE 22.
       77  SQLA-TYPE-DISCONNECT-DBNAME PIC S9(4) COMP-5 VALUE 23.
       77  SQLA-TYPE-DISCONNECT-CURRENT PIC S9(4) COMP-5 VALUE 24.
       77  SQLA-TYPE-DISCONNECT-ALL  PIC S9(4) COMP-5 VALUE 25.
       77  SQLA-TYPE-RELEASE-DBNAME  PIC S9(4) COMP-5 VALUE 26.
       77  SQLA-TYPE-RELEASE-CURRENT PIC S9(4) COMP-5 VALUE 27.
       77  SQLA-TYPE-RELEASE-ALL     PIC S9(4) COMP-5 VALUE 28.
       77  SQLA-TYPE-SET-CONNECTION  PIC S9(4) COMP-5 VALUE 29.
       77  SQLA-TYPE-DECLARE-SELECT  PIC S9(4) COMP-5 VALUE 30.
       77  SQLA-TYPE-DECLARE-SNAME   PIC S9(4) COMP-5 VALUE 40.
       77  SQLA-TYPE-ALLOCATE-CURSOR PIC S9(4) COMP-5 VALUE 45.
       77  SQLA-TYPE-DESCRIBE        PIC S9(4) COMP-5 VALUE 50.
       77  SQLA-TYPE-DESCRIBE-CURSOR PIC S9(4) COMP-5 VALUE 55.
       77  SQLA-TYPE-END             PIC S9(4) COMP-5 VALUE 60.
       77  SQLA-TYPE-END-COMPOUND    PIC S9(4) COMP-5 VALUE 65.
       77  SQLA-TYPE-EXECUTE         PIC S9(4) COMP-5 VALUE 70.
       77  SQLA-TYPE-EXECUTE-IMM     PIC S9(4) COMP-5 VALUE 80.
       77  SQLA-TYPE-FETCH           PIC S9(4) COMP-5 VALUE 90.
       77  SQLA-TYPE-FREE-LOB-LOCATOR PIC S9(4) COMP-5 VALUE 95.
       77  SQLA-TYPE-INCLUDE-SQLCA   PIC S9(4) COMP-5 VALUE 100.
       77  SQLA-TYPE-INCLUDE-SQLDA   PIC S9(4) COMP-5 VALUE 110.
       77  SQLA-TYPE-INCLUDE-TEXTFILE PIC S9(4) COMP-5 VALUE 115.
       77  SQLA-TYPE-OPEN            PIC S9(4) COMP-5 VALUE 120.
       77  SQLA-TYPE-PREPARE         PIC S9(4) COMP-5 VALUE 130.
       77  SQLA-TYPE-ROLLBACK        PIC S9(4) COMP-5 VALUE 140.
       77  SQLA-TYPE-SELECT          PIC S9(4) COMP-5 VALUE 150.
       77  SQLA-TYPE-SELECT-INTO     PIC S9(4) COMP-5 VALUE 160.
       77  SQLA-TYPE-SET-CURRENT-PKGSET PIC S9(4) COMP-5 VALUE 163.
       77  SQLA-TYPE-WHEN-NOT-FOUND  PIC S9(4) COMP-5 VALUE 170.
       77  SQLA-TYPE-WHEN-SQLERROR   PIC S9(4) COMP-5 VALUE 180.
       77  SQLA-TYPE-WHEN-SQLWARNING PIC S9(4) COMP-5 VALUE 190.
       77  SQLA-TYPE-SET-CURRENT-PKG-PATH PIC S9(4) COMP-5 VALUE 195.
       77  SQLA-TYPE-OTHER           PIC S9(4) COMP-5 VALUE 200.

      * Values used for term_option, an input parameter of SQLAFINI.
      * Save the access plan and/or bind file
       77  SQLA-SAVE                 PIC S9(4) COMP-5 VALUE 1.
      * Discard the access plan and/or bind file
       77  SQLA-DISCARD              PIC S9(4) COMP-5 VALUE 0.

      * Miscellaneous Constants

      * size of SQLAINIT file name buffer
       77  SQL-MAX-FILE-NAME-LENGTH  PIC S9(4) COMP-5 VALUE 128.

      * size of SQLACMPL output buffers
       77  SQL-CMPL-BUFSIZ           PIC S9(4) COMP-5 VALUE 256.
      * length of precompiler pid for V6.1 or earlier
       77  SQL-PID-LENGTH-V610       PIC S9(4) COMP-5 VALUE 40.
      * length of precompiler pid
       77  SQL-PID-LENGTH            PIC S9(4) COMP-5 VALUE 162.
      * minimum length of precompiler pid
       77  SQL-MIN-PID-LENGTH        PIC S9(4) COMP-5 VALUE 34.
      * Message array length same as SQLH_MAXMSGS
       77  SQLA-FLAG-MAXMSGS         PIC S9(4) COMP-5 VALUE 10.
      * Current SQL Flagger version
       77  SQLA-FLAG-VERSION         PIC S9(4) COMP-5 VALUE 1.

      * Return codes from Precompiler Services and Runtime Services
      * functions.
      * sqlca pointer/length invalid
       77  SQLA-SQLCA-BAD            PIC S9(4) COMP-5 VALUE -1.
      * check sqlca completion status
       77  SQLA-CHECK-SQLCA          PIC S9(4) COMP-5 VALUE 0.

      * Values returned in sqlca.sqlcode by Precompiler or Runtime Services
       77  SQLA-RC-OK                PIC S9(4) COMP-5 VALUE 0.
       77  SQLA-RC-OPTION-IGNORED    PIC S9(4) COMP-5 VALUE 20.
       77  SQLA-RC-CHAR-BAD          PIC S9(4) COMP-5 VALUE -7.
       77  SQLA-RC-STRING-NOT-TERMINATED PIC S9(4) COMP-5 VALUE -10.
       77  SQLA-RC-EMPTY-DEL-IDENT   PIC S9(4) COMP-5 VALUE -13.
       77  SQLA-RC-BFILE-OPEN-ERROR  PIC S9(4) COMP-5 VALUE -31.
       77  SQLA-RC-BFILE-DISK-ERROR  PIC S9(4) COMP-5 VALUE -32.
       77  SQLA-RC-SECTION-LIMIT     PIC S9(4) COMP-5 VALUE -51.
       77  SQLA-RC-MEMORY-BAD        PIC S9(4) COMP-5 VALUE -83.
       77  SQLA-RC-SNAME-DUP         PIC S9(4) COMP-5 VALUE -85.
       77  SQLA-RC-NO-STRUCT         PIC S9(4) COMP-5 VALUE -87.
       77  SQLA-RC-AMBIG-HOSTVAR     PIC S9(4) COMP-5 VALUE -88.
       77  SQLA-RC-STMT-LIMIT        PIC S9(4) COMP-5 VALUE -101.
       77  SQLA-RC-NUMBER-BAD        PIC S9(4) COMP-5 VALUE -103.
       77  SQLA-RC-STMT-SYNTAX-BAD   PIC S9(4) COMP-5 VALUE -104.
      * DBCS only
       77  SQLA-RC-GSTRING-BAD       PIC S9(4) COMP-5 VALUE -105.
       77  SQLA-RC-IDENTIFIER-LIMIT  PIC S9(4) COMP-5 VALUE -107.
       77  SQLA-RC-QUALIFIER-BAD     PIC S9(4) COMP-5 VALUE -108.
       77  SQLA-RC-DDSBAD            PIC S9(4) COMP-5 VALUE -142.
       77  SQLA-RC-DDSIGN            PIC S9(4) COMP-5 VALUE 143.
       77  SQLA-RC-DDSWRN            PIC S9(4) COMP-5 VALUE 143.
       77  SQLA-RC-STMT-IGNORED      PIC S9(4) COMP-5 VALUE 143.
       77  SQLA-RC-KEYWORD-BAD       PIC S9(4) COMP-5 VALUE -199.
       77  SQLA-RC-HVAR-NOT-DEC      PIC S9(4) COMP-5 VALUE -306.
       77  SQLA-RC-HVAR-DUP-NAME     PIC S9(4) COMP-5 VALUE -307.
       77  SQLA-RC-HVAR-LIMIT        PIC S9(4) COMP-5 VALUE -308.
       77  SQLA-RC-STMT-HVAR-LIMIT   PIC S9(4) COMP-5 VALUE -310.
       77  SQLA-RC-HVAR-USE-BAD      PIC S9(4) COMP-5 VALUE -324.
       77  SQLA-RC-CURSOR-DUP        PIC S9(4) COMP-5 VALUE -505.
       77  SQLA-RC-STMT-MODIFY-ALL   PIC S9(4) COMP-5 VALUE 513.
       77  SQLA-RC-INVALID-TRIGGER-STMT PIC S9(4) COMP-5 VALUE -797.
       77  SQLA-RC-SQLDA-SQLD-ERR    PIC S9(4) COMP-5 VALUE -804.
       77  SQLA-RC-SQLVAR-TYPE-ERR   PIC S9(4) COMP-5 VALUE -804.
       77  SQLA-RC-DISK-FULL         PIC S9(4) COMP-5 VALUE -968.
       77  SQLA-RC-NODE-FAILURE      PIC S9(4) COMP-5 VALUE -1229.
       77  SQLA-RC-CMPD-NESTED       PIC S9(4) COMP-5 VALUE -4010.
       77  SQLA-RC-CMPD-INVALID-STMT PIC S9(4) COMP-5 VALUE -4011.
       77  SQLA-RC-CMPD-INVALID-COMMIT PIC S9(4) COMP-5 VALUE -4012.
       77  SQLA-RC-CMPD-INVALID-END  PIC S9(4) COMP-5 VALUE -4013.
       77  SQLA-RC-INVALID-CMPD-SYNTAX PIC S9(4) COMP-5 VALUE -4014.
       77  SQLA-RC-FATAL-ERROR       PIC S9(4) COMP-5 VALUE -4901.
       77  SQLA-RC-PARM-CHARS-BAD    PIC S9(4) COMP-5 VALUE -4902.
       77  SQLA-RC-PARM-LENGTH-BAD   PIC S9(4) COMP-5 VALUE -4903.
       77  SQLA-RC-PARM-POINTER-BAD  PIC S9(4) COMP-5 VALUE -4904.
       77  SQLA-RC-PARM-RANGE-BAD    PIC S9(4) COMP-5 VALUE -4905.
       77  SQLA-RC-HVAR-SQLTYPE-BAD  PIC S9(4) COMP-5 VALUE -4911.
       77  SQLA-RC-HVAR-SQLLEN-BAD   PIC S9(4) COMP-5 VALUE -4912.
       77  SQLA-RC-HVAR-TOKEN-ID-DUP PIC S9(4) COMP-5 VALUE -4913.
       77  SQLA-RC-HVAR-TOKEN-ID-BAD PIC S9(4) COMP-5 VALUE -4914.
       77  SQLA-RC-INIT-DUP          PIC S9(4) COMP-5 VALUE -4915.
       77  SQLA-RC-INIT-REQUIRED     PIC S9(4) COMP-5 VALUE -4916.
       77  SQLA-RC-OPTION-BAD        PIC S9(4) COMP-5 VALUE -4917.
       77  SQLA-RC-TERM-OPTION-BAD   PIC S9(4) COMP-5 VALUE -4918.
       77  SQLA-RC-TASK-ARRAY-LIMIT  PIC S9(4) COMP-5 VALUE -4919.
       77  SQLA-RC-TOKEN-ARRAY-LIMIT PIC S9(4) COMP-5 VALUE -4920.
       77  SQLA-RC-OPTION-GEN-INVALID PIC S9(9) COMP-5 VALUE -30104.
      * Prep version of -109
       77  SQLA-RC-STMT-CLAUSE-BAD   PIC S9(4) COMP-5 VALUE -4940.
      * Prep version of -198
       77  SQLA-RC-STMT-BLANK        PIC S9(4) COMP-5 VALUE -4941.
      * Prep version of -303
       77  SQLA-RC-SELECT-HVAR-TYPE-BAD PIC S9(4) COMP-5 VALUE -4942.
      * Prep version of -313
       77  SQLA-RC-SELECT-LIST-BAD   PIC S9(4) COMP-5 VALUE 4943.
      * Prep version of -407
       77  SQLA-RC-COLUMN-NOT-NULLABLE PIC S9(4) COMP-5 VALUE -4944.
      * Prep version of -418
       77  SQLA-RC-STMT-MARKER-BAD   PIC S9(4) COMP-5 VALUE -4945.
      * Prep version of -504
       77  SQLA-RC-CURSOR-NOT-DECLARED PIC S9(4) COMP-5 VALUE -4946.
       77  SQLA-RC-NO-USERDA-IN-CMPD PIC S9(4) COMP-5 VALUE -4950.
       77  SQLA-RC-SQLDA-ID-BAD      PIC S9(4) COMP-5 VALUE -4951.
       77  SQLA-RC-SQLVAR-INDEX-BAD  PIC S9(4) COMP-5 VALUE -4952.
       77  SQLA-RC-CALL-TYPE-BAD     PIC S9(4) COMP-5 VALUE -4953.
       77  SQLA-RC-SECTION-BAD       PIC S9(4) COMP-5 VALUE -4954.

      * Temporary value used in sqlca.sqlcode after an SQLAALOC function.
      * Runtime code can check sqlca.sqlcode to determine whether or not the
      * SQLASETV function needs to be called to initialize the sqlda sqlvar
      * elements.
      * sqlvars already initialized
       77  SQLA-RC-SQLVARS-SET       PIC S9(4) COMP-5 VALUE 4959.

      * Errors that cause Precompiler Services to enter the fatal error
      * state. After encountering one of these types of errors, Precompiler
      * Services requires an SQLAFINI and SQLAINIT call before it will
      * continue.

       77  SQLA-RC-INV-INSERT        PIC S9(4) COMP-5 VALUE -803.
       77  SQLA-RC-INV-SQLDA-ADR     PIC S9(4) COMP-5 VALUE -822.
       77  SQLA-RC-SYS-ERROR         PIC S9(4) COMP-5 VALUE -902.
       77  SQLA-RC-DEADLOCK-ERR      PIC S9(4) COMP-5 VALUE -911.
       77  SQLA-RC-FAT-SYS-ERR       PIC S9(4) COMP-5 VALUE -912.
       77  SQLA-RC-TOO-MANY-LKS      PIC S9(4) COMP-5 VALUE -930.
       77  SQLA-RC-STORAGE-ERR       PIC S9(4) COMP-5 VALUE -954.
       77  SQLA-RC-DB-HEAP-ERR       PIC S9(4) COMP-5 VALUE -956.
       77  SQLA-RC-TOOMANY-OFLS      PIC S9(4) COMP-5 VALUE -958.
       77  SQLA-RC-TOOMANY-FILES     PIC S9(4) COMP-5 VALUE -960.
       77  SQLA-RC-LOG-FULL          PIC S9(4) COMP-5 VALUE -964.
       77  SQLA-RC-FILE-SYS-FULL     PIC S9(4) COMP-5 VALUE -968.
       77  SQLA-RC-READ-ONLY-FIL     PIC S9(4) COMP-5 VALUE -970.

       77  SQLA-RC-INCORRECT-DSK     PIC S9(4) COMP-5 VALUE -972.
       77  SQLA-RC-DB-DRV-LOCKED     PIC S9(4) COMP-5 VALUE -974.
       77  SQLA-RC-DRV-DOOR-OPEN     PIC S9(4) COMP-5 VALUE -976.
       77  SQLA-RC-DISK-WRT-PRO      PIC S9(4) COMP-5 VALUE -978.
       77  SQLA-RC-FILESYSTEM-ACCESS PIC S9(9) COMP-5 VALUE -10019.

       77  SQLA-RC-DISK-ERROR        PIC S9(4) COMP-5 VALUE -980.
       77  SQLA-RC-DISK-ERR          PIC S9(4) COMP-5 VALUE -982.
       77  SQLA-RC-COMM-RB-ERR       PIC S9(4) COMP-5 VALUE -984.
       77  SQLA-RC-CAT-FILE-ERR      PIC S9(4) COMP-5 VALUE -985.
       77  SQLA-RC-TAB-FILE-ERR      PIC S9(4) COMP-5 VALUE -986.
       77  SQLA-RC-INDEX-ERR         PIC S9(4) COMP-5 VALUE -990.
       77  SQLA-RC-REL-NUM-BAD       PIC S9(4) COMP-5 VALUE -992.
       77  SQLA-RC-AGENT-GONE        PIC S9(4) COMP-5 VALUE -1224.
      * Prep version of -952
       77  SQLA-RC-CTRL-BREAK        PIC S9(4) COMP-5 VALUE -4994.
      * Territory info not found
       77  SQLA-RC-CODEPAGE-BAD      PIC S9(4) COMP-5 VALUE -4995.
      * Prep version of -1046
       77  SQLA-RC-SQLUSER-BAD       PIC S9(4) COMP-5 VALUE -4997.
      * Prep version of -1049
       77  SQLA-RC-DB-DISCONNECTED   PIC S9(4) COMP-5 VALUE -4998.
      * Precompiler Services err
       77  SQLA-RC-INTERNAL-ERR      PIC S9(4) COMP-5 VALUE -4999.

      *************************************************************************
      * Target API constants for sqlaopts()
      **************************************************************************
      * Precompiler Services API (sqlainit)
       77  SQLAO-PREP-SVCS-API       PIC S9(4) COMP-5 VALUE 0.
      * Precompile API (sqlaprep)
       77  SQLAO-PREP-API            PIC S9(4) COMP-5 VALUE 1.
      * Binder API (sqlabndx)
       77  SQLAO-BIND-API            PIC S9(4) COMP-5 VALUE 2.

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
           05 SQLUSER                PIC X(8)
                                     USAGE DISPLAY NATIVE.
           05 SQL-PLANNAME           PIC X(8)
                                     USAGE DISPLAY NATIVE.
           05 SQL-CONTOKEN           PIC X(8)
                                     USAGE DISPLAY NATIVE.
           05 SQL-BUFFER             PIC X(8)
                                     USAGE DISPLAY NATIVE.

       01 SQL-SQLA-PROGRAM-ID.
           05 SQL-LENGTH             PIC 9(4) COMP-5.
           05 SQL-RP-REL-NUM         PIC 9(4) COMP-5.
           05 SQL-DB-REL-NUM         PIC 9(4) COMP-5.
           05 SQL-BF-REL-NUM         PIC 9(4) COMP-5.
           05 SQLUSER-LEN            PIC 9(4) COMP-5.
           05 SQLUSER                PIC X(128)
                                     USAGE DISPLAY NATIVE.
           05 SQL-PLANNAME           PIC X(8)
                                     USAGE DISPLAY NATIVE.
           05 SQL-CONTOKEN           PIC X(8)
                                     USAGE DISPLAY NATIVE.
           05 SQL-BUFFER             PIC X(8)
                                     USAGE DISPLAY NATIVE.

       01 SQL-SQLA-PROGRAM-ID.
           05 SQL-LENGTH             PIC 9(4) COMP-5.
           05 SQL-RP-REL-NUM         PIC 9(4) COMP-5.
           05 SQL-DB-REL-NUM         PIC 9(4) COMP-5.
           05 SQL-BF-REL-NUM         PIC 9(4) COMP-5.
           05 SQL-PLANNAME           PIC X(8)
                                     USAGE DISPLAY NATIVE.
           05 SQL-CONTOKEN           PIC X(8)
                                     USAGE DISPLAY NATIVE.
           05 SQL-BUFFER             PIC X(8)
                                     USAGE DISPLAY NATIVE.
           05 SQLUSER-LEN            PIC 9(4) COMP-5.
           05 SQLUSER                PIC X(128)
                                     USAGE DISPLAY NATIVE.

      * Indexes into the buffer field, showing the information it provides
       77  SQLA-PGMID-STD            PIC S9(4) COMP-5 VALUE 1.
       77  SQLA-PGMID-CONNECT        PIC S9(4) COMP-5 VALUE 2.
       77  SQLA-PGMID-SQLRULES       PIC S9(4) COMP-5 VALUE 3.
       77  SQLA-PGMID-DISCONNECT     PIC S9(4) COMP-5 VALUE 4.
       77  SQLA-PGMID-SYNCPOINT      PIC S9(4) COMP-5 VALUE 5.
       77  SQLA-PGMID-NETBIOS        PIC S9(4) COMP-5 VALUE 6.
       77  SQLA-PGMID-DEFERRED-PREPARE PIC S9(4) COMP-5 VALUE 7.
       77  SQLA-INT-TO-ASCII-DIFF    PIC S9(4) COMP-5 VALUE 48.

      * Number of DUOW connection options
       77  SQLA-PGMID-CONNOPTS       PIC S9(4) COMP-5 VALUE 6.

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
          05 FILLER                     PIC X(2)
                                        USAGE DISPLAY NATIVE.
          05 SQLFLAG-MSGS. 
             10 SQLFLAG-MSGS-COUNT      PIC 9(4) COMP-5.
             10 FILLER                  PIC X(2)
                                        USAGE DISPLAY NATIVE.
             10 SQLFLAG-MSGS-SQLCA OCCURS 10 TIMES.
                20 SQLFLAG-SQLCAID pic X(8) value "SQLCA   "
                                   USAGE DISPLAY NATIVE.
                20 SQLFLAG-SQLCABC pic S9(9) comp-5 value 136.
                20 SQLFLAG-SQLCODE pic S9(9) comp-5.
                20 SQLFLAG-SQLERRM.
                   49 SQLFLAG-SQLERRML PIC S9(4) comp-5.
                   49 SQLFLAG-SQLERRMC PIC X(70)
                                       USAGE DISPLAY NATIVE.
                20 SQLFLAG-SQLERRP PIC X(8)
                                   USAGE DISPLAY NATIVE.
                20 SQLFLAG-SQLERRD OCCURS 6 TIMES PIC S9(9) comp-5.
                20 SQLFLAG-SQLWARN USAGE DISPLAY NATIVE.
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
                20 SQLFLAG-SQLSTATE PIC X(5)
                                    USAGE DISPLAY NATIVE.
