
#include "URL.h"

#include <Base\funcGeneral.h>

CString	strContent = _T("");

BOOL URL_MONITOR(char *url, char *match, char *post, int timeout, char *uid, char *pwd, 
				 char *pserver, int pport, char *puid, char *ppwd, 
				 int ngid, int nmid, char *custpath, char *szReturn)
{
	CString	strCmdLine = _T(""), 
			strResult = _T("");
	char	tmp[256] = {0};
	int		index = 0;

	strCmdLine.Format("%s\\cgi-bin\\%s %s", FuncGetInstallPath(), CURL_PATH, CURL_FORMAT);

	strCmdLine += " \"--connect-timeout\" \"";
	strCmdLine += _itoa(timeout, tmp, 10);
	strCmdLine += "\"";

	if(*post)
	{
		strCmdLine += " \"-d\" \"";
		strCmdLine += post;
		strCmdLine += "\"";
	}

	if(*uid)
	{
		strCmdLine += " \"-u\" \"";
		strCmdLine += uid;
		strCmdLine += ":";
		strCmdLine += pwd;
		strCmdLine += "\"";
	}

	if(*pserver)
	{
		strCmdLine += " \"-x\" \"";
		strCmdLine += pserver;
		strCmdLine += ":";
		strCmdLine += _itoa(pport, tmp, 10);
		strCmdLine += "\"";
	}

	if(*puid)
	{
		strCmdLine += " \"-U\" \"";
		strCmdLine += puid;
		strCmdLine += ":";
		strCmdLine += ppwd;
		strCmdLine += "\"";
	}

	strCmdLine += " \"";
	strCmdLine += url;
	strCmdLine += "\"";

	//ExecuteFile(strCmdLine);
	ExecuteFile(strCmdLine, strContent, ngid, nmid);

	CString strCode = _T("");
//	strContent.Replace("CURL Terminate.", "");
	if(!CODE_VALUE(strContent, strCode))
	{
		sprintf(szReturn, "error=%s", strCode);
		return FALSE;
	}

	if(*match && strContent.Find(match, 0) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_URL_1%>"));//<%IDS_URL_1%>
		return FALSE;
	}

	long	status_code = 0, 
			download_size = 0, 
			upload_size = 0, 
			header_size = 0, 
			request_size = 0;
	double	total_time = 0.0, 
			dns_time = 0.0, 
			connect_time = 0.0, 
			pretransfer_time = 0.0, 
			start_transfer_time = 0.0, 
			download_speed = 0.0, 
			upload_speed = 0.0;
	
	char	*ca = (char *)(LPCTSTR)strContent, *cb = NULL;
	if(cb = strstr(ca, STATUS_CODE_MATCH))
		status_code = atol(cb + strlen(STATUS_CODE_MATCH));
	if(cb = strstr(ca, TOTAL_TIME_MATCH))
		total_time = atof(cb + strlen(TOTAL_TIME_MATCH));
	if(cb = strstr(ca, DNS_TIME_MATCH))
		dns_time = atof(cb + strlen(DNS_TIME_MATCH));
	if(cb = strstr(ca, CONNECT_TIME_MATCH))
		connect_time = atof(cb + strlen(CONNECT_TIME_MATCH));
	if(cb = strstr(ca, PRETRANSFER_TIME_MATCH))
		pretransfer_time = atof(cb + strlen(PRETRANSFER_TIME_MATCH));
	if(cb = strstr(ca, START_TRANSFER_TIME_MATCH))
		start_transfer_time = atof(cb + strlen(START_TRANSFER_TIME_MATCH));
	if(cb = strstr(ca, DOWNLOAD_SIZE_MATCH))
		download_size = atol(cb + strlen(DOWNLOAD_SIZE_MATCH));
	if(cb = strstr(ca, UPLOAD_SIZE_MATCH))
		upload_size = atol(cb + strlen(UPLOAD_SIZE_MATCH));
	if(cb = strstr(ca, HEADER_SIZE_MATCH))
		header_size = atol(cb + strlen(HEADER_SIZE_MATCH));
	if(cb = strstr(ca, REQUEST_SIZE_MATCH))
		request_size = atoi(cb + strlen(REQUEST_SIZE_MATCH));
	if(cb = strstr(ca, DOWNLOAD_SPEED_MATCH))
		download_speed = atof(cb + strlen(DOWNLOAD_SPEED_MATCH));
	if(cb = strstr(ca, UPLOAD_SPEED_MATCH))
		upload_speed = atof(cb + strlen(UPLOAD_SPEED_MATCH));

	sprintf(szReturn, "status=%ld$totaltime=%.3f$dnstime=%.3f$connecttime=%.3f$pretransfertime=%.3f$starttransfertime=%.3f$downloadsize=%ld$uploadsize=%ld$headersize=%ld$requestsize=%ld$downloadspeed=%.3f$uploadspeed=%.3f$", 
					status_code, total_time, dns_time, connect_time, pretransfer_time, 
					start_transfer_time, download_size, upload_size, header_size, 
					request_size, download_speed, upload_size);

	return TRUE;
}

