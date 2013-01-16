#include "stdafx.h"
static char sqla_program_id[162] = 
{
 47,0,65,68,65,74,65,72,85,84,73,76,69,77,66,32,99,65,113,104,
 78,90,67,88,48,49,49,49,49,50,32,32,13,0,65,68,77,73,78,73,
 83,84,82,65,84,79,82,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0
};

#include "sqladef.h"

static struct sqla_runtime_info sqla_rtinfo = 
{{'S','Q','L','A','R','T','I','N'}, sizeof(wchar_t), 0, {' ',' ',' ',' '}};


#line 1 "utilemb.sqc"
/*******************************************************************************
**
** Source File Name = utilemb.sqc 
**
** Licensed Materials - Property of IBM
**
** (C) COPYRIGHT International Business Machines Corp. 1995, 2000
** All Rights Reserved.
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
**
**
**    PURPOSE :
**    - contains various functions, used by most other samples:
**
**    1. SQL_CHECK section
**
**        1.1 - SqlInfoPrint - prints on the screen everything that
**                             goes unexpected.
**        1.2 - TransRollback - rolls back the transaction
**
**     2. non SQL_CHECK section
**
**          2.1 - CmdLineArgsCheck1 - checks the command line arguments, ver.1
**          2.2 - CmdLineArgsCheck2 - checks the command line arguments, ver.2
**          2.3 - EmbAppInit - connects to the database
**          2.4 - EmbAppTerm - disconnects from the database
**
** For more information about these samples see the README file.
**
** For more information on programming in C, see the:
**   -  "Programming in C and C++" section of the Application Development Guide
** For more information on building C applications, see the:
**   -  "Building C Applications" section of the Application Building Guide.
**
** For more information on the SQL language see the SQL Reference.
**
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <sql.h>
#include <sqlenv.h>
#include <sqlda.h>
#include <sqlca.h>
#include <string.h>
#include <ctype.h>
#include "utilemb.h"


/*#############################################################################
**    1. SQL_CHECK section
**
**        1.1 - SqlInfoPrint - prints on the screen everything that
**                             goes unexpected.
**        1.2 - TransRollback - rolls back the transaction
#############################################################################*/

/******************************************************************************
**        1.1 - SqlInfoPrint - prints on the screen everything that
**                             goes unexpected.
******************************************************************************/
int SqlInfoPrint( char *         appMsg, 
                  struct sqlca * pSqlca,
                  int            line,      
                  char *         file )
{   int   rc = 0;

    char  sqlInfo[1024];
    char  sqlInfoToken[1024];

    char  sqlstateMsg[1024];
    char  errorMsg[1024];


    if (pSqlca->sqlcode != 0 && pSqlca->sqlcode != 100)
    {   strcpy(sqlInfo, "");

        if( pSqlca->sqlcode < 0)
        {   sprintf( sqlInfoToken, "\n---- error report ----\n");
            strcat( sqlInfo, sqlInfoToken);
        } 
        else 
        {   sprintf( sqlInfoToken, "\n---- warning report ----\n");
            strcat( sqlInfo, sqlInfoToken);
        } /* endif */

        sprintf( sqlInfoToken, "  app. message      = %s\n", appMsg);
        strcat( sqlInfo, sqlInfoToken);	
        sprintf( sqlInfoToken, "  line              = %d\n", line);
        strcat( sqlInfo, sqlInfoToken);	
        sprintf( sqlInfoToken, "  file              = %s\n", file);
        strcat( sqlInfo, sqlInfoToken);	
        sprintf( sqlInfoToken, "  SQLCODE           = %ld\n", pSqlca->sqlcode);
        strcat( sqlInfo, sqlInfoToken);		
 
        /* get error message */
        rc = sqlaintp( errorMsg, 1024, 80, pSqlca);
        /* return code is the length of the errorMsg string */	
        if( rc > 0)
        {   sprintf( sqlInfoToken, "%s\n", errorMsg);
            strcat( sqlInfo, sqlInfoToken);
        } 
 
        /* get SQLSTATE message */
        rc = sqlogstt( sqlstateMsg, 1024, 80, pSqlca->sqlstate);
        if (rc == 0)
        {   sprintf( sqlInfoToken, "%s\n", sqlstateMsg);
            strcat( sqlInfo, sqlInfoToken);
        }  	


        if( pSqlca->sqlcode < 0)
        {   sprintf( sqlInfoToken, "--- end error report ---\n");
            strcat( sqlInfo, sqlInfoToken);

            printf("%s", sqlInfo);
            return 1;
        } 
        else 
        {   sprintf( sqlInfoToken, "--- end warning report ---\n");
            strcat( sqlInfo, sqlInfoToken);
 
            printf("%s", sqlInfo);
            return 0;
        } /* endif */
    } /* endif */

    return 0;
}

