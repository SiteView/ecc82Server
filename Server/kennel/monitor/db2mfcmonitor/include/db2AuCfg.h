/**********************************************************************
*
*  Source File Name = db2AuCfg
*
*  (C) COPYRIGHT International Business Machines Corp. 1987, 1999, 2002
*  All Rights Reserved
*  Licensed Materials - Property of IBM
*
*  US Government Users Restricted Rights - Use, duplication or
*  disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
*
*  Function = Include File definitions for the APIs, db2AutoConfig and 
*             db2AutoConfigFreeMemory, and their corresponding data 
*             structures.
*
***********************************************************************/
#ifndef SQL_H_DB2AUCFG
#define SQL_H_DB2AUCFG

#include "sqlenv.h"
#include "db2ApiDf.h"


/***********************************************************************
* Type definitions used for input and output for the db2AutoConfig API
************************************************************************/
typedef struct {
  db2int32  token;
  db2int32  value;
} db2AutoConfigElement;
      
typedef struct {
  db2Uint32  numElements;            /* number of array elements */
  db2AutoConfigElement *  pElements; /* the element array */
} db2AutoConfigArray;

typedef db2AutoConfigArray  db2AutoConfigInput;
typedef db2AutoConfigArray  db2AutoConfigDiags;

typedef struct {
  char * pName;
  db2int32 value;
} db2AutoConfigNameElement;

typedef struct {
  db2Uint32 numElements;                /* number of array elements */
  db2AutoConfigNameElement * pElements; /* the element array */
} db2AutoConfigNameArray;

typedef db2AutoConfigNameArray db2BpValues;

typedef struct {
  db2Uint32  numElements;
  struct sqlfupd *  pConfigs;
  void * pDataArea;
} db2ConfigValues;

typedef struct { 
  db2ConfigValues  oOldDbValues;    /* current/old database configurations */
  db2ConfigValues  oOldDbmValues;   /* current/old database manager configurations */
  db2ConfigValues  oNewDbValues;    /* suggested/new database configurations */
  db2ConfigValues  oNewDbmValues;   /* suggested/new database manager configurations */
  db2AutoConfigDiags  oDiagnostics; /* diagnostic details */
  db2BpValues  oOldBpValues;
  db2BpValues  oNewBpValues;
} db2AutoConfigOutput;

#define DB2_SG_PROD_VERSION_SIZE  16

typedef struct {
  db2int32  iProductID;                             /* product id */
  char iProductVersion[DB2_SG_PROD_VERSION_SIZE+1]; /* product version */
  char iDbAlias[SQL_ALIAS_SZ+1];                    /* database alias */
  db2int32  iApply;                /* update the configuration automatically */
  db2AutoConfigInput  iParams;     /* input parameters */
  db2AutoConfigOutput  oResult;    /* results */
} db2AutoConfigInterface;



/***********************************************************************
* Definitions used for input to the db2AutoConfig API
***********************************************************************/
/* Product ID for the product using the API (values for iProductID) */
#define DB2_SG_PID_DEFAULT                   0
#define DB2_SG_PID_WEBSPHERE_COMMERCE_SUITE  1
#define DB2_SG_PID_SAP                       2
#define DB2_SG_PID_WEBSPHERE_ADVANCED_SERVER 3 
#define DB2_SG_PID_SIEBEL                    4 
#define DB2_SG_PID_PS_EPM                    5
#define DB2_SG_PID_PS_ONLINE                 6
#define DB2_SG_PID_PS_BATCH                  7
#define DB2_SG_PID_PS                        8
#define DB2_SG_PID_LOTUS_DOMINO              9
#define DB2_SG_PID_CONTENT_MANAGER           10

/* Input values for iProductVersion */
#define DB2_SG_PRODUCT_VERSION_1_1  "1.1"