/*
void ExecuteFile(CString strCmdLine, CString &strContent, int ngid, int nmid)
{
	char tmpfile[64] = {0};
	SHELLEXECUTEINFO ShExecInfo;

	memset(&ShExecInfo, 0, sizeof(SHELLEXECUTEINFO));
    ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    ShExecInfo.fMask = NULL;
    ShExecInfo.hwnd = NULL;
    ShExecInfo.lpVerb = NULL;
    ShExecInfo.lpFile = CURL_PATH;
    ShExecInfo.lpParameters = "> curl_tmp_file_00";
    ShExecInfo.lpDirectory = NULL;
    ShExecInfo.nShow = SW_MAXIMIZE;
    ShExecInfo.hInstApp = NULL;

    BOOL bResult = ShellExecuteEx(&ShExecInfo);

	char buffer[8192] = {0};
	DWORD bytesRead = 0;

	DWORD dwResult = WaitForSingleObject(ShExecInfo.hProcess, CURL_TIMEOUT);
	switch(dwResult)
	{
	case WAIT_OBJECT_0:
		HANDLE hFile;
		hFile = CreateFile(tmpfile, GENERIC_READ, 0, NULL, 
							OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile == INVALID_HANDLE_VALUE) 
			break;

		do
		{
			if (ReadFile(hFile, buffer, 8192, &bytesRead, NULL)) 
			{
				buffer[bytesRead] = 0;
				strContent += buffer;
			}
		}while(bytesRead == 8192);

		TerminateProcess(ShExecInfo.hProcess, 1);
		break;
	case WAIT_TIMEOUT:
	case WAIT_ABANDONED:
//		HANDLE hProcess;
//		hProcess = OpenProcess (PROCESS_ALL_ACCESS, FALSE, pi.dwProcessId);
//		if (hProcess)
//			BOOL bRet = TerminateProcess(hProcess, 1);
			//TerminateProcess(pi.hProcess, 1);
		break;
	}

}
*/

void ExecuteFile(CString strCmdLine, CString &strContent, int ngid, int nmid)
{
	SECURITY_ATTRIBUTES sa;
	char tmpfile[64] = {0};
	int nOldOutput;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	nOldOutput = _dup( 1 );

	sprintf(tmpfile, "%s\\cgi-bin\\curl_tmp_file_%d%d", FuncGetInstallPath(), ngid, nmid);
	FILE *fp = freopen(tmpfile, "w", stdout);
	if(fp == NULL)
		return;

	HANDLE hOutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hOutHandle == NULL)
		return;

	STARTUPINFO si;
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdOutput = hOutHandle;
	si.hStdError = hOutHandle;
	
	PROCESS_INFORMATION pi;
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));
 
	if (CreateProcess(NULL, strCmdLine.GetBuffer(strCmdLine.GetLength()),  NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) 
	{
		char buffer[8192] = {0};
		DWORD bytesRead = 0;

		DWORD dwResult = WaitForSingleObject(pi.hProcess, CURL_TIMEOUT);
		switch(dwResult)
		{
		case WAIT_OBJECT_0:
			HANDLE hFile;

			fclose(fp);

			hFile = CreateFile(tmpfile, GENERIC_READ, 0, NULL, 
								OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hFile == INVALID_HANDLE_VALUE) 
				break;

			do
			{
				if (ReadFile(hFile, buffer, 8192, &bytesRead, NULL)) 
				{
					buffer[bytesRead] = 0;
					strContent += buffer;
				}
			}while(bytesRead == 8192);

			TerminateProcess(pi.hProcess, 1);
			break;
		case WAIT_TIMEOUT:
		case WAIT_ABANDONED:
			HANDLE hProcess;
			hProcess = OpenProcess (PROCESS_ALL_ACCESS, FALSE, pi.dwProcessId);
			if (hProcess)
				BOOL bRet = TerminateProcess(hProcess, 1);
				//TerminateProcess(pi.hProcess, 1);
			break;
		}
	}

	if(nOldOutput != -1) _dup2(nOldOutput, 1);

	CloseHandle(hOutHandle);
}