/******************************************************************************
**        1.2 - TransRollback - rolls back the transaction
******************************************************************************/
void TransRollback( )
{   struct sqlca sqlca;
    int        rc = 0;

    /* rollback the transaction */
    printf( "\nRolling back the transaction ...\n") ;
    
/*
EXEC SQL ROLLBACK;
*/

{
#line 141 "utilemb.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 141 "utilemb.sqc"
  sqlacall((unsigned short)28,0,0,0,0L);
#line 141 "utilemb.sqc"
  sqlastop(0L);
}

#line 141 "utilemb.sqc"
 
    rc = SqlInfoPrint( "ROLLBACK", &sqlca, __LINE__, __FILE__);
    if( rc == 0)
    {   printf( "The transaction was rolled back.\n") ;
    }		
    
}



/*#############################################################################
**     2. non SQL_CHECK section
**
**          2.1 - CmdLineArgsCheck1 - checks the command line arguments, ver.1
**          2.2 - CmdLineArgsCheck2 - checks the command line arguments, ver.2
**          2.3 - EmbAppInit - connects to the database
**          2.4 - EmbAppTerm - disconnects from the database
#############################################################################*/

/******************************************************************************
**          2.1 - CmdLineArgsCheck1 - checks the command line arguments, ver.1
******************************************************************************/
int  CmdLineArgsCheck1( int argc,
                        char * argv[],
                        char dbAlias[],
                        char user[],
                        char pswd[] )
{   int rc = 0;

    switch (argc)
    {    case 1:
             strcpy(dbAlias, "sample");
             strcpy(user, "");
             strcpy(pswd, "");
             break;
         case 2:
             strcpy(dbAlias, argv[1]);
             strcpy(user, "");
             strcpy(pswd, "");
             break;
         case 4:
             strcpy(dbAlias, argv[1]);
             strcpy(user, argv[2]);
             strcpy(pswd, argv[3]);
             break;    
         default:
             printf( "\nUSAGE: %s [dbAlias [userid  passwd]]]\n", argv[0] ) ;
             rc = 1;
             break;
    }  /* endswitch */

    return (rc);
}

/******************************************************************************
**          2.2 - CmdLineArgsCheck2 - checks the command line arguments, ver.2
******************************************************************************/
int  CmdLineArgsCheck2( int argc,
                        char * argv[],
                        char dbAlias[],
                        char user[],
                        char pswd[],
                        char remoteNodeName[] )
{   int rc = 0;

    switch (argc)
    {    case 1:
             strcpy(dbAlias, "sample");
             strcpy(user, "");
             strcpy(pswd, "");
             strcpy(remoteNodeName, "");   
             break;
         case 2:
             strcpy(dbAlias, argv[1]);
             strcpy(user, "");
             strcpy(pswd, "");
             strcpy(remoteNodeName, "");   	     
             break;
         case 4:
             strcpy(dbAlias, argv[1]);
             strcpy(user, argv[2]);
             strcpy(pswd, argv[3]);
             strcpy(remoteNodeName, "");   	     
             break;   
         case 5:
             strcpy(dbAlias, argv[1]);
             strcpy(user, argv[2]);
             strcpy(pswd, argv[3]);
             strcpy(remoteNodeName, argv[4]);   	     
             break;   	     
         default:
             printf( "\nUSAGE: %s [dbAlias [userid passwd [remoteNodeName]]]\n",
                     argv[0] ) ;
             rc = 1;
             break;
    }  /* endswitch */

    return (rc);
}

