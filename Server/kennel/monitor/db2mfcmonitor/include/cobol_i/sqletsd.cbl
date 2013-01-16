      *************************************************************************
      * 
      * Source File Name: SQLETSD
      * 
      * (C) COPYRIGHT International Business Machines Corp. 1987, 1995
      * All Rights Reserved
      * Licensed Materials - Property of IBM
      * 
      * US Government Users Restricted Rights - Use, duplication or
      * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
      * 
      * Function: Table space descriptor
      * 
      * Operating System:  Windows NT
      * 
      **************************************************************************
       01 SQLETSDESC.
      * eyecatcher/descriptor version
           05 SQLTSDID               PIC X(8)
                                     USAGE DISPLAY NATIVE.
      * extent size (in pages)
           05 SQLEXTNT               PIC S9(9) COMP-5.
      * prefetch size (in pages)
           05 SQLPRFTC               PIC S9(9) COMP-5.
      * i/o overhead
           05 SQLPOVHD               USAGE COMP-2.
      * i/o transfer rate
           05 SQLTRFRT               USAGE COMP-2.
      * tableSpace type
           05 SQLTSTYP               PIC X
                                     USAGE DISPLAY NATIVE.
      * file system caching policy
           05 SQLFSCACHING           PIC X
                                     USAGE DISPLAY NATIVE.
      * container count
           05 SQLCCNT                PIC S9(4) COMP-5.
      * array of container specs
           05 SQL-CONTAINR OCCURS 001 TIMES.
      * container type
               10 SQLCTYPE           PIC X
                                     USAGE DISPLAY NATIVE.
      * reserved
               10 SQL-PAD1           PIC X(3)
                                     USAGE DISPLAY NATIVE.
      * container size (in pages)
               10 SQLCSIZE           PIC S9(9) COMP-5.
      * length of container name
               10 SQLCLEN            PIC S9(4) COMP-5.
      * container name (includes 1 byte for C NULL terminator)
               10 SQLCONTR           PIC X(256)
                                     USAGE DISPLAY NATIVE.
      * 2 bytes padding before next container
               10 SQL-PAD2           PIC X(2)
                                     USAGE DISPLAY NATIVE.
