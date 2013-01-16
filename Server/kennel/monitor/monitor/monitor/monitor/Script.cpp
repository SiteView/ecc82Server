
#include "Script.h"
#include "Utils.h"
#include "TelMonitor.h"
#include "SSHMonitor.h"
#include "HttpMonitor.h"
#include "RloginMonitor.h"
#include "iostream"

#include "base\funcGeneral.h"

//extern char	databuffer[BUF_SIZE];
//extern int	datalen;
using namespace std;

extern char	rlogin_databuffer[BUF_SIZE];
extern int	rlogin_datalen;

BOOL TELEXESCRIPT(char *server, int port, char *uid, char *pwd, 
                  char *proxy_server, int proxy_port, char *proxy_uid, char *proxy_pwd, 
                  char *logprompt, char *pwdprompt, char *prompts, 
                  char *inifile, char *script, char *params, char *match, 
                  char *custpath, char *szReturn)
{
    int		ret = 0;
    int		hSocket = 0;
    BOOL	bResult = TRUE;
    char	iniFile[255] = {0};
    char	cmd[255] = {0};
    DWORD	dwbTime = 0, dweTime = 0;

    /*************************************
    * For Reason Of Prompt
    */
    int		c = 0;
    char	*ca = prompts, *cb = NULL;
    char	arprompt[PR_NUM][256];
    memset(arprompt, 0, sizeof(arprompt));

    while(cb = strchr(ca, 0x20))
    {
        if(ca == cb) 
        {
            ca ++;
            continue;
        }

        strncpy(arprompt[c++], ca, cb - ca);
        ca = cb + 1;
    }

    strcpy(arprompt[c++], ca);
    /***********************************/

    LPGDATA pgd=new GDATA;
    memset(pgd,0,sizeof(GDATA));


    dwbTime = ::GetTickCount();

    hSocket = telnet_init(server, port, proxy_server, proxy_port, proxy_uid, proxy_pwd);
    if(hSocket <= 0)
    {
        if(hSocket == -1)
            sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_17%>"));    //  <%IDS_TelMonitor_17%>
        else if(hSocket == -2)
            sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_18%>"));   //  <%IDS_TelMonitor_18%>
        else if(hSocket == -3)
            sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_19%>"));     //  <%IDS_TelMonitor_19%>
        else if(hSocket == -4)
            sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_20%>"));     //  <%IDS_TelMonitor_20%>
        else if(hSocket == -5)
            sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_21%>"));     //  <%IDS_TelMonitor_21%>
        else 
            sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_22%>"));   //  <%IDS_TelMonitor_22%>

        bResult = FALSE;
        goto err;
    }

    if((ret = telnet_connect(hSocket, uid, pwd, logprompt, pwdprompt, arprompt,pgd)) < 0)
    {
        if(ret == -1)
            sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_23%>"));    //  <%IDS_TelMonitor_23%>
        else if(ret == -2)
            sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_24%>"));    //  <%IDS_TelMonitor_24%>
        else if(ret == -3)
            sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_25%>"));    //  <%IDS_TelMonitor_25%>
        else if(ret == -4)
            sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_26%>"));   //  <%IDS_TelMonitor_26%>
        else if(ret == -5)
            sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_27%>"));    //  <%IDS_TelMonitor_27%>
        else if(ret == -6)
            sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_28%>"));   //  <%IDS_TelMonitor_28%>
        else 
            sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_29%>"));    //  <%IDS_TelMonitor_29%>

        bResult = FALSE;
        goto err;

    }

	//---------------------------------------------------------------------------------------------------------
	//获取脚本的绝对路径！( zouXiao 2008-12-25 )
	//---------------------------------------------------------------------------------------------------------
	char scriptsPath[1024] = {0};

	if( DFN_GetPrivateProfileString( inifile, "scripts_path", "", scriptsPath, 1024, "oscmd.ini" ) <= 0 )
	{
		sprintf( szReturn, "error=%s", "获取脚本路径失败！" );
        bResult = FALSE;
        goto err;	
	}
	if ( scriptsPath[0] != '/' )
	{
		sprintf( szReturn, "error=%s", "路径不是绝对路径！" );
        bResult = FALSE;
        goto err;
	}

    sprintf(cmd, "%s/%s %s", scriptsPath, script, params);
    //sprintf(cmd, "./scripts/%s %s", script, params);
    if((ret = telnet_command(hSocket, cmd,pgd)) < 0)
    {
        sprintf(szReturn, "error=%s", "send command failed.");
        bResult = FALSE;
        goto err;
    }

    dweTime = ::GetTickCount();

    if(*match)
    {
        if(!strstr(pgd->databuffer, match))
        {
            sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_30%>"));    //  <%IDS_TelMonitor_30%>
            bResult = FALSE;
            goto err;
        }
    }

    sprintf(szReturn, "roundTripTime=%ld$", dweTime - dwbTime);

    {
        char *ca = NULL;

        if(ca = strstr(pgd->databuffer, "Return Code:"))
            sprintf(szReturn, "%sValue=%ld$", szReturn, atol(ca + 13));
		else
            sprintf(szReturn, "%sValue=0$", szReturn);

        if(ca = strstr(pgd->databuffer, "Return Code2:"))
            sprintf(szReturn, "%sValue2=%ld$", szReturn, atol(ca + 14));
        else
            sprintf(szReturn, "%sValue2=0$", szReturn);

        if(ca = strstr(pgd->databuffer, "Return Code3:"))
        {
            char tmp[1024] = {0};

            char *cb = strchr(ca, '\r');
            if(cb)
                strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
            else
            {
                cb = strchr(ca, '\n');
                if(cb)
                    strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
                else
                    strncpy(tmp, ca + 14, strlen(ca + 14) < 1024 ? strlen(ca + 14) : 1023);
            }

            sprintf(szReturn, "%sValue3=%s$", szReturn, tmp);
        }
        else
            sprintf(szReturn, "%sValue3=0$", szReturn);

        if(ca = strstr(pgd->databuffer, "Return Code4:"))
        {
            char tmp[1024] = {0};

            char *cb = strchr(ca, '\r');
            if(cb)
                strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
            else
            {
                cb = strchr(ca, '\n');
                if(cb)
                    strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
                else
                    strncpy(tmp, ca + 14, strlen(ca + 14) < 1024 ? strlen(ca + 14) : 1023);
            }

            sprintf(szReturn, "%sValue4=%s$", szReturn, tmp);
        }
        else
            sprintf(szReturn, "%sValue4=0$", szReturn);
    }
    char *pos = szReturn;
    while(pos != '\0')
    {
        if(pos = strstr(pos, "$"))
        {
            *pos = '\0';
            pos ++;
        }
        else
        {
            break;
        }
    }

err:
	delete pgd;
	shutdown(hSocket, 0x02);
	closesocket(hSocket);
	return bResult;
}

