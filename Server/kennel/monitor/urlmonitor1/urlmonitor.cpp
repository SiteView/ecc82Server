// UrlMonitor.cpp : Defines the initialization routines for the DLL.
//


#include "stdafx.h"
#include "UrlMonitor.h"
//#include "Wininet.h"

#include "../../base/base64.h"
#include "io.h"
#include "stdlib.h"
#include "stdio.h"
#include "../../base/SVHttp.h"

#include <atlconv.h>

#include <afxinet.h>



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CUrlMonitorApp

BEGIN_MESSAGE_MAP(CUrlMonitorApp, CWinApp)
	//{{AFX_MSG_MAP(CUrlMonitorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUrlMonitorApp construction

CUrlMonitorApp::CUrlMonitorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CUrlMonitorApp object

CUrlMonitorApp theApp;


void WriteLogFile(char* chMsg);

CString GetErrorMessage(DWORD dwError)
{
	CString strError=_T("");

	switch(dwError)
	{
	case    ERROR_WINHTTP_AUTO_PROXY_SERVICE_ERROR:
		return "ERROR_WINHTTP_AUTO_PROXY_SERVICE_ERROR";
	case	ERROR_WINHTTP_AUTODETECTION_FAILED:
		return "ERROR_WINHTTP_AUTODETECTION_FAILED";
	case	ERROR_WINHTTP_BAD_AUTO_PROXY_SCRIPT:
		return "ERROR_WINHTTP_BAD_AUTO_PROXY_SCRIPT";
	case	ERROR_WINHTTP_CANNOT_CALL_AFTER_OPEN:				
		return "ERROR_WINHTTP_CANNOT_CALL_AFTER_OPEN";
	case	ERROR_WINHTTP_CANNOT_CALL_AFTER_SEND:				
		return "ERROR_WINHTTP_CANNOT_CALL_AFTER_SEND";
	case	ERROR_WINHTTP_CANNOT_CALL_BEFORE_OPEN:				
		return "ERROR_WINHTTP_CANNOT_CALL_BEFORE_OPEN";
	case	ERROR_WINHTTP_CANNOT_CALL_BEFORE_SEND:				
		return "ERROR_WINHTTP_CANNOT_CALL_BEFORE_SEND";
	case	ERROR_WINHTTP_CANNOT_CONNECT:					
		return "ERROR_WINHTTP_CANNOT_CONNECT";
	case	ERROR_WINHTTP_CHUNKED_ENCODING_HEADER_SIZE_OVERFLOW:
		return "ERROR_WINHTTP_CHUNKED_ENCODING_HEADER_SIZE_OVERFLOW";
	case	ERROR_WINHTTP_CLIENT_AUTH_CERT_NEEDED:	
		return "ERROR_WINHTTP_CLIENT_AUTH_CERT_NEEDED";
	case	ERROR_WINHTTP_CONNECTION_ERROR:					
		return "ERROR_WINHTTP_CONNECTION_ERROR";
	case	ERROR_WINHTTP_HEADER_ALREADY_EXISTS:
		return "ERROR_WINHTTP_HEADER_ALREADY_EXISTS";
	case	ERROR_WINHTTP_HEADER_COUNT_EXCEEDED:					
		return "ERROR_WINHTTP_HEADER_COUNT_EXCEEDED";
	case	ERROR_WINHTTP_HEADER_NOT_FOUND:					
		return "ERROR_WINHTTP_HEADER_NOT_FOUND";
	case	ERROR_WINHTTP_HEADER_SIZE_OVERFLOW:
		return "ERROR_WINHTTP_HEADER_SIZE_OVERFLOW";
	case	ERROR_WINHTTP_INCORRECT_HANDLE_STATE:
		return "ERROR_WINHTTP_INCORRECT_HANDLE_STATE";
	case	ERROR_WINHTTP_INCORRECT_HANDLE_TYPE:					
		return "ERROR_WINHTTP_INCORRECT_HANDLE_TYPE";
	case	ERROR_WINHTTP_INTERNAL_ERROR:					
		return "ERROR_WINHTTP_INTERNAL_ERROR";
	case	ERROR_WINHTTP_INVALID_OPTION:
		return "ERROR_WINHTTP_INVALID_OPTION";
	case	ERROR_WINHTTP_INVALID_QUERY_REQUEST:
		return "ERROR_WINHTTP_INVALID_QUERY_REQUEST";
	case	ERROR_WINHTTP_INVALID_SERVER_RESPONSE:
		return "ERROR_WINHTTP_INVALID_SERVER_RESPONSE";
	case	ERROR_WINHTTP_INVALID_URL:					
		return "ERROR_WINHTTP_INVALID_URL";
	case	ERROR_WINHTTP_LOGIN_FAILURE:							
		return "ERROR_WINHTTP_LOGIN_FAILURE";
	case	ERROR_WINHTTP_NAME_NOT_RESOLVED:
		return "ERROR_WINHTTP_NAME_NOT_RESOLVED";
	case	ERROR_WINHTTP_NOT_INITIALIZED:
		return "ERROR_WINHTTP_NOT_INITIALIZED";
	case	ERROR_WINHTTP_OPERATION_CANCELLED:
		return "ERROR_WINHTTP_OPERATION_CANCELLED";
	case	ERROR_WINHTTP_OPTION_NOT_SETTABLE:
		return "ERROR_WINHTTP_OPTION_NOT_SETTABLE";
	case	ERROR_WINHTTP_OUT_OF_HANDLES:						
		return "ERROR_WINHTTP_OUT_OF_HANDLES";
	case	ERROR_WINHTTP_REDIRECT_FAILED:
		return "ERROR_WINHTTP_REDIRECT_FAILED";
	case	ERROR_WINHTTP_RESEND_REQUEST:
		return "ERROR_WINHTTP_RESEND_REQUEST";
	case	ERROR_WINHTTP_RESPONSE_DRAIN_OVERFLOW:
		return "ERROR_WINHTTP_RESPONSE_DRAIN_OVERFLOW";
	case	ERROR_WINHTTP_SECURE_CERT_CN_INVALID:				
		return "ERROR_WINHTTP_SECURE_CERT_CN_INVALID";
	case	ERROR_WINHTTP_SECURE_CERT_DATE_INVALID:				
		return "ERROR_WINHTTP_SECURE_CERT_DATE_INVALID";
	case	ERROR_WINHTTP_SECURE_CERT_REV_FAILED:				
		return "ERROR_WINHTTP_SECURE_CERT_REV_FAILED";
	case	ERROR_WINHTTP_SECURE_CERT_REVOKED:					
		return "ERROR_WINHTTP_SECURE_CERT_REVOKED";
	case	ERROR_WINHTTP_SECURE_CERT_WRONG_USAGE:				
		return "ERROR_WINHTTP_SECURE_CERT_WRONG_USAGE";
	case	ERROR_WINHTTP_SECURE_CHANNEL_ERROR:					
		return "ERROR_WINHTTP_SECURE_CHANNEL_ERROR";
	case	ERROR_WINHTTP_SECURE_FAILURE:						
		return "ERROR_WINHTTP_SECURE_FAILURE";
	case	ERROR_WINHTTP_SECURE_INVALID_CA:						
		return "ERROR_WINHTTP_SECURE_INVALID_CA";
	case	ERROR_WINHTTP_SECURE_INVALID_CERT:					
		return "ERROR_WINHTTP_SECURE_INVALID_CERT";
	case	ERROR_WINHTTP_SHUTDOWN:						
		return "ERROR_WINHTTP_SHUTDOWN";
	case	ERROR_WINHTTP_TIMEOUT:								
		return "ERROR_WINHTTP_TIMEOUT";
	case	ERROR_WINHTTP_UNABLE_TO_DOWNLOAD_SCRIPT:
		return "ERROR_WINHTTP_UNABLE_TO_DOWNLOAD_SCRIPT";
	case	ERROR_WINHTTP_UNRECOGNIZED_SCHEME:				
		return "ERROR_WINHTTP_UNRECOGNIZED_SCHEME";
	default:
		return "ERROR_OTHERERROR";
	}
	return strError;
	
}

