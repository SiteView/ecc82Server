
#include "SSHMonitor.h"
#include "Utils.h"

//#include "putty.h"
//#include "ssh.h"
#include <iostream>

#include "base\funcGeneral.h"
using namespace std;

int WriteLog(const char* str);
bool WriteLog(CString strFileName,const CString strLog);

bool dyWriteLog( const char* str )
{
	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	char szLogFile[] = "MonitorPlink.log";

	FILE *log = fopen( szLogFile,"a+");
	if( log != NULL )
	{
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fclose( log );
	}

	return true;
}

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
	//////////////////////////////////////////////////////////////////////////
	//lish
	//printf("--------------- SSHMonitorValue Start--------------------\n");
	cout <<"--------------- SSHMonitorValue Start--------------------"<<endl;
	//////////////////////////////////////////////////////////////////////////
	WriteLog("============ SSHMonitorValue ============");
	WriteLog( extraparam );

	CString	strCmdLine = _T(""), 
			strResult = _T("");
	int		nResult = 0;

	char	iniFile[255] = {0};
	char	cmd[255] = {0};

	GetIniFileName(inifile, iniFile);

	if ((mtype == MEMORYIO_TYPE_MONITOR) &&  (strcmp( iniFile, "Sun" ) == 0 || strcmp(iniFile, "sun_china") == 0 || strcmp(iniFile, "AIX") == 0))
	{
		cout << "call sun_china" << endl;

		char buffer[1024] = {0};

		float fTotalMemory(0.0);
		float fFreeMemory(0.0);
		float fPercentUsed(0.0);

		if(!RETURN_COMMAND( TOTAL_PMEMORY, iniFile, cmd ) )
		{
			sprintf( szReturn, "error=%s", FuncGetStringFromIDS( "SV_BASIC",
				"BASIC_READ_CMDFILE_FAILD" ) );// <%IDS_TelMonitor_14%>
			printf( szReturn );
			return false;
		}

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

			OutputDebugString(strkeyfile);
			OutputDebugString("\n");

			printf("strkeyfile = %s\n",strkeyfile);
			//strCmdLine.Format("%s \"%s\" \"-ssh\"  \"-P\" \"%d\" \"-pw\" \"%s\" \"-l\" \"%s\" \"-i\" "\%s\" \"%s\"  ", PLINK_PATH, server,port, pwd, uid,  privatekeyfile, cmd);
			strCmdLine.Format("%s \"%s \\\"%s\\\" \\\"-ssh\\\" \\\"-2\\\" \\\"-P\\\" \\\"%d\\\" \\\"-pw\\\" \\\"%s\\\" \\\"-l\\\" \\\"%s\\\" \\\"-i\\\" \\\"%s\\\" \\\"%s\\\"  \" ","invoke_plink", PLINK_PATH, server,port, pwd, uid,strkeyfile, cmd);

		}

		//////////////////////////////////////////////////////////////////////////
		//lish
		//printf("strCmdLine=%s\n",strCmdLine);
		cout << "strCmdLine = "<<strCmdLine <<endl;
		//////////////////////////////////////////////////////////////////////////

		nResult = executeExeFile(strCmdLine, strResult);
		char* pszTemp;

		switch(nResult)
		{
		case DragonFlow_OK:
			if(!MONITOR_PARSE(TOTAL_PMEMORY, strResult.GetBuffer(strResult.GetLength()), inifile, extraparam, buffer))
			{
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
					"BASIC_PARSER_STRING_FAILED"));   //  <%IDS_SSHMonitor_2%>
				return false;
			}			
			pszTemp = strchr( buffer, '=' ) + 1;
			cout << "pszTemp:" << pszTemp << endl;
			fTotalMemory =(float)atof( pszTemp );

			goto Loop;
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
Loop:
		cout << "Call Loop" << endl;
		if(!RETURN_COMMAND( MEMORYIO_TYPE_MONITOR, iniFile, cmd ) )
		{
			sprintf( szReturn, "error=%s", FuncGetStringFromIDS( "SV_BASIC",
				"BASIC_READ_CMDFILE_FAILD" ) );// <%IDS_TelMonitor_14%>
			printf( szReturn );
			return false;
		}

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

			OutputDebugString(strkeyfile);
			OutputDebugString("\n");

			//strCmdLine.Format("%s \"%s\" \"-ssh\"  \"-P\" \"%d\" \"-pw\" \"%s\" \"-l\" \"%s\" \"-i\" "\%s\" \"%s\"  ", PLINK_PATH, server,port, pwd, uid,  privatekeyfile, cmd);
			strCmdLine.Format("%s \"%s \\\"%s\\\" \\\"-ssh\\\" \\\"-2\\\" \\\"-P\\\" \\\"%d\\\" \\\"-pw\\\" \\\"%s\\\" \\\"-l\\\" \\\"%s\\\" \\\"-i\\\" \\\"%s\\\" \\\"%s\\\"  \" ","invoke_plink", PLINK_PATH, server,port, pwd, uid,strkeyfile, cmd);

		}

		//////////////////////////////////////////////////////////////////////////
		//lish
		//printf("strCmdLine=%s\n",strCmdLine);
		cout << "strCmdLine = "<<strCmdLine <<endl;
		//////////////////////////////////////////////////////////////////////////

		nResult = executeExeFile(strCmdLine, strResult);

		CString strTemp = _T("");
		CString strVal = _T("");
		CString strMem = _T("");
		int nPos = 0;
		int nEnd = 0;

		switch(nResult)
		{
		case DragonFlow_OK:
			if(!MONITOR_PARSE(MEMORYIO_TYPE_MONITOR, strResult.GetBuffer(strResult.GetLength()), inifile, extraparam, buffer))
			{
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
					"BASIC_PARSER_STRING_FAILED"));   //  <%IDS_SSHMonitor_2%>
				return false;
			}			
			strTemp = buffer;

			cout << "strTemp:" << endl;
			nPos = strTemp.Find("Mbfree=",0);
			strMem = strTemp.Left(nPos);
			nPos += strlen("Mbfree=");
			nEnd = strTemp.Find("$",nPos);
			strVal = strTemp.Mid(nPos, nEnd - nPos);
			fFreeMemory = atof((LPCTSTR)strVal);
			if( fTotalMemory > 0 )
			{
				fPercentUsed = ( fTotalMemory - fFreeMemory ) / fTotalMemory;
				fTotalMemory /= ( 1024*1024 );
				fFreeMemory /= ( 1024*1024 );
				fPercentUsed *= 100;
			}
			strTemp.Format("TotalMemory=%.2f$Mbfree=%.2f$percentUsed=%.2f$",fTotalMemory,fFreeMemory,fPercentUsed);
			strMem += strTemp;
			sprintf(szReturn,"%s",strMem);
			cout << "szReturn:" << szReturn << endl;

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