/* Indicate whether to apply the configuration recommendations or not */
#define DB2_SG_NOT_APPLY         0 /* do not apply any recommendations */
#define DB2_SG_APPLY             1 /* apply all recommendations */
#define DB2_SG_APPLY_DB          2 /* apply only database (and bufferpool) */
                                   /* recommendations */

/* Tokens for input parameters (iParams) */

#define DB2_SG_MEMORY_PERCENTAGE   1  /* The percentage of the memory the */
                                      /* database manager tries to use for all */
                                      /* logical partitions, but only for the */
                                      /* current database. Adjust accordingly */
                                      /* on an instance with multiple databases */
#define DB2_SG_WORKLOAD            2  /* The type of workload that best */
                                      /* reflects the database. */
#define DB2_SG_NUM_STATEMENT       3  /* The average number of SQL statements */
                                      /* in a single unit of work. */
#define DB2_SG_TRANS_PER_MINUTE    4  /* The number of transactions per */
                                      /* minute. */
#define DB2_SG_ADMIN_PRIORITY      5  /* The database administration */
                                      /* priority. */
#define DB2_SG_IS_POPULATED        6  /* The database is populated with */
                                      /* data. */
#define DB2_SG_LOCAL_APPLICATION   7  /* Average number of connected local */
                                      /* applications. */
#define DB2_SG_REMOTE_APPLICATION  8  /* Average number of connected remote */
                                      /* applications. */
#define DB2_SG_ISOLATION_LEVEL     9  /* Isolation level that best reflects */
                                      /* your applications. */
#define DB2_SG_BP_RESIZEABLE       10 /* Should db2AutoConfig change */
                                      /* bufferpool sizes. */

/* Valid values for input parameters (iParams) */
#define DB2_SG_MEMORY_PERCENTAGE_MIN 1 
#define DB2_SG_MEMORY_PERCENTAGE_MAX 100 

#define DB2_SG_WORKLOAD_MIN          1
#define DB2_SG_WORKLOAD_QUERIES      1
#define DB2_SG_WORKLOAD_MIXED        2
#define DB2_SG_WORKLOAD_TRANSACTIONS 3
#define DB2_SG_WORKLOAD_MAX          3

#define DB2_SG_NUM_STATEMENT_MIN     1
#define DB2_SG_NUM_STATEMENT_MAX     1000000

#define DB2_SG_ADMIN_PRIORITY_MIN         1
#define DB2_SG_ADMIN_PRIORITY_TRANSACTION 1
#define DB2_SG_ADMIN_PRIORITY_BOTH        2
#define DB2_SG_ADMIN_PRIORITY_RECOVERY    3
#define DB2_SG_ADMIN_PRIORITY_MAX         3

#define DB2_SG_TRANS_PER_MINUTE_MIN 1     
#define DB2_SG_TRANS_PER_MINUTE_MAX 200000

#define DB2_SG_IS_POPULATED_MIN 1
#define DB2_SG_IS_POPULATED_YES 1
#define DB2_SG_IS_POPULATED_NO  2
#define DB2_SG_IS_POPULATED_MAX 2

#define DB2_SG_LOCAL_APPLICATION_MIN 0  
#define DB2_SG_LOCAL_APPLICATION_MAX 5000

#define DB2_SG_REMOTE_APPLICATION_MIN 0 
#define DB2_SG_REMOTE_APPLICATION_MAX 5000 

#define DB2_SG_ISOLATION_LEVEL_MIN              1
#define DB2_SG_ISOLATION_LEVEL_REPEAT_READ      1
#define DB2_SG_ISOLATION_LEVEL_READ_STABILITY   2
#define DB2_SG_ISOLATION_LEVEL_CURSOR_STABILITY 3
#define DB2_SG_ISOLATION_LEVEL_UNCOMMITTED_READ 4
#define DB2_SG_ISOLATION_LEVEL_MAX              4

