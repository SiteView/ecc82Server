/******************************************************************************
** 
** Source File Name: SQLCODES
** 
** (C) COPYRIGHT International Business Machines Corp. 1987, 1997
** All Rights Reserved
** Licensed Materials - Property of IBM
** 
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
** 
** Function: Include File defining:
**             Labels for SQLCODES
** 
** Operating System:  Windows NT
** 
*******************************************************************************/
#ifndef SQL_H_SQLCODES
#define SQL_H_SQLCODES

#ifdef __cplusplus
extern "C" {
#endif


#include "sqlsystm.h"                  /* System dependent defines  */

/* SQL Return Codes in SQLCODE                                                */

#ifndef   SQL_RC_OK
#define SQL_RC_OK              0       /* successful execution                */
#endif

/* ------------ warnings ------------                                         */
#define SQL_RC_W012            12      /* correlation without qualification   */
#define SQL_RC_W020            20      /* unsupported bind/prep options       */
#define SQL_RC_W100            100     /* eof                                 */

#define SQL_RC_W117            117     /* wrong nbr of insert values          */
#define SQL_RC_W139            139     /* duplicate column conistraint        */
#define SQL_RC_W143            143     /* invalid syntax ignored              */

#define SQL_RC_W204            204     /* undefined name                      */
#define SQL_RC_W205            205     /* not a column                        */
#define SQL_RC_W206            206     /* not a column of referenced tables   */
#define SQL_RC_W217            217     /* explain mode incompatible           */
#define SQL_RC_W222            222     /* hole detected during fetch of       */
                                       /* cursor                              */
#define SQL_RC_W231            231     /* cursor position prevents FETCH      */
                                       /* current row                         */
#define SQL_RC_W236            236     /* not enough sqlvars, none filled in  */
#define SQL_RC_W237            237     /* distinct type: not enough sqlvars,  */
                                       /* some filled in                      */
#define SQL_RC_W238            238     /* LOB type: not enough sqlvars, none  */
                                       /* filled in                           */
#define SQL_RC_W239            239     /* distinct type: not enough sqlvars,  */
                                       /* none filled in                      */
#define SQL_RC_W280            280     /* new view replaced old               */
#define SQL_RC_W293            293     /* error accessing container           */

#define SQL_RC_W347            347     /* possible infinite loop              */
#define SQL_RC_W360            360     /* DATALINK value may not be valid     */
#define SQL_RC_W385            385     /* SQLSTATE or SQLCODE may be over     */
                                       /* written                             */

#define SQL_RC_W403            403     /* alias target is not defined         */
#define SQL_RC_W408            408     /* invalid data type for column        */
#define SQL_RC_W434            434     /* clause value has been replaced      */
#define SQL_RC_W437            437     /* Sub-optimal query                   */
#define SQL_RC_W440            440     /* No function with compatible arg     */
#define SQL_RC_W445            445     /* Function has truncated value        */
#define SQL_RC_W447            447     /* Create UDF contains redundant keyw  */
#define SQL_RC_W464            464     /* Procedure returned too many query   */
                                       /* result sets                         */
#define SQL_RC_W462            462     /* UDF returns a warning sqlstate      */
#define SQL_RC_W466            466     /* one or more results sets for        */
                                       /* stored procedure                    */
#define SQL_RC_W467            467     /* Another result set exists for a     */
                                       /* stored procedure                    */
#define SQL_RC_W474            474     /* DDL:could truncate value at         */
                                       /* runtime                             */
#define SQL_RC_W477            477     /* DDL:could truncate value at         */
                                       /* runtime                             */
#define SQL_RC_W494            494     /* Number of result sets greater than  */
                                       /* number of locators                  */

#define SQL_RC_W541            541     /* duplicate referential constraint    */
#define SQL_RC_W551            551     /* authorization error w/obj insert    */
#define SQL_RC_W552            552     /* auth error w/o obj ins              */
#define SQL_RC_W556            556     /* revoke stmt denied--priv not held   */
#define SQL_RC_W558            558     /* revoke stmt denied--has CONTROL     */
#define SQL_RC_W570            570     /* some privileges were not granted    */
#define SQL_RC_W585            585     /* Duplicate schema name               */
#define SQL_RC_W595            595     /* Isolation level escalated           */
#define SQL_RC_W598            598     /* existing index used                 */
#define SQL_RC_W599            599     /* compare func not created for lstr   */

#define SQL_RC_W605            605     /* index already exists                */

#define SQL_RC_W799            799     /* set stmt not supported              */

#define SQL_RC_W803            803     /* duplicate row warning               */

#define SQL_RC_W965            965     /* unknown SQL warning another         */
                                       /* product                             */
#define SQL_RC_W997            997     /* XA informational message            */

#define SQL_RC_W1138           1138    /* already existing index was          */
                                       /* migrated                            */
#define SQL_RC_W1140           1140    /* estimate cost exceeds resource      */
                                       /* limit warning threshold             */

#define SQL_RC_W1161           1161    /* DataLink column not defined on DLM  */
#define SQL_RC_W1162           1162    /* DLM down during exception           */
                                       /* processing                          */

#define SQL_RC_W1165           1165    /* value not within host variable      */
                                       /* data type range                     */
#define SQL_RC_W1166           1166    /* Division by zero                    */
#define SQL_RC_W1167           1167    /* Arithmetic overflow or arithmetic   */
                                       /* exception                           */
#define SQL_RC_W1179           1179    /* object may require invoker to have  */
                                       /* privileges on data source object    */
#define SQL_RC_W1196           1196    /* DLM down during backup              */

#define SQL_RC_W1251           1251    /* XA no data to return from recover   */
#define SQL_RC_W1362           1362    /* Parameter not changed dynamically   */
#define SQL_RC_W1363           1363    /* Parameter not changed dynamically   */
#define SQL_RC_W1364           1364    /* Parameter does not support          */
                                       /* AUTOMATIC                           */

#define SQL_RC_W1440           1440    /* WITH GRANT OPTION ignored           */
#define SQL_RC_ERR_DB_TERM     1475    /* system error on DB termination      */
#define SQL_RC_W1479           1479    /* Invalid cursor position             */

#define SQL_RC_W1530           1530    /* value of DEGREE ignored             */
#define SQL_RC_W1580           1580    /* Trailing blanks are truncated       */
#define SQL_RC_W1594           1594    /* data remains unverified             */

#define SQLM_RC_NOT_SET        1615    /* Event monitor state not changed     */
#define SQL_RC_W1625           1625    /* Monitor: No conversion from source  */
                                       /* cp to target cp                     */
#define SQL_RC_W1626           1626    /* Monitor: code page conversion       */
                                       /* overflow                            */
#define SQL_RC_W1627           1627    /* Monitor: V5 snapshot returned on    */
                                       /* V6 request                          */
#define SQL_RC_W1628           1628    /* Monitor: A remote snapshot          */
                                       /* operation failed                    */
#define SQL_RC_W1629           1629    /* Monitor: A get switches operation   */
                                       /* ran out of buffer space             */

#define SQL_RC_W1758           1758    /* containers not designated for       */
                                       /* specific nodes are not used         */
#define SQL_RC_W1759           1759    /* Redistribute required to change     */
                                       /* data partitioning                   */
#define SQL_RC_W1765           1765    /* Index creation, recreation, or      */
                                       /* reorganization may not be           */
                                       /* recovered on secondary database     */
                                       /* server                              */
#define SQL_RC_W1766           1766    /* LOGINDEXBUILD has not been enabled  */
#define SQL_RC_W1790           1790    /* No default primary table space      */
                                       /* exists                              */
#define SQL_RC_W1792           1792    /* The stats for the specified         */
                                       /* nicknames were not updated          */

#define SQL_RC_W1821           1821    /* LOB value retrieved may have been   */
                                       /* changed                             */
#define SQL_RC_W1824           1824    /* base tables of UNION ALL may be     */
                                       /* same table                          */
#define SQL_RC_W1844           1844    /* Data for a column was truncated     */

#define SQL_RC_W2077           2077    /* Reconcile pending on some DLMs      */

#define SQL_RC_W2314           2314    /* Statistics are in an inconsistent   */
                                       /* state                               */
#define SQL_RC_W2316           2316    /* The statistics profile has exceed   */
                                       /* the max size                        */

#define SQL_RC_W3274           3274    /* The database was not catalogued in  */
                                       /* LDAP                                */
#define SQL_RC_W3275           3275    /* The database was not uncatalogued   */
                                       /* in LDAP                             */
#define SQL_RC_W3283           3283    /* Protocol info was not updated in    */
                                       /* LDAP                                */

#define SQL_RC_W7035           7035    /* SQL procedure binary not in         */
                                       /* catalog                             */

#define SQL_RC_W8006           8006    /* evaluation period started           */
#define SQL_RC_W8007           8007    /* db2 is in evaluation period         */
#define SQL_RC_W8009           8009    /* concurrent user limit exceeded -    */
                                       /* DB2 Workgroup                       */
#define SQL_RC_W8010           8010    /* concurrent user limit exceeded -    */
                                       /* DB2 Connect                         */
#define SQL_RC_W8012           8012    /* concurrent user limit exceeded -    */
                                       /* DB2 Enterprise                      */
#define SQL_RC_W8013           8013    /* concurrent connection limit         */
                                       /* exceeded - DB2 Connect              */
#define SQL_RC_W8017           8017    /* processor license limit exceeded    */
#define SQL_RC_W8018           8018    /* concurrent user license limit       */
                                       /* exceeded                            */
#define SQL_RC_W8020           8020    /* concurrent connectors limit         */
                                       /* exceeded                            */
#define SQL_RC_W8021           8021    /* this connector is not registered    */

#define SQL_RC_W20015          20015   /* transform for type not defined      */
#define SQL_RC_W20059          20059   /* summary table cannot be used to     */
                                       /* optimize query                      */
#define SQL_RC_W20090          20090   /* DATALINK attribute only for typed   */
                                       /* table or type view                  */

#define SQL_RC_W20109          20109   /* Error in debugger support           */
#define SQL_RC_W20114          20114   /* Column is not long enough           */
#define SQL_RC_W20140          20140   /* ABBREVIATE column attribute         */
                                       /* ignored                             */
#define SQL_RC_W20149          20149   /* buffer pool configuration complete  */
#define SQL_RC_W20156          20156   /* Event Monitor activated; info may   */
                                       /* be lost                             */
#define SQL_RC_W20159          20159   /* Isolation clause is ignored.        */
#define SQL_RC_W20160          20160   /* Authorization granted to USER       */
#define SQL_RC_W20161          20161   /* Column name is invalid for event    */
                                       /* monitor table                       */
#define SQL_RC_W20169          20169   /* The buffer pool is not started.     */
#define SQL_RC_W20173          20173   /* Event monitor created; target       */
                                       /* tables already exists               */
#define SQL_RC_W20189          20189   /* Insufficient memory; bufferpool     */
                                       /* operation will not take effect.     */

#define SQL_RC_W20206          20206   /* procedure returned too many         */
                                       /* results                             */
#define SQL_RC_W20225          20225   /* The buffer pool drop operation      */
                                       /* will not take effect immediately    */
#define SQL_RC_W20271          20271   /* The name was truncated              */
#define SQL_RC_W20274          20274   /* Some nickname stats cannot be       */
                                       /* updated                             */
#define SQL_RC_W20277          20277   /* Characters truncated during code    */
                                       /* page conversion                     */
#define SQL_RC_W20278          20278   /* View cannot be used to optimize     */
                                       /* query processing                    */
#define SQL_RC_W20287          20287   /* The specified cached statement is   */
                                       /* different that the current          */
                                       /* environment                         */
#define SQL_RC_W20280          20280   /* Insufficient permission to create   */
                                       /* or write to file                    */

#define SQL_RC_W30101          30101   /* Rebind options ignored              */
#define SQL_RC_W30102          30102   /* Connection disabled                 */

/* ------------ errors ------------                                           */
#define SQL_RC_E007            -7      /* illegal character                   */
#define SQL_RC_E010            -10     /* string constant not terminated      */
#define SQL_RC_E013            -13     /* cursor or statement name = ""       */
#define SQL_RC_E017            -17     /* function or method does not end     */
                                       /* with RETURN                         */
#define SQL_RC_E029            -29     /* INTO clause required                */
#define SQL_RC_E051            -51     /* maximum SQL statements              */
#define SQL_RC_E056            -56     /* SQLSTATE and SQLCODE declaration    */
                                       /* in wrong scope                      */
#define SQL_RC_E057            -57     /* RETURN statement must include a     */
                                       /* value                               */
#define SQL_RC_E058            -58     /* data type for RETURN value must be  */
                                       /* INTEGER                             */
#define SQL_RC_E078            -78     /* parameter names not specified for   */
                                       /* routine                             */
#define SQL_RC_E079            -79     /* global temp table schema must be    */
                                       /* SESSION                             */
#define SQL_RC_E084            -84     /* bad SQL statement                   */
#define SQL_RC_E097            -97     /* data type not supported in SQL      */
                                       /* routine                             */

#define SQL_RC_E101            -101    /* statement too long                  */
#define SQL_RC_E102            -102    /* string constant too long            */
#define SQL_RC_E103            -103    /* invalid numeric literal             */
#define SQL_RC_E104            -104    /* invalid character/token             */
#define SQL_RC_E105            -105    /* invalid string constant             */
#define SQL_RC_E106            -106    /* incomplete statement                */
#define SQL_RC_E107            -107    /* name too long                       */
#define SQL_RC_E108            -108    /* name has too many qualifiers        */
#define SQL_RC_E109            -109    /* clause not permitted                */
#define SQL_RC_E110            -110    /* invalid hex constant                */
#define SQL_RC_E111            -111    /* no column name                      */
#define SQL_RC_E112            -112    /* operand is SQL function             */
#define SQL_RC_E113            -113    /* identifier contains invalid         */
                                       /* character                           */
#define SQL_RC_E117            -117    /* wrong nbr of insert values          */
#define SQL_RC_E118            -118    /* object table in from clause         */
#define SQL_RC_E119            -119    /* column not in group by              */
#define SQL_RC_E120            -120    /* clause includes SQL fn              */
#define SQL_RC_E121            -121    /* dup column name                     */
#define SQL_RC_E122            -122    /* no group by                         */
#define SQL_RC_NOTCONST        -123    /* parameter must be constant          */
#define SQL_RC_E125            -125    /* no result column                    */
#define SQL_RC_E127            -127    /* dup distinct                        */
#define SQL_RC_E129            -129    /* too many table names                */
#define SQL_RC_INVESC          -130    /* invalid escape character            */
#define SQL_RC_E131            -131    /* incompatible data for like          */
#define SQL_RC_E132            -132    /* like predicate invalid              */
#define SQL_RC_E134            -134    /* improper use of long string         */
#define SQL_RC_E135            -135    /* input must be a host variable or    */
                                       /* NULL                                */
#define SQL_RC_E137            -137    /* length of concat too long           */
#define SQL_RC_E138            -138    /* substr arg out of range             */
#define SQL_RC_E142            -142    /* syntax not supported                */
#define SQL_RC_E146            -146    /* unsupported ANSI syntax             */
#define SQL_RC_E150            -150    /* view not updatable                  */
#define SQL_RC_E151            -151    /* column not updatable                */
#define SQL_RC_E152            -152    /* actual constraint type is not       */
                                       /* expected constraint type            */
#define SQL_RC_E153            -153    /* no column list                      */
#define SQL_RC_E155            -155    /* trigger trans tbl not modifiable    */
#define SQL_RC_E156            -156    /* command not allowed on view         */
#define SQL_RC_E157            -157    /* view name in foreign key            */
#define SQL_RC_E158            -158    /* nbr of columns does not match       */
#define SQL_RC_E159            -159    /* drop view on table                  */
#define SQL_RC_E160            -160    /* with check not allowed on view      */
#define SQL_RC_E161            -161    /* with check violation                */
#define SQL_RC_E170            -170    /* nbr of arguments invalid            */
#define SQL_RC_E171            -171    /* argument invalid                    */
#define SQL_RC_E172            -172    /* function name invalid               */
#define SQL_RC_E176            -176    /* translate scalar argument invalid   */
#define SQL_RC_E180            -180    /* datetime syntax invalid             */
#define SQL_RC_E181            -181    /* datetime value invalid              */
#define SQL_RC_E182            -182    /* datetime arithmetic invalid         */
#define SQL_RC_E183            -183    /* datetime arithmetic out of range    */
#define SQL_RC_DTMEREG         -187    /* datetime register is invalid        */
#define SQL_RC_E190            -190    /* data type or length of column       */
                                       /* incompatible                        */
#define SQL_RC_DBCSTRUNC       -191    /* truncated DBCS character found      */
#define SQL_RC_E193            -193    /* NOT NULL needs DEFAULT              */
#define SQL_RC_E197            -197    /* no qualified columns in ORDER BY    */
#define SQL_RC_E198            -198    /* no statement text                   */
#define SQL_RC_E199            -199    /* illegal use of reserved word        */

#define SQL_RC_E203            -203    /* ambiguous column reference          */
#define SQL_RC_E204            -204    /* undefined name                      */
#define SQL_RC_E205            -205    /* not a column                        */
#define SQL_RC_E206            -206    /* not a column of referenced tables   */
#define SQL_RC_E207            -207    /* cannot orderby column name w/union  */
#define SQL_RC_E208            -208    /* column not part of result table     */
#define SQL_RC_E212            -212    /* duplicate table designator          */
#define SQL_RC_E214            -214    /* bad expression in group/by or       */
                                       /* order by                            */
#define SQL_RC_E216            -216    /* number of elements does not match   */
#define SQL_RC_E219            -219    /* required explain table not exist    */
#define SQL_RC_E220            -220    /* explain table has improper def      */
#define SQL_RC_E222            -222    /* update or delete against a hole     */
#define SQL_RC_E224            -224    /* result table does not agree with    */
                                       /* base table                          */
#define SQL_RC_E225            -225    /* FETCH against non-scrollable        */
                                       /* cursor                              */
#define SQL_RC_E227            -227    /* cursor has unknown position         */
#define SQL_RC_E228            -228    /* FOR UPDATE specified for read-only  */
                                       /* cursor                              */
#define SQL_RC_E242            -242    /* duplicate object in list            */
#define SQL_RC_E243            -243    /* SENSITIVE cursor cannot be defined  */
                                       /* for SELECT                          */
#define SQL_RC_E244            -244    /* SENSITIVITY not valid for cursor    */
#define SQL_RC_E257            -257    /* raw device containers not allowed   */
#define SQL_RC_E258            -258    /* cannot add container to pool        */
#define SQL_RC_E259            -259    /* container map too big               */
#define SQL_RC_E260            -260    /* partition key has long field        */
#define SQL_RC_E261            -261    /* node in use, cannot drop            */
#define SQL_RC_E262            -262    /* multinode table without part key    */
#define SQL_RC_E263            -263    /* invalid node range                  */
#define SQL_RC_E264            -264    /* multinode tbl, cannot drop part     */
#define SQL_RC_E265            -265    /* duplicate node name/number          */
#define SQL_RC_E266            -266    /* node not defined                    */
#define SQL_RC_E268            -268    /* operatn not done because rebalance  */
#define SQL_RC_E269            -269    /* too many nodegroups                 */
#define SQL_RC_E270            -270    /* function not supported              */
#define SQL_RC_E271            -271    /* Index file missing or invalid       */
#define SQL_RC_E276            -276    /* database in restore pending state   */
#define SQL_RC_E279            -279    /* connection terminated during        */
                                       /* COMMIT                              */
#define SQL_RC_E281            -281    /* cannot add containers to            */
                                       /* tablespace                          */
#define SQL_RC_E282            -282    /* cannot drop tblspace, tbl conflict  */
#define SQL_RC_E283            -283    /* cannot drop only temp tablespace    */
#define SQL_RC_E284            -284    /* invalid tablespace type for clause  */
#define SQL_RC_E285            -285    /* all table parts must be in tblspce  */
#define SQL_RC_E286            -286    /* need default tblspce for new        */
                                       /* tables                              */
#define SQL_RC_E287            -287    /* SYSCATSPACE not for user objects    */
#define SQL_RC_E288            -288    /* long tablespace cannot use system   */
#define SQL_RC_E289            -289    /* cannot allocate new pages in        */
                                       /* tablespace                          */
#define SQL_RC_E290            -290    /* access to tablespace not allowed    */
#define SQL_RC_E291            -291    /* invalid state transition            */
#define SQL_RC_E292            -292    /* cannot create internal db file      */
#define SQL_RC_E293            -293    /* error accessing container           */
#define SQL_RC_E294            -294    /* container already in use            */
#define SQL_RC_E295            -295    /* container names too long            */
#define SQL_RC_E296            -296    /* tablespace limit exceeded           */
#define SQL_RC_E297            -297    /* container pathname too long         */
#define SQL_RC_E298            -298    /* bad container pathname              */
#define SQL_RC_E299            -299    /* container already added             */

#define SQL_RC_E301            -301    /* host variable has invalid type      */
#define SQL_RC_E302            -302    /* host variable value too large       */
#define SQL_RC_E303            -303    /* data types not comparable           */
#define SQL_RC_E304            -304    /* value not in range of host var      */
#define SQL_RC_E305            -305    /* host var cannot be null             */
#define SQL_RC_E308            -308    /* host var limit reached              */
#define SQL_RC_E309            -309    /* host var should not be null         */
#define SQL_RC_E311            -311    /* length of host var is negative      */
#define SQL_RC_E312            -312    /* unusable host variable              */
#define SQL_RC_E313            -313    /* wrong nbr of host variables         */
#define SQL_RC_E332            -332    /* no conversn source-cp to target-cp  */
#define SQL_RC_E334            -334    /* conversion overflow                 */
#define SQL_RC_E336            -336    /* decimal scale must be zero          */
#define SQL_RC_E338            -338    /* ON clause not valid for Outer Join  */
#define SQL_RC_E340            -340    /* duplicate common table expression   */
#define SQL_RC_E341            -341    /* cyclic ref between comm tbl exp     */
#define SQL_RC_E342            -342    /* comm tbl exp use UNION ALL          */
#define SQL_RC_E343            -343    /* col names required                  */
#define SQL_RC_E344            -344    /* columns must match exactly          */
#define SQL_RC_E345            -345    /* cannot use GROUP BY or HAVING here  */
#define SQL_RC_E346            -346    /* invalid reference to comm tbl exp   */
#define SQL_RC_E348            -348    /* cannot use identity column or       */
                                       /* sequence                            */
#define SQL_RC_E349            -349    /* different sequence expressions in   */
                                       /* INSERT                              */
#define SQL_RC_E350            -350    /* LOB col cannot be idx, key, constr  */
#define SQL_RC_E351            -351    /* LOB col cannot be selected by DRDA  */
#define SQL_RC_E352            -352    /* LOB col cannot be inserted by DRDA  */
#define SQL_RC_E355            -355    /* LOB col cannot be logged            */
#define SQL_RC_E357            -357    /* DataLink Error                      */
#define SQL_RC_E358            -358    /* DataLink Error                      */
#define SQL_RC_E359            -359    /* Range exhausted                     */
#define SQL_RC_E368            -368    /* DataLink Error                      */
#define SQL_RC_E370            -370    /* parameter must be named in CREATE   */
                                       /* FUNCTION statement                  */
#define SQL_RC_E372            -372    /* One identity column per table       */
#define SQL_RC_E373            -373    /* DEFAULT clause not allowed on       */
                                       /* identity column                     */
#define SQL_RC_E374            -374    /* clause not specified in CREATE      */
                                       /* FUNCTION                            */
#define SQL_RC_E388            -388    /* source and target are built-in      */
                                       /* types or same                       */
#define SQL_RC_E389            -389    /* invalid specific function instance  */
                                       /* in CREATE CAST                      */
#define SQL_RC_E390            -390    /* function is invalid in the context  */
                                       /* it is used                          */
#define SQL_RC_E391            -391    /* Invalid use of a row based          */
                                       /* function                            */
#define SQL_RC_E392            -392    /* SQLDA provided for fetch has been   */
                                       /* changed                             */
#define SQL_RC_E396            -396    /* Execute SQL statement during final  */
                                       /* call processing                     */

#define SQL_RC_E401            -401    /* operands not comparable             */
#define SQL_RC_E402            -402    /* invalid type for arithmetic op      */
#define SQL_RC_E403            -403    /* alias object undefined              */
#define SQL_RC_E404            -404    /* update/insert string too long       */
#define SQL_RC_E405            -405    /* numeric literal out of range        */
#define SQL_RC_E406            -406    /* derived value out of range          */
#define SQL_RC_E407            -407    /* column cannot be null               */
#define SQL_RC_E408            -408    /* invalid data type for column        */
#define SQL_RC_E409            -409    /* invalid operand for count           */
#define SQL_RC_E410            -410    /* float literal too long              */
#define SQL_RC_E412            -412    /* multiple columns in subquery        */
#define SQL_RC_E413            -413    /* overflow during data conversion     */
#define SQL_RC_E415            -415    /* incompatible data types for union   */
#define SQL_RC_E416            -416    /* long string in union                */
#define SQL_RC_E417            -417    /* invalid use of parameter markers    */
#define SQL_RC_E418            -418    /* parameter marker in select clause   */
#define SQL_RC_E419            -419    /* division produced negative scale    */
#define SQL_RC_E420            -420    /* invalid character in input string   */
#define SQL_RC_E421            -421    /* diff number of columns for union    */
#define SQL_RC_E423            -423    /* Invalid handle                      */
#define SQL_RC_E426            -426    /* dynamic COMMIT not valid            */
#define SQL_RC_E427            -427    /* dynamic ROLLBACK not valid          */
#define SQL_RC_E428            -428    /* DISCONNECT in unit of work          */
                                       /* notallow                            */
#define SQL_RC_E429            -429    /* Handle table full                   */
#define SQL_RC_E430            -430    /* UDF abnormal end                    */
#define SQL_RC_E431            -431    /* UDF interrupted                     */
#define SQL_RC_E432            -432    /* Parameter marker cannot have udf    */
                                       /* nm                                  */
#define SQL_RC_E433            -433    /* Value is too long                   */
#define SQL_RC_E435            -435    /* Inv SQLSTATE in RAISE_ERROR         */
#define SQL_RC_E436            -436    /* C language char string missing NUL  */
#define SQL_RC_E438            -438    /* App raised error                    */
#define SQL_RC_E439            -439    /* Error in UDF                        */
#define SQL_RC_E440            -440    /* No function with compatible arg     */
#define SQL_RC_E441            -441    /* Invalid use of DISTINCT with        */
                                       /* scalar                              */
#define SQL_RC_E442            -442    /* Error refering function in DML      */
#define SQL_RC_E443            -443    /* UDF returns error sqlstate          */
#define SQL_RC_E444            -444    /* Unable to access UDF function       */
#define SQL_RC_E448            -448    /* Exceed max number of parameters     */
#define SQL_RC_E449            -449    /* Invalid EXTERNAL NAME format        */
#define SQL_RC_E450            -450    /* UDF generates too long result       */
                                       /* value                               */
#define SQL_RC_E451            -451    /* DDL: invalid type for external UDF  */
#define SQL_RC_E452            -452    /* Hostvar file inaccessible           */
#define SQL_RC_E453            -453    /* Error in RETURNS and CAST FROM      */
#define SQL_RC_E454            -454    /* Duplicate UDF name and signature    */
#define SQL_RC_E455            -455    /* Diff schema for UDF name &          */
                                       /* specific                            */
#define SQL_RC_E456            -456    /* Duplicate specific name             */
#define SQL_RC_E457            -457    /* Name reserved for system use        */
#define SQL_RC_E458            -458    /* DDL refers UDF signature not found  */
#define SQL_RC_E459            -459    /* AS CAST use error                   */
#define SQL_RC_E461            -461    /* invalid CAST                        */
#define SQL_RC_E463            -463    /* UDF returns an invalid sqlstate     */
#define SQL_RC_E465            -465    /* Unable to start fenced UDF          */
#define SQL_RC_E469            -469    /* Invalid use of IN, OUT, or INOUT    */
                                       /* parameter                           */
#define SQL_RC_E470            -470    /* Could not pass NULL argument to     */
                                       /* procedure                           */
#define SQL_RC_E471            -471    /* Error occurred calling a routine    */
#define SQL_RC_E472            -472    /* Cursor left open by function or     */
                                       /* method                              */
#define SQL_RC_E473            -473    /* reserved object name                */
#define SQL_RC_E475            -475    /* Result type of source is different  */
#define SQL_RC_E476            -476    /* Reference to function is not        */
                                       /* unique                              */
#define SQL_RC_E478            -478    /* other obj depends on this obj       */
#define SQL_RC_E480            -480    /* procedure not yet called            */
#define SQL_RC_E481            -481    /* GROUP BY: element nested in         */
                                       /* element                             */
#define SQL_RC_E483            -483    /* Parm num not match with source      */
#define SQL_RC_E486            -486    /* Boolean type is system used only    */
#define SQL_RC_E487            -487    /* Attempt to excute an SQL statement  */
#define SQL_RC_E489            -489    /* Inv BOOLEAN in select list result   */
#define SQL_RC_E490            -490    /* Number outside the range of         */
                                       /* allowable values                    */
#define SQL_RC_E491            -491    /* DDL: missing reqired clause         */
#define SQL_RC_E492            -492    /* DDL: miss match with source type    */
#define SQL_RC_E493            -493    /* UDF returned bad date/time value    */
#define SQL_RC_E495            -495    /* estimated cost exceeds resource     */
                                       /* limit error threshold               */
#define SQL_RC_E499            -499    /* cursor already assigned to a        */
                                       /* result set                          */

#define SQL_RC_E501            -501    /* cursor not open (fetch/close)       */
#define SQL_RC_E502            -502    /* cursor already open                 */
#define SQL_RC_E503            -503    /* column not in update clause         */
#define SQL_RC_E504            -504    /* cursor name not defined             */
#define SQL_RC_E505            -505    /* cursor name already declared        */
#define SQL_RC_E507            -507    /* cursor not open (update/delete)     */
#define SQL_RC_E508            -508    /* cursor not on a row                 */
#define SQL_RC_E509            -509    /* table not same as for cursor        */
#define SQL_RC_E510            -510    /* table cannot be modified            */
#define SQL_RC_E511            -511    /* for update not allowed              */
#define SQL_RC_E514            -514    /* cursor not prepared                 */
#define SQL_RC_E516            -516    /* describe not a prepared statement   */
#define SQL_RC_E517            -517    /* prepared statment not a select      */
#define SQL_RC_E518            -518    /* execute not a prepared statement    */
#define SQL_RC_E519            -519    /* statement has an open cursor        */
#define SQL_RC_E525            -525    /* statement cannot be executed        */
#define SQL_RC_E526            -526    /* function does not apply to global   */
                                       /* temp tables                         */
#define SQL_RC_E528            -528    /* duplicate primary/unique key        */
#define SQL_RC_E530            -530    /* invalid foreign key value           */
#define SQL_RC_E531            -531    /* cannot update primary key           */
#define SQL_RC_E532            -532    /* delete is restricted                */
#define SQL_RC_E533            -533    /* multi-row insert not allowed        */
#define SQL_RC_E534            -534    /* multi-row update of pk              */
#define SQL_RC_E535            -535    /* multi-row delete not allowed        */
#define SQL_RC_E536            -536    /* descendent in subquery              */
#define SQL_RC_E537            -537    /* dup column in key def               */
#define SQL_RC_E538            -538    /* foreign key does not match pk       */
#define SQL_RC_E539            -539    /* table does not have primary key     */
#define SQL_RC_E540            -540    /* table does not have primary index   */
#define SQL_RC_E541            -541    /* duplicate referential constraint    */
#define SQL_RC_E542            -542    /* pk column cannot allow nulls        */
#define SQL_RC_E543            -543    /* restricted row delete               */
#define SQL_RC_E544            -544    /* check contraint violated            */
#define SQL_RC_E545            -545    /* check contraint not satisfied       */
#define SQL_RC_E546            -546    /* check contraint invalid             */
#define SQL_RC_E548            -548    /* check contraint invalid             */
#define SQL_RC_E549            -549    /* restricted stmt used with           */
                                       /* DYNAMICRULES BIND pkg               */
#define SQL_RC_E551            -551    /* authorization error w/obj insert    */
#define SQL_RC_E552            -552    /* auth error w/o obj ins              */
#define SQL_RC_E553            -553    /* SYSIBM qualifier                    */
#define SQL_RC_E554            -554    /* cannot grant privilege to self      */
#define SQL_RC_E555            -555    /* cannot revoke privilege from self   */
#define SQL_RC_E556            -556    /* revoke stmt denied--priv not held   */
#define SQL_RC_E557            -557    /* invalid combination of privileges   */
#define SQL_RC_E558            -558    /* revoke stmt denied--has CONTROL     */
#define SQL_RC_E562            -562    /* privilege not allowed for public    */
#define SQL_RC_E567            -567    /* invalid authorization id            */
#define SQL_RC_E569            -569    /* user/group ambiguity                */
#define SQL_RC_E572            -572    /* Inoperative package                 */
#define SQL_RC_E573            -573    /* contraint col not primary key       */
#define SQL_RC_E574            -574    /* DEFAULT invalid for column          */
#define SQL_RC_E575            -575    /* view is inoperative                 */
#define SQL_RC_E576            -576    /* Repetitive alias chain              */
#define SQL_RC_E577            -577    /* function or procedure not defined   */
                                       /* as MODIFIES SQL DATA                */
#define SQL_RC_E579            -579    /* function or procedure not defined   */
                                       /* as READS SQL DATA                   */
#define SQL_RC_E580            -580    /* Result of CASE expr cannot be NULL  */
#define SQL_RC_E581            -581    /* Data types of CASE expr incompat    */
#define SQL_RC_E582            -582    /* Inv predicate in CASE expr          */
#define SQL_RC_E583            -583    /* Variant or ext action function      */
#define SQL_RC_E584            -584    /* Inv use of NULL or DEFAULT          */
#define SQL_RC_E585            -585    /* Duplicate schema name               */
#define SQL_RC_E586            -586    /* Too many schema names               */
#define SQL_RC_E590            -590    /* Name in SQL procedure not unique    */

#define SQL_RC_E597            -597    /* Not authorized to update linked     */
                                       /* file                                */
#define SQL_RC_E600            -600    /* udf - dup func signature            */
#define SQL_RC_E601            -601    /* duplicate table/view name           */
#define SQL_RC_E602            -602    /* too many columns in index           */
#define SQL_RC_E603            -603    /* cannot create unique index          */
#define SQL_RC_E604            -604    /* invalid length, precision, scale    */
#define SQL_RC_E606            -606    /* authid does not own column or       */
                                       /* table                               */
#define SQL_RC_E607            -607    /* op not allowed on system tables     */
#define SQL_RC_E612            -612    /* duplicate column name               */
#define SQL_RC_E613            -613    /* primary key too long                */
#define SQL_RC_E614            -614    /* index key too long                  */
#define SQL_RC_E615            -615    /* object is in use and cannot be      */
                                       /* dropped                             */
#define SQL_RC_E620            -620    /* userid does not have appropriate    */
                                       /* dbspaces                            */
#define SQL_RC_E622            -622    /* clause invalid for this database    */
#define SQL_RC_E623            -623    /* clustering index already exists     */
                                       /* for table                           */
#define SQL_RC_E624            -624    /* table already has primary key       */
#define SQL_RC_E628            -628    /* DDL: multiple/conflict keywords     */
#define SQL_RC_E629            -629    /* foreign key is not nullable         */
#define SQL_RC_E631            -631    /* foreign key is too long             */
#define SQL_RC_E632            -632    /* delete rule restriction             */
#define SQL_RC_E633            -633    /* delete rule is restricted           */
#define SQL_RC_E634            -634    /* delete rule cannot be CASCADE       */
#define SQL_RC_E637            -637    /* dup pk or drop pk clause            */
#define SQL_RC_E638            -638    /* no column definitions               */
#define SQL_RC_E644            -644    /* invalid keyword value               */
#define SQL_RC_E647            -647    /* bufferpool not active               */
#define SQL_RC_E648            -648    /* invalid referential constraint      */
#define SQL_RC_E658            -658    /* object cannot be explicitely        */
                                       /* dropped                             */
#define SQL_RC_E659            -659    /* architectural size limit of object  */
#define SQL_RC_E667            -667    /* ref constraint does not hold        */
#define SQL_RC_E668            -668    /* table in CHECK PENDING state        */
#define SQL_RC_E669            -669    /* cannot drop primary key             */
#define SQL_RC_E670            -670    /* row length too large                */
#define SQL_RC_E672            -672    /* cannot drop table                   */
#define SQL_RC_E673            -673    /* primary key not unique              */
#define SQL_RC_E680            -680    /* too many columns for table          */
#define SQL_RC_E683            -683    /* incompatible column constraint      */
#define SQL_RC_E696            -696    /* invalid trigger definition          */
#define SQL_RC_E697            -697    /* invalid correlation name use        */

#define SQL_RC_E707            -707    /* object uses reserved name           */
#define SQL_RC_INV_REPL        -713    /* invalid replace value for sp reg    */
#define SQL_RC_E719            -719    /* Bind error for user; pkg already    */
                                       /* exists                              */
#define SQL_RC_E720            -720    /* Attempt to replace existing         */
                                       /* package                             */
#define SQL_RC_E721            -721    /* Pkg name with consistency tokens    */
                                       /* is not unique                       */
#define SQL_RC_E722            -722    /* Bind/rebind error for uesr; pkg     */
                                       /* does not exist                      */
#define SQL_RC_E723            -723    /* trigger error                       */
#define SQL_RC_E724            -724    /* max level of cascading              */
#define SQL_RC_E727            -727    /* Invalid implicit PREP or REBIND     */
#define SQL_RC_E740            -740    /* MODIFIES SQL DATA option not valid  */
                                       /* in context                          */
#define SQL_RC_E746            -746    /* Routine violates nested SQL         */
                                       /* statement rules                     */
#define SQL_RC_E750            -750    /* Table cannot be renamed             */
#define SQL_RC_E751            -751    /* statement not allowed in function   */
                                       /* or proc                             */
#define SQL_RC_E752            -752    /* Inoperative package                 */
#define SQL_RC_E773            -773    /* Case not found for CASE statement   */
#define SQL_RC_E774            -774    /* statement cannot be executed        */
                                       /* within an ATOMIC statement          */
#define SQL_RC_E776            -776    /* Use of cursor not valid in FOR      */
                                       /* statement                           */
#define SQL_RC_E777            -777    /* Nested compound statements not      */
                                       /* allowed                             */
#define SQL_RC_E778            -778    /* End label not same name as begin    */
                                       /* label                               */
#define SQL_RC_E779            -779    /* Label in ITERATE or LEAVE is not    */
                                       /* valid                               */
#define SQL_RC_E780            -780    /* UNDO in handler in compound         */
                                       /* statement                           */
#define SQL_RC_E781            -781    /* Condition in handler not defined    */
                                       /*                                     */
#define SQL_RC_E782            -782    /* Condition or SQLSTATE in handler    */
                                       /* not valid                           */
#define SQL_RC_E783            -783    /* SELECT list in FOR statement not    */
                                       /* valid                               */
#define SQL_RC_E785            -785    /* use of SQLCODE or SQLSTATE is not   */
                                       /* valid                               */
#define SQL_RC_E787            -787    /* RESIGNAL statement used outside     */
                                       /* handler                             */
#define SQL_RC_E788            -788    /* The same table row cannot be the    */
                                       /* target                              */
#define SQL_RC_E789            -789    /* type not supported in SQL           */
                                       /* procedures                          */
#define SQL_RC_E797            -797    /* Invalid triggered statement         */
#define SQL_RC_E798            -798    /* Insert value not allowed on         */
                                       /* generated always identity column    */

#define SQL_RC_E801            -801    /* divide by zero                      */
#define SQL_RC_E802            -802    /* arith overflow or divide by zero    */
#define SQL_RC_E803            -803    /* distinct violation                  */
#define SQL_RC_E804            -804    /* bad input parameters                */
#define SQL_RC_E805            -805    /* program not bound                   */
#define SQL_RC_E808            -808    /* inconsistent connect semantics      */
#define SQL_RC_E811            -811    /* more than one row/value             */
#define SQL_RC_E817            -817    /* statement would cause invalid       */
                                       /* update operation                    */
#define SQL_RC_E818            -818    /* time stamp conflict                 */
#define SQL_RC_E822            -822    /* invalid address in sqlda            */
#define SQL_RC_E838            -838    /* Dynamic statement requires a        */
                                       /* result area                         */
#define SQL_RC_E840            -840    /* too many items in list              */
#define SQL_RC_E842            -842    /* connection already exists           */
#define SQL_RC_E843            -843    /* connection does not exist           */
#define SQL_RC_E845            -845    /* cannot use CURRVAL before NEXTVAL   */
#define SQL_RC_E846            -846    /* Invalid identity specification      */
#define SQL_RC_E847            -847    /* can't change connection settings    */
#define SQL_RC_E859            -859    /* TM not for 2 phase commit apps      */
#define SQL_RC_E864            -864    /* Referential contstraint attempted   */
                                       /* to modify a table modified by SQL   */
                                       /* data change stmt                    */
#define SQL_RC_E865            -865    /* inv TM_DATABASE value               */
#define SQL_RC_E866            -866    /* connect redirect failed             */
#define SQL_RC_E868            -868    /* connection already exists           */
#define SQL_RC_E873            -873    /* objects with different encoding     */
                                       /* cannot be referenced                */
#define SQL_RC_E874            -874    /* CCSID parameter must match routine  */
                                       /* parameter                           */
#define SQL_RC_E880            -880    /* savepoint does not exist or is      */
                                       /* invalid                             */
#define SQL_RC_E881            -881    /* savepoint name cannot be reused     */
#define SQL_RC_E882            -882    /* savepoint does not exist            */

#define SQL_RC_E901            -901    /* non-fatal system error              */
#define SQL_RC_E902            -902    /* fatal error                         */
#define SQL_RC_E903            -903    /* commit failed, rollback             */
#define SQL_RC_E904            -904    /* resource unavailable                */
#define SQL_RC_E905            -905    /* resource limit exceeded             */
#define SQL_RC_E906            -906    /* function disabled                   */
#define SQL_RC_E907            -907    /* failed attempt to modify target     */
                                       /* table of MERGE stmt                 */
#define SQL_RC_E908            -908    /* BIND operation not allowed          */
#define SQL_RC_E909            -909    /* object deleted                      */
#define SQL_RC_E910            -910    /* drop pending                        */
#define SQL_RC_E911            -911    /* deadlock                            */
#define SQL_RC_E912            -912    /* too many lock requests              */
#define SQL_RC_E913            -913    /* dist env rollback                   */
#define SQL_RC_E917            -917    /* remote rebind from DRDA failed      */
#define SQL_RC_E918            -918    /* ROLLBACK required                   */
#define SQL_RC_E920            -920    /* reject DB connection to SA REQ      */
#define SQL_RC_E925            -925    /* COMMIT not allowed                  */
#define SQL_RC_E926            -926    /* ROLLBACK not allowed                */
#define SQL_RC_E930            -930    /* insufficient storage                */
#define SQL_RC_INODE           -931    /* all inodes used, system limit       */
#define SQL_RC_E950            -950    /* cursors active on dropped object    */
#define SQL_RC_E951            -951    /* table in use - alter disallowed     */
#define SQL_RC_E952            -952    /* user cancel                         */
#define SQL_RC_E953            -953    /* agent heap too small                */
#define SQL_RC_E954            -954    /* application heap too small          */
#define SQL_RC_E955            -955    /* sort heap error                     */
#define SQL_RC_E956            -956    /* database heap too small             */
#define SQL_RC_E958            -958    /* max number of files open            */
#define SQL_RC_E959            -959    /* server comm heap too small          */
#define SQL_RC_E960            -960    /* max nbr of files in database        */
#define SQL_RC_E964            -964    /* log file full                       */
#define SQL_RC_E966            -966    /* error opening sqlcode mapping file  */
#define SQL_RC_E967            -967    /* format error in sqlcode map file    */
#define SQL_RC_E968            -968    /* disk full                           */
#define SQL_RC_E969            -969    /* unknown sql error another product   */
#define SQL_RC_E970            -970    /* read-only file                      */

#define SQL_RC_E972_DB2AIX     -10019  /* incorrect diskette                  */
#define SQL_RC_E972_DB2OS2     -972    /* incorrect diskette                  */
#define SQL_RC_E972            SQL_RC_E972_DB2OS2 /* incorrect diskette       */

#define SQL_RC_E973            -973    /* out of memory error                 */
#define SQL_RC_W973            973     /* out of memory warning               */

#define SQL_RC_E974_DB2AIX     -10019  /* drive locked                        */
#define SQL_RC_E974_DB2OS2     -974    /* drive locked                        */
#define SQL_RC_E974            SQL_RC_E974_DB2OS2 /* drive locked             */

#define SQL_RC_QBACK           -975    /* quiesce backup                      */

#define SQL_RC_E976_DB2AIX     -10019  /* diskette door open                  */
#define SQL_RC_E976_DB2OS2     -976    /* diskette door open                  */
#define SQL_RC_E976            SQL_RC_E976_DB2OS2 /* diskette door open       */

#define SQL_RC_E977            -977    /* unknown commit state                */
#define SQL_RC_E978_DB2AIX     -10019  /* diskette write-protected            */
#define SQL_RC_E978_DB2OS2     -978    /* diskette write-protected            */
#define SQL_RC_E978            SQL_RC_E978_DB2OS2 /* diskette write           */
                                       /* protected                           */

#define SQL_RC_E979            -979    /* commit failed with SYNCPOINT NONE   */
#define SQL_RC_E980            -980    /* disk error                          */
#define SQL_RC_E982            -982    /* disk error on temp file             */
#define SQL_RC_E984            -984    /* unsuccessful commit or rollback     */
#define SQL_RC_E985            -985    /* file error - catalog file bad       */
#define SQL_RC_E986            -986    /* file error - file renamed           */
#define SQL_RC_E987            -987    /* application shared memory cannot    */
                                       /* be allocated                        */
#define SQL_RC_E989            -989    /* AFTER trigger cannot modify a row   */
                                       /* being modified by an SQL data       */
                                       /* change stmt                         */
#define SQL_RC_E990            -990    /* index structure limit error         */
#define SQL_RC_E992            -992    /* release number incompatible         */
#define SQL_RC_E994            -994    /* reserved                            */
#define SQL_RC_E996            -996    /* error freeing pages in DMS          */
                                       /* tablespace                          */
#define SQL_RC_E998            -998    /* General XA error                    */

#define SQL_RC_E1007           -1007   /* error finding pages in DMS          */
                                       /* tablespace                          */
#define SQL_RC_E1008           -1008   /* invalid tablespace id               */
#define SQL_RC_E1042           -1042   /* Unexpected system error             */
#define SQL_RC_E1046           -1046   /* Authid not valid                    */
#define SQL_RC_E1068           -1068   /* domain is not defined in            */
                                       /* DB2DOMAINLIST                       */
#define SQL_RC_E1092           -1092   /* Insufficient authority              */
#define SQL_RC_E1093           -1093   /* User not logged on                  */
#define SQL_RC_E1101           -1101   /* Remote database cannot be accessed  */
#define SQL_RC_E1139           -1139   /* table space maximum size exceeded   */
#define SQL_RC_E1141           -1141   /* inspect completes with error        */
                                       /* warnings                            */
#define SQL_RC_E1142           -1142   /* inspect fails with file in use      */
#define SQL_RC_E1143           -1143   /* inspect fails with file i/o error   */
#define SQL_RC_E1144           -1144   /* transaction rolled back due to      */
                                       /* failure creating index              */
#define SQL_RC_E1145           -1145   /* dynamically prepared statement not  */
                                       /* supported                           */
#define SQL_RC_E1146           -1146   /* There are no indexes on table       */
                                       /* <name>                              */
#define SQL_RC_E1148           -1148   /* Index needs refresh, but            */
                                       /* tablespace is in backup pending     */
                                       /* state                               */
#define SQL_RC_E1163           -1163   /* table cannot be enabled for data    */
                                       /* capture                             */
#define SQL_RC_E1169           -1169   /* An error occurred while explaining  */
                                       /* the statement.                      */
#define SQL_RC_E1178           -1178   /* FEDERATED does not reference        */
                                       /* nickname or OLE DB function         */
#define SQL_RC_E1180           -1180   /* UDF caused an OLE error             */
#define SQL_RC_E1181           -1181   /* UDF raised an exception             */
#define SQL_RC_E1182           -1182   /* UDF cannot initialize source        */
                                       /* object of OLE DB                    */
#define SQL_RC_E1183           -1183   /* UDF received OLE DB error           */
#define SQL_RC_E1184           -1184   /* EXPLAIN table(s) created with       */
                                       /* earlier EXPLAIN.DDL                 */
#define SQL_RC_E1185           -1185   /* FEDERATED clause needed to bind     */
                                       /* package                             */
#define SQL_RC_E1186           -1186   /* FEDERATED needed with nickname or   */
                                       /* OLE DB function                     */
#define SQL_RC_E1198           -1198   /* command is not supported in the     */
                                       /* current downlevel client-server     */
                                       /* configuration                       */
#define SQL_RC_E1199           -1199   /* Suboptimal performance. Reconnect   */
                                       /* to an alternate node.               */

#define SQL_RC_E1216           -1216   /* invalid use of graphic data         */
#define SQL_RC_E1217           -1217   /* REAL data type not supported        */
#define SQL_RC_E1218           -1218   /* no bufferpool pages available       */
#define SQL_RC_E1219           -1219   /* out of private memory               */
#define SQL_RC_E1226           -1226   /* max number of agents started        */
#define SQL_RC_E1227           -1227   /* Updatale cat values inv             */
#define SQL_RC_E1229           -1229   /* System error rollback               */
#define SQL_RC_E1233           -1233   /* Graphic data that is not UCS-2 is   */
                                       /* not supported                       */
#define SQL_RC_W1244           1244    /* Disconn at next commit              */
#define SQL_RC_E1245           -1245   /* Max client connects                 */
#define SQL_RC_E1247           -1247   /* XA TM uses syncpoint 2              */
#define SQL_RC_E1248           -1248   /* db not defined with TM              */
#define SQL_RC_E1288           -1288   /* Non-SQL requests not supported      */
                                       /* from this downlevel client          */
#define SQL_RC_E1290           -1290   /* DB2CLIENTCOMM env var inv           */
#define SQL_RC_E1291           -1291   /* direcory services error             */
#define SQL_RC_E1293           -1293   /* global dir error                    */
#define SQL_RC_E1294           -1294   /* global dir path invalid             */
#define SQL_RC_E1295           -1295   /* global dir router invalid           */

#define SQL_RC_E1301           -1301   /* Error processing keytab file        */
#define SQL_RC_E1302           -1302   /* DCE principal to authid mapping     */
                                       /* error                               */
#define SQL_RC_E1305           -1305   /* Internal DCE error                  */
#define SQL_RC_E1309           -1309   /* Invalid server principal name       */
#define SQL_RC_E1322           -1322   /* Error writing to audit log          */
#define SQL_RC_E1323           -1323   /* Error accessing audit cfg           */
#define SQL_RC_E1336           -1336   /* Remote host not found               */
#define SQL_RC_E1339           -1339   /* Not Atomic Compound SQL error(s)    */

#define SQL_RC_E1365           -1365   /* Plugin processing failed on the     */
                                       /* server                              */
#define SQL_RC_E1366           -1366   /* Plugin processing failed on the     */
                                       /* client                              */
#define SQL_RC_E1367           -1367   /* Operating system does not support   */
                                       /* resource policy definition.         */
#define SQL_RC_E1368           -1368   /* Invalid resource policy             */
                                       /* configuration                       */

#define SQL_RC_E1380           -1380   /* Unexpected Kerberos security error  */
#define SQL_RC_E1381           -1381   /* Security support interface not      */
                                       /* available                           */
#define SQL_RC_E1382           -1382   /* The Kerberos support not available  */
#define SQL_RC_E1383           -1383   /* Invalid target principal name       */
#define SQL_RC_E1384           -1384   /* Unable to complete mutual           */
                                       /* authentication                      */

#define SQL_RC_E1400           -1400   /* Auth type unsupported               */
#define SQL_RC_E1402           -1402   /* Unexpected auth system error        */
#define SQL_RC_E1403           -1403   /* Username or password incorrect      */
#define SQL_RC_E1404           -1404   /* Password expired                    */
#define SQL_RC_E1405           -1405   /* Error communicating to auth server  */
#define SQL_RC_E1420           -1420   /* too many concat operators           */
#define SQL_RC_E1421           -1421   /* MBCS conversion error               */
#define SQL_RC_E1422           -1422   /* Container wrong size                */
#define SQL_RC_E1423           -1423   /* no blobs for dwn lvl cl             */
#define SQL_RC_E1424           -1424   /* too many transition tbles           */
#define SQL_RC_E1434           -1434   /* 32 / 64 bit connect incompatible    */

#define SQL_RC_E1462           -1462   /* only valid for sync mgr connection  */

#define SQL_RC_E1550           -1550   /* SUSPEND WRITE command failed        */
#define SQL_RC_E1551           -1551   /* RESUME WRITE command failed         */
#define SQL_RC_E1552           -1552   /* RESTART command failed; database    */
                                       /* is in SUSPEND WRITE                 */
#define SQL_RC_E1553           -1553   /* DB2STOP command failed; database    */
                                       /* is in SUSPEND WRITE                 */
#define SQL_RC_E1581           -1581   /* table cannot be in append mode      */
                                       /* with clustering index               */
#define SQL_RC_E1582           -1582   /* pagesize of table space doesn't     */
                                       /* match bufferpool                    */
#define SQL_RC_E1583           -1583   /* pagesize not supported              */
#define SQL_RC_E1584           -1584   /* temporary table space cannot be     */
                                       /* found                               */
#define SQL_RC_E1585           -1585   /* No temp table space with            */
                                       /* sufficient page size                */
#define SQL_RC_E1591           -1591   /* ON option of SET INTEGRITY invalid  */
#define SQL_RC_E1592           -1592   /* INCREMENTAL option invalid          */
#define SQL_RC_E1593           -1593   /* REMAIN PENDING option invalid       */
#define SQL_RC_E1595           -1595   /* table integrity cannot be checked   */
#define SQL_RC_E1596           -1596   /* WITH EMPTY TABLE option not         */
                                       /* allowed because of dependent        */
                                       /* REFRESH IMMEDIATE table             */

#define SQL_RC_E1749           -1749   /* The table is not created with NOT   */
                                       /* LOGGED INITIALLY                    */

#define SQL_RC_E1750           -1750   /* foreign keys not allowed for        */
                                       /* tables created not logged           */
                                       /* initially                           */
#define SQL_RC_E1751           -1751   /* nodegroups must have at least one   */
                                       /* node                                */
#define SQL_RC_E1752           -1752   /* tablespace not created in the       */
                                       /* correct nodegroup                   */
#define SQL_RC_E1753           -1753   /* a node does not have the complete   */
                                       /* temporary tablespaces               */
#define SQL_RC_E1754           -1754   /* all tablespace in CREATE TABLE      */
                                       /* must be in the same nodegroup       */
#define SQL_RC_E1756           -1756   /* more than one clause specifies      */
                                       /* containers without ON NODES clause  */
#define SQL_RC_E1757           -1757   /* missing USING clause without ON     */
                                       /* NODES clause                        */
#define SQL_RC_E1760           -1760   /* missing clause for create stored    */
                                       /* procedure                           */
#define SQL_RC_E1761           -1761   /* nodegroup not defined for buffer    */
                                       /* pool                                */
#define SQL_RC_E1762           -1762   /* not enough disk space for connect   */
#define SQL_RC_E1763           -1763   /* cannot specify multiple ALTER       */
                                       /* TABLESPACE actions                  */
#define SQL_RC_E1764           -1764   /* cannot specify a smaller container  */
                                       /* size                                */

#define SQL_RC_E1803           -1803   /* operation cannot be executed in No  */
                                       /* Package Lock mode                   */
#define SQL_RC_E1816           -1816   /* Wrapper cannot be used to access    */
                                       /* data source                         */
#define SQL_RC_E1817           -1817   /* CREATE SERVER statement does not    */
                                       /* identify data source                */
#define SQL_RC_E1818           -1818   /* ALTER SERVER statement cannot be    */
                                       /* processed                           */
#define SQL_RC_E1819           -1819   /* DROP SERVER statement cannot be     */
                                       /* processed                           */
#define SQL_RC_E1820           -1820   /* Action on LOB value failed          */
#define SQL_RC_E1822           -1822   /* Unexpected error from data source   */
#define SQL_RC_E1823           -1823   /* No data type mapping exists for     */
                                       /* server                              */
#define SQL_RC_E1825           -1825   /* Statement cannot be handled by      */
                                       /* Datajoiner                          */
#define SQL_RC_E1826           -1826   /* Invalid value for system catalog    */
                                       /* column                              */
#define SQL_RC_E1827           -1827   /* user mapping undefined              */
#define SQL_RC_E1828           -1828   /* server option undefined             */

#define SQL_RC_NO_TCPIP        -1468   /* TCPIP not running                   */
#define SQL_RC_NODE_INVALID    -1469   /* invalid node                        */
#define SQL_RC_DB2NODE_INVALID -1470   /* DB2NODE env var invalid             */
#define SQL_RC_LOG_MISMATCH    -1471   /* nodes out of sync                   */
#define SQL_RC_TIMEDIFF_CA     -1472   /* connect fail, system clocks out of  */
                                       /* sync                                */
#define SQL_RC_TIMEDIFF_CO     -1473   /* commit fail, system clocks out of   */
                                       /* sync                                */
#define SQL_RC_TIMEDIFF_W      -1474   /* transaction OK, system clocks out   */
                                       /* of sync                             */
#define SQL_RC_E1476           -1476   /* rollback on table error             */
#define SQL_RC_E1477           -1477   /* table not accessible                */

#define SQL_RC_E1590           -1590   /* LONG not allowed on devices         */
#define SQL_RC_E1791           -1791   /* Specified definition, schema, or    */
                                       /* nickname does not exist             */
#define SQL_RC_E1830           -1830   /* RETURNS clause must be specified    */
                                       /* before EXPRESSION AS                */
#define SQL_RC_E1831           -1831   /* Cannot update table stats for       */
                                       /* subtable                            */
#define SQL_RC_E1832           -1832   /* data filter function cannot be      */
                                       /* LANGUAGE SQL                        */
#define SQL_RC_E1833           -1833   /* Connection to Extended Search       */
                                       /* Server failed                       */
#define SQL_RC_E1834           -1834   /* User-defined column identical to    */
                                       /* fixed column                        */
#define SQL_RC_E1835           -1835   /* Extended Search  object could not   */
                                       /* be found                            */
#define SQL_RC_E1836           -1836   /* No column mapping exist to          */
                                       /* Extended Search field               */
#define SQL_RC_E1837           -1837   /* Required option can not be dropped  */
#define SQL_RC_E1838           -1838   /* Statement is not a valid Extended   */
                                       /* Search query                        */
#define SQL_RC_E1839           -1839   /* One ore more search parameter are   */
                                       /* not valid                           */
#define SQL_RC_E1840           -1840   /* Option cannot be added to object    */
#define SQL_RC_E1841           -1841   /* The value cannot be changed for     */
                                       /* the object                          */
#define SQL_RC_E1842           -1842   /* Option is not valid                 */
#define SQL_RC_E1843           -1843   /* The operator is not supported       */
#define SQL_RC_E1846           -1846   /* The option conflicts with the       */
                                       /* object                              */
#define SQL_RC_E1847           -1847   /* Template substiution error          */
#define SQL_RC_E1870           -1870   /* Key sequence column is out of       */
                                       /* range                               */
#define SQL_RC_E1871           -1871   /* Function not supported on range     */
                                       /* clustered tables                    */
#define SQL_RC_E1881           -1881   /* option is not valid for this data   */
                                       /* source                              */
#define SQL_RC_E1882           -1882   /* option is not valid for this data   */
                                       /* source                              */
#define SQL_RC_E1883           -1883   /* Missing option for this data        */
                                       /* source                              */
#define SQL_RC_E1884           -1884   /* Option specified more than once     */
#define SQL_RC_E1885           -1885   /* option is already defined           */
#define SQL_RC_E1886           -1886   /* option has not been added           */
#define SQL_RC_E1887           -1887   /* SPECIFICATION ONLY clause required  */

#define SQL_RC_E2078           -2078   /* cannot add/drop DB2 Data Links      */
                                       /* Manager                             */

#define SQL_RC_E2180           -2180   /* incorrect syntax or password for    */
                                       /* filtering                           */
#define SQL_RC_E2181           -2181   /* internal error occurred during      */
                                       /* filter recovery                     */
#define SQL_RC_E2315           -2315   /* A statistics profile does not       */
                                       /* exist                               */

#define SQL_RC_E3191           -3191   /* Invalid user format in filetmod     */
#define SQL_RC_E3192           -3192   /* Invalid user format in filetmod     */

#define SQL_RC_E3201           -3201   /* Cannot import replace parent        */
#define SQL_RC_E3260           -3260   /* Unexpected LDAP error               */
#define SQL_RC_E3261           -3261   /* Missing required parameters         */
#define SQL_RC_E3262           -3262   /* The TCP/IP service name is not      */
                                       /* valid                               */
#define SQL_RC_E3263           -3263   /* The protocol type is not supported  */
#define SQL_RC_E3264           -3264   /* The DB2 server has not been         */
                                       /* registered in LDAP                  */
#define SQL_RC_E3265           -3265   /* LDAP authentication error           */
#define SQL_RC_E3267           -3267   /* Insufficient LDAP authority         */
#define SQL_RC_E3268           -3268   /* LDAP schema is not compatible       */
#define SQL_RC_E3269           -3269   /* The LDAP server is not available    */
#define SQL_RC_E3270           -3270   /* LDAP user's DN is invalid           */
#define SQL_RC_E3271           -3271   /* LDAP user's DN or password is not   */
                                       /* configured                          */
#define SQL_RC_E3272           -3272   /* The LDAP node was not found         */
#define SQL_RC_E3273           -3273   /* The LDAP database was not found     */
#define SQL_RC_E3276           -3276   /* Unable to obtain the LDAP naming    */
                                       /* context                             */
#define SQL_RC_E3277           -3277   /* The database already exists         */
#define SQL_RC_E3278           -3278   /* The node already exists             */
#define SQL_RC_E3279           -3279   /* LDAP is disabled                    */
#define SQL_RC_E3280           -3280   /* Attempt to connect to a DRDA        */
                                       /* server failed.                      */
#define SQL_RC_E3281           -3281   /* The operating system type is        */
                                       /* invalid                             */
#define SQL_RC_E3282           -3282   /* The supplied credentials are not    */
                                       /* valid.                              */
#define SQL_RC_E3284           -3284   /* The node type is not supported      */

#define SQL_RC_E3600           -3600   /* Table not in check pend state       */
#define SQL_RC_W3601           3601    /* Auto Check pending state            */
#define SQL_RC_W3602           3602    /* Constraint violation moved          */
#define SQL_RC_E3603           -3603   /* Constraint Violations               */
#define SQL_RC_E3604           -3604   /* Invalid Exception Table             */
#define SQL_RC_E3605           -3605   /* Exception Table same as Check       */
                                       /* Table                               */
#define SQL_RC_E3606           -3606   /* # check & exception tables not      */
                                       /* match                               */
#define SQL_RC_E3608           -3608   /* Parent in Check Pending state       */

#define SQL_RC_E4011           -4011   /* Invalid SQL sub-statement in        */
                                       /* Compound SQL                        */
#define SQL_RC_E4020           -4020   /* A 'long' host variable is not       */
                                       /* valid - use 'sqlint32' instead.     */

#define SQL_RC_E4300           -4300   /* Java support not installed          */
#define SQL_RC_E4301           -4301   /* Java startup, comm, shutdown        */
                                       /* failed                              */
#define SQL_RC_E4302           -4302   /* Java unclassified exception         */
#define SQL_RC_E4303           -4303   /* Java could not parse class!method   */
#define SQL_RC_E4304           -4304   /* Java could not instantiate class    */
#define SQL_RC_E4305           -4305   /* Java internal error code            */
#define SQL_RC_E4306           -4306   /* Java cannot call method             */
#define SQL_RC_E4307           -4307   /* Java method call problems           */

#define SQL_RC_E5012           -5012   /* Host variable is not exact numeric  */
                                       /* type                                */
#define SQL_RC_E5051           -5051   /* Invalid qualifier specified in      */
                                       /* CREATE SCHEMA                       */

#define SQL_RC_E5182           -5182   /* Required environment variable no    */
                                       /* set                                 */
#define SQL_RC_E5185           -5185   /* Pass-through not supported for      */
                                       /* server type                         */

#define SQL_RC_E5500           -5500   /* Unable to open vendor               */
                                       /* configuration file                  */
#define SQL_RC_E5501           -5501   /* Format of vendor configuration      */
                                       /* file is wrong                       */

#define SQL_RC_E6040           -6040   /* Insufficient FCM buffers            */
#define SQL_RC_E6041           -6041   /* Insufficient FCM connection         */
                                       /* entries                             */
#define SQL_RC_E6042           -6042   /* Insufficient FCM message anchors    */
#define SQL_RC_E6043           -6043   /* Insufficient FCM request blocks     */
#define SQL_RC_E6071           -6071   /* New node requires stop and start    */

#define SQL_RC_E7032           -7032   /* SQL Procedure not created           */

#define SQL_RC_E8000           -8000   /* db2start failed, license not found  */
#define SQL_RC_E8001           -8001   /* udb connection failed, license not  */
                                       /* found                               */
#define SQL_RC_E8002           -8002   /* 'connect' connection failed,        */
                                       /* license not found                   */
#define SQL_RC_E8008           -8008   /* evaluation period expired           */
#define SQL_RC_E8014           -8014   /* not licensed for TCP/IP             */
                                       /* connections.                        */
#define SQL_RC_E8015           -8015   /* not licensed for multiple database  */
                                       /* updates per transaction             */
#define SQL_RC_E8016           -8016   /* this user is not defined as         */
                                       /* registered user                     */
#define SQL_RC_E8022           -8022   /* not licensed for database           */
                                       /* partitioning                        */
#define SQL_RC_E8023           -8023   /* concurrent users limit exceeded     */
#define SQL_RC_E8024           -8024   /* limited function license -          */
                                       /* function not allowed                */

#define SQL_RC_E8100           -8100   /* Page number too high                */
#define SQL_RC_E8101           -8101   /* Segment in error                    */

#define SQL_RC_E9999           -9999   /* Development SQLCODE                 */
#define SQL_RC_C10003          -10003  /* not enough systems resources to     */
                                       /* process request                     */
#define SQL_RC_E20005          -20005  /* internal object limit exceeded      */
#define SQL_RC_E20010          -20010  /* mutation methd not allowed          */
#define SQL_RC_E20011          -20011  /* transform for data type already     */
                                       /* exists                              */
#define SQL_RC_E20012          -20012  /* no transforms were dropped          */
#define SQL_RC_E20013          -20013  /* Invalid object for supertype        */
#define SQL_RC_E20014          -20014  /* function cannot be used as          */
                                       /* transform function                  */
#define SQL_RC_E20015          -20015  /* transform for type not defined      */
#define SQL_RC_E20016          -20016  /* column length value too small       */
#define SQL_RC_E20017          -20017  /* Hierarchy too deep                  */
#define SQL_RC_E20018          -20018  /* row function must return at most    */
                                       /* one row                             */
#define SQL_RC_E20019          -20019  /* result type cannot be assigned to   */
                                       /* RETURN type                         */
#define SQL_RC_E20020          -20020  /* Invalid operation for typed table   */
#define SQL_RC_E20021          -20021  /* Cannot change inherited column      */
#define SQL_RC_E20022          -20022  /* SCOPE already defined               */
#define SQL_RC_E20023          -20023  /* SCOPE not allowed for parameter     */
#define SQL_RC_E20024          -20024  /* SCOPE not valid for reference       */
#define SQL_RC_E20025          -20025  /* Incorrect SCOPE for RETURNS         */
#define SQL_RC_E20026          -20026  /* Type is not a structured type       */
#define SQL_RC_E20027          -20027  /* Subtable already exists of type     */
#define SQL_RC_E20028          -20028  /* Subtable schema incorrect           */
#define SQL_RC_E20029          -20029  /* Invalid operation for subtable      */
#define SQL_RC_E20030          -20030  /* Attrs cannot be altered when in     */
                                       /* use                                 */
#define SQL_RC_E20031          -20031  /* Invalid object for subtable         */
#define SQL_RC_E20032          -20032  /* Invalid index columns for subtable  */
#define SQL_RC_E20033          -20033  /* Unscoped reference                  */
#define SQL_RC_E20034          -20034  /* Invalid TYPE predicate              */
#define SQL_RC_E20035          -20035  /* Invalid path expression             */
#define SQL_RC_E20036          -20036  /* Path includes OID column            */
#define SQL_RC_E20037          -20037  /* REF IS column must be defined       */
#define SQL_RC_E20038          -20038  /* ASC or DESC cannot be specified     */
#define SQL_RC_E20039          -20039  /* definition of index does not match  */
                                       /* extension                           */
#define SQL_RC_E20040          -20040  /* number or type of result            */
                                       /* inconsistent                        */
#define SQL_RC_E20041          -20041  /* number or type of parameters does   */
                                       /* not match                           */
#define SQL_RC_E20042          -20042  /* maximum allowable parameters        */
                                       /* exceeded                            */
#define SQL_RC_E20043          -20043  /* argument for function is invalid    */
#define SQL_RC_E20044          -20044  /* function not supported in CREATE    */
                                       /* INDEX EXTENSION                     */
#define SQL_RC_E20045          -20045  /* datatype of instance parameter      */
                                       /* invalid                             */
#define SQL_RC_E20046          -20046  /* SELECTIVITY clause needs user       */
                                       /* defined predicate                   */
#define SQL_RC_E20047          -20047  /* search method not found             */
#define SQL_RC_E20048          -20048  /* search method argument mismatch     */
#define SQL_RC_E20049          -20049  /* operand type mismatch               */
#define SQL_RC_E20050          -20050  /* search target or search argument    */
                                       /* mismatch                            */
#define SQL_RC_E20051          -20051  /* argument cannot be both search      */
                                       /* target and search argument          */
#define SQL_RC_E20052          -20052  /* Cannot update OID column            */
#define SQL_RC_E20053          -20053  /* Fullselect in typed view is not     */
                                       /* valid                               */
#define SQL_RC_E20054          -20054  /* Invalid state for operation         */
#define SQL_RC_E20055          -20055  /* Result column data type not         */
                                       /* compatible                          */
#define SQL_RC_E20056          -20056  /* Processing error on file server     */
#define SQL_RC_E20057          -20057  /* column cannot be defined as read    */
                                       /* only                                */
#define SQL_RC_E20058          -20058  /* fullselect for summary table is     */
                                       /* invalid                             */
#define SQL_RC_E20060          -20060  /* key transform function generated    */
                                       /* duplicate rows                      */
#define SQL_RC_E20062          -20062  /* transform function not valid for    */
                                       /* function or method                  */
#define SQL_RC_E20063          -20063  /* TRANSFORM GROUP clause is required  */
#define SQL_RC_E20064          -20064  /* specified transform group clause    */
                                       /* is not used                         */
#define SQL_RC_E20065          -20065  /* transform group cannot be used      */
                                       /* with client application             */
#define SQL_RC_E20066          -20066  /* transform function not defined in   */
                                       /* transform group                     */
#define SQL_RC_E20067          -20067  /* transform function defined more     */
                                       /* than once in transform group        */
#define SQL_RC_E20068          -20068  /* structured type cannot depend on    */
                                       /* itself                              */
#define SQL_RC_E20069          -20069  /* returns type not same as subject    */
                                       /* type                                */
#define SQL_RC_E20075          -20075  /* index not created, column length    */
                                       /* too long                            */
#define SQL_RC_E20076          -20076  /* instance not enabled for operation  */
#define SQL_RC_E20077          -20077  /* Cannot construct object with        */
                                       /* Datalink or Reference type          */
                                       /* attributes                          */
#define SQL_RC_E20078          -20078  /* operation cannot be applied to      */
                                       /* object                              */
#define SQL_RC_E20080          -20080  /* method specification cannot be      */
                                       /* dropped                             */
#define SQL_RC_E20081          -20081  /* method body must correspond to      */
                                       /* specfication language               */
#define SQL_RC_E20082          -20082  /* dynamic type is not a subtype of    */
                                       /* target                              */
#define SQL_RC_E20083          -20083  /* returned data type does not match   */
                                       /* RESULT                              */
#define SQL_RC_E20084          -20084  /* routine would override an existing  */
                                       /* method                              */
#define SQL_RC_E20085          -20085  /* Java routine cannot have            */
                                       /* structured type parameter or        */
                                       /* returns type                        */
#define SQL_RC_E20086          -20086  /* Length of structured type exceeds   */
                                       /* limit                               */
#define SQL_RC_E20087          -20087  /* DEFAULT or NULL cannot be used in   */
                                       /* an attribute assignment             */
#define SQL_RC_E20089          -20089  /* method name and structured type     */
                                       /* name match                          */
#define SQL_RC_E20093          -20093  /* conversion error between summary    */
                                       /* table and regular table             */
#define SQL_RC_E20094          -20094  /* GENERATED column cannot be          */
                                       /* referenced in BEFORE trigger        */

#define SQL_RC_E20102          -20102  /* CREATE or ALTER for the routine is  */
                                       /* not allowed                         */
#define SQL_RC_E20108          -20108  /* result set contains unsupported     */
                                       /* data type                           */
#define SQL_RC_E20111          -20111  /* SAVEPOINT statement is not allowed  */
                                       /* in this context                     */
#define SQL_RC_E20112          -20112  /* nested savepoint is not allowed     */
#define SQL_RC_E20113          -20113  /* cannot return null from SELF AS     */
                                       /* RESULT method                       */
#define SQL_RC_E20115          -20115  /* routine cannot be used as a         */
                                       /* transform function                  */
#define SQL_RC_E20116          -20116  /* search target and source key data   */
                                       /* types do not match                  */
#define SQL_RC_E20117          -20117  /* window specification for an OLAP    */
                                       /* function invalid                    */
#define SQL_RC_E20118          -20118  /* maximum number of attributes        */
                                       /* exceeded                            */
#define SQL_RC_E20119          -20119  /* row function must return at least   */
                                       /* two columns                         */
#define SQL_RC_E20120          -20120  /* SQL table function must return a    */
                                       /* table                               */
#define SQL_RC_E20121          -20121  /* WITH RETURN and SCROLL specified    */
                                       /* for a single cursor                 */
#define SQL_RC_E20123          -20123  /* stored procedure cursor error       */
#define SQL_RC_E20128          -20128  /* scrollable cursor cannot include    */
                                       /* table function output               */
#define SQL_RC_E20131          -20131  /* Obj number specified more than      */
                                       /* once in list                        */
#define SQL_RC_E20133          -20133  /* operation must be performed on SQL  */
                                       /* routine                             */
#define SQL_RC_E20134          -20134  /* SQL archive could not be created    */
                                       /* on server                           */
#define SQL_RC_E20135          -20135  /* SQL archive does not match target   */
                                       /* environment                         */
#define SQL_RC_E20136          -20136  /* Routine is NOT FEDERATED            */
#define SQL_RC_E20138          -20138  /* Routine is not defined as MODIFIES  */
                                       /* SQL DATA                            */
#define SQL_RC_E20139          -20139  /* Previous statement failed or was    */
                                       /* interrupted                         */
#define SQL_RC_E20142          -20142  /* Sequence cannot be used as          */
                                       /* specified                           */
#define SQL_RC_E20143          -20143  /* ENCRYPTION PASSWORD special         */
                                       /* register not set                    */
#define SQL_RC_E20144          -20144  /* invalid length for encryption       */
                                       /* password                            */
#define SQL_RC_E20145          -20145  /* decryption key does not match       */
                                       /* encryption key                      */
#define SQL_RC_E20146          -20146  /* DECRYPT failed because data is not  */
                                       /* encrypted                           */
#define SQL_RC_E20147          -20147  /* ENCRYPT function failed             */
#define SQL_RC_E20148          -20148  /* routine must end with RETURN        */
                                       /* statement                           */
#define SQL_RC_E20150          -20150  /* block pages too large for the       */
                                       /* buffer pool size                    */
#define SQL_RC_E20151          -20151  /* BLOCKSIZE value is not in the       */
                                       /* valid range                         */
#define SQL_RC_E20152          -20152  /* specified buffer pool is not block  */
                                       /* based                               */
#define SQL_RC_E20153          -20153  /* database split image is suspended   */
#define SQL_RC_E20154          -20154  /* Insert into view not allowed        */
                                       /* target table cannot be determined   */
#define SQL_RC_E20155          -20155  /* event monitor target table invalid  */
#define SQL_RC_E20157          -20157  /* User does not have QUIESCE_CONNECT  */
                                       /* privilege                           */
#define SQL_RC_E20158          -20158  /* Function not supported for level    */
                                       /* of DLM                              */
#define SQL_RC_E20162          -20162  /* cannot use block-based and          */
                                       /* extended storage                    */
#define SQL_RC_E20165          -20165  /* SQL data change stmt not allowed    */
#define SQL_RC_E20166          -20166  /* Speicifed view is not a symmetric   */
                                       /* view                                */
#define SQL_RC_E20167          -20167  /* Virtual storage or database         */
                                       /* resource is not available.          */
#define SQL_RC_E20168          -20168  /* The ALTER BUFFERPOOL statement is   */
                                       /* currently in progress.              */
#define SQL_RC_E20170          -20170  /* There is not enough space in the    */
                                       /* table space for the specified       */
                                       /* action.                             */
#define SQL_RC_E20178          -20178  /* view already has INSTEAD OF         */
                                       /* trigger defined                     */
#define SQL_RC_E20179          -20179  /* trigger not created because view    */
                                       /* defined WITH CHECK OPTION           */

#define SQL_RC_E20188          -20188  /* primary or unique key is a subset   */
                                       /* of columns in dimensions clause     */
#define SQL_RC_E20190          -20190  /* Federated operation not compiled    */
                                       /* due to potential inconsistency      */
#define SQL_RC_E20191          -20191  /* the same host variable must be      */
                                       /* used in both USING and INTO         */
                                       /* clauses                             */
#define SQL_RC_E20192          -20192  /* the requested feature is not        */
                                       /* supported in this environment       */
#define SQL_RC_E20193          -20193  /* error has occurred when accessing   */
                                       /* a file with reason-code             */
#define SQL_RC_E20194          -20194  /* buffer pool does not exist on       */
                                       /* database partition                  */
#define SQL_RC_E20195          -20195  /* error encountered while processing  */
                                       /* the path rename config file         */
#define SQL_RC_E20196          -20196  /* one or more built-in types do not   */
                                       /* match corresponding built-in types  */
#define SQL_RC_E20197          -20197  /* cannot define method as an          */
                                       /* overriding method                   */
#define SQL_RC_E20198          -20198  /* method calls itself recursively     */
#define SQL_RC_E20200          -20200  /* URL not found                       */
#define SQL_RC_E20201          -20201  /* jar name invalid                    */
#define SQL_RC_E20202          -20202  /* class is in use                     */
#define SQL_RC_E20203          -20203  /* Java method has invalid signature   */
#define SQL_RC_E20204          -20204  /* function unable to map to single    */
                                       /* method                              */
#define SQL_RC_E20205          -20205  /* null value not allowed in argument  */
#define SQL_RC_E20207          -20207  /* unsupported deployment descriptor   */
#define SQL_RC_E20208          -20208  /* Table used to define a staging      */
                                       /* table is not valid                  */
#define SQL_RC_E20209          -20209  /* Option not valid for table with     */
                                       /* reason-code                         */

#define SQL_RC_E20210          -20210  /* table-designator does not contain   */
                                       /* ORDER BY                            */
#define SQL_RC_E20211          -20211  /* ORDER BY and FETCH FIRST n ROWS     */
                                       /* ONLY is invalid                     */
#define SQL_RC_E20212          -20212  /* User defined routine encountered    */
                                       /* an exception while loading java     */
                                       /* class                               */
#define SQL_RC_E20214          -20214  /* ORDER OF specified but table        */
                                       /* designator not ordered              */
#define SQL_RC_E20223          -20223  /* Encryption facility not available   */
#define SQL_RC_E20230          -20230  /* Procedure name may not be           */
                                       /* specified by a host variable in     */
                                       /* the CALL statement                  */
#define SQL_RC_E20238          -20238  /* Table defined as CCSID UNICODE      */
#define SQL_RC_E20239          -20239  /* Table cannot be typed, or contain   */
                                       /* graphic or user-defined types       */
#define SQL_RC_E20241          -20241  /* Unable to write a history file      */
                                       /* entry                               */
#define SQL_RC_E20242          -20242  /* Sample size in clause is invalid    */
#define SQL_RC_E20243          -20243  /* The view is missing the INSTEAD OF  */
                                       /* triggers                            */
#define SQL_RC_E20253          -20253  /* BEFORE TRIGGER or GENERATED COLUMN  */
                                       /* would cause table to be delete      */
                                       /* connected with ovelapping set null  */
                                       /* rules                               */
#define SQL_RC_E20254          -20254  /* FOREIGN KEY would cause table to    */
                                       /* be delete-connected to itself       */
#define SQL_RC_E20255          -20255  /* FOREIGN KEY would cause a descent   */
                                       /* table to be delete-connected to     */
                                       /* ancestor table                      */
#define SQL_RC_E20256          -20256  /* FOREIGN KEY would cause two tables  */
                                       /* to be delete-connected              */
#define SQL_RC_E20257          -20257  /* FINAL TABLE is not valid            */
#define SQL_RC_E20258          -20258  /* Invalid use of INPUT SEQUENCE       */
                                       /* ordering                            */
#define SQL_RC_E20259          -20259  /* column cannot be specified in the   */
                                       /* select list of query                */
#define SQL_RC_E20260          -20260  /* assignment clause of UPDATE stmt    */
                                       /* must specify at least one column    */
                                       /* that is not an INCLUDE column       */
#define SQL_RC_E20261          -20261  /* Invalid row movement to table       */
#define SQL_RC_E20262          -20262  /* Invalid usage of WITH ROW MOVEMENT  */
                                       /* in a view                           */
#define SQL_RC_E20263          -20263  /* Invalid attempt to update view      */
#define SQL_RC_E20267          -20267  /* A function is invoked in an         */
                                       /* illegal context                     */
#define SQL_RC_E20269          -20269  /* A nickname cannot be referenced in  */
                                       /* an enforced referential constraint  */
#define SQL_RC_E20273          -20273  /* Nickname stats cannot be updated    */
#define SQL_RC_E20275          -20275  /* XML name is not valid               */
#define SQL_RC_E20276          -20276  /* XML namespace is not valid          */
#define SQL_RC_E20279          -20279  /* View cannot be enabled for query    */
                                       /* optimization                        */
#define SQL_RC_E20282          -20282  /* Unable to load .NET class           */
#define SQL_RC_E20284          -20284  /* Unable to create plan               */
#define SQL_RC_E20288          -20288  /* Stats could not be updated          */
#define SQL_RC_E20290          -20290  /* Routine cannot be run on the        */
                                       /* specified partition                 */
#define SQL_RC_E20316          -20316  /* Invalid compilation environment     */
#define SQL_RC_E20317          -20317  /* Automatic storage not defined for   */
                                       /* database                            */
#define SQL_RC_E20318          -20318  /* Cannot perform container            */
                                       /* operations against table space      */
#define SQL_RC_E20319          -20319  /* Redirected restore of automatic     */
                                       /* storage table space not allowed     */
#define SQL_RC_E20320          -20320  /* Maximum size for table space not    */
                                       /* valid                               */
#define SQL_RC_E20321          -20321  /* Storage paths cannot be specified   */
#define SQL_RC_E20322          -20322  /* Database name does not match        */
                                       /* current server                      */
#define SQL_RC_E20323          -20323  /* Storage path is a duplicate         */
#define SQL_RC_E20324          -20324  /* Operation cannot occur multiple     */
                                       /* times in transaction                */
#define SQL_RC_E20325          -20325  /* Maximum size of table space         */
                                       /* exceeded                            */
#define SQL_RC_E20333          -20333  /* Operation violated an integrity     */
                                       /* constraint                          */
#define SQL_RC_E21000          -21000  /* Text Information Extender           */
                                       /* incorrectly configured              */

#define SQL_RC_E22400          -22400  /* The funtion or feature name is      */
                                       /* invalid                             */
#define SQL_RC_E22401          -22401  /* Application ID does not exist       */
#define SQL_RC_E22402          -22402  /* No activity monitor reports exist   */
#define SQL_RC_E22403          -22403  /* Invalid values specified            */
#define SQL_RC_E22404          -22404  /* Specified action mode is invalid    */
#define SQL_RC_E22405          -22405  /* Unable to collect snapshot data     */

#define SQL_RC_E30000          -30000  /* Distribution protocol error, no     */
                                       /* disconnect                          */
#define SQL_RC_E30002          -30002  /* statement cannot be executed due    */
                                       /* to prior condition                  */
#define SQL_RC_E30005          -30005  /* Execution failed; function not      */
                                       /* supported                           */
#define SQL_RC_E30020          -30020  /* Distribution protocol error,        */
                                       /* disconnect                          */
#define SQL_RC_E30021          -30021  /* Distribution compatibility error    */
#define SQL_RC_E30040          -30040  /* Distribution memory allocation err  */
#define SQL_RC_E30041          -30041  /* Distribution memory allocation      */
                                       /* err, disconnect                     */
#define SQL_RC_E30050          -30050  /* Invalid command while bind in prog  */
#define SQL_RC_E30051          -30051  /* Bind not active                     */
#define SQL_RC_E30053          -30053  /* Bind Owner authorization failure    */
#define SQL_RC_E30060          -30060  /* RDB authorization failure           */
#define SQL_RC_E30061          -30061  /* RDB not found                       */
#define SQL_RC_E30070          -30070  /* Distribution command error          */
#define SQL_RC_E30071          -30071  /* Distribution object error           */
#define SQL_RC_E30072          -30072  /* Distribution parameter error        */
#define SQL_RC_E30073          -30073  /* Distribution parameter value error  */
#define SQL_RC_E30074          -30074  /* Distribution reply error            */
#define SQL_RC_E30080          -30080  /* Communication error                 */
#define SQL_RC_E30081          -30081  /* Communication error                 */
#define SQL_RC_E30082          -30082  /* Security error                      */
#define SQL_RC_E30083          -30083  /* Attempt to change password failed   */
#define SQL_RC_E30090          -30090  /* Remote operation invalid            */

#define SQL_RC_E30104          -30104  /* A bind option is invalid            */
#define SQL_RC_E30106          -30106  /* Invalid input for mulitple row      */
                                       /* INSERT                              */
#define SQL_RC_E30108          -30108  /* A failed connection has been re     */
                                       /* established                         */
/* Database Monitor SQLCODES                                                  */

#define SQLM_RC_BAD_PATH       -1612   /* bad path specified for event        */
                                       /* monitor                             */
#define SQLM_RC_BAD_OPTION     -1613   /* bad OPTION specified for event      */
                                       /* monitor                             */
#define SQLM_RC_IO_ERROR       -1614   /* I/O error on activating event       */
                                       /* monitor                             */
#define SQLM_RC_NOT_ACTIVATED  -1616   /* Event monitor not activated         */
#define SQLM_RC_EVMON_FULL     -1617   /* Event monitor data files are full   */
#define SQLM_RC_PATH_IN_USE    -1618   /* Event monitor path is in use        */
#define SQLM_RC_CANNOT_DROP    -1619   /* Cannot drop active event monitor    */
#define SQLM_RC_CANNOT_FLUSH   -1620   /* Cannot flush event monitor          */
#define SQLM_RC_MUST_COMMIT    -1621   /* Must commit to use event monitor    */
#define SQLM_RC_BAD_STATE      -1622   /* Invalid state                       */

#ifdef __cplusplus 
}
#endif

#endif /* SQL_H_SQLCODES */
