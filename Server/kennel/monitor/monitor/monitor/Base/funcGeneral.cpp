/*
 * 
 */

#include "stdafx.h"
#include "stdio.h"
#include <afx.h>
#include <afxsock.h>
#include <direct.h>
#include "funcGeneral.h"
#include "../kennel/svdb/svapi/svapi.h"
#include <string>
#include "stlini.h"

#define MAX_BIN  16
#define	MAX_LEN					1024*16
#define BUFF_SIZE               1024*64

#pragma comment(lib, "./../bin/kennel/svapi.lib")

//////////////////////////////////////////////////////////////////////////////////
/////////////以下用于加密和解密  and by Harvey in 4/28/2002///////////////////////
//std::string   gRoot_path ="";
void CheckTempPath()
{
}

CString FuncGetInstallPath()
{
	CString strRet = _T("");
	strRet= GetSiteViewRootPath().c_str();
	return strRet;
}
/////////////////////////////////////////////////////////////////////////////////////////////
CString FuncGetInstallRootSCPath()
{
	return  FuncGetInstallPath();
}

/////////////////////////////////////////////////////////////////////////////////////////////
CString FuncGetInstallPath(CString strDir)
{
	CString strAppRoot= FuncGetInstallPath();
	CString strRet="";

	if(strAppRoot.GetLength()>0)
	{
		if(strDir != "")
			strRet.Format("%s\\%s",strAppRoot, strDir);
		else
		{
			CString strCid = _T("");
			strCid = GetCIDFromCookie();

			if(strCid != "")
				strRet.Format("%s\\%s",strAppRoot, strCid);
			else
				strRet.Format("%s",strAppRoot);
		}
	}

	return strRet;
}

CString FuncGetStringFromIDS(const char * szIDS)
{
	CString strRet="";
	std::string rKey;
	if(szIDS)
		rKey = szIDS;
	std::string sRet= GetResourceValue(rKey);
	strRet = sRet.c_str();
	return strRet;
}

/////////////////////////////////////////////////////////////////////////////////////////////
CString FuncGetStringFromIDS(const char* szSection, const char * szIDS)
{
	CString strRet="";
	std::string rKey;
	if(szIDS)
		rKey = szIDS;
	std::string sRet= GetResourceValue(rKey);
	strRet = sRet.c_str();
	return strRet;
}
/////////////////////////////////////////////////////////////////////////////////////////////
CString GetCIDFromCookie()
{
	CString strCid= _T("");
	char *hc=getenv("HTTP_COOKIE"); 
	if (hc == NULL) 
		return strCid;

	CString strSource = hc;

	int nLeftIndex = strSource.Find("cid=", 0);
	if (nLeftIndex == -1)
		return strCid;

	int nRightIndex = strSource.Find(";", nLeftIndex);
	if (nRightIndex == -1) {// may be only two cookies.
		strCid = strSource.Right(strSource.GetLength() - nLeftIndex - 4);
		return strCid;
	}

	int nLength = static_cast<int>(strlen("cid="));

	CString str = strSource.Mid(nLeftIndex + nLength , nRightIndex - nLeftIndex - nLength);
	strCid = str;

	return strCid;
}

CString FuncGetInstallRootPath()
{
	CString strRet = _T("");
	HKEY hKey = NULL;
	LONG lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE,KEY_PATH,0,KEY_READ,&hKey);
	if(lRet != ERROR_SUCCESS)
		return strRet;
	
	LPTSTR  lpName = NULL,
 			lpValue = NULL;
	CString strSubKey = _T(""),
 			strValKey = _T("");
	DWORD   dwIndex = 0,
			dwS1 = 255,
			dwS2 = 255,
			dwType = REG_SZ;
	BOOL    bFindFlag = FALSE;
	char szAppRoot[255] = {0};
	while(lRet == ERROR_SUCCESS)
	{
		lpName = strSubKey.GetBuffer(dwS1);
		lpValue = strValKey.GetBuffer(dwS2);
		lRet = RegEnumValue(hKey, dwIndex++, lpName , &dwS1,NULL, &dwType ,(LPBYTE)lpValue, &dwS2);
		dwS1 = 255;
		dwS2 = 255;
		if(lRet == ERROR_NO_MORE_ITEMS)
			break;
		if(strSubKey.CompareNoCase(KEY_NAME) == 0)
		{
			bFindFlag = TRUE;
			strcpy(szAppRoot,strValKey);
			break;
		}
	}

	RegCloseKey(hKey);
	
	if(bFindFlag)
	{
		strRet.Format("%s",szAppRoot);
	}

	return strRet;
}

