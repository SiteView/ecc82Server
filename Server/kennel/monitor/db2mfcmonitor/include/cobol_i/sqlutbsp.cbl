      *************************************************************************
      * 
      * Source File Name: SQLUTBSQ
      * 
      * (C) COPYRIGHT International Business Machines Corp. 1987, 1995
      * All Rights Reserved
      * Licensed Materials - Property of IBM
      * 
      * US Government Users Restricted Rights - Use, duplication or
      * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
      * 
      * Function: Table space query
      * 
      * Operating System:  Windows NT
      * 
      **************************************************************************
       01 SQLB-TBSPQRY-DATA.
      * version
           05 SQL-TBSPQVER           PIC X(8)
                                     USAGE DISPLAY NATIVE.
      * internal id for the pool
           05 SQL-ID                 PIC 9(9) COMP-5.
      * length of the name (for cobol & other nonC lang.)
           05 SQL-NAME-LEN           PIC 9(9) COMP-5.
      * NUL terminated name
           05 SQL-NAME               PIC X(128)
                                     USAGE DISPLAY NATIVE.
      * DMS : Pages specified by CREATE TABLESPACE.
      * SMS : N/A
           05 SQL-TOTAL-PAGES        PIC 9(9) COMP-5.
      * DMS : totalPages - overhead
      * SMS : N/A
           05 SQL-USEABLE-PAGES      PIC 9(9) COMP-5.
      * bit attributes for the pool -- includes 'class' & other stuff to be
      * defined
           05 SQL-FLAGS              PIC 9(9) COMP-5.
      * pagesize of tablespace (in bytes) .. one of 4K, 8K, 16K, or 32K
           05 SQL-PAGE-SIZE          PIC 9(9) COMP-5.
      * extent size (in pages)
           05 SQL-EXT-SIZE           PIC 9(9) COMP-5.
      * prefetch size
           05 SQL-PREFETCH-SIZE      PIC 9(9) COMP-5.
      * number of containers
           05 SQL-N-CONTAINERS       PIC 9(9) COMP-5.
      * SQLB_NORMAL, SQLB_LOAD_PENDING, etc.
           05 SQL-TBS-STATE          PIC 9(9) COMP-5.
      * 'timestamp' identifying the origin of the tbs
           05 SQL-LIFE-LSN           PIC X(6)
                                     USAGE DISPLAY NATIVE.
      * for alignment
           05 SQL-PAD                PIC X(2)
                                     USAGE DISPLAY NATIVE.
      * bit attributes for the pool
           05 SQL-FLAGS2             PIC 9(9) COMP-5.
      * Minimum Recovery Time
           05 SQL-MINIMUM-REC-TIME   PIC X(26)
                                     USAGE DISPLAY NATIVE.
           05 FILLER                 PIC X
                                     USAGE DISPLAY NATIVE.
      * for alignment
           05 SQL-PAD1               PIC X(1)
                                     USAGE DISPLAY NATIVE.
      * For Load or Delete Pending
           05 SQL-STATE-CHNG-OBJ     PIC 9(9) COMP-5.
      * For Load or Delete Pending
           05 SQL-STATE-CHNG-ID      PIC 9(9) COMP-5.
      * Number of queiscers
           05 SQL-N-QUIESCERS        PIC 9(9) COMP-5.
      * quiescer info
           05 SQL-QUIESCER OCCURS 5 TIMES.
      * tablespace id of quiesceObject
               10 SQL-QUIESCE-ID     PIC 9(9) COMP-5.
      * object in quiesceId
               10 SQL-QUIESCE-OBJECT PIC 9(9) COMP-5.
      * File system caching policy
           05 SQL-FS-CACHING         PIC X
                                     USAGE DISPLAY NATIVE.
      * for future use
           05 SQL-RESERVED           PIC X(31)
                                     USAGE DISPLAY NATIVE.
