      **************************************************************************
      *
      *  Source File Name = SQLDA.CBL
      *
      *  (C) Copyright IBM Corp. 1987, 1997
      *  All Rights Reserved
      *  Licensed Material - Program Property of IBM
      *
      *  US Government Users Restricted Rights - Use, duplication or
      *  disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
      *
      *  Function = Copy File defining:
      *               SQLDA
      *
      **************************************************************************

      *   SQL Descriptor Area - Variable descriptor


      *                        SQL Descriptor Area - SQLDA
       01 SQLDA SYNC.
          05 SQLDAID PIC X(8) VALUE "SQLDA  " USAGE DISPLAY NATIVE.
      *                        Eye catcher = 'SQLDA   '
          05 SQLDABC PIC S9(9) COMP-5.
      *                        SQLDA size in bytes = 16+44*SQLN
          05 SQLN PIC S9(4) COMP-5.
      *                        Number of SQLVAR elements
          05 SQLD PIC S9(4) COMP-5.
      *                        # of used SQLVAR elements
          05 SQLVAR-ENTRIES OCCURS 0 TO 1489 TIMES
                DEPENDING ON SQLN OF SQLDA.
             10 SQLVAR.
                15 SQLTYPE PIC S9(4) COMP-5.
      *                        Variable data type
                15 SQLLEN PIC S9(4) COMP-5.
      *                        Variable data length
                15 SQLDATA USAGE POINTER.
      *                        Pointer to variable data value
                15 SQLIND USAGE POINTER.
      *                        Pointer to Null indicator
                15 SQLNAME.
      *                        Variable Name
                   49 SQLNAMEL PIC S9(4) COMP-5.
      *                        Name length varies from 1 to 30
                   49 SQLNAMEC PIC X(30) USAGE DISPLAY NATIVE.
      *                        Variable or Column name
             10 SQLVAR2 REDEFINES SQLVAR.
                15 SQLVAR2-RESERVED-1 PIC S9(9) COMP-5.
      *                        Reserved for future use
                15 SQLLONGLEN REDEFINES SQLVAR2-RESERVED-1
                      PIC S9(9) COMP-5.
      *                        LOB variable data length
                15 SQLVAR2-RESERVED-2 PIC X(3).
      *                        Reserved for future use
                15 SQLFLAG4 PIC X(1).
      *                        X'01' for REFERENCE to row type
                15 SQLDATALEN USAGE POINTER.
      *                        Pointer to LOB variable data length
                15 SQLDATATYPE-NAME.
      *                        Variable Name
                   49 SQLDATATYPE-NAMEL PIC S9(4) COMP-5.
      *                        Name length varies from 1 to 27
                   49 SQLDATATYPE-NAMEC PIC X(27) USAGE DISPLAY NATIVE.
      *                        Variable or Column name
                15 FILLER PIC X(3) USAGE DISPLAY NATIVE.

