/*//////////////////////////////////////////////////////////////////////////////////////////////////
Copyright (c) 2005, DragonFlow Corporation All rights reserved

项目名称：
　　ECC监测器共用代码

文件名：
    StringHelper.h

依赖：
    <文件名1>(<文件版本号>)
    <文件名2>(<文件版本号>)
    <文件名n>(<文件版本号>)
    
文件描述：
    该文件实现了一些在ECC监测器开发、调试过程中经常用到的字符串相关处理工具类，初期版本中包括调试信
	息输出、简单字符串处理和监测函数调用参数串解析等。
	该文件实现了所有代码，无需其他文件支持，只要include该文件即可使用所有功能。
	该文件使用SH命名空间。

版本号：
    Ver 1.4

修改历史：
    Ver 0.1 (龚超 2007/7/6)
        初稿，实现了监测器调用参数分析器类CParamsParser和调试信息输出帮助类COutputDebugString。
    Ver 0.2 (龚超 2007/7/18)
        新增了带词法分析的_bstr_t类型字符串分段器类CAnalyzer4_bstr_t。
    Ver 0.3 (龚超 2007/7/20)
        1. 修改了CParamsParser::_SplitStrings实现，增加了限制切分数量的参数。
        2. 基于对CParamsParser::_SplitStrings修改，调整CParamsParser::Analysis的算法实现了对包含分隔
           符参数的正确解析。例如：修改前arg1=123=456会被解析为[参数名:arge, 参数值:123]，修改后同样
           的字符串则可以被解析为[参数名:arge, 参数值:123=456]。
        进行上述修改是为了解决监测器入口参数中出现多个=符号而影响监测器工作的问题。
    Ver 1.4 (龚超 2007/7/31)
        调整了COutputDebugString的构造函数参数并且增加了新的编译宏ODS_SHOW_TICKCOUNT。
//////////////////////////////////////////////////////////////////////////////////////////////////*/

#pragma once

#pragma warning(disable:4786)

#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <comdef.h>

#ifndef LPCTSTR
#	include <WINNT.h>
#endif // ifndef LPCTSTR

#include <tchar.h>

// 该标志决定是否在输出调试信息的时候根据同一线程内嵌套层次进行缩进
// 这个功能对于跟踪函数调用路径很有帮助，如果在相关函数的入口处定义
// COutputDebugString对象并调用COutputDebugString对象的OutputDebugString
// 输出一个指示当前所在函数调试信息，那么在调试信息观察窗口中将以缩
// 进的方式逐级显示调试信息，没加深一层函数调用，调试信息则多缩进一
// 半角空格。
#ifndef ODS_OUTPUT_INDENT
#	define ODS_OUTPUT_INDENT 1
#endif // ifndef ODS_OUTPUT_INDENT

// 该标志决定在输出调试信息的时候是否在前缀中显示线程ID
#ifndef ODS_SHOW_THREADID
#	define ODS_SHOW_THREADID 0
#endif // ifndef ODS_SHOW_THREADID

// 该标志决定在输出调试信息的时候是否在前缀中显示调用OutputDebugString时的TickCount值。
#ifndef ODS_SHOW_TICKCOUNT
#	define ODS_SHOW_TICKCOUNT 0
#endif // ifndef ODS_SHOW_TICKCOUNT

#ifdef ODS_USING // 使用ODS代码输出调试信息

/*
// ODS宏使用示例代码
int func2(void)
{
	int n = 100;
	ODS_OUTDEBUGD("return %d", n); // 直接输出调试信息
	return n;
}
void func(void)
{
	ODS_DEFINE(" key"); // 定义一个COutputDebugString对象，后续代码可以通过这个对象输出调试信息。
	ODS_OUTDEBUG("call func ..."); // 通过ODS_DEFINE定义的对象输出调试信息

	int a = func2();
	ODS_OUTDEBUG("a = %d", a); // 通过ODS_DEFINE定义的对象输出调试信息

	LPCTSTR pszDbg = "string1\0 string2\0";
	ODS_OUTDEBUGM(pszDbg, " key"); // 直接输出一个多字符串，输出时多字符串的空字符('\0')将被替换成空格字符' '
}
*/

// 定义COutputDebugString对象
#	define ODS_DEFINE			SH::COutputDebugString __ods
// 通过ODS_DEFINE定义的对象输出调试信息
#	define ODS_OUTDEBUG			__ods.OutputDebugString
// 不需要ODS_DEFINE定义的支持直接输出调试信息
#	define ODS_OUTDEBUGD		SH::COutputDebugString::OutputDebugStringD
// 不需要ODS_DEFINE定义的支持直接输出一个多字符串调试信息
#	define ODS_OUTDEBUGM		SH::COutputDebugString::OutputDebugMString

#else // 屏蔽ODS调试信息输出代码

#	define ODS_DEFINE			//
#	define ODS_OUTDEBUG			//
#	define ODS_OUTDEBUGD		//
#	define ODS_OUTDEBUGM		//

