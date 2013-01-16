// UrlMonitor.cpp : Defines the initialization routines for the DLL.
//


#include "stdafx.h"
#include "UrlMonitor.h"
//#include "Wininet.h"

#include "base/base64.h"
#include "io.h"
#include "stdlib.h"
#include "stdio.h"
#include <Base/SVHttp.h>

#include <atlconv.h>
#include <afxtempl.h>
#include <afxinet.h>
#include "iostream"
using namespace std;

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
int SplitMatchContent( const CString strMatchContent,CStringList& pList);
bool IsMatch(const CString strContent, const CStringList& pStrList,  CList<int>& pIntlist);

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

void ConvertUtf8ToGBK(CString &strUtf8) 
{ 
	int len= MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, NULL,0); 
	unsigned short * wszGBK = new unsigned short[len+1]; 
	memset(wszGBK, 0, len * 2 + 2); 
	MultiByteToWideChar(CP_UTF8, 0, (LPCTSTR)strUtf8, -1, (LPWSTR)wszGBK, len); 
	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL); 
	char *szGBK=new char[len + 1]; 
	memset(szGBK, 0, len + 1); 
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL,NULL); 
	strUtf8 = szGBK; 
	delete[] szGBK; 
	szGBK = NULL;
	delete[] wszGBK; 
	wszGBK = NULL;
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

