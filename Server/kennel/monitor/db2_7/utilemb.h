/*******************************************************************************
**
** Source File Name = utilemb.h
**
** Licensed Materials - Property of IBM
**
** (C) COPYRIGHT International Business Machines Corp. 1995, 1999
** All Rights Reserved.
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
**
**
*******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_UID_LENGTH      18
#define MAX_PWD_LENGTH      30

#ifndef max
#define  max(A, B) ((A) > (B) ? (A) : (B))
#endif
#ifndef min
#define  min(A, B) ((A) > (B) ? (B) : (A))
#endif

/* Defines for BINDING */
#define DEFAULT_ISOLVL     0
#define REPEATABLE_READ    1
#define READ_STABILITY     2
#define CURSOR_STABILITY   3
#define UNCOMMITTED_READ   4

#define MEMERROR        32000
#define  LOBLENGTH  29


void init_da (struct sqlda **DAPointer, int DAsqln);

void alloc_host_vars (struct sqlda *sqldaPointer);

void free_da (struct sqlda *sqldaPointer);

void print_var (char *ptr, int type, short collen, short datalen);

void display_col_titles (struct sqlda *sqldaPointer);

void display_da (struct sqlda *sqldaPointer);

/* Macro for embedded SQL Checking */
//int rc;
#define EMB_SQL_CHECK( MSG_STR )                               \
    rc = SqlInfoPrint( MSG_STR, &sqlca, __LINE__, __FILE__) ;  \
    if ( rc != 0 )                                             \
    {   TransRollback( );                                      \
        return 1;                                              \
    }		                                              


/* functions used in ...SQL_CHECK macro */
int  SqlInfoPrint( char *, struct sqlca *, int, char *);
void TransRollback( void);
    
/* other useful functions with self-explanatory names */
int CmdLineArgsCheck1( int, char * argv[], char *, char *, char *);
int CmdLineArgsCheck2( int, char * argv[], char *, char *, char *, char *);
int EmbAppInit( char * , char *, char *);
int EmbAppTerm( char *);


#ifdef __cplusplus
}
#endif


#define    sp5     "     "
#define    sp8     "        "