#endif // ifdef ODS_USING

namespace SH // StringHelper 避免命名冲突
{
	/*//////////////////////////////////////////////////////////////////////////////////////////////
	功能简述：
		带词法分析的_bstr_t类型字符串分段器。该类实现对_bstr_t类型的字符串进行分段处理，分段过程不会
		截断英文单词，也不会将一个中文分成两半。
	//////////////////////////////////////////////////////////////////////////////////////////////*/
	class CAnalyzer4_bstr_t
	{
	public:
		// 构造函数
		// 功能简述：
		//     在对象构造时决定可以做为分段断点的字符集合。
		// 参数：
		//     strBreakpointChars [in] - 指定可以做为断点的字符集合。
		CAnalyzer4_bstr_t(_bstr_t strBreakpointChars = _T(" ,.-;!?/\\"))
		{
			m_strBreakpointChars = strBreakpointChars;
		}

		// 对字符串进行分段处理
		// 功能简述：
		//     对一个_bstr_t字符串进行分段处理，分段结果存入对象的结果集里面。
		// 参数：
		//     strTagString [in] - 待分段的_bstr_t字符串对象。
		//     nMaxChars [in] - 每个分段的最大字符数。
		// 返回值：
		//     返回分段数量。
		int Analyzer(_bstr_t strTagString, int nMaxChars)
		{
			m_lsResults.clear();
			LPCWSTR pszTagString = strTagString;
			while(NULL != pszTagString && *pszTagString != WCHAR('\0'))
			{
				pszTagString = _SplitToResults(pszTagString, min((int)::wcslen(pszTagString), nMaxChars));
			}
			return getResultsCount();
		}

		// 取分段结果
		// 功能简述：
		//     从当前对象结果集中取指定索引位置的分段字符串。
		// 参数：
		//     nIndex [in] - 指定分段字符串的索引值。
		// 返回值：
		//     如果成功则返回nIndex指定的分段字符串，否则返回一个空字符串。
		_bstr_t GetResult(int nIndex)
		{
			_bstr_t strResult;
			if (nIndex >= getResultsCount())
				return strResult;
			std::list<_bstr_t>::iterator it = m_lsResults.begin();
			for (int i = 0; i < nIndex && it != m_lsResults.end(); i++)
			{
				it++;
			}
			if (it != m_lsResults.end())
				strResult = *it;
			return strResult;
		}

		// 分段数量属性
		// 功能简述：
		//     返回当前对象结果集中分段字符串的数量。
		// 返回值：
		//     返回分段数量值。
		int getResultsCount(void)
		{
			return int(m_lsResults.size());
		}
	protected:

		// 判断一个宽字符是否可以做为分段断点字符
		bool _IsBreakpointChar(WCHAR c)
		{
			if (c >> 8 || c == WCHAR('\0'))
				return true;
			LPCWSTR pszBCs = m_strBreakpointChars;
			while(*pszBCs)
			{
				if (*pszBCs == c)
					return true;
				++pszBCs;
			}
			return false;
		}

		// 将指定宽字符串的部分子串存入分析结果集
		bool _AddToResults(LPCWSTR pszTagString, int nChars)
		{
			LPWSTR pszChars = new WCHAR[nChars + 1];
			if (NULL == pszChars)
				return false;
			::wcsncpy(pszChars, pszTagString, nChars);
			pszChars[nChars] = WCHAR('\0');
			_bstr_t strChars = pszChars;
			m_lsResults.push_back(strChars);
			delete[] pszChars;
			return true;
		}

		// 反向搜索可截断位置
		int _ReverseFindBreakpoint(LPCWSTR pszTagString, int nStartPos)
		{
			int nBPos = nStartPos;

			LPCWSTR pszRet = pszTagString + nStartPos;
			while(nBPos > 0)
			{
				if (_IsBreakpointChar(pszTagString[nBPos]))
				{
					nBPos++;
					break;
				}
				--nBPos;
			}
			return nBPos;
		}

		// 将指定宽字符串按照指定最大长度截断并存入分析结果集
		LPCWSTR _SplitToResults(LPCWSTR pszTagString, int nMaxChars)
		{
			LPCWSTR pszRet = NULL;
			if (NULL == pszTagString || pszTagString[0] == WCHAR('\0'))
				return pszRet;
			if (_IsBreakpointChar(pszTagString[nMaxChars]) ||
				_IsBreakpointChar(pszTagString[nMaxChars - 1]))
			{// 可以直接分段
				if (_AddToResults(pszTagString, nMaxChars))
					pszRet = pszTagString + nMaxChars;
			}
			else
			{
				int nBPos = _ReverseFindBreakpoint(pszTagString, nMaxChars - 1);
				if (nBPos > 0)
				{// 反向搜索到了可以截断的位置...

					if (_AddToResults(pszTagString, nBPos))
						pszRet = pszTagString + nBPos;
				}
			}
			return pszRet;
		}

