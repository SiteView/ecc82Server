// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__A50C0197_DE2E_4202_9C26_89CC2A269CD2__INCLUDED_)
#define AFX_STDAFX_H__A50C0197_DE2E_4202_9C26_89CC2A269CD2__INCLUDED_

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

#define		CPU_TYPE_MONITOR		0x01
#define		MEMORY_TYPE_MONITOR		0x02
#define		DISK_TYPE_MONITOR		0x03
#define		SERVICE_TYPE_MONITOR	0x04
#define		USER_CPU_MONITOR     	0x09
#define		FILE_TYPE_MONITOR		0x15
#define		DISKS_TYPE_MONITOR		0x05
#define		PMEMORY_TYPE_MONITOR	0x12


// �������������ڻ�ȡaix��solaris�����ڴ���Ϣ�� ���� 2009.02.25 ��
#define     TOTAL_PMEMORY                   0x20
#define     FREE_PMEMORY                    0x21

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A50C0197_DE2E_4202_9C26_89CC2A269CD2__INCLUDED_)