CString TELGETSCRIPT(char *server, int port, char *uid, char *pwd, 
					 char *proxy_server, int proxy_port, char *proxy_uid, char *proxy_pwd, 
					 char *logprompt, char *pwdprompt, char *prompts, 
					 char *inifile, char *custpath)
{
	int		ret = 0;
	int		hSocket = 0;
	char	iniFile[255] = {0};
	char	cmd[255] = {0};
	char    szReturn[256]={0};

	/*************************************
	 * For Reason Of Prompt
	 */
	int		c = 0;
	char	*ca = prompts, *cb = NULL;
	char	arprompt[PR_NUM][256];
	memset(arprompt, 0, sizeof(arprompt));

	while(cb = strchr(ca, 0x20))
	{
		if(ca == cb) 
		{
			ca ++;
			continue;
		}

		strncpy(arprompt[c++], ca, cb - ca);
		ca = cb + 1;
	}

	strcpy(arprompt[c++], ca);
	/***********************************/

	LPGDATA pgd=new GDATA;
	memset(pgd,0,sizeof(GDATA));


	hSocket = telnet_init(server, port, proxy_server, proxy_port, proxy_uid, proxy_pwd);
	if(hSocket <= 0)
	{
		if(hSocket == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_31%>"));    //  <%IDS_TelMonitor_31%>
		else if(hSocket == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_32%>"));    //  <%IDS_TelMonitor_32%>
		else if(hSocket == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_33%>"));      //  <%IDS_TelMonitor_33%>
		else if(hSocket == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_34%>"));      //  <%IDS_TelMonitor_34%>
		else if(hSocket == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_35%>"));    //  <%IDS_TelMonitor_35%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_36%>"));   //  <%IDS_TelMonitor_36%>
	
		shutdown(hSocket, 0x02);
		closesocket(hSocket);
		delete pgd;
		return _T(szReturn);
	}

	if((ret = telnet_connect(hSocket, uid, pwd, logprompt, pwdprompt, arprompt,pgd)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_37%>"));    //  <%IDS_TelMonitor_37%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_38%>"));    //  <%IDS_TelMonitor_38%>
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_39%>"));    //  <%IDS_TelMonitor_39%>
		else if(ret == -4) 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_40%>"));   //  <%IDS_TelMonitor_40%>
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_41%>"));   //  <%IDS_TelMonitor_41%>
		else if(ret == -6)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_42%>"));   //  <%IDS_TelMonitor_42%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_43%>"));    //  <%IDS_TelMonitor_43%>

		shutdown(hSocket, 0x02);
		closesocket(hSocket);
		delete pgd;
		return _T(szReturn);

	}

	GetIniFileName(inifile, iniFile);
	
	if(!RETURN_COMMAND(FILE_TYPE_MONITOR, iniFile, cmd))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_44%>"));     //  <%IDS_TelMonitor_44%>
		shutdown(hSocket, 0x02);
		closesocket(hSocket);
		delete pgd;
		return _T(szReturn);
	}


	//---------------------------------------------------------------------------------------------------------
	//获取脚本的绝对路径！( zouXiao 2008-12-25 )
	//---------------------------------------------------------------------------------------------------------
	char scriptsPath[1024] = {0};

	if( DFN_GetPrivateProfileString( inifile, "scripts_path", "", scriptsPath, 1024, "oscmd.ini" ) <= 0 )
	{
		sprintf( szReturn, "error=%s", "获取脚本路径失败！" );
        shutdown( hSocket, 0x02 );
		closesocket( hSocket );
		delete pgd;
		return _T( szReturn );	
	}
	if ( scriptsPath[0] != '/' )
	{
		sprintf( szReturn, "error=%s", "路径不是绝对路径！" );
        shutdown( hSocket, 0x02 );
		closesocket( hSocket );
		delete pgd;
		return _T( szReturn );
	}

	sprintf( cmd, "%s %s", cmd, scriptsPath );

	//strncat(cmd, " scripts", 8);
	OutputDebugString(cmd);
	if((ret = telnet_command(hSocket, cmd,pgd)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_45%>"));    //  <%IDS_TelMonitor_45%>
		shutdown(hSocket, 0x02);
		closesocket(hSocket);
		delete pgd;
		return _T(szReturn);
	}

	shutdown(hSocket, 0x02);
	closesocket(hSocket);
	CString strRet=pgd->databuffer;
	delete pgd;
	return strRet;
}