		// 可以做为断点的字符集合
		_bstr_t m_strBreakpointChars;
		// 分段字符串结果集
		std::list<_bstr_t> m_lsResults;

	}; // class CAnalyzer4_bstr_t

	/*//////////////////////////////////////////////////////////////////////////////////////////////
	功能简述：
		调试信息帮助类。借助该类，可以更灵活的输出调试信息，例如可以像使用printf函数一样输出调试信息
		还能输出按照线程调用路径自动缩进的格式化调试信息。
	//////////////////////////////////////////////////////////////////////////////////////////////*/
	class COutputDebugString
	{
	public:
		// 构造函数
		// 功能简述：
		//     在对象构造时设置缺省的调试信息关键字和TickCount值显示标志。
		// 参数：
		//     pszKey [in] - 指定一个会出现在每一个调试输出信息尾部的关键字串。
		COutputDebugString(LPCTSTR pszKey = _T("\n"))
		{
			m_nThisThreadLeve = _ThreadLeve(::GetCurrentThreadId(), true);

			const int MAX_BUF = 128;
			TCHAR szBuf[MAX_BUF] = {0};

#if ODS_SHOW_THREADID > 0 // 指定显示构建当前COutputDebugString对象的线程ID。

			::_sntprintf(szBuf, MAX_BUF, _T("(%d) "), ::GetCurrentThreadId());
			m_strPrefix = szBuf;

#endif // if ODS_SHOW_THREADID > 0

			if (NULL != pszKey)
				m_strKey = pszKey;
		}

		virtual ~COutputDebugString()
		{
			_ThreadLeve(::GetCurrentThreadId(), false);
		}

		// 直接输出多字符串调试信息
		// 功能简述：
		//     直接输出（不依赖COutputDebugString实例）一个多字符串调试信息，多字符串在监测器开发中
		//     经常用到，例如：任何一个监测调用函数的入口参数和返回值都是一个多字符串。多字符串以两
		//     个'\0'表示结束，可以包含多个以'\0'结束的字符串。从调试端口输出一组多字符串时，多字符
		//     串之间的'\0'将被替换为' '。
		// 参数：
		//     pMString [in] - 指定一组多字符串。
		//     pszKey [in] - 指定显示在调试信息尾部的关键字字符串。
		// 返回值：
		//     如果成功返回NO_ERROR，否则返回标准错误值。
		static int OutputDebugMString(LPCTSTR pMString, LPCTSTR pszKey = NULL)
		{
			int nRetVal = NO_ERROR;

			// 准备好调试信息输出缓冲区
			TCHAR szDbgStr[COutputDebugString::MAX_DEBUG_STRING];
			::memset(szDbgStr, 0, sizeof(TCHAR) * MAX_DEBUG_STRING);
			LPTSTR pszDbgStr = szDbgStr;
			int nMaxDebugString = MAX_DEBUG_STRING;

			LPCTSTR p = pMString;
			while(*p)
			{
				::lstrcpyn(pszDbgStr, p, nMaxDebugString);

				nMaxDebugString -= ::lstrlen(p);
				pszDbgStr += ::lstrlen(p);
				p += ::lstrlen(p) + 1;

				::lstrcpyn(pszDbgStr, _T(" "), nMaxDebugString);
				nMaxDebugString--;
				pszDbgStr++;
				if (nMaxDebugString <= 0)
					break;
			}

			if (nMaxDebugString > 0 && NULL != pszKey)
			{
				::lstrcpyn(pszDbgStr, pszKey, nMaxDebugString);
				pszDbgStr += ::lstrlen(pszKey);
				nMaxDebugString -= ::lstrlen(pszKey);
			}

			::OutputDebugString(szDbgStr);
			nRetVal = ::lstrlen(szDbgStr);

			return nRetVal;
		}

		// 以printf方式直接输出调试信息
		// 功能简述：
		//     以printf方式直接输出（不依赖COutputDebugString实例）调试信息，输出的调试信息完全与
		//     printf的输出相同。
		// 参数：
		//     pszFormat [in] - 指定一个输出格式字符串，其格式要求参见MSDN中printf函数的说明。
		// 返回值：
		//     如果成功返回NO_ERROR，否则返回标准错误值。
		static int OutputDebugStringD(LPCTSTR pszFormat, ...)
		{
			int nRetVal = NO_ERROR;

			// 准备好调试信息输出缓冲区
			TCHAR szDbgStr[COutputDebugString::MAX_DEBUG_STRING];
			::memset(szDbgStr, 0, sizeof(TCHAR) * MAX_DEBUG_STRING);
			LPTSTR pszDbgStr = szDbgStr;
			int nMaxDebugString = MAX_DEBUG_STRING;

			va_list arglist;
			va_start(arglist, pszFormat);

			nRetVal = _vsntprintf(
				pszDbgStr, 
				nMaxDebugString - 1, 
				pszFormat, 
				arglist);
			pszDbgStr += ::lstrlen(pszDbgStr);
			nMaxDebugString -= ::lstrlen(pszDbgStr);

			::OutputDebugString(szDbgStr);

			return nRetVal;
		}

