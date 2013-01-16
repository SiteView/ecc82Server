/******************************************************************************
 *
 * Source File Name = db2gse.h
 *
 * (C) COPYRIGHT International Business Machines Corp. 1993, 2000, 2002
 * All Rights Reserved
 * Licensed Materials - Property of IBM
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with IBM Corp.
 *
 * Function = Include File defining:
 *              DB2 UDB Spatial Extender Interface - Constants
 *              DB2 UDB Spatial Extender Interface - Data Structures
 *              DB2 UDB Spatial Extender Interface - Function Prototypes
 *
 * Operating System = Common C Include File
 *
 *****************************************************************************/

#if !defined(DB2GSE_H_DEFINED)
#define DB2GSE_H_DEFINED

#ifdef __cplusplus
extern "C" {
#endif


/*============================================================================*/
/*==                                                                        ==*/
/*==                          CONSTANT DEFINITIONS                          ==*/
/*==                                                                        ==*/
/*============================================================================*/

#define GSEADM_SUCCESS			 0

/* disable database type */
#define GSE_DISABLE_NOFORCE		 0
#define GSE_DISABLE_FORCE		 1

/* import flags */
#define GSE_IMPORT_EXISTING_TABLE	 0
#define GSE_IMPORT_CREATE_TABLE		 1
#define GSE_IMPORT_ID_IS_NOT_IDENTITY	 0
#define GSE_IMPORT_ID_IS_IDENTITY	 1

/* export flags */
#define GSE_EXPORT_CREATE_NEW_FILE	 0
#define GSE_EXPORT_APPEND_TO_FILE	 1

/* shape format type ids */
#define GSE_SHAPE_TYPE_EMPTY		 0
#define GSE_SHAPE_TYPE_POINT		 1
#define GSE_SHAPE_TYPE_POINT_Z		 9
#define GSE_SHAPE_TYPE_POINT_M		21
#define GSE_SHAPE_TYPE_POINT_ZM		11
#define GSE_SHAPE_TYPE_MULTI_POINT	 8
#define GSE_SHAPE_TYPE_MULTI_POINT_Z	20
#define GSE_SHAPE_TYPE_MULTI_POINT_M	28
#define GSE_SHAPE_TYPE_MULTI_POINT_ZM	18
#define GSE_SHAPE_TYPE_MULTI_LINE	 3
#define GSE_SHAPE_TYPE_MULTI_LINE_Z	10
#define GSE_SHAPE_TYPE_MULTI_LINE_M	23
#define GSE_SHAPE_TYPE_MULTI_LINE_ZM	13
#define GSE_SHAPE_TYPE_MULTI_POLYGON	 5
#define GSE_SHAPE_TYPE_MULTI_POLYGON_Z	19
#define GSE_SHAPE_TYPE_MULTI_POLYGON_M	25
#define GSE_SHAPE_TYPE_MULTI_POLYGON_ZM	15

/* well-known-binary format type ids */
#define GSE_WKB_TYPE_POINT		 1
#define GSE_WKB_TYPE_LINE		 2
#define GSE_WKB_TYPE_POLYGON		 3
#define GSE_WKB_TYPE_MULTI_POINT	 4
#define GSE_WKB_TYPE_MULTI_LINE		 5
#define GSE_WKB_TYPE_MULTI_POLYGON	 6
#define GSE_WKB_TYPE_COLLECTION		 7


/*============================================================================*/
/*==                                                                        ==*/
/*==                    DEPRECATED CONSTANTS DEFINITIONS                    ==*/
/*==                                                                        ==*/
/*============================================================================*/

/* Note: For length specifications refer to the db2 header file <sql.h> */
#include <sqlutil.h>

/* length */
#define MAX_TYPE_NAME       64
#define MAX_SCHEMA_NAME     30
#define MAX_TABLE_NAME      128
#define MAX_COLUMN_NAME     128
#define DEFAULT_VARCHAR     256
#define MAX_STMT            3071
#define MAX_IDENTIFIER      64
#define MAX_TRIG_NAME       SQL_LG_IDENT  /* 18 */
#define MAX_NUM_ATTR        10

/* layer type */
#define GSE_LAYER_SIMPLE    1
#define GSE_LAYER_VIEW      2

/* auto geocode mode */
#define GSE_AUTOGC_CREATE     1
#define GSE_AUTOGC_RECREATE   2
#define GSE_AUTOGC_DROP       3
#define GSE_AUTOGC_INVALIDATE 4

/* return code from check_auth function */
#define GSE_AUTHORIZED        0
#define GSE_NOT_AUTHORIZED    1
#define GSE_PROCESSING_ERR    -1

/* DB2GSE messages */
/* common */
#define GSE0000I        0
#define GSE0001E        -1
#define GSE0002E        -2
#define GSE0003E        -3
#define GSE0004E        -4
#define GSE0005E        -5
#define GSE0006E        -6
#define GSE0007E        -7
#define GSE0008E        -8
#define GSE0009E        -9
#define GSE0010E        -10
#define GSE0011E        -11
#define GSE0012E        -12
#define GSE0013E        -13
#define GSE0014E        -14
#define GSE0498E        -498
#define GSE0499W        -499
#define GSE0500E        -500
/* gseadm - layer */
#define GSE1001E        -1001
#define GSE1002E        -1002
#define GSE1003E        -1003
#define GSE1004E        -1004
#define GSE1005E        -1005
#define GSE1006E        -1006
#define GSE1007E        -1007
#define GSE1008E        -1008
/* gseadm - spatial reference */
#define GSE1020E        -1020
#define GSE1021E        -1021
/* gseadm - geocoder, autogc  */
#define GSE1030E        -1030
#define GSE1031E        -1031
#define GSE1032E        -1032
#define GSE1033E        -1033
#define GSE1034E        -1034
#define GSE1035E        -1035
#define GSE1036E        -1036
#define GSE1037E        -1037
#define GSE1038E        -1038
/* gseadm - index */
#define GSE1050E        -1050
#define GSE1051E        -1051
#define GSE1052E        -1052
/* rungc */
#define GSE1500I        1500
#define GSE1501W        -1501
#define GSE1502W        -1502
/* import/export */
#define GSE2001E        -2001
#define GSE2002E        -2002
#define GSE2003E        -2003
#define GSE2004E        -2004
#define GSE2005E        -2005
#define GSE2006E        -2006
#define GSE2007E        -2007
#define GSE2008E        -2008

#define GSE9999E        -9999  /* undefined */


/*============================================================================*/
/*==                                                                        ==*/
/*==                    DEPRECATED STRUCTURE DEFINITIONS                    ==*/
/*==                                                                        ==*/
/*============================================================================*/

/*  Define Geometry type array                                  */
#define MAX_GEOTYPE             12
static char *geo_type[MAX_GEOTYPE] = {
/* 0 */    "ST_GEOMETRY",
/* 1 */    "ST_POINT",
/* 2 */    "ST_CURVE",
/* 3 */    "ST_LINESTRING",
/* 4 */    "ST_SURFACE",
/* 5 */    "ST_POLYGON",
/* 6 */    "ST_GEOMCOLLECTION",
/* 7 */    "ST_MULTIPOINT",
/* 8 */    "ST_MULTICURVE",
/* 9 */    "ST_MULTILINESTRING",
/* 10 */   "ST_MULTISURFACE",
/* 11 */   "ST_MULTIPOLYGON"
};


#ifdef __cplusplus
}
#endif

#endif /* #ifndef DB2GSE_H_DEFINED */