//////////////////////////////////////////////////////////////////////////
//
// ../表示向上一层
// /表示根目录下的
// XX.htm表示当前目录下的
//把URL转换成绝对地址
CString OnConversionURL(CString sURL,CString str_fafURL)
{

	if(sURL.Find("/",8)<0)
	{
		sURL +="/";
	}
	CString str_activeURL;
	int int_j = 0;
	int i=0;
	str_activeURL = str_fafURL;
	if(str_fafURL.Find("../",0)!=-1&&str_fafURL[0]!='/')
	{
		while( i<=str_fafURL.GetLength() )
		{
			if( str_fafURL[i] == '.' && str_fafURL[i+1] == '.' && str_fafURL[i+2] == '/' )
			{ 
				int_j++;
			}
			i++;
		}
		if(str_fafURL[0]=='/')
		{
			str_fafURL.Delete(0,1);
		}
		str_fafURL.Replace("../","");
		i=0;
		int int_i=0;
		while( i <= sURL.GetLength() )
		{
			if( sURL[i]=='/' )
			{ 
				int_i++;
			}
			i++;
		}
		int_i -= int_j;

		if( int_i<3 )
		{
			int_i = 3;
		}

		int int_cour=0;
		for( i=0; i<=sURL.GetLength(); i++)
		{
			if( sURL[i]=='/' )
			{ 
				int_cour++;
			}
			if( int_cour==int_i )
			{			 
				sURL= sURL.Left(i+1);
				break;
			}
		}
		//容错处理
		if( sURL[sURL.GetLength()-1]!='/' )
		{	
			sURL +="/";
		}
		sURL += str_fafURL;
		return sURL;
	}
	else
	{
		if( str_fafURL[0] =='/' )
		{
			int int_b = 0 ;
			for( int a=0; int_b<3 && a<sURL.GetLength(); a++)
			{
				if( sURL[a]=='/' )
			 {
				 int_b++;
			 }
			 if( int_b==3 )
			 {
				 sURL = sURL.Left(a);
				 break;
			 }
			}
			sURL += str_fafURL;
		}
		else
		{
			for( int i=sURL.GetLength() ; i> 0 ; i-- )
			{
				if( sURL[i] =='/' )
				{
					sURL = sURL.Left( i+1 );
					break;
				}
			}
			sURL += str_fafURL;
		}
		return sURL;
	}
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

LOOP://循环调用
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
	printf("strURL=%s\n",strURL);
	
	if(svHttp.SendRequest())
//	if(svHttp.SendAllRequest())
	{		
		CString str = _T("");
		//重定向 lish 2009.8.24
		int nStatusCode = svHttp.GetStatusCode();
		if ( nStatusCode>=300 && nStatusCode <= 306)
		{
			printf("---------- Location  Start --------------\n");
			CString strNewLocation = svHttp.GetResponseHeader();
			printf("nStatusCode=%d\n",nStatusCode);
			int nPlace = strNewLocation.Find(_T("Location: "));
			printf("strNewLocation=%s",strNewLocation);
			if (-1 == nPlace)
			{
				str.Format("error=Error: Site redirects with no new location");
				strcpy(szReturn, (LPCTSTR)str);
				return FALSE;
			}
			strNewLocation = strNewLocation.Mid(nPlace + 10);
			nPlace = strNewLocation.Find('\n');
			if (nPlace > 0)
			{
				strNewLocation = strNewLocation.Left(nPlace);
			}
			if(strNewLocation.Find("http://",0)<0)
			{
				strURL = OnConversionURL(strURL,strNewLocation);
				printf("strNewLocation=%s\n",strNewLocation);
			}
			else
			{
				strURL = strNewLocation;
				printf("strNewLocation=%s\n",strNewLocation);

			}
			printf("---------- Location  End --------------\n");
			goto LOOP;//循环

		}
		
		str.Format("status=%d$totaltime=%.2f$downloadsize=%d$", svHttp.GetStatusCode(), svHttp.GetResponseTime(), svHttp.GetPageSize());

//		printf("HttpHeader=%s\n",svHttp.GetHttpHeader());
//		printf("ResponseText=%s\n",svHttp.GetResponseHeader());
//        printf("ResponseText=%s\n",svHttp.GetResponseText());

		
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
				printf("GBK:匹配成功\nstr=%s\nstrMatchContent=%s\n",str,strMatchContent);
			}
			else
			{
				BSTR uc=UTF2GB(strReturn.GetBuffer(strReturn.GetLength()),strReturn.GetLength());
				char *ptem=W2A(uc);

				char *p=strstr(ptem,strMatchContent.GetBuffer(strMatchContent.GetLength()));
				if(p)
				{
					printf("UTF:匹配成功\nstr=%s\nstrMatchContent=%s\n",str,strMatchContent);
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

	/*
	CStringList paramList1;
	MakeStringListByChar(paramList1, strParas);
	POSITION pos1 = paramList1.GetHeadPosition();
	CString str;

	 while(pos1)
	{
		str = paramList1.GetNext(pos1);
		str += " for debug zjw";
		OutputDebugString(str);
	}
	 return true;
	*/

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
			//sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_URL",
            //"URL_DATA_MISTAKE"));//<%IDS_Apache_2%>
			sprintf(szReturn, "error=%s", "Siteview:URL:错误的数据格式！");//<%IDS_Apache_2%>
			//return FALSE;
			return TRUE;
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

void ConverGBKToBIG5(CString& szBuf)
{
	if (szBuf.IsEmpty())
	{
		return;
	}
	int nStrLen = strlen(szBuf);
	wchar_t *wszGBK = new wchar_t[nStrLen + 1];
	memset(wszGBK,0, nStrLen*2+2);
	char* szGBK = new char[nStrLen+1];
	memset(szGBK,0, nStrLen+1);
	__try
	{
		int nReturn = MultiByteToWideChar(936, 0, szBuf, nStrLen, wszGBK, nStrLen + 1);
		BOOL bValue = false;
		nReturn = WideCharToMultiByte(950, 0, wszGBK, nReturn, szGBK, nStrLen + 1, "?", &bValue);
		szGBK[nReturn] = 0;
		szBuf = szGBK;
	}
	__finally
	{
		delete[] wszGBK;
		wszGBK = NULL;
		delete[]szGBK;
		szGBK = NULL ;
	}
}
void ConverBIG5ToGBK(CString& szBuf)
{
	if(szBuf.IsEmpty())
	{
		return;
	}
	int nStrLen = strlen(szBuf);
	wchar_t *wszGBK = new wchar_t[nStrLen + 1];
	memset(wszGBK,0, nStrLen*2+2);
	char* szGBK = new char[nStrLen+1];
	memset(szGBK,0, nStrLen+1);
	__try
	{
		int nReturn = MultiByteToWideChar(950, 0, szBuf, nStrLen, wszGBK, nStrLen + 1);
		BOOL bValue = false;
		nReturn = WideCharToMultiByte(936, 0, wszGBK, nReturn, szGBK, nStrLen + 1, "?", &bValue);
		szGBK[nReturn] = 0;
		szBuf = szGBK;
	}
	__finally
	{
		delete[] wszGBK;
		delete[]szGBK;
	}
}
void ConverGBKToGB2312(CString&szBuf)
{
	if(!strcmp(szBuf, ""))
		return;
	int nStrLen = strlen(szBuf);
	WORD wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
	int nReturn = LCMapString(wLCID, LCMAP_SIMPLIFIED_CHINESE, szBuf, nStrLen, NULL, 0);
	if(!nReturn)
	{
		return;
	}
	char *pcBuf = new char[nReturn + 1];
	memset(pcBuf,0,nReturn+1);
	__try
	{
		wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_BIG5);
		LCMapString(wLCID, LCMAP_SIMPLIFIED_CHINESE, szBuf, nReturn, pcBuf, nReturn + 1);
		//strncpy(szBuf, pcBuf, nReturn);
		szBuf = pcBuf;
	}
	__finally
	{
		delete[] pcBuf;
	}

}

void ConverGB2312ToGBK(CString&szBuf)
{
	if(!strcmp(szBuf, ""))
		return;
	int nStrLen = strlen(szBuf);
	WORD wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
	int nReturn = LCMapString(wLCID, LCMAP_TRADITIONAL_CHINESE, szBuf, nStrLen, NULL, 0);
	if(!nReturn)
	{
		return;
	}
	char *pcBuf = new char[nReturn + 1];
	memset(pcBuf,0,nReturn+1);
	//char *pGBKBuf = new char[nReturn + 1];
	//memset(pGBKBuf,0,nReturn+1);
	__try
	{
		wLCID = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_CHINESE_PRC);
		LCMapString(wLCID, LCMAP_TRADITIONAL_CHINESE, szBuf, nReturn, pcBuf, nReturn + 1);
		//		strncpy(pGBKBuf, pcBuf, nReturn);
		szBuf = pcBuf;		
	}
	__finally
	{
		delete[] pcBuf;
		pcBuf = NULL;
		//delete[]pGBKBuf;
	}

}
//BIG5 => GB2312
void BIG5ToGB2(CString & strBuf)
{
	ConverBIG5ToGBK(strBuf);
	ConverGBKToGB2312(strBuf);

}

void GB2ToBIG5(CString & strBuf)
{
	ConverGB2312ToGBK(strBuf);
	ConverGBKToBIG5(strBuf);
}
extern "C" __declspec(dllexport) BOOL INETComplex(const char * strParas, char * szReturn, int& nSize)
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
		strMatchContent = _T(""),		// Match Content
		strServerIP =_T("");			// Server IP	

	CString strProxyPort = _T("");
	CString strCustomer = _T("");
	CString strInput;

	DWORD dwError=0;
	CString strError=_T("");

	CStringList paramList;

	MakeStringListByChar(paramList,strParas);

	int	nTimeout = 60, 
		nGID = 0, 
		nMID = 0,
		nCharset = 0; //字符集标识


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
		else if (strTemp.Find(__IP__,0) == 0 )
		{
			strServerIP = strTemp.Right(strTemp.GetLength() - strlen(__IP__));
		}
		else if (strTemp.Find(__CHARSET__,0) == 0)
		{
			nCharset = atoi(strTemp.Right(strTemp.GetLength() - strlen(__CHARSET__)));
		}
	}

	if(strURL.IsEmpty()) 
	{
		//<%IDS_UrlMonitor_1%>"error=缺少URL$"
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_URL",
			"URL_LOST_ADDRESS"));	
		return bRet;
	}


	//监测的实现：
	SVHttp svHttp;
