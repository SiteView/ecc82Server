// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once


#include <iostream>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯����Ϊ��ʽ��

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include <afx.h>
#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#include <afxdtctl.h>		// MFC �� Internet Explorer 4 �����ؼ���֧��
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "afxinet.h"

// 2007/7/18 ���� Ϊ��ͨ��������Ը��ٴ�������ʱ���������StringHelper.h�еĵ�����Ϣ�������
// ʹ��COutputDebugString�����������Ϣ�ĺ��־
// ���ע�͸ñ�־����Ŀ������ͨ��ODS_DEFINE��ODS_OUTDEBUG���������Ϣ�Ĵ����ڱ����ʱ�򶼽������ԣ�
// ������ϢҲ�����������
//#define ODS_USING 

#pragma warning (disable:4098)
// TODO: reference additional headers your program requires here

#define OUT_DEBUG(str) { \
	CString strDbg; \
	strDbg = str; \
	strDbg += " as_debug"; \
	strDbg.Replace('\r', ' '); \
	strDbg.Replace('\n', ' '); \
	::OutputDebugString(strDbg); \
}

#import "msxml3.dll" named_guids raw_interfaces_only
#import "mqoa.dll" no_namespace, named_guids
