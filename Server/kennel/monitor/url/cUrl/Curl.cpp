// Curl.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "Curl.h"

#include "io.h"
#include "stdlib.h"
#include "stdio.h"
#include "Wininet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define BUFFERSIZE  2048
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


#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <scsvapi/svapi.h>
void WriteLog( const char* str )
{
	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	char szProgramName[] = "curl.log";
	string	strEccPath = GetSiteViewRootPath();
	char szLogFile[128];
	sprintf( szLogFile, "%s\\itsm\\runtime\\logs\\%s", 
		     strEccPath.c_str(), szProgramName );

	
	// 判断文件大小：在不打开文件的情况下实现
	struct _stat buf;
	if( _stat( szLogFile, &buf ) == 0 )
	{
		if( buf.st_size > 1000*1024 )
		{
			FILE *log = fopen( szLogFile, "w" );
			if( log != NULL )
				fclose( log );
		}
	}
	


	FILE *log = fopen( szLogFile,"a+");
	if( log != NULL )
	{
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fclose( log );
	}

}


/////////////////////////////////////////////////////////////////////////////
// CCurlApp


BEGIN_MESSAGE_MAP(CCurlApp, CWinApp)
	//{{AFX_MSG_MAP(CCurlApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurlApp construction

CCurlApp::CCurlApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCurlApp object

CCurlApp theApp;

CString FuncGetCombineUrl(CString strReferentType, CString strReferent, CString strBaseUrl, CString strPaserStepFile)
{
	CString strReturnData = _T("");
	int nCount = 0;
	CString strSection = _T("");
	CString strKey = _T("");

	if (strReferentType == "form")
	{
		CString strFormName = _T("");
		strSection = "Forms";
		nCount = FuncGetProfileIntBy(strSection, "_formNum", strPaserStepFile);
	
		for(int i = 1; i <= nCount; i++)
		{
			CString strEachText = _T("");
			strKey.Format("_text%d", i);
			strEachText = FuncGetProfileStringBy(strSection, strKey, strPaserStepFile); 
			if(!strEachText.CompareNoCase(strReferent))
			{
				strKey.Format("_action%d", i);
				CString strAction = _T("");
				strAction = FuncGetProfileStringBy(strSection, strKey, strPaserStepFile);

				char buffer[1024] = {0};
				DWORD nsize = 1000;
				InternetCombineUrl(strBaseUrl, strAction, buffer, &nsize, ICU_DECODE);
				
				strReturnData.Format("%s", buffer);
			}
		}
	}
	else if (strReferentType == "link")
	{
		CString strFormName = _T("");
		strSection = "Links";
		nCount = FuncGetProfileIntBy(strSection, "_linkNum", strPaserStepFile);
	
		for(int i = 1; i <= nCount; i++)
		{
			CString strEachText = _T("");
			strKey.Format("_text%d", i);
			strEachText = FuncGetProfileStringBy(strSection, strKey, strPaserStepFile); 
			if(!strEachText.CompareNoCase(strReferent))
			{
				strKey.Format("_herf%d", i);
				CString strAction = _T("");
				strAction = FuncGetProfileStringBy(strSection, strKey, strPaserStepFile);

				char buffer[1024] = {0};
				DWORD nsize = 1000;
				InternetCombineUrl(strBaseUrl, strAction, buffer, &nsize, ICU_DECODE);
				
				strReturnData.Format("%s", buffer);
			}
		}
	}
	else if (strReferentType == "frame")
	{
		CString strFormName = _T("");
		strSection = "Frames";
		nCount = FuncGetProfileIntBy(strSection, "_frameNum", strPaserStepFile);
	
		for(int i = 1; i <= nCount; i++)
		{
			CString strEachText = _T("");
			strKey.Format("_text%d", i);
			strEachText = FuncGetProfileStringBy(strSection, strKey, strPaserStepFile); 
			if(!strEachText.CompareNoCase(strReferent))
			{
				strKey.Format("_src%d", i);
				CString strAction = _T("");
				strAction = FuncGetProfileStringBy(strSection, strKey, strPaserStepFile);

				char buffer[1024] = {0};
				DWORD nsize = 1000;
				InternetCombineUrl(strBaseUrl, strAction, buffer, &nsize, ICU_DECODE);
				
				strReturnData.Format("%s", buffer);
			}
		}
	}
	else if (strReferentType == "refresh")
	{
		CString strFormName = _T("");
		strSection = "Refreshes";
		nCount = FuncGetProfileIntBy(strSection, "_refreshNum", strPaserStepFile);
	
		for(int i = 1; i <= nCount; i++)
		{
			CString strEachText = _T("");
			strKey.Format("_text%d", i);
			strEachText = FuncGetProfileStringBy(strSection, strKey, strPaserStepFile); 
			if(!strEachText.CompareNoCase(strReferent))
			{
				strKey.Format("_url%d", i);
				CString strAction = _T("");
				strAction = FuncGetProfileStringBy(strSection, strKey, strPaserStepFile);

				char buffer[1024] = {0};
				DWORD nsize = 1000;
				InternetCombineUrl(strBaseUrl, strAction, buffer, &nsize, ICU_DECODE);
				
				strReturnData.Format("%s", buffer);
			}
		}
	}

	return strReturnData;
}