BOOL SSHEXESCRIPT(char *server, int port, char *uid, char *pwd, 
				  char *inifile, char *script, char *params, char *match, 
				  char *custpath, char *szReturn)
{
	CString	strCmdLine = _T(""), 
			strResult = _T("");
	int		nResult = 0;
	char	iniFile[255] = {0};
	char	cmd[255] = {0};
	DWORD	dwbTime = 0, dweTime = 0;

	dwbTime = ::GetTickCount();

	//---------------------------------------------------------------------------------------------------------
	//获取脚本的绝对路径！( zouXiao 2008-12-25 )
	//---------------------------------------------------------------------------------------------------------
	char scriptsPath[1024] = {0};

	if( DFN_GetPrivateProfileString( inifile, "scripts_path", "", scriptsPath, 1024, "oscmd.ini" ) <= 0 )
	{
		sprintf( szReturn, "error=%s", "获取脚本路径失败！" );
        return FALSE;
	}
	if ( scriptsPath[0] != '/' )
	{
		sprintf( szReturn, "error=%s", "路径不是绝对路径！" );
        return FALSE;
	}

    sprintf(cmd, "%s/%s %s", scriptsPath, script, params);
	//sprintf(cmd, "./scripts/%s %s", script, params);

	cout << "cmd:" <<cmd<<endl;

	//strCmdLine.Format("%s \"-ssh\" \"-t\" \"%d\" \"-P\" \"%d\" \"-pw\" \"%s\" \"%s@%s\" \"%s\"", PLINK_PATH, CALL_TIMEOUT, port, pwd, uid, server, cmd);
	strCmdLine.Format("%s \"-ssh\"  \"-P\" \"%d\" \"-pw\" \"%s\" \"%s@%s\" \"%s\"", PLINK_PATH,  port, pwd, uid, server, cmd);
	nResult = executeExeFile(strCmdLine, strResult);

	switch(nResult)
	{
	case DragonFlow_OK:
		dweTime = ::GetTickCount();

		if(*match)
		{
			if(strResult.Find(match, 0) < 0)
			{
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SSHMonitor_8%>"));   //  <%IDS_SSHMonitor_8%>
				return FALSE;
			}
		}

		sprintf(szReturn, "roundTripTime=%ld$", dweTime - dwbTime);

		{
		char *ca = NULL;

		if(ca = strstr(strResult, "Return Code: "))
			sprintf(szReturn, "%sValue=%ld$", szReturn, atol(ca + 13));
		else
            sprintf(szReturn, "%sValue=0$", szReturn);

		if(ca = strstr(strResult, "Return Code2: "))
			sprintf(szReturn, "%sValue2=%ld$", szReturn, atol(ca + 14));
		else
            sprintf(szReturn, "%sValue2=0$", szReturn);

		if(ca = strstr(strResult, "Return Code3: "))
		{
			char tmp[1024] = {0};

			char *cb = strchr(ca, '\r');
			if(cb)
				strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
			else
			{
				cb = strchr(ca, '\n');
				if(cb)
					strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
				else
					strncpy(tmp, ca + 14, strlen(ca + 14) < 1024 ? strlen(ca + 14) : 1023);
			}

			sprintf(szReturn, "%sValue3=%s$", szReturn, tmp);
		}
		else
            sprintf(szReturn, "%sValue3=0$", szReturn);

		if(ca = strstr(strResult, "Return Code4: "))
		{
			char tmp[1024] = {0};

			char *cb = strchr(ca, '\r');
			if(cb)
				strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
			else
			{
				cb = strchr(ca, '\n');
				if(cb)
					strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
				else
					strncpy(tmp, ca + 14, strlen(ca + 14) < 1024 ? strlen(ca + 14) : 1023);
			}

			sprintf(szReturn, "%sValue4=%s$", szReturn, tmp);
		}
		else
            sprintf(szReturn, "%sValue4=0$", szReturn);
		}
		return TRUE;
	case DragonFlow_Timeout:
		sprintf(szReturn, "error=%scontent=%s$", FuncGetStringFromIDS("<%IDS_SSHMonitor_9%>"), strResult);    // <%IDS_SSHMonitor_9%>
		break;
	case DragonFlow_OpenConnection:
		sprintf(szReturn, "error=%scontent=%s$", FuncGetStringFromIDS("<%IDS_SSHMonitor_10%>"), strResult);   //  <%IDS_SSHMonitor_10%>
		break;
	case DragonFlow_CreateOutputThread:
	case DragonFlow_CreateErrorOutputThread:
	case DragonFlow_CreateInputThread:
	case DragonFlow_WriteToStandardOutput:
	case DragonFlow_OpenCommandFile:
	case DragonFlow_NotEnoughSpace:
	case DragonFlow_InitialiseWinSock:
	case DragonFlow_WinSockVersionIncompatible:
		sprintf(szReturn, "error=%scontent=%s$", FuncGetStringFromIDS("<%IDS_SSHMonitor_11%>"), strResult);   //  <%IDS_SSHMonitor_11%>
		break;
	case DragonFlow_FatalError:
		sprintf(szReturn, "error=%scontent=%s$", FuncGetStringFromIDS("<%IDS_SSHMonitor_12%>"), strResult);    //  <%IDS_SSHMonitor_12%>
		break;
	default:
		sprintf(szReturn, "error=%scontent=%s$", FuncGetStringFromIDS("<%IDS_SSHMonitor_13%>"), strResult);    //  <%IDS_SSHMonitor_13%>
		break;
	}

	return FALSE;
}