		// 以printf方式输出调试信息
		// 功能简述：
		//     以printf方式输出调试信息。输出的调试信息中不仅仅包括调用参数中所指定的信息，还会附加
		//     前缀和后缀信息，详情参见附注。
		// 参数：
		//     pszFormat [in] - 指定一个输出格式字符串，其格式要求参见MSDN中printf函数的说明。
		// 返回值：
		//     如果成功返回NO_ERROR，否则返回标准错误值。
		// 附注：
		//     当COutputDebugString实例构造时，pszKey指定的关键字字符串将被做为后缀。
		//     如果编译宏ODS_SHOW_TICKCOUNT的值大于0，会在调试信息前缀中附加GetTickCount()返回值。
		//     如果编译宏ODS_OUTPUT_INDENT的值大于0，调试信息会根据同一个线程中的嵌套输出顺序进行自
		//     动缩进。
		//     如果编译宏ODS_SHOW_THREADID的值大于0，会在调试信息前缀中将附加线程ID值。
		int OutputDebugString(LPCTSTR pszFormat, ...)
		{
			int nRetVal = NO_ERROR;

			// 准备好调试信息输出缓冲区
			TCHAR szDbgStr[MAX_DEBUG_STRING];
			::memset(szDbgStr, 0, sizeof(TCHAR) * MAX_DEBUG_STRING);
			LPTSTR pszDbgStr = szDbgStr;
			int nMaxDebugString = MAX_DEBUG_STRING;

#if ODS_SHOW_TICKCOUNT > 0 // 指定在调试信息中添加TickCount值前缀

			::_sntprintf(pszDbgStr, nMaxDebugString, _T("%u "), ::GetTickCount());
			pszDbgStr += ::lstrlen(pszDbgStr);
			nMaxDebugString -= ::lstrlen(pszDbgStr);

#endif // if ODS_SHOW_TICKCOUNT > 0

			// 首先添加好前缀信息
			::lstrcpyn(
				pszDbgStr, 
				m_strPrefix.c_str(), 
				nMaxDebugString);
			pszDbgStr += ::lstrlen(pszDbgStr);
			nMaxDebugString -= ::lstrlen(pszDbgStr);

#if ODS_OUTPUT_INDENT > 0 // 指定根据同一个线程的嵌套输出进行缩进

			if (m_nThisThreadLeve > 1 && 
				pszFormat[0] != TCHAR('*') &&
				pszFormat[0] != TCHAR('~'))
			{// 根据当前线程的进入计数进行缩进

				long nIndent = min(m_nThisThreadLeve, long(nMaxDebugString - 1));
				for (long i = 1; i < nIndent; i++)
				{
					*pszDbgStr = TCHAR(' ');
					++pszDbgStr;
					--nMaxDebugString;
				}
			}

#endif // #if ODS_OUTPUT_INDENT > 0

			va_list arglist;
			va_start(arglist, pszFormat);

			nRetVal = _vsntprintf(
				pszDbgStr, 
				nMaxDebugString - 1, 
				pszFormat, 
				arglist);
			pszDbgStr += ::lstrlen(pszDbgStr);
			nMaxDebugString -= ::lstrlen(pszDbgStr);

			if (m_strKey.length() > 0)
			{
				::lstrcpyn(
					pszDbgStr, 
					m_strKey.c_str(), 
					nMaxDebugString);
				pszDbgStr += ::lstrlen(pszDbgStr);
				nMaxDebugString -= ::lstrlen(pszDbgStr);
			}

			LPTSTR p = szDbgStr;
			while(*p)
			{
				if (*p == TCHAR('\r') && p[1] != TCHAR('\0') && p[2] != TCHAR('\0') ||
					*p == TCHAR('\n') && p[1] != TCHAR('\0'))
					*p = TCHAR(' ');
				++p;
			}
			::OutputDebugString(szDbgStr);

			return nRetVal;
		}
	protected:
		enum { MAX_DEBUG_STRING = 1024 * 8 };
		std::string m_strPrefix; // 输出调试信息前缀
		std::string m_strKey; // 关键字，追加在调试信息尾部

		int m_nThisThreadLeve; // 当前对象在当前线程中的进入层次，其值将决定调试信息输出是的缩进量。

		// 注册或注销一个线程进入层级
		static int _ThreadLeve(DWORD uThreadId, bool bReg)
		{
			typedef std::pair<DWORD, int> int_pair;
			static std::map<DWORD, int> mapThreadsLeve;
			std::map<DWORD, int>::iterator it;
			if (bReg)
			{
				it = mapThreadsLeve.find(uThreadId);
				if (it != mapThreadsLeve.end())
				{
					it->second++;
					return it->second;
				}
				else
				{
					mapThreadsLeve.insert(int_pair(uThreadId, 1));
					return 1;
				}
			}
			else
			{
				it = mapThreadsLeve.find(uThreadId);
				if (it != mapThreadsLeve.end())
				{
					it->second--;
					int nLeve = it->second;
					if (it->second < 1)
						mapThreadsLeve.erase(uThreadId);
					return nLeve;
				}
				else
				{
					return 0;
				}
			}
		}
	}; // class COutputDebugString

