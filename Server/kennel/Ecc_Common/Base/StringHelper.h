/*//////////////////////////////////////////////////////////////////////////////////////////////////
Copyright (c) 2005, DragonFlow Corporation All rights reserved

��Ŀ���ƣ�
����ECC��������ô���

�ļ�����
    StringHelper.h

������
    <�ļ���1>(<�ļ��汾��>)
    <�ļ���2>(<�ļ��汾��>)
    <�ļ���n>(<�ļ��汾��>)
    
�ļ�������
    ���ļ�ʵ����һЩ��ECC��������������Թ����о����õ����ַ�����ش������࣬���ڰ汾�а���������
	Ϣ��������ַ�������ͼ�⺯�����ò����������ȡ�
	���ļ�ʵ�������д��룬���������ļ�֧�֣�ֻҪinclude���ļ�����ʹ�����й��ܡ�
	���ļ�ʹ��SH�����ռ䡣

�汾�ţ�
    Ver 1.4

�޸���ʷ��
    Ver 0.1 (���� 2007/7/6)
        ���壬ʵ���˼�������ò�����������CParamsParser�͵�����Ϣ���������COutputDebugString��
    Ver 0.2 (���� 2007/7/18)
        �����˴��ʷ�������_bstr_t�����ַ����ֶ�����CAnalyzer4_bstr_t��
    Ver 0.3 (���� 2007/7/20)
        1. �޸���CParamsParser::_SplitStringsʵ�֣������������з������Ĳ�����
        2. ���ڶ�CParamsParser::_SplitStrings�޸ģ�����CParamsParser::Analysis���㷨ʵ���˶԰����ָ�
           ����������ȷ���������磺�޸�ǰarg1=123=456�ᱻ����Ϊ[������:arge, ����ֵ:123]���޸ĺ�ͬ��
           ���ַ�������Ա�����Ϊ[������:arge, ����ֵ:123=456]��
        ���������޸���Ϊ�˽���������ڲ����г��ֶ��=���Ŷ�Ӱ���������������⡣
    Ver 1.4 (���� 2007/7/31)
        ������COutputDebugString�Ĺ��캯�����������������µı����ODS_SHOW_TICKCOUNT��
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

// �ñ�־�����Ƿ������������Ϣ��ʱ�����ͬһ�߳���Ƕ�ײ�ν�������
// ������ܶ��ڸ��ٺ�������·�����а������������غ�������ڴ�����
// COutputDebugString���󲢵���COutputDebugString�����OutputDebugString
// ���һ��ָʾ��ǰ���ں���������Ϣ����ô�ڵ�����Ϣ�۲촰���н�����
// ���ķ�ʽ����ʾ������Ϣ��û����һ�㺯�����ã�������Ϣ�������һ
// ��ǿո�
#ifndef ODS_OUTPUT_INDENT
#	define ODS_OUTPUT_INDENT 1
#endif // ifndef ODS_OUTPUT_INDENT

// �ñ�־���������������Ϣ��ʱ���Ƿ���ǰ׺����ʾ�߳�ID
#ifndef ODS_SHOW_THREADID
#	define ODS_SHOW_THREADID 0
#endif // ifndef ODS_SHOW_THREADID

// �ñ�־���������������Ϣ��ʱ���Ƿ���ǰ׺����ʾ����OutputDebugStringʱ��TickCountֵ��
#ifndef ODS_SHOW_TICKCOUNT
#	define ODS_SHOW_TICKCOUNT 0
#endif // ifndef ODS_SHOW_TICKCOUNT

#ifdef ODS_USING // ʹ��ODS�������������Ϣ

/*
// ODS��ʹ��ʾ������
int func2(void)
{
	int n = 100;
	ODS_OUTDEBUGD("return %d", n); // ֱ�����������Ϣ
	return n;
}
void func(void)
{
	ODS_DEFINE(" key"); // ����һ��COutputDebugString���󣬺����������ͨ������������������Ϣ��
	ODS_OUTDEBUG("call func ..."); // ͨ��ODS_DEFINE����Ķ������������Ϣ

	int a = func2();
	ODS_OUTDEBUG("a = %d", a); // ͨ��ODS_DEFINE����Ķ������������Ϣ

	LPCTSTR pszDbg = "string1\0 string2\0";
	ODS_OUTDEBUGM(pszDbg, " key"); // ֱ�����һ�����ַ��������ʱ���ַ����Ŀ��ַ�('\0')�����滻�ɿո��ַ�' '
}
*/

