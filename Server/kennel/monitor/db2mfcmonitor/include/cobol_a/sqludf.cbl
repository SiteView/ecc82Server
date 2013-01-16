      *************************************************************************
      * 
      * Source File Name: SQLUDF
      * 
      * (C) COPYRIGHT International Business Machines Corp. 1987, 2003
      * All Rights Reserved
      * Licensed Materials - Property of IBM
      * 
      * US Government Users Restricted Rights - Use, duplication or
      * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
      * 
      * Function = Include File for use in compiling User Defined Function.
      *            Intended to help writers of UDFs deal with the interface
      *            between DBMS and UDF.  Contains:
      *              common structures
      *              "language types" for the SQL types
      *              enumerated type declaration for call-type argument
      *              SQLSTATE constants
      *              macro for prescribed part of the argument list
      *              macro for scratchpad oriented part of argument list
      *             macros to address prescribed parts of argument list
      *             macros to test for argument null and notnull
      * 
      * Operating System: Common Include File
      * 
      * Product Names = DB2 Universal Database for z/OS Version 8
      *                 DB2 Universal Database Version 8
      *                 DB2 Universal Database for iSeries Version 5 Release 3
      * 
      **************************************************************************

      *************************************************************************
      * Structure used for: the scratchpad.
      * Note: This structure as provided by DB2 gives the
      *       overall size/structure of the scratchpad argument.
      *       Individual UDFs which use a scratchpad may wish to
      *       override the "data" variable of this scratchpad
      *       with variables that make sense for the particular
      *       UDF.
      * Note: Remember that the "data" variable is initialized
      *       by DB2 to all \0 before the first call.
      **************************************************************************
       01 SQLUDF-SCRATCHPAD.
      * length of scratchpad data
           05 LENGTH-FIELD           PIC 9(9) COMP-5.
      * scratchpad data, init. to all 
           05 DATA-FIELD             PIC X(100).

      * ASCII
       77  SQLUDF-ASCII              PIC S9(4) COMP-5 VALUE 0.
      * EBCDIC
       77  SQLUDF-EBCDIC             PIC S9(4) COMP-5 VALUE 1.
      * UNICODE
       77  SQLUDF-UNICODE            PIC S9(4) COMP-5 VALUE 2.

      *************************************************************************
      * Operating System "platform" definitions.
      **************************************************************************
      * Unknown platform
       77  SQLUDF-PLATFORM-UNKNOWN   PIC S9(4) COMP-5 VALUE 0.
      * OS/2
       77  SQLUDF-PLATFORM-OS2       PIC S9(4) COMP-5 VALUE 1.
      * Windows
       77  SQLUDF-PLATFORM-WINDOWS   PIC S9(4) COMP-5 VALUE 3.
      * AIX
       77  SQLUDF-PLATFORM-AIX       PIC S9(4) COMP-5 VALUE 4.
      * NT
       77  SQLUDF-PLATFORM-NT        PIC S9(4) COMP-5 VALUE 5.
      * HP
       77  SQLUDF-PLATFORM-HP        PIC S9(4) COMP-5 VALUE 6.
      * Sun
       77  SQLUDF-PLATFORM-SUN       PIC S9(4) COMP-5 VALUE 7.
      * Z/OS
       77  SQLUDF-PLATFORM-ZOS       PIC S9(4) COMP-5 VALUE 8.
      * Siemens Nixdorf
       77  SQLUDF-PLATFORM-SNI       PIC S9(4) COMP-5 VALUE 13.
      * Windows 95
       77  SQLUDF-PLATFORM-WINDOWS95 PIC S9(4) COMP-5 VALUE 15.
      * SCO
       77  SQLUDF-PLATFORM-SCO       PIC S9(4) COMP-5 VALUE 16.
      * Linux
       77  SQLUDF-PLATFORM-LINUX     PIC S9(4) COMP-5 VALUE 18.
      * DYNIX/ptx
       77  SQLUDF-PLATFORM-DYNIX     PIC S9(4) COMP-5 VALUE 19.
      * Linux/390
       77  SQLUDF-PLATFORM-LINUX390  PIC S9(4) COMP-5 VALUE 24.
      * LinuxZ64
       77  SQLUDF-PLATFORM-LINUXZ64  PIC S9(4) COMP-5 VALUE 25.
      * Linux/IA64
       77  SQLUDF-PLATFORM-LINUXIA64 PIC S9(4) COMP-5 VALUE 26.
      * Linux/PPC
       77  SQLUDF-PLATFORM-LINUXPPC  PIC S9(4) COMP-5 VALUE 27.
      * Linux/PPC64
       77  SQLUDF-PLATFORM-LINUXPPC64 PIC S9(4) COMP-5 VALUE 28.
      * Linux/X8664
       77  SQLUDF-PLATFORM-LINUXX8664 PIC S9(4) COMP-5 VALUE 29.
      * HP-PA64
       77  SQLUDF-PLATFORM-HP64      PIC S9(4) COMP-5 VALUE 30.
      * HP-IA
       77  SQLUDF-PLATFORM-HPIA      PIC S9(4) COMP-5 VALUE 31.
      * HP-IA64
       77  SQLUDF-PLATFORM-HPIA64    PIC S9(4) COMP-5 VALUE 32.
      * AS/400
       77  SQLUDF-PLATFORM-ISERIES   PIC S9(4) COMP-5 VALUE 400.

      * iSeries
       77  SQLUDF-PLATFORM-AS400     PIC S9(4) COMP-5 VALUE 400.
      * z/OS
       77  SQLUDF-PLATFORM-MVS       PIC S9(4) COMP-5 VALUE 8.
      * z/OS
       77  SQLUDF-PLATFORM-OS390     PIC S9(4) COMP-5 VALUE 8.

      *************************************************************************
      * Structure used for: dbinfo.
      * Note: This structure as provided by DB2 to the UDF
      *       contains additional information which may be used to
      *       enable (but not require) additional capability or
      *       optimizations on the part of the external function.
      *       This argument is only present if the DBINFO
      *       keyword is specified in the CREATE FUNCTION
      *       statement.
      * 
      * Database version/release.  The field has the format pppvvrrm:
      *   ppp -- identifies the product name
      *   vv  -- two digit rversion identifier such as '03'
      *   rr  -- two digit release identifier such as '01'
      *   m   -- one digit modification level identifier such as '0'
      * 
      * OS Platform information. Please refer to Operating System
      * "platform" definitions above for its definition.
      **************************************************************************
       01 SQLUDF-DBINFO.
      * database name length
           05 DBNAMELEN              PIC 9(4) COMP-5.
      * database name
           05 DBNAME                 PIC X(128).
      * authorization ID length
           05 AUTHIDLEN              PIC 9(4) COMP-5.
      * appl authorization ID
           05 AUTHID                 PIC X(128).
      * database code page
           05 CODEPG.
      * DB2 UDB UWO structure. Deprecated.
               10 CDPG-CS.
      * DB single byte CS
                   15 SBCS           PIC 9(9) COMP-5.
      * DB double byte CS
                   15 DBCS-FIELD     PIC 9(9) COMP-5.
      * DB composite codepage CS
                   15 COMP-FIELD     PIC 9(9) COMP-5.
      * reserved for later use
                   15 DUMMY          PIC X(36).
      * DB2 for z/OS structure. Deprecated.
               10 CDPG-MVS REDEFINES CDPG-CS.
                   15 ASCII-SBCS     PIC 9(9) COMP-5.
                   15 ASCII-DBCS     PIC 9(9) COMP-5.
                   15 ASCII-MIXED    PIC 9(9) COMP-5.
                   15 EBCDIC-SBCD    PIC 9(9) COMP-5.
                   15 EBCDIC-DBCS    PIC 9(9) COMP-5.
                   15 EBCDIC-MIXED   PIC 9(9) COMP-5.
                   15 ENCODING-SCHEME PIC 9(9) COMP-5.
                   15 RESERVED       PIC X(20).
      * DB2 UDB common structure 
               10 CDPG-DB2 REDEFINES CDPG-CS.
      * CCSID tripplet
                   15 DB2-CCSIDS-TRIPLET OCCURS 3 TIMES.
      * DB single byte CS
                       20 DB2-SBCS   PIC 9(9) COMP-5.
      * DB double byte CS
                       20 DBCS-FIELD PIC 9(9) COMP-5.
      * DB composite codepage CS
                       20 COMP-FIELD PIC 9(9) COMP-5.
      * DB2 ccsid encoding schema
                   15  DB2-ENCODING-SCHEME PIC 9(9) COMP-5.
                   15 RESERVED       PIC X(8).
      * table schema name length
           05 TBSCHEMALEN            PIC 9(4) COMP-5.
      * table schema name
           05 TBSCHEMA               PIC X(128).
      * table name length
           05 TBNAMELEN              PIC 9(4) COMP-5.
      * table name
           05 TBNAME                 PIC X(128).
      * column name length
           05 COLNAMELEN             PIC 9(4) COMP-5.
      * column name
           05 COLNAME                PIC X(128).
      * database version/release
           05 VER-REL                PIC X(8).
      * alignment
           05 RESD0                  PIC X(2).
      * platform
           05 PLATFORM               PIC 9(9) COMP-5.
      * number of entries in the TF column list array (tfcolumn, below)
           05 NUMTFCOL               PIC 9(4) COMP-5.
      * Reserved for expansion
           05 RESD1                  PIC X(2).
      * ID of procedure being executed
           05 PROCID                 PIC 9(9) COMP-5.
      * Reserved for expansion
           05 RESD2                  PIC X(32).
      * tfcolumn will be allocated dynamically if TF is defined; otherwise,
      * this will be a null pointer.
           05 TFCOLUMN               USAGE IS POINTER.
      * application identifier
           05 APPL-ID                USAGE IS POINTER.
      * Reserved for expansion
           05 RESD3                  PIC X(20).

