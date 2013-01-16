#include "sshmonitor.h"
#include "../base/SVTelnet.h"
#include "../base/SVDefines.h"

#include <scsvapi/libutil/Time.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SSHMONITOR(map<string, string, less<string> >InputParams, char *pszReturn, int &nSize)
{
    bool bRet = false;
    map<string, string, less<string> >::iterator paramItem;

    string szFrontStr = "", szBackStr = "", szFrontStr1 = "", szBackStr1 = "";
	string szMatchCmd = "", szDayNum = "", szExtParam = "";

	string		szServer = "",
				szUID = "",
				szPWD = "",
				//szPrompt = "# $ >",
				//szLogPrompt = "ogin:",
				//szPwdPrompt = "assword:", 
				szOsType = "";
	int			nPort = 23, 
				nAppendMethod = 1;
	int			nProxyPort = 0;

	string		szProxyServer = "", 
				szProxyUid = "", 
				szProxyPwd = "";

	string		szFileName = "", 
				szMatch    = "", 
				szRuleFile = "",
                szCustomerPath = "";

    paramItem = InputParams.find(SV_Host);
    if(paramItem != InputParams.end())
        szServer = paramItem->second;
    else
    {
        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_InputParam").c_str());//SiteView_ECC:UNIX:服务器名称为空，请检查输入参数。");//UNIX_SERVER_IP_ADDRESS_NULL
        return false;
    }

    paramItem = InputParams.find(SV_Port);
    if(paramItem != InputParams.end())
        nPort = atoi(paramItem->second.c_str());

    if(nPort <= 0)
    {
        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_ServerPortError").c_str());//SiteView_ECC:UNIX:服务器Telnet服务端口错误。");//UNIX_SERVER_PORT_ERROE
        return false;
    }

    paramItem = InputParams.find(SV_User);
    if(paramItem != InputParams.end())
        szUID = paramItem->second;
    else
    {
        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_LoginNameNotNull").c_str());//SiteView_ECC:UNIX:登陆用户名不能为空。");//UNIX_SERVER_USER_NULL
        return false;
    }

    paramItem = InputParams.find(SV_Pwd);
    if(paramItem != InputParams.end())
        szPWD = paramItem->second;

    //paramItem = InputParams.find(SV_LoginPrompt);
    //if(paramItem != InputParams.end())
    //    szLogPrompt = paramItem->second;
    //else
    //{
    //    nSize = sprintf(pszReturn, "error=SiteView_ECC:UNIX:登陆提示符不能为空。");//UNIX_SERVER_LOGIN_PROMPT_NULL
    //    return false;
    //}

    //paramItem = InputParams.find(SV_PwdPrompt);
    //if(paramItem != InputParams.end())
    //    szPwdPrompt = paramItem->second;
    //else
    //{
    //    nSize = sprintf(pszReturn, "error=SiteView_ECC:UNIX:密码提示符不能为空。");//UNIX_SERVER_PASSWORD_PROMPT_NULL
    //    return false;
    //}

    //paramItem = InputParams.find(SV_Prompt);
    //if(paramItem != InputParams.end())
    //    szPrompt = paramItem->second;
    //else
    //{
    //    nSize = sprintf(pszReturn, "error=SiteView_ECC:UNIX:登陆成功提示符不能为空。");//UNIX_SERVER_LOGIN_SUCC_PROMPT_NULL
    //    return false;
    //}

    paramItem = InputParams.find(SV_OSType);
    if(paramItem != InputParams.end())
        szOsType = paramItem->second;
    else
    {
        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_OSTypeError").c_str());//SiteView_ECC:UNIX:操作系统类型错误。");//UNIX_SERVER_OS_IS_NULL
        return false;
    }

    //paramItem = InputParams.find(SV_OSType);
    //if(paramItem != InputParams.end())
    //    szRuleFile = paramItem->second;

    paramItem = InputParams.find("_FrontStr");
    if(paramItem != InputParams.end())
        szFrontStr = paramItem->second;

    paramItem = InputParams.find("_FrontStr1");
    if(paramItem != InputParams.end())
        szFrontStr1 = paramItem->second;

    paramItem = InputParams.find("_BackStr");
    if(paramItem != InputParams.end())
        szBackStr = paramItem->second;

    paramItem = InputParams.find("_BackStr1");
    if(paramItem != InputParams.end())
        szBackStr1 = paramItem->second;

    paramItem = InputParams.find("_DayNum");
    if(paramItem != InputParams.end())
        szDayNum = paramItem->second;

    paramItem = InputParams.find("_MatchCmd");
    if(paramItem != InputParams.end())
        szMatchCmd = paramItem->second;

    paramItem = InputParams.find(SV_MatchStr);
    if(paramItem != InputParams.end())
        szMatchCmd = paramItem->second;

    paramItem = InputParams.find(SV_AppendMethod);
    if(paramItem != InputParams.end())
        nAppendMethod = atoi(paramItem->second.c_str());

    paramItem = InputParams.find(SV_FileName);
    if(paramItem != InputParams.end())
        szFileName = paramItem->second.c_str();

	if(szFileName.empty())
	{
		int nDay = atoi(szDayNum.c_str());
        svutil::TTimeSpan span(nDay, 0, 0, 0);
        svutil::TTime ttime = svutil::TTime::GetCurrentTimeEx() - span;
        char chDate[256] = {0};
        sprintf(chDate, "%d%.2d%.2d", ttime.GetYear(), ttime.GetMonth(), ttime.GetDay());

		if(!szFrontStr.empty() || !szBackStr.empty())
		{
			szFileName = szFrontStr + chDate + szBackStr;
		}

		if(!szBackStr1.empty() || !szFrontStr1.empty())
		{
			szFileName = szFileName + szFrontStr1 + chDate + szBackStr1;
		}
	}

	if(SV_NetInit())
	{
		sprintf(pszReturn, "error=%s",GetResourceValue("IDS_NET_INIT_FAILED").c_str());//通信初始化错误");//, GetResourceValue("IDS_NET_INIT_FAILED"));//<%IDS_Monitor_61%>"通信初始化错误"
		return FALSE;
	}

    bRet = SSH_VALUE(szServer.c_str(), nPort, szUID.c_str(), szPWD.c_str(), szOsType.c_str(), 
        szFileName.c_str(), szMatchCmd.c_str(), szRuleFile.c_str(), nAppendMethod, 
        szCustomerPath.c_str(), pszReturn, nSize);

    SV_NetFree();

    return bRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SSH_VALUE(const char * pszServer, const int & nPort, const char * pszUid, const char * pszPwd, 
                  const char * pszOsType, const char * pszLogFile, const char * pszMatchCmd, 
                  const char * pszRuleFile, const int & nAppend, const char * pszCustomerPath, 
                  char * pszReturn, int & nSize)
{
    bool bRet = true;
    SVUnixOpMonitor  svUnixOpMonitor;
    if(!svUnixOpMonitor.init(pszServer, nPort, pszUid, pszPwd, "", "", ""))
    {
        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_ASSIGN_MEMERY_FAILED").c_str());//分配内存失败");//IDS_ASSIGN_MEMERY_FAILED
        return false;
    }