/******************************************************************************
**          2.3 - EmbAppInit - connects to the database
******************************************************************************/
int EmbAppInit( char paramDbAlias[],
                char paramUser[],
                char paramPswd[] )
{   struct sqlca sqlca;
    int        rc = 0;

    
/*
EXEC SQL BEGIN DECLARE SECTION;
*/

#line 250 "utilemb.sqc"

        char dbAlias[15] ;
        char user[15] ; 
        char pswd[15] ; 
    
/*
EXEC SQL END DECLARE SECTION;
*/

#line 254 "utilemb.sqc"


    strcpy( dbAlias, paramDbAlias) ;
    strcpy( user, paramUser) ;    
    strcpy( pswd, paramPswd) ;    

    printf( "\nConnecting to %s ...\n", dbAlias ) ;
    if ( strlen( user) == 0)
    {   
/*
EXEC SQL CONNECT TO :dbAlias;
*/

{
#line 262 "utilemb.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 262 "utilemb.sqc"
  sqlaaloc(2,1,1,0L);
    {
      struct sqla_setd_list sql_setdlist[1];
#line 262 "utilemb.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 15;
#line 262 "utilemb.sqc"
      sql_setdlist[0].sqldata = (void*)dbAlias;
#line 262 "utilemb.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 262 "utilemb.sqc"
      sqlasetda(2,0,1,sql_setdlist,NULL,0L);
    }
#line 262 "utilemb.sqc"
  sqlacall((unsigned short)29,4,2,0,0L);
#line 262 "utilemb.sqc"
  sqlastop(0L);
}

#line 262 "utilemb.sqc"

        EMB_SQL_CHECK( "CONNECT");
    }
    else 
    {   
/*
EXEC SQL CONNECT TO :dbAlias USER :user USING :pswd;
*/

{
#line 266 "utilemb.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 266 "utilemb.sqc"
  sqlaaloc(2,3,2,0L);
    {
      struct sqla_setd_list sql_setdlist[3];
#line 266 "utilemb.sqc"
      sql_setdlist[0].sqltype = 460; sql_setdlist[0].sqllen = 15;
#line 266 "utilemb.sqc"
      sql_setdlist[0].sqldata = (void*)dbAlias;
#line 266 "utilemb.sqc"
      sql_setdlist[0].sqlind = 0L;
#line 266 "utilemb.sqc"
      sql_setdlist[1].sqltype = 460; sql_setdlist[1].sqllen = 15;
#line 266 "utilemb.sqc"
      sql_setdlist[1].sqldata = (void*)user;
#line 266 "utilemb.sqc"
      sql_setdlist[1].sqlind = 0L;
#line 266 "utilemb.sqc"
      sql_setdlist[2].sqltype = 460; sql_setdlist[2].sqllen = 15;
#line 266 "utilemb.sqc"
      sql_setdlist[2].sqldata = (void*)pswd;
#line 266 "utilemb.sqc"
      sql_setdlist[2].sqlind = 0L;
#line 266 "utilemb.sqc"
      sqlasetda(2,0,3,sql_setdlist,NULL,0L);
    }
#line 266 "utilemb.sqc"
  sqlacall((unsigned short)29,5,2,0,0L);
#line 266 "utilemb.sqc"
  sqlastop(0L);
}

#line 266 "utilemb.sqc"

        EMB_SQL_CHECK( "CONNECT");
    }
    printf( "Connected to %s.\n\n", dbAlias ) ;      
 
    return( 0 ) ;
}

