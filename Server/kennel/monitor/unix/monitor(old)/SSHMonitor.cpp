
#include "SSHMonitor.h"
#include "Utils.h"

#include "..\..\base\funcGeneral.h"
extern int PrintLog(const char * strReceive);

bool SSHMonitorValue(char *server, int port, char *uid, char *pwd,  
				  int mtype, char *inifile, char *extraparam, char *custpath, char *privatekeyfile,
				  char *szReturn)
{
	CString	strCmdLine = _T(""), 
			strResult = _T("");
	int		nResult = 0;

	char	iniFile[255] = {0};
	char	cmd[255] = {0};

	GetIniFileName(inifile, iniFile);

	if(!RETURN_COMMAND(mtype, iniFile, cmd))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX",
            "UNIX_EXECUTE_COMMAND_FAILED"));   //  <%IDS_SSHMonitor_1%>
		return false;
	}

	char	*ca = NULL;
	if(ca = strchr(cmd, '<')) 
	{
		strcpy(ca , extraparam);
		*(ca + strlen(extraparam)) = 0;
	}


	//	strCmdLine.Format("%s \"-ssh\" \"-t\" \"%d\" \"-P\" \"%d\" \"-pw\" \"%s\" \"%s@%s\" \"%s\"", PLINK_PATH, CALL_TIMEOUT, port, pwd, uid, server, cmd);
	//wangpeng for new plink

	if(strlen(privatekeyfile) == 0)
	{
		strCmdLine.Format("%s \"%s\" \"-ssh\"  \"-P\" \"%d\" \"-pw\" \"%s\" \"-l\" \"%s\"  \"%s\"  ", PLINK_PATH, server,port, pwd, uid,   cmd);
	}
	else
	{
		//strCmdLine.Format("%s \"%s\" \"-ssh\"  \"-P\" \"%d\" \"-pw\" \"%s\" \"-l\" \"%s\" \"-i\" "\%s\" \"%s\"  ", PLINK_PATH, server,port, pwd, uid,  privatekeyfile, cmd);
		strCmdLine.Format("%s \"%s\" \"-ssh\" \"-2\" \"-P\" \"%d\" \"-pw\" \"%s\" \"-l\" \"%s\" \"-i\" \"%s\" \"%s\"  ", PLINK_PATH, server,port, pwd, uid,privatekeyfile, cmd);
	}

	//OutputDebugString("--------------ssh command line output-----------------\n");
	//OutputDebugString(strCmdLine.GetBuffer());
	//OutputDebugString("\n");
	//WriteDebugString(strCmdLine, false, mtype, __FILE__, __LINE__);//Add By zjw for trace

	nResult = executeExeFile(strCmdLine, strResult);

	//WriteDebugString(strResult, false, nResult, __FILE__, __LINE__);//Add By zjw for trace

	switch(nResult)
	{
	case DragonFlow_OK:
		if(!MONITOR_PARSE(mtype, strResult.GetBuffer(strResult.GetLength()), inifile, extraparam, szReturn))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
                "BASIC_PARSER_STRING_FAILED"));   //  <%IDS_SSHMonitor_2%>
			return false;
		}
		return true;
	case DragonFlow_Timeout:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX",
                "UNIX_TIMEOUT_ERROR"));     //  <%IDS_SSHMonitor_3%>
		break;
	case DragonFlow_OpenConnection:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX",
                "UNIX_CONNECT_FAILED"));   //  <%IDS_SSHMonitor_4%>
		break;
	case DragonFlow_CreateOutputThread:
	case DragonFlow_CreateErrorOutputThread:
	case DragonFlow_CreateInputThread:
	case DragonFlow_WriteToStandardOutput:
	case DragonFlow_OpenCommandFile:
	case DragonFlow_NotEnoughSpace:
	case DragonFlow_InitialiseWinSock:
	case DragonFlow_WinSockVersionIncompatible:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX",
                "UNIX_GENERAL_ERROR"));    //  <%IDS_SSHMonitor_5%>
		break;
	case DragonFlow_FatalError:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX",
                "UNIX_BADLY_ERROR"));     //  <%IDS_SSHMonitor_6%>
		break;
	case DragonFlow_OtherError:
		sprintf(szReturn,"error=%s", strResult);
		break;
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX",
                "UNIX_READ_DATA_FAILED"));    //  <%IDS_SSHMonitor_7%>
		break;
	}

	return false;
}

