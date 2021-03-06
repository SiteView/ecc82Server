
#include "SSHMonitor.h"
#include "Utils.h"

//#include "putty.h"
//#include "ssh.h"


#include "base\funcGeneral.h"

int WriteLog(const char* str);

/*
bool IsPlink(const char* strSourKey)
{
	Filename filename = filename_from_str(strSourKey);

	int nkeytype=key_type(&filename);

	if (nkeytype == SSH_KEYTYPE_SSH1 || nkeytype == SSH_KEYTYPE_SSH2)
		return (true);
	else 
		return (false);


}


int ConvertToPlink(const char* strSourKey,char *passphrase,char* strDestkey)
{
	Filename filename = filename_from_str(strSourKey);

	int nkeytype=key_type(&filename);

	char strTemp[1024];
	sprintf(strTemp,"keytype=%d\n",nkeytype);

	OutputDebugString(strTemp);


	if (nkeytype == SSH_KEYTYPE_SSH1 || nkeytype == SSH_KEYTYPE_SSH2)
		return (1);

	struct RSAKey newkey1;	

    struct ssh2_userkey *newkey2 = NULL;

	if (!import_possible(nkeytype)) 
	{
		OutputDebugString("载入key失败！\n");
		return(0);
    }

	

	int type = import_target_type(nkeytype);

	sprintf(strTemp,"target=%d\n",type);

	OutputDebugString(strTemp);

	//import_encrypted(&filename, nkeytype,NULL);

	//return (1);

	if (type == SSH_KEYTYPE_SSH1) 
	{
		import_ssh1(&filename, nkeytype,
				    &newkey1, passphrase,NULL);
	} 
	else 
	{
        newkey2 = import_ssh2(&filename, nkeytype,
				              passphrase, NULL);
		
	}

	filename= filename_from_str(strDestkey);
	
	if(ssh2_save_userkey(&filename, newkey2,passphrase)<0)
		return(0);

	return(1);
}
*/

