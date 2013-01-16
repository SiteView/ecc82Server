      ***************************************************************************
      *
      *  Source File Name = SQLCA.CBL
      *
      *  (C) Copyright IBM Corp. 1987, 1997
      *  All Rights Reserved
      *  Licensed Material - Program Property of IBM
      *
      *  US Government Users Restricted Rights - Use, duplication or
      *  disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
      *
      *  Function = Copy File defining:
      *               SQLCA
      *
      ***************************************************************************

      *   SQL Communication Area - SQLCA

       01 SQLCA SYNC.
          05 SQLCAID PIC X(8) VALUE "SQLCA   ".
      *                        Eyecatcher = 'SQLCA   '
          05 SQLCABC PIC S9(9) COMP-5 VALUE 136.
      *                        SQLCA size in bytes = 136
          05 SQLCODE PIC S9(9) COMP-5.
      *                        SQL return code
          05 SQLERRM.
      *                        SQL error message
             49 SQLERRML PIC S9(4) COMP-5.
      *                        Length for SQLERRMC
             49 SQLERRMC PIC X(70).
      *                        Error message tokens
          05 SQLERRP PIC X(8).
      *                        Diagnostic information
          05 SQLERRD OCCURS 6 TIMES PIC S9(9) COMP-5.
      *                        Diagnostic information
          05 SQLWARN.
      *                        Warning flags
             10 SQLWARN0 PIC X.
             10 SQLWARN1 PIC X.
             10 SQLWARN2 PIC X.
             10 SQLWARN3 PIC X.
             10 SQLWARN4 PIC X.
             10 SQLWARN5 PIC X.
             10 SQLWARN6 PIC X.
             10 SQLWARN7 PIC X.
             10 SQLWARN8 PIC X.
             10 SQLWARN9 PIC X.
             10 SQLWARNA PIC X.
      *                        Warning flags
          05 SQLSTATE PIC X(5).
      *                        SQLSTATE