	// 用于字符串表操作
	typedef std::pair<std::string, std::string> string_pair;

	// 清除指定字符串对象首尾的指定子串
	// 功能简述：
	//     清除指定字符串对象首尾的指定子串。
	// 参数：
	//     s [in, out] - 指定待处理的字符串。
	//     strTrim [in] - 指定要清楚的首尾子串
	// 返回值：
	//     返回处理以后的字符串对象，与入口参数传入的s是同一个字符串对象。
	inline std::string & Trim(std::string & s, std::string strTrim = " ")
	{
		if (s.empty())
		{
			return s;
		}

		s.erase(0, s.find_first_not_of(strTrim));
		s.erase(s.find_last_not_of(strTrim) + 1);
		return s;
	}


	// 搜索指定字符串在一个字符串列表中的索引
	// 功能简述：
	//     搜索指定字符串在一个字符串列表中的索引。
	// 参数：
	//     slStrs [in] - 指定要搜索的字符串表。
	//     strFind [in] - 指定要搜索的字符串。
	// 返回值：
	//     如果找到目标字符串则返回其在字符串表中的索引，否则返回-1。
	inline int Find(std::list<std::string> slStrs, std::string strFind)
	{
		int nPos = 0;
		for (std::list<std::string>::iterator it = slStrs.begin(); it != slStrs.end(); it++)
		{
			if (*it == strFind)
				return nPos;
			++nPos;
		}
		return -1;
	}

	// 计算一个子串在一个字符串中出现的次数
	// 功能简述：
	//     strString [in] - 指定可能包含子串的字符串。
	//     strSub [in] - 指定待搜索的子串。
	// 返回值：
	//     返回strSub在strString中出现的次数。
	inline int SubStringCount(std::string strString, std::string strSub)
	{
		int nCount = 0;
		std::string::size_type pos = 0;
		do
		{
			pos = strString.find(strSub, pos);
			if (int(pos) >= 0)
			{
				++nCount;
				pos += strSub.length();
			}
		} while(int(pos) >= 0);
		return nCount;
	}

	/*//////////////////////////////////////////////////////////////////////////////////////////////
	功能简述：
		参数型字符串分析器（适用于所有监测器入口参数字符串解析）
		用于分析在一个字符串中包含多个逻辑字符串的参数类应用
		例串：Param1=Value1 Param2="Value 2"
		例串如果把切隔符设置为空格，关联符设置为=，结束符设置为空，左右标识符设置为"，则例串
		将被解析为两组参数，参数名分别为Param1和Param2，对应的参数值则为Value1和Value 2
		对象缺省配置：
			切隔符：空字符"\0"
		左右标记符：双引号"\""
			关联符：等号"="
			结束符：双空字符"\0\0"
		对象的缺省配置可以直接用于解析监测器监测函数的调用参数解析。
	//////////////////////////////////////////////////////////////////////////////////////////////*/
	class CParamsParser
	{
	public:
		// 构造函数
		// 功能简述：
		//     在对象构造时用缺省配置进行参数字符串解析。
		// 参数：
		//     pszParamsString [in] - 指定待解析的参数字符串。
		//     nMaxChars [in] - 指定pszParamsString中字符的数量，指定0表示由分析器自行计算。
		CParamsParser(
			LPCTSTR pszParamsString,
			int nMaxChars = 0) 
		{
			_Init();
			if (NULL != pszParamsString)
				Analysis(pszParamsString, nMaxChars);
		}

		// 构造函数
		// 功能简述：
		//     在对象构造时用定制配置进行参数字符串解析。
		// 参数：
		//     slSplitStrings [in] - 指定用来切割原始字符串的切割标志字符串，空字符用可显示的"\0"做为转义字符。
		//     cTagLeft [in] - 指定字符串中用来限定完整子串的标记的左边部分，如'<'。
		//     cTagRight [in] - 指定字符串中用来限定完整子串的标记的右边部分，如'>'。
		//     cRelational [in] - 指定参数名和参数值之间的关联符，如'='。
		//     strEndTag [in] - 结束标记，空字符用可显示的"\0"做为转义字符。
		//     pszParamsString [in] - 指定待分析的参数字符串。
		//     nMaxChars [in] - 指定pszParamsString中字符的数量，指定0表示由分析器自行计算。
		CParamsParser(
			std::string strSplitString = _T("\\0"), 
			TCHAR cTagLeft = TCHAR('\"'),
			TCHAR cTagRight = TCHAR('\"'), 
			TCHAR cRelational = TCHAR('='), 
			std::string strEndTag = _T("\\0\\0"), 
			LPCTSTR pszParamsString = NULL,
			int nMaxChars = 0) 
		{
			_Init();
			SetSplitString(strSplitString);
			SetTags(cTagLeft, cTagRight);
			SetRelational(cRelational);
			SetEndTag(strEndTag);

			if (NULL != pszParamsString)
				Analysis(pszParamsString, nMaxChars);
		}