CString FuncGetPostItemFrom(CString strReferentType, CString strReferent, CString strPostData, CString strPaserInfoFileName)
{
	CString strReturnData = _T("");
	int nCount = 0;
	CString strSection = _T("");
	CString strKey = _T("");

	if (strReferentType == "form")
	{
		//
		strReturnData = strPostData;

		//CString strFormName = _T("");
		//strSection = "Forms";
		//nCount = FuncGetProfileIntBy(strSection, "_formNum", strPaserInfoFileName);
	
		//for(int i = 1; i <= nCount; i++)
		//{
		//	CString strEachText = _T("");
		//	strKey.Format("_text%d", i);
		//	strEachText = FuncGetProfileStringBy(strSection, strKey, strPaserInfoFileName); 
		//	if(!strEachText.CompareNoCase(strReferent))
		//	{
		//		CString strTemp = _T("");
		//		strTemp = FuncGetRightString(strEachText, "}");
		//		strFormName = strEachText;
		//		strFormName.Replace(strTemp, "");
		//		int nPostDataCount = 0;
		//		strKey.Format("_postdataNum%d", nCount);
		//		nPostDataCount = FuncGetProfileIntBy(strSection, strKey, strPaserInfoFileName);
		//		
		//		if(strPostData != "")
		//		{
		//			strPostData.Replace(strFormName, "&");
		//			strPostData = strPostData + "&";
		//			strPostData.Delete(0, 1);
		//		}

		//		CString strFinalData = _T("");
		//		for(int j = 1; j <= nPostDataCount; j++)
		//		{
		//			strKey.Format("_postdata%d_%d", i, j);
		//			CString strEachValue = _T("");
		//			strEachValue = FuncGetProfileStringBy(strSection, strKey, strPaserInfoFileName); 
		//			CString strEachPostData = _T("");
		//			strEachPostData = strEachValue;

		//			int nChangeCount = 0;
		//			nChangeCount = FuncGetCountByMark(strPostData, "&");
		//			for(int k = 1; k <= nChangeCount; k++)
		//			{
		//				CString strEachChange = _T("");
		//				strEachChange = FuncGetLeftStringByMark(strPostData, "&", k);
		//				CString strEachChangeName = _T("");
		//				strEachChangeName = FuncGetLeftStringByMark(strEachChange, "=", 1);
		//				
		//				if(strEachValue.Find(strEachChangeName, 0) > -1)
		//				{
		//					strEachPostData = strEachChange;
		//				}
		//			}

		//			strFinalData = strFinalData + strEachPostData;
		//			
		//			if(j < nPostDataCount)
		//				strFinalData = strFinalData + "&";
		//		}

		//		strReturnData = strFinalData;
		//	}
		//}
	}
	else if (strReferentType == "link")
	{
		strReturnData = strPostData;
	}	
	else if (strReferentType == "url")
	{
		strReturnData = strPostData;
	}


	return strReturnData;
}

void MyDebugFile(const char* szMsg)
{
	FILE * fp = fopen("c:\\transaction.txt", "a+");
	if (fp) {
		fputs(szMsg, fp);
		fputs("\r\n", fp);
		fclose(fp);
	}
}

BOOL HTMLparser(CString strMonitorId, CString strStepIndex, 
		   CStringList& lstLinks,
		   CStringList& lstForms,
		   CStringList& lstFormDate,
		   CStringList& lstFrames,
		   CStringList& lstRefreshs, 
		   CString strPaserFileName,
		   CString strPaserInfoFileName)		
{
	WriteLog("============== HTMLparser ==============");

	BOOL bReturn = FALSE;
	typedef MyAddTask(CString, CString, CStringList& , CStringList& ,  CStringList& , CStringList& , CStringList& , CString , CString);//, int, int
	HINSTANCE hInstance;
	MyAddTask *pAddTask;
	
	if(hInstance = ::LoadLibrary("HtmlParser.dll"))	{
		if(pAddTask = (MyAddTask *)::GetProcAddress(hInstance, "HtmlParser")) { //WriteTaskSection	
			
//			CString strStepIndex = _T("1");
//			CStringList lstLinks;
//			CStringList lstForms; 
//			CStringList lstFormDate;
//			CStringList lstFrames; 
//			CStringList lstRefreshs;
//			char szFileName[2056] = {0};

//			MyDebugFile(strPaserFileName);
//			MyDebugFile(strPaserInfoFileName);
//			MyDebugFile(strStepIndex);

			//CString strMsg = _T("");
			//strMsg.Format("GID=%d, MID=%d", nGroupID, nMonitorID);

//			MyDebugFile(strMsg);

			WriteLog("载入函数HtmlParser成功！");
			WriteLog(strPaserFileName.GetBuffer());
			WriteLog(strPaserInfoFileName.GetBuffer());
			
			bReturn = (* pAddTask)(strMonitorId, strStepIndex, lstLinks, lstForms,  lstFormDate, lstFrames, lstRefreshs, strPaserFileName, strPaserInfoFileName);

			WriteLog("运行函数HtmlParser成功！");
		}
		
		::FreeLibrary(hInstance);
	}
	return bReturn;
}

BOOL FuncFindStringInFile(CString strFileName, CString strFindString)
{
	BOOL bFindOut = FALSE;
	char buffer[BUFFERSIZE];
	LPTSTR lpFileName = NULL;
	lpFileName = strFileName.GetBuffer(strFileName.GetLength());
    int i = 0;
	char * filename = lpFileName;
	
	FILE *fp;
	int len = 0;		
	
	if( (fp  = fopen( filename, "r" )) == NULL )
		return -1;

	CStringList lstData;
	while(!feof(fp))
	{
		memset(buffer, 0, BUFFERSIZE);	
		// end line
		if( fgets( buffer, BUFFERSIZE-1, fp ) == NULL)
			break;		
		
		// remove '\n'
		len = strlen(buffer);
			
		CString strLine = _T("");
		strLine.Format("%s",buffer);
		i++;

		int nIndex = strLine.Find(strFindString,0);

		if (nIndex > -1)
		{
            bFindOut = TRUE;
		}
	}

	fclose(fp);	

	return bFindOut;
}

CString FuncGetCookieFileName(CString strPath, int nStepIndex)
{
	CString strReturnFileName = _T("");
	for(int i = nStepIndex - 1; i > 0; i --)
	{
		CString strEachFileName = _T("");
		strEachFileName.Format("%s\\cgi-bin\\StepCOOKIE%d.Info", FuncGetInstallRootPath(), i); 

		BOOL bHasSetCookie = FALSE;
		bHasSetCookie = FuncFindStringInFile(strEachFileName, "Set-Cookie");

		if(bHasSetCookie)
		{
			strReturnFileName = strEachFileName;
			break;
		}
	}

	return strReturnFileName;
}