/******************************************************************************
**        2.4 - EmbAppTerm - disconnects from the database
******************************************************************************/
int EmbAppTerm( char * dbAlias )
{   struct sqlca sqlca;
    int        rc = 0;
    
    printf( "\nDisconnecting from %s ...\n", dbAlias ) ;
    
/*
EXEC SQL CONNECT RESET ;
*/

{
#line 282 "utilemb.sqc"
  sqlastrt(sqla_program_id, &sqla_rtinfo, &sqlca);
#line 282 "utilemb.sqc"
  sqlacall((unsigned short)29,3,0,0,0L);
#line 282 "utilemb.sqc"
  sqlastop(0L);
}

#line 282 "utilemb.sqc"

    EMB_SQL_CHECK( "CONNECT RESET") ;
    printf( "Disconnected from %s.\n", dbAlias ) ;    

    return( 0 ) ;
}



/* functions/procedures on setting up & outputting info from SQLDA */

/*******************************************************************************
* PROCEDURE : init_da
* The following procedure allocate memory space for the prepared SQL statement
*  to reside in.  The use of an internally described function called
*  SQLDASIZE is used to calculate the proper amount of memory is needed.
*******************************************************************************/
void init_da (struct sqlda **DAPointer, int DAsqln) {
   *DAPointer = (struct sqlda *) malloc (SQLDASIZE(DAsqln));
   if (*DAPointer == NULL) {
      fprintf (stderr, "\ninit_da : out of memory error.\n");
      exit (-1);
   }
   strncpy((*DAPointer)->sqldaid, "SQLDA   ", sizeof ((*DAPointer)->sqldaid));
   (*DAPointer)->sqldabc = (sqlint32)SQLDASIZE(DAsqln);
   (*DAPointer)->sqln = DAsqln;
   (*DAPointer)->sqld = 0;
}



/*******************************************************************************
* FUNCTION : alloc_host_vars
* This function allocates memory to be filled with the data directed from an
*  SQLDA pointer.
*******************************************************************************/
void alloc_host_vars (struct sqlda *sqldaPointer) {
   short idx;
   unsigned int memsize =0;
   long longmemsize =0;
   int precision  =0;

   for (idx = 0; idx < sqldaPointer->sqld; idx++) {
      switch (sqldaPointer->sqlvar[idx].sqltype ) {
      case SQL_TYP_BLOB:
      case SQL_TYP_NBLOB:
      case SQL_TYP_CLOB:
      case SQL_TYP_NCLOB:
      case SQL_TYP_DBCLOB:
      case SQL_TYP_NDBCLOB:
         longmemsize = GETSQLDALONGLEN(sqldaPointer, idx);

#if defined(DB2WIN)
         memsize = (unsigned int) (min(longmemsize, 64000));
         SETSQLDALONGLEN(sqldaPointer, idx, memsize);
#else
         memsize = longmemsize;
#endif

         sqldaPointer->sqlvar[idx].sqldata = (char *SQL_POINTER)
            malloc (memsize);
         break;
      case SQL_TYP_VARCHAR:
      case SQL_TYP_NVARCHAR:
      case SQL_TYP_LONG:
      case SQL_TYP_NLONG:
      case SQL_TYP_DATE:
      case SQL_TYP_NDATE:
      case SQL_TYP_TIME:
      case SQL_TYP_NTIME:
      case SQL_TYP_STAMP:
      case SQL_TYP_NSTAMP:
         sqldaPointer->sqlvar[idx].sqltype = SQL_TYP_NCSTR;
         sqldaPointer->sqlvar[idx].sqldata = (char *SQL_POINTER)
            malloc ((sqldaPointer->sqlvar[idx].sqllen));
         memsize = (sqldaPointer->sqlvar[idx].sqllen);
         break;
      case SQL_TYP_DECIMAL:
      case SQL_TYP_NDECIMAL:
         precision = ((char *)&(sqldaPointer->sqlvar[idx].sqllen))[0];
         sqldaPointer->sqlvar[idx].sqldata = (char *SQL_POINTER)
            malloc ((precision + 2) /2);
         memsize = (precision +2) /2;
         break;

      default:
         sqldaPointer->sqlvar[idx].sqldata = (char *SQL_POINTER)
            malloc (sqldaPointer->sqlvar[idx].sqllen);
         memsize = sqldaPointer->sqlvar[idx].sqllen;
         break;
      } /* endswitch */



      if (sqldaPointer->sqlvar[idx].sqldata == NULL) {
         fprintf (stderr, "\nalloc host vars: out of memory error.\n");
         exit (-1);
      } else {
         memset (sqldaPointer->sqlvar[idx].sqldata,'\0',memsize);
      } /* endif */


      /*
      ** If the SQLTYPE is odd, the host var requires a null indicator variable
      ** this is needed to allocate memory for "sqlind" which is used in the
      ** "display_da" procedure when detecting whether or not the host indicator
      ** is NULLable or not.
      */
      if ( sqldaPointer->sqlvar[idx].sqltype & 1 ) {
         /* Allocate storage for short int */
         sqldaPointer->sqlvar[idx].sqlind = (short *)malloc(sizeof(short));
         /* Detect memory allocation error */
         if ( sqldaPointer->sqlvar[idx].sqlind == NULL ) {
            fprintf(stderr, "Out of dynamic memory while "
               "allocating for your select statement\n") ;
            exit(-1) ;
         } else {
            /* initialize memory to zero */
            memset(sqldaPointer->sqlvar[idx].sqldata,'\0',sizeof(short));
         } /* endif */
      } /* endif */
   } /* endfor */
}

