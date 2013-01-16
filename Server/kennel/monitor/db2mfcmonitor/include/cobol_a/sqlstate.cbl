      *************************************************************************
      * 
      * Source File Name: sqlstate
      * 
      * (C) COPYRIGHT International Business Machines Corp. 1991, 1997
      * All Rights Reserved
      * Licensed Materials - Property of IBM
      * 
      * US Government Users Restricted Rights - Use, duplication or
      * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
      * 
      * Function: Include File defining:
      *             Labels for the SQL Council's SQL States
      * 
      * Operating System:  Windows NT
      * 
      * 
      * The last five bytes of the SQLCA contain a diagnostic code called
      * SQLSTATE, which is consistent across all IBM SQL products.  The first
      * character of SQLSTATE indicates whether the execution of the SQL
      * statement was successful or unsuccessful ('0' or not).  The first two
      * characters of SQLSTATE represent classes of successful and unsuccessful
      * execution conditions (with '00' through '49' reserved for ANS-defined
      * classes). The class codes and their meanings are:
      * 
      * 00  Unqualified Successful Completion
      * 01  Warning
      * 02  No Data
      * 03  SQL Statement Not Yet Complete
      * 07  Dynamic SQL Error
      * 08  Connection Exception
      * 09  Triggered Action Exception
      * 0A  Feature Not Supported
      * 0D  Invalid Target Type Specification
      * 0E  Invalid Schema Name List Specification
      * 0F  Invalid Token
      * 0W  Resignal When Handler Not Active
      * 0W  Prohibited Statement Encounetered During Trigger
      * 20  Case Not Found for Case Statement
      * 21  Cardinality Violation
      * 22  Data Exception
      * 23  Constraint Violation
      * 24  Invalid Cursor State
      * 24  Invalid Transaction State
      * 26  Invalid SQL Statement Identifier
      * 27  Triggered Data Change Violation
      * 28  Invalid Authorization Specification
      * 2B  Dependent Privilege Descriptors Still Exist
      * 2C  Invalid Character Set Name
      * 2D  Invalid Transaction Termination
      * 2E  Invalid Connection Name
      * 2F  SQL Function Exception
      * 33  Invalid SQL Descriptor Name
      * 34  Invalid Cursor Name
      * 35  Invalid Condition Number
      * 37  Syntax Error
      * 38  External Function Exception
      * 39  External Function Call Exception
      * 3B  Savepoint Exception
      * 3C  Ambiguous Cursor Name
      * 3D  Invalid Catalog Name
      * 3E  Invalid Schema (Collection) Name
      * 40  Serialization Failure
      * 42  Authorization Violation
      * 51  Invalid Application State
      * 52  Duplicate or Undefined Name
      * 53  Invalid Operand or Inconsistent Specification
      * 54  SQL or Product Limit Exceeded
      * 55  Object Not in Prerequisite State
      * 56  Miscellaneous SQL or Product Restriction
      * 57  Resource Not Available or Operator Intervention
      * 58  System Error
      * 
      **************************************************************************
      * SQLSTATE CLASSES
       77  SQLSTATE-CLASS-SUCCESS              PIC X(2) value "00".
       77  SQLSTATE-CLASS-WARNING              PIC X(2) value "01".
       77  SQLSTATE-CLASS-NO-DATA              PIC X(2) value "02".
       77  SQLSTATE-CLASS-CONNECT-EXCEPT       PIC X(2) value "08".
       77  SQLSTATE-CLASS-TRIGGER-EXCEPT       PIC X(2) value "09".
       77  SQLSTATE-CLASS-FEATURE-EXCEPT       PIC X(2) value "0A".
       77  SQLSTATE-CLASS-TYPE-EXCEPT          PIC X(2) value "0D".
       77  SQLSTATE-CLASS-SCHEMA-EXCEPT        PIC X(2) value "0E".
       77  SQLSTATE-CLASS-INVALID-TOKEN        PIC X(2) value "0F".
       77  SQLSTATE-CLASS-TRIG-PROHIBIT        PIC X(2) value "0W".
       77  SQLSTATE-CLASS-CASE-EXCEPT          PIC X(2) value "20".
       77  SQLSTATE-CLASS-CARDINALITY          PIC X(2) value "21".
       77  SQLSTATE-CLASS-DATA-EXCEPT          PIC X(2) value "22".
       77  SQLSTATE-CLASS-CONSTRAINT           PIC X(2) value "23".
       77  SQLSTATE-CLASS-INV-CURS-STATE       PIC X(2) value "24".
       77  SQLSTATE-CLASS-INV-STMT-ID          PIC X(2) value "26".
       77  SQLSTATE-CLASS-SYNTAX               PIC X(2) value "37".
       77  SQLSTATE-CLASS-SERIALIZATION        PIC X(2) value "40".
       77  SQLSTATE-CLASS-AUTHORZATION         PIC X(2) value "42".
       77  SQLSTATE-CLASS-INV-APP-STATE        PIC X(2) value "51".
       77  SQLSTATE-CLASS-DUP-UNDEF-NAME       PIC X(2) value "52".
       77  SQLSTATE-CLASS-INV-OPERAND          PIC X(2) value "53".
       77  SQLSTATE-CLASS-LIMIT                PIC X(2) value "54".
       77  SQLSTATE-CLASS-PREREQ-STATE         PIC X(2) value "55".
       77  SQLSTATE-CLASS-MISC-RESTRICT        PIC X(2) value "56".
       77  SQLSTATE-CLASS-RESOURCE             PIC X(2) value "57".
       77  SQLSTATE-CLASS-SYSTEM-ERROR         PIC X(2) value "58".
      * SQLSTATES
       77  SQL-EXEC-OK                         PIC X(5) value "00000".
       77  SQL-WARN-DISCONNECT-ERR             PIC X(5) value "01002".
       77  SQL-WARN-NULL-VAL-ELIMINATED        PIC X(5) value "01003".
       77  SQL-WARN-VAL-TRUNC                  PIC X(5) value "01004".
       77  SQL-WARN-INSUFF-ITEM-DESCRIPT       PIC X(5) value "01005".
       77  SQL-WARN-NOT-ALL-PRIVS-GRANTED      PIC X(5) value "01007".
       77  SQL-STATE-0100C                     PIC X(5) value "0100C".
       77  SQL-STATE-0100D                     PIC X(5) value "0100D".
       77  SQL-STATE-0100E                     PIC X(5) value "0100E".
       77  SQL-WARN-STR-TRUNC                  PIC X(5) value "01501".
       77  SQL-WARN-NULLS-ELIM                 PIC X(5) value "01502".
       77  SQL-WARN-COLS-TOOMANY               PIC X(5) value "01503".
       77  SQL-WARN-STMT-NOWHERE               PIC X(5) value "01504".
       77  SQL-WARN-STMT-NOTEXEC               PIC X(5) value "01505".
       77  SQL-WARN-DATE-ADJ                   PIC X(5) value "01506".
       77  SQL-WARN-CURSOR-NOT-BLOCKED         PIC X(5) value "01509".
       77  SQL-WARN-HOST-NULL-COL-RANGE        PIC X(5) value "01515".
       77  SQL-WARN-WITH-GRANT-IGNORED         PIC X(5) value "01516".
       77  SQL-WARN-CHAR-SUB                   PIC X(5) value "01517".
       77  SQL-WARN-HOST-NULL-NUM-RANGE        PIC X(5) value "01519".
       77  SQL-WARN-VAR-NOWNULL                PIC X(5) value "01520".
      * Planned
       77  SQL-WARN-TABLE-UNDEF                PIC X(5) value "01522".
       77  SQL-WARN-NULLS2-ELIM                PIC X(5) value "01524".
       77  SQL-WARN-NUM-INSERTS-INVALID        PIC X(5) value "01525".
       77  SQL-WARN-ISO-ESCAL                  PIC X(5) value "01526".
      * Planned
       77  SQL-WARN-REG-NOTEXIST               PIC X(5) value "01527".
      * Planned
       77  SQL-WARN-WHERE-IGNORE               PIC X(5) value "01528".
       77  SQL-WARN-NAME-UNDEF                 PIC X(5) value "01532".
       77  SQL-WARN-UNDEFINED-COLUMN-NAME      PIC X(5) value "01533".
       77  SQL-WARN-USE-SBCS-CHARS-ONLY        PIC X(5) value "01539".
       77  SQL-WARN-OP-NOPRIV                  PIC X(5) value "01542".
       77  SQL-WARN-REF-IGNORE                 PIC X(5) value "01543".
       77  SQL-WARN-NAME-INTERP                PIC X(5) value "01545".
       77  SQL-WARN-OBJ-NOPRIV                 PIC X(5) value "01548".
      * Planned
       77  SQL-WARN-PACK-PRIV                  PIC X(5) value "01549".
       77  SQL-WARN-INDEX-EXIST                PIC X(5) value "01550".
       77  SQL-WARN-GRANT-IGNORED              PIC X(5) value "01560".
       77  SQL-WARN-NEWLOGPATH-INV             PIC X(5) value "01562".
       77  SQL-WARN-CURR-LOGPATH-INV           PIC X(5) value "01563".
       77  SQL-WARN-HOST-NULL-DIV-ZERO         PIC X(5) value "01564".
       77  SQL-WARN-VAL-INCMPAT-WITH-COL       PIC X(5) value "01580".
       77  SQL-WARN-AUTO-CHECK-PEND-STATE      PIC X(5) value "01586".
       77  SQL-WARN-KEYWORD-REDUNDANT          PIC X(5) value "01589".
       77  SQL-WARN-VAL-MAY-BE-TRUNC           PIC X(5) value "01592".
       77  SQL-WARN-VAL-COULD-BE-TRUNC         PIC X(5) value "01593".
       77  SQL-WARN-INSUFF-DESCRIPTORS         PIC X(5) value "01594".
       77  SQL-WARN-OLD-VIEW-REPLACED          PIC X(5) value "01595".
       77  SQL-WARN-NO-COMPFUNC-FOR-LSTR       PIC X(5) value "01596".
       77  SQL-WARN-EVMON-STATE-NOT-SET        PIC X(5) value "01598".
       77  SQL-WARN-REBIND-OPTS-IGNORED        PIC X(5) value "01599".
       77  SQL-WARN-CONN-DISABLED              PIC X(5) value "01601".
       77  SQL-WARN-SUB-OPTIMAL-QUERY          PIC X(5) value "01602".
       77  SQL-WARN-CONSTRAINT-VIO-MOVED       PIC X(5) value "01603".
       77  SQL-WARN-EXPLAIN-MODE-RESTRICT      PIC X(5) value "01604".
       77  SQL-WARN-POSSIBLE-INFINIT-LOOP      PIC X(5) value "01605".
       77  SQL-WARN-DIR-EMPTY                  PIC X(5) value "01606".
       77  SQL-WARN-TIMEDIFF-EXCEEDED          PIC X(5) value "01607".
       77  SQL-WARN-CLAUSE-VAL-REPLACED        PIC X(5) value "01608".
       77  SQL-WARN-MAX-QUERY-RESULT-SET       PIC X(5) value "01609".
       77  SQL-WARN-MORE-RESULT-SETS           PIC X(5) value "01610".
       77  SQL-WARN-NEXT-RESULT-SET            PIC X(5) value "01611".
       77  SQL-STATE-01614                     PIC X(5) value "01614".
       77  SQL-WARN-COST-EXCEED-LIMIT          PIC X(5) value "01616".
       77  SQL-WARN-REDIST-REQUIRED            PIC X(5) value "01618".
       77  SQL-WARN-UNIONALL-TABLES-SAME       PIC X(5) value "01620".
       77  SQL-WARN-LOB-CHANGED                PIC X(5) value "01621".
       77  SQL-WARN-SYSTEM-ERROR               PIC X(5) value "01622".
       77  SQL-WARN-DEGREE-IGNORED             PIC X(5) value "01623".
       77  SQL-STATE-01625                     PIC X(5) value "01625".
       77  SQL-WARN-ONE-ACTIVE-BUFFPOOL        PIC X(5) value "01626".
       77  SQL-WARN-DATALINK-VAL-INVAL         PIC X(5) value "01627".
       77  SQL-WARN-RESOURCE-UNAVAIL           PIC X(5) value "01630".
       77  SQL-NUM-CONN-EXCEEDED               PIC X(5) value "01632".
       77  SQL-SUMM-TABLE-NO-OPTIMIZE          PIC X(5) value "01633".
       77  SQL-INTEGRITY-UNVERIFIED            PIC X(5) value "01636".
       77  SQL-DEBUGGER-NOT-ENABLED            PIC X(5) value "01637".
       77  SQL-STATE-01639                     PIC X(5) value "01639".
       77  SQL-STATE-01641                     PIC X(5) value "01641".
       77  SQL-STATE-01642                     PIC X(5) value "01642".
       77  SQL-STATE-01643                     PIC X(5) value "01643".
       77  SQL-STATE-01645                     PIC X(5) value "01645".
       77  SQL-STATE-01648                     PIC X(5) value "01648".
       77  SQL-STATE-01649                     PIC X(5) value "01649".
       77  SQL-STATE-01650                     PIC X(5) value "01650".
       77  SQL-STATE-01651                     PIC X(5) value "01651".
       77  SQL-STATE-01652                     PIC X(5) value "01652".
       77  SQL-STATE-01653                     PIC X(5) value "01653".
       77  SQL-STATE-01654                     PIC X(5) value "01654".
       77  SQL-STATE-01655                     PIC X(5) value "01655".
       77  SQL-STATE-01657                     PIC X(5) value "01657".
       77  SQL-WARN-INDEX-UNIQUE               PIC X(5) value "01661".
       77  SQL-STATE-01665                     PIC X(5) value "01665".
       77  SQL-STATE-01667                     PIC X(5) value "01667".
       77  SQL-STATE-01669                     PIC X(5) value "01669".
       77  SQL-STATE-01670                     PIC X(5) value "01670".
       77  SQL-STATE-01671                     PIC X(5) value "01671".
       77  SQL-WARN-INVALID-CURSOR-POS         PIC X(5) value "01S06".
       77  SQL-WARN-UDF-RETURNS-STATE          PIC X(5) value "01Hxx".
       77  SQL-NODATA-EXCEPTION                PIC X(5) value "02000".
       77  SQL-NODATA-UHOLE                    PIC X(5) value "02501".
       77  SQL-NODATA-DHOLE                    PIC X(5) value "02502".
       77  SQL-INV-NUM-HOST-VARS               PIC X(5) value "07001".
       77  SQL-INV-PARM-OR-CB                  PIC X(5) value "07002".
       77  SQL-INV-EXECUTE-STMT                PIC X(5) value "07003".
       77  SQL-USING-CLAUSE-REQUIRED           PIC X(5) value "07004".
       77  SQL-INV-PREP-STMT-CURSOR            PIC X(5) value "07005".
       77  SQL-HOSTVAR-NOT-USABLE              PIC X(5) value "07006".
       77  SQL-STATE-07007                     PIC X(5) value "07007".
       77  SQL-CONN-FAILED                     PIC X(5) value "08001".
       77  SQL-CONN-ALREADY-EXIST              PIC X(5) value "08002".
       77  SQL-CONN-DOES-NOT-EXIT              PIC X(5) value "08003".
       77  SQL-CONN-REJECTED-BY-SERVER         PIC X(5) value "08004".
       77  SQL-CONN-RESOLUTION-UNKNOWN         PIC X(5) value "08007".
       77  SQL-CONN-FAILED-NO-TM               PIC X(5) value "08502".
       77  SQL-STATE-08504                     PIC X(5) value "08504".
       77  SQL-STATE-08506                     PIC X(5) value "08506".
       77  SQL-TRIGGER-ERROR                   PIC X(5) value "09000".
       77  SQL-PROC-NOT-CONNECTABLE            PIC X(5) value "0A001".
       77  SQL-INSTANCE-NOT-ENABLED            PIC X(5) value "0A502".
       77  SQL-STATE-0A503                     PIC X(5) value "0A503".
       77  SQL-STATE-0D000                     PIC X(5) value "0D000".
       77  SQL-LOB-NOVALUE-TOKEN-VAR           PIC X(5) value "0F001".
       77  SQL-STATE-0K000                     PIC X(5) value "0K000".
       77  SQL-STATE-0W000                     PIC X(5) value "0W000".
       77  SQL-STATE-20000                     PIC X(5) value "20000".
       77  SQL-CARD-VALUE-TOOMANY              PIC X(5) value "21000".
       77  SQL-CARD-INS-MULTIROW               PIC X(5) value "21501".
       77  SQL-CARD-UPD-MULTIROW               PIC X(5) value "21502".
       77  SQL-CARD-DEL-MULTIROW               PIC X(5) value "21504".
       77  SQL-DATA-ONE-ROW                    PIC X(5) value "21505".
       77  SQL-STATE-21506                     PIC X(5) value "21506".
       77  SQL-DATA-CHAR-RTRUNC                PIC X(5) value "22001".
       77  SQL-DATA-INDICATOR-NULL             PIC X(5) value "22002".
       77  SQL-DATA-NUM-RANGE                  PIC X(5) value "22003".
       77  SQL-STATE-22004                     PIC X(5) value "22004".
      * Planned
       77  SQL-DATA-FETCH-INV                  PIC X(5) value "22006".
       77  SQL-DATA-FORMAT-INV                 PIC X(5) value "22007".
       77  SQL-DATA-DATETIME-OV                PIC X(5) value "22008".
       77  SQL-STATE-2200G                     PIC X(5) value "2200G".
       77  SQL-DATA-STR-RANGE                  PIC X(5) value "22011".
       77  SQL-DATA-DIV-ZERO                   PIC X(5) value "22012".
       77  SQL-DATA-INV-CAST-CHAR              PIC X(5) value "22018".
      * Planned
       77  SQL-DATA-LIKE-INV-CHAR              PIC X(5) value "22019".
       77  SQL-DATA-NULL-MISSING               PIC X(5) value "22024".
       77  SQL-DATA-LIKE-INV-ESC               PIC X(5) value "22025".
       77  SQL-NULL-INSTANCE-MUT               PIC X(5) value "2202D".
       77  SQL-STATE-2202H                     PIC X(5) value "2202H".
       77  SQL-DATA-LENGTH-RANGE               PIC X(5) value "22501".
       77  SQL-DATA-STR-INV                    PIC X(5) value "22503".
       77  SQL-DATA-DATA-BADFORM               PIC X(5) value "22504".
       77  SQL-DATA-DATETIME-REG-INV           PIC X(5) value "22506".
       77  SQL-DATA-TYPE-CANTUSE               PIC X(5) value "22507".
       77  SQL-DATA-TYPE-INCOMPAT              PIC X(5) value "22509".
       77  SQL-HOSTVAR-NEG-INDVAR              PIC X(5) value "22512".
       77  SQL-DATA-INDEX-DUP                  PIC X(5) value "22515".
       77  SQL-DATA-STR-CANTUSE-XLATE          PIC X(5) value "22517".
       77  SQL-DATA-STR-CANTASGN-XLATE         PIC X(5) value "22518".
       77  SQL-DATA-KEY-NOTEQUAL               PIC X(5) value "22520".
       77  SQL-DATA-CCSIDVAL-INV               PIC X(5) value "22522".
       77  SQL-DATA-CONVERSION-TRUNC           PIC X(5) value "22524".
       77  SQL-STATE-22526                     PIC X(5) value "22526".
       77  SQL-STATE-22527                     PIC X(5) value "22527".
       77  SQL-CONSTR-CHG-KEY-NOTALLOW         PIC X(5) value "23001".
       77  SQL-CONSTR-ROW-NOTSATISFY-VIEW      PIC X(5) value "23501".
       77  SQL-CONSTR-NULL-NOTALLOW            PIC X(5) value "23502".
       77  SQL-CONSTR-VALUE-INV                PIC X(5) value "23503".
       77  SQL-CONSTR-OP-RESTRICT              PIC X(5) value "23504".
       77  SQL-CONSTR-INDEX-UNIQUE             PIC X(5) value "23505".
       77  SQL-CONSTR-VIOLATION-RLST           PIC X(5) value "23510".
       77  SQL-CONSTR-PAR-ROW-DEL-NOALLOW      PIC X(5) value "23511".
       77  SQL-CONSTR-ROW-INVALID              PIC X(5) value "23512".
       77  SQL-CONSTR-ROW-NONCONFORM           PIC X(5) value "23513".
       77  SQL-CONSTR-VIOLATIONS               PIC X(5) value "23514".
       77  SQL-CONSTR-DUP-KEY-VALUES           PIC X(5) value "23515".
       77  SQL-CONSTR-FOREIGN-KEY-NOALLOW      PIC X(5) value "23520".
       77  SQL-CONSTR-UPDATE-CAT-STATS         PIC X(5) value "23521".
       77  SQL-IDENTITY-EXHAUSTED              PIC X(5) value "23522".
       77  SQL-STATE-23524                     PIC X(5) value "23524".
       77  SQL-STATE-23527                     PIC X(5) value "23527".
       77  SQL-CURSOR-NOTOPEN                  PIC X(5) value "24501".
       77  SQL-CURSOR-OPEN-ALREADY             PIC X(5) value "24502".
       77  SQL-CURSOR-NOTPOS                   PIC X(5) value "24504".
       77  SQL-CURSOR-OPEN-NO-COMMIT           PIC X(5) value "24505".
       77  SQL-CURSOR-PD-OPEN                  PIC X(5) value "24506".
      * Planned
       77  SQL-CURSOR-DELETED                  PIC X(5) value "24507".
       77  SQL-CURSOR-DHOLE                    PIC X(5) value "24510".
      * Planned
       77  SQL-CURSOR-UHOLE                    PIC X(5) value "24511".
      * Planned
       77  SQL-CURSOR-AGREE                    PIC X(5) value "24512".
       77  SQL-CURSOR-MOVED                    PIC X(5) value "24513".
       77  SQL-CURSOR-PREVIOUS-ERROR           PIC X(5) value "24514".
       77  SQL-CURSOR-ALREADY-ASSIGNED         PIC X(5) value "24516".
       77  SQL-STATE-24517                     PIC X(5) value "24517".
      * Planned
       77  SQL-INV-EXECUTION-ENVIRONMENT       PIC X(5) value "25000".
       77  SQL-STATE-25001                     PIC X(5) value "25001".
      * Planned
       77  SQL-ONLY-AS-FIRST-STATEMENT         PIC X(5) value "25501".
       77  SQL-MULTIPLE-OPERATIONS             PIC X(5) value "25502".
       77  SQL-IDENT-NOTEXIST                  PIC X(5) value "26501".
       77  SQL-IDENT-NOASSOC                   PIC X(5) value "26503".
       77  SQL-IDENT-SELECT                    PIC X(5) value "26504".
      * Planned
       77  SQL-IDENT-NOTSPEC                   PIC X(5) value "26506".
       77  SQL-IDENT-NOTSRSEL                  PIC X(5) value "26507".
      * Planned
       77  SQL-IDENT-NOTSELECT                 PIC X(5) value "26508".
      * Planned
       77  SQL-IDENT-DUP                       PIC X(5) value "26509".
      * Planned
       77  SQL-IDENT-HASCURSOR                 PIC X(5) value "26510".
       77  SQL-STATE-27000                     PIC X(5) value "27000".
       77  SQL-INV-AUTHORIZATION-SPEC          PIC X(5) value "28000".
       77  SQL-INV-OPERATION-FOR-ENVIR         PIC X(5) value "2D521".
       77  SQL-STATE-2D522                     PIC X(5) value "2D522".
       77  SQL-DYNAMIC-COMMIT-INVALID          PIC X(5) value "2D528".
       77  SQL-DYNAMIC-ROLLBACK-INVALID        PIC X(5) value "2D529".
       77  SQL-INV-CONN-NAME                   PIC X(5) value "2E000".
       77  SQL-INV-CURSOR-NAME                 PIC X(5) value "34000".
       77  SQL-STATE-36001                     PIC X(5) value "36001".
       77  SQL-SYNTAX-TOKEN-MISSING            PIC X(5) value "37501".
       77  SQL-SYNTAX-NAME-INVCHAR             PIC X(5) value "37502".
       77  SQL-SYNTAX-STR-UNTERM               PIC X(5) value "37503".
       77  SQL-SYNTAX-CONST-INV                PIC X(5) value "37504".
       77  SQL-SYNTAX-NUMARGS-INV              PIC X(5) value "37505".
      * Planned
       77  SQL-SYNTAX-HCONST-INV               PIC X(5) value "37506".
       77  SQL-SYNTAX-OP-INV-COL               PIC X(5) value "37507".
       77  SQL-SYNTAX-OP-MARKER                PIC X(5) value "37509".
       77  SQL-SYNTAX-MARKER-INV               PIC X(5) value "37510".
       77  SQL-SYNTAX-COL-INV                  PIC X(5) value "37511".
       77  SQL-SYNTAX-STMT-BAD                 PIC X(5) value "37512".
      * Planned
       77  SQL-SYNTAX-CLAUSE-EXCLUSIVE         PIC X(5) value "37513".
       77  SQL-SYNTAX-KEYWORD-DUP              PIC X(5) value "37514".
      * Planned
       77  SQL-SYNTAX-OPT-INV                  PIC X(5) value "37516".
       77  SQL-SYNTAX-STMT-EMPTY               PIC X(5) value "37517".
       77  SQL-SYNTAX-HOSTVAR-NOTALLOW         PIC X(5) value "37518".
      * Planned
       77  SQL-SYNTAX-READONLY-NOTALLOW        PIC X(5) value "37520".
       77  SQL-UDF-RETURNS-ERROR-STATE         PIC X(5) value "38xxx".
       77  SQL-UDF-SQL-NOT-ALLOWED             PIC X(5) value "38001".
       77  SQL-UDF-MODIFIES-NOT-ALLOWED        PIC X(5) value "38002".
       77  SQL-UDF-STMT-NOT-ALLOWED            PIC X(5) value "38003".
       77  SQL-UDF-READS-NOT-ALLOWED           PIC X(5) value "38004".
       77  SQL-UDF-CALL-ERROR                  PIC X(5) value "38501".
       77  SQL-UDF-EXCUTE-SQL-NOTALLOW         PIC X(5) value "38502".
       77  SQL-UDF-ABNORMAL-END                PIC X(5) value "38503".
       77  SQL-UDF-INTERRUPTED                 PIC X(5) value "38504".
       77  SQL-STATE-38505                     PIC X(5) value "38505".
       77  SQL-STATE-38506                     PIC X(5) value "38506".
       77  SQL-STATE-38553                     PIC X(5) value "38553".
       77  SQL-UDF-RETURNS-INV-STATE           PIC X(5) value "39001".
       77  SQL-STATE-39004                     PIC X(5) value "39004".
       77  SQL-UDF-RETURNS-RESULT-TOOLONG      PIC X(5) value "39501".
       77  SQL-OUT-SQLDA-CHANGED               PIC X(5) value "39502".
       77  SQL-STATE-3B001                     PIC X(5) value "3B001".
       77  SQL-STATE-3B002                     PIC X(5) value "3B002".
       77  SQL-STATE-3B501                     PIC X(5) value "3B501".
       77  SQL-STATE-3B502                     PIC X(5) value "3B502".
       77  SQL-STATE-3B503                     PIC X(5) value "3B503".
       77  SQL-SERIAL-ROLLBACK                 PIC X(5) value "40000".
       77  SQL-ROLLBK-AUTOMATIC                PIC X(5) value "40001".
       77  SQL-STMT-COMPLETION-UNKNOWN         PIC X(5) value "40003".
       77  SQL-ROLLBK-DIST-ENV                 PIC X(5) value "40502".
       77  SQL-ROLLBK-CONN-LOST                PIC X(5) value "40504".
       77  SQL-ROLLBK-TABLE                    PIC X(5) value "40506".
       77  SQL-STATE-40507                     PIC X(5) value "40507".
       77  SQL-ACCESS-AUTH-NOTPRIV-OBJ         PIC X(5) value "42501".
       77  SQL-ACCESS-AUTH-NOTPRIV-OP          PIC X(5) value "42502".
       77  SQL-ACCESS-AUTH-NOT-AUTHORIZED      PIC X(5) value "42503".
       77  SQL-ACCESS-PRIV-CANTREVOKE          PIC X(5) value "42504".
       77  SQL-ACCESS-AUTH-FAIL-CONN           PIC X(5) value "42505".
       77  SQL-ACCESS-AUTH-FAIL-OWNER          PIC X(5) value "42506".
       77  SQL-ACCESS-AUTH-FAIL-SERVER         PIC X(5) value "42507".
       77  SQL-ACCESS-PRIV-CANTPUBLIC          PIC X(5) value "42508".
       77  SQL-STAT-NOT-AUTH-STATICRULES       PIC X(5) value "42509".
       77  SQL-STATE-42511                     PIC X(5) value "42511".
       77  SQL-SYNTAX-KEYWORD-INCOMPAT         PIC X(5) value "42601".
       77  SQL-SYNTAX-INVALID                  PIC X(5) value "42601".
       77  SQL-INVALID-CHAR-IN-NAME            PIC X(5) value "42602".
       77  SQL-UNTERMINATED-STRING-CONST       PIC X(5) value "42603".
       77  SQL-INVALID-NUM-OR-STR-CONST        PIC X(5) value "42604".
       77  SQL-INVALID-NUM-ARGS                PIC X(5) value "42605".
       77  SQL-INVALID-HEX-CONST               PIC X(5) value "42606".
       77  SQL-INVALID-COL-FUNC-OPERAND        PIC X(5) value "42607".
       77  SQL-INV-USE-OF-NULL-OR-DFLT         PIC X(5) value "42608".
       77  SQL-ALL-OPERANDS-PARM-MARKERS       PIC X(5) value "42609".
       77  SQL-PARAMETER-MARKERS-INVALID       PIC X(5) value "42610".
       77  SQL-SYNTAX-LEN-SCALE-INV            PIC X(5) value "42611".
       77  SQL-STMT-NOT-IN-CONTEXT             PIC X(5) value "42612".
       77  SQL-SYNTAX-KEYWORD-CONFLICT         PIC X(5) value "42613".
       77  SQL-DUP-KEYWORD-INV                 PIC X(5) value "42614".
       77  SQL-INV-ALTERNATIVE                 PIC X(5) value "42615".
       77  SQL-STATE-42616                     PIC X(5) value "42616".
       77  SQL-STMT-STR-BLANK-OR-EMPTY         PIC X(5) value "42617".
       77  SQL-HOST-VAR-NOT-ALLOWED            PIC X(5) value "42618".
       77  SQL-STATE-42620                     PIC X(5) value "42620".
       77  SQL-CHECK-CONSTRAINT-INVALID        PIC X(5) value "42621".
       77  SQL-NAME-OR-LABEL-IS-TOO-LONG       PIC X(5) value "42622".
       77  SQL-IDENTITY-NO-DEFAULT             PIC X(5) value "42623".
       77  SQL-CASE-RES-NOT-EXPRESSION         PIC X(5) value "42625".
       77  SQL-RETURNS-EXPR-AS                 PIC X(5) value "42627".
       77  SQL-STATE-42628                     PIC X(5) value "42628".
       77  SQL-STATE-42629                     PIC X(5) value "42629".
       77  SQL-STATE-42630                     PIC X(5) value "42630".
       77  SQL-STATE-42631                     PIC X(5) value "42631".
       77  SQL-STATE-42632                     PIC X(5) value "42632".
       77  SQL-STATE-42634                     PIC X(5) value "42634".
       77  SQL-STATE-42635                     PIC X(5) value "42635".
       77  SQL-DUP-COL-IN-INS-OR-UPD           PIC X(5) value "42701".
       77  SQL-COLUMN-NAME-AMBIGUOUS           PIC X(5) value "42702".
       77  SQL-UNDEFINED-COLUMN-NAME           PIC X(5) value "42703".
       77  SQL-NAME-OBJECT-UNDEF               PIC X(5) value "42704".
       77  SQL-NAME-SERVER-UNDEFINED           PIC X(5) value "42705".
       77  SQL-UNNAMED-COLS-IN-RESULT-TBL      PIC X(5) value "42706".
       77  SQL-NAME-NOT-IN-RESULT-TABLE        PIC X(5) value "42707".
       77  SQL-NAME-DUP-COL-IN-KEY             PIC X(5) value "42709".
       77  SQL-NAME-DUP-OBJ-NAME               PIC X(5) value "42710".
       77  SQL-NAME-DUP-COL-NAME               PIC X(5) value "42711".
       77  SQL-NAME-DUP-IN-FROM-CLAUSE         PIC X(5) value "42712".
       77  SQL-DUP-OBJECT-IN-LIST              PIC X(5) value "42713".
       77  SQL-NAME-NODE-NOT-FOUND-IN-DIR      PIC X(5) value "42720".
       77  SQL-NAME-DUP-SIGNATURE              PIC X(5) value "42723".
       77  SQL-ACCESS-UNABLE                   PIC X(5) value "42724".
       77  SQL-NAME-AMBIGUOUS                  PIC X(5) value "42725".
       77  SQL-DUP-DERIVED-TABLE-NAME          PIC X(5) value "42726".
       77  SQL-NEED-DEFAULT-PRIM-TBLSPACE      PIC X(5) value "42727".
       77  SQL-ACCESS-DUPLICATE-NODE           PIC X(5) value "42728".
       77  SQL-ACCESS-UNDEFINED-NODE           PIC X(5) value "42729".
       77  SQL-CONTNR-NAME-USEDBY-ANOTHER      PIC X(5) value "42730".
       77  SQL-CONTNR-NAME-USED                PIC X(5) value "42731".
       77  SQL-DUP-SCHEMA-NAME-IN-PATH         PIC X(5) value "42732".
       77  SQL-STATE-42734                     PIC X(5) value "42734".
       77  SQL-NODEGROUP-NOT-DEFINED           PIC X(5) value "42735".
       77  SQL-STATE-42736                     PIC X(5) value "42736".
       77  SQL-STATE-42737                     PIC X(5) value "42737".
       77  SQL-STATE-42738                     PIC X(5) value "42738".
       77  SQL-TRANSFORM-EXISTS                PIC X(5) value "42739".
       77  SQL-NO-TRANSFORM-DROPPED            PIC X(5) value "42740".
       77  SQL-TRANSFORM-NOT-FOUND             PIC X(5) value "42741".
       77  SQL-SUBTABLE-EXISTS                 PIC X(5) value "42742".
       77  SQL-METHOD-NOT-FOUND                PIC X(5) value "42743".
       77  SQL-STATE-42744                     PIC X(5) value "42744".
       77  SQL-STATE-42745                     PIC X(5) value "42745".
       77  SQL-STATE-42746                     PIC X(5) value "42746".
       77  SQL-DUPLICATE-STORAGE-PATH          PIC X(5) value "42748".
       77  SQL-NUM-INSERTS-INVALID             PIC X(5) value "42802".
       77  SQL-COLUMN-REF-INVALID              PIC X(5) value "42803".
       77  SQL-CASE-RES-NOT-COMPATIBLE         PIC X(5) value "42804".
       77  SQL-ORDER-BY-INT-NOT-IN-RESULT      PIC X(5) value "42805".
       77  SQL-DATA-TYPES-INCOMPAT             PIC X(5) value "42806".
       77  SQL-OPERATION-NOTPERMIT-ON-OBJ      PIC X(5) value "42807".
       77  SQL-COLUMN-NOT-UPDATEABLE           PIC X(5) value "42808".
       77  SQL-STATEMENT-NOT-APPLICABLE        PIC X(5) value "42809".
       77  SQL-FOREIGN-KEY-HAS-VIEW            PIC X(5) value "42810".
       77  SQL-NUM-COLS-NOT-MATCH-SELECT       PIC X(5) value "42811".
       77  SQL-VIEW-NOTALLOW-WITH-CHECK        PIC X(5) value "42813".
       77  SQL-SYNTAX-LANG-TYPE-INV            PIC X(5) value "42815".
       77  SQL-INV-USE-DATATIME-OR-DUR         PIC X(5) value "42816".
       77  SQL-OPERANDS-NOT-COMPATIBLE         PIC X(5) value "42818".
       77  SQL-NUMBER-IS-REQUIRED              PIC X(5) value "42819".
       77  SQL-NUM-CONST-OUT-OF-RANGE          PIC X(5) value "42820".
       77  SQL-VALUE-INCOMPAT-WITH-COLUMN      PIC X(5) value "42821".
       77  SQL-BAD-GROUP-ORDER-BY-EXPR         PIC X(5) value "42822".
       77  SQL-MULT-COLS-IN-SELECT-SUBQ        PIC X(5) value "42823".
       77  SQL-LIKE-OPERAND-INVALID            PIC X(5) value "42824".
       77  SQL-UNION-OPERANDS-COL-INCOMP       PIC X(5) value "42825".
       77  SQL-NUM-COLUMNS-DO-NOT-MATCH        PIC X(5) value "42826".
       77  SQL-TABLE-DIFFERS-FROM-CURSOR       PIC X(5) value "42827".
       77  SQL-TABLE-CANNOT-BE-MODIFIED        PIC X(5) value "42828".
       77  SQL-CANNOT-MODIFY-RESULT-TBL        PIC X(5) value "42829".
       77  SQL-FOREIGNKEY-NOT-LIKE-PARENT      PIC X(5) value "42830".
       77  SQL-NO-NULLS-ALLOWED-IN-KEY         PIC X(5) value "42831".
       77  SQL-AUTH-OPER-SYSOBJ-NOTALLOW       PIC X(5) value "42832".
       77  SQL-NULL-VALUE-NOTALLOW             PIC X(5) value "42834".
       77  SQL-UDF-INVALID-HANDLE-PLACED       PIC X(5) value "42835".
       77  SQL-RECURS-NAME-DERIVE-TBL-INV      PIC X(5) value "42836".
       77  SQL-COL-ATTRIBUTE-INCOMPAT          PIC X(5) value "42837".
       77  SQL-INV-USE-OF-TABLESPACE           PIC X(5) value "42838".
       77  SQL-MUST-USE-SAME-TABLESPACE        PIC X(5) value "42839".
       77  SQL-AS-CAST-USE-ERROR               PIC X(5) value "42840".
       77  SQL-PARM-MARKER-INV-NAME            PIC X(5) value "42841".
       77  SQL-SYNTAX-TYPE-OPT-INV             PIC X(5) value "42842".
       77  SQL-INV-BOOL-IN-SELECT-LIST         PIC X(5) value "42844".
       77  SQL-INV-VARIANT-FUNCTION            PIC X(5) value "42845".
       77  SQL-INV-CAST                        PIC X(5) value "42846".
       77  SQL-STATE-42849                     PIC X(5) value "42849".
       77  SQL-PRIVS-INV-OR-INCONSIST          PIC X(5) value "42852".
       77  SQL-STATE-42853                     PIC X(5) value "42853".
       77  SQL-RES-COL-TYPE-INCOMPAT           PIC X(5) value "42854".
       77  SQL-LOB-ASSIGN-NOT-ALLOWED          PIC X(5) value "42855".
       77  SQL-STATE-42858                     PIC X(5) value "42858".
       77  SQL-SYNTAX-TYPE-NOTMATCH            PIC X(5) value "42866".
       77  SQL-STATE-42867                     PIC X(5) value "42867".
       77  SQL-STATE-42872                     PIC X(5) value "42872".
       77  SQL-INV-SCHEMA-NAME                 PIC X(5) value "42875".
       77  SQL-CANNOT-QUALIFY-COLUMN           PIC X(5) value "42877".
       77  SQL-SYNTAX-XTERN-NAME-INV           PIC X(5) value "42878".
       77  SQL-SYNTAX-FUNC-SRC-NOTMATCH        PIC X(5) value "42879".
       77  SQL-SYNTAX-CAST-ERROR               PIC X(5) value "42880".
       77  SQL-INVALID-USE-ROW-FUNC            PIC X(5) value "42881".
       77  SQL-NAME-SCHEMA-DIFF                PIC X(5) value "42882".
       77  SQL-NAME-FUNC-SIGNATURE-UNDEF       PIC X(5) value "42883".
       77  SQL-NAME-FUNC-ARGUMENT-UNDEF        PIC X(5) value "42884".
       77  SQL-SYNTAX-PARM-NUM-NOTMATCH        PIC X(5) value "42885".
       77  SQL-STATE-42886                     PIC X(5) value "42886".
       77  SQL-FUNC-INV-IN-CONTEXT             PIC X(5) value "42887".
       77  SQL-TABLE-HAS-NO-PRIMKEY            PIC X(5) value "42888".
       77  SQL-PRIMARY-KEY-ALREADY-EXIST       PIC X(5) value "42889".
       77  SQL-NOT-UNIQUE-CONSTRAINT-COLS      PIC X(5) value "42890".
       77  SQL-DUP-UNIQUE-CONSTRAINT           PIC X(5) value "42891".
       77  SQL-OBJ-DEPENDS-ON-THIS             PIC X(5) value "42893".
       77  SQL-DEFAULT-VALUE-INV               PIC X(5) value "42894".
       77  SQL-SYNTAX-HOSTVAR-TYPE-INCOMP      PIC X(5) value "42895".
       77  SQL-INV-CORRELATION-REF             PIC X(5) value "42898".
       77  SQL-CORRELATION-REF-NOT-ALLOW       PIC X(5) value "42899".
       77  SQL-ERROR-IN-UDF                    PIC X(5) value "428A0".
       77  SQL-NOACCESS-HOSTFILE-VAR-FILE      PIC X(5) value "428A1".
       77  SQL-ACCESS-PART-NULL                PIC X(5) value "428A2".
       77  SQL-EVMON-BAD-PATH-SPECIFIED        PIC X(5) value "428A3".
       77  SQL-EVMON-BAD-OPTION-SPECIFIED      PIC X(5) value "428A4".
       77  SQL-INV-EXCEPTION-TABLE             PIC X(5) value "428A5".
       77  SQL-EXCEP-TBL-SAME-AS-CHK-TBL       PIC X(5) value "428A6".
       77  SQL-NUM-CHECK-TBLS-MISMATCH         PIC X(5) value "428A7".
       77  SQL-PARENT-TBL-IN-CHCK-PEND-ST      PIC X(5) value "428A8".
       77  SQL-ACCESS-INVALID-RANGE            PIC X(5) value "428A9".
       77  SQL-STATE-428AA                     PIC X(5) value "428AA".
       77  SQL-ILLEGAL-ELMNT-NSTNG             PIC X(5) value "428B0".
       77  SQL-INCORRECT-NUM-ON-NODES          PIC X(5) value "428B1".
       77  SQL-CONTNR-PATH-NM-NOT-VALID        PIC X(5) value "428B2".
       77  SQL-INV-SQLSTATE-FROM-APP           PIC X(5) value "428B3".
       77  SQL-STATE-428B7                     PIC X(5) value "428B7".
       77  SQL-DROP-ONLY-NODE                  PIC X(5) value "428C0".
       77  SQL-IDENTITY-ONLY-ONE               PIC X(5) value "428C1".
       77  SQL-CREATE-MISSING-CLAUSE           PIC X(5) value "428C2".
       77  SQL-NUM-ELEMENTS-NO-MATCH           PIC X(5) value "428C4".
       77  SQL-DATATYPE-MAPPING-NOT-FOUND      PIC X(5) value "428C5".
       77  SQL-IDENTITY-NO-INSERT              PIC X(5) value "428C9".
       77  SQL-TABLE-CLUSTERING-INDEX          PIC X(5) value "428CA".
       77  SQL-TABLESPACE-BUFFPOOL-MATCH       PIC X(5) value "428CB".
       77  SQL-DATALINK-FILE-NO-ACCESS         PIC X(5) value "428D1".
       77  SQL-STATE-428D4                     PIC X(5) value "428D4".
       77  SQL-STATE-428D5                     PIC X(5) value "428D5".
       77  SQL-STATE-428D6                     PIC X(5) value "428D6".
       77  SQL-STATE-428D7                     PIC X(5) value "428D7".
       77  SQL-STATE-428D8                     PIC X(5) value "428D8".
       77  SQL-INVALID-SUPER-OBJECT            PIC X(5) value "428DB".
       77  SQL-TRANSFORM-FUNCTION-INV          PIC X(5) value "428DC".
       77  SQL-TRANSFORM-NOT-DEFINED           PIC X(5) value "428DD".
       77  SQL-PAGESIZE-NOT-SUPPORTED          PIC X(5) value "428DE".
       77  SQL-CREATE-CAST-TYPE-INVALID        PIC X(5) value "428DF".
       77  SQL-CREATE-CAST-FUNC-INVALID        PIC X(5) value "428DG".
       77  SQL-TYPED-TABLE-OP-INVALID          PIC X(5) value "428DH".
       77  SQL-INHERIT-COL-NO-DROP             PIC X(5) value "428DJ".
       77  SQL-COLUMN-SCOPE-DEFINED            PIC X(5) value "428DK".
       77  SQL-FUNC-PARM-SCOPE-DEFINED         PIC X(5) value "428DL".
       77  SQL-TABLE-SCOPE-INVALID             PIC X(5) value "428DM".
       77  SQL-RETURNS-SCOPE-NOT-SPEC          PIC X(5) value "428DN".
       77  SQL-NOT-STRUCTURED-TYPE             PIC X(5) value "428DP".
       77  SQL-SUBTABLE-DIFF-SCHEMA            PIC X(5) value "428DQ".
       77  SQL-SUBTABLE-OP-NO-APPLY            PIC X(5) value "428DR".
       77  SQL-CANNOT-DEF-SUBTABLE-INDEX       PIC X(5) value "428DS".
       77  SQL-OPERAND-INVALID-TYPE            PIC X(5) value "428DT".
       77  SQL-TYPE-NOT-INCLUDED               PIC X(5) value "428DU".
       77  SQL-INVALID-DEF-OPERAND             PIC X(5) value "428DV".
       77  SQL-CANNOT-REF-OBJECT-COL           PIC X(5) value "428DW".
       77  SQL-OBJECT-COL-REQ-ROOT             PIC X(5) value "428DX".
       77  SQL-NO-UPDATE-SUBTABLE-STATS        PIC X(5) value "428DY".
       77  SQL-OBJECT-COLUMN-NO-UPDATE         PIC X(5) value "428DZ".
       77  SQL-INDEX-DEF-NO-MATCH              PIC X(5) value "428E0".
       77  SQL-RANGE-FUNC-INCONSISTENT         PIC X(5) value "428E1".
       77  SQL-NUM-KEY-PARMS-MISMATCH          PIC X(5) value "428E2".
       77  SQL-INDEX-FUNC-ARG-INVALID          PIC X(5) value "428E3".
       77  SQL-FUNC-NOT-SUPP-CRTINDEX          PIC X(5) value "428E4".
       77  SQL-STATE-428E5                     PIC X(5) value "428E5".
       77  SQL-SEARCH-ARG-MISMATCH             PIC X(5) value "428E6".
       77  SQL-USER-DEF-PRED-MISMATCH          PIC X(5) value "428E7".
       77  SQL-SEARCH-TARGET-MISMATCH          PIC X(5) value "428E8".
       77  SQL-ARG-PARM-NO-BOTH                PIC X(5) value "428E9".
       77  SQL-INV-FULLSEL-TYPED-VIEW          PIC X(5) value "428EA".
       77  SQL-COLUMN-NO-READ-ONLY             PIC X(5) value "428EB".
       77  SQL-SUMM-TABLE-FULLSELECT-INV       PIC X(5) value "428EC".
       77  SQL-STRUCT-TYPE-CANNOT-CONST        PIC X(5) value "428ED".
       77  SQL-STATE-428EE                     PIC X(5) value "428EE".
       77  SQL-STATE-428EF                     PIC X(5) value "428EF".
       77  SQL-STATE-428EG                     PIC X(5) value "428EG".
       77  SQL-STATE-428EH                     PIC X(5) value "428EH".
       77  SQL-STATE-428EJ                     PIC X(5) value "428EJ".
       77  SQL-STATE-428EK                     PIC X(5) value "428EK".
       77  SQL-STATE-428EL                     PIC X(5) value "428EL".
       77  SQL-STATE-428EM                     PIC X(5) value "428EM".
       77  SQL-STATE-428EN                     PIC X(5) value "428EN".
       77  SQL-STATE-428EP                     PIC X(5) value "428EP".
       77  SQL-STATE-428EQ                     PIC X(5) value "428EQ".
       77  SQL-STATE-428ER                     PIC X(5) value "428ER".
       77  SQL-STATE-428ES                     PIC X(5) value "428ES".
       77  SQL-STATE-428EU                     PIC X(5) value "428EU".
       77  SQL-STATE-428EV                     PIC X(5) value "428EV".
       77  SQL-STATE-428EW                     PIC X(5) value "428EW".
       77  SQL-STATE-428EX                     PIC X(5) value "428EX".
       77  SQL-STATE-428EY                     PIC X(5) value "428EY".
       77  SQL-STATE-428EZ                     PIC X(5) value "428EZ".
       77  SQL-STATE-428F0                     PIC X(5) value "428F0".
       77  SQL-STATE-428F1                     PIC X(5) value "428F1".
       77  SQL-STATE-428F2                     PIC X(5) value "428F2".
       77  SQL-STATE-428F3                     PIC X(5) value "428F3".
       77  SQL-STATE-428F4                     PIC X(5) value "428F4".
       77  SQL-STATE-428F6                     PIC X(5) value "428F6".
       77  SQL-STATE-428F7                     PIC X(5) value "428F7".
       77  SQL-STATE-428F9                     PIC X(5) value "428F9".
       77  SQL-STATE-428FA                     PIC X(5) value "428FA".
       77  SQL-STATE-428FB                     PIC X(5) value "428FB".
       77  SQL-STATE-428FC                     PIC X(5) value "428FC".
       77  SQL-STATE-428FD                     PIC X(5) value "428FD".
       77  SQL-STATE-428FE                     PIC X(5) value "428FE".
       77  SQL-STATE-428FF                     PIC X(5) value "428FF".
       77  SQL-STATE-428FG                     PIC X(5) value "428FG".
       77  SQL-STATE-428FH                     PIC X(5) value "428FH".
       77  SQL-STATE-428FI                     PIC X(5) value "428FI".
       77  SQL-STATE-428FJ                     PIC X(5) value "428FJ".
       77  SQL-STATE-428FL                     PIC X(5) value "428FL".
       77  SQL-STATE-428FM                     PIC X(5) value "428FM".
       77  SQL-STATE-428FP                     PIC X(5) value "428FP".
       77  SQL-STATE-428FQ                     PIC X(5) value "428FQ".
       77  SQL-STATE-428FU                     PIC X(5) value "428FU".
       77  SQL-STATE-428FV                     PIC X(5) value "428FV".
       77  SQL-STATE-428FZ                     PIC X(5) value "428FZ".
       77  SQL-STATE-428G3                     PIC X(5) value "428G3".
       77  SQL-STATE-428G4                     PIC X(5) value "428G4".
       77  SQL-STATE-428G5                     PIC X(5) value "428G5".
       77  SQL-STATE-428G6                     PIC X(5) value "428G6".
       77  SQL-STATE-428G8                     PIC X(5) value "428G8".
       77  SQL-STATE-428G9                     PIC X(5) value "428G9".
       77  SQL-STATE-428GA                     PIC X(5) value "428GA".
       77  SQL-COL-FUNC-NEEDS-COL-NAME         PIC X(5) value "42901".
       77  SQL-OBJECT-ALSO-IN-FROM-CLAUSE      PIC X(5) value "42902".
       77  SQL-WHERE-OR-SET-HAS-INV-REF        PIC X(5) value "42903".
       77  SQL-STATE-42904                     PIC X(5) value "42904".
       77  SQL-STRING-IS-TOO-LONG              PIC X(5) value "42907".
       77  SQL-COLUMN-LIST-MISSING             PIC X(5) value "42908".
       77  SQL-STATE-42910                     PIC X(5) value "42910".
       77  SQL-INVALID-DECIMAL-DIVIDE          PIC X(5) value "42911".
       77  SQL-COLUMN-NOT-IN-UPD-CLAUSE        PIC X(5) value "42912".
       77  SQL-UPDATE-OR-DELETE-INVALID        PIC X(5) value "42913".
       77  SQL-TABLE-IN-SUBQUERY-AFFECTED      PIC X(5) value "42914".
       77  SQL-INV-REFERENTIAL-CONSTRAINT      PIC X(5) value "42915".
       77  SQL-REP-CHAIN-ALIAS-INVALID         PIC X(5) value "42916".
       77  SQL-CANNOT-EXPLICITLY-DROP-OBJ      PIC X(5) value "42917".
       77  SQL-NAME-RESERVED                   PIC X(5) value "42918".
       77  SQL-STATE-42919                     PIC X(5) value "42919".
       77  SQL-CANNOT-ADD-CONTAINERS           PIC X(5) value "42921".
       77  SQL-REC-NAME-DER-TLB-SPEC-INV       PIC X(5) value "42925".
       77  SQL-STATE-42928                     PIC X(5) value "42928".
       77  SQL-PROG-ASSUMPTIONS-INCORRECT      PIC X(5) value "42932".
       77  SQL-AUTH-CREATE-SYSOBJ-NOALLOW      PIC X(5) value "42939".
       77  SQL-SERVER-NAME-MISMATCH            PIC X(5) value "42961".
       77  SQL-INV-USE-LONG-COLUMN             PIC X(5) value "42962".
       77  SQL-NO-SW-LICENSE                   PIC X(5) value "42968".
       77  SQL-PACKAGE-NOT-CREATED             PIC X(5) value "42969".
       77  SQL-INV-JOIN-CONDITION              PIC X(5) value "42972".
       77  SQL-STATE-42985                     PIC X(5) value "42985".
       77  SQL-INV-CONN-RESET                  PIC X(5) value "42986".
       77  SQL-STMT-INV-FOR-STORPROC-TRIG      PIC X(5) value "42987".
       77  SQL-STATE-42989                     PIC X(5) value "42989".
       77  SQL-SYNTAX-SYSTEM-USE-ONLY          PIC X(5) value "42991".
       77  SQL-LOB-COL-TOO-BIG-TO-LOG          PIC X(5) value "42993".
       77  SQL-RAW-DEVICE-NOT-SUPP             PIC X(5) value "42994".
       77  SQL-FUNC-NO-GLOBAL-TEMP             PIC X(5) value "42995".
       77  SQL-UNIQUE-NOT-ALLOWED              PIC X(5) value "42997".
       77  SQL-INV-FK-REF                      PIC X(5) value "429A0".
       77  SQL-INV-NODEGROUP                   PIC X(5) value "429A1".
       77  SQL-DATAJOINER-NO-PROCESS           PIC X(5) value "429A9".
       77  SQL-INV-NOTLOGINIT                  PIC X(5) value "429AA".
       77  SQL-TYPE-LENGTH-TOO-SMALL           PIC X(5) value "429B2".
       77  SQL-NO-DEF-OBJECT-SUBTBL            PIC X(5) value "429B3".
       77  SQL-FILTER-FUNC-NO-LANG             PIC X(5) value "429B4".
       77  SQL-INSTANCE-PARM-TYPE-INV          PIC X(5) value "429B5".
       77  SQL-STATE-429B8                     PIC X(5) value "429B8".
       77  SQL-STATE-429B9                     PIC X(5) value "429B9".
       77  SQL-STATE-429BA                     PIC X(5) value "429BA".
       77  SQL-STATE-429BB                     PIC X(5) value "429BB".
       77  SQL-STATE-429BC                     PIC X(5) value "429BC".
       77  SQL-STATE-429BE                     PIC X(5) value "429BE".
       77  SQL-STATE-429BG                     PIC X(5) value "429BG".
       77  SQL-STATE-429BJ                     PIC X(5) value "429BJ".
       77  SQL-STATE-429BK                     PIC X(5) value "429BK".
       77  SQL-STATE-429BL                     PIC X(5) value "429BL".
       77  SQL-STATE-429BO                     PIC X(5) value "429BO".
       77  SQL-STATE-429BP                     PIC X(5) value "429BP".
       77  SQL-STATE-429DB                     PIC X(5) value "429DB".
       77  SQL-VIEW-DEF-NOT-SATISFIED          PIC X(5) value "44000".
       77  SQL-STATE-46001                     PIC X(5) value "46001".
       77  SQL-STATE-46002                     PIC X(5) value "46002".
       77  SQL-STATE-46003                     PIC X(5) value "46003".
       77  SQL-STATE-46007                     PIC X(5) value "46007".
       77  SQL-STATE-46008                     PIC X(5) value "46008".
       77  SQL-STATE-46103                     PIC X(5) value "46103".
       77  SQL-STATE-46501                     PIC X(5) value "46501".
       77  SQL-STATE-CALL-INV                  PIC X(5) value "51001".
       77  SQL-STATE-PACKAGE-NOTFOUND          PIC X(5) value "51002".
       77  SQL-STATE-TOKENS-NOTMATCH           PIC X(5) value "51003".
       77  SQL-STATE-ADDR-INV-DA               PIC X(5) value "51004".
       77  SQL-STATE-FUNC-DISABLED             PIC X(5) value "51005".
       77  SQL-STATE-STMT-NOTEXEC              PIC X(5) value "51007".
       77  SQL-STATE-RELEASE-INV               PIC X(5) value "51008".
       77  SQL-STATE-PROC-NOTCONNECTABLE       PIC X(5) value "51011".
       77  SQL-STATE-SECTION-BINDERR           PIC X(5) value "51015".
       77  SQL-STATE-USER-NOTLOGGEDON          PIC X(5) value "51017".
       77  SQL-STATE-APP-UNCONNECTED           PIC X(5) value "51018".
      * Planned
       77  SQL-STATE-CRTPACK-BUSY              PIC X(5) value "51019".
      * Planned
       77  SQL-STATE-PACK-NONE                 PIC X(5) value "51020".
       77  SQL-ROLLBACK-REQUIRED               PIC X(5) value "51021".
       77  SQL-CONN-WITH-AUTH-ALREADY          PIC X(5) value "51022".
       77  SQL-STATE-DB-INUSE-ANOTH-INST       PIC X(5) value "51023".
       77  SQL-INOPERAITVE-VIEW                PIC X(5) value "51024".
       77  SQL-CONN-SETTINGS-IN-USE            PIC X(5) value "51025".
       77  SQL-EVMON-PATH-INUSE                PIC X(5) value "51026".
       77  SQL-IMMEDIATE-CHECKED-OPT-INV       PIC X(5) value "51027".
       77  SQL-PACKAGE-INOPERATIVE             PIC X(5) value "51028".
       77  SQL-STATE-51030                     PIC X(5) value "51030".
       77  SQL-STATE-51034                     PIC X(5) value "51034".
       77  SQL-STATE-51035                     PIC X(5) value "51035".
       77  SQL-STATE-51038                     PIC X(5) value "51038".
       77  SQL-STATE-51039                     PIC X(5) value "51039".
       77  SQL-STATE-51040                     PIC X(5) value "51040".
       77  SQL-NAME-COL-DUP-STMT               PIC X(5) value "52001".
       77  SQL-NAME-COLREF-DUP-NAME            PIC X(5) value "52002".
       77  SQL-NAME-COL-UNDEF                  PIC X(5) value "52003".
       77  SQL-NAME-OBJ-UNDEF                  PIC X(5) value "52004".
       77  SQL-NAME-SERVER-UNDEF               PIC X(5) value "52005".
       77  SQL-NAME-COL-INV                    PIC X(5) value "52006".
       77  SQL-NAME-COL-NOTRESULT              PIC X(5) value "52007".
       77  SQL-NAME-CURSOR-UNDEF               PIC X(5) value "52008".
       77  SQL-NAME-COL-DUP-KEYCLAUSE          PIC X(5) value "52009".
       77  SQL-NAME-OBJ-DUP                    PIC X(5) value "52010".
       77  SQL-NAME-COL-DUP-OBJDEF             PIC X(5) value "52011".
       77  SQL-NAME-TABLE-DUP                  PIC X(5) value "52012".
      * Planned
       77  SQL-NAME-CURSOR-DUP                 PIC X(5) value "52013".
      * Planned
       77  SQL-NAME-CURSOR-NOTQUAL             PIC X(5) value "52015".
      * Planned
       77  SQL-NAME-COLREF-DUP-SET             PIC X(5) value "52016".
       77  SQL-NAME-AS-NONAME                  PIC X(5) value "52017".
      * Planned
       77  SQL-NAME-CURSOR-UNDECL              PIC X(5) value "52019".
       77  SQL-NAME-NODE-NOTFOUND              PIC X(5) value "52020".
      * Planned
       77  SQL-NAME-REG-UNKNOWN                PIC X(5) value "52021".
       77  SQL-STATE-5U001                     PIC X(5) value "5U001".
       77  SQL-STATE-5U002                     PIC X(5) value "5U002".
       77  SQL-STATE-5U003                     PIC X(5) value "5U003".
       77  SQL-STATE-5U004                     PIC X(5) value "5U004".
       77  SQL-STATE-5U005                     PIC X(5) value "5U005".
       77  SQL-STATE-5U006                     PIC X(5) value "5U006".
       77  SQL-STATE-5U0ZZ                     PIC X(5) value "5U0ZZ".
       77  SQL-STATE-ZZZZZ                     PIC X(5) value "ZZZZZ".
       77  SQL-OSPEC-NUMVAL-NOTSAME            PIC X(5) value "53002".
       77  SQL-OSPEC-COLUMN-INV                PIC X(5) value "53003".
       77  SQL-OSPEC-CLAUSE-NOTCOLUMN          PIC X(5) value "53005".
       77  SQL-OSPEC-OP-NOTPERMIT              PIC X(5) value "53007".
       77  SQL-OSPEC-COL-NOTUPDATABLE          PIC X(5) value "53008".
       77  SQL-OSPEC-OBJ-WRONGTYPE             PIC X(5) value "53009".
       77  SQL-OSPEC-CLAUSE-ISVIEW             PIC X(5) value "53010".
       77  SQL-OSPEC-NUMCOL-NOTSAME            PIC X(5) value "53011".
       77  SQL-OSPEC-OPTION-BAD                PIC X(5) value "53013".
       77  SQL-OSPEC-DATA-INV                  PIC X(5) value "53015".
       77  SQL-OSPEC-DATETIME-INV-EXPR         PIC X(5) value "53016".
       77  SQL-OSPEC-NUMVAR-NOTSAME            PIC X(5) value "53017".
       77  SQL-OSPEC-OPERANDS-NOTCOMPAT        PIC X(5) value "53018".
       77  SQL-OSPEC-OP-NOTNUMB                PIC X(5) value "53019".
       77  SQL-OSPEC-CONS-TOOLONG              PIC X(5) value "53020".
       77  SQL-OSPEC-VAL-NOTCOMPAT-COL         PIC X(5) value "53021".
       77  SQL-OSPEC-CLAUSE-TOOMANY            PIC X(5) value "53023".
       77  SQL-OSPEC-LIKEOP-BAD                PIC X(5) value "53024".
       77  SQL-OSPEC-UNIONOP-INCOMPAT-COL      PIC X(5) value "53025".
       77  SQL-OSPEC-UNIONOP-BADNUM-COL        PIC X(5) value "53026".
       77  SQL-OSPEC-TABLE-NOTSAME-CURSOR      PIC X(5) value "53027".
       77  SQL-OSPEC-TABLE-CANTMODIFY          PIC X(5) value "53028".
       77  SQL-OSPEC-UPDATE-INV-ROTABLE        PIC X(5) value "53029".
       77  SQL-OSPEC-FKEY-NOTCONFORM           PIC X(5) value "53030".
       77  SQL-OSPEC-NULL-NOTALLOW             PIC X(5) value "53031".
       77  SQL-OSPEC-OP-NOTALLOW-SYSOBJ        PIC X(5) value "53032".
       77  SQL-OSPEC-FKEY-NOTALLOW-NULL        PIC X(5) value "53034".
       77  SQL-STATE-53040                     PIC X(5) value "53040".
       77  SQL-OSPEC-COL-INV                   PIC X(5) value "53042".
       77  SQL-OSPEC-PRIV-INV                  PIC X(5) value "53052".
      * Planned
       77  SQL-OSPEC-OPT-BOTH                  PIC X(5) value "53053".
       77  SQL-OSPEC-AUTH-BAD                  PIC X(5) value "53056".
      * Planned
       77  SQL-OSPEC-STMT-CANTUSE              PIC X(5) value "53059".
       77  SQL-OSPEC-DB-CANTUSE                PIC X(5) value "53061".
      * Planned
       77  SQL-OSPEC-STMT-CANTEXEC             PIC X(5) value "53062".
       77  SQL-OSPEC-HOSTVAR-UNDEF             PIC X(5) value "53063".
      * Planned
       77  SQL-OSPEC-INFO-NOTMATCH             PIC X(5) value "53064".
      * Planned
       77  SQL-OSPEC-OPTIONS-CONFLICT          PIC X(5) value "53067".
      * Planned
       77  SQL-OSPEC-DESCRIPTORS-TOOFEW        PIC X(5) value "53070".
      * Planned
       77  SQL-OSPEC-NUMSTRUCTS-NOTMATCH       PIC X(5) value "53071".
      * Planned
       77  SQL-OSPEC-CLAUSE-INCOMPAT           PIC X(5) value "53072".
      * Planned
       77  SQL-OSPEC-NUMROWS-INV               PIC X(5) value "53073".
       77  SQL-STATE-53090                     PIC X(5) value "53090".
       77  SQL-STATE-53091                     PIC X(5) value "53091".
       77  SQL-LIMIT-STMT-TOOLONG              PIC X(5) value "54001".
       77  SQL-LIMIT-SCONST-TOOLONG            PIC X(5) value "54002".
       77  SQL-LIMIT-NAME-TOOLONG              PIC X(5) value "54003".
       77  SQL-LIMIT-NAMES-TOOMANY             PIC X(5) value "54004".
       77  SQL-LIMIT-CONCAT-TOOLONG            PIC X(5) value "54006".
      * Planned
       77  SQL-LIMIT-AS-TOOBIG                 PIC X(5) value "54007".
       77  SQL-LIMIT-KEY-TOOLONG               PIC X(5) value "54008".
       77  SQL-LIMIT-TABLE-TOOLONG             PIC X(5) value "54010".
       77  SQL-LIMIT-COLS-TOOMANY              PIC X(5) value "54011".
       77  SQL-LIMIT-DESCRIPTORS-TOOMANY       PIC X(5) value "54019".
       77  SQL-LIMIT-ARGUMENTS-TOOMANY         PIC X(5) value "54023".
       77  SQL-LIMIT-CONCUR-LOB-HANDLES        PIC X(5) value "54028".
       77  SQL-LIMIT-OPEN-DIR-SCANS            PIC X(5) value "54029".
       77  SQL-LIMIT-EVMON-TOOMANY             PIC X(5) value "54030".
       77  SQL-LIMIT-EVTFILES-TOOMANY          PIC X(5) value "54031".
       77  SQL-LIMIT-MAX-TABLE-SIZE            PIC X(5) value "54032".
       77  SQL-LIMIT-PMAP-TOOMANY              PIC X(5) value "54033".
       77  SQL-LIMIT-CONTNR-NAMES-LEN          PIC X(5) value "54034".
       77  SQL-LIMIT-TBLSPACE-SIZE             PIC X(5) value "54035".
       77  SQL-LIMIT-CONTNR-PATH-LEN           PIC X(5) value "54036".
       77  SQL-LIMIT-CONTAIN-MAP-COMPLX        PIC X(5) value "54037".
       77  SQL-LIMIT-CASCADED-TRIGGERS         PIC X(5) value "54038".
       77  SQL-LIMIT-CONTAINER-SIZE            PIC X(5) value "54039".
       77  SQL-LIMIT-TRANSITION-TBL            PIC X(5) value "54040".
       77  SQL-LIMIT-TYPE-LEVEL                PIC X(5) value "54045".
       77  SQL-LIMIT-MAX-PARM-INDEX            PIC X(5) value "54046".
       77  SQL-LIMIT-MAX-TSPACE-SIZE           PIC X(5) value "54047".
       77  SQL-NO-TEMP-TSPACE-PAGE-SIZE        PIC X(5) value "54048".
       77  SQL-STATE-54049                     PIC X(5) value "54049".
       77  SQL-STATE-54050                     PIC X(5) value "54050".
       77  SQL-STATE-54052                     PIC X(5) value "54052".
       77  SQL-STATE-54053                     PIC X(5) value "54053".
       77  SQL-STATE-DB-NOTMIGR                PIC X(5) value "55001".
       77  SQL-STATE-EXPL-TBL-NOT-DEFINED      PIC X(5) value "55002".
       77  SQL-STATE-OBJ-INUSE                 PIC X(5) value "55006".
       77  SQL-STATE-OBJ-NOTALTER              PIC X(5) value "55007".
       77  SQL-STATE-TABLE-NOKEY               PIC X(5) value "55008".
       77  SQL-STATE-FILE-READONLY             PIC X(5) value "55009".
      * Planned
       77  SQL-STATE-OBJ-HASDEPS               PIC X(5) value "55010".
       77  SQL-STATE-TABLE-HASCLUSTIDX         PIC X(5) value "55012".
       77  SQL-STATE-TABLE-HASKEY              PIC X(5) value "55013".
       77  SQL-STATE-TABLE-INVALID             PIC X(5) value "55019".
       77  SQL-SERVER-NOT-REGISTERED           PIC X(5) value "55022".
       77  SQL-STATE-55023                     PIC X(5) value "55023".
       77  SQL-CHK-CONSTRAINT-TOO-LONG         PIC X(5) value "55024".
       77  SQL-STATE-DB-NEEDRESTART            PIC X(5) value "55025".
       77  SQL-TRIGGER-DEF-TOO-LONG            PIC X(5) value "55026".
      * Planned
       77  SQL-STATE-OS2-RESERVED              PIC X(5) value "55031".
      * Planned
       77  SQL-CONN-INV-DBM-STOPPED            PIC X(5) value "55032".
       77  SQL-MUST-COMMIT-FOR-EVMON           PIC X(5) value "55033".
       77  SQL-EVMON-ACTIVE-CANT-DROP          PIC X(5) value "55034".
       77  SQL-STATE-55035                     PIC X(5) value "55035".
       77  SQL-STATE-DROP-PART-KEY             PIC X(5) value "55037".
       77  SQL-STATE-REBALANCING               PIC X(5) value "55038".
       77  SQL-INV-STATE-CHANGE                PIC X(5) value "55039".
       77  SQL-STATE-55040                     PIC X(5) value "55040".
       77  SQL-REBALANCE-IN-PROGRESS           PIC X(5) value "55041".
       77  SQL-TYPED-TABLE-EXISTS              PIC X(5) value "55043".
       77  SQL-STATE-55045                     PIC X(5) value "55045".
       77  SQL-STATE-55046                     PIC X(5) value "55046".
       77  SQL-STATE-55047                     PIC X(5) value "55047".
       77  SQL-STATE-55048                     PIC X(5) value "55048".
       77  SQL-STATE-55049                     PIC X(5) value "55049".
       77  SQL-STATE-55051                     PIC X(5) value "55051".
       77  SQL-STATE-55054                     PIC X(5) value "55054".
       77  SQL-STATE-55056                     PIC X(5) value "55056".
       77  SQL-NO-AUTOMATIC-STORAGE            PIC X(5) value "55060".
       77  SQL-NO-REDIRECTED-RESTORE           PIC X(5) value "55061".
       77  SQL-STORAGE-PATHS-NOT-ALLOWED       PIC X(5) value "55062".
       77  SQL-MISC-NONAME                     PIC X(5) value "56001".
       77  SQL-MISC-FROM-INV                   PIC X(5) value "56002".
       77  SQL-MISC-WHERE-INV                  PIC X(5) value "56003".
       77  SQL-MISC-DISTINCT-TOOMANY           PIC X(5) value "56005".
       77  SQL-MISC-STR-TOOLONG                PIC X(5) value "56007".
       77  SQL-MISC-LIST-MISSING               PIC X(5) value "56008".
       77  SQL-MISC-DIVIDE-NEG                 PIC X(5) value "56011".
       77  SQL-MISC-COL-NOUPDATE               PIC X(5) value "56012".
       77  SQL-MISC-UPDDEL-INV                 PIC X(5) value "56013".
       77  SQL-MISC-DEL-INV                    PIC X(5) value "56014".
       77  SQL-MISC-CONSTR-INV                 PIC X(5) value "56015".
       77  SQL-MISC-OBJECT-NODROP              PIC X(5) value "56017".
       77  SQL-MISC-INV-COMM-ROLL              PIC X(5) value "56021".
      * Planned
       77  SQL-MISC-REF-INV                    PIC X(5) value "56023".
      * Planned
       77  SQL-MISC-ALIAS-NOTTABLE             PIC X(5) value "56024".
       77  SQL-MISC-OP-INV                     PIC X(5) value "56026".
       77  SQL-MISC-MIXED-DBCS-INV             PIC X(5) value "56031".
       77  SQL-MISC-PREP-INV                   PIC X(5) value "56032".
       77  SQL-MISC-STRING-HOSTVAR             PIC X(5) value "56033".
       77  SQL-STATE-56038                     PIC X(5) value "56038".
       77  SQL-MISC-OBJ-NOTCRT                 PIC X(5) value "56039".
      * Planned
       77  SQL-MISC-PACKAGE-NOTCOMMIT          PIC X(5) value "56043".
      * Planned
       77  SQL-MISC-NAME-NOTID                 PIC X(5) value "56052".
      * Planned
       77  SQL-MISC-DATA-NOTSUPP               PIC X(5) value "56059".
      * Planned
       77  SQL-MISC-STMT-NOTEXEC               PIC X(5) value "56060".
       77  SQL-MISC-NAME-NOTMATCH              PIC X(5) value "56061".
       77  SQL-MISC-ISO-NOTUSER                PIC X(5) value "56068".
      * Planned
       77  SQL-MISC-JOIN-BADCOLS               PIC X(5) value "56072".
      * Planned
       77  SQL-MISC-NAME-REQUIRED              PIC X(5) value "56073".
      * Planned
       77  SQL-MISC-NAME-NOTQUAL               PIC X(5) value "56074".
       77  SQL-LOB-NOSUPP-IN-DRDA              PIC X(5) value "56084".
       77  SQL-MISC-N-A-COMPOUND-SQL-ERR       PIC X(5) value "56091".
       77  SQL-MISC-NAME-BOTH-USER-GROUP       PIC X(5) value "56092".
       77  SQL-MISC-NO-BLOB-IN-DWNLVL-CL       PIC X(5) value "56093".
       77  SQL-STATE-56095                     PIC X(5) value "56095".
       77  SQL-MISC-NOT-ALIGNED                PIC X(5) value "56097".
       77  SQL-MISC-PREP-REBIND-INV            PIC X(5) value "56098".
       77  SQL-MISC-REAL-NOT-SUPPORTED         PIC X(5) value "56099".
       77  SQL-MISC-LOB-ACTION-FAILED          PIC X(5) value "560A0".
       77  SQL-FUNC-CLAUSE-INVALID-UCS2        PIC X(5) value "560AA".
       77  SQL-STATE-560AC                     PIC X(5) value "560AC".
       77  SQL-STATE-560AF                     PIC X(5) value "560AF".
       77  SQL-STATE-560B0                     PIC X(5) value "560B0".
       77  SQL-STATE-560B1                     PIC X(5) value "560B1".
       77  SQL-STATE-560B7                     PIC X(5) value "560B7".
       77  SQL-STATE-560BB                     PIC X(5) value "560BB".
       77  SQL-STATE-560BC                     PIC X(5) value "560BC".
       77  SQL-STATE-560BD                     PIC X(5) value "560BD".
       77  SQL-STATE-560BF                     PIC X(5) value "560BF".
       77  SQL-STATE-560C0                     PIC X(5) value "560C0".
       77  SQL-STATE-560C1                     PIC X(5) value "560C1".
       77  SQL-STATE-560C2                     PIC X(5) value "560C2".
       77  SQL-STATE-560C3                     PIC X(5) value "560C3".
       77  SQL-STATE-560C6                     PIC X(5) value "560C6".
       77  SQL-STATE-560C8                     PIC X(5) value "560C8".
       77  SQL-STATE-560C9                     PIC X(5) value "560C9".
       77  SQL-STATE-560CA                     PIC X(5) value "560CA".
       77  SQL-STATE-57011                     PIC X(5) value "57011".
       77  SQL-STATE-57053                     PIC X(5) value "57053".
       77  SQL-STATE-57059                     PIC X(5) value "57059".
       77  SQL-RSRC-TABLE-UNAVAILABLE          PIC X(5) value "57001".
       77  SQL-RSRC-BUFFPOOL-INACTIVE          PIC X(5) value "57003".
       77  SQL-RSRC-PENDING                    PIC X(5) value "57007".
       77  SQL-RSRC-VSDB-TUNAVAIL              PIC X(5) value "57009".
       77  SQL-RSRC-VSDB-UNAVAIL               PIC X(5) value "57011".
       77  SQL-RSRC-NONDB-UNAVAIL-CONT         PIC X(5) value "57012".
       77  SQL-RSRC-NONDB-UNAVAIL-STOP         PIC X(5) value "57013".
       77  SQL-RSRC-CANCEL                     PIC X(5) value "57014".
       77  SQL-RSRC-TABLE-INACTIVE             PIC X(5) value "57016".
       77  SQL-RSRC-CXLATE-NOTDEF              PIC X(5) value "57017".
       77  SQL-RSRC-DRIVE-NOTEXIST             PIC X(5) value "57019".
       77  SQL-RSRC-DRIVE-LOCKED               PIC X(5) value "57020".
       77  SQL-RSRC-DRIVE-OPEN                 PIC X(5) value "57021".
       77  SQL-RSRC-NO-DBSPACES                PIC X(5) value "57022".
       77  SQL-RSRC-MAXAPPS                    PIC X(5) value "57030".
       77  SQL-RSRC-MAXDB                      PIC X(5) value "57032".
       77  SQL-RSRC-DB-STARTED                 PIC X(5) value "57033".
       77  SQL-RSRC-DB-ACQUIRED                PIC X(5) value "57034".
       77  SQL-RSRC-DBFILE-NOTOWN              PIC X(5) value "57035".
       77  SQL-RSRC-LOG-NOTOWN                 PIC X(5) value "57036".
       77  SQL-RSRC-STANDALONE                 PIC X(5) value "57041".
       77  SQL-RSRC-QUIESCE-BACKUP             PIC X(5) value "57046".
       77  SQL-RSRC-DIR-NOT-ACCESS             PIC X(5) value "57047".
       77  SQL-RSRC-CONTNR-ACC-ERR             PIC X(5) value "57048".
       77  SQL-RSRC-SYS-LIMIT                  PIC X(5) value "57049".
       77  SQL-RSRC-PATH                       PIC X(5) value "57050".
       77  SQL-RSRC-CPU-COST                   PIC X(5) value "57051".
       77  SQL-RSRC-UNAVAIL-NODE               PIC X(5) value "57052".
       77  SQL-RSRC-TABSPACE-SIZE              PIC X(5) value "57055".
       77  SQL-RSRC-NO-PACKAGE-LOCK            PIC X(5) value "57056".
       77  SQL-RSRC-DRDA-CHAIN-COND            PIC X(5) value "57057".
       77  SQL-RSRC-DISK-STUFFIT               PIC X(5) value "57901".
       77  SQL-RSRC-DISK-BAD                   PIC X(5) value "57902".
       77  SQL-RSRC-DISK-FULL                  PIC X(5) value "57903".
       77  SQL-SYSERR-INV-SECT                 PIC X(5) value "58003".
       77  SQL-SYSERR-ERR-CONT                 PIC X(5) value "58004".
       77  SQL-SYSERR-ERR-STOP                 PIC X(5) value "58005".
       77  SQL-SYSERR-DIST-CONT                PIC X(5) value "58008".
       77  SQL-SYSERR-DIST-DEALLOC             PIC X(5) value "58009".
       77  SQL-SYSERR-DIST-STOP                PIC X(5) value "58010".
       77  SQL-SYSERR-BIND-ACTIVE              PIC X(5) value "58011".
       77  SQL-SYSERR-BIND-INACTIVE            PIC X(5) value "58012".
       77  SQL-SYSERR-CMD-NOTSUPP              PIC X(5) value "58014".
       77  SQL-SYSERR-OBJ-NOTSUPP              PIC X(5) value "58015".
       77  SQL-SYSERR-PARAM-NOTSUPP            PIC X(5) value "58016".
       77  SQL-SYSERR-VALUE-NOTSUPP            PIC X(5) value "58017".
       77  SQL-SYSERR-MSG-NOTSUPP              PIC X(5) value "58018".
       77  SQL-SYSERR-COMMERR                  PIC X(5) value "58019".
       77  SQL-SYSERR-NOTDEF                   PIC X(5) value "58020".
       77  SQL-SYSERR-PROG-CANCELED            PIC X(5) value "58023".
      * Planned
       77  SQL-SYSERR-HOSTVARS                 PIC X(5) value "58026".
       77  SQL-SYSERR-IOERR                    PIC X(5) value "58030".
       77  SQL-SYSERR-DBFILE                   PIC X(5) value "58031".
       77  SQL-SYSERR-STR-FENCED-FUN-FAIL      PIC X(5) value "58032".
       77  SQL-SYSERR-FIND-TBSP-PG-FAIL        PIC X(5) value "58034".
       77  SQL-SYSERR-FREE-TBSP-PG-FAIL        PIC X(5) value "58035".
       77  SQL-SYSERR-TBLSPC-ID-NOEXIST        PIC X(5) value "58036".

