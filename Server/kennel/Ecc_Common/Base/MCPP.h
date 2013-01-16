/*//////////////////////////////////////////////////////////////////////////////////////////////////
Copyright (c) 2005, DragonFlow Corporation All rights reserved

项目名称：
　　ECC监测器调试工具

文件名：
    mcpp.h

依赖：
    RegHelper.h(Ver 0.0.2)
    
文件描述：
    该文件主要实现对ECC监测器监测函数调用参数的持久化，通过持久化，ECC监测器的调试工具MonitorCall可
	以通过重复使用历史调用参数配置的方式简化调试监测器时的调用参数设置，还可以在监测器源码中暂时性
	插入Helper代码，直接获取实际监测调用参数。

版本号：
    Ver 0.1

修改历史：
    Ver 0.1 (龚超 2007/8/1)
        初稿，早就做好了今天才整理，实现了CMonitorModule和CCallParam两个类以及一个Helper函数。
//////////////////////////////////////////////////////////////////////////////////////////////////*/

#if !defined(AFX_MCPP_H__E885D19D_E38C_4808_9A52_A909F2C64320__INCLUDED_)
#define AFX_MCPP_H__E885D19D_E38C_4808_9A52_A909F2C64320__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RegHelper.h"

namespace MCPP // Monitor Call Params Persistent
{

#define MCP_REGKEY					HKEY(HKEY_CURRENT_USER)
#define MCP_REGPATH					_T("SOFTWARE\\SiteviewTool\\MonitorCall\\1.0\\")

	// 监测器模块调用参数集
	class CMonitorModule;
	/*//////////////////////////////////////////////////////////////////////////////////////////////
	功能简述：
	    监测器调用参数
	    该类的每一个实例描述一组监测函数的调用参数，通过该类，可以完成对调用参数的创建、存储、装载、
		修改和删除。
	//////////////////////////////////////////////////////////////////////////////////////////////*/
	class CCallParam
	{
	public:
		CCallParam()
		{
			m_pMonitorModule = NULL;
			m_pmsParams = NULL;
			m_uParamsBytes = 0;
			m_nParams = 0;
			m_pParams = NULL;
			memset(m_szFuncName, 0, sizeof(TCHAR) * MAX_PATH);
		}
		virtual ~CCallParam()
		{
			_Clear();
		}
		// 取参数对数量
		int getParams(void)
		{
			return m_nParams;
		}
		// 根据索引值取相应的参数对字符串
		LPCTSTR GetParamByIndex(int nIndex)
		{
			if (nIndex < 0 || nIndex >= m_nParams || NULL == m_pParams)
				return NULL;
			return m_pParams[nIndex];
		}
		// 判断对象是否有效
		bool IsValid(void)
		{
			return bool(NULL != m_pmsParams && getParams() > 0 && ::lstrlen(m_szFuncName) > 0);
		}

		BOOL Remove(void);
		BOOL Load(void);
		BOOL Save(void);

		// 取调用参数多字符串
		LPTSTR GetCallParamMString(void)
		{
			return m_pmsParams;
		}
		// 设置调用参数多字符串
		LPTSTR SetCallParamMString(LPTSTR pmsParams, int nParamsBytes)
		{
			_Clear();
			if (NULL != pmsParams && nParamsBytes > 0)
			{
				m_pmsParams = LPTSTR(new BYTE[nParamsBytes]);
				if (NULL != m_pmsParams)
				{
					memcpy(m_pmsParams, pmsParams, nParamsBytes);
					m_uParamsBytes = DWORD(nParamsBytes);
					_CompParams();
					m_bChanged = TRUE;
				}
			}
			return m_pmsParams;
		}
		// 设置监测器模块属性
		void setMonitorModule(CMonitorModule * pMonitorModule)
		{
			m_pMonitorModule = pMonitorModule;
		}
		// 设置函数名称
		void setFuncName(LPCTSTR pszFuncName)
		{
			if (NULL != pszFuncName)
				::lstrcpyn(m_szFuncName, pszFuncName, MAX_PATH);
			else
				::lstrcpyn(m_szFuncName, _T(""), MAX_PATH);
		}

		TCHAR m_szFuncName[MAX_PATH];

