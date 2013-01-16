
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef DRAGONFLOW_FUNCGENERAL
#define DRAGONFLOW_FUNCGENERAL

#include <string>
#include <list>

using namespace std;

//#define SV_VER_62
#define KEY_PATH		"SOFTWARE\\siteview\\siteviewecc"

#define KEY_NAME		"root_path_7"

#define  KEY_CONNECTSTRING  "ConnectionString"

void CheckTempPath();

/////////////////////////////////////////////////////////////////////////////
//���ܣ��õ���װ·�� 
CString FuncGetInstallPath();
CString FuncGetInstallRootSCPath();
CString FuncGetInstallPath(CString strCid);
/////////////////////////////////////////////////////////////////////////////
CString FuncGetInstallRootPath();

CString FuncGetStringFromIDS(const char * szIDS);
CString FuncGetStringFromIDS(const char* szSection, const char * szIDS);

CString GetCIDFromCookie();
/////////////////////////////////////////////////////////////////////////////
void WSA_Free();
int WSA_Init();
/////////////////////////////////////////////////////////////////////////////
//�õ�ini�ļ��е�һ��section�ľ���һ��keyֵ,����Ϊint��
int FuncGetProfileIntBy(CString strSection, CString strKey, CString strFileName);

int DFNParser_GetPrivateProfileInt(LPCTSTR lpAppName,  // section name
  LPCTSTR lpKeyName,  // key name
  INT nDefault,       // return value if key name not found
  LPCTSTR lpFileName  // initialization file name
);
/////////////////////////////////////////////////////////////////////////////
int DFN_GetPrivateProfileInt(
  LPCTSTR lpAppName,  // section name
  LPCTSTR lpKeyName,  // key name
  INT nDefault,       // return value if key name not found
  LPCTSTR lpFileName  // initialization file name
);
//////////////////////////////////////////////////////////////////////////////

DWORD DFNParser_GetPrivateProfileString(
  LPCTSTR lpAppName,        // section name
  LPCTSTR lpKeyName,        // key name
  LPCTSTR lpDefault,        // default string
  LPTSTR lpReturnedString,  // destination buffer
  DWORD nSize,              // size of destination buffer
  LPCTSTR lpFileName        // initialization file name
 );
DWORD DFN_GetPrivateProfileString(
  LPCTSTR lpAppName,        // section name
  LPCTSTR lpKeyName,        // key name
  LPCTSTR lpDefault,        // default string
  LPTSTR lpReturnedString,  // destination buffer
  DWORD nSize,              // size of destination buffer
  LPCTSTR lpFileName        // initialization file name
 );

/////////////////////////////////////////////////////////////////////////////
BOOL DFN_WritePrivateProfileString(
  LPCTSTR lpAppName,  // section name
  LPCTSTR lpKeyName,  // key name
  LPCTSTR lpString,   // string to add
  LPCTSTR lpFileName  // initialization file
);
/////////////////////////////////////////////////////////////////////////////
int FuncGetCountByMark(CString strWholeString, CString strMark);
/////////////////////////////////////////////////////////////////////////////
//�õ����б���ַ�����nIndex�����ߵ��ַ���
//strWholeString:Ϊ���б�ǵ������ַ���,
//strMark:Ϊ�ָ��ַ������õı��
//nIndex:ȡ�õڼ��������ߵ��ַ�,index��1��ʼ
CString FuncGetLeftStringByMark(CString strWholeString,	
                                CString strMark, int nIndex);
/////////////////////////////////////////////////////////////////////////////
CString FuncGetProfileStringBy(CString strSection, CString strKey, 
                               CString strFileName);
/////////////////////////////////////////////////////////////////////////////
//��CTimeʱ��ת�����ַ���ʱ��
CString TimeToString(CTime Time);
/////////////////////////////////////////////////////////////////////////////
CString GetValueFromList(const CString strName,CStringList& strList);
/////////////////////////////////////////////////////////////////////////////
void CreateListByStringandMark(CString strSource ,CStringList& strDesList, 
                               CString strMark);
/////////////////////////////////////////////////////////////////////////////
void WriteLog(CString strLog, int nLogType = 0, CString strLogFile = _T(""));
/*
DWORD DFN_GetPrivateProfileSectionNames(
  LPTSTR lpszReturnBuffer,  // return buffer
  DWORD nSize,              // size of return buffer
  LPCTSTR lpFileName        // initialization file name
);
*/
void FuncGetCompanyName(CString& strRet);

void FuncGetCompanyUrl(CString& strRet);

void SortList(CStringList &KeyList, CStringList &ValList, BOOL bAsc);
void _Sort(struct _rec *r,int n, BOOL bAsc);

struct _rec
{
	CString key;
	CString val;
};


CString FuncGetRightString(CString strWholeString, CString strMark);
bool MakeCharByString(char *pOut,int &nOutSize,CString strInput );
bool MakeStringListByChar(CStringList& pList, const char * pInput );
string GetResourceValue(const string rkey);
static std::string  gRoot_path="";
#endif