LOOP://循环调用
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
	{
		svHttp.SetProxyIp(strProxyServer);
	}
	if(!strProxyPort.IsEmpty())
	{
		svHttp.SetProxyPort(strProxyPort);
	}
	if(!strProxyUsername.IsEmpty())
	{
		svHttp.SetProxyUser(strProxyUsername);
	}
	if(!strProxyPassword.IsEmpty())
	{
		svHttp.SetProxyPwd(strProxyPassword);
	}
/*	if (!strServerIP.IsEmpty())// set server ip
	{
		svHttp.SetServerIP(strServerIP);
	}*/
	svHttp.SetUrl(strURL);
	svHttp.SetPostData(strPostData);	
	svHttp.SetTimeOut(nTimeout);
	//printf("strURL=%s\n",strURL);

	//printf("strURL=%s\nstrServerIP=%s\n",strURL,strServerIP);

	if(svHttp.SendRequest())
	{		
		CString str = _T("");
		//重定向 lish 2009.8.24
		int nStatusCode = svHttp.GetStatusCode();
		//CString strRequest = svHttp.GetResponseHeader();
		//printf("strRequest=%s\n",strRequest);
		if ( nStatusCode>=300 && nStatusCode <= 306)
		{
			printf("---------- Location  Start --------------\n");
			CString strNewLocation = svHttp.GetResponseHeader();
			printf("nStatusCode=%d\n",nStatusCode);
			int nPlace = strNewLocation.Find(_T("Location: "));
			printf("strNewLocation=%s",strNewLocation);
			if (-1 == nPlace)
			{
				str.Format("error=Error: Site redirects with no new location");
				strcpy(szReturn, (LPCTSTR)str);
				return FALSE;
			}
			strNewLocation = strNewLocation.Mid(nPlace + 10);
			nPlace = strNewLocation.Find('\n');
			if (nPlace > 0)
			{
				strNewLocation = strNewLocation.Left(nPlace);
			}
			if(strNewLocation.Find("http://",0)<0)
			{
				strURL = OnConversionURL(strURL,strNewLocation);
				//printf("strNewLocation=%s\n",strNewLocation);
			}
			else
			{
				strURL = strNewLocation;
				//printf("strNewLocation=%s\n",strNewLocation);

			}
			printf("---------- Location  End --------------\n");
			goto LOOP;//循环
		}
		str.Format("status=%d$totaltime=%.2f$downloadsize=%d$", svHttp.GetStatusCode(), svHttp.GetResponseTime(), svHttp.GetPageSize());

		CString strReturn=svHttp.GetResponseText();
		
		//匹配字符串 :
		if (strMatchContent.IsEmpty()) //匹配串为空 直接返回
		{
			str.Format("%sIsMatch=匹配成功$",str);
			strcpy(szReturn, (LPCTSTR)str);
		}
		else // 进行匹配
		{
			CStringList pStrList;
			CList<int> pIntList;

			switch(nCharset)
			{
			case CHAR_GBK:
				break;
			case CHAR_BIG5:
				GB2ToBIG5(strMatchContent);
				//printf("strMatchContent=%s\n",strMatchContent);
				break;
			case CHAR_UTF8:
				ConvertUtf8ToGBK(strMatchContent);
				break;
			default:
				break;
			}
			
			//ConverGBKToGB2312(strMatchContent);
			//cout << "匹配内容: "<< strMatchContent <<endl;
			//cout << strReturn << endl;
			
			SplitMatchContent(strMatchContent , pStrList);
			//BIG5ToGB2(strMatchContent);
			//cout<< strMatchContent <<endl;
			//POSITION pos = pStrList.GetHeadPosition();
			//CString strVal;
			//while(pos)
			//{
			//	strVal = pStrList.GetNext(pos);
			//	cout<< strVal << endl;
			//}
			bool bRet = IsMatch(strReturn, pStrList, pIntList);
			if ( bRet) //GBK编码 匹配成功  
			{
				str.Format("%sIsMatch=匹配成功$",str);
				strcpy(szReturn, (LPCTSTR)str);
			}
			else  //转化成UTF-8编码 
			{
				str.Format("%sIsMatch=匹配失败$",str);
				strcpy(szReturn, (LPCTSTR)str);
				strInput = szReturn;
				MakeCharByString(szReturn,nSize,strInput);
				return FALSE ;	
				//ConvertUtf8ToGBK(strReturn);
				//bRet = IsMatch(strReturn,pStrList,pIntList);
				//cout << "bRet=" << bRet << endl;
				//POSITION pos = pIntList.GetHeadPosition();
				///*int nVal;
				//CString strLog="";
				//while(pos )
				//{
				//	nVal = pIntList.GetNext(pos);
				//	strLog.Format("%s\nnVal=%d",strLog,nVal);
				//}
				//cout << strLog << endl;
				//cout << "List Count=" << pIntList.GetCount() <<endl;
				//strLog = "";
				//pos = pStrList.GetHeadPosition();
				//while(pos)
				//{
				//	strLog += pStrList.GetNext(pos);
				//	
				//}
				//cout << strLog << endl;*/

				//if (bRet)  //匹配成功
				//{
				//	str.Format("%sIsMatch=(Yes)匹配成功$",str);
				//	strcpy(szReturn, (LPCTSTR)str);
				//}
				//else 
				//{
				//	str.Format("%sIsMatch=(NO)匹配失败$",str);
				//	strcpy(szReturn, (LPCTSTR)str);
				//	strInput = szReturn;
				//	MakeCharByString(szReturn,nSize,strInput);
				//	return FALSE ;					
				//}
			}
			
		}
	}
	else
	{
		sprintf(szReturn,"error=%s", svHttp.GetErrorMessage());
		return FALSE;
	}
	strInput = szReturn;
	MakeCharByString(szReturn,nSize,strInput);
	return TRUE;
}

