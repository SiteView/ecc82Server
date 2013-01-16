
#include "HttpMonitor.h"
#include "Utils.h"
#include "Base64.h"

#include "..\..\base\funcGeneral.h"

#define		C_STA				"DF-CMD-STA"
#define		C_END				"DF-CMD-END"
#define		READ_TIMEOUT		60

BOOL http_command(char *url, char *cmd, int timeout, char *uid, char *pwd, 
				  char *pserver, int pport, char *puid, char *ppwd, char *szReturn)
{
	BOOL bResult = TRUE;
	CString strCmdInfo = _T("");
	CString	strRemoteUrl = _T("");
	CInternetSession *psession = NULL;
	CHttpConnection* pServer = NULL;
	CHttpFile* pFile = NULL;

	if(*pserver)
	{
		char	pbuff[256] = {0};
		sprintf(pbuff, "%s:%d", pserver, pport);
		psession = new CInternetSession("WinInet", 1, 
								INTERNET_OPEN_TYPE_PROXY, pbuff, NULL, 0);
	}
	else
	{
		psession = new CInternetSession("WinInet");
	}

	try
	{
		CString strServerName;
		CString strObject;
		INTERNET_PORT nPort;
		DWORD dwServiceType;

		strCmdInfo.Format("echo %s;%s;echo %s", C_STA, cmd, C_END);
		strCmdInfo.Replace(' ', '+');
		strRemoteUrl.Format("%s?%s", url, strCmdInfo);

		if (!AfxParseURL(strRemoteUrl, dwServiceType, strServerName, strObject, nPort) ||
			dwServiceType != INTERNET_SERVICE_HTTP)
		{
			sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("<%IDS_Utils_1%>"));//<%IDS_Utils_1%>
			return FALSE;
		}

		pServer = psession->GetHttpConnection(strServerName, nPort);

		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
			strObject, NULL, 1, NULL, NULL, INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT);
		if(*puid)
		{
			CString	User_Pass = _T("");
			User_Pass.Format("%s:%s", puid, ppwd);

			CString	strOutput = _T("Proxy-Authorization: Basic ");

			CBase64	*pBase64 = new CBase64();
			strOutput.Insert(strOutput.GetLength(), pBase64->Encode(User_Pass, User_Pass.GetLength()));
			delete pBase64;
			pBase64 = NULL;
			
			pFile->AddRequestHeaders(strOutput);
		}

		pFile->SendRequest();

		DWORD dwRet;
		pFile->QueryInfoStatusCode(dwRet);
		if(dwRet == HTTP_STATUS_DENIED)
		{
			CString	User_Pass = _T("");
			User_Pass.Insert(User_Pass.GetLength(), uid);
			User_Pass.Insert(User_Pass.GetLength(), ':');
			User_Pass.Insert(User_Pass.GetLength(), pwd);

			CString	strOutput = _T("Authorization: Basic ");

			CBase64	*pBase64 = new CBase64();
			strOutput.Insert(strOutput.GetLength(), pBase64->Encode(User_Pass, User_Pass.GetLength()));
			delete pBase64;
			pBase64 = NULL;
			
			pFile->AddRequestHeaders(strOutput);
			pFile->SendRequest();
			pFile->QueryInfoStatusCode(dwRet);
		}

		if(dwRet == 200)
		{
			while(1)
			{
				char *ca = NULL, *cb = NULL;
				char buf[8192] = {0};
				int n = pFile->Read(buf, sizeof(buf));
				if(n == 0) break;
				buf[n] = 0;
				strncat(szReturn, buf, n);

				if(ca = strstr(szReturn, C_STA)) 
				{
					if(cb = strstr(szReturn, C_END))
					{
						ca += strlen(C_STA);
						while(*ca == 0x0A || *ca == 0x0D) ca ++;
						while(*cb == 0x0A || *cb == 0x0D) cb ++;
						strncpy(szReturn, ca, cb - ca);
						szReturn[cb - ca] = 0;
						break;
					}
				}
			}
		}
		else
		{
			sprintf(szReturn, "%ld", dwRet);
			bResult = FALSE;
		}

		if(pFile) pFile->Close();
		if(pServer) pServer->Close();
	}
	catch (CInternetException* pEx)
	{
		TCHAR szErr[1024];
		pEx->GetErrorMessage(szErr, 1024);

		sprintf(szReturn, "%s", szErr);
		
		bResult = FALSE;

		pEx->Delete();
	}

	if (pFile != NULL)
		delete pFile;
	if (pServer != NULL)
		delete pServer;
	psession->Close();
	delete psession;

	return bResult;
}

BOOL HttpMonitorValue(char *url, char *uid, char *pwd, char *pserver, int pport, 
					  char *puid, char *ppwd, char *inifile, int mtype, 
					  char *extraparam, char *custpath, char *szReturn)
{
	int		ret = 0;
	BOOL	bResult = TRUE;
	char	iniFile[256] = {0};
	char	cmd[256] = {0};
	char	*ca = NULL;

	GetIniFileName(inifile, iniFile);
	
	if(!RETURN_COMMAND(mtype, iniFile, cmd))
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("<%IDS_TelMonitor_14%>"));    //  <%IDS_TelMonitor_14%>
		return FALSE;
	}

	if(ca = strchr(cmd, '<')) 
	{
		strcpy(ca, extraparam);
		*(ca + strlen(extraparam)) = 0;
	}

	char msg[100 * 1024] = {0};
	if(!http_command(url, cmd, READ_TIMEOUT, uid, pwd, pserver, pport, puid, ppwd, msg))
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("<%IDS_TelMonitor_15%>"));    //  <%IDS_TelMonitor_15%>
		return FALSE;
	}

	if(!MONITOR_PARSE(mtype, msg, inifile, extraparam, szReturn))
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));   //  <%IDS_TelMonitor_16%>
		return FALSE;
	}

	return TRUE;
}

BOOL HttpUnixPerfMonitorValue(char *url, char *uid, char *pwd, char *pserver, int pport, 
			char *puid, char *ppwd, char *inifile, char *custpath, char *szReturn)
{
	int		ret = 0;
	BOOL	bResult = TRUE;
	char	iniFile[256] = {0};
	char	cmd[1024] = {0};
	char	*ca = NULL;

	GetIniFileName(inifile, iniFile);
	
	if(!RETURN_COMMAND(0, iniFile, cmd))
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("<%IDS_TelMonitor_14%>"));    //  <%IDS_TelMonitor_14%>
		return FALSE;
	}

	char msg[100 * 1024] = {0};
	if(!http_command(url, cmd, READ_TIMEOUT, uid, pwd, pserver, pport, puid, ppwd, msg))
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("<%IDS_TelMonitor_15%>"));    //  <%IDS_TelMonitor_15%>
		return FALSE;
	}

	if(!MONITOR_PARSE(0, msg, inifile, NULL, szReturn))
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("<%IDS_TelMonitor_16%>"));   //  <%IDS_TelMonitor_16%>
		return FALSE;
	}

	return TRUE;
}
 
							  
							  