/*
void ExecuteFile(CString strCmdLine, CString &strContent)
{
	HANDLE hOutputRead, hOutputWrite;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	if (CreatePipe(&hOutputRead, &hOutputWrite, &sa, 0)) {
		STARTUPINFO si;
		memset(&si, 0, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdOutput = hOutputWrite;
		si.hStdError = hOutputWrite;
		
		PROCESS_INFORMATION pi;
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));

		if (CreateProcess(NULL, strCmdLine.GetBuffer(strCmdLine.GetLength()),  NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
			char buffer[8192] = {0};
			DWORD bytesRead = 0;

			while(1)
			{
				int index = 0;
				if(ReadFile(hOutputRead, buffer, sizeof(buffer), &bytesRead, NULL))
				{
					buffer[bytesRead] = 0;
					strContent += buffer;
					index = strContent.Find("CURL Terminate.", 0);
					if(index >= 0)
					{
						TerminateProcess(pi.hProcess, 1);
						break;
					}
				}
				else
				{
					TerminateProcess(pi.hProcess, 1);
					break;
				}
			}

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}

		CloseHandle(hOutputRead);
		CloseHandle(hOutputWrite);
	}

}
*/
/*
void ExecuteFile(CString strCmdLine)
{
	DWORD dwID = GetCurrentThreadId();

	// Start Timeout Thread
	CWinThread* pTimeoutThread = AfxBeginThread(CurlThreadProc,(LPVOID)&dwID,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED,NULL);
	if(pTimeoutThread == NULL)
	{
		strContent = _T("");
		return;
	}

	pTimeoutThread->m_bAutoDelete= TRUE;
	pTimeoutThread->ResumeThread();
	Sleep(0);

	// Start Timeout Thread
	struct tagCurlParam curlParam;
	curlParam.dwThreadID = dwID;
	curlParam.strCmdLine = strCmdLine;
	CWinThread* pWorkThread = AfxBeginThread(CurlThreadProc2,(LPVOID)&curlParam,THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED,NULL);
	if(pWorkThread == NULL)
	{
		strContent = _T("");
		return;
	}

	pWorkThread->m_bAutoDelete= TRUE;
	pWorkThread->ResumeThread();
	Sleep(0);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (WM_USER_CURLTIMEOUT == msg.message && NULL == msg.hwnd)
		{
			TerminateProcess(pWorkThread, 1);
			break;
		}
		else if (WM_USER_CURLOK == msg.message && NULL == msg.hwnd)
		{
			TerminateProcess(pTimeoutThread, 1);
			break;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}
*/
UINT CurlThreadProc( LPVOID pParam )
{
	DWORD dwThreadID = *((DWORD *)pParam);

	Sleep(CURL_TIMEOUT);

	if(!PostThreadMessage(dwThreadID , WM_USER_CURLTIMEOUT, (WPARAM)0, (LPARAM)0))
		ExitProcess(1);

	return 0;
}