	protected:

		// 清除当前对象的内部数据
		void _Clear(void)
		{
			if (NULL != m_pmsParams)
				delete m_pmsParams;
			m_pmsParams = NULL;
			m_bChanged = FALSE;
			m_uParamsBytes = 0;
			if (NULL != m_pParams)
				delete m_pParams;
			m_pParams = NULL;
			m_nParams = 0;
		}

		// 计算参数数量
		// 该函数计算当前对象包含的多字符串数量并为其创建一个索引。
		int _CompParams(void)
		{
			if (NULL != m_pParams)
				delete m_pParams;
			m_pParams = NULL;
			m_nParams = 0;

			if (NULL != m_pmsParams)
			{
				LPTSTR p = m_pmsParams;
				int nParams = 0;
				while(*p)
				{// 计算数量

					++nParams;
					p += lstrlen(p) + 1;
				}

				if (nParams > 0)
				{// 创建索引

					m_pParams = new LPTSTR[nParams];
					if (NULL != m_pParams)
					{
						for (int i = 0; i < nParams; i++)
						{
							m_pParams[i] = NULL;
						}

						p = m_pmsParams;
						while(*p)
						{
							m_pParams[m_nParams++] = p;
							p += lstrlen(p) + 1;
						}
					}
				}
			}
			return m_nParams;
		}

		// 参数内容改变标志
		BOOL m_bChanged;
		// 参数集中的参数对数量
		int m_nParams;
		// 参数集索引
		LPTSTR * m_pParams;
		// 参数集多字符串
		LPTSTR m_pmsParams;
		// 参数集多字符串的有效字节数
		DWORD m_uParamsBytes;
		// 所属监测器模块对象的指针
		CMonitorModule * m_pMonitorModule;
	}; // class CCallParam

	/*//////////////////////////////////////////////////////////////////////////////////////////////
	功能简述：
	    监测器模块调用参数集
	    该类的每一个实例描述一个监测器模块中所有监测函数的调用参数集，通过该类可以级联装载或保存相关
		监测器模块中监测函数的调用参数集，还可以对这些参数集进行管理。
	//////////////////////////////////////////////////////////////////////////////////////////////*/
	class CMonitorModule
	{
	public:
		CMonitorModule(LPCTSTR pszPathName)
		{
			::memset(m_szPathName, 0, sizeof(m_szPathName));
			::memset(m_szName, 0, sizeof(m_szName));
			if (NULL != pszPathName)
			{
				::lstrcpyn(m_szPathName, pszPathName, MAX_PATH);
				LPTSTR p = m_szPathName + ::lstrlen(m_szPathName);
				while(p > m_szPathName)
				{// 查找最后一个路径分隔符以确定模块名称
					if (*p == TCHAR('\\') ||
						*p == TCHAR('/'))
					{
						++p;
						break;
					}
					--p;
				}
				::lstrcpyn(m_szName, p, MAX_PATH);
			}
			for (int i = 0; i <= MAX_CALL_PARAMS; i++)
			{
				m_CallParams[i].setMonitorModule(this);
			}
		}

		// 取监测器模块调用参数存储路径
		int GetRegPath(LPTSTR pszBuff, int nBuffLen)
		{
			int nRegPathLen = ::lstrlen(MCP_REGPATH) + ::lstrlen(m_szName);
			if (NULL == pszBuff || nBuffLen < 1)
				return nRegPathLen;
			::lstrcpyn(pszBuff, MCP_REGPATH, nBuffLen);
			::lstrcpyn(pszBuff + ::lstrlen(MCP_REGPATH), m_szName, nBuffLen - \
				::lstrlen(MCP_REGPATH));
			return ::lstrlen(pszBuff);
		}