BSTR UTF2GB(LPCSTR lp, int nLen)
{
   BSTR str = NULL;
   int nConvertedLen = MultiByteToWideChar(CP_UTF8, 0, lp,
     nLen, NULL, NULL);
 
   // BUG FIX #1 (from Q241857): only subtract 1 from 
   // the length if the source data is nul-terminated
   if (nLen == -1)
      nConvertedLen--;
 
   str = SysAllocStringLen(NULL, nConvertedLen);
   if(str!=NULL)
   {
	   MultiByteToWideChar(CP_UTF8, 0, lp, nLen, str, nConvertedLen);
	   return str;
   }
   return NULL;
}

extern "C" __declspec(dllexport) BOOL INET(const char * strParas, char * szReturn, int& nSize)
//(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	USES_CONVERSION;

	BOOL bRet = FALSE;

	//参数解析：
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

	CStringList paramList;

	MakeStringListByChar(paramList,strParas);


//	DWORD dwStart = ::GetTickCount();

	int		nTimeout = 60, 
			nGID = 0, 
			nMID = 0;

    POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__URL__, 0) == 0)
		{
			strURL = strTemp.Right(strTemp.GetLength() - strlen(__URL__));
		}
		else if(strTemp.Find(__POSTDATA__, 0) == 0)
		{
			strPostData = strTemp.Right(strTemp.GetLength() - strlen(__POSTDATA__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUsername = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPassword = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp.Delete(0, strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				strProxyPort = strTemp;				
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUsername = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPassword = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strMatchContent = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__GROUPID__, 0) == 0)
		{
			nGID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__GROUPID__)));
		}
		else if(strTemp.Find(__MONITORID__, 0) == 0)
		{
			nMID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__)));
		}
		else if(strTemp.Find(__CUSTOMER__, 0) == 0)
		{
			strCustomer = strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMER__));
		}
	}

	//printf("timeout =%d\r\n",nTimeout);

	if(strURL.IsEmpty()) 
	{
		//<%IDS_UrlMonitor_1%>"error=缺少URL$"
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_URL",
            "URL_LOST_ADDRESS"));	
		return bRet;
	}



	//cxy 2005.01.25 多线程安全的URLMONITOR的实现
	//尚未完成的功能：........
	//1、重定向。
	//2、设置cookie。
	//3、tiemout(原来就没有意义的)。

	//监测的实现：
	SVHttp svHttp;
	svHttp.Init();
	svHttp.SetHttpHeader("Content-Type", "application/x-www-form-urlencoded");
	//svHttp.ClearHeader