#define DB2_SG_BP_RESIZEABLE_MIN 0
#define DB2_SG_BP_RESIZEABLE_NO  0 
#define DB2_SG_BP_RESIZEABLE_YES 1
#define DB2_SG_BP_RESIZEABLE_MAX 1

/***********************************************************************
* Definitions used in the output diagnostics
* These token/values pairs may appear in the structure 
* db2AutoConfigOutput.db2AutoConfigDiags.  Some of the tokens return a 
* meaningful value, and some are just flags indicating a state (for which 
* the value will usually be DB2_SG_DIAG_NO_VALUE).
***********************************************************************/
/* The "no value" value for flag-type diagnostic tokens                      */
#define DB2_SG_DIAG_NO_VALUE                    -1

/* Token indicating that the calculations are made for default products.    */
/* This token is returned when DB2_SG_PID_DEFAULT is input to the API.       */
/* The corresponding value returned is DB2_SG_DIAG_NO_VALUE.                 */
#define DB2_SG_DIAG_DEFAULT_USED                 0

/* Token indicating that there is not enough memory on the machine for the   */
/* given inputs.                                                             */
/* The corresponding value returned is the recommended amount of memory.     */
#define DB2_SG_DIAG_NOT_ENOUGH_MEMORY           -1  

/* Token indicating that the are not enough disks for the given inputs.      */
/* The corresponding value returned is the recommended number of disks.      */
#define DB2_SG_DIAG_NOT_ENOUGH_DISKS            -2  

/* Token indicating that the input memory percentage is too low.             */
/* The corresponding value returned is the percentage of memory to expect    */
/* DB2 to consume for the given inputs.                                      */
#define DB2_SG_DIAG_LOW_MEMORY_PERCENTAGE       -3

/* Token indicating that database configurations are applied only to         */
/* the current partition.  (See also associated DB210211 message.)           */
/* The corresponding value returned is DB2_SG_DIAG_NO_VALUE.                 */
#define DB2_SG_DIAG_PARTITION_WARNING           10211 

/* Token indicating that there is not enough memory dedicated to the server, */
/* and no recommendations are made.                                          */
/* The corresponding value returned is the actual amount of physical         */
/* memory on the machine.                                                    */
#define DB2_SG_DIAG_DBA_MEMORY                  1100

/* Token indicating that the advisor was unable to allocate a minimum amount */
/* of memory to the bufferpools.  The corresponding value returned is        */
/* DB2_SG_DIAG_NO_VALUE.                                                     */
#define DB2_SG_DIAG_DBA_MINIMUM_BUFFER_POOL     1103

/* Token indicating that the bufferpools could not be increased due to other */
/* memory requirements determined from the given inputs.                     */
/* The corresponding value returned is DB2_SG_DIAG_NO_VALUE.                 */
#define DB2_SG_DIAG_DBA_BUFFER_POOL_SIZE        1108

/* Token indicating that the requested transaction rate is optimistic. That  */
/* is, the transaction rate is more than ten times the average number of     */
/* connected applications.                                                   */
/* The corresponding value returned is DB2_SG_DIAG_NO_VALUE.                 */
#define DB2_SG_DIAG_DBA_TRANSACTION_RATE        1109



/***********************************************************************
* API return codes
***********************************************************************/
#define DB2_SG_RC_OK     0
#define DB2_SG_RC_FAIL  -1



/***********************************************************************
* API definitions 
***********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
  
SQL_API_RC SQL_API_FN
db2AutoConfig(
  db2Uint32 db2VersionNumber,    /* versions defined in db2ApiDf.h */
  void * pAutoConfigInterface,   /* pointer to db2AutoConfigInterface */
  struct sqlca * pSqlca);

SQL_API_RC SQL_API_FN
db2AutoConfigFreeMemory(
  db2Uint32  db2VersionNumber,                
  void * pAutoConfigInterface,
  struct sqlca * pSqlca);
  
#ifdef __cplusplus
}
#endif



#endif  /* DB2AUCFG_H */