//    int nInit = 0;
//REOPEN:
//    int nResult=svUnixOpMonitor.telnetOpen();
//    if(svUnixOpMonitor.SVFAILED(nResult))
//    {
//		switch(nResult)
//		{
//		case FISTCONNECTERROR:
//			nSize = sprintf(pszReturn,"error=SiteView_ECC:UNIX:登陆服务器失败。");//UNIX_SERVER_CONNECT_FAILED
//			break;
//		case USERLOGINERROR:
//			nSize = sprintf(pszReturn,"error=SiteView_ECC:UNIX:登陆服务器失败，用户输入提示符不匹配。");//UNIX_SERVER_CONNECT_USERLOGINERROR_FAILED
//			break;
//		case PWDLOGINERROR:
//			nSize = sprintf(pszReturn,"error=SiteView_ECC:UNIX:登陆服务器失败，密码输入提示符不匹配");//UNIX_SERVER_CONNECT_PWDLOGINERROR_FAILED
//			break;
//		case LOGINSOCKETERROR:
//			nSize = sprintf(pszReturn,"error=SiteView_ECC:UNIX:登陆服务器失败，网络错误。");//UNIX_SERVER_CONNECT_SOCKET_FAILED
//			break;
//		case CONNECTTIMEOUTERROR:
//			nSize = sprintf(pszReturn,"error=SiteView_ECC:UNIX:登陆服务器失败，连接超时");//UNIX_SERVER_CONNECT_TIMEOUT_FAILED
//			if (nInit<1)
//			{
//				nInit++;
//				goto REOPEN;
//			}
//			break;				
//		default:
//			nSize = sprintf(pszReturn,"error=SiteView_ECC:UNIX:登陆服务器失败。");//UNIX_SERVER_CONNECT_FAILED
//			break;
//		}
//        bRet = false;
//    }
//    else
    {
        string szCmd = GetUnixCmd(pszOsType, pszMatchCmd);
        if(!szCmd.empty())
        {
            int nIndex = 0;
            if(pszLogFile && strlen(pszLogFile) > 0)
            {
                nIndex = static_cast<int>(szCmd.find("<file>"));
                while(nIndex >= 0)
                {
                    szCmd = szCmd.substr(0, nIndex) + pszLogFile + szCmd.substr(nIndex + strlen("<file>"));
                    nIndex += static_cast<int>(strlen("<file>"));
                    nIndex = static_cast<int>(szCmd.find("<file>", nIndex));
                }
            }
            int nResult = svUnixOpMonitor.SSHCMD(szCmd.c_str());
            if(!svUnixOpMonitor.SVFAILED(nResult))
            {
                nSize = sprintf(pszReturn, "Value=%s", svUnixOpMonitor.GetResultBuff());
            }
            else
            {
                nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_ExecCMDFailed").c_str());//SiteView_ECC:UNIX:执行命令失败。");//SCRIPT_EXECUTE_COMMAND_FAILED");
                bRet =  false;
            }
        }
    }
    return bRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SSHDIRFILEMONITOR(map<string, string, less<string> >InputParams, char *pszReturn, int &nSize)
{
    bool bRet = false;
    map<string, string, less<string> >::iterator paramItem;

    string szFrontStr = "", szBackStr = "", szFrontStr1 = "", szBackStr1 = "";
	string szMatchCmd = "", szDayNum = "", szExtParam = "";
    string szMatchCmd2 = "";

	string		szServer = "",
				szUID = "",
				szPWD = "",
				//szPrompt = "# $ >",
				//szLogPrompt = "ogin:",
				//szPwdPrompt = "assword:", 
				szOsType = "";
	int			nPort = 23, 
				nAppendMethod = 1;
	int			nProxyPort = 0;

	string		szProxyServer = "", 
				szProxyUid = "", 
				szProxyPwd = "";

	string		szFileName = "", 
				szMatch    = "", 
				szRuleFile = "",
                szCustomerPath = "";

    paramItem = InputParams.find(SV_Host);
    if(paramItem != InputParams.end())
        szServer = paramItem->second;
    else
    {
        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_InputParam").c_str());//SiteView_ECC:UNIX:服务器名称为空，请检查输入参数。");//UNIX_SERVER_IP_ADDRESS_NULL
        return false;
    }

    paramItem = InputParams.find(SV_Port);
    if(paramItem != InputParams.end())
        nPort = atoi(paramItem->second.c_str());

    if(nPort <= 0)
    {
        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_ServerPortError").c_str());//SiteView_ECC:UNIX:服务器Telnet服务端口错误。");//UNIX_SERVER_PORT_ERROE
        return false;
    }

    paramItem = InputParams.find(SV_User);
    if(paramItem != InputParams.end())
        szUID = paramItem->second;
    else
    {
        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_LoginNameNotNull").c_str());//SiteView_ECC:UNIX:登陆用户名不能为空。");//UNIX_SERVER_USER_NULL
        return false;
    }

    paramItem = InputParams.find(SV_Pwd);
    if(paramItem != InputParams.end())
        szPWD = paramItem->second;

    //paramItem = InputParams.find(SV_LoginPrompt);
    //if(paramItem != InputParams.end())
    //    szLogPrompt = paramItem->second;
    //else
    //{
    //    nSize = sprintf(pszReturn, "error=SiteView_ECC:UNIX:登陆提示符不能为空。");//UNIX_SERVER_LOGIN_PROMPT_NULL
    //    return false;
    //}

    //paramItem = InputParams.find(SV_PwdPrompt);
    //if(paramItem != InputParams.end())
    //    szPwdPrompt = paramItem->second;
    //else
    //{
    //    nSize = sprintf(pszReturn, "error=SiteView_ECC:UNIX:密码提示符不能为空。");//UNIX_SERVER_PASSWORD_PROMPT_NULL
    //    return false;
    //}

    //paramItem = InputParams.find(SV_Prompt);
    //if(paramItem != InputParams.end())
    //    szPrompt = paramItem->second;
    //else
    //{
    //    nSize = sprintf(pszReturn, "error=SiteView_ECC:UNIX:登陆成功提示符不能为空。");//UNIX_SERVER_LOGIN_SUCC_PROMPT_NULL
    //    return false;
    //}

    paramItem = InputParams.find(SV_OSType);
    if(paramItem != InputParams.end())
        szOsType = paramItem->second;
    else
    {
        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_OSTypeError").c_str());//SiteView_ECC:UNIX:操作系统类型错误。");//UNIX_SERVER_OS_IS_NULL
        return false;
    }

    //paramItem = InputParams.find(SV_OSType);
    //if(paramItem != InputParams.end())
    //    szRuleFile = paramItem->second;

    paramItem = InputParams.find("_FrontStr");
    if(paramItem != InputParams.end())
        szFrontStr = paramItem->second;

    paramItem = InputParams.find("_FrontStr1");
    if(paramItem != InputParams.end())
        szFrontStr1 = paramItem->second;

    paramItem = InputParams.find("_BackStr");
    if(paramItem != InputParams.end())
        szBackStr = paramItem->second;

    paramItem = InputParams.find("_BackStr1");
    if(paramItem != InputParams.end())
        szBackStr1 = paramItem->second;

    paramItem = InputParams.find("_DayNum");
    if(paramItem != InputParams.end())
        szDayNum = paramItem->second;

    paramItem = InputParams.find("_MatchCmd");
    if(paramItem != InputParams.end())
        szMatchCmd = paramItem->second;

    paramItem = InputParams.find("_MatchCmd2");
    if(paramItem != InputParams.end())
        szMatchCmd2 = paramItem->second;

    paramItem = InputParams.find("_ExtParams");
    if(paramItem != InputParams.end())
        szExtParam = paramItem->second;

    paramItem = InputParams.find(SV_MatchStr);
    if(paramItem != InputParams.end())
        szMatchCmd = paramItem->second;

    paramItem = InputParams.find(SV_AppendMethod);
    if(paramItem != InputParams.end())
        nAppendMethod = atoi(paramItem->second.c_str());

    paramItem = InputParams.find(SV_FileName);
    if(paramItem != InputParams.end())
        szFileName = paramItem->second.c_str();

	if(szFileName.empty())
	{
		int nDay = atoi(szDayNum.c_str());
        svutil::TTimeSpan span(nDay, 0, 0, 0);
        svutil::TTime ttime = svutil::TTime::GetCurrentTimeEx() - span;
        char chDate[256] = {0};
        sprintf(chDate, "%d%.2d%.2d", ttime.GetYear(), ttime.GetMonth(), ttime.GetDay());

		if(!szFrontStr.empty() || !szBackStr.empty())
		{
			szFileName = szFrontStr + chDate + szBackStr;
		}

		if(!szBackStr1.empty() || !szFrontStr1.empty())
		{
			szFileName = szFileName + szFrontStr1 + chDate + szBackStr1;
		}
	}

	if(SV_NetInit())
	{
		sprintf(pszReturn, "error=%s",GetResourceValue("IDS_NET_INIT_FAILED").c_str());//通信初始化错误");//, GetResourceValue("IDS_NET_INIT_FAILED"));//<%IDS_Monitor_61%>"通信初始化错误"
		return FALSE;
	}

    bRet = SSH_DIREXIST_FILENUM(szServer.c_str(), nPort, szUID.c_str(), szPWD.c_str(), 
        szOsType.c_str(), szFileName.c_str(), atoi(szDayNum.c_str()), szMatchCmd.c_str(),
        szMatchCmd2.c_str(), szExtParam.c_str(), 
        szRuleFile.c_str(), nAppendMethod, szCustomerPath.c_str(), pszReturn, nSize);

    SV_NetFree();
    return bRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SSH_DIREXIST_FILENUM(const char * pszServer, const int & nPort, const char * pszUid, const char * pszPwd, 
                  const char * pszOsType, const char * pszLogFile, const int &nDayNum, const char * pszMatchCmd, 
                  const char * pszMatchCmd2, const char *pszExtParam,
                  const char * pszRuleFile, const int & nAppend, const char * pszCustomerPath, 
                  char * pszReturn, int & nSize)
{
    bool bRet = true;
    SVUnixOpMonitor  svUnixOpMonitor;
    if(!svUnixOpMonitor.init(pszServer, nPort, pszUid, pszPwd, "", "",   ""))
    {
        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_ASSIGN_MEMERY_FAILED").c_str());//分配内存失败");//IDS_ASSIGN_MEMERY_FAILED
        return false;
    }