int WSA_Init()
{
    //return 0;
	WORD wVersionRequested;  
	WSADATA wsaData; 
	int err; 
	wVersionRequested = MAKEWORD(1, 1); 
    
	err = WSAStartup(wVersionRequested, &wsaData); 
    
	if (err != 0) 
		return -1; 
    
	if ( LOBYTE( wsaData.wVersion ) != 1 || 
		HIBYTE( wsaData.wVersion ) != 1 ) 
	{ 
		WSACleanup(); 
		return -2;
	}
	
	return 0;
}

void WSA_Free()
{
    //return;
	WSACleanup();
}






/////////////////////////////////////////////////////////////////////////////
DWORD DFNParser_GetPrivateProfileString(
  LPCTSTR lpAppName,        // section name
  LPCTSTR lpKeyName,        // key name
  LPCTSTR lpDefault,        // default string
  LPTSTR lpReturnedString,  // destination buffer
  DWORD nSize,              // size of destination buffer
  LPCTSTR lpFileName        // initialization file name
 )
{
	char cKey[255]={0};
	sprintf(cKey,"%s_%s",lpAppName,lpKeyName);
	return  DFN_GetPrivateProfileString(lpFileName,cKey,lpDefault,lpReturnedString,nSize,"oscmd.ini");

}


DWORD DFN_GetPrivateProfileString(
  LPCTSTR lpAppName,        // section name
  LPCTSTR lpKeyName,        // key name
  LPCTSTR lpDefault,        // default string
  LPTSTR lpReturnedString,  // destination buffer
  DWORD nSize,              // size of destination buffer
  LPCTSTR lpFileName        // initialization file name
 )
{
	std::string dfn_Section;
	std::string dfn_Key;
	std::string dfn_Default;
	std::string dfn_File;
	dfn_Section =lpAppName;
	dfn_Key   = lpKeyName;
	dfn_Default= lpDefault;
	dfn_File = lpFileName;

	std::string dfn_Val;

	if(gRoot_path=="")
			gRoot_path =FuncGetInstallPath();
	
	//dfn_Val=GetIniFileString(dfn_Section, dfn_Key,dfn_Default,dfn_File);

	char configpath[1024]={0};

	sprintf(configpath,"%s\\data\\TmpIniFile\\%s",gRoot_path.c_str(),dfn_File.c_str());

	//puts(configpath);
	return GetPrivateProfileString(lpAppName,lpKeyName,lpDefault,lpReturnedString,nSize,configpath);

}

int DFNParser_GetPrivateProfileInt(LPCTSTR lpAppName,  // section name
  LPCTSTR lpKeyName,  // key name
  INT nDefault,       // return value if key name not found
  LPCTSTR lpFileName  // initialization file name
)
{
	char cKey[255]={0};
	sprintf(cKey,"%s_%s",lpAppName,lpKeyName);

	return DFN_GetPrivateProfileInt(lpFileName,cKey,nDefault,"oscmd.ini" );

}

int DFN_GetPrivateProfileInt(
  LPCTSTR lpAppName,  // section name
  LPCTSTR lpKeyName,  // key name
  INT nDefault,       // return value if key name not found
  LPCTSTR lpFileName  // initialization file name
)
{
	int Result;
	char strDefault[30]={0};
	char strGet[30]={0};
	sprintf(strDefault,"%d",nDefault);
	int nSize;
	nSize =29;
	DFN_GetPrivateProfileString(lpAppName,lpKeyName,strDefault,strGet,nSize,lpFileName);
	if(nSize>-1)
	{
		Result=atoi(strGet);
	}
	else Result =nDefault;
	
	return Result;
}