//    WriteLogFile(strUsername.GetBuffer(strUsername.GetLength()));
//    WriteLogFile(strPassword.GetBuffer(strPassword.GetLength()));
    if(!strUsername.IsEmpty())
    {
	    svHttp.SetHttpUser(strUsername);
    }
    if(!strPassword.IsEmpty())
    {
	    svHttp.SetHttpPwd(strPassword);
    }
    if(!strProxyServer.IsEmpty())
	    svHttp.SetProxyIp(strProxyServer);
    if(!strProxyPort.IsEmpty())
	    svHttp.SetProxyPort(strProxyPort);
    if(!strProxyUsername.IsEmpty())
	    svHttp.SetProxyUser(strProxyUsername);
    if(!strProxyPassword.IsEmpty())
	    svHttp.SetProxyPwd(strProxyPassword);
	svHttp.SetUrl(strURL);
	svHttp.SetPostData(strPostData);	
	svHttp.SetTimeOut(nTimeout);
	
	if(svHttp.SendRequest())
//	if(svHttp.SendAllRequest())
	{
		CString str = _T("");
		str.Format("status=%d$totaltime=%.2f$downloadsize=%d$", svHttp.GetStatusCode(), svHttp.GetResponseTime(), svHttp.GetPageSize());		
		
		CString strReturn=svHttp.GetResponseText();
		//printf("Reutrn result is :%s\n",strReturn);
		//匹配字符串 :
		if (strMatchContent.IsEmpty()) 
		{
//			wprintf(L"len is :%d,Reutrn result is wide char :%s len is \n",strReturn.GetLength(),strReturn.AllocSysString());
			strcpy(szReturn, (LPCTSTR)str);
		}
		else
		{

		//	if(svHttp.GetResponseText().Find(strMatchContent) != -1)
			if(strReturn.Find(strMatchContent) != -1)
			{
				strcpy(szReturn, (LPCTSTR)str);
			}
			else
			{
				BSTR uc=UTF2GB(strReturn.GetBuffer(strReturn.GetLength()),strReturn.GetLength());
				char *ptem=W2A(uc);

				char *p=strstr(ptem,strMatchContent.GetBuffer(strMatchContent.GetLength()));
				if(p)
				{
					strcpy(szReturn, (LPCTSTR)str);
					CString strInput;
					strInput =szReturn;
					MakeCharByString(szReturn,nSize,strInput);
					return TRUE;
				}

				//<%IDS_UrlMonitor_2%>"error=匹配失败$"
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_URL","URL_NOT_MATCH"));
				return FALSE;
			}
		}		
		
	}
	else
	{
		sprintf(szReturn,"error=%s", svHttp.GetErrorMessage());
		return FALSE;
	}	
	CString strInput;
	strInput =szReturn;
	MakeCharByString(szReturn,nSize,strInput);

	return TRUE;
}