		// 装载监测器模块信息
		// 参数：
		//     bLoadAllParams - 指定是否级联装载当前监测器模块所有函数的调用参数。
		BOOL Load(BOOL bLoadAllParams = FALSE)
		{
			TCHAR szRegPath[MAX_PATH] = MCP_REGPATH;
			GetRegPath(szRegPath, MAX_PATH);

			HKEY hCurKey = NULL;
			DWORD dwDis = 0;
			long lRetVal = ::RegOpenKeyEx(
				MCP_REGKEY,
				szRegPath,
				REG_OPTION_NON_VOLATILE,
				KEY_QUERY_VALUE,
				&hCurKey);

			if(lRetVal != ERROR_SUCCESS || NULL == hCurKey)
				return FALSE;

			int nCPIndex = 0;
			for (DWORD i = 0; lRetVal == ERROR_SUCCESS; i++)
			{// 循环枚举监测器模块参数存储信息路径下的所有参数键值

				DWORD uType = 0;
				TCHAR szValName[MAX_PATH] ={0};
				DWORD uValNameChars = MAX_PATH;
				lRetVal = ::RegEnumValue(
					hCurKey, i, 
					szValName, &uValNameChars,
					NULL, &uType, NULL, NULL);
				if (ERROR_SUCCESS == lRetVal)
				{
					m_CallParams[nCPIndex].setMonitorModule(this);
					m_CallParams[nCPIndex].setFuncName(szValName);
					if (bLoadAllParams)
						m_CallParams[nCPIndex].Load();
					++nCPIndex;
					if (nCPIndex >= MAX_CALL_PARAMS)
						break;
				}
			}
			::RegCloseKey(hCurKey);
			return TRUE;
		}

		// 保存监测器模块信息
		BOOL Save(void)
		{
			for (int i = 0; i < MAX_CALL_PARAMS; i++)
			{
				m_CallParams[i].Save();
			}
			return TRUE;
		}
		// 根据索引值取对应的调用参数对象
		CCallParam & GetCallParamByIndex(int nIndex)
		{
			if (nIndex < 0 || nIndex >= MAX_CALL_PARAMS)
				return m_CallParams[MAX_CALL_PARAMS]; // 返回一个无效对象

			return m_CallParams[nIndex];
		}
		// 取得一个未使用的调用参数对象索引
		int GetFreeCallParamObject(void)
		{
			for (int i = 0; i < MAX_CALL_PARAMS; i++)
			{
				if (!m_CallParams[i].IsValid())
					return i;
			}
			return -1;
		}
		// 根据函数名取对应的调用参数对象
		CCallParam & GetCallParam(LPCTSTR pszFunc)
		{
			if (NULL == pszFunc)
				return m_CallParams[MAX_CALL_PARAMS]; // 返回一个无效对象
			for (int i = 0; i < MAX_CALL_PARAMS; i++)
			{
				if (::lstrcmp(pszFunc, m_CallParams[i].m_szFuncName) == 0)
					return m_CallParams[i];
			}
			return m_CallParams[MAX_CALL_PARAMS]; // 返回一个无效对象
		}
		// 取已经命名的调用参数对象数量
		int GetNamedCallParamCount(void)
		{
			int nCount = 0;
			for (int i = 0; i < MAX_CALL_PARAMS; i++)
			{
				if (::lstrlen(m_CallParams[i].m_szFuncName))
					++nCount;
				else
					break;
			}
			return nCount;
		}
		// 取有效的调用参数对象数量
		int GetValidCallParamCount(void)
		{
			int nCount = 0;
			for (int i = 0; i < MAX_CALL_PARAMS; i++)
			{
				if (m_CallParams[i].IsValid())
					++nCount;
			}
			return nCount;
		}
		// 一个模块内函数数量的最大数量
		enum { MAX_CALL_PARAMS = 256 };
		// 当前模块的全路径名称
		TCHAR m_szPathName[MAX_PATH];
		// 当前模块的名称
		TCHAR m_szName[MAX_PATH];
		// 当前模块内所有函数的调用参数对象
		// 最后一个对象保留用于返回无效对象
		MCPP::CCallParam m_CallParams[MAX_CALL_PARAMS + 1];
	}; // class CMonitorModule