extern "C" __declspec(dllexport) BOOL CURL(CStringList& lstIn, 
										   CStringList& lstLinks, 
										   CStringList& lstForms, 
										   CStringList& lstFormDate,
										   CStringList& lstFrames,
										   CStringList& lstRefreshs, 
										   int& nRetCode, 
										   char *szRetMsg,
										   char *szFileName,
										   char *szCookieFileName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	BOOL bRet = FALSE;

	CString strPath = ::FuncGetInstallPath();

	if (strPath.IsEmpty())
		return bRet;

	CString	strURL = _T(""),// URL
			strTimeOut = _T(""),
			strPostData = _T(""),			// Post Data
			strUsername = _T(""),			// Authorization User Name
			strPassword = _T(""),			// Authorization Password
			strProxyServer = _T(""),		// HTTP Proxy
			strProxyUsername = _T(""),		// Proxy Server User Name
			strProxyPassword = _T(""),		// Proxy Server Password
			strMatchContent = _T(""),
			strDisMatchContent = _T(""),		// Match Content
			strProxyServerPort = _T(""),
			strStepIndex = _T(""),
			strReferentType = _T(""),
			strReferent = _T(""),
			strBaseUrl = _T("");
		
	POSITION pos = lstIn.FindIndex(0);

	while(pos)
	{
		CString strTemp = lstIn.GetNext(pos);
		if(strTemp.Find("_TimeOut", 0) == 0)
		{
			strTimeOut = strTemp.Right(strTemp.GetLength() - strlen("_TimeOut")-1);
		}
		else if(strTemp.Find("_ProxyServerPort", 0) == 0)
		{
			strProxyServerPort = strTemp.Right(strTemp.GetLength() - strlen("_ProxyServerPort")-1);
		}
		else if(strTemp.Find("_ProxyUser", 0) == 0)
		{
			strProxyUsername = strTemp.Right(strTemp.GetLength() - strlen("_ProxyUser")-1);
		}
		else if(strTemp.Find("_ProxyPass", 0) == 0)
		{
			strProxyPassword = strTemp.Right(strTemp.GetLength() - strlen("_ProxyPass")-1);
		}
		else if(strTemp.Find("_baseUrl", 0) == 0)
		{
			strBaseUrl = strTemp.Right(strTemp.GetLength() - strlen("_baseUrl")-1);
		}
		else if(strTemp.Find("_ProxyServerPort", 0) == 0)
		{
			strTemp.Delete(0, strlen("_ProxyServerPort"));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				strProxyServerPort = strTemp;				
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find("_tranStep", 0) == 0)
		{
			strStepIndex = strTemp.Right(strTemp.GetLength() - strlen("_tranStep")-1);
		}

	}

	if (strStepIndex != "")
	{
		POSITION pos = lstIn.FindIndex(0);
		CString strFind = _T("");
		
		while(pos)
		{
			CString strTemp = lstIn.GetNext(pos);
			if(strTemp.Find("_referenceType", 0) == 0)
			{
				strFind.Format("_referenceType%s", strStepIndex);
				strReferentType = strTemp.Right(strTemp.GetLength() - strlen(strFind)-1);
			}
			else if(strTemp.Find("_reference", 0) == 0)
			{
				strFind.Format("_reference%s", strStepIndex);
				strReferent = strTemp.Right(strTemp.GetLength() - strlen(strFind)-1);
			}
			else if(strTemp.Find("_postData", 0) == 0)
			{
				strFind.Format("_postData%s", strStepIndex);
				strPostData = strTemp.Right(strTemp.GetLength() - strlen(strFind)-1);
			}
			else if(strTemp.Find("_username", 0) == 0)
			{
				strFind.Format("_username%s", strStepIndex);
				strUsername = strTemp.Right(strTemp.GetLength() - strlen(strFind)-1);
			}
			else if(strTemp.Find("_password", 0) == 0)
			{
				strFind.Format("_password%s", strStepIndex);
				strPassword = strTemp.Right(strTemp.GetLength() - strlen(strFind)-1);
			}
			else if(strTemp.Find("_content", 0) == 0)
			{
				strFind.Format("_content%s", strStepIndex);
				strMatchContent = strTemp.Right(strTemp.GetLength() - strlen(strFind)-1);
			}
			else if(strTemp.Find("_errorContent", 0) == 0)
			{
				strFind.Format("_errorContent%s", strStepIndex);
				strDisMatchContent = strTemp.Right(strTemp.GetLength() - strlen(strFind)-1);
			}
		}
	}

	CString strPaserStepFile = _T("");
	int nStepIndex = 0;
	nStepIndex = atoi(strStepIndex);
	strPaserStepFile.Format("%s\\cgi-bin\\PaserStep%d.Info", FuncGetInstallRootPath(), nStepIndex-1);
	CString strNextPaserStepFile = _T("");
	strNextPaserStepFile.Format("%s\\cgi-bin\\PaserStep%d.Info", FuncGetInstallRootPath(), nStepIndex);

	if(strReferentType == "url")
	{
		strURL = strReferent;
		if(strURL.IsEmpty()) {
			strcpy(szRetMsg, FuncGetStringFromIDS("<%IDS_Curl_01%>"));		//<%IDS_Curl_01%>
			return bRet;
		}
	}
	else
	{
		if(nStepIndex > 1)
			strBaseUrl = FuncGetProfileStringBy("BaseUrl", "_url", strPaserStepFile);

		strURL = FuncGetCombineUrl(strReferentType, strReferent, strBaseUrl, strPaserStepFile);
	}
	

	::DFN_WritePrivateProfileString("BaseUrl", "_url", strURL, strNextPaserStepFile);

	CString strCmdLine = _T("");
	CString strCookieFileFrom = _T("");
	CString strCookieFileTo = _T("");

	if(nStepIndex == 1)//
	{
		strCookieFileTo.Format("%s\\cgi-bin\\StepCOOKIE%d.Info", FuncGetInstallRootPath(), nStepIndex);
		//<%IDS_Curl_02%>
//		strCmdLine.Format("\"%s\\cgi-bin\\curl.exe\" \"-L\" \"-s\" \"-D\" \"%s\" \"-v\" \"-w\" \"****Begin****状态返回码=%%{http_code}，总下载时间=%%{time_total}秒，总下载字节数=%%{size_download}字节。****End****\r\n\"", FuncGetInstallRootPath(), strCookieFileTo);
		strCmdLine.Format("\"%s\\cgi-bin\\curl.exe\" \"-L\" \"-s\" \"-D\" \"%s\" \"-v\" \"-w\" \"****Begin****%s****End****\r\n\"", FuncGetInstallRootPath(), strCookieFileTo,FuncGetStringFromIDS("<%IDS_Curl_02%>"));
//		strCmdLine.Format("\"%s\\cgi-bin\\curl.exe\" \"-s\" \"-D\" \"%s\" ", (LPCTSTR)strPath, strCookieFileTo);
	}
	else
	{
		CString strTransPostData = _T("");
		strCookieFileFrom = FuncGetCookieFileName(strPath, nStepIndex);//.Format("%s\\cgi-bin\\StepCOOKIE%d.Info", (LPCTSTR)strPath, 2);//nStepIndex - 1
		strCookieFileTo.Format("%s\\cgi-bin\\StepCOOKIE%d.Info", FuncGetInstallRootPath(), nStepIndex);
		if(strCookieFileFrom == "")
			//<%IDS_Curl_03%>
			strCmdLine.Format("\"%s\\cgi-bin\\curl.exe\" \"-L\" \"-s\" \"-D\" \"%s\" \"-v\" \"-w\" \"****Begin****%s****End****\r\n\"", FuncGetInstallRootPath(), strCookieFileTo,FuncGetStringFromIDS("<%IDS_Curl_03%>"));
		else  //<%IDS_Curl_04%>
			strCmdLine.Format("\"%s\\cgi-bin\\curl.exe\" \"-L\" \"-s\" \"-D\" \"%s\" \"-b\" \"%s\" \"-v\" \"-w\" \"****Begin****%s****End****\r\n\"", FuncGetInstallRootPath(), strCookieFileTo, strCookieFileFrom,FuncGetStringFromIDS("<%IDS_Curl_04%>"));

//		strCmdLine.Format("\"%s\\cgi-bin\\curl.exe\" \"-s\" \"-D\" \"%s\" \"-b\" \"%s\" \"-v\"", (LPCTSTR)strPath, strCookieFileTo, strCookieFileFrom);
	}


//	strCmdLine.Format("\"%s\\cgi-bin\\curl.exe\" \"-i\" \"-v\" \"-w\" \"****Begin****status=%%{http_code}$totaltime=%%{time_total}$downloadsize=%%{size_download}$****End****\" \"-s\"", (LPCTSTR)strPath);

/*	if(!strTimeOut.IsEmpty())
	{
		strCmdLine += " \"-m\" \"";
		strCmdLine += strTimeOut;
		strCmdLine += "\"";
	}
*/
	if(!strUsername.IsEmpty())
	{
		strCmdLine += " \"-u\" \"";
		strCmdLine += strUsername;
		strCmdLine += ":";
		strCmdLine += strPassword;
		strCmdLine += "\"";
	}

	if(!strProxyServer.IsEmpty())
	{
		strCmdLine += " \"-x\" \"";
		strCmdLine += strProxyServer;
		strCmdLine += ":";
		strCmdLine += strProxyServerPort;
		strCmdLine += "\"";		
	}

	if(!strProxyUsername.IsEmpty())
	{
		strCmdLine += " \"-u\" \"";
		strCmdLine += strProxyUsername;
		strCmdLine += ":";

		if (strProxyPassword.IsEmpty()) // just for curl
			strProxyPassword = _T("password");

		strCmdLine += strProxyPassword;
		strCmdLine += "\"";
	}


//strPostData = "{[1]frmpost}Account=Admin{[1]frmpost}Password=";//%%B5%%C7++%%C2%%BC
	strPostData = FuncGetPostItemFrom(strReferentType, strReferent, strPostData, strPaserStepFile);

	if(!strPostData.IsEmpty())
	{
		strCmdLine += " \"-d\" \"";
		strCmdLine += strPostData;
		strCmdLine += "\"";
	}
	// url will be modified when use proxy
	if(!strProxyServer.IsEmpty()) {
		CString strTemp = _T("");
		strTemp = strURL.Right(1);
		if (strTemp.CompareNoCase("/")) {
			int nIndex = strURL.ReverseFind('.');
			if (nIndex != -1) {
				strTemp = strURL.Right(strURL.GetLength() - nIndex - 1);
				if (!strTemp.CompareNoCase("asp")) {
				}
				else if (!strTemp.CompareNoCase("html")) {
				}
				else if (!strTemp.CompareNoCase("htm")) {
				}
				else if (!strTemp.CompareNoCase("xml")) {
				}
				else if (!strTemp.CompareNoCase("js")) {
				}
				else {
					strURL += "/";
				}
			}
		}		
	}

	strCmdLine += " \"";
	strCmdLine += strURL;
	strCmdLine += "\"";

//	puts(strCmdLine);

	int nOldOutput = _dup(1);

	CString strTempFile = _T("");
	//strTempFile.Format("%s\\cgi-bin\\CUrlStep%s.html", FuncGetInstallRootPath(), strStepIndex);
	strTempFile.Format("%s\\htdocs\\CUrlStep%s.html", FuncGetInstallRootPath(), strStepIndex);
//	strTempFile = _T("c:\\test.txt");
//	strTempFile.Format("curl.html", (LPCTSTR)strPath);
	strcpy(szFileName, strTempFile);

	CString strCookieFile = _T("");

	strCookieFile.Format("%s\\cgi-bin\\StepCOOKIE%d.Info", FuncGetInstallRootPath(), nStepIndex);
	strcpy(szCookieFileName, strCookieFile);


	FILE *stream = freopen((LPCTSTR)strTempFile, "w", stdout);
	if (stream) {
		HANDLE hOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		if (hOutHandle) {
			SECURITY_ATTRIBUTES sa;	
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.bInheritHandle = TRUE;
			sa.lpSecurityDescriptor = NULL;

			STARTUPINFO si;
			memset(&si, 0, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			//si.dwFlags = STARTF_USESTDHANDLES;
			//si.hStdOutput = hOutHandle;
			//si.hStdError = hOutHandle;

			PROCESS_INFORMATION pi;
			memset(&pi, 0, sizeof(PROCESS_INFORMATION));

			if (CreateProcess(NULL, strCmdLine.GetBuffer(strCmdLine.GetLength()),  NULL, NULL, TRUE, CREATE_NEW_CONSOLE/*CREATE_NO_WINDOW*/, NULL, NULL, &si, &pi)) 
			{				
				DWORD dwResult = WaitForSingleObject(pi.hProcess, atoi(strTimeOut)*1000);
				if (dwResult == WAIT_OBJECT_0) {
					
//					fclose(stream);
//					stream = NULL;

					DWORD dwExitCode = 0;
					if (GetExitCodeProcess(pi.hProcess, &dwExitCode)) {
						if (dwExitCode == 0) { // curl is ok							
							CFile myFile(strTempFile, CFile::modeRead|CFile::shareDenyNone);

							int nFileLength = myFile.GetLength();
							myFile.Close();
							
							if (nFileLength > 0) {
								bRet = TRUE;
								char * buffer = NULL;
								buffer = (char*)malloc(nFileLength+1);
								if (buffer) {
									memset(buffer, 0, nFileLength+1);
									FILE * fp = fopen((LPCTSTR)strTempFile, "r");
									if (fp) {
										fread(buffer, sizeof(char), nFileLength+1, fp);
										fclose(fp);
										
										CString strSource = _T("");
										strSource.Format("%s", buffer);
										
										int nLeftIndex = strSource.Find("****Begin****", 0);

										if (nLeftIndex == 0) {
											int nRightIndex = strSource.Find("****End****", nLeftIndex);
											if (nRightIndex != -1) {
												int nLength1 = strlen("****Begin****");
												int nLength2 = strlen("****End****");
												
												strSource.Delete(0, nRightIndex + nLength2);

											}
										}

										if (nLeftIndex != -1) 
										{
											int nRightIndex = strSource.Find("****End****", nLeftIndex);
											if (nRightIndex != -1) 
											{
												int nLength = strlen("****Begin****");
												
												CString str = strSource.Mid(nLeftIndex + nLength , nRightIndex - nLeftIndex - nLength);
												
												str =FuncGetStringFromIDS("<%IDS_Curl_05%>") + str;//<%IDS_Curl_05%>

												if (strMatchContent.IsEmpty()) 
												{
													strcpy(szRetMsg, (LPCTSTR)str);
												}
												else 
												{
													if (!strstr(buffer, (LPCTSTR)strMatchContent))
													{
														OutputDebugString(strMatchContent);

														strcpy(szRetMsg, (LPCTSTR)str);
														//strcat(szRetMsg, FuncGetStringFromIDS("<%IDS_Curl_06%>"));//strcpy(szRetMsg, FuncGetStringFromIDS("<%IDS_Curl_06%>"));//<%IDS_Curl_06%>
														strcat(szRetMsg, "error= Content Not Match$");
													}
													else
													{
														OutputDebugString("else");
														OutputDebugString(strMatchContent);
														strcpy(szRetMsg, (LPCTSTR)str);													
													}
												}
												
												if(!strstr(szRetMsg, "error"))
												{	
													if (strDisMatchContent.IsEmpty()) 
													{
														strcpy(szRetMsg, (LPCTSTR)str);
													}
													else 
													{
														if (strstr(buffer, (LPCTSTR)strDisMatchContent))
														{
															strcpy(szRetMsg, (LPCTSTR)str);
															//strcat(szRetMsg, FuncGetStringFromIDS("<%IDS_Curl_07%>"));//strcpy(szRetMsg, FuncGetStringFromIDS("<%IDS_Curl_07%>"));//<%IDS_Curl_07%>
															strcat(szRetMsg, "error= DisMatchContent Not Match$");
														}
														else
															strcpy(szRetMsg, (LPCTSTR)str);
														
													}	
												}
											}
										}
									}
									free(buffer);
								}
							}
						}
					}
//parser
					//cxy 暂时注释
					//HTMLparser(0, 0, strStepIndex, lstLinks, lstForms, lstFormDate, lstFrames, lstRefreshs, strTempFile, "");					

				}
				else if (dwResult == WAIT_TIMEOUT){
					HANDLE hProcess;
					hProcess = OpenProcess (PROCESS_ALL_ACCESS, FALSE, pi.dwProcessId);
					if (hProcess) {
						TerminateProcess(hProcess, 1);
						CloseHandle(hProcess);
					}
					strcpy(szRetMsg, FuncGetStringFromIDS("<%IDS_Curl_08%>"));//<%IDS_Curl_08%>
				}
				else {
				}				
			}
		}
//		if (stream)
//			fclose(stream);

		SetStdHandle(STD_OUTPUT_HANDLE, hOutHandle); // Debug may be error
	}	

	sscanf(szRetMsg, "status=%d$", &nRetCode);
	_dup2(nOldOutput, 1);

	return bRet;
}


CString FuncGetMONITORCookieFileName(CString strMonitorId, CString strPath, int nStepIndex)
{
	CString strReturnFileName = _T("");
	for(int i = nStepIndex - 1; i > 0; i --)
	{
		CString strEachFileName = _T("");
		strEachFileName.Format("%s\\cgi-bin\\%s_StepCOOKIE%d.Info", FuncGetInstallRootPath(), strMonitorId, i); 

		BOOL bHasSetCookie = FALSE;
		bHasSetCookie = FuncFindStringInFile(strEachFileName, "HTTP Cookie File");

		if(bHasSetCookie)
		{
			strReturnFileName = strEachFileName;
			break;
		}
	}

	return strReturnFileName;
}

extern "C" __declspec(dllexport) BOOL MONITOR_CURL(CString strMonitorId, CStringList& lstIn,char *szRetMsg, bool bPaserStep)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	WriteLog("=============== MONITOR_CURL =================");

	BOOL bRet = FALSE;

	CStringList lstLinks, lstForms, lstFormDate, lstFrames, lstRefreshs;
	CString strPath = ::FuncGetInstallPath();
	if (strPath.IsEmpty())
		return bRet;

	CString	strURL = _T(""),// URL
			strTimeOut = _T(""),
			strPostData = _T(""),			// Post Data
			strUsername = _T(""),			// Authorization User Name
			strPassword = _T(""),			// Authorization Password
			strProxyServer = _T(""),		// HTTP Proxy
			strProxyUsername = _T(""),		// Proxy Server User Name
			strProxyPassword = _T(""),		// Proxy Server Password
			strMatchContent = _T(""),
			strDisMatchContent = _T(""),		// Match Content
			strProxyServerPort = _T(""),
			strStepIndex = _T(""),
			strReferentType = _T(""),
			strReferent = _T(""),
			strBaseUrl = _T("");
		
	POSITION pos = lstIn.FindIndex(0);

	while(pos)
	{
		CString strTemp = lstIn.GetNext(pos);
		if(strTemp.Find("_TimeOut", 0) == 0)
		{
			strTimeOut = strTemp.Right(strTemp.GetLength() - strlen("_TimeOut")-1);
		}
		else if(strTemp.Find("_ProxyServerPort", 0) == 0)
		{
			strProxyServerPort = strTemp.Right(strTemp.GetLength() - strlen("_ProxyServerPort")-1);
		}
		else if(strTemp.Find("_ProxyUser", 0) == 0)
		{
			strProxyUsername = strTemp.Right(strTemp.GetLength() - strlen("_ProxyUser")-1);
		}
		else if(strTemp.Find("_ProxyPass", 0) == 0)
		{
			strProxyPassword = strTemp.Right(strTemp.GetLength() - strlen("_ProxyPass")-1);
		}
		else if(strTemp.Find("_baseUrl", 0) == 0)
		{
			strBaseUrl = strTemp.Right(strTemp.GetLength() - strlen("_baseUrl")-1);
		}
		else if(strTemp.Find("_ProxyServerPort", 0) == 0)
		{
			strTemp.Delete(0, strlen("_ProxyServerPort"));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServer = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				strProxyServerPort = strTemp;				
			}
			else
			{
				strProxyServer = strTemp;
			}
		}
		else if(strTemp.Find("_tranStep", 0) == 0)
		{
			strStepIndex = strTemp.Right(strTemp.GetLength() - strlen("_tranStep")-1);
		}

	}

	if (strStepIndex != "")
	{
		POSITION pos = lstIn.FindIndex(0);
		CString strFind = _T("");
		
		while(pos)
		{
			CString strTemp = lstIn.GetNext(pos);
			if(strTemp.Find("_referenceType", 0) == 0)
			{
				strFind.Format("_referenceType%s", strStepIndex);
				strReferentType = strTemp.Right(strTemp.GetLength() - strlen(strFind)-1);
			}
			else if(strTemp.Find("_reference", 0) == 0)
			{
				strFind.Format("_reference%s", strStepIndex);
				strReferent = strTemp.Right(strTemp.GetLength() - strlen(strFind)-1);
			}
			else if(strTemp.Find("_postData", 0) == 0)
			{
				strFind.Format("_postData%s", strStepIndex);
				strPostData = strTemp.Right(strTemp.GetLength() - strlen(strFind)-1);
			}
			else if(strTemp.Find("_username", 0) == 0)
			{
				strFind.Format("_username%s", strStepIndex);
				strUsername = strTemp.Right(strTemp.GetLength() - strlen(strFind)-1);
			}
			else if(strTemp.Find("_password", 0) == 0)
			{
				strFind.Format("_password%s", strStepIndex);
				strPassword = strTemp.Right(strTemp.GetLength() - strlen(strFind)-1);
			}
			else if(strTemp.Find("_content", 0) == 0)
			{
				strFind.Format("_content%s", strStepIndex);
				strMatchContent = strTemp.Right(strTemp.GetLength() - strlen(strFind)-1);
			}
			else if(strTemp.Find("_errorContent", 0) == 0)
			{
				strFind.Format("_errorContent%s", strStepIndex);
				strDisMatchContent = strTemp.Right(strTemp.GetLength() - strlen(strFind)-1);
			}
		}
	}

	CString strPaserStepFile = _T("");
	int nStepIndex = 0;
	nStepIndex = atoi(strStepIndex);
	strPaserStepFile.Format("%s\\cgi-bin\\%s_PaserStep%d.Info", FuncGetInstallRootPath(), strMonitorId, nStepIndex-1);

	CString strNextPaserStepFile = _T("");
	strNextPaserStepFile.Format("%s\\cgi-bin\\%s_PaserStep%d.Info", FuncGetInstallRootPath(), strMonitorId, nStepIndex);

	//运行总的流程时不做， 添加步数时做
	if(bPaserStep)
		DeleteFile(strNextPaserStepFile);
	
	if(strReferentType == "url")
	{
		strURL = strReferent;
		if(strURL.IsEmpty()) {
			strcpy(szRetMsg, FuncGetStringFromIDS("<%IDS_Curl_09%>"));		//<%IDS_Curl_09%>
			return bRet;
		}
	}
	else
	{
		if(nStepIndex > 1)
			strBaseUrl = FuncGetProfileStringBy("BaseUrl", "_url", strPaserStepFile);

		strURL = FuncGetCombineUrl(strReferentType, strReferent, strBaseUrl, strPaserStepFile);
	}
	
	//运行总的流程时不做， 添加步数时做
	if(bPaserStep)
		::DFN_WritePrivateProfileString("BaseUrl", "_url", strURL, strNextPaserStepFile);

	CString strCmdLine = _T("");
	CString strCookieFileFrom = _T("");
	CString strCookieFileTo = _T("");
	CString strHeadFile = _T("");

	if(nStepIndex == 1)//
	{
		strHeadFile.Format("%s\\cgi-bin\\%s_HEADER%d.Info", FuncGetInstallRootPath(), strMonitorId, nStepIndex);
		strCookieFileTo.Format("%s\\cgi-bin\\%s_StepCOOKIE%d.Info", FuncGetInstallRootPath(), strMonitorId, nStepIndex);
		strCmdLine.Format("\"%s\\cgi-bin\\curl.exe\" \"-L\" \"-s\" -D \"%s\" -c \"%s\" \"-v\" \"-w\" \"****Begin****steptotaltime=%%{time_total}****End****\r\n\"", FuncGetInstallRootPath(), strHeadFile,strCookieFileTo);
	}
	else
	{
		CString strTransPostData = _T("");
		strCookieFileFrom = FuncGetMONITORCookieFileName(strMonitorId, strPath, nStepIndex);//.Format("%s\\cgi-bin\\StepCOOKIE%d.Info", (LPCTSTR)strPath, 2);//nStepIndex - 1
		strCookieFileTo.Format("%s\\cgi-bin\\%s_StepCOOKIE%d.Info", FuncGetInstallRootPath(), strMonitorId, nStepIndex);
		strHeadFile.Format("%s\\cgi-bin\\%s_HEADER%d.Info", FuncGetInstallRootPath(), strMonitorId, nStepIndex);
		if(strCookieFileFrom == "")
			strCmdLine.Format("\"%s\\cgi-bin\\curl.exe\" \"-L\" \"-s\" -D \"%s\" -c \"%s\" \"-v\" \"-w\" \"****Begin****steptotaltime=%%{time_total}****End****\r\n\"", FuncGetInstallRootPath(), strCookieFileTo,strHeadFile);
		else
			strCmdLine.Format("\"%s\\cgi-bin\\curl.exe\" \"-L\" \"-s\" -D \"%s\"  -c \"%s\" -b \"%s\" \"-v\" \"-w\" \"****Begin****steptotaltime=%%{time_total}****End****\r\n\"", FuncGetInstallRootPath(),strHeadFile, strCookieFileTo, strCookieFileFrom);

//		strCmdLine.Format("\"%s\\cgi-bin\\curl.exe\" \"-s\" \"-D\" \"%s\" \"-b\" \"%s\" \"-v\"", (LPCTSTR)strPath, strCookieFileTo, strCookieFileFrom);
	}