extern "C" __declspec(dllexport) BOOL APACHE(const char * strParas, char * szReturn, int& nSize)
//(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strURL = _T(""), 
				strUser = _T(""), 
				strPass = _T("");
	int			nTimeout = 60;

	CString		strProxyServer = _T(""), 
				strProxyPort = _T(""), 
				strProxyUser = _T(""), 
				strProxyPass = _T("");
	int			nProxyPort = 8080;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());

	CStringList paramList;

	MakeStringListByChar(paramList,strParas);
	// NT Chalenge

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERMANURL__, 0) == 0)
		{
			strURL = strTemp.Right(strTemp.GetLength() - strlen(__SERVERMANURL__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUser = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPass = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strURL.IsEmpty())
	{
		// Input URL Empty
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_URL",
            "URL_LOST_ADDRESS"));//<%IDS_Monitor_1%>"缺少URL"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		// Input Timeout Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_URL",
            "URL_TIMEOUT_VALUE_ERROR"));//<%IDS_Monitor_2%>"输入超时错误"
		return FALSE;
	}

	strProxyPort.Format("%d", nProxyPort);
	if(!strProxyServer.IsEmpty() && nProxyPort <= 0)
	{
		// Input Proxy Port Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_URL",
            "URL_PROXY_PORT_ERROR"));//<%IDS_Monitor_3%>"代理服务器端口错误"
		return FALSE;
	}

	//cxy 2005.01.25 多线程安全的URLMONITOR的实现
	//尚未完成的功能：........
	//1、重定向。
	//2、设置cookie。
	//3、tiemout(原来就没有意义的)。

	//监测的实现：
	SVHttp svHttp;
	svHttp.Init();
	svHttp.SetHttpHeader("Content-Type", "application/x-www-form-urlencoded");
	//svHttp.ClearHeader
	svHttp.SetHttpUser(strUser);
	svHttp.SetHttpPwd(strPass);
	svHttp.SetProxyIp(strProxyServer);
	svHttp.SetProxyPort(strProxyPort);
	svHttp.SetProxyUser(strProxyUser);
	svHttp.SetProxyPwd(strProxyPass);
	svHttp.SetUrl(strURL);
	//svHttp.SetPostData(strPostData);	
	
	if(svHttp.SendRequest())
	{
		float	cpuLoad = 0.0, requestsPerSec = 0.0, bytesPerSec = 0.0;
		int		busyServers = 0, idleServers = 0;
		
		CString strContent = svHttp.GetResponseText();
		char *ca = NULL;
		char tmp[256] = {0};
		ca = strstr(strContent, "CPULoad:");
		if(ca)
		{			
			sscanf(ca, "%s %f", tmp, &cpuLoad); 			
		}
		else
		{
			cpuLoad = 0;
			//sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Apache_1%>"));//<%IDS_Apache_1%>
			//return FALSE;			
		}

		ca = strstr(strContent, "ReqPerSec:");
		if(ca) //ReqPerSec
			sscanf(ca, "%s %f", tmp, &requestsPerSec); 
		else
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_URL",
            "URL_DATA_MISTAKE"));//<%IDS_Apache_2%>
			return FALSE;
		}

		ca = strstr(strContent, "BytesPerSec:");
		if(ca)
			sscanf(ca, "%s %f", tmp, &bytesPerSec); 
		else
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_URL",
            "URL_DATA_MISTAKE"));//<%IDS_Apache_3%>
			return FALSE;
		}

		ca = strstr(strContent, "BusyServers:");
		if(ca)
			sscanf(ca, "%s %d", tmp, &busyServers); 
		else
		{
			ca =strstr(strContent, "BusyWorkers:");
			if(ca)
			{
				sscanf(ca, "%s %d", tmp, &busyServers); 
			//	printf("%d-  %s  \n",busyServers, ca);
			}else{

			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_URL",
            "URL_DATA_MISTAKE"));//<%IDS_Apache_4%>
			return FALSE;
			}
		}

		ca = strstr(strContent, "IdleServers:");
		if(ca)
			sscanf(ca, "%s %d", tmp, &idleServers); 
		else
		{
			ca =strstr(strContent, "IdleWorkers:");
			if(ca)
			{
				sscanf(ca, "%s %d", tmp, &idleServers); 
			}else{

				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_URL",
				"URL_DATA_MISTAKE"));//<%IDS_Apache_5%>
				return FALSE;
			}
		}

		sprintf(szReturn, "cpuLoad=%f$reqPerSec=%f$bytesPerSec=%f$busyServers=%d$idleServers=%d$", 
				cpuLoad, requestsPerSec, bytesPerSec, busyServers, idleServers);	
		//printf("%s\n",szReturn);
		CString strInput;
		strInput =szReturn;
		MakeCharByString(szReturn,nSize,strInput);

	}
	else
	{
		sprintf(szReturn,"error=%s", svHttp.GetErrorMessage());
		return FALSE;
	}	

	return TRUE;
}