CString SSHGETSCRIPT(char *server, int port, char *uid, char *pwd, 
					 char *inifile, char *custpath)
{
	CString	strCmdLine = _T(""), 
			strResult = _T("");
	int		nResult = 0;

	char	iniFile[255] = {0};
	char	cmd[255] = {0};

	GetIniFileName(inifile, iniFile);

	if(!RETURN_COMMAND(FILE_TYPE_MONITOR, iniFile, cmd))
	{
//		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SSHMonitor_14%>"));   //  <%IDS_SSHMonitor_14%>
		return _T("");
	}

	//---------------------------------------------------------------------------------------------------------
	//获取脚本的绝对路径！( zouXiao 2008-12-25 )
	//---------------------------------------------------------------------------------------------------------
	char scriptsPath[1024] = {0};

	if( DFN_GetPrivateProfileString( inifile, "scripts_path", "", scriptsPath, 1024, "oscmd.ini" ) <= 0 )
	{
		//sprintf( szReturn, "error=%s", "获取脚本路径失败！" );
        return _T("");	
	}
	if ( scriptsPath[0] != '/' )
	{
		//sprintf( szReturn, "error=%s", "路径不是绝对路径！" );
        return _T("");
	}

	sprintf( cmd, "%s %s", cmd, scriptsPath );
	//strncat(cmd, " scripts", 8);

	strCmdLine.Format("%s \"-ssh\"  \"-P\" \"%d\" \"-pw\" \"%s\" \"%s@%s\" \"%s\"", PLINK_PATH,  port, pwd, uid, server, cmd);
	nResult = executeExeFile(strCmdLine, strResult);
	switch(nResult)
	{
	case DragonFlow_OK:
		return strResult;
	case DragonFlow_Timeout:
//		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SSHMonitor_15%>"));     //  <%IDS_SSHMonitor_15%>
		break;
	case DragonFlow_OpenConnection:
//		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SSHMonitor_16%>"));    //  <%IDS_SSHMonitor_16%>
		break;
	case DragonFlow_CreateOutputThread:
	case DragonFlow_CreateErrorOutputThread:
	case DragonFlow_CreateInputThread:
	case DragonFlow_WriteToStandardOutput:
	case DragonFlow_OpenCommandFile:
	case DragonFlow_NotEnoughSpace:
	case DragonFlow_InitialiseWinSock:
	case DragonFlow_WinSockVersionIncompatible:
//		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SSHMonitor_17%>"));    //  <%IDS_SSHMonitor_17%>
		break;
	case DragonFlow_FatalError:
//		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SSHMonitor_18%>"));     //  <%IDS_SSHMonitor_18%>
		break;
	default:
//		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SSHMonitor_19%>"));    //  <%IDS_SSHMonitor_19%>
		break;
	}

	return _T("");
}