BOOL SSHUnixPerfMonitorValue(char *server, int port, char *uid, char *pwd,  
				  char *inifile, char *custpath, char *privatekeyfile, char *szReturn)
{
	CString	strCmdLine = _T(""), 
			strResult = _T("");
	int		nResult = 0;

	char	iniFile[255] = {0};
	char	cmd[1024] = {0};

	GetIniFileName(inifile, iniFile);

	if(!RETURN_COMMAND(0, iniFile, cmd))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SSHMonitor_1%>"));   //  <%IDS_SSHMonitor_1%>
		return FALSE;
	}

	//strCmdLine.Format("%s \"-ssh\" \"-t\" \"%d\" \"-P\" \"%d\" \"-pw\" \"%s\" \"%s@%s\" \"%s\"", PLINK_PATH, CALL_TIMEOUT, port, pwd, uid, server, cmd);

	//wangpeng for new plink.exe
	
	if(strlen(privatekeyfile) == 0)
	{
		strCmdLine.Format("%s \"%s\" \"-ssh\"  \"-P\" \"%d\" \"-pw\" \"%s\" \"-l\" \"%s\"  \"%s\"  ", PLINK_PATH, server,port, pwd, uid,   cmd);
	}
	else
	{
		strCmdLine.Format("%s \"%s\" \"-ssh\" \"-2\" \"-P\" \"%d\" \"-pw\" \"%s\" \"-l\" \"%s\" \"-i\" \"%s\" \"%s\"  ", PLINK_PATH, server,port, pwd, uid,privatekeyfile, cmd);
	}


	nResult = executeExeFile(strCmdLine, strResult);
	switch(nResult)
	{
	case DragonFlow_OK:
		if(!MONITOR_PARSE(0, strResult.GetBuffer(strResult.GetLength()), inifile, NULL, szReturn))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SSHMonitor_2%>"));   //  <%IDS_SSHMonitor_2%>
			return FALSE;
		}
		return TRUE;
	case DragonFlow_Timeout:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SSHMonitor_3%>"));     //  <%IDS_SSHMonitor_3%>
		break;
	case DragonFlow_OpenConnection:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SSHMonitor_4%>"));   //  <%IDS_SSHMonitor_4%>
		break;
	case DragonFlow_CreateOutputThread:
	case DragonFlow_CreateErrorOutputThread:
	case DragonFlow_CreateInputThread:
	case DragonFlow_WriteToStandardOutput:
	case DragonFlow_OpenCommandFile:
	case DragonFlow_NotEnoughSpace:
	case DragonFlow_InitialiseWinSock:
	case DragonFlow_WinSockVersionIncompatible:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SSHMonitor_5%>"));    //  <%IDS_SSHMonitor_5%>
		break;
	case DragonFlow_FatalError:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SSHMonitor_6%>"));     //  <%IDS_SSHMonitor_6%>
		break;
	case DragonFlow_OtherError:
		sprintf(szReturn,"error=%s", strResult);
		break;
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SSHMonitor_7%>"));    //  <%IDS_SSHMonitor_7%>
		break;
	}

	return FALSE;
}
							 