UINT CurlThreadProc2( LPVOID pParam )
{
	struct tagCurlParam *pCurlParam = (struct tagCurlParam *)pParam;

	HANDLE hOutputRead, hOutputWrite;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	if (CreatePipe(&hOutputRead, &hOutputWrite, &sa, 0)) {
		STARTUPINFO si;
		memset(&si, 0, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdOutput = hOutputWrite;
		si.hStdError = hOutputWrite;
		
		PROCESS_INFORMATION pi;
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));

		if (CreateProcess(NULL, pCurlParam->strCmdLine.GetBuffer(pCurlParam->strCmdLine.GetLength()),  NULL, NULL, TRUE, /*CREATE_NEW_CONSOLE*/CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
			char buffer[8192] = {0};
			DWORD bytesRead = 0;

			while(1)
			{
				if(ReadFile(hOutputRead, buffer, sizeof(buffer), &bytesRead, NULL))
				{
					buffer[bytesRead] = 0;
					strContent += buffer;
					int index = strContent.Find("CURL Terminate.", 0);
					if(index >= 0)
					{
						TerminateProcess(pi.hProcess, 1);
						break;
					}
				}
				else
				{
					TerminateProcess(pi.hProcess, 1);
					break;
				}
			}

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}

		CloseHandle(hOutputRead);
		CloseHandle(hOutputWrite);
	}

	if(!PostThreadMessage(pCurlParam->dwThreadID, WM_USER_CURLOK, (WPARAM)0, (LPARAM)0))
		ExitProcess(1);

	return 0;
}

