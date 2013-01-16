/******************************************************************************
**
** Source File Name: db2VendorApi.h
**
** (C) COPYRIGHT International Business Machines Corp. 1987, 1997
** All Rights Reserved
** Licensed Materials - Property of IBM
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
**
** Function = Include File defining new additions beyond that found in
**            sqluvend.h:
**              - Interface to vendor devices.
**              - Structures required by vendor interfaces.
**              - Defined symbols
**
** Operating System: AIX
**
*******************************************************************************/
#ifndef SQL_H_DB2VENDORAPI
#define SQL_H_DB2VENDORAPI

#ifdef __cplusplus
extern "C" {
#endif

/* Required Include Files */

#include "sql.h"
#include "sqlenv.h"
#include "sqlutil.h"
#include "db2ApiDf.h"
#include "sqluvend.h"


/* API version constants */

#define  DB2VENDOR_API_VERSION1   db2Version820


/* Constants for field lengths */

#define   DB2VENDOR_MAX_FILENAME_SZ    1024
#define   DB2VENDOR_MAX_MGMTCLASS_SZ   256
#define   DB2VENDOR_MAX_OWNER_SZ       128


/* Structures */

typedef struct db2VendorQueryInfo
{
   db2Uint64          sizeEstimate;                      /* Size estimate of obj */
   db2Uint32          type;                              /* Type of image        */
   SQL_PDB_NODE_TYPE  dbPartitionNum;                    /* Db partition number  */
   db2Uint16          sequenceNum;                       /* Seq. # of image      */

   char       db2Instance[SQL_INSTNAME_SZ + 1];          /* DB2 instance         */
   char       dbname[SQL_DBNAME_SZ + 1];                 /* Database name        */
   char       dbalias[SQL_ALIAS_SZ + 1];                 /* Database alias       */
   char       timestamp[SQLU_TIME_STAMP_LEN + 1];        /* Timestamp of image   */
   char       filename[DB2VENDOR_MAX_FILENAME_SZ + 1];   /* Specific filename    */
   char       owner[DB2VENDOR_MAX_OWNER_SZ + 1];         /* Object owner         */
   char       mgmtClass[DB2VENDOR_MAX_MGMTCLASS_SZ + 1]; /* Vendor defined       */
   char       oldestLogfile[DB2_LOGFILE_NAME_LEN + 1];   /* Oldest log file      */
                                                         /* stored with an image */
} db2VendorQueryInfo;


/* Functions */

/* Determine what level of the vendor API is supported by vendor library. */
void db2VendorQueryApiVersion ( db2Uint32  * );

/* Return next matching object for a query operation. */
int db2VendorGetNextObj ( void                        * ,
                          struct db2VendorQueryInfo   * ,
                          struct Return_code          * );

#ifdef __cplusplus
}
#endif

#endif
