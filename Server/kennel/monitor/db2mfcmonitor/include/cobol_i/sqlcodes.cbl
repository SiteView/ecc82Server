      *************************************************************************
      * 
      * Source File Name: SQLCODES
      * 
      * (C) COPYRIGHT International Business Machines Corp. 1987, 1997
      * All Rights Reserved
      * Licensed Materials - Property of IBM
      * 
      * US Government Users Restricted Rights - Use, duplication or
      * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
      * 
      * Function: Include File defining:
      *             Labels for SQLCODES
      * 
      * Operating System:  Windows NT
      * 
      **************************************************************************
      * SQL Return Codes in SQLCODE

      * ------------ warnings ------------
      * correlation without qualification
       77  SQL-RC-W012               PIC S9(4) COMP-5 VALUE 12.
      * unsupported bind/prep options
       77  SQL-RC-W020               PIC S9(4) COMP-5 VALUE 20.
      * eof
       77  SQL-RC-W100               PIC S9(4) COMP-5 VALUE 100.

      * wrong nbr of insert values
       77  SQL-RC-W117               PIC S9(4) COMP-5 VALUE 117.
      * duplicate column conistraint
       77  SQL-RC-W139               PIC S9(4) COMP-5 VALUE 139.
      * invalid syntax ignored
       77  SQL-RC-W143               PIC S9(4) COMP-5 VALUE 143.

      * undefined name
       77  SQL-RC-W204               PIC S9(4) COMP-5 VALUE 204.
      * not a column
       77  SQL-RC-W205               PIC S9(4) COMP-5 VALUE 205.
      * not a column of referenced tables
       77  SQL-RC-W206               PIC S9(4) COMP-5 VALUE 206.
      * explain mode incompatible
       77  SQL-RC-W217               PIC S9(4) COMP-5 VALUE 217.
      * hole detected during fetch of cursor
       77  SQL-RC-W222               PIC S9(4) COMP-5 VALUE 222.
      * cursor position prevents FETCH current row
       77  SQL-RC-W231               PIC S9(4) COMP-5 VALUE 231.
      * not enough sqlvars, none filled in
       77  SQL-RC-W236               PIC S9(4) COMP-5 VALUE 236.
      * distinct type: not enough sqlvars, some filled in
       77  SQL-RC-W237               PIC S9(4) COMP-5 VALUE 237.
      * LOB type: not enough sqlvars, none filled in
       77  SQL-RC-W238               PIC S9(4) COMP-5 VALUE 238.
      * distinct type: not enough sqlvars, none filled in
       77  SQL-RC-W239               PIC S9(4) COMP-5 VALUE 239.
      * new view replaced old
       77  SQL-RC-W280               PIC S9(4) COMP-5 VALUE 280.
      * error accessing container
       77  SQL-RC-W293               PIC S9(4) COMP-5 VALUE 293.

      * possible infinite loop
       77  SQL-RC-W347               PIC S9(4) COMP-5 VALUE 347.
      * DATALINK value may not be valid
       77  SQL-RC-W360               PIC S9(4) COMP-5 VALUE 360.
      * SQLSTATE or SQLCODE may be over-written
       77  SQL-RC-W385               PIC S9(4) COMP-5 VALUE 385.

      * alias target is not defined
       77  SQL-RC-W403               PIC S9(4) COMP-5 VALUE 403.
      * invalid data type for column
       77  SQL-RC-W408               PIC S9(4) COMP-5 VALUE 408.
      * clause value has been replaced
       77  SQL-RC-W434               PIC S9(4) COMP-5 VALUE 434.
      * Sub-optimal query
       77  SQL-RC-W437               PIC S9(4) COMP-5 VALUE 437.
      * No function with compatible arg
       77  SQL-RC-W440               PIC S9(4) COMP-5 VALUE 440.
      * Function has truncated value
       77  SQL-RC-W445               PIC S9(4) COMP-5 VALUE 445.
      * Create UDF contains redundant keyw
       77  SQL-RC-W447               PIC S9(4) COMP-5 VALUE 447.
      * Procedure returned too many query result sets
       77  SQL-RC-W464               PIC S9(4) COMP-5 VALUE 464.
      * UDF returns a warning sqlstate
       77  SQL-RC-W462               PIC S9(4) COMP-5 VALUE 462.
      * one or more results sets for stored procedure
       77  SQL-RC-W466               PIC S9(4) COMP-5 VALUE 466.
      * Another result set exists for a stored procedure
       77  SQL-RC-W467               PIC S9(4) COMP-5 VALUE 467.
      * DDL:could truncate value at runtime
       77  SQL-RC-W474               PIC S9(4) COMP-5 VALUE 474.
      * DDL:could truncate value at runtime
       77  SQL-RC-W477               PIC S9(4) COMP-5 VALUE 477.
      * Number of result sets greater than number of locators
       77  SQL-RC-W494               PIC S9(4) COMP-5 VALUE 494.

      * duplicate referential constraint
       77  SQL-RC-W541               PIC S9(4) COMP-5 VALUE 541.
      * authorization error w/obj insert
       77  SQL-RC-W551               PIC S9(4) COMP-5 VALUE 551.
      * auth error w/o obj ins
       77  SQL-RC-W552               PIC S9(4) COMP-5 VALUE 552.
      * revoke stmt denied--priv not held
       77  SQL-RC-W556               PIC S9(4) COMP-5 VALUE 556.
      * revoke stmt denied--has CONTROL
       77  SQL-RC-W558               PIC S9(4) COMP-5 VALUE 558.
      * some privileges were not granted
       77  SQL-RC-W570               PIC S9(4) COMP-5 VALUE 570.
      * Duplicate schema name     
       77  SQL-RC-W585               PIC S9(4) COMP-5 VALUE 585.
      * Isolation level escalated
       77  SQL-RC-W595               PIC S9(4) COMP-5 VALUE 595.
      * existing index used
       77  SQL-RC-W598               PIC S9(4) COMP-5 VALUE 598.
      * compare func not created for lstr
       77  SQL-RC-W599               PIC S9(4) COMP-5 VALUE 599.

      * index already exists
       77  SQL-RC-W605               PIC S9(4) COMP-5 VALUE 605.

      * set stmt not supported
       77  SQL-RC-W799               PIC S9(4) COMP-5 VALUE 799.

      * duplicate row warning
       77  SQL-RC-W803               PIC S9(4) COMP-5 VALUE 803.

      * unknown SQL warning another product
       77  SQL-RC-W965               PIC S9(4) COMP-5 VALUE 965.
      * XA informational message
       77  SQL-RC-W997               PIC S9(4) COMP-5 VALUE 997.

      * already existing index was migrated
       77  SQL-RC-W1138              PIC S9(4) COMP-5 VALUE 1138.
      * estimate cost exceeds resource limit warning threshold
       77  SQL-RC-W1140              PIC S9(4) COMP-5 VALUE 1140.

      * DataLink column not defined on DLM 
       77  SQL-RC-W1161              PIC S9(4) COMP-5 VALUE 1161.
      * DLM down during exception processing 
       77  SQL-RC-W1162              PIC S9(4) COMP-5 VALUE 1162.

      * value not within host variable data type range
       77  SQL-RC-W1165              PIC S9(4) COMP-5 VALUE 1165.
      * Division by zero
       77  SQL-RC-W1166              PIC S9(4) COMP-5 VALUE 1166.
      * Arithmetic overflow or arithmetic exception
       77  SQL-RC-W1167              PIC S9(4) COMP-5 VALUE 1167.
      * object may require invoker to have privileges on data source object
       77  SQL-RC-W1179              PIC S9(4) COMP-5 VALUE 1179.
      * DLM down during backup
       77  SQL-RC-W1196              PIC S9(4) COMP-5 VALUE 1196.

      * XA no data to return from recover
       77  SQL-RC-W1251              PIC S9(4) COMP-5 VALUE 1251.
      * Parameter not changed dynamically
       77  SQL-RC-W1362              PIC S9(4) COMP-5 VALUE 1362.
      * Parameter not changed dynamically
       77  SQL-RC-W1363              PIC S9(4) COMP-5 VALUE 1363.
      * Parameter does not support AUTOMATIC
       77  SQL-RC-W1364              PIC S9(4) COMP-5 VALUE 1364.

      * WITH GRANT OPTION ignored
       77  SQL-RC-W1440              PIC S9(4) COMP-5 VALUE 1440.
      * system error on DB termination
       77  SQL-RC-ERR-DB-TERM        PIC S9(4) COMP-5 VALUE 1475.
      * Invalid cursor position
       77  SQL-RC-W1479              PIC S9(4) COMP-5 VALUE 1479.

      * value of DEGREE ignored
       77  SQL-RC-W1530              PIC S9(4) COMP-5 VALUE 1530.
      * Trailing blanks are truncated
       77  SQL-RC-W1580              PIC S9(4) COMP-5 VALUE 1580.
      * data remains unverified
       77  SQL-RC-W1594              PIC S9(4) COMP-5 VALUE 1594.

      * Event monitor state not changed
       77  SQLM-RC-NOT-SET           PIC S9(4) COMP-5 VALUE 1615.
      * Monitor: No conversion from source cp to target cp
       77  SQL-RC-W1625              PIC S9(4) COMP-5 VALUE 1625.
      * Monitor: code page conversion overflow
       77  SQL-RC-W1626              PIC S9(4) COMP-5 VALUE 1626.
      * Monitor: V5 snapshot returned on V6 request
       77  SQL-RC-W1627              PIC S9(4) COMP-5 VALUE 1627.
      * Monitor: A remote snapshot operation failed
       77  SQL-RC-W1628              PIC S9(4) COMP-5 VALUE 1628.
      * Monitor: A get switches operation ran out of buffer space
       77  SQL-RC-W1629              PIC S9(4) COMP-5 VALUE 1629.

      * containers not designated for specific nodes are not used
       77  SQL-RC-W1758              PIC S9(4) COMP-5 VALUE 1758.
      * Redistribute required to change data partitioning
       77  SQL-RC-W1759              PIC S9(4) COMP-5 VALUE 1759.
      * Index creation, recreation, or reorganization may not be recovered
      * on secondary database server 
       77  SQL-RC-W1765              PIC S9(4) COMP-5 VALUE 1765.
      * LOGINDEXBUILD has not been enabled 
       77  SQL-RC-W1766              PIC S9(4) COMP-5 VALUE 1766.
      * No default primary table space exists
       77  SQL-RC-W1790              PIC S9(4) COMP-5 VALUE 1790.
      * The stats for the specified nicknames were not updated
       77  SQL-RC-W1792              PIC S9(4) COMP-5 VALUE 1792.

      * LOB value retrieved may have been changed
       77  SQL-RC-W1821              PIC S9(4) COMP-5 VALUE 1821.
      * base tables of UNION ALL may be same table
       77  SQL-RC-W1824              PIC S9(4) COMP-5 VALUE 1824.
      * Data for a column was truncated
       77  SQL-RC-W1844              PIC S9(4) COMP-5 VALUE 1844.

      * Reconcile pending on some DLMs
       77  SQL-RC-W2077              PIC S9(4) COMP-5 VALUE 2077.

      * Statistics are in an inconsistent state
       77  SQL-RC-W2314              PIC S9(4) COMP-5 VALUE 2314.
      * The statistics profile has exceed the max size
       77  SQL-RC-W2316              PIC S9(4) COMP-5 VALUE 2316.

      * The database was not catalogued in LDAP
       77  SQL-RC-W3274              PIC S9(4) COMP-5 VALUE 3274.
      * The database was not uncatalogued in LDAP
       77  SQL-RC-W3275              PIC S9(4) COMP-5 VALUE 3275.
      * Protocol info was not updated in LDAP
       77  SQL-RC-W3283              PIC S9(4) COMP-5 VALUE 3283.

      * SQL procedure binary not in catalog
       77  SQL-RC-W7035              PIC S9(4) COMP-5 VALUE 7035.

      * evaluation period started
       77  SQL-RC-W8006              PIC S9(4) COMP-5 VALUE 8006.
      * db2 is in evaluation period
       77  SQL-RC-W8007              PIC S9(4) COMP-5 VALUE 8007.
      * concurrent user limit exceeded - DB2 Workgroup
       77  SQL-RC-W8009              PIC S9(4) COMP-5 VALUE 8009.
      * concurrent user limit exceeded - DB2 Connect
       77  SQL-RC-W8010              PIC S9(4) COMP-5 VALUE 8010.
      * concurrent user limit exceeded - DB2 Enterprise
       77  SQL-RC-W8012              PIC S9(4) COMP-5 VALUE 8012.
      * concurrent connection limit exceeded - DB2 Connect
       77  SQL-RC-W8013              PIC S9(4) COMP-5 VALUE 8013.
      * processor license limit exceeded
       77  SQL-RC-W8017              PIC S9(4) COMP-5 VALUE 8017.
      * concurrent user license limit exceeded
       77  SQL-RC-W8018              PIC S9(4) COMP-5 VALUE 8018.
      * concurrent connectors limit exceeded
       77  SQL-RC-W8020              PIC S9(4) COMP-5 VALUE 8020.
      * this connector is not registered
       77  SQL-RC-W8021              PIC S9(4) COMP-5 VALUE 8021.

      * transform for type not defined
       77  SQL-RC-W20015             PIC S9(9) COMP-5 VALUE 20015.
      * summary table cannot be used to optimize query
       77  SQL-RC-W20059             PIC S9(9) COMP-5 VALUE 20059.
      * DATALINK attribute only for typed table or type view
       77  SQL-RC-W20090             PIC S9(9) COMP-5 VALUE 20090.

      * Error in debugger support
       77  SQL-RC-W20109             PIC S9(9) COMP-5 VALUE 20109.
      * Column is not long enough
       77  SQL-RC-W20114             PIC S9(9) COMP-5 VALUE 20114.
      * ABBREVIATE column attribute ignored
       77  SQL-RC-W20140             PIC S9(9) COMP-5 VALUE 20140.
      * buffer pool configuration complete
       77  SQL-RC-W20149             PIC S9(9) COMP-5 VALUE 20149.
      * Event Monitor activated; info may be lost
       77  SQL-RC-W20156             PIC S9(9) COMP-5 VALUE 20156.
      * Isolation clause is ignored.
       77  SQL-RC-W20159             PIC S9(9) COMP-5 VALUE 20159.
      * Authorization granted to USER
       77  SQL-RC-W20160             PIC S9(9) COMP-5 VALUE 20160.
      * Column name is invalid for event monitor table
       77  SQL-RC-W20161             PIC S9(9) COMP-5 VALUE 20161.
      * The buffer pool is not started.
       77  SQL-RC-W20169             PIC S9(9) COMP-5 VALUE 20169.
      * Event monitor created; target tables already exists
       77  SQL-RC-W20173             PIC S9(9) COMP-5 VALUE 20173.
      * Insufficient memory; bufferpool operation will not take effect.
       77  SQL-RC-W20189             PIC S9(9) COMP-5 VALUE 20189.

      * procedure returned too many results
       77  SQL-RC-W20206             PIC S9(9) COMP-5 VALUE 20206.
      * The buffer pool drop operation will not take effect immediately
       77  SQL-RC-W20225             PIC S9(9) COMP-5 VALUE 20225.
      * The name was truncated
       77  SQL-RC-W20271             PIC S9(9) COMP-5 VALUE 20271.
      * Some nickname stats cannot be updated
       77  SQL-RC-W20274             PIC S9(9) COMP-5 VALUE 20274.
      * Characters truncated during code page conversion
       77  SQL-RC-W20277             PIC S9(9) COMP-5 VALUE 20277.
      * View cannot be used to optimize query processing
       77  SQL-RC-W20278             PIC S9(9) COMP-5 VALUE 20278.
      * The specified cached statement is different that the current
      * environment
       77  SQL-RC-W20287             PIC S9(9) COMP-5 VALUE 20287.
      * Insufficient permission to create or write to file
       77  SQL-RC-W20280             PIC S9(9) COMP-5 VALUE 20280.

      * Rebind options ignored
       77  SQL-RC-W30101             PIC S9(9) COMP-5 VALUE 30101.
      * Connection disabled
       77  SQL-RC-W30102             PIC S9(9) COMP-5 VALUE 30102.

      * ------------ errors ------------
      * illegal character
       77  SQL-RC-E007               PIC S9(4) COMP-5 VALUE -7.
      * string constant not terminated
       77  SQL-RC-E010               PIC S9(4) COMP-5 VALUE -10.
      * cursor or statement name = ""
       77  SQL-RC-E013               PIC S9(4) COMP-5 VALUE -13.
      * function or method does not end with RETURN
       77  SQL-RC-E017               PIC S9(4) COMP-5 VALUE -17.
      * INTO clause required
       77  SQL-RC-E029               PIC S9(4) COMP-5 VALUE -29.
      * maximum SQL statements
       77  SQL-RC-E051               PIC S9(4) COMP-5 VALUE -51.
      * SQLSTATE and SQLCODE declaration in wrong scope
       77  SQL-RC-E056               PIC S9(4) COMP-5 VALUE -56.
      * RETURN statement must include a value
       77  SQL-RC-E057               PIC S9(4) COMP-5 VALUE -57.
      * data type for RETURN value must be INTEGER
       77  SQL-RC-E058               PIC S9(4) COMP-5 VALUE -58.
      * parameter names not specified for routine
       77  SQL-RC-E078               PIC S9(4) COMP-5 VALUE -78.
      * global temp table schema must be SESSION
       77  SQL-RC-E079               PIC S9(4) COMP-5 VALUE -79.
      * bad SQL statement
       77  SQL-RC-E084               PIC S9(4) COMP-5 VALUE -84.
      * data type not supported in SQL routine
       77  SQL-RC-E097               PIC S9(4) COMP-5 VALUE -97.

      * statement too long
       77  SQL-RC-E101               PIC S9(4) COMP-5 VALUE -101.
      * string constant too long
       77  SQL-RC-E102               PIC S9(4) COMP-5 VALUE -102.
      * invalid numeric literal
       77  SQL-RC-E103               PIC S9(4) COMP-5 VALUE -103.
      * invalid character/token
       77  SQL-RC-E104               PIC S9(4) COMP-5 VALUE -104.
      * invalid string constant
       77  SQL-RC-E105               PIC S9(4) COMP-5 VALUE -105.
      * incomplete statement
       77  SQL-RC-E106               PIC S9(4) COMP-5 VALUE -106.
      * name too long
       77  SQL-RC-E107               PIC S9(4) COMP-5 VALUE -107.
      * name has too many qualifiers
       77  SQL-RC-E108               PIC S9(4) COMP-5 VALUE -108.
      * clause not permitted
       77  SQL-RC-E109               PIC S9(4) COMP-5 VALUE -109.
      * invalid hex constant
       77  SQL-RC-E110               PIC S9(4) COMP-5 VALUE -110.
      * no column name
       77  SQL-RC-E111               PIC S9(4) COMP-5 VALUE -111.
      * operand is SQL function
       77  SQL-RC-E112               PIC S9(4) COMP-5 VALUE -112.
      * identifier contains invalid character
       77  SQL-RC-E113               PIC S9(4) COMP-5 VALUE -113.
      * wrong nbr of insert values
       77  SQL-RC-E117               PIC S9(4) COMP-5 VALUE -117.
      * object table in from clause
       77  SQL-RC-E118               PIC S9(4) COMP-5 VALUE -118.
      * column not in group by
       77  SQL-RC-E119               PIC S9(4) COMP-5 VALUE -119.
      * clause includes SQL fn
       77  SQL-RC-E120               PIC S9(4) COMP-5 VALUE -120.
      * dup column name
       77  SQL-RC-E121               PIC S9(4) COMP-5 VALUE -121.
      * no group by
       77  SQL-RC-E122               PIC S9(4) COMP-5 VALUE -122.
      * parameter must be constant
       77  SQL-RC-NOTCONST           PIC S9(4) COMP-5 VALUE -123.
      * no result column
       77  SQL-RC-E125               PIC S9(4) COMP-5 VALUE -125.
      * dup distinct
       77  SQL-RC-E127               PIC S9(4) COMP-5 VALUE -127.
      * too many table names
       77  SQL-RC-E129               PIC S9(4) COMP-5 VALUE -129.
      * invalid escape character
       77  SQL-RC-INVESC             PIC S9(4) COMP-5 VALUE -130.
      * incompatible data for like
       77  SQL-RC-E131               PIC S9(4) COMP-5 VALUE -131.
      * like predicate invalid
       77  SQL-RC-E132               PIC S9(4) COMP-5 VALUE -132.
      * improper use of long string
       77  SQL-RC-E134               PIC S9(4) COMP-5 VALUE -134.
      * input must be a host variable or NULL
       77  SQL-RC-E135               PIC S9(4) COMP-5 VALUE -135.
      * length of concat too long
       77  SQL-RC-E137               PIC S9(4) COMP-5 VALUE -137.
      * substr arg out of range
       77  SQL-RC-E138               PIC S9(4) COMP-5 VALUE -138.
      * syntax not supported
       77  SQL-RC-E142               PIC S9(4) COMP-5 VALUE -142.
      * unsupported ANSI syntax
       77  SQL-RC-E146               PIC S9(4) COMP-5 VALUE -146.
      * view not updatable
       77  SQL-RC-E150               PIC S9(4) COMP-5 VALUE -150.
      * column not updatable
       77  SQL-RC-E151               PIC S9(4) COMP-5 VALUE -151.
      * actual constraint type is not expected constraint type
       77  SQL-RC-E152               PIC S9(4) COMP-5 VALUE -152.
      * no column list
       77  SQL-RC-E153               PIC S9(4) COMP-5 VALUE -153.
      * trigger trans tbl not modifiable
       77  SQL-RC-E155               PIC S9(4) COMP-5 VALUE -155.
      * command not allowed on view
       77  SQL-RC-E156               PIC S9(4) COMP-5 VALUE -156.
      * view name in foreign key
       77  SQL-RC-E157               PIC S9(4) COMP-5 VALUE -157.
      * nbr of columns does not match
       77  SQL-RC-E158               PIC S9(4) COMP-5 VALUE -158.
      * drop view on table
       77  SQL-RC-E159               PIC S9(4) COMP-5 VALUE -159.
      * with check not allowed on view
       77  SQL-RC-E160               PIC S9(4) COMP-5 VALUE -160.
      * with check violation
       77  SQL-RC-E161               PIC S9(4) COMP-5 VALUE -161.
      * nbr of arguments invalid
       77  SQL-RC-E170               PIC S9(4) COMP-5 VALUE -170.
      * argument invalid
       77  SQL-RC-E171               PIC S9(4) COMP-5 VALUE -171.
      * function name invalid
       77  SQL-RC-E172               PIC S9(4) COMP-5 VALUE -172.
      * translate scalar argument invalid
       77  SQL-RC-E176               PIC S9(4) COMP-5 VALUE -176.
      * datetime syntax invalid
       77  SQL-RC-E180               PIC S9(4) COMP-5 VALUE -180.
      * datetime value invalid
       77  SQL-RC-E181               PIC S9(4) COMP-5 VALUE -181.
      * datetime arithmetic invalid
       77  SQL-RC-E182               PIC S9(4) COMP-5 VALUE -182.
      * datetime arithmetic out of range
       77  SQL-RC-E183               PIC S9(4) COMP-5 VALUE -183.
      * datetime register is invalid
       77  SQL-RC-DTMEREG            PIC S9(4) COMP-5 VALUE -187.
      * data type or length of column incompatible
       77  SQL-RC-E190               PIC S9(4) COMP-5 VALUE -190.
      * truncated DBCS character found
       77  SQL-RC-DBCSTRUNC          PIC S9(4) COMP-5 VALUE -191.
      * NOT NULL needs DEFAULT
       77  SQL-RC-E193               PIC S9(4) COMP-5 VALUE -193.
      * no qualified columns in ORDER BY
       77  SQL-RC-E197               PIC S9(4) COMP-5 VALUE -197.
      * no statement text
       77  SQL-RC-E198               PIC S9(4) COMP-5 VALUE -198.
      * illegal use of reserved word
       77  SQL-RC-E199               PIC S9(4) COMP-5 VALUE -199.

      * ambiguous column reference
       77  SQL-RC-E203               PIC S9(4) COMP-5 VALUE -203.
      * undefined name
       77  SQL-RC-E204               PIC S9(4) COMP-5 VALUE -204.
      * not a column
       77  SQL-RC-E205               PIC S9(4) COMP-5 VALUE -205.
      * not a column of referenced tables
       77  SQL-RC-E206               PIC S9(4) COMP-5 VALUE -206.
      * cannot orderby column name w/union
       77  SQL-RC-E207               PIC S9(4) COMP-5 VALUE -207.
      * column not part of result table
       77  SQL-RC-E208               PIC S9(4) COMP-5 VALUE -208.
      * duplicate table designator
       77  SQL-RC-E212               PIC S9(4) COMP-5 VALUE -212.
      * bad expression in group/by or order by
       77  SQL-RC-E214               PIC S9(4) COMP-5 VALUE -214.
      * number of elements does not match
       77  SQL-RC-E216               PIC S9(4) COMP-5 VALUE -216.
      * required explain table not exist
       77  SQL-RC-E219               PIC S9(4) COMP-5 VALUE -219.
      * explain table has improper def
       77  SQL-RC-E220               PIC S9(4) COMP-5 VALUE -220.
      * update or delete against a hole
       77  SQL-RC-E222               PIC S9(4) COMP-5 VALUE -222.
      * result table does not agree with base table
       77  SQL-RC-E224               PIC S9(4) COMP-5 VALUE -224.
      * FETCH against non-scrollable cursor
       77  SQL-RC-E225               PIC S9(4) COMP-5 VALUE -225.
      * cursor has unknown position
       77  SQL-RC-E227               PIC S9(4) COMP-5 VALUE -227.
      * FOR UPDATE specified for read-only cursor
       77  SQL-RC-E228               PIC S9(4) COMP-5 VALUE -228.
      * duplicate object in list
       77  SQL-RC-E242               PIC S9(4) COMP-5 VALUE -242.
      * SENSITIVE cursor cannot be defined for SELECT
       77  SQL-RC-E243               PIC S9(4) COMP-5 VALUE -243.
      * SENSITIVITY not valid for cursor
       77  SQL-RC-E244               PIC S9(4) COMP-5 VALUE -244.
      * raw device containers not allowed
       77  SQL-RC-E257               PIC S9(4) COMP-5 VALUE -257.
      * cannot add container to pool
       77  SQL-RC-E258               PIC S9(4) COMP-5 VALUE -258.
      * container map too big
       77  SQL-RC-E259               PIC S9(4) COMP-5 VALUE -259.
      * partition key has long field
       77  SQL-RC-E260               PIC S9(4) COMP-5 VALUE -260.
      * node in use, cannot drop
       77  SQL-RC-E261               PIC S9(4) COMP-5 VALUE -261.
      * multinode table without part key
       77  SQL-RC-E262               PIC S9(4) COMP-5 VALUE -262.
      * invalid node range
       77  SQL-RC-E263               PIC S9(4) COMP-5 VALUE -263.
      * multinode tbl, cannot drop part
       77  SQL-RC-E264               PIC S9(4) COMP-5 VALUE -264.
      * duplicate node name/number
       77  SQL-RC-E265               PIC S9(4) COMP-5 VALUE -265.
      * node not defined
       77  SQL-RC-E266               PIC S9(4) COMP-5 VALUE -266.
      * operatn not done because rebalance
       77  SQL-RC-E268               PIC S9(4) COMP-5 VALUE -268.
      * too many nodegroups
       77  SQL-RC-E269               PIC S9(4) COMP-5 VALUE -269.
      * function not supported
       77  SQL-RC-E270               PIC S9(4) COMP-5 VALUE -270.
      * Index file missing or invalid
       77  SQL-RC-E271               PIC S9(4) COMP-5 VALUE -271.
      * database in restore pending state
       77  SQL-RC-E276               PIC S9(4) COMP-5 VALUE -276.
      * connection terminated during COMMIT
       77  SQL-RC-E279               PIC S9(4) COMP-5 VALUE -279.
      * cannot add containers to tablespace
       77  SQL-RC-E281               PIC S9(4) COMP-5 VALUE -281.
      * cannot drop tblspace, tbl conflict
       77  SQL-RC-E282               PIC S9(4) COMP-5 VALUE -282.
      * cannot drop only temp tablespace
       77  SQL-RC-E283               PIC S9(4) COMP-5 VALUE -283.
      * invalid tablespace type for clause
       77  SQL-RC-E284               PIC S9(4) COMP-5 VALUE -284.
      * all table parts must be in tblspce
       77  SQL-RC-E285               PIC S9(4) COMP-5 VALUE -285.
      * need default tblspce for new tables
       77  SQL-RC-E286               PIC S9(4) COMP-5 VALUE -286.
      * SYSCATSPACE not for user objects
       77  SQL-RC-E287               PIC S9(4) COMP-5 VALUE -287.
      * long tablespace cannot use system
       77  SQL-RC-E288               PIC S9(4) COMP-5 VALUE -288.
      * cannot allocate new pages in tablespace
       77  SQL-RC-E289               PIC S9(4) COMP-5 VALUE -289.
      * access to tablespace not allowed
       77  SQL-RC-E290               PIC S9(4) COMP-5 VALUE -290.
      * invalid state transition
       77  SQL-RC-E291               PIC S9(4) COMP-5 VALUE -291.
      * cannot create internal db file
       77  SQL-RC-E292               PIC S9(4) COMP-5 VALUE -292.
      * error accessing container
       77  SQL-RC-E293               PIC S9(4) COMP-5 VALUE -293.
      * container already in use
       77  SQL-RC-E294               PIC S9(4) COMP-5 VALUE -294.
      * container names too long
       77  SQL-RC-E295               PIC S9(4) COMP-5 VALUE -295.
      * tablespace limit exceeded
       77  SQL-RC-E296               PIC S9(4) COMP-5 VALUE -296.
      * container pathname too long
       77  SQL-RC-E297               PIC S9(4) COMP-5 VALUE -297.
      * bad container pathname
       77  SQL-RC-E298               PIC S9(4) COMP-5 VALUE -298.
      * container already added
       77  SQL-RC-E299               PIC S9(4) COMP-5 VALUE -299.

      * host variable has invalid type
       77  SQL-RC-E301               PIC S9(4) COMP-5 VALUE -301.
      * host variable value too large
       77  SQL-RC-E302               PIC S9(4) COMP-5 VALUE -302.
      * data types not comparable
       77  SQL-RC-E303               PIC S9(4) COMP-5 VALUE -303.
      * value not in range of host var
       77  SQL-RC-E304               PIC S9(4) COMP-5 VALUE -304.
      * host var cannot be null
       77  SQL-RC-E305               PIC S9(4) COMP-5 VALUE -305.
      * host var limit reached
       77  SQL-RC-E308               PIC S9(4) COMP-5 VALUE -308.
      * host var should not be null
       77  SQL-RC-E309               PIC S9(4) COMP-5 VALUE -309.
      * length of host var is negative
       77  SQL-RC-E311               PIC S9(4) COMP-5 VALUE -311.
      * unusable host variable
       77  SQL-RC-E312               PIC S9(4) COMP-5 VALUE -312.
      * wrong nbr of host variables
       77  SQL-RC-E313               PIC S9(4) COMP-5 VALUE -313.
      * no conversn source-cp to target-cp
       77  SQL-RC-E332               PIC S9(4) COMP-5 VALUE -332.
      * conversion overflow
       77  SQL-RC-E334               PIC S9(4) COMP-5 VALUE -334.
      * decimal scale must be zero
       77  SQL-RC-E336               PIC S9(4) COMP-5 VALUE -336.
      * ON clause not valid for Outer Join
       77  SQL-RC-E338               PIC S9(4) COMP-5 VALUE -338.
      * duplicate common table expression
       77  SQL-RC-E340               PIC S9(4) COMP-5 VALUE -340.
      * cyclic ref between comm tbl exp
       77  SQL-RC-E341               PIC S9(4) COMP-5 VALUE -341.
      * comm tbl exp use UNION ALL
       77  SQL-RC-E342               PIC S9(4) COMP-5 VALUE -342.
      * col names required
       77  SQL-RC-E343               PIC S9(4) COMP-5 VALUE -343.
      * columns must match exactly
       77  SQL-RC-E344               PIC S9(4) COMP-5 VALUE -344.
      * cannot use GROUP BY or HAVING here
       77  SQL-RC-E345               PIC S9(4) COMP-5 VALUE -345.
      * invalid reference to comm tbl exp
       77  SQL-RC-E346               PIC S9(4) COMP-5 VALUE -346.
      * cannot use identity column or sequence
       77  SQL-RC-E348               PIC S9(4) COMP-5 VALUE -348.
      * different sequence expressions in INSERT
       77  SQL-RC-E349               PIC S9(4) COMP-5 VALUE -349.
      * LOB col cannot be idx, key, constr
       77  SQL-RC-E350               PIC S9(4) COMP-5 VALUE -350.
      * LOB col cannot be selected by DRDA
       77  SQL-RC-E351               PIC S9(4) COMP-5 VALUE -351.
      * LOB col cannot be inserted by DRDA
       77  SQL-RC-E352               PIC S9(4) COMP-5 VALUE -352.
      * LOB col cannot be logged
       77  SQL-RC-E355               PIC S9(4) COMP-5 VALUE -355.
      * DataLink Error
       77  SQL-RC-E357               PIC S9(4) COMP-5 VALUE -357.
      * DataLink Error
       77  SQL-RC-E358               PIC S9(4) COMP-5 VALUE -358.
      * Range exhausted
       77  SQL-RC-E359               PIC S9(4) COMP-5 VALUE -359.
      * DataLink Error
       77  SQL-RC-E368               PIC S9(4) COMP-5 VALUE -368.
      * parameter must be named in CREATE FUNCTION statement
       77  SQL-RC-E370               PIC S9(4) COMP-5 VALUE -370.
      * One identity column per table
       77  SQL-RC-E372               PIC S9(4) COMP-5 VALUE -372.
      * DEFAULT clause not allowed on identity column
       77  SQL-RC-E373               PIC S9(4) COMP-5 VALUE -373.
      * clause not specified in CREATE FUNCTION
       77  SQL-RC-E374               PIC S9(4) COMP-5 VALUE -374.
      * source and target are built-in types or same
       77  SQL-RC-E388               PIC S9(4) COMP-5 VALUE -388.
      * invalid specific function instance in CREATE CAST
       77  SQL-RC-E389               PIC S9(4) COMP-5 VALUE -389.
      * function is invalid in the context it is used
       77  SQL-RC-E390               PIC S9(4) COMP-5 VALUE -390.
      * Invalid use of a row based function
       77  SQL-RC-E391               PIC S9(4) COMP-5 VALUE -391.
      * SQLDA provided for fetch has been changed
       77  SQL-RC-E392               PIC S9(4) COMP-5 VALUE -392.
      * Execute SQL statement during final call processing
       77  SQL-RC-E396               PIC S9(4) COMP-5 VALUE -396.

      * operands not comparable
       77  SQL-RC-E401               PIC S9(4) COMP-5 VALUE -401.
      * invalid type for arithmetic op
       77  SQL-RC-E402               PIC S9(4) COMP-5 VALUE -402.
      * alias object undefined
       77  SQL-RC-E403               PIC S9(4) COMP-5 VALUE -403.
      * update/insert string too long
       77  SQL-RC-E404               PIC S9(4) COMP-5 VALUE -404.
      * numeric literal out of range
       77  SQL-RC-E405               PIC S9(4) COMP-5 VALUE -405.
      * derived value out of range
       77  SQL-RC-E406               PIC S9(4) COMP-5 VALUE -406.
      * column cannot be null
       77  SQL-RC-E407               PIC S9(4) COMP-5 VALUE -407.
      * invalid data type for column
       77  SQL-RC-E408               PIC S9(4) COMP-5 VALUE -408.
      * invalid operand for count
       77  SQL-RC-E409               PIC S9(4) COMP-5 VALUE -409.
      * float literal too long
       77  SQL-RC-E410               PIC S9(4) COMP-5 VALUE -410.
      * multiple columns in subquery
       77  SQL-RC-E412               PIC S9(4) COMP-5 VALUE -412.
      * overflow during data conversion
       77  SQL-RC-E413               PIC S9(4) COMP-5 VALUE -413.
      * incompatible data types for union
       77  SQL-RC-E415               PIC S9(4) COMP-5 VALUE -415.
      * long string in union
       77  SQL-RC-E416               PIC S9(4) COMP-5 VALUE -416.
      * invalid use of parameter markers
       77  SQL-RC-E417               PIC S9(4) COMP-5 VALUE -417.
      * parameter marker in select clause
       77  SQL-RC-E418               PIC S9(4) COMP-5 VALUE -418.
      * division produced negative scale
       77  SQL-RC-E419               PIC S9(4) COMP-5 VALUE -419.
      * invalid character in input string
       77  SQL-RC-E420               PIC S9(4) COMP-5 VALUE -420.
      * diff number of columns for union
       77  SQL-RC-E421               PIC S9(4) COMP-5 VALUE -421.
      * Invalid handle
       77  SQL-RC-E423               PIC S9(4) COMP-5 VALUE -423.
      * dynamic COMMIT not valid
       77  SQL-RC-E426               PIC S9(4) COMP-5 VALUE -426.
      * dynamic ROLLBACK not valid
       77  SQL-RC-E427               PIC S9(4) COMP-5 VALUE -427.
      * DISCONNECT in unit of work notallow
       77  SQL-RC-E428               PIC S9(4) COMP-5 VALUE -428.
      * Handle table full
       77  SQL-RC-E429               PIC S9(4) COMP-5 VALUE -429.
      * UDF abnormal end
       77  SQL-RC-E430               PIC S9(4) COMP-5 VALUE -430.
      * UDF interrupted
       77  SQL-RC-E431               PIC S9(4) COMP-5 VALUE -431.
      * Parameter marker cannot have udf nm
       77  SQL-RC-E432               PIC S9(4) COMP-5 VALUE -432.
      * Value is too long
       77  SQL-RC-E433               PIC S9(4) COMP-5 VALUE -433.
      * Inv SQLSTATE in RAISE_ERROR
       77  SQL-RC-E435               PIC S9(4) COMP-5 VALUE -435.
      * C language char string missing NUL
       77  SQL-RC-E436               PIC S9(4) COMP-5 VALUE -436.
      * App raised error
       77  SQL-RC-E438               PIC S9(4) COMP-5 VALUE -438.
      * Error in UDF
       77  SQL-RC-E439               PIC S9(4) COMP-5 VALUE -439.
      * No function with compatible arg
       77  SQL-RC-E440               PIC S9(4) COMP-5 VALUE -440.
      * Invalid use of DISTINCT with scalar
       77  SQL-RC-E441               PIC S9(4) COMP-5 VALUE -441.
      * Error refering function in DML
       77  SQL-RC-E442               PIC S9(4) COMP-5 VALUE -442.
      * UDF returns error sqlstate
       77  SQL-RC-E443               PIC S9(4) COMP-5 VALUE -443.
      * Unable to access UDF function
       77  SQL-RC-E444               PIC S9(4) COMP-5 VALUE -444.
      * Exceed max number of parameters
       77  SQL-RC-E448               PIC S9(4) COMP-5 VALUE -448.
      * Invalid EXTERNAL NAME format
       77  SQL-RC-E449               PIC S9(4) COMP-5 VALUE -449.
      * UDF generates too long result value
       77  SQL-RC-E450               PIC S9(4) COMP-5 VALUE -450.
      * DDL: invalid type for external UDF
       77  SQL-RC-E451               PIC S9(4) COMP-5 VALUE -451.
      * Hostvar file inaccessible
       77  SQL-RC-E452               PIC S9(4) COMP-5 VALUE -452.
      * Error in RETURNS and CAST FROM
       77  SQL-RC-E453               PIC S9(4) COMP-5 VALUE -453.
      * Duplicate UDF name and signature
       77  SQL-RC-E454               PIC S9(4) COMP-5 VALUE -454.
      * Diff schema for UDF name & specific
       77  SQL-RC-E455               PIC S9(4) COMP-5 VALUE -455.
      * Duplicate specific name
       77  SQL-RC-E456               PIC S9(4) COMP-5 VALUE -456.
      * Name reserved for system use
       77  SQL-RC-E457               PIC S9(4) COMP-5 VALUE -457.
      * DDL refers UDF signature not found
       77  SQL-RC-E458               PIC S9(4) COMP-5 VALUE -458.
      * AS CAST use error
       77  SQL-RC-E459               PIC S9(4) COMP-5 VALUE -459.
      * invalid CAST
       77  SQL-RC-E461               PIC S9(4) COMP-5 VALUE -461.
      * UDF returns an invalid sqlstate
       77  SQL-RC-E463               PIC S9(4) COMP-5 VALUE -463.
      * Unable to start fenced UDF
       77  SQL-RC-E465               PIC S9(4) COMP-5 VALUE -465.
      * Invalid use of IN, OUT, or INOUT parameter
       77  SQL-RC-E469               PIC S9(4) COMP-5 VALUE -469.
      * Could not pass NULL argument to procedure
       77  SQL-RC-E470               PIC S9(4) COMP-5 VALUE -470.
      * Error occurred calling a routine
       77  SQL-RC-E471               PIC S9(4) COMP-5 VALUE -471.
      * Cursor left open by function or method
       77  SQL-RC-E472               PIC S9(4) COMP-5 VALUE -472.
      * reserved object name
       77  SQL-RC-E473               PIC S9(4) COMP-5 VALUE -473.
      * Result type of source is different
       77  SQL-RC-E475               PIC S9(4) COMP-5 VALUE -475.
      * Reference to function is not unique
       77  SQL-RC-E476               PIC S9(4) COMP-5 VALUE -476.
      * other obj depends on this obj
       77  SQL-RC-E478               PIC S9(4) COMP-5 VALUE -478.
      * procedure not yet called
       77  SQL-RC-E480               PIC S9(4) COMP-5 VALUE -480.
      * GROUP BY: element nested in element
       77  SQL-RC-E481               PIC S9(4) COMP-5 VALUE -481.
      * Parm num not match with source
       77  SQL-RC-E483               PIC S9(4) COMP-5 VALUE -483.
      * Boolean type is system used only
       77  SQL-RC-E486               PIC S9(4) COMP-5 VALUE -486.
      * Attempt to excute an SQL statement
       77  SQL-RC-E487               PIC S9(4) COMP-5 VALUE -487.
      * Inv BOOLEAN in select list result
       77  SQL-RC-E489               PIC S9(4) COMP-5 VALUE -489.
      * Number outside the range of allowable values
       77  SQL-RC-E490               PIC S9(4) COMP-5 VALUE -490.
      * DDL: missing reqired clause
       77  SQL-RC-E491               PIC S9(4) COMP-5 VALUE -491.
      * DDL: miss match with source type
       77  SQL-RC-E492               PIC S9(4) COMP-5 VALUE -492.
      * UDF returned bad date/time value
       77  SQL-RC-E493               PIC S9(4) COMP-5 VALUE -493.
      * estimated cost exceeds resource limit error threshold
       77  SQL-RC-E495               PIC S9(4) COMP-5 VALUE -495.
      * cursor already assigned to a result set
       77  SQL-RC-E499               PIC S9(4) COMP-5 VALUE -499.

      * cursor not open (fetch/close)
       77  SQL-RC-E501               PIC S9(4) COMP-5 VALUE -501.
      * cursor already open
       77  SQL-RC-E502               PIC S9(4) COMP-5 VALUE -502.
      * column not in update clause
       77  SQL-RC-E503               PIC S9(4) COMP-5 VALUE -503.
      * cursor name not defined
       77  SQL-RC-E504               PIC S9(4) COMP-5 VALUE -504.
      * cursor name already declared
       77  SQL-RC-E505               PIC S9(4) COMP-5 VALUE -505.
      * cursor not open (update/delete)
       77  SQL-RC-E507               PIC S9(4) COMP-5 VALUE -507.
      * cursor not on a row
       77  SQL-RC-E508               PIC S9(4) COMP-5 VALUE -508.
      * table not same as for cursor
       77  SQL-RC-E509               PIC S9(4) COMP-5 VALUE -509.
      * table cannot be modified
       77  SQL-RC-E510               PIC S9(4) COMP-5 VALUE -510.
      * for update not allowed
       77  SQL-RC-E511               PIC S9(4) COMP-5 VALUE -511.
      * cursor not prepared
       77  SQL-RC-E514               PIC S9(4) COMP-5 VALUE -514.
      * describe not a prepared statement
       77  SQL-RC-E516               PIC S9(4) COMP-5 VALUE -516.
      * prepared statment not a select
       77  SQL-RC-E517               PIC S9(4) COMP-5 VALUE -517.
      * execute not a prepared statement
       77  SQL-RC-E518               PIC S9(4) COMP-5 VALUE -518.
      * statement has an open cursor
       77  SQL-RC-E519               PIC S9(4) COMP-5 VALUE -519.
      * statement cannot be executed
       77  SQL-RC-E525               PIC S9(4) COMP-5 VALUE -525.
      * function does not apply to global temp tables
       77  SQL-RC-E526               PIC S9(4) COMP-5 VALUE -526.
      * duplicate primary/unique key
       77  SQL-RC-E528               PIC S9(4) COMP-5 VALUE -528.
      * invalid foreign key value
       77  SQL-RC-E530               PIC S9(4) COMP-5 VALUE -530.
      * cannot update primary key
       77  SQL-RC-E531               PIC S9(4) COMP-5 VALUE -531.
      * delete is restricted
       77  SQL-RC-E532               PIC S9(4) COMP-5 VALUE -532.
      * multi-row insert not allowed
       77  SQL-RC-E533               PIC S9(4) COMP-5 VALUE -533.
      * multi-row update of pk
       77  SQL-RC-E534               PIC S9(4) COMP-5 VALUE -534.
      * multi-row delete not allowed
       77  SQL-RC-E535               PIC S9(4) COMP-5 VALUE -535.
      * descendent in subquery
       77  SQL-RC-E536               PIC S9(4) COMP-5 VALUE -536.
      * dup column in key def
       77  SQL-RC-E537               PIC S9(4) COMP-5 VALUE -537.
      * foreign key does not match pk
       77  SQL-RC-E538               PIC S9(4) COMP-5 VALUE -538.
      * table does not have primary key
       77  SQL-RC-E539               PIC S9(4) COMP-5 VALUE -539.
      * table does not have primary index
       77  SQL-RC-E540               PIC S9(4) COMP-5 VALUE -540.
      * duplicate referential constraint
       77  SQL-RC-E541               PIC S9(4) COMP-5 VALUE -541.
      * pk column cannot allow nulls
       77  SQL-RC-E542               PIC S9(4) COMP-5 VALUE -542.
      * restricted row delete
       77  SQL-RC-E543               PIC S9(4) COMP-5 VALUE -543.
      * check contraint violated
       77  SQL-RC-E544               PIC S9(4) COMP-5 VALUE -544.
      * check contraint not satisfied
       77  SQL-RC-E545               PIC S9(4) COMP-5 VALUE -545.
      * check contraint invalid
       77  SQL-RC-E546               PIC S9(4) COMP-5 VALUE -546.
      * check contraint invalid
       77  SQL-RC-E548               PIC S9(4) COMP-5 VALUE -548.
      * restricted stmt used with DYNAMICRULES BIND pkg
       77  SQL-RC-E549               PIC S9(4) COMP-5 VALUE -549.
      * authorization error w/obj insert
       77  SQL-RC-E551               PIC S9(4) COMP-5 VALUE -551.
      * auth error w/o obj ins
       77  SQL-RC-E552               PIC S9(4) COMP-5 VALUE -552.
      * SYSIBM qualifier
       77  SQL-RC-E553               PIC S9(4) COMP-5 VALUE -553.
      * cannot grant privilege to self
       77  SQL-RC-E554               PIC S9(4) COMP-5 VALUE -554.
      * cannot revoke privilege from self
       77  SQL-RC-E555               PIC S9(4) COMP-5 VALUE -555.
      * revoke stmt denied--priv not held
       77  SQL-RC-E556               PIC S9(4) COMP-5 VALUE -556.
      * invalid combination of privileges
       77  SQL-RC-E557               PIC S9(4) COMP-5 VALUE -557.
      * revoke stmt denied--has CONTROL
       77  SQL-RC-E558               PIC S9(4) COMP-5 VALUE -558.
      * privilege not allowed for public
       77  SQL-RC-E562               PIC S9(4) COMP-5 VALUE -562.
      * invalid authorization id
       77  SQL-RC-E567               PIC S9(4) COMP-5 VALUE -567.
      * user/group ambiguity
       77  SQL-RC-E569               PIC S9(4) COMP-5 VALUE -569.
      * Inoperative package
       77  SQL-RC-E572               PIC S9(4) COMP-5 VALUE -572.
      * contraint col not primary key
       77  SQL-RC-E573               PIC S9(4) COMP-5 VALUE -573.
      * DEFAULT invalid for column
       77  SQL-RC-E574               PIC S9(4) COMP-5 VALUE -574.
      * view is inoperative
       77  SQL-RC-E575               PIC S9(4) COMP-5 VALUE -575.
      * Repetitive alias chain
       77  SQL-RC-E576               PIC S9(4) COMP-5 VALUE -576.
      * function or procedure not defined as MODIFIES SQL DATA
       77  SQL-RC-E577               PIC S9(4) COMP-5 VALUE -577.
      * function or procedure not defined as READS SQL DATA
       77  SQL-RC-E579               PIC S9(4) COMP-5 VALUE -579.
      * Result of CASE expr cannot be NULL
       77  SQL-RC-E580               PIC S9(4) COMP-5 VALUE -580.
      * Data types of CASE expr incompat
       77  SQL-RC-E581               PIC S9(4) COMP-5 VALUE -581.
      * Inv predicate in CASE expr
       77  SQL-RC-E582               PIC S9(4) COMP-5 VALUE -582.
      * Variant or ext action function
       77  SQL-RC-E583               PIC S9(4) COMP-5 VALUE -583.
      * Inv use of NULL or DEFAULT
       77  SQL-RC-E584               PIC S9(4) COMP-5 VALUE -584.
      * Duplicate schema name     
       77  SQL-RC-E585               PIC S9(4) COMP-5 VALUE -585.
      * Too many schema names     
       77  SQL-RC-E586               PIC S9(4) COMP-5 VALUE -586.
      * Name in SQL procedure not unique 
       77  SQL-RC-E590               PIC S9(4) COMP-5 VALUE -590.

      * Not authorized to update linked file
       77  SQL-RC-E597               PIC S9(4) COMP-5 VALUE -597.
      * udf - dup func signature
       77  SQL-RC-E600               PIC S9(4) COMP-5 VALUE -600.
      * duplicate table/view name
       77  SQL-RC-E601               PIC S9(4) COMP-5 VALUE -601.
      * too many columns in index
       77  SQL-RC-E602               PIC S9(4) COMP-5 VALUE -602.
      * cannot create unique index
       77  SQL-RC-E603               PIC S9(4) COMP-5 VALUE -603.
      * invalid length, precision, scale
       77  SQL-RC-E604               PIC S9(4) COMP-5 VALUE -604.
      * authid does not own column or table
       77  SQL-RC-E606               PIC S9(4) COMP-5 VALUE -606.
      * op not allowed on system tables
       77  SQL-RC-E607               PIC S9(4) COMP-5 VALUE -607.
      * duplicate column name
       77  SQL-RC-E612               PIC S9(4) COMP-5 VALUE -612.
      * primary key too long
       77  SQL-RC-E613               PIC S9(4) COMP-5 VALUE -613.
      * index key too long
       77  SQL-RC-E614               PIC S9(4) COMP-5 VALUE -614.
      * object is in use and cannot be dropped
       77  SQL-RC-E615               PIC S9(4) COMP-5 VALUE -615.
      * userid does not have appropriate dbspaces
       77  SQL-RC-E620               PIC S9(4) COMP-5 VALUE -620.
      * clause invalid for this database
       77  SQL-RC-E622               PIC S9(4) COMP-5 VALUE -622.
      * clustering index already exists for table
       77  SQL-RC-E623               PIC S9(4) COMP-5 VALUE -623.
      * table already has primary key
       77  SQL-RC-E624               PIC S9(4) COMP-5 VALUE -624.
      * DDL: multiple/conflict keywords
       77  SQL-RC-E628               PIC S9(4) COMP-5 VALUE -628.
      * foreign key is not nullable
       77  SQL-RC-E629               PIC S9(4) COMP-5 VALUE -629.
      * foreign key is too long
       77  SQL-RC-E631               PIC S9(4) COMP-5 VALUE -631.
      * delete rule restriction
       77  SQL-RC-E632               PIC S9(4) COMP-5 VALUE -632.
      * delete rule is restricted
       77  SQL-RC-E633               PIC S9(4) COMP-5 VALUE -633.
      * delete rule cannot be CASCADE
       77  SQL-RC-E634               PIC S9(4) COMP-5 VALUE -634.
      * dup pk or drop pk clause
       77  SQL-RC-E637               PIC S9(4) COMP-5 VALUE -637.
      * no column definitions
       77  SQL-RC-E638               PIC S9(4) COMP-5 VALUE -638.
      * invalid keyword value
       77  SQL-RC-E644               PIC S9(4) COMP-5 VALUE -644.
      * bufferpool not active
       77  SQL-RC-E647               PIC S9(4) COMP-5 VALUE -647.
      * invalid referential constraint
       77  SQL-RC-E648               PIC S9(4) COMP-5 VALUE -648.
      * object cannot be explicitely dropped
       77  SQL-RC-E658               PIC S9(4) COMP-5 VALUE -658.
      * architectural size limit of object
       77  SQL-RC-E659               PIC S9(4) COMP-5 VALUE -659.
      * ref constraint does not hold
       77  SQL-RC-E667               PIC S9(4) COMP-5 VALUE -667.
      * table in CHECK PENDING state
       77  SQL-RC-E668               PIC S9(4) COMP-5 VALUE -668.
      * cannot drop primary key
       77  SQL-RC-E669               PIC S9(4) COMP-5 VALUE -669.
      * row length too large
       77  SQL-RC-E670               PIC S9(4) COMP-5 VALUE -670.
      * cannot drop table
       77  SQL-RC-E672               PIC S9(4) COMP-5 VALUE -672.
      * primary key not unique
       77  SQL-RC-E673               PIC S9(4) COMP-5 VALUE -673.
      * too many columns for table
       77  SQL-RC-E680               PIC S9(4) COMP-5 VALUE -680.
      * incompatible column constraint
       77  SQL-RC-E683               PIC S9(4) COMP-5 VALUE -683.
      * invalid trigger definition
       77  SQL-RC-E696               PIC S9(4) COMP-5 VALUE -696.
      * invalid correlation name use
       77  SQL-RC-E697               PIC S9(4) COMP-5 VALUE -697.

      * object uses reserved name
       77  SQL-RC-E707               PIC S9(4) COMP-5 VALUE -707.
      * invalid replace value for sp reg
       77  SQL-RC-INV-REPL           PIC S9(4) COMP-5 VALUE -713.
      * Bind error for user; pkg already exists
       77  SQL-RC-E719               PIC S9(4) COMP-5 VALUE -719.
      * Attempt to replace existing package
       77  SQL-RC-E720               PIC S9(4) COMP-5 VALUE -720.
      * Pkg name with consistency tokens is not unique
       77  SQL-RC-E721               PIC S9(4) COMP-5 VALUE -721.
      * Bind/rebind error for uesr; pkg does not exist
       77  SQL-RC-E722               PIC S9(4) COMP-5 VALUE -722.
      * trigger error
       77  SQL-RC-E723               PIC S9(4) COMP-5 VALUE -723.
      * max level of cascading
       77  SQL-RC-E724               PIC S9(4) COMP-5 VALUE -724.
      * Invalid implicit PREP or REBIND
       77  SQL-RC-E727               PIC S9(4) COMP-5 VALUE -727.
      * MODIFIES SQL DATA option not valid in context
       77  SQL-RC-E740               PIC S9(4) COMP-5 VALUE -740.
      * Routine violates nested SQL statement rules
       77  SQL-RC-E746               PIC S9(4) COMP-5 VALUE -746.
      * Table cannot be renamed
       77  SQL-RC-E750               PIC S9(4) COMP-5 VALUE -750.
      * statement not allowed in function or proc
       77  SQL-RC-E751               PIC S9(4) COMP-5 VALUE -751.
      * Inoperative package
       77  SQL-RC-E752               PIC S9(4) COMP-5 VALUE -752.
      * Case not found for CASE statement 
       77  SQL-RC-E773               PIC S9(4) COMP-5 VALUE -773.
      * statement cannot be executed within an ATOMIC statement
       77  SQL-RC-E774               PIC S9(4) COMP-5 VALUE -774.
      * Use of cursor not valid in FOR statement
       77  SQL-RC-E776               PIC S9(4) COMP-5 VALUE -776.
      * Nested compound statements not allowed
       77  SQL-RC-E777               PIC S9(4) COMP-5 VALUE -777.
      * End label not same name as begin label
       77  SQL-RC-E778               PIC S9(4) COMP-5 VALUE -778.
      * Label in ITERATE or LEAVE is not valid
       77  SQL-RC-E779               PIC S9(4) COMP-5 VALUE -779.
      * UNDO in handler in compound statement 
       77  SQL-RC-E780               PIC S9(4) COMP-5 VALUE -780.
      * Condition in handler not defined      
       77  SQL-RC-E781               PIC S9(4) COMP-5 VALUE -781.
      * Condition or SQLSTATE in handler not valid
       77  SQL-RC-E782               PIC S9(4) COMP-5 VALUE -782.
      * SELECT list in FOR statement not valid
       77  SQL-RC-E783               PIC S9(4) COMP-5 VALUE -783.
      * use of SQLCODE or SQLSTATE is not valid
       77  SQL-RC-E785               PIC S9(4) COMP-5 VALUE -785.
      * RESIGNAL statement used outside handler
       77  SQL-RC-E787               PIC S9(4) COMP-5 VALUE -787.
      * The same table row cannot be the target
       77  SQL-RC-E788               PIC S9(4) COMP-5 VALUE -788.
      * type not supported in SQL procedures
       77  SQL-RC-E789               PIC S9(4) COMP-5 VALUE -789.
      * Invalid triggered statement
       77  SQL-RC-E797               PIC S9(4) COMP-5 VALUE -797.
      * Insert value not allowed on generated always identity column
       77  SQL-RC-E798               PIC S9(4) COMP-5 VALUE -798.

      * divide by zero
       77  SQL-RC-E801               PIC S9(4) COMP-5 VALUE -801.
      * arith overflow or divide by zero
       77  SQL-RC-E802               PIC S9(4) COMP-5 VALUE -802.
      * distinct violation
       77  SQL-RC-E803               PIC S9(4) COMP-5 VALUE -803.
      * bad input parameters
       77  SQL-RC-E804               PIC S9(4) COMP-5 VALUE -804.
      * program not bound
       77  SQL-RC-E805               PIC S9(4) COMP-5 VALUE -805.
      * inconsistent connect semantics
       77  SQL-RC-E808               PIC S9(4) COMP-5 VALUE -808.
      * more than one row/value
       77  SQL-RC-E811               PIC S9(4) COMP-5 VALUE -811.
      * statement would cause invalid update operation
       77  SQL-RC-E817               PIC S9(4) COMP-5 VALUE -817.
      * time stamp conflict
       77  SQL-RC-E818               PIC S9(4) COMP-5 VALUE -818.
      * invalid address in sqlda
       77  SQL-RC-E822               PIC S9(4) COMP-5 VALUE -822.
      * Dynamic statement requires a result area
       77  SQL-RC-E838               PIC S9(4) COMP-5 VALUE -838.
      * too many items in list
       77  SQL-RC-E840               PIC S9(4) COMP-5 VALUE -840.
      * connection already exists
       77  SQL-RC-E842               PIC S9(4) COMP-5 VALUE -842.
      * connection does not exist
       77  SQL-RC-E843               PIC S9(4) COMP-5 VALUE -843.
      * cannot use CURRVAL before NEXTVAL
       77  SQL-RC-E845               PIC S9(4) COMP-5 VALUE -845.
      * Invalid identity specification
       77  SQL-RC-E846               PIC S9(4) COMP-5 VALUE -846.
      * can't change connection settings
       77  SQL-RC-E847               PIC S9(4) COMP-5 VALUE -847.
      * TM not for 2 phase commit apps
       77  SQL-RC-E859               PIC S9(4) COMP-5 VALUE -859.
      * Referential contstraint attempted to modify a table modified by SQL
      * data change stmt
       77  SQL-RC-E864               PIC S9(4) COMP-5 VALUE -864.
      * inv TM_DATABASE value
       77  SQL-RC-E865               PIC S9(4) COMP-5 VALUE -865.
      * connect redirect failed
       77  SQL-RC-E866               PIC S9(4) COMP-5 VALUE -866.
      * connection already exists
       77  SQL-RC-E868               PIC S9(4) COMP-5 VALUE -868.
      * objects with different encoding cannot be referenced
       77  SQL-RC-E873               PIC S9(4) COMP-5 VALUE -873.
      * CCSID parameter must match routine parameter
       77  SQL-RC-E874               PIC S9(4) COMP-5 VALUE -874.
      * savepoint does not exist or is invalid
       77  SQL-RC-E880               PIC S9(4) COMP-5 VALUE -880.
      * savepoint name cannot be reused
       77  SQL-RC-E881               PIC S9(4) COMP-5 VALUE -881.
      * savepoint does not exist
       77  SQL-RC-E882               PIC S9(4) COMP-5 VALUE -882.

      * non-fatal system error
       77  SQL-RC-E901               PIC S9(4) COMP-5 VALUE -901.
      * fatal error
       77  SQL-RC-E902               PIC S9(4) COMP-5 VALUE -902.
      * commit failed, rollback
       77  SQL-RC-E903               PIC S9(4) COMP-5 VALUE -903.
      * resource unavailable
       77  SQL-RC-E904               PIC S9(4) COMP-5 VALUE -904.
      * resource limit exceeded
       77  SQL-RC-E905               PIC S9(4) COMP-5 VALUE -905.
      * function disabled
       77  SQL-RC-E906               PIC S9(4) COMP-5 VALUE -906.
      * failed attempt to modify target table of MERGE stmt
       77  SQL-RC-E907               PIC S9(4) COMP-5 VALUE -907.
      * BIND operation not allowed
       77  SQL-RC-E908               PIC S9(4) COMP-5 VALUE -908.
      * object deleted
       77  SQL-RC-E909               PIC S9(4) COMP-5 VALUE -909.
      * drop pending
       77  SQL-RC-E910               PIC S9(4) COMP-5 VALUE -910.
      * deadlock
       77  SQL-RC-E911               PIC S9(4) COMP-5 VALUE -911.
      * too many lock requests
       77  SQL-RC-E912               PIC S9(4) COMP-5 VALUE -912.
      * dist env rollback     
       77  SQL-RC-E913               PIC S9(4) COMP-5 VALUE -913.
      * remote rebind from DRDA failed
       77  SQL-RC-E917               PIC S9(4) COMP-5 VALUE -917.
      * ROLLBACK required
       77  SQL-RC-E918               PIC S9(4) COMP-5 VALUE -918.
      * reject DB connection to SA REQ
       77  SQL-RC-E920               PIC S9(4) COMP-5 VALUE -920.
      * COMMIT not allowed
       77  SQL-RC-E925               PIC S9(4) COMP-5 VALUE -925.
      * ROLLBACK not allowed
       77  SQL-RC-E926               PIC S9(4) COMP-5 VALUE -926.
      * insufficient storage
       77  SQL-RC-E930               PIC S9(4) COMP-5 VALUE -930.
      * all inodes used, system limit
       77  SQL-RC-INODE              PIC S9(4) COMP-5 VALUE -931.
      * cursors active on dropped object
       77  SQL-RC-E950               PIC S9(4) COMP-5 VALUE -950.
      * table in use - alter disallowed
       77  SQL-RC-E951               PIC S9(4) COMP-5 VALUE -951.
      * user cancel
       77  SQL-RC-E952               PIC S9(4) COMP-5 VALUE -952.
      * agent heap too small
       77  SQL-RC-E953               PIC S9(4) COMP-5 VALUE -953.
      * application heap too small
       77  SQL-RC-E954               PIC S9(4) COMP-5 VALUE -954.
      * sort heap error
       77  SQL-RC-E955               PIC S9(4) COMP-5 VALUE -955.
      * database heap too small
       77  SQL-RC-E956               PIC S9(4) COMP-5 VALUE -956.
      * max number of files open
       77  SQL-RC-E958               PIC S9(4) COMP-5 VALUE -958.
      * server comm heap too small
       77  SQL-RC-E959               PIC S9(4) COMP-5 VALUE -959.
      * max nbr of files in database
       77  SQL-RC-E960               PIC S9(4) COMP-5 VALUE -960.
      * log file full
       77  SQL-RC-E964               PIC S9(4) COMP-5 VALUE -964.
      * error opening sqlcode mapping file
       77  SQL-RC-E966               PIC S9(4) COMP-5 VALUE -966.
      * format error in sqlcode map file
       77  SQL-RC-E967               PIC S9(4) COMP-5 VALUE -967.
      * disk full
       77  SQL-RC-E968               PIC S9(4) COMP-5 VALUE -968.
      * unknown sql error another product
       77  SQL-RC-E969               PIC S9(4) COMP-5 VALUE -969.
      * read-only file
       77  SQL-RC-E970               PIC S9(4) COMP-5 VALUE -970.

      * incorrect diskette
       77  SQL-RC-E972-DB2AIX        PIC S9(9) COMP-5 VALUE -10019.
      * incorrect diskette
       77  SQL-RC-E972-DB2OS2        PIC S9(4) COMP-5 VALUE -972.
      * incorrect diskette
       77  SQL-RC-E972               PIC S9(4) COMP-5 VALUE -972.

      * out of memory error
       77  SQL-RC-E973               PIC S9(4) COMP-5 VALUE -973.
      * out of memory warning
       77  SQL-RC-W973               PIC S9(4) COMP-5 VALUE 973.

      * drive locked
       77  SQL-RC-E974-DB2AIX        PIC S9(9) COMP-5 VALUE -10019.
      * drive locked
       77  SQL-RC-E974-DB2OS2        PIC S9(4) COMP-5 VALUE -974.
      * drive locked
       77  SQL-RC-E974               PIC S9(4) COMP-5 VALUE -974.

      * quiesce backup
       77  SQL-RC-QBACK              PIC S9(4) COMP-5 VALUE -975.

      * diskette door open
       77  SQL-RC-E976-DB2AIX        PIC S9(9) COMP-5 VALUE -10019.
      * diskette door open
       77  SQL-RC-E976-DB2OS2        PIC S9(4) COMP-5 VALUE -976.
      * diskette door open
       77  SQL-RC-E976               PIC S9(4) COMP-5 VALUE -976.

      * unknown commit state
       77  SQL-RC-E977               PIC S9(4) COMP-5 VALUE -977.
      * diskette write-protected
       77  SQL-RC-E978-DB2AIX        PIC S9(9) COMP-5 VALUE -10019.
      * diskette write-protected
       77  SQL-RC-E978-DB2OS2        PIC S9(4) COMP-5 VALUE -978.
      * diskette write-protected
       77  SQL-RC-E978               PIC S9(4) COMP-5 VALUE -978.

      * commit failed with SYNCPOINT NONE
       77  SQL-RC-E979               PIC S9(4) COMP-5 VALUE -979.
      * disk error
       77  SQL-RC-E980               PIC S9(4) COMP-5 VALUE -980.
      * disk error on temp file
       77  SQL-RC-E982               PIC S9(4) COMP-5 VALUE -982.
      * unsuccessful commit or rollback
       77  SQL-RC-E984               PIC S9(4) COMP-5 VALUE -984.
      * file error - catalog file bad
       77  SQL-RC-E985               PIC S9(4) COMP-5 VALUE -985.
      * file error - file renamed
       77  SQL-RC-E986               PIC S9(4) COMP-5 VALUE -986.
      * application shared memory cannot be allocated
       77  SQL-RC-E987               PIC S9(4) COMP-5 VALUE -987.
      * AFTER trigger cannot modify a row being modified by an SQL data
      * change stmt
       77  SQL-RC-E989               PIC S9(4) COMP-5 VALUE -989.
      * index structure limit error
       77  SQL-RC-E990               PIC S9(4) COMP-5 VALUE -990.
      * release number incompatible
       77  SQL-RC-E992               PIC S9(4) COMP-5 VALUE -992.
      * reserved
       77  SQL-RC-E994               PIC S9(4) COMP-5 VALUE -994.
      * error freeing pages in DMS tablespace
       77  SQL-RC-E996               PIC S9(4) COMP-5 VALUE -996.
      * General XA error
       77  SQL-RC-E998               PIC S9(4) COMP-5 VALUE -998.

      * error finding pages in DMS tablespace
       77  SQL-RC-E1007              PIC S9(4) COMP-5 VALUE -1007.
      * invalid tablespace id
       77  SQL-RC-E1008              PIC S9(4) COMP-5 VALUE -1008.
      * Unexpected system error
       77  SQL-RC-E1042              PIC S9(4) COMP-5 VALUE -1042.
      * Authid not valid
       77  SQL-RC-E1046              PIC S9(4) COMP-5 VALUE -1046.
      * domain is not defined in DB2DOMAINLIST
       77  SQL-RC-E1068              PIC S9(4) COMP-5 VALUE -1068.
      * Insufficient authority
       77  SQL-RC-E1092              PIC S9(4) COMP-5 VALUE -1092.
      * User not logged on
       77  SQL-RC-E1093              PIC S9(4) COMP-5 VALUE -1093.
      * Remote database cannot be accessed
       77  SQL-RC-E1101              PIC S9(4) COMP-5 VALUE -1101.
      * table space maximum size exceeded
       77  SQL-RC-E1139              PIC S9(4) COMP-5 VALUE -1139.
      * inspect completes with error/warnings
       77  SQL-RC-E1141              PIC S9(4) COMP-5 VALUE -1141.
      * inspect fails with file in use
       77  SQL-RC-E1142              PIC S9(4) COMP-5 VALUE -1142.
      * inspect fails with file i/o error
       77  SQL-RC-E1143              PIC S9(4) COMP-5 VALUE -1143.
      * transaction rolled back due to failure creating index
       77  SQL-RC-E1144              PIC S9(4) COMP-5 VALUE -1144.
      * dynamically prepared statement not supported
       77  SQL-RC-E1145              PIC S9(4) COMP-5 VALUE -1145.
      * There are no indexes on table <name>
       77  SQL-RC-E1146              PIC S9(4) COMP-5 VALUE -1146.
      * Index needs refresh, but tablespace is in backup pending state
       77  SQL-RC-E1148              PIC S9(4) COMP-5 VALUE -1148.
      * table cannot be enabled for data capture
       77  SQL-RC-E1163              PIC S9(4) COMP-5 VALUE -1163.
      * An error occurred while explaining the statement.
       77  SQL-RC-E1169              PIC S9(4) COMP-5 VALUE -1169.
      * FEDERATED does not reference nickname or OLE DB function
       77  SQL-RC-E1178              PIC S9(4) COMP-5 VALUE -1178.
      * UDF caused an OLE error
       77  SQL-RC-E1180              PIC S9(4) COMP-5 VALUE -1180.
      * UDF raised an exception
       77  SQL-RC-E1181              PIC S9(4) COMP-5 VALUE -1181.
      * UDF cannot initialize source object of OLE DB
       77  SQL-RC-E1182              PIC S9(4) COMP-5 VALUE -1182.
      * UDF received OLE DB error
       77  SQL-RC-E1183              PIC S9(4) COMP-5 VALUE -1183.
      * EXPLAIN table(s) created with earlier EXPLAIN.DDL
       77  SQL-RC-E1184              PIC S9(4) COMP-5 VALUE -1184.
      * FEDERATED clause needed to bind package
       77  SQL-RC-E1185              PIC S9(4) COMP-5 VALUE -1185.
      * FEDERATED needed with nickname or OLE DB function
       77  SQL-RC-E1186              PIC S9(4) COMP-5 VALUE -1186.
      * command is not supported in the current downlevel client-server
      * configuration
       77  SQL-RC-E1198              PIC S9(4) COMP-5 VALUE -1198.
      * Suboptimal performance. Reconnect to an alternate node.
       77  SQL-RC-E1199              PIC S9(4) COMP-5 VALUE -1199.

      * invalid use of graphic data
       77  SQL-RC-E1216              PIC S9(4) COMP-5 VALUE -1216.
      * REAL data type not supported
       77  SQL-RC-E1217              PIC S9(4) COMP-5 VALUE -1217.
      * no bufferpool pages available
       77  SQL-RC-E1218              PIC S9(4) COMP-5 VALUE -1218.
      * out of private memory
       77  SQL-RC-E1219              PIC S9(4) COMP-5 VALUE -1219.
      * max number of agents started
       77  SQL-RC-E1226              PIC S9(4) COMP-5 VALUE -1226.
      * Updatale cat values inv
       77  SQL-RC-E1227              PIC S9(4) COMP-5 VALUE -1227.
      * System error rollback
       77  SQL-RC-E1229              PIC S9(4) COMP-5 VALUE -1229.
      * Graphic data that is not UCS-2 is not supported
       77  SQL-RC-E1233              PIC S9(4) COMP-5 VALUE -1233.
      * Disconn at next commit
       77  SQL-RC-W1244              PIC S9(4) COMP-5 VALUE 1244.
      * Max client connects
       77  SQL-RC-E1245              PIC S9(4) COMP-5 VALUE -1245.
      * XA TM uses syncpoint 2
       77  SQL-RC-E1247              PIC S9(4) COMP-5 VALUE -1247.
      * db not defined with TM
       77  SQL-RC-E1248              PIC S9(4) COMP-5 VALUE -1248.
      * Non-SQL requests not supported from this downlevel client
       77  SQL-RC-E1288              PIC S9(4) COMP-5 VALUE -1288.
      * DB2CLIENTCOMM env var inv
       77  SQL-RC-E1290              PIC S9(4) COMP-5 VALUE -1290.
      * direcory services error
       77  SQL-RC-E1291              PIC S9(4) COMP-5 VALUE -1291.
      * global dir error
       77  SQL-RC-E1293              PIC S9(4) COMP-5 VALUE -1293.
      * global dir path invalid
       77  SQL-RC-E1294              PIC S9(4) COMP-5 VALUE -1294.
      * global dir router invalid
       77  SQL-RC-E1295              PIC S9(4) COMP-5 VALUE -1295.

      * Error processing keytab file
       77  SQL-RC-E1301              PIC S9(4) COMP-5 VALUE -1301.
      * DCE principal to authid mapping error
       77  SQL-RC-E1302              PIC S9(4) COMP-5 VALUE -1302.
      * Internal DCE error
       77  SQL-RC-E1305              PIC S9(4) COMP-5 VALUE -1305.
      * Invalid server principal name
       77  SQL-RC-E1309              PIC S9(4) COMP-5 VALUE -1309.
      * Error writing to audit log
       77  SQL-RC-E1322              PIC S9(4) COMP-5 VALUE -1322.
      * Error accessing audit cfg
       77  SQL-RC-E1323              PIC S9(4) COMP-5 VALUE -1323.
      * Remote host not found
       77  SQL-RC-E1336              PIC S9(4) COMP-5 VALUE -1336.
      * Not Atomic Compound SQL error(s)
       77  SQL-RC-E1339              PIC S9(4) COMP-5 VALUE -1339.

      * Plugin processing failed on the server
       77  SQL-RC-E1365              PIC S9(4) COMP-5 VALUE -1365.
      * Plugin processing failed on the client
       77  SQL-RC-E1366              PIC S9(4) COMP-5 VALUE -1366.
      * Operating system does not support resource policy definition.
       77  SQL-RC-E1367              PIC S9(4) COMP-5 VALUE -1367.
      * Invalid resource policy configuration
       77  SQL-RC-E1368              PIC S9(4) COMP-5 VALUE -1368.

      * Unexpected Kerberos security error
       77  SQL-RC-E1380              PIC S9(4) COMP-5 VALUE -1380.
      * Security support interface not available
       77  SQL-RC-E1381              PIC S9(4) COMP-5 VALUE -1381.
      * The Kerberos support not available
       77  SQL-RC-E1382              PIC S9(4) COMP-5 VALUE -1382.
      * Invalid target principal name
       77  SQL-RC-E1383              PIC S9(4) COMP-5 VALUE -1383.
      * Unable to complete mutual authentication
       77  SQL-RC-E1384              PIC S9(4) COMP-5 VALUE -1384.

      * Auth type unsupported
       77  SQL-RC-E1400              PIC S9(4) COMP-5 VALUE -1400.
      * Unexpected auth system error
       77  SQL-RC-E1402              PIC S9(4) COMP-5 VALUE -1402.
      * Username or password incorrect
       77  SQL-RC-E1403              PIC S9(4) COMP-5 VALUE -1403.
      * Password expired
       77  SQL-RC-E1404              PIC S9(4) COMP-5 VALUE -1404.
      * Error communicating to auth server
       77  SQL-RC-E1405              PIC S9(4) COMP-5 VALUE -1405.
      * too many concat operators
       77  SQL-RC-E1420              PIC S9(4) COMP-5 VALUE -1420.
      * MBCS conversion error
       77  SQL-RC-E1421              PIC S9(4) COMP-5 VALUE -1421.
      * Container wrong size
       77  SQL-RC-E1422              PIC S9(4) COMP-5 VALUE -1422.
      * no blobs for dwn lvl cl
       77  SQL-RC-E1423              PIC S9(4) COMP-5 VALUE -1423.
      * too many transition tbles
       77  SQL-RC-E1424              PIC S9(4) COMP-5 VALUE -1424.
      * 32 / 64 bit connect incompatible
       77  SQL-RC-E1434              PIC S9(4) COMP-5 VALUE -1434.

      * only valid for sync mgr connection
       77  SQL-RC-E1462              PIC S9(4) COMP-5 VALUE -1462.

      * SUSPEND WRITE command failed
       77  SQL-RC-E1550              PIC S9(4) COMP-5 VALUE -1550.
      * RESUME WRITE command failed
       77  SQL-RC-E1551              PIC S9(4) COMP-5 VALUE -1551.
      * RESTART command failed; database is in SUSPEND WRITE
       77  SQL-RC-E1552              PIC S9(4) COMP-5 VALUE -1552.
      * DB2STOP command failed; database is in SUSPEND WRITE
       77  SQL-RC-E1553              PIC S9(4) COMP-5 VALUE -1553.
      * table cannot be in append mode with clustering index
       77  SQL-RC-E1581              PIC S9(4) COMP-5 VALUE -1581.
      * pagesize of table space doesn't match bufferpool
       77  SQL-RC-E1582              PIC S9(4) COMP-5 VALUE -1582.
      * pagesize not supported
       77  SQL-RC-E1583              PIC S9(4) COMP-5 VALUE -1583.
      * temporary table space cannot be found
       77  SQL-RC-E1584              PIC S9(4) COMP-5 VALUE -1584.
      * No temp table space with sufficient page size
       77  SQL-RC-E1585              PIC S9(4) COMP-5 VALUE -1585.
      * ON option of SET INTEGRITY invalid
       77  SQL-RC-E1591              PIC S9(4) COMP-5 VALUE -1591.
      * INCREMENTAL option invalid
       77  SQL-RC-E1592              PIC S9(4) COMP-5 VALUE -1592.
      * REMAIN PENDING option invalid
       77  SQL-RC-E1593              PIC S9(4) COMP-5 VALUE -1593.
      * table integrity cannot be checked
       77  SQL-RC-E1595              PIC S9(4) COMP-5 VALUE -1595.
      * WITH EMPTY TABLE option not allowed because of dependent REFRESH
      * IMMEDIATE table 
       77  SQL-RC-E1596              PIC S9(4) COMP-5 VALUE -1596.

      * The table is not created with NOT LOGGED INITIALLY
       77  SQL-RC-E1749              PIC S9(4) COMP-5 VALUE -1749.

      * foreign keys not allowed for tables created not logged initially
       77  SQL-RC-E1750              PIC S9(4) COMP-5 VALUE -1750.
      * nodegroups must have at least one node
       77  SQL-RC-E1751              PIC S9(4) COMP-5 VALUE -1751.
      * tablespace not created in the correct nodegroup
       77  SQL-RC-E1752              PIC S9(4) COMP-5 VALUE -1752.
      * a node does not have the complete temporary tablespaces
       77  SQL-RC-E1753              PIC S9(4) COMP-5 VALUE -1753.
      * all tablespace in CREATE TABLE must be in the same nodegroup
       77  SQL-RC-E1754              PIC S9(4) COMP-5 VALUE -1754.
      * more than one clause specifies containers without ON NODES clause
       77  SQL-RC-E1756              PIC S9(4) COMP-5 VALUE -1756.
      * missing USING clause without ON NODES clause
       77  SQL-RC-E1757              PIC S9(4) COMP-5 VALUE -1757.
      * missing clause for create stored procedure
       77  SQL-RC-E1760              PIC S9(4) COMP-5 VALUE -1760.
      * nodegroup not defined for buffer pool
       77  SQL-RC-E1761              PIC S9(4) COMP-5 VALUE -1761.
      * not enough disk space for connect
       77  SQL-RC-E1762              PIC S9(4) COMP-5 VALUE -1762.
      * cannot specify multiple ALTER TABLESPACE actions
       77  SQL-RC-E1763              PIC S9(4) COMP-5 VALUE -1763.
      * cannot specify a smaller container size
       77  SQL-RC-E1764              PIC S9(4) COMP-5 VALUE -1764.

      * operation cannot be executed in No Package Lock mode
       77  SQL-RC-E1803              PIC S9(4) COMP-5 VALUE -1803.
      * Wrapper cannot be used to access data source
       77  SQL-RC-E1816              PIC S9(4) COMP-5 VALUE -1816.
      * CREATE SERVER statement does not identify data source
       77  SQL-RC-E1817              PIC S9(4) COMP-5 VALUE -1817.
      * ALTER SERVER statement cannot be processed
       77  SQL-RC-E1818              PIC S9(4) COMP-5 VALUE -1818.
      * DROP SERVER statement cannot be processed
       77  SQL-RC-E1819              PIC S9(4) COMP-5 VALUE -1819.
      * Action on LOB value failed
       77  SQL-RC-E1820              PIC S9(4) COMP-5 VALUE -1820.
      * Unexpected error from data source
       77  SQL-RC-E1822              PIC S9(4) COMP-5 VALUE -1822.
      * No data type mapping exists for server
       77  SQL-RC-E1823              PIC S9(4) COMP-5 VALUE -1823.
      * Statement cannot be handled by Datajoiner
       77  SQL-RC-E1825              PIC S9(4) COMP-5 VALUE -1825.
      * Invalid value for system catalog column
       77  SQL-RC-E1826              PIC S9(4) COMP-5 VALUE -1826.
      * user mapping undefined
       77  SQL-RC-E1827              PIC S9(4) COMP-5 VALUE -1827.
      * server option undefined
       77  SQL-RC-E1828              PIC S9(4) COMP-5 VALUE -1828.

      * TCPIP not running
       77  SQL-RC-NO-TCPIP           PIC S9(4) COMP-5 VALUE -1468.
      * invalid node
       77  SQL-RC-NODE-INVALID       PIC S9(4) COMP-5 VALUE -1469.
      * DB2NODE env var invalid
       77  SQL-RC-DB2NODE-INVALID    PIC S9(4) COMP-5 VALUE -1470.
      * nodes out of sync
       77  SQL-RC-LOG-MISMATCH       PIC S9(4) COMP-5 VALUE -1471.
      * connect fail, system clocks out of sync
       77  SQL-RC-TIMEDIFF-CA        PIC S9(4) COMP-5 VALUE -1472.
      * commit fail, system clocks out of sync
       77  SQL-RC-TIMEDIFF-CO        PIC S9(4) COMP-5 VALUE -1473.
      * transaction OK, system clocks out of sync
       77  SQL-RC-TIMEDIFF-W         PIC S9(4) COMP-5 VALUE -1474.
      * rollback on table error
       77  SQL-RC-E1476              PIC S9(4) COMP-5 VALUE -1476.
      * table not accessible
       77  SQL-RC-E1477              PIC S9(4) COMP-5 VALUE -1477.

      * LONG not allowed on devices
       77  SQL-RC-E1590              PIC S9(4) COMP-5 VALUE -1590.
      * Specified definition, schema, or nickname does not exist
       77  SQL-RC-E1791              PIC S9(4) COMP-5 VALUE -1791.
      * RETURNS clause must be specified before EXPRESSION AS
       77  SQL-RC-E1830              PIC S9(4) COMP-5 VALUE -1830.
      * Cannot update table stats for subtable
       77  SQL-RC-E1831              PIC S9(4) COMP-5 VALUE -1831.
      * data filter function cannot be LANGUAGE SQL
       77  SQL-RC-E1832              PIC S9(4) COMP-5 VALUE -1832.
      * Connection to Extended Search Server failed
       77  SQL-RC-E1833              PIC S9(4) COMP-5 VALUE -1833.
      * User-defined column identical to fixed column
       77  SQL-RC-E1834              PIC S9(4) COMP-5 VALUE -1834.
      * Extended Search  object could not be found 
       77  SQL-RC-E1835              PIC S9(4) COMP-5 VALUE -1835.
      * No column mapping exist to Extended Search field
       77  SQL-RC-E1836              PIC S9(4) COMP-5 VALUE -1836.
      * Required option can not be dropped
       77  SQL-RC-E1837              PIC S9(4) COMP-5 VALUE -1837.
      * Statement is not a valid Extended Search query
       77  SQL-RC-E1838              PIC S9(4) COMP-5 VALUE -1838.
      * One ore more search parameter are not valid
       77  SQL-RC-E1839              PIC S9(4) COMP-5 VALUE -1839.
      * Option cannot be added to object
       77  SQL-RC-E1840              PIC S9(4) COMP-5 VALUE -1840.
      * The value cannot be changed for the object
       77  SQL-RC-E1841              PIC S9(4) COMP-5 VALUE -1841.
      * Option is not valid
       77  SQL-RC-E1842              PIC S9(4) COMP-5 VALUE -1842.
      * The operator is not supported
       77  SQL-RC-E1843              PIC S9(4) COMP-5 VALUE -1843.
      * The option conflicts with the object
       77  SQL-RC-E1846              PIC S9(4) COMP-5 VALUE -1846.
      * Template substiution error
       77  SQL-RC-E1847              PIC S9(4) COMP-5 VALUE -1847.
      * Key sequence column is out of range
       77  SQL-RC-E1870              PIC S9(4) COMP-5 VALUE -1870.
      * Function not supported on range-clustered tables
       77  SQL-RC-E1871              PIC S9(4) COMP-5 VALUE -1871.
      * option is not valid for this data source
       77  SQL-RC-E1881              PIC S9(4) COMP-5 VALUE -1881.
      * option is not valid for this data source
       77  SQL-RC-E1882              PIC S9(4) COMP-5 VALUE -1882.
      * Missing option for this data source
       77  SQL-RC-E1883              PIC S9(4) COMP-5 VALUE -1883.
      * Option specified more than once
       77  SQL-RC-E1884              PIC S9(4) COMP-5 VALUE -1884.
      * option is already defined
       77  SQL-RC-E1885              PIC S9(4) COMP-5 VALUE -1885.
      * option has not been added
       77  SQL-RC-E1886              PIC S9(4) COMP-5 VALUE -1886.
      * SPECIFICATION ONLY clause required
       77  SQL-RC-E1887              PIC S9(4) COMP-5 VALUE -1887.

      * cannot add/drop DB2 Data Links Manager
       77  SQL-RC-E2078              PIC S9(4) COMP-5 VALUE -2078.

      * incorrect syntax or password for filtering
       77  SQL-RC-E2180              PIC S9(4) COMP-5 VALUE -2180.
      * internal error occurred during filter recovery
       77  SQL-RC-E2181              PIC S9(4) COMP-5 VALUE -2181.
      * A statistics profile does not exist
       77  SQL-RC-E2315              PIC S9(4) COMP-5 VALUE -2315.

      * Invalid user format in filetmod
       77  SQL-RC-E3191              PIC S9(4) COMP-5 VALUE -3191.
      * Invalid user format in filetmod
       77  SQL-RC-E3192              PIC S9(4) COMP-5 VALUE -3192.

      * Cannot import replace parent
       77  SQL-RC-E3201              PIC S9(4) COMP-5 VALUE -3201.
      * Unexpected LDAP error
       77  SQL-RC-E3260              PIC S9(4) COMP-5 VALUE -3260.
      * Missing required parameters
       77  SQL-RC-E3261              PIC S9(4) COMP-5 VALUE -3261.
      * The TCP/IP service name is not valid
       77  SQL-RC-E3262              PIC S9(4) COMP-5 VALUE -3262.
      * The protocol type is not supported
       77  SQL-RC-E3263              PIC S9(4) COMP-5 VALUE -3263.
      * The DB2 server has not been registered in LDAP
       77  SQL-RC-E3264              PIC S9(4) COMP-5 VALUE -3264.
      * LDAP authentication error
       77  SQL-RC-E3265              PIC S9(4) COMP-5 VALUE -3265.
      * Insufficient LDAP authority
       77  SQL-RC-E3267              PIC S9(4) COMP-5 VALUE -3267.
      * LDAP schema is not compatible
       77  SQL-RC-E3268              PIC S9(4) COMP-5 VALUE -3268.
      * The LDAP server is not available
       77  SQL-RC-E3269              PIC S9(4) COMP-5 VALUE -3269.
      * LDAP user's DN is invalid
       77  SQL-RC-E3270              PIC S9(4) COMP-5 VALUE -3270.
      * LDAP user's DN or password is not configured
       77  SQL-RC-E3271              PIC S9(4) COMP-5 VALUE -3271.
      * The LDAP node was not found
       77  SQL-RC-E3272              PIC S9(4) COMP-5 VALUE -3272.
      * The LDAP database was not found
       77  SQL-RC-E3273              PIC S9(4) COMP-5 VALUE -3273.
      * Unable to obtain the LDAP naming context
       77  SQL-RC-E3276              PIC S9(4) COMP-5 VALUE -3276.
      * The database already exists
       77  SQL-RC-E3277              PIC S9(4) COMP-5 VALUE -3277.
      * The node already exists
       77  SQL-RC-E3278              PIC S9(4) COMP-5 VALUE -3278.
      * LDAP is disabled
       77  SQL-RC-E3279              PIC S9(4) COMP-5 VALUE -3279.
      * Attempt to connect to a DRDA server failed.
       77  SQL-RC-E3280              PIC S9(4) COMP-5 VALUE -3280.
      * The operating system type is invalid
       77  SQL-RC-E3281              PIC S9(4) COMP-5 VALUE -3281.
      * The supplied credentials are not valid.
       77  SQL-RC-E3282              PIC S9(4) COMP-5 VALUE -3282.
      * The node type is not supported
       77  SQL-RC-E3284              PIC S9(4) COMP-5 VALUE -3284.

      * Table not in check pend state
       77  SQL-RC-E3600              PIC S9(4) COMP-5 VALUE -3600.
      * Auto Check pending state
       77  SQL-RC-W3601              PIC S9(4) COMP-5 VALUE 3601.
      * Constraint violation moved
       77  SQL-RC-W3602              PIC S9(4) COMP-5 VALUE 3602.
      * Constraint Violations
       77  SQL-RC-E3603              PIC S9(4) COMP-5 VALUE -3603.
      * Invalid Exception Table
       77  SQL-RC-E3604              PIC S9(4) COMP-5 VALUE -3604.
      * Exception Table same as Check Table
       77  SQL-RC-E3605              PIC S9(4) COMP-5 VALUE -3605.
      * # check & exception tables not match
       77  SQL-RC-E3606              PIC S9(4) COMP-5 VALUE -3606.
      * Parent in Check Pending state
       77  SQL-RC-E3608              PIC S9(4) COMP-5 VALUE -3608.

      * Invalid SQL sub-statement in Compound SQL
       77  SQL-RC-E4011              PIC S9(4) COMP-5 VALUE -4011.
      * A 'long' host variable is not valid - use 'sqlint32' instead.
       77  SQL-RC-E4020              PIC S9(4) COMP-5 VALUE -4020.

      * Java support not installed
       77  SQL-RC-E4300              PIC S9(4) COMP-5 VALUE -4300.
      * Java startup, comm, shutdown failed
       77  SQL-RC-E4301              PIC S9(4) COMP-5 VALUE -4301.
      * Java unclassified exception
       77  SQL-RC-E4302              PIC S9(4) COMP-5 VALUE -4302.
      * Java could not parse class!method
       77  SQL-RC-E4303              PIC S9(4) COMP-5 VALUE -4303.
      * Java could not instantiate class
       77  SQL-RC-E4304              PIC S9(4) COMP-5 VALUE -4304.
      * Java internal error code
       77  SQL-RC-E4305              PIC S9(4) COMP-5 VALUE -4305.
      * Java cannot call method
       77  SQL-RC-E4306              PIC S9(4) COMP-5 VALUE -4306.
      * Java method call problems
       77  SQL-RC-E4307              PIC S9(4) COMP-5 VALUE -4307.

      * Host variable is not exact numeric type
       77  SQL-RC-E5012              PIC S9(4) COMP-5 VALUE -5012.
      * Invalid qualifier specified in CREATE SCHEMA
       77  SQL-RC-E5051              PIC S9(4) COMP-5 VALUE -5051.

      * Required environment variable no set
       77  SQL-RC-E5182              PIC S9(4) COMP-5 VALUE -5182.
      * Pass-through not supported for server type
       77  SQL-RC-E5185              PIC S9(4) COMP-5 VALUE -5185.

      * Unable to open vendor configuration file
       77  SQL-RC-E5500              PIC S9(4) COMP-5 VALUE -5500.
      * Format of vendor configuration file is wrong
       77  SQL-RC-E5501              PIC S9(4) COMP-5 VALUE -5501.

      * Insufficient FCM buffers
       77  SQL-RC-E6040              PIC S9(4) COMP-5 VALUE -6040.
      * Insufficient FCM connection entries
       77  SQL-RC-E6041              PIC S9(4) COMP-5 VALUE -6041.
      * Insufficient FCM message anchors
       77  SQL-RC-E6042              PIC S9(4) COMP-5 VALUE -6042.
      * Insufficient FCM request blocks
       77  SQL-RC-E6043              PIC S9(4) COMP-5 VALUE -6043.
      * New node requires stop and start
       77  SQL-RC-E6071              PIC S9(4) COMP-5 VALUE -6071.

      * SQL Procedure not created       
       77  SQL-RC-E7032              PIC S9(4) COMP-5 VALUE -7032.

      * db2start failed, license not found
       77  SQL-RC-E8000              PIC S9(4) COMP-5 VALUE -8000.
      * udb connection failed, license not found
       77  SQL-RC-E8001              PIC S9(4) COMP-5 VALUE -8001.
      * 'connect' connection failed, license not found
       77  SQL-RC-E8002              PIC S9(4) COMP-5 VALUE -8002.
      * evaluation period expired
       77  SQL-RC-E8008              PIC S9(4) COMP-5 VALUE -8008.
      * not licensed for TCP/IP connections.
       77  SQL-RC-E8014              PIC S9(4) COMP-5 VALUE -8014.
      * not licensed for multiple database updates per transaction
       77  SQL-RC-E8015              PIC S9(4) COMP-5 VALUE -8015.
      * this user is not defined as registered user
       77  SQL-RC-E8016              PIC S9(4) COMP-5 VALUE -8016.
      * not licensed for database partitioning
       77  SQL-RC-E8022              PIC S9(4) COMP-5 VALUE -8022.
      * concurrent users limit exceeded
       77  SQL-RC-E8023              PIC S9(4) COMP-5 VALUE -8023.
      * limited function license - function not allowed
       77  SQL-RC-E8024              PIC S9(4) COMP-5 VALUE -8024.

      * Page number too high
       77  SQL-RC-E8100              PIC S9(4) COMP-5 VALUE -8100.
      * Segment in error
       77  SQL-RC-E8101              PIC S9(4) COMP-5 VALUE -8101.

      * Development SQLCODE
       77  SQL-RC-E9999              PIC S9(4) COMP-5 VALUE -9999.
      * not enough systems resources to process request
       77  SQL-RC-C10003             PIC S9(9) COMP-5 VALUE -10003.
      * internal object limit exceeded
       77  SQL-RC-E20005             PIC S9(9) COMP-5 VALUE -20005.
      * mutation methd not allowed
       77  SQL-RC-E20010             PIC S9(9) COMP-5 VALUE -20010.
      * transform for data type already exists
       77  SQL-RC-E20011             PIC S9(9) COMP-5 VALUE -20011.
      * no transforms were dropped
       77  SQL-RC-E20012             PIC S9(9) COMP-5 VALUE -20012.
      * Invalid object for supertype
       77  SQL-RC-E20013             PIC S9(9) COMP-5 VALUE -20013.
      * function cannot be used as transform function
       77  SQL-RC-E20014             PIC S9(9) COMP-5 VALUE -20014.
      * transform for type not defined
       77  SQL-RC-E20015             PIC S9(9) COMP-5 VALUE -20015.
      * column length value too small
       77  SQL-RC-E20016             PIC S9(9) COMP-5 VALUE -20016.
      * Hierarchy too deep
       77  SQL-RC-E20017             PIC S9(9) COMP-5 VALUE -20017.
      * row function must return at most one row
       77  SQL-RC-E20018             PIC S9(9) COMP-5 VALUE -20018.
      * result type cannot be assigned to RETURN type
       77  SQL-RC-E20019             PIC S9(9) COMP-5 VALUE -20019.
      * Invalid operation for typed table
       77  SQL-RC-E20020             PIC S9(9) COMP-5 VALUE -20020.
      * Cannot change inherited column
       77  SQL-RC-E20021             PIC S9(9) COMP-5 VALUE -20021.
      * SCOPE already defined
       77  SQL-RC-E20022             PIC S9(9) COMP-5 VALUE -20022.
      * SCOPE not allowed for parameter
       77  SQL-RC-E20023             PIC S9(9) COMP-5 VALUE -20023.
      * SCOPE not valid for reference
       77  SQL-RC-E20024             PIC S9(9) COMP-5 VALUE -20024.
      * Incorrect SCOPE for RETURNS
       77  SQL-RC-E20025             PIC S9(9) COMP-5 VALUE -20025.
      * Type is not a structured type
       77  SQL-RC-E20026             PIC S9(9) COMP-5 VALUE -20026.
      * Subtable already exists of type
       77  SQL-RC-E20027             PIC S9(9) COMP-5 VALUE -20027.
      * Subtable schema incorrect
       77  SQL-RC-E20028             PIC S9(9) COMP-5 VALUE -20028.
      * Invalid operation for subtable
       77  SQL-RC-E20029             PIC S9(9) COMP-5 VALUE -20029.
      * Attrs cannot be altered when in use
       77  SQL-RC-E20030             PIC S9(9) COMP-5 VALUE -20030.
      * Invalid object for subtable
       77  SQL-RC-E20031             PIC S9(9) COMP-5 VALUE -20031.
      * Invalid index columns for subtable
       77  SQL-RC-E20032             PIC S9(9) COMP-5 VALUE -20032.
      * Unscoped reference
       77  SQL-RC-E20033             PIC S9(9) COMP-5 VALUE -20033.
      * Invalid TYPE predicate
       77  SQL-RC-E20034             PIC S9(9) COMP-5 VALUE -20034.
      * Invalid path expression
       77  SQL-RC-E20035             PIC S9(9) COMP-5 VALUE -20035.
      * Path includes OID column
       77  SQL-RC-E20036             PIC S9(9) COMP-5 VALUE -20036.
      * REF IS column must be defined
       77  SQL-RC-E20037             PIC S9(9) COMP-5 VALUE -20037.
      * ASC or DESC cannot be specified
       77  SQL-RC-E20038             PIC S9(9) COMP-5 VALUE -20038.
      * definition of index does not match extension
       77  SQL-RC-E20039             PIC S9(9) COMP-5 VALUE -20039.
      * number or type of result inconsistent
       77  SQL-RC-E20040             PIC S9(9) COMP-5 VALUE -20040.
      * number or type of parameters does not match
       77  SQL-RC-E20041             PIC S9(9) COMP-5 VALUE -20041.
      * maximum allowable parameters exceeded
       77  SQL-RC-E20042             PIC S9(9) COMP-5 VALUE -20042.
      * argument for function is invalid
       77  SQL-RC-E20043             PIC S9(9) COMP-5 VALUE -20043.
      * function not supported in CREATE INDEX EXTENSION
       77  SQL-RC-E20044             PIC S9(9) COMP-5 VALUE -20044.
      * datatype of instance parameter invalid
       77  SQL-RC-E20045             PIC S9(9) COMP-5 VALUE -20045.
      * SELECTIVITY clause needs user-defined predicate
       77  SQL-RC-E20046             PIC S9(9) COMP-5 VALUE -20046.
      * search method not found
       77  SQL-RC-E20047             PIC S9(9) COMP-5 VALUE -20047.
      * search method argument mismatch
       77  SQL-RC-E20048             PIC S9(9) COMP-5 VALUE -20048.
      * operand type mismatch
       77  SQL-RC-E20049             PIC S9(9) COMP-5 VALUE -20049.
      * search target or search argument mismatch
       77  SQL-RC-E20050             PIC S9(9) COMP-5 VALUE -20050.
      * argument cannot be both search target and search argument
       77  SQL-RC-E20051             PIC S9(9) COMP-5 VALUE -20051.
      * Cannot update OID column
       77  SQL-RC-E20052             PIC S9(9) COMP-5 VALUE -20052.
      * Fullselect in typed view is not valid
       77  SQL-RC-E20053             PIC S9(9) COMP-5 VALUE -20053.
      * Invalid state for operation
       77  SQL-RC-E20054             PIC S9(9) COMP-5 VALUE -20054.
      * Result column data type not compatible
       77  SQL-RC-E20055             PIC S9(9) COMP-5 VALUE -20055.
      * Processing error on file server
       77  SQL-RC-E20056             PIC S9(9) COMP-5 VALUE -20056.
      * column cannot be defined as read only
       77  SQL-RC-E20057             PIC S9(9) COMP-5 VALUE -20057.
      * fullselect for summary table is invalid
       77  SQL-RC-E20058             PIC S9(9) COMP-5 VALUE -20058.
      * key transform function generated duplicate rows
       77  SQL-RC-E20060             PIC S9(9) COMP-5 VALUE -20060.
      * transform function not valid for function or method
       77  SQL-RC-E20062             PIC S9(9) COMP-5 VALUE -20062.
      * TRANSFORM GROUP clause is required
       77  SQL-RC-E20063             PIC S9(9) COMP-5 VALUE -20063.
      * specified transform group clause is not used
       77  SQL-RC-E20064             PIC S9(9) COMP-5 VALUE -20064.
      * transform group cannot be used with client application
       77  SQL-RC-E20065             PIC S9(9) COMP-5 VALUE -20065.
      * transform function not defined in transform group
       77  SQL-RC-E20066             PIC S9(9) COMP-5 VALUE -20066.
      * transform function defined more than once in transform group
       77  SQL-RC-E20067             PIC S9(9) COMP-5 VALUE -20067.
      * structured type cannot depend on itself
       77  SQL-RC-E20068             PIC S9(9) COMP-5 VALUE -20068.
      * returns type not same as subject type
       77  SQL-RC-E20069             PIC S9(9) COMP-5 VALUE -20069.
      * index not created, column length too long
       77  SQL-RC-E20075             PIC S9(9) COMP-5 VALUE -20075.
      * instance not enabled for operation
       77  SQL-RC-E20076             PIC S9(9) COMP-5 VALUE -20076.
      * Cannot construct object with Datalink or Reference type attributes
       77  SQL-RC-E20077             PIC S9(9) COMP-5 VALUE -20077.
      * operation cannot be applied to object
       77  SQL-RC-E20078             PIC S9(9) COMP-5 VALUE -20078.
      * method specification cannot be dropped
       77  SQL-RC-E20080             PIC S9(9) COMP-5 VALUE -20080.
      * method body must correspond to specfication language
       77  SQL-RC-E20081             PIC S9(9) COMP-5 VALUE -20081.
      * dynamic type is not a subtype of target
       77  SQL-RC-E20082             PIC S9(9) COMP-5 VALUE -20082.
      * returned data type does not match RESULT
       77  SQL-RC-E20083             PIC S9(9) COMP-5 VALUE -20083.
      * routine would override an existing method
       77  SQL-RC-E20084             PIC S9(9) COMP-5 VALUE -20084.
      * Java routine cannot have structured type parameter or returns type
       77  SQL-RC-E20085             PIC S9(9) COMP-5 VALUE -20085.
      * Length of structured type exceeds limit
       77  SQL-RC-E20086             PIC S9(9) COMP-5 VALUE -20086.
      * DEFAULT or NULL cannot be used in an attribute assignment
       77  SQL-RC-E20087             PIC S9(9) COMP-5 VALUE -20087.
      * method name and structured type name match
       77  SQL-RC-E20089             PIC S9(9) COMP-5 VALUE -20089.
      * conversion error between summary table and regular table
       77  SQL-RC-E20093             PIC S9(9) COMP-5 VALUE -20093.
      * GENERATED column cannot be referenced in BEFORE trigger
       77  SQL-RC-E20094             PIC S9(9) COMP-5 VALUE -20094.

      * CREATE or ALTER for the routine is not allowed
       77  SQL-RC-E20102             PIC S9(9) COMP-5 VALUE -20102.
      * result set contains unsupported data type
       77  SQL-RC-E20108             PIC S9(9) COMP-5 VALUE -20108.
      * SAVEPOINT statement is not allowed in this context
       77  SQL-RC-E20111             PIC S9(9) COMP-5 VALUE -20111.
      * nested savepoint is not allowed
       77  SQL-RC-E20112             PIC S9(9) COMP-5 VALUE -20112.
      * cannot return null from SELF AS RESULT method
       77  SQL-RC-E20113             PIC S9(9) COMP-5 VALUE -20113.
      * routine cannot be used as a transform function
       77  SQL-RC-E20115             PIC S9(9) COMP-5 VALUE -20115.
      * search target and source key data types do not match
       77  SQL-RC-E20116             PIC S9(9) COMP-5 VALUE -20116.
      * window specification for an OLAP function invalid
       77  SQL-RC-E20117             PIC S9(9) COMP-5 VALUE -20117.
      * maximum number of attributes exceeded
       77  SQL-RC-E20118             PIC S9(9) COMP-5 VALUE -20118.
      * row function must return at least two columns
       77  SQL-RC-E20119             PIC S9(9) COMP-5 VALUE -20119.
      * SQL table function must return a table
       77  SQL-RC-E20120             PIC S9(9) COMP-5 VALUE -20120.
      * WITH RETURN and SCROLL specified for a single cursor
       77  SQL-RC-E20121             PIC S9(9) COMP-5 VALUE -20121.
      * stored procedure cursor error
       77  SQL-RC-E20123             PIC S9(9) COMP-5 VALUE -20123.
      * scrollable cursor cannot include table function output
       77  SQL-RC-E20128             PIC S9(9) COMP-5 VALUE -20128.
      * Obj number specified more than once in list
       77  SQL-RC-E20131             PIC S9(9) COMP-5 VALUE -20131.
      * operation must be performed on SQL routine
       77  SQL-RC-E20133             PIC S9(9) COMP-5 VALUE -20133.
      * SQL archive could not be created on server
       77  SQL-RC-E20134             PIC S9(9) COMP-5 VALUE -20134.
      * SQL archive does not match target environment
       77  SQL-RC-E20135             PIC S9(9) COMP-5 VALUE -20135.
      * Routine is NOT FEDERATED
       77  SQL-RC-E20136             PIC S9(9) COMP-5 VALUE -20136.
      * Routine is not defined as MODIFIES SQL DATA
       77  SQL-RC-E20138             PIC S9(9) COMP-5 VALUE -20138.
      * Previous statement failed or was interrupted
       77  SQL-RC-E20139             PIC S9(9) COMP-5 VALUE -20139.
      * Sequence cannot be used as specified
       77  SQL-RC-E20142             PIC S9(9) COMP-5 VALUE -20142.
      * ENCRYPTION PASSWORD special register not set
       77  SQL-RC-E20143             PIC S9(9) COMP-5 VALUE -20143.
      * invalid length for encryption password
       77  SQL-RC-E20144             PIC S9(9) COMP-5 VALUE -20144.
      * decryption key does not match encryption key
       77  SQL-RC-E20145             PIC S9(9) COMP-5 VALUE -20145.
      * DECRYPT failed because data is not encrypted
       77  SQL-RC-E20146             PIC S9(9) COMP-5 VALUE -20146.
      * ENCRYPT function failed
       77  SQL-RC-E20147             PIC S9(9) COMP-5 VALUE -20147.
      * routine must end with RETURN statement
       77  SQL-RC-E20148             PIC S9(9) COMP-5 VALUE -20148.
      * block pages too large for the buffer pool size
       77  SQL-RC-E20150             PIC S9(9) COMP-5 VALUE -20150.
      * BLOCKSIZE value is not in the valid range
       77  SQL-RC-E20151             PIC S9(9) COMP-5 VALUE -20151.
      * specified buffer pool is not block-based
       77  SQL-RC-E20152             PIC S9(9) COMP-5 VALUE -20152.
      * database split image is suspended
       77  SQL-RC-E20153             PIC S9(9) COMP-5 VALUE -20153.
      * Insert into view not allowed target table cannot be determined
       77  SQL-RC-E20154             PIC S9(9) COMP-5 VALUE -20154.
      * event monitor target table invalid
       77  SQL-RC-E20155             PIC S9(9) COMP-5 VALUE -20155.
      * User does not have QUIESCE_CONNECT privilege
       77  SQL-RC-E20157             PIC S9(9) COMP-5 VALUE -20157.
      * Function not supported for level of DLM
       77  SQL-RC-E20158             PIC S9(9) COMP-5 VALUE -20158.
      * cannot use block-based and extended storage
       77  SQL-RC-E20162             PIC S9(9) COMP-5 VALUE -20162.
      * SQL data change stmt not allowed
       77  SQL-RC-E20165             PIC S9(9) COMP-5 VALUE -20165.
      * Speicifed view is not a symmetric view
       77  SQL-RC-E20166             PIC S9(9) COMP-5 VALUE -20166.
      * Virtual storage or database resource is not available.
       77  SQL-RC-E20167             PIC S9(9) COMP-5 VALUE -20167.
      * The ALTER BUFFERPOOL statement is currently in progress.
       77  SQL-RC-E20168             PIC S9(9) COMP-5 VALUE -20168.
      * There is not enough space in the table space for the specified
      * action.
       77  SQL-RC-E20170             PIC S9(9) COMP-5 VALUE -20170.
      * view already has INSTEAD OF trigger defined
       77  SQL-RC-E20178             PIC S9(9) COMP-5 VALUE -20178.
      * trigger not created because view defined WITH CHECK OPTION
       77  SQL-RC-E20179             PIC S9(9) COMP-5 VALUE -20179.

      * primary or unique key is a subset of columns in dimensions clause
       77  SQL-RC-E20188             PIC S9(9) COMP-5 VALUE -20188.
      * Federated operation not compiled due to potential inconsistency
       77  SQL-RC-E20190             PIC S9(9) COMP-5 VALUE -20190.
      * the same host variable must be used in both USING and INTO clauses
       77  SQL-RC-E20191             PIC S9(9) COMP-5 VALUE -20191.
      * the requested feature is not supported in this environment
       77  SQL-RC-E20192             PIC S9(9) COMP-5 VALUE -20192.
      * error has occurred when accessing a file with reason-code
       77  SQL-RC-E20193             PIC S9(9) COMP-5 VALUE -20193.
      * buffer pool does not exist on database partition
       77  SQL-RC-E20194             PIC S9(9) COMP-5 VALUE -20194.
      * error encountered while processing the path rename config file
       77  SQL-RC-E20195             PIC S9(9) COMP-5 VALUE -20195.
      * one or more built-in types do not match corresponding built-in types
       77  SQL-RC-E20196             PIC S9(9) COMP-5 VALUE -20196.
      * cannot define method as an overriding method
       77  SQL-RC-E20197             PIC S9(9) COMP-5 VALUE -20197.
      * method calls itself recursively
       77  SQL-RC-E20198             PIC S9(9) COMP-5 VALUE -20198.
      * URL not found
       77  SQL-RC-E20200             PIC S9(9) COMP-5 VALUE -20200.
      * jar name invalid
       77  SQL-RC-E20201             PIC S9(9) COMP-5 VALUE -20201.
      * class is in use
       77  SQL-RC-E20202             PIC S9(9) COMP-5 VALUE -20202.
      * Java method has invalid signature
       77  SQL-RC-E20203             PIC S9(9) COMP-5 VALUE -20203.
      * function unable to map to single method
       77  SQL-RC-E20204             PIC S9(9) COMP-5 VALUE -20204.
      * null value not allowed in argument
       77  SQL-RC-E20205             PIC S9(9) COMP-5 VALUE -20205.
      * unsupported deployment descriptor
       77  SQL-RC-E20207             PIC S9(9) COMP-5 VALUE -20207.
      * Table used to define a staging table is not valid
       77  SQL-RC-E20208             PIC S9(9) COMP-5 VALUE -20208.
      * Option not valid for table with reason-code
       77  SQL-RC-E20209             PIC S9(9) COMP-5 VALUE -20209.

      * table-designator does not contain ORDER BY
       77  SQL-RC-E20210             PIC S9(9) COMP-5 VALUE -20210.
      * ORDER BY and FETCH FIRST n ROWS ONLY is invalid
       77  SQL-RC-E20211             PIC S9(9) COMP-5 VALUE -20211.
      * User defined routine encountered an exception while loading java
      * class
       77  SQL-RC-E20212             PIC S9(9) COMP-5 VALUE -20212.
      * ORDER OF specified but table designator not ordered
       77  SQL-RC-E20214             PIC S9(9) COMP-5 VALUE -20214.
      * Encryption facility not available
       77  SQL-RC-E20223             PIC S9(9) COMP-5 VALUE -20223.
      * Procedure name may not be specified by a host variable in the CALL
      * statement
       77  SQL-RC-E20230             PIC S9(9) COMP-5 VALUE -20230.
      * Table defined as CCSID UNICODE
       77  SQL-RC-E20238             PIC S9(9) COMP-5 VALUE -20238.
      * Table cannot be typed, or contain graphic or user-defined types
       77  SQL-RC-E20239             PIC S9(9) COMP-5 VALUE -20239.
      * Unable to write a history file entry
       77  SQL-RC-E20241             PIC S9(9) COMP-5 VALUE -20241.
      * Sample size in clause is invalid
       77  SQL-RC-E20242             PIC S9(9) COMP-5 VALUE -20242.
      * The view is missing the INSTEAD OF triggers
       77  SQL-RC-E20243             PIC S9(9) COMP-5 VALUE -20243.
      * BEFORE TRIGGER or GENERATED COLUMN would cause table to be
      * delete-connected with ovelapping set null rules
       77  SQL-RC-E20253             PIC S9(9) COMP-5 VALUE -20253.
      * FOREIGN KEY would cause table to be delete-connected to itself
       77  SQL-RC-E20254             PIC S9(9) COMP-5 VALUE -20254.
      * FOREIGN KEY would cause a descent table to be delete-connected to
      * ancestor table
       77  SQL-RC-E20255             PIC S9(9) COMP-5 VALUE -20255.
      * FOREIGN KEY would cause two tables to be delete-connected
       77  SQL-RC-E20256             PIC S9(9) COMP-5 VALUE -20256.
      * FINAL TABLE is not valid
       77  SQL-RC-E20257             PIC S9(9) COMP-5 VALUE -20257.
      * Invalid use of INPUT SEQUENCE ordering
       77  SQL-RC-E20258             PIC S9(9) COMP-5 VALUE -20258.
      * column cannot be specified in the select list of query 
       77  SQL-RC-E20259             PIC S9(9) COMP-5 VALUE -20259.
      * assignment clause of UPDATE stmt must specify at least one column
      * that is not an INCLUDE column
       77  SQL-RC-E20260             PIC S9(9) COMP-5 VALUE -20260.
      * Invalid row movement to table
       77  SQL-RC-E20261             PIC S9(9) COMP-5 VALUE -20261.
      * Invalid usage of WITH ROW MOVEMENT in a view
       77  SQL-RC-E20262             PIC S9(9) COMP-5 VALUE -20262.
      * Invalid attempt to update view
       77  SQL-RC-E20263             PIC S9(9) COMP-5 VALUE -20263.
      * A function is invoked in an illegal context
       77  SQL-RC-E20267             PIC S9(9) COMP-5 VALUE -20267.
      * A nickname cannot be referenced in an enforced referential
      * constraint
       77  SQL-RC-E20269             PIC S9(9) COMP-5 VALUE -20269.
      * Nickname stats cannot be updated
       77  SQL-RC-E20273             PIC S9(9) COMP-5 VALUE -20273.
      * XML name is not valid
       77  SQL-RC-E20275             PIC S9(9) COMP-5 VALUE -20275.
      * XML namespace is not valid
       77  SQL-RC-E20276             PIC S9(9) COMP-5 VALUE -20276.
      * View cannot be enabled for query optimization
       77  SQL-RC-E20279             PIC S9(9) COMP-5 VALUE -20279.
      * Unable to load .NET class
       77  SQL-RC-E20282             PIC S9(9) COMP-5 VALUE -20282.
      * Unable to create plan
       77  SQL-RC-E20284             PIC S9(9) COMP-5 VALUE -20284.
      * Stats could not be updated
       77  SQL-RC-E20288             PIC S9(9) COMP-5 VALUE -20288.
      * Routine cannot be run on the specified partition
       77  SQL-RC-E20290             PIC S9(9) COMP-5 VALUE -20290.
      * Invalid compilation environment
       77  SQL-RC-E20316             PIC S9(9) COMP-5 VALUE -20316.
      * Automatic storage not defined for database
       77  SQL-RC-E20317             PIC S9(9) COMP-5 VALUE -20317.
      * Cannot perform container operations against table space
       77  SQL-RC-E20318             PIC S9(9) COMP-5 VALUE -20318.
      * Redirected restore of automatic storage table space not allowed
       77  SQL-RC-E20319             PIC S9(9) COMP-5 VALUE -20319.
      * Maximum size for table space not valid
       77  SQL-RC-E20320             PIC S9(9) COMP-5 VALUE -20320.
      * Storage paths cannot be specified
       77  SQL-RC-E20321             PIC S9(9) COMP-5 VALUE -20321.
      * Database name does not match current server
       77  SQL-RC-E20322             PIC S9(9) COMP-5 VALUE -20322.
      * Storage path is a duplicate
       77  SQL-RC-E20323             PIC S9(9) COMP-5 VALUE -20323.
      * Operation cannot occur multiple times in transaction
       77  SQL-RC-E20324             PIC S9(9) COMP-5 VALUE -20324.
      * Maximum size of table space exceeded
       77  SQL-RC-E20325             PIC S9(9) COMP-5 VALUE -20325.
      * Operation violated an integrity constraint
       77  SQL-RC-E20333             PIC S9(9) COMP-5 VALUE -20333.
      * Text Information Extender incorrectly configured
       77  SQL-RC-E21000             PIC S9(9) COMP-5 VALUE -21000.

      * The funtion or feature name is invalid
       77  SQL-RC-E22400             PIC S9(9) COMP-5 VALUE -22400.
      * Application ID does not exist
       77  SQL-RC-E22401             PIC S9(9) COMP-5 VALUE -22401.
      * No activity monitor reports exist
       77  SQL-RC-E22402             PIC S9(9) COMP-5 VALUE -22402.
      * Invalid values specified
       77  SQL-RC-E22403             PIC S9(9) COMP-5 VALUE -22403.
      * Specified action mode is invalid
       77  SQL-RC-E22404             PIC S9(9) COMP-5 VALUE -22404.
      * Unable to collect snapshot data
       77  SQL-RC-E22405             PIC S9(9) COMP-5 VALUE -22405.

      * Distribution protocol error, no disconnect
       77  SQL-RC-E30000             PIC S9(9) COMP-5 VALUE -30000.
      * statement cannot be executed due to prior condition
       77  SQL-RC-E30002             PIC S9(9) COMP-5 VALUE -30002.
      * Execution failed; function not supported
       77  SQL-RC-E30005             PIC S9(9) COMP-5 VALUE -30005.
      * Distribution protocol error, disconnect
       77  SQL-RC-E30020             PIC S9(9) COMP-5 VALUE -30020.
      * Distribution compatibility error
       77  SQL-RC-E30021             PIC S9(9) COMP-5 VALUE -30021.
      * Distribution memory allocation err
       77  SQL-RC-E30040             PIC S9(9) COMP-5 VALUE -30040.
      * Distribution memory allocation err, disconnect
       77  SQL-RC-E30041             PIC S9(9) COMP-5 VALUE -30041.
      * Invalid command while bind in prog
       77  SQL-RC-E30050             PIC S9(9) COMP-5 VALUE -30050.
      * Bind not active
       77  SQL-RC-E30051             PIC S9(9) COMP-5 VALUE -30051.
      * Bind Owner authorization failure
       77  SQL-RC-E30053             PIC S9(9) COMP-5 VALUE -30053.
      * RDB authorization failure
       77  SQL-RC-E30060             PIC S9(9) COMP-5 VALUE -30060.
      * RDB not found
       77  SQL-RC-E30061             PIC S9(9) COMP-5 VALUE -30061.
      * Distribution command error
       77  SQL-RC-E30070             PIC S9(9) COMP-5 VALUE -30070.
      * Distribution object error
       77  SQL-RC-E30071             PIC S9(9) COMP-5 VALUE -30071.
      * Distribution parameter error
       77  SQL-RC-E30072             PIC S9(9) COMP-5 VALUE -30072.
      * Distribution parameter value error
       77  SQL-RC-E30073             PIC S9(9) COMP-5 VALUE -30073.
      * Distribution reply error
       77  SQL-RC-E30074             PIC S9(9) COMP-5 VALUE -30074.
      * Communication error
       77  SQL-RC-E30080             PIC S9(9) COMP-5 VALUE -30080.
      * Communication error
       77  SQL-RC-E30081             PIC S9(9) COMP-5 VALUE -30081.
      * Security error
       77  SQL-RC-E30082             PIC S9(9) COMP-5 VALUE -30082.
      * Attempt to change password failed
       77  SQL-RC-E30083             PIC S9(9) COMP-5 VALUE -30083.
      * Remote operation invalid
       77  SQL-RC-E30090             PIC S9(9) COMP-5 VALUE -30090.

      * A bind option is invalid
       77  SQL-RC-E30104             PIC S9(9) COMP-5 VALUE -30104.
      * Invalid input for mulitple row INSERT
       77  SQL-RC-E30106             PIC S9(9) COMP-5 VALUE -30106.
      * A failed connection has been re-established
       77  SQL-RC-E30108             PIC S9(9) COMP-5 VALUE -30108.
      * Database Monitor SQLCODES

      * bad path specified for event monitor
       77  SQLM-RC-BAD-PATH          PIC S9(4) COMP-5 VALUE -1612.
      * bad OPTION specified for event monitor
       77  SQLM-RC-BAD-OPTION        PIC S9(4) COMP-5 VALUE -1613.
      * I/O error on activating event monitor
       77  SQLM-RC-IO-ERROR          PIC S9(4) COMP-5 VALUE -1614.
      * Event monitor not activated
       77  SQLM-RC-NOT-ACTIVATED     PIC S9(4) COMP-5 VALUE -1616.
      * Event monitor data files are full
       77  SQLM-RC-EVMON-FULL        PIC S9(4) COMP-5 VALUE -1617.
      * Event monitor path is in use
       77  SQLM-RC-PATH-IN-USE       PIC S9(4) COMP-5 VALUE -1618.
      * Cannot drop active event monitor
       77  SQLM-RC-CANNOT-DROP       PIC S9(4) COMP-5 VALUE -1619.
      * Cannot flush event monitor
       77  SQLM-RC-CANNOT-FLUSH      PIC S9(4) COMP-5 VALUE -1620.
      * Must commit to use event monitor
       77  SQLM-RC-MUST-COMMIT       PIC S9(4) COMP-5 VALUE -1621.
      * Invalid state
       77  SQLM-RC-BAD-STATE         PIC S9(4) COMP-5 VALUE -1622.

