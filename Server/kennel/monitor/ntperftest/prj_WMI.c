/* this file contains the actual definitions of */
/* the IIDs and CLSIDs */

/* link this file in with the server and any clients */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Jul 03 10:39:39 2006
 */
/* Compiler settings for C:\DOCUME~1\dragon\LOCALS~1\Temp\IDL1FE.tmp:
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

const IID LIBID_project1 = {0x4B89813B,0xC4DC,0x458F,{0xA3,0x4F,0xB7,0x30,0x54,0x29,0xDA,0x92}};//2E344B3B-6FE5-4BBC-9217-FB96755D724D


const IID IID__dragonflow = {0xF42D1DBC,0x9717,0x4F33,{0x90,0x98,0x0C,0x7C,0x17,0xEA,0x51,0x03}};//77819406-B86F-4E0B-B757-143A53C3A970


const CLSID CLSID_dragonflow = {0xB36D7067,0x5C01,0x4FEE,{0x85,0x09,0x14,0x21,0x61,0x61,0xE1,0x68}};//0E774AC6-E331-4159-8B7F-AA7306DF69BA


#ifdef __cplusplus
}
#endif

