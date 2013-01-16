/*//////////////////////////////////////////////////////////////////////////////////////////////////
Copyright (c) 2005, DragonFlow Corporation All rights reserved

��Ŀ���ƣ�
����ECC��������Թ���

�ļ�����
    mcpp.h

������
    RegHelper.h(Ver 0.0.2)
    
�ļ�������
    ���ļ���Ҫʵ�ֶ�ECC�������⺯�����ò����ĳ־û���ͨ���־û���ECC������ĵ��Թ���MonitorCall��
	��ͨ���ظ�ʹ����ʷ���ò������õķ�ʽ�򻯵��Լ����ʱ�ĵ��ò������ã��������ڼ����Դ������ʱ��
	����Helper���룬ֱ�ӻ�ȡʵ�ʼ����ò�����

�汾�ţ�
    Ver 0.1

�޸���ʷ��
    Ver 0.1 (���� 2007/8/1)
        ���壬��������˽��������ʵ����CMonitorModule��CCallParam�������Լ�һ��Helper������
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

	// �����ģ����ò�����
	class CMonitorModule;
	/*//////////////////////////////////////////////////////////////////////////////////////////////
	���ܼ�����
	    ��������ò���
	    �����ÿһ��ʵ������һ���⺯���ĵ��ò�����ͨ�����࣬������ɶԵ��ò����Ĵ������洢��װ�ء�
		�޸ĺ�ɾ����
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
		// ȡ����������
		int getParams(void)
		{
			return m_nParams;
		}
		// ��������ֵȡ��Ӧ�Ĳ������ַ���
		LPCTSTR GetParamByIndex(int nIndex)
		{
			if (nIndex < 0 || nIndex >= m_nParams || NULL == m_pParams)
				return NULL;
			return m_pParams[nIndex];
		}
		// �ж϶����Ƿ���Ч
		bool IsValid(void)
		{
			return bool(NULL != m_pmsParams && getParams() > 0 && ::lstrlen(m_szFuncName) > 0);
		}

		BOOL Remove(void);
		BOOL Load(void);
		BOOL Save(void);

		// ȡ���ò������ַ���
		LPTSTR GetCallParamMString(void)
		{
			return m_pmsParams;
		}
		// ���õ��ò������ַ���
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
		// ���ü����ģ������
		void setMonitorModule(CMonitorModule * pMonitorModule)
		{
			m_pMonitorModule = pMonitorModule;
		}
		// ���ú�������
		void setFuncName(LPCTSTR pszFuncName)
		{
			if (NULL != pszFuncName)
				::lstrcpyn(m_szFuncName, pszFuncName, MAX_PATH);
			else
				::lstrcpyn(m_szFuncName, _T(""), MAX_PATH);
		}

		TCHAR m_szFuncName[MAX_PATH];

	protected:

		// �����ǰ������ڲ�����
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

		// �����������
		// �ú������㵱ǰ��������Ķ��ַ���������Ϊ�䴴��һ��������
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
				{// ��������

					++nParams;
					p += lstrlen(p) + 1;
				}

				if (nParams > 0)
				{// ��������

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

		// �������ݸı��־
		BOOL m_bChanged;
		// �������еĲ���������
		int m_nParams;
		// ����������
		LPTSTR * m_pParams;
		// ���������ַ���
		LPTSTR m_pmsParams;
		// ���������ַ�������Ч�ֽ���
		DWORD m_uParamsBytes;
		// ���������ģ������ָ��
		CMonitorModule * m_pMonitorModule;
	}; // class CCallParam

	/*//////////////////////////////////////////////////////////////////////////////////////////////
	���ܼ�����
	    �����ģ����ò�����
	    �����ÿһ��ʵ������һ�������ģ�������м�⺯���ĵ��ò�������ͨ��������Լ���װ�ػ򱣴����
		�����ģ���м�⺯���ĵ��ò������������Զ���Щ���������й���
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
				{// �������һ��·���ָ�����ȷ��ģ������
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

		// ȡ�����ģ����ò����洢·��
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

		// װ�ؼ����ģ����Ϣ
		// ������
		//     bLoadAllParams - ָ���Ƿ���װ�ص�ǰ�����ģ�����к����ĵ��ò�����
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
			{// ѭ��ö�ټ����ģ������洢��Ϣ·���µ����в�����ֵ

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

		// ��������ģ����Ϣ
		BOOL Save(void)
		{
			for (int i = 0; i < MAX_CALL_PARAMS; i++)
			{
				m_CallParams[i].Save();
			}
			return TRUE;
		}
		// ��������ֵȡ��Ӧ�ĵ��ò�������
		CCallParam & GetCallParamByIndex(int nIndex)
		{
			if (nIndex < 0 || nIndex >= MAX_CALL_PARAMS)
				return m_CallParams[MAX_CALL_PARAMS]; // ����һ����Ч����

			return m_CallParams[nIndex];
		}
		// ȡ��һ��δʹ�õĵ��ò�����������
		int GetFreeCallParamObject(void)
		{
			for (int i = 0; i < MAX_CALL_PARAMS; i++)
			{
				if (!m_CallParams[i].IsValid())
					return i;
			}
			return -1;
		}
		// ���ݺ�����ȡ��Ӧ�ĵ��ò�������
		CCallParam & GetCallParam(LPCTSTR pszFunc)
		{
			if (NULL == pszFunc)
				return m_CallParams[MAX_CALL_PARAMS]; // ����һ����Ч����
			for (int i = 0; i < MAX_CALL_PARAMS; i++)
			{
				if (::lstrcmp(pszFunc, m_CallParams[i].m_szFuncName) == 0)
					return m_CallParams[i];
			}
			return m_CallParams[MAX_CALL_PARAMS]; // ����һ����Ч����
		}
		// ȡ�Ѿ������ĵ��ò�����������
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
		// ȡ��Ч�ĵ��ò�����������
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
		// һ��ģ���ں����������������
		enum { MAX_CALL_PARAMS = 256 };
		// ��ǰģ���ȫ·������
		TCHAR m_szPathName[MAX_PATH];
		// ��ǰģ�������
		TCHAR m_szName[MAX_PATH];
		// ��ǰģ�������к����ĵ��ò�������
		// ���һ�����������ڷ�����Ч����
		MCPP::CCallParam m_CallParams[MAX_CALL_PARAMS + 1];
	}; // class CMonitorModule

	// ������ò���
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

		// ͨ��ע���ֵӳ����󱣴������¼
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

	// ɾ����ǰ���������ĵ��ò�����¼
	inline BOOL CCallParam::Remove(void)
	{
		if (NULL == m_pMonitorModule ||
			::lstrlen(m_szFuncName) < 1)
			return FALSE;

		// ����ע���ֵӳ�������Զ�ɾ������ɾ����¼
		TCHAR szRegPath[MAX_PATH] = {0};
		m_pMonitorModule->GetRegPath(szRegPath, MAX_PATH);
		MiniHelper::CRegHelper::CRegValue rv(MCP_REGKEY, szRegPath, m_szFuncName, FALSE, TRUE);

		m_bChanged = TRUE;
		return TRUE;
	}

	// װ�ص�ǰ���������������ĵ��ò���
	inline BOOL CCallParam::Load(void)
	{
		if (NULL == m_pMonitorModule ||
			::lstrlen(m_szFuncName) < 1)
			return FALSE;

		_Clear();

		BOOL bRet = FALSE;

		// ��ȡ��ǰ����ģ���������ע���·��
		TCHAR szRegPath[MAX_PATH] = {0};
		m_pMonitorModule->GetRegPath(szRegPath, MAX_PATH);

		// ����һ��ע���ֵӳ�����װ�ص�ǰ���������Ĳ���
		MiniHelper::CRegHelper::CRegValue rv(MCP_REGKEY, szRegPath, m_szFuncName, FALSE);
		if (NO_ERROR == rv.Load() && rv.getValueBytes() > 0)
		{
			// ��������һ�����ַ��������浽m_pmsParams
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

	// ������ò���
	// ���ܼ�����
	//     �ú������ڼ򻯵��ò����ı�����̡���ʵ�ʵļ���������͵��Թ����У������ڼ�������ú�����
	// ��ڳ����øú�����ֻ�����ʵ�ʻ���������һ�Σ�����ע�͸ú����ĵ��ô��룬Ȼ�󼴿���
	// MonitorCall.exe����ȫģ��ʵ�ʻ����ĵ��ò����Լ�������е��Ե��á�
	// ������
	//     pszModuleName [in] - ָ�������ģ������
	//     pszFuncName [in] - ָ���������������
	//     pmsParams [in] - ָ�����������������ʱ����Ĳ���ָ�롣
	// ����ֵ��
	//     ����ɹ��򷵻�pmsParams���ֽ��������򷵻�ֵ<0��
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