/*******************************************************************************
* FUNCTION : free_da
* This function frees up the memory that has been allocated for the use of
*  an SQLDA data structure.
*******************************************************************************/
void free_da (struct sqlda *sqldaPointer) {
   short idx;
   for (idx = 0; idx < sqldaPointer->sqld; idx++) {
      free (sqldaPointer->sqlvar[idx].sqldata);
      if (sqldaPointer->sqlvar[idx].sqltype & 1) {
         free (sqldaPointer->sqlvar[idx].sqlind);
      }
   } /* endfor */
   free (sqldaPointer);
}

/*******************************************************************************
* PROCEDURE : print_var
* The following procedure prints out the SQLDA SQLVAR variables.
* The procedure first determines which type the data is and then goes through
*  the appropriate sub-routines that are required to print out the data.
*
*******************************************************************************/
void print_var (char *ptr, int type, short collen, short datalen) {
   short idx, ind ;            /* Array idx variables */
   /* Variables for decoding packed decimal data */
   short bottom, point ;
   unsigned short top, precision, scale;

   short *iptr;                           /* Pointer for short integer data. */
   char blen;                             /* Single byte length variable */
   sqlint32 *lptr ;                       /* Pointer for long integer data */
   double *dptr ;                         /* Pointer for float data */
   short maxCollen = max(collen, datalen);

   maxCollen = max(maxCollen, 5);         /* setting a minimum column length */
   /*
   ** Determine the type of data, coerce or decode the data for output
   ** if necessary, then output the data.
   */

   switch ( type ) {
      case SQL_TYP_INTEGER:   /* long */
      case SQL_TYP_NINTEGER:  /* long with null indicator */
         lptr = (sqlint32 *) ptr ;
         printf("%*ld", maxCollen, (long)*lptr ) ;
         break ;
      case SQL_TYP_SMALL:     /* short */
      case SQL_TYP_NSMALL:    /* short with null indicator */
         iptr = (short *) ptr ;
         printf("%*d", maxCollen, *iptr ) ;
         break ;
      case SQL_TYP_DECIMAL:   /* decimal */
      case SQL_TYP_NDECIMAL:  /* decimal with null indicator */
      /* Determine the scale and precision */
         precision = ((char *)&(maxCollen))[0];
         scale = ((char *)&(maxCollen))[1];

/*****************************************************************************/
/* Note:  Precision can only be odd because internally only odd are stored.  */
/*        When and if it happens that an even precision can occur, the       */
/*        precision must be incremented by 1 in order for the proper         */
/*        calculation of "idx" and "point" to occur, and the correct value */
/*        outputted.                                                         */
/*****************************************************************************/
         if ((precision %2) == 0) precision += 1;

         /* Calculate the total number of bytes */
         idx = ( short ) ( precision + 2 ) / 2 ;
         point = precision - scale ;

         /* Determine the sign */
         bottom = *(ptr + idx -1) & 0x000F ;   /* sign */
         if ( (bottom == 0x000D) || (bottom == 0x000B) ) {
            printf("-") ;
         } else {
            printf(" ") ;
         }


         /* Decode and print the decimal number */
         for (ind=0; ind < idx; ind++) {
            top = *(ptr + ind) & 0x00F0 ;
            top = (top >> 4 ) ;
            bottom = *(ptr + ind) & 0x000F ;
               if ( point-- == 0 ) printf(".") ;
               printf("%d", top ) ;
/*****************************************************************************/
/* Ignore bottom of last half byte because its the sign.                     */
/*****************************************************************************/
            if ( ind < idx - 1 ) { /* sign half byte ? */
               if ( point-- == 0 ) printf(".") ;
               printf("%d", bottom ) ;
            }
         }
         if ( scale == 0 ) printf(".") ;
         break ;

      case SQL_TYP_FLOAT:  /* double */
      case SQL_TYP_NFLOAT: /* double with null indicator */
         dptr = (double *) ptr ;
         printf("%*.*e", maxCollen, datalen, *dptr ) ;
         break ;
      case SQL_TYP_CHAR:  /* fixed length character string */
      case SQL_TYP_NCHAR: /* fixed length character string with null
                             indicator */
         printf ("%-*.*s", maxCollen, datalen, ptr);
         break;


      case SQL_TYP_LSTR:  /* varying length character string, 1-byte length */
      case SQL_TYP_NLSTR: /* varying length character string, 1-byte length,
                             with null indicator */

         /*
         ** Initialize blen to the value the length field in the
         ** varchar data structure.
         */
         blen = *ptr;

         /* Advance the data pointer beyond the length field */
         ptr+=sizeof(char);
         for (idx = 0; (idx < (short)blen) && (idx < maxCollen);
            idx++)
            printf("%c",*(ptr + idx) ) ;
         break ;
      case SQL_TYP_CSTR:  /* null terminated varying length character string */
      case SQL_TYP_NCSTR: /* null terminate varying length character
                              string with null indicator */
         /*
         ** Advance the data pointer beyond the length field and print the data.
         */
         printf ("%-*.*s", maxCollen, datalen, ptr);
         break ;
      default:
         /* Unknown data type */
         printf("%-*.*s", maxCollen, datalen, "UNKNOWN");
         break ;
   }
   /* print the column indicator */
   printf (" | ");
}