extern "C" __declspec(dllexport) BOOL APACHE_2_0_34(const char * strParas, char * szReturn, int& nSize)
//(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strURL = _T(""), 
				strUser = _T(""), 
				strPass = _T("");
	int			nTimeout = 60;

	CString		strProxyServer = _T(""), 
				strProxyPort = _T(""), 
				strProxyUser = _T(""), 
				strProxyPass = _T("");
	int			nProxyPort = 8080;

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());
	CStringList paramList;

	MakeStringListByChar(paramList,strParas);

	// NT Chalenge

	POSITION pos = paramList.FindIndex(0);
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERMANURL__, 0) == 0)
		{
			strURL = strTemp.Right(strTemp.GetLength() - strlen(__SERVERMANURL__));
		}
		else if(strTemp.Find(__USERACCOUNT__, 0) == 0)
		{
			strUser = strTemp.Right(strTemp.GetLength() - strlen(__USERACCOUNT__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strPass = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nProxyPort = atoi(strTemp);
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUser = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPass = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strURL.IsEmpty())
	{
		// Input URL Empty
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_URL",
            "URL_LOST_ADDRESS"));//<%IDS_Monitor_1%>"缺少URL"
		return FALSE;
	}

	if(nTimeout <= 0)
	{
		// Input Timeout Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_URL",
            "URL_TIMEOUT_VALUE_ERROR"));//<%IDS_Monitor_2%>"输入超时错误"
		return FALSE;
	}

	strProxyPort.Format("%d", nProxyPort);
	if(!strProxyServer.IsEmpty() && nProxyPort <= 0)
	{
		// Input Proxy Port Error
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_URL",
            "URL_PROXY_PORT_ERROR"));//<%IDS_Monitor_3%>"代理服务器端口错误"
		return FALSE;
	}

	//cxy 2005.01.25 多线程安全的URLMONITOR的实现
	//尚未完成的功能：........
	//1、重定向。
	//2、设置cookie。
	//3、tiemout(原来就没有意义的)。

	//监测的实现：
	SVHttp svHttp;
	svHttp.Init();
	svHttp.SetHttpHeader("Content-Type", "application/x-www-form-urlencoded");
	//svHttp.ClearHeader
    //WriteLogFile(strUser.GetBuffer(strUser.GetLength()));
    //WriteLogFile(strPass.GetBuffer(strPass.GetLength()));
    if(!strUser.IsEmpty())
    {
	    svHttp.SetHttpUser(strUser);
    }
    if(!strPass.IsEmpty())
    {
	    svHttp.SetHttpPwd(strPass);
    }
    if(!strProxyServer.IsEmpty())
	    svHttp.SetProxyIp(strProxyServer);
    if(!strProxyPort.IsEmpty())
	    svHttp.SetProxyPort(strProxyPort);
    if(!strProxyUser.IsEmpty())
	    svHttp.SetProxyUser(strProxyUser);
    if(!strProxyPass.IsEmpty())
	    svHttp.SetProxyPwd(strProxyPass);
	svHttp.SetUrl(strURL);
	//svHttp.SetPostData(strPostData);	

	
	if(svHttp.SendRequest())
	{
		long	totalAccesses = 0, totalKBytes = 0;
		float	reqPerSec = 0.0, bytesPerSec = 0.0, bytesPerReq;
		int		busyWorkers = 0, idleWorkers = 0;		
		CString strContent = svHttp.GetResponseText();
		char *ca = NULL;

		// Total Accesses: 
		if(ca = strstr(strContent, "Total Accesses: "))
			totalAccesses = atol(ca + 16); 
		else
			goto Err;

		// Total kBytes
		if(ca = strstr(strContent, "Total kBytes: "))
			totalKBytes = atol(ca + 14); 
		else
			goto Err;

		// ReqPerSec
		if(ca = strstr(strContent, "ReqPerSec: "))
			reqPerSec = (float)atof(ca + 11); 
		else
			goto Err;

		// BytesPerSec
		if(ca = strstr(strContent, "BytesPerSec: "))
			bytesPerSec = (float)atof(ca + 13); 
		else
			goto Err;

		// BytesPerReq
		if(ca = strstr(strContent, "BytesPerReq: "))
			bytesPerReq = (float)atof(ca + 13); 
		else
			goto Err;

		// Busy Workers
		if(ca = strstr(strContent, "BusyWorkers: "))
			busyWorkers = atoi(ca + 13); 
		else
			goto Err;

		// Idle Workers
		if(ca = strstr(strContent, "IdleWorkers: "))
			idleWorkers = atoi(ca + 13); 
		else
			goto Err;

		sprintf(szReturn, "totalAccesses=%ld$totalKBytes=%ld$reqPerSec=%f$bytesPerSec=%f$bytesPerReq=%f$busyWorkers=%d$idleWorkers=%d$", 
				totalAccesses, totalKBytes, reqPerSec, bytesPerSec, bytesPerReq, 
				busyWorkers, idleWorkers);	
		CString strInput;
		strInput =szReturn;
		MakeCharByString(szReturn,nSize,strInput);

	}
	else
	{
		sprintf(szReturn,"error=%s", svHttp.GetErrorMessage());
		return FALSE;
	}

	return TRUE;

Err:
	sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_URL",
            "URL_DATA_MISTAKE"));//<%IDS_Apache_2%>
	return FALSE;
}


void WriteLogFile(char* chMsg)
{
    FILE *stream;
    char chTime[50] = {0};
    //打开文件
#if _DEBUG
    stream=fopen(FuncGetInstallRootPath() + "\\Temp\\URLMonitor.log","a");
#else
    stream=fopen(FuncGetInstallRootPath() + "\\Monitormanager\\Temp\\URLMonitor.log","a");
#endif
    if(stream == NULL)
        return;//打开文件失败，直接返回
   	SYSTEMTIME time;
	//取系统当前时间
	GetLocalTime(&time);
    //sprintf(chTime ,"%02d年-%02d月-%02d日 %02d时-%02d分-%02d秒 ",
    sprintf(chTime ,"%dY-%dM-%d D %dH-%dM-%dS ",
				//time.wYear - 2000,
                time.wYear,
				time.wMonth,
				time.wDay,
				time.wHour,
				time.wMinute,
				time.wSecond
			);
    fputs(chTime , stream);
	fputs(chMsg,stream);
    fputs("\n",stream);
	fclose(stream);
}