bool SSHMonitorValue(char *server, int port, char *uid, char *pwd,  
				  int mtype, char *inifile, char *extraparam, char *custpath, char *privatekeyfile,
				  char *szReturn)
{
	WriteLog("============ SSHMonitorValue ============");
	WriteLog( extraparam );

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
		strCmdLine.Format("%s \"%s \\\"%s\\\" \\\"-ssh\\\"  \\\"-P\\\" \\\"%d\\\" \\\"-pw\\\" \\\"%s\\\" \\\"-l\\\" \\\"%s\\\"  \\\"%s\\\" \" ", "invoke_plink",PLINK_PATH, server,port, pwd, uid,   cmd);
	}
	else
	{
		//获取key_file
		char strkeyfile[1024];
		memset(strkeyfile,0,1024);

		strncpy(strkeyfile,privatekeyfile,strlen(privatekeyfile));

		/*
		CFileFind sourFind,destFind;
			
		bool bFindSour=sourFind.FindFile(strkeyfile);

		if(!bFindSour)
		{
			sprintf(szReturn, "error=%s","unable to find keyfile!");
			return false;
		}
		else
		{
			//检测key_file的格式
			if(!IsPlink(strkeyfile))//不是plink格式的文件
			{
				OutputDebugString("不是plink格式的密钥!");

				CString destFileName = strkeyfile;

				destFileName.Remove(':');
				destFileName.Remove('\\');
				destFileName.Remove('.');

				destFileName = "temp\\" + destFileName + ".ppk";

				strncpy(strkeyfile,destFileName.GetBuffer(),destFileName.GetLength()+1);

				OutputDebugString("目标文件名:");
				OutputDebugString(strkeyfile);
				OutputDebugString("\n");

				bool bInExistent = true;
				bool bOutDate = true;

				if(destFind.FindFile(strkeyfile))
				{
					OutputDebugString("密钥文件存在!");

					bInExistent = false;

					ATL::CTime sourTime,destTime;

					sourFind.GetCreationTime( sourTime);
					destFind.GetCreationTime( destTime);

					if(sourTime>destTime)
					{
						OutputDebugString("密钥已过期!");
						bOutDate = true;
						DeleteFile(strkeyfile);
					}
					else
						bOutDate = false;

					destFind.Close();


				}

				if(bInExistent || (!bInExistent&&bOutDate))
				{
					if(!ConvertToPlink(privatekeyfile,pwd,strkeyfile))
					{
						sprintf(szReturn, "error=%s","Convert key failed!");
						return false;
					}	
				}

			}

			sourFind.Close();

		}

		*/

		OutputDebugString(strkeyfile);
		OutputDebugString("\n");
		

		//strCmdLine.Format("%s \"%s\" \"-ssh\"  \"-P\" \"%d\" \"-pw\" \"%s\" \"-l\" \"%s\" \"-i\" "\%s\" \"%s\"  ", PLINK_PATH, server,port, pwd, uid,  privatekeyfile, cmd);
		strCmdLine.Format("%s \"%s \\\"%s\\\" \\\"-ssh\\\" \\\"-2\\\" \\\"-P\\\" \\\"%d\\\" \\\"-pw\\\" \\\"%s\\\" \\\"-l\\\" \\\"%s\\\" \\\"-i\\\" \\\"%s\\\" \\\"%s\\\"  \" ","invoke_plink", PLINK_PATH, server,port, pwd, uid,strkeyfile, cmd);
		
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
		strCmdLine.Format("%s \"%s \\\"%s\\\" \\\"-ssh\\\"  \\\"-P\\\" \\\"%d\\\" \\\"-pw\\\" \\\"%s\\\" \\\"-l\\\" \\\"%s\\\"  \\\"%s\\\" \" ", "invoke_plink",PLINK_PATH, server,port, pwd, uid,   cmd);
	}
	else
	{

		//获取key_file
		char strkeyfile[1024];
		memset(strkeyfile,0,1024);

		strncpy(strkeyfile,privatekeyfile,strlen(privatekeyfile));

		/*
		CFileFind sourFind,destFind;
			
		bool bFindSour=sourFind.FindFile(strkeyfile);

		if(!bFindSour)
		{
			sprintf(szReturn, "error=%s","unable to find keyfile!");
			return false;
		}
		else
		{
			//检测key_file的格式
			if(!IsPlink(strkeyfile))//不是plink格式的文件
			{
				OutputDebugString("不是plink格式的密钥!");

				CString destFileName = strkeyfile;

				destFileName.Remove(':');
				destFileName.Remove('\\');
				destFileName.Remove('.');

				destFileName = "temp\\" + destFileName + ".ppk";

				strncpy(strkeyfile,destFileName.GetBuffer(),destFileName.GetLength()+1);

				OutputDebugString("目标文件名:");
				OutputDebugString(strkeyfile);
				OutputDebugString("\n");

				bool bInExistent = true;
				bool bOutDate = true;

				if(destFind.FindFile(strkeyfile))
				{
					OutputDebugString("密钥文件存在!");

					bInExistent = false;

					ATL::CTime sourTime,destTime;

					sourFind.GetCreationTime( sourTime);
					destFind.GetCreationTime( destTime);

					if(sourTime>destTime)
					{
						OutputDebugString("密钥已过期!");
						bOutDate = true;
						DeleteFile(strkeyfile);
					}
					else
						bOutDate = false;

					destFind.Close();


				}

				if(bInExistent || (!bInExistent&&bOutDate))
				{
					if(!ConvertToPlink(privatekeyfile,pwd,strkeyfile))
					{
						sprintf(szReturn, "error=%s","Convert key failed!");
						return false;
					}	
				}

			}

			sourFind.Close();

		}
		*/

		OutputDebugString(strkeyfile);
		OutputDebugString("\n");


		strCmdLine.Format("%s \"%s \\\"%s\\\" \\\"-ssh\\\" \\\"-2\\\" \\\"-P\\\" \\\"%d\\\" \\\"-pw\\\" \\\"%s\\\" \\\"-l\\\" \\\"%s\\\" \\\"-i\\\" \\\"%s\\\" \\\"%s\\\"  \" ","invoke_plink", PLINK_PATH, server,port, pwd, uid,strkeyfile, cmd);

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
		strCmdLine.Format("%s \"%s \\\"%s\\\" \\\"-ssh\\\"  \\\"-P\\\" \\\"%d\\\" \\\"-pw\\\" \\\"%s\\\" \\\"-l\\\" \\\"%s\\\"  \\\"%s\\\" \" ", "invoke_plink",PLINK_PATH, server,port, pwd, uid,   cmd);
	}
	else
	{

		//获取key_file
		char strkeyfile[1024];
		memset(strkeyfile,0,1024);

		strncpy(strkeyfile,privatekeyfile,strlen(privatekeyfile));

		/*
		CFileFind sourFind,destFind;
			
		bool bFindSour=sourFind.FindFile(strkeyfile);

		if(!bFindSour)
		{
			char str[128];
			sprintf(str, "error=%s","unable to find keyfile!");
			strResult = str;
			return false;
		}
		else
		{
			//检测key_file的格式
			if(!IsPlink(strkeyfile))//不是plink格式的文件
			{
				OutputDebugString("不是plink格式的密钥!");

				CString destFileName = strkeyfile;

				destFileName.Remove(':');
				destFileName.Remove('\\');
				destFileName.Remove('.');

				destFileName = "temp\\" + destFileName + ".ppk";

				strncpy(strkeyfile,destFileName.GetBuffer(),destFileName.GetLength()+1);

				OutputDebugString("目标文件名:");
				OutputDebugString(strkeyfile);
				OutputDebugString("\n");

				bool bInExistent = true;
				bool bOutDate = true;

				if(destFind.FindFile(strkeyfile))
				{
					OutputDebugString("密钥文件存在!");

					bInExistent = false;

					ATL::CTime sourTime,destTime;

					sourFind.GetCreationTime( sourTime);
					destFind.GetCreationTime( destTime);

					if(sourTime>destTime)
					{
						OutputDebugString("密钥已过期!");
						bOutDate = true;
						DeleteFile(strkeyfile);
					}
					else
						bOutDate = false;

					destFind.Close();


				}

				if(bInExistent || (!bInExistent&&bOutDate))
				{
					if(!ConvertToPlink(privatekeyfile,pwd,strkeyfile))
					{
						char str[128];
						sprintf(str, "error=%s","Convert key failed!");
						strResult = str;
						return false;
					}	
				}

			}

			sourFind.Close();

		}
		*/

		OutputDebugString(strkeyfile);
		OutputDebugString("\n");


		//strCmdLine.Format("%s \"%s\" \"-ssh\" \"-2\" \"-P\" \"%d\" \"-pw\" \"%s\" \"-l\" \"%s\" \"-i\" \"%s\" \"%s\"  ", PLINK_PATH, server,port, pwd, uid,strkeyfile, cmd);
		strCmdLine.Format("%s \"%s \\\"%s\\\" \\\"-ssh\\\" \\\"-2\\\" \\\"-P\\\" \\\"%d\\\" \\\"-pw\\\" \\\"%s\\\" \\\"-l\\\" \\\"%s\\\" \\\"-i\\\" \\\"%s\\\" \\\"%s\\\"  \" ","invoke_plink", PLINK_PATH, server,port, pwd, uid,strkeyfile, cmd);
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
	HANDLE hOutputRead, hOutputWrite,hOutputReadDup;
	SECURITY_ATTRIBUTES sa;
	BOOL fSuccess; 


	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	try
	{
		if (CreatePipe(&hOutputRead, &hOutputWrite, &sa, 1024*50)) 
		{

			STARTUPINFO si;
			memset(&si, 0, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			si.dwFlags = STARTF_USESTDHANDLES;
			si.hStdOutput = hOutputWrite;
			si.hStdError = hOutputWrite;

			char strTemp[1024];
			
			PROCESS_INFORMATION pi;
			memset(&pi, 0, sizeof(PROCESS_INFORMATION));

			if (CreateProcess(NULL, strCmdLine.GetBuffer(strCmdLine.GetLength()),  NULL, NULL, TRUE, /*CREATE_NEW_CONSOLE*/CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
			{

				if(WaitForSingleObject(pi.hProcess, 180000) == WAIT_OBJECT_0 )
				{
					//关闭写句柄
					if (!CloseHandle(hOutputWrite)) 
					{
						OutputDebugString("关闭写句柄失败！");
					}


					char buffer[1024*10] = {0};
					DWORD bytesToRead = sizeof(buffer)-1;//保留一位放字符串结束符
					DWORD bytesRead = 0;
					int index = 0;

					if(GetFileSize(hOutputRead,NULL) > 0)//判断管道中是否有数据，必须有数据，方可执行ReadFile
					{
						while(ReadFile(hOutputRead, buffer, bytesToRead, &bytesRead, NULL))//用循环确保管道中的数据被全部读完
						{
							
							buffer[bytesRead] = 0;

							strContent += buffer;
							
							if(bytesToRead > bytesRead)//当读完管道中所有数据时，结束读数
							{
								break;
							}
						}
					}
				}
				else
				{
					strContent="DragonFlow: Timeout";
					//即使时间超时，也不结束子进程invoke_plink.exe，而是让其自己结束！
					//目的是为了invoke_plink.exe可以结束它的子进程plink.exe。
					//TerminateProcess(pi.hProcess, 0);
				}


				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
			}

			CloseHandle(hOutputRead);
			//CloseHandle(hOutputWrite);
		}
	}
	catch(...)
	{
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
