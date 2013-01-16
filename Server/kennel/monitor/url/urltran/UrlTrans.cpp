// UrlTrans.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "UrlTrans.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <scsvapi/svapi.h>
//#include <scsvapi/idcsvapi.h>
#include <scsvapi/svdbapi.h>
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
// CUrlTransApp

BEGIN_MESSAGE_MAP(CUrlTransApp, CWinApp)
	//{{AFX_MSG_MAP(CUrlTransApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUrlTransApp construction

CUrlTransApp::CUrlTransApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CUrlTransApp object

CUrlTransApp theApp;

typedef BOOL(MONITOR_CURL)(CString strMonitorId, CStringList& lstIn,char *szRetMsg, bool bPaserStep);

#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
void WriteLog( const char* str )
{
	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	char szProgramName[] = "urltrans.log";
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

void DebugParasInfo(CStringList& lstParas)
{
	FILE * fp = NULL;
	fp = fopen("urltrans.log", "a+");
	if (!fp)
		return;

	POSITION pos = lstParas.GetHeadPosition();
	while (pos) {
		CString str = lstParas.GetNext(pos);
		fputs((LPCTSTR)str, fp);
		fputs("\n", fp);
	}
	fputs("-------------------------------------\n", fp);

	fclose(fp);
}

BOOL GetTimeFromString(const char * szProperty, const CString strSource, float& fRet)
{
	BOOL bRet = FALSE;

	int nLeftIndex = strSource.Find(szProperty, 0);
	if (nLeftIndex == -1)
		return bRet;

	int nRightIndex = strSource.Find("$", nLeftIndex);
	if (nRightIndex == -1)
		return bRet;

	int nLength = strlen(szProperty);

	CString str = strSource.Mid(nLeftIndex + nLength, nRightIndex - nLeftIndex - nLength);
	fRet= atof((LPCTSTR)str);

	bRet = TRUE;
	return bRet;
}

void GetRunParametersByMid(CString strMonitorId, CStringList& lstOut, int& nTranStep)
{
	
	CString strPara = _T("");
	
	string strIniFileName = strMonitorId;
	strIniFileName += "_UrlStepInfo.ini";

	std::list<string> keylist;
	std::list<string>::iterator keyitem;
	string strIndex, strStepName, strStepType, 	strPostData, strContent, strErrorContent, strUserName, strUserPwd;

	//仅有一步时应该不从ini读取参数， 直接从输入参数里取

	////从ini获取步列表
	//if(nSections <= 1)
	//{
	//	//没有进行步数配置， 以默认配置参数0段来进行监测(需要主动进行一次分析吗?， 应该不需要，只有增添步数时才分析)......

	//	//或者不承认这种情况， 直接返回错误提示没有进行步数配置.....
	//	nTranStep ++;
	//	strStepName = GetIniFileString((*keyitem), "StepName", "", strIniFileName);
	//	strPara.Format("_reference%s=%s", strIndex.c_str(), strStepName.c_str());
	//	lstOut.AddTail(strPara);

	//	strStepType = GetIniFileString((*keyitem), "StepType", "", strIniFileName);	
	//	strPara.Format("_referenceType%s=%s", strIndex.c_str(), strStepType.c_str());
	//	lstOut.AddTail(strPara);

	//	strPostData = GetIniFileString((*keyitem), "StepPostData", "", strIniFileName);
	//	strPara.Format("_postData%s=%s", strIndex.c_str(), strPostData.c_str());
	//	lstOut.AddTail(strPara);

	//	strContent = GetIniFileString((*keyitem), "StepContent", "", strIniFileName);
	//	strPara.Format("_content%s=%s", strIndex.c_str(), strContent.c_str());
	//	lstOut.AddTail(strPara);

	//	strErrorContent = GetIniFileString((*keyitem), "StepErrorContent", "", strIniFileName);
	//	strPara.Format("_errorContent%s=%s", strIndex.c_str(), strErrorContent.c_str());
	//	lstOut.AddTail(strPara);

	//	strUserName = GetIniFileString((*keyitem), "StepUserName", "", strIniFileName);
	//	strPara.Format("_username%s=%s", strIndex.c_str(), strUserName.c_str());
	//	lstOut.AddTail(strPara);

	//	strUserPwd = GetIniFileString((*keyitem), "StepUserPwd", "", strIniFileName);
	//	strPara.Format("_password%s=%s", strIndex.c_str(), strUserPwd.c_str());
	//	lstOut.AddTail(strPara);
	//}
	//else
	{
		if(GetIniFileSections(keylist, strIniFileName))
		{
			if(keylist.size() <= 1)
			{
				return;
			}
			else
			{
				nTranStep = keylist.size();
			}

			//从ini初始化步列表
			for(keyitem = keylist.begin(); keyitem != keylist.end(); keyitem ++)	
			{	
				strIndex = GetIniFileString((*keyitem), "nIndex", "", strIniFileName);
				//if(strIndex != "0")
				{
					//nTranStep ++;

					strStepName = GetIniFileString((*keyitem), "StepName", "", strIniFileName);
					strPara.Format("_reference%s=%s", strIndex.c_str(), strStepName.c_str());
					lstOut.AddTail(strPara);

					strStepType = GetIniFileString((*keyitem), "StepType", "", strIniFileName);	
					strPara.Format("_referenceType%s=%s", strIndex.c_str(), strStepType.c_str());
					lstOut.AddTail(strPara);

					strPostData = GetIniFileString((*keyitem), "StepPostData", "", strIniFileName);
					strPara.Format("_postData%s=%s", strIndex.c_str(), strPostData.c_str());
					lstOut.AddTail(strPara);

					strContent = GetIniFileString((*keyitem), "StepContent", "", strIniFileName);
					strPara.Format("_content%s=%s", strIndex.c_str(), strContent.c_str());
					lstOut.AddTail(strPara);

					strErrorContent = GetIniFileString((*keyitem), "StepErrorContent", "", strIniFileName);
					strPara.Format("_errorContent%s=%s", strIndex.c_str(), strErrorContent.c_str());
					lstOut.AddTail(strPara);

					strUserName = GetIniFileString((*keyitem), "StepUserName", "", strIniFileName);
					strPara.Format("_username%s=%s", strIndex.c_str(), strUserName.c_str());
					lstOut.AddTail(strPara);

					strUserPwd = GetIniFileString((*keyitem), "StepUserPwd", "", strIniFileName);
					strPara.Format("_password%s=%s", strIndex.c_str(), strUserPwd.c_str());
					lstOut.AddTail(strPara);
				}
			}
		}
	}
}

void GetRunParameters(CStringList& lstIn, CStringList& lstOut, int nCurStep)
{
	CString strPara = _T("");

	strPara.Format("_tranStep=%d", nCurStep);
	lstOut.AddTail(strPara);

	CString strRefereceType = _T("");
	strRefereceType.Format("_referenceType%d=", nCurStep);

	CString strReferece = _T("");
	strReferece.Format("_reference%d=", nCurStep);

	CString strPostData = _T("");
	strPostData.Format("_postData%d=", nCurStep);

	CString strContent = _T("");
	strContent.Format("_content%d=", nCurStep);

	CString strErrorContent = _T("");
	strErrorContent.Format("_errorContent%d=", nCurStep);

	CString strUserName = _T("");
	strUserName.Format("_username%d=", nCurStep);

	CString strPassword = _T("");
	strPassword.Format("_password%d=", nCurStep);

	CString strMsg = _T("");
	CString strTemp= _T("");

	POSITION pos = lstIn.FindIndex(0);
	while(pos)	{
		strTemp = lstIn.GetNext(pos);

		if(strTemp.Find(strRefereceType, 0) == 0) {
			strMsg = strTemp.Right(strTemp.GetLength() - strRefereceType.GetLength());
			strPara.Format("_referenceType%d=%s", nCurStep, strMsg);
			lstOut.AddTail(strPara);
		}
		else if(strTemp.Find(strReferece, 0) == 0) {
			strMsg = strTemp.Right(strTemp.GetLength() - strReferece.GetLength());
			strPara.Format("_reference%d=%s", nCurStep, strMsg);
			lstOut.AddTail(strPara);
		}
		else if(strTemp.Find(strPostData, 0) == 0) {
			strMsg = strTemp.Right(strTemp.GetLength() - strPostData.GetLength());
			strPara.Format("_postData%d=%s", nCurStep, strMsg);
			lstOut.AddTail(strPara);
		}
		else if(strTemp.Find(strContent, 0) == 0) {
			strMsg = strTemp.Right(strTemp.GetLength() - strContent.GetLength());
			strPara.Format("_content%d=%s", nCurStep, strMsg);
			lstOut.AddTail(strPara);
		}
		else if(strTemp.Find(strErrorContent, 0) == 0) {
			strMsg = strTemp.Right(strTemp.GetLength() - strErrorContent.GetLength());
			strPara.Format("_errorContent%d=%s", nCurStep, strMsg);
			lstOut.AddTail(strPara);
		}
		else if(strTemp.Find(strUserName, 0) == 0) {
			strMsg = strTemp.Right(strTemp.GetLength() - strUserName.GetLength());
			strPara.Format("_username%d=%s", nCurStep, strMsg);
			lstOut.AddTail(strPara);
		}
		else if(strTemp.Find(strPassword, 0) == 0) {
			strMsg = strTemp.Right(strTemp.GetLength() - strPassword.GetLength());
			strPara.Format("_password%d=%s", nCurStep, strMsg);
			lstOut.AddTail(strPara);
		}
		else {
		}
	}

	//strPara.Format("_baseUrl=%s", "http://www.sina.com.cn");
	//lstOut.AddTail(strPara);
	
	/////////////////////////////////////////////////////
	// Find BaseUrl
	for (int i = nCurStep-1; i >= 1; i --) {
		strRefereceType.Format("_referenceType%d=", i);
		pos = lstIn.GetHeadPosition();
		while (pos) {
			strTemp = lstIn.GetNext(pos);
			if(strTemp.Find(strRefereceType, 0) == 0) {
				strMsg = strTemp.Right(strTemp.GetLength() - strRefereceType.GetLength());
				if (!stricmp((LPCTSTR)strMsg, "url")) {
					strReferece.Format("_reference%d=", i);
					POSITION pos1 = lstIn.GetHeadPosition();
					while (pos1) {
						strTemp = lstIn.GetNext(pos1);
						if(strTemp.Find(strReferece, 0) == 0) {
							strMsg = strTemp.Right(strTemp.GetLength() - strReferece.GetLength());
							strPara.Format("_baseUrl=%s", strMsg);
							lstOut.AddTail(strPara);
							break;
						}
					}
					break;
				}
			}
		}		
	}
	/////////////////////////////////////////////////////
}

//extern "C" __declspec(dllexport) BOOL UrlTransaction(CStringList &paramList, char *szReturn)
extern "C" __declspec(dllexport) BOOL UrlTransaction(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	BOOL bRet = FALSE;

	CString strPath = ::FuncGetInstallPath();
	if (strPath.IsEmpty())
		return bRet;

	CString	strProxyServer = _T(""),		// HTTP Proxy
			strProxyUsername = _T(""),		// Proxy Server User Name
			strProxyPassword = _T(""),  	// Proxy Server Password
			strMonitorId =  _T("");		

	int		nTimeout = 60, 
			nTranStep = 0,
			nLimitStep = 0;
	CStringList paramList;

	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);
	while(pos)	{
		CString strTemp = paramList.GetNext(pos);		
		
		if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strProxyServer = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
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
		if(strTemp.Find(__MONITORID__, 0) == 0)
		{
			strMonitorId = strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__));
		}
		else if(strTemp.Find(__TRANSTEP__, 0) == 0)//不应该注释了， 但和GetRunParametersByMid怎么共存？
		{
			nTranStep = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TRANSTEP__)));
		}
		else if(strTemp.Find(__LIMITEDSTEP__, 0) == 0)
		{
			nLimitStep = atoi(strTemp.Right(strTemp.GetLength() - strlen(__LIMITEDSTEP__)));
		}
		else
		{
			
		}
	}

	GetRunParametersByMid(strMonitorId, paramList, nTranStep);

	float fTotalTime = 0.0;

	HINSTANCE hInstance = ::LoadLibrary("Curl.dll");
	if (hInstance) {
		MONITOR_CURL * pGatherData = (MONITOR_CURL*)::GetProcAddress(hInstance, "MONITOR_CURL");
		if (pGatherData) {
			CString strPara = _T("");
			CStringList lstStepParas;
			BOOL bRet1 = FALSE;

			for (int i = 1; i <= nTranStep; i++) {
				lstStepParas.RemoveAll();
				
				strPara.Format("_TimeOut=%d", nTimeout);
				lstStepParas.AddTail(strPara);
				
				strPara.Format("_ProxyServerPort=%s", strProxyServer);
				lstStepParas.AddTail(strPara);
				
				strPara.Format("_ProxyUser=%s", strProxyUsername);
				lstStepParas.AddTail(strPara);
				
				strPara.Format("_ProxyPass=%s", strProxyPassword);
				lstStepParas.AddTail(strPara);
				
				GetRunParameters(paramList, lstStepParas, i);

				char buf[1024] = {0};
				
				DebugParasInfo(lstStepParas);

				bRet1 = (*pGatherData)(strMonitorId, lstStepParas, buf, false );

				OutputDebugString("urltrans : ");
				OutputDebugString(buf);
				if (bRet1) {
					OutputDebugString("cur return true ");
					if (strstr(buf, "error"))
					{
						strcpy(szReturn, buf);
						bRet = FALSE;
					
					}
					else {
						float fValue = 0.0;
						strPara.Format("step%dtotaltime=", i);
						GetTimeFromString(strPara, buf, fValue);
						fTotalTime += fValue;
						strcat(szReturn, buf);
						bRet = TRUE;
					}					
				}
				else {
					strcpy(szReturn, "error=Connect Server Error!$");
					bRet = FALSE;
					break;	
					
				}
			}			
		}
		::FreeLibrary(hInstance);
	}

	if (bRet) 
	{
		CString strTemp = _T("");
		strTemp.Format("transtotaltime=%f$", fTotalTime);
		strcat(szReturn, strTemp);

		for (int i = nTranStep + 1; i <= nLimitStep; i++)
		{
			CString strTemp = _T("");
			strTemp.Format("step%dtotaltime=%f$", i, 0);
			strcat(szReturn, strTemp);		
		}
	}
	
	CString strInput;
	strInput =szReturn;
	MakeCharByString(szReturn,nSize,strInput);

	return TRUE;
}