Loop1:
	cout << "all" << endl;
	cout << "mtype = " << mtype << endl;  
	cout << "iniFile = " << iniFile << endl;


	if(!RETURN_COMMAND(mtype, iniFile, cmd))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX",
            "UNIX_EXECUTE_COMMAND_FAILED"));   //  <%IDS_SSHMonitor_1%>
		return false;
	}
	CString strFile = "SSHMonitorValue";
	//WriteLog(strFile,cmd);
	//////////////////////////////////////////////////////////////////////////
	//lish
	//printf("cmd=%s\n",cmd);
	cout <<"cmd = "<<cmd <<endl; 
	//////////////////////////////////////////////////////////////////////////


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

		OutputDebugString(strkeyfile);
		OutputDebugString("\n");
		

		//strCmdLine.Format("%s \"%s\" \"-ssh\"  \"-P\" \"%d\" \"-pw\" \"%s\" \"-l\" \"%s\" \"-i\" "\%s\" \"%s\"  ", PLINK_PATH, server,port, pwd, uid,  privatekeyfile, cmd);
		strCmdLine.Format("%s \"%s \\\"%s\\\" \\\"-ssh\\\" \\\"-2\\\" \\\"-P\\\" \\\"%d\\\" \\\"-pw\\\" \\\"%s\\\" \\\"-l\\\" \\\"%s\\\" \\\"-i\\\" \\\"%s\\\" \\\"%s\\\"  \" ","invoke_plink", PLINK_PATH, server,port, pwd, uid,strkeyfile, cmd);
		
	}
	
	//OutputDebugString("--------------ssh command line output-----------------\n");
	//OutputDebugString(strCmdLine.GetBuffer());
	//OutputDebugString("\n");
	//WriteDebugString(strCmdLine, false, mtype, __FILE__, __LINE__);//Add By zjw for trace

	//////////////////////////////////////////////////////////////////////////
	//lish
	//printf("strCmdLine=%s\n",strCmdLine);
	cout << "strCmdLine = "<<strCmdLine <<endl;
	//////////////////////////////////////////////////////////////////////////


	nResult = executeExeFile(strCmdLine, strResult);

	//////////////////////////////////////////////////////////////////////////
	//lish
	//printf("strCmdLine=%s\nnResult=%d\n strResult:\n%s\n",strCmdLine,nResult,strResult);
	//cout << 
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	//lish
	//printf("--------------- SSHMonitorValue End--------------------\n");
	//////////////////////////////////////////////////////////////////////////

	//WriteDebugString(strResult, false, nResult, __FILE__, __LINE__);//Add By zjw for trace

	//WriteLog(strResult.GetBuffer(strResult.GetLength()));




	switch(nResult)
	{
	case DragonFlow_OK:
		//if ((strlen(custpath))>0 && mtype==SERVICE_TYPE_MONITOR)
		//{
		//	if(!MONITOR_PARSE_Process(custpath,strResult.GetBuffer(strResult.GetLength()), inifile, extraparam, szReturn))
		//	{
		//		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
		//			"BASIC_PARSER_STRING_FAILED"));   //  <%IDS_SSHMonitor_2%>
		//		return false;
		//	}
		//}
		//else
		//{
			if(!MONITOR_PARSE(mtype, strResult.GetBuffer(strResult.GetLength()), inifile, extraparam, szReturn))
			{
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
					"BASIC_PARSER_STRING_FAILED"));   //  <%IDS_SSHMonitor_2%>
				return false;
			}
		//}
		
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
	case UnixServerConnectionLimit:
		sprintf(szReturn,"error=%s", FuncGetStringFromIDS("SV_UNIX",
			    "UNIX_SERVER_MAX_CONNECTION_LIMIT"));
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
	
	
//	WriteLog("ssh_command", strCmdLine);
	nResult = executeExeFile(strCmdLine, strResult);


	WriteLog("nResult");
	WriteLog("nResult");

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

	PROCESS_INFORMATION pi;  //dy
	DWORD dwResult;			 //dy

	try
	{
		if (CreatePipe(&hOutputRead, &hOutputWrite, &sa, 1024*100))  //1024*50
		{

			STARTUPINFO si;
			memset(&si, 0, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			si.dwFlags = STARTF_USESTDHANDLES;
			si.hStdOutput = hOutputWrite;
			si.hStdError = hOutputWrite;

			char strTemp[1024];
			
			//PROCESS_INFORMATION pi;  //原来的
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


					char buffer[1024*100] = {0}; //1024*10
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
		
				//TerminateProcess(pi.hProcess, 0); //dy+ 把进程关了 2010-05-28 //还是让他自己结束吧
				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
			}

			CloseHandle(hOutputRead);
			//CloseHandle(hOutputWrite);
//dy----------------------------------------------------
			GetExitCodeThread(pi.hProcess, &dwResult);

			//if(dwResult == STILL_ACTIVE)//说明还在运行，再关一次
			//{
			//	TerminateProcess(pi.hProcess, 0);
			//	CloseHandle(pi.hProcess);
			//	CloseHandle(pi.hThread);  
			//	dyWriteLog("close");
			//}
//dy----------------------------------------------------
		}
	}
	catch(...)
	{
		CloseHandle(hOutputRead);
		CloseHandle(hOutputWrite);
	}
    

	int index = 0;
	index = strContent.Find("DragonFlow: OK", 0);

	CString strTemp;
	int nCount = 0;

	//////////////////////////////////////////////////////////////////////////
	//lish
	printf("------------------ executeExeFile start ------------------\n");
	nCount = strContent.GetLength() - index ;
	
	printf("index=%d\nnCount=%d\nstrContent:\n%s\n",index,nCount,strContent);

	printf("------------------ executeExeFile end ------------------\n");
	//////////////////////////////////////////////////////////////////////////
	if(index >= 0)
	{
		strContent.Delete(index, strContent.GetLength() - index);
//		sprintf( strContent, strTemp.GetBuffer( strTemp.GetLength() ) );
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
	else if(strContent.Find("Server unexpectedly closed network connection") >= 0)//段毅2010-08-17 服务器连接数限制
	{
		char temp[50] = {0};
		sprintf(temp, "dwResult 333 = %02X",dwResult);
		dyWriteLog(temp);
		dyWriteLog("return 13 strContent = ");
		dyWriteLog(strContent.GetBuffer(strContent.GetLength()));
		if(dwResult == STILL_ACTIVE)//说明进程还在运行，再关一次
		{
			TerminateProcess(pi.hProcess, 0);
			/*			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread); */ 
			dyWriteLog("Server unexpectedly closed ,ProcessClose");
		}
		return 13;
	}	
	else
	{

//dy--------------2010-05-28 湖北电力pling.exe,invoke_plink.exe不退出-------------- 
		//strContent.Find("Server unexpectedly closed network connection") >= 0
		char temp[50] = {0};
        sprintf(temp, "dwResult 333 = %02X",dwResult);
		dyWriteLog(temp);
		dyWriteLog("return -1 strContent = ");
		dyWriteLog(strContent.GetBuffer(strContent.GetLength()));
		if(dwResult == STILL_ACTIVE)//说明进程还在运行，再关一次
		{
			TerminateProcess(pi.hProcess, 0);
/*			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread); */ 
			dyWriteLog("ProcessClose");
		}
//dy----------------------------------------------------
		return -1;
	}
		
		
}
///
///--------------------bin.liu---------------
///--------------------卓望进程--------------
///--------------------2012-3-7--------------
///
bool ZHWSSHMonitorValue(char *server, int port, char *uid, char *pwd,  
				  int mtype, char *inifile, char *strMatchStr,char *strMatchALLStr, char *custpath, char *privatekeyfile,
				  char *szReturn)
{
	//////////////////////////////////////////////////////////////////////////
	//lish
	//printf("--------------- SSHMonitorValue Start--------------------\n");
	cout <<"--------------- SSHMonitorValue Start--------------------"<<endl;
	//////////////////////////////////////////////////////////////////////////
	WriteLog("============ SSHMonitorValue ============");
	//WriteLog( extraparam );
	WriteLog( strMatchStr );

	CString	strCmdLine = _T(""), 
			strResult = _T("");
	int		nResult = 0;

	char	iniFile[255] = {0};
	char	cmd[255] = {0};

	GetIniFileName(inifile, iniFile);
Loop1:
	cout << "all" << endl;
	cout << "mtype = " << mtype << endl;  
	cout << "iniFile = " << iniFile << endl;


	if(!RETURN_COMMAND(mtype, iniFile, cmd))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX",
            "UNIX_EXECUTE_COMMAND_FAILED"));   //  <%IDS_SSHMonitor_1%>
		return false;
	}
	CString strFile = "SSHMonitorValue";
	//WriteLog(strFile,cmd);
	//////////////////////////////////////////////////////////////////////////
	//lish
	//printf("cmd=%s\n",cmd);
	cout <<"cmd = "<<cmd <<endl; 
	//////////////////////////////////////////////////////////////////////////


	///待研究
	/*char	*ca = NULL;
	if(ca = strchr(cmd, '<')) 
	{
		strcpy(ca , extraparam);
		*(ca + strlen(extraparam)) = 0;
	}*/
	

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

		OutputDebugString(strkeyfile);
		OutputDebugString("\n");
		

		//strCmdLine.Format("%s \"%s\" \"-ssh\"  \"-P\" \"%d\" \"-pw\" \"%s\" \"-l\" \"%s\" \"-i\" "\%s\" \"%s\"  ", PLINK_PATH, server,port, pwd, uid,  privatekeyfile, cmd);
		strCmdLine.Format("%s \"%s \\\"%s\\\" \\\"-ssh\\\" \\\"-2\\\" \\\"-P\\\" \\\"%d\\\" \\\"-pw\\\" \\\"%s\\\" \\\"-l\\\" \\\"%s\\\" \\\"-i\\\" \\\"%s\\\" \\\"%s\\\"  \" ","invoke_plink", PLINK_PATH, server,port, pwd, uid,strkeyfile, cmd);
		
	}
	
	//OutputDebugString("--------------ssh command line output-----------------\n");
	//OutputDebugString(strCmdLine.GetBuffer());
	//OutputDebugString("\n");
	//WriteDebugString(strCmdLine, false, mtype, __FILE__, __LINE__);//Add By zjw for trace

	//////////////////////////////////////////////////////////////////////////
	//lish
	//printf("strCmdLine=%s\n",strCmdLine);
	cout << "strCmdLine = "<<strCmdLine <<endl;
	//////////////////////////////////////////////////////////////////////////


	nResult = executeExeFile(strCmdLine, strResult);

	//////////////////////////////////////////////////////////////////////////
	//lish
	//printf("strCmdLine=%s\nnResult=%d\n strResult:\n%s\n",strCmdLine,nResult,strResult);
	//cout << 
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	//lish
	//printf("--------------- SSHMonitorValue End--------------------\n");
	//////////////////////////////////////////////////////////////////////////

	//WriteDebugString(strResult, false, nResult, __FILE__, __LINE__);//Add By zjw for trace

	//WriteLog(strResult.GetBuffer(strResult.GetLength()));




	switch(nResult)
	{
	case DragonFlow_OK:
		//if ((strlen(custpath))>0 && mtype==SERVICE_TYPE_MONITOR)
		//{
		if(!MONITOR_PARSE_Process(strMatchStr,strResult.GetBuffer(strResult.GetLength()), inifile, strMatchALLStr, szReturn))
			{
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
					"BASIC_PARSER_STRING_FAILED"));   //  <%IDS_SSHMonitor_2%>
				return false;
			}
		//}
		//else
		//{
			//if(!MONITOR_PARSE(mtype, strResult.GetBuffer(strResult.GetLength()), inifile, strMatchStr, szReturn))
			//{
			//	sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_BASIC",
			//		"BASIC_PARSER_STRING_FAILED"));   //  <%IDS_SSHMonitor_2%>
			//	return false;
			//}
		//}
		
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
	case UnixServerConnectionLimit:
		sprintf(szReturn,"error=%s", FuncGetStringFromIDS("SV_UNIX",
			    "UNIX_SERVER_MAX_CONNECTION_LIMIT"));
		break;

	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("SV_UNIX",
                "UNIX_READ_DATA_FAILED"));    //  <%IDS_SSHMonitor_7%>
		break;
	}

	return false;
}