//	strCmdLine.Format("\"%s\\cgi-bin\\curl.exe\" \"-i\" \"-v\" \"-w\" \"****Begin****status=%%{http_code}$totaltime=%%{time_total}$downloadsize=%%{size_download}$****End****\" \"-s\"", (LPCTSTR)strPath);

/*	if(!strTimeOut.IsEmpty())
	{
		strCmdLine += " \"-m\" \"";
		strCmdLine += strTimeOut;
		strCmdLine += "\"";
	}
*/
	if(!strUsername.IsEmpty())
	{
		strCmdLine += " \"-u\" \"";
		strCmdLine += strUsername;
		strCmdLine += ":";
		strCmdLine += strPassword;
		strCmdLine += "\"";
	}

	if(!strProxyServer.IsEmpty())
	{
		strCmdLine += " \"-x\" \"";
		strCmdLine += strProxyServer;
		strCmdLine += ":";
		strCmdLine += strProxyServerPort;
		strCmdLine += "\"";		
	}

	if(!strProxyUsername.IsEmpty())
	{
		strCmdLine += " \"-u\" \"";
		strCmdLine += strProxyUsername;
		strCmdLine += ":";

		if (strProxyPassword.IsEmpty()) // just for curl
			strProxyPassword = _T("password");

		strCmdLine += strProxyPassword;
		strCmdLine += "\"";
	}