//根据步等　--> monotorid + PaserStep + nCurStep.Info、monotorid + Header + nCurStep.Info、monotorid + Step + nCurStep.html等
extern "C" __declspec(dllexport) BOOL UrlTransactionByStep(const char * strParas, char * szReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	WriteLog( "============== UrlTransactionByStep ===============" );
	const char* pszParas = strParas;
	while(*pszParas)
	{
		WriteLog( pszParas);
		pszParas += strlen(pszParas) + 1;
	}

	BOOL bRet = FALSE;

	CString strPath = ::FuncGetInstallPath();
	if (strPath.IsEmpty())
		return bRet;

	CString	strProxyServer = _T(""),		// HTTP Proxy
			strProxyUsername = _T(""),		// Proxy Server User Name
			strProxyPassword = _T(""),  	// Proxy Server Password
			strMonitorId =  _T("");
	int		nTimeout = 60, 
			nTranStep = 0;
	CStringList paramList;

	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);
	while(pos)	{
		CString strTemp = paramList.GetNext(pos);		
		
		if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strProxyServer = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
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
		else if(strTemp.Find(__MONITORID__, 0) == 0)
		{
			strMonitorId = strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__));
		}
		else if(strTemp.Find(__TRANSTEP__, 0) == 0)
		{
			nTranStep = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TRANSTEP__)));
		}
		else
		{
			
		}
	}

	float fTotalTime = 0.0;

	HINSTANCE hInstance = ::LoadLibrary("Curl.dll");
	if (hInstance) {
		MONITOR_CURL * pGatherData = (MONITOR_CURL*)::GetProcAddress(hInstance, "MONITOR_CURL");
		if (pGatherData) {
			CString strPara = _T("");
			CStringList lstStepParas;
			BOOL bRet1 = FALSE;

			//for (int i = 1; i <= nTranStep; i++) {
				lstStepParas.RemoveAll();
				
				strPara.Format("_TimeOut=%d", nTimeout);
				lstStepParas.AddTail(strPara);
				
				strPara.Format("_ProxyServerPort=%s", strProxyServer);
				lstStepParas.AddTail(strPara);
				
				strPara.Format("_ProxyUser=%s", strProxyUsername);
				lstStepParas.AddTail(strPara);
				
				strPara.Format("_ProxyPass=%s", strProxyPassword);
				lstStepParas.AddTail(strPara);
				
				GetRunParameters(paramList, lstStepParas, nTranStep);

				char buf[1024] = {0};
				
				DebugParasInfo(lstStepParas);

				bRet1 = (*pGatherData)(strMonitorId, lstStepParas, buf, true);

				if (bRet1) {
					if (strstr(buf, "error")) { 
						strcpy(szReturn, buf);
						bRet = FALSE;
						//break;
					}
					else {
						float fValue = 0.0;
						strPara.Format("step%dtotaltime=", nTranStep);
						GetTimeFromString(strPara, buf, fValue);
						fTotalTime += fValue;
						strcat(szReturn, buf);
						bRet = TRUE;
					}					
				}
				else {
					strcpy(szReturn, "error=Failed Call DLL$");
					bRet = FALSE;
					//break;
				}
			}			
		//}
		::FreeLibrary(hInstance);
	}

	if (bRet) {
		CString strTemp = _T("");
		strTemp.Format("transtotaltime=%f$", fTotalTime);
		strcat(szReturn, strTemp);
	}

	WriteLog("结果：");
	WriteLog( szReturn );
	
	CString strInput;
	strInput =szReturn;
	MakeCharByString(szReturn,nSize,strInput);

	return TRUE;
}