//    int nInit = 0;
//REOPEN:
//    int nResult=svUnixOpMonitor.telnetOpen();
//    if(svUnixOpMonitor.SVFAILED(nResult))
//    {
//		switch(nResult)
//		{
//		case FISTCONNECTERROR:
//			nSize = sprintf(pszReturn,"error=%s",GetResourceValue("IDS_LoginServerFailed").c_str());//SiteView_ECC:UNIX:登陆服务器失败。");//UNIX_SERVER_CONNECT_FAILED
//			break;
//		case USERLOGINERROR:
//			nSize = sprintf(pszReturn,"error=%s",GetResourceValue("IDS_LoginServerUserNameError").c_str());//SiteView_ECC:UNIX:登陆服务器失败，用户输入提示符不匹配。");//UNIX_SERVER_CONNECT_USERLOGINERROR_FAILED
//			break;
//		case PWDLOGINERROR:
//			nSize = sprintf(pszReturn,"error=%s",GetResourceValue("IDS_LoginServerPWDError").c_str());//SiteView_ECC:UNIX:登陆服务器失败，密码输入提示符不匹配");//UNIX_SERVER_CONNECT_PWDLOGINERROR_FAILED
//			break;
//		case LOGINSOCKETERROR:
//			nSize = sprintf(pszReturn,"error=%s",GetResourceValue("IDS_LoginServerNetError").c_str());//SiteView_ECC:UNIX:登陆服务器失败，网络错误。");//UNIX_SERVER_CONNECT_SOCKET_FAILED
//			break;
//		case CONNECTTIMEOUTERROR:
//			nSize = sprintf(pszReturn,"error=%s",GetResourceValue("IDS_LoginServerTimeout").c_str());//SiteView_ECC:UNIX:登陆服务器失败，连接超时");//UNIX_SERVER_CONNECT_TIMEOUT_FAILED
//			if (nInit<1)
//			{
//				nInit++;
//				goto REOPEN;
//			}
//			break;				
//		default:
//			nSize = sprintf(pszReturn,"error=%s",GetResourceValue("IDS_LoginServerFailed").c_str());//SiteView_ECC:UNIX:登陆服务器失败。");//UNIX_SERVER_CONNECT_FAILED
//			break;
//		}
//        bRet = false;
//    }
    else
    {
        string szCmd = GetUnixCmd(pszOsType, pszMatchCmd);
        if(!szCmd.empty())
        {
            int nIndex = 0;
            if(pszLogFile && strlen(pszLogFile) > 0)
            {
                nIndex = static_cast<int>(szCmd.find("<file>"));
                while(nIndex >= 0)
                {
                    szCmd = szCmd.substr(0, nIndex) + pszLogFile + szCmd.substr(nIndex + strlen("<file>"));
                    nIndex += static_cast<int>(strlen("<file>"));
                    nIndex = static_cast<int>(szCmd.find("<file>", nIndex));
                }
            }
            int nResult = svUnixOpMonitor.SSHCMD(szCmd.c_str());
            if(!svUnixOpMonitor.SVFAILED(nResult))
            {
                string szReturn = svUnixOpMonitor.GetResultBuff();
                nIndex = static_cast<int>(szReturn.find("Execute permission denied"));
                if(nIndex >= 0)
                {
                    nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_UserNoRight").c_str());//SiteView_ECC:UNIX:登陆用户没有执行该命令的权限。");//SCRIPT_PERMISSION_DENIED
                    bRet = false;
                }
                if(bRet)
                {
                    nIndex = static_cast<int>(szReturn.find("not found"));
                    if(nIndex >= 0)
                    {
                        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_PathError").c_str());//SiteView_ECC:UNIX:路径不正确。");//SCRIPT_PERMISSION_DENIED
                        bRet = false;
                    }
                }
                if(bRet)
                { 
                    nSize = sprintf(pszReturn, "DirExist=%s", szReturn.c_str());
                    //cout << pszReturn << endl;
                    nSize ++;
                    if(szReturn == "1")
                    {
                        memcpy(pszReturn + nSize, "FileNum=0", strlen("FileNum=0"));
                        //cout << pszReturn + nSize << endl;
                        nSize += static_cast<int>(strlen("FileNum=0"));
                    }
                    else
                    {
                        if(pszMatchCmd2 && strlen(pszMatchCmd2))
                        {
                            szCmd = GetUnixCmd(pszOsType, pszMatchCmd2);
                            if(!szCmd.empty())
                            {
                                if(pszLogFile && strlen(pszLogFile) > 0)
                                {
                                    nIndex = static_cast<int>(szCmd.find("<file>"));
                                    while(nIndex >= 0)
                                    {
                                        szCmd = szCmd.substr(0, nIndex) + pszLogFile + szCmd.substr(nIndex + strlen("<file>"));
                                        nIndex += static_cast<int>(strlen("<file>"));
                                        nIndex = static_cast<int>(szCmd.find("<file>", nIndex));
                                    }
                                }
                                if(pszExtParam && strlen(pszExtParam))
                                { 
                                    svutil::TTimeSpan span(nDayNum, 0, 0, 0);
                                    svutil::TTime ttime = svutil::TTime::GetCurrentTimeEx() - span;
                                    char chDate[256] = {0};
                                    sprintf(chDate, "%d", ttime.GetDay());
                                    nIndex = static_cast<int>(szCmd.find(pszExtParam));
                                    while(nIndex >= 0)
                                    {
                                        szCmd = szCmd.substr(0, nIndex) + chDate + szCmd.substr(nIndex + strlen(pszExtParam));
                                        nIndex += static_cast<int>(strlen(chDate));
                                        nIndex = static_cast<int>(szCmd.find(pszExtParam, nIndex));
                                    }
                                }
                                nResult = svUnixOpMonitor.SSHCMD(szCmd.c_str());
                                if(!svUnixOpMonitor.SVFAILED(nResult))
                                {
                                    string szReturn = svUnixOpMonitor.GetResultBuff();
                                    nIndex = static_cast<int>(szReturn.find("Execute permission denied"));
                                    if(nIndex >= 0)
                                    {
                                        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_UserNoRight").c_str());//SiteView_ECC:UNIX:登陆用户没有执行该命令的权限。");//SCRIPT_PERMISSION_DENIED
                                        bRet = false;
                                    }
                                    if(bRet)
                                    {
                                        nIndex = static_cast<int>(szReturn.find("not found"));
                                        if(nIndex >= 0)
                                        {
                                            nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_PathError").c_str());//SiteView_ECC:UNIX:路径不正确。");//SCRIPT_PERMISSION_DENIED
                                            bRet = false;
                                        }
                                    }
                                    if(bRet)
                                    { 
                                        szReturn = "FileNum=" + szReturn;
                                        memcpy(pszReturn + nSize, szReturn.c_str(), strlen(szReturn.c_str()));
                                        //nSize 
                                    }
                                }

                            }
                        }
                    }
                }
            }
            else
            {
                nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_ExecCMDFailed").c_str());//SiteView_ECC:UNIX:执行命令失败。");//SCRIPT_EXECUTE_COMMAND_FAILED");
                bRet =  false;
            }
        }
        else
        {
            nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_ConfigError").c_str());//SiteView_ECC:UNIX:配置文件有误。");//SCRIPT_CONFIG_FILE_ERROR_FAILED");
            bRet =  false;
        }
    }
    return bRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SSHDATE_MONITOR(map<string, string, less<string> >InputParams, char *pszReturn, int &nSize)
{
    bool bRet = false;
    map<string, string, less<string> >::iterator paramItem;

    string szFrontStr = "", szBackStr = "", szFrontStr1 = "", szBackStr1 = "";
	string szMatchCmd = "", szDayNum = "", szExtParam = "";

	string		szServer = "",
				szUID = "",
				szPWD = "",
				//szPrompt = "# $ >",
				//szLogPrompt = "ogin:",
				//szPwdPrompt = "assword:", 
				szOsType = "";
	int			nPort = 23, 
				nAppendMethod = 1;
	int			nProxyPort = 0;

	string		szProxyServer = "", 
				szProxyUid = "", 
				szProxyPwd = "";

	string		szFileName = "", 
				szMatch    = "", 
				szRuleFile = "",
                szCustomerPath = "";

    paramItem = InputParams.find(SV_Host);
    if(paramItem != InputParams.end())
        szServer = paramItem->second;
    else
    {
        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_InputParam").c_str());//SiteView_ECC:UNIX:服务器名称为空，请检查输入参数。");//UNIX_SERVER_IP_ADDRESS_NULL
        return false;
    }

    paramItem = InputParams.find(SV_Port);
    if(paramItem != InputParams.end())
        nPort = atoi(paramItem->second.c_str());

    if(nPort <= 0)
    {
        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_ServerPortError").c_str());//SiteView_ECC:UNIX:服务器Telnet服务端口错误。");//UNIX_SERVER_PORT_ERROE
        return false;
    }

    paramItem = InputParams.find(SV_User);
    if(paramItem != InputParams.end())
        szUID = paramItem->second;
    else
    {
        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_LoginNameNotNull").c_str());//SiteView_ECC:UNIX:登陆用户名不能为空。");//UNIX_SERVER_USER_NULL
        return false;
    }

    paramItem = InputParams.find(SV_Pwd);
    if(paramItem != InputParams.end())
        szPWD = paramItem->second;

    //paramItem = InputParams.find(SV_LoginPrompt);
    //if(paramItem != InputParams.end())
    //    szLogPrompt = paramItem->second;
    //else
    //{
    //    nSize = sprintf(pszReturn, "error=SiteView_ECC:UNIX:登陆提示符不能为空。");//UNIX_SERVER_LOGIN_PROMPT_NULL
    //    return false;
    //}

    //paramItem = InputParams.find(SV_PwdPrompt);
    //if(paramItem != InputParams.end())
    //    szPwdPrompt = paramItem->second;
    //else
    //{
    //    nSize = sprintf(pszReturn, "error=SiteView_ECC:UNIX:密码提示符不能为空。");//UNIX_SERVER_PASSWORD_PROMPT_NULL
    //    return false;
    //}

    //paramItem = InputParams.find(SV_Prompt);
    //if(paramItem != InputParams.end())
    //    szPrompt = paramItem->second;
    //else
    //{
    //    nSize = sprintf(pszReturn, "error=SiteView_ECC:UNIX:登陆成功提示符不能为空。");//UNIX_SERVER_LOGIN_SUCC_PROMPT_NULL
    //    return false;
    //}

    paramItem = InputParams.find(SV_OSType);
    if(paramItem != InputParams.end())
        szOsType = paramItem->second;
    else
    {
        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_OSTypeError").c_str());//SiteView_ECC:UNIX:操作系统类型错误。");//UNIX_SERVER_OS_IS_NULL
        return false;
    }

    //paramItem = InputParams.find(SV_OSType);
    //if(paramItem != InputParams.end())
    //    szRuleFile = paramItem->second;

    paramItem = InputParams.find("_FrontStr");
    if(paramItem != InputParams.end())
        szFrontStr = paramItem->second;

    paramItem = InputParams.find("_FrontStr1");
    if(paramItem != InputParams.end())
        szFrontStr1 = paramItem->second;

    paramItem = InputParams.find("_BackStr");
    if(paramItem != InputParams.end())
        szBackStr = paramItem->second;

    paramItem = InputParams.find("_BackStr1");
    if(paramItem != InputParams.end())
        szBackStr1 = paramItem->second;

    paramItem = InputParams.find("_DayNum");
    if(paramItem != InputParams.end())
        szDayNum = paramItem->second;

    paramItem = InputParams.find("_MatchCmd");
    if(paramItem != InputParams.end())
        szMatchCmd = paramItem->second;

    paramItem = InputParams.find(SV_MatchStr);
    if(paramItem != InputParams.end())
        szMatchCmd = paramItem->second;

    paramItem = InputParams.find(SV_AppendMethod);
    if(paramItem != InputParams.end())
        nAppendMethod = atoi(paramItem->second.c_str());

    paramItem = InputParams.find(SV_FileName);
    if(paramItem != InputParams.end())
        szFileName = paramItem->second.c_str();

	if(szFileName.empty())
	{
		int nDay = atoi(szDayNum.c_str());
        svutil::TTimeSpan span(nDay, 0, 0, 0);
        svutil::TTime ttime = svutil::TTime::GetCurrentTimeEx() - span;
        char chDate[256] = {0};
        sprintf(chDate, "%d%.2d%.2d", ttime.GetYear(), ttime.GetMonth(), ttime.GetDay());

		if(!szFrontStr.empty() || !szBackStr.empty())
		{
			szFileName = szFrontStr + chDate + szBackStr;
		}

		if(!szBackStr1.empty() || !szFrontStr1.empty())
		{
			szFileName = szFileName + szFrontStr1 + chDate + szBackStr1;
		}
	}

	if(SV_NetInit())
	{
		sprintf(pszReturn, "error=%s",GetResourceValue("IDS_NET_INIT_FAILED").c_str());//通信初始化错误");//, GetResourceValue("IDS_NET_INIT_FAILED"));//<%IDS_Monitor_61%>"通信初始化错误"
		return FALSE;
	}

    bRet = SSH_DATE_VALUE(szServer.c_str(), nPort, szUID.c_str(), szPWD.c_str(), szOsType.c_str(), szFileName.c_str(), szMatchCmd.c_str(), szRuleFile.c_str(), nAppendMethod, 
        szCustomerPath.c_str(), pszReturn, nSize);

    SV_NetFree();

    return bRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SSH_DATE_VALUE(const char * pszServer, const int & nPort, const char * pszUid, const char * pszPwd, 
                  const char * pszOsType, const char * pszLogFile, const char * pszMatchCmd, 
                  const char * pszRuleFile, const int & nAppend, const char * pszCustomerPath, 
                  char * pszReturn, int & nSize)
{
    bool bRet = true;
    SVUnixOpMonitor  svUnixOpMonitor;

    if(!svUnixOpMonitor.init(pszServer, nPort, pszUid, pszPwd, "", "", ""))
    {
        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_ASSIGN_MEMERY_FAILED").c_str());//分配内存失败");//IDS_ASSIGN_MEMERY_FAILED
        return false;
    }
