      *************************************************************************
      * 
      * Source File Name: SQL
      * 
      * (C) COPYRIGHT International Business Machines Corp. 1987, 2003
      * All Rights Reserved
      * Licensed Materials - Property of IBM
      * 
      * US Government Users Restricted Rights - Use, duplication or
      * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
      * 
      * Function = Include File defining:
      *              System Constants
      *              National Language Support Information
      *              SQLCA / SQLDA Constants
      *              Interface to BINDER and PRECOMPILER
      *              Error Message Retrieval Interface
      *              Authorization Constants
      * 
      * Operating System:  Windows NT
      * 
      **************************************************************************
      * Release Identifier Constants

      * Pre Version 6.1.0.0
       78  SQL-RELPRE6               VALUE    0.
      * V6.1.0.0
       78  SQL-REL6100               VALUE    6010000.
      * V7.1.0.0
       78  SQL-REL7100               VALUE    7010000.
      * V7.1.2.0
       78  SQL-REL7102               VALUE    7010200.
      * V7.2.0.0
       78  SQL-REL7200               VALUE    7020100.
      * V7.2.1.0
       78  SQL-REL7201               VALUE    7020200.
      * V7.2.4.0
       78  SQL-REL7204               VALUE    7020400.
      * V8.1.0.0
       78  SQL-REL8100               VALUE    8010000.
      * V8.1.1.0
       78  SQL-REL8101               VALUE    8010100.
      * V8.1.2.0
       78  SQL-REL8102               VALUE    8010200.
      * V8.1.3.0
       78  SQL-REL8103               VALUE    8010300.
      * V8.1.4.0
       78  SQL-REL8104               VALUE    8010400.
      * V8.1.5.0
       78  SQL-REL8105               VALUE    8010500.
      * V8.1.6.0
       78  SQL-REL8106               VALUE    8010600.
      * V8.2.0.0
       78  SQL-REL8200               VALUE    8020000.
      * V8.2.1.0
       78  SQL-REL8201               VALUE    8020100.
      * V8.2.2.0
       78  SQL-REL8202               VALUE    8020200.
      * V8.2.3.0
       78  SQL-REL8203               VALUE    8020300.
      * V8.2.4.0
       78  SQL-REL8204               VALUE    8020400.
      * Future Release
       78  SQL-FUTUREL               VALUE    9999999.

      * System Constants

      * successful execution
       78  SQL-RC-OK                 VALUE    0.

      * Maximum nbr of key parts in Index
       78  SQL-KEYPMAX               VALUE    16.
      * Maximum key length
       78  SQL-KEYLMAX               VALUE    1024.
      * Maximum size of field for sort
       78  SQL-SORTFLDLMT            VALUE    32677.
      * Maximum record length on a 4K page
       78  SQL-MAXRECL-4K            VALUE    4005.
      * Maximum record length on a 8K page
       78  SQL-MAXRECL-8K            VALUE    8101.
      * Maximum record length
       78  SQL-MAXRECL               VALUE    32677.
      * Maximum nbr of tables in a SELECT
       78  SQL-MAXTABLES             VALUE    15.
      * Maximum nbr of Host Vars per stmt
       78  SQL-MAXVARS-STMT          VALUE    32767.
      * Internal max nbr of columns in a table
       78  SQL-MAXCOLS               VALUE    3000.
      * External max nbr of columns in a table
       78  SQL-MAXCOLS-EXT           VALUE    1012.
      * Maximum nbr of items in a SELECT
       78  SQL-MAXSEL-ITEMS          VALUE    1012.
      * Maximum nbr of parms in a function
       78  SQL-MAXPARMS              VALUE    90.
      * Maximum statement size
       78  SQL-MAX-STMT-SIZ          VALUE    2097152.

      * Size of a SMALLINT
       78  SQL-SMALL-LENGTH          VALUE    2.
      * Maximum value of a SMALLINT
       78  SQL-MAXSMALLVAL           VALUE    32767.
      * Minimum value of a SMALLINT
       78  SQL-MINSMALLVAL           VALUE    -32768.
      * Size of an INTEGER
       78  SQL-INT-LENGTH            VALUE    4.
      * Maximum value of an INTEGER
       78  SQL-MAXINTVAL             VALUE    2147483647.
      * Minimum value of an INTEGER
       78  SQL-MININTVAL             VALUE    -2147483648.
      * Size of a BIGINT
       78  SQL-BIGINT-LENGTH         VALUE    8.
      * Size of a FLOAT
       78  SQL-FLOAT-LENGTH          VALUE    8.
      * Size of a 4-byte FLOAT
       78  SQL-FLOAT4-LENGTH         VALUE    4.
      * Maximum prec for small float
       78  SQL-MAXSFLTPREC           VALUE    24.
      * Minimum prec for small float
       78  SQL-MINSFLTPREC           VALUE    1.
      * Minimum prec for any float
       78  SQL-MAXFLOATPREC          VALUE    53.
      * Default precision for DECIMAL
       78  SQL-DEFDEC-PRECISION      VALUE    5.
      * Default scale for DECIMAL
       78  SQL-DEFDEC-SCALE          VALUE    0.
      * Maximum scale/prec. for DECIMAL
       78  SQL-MAXDECIMAL            VALUE    31.
      * Default length for a CHAR
       78  SQL-DEFCHAR               VALUE    1.
      * Default length for a graphic
       78  SQL-DEFWCHAR              VALUE    1.
      * Maximum length of a CHAR
       78  SQL-MAXCHAR               VALUE    254.
      * Maximum length of an LSTRING
       78  SQL-MAXLSTR               VALUE    255.
      * Maximum length of a VARCHAR
       78  SQL-MAXVCHAR              value    32672.
      * Maximum length of a VARGRAPHIC
       78  SQL-MAXVGRAPH             value    16336.
      * Max. length of a BLOB host var
       78  SQL-MAXBLOB               VALUE    2147483647.
      * Max. length of a CLOB host var
       78  SQL-MAXCLOB               VALUE    2147483647.
      * Max. length of an DBCLOB host var
       78  SQL-MAXDBCLOB             VALUE    1073741823.
      * Length of a LOB locator host var
       78  SQL-LOBLOCATOR-LEN        VALUE    4.
      * Length of a LOB file host var
       78  SQL-LOBFILE-LEN           VALUE    267.
      * Overhead for VARCHAR in record
       78  SQL-VCHAROH               VALUE    4.
      * Overhead for variable length type in record
       78  SQL-VARCOL-OH             VALUE    4.
      * Overhead for variable keyparts
       78  SQL-VARKEY-OH             VALUE    2.
      * Maximum length of a LONG VARCHAR
       78  SQL-LONGMAX               VALUE    32700.
      * Max. length of a LONG VARGRAPHIC
       78  SQL-LONGGRMAX             VALUE    16350.
      * Overhead for LONG VARCHAR in record
       78  SQL-LVCHAROH              VALUE    24.
      * Overhead for LOB in record
       78  SQL-LOBCHAROH             VALUE    312.
      * BLOB maximum length, in bytes
       78  SQL-BLOB-MAXLEN           VALUE    2147483647.
      * CLOB maximum length, in chars
       78  SQL-CLOB-MAXLEN           VALUE    2147483647.
      * maxlen for dbcs lobs
       78  SQL-DBCLOB-MAXLEN         VALUE    1073741823.
      * Size of a TIME field
       78  SQL-TIME-LENGTH           VALUE    3.
      * Size of a TIME field output
       78  SQL-TIME-STRLEN           VALUE    8.
      * Size of a non-USA TIME field output without seconds
       78  SQL-TIME-MINSTRLEN        VALUE    5.
      * Size of a DATE field
       78  SQL-DATE-LENGTH           VALUE    4.
      * Size of a DATE field output
       78  SQL-DATE-STRLEN           VALUE    10.
      * Size of a TIMESTAMP field
       78  SQL-STAMP-LENGTH          VALUE    10.
      * Size of a TIMESTAMP field output
       78  SQL-STAMP-STRLEN          VALUE    26.
      * Size of a TIMESTAMP field output without microseconds
       78  SQL-STAMP-MINSTRLEN       VALUE    19.
      * Size of a BOOLEAN field
       78  SQL-BOOLEAN-LENGTH        VALUE    1.
      * Size of a DATALINK field
       78  SQL-DATALINK-LENGTH       VALUE    254.
      * Size of an indicator value
       78  SQL-IND-LENGTH            VALUE    2.

      * Max size of Stored Proc Name
       78  SQL-MAX-PNAME-LENGTH      VALUE    254.
      * Maximum length of Identifer
       78  SQL-MAX-IDENT             VALUE    128.
      * Maximum length of Long Identifer
       78  SQL-LG-IDENT              VALUE    18.
      * Maximum length of Short Identifer
       78  SQL-SH-IDENT              VALUE    8.
      * Minimum length of Identifiers
       78  SQL-MN-IDENT              VALUE    1.
      * Max size of Host Variable Name
       78  SQL-MAX-VAR-NAME          VALUE    255.
      * Number of partitions in a pmap
       78  SQL-PDB-MAP-SIZE          VALUE    4096.
      * Max # of Partition Keys
       78  SQL-MAX-NUM-PART-KEYS     VALUE    500.
      * decimal columns for sqlugrpn are in zoneddecimal format
       78  SQL-ZONEDDECIMAL-FORMAT   VALUE    32.
      * decimal columns for sqlugrpn are in implieddecimal format
       78  SQL-IMPLIEDDECIMAL-FORMAT VALUE    16.
      * numeric columns for sqlugrpn are in binary format
       78  SQL-BINARYNUMERICS-FORMAT VALUE    4.
      * decimal columns for sqlugrpn are in packeddecimal format
       78  SQL-PACKEDDECIMAL-FORMAT  VALUE    8.
      * numeric/decimal columns for sqlugrpn are in character string format
       78  SQL-CHARSTRING-FORMAT     VALUE    0.
      * # of bytes in a kilobyte
       78  SQL-KILO-VALUE            VALUE    1024.
      * # of bytes in a megabyte
       78  SQL-MEGA-VALUE            VALUE    1048576.
      * # of bytes in a gigabyte
       78  SQL-GIGA-VALUE            VALUE    1073741824.

      * size of container name for api's (includes 1 byte for C NULL
      * terminator)
       78  SQLB-MAX-CONTAIN-NAME-SZ  VALUE    256.
      * System types 
      * limit for max. # of nodes
       78  SQL-PDB-MAX-NUM-NODE      VALUE    1000.
      * information related to logical node name
      * no. of digits for node number in node name
       78  SQL-PDB-NODE-NUM-DIGIT    VALUE    4.
      * length of logical node name
       78  SQL-PDB-NODE-NAME-LEN     VALUE    8.
      * Codepages
      * Codepage 367 - EUC single byte
       78  SQL-CP-367                VALUE    367.
      * CCSID x01A4, (CP420, ST4)
       78  SQL-CP-420                VALUE    420.
      * CCSID x01A8, (CP424, ST4)
       78  SQL-CP-424                VALUE    424.
      * CCSID x01A9, (CP420, ST5)
       78  SQL-CP-425                VALUE    425.
      * Codepage 437 - US, Europe
       78  SQL-CP-437                VALUE    437.
      * Codepage 737 - WIN Greece
       78  SQL-CP-737                VALUE    737.
      * Codepage 806 - ISCII, India
       78  SQL-CP-806                VALUE    806.
      * Codepage 813 - AIX Greece
       78  SQL-CP-813                VALUE    813.
      * Codepage 819 - ISO 8859-1
       78  SQL-CP-819                VALUE    819.
      * Codepage 850 - International PC
       78  SQL-CP-850                VALUE    850.
      * Codepage 855 - OS2 Cyrillic
       78  SQL-CP-855                VALUE    855.
      * Codepage 852 - OS2 Latin2
       78  SQL-CP-852                VALUE    852.
      * Codepage 856 - Hebrew
       78  SQL-CP-856                VALUE    856.
      * Codepage 857 - OS2 Turkey
       78  SQL-CP-857                VALUE    857.
      * Codepage 860 - Portuguese
       78  SQL-CP-860                VALUE    860.
      * Codepage 862 - OS2 Hebrew
       78  SQL-CP-862                VALUE    862.
      * Codepage 863 - Canadian-French
       78  SQL-CP-863                VALUE    863.
      * Codepage 864 - OS2 Arabic
       78  SQL-CP-864                VALUE    864.
      * Codepage 865 - Norway, Denmark
       78  SQL-CP-865                VALUE    865.
      * Codepage 866 - Russia
       78  SQL-CP-866                VALUE    866.
      * Codepage 867 - OS2 Hebrew
       78  SQL-CP-867                VALUE    867.
      * Codepage 869 - OS2 Greece
       78  SQL-CP-869                VALUE    869.
      * Codepage 874 - OS2/AIX Thailand
       78  SQL-CP-874                VALUE    874.
      * Codepage 878 - KOI-8R Russia
       78  SQL-CP-878                VALUE    878.
      * Codepage 891 - Korean
       78  SQL-CP-891                VALUE    891.
      * Codepage 897 - Japanese
       78  SQL-CP-897                VALUE    897.
      * Codepage 903 - Chinese
       78  SQL-CP-903                VALUE    903.
      * Codepage 904 - Taiwan
       78  SQL-CP-904                VALUE    904.
      * Codepage 912 - AIX Latin2
       78  SQL-CP-912                VALUE    912.
      * Codepage 915 - AIX Cyrillic
       78  SQL-CP-915                VALUE    915.
      * Codepage 916 - AIX Hebrew
       78  SQL-CP-916                VALUE    916.
      * Codepage 920 - AIX Turkey
       78  SQL-CP-920                VALUE    920.
      * Codepage 921 - Latvia, Lithuania
       78  SQL-CP-921                VALUE    921.
      * Codepage 922 - Estonia
       78  SQL-CP-922                VALUE    922.
      * Codepage 923 - ISO 8859-15
       78  SQL-CP-923                VALUE    923.
      * Codepage 1004 - MS-WINDOWS
       78  SQL-CP-1004               VALUE    1004.
      * Codepage 1040 - Extended Korean
       78  SQL-CP-1040               VALUE    1040.
      * Codepage 1041 - Extended Japanese
       78  SQL-CP-1041               VALUE    1041.
      * Codepage 1042 - Extended Chinese
       78  SQL-CP-1042               VALUE    1042.
      * Codepage 1043 - Extended Taiwan
       78  SQL-CP-1043               VALUE    1043.
      * Codepage 1046 - AIX Arabic
       78  SQL-CP-1046               VALUE    1046.
      * Codepage 1051 - HP Roman8
       78  SQL-CP-1051               VALUE    1051.
      * Codepage 1088 - Korea Std
       78  SQL-CP-1088               VALUE    1088.
      * Codepage 1089 - AIX Arabic
       78  SQL-CP-1089               VALUE    1089.
      * Codepage 1114 - Big-5 & GBK
       78  SQL-CP-1114               VALUE    1114.
      * Codepage 1115 - China GB
       78  SQL-CP-1115               VALUE    1115.
      * Codepage 1124 - AIX Ukraine 
       78  SQL-CP-1124               VALUE    1124.
      * Codepage 1125 - OS/2 Ukraine 
       78  SQL-CP-1125               VALUE    1125.
      * Codepage 1126 - Windows Korean Std
       78  SQL-CP-1126               VALUE    1126.
      * Codepage 1129 - Vietnamese
       78  SQL-CP-1129               VALUE    1129.
      * Codepage 1131 - OS/2 Belarus 
       78  SQL-CP-1131               VALUE    1131.
      * Codepage 1163 - Vietnamese
       78  SQL-CP-1163               VALUE    1163.
      * KOI8-RU - Belarus
       78  SQL-CP-1167               VALUE    1167.
      * KOI8-U - Ukraine
       78  SQL-CP-1168               VALUE    1168.
      * Codepage 1250 - Windows Latin-2
       78  SQL-CP-1250               VALUE    1250.
      * Codepage 1251 - Windows Cyrillic
       78  SQL-CP-1251               VALUE    1251.
      * Codepage 1252 - Windows Latin-1
       78  SQL-CP-1252               VALUE    1252.
      * Codepage 1253 - Windows Greek
       78  SQL-CP-1253               VALUE    1253.
      * Codepage 1254 - Windows Turkish
       78  SQL-CP-1254               VALUE    1254.
      * Codepage 1255 - Windows Hebrew
       78  SQL-CP-1255               VALUE    1255.
      * Codepage 1256 - Windows Arabic
       78  SQL-CP-1256               VALUE    1256.
      * Codepage 1257 - Windows Baltic
       78  SQL-CP-1257               VALUE    1257.
      * Codepage 1258 - Windows Vietnamese
       78  SQL-CP-1258               VALUE    1258.
      * Codepage 1275 - Mac Latin-1
       78  SQL-CP-1275               VALUE    1275.
      * Codepage 1280 - Mac Greek
       78  SQL-CP-1280               VALUE    1280.
      * Codepage 1281 - Mac Turkish
       78  SQL-CP-1281               VALUE    1281.
      * Codepage 1282 - Mac Latin-2
       78  SQL-CP-1282               VALUE    1282.
      * Codepage 1283 - Mac Cyrillic
       78  SQL-CP-1283               VALUE    1283.
      * CCSID xF300, (CP856, ST4)
       78  SQL-CP-62208              VALUE    62208.
      * CCSID xF301, (CP862, ST4)
       78  SQL-CP-62209              VALUE    62209.
      * CCSID xF302, (CP916, ST4)
       78  SQL-CP-62210              VALUE    62210.
      * CCSID xF305, (CP862, ST5)
       78  SQL-CP-62213              VALUE    62213.
      * CCSID xF30C, (CP856, ST6)
       78  SQL-CP-62220              VALUE    62220.
      * CCSID xF30D, (CP862, ST6)
       78  SQL-CP-62221              VALUE    62221.
      * CCSID xF30E, (CP916, ST6)
       78  SQL-CP-62222              VALUE    62222.
      * CCSID xF30F, (CP1255, ST6)
       78  SQL-CP-62223              VALUE    62223.
      * CCSID xF311, (CP864, ST6)
       78  SQL-CP-62225              VALUE    62225.
      * CCSID xF312, (CP1046, ST6)
       78  SQL-CP-62226              VALUE    62226.
      * CCSID xF313, (CP1089, ST6)
       78  SQL-CP-62227              VALUE    62227.
      * CCSID xF314, (CP1256, ST6)
       78  SQL-CP-62228              VALUE    62228.
      * CCSID xF316, (CP856, ST8)
       78  SQL-CP-62230              VALUE    62230.
      * CCSID xF317, (CP862, ST8)
       78  SQL-CP-62231              VALUE    62231.
      * CCSID xF318, (CP916, ST8)
       78  SQL-CP-62232              VALUE    62232.
      * CCSID xF31C, (CP856, ST10)
       78  SQL-CP-62236              VALUE    62236.
      * CCSID xF31E, (CP916, ST10)
       78  SQL-CP-62238              VALUE    62238.
      * CCSID xF31F, (CP1255, ST10)
       78  SQL-CP-62239              VALUE    62239.
      * CCSID xF321, (CP856, ST11)
       78  SQL-CP-62241              VALUE    62241.
      * CCSID xF322, (CP862, ST11)
       78  SQL-CP-62242              VALUE    62242.
      * CCSID xF323, (CP916, ST11)
       78  SQL-CP-62243              VALUE    62243.
      * CCSID xF324, (CP1255, ST11)
       78  SQL-CP-62244              VALUE    62244.
      * CCSID xE000, (Unknown or unsupported)
       78  SQL-CP-UNKNOWN            VALUE    57344.
      * CCSID 1162 - Windows Thailand (with Euro)
       78  SQL-CP-1162               VALUE    1162.
      * CCSID 5222 - Windows Korea
       78  SQL-CP-5222               VALUE    5222.
      * CCSID 5346 - Windows Latin-2 (v2 with Euro)
       78  SQL-CP-5346               VALUE    5346.
      * CCSID 5347 - Windows Cyrillic (v2 with Euro)
       78  SQL-CP-5347               VALUE    5347.
      * CCSID 5348 - Windows Latin-1 (v2 with Euro)
       78  SQL-CP-5348               VALUE    5348.
      * CCSID 5349 - Windows Greece (v2 with Euro)
       78  SQL-CP-5349               VALUE    5349.
      * CCSID 5350 - Windows Turkey (v2 with Euro)
       78  SQL-CP-5350               VALUE    5350.
      * CCSID 5351 - Windows Hebrew ST5 (v2 with Euro)
       78  SQL-CP-5351               VALUE    5351.
      * CCSID 5352 - Windows Arabic ST5 (v2 with Euro)
       78  SQL-CP-5352               VALUE    5352.
      * CCSID 5353 - Windows Baltic (v2 with Euro)
       78  SQL-CP-5353               VALUE    5353.
      * CCSID 5354 - Windows Vietnam (v2 with Euro)
       78  SQL-CP-5354               VALUE    5354.
      * CCSID 62215 - Windows Hebrew ST4
       78  SQL-CP-62215              VALUE    62215.
      * CCSID 62237 - Windows Hebrew ST8
       78  SQL-CP-62237              VALUE    62237.
      * CCSID 895 - Japan 7-bit Latin
       78  SQL-CP-895                VALUE    895.
      * CCSID 901 - Baltic 8-bit (with Euro)
       78  SQL-CP-901                VALUE    901.
      * CCSID 902 - Estonia ISO-8 (with Euro)
       78  SQL-CP-902                VALUE    902.
      * CCSID 1008 - Arabic 8-bit ISO/ASCII
       78  SQL-CP-1008               VALUE    1008.
      * CCSID 1155 - Turkey Latin-5 (with Euro)
       78  SQL-CP-1155               VALUE    1155.
      * CCSID 4909 - Greece, Latin ISO-8 (with Euro)
       78  SQL-CP-4909               VALUE    4909.
      * CCSID 5104 - Arabic 8-bit ISO/ASCII (with Euro)
       78  SQL-CP-5104               VALUE    5104.
      * CCSID 21427 - Taiwan IBM Big-5 (with 13493 CNS, 566, 6204 UDC, Euro)
       78  SQL-CP-21427              VALUE    21427.
      * CCSID 62212 - CP867 Hebrew ST10
       78  SQL-CP-62212              VALUE    62212.
      * CCSID 62214 - CP867 Hebrew ST5
       78  SQL-CP-62214              VALUE    62214.
      * CCSID 62216 - CP867 Hebrew ST6
       78  SQL-CP-62216              VALUE    62216.
      * CCSID 62217 - CP867 Hebrew ST8
       78  SQL-CP-62217              VALUE    62217.
      * CCSID 62219 - CP867 Hebrew ST11
       78  SQL-CP-62219              VALUE    62219.
      * CCSID 62240 - CP856 Hebrew ST11
       78  SQL-CP-62240              VALUE    62240.

      * DBCS Codepages
      * Codepage 926 - Korean
       78  SQL-CP-926                VALUE    926.
      * Codepage 951 - New Korean
       78  SQL-CP-951                VALUE    951.
      * Codepage 301 - Japanese
       78  SQL-CP-301                VALUE    301.
      * Codepage 928 - Chinese
       78  SQL-CP-928                VALUE    928.
      * Codepage 927 - Taiwan
       78  SQL-CP-927                VALUE    927.
      * Codepage 941 - Japanese
       78  SQL-CP-941                VALUE    941.
      * Codepage 947 - Taiwan Big-5
       78  SQL-CP-947                VALUE    947.
      * Codepage 971 - Korean 5601
       78  SQL-CP-971                VALUE    971.
      * Codepage 1351 - Japanese
       78  SQL-CP-1351               VALUE    1351.
      * Codepage 1362 - Korean Windows
       78  SQL-CP-1362               VALUE    1362.
      * Codepage1380 - China GB
       78  SQL-CP-1380               VALUE    1380.
      * Codepage1382 - Simp Chinese GB
       78  SQL-CP-1382               VALUE    1382.
      * Codepage1385 - Simp Chinese GBK
       78  SQL-CP-1385               VALUE    1385.
      * Codepage 1393 - Japanese
       78  SQL-CP-1393               VALUE    1393.

      * Combined Codepages
      * Codepage 891 + 926 - Korean
       78  SQL-CP-934                VALUE    934.
      * CP 1088 + 951 - Korean Std
       78  SQL-CP-949                VALUE    949.
      * Codepage 897 + 301 - Japanese
       78  SQL-CP-932                VALUE    932.
      * Codepage 903 + 928 - Chinese
       78  SQL-CP-936                VALUE    936.
      * Codepage 904 + 927 - Taiwan
       78  SQL-CP-938                VALUE    938.
      * Codepage 1040 + 926 - Ext.Korean
       78  SQL-CP-944                VALUE    944.
      * Codepage 1041 + 301 - Ext.Japanese
       78  SQL-CP-942                VALUE    942.
      * Codepage  897 + 941 - Ext.Japanese
       78  SQL-CP-943                VALUE    943.
      * Codepage 1042 + 928 - Ext.Chinese
       78  SQL-CP-946                VALUE    946.
      * Codepage 1043 + 927 - Ext.Taiwan
       78  SQL-CP-948                VALUE    948.
      * Codepage 1114 + 947 - Taiwan Big5
       78  SQL-CP-950                VALUE    950.
      * Codepage 954 + 13488 - euc Japan
       78  SQL-CP-954                VALUE    954.
      * Codepage 964 + 13488 - euc Taiwan
       78  SQL-CP-964                VALUE    964.
      * Codepage  367 + 971 - Korean 5601
       78  SQL-CP-970                VALUE    970.
      * Codepage 1363 - Korean Windows
       78  SQL-CP-1363               VALUE    1363.
      * Codepage 1115 +1380 - China GB
       78  SQL-CP-1381               VALUE    1381.
      * Codepage  367 +1382 - Chinese GB
       78  SQL-CP-1383               VALUE    1383.
      * Codepage  1114 +1385 - Chinese GBK
       78  SQL-CP-1386               VALUE    1386.
      * Codepage  - Chinese GB18030
       78  SQL-CP-5488               VALUE    5488.
      * Codepage  - Chinese GB18030
       78  SQL-CP-1392               VALUE    1392.
      * Codepage  - Japanese Shift_JISX0213
       78  SQL-CP-1394               VALUE    1394.
      * Codepage  1041 + 1351 - Japanese
       78  SQL-CP-5039               VALUE    5039.
      * CCSID x21A4, (CP420, ST5)
       78  SQL-CP-8612               VALUE    8612.
      * CCSID xF30A, (CP864, ST8)
       78  SQL-CP-62218              VALUE    62218.
      * CCSID xF310, (CP420, ST6)
       78  SQL-CP-62224              VALUE    62224.
      * CCSID xF319, (CP420, ST8)
       78  SQL-CP-62233              VALUE    62233.
      * CCSID xF31A, (CP1255, ST9)
       78  SQL-CP-62234              VALUE    62234.
      * CCSID xF326, (CP1046, ST8)
       78  SQL-CP-62246              VALUE    62246.
      * CCSID xF327, (CP1046, ST9)
       78  SQL-CP-62247              VALUE    62247.
      * CCSID xF328, (CP1046, ST4)
       78  SQL-CP-62248              VALUE    62248.
      * CCSID xF329, (CP1046, ST2)
       78  SQL-CP-62249              VALUE    62249.
      * CCSID xF32A, (CP420, ST12)
       78  SQL-CP-62250              VALUE    62250.

      * Unicode CCSIDs
      * Codepage1200 - UCS-2 (big-endian)
       78  SQL-CP-1200               VALUE    1200.
      * Codepage1202 - UCS-2 (little-endian)
       78  SQL-CP-1202               VALUE    1202.
      * Codepage1208 - UTF-8
       78  SQL-CP-1208               VALUE    1208.
      * Codepage1232 - UTF-32 (big-endian)
       78  SQL-CP-1232               VALUE    1232.
      * Codepage1234 - UTF-32 (little-endian)
       78  SQL-CP-1234               VALUE    1234.
      * Codepg13488 - UCS-2 (Unicode v2, big-endian)
       78  SQL-CP-13488              VALUE    13488.
      * Codepg13490 - UCS-2 (Unicode v2, little-endiant)
       78  SQL-CP-13490              VALUE    13490.
      * Codepg17584 - UCS-2 (Unicode v3, big-endian)
       78  SQL-CP-17584              VALUE    17584.
      * Codepg17586 - UCS-2 (Unicode v3, little-endiant)
       78  SQL-CP-17586              VALUE    17586.
      * Big-endian UTF-16
       78  SQL-CP-UTF16BE            value    1200.
      * Little-endian UTF-16
       78  SQL-CP-UTF16LE            value    1202.

      * EBCDIC, PCDATA and ECECP CCSIDs
      * CCSID 37 - EBCDIC - Common European
       78  SQL-CP-37                 VALUE    37.
      * CCSID 273 - EBCDIC Austria, Germany
       78  SQL-CP-273                VALUE    273.
      * CCSID 274 - EBCDIC Belgium
       78  SQL-CP-274                VALUE    274.
      * CCSID 277 - EBCDIC Denmark, Norway
       78  SQL-CP-277                VALUE    277.
      * CCSID 278 - EBCDIC Finland, Sweden
       78  SQL-CP-278                VALUE    278.
      * CCSID 280 - EBCDIC Italy
       78  SQL-CP-280                VALUE    280.
      * CCSID 284 - EBCDIC Spain, Latin America
       78  SQL-CP-284                VALUE    284.
      * CCSID 285 - EBCDIC UK
       78  SQL-CP-285                VALUE    285.
      * CCSID 290 - EBCDIC Japan
       78  SQL-CP-290                VALUE    290.
      * CCSID 297 - EBCDIC France
       78  SQL-CP-297                VALUE    297.
      * CCSID 300 - EBCDIC Japan DBCS
       78  SQL-CP-300                VALUE    300.
      * CCSID 423 - EBCDIC Greece
       78  SQL-CP-423                VALUE    423.
      * CCSID 500 - EBCDIC Latin-1
       78  SQL-CP-500                VALUE    500.
      * CCSID 803 - EBCDIC Hebrew Set-A, ST4
       78  SQL-CP-803                VALUE    803.
      * CCSID 833 - EBCDIC Korea Extended SBCS
       78  SQL-CP-833                VALUE    833.
      * CCSID 834 - EBCDIC Korea DBCS (with 1880 UDC)
       78  SQL-CP-834                VALUE    834.
      * CCSID 835 - EBCDIC Taiwan DBCS (with 6204 UDC)
       78  SQL-CP-835                VALUE    835.
      * CCSID 836 - EBCDIC China SBCS
       78  SQL-CP-836                VALUE    836.
      * CCSID 837 - EBCDIC China DBCS (with 1880 UDC)
       78  SQL-CP-837                VALUE    837.
      * CCSID 838 - EBCDIC Thailand Extended SBCS
       78  SQL-CP-838                VALUE    838.
      * CCSID 870 - EBCDIC Latin-2
       78  SQL-CP-870                VALUE    870.
      * CCSID 871 - EBCDIC Iceland
       78  SQL-CP-871                VALUE    871.
      * CCSID 875 - EBCDIC Greece
       78  SQL-CP-875                VALUE    875.
      * CCSID 924 - EBCDIC Latin-9
       78  SQL-CP-924                VALUE    924.
      * CCSID 930 - EBCDIC Japan mix (with 4370 UDC, Extended SBCS)
       78  SQL-CP-930                VALUE    930.
      * CCSID 933 - EBCDIC Korea mix (with 1880 UDC, Extended SBCS)
       78  SQL-CP-933                VALUE    933.
      * CCSID 935 - EBCDIC China mix (with 1880 UDC, Extended SBCS)
       78  SQL-CP-935                VALUE    935.
      * CCSID 937 - EBCDIC Taiwan mix (with 6204 UDC, Extended SBCS)
       78  SQL-CP-937                VALUE    937.
      * CCSID 939 - EBCDIC Japan mix (with 4370 UDC, Extended SBCS)
       78  SQL-CP-939                VALUE    939.
      * CCSID 1025 - EBCDIC Cyrillic
       78  SQL-CP-1025               VALUE    1025.
      * CCSID 1026 - EBCDIC Turkey Latin-5
       78  SQL-CP-1026               VALUE    1026.
      * CCSID 1027 - EBCDIC Japan Extended SBCS
       78  SQL-CP-1027               VALUE    1027.
      * CCSID 1047 - EBCDIC Open Systems Latin-1
       78  SQL-CP-1047               VALUE    1047.
      * CCSID 1112 - EBCDIC Baltic
       78  SQL-CP-1112               VALUE    1112.
      * CCSID 1122 - EBCDIC Estonia
       78  SQL-CP-1122               VALUE    1122.
      * CCSID 1123 - EBCDIC Ukraine
       78  SQL-CP-1123               VALUE    1123.
      * CCSID 1130 - EBCDIC Vietnam
       78  SQL-CP-1130               VALUE    1130.
      * CCSID 1137 - EBCDIC Devangari (Based on Unicode)
       78  SQL-CP-1137               VALUE    1137.
      * CCSID 1153 - EBCDIC Latin-2 (with Euro)
       78  SQL-CP-1153               VALUE    1153.
      * CCSID 1154 - EBCDIC Cyrillic (with Euro)
       78  SQL-CP-1154               VALUE    1154.
      * CCSID 1156 - EBCDIC Baltic
       78  SQL-CP-1156               VALUE    1156.
      * CCSID 1157 - EBCDIC Estonia
       78  SQL-CP-1157               VALUE    1157.
      * CCSID 1158 - EBCDIC Ukraine
       78  SQL-CP-1158               VALUE    1158.
      * CCSID 1159 - EBCDIC Taiwan Extended SBCS (with Euro)
       78  SQL-CP-1159               VALUE    1159.
      * CCSID 1160 - EBCDIC Thailanf (with Euro)
       78  SQL-CP-1160               VALUE    1160.
      * CCSID 1164 - EBCDIC Vietnam (with Euro)
       78  SQL-CP-1164               VALUE    1164.
      * CCSID 1364 - EBCDIC Korea mix (with Full Hangul)
       78  SQL-CP-1364               VALUE    1364.
      * CCSID 1371 - EBCDIC Taiwan mix (with 6204 UDC, Extended SBCS)
       78  SQL-CP-1371               VALUE    1371.
      * CCSID 1388 - EBCDIC China mix
       78  SQL-CP-1388               VALUE    1388.
      * CCSID 1390 - EBCDIC Japan mix (with 6205 UDC, Extended SBCS, Euro)
       78  SQL-CP-1390               VALUE    1390.
      * CCSID 1399 - EBCDIC Japan max (with 6205 UDC, Exnteded SBCS, Euro)
       78  SQL-CP-1399               VALUE    1399.
      * CCSID 4369 - EBCDIC Austria, German (DP94)
       78  SQL-CP-4369               VALUE    4369.
      * CCSID 4396 - EBCDIC Japan DBCS (with 1880 UCD)
       78  SQL-CP-4396               VALUE    4396.
      * CCSID 4899 - EBCDIC Hebrew Set-A, ST4 (with Euro, Sheqel)
       78  SQL-CP-4899               VALUE    4899.
      * CCSID 4930 - EBCDIC Korea DBCS (with Full Hangul)
       78  SQL-CP-4930               VALUE    4930.
      * CCSID 4933 - EBCDIC China DBCS (with all GBK)
       78  SQL-CP-4933               VALUE    4933.
      * CCSID 4971 - EBCDIC Greece (with Euro)
       78  SQL-CP-4971               VALUE    4971.
      * CCSID 5026 - EBCDIC Japan mix (with 1880 UDC, Extended SBCS)
       78  SQL-CP-5026               VALUE    5026.
      * CCSID 5035 - EBCDIC Japan mix (with 1880 UDC, Extended SBCS)
       78  SQL-CP-5035               VALUE    5035.
      * CCSID 5123 - EBCDIC Japan Latin (with Extended SBCS, Euro)
       78  SQL-CP-5123               VALUE    5123.
      * CCSID 8482 - EBCDIC Japan SBCS (with Euro)
       78  SQL-CP-8482               VALUE    8482.
      * CCSID 8616 - EBCDIC Hebrew subset ST10
       78  SQL-CP-8616               VALUE    8616.
      * CCSID 9027 - EBCDIC Taiwan (with 6204 UDC, Euro)
       78  SQL-CP-9027               VALUE    9027.
      * CCSID 12708 - EBCDIC Arabic ST7
       78  SQL-CP-12708              VALUE    12708.
      * CCSID 12712 - EBCDIC Hebrew ST10 (with Euro, Sheqel)
       78  SQL-CP-12712              VALUE    12712.
      * CCSID 13121 - EBCDIC Korea (with Extended SBCS)
       78  SQL-CP-13121              VALUE    13121.
      * CCSID 13124 - EBCDIC China SBCS
       78  SQL-CP-13124              VALUE    13124.
      * CCSID 16684 - EBCDIC Japan (with 6295 UDC, Euro)
       78  SQL-CP-16684              VALUE    16684.
      * CCSID 16804 - EBCDIC Arabic ST4 (with Euro)
       78  SQL-CP-16804              VALUE    16804.
      * CCSID 28709 - EBCDIC Taiwan SBCS
       78  SQL-CP-28709              VALUE    28709.
      * CCSID 62211 - EBCDIC Hebrew ST5
       78  SQL-CP-62211              VALUE    62211.
      * CCSID 62229 - EBCDIC Hebrew ST8
       78  SQL-CP-62229              VALUE    62229.
      * CCSID 62235 - EBCDIC Hebrew ST6
       78  SQL-CP-62235              VALUE    62235.
      * CCSID 62245 - EBCDIC Hebrew ST10
       78  SQL-CP-62245              VALUE    62245.
      * CCSID 62251 - zOS Arabic/Latin ST6
       78  SQL-CP-62251              VALUE    62251.
      * CCSID 808 - PCDATA Cyrillic (Russian with Euro)
       78  SQL-CP-808                VALUE    808.
      * CCSID 848 - PCDATA Uktaine (with Euro)
       78  SQL-CP-848                VALUE    848.
      * CCSID 849 - PCDATA Belarus (with Euro)
       78  SQL-CP-849                VALUE    849.
      * CCSID 858 - PCDATA Latin-1E (with Euro)
       78  SQL-CP-858                VALUE    858.
      * CCSID 872 - PCDATA Cyrillic (with Euro)
       78  SQL-CP-872                VALUE    872.
      * CCSID 1161 - PCDATA Thailand (with Euero)
       78  SQL-CP-1161               VALUE    1161.
      * CCSID 1370 - PCDATA Taiwan mix (with Euro)
       78  SQL-CP-1370               VALUE    1370.
      * CCSID 5210 - PCDATA China SBCS
       78  SQL-CP-5210               VALUE    5210.
      * CCSID 9044 - PCDATA Latin-2 (with Euro)
       78  SQL-CP-9044               VALUE    9044.
      * CCSID 9048 - PCDATA Hebrew ST5 (with Euro, Sheqel)
       78  SQL-CP-9048               VALUE    9048.
      * CCSID 9049 - PCDATA Turkey Latin-5 (with Euro)
       78  SQL-CP-9049               VALUE    9049.
      * CCSID 9061 - PCDATA Greece (with Euro)
       78  SQL-CP-9061               VALUE    9061.
      * CCSID 9238 - PCDATA Arabic ST5 (with Euro)
       78  SQL-CP-9238               VALUE    9238.
      * CCSID 17248 - PCDATA Arabic ST5 (with Euro)
       78  SQL-CP-17248              VALUE    17248.
      * CCSID 1140 - ECECP Common European, US, Canada
       78  SQL-CP-1140               VALUE    1140.
      * CCSID 1141 - ECECP Austria, Germany
       78  SQL-CP-1141               VALUE    1141.
      * CCSID 1142 - ECECP Denmakr, Norway
       78  SQL-CP-1142               VALUE    1142.
      * CCSID 1143 - ECECP Finalnd, Sweden
       78  SQL-CP-1143               VALUE    1143.
      * CCSID 1144 - ECECP Italy
       78  SQL-CP-1144               VALUE    1144.
      * CCSID 1145 - ECECP Spanish, Latin America
       78  SQL-CP-1145               VALUE    1145.
      * CCSID 1146 - ECECP UK
       78  SQL-CP-1146               VALUE    1146.
      * CCSID 1147 - ECECP France
       78  SQL-CP-1147               VALUE    1147.
      * CCSID 1148 - ECECP International 1
       78  SQL-CP-1148               VALUE    1148.
      * CCSID 1149 - ECECP Iceland
       78  SQL-CP-1149               VALUE    1149.
      * CCSID 65535 - Reserved
       78  SQL-CP-65535              VALUE    65535.

      * Datastream Types
      * Single byte PC
       78  SQL-SBC-PC                VALUE    0.
      * Japanese-PC
       78  SQL-JPN-PC                VALUE    1.
      * Chinese-PC
       78  SQL-CHN-PC                VALUE    2.
      * Korean-PC
       78  SQL-KOR-PC                VALUE    3.
      * New Korean-PC
       78  SQL-KSC-PC                VALUE    4.
      * Korean 5601
       78  SQL-KR5-PC                VALUE    5.
      * Taiwan Big-5
       78  SQL-TWN-PC                VALUE    6.
      * China GB
       78  SQL-CGB-PC                VALUE    7.
      * China GB on AIX
       78  SQL-CGBA-PC               VALUE    8.
      * Japan euc
       78  SQL-EUCJP-PC              VALUE    9.
      * Taiwan euc
       78  SQL-EUCTW-PC              VALUE    10.
      * UCS-2
       78  SQL-UCS2-PC               VALUE    11.
      * Korean Windows 
       78  SQL-KSC2-PC               VALUE    12.
      * China GBK
       78  SQL-CGBK-PC               VALUE    13.
      * UTF-8
       78  SQL-UTF8-PC               VALUE    14.
      * China GB18030
       78  SQL-CGB18030-PC           VALUE    15.
      * Unknown
       78  SQL-UNKN-PC               VALUE    255.

      * AIX codeset & locale lengths
       78  SQL-CODESET-LEN           VALUE    9.
       78  SQL-LOCALE-LEN            VALUE    5.

      * Codeset & locale lengths for sqle_db_territory_info struct
       78  SQL-CODESET-SIZE          VALUE    17.
       78  SQL-LOCALE-SIZE           VALUE    33.

      * SQLCA Constants
      * Invalid SQLCA address
       78  SQL-RC-INVALID-SQLCA      VALUE    -1.

      * Size of SQLCA
       78  SQLCA-SIZE                VALUE    136.

      * SQL Error message token delimiter
       78  SQL-ERRMC-PRES            VALUE    255.

      * Offset in SQLERRD - Diagnostic information
      * return code
       78  SQL-ERRD-RC               VALUE    0.
      * reason code
       78  SQL-ERRD-REAS             VALUE    1.
      * nbr rows inserted/updated/deleted
       78  SQL-ERRD-CNT              VALUE    2.
      * optimizer estimate of # rows
       78  SQL-ERRD-OPT-CARD         VALUE    2.
      * obsolete -- do not use -- 
       78  SQL-ERRD-OPTM             VALUE    3.
      * optimzer estimate of total cost
       78  SQL-ERRD-OPT-TOTCOST      VALUE    3.
      * nbr of cascaded deletes/updates
       78  SQL-ERRD-DCNT             VALUE    4.
      * line number for recompile error
       78  SQL-ERRD-LINE             VALUE    4.
      * authentication type returned for CONNECT/ATTACH
       78  SQL-ERRD-AUTHTYPE         VALUE    4.
      * diagnostics
       78  SQL-ERRD-DIAG             VALUE    5.

      * Indexes in SQLWARN - Warning flags
      * composite - set if any warnings
       78  SQL-WARN-ANY              VALUE    0.
      * string column truncated
       78  SQL-WARN-TRUNC            VALUE    1.
      * null values eliminated
       78  SQL-WARN-NUL              VALUE    2.
      * nbr of columns/host vars mismatch
       78  SQL-WARN-MISM             VALUE    3.
      * no WHERE clause in update/delete
       78  SQL-WARN-ALLR             VALUE    4.
      * date has been truncated
       78  SQL-WARN-DATE             VALUE    6.
      * character conversion substitution
       78  SQL-WARN-SUB              VALUE    8.
      * arithmetic error nulls eliminated
       78  SQL-WARN-NUL2             VALUE    9.
      * SQLCA conversion error
       78  SQL-WARN-SQLCA            VALUE    10.

      * Values for Warning flags in SQLWARN
      * warning indicator
       78  SQL-WARNING               value    "W".
      * null terminator truncated warning
       78  SQL-NULL-TRN              value    "N".
      * truncation warning with application context length returned in
      * sqlind
       78  SQL-TRN-APP-LEN           value    "X".
      * no warning indicator
       78  SQL-NO-WARN               value    " ".

      * Compiler estimate warning indicator
       78  SQL-PREPARE-ESTIMATE-WARNING value "P".
      * SQLDA Constants

      * Increment for type with null indicator
       78  SQL-TYP-NULINC            VALUE    1.

      * Variable Types
      * DATE
       78  SQL-TYP-DATE              VALUE    384.
       78  SQL-TYP-NDATE             value    385.

      * TIME
       78  SQL-TYP-TIME              VALUE    388.
       78  SQL-TYP-NTIME             value    389.

      * TIMESTAMP
       78  SQL-TYP-STAMP             VALUE    392.
       78  SQL-TYP-NSTAMP            value    393.

      * DATALINK
       78  SQL-TYP-DATALINK          VALUE    396.
       78  SQL-TYP-NDATALINK         value    397.

      * C NUL-terminated graphic str
       78  SQL-TYP-CGSTR             VALUE    400.
       78  SQL-TYP-NCGSTR            value    401.

      * BLOB - varying length string
       78  SQL-TYP-BLOB              VALUE    404.
       78  SQL-TYP-NBLOB             value    405.

      * CLOB - varying length string
       78  SQL-TYP-CLOB              VALUE    408.
       78  SQL-TYP-NCLOB             value    409.

      * DBCLOB - varying length string
       78  SQL-TYP-DBCLOB            VALUE    412.
       78  SQL-TYP-NDBCLOB           value    413.

      * VARCHAR(i) - varying length string (2 byte length)
       78  SQL-TYP-VARCHAR           VALUE    448.
       78  SQL-TYP-NVARCHAR          value    449.

      * CHAR(i) - fixed length string
       78  SQL-TYP-CHAR              VALUE    452.
       78  SQL-TYP-NCHAR             value    453.

      * LONG VARCHAR - varying length string
       78  SQL-TYP-LONG              VALUE    456.
       78  SQL-TYP-NLONG             value    457.

      * varying length string for C (null terminated)
       78  SQL-TYP-CSTR              VALUE    460.
       78  SQL-TYP-NCSTR             value    461.

      * VARGRAPHIC(i) - varying length graphic string (2 byte length)
       78  SQL-TYP-VARGRAPH          VALUE    464.
       78  SQL-TYP-NVARGRAPH         value    465.

      * GRAPHIC(i) - fixed length graphic string
       78  SQL-TYP-GRAPHIC           VALUE    468.
       78  SQL-TYP-NGRAPHIC          value    469.

      * LONG VARGRAPHIC(i) - varying length graphic string
       78  SQL-TYP-LONGRAPH          VALUE    472.
       78  SQL-TYP-NLONGRAPH         value    473.

      * varying length string for Pascal (1-byte length)
       78  SQL-TYP-LSTR              VALUE    476.
       78  SQL-TYP-NLSTR             value    477.

      * FLOAT - 4 or 8 byte floating point
       78  SQL-TYP-FLOAT             VALUE    480.
       78  SQL-TYP-NFLOAT            value    481.

      * DECIMAL (m,n)
       78  SQL-TYP-DECIMAL           VALUE    484.
       78  SQL-TYP-NDECIMAL          value    485.

      * Zoned Decimal -> DECIMAL (m,n)
       78  SQL-TYP-ZONED             VALUE    488.
       78  SQL-TYP-NZONED            value    489.

      * BIGINT - 8-byte signed integer
       78  SQL-TYP-BIGINT            VALUE    492.
       78  SQL-TYP-NBIGINT           value    493.

      * INTEGER - 4-byte signed integer
       78  SQL-TYP-INTEGER           VALUE    496.
       78  SQL-TYP-NINTEGER          value    497.

      * SMALLINT - 2-byte signed integer
       78  SQL-TYP-SMALL             VALUE    500.
       78  SQL-TYP-NSMALL            value    501.

      * NUMERIC -> DECIMAL (m,n)
       78  SQL-TYP-NUMERIC           VALUE    504.
       78  SQL-TYP-NNUMERIC          value    505.

      * Obsolete Value
       78  SQL-TYP-BLOB-FILE-OBSOLETE VALUE   804.
       78  SQL-TYP-NBLOB-FILE-OBSOLETE value  805.

      * Obsolete Value
       78  SQL-TYP-CLOB-FILE-OBSOLETE VALUE   808.
       78  SQL-TYP-NCLOB-FILE-OBSOLETE value  809.

      * Obsolete Value
       78  SQL-TYP-DBCLOB-FILE-OBSOLETE VALUE 812.
       78  SQL-TYP-NDBCLOB-FILE-OBSOLETE value 813.

      * BLOB File - Binary Large Object File
       78  SQL-TYP-BLOB-FILE         VALUE    916.
       78  SQL-TYP-NBLOB-FILE        value    917.

      * CLOB File - Char Large Object File
       78  SQL-TYP-CLOB-FILE         VALUE    920.
       78  SQL-TYP-NCLOB-FILE        value    921.

      * DBCLOB File - Double Byte Char Large Object File
       78  SQL-TYP-DBCLOB-FILE       VALUE    924.
       78  SQL-TYP-NDBCLOB-FILE      value    925.

      * BLOB locator
       78  SQL-TYP-BLOB-LOCATOR      VALUE    960.
       78  SQL-TYP-NBLOB-LOCATOR     value    961.

      * CLOB locator
       78  SQL-TYP-CLOB-LOCATOR      VALUE    964.
       78  SQL-TYP-NCLOB-LOCATOR     value    965.

      * DBCLOB locator
       78  SQL-TYP-DBCLOB-LOCATOR    VALUE    968.
       78  SQL-TYP-NDBCLOB-LOCATOR   value    969.

       78  SQL-LOBTOKEN-LEN          value    4.
       78  SQL-TYP-BLOB-TOKEN        value    960.
       78  SQL-TYP-NBLOB-TOKEN       value    961.
       78  SQL-TYP-CLOB-TOKEN        value    964.
       78  SQL-TYP-NCLOB-TOKEN       value    965.
       78  SQL-TYP-DBCLOB-TOKEN      value    968.
       78  SQL-TYP-NDBCLOB-TOKEN     value    969.
       78  SQL-NCLOB-MAXLEN          value    1073741823.
       78  SQL-LOBHANDLE-LEN         value    4.
       78  SQL-TYP-BLOB-HANDLE       value    960.
       78  SQL-TYP-NBLOB-HANDLE      value    961.
       78  SQL-TYP-CLOB-HANDLE       value    964.
       78  SQL-TYP-NCLOB-HANDLE      value    965.
       78  SQL-TYP-DBCLOB-HANDLE     value    968.
       78  SQL-TYP-NDBCLOB-HANDLE    value    969.

      * Values for 30th byte of sqlname
       78  SQL-SQLNAME-SYSGEN        VALUE    255.
       78  SQL-SQLNAME-NOT-SYSGEN    VALUE    0.

      * Return Codes for sqlabndx, sqlaprep and sqlarbnd
      * The option(s) specified are not supported by the target  database
      * and will be ignored
       78  SQLA-RC-OPT-IGNORED       VALUE    20.
      * Bind execution succeeded with warnings.
       78  SQLA-RC-BINDWARN          VALUE    25.
      * Precompilation succeeded with warnings.
       78  SQLA-RC-PREPWARN          VALUE    25.
      * Bind execution failed
       78  SQLA-RC-BINDERROR         VALUE    -1.
      * Precompilation failed
       78  SQLA-RC-PREPERROR         VALUE    -1.
      * Invalid bind file
       78  SQLA-RC-BAD-BINDNAME      VALUE    -2.
      * Invalid database
       78  SQLA-RC-BAD-DBNAME        VALUE    -3.
      * Invalid message file
       78  SQLA-RC-BAD-MSGNAME       VALUE    -5.
      * Invalid format
       78  SQLA-RC-BAD-FORMAT        VALUE    -6.
      * Error opening list file
       78  SQLA-RC-OPEN-ERROR        VALUE    -31.
      * Error opening message file
       78  SQLA-RC-MFILE-OPEN-ERR    VALUE    -35.
      * Source file name is invalid
       78  SQLA-RC-FILE-NAME-BAD     VALUE    -36.
      * Bind file corrupted
       78  SQLA-RC-BAD-BNDFILE       VALUE    -39.
      * Bind list errors
       78  SQLA-RC-LIST-ERROR        VALUE    -40.
      * Interrupt
       78  SQLA-RC-INTERRUPT         VALUE    -94.
      * System error
       78  SQLA-RC-OSERROR           VALUE    -1086.
      * Prep/Bind already in use
       78  SQLA-RC-PREP-BIND-BUSY    VALUE    -1392.
      * Invalid parm. length
       78  SQLA-RC-OPTION-LEN-BAD    VALUE    -4903.
      * Invalid parm. ptr
       78  SQLA-RC-OPTION-PTR-BAD    VALUE    -4904.
      * Invalid parm. size
       78  SQLA-RC-OPTION-SIZE-BAD   VALUE    -4905.
      * Invalid parm. data
       78  SQLA-RC-OPTION-DATA-BAD   VALUE    -4917.
      * Invalid option or option value
       78  SQLA-RC-OPTION-INVALID    VALUE    -30104.
      * No SDK/6000 license
       78  SQLA-RC-SDK-LICENSE       VALUE    -8005.

      * Values used for the date/time format parameter of sqlabind() **
      * OBSOLETE **
      * FORMAT = Default for Country Code
       78  SQL-FMT-DEF               value    "DEF".
      * FORMAT = USA
       78  SQL-FMT-USA               value    "USA".
      * FORMAT = EUR
       78  SQL-FMT-EUR               value    "EUR".
      * FORMAT = ISO
       78  SQL-FMT-ISO               value    "ISO".
      * FORMAT = JIS
       78  SQL-FMT-JIS               value    "JIS".
      * FORMAT = LOCAL
       78  SQL-FMT-LOC               value    "LOC".

      * The size of a date/time format buffer
       78  SQL-FMT-LENGTH            VALUE    3.

      * Structures used system wide


      * File reference structure for LOBs
       01 SQLFILE.
           05 SQL-NAME-LENGTH        PIC 9(9) COMP-5.
           05 SQL-DATA-LENGTH        PIC 9(9) COMP-5.
           05 SQL-FILE-OPTIONS       PIC 9(9) COMP-5.
           05 SQL-NAME               PIC X(255).
           05 FILLER                 PIC X(1).

      * Values used for file_options in the sqlfile structure
      * Input file to read from
       78  SQL-FILE-READ             VALUE    2.
      * Output file - new file to be created
       78  SQL-FILE-CREATE           VALUE    8.
      * Output file - overwrite existing file or create a new file if it
      * doesn't exist
       78  SQL-FILE-OVERWRITE        VALUE    16.
      * Output file - append to an existing file or create a new file if it
      * doesn't exist
       78  SQL-FILE-APPEND           VALUE    32.

      * Structure used to store binder options when calling sqlabndr()
      * or sqlabndx(), or to store precompile options when calling
      * sqlaprep(), or to store rebind options when calling sqlarbnd().

      * Assume maximum number of binder/precompiler options ranges
      * between 1 and 50.

       01 SQLOPT.
           05 SQLOPTHEADER.
             10 ALLOCATED   PIC 9(9) COMP-5.
             10 USED        PIC 9(9) COMP-5.
           05 SQLOPTIONS OCCURS 1 TO 50 DEPENDING ON ALLOCATED.
             10 SQLOPT-TYPE        PIC 9(9) COMP-5.
      $IF P64 SET
             10 FILLER             PIC 9(9) COMP-5.
             10 SQLOPT-VAL         PIC 9(18) COMP-5.
             10 SQLOPT-VAL-PTR     REDEFINES SQLOPT-VAL
                                   USAGE IS POINTER.
      $ELSE
             10 SQLOPT-VAL         PIC 9(9) COMP-5.
             10 SQLOPT-VAL-PTR     REDEFINES SQLOPT-VAL
                                   USAGE IS POINTER.
      $END

      * Values used for option[n].type in the sqloptions structure
      * of sqlabndx(), sqlaprep() and sqlarbnd().
      * Option for date/time format - bind and precompile option
       78  SQL-DATETIME-OPT          VALUE    1.
      * Option for standards level compliance - precompile option only
       78  SQL-STANDARDS-OPT         VALUE    2.
      * Option for isolation level - bind and precompile option
       78  SQL-ISO-OPT               VALUE    4.
      * Option for record blocking - bind and precompile option
       78  SQL-BLOCK-OPT             VALUE    5.
      * Option for granting privileges - bind option only
       78  SQL-GRANT-OPT             VALUE    6.
      * Option for the Flagger - precompile option only
       78  SQL-FLAG-OPT              VALUE    8.
      * Option for granting privileges to a user - bind option only
       78  SQL-GRANT-USER-OPT        VALUE    9.
      * Option for granting privileges to a group - bind option only
       78  SQL-GRANT-GROUP-OPT       VALUE    10.
      * Option for adding NULLs to strings - bind option only
       78  SQL-CNULREQD-OPT          VALUE    11.
      * Generic option for DRDA servers - bind option only
       78  SQL-GENERIC-OPT           VALUE    12.
      * Option for Deferred Prepare - precompile option only
       78  SQL-DEFERRED-PREPARE-OPT  VALUE    15.
      * Specifies whether one or multiple connections are allowed
      * similtaneously within a unit of work.
       78  SQL-CONNECT-OPT           VALUE    16.
      * Specifies the set of rules used for connection to multiple databases
      * within a single unit of work
       78  SQL-RULES-OPT             VALUE    17.
      * Specifies which of multiple databases connected to will be
      * disconnected when a COMMIT or ROLLBACK is issued.
       78  SQL-DISCONNECT-OPT        VALUE    18.
      * Specifies what syncpoint option (for example one phase or two phase)
      * will be used
       78  SQL-SYNCPOINT-OPT         VALUE    19.
      * Option to create a bind file - precompile option only
       78  SQL-BIND-OPT              VALUE    20.
      * Option specifies SAA/non-SAA compatibility - FORTRAN precompile
      * option only
       78  SQL-SAA-OPT               VALUE    21.
      * Option to create a package with a specific name - precompile option
      * only
       78  SQL-PKG-OPT               VALUE    23.
      * Option to specify SQLDA optimization - precompile option only
       78  SQL-OPTIM-OPT             VALUE    24.
      * Option to not create a package or bind file - precompile option only
       78  SQL-SYNTAX-OPT            VALUE    25.
      * Indicates under what conditions a package will be be created - bind
      * and precompile option
       78  SQL-SQLERROR-OPT          value    25.
      * Option to suppress #line macro generation in modified source file -
      * C precompile option only
       78  SQL-LINEMACRO-OPT         VALUE    26.
      * 'No-op' option - ignore this entry in the option array - bind,
      * precompile and rebind option
       78  SQL-NO-OPT                VALUE    27.
      * Level of a module - precompile option only
       78  SQL-LEVEL-OPT             VALUE    30.
      * Package collection identifier - precompile option only
       78  SQL-COLLECTION-OPT        VALUE    31.
      * Package version identifier - precompile and rebind option only
       78  SQL-VERSION-OPT           VALUE    32.
      * Package owner authorization identifier - bind and precompile option
       78  SQL-OWNER-OPT             VALUE    33.
      * Synonym for owner - bind and precompile option
       78  SQL-SCHEMA-OPT            value    33.
      * Authorization identifier that is to be used as a qualifier for
      * unqualified objects - bind and precompile option
       78  SQL-QUALIFIER-OPT         VALUE    34.
      * Synonym for qualifier - bind and precompile option
       78  SQL-CATALOG-OPT           value    34.
      * Package description - bind and precompile option
       78  SQL-TEXT-OPT              VALUE    35.
      * Indicates when object validation occurs - bind and precompile option
       78  SQL-VALIDATE-OPT          VALUE    40.
      * Determines whether information will be produced about how the SQL
      * statements in a package will be executed - bind and precompile
      * option
       78  SQL-EXPLAIN-OPT           VALUE    41.
      * Indicates whether a package is to be added or replaced - bind and
      * precompile option
       78  SQL-ACTION-OPT            VALUE    42.
      * Replaces a specific version of a package - bind and precompile
      * option
       78  SQL-REPLVER-OPT           VALUE    44.
      * Indicates whether EXECUTE authorities are to be preserved when a
      * package is replaced - bind and precompile option
       78  SQL-RETAIN-OPT            VALUE    45.
      * Indicates whether resources are released at each COMMIT or when the
      * application  terminates - bind and precompile option
       78  SQL-RELEASE-OPT           VALUE    46.
      * Specifies whether or not the query is executed using I/O parallelism
      * - bind and precompile option
       78  SQL-DEGREE-OPT            VALUE    47.
      * Designates whether an apostrophe or quote will be used as a string
      * delimiter - bind and precompile option
       78  SQL-STRDEL-OPT            VALUE    50.
      * Designates whether a period or comma will be used as a decimal point
      * indicator - bind and precompile option
       78  SQL-DECDEL-OPT            VALUE    51.
      * Designates default character subtype that is to be used for column
      * definitions in the CREATE and ALTER TABLE SQL statements - bind and
      * precompile option
       78  SQL-CHARSUB-OPT           VALUE    55.
      * Designates what CCSID will be used for single byte characters for
      * character column definitions without a specific CCSID clause in the
      * CREATE and ALTER TABLE SQL statements - bind and precompile option
       78  SQL-CCSIDS-OPT            VALUE    56.
      * Designates what CCSID will be used for mixed byte characters for
      * character column definitions without a specific CCSID clause in the
      * CREATE and ALTER TABLE SQL statements - bind and precompile option
       78  SQL-CCSIDM-OPT            VALUE    57.
      * Designates what CCSID will be used for double byte characters for
      * character column definitions without a specific CCSID clause in the
      * CREATE and ALTER TABLE SQL statements - bind and precompile option
       78  SQL-CCSIDG-OPT            VALUE    58.
      * Specifies maximum precision to be used in decimal arithmetic
      * operations - bind and precompile option
       78  SQL-DEC-OPT               VALUE    59.
      * Specifies handling of graphic vars. - precompile only
       78  SQL-WCHAR-OPT             VALUE    60.
      * Specifies which authorization identifier to use when dynamic SQL in
      * a package is executed - bind and precompile option
       78  SQL-DYNAMICRULES-OPT      VALUE    61.
      * Buffers VALUE inserts - bind and precompile option for DB2/PE
      * servers only
       78  SQL-INSERT-OPT            VALUE    62.
      * Capture explain snapshot - bind and precompile option
       78  SQL-EXPLSNAP-OPT          VALUE    63.
      * Path for user-defined function resolution - bind and precompile
      * option
       78  SQL-FUNCTION-PATH         VALUE    64.
      * Disable prepare-time SQL warnings - bind and precompile option
       78  SQL-SQLWARN-OPT           VALUE    65.
      * Set query optimization class - bind and precompile option
       78  SQL-QUERYOPT-OPT          VALUE    66.
      * Target compiler - precompile option
       78  SQL-TARGET-OPT            VALUE    67.
      * Name of precompiler output file - precompile option
       78  SQL-PREP-OUTPUT-OPT       VALUE    68.
      * Preprocessor command - precompile option only
       78  SQL-PREPROCESSOR-OPT      VALUE    69.
      * Indicates whether function and type resolution should or should not
      * use conservative binding semantics - rebind option only
       78  SQL-RESOLVE-OPT           VALUE    70.
      * CLIPKG option - bind option only
       78  SQL-CLIPKG-OPT            VALUE    71.
      * FEDERATED option - bind and precompile option
       78  SQL-FEDERATED-OPT         VALUE    72.
      * Transform Group - precompile and bind option
       78  SQL-TRANSFORMGROUP-OPT    VALUE    73.
      * Option to treat long host variable declarations as errors -
      * precompile option only
       78  SQL-LONGERROR-OPT         VALUE    74.
      * DECTYPE Option to convert decimals  precompile option only
       78  SQL-DECTYPE-OPT           VALUE    75.
      * Specifies whether dynamic SQL statements are to be kept after commit
      * points - bind and precompile option
       78  SQL-KEEPDYNAMIC-OPT       VALUE    76.
      * Specifies what protocol to use when connecting to a remote site that
      * is identified by a three-part name statement - bind and precompile
      * option
       78  SQL-DBPROTOCOL-OPT        VALUE    77.
      * Controls whether query optimization hints are used for static SQL -
      * bind and precompile option
       78  SQL-OPTHINT-OPT           VALUE    78.
      * Tells whether immediate writes will be done for updates made to
      * group buffer pool dependent pagesets or partitions  - bind and
      * precompile option
       78  SQL-IMMEDWRITE-OPT        VALUE    79.
      * Specifies the encoding for all host variables in static statements
      * in the plan or package (bind and precompile option)
       78  SQL-ENCODING-OPT          VALUE    80.
      * Specifies which naming option is to be used when accessing DB2 UDB
      * for iSeries data - bind and precompile option
       78  SQL-OS400NAMING-OPT       VALUE    81.
      * Specifies which sort sequence table to use on the iSeries system -
      * bind and precompile option
       78  SQL-SORTSEQ-OPT           VALUE    82.
      * Specifies whether to have DB2 determine an access path at run time
      * using values for host variables, parameter markers, and special
      * registers - bind and precompile option
       78  SQL-REOPT-OPT             VALUE    83.
      * PSM option - bind option
       78  SQL-PSM-OPT               VALUE    84.
      * Specifies whether to use immediate ordeferred procedure
      * resolutionprecompile option
       78  SQL-CALL-RES-OPT          VALUE    85.
      * Option for date/time format - bind and precompile option
       78  SQL-TIMESTAMP-OPT         VALUE    86.
      * Specifies whether static cursors will be treated as read-only if
      * they are ambiguous - precompile and bind option
       78  SQL-STATICREADONLY-OPT    VALUE    87.
      * Specifies a two-part name of the form [schemaname.]basename where
      * basename is a character string of up to 128 chars in length used to
      * uniquely identify the optimization profile within a particular
      * schema.  Schemaname is a character string identifier of up to 30
      * bytes used to explicitelyqualify an optimization profile schema.
       78  SQL-OPTPROFILE-OPT        VALUE    88.

      * # of PREP/BIND/REBIND options
       78  SQL-NUM-OPTS              VALUE    88.

      * Values used for option[n].val when option[n].type is
      * SQL_DATETIME_OPT. These can also be used for the date/time
      * format parameter of sqlabind().

      * FORMAT = Default for Country Code
       78  SQL-DATETIME-DEF          VALUE    48.
      * FORMAT = USA
       78  SQL-DATETIME-USA          VALUE    49.
      * FORMAT = EUR
       78  SQL-DATETIME-EUR          VALUE    50.
      * FORMAT = ISO
       78  SQL-DATETIME-ISO          VALUE    51.
      * FORMAT = JIS
       78  SQL-DATETIME-JIS          VALUE    52.
      * FORMAT = LOCAL
       78  SQL-DATETIME-LOC          VALUE    53.

      * The following constants are here for backwards compatbility with
      * earlier releases.

       78  SQL-FRMT-OPT              value    1.
       78  SQL-FMT-0                 value    48.
       78  SQL-FMT-1                 value    49.
       78  SQL-FMT-2                 value    50.
       78  SQL-FMT-3                 value    51.
       78  SQL-FMT-4                 value    52.
       78  SQL-FMT-5                 value    53.

      * Values used for option[n].val when option[n].type is
      * SQL_STANDARDS_OPT.
      * SAA Level 1 Database CPI
       78  SQL-SAA-COMP              VALUE    0.
      * MIA
       78  SQL-MIA-COMP              VALUE    1.
      * SQL92 Entry
       78  SQL-SQL92E-COMP           VALUE    2.

      * Values used for option[n].val when option[n].type is SQL_ISO_OPT
      * Repeatable read level
       78  SQL-REP-READ              VALUE    0.
      * Cursor stability level
       78  SQL-CURSOR-STAB           VALUE    1.
      * Uncommitted read level
       78  SQL-UNCOM-READ            VALUE    2.
      * Read stability level
       78  SQL-READ-STAB             VALUE    3.
      * No Commit level      l
       78  SQL-NO-COMMIT             VALUE    4.

      * Values used for option[n].val when option[n].type is SQL_BLOCK_OPT
      * Block Unambiguous cursors
       78  SQL-BL-UNAMBIG            VALUE    0.
      * Block All cursors
       78  SQL-BL-ALL                VALUE    1.
      * Block No cursors
       78  SQL-NO-BL                 VALUE    2.

      * Values used for option[n].val when option[n].type is SQL_FLAG_OPT
      * Flagger check against MVS
       78  SQL-MVSDB2V23-SYNTAX      VALUE    4.
      * DB2 V2.3, V3.1 or V4.1 SQL
       78  SQL-MVSDB2V31-SYNTAX      VALUE    5.
      * syntax
       78  SQL-MVSDB2V41-SYNTAX      VALUE    6.
      * FIPS flagger SQL92E syntax
       78  SQL-SQL92E-SYNTAX         VALUE    7.

      * Values used for option[n].val when option[n].type is
      * SQL_CNULREQD_OPT
      * C NULL value not required
       78  SQL-CNULREQD-NO           VALUE    0.
      * C NULL value required
       78  SQL-CNULREQD-YES          VALUE    1.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_GENERIC_OPT
       78  SQL-MAX-GENERIC           VALUE    1023.

      * Values used for option[n].val when option[n].type is SQL_SAA_OPT.
      * SQLCA definition not SAA compatible
       78  SQL-SAA-NO                VALUE    0.
      * SQLCA definition is SAA compatible
       78  SQL-SAA-YES               VALUE    1.

      * Values used for option[n].val when option[n].type is SQL_OPTIM_OPT.
      * Do not optimize SQLDA initialization
       78  SQL-DONT-OPTIMIZE         VALUE    0.
      * Optimize SQLDA initialization
       78  SQL-OPTIMIZE              VALUE    1.

      * Values used for option[n].val when option[n].type is SQL_SYNTAX_OPT.
      * Create a package and/or a bind file
       78  SQL-NO-SYNTAX-CHECK       VALUE    0.
      * Do not create a package or bind file
       78  SQL-SYNTAX-CHECK          VALUE    1.

      * Values used for option[n].val when option[n].type is
      * SQL_LINEMACRO_OPT.
      * Do not generate #line macros in modified source file
       78  SQL-NO-LINE-MACROS        VALUE    0.
      * Generate #line macros in modified source file
       78  SQL-LINE-MACROS           VALUE    1.

      * Values used for option[n].val when option[n].type is SQL_WCHAR_OPT.
      * graphic variable not converted
       78  SQL-WCHAR-NOCONVERT       VALUE    0.
      * graphic variable converted
       78  SQL-WCHAR-CONVERT         VALUE    1.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_LEVEL_OPT
       78  SQL-MAX-LEVEL             VALUE    8.

      * Values used for option[n].val when option[n].type is SQL_CONNECT_OPT
      * Dynamic SQL statements will be chained.
       78  SQL-DEFERRED-PREPARE-YES  VALUE    1.
      * Dynamic SQL statements will not be chained.
       78  SQL-DEFERRED-PREPARE-NO   VALUE    2.
      * Dynamic SQL statements will be chained in all cases.  The
      * application must not allocate host vars a FETCH SQLDA until after
      * the OPEN statement for the cursor.
       78  SQL-DEFERRED-PREPARE-ALL  VALUE    3.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_COLLECTION_OPT
       78  SQL-MAX-COLLECTION        VALUE    30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_VERSION_OPT
       78  SQL-MAX-VERSION           VALUE    254.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_OWNER_OPT
       78  SQL-MAX-OWNER             VALUE    30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_SCHEMA_OPT
       78  SQL-MAX-SCHEMA            value    30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_QUALIFIER_OPT
       78  SQL-MAX-QUALIFIER         VALUE    30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_CATALOG_OPT
       78  SQL-MAX-CATALOG           value    30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_TEXT_OPT
       78  SQL-MAX-TEXT              VALUE    255.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_PREPROCESSOR_OPT
       78  SQL-MAX-PREPROCESSOR      VALUE    1024.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_TRANSFORMGROUP_OPT
       78  SQL-MAX-TRANSFORMGROUP    VALUE    18.

      * Values used for option[n].val when option[n].type is
      * SQL_VALIDATE_OPT
      * Validate objects during BIND
       78  SQL-VALIDATE-BIND         VALUE    0.
      * Validate objects during execution
       78  SQL-VALIDATE-RUN          VALUE    1.

      * Values used for option[n].val when option[n].type is SQL_EXPLAIN_OPT
      * No Explain output saved
       78  SQL-EXPLAIN-NO            VALUE    0.
      * Explain output saved
       78  SQL-EXPLAIN-YES           VALUE    1.
      * Explain output saved for all static and dynamic statements
       78  SQL-EXPLAIN-ALL           VALUE    2.
      * Explain output saved for static reoptimizable statements
       78  SQL-EXPLAIN-REOPT         VALUE    3.

      * Values used for option[n].val when option[n].type is SQL_ACTION_OPT
      * Package is to be added
       78  SQL-ACTION-ADD            VALUE    0.
      * Package is to be replaced
       78  SQL-ACTION-REPLACE        VALUE    1.

      * Max/Min value of CLIPKG for option[n].val when option[n].type is
      * SQL_CLIPKG_OPT
       78  SQL-MIN-CLIPKG            VALUE    3.
       78  SQL-MAX-CLIPKG            VALUE    30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_REPLVER_OPT
       78  SQL-MAX-REPLVER           VALUE    254.

      * Values used for option[n].val when option[n].type is
      * SQL_SQLERROR_OPT
      * Do not create a package if errors are encountered
       78  SQL-SQLERROR-NOPACKAGE    value    0.
      * Do not create a package
       78  SQL-SQLERROR-CHECK        value    1.
      * Create a package even if errors are encountered
       78  SQL-SQLERROR-CONTINUE     VALUE    2.

      * Values used for option[n].val when option[n].type is SQL_RETAIN_OPT
      * Do not preserve EXECUTE authorities when a package is replaced
       78  SQL-RETAIN-NO             VALUE    0.
      * Preserve EXECUTE authorities when a package is replaced
       78  SQL-RETAIN-YES            VALUE    1.

      * Values used for option[n].val when option[n].type is SQL_RELEASE_OPT
      * Release resources at COMMIT
       78  SQL-RELEASE-COMMIT        VALUE    0.
      * Release resources when the program terminates
       78  SQL-RELEASE-DEALLOCATE    VALUE    1.

      * Values used for option[n].val when option[n].type is SQL_STRDEL_OPT
      * Apostrophe string delimiter
       78  SQL-STRDEL-APOSTROPHE     VALUE    0.
      * Quote string delimiter
       78  SQL-STRDEL-QUOTE          VALUE    1.

      * Values used for option[n].val when option[n].type is SQL_DECDEL_OPT
      * Period is used as a decimal point indicator in decimal and floating
      * point literals
       78  SQL-DECDEL-PERIOD         VALUE    0.
      * Comma is used as a decimal point indicator in decimal and floating
      * point literals
       78  SQL-DECDEL-COMMA          VALUE    1.

      * Values used for option[n].val when option[n].type is SQL_CHARSUB_OPT
      * Use the target system defined default for all new character columns
      * for which an explicit subtype is not specified
       78  SQL-CHARSUB-DEFAULT       VALUE    0.
      * Use the BIT character subtype for all new character columns for
      * which an explicit subtype is not specified
       78  SQL-CHARSUB-BIT           VALUE    1.
      * Use the SBCS character subtype for all new character columns for
      * which an explicit subtype is not specified
       78  SQL-CHARSUB-SBCS          VALUE    2.
      * Use the mixed character subtype for all new character columns for
      * which an explicit subtype is not specified
       78  SQL-CHARSUB-MIXED         VALUE    3.

      * Values used for option[n].val when option[n].type is SQL_DEC_OPT
      * 15 bit precision is used in decimal arithmetic operations
       78  SQL-DEC-15                VALUE    15.
      * 31 bit precision is used in decimal arithmetic operations
       78  SQL-DEC-31                VALUE    31.

      * Values used for option[n].val when option[n].type is SQL_DEGREE_OPT
      * Prohibits parallel I/O operations
       78  SQL-DEGREE-1              VALUE    1.
      * Allows the target database system to determine the degree of
      * parallel I/O operations
       78  SQL-DEGREE-ANY            VALUE    0.
      * Maximum value
       78  SQL-MAX-DEGREE-VAL        VALUE    32767.

      * Values used for option[n].val when option[n].type is SQL_VERSION_OPT
      * Use the timestamp to generate the package VERSION
       78  SQL-VERSION-AUTO          value    "AUTO".

      * The next four option values (for CONNECT type, SQLRULES, DISCONNECT
      * and SYNCPOINT) are used not only by the precompiler but also by the
      * sqlesetc and sqleqryc APIs.

      * Values used for option[n].val when option[n].type is SQL_CONNECT_OPT
      * Indicates that only one connection to a database can exist at any
      * given time.
       78  SQL-CONNECT-1             VALUE    1.
      * Indicates that multiple connections can exist simultaneously, with
      * one being active and the others dormant.
       78  SQL-CONNECT-2             VALUE    2.

      * Values used for option[n].val when option[n].type is SQL_RULES_OPT
      * Indicates that CONNECT TO can be used to make a dormant connection
      * the current connection.
       78  SQL-RULES-DB2             VALUE    1.
      * Indicates that CONNECT TO is not valid for making a dormant
      * connection current, and SET CONNECTION must be used instead.
       78  SQL-RULES-STD             VALUE    2.

      * Values used for option[n].val when option[n].type is
      * SQL_DISCONNECT_OPT
      * Indicates that all connections marked by the RELEASE statement will
      * be the only connections released when a COMMIT is issued.
       78  SQL-DISCONNECT-EXPL       VALUE    1.
      * Indicates that all connections that do not have open WITH HOLD
      * cursors will be the only connections released when a COMMIT is
      * issued.
       78  SQL-DISCONNECT-COND       VALUE    2.
      * Indicates that all connections will be released when a COMMIT is
      * issued.
       78  SQL-DISCONNECT-AUTO       VALUE    3.

      * Values used for option[n].val when option[n].type is
      * SQL_SYNCPOINT_OPT
      * Do not use a transaction manager to perform two phase commit, but
      * enforce that there is only one database is updated when multiple
      * databases are accessed within a single transaction.
       78  SQL-SYNC-ONEPHASE         VALUE    1.
      * Use a transaction manager to coordinate two phase commit.
       78  SQL-SYNC-TWOPHASE         VALUE    2.
      * No update enforcement or two phase commit protocol will be used.
       78  SQL-SYNC-NONE             VALUE    0.

      * The next option value (for SQL_CONNECT_NODE) is used only by the
      * sqlesetc and sqleqryc APIs.
      * Connect to the catalog node
       78  SQL-CONN-CATALOG-NODE     VALUE    65534.

      * Values used for option[n].val when option[n].type is
      * SQL_DYNAMICRULES_OPT
      * Dynamic SQL in package will use authid of person running the package
       78  SQL-DYNAMICRULES-RUN      VALUE    0.
      * Dynamic SQL in package will use authid of person who owns the
      * package
       78  SQL-DYNAMICRULES-BIND     VALUE    1.
      * Dynamic SQL in a routine will use authid of invoker of routine
       78  SQL-DYNAMICRULES-INVOKERUN VALUE   2.
      * Dynamic SQL in a routine will use authid of definer of routine
       78  SQL-DYNAMICRULES-DEFINERUN VALUE   3.
      * Dynamic SQL in a routine will use authid of invoker of routine
       78  SQL-DYNAMICRULES-INVOKEBIND VALUE  4.
      * Dynamic SQL in a routine will use authid of definer of routine
       78  SQL-DYNAMICRULES-DEFINEBIND VALUE  5.

      * Values used for option[n].val when option[n].type is SQL_INSERT_OPT
      * Do not buffer VALUE inserts
       78  SQL-INSERT-DEF            VALUE    0.
      * Buffer VALUE inserts
       78  SQL-INSERT-BUF            VALUE    1.

      * Values used for option[n].val when option[n].type is
      * SQL_EXPLSNAP_OPT
      * No Explain snapshot saved
       78  SQL-EXPLSNAP-NO           VALUE    0.
      * Explain snapshot saved
       78  SQL-EXPLSNAP-YES          VALUE    1.
      * Explain snapshot saved for all static and dynamic statements
       78  SQL-EXPLSNAP-ALL          VALUE    2.
      * Explain snapshot saved for static reoptimizable statements
       78  SQL-EXPLSNAP-REOPT        VALUE    3.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_FUNCTION_PATH
       78  SQL-MAX-FUNCPATH          VALUE    254.

      * Values used for option[n].val when option[n].type is SQL_SQLWARN_OPT
      * Suppress prepare-time warning SQLCODEs
       78  SQL-SQLWARN-NO            VALUE    0.
      * Permit prepare-time warning SQLCODEs
       78  SQL-SQLWARN-YES           VALUE    1.

      * Values used for option[n].val when option[n].type is
      * SQL_QUERYOPT_OPT
      * Class 0 query optimization
       78  SQL-QUERYOPT-0            VALUE    0.
      * Class 1 query optimization
       78  SQL-QUERYOPT-1            VALUE    1.
      * Class 2 query optimization
       78  SQL-QUERYOPT-2            VALUE    2.
      * Class 3 query optimization
       78  SQL-QUERYOPT-3            VALUE    3.
      * Class 5 query optimization
       78  SQL-QUERYOPT-5            VALUE    5.
      * Class 7 query optimization
       78  SQL-QUERYOPT-7            VALUE    7.
      * Class 9 query optimization
       78  SQL-QUERYOPT-9            VALUE    9.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_TARGET_OPT
       78  SQL-MAX-TARGET-LEN        VALUE    32.

      * Values used for option[n].val when option[n].type is SQL_RESOLVE_OPT
      * Conservative binding semantics are not used
       78  SQL-RESOLVE-ANY           VALUE    0.
      * Conservative binding semantics are used
       78  SQL-RESOLVE-CONSERVATIVE  VALUE    1.

      * Values used for option[n].val when option[n].type is
      * SQL_FEDERATED_OPT
      * Federated systems are not supported
       78  SQL-FEDERATED-NO          VALUE    0.
      * Federated systems are supported
       78  SQL-FEDERATED-YES         VALUE    1.
      * Values used for option[n].val when option[n].type is SQL_PSM_OPT
      * PSM no
       78  SQL-PSM-NO                VALUE    0.
      * PSM yes
       78  SQL-PSM-YES               VALUE    1.

      * Values used for option[n].val when option[n].type is
      * SQL_LONGERROR_OPT.
      * Do not generate errors for the use of long host variable
      * declarations
       78  SQL-LONGERROR-NO          VALUE    0.
      * Generate errors for the use of long host variable declarations
       78  SQL-LONGERROR-YES         VALUE    1.

      * Values used for option[n].val when option[n].type is
      * SQL_DECTYPE_OPT.
      * Decimal type not converted 
       78  SQL-DECTYPE-NOCONVERT     VALUE    0.
      * Decimal type converted 
       78  SQL-DECTYPE-CONVERT       VALUE    1.

      * Values used for option[n].val when option[n].type is
      * SQL_KEEPDYNAMIC_OPT
      * Do not keep dynamic SQL statements after commit points
       78  SQL-KEEPDYNAMIC-NO        VALUE    0.
      * Keep dynamic SQL statements after commit points
       78  SQL-KEEPDYNAMIC-YES       VALUE    1.

      * Values used for option[n].val when option[n].type is
      * SQL_DBPROTOCOL_OPT
      * Use DRDA protocol when connecting to a remote site that is
      * identified by a three-part name statement
       78  SQL-DBPROTOCOL-DRDA       VALUE    0.
      * Use a private protocol when connecting to a remote site that is
      * identified by a three-part name statement
       78  SQL-DBPROTOCOL-PRIVATE    VALUE    1.

      * Values used for option[n].val when option[n].type is
      * SQL_IMMEDWRITE_OPT
      * Updated pages are written at or before phase two of commit
       78  SQL-IMMEDWRITE-NO         VALUE    0.
      * Updated pages are written as soon as the buffer update completes
       78  SQL-IMMEDWRITE-YES        VALUE    1.
      * Updated pages are written at or before phase one of commit
       78  SQL-IMMEDWRITE-PH1        VALUE    2.

      * Values used for option[n].val when option[n].type is
      * SQL_ENCODING_OPT
      * Host variables in static statements are encoded in ascii
       78  SQL-ENCODING-ASCII        VALUE    0.
      * Host variables in static statements are encoded in ebcdic
       78  SQL-ENCODING-EBCDIC       VALUE    1.
      * Host variables in static statements are encoded in unicode
       78  SQL-ENCODING-UNICODE      VALUE    2.

      * Values used for option[n].val when option[n].type is
      * SQL_OS400NAMING_OPT
      * Use the iSeries system naming option when accessing DB2 UDB for
      * iSeries data
       78  SQL-OS400NAMING-SYSTEM    VALUE    0.
      * Use the SQL naming option when accessing DB2 UDB for iSeries data
       78  SQL-OS400NAMING-SQL       VALUE    1.

      * Values used for option[n].val when option[n].type is SQL_SORTSEQ_OPT
      * Use the NLSS sort sequence table of the DRDA job on the iSeries
      * system
       78  SQL-SORTSEQ-JOBRUN        VALUE    0.
      * Use the EBCDIC sort sequence table of the DRDA job on the iSeries
      * system
       78  SQL-SORTSEQ-HEX           VALUE    1.

      * Values used for option[n].val when option[n].type is SQL_REOPT_OPT
      * Do not determine an access path at run time using values for host
      * variables, parameter markers, and special registers
       78  SQL-REOPT-NO              VALUE    0.
      * Re-determine an access path at run time using values for host
      * variables, parameter markers, and special registers
       78  SQL-REOPT-YES             VALUE    1.
      * The access path is determined based on the default estimates for
      * host variables, parameter markers, and special registers
       78  SQL-REOPT-NONE            VALUE    2.
      * Re-determine an access path only once at run time using values for
      * host variables, parameter markers, and special registers
       78  SQL-REOPT-ONCE            VALUE    3.
      * Re-determine an access path at every execution values for host
      * variables, parameter markers, and special registers
       78  SQL-REOPT-ALWAYS          VALUE    4.

      * Values used for option[n].val when option[n].type is
      * SQL_CALL_RES_OPT
      * Immediate SP call resolution
       78  SQL-CALL-RES-IMMED        VALUE    0.
      * Deferred SP call resolution
       78  SQL-CALL-RES-DEFERRED     VALUE    1.

      * Values used for option[n].val when option[n].type is
      * SQL_STATICREADONLY_OPT
      * Static cursors take on attributes as wouldnormally be generated
      * given the statementtext and setting of the LANGLEVEL option.
       78  SQL-STATICRO-NO           VALUE    0.
      * Any static cursor that does not contain theFOR UPDATE or FOR READ
      * ONLY clause will beconsidered READ ONLY.
       78  SQL-STATICRO-YES          VALUE    1.

      * Binder Interface Parameters/Return Codes


      * serial access
       78  SQL-CTX-ORIGINAL          VALUE    0.
      * concurrent access
       78  SQL-CTX-MULTI-MANUAL      VALUE    1.
      * trusted routine (internal)
       78  SQL-CTX-TRUSTED-ROUTINE   VALUE    2.
      * create & attach
       78  SQL-CTX-BEGIN-ALL         VALUE    0.
      * create only
       78  SQL-CTX-CREATE-ONLY       VALUE    1.
      * detach & free
       78  SQL-CTX-END-ALL           VALUE    0.
      * free only
       78  SQL-CTX-FREE-ONLY         VALUE    1.
      * Return Codes for sqlaintp/sqlogstt
      * insufficient memory for msg file
       78  SQLA-ERR-BADCC            VALUE    -1.
      * no error code in SQLCA
       78  SQLA-ERR-NOCOD            VALUE    -2.
      * message file inaccessible or incorrect
       78  SQLA-ERR-NOMSG            VALUE    -3.
      * specified line length negative
       78  SQLA-ERR-BADLL            VALUE    -4.
      * invalid sqlca/buffer addr/length
       78  SQLA-ERR-BADCA            VALUE    -5.

      * Administrative/Database Authorizations returned from Get
      * Administrative Authorizations function
      * Authorizations granted explicitly to user
      * SYSADM Authority
       78  SQL-SYSADM                VALUE    1.
      * DBADM Authority
       78  SQL-DBADM                 VALUE    2.
      * CREATETAB Privilege
       78  SQL-CREATETAB             VALUE    4.
      * BINDADD Privilege
       78  SQL-BINDADD               VALUE    8.
      * CONNECT Privilege
       78  SQL-CONNECT               VALUE    16.
      * CREATE_NOT_FENCED Privilege
       78  SQL-CREATE-NOT-FENC       VALUE    32.
      * SYSCTRL Authority
       78  SQL-SYSCTRL               VALUE    64.
      * SYSMAINT Authority
       78  SQL-SYSMAINT              VALUE    128.
      * IMPLICIT_SCHEMA Privilege
       78  SQL-IMPLICIT-SCHEMA       VALUE    65536.
      * LOAD Privilege
       78  SQL-LOAD                  VALUE    131072.
      * CREATE_EXTERNAL_ROUTINE Privilege
       78  SQL-CREATE-EXT-RT         VALUE    262144.
      * LIBRARYADM Privilege
       78  SQL-LIBADM                VALUE    524288.
      * QUIESCE_CONNECT Privilege
       78  SQL-QUIESCE-CONN          VALUE    1048576.
      * SECURITYADM Privilege
       78  SQL-SECADM                VALUE    2097152.
      * SYSQUIESCE Authority
       78  SQL-SYSQUIESCE            VALUE    4194304.
      * SYSMON Authority
       78  SQL-SYSMON                VALUE    8388608.

      * Composite of authorizations granted explicitly to user, to groups of
      * which user is a member, and to PUBLIC
       78  SQL-SYSADM-GRP            value    256.
       78  SQL-DBADM-GRP             value    512.
       78  SQL-CREATETAB-GRP         value    1024.
       78  SQL-BINDADD-GRP           value    2048.
       78  SQL-CONNECT-GRP           value    4096.
       78  SQL-CREATE-NOT-FENC-GRP   value    8192.
       78  SQL-SYSCTRL-GRP           value    16384.
       78  SQL-SYSMAINT-GRP          value    32768.
       78  SQL-IMPLICIT-SCHEMA-GRP   value    16777216.
       78  SQL-LOAD-GRP              value    33554432.
       78  SQL-CREATE-EXT-RT-GRP     value    67108864.
       78  SQL-LIBADM-GRP            value    134217728.
       78  SQL-QUIESCE-CONN-GRP      value    268435456.
       78  SQL-SECADM-GRP            value    536870912.
       78  SQL-SYSQUIESCE-GRP        value    1073741824.
       78  SQL-SYSMON-GRP            value    -2147483648.

      * Table/View Authorizations/Dependencies Bit definitions in
      * SYSTABAUTH.TABAUTH and SYSPLANDEP.TABAUTH
      * Control Authority
       78  SQL-TAB-CTL               VALUE    1.
      * Alter Privilege
       78  SQL-TAB-ALT               VALUE    2.
      * Delete Privilege/Dependency
       78  SQL-TAB-DEL               VALUE    4.
      * Index Privilege
       78  SQL-TAB-IDX               VALUE    8.
      * Insert Privilege/Dependency
       78  SQL-TAB-INS               VALUE    16.
      * Select Privilege/Dependency
       78  SQL-TAB-SEL               VALUE    32.
      * Update Privilege/Dependency
       78  SQL-TAB-UPD               VALUE    64.
      * Reference Privilege
       78  SQL-TAB-REF               VALUE    128.
      * Key Dependency
       78  SQL-TAB-KEY               VALUE    8192.
      * Cascade Dependency
       78  SQL-TAB-CAS               VALUE    16384.

      * Bit definitions for SYSTABAUTH.TABAUTH indicating the specified
      * table or view privilege is grantable.
      * Alter Privilege Grantable
       78  SQL-TAB-ALT-G             VALUE    512.
      * Delete Privilege Grantable
       78  SQL-TAB-DEL-G             VALUE    1024.
      * Index Privilege Grantable
       78  SQL-TAB-IDX-G             VALUE    2048.
      * Insert Privilege Grantable
       78  SQL-TAB-INS-G             VALUE    4096.
      * Select Privilege Grantable
       78  SQL-TAB-SEL-G             VALUE    8192.
      * Update Privilege Grantable
       78  SQL-TAB-UPD-G             VALUE    16384.
      * References Privilege Grantable
       78  SQL-TAB-REF-G             VALUE    32768.

      * Definitions for application remote interface
      * Unload Progam
       78  SQLZ-DISCONNECT-PROC      VALUE    1.
      * Keep program loaded
       78  SQLZ-HOLD-PROC            VALUE    2.
      *************************************************************************
      * 
      * The following functions and symbols are obsolete and may not be
      * supported in future releases. The obsolete functions are provided
      * for backward compatibility and exported from DB2API.LIB. All 
      * applications should be migrated to use new APIs.
      **************************************************************************
      * Invalid password
       78  SQLA-RC-BAD-PASSWD        VALUE    -4.
      * Maximum statements (see SQL reference)
       78  SQL-MAXSTMTS              VALUE    32767.
      * Maximum host variables per precompile unit (see SQL reference)
       78  SQL-MAXVARS               VALUE    32767.
      * Dynamic SQL in UDF or stored procedure will use authid of invoker of
      * UDF or stored procedure
       78  SQL-DYNAMICRULES-INVOKE   value    2.
      * Dynamic SQL in UDF or stored procedure will use authid of definer of
      * UDF or stored procedure
       78  SQL-DYNAMICRULES-DEFINE   value    3.