BOOL HTTPEXESCRIPT(char *url, char *uid, char *pwd, char *pserver, int pport, 
				   char *puid, char *ppwd, char *inifile, char *script, char *params, 
				   char *match, char *custpath, char *szReturn)
{
	char	iniFile[255] = {0};
	char	cmd[256] = {0};
	DWORD	dwbTime = 0, dweTime = 0;
	char	msg[8192] = {0};

	dwbTime = ::GetTickCount();

	//---------------------------------------------------------------------------------------------------------
	//获取脚本的绝对路径！( zouXiao 2008-12-25 )
	//---------------------------------------------------------------------------------------------------------
	char scriptsPath[1024] = {0};

	if( DFN_GetPrivateProfileString( inifile, "scripts_path", "", scriptsPath, 1024, "oscmd.ini" ) <= 0 )
	{
		sprintf( szReturn, "error=%s", "获取脚本路径失败！" );
        return FALSE;
	}
	if ( scriptsPath[0] != '/' )
	{
		sprintf( szReturn, "error=%s", "路径不是绝对路径！" );
        return FALSE;
	}

    sprintf(cmd, "%s/%s %s", scriptsPath, script, params);
	//sprintf(cmd, "./scripts/%s %s", script, params);
	
	memset(msg, 0, sizeof(msg));
	if(!http_command(url, cmd, READ_TIMEOUT, uid, pwd, pserver, pport, puid, ppwd, msg))
	{
		sprintf(szReturn, "error=%s", "send command failed.");
		return FALSE;
	}

	dweTime = ::GetTickCount();

	if(*match)
	{
		if(!strstr(msg, match))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_30%>"));    //  <%IDS_TelMonitor_30%>
			return FALSE;
		}
	}

	sprintf(szReturn, "roundTripTime=%ld$", dweTime - dwbTime);

	{
	char *ca = NULL;

	if(ca = strstr(msg, "Return Code: "))
		sprintf(szReturn, "%sValue=%ld$", szReturn, atol(ca + 13));

	if(ca = strstr(msg, "Return Code2: "))
		sprintf(szReturn, "%sValue2=%ld$", szReturn, atol(ca + 14));

	if(ca = strstr(msg, "Return Code3: "))
	{
		char tmp[1024] = {0};

		char *cb = strchr(ca, '\r');
		if(cb)
			strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
		else
		{
			cb = strchr(ca, '\n');
			if(cb)
				strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
			else
				strncpy(tmp, ca + 14, strlen(ca + 14) < 1024 ? strlen(ca + 14) : 1023);
		}

		sprintf(szReturn, "%sValue3=%s$", szReturn, tmp);
	}

	if(ca = strstr(msg, "Return Code4: "))
	{
		char tmp[1024] = {0};

		char *cb = strchr(ca, '\r');
		if(cb)
			strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
		else
		{
			cb = strchr(ca, '\n');
			if(cb)
				strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
			else
				strncpy(tmp, ca + 14, strlen(ca + 14) < 1024 ? strlen(ca + 14) : 1023);
		}

		sprintf(szReturn, "%sValue4=%s$", szReturn, tmp);
	}
	}

	return TRUE;
}

CString HTTPGETSCRIPT(char *url, char *uid, char *pwd, char *pserver, int pport, 
					  char *puid, char *ppwd, char *inifile, char *custpath)
{
	char	iniFile[256] = {0};
	char	cmd[256] = {0};
	char	msg[8192] = {0};

	GetIniFileName(inifile, iniFile);
	
	if(!RETURN_COMMAND(FILE_TYPE_MONITOR, iniFile, cmd))
	{
//		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_44%>"));     //  <%IDS_TelMonitor_44%>
		return _T("");
	}

	//---------------------------------------------------------------------------------------------------------
	//获取脚本的绝对路径！( zouXiao 2008-12-25 )
	//---------------------------------------------------------------------------------------------------------
	char scriptsPath[1024] = {0};

	if( DFN_GetPrivateProfileString( inifile, "scripts_path", "", scriptsPath, 1024, "oscmd.ini" ) <= 0 )
	{
		//sprintf( szReturn, "error=%s", "获取脚本路径失败！" );
        return _T("");	
	}
	if ( scriptsPath[0] != '/' )
	{
		//sprintf( szReturn, "error=%s", "路径不是绝对路径！" );
        return _T("");
	}

	sprintf( cmd, "%s %s", cmd, scriptsPath );
	//strncat(cmd, " scripts", 8);

	memset(msg, 0, sizeof(msg));
	if(!http_command(url, cmd, READ_TIMEOUT, uid, pwd, pserver, pport, puid, ppwd, msg))
	{
//		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_45%>"));    //  <%IDS_TelMonitor_45%>
		return _T("");
	}

	return msg;
}

