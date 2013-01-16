/*_############################################################################
  _## 
  _##  snmperrs.h  
  _##
  _##  SNMPv3.2.15
  _##  -----------------------------------------------
  _##  Copyright (c) 2001-2004 Jochen Katz, Frank Fock
  _##
  _##  This software is based on SNMP2.6 from Hewlett Packard:
  _##  
  _##    Copyright (c) 1996
  _##    Hewlett-Packard Company
  _##  
  _##  ATTENTION: USE OF THIS SOFTWARE IS SUBJECT TO THE FOLLOWING TERMS.
  _##  Permission to use, copy, modify, distribute and/or sell this software 
  _##  and/or its documentation is hereby granted without fee. User agrees 
  _##  to display the above copyright notice and this license notice in all 
  _##  copies of the software and any documentation of the software. User 
  _##  agrees to assume all liability for the use of the software; 
  _##  Hewlett-Packard and Jochen Katz make no representations about the 
  _##  suitability of this software for any purpose. It is provided 
  _##  "AS-IS" without warranty of any kind, either express or implied. User 
  _##  hereby grants a royalty-free license to any and all derivatives based
  _##  upon this software code base. 
  _##  
  _##  Stuttgart, Germany, Tue Jan  4 21:42:42 CET 2005 
  _##  
  _##########################################################################*/
/*===================================================================

  Copyright (c) 1999
  Hewlett-Packard Company

  ATTENTION: USE OF THIS SOFTWARE IS SUBJECT TO THE FOLLOWING TERMS.
  Permission to use, copy, modify, distribute and/or sell this software
  and/or its documentation is hereby granted without fee. User agrees
  to display the above copyright notice and this license notice in all
  copies of the software and any documentation of the software. User
  agrees to assume all liability for the use of the software; Hewlett-Packard
  makes no representations about the suitability of this software for any
  purpose. It is provided "AS-IS without warranty of any kind,either express
  or implied. User hereby grants a royalty-free license to any and all
  derivatives based upon this software code base.


  SNMP S N M P E R R S. H

  SNMP ERROR CODE AND STRING DEFINITIONS

  DESCRIPTION:
  Definition of error macros and error strings

  DESIGN + AUTHOR:
  Jeff Meyer

  LANGUAGE:
  ANSI C++

  OPERATING SYSTEMS:
  WIN32
  BSD UNIX

============================================================================*/
// $Id: snmperrs.h,v 1.5 2004/03/03 23:11:21 katz Exp $

#ifndef _SNMPERRS_H
#define _SNMPERRS_H

#include "snmp_pp/config_snmp_pp.h"

#ifdef SNMP_PP_NAMESPACE
namespace Snmp_pp {
#endif

//-------[ Positive SNMP ++ Error Return Codes ]------------------------------
/** @name Pdu error codes
 * These values are error status values from RFC 1905
 *
 * The values can be returned via Pdu::get_error_status()
 */
//@{
#define SNMP_ERROR_SUCCESS           0 //!< Success Status
#define SNMP_ERROR_TOO_BIG           1 //!< Pdu encoding too big
#define SNMP_ERROR_NO_SUCH_NAME	     2 //!< No such VB name, see error index
#define SNMP_ERROR_BAD_VALUE         3 //!< Bad Vb
#define SNMP_ERROR_READ_ONLY	     4 //!< VB is read only, see error index
#define SNMP_ERROR_GENERAL_VB_ERR    5 //!< General VB error, see error index
#define SNMP_ERROR_NO_ACCESS         6 //!< No access to MIBs data
#define SNMP_ERROR_WRONG_TYPE        7 //!< Requested type was incorrect
#define SNMP_ERROR_WRONG_LENGTH      8 //!< Request Pdu has inccorect length
#define SNMP_ERROR_WRONG_ENCODING    9 //!< Request Pdu has wrong encoding
#define SNMP_ERROR_WRONG_VALUE      10 //!< Request Pdu has wrong value
#define SNMP_ERROR_NO_CREATION      11 //!< Unable to create object specified
#define SNMP_ERROR_INCONSIST_VAL    12 //!< Inconsistent value in request
#define SNMP_ERROR_RESOURCE_UNAVAIL 13 //!< Resources unavailable
#define SNMP_ERROR_COMITFAIL        14 //!< Unable to comit
#define SNMP_ERROR_UNDO_FAIL        15 //!< Unable to undo
#define SNMP_ERROR_AUTH_ERR         16 //!< Authorization error
#define SNMP_ERROR_NOT_WRITEABLE    17 //!< Mib Object not writeable
#define SNMP_ERROR_INCONSIS_NAME    18 //!< Inconsistent naming used
//@}

//-------[ Negative SNMP ++ Result/Error Return Codes ]-------------------

/** @name Snmp class return codes
 */
//@{

// General
#define SNMP_CLASS_SUCCESS             0 //!< success
#define SNMP_CLASS_ERROR              -1 //!< general error
#define SNMP_CLASS_RESOURCE_UNAVAIL   -2 //!< e.g., malloc failed
#define SNMP_CLASS_INTERNAL_ERROR     -3 //!< unexpected / internal error
#define SNMP_CLASS_UNSUPPORTED        -4 //!< unsupported function

// Callback reasons:
#define SNMP_CLASS_TIMEOUT           -5 //!< outstanding request timed out
#define SNMP_CLASS_ASYNC_RESPONSE    -6 //!< received response for outstd request
#define SNMP_CLASS_NOTIFICATION      -7 //!< received notification (trap/inform)
#define SNMP_CLASS_SESSION_DESTROYED -8 //!< snmp::destroyed with oustanding reqs pending
					
// Snmp Class:
#define SNMP_CLASS_INVALID           -10 //!< snmp::mf called on invalid instance
#define SNMP_CLASS_INVALID_PDU       -11 //!< invalid pdu passed to mf
#define SNMP_CLASS_INVALID_TARGET    -12 //!< invalid target passed to mf
#define SNMP_CLASS_INVALID_CALLBACK  -13 //!< invalid callback to mf
#define SNMP_CLASS_INVALID_REQID     -14 //!< invalid request id to cancel
#define SNMP_CLASS_INVALID_NOTIFYID  -15 //!< missing trap/inform oid
#define SNMP_CLASS_INVALID_OPERATION -16 //!< snmp operation not allowed for specified target
#define SNMP_CLASS_INVALID_OID	     -17 //!< invalid oid passed to mf
#define SNMP_CLASS_INVALID_ADDRESS   -18 //!< invalid address passed to mf
#define SNMP_CLASS_ERR_STATUS_SET    -19 //!< agent returned response pdu with error_status set

// Transport Errors:
#define SNMP_CLASS_TL_UNSUPPORTED    -20 //!< transport unsupported
#define SNMP_CLASS_TL_IN_USE         -21 //!< transport in use
#define SNMP_CLASS_TL_FAILED         -22 //!< transport operation failed

// extras
#define SNMP_CLASS_SHUTDOWN          -23 //!< used for back door shutdown

// ASN.1 parse errors
#define SNMP_CLASS_BADVERSION        -50 //!< unsupported version
#define SNMP_CLASS_ASN1ERROR         -51 //!< used for ASN.1 parse errors
//@}

#define MAX_POS_ERROR                    SNMP_ERROR_INCONSIS_NAME
#define MAX_NEG_ERROR                    SNMP_CLASS_SHUTDOWN


#ifdef _INCLUDE_SNMP_ERR_STRINGS

/**
 * ASCII strings returned through Snmp::error() function.
 *
 * @note altering the strings in this header file will not affect the
 *       return values of Snmp::error(), unless you rebuild the SNMP
 *       library from source.
 */
//@{
static const char * pErrs[] = {
  "Success",                                                  // 0
  "Siteview:SNMP: Response PDU Too Big",                               // 1
  "Siteview:SNMP: Variable does not exist",  	      	              // 2
  "Siteview:SNMP: Cannot modify variable: Bad Value",                  // 3
  "Siteview:SNMP: Cannot modify object, Read Only",  	              // 4
  "Siteview:SNMP: Cannot perform operation, General Error",            // 5
  "Siteview:SNMP: Cannot access variable, No Access",                  // 6
  "Siteview:SNMP: Cannot create/modify variable, Wrong Type",          // 7
  "Siteview:SNMP: Cannot create/set variable, Wrong Length",           // 8
  "Siteview:SNMP: Cannot create/set variable, Wrong Encoding",         // 9
  "Siteview:SNMP: Cannot create/set variable, Wrong Value",            // 10
  "Siteview:SNMP: Cannot create variable, Creation Not Allowed",       // 11
  "Siteview:SNMP: Cannot create/set variable, Inconsistent Value",     // 12
  "Siteview:SNMP: Cannot create/set variable, Resource Unavailable",   // 13
  "Siteview:SNMP: Cannot create/set variable, Commit Failed",          // 14
  "Siteview:SNMP: Cannot create/set variable, Undo Failed",            // 15
  "Siteview:SNMP: Cannot perform operation, Authorization Error",      // 16
  "Siteview:SNMP: Cannot create/set variable, Not Writable",           // 17
  "Siteview:SNMP: Cannot create variable, Inconsistent Name",          // 18
  "Siteview:SNMP: Unknown Error Status"                                // 19
};

#ifdef _SNMPv3
static const char * nv3Errs[] = {
  "Siteview:SNMPv3: v3MP error",                 // -1400
  "Siteview:SNMPv3: v3MP ok",                    // -1401
  "Siteview:SNMPv3: Unsupported Security Model", // -1402
  "Siteview:SNMPv3: Message not in Time Window", // -1403
  "Siteview:SNMPv3: received same Message twice",// -1404
  "Siteview:SNMPv3: Invalid Message",            // -1405
  "Siteview:SNMPv3: Invalid EngineID",           // -1406
  "Siteview:SNMPv3: v3MP not initialized",       // -1407
  "Siteview:SNMPv3: Parse Error",                // -1408
  "Siteview:SNMPv3: Received Message with unknown MsgID",  // -1409
  "Siteview:SNMPv3: Message does not match known message", // -1410
  "Siteview:SNMPv3: Community format error",     // -1411
  "Siteview:SNMPv3: Unknown UserName",           //-1412
  "Siteview:SNMPv3: Build error",                //-1413
  "Siteview:SNMPv3: USM: error",                 //-1414
  "Siteview:SNMPv3: Unknown pdu handlers",       //-1415
  "Siteview:SNMPv3: Unavailable Context",        //-1416
  "Siteview:SNMPv3: Unknown Context",            //-1417
  "Siteview:SNMPv3: Report sent",                //-1418
  "Siteview:SNMPv3: Unknown errorcode"
};

static const char * pv3Errs[] = {
  "Siteview:SNMPv3: USM: ok",                          // 1400
  "Siteview:SNMPv3: USM: error",                       // 1401
  "Siteview:SNMPv3: USM: Configfile write error",      // 1402
  "Siteview:SNMPv3: USM: Unsupported SecurityLevel",   // 1403
  "Siteview:SNMPv3: USM: Unknown SecurityName",        // 1404
  "Siteview:SNMPv3: USM: Encryption error",            // 1405
  "Siteview:SNMPv3: USM: Decryption error",            // 1406
  "Siteview:SNMPv3: USM: Authentication error",        // 1407
  "Siteview:SNMPv3: USM: Authentication failure",      // 1408
  "Siteview:SNMPv3: USM: Parse error",                 // 1409
  "Siteview:SNMPv3: USM: Unknown EngineID",            // 1410
  "Siteview:SNMPv3: USM: Message not in TimeWindow",   // 1411
  "Siteview:SNMPv3: USM: Unsupported AuthProtocol",    // 1412
  "Siteview:SNMPv3: USM: Unsupported PrivProtocol"     // 1413
  "Siteview:SNMPv3: USM: Address error",               // 1414
  "Siteview:SNMPv3: USM: Could not create file",       // 1415
  "Siteview:SNMPv3: USM: Could not open file",         // 1416
  "Siteview:SNMPv3: USM: Could not rename file",       // 1417
  "Siteview:SNMPv3: USM: Could not delete file",       // 1418
  "Siteview:SNMPv3: USM: Could not write into file",   // 1419
  "Siteview:SNMPv3: USM: Could not read from file",    // 1420
  "Siteview:SNMPv3: USM: unknown errorcode"
};
#endif

static const char * nErrs[] =
{
  // General:
  "Siteview:SNMP: Success",                 // 0  SNMP_CLASS_SUCCESS
  "Siteview:SNMP: Operation failed",        // 1  SNMP_CLASS_ERROR
  "Siteview:SNMP: Resource unavailable",    // 2  SNMP_CLASS_RESOURCE_UNAVAIL
  "Siteview:SNMP: Internal error",          // 3  SNMP_CLASS_INTERNAL_ERROR
  "Siteview:SNMP: Unsupported function",    // 4  SNMP_CLASS_UNSUPPORTED

  // Callback reasons:
  "Siteview:SNMP: SNMP request timed out",  // 5  SNMP_CLASS_TIMEOUT
  "Siteview:SNMP: Received SNMP Response",  // 6  SNMP_CLASS_ASYNC_RESPONSE
                                     // 7  SNMP_CLASS_NOTIFICATION	
  "Siteview:SNMP: Received SNMP Notification (trap or inform)",
                                     // 8  SNMP_CLASS_SESSION_DESTROYED
  "Siteview:SNMP: Closing session with outstanding requests",
  "Siteview:Unknown error code",              // 9  reserved for future

  // Snmp Class errors:
  "Siteview:SNMP: Class not valid",           // 10 SNMP_CLASS_INVALID
  "Siteview:SNMP: Invalid Pdu",               // 11 SNMP_CLASS_INVALID_PDU
  "Siteview:SNMP: Invalid Target",            // 12 SNMP_CLASS_INVALID_TARGET
  "Siteview:SNMP: Invalid (null) Callback Function", // 13 SNMP_CLASS_INVALID_CALLBACK
  "Siteview:SNMP: Invalid Request Id",        // 14 SNMP_CLASS_INVALID_REQID
  "Siteview:SNMP: Invalid Notification Id",   // 15 SNMP_CLASS_INVALID_NOTIFYID
                                       // 16 SNMP_CLASS_INVALID_OPERATION
  "Siteview:SNMP: SNMP Operation not supported on specified Target",
  "Siteview:SNMP: Invalid Object Identifier", // 17 SNMP_CLASS_INVALID_OID
  "Siteview:SNMP: Invalid Address",           // 18 SNMP_CLASS_INVALID_ADDRESS
                                       // 19 SNMP_CLASS_ERR_STATUS_SET
  "Siteview:SNMP: Agent indicates error in SNMP request",

  // Transport Errors:
  "Siteview:SNMP: Transport is not supported",  // 20 SNMP_CLASS_TL_UNSUPPORTED
  "Siteview:SNMP: Transport is in use",         // 21 SNMP_CLASS_TL_IN_USE
  "Siteview:SNMP: Transport operation failed",  // 22 SNMP_CLASS_TL_FAILED
  "Siteview:SNMP: Blocked Mode Shutdown",       // 23 SNMP_CLASS_SHUTDOWN

  "Siteview:Unknown error code",  // unknown error code
};
//@}
#endif //_INCLUDE_SNMP_ERR_STRINGS

#ifdef SNMP_PP_NAMESPACE
}; // end of namespace Snmp_pp
#endif 

#endif //_SNMPERRS_H
