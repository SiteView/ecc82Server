
#include "Cluster.h"
#include "TelMonitor.h"
#include "SSHMonitor.h"
#include "HttpMonitor.h"
#include "RloginMonitor.h"

#include <Base\funcGeneral.h>

//extern char	databuffer[BUF_SIZE];
//extern int	datalen;

extern char	rlogin_databuffer[BUF_SIZE];
extern int	rlogin_datalen;

BOOL TELCLUSTER_MONITOR(char *server, int port, char *uid, char *pwd, char *pserver, 
		int pport, char *puid, char *ppwd, char *lprompt, char *pprompt, char *prompts, 
		char *ostype, char *clusterlog, char *cpath, char *szReturn)
{
	int		ret = 0;
	int		hSocket = 0;
	BOOL	bResult = TRUE;
	char	iniFile[256] = {0};
	char	cmd[256] = {0};

	/*************************************
	 * For Reason Of Prompt
	 */
	int		c = 0;
	char	*ca = prompts, *cb = NULL;
	char	arprompt[PR_NUM][256];
	memset(arprompt, 0, sizeof(arprompt));

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

	hSocket = telnet_init(server, port, pserver, pport, puid, ppwd);
	if(hSocket <= 0)
	{
		if(hSocket == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_1%>"));    //  <%IDS_TelMonitor_1%>
		else if(hSocket == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_2%>"));   //  <%IDS_TelMonitor_2%>
		else if(hSocket == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_3%>"));     //  <%IDS_TelMonitor_3%>
		else if(hSocket == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_4%>"));     //  <%IDS_TelMonitor_4%>
		else if(hSocket == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_5%>"));        //  <%IDS_TelMonitor_5%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_6%>"));     //  <%IDS_TelMonitor_6%>

		bResult = FALSE;
		goto err;
	}

	if((ret = telnet_connect(hSocket, uid, pwd, lprompt, pprompt, arprompt,pgd)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_7%>"));    //  <%IDS_TelMonitor_7%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_8%>"));     //  <%IDS_TelMonitor_8%>
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_9%>"));     //  <%IDS_TelMonitor_9%>
		else if(ret == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_10%>"));   //  <%IDS_TelMonitor_10%>
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_11%>"));   //  <%IDS_TelMonitor_11%>
		else if(ret == -6)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_12%>"));   //  <%IDS_TelMonitor_12%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_13%>"));     //  <%IDS_TelMonitor_13%>

		bResult = FALSE;
		goto err;

	}
#if _DEBUG
	sprintf(iniFile, "%s\\templates.os\\%s", FuncGetInstallRootPath(), ostype);
#else
	sprintf(iniFile, "%s\\MonitorManager\\templates.os\\%s", FuncGetInstallRootPath(), ostype);
#endif
	DFN_GetPrivateProfileString("cat", "command", "", cmd, sizeof(cmd), iniFile);	
	if(!*cmd)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_14%>"));    //  <%IDS_TelMonitor_14%>
		bResult = FALSE;
		goto err;
	}

	if(ca = strstr(cmd, "<file>"))
	{
		strcpy(ca, clusterlog);
		*(ca + strlen(clusterlog)) = 0;
	}

	if((ret = telnet_command(hSocket, cmd,pgd)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_15%>"));    //  <%IDS_TelMonitor_15%>
		bResult = FALSE;
		goto err;
	}

	{
	int index = 0;
	CString	strBuffer(pgd->databuffer);
	CString	strLine = _T("");

	// Cluster
	if((index = strBuffer.Find("CLUSTER", 0)) >= 0)
	{
		strBuffer.Delete(0, index);
	}
	else
	{
		sprintf(szReturn, "error=%s", "No Cluster");	// No Cluster
		bResult = FALSE;
		goto err;
	}

	strLine = GetStringFromLineNo(strBuffer, 2);
	sprintf(szReturn, "%sClusterName=%s$ClusterStatus=%s$", szReturn, 
		GetColumnFromLine(strLine, 1), GetColumnFromLine(strLine, 2));

	// Node
	if((index = strBuffer.Find("NODE", 0)) >= 0)
	{
		strBuffer.Delete(0, index);
	}
	else
	{
		sprintf(szReturn, "error=%s", "No Node");	// No Node
		bResult = FALSE;
		goto err;
	}

	strLine = GetStringFromLineNo(strBuffer, 2);
	sprintf(szReturn, "%sNodeName=%s$NodeStatus=%s$NodeState=%s$", szReturn, 
		GetColumnFromLine(strLine, 1), GetColumnFromLine(strLine, 2), 
		GetColumnFromLine(strLine, 3));

	// Package
	if((index = strBuffer.Find("PACKAGE", 0)) >= 0)
	{
		strBuffer.Delete(0, index);
	}
	else
	{
		sprintf(szReturn, "error=%s", "No Package");	// No Package
		bResult = FALSE;
		goto err;
	}

	strLine = GetStringFromLineNo(strBuffer, 2);
	sprintf(szReturn, "%sPackageName=%s$PackageStatus=%s$PackageState=%s$PackageAutorun=%s$", 
		szReturn, GetColumnFromLine(strLine, 1), GetColumnFromLine(strLine, 2), 
		GetColumnFromLine(strLine, 3), GetColumnFromLine(strLine, 4));
	}

err:
	shutdown(hSocket, 0x02);
	closesocket(hSocket);
	delete pgd;
	return bResult;
}

BOOL SSHCLUSTER_MONITOR(char *server, int port, char *uid, char *pwd, char *ostype, 
		char *clusterlog, char *cpath, char *szReturn)
{
	CString	strCmdLine = _T(""), 
			strResult = _T("");
	int		nResult = 0;

	char	*ca = NULL;
	char	iniFile[256] = {0};
	char	cmd[256] = {0};

#if _DEBUG
	sprintf(iniFile, "%s\\templates.os\\%s", FuncGetInstallRootPath(), ostype);
#else
	sprintf(iniFile, "%s\\MonitorManager\\templates.os\\%s", FuncGetInstallRootPath(), ostype);
#endif
	DFN_GetPrivateProfileString("cat", "command", "", cmd, sizeof(cmd), iniFile);
	if(!*cmd)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SSHMonitor_1%>"));   //  <%IDS_SSHMonitor_1%>
		return FALSE;
	}

	if(ca = strstr(cmd, "<file>"))
	{
		strcpy(ca, clusterlog);
		*(ca + strlen(clusterlog)) = 0;
	}

	strCmdLine.Format("%s \"-ssh\" \"-t\" \"%d\" \"-P\" \"%d\" \"-pw\" \"%s\" \"%s@%s\" \"%s\"", PLINK_PATH, CALL_TIMEOUT, port, pwd, uid, server, cmd);
	nResult = executeExeFile(strCmdLine, strResult);
	switch(nResult)
	{
	case DragonFlow_OK:
	{
		int index = 0;
		CString	strBuffer = strResult;
		CString	strLine = _T("");

		// Cluster
		if((index = strBuffer.Find("CLUSTER", 0)) >= 0)
		{
			strBuffer.Delete(0, index);
		}
		else
		{
			sprintf(szReturn, "error=%s", "No Cluster");	// No Cluster
			return FALSE;
		}

		strLine = GetStringFromLineNo(strBuffer, 2);
		sprintf(szReturn, "%sClusterName=%s$ClusterStatus=%s$", szReturn, 
			GetColumnFromLine(strLine, 1), GetColumnFromLine(strLine, 2));

		// Node
		if((index = strBuffer.Find("NODE", 0)) >= 0)
		{
			strBuffer.Delete(0, index);
		}
		else
		{
			sprintf(szReturn, "error=%s", "No Node");	// No Node
			return FALSE;
		}

		strLine = GetStringFromLineNo(strBuffer, 2);
		sprintf(szReturn, "%sNodeName=%s$NodeStatus=%s$NodeState=%s$", szReturn, 
			GetColumnFromLine(strLine, 1), GetColumnFromLine(strLine, 2), 
			GetColumnFromLine(strLine, 3));

		// Package
		if((index = strBuffer.Find("PACKAGE", 0)) >= 0)
		{
			strBuffer.Delete(0, index);
		}
		else
		{
			sprintf(szReturn, "error=%s", "No Package");	// No Package
			return FALSE;
		}

		strLine = GetStringFromLineNo(strBuffer, 2);
		sprintf(szReturn, "%sPackageName=%s$PackageStatus=%s$PackageState=%s$PackageAutorun=%s$", 
			szReturn, GetColumnFromLine(strLine, 1), GetColumnFromLine(strLine, 2), 
			GetColumnFromLine(strLine, 3), GetColumnFromLine(strLine, 4));
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
	default:
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_SSHMonitor_7%>"));    //  <%IDS_SSHMonitor_7%>
		break;
	}

	return FALSE;
}

BOOL HTTPCLUSTER_MONITOR(char *url, char *uid, char *pwd, char *pserver, int pport, 
		char *puid, char *ppwd, char *ostype, char *clusterlog, char *cpath, 
		char *szReturn)
{
	int		ret = 0;
	BOOL	bResult = TRUE;
	char	*ca = NULL;
	char	iniFile[256] = {0};
	char	cmd[256] = {0};

#if _DEBUG
	sprintf(iniFile, "%s\\templates.os\\%s", FuncGetInstallRootPath(), ostype);
#else
	sprintf(iniFile, "%s\\MonitorManager\\templates.os\\%s", FuncGetInstallRootPath(), ostype);
#endif
	DFN_GetPrivateProfileString("cat", "command", "", cmd, sizeof(cmd), iniFile);	
	if(!*cmd)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_14%>"));    //  <%IDS_TelMonitor_14%>
		return FALSE;
	}

	if(ca = strstr(cmd, "<file>"))
	{
		strcpy(ca, clusterlog);
		*(ca + strlen(clusterlog)) = 0;
	}

	char msg[100 * 1024] = {0};
	if(!http_command(url, cmd, READ_TIMEOUT, uid, pwd, pserver, pport, puid, ppwd, msg))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_15%>"));    //  <%IDS_TelMonitor_15%>
		return FALSE;
	}

	{
	int index = 0;
	CString	strBuffer(msg);
	CString	strLine = _T("");

	// Cluster
	if((index = strBuffer.Find("CLUSTER", 0)) >= 0)
	{
		strBuffer.Delete(0, index);
	}
	else
	{
		sprintf(szReturn, "error=%s", "No Cluster");	// No Cluster
		return FALSE;
	}

	strLine = GetStringFromLineNo(strBuffer, 2);
	sprintf(szReturn, "%sClusterName=%s$ClusterStatus=%s$", szReturn, 
		GetColumnFromLine(strLine, 1), GetColumnFromLine(strLine, 2));

	// Node
	if((index = strBuffer.Find("NODE", 0)) >= 0)
	{
		strBuffer.Delete(0, index);
	}
	else
	{
		sprintf(szReturn, "error=%s", "No Node");	// No Node
		return FALSE;
	}

	strLine = GetStringFromLineNo(strBuffer, 2);
	sprintf(szReturn, "%sNodeName=%s$NodeStatus=%s$NodeState=%s$", szReturn, 
		GetColumnFromLine(strLine, 1), GetColumnFromLine(strLine, 2), 
		GetColumnFromLine(strLine, 3));

	// Package
	if((index = strBuffer.Find("PACKAGE", 0)) >= 0)
	{
		strBuffer.Delete(0, index);
	}
	else
	{
		sprintf(szReturn, "error=%s", "No Package");	// No Package
		return FALSE;
	}

	strLine = GetStringFromLineNo(strBuffer, 2);
	sprintf(szReturn, "%sPackageName=%s$PackageStatus=%s$PackageState=%s$PackageAutorun=%s$", 
		szReturn, GetColumnFromLine(strLine, 1), GetColumnFromLine(strLine, 2), 
		GetColumnFromLine(strLine, 3), GetColumnFromLine(strLine, 4));
	}

	return TRUE;
}