//拆分匹配串
int SplitMatchContent(const CString strMatchContent, CStringList& pList )
{
	int nAndLen = strlen(__AND__);
	int nOrLen = strlen(__OR__);
	if (strMatchContent.IsEmpty())
	{
		return -1;
	}
	CString strTemp;
	int nAndPos=0, nPrePos=0, nOrPos=0;
	nAndPos = strMatchContent.Find(__AND__,0);
	nOrPos  = strMatchContent.Find(__OR__,0);
	if ( (nAndPos == -1) && (nOrPos == -1) )//只有一个匹配串
	{
		strTemp = strMatchContent;
		strTemp.TrimRight();
		strTemp.TrimLeft();
		pList.AddTail(strTemp);
		return 0;
	}
	if ( nOrPos == -1 ) //只有and 
	{
		while( nAndPos != -1 )
		{			
			strTemp = strMatchContent.Mid(nPrePos , nAndPos-nPrePos);
			strTemp.TrimLeft();
			strTemp.TrimRight();
			pList.AddTail(strTemp);
			pList.AddTail(__AND__);
			nPrePos = nAndPos+nAndLen;
			nAndPos = strMatchContent.Find(__AND__,nPrePos);
		}
		strTemp = strMatchContent.Right(strMatchContent.GetLength() - nPrePos);
		strTemp.TrimLeft();
		strTemp.TrimRight();
		pList.AddTail(strTemp);
		return 1;
	}
	if ( nAndPos == -1 ) //只有or
	{
		while( nOrPos != -1 )
		{
			strTemp = strMatchContent.Mid(nPrePos , nOrPos-nPrePos);
			strTemp.TrimLeft();
			strTemp.TrimRight();
			pList.AddTail(strTemp);
			pList.AddTail(__OR__);
			nPrePos = nOrPos+nOrLen;
			nOrPos = strMatchContent.Find(__OR__,nPrePos);			
		}
		strTemp = strMatchContent.Right(strMatchContent.GetLength() - nPrePos);
		strTemp.TrimLeft();
		strTemp.TrimRight();
		pList.AddTail(strTemp);
		return 2;
	}
	while( (nAndPos != -1) && (nOrPos != -1) ) // 二者都有
	{
		if ( (nAndPos < nOrPos))
		{
			strTemp = strMatchContent.Mid(nPrePos , nAndPos-nPrePos);
			strTemp.TrimLeft();
			strTemp.TrimRight();
			pList.AddTail(strTemp);
			pList.AddTail(__AND__);
			nPrePos = nAndPos+nAndLen;
			nAndPos = strMatchContent.Find(__AND__,nPrePos);
		}
		else
		{
			strTemp = strMatchContent.Mid(nPrePos , nOrPos-nPrePos);
			strTemp.TrimLeft();
			strTemp.TrimRight();
			pList.AddTail(strTemp);
			pList.AddTail(__OR__);
			nPrePos = nOrPos+nOrLen;
			nOrPos = strMatchContent.Find(__OR__,nPrePos);
		}

	}
	if ( nOrPos == -1 ) //只剩下and 
	{
		while( nAndPos != -1 )
		{			
			strTemp = strMatchContent.Mid(nPrePos , nAndPos-nPrePos);
			strTemp.TrimLeft();
			strTemp.TrimRight();
			pList.AddTail(strTemp);
			pList.AddTail(__AND__);
			nPrePos = nAndPos+nAndLen;
			nAndPos = strMatchContent.Find(__AND__,nPrePos);
		}
		strTemp = strMatchContent.Right(strMatchContent.GetLength() - nPrePos);
		strTemp.TrimLeft();
		strTemp.TrimRight();
		pList.AddTail(strTemp);

	}
	else //只剩下or
	{
		while( nOrPos != -1 )
		{
			strTemp = strMatchContent.Mid(nPrePos , nOrPos-nPrePos);
			strTemp.TrimLeft();
			strTemp.TrimRight();
			pList.AddTail(strTemp);
			pList.AddTail(__OR__);
			nPrePos = nOrPos+nOrLen;
			nOrPos = strMatchContent.Find(__OR__,nPrePos);			
		}
		strTemp = strMatchContent.Right(strMatchContent.GetLength() - nPrePos);
		strTemp.TrimLeft();
		strTemp.TrimRight();
		pList.AddTail(strTemp);
	}

	return 3;

}