//    int nInit = 0;
//REOPEN:
//    int nResult=svUnixOpMonitor.telnetOpen();
//    if(svUnixOpMonitor.SVFAILED(nResult))
//    {
//		switch(nResult)
//		{
//		case FISTCONNECTERROR:
//			nSize = sprintf(pszReturn,"error=%s",GetResourceValue("IDS_LoginServerFailed").c_str());//SiteView_ECC:UNIX:登陆服务器失败。");//UNIX_SERVER_CONNECT_FAILED
//			break;
//		case USERLOGINERROR:
//			nSize = sprintf(pszReturn,"error=%s",GetResourceValue("IDS_LoginServerUserNameError").c_str());//SiteView_ECC:UNIX:登陆服务器失败，用户输入提示符不匹配。");//UNIX_SERVER_CONNECT_USERLOGINERROR_FAILED
//			break;
//		case PWDLOGINERROR:
//			nSize = sprintf(pszReturn,"error=%s",GetResourceValue("IDS_LoginServerPWDError").c_str());//SiteView_ECC:UNIX:登陆服务器失败，密码输入提示符不匹配");//UNIX_SERVER_CONNECT_PWDLOGINERROR_FAILED
//			break;
//		case LOGINSOCKETERROR:
//			nSize = sprintf(pszReturn,"error=%s",GetResourceValue("IDS_LoginServerNetError").c_str());//SiteView_ECC:UNIX:登陆服务器失败，网络错误。");//UNIX_SERVER_CONNECT_SOCKET_FAILED
//			break;
//		case CONNECTTIMEOUTERROR:
//			nSize = sprintf(pszReturn,"error=%s",GetResourceValue("IDS_LoginServerTimeout").c_str());//SiteView_ECC:UNIX:登陆服务器失败，连接超时");//UNIX_SERVER_CONNECT_TIMEOUT_FAILED
//			if (nInit<1)
//			{
//				nInit++;
//				goto REOPEN;
//			}
//			break;				
//		default:
//			nSize = sprintf(pszReturn,"error=%s",GetResourceValue("IDS_LoginServerFailed").c_str());//SiteView_ECC:UNIX:登陆服务器失败。");//UNIX_SERVER_CONNECT_FAILED
//			break;
//		}
//        bRet = false;
//    }
//    else
    {
        cout << "OS Type : " << pszOsType << endl;
        //cout << "Match Command " << pszMatchCmd << endl;
        string szCmd = GetUnixCmd(pszOsType, pszMatchCmd);
        if(!szCmd.empty())
        {
            //cout << szCmd.c_str() << endl;
            int nIndex = 0;
            if(pszLogFile && strlen(pszLogFile) > 0)
            {
                nIndex = static_cast<int>(szCmd.find("<file>"));
                while(nIndex >= 0)
                {
                    szCmd = szCmd.substr(0, nIndex) + pszLogFile + szCmd.substr(nIndex + strlen("<file>"));
                    nIndex += static_cast<int>(strlen("<file>"));
                    nIndex = static_cast<int>(szCmd.find("<file>", nIndex));
                }
            }
            //cout << "after change : " << szCmd.c_str() << endl; 
            int nResult = svUnixOpMonitor.SSHCMD(szCmd.c_str());
            if(!svUnixOpMonitor.SVFAILED(nResult))
            {
                int nLastDay = 0, nLastHour = 0, nLastMinuter = 0;
                int nCurDay = 0, nCurHour = 0, nCurMinuter = 0;
                string szReturn = svUnixOpMonitor.GetResultBuff();
                nIndex = static_cast<int>(szReturn.find("Execute permission denied"));
                if(nIndex >= 0)
                {
                    nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_UserNoRight").c_str());//SiteView_ECC:UNIX:登陆用户没有执行该命令的权限。");//SCRIPT_PERMISSION_DENIED
                    bRet = false;
                }
                if(bRet)
                {
                    nIndex = static_cast<int>(szReturn.find("not found"));
                    if(nIndex >= 0)
                    {
                        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_PathError").c_str());//SiteView_ECC:UNIX:路径不正确。");//SCRIPT_PERMISSION_DENIED
                        bRet = false;
                    }
                }
                if(bRet)
                {
                    sscanf(szReturn.c_str(), "%d %d : %d", &nLastDay, &nLastHour, &nLastMinuter);
                    szCmd = GetUnixCmd(pszOsType, "currenttime_command");
                    if(!szCmd.empty())
                    {
                        nResult = svUnixOpMonitor.SSHCMD(szCmd.c_str());
                        if(!svUnixOpMonitor.SVFAILED(nResult))
                        {
                            szReturn = svUnixOpMonitor.GetResultBuff();
                            nIndex = static_cast<int>(szReturn.find("Execute permission denied"));
                            if(nIndex >= 0)
                            {
                                nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_UserNoRight").c_str());//SiteView_ECC:UNIX:登陆用户没有执行该命令的权限。");//SCRIPT_PERMISSION_DENIED
                                bRet = false;
                            }
                            if(bRet)
                            {
                                nIndex = static_cast<int>(szReturn.find("not found"));
                                if(nIndex >= 0)
                                {
                                    nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_PathError").c_str());//SiteView_ECC:UNIX:路径不正确。");//SCRIPT_PERMISSION_DENIED
                                    bRet = false;
                                }
                            }
                            if(bRet)
                            {
                                sscanf(szReturn.c_str(), "%d %d : %d", &nCurDay, &nCurHour, &nCurMinuter);
                                int nTime = (nCurDay - nLastDay) * 24 * 60 + (nCurHour - nLastHour) * 60 + nCurMinuter - nLastMinuter;
                                nSize = sprintf(pszReturn, "tatoltime=%d", nTime);
                            } 
                        }
                        else
                        {
                            nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_ExecCMDFailed").c_str());//SiteView_ECC:UNIX:执行命令失败。");//SCRIPT_EXECUTE_COMMAND_FAILED");
                            bRet =  false;
                        }
                    }
                    else
                    {
                        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_ConfigError").c_str());//SiteView_ECC:UNIX:配置文件有误。");//SCRIPT_CONFIG_FILE_ERROR_FAILED");
                        bRet =  false;
                    }
                }
            }
            else
            {
                nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_ExecCMDFailed").c_str());//SiteView_ECC:UNIX:执行命令失败。");//SCRIPT_EXECUTE_COMMAND_FAILED");
                bRet =  false;
            }
        }
        else
        {
            nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_ConfigError").c_str());//SiteView_ECC:UNIX:配置文件有误。");//SCRIPT_CONFIG_FILE_ERROR_FAILED");
            bRet =  false;
        }
    }
    return bRet;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SSHDirectory(map<string, string, less<string> >InputParams, char *pszReturn, int nSize)
{
    bool bRet = true;

	string		szServer(""),
				szUID(""),
				szPWD(""),
				szOsType(""),
                szFilePath("");
	int			nPort = 23;

    map<string, string, less<string> >::iterator paramItem;
    paramItem = InputParams.find(SV_Host);
    if(paramItem != InputParams.end())
        szServer = paramItem->second;
    else
    {
        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_InputParam").c_str());//SiteView_ECC:UNIX:服务器名称为空，请检查输入参数。");//UNIX_SERVER_IP_ADDRESS_NULL
        return false;
    }

    paramItem = InputParams.find(SV_Port);
    if(paramItem != InputParams.end())
        nPort = atoi(paramItem->second.c_str());

    if(nPort <= 0)
    {
        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_ServerPortError").c_str());//SiteView_ECC:UNIX:服务器Telnet服务端口错误。");//UNIX_SERVER_PORT_ERROE
        return false;
    }

    paramItem = InputParams.find(SV_User);
    if(paramItem != InputParams.end())
        szUID = paramItem->second;
    else
    {
        nSize = sprintf(pszReturn, "error=%s", GetResourceValue("IDS_LoginNameNotNull").c_str());//SiteView_ECC:UNIX:登陆用户名不能为空。");//UNIX_SERVER_USER_NULL
        return false;
    }

    paramItem = InputParams.find(SV_Pwd);
    if(paramItem != InputParams.end())
        szPWD = paramItem->second;

    paramItem = InputParams.find(SV_OSType);
    if(paramItem != InputParams.end())
        szOsType = paramItem->second;
    else
    {
        nSize = sprintf(pszReturn, "error=%s", GetResourceValue("IDS_OSTypeError").c_str());//SiteView_ECC:UNIX:操作系统类型错误。");//UNIX_SERVER_OS_IS_NULL
        return false;
    }

    paramItem = InputParams.find("_dirpath");
    if(paramItem != InputParams.end())
        szFilePath = paramItem->second;
    else
    {
        nSize = sprintf(pszReturn, "error=%s is null", GetResourceValue("IDC_157").c_str());
        return false;
    }
	if(SV_NetInit())
	{
		sprintf(pszReturn, "error=%s",GetResourceValue("IDS_NET_INIT_FAILED").c_str());//通信初始化错误");//, GetResourceValue("IDS_NET_INIT_FAILED"));//<%IDS_Monitor_61%>"通信初始化错误"
		return FALSE;
	}

    SVUnixOpMonitor  svUnixOpMonitor;

    if(!svUnixOpMonitor.init(szServer.c_str(), nPort, szUID.c_str(), szPWD.c_str(), "", "", ""))
    {
        nSize = sprintf(pszReturn, "error=%s",GetResourceValue("IDS_ASSIGN_MEMERY_FAILED").c_str());//分配内存失败");//IDS_ASSIGN_MEMERY_FAILED
        return false;
    }

    string szReturn1(""), szReturn2(""), szReturn3(""), szReturn4("");
    string szCmd("du -sk " + szFilePath);

    bRet = SSHMonitor(svUnixOpMonitor, szCmd, szReturn1);
    if(bRet)
    {
        szReturn1 = strtriml(szReturn1.c_str());
        szReturn1 = strtrimr(szReturn1.c_str());

        szCmd = "du -a " + szFilePath + " |wc -l";
        bRet = SSHMonitor(svUnixOpMonitor, szCmd, szReturn2);
        if(bRet)
        {
            szReturn2 = strtriml(szReturn2.c_str());
            szReturn2 = strtrimr(szReturn2.c_str());

            szCmd = "du -x " + szFilePath + " |wc -l";
            bRet = SSHMonitor(svUnixOpMonitor, szCmd, szReturn3);
            if(bRet)
            {
                szReturn3 = strtriml(szReturn3.c_str());
                szReturn3 = strtrimr(szReturn3.c_str());

                szCmd = "date -r " + szFilePath + " +'%Y-%m-%d %H-%M-%S'";
                bRet = SSHMonitor(svUnixOpMonitor, szCmd, szReturn4);
                if(bRet)
                {
                    szReturn4 = strtriml(szReturn3.c_str());
                    szReturn4 = strtrimr(szReturn3.c_str());

                    int nFileCount = 0, nFolderCount = 0;
					double dSize = 0.0f;

					dSize = atof( szReturn1.c_str() ) / 1024;
					nFolderCount = atol( szReturn3.c_str() ) - 1;
					nFileCount = atol( szReturn2.c_str() ) - 1 - nFolderCount;
					if (nFileCount < 0) 
						nFileCount = 0;

					sprintf(pszReturn, "FileCount=%ld$FolderCount=%ld$FileSize=%.2f%$lastModified=%s$",
						nFileCount, nFolderCount , dSize, szReturn4.c_str());
                }
                else
                {
                    nSize = sprintf(pszReturn, "error=%s", szReturn4.c_str());
                }
            }
            else
            {
                nSize = sprintf(pszReturn, "error=%s(3)", szReturn3.c_str());
            }
        }
        else
        {
            nSize = sprintf(pszReturn, "error=%s(2)", szReturn3.c_str());
        }
    }
    else
    {
        nSize = sprintf(pszReturn, "error=%s(1)", szReturn1.c_str());
    }

    SV_NetFree();
    return bRet;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SSHMonitor(SVUnixOpMonitor &svUnixOpMonitor, const string &szCmd, string &szReturn)
{
    bool bRet = true;
    if(svUnixOpMonitor.SVFAILED(svUnixOpMonitor.SSHCMD( szCmd.c_str())))
    {
        szReturn = GetResourceValue("IDS_ExecCMDFailed");
        return false;
    }   
    else
    {
        szReturn = svUnixOpMonitor.GetResultBuff(); 
    }
    return bRet;
}

