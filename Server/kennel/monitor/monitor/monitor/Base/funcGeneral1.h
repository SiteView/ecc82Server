
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
//功能：得到安装路径 
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
//得到ini文件中的一个section的具体一个key值,返回为int型
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
//得到含有标记字符串第nIndex标记左边的字符，
//strWholeString:为含有标记的整个字符串,
//strMark:为分隔字符串所用的标记
//nIndex:取得第几个标记左边的字符,index从1开始
CString FuncGetLeftStringByMark(CString strWholeString,	
                                CString strMark, int nIndex);
/////////////////////////////////////////////////////////////////////////////
CString FuncGetProfileStringBy(CString strSection, CString strKey, 
                               CString strFileName);
/////////////////////////////////////////////////////////////////////////////
//将CTime时间转换成字符串时间
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