//根据步等　-> monotorid + PaserStep + nCurStep.Info　-> lstLinks 等
extern "C" __declspec(dllexport) BOOL UrlPaserStepList(const char * strParas, list<string >& lstLinks,
	list<string >& lstForms, list<string >& lstPostDatas, string& strBaseUrl)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	WriteLog("============= UrlPaserStepList ==============");

	BOOL bRet = FALSE;

	CString strPath = ::FuncGetInstallPath();
	CString strMonitorId =  _T("");

	if (strPath.IsEmpty())
		return bRet;

	int		nTimeout = 60, 
			nTranStep = 0;

	CStringList paramList;

	MakeStringListByChar(paramList,strParas);

	POSITION pos = paramList.FindIndex(0);
	while(pos)	{
		CString strTemp = paramList.GetNext(pos);				
		
		if(strTemp.Find(__MONITORID__, 0) == 0)
		{
			strMonitorId = strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__));
		}
		else if(strTemp.Find(__TRANSTEP__, 0) == 0)
		{			
			nTranStep = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TRANSTEP__)));
		}
		else
		{
			
		}
	}


	//CString strPaserStepFile = "";
	CString strPaserStepFile = "";
	strPaserStepFile.Format("%s\\cgi-bin\\%s_PaserStep%d.Info", GetSiteViewRootPath().c_str(), strMonitorId, nTranStep);
	//strPaserStepFile.Format("C:\\SiteView\\SiteView ECC\\cgi-bin\\%s_PaserStep%d.Info", strMonitorId, nTranStep);

	WriteLog(strPaserStepFile.GetBuffer());

	CString strFormName = _T("");
	
	WriteLog("links");
	CString strSection = "Links";
	CString strKey = "";
	CString strEachText = _T("");
	int nCount = FuncGetProfileIntBy(strSection, "_linkNum", strPaserStepFile);
	//int nCount = GetPrivateProfileInt( strSection.GetBuffer(), "_linkNum", 0, strPaserStepFile.GetBuffer() );
	for(int i = 1; i <= nCount; i++)
	{
		strEachText = _T("");
		strKey.Format("_text%d", i);
		//strKey.Format("_herf%d", i);
		strEachText = FuncGetProfileStringBy(strSection, strKey, strPaserStepFile); 
		string strTmp(strEachText.GetBuffer(strEachText.GetLength()));
		//char szEachText[1024] = {0};
		//GetPrivateProfileString( strSection.GetBuffer(), strKey.GetBuffer(), "", szEachText, sizeof(szEachText)-1, strPaserStepFile.GetBuffer() );
		//string strTmp = szEachText;

		WriteLog(strTmp.c_str());

		lstLinks.push_back(strTmp);
	}

	//Forms 	
	strSection = "Forms";
	strKey = "";
	strEachText = _T("");
	nCount = FuncGetProfileIntBy(strSection, "_formNum", strPaserStepFile);
	for(int i = 1; i <= nCount; i++)
	{
		strEachText = _T("");
		strKey.Format("_text%d", i);
		//strKey.Format("_herf%d", i);
		strEachText = FuncGetProfileStringBy(strSection, strKey, strPaserStepFile); 
		string strTmp(strEachText.GetBuffer(strEachText.GetLength()));

		lstForms.push_back(strTmp);

		//Form's postDatas
		CString strEachPostData = _T("");
		
		//Get _postdataNum + i 
		int nPostDataCount = 0;
		strKey.Format("_postdataNum%d", i);
		nPostDataCount = FuncGetProfileIntBy(strSection, strKey, strPaserStepFile);

		//从_postdata1_1->postData
		for(int j = 1; j <= nPostDataCount; j++)
		{
			//Get _postdata + i_j
			strKey.Format("_postdata%d_%d", i, j);
			CString strEachValue = _T("");
			strEachValue = FuncGetProfileStringBy(strSection, strKey, strPaserStepFile);

			//Make PostData
			strEachPostData += strEachValue;
			strEachPostData += "&";
		}
		
		string strTmpPost(strEachPostData.GetBuffer(strEachPostData.GetLength()));
		lstPostDatas.push_back(strTmpPost);
	}

	//BaseUrl
	CString csBaseUrl = FuncGetProfileStringBy("BaseUrl", "_url", strPaserStepFile);
	string strTmpUrl(csBaseUrl.GetBuffer(csBaseUrl.GetLength()));
	strBaseUrl = strTmpUrl.c_str();

	//Frames Refreshs ...

	return TRUE;
}

///use  inet 

/*
extern "C" __declspec(dllexport) BOOL UrlTransaction(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	BOOL bRet = FALSE;

	CString strPath = ::FuncGetInstallPath();
	if (strPath.IsEmpty())
		return bRet;

	CString	strProxyServer = _T(""),		// HTTP Proxy
			strProxyUsername = _T(""),		// Proxy Server User Name
			strProxyPassword = _T("");	// Proxy Server Password
			
	int		nTimeout = 60, 
			nGID = 0, 
			nMID = 0,
			nTranStep = 0;

	POSITION pos = paramList.FindIndex(0);
	while(pos)	{
		CString strTemp = paramList.GetNext(pos);		
		
		if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strProxyServer = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
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
		else if(strTemp.Find(__GROUPID__, 0) == 0)
		{
			nGID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__GROUPID__)));
		}
		else if(strTemp.Find(__MONITORID__, 0) == 0)
		{
			nMID = atoi(strTemp.Right(strTemp.GetLength() - strlen(__MONITORID__)));
		}
		else if(strTemp.Find(__TRANSTEP__, 0) == 0)
		{
			nTranStep = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TRANSTEP__)));
		}
		else
		{
		}
	}

	float fTotalTime = 0.0;

	HINSTANCE hInstance = ::LoadLibrary("Urlcurl.dll");
	if (hInstance) {
		MONITOR_CURL * pGatherData = (MONITOR_CURL*)::GetProcAddress(hInstance, "MONITOR_CURL");
		if (pGatherData) {
			CString strPara = _T("");
			CStringList lstStepParas;
			BOOL bRet1 = FALSE;

			for (int i = 1; i <= nTranStep; i++) {
				lstStepParas.RemoveAll();
				
				strPara.Format("_TimeOut=%d", nTimeout);
				lstStepParas.AddTail(strPara);
				
				strPara.Format("_ProxyServerPort=%s", strProxyServer);
				lstStepParas.AddTail(strPara);
				
				strPara.Format("_ProxyUser=%s", strProxyUsername);
				lstStepParas.AddTail(strPara);
				
				strPara.Format("_ProxyPass=%s", strProxyPassword);
				lstStepParas.AddTail(strPara);
				
				GetRunParameters(paramList, lstStepParas, i);

				char buf[1024] = {0};
				
				DebugParasInfo(lstStepParas);

				bRet1 = (*pGatherData)(nGID, nMID, lstStepParas, buf);

				if (bRet1) {					 
					if (strstr(buf, "error")) { 
						strcpy(szReturn, buf);
						bRet = FALSE;
						break;
					}
					else {
						float fValue = 0.0;
						strPara.Format("step%dtotaltime=", i);
						GetTimeFromString(strPara, buf, fValue);
						fTotalTime += fValue;
						strcat(szReturn, buf);
						bRet = TRUE;
					}					
				}
				else {
					strcpy(szReturn, "error=Failed Call DLL$");
					bRet = FALSE;
					break;
				}
			}			
		}
		::FreeLibrary(hInstance);
	}

	if (bRet) {
		CString strTemp = _T("");
		strTemp.Format("transtotaltime=%f$", fTotalTime);
		strcat(szReturn, strTemp);
	}

	return TRUE;
}
*/