BOOL RLOGINCLUSTER_MONITOR(char *server, int port, char *uid, char *pwd, char *pserver, 
		int pport, char *puid, char *ppwd, char *pprompt, char *prompts, char *ostype, 
		char *clusterlog, char *cpath, char *szReturn)
{
	int		ret = 0;
	int		hSocket = 0;
	BOOL	bResult = TRUE;
	char	iniFile[256] = {0};
	char	cmd[256] = {0};

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
		if(hSocket == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_1%>"));    //  <%IDS_TelMonitor_1%>
		else if(hSocket == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_2%>"));   //  <%IDS_TelMonitor_2%>
		else if(hSocket == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_3%>"));     //  <%IDS_TelMonitor_3%>
		else if(hSocket == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_4%>"));     //  <%IDS_TelMonitor_4%>
		else if(hSocket == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_5%>"));        //  <%IDS_TelMonitor_5%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_6%>"));     //  <%IDS_TelMonitor_6%>

		bResult = FALSE;
		goto err;
	}

	if((ret = rlogin_connect(hSocket, uid, pwd, pprompt, arprompt)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_7%>"));    //  <%IDS_TelMonitor_7%>
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_8%>"));     //  <%IDS_TelMonitor_8%>
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_9%>"));     //  <%IDS_TelMonitor_9%>
		else if(ret == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_10%>"));   //  <%IDS_TelMonitor_10%>
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_11%>"));   //  <%IDS_TelMonitor_11%>
		else if(ret == -6)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_12%>"));   //  <%IDS_TelMonitor_12%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_13%>"));     //  <%IDS_TelMonitor_13%>

		bResult = FALSE;
		goto err;

	}

#if _DEBUG
	sprintf(iniFile, "%s\\templates.os\\%s", FuncGetInstallRootPath(), ostype);
#else
	sprintf(iniFile, "%s\\MonitorManager\\templates.os\\%s", FuncGetInstallRootPath(), ostype);
#endif
	DFN_GetPrivateProfileString("cat", "command", "", cmd, sizeof(cmd), iniFile);	
	if(!*cmd)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_14%>"));    //  <%IDS_TelMonitor_14%>
		bResult = FALSE;
		goto err;
	}

	if(ca = strstr(cmd, "<file>"))
	{
		strcpy(ca, clusterlog);
		*(ca + strlen(clusterlog)) = 0;
	}

	if((ret = rlogin_command(hSocket, cmd)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_TelMonitor_15%>"));    //  <%IDS_TelMonitor_15%>
		bResult = FALSE;
		goto err;
	}

	{
	int index = 0;
	CString	strBuffer(rlogin_databuffer);
	CString	strLine = _T("");

	// Cluster
	if((index = strBuffer.Find("CLUSTER", 0)) >= 0)
	{
		strBuffer.Delete(0, index);
	}
	else
	{
		sprintf(szReturn, "error=%s", "No Cluster");	// No Cluster
		bResult = FALSE;
		goto err;
	}

	strLine = GetStringFromLineNo(strBuffer, 2);
	sprintf(szReturn, "%sClusterName=%s$ClusterStatus=%s$", szReturn, 
		GetColumnFromLine(strLine, 1), GetColumnFromLine(strLine, 2));

	// Node
	if((index = strBuffer.Find("NODE", 0)) >= 0)
	{
		strBuffer.Delete(0, index);
	}
	else
	{
		sprintf(szReturn, "error=%s", "No Node");	// No Node
		bResult = FALSE;
		goto err;
	}

	strLine = GetStringFromLineNo(strBuffer, 2);
	sprintf(szReturn, "%sNodeName=%s$NodeStatus=%s$NodeState=%s$", szReturn, 
		GetColumnFromLine(strLine, 1), GetColumnFromLine(strLine, 2), 
		GetColumnFromLine(strLine, 3));

	// Package
	if((index = strBuffer.Find("PACKAGE", 0)) >= 0)
	{
		strBuffer.Delete(0, index);
	}
	else
	{
		sprintf(szReturn, "error=%s", "No Package");	// No Package
		bResult = FALSE;
		goto err;
	}

	strLine = GetStringFromLineNo(strBuffer, 2);
	sprintf(szReturn, "%sPackageName=%s$PackageStatus=%s$PackageState=%s$PackageAutorun=%s$", 
		szReturn, GetColumnFromLine(strLine, 1), GetColumnFromLine(strLine, 2), 
		GetColumnFromLine(strLine, 3), GetColumnFromLine(strLine, 4));
	}

err:
	shutdown(hSocket, 0x02);
	closesocket(hSocket);
	return bResult;
}

CString GetStringFromLineNo(CString strBuffer, int nLine)
{
	int		count = 0;
	int		index = 0, index2 = 0;
	CString strLine = _T("");

	while(1)
	{
		index =  strBuffer.Find("\r", 0);
		index2 = strBuffer.Find("\n", 0);

		if(index >= 0 && index2 >= 0)
		{
			if(index2 < index) index = index2;
		}
		else if(index2 >= 0 && index < 0)
		{
			index = index2;
		}
		else if(index2 < 0 && index >= 0)
		{
		}
		else
		{
			break;
		}

		strLine = strBuffer.Left(index);
		if(++ count == nLine)
		{
			break;			
		}
		else
		{
			strBuffer.Delete(0, index);
			while(strBuffer.GetAt(0) == '\r' || strBuffer.GetAt(0) == '\n')
				strBuffer.Delete(0, 1);
		}
	}

	return strLine;
}

CString GetColumnFromLine(CString strBuffer, int nColumn)
{
	int nLen = strBuffer.GetLength();
	TCHAR ch;
	CString strOut  = _T("");

	if(nColumn == 999)
	{
		int index = strBuffer.ReverseFind(' ');
		if(index >= 0)
		{
			strOut = strBuffer.Right(strBuffer.GetLength() - index - 1);
			return strOut;
		}
	}
	else
	{
		for (int i = 0; i < nLen; i++) 
		{		
			ch = strBuffer.GetAt(i);
			if (ch == 0x20)
				continue;
			nColumn--;
			strOut = strBuffer.Right(nLen - i);
			strOut = strOut.SpanExcluding(" ");

			if (nColumn == 0) 
			{
				return strOut;
			}

			i += strOut.GetLength();
		}
	}

	return _T("");
}