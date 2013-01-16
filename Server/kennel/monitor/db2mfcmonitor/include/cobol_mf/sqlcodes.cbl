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
       78  SQL-RC-W012               VALUE    12.
      * unsupported bind/prep options
       78  SQL-RC-W020               VALUE    20.
      * eof
       78  SQL-RC-W100               VALUE    100.

      * wrong nbr of insert values
       78  SQL-RC-W117               VALUE    117.
      * duplicate column conistraint
       78  SQL-RC-W139               VALUE    139.
      * invalid syntax ignored
       78  SQL-RC-W143               VALUE    143.

      * undefined name
       78  SQL-RC-W204               VALUE    204.
      * not a column
       78  SQL-RC-W205               VALUE    205.
      * not a column of referenced tables
       78  SQL-RC-W206               VALUE    206.
      * explain mode incompatible
       78  SQL-RC-W217               VALUE    217.
      * hole detected during fetch of cursor
       78  SQL-RC-W222               VALUE    222.
      * cursor position prevents FETCH current row
       78  SQL-RC-W231               VALUE    231.
      * not enough sqlvars, none filled in
       78  SQL-RC-W236               VALUE    236.
      * distinct type: not enough sqlvars, some filled in
       78  SQL-RC-W237               VALUE    237.
      * LOB type: not enough sqlvars, none filled in
       78  SQL-RC-W238               VALUE    238.
      * distinct type: not enough sqlvars, none filled in
       78  SQL-RC-W239               VALUE    239.
      * new view replaced old
       78  SQL-RC-W280               VALUE    280.
      * error accessing container
       78  SQL-RC-W293               VALUE    293.

      * possible infinite loop
       78  SQL-RC-W347               VALUE    347.
      * DATALINK value may not be valid
       78  SQL-RC-W360               VALUE    360.
      * SQLSTATE or SQLCODE may be over-written
       78  SQL-RC-W385               VALUE    385.

      * alias target is not defined
       78  SQL-RC-W403               VALUE    403.
      * invalid data type for column
       78  SQL-RC-W408               VALUE    408.
      * clause value has been replaced
       78  SQL-RC-W434               VALUE    434.
      * Sub-optimal query
       78  SQL-RC-W437               VALUE    437.
      * No function with compatible arg
       78  SQL-RC-W440               VALUE    440.
      * Function has truncated value
       78  SQL-RC-W445               VALUE    445.
      * Create UDF contains redundant keyw
       78  SQL-RC-W447               VALUE    447.
      * Procedure returned too many query result sets
       78  SQL-RC-W464               VALUE    464.
      * UDF returns a warning sqlstate
       78  SQL-RC-W462               VALUE    462.
      * one or more results sets for stored procedure
       78  SQL-RC-W466               VALUE    466.
      * Another result set exists for a stored procedure
       78  SQL-RC-W467               VALUE    467.
      * DDL:could truncate value at runtime
       78  SQL-RC-W474               VALUE    474.
      * DDL:could truncate value at runtime
       78  SQL-RC-W477               VALUE    477.
      * Number of result sets greater than number of locators
       78  SQL-RC-W494               VALUE    494.

      * duplicate referential constraint
       78  SQL-RC-W541               VALUE    541.
      * authorization error w/obj insert
       78  SQL-RC-W551               VALUE    551.
      * auth error w/o obj ins
       78  SQL-RC-W552               VALUE    552.
      * revoke stmt denied--priv not held
       78  SQL-RC-W556               VALUE    556.
      * revoke stmt denied--has CONTROL
       78  SQL-RC-W558               VALUE    558.
      * some privileges were not granted
       78  SQL-RC-W570               VALUE    570.
      * Duplicate schema name     
       78  SQL-RC-W585               VALUE    585.
      * Isolation level escalated
       78  SQL-RC-W595               VALUE    595.
      * existing index used
       78  SQL-RC-W598               VALUE    598.
      * compare func not created for lstr
       78  SQL-RC-W599               VALUE    599.

      * index already exists
       78  SQL-RC-W605               VALUE    605.

      * set stmt not supported
       78  SQL-RC-W799               VALUE    799.

      * duplicate row warning
       78  SQL-RC-W803               VALUE    803.

      * unknown SQL warning another product
       78  SQL-RC-W965               VALUE    965.
      * XA informational message
       78  SQL-RC-W997               VALUE    997.

      * already existing index was migrated
       78  SQL-RC-W1138              VALUE    1138.
      * estimate cost exceeds resource limit warning threshold
       78  SQL-RC-W1140              VALUE    1140.

      * DataLink column not defined on DLM 
       78  SQL-RC-W1161              VALUE    1161.
      * DLM down during exception processing 
       78  SQL-RC-W1162              VALUE    1162.

      * value not within host variable data type range
       78  SQL-RC-W1165              VALUE    1165.
      * Division by zero
       78  SQL-RC-W1166              VALUE    1166.
      * Arithmetic overflow or arithmetic exception
       78  SQL-RC-W1167              VALUE    1167.
      * object may require invoker to have privileges on data source object
       78  SQL-RC-W1179              VALUE    1179.
      * DLM down during backup
       78  SQL-RC-W1196              VALUE    1196.

      * XA no data to return from recover
       78  SQL-RC-W1251              VALUE    1251.
      * Parameter not changed dynamically
       78  SQL-RC-W1362              VALUE    1362.
      * Parameter not changed dynamically
       78  SQL-RC-W1363              VALUE    1363.
      * Parameter does not support AUTOMATIC
       78  SQL-RC-W1364              VALUE    1364.

      * WITH GRANT OPTION ignored
       78  SQL-RC-W1440              VALUE    1440.
      * system error on DB termination
       78  SQL-RC-ERR-DB-TERM        VALUE    1475.
      * Invalid cursor position
       78  SQL-RC-W1479              VALUE    1479.

      * value of DEGREE ignored
       78  SQL-RC-W1530              VALUE    1530.
      * Trailing blanks are truncated
       78  SQL-RC-W1580              VALUE    1580.
      * data remains unverified
       78  SQL-RC-W1594              VALUE    1594.

      * Event monitor state not changed
       78  SQLM-RC-NOT-SET           VALUE    1615.
      * Monitor: No conversion from source cp to target cp
       78  SQL-RC-W1625              VALUE    1625.
      * Monitor: code page conversion overflow
       78  SQL-RC-W1626              VALUE    1626.
      * Monitor: V5 snapshot returned on V6 request
       78  SQL-RC-W1627              VALUE    1627.
      * Monitor: A remote snapshot operation failed
       78  SQL-RC-W1628              VALUE    1628.
      * Monitor: A get switches operation ran out of buffer space
       78  SQL-RC-W1629              VALUE    1629.

      * containers not designated for specific nodes are not used
       78  SQL-RC-W1758              VALUE    1758.
      * Redistribute required to change data partitioning
       78  SQL-RC-W1759              VALUE    1759.
      * Index creation, recreation, or reorganization may not be recovered
      * on secondary database server 
       78  SQL-RC-W1765              VALUE    1765.
      * LOGINDEXBUILD has not been enabled 
       78  SQL-RC-W1766              VALUE    1766.
      * No default primary table space exists
       78  SQL-RC-W1790              VALUE    1790.
      * The stats for the specified nicknames were not updated
       78  SQL-RC-W1792              VALUE    1792.

      * LOB value retrieved may have been changed
       78  SQL-RC-W1821              VALUE    1821.
      * base tables of UNION ALL may be same table
       78  SQL-RC-W1824              VALUE    1824.
      * Data for a column was truncated
       78  SQL-RC-W1844              VALUE    1844.

      * Reconcile pending on some DLMs
       78  SQL-RC-W2077              VALUE    2077.

      * Statistics are in an inconsistent state
       78  SQL-RC-W2314              VALUE    2314.
      * The statistics profile has exceed the max size
       78  SQL-RC-W2316              VALUE    2316.

      * The database was not catalogued in LDAP
       78  SQL-RC-W3274              VALUE    3274.
      * The database was not uncatalogued in LDAP
       78  SQL-RC-W3275              VALUE    3275.
      * Protocol info was not updated in LDAP
       78  SQL-RC-W3283              VALUE    3283.

      * SQL procedure binary not in catalog
       78  SQL-RC-W7035              VALUE    7035.

      * evaluation period started
       78  SQL-RC-W8006              VALUE    8006.
      * db2 is in evaluation period
       78  SQL-RC-W8007              VALUE    8007.
      * concurrent user limit exceeded - DB2 Workgroup
       78  SQL-RC-W8009              VALUE    8009.
      * concurrent user limit exceeded - DB2 Connect
       78  SQL-RC-W8010              VALUE    8010.
      * concurrent user limit exceeded - DB2 Enterprise
       78  SQL-RC-W8012              VALUE    8012.
      * concurrent connection limit exceeded - DB2 Connect
       78  SQL-RC-W8013              VALUE    8013.
      * processor license limit exceeded
       78  SQL-RC-W8017              VALUE    8017.
      * concurrent user license limit exceeded
       78  SQL-RC-W8018              VALUE    8018.
      * concurrent connectors limit exceeded
       78  SQL-RC-W8020              VALUE    8020.
      * this connector is not registered
       78  SQL-RC-W8021              VALUE    8021.

      * transform for type not defined
       78  SQL-RC-W20015             VALUE    20015.
      * summary table cannot be used to optimize query
       78  SQL-RC-W20059             VALUE    20059.
      * DATALINK attribute only for typed table or type view
       78  SQL-RC-W20090             VALUE    20090.

      * Error in debugger support
       78  SQL-RC-W20109             VALUE    20109.
      * Column is not long enough
       78  SQL-RC-W20114             VALUE    20114.
      * ABBREVIATE column attribute ignored
       78  SQL-RC-W20140             VALUE    20140.
      * buffer pool configuration complete
       78  SQL-RC-W20149             VALUE    20149.
      * Event Monitor activated; info may be lost
       78  SQL-RC-W20156             VALUE    20156.
      * Isolation clause is ignored.
       78  SQL-RC-W20159             VALUE    20159.
      * Authorization granted to USER
       78  SQL-RC-W20160             VALUE    20160.
      * Column name is invalid for event monitor table
       78  SQL-RC-W20161             VALUE    20161.
      * The buffer pool is not started.
       78  SQL-RC-W20169             VALUE    20169.
      * Event monitor created; target tables already exists
       78  SQL-RC-W20173             VALUE    20173.
      * Insufficient memory; bufferpool operation will not take effect.
       78  SQL-RC-W20189             VALUE    20189.

      * procedure returned too many results
       78  SQL-RC-W20206             VALUE    20206.
      * The buffer pool drop operation will not take effect immediately
       78  SQL-RC-W20225             VALUE    20225.
      * The name was truncated
       78  SQL-RC-W20271             VALUE    20271.
      * Some nickname stats cannot be updated
       78  SQL-RC-W20274             VALUE    20274.
      * Characters truncated during code page conversion
       78  SQL-RC-W20277             VALUE    20277.
      * View cannot be used to optimize query processing
       78  SQL-RC-W20278             VALUE    20278.
      * The specified cached statement is different that the current
      * environment
       78  SQL-RC-W20287             VALUE    20287.
      * Insufficient permission to create or write to file
       78  SQL-RC-W20280             VALUE    20280.

      * Rebind options ignored
       78  SQL-RC-W30101             VALUE    30101.
      * Connection disabled
       78  SQL-RC-W30102             VALUE    30102.

      * ------------ errors ------------
      * illegal character
       78  SQL-RC-E007               VALUE    -7.
      * string constant not terminated
       78  SQL-RC-E010               VALUE    -10.
      * cursor or statement name = ""
       78  SQL-RC-E013               VALUE    -13.
      * function or method does not end with RETURN
       78  SQL-RC-E017               VALUE    -17.
      * INTO clause required
       78  SQL-RC-E029               VALUE    -29.
      * maximum SQL statements
       78  SQL-RC-E051               VALUE    -51.
      * SQLSTATE and SQLCODE declaration in wrong scope
       78  SQL-RC-E056               VALUE    -56.
      * RETURN statement must include a value
       78  SQL-RC-E057               VALUE    -57.
      * data type for RETURN value must be INTEGER
       78  SQL-RC-E058               VALUE    -58.
      * parameter names not specified for routine
       78  SQL-RC-E078               VALUE    -78.
      * global temp table schema must be SESSION
       78  SQL-RC-E079               VALUE    -79.
      * bad SQL statement
       78  SQL-RC-E084               VALUE    -84.
      * data type not supported in SQL routine
       78  SQL-RC-E097               VALUE    -97.

      * statement too long
       78  SQL-RC-E101               VALUE    -101.
      * string constant too long
       78  SQL-RC-E102               VALUE    -102.
      * invalid numeric literal
       78  SQL-RC-E103               VALUE    -103.
      * invalid character/token
       78  SQL-RC-E104               VALUE    -104.
      * invalid string constant
       78  SQL-RC-E105               VALUE    -105.
      * incomplete statement
       78  SQL-RC-E106               VALUE    -106.
      * name too long
       78  SQL-RC-E107               VALUE    -107.
      * name has too many qualifiers
       78  SQL-RC-E108               VALUE    -108.
      * clause not permitted
       78  SQL-RC-E109               VALUE    -109.
      * invalid hex constant
       78  SQL-RC-E110               VALUE    -110.
      * no column name
       78  SQL-RC-E111               VALUE    -111.
      * operand is SQL function
       78  SQL-RC-E112               VALUE    -112.
      * identifier contains invalid character
       78  SQL-RC-E113               VALUE    -113.
      * wrong nbr of insert values
       78  SQL-RC-E117               VALUE    -117.
      * object table in from clause
       78  SQL-RC-E118               VALUE    -118.
      * column not in group by
       78  SQL-RC-E119               VALUE    -119.
      * clause includes SQL fn
       78  SQL-RC-E120               VALUE    -120.
      * dup column name
       78  SQL-RC-E121               VALUE    -121.
      * no group by
       78  SQL-RC-E122               VALUE    -122.
      * parameter must be constant
       78  SQL-RC-NOTCONST           VALUE    -123.
      * no result column
       78  SQL-RC-E125               VALUE    -125.
      * dup distinct
       78  SQL-RC-E127               VALUE    -127.
      * too many table names
       78  SQL-RC-E129               VALUE    -129.
      * invalid escape character
       78  SQL-RC-INVESC             VALUE    -130.
      * incompatible data for like
       78  SQL-RC-E131               VALUE    -131.
      * like predicate invalid
       78  SQL-RC-E132               VALUE    -132.
      * improper use of long string
       78  SQL-RC-E134               VALUE    -134.
      * input must be a host variable or NULL
       78  SQL-RC-E135               VALUE    -135.
      * length of concat too long
       78  SQL-RC-E137               VALUE    -137.
      * substr arg out of range
       78  SQL-RC-E138               VALUE    -138.
      * syntax not supported
       78  SQL-RC-E142               VALUE    -142.
      * unsupported ANSI syntax
       78  SQL-RC-E146               VALUE    -146.
      * view not updatable
       78  SQL-RC-E150               VALUE    -150.
      * column not updatable
       78  SQL-RC-E151               VALUE    -151.
      * actual constraint type is not expected constraint type
       78  SQL-RC-E152               VALUE    -152.
      * no column list
       78  SQL-RC-E153               VALUE    -153.
      * trigger trans tbl not modifiable
       78  SQL-RC-E155               VALUE    -155.
      * command not allowed on view
       78  SQL-RC-E156               VALUE    -156.
      * view name in foreign key
       78  SQL-RC-E157               VALUE    -157.
      * nbr of columns does not match
       78  SQL-RC-E158               VALUE    -158.
      * drop view on table
       78  SQL-RC-E159               VALUE    -159.
      * with check not allowed on view
       78  SQL-RC-E160               VALUE    -160.
      * with check violation
       78  SQL-RC-E161               VALUE    -161.
      * nbr of arguments invalid
       78  SQL-RC-E170               VALUE    -170.
      * argument invalid
       78  SQL-RC-E171               VALUE    -171.
      * function name invalid
       78  SQL-RC-E172               VALUE    -172.
      * translate scalar argument invalid
       78  SQL-RC-E176               VALUE    -176.
      * datetime syntax invalid
       78  SQL-RC-E180               VALUE    -180.
      * datetime value invalid
       78  SQL-RC-E181               VALUE    -181.
      * datetime arithmetic invalid
       78  SQL-RC-E182               VALUE    -182.
      * datetime arithmetic out of range
       78  SQL-RC-E183               VALUE    -183.
      * datetime register is invalid
       78  SQL-RC-DTMEREG            VALUE    -187.
      * data type or length of column incompatible
       78  SQL-RC-E190               VALUE    -190.
      * truncated DBCS character found
       78  SQL-RC-DBCSTRUNC          VALUE    -191.
      * NOT NULL needs DEFAULT
       78  SQL-RC-E193               VALUE    -193.
      * no qualified columns in ORDER BY
       78  SQL-RC-E197               VALUE    -197.
      * no statement text
       78  SQL-RC-E198               VALUE    -198.
      * illegal use of reserved word
       78  SQL-RC-E199               VALUE    -199.

      * ambiguous column reference
       78  SQL-RC-E203               VALUE    -203.
      * undefined name
       78  SQL-RC-E204               VALUE    -204.
      * not a column
       78  SQL-RC-E205               VALUE    -205.
      * not a column of referenced tables
       78  SQL-RC-E206               VALUE    -206.
      * cannot orderby column name w/union
       78  SQL-RC-E207               VALUE    -207.
      * column not part of result table
       78  SQL-RC-E208               VALUE    -208.
      * duplicate table designator
       78  SQL-RC-E212               VALUE    -212.
      * bad expression in group/by or order by
       78  SQL-RC-E214               VALUE    -214.
      * number of elements does not match
       78  SQL-RC-E216               VALUE    -216.
      * required explain table not exist
       78  SQL-RC-E219               VALUE    -219.
      * explain table has improper def
       78  SQL-RC-E220               VALUE    -220.
      * update or delete against a hole
       78  SQL-RC-E222               VALUE    -222.
      * result table does not agree with base table
       78  SQL-RC-E224               VALUE    -224.
      * FETCH against non-scrollable cursor
       78  SQL-RC-E225               VALUE    -225.
      * cursor has unknown position
       78  SQL-RC-E227               VALUE    -227.
      * FOR UPDATE specified for read-only cursor
       78  SQL-RC-E228               VALUE    -228.
      * duplicate object in list
       78  SQL-RC-E242               VALUE    -242.
      * SENSITIVE cursor cannot be defined for SELECT
       78  SQL-RC-E243               VALUE    -243.
      * SENSITIVITY not valid for cursor
       78  SQL-RC-E244               VALUE    -244.
      * raw device containers not allowed
       78  SQL-RC-E257               VALUE    -257.
      * cannot add container to pool
       78  SQL-RC-E258               VALUE    -258.
      * container map too big
       78  SQL-RC-E259               VALUE    -259.
      * partition key has long field
       78  SQL-RC-E260               VALUE    -260.
      * node in use, cannot drop
       78  SQL-RC-E261               VALUE    -261.
      * multinode table without part key
       78  SQL-RC-E262               VALUE    -262.
      * invalid node range
       78  SQL-RC-E263               VALUE    -263.
      * multinode tbl, cannot drop part
       78  SQL-RC-E264               VALUE    -264.
      * duplicate node name/number
       78  SQL-RC-E265               VALUE    -265.
      * node not defined
       78  SQL-RC-E266               VALUE    -266.
      * operatn not done because rebalance
       78  SQL-RC-E268               VALUE    -268.
      * too many nodegroups
       78  SQL-RC-E269               VALUE    -269.
      * function not supported
       78  SQL-RC-E270               VALUE    -270.
      * Index file missing or invalid
       78  SQL-RC-E271               VALUE    -271.
      * database in restore pending state
       78  SQL-RC-E276               VALUE    -276.
      * connection terminated during COMMIT
       78  SQL-RC-E279               VALUE    -279.
      * cannot add containers to tablespace
       78  SQL-RC-E281               VALUE    -281.
      * cannot drop tblspace, tbl conflict
       78  SQL-RC-E282               VALUE    -282.
      * cannot drop only temp tablespace
       78  SQL-RC-E283               VALUE    -283.
      * invalid tablespace type for clause
       78  SQL-RC-E284               VALUE    -284.
      * all table parts must be in tblspce
       78  SQL-RC-E285               VALUE    -285.
      * need default tblspce for new tables
       78  SQL-RC-E286               VALUE    -286.
      * SYSCATSPACE not for user objects
       78  SQL-RC-E287               VALUE    -287.
      * long tablespace cannot use system
       78  SQL-RC-E288               VALUE    -288.
      * cannot allocate new pages in tablespace
       78  SQL-RC-E289               VALUE    -289.
      * access to tablespace not allowed
       78  SQL-RC-E290               VALUE    -290.
      * invalid state transition
       78  SQL-RC-E291               VALUE    -291.
      * cannot create internal db file
       78  SQL-RC-E292               VALUE    -292.
      * error accessing container
       78  SQL-RC-E293               VALUE    -293.
      * container already in use
       78  SQL-RC-E294               VALUE    -294.
      * container names too long
       78  SQL-RC-E295               VALUE    -295.
      * tablespace limit exceeded
       78  SQL-RC-E296               VALUE    -296.
      * container pathname too long
       78  SQL-RC-E297               VALUE    -297.
      * bad container pathname
       78  SQL-RC-E298               VALUE    -298.
      * container already added
       78  SQL-RC-E299               VALUE    -299.

      * host variable has invalid type
       78  SQL-RC-E301               VALUE    -301.
      * host variable value too large
       78  SQL-RC-E302               VALUE    -302.
      * data types not comparable
       78  SQL-RC-E303               VALUE    -303.
      * value not in range of host var
       78  SQL-RC-E304               VALUE    -304.
      * host var cannot be null
       78  SQL-RC-E305               VALUE    -305.
      * host var limit reached
       78  SQL-RC-E308               VALUE    -308.
      * host var should not be null
       78  SQL-RC-E309               VALUE    -309.
      * length of host var is negative
       78  SQL-RC-E311               VALUE    -311.
      * unusable host variable
       78  SQL-RC-E312               VALUE    -312.
      * wrong nbr of host variables
       78  SQL-RC-E313               VALUE    -313.
      * no conversn source-cp to target-cp
       78  SQL-RC-E332               VALUE    -332.
      * conversion overflow
       78  SQL-RC-E334               VALUE    -334.
      * decimal scale must be zero
       78  SQL-RC-E336               VALUE    -336.
      * ON clause not valid for Outer Join
       78  SQL-RC-E338               VALUE    -338.
      * duplicate common table expression
       78  SQL-RC-E340               VALUE    -340.
      * cyclic ref between comm tbl exp
       78  SQL-RC-E341               VALUE    -341.
      * comm tbl exp use UNION ALL
       78  SQL-RC-E342               VALUE    -342.
      * col names required
       78  SQL-RC-E343               VALUE    -343.
      * columns must match exactly
       78  SQL-RC-E344               VALUE    -344.
      * cannot use GROUP BY or HAVING here
       78  SQL-RC-E345               VALUE    -345.
      * invalid reference to comm tbl exp
       78  SQL-RC-E346               VALUE    -346.
      * cannot use identity column or sequence
       78  SQL-RC-E348               VALUE    -348.
      * different sequence expressions in INSERT
       78  SQL-RC-E349               VALUE    -349.
      * LOB col cannot be idx, key, constr
       78  SQL-RC-E350               VALUE    -350.
      * LOB col cannot be selected by DRDA
       78  SQL-RC-E351               VALUE    -351.
      * LOB col cannot be inserted by DRDA
       78  SQL-RC-E352               VALUE    -352.
      * LOB col cannot be logged
       78  SQL-RC-E355               VALUE    -355.
      * DataLink Error
       78  SQL-RC-E357               VALUE    -357.
      * DataLink Error
       78  SQL-RC-E358               VALUE    -358.
      * Range exhausted
       78  SQL-RC-E359               VALUE    -359.
      * DataLink Error
       78  SQL-RC-E368               VALUE    -368.
      * parameter must be named in CREATE FUNCTION statement
       78  SQL-RC-E370               VALUE    -370.
      * One identity column per table
       78  SQL-RC-E372               VALUE    -372.
      * DEFAULT clause not allowed on identity column
       78  SQL-RC-E373               VALUE    -373.
      * clause not specified in CREATE FUNCTION
       78  SQL-RC-E374               VALUE    -374.
      * source and target are built-in types or same
       78  SQL-RC-E388               VALUE    -388.
      * invalid specific function instance in CREATE CAST
       78  SQL-RC-E389               VALUE    -389.
      * function is invalid in the context it is used
       78  SQL-RC-E390               VALUE    -390.
      * Invalid use of a row based function
       78  SQL-RC-E391               VALUE    -391.
      * SQLDA provided for fetch has been changed
       78  SQL-RC-E392               VALUE    -392.
      * Execute SQL statement during final call processing
       78  SQL-RC-E396               VALUE    -396.

      * operands not comparable
       78  SQL-RC-E401               VALUE    -401.
      * invalid type for arithmetic op
       78  SQL-RC-E402               VALUE    -402.
      * alias object undefined
       78  SQL-RC-E403               VALUE    -403.
      * update/insert string too long
       78  SQL-RC-E404               VALUE    -404.
      * numeric literal out of range
       78  SQL-RC-E405               VALUE    -405.
      * derived value out of range
       78  SQL-RC-E406               VALUE    -406.
      * column cannot be null
       78  SQL-RC-E407               VALUE    -407.
      * invalid data type for column
       78  SQL-RC-E408               VALUE    -408.
      * invalid operand for count
       78  SQL-RC-E409               VALUE    -409.
      * float literal too long
       78  SQL-RC-E410               VALUE    -410.
      * multiple columns in subquery
       78  SQL-RC-E412               VALUE    -412.
      * overflow during data conversion
       78  SQL-RC-E413               VALUE    -413.
      * incompatible data types for union
       78  SQL-RC-E415               VALUE    -415.
      * long string in union
       78  SQL-RC-E416               VALUE    -416.
      * invalid use of parameter markers
       78  SQL-RC-E417               VALUE    -417.
      * parameter marker in select clause
       78  SQL-RC-E418               VALUE    -418.
      * division produced negative scale
       78  SQL-RC-E419               VALUE    -419.
      * invalid character in input string
       78  SQL-RC-E420               VALUE    -420.
      * diff number of columns for union
       78  SQL-RC-E421               VALUE    -421.
      * Invalid handle
       78  SQL-RC-E423               VALUE    -423.
      * dynamic COMMIT not valid
       78  SQL-RC-E426               VALUE    -426.
      * dynamic ROLLBACK not valid
       78  SQL-RC-E427               VALUE    -427.
      * DISCONNECT in unit of work notallow
       78  SQL-RC-E428               VALUE    -428.
      * Handle table full
       78  SQL-RC-E429               VALUE    -429.
      * UDF abnormal end
       78  SQL-RC-E430               VALUE    -430.
      * UDF interrupted
       78  SQL-RC-E431               VALUE    -431.
      * Parameter marker cannot have udf nm
       78  SQL-RC-E432               VALUE    -432.
      * Value is too long
       78  SQL-RC-E433               VALUE    -433.
      * Inv SQLSTATE in RAISE_ERROR
       78  SQL-RC-E435               VALUE    -435.
      * C language char string missing NUL
       78  SQL-RC-E436               VALUE    -436.
      * App raised error
       78  SQL-RC-E438               VALUE    -438.
      * Error in UDF
       78  SQL-RC-E439               VALUE    -439.
      * No function with compatible arg
       78  SQL-RC-E440               VALUE    -440.
      * Invalid use of DISTINCT with scalar
       78  SQL-RC-E441               VALUE    -441.
      * Error refering function in DML
       78  SQL-RC-E442               VALUE    -442.
      * UDF returns error sqlstate
       78  SQL-RC-E443               VALUE    -443.
      * Unable to access UDF function
       78  SQL-RC-E444               VALUE    -444.
      * Exceed max number of parameters
       78  SQL-RC-E448               VALUE    -448.
      * Invalid EXTERNAL NAME format
       78  SQL-RC-E449               VALUE    -449.
      * UDF generates too long result value
       78  SQL-RC-E450               VALUE    -450.
      * DDL: invalid type for external UDF
       78  SQL-RC-E451               VALUE    -451.
      * Hostvar file inaccessible
       78  SQL-RC-E452               VALUE    -452.
      * Error in RETURNS and CAST FROM
       78  SQL-RC-E453               VALUE    -453.
      * Duplicate UDF name and signature
       78  SQL-RC-E454               VALUE    -454.
      * Diff schema for UDF name & specific
       78  SQL-RC-E455               VALUE    -455.
      * Duplicate specific name
       78  SQL-RC-E456               VALUE    -456.
      * Name reserved for system use
       78  SQL-RC-E457               VALUE    -457.
      * DDL refers UDF signature not found
       78  SQL-RC-E458               VALUE    -458.
      * AS CAST use error
       78  SQL-RC-E459               VALUE    -459.
      * invalid CAST
       78  SQL-RC-E461               VALUE    -461.
      * UDF returns an invalid sqlstate
       78  SQL-RC-E463               VALUE    -463.
      * Unable to start fenced UDF
       78  SQL-RC-E465               VALUE    -465.
      * Invalid use of IN, OUT, or INOUT parameter
       78  SQL-RC-E469               VALUE    -469.
      * Could not pass NULL argument to procedure
       78  SQL-RC-E470               VALUE    -470.
      * Error occurred calling a routine
       78  SQL-RC-E471               VALUE    -471.
      * Cursor left open by function or method
       78  SQL-RC-E472               VALUE    -472.
      * reserved object name
       78  SQL-RC-E473               VALUE    -473.
      * Result type of source is different
       78  SQL-RC-E475               VALUE    -475.
      * Reference to function is not unique
       78  SQL-RC-E476               VALUE    -476.
      * other obj depends on this obj
       78  SQL-RC-E478               VALUE    -478.
      * procedure not yet called
       78  SQL-RC-E480               VALUE    -480.
      * GROUP BY: element nested in element
       78  SQL-RC-E481               VALUE    -481.
      * Parm num not match with source
       78  SQL-RC-E483               VALUE    -483.
      * Boolean type is system used only
       78  SQL-RC-E486               VALUE    -486.
      * Attempt to excute an SQL statement
       78  SQL-RC-E487               VALUE    -487.
      * Inv BOOLEAN in select list result
       78  SQL-RC-E489               VALUE    -489.
      * Number outside the range of allowable values
       78  SQL-RC-E490               VALUE    -490.
      * DDL: missing reqired clause
       78  SQL-RC-E491               VALUE    -491.
      * DDL: miss match with source type
       78  SQL-RC-E492               VALUE    -492.
      * UDF returned bad date/time value
       78  SQL-RC-E493               VALUE    -493.
      * estimated cost exceeds resource limit error threshold
       78  SQL-RC-E495               VALUE    -495.
      * cursor already assigned to a result set
       78  SQL-RC-E499               VALUE    -499.

      * cursor not open (fetch/close)
       78  SQL-RC-E501               VALUE    -501.
      * cursor already open
       78  SQL-RC-E502               VALUE    -502.
      * column not in update clause
       78  SQL-RC-E503               VALUE    -503.
      * cursor name not defined
       78  SQL-RC-E504               VALUE    -504.
      * cursor name already declared
       78  SQL-RC-E505               VALUE    -505.
      * cursor not open (update/delete)
       78  SQL-RC-E507               VALUE    -507.
      * cursor not on a row
       78  SQL-RC-E508               VALUE    -508.
      * table not same as for cursor
       78  SQL-RC-E509               VALUE    -509.
      * table cannot be modified
       78  SQL-RC-E510               VALUE    -510.
      * for update not allowed
       78  SQL-RC-E511               VALUE    -511.
      * cursor not prepared
       78  SQL-RC-E514               VALUE    -514.
      * describe not a prepared statement
       78  SQL-RC-E516               VALUE    -516.
      * prepared statment not a select
       78  SQL-RC-E517               VALUE    -517.
      * execute not a prepared statement
       78  SQL-RC-E518               VALUE    -518.
      * statement has an open cursor
       78  SQL-RC-E519               VALUE    -519.
      * statement cannot be executed
       78  SQL-RC-E525               VALUE    -525.
      * function does not apply to global temp tables
       78  SQL-RC-E526               VALUE    -526.
      * duplicate primary/unique key
       78  SQL-RC-E528               VALUE    -528.
      * invalid foreign key value
       78  SQL-RC-E530               VALUE    -530.
      * cannot update primary key
       78  SQL-RC-E531               VALUE    -531.
      * delete is restricted
       78  SQL-RC-E532               VALUE    -532.
      * multi-row insert not allowed
       78  SQL-RC-E533               VALUE    -533.
      * multi-row update of pk
       78  SQL-RC-E534               VALUE    -534.
      * multi-row delete not allowed
       78  SQL-RC-E535               VALUE    -535.
      * descendent in subquery
       78  SQL-RC-E536               VALUE    -536.
      * dup column in key def
       78  SQL-RC-E537               VALUE    -537.
      * foreign key does not match pk
       78  SQL-RC-E538               VALUE    -538.
      * table does not have primary key
       78  SQL-RC-E539               VALUE    -539.
      * table does not have primary index
       78  SQL-RC-E540               VALUE    -540.
      * duplicate referential constraint
       78  SQL-RC-E541               VALUE    -541.
      * pk column cannot allow nulls
       78  SQL-RC-E542               VALUE    -542.
      * restricted row delete
       78  SQL-RC-E543               VALUE    -543.
      * check contraint violated
       78  SQL-RC-E544               VALUE    -544.
      * check contraint not satisfied
       78  SQL-RC-E545               VALUE    -545.
      * check contraint invalid
       78  SQL-RC-E546               VALUE    -546.
      * check contraint invalid
       78  SQL-RC-E548               VALUE    -548.
      * restricted stmt used with DYNAMICRULES BIND pkg
       78  SQL-RC-E549               VALUE    -549.
      * authorization error w/obj insert
       78  SQL-RC-E551               VALUE    -551.
      * auth error w/o obj ins
       78  SQL-RC-E552               VALUE    -552.
      * SYSIBM qualifier
       78  SQL-RC-E553               VALUE    -553.
      * cannot grant privilege to self
       78  SQL-RC-E554               VALUE    -554.
      * cannot revoke privilege from self
       78  SQL-RC-E555               VALUE    -555.
      * revoke stmt denied--priv not held
       78  SQL-RC-E556               VALUE    -556.
      * invalid combination of privileges
       78  SQL-RC-E557               VALUE    -557.
      * revoke stmt denied--has CONTROL
       78  SQL-RC-E558               VALUE    -558.
      * privilege not allowed for public
       78  SQL-RC-E562               VALUE    -562.
      * invalid authorization id
       78  SQL-RC-E567               VALUE    -567.
      * user/group ambiguity
       78  SQL-RC-E569               VALUE    -569.
      * Inoperative package
       78  SQL-RC-E572               VALUE    -572.
      * contraint col not primary key
       78  SQL-RC-E573               VALUE    -573.
      * DEFAULT invalid for column
       78  SQL-RC-E574               VALUE    -574.
      * view is inoperative
       78  SQL-RC-E575               VALUE    -575.
      * Repetitive alias chain
       78  SQL-RC-E576               VALUE    -576.
      * function or procedure not defined as MODIFIES SQL DATA
       78  SQL-RC-E577               VALUE    -577.
      * function or procedure not defined as READS SQL DATA
       78  SQL-RC-E579               VALUE    -579.
      * Result of CASE expr cannot be NULL
       78  SQL-RC-E580               VALUE    -580.
      * Data types of CASE expr incompat
       78  SQL-RC-E581               VALUE    -581.
      * Inv predicate in CASE expr
       78  SQL-RC-E582               VALUE    -582.
      * Variant or ext action function
       78  SQL-RC-E583               VALUE    -583.
      * Inv use of NULL or DEFAULT
       78  SQL-RC-E584               VALUE    -584.
      * Duplicate schema name     
       78  SQL-RC-E585               VALUE    -585.
      * Too many schema names     
       78  SQL-RC-E586               VALUE    -586.
      * Name in SQL procedure not unique 
       78  SQL-RC-E590               VALUE    -590.

      * Not authorized to update linked file
       78  SQL-RC-E597               VALUE    -597.
      * udf - dup func signature
       78  SQL-RC-E600               VALUE    -600.
      * duplicate table/view name
       78  SQL-RC-E601               VALUE    -601.
      * too many columns in index
       78  SQL-RC-E602               VALUE    -602.
      * cannot create unique index
       78  SQL-RC-E603               VALUE    -603.
      * invalid length, precision, scale
       78  SQL-RC-E604               VALUE    -604.
      * authid does not own column or table
       78  SQL-RC-E606               VALUE    -606.
      * op not allowed on system tables
       78  SQL-RC-E607               VALUE    -607.
      * duplicate column name
       78  SQL-RC-E612               VALUE    -612.
      * primary key too long
       78  SQL-RC-E613               VALUE    -613.
      * index key too long
       78  SQL-RC-E614               VALUE    -614.
      * object is in use and cannot be dropped
       78  SQL-RC-E615               VALUE    -615.
      * userid does not have appropriate dbspaces
       78  SQL-RC-E620               VALUE    -620.
      * clause invalid for this database
       78  SQL-RC-E622               VALUE    -622.
      * clustering index already exists for table
       78  SQL-RC-E623               VALUE    -623.
      * table already has primary key
       78  SQL-RC-E624               VALUE    -624.
      * DDL: multiple/conflict keywords
       78  SQL-RC-E628               VALUE    -628.
      * foreign key is not nullable
       78  SQL-RC-E629               VALUE    -629.
      * foreign key is too long
       78  SQL-RC-E631               VALUE    -631.
      * delete rule restriction
       78  SQL-RC-E632               VALUE    -632.
      * delete rule is restricted
       78  SQL-RC-E633               VALUE    -633.
      * delete rule cannot be CASCADE
       78  SQL-RC-E634               VALUE    -634.
      * dup pk or drop pk clause
       78  SQL-RC-E637               VALUE    -637.
      * no column definitions
       78  SQL-RC-E638               VALUE    -638.
      * invalid keyword value
       78  SQL-RC-E644               VALUE    -644.
      * bufferpool not active
       78  SQL-RC-E647               VALUE    -647.
      * invalid referential constraint
       78  SQL-RC-E648               VALUE    -648.
      * object cannot be explicitely dropped
       78  SQL-RC-E658               VALUE    -658.
      * architectural size limit of object
       78  SQL-RC-E659               VALUE    -659.
      * ref constraint does not hold
       78  SQL-RC-E667               VALUE    -667.
      * table in CHECK PENDING state
       78  SQL-RC-E668               VALUE    -668.
      * cannot drop primary key
       78  SQL-RC-E669               VALUE    -669.
      * row length too large
       78  SQL-RC-E670               VALUE    -670.
      * cannot drop table
       78  SQL-RC-E672               VALUE    -672.
      * primary key not unique
       78  SQL-RC-E673               VALUE    -673.
      * too many columns for table
       78  SQL-RC-E680               VALUE    -680.
      * incompatible column constraint
       78  SQL-RC-E683               VALUE    -683.
      * invalid trigger definition
       78  SQL-RC-E696               VALUE    -696.
      * invalid correlation name use
       78  SQL-RC-E697               VALUE    -697.

      * object uses reserved name
       78  SQL-RC-E707               VALUE    -707.
      * invalid replace value for sp reg
       78  SQL-RC-INV-REPL           VALUE    -713.
      * Bind error for user; pkg already exists
       78  SQL-RC-E719               VALUE    -719.
      * Attempt to replace existing package
       78  SQL-RC-E720               VALUE    -720.
      * Pkg name with consistency tokens is not unique
       78  SQL-RC-E721               VALUE    -721.
      * Bind/rebind error for uesr; pkg does not exist
       78  SQL-RC-E722               VALUE    -722.
      * trigger error
       78  SQL-RC-E723               VALUE    -723.
      * max level of cascading
       78  SQL-RC-E724               VALUE    -724.
      * Invalid implicit PREP or REBIND
       78  SQL-RC-E727               VALUE    -727.
      * MODIFIES SQL DATA option not valid in context
       78  SQL-RC-E740               VALUE    -740.
      * Routine violates nested SQL statement rules
       78  SQL-RC-E746               VALUE    -746.
      * Table cannot be renamed
       78  SQL-RC-E750               VALUE    -750.
      * statement not allowed in function or proc
       78  SQL-RC-E751               VALUE    -751.
      * Inoperative package
       78  SQL-RC-E752               VALUE    -752.
      * Case not found for CASE statement 
       78  SQL-RC-E773               VALUE    -773.
      * statement cannot be executed within an ATOMIC statement
       78  SQL-RC-E774               VALUE    -774.
      * Use of cursor not valid in FOR statement
       78  SQL-RC-E776               VALUE    -776.
      * Nested compound statements not allowed
       78  SQL-RC-E777               VALUE    -777.
      * End label not same name as begin label
       78  SQL-RC-E778               VALUE    -778.
      * Label in ITERATE or LEAVE is not valid
       78  SQL-RC-E779               VALUE    -779.
      * UNDO in handler in compound statement 
       78  SQL-RC-E780               VALUE    -780.
      * Condition in handler not defined      
       78  SQL-RC-E781               VALUE    -781.
      * Condition or SQLSTATE in handler not valid
       78  SQL-RC-E782               VALUE    -782.
      * SELECT list in FOR statement not valid
       78  SQL-RC-E783               VALUE    -783.
      * use of SQLCODE or SQLSTATE is not valid
       78  SQL-RC-E785               VALUE    -785.
      * RESIGNAL statement used outside handler
       78  SQL-RC-E787               VALUE    -787.
      * The same table row cannot be the target
       78  SQL-RC-E788               VALUE    -788.
      * type not supported in SQL procedures
       78  SQL-RC-E789               VALUE    -789.
      * Invalid triggered statement
       78  SQL-RC-E797               VALUE    -797.
      * Insert value not allowed on generated always identity column
       78  SQL-RC-E798               VALUE    -798.

      * divide by zero
       78  SQL-RC-E801               VALUE    -801.
      * arith overflow or divide by zero
       78  SQL-RC-E802               VALUE    -802.
      * distinct violation
       78  SQL-RC-E803               VALUE    -803.
      * bad input parameters
       78  SQL-RC-E804               VALUE    -804.
      * program not bound
       78  SQL-RC-E805               VALUE    -805.
      * inconsistent connect semantics
       78  SQL-RC-E808               VALUE    -808.
      * more than one row/value
       78  SQL-RC-E811               VALUE    -811.
      * statement would cause invalid update operation
       78  SQL-RC-E817               VALUE    -817.
      * time stamp conflict
       78  SQL-RC-E818               VALUE    -818.
      * invalid address in sqlda
       78  SQL-RC-E822               VALUE    -822.
      * Dynamic statement requires a result area
       78  SQL-RC-E838               VALUE    -838.
      * too many items in list
       78  SQL-RC-E840               VALUE    -840.
      * connection already exists
       78  SQL-RC-E842               VALUE    -842.
      * connection does not exist
       78  SQL-RC-E843               VALUE    -843.
      * cannot use CURRVAL before NEXTVAL
       78  SQL-RC-E845               VALUE    -845.
      * Invalid identity specification
       78  SQL-RC-E846               VALUE    -846.
      * can't change connection settings
       78  SQL-RC-E847               VALUE    -847.
      * TM not for 2 phase commit apps
       78  SQL-RC-E859               VALUE    -859.
      * Referential contstraint attempted to modify a table modified by SQL
      * data change stmt
       78  SQL-RC-E864               VALUE    -864.
      * inv TM_DATABASE value
       78  SQL-RC-E865               VALUE    -865.
      * connect redirect failed
       78  SQL-RC-E866               VALUE    -866.
      * connection already exists
       78  SQL-RC-E868               VALUE    -868.
      * objects with different encoding cannot be referenced
       78  SQL-RC-E873               VALUE    -873.
      * CCSID parameter must match routine parameter
       78  SQL-RC-E874               VALUE    -874.
      * savepoint does not exist or is invalid
       78  SQL-RC-E880               VALUE    -880.
      * savepoint name cannot be reused
       78  SQL-RC-E881               VALUE    -881.
      * savepoint does not exist
       78  SQL-RC-E882               VALUE    -882.

      * non-fatal system error
       78  SQL-RC-E901               VALUE    -901.
      * fatal error
       78  SQL-RC-E902               VALUE    -902.
      * commit failed, rollback
       78  SQL-RC-E903               VALUE    -903.
      * resource unavailable
       78  SQL-RC-E904               VALUE    -904.
      * resource limit exceeded
       78  SQL-RC-E905               VALUE    -905.
      * function disabled
       78  SQL-RC-E906               VALUE    -906.
      * failed attempt to modify target table of MERGE stmt
       78  SQL-RC-E907               VALUE    -907.
      * BIND operation not allowed
       78  SQL-RC-E908               VALUE    -908.
      * object deleted
       78  SQL-RC-E909               VALUE    -909.
      * drop pending
       78  SQL-RC-E910               VALUE    -910.
      * deadlock
       78  SQL-RC-E911               VALUE    -911.
      * too many lock requests
       78  SQL-RC-E912               VALUE    -912.
      * dist env rollback     
       78  SQL-RC-E913               VALUE    -913.
      * remote rebind from DRDA failed
       78  SQL-RC-E917               VALUE    -917.
      * ROLLBACK required
       78  SQL-RC-E918               VALUE    -918.
      * reject DB connection to SA REQ
       78  SQL-RC-E920               VALUE    -920.
      * COMMIT not allowed
       78  SQL-RC-E925               VALUE    -925.
      * ROLLBACK not allowed
       78  SQL-RC-E926               VALUE    -926.
      * insufficient storage
       78  SQL-RC-E930               VALUE    -930.
      * all inodes used, system limit
       78  SQL-RC-INODE              VALUE    -931.
      * cursors active on dropped object
       78  SQL-RC-E950               VALUE    -950.
      * table in use - alter disallowed
       78  SQL-RC-E951               VALUE    -951.
      * user cancel
       78  SQL-RC-E952               VALUE    -952.
      * agent heap too small
       78  SQL-RC-E953               VALUE    -953.
      * application heap too small
       78  SQL-RC-E954               VALUE    -954.
      * sort heap error
       78  SQL-RC-E955               VALUE    -955.
      * database heap too small
       78  SQL-RC-E956               VALUE    -956.
      * max number of files open
       78  SQL-RC-E958               VALUE    -958.
      * server comm heap too small
       78  SQL-RC-E959               VALUE    -959.
      * max nbr of files in database
       78  SQL-RC-E960               VALUE    -960.
      * log file full
       78  SQL-RC-E964               VALUE    -964.
      * error opening sqlcode mapping file
       78  SQL-RC-E966               VALUE    -966.
      * format error in sqlcode map file
       78  SQL-RC-E967               VALUE    -967.
      * disk full
       78  SQL-RC-E968               VALUE    -968.
      * unknown sql error another product
       78  SQL-RC-E969               VALUE    -969.
      * read-only file
       78  SQL-RC-E970               VALUE    -970.

      * incorrect diskette
       78  SQL-RC-E972-DB2AIX        VALUE    -10019.
      * incorrect diskette
       78  SQL-RC-E972-DB2OS2        VALUE    -972.
      * incorrect diskette
       78  SQL-RC-E972               value    -972.

      * out of memory error
       78  SQL-RC-E973               VALUE    -973.
      * out of memory warning
       78  SQL-RC-W973               VALUE    973.

      * drive locked
       78  SQL-RC-E974-DB2AIX        VALUE    -10019.
      * drive locked
       78  SQL-RC-E974-DB2OS2        VALUE    -974.
      * drive locked
       78  SQL-RC-E974               value    -974.

      * quiesce backup
       78  SQL-RC-QBACK              VALUE    -975.

      * diskette door open
       78  SQL-RC-E976-DB2AIX        VALUE    -10019.
      * diskette door open
       78  SQL-RC-E976-DB2OS2        VALUE    -976.
      * diskette door open
       78  SQL-RC-E976               value    -976.

      * unknown commit state
       78  SQL-RC-E977               VALUE    -977.
      * diskette write-protected
       78  SQL-RC-E978-DB2AIX        VALUE    -10019.
      * diskette write-protected
       78  SQL-RC-E978-DB2OS2        VALUE    -978.
      * diskette write-protected
       78  SQL-RC-E978               value    -978.

      * commit failed with SYNCPOINT NONE
       78  SQL-RC-E979               VALUE    -979.
      * disk error
       78  SQL-RC-E980               VALUE    -980.
      * disk error on temp file
       78  SQL-RC-E982               VALUE    -982.
      * unsuccessful commit or rollback
       78  SQL-RC-E984               VALUE    -984.
      * file error - catalog file bad
       78  SQL-RC-E985               VALUE    -985.
      * file error - file renamed
       78  SQL-RC-E986               VALUE    -986.
      * application shared memory cannot be allocated
       78  SQL-RC-E987               VALUE    -987.
      * AFTER trigger cannot modify a row being modified by an SQL data
      * change stmt
       78  SQL-RC-E989               VALUE    -989.
      * index structure limit error
       78  SQL-RC-E990               VALUE    -990.
      * release number incompatible
       78  SQL-RC-E992               VALUE    -992.
      * reserved
       78  SQL-RC-E994               VALUE    -994.
      * error freeing pages in DMS tablespace
       78  SQL-RC-E996               VALUE    -996.
      * General XA error
       78  SQL-RC-E998               VALUE    -998.

      * error finding pages in DMS tablespace
       78  SQL-RC-E1007              VALUE    -1007.
      * invalid tablespace id
       78  SQL-RC-E1008              VALUE    -1008.
      * Unexpected system error
       78  SQL-RC-E1042              VALUE    -1042.
      * Authid not valid
       78  SQL-RC-E1046              VALUE    -1046.
      * domain is not defined in DB2DOMAINLIST
       78  SQL-RC-E1068              VALUE    -1068.
      * Insufficient authority
       78  SQL-RC-E1092              VALUE    -1092.
      * User not logged on
       78  SQL-RC-E1093              VALUE    -1093.
      * Remote database cannot be accessed
       78  SQL-RC-E1101              VALUE    -1101.
      * table space maximum size exceeded
       78  SQL-RC-E1139              VALUE    -1139.
      * inspect completes with error/warnings
       78  SQL-RC-E1141              VALUE    -1141.
      * inspect fails with file in use
       78  SQL-RC-E1142              VALUE    -1142.
      * inspect fails with file i/o error
       78  SQL-RC-E1143              VALUE    -1143.
      * transaction rolled back due to failure creating index
       78  SQL-RC-E1144              VALUE    -1144.
      * dynamically prepared statement not supported
       78  SQL-RC-E1145              VALUE    -1145.
      * There are no indexes on table <name>
       78  SQL-RC-E1146              VALUE    -1146.
      * Index needs refresh, but tablespace is in backup pending state
       78  SQL-RC-E1148              VALUE    -1148.
      * table cannot be enabled for data capture
       78  SQL-RC-E1163              VALUE    -1163.
      * An error occurred while explaining the statement.
       78  SQL-RC-E1169              VALUE    -1169.
      * FEDERATED does not reference nickname or OLE DB function
       78  SQL-RC-E1178              VALUE    -1178.
      * UDF caused an OLE error
       78  SQL-RC-E1180              VALUE    -1180.
      * UDF raised an exception
       78  SQL-RC-E1181              VALUE    -1181.
      * UDF cannot initialize source object of OLE DB
       78  SQL-RC-E1182              VALUE    -1182.
      * UDF received OLE DB error
       78  SQL-RC-E1183              VALUE    -1183.
      * EXPLAIN table(s) created with earlier EXPLAIN.DDL
       78  SQL-RC-E1184              VALUE    -1184.
      * FEDERATED clause needed to bind package
       78  SQL-RC-E1185              VALUE    -1185.
      * FEDERATED needed with nickname or OLE DB function
       78  SQL-RC-E1186              VALUE    -1186.
      * command is not supported in the current downlevel client-server
      * configuration
       78  SQL-RC-E1198              VALUE    -1198.
      * Suboptimal performance. Reconnect to an alternate node.
       78  SQL-RC-E1199              VALUE    -1199.

      * invalid use of graphic data
       78  SQL-RC-E1216              VALUE    -1216.
      * REAL data type not supported
       78  SQL-RC-E1217              VALUE    -1217.
      * no bufferpool pages available
       78  SQL-RC-E1218              VALUE    -1218.
      * out of private memory
       78  SQL-RC-E1219              VALUE    -1219.
      * max number of agents started
       78  SQL-RC-E1226              VALUE    -1226.
      * Updatale cat values inv
       78  SQL-RC-E1227              VALUE    -1227.
      * System error rollback
       78  SQL-RC-E1229              VALUE    -1229.
      * Graphic data that is not UCS-2 is not supported
       78  SQL-RC-E1233              VALUE    -1233.
      * Disconn at next commit
       78  SQL-RC-W1244              VALUE    1244.
      * Max client connects
       78  SQL-RC-E1245              VALUE    -1245.
      * XA TM uses syncpoint 2
       78  SQL-RC-E1247              VALUE    -1247.
      * db not defined with TM
       78  SQL-RC-E1248              VALUE    -1248.
      * Non-SQL requests not supported from this downlevel client
       78  SQL-RC-E1288              VALUE    -1288.
      * DB2CLIENTCOMM env var inv
       78  SQL-RC-E1290              VALUE    -1290.
      * direcory services error
       78  SQL-RC-E1291              VALUE    -1291.
      * global dir error
       78  SQL-RC-E1293              VALUE    -1293.
      * global dir path invalid
       78  SQL-RC-E1294              VALUE    -1294.
      * global dir router invalid
       78  SQL-RC-E1295              VALUE    -1295.

      * Error processing keytab file
       78  SQL-RC-E1301              VALUE    -1301.
      * DCE principal to authid mapping error
       78  SQL-RC-E1302              VALUE    -1302.
      * Internal DCE error
       78  SQL-RC-E1305              VALUE    -1305.
      * Invalid server principal name
       78  SQL-RC-E1309              VALUE    -1309.
      * Error writing to audit log
       78  SQL-RC-E1322              VALUE    -1322.
      * Error accessing audit cfg
       78  SQL-RC-E1323              VALUE    -1323.
      * Remote host not found
       78  SQL-RC-E1336              VALUE    -1336.
      * Not Atomic Compound SQL error(s)
       78  SQL-RC-E1339              VALUE    -1339.

      * Plugin processing failed on the server
       78  SQL-RC-E1365              VALUE    -1365.
      * Plugin processing failed on the client
       78  SQL-RC-E1366              VALUE    -1366.
      * Operating system does not support resource policy definition.
       78  SQL-RC-E1367              VALUE    -1367.
      * Invalid resource policy configuration
       78  SQL-RC-E1368              VALUE    -1368.

      * Unexpected Kerberos security error
       78  SQL-RC-E1380              VALUE    -1380.
      * Security support interface not available
       78  SQL-RC-E1381              VALUE    -1381.
      * The Kerberos support not available
       78  SQL-RC-E1382              VALUE    -1382.
      * Invalid target principal name
       78  SQL-RC-E1383              VALUE    -1383.
      * Unable to complete mutual authentication
       78  SQL-RC-E1384              VALUE    -1384.

      * Auth type unsupported
       78  SQL-RC-E1400              VALUE    -1400.
      * Unexpected auth system error
       78  SQL-RC-E1402              VALUE    -1402.
      * Username or password incorrect
       78  SQL-RC-E1403              VALUE    -1403.
      * Password expired
       78  SQL-RC-E1404              VALUE    -1404.
      * Error communicating to auth server
       78  SQL-RC-E1405              VALUE    -1405.
      * too many concat operators
       78  SQL-RC-E1420              VALUE    -1420.
      * MBCS conversion error
       78  SQL-RC-E1421              VALUE    -1421.
      * Container wrong size
       78  SQL-RC-E1422              VALUE    -1422.
      * no blobs for dwn lvl cl
       78  SQL-RC-E1423              VALUE    -1423.
      * too many transition tbles
       78  SQL-RC-E1424              VALUE    -1424.
      * 32 / 64 bit connect incompatible
       78  SQL-RC-E1434              VALUE    -1434.

      * only valid for sync mgr connection
       78  SQL-RC-E1462              VALUE    -1462.

      * SUSPEND WRITE command failed
       78  SQL-RC-E1550              VALUE    -1550.
      * RESUME WRITE command failed
       78  SQL-RC-E1551              VALUE    -1551.
      * RESTART command failed; database is in SUSPEND WRITE
       78  SQL-RC-E1552              VALUE    -1552.
      * DB2STOP command failed; database is in SUSPEND WRITE
       78  SQL-RC-E1553              VALUE    -1553.
      * table cannot be in append mode with clustering index
       78  SQL-RC-E1581              VALUE    -1581.
      * pagesize of table space doesn't match bufferpool
       78  SQL-RC-E1582              VALUE    -1582.
      * pagesize not supported
       78  SQL-RC-E1583              VALUE    -1583.
      * temporary table space cannot be found
       78  SQL-RC-E1584              VALUE    -1584.
      * No temp table space with sufficient page size
       78  SQL-RC-E1585              VALUE    -1585.
      * ON option of SET INTEGRITY invalid
       78  SQL-RC-E1591              VALUE    -1591.
      * INCREMENTAL option invalid
       78  SQL-RC-E1592              VALUE    -1592.
      * REMAIN PENDING option invalid
       78  SQL-RC-E1593              VALUE    -1593.
      * table integrity cannot be checked
       78  SQL-RC-E1595              VALUE    -1595.
      * WITH EMPTY TABLE option not allowed because of dependent REFRESH
      * IMMEDIATE table 
       78  SQL-RC-E1596              VALUE    -1596.

      * The table is not created with NOT LOGGED INITIALLY
       78  SQL-RC-E1749              VALUE    -1749.

      * foreign keys not allowed for tables created not logged initially
       78  SQL-RC-E1750              VALUE    -1750.
      * nodegroups must have at least one node
       78  SQL-RC-E1751              VALUE    -1751.
      * tablespace not created in the correct nodegroup
       78  SQL-RC-E1752              VALUE    -1752.
      * a node does not have the complete temporary tablespaces
       78  SQL-RC-E1753              VALUE    -1753.
      * all tablespace in CREATE TABLE must be in the same nodegroup
       78  SQL-RC-E1754              VALUE    -1754.
      * more than one clause specifies containers without ON NODES clause
       78  SQL-RC-E1756              VALUE    -1756.
      * missing USING clause without ON NODES clause
       78  SQL-RC-E1757              VALUE    -1757.
      * missing clause for create stored procedure
       78  SQL-RC-E1760              VALUE    -1760.
      * nodegroup not defined for buffer pool
       78  SQL-RC-E1761              VALUE    -1761.
      * not enough disk space for connect
       78  SQL-RC-E1762              VALUE    -1762.
      * cannot specify multiple ALTER TABLESPACE actions
       78  SQL-RC-E1763              VALUE    -1763.
      * cannot specify a smaller container size
       78  SQL-RC-E1764              VALUE    -1764.

      * operation cannot be executed in No Package Lock mode
       78  SQL-RC-E1803              VALUE    -1803.
      * Wrapper cannot be used to access data source
       78  SQL-RC-E1816              VALUE    -1816.
      * CREATE SERVER statement does not identify data source
       78  SQL-RC-E1817              VALUE    -1817.
      * ALTER SERVER statement cannot be processed
       78  SQL-RC-E1818              VALUE    -1818.
      * DROP SERVER statement cannot be processed
       78  SQL-RC-E1819              VALUE    -1819.
      * Action on LOB value failed
       78  SQL-RC-E1820              VALUE    -1820.
      * Unexpected error from data source
       78  SQL-RC-E1822              VALUE    -1822.
      * No data type mapping exists for server
       78  SQL-RC-E1823              VALUE    -1823.
      * Statement cannot be handled by Datajoiner
       78  SQL-RC-E1825              VALUE    -1825.
      * Invalid value for system catalog column
       78  SQL-RC-E1826              VALUE    -1826.
      * user mapping undefined
       78  SQL-RC-E1827              VALUE    -1827.
      * server option undefined
       78  SQL-RC-E1828              VALUE    -1828.

      * TCPIP not running
       78  SQL-RC-NO-TCPIP           VALUE    -1468.
      * invalid node
       78  SQL-RC-NODE-INVALID       VALUE    -1469.
      * DB2NODE env var invalid
       78  SQL-RC-DB2NODE-INVALID    VALUE    -1470.
      * nodes out of sync
       78  SQL-RC-LOG-MISMATCH       VALUE    -1471.
      * connect fail, system clocks out of sync
       78  SQL-RC-TIMEDIFF-CA        VALUE    -1472.
      * commit fail, system clocks out of sync
       78  SQL-RC-TIMEDIFF-CO        VALUE    -1473.
      * transaction OK, system clocks out of sync
       78  SQL-RC-TIMEDIFF-W         VALUE    -1474.
      * rollback on table error
       78  SQL-RC-E1476              VALUE    -1476.
      * table not accessible
       78  SQL-RC-E1477              VALUE    -1477.

      * LONG not allowed on devices
       78  SQL-RC-E1590              VALUE    -1590.
      * Specified definition, schema, or nickname does not exist
       78  SQL-RC-E1791              VALUE    -1791.
      * RETURNS clause must be specified before EXPRESSION AS
       78  SQL-RC-E1830              VALUE    -1830.
      * Cannot update table stats for subtable
       78  SQL-RC-E1831              VALUE    -1831.
      * data filter function cannot be LANGUAGE SQL
       78  SQL-RC-E1832              VALUE    -1832.
      * Connection to Extended Search Server failed
       78  SQL-RC-E1833              VALUE    -1833.
      * User-defined column identical to fixed column
       78  SQL-RC-E1834              VALUE    -1834.
      * Extended Search  object could not be found 
       78  SQL-RC-E1835              VALUE    -1835.
      * No column mapping exist to Extended Search field
       78  SQL-RC-E1836              VALUE    -1836.
      * Required option can not be dropped
       78  SQL-RC-E1837              VALUE    -1837.
      * Statement is not a valid Extended Search query
       78  SQL-RC-E1838              VALUE    -1838.
      * One ore more search parameter are not valid
       78  SQL-RC-E1839              VALUE    -1839.
      * Option cannot be added to object
       78  SQL-RC-E1840              VALUE    -1840.
      * The value cannot be changed for the object
       78  SQL-RC-E1841              VALUE    -1841.
      * Option is not valid
       78  SQL-RC-E1842              VALUE    -1842.
      * The operator is not supported
       78  SQL-RC-E1843              VALUE    -1843.
      * The option conflicts with the object
       78  SQL-RC-E1846              VALUE    -1846.
      * Template substiution error
       78  SQL-RC-E1847              VALUE    -1847.
      * Key sequence column is out of range
       78  SQL-RC-E1870              VALUE    -1870.
      * Function not supported on range-clustered tables
       78  SQL-RC-E1871              VALUE    -1871.
      * option is not valid for this data source
       78  SQL-RC-E1881              VALUE    -1881.
      * option is not valid for this data source
       78  SQL-RC-E1882              VALUE    -1882.
      * Missing option for this data source
       78  SQL-RC-E1883              VALUE    -1883.
      * Option specified more than once
       78  SQL-RC-E1884              VALUE    -1884.
      * option is already defined
       78  SQL-RC-E1885              VALUE    -1885.
      * option has not been added
       78  SQL-RC-E1886              VALUE    -1886.
      * SPECIFICATION ONLY clause required
       78  SQL-RC-E1887              VALUE    -1887.

      * cannot add/drop DB2 Data Links Manager
       78  SQL-RC-E2078              VALUE    -2078.

      * incorrect syntax or password for filtering
       78  SQL-RC-E2180              VALUE    -2180.
      * internal error occurred during filter recovery
       78  SQL-RC-E2181              VALUE    -2181.
      * A statistics profile does not exist
       78  SQL-RC-E2315              VALUE    -2315.

      * Invalid user format in filetmod
       78  SQL-RC-E3191              VALUE    -3191.
      * Invalid user format in filetmod
       78  SQL-RC-E3192              VALUE    -3192.

      * Cannot import replace parent
       78  SQL-RC-E3201              VALUE    -3201.
      * Unexpected LDAP error
       78  SQL-RC-E3260              VALUE    -3260.
      * Missing required parameters
       78  SQL-RC-E3261              VALUE    -3261.
      * The TCP/IP service name is not valid
       78  SQL-RC-E3262              VALUE    -3262.
      * The protocol type is not supported
       78  SQL-RC-E3263              VALUE    -3263.
      * The DB2 server has not been registered in LDAP
       78  SQL-RC-E3264              VALUE    -3264.
      * LDAP authentication error
       78  SQL-RC-E3265              VALUE    -3265.
      * Insufficient LDAP authority
       78  SQL-RC-E3267              VALUE    -3267.
      * LDAP schema is not compatible
       78  SQL-RC-E3268              VALUE    -3268.
      * The LDAP server is not available
       78  SQL-RC-E3269              VALUE    -3269.
      * LDAP user's DN is invalid
       78  SQL-RC-E3270              VALUE    -3270.
      * LDAP user's DN or password is not configured
       78  SQL-RC-E3271              VALUE    -3271.
      * The LDAP node was not found
       78  SQL-RC-E3272              VALUE    -3272.
      * The LDAP database was not found
       78  SQL-RC-E3273              VALUE    -3273.
      * Unable to obtain the LDAP naming context
       78  SQL-RC-E3276              VALUE    -3276.
      * The database already exists
       78  SQL-RC-E3277              VALUE    -3277.
      * The node already exists
       78  SQL-RC-E3278              VALUE    -3278.
      * LDAP is disabled
       78  SQL-RC-E3279              VALUE    -3279.
      * Attempt to connect to a DRDA server failed.
       78  SQL-RC-E3280              VALUE    -3280.
      * The operating system type is invalid
       78  SQL-RC-E3281              VALUE    -3281.
      * The supplied credentials are not valid.
       78  SQL-RC-E3282              VALUE    -3282.
      * The node type is not supported
       78  SQL-RC-E3284              VALUE    -3284.

      * Table not in check pend state
       78  SQL-RC-E3600              VALUE    -3600.
      * Auto Check pending state
       78  SQL-RC-W3601              VALUE    3601.
      * Constraint violation moved
       78  SQL-RC-W3602              VALUE    3602.
      * Constraint Violations
       78  SQL-RC-E3603              VALUE    -3603.
      * Invalid Exception Table
       78  SQL-RC-E3604              VALUE    -3604.
      * Exception Table same as Check Table
       78  SQL-RC-E3605              VALUE    -3605.
      * # check & exception tables not match
       78  SQL-RC-E3606              VALUE    -3606.
      * Parent in Check Pending state
       78  SQL-RC-E3608              VALUE    -3608.

      * Invalid SQL sub-statement in Compound SQL
       78  SQL-RC-E4011              VALUE    -4011.
      * A 'long' host variable is not valid - use 'sqlint32' instead.
       78  SQL-RC-E4020              VALUE    -4020.

      * Java support not installed
       78  SQL-RC-E4300              VALUE    -4300.
      * Java startup, comm, shutdown failed
       78  SQL-RC-E4301              VALUE    -4301.
      * Java unclassified exception
       78  SQL-RC-E4302              VALUE    -4302.
      * Java could not parse class!method
       78  SQL-RC-E4303              VALUE    -4303.
      * Java could not instantiate class
       78  SQL-RC-E4304              VALUE    -4304.
      * Java internal error code
       78  SQL-RC-E4305              VALUE    -4305.
      * Java cannot call method
       78  SQL-RC-E4306              VALUE    -4306.
      * Java method call problems
       78  SQL-RC-E4307              VALUE    -4307.

      * Host variable is not exact numeric type
       78  SQL-RC-E5012              VALUE    -5012.
      * Invalid qualifier specified in CREATE SCHEMA
       78  SQL-RC-E5051              VALUE    -5051.

      * Required environment variable no set
       78  SQL-RC-E5182              VALUE    -5182.
      * Pass-through not supported for server type
       78  SQL-RC-E5185              VALUE    -5185.

      * Unable to open vendor configuration file
       78  SQL-RC-E5500              VALUE    -5500.
      * Format of vendor configuration file is wrong
       78  SQL-RC-E5501              VALUE    -5501.

      * Insufficient FCM buffers
       78  SQL-RC-E6040              VALUE    -6040.
      * Insufficient FCM connection entries
       78  SQL-RC-E6041              VALUE    -6041.
      * Insufficient FCM message anchors
       78  SQL-RC-E6042              VALUE    -6042.
      * Insufficient FCM request blocks
       78  SQL-RC-E6043              VALUE    -6043.
      * New node requires stop and start
       78  SQL-RC-E6071              VALUE    -6071.

      * SQL Procedure not created       
       78  SQL-RC-E7032              VALUE    -7032.

      * db2start failed, license not found
       78  SQL-RC-E8000              VALUE    -8000.
      * udb connection failed, license not found
       78  SQL-RC-E8001              VALUE    -8001.
      * 'connect' connection failed, license not found
       78  SQL-RC-E8002              VALUE    -8002.
      * evaluation period expired
       78  SQL-RC-E8008              VALUE    -8008.
      * not licensed for TCP/IP connections.
       78  SQL-RC-E8014              VALUE    -8014.
      * not licensed for multiple database updates per transaction
       78  SQL-RC-E8015              VALUE    -8015.
      * this user is not defined as registered user
       78  SQL-RC-E8016              VALUE    -8016.
      * not licensed for database partitioning
       78  SQL-RC-E8022              VALUE    -8022.
      * concurrent users limit exceeded
       78  SQL-RC-E8023              VALUE    -8023.
      * limited function license - function not allowed
       78  SQL-RC-E8024              VALUE    -8024.

      * Page number too high
       78  SQL-RC-E8100              VALUE    -8100.
      * Segment in error
       78  SQL-RC-E8101              VALUE    -8101.

      * Development SQLCODE
       78  SQL-RC-E9999              VALUE    -9999.
      * not enough systems resources to process request
       78  SQL-RC-C10003             VALUE    -10003.
      * internal object limit exceeded
       78  SQL-RC-E20005             VALUE    -20005.
      * mutation methd not allowed
       78  SQL-RC-E20010             VALUE    -20010.
      * transform for data type already exists
       78  SQL-RC-E20011             VALUE    -20011.
      * no transforms were dropped
       78  SQL-RC-E20012             VALUE    -20012.
      * Invalid object for supertype
       78  SQL-RC-E20013             VALUE    -20013.
      * function cannot be used as transform function
       78  SQL-RC-E20014             VALUE    -20014.
      * transform for type not defined
       78  SQL-RC-E20015             VALUE    -20015.
      * column length value too small
       78  SQL-RC-E20016             VALUE    -20016.
      * Hierarchy too deep
       78  SQL-RC-E20017             VALUE    -20017.
      * row function must return at most one row
       78  SQL-RC-E20018             VALUE    -20018.
      * result type cannot be assigned to RETURN type
       78  SQL-RC-E20019             VALUE    -20019.
      * Invalid operation for typed table
       78  SQL-RC-E20020             VALUE    -20020.
      * Cannot change inherited column
       78  SQL-RC-E20021             VALUE    -20021.
      * SCOPE already defined
       78  SQL-RC-E20022             VALUE    -20022.
      * SCOPE not allowed for parameter
       78  SQL-RC-E20023             VALUE    -20023.
      * SCOPE not valid for reference
       78  SQL-RC-E20024             VALUE    -20024.
      * Incorrect SCOPE for RETURNS
       78  SQL-RC-E20025             VALUE    -20025.
      * Type is not a structured type
       78  SQL-RC-E20026             VALUE    -20026.
      * Subtable already exists of type
       78  SQL-RC-E20027             VALUE    -20027.
      * Subtable schema incorrect
       78  SQL-RC-E20028             VALUE    -20028.
      * Invalid operation for subtable
       78  SQL-RC-E20029             VALUE    -20029.
      * Attrs cannot be altered when in use
       78  SQL-RC-E20030             VALUE    -20030.
      * Invalid object for subtable
       78  SQL-RC-E20031             VALUE    -20031.
      * Invalid index columns for subtable
       78  SQL-RC-E20032             VALUE    -20032.
      * Unscoped reference
       78  SQL-RC-E20033             VALUE    -20033.
      * Invalid TYPE predicate
       78  SQL-RC-E20034             VALUE    -20034.
      * Invalid path expression
       78  SQL-RC-E20035             VALUE    -20035.
      * Path includes OID column
       78  SQL-RC-E20036             VALUE    -20036.
      * REF IS column must be defined
       78  SQL-RC-E20037             VALUE    -20037.
      * ASC or DESC cannot be specified
       78  SQL-RC-E20038             VALUE    -20038.
      * definition of index does not match extension
       78  SQL-RC-E20039             VALUE    -20039.
      * number or type of result inconsistent
       78  SQL-RC-E20040             VALUE    -20040.
      * number or type of parameters does not match
       78  SQL-RC-E20041             VALUE    -20041.
      * maximum allowable parameters exceeded
       78  SQL-RC-E20042             VALUE    -20042.
      * argument for function is invalid
       78  SQL-RC-E20043             VALUE    -20043.
      * function not supported in CREATE INDEX EXTENSION
       78  SQL-RC-E20044             VALUE    -20044.
      * datatype of instance parameter invalid
       78  SQL-RC-E20045             VALUE    -20045.
      * SELECTIVITY clause needs user-defined predicate
       78  SQL-RC-E20046             VALUE    -20046.
      * search method not found
       78  SQL-RC-E20047             VALUE    -20047.
      * search method argument mismatch
       78  SQL-RC-E20048             VALUE    -20048.
      * operand type mismatch
       78  SQL-RC-E20049             VALUE    -20049.
      * search target or search argument mismatch
       78  SQL-RC-E20050             VALUE    -20050.
      * argument cannot be both search target and search argument
       78  SQL-RC-E20051             VALUE    -20051.
      * Cannot update OID column
       78  SQL-RC-E20052             VALUE    -20052.
      * Fullselect in typed view is not valid
       78  SQL-RC-E20053             VALUE    -20053.
      * Invalid state for operation
       78  SQL-RC-E20054             VALUE    -20054.
      * Result column data type not compatible
       78  SQL-RC-E20055             VALUE    -20055.
      * Processing error on file server
       78  SQL-RC-E20056             VALUE    -20056.
      * column cannot be defined as read only
       78  SQL-RC-E20057             VALUE    -20057.
      * fullselect for summary table is invalid
       78  SQL-RC-E20058             VALUE    -20058.
      * key transform function generated duplicate rows
       78  SQL-RC-E20060             VALUE    -20060.
      * transform function not valid for function or method
       78  SQL-RC-E20062             VALUE    -20062.
      * TRANSFORM GROUP clause is required
       78  SQL-RC-E20063             VALUE    -20063.
      * specified transform group clause is not used
       78  SQL-RC-E20064             VALUE    -20064.
      * transform group cannot be used with client application
       78  SQL-RC-E20065             VALUE    -20065.
      * transform function not defined in transform group
       78  SQL-RC-E20066             VALUE    -20066.
      * transform function defined more than once in transform group
       78  SQL-RC-E20067             VALUE    -20067.
      * structured type cannot depend on itself
       78  SQL-RC-E20068             VALUE    -20068.
      * returns type not same as subject type
       78  SQL-RC-E20069             VALUE    -20069.
      * index not created, column length too long
       78  SQL-RC-E20075             VALUE    -20075.
      * instance not enabled for operation
       78  SQL-RC-E20076             VALUE    -20076.
      * Cannot construct object with Datalink or Reference type attributes
       78  SQL-RC-E20077             VALUE    -20077.
      * operation cannot be applied to object
       78  SQL-RC-E20078             VALUE    -20078.
      * method specification cannot be dropped
       78  SQL-RC-E20080             VALUE    -20080.
      * method body must correspond to specfication language
       78  SQL-RC-E20081             VALUE    -20081.
      * dynamic type is not a subtype of target
       78  SQL-RC-E20082             VALUE    -20082.
      * returned data type does not match RESULT
       78  SQL-RC-E20083             VALUE    -20083.
      * routine would override an existing method
       78  SQL-RC-E20084             VALUE    -20084.
      * Java routine cannot have structured type parameter or returns type
       78  SQL-RC-E20085             VALUE    -20085.
      * Length of structured type exceeds limit
       78  SQL-RC-E20086             VALUE    -20086.
      * DEFAULT or NULL cannot be used in an attribute assignment
       78  SQL-RC-E20087             VALUE    -20087.
      * method name and structured type name match
       78  SQL-RC-E20089             VALUE    -20089.
      * conversion error between summary table and regular table
       78  SQL-RC-E20093             VALUE    -20093.
      * GENERATED column cannot be referenced in BEFORE trigger
       78  SQL-RC-E20094             VALUE    -20094.

      * CREATE or ALTER for the routine is not allowed
       78  SQL-RC-E20102             VALUE    -20102.
      * result set contains unsupported data type
       78  SQL-RC-E20108             VALUE    -20108.
      * SAVEPOINT statement is not allowed in this context
       78  SQL-RC-E20111             VALUE    -20111.
      * nested savepoint is not allowed
       78  SQL-RC-E20112             VALUE    -20112.
      * cannot return null from SELF AS RESULT method
       78  SQL-RC-E20113             VALUE    -20113.
      * routine cannot be used as a transform function
       78  SQL-RC-E20115             VALUE    -20115.
      * search target and source key data types do not match
       78  SQL-RC-E20116             VALUE    -20116.
      * window specification for an OLAP function invalid
       78  SQL-RC-E20117             VALUE    -20117.
      * maximum number of attributes exceeded
       78  SQL-RC-E20118             VALUE    -20118.
      * row function must return at least two columns
       78  SQL-RC-E20119             VALUE    -20119.
      * SQL table function must return a table
       78  SQL-RC-E20120             VALUE    -20120.
      * WITH RETURN and SCROLL specified for a single cursor
       78  SQL-RC-E20121             VALUE    -20121.
      * stored procedure cursor error
       78  SQL-RC-E20123             VALUE    -20123.
      * scrollable cursor cannot include table function output
       78  SQL-RC-E20128             VALUE    -20128.
      * Obj number specified more than once in list
       78  SQL-RC-E20131             VALUE    -20131.
      * operation must be performed on SQL routine
       78  SQL-RC-E20133             VALUE    -20133.
      * SQL archive could not be created on server
       78  SQL-RC-E20134             VALUE    -20134.
      * SQL archive does not match target environment
       78  SQL-RC-E20135             VALUE    -20135.
      * Routine is NOT FEDERATED
       78  SQL-RC-E20136             VALUE    -20136.
      * Routine is not defined as MODIFIES SQL DATA
       78  SQL-RC-E20138             VALUE    -20138.
      * Previous statement failed or was interrupted
       78  SQL-RC-E20139             VALUE    -20139.
      * Sequence cannot be used as specified
       78  SQL-RC-E20142             VALUE    -20142.
      * ENCRYPTION PASSWORD special register not set
       78  SQL-RC-E20143             VALUE    -20143.
      * invalid length for encryption password
       78  SQL-RC-E20144             VALUE    -20144.
      * decryption key does not match encryption key
       78  SQL-RC-E20145             VALUE    -20145.
      * DECRYPT failed because data is not encrypted
       78  SQL-RC-E20146             VALUE    -20146.
      * ENCRYPT function failed
       78  SQL-RC-E20147             VALUE    -20147.
      * routine must end with RETURN statement
       78  SQL-RC-E20148             VALUE    -20148.
      * block pages too large for the buffer pool size
       78  SQL-RC-E20150             VALUE    -20150.
      * BLOCKSIZE value is not in the valid range
       78  SQL-RC-E20151             VALUE    -20151.
      * specified buffer pool is not block-based
       78  SQL-RC-E20152             VALUE    -20152.
      * database split image is suspended
       78  SQL-RC-E20153             VALUE    -20153.
      * Insert into view not allowed target table cannot be determined
       78  SQL-RC-E20154             VALUE    -20154.
      * event monitor target table invalid
       78  SQL-RC-E20155             VALUE    -20155.
      * User does not have QUIESCE_CONNECT privilege
       78  SQL-RC-E20157             VALUE    -20157.
      * Function not supported for level of DLM
       78  SQL-RC-E20158             VALUE    -20158.
      * cannot use block-based and extended storage
       78  SQL-RC-E20162             VALUE    -20162.
      * SQL data change stmt not allowed
       78  SQL-RC-E20165             VALUE    -20165.
      * Speicifed view is not a symmetric view
       78  SQL-RC-E20166             VALUE    -20166.
      * Virtual storage or database resource is not available.
       78  SQL-RC-E20167             VALUE    -20167.
      * The ALTER BUFFERPOOL statement is currently in progress.
       78  SQL-RC-E20168             VALUE    -20168.
      * There is not enough space in the table space for the specified
      * action.
       78  SQL-RC-E20170             VALUE    -20170.
      * view already has INSTEAD OF trigger defined
       78  SQL-RC-E20178             VALUE    -20178.
      * trigger not created because view defined WITH CHECK OPTION
       78  SQL-RC-E20179             VALUE    -20179.

      * primary or unique key is a subset of columns in dimensions clause
       78  SQL-RC-E20188             VALUE    -20188.
      * Federated operation not compiled due to potential inconsistency
       78  SQL-RC-E20190             VALUE    -20190.
      * the same host variable must be used in both USING and INTO clauses
       78  SQL-RC-E20191             VALUE    -20191.
      * the requested feature is not supported in this environment
       78  SQL-RC-E20192             VALUE    -20192.
      * error has occurred when accessing a file with reason-code
       78  SQL-RC-E20193             VALUE    -20193.
      * buffer pool does not exist on database partition
       78  SQL-RC-E20194             VALUE    -20194.
      * error encountered while processing the path rename config file
       78  SQL-RC-E20195             VALUE    -20195.
      * one or more built-in types do not match corresponding built-in types
       78  SQL-RC-E20196             VALUE    -20196.
      * cannot define method as an overriding method
       78  SQL-RC-E20197             VALUE    -20197.
      * method calls itself recursively
       78  SQL-RC-E20198             VALUE    -20198.
      * URL not found
       78  SQL-RC-E20200             VALUE    -20200.
      * jar name invalid
       78  SQL-RC-E20201             VALUE    -20201.
      * class is in use
       78  SQL-RC-E20202             VALUE    -20202.
      * Java method has invalid signature
       78  SQL-RC-E20203             VALUE    -20203.
      * function unable to map to single method
       78  SQL-RC-E20204             VALUE    -20204.
      * null value not allowed in argument
       78  SQL-RC-E20205             VALUE    -20205.
      * unsupported deployment descriptor
       78  SQL-RC-E20207             VALUE    -20207.
      * Table used to define a staging table is not valid
       78  SQL-RC-E20208             VALUE    -20208.
      * Option not valid for table with reason-code
       78  SQL-RC-E20209             VALUE    -20209.

      * table-designator does not contain ORDER BY
       78  SQL-RC-E20210             VALUE    -20210.
      * ORDER BY and FETCH FIRST n ROWS ONLY is invalid
       78  SQL-RC-E20211             VALUE    -20211.
      * User defined routine encountered an exception while loading java
      * class
       78  SQL-RC-E20212             VALUE    -20212.
      * ORDER OF specified but table designator not ordered
       78  SQL-RC-E20214             VALUE    -20214.
      * Encryption facility not available
       78  SQL-RC-E20223             VALUE    -20223.
      * Procedure name may not be specified by a host variable in the CALL
      * statement
       78  SQL-RC-E20230             VALUE    -20230.
      * Table defined as CCSID UNICODE
       78  SQL-RC-E20238             VALUE    -20238.
      * Table cannot be typed, or contain graphic or user-defined types
       78  SQL-RC-E20239             VALUE    -20239.
      * Unable to write a history file entry
       78  SQL-RC-E20241             VALUE    -20241.
      * Sample size in clause is invalid
       78  SQL-RC-E20242             VALUE    -20242.
      * The view is missing the INSTEAD OF triggers
       78  SQL-RC-E20243             VALUE    -20243.
      * BEFORE TRIGGER or GENERATED COLUMN would cause table to be
      * delete-connected with ovelapping set null rules
       78  SQL-RC-E20253             VALUE    -20253.
      * FOREIGN KEY would cause table to be delete-connected to itself
       78  SQL-RC-E20254             VALUE    -20254.
      * FOREIGN KEY would cause a descent table to be delete-connected to
      * ancestor table
       78  SQL-RC-E20255             VALUE    -20255.
      * FOREIGN KEY would cause two tables to be delete-connected
       78  SQL-RC-E20256             VALUE    -20256.
      * FINAL TABLE is not valid
       78  SQL-RC-E20257             VALUE    -20257.
      * Invalid use of INPUT SEQUENCE ordering
       78  SQL-RC-E20258             VALUE    -20258.
      * column cannot be specified in the select list of query 
       78  SQL-RC-E20259             VALUE    -20259.
      * assignment clause of UPDATE stmt must specify at least one column
      * that is not an INCLUDE column
       78  SQL-RC-E20260             VALUE    -20260.
      * Invalid row movement to table
       78  SQL-RC-E20261             VALUE    -20261.
      * Invalid usage of WITH ROW MOVEMENT in a view
       78  SQL-RC-E20262             VALUE    -20262.
      * Invalid attempt to update view
       78  SQL-RC-E20263             VALUE    -20263.
      * A function is invoked in an illegal context
       78  SQL-RC-E20267             VALUE    -20267.
      * A nickname cannot be referenced in an enforced referential
      * constraint
       78  SQL-RC-E20269             VALUE    -20269.
      * Nickname stats cannot be updated
       78  SQL-RC-E20273             VALUE    -20273.
      * XML name is not valid
       78  SQL-RC-E20275             VALUE    -20275.
      * XML namespace is not valid
       78  SQL-RC-E20276             VALUE    -20276.
      * View cannot be enabled for query optimization
       78  SQL-RC-E20279             VALUE    -20279.
      * Unable to load .NET class
       78  SQL-RC-E20282             VALUE    -20282.
      * Unable to create plan
       78  SQL-RC-E20284             VALUE    -20284.
      * Stats could not be updated
       78  SQL-RC-E20288             VALUE    -20288.
      * Routine cannot be run on the specified partition
       78  SQL-RC-E20290             VALUE    -20290.
      * Invalid compilation environment
       78  SQL-RC-E20316             VALUE    -20316.
      * Automatic storage not defined for database
       78  SQL-RC-E20317             VALUE    -20317.
      * Cannot perform container operations against table space
       78  SQL-RC-E20318             VALUE    -20318.
      * Redirected restore of automatic storage table space not allowed
       78  SQL-RC-E20319             VALUE    -20319.
      * Maximum size for table space not valid
       78  SQL-RC-E20320             VALUE    -20320.
      * Storage paths cannot be specified
       78  SQL-RC-E20321             VALUE    -20321.
      * Database name does not match current server
       78  SQL-RC-E20322             VALUE    -20322.
      * Storage path is a duplicate
       78  SQL-RC-E20323             VALUE    -20323.
      * Operation cannot occur multiple times in transaction
       78  SQL-RC-E20324             VALUE    -20324.
      * Maximum size of table space exceeded
       78  SQL-RC-E20325             VALUE    -20325.
      * Operation violated an integrity constraint
       78  SQL-RC-E20333             VALUE    -20333.
      * Text Information Extender incorrectly configured
       78  SQL-RC-E21000             VALUE    -21000.

      * The funtion or feature name is invalid
       78  SQL-RC-E22400             VALUE    -22400.
      * Application ID does not exist
       78  SQL-RC-E22401             VALUE    -22401.
      * No activity monitor reports exist
       78  SQL-RC-E22402             VALUE    -22402.
      * Invalid values specified
       78  SQL-RC-E22403             VALUE    -22403.
      * Specified action mode is invalid
       78  SQL-RC-E22404             VALUE    -22404.
      * Unable to collect snapshot data
       78  SQL-RC-E22405             VALUE    -22405.

      * Distribution protocol error, no disconnect
       78  SQL-RC-E30000             VALUE    -30000.
      * statement cannot be executed due to prior condition
       78  SQL-RC-E30002             VALUE    -30002.
      * Execution failed; function not supported
       78  SQL-RC-E30005             VALUE    -30005.
      * Distribution protocol error, disconnect
       78  SQL-RC-E30020             VALUE    -30020.
      * Distribution compatibility error
       78  SQL-RC-E30021             VALUE    -30021.
      * Distribution memory allocation err
       78  SQL-RC-E30040             VALUE    -30040.
      * Distribution memory allocation err, disconnect
       78  SQL-RC-E30041             VALUE    -30041.
      * Invalid command while bind in prog
       78  SQL-RC-E30050             VALUE    -30050.
      * Bind not active
       78  SQL-RC-E30051             VALUE    -30051.
      * Bind Owner authorization failure
       78  SQL-RC-E30053             VALUE    -30053.
      * RDB authorization failure
       78  SQL-RC-E30060             VALUE    -30060.
      * RDB not found
       78  SQL-RC-E30061             VALUE    -30061.
      * Distribution command error
       78  SQL-RC-E30070             VALUE    -30070.
      * Distribution object error
       78  SQL-RC-E30071             VALUE    -30071.
      * Distribution parameter error
       78  SQL-RC-E30072             VALUE    -30072.
      * Distribution parameter value error
       78  SQL-RC-E30073             VALUE    -30073.
      * Distribution reply error
       78  SQL-RC-E30074             VALUE    -30074.
      * Communication error
       78  SQL-RC-E30080             VALUE    -30080.
      * Communication error
       78  SQL-RC-E30081             VALUE    -30081.
      * Security error
       78  SQL-RC-E30082             VALUE    -30082.
      * Attempt to change password failed
       78  SQL-RC-E30083             VALUE    -30083.
      * Remote operation invalid
       78  SQL-RC-E30090             VALUE    -30090.

      * A bind option is invalid
       78  SQL-RC-E30104             VALUE    -30104.
      * Invalid input for mulitple row INSERT
       78  SQL-RC-E30106             VALUE    -30106.
      * A failed connection has been re-established
       78  SQL-RC-E30108             VALUE    -30108.
      * Database Monitor SQLCODES

      * bad path specified for event monitor
       78  SQLM-RC-BAD-PATH          VALUE    -1612.
      * bad OPTION specified for event monitor
       78  SQLM-RC-BAD-OPTION        VALUE    -1613.
      * I/O error on activating event monitor
       78  SQLM-RC-IO-ERROR          VALUE    -1614.
      * Event monitor not activated
       78  SQLM-RC-NOT-ACTIVATED     VALUE    -1616.
      * Event monitor data files are full
       78  SQLM-RC-EVMON-FULL        VALUE    -1617.
      * Event monitor path is in use
       78  SQLM-RC-PATH-IN-USE       VALUE    -1618.
      * Cannot drop active event monitor
       78  SQLM-RC-CANNOT-DROP       VALUE    -1619.
      * Cannot flush event monitor
       78  SQLM-RC-CANNOT-FLUSH      VALUE    -1620.
      * Must commit to use event monitor
       78  SQLM-RC-MUST-COMMIT       VALUE    -1621.
      * Invalid state
       78  SQLM-RC-BAD-STATE         VALUE    -1622.

