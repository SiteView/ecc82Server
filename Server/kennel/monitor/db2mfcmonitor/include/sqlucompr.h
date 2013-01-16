/******************************************************************************
**
** Source File Name: SQLUCOMPR
**
** (C) COPYRIGHT International Business Machines Corp. 1995, 2002
** All Rights Reserved
** Licensed Materials - Property of IBM
**
** US Government Users Restricted Rights - Use, duplication or
** disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
**
** Function = Include File defining:
**              Compression plug-in - Constants
**              Compression plug-in - Data Structures
**              Compression plug-in - Function Prototypes
**
*******************************************************************************/
#ifndef SQL_H_SQLUCOMPR
#define SQL_H_SQLUCOMPR

#include "db2ApiDf.h"
#include "sqlenv.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SQLUV_OK                        0
#define SQLUV_INIT_FAILED               11
#define SQLUV_UNEXPECTED_ERROR          30
#define SQLUV_BUFFER_TOO_SMALL          100
#define SQLUV_PARTIAL_BUFFER            101
#define SQLUV_NO_MEMORY                 102
#define SQLUV_EXCEPTION                 103
#define SQLUV_INTERNAL_ERROR            104

/* Description of the DB2 envionment
 * -------------------------------------------------------------------------- */
struct COMPR_DB2INFO {
#define COMPR_DB2INFOTAG                 "COMPR_DB2INFO  "
   char                  tag[16];       /* "COMPR_DB2INFO  \0"                */
   db2Uint32             version;       /* version number of the structure    */
   db2Uint32             size;          /* sizeof(COMPR_DB2INFO)              */
   char                  dbalias[SQLU_ALIAS_SZ+1];
   char                  instance[SQL_INSTNAME_SZ+1];
   SQL_PDB_NODE_TYPE     node;
   SQL_PDB_NODE_TYPE     catnode;
   char                  timestamp[SQLU_TIME_STAMP_LEN+1];
   db2Uint32             bufferSize;
   db2Uint32             options;       /* iOptions passed on API             */
   db2Uint32             bkOptions;     /* iOptions used on backup (for       */
                                        /* restore only)                      */
   db2Uint32             db2Version;    /* Currently db2Version810            */
   db2Uint32             platform;      /* from sqlmon.h                      */
   db2int32              comprOptionsByteOrder;  /* from sqlmon.h             */
   db2Uint32             comprOptionsSize;
   void                 *comprOptions;
   db2Uint32             savedBlockSize; /* size of savedBlock                */
   void                 *savedBlock;    /* block of data saved with backup    */
                                        /* (restore only; NULL for backup)    */
};

/* Description of plugin
 * -------------------------------------------------------------------------- */
struct COMPR_PIINFO {
#define COMPR_PIINFOTAG                 "COMPR_PIINFO   "
   char                  tag[16];       /* "COMPR_PIINFO   \0"                */
   db2Uint32             version;       /* version number of the structure    */
   db2Uint32             size;          /* sizeof(COMPR_PIINFO)               */
   db2Uint32             useCRC;        /* library will calculate CRC (0/1)   */
   db2Uint32             useGran;       /* library will respect srcGran (0/1) */
   db2Uint32             useAllBlocks;  /* always store compressed data       */
   db2Uint32             savedBlockSize; /* size of data block stored in bkp  */
};

/* Control block.  Contains data used internally by
 * compression and decompression routines
 * -------------------------------------------------------------------------- */
struct COMPR_CB;

int InitCompression(                    /* Initialise for compression         */
      const struct COMPR_DB2INFO
                        *db2Info,       /* (in)  DB2 info                     */
      struct COMPR_PIINFO
                        *piInfo,        /* (out) Info about plugin            */
      struct COMPR_CB  **pCB);          /* (out) Control block                */

int GetSavedBlock(                      /* Get data to save in backup         */
      struct COMPR_CB   *pCB,           /* (in)  Control block                */
      db2Uint32          blockSize,     /* (in)  size of block from Init call */
      void              *data);         /* (out) data                         */

int Compress(                           /* Compress a block of data           */
      struct COMPR_CB   *pCB,           /* (in)  Control block                */
      const char        *src,           /* (in)  text to compress             */
      db2int32           srcSize,       /* (in)  length of data block         */
      db2Uint32          srcGran,
      char              *tgt,           /* (i/o) buffer to compress into      */
      db2int32           tgtSize,       /* (in)  size of buffer               */
      db2int32          *srcAct,        /* (out) actual size of input         */
      db2int32          *tgtAct,        /* (out) actual size of output        */
      db2Uint32         *tgtCRC);       /* (out) crc of input data (or 0)     */

int GetMaxCompressedSize(               /* Estimate size of compressed data   */
      struct COMPR_CB   *pCB,           /* (in)  Control block                */
      db2Uint32          srcLen);       /* (in)  length of data block         */

int TermCompression(                    /* Terminate compression.             */
      struct COMPR_CB   *pCB);          /* (in)  Control block                */

int InitDecompression(                  /* Initialise for decompression       */
      const struct COMPR_DB2INFO
                        *db2Info,       /* (in)  DB2 info                     */
      struct COMPR_CB  **pCB);          /* (out) Control block                */

int Decompress(                         /* Decompress a block of data         */
      struct COMPR_CB   *pCB,           /* (in)  Control block                */
      const char        *src,           /* (in)  text to decompress           */
      db2int32           srcSize,       /* (in)  length of data block         */
      char              *tgt,           /* (i/o) buffer to decompress into    */
      db2int32           tgtSize,       /* (in)  size of buffer               */
      db2int32          *tgtLen,        /* (out) actual size of output        */
      db2Uint32         *tgtCRC);       /* (out) CRC of output data (or 0)    */

int TermDecompression(                  /* Terminate decompression.           */
      struct COMPR_CB   *pCB);          /* (in)  Control block                */


#ifdef __cplusplus
}
#endif

#endif
