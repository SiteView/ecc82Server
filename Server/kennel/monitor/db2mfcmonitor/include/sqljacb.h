/******************************************************************************
 *
 * Source File Name: SQLJACB.H
 *
 * (C) COPYRIGHT International Business Machines Corp. 1991, 1997
 * All Rights Reserved
 * Licensed Materials - Property of IBM
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *
 * Function: Include File defining:
 *             Database Connection Services (DCS) Interface - Constants
 *             DCS Interface - SQLJ_ACBH, SQLJACB handle structure
 *             DCS Interface - SQLJACB, DCS Interface Control Block
 *
 * Operating System: Common C Include File
 * 155350  24/Jul/2000 xun remove sqlapconst and sqlapcons2
 *
 *****************************************************************************/

#ifndef  SQL_H_SQLJACB                  /* Permit duplicate includes */
   #define SQL_H_SQLJACB

#include "sqlsystm.h"

#if (defined(DB2OS2) || defined(DB2NT))
#if defined _WIN64
#pragma pack(8)
#else
#pragma pack(4)
#endif
#elif (defined(DB2WIN))
#pragma pack(2)
#elif (defined(DB2MAC))
#if (defined(DB2PPC))
#pragma align 4
#elif (defined(DB268K))
#pragma align 2
#endif
#elif (defined(DB2HP) || defined(DB2SNI))

#elif (defined(DB2SUN) && defined(__xlC__))
#pragma options align=natural
#elif (defined(DB2AIX) && defined(__64BIT__))
#pragma options align=natural
#elif (defined(DB2AIX))
#pragma options align=power
#endif

/* Forward Declarations */
struct SQLERCB;

#define SQLJACB_TTL       "SQLJACB "    /* SQLJACB Title   */
#define SQLJACB_VER       0x0100        /* SQLJACB Version */

/* Connection type - SQLJ_ACBH.conn_type */

#define SQLJ_CONN_DRD1     1
#define SQLJ_CONN_ASP0     2
#define SQLJ_CONN_LOC      3
#define SQLJ_CONN_VNDR     5

/* ACB type - SQLJACB.sqlapcont */
#define SQLJALOC        0x00            /* Database is local to Application  */
#define SQLJAREM        0x01            /* Database is remote to Application */

typedef struct sqlj_acb
{
   char                  sqlacbid[8];   /* Control block eye catcher */
   sqluint32             sqlacbsz;      /* Size in bytes             */
   unsigned short        sqlacbver;     /* Version number            */

   unsigned short        sqlnacb;       /* Reserved for Database Connection
                                           Router use only                  */
   /*************************************************************************/
   /* the following two pointer fields are in support of the                */
   /* Application connection set. Each SQLJACB is linked onto one           */
   /* of the synonym chains within the sqlecvt array in the                 */
   /* sqlerglob structure.                                                  */
   /*************************************************************************/
   struct sqlj_acb      *sqljfwd;       /* forward chain field              */
   struct sqlj_acb      *sqljback;      /* backward chain field             */
   short                 sqlhash;       /* index into sqlecvt               */
   /*************************************************************************/
   /* the database use, product signature and territory code are              */
   /* moved from the sqlerglob to the sqljacb as they are attributes        */
   /* of the connection.                                                    */
   /*************************************************************************/
   char                  use;           /* database use                 */
   char                  prod_sig[8];   /* served product signature     */
   char                  dbcpcc[32];    /* Database  territory and      */
                                        /* code page                    */
   unsigned short        sqlapcont;     /* Application connection type  */

/* Process and thread id */

   unsigned short        sqlappid;      /* Process ID of server process */
   unsigned short        sqlaptid;      /* Thread ID of server process  */

/* Codepages */

   unsigned short        sqlapcpg;      /* Codepage of originating
                                           application             */
   unsigned short        sqldbcpg;      /* Codepage of database    */

/* Datastream type */

   unsigned short        sqldtast;      /* Datastream type */

/* Originating Application node */

   sqluint32             sqlapnnl;      /* Length of application node name    */
   unsigned short        sqlapnnt;      /* Data type of application node name */
   void *                sqlapnn;       /* Pointer to application node name   */

/* Originating Application ID */

   sqluint32             sqlapidl;      /* Length of application id    */
   unsigned short        sqlapidt;      /* Data type of application id */
   void *                sqlapid;       /* Pointer to application id   */

/* Database Connection Router internal control block */

   sqluint32             sqlcrtcbl;     /* Length of Database Connection
                                           Router internal control block    */
   unsigned short        sqlcrtcbt;     /* Data type of Database Connection
                                           Router internal control block    */
   struct SQLERCB *      sqlcrtcb;      /* Pointer to Database Connection
                                           Router internal control block
                                           (Database Connection Router use
                                           only)                            */

/* Target internal control block */

   sqluint32             sqlcsvcbl;     /* Length of target internal control
                                           block                              */
   unsigned short        sqlcsvcbt;     /* Data type of target internal
                                           control block                      */
   void *                sqlcsvcb;      /* Pointer to target internal control
                                           block                              */

/* Userid */

   sqluint32             sqlusl;        /* Length of userid    */
   unsigned short        sqlust;        /* Data type of userid */
   char *                sqluser;       /* Pointer to userid   */

/* Alias name in system database directory */

   sqluint32             sqldball;      /* Length of alias name              */
   unsigned short        sqldbalt;      /* Data type of alias name           */
   char *                sqldbal;       /* Pointer to alias name of database
                                           in system database directory      */

/* Database name in system database directory */

   sqluint32             sqlldbnl;      /* Length of database name      */
   unsigned short        sqlldbnt;      /* Data type of database name   */
   char *                sqlldbn;       /* Pointer to name of database
                                           in system database directory */

/* Node name in system database directory */

   sqluint32             sqlnnl;        /* Length of node name            */
   unsigned short        sqlnnt;        /* Data type of node name         */
   char *                sqlnn;         /* Pointer to node name in system
                                           database directory             */

/* Database name in Database Connection Services directory */

   sqluint32             sqlsdbnl;      /* Length of database name            */
   unsigned short        sqlsdbnt;      /* Data type of database name         */
   char *                sqlsdbn;       /* Pointer to database name in Database
                                           Connection Services directory      */

/* Target library name */

   sqluint32             sqlpgmnl;      /* Length of library name        */
   unsigned short        sqlpgmnt;      /* Data type of library name     */
   char *                sqlpgmn;       /* Pointer to library name of
                                           target element                */

/* Parameter string */

   sqluint32             sqlparml;      /* Length of parameter string       */
   unsigned short        sqlparmt;      /* Data type of parameter string    */
   char *                sqlparm;       /* Pointer to parameter string from
                                           Database Connection Services
                                           directory                        */
/* Target element data heap */

   sqluint32             sqlrshpl;      /* Length of target data heap    */
   unsigned short        sqlrshpt;      /* Data type of target data heap */
   unsigned int  *       sqlrshp;       /* Pointer to target data heap   */

} SQLJACB;

#define SQLJ_LEN_SQLJACB sizeof(SQLJACB)

/* SQLJ_ACBH */

typedef struct sqlj_acbh
{
   sqlint32            handle;
   short               conn_type;       /* int to short  */
} SQLJ_ACBH;

#if (defined(DB2OS2) || defined(DB2NT) || defined(DB2WIN))
#pragma pack()
#elif (defined(DB2MAC))
#pragma align
#elif (defined(DB2HP) || defined(DB2SNI))

#elif (defined(DB2AIX) || (defined(DB2SUN) && defined(__xlC__)))
#pragma options align=reset
#endif

#endif /* SQL_H_SQLJACB */
