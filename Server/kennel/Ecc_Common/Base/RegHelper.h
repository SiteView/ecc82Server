/*++

Copyright (c) 2005, Mayasoft Corporation All rights reserved

��Ŀ���ƣ�
����ͨ��

�ļ�����
    RegHelper.h

������
    
�ļ�������
    ʵ����MiniHelper�����ռ��е�ע��������װ��ע��������CRegHelper�ྲ̬��Ա
    �������CRegValue��Ա����з�װ��

�汾�ţ�
    Ver 0.0.2

�޸���ʷ��
    Ver 0.0.1 (���� 2005/05/10)
        ʵ����CRegHelper��ĵ�һ����ʼ�汾���ð�����ʵ����CRegValue��Ա�࣬�Դ�
		��װ�˶�ע���ֵ�Ĵ�������ȡ�͸�д�Ĳ�����
        �ð汾�Ѿ���.exe��ͨ�����ֹ�BoundsChecker��顣
	Ver 0.0.2 (���� 2007/07/26)
		ͨ��ָ���Զ�ɾ����־ʵ����CRegValue������������ʱ��ɾ����ֵ�Ĺ��ܡ�
        �ð汾�Ѿ���.exe��ͨ�����ֹ�BoundsChecker��顣

--*/

#if !defined(MINIHELPER_REGHELPER_H_INCLUDED_)
#define MINIHELPER_REGHELPER_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <tchar.h>

/*++

���ܼ�����
    �������������ռ䡣
    �������ռ���ֻ������inline����ʽʵ�ֵ�ͨ�ò�����װ�࣬���ڸ������ռ������
    ��Ӧ������inline����ʽ��һЩ���ò����ļ򵥷�װ����Щ��ϵ���������಻Ӧ����
    �������ռ��н��ж����ʵ�֡�

--*/
namespace MiniHelper
{

	/*++

	���ܼ�����
	    ע�����������ࡣ
	    �����Ծ�̬��Ա�����ͳ�Ա�����ʽ��һЩ����ע����������˼򵥷�װ��

	--*/
	class CRegHelper
	{
	public:

		// ע���ֵ����ö��
		enum ERegValueType
		{
			// ������
			rvtNone		= 0, 
			// �ַ���
			rvtString	= 1, 
			// �޷��ų�����
			rvtDWORD	= 2,
			// ������������
			rvtBinary	= 3,
		};

		/*++

		���ܼ�����
			ע���ֵ��װ�ࡣ
			���ཫһ��ע���ֵ����һ����������˷�װ��һ��CRegValue������Ծ���
			����һ��ע���ֵ�������Ժ��CRegValue����ĸ�ֵ��ȡֵ��������ʵ��
			��ע���ֵ��CRegValue��װ��ע���ֵֻ֧���ַ����������Ͷ��������ݿ�
			�������͡�

		--*/
		class CRegValue
		{

		friend class CRegValue;
		friend class CRegHelper;

		public:

			CRegValue(
				HKEY hRootKey = NULL, 
				LPCTSTR pszPath = NULL, 
				LPCTSTR pszValueName = NULL,
				BOOL bAutoLoad = TRUE,
				BOOL bAutoRemove = FALSE)
			/*++

			���캯��

			���ܼ�����
				����ָ���Ĳ�������ǰ������ָ����ע���ֵ���о�������������Ե�ǰ��
				������и�ֵ��ȡֵ��������ʵ�ʵ�ע���ֵ��ȥ��

			������
				hRootKey
					[in] ָ��Ҫ������ע���ֵ���ڵ�ע��������
				pszPath
					[in] ָ��Ҫ������ע���ֵ���ڵ�ע���·����
				pszValueName
					[in] ָ��Ҫ������ע���ֵ�����ơ�
				bAutoLoad
					[in] ָ���ڹ�������ʱ���Ƿ��Զ�װ��ָ����ע���ֵ��
				bAutoRemove
					[in] ָ���Ƿ��ڶ���������ʱ���Զ�ɾ����ֵ��

			����ֵ����

			����
				�ú��������������Ϊ��ڲ�����Ч������󱣳���Чֵ״̬�������κ�
				ע���ֵ������

			--*/
			{

				// ֵ����ΪrvtNone��ʾֵ��Ч
				m_eValType = rvtNone;
				m_hRootKey = hRootKey;

				m_pszPath = NULL;
				m_pszValueName = NULL;
				m_pValData = NULL;
				m_uValSize = 0;
				m_bAutoRemove = bAutoRemove;

				if (NULL != pszPath)
				{
					int nLen = ::lstrlen(pszPath);
					m_pszPath = new TCHAR[nLen + 1];
					if (NULL != m_pszPath)
						::lstrcpyn(m_pszPath, pszPath, nLen + 1);
				}

				if (NULL != pszValueName)
				{
					int nLen = ::lstrlen(pszValueName);
					m_pszValueName = new TCHAR[nLen + 1];
					if (NULL != m_pszValueName)
						::lstrcpyn(m_pszValueName, pszValueName, nLen + 1);
				}

				if (bAutoLoad)
					Load();
			}

			virtual ~CRegValue()
			{
				ClearValue();
				if (m_bAutoRemove)
				{// ����ڹ������ʱָ����Ҫɾ��������ע����ֵ...

					if (NULL != m_hRootKey && 
						::lstrlen(m_pszPath) > 0 &&
						::lstrlen(m_pszValueName) > 0 &&
						NULL != m_pszValueName)
					{

						LONG lRetVal = NO_ERROR;

						HKEY hCurKey = NULL;
						DWORD dwDis = 0;
						lRetVal = ::RegCreateKeyEx(
							m_hRootKey,
							m_pszPath,
							0,
							NULL,
							REG_OPTION_NON_VOLATILE,
							KEY_ALL_ACCESS, 
							NULL,
							&hCurKey,
							&dwDis);

						if(lRetVal == ERROR_SUCCESS && NULL != hCurKey)
						{
							::RegDeleteValue(hCurKey, m_pszValueName);
							::RegCloseKey(hCurKey);
						}
					}
				}
				if (NULL != m_pszPath)
					delete[] m_pszPath;
				if (NULL != m_pszValueName)
					delete[] m_pszValueName;
			}

			// ȡֵ��������
			enum ERegValueType getValueType(void)
			{
				return m_eValType;
			}// getValueType(void)

			// ȡֵ���ֽڴ�С
			DWORD getValueBytes(void)
			{
				return m_uValSize;
			}

			// �����ǰ�����ֵ
			void ClearValue(void)
			{
				if (NULL != m_pValData)
				{
					delete[] m_pValData;
					m_pValData = NULL;
				}
				m_eValType = rvtNone;
				m_uValSize = 0;
			}// ClearValue()

			// װ��ע���ֵ������
			LONG Load(void)
			/*++

			װ��ע���ֵ������

			���ܼ�����
				���ݹ��캯������ָ����λ�ý�ע���ֵ�����������ָ����ע���
				ֵ�����ڣ��ú������ò���Ӱ�����ԭ�е�ֵ��

			��������

			����ֵ��
				����ɹ�����NO_ERROR�����򷵻�ϵͳ��׼�Ĵ�����롣

			����
				�������ʹ�ñ�׼��ϵͳ������붨�壬����μ�ϵͳ������붨�塣

			--*/
			{
				if (NULL == m_hRootKey || 
					NULL == m_pszPath || 
					::lstrlen(m_pszPath) < 1)
					return ERROR_INVALID_PARAMETER;

				LONG lRetVal = NO_ERROR;

				HKEY hCurKey = NULL;
				DWORD dwDis = 0;
				lRetVal = ::RegOpenKeyEx(
					m_hRootKey,
					m_pszPath,
					REG_OPTION_NON_VOLATILE,
					KEY_QUERY_VALUE,
					&hCurKey);

				if(lRetVal != ERROR_SUCCESS || NULL == hCurKey)
					return lRetVal;

				ClearValue();

				// ȡ���ݵ����ͼ���С
				DWORD uValType = REG_NONE;
				DWORD uValSize = 0;
				lRetVal = ::RegQueryValueEx(
					hCurKey,
					m_pszValueName,
					NULL,
					&uValType,
					NULL,
					&uValSize);
				if (lRetVal != ERROR_SUCCESS)
				{
					::RegCloseKey(hCurKey);
					return lRetVal;
				}

				// ����ֵ��С�����ڴ�
				m_pValData = new BYTE[uValSize + sizeof(TCHAR)];
				if (NULL != m_pValData)
				{

					::memset(m_pValData, 0, uValSize + sizeof(TCHAR));

					lRetVal = ::RegQueryValueEx(
						hCurKey,
						m_pszValueName,
						NULL,
						&uValType,
						m_pValData,
						&uValSize);
				}

				::RegCloseKey(hCurKey);

				if (lRetVal != ERROR_SUCCESS)
				{
					ClearValue();
					return lRetVal;
				}

				// ����ֵ�����Ƿ�Ϊ֧�ֵ�����
				switch(uValType)
				{
				case REG_SZ:
				case REG_EXPAND_SZ:
					// �ַ���
					m_eValType = rvtString;
					m_uValSize = uValSize;
					break;

				case REG_DWORD:
				case REG_DWORD_BIG_ENDIAN:
					// ����
					m_eValType = rvtDWORD;
					m_uValSize = uValSize;
					break;
				case REG_BINARY:
					// ���������ݿ�
					m_eValType = rvtBinary;
					m_uValSize = uValSize;
					break;
				default:
					// ��֧�ֵ���������
					lRetVal = ERROR_INVALID_DATATYPE;
					break;
				}

				if (ERROR_SUCCESS != lRetVal)
					ClearValue();

				return lRetVal;

			}// Load()

			// ����ǰ���������ֵ���浽������ע���ֵ
			LONG Save(void)
			/*++

			����ǰ���������ֵ���浽������ע���ֵ

			���ܼ�����
				����ǰ���������ֵ���浽������ע���ֵ�������ע���ֵ��������
				����֮��

			��������

			����ֵ��
				����ɹ�����NO_ERROR�����򷵻�ϵͳ��׼�Ĵ�����롣

			����
				�������ʹ�ñ�׼��ϵͳ������붨�壬����μ�ϵͳ������붨�塣

			--*/
			{
				if (NULL == m_hRootKey || 
					::lstrlen(m_pszPath) < 1 ||
					NULL == m_pValData ||
					m_uValSize < 1 ||
					rvtNone == m_eValType)
					return ERROR_INVALID_PARAMETER;

				LONG lRetVal = NO_ERROR;

				HKEY hCurKey = NULL;
				DWORD dwDis = 0;
				lRetVal = ::RegCreateKeyEx(
					m_hRootKey,
					m_pszPath,
					0,
					NULL,
					REG_OPTION_NON_VOLATILE,
					KEY_ALL_ACCESS, 
					NULL,
					&hCurKey,
					&dwDis);

				if(lRetVal != ERROR_SUCCESS || NULL == hCurKey)
					return lRetVal;

				// ����ֵת��
				DWORD uValType = REG_NONE;
				switch(m_eValType)
				{
				case rvtString:
					uValType = REG_SZ;
					break;
				case rvtDWORD:
					uValType = REG_DWORD;
					break;
				case rvtBinary:
					uValType = REG_BINARY;
					break;
				}

				lRetVal = ::RegSetValueEx(
					hCurKey,
					m_pszValueName,
					0,
					uValType,
					m_pValData,
					m_uValSize);

				::RegCloseKey(hCurKey);

				return lRetVal;

			}// Save()

			// �������ֵ����Ϊָ�����ַ���
			BOOL SetValue(LPCTSTR pszVal)
			{
				if (NULL == pszVal)
					pszVal = _T("");

				if (NULL != m_pValData)
				{
					if (rvtString == m_eValType)
					{
						if (::lstrcmp(LPCTSTR(m_pValData), pszVal) == 0)
							return TRUE; // ԭֵ����ֵ��ͬ
					}
					ClearValue();
				}

				m_uValSize = (::lstrlen(pszVal) + 1) * sizeof(TCHAR);
				m_pValData = new BYTE[m_uValSize];
				if (NULL == m_pValData)
				{
					ClearValue();
					return FALSE;
				}
				::memcpy(m_pValData, pszVal, m_uValSize);
				m_eValType = rvtString;
				return TRUE;
			}// SetValue(LPCTSTR pszVal)

			// �������ֵ����Ϊָ��������
			BOOL SetValue(DWORD uVal)
			{
				if (NULL != m_pValData)
				{
					if (rvtDWORD == m_eValType)
					{
						*LPDWORD(m_pValData) = uVal;
						return TRUE; // ԭֵ����ֵ��ͬ
					}
					ClearValue();
				}

				m_uValSize = sizeof(DWORD);
				m_pValData = new BYTE[m_uValSize];
				if (NULL == m_pValData)
				{
					ClearValue();
					return FALSE;
				}
				*LPDWORD(m_pValData) = uVal;
				m_eValType = rvtDWORD;
				return TRUE;
			}// SetValue(DWORD uVal)

			// �������ֵ����Ϊָ���Ķ��������ݿ�
			BOOL SetValue(LPBYTE pValData, DWORD uValSize)
			{
				if (NULL == pValData || 0 == uValSize)
				{
					// pValDataΪNULL��uValSizeΪ0�����������ֵ����
					ClearValue();
					return FALSE;
				}

				if (NULL != m_pValData)
				{
					if (rvtBinary == m_eValType && 
						uValSize == m_uValSize &&
						::memcmp(m_pValData, pValData, uValSize) == 0)
						return TRUE; // ԭֵ����ֵ��ͬ
					ClearValue();
				}

				m_uValSize = uValSize;
				m_pValData = new BYTE[m_uValSize];
				if (NULL == m_pValData)
				{
					ClearValue();
					return FALSE;
				}

				::memcpy(m_pValData, pValData, m_uValSize);
				m_eValType = rvtBinary;

				return TRUE;

			}// SetValue(LPBYTE pValData, DOWRD uValSize)

			// ȡ���������ݿ�ֵ
			// ֵ���Ͳ�ƥ�佫ʧ�ܡ�
			BOOL GetValue(LPBYTE pVal, int nMaxSize)
			{
				if (rvtBinary != m_eValType || 
					NULL == pVal || 
					nMaxSize < 1 ||
					NULL == m_pValData)
					return FALSE;

				::memcpy(pVal, m_pValData, min(nMaxSize, int(m_uValSize)));

				return TRUE;
			}// GetValue(LPTSTR pszVal, int nMaxLen)

			// ȡ�ַ���ֵ
			// ֵ���Ͳ�ƥ�佫ʧ�ܡ�
			BOOL GetValue(LPTSTR pszVal, int nMaxLen)
			{
				if (rvtString != m_eValType || 
					NULL == pszVal || 
					nMaxLen < 1 ||
					NULL == m_pValData)
					return FALSE;

				::lstrcpyn(pszVal, LPCTSTR(m_pValData), nMaxLen);

				return TRUE;
			}// GetValue(LPTSTR pszVal, int nMaxLen)

			// ȡ����ֵ
			// ֵ���Ͳ�ƥ�佫ʧ�ܡ�
			BOOL GetValue(LPDWORD pVal)
			{
				if (rvtDWORD != m_eValType || 
					NULL == pVal || 
					NULL == m_pValData)
					return FALSE;

				*pVal = *LPDWORD(m_pValData);

				return TRUE;
			}// GetValue(LPDWORD pVal)

			// �����ַ������͵�˫��ֵ������
			LPCTSTR operator=(LPCTSTR pszVal)
			{
				if (SetValue(pszVal))
					Save();
				return pszVal;
			}
			operator LPCTSTR()
			{
				if (rvtString == m_eValType && NULL != m_pValData)
					return LPCTSTR(m_pValData);
				return _T("");
			}

			// �����������͵�˫��ֵ������
			DWORD operator=(const DWORD uVal)
			{
				if (SetValue(uVal))
					Save();
				return uVal;
			}
			operator DWORD()
			{
				if (rvtDWORD == m_eValType && NULL != m_pValData)
					return *LPDWORD(m_pValData);
				return 0;
			}

			// �����������͵�˫��ֵ������
			long operator=(const long nVal)
			{
				*this = DWORD(nVal);
				return nVal;
			}
			operator long()
			{
				return long(DWORD(*this));
			}

			// �����������͵�˫��ֵ������
			int operator=(const int nVal)
			{
				*this = DWORD(nVal);
				return nVal;
			}
			operator int()
			{
				return int(DWORD(*this));
			}

			// ���ز������͵�˫��ֵ������
			bool operator=(const bool bVal)
			{
				*this = DWORD(bVal);
				return bVal;
			}
			operator bool()
			{
				return bool(DWORD(*this) != 0);
			}

			// ����CRegValue����ֵ������
			CRegValue & operator=(const CRegValue & RegVal)
			{
				// ���������ǰ��������
				ClearValue();
				if (NULL != m_pszPath)
					delete[] m_pszPath;
				if (NULL != m_pszValueName)
					delete[] m_pszValueName;

				// ����ע���·����ע���ֵ���Ƶ���Ϣ
				if (NULL != RegVal.m_pszPath)
				{
					int nLen = ::lstrlen(RegVal.m_pszPath);
					m_pszPath = new TCHAR[nLen + 1];
					if (NULL != m_pszPath)
						::lstrcpyn(m_pszPath, RegVal.m_pszPath, nLen);
				}
				if (NULL != RegVal.m_pszValueName)
				{
					int nLen = ::lstrlen(RegVal.m_pszValueName);
					m_pszValueName = new TCHAR[nLen + 1];
					if (NULL != m_pszValueName)
						::lstrcpyn(m_pszValueName, RegVal.m_pszValueName, nLen);
				}
				m_hRootKey = RegVal.m_hRootKey;

				// ���ƶ���ֵ
				if (NULL != RegVal.m_pValData && RegVal.m_uValSize > 0)
				{
					m_pValData = new BYTE[RegVal.m_uValSize];
					if (NULL != m_pValData)
					{
						::memcpy(m_pValData, RegVal.m_pValData, RegVal.m_uValSize);
						m_uValSize = RegVal.m_uValSize;
						m_eValType = RegVal.m_eValType;
					}
				}

				return *this;
			}

		protected:

			// ��ǰע���ֵ�����ע������
			HKEY m_hRootKey;
			// ��ǰע���ֵ������ע����·��
			LPTSTR m_pszPath;
			// ��ǰע���ֵ������
			LPTSTR m_pszValueName;
			// ��ǰע���ֵ��ֵ����
			LPBYTE m_pValData;
			// ��ǰע���ֵ��ֵ��С
			DWORD m_uValSize;
			// �Ƿ��ڶ���������ʱ���Զ�ɾ����ֵ
			BOOL m_bAutoRemove;
			// ��ǰע���ֵ��ֵ����
			enum ERegValueType m_eValType;

		};// class CRegValue

	};// class CShellHelper

}; // namespace MiniHelper

#endif // #if !defined(MINIHELPER_REGHELPER_H_INCLUDED_)