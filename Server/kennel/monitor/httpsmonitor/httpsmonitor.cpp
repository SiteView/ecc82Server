// HttpsMonitor.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "HttpsMonitor.h"
#include "W3c.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include ".\base\funcGeneral.h"

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CHttpsMonitorApp

BEGIN_MESSAGE_MAP(CHttpsMonitorApp, CWinApp)
	//{{AFX_MSG_MAP(CHttpsMonitorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHttpsMonitorApp construction

CHttpsMonitorApp::CHttpsMonitorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CHttpsMonitorApp object

CHttpsMonitorApp theApp;

CString QueryRawHeaderStatus(CString strRawHeader);
BSTR UTF2Unicode(LPCSTR lp, int nLen);
void MatchContent(CString strContent, CString strFilePath, char *szReturn);


extern "C" __declspec(dllexport) BOOL HttpsMonitor(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	BOOL bRet = FALSE;

	CString strPath = ::FuncGetInstallRootPath();
	if (strPath.IsEmpty())//得到Siteview安装路径失败
		return bRet;

	CString	strURL = _T(""),				// URL
			strPostData = _T(""),			// Post Data
			strUsername = _T(""),			// Authorization User Name
			strPassword = _T(""),			// Authorization Password
			strProxyServer = _T(""),		// HTTP Proxy
			strProxyUsername = _T(""),		// Proxy Server User Name
			strProxyPassword = _T(""),		// Proxy Server Password
			strMatchContent = _T("");		// Match Content

	CString strProxyPort = _T("");
	CString strCustomer = _T("");

	DWORD dwError=0;
	CString strError=_T("");

	DWORD dwStart = ::GetTickCount();

	CString strGID;
	CString strMid;
	//int		nTimeout = 60, 
	//		nGID = 0, 
	//		nMID = 0;
	int		nTimeout = 60;
	CStringList paramList;

	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);//得到StringList中的第一个元素
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__URL__, 0) == 0)
		{//URL地址
			strURL = strTemp.Right(strTemp.GetLength() - strlen(__URL__));
		}
		else if(strTemp.Find(__POSTDATA__, 0) == 0)
		{//Post Data
			strPostData = strTemp.Right(strTemp.GetLength() - strlen(__POSTDATA__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{//用户名
			strUsername = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{//密码
			strPassword = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{//代理服务器和端口
			strTemp.Delete(0, strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{//如果不使用缺省的端口
				strProxyServer = strTemp.Left(index);//代理服务器
				strTemp.Delete(0, index + 1);
				strProxyPort = strTemp;//代理服务器端口				
			}
			else
			{//代理服务器
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{//代理服务器访问用户
			strProxyUsername = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{//代理服务器访问密码
			strProxyPassword = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{//超时时间
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{//匹配字符串
			strMatchContent = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__GROUPID__, 0) == 0)
		{//Group ID

			//nGID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__GROUPID__)));
			strGID=strTemp.Right(strTemp.GetLength() - strlen(__GROUPID__));
		}
		else if(strTemp.Find(__MONITORID__, 0) == 0)
		{//Monitor ID
			//nMID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__)));
			strMid=strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__));
		}
		else if(strTemp.Find(__CUSTOMER__, 0) == 0)
		{//IDC用户
			strCustomer = strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMER__));
		}
	}

	if(strURL.IsEmpty()) 
	{//请求页面为空
		sprintf(szReturn, "error=%s",FuncGetStringFromIDS("<%IDS_UrlMonitor_1%>"));	//<%IDS_UrlMonitor_1%>"error=缺少URL"
		return bRet;
	}

	CString strTempFile = _T("");//页面内容保存在本地的文件路径
	if (strCustomer.IsEmpty())
	{//如果不是IDC用户

		//strTempFile.Format("%s\\MonitorManager\\temp\\https-%d-%d.html", (LPCTSTR)strPath, 
          //  nGID, nMID);
		strTempFile.Format("%s\\temp\\https-%s.html", (LPCTSTR)strPath, 
             strMid);
		puts(strTempFile);

	}
	else
	{//IDC用户
		strTempFile.Format("%s\\%s\\temp\\https-%s-%s.html", (LPCTSTR)strPath, strCustomer, strGID, strMid);
	}
	W3Client *_pclient = new W3Client();//创建新的客户端
	
	WriteLog(strTempFile,300,"wp.ini");

	if(_pclient->Connect((const char*)strURL.GetBuffer(0)))
	{//连接成功		
		if(_pclient->Request(_pclient->GetURI()))
		{//请求页面成功
			FILE * fp = NULL;
			//创建文件，如果原文件已经存在则销毁
			fp = fopen(strTempFile, "w");
			if (fp) 
			{//创建文件成功
				unsigned char buf[1024]="\0";
				//读取返回头
				_pclient->QueryRawHeader(buf, 1024);
				//得到返回码
				CString strStatusCode = QueryRawHeaderStatus(buf);
				memset(buf, 0x00, 1024);//重新设置
				unsigned long ulRead = 1,//读取的长度
					nPageSize =0;//页面的大小(单位为byte)
				while(ulRead)
				{
					ulRead = _pclient->Response(buf, 1024);//读取返回内容
					nPageSize += ulRead;//下载大小累加
					UTF2Unicode((const char*)buf,ulRead);
					fwrite(buf,ulRead,sizeof(char),fp);//写数据到文件中
					memset(buf, 0x00, 1024);//重新设置
				}	
				fclose(fp);//关闭文件
				//得到当前时间
				DWORD dwEnd = ::GetTickCount();
				sprintf(szReturn,"status=%s$totaltime=%.2f$downloadsize=%d$", 
					strStatusCode, (dwEnd - dwStart)*1.0/1000, nPageSize); 

				if(!strMatchContent.IsEmpty())
				{//如果匹配字符串不为空
					MatchContent(strMatchContent, strTempFile, szReturn);
				}
				CString strResult= szReturn;
				MakeCharByString(szReturn,nSize,strResult);


			}
			else
			{//创建文件失败
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("IDS_OpenFileFailed"));//打开文件失败");
				return FALSE;
			}
		}
		else
		{//打开页面失败	
			sprintf(szReturn, "error=%s", _pclient->GetErrorMsg().c_str());
			//sprintf(szReturn, "error=打开页面失败$");
			return FALSE;
		}
	}
	else
	{//连接失败
		//sprintf(szReturn, "error=%s$", _pclient->GetErrorMsg().c_str());
		sprintf(szReturn, "error=%s",FuncGetStringFromIDS("<%IDS_UrlMonitor_3%>"));//sprintf(szReturn, "error=连接失败");

		return FALSE;
	}
	return TRUE;
}
CString QueryRawHeaderStatus(CString strRawHeader)
{
	CString strStatus = _T("");
	strStatus = strRawHeader.Left(strRawHeader.Find("\r\n"));
	strStatus = strStatus.Right(strStatus.GetLength() - 
		strStatus.Find(" ") - 1);
	strStatus = strStatus.Left(strStatus.Find(" "));
	return strStatus;
}

BSTR UTF2Unicode(LPCSTR lp, int nLen)
{
   BSTR str = NULL;
   int nConvertedLen = MultiByteToWideChar(CP_UTF8, 0, lp,
     nLen, NULL, NULL);
 
   // BUG FIX #1 (from Q241857): only subtract 1 from 
   // the length if the source data is nul-terminated
   if (nLen == -1)
      nConvertedLen--;
 
   str = ::SysAllocStringLen(NULL, nConvertedLen);
   if (str != NULL)
   {
     MultiByteToWideChar(CP_UTF8, 0, lp, nLen, str, nConvertedLen);
   }
   return str;
}

void MatchContent(CString strContent, CString strFilePath, char *szReturn)
{
	CFile fileTmp;
	CFileException fileError;
	if(!fileTmp.Open(strFilePath, CFile::modeRead , &fileError))
	{
		sprintf(szReturn, "error=%s",FuncGetStringFromIDS("IDS_OpenFileFailed"));//打开文件失败");
		return ;
	}
	DWORD dwLength = fileTmp.GetLength();
    if(dwLength == 0)
    {
		//strcpy(szReturn, "error=打开文件失败");
		return ;
    }
	char *chContent;
	chContent = (char*)malloc(dwLength);
	CString strRead;
	fileTmp.Read(chContent, dwLength);
	fileTmp.Close();
	if (!strstr(chContent, strContent.GetBuffer(strContent.GetLength())))
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_UrlMonitor_2%>"));//<%IDS_UrlMonitor_2%>"error=匹配失败"
	delete chContent;
}