int ssh_command(char *server, int port, char *uid, char *pwd, char *cmd, char *privatekeyfile, CString &strResult)
{
	CString	strCmdLine = _T("");
	int		nResult = 0;

	//wangpeng for new plink
	if(strlen(privatekeyfile) == 0)
	{
		strCmdLine.Format("%s \"%s\" \"-ssh\"  \"-P\" \"%d\" \"-pw\" \"%s\" \"-l\" \"%s\"  \"%s\"  ", PLINK_PATH, server,port, pwd, uid,   cmd);
	}
	else
	{
		strCmdLine.Format("%s \"%s\" \"-ssh\" \"-2\" \"-P\" \"%d\" \"-pw\" \"%s\" \"-l\" \"%s\" \"-i\" \"%s\" \"%s\"  ", PLINK_PATH, server,port, pwd, uid,privatekeyfile, cmd);
	}
	//strCmdLine.Format("%s \"-ssh\" \"-t\" \"%d\" \"-P\" \"%d\" \"-pw\" \"%s\" \"%s@%s\" \"%s\"", PLINK_PATH, CALL_TIMEOUT, port, pwd, uid, server, cmd);
	
	

	nResult = executeExeFile(strCmdLine, strResult);
	switch(nResult)
	{
	case DragonFlow_OK:
		return strResult.GetLength();
	case DragonFlow_Timeout:
		break;
	case DragonFlow_OpenConnection:
		break;
	case DragonFlow_CreateOutputThread:
	case DragonFlow_CreateErrorOutputThread:
	case DragonFlow_CreateInputThread:
	case DragonFlow_WriteToStandardOutput:
	case DragonFlow_OpenCommandFile:
	case DragonFlow_NotEnoughSpace:
	case DragonFlow_InitialiseWinSock:
	case DragonFlow_WinSockVersionIncompatible:
		break;
	case DragonFlow_FatalError:
		break;
	case DragonFlow_OtherError:
		break;
	default:
		break;
	}

	return -1;
}

int executeExeFile(CString strCmdLine, CString &strContent)
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

		if (CreateProcess(NULL, strCmdLine.GetBuffer(strCmdLine.GetLength()),  NULL, NULL, TRUE, /*CREATE_NEW_CONSOLE*/CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
			char buffer[BUFFER_SIZE] = {0};
			DWORD bytesRead = 0;

			while(1)
			{
				int index = 0;
				if(ReadFile(hOutputRead, buffer, sizeof(buffer), &bytesRead, NULL))
				{
					buffer[bytesRead] = 0;
					strContent += buffer;
					index = strContent.Find("DragonFlow: ", 0);
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

	int index = 0;
	index = strContent.Find("DragonFlow: OK", 0);
	if(index >= 0)
	{
		strContent.Delete(index, strContent.GetLength() - index);
		return 0;
	}
	else if(strContent.Find("DragonFlow: Timeout") >= 0)
		return 1;
	else if(strContent.Find("DragonFlow: CreateOutputThread") >= 0)
		return 2;
	else if(strContent.Find("DragonFlow: CreateErrorOutputThread") >= 0)
		return 3;
	else if(strContent.Find("DragonFlow: CreateInputThread") >= 0)
		return 4;
	else if(strContent.Find("DragonFlow: WriteToStandardOutput") >= 0)
		return 5;
	else if(strContent.Find("DragonFlow: OpenConnection") >= 0)
		return 6;
	else if(strContent.Find("DragonFlow: OpenCommandFile") >= 0)
		return 7;
	else if(strContent.Find("DragonFlow: NotEnoughSpace") >= 0)
		return 8;
	else if(strContent.Find("DragonFlow: InitialiseWinSock") >= 0)
		return 9;
	else if(strContent.Find("DragonFlow: WinSockVersionIncompatible") >= 0)
		return 10;
	else if(strContent.Find("DragonFlow: Fatal Error") >= 0)
		return 11;
	else if(strContent.Find("DragonFlow") >= 0)
	{
		int nErrIn=strContent.Find("DragonFlow");
		int nEnd =strContent.Find("\r",nErrIn);
		strContent.Delete(0,nErrIn);
		strContent=strContent.Left(nEnd-nErrIn);		
		return 12;
	}
	else 	return -1;
}