// ����COutputDebugString����
#	define ODS_DEFINE			SH::COutputDebugString __ods
// ͨ��ODS_DEFINE����Ķ������������Ϣ
#	define ODS_OUTDEBUG			__ods.OutputDebugString
// ����ҪODS_DEFINE�����֧��ֱ�����������Ϣ
#	define ODS_OUTDEBUGD		SH::COutputDebugString::OutputDebugStringD
// ����ҪODS_DEFINE�����֧��ֱ�����һ�����ַ���������Ϣ
#	define ODS_OUTDEBUGM		SH::COutputDebugString::OutputDebugMString

#else // ����ODS������Ϣ�������

#	define ODS_DEFINE			//
#	define ODS_OUTDEBUG			//
#	define ODS_OUTDEBUGD		//
#	define ODS_OUTDEBUGM		//

#endif // ifdef ODS_USING

namespace SH // StringHelper ����������ͻ
{
	/*//////////////////////////////////////////////////////////////////////////////////////////////
	���ܼ�����
		���ʷ�������_bstr_t�����ַ����ֶ���������ʵ�ֶ�_bstr_t���͵��ַ������зֶδ����ֶι��̲���
		�ض�Ӣ�ĵ��ʣ�Ҳ���Ὣһ�����ķֳ����롣
	//////////////////////////////////////////////////////////////////////////////////////////////*/
	class CAnalyzer4_bstr_t
	{
	public:
		// ���캯��
		// ���ܼ�����
		//     �ڶ�����ʱ����������Ϊ�ֶζϵ���ַ����ϡ�
		// ������
		//     strBreakpointChars [in] - ָ��������Ϊ�ϵ���ַ����ϡ�
		CAnalyzer4_bstr_t(_bstr_t strBreakpointChars = _T(" ,.-;!?/\\"))
		{
			m_strBreakpointChars = strBreakpointChars;
		}

		// ���ַ������зֶδ���
		// ���ܼ�����
		//     ��һ��_bstr_t�ַ������зֶδ����ֶν���������Ľ�������档
		// ������
		//     strTagString [in] - ���ֶε�_bstr_t�ַ�������
		//     nMaxChars [in] - ÿ���ֶε�����ַ�����
		// ����ֵ��
		//     ���طֶ�������
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

		// ȡ�ֶν��
		// ���ܼ�����
		//     �ӵ�ǰ����������ȡָ������λ�õķֶ��ַ�����
		// ������
		//     nIndex [in] - ָ���ֶ��ַ���������ֵ��
		// ����ֵ��
		//     ����ɹ��򷵻�nIndexָ���ķֶ��ַ��������򷵻�һ�����ַ�����
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

		// �ֶ���������
		// ���ܼ�����
		//     ���ص�ǰ���������зֶ��ַ�����������
		// ����ֵ��
		//     ���طֶ�����ֵ��
		int getResultsCount(void)
		{
			return int(m_lsResults.size());
		}
	protected:

		// �ж�һ�����ַ��Ƿ������Ϊ�ֶζϵ��ַ�
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

		// ��ָ�����ַ����Ĳ����Ӵ�������������
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

		// ���������ɽض�λ��
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