BOOL RLOGINEXESCRIPT(char *server, int port, char *uid, char *pwd, 
					 char *pserver, int pport, char *puid, char *ppwd, 
				     char *pwdprompt, char *prompts, 
				     char *inifile, char *script, char *params, char *match, 
				     char *custpath, char *szReturn)
{
	int		ret = 0;
	int		hSocket = 0;
	BOOL	bResult = TRUE;
	char	iniFile[255] = {0};
	char	cmd[255] = {0};
	DWORD	dwbTime = 0, dweTime = 0;

	/*************************************
	 * For Reason Of Prompt
	 */
	int		c = 0;
	char	*ca = prompts, *cb = NULL;
	char	arprompt[PR_NUM][256];
	memset(arprompt, 0, sizeof(arprompt));

	while(cb = strchr(ca, 0x20))
	{
		if(ca == cb) 
		{
			ca ++;
			continue;
		}

		strncpy(arprompt[c++], ca, cb - ca);
		ca = cb + 1;
	}

	strcpy(arprompt[c++], ca);
	/***********************************/

	dwbTime = ::GetTickCount();

	hSocket = rlogin_init(server, port, pserver, pport, puid, ppwd);
	if(hSocket <= 0)
	{
		if(hSocket == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_17%>"));    //  <%IDS_TelMonitor_17%>
		else if(hSocket == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_18%>"));   //  <%IDS_TelMonitor_18%>
		else if(hSocket == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_19%>"));     //  <%IDS_TelMonitor_19%>
		else if(hSocket == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_20%>"));     //  <%IDS_TelMonitor_20%>
		else if(hSocket == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_21%>"));     //  <%IDS_TelMonitor_21%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_22%>"));   //  <%IDS_TelMonitor_22%>

		bResult = FALSE;
		goto err;
	}

	if((ret = rlogin_connect(hSocket, uid, pwd, pwdprompt, arprompt)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_23%>"));    //  <%IDS_TelMonitor_23%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_24%>"));    //  <%IDS_TelMonitor_24%>
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_25%>"));    //  <%IDS_TelMonitor_25%>
		else if(ret == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_26%>"));   //  <%IDS_TelMonitor_26%>
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_27%>"));    //  <%IDS_TelMonitor_27%>
		else if(ret == -6)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_28%>"));   //  <%IDS_TelMonitor_28%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_29%>"));    //  <%IDS_TelMonitor_29%>

		bResult = FALSE;
		goto err;

	}


	//---------------------------------------------------------------------------------------------------------
	//获取脚本的绝对路径！( zouXiao 2008-12-25 )
	//---------------------------------------------------------------------------------------------------------
	char scriptsPath[1024] = {0};

	if( DFN_GetPrivateProfileString( inifile, "scripts_path", "", scriptsPath, 1024, "oscmd.ini" ) <= 0 )
	{
		sprintf( szReturn, "error=%s", "获取脚本路径失败！" );
        bResult = FALSE;
		goto err;

	}
	if ( scriptsPath[0] != '/' )
	{
		sprintf( szReturn, "error=%s", "路径不是绝对路径！" );
        bResult = FALSE;
		goto err;
	}

    sprintf(cmd, "%s/%s %s", scriptsPath, script, params);
	//sprintf(cmd, "./scripts/%s %s", script, params);
	if((ret = rlogin_command(hSocket, cmd)) < 0)
	{
		sprintf(szReturn, "error=%s", "send command failed.");
		bResult = FALSE;
		goto err;
	}

	dweTime = ::GetTickCount();

	if(*match)
	{
		if(!strstr(rlogin_databuffer, match))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_30%>"));    //  <%IDS_TelMonitor_30%>
			bResult = FALSE;
			goto err;
		}
	}

	sprintf(szReturn, "roundTripTime=%ld$", dweTime - dwbTime);

	{
	char *ca = NULL;

	if(ca = strstr(rlogin_databuffer, "Return Code: "))
		sprintf(szReturn, "%sValue=%ld$", szReturn, atol(ca + 13));

	if(ca = strstr(rlogin_databuffer, "Return Code2: "))
		sprintf(szReturn, "%sValue2=%ld$", szReturn, atol(ca + 14));

	if(ca = strstr(rlogin_databuffer, "Return Code3: "))
	{
		char tmp[1024] = {0};

		char *cb = strchr(ca, '\r');
		if(cb)
			strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
		else
		{
			cb = strchr(ca, '\n');
			if(cb)
				strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
			else
				strncpy(tmp, ca + 14, strlen(ca + 14) < 1024 ? strlen(ca + 14) : 1023);
		}

		sprintf(szReturn, "%sValue3=%s$", szReturn, tmp);
	}

	if(ca = strstr(rlogin_databuffer, "Return Code4: "))
	{
		char tmp[1024] = {0};

		char *cb = strchr(ca, '\r');
		if(cb)
			strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
		else
		{
			cb = strchr(ca, '\n');
			if(cb)
				strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
			else
				strncpy(tmp, ca + 14, strlen(ca + 14) < 1024 ? strlen(ca + 14) : 1023);
		}

		sprintf(szReturn, "%sValue4=%s$", szReturn, tmp);
	}
	}


err:
	shutdown(hSocket, 0x02);
	closesocket(hSocket);
	return bResult;
}

