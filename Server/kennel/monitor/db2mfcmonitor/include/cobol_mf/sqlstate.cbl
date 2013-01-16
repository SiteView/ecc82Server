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
       78  SQLSTATE-CLASS-SUCCESS              value     "00".
       78  SQLSTATE-CLASS-WARNING              value     "01".
       78  SQLSTATE-CLASS-NO-DATA              value     "02".
       78  SQLSTATE-CLASS-CONNECT-EXCEPT       value     "08".
       78  SQLSTATE-CLASS-TRIGGER-EXCEPT       value     "09".
       78  SQLSTATE-CLASS-FEATURE-EXCEPT       value     "0A".
       78  SQLSTATE-CLASS-TYPE-EXCEPT          value     "0D".
       78  SQLSTATE-CLASS-SCHEMA-EXCEPT        value     "0E".
       78  SQLSTATE-CLASS-INVALID-TOKEN        value     "0F".
       78  SQLSTATE-CLASS-TRIG-PROHIBIT        value     "0W".
       78  SQLSTATE-CLASS-CASE-EXCEPT          value     "20".
       78  SQLSTATE-CLASS-CARDINALITY          value     "21".
       78  SQLSTATE-CLASS-DATA-EXCEPT          value     "22".
       78  SQLSTATE-CLASS-CONSTRAINT           value     "23".
       78  SQLSTATE-CLASS-INV-CURS-STATE       value     "24".
       78  SQLSTATE-CLASS-INV-STMT-ID          value     "26".
       78  SQLSTATE-CLASS-SYNTAX               value     "37".
       78  SQLSTATE-CLASS-SERIALIZATION        value     "40".
       78  SQLSTATE-CLASS-AUTHORZATION         value     "42".
       78  SQLSTATE-CLASS-INV-APP-STATE        value     "51".
       78  SQLSTATE-CLASS-DUP-UNDEF-NAME       value     "52".
       78  SQLSTATE-CLASS-INV-OPERAND          value     "53".
       78  SQLSTATE-CLASS-LIMIT                value     "54".
       78  SQLSTATE-CLASS-PREREQ-STATE         value     "55".
       78  SQLSTATE-CLASS-MISC-RESTRICT        value     "56".
       78  SQLSTATE-CLASS-RESOURCE             value     "57".
       78  SQLSTATE-CLASS-SYSTEM-ERROR         value     "58".
      * SQLSTATES
       78  SQL-EXEC-OK                         value     "00000".
       78  SQL-WARN-DISCONNECT-ERR             value     "01002".
       78  SQL-WARN-NULL-VAL-ELIMINATED        value     "01003".
       78  SQL-WARN-VAL-TRUNC                  value     "01004".
       78  SQL-WARN-INSUFF-ITEM-DESCRIPT       value     "01005".
       78  SQL-WARN-NOT-ALL-PRIVS-GRANTED      value     "01007".
       78  SQL-STATE-0100C                     value     "0100C".
       78  SQL-STATE-0100D                     value     "0100D".
       78  SQL-STATE-0100E                     value     "0100E".
       78  SQL-WARN-STR-TRUNC                  value     "01501".
       78  SQL-WARN-NULLS-ELIM                 value     "01502".
       78  SQL-WARN-COLS-TOOMANY               value     "01503".
       78  SQL-WARN-STMT-NOWHERE               value     "01504".
       78  SQL-WARN-STMT-NOTEXEC               value     "01505".
       78  SQL-WARN-DATE-ADJ                   value     "01506".
       78  SQL-WARN-CURSOR-NOT-BLOCKED         value     "01509".
       78  SQL-WARN-HOST-NULL-COL-RANGE        value     "01515".
       78  SQL-WARN-WITH-GRANT-IGNORED         value     "01516".
       78  SQL-WARN-CHAR-SUB                   value     "01517".
       78  SQL-WARN-HOST-NULL-NUM-RANGE        value     "01519".
       78  SQL-WARN-VAR-NOWNULL                value     "01520".
      * Planned
       78  SQL-WARN-TABLE-UNDEF                value     "01522".
       78  SQL-WARN-NULLS2-ELIM                value     "01524".
       78  SQL-WARN-NUM-INSERTS-INVALID        value     "01525".
       78  SQL-WARN-ISO-ESCAL                  value     "01526".
      * Planned
       78  SQL-WARN-REG-NOTEXIST               value     "01527".
      * Planned
       78  SQL-WARN-WHERE-IGNORE               value     "01528".
       78  SQL-WARN-NAME-UNDEF                 value     "01532".
       78  SQL-WARN-UNDEFINED-COLUMN-NAME      value     "01533".
       78  SQL-WARN-USE-SBCS-CHARS-ONLY        value     "01539".
       78  SQL-WARN-OP-NOPRIV                  value     "01542".
       78  SQL-WARN-REF-IGNORE                 value     "01543".
       78  SQL-WARN-NAME-INTERP                value     "01545".
       78  SQL-WARN-OBJ-NOPRIV                 value     "01548".
      * Planned
       78  SQL-WARN-PACK-PRIV                  value     "01549".
       78  SQL-WARN-INDEX-EXIST                value     "01550".
       78  SQL-WARN-GRANT-IGNORED              value     "01560".
       78  SQL-WARN-NEWLOGPATH-INV             value     "01562".
       78  SQL-WARN-CURR-LOGPATH-INV           value     "01563".
       78  SQL-WARN-HOST-NULL-DIV-ZERO         value     "01564".
       78  SQL-WARN-VAL-INCMPAT-WITH-COL       value     "01580".
       78  SQL-WARN-AUTO-CHECK-PEND-STATE      value     "01586".
       78  SQL-WARN-KEYWORD-REDUNDANT          value     "01589".
       78  SQL-WARN-VAL-MAY-BE-TRUNC           value     "01592".
       78  SQL-WARN-VAL-COULD-BE-TRUNC         value     "01593".
       78  SQL-WARN-INSUFF-DESCRIPTORS         value     "01594".
       78  SQL-WARN-OLD-VIEW-REPLACED          value     "01595".
       78  SQL-WARN-NO-COMPFUNC-FOR-LSTR       value     "01596".
       78  SQL-WARN-EVMON-STATE-NOT-SET        value     "01598".
       78  SQL-WARN-REBIND-OPTS-IGNORED        value     "01599".
       78  SQL-WARN-CONN-DISABLED              value     "01601".
       78  SQL-WARN-SUB-OPTIMAL-QUERY          value     "01602".
       78  SQL-WARN-CONSTRAINT-VIO-MOVED       value     "01603".
       78  SQL-WARN-EXPLAIN-MODE-RESTRICT      value     "01604".
       78  SQL-WARN-POSSIBLE-INFINIT-LOOP      value     "01605".
       78  SQL-WARN-DIR-EMPTY                  value     "01606".
       78  SQL-WARN-TIMEDIFF-EXCEEDED          value     "01607".
       78  SQL-WARN-CLAUSE-VAL-REPLACED        value     "01608".
       78  SQL-WARN-MAX-QUERY-RESULT-SET       value     "01609".
       78  SQL-WARN-MORE-RESULT-SETS           value     "01610".
       78  SQL-WARN-NEXT-RESULT-SET            value     "01611".
       78  SQL-STATE-01614                     value     "01614".
       78  SQL-WARN-COST-EXCEED-LIMIT          value     "01616".
       78  SQL-WARN-REDIST-REQUIRED            value     "01618".
       78  SQL-WARN-UNIONALL-TABLES-SAME       value     "01620".
       78  SQL-WARN-LOB-CHANGED                value     "01621".
       78  SQL-WARN-SYSTEM-ERROR               value     "01622".
       78  SQL-WARN-DEGREE-IGNORED             value     "01623".
       78  SQL-STATE-01625                     value     "01625".
       78  SQL-WARN-ONE-ACTIVE-BUFFPOOL        value     "01626".
       78  SQL-WARN-DATALINK-VAL-INVAL         value     "01627".
       78  SQL-WARN-RESOURCE-UNAVAIL           value     "01630".
       78  SQL-NUM-CONN-EXCEEDED               value     "01632".
       78  SQL-SUMM-TABLE-NO-OPTIMIZE          value     "01633".
       78  SQL-INTEGRITY-UNVERIFIED            value     "01636".
       78  SQL-DEBUGGER-NOT-ENABLED            value     "01637".
       78  SQL-STATE-01639                     value     "01639".
       78  SQL-STATE-01641                     value     "01641".
       78  SQL-STATE-01642                     value     "01642".
       78  SQL-STATE-01643                     value     "01643".
       78  SQL-STATE-01645                     value     "01645".
       78  SQL-STATE-01648                     value     "01648".
       78  SQL-STATE-01649                     value     "01649".
       78  SQL-STATE-01650                     value     "01650".
       78  SQL-STATE-01651                     value     "01651".
       78  SQL-STATE-01652                     value     "01652".
       78  SQL-STATE-01653                     value     "01653".
       78  SQL-STATE-01654                     value     "01654".
       78  SQL-STATE-01655                     value     "01655".
       78  SQL-STATE-01657                     value     "01657".
       78  SQL-WARN-INDEX-UNIQUE               value     "01661".
       78  SQL-STATE-01665                     value     "01665".
       78  SQL-STATE-01667                     value     "01667".
       78  SQL-STATE-01669                     value     "01669".
       78  SQL-STATE-01670                     value     "01670".
       78  SQL-STATE-01671                     value     "01671".
       78  SQL-WARN-INVALID-CURSOR-POS         value     "01S06".
       78  SQL-WARN-UDF-RETURNS-STATE          value     "01Hxx".
       78  SQL-NODATA-EXCEPTION                value     "02000".
       78  SQL-NODATA-UHOLE                    value     "02501".
       78  SQL-NODATA-DHOLE                    value     "02502".
       78  SQL-INV-NUM-HOST-VARS               value     "07001".
       78  SQL-INV-PARM-OR-CB                  value     "07002".
       78  SQL-INV-EXECUTE-STMT                value     "07003".
       78  SQL-USING-CLAUSE-REQUIRED           value     "07004".
       78  SQL-INV-PREP-STMT-CURSOR            value     "07005".
       78  SQL-HOSTVAR-NOT-USABLE              value     "07006".
       78  SQL-STATE-07007                     value     "07007".
       78  SQL-CONN-FAILED                     value     "08001".
       78  SQL-CONN-ALREADY-EXIST              value     "08002".
       78  SQL-CONN-DOES-NOT-EXIT              value     "08003".
       78  SQL-CONN-REJECTED-BY-SERVER         value     "08004".
       78  SQL-CONN-RESOLUTION-UNKNOWN         value     "08007".
       78  SQL-CONN-FAILED-NO-TM               value     "08502".
       78  SQL-STATE-08504                     value     "08504".
       78  SQL-STATE-08506                     value     "08506".
       78  SQL-TRIGGER-ERROR                   value     "09000".
       78  SQL-PROC-NOT-CONNECTABLE            value     "0A001".
       78  SQL-INSTANCE-NOT-ENABLED            value     "0A502".
       78  SQL-STATE-0A503                     value     "0A503".
       78  SQL-STATE-0D000                     value     "0D000".
       78  SQL-LOB-NOVALUE-TOKEN-VAR           value     "0F001".
       78  SQL-STATE-0K000                     value     "0K000".
       78  SQL-STATE-0W000                     value     "0W000".
       78  SQL-STATE-20000                     value     "20000".
       78  SQL-CARD-VALUE-TOOMANY              value     "21000".
       78  SQL-CARD-INS-MULTIROW               value     "21501".
       78  SQL-CARD-UPD-MULTIROW               value     "21502".
       78  SQL-CARD-DEL-MULTIROW               value     "21504".
       78  SQL-DATA-ONE-ROW                    value     "21505".
       78  SQL-STATE-21506                     value     "21506".
       78  SQL-DATA-CHAR-RTRUNC                value     "22001".
       78  SQL-DATA-INDICATOR-NULL             value     "22002".
       78  SQL-DATA-NUM-RANGE                  value     "22003".
       78  SQL-STATE-22004                     value     "22004".
      * Planned
       78  SQL-DATA-FETCH-INV                  value     "22006".
       78  SQL-DATA-FORMAT-INV                 value     "22007".
       78  SQL-DATA-DATETIME-OV                value     "22008".
       78  SQL-STATE-2200G                     value     "2200G".
       78  SQL-DATA-STR-RANGE                  value     "22011".
       78  SQL-DATA-DIV-ZERO                   value     "22012".
       78  SQL-DATA-INV-CAST-CHAR              value     "22018".
      * Planned
       78  SQL-DATA-LIKE-INV-CHAR              value     "22019".
       78  SQL-DATA-NULL-MISSING               value     "22024".
       78  SQL-DATA-LIKE-INV-ESC               value     "22025".
       78  SQL-NULL-INSTANCE-MUT               value     "2202D".
       78  SQL-STATE-2202H                     value     "2202H".
       78  SQL-DATA-LENGTH-RANGE               value     "22501".
       78  SQL-DATA-STR-INV                    value     "22503".
       78  SQL-DATA-DATA-BADFORM               value     "22504".
       78  SQL-DATA-DATETIME-REG-INV           value     "22506".
       78  SQL-DATA-TYPE-CANTUSE               value     "22507".
       78  SQL-DATA-TYPE-INCOMPAT              value     "22509".
       78  SQL-HOSTVAR-NEG-INDVAR              value     "22512".
       78  SQL-DATA-INDEX-DUP                  value     "22515".
       78  SQL-DATA-STR-CANTUSE-XLATE          value     "22517".
       78  SQL-DATA-STR-CANTASGN-XLATE         value     "22518".
       78  SQL-DATA-KEY-NOTEQUAL               value     "22520".
       78  SQL-DATA-CCSIDVAL-INV               value     "22522".
       78  SQL-DATA-CONVERSION-TRUNC           value     "22524".
       78  SQL-STATE-22526                     value     "22526".
       78  SQL-STATE-22527                     value     "22527".
       78  SQL-CONSTR-CHG-KEY-NOTALLOW         value     "23001".
       78  SQL-CONSTR-ROW-NOTSATISFY-VIEW      value     "23501".
       78  SQL-CONSTR-NULL-NOTALLOW            value     "23502".
       78  SQL-CONSTR-VALUE-INV                value     "23503".
       78  SQL-CONSTR-OP-RESTRICT              value     "23504".
       78  SQL-CONSTR-INDEX-UNIQUE             value     "23505".
       78  SQL-CONSTR-VIOLATION-RLST           value     "23510".
       78  SQL-CONSTR-PAR-ROW-DEL-NOALLOW      value     "23511".
       78  SQL-CONSTR-ROW-INVALID              value     "23512".
       78  SQL-CONSTR-ROW-NONCONFORM           value     "23513".
       78  SQL-CONSTR-VIOLATIONS               value     "23514".
       78  SQL-CONSTR-DUP-KEY-VALUES           value     "23515".
       78  SQL-CONSTR-FOREIGN-KEY-NOALLOW      value     "23520".
       78  SQL-CONSTR-UPDATE-CAT-STATS         value     "23521".
       78  SQL-IDENTITY-EXHAUSTED              value     "23522".
       78  SQL-STATE-23524                     value     "23524".
       78  SQL-STATE-23527                     value     "23527".
       78  SQL-CURSOR-NOTOPEN                  value     "24501".
       78  SQL-CURSOR-OPEN-ALREADY             value     "24502".
       78  SQL-CURSOR-NOTPOS                   value     "24504".
       78  SQL-CURSOR-OPEN-NO-COMMIT           value     "24505".
       78  SQL-CURSOR-PD-OPEN                  value     "24506".
      * Planned
       78  SQL-CURSOR-DELETED                  value     "24507".
       78  SQL-CURSOR-DHOLE                    value     "24510".
      * Planned
       78  SQL-CURSOR-UHOLE                    value     "24511".
      * Planned
       78  SQL-CURSOR-AGREE                    value     "24512".
       78  SQL-CURSOR-MOVED                    value     "24513".
       78  SQL-CURSOR-PREVIOUS-ERROR           value     "24514".
       78  SQL-CURSOR-ALREADY-ASSIGNED         value     "24516".
       78  SQL-STATE-24517                     value     "24517".
      * Planned
       78  SQL-INV-EXECUTION-ENVIRONMENT       value     "25000".
       78  SQL-STATE-25001                     value     "25001".
      * Planned
       78  SQL-ONLY-AS-FIRST-STATEMENT         value     "25501".
       78  SQL-MULTIPLE-OPERATIONS             value     "25502".
       78  SQL-IDENT-NOTEXIST                  value     "26501".
       78  SQL-IDENT-NOASSOC                   value     "26503".
       78  SQL-IDENT-SELECT                    value     "26504".
      * Planned
       78  SQL-IDENT-NOTSPEC                   value     "26506".
       78  SQL-IDENT-NOTSRSEL                  value     "26507".
      * Planned
       78  SQL-IDENT-NOTSELECT                 value     "26508".
      * Planned
       78  SQL-IDENT-DUP                       value     "26509".
      * Planned
       78  SQL-IDENT-HASCURSOR                 value     "26510".
       78  SQL-STATE-27000                     value     "27000".
       78  SQL-INV-AUTHORIZATION-SPEC          value     "28000".
       78  SQL-INV-OPERATION-FOR-ENVIR         value     "2D521".
       78  SQL-STATE-2D522                     value     "2D522".
       78  SQL-DYNAMIC-COMMIT-INVALID          value     "2D528".
       78  SQL-DYNAMIC-ROLLBACK-INVALID        value     "2D529".
       78  SQL-INV-CONN-NAME                   value     "2E000".
       78  SQL-INV-CURSOR-NAME                 value     "34000".
       78  SQL-STATE-36001                     value     "36001".
       78  SQL-SYNTAX-TOKEN-MISSING            value     "37501".
       78  SQL-SYNTAX-NAME-INVCHAR             value     "37502".
       78  SQL-SYNTAX-STR-UNTERM               value     "37503".
       78  SQL-SYNTAX-CONST-INV                value     "37504".
       78  SQL-SYNTAX-NUMARGS-INV              value     "37505".
      * Planned
       78  SQL-SYNTAX-HCONST-INV               value     "37506".
       78  SQL-SYNTAX-OP-INV-COL               value     "37507".
       78  SQL-SYNTAX-OP-MARKER                value     "37509".
       78  SQL-SYNTAX-MARKER-INV               value     "37510".
       78  SQL-SYNTAX-COL-INV                  value     "37511".
       78  SQL-SYNTAX-STMT-BAD                 value     "37512".
      * Planned
       78  SQL-SYNTAX-CLAUSE-EXCLUSIVE         value     "37513".
       78  SQL-SYNTAX-KEYWORD-DUP              value     "37514".
      * Planned
       78  SQL-SYNTAX-OPT-INV                  value     "37516".
       78  SQL-SYNTAX-STMT-EMPTY               value     "37517".
       78  SQL-SYNTAX-HOSTVAR-NOTALLOW         value     "37518".
      * Planned
       78  SQL-SYNTAX-READONLY-NOTALLOW        value     "37520".
       78  SQL-UDF-RETURNS-ERROR-STATE         value     "38xxx".
       78  SQL-UDF-SQL-NOT-ALLOWED             value     "38001".
       78  SQL-UDF-MODIFIES-NOT-ALLOWED        value     "38002".
       78  SQL-UDF-STMT-NOT-ALLOWED            value     "38003".
       78  SQL-UDF-READS-NOT-ALLOWED           value     "38004".
       78  SQL-UDF-CALL-ERROR                  value     "38501".
       78  SQL-UDF-EXCUTE-SQL-NOTALLOW         value     "38502".
       78  SQL-UDF-ABNORMAL-END                value     "38503".
       78  SQL-UDF-INTERRUPTED                 value     "38504".
       78  SQL-STATE-38505                     value     "38505".
       78  SQL-STATE-38506                     value     "38506".
       78  SQL-STATE-38553                     value     "38553".
       78  SQL-UDF-RETURNS-INV-STATE           value     "39001".
       78  SQL-STATE-39004                     value     "39004".
       78  SQL-UDF-RETURNS-RESULT-TOOLONG      value     "39501".
       78  SQL-OUT-SQLDA-CHANGED               value     "39502".
       78  SQL-STATE-3B001                     value     "3B001".
       78  SQL-STATE-3B002                     value     "3B002".
       78  SQL-STATE-3B501                     value     "3B501".
       78  SQL-STATE-3B502                     value     "3B502".
       78  SQL-STATE-3B503                     value     "3B503".
       78  SQL-SERIAL-ROLLBACK                 value     "40000".
       78  SQL-ROLLBK-AUTOMATIC                value     "40001".
       78  SQL-STMT-COMPLETION-UNKNOWN         value     "40003".
       78  SQL-ROLLBK-DIST-ENV                 value     "40502".
       78  SQL-ROLLBK-CONN-LOST                value     "40504".
       78  SQL-ROLLBK-TABLE                    value     "40506".
       78  SQL-STATE-40507                     value     "40507".
       78  SQL-ACCESS-AUTH-NOTPRIV-OBJ         value     "42501".
       78  SQL-ACCESS-AUTH-NOTPRIV-OP          value     "42502".
       78  SQL-ACCESS-AUTH-NOT-AUTHORIZED      value     "42503".
       78  SQL-ACCESS-PRIV-CANTREVOKE          value     "42504".
       78  SQL-ACCESS-AUTH-FAIL-CONN           value     "42505".
       78  SQL-ACCESS-AUTH-FAIL-OWNER          value     "42506".
       78  SQL-ACCESS-AUTH-FAIL-SERVER         value     "42507".
       78  SQL-ACCESS-PRIV-CANTPUBLIC          value     "42508".
       78  SQL-STAT-NOT-AUTH-STATICRULES       value     "42509".
       78  SQL-STATE-42511                     value     "42511".
       78  SQL-SYNTAX-KEYWORD-INCOMPAT         value     "42601".
       78  SQL-SYNTAX-INVALID                  value     "42601".
       78  SQL-INVALID-CHAR-IN-NAME            value     "42602".
       78  SQL-UNTERMINATED-STRING-CONST       value     "42603".
       78  SQL-INVALID-NUM-OR-STR-CONST        value     "42604".
       78  SQL-INVALID-NUM-ARGS                value     "42605".
       78  SQL-INVALID-HEX-CONST               value     "42606".
       78  SQL-INVALID-COL-FUNC-OPERAND        value     "42607".
       78  SQL-INV-USE-OF-NULL-OR-DFLT         value     "42608".
       78  SQL-ALL-OPERANDS-PARM-MARKERS       value     "42609".
       78  SQL-PARAMETER-MARKERS-INVALID       value     "42610".
       78  SQL-SYNTAX-LEN-SCALE-INV            value     "42611".
       78  SQL-STMT-NOT-IN-CONTEXT             value     "42612".
       78  SQL-SYNTAX-KEYWORD-CONFLICT         value     "42613".
       78  SQL-DUP-KEYWORD-INV                 value     "42614".
       78  SQL-INV-ALTERNATIVE                 value     "42615".
       78  SQL-STATE-42616                     value     "42616".
       78  SQL-STMT-STR-BLANK-OR-EMPTY         value     "42617".
       78  SQL-HOST-VAR-NOT-ALLOWED            value     "42618".
       78  SQL-STATE-42620                     value     "42620".
       78  SQL-CHECK-CONSTRAINT-INVALID        value     "42621".
       78  SQL-NAME-OR-LABEL-IS-TOO-LONG       value     "42622".
       78  SQL-IDENTITY-NO-DEFAULT             value     "42623".
       78  SQL-CASE-RES-NOT-EXPRESSION         value     "42625".
       78  SQL-RETURNS-EXPR-AS                 value     "42627".
       78  SQL-STATE-42628                     value     "42628".
       78  SQL-STATE-42629                     value     "42629".
       78  SQL-STATE-42630                     value     "42630".
       78  SQL-STATE-42631                     value     "42631".
       78  SQL-STATE-42632                     value     "42632".
       78  SQL-STATE-42634                     value     "42634".
       78  SQL-STATE-42635                     value     "42635".
       78  SQL-DUP-COL-IN-INS-OR-UPD           value     "42701".
       78  SQL-COLUMN-NAME-AMBIGUOUS           value     "42702".
       78  SQL-UNDEFINED-COLUMN-NAME           value     "42703".
       78  SQL-NAME-OBJECT-UNDEF               value     "42704".
       78  SQL-NAME-SERVER-UNDEFINED           value     "42705".
       78  SQL-UNNAMED-COLS-IN-RESULT-TBL      value     "42706".
       78  SQL-NAME-NOT-IN-RESULT-TABLE        value     "42707".
       78  SQL-NAME-DUP-COL-IN-KEY             value     "42709".
       78  SQL-NAME-DUP-OBJ-NAME               value     "42710".
       78  SQL-NAME-DUP-COL-NAME               value     "42711".
       78  SQL-NAME-DUP-IN-FROM-CLAUSE         value     "42712".
       78  SQL-DUP-OBJECT-IN-LIST              value     "42713".
       78  SQL-NAME-NODE-NOT-FOUND-IN-DIR      value     "42720".
       78  SQL-NAME-DUP-SIGNATURE              value     "42723".
       78  SQL-ACCESS-UNABLE                   value     "42724".
       78  SQL-NAME-AMBIGUOUS                  value     "42725".
       78  SQL-DUP-DERIVED-TABLE-NAME          value     "42726".
       78  SQL-NEED-DEFAULT-PRIM-TBLSPACE      value     "42727".
       78  SQL-ACCESS-DUPLICATE-NODE           value     "42728".
       78  SQL-ACCESS-UNDEFINED-NODE           value     "42729".
       78  SQL-CONTNR-NAME-USEDBY-ANOTHER      value     "42730".
       78  SQL-CONTNR-NAME-USED                value     "42731".
       78  SQL-DUP-SCHEMA-NAME-IN-PATH         value     "42732".
       78  SQL-STATE-42734                     value     "42734".
       78  SQL-NODEGROUP-NOT-DEFINED           value     "42735".
       78  SQL-STATE-42736                     value     "42736".
       78  SQL-STATE-42737                     value     "42737".
       78  SQL-STATE-42738                     value     "42738".
       78  SQL-TRANSFORM-EXISTS                value     "42739".
       78  SQL-NO-TRANSFORM-DROPPED            value     "42740".
       78  SQL-TRANSFORM-NOT-FOUND             value     "42741".
       78  SQL-SUBTABLE-EXISTS                 value     "42742".
       78  SQL-METHOD-NOT-FOUND                value     "42743".
       78  SQL-STATE-42744                     value     "42744".
       78  SQL-STATE-42745                     value     "42745".
       78  SQL-STATE-42746                     value     "42746".
       78  SQL-DUPLICATE-STORAGE-PATH          value     "42748".
       78  SQL-NUM-INSERTS-INVALID             value     "42802".
       78  SQL-COLUMN-REF-INVALID              value     "42803".
       78  SQL-CASE-RES-NOT-COMPATIBLE         value     "42804".
       78  SQL-ORDER-BY-INT-NOT-IN-RESULT      value     "42805".
       78  SQL-DATA-TYPES-INCOMPAT             value     "42806".
       78  SQL-OPERATION-NOTPERMIT-ON-OBJ      value     "42807".
       78  SQL-COLUMN-NOT-UPDATEABLE           value     "42808".
       78  SQL-STATEMENT-NOT-APPLICABLE        value     "42809".
       78  SQL-FOREIGN-KEY-HAS-VIEW            value     "42810".
       78  SQL-NUM-COLS-NOT-MATCH-SELECT       value     "42811".
       78  SQL-VIEW-NOTALLOW-WITH-CHECK        value     "42813".
       78  SQL-SYNTAX-LANG-TYPE-INV            value     "42815".
       78  SQL-INV-USE-DATATIME-OR-DUR         value     "42816".
       78  SQL-OPERANDS-NOT-COMPATIBLE         value     "42818".
       78  SQL-NUMBER-IS-REQUIRED              value     "42819".
       78  SQL-NUM-CONST-OUT-OF-RANGE          value     "42820".
       78  SQL-VALUE-INCOMPAT-WITH-COLUMN      value     "42821".
       78  SQL-BAD-GROUP-ORDER-BY-EXPR         value     "42822".
       78  SQL-MULT-COLS-IN-SELECT-SUBQ        value     "42823".
       78  SQL-LIKE-OPERAND-INVALID            value     "42824".
       78  SQL-UNION-OPERANDS-COL-INCOMP       value     "42825".
       78  SQL-NUM-COLUMNS-DO-NOT-MATCH        value     "42826".
       78  SQL-TABLE-DIFFERS-FROM-CURSOR       value     "42827".
       78  SQL-TABLE-CANNOT-BE-MODIFIED        value     "42828".
       78  SQL-CANNOT-MODIFY-RESULT-TBL        value     "42829".
       78  SQL-FOREIGNKEY-NOT-LIKE-PARENT      value     "42830".
       78  SQL-NO-NULLS-ALLOWED-IN-KEY         value     "42831".
       78  SQL-AUTH-OPER-SYSOBJ-NOTALLOW       value     "42832".
       78  SQL-NULL-VALUE-NOTALLOW             value     "42834".
       78  SQL-UDF-INVALID-HANDLE-PLACED       value     "42835".
       78  SQL-RECURS-NAME-DERIVE-TBL-INV      value     "42836".
       78  SQL-COL-ATTRIBUTE-INCOMPAT          value     "42837".
       78  SQL-INV-USE-OF-TABLESPACE           value     "42838".
       78  SQL-MUST-USE-SAME-TABLESPACE        value     "42839".
       78  SQL-AS-CAST-USE-ERROR               value     "42840".
       78  SQL-PARM-MARKER-INV-NAME            value     "42841".
       78  SQL-SYNTAX-TYPE-OPT-INV             value     "42842".
       78  SQL-INV-BOOL-IN-SELECT-LIST         value     "42844".
       78  SQL-INV-VARIANT-FUNCTION            value     "42845".
       78  SQL-INV-CAST                        value     "42846".
       78  SQL-STATE-42849                     value     "42849".
       78  SQL-PRIVS-INV-OR-INCONSIST          value     "42852".
       78  SQL-STATE-42853                     value     "42853".
       78  SQL-RES-COL-TYPE-INCOMPAT           value     "42854".
       78  SQL-LOB-ASSIGN-NOT-ALLOWED          value     "42855".
       78  SQL-STATE-42858                     value     "42858".
       78  SQL-SYNTAX-TYPE-NOTMATCH            value     "42866".
       78  SQL-STATE-42867                     value     "42867".
       78  SQL-STATE-42872                     value     "42872".
       78  SQL-INV-SCHEMA-NAME                 value     "42875".
       78  SQL-CANNOT-QUALIFY-COLUMN           value     "42877".
       78  SQL-SYNTAX-XTERN-NAME-INV           value     "42878".
       78  SQL-SYNTAX-FUNC-SRC-NOTMATCH        value     "42879".
       78  SQL-SYNTAX-CAST-ERROR               value     "42880".
       78  SQL-INVALID-USE-ROW-FUNC            value     "42881".
       78  SQL-NAME-SCHEMA-DIFF                value     "42882".
       78  SQL-NAME-FUNC-SIGNATURE-UNDEF       value     "42883".
       78  SQL-NAME-FUNC-ARGUMENT-UNDEF        value     "42884".
       78  SQL-SYNTAX-PARM-NUM-NOTMATCH        value     "42885".
       78  SQL-STATE-42886                     value     "42886".
       78  SQL-FUNC-INV-IN-CONTEXT             value     "42887".
       78  SQL-TABLE-HAS-NO-PRIMKEY            value     "42888".
       78  SQL-PRIMARY-KEY-ALREADY-EXIST       value     "42889".
       78  SQL-NOT-UNIQUE-CONSTRAINT-COLS      value     "42890".
       78  SQL-DUP-UNIQUE-CONSTRAINT           value     "42891".
       78  SQL-OBJ-DEPENDS-ON-THIS             value     "42893".
       78  SQL-DEFAULT-VALUE-INV               value     "42894".
       78  SQL-SYNTAX-HOSTVAR-TYPE-INCOMP      value     "42895".
       78  SQL-INV-CORRELATION-REF             value     "42898".
       78  SQL-CORRELATION-REF-NOT-ALLOW       value     "42899".
       78  SQL-ERROR-IN-UDF                    value     "428A0".
       78  SQL-NOACCESS-HOSTFILE-VAR-FILE      value     "428A1".
       78  SQL-ACCESS-PART-NULL                value     "428A2".
       78  SQL-EVMON-BAD-PATH-SPECIFIED        value     "428A3".
       78  SQL-EVMON-BAD-OPTION-SPECIFIED      value     "428A4".
       78  SQL-INV-EXCEPTION-TABLE             value     "428A5".
       78  SQL-EXCEP-TBL-SAME-AS-CHK-TBL       value     "428A6".
       78  SQL-NUM-CHECK-TBLS-MISMATCH         value     "428A7".
       78  SQL-PARENT-TBL-IN-CHCK-PEND-ST      value     "428A8".
       78  SQL-ACCESS-INVALID-RANGE            value     "428A9".
       78  SQL-STATE-428AA                     value     "428AA".
       78  SQL-ILLEGAL-ELMNT-NSTNG             value     "428B0".
       78  SQL-INCORRECT-NUM-ON-NODES          value     "428B1".
       78  SQL-CONTNR-PATH-NM-NOT-VALID        value     "428B2".
       78  SQL-INV-SQLSTATE-FROM-APP           value     "428B3".
       78  SQL-STATE-428B7                     value     "428B7".
       78  SQL-DROP-ONLY-NODE                  value     "428C0".
       78  SQL-IDENTITY-ONLY-ONE               value     "428C1".
       78  SQL-CREATE-MISSING-CLAUSE           value     "428C2".
       78  SQL-NUM-ELEMENTS-NO-MATCH           value     "428C4".
       78  SQL-DATATYPE-MAPPING-NOT-FOUND      value     "428C5".
       78  SQL-IDENTITY-NO-INSERT              value     "428C9".
       78  SQL-TABLE-CLUSTERING-INDEX          value     "428CA".
       78  SQL-TABLESPACE-BUFFPOOL-MATCH       value     "428CB".
       78  SQL-DATALINK-FILE-NO-ACCESS         value     "428D1".
       78  SQL-STATE-428D4                     value     "428D4".
       78  SQL-STATE-428D5                     value     "428D5".
       78  SQL-STATE-428D6                     value     "428D6".
       78  SQL-STATE-428D7                     value     "428D7".
       78  SQL-STATE-428D8                     value     "428D8".
       78  SQL-INVALID-SUPER-OBJECT            value     "428DB".
       78  SQL-TRANSFORM-FUNCTION-INV          value     "428DC".
       78  SQL-TRANSFORM-NOT-DEFINED           value     "428DD".
       78  SQL-PAGESIZE-NOT-SUPPORTED          value     "428DE".
       78  SQL-CREATE-CAST-TYPE-INVALID        value     "428DF".
       78  SQL-CREATE-CAST-FUNC-INVALID        value     "428DG".
       78  SQL-TYPED-TABLE-OP-INVALID          value     "428DH".
       78  SQL-INHERIT-COL-NO-DROP             value     "428DJ".
       78  SQL-COLUMN-SCOPE-DEFINED            value     "428DK".
       78  SQL-FUNC-PARM-SCOPE-DEFINED         value     "428DL".
       78  SQL-TABLE-SCOPE-INVALID             value     "428DM".
       78  SQL-RETURNS-SCOPE-NOT-SPEC          value     "428DN".
       78  SQL-NOT-STRUCTURED-TYPE             value     "428DP".
       78  SQL-SUBTABLE-DIFF-SCHEMA            value     "428DQ".
       78  SQL-SUBTABLE-OP-NO-APPLY            value     "428DR".
       78  SQL-CANNOT-DEF-SUBTABLE-INDEX       value     "428DS".
       78  SQL-OPERAND-INVALID-TYPE            value     "428DT".
       78  SQL-TYPE-NOT-INCLUDED               value     "428DU".
       78  SQL-INVALID-DEF-OPERAND             value     "428DV".
       78  SQL-CANNOT-REF-OBJECT-COL           value     "428DW".
       78  SQL-OBJECT-COL-REQ-ROOT             value     "428DX".
       78  SQL-NO-UPDATE-SUBTABLE-STATS        value     "428DY".
       78  SQL-OBJECT-COLUMN-NO-UPDATE         value     "428DZ".
       78  SQL-INDEX-DEF-NO-MATCH              value     "428E0".
       78  SQL-RANGE-FUNC-INCONSISTENT         value     "428E1".
       78  SQL-NUM-KEY-PARMS-MISMATCH          value     "428E2".
       78  SQL-INDEX-FUNC-ARG-INVALID          value     "428E3".
       78  SQL-FUNC-NOT-SUPP-CRTINDEX          value     "428E4".
       78  SQL-STATE-428E5                     value     "428E5".
       78  SQL-SEARCH-ARG-MISMATCH             value     "428E6".
       78  SQL-USER-DEF-PRED-MISMATCH          value     "428E7".
       78  SQL-SEARCH-TARGET-MISMATCH          value     "428E8".
       78  SQL-ARG-PARM-NO-BOTH                value     "428E9".
       78  SQL-INV-FULLSEL-TYPED-VIEW          value     "428EA".
       78  SQL-COLUMN-NO-READ-ONLY             value     "428EB".
       78  SQL-SUMM-TABLE-FULLSELECT-INV       value     "428EC".
       78  SQL-STRUCT-TYPE-CANNOT-CONST        value     "428ED".
       78  SQL-STATE-428EE                     value     "428EE".
       78  SQL-STATE-428EF                     value     "428EF".
       78  SQL-STATE-428EG                     value     "428EG".
       78  SQL-STATE-428EH                     value     "428EH".
       78  SQL-STATE-428EJ                     value     "428EJ".
       78  SQL-STATE-428EK                     value     "428EK".
       78  SQL-STATE-428EL                     value     "428EL".
       78  SQL-STATE-428EM                     value     "428EM".
       78  SQL-STATE-428EN                     value     "428EN".
       78  SQL-STATE-428EP                     value     "428EP".
       78  SQL-STATE-428EQ                     value     "428EQ".
       78  SQL-STATE-428ER                     value     "428ER".
       78  SQL-STATE-428ES                     value     "428ES".
       78  SQL-STATE-428EU                     value     "428EU".
       78  SQL-STATE-428EV                     value     "428EV".
       78  SQL-STATE-428EW                     value     "428EW".
       78  SQL-STATE-428EX                     value     "428EX".
       78  SQL-STATE-428EY                     value     "428EY".
       78  SQL-STATE-428EZ                     value     "428EZ".
       78  SQL-STATE-428F0                     value     "428F0".
       78  SQL-STATE-428F1                     value     "428F1".
       78  SQL-STATE-428F2                     value     "428F2".
       78  SQL-STATE-428F3                     value     "428F3".
       78  SQL-STATE-428F4                     value     "428F4".
       78  SQL-STATE-428F6                     value     "428F6".
       78  SQL-STATE-428F7                     value     "428F7".
       78  SQL-STATE-428F9                     value     "428F9".
       78  SQL-STATE-428FA                     value     "428FA".
       78  SQL-STATE-428FB                     value     "428FB".
       78  SQL-STATE-428FC                     value     "428FC".
       78  SQL-STATE-428FD                     value     "428FD".
       78  SQL-STATE-428FE                     value     "428FE".
       78  SQL-STATE-428FF                     value     "428FF".
       78  SQL-STATE-428FG                     value     "428FG".
       78  SQL-STATE-428FH                     value     "428FH".
       78  SQL-STATE-428FI                     value     "428FI".
       78  SQL-STATE-428FJ                     value     "428FJ".
       78  SQL-STATE-428FL                     value     "428FL".
       78  SQL-STATE-428FM                     value     "428FM".
       78  SQL-STATE-428FP                     value     "428FP".
       78  SQL-STATE-428FQ                     value     "428FQ".
       78  SQL-STATE-428FU                     value     "428FU".
       78  SQL-STATE-428FV                     value     "428FV".
       78  SQL-STATE-428FZ                     value     "428FZ".
       78  SQL-STATE-428G3                     value     "428G3".
       78  SQL-STATE-428G4                     value     "428G4".
       78  SQL-STATE-428G5                     value     "428G5".
       78  SQL-STATE-428G6                     value     "428G6".
       78  SQL-STATE-428G8                     value     "428G8".
       78  SQL-STATE-428G9                     value     "428G9".
       78  SQL-STATE-428GA                     value     "428GA".
       78  SQL-COL-FUNC-NEEDS-COL-NAME         value     "42901".
       78  SQL-OBJECT-ALSO-IN-FROM-CLAUSE      value     "42902".
       78  SQL-WHERE-OR-SET-HAS-INV-REF        value     "42903".
       78  SQL-STATE-42904                     value     "42904".
       78  SQL-STRING-IS-TOO-LONG              value     "42907".
       78  SQL-COLUMN-LIST-MISSING             value     "42908".
       78  SQL-STATE-42910                     value     "42910".
       78  SQL-INVALID-DECIMAL-DIVIDE          value     "42911".
       78  SQL-COLUMN-NOT-IN-UPD-CLAUSE        value     "42912".
       78  SQL-UPDATE-OR-DELETE-INVALID        value     "42913".
       78  SQL-TABLE-IN-SUBQUERY-AFFECTED      value     "42914".
       78  SQL-INV-REFERENTIAL-CONSTRAINT      value     "42915".
       78  SQL-REP-CHAIN-ALIAS-INVALID         value     "42916".
       78  SQL-CANNOT-EXPLICITLY-DROP-OBJ      value     "42917".
       78  SQL-NAME-RESERVED                   value     "42918".
       78  SQL-STATE-42919                     value     "42919".
       78  SQL-CANNOT-ADD-CONTAINERS           value     "42921".
       78  SQL-REC-NAME-DER-TLB-SPEC-INV       value     "42925".
       78  SQL-STATE-42928                     value     "42928".
       78  SQL-PROG-ASSUMPTIONS-INCORRECT      value     "42932".
       78  SQL-AUTH-CREATE-SYSOBJ-NOALLOW      value     "42939".
       78  SQL-SERVER-NAME-MISMATCH            value     "42961".
       78  SQL-INV-USE-LONG-COLUMN             value     "42962".
       78  SQL-NO-SW-LICENSE                   value     "42968".
       78  SQL-PACKAGE-NOT-CREATED             value     "42969".
       78  SQL-INV-JOIN-CONDITION              value     "42972".
       78  SQL-STATE-42985                     value     "42985".
       78  SQL-INV-CONN-RESET                  value     "42986".
       78  SQL-STMT-INV-FOR-STORPROC-TRIG      value     "42987".
       78  SQL-STATE-42989                     value     "42989".
       78  SQL-SYNTAX-SYSTEM-USE-ONLY          value     "42991".
       78  SQL-LOB-COL-TOO-BIG-TO-LOG          value     "42993".
       78  SQL-RAW-DEVICE-NOT-SUPP             value     "42994".
       78  SQL-FUNC-NO-GLOBAL-TEMP             value     "42995".
       78  SQL-UNIQUE-NOT-ALLOWED              value     "42997".
       78  SQL-INV-FK-REF                      value     "429A0".
       78  SQL-INV-NODEGROUP                   value     "429A1".
       78  SQL-DATAJOINER-NO-PROCESS           value     "429A9".
       78  SQL-INV-NOTLOGINIT                  value     "429AA".
       78  SQL-TYPE-LENGTH-TOO-SMALL           value     "429B2".
       78  SQL-NO-DEF-OBJECT-SUBTBL            value     "429B3".
       78  SQL-FILTER-FUNC-NO-LANG             value     "429B4".
       78  SQL-INSTANCE-PARM-TYPE-INV          value     "429B5".
       78  SQL-STATE-429B8                     value     "429B8".
       78  SQL-STATE-429B9                     value     "429B9".
       78  SQL-STATE-429BA                     value     "429BA".
       78  SQL-STATE-429BB                     value     "429BB".
       78  SQL-STATE-429BC                     value     "429BC".
       78  SQL-STATE-429BE                     value     "429BE".
       78  SQL-STATE-429BG                     value     "429BG".
       78  SQL-STATE-429BJ                     value     "429BJ".
       78  SQL-STATE-429BK                     value     "429BK".
       78  SQL-STATE-429BL                     value     "429BL".
       78  SQL-STATE-429BO                     value     "429BO".
       78  SQL-STATE-429BP                     value     "429BP".
       78  SQL-STATE-429DB                     value     "429DB".
       78  SQL-VIEW-DEF-NOT-SATISFIED          value     "44000".
       78  SQL-STATE-46001                     value     "46001".
       78  SQL-STATE-46002                     value     "46002".
       78  SQL-STATE-46003                     value     "46003".
       78  SQL-STATE-46007                     value     "46007".
       78  SQL-STATE-46008                     value     "46008".
       78  SQL-STATE-46103                     value     "46103".
       78  SQL-STATE-46501                     value     "46501".
       78  SQL-STATE-CALL-INV                  value     "51001".
       78  SQL-STATE-PACKAGE-NOTFOUND          value     "51002".
       78  SQL-STATE-TOKENS-NOTMATCH           value     "51003".
       78  SQL-STATE-ADDR-INV-DA               value     "51004".
       78  SQL-STATE-FUNC-DISABLED             value     "51005".
       78  SQL-STATE-STMT-NOTEXEC              value     "51007".
       78  SQL-STATE-RELEASE-INV               value     "51008".
       78  SQL-STATE-PROC-NOTCONNECTABLE       value     "51011".
       78  SQL-STATE-SECTION-BINDERR           value     "51015".
       78  SQL-STATE-USER-NOTLOGGEDON          value     "51017".
       78  SQL-STATE-APP-UNCONNECTED           value     "51018".
      * Planned
       78  SQL-STATE-CRTPACK-BUSY              value     "51019".
      * Planned
       78  SQL-STATE-PACK-NONE                 value     "51020".
       78  SQL-ROLLBACK-REQUIRED               value     "51021".
       78  SQL-CONN-WITH-AUTH-ALREADY          value     "51022".
       78  SQL-STATE-DB-INUSE-ANOTH-INST       value     "51023".
       78  SQL-INOPERAITVE-VIEW                value     "51024".
       78  SQL-CONN-SETTINGS-IN-USE            value     "51025".
       78  SQL-EVMON-PATH-INUSE                value     "51026".
       78  SQL-IMMEDIATE-CHECKED-OPT-INV       value     "51027".
       78  SQL-PACKAGE-INOPERATIVE             value     "51028".
       78  SQL-STATE-51030                     value     "51030".
       78  SQL-STATE-51034                     value     "51034".
       78  SQL-STATE-51035                     value     "51035".
       78  SQL-STATE-51038                     value     "51038".
       78  SQL-STATE-51039                     value     "51039".
       78  SQL-STATE-51040                     value     "51040".
       78  SQL-NAME-COL-DUP-STMT               value     "52001".
       78  SQL-NAME-COLREF-DUP-NAME            value     "52002".
       78  SQL-NAME-COL-UNDEF                  value     "52003".
       78  SQL-NAME-OBJ-UNDEF                  value     "52004".
       78  SQL-NAME-SERVER-UNDEF               value     "52005".
       78  SQL-NAME-COL-INV                    value     "52006".
       78  SQL-NAME-COL-NOTRESULT              value     "52007".
       78  SQL-NAME-CURSOR-UNDEF               value     "52008".
       78  SQL-NAME-COL-DUP-KEYCLAUSE          value     "52009".
       78  SQL-NAME-OBJ-DUP                    value     "52010".
       78  SQL-NAME-COL-DUP-OBJDEF             value     "52011".
       78  SQL-NAME-TABLE-DUP                  value     "52012".
      * Planned
       78  SQL-NAME-CURSOR-DUP                 value     "52013".
      * Planned
       78  SQL-NAME-CURSOR-NOTQUAL             value     "52015".
      * Planned
       78  SQL-NAME-COLREF-DUP-SET             value     "52016".
       78  SQL-NAME-AS-NONAME                  value     "52017".
      * Planned
       78  SQL-NAME-CURSOR-UNDECL              value     "52019".
       78  SQL-NAME-NODE-NOTFOUND              value     "52020".
      * Planned
       78  SQL-NAME-REG-UNKNOWN                value     "52021".
       78  SQL-STATE-5U001                     value     "5U001".
       78  SQL-STATE-5U002                     value     "5U002".
       78  SQL-STATE-5U003                     value     "5U003".
       78  SQL-STATE-5U004                     value     "5U004".
       78  SQL-STATE-5U005                     value     "5U005".
       78  SQL-STATE-5U006                     value     "5U006".
       78  SQL-STATE-5U0ZZ                     value     "5U0ZZ".
       78  SQL-STATE-ZZZZZ                     value     "ZZZZZ".
       78  SQL-OSPEC-NUMVAL-NOTSAME            value     "53002".
       78  SQL-OSPEC-COLUMN-INV                value     "53003".
       78  SQL-OSPEC-CLAUSE-NOTCOLUMN          value     "53005".
       78  SQL-OSPEC-OP-NOTPERMIT              value     "53007".
       78  SQL-OSPEC-COL-NOTUPDATABLE          value     "53008".
       78  SQL-OSPEC-OBJ-WRONGTYPE             value     "53009".
       78  SQL-OSPEC-CLAUSE-ISVIEW             value     "53010".
       78  SQL-OSPEC-NUMCOL-NOTSAME            value     "53011".
       78  SQL-OSPEC-OPTION-BAD                value     "53013".
       78  SQL-OSPEC-DATA-INV                  value     "53015".
       78  SQL-OSPEC-DATETIME-INV-EXPR         value     "53016".
       78  SQL-OSPEC-NUMVAR-NOTSAME            value     "53017".
       78  SQL-OSPEC-OPERANDS-NOTCOMPAT        value     "53018".
       78  SQL-OSPEC-OP-NOTNUMB                value     "53019".
       78  SQL-OSPEC-CONS-TOOLONG              value     "53020".
       78  SQL-OSPEC-VAL-NOTCOMPAT-COL         value     "53021".
       78  SQL-OSPEC-CLAUSE-TOOMANY            value     "53023".
       78  SQL-OSPEC-LIKEOP-BAD                value     "53024".
       78  SQL-OSPEC-UNIONOP-INCOMPAT-COL      value     "53025".
       78  SQL-OSPEC-UNIONOP-BADNUM-COL        value     "53026".
       78  SQL-OSPEC-TABLE-NOTSAME-CURSOR      value     "53027".
       78  SQL-OSPEC-TABLE-CANTMODIFY          value     "53028".
       78  SQL-OSPEC-UPDATE-INV-ROTABLE        value     "53029".
       78  SQL-OSPEC-FKEY-NOTCONFORM           value     "53030".
       78  SQL-OSPEC-NULL-NOTALLOW             value     "53031".
       78  SQL-OSPEC-OP-NOTALLOW-SYSOBJ        value     "53032".
       78  SQL-OSPEC-FKEY-NOTALLOW-NULL        value     "53034".
       78  SQL-STATE-53040                     value     "53040".
       78  SQL-OSPEC-COL-INV                   value     "53042".
       78  SQL-OSPEC-PRIV-INV                  value     "53052".
      * Planned
       78  SQL-OSPEC-OPT-BOTH                  value     "53053".
       78  SQL-OSPEC-AUTH-BAD                  value     "53056".
      * Planned
       78  SQL-OSPEC-STMT-CANTUSE              value     "53059".
       78  SQL-OSPEC-DB-CANTUSE                value     "53061".
      * Planned
       78  SQL-OSPEC-STMT-CANTEXEC             value     "53062".
       78  SQL-OSPEC-HOSTVAR-UNDEF             value     "53063".
      * Planned
       78  SQL-OSPEC-INFO-NOTMATCH             value     "53064".
      * Planned
       78  SQL-OSPEC-OPTIONS-CONFLICT          value     "53067".
      * Planned
       78  SQL-OSPEC-DESCRIPTORS-TOOFEW        value     "53070".
      * Planned
       78  SQL-OSPEC-NUMSTRUCTS-NOTMATCH       value     "53071".
      * Planned
       78  SQL-OSPEC-CLAUSE-INCOMPAT           value     "53072".
      * Planned
       78  SQL-OSPEC-NUMROWS-INV               value     "53073".
       78  SQL-STATE-53090                     value     "53090".
       78  SQL-STATE-53091                     value     "53091".
       78  SQL-LIMIT-STMT-TOOLONG              value     "54001".
       78  SQL-LIMIT-SCONST-TOOLONG            value     "54002".
       78  SQL-LIMIT-NAME-TOOLONG              value     "54003".
       78  SQL-LIMIT-NAMES-TOOMANY             value     "54004".
       78  SQL-LIMIT-CONCAT-TOOLONG            value     "54006".
      * Planned
       78  SQL-LIMIT-AS-TOOBIG                 value     "54007".
       78  SQL-LIMIT-KEY-TOOLONG               value     "54008".
       78  SQL-LIMIT-TABLE-TOOLONG             value     "54010".
       78  SQL-LIMIT-COLS-TOOMANY              value     "54011".
       78  SQL-LIMIT-DESCRIPTORS-TOOMANY       value     "54019".
       78  SQL-LIMIT-ARGUMENTS-TOOMANY         value     "54023".
       78  SQL-LIMIT-CONCUR-LOB-HANDLES        value     "54028".
       78  SQL-LIMIT-OPEN-DIR-SCANS            value     "54029".
       78  SQL-LIMIT-EVMON-TOOMANY             value     "54030".
       78  SQL-LIMIT-EVTFILES-TOOMANY          value     "54031".
       78  SQL-LIMIT-MAX-TABLE-SIZE            value     "54032".
       78  SQL-LIMIT-PMAP-TOOMANY              value     "54033".
       78  SQL-LIMIT-CONTNR-NAMES-LEN          value     "54034".
       78  SQL-LIMIT-TBLSPACE-SIZE             value     "54035".
       78  SQL-LIMIT-CONTNR-PATH-LEN           value     "54036".
       78  SQL-LIMIT-CONTAIN-MAP-COMPLX        value     "54037".
       78  SQL-LIMIT-CASCADED-TRIGGERS         value     "54038".
       78  SQL-LIMIT-CONTAINER-SIZE            value     "54039".
       78  SQL-LIMIT-TRANSITION-TBL            value     "54040".
       78  SQL-LIMIT-TYPE-LEVEL                value     "54045".
       78  SQL-LIMIT-MAX-PARM-INDEX            value     "54046".
       78  SQL-LIMIT-MAX-TSPACE-SIZE           value     "54047".
       78  SQL-NO-TEMP-TSPACE-PAGE-SIZE        value     "54048".
       78  SQL-STATE-54049                     value     "54049".
       78  SQL-STATE-54050                     value     "54050".
       78  SQL-STATE-54052                     value     "54052".
       78  SQL-STATE-54053                     value     "54053".
       78  SQL-STATE-DB-NOTMIGR                value     "55001".
       78  SQL-STATE-EXPL-TBL-NOT-DEFINED      value     "55002".
       78  SQL-STATE-OBJ-INUSE                 value     "55006".
       78  SQL-STATE-OBJ-NOTALTER              value     "55007".
       78  SQL-STATE-TABLE-NOKEY               value     "55008".
       78  SQL-STATE-FILE-READONLY             value     "55009".
      * Planned
       78  SQL-STATE-OBJ-HASDEPS               value     "55010".
       78  SQL-STATE-TABLE-HASCLUSTIDX         value     "55012".
       78  SQL-STATE-TABLE-HASKEY              value     "55013".
       78  SQL-STATE-TABLE-INVALID             value     "55019".
       78  SQL-SERVER-NOT-REGISTERED           value     "55022".
       78  SQL-STATE-55023                     value     "55023".
       78  SQL-CHK-CONSTRAINT-TOO-LONG         value     "55024".
       78  SQL-STATE-DB-NEEDRESTART            value     "55025".
       78  SQL-TRIGGER-DEF-TOO-LONG            value     "55026".
      * Planned
       78  SQL-STATE-OS2-RESERVED              value     "55031".
      * Planned
       78  SQL-CONN-INV-DBM-STOPPED            value     "55032".
       78  SQL-MUST-COMMIT-FOR-EVMON           value     "55033".
       78  SQL-EVMON-ACTIVE-CANT-DROP          value     "55034".
       78  SQL-STATE-55035                     value     "55035".
       78  SQL-STATE-DROP-PART-KEY             value     "55037".
       78  SQL-STATE-REBALANCING               value     "55038".
       78  SQL-INV-STATE-CHANGE                value     "55039".
       78  SQL-STATE-55040                     value     "55040".
       78  SQL-REBALANCE-IN-PROGRESS           value     "55041".
       78  SQL-TYPED-TABLE-EXISTS              value     "55043".
       78  SQL-STATE-55045                     value     "55045".
       78  SQL-STATE-55046                     value     "55046".
       78  SQL-STATE-55047                     value     "55047".
       78  SQL-STATE-55048                     value     "55048".
       78  SQL-STATE-55049                     value     "55049".
       78  SQL-STATE-55051                     value     "55051".
       78  SQL-STATE-55054                     value     "55054".
       78  SQL-STATE-55056                     value     "55056".
       78  SQL-NO-AUTOMATIC-STORAGE            value     "55060".
       78  SQL-NO-REDIRECTED-RESTORE           value     "55061".
       78  SQL-STORAGE-PATHS-NOT-ALLOWED       value     "55062".
       78  SQL-MISC-NONAME                     value     "56001".
       78  SQL-MISC-FROM-INV                   value     "56002".
       78  SQL-MISC-WHERE-INV                  value     "56003".
       78  SQL-MISC-DISTINCT-TOOMANY           value     "56005".
       78  SQL-MISC-STR-TOOLONG                value     "56007".
       78  SQL-MISC-LIST-MISSING               value     "56008".
       78  SQL-MISC-DIVIDE-NEG                 value     "56011".
       78  SQL-MISC-COL-NOUPDATE               value     "56012".
       78  SQL-MISC-UPDDEL-INV                 value     "56013".
       78  SQL-MISC-DEL-INV                    value     "56014".
       78  SQL-MISC-CONSTR-INV                 value     "56015".
       78  SQL-MISC-OBJECT-NODROP              value     "56017".
       78  SQL-MISC-INV-COMM-ROLL              value     "56021".
      * Planned
       78  SQL-MISC-REF-INV                    value     "56023".
      * Planned
       78  SQL-MISC-ALIAS-NOTTABLE             value     "56024".
       78  SQL-MISC-OP-INV                     value     "56026".
       78  SQL-MISC-MIXED-DBCS-INV             value     "56031".
       78  SQL-MISC-PREP-INV                   value     "56032".
       78  SQL-MISC-STRING-HOSTVAR             value     "56033".
       78  SQL-STATE-56038                     value     "56038".
       78  SQL-MISC-OBJ-NOTCRT                 value     "56039".
      * Planned
       78  SQL-MISC-PACKAGE-NOTCOMMIT          value     "56043".
      * Planned
       78  SQL-MISC-NAME-NOTID                 value     "56052".
      * Planned
       78  SQL-MISC-DATA-NOTSUPP               value     "56059".
      * Planned
       78  SQL-MISC-STMT-NOTEXEC               value     "56060".
       78  SQL-MISC-NAME-NOTMATCH              value     "56061".
       78  SQL-MISC-ISO-NOTUSER                value     "56068".
      * Planned
       78  SQL-MISC-JOIN-BADCOLS               value     "56072".
      * Planned
       78  SQL-MISC-NAME-REQUIRED              value     "56073".
      * Planned
       78  SQL-MISC-NAME-NOTQUAL               value     "56074".
       78  SQL-LOB-NOSUPP-IN-DRDA              value     "56084".
       78  SQL-MISC-N-A-COMPOUND-SQL-ERR       value     "56091".
       78  SQL-MISC-NAME-BOTH-USER-GROUP       value     "56092".
       78  SQL-MISC-NO-BLOB-IN-DWNLVL-CL       value     "56093".
       78  SQL-STATE-56095                     value     "56095".
       78  SQL-MISC-NOT-ALIGNED                value     "56097".
       78  SQL-MISC-PREP-REBIND-INV            value     "56098".
       78  SQL-MISC-REAL-NOT-SUPPORTED         value     "56099".
       78  SQL-MISC-LOB-ACTION-FAILED          value     "560A0".
       78  SQL-FUNC-CLAUSE-INVALID-UCS2        value     "560AA".
       78  SQL-STATE-560AC                     value     "560AC".
       78  SQL-STATE-560AF                     value     "560AF".
       78  SQL-STATE-560B0                     value     "560B0".
       78  SQL-STATE-560B1                     value     "560B1".
       78  SQL-STATE-560B7                     value     "560B7".
       78  SQL-STATE-560BB                     value     "560BB".
       78  SQL-STATE-560BC                     value     "560BC".
       78  SQL-STATE-560BD                     value     "560BD".
       78  SQL-STATE-560BF                     value     "560BF".
       78  SQL-STATE-560C0                     value     "560C0".
       78  SQL-STATE-560C1                     value     "560C1".
       78  SQL-STATE-560C2                     value     "560C2".
       78  SQL-STATE-560C3                     value     "560C3".
       78  SQL-STATE-560C6                     value     "560C6".
       78  SQL-STATE-560C8                     value     "560C8".
       78  SQL-STATE-560C9                     value     "560C9".
       78  SQL-STATE-560CA                     value     "560CA".
       78  SQL-STATE-57011                     value     "57011".
       78  SQL-STATE-57053                     value     "57053".
       78  SQL-STATE-57059                     value     "57059".
       78  SQL-RSRC-TABLE-UNAVAILABLE          value     "57001".
       78  SQL-RSRC-BUFFPOOL-INACTIVE          value     "57003".
       78  SQL-RSRC-PENDING                    value     "57007".
       78  SQL-RSRC-VSDB-TUNAVAIL              value     "57009".
       78  SQL-RSRC-VSDB-UNAVAIL               value     "57011".
       78  SQL-RSRC-NONDB-UNAVAIL-CONT         value     "57012".
       78  SQL-RSRC-NONDB-UNAVAIL-STOP         value     "57013".
       78  SQL-RSRC-CANCEL                     value     "57014".
       78  SQL-RSRC-TABLE-INACTIVE             value     "57016".
       78  SQL-RSRC-CXLATE-NOTDEF              value     "57017".
       78  SQL-RSRC-DRIVE-NOTEXIST             value     "57019".
       78  SQL-RSRC-DRIVE-LOCKED               value     "57020".
       78  SQL-RSRC-DRIVE-OPEN                 value     "57021".
       78  SQL-RSRC-NO-DBSPACES                value     "57022".
       78  SQL-RSRC-MAXAPPS                    value     "57030".
       78  SQL-RSRC-MAXDB                      value     "57032".
       78  SQL-RSRC-DB-STARTED                 value     "57033".
       78  SQL-RSRC-DB-ACQUIRED                value     "57034".
       78  SQL-RSRC-DBFILE-NOTOWN              value     "57035".
       78  SQL-RSRC-LOG-NOTOWN                 value     "57036".
       78  SQL-RSRC-STANDALONE                 value     "57041".
       78  SQL-RSRC-QUIESCE-BACKUP             value     "57046".
       78  SQL-RSRC-DIR-NOT-ACCESS             value     "57047".
       78  SQL-RSRC-CONTNR-ACC-ERR             value     "57048".
       78  SQL-RSRC-SYS-LIMIT                  value     "57049".
       78  SQL-RSRC-PATH                       value     "57050".
       78  SQL-RSRC-CPU-COST                   value     "57051".
       78  SQL-RSRC-UNAVAIL-NODE               value     "57052".
       78  SQL-RSRC-TABSPACE-SIZE              value     "57055".
       78  SQL-RSRC-NO-PACKAGE-LOCK            value     "57056".
       78  SQL-RSRC-DRDA-CHAIN-COND            value     "57057".
       78  SQL-RSRC-DISK-STUFFIT               value     "57901".
       78  SQL-RSRC-DISK-BAD                   value     "57902".
       78  SQL-RSRC-DISK-FULL                  value     "57903".
       78  SQL-SYSERR-INV-SECT                 value     "58003".
       78  SQL-SYSERR-ERR-CONT                 value     "58004".
       78  SQL-SYSERR-ERR-STOP                 value     "58005".
       78  SQL-SYSERR-DIST-CONT                value     "58008".
       78  SQL-SYSERR-DIST-DEALLOC             value     "58009".
       78  SQL-SYSERR-DIST-STOP                value     "58010".
       78  SQL-SYSERR-BIND-ACTIVE              value     "58011".
       78  SQL-SYSERR-BIND-INACTIVE            value     "58012".
       78  SQL-SYSERR-CMD-NOTSUPP              value     "58014".
       78  SQL-SYSERR-OBJ-NOTSUPP              value     "58015".
       78  SQL-SYSERR-PARAM-NOTSUPP            value     "58016".
       78  SQL-SYSERR-VALUE-NOTSUPP            value     "58017".
       78  SQL-SYSERR-MSG-NOTSUPP              value     "58018".
       78  SQL-SYSERR-COMMERR                  value     "58019".
       78  SQL-SYSERR-NOTDEF                   value     "58020".
       78  SQL-SYSERR-PROG-CANCELED            value     "58023".
      * Planned
       78  SQL-SYSERR-HOSTVARS                 value     "58026".
       78  SQL-SYSERR-IOERR                    value     "58030".
       78  SQL-SYSERR-DBFILE                   value     "58031".
       78  SQL-SYSERR-STR-FENCED-FUN-FAIL      value     "58032".
       78  SQL-SYSERR-FIND-TBSP-PG-FAIL        value     "58034".
       78  SQL-SYSERR-FREE-TBSP-PG-FAIL        value     "58035".
       78  SQL-SYSERR-TBLSPC-ID-NOEXIST        value     "58036".