		// ��ָ�����ַ�������ָ����󳤶Ƚضϲ�������������
		LPCWSTR _SplitToResults(LPCWSTR pszTagString, int nMaxChars)
		{
			LPCWSTR pszRet = NULL;
			if (NULL == pszTagString || pszTagString[0] == WCHAR('\0'))
				return pszRet;
			if (_IsBreakpointChar(pszTagString[nMaxChars]) ||
				_IsBreakpointChar(pszTagString[nMaxChars - 1]))
			{// ����ֱ�ӷֶ�
				if (_AddToResults(pszTagString, nMaxChars))
					pszRet = pszTagString + nMaxChars;
			}
			else
			{
				int nBPos = _ReverseFindBreakpoint(pszTagString, nMaxChars - 1);
				if (nBPos > 0)
				{// �����������˿��Խضϵ�λ��...

					if (_AddToResults(pszTagString, nBPos))
						pszRet = pszTagString + nBPos;
				}
			}
			return pszRet;
		}

		// ������Ϊ�ϵ���ַ�����
		_bstr_t m_strBreakpointChars;
		// �ֶ��ַ��������
		std::list<_bstr_t> m_lsResults;

	}; // class CAnalyzer4_bstr_t

	/*//////////////////////////////////////////////////////////////////////////////////////////////
	���ܼ�����
		������Ϣ�����ࡣ�������࣬���Ը��������������Ϣ�����������ʹ��printf����һ�����������Ϣ
		������������̵߳���·���Զ������ĸ�ʽ��������Ϣ��
	//////////////////////////////////////////////////////////////////////////////////////////////*/
	class COutputDebugString
	{
	public:
		// ���캯��
		// ���ܼ�����
		//     �ڶ�����ʱ����ȱʡ�ĵ�����Ϣ�ؼ��ֺ�TickCountֵ��ʾ��־��
		// ������
		//     pszKey [in] - ָ��һ���������ÿһ�����������Ϣβ���Ĺؼ��ִ���
		COutputDebugString(LPCTSTR pszKey = _T("\n"))
		{
			m_nThisThreadLeve = _ThreadLeve(::GetCurrentThreadId(), true);

			const int MAX_BUF = 128;
			TCHAR szBuf[MAX_BUF] = {0};

#if ODS_SHOW_THREADID > 0 // ָ����ʾ������ǰCOutputDebugString������߳�ID��

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

		// ֱ��������ַ���������Ϣ
		// ���ܼ�����
		//     ֱ�������������COutputDebugStringʵ����һ�����ַ���������Ϣ�����ַ����ڼ����������
		//     �����õ������磺�κ�һ�������ú�������ڲ����ͷ���ֵ����һ�����ַ��������ַ�������
		//     ��'\0'��ʾ���������԰��������'\0'�������ַ������ӵ��Զ˿����һ����ַ���ʱ�����ַ�
		//     ��֮���'\0'�����滻Ϊ' '��
		// ������
		//     pMString [in] - ָ��һ����ַ�����
		//     pszKey [in] - ָ����ʾ�ڵ�����Ϣβ���Ĺؼ����ַ�����
		// ����ֵ��
		//     ����ɹ�����NO_ERROR�����򷵻ر�׼����ֵ��
		static int OutputDebugMString(LPCTSTR pMString, LPCTSTR pszKey = NULL)
		{
			int nRetVal = NO_ERROR;

			// ׼���õ�����Ϣ���������
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

		// ��printf��ʽֱ�����������Ϣ
		// ���ܼ�����
		//     ��printf��ʽֱ�������������COutputDebugStringʵ����������Ϣ������ĵ�����Ϣ��ȫ��
		//     printf�������ͬ��
		// ������
		//     pszFormat [in] - ָ��һ�������ʽ�ַ��������ʽҪ��μ�MSDN��printf������˵����
		// ����ֵ��
		//     ����ɹ�����NO_ERROR�����򷵻ر�׼����ֵ��
		static int OutputDebugStringD(LPCTSTR pszFormat, ...)
		{
			int nRetVal = NO_ERROR;

			// ׼���õ�����Ϣ���������
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

		// ��printf��ʽ���������Ϣ
		// ���ܼ�����
		//     ��printf��ʽ���������Ϣ������ĵ�����Ϣ�в������������ò�������ָ������Ϣ�����ḽ��
		//     ǰ׺�ͺ�׺��Ϣ������μ���ע��
		// ������
		//     pszFormat [in] - ָ��һ�������ʽ�ַ��������ʽҪ��μ�MSDN��printf������˵����
		// ����ֵ��
		//     ����ɹ�����NO_ERROR�����򷵻ر�׼����ֵ��
		// ��ע��
		//     ��COutputDebugStringʵ������ʱ��pszKeyָ���Ĺؼ����ַ���������Ϊ��׺��
		//     ��������ODS_SHOW_TICKCOUNT��ֵ����0�����ڵ�����Ϣǰ׺�и���GetTickCount()����ֵ��
		//     ��������ODS_OUTPUT_INDENT��ֵ����0��������Ϣ�����ͬһ���߳��е�Ƕ�����˳�������
		//     ��������
		//     ��������ODS_SHOW_THREADID��ֵ����0�����ڵ�����Ϣǰ׺�н������߳�IDֵ��
		int OutputDebugString(LPCTSTR pszFormat, ...)
		{
			int nRetVal = NO_ERROR;

			// ׼���õ�����Ϣ���������
			TCHAR szDbgStr[MAX_DEBUG_STRING];
			::memset(szDbgStr, 0, sizeof(TCHAR) * MAX_DEBUG_STRING);
			LPTSTR pszDbgStr = szDbgStr;
			int nMaxDebugString = MAX_DEBUG_STRING;

#if ODS_SHOW_TICKCOUNT > 0 // ָ���ڵ�����Ϣ�����TickCountֵǰ׺

			::_sntprintf(pszDbgStr, nMaxDebugString, _T("%u "), ::GetTickCount());
			pszDbgStr += ::lstrlen(pszDbgStr);
			nMaxDebugString -= ::lstrlen(pszDbgStr);

#endif // if ODS_SHOW_TICKCOUNT > 0

			// ������Ӻ�ǰ׺��Ϣ
			::lstrcpyn(
				pszDbgStr, 
				m_strPrefix.c_str(), 
				nMaxDebugString);
			pszDbgStr += ::lstrlen(pszDbgStr);
			nMaxDebugString -= ::lstrlen(pszDbgStr);

#if ODS_OUTPUT_INDENT > 0 // ָ������ͬһ���̵߳�Ƕ�������������

			if (m_nThisThreadLeve > 1 && 
				pszFormat[0] != TCHAR('*') &&
				pszFormat[0] != TCHAR('~'))
			{// ���ݵ�ǰ�̵߳Ľ��������������

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
		std::string m_strPrefix; // ���������Ϣǰ׺
		std::string m_strKey; // �ؼ��֣�׷���ڵ�����Ϣβ��

		int m_nThisThreadLeve; // ��ǰ�����ڵ�ǰ�߳��еĽ����Σ���ֵ������������Ϣ����ǵ���������

		// ע���ע��һ���߳̽���㼶
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

	// �����ַ��������
	typedef std::pair<std::string, std::string> string_pair;

	// ���ָ���ַ���������β��ָ���Ӵ�
	// ���ܼ�����
	//     ���ָ���ַ���������β��ָ���Ӵ���
	// ������
	//     s [in, out] - ָ����������ַ�����
	//     strTrim [in] - ָ��Ҫ�������β�Ӵ�
	// ����ֵ��
	//     ���ش����Ժ���ַ�����������ڲ��������s��ͬһ���ַ�������
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


	// ����ָ���ַ�����һ���ַ����б��е�����
	// ���ܼ�����
	//     ����ָ���ַ�����һ���ַ����б��е�������
	// ������
	//     slStrs [in] - ָ��Ҫ�������ַ�����
	//     strFind [in] - ָ��Ҫ�������ַ�����
	// ����ֵ��
	//     ����ҵ�Ŀ���ַ����򷵻������ַ������е����������򷵻�-1��
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

	// ����һ���Ӵ���һ���ַ����г��ֵĴ���
	// ���ܼ�����
	//     strString [in] - ָ�����ܰ����Ӵ����ַ�����
	//     strSub [in] - ָ�����������Ӵ���
	// ����ֵ��
	//     ����strSub��strString�г��ֵĴ�����
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
	���ܼ�����
		�������ַ��������������������м������ڲ����ַ���������
		���ڷ�����һ���ַ����а�������߼��ַ����Ĳ�����Ӧ��
		������Param1=Value1 Param2="Value 2"
		����������и�������Ϊ�ո񣬹���������Ϊ=������������Ϊ�գ����ұ�ʶ������Ϊ"��������
		��������Ϊ����������������ֱ�ΪParam1��Param2����Ӧ�Ĳ���ֵ��ΪValue1��Value 2
		����ȱʡ���ã�
			�и��������ַ�"\0"
		���ұ�Ƿ���˫����"\""
			���������Ⱥ�"="
			��������˫���ַ�"\0\0"
		�����ȱʡ���ÿ���ֱ�����ڽ����������⺯���ĵ��ò���������
	//////////////////////////////////////////////////////////////////////////////////////////////*/
	class CParamsParser
	{
	public:
		// ���캯��
		// ���ܼ�����
		//     �ڶ�����ʱ��ȱʡ���ý��в����ַ���������
		// ������
		//     pszParamsString [in] - ָ���������Ĳ����ַ�����
		//     nMaxChars [in] - ָ��pszParamsString���ַ���������ָ��0��ʾ�ɷ��������м��㡣
		CParamsParser(
			LPCTSTR pszParamsString,
			int nMaxChars = 0) 
		{
			_Init();
			if (NULL != pszParamsString)
				Analysis(pszParamsString, nMaxChars);
		}

		// ���캯��
		// ���ܼ�����
		//     �ڶ�����ʱ�ö������ý��в����ַ���������
		// ������
		//     slSplitStrings [in] - ָ�������и�ԭʼ�ַ������и��־�ַ��������ַ��ÿ���ʾ��"\0"��Ϊת���ַ���
		//     cTagLeft [in] - ָ���ַ����������޶������Ӵ��ı�ǵ���߲��֣���'<'��
		//     cTagRight [in] - ָ���ַ����������޶������Ӵ��ı�ǵ��ұ߲��֣���'>'��
		//     cRelational [in] - ָ���������Ͳ���ֵ֮��Ĺ���������'='��
		//     strEndTag [in] - ������ǣ����ַ��ÿ���ʾ��"\0"��Ϊת���ַ���
		//     pszParamsString [in] - ָ���������Ĳ����ַ�����
		//     nMaxChars [in] - ָ��pszParamsString���ַ���������ָ��0��ʾ�ɷ��������м��㡣
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

		// ��������ַ������ֽڳ���
		// ���ܼ�����
		//     ����һ��ԭʼ�����ַ������ֽڳ��ȣ������������ַ���(ֱ�ӵ��ò���ͨ��)
		//     ע����Ϊ�㷨ԭ�򣬱��������ʺ�ͳ���ó���2�����ַ���Ϊ���������ַ�����
		// ������
		//     pszParamsString [in] - ָ��ԭʼ�����ַ�����
		//     nMaxChars [in] - ָ��pszParamsString���ַ���������ָ��0��ʾ�ɷ��������м��㡣
		// ����ֵ��
		//     ����ָ���ַ������ֽڳ��ȡ�
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
			{// �������ʹ�ÿ��ַ���Ϊ�и���...

				nLen = ::lstrlen(pszParamsString);
				pChars = LPCTSTR(m_pEndTagBytes);
				while(*pChars)
				{
					++pChars;
				}

				// �ɼ��ַ�������+���������ɼ��Ĳ��ֳ���
				nLen += int((m_nEndTagBytes / sizeof(TCHAR)) - (LPCTSTR(m_pEndTagBytes) - pChars));
			}
			else
			{// ʹ�ÿ��ַ���Ϊ�и���...

				nLen = 0;
				LPCTSTR p = pszParamsString;
				bool bLoop = true;
				while(bLoop)
				{// ѭ��ͳ�Ƹ��Ӵ��ֽ������ۼ�...

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
							{// ����Ӵ��ֽ����е�β���ͽ������ֽ�����ƥ������ֹͳ��

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

		// ���������ַ���
		// ���ܼ�����
		//     �����������ݷ��������ַ�������������������ڶ����ڲ��������С�
		// ������
		//     pszParamsString [in] - ָ���������Ĳ����ַ�����
		//     nMaxChars [in] - ָ��pszParamsString���ַ���������ָ��0��ʾ�ɷ��������м��㡣
		// ����ֵ��
		//     ���ط������õĲ���������
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

		// ���Ҳ���
		// ���ܼ�����
		//     �ڷ������õĲ������и��ݲ����ڲ������е�����ֵ���Ҳ���ֵ��
		// ������
		//     nIndex [in] - ����������
		// ����ֵ��
		//     ����ɹ��򷵻�nIndexָ���Ĳ���ֵ�����򷵻�""��
		std::string Find(int nIndex)
		{
			std::string strName;
			return Find(nIndex, strName);
		}

		// ���Ҳ���
		// ���ܼ�����
		//     �ڷ������õĲ������и��ݲ����ڲ������е�����ֵ���Ҳ���ֵ�Ͳ�������
		// ������
		//     nIndex [in] - ����������
		//     strName [out] - ����nIndexָ���Ĳ������ơ�
		// ����ֵ��
		//     ����ɹ��򷵻�nIndexָ���Ĳ���ֵ�����򷵻�""��
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

		// ���Ҳ���
		// ���ܼ�����
		//     �ڷ������õĲ������и��ݲ��������Ҳ���ֵ��
		// ������
		//     strName [in] - �������ơ�
		//     bPrecisely [in] - ����Ƿ���о�ȷ��������ȷ������ζ�Ų������κδ�����ϸ�ƥ���������
		//         ���������������ִ�Сд��������β�ո�
		// ����ֵ��
		//     ����ɹ��򷵻�strNameָ���Ĳ���ֵ�����򷵻�""��
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

		// ���÷ָ�������
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

		// �������ұ�Ƿ�����
		bool SetTags(TCHAR cLeft, TCHAR cRight)
		{
			m_cTagLeft = cLeft;
			m_cTagRight = cRight;
			return true;
		}

		// ���ù���������
		bool SetRelational(TCHAR cRelational)
		{
			m_cRelational = cRelational;
			return true;
		}

		// ���ý���������
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

		// ȡ�Ѿ������Ĳ�������
		// ����ֵ��
		//     �����Ѿ������Ĳ���������
		int GetCount(void)
		{
			return int(m_smParams.size());
		}

	protected:

		// ���и��ַ���
		// ���ܼ�����
		//     �и����pSplitָ��������ֹ�������ұ�ǲ��ö���������ã������еĹ���������ʹ�á�
		// ������
		//     pStrings [in] - ���и��ԭʼ����
		//     pSplit [in] - �и���ֽ����С�
		//     nSplitBytes [in] - �и���ֽ�����
		//     pEndTagBytes [in] - �������ֽ����С�
		//     nEndTagBytes [in] - �������ֽ�����
		//     slStrings [out] - ���ڷ����и��Ժ���ַ����顣
		//     nMaxStrings [in] - ָ�������и��ַ������������ʵ���и�������Ӵ��������������ֵ������
		//         ���n���Ӵ��������������и�����ֵ<1��ʾ�������и�������
		//     nMaxChars [in] - ָ��pStrings������ַ�������<1��ʾ������pStrings������ַ�������
		// ����ֵ��
		//     �����и��ַ���������
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
				{// ���ܷ����˽�����

					if (::memcmp(pStr, pEndTagBytes, nEndTagBytes) == 0)
					{// �ҵ���һ��������

						// ��ʱpTempStrָ����һ���и�����ԭʼ���ײ�
						std::string strTemp(pTempStr);
						// �������ָ���֮����ַ�������slStrings
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
					{// ���������Ƿ���

						bFoundLeftTag = true;
					}
					else
					{
						if ((nMaxStrings < 1 || nCount < (nMaxStrings - 1)) &&
							*pStr == LPCTSTR(pSplit)[0])
						{// ���ܷ������и��...

							if (::memcmp(pStr, pSplit, nSplitBytes) == 0)
							{// �ҵ���һ���и��

								if (pTempStr != pStr)
								{
									// ��ʱpTempStrָ����һ���и�����ԭʼ���ײ�
									std::string strTemp(pTempStr);
									// �������ָ���֮����ַ�������slStrings
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

		// �ַ�������ת�����ֽ�����
		// ���ܼ�����
		//     �ַ�������ת�����ֽ����У��������������ַ�����ת�������н��滻���ַ�ת�����Ϊ���ַ���
		// ������
		//     str [in] - ָ���ַ�������
		//     pBytes [in] - ָ���û������ֽ����еĻ�������
		//     nBytes [in] - ָ��pBytes���ֽڴ�С������ʵ��ת�������ݵ��ֽڴ�С��
		// ����ֵ��
		//     ����ɹ�����true�����nBytesָ���Ļ�����̫С�򷵻�false��
		bool _string2Bytes(std::string str, LPBYTE pBytes, int & nBytes)
		{
			int nNulls = SubStringCount(str, std::string(_T("\\0")));
			int nNeedBytes = int((str.length() - nNulls)) * int(sizeof(TCHAR));
			nBytes = nNeedBytes;
			if (nNeedBytes > nBytes || NULL == pBytes)
			{// �������ߴ粻����...

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
				{// ��ǰ�ַ��ǿ��ַ�ת�����
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
		// �ڲ�����
		void _Clear(void)
		{
			m_smParams.clear();
		}
		// ��ʼ���ڲ���Ա
		void _Init(void)
		{
			m_pSplitStringBytes = NULL;
			m_nSplitStringBytes = 0;
			m_pEndTagBytes = NULL;
			m_nEndTagBytes = 0;

			// ȱʡ���ÿ��ַ��зֲ����ַ���
			SetSplitString(_T("\\0"));
			// ȱʡ��'"'˫������Ϊ�����Ӵ��޶����
			SetTags(TCHAR('\"'), TCHAR('\"'));
			// ȱʡ��'='��Ϊ�������Ͳ���ֵ֮��Ĺ�����
			SetRelational(TCHAR('='));
			// ȱʡ��˫���ַ���Ϊ�������
			SetEndTag(_T("\\0\\0"));
		}

		LPBYTE m_pSplitStringBytes; // �����и�ԭʼ�ַ������и��־�ַ������ֽ�����
		int m_nSplitStringBytes; // �����и�ԭʼ�ַ������и��־�ַ������ֽ����г���
		TCHAR m_cTagLeft; // �ַ����������޶������Ӵ������ǡ���'<'
		TCHAR m_cTagRight; // �ַ����������޶������Ӵ����ұ�ǡ���'>'
		TCHAR m_cRelational; // �������Ͳ���ֵ֮��Ĺ���������'='��
		LPBYTE m_pEndTagBytes; // ��������ַ������ֽ����У����ַ��ÿ���ʾ��"\0"��Ϊת���ַ���
		int m_nEndTagBytes; //��������ַ������ֽ����г��ȡ�
		std::multimap<std::string, std::string> m_smParams; // �����Ժ�Ĳ�����
	}; // class CParamsParser

}; // namespace SH