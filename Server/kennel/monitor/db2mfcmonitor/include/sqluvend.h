/******************************************************************************
**
** Source File Name: SQLUVEND
**
** (C) COPYRIGHT International Business Machines Corp. 1987, 1997
** All Rights Reserved
** Licensed Materials - Property of IBM
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
**
**
** This include file is to be used by backup interface vendors.
**
** Function = Include File defining:
**              - Interface to vendor devices.
**              - Structures required by vendor interfaces.
**              - Defined symbols and return codes to be
**                returned from vendor interfaces.
**
*******************************************************************************/
#ifndef _H_SQLUVEND
#define _H_SQLUVEND

#ifdef __cplusplus
extern "C" {
#endif

#include "sql.h"

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


/******************************************************************************
** sqluv API Return Codes
******************************************************************************/
#define SQLUV_OK               0   /* Operation is successful */
#define SQLUV_LINK_EXIST       1   /* Session activated previously */
#define SQLUV_COMM_ERROR       2   /* Communication error with device */
#define SQLUV_INV_VERSION      3   /* The DB2 and vendor products are incompatible */
#define SQLUV_INV_ACTION       4   /* Invalid action is requested */
#define SQLUV_NO_DEV_AVAIL     5   /* No device is available for use at the moment */
#define SQLUV_OBJ_NOT_FOUND    6   /* Object specified cannot be found */
#define SQLUV_OBJS_FOUND       7   /* More than 1 object matches specification is found */
#define SQLUV_INV_USERID       8   /* Invalid userid specified */
#define SQLUV_INV_PASSWORD     9   /* Invalid password provided */
#define SQLUV_INV_OPTIONS      10  /* Invalid options specified */
#define SQLUV_INIT_FAILED      11  /* Initialization failed */
#define SQLUV_INV_DEV_HANDLE   12  /* Invalid device handle */
#define SQLUV_BUFF_SIZE        13  /* Invalid buffer size specified */
#define SQLUV_END_OF_DATA      14  /* End of data reached */
#define SQLUV_END_OF_TAPE      15  /* End of tape reached. Requires attention */
#define SQLUV_DATA_RESEND      16  /* Device requested to have last buffer sent again */
#define SQLUV_COMMIT_FAILED    17  /* Commit request failed */
#define SQLUV_DEV_ERROR        18  /* Device error */
#define SQLUV_WARNING          19  /* Warning. see return code */
#define SQLUV_LINK_NOT_EXIST   20  /* Session not activated previously */
#define SQLUV_MORE_DATA        21  /* More data to come */
#define SQLUV_ENDOFMEDIA_NO_DATA   22  /* End of media encountered with no data */
#define SQLUV_ENDOFMEDIA           23  /* ENd of media encountered */
#define SQLUV_MAX_LINK_GRANT   24  /* Max no. of link established */
#define SQLUV_IO_ERROR         25  /* I/O error encountered */
#define SQLUV_DELETE_FAILED    26  /* Delete object fails */
#define SQLUV_INV_BKUP_FNAME   27  /* Invalid backup filename provided */
#define SQLUV_NOT_ENOUGH_SPACE 28  /* insufficient space for estimated database size */
#define SQLUV_ABORT_FAILED     29  /* Abort request failed */
#define SQLUV_UNEXPECTED_ERROR 30  /* A severe error was experienced */
#define SQLUV_NO_DATA          31  /* No data has been returned */

#define SQLUV_COMMIT           0
#define SQLUV_ABORT            1
#define SQLUV_TERMINATE        2    /* For use by ADSM only */

#define SQLUV_COMMENT_LEN     30

typedef struct Return_code
{
        sqlint32   return_code;  /* return code from the vendor function  */
        char       description[SQLUV_COMMENT_LEN];
        /* descriptive message                   */
        void       *reserve;     /* reserve for future use                */
} Return_code;


/******************************************************************************
** Misc Definitions
******************************************************************************/

/*
** Caller actions
*/

/* For sqluvint */
#define SQLUV_WRITE         'W'   /* to write images         */
#define SQLUV_READ          'R'   /* to read images          */
#define SQLUV_ARCHIVE       'A'   /* to archive (write) logs */
#define SQLUV_RETRIEVE      'T'   /* to retrieve (read) logs */
#define SQLUV_QUERY_IMAGES  'I'   /* to query images         */
#define SQLUV_QUERY_LOGS    'L'   /* to query logs           */

/* For sqluvdel */
#define SQLUV_DELETE_IMAGES 'M'   /* for images              */
#define SQLUV_DELETE_LOGS   'O'   /* for logs                */
#define SQLUV_DELETE_ALL    'B'   /* for images/logs         */

/*
** Low level backup and load copy image names
*/
#define SQLUV_NAME_DB         "DB"
#define SQLUV_NAME_TSP        "TSP"
#define SQLUV_NAME_INCR       "_INCR"
#define SQLUV_NAME_DELTA      "_DELTA"
#define SQLUV_NAME_SUFFIX     "_BACKUP"

#define SQLUV_NAME_LOAD_COPY  "LOAD_COPY"
#define SQLUV_NAME_DB_FULL    "FULL"                           SQLUV_NAME_SUFFIX
#define SQLUV_NAME_DB_INCR    SQLUV_NAME_DB  SQLUV_NAME_INCR   SQLUV_NAME_SUFFIX
#define SQLUV_NAME_DB_DELTA   SQLUV_NAME_DB  SQLUV_NAME_DELTA  SQLUV_NAME_SUFFIX
#define SQLUV_NAME_TSP_FULL   SQLUV_NAME_TSP                   SQLUV_NAME_SUFFIX
#define SQLUV_NAME_TSP_INCR   SQLUV_NAME_TSP SQLUV_NAME_INCR   SQLUV_NAME_SUFFIX
#define SQLUV_NAME_TSP_DELTA  SQLUV_NAME_TSP SQLUV_NAME_DELTA  SQLUV_NAME_SUFFIX

/*
** Image types to help create names.
 */
#define SQLUV_DB          SQLUB_DB
#define SQLUV_TBSP        SQLUB_TABLESPACE
#define SQLUV_INCREMENTAL SQLUB_INCREMENTAL
#define SQLUV_DELTA       SQLUB_DELTA
#define SQLUV_LOAD_COPY   4

/*
** Create the low-level backup image name used by DB2 supported storage vendors.
*/
#define SQLUV_NAME_GENERATE(_type, _incr) \
( (_type == SQLUV_DB) \
   ? ( !_incr ? SQLUV_NAME_DB_FULL \
              : ( (_incr == SQLUV_INCREMENTAL) \
                   ? SQLUV_NAME_DB_INCR : SQLUV_NAME_DB_DELTA )) \
   : ( (_type == SQLUV_TBSP) \
        ? ( !_incr ? SQLUV_NAME_TSP_FULL \
                   : ( (_incr == SQLUV_INCREMENTAL) \
                        ? SQLUV_NAME_TSP_INCR : SQLUV_NAME_TSP_DELTA )) \
        : SQLUV_NAME_LOAD_COPY ))


/******************************************************************************
** Vendor API Structures
******************************************************************************/
typedef  struct list_entry
{
    int       entry_len;        /* Including NULL terminator */
    char     *pentry;
} list_entry;

typedef   struct sqlu_gen_list
{
    int                    num_of_entries;
    struct list_entry     *entry;
} sqlu_gen_list;


typedef struct DB2_info
{
  char     *DB2_id;            /* DB2_id                            */
  char     *version;           /* Current DB2 version               */
  char     *release;           /* Current DB2 release               */
  char     *level;             /* Current DB2 level                 */
  char     *action;            /* Caller action                     */
  char     *filename;          /* file to read or write.            */
  char     *server_id;         /* Unique name identifying db server */
  char     *db2instance;       /* db2insance id                     */
  char     *type;              /* When action is SQLUV_WRITE,       */
                               /*  0 - full database backup         */
                               /*  3 - datapool level backup        */
                               /*  4 - load copy image              */
                               /* When action is SQLUV_READ,        */
                               /*  0 - full restore                 */
                               /*  4 - tablespace restore           */
                               /*    - load copy restore            */
                               /*  5 - history file restore         */
  char     *dbname;            /* Database alias to be backed up or */
                               /*   recovered                       */
  char     *alias;             /* Database alias to be backed up or recovered */
  char     *timestamp;         /* Timestamp to identify the backup image */
  char     *sequence;          /* Sequence number within a backup        */

  struct sqlu_gen_list         /* List of objects in the backup     */
                  *obj_list;
  sqlint32  max_bytes_per_txn; /* Transfer buffer size want to use  */
  char     *image_filename;    /* Not used.                         */
  void     *reserve;           /* Reserved for future use           */
  char     *nodename;          /* name of node at which the backup  */
                               /* was generated                     */
  char     *password;          /* password for the node at which the */
                               /* backup was generated              */
  char     *owner;             /* backup originator ID              */
  char     *mcNameP;           /* Management Class                  */
  SQL_PDB_NODE_TYPE nodeNum;   /* Node number.                      */
} DB2_info ;



typedef struct Vendor_info
{
  char     *vendor_id;         /* An identifier for the vendor      */
  char     *version;           /* Current version                   */
  char     *release;           /* Current release                   */
  char     *level;             /* Current level                     */
  char     *server_id;         /* Unique name identifying db server */
  sqlint32  max_bytes_per_txn; /* Vendor supports max bytes / transfer*/
  sqlint32  num_objects_in_backup;   /* no. of objects found in backup*/
  void     *reserve;           /* Reserve for future use.           */
} Vendor_info;



typedef struct Init_input
{
   struct DB2_info  *DB2_session;   /* DB2 Identifier for session.  */
   unsigned short   size_options;   /* size of options field.       */
   sqluint32        size_HI_order;  /* High order 32 bits of DB size*/
   sqluint32        size_LOW_order; /* Low order 32 bits of DB size */
   void             *options;       /* options passed in by user.   */
   void             *reserve;       /* reserve for future use.      */
   char             *prompt_lvl;    /* Prompt level                 */
   unsigned short   num_sessions;   /* Number of sessions           */
} Init_input;


typedef struct Init_output
{
   struct Vendor_info * vendor_session; /* Vendor id for the session */
   void               * pVendorCB;      /* vendor control block      */
   void               * reserve;        /* reserve for future use.   */
} Init_output ;

typedef struct Data
{
   sqlint32  obj_num;                 /* indicate which obj to be read */
                                      /* It is useful for restore.     */
   sqlint32  buff_size;               /* buffer size to be used        */
   sqlint32  actual_buff_size;        /* actual bytes read or written  */
   void      *dataptr;                /* Pointer to the data buffer    */
   void      *reserve;                /* reserve for future use        */
} Data;



/******************************************************************************
** Vendor API Prototypes
******************************************************************************/

/* To initialize or link to the device. */
int sqluvint ( struct Init_input   *,
               struct Init_output  *,
               struct Return_code  *);


/* To read data from initialized device. */
int sqluvget ( void *             ,
               struct Data         *,
               struct Return_code  *);


/* To write data to the initialized device. */
int sqluvput ( void *             ,
               struct Data         *,
               struct Return_code  *);


/* To end or unlink the device and free all its related resources. */
int sqluvend ( sqlint32  ,
               void *    ,
               struct Init_output  *,
               struct Return_code  *);


/* To delete a committed session. */
int sqluvdel ( struct Init_input   *,
               struct Init_output  *,
               struct Return_code  *);


#if (defined(DB2OS2) || defined(DB2NT) || defined(DB2WIN))
#pragma pack()
#elif (defined(DB2MAC))
#pragma align
#elif (defined(DB2HP) || defined(DB2SNI))

#elif (defined(DB2AIX) || (defined(DB2SUN) && defined(__xlC__)))
#pragma options align=reset
#endif

#ifdef __cplusplus
}
#endif

#endif
