// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#pragma once


#include <iostream>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将为显式的

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// 从 Windows 头中排除极少使用的资料
#endif

#include <afx.h>
#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展
#include <afxdtctl.h>		// MFC 对 Internet Explorer 4 公共控件的支持
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "afxinet.h"

// 2007/7/18 龚超 为了通过输出调试跟踪代码运行时错误而引用StringHelper.h中的调试信息输出工具
// 使用COutputDebugString类输出调试信息的宏标志
// 如果注释该标志则项目中所有通过ODS_DEFINE和ODS_OUTDEBUG输出调试信息的代码在编译的时候都将被忽略，
// 调试信息也将不再输出。
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
