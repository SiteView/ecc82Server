/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Tue Oct 10 09:00:59 2006
 */
/* Compiler settings for ExchangeWmi.IDL:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )
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

const IID LIBID_ExchangeWmi = {0x615675B3,0x8AA3,0x4D90,{0xAB,0xC1,0xFB,0xAA,0x44,0x5A,0xAB,0xBD}};


const IID IID__Exchange = {0x8DEFCA17,0x0900,0x47B0,{0x92,0xFE,0x21,0x63,0xC4,0x80,0x03,0x08}};


const CLSID CLSID_Exchange = {0x51CB4772,0xBD8E,0x4673,{0xAA,0x36,0xF6,0x46,0x2D,0xF6,0x68,0xC8}};


#ifdef __cplusplus
}
#endif