/*******************************************************************************
* PROCEDURE : display_col_titles
* The following procedure displays the column names, which is available from
*  the SQLDA data structure.  This is then passed on to "print_var" to be
*  outputted on to the screen.
*******************************************************************************/
void display_col_titles (struct sqlda *sqldaPointer) {
   short sqlvarIndex, numBytes;

   for(sqlvarIndex=0; sqlvarIndex < sqldaPointer->sqld; sqlvarIndex++) {
      switch (sqldaPointer->sqlvar[sqlvarIndex].sqltype) {
      case SQL_TYP_DECIMAL:
      case SQL_TYP_NDECIMAL:
         numBytes = max (sqldaPointer->sqlvar[sqlvarIndex].sqlname.length,
            ((char *)&(sqldaPointer->sqlvar[sqlvarIndex].sqllen))[0] + 2);
         break;
      case SQL_TYP_BLOB:
      case SQL_TYP_NBLOB:
      case SQL_TYP_DBCLOB:
      case SQL_TYP_NDBCLOB:
         numBytes = LOBLENGTH;
         break;
      case SQL_TYP_CLOB:
      case SQL_TYP_NCLOB:
         numBytes = LOBLENGTH;
         break;
      case SQL_TYP_GRAPHIC:
      case SQL_TYP_NGRAPHIC:
         break;
      default:
         numBytes = max (sqldaPointer->sqlvar[sqlvarIndex].sqlname.length,
            sqldaPointer->sqlvar[sqlvarIndex].sqllen);
         break;
      } /* endswitch */
      print_var (sqldaPointer->sqlvar[sqlvarIndex].sqlname.data,
         SQL_TYP_CSTR, numBytes,
         sqldaPointer->sqlvar[sqlvarIndex].sqlname.length);
   }
   printf("\n");
}