	// 保存调用参数
	inline BOOL CCallParam::Save(void)
	{
		if (!m_bChanged)
			return TRUE;

		if (NULL == m_pMonitorModule ||
			::lstrlen(m_szFuncName) < 1 ||
			NULL == m_pmsParams ||
			m_uParamsBytes < 1)
			return FALSE;

		BOOL bRet = FALSE;

		// 通过注册表值映射对象保存参数记录
		TCHAR szRegPath[MAX_PATH] = {0};
		m_pMonitorModule->GetRegPath(szRegPath, MAX_PATH);
		MiniHelper::CRegHelper::CRegValue rv(MCP_REGKEY, szRegPath, m_szFuncName, FALSE);
		bRet = rv.SetValue(LPBYTE(m_pmsParams), m_uParamsBytes);
		if (bRet)
		{
			if (NO_ERROR == rv.Save())
			{
				bRet = TRUE;
				m_bChanged = FALSE;
			}
		}
		return bRet;
	}

	// 删除当前关联函数的调用参数记录
	inline BOOL CCallParam::Remove(void)
	{
		if (NULL == m_pMonitorModule ||
			::lstrlen(m_szFuncName) < 1)
			return FALSE;

		// 利用注册表值映射对象的自动删除功能删除记录
		TCHAR szRegPath[MAX_PATH] = {0};
		m_pMonitorModule->GetRegPath(szRegPath, MAX_PATH);
		MiniHelper::CRegHelper::CRegValue rv(MCP_REGKEY, szRegPath, m_szFuncName, FALSE, TRUE);

		m_bChanged = TRUE;
		return TRUE;
	}

	// 装载当前对象所关联函数的调用参数
	inline BOOL CCallParam::Load(void)
	{
		if (NULL == m_pMonitorModule ||
			::lstrlen(m_szFuncName) < 1)
			return FALSE;

		_Clear();

		BOOL bRet = FALSE;

		// 获取当前关联模块参数保存注册表路径
		TCHAR szRegPath[MAX_PATH] = {0};
		m_pMonitorModule->GetRegPath(szRegPath, MAX_PATH);

		// 创建一个注册表值映射对象装载当前关联函数的参数
		MiniHelper::CRegHelper::CRegValue rv(MCP_REGKEY, szRegPath, m_szFuncName, FALSE);
		if (NO_ERROR == rv.Load() && rv.getValueBytes() > 0)
		{
			// 将参数（一个多字符串）保存到m_pmsParams
			m_pmsParams = LPTSTR(new BYTE[rv.getValueBytes()]);
			if (NULL != m_pmsParams)
			{
				bRet = rv.GetValue(LPBYTE(m_pmsParams), rv.getValueBytes());
				if (bRet)
				{
					m_uParamsBytes = DWORD(rv.getValueBytes());
					_CompParams();
				}
				else
					_Clear();
			}
		}
		return bRet;
	}

	// 保存调用参数
	// 功能简述：
	//     该函数用于简化调用参数的保存过程。在实际的监测器开发和调试过程中，可以在监测器调用函数的
	// 入口出调用该函数，只需放在实际环境中运行一次，即可注释该函数的调用代码，然后即可在
	// MonitorCall.exe中完全模拟实际环境的调用参数对监测器进行调试调用。
	// 参数：
	//     pszModuleName [in] - 指定监测器模块名。
	//     pszFuncName [in] - 指定监测器函数名。
	//     pmsParams [in] - 指定监测器函数被调用时传入的参数指针。
	// 返回值：
	//     如果成功则返回pmsParams的字节数，否则返回值<0。
	inline int SaveCallParams(LPCTSTR pszModuleName, LPCTSTR pszFuncName, const char * pmsParams)
	{
		if (NULL == pszModuleName || NULL == pszFuncName || NULL == pmsParams)
			return -1;
		int nBytes = -1;
		CMonitorModule mm(pszModuleName);
		int nFcpoIndex = mm.GetFreeCallParamObject();
		if (nFcpoIndex >= 0)
		{
			CCallParam & cp = mm.GetCallParamByIndex(nFcpoIndex);
			const char * p = pmsParams;
			nBytes = 0;
			while(*p)
			{
				nBytes += strlen(p) + 1;
				p += strlen(p) + 1;
			}
			++nBytes;
			cp.setFuncName(pszFuncName);
			cp.SetCallParamMString(LPTSTR(pmsParams), nBytes);
			cp.Save();
		}
		return nBytes;
	}

}; // namespace MCPP

#endif // !defined(AFX_MCPP_H__E885D19D_E38C_4808_9A52_A909F2C64320__INCLUDED_)