CString RLOGINGETSCRIPT(char *server, int port, char *uid, char *pwd, 
					    char *pserver, int pport, char *puid, char *ppwd, 
					    char *pwdprompt, char *prompts, 
					    char *inifile, char *custpath)
{
	int		ret = 0;
	int		hSocket = 0;
	char	iniFile[255] = {0};
	char	cmd[255] = {0};

	/*************************************
	 * For Reason Of Prompt
	 */
	int		c = 0;
	char	*ca = prompts, *cb = NULL;
	char	arprompt[PR_NUM][256];
	memset(arprompt, 0, sizeof(arprompt));

	while(cb = strchr(ca, 0x20))
	{
		if(ca == cb) 
		{
			ca ++;
			continue;
		}

		strncpy(arprompt[c++], ca, cb - ca);
		ca = cb + 1;
	}

	strcpy(arprompt[c++], ca);
	/***********************************/

	hSocket = rlogin_init(server, port, pserver, pport, puid, ppwd);
	if(hSocket <= 0)
	{
/*		if(hSocket == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_31%>"));    //  <%IDS_TelMonitor_31%>
		else if(hSocket == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_32%>"));    //  <%IDS_TelMonitor_32%>
		else if(hSocket == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_33%>"));      //  <%IDS_TelMonitor_33%>
		else if(hSocket == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_34%>"));      //  <%IDS_TelMonitor_34%>
		else if(hSocket == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_35%>"));    //  <%IDS_TelMonitor_35%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_36%>"));   //  <%IDS_TelMonitor_36%>
*/		
		shutdown(hSocket, 0x02);
		closesocket(hSocket);
		return _T("");
	}

	if((ret = rlogin_connect(hSocket, uid, pwd, pwdprompt, arprompt)) < 0)
	{
/*		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_37%>"));    //  <%IDS_TelMonitor_37%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_38%>"));    //  <%IDS_TelMonitor_38%>
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_39%>"));    //  <%IDS_TelMonitor_39%>
		else if(ret == -4) 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_40%>"));   //  <%IDS_TelMonitor_40%>
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_41%>"));   //  <%IDS_TelMonitor_41%>
		else if(ret == -6)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_42%>"));   //  <%IDS_TelMonitor_42%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_43%>"));    //  <%IDS_TelMonitor_43%>
*/
		shutdown(hSocket, 0x02);
		closesocket(hSocket);
		return _T("");

	}

	GetIniFileName(inifile, iniFile);
	
	if(!RETURN_COMMAND(FILE_TYPE_MONITOR, iniFile, cmd))
	{
//		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_44%>"));     //  <%IDS_TelMonitor_44%>
		shutdown(hSocket, 0x02);
		closesocket(hSocket);
		return _T("");
	}

	//---------------------------------------------------------------------------------------------------------
	//获取脚本的绝对路径！( zouXiao 2008-12-25 )
	//---------------------------------------------------------------------------------------------------------
	char scriptsPath[1024] = {0};

	if( DFN_GetPrivateProfileString( inifile, "scripts_path", "", scriptsPath, 1024, "oscmd.ini" ) <= 0 )
	{
		//sprintf( szReturn, "error=%s", "获取脚本路径失败！" );
        shutdown(hSocket, 0x02);
		closesocket(hSocket);
		return _T("");	
	}
	if ( scriptsPath[0] != '/' )
	{
		//sprintf( szReturn, "error=%s", "路径不是绝对路径！" );
        shutdown(hSocket, 0x02);
		closesocket(hSocket);
		return _T("");
	}

	sprintf( cmd, "%s %s", cmd, scriptsPath );
	//strncat(cmd, " scripts", 8);
	if((ret = rlogin_command(hSocket, cmd)) < 0)
	{
//		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_45%>"));    //  <%IDS_TelMonitor_45%>
		shutdown(hSocket, 0x02);
		closesocket(hSocket);
		return _T("");
	}

	shutdown(hSocket, 0x02);
	closesocket(hSocket);
	return rlogin_databuffer;
}
BOOL PaseString2(CString strScr,CStringList&strList)
{
	strList.RemoveAll();
	int pos=strScr.Find(',');
	while(pos>-1)
	{
		strList.AddTail(strScr.SpanExcluding(","));
		strScr=strScr.Right(strScr.GetLength()-pos-1);
		pos=strScr.Find(',');
	}
	if(strScr.GetLength()>0)
		strList.AddTail(strScr);
	return TRUE;
}
BOOL TELEXESCRIPT2(char *server, int port, char *uid, char *pwd, 
				  char *proxy_server, int proxy_port, char *proxy_uid, char *proxy_pwd, 
				  char *logprompt, char *pwdprompt, char *prompts, 
				  char *inifile, char *script, char *params, char *match, 
				  char *custpath, char *szReturn)
{
	int		ret = 0;
	int		hSocket = 0;
	BOOL	bResult = TRUE;
	char	iniFile[255] = {0};
	char	cmd[255] = {0};
	DWORD	dwbTime = 0, dweTime = 0;

	/*************************************
	 * For Reason Of Prompt
	 */
	int		c = 0;
	char	*ca = prompts, *cb = NULL;
	char	arprompt[PR_NUM][256];
	memset(arprompt, 0, sizeof(arprompt));

	CString strS=_T("");
	CString strT=_T("");
	CStringList strList;
	int count=0;
    POSITION pos=0;

	LPGDATA pgd=new GDATA;
	memset(pgd,0,sizeof(GDATA));




	while(cb = strchr(ca, 0x20))
	{
		if(ca == cb) 
		{
			ca ++;
			continue;
		}

		strncpy(arprompt[c++], ca, cb - ca);
		ca = cb + 1;
	}

	strcpy(arprompt[c++], ca);
	/***********************************/

	dwbTime = ::GetTickCount();

	hSocket = telnet_init(server, port, proxy_server, proxy_port, proxy_uid, proxy_pwd);
	if(hSocket <= 0)
	{
		if(hSocket == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_17%>"));    //  <%IDS_TelMonitor_17%>
		else if(hSocket == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_18%>"));   //  <%IDS_TelMonitor_18%>
		else if(hSocket == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_19%>"));     //  <%IDS_TelMonitor_19%>
		else if(hSocket == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_20%>"));     //  <%IDS_TelMonitor_20%>
		else if(hSocket == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_21%>"));     //  <%IDS_TelMonitor_21%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_22%>"));   //  <%IDS_TelMonitor_22%>

		bResult = FALSE;
		goto err;
	}

	if((ret = telnet_connect(hSocket, uid, pwd, logprompt, pwdprompt, arprompt,pgd)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_23%>"));    //  <%IDS_TelMonitor_23%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_24%>"));    //  <%IDS_TelMonitor_24%>
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_25%>"));    //  <%IDS_TelMonitor_25%>
		else if(ret == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_26%>"));   //  <%IDS_TelMonitor_26%>
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_27%>"));    //  <%IDS_TelMonitor_27%>
		else if(ret == -6)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_28%>"));   //  <%IDS_TelMonitor_28%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_29%>"));    //  <%IDS_TelMonitor_29%>

		bResult = FALSE;
		goto err;

	}

//	sprintf(cmd, "/usr/bin/vmstat 3 3"/*"./scripts/%s %s", script, params*/);
	strcpy(cmd,script);
	if((ret = telnet_command(hSocket, cmd,pgd)) < 0)
	{
		sprintf(szReturn, "error=%s", "send command failed.");
		bResult = FALSE;
		goto err;
	}

	dweTime = ::GetTickCount();


	PaseString2(params,strList);

	pos=strList.GetHeadPosition();
	strS=pgd->databuffer;
	while(pos)
	{
		strT=strList.GetNext(pos);
		strS.MakeLower();
		strT.MakeLower();
		if(strS.Find(strT)>-1)
			count++;
	}

	if(count>0)
		sprintf(szReturn,"status=1$TotalCount=%d$",count);
	else
		sprintf(szReturn,"status=0$TotalCount=0$");



/*	if(*match)
	{
		if(!strstr(databuffer, match))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_30%>"));    //  <%IDS_TelMonitor_30%>
			bResult = FALSE;
			goto err;
		}
	}

	sprintf(szReturn, "roundTripTime=%ld$", dweTime - dwbTime);

	{
	char *ca = NULL;

	if(ca = strstr(databuffer, "Return Code: "))
		sprintf(szReturn, "%sValue=%ld$", szReturn, atol(ca + 13));

	if(ca = strstr(databuffer, "Return Code2: "))
		sprintf(szReturn, "%sValue2=%ld$", szReturn, atol(ca + 14));

	if(ca = strstr(databuffer, "Return Code3: "))
	{
		char tmp[1024] = {0};

		char *cb = strchr(ca, '\r');
		if(cb)
			strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
		else
		{
			cb = strchr(ca, '\n');
			if(cb)
				strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
			else
				strncpy(tmp, ca + 14, strlen(ca + 14) < 1024 ? strlen(ca + 14) : 1023);
		}

		sprintf(szReturn, "%sValue3=%s$", szReturn, tmp);
	}

	if(ca = strstr(databuffer, "Return Code4: "))
	{
		char tmp[1024] = {0};

		char *cb = strchr(ca, '\r');
		if(cb)
			strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
		else
		{
			cb = strchr(ca, '\n');
			if(cb)
				strncpy(tmp, ca + 14, cb - ca - 14 < 1024 ? cb - ca - 14 : 1023);
			else
				strncpy(tmp, ca + 14, strlen(ca + 14) < 1024 ? strlen(ca + 14) : 1023);
		}

		sprintf(szReturn, "%sValue4=%s$", szReturn, tmp);
	}
	}*/


err:
	delete pgd;
	shutdown(hSocket, 0x02);
	closesocket(hSocket);
	return bResult;
}