//////////////////////////////////////////////////////////////////////////
//输入参数一 CString		网页源代码（用于查找是否有匹配的文本）
//输入参数二 CStringList&	保存了需要匹配的链表，包含与或关键字（and or）
//输出参数三 CList<int>&	用于保存返回匹配位置（0 为不匹配）
//////////////////////////////////////////////////////////////////////////
bool IsMatch(const CString strContent, const CStringList& pStrList,  CList<int>& pIntlist)
{
	CString strMatch;
	int nValue;
	if(!pIntlist.IsEmpty())
	{
		pIntlist.RemoveAll();
	}
	POSITION pos = pStrList.GetHeadPosition();	
	while(pos)
	{
		strMatch = pStrList.GetNext(pos);
		if ( (strMatch.Compare(__AND__)!= 0) && (strMatch.Compare(__OR__) != 0) )//不是与或关键字
		{
			int nPos = strContent.Find(strMatch,0);
			if ( nPos != -1 )
			{
				nValue = 1;
			}
			else
			{
				nValue =0 ;
			}
			pIntlist.AddTail(nValue);
		}
		else if (strMatch.Compare(__AND__)== 0) //如果是关键字 and 插入-1
		{
			pIntlist.AddTail(-1);
		}
		else //(strMatch.Compare(__OR__) == 0) //如果是关键字 or 插入 -2
		{
			pIntlist.AddTail(-2);
		}
	}
	//	int nVal1,nVal2;	
	bool bRet(true);
	int nCount =0;
	pos = pIntlist.GetHeadPosition();
	while(pos)
	{		
		nValue = pIntlist.GetNext(pos);
		if ( nValue!=-1 && nValue != -2 )
		{
			nCount ++;
			if (nCount == 1)
			{
				bRet = nValue;
			}
		}
		else if( nValue == -1)
		{
			nValue = pIntlist.GetNext(pos);
			bRet = (bRet && nValue) ;
		}
		else
		{
			nValue = pIntlist.GetNext(pos);
			bRet = (bRet || nValue);
		}
	}

	return bRet;
}