		virtual ~CParamsParser()
		{
			if (NULL != m_pSplitStringBytes)
				delete[] m_pSplitStringBytes;
			if (NULL != m_pEndTagBytes)
				delete[] m_pEndTagBytes;
		}

		// 计算参数字符串的字节长度
		// 功能简述：
		//     计算一个原始参数字符串的字节长度（包括结束空字符）(直接调用测试通过)
		//     注：因为算法原因，本方法不适合统计用超过2个空字符做为结束符的字符串。
		// 参数：
		//     pszParamsString [in] - 指定原始参数字符串。
		//     nMaxChars [in] - 指定pszParamsString中字符的数量，指定0表示由分析器自行计算。
		// 返回值：
		//     返回指定字符串的字节长度。
		int StrBytes(LPCTSTR pszParamsString, int nMaxChars = 0)
		{
			if (NULL == pszParamsString)
				return 0;

			bool bUseNulChar = false;
			LPCTSTR pChars = LPCTSTR(m_pSplitStringBytes);
			for (int i = 0; i < m_nSplitStringBytes; i += sizeof(TCHAR))
			{
				if (*pChars == TCHAR('\0'))
				{
					bUseNulChar = true;
					break;
				}
				++pChars;
			}
			
			int nLen = 0;
			if (!bUseNulChar)
			{// 如果不是使用空字符做为切割标记...

				nLen = ::lstrlen(pszParamsString);
				pChars = LPCTSTR(m_pEndTagBytes);
				while(*pChars)
				{
					++pChars;
				}

				// 可见字符串长度+结束符不可见的部分长度
				nLen += int((m_nEndTagBytes / sizeof(TCHAR)) - (LPCTSTR(m_pEndTagBytes) - pChars));
			}
			else
			{// 使用空字符做为切割标记...

				nLen = 0;
				LPCTSTR p = pszParamsString;
				bool bLoop = true;
				while(bLoop)
				{// 循环统计各子串字节数并累加...

					if (nMaxChars > 0 && nMaxChars <= nLen)
						break;

					int l = ::lstrlen(p);
					nLen += l + 1;

					LPCTSTR pCharsEndTag = LPCTSTR(m_pEndTagBytes);
					for (int i = 0; i <= l; i++)
					{
						if (p[i] == pCharsEndTag[0])
						{
							if (::memcmp(&(p[i]), m_pEndTagBytes, m_nEndTagBytes) == 0)
							{// 如果子串字节序列的尾部和结束符字节序列匹配则终止统计

								nLen += m_nEndTagBytes / sizeof(TCHAR) - (l - i + 1);
								bLoop = false;
								break;
							}
						}
					}
					p += l + 1;
				} // while(*p)
			}
			return nLen * sizeof(TCHAR);
		}

		// 分析参数字符串
		// 功能简述：
		//     根据配置数据分析参数字符串，将分析结果保存在对象内部参数集中。
		// 参数：
		//     pszParamsString [in] - 指定待解析的参数字符串。
		//     nMaxChars [in] - 指定pszParamsString中字符的数量，指定0表示由分析器自行计算。
		// 返回值：
		//     返回分析所得的参数数量。
		int Analysis(LPCTSTR pszParamsString, int nMaxChars = 0)
		{
			_Clear();
			int nCount = 0;
			std::list<std::string> slStrings;
			std::list<std::string>::iterator it;
			int nStrings = _SplitStrings(pszParamsString, m_pSplitStringBytes, m_nSplitStringBytes, m_pEndTagBytes, m_nEndTagBytes, slStrings, nMaxChars);
			for (it = slStrings.begin(); it != slStrings.end(); it++)
			{
				std::list<std::string> slParam;
				std::list<std::string>::iterator itItem;
				TCHAR cEnd = TCHAR('\0');
				int nItems = _SplitStrings((*it).c_str(), LPBYTE(&m_cRelational), int(sizeof(m_cRelational)), LPBYTE(&cEnd), int(sizeof(cEnd)), slParam, 2, int((*it).length() + 1));
				if (nItems == 1)
				{
					++nCount;
					m_smParams.insert(string_pair(_T(""), *slParam.begin()));
				}
				else if (nItems >= 2)
				{
					++nCount;
					itItem = slParam.begin();
					itItem++;
					m_smParams.insert(string_pair(*slParam.begin(), *(itItem)));
				}
			}
			return nCount;
		}

		// 查找参数
		// 功能简述：
		//     在分析所得的参数集中根据参数在参数集中的索引值查找参数值。
		// 参数：
		//     nIndex [in] - 参数索引。
		// 返回值：
		//     如果成功则返回nIndex指定的参数值，否则返回""。
		std::string Find(int nIndex)
		{
			std::string strName;
			return Find(nIndex, strName);
		}