/*******************************************************************************
* PROCEDURE : display_da
* The following procedure displays the output of pointer which has been
*  passed through.  All pertinent information on the structure of the outputted
*  data is available from this pointer, and is further examined in the
*  procedure "print_var".
*******************************************************************************/
void display_da (struct sqlda *sqldaPointer) {
   short numBytes, sqlvarIndex;
   struct lob {
      sqlint32 length;
      char *data;
   } *lobPointer;

   /* Output the contents for all host variables */
   for(sqlvarIndex=0;sqlvarIndex < sqldaPointer->sqld; sqlvarIndex++) {
      numBytes = sqldaPointer->sqlvar[ sqlvarIndex ].sqllen ;

      if ( sqldaPointer->sqlvar[sqlvarIndex].sqltype & 1 &&
         *(sqldaPointer->sqlvar[sqlvarIndex].sqlind) < 0 ) {
         /* the data in the sqlda is NULL */
         switch (sqldaPointer->sqlvar[sqlvarIndex].sqltype) {
            case SQL_TYP_NBLOB:
            case SQL_TYP_NDBCLOB:
               printf ("LOB length = NULL       | ");
               break;
            case SQL_TYP_NCLOB:
               print_var ("NULL CLOB", SQL_TYP_CSTR, LOBLENGTH, LOBLENGTH);
               break;
            case SQL_TYP_NGRAPHIC:
               break;
            case SQL_TYP_NDECIMAL:
               numBytes = max (sqldaPointer->sqlvar[sqlvarIndex].sqlname.length,
                  ((char *)&(sqldaPointer->sqlvar[sqlvarIndex].sqllen))[0] + 2);

               print_var ("-", SQL_TYP_CSTR, numBytes,
                  sqldaPointer->sqlvar[sqlvarIndex].sqlname.length);
               break;
            default:
               /* Output the data in the sqlda */
               print_var( sqldaPointer->sqlvar[ sqlvarIndex ].sqldata,
                  sqldaPointer->sqlvar[ sqlvarIndex ].sqltype,
                  sqldaPointer->sqlvar[sqlvarIndex].sqlname.length,
                  numBytes);
               break;
         } /* endswitch */
      } else {   /* output the data in the sqlda */
         switch (sqldaPointer->sqlvar[sqlvarIndex].sqltype) {
            case SQL_TYP_CLOB:
            case SQL_TYP_NCLOB:
            case SQL_TYP_BLOB:
            case SQL_TYP_NBLOB:
            case SQL_TYP_DBCLOB:
            case SQL_TYP_NDBCLOB:
               lobPointer = (struct lob *)
                  (sqldaPointer->sqlvar[sqlvarIndex].sqldata);
               if (*sqldaPointer->sqlvar[sqlvarIndex].sqlind == 0) {
                  printf ("LOB length = %10ld       | ", lobPointer->length);
               } else {
               /* the sqlvar LOB has been truncated */
                  printf ("LOB length = %10ld trunc.| ", lobPointer->length);
               } /* endif */
               break;
            case SQL_TYP_GRAPHIC:
            case SQL_TYP_NGRAPHIC:
               printf ("GRAPHIC length = %5ld | ",
                  sqldaPointer->sqlvar[sqlvarIndex].sqlname.length);
               break;
            default:
               /* Output the data in the sqlda */
               print_var( sqldaPointer->sqlvar[ sqlvarIndex ].sqldata,
                  sqldaPointer->sqlvar[ sqlvarIndex ].sqltype,
                  sqldaPointer->sqlvar[sqlvarIndex].sqlname.length,
                  numBytes);
               break;
         } /* endswitch */
      } /* endif */
   } /* endfor */
   printf("\n") ;
}
/* COMMENT OUT OFF */
