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
       77  SQL-RELPRE6               PIC S9(4) COMP-5 VALUE 0.
      * V6.1.0.0
       77  SQL-REL6100               PIC S9(9) COMP-5 VALUE 6010000.
      * V7.1.0.0
       77  SQL-REL7100               PIC S9(9) COMP-5 VALUE 7010000.
      * V7.1.2.0
       77  SQL-REL7102               PIC S9(9) COMP-5 VALUE 7010200.
      * V7.2.0.0
       77  SQL-REL7200               PIC S9(9) COMP-5 VALUE 7020100.
      * V7.2.1.0
       77  SQL-REL7201               PIC S9(9) COMP-5 VALUE 7020200.
      * V7.2.4.0
       77  SQL-REL7204               PIC S9(9) COMP-5 VALUE 7020400.
      * V8.1.0.0
       77  SQL-REL8100               PIC S9(9) COMP-5 VALUE 8010000.
      * V8.1.1.0
       77  SQL-REL8101               PIC S9(9) COMP-5 VALUE 8010100.
      * V8.1.2.0
       77  SQL-REL8102               PIC S9(9) COMP-5 VALUE 8010200.
      * V8.1.3.0
       77  SQL-REL8103               PIC S9(9) COMP-5 VALUE 8010300.
      * V8.1.4.0
       77  SQL-REL8104               PIC S9(9) COMP-5 VALUE 8010400.
      * V8.1.5.0
       77  SQL-REL8105               PIC S9(9) COMP-5 VALUE 8010500.
      * V8.1.6.0
       77  SQL-REL8106               PIC S9(9) COMP-5 VALUE 8010600.
      * V8.2.0.0
       77  SQL-REL8200               PIC S9(9) COMP-5 VALUE 8020000.
      * V8.2.1.0
       77  SQL-REL8201               PIC S9(9) COMP-5 VALUE 8020100.
      * V8.2.2.0
       77  SQL-REL8202               PIC S9(9) COMP-5 VALUE 8020200.
      * V8.2.3.0
       77  SQL-REL8203               PIC S9(9) COMP-5 VALUE 8020300.
      * V8.2.4.0
       77  SQL-REL8204               PIC S9(9) COMP-5 VALUE 8020400.
      * Future Release
       77  SQL-FUTUREL               PIC S9(9) COMP-5 VALUE 9999999.

      * System Constants

      * successful execution
       77  SQL-RC-OK                 PIC S9(4) COMP-5 VALUE 0.

      * Maximum nbr of key parts in Index
       77  SQL-KEYPMAX               PIC S9(4) COMP-5 VALUE 16.
      * Maximum key length
       77  SQL-KEYLMAX               PIC S9(4) COMP-5 VALUE 1024.
      * Maximum size of field for sort
       77  SQL-SORTFLDLMT            PIC S9(9) COMP-5 VALUE 32677.
      * Maximum record length on a 4K page
       77  SQL-MAXRECL-4K            PIC S9(4) COMP-5 VALUE 4005.
      * Maximum record length on a 8K page
       77  SQL-MAXRECL-8K            PIC S9(4) COMP-5 VALUE 8101.
      * Maximum record length
       77  SQL-MAXRECL               PIC S9(9) COMP-5 VALUE 32677.
      * Maximum nbr of tables in a SELECT
       77  SQL-MAXTABLES             PIC S9(4) COMP-5 VALUE 15.
      * Maximum nbr of Host Vars per stmt
       77  SQL-MAXVARS-STMT          PIC S9(9) COMP-5 VALUE 32767.
      * Internal max nbr of columns in a table
       77  SQL-MAXCOLS               PIC S9(4) COMP-5 VALUE 3000.
      * External max nbr of columns in a table
       77  SQL-MAXCOLS-EXT           PIC S9(4) COMP-5 VALUE 1012.
      * Maximum nbr of items in a SELECT
       77  SQL-MAXSEL-ITEMS          PIC S9(4) COMP-5 VALUE 1012.
      * Maximum nbr of parms in a function
       77  SQL-MAXPARMS              PIC S9(4) COMP-5 VALUE 90.
      * Maximum statement size
       77  SQL-MAX-STMT-SIZ          PIC S9(9) COMP-5 VALUE 2097152.

      * Size of a SMALLINT
       77  SQL-SMALL-LENGTH          PIC S9(4) COMP-5 VALUE 2.
      * Maximum value of a SMALLINT
       77  SQL-MAXSMALLVAL           PIC S9(9) COMP-5 VALUE 32767.
      * Minimum value of a SMALLINT
       77  SQL-MINSMALLVAL           PIC S9(9) COMP-5 VALUE -32768.
      * Size of an INTEGER
       77  SQL-INT-LENGTH            PIC S9(4) COMP-5 VALUE 4.
      * Maximum value of an INTEGER
      * Value of SQL-MAXINTVAL is 2147483647
       01  SQL-MAXINTVAL-INIT PIC X(4) VALUE x"7FFFFFFF"
           USAGE DISPLAY NATIVE.
       01  SQL-MAXINTVAL REDEFINES
           SQL-MAXINTVAL-INIT PIC S9(9) COMP-5.
      * Minimum value of an INTEGER
      * Value of SQL-MININTVAL is -2147483648
       01  SQL-MININTVAL-INIT PIC X(4) VALUE x"80000000"
           USAGE DISPLAY NATIVE.
       01  SQL-MININTVAL REDEFINES
           SQL-MININTVAL-INIT PIC S9(9) COMP-5.
      * Size of a BIGINT
       77  SQL-BIGINT-LENGTH         PIC S9(4) COMP-5 VALUE 8.
      * Maximum value of a BIGINT
      * Value of SQL-MAXBIGINTVAL is 9223372036854775807
       01  SQL-MAXBIGINTVAL-INIT PIC X(8) VALUE x"7FFFFFFFFFFFFFFF"
           USAGE DISPLAY NATIVE.
       01  SQL-MAXBIGINTVAL REDEFINES
           SQL-MAXBIGINTVAL-INIT PIC S9(18) COMP-5.
      * Minimum value of a BIGINT
      * Value of SQL-MINBIGINTVAL is -9223372036854775808
       01  SQL-MINBIGINTVAL-INIT PIC X(8) VALUE x"8000000000000000"
           USAGE DISPLAY NATIVE.
       01  SQL-MINBIGINTVAL REDEFINES
           SQL-MINBIGINTVAL-INIT PIC S9(18) COMP-5.
      * Size of a FLOAT
       77  SQL-FLOAT-LENGTH          PIC S9(4) COMP-5 VALUE 8.
      * Size of a 4-byte FLOAT
       77  SQL-FLOAT4-LENGTH         PIC S9(4) COMP-5 VALUE 4.
      * Maximum prec for small float
       77  SQL-MAXSFLTPREC           PIC S9(4) COMP-5 VALUE 24.
      * Minimum prec for small float
       77  SQL-MINSFLTPREC           PIC S9(4) COMP-5 VALUE 1.
      * Minimum prec for any float
       77  SQL-MAXFLOATPREC          PIC S9(4) COMP-5 VALUE 53.
      * Default precision for DECIMAL
       77  SQL-DEFDEC-PRECISION      PIC S9(4) COMP-5 VALUE 5.
      * Default scale for DECIMAL
       77  SQL-DEFDEC-SCALE          PIC S9(4) COMP-5 VALUE 0.
      * Maximum scale/prec. for DECIMAL
       77  SQL-MAXDECIMAL            PIC S9(4) COMP-5 VALUE 31.
      * Default length for a CHAR
       77  SQL-DEFCHAR               PIC S9(4) COMP-5 VALUE 1.
      * Default length for a graphic
       77  SQL-DEFWCHAR              PIC S9(4) COMP-5 VALUE 1.
      * Maximum length of a CHAR
       77  SQL-MAXCHAR               PIC S9(4) COMP-5 VALUE 254.
      * Maximum length of an LSTRING
       77  SQL-MAXLSTR               PIC S9(4) COMP-5 VALUE 255.
      * Maximum length of a VARCHAR
       77  SQL-MAXVCHAR              PIC S9(9) COMP-5 VALUE 32672.
      * Maximum length of a VARGRAPHIC
       77  SQL-MAXVGRAPH             PIC S9(9) COMP-5 VALUE 16336.
      * Max. length of a BLOB host var
      * Value of SQL-MAXBLOB is 2147483647
       01  SQL-MAXBLOB-INIT PIC X(4) VALUE x"7FFFFFFF"
           USAGE DISPLAY NATIVE.
       01  SQL-MAXBLOB REDEFINES
           SQL-MAXBLOB-INIT PIC S9(9) COMP-5.
      * Max. length of a CLOB host var
      * Value of SQL-MAXCLOB is 2147483647
       01  SQL-MAXCLOB-INIT PIC X(4) VALUE x"7FFFFFFF"
           USAGE DISPLAY NATIVE.
       01  SQL-MAXCLOB REDEFINES
           SQL-MAXCLOB-INIT PIC S9(9) COMP-5.
      * Max. length of an DBCLOB host var
      * Value of SQL-MAXDBCLOB is 1073741823
       01  SQL-MAXDBCLOB-INIT PIC X(4) VALUE x"3FFFFFFF"
           USAGE DISPLAY NATIVE.
       01  SQL-MAXDBCLOB REDEFINES
           SQL-MAXDBCLOB-INIT PIC S9(9) COMP-5.
      * Length of a LOB locator host var
       77  SQL-LOBLOCATOR-LEN        PIC S9(4) COMP-5 VALUE 4.
      * Length of a LOB file host var
       77  SQL-LOBFILE-LEN           PIC S9(4) COMP-5 VALUE 267.
      * Overhead for VARCHAR in record
       77  SQL-VCHAROH               PIC S9(4) COMP-5 VALUE 4.
      * Overhead for variable length type in record
       77  SQL-VARCOL-OH             PIC S9(4) COMP-5 VALUE 4.
      * Overhead for variable keyparts
       77  SQL-VARKEY-OH             PIC S9(4) COMP-5 VALUE 2.
      * Maximum length of a LONG VARCHAR
       77  SQL-LONGMAX               PIC S9(9) COMP-5 VALUE 32700.
      * Max. length of a LONG VARGRAPHIC
       77  SQL-LONGGRMAX             PIC S9(9) COMP-5 VALUE 16350.
      * Overhead for LONG VARCHAR in record
       77  SQL-LVCHAROH              PIC S9(4) COMP-5 VALUE 24.
      * Overhead for LOB in record
       77  SQL-LOBCHAROH             PIC S9(4) COMP-5 VALUE 312.
      * BLOB maximum length, in bytes
      * Value of SQL-BLOB-MAXLEN is 2147483647
       01  SQL-BLOB-MAXLEN-INIT PIC X(4) VALUE x"7FFFFFFF"
           USAGE DISPLAY NATIVE.
       01  SQL-BLOB-MAXLEN REDEFINES
           SQL-BLOB-MAXLEN-INIT PIC S9(9) COMP-5.
      * CLOB maximum length, in chars
      * Value of SQL-CLOB-MAXLEN is 2147483647
       01  SQL-CLOB-MAXLEN-INIT PIC X(4) VALUE x"7FFFFFFF"
           USAGE DISPLAY NATIVE.
       01  SQL-CLOB-MAXLEN REDEFINES
           SQL-CLOB-MAXLEN-INIT PIC S9(9) COMP-5.
      * maxlen for dbcs lobs
      * Value of SQL-DBCLOB-MAXLEN is 1073741823
       01  SQL-DBCLOB-MAXLEN-INIT PIC X(4) VALUE x"3FFFFFFF"
           USAGE DISPLAY NATIVE.
       01  SQL-DBCLOB-MAXLEN REDEFINES
           SQL-DBCLOB-MAXLEN-INIT PIC S9(9) COMP-5.
      * Size of a TIME field
       77  SQL-TIME-LENGTH           PIC S9(4) COMP-5 VALUE 3.
      * Size of a TIME field output
       77  SQL-TIME-STRLEN           PIC S9(4) COMP-5 VALUE 8.
      * Size of a non-USA TIME field output without seconds
       77  SQL-TIME-MINSTRLEN        PIC S9(4) COMP-5 VALUE 5.
      * Size of a DATE field
       77  SQL-DATE-LENGTH           PIC S9(4) COMP-5 VALUE 4.
      * Size of a DATE field output
       77  SQL-DATE-STRLEN           PIC S9(4) COMP-5 VALUE 10.
      * Size of a TIMESTAMP field
       77  SQL-STAMP-LENGTH          PIC S9(4) COMP-5 VALUE 10.
      * Size of a TIMESTAMP field output
       77  SQL-STAMP-STRLEN          PIC S9(4) COMP-5 VALUE 26.
      * Size of a TIMESTAMP field output without microseconds
       77  SQL-STAMP-MINSTRLEN       PIC S9(4) COMP-5 VALUE 19.
      * Size of a BOOLEAN field
       77  SQL-BOOLEAN-LENGTH        PIC S9(4) COMP-5 VALUE 1.
      * Size of a DATALINK field
       77  SQL-DATALINK-LENGTH       PIC S9(4) COMP-5 VALUE 254.
      * Size of an indicator value
       77  SQL-IND-LENGTH            PIC S9(4) COMP-5 VALUE 2.

      * Max size of Stored Proc Name
       77  SQL-MAX-PNAME-LENGTH      PIC S9(4) COMP-5 VALUE 254.
      * Maximum length of Identifer
       77  SQL-MAX-IDENT             PIC S9(4) COMP-5 VALUE 128.
      * Maximum length of Long Identifer
       77  SQL-LG-IDENT              PIC S9(4) COMP-5 VALUE 18.
      * Maximum length of Short Identifer
       77  SQL-SH-IDENT              PIC S9(4) COMP-5 VALUE 8.
      * Minimum length of Identifiers
       77  SQL-MN-IDENT              PIC S9(4) COMP-5 VALUE 1.
      * Max size of Host Variable Name
       77  SQL-MAX-VAR-NAME          PIC S9(4) COMP-5 VALUE 255.
      * Number of partitions in a pmap
       77  SQL-PDB-MAP-SIZE          PIC S9(4) COMP-5 VALUE 4096.
      * Max # of Partition Keys
       77  SQL-MAX-NUM-PART-KEYS     PIC S9(4) COMP-5 VALUE 500.
      * decimal columns for sqlugrpn are in zoneddecimal format
       77  SQL-ZONEDDECIMAL-FORMAT   PIC S9(4) COMP-5 VALUE 32.
      * decimal columns for sqlugrpn are in implieddecimal format
       77  SQL-IMPLIEDDECIMAL-FORMAT PIC S9(4) COMP-5 VALUE 16.
      * numeric columns for sqlugrpn are in binary format
       77  SQL-BINARYNUMERICS-FORMAT PIC S9(4) COMP-5 VALUE 4.
      * decimal columns for sqlugrpn are in packeddecimal format
       77  SQL-PACKEDDECIMAL-FORMAT  PIC S9(4) COMP-5 VALUE 8.
      * numeric/decimal columns for sqlugrpn are in character string format
       77  SQL-CHARSTRING-FORMAT     PIC S9(4) COMP-5 VALUE 0.
      * # of bytes in a kilobyte
       77  SQL-KILO-VALUE            PIC S9(4) COMP-5 VALUE 1024.
      * # of bytes in a megabyte
       77  SQL-MEGA-VALUE            PIC S9(9) COMP-5 VALUE 1048576.
      * # of bytes in a gigabyte
      * Value of SQL-GIGA-VALUE is 1073741824
       01  SQL-GIGA-VALUE-INIT PIC X(4) VALUE x"40000000"
           USAGE DISPLAY NATIVE.
       01  SQL-GIGA-VALUE REDEFINES
           SQL-GIGA-VALUE-INIT PIC S9(9) COMP-5.

      * size of container name for api's (includes 1 byte for C NULL
      * terminator)
       77  SQLB-MAX-CONTAIN-NAME-SZ  PIC S9(4) COMP-5 VALUE 256.
      * System types 
      * limit for max. # of nodes
       77  SQL-PDB-MAX-NUM-NODE      PIC S9(4) COMP-5 VALUE 1000.
      * information related to logical node name
      * no. of digits for node number in node name
       77  SQL-PDB-NODE-NUM-DIGIT    PIC S9(4) COMP-5 VALUE 4.
      * length of logical node name
       77  SQL-PDB-NODE-NAME-LEN     PIC S9(4) COMP-5 VALUE 8.
      * Codepages
      * Codepage 367 - EUC single byte
       77  SQL-CP-367                PIC S9(4) COMP-5 VALUE 367.
      * CCSID x01A4, (CP420, ST4)
       77  SQL-CP-420                PIC S9(4) COMP-5 VALUE 420.
      * CCSID x01A8, (CP424, ST4)
       77  SQL-CP-424                PIC S9(4) COMP-5 VALUE 424.
      * CCSID x01A9, (CP420, ST5)
       77  SQL-CP-425                PIC S9(4) COMP-5 VALUE 425.
      * Codepage 437 - US, Europe
       77  SQL-CP-437                PIC S9(4) COMP-5 VALUE 437.
      * Codepage 737 - WIN Greece
       77  SQL-CP-737                PIC S9(4) COMP-5 VALUE 737.
      * Codepage 806 - ISCII, India
       77  SQL-CP-806                PIC S9(4) COMP-5 VALUE 806.
      * Codepage 813 - AIX Greece
       77  SQL-CP-813                PIC S9(4) COMP-5 VALUE 813.
      * Codepage 819 - ISO 8859-1
       77  SQL-CP-819                PIC S9(4) COMP-5 VALUE 819.
      * Codepage 850 - International PC
       77  SQL-CP-850                PIC S9(4) COMP-5 VALUE 850.
      * Codepage 855 - OS2 Cyrillic
       77  SQL-CP-855                PIC S9(4) COMP-5 VALUE 855.
      * Codepage 852 - OS2 Latin2
       77  SQL-CP-852                PIC S9(4) COMP-5 VALUE 852.
      * Codepage 856 - Hebrew
       77  SQL-CP-856                PIC S9(4) COMP-5 VALUE 856.
      * Codepage 857 - OS2 Turkey
       77  SQL-CP-857                PIC S9(4) COMP-5 VALUE 857.
      * Codepage 860 - Portuguese
       77  SQL-CP-860                PIC S9(4) COMP-5 VALUE 860.
      * Codepage 862 - OS2 Hebrew
       77  SQL-CP-862                PIC S9(4) COMP-5 VALUE 862.
      * Codepage 863 - Canadian-French
       77  SQL-CP-863                PIC S9(4) COMP-5 VALUE 863.
      * Codepage 864 - OS2 Arabic
       77  SQL-CP-864                PIC S9(4) COMP-5 VALUE 864.
      * Codepage 865 - Norway, Denmark
       77  SQL-CP-865                PIC S9(4) COMP-5 VALUE 865.
      * Codepage 866 - Russia
       77  SQL-CP-866                PIC S9(4) COMP-5 VALUE 866.
      * Codepage 867 - OS2 Hebrew
       77  SQL-CP-867                PIC S9(4) COMP-5 VALUE 867.
      * Codepage 869 - OS2 Greece
       77  SQL-CP-869                PIC S9(4) COMP-5 VALUE 869.
      * Codepage 874 - OS2/AIX Thailand
       77  SQL-CP-874                PIC S9(4) COMP-5 VALUE 874.
      * Codepage 878 - KOI-8R Russia
       77  SQL-CP-878                PIC S9(4) COMP-5 VALUE 878.
      * Codepage 891 - Korean
       77  SQL-CP-891                PIC S9(4) COMP-5 VALUE 891.
      * Codepage 897 - Japanese
       77  SQL-CP-897                PIC S9(4) COMP-5 VALUE 897.
      * Codepage 903 - Chinese
       77  SQL-CP-903                PIC S9(4) COMP-5 VALUE 903.
      * Codepage 904 - Taiwan
       77  SQL-CP-904                PIC S9(4) COMP-5 VALUE 904.
      * Codepage 912 - AIX Latin2
       77  SQL-CP-912                PIC S9(4) COMP-5 VALUE 912.
      * Codepage 915 - AIX Cyrillic
       77  SQL-CP-915                PIC S9(4) COMP-5 VALUE 915.
      * Codepage 916 - AIX Hebrew
       77  SQL-CP-916                PIC S9(4) COMP-5 VALUE 916.
      * Codepage 920 - AIX Turkey
       77  SQL-CP-920                PIC S9(4) COMP-5 VALUE 920.
      * Codepage 921 - Latvia, Lithuania
       77  SQL-CP-921                PIC S9(4) COMP-5 VALUE 921.
      * Codepage 922 - Estonia
       77  SQL-CP-922                PIC S9(4) COMP-5 VALUE 922.
      * Codepage 923 - ISO 8859-15
       77  SQL-CP-923                PIC S9(4) COMP-5 VALUE 923.
      * Codepage 1004 - MS-WINDOWS
       77  SQL-CP-1004               PIC S9(4) COMP-5 VALUE 1004.
      * Codepage 1040 - Extended Korean
       77  SQL-CP-1040               PIC S9(4) COMP-5 VALUE 1040.
      * Codepage 1041 - Extended Japanese
       77  SQL-CP-1041               PIC S9(4) COMP-5 VALUE 1041.
      * Codepage 1042 - Extended Chinese
       77  SQL-CP-1042               PIC S9(4) COMP-5 VALUE 1042.
      * Codepage 1043 - Extended Taiwan
       77  SQL-CP-1043               PIC S9(4) COMP-5 VALUE 1043.
      * Codepage 1046 - AIX Arabic
       77  SQL-CP-1046               PIC S9(4) COMP-5 VALUE 1046.
      * Codepage 1051 - HP Roman8
       77  SQL-CP-1051               PIC S9(4) COMP-5 VALUE 1051.
      * Codepage 1088 - Korea Std
       77  SQL-CP-1088               PIC S9(4) COMP-5 VALUE 1088.
      * Codepage 1089 - AIX Arabic
       77  SQL-CP-1089               PIC S9(4) COMP-5 VALUE 1089.
      * Codepage 1114 - Big-5 & GBK
       77  SQL-CP-1114               PIC S9(4) COMP-5 VALUE 1114.
      * Codepage 1115 - China GB
       77  SQL-CP-1115               PIC S9(4) COMP-5 VALUE 1115.
      * Codepage 1124 - AIX Ukraine 
       77  SQL-CP-1124               PIC S9(4) COMP-5 VALUE 1124.
      * Codepage 1125 - OS/2 Ukraine 
       77  SQL-CP-1125               PIC S9(4) COMP-5 VALUE 1125.
      * Codepage 1126 - Windows Korean Std
       77  SQL-CP-1126               PIC S9(4) COMP-5 VALUE 1126.
      * Codepage 1129 - Vietnamese
       77  SQL-CP-1129               PIC S9(4) COMP-5 VALUE 1129.
      * Codepage 1131 - OS/2 Belarus 
       77  SQL-CP-1131               PIC S9(4) COMP-5 VALUE 1131.
      * Codepage 1163 - Vietnamese
       77  SQL-CP-1163               PIC S9(4) COMP-5 VALUE 1163.
      * KOI8-RU - Belarus
       77  SQL-CP-1167               PIC S9(4) COMP-5 VALUE 1167.
      * KOI8-U - Ukraine
       77  SQL-CP-1168               PIC S9(4) COMP-5 VALUE 1168.
      * Codepage 1250 - Windows Latin-2
       77  SQL-CP-1250               PIC S9(4) COMP-5 VALUE 1250.
      * Codepage 1251 - Windows Cyrillic
       77  SQL-CP-1251               PIC S9(4) COMP-5 VALUE 1251.
      * Codepage 1252 - Windows Latin-1
       77  SQL-CP-1252               PIC S9(4) COMP-5 VALUE 1252.
      * Codepage 1253 - Windows Greek
       77  SQL-CP-1253               PIC S9(4) COMP-5 VALUE 1253.
      * Codepage 1254 - Windows Turkish
       77  SQL-CP-1254               PIC S9(4) COMP-5 VALUE 1254.
      * Codepage 1255 - Windows Hebrew
       77  SQL-CP-1255               PIC S9(4) COMP-5 VALUE 1255.
      * Codepage 1256 - Windows Arabic
       77  SQL-CP-1256               PIC S9(4) COMP-5 VALUE 1256.
      * Codepage 1257 - Windows Baltic
       77  SQL-CP-1257               PIC S9(4) COMP-5 VALUE 1257.
      * Codepage 1258 - Windows Vietnamese
       77  SQL-CP-1258               PIC S9(4) COMP-5 VALUE 1258.
      * Codepage 1275 - Mac Latin-1
       77  SQL-CP-1275               PIC S9(4) COMP-5 VALUE 1275.
      * Codepage 1280 - Mac Greek
       77  SQL-CP-1280               PIC S9(4) COMP-5 VALUE 1280.
      * Codepage 1281 - Mac Turkish
       77  SQL-CP-1281               PIC S9(4) COMP-5 VALUE 1281.
      * Codepage 1282 - Mac Latin-2
       77  SQL-CP-1282               PIC S9(4) COMP-5 VALUE 1282.
      * Codepage 1283 - Mac Cyrillic
       77  SQL-CP-1283               PIC S9(4) COMP-5 VALUE 1283.
      * CCSID xF300, (CP856, ST4)
       77  SQL-CP-62208              PIC S9(9) COMP-5 VALUE 62208.
      * CCSID xF301, (CP862, ST4)
       77  SQL-CP-62209              PIC S9(9) COMP-5 VALUE 62209.
      * CCSID xF302, (CP916, ST4)
       77  SQL-CP-62210              PIC S9(9) COMP-5 VALUE 62210.
      * CCSID xF305, (CP862, ST5)
       77  SQL-CP-62213              PIC S9(9) COMP-5 VALUE 62213.
      * CCSID xF30C, (CP856, ST6)
       77  SQL-CP-62220              PIC S9(9) COMP-5 VALUE 62220.
      * CCSID xF30D, (CP862, ST6)
       77  SQL-CP-62221              PIC S9(9) COMP-5 VALUE 62221.
      * CCSID xF30E, (CP916, ST6)
       77  SQL-CP-62222              PIC S9(9) COMP-5 VALUE 62222.
      * CCSID xF30F, (CP1255, ST6)
       77  SQL-CP-62223              PIC S9(9) COMP-5 VALUE 62223.
      * CCSID xF311, (CP864, ST6)
       77  SQL-CP-62225              PIC S9(9) COMP-5 VALUE 62225.
      * CCSID xF312, (CP1046, ST6)
       77  SQL-CP-62226              PIC S9(9) COMP-5 VALUE 62226.
      * CCSID xF313, (CP1089, ST6)
       77  SQL-CP-62227              PIC S9(9) COMP-5 VALUE 62227.
      * CCSID xF314, (CP1256, ST6)
       77  SQL-CP-62228              PIC S9(9) COMP-5 VALUE 62228.
      * CCSID xF316, (CP856, ST8)
       77  SQL-CP-62230              PIC S9(9) COMP-5 VALUE 62230.
      * CCSID xF317, (CP862, ST8)
       77  SQL-CP-62231              PIC S9(9) COMP-5 VALUE 62231.
      * CCSID xF318, (CP916, ST8)
       77  SQL-CP-62232              PIC S9(9) COMP-5 VALUE 62232.
      * CCSID xF31C, (CP856, ST10)
       77  SQL-CP-62236              PIC S9(9) COMP-5 VALUE 62236.
      * CCSID xF31E, (CP916, ST10)
       77  SQL-CP-62238              PIC S9(9) COMP-5 VALUE 62238.
      * CCSID xF31F, (CP1255, ST10)
       77  SQL-CP-62239              PIC S9(9) COMP-5 VALUE 62239.
      * CCSID xF321, (CP856, ST11)
       77  SQL-CP-62241              PIC S9(9) COMP-5 VALUE 62241.
      * CCSID xF322, (CP862, ST11)
       77  SQL-CP-62242              PIC S9(9) COMP-5 VALUE 62242.
      * CCSID xF323, (CP916, ST11)
       77  SQL-CP-62243              PIC S9(9) COMP-5 VALUE 62243.
      * CCSID xF324, (CP1255, ST11)
       77  SQL-CP-62244              PIC S9(9) COMP-5 VALUE 62244.
      * CCSID xE000, (Unknown or unsupported)
       77  SQL-CP-UNKNOWN            PIC S9(9) COMP-5 VALUE 57344.
      * CCSID 1162 - Windows Thailand (with Euro)
       77  SQL-CP-1162               PIC S9(4) COMP-5 VALUE 1162.
      * CCSID 5222 - Windows Korea
       77  SQL-CP-5222               PIC S9(4) COMP-5 VALUE 5222.
      * CCSID 5346 - Windows Latin-2 (v2 with Euro)
       77  SQL-CP-5346               PIC S9(4) COMP-5 VALUE 5346.
      * CCSID 5347 - Windows Cyrillic (v2 with Euro)
       77  SQL-CP-5347               PIC S9(4) COMP-5 VALUE 5347.
      * CCSID 5348 - Windows Latin-1 (v2 with Euro)
       77  SQL-CP-5348               PIC S9(4) COMP-5 VALUE 5348.
      * CCSID 5349 - Windows Greece (v2 with Euro)
       77  SQL-CP-5349               PIC S9(4) COMP-5 VALUE 5349.
      * CCSID 5350 - Windows Turkey (v2 with Euro)
       77  SQL-CP-5350               PIC S9(4) COMP-5 VALUE 5350.
      * CCSID 5351 - Windows Hebrew ST5 (v2 with Euro)
       77  SQL-CP-5351               PIC S9(4) COMP-5 VALUE 5351.
      * CCSID 5352 - Windows Arabic ST5 (v2 with Euro)
       77  SQL-CP-5352               PIC S9(4) COMP-5 VALUE 5352.
      * CCSID 5353 - Windows Baltic (v2 with Euro)
       77  SQL-CP-5353               PIC S9(4) COMP-5 VALUE 5353.
      * CCSID 5354 - Windows Vietnam (v2 with Euro)
       77  SQL-CP-5354               PIC S9(4) COMP-5 VALUE 5354.
      * CCSID 62215 - Windows Hebrew ST4
       77  SQL-CP-62215              PIC S9(9) COMP-5 VALUE 62215.
      * CCSID 62237 - Windows Hebrew ST8
       77  SQL-CP-62237              PIC S9(9) COMP-5 VALUE 62237.
      * CCSID 895 - Japan 7-bit Latin
       77  SQL-CP-895                PIC S9(4) COMP-5 VALUE 895.
      * CCSID 901 - Baltic 8-bit (with Euro)
       77  SQL-CP-901                PIC S9(4) COMP-5 VALUE 901.
      * CCSID 902 - Estonia ISO-8 (with Euro)
       77  SQL-CP-902                PIC S9(4) COMP-5 VALUE 902.
      * CCSID 1008 - Arabic 8-bit ISO/ASCII
       77  SQL-CP-1008               PIC S9(4) COMP-5 VALUE 1008.
      * CCSID 1155 - Turkey Latin-5 (with Euro)
       77  SQL-CP-1155               PIC S9(4) COMP-5 VALUE 1155.
      * CCSID 4909 - Greece, Latin ISO-8 (with Euro)
       77  SQL-CP-4909               PIC S9(4) COMP-5 VALUE 4909.
      * CCSID 5104 - Arabic 8-bit ISO/ASCII (with Euro)
       77  SQL-CP-5104               PIC S9(4) COMP-5 VALUE 5104.
      * CCSID 21427 - Taiwan IBM Big-5 (with 13493 CNS, 566, 6204 UDC, Euro)
       77  SQL-CP-21427              PIC S9(9) COMP-5 VALUE 21427.
      * CCSID 62212 - CP867 Hebrew ST10
       77  SQL-CP-62212              PIC S9(9) COMP-5 VALUE 62212.
      * CCSID 62214 - CP867 Hebrew ST5
       77  SQL-CP-62214              PIC S9(9) COMP-5 VALUE 62214.
      * CCSID 62216 - CP867 Hebrew ST6
       77  SQL-CP-62216              PIC S9(9) COMP-5 VALUE 62216.
      * CCSID 62217 - CP867 Hebrew ST8
       77  SQL-CP-62217              PIC S9(9) COMP-5 VALUE 62217.
      * CCSID 62219 - CP867 Hebrew ST11
       77  SQL-CP-62219              PIC S9(9) COMP-5 VALUE 62219.
      * CCSID 62240 - CP856 Hebrew ST11
       77  SQL-CP-62240              PIC S9(9) COMP-5 VALUE 62240.

      * DBCS Codepages
      * Codepage 926 - Korean
       77  SQL-CP-926                PIC S9(4) COMP-5 VALUE 926.
      * Codepage 951 - New Korean
       77  SQL-CP-951                PIC S9(4) COMP-5 VALUE 951.
      * Codepage 301 - Japanese
       77  SQL-CP-301                PIC S9(4) COMP-5 VALUE 301.
      * Codepage 928 - Chinese
       77  SQL-CP-928                PIC S9(4) COMP-5 VALUE 928.
      * Codepage 927 - Taiwan
       77  SQL-CP-927                PIC S9(4) COMP-5 VALUE 927.
      * Codepage 941 - Japanese
       77  SQL-CP-941                PIC S9(4) COMP-5 VALUE 941.
      * Codepage 947 - Taiwan Big-5
       77  SQL-CP-947                PIC S9(4) COMP-5 VALUE 947.
      * Codepage 971 - Korean 5601
       77  SQL-CP-971                PIC S9(4) COMP-5 VALUE 971.
      * Codepage 1351 - Japanese
       77  SQL-CP-1351               PIC S9(4) COMP-5 VALUE 1351.
      * Codepage 1362 - Korean Windows
       77  SQL-CP-1362               PIC S9(4) COMP-5 VALUE 1362.
      * Codepage1380 - China GB
       77  SQL-CP-1380               PIC S9(4) COMP-5 VALUE 1380.
      * Codepage1382 - Simp Chinese GB
       77  SQL-CP-1382               PIC S9(4) COMP-5 VALUE 1382.
      * Codepage1385 - Simp Chinese GBK
       77  SQL-CP-1385               PIC S9(4) COMP-5 VALUE 1385.
      * Codepage 1393 - Japanese
       77  SQL-CP-1393               PIC S9(4) COMP-5 VALUE 1393.

      * Combined Codepages
      * Codepage 891 + 926 - Korean
       77  SQL-CP-934                PIC S9(4) COMP-5 VALUE 934.
      * CP 1088 + 951 - Korean Std
       77  SQL-CP-949                PIC S9(4) COMP-5 VALUE 949.
      * Codepage 897 + 301 - Japanese
       77  SQL-CP-932                PIC S9(4) COMP-5 VALUE 932.
      * Codepage 903 + 928 - Chinese
       77  SQL-CP-936                PIC S9(4) COMP-5 VALUE 936.
      * Codepage 904 + 927 - Taiwan
       77  SQL-CP-938                PIC S9(4) COMP-5 VALUE 938.
      * Codepage 1040 + 926 - Ext.Korean
       77  SQL-CP-944                PIC S9(4) COMP-5 VALUE 944.
      * Codepage 1041 + 301 - Ext.Japanese
       77  SQL-CP-942                PIC S9(4) COMP-5 VALUE 942.
      * Codepage  897 + 941 - Ext.Japanese
       77  SQL-CP-943                PIC S9(4) COMP-5 VALUE 943.
      * Codepage 1042 + 928 - Ext.Chinese
       77  SQL-CP-946                PIC S9(4) COMP-5 VALUE 946.
      * Codepage 1043 + 927 - Ext.Taiwan
       77  SQL-CP-948                PIC S9(4) COMP-5 VALUE 948.
      * Codepage 1114 + 947 - Taiwan Big5
       77  SQL-CP-950                PIC S9(4) COMP-5 VALUE 950.
      * Codepage 954 + 13488 - euc Japan
       77  SQL-CP-954                PIC S9(4) COMP-5 VALUE 954.
      * Codepage 964 + 13488 - euc Taiwan
       77  SQL-CP-964                PIC S9(4) COMP-5 VALUE 964.
      * Codepage  367 + 971 - Korean 5601
       77  SQL-CP-970                PIC S9(4) COMP-5 VALUE 970.
      * Codepage 1363 - Korean Windows
       77  SQL-CP-1363               PIC S9(4) COMP-5 VALUE 1363.
      * Codepage 1115 +1380 - China GB
       77  SQL-CP-1381               PIC S9(4) COMP-5 VALUE 1381.
      * Codepage  367 +1382 - Chinese GB
       77  SQL-CP-1383               PIC S9(4) COMP-5 VALUE 1383.
      * Codepage  1114 +1385 - Chinese GBK
       77  SQL-CP-1386               PIC S9(4) COMP-5 VALUE 1386.
      * Codepage  - Chinese GB18030
       77  SQL-CP-5488               PIC S9(4) COMP-5 VALUE 5488.
      * Codepage  - Chinese GB18030
       77  SQL-CP-1392               PIC S9(4) COMP-5 VALUE 1392.
      * Codepage  - Japanese Shift_JISX0213
       77  SQL-CP-1394               PIC S9(4) COMP-5 VALUE 1394.
      * Codepage  1041 + 1351 - Japanese
       77  SQL-CP-5039               PIC S9(4) COMP-5 VALUE 5039.
      * CCSID x21A4, (CP420, ST5)
       77  SQL-CP-8612               PIC S9(4) COMP-5 VALUE 8612.
      * CCSID xF30A, (CP864, ST8)
       77  SQL-CP-62218              PIC S9(9) COMP-5 VALUE 62218.
      * CCSID xF310, (CP420, ST6)
       77  SQL-CP-62224              PIC S9(9) COMP-5 VALUE 62224.
      * CCSID xF319, (CP420, ST8)
       77  SQL-CP-62233              PIC S9(9) COMP-5 VALUE 62233.
      * CCSID xF31A, (CP1255, ST9)
       77  SQL-CP-62234              PIC S9(9) COMP-5 VALUE 62234.
      * CCSID xF326, (CP1046, ST8)
       77  SQL-CP-62246              PIC S9(9) COMP-5 VALUE 62246.
      * CCSID xF327, (CP1046, ST9)
       77  SQL-CP-62247              PIC S9(9) COMP-5 VALUE 62247.
      * CCSID xF328, (CP1046, ST4)
       77  SQL-CP-62248              PIC S9(9) COMP-5 VALUE 62248.
      * CCSID xF329, (CP1046, ST2)
       77  SQL-CP-62249              PIC S9(9) COMP-5 VALUE 62249.
      * CCSID xF32A, (CP420, ST12)
       77  SQL-CP-62250              PIC S9(9) COMP-5 VALUE 62250.

      * Unicode CCSIDs
      * Codepage1200 - UCS-2 (big-endian)
       77  SQL-CP-1200               PIC S9(4) COMP-5 VALUE 1200.
      * Codepage1202 - UCS-2 (little-endian)
       77  SQL-CP-1202               PIC S9(4) COMP-5 VALUE 1202.
      * Codepage1208 - UTF-8
       77  SQL-CP-1208               PIC S9(4) COMP-5 VALUE 1208.
      * Codepage1232 - UTF-32 (big-endian)
       77  SQL-CP-1232               PIC S9(4) COMP-5 VALUE 1232.
      * Codepage1234 - UTF-32 (little-endian)
       77  SQL-CP-1234               PIC S9(4) COMP-5 VALUE 1234.
      * Codepg13488 - UCS-2 (Unicode v2, big-endian)
       77  SQL-CP-13488              PIC S9(9) COMP-5 VALUE 13488.
      * Codepg13490 - UCS-2 (Unicode v2, little-endiant)
       77  SQL-CP-13490              PIC S9(9) COMP-5 VALUE 13490.
      * Codepg17584 - UCS-2 (Unicode v3, big-endian)
       77  SQL-CP-17584              PIC S9(9) COMP-5 VALUE 17584.
      * Codepg17586 - UCS-2 (Unicode v3, little-endiant)
       77  SQL-CP-17586              PIC S9(9) COMP-5 VALUE 17586.
      * Big-endian UTF-16
       77  SQL-CP-UTF16BE            PIC S9(4) COMP-5 VALUE 1200.
      * Little-endian UTF-16
       77  SQL-CP-UTF16LE            PIC S9(4) COMP-5 VALUE 1202.

      * EBCDIC, PCDATA and ECECP CCSIDs
      * CCSID 37 - EBCDIC - Common European
       77  SQL-CP-37                 PIC S9(4) COMP-5 VALUE 37.
      * CCSID 273 - EBCDIC Austria, Germany
       77  SQL-CP-273                PIC S9(4) COMP-5 VALUE 273.
      * CCSID 274 - EBCDIC Belgium
       77  SQL-CP-274                PIC S9(4) COMP-5 VALUE 274.
      * CCSID 277 - EBCDIC Denmark, Norway
       77  SQL-CP-277                PIC S9(4) COMP-5 VALUE 277.
      * CCSID 278 - EBCDIC Finland, Sweden
       77  SQL-CP-278                PIC S9(4) COMP-5 VALUE 278.
      * CCSID 280 - EBCDIC Italy
       77  SQL-CP-280                PIC S9(4) COMP-5 VALUE 280.
      * CCSID 284 - EBCDIC Spain, Latin America
       77  SQL-CP-284                PIC S9(4) COMP-5 VALUE 284.
      * CCSID 285 - EBCDIC UK
       77  SQL-CP-285                PIC S9(4) COMP-5 VALUE 285.
      * CCSID 290 - EBCDIC Japan
       77  SQL-CP-290                PIC S9(4) COMP-5 VALUE 290.
      * CCSID 297 - EBCDIC France
       77  SQL-CP-297                PIC S9(4) COMP-5 VALUE 297.
      * CCSID 300 - EBCDIC Japan DBCS
       77  SQL-CP-300                PIC S9(4) COMP-5 VALUE 300.
      * CCSID 423 - EBCDIC Greece
       77  SQL-CP-423                PIC S9(4) COMP-5 VALUE 423.
      * CCSID 500 - EBCDIC Latin-1
       77  SQL-CP-500                PIC S9(4) COMP-5 VALUE 500.
      * CCSID 803 - EBCDIC Hebrew Set-A, ST4
       77  SQL-CP-803                PIC S9(4) COMP-5 VALUE 803.
      * CCSID 833 - EBCDIC Korea Extended SBCS
       77  SQL-CP-833                PIC S9(4) COMP-5 VALUE 833.
      * CCSID 834 - EBCDIC Korea DBCS (with 1880 UDC)
       77  SQL-CP-834                PIC S9(4) COMP-5 VALUE 834.
      * CCSID 835 - EBCDIC Taiwan DBCS (with 6204 UDC)
       77  SQL-CP-835                PIC S9(4) COMP-5 VALUE 835.
      * CCSID 836 - EBCDIC China SBCS
       77  SQL-CP-836                PIC S9(4) COMP-5 VALUE 836.
      * CCSID 837 - EBCDIC China DBCS (with 1880 UDC)
       77  SQL-CP-837                PIC S9(4) COMP-5 VALUE 837.
      * CCSID 838 - EBCDIC Thailand Extended SBCS
       77  SQL-CP-838                PIC S9(4) COMP-5 VALUE 838.
      * CCSID 870 - EBCDIC Latin-2
       77  SQL-CP-870                PIC S9(4) COMP-5 VALUE 870.
      * CCSID 871 - EBCDIC Iceland
       77  SQL-CP-871                PIC S9(4) COMP-5 VALUE 871.
      * CCSID 875 - EBCDIC Greece
       77  SQL-CP-875                PIC S9(4) COMP-5 VALUE 875.
      * CCSID 924 - EBCDIC Latin-9
       77  SQL-CP-924                PIC S9(4) COMP-5 VALUE 924.
      * CCSID 930 - EBCDIC Japan mix (with 4370 UDC, Extended SBCS)
       77  SQL-CP-930                PIC S9(4) COMP-5 VALUE 930.
      * CCSID 933 - EBCDIC Korea mix (with 1880 UDC, Extended SBCS)
       77  SQL-CP-933                PIC S9(4) COMP-5 VALUE 933.
      * CCSID 935 - EBCDIC China mix (with 1880 UDC, Extended SBCS)
       77  SQL-CP-935                PIC S9(4) COMP-5 VALUE 935.
      * CCSID 937 - EBCDIC Taiwan mix (with 6204 UDC, Extended SBCS)
       77  SQL-CP-937                PIC S9(4) COMP-5 VALUE 937.
      * CCSID 939 - EBCDIC Japan mix (with 4370 UDC, Extended SBCS)
       77  SQL-CP-939                PIC S9(4) COMP-5 VALUE 939.
      * CCSID 1025 - EBCDIC Cyrillic
       77  SQL-CP-1025               PIC S9(4) COMP-5 VALUE 1025.
      * CCSID 1026 - EBCDIC Turkey Latin-5
       77  SQL-CP-1026               PIC S9(4) COMP-5 VALUE 1026.
      * CCSID 1027 - EBCDIC Japan Extended SBCS
       77  SQL-CP-1027               PIC S9(4) COMP-5 VALUE 1027.
      * CCSID 1047 - EBCDIC Open Systems Latin-1
       77  SQL-CP-1047               PIC S9(4) COMP-5 VALUE 1047.
      * CCSID 1112 - EBCDIC Baltic
       77  SQL-CP-1112               PIC S9(4) COMP-5 VALUE 1112.
      * CCSID 1122 - EBCDIC Estonia
       77  SQL-CP-1122               PIC S9(4) COMP-5 VALUE 1122.
      * CCSID 1123 - EBCDIC Ukraine
       77  SQL-CP-1123               PIC S9(4) COMP-5 VALUE 1123.
      * CCSID 1130 - EBCDIC Vietnam
       77  SQL-CP-1130               PIC S9(4) COMP-5 VALUE 1130.
      * CCSID 1137 - EBCDIC Devangari (Based on Unicode)
       77  SQL-CP-1137               PIC S9(4) COMP-5 VALUE 1137.
      * CCSID 1153 - EBCDIC Latin-2 (with Euro)
       77  SQL-CP-1153               PIC S9(4) COMP-5 VALUE 1153.
      * CCSID 1154 - EBCDIC Cyrillic (with Euro)
       77  SQL-CP-1154               PIC S9(4) COMP-5 VALUE 1154.
      * CCSID 1156 - EBCDIC Baltic
       77  SQL-CP-1156               PIC S9(4) COMP-5 VALUE 1156.
      * CCSID 1157 - EBCDIC Estonia
       77  SQL-CP-1157               PIC S9(4) COMP-5 VALUE 1157.
      * CCSID 1158 - EBCDIC Ukraine
       77  SQL-CP-1158               PIC S9(4) COMP-5 VALUE 1158.
      * CCSID 1159 - EBCDIC Taiwan Extended SBCS (with Euro)
       77  SQL-CP-1159               PIC S9(4) COMP-5 VALUE 1159.
      * CCSID 1160 - EBCDIC Thailanf (with Euro)
       77  SQL-CP-1160               PIC S9(4) COMP-5 VALUE 1160.
      * CCSID 1164 - EBCDIC Vietnam (with Euro)
       77  SQL-CP-1164               PIC S9(4) COMP-5 VALUE 1164.
      * CCSID 1364 - EBCDIC Korea mix (with Full Hangul)
       77  SQL-CP-1364               PIC S9(4) COMP-5 VALUE 1364.
      * CCSID 1371 - EBCDIC Taiwan mix (with 6204 UDC, Extended SBCS)
       77  SQL-CP-1371               PIC S9(4) COMP-5 VALUE 1371.
      * CCSID 1388 - EBCDIC China mix
       77  SQL-CP-1388               PIC S9(4) COMP-5 VALUE 1388.
      * CCSID 1390 - EBCDIC Japan mix (with 6205 UDC, Extended SBCS, Euro)
       77  SQL-CP-1390               PIC S9(4) COMP-5 VALUE 1390.
      * CCSID 1399 - EBCDIC Japan max (with 6205 UDC, Exnteded SBCS, Euro)
       77  SQL-CP-1399               PIC S9(4) COMP-5 VALUE 1399.
      * CCSID 4369 - EBCDIC Austria, German (DP94)
       77  SQL-CP-4369               PIC S9(4) COMP-5 VALUE 4369.
      * CCSID 4396 - EBCDIC Japan DBCS (with 1880 UCD)
       77  SQL-CP-4396               PIC S9(4) COMP-5 VALUE 4396.
      * CCSID 4899 - EBCDIC Hebrew Set-A, ST4 (with Euro, Sheqel)
       77  SQL-CP-4899               PIC S9(4) COMP-5 VALUE 4899.
      * CCSID 4930 - EBCDIC Korea DBCS (with Full Hangul)
       77  SQL-CP-4930               PIC S9(4) COMP-5 VALUE 4930.
      * CCSID 4933 - EBCDIC China DBCS (with all GBK)
       77  SQL-CP-4933               PIC S9(4) COMP-5 VALUE 4933.
      * CCSID 4971 - EBCDIC Greece (with Euro)
       77  SQL-CP-4971               PIC S9(4) COMP-5 VALUE 4971.
      * CCSID 5026 - EBCDIC Japan mix (with 1880 UDC, Extended SBCS)
       77  SQL-CP-5026               PIC S9(4) COMP-5 VALUE 5026.
      * CCSID 5035 - EBCDIC Japan mix (with 1880 UDC, Extended SBCS)
       77  SQL-CP-5035               PIC S9(4) COMP-5 VALUE 5035.
      * CCSID 5123 - EBCDIC Japan Latin (with Extended SBCS, Euro)
       77  SQL-CP-5123               PIC S9(4) COMP-5 VALUE 5123.
      * CCSID 8482 - EBCDIC Japan SBCS (with Euro)
       77  SQL-CP-8482               PIC S9(4) COMP-5 VALUE 8482.
      * CCSID 8616 - EBCDIC Hebrew subset ST10
       77  SQL-CP-8616               PIC S9(4) COMP-5 VALUE 8616.
      * CCSID 9027 - EBCDIC Taiwan (with 6204 UDC, Euro)
       77  SQL-CP-9027               PIC S9(4) COMP-5 VALUE 9027.
      * CCSID 12708 - EBCDIC Arabic ST7
       77  SQL-CP-12708              PIC S9(9) COMP-5 VALUE 12708.
      * CCSID 12712 - EBCDIC Hebrew ST10 (with Euro, Sheqel)
       77  SQL-CP-12712              PIC S9(9) COMP-5 VALUE 12712.
      * CCSID 13121 - EBCDIC Korea (with Extended SBCS)
       77  SQL-CP-13121              PIC S9(9) COMP-5 VALUE 13121.
      * CCSID 13124 - EBCDIC China SBCS
       77  SQL-CP-13124              PIC S9(9) COMP-5 VALUE 13124.
      * CCSID 16684 - EBCDIC Japan (with 6295 UDC, Euro)
       77  SQL-CP-16684              PIC S9(9) COMP-5 VALUE 16684.
      * CCSID 16804 - EBCDIC Arabic ST4 (with Euro)
       77  SQL-CP-16804              PIC S9(9) COMP-5 VALUE 16804.
      * CCSID 28709 - EBCDIC Taiwan SBCS
       77  SQL-CP-28709              PIC S9(9) COMP-5 VALUE 28709.
      * CCSID 62211 - EBCDIC Hebrew ST5
       77  SQL-CP-62211              PIC S9(9) COMP-5 VALUE 62211.
      * CCSID 62229 - EBCDIC Hebrew ST8
       77  SQL-CP-62229              PIC S9(9) COMP-5 VALUE 62229.
      * CCSID 62235 - EBCDIC Hebrew ST6
       77  SQL-CP-62235              PIC S9(9) COMP-5 VALUE 62235.
      * CCSID 62245 - EBCDIC Hebrew ST10
       77  SQL-CP-62245              PIC S9(9) COMP-5 VALUE 62245.
      * CCSID 62251 - zOS Arabic/Latin ST6
       77  SQL-CP-62251              PIC S9(9) COMP-5 VALUE 62251.
      * CCSID 808 - PCDATA Cyrillic (Russian with Euro)
       77  SQL-CP-808                PIC S9(4) COMP-5 VALUE 808.
      * CCSID 848 - PCDATA Uktaine (with Euro)
       77  SQL-CP-848                PIC S9(4) COMP-5 VALUE 848.
      * CCSID 849 - PCDATA Belarus (with Euro)
       77  SQL-CP-849                PIC S9(4) COMP-5 VALUE 849.
      * CCSID 858 - PCDATA Latin-1E (with Euro)
       77  SQL-CP-858                PIC S9(4) COMP-5 VALUE 858.
      * CCSID 872 - PCDATA Cyrillic (with Euro)
       77  SQL-CP-872                PIC S9(4) COMP-5 VALUE 872.
      * CCSID 1161 - PCDATA Thailand (with Euero)
       77  SQL-CP-1161               PIC S9(4) COMP-5 VALUE 1161.
      * CCSID 1370 - PCDATA Taiwan mix (with Euro)
       77  SQL-CP-1370               PIC S9(4) COMP-5 VALUE 1370.
      * CCSID 5210 - PCDATA China SBCS
       77  SQL-CP-5210               PIC S9(4) COMP-5 VALUE 5210.
      * CCSID 9044 - PCDATA Latin-2 (with Euro)
       77  SQL-CP-9044               PIC S9(4) COMP-5 VALUE 9044.
      * CCSID 9048 - PCDATA Hebrew ST5 (with Euro, Sheqel)
       77  SQL-CP-9048               PIC S9(4) COMP-5 VALUE 9048.
      * CCSID 9049 - PCDATA Turkey Latin-5 (with Euro)
       77  SQL-CP-9049               PIC S9(4) COMP-5 VALUE 9049.
      * CCSID 9061 - PCDATA Greece (with Euro)
       77  SQL-CP-9061               PIC S9(4) COMP-5 VALUE 9061.
      * CCSID 9238 - PCDATA Arabic ST5 (with Euro)
       77  SQL-CP-9238               PIC S9(4) COMP-5 VALUE 9238.
      * CCSID 17248 - PCDATA Arabic ST5 (with Euro)
       77  SQL-CP-17248              PIC S9(9) COMP-5 VALUE 17248.
      * CCSID 1140 - ECECP Common European, US, Canada
       77  SQL-CP-1140               PIC S9(4) COMP-5 VALUE 1140.
      * CCSID 1141 - ECECP Austria, Germany
       77  SQL-CP-1141               PIC S9(4) COMP-5 VALUE 1141.
      * CCSID 1142 - ECECP Denmakr, Norway
       77  SQL-CP-1142               PIC S9(4) COMP-5 VALUE 1142.
      * CCSID 1143 - ECECP Finalnd, Sweden
       77  SQL-CP-1143               PIC S9(4) COMP-5 VALUE 1143.
      * CCSID 1144 - ECECP Italy
       77  SQL-CP-1144               PIC S9(4) COMP-5 VALUE 1144.
      * CCSID 1145 - ECECP Spanish, Latin America
       77  SQL-CP-1145               PIC S9(4) COMP-5 VALUE 1145.
      * CCSID 1146 - ECECP UK
       77  SQL-CP-1146               PIC S9(4) COMP-5 VALUE 1146.
      * CCSID 1147 - ECECP France
       77  SQL-CP-1147               PIC S9(4) COMP-5 VALUE 1147.
      * CCSID 1148 - ECECP International 1
       77  SQL-CP-1148               PIC S9(4) COMP-5 VALUE 1148.
      * CCSID 1149 - ECECP Iceland
       77  SQL-CP-1149               PIC S9(4) COMP-5 VALUE 1149.
      * CCSID 65535 - Reserved
       77  SQL-CP-65535              PIC S9(9) COMP-5 VALUE 65535.

      * Datastream Types
      * Single byte PC
       77  SQL-SBC-PC                PIC S9(4) COMP-5 VALUE 0.
      * Japanese-PC
       77  SQL-JPN-PC                PIC S9(4) COMP-5 VALUE 1.
      * Chinese-PC
       77  SQL-CHN-PC                PIC S9(4) COMP-5 VALUE 2.
      * Korean-PC
       77  SQL-KOR-PC                PIC S9(4) COMP-5 VALUE 3.
      * New Korean-PC
       77  SQL-KSC-PC                PIC S9(4) COMP-5 VALUE 4.
      * Korean 5601
       77  SQL-KR5-PC                PIC S9(4) COMP-5 VALUE 5.
      * Taiwan Big-5
       77  SQL-TWN-PC                PIC S9(4) COMP-5 VALUE 6.
      * China GB
       77  SQL-CGB-PC                PIC S9(4) COMP-5 VALUE 7.
      * China GB on AIX
       77  SQL-CGBA-PC               PIC S9(4) COMP-5 VALUE 8.
      * Japan euc
       77  SQL-EUCJP-PC              PIC S9(4) COMP-5 VALUE 9.
      * Taiwan euc
       77  SQL-EUCTW-PC              PIC S9(4) COMP-5 VALUE 10.
      * UCS-2
       77  SQL-UCS2-PC               PIC S9(4) COMP-5 VALUE 11.
      * Korean Windows 
       77  SQL-KSC2-PC               PIC S9(4) COMP-5 VALUE 12.
      * China GBK
       77  SQL-CGBK-PC               PIC S9(4) COMP-5 VALUE 13.
      * UTF-8
       77  SQL-UTF8-PC               PIC S9(4) COMP-5 VALUE 14.
      * China GB18030
       77  SQL-CGB18030-PC           PIC S9(4) COMP-5 VALUE 15.
      * Unknown
       77  SQL-UNKN-PC               PIC S9(4) COMP-5 VALUE 255.

      * AIX codeset & locale lengths
       77  SQL-CODESET-LEN           PIC S9(4) COMP-5 VALUE 9.
       77  SQL-LOCALE-LEN            PIC S9(4) COMP-5 VALUE 5.

      * Codeset & locale lengths for sqle_db_territory_info struct
       77  SQL-CODESET-SIZE          PIC S9(4) COMP-5 VALUE 17.
       77  SQL-LOCALE-SIZE           PIC S9(4) COMP-5 VALUE 33.

      * SQLCA Constants
      * Invalid SQLCA address
       77  SQL-RC-INVALID-SQLCA      PIC S9(4) COMP-5 VALUE -1.

      * Size of SQLCA
       77  SQLCA-SIZE                PIC S9(4) COMP-5 VALUE 136.

      * SQL Error message token delimiter
       77  SQL-ERRMC-PRES            PIC S9(4) COMP-5 VALUE 255.

      * Offset in SQLERRD - Diagnostic information
      * return code
       77  SQL-ERRD-RC               PIC S9(4) COMP-5 VALUE 0.
      * reason code
       77  SQL-ERRD-REAS             PIC S9(4) COMP-5 VALUE 1.
      * nbr rows inserted/updated/deleted
       77  SQL-ERRD-CNT              PIC S9(4) COMP-5 VALUE 2.
      * optimizer estimate of # rows
       77  SQL-ERRD-OPT-CARD         PIC S9(4) COMP-5 VALUE 2.
      * obsolete -- do not use -- 
       77  SQL-ERRD-OPTM             PIC S9(4) COMP-5 VALUE 3.
      * optimzer estimate of total cost
       77  SQL-ERRD-OPT-TOTCOST      PIC S9(4) COMP-5 VALUE 3.
      * nbr of cascaded deletes/updates
       77  SQL-ERRD-DCNT             PIC S9(4) COMP-5 VALUE 4.
      * line number for recompile error
       77  SQL-ERRD-LINE             PIC S9(4) COMP-5 VALUE 4.
      * authentication type returned for CONNECT/ATTACH
       77  SQL-ERRD-AUTHTYPE         PIC S9(4) COMP-5 VALUE 4.
      * diagnostics
       77  SQL-ERRD-DIAG             PIC S9(4) COMP-5 VALUE 5.

      * Indexes in SQLWARN - Warning flags
      * composite - set if any warnings
       77  SQL-WARN-ANY              PIC S9(4) COMP-5 VALUE 0.
      * string column truncated
       77  SQL-WARN-TRUNC            PIC S9(4) COMP-5 VALUE 1.
      * null values eliminated
       77  SQL-WARN-NUL              PIC S9(4) COMP-5 VALUE 2.
      * nbr of columns/host vars mismatch
       77  SQL-WARN-MISM             PIC S9(4) COMP-5 VALUE 3.
      * no WHERE clause in update/delete
       77  SQL-WARN-ALLR             PIC S9(4) COMP-5 VALUE 4.
      * date has been truncated
       77  SQL-WARN-DATE             PIC S9(4) COMP-5 VALUE 6.
      * character conversion substitution
       77  SQL-WARN-SUB              PIC S9(4) COMP-5 VALUE 8.
      * arithmetic error nulls eliminated
       77  SQL-WARN-NUL2             PIC S9(4) COMP-5 VALUE 9.
      * SQLCA conversion error
       77  SQL-WARN-SQLCA            PIC S9(4) COMP-5 VALUE 10.

      * Values for Warning flags in SQLWARN
      * warning indicator
       77  SQL-WARNING               PIC X(1) value "W"
                                     USAGE DISPLAY NATIVE.
      * null terminator truncated warning
       77  SQL-NULL-TRN              PIC X(1) value "N"
                                     USAGE DISPLAY NATIVE.
      * truncation warning with application context length returned in
      * sqlind
       77  SQL-TRN-APP-LEN           PIC X(1) value "X"
                                     USAGE DISPLAY NATIVE.
      * no warning indicator
       77  SQL-NO-WARN               PIC X(1) value " "
                                     USAGE DISPLAY NATIVE.

      * Compiler estimate warning indicator
       77  SQL-PREPARE-ESTIMATE-WARNING PIC X(1) value "P"
                                     USAGE DISPLAY NATIVE.
      * SQLDA Constants

      * Increment for type with null indicator
       77  SQL-TYP-NULINC            PIC S9(4) COMP-5 VALUE 1.

      * Variable Types
      * DATE
       77  SQL-TYP-DATE              PIC S9(4) COMP-5 VALUE 384.
       77  SQL-TYP-NDATE             PIC S9(4) COMP-5 VALUE 385.

      * TIME
       77  SQL-TYP-TIME              PIC S9(4) COMP-5 VALUE 388.
       77  SQL-TYP-NTIME             PIC S9(4) COMP-5 VALUE 389.

      * TIMESTAMP
       77  SQL-TYP-STAMP             PIC S9(4) COMP-5 VALUE 392.
       77  SQL-TYP-NSTAMP            PIC S9(4) COMP-5 VALUE 393.

      * DATALINK
       77  SQL-TYP-DATALINK          PIC S9(4) COMP-5 VALUE 396.
       77  SQL-TYP-NDATALINK         PIC S9(4) COMP-5 VALUE 397.

      * C NUL-terminated graphic str
       77  SQL-TYP-CGSTR             PIC S9(4) COMP-5 VALUE 400.
       77  SQL-TYP-NCGSTR            PIC S9(4) COMP-5 VALUE 401.

      * BLOB - varying length string
       77  SQL-TYP-BLOB              PIC S9(4) COMP-5 VALUE 404.
       77  SQL-TYP-NBLOB             PIC S9(4) COMP-5 VALUE 405.

      * CLOB - varying length string
       77  SQL-TYP-CLOB              PIC S9(4) COMP-5 VALUE 408.
       77  SQL-TYP-NCLOB             PIC S9(4) COMP-5 VALUE 409.

      * DBCLOB - varying length string
       77  SQL-TYP-DBCLOB            PIC S9(4) COMP-5 VALUE 412.
       77  SQL-TYP-NDBCLOB           PIC S9(4) COMP-5 VALUE 413.

      * VARCHAR(i) - varying length string (2 byte length)
       77  SQL-TYP-VARCHAR           PIC S9(4) COMP-5 VALUE 448.
       77  SQL-TYP-NVARCHAR          PIC S9(4) COMP-5 VALUE 449.

      * CHAR(i) - fixed length string
       77  SQL-TYP-CHAR              PIC S9(4) COMP-5 VALUE 452.
       77  SQL-TYP-NCHAR             PIC S9(4) COMP-5 VALUE 453.

      * LONG VARCHAR - varying length string
       77  SQL-TYP-LONG              PIC S9(4) COMP-5 VALUE 456.
       77  SQL-TYP-NLONG             PIC S9(4) COMP-5 VALUE 457.

      * varying length string for C (null terminated)
       77  SQL-TYP-CSTR              PIC S9(4) COMP-5 VALUE 460.
       77  SQL-TYP-NCSTR             PIC S9(4) COMP-5 VALUE 461.

      * VARGRAPHIC(i) - varying length graphic string (2 byte length)
       77  SQL-TYP-VARGRAPH          PIC S9(4) COMP-5 VALUE 464.
       77  SQL-TYP-NVARGRAPH         PIC S9(4) COMP-5 VALUE 465.

      * GRAPHIC(i) - fixed length graphic string
       77  SQL-TYP-GRAPHIC           PIC S9(4) COMP-5 VALUE 468.
       77  SQL-TYP-NGRAPHIC          PIC S9(4) COMP-5 VALUE 469.

      * LONG VARGRAPHIC(i) - varying length graphic string
       77  SQL-TYP-LONGRAPH          PIC S9(4) COMP-5 VALUE 472.
       77  SQL-TYP-NLONGRAPH         PIC S9(4) COMP-5 VALUE 473.

      * varying length string for Pascal (1-byte length)
       77  SQL-TYP-LSTR              PIC S9(4) COMP-5 VALUE 476.
       77  SQL-TYP-NLSTR             PIC S9(4) COMP-5 VALUE 477.

      * FLOAT - 4 or 8 byte floating point
       77  SQL-TYP-FLOAT             PIC S9(4) COMP-5 VALUE 480.
       77  SQL-TYP-NFLOAT            PIC S9(4) COMP-5 VALUE 481.

      * DECIMAL (m,n)
       77  SQL-TYP-DECIMAL           PIC S9(4) COMP-5 VALUE 484.
       77  SQL-TYP-NDECIMAL          PIC S9(4) COMP-5 VALUE 485.

      * Zoned Decimal -> DECIMAL (m,n)
       77  SQL-TYP-ZONED             PIC S9(4) COMP-5 VALUE 488.
       77  SQL-TYP-NZONED            PIC S9(4) COMP-5 VALUE 489.

      * BIGINT - 8-byte signed integer
       77  SQL-TYP-BIGINT            PIC S9(4) COMP-5 VALUE 492.
       77  SQL-TYP-NBIGINT           PIC S9(4) COMP-5 VALUE 493.

      * INTEGER - 4-byte signed integer
       77  SQL-TYP-INTEGER           PIC S9(4) COMP-5 VALUE 496.
       77  SQL-TYP-NINTEGER          PIC S9(4) COMP-5 VALUE 497.

      * SMALLINT - 2-byte signed integer
       77  SQL-TYP-SMALL             PIC S9(4) COMP-5 VALUE 500.
       77  SQL-TYP-NSMALL            PIC S9(4) COMP-5 VALUE 501.

      * NUMERIC -> DECIMAL (m,n)
       77  SQL-TYP-NUMERIC           PIC S9(4) COMP-5 VALUE 504.
       77  SQL-TYP-NNUMERIC          PIC S9(4) COMP-5 VALUE 505.

      * Obsolete Value
       77  SQL-TYP-BLOB-FILE-OBSOLETE PIC S9(4) COMP-5 VALUE 804.
       77  SQL-TYP-NBLOB-FILE-OBSOLETE PIC S9(4) COMP-5 VALUE 805.

      * Obsolete Value
       77  SQL-TYP-CLOB-FILE-OBSOLETE PIC S9(4) COMP-5 VALUE 808.
       77  SQL-TYP-NCLOB-FILE-OBSOLETE PIC S9(4) COMP-5 VALUE 809.

      * Obsolete Value
       77  SQL-TYP-DBCLOB-FILE-OBSOLETE PIC S9(4) COMP-5 VALUE 812.
       77  SQL-TYP-NDBCLOB-FILE-OBSOLETE PIC S9(4) COMP-5 VALUE 813.

      * BLOB File - Binary Large Object File
       77  SQL-TYP-BLOB-FILE         PIC S9(4) COMP-5 VALUE 916.
       77  SQL-TYP-NBLOB-FILE        PIC S9(4) COMP-5 VALUE 917.

      * CLOB File - Char Large Object File
       77  SQL-TYP-CLOB-FILE         PIC S9(4) COMP-5 VALUE 920.
       77  SQL-TYP-NCLOB-FILE        PIC S9(4) COMP-5 VALUE 921.

      * DBCLOB File - Double Byte Char Large Object File
       77  SQL-TYP-DBCLOB-FILE       PIC S9(4) COMP-5 VALUE 924.
       77  SQL-TYP-NDBCLOB-FILE      PIC S9(4) COMP-5 VALUE 925.

      * BLOB locator
       77  SQL-TYP-BLOB-LOCATOR      PIC S9(4) COMP-5 VALUE 960.
       77  SQL-TYP-NBLOB-LOCATOR     PIC S9(4) COMP-5 VALUE 961.

      * CLOB locator
       77  SQL-TYP-CLOB-LOCATOR      PIC S9(4) COMP-5 VALUE 964.
       77  SQL-TYP-NCLOB-LOCATOR     PIC S9(4) COMP-5 VALUE 965.

      * DBCLOB locator
       77  SQL-TYP-DBCLOB-LOCATOR    PIC S9(4) COMP-5 VALUE 968.
       77  SQL-TYP-NDBCLOB-LOCATOR   PIC S9(4) COMP-5 VALUE 969.

       77  SQL-LOBTOKEN-LEN          PIC S9(4) COMP-5 VALUE 4.
       77  SQL-TYP-BLOB-TOKEN        PIC S9(4) COMP-5 VALUE 960.
       77  SQL-TYP-NBLOB-TOKEN       PIC S9(4) COMP-5 VALUE 961.
       77  SQL-TYP-CLOB-TOKEN        PIC S9(4) COMP-5 VALUE 964.
       77  SQL-TYP-NCLOB-TOKEN       PIC S9(4) COMP-5 VALUE 965.
       77  SQL-TYP-DBCLOB-TOKEN      PIC S9(4) COMP-5 VALUE 968.
       77  SQL-TYP-NDBCLOB-TOKEN     PIC S9(4) COMP-5 VALUE 969.
       01  SQL-NCLOB-MAXLEN-INIT PIC X(4) VALUE x"3FFFFFFF"
           USAGE DISPLAY NATIVE.
       01  SQL-NCLOB-MAXLEN REDEFINES
           SQL-NCLOB-MAXLEN-INIT PIC S9(9) COMP-5.
       77  SQL-LOBHANDLE-LEN         PIC S9(4) COMP-5 VALUE 4.
       77  SQL-TYP-BLOB-HANDLE       PIC S9(4) COMP-5 VALUE 960.
       77  SQL-TYP-NBLOB-HANDLE      PIC S9(4) COMP-5 VALUE 961.
       77  SQL-TYP-CLOB-HANDLE       PIC S9(4) COMP-5 VALUE 964.
       77  SQL-TYP-NCLOB-HANDLE      PIC S9(4) COMP-5 VALUE 965.
       77  SQL-TYP-DBCLOB-HANDLE     PIC S9(4) COMP-5 VALUE 968.
       77  SQL-TYP-NDBCLOB-HANDLE    PIC S9(4) COMP-5 VALUE 969.

      * Values for 30th byte of sqlname
       77  SQL-SQLNAME-SYSGEN        PIC S9(4) COMP-5 VALUE 255.
       77  SQL-SQLNAME-NOT-SYSGEN    PIC S9(4) COMP-5 VALUE 0.

      * Return Codes for sqlabndx, sqlaprep and sqlarbnd
      * The option(s) specified are not supported by the target  database
      * and will be ignored
       77  SQLA-RC-OPT-IGNORED       PIC S9(4) COMP-5 VALUE 20.
      * Bind execution succeeded with warnings.
       77  SQLA-RC-BINDWARN          PIC S9(4) COMP-5 VALUE 25.
      * Precompilation succeeded with warnings.
       77  SQLA-RC-PREPWARN          PIC S9(4) COMP-5 VALUE 25.
      * Bind execution failed
       77  SQLA-RC-BINDERROR         PIC S9(4) COMP-5 VALUE -1.
      * Precompilation failed
       77  SQLA-RC-PREPERROR         PIC S9(4) COMP-5 VALUE -1.
      * Invalid bind file
       77  SQLA-RC-BAD-BINDNAME      PIC S9(4) COMP-5 VALUE -2.
      * Invalid database
       77  SQLA-RC-BAD-DBNAME        PIC S9(4) COMP-5 VALUE -3.
      * Invalid message file
       77  SQLA-RC-BAD-MSGNAME       PIC S9(4) COMP-5 VALUE -5.
      * Invalid format
       77  SQLA-RC-BAD-FORMAT        PIC S9(4) COMP-5 VALUE -6.
      * Error opening list file
       77  SQLA-RC-OPEN-ERROR        PIC S9(4) COMP-5 VALUE -31.
      * Error opening message file
       77  SQLA-RC-MFILE-OPEN-ERR    PIC S9(4) COMP-5 VALUE -35.
      * Source file name is invalid
       77  SQLA-RC-FILE-NAME-BAD     PIC S9(4) COMP-5 VALUE -36.
      * Bind file corrupted
       77  SQLA-RC-BAD-BNDFILE       PIC S9(4) COMP-5 VALUE -39.
      * Bind list errors
       77  SQLA-RC-LIST-ERROR        PIC S9(4) COMP-5 VALUE -40.
      * Interrupt
       77  SQLA-RC-INTERRUPT         PIC S9(4) COMP-5 VALUE -94.
      * System error
       77  SQLA-RC-OSERROR           PIC S9(4) COMP-5 VALUE -1086.
      * Prep/Bind already in use
       77  SQLA-RC-PREP-BIND-BUSY    PIC S9(4) COMP-5 VALUE -1392.
      * Invalid parm. length
       77  SQLA-RC-OPTION-LEN-BAD    PIC S9(4) COMP-5 VALUE -4903.
      * Invalid parm. ptr
       77  SQLA-RC-OPTION-PTR-BAD    PIC S9(4) COMP-5 VALUE -4904.
      * Invalid parm. size
       77  SQLA-RC-OPTION-SIZE-BAD   PIC S9(4) COMP-5 VALUE -4905.
      * Invalid parm. data
       77  SQLA-RC-OPTION-DATA-BAD   PIC S9(4) COMP-5 VALUE -4917.
      * Invalid option or option value
       77  SQLA-RC-OPTION-INVALID    PIC S9(9) COMP-5 VALUE -30104.
      * No SDK/6000 license
       77  SQLA-RC-SDK-LICENSE       PIC S9(4) COMP-5 VALUE -8005.

      * Values used for the date/time format parameter of sqlabind() **
      * OBSOLETE **
      * FORMAT = Default for Country Code
       77  SQL-FMT-DEF               PIC X(3) value "DEF"
                                     USAGE DISPLAY NATIVE.
      * FORMAT = USA
       77  SQL-FMT-USA               PIC X(3) value "USA"
                                     USAGE DISPLAY NATIVE.
      * FORMAT = EUR
       77  SQL-FMT-EUR               PIC X(3) value "EUR"
                                     USAGE DISPLAY NATIVE.
      * FORMAT = ISO
       77  SQL-FMT-ISO               PIC X(3) value "ISO"
                                     USAGE DISPLAY NATIVE.
      * FORMAT = JIS
       77  SQL-FMT-JIS               PIC X(3) value "JIS"
                                     USAGE DISPLAY NATIVE.
      * FORMAT = LOCAL
       77  SQL-FMT-LOC               PIC X(3) value "LOC"
                                     USAGE DISPLAY NATIVE.

      * The size of a date/time format buffer
       77  SQL-FMT-LENGTH            PIC S9(4) COMP-5 VALUE 3.

      * Structures used system wide


      * File reference structure for LOBs
       01 SQLFILE.
           05 SQL-NAME-LENGTH        PIC 9(9) COMP-5.
           05 SQL-DATA-LENGTH        PIC 9(9) COMP-5.
           05 SQL-FILE-OPTIONS       PIC 9(9) COMP-5.
           05 SQL-NAME               PIC X(255)
                                     USAGE DISPLAY NATIVE.
           05 FILLER                 PIC X(1)
                                     USAGE DISPLAY NATIVE.

      * Values used for file_options in the sqlfile structure
      * Input file to read from
       77  SQL-FILE-READ             PIC S9(4) COMP-5 VALUE 2.
      * Output file - new file to be created
       77  SQL-FILE-CREATE           PIC S9(4) COMP-5 VALUE 8.
      * Output file - overwrite existing file or create a new file if it
      * doesn't exist
       77  SQL-FILE-OVERWRITE        PIC S9(4) COMP-5 VALUE 16.
      * Output file - append to an existing file or create a new file if it
      * doesn't exist
       77  SQL-FILE-APPEND           PIC S9(4) COMP-5 VALUE 32.

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
             10 SQLOPT-VAL         PIC 9(9) COMP-5.
             10 SQLOPT-VAL-PTR     REDEFINES SQLOPT-VAL
                                   USAGE IS POINTER.

      * Values used for option[n].type in the sqloptions structure
      * of sqlabndx(), sqlaprep() and sqlarbnd().
      * Option for date/time format - bind and precompile option
       77  SQL-DATETIME-OPT          PIC S9(4) COMP-5 VALUE 1.
      * Option for standards level compliance - precompile option only
       77  SQL-STANDARDS-OPT         PIC S9(4) COMP-5 VALUE 2.
      * Option for isolation level - bind and precompile option
       77  SQL-ISO-OPT               PIC S9(4) COMP-5 VALUE 4.
      * Option for record blocking - bind and precompile option
       77  SQL-BLOCK-OPT             PIC S9(4) COMP-5 VALUE 5.
      * Option for granting privileges - bind option only
       77  SQL-GRANT-OPT             PIC S9(4) COMP-5 VALUE 6.
      * Option for the Flagger - precompile option only
       77  SQL-FLAG-OPT              PIC S9(4) COMP-5 VALUE 8.
      * Option for granting privileges to a user - bind option only
       77  SQL-GRANT-USER-OPT        PIC S9(4) COMP-5 VALUE 9.
      * Option for granting privileges to a group - bind option only
       77  SQL-GRANT-GROUP-OPT       PIC S9(4) COMP-5 VALUE 10.
      * Option for adding NULLs to strings - bind option only
       77  SQL-CNULREQD-OPT          PIC S9(4) COMP-5 VALUE 11.
      * Generic option for DRDA servers - bind option only
       77  SQL-GENERIC-OPT           PIC S9(4) COMP-5 VALUE 12.
      * Option for Deferred Prepare - precompile option only
       77  SQL-DEFERRED-PREPARE-OPT  PIC S9(4) COMP-5 VALUE 15.
      * Specifies whether one or multiple connections are allowed
      * similtaneously within a unit of work.
       77  SQL-CONNECT-OPT           PIC S9(4) COMP-5 VALUE 16.
      * Specifies the set of rules used for connection to multiple databases
      * within a single unit of work
       77  SQL-RULES-OPT             PIC S9(4) COMP-5 VALUE 17.
      * Specifies which of multiple databases connected to will be
      * disconnected when a COMMIT or ROLLBACK is issued.
       77  SQL-DISCONNECT-OPT        PIC S9(4) COMP-5 VALUE 18.
      * Specifies what syncpoint option (for example one phase or two phase)
      * will be used
       77  SQL-SYNCPOINT-OPT         PIC S9(4) COMP-5 VALUE 19.
      * Option to create a bind file - precompile option only
       77  SQL-BIND-OPT              PIC S9(4) COMP-5 VALUE 20.
      * Option specifies SAA/non-SAA compatibility - FORTRAN precompile
      * option only
       77  SQL-SAA-OPT               PIC S9(4) COMP-5 VALUE 21.
      * Option to create a package with a specific name - precompile option
      * only
       77  SQL-PKG-OPT               PIC S9(4) COMP-5 VALUE 23.
      * Option to specify SQLDA optimization - precompile option only
       77  SQL-OPTIM-OPT             PIC S9(4) COMP-5 VALUE 24.
      * Option to not create a package or bind file - precompile option only
       77  SQL-SYNTAX-OPT            PIC S9(4) COMP-5 VALUE 25.
      * Indicates under what conditions a package will be be created - bind
      * and precompile option
       77  SQL-SQLERROR-OPT          PIC S9(4) COMP-5 VALUE 25.
      * Option to suppress #line macro generation in modified source file -
      * C precompile option only
       77  SQL-LINEMACRO-OPT         PIC S9(4) COMP-5 VALUE 26.
      * 'No-op' option - ignore this entry in the option array - bind,
      * precompile and rebind option
       77  SQL-NO-OPT                PIC S9(4) COMP-5 VALUE 27.
      * Level of a module - precompile option only
       77  SQL-LEVEL-OPT             PIC S9(4) COMP-5 VALUE 30.
      * Package collection identifier - precompile option only
       77  SQL-COLLECTION-OPT        PIC S9(4) COMP-5 VALUE 31.
      * Package version identifier - precompile and rebind option only
       77  SQL-VERSION-OPT           PIC S9(4) COMP-5 VALUE 32.
      * Package owner authorization identifier - bind and precompile option
       77  SQL-OWNER-OPT             PIC S9(4) COMP-5 VALUE 33.
      * Synonym for owner - bind and precompile option
       77  SQL-SCHEMA-OPT            PIC S9(4) COMP-5 VALUE 33.
      * Authorization identifier that is to be used as a qualifier for
      * unqualified objects - bind and precompile option
       77  SQL-QUALIFIER-OPT         PIC S9(4) COMP-5 VALUE 34.
      * Synonym for qualifier - bind and precompile option
       77  SQL-CATALOG-OPT           PIC S9(4) COMP-5 VALUE 34.
      * Package description - bind and precompile option
       77  SQL-TEXT-OPT              PIC S9(4) COMP-5 VALUE 35.
      * Indicates when object validation occurs - bind and precompile option
       77  SQL-VALIDATE-OPT          PIC S9(4) COMP-5 VALUE 40.
      * Determines whether information will be produced about how the SQL
      * statements in a package will be executed - bind and precompile
      * option
       77  SQL-EXPLAIN-OPT           PIC S9(4) COMP-5 VALUE 41.
      * Indicates whether a package is to be added or replaced - bind and
      * precompile option
       77  SQL-ACTION-OPT            PIC S9(4) COMP-5 VALUE 42.
      * Replaces a specific version of a package - bind and precompile
      * option
       77  SQL-REPLVER-OPT           PIC S9(4) COMP-5 VALUE 44.
      * Indicates whether EXECUTE authorities are to be preserved when a
      * package is replaced - bind and precompile option
       77  SQL-RETAIN-OPT            PIC S9(4) COMP-5 VALUE 45.
      * Indicates whether resources are released at each COMMIT or when the
      * application  terminates - bind and precompile option
       77  SQL-RELEASE-OPT           PIC S9(4) COMP-5 VALUE 46.
      * Specifies whether or not the query is executed using I/O parallelism
      * - bind and precompile option
       77  SQL-DEGREE-OPT            PIC S9(4) COMP-5 VALUE 47.
      * Designates whether an apostrophe or quote will be used as a string
      * delimiter - bind and precompile option
       77  SQL-STRDEL-OPT            PIC S9(4) COMP-5 VALUE 50.
      * Designates whether a period or comma will be used as a decimal point
      * indicator - bind and precompile option
       77  SQL-DECDEL-OPT            PIC S9(4) COMP-5 VALUE 51.
      * Designates default character subtype that is to be used for column
      * definitions in the CREATE and ALTER TABLE SQL statements - bind and
      * precompile option
       77  SQL-CHARSUB-OPT           PIC S9(4) COMP-5 VALUE 55.
      * Designates what CCSID will be used for single byte characters for
      * character column definitions without a specific CCSID clause in the
      * CREATE and ALTER TABLE SQL statements - bind and precompile option
       77  SQL-CCSIDS-OPT            PIC S9(4) COMP-5 VALUE 56.
      * Designates what CCSID will be used for mixed byte characters for
      * character column definitions without a specific CCSID clause in the
      * CREATE and ALTER TABLE SQL statements - bind and precompile option
       77  SQL-CCSIDM-OPT            PIC S9(4) COMP-5 VALUE 57.
      * Designates what CCSID will be used for double byte characters for
      * character column definitions without a specific CCSID clause in the
      * CREATE and ALTER TABLE SQL statements - bind and precompile option
       77  SQL-CCSIDG-OPT            PIC S9(4) COMP-5 VALUE 58.
      * Specifies maximum precision to be used in decimal arithmetic
      * operations - bind and precompile option
       77  SQL-DEC-OPT               PIC S9(4) COMP-5 VALUE 59.
      * Specifies handling of graphic vars. - precompile only
       77  SQL-WCHAR-OPT             PIC S9(4) COMP-5 VALUE 60.
      * Specifies which authorization identifier to use when dynamic SQL in
      * a package is executed - bind and precompile option
       77  SQL-DYNAMICRULES-OPT      PIC S9(4) COMP-5 VALUE 61.
      * Buffers VALUE inserts - bind and precompile option for DB2/PE
      * servers only
       77  SQL-INSERT-OPT            PIC S9(4) COMP-5 VALUE 62.
      * Capture explain snapshot - bind and precompile option
       77  SQL-EXPLSNAP-OPT          PIC S9(4) COMP-5 VALUE 63.
      * Path for user-defined function resolution - bind and precompile
      * option
       77  SQL-FUNCTION-PATH         PIC S9(4) COMP-5 VALUE 64.
      * Disable prepare-time SQL warnings - bind and precompile option
       77  SQL-SQLWARN-OPT           PIC S9(4) COMP-5 VALUE 65.
      * Set query optimization class - bind and precompile option
       77  SQL-QUERYOPT-OPT          PIC S9(4) COMP-5 VALUE 66.
      * Target compiler - precompile option
       77  SQL-TARGET-OPT            PIC S9(4) COMP-5 VALUE 67.
      * Name of precompiler output file - precompile option
       77  SQL-PREP-OUTPUT-OPT       PIC S9(4) COMP-5 VALUE 68.
      * Preprocessor command - precompile option only
       77  SQL-PREPROCESSOR-OPT      PIC S9(4) COMP-5 VALUE 69.
      * Indicates whether function and type resolution should or should not
      * use conservative binding semantics - rebind option only
       77  SQL-RESOLVE-OPT           PIC S9(4) COMP-5 VALUE 70.
      * CLIPKG option - bind option only
       77  SQL-CLIPKG-OPT            PIC S9(4) COMP-5 VALUE 71.
      * FEDERATED option - bind and precompile option
       77  SQL-FEDERATED-OPT         PIC S9(4) COMP-5 VALUE 72.
      * Transform Group - precompile and bind option
       77  SQL-TRANSFORMGROUP-OPT    PIC S9(4) COMP-5 VALUE 73.
      * Option to treat long host variable declarations as errors -
      * precompile option only
       77  SQL-LONGERROR-OPT         PIC S9(4) COMP-5 VALUE 74.
      * DECTYPE Option to convert decimals  precompile option only
       77  SQL-DECTYPE-OPT           PIC S9(4) COMP-5 VALUE 75.
      * Specifies whether dynamic SQL statements are to be kept after commit
      * points - bind and precompile option
       77  SQL-KEEPDYNAMIC-OPT       PIC S9(4) COMP-5 VALUE 76.
      * Specifies what protocol to use when connecting to a remote site that
      * is identified by a three-part name statement - bind and precompile
      * option
       77  SQL-DBPROTOCOL-OPT        PIC S9(4) COMP-5 VALUE 77.
      * Controls whether query optimization hints are used for static SQL -
      * bind and precompile option
       77  SQL-OPTHINT-OPT           PIC S9(4) COMP-5 VALUE 78.
      * Tells whether immediate writes will be done for updates made to
      * group buffer pool dependent pagesets or partitions  - bind and
      * precompile option
       77  SQL-IMMEDWRITE-OPT        PIC S9(4) COMP-5 VALUE 79.
      * Specifies the encoding for all host variables in static statements
      * in the plan or package (bind and precompile option)
       77  SQL-ENCODING-OPT          PIC S9(4) COMP-5 VALUE 80.
      * Specifies which naming option is to be used when accessing DB2 UDB
      * for iSeries data - bind and precompile option
       77  SQL-OS400NAMING-OPT       PIC S9(4) COMP-5 VALUE 81.
      * Specifies which sort sequence table to use on the iSeries system -
      * bind and precompile option
       77  SQL-SORTSEQ-OPT           PIC S9(4) COMP-5 VALUE 82.
      * Specifies whether to have DB2 determine an access path at run time
      * using values for host variables, parameter markers, and special
      * registers - bind and precompile option
       77  SQL-REOPT-OPT             PIC S9(4) COMP-5 VALUE 83.
      * PSM option - bind option
       77  SQL-PSM-OPT               PIC S9(4) COMP-5 VALUE 84.
      * Specifies whether to use immediate ordeferred procedure
      * resolutionprecompile option
       77  SQL-CALL-RES-OPT          PIC S9(4) COMP-5 VALUE 85.
      * Option for date/time format - bind and precompile option
       77  SQL-TIMESTAMP-OPT         PIC S9(4) COMP-5 VALUE 86.
      * Specifies whether static cursors will be treated as read-only if
      * they are ambiguous - precompile and bind option
       77  SQL-STATICREADONLY-OPT    PIC S9(4) COMP-5 VALUE 87.
      * Specifies a two-part name of the form [schemaname.]basename where
      * basename is a character string of up to 128 chars in length used to
      * uniquely identify the optimization profile within a particular
      * schema.  Schemaname is a character string identifier of up to 30
      * bytes used to explicitelyqualify an optimization profile schema.
       77  SQL-OPTPROFILE-OPT        PIC S9(4) COMP-5 VALUE 88.

      * # of PREP/BIND/REBIND options
       77  SQL-NUM-OPTS              PIC S9(4) COMP-5 VALUE 88.

      * Values used for option[n].val when option[n].type is
      * SQL_DATETIME_OPT. These can also be used for the date/time
      * format parameter of sqlabind().

      * FORMAT = Default for Country Code
       77  SQL-DATETIME-DEF          PIC S9(4) COMP-5 VALUE 48.
      * FORMAT = USA
       77  SQL-DATETIME-USA          PIC S9(4) COMP-5 VALUE 49.
      * FORMAT = EUR
       77  SQL-DATETIME-EUR          PIC S9(4) COMP-5 VALUE 50.
      * FORMAT = ISO
       77  SQL-DATETIME-ISO          PIC S9(4) COMP-5 VALUE 51.
      * FORMAT = JIS
       77  SQL-DATETIME-JIS          PIC S9(4) COMP-5 VALUE 52.
      * FORMAT = LOCAL
       77  SQL-DATETIME-LOC          PIC S9(4) COMP-5 VALUE 53.

      * The following constants are here for backwards compatbility with
      * earlier releases.

       77  SQL-FRMT-OPT              PIC S9(4) COMP-5 VALUE 1.
       77  SQL-FMT-0                 PIC S9(4) COMP-5 VALUE 48.
       77  SQL-FMT-1                 PIC S9(4) COMP-5 VALUE 49.
       77  SQL-FMT-2                 PIC S9(4) COMP-5 VALUE 50.
       77  SQL-FMT-3                 PIC S9(4) COMP-5 VALUE 51.
       77  SQL-FMT-4                 PIC S9(4) COMP-5 VALUE 52.
       77  SQL-FMT-5                 PIC S9(4) COMP-5 VALUE 53.

      * Values used for option[n].val when option[n].type is
      * SQL_STANDARDS_OPT.
      * SAA Level 1 Database CPI
       77  SQL-SAA-COMP              PIC S9(4) COMP-5 VALUE 0.
      * MIA
       77  SQL-MIA-COMP              PIC S9(4) COMP-5 VALUE 1.
      * SQL92 Entry
       77  SQL-SQL92E-COMP           PIC S9(4) COMP-5 VALUE 2.

      * Values used for option[n].val when option[n].type is SQL_ISO_OPT
      * Repeatable read level
       77  SQL-REP-READ              PIC S9(4) COMP-5 VALUE 0.
      * Cursor stability level
       77  SQL-CURSOR-STAB           PIC S9(4) COMP-5 VALUE 1.
      * Uncommitted read level
       77  SQL-UNCOM-READ            PIC S9(4) COMP-5 VALUE 2.
      * Read stability level
       77  SQL-READ-STAB             PIC S9(4) COMP-5 VALUE 3.
      * No Commit level      l
       77  SQL-NO-COMMIT             PIC S9(4) COMP-5 VALUE 4.

      * Values used for option[n].val when option[n].type is SQL_BLOCK_OPT
      * Block Unambiguous cursors
       77  SQL-BL-UNAMBIG            PIC S9(4) COMP-5 VALUE 0.
      * Block All cursors
       77  SQL-BL-ALL                PIC S9(4) COMP-5 VALUE 1.
      * Block No cursors
       77  SQL-NO-BL                 PIC S9(4) COMP-5 VALUE 2.

      * Values used for option[n].val when option[n].type is SQL_FLAG_OPT
      * Flagger check against MVS
       77  SQL-MVSDB2V23-SYNTAX      PIC S9(4) COMP-5 VALUE 4.
      * DB2 V2.3, V3.1 or V4.1 SQL
       77  SQL-MVSDB2V31-SYNTAX      PIC S9(4) COMP-5 VALUE 5.
      * syntax
       77  SQL-MVSDB2V41-SYNTAX      PIC S9(4) COMP-5 VALUE 6.
      * FIPS flagger SQL92E syntax
       77  SQL-SQL92E-SYNTAX         PIC S9(4) COMP-5 VALUE 7.

      * Values used for option[n].val when option[n].type is
      * SQL_CNULREQD_OPT
      * C NULL value not required
       77  SQL-CNULREQD-NO           PIC S9(4) COMP-5 VALUE 0.
      * C NULL value required
       77  SQL-CNULREQD-YES          PIC S9(4) COMP-5 VALUE 1.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_GENERIC_OPT
       77  SQL-MAX-GENERIC           PIC S9(4) COMP-5 VALUE 1023.

      * Values used for option[n].val when option[n].type is SQL_SAA_OPT.
      * SQLCA definition not SAA compatible
       77  SQL-SAA-NO                PIC S9(4) COMP-5 VALUE 0.
      * SQLCA definition is SAA compatible
       77  SQL-SAA-YES               PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is SQL_OPTIM_OPT.
      * Do not optimize SQLDA initialization
       77  SQL-DONT-OPTIMIZE         PIC S9(4) COMP-5 VALUE 0.
      * Optimize SQLDA initialization
       77  SQL-OPTIMIZE              PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is SQL_SYNTAX_OPT.
      * Create a package and/or a bind file
       77  SQL-NO-SYNTAX-CHECK       PIC S9(4) COMP-5 VALUE 0.
      * Do not create a package or bind file
       77  SQL-SYNTAX-CHECK          PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is
      * SQL_LINEMACRO_OPT.
      * Do not generate #line macros in modified source file
       77  SQL-NO-LINE-MACROS        PIC S9(4) COMP-5 VALUE 0.
      * Generate #line macros in modified source file
       77  SQL-LINE-MACROS           PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is SQL_WCHAR_OPT.
      * graphic variable not converted
       77  SQL-WCHAR-NOCONVERT       PIC S9(4) COMP-5 VALUE 0.
      * graphic variable converted
       77  SQL-WCHAR-CONVERT         PIC S9(4) COMP-5 VALUE 1.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_LEVEL_OPT
       77  SQL-MAX-LEVEL             PIC S9(4) COMP-5 VALUE 8.

      * Values used for option[n].val when option[n].type is SQL_CONNECT_OPT
      * Dynamic SQL statements will be chained.
       77  SQL-DEFERRED-PREPARE-YES  PIC S9(4) COMP-5 VALUE 1.
      * Dynamic SQL statements will not be chained.
       77  SQL-DEFERRED-PREPARE-NO   PIC S9(4) COMP-5 VALUE 2.
      * Dynamic SQL statements will be chained in all cases.  The
      * application must not allocate host vars a FETCH SQLDA until after
      * the OPEN statement for the cursor.
       77  SQL-DEFERRED-PREPARE-ALL  PIC S9(4) COMP-5 VALUE 3.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_COLLECTION_OPT
       77  SQL-MAX-COLLECTION        PIC S9(4) COMP-5 VALUE 30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_VERSION_OPT
       77  SQL-MAX-VERSION           PIC S9(4) COMP-5 VALUE 254.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_OWNER_OPT
       77  SQL-MAX-OWNER             PIC S9(4) COMP-5 VALUE 30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_SCHEMA_OPT
       77  SQL-MAX-SCHEMA            PIC S9(4) COMP-5 VALUE 30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_QUALIFIER_OPT
       77  SQL-MAX-QUALIFIER         PIC S9(4) COMP-5 VALUE 30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_CATALOG_OPT
       77  SQL-MAX-CATALOG           PIC S9(4) COMP-5 VALUE 30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_TEXT_OPT
       77  SQL-MAX-TEXT              PIC S9(4) COMP-5 VALUE 255.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_PREPROCESSOR_OPT
       77  SQL-MAX-PREPROCESSOR      PIC S9(4) COMP-5 VALUE 1024.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_TRANSFORMGROUP_OPT
       77  SQL-MAX-TRANSFORMGROUP    PIC S9(4) COMP-5 VALUE 18.

      * Values used for option[n].val when option[n].type is
      * SQL_VALIDATE_OPT
      * Validate objects during BIND
       77  SQL-VALIDATE-BIND         PIC S9(4) COMP-5 VALUE 0.
      * Validate objects during execution
       77  SQL-VALIDATE-RUN          PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is SQL_EXPLAIN_OPT
      * No Explain output saved
       77  SQL-EXPLAIN-NO            PIC S9(4) COMP-5 VALUE 0.
      * Explain output saved
       77  SQL-EXPLAIN-YES           PIC S9(4) COMP-5 VALUE 1.
      * Explain output saved for all static and dynamic statements
       77  SQL-EXPLAIN-ALL           PIC S9(4) COMP-5 VALUE 2.
      * Explain output saved for static reoptimizable statements
       77  SQL-EXPLAIN-REOPT         PIC S9(4) COMP-5 VALUE 3.

      * Values used for option[n].val when option[n].type is SQL_ACTION_OPT
      * Package is to be added
       77  SQL-ACTION-ADD            PIC S9(4) COMP-5 VALUE 0.
      * Package is to be replaced
       77  SQL-ACTION-REPLACE        PIC S9(4) COMP-5 VALUE 1.

      * Max/Min value of CLIPKG for option[n].val when option[n].type is
      * SQL_CLIPKG_OPT
       77  SQL-MIN-CLIPKG            PIC S9(4) COMP-5 VALUE 3.
       77  SQL-MAX-CLIPKG            PIC S9(4) COMP-5 VALUE 30.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_REPLVER_OPT
       77  SQL-MAX-REPLVER           PIC S9(4) COMP-5 VALUE 254.

      * Values used for option[n].val when option[n].type is
      * SQL_SQLERROR_OPT
      * Do not create a package if errors are encountered
       77  SQL-SQLERROR-NOPACKAGE    PIC S9(4) COMP-5 VALUE 0.
      * Do not create a package
       77  SQL-SQLERROR-CHECK        PIC S9(4) COMP-5 VALUE 1.
      * Create a package even if errors are encountered
       77  SQL-SQLERROR-CONTINUE     PIC S9(4) COMP-5 VALUE 2.

      * Values used for option[n].val when option[n].type is SQL_RETAIN_OPT
      * Do not preserve EXECUTE authorities when a package is replaced
       77  SQL-RETAIN-NO             PIC S9(4) COMP-5 VALUE 0.
      * Preserve EXECUTE authorities when a package is replaced
       77  SQL-RETAIN-YES            PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is SQL_RELEASE_OPT
      * Release resources at COMMIT
       77  SQL-RELEASE-COMMIT        PIC S9(4) COMP-5 VALUE 0.
      * Release resources when the program terminates
       77  SQL-RELEASE-DEALLOCATE    PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is SQL_STRDEL_OPT
      * Apostrophe string delimiter
       77  SQL-STRDEL-APOSTROPHE     PIC S9(4) COMP-5 VALUE 0.
      * Quote string delimiter
       77  SQL-STRDEL-QUOTE          PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is SQL_DECDEL_OPT
      * Period is used as a decimal point indicator in decimal and floating
      * point literals
       77  SQL-DECDEL-PERIOD         PIC S9(4) COMP-5 VALUE 0.
      * Comma is used as a decimal point indicator in decimal and floating
      * point literals
       77  SQL-DECDEL-COMMA          PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is SQL_CHARSUB_OPT
      * Use the target system defined default for all new character columns
      * for which an explicit subtype is not specified
       77  SQL-CHARSUB-DEFAULT       PIC S9(4) COMP-5 VALUE 0.
      * Use the BIT character subtype for all new character columns for
      * which an explicit subtype is not specified
       77  SQL-CHARSUB-BIT           PIC S9(4) COMP-5 VALUE 1.
      * Use the SBCS character subtype for all new character columns for
      * which an explicit subtype is not specified
       77  SQL-CHARSUB-SBCS          PIC S9(4) COMP-5 VALUE 2.
      * Use the mixed character subtype for all new character columns for
      * which an explicit subtype is not specified
       77  SQL-CHARSUB-MIXED         PIC S9(4) COMP-5 VALUE 3.

      * Values used for option[n].val when option[n].type is SQL_DEC_OPT
      * 15 bit precision is used in decimal arithmetic operations
       77  SQL-DEC-15                PIC S9(4) COMP-5 VALUE 15.
      * 31 bit precision is used in decimal arithmetic operations
       77  SQL-DEC-31                PIC S9(4) COMP-5 VALUE 31.

      * Values used for option[n].val when option[n].type is SQL_DEGREE_OPT
      * Prohibits parallel I/O operations
       77  SQL-DEGREE-1              PIC S9(4) COMP-5 VALUE 1.
      * Allows the target database system to determine the degree of
      * parallel I/O operations
       77  SQL-DEGREE-ANY            PIC S9(4) COMP-5 VALUE 0.
      * Maximum value
       77  SQL-MAX-DEGREE-VAL        PIC S9(9) COMP-5 VALUE 32767.

      * Values used for option[n].val when option[n].type is SQL_VERSION_OPT
      * Use the timestamp to generate the package VERSION
       77  SQL-VERSION-AUTO          PIC X(4) value "AUTO"
                                     USAGE DISPLAY NATIVE.

      * The next four option values (for CONNECT type, SQLRULES, DISCONNECT
      * and SYNCPOINT) are used not only by the precompiler but also by the
      * sqlesetc and sqleqryc APIs.

      * Values used for option[n].val when option[n].type is SQL_CONNECT_OPT
      * Indicates that only one connection to a database can exist at any
      * given time.
       77  SQL-CONNECT-1             PIC S9(4) COMP-5 VALUE 1.
      * Indicates that multiple connections can exist simultaneously, with
      * one being active and the others dormant.
       77  SQL-CONNECT-2             PIC S9(4) COMP-5 VALUE 2.

      * Values used for option[n].val when option[n].type is SQL_RULES_OPT
      * Indicates that CONNECT TO can be used to make a dormant connection
      * the current connection.
       77  SQL-RULES-DB2             PIC S9(4) COMP-5 VALUE 1.
      * Indicates that CONNECT TO is not valid for making a dormant
      * connection current, and SET CONNECTION must be used instead.
       77  SQL-RULES-STD             PIC S9(4) COMP-5 VALUE 2.

      * Values used for option[n].val when option[n].type is
      * SQL_DISCONNECT_OPT
      * Indicates that all connections marked by the RELEASE statement will
      * be the only connections released when a COMMIT is issued.
       77  SQL-DISCONNECT-EXPL       PIC S9(4) COMP-5 VALUE 1.
      * Indicates that all connections that do not have open WITH HOLD
      * cursors will be the only connections released when a COMMIT is
      * issued.
       77  SQL-DISCONNECT-COND       PIC S9(4) COMP-5 VALUE 2.
      * Indicates that all connections will be released when a COMMIT is
      * issued.
       77  SQL-DISCONNECT-AUTO       PIC S9(4) COMP-5 VALUE 3.

      * Values used for option[n].val when option[n].type is
      * SQL_SYNCPOINT_OPT
      * Do not use a transaction manager to perform two phase commit, but
      * enforce that there is only one database is updated when multiple
      * databases are accessed within a single transaction.
       77  SQL-SYNC-ONEPHASE         PIC S9(4) COMP-5 VALUE 1.
      * Use a transaction manager to coordinate two phase commit.
       77  SQL-SYNC-TWOPHASE         PIC S9(4) COMP-5 VALUE 2.
      * No update enforcement or two phase commit protocol will be used.
       77  SQL-SYNC-NONE             PIC S9(4) COMP-5 VALUE 0.

      * The next option value (for SQL_CONNECT_NODE) is used only by the
      * sqlesetc and sqleqryc APIs.
      * Connect to the catalog node
       77  SQL-CONN-CATALOG-NODE     PIC S9(9) COMP-5 VALUE 65534.

      * Values used for option[n].val when option[n].type is
      * SQL_DYNAMICRULES_OPT
      * Dynamic SQL in package will use authid of person running the package
       77  SQL-DYNAMICRULES-RUN      PIC S9(4) COMP-5 VALUE 0.
      * Dynamic SQL in package will use authid of person who owns the
      * package
       77  SQL-DYNAMICRULES-BIND     PIC S9(4) COMP-5 VALUE 1.
      * Dynamic SQL in a routine will use authid of invoker of routine
       77  SQL-DYNAMICRULES-INVOKERUN PIC S9(4) COMP-5 VALUE 2.
      * Dynamic SQL in a routine will use authid of definer of routine
       77  SQL-DYNAMICRULES-DEFINERUN PIC S9(4) COMP-5 VALUE 3.
      * Dynamic SQL in a routine will use authid of invoker of routine
       77  SQL-DYNAMICRULES-INVOKEBIND PIC S9(4) COMP-5 VALUE 4.
      * Dynamic SQL in a routine will use authid of definer of routine
       77  SQL-DYNAMICRULES-DEFINEBIND PIC S9(4) COMP-5 VALUE 5.

      * Values used for option[n].val when option[n].type is SQL_INSERT_OPT
      * Do not buffer VALUE inserts
       77  SQL-INSERT-DEF            PIC S9(4) COMP-5 VALUE 0.
      * Buffer VALUE inserts
       77  SQL-INSERT-BUF            PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is
      * SQL_EXPLSNAP_OPT
      * No Explain snapshot saved
       77  SQL-EXPLSNAP-NO           PIC S9(4) COMP-5 VALUE 0.
      * Explain snapshot saved
       77  SQL-EXPLSNAP-YES          PIC S9(4) COMP-5 VALUE 1.
      * Explain snapshot saved for all static and dynamic statements
       77  SQL-EXPLSNAP-ALL          PIC S9(4) COMP-5 VALUE 2.
      * Explain snapshot saved for static reoptimizable statements
       77  SQL-EXPLSNAP-REOPT        PIC S9(4) COMP-5 VALUE 3.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_FUNCTION_PATH
       77  SQL-MAX-FUNCPATH          PIC S9(4) COMP-5 VALUE 254.

      * Values used for option[n].val when option[n].type is SQL_SQLWARN_OPT
      * Suppress prepare-time warning SQLCODEs
       77  SQL-SQLWARN-NO            PIC S9(4) COMP-5 VALUE 0.
      * Permit prepare-time warning SQLCODEs
       77  SQL-SQLWARN-YES           PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is
      * SQL_QUERYOPT_OPT
      * Class 0 query optimization
       77  SQL-QUERYOPT-0            PIC S9(4) COMP-5 VALUE 0.
      * Class 1 query optimization
       77  SQL-QUERYOPT-1            PIC S9(4) COMP-5 VALUE 1.
      * Class 2 query optimization
       77  SQL-QUERYOPT-2            PIC S9(4) COMP-5 VALUE 2.
      * Class 3 query optimization
       77  SQL-QUERYOPT-3            PIC S9(4) COMP-5 VALUE 3.
      * Class 5 query optimization
       77  SQL-QUERYOPT-5            PIC S9(4) COMP-5 VALUE 5.
      * Class 7 query optimization
       77  SQL-QUERYOPT-7            PIC S9(4) COMP-5 VALUE 7.
      * Class 9 query optimization
       77  SQL-QUERYOPT-9            PIC S9(4) COMP-5 VALUE 9.

      * Maximum sqlchar length for option[n].val when option[n].type is
      * SQL_TARGET_OPT
       77  SQL-MAX-TARGET-LEN        PIC S9(4) COMP-5 VALUE 32.

      * Values used for option[n].val when option[n].type is SQL_RESOLVE_OPT
      * Conservative binding semantics are not used
       77  SQL-RESOLVE-ANY           PIC S9(4) COMP-5 VALUE 0.
      * Conservative binding semantics are used
       77  SQL-RESOLVE-CONSERVATIVE  PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is
      * SQL_FEDERATED_OPT
      * Federated systems are not supported
       77  SQL-FEDERATED-NO          PIC S9(4) COMP-5 VALUE 0.
      * Federated systems are supported
       77  SQL-FEDERATED-YES         PIC S9(4) COMP-5 VALUE 1.
      * Values used for option[n].val when option[n].type is SQL_PSM_OPT
      * PSM no
       77  SQL-PSM-NO                PIC S9(4) COMP-5 VALUE 0.
      * PSM yes
       77  SQL-PSM-YES               PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is
      * SQL_LONGERROR_OPT.
      * Do not generate errors for the use of long host variable
      * declarations
       77  SQL-LONGERROR-NO          PIC S9(4) COMP-5 VALUE 0.
      * Generate errors for the use of long host variable declarations
       77  SQL-LONGERROR-YES         PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is
      * SQL_DECTYPE_OPT.
      * Decimal type not converted 
       77  SQL-DECTYPE-NOCONVERT     PIC S9(4) COMP-5 VALUE 0.
      * Decimal type converted 
       77  SQL-DECTYPE-CONVERT       PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is
      * SQL_KEEPDYNAMIC_OPT
      * Do not keep dynamic SQL statements after commit points
       77  SQL-KEEPDYNAMIC-NO        PIC S9(4) COMP-5 VALUE 0.
      * Keep dynamic SQL statements after commit points
       77  SQL-KEEPDYNAMIC-YES       PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is
      * SQL_DBPROTOCOL_OPT
      * Use DRDA protocol when connecting to a remote site that is
      * identified by a three-part name statement
       77  SQL-DBPROTOCOL-DRDA       PIC S9(4) COMP-5 VALUE 0.
      * Use a private protocol when connecting to a remote site that is
      * identified by a three-part name statement
       77  SQL-DBPROTOCOL-PRIVATE    PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is
      * SQL_IMMEDWRITE_OPT
      * Updated pages are written at or before phase two of commit
       77  SQL-IMMEDWRITE-NO         PIC S9(4) COMP-5 VALUE 0.
      * Updated pages are written as soon as the buffer update completes
       77  SQL-IMMEDWRITE-YES        PIC S9(4) COMP-5 VALUE 1.
      * Updated pages are written at or before phase one of commit
       77  SQL-IMMEDWRITE-PH1        PIC S9(4) COMP-5 VALUE 2.

      * Values used for option[n].val when option[n].type is
      * SQL_ENCODING_OPT
      * Host variables in static statements are encoded in ascii
       77  SQL-ENCODING-ASCII        PIC S9(4) COMP-5 VALUE 0.
      * Host variables in static statements are encoded in ebcdic
       77  SQL-ENCODING-EBCDIC       PIC S9(4) COMP-5 VALUE 1.
      * Host variables in static statements are encoded in unicode
       77  SQL-ENCODING-UNICODE      PIC S9(4) COMP-5 VALUE 2.

      * Values used for option[n].val when option[n].type is
      * SQL_OS400NAMING_OPT
      * Use the iSeries system naming option when accessing DB2 UDB for
      * iSeries data
       77  SQL-OS400NAMING-SYSTEM    PIC S9(4) COMP-5 VALUE 0.
      * Use the SQL naming option when accessing DB2 UDB for iSeries data
       77  SQL-OS400NAMING-SQL       PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is SQL_SORTSEQ_OPT
      * Use the NLSS sort sequence table of the DRDA job on the iSeries
      * system
       77  SQL-SORTSEQ-JOBRUN        PIC S9(4) COMP-5 VALUE 0.
      * Use the EBCDIC sort sequence table of the DRDA job on the iSeries
      * system
       77  SQL-SORTSEQ-HEX           PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is SQL_REOPT_OPT
      * Do not determine an access path at run time using values for host
      * variables, parameter markers, and special registers
       77  SQL-REOPT-NO              PIC S9(4) COMP-5 VALUE 0.
      * Re-determine an access path at run time using values for host
      * variables, parameter markers, and special registers
       77  SQL-REOPT-YES             PIC S9(4) COMP-5 VALUE 1.
      * The access path is determined based on the default estimates for
      * host variables, parameter markers, and special registers
       77  SQL-REOPT-NONE            PIC S9(4) COMP-5 VALUE 2.
      * Re-determine an access path only once at run time using values for
      * host variables, parameter markers, and special registers
       77  SQL-REOPT-ONCE            PIC S9(4) COMP-5 VALUE 3.
      * Re-determine an access path at every execution values for host
      * variables, parameter markers, and special registers
       77  SQL-REOPT-ALWAYS          PIC S9(4) COMP-5 VALUE 4.

      * Values used for option[n].val when option[n].type is
      * SQL_CALL_RES_OPT
      * Immediate SP call resolution
       77  SQL-CALL-RES-IMMED        PIC S9(4) COMP-5 VALUE 0.
      * Deferred SP call resolution
       77  SQL-CALL-RES-DEFERRED     PIC S9(4) COMP-5 VALUE 1.

      * Values used for option[n].val when option[n].type is
      * SQL_STATICREADONLY_OPT
      * Static cursors take on attributes as wouldnormally be generated
      * given the statementtext and setting of the LANGLEVEL option.
       77  SQL-STATICRO-NO           PIC S9(4) COMP-5 VALUE 0.
      * Any static cursor that does not contain theFOR UPDATE or FOR READ
      * ONLY clause will beconsidered READ ONLY.
       77  SQL-STATICRO-YES          PIC S9(4) COMP-5 VALUE 1.

      * Binder Interface Parameters/Return Codes


      * serial access
       77  SQL-CTX-ORIGINAL          PIC S9(4) COMP-5 VALUE 0.
      * concurrent access
       77  SQL-CTX-MULTI-MANUAL      PIC S9(4) COMP-5 VALUE 1.
      * trusted routine (internal)
       77  SQL-CTX-TRUSTED-ROUTINE   PIC S9(4) COMP-5 VALUE 2.
      * create & attach
       77  SQL-CTX-BEGIN-ALL         PIC S9(4) COMP-5 VALUE 0.
      * create only
       77  SQL-CTX-CREATE-ONLY       PIC S9(4) COMP-5 VALUE 1.
      * detach & free
       77  SQL-CTX-END-ALL           PIC S9(4) COMP-5 VALUE 0.
      * free only
       77  SQL-CTX-FREE-ONLY         PIC S9(4) COMP-5 VALUE 1.
      * Return Codes for sqlaintp/sqlogstt
      * insufficient memory for msg file
       77  SQLA-ERR-BADCC            PIC S9(4) COMP-5 VALUE -1.
      * no error code in SQLCA
       77  SQLA-ERR-NOCOD            PIC S9(4) COMP-5 VALUE -2.
      * message file inaccessible or incorrect
       77  SQLA-ERR-NOMSG            PIC S9(4) COMP-5 VALUE -3.
      * specified line length negative
       77  SQLA-ERR-BADLL            PIC S9(4) COMP-5 VALUE -4.
      * invalid sqlca/buffer addr/length
       77  SQLA-ERR-BADCA            PIC S9(4) COMP-5 VALUE -5.

      * Administrative/Database Authorizations returned from Get
      * Administrative Authorizations function
      * Authorizations granted explicitly to user
      * SYSADM Authority
       77  SQL-SYSADM                PIC S9(4) COMP-5 VALUE 1.
      * DBADM Authority
       77  SQL-DBADM                 PIC S9(4) COMP-5 VALUE 2.
      * CREATETAB Privilege
       77  SQL-CREATETAB             PIC S9(4) COMP-5 VALUE 4.
      * BINDADD Privilege
       77  SQL-BINDADD               PIC S9(4) COMP-5 VALUE 8.
      * CONNECT Privilege
       77  SQL-CONNECT               PIC S9(4) COMP-5 VALUE 16.
      * CREATE_NOT_FENCED Privilege
       77  SQL-CREATE-NOT-FENC       PIC S9(4) COMP-5 VALUE 32.
      * SYSCTRL Authority
       77  SQL-SYSCTRL               PIC S9(4) COMP-5 VALUE 64.
      * SYSMAINT Authority
       77  SQL-SYSMAINT              PIC S9(4) COMP-5 VALUE 128.
      * IMPLICIT_SCHEMA Privilege
       77  SQL-IMPLICIT-SCHEMA       PIC S9(9) COMP-5 VALUE 65536.
      * LOAD Privilege
       77  SQL-LOAD                  PIC S9(9) COMP-5 VALUE 131072.
      * CREATE_EXTERNAL_ROUTINE Privilege
       77  SQL-CREATE-EXT-RT         PIC S9(9) COMP-5 VALUE 262144.
      * LIBRARYADM Privilege
       77  SQL-LIBADM                PIC S9(9) COMP-5 VALUE 524288.
      * QUIESCE_CONNECT Privilege
       77  SQL-QUIESCE-CONN          PIC S9(9) COMP-5 VALUE 1048576.
      * SECURITYADM Privilege
       77  SQL-SECADM                PIC S9(9) COMP-5 VALUE 2097152.
      * SYSQUIESCE Authority
       77  SQL-SYSQUIESCE            PIC S9(9) COMP-5 VALUE 4194304.
      * SYSMON Authority
       77  SQL-SYSMON                PIC S9(9) COMP-5 VALUE 8388608.

      * Composite of authorizations granted explicitly to user, to groups of
      * which user is a member, and to PUBLIC
       77  SQL-SYSADM-GRP            PIC S9(4) COMP-5 VALUE 256.
       77  SQL-DBADM-GRP             PIC S9(4) COMP-5 VALUE 512.
       77  SQL-CREATETAB-GRP         PIC S9(4) COMP-5 VALUE 1024.
       77  SQL-BINDADD-GRP           PIC S9(4) COMP-5 VALUE 2048.
       77  SQL-CONNECT-GRP           PIC S9(4) COMP-5 VALUE 4096.
       77  SQL-CREATE-NOT-FENC-GRP   PIC S9(4) COMP-5 VALUE 8192.
       77  SQL-SYSCTRL-GRP           PIC S9(9) COMP-5 VALUE 16384.
       77  SQL-SYSMAINT-GRP          PIC S9(9) COMP-5 VALUE 32768.
       77  SQL-IMPLICIT-SCHEMA-GRP   PIC S9(9) COMP-5 VALUE 16777216.
       77  SQL-LOAD-GRP              PIC S9(9) COMP-5 VALUE 33554432.
       77  SQL-CREATE-EXT-RT-GRP     PIC S9(9) COMP-5 VALUE 67108864.
       77  SQL-LIBADM-GRP            PIC S9(9) COMP-5 VALUE 134217728.
       77  SQL-QUIESCE-CONN-GRP      PIC S9(9) COMP-5 VALUE 268435456.
       77  SQL-SECADM-GRP            PIC S9(9) COMP-5 VALUE 536870912.
       01  SQL-SYSQUIESCE-GRP-INIT PIC X(4) VALUE x"40000000"
           USAGE DISPLAY NATIVE.
       01  SQL-SYSQUIESCE-GRP REDEFINES
           SQL-SYSQUIESCE-GRP-INIT PIC S9(9) COMP-5.
       01  SQL-SYSMON-GRP-INIT PIC X(4) VALUE x"80000000"
           USAGE DISPLAY NATIVE.
       01  SQL-SYSMON-GRP REDEFINES
           SQL-SYSMON-GRP-INIT PIC S9(9) COMP-5.

      * Table/View Authorizations/Dependencies Bit definitions in
      * SYSTABAUTH.TABAUTH and SYSPLANDEP.TABAUTH
      * Control Authority
       77  SQL-TAB-CTL               PIC S9(4) COMP-5 VALUE 1.
      * Alter Privilege
       77  SQL-TAB-ALT               PIC S9(4) COMP-5 VALUE 2.
      * Delete Privilege/Dependency
       77  SQL-TAB-DEL               PIC S9(4) COMP-5 VALUE 4.
      * Index Privilege
       77  SQL-TAB-IDX               PIC S9(4) COMP-5 VALUE 8.
      * Insert Privilege/Dependency
       77  SQL-TAB-INS               PIC S9(4) COMP-5 VALUE 16.
      * Select Privilege/Dependency
       77  SQL-TAB-SEL               PIC S9(4) COMP-5 VALUE 32.
      * Update Privilege/Dependency
       77  SQL-TAB-UPD               PIC S9(4) COMP-5 VALUE 64.
      * Reference Privilege
       77  SQL-TAB-REF               PIC S9(4) COMP-5 VALUE 128.
      * Key Dependency
       77  SQL-TAB-KEY               PIC S9(4) COMP-5 VALUE 8192.
      * Cascade Dependency
       77  SQL-TAB-CAS               PIC S9(9) COMP-5 VALUE 16384.

      * Bit definitions for SYSTABAUTH.TABAUTH indicating the specified
      * table or view privilege is grantable.
      * Alter Privilege Grantable
       77  SQL-TAB-ALT-G             PIC S9(4) COMP-5 VALUE 512.
      * Delete Privilege Grantable
       77  SQL-TAB-DEL-G             PIC S9(4) COMP-5 VALUE 1024.
      * Index Privilege Grantable
       77  SQL-TAB-IDX-G             PIC S9(4) COMP-5 VALUE 2048.
      * Insert Privilege Grantable
       77  SQL-TAB-INS-G             PIC S9(4) COMP-5 VALUE 4096.
      * Select Privilege Grantable
       77  SQL-TAB-SEL-G             PIC S9(4) COMP-5 VALUE 8192.
      * Update Privilege Grantable
       77  SQL-TAB-UPD-G             PIC S9(9) COMP-5 VALUE 16384.
      * References Privilege Grantable
       77  SQL-TAB-REF-G             PIC S9(9) COMP-5 VALUE 32768.

      * Definitions for application remote interface
      * Unload Progam
       77  SQLZ-DISCONNECT-PROC      PIC S9(4) COMP-5 VALUE 1.
      * Keep program loaded
       77  SQLZ-HOLD-PROC            PIC S9(4) COMP-5 VALUE 2.
      *************************************************************************
      * 
      * The following functions and symbols are obsolete and may not be
      * supported in future releases. The obsolete functions are provided
      * for backward compatibility and exported from DB2API.LIB. All 
      * applications should be migrated to use new APIs.
      **************************************************************************
      * Invalid password
       77  SQLA-RC-BAD-PASSWD        PIC S9(4) COMP-5 VALUE -4.
      * Maximum statements (see SQL reference)
       77  SQL-MAXSTMTS              PIC S9(9) COMP-5 VALUE 32767.
      * Maximum host variables per precompile unit (see SQL reference)
       77  SQL-MAXVARS               PIC S9(9) COMP-5 VALUE 32767.
      * Dynamic SQL in UDF or stored procedure will use authid of invoker of
      * UDF or stored procedure
       77  SQL-DYNAMICRULES-INVOKE   PIC S9(4) COMP-5 VALUE 2.
      * Dynamic SQL in UDF or stored procedure will use authid of definer of
      * UDF or stored procedure
       77  SQL-DYNAMICRULES-DEFINE   PIC S9(4) COMP-5 VALUE 3.