		// 查找参数
		// 功能简述：
		//     在分析所得的参数集中根据参数在参数集中的索引值查找参数值和参数名。
		// 参数：
		//     nIndex [in] - 参数索引。
		//     strName [out] - 返回nIndex指定的参数名称。
		// 返回值：
		//     如果成功则返回nIndex指定的参数值，否则返回""。
		std::string Find(int nIndex, std::string & strName)
		{
			if (nIndex >= GetCount() || nIndex < 0)
				return "";
			std::multimap<std::string, std::string>::const_iterator it;
			it = m_smParams.begin();
			for (int i = 0; i < nIndex; i++)
				it++;
			strName = it->first;
			return it->second;
		}

		// 查找参数
		// 功能简述：
		//     在分析所得的参数集中根据参数名查找参数值。
		// 参数：
		//     strName [in] - 参数名称。
		//     bPrecisely [in] - 标记是否进行精确搜索，精确搜索意味着不进行任何处理而严格匹配参数名，
		//         否则搜索将不区分大小写并忽略首尾空格。
		// 返回值：
		//     如果成功则返回strName指定的参数值，否则返回""。
		std::string Find(std::string strName, bool bPrecisely = false)
		{
			strName.erase(strName.find_last_not_of(m_cRelational) + 1);
			if (!bPrecisely)
			{
				Trim(strName);
				std::transform(strName.begin(), strName.end(), strName.begin(), tolower);
			}

			std::multimap<std::string, std::string>::const_iterator it;
			for (it = m_smParams.begin(); it != m_smParams.end(); it++)
			{
				std::string strTemp = it->first;
				if (!bPrecisely)
				{
					Trim(strTemp);
					std::transform(strTemp.begin(), strTemp.end(), strTemp.begin(), tolower);
				}
				if (strTemp == strName)
					return it->second;
			}
			
			return "";
		}

		// 设置分隔符配置
		bool SetSplitString(std::string strSplit)
		{
			if (NULL != m_pSplitStringBytes)
				delete[] m_pSplitStringBytes;
			m_pSplitStringBytes = NULL;
			m_nSplitStringBytes = 0;
			_string2Bytes(strSplit, NULL, m_nSplitStringBytes);
			m_pSplitStringBytes = new BYTE[m_nSplitStringBytes];
			return _string2Bytes(strSplit, m_pSplitStringBytes, m_nSplitStringBytes);
		}

		// 设置左右标记符配置
		bool SetTags(TCHAR cLeft, TCHAR cRight)
		{
			m_cTagLeft = cLeft;
			m_cTagRight = cRight;
			return true;
		}

		// 设置关联符配置
		bool SetRelational(TCHAR cRelational)
		{
			m_cRelational = cRelational;
			return true;
		}

		// 设置结束符配置
		bool SetEndTag(std::string strEndTag)
		{
			if (NULL != m_pEndTagBytes)
				delete[] m_pEndTagBytes;
			m_pEndTagBytes = NULL;
			m_nEndTagBytes = 0;
			_string2Bytes(strEndTag, NULL, m_nEndTagBytes);
			m_pEndTagBytes = new BYTE[m_nEndTagBytes];
			return _string2Bytes(strEndTag, m_pEndTagBytes, m_nEndTagBytes);
		}

		// 取已经解析的参数数量
		// 返回值：
		//     返回已经解析的参数数量。
		int GetCount(void)
		{
			return int(m_smParams.size());
		}

	protected:

		// 简单切割字符串
		// 功能简述：
		//     切割符由pSplit指定，而终止符和左右标记采用对象相关设置，设置中的关联符不被使用。
		// 参数：
		//     pStrings [in] - 待切割的原始串。
		//     pSplit [in] - 切割符字节序列。
		//     nSplitBytes [in] - 切割符字节数。
		//     pEndTagBytes [in] - 结束符字节序列。
		//     nEndTagBytes [in] - 结束符字节数。
		//     slStrings [out] - 用于返回切割以后的字符串组。
		//     nMaxStrings [in] - 指定最大的切割字符串数量，如果实际切割出来的子串数量大于这个数值，则最
		//         后的n个子串将不被解析和切割。如果该值<1表示不限制切割数量。
		//     nMaxChars [in] - 指定pStrings的最大字符数量。<1表示不限制pStrings的最大字符数量。
		// 返回值：
		//     返回切割字符串数量。
		int _SplitStrings(LPCTSTR pStrings, LPBYTE pSplit, int nSplitBytes, LPBYTE pEndTagBytes, int nEndTagBytes, std::list<std::string> & slStrings, int nMaxStrings = 0, int nMaxChars = 0)
		{
			int nCount = 0;
			bool bFoundLeftTag = false;
			LPCTSTR pStr = pStrings;
			LPCTSTR pTempStr = pStr;
			LPCTSTR pLeftStr = NULL;

			while(true)
			{
				if (nMaxChars > 0 && (pStr - pStrings) >= nMaxChars)
					break;

				if (*pStr == LPCTSTR(pEndTagBytes)[0])
				{// 可能发现了结束符

					if (::memcmp(pStr, pEndTagBytes, nEndTagBytes) == 0)
					{// 找到了一个结束符

						// 此时pTempStr指向上一个切割符后或原始串首部
						std::string strTemp(pTempStr);
						// 将两个分隔符之间的字符串推入slStrings
						slStrings.push_back(strTemp.substr(0, pStr - pTempStr));
						++nCount;
						break;
					}
				}

				if (bFoundLeftTag)
				{
					if (*pStr == m_cTagRight)
					{
						bFoundLeftTag = false;
					}
				}
				else
				{
					if (*pStr == m_cTagLeft)
					{// 发现了左标记符号

						bFoundLeftTag = true;
					}
					else
					{
						if ((nMaxStrings < 1 || nCount < (nMaxStrings - 1)) &&
							*pStr == LPCTSTR(pSplit)[0])
						{// 可能发现了切割符...

							if (::memcmp(pStr, pSplit, nSplitBytes) == 0)
							{// 找到了一个切割符

								if (pTempStr != pStr)
								{
									// 此时pTempStr指向上一个切割符后或原始串首部
									std::string strTemp(pTempStr);
									// 将两个分隔符之间的字符串推入slStrings
									slStrings.push_back(strTemp.substr(0, pStr - pTempStr));
									pStr += nSplitBytes / sizeof(TCHAR) - 1;
									pTempStr = pStr + 1;
									++nCount;
								}
								else
								{
									pTempStr += nSplitBytes / sizeof(TCHAR);
								}
							}
						}
					}
				}
				++pStr;
			} // while(bLoop)
			return nCount;
		}

		// 字符串对象转换成字节序列
		// 功能简述：
		//     字符串对象转换成字节序列（不包括结束空字符），转换过程中将替换空字符转义符号为空字符。
		// 参数：
		//     str [in] - 指定字符串对象。
		//     pBytes [in] - 指定用户接受字节序列的缓冲区。
		//     nBytes [in] - 指定pBytes的字节大小并返回实际转换后数据的字节大小。
		// 返回值：
		//     如果成功返回true，如果nBytes指定的缓冲区太小则返回false。
		bool _string2Bytes(std::string str, LPBYTE pBytes, int & nBytes)
		{
			int nNulls = SubStringCount(str, std::string(_T("\\0")));
			int nNeedBytes = int((str.length() - nNulls)) * int(sizeof(TCHAR));
			nBytes = nNeedBytes;
			if (nNeedBytes > nBytes || NULL == pBytes)
			{// 缓冲区尺寸不够用...

				return false;
			}

			LPBYTE pTemp = new BYTE[(str.length()) * sizeof(TCHAR)];
			if (NULL == pTemp)
				return false;
			LPCTSTR pStr = str.c_str();
			LPTSTR pDes = LPTSTR(pTemp);
			for (int i = 0; i < int(str.length()); i++)
			{
				if (pStr[i] == TCHAR('\\') &&
					pStr[i + 1] == TCHAR('0'))
				{// 当前字符是空字符转义符号
					*pDes = TCHAR('\0');
					++i;
				}
				else
					*pDes = pStr[i];
				++pDes;
			}
			::memcpy(pBytes, pTemp, nBytes);
			delete[] pTemp;
			return true;
		}
		// 内部清理
		void _Clear(void)
		{
			m_smParams.clear();
		}
		// 初始化内部成员
		void _Init(void)
		{
			m_pSplitStringBytes = NULL;
			m_nSplitStringBytes = 0;
			m_pEndTagBytes = NULL;
			m_nEndTagBytes = 0;

			// 缺省采用空字符切分参数字符串
			SetSplitString(_T("\\0"));
			// 缺省用'"'双引号做为完整子串限定标记
			SetTags(TCHAR('\"'), TCHAR('\"'));
			// 缺省用'='做为参数名和参数值之间的关联符
			SetRelational(TCHAR('='));
			// 缺省用双空字符做为结束标记
			SetEndTag(_T("\\0\\0"));
		}

		LPBYTE m_pSplitStringBytes; // 用来切割原始字符串的切割标志字符串的字节序列
		int m_nSplitStringBytes; // 用来切割原始字符串的切割标志字符串的字节序列长度
		TCHAR m_cTagLeft; // 字符串中用来限定完整子串的左标记。如'<'
		TCHAR m_cTagRight; // 字符串中用来限定完整子串的右标记。如'>'
		TCHAR m_cRelational; // 参数名和参数值之间的关联符，如'='。
		LPBYTE m_pEndTagBytes; // 结束标记字符串的字节序列，空字符用可显示的"\0"做为转义字符。
		int m_nEndTagBytes; //结束标记字符串的字节序列长度。
		std::multimap<std::string, std::string> m_smParams; // 分析以后的参数集
	}; // class CParamsParser

}; // namespace SH