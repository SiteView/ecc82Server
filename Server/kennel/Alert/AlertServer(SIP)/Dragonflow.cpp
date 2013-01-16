/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Wed Aug 02 13:54:26 2006
 */
/* Compiler settings for C:\DOCUME~1\dragon\LOCALS~1\Temp\IDL55.tmp:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
#include "stdafx.h"
#include "Dragonflow.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

const IID LIBID_Dragonflow = {0xCA5D2436,0xEA6F,0x4528,{0xAC,0x81,0x74,0xB4,0x6F,0x0E,0x6E,0x6C}};


const IID IID__Alert = {0xA9F30FE7,0x2D11,0x4BB7,{0x83,0xA9,0x5C,0xDB,0x8C,0x39,0x3B,0x13}};


const CLSID CLSID_Alert = {0x45F69F49,0x5A38,0x4820,{0xAA,0xD9,0xDF,0x7B,0xFF,0xF8,0x11,0xDB}};


#ifdef __cplusplus
}
#endif

