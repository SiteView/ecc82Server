// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__423714D7_47AD_4FA1_8AEE_E0A4AABC01F0__INCLUDED_)
#define AFX_STDAFX_H__423714D7_47AD_4FA1_8AEE_E0A4AABC01F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//#define		KEY_PATH						"SOFTWARE\\Aim"
//#define		KEY_NAME						"RootPath"


#define		CPU_TYPE_MONITOR				0x01
#define		MEMORY_TYPE_MONITOR				0x02
#define		DISK_TYPE_MONITOR				0x03
#define		SERVICE_TYPE_MONITOR			0x04

#define		DISKS_TYPE_MONITOR				0x05
#define		FILE_TYPE_MONITOR				0x06
#define		DISK_ACTIVITY					0x07
#define		DISK_ACTIVITY_MONITOR			0x08

#define		PMEMORY_TYPE_MONITOR			0x12

#define		HP_CPU_TYPE_MONITOR				0x13

#include <afxsock.h>
#include <afxinet.h>
#include <afxpriv.h>
#include <afxmt.h>
#include <afxtempl.h>

#include <io.h>
#include <errno.h>
#include <math.h>
#include <windows.h> 
#include <windowsx.h>

//#define		BUF_SIZE			100 * 1024 //Old Defined
//#define		BUF_SIZE			500 * 1024 //Edit 2005-04-13
#define		BUF_SIZE			1000 * 1024 //Edit 2005-05-16
//////////////////////////////////////////////////////////////chen
typedef struct st_gd{
	int		term_index;
	char	databuffer[BUF_SIZE];
	int		datalen;
	char	codebuf[256];
	unsigned char codeptr;
}GDATA,*LPGDATA;
//////////////////////////////////////////////////////////////

typedef BOOL(PARSER_ZW)(const char * szSource, const int nMonitorType, char *szOut, const char *FileName, const char* szTotalMem);
typedef BOOL(PARSER)(const char * szSource, const int nMonitorType, char *szOut, const char *FileName);
typedef BOOL(PARSER2)(const char * szSource, const int nMonitorType, char *szOut, const char* szProcName, const char *FileName);
PARSER *ReturnParser(int nMonitorType);
PARSER2 *ReturnParser2(int nMonitorType);

typedef unsigned int uint32;
typedef unsigned int word32;

#include <fstream>

void WriteDebugString(LPCTSTR pszString, bool bIsMS, int nNum, LPCTSTR pszFile, int nLine);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__423714D7_47AD_4FA1_8AEE_E0A4AABC01F0__INCLUDED_)
