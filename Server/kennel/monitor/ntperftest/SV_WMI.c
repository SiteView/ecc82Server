/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Nov 21 11:40:14 2005
 */
/* Compiler settings for C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\IDL86.tmp:
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

const IID LIBID_sv_wmi = {0xCC74EC46,0x10CC,0x4B6A,{0xB2,0x0D,0x86,0xB0,0xE5,0x35,0xAD,0xBB}};


const IID IID__clsWMI = {0x1F95EF5F,0x5B3C,0x49F3,{0xBD,0xBD,0x15,0x0A,0x03,0x1B,0x79,0xEE}};


const CLSID CLSID_clsWMI = {0x5BCCACE8,0x9100,0x422E,{0x8E,0xDB,0xA8,0xF2,0x49,0xD8,0x60,0x51}};


#ifdef __cplusplus
}
#endif