//strPostData = "{[1]frmpost}Account=Admin{[1]frmpost}Password=";//%%B5%%C7++%%C2%%BC
	strPostData = FuncGetPostItemFrom(strReferentType, strReferent, strPostData, strPaserStepFile);

	if(!strPostData.IsEmpty())
	{
		strCmdLine += " \"-d\" \"";
		strCmdLine += strPostData;
		strCmdLine += "\"";
	}
	// url will be modified when use proxy
	if(!strProxyServer.IsEmpty()) {
		CString strTemp = _T("");
		strTemp = strURL.Right(1);
		if (strTemp.CompareNoCase("/")) {
			int nIndex = strURL.ReverseFind('.');
			if (nIndex != -1) {
				strTemp = strURL.Right(strURL.GetLength() - nIndex - 1);
				if (!strTemp.CompareNoCase("asp")) {
				}
				else if (!strTemp.CompareNoCase("html")) {
				}
				else if (!strTemp.CompareNoCase("htm")) {
				}
				else if (!strTemp.CompareNoCase("xml")) {
				}
				else if (!strTemp.CompareNoCase("js")) {
				}
				else {
					strURL += "/";
				}
			}
		}		
	}

	strCmdLine += " \"";
	strURL.TrimLeft();
	strURL.TrimRight();
	strCmdLine += strURL;
	strCmdLine += "\"";

//	puts(strCmdLine);

	//int nOldOutput = _dup(1);

	CString strTempFile = _T("");
	//strTempFile.Format("%s\\cgi-bin\\%s_CUrlStep%s.html", FuncGetInstallRootPath(), strMonitorId, strStepIndex);
	strTempFile.Format("%s\\htdocs\\%s_CUrlStep%s.html", FuncGetInstallRootPath(), strMonitorId, strStepIndex);
	//strTempFile = strBaseUrl;

	//FILE *stream = freopen((LPCTSTR)strTempFile, "w", stdout);
	//if (stream) 
	{
		//HANDLE hOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		//if (hOutHandle)
		{
			HANDLE hRead,hWrite;
			
			SECURITY_ATTRIBUTES sa;	
			sa.nLength = sizeof(SECURITY_ATTRIBUTES);
			sa.bInheritHandle = TRUE;
			sa.lpSecurityDescriptor = NULL;
			
			if (!CreatePipe(&hRead,&hWrite,&sa,1200000))
			{
				//MessageBox("Error On CreatePipe()");
				return false;
			}

		//if (! SetStdHandle(STD_OUTPUT_HANDLE, hWrite)) 
		//{
		//	//ErrorExit("Redirecting STDOUT failed"); 
		//	return false;
		//}
			STARTUPINFO si;
			memset(&si, 0, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			GetStartupInfo(&si);
			//si.dwFlags = STARTF_USESTDHANDLES;
			//si.hStdOutput = hOutHandle;
			//si.hStdError = hOutHandle;
			si.hStdError = hWrite;
			si.hStdOutput = hWrite;
			si.wShowWindow = SW_HIDE;
			si.dwFlags =STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;

			PROCESS_INFORMATION pi;
			memset(&pi, 0, sizeof(PROCESS_INFORMATION));

			if (CreateProcess(NULL, strCmdLine.GetBuffer(strCmdLine.GetLength()),  NULL, NULL, TRUE, CREATE_NEW_CONSOLE/*CREATE_NO_WINDOW*/, NULL, NULL, &si, &pi)) 
			{				
				DWORD dwResult = WaitForSingleObject(pi.hProcess, atoi(strTimeOut)*1000);
				if (dwResult == WAIT_OBJECT_0) {
//					fclose(stream);
//					stream = NULL;

					DWORD dwExitCode = 0;
					if (GetExitCodeProcess(pi.hProcess, &dwExitCode)) {
						if (dwExitCode == 0) 
						{ // curl is ok
							bRet = TRUE;							
							CloseHandle(hWrite);

						CFile* pFile = NULL;
						int nFileLength = 0;
						TRY	
						{
							pFile = new CFile(hRead);								
							nFileLength = pFile->GetLength();
						}
						CATCH(CFileException, pEx) 
						{
							// Do Nothing.
						}
						END_CATCH
							
						char * buffer = NULL;
						buffer = (char*)malloc(nFileLength+1);
						
						//
						pFile->Read(buffer, nFileLength);

						WriteLog("从curl.exe返回的信息：");
						WriteLog(buffer);
						
						//pFile->SetFilePath(strTempFile);
						//
						//pFile->Write(buffer, nFileLength+1);
						//pFile->Flush();

						//CString strSource = _T("");
						//strSource.Format("%s", buffer);

						//	CString csBuffer = "";
						//	char buffer[1200000] = {0};
						//	DWORD bytesRead;
						//	while(true){
						//		if (ReadFile(hRead,buffer,1200000,&bytesRead,NULL) == NULL)
						//			break;
						//		csBuffer += buffer;
						//		Sleep(200);
						//	}
						//	
						//OutputDebugString(buffer);
						pFile->Close();
						CFile myFile(strTempFile, CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone);
						myFile.Write(buffer, nFileLength+1);
						myFile.Flush();
						//int nFileLength = myFile.GetLength();
						myFile.Close();
						//	
						//	
						//	if (nFileLength > 0) {
						//		char * buffer = NULL;
						//		buffer = (char*)malloc(nFileLength+1);
								if (buffer) {
									memset(buffer, 0, nFileLength+1);
									FILE * fp = fopen((LPCTSTR)strTempFile, "r");
									if (fp) {
										fread(buffer, sizeof(char), nFileLength+1, fp);
										fclose(fp);
										
										CString strSource = _T("");
										strSource.Format("%s", buffer);
										
										int nLeftIndex = strSource.Find("****Begin****", 0);

										if (nLeftIndex == 0) {
											int nRightIndex = strSource.Find("****End****", nLeftIndex);
											if (nRightIndex != -1) {
												int nLength1 = strlen("****Begin****");
												int nLength2 = strlen("****End****");
												
												strSource.Delete(0, nRightIndex + nLength2);

											}
										}

										if (nLeftIndex != -1) {
											int nRightIndex = strSource.Find("****End****", nLeftIndex);
											if (nRightIndex != -1) {
												int nLength = strlen("****Begin****");
												
												CString str = strSource.Mid(nLeftIndex + nLength , nRightIndex - nLeftIndex - nLength);
												str.Insert(4, strStepIndex);
												str = str + "$";

												if (strMatchContent.IsEmpty()) {
													strcpy(szRetMsg, (LPCTSTR)str);
												}
												else {
													if (!strstr(buffer, (LPCTSTR)strMatchContent))
													{
														//strcpy(szRetMsg, FuncGetStringFromIDS("<%IDS_Curl_10%>"));//<%IDS_Curl_10%>
														strcpy(szRetMsg, "error= Content Not Match$");
													    return -1;
													}
													else
														strcpy(szRetMsg, (LPCTSTR)str);
													
												}

												if (strDisMatchContent.IsEmpty()) {
													strcpy(szRetMsg, (LPCTSTR)str);
												}
												else {
													if (strstr(buffer, (LPCTSTR)strDisMatchContent))
													{
														//strcpy(szRetMsg, FuncGetStringFromIDS("<%IDS_Curl_11%>"));//<%IDS_Curl_11%>
														strcat(szRetMsg, "error= DisMatchContent Not Match$");
														return -1;
													}
													else
														strcpy(szRetMsg, (LPCTSTR)str);
													
												}	
											}
										}

										//****************	得到返回码

										TRY
										{
										   CFile headFile(strHeadFile, CFile::modeRead|CFile::shareDenyNone);
										   char cCodeLine[100]={0};	
											headFile.Read(cCodeLine,99);
											CString strCode;
											strCode.Format("%s",cCodeLine);
											strCode.Replace("HTTP/1.1 ","");
											strCode.Replace("HTTP/1.0 ","");
											strCode=strCode.SpanExcluding(" ");
											if( (400<= atoi(strCode))||
												(0== atoi(strCode)) )
											{
												sprintf(szRetMsg,"error=返回码=%s(第%d步)$",nStepIndex, strCode);
												bRet=TRUE;
											}

										}
										CATCH( CFileException, e )
										{
										}
										END_CATCH
										
									}
									free(buffer);
								}
							//}
						}
					}
					
					//是否加条件判断， 运行总的流程时不需要分析， 添加步数时也只需要它就可以...
					//parser
					//HTMLparser(nGroupID, nMonitorID, strStepIndex, lstLinks, lstForms, lstFormDate, lstFrames, lstRefreshs, strTempFile, "");
					if(bPaserStep)
					{
						//if(strReferentType == "form")
							HTMLparser(strMonitorId, strStepIndex, lstLinks, lstForms, lstFormDate, lstFrames, lstRefreshs, strTempFile, "");
						//else
						//	HTMLparser(strMonitorId, strStepIndex, lstLinks, lstForms, lstFormDate, lstFrames, lstRefreshs, strURL, "");						
						
					}
				}
				else if (dwResult == WAIT_TIMEOUT){
					HANDLE hProcess;
					hProcess = OpenProcess (PROCESS_ALL_ACCESS, FALSE, pi.dwProcessId);
					if (hProcess) {
						TerminateProcess(hProcess, 1);
						CloseHandle(hProcess);
					}

					//strcpy(szRetMsg, FuncGetStringFromIDS("<%IDS_Curl_12%>"));//<%IDS_Curl_12%>
					strcpy(szRetMsg, "error= Connect Server Error!$");

					WriteLog("执行curl.exe超时！");
					return false;

				}
				else {
				}				
			}
		}
//		if (stream)
//			fclose(stream);

		//SetStdHandle(STD_OUTPUT_HANDLE, hOutHandle); // Debug may be error
	}	

	//_dup2(nOldOutput, 1);

	return bRet;
}
