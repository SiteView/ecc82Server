      *************************************************************************
      * 
      * Source File Name: SQLUTBCQ
      * 
      * (C) COPYRIGHT International Business Machines Corp. 1987, 1995
      * All Rights Reserved
      * Licensed Materials - Property of IBM
      * 
      * US Government Users Restricted Rights - Use, duplication or
      * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
      * 
      * Function: Table space container query
      * 
      * Operating System:  Windows NT
      * 
      **************************************************************************
       01 SQLB-TBSCONTQRY-DATA.
      * container id
           05 SQL-ID                 PIC 9(9) COMP-5.
      * num table spaces sharing this container (is always 1 for DMS
      * tablespaces)
           05 SQL-N-TBS              PIC 9(9) COMP-5.
      * tableSpace id, when 'nTbs' is 1
           05 SQL-TBS-ID             PIC 9(9) COMP-5.
      * length of name (for non-C lang.)
           05 SQL-NAME-LEN           PIC 9(9) COMP-5.
      * null term. name
           05 SQL-NAME               PIC X(256).
      * 1 if container is under the DB directory, 0 if not
           05 SQL-UNDER-DBDIR        PIC 9(9) COMP-5.
      * SQLB_CONT_PATH/DISK/FILE
           05 SQL-CONT-TYPE          PIC 9(9) COMP-5.
      * DMS : Pages specified by CREATE TABLESPACE. SMS : N/A
           05 SQL-TOTAL-PAGES        PIC 9(9) COMP-5.
      * DMS : totalPages - overhead SMS : N/A
           05 SQL-USEABLE-PAGES      PIC 9(9) COMP-5.
      * 1 if container is accessible, 0 if there is a problem
           05 SQL-OK                 PIC 9(9) COMP-5.