/////////////////////////////////////////////////////////////////////////////
BOOL DFN_WritePrivateProfileString(
  LPCTSTR lpAppName,  // section name
  LPCTSTR lpKeyName,  // key name
  LPCTSTR lpString,   // string to add
  LPCTSTR lpFileName  // initialization file
)
{  
	BOOL Result = TRUE;

	std::string dfn_Section;
	std::string dfn_Key;
	std::string dfn_Default;
	std::string dfn_File;
	dfn_Section =lpAppName;
	dfn_Key   = lpKeyName;
	dfn_File = lpFileName;
	dfn_Default = lpString;
	
	std::string dfn_Val;
	std::string strInt ;
	if(gRoot_path=="")
			gRoot_path =FuncGetInstallPath();
	char szPath[1024]={0};
	sprintf(szPath,"%s\\data\\TmpIniFile\\%s",gRoot_path.c_str(),dfn_File.c_str());
	//puts(szPath);
	WritePrivateProfileString(lpAppName,lpKeyName,lpString,szPath);
    return Result;
}

/////////////////////////////////////////////////////////////////////////////
int FuncGetCountByMark(CString strWholeString, CString strMark)
{
	CString strTemp = _T("");
	strTemp = strWholeString;

	int nIndex1 = strTemp.Find(strMark, 0);
	int i = 0;
	CString strReturn = _T("");

	int nMarkByte = 0;
	nMarkByte = strMark.GetLength();

	while (nIndex1!=-1) 
	{
		i++;
		
		CString strLeft = strTemp.Left(nIndex1);
		CString strRight = strTemp.Right(strTemp.GetLength()-nIndex1-nMarkByte);
		
		nIndex1 = strRight.Find(strMark, 0);
		strTemp = strRight;	
	}

	return i;
}
/////////////////////////////////////////////////////////////////////////////
int FuncGetProfileIntBy(CString strSection, CString strKey, CString strFileName)
{
	char	buffer[512] = {0};
	DWORD nSize = 256;
    DFN_GetPrivateProfileString(strSection, strKey, _T(""), buffer, nSize, strFileName);

	CString strString = _T("");
	strString = buffer;

	int nReturnValue = atoi(strString);
	return nReturnValue;
}
/////////////////////////////////////////////////////////////////////////////
CString FuncGetLeftStringByMark(CString strWholeString, CString strMark, int nIndex)
{
	CString strTemp = _T("");
	strTemp = strWholeString;
	
	int nIndex1 = strTemp.Find(strMark, 0);
	int i = 0;
	CString strReturn = _T("");
	int nMarkByte = 0;
	nMarkByte = strMark.GetLength();

	while (nIndex1!=-1) 
	{
		i++;
		
		CString strLeft = strTemp.Left(nIndex1);
		CString strRight = strTemp.Right(strTemp.GetLength()-nIndex1-nMarkByte);

		if (i == nIndex)
			strReturn = strLeft;
		
		nIndex1 = strRight.Find(strMark, 0);
		strTemp = strRight;	
	}

	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
CString TimeToString(CTime Time)
{
	CString strTime = _T("");
	strTime.Format("%d-%d-%d %d:%d:%d", Time.GetYear(), Time.GetMonth(), 
        Time.GetDay(), Time.GetHour(), Time.GetMinute(), Time.GetSecond());
	return strTime;
}

/////////////////////////////////////////////////////////////////////////////
CString GetValueFromList(const CString strName,CStringList& strList)
{
	POSITION pos=NULL;
	pos=strList.GetHeadPosition();
	int nIndex=0;
	CString strItem=_T("");
	CString strValue=_T("");
	while(pos!=NULL)
	{
		strItem=strList.GetNext(pos);
		nIndex=strItem.Find("=");
		strValue=strItem.Left(nIndex);
		if (strValue==strName)
		{
			strValue=strItem.Right(strItem.GetLength()-nIndex-1);
			return strValue;
		}
	}
	return "";	
}

/////////////////////////////////////////////////////////////////////////////
void CreateListByStringandMark(CString strSource ,CStringList& strDesList, 
                               CString strMark)
{
	int nEmail=FuncGetCountByMark( strSource,strMark);
	for(int i = 1; i <= nEmail; i++)
		strDesList.AddTail( FuncGetLeftStringByMark(strSource,strMark,i));
}
/////////////////////////////////////////////////////////////////////////////
void WriteLog(CString strLog, int nLogType, CString strLogFile)
{

	
	CString strLogIni = _T("");
#if 1
	strLogIni.Format("%s\\groups\\log.ini", FuncGetInstallPath());
#else
	strLogIni.Format("%s\\monitormanager\\groups\\log.ini", FuncGetInstallPath());
#endif
	CString strKey = _T("");
	if (nLogType != 0)
	{
		strKey.Format("IsOpen%d", nLogType);
	}
	else
	{
		strKey.Format("IsOpen");
	}
	int nDebug = GetPrivateProfileInt("Debug", strKey.GetBuffer(strKey.GetLength()), 
									  0, strLogIni.GetBuffer(strLogIni.GetLength()));
	//nDebug=1;
	if (nDebug == 0)
	{
		return;
	}
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString strOut = _T("");
	strOut.Format("%d年%d月%d日 LogType=%d %d:%d:%d %s", st.wYear, st.wMonth, st.wDay, nLogType, st.wHour, st.wMinute, st.wSecond, strLog);
	FILE *f = NULL;
	CString strRealFile = _T("");
	if (strLogFile == _T(""))
	{
		strRealFile.Format("%s\\logs\\log.txt", FuncGetInstallPath());
	}
	else
	{
		if (strLogFile.Find(":") == -1)
		{
#if _DEBUG			
			strRealFile.Format("%s\\logs\\%s", FuncGetInstallPath(), strLogFile);
#else
			strRealFile.Format("%s\\monitormanager\\logs\\%s", FuncGetInstallPath(), strLogFile);
#endif
		}
		else
		{
			strRealFile = strLogFile;
		}
	}
	f = fopen(strRealFile, "a+");
    if (f!=NULL)
    {
        fprintf(f, strOut);
	    fprintf(f, "\r\n");
        fclose(f);
    }
	
}
/////////////////////////////////////////////////////////////////////////////
CString FuncGetProfileStringBy(CString strSection, CString strKey, CString strFileName)
{
	char	buffer[2048] = {0};
	DWORD nSize = 2048;
    DFN_GetPrivateProfileString(strSection, strKey, _T(""), buffer, nSize, strFileName);

	CString strString = _T("");
	strString = buffer;

	return strString;
}

void FuncGetCompanyName(CString& strRet)
{
	CString strPath = _T("");
	strPath = ::FuncGetInstallPath();

	CString strResourceFile = _T("");
	strResourceFile.Format("%s\\groups\\resource.config", (LPCTSTR)strPath);
	
	char buffer[1024] = {0};	
	GetPrivateProfileString("Resource", "CompanyName", "", buffer, sizeof(buffer) - 1, (LPCTSTR)strResourceFile);

	strRet = buffer;
}

void FuncGetCompanyUrl(CString& strRet)
{
	CString strPath = _T("");
	strPath = ::FuncGetInstallPath();

	CString strResourceFile = _T("");
	strResourceFile.Format("%s\\groups\\resource.config", (LPCTSTR)strPath);
	
	char buffer[1024] = {0};	
	GetPrivateProfileString("Resource", "CompanyUrl", "", buffer, sizeof(buffer) - 1, (LPCTSTR)strResourceFile);

	strRet = buffer;
}


//排序接口函数(bAsc=TRUE升序)
void SortList(CStringList &KeyList, CStringList &ValList, BOOL bAsc)
{
	int Count = KeyList.GetCount();

	_rec *r = new _rec[Count*sizeof(_rec)];

	POSITION posKey = KeyList.FindIndex(0);
	POSITION posVal = ValList.FindIndex(0);

	int i=0;
	while(posKey && posVal)
	{
		CString Key = KeyList.GetNext(posKey);
		CString Val = ValList.GetNext(posVal);
		r[i].key = Key;
		r[i].val = Val;		
		i++;
	}

	//排序函数
	_Sort(r,Count,bAsc);

	KeyList.RemoveAll();
	ValList.RemoveAll();

	for(int j=0; j<Count; j++)
	{
		CString s1 = r[j].key;
		CString s2 = r[j].val;

		KeyList.AddTail(r[j].key);
		ValList.AddTail(r[j].val);
	}

	delete[] r;
}

//排序函数(冒泡排序算法 )
void _Sort(struct _rec r[],int n, BOOL bAsc)
{
	int i,j;
	struct _rec w;
	int compare=0,move=0;
	
	//升序
	if(bAsc)
	{
		for(i=0;i<=n;i++)
		{
			for(j=n-1;j>=i+1;j--)
			{  
				int n = (r[j].key).Compare(r[j-1].key);
				if(n<0)
				{
					w=r[j];
					r[j]=r[j-1];
					r[j-1]=w;
					move=move+3;
				}

			}
			compare++;
		  }
	}
	//降序
	else
	{
		for(i=0;i<=n;i++)
		{
			for(j=n-1;j>=i+1;j--)
			{  
				int n = (r[j].key).Compare(r[j-1].key);
				if(n>0)
				{
					w=r[j];
					r[j]=r[j-1];
					r[j-1]=w;
					move=move+3;
				}

			}
			compare++;
		  }
	}

}
CString FuncGetRightString(CString strWholeString, CString strMark)
{
	CString strTemp = _T("");
	strTemp = strWholeString;
	int nMarkByte = 0;
	nMarkByte = strMark.GetLength();

	int nIndex1 = strTemp.Find(strMark, 0);
	CString strReturn = _T("");

	if(nIndex1 < 0)
		return "";

	while (nIndex1!=-1) 
	{
		CString strLeft = strTemp.Left(nIndex1);
		CString strRight = strTemp.Right(strTemp.GetLength()-nIndex1-nMarkByte);

		nIndex1 = strRight.Find(strMark, 0);
		strTemp = strRight;	
	}

	return strTemp;
}
bool MakeStringListByChar(CStringList& pList, const char * pInput )
{
	const char * p;
	int nSize;
	p=pInput;
	while(*p!='\0')
	{
		nSize = static_cast<int>(strlen(p));
		if( nSize>0 )
		{	
			pList.AddHead(p);
		//	printf(p);
		}
		p=p+nSize+1;
	}

	return true;
}
bool MakeCharByString(char *pOut,int &nOutSize,CString strInput )
{
	 char *p;
	
	int nSize=strInput.GetLength();
	if(nSize+2 <nOutSize)
	{
		strcpy(pOut,strInput.GetBuffer(strInput.GetLength()));
	}else return false;
	p=pOut;
	//printf("%d\n",nSize);23028830 13602067678 王波
	for(int i=0;i<nSize;i++)
	{
		if(*p=='$') 	*p='\0';
		p++;
	}
	nOutSize=nSize+1;
	return true;
	
}
string GetSEIP()
{
	CString strRootPath;
	strRootPath=FuncGetInstallRootSCPath();
	char szFile[1024]={0};
	sprintf(szFile,"%s\\fcgi-bin\\mc.config",strRootPath);
	puts(szFile);
	int nSeId=1;
	nSeId =GetPrivateProfileInt("MonitorSchedule","SEID",1,szFile);
	if(nSeId==1)	return "localhost";
	char szSeHost[1024]={0};
	
	GetPrivateProfileString("MonitorSchedule","ServerHost","localhost",szSeHost,1023,szFile);
	return szSeHost;
	
	
}

string GetResourceValue(const string rkey)
{
	string rvalue="";
	//OBJECT obj=LoadResource("default", "localhost");  
	OBJECT obj=LoadResource("default", GetSEIP());  
	if( obj==INVALID_VALUE )
		return rvalue;

	MAPNODE ma=GetResourceNode(obj);
	if( ma==INVALID_VALUE )
	{
		CloseResource(obj);
		return rvalue;
	}

	FindNodeValue(ma,rkey,rvalue);
	CloseResource(obj);
	return rvalue;
}