BOOL CODE_VALUE(CString strResult, CString &strCode)
{
	if(strResult.Find("curl: (1) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_2%>");//<%IDS_URL_2%>
		return FALSE;
	}
	if(strResult.Find("curl: (2) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_3%>");//<%IDS_URL_3%>
		return FALSE;
	}
	if(strResult.Find("curl: (3) ", 0) >= 0)
	{
		strCode =FuncGetStringFromIDS("<%IDS_URL_4%>");//<%IDS_URL_4%>
		return FALSE;
	}
	if(strResult.Find("curl: (4) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_5%>");//<%IDS_URL_5%>
		return FALSE;
	}
	else if(strResult.Find("curl: (5) ", 0) >= 0)
	{
		strCode =FuncGetStringFromIDS("<%IDS_URL_6%>");//<%IDS_URL_6%>
		return FALSE;
	}
	else if(strResult.Find("curl: (6) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_7%>");//<%IDS_URL_7%>
		return FALSE;
	}
	else if(strResult.Find("curl: (7) ", 0) >= 0)
	{
		strCode =FuncGetStringFromIDS("<%IDS_URL_8%>");//<%IDS_URL_8%>
		return FALSE;
	}
	else if(strResult.Find("curl: (8) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_9%>");//<%IDS_URL_9%>
		return FALSE;
	}
	else if(strResult.Find("curl: (9) ", 0) >= 0)
	{
		strCode =FuncGetStringFromIDS("<%IDS_URL_10%>");//<%IDS_URL_10%>
		return FALSE;
	}
	else if(strResult.Find("curl: (10) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_11%>");//<%IDS_URL_11%>
		return FALSE;
	}
	else if(strResult.Find("curl: (11) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_12%>");//<%IDS_URL_12%>
		return FALSE;
	}
	else if(strResult.Find("curl: (12) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_13%>");//<%IDS_URL_13%>
		return FALSE;
	}
	else if(strResult.Find("curl: (13) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_14%>");//<%IDS_URL_14%>
		return FALSE;
	}
	else if(strResult.Find("curl: (14) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_15%>");//<%IDS_URL_15%>
		return FALSE;
	}
	else if(strResult.Find("curl: (15) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_16%>");//<%IDS_URL_16%>
		return FALSE;
	}
	else if(strResult.Find("curl: (16) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_17%>");//<%IDS_URL_17%>
		return FALSE;
	}
	else if(strResult.Find("curl: (17) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_18%>");//<%IDS_URL_18%>
		return FALSE;
	}
	else if(strResult.Find("curl: (18) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_19%>");//<%IDS_URL_19%>
		return FALSE;
	}
	else if(strResult.Find("curl: (19) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_20%>");//<%IDS_URL_20%>
		return FALSE;
	}
	else if(strResult.Find("curl: (20) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_21%>");//<%IDS_URL_21%>
		return FALSE;
	}
	else if(strResult.Find("curl: (21) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_22%>");//<%IDS_URL_22%>
		return FALSE;
	}
	else if(strResult.Find("curl: (22) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_23%>");//<%IDS_URL_23%>
		return FALSE;
	}
	else if(strResult.Find("curl: (23) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_24%>");//<%IDS_URL_24%>
		return FALSE;
	}
	else if(strResult.Find("curl: (24) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_25%>");//<%IDS_URL_25%>
		return FALSE;
	}
	else if(strResult.Find("curl: (25) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_26%>");//<%IDS_URL_26%>
		return FALSE;
	}
	else if(strResult.Find("curl: (26) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_27%>");//<%IDS_URL_27%>
		return FALSE;
	}
	else if(strResult.Find("curl: (27) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_28%>");//<%IDS_URL_28%>
		return FALSE;
	}
	else if(strResult.Find("curl: (28) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_29%>");//<%IDS_URL_29%>
		return FALSE;
	}
	else if(strResult.Find("curl: (29) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_30%>");//<%IDS_URL_30%>
		return FALSE;
	}
	else if(strResult.Find("curl: (30) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_31%>");//<%IDS_URL_31%>
		return FALSE;
	}
	else if(strResult.Find("curl: (31) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_32%>");//<%IDS_URL_32%>
		return FALSE;
	}
	else if(strResult.Find("curl: (32) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_33%>");//<%IDS_URL_33%>
		return FALSE;
	}
	else if(strResult.Find("curl: (33) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_34%>");//<%IDS_URL_34%>
		return FALSE;
	}
	else if(strResult.Find("curl: (34) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_35%>");//<%IDS_URL_35%>
		return FALSE;
	}
	else if(strResult.Find("curl: (35) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_36%>");//<%IDS_URL_36%>
		return FALSE;
	}
	else if(strResult.Find("curl: (36) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_37%>");//<%IDS_URL_37%>
		return FALSE;
	}
	else if(strResult.Find("curl: (37) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_38%>");//<%IDS_URL_38%>
		return FALSE;
	}
	else if(strResult.Find("curl: (38) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_39%>");//<%IDS_URL_39%>
		return FALSE;
	}
	else if(strResult.Find("curl: (39) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_40%>");//<%IDS_URL_40%>
		return FALSE;
	}
	else if(strResult.Find("curl: (40) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_41%>");//<%IDS_URL_41%>
		return FALSE;
	}
	else if(strResult.Find("curl: (41) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_42%>");//<%IDS_URL_42%>
		return FALSE;
	}
	else if(strResult.Find("curl: (42) ", 0) >= 0)
	{
		strCode =FuncGetStringFromIDS("<%IDS_URL_43%>");//<%IDS_URL_43%>
		return FALSE;
	}
	else if(strResult.Find("curl: (43) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_44%>");//<%IDS_URL_44%>
		return FALSE;
	}
	else if(strResult.Find("curl: (44) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_45%>");//<%IDS_URL_45%>
		return FALSE;
	}
	else if(strResult.Find("curl: (45) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_46%>");//<%IDS_URL_46%>
		return FALSE;
	}
	else if(strResult.Find("curl: (46) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_47%>");//<%IDS_URL_47%>
		return FALSE;
	}
	else if(strResult.Find("curl: (47) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_48%>");//<%IDS_URL_48%>
		return FALSE;
	}
	else if(strResult.Find("curl: (48) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_49%>");//<%IDS_URL_49%>
		return FALSE;
	}
	else if(strResult.Find("curl: (49) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_50%>");//<%IDS_URL_50%>
		return FALSE;
	}
	else if(strResult.Find("curl: (50) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_51%>");//<%IDS_URL_51%>
		return FALSE;
	}
	else if(strResult.Find("curl: (51) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_52%>");//<%IDS_URL_52%>
		return FALSE;
	}
	else if(strResult.Find("curl: (52) ", 0) >= 0)
	{
		strCode = FuncGetStringFromIDS("<%IDS_URL_53%>");//<%IDS_URL_53%>
		return FALSE;
	}

	return TRUE;
}