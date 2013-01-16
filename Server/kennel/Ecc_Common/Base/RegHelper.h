/*++

Copyright (c) 2005, Mayasoft Corporation All rights reserved

项目名称：
　　通用

文件名：
    RegHelper.h

依赖：
    
文件描述：
    实现了MiniHelper命名空间中的注册表操作封装，注册表操作以CRegHelper类静态成员
    函数结合CRegValue成员类进行封装。

版本号：
    Ver 0.0.2

修改历史：
    Ver 0.0.1 (龚超 2005/05/10)
        实现了CRegHelper类的第一个初始版本，该版完整实现了CRegValue成员类，以此
		封装了对注册表值的创建、读取和改写的操作。
        该版本已经在.exe中通过了手工BoundsChecker检查。
	Ver 0.0.2 (龚超 2007/07/26)
		通过指定自动删除标志实现了CRegValue对象在析构的时候删除键值的功能。
        该版本已经在.exe中通过了手工BoundsChecker检查。

--*/

#if !defined(MINIHELPER_REGHELPER_H_INCLUDED_)
#define MINIHELPER_REGHELPER_H_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <tchar.h>

/*++

功能简述：
    迷你助手命名空间。
    该命名空间中只包含以inline类形式实现的通用操作封装类，属于该命名空间域的类
    都应该是以inline类形式对一些常用操作的简单封装，那些体系化的重型类不应该在
    该命名空间中进行定义和实现。

--*/
namespace MiniHelper
{

	/*++

	功能简述：
	    注册表操作助手类。
	    该类以静态成员函数和成员类的形式对一些常规注册操作进行了简单封装。

	--*/
	class CRegHelper
	{
	public:

		// 注册表值类型枚举
		enum ERegValueType
		{
			// 无类型
			rvtNone		= 0, 
			// 字符串
			rvtString	= 1, 
			// 无符号长整型
			rvtDWORD	= 2,
			// 二进制数据型
			rvtBinary	= 3,
		};

		/*++

		功能简述：
			注册表值封装类。
			该类将一个注册表值看做一个对象进行了封装。一个CRegValue对象可以镜像
			关联一个注册表值，关联以后对CRegValue对象的赋值和取值都将镜像到实际
			的注册表值。CRegValue封装的注册表值只支持字符串、整数和二进制数据块
			三种类型。

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

			构造函数

			功能简述：
				根据指定的参数将当前对象与指定的注册表值进行镜像关联，后续对当前对
				象的所有赋值或取值都将镜像到实际的注册表值中去。

			参数：
				hRootKey
					[in] 指定要关联的注册表值所在的注册表根键。
				pszPath
					[in] 指定要关联的注册表值所在的注册表路径。
				pszValueName
					[in] 指定要关联的注册表值的名称。
				bAutoLoad
					[in] 指定在构造对象的时候是否自动装载指定的注册表值。
				bAutoRemove
					[in] 指定是否在对象析构的时候自动删除键值。

			返回值：无

			错误：
				该函数不报错，如果因为入口参数无效，则对象保持无效值状态，不与任何
				注册表值关联。

			--*/
			{

				// 值类型为rvtNone表示值无效
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
				{// 如果在构造对象时指定了要删除关联的注册表键值...

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

			// 取值类型属性
			enum ERegValueType getValueType(void)
			{
				return m_eValType;
			}// getValueType(void)

			// 取值的字节大小
			DWORD getValueBytes(void)
			{
				return m_uValSize;
			}

			// 清除当前对象的值
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

			// 装载注册表值到对象
			LONG Load(void)
			/*++

			装载注册表值到对象

			功能简述：
				根据构造函数参数指定的位置将注册表值读到对象，如果指定的注册表
				值不存在，该函数调用不会影响对象原有的值。

			参数：无

			返回值：
				如果成功返回NO_ERROR，否则返回系统标准的错误代码。

			错误：
				错误代码使用标准的系统错误代码定义，详情参见系统错误代码定义。

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

				// 取数据的类型及大小
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

				// 根据值大小分配内存
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

				// 分析值类型是否为支持的类型
				switch(uValType)
				{
				case REG_SZ:
				case REG_EXPAND_SZ:
					// 字符串
					m_eValType = rvtString;
					m_uValSize = uValSize;
					break;

				case REG_DWORD:
				case REG_DWORD_BIG_ENDIAN:
					// 整数
					m_eValType = rvtDWORD;
					m_uValSize = uValSize;
					break;
				case REG_BINARY:
					// 二进制数据块
					m_eValType = rvtBinary;
					m_uValSize = uValSize;
					break;
				default:
					// 不支持的数据类型
					lRetVal = ERROR_INVALID_DATATYPE;
					break;
				}

				if (ERROR_SUCCESS != lRetVal)
					ClearValue();

				return lRetVal;

			}// Load()

			// 将当前对象里面的值保存到关联的注册表值
			LONG Save(void)
			/*++

			将当前对象里面的值保存到关联的注册表值

			功能简述：
				将当前对象里面的值保存到关联的注册表值，如果该注册表值不存在则
				创建之。

			参数：无

			返回值：
				如果成功返回NO_ERROR，否则返回系统标准的错误代码。

			错误：
				错误代码使用标准的系统错误代码定义，详情参见系统错误代码定义。

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

				// 类型值转换
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

			// 将对象的值设置为指定的字符串
			BOOL SetValue(LPCTSTR pszVal)
			{
				if (NULL == pszVal)
					pszVal = _T("");

				if (NULL != m_pValData)
				{
					if (rvtString == m_eValType)
					{
						if (::lstrcmp(LPCTSTR(m_pValData), pszVal) == 0)
							return TRUE; // 原值与新值相同
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

			// 将对象的值设置为指定的整数
			BOOL SetValue(DWORD uVal)
			{
				if (NULL != m_pValData)
				{
					if (rvtDWORD == m_eValType)
					{
						*LPDWORD(m_pValData) = uVal;
						return TRUE; // 原值与新值相同
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

			// 将对象的值设置为指定的二进制数据块
			BOOL SetValue(LPBYTE pValData, DWORD uValSize)
			{
				if (NULL == pValData || 0 == uValSize)
				{
					// pValData为NULL或uValSize为0可以用来清空值内容
					ClearValue();
					return FALSE;
				}

				if (NULL != m_pValData)
				{
					if (rvtBinary == m_eValType && 
						uValSize == m_uValSize &&
						::memcmp(m_pValData, pValData, uValSize) == 0)
						return TRUE; // 原值与新值相同
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

			// 取二进制数据块值
			// 值类型不匹配将失败。
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

			// 取字符串值
			// 值类型不匹配将失败。
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

			// 取整数值
			// 值类型不匹配将失败。
			BOOL GetValue(LPDWORD pVal)
			{
				if (rvtDWORD != m_eValType || 
					NULL == pVal || 
					NULL == m_pValData)
					return FALSE;

				*pVal = *LPDWORD(m_pValData);

				return TRUE;
			}// GetValue(LPDWORD pVal)

			// 重载字符串类型的双向赋值操作符
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

			// 重载整数类型的双向赋值操作符
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

			// 重载整数类型的双向赋值操作符
			long operator=(const long nVal)
			{
				*this = DWORD(nVal);
				return nVal;
			}
			operator long()
			{
				return long(DWORD(*this));
			}

			// 重载整数类型的双向赋值操作符
			int operator=(const int nVal)
			{
				*this = DWORD(nVal);
				return nVal;
			}
			operator int()
			{
				return int(DWORD(*this));
			}

			// 重载布尔类型的双向赋值操作符
			bool operator=(const bool bVal)
			{
				*this = DWORD(bVal);
				return bVal;
			}
			operator bool()
			{
				return bool(DWORD(*this) != 0);
			}

			// 重载CRegValue对象赋值操作符
			CRegValue & operator=(const CRegValue & RegVal)
			{
				// 彻底清除当前对象内容
				ClearValue();
				if (NULL != m_pszPath)
					delete[] m_pszPath;
				if (NULL != m_pszValueName)
					delete[] m_pszValueName;

				// 复制注册表路径和注册表值名称等信息
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

				// 复制对象值
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

			// 当前注册表值对象的注册表根键
			HKEY m_hRootKey;
			// 当前注册表值关联的注册表键路径
			LPTSTR m_pszPath;
			// 当前注册表值的名称
			LPTSTR m_pszValueName;
			// 当前注册表值的值数据
			LPBYTE m_pValData;
			// 当前注册表值的值大小
			DWORD m_uValSize;
			// 是否在对象析构的时候自动删除键值
			BOOL m_bAutoRemove;
			// 当前注册表值的值类型
			enum ERegValueType m_eValType;

		};// class CRegValue

	};// class CShellHelper

}; // namespace MiniHelper

#endif // #if !defined(MINIHELPER_REGHELPER_H_INCLUDED_)