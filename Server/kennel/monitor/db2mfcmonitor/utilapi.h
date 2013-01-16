/****************************************************************************
** Licensed Materials - Property of IBM
** 
** Governed under the terms of the International
** License Agreement for Non-Warranted Sample Code.
**
** (C) COPYRIGHT International Business Machines Corp. 1996 - 2002        
** All Rights Reserved.
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
*****************************************************************************
**
** SOURCE FILE NAME: utilapi.h
**
** SAMPLE: Error-checking utility header file for utilapi.c
**
**         This is the header file for the utilapi.c error-checking utility 
**         file. The utilapi.c file is compiled and linked in as an object 
**         module with non-embedded SQL sample programs by the supplied 
**         makefile and build files.
**
** Macros defined:
**         DB2_API_CHECK(MSG_STR)
**         EXPECTED_ERR_CHECK(MSG_STR)
**         EXPECTED_WARN_CHECK(MSG_STR) 
**
** Functions declared:
**         SqlInfoPrint - prints to the screen SQL warnings and errors
**         CmdLineArgsCheck1 - checks the command line arguments, version 1
**         CmdLineArgsCheck2 - checks the command line arguments, version 2
**         CmdLineArgsCheck3 - checks the command line arguments, version 3
**         CmdLineArgsCheck4 - checks the command line arguments, version 4
**         InstanceAttach - attach to instance
**         InstanceDetach - detach from instance
**
*****************************************************************************
**
** For more information on the sample programs, see the README file.
**
** For information on developing C applications, see the Application
** Development Guide.
**
** For information on DB2 APIs, see the Administrative API Reference.
**
** For the latest information on programming, building, and running DB2 
** applications, visit the DB2 application development website: 
**     http://www.software.ibm.com/data/db2/udb/ad
****************************************************************************/

#ifndef UTILAPI_H
#define UTILAPI_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef max
#define max(A, B) ((A) > (B) ? (A) : (B))
#endif
#ifndef min
#define min(A, B) ((A) > (B) ? (B) : (A))
#endif

#define USERID_SZ 128
#define PSWD_SZ 14

#if (defined(DB2NT))
#define PATH_SEP "\\"
#else /* UNIX */
#define PATH_SEP "/"
#endif

/* macro for DB2_API checking */
#define DB2_API_CHECK(MSG_STR)                     \
SqlInfoPrint(MSG_STR, &sqlca, __LINE__, __FILE__); \
if (sqlca.sqlcode < 0)                             \
{                                                  \
  return 1;                                        \
}

/* macro for expected error checking and message */
#define EXPECTED_ERR_CHECK(MSG_STR)                         \
printf("\n-- The following error report is expected! --"); \
SqlInfoPrint(MSG_STR, &sqlca, __LINE__, __FILE__);          \

/* macro for expected warning */
#define EXPECTED_WARN_CHECK(MSG_STR)                         \
printf("\n-- The following warning report is expected! --"); \
SqlInfoPrint(MSG_STR, &sqlca, __LINE__, __FILE__);          \

/* functions used in ..._CHECK macros */
void SqlInfoPrint(char *, struct sqlca *, int, char *);

/* other functions */
int CmdLineArgsCheck1(int, char * argv[], char *, char *, char *);
int CmdLineArgsCheck2(int, char * argv[], char *, char *, char *);
int CmdLineArgsCheck3(int, char * argv[], char *, char *, char *, char *);
int CmdLineArgsCheck4(int, char * argv[], char *, char *,
                      char *, char *, char *, char *);
int InstanceAttach(char * , char *, char *);
int InstanceDetach(char *);

#ifdef __cplusplus
}
#endif

#endif /* UTILAPI_H */

