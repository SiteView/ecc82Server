
#include "CiscoLog.h"
#include "TelMonitor.h"
#include "Utils.h"
#include <Base\funcGeneral.h>

#ifdef WIN32
#pragma warning (disable : 4267)
#endif
//extern char databuffer[BUF_SIZE];
//extern int	datalen;

BOOL CISCOCONFIG_MONITOR(char *server, int port, char *pwd, char *enpwd, 
					  char *pwdprompt, char *prompt, char *enpwdprompt, 
					  char *enprompt, char *encmd, int groupid, int monitorid, 
					  char *proxy_server, int proxy_port, char *proxy_uid, 
					  char *proxy_pwd, char *custpath, char *szReturn)
{
	int		ret = 0;
	int		hSocket = 0;
	char runcmd[64] = {0};

    printf("Server:%s\nPort:%d\n", server, port);
	hSocket = telnet_init(server, port, proxy_server, proxy_port, proxy_uid, proxy_pwd);
	if(hSocket <= 0)
	{
		if(hSocket == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_1%>"));//<%IDS_CiscoLog_1%>
		else if(hSocket == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_2%>"));//<%IDS_CiscoLog_2%>
		else if(hSocket == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_3%>"));//<%IDS_CiscoLog_3%>
		else if(hSocket == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_4%>"));//<%IDS_CiscoLog_4%>
		else if(hSocket == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_5%>"));//<%IDS_CiscoLog_5%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_6%>"));//<%IDS_CiscoLog_6%>

		return FALSE;
	}

	LPGDATA pgd=new GDATA;
	memset(pgd,0,sizeof(GDATA));

    printf("Connect Cisio network set\n");
	if((ret = cisco_connect(hSocket, pwd, pwdprompt, prompt,pgd)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_7%>"));//<%IDS_CiscoLog_7%>
		delete pgd;
		return FALSE;
	}

	strncpy(runcmd, "enable\r\n", 8);
	send(hSocket, runcmd, strlen(runcmd), 0);
    printf("Login Cisio Config\n");
	if((ret = cisco_connect(hSocket, enpwd, enpwdprompt, enprompt,pgd)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_8%>"));//<%IDS_CiscoLog_8%>
		delete pgd;
		return FALSE;
	}
    printf("Execute command\n");
	if((ret = cisco_command(hSocket, encmd, enprompt,pgd)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_9%>"));//<%IDS_CiscoLog_9%>
		delete pgd;
		return FALSE;
	}

	int index = 0;
	char prebuffer[MAX_READ_SIZE] = {0};
	CString strBuffer(pgd->databuffer);
	
	index = strBuffer.Find("show running-config\r\n", 0);
	if(index >= 0)
		strBuffer.Delete(0, index + 21);

	if(!strBuffer.Right(strlen(enprompt)).Compare(enprompt))
	{
		index = strBuffer.ReverseFind('\r');
		if(index >= 0)
			strBuffer = strBuffer.Left(index);
	}

	FILE *cisco_fp = NULL;
	char prefile[256] = {0};

	sprintf(prefile, "%s\\cisco_config_%d_%d", custpath, groupid, monitorid);
	if((_access(prefile, 0x04)) == -1)
	{
		if((cisco_fp = fopen(prefile, "w")) == NULL)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_10%>"));//<%IDS_CiscoLog_10%>
			delete pgd;
			return FALSE;
		}

		sprintf(szReturn,"status=%s$", FuncGetStringFromIDS("<%IDS_CiscoLog_11%>"));//<%IDS_CiscoLog_11%>
		int i = fwrite(strBuffer, sizeof(char), strBuffer.GetLength(), cisco_fp);
		fclose(cisco_fp);
		delete pgd;
		return TRUE;
	}

	if((cisco_fp = fopen(prefile, "r")) == NULL)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_12%>"));//<%IDS_CiscoLog_12%>
		delete pgd;
		return FALSE;
	}

	int j = fread(prebuffer, sizeof(char), sizeof(prebuffer), cisco_fp);
	fclose(cisco_fp);
	prebuffer[j] = 0;
	int m = strlen(prebuffer);
	int n = strBuffer.GetLength();
	if(!strBuffer.Compare(prebuffer))
	{
		char buffer [256] = {0};
		char tmp [256] = {0};
#if _DEBUG
		sprintf(buffer, "%s\\temp\\Cisio_logfile_%s.ini", FuncGetInstallRootPath(), server);
#else
		sprintf(buffer, "%s\\MonitorManager\\temp\\Cisio_logfile_%s.ini", FuncGetInstallRootPath(), server);
#endif
		BOOL bChange = DFN_GetPrivateProfileInt("logfile", "needconfirm", FALSE, buffer);
		if (!bChange)
		{
			sprintf(szReturn, "status=%s$", FuncGetStringFromIDS("<%IDS_CiscoLog_13%>"));//<%IDS_CiscoLog_13%>
			delete pgd;
			return TRUE;
		}
		else
		{
			sprintf(szReturn, "status=%s$", FuncGetStringFromIDS("<%IDS_CiscoLog_14%>"));//<%IDS_CiscoLog_14%>			
			delete pgd;
			return TRUE;
		}
	}
	else
	{
		sprintf(szReturn, "status=%s$", FuncGetStringFromIDS("<%IDS_CiscoLog_14%>"));//<%IDS_CiscoLog_14%>
		if((cisco_fp = fopen(prefile, "w")) == NULL)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_15%>"));//<%IDS_CiscoLog_15%>
			delete pgd;
			return FALSE;
		}

		int k = fwrite(strBuffer, sizeof(char), strBuffer.GetLength(), cisco_fp);
		fclose(cisco_fp);

		char buffer [256] = {0};
		char tmp [256] = {0};
#if _DEBUG
		sprintf(buffer, "%s\\temp\\Cisio_logfile_%s.ini", FuncGetInstallRootPath(), server);
#else
		sprintf(buffer, "%s\\MonitorManager\\temp\\Cisio_logfile_%s.ini", FuncGetInstallRootPath(), server);
#endif
		DFN_WritePrivateProfileString("logfile", "needconfirm", _ltoa(TRUE, tmp, 10), buffer);

		delete pgd;
		return TRUE;
	}
}


BOOL CISCOCONFIG_MONITOR62(char *server, int port, char *pwd, char *enpwd, 
					  char *pwdprompt, char *prompt, char *enpwdprompt, 
					  char *enprompt, char *encmd, char *monitorid, 
					  char *proxy_server, int proxy_port, char *proxy_uid, 
					  char *proxy_pwd, char *custpath, char *szReturn)
{
	int		ret = 0;
	int		hSocket = 0;
	char runcmd[64] = {0};

    printf("Server:%s\nPort:%d\n", server, port);
	hSocket = telnet_init(server, port, proxy_server, proxy_port, proxy_uid, proxy_pwd);
	if(hSocket <= 0)
	{
		if(hSocket == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_1%>"));//<%IDS_CiscoLog_1%>
		else if(hSocket == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_2%>"));//<%IDS_CiscoLog_2%>
		else if(hSocket == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_3%>"));//<%IDS_CiscoLog_3%>
		else if(hSocket == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_4%>"));//<%IDS_CiscoLog_4%>
		else if(hSocket == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_5%>"));//<%IDS_CiscoLog_5%>
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_6%>"));//<%IDS_CiscoLog_6%>

		return FALSE;
	}

	LPGDATA pgd=new GDATA;
	memset(pgd,0,sizeof(GDATA));

    printf("Connect Cisio network set\n");
	if((ret = cisco_connect(hSocket, pwd, pwdprompt, prompt,pgd)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_7%>"));//<%IDS_CiscoLog_7%>
		delete pgd;
		return FALSE;
	}

	strncpy(runcmd, "enable\r\n", 8);
	send(hSocket, runcmd, strlen(runcmd), 0);
    printf("Login Cisio Config\n");
	if((ret = cisco_connect(hSocket, enpwd, enpwdprompt, enprompt,pgd)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_8%>"));//<%IDS_CiscoLog_8%>
		delete pgd;
		return FALSE;
	}
    printf("Execute command\n");
	if((ret = cisco_command(hSocket, encmd, enprompt,pgd)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_9%>"));//<%IDS_CiscoLog_9%>
		delete pgd;
		return FALSE;
	}

	int index = 0;
	char prebuffer[MAX_READ_SIZE] = {0};
	CString strBuffer(pgd->databuffer);
	
	index = strBuffer.Find("show running-config\r\n", 0);
	if(index >= 0)
		strBuffer.Delete(0, index + 21);

	if(!strBuffer.Right(strlen(enprompt)).Compare(enprompt))
	{
		index = strBuffer.ReverseFind('\r');
		if(index >= 0)
			strBuffer = strBuffer.Left(index);
	}

	FILE *cisco_fp = NULL;
	char prefile[256] = {0};

	sprintf(prefile, "%s\\cisco_config_%s", custpath, monitorid);

	if((_access(prefile, 0x04)) == -1)
	{
		if((cisco_fp = fopen(prefile, "w")) == NULL)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_10%>"));//<%IDS_CiscoLog_10%>
			delete pgd;
			return FALSE;
		}

		sprintf(szReturn,"status=%s$", FuncGetStringFromIDS("<%IDS_CiscoLog_11%>"));//<%IDS_CiscoLog_11%>
		int i = fwrite(strBuffer, sizeof(char), strBuffer.GetLength(), cisco_fp);
		fclose(cisco_fp);
		delete pgd;
		return TRUE;
	}

	if((cisco_fp = fopen(prefile, "r")) == NULL)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_12%>"));//<%IDS_CiscoLog_12%>
		delete pgd;
		return FALSE;
	}

	int j = fread(prebuffer, sizeof(char), sizeof(prebuffer), cisco_fp);
	fclose(cisco_fp);
	prebuffer[j] = 0;
	int m = strlen(prebuffer);
	int n = strBuffer.GetLength();
	if(!strBuffer.Compare(prebuffer))
	{
		char buffer [256] = {0};
		char tmp [256] = {0};
#if _DEBUG
		sprintf(buffer, "%s\\temp\\Cisio_logfile_%s.ini", FuncGetInstallRootPath(), server);
#else
		sprintf(buffer, "%s\\MonitorManager\\temp\\logfile.ini", FuncGetInstallRootPath());
#endif

		BOOL bChange = DFN_GetPrivateProfileInt(monitorid, "needconfirm", FALSE, buffer);
		if (!bChange)
		{
			sprintf(szReturn, "status=%s$", FuncGetStringFromIDS("<%IDS_CiscoLog_13%>"));//<%IDS_CiscoLog_13%>
			delete pgd;
			return TRUE;
		}
		else
		{
			sprintf(szReturn, "status=%s$", FuncGetStringFromIDS("<%IDS_CiscoLog_14%>"));//<%IDS_CiscoLog_14%>			
			delete pgd;
			return TRUE;
		}
	}
	else
	{
		sprintf(szReturn, "status=%s$", FuncGetStringFromIDS("<%IDS_CiscoLog_14%>"));//<%IDS_CiscoLog_14%>
		if((cisco_fp = fopen(prefile, "w")) == NULL)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_CiscoLog_15%>"));//<%IDS_CiscoLog_15%>
			delete pgd;
			return FALSE;
		}

		int k = fwrite(strBuffer, sizeof(char), strBuffer.GetLength(), cisco_fp);
		fclose(cisco_fp);

		char buffer [256] = {0};
		char tmp [256] = {0};
#if _DEBUG
		sprintf(buffer, "%s\\temp\\Cisio_logfile_%s.ini", FuncGetInstallRootPath(), server);
#else
		sprintf(buffer, "%s\\MonitorManager\\temp\\logfile.ini", FuncGetInstallRootPath());
#endif
		DFN_WritePrivateProfileString(monitorid, "needconfirm", _ltoa(TRUE, tmp, 10), buffer);

		delete pgd;
		return TRUE;
	}
}

int cisco_connect(SOCKET hSocket, char *pwd, char *pwdprompt, char *prompt,LPGDATA pgd)
{
	int		n = 0;
	char	buffer[8192] = {0};
	char	*scan = NULL;
	int		telnetprogress = 0;
	int		count = 0;

	memset(buffer, 0, sizeof(buffer));
	memset(pgd->databuffer, 0, BUF_SIZE);
	pgd->datalen = 0;
	while(1)
	{
		char buff[8192] = {0};

		if(RESPONSE_WAIT(hSocket, READ_TIMEOUT) <= 0)
			return -1;

		n = recv(hSocket, buff, sizeof(buff), 0);
		if(n == SOCKET_ERROR)
			return -2;
		buff[n] = 0;
		
		strcat(buffer, buff);

		scan = buff;
		while(n--)
		{
			telnet_protocol(hSocket, *scan,pgd);
			TRACE("%c", *scan);
			scan ++;
		}
		switch(telnetprogress)
		{
		case 0:
			if(strstr(pgd->databuffer, pwdprompt))
			{
				char	buf[255] = {0};
				strcpy(buf, pwd);
				strcat(buf, "\r\n");
				send(hSocket, buf, strlen(buf), 0);
				telnetprogress ++;
				count = 0;
				memset(buffer, 0, sizeof(buffer));
				memset(pgd->databuffer, 0, BUF_SIZE);
				pgd->datalen = 0;
			}
			break;
		case 1:
			if(strstr(pgd->databuffer, prompt))
			{
				memset(buffer, 0, sizeof(buffer));
				memset(pgd->databuffer, 0, BUF_SIZE);
				pgd->datalen = 0;
				return 0;
			}
			break;
		}

		if(++count > 50)
		{
			if(telnetprogress == 0)
				return -3;
			if(telnetprogress == 1) 
				return -4;
		}
	}
}

/*
int cisco_connect(SOCKET hSocket, char *pwd, char *pwdprompt, char *prompt)
{
	int		n = 0;
	char	buffer[4096] = {0};
	char	*scan = NULL;
	int		telnetprogress = 0;
	int		count = 0;

	memset(databuffer, 0, BUF_SIZE);
	datalen = 0;
	while(1)
	{
		memset(buffer, 0, 4096);
		if(RESPONSE_WAIT(hSocket, READ_TIMEOUT) <= 0)
			return -1;

		n = recv(hSocket, buffer, 4096, 0);
		if(n == SOCKET_ERROR)
			return -2;
		buffer[n] = 0;

		scan = buffer;
		while(n--)
		{
			telnet_protocol(hSocket, *scan);
			TRACE("%c", *scan);
			scan ++;
		}
		switch(telnetprogress)
		{
		case 0:
			if(strstr(databuffer, pwdprompt))
			{
				char	buf[255] = {0};
				strcpy(buf, pwd);
				strcat(buf, "\r\n");
				send(hSocket, buf, strlen(buf), 0);
				telnetprogress ++;
				count = 0;
				memset(databuffer, 0, BUF_SIZE);
				datalen = 0;
			}
			break;
		case 1:
			if(strstr(databuffer, prompt))
			{
				memset(databuffer, 0, BUF_SIZE);
				datalen = 0;
				return 0;
			}
			break;
		}

		if(++count > 50)
		{
			if(telnetprogress == 0)
				return -3;
			if(telnetprogress == 1) 
				return -4;
		}
	}
}
*/

int cisco_command(SOCKET hSocket, char *cmd, char *prompt,LPGDATA pgd)
{
	int		n = 0;
	char	buffer[4096] = {0};
	char	*scan = NULL;
	int		flag = 0;

	strcpy(buffer, cmd);
	strcat(buffer, "\r\n");

	send(hSocket, buffer, strlen(buffer), 0);

	memset(pgd->databuffer, 0, BUF_SIZE);
	pgd->datalen = 0;
	while(RESPONSE_WAIT(hSocket, READ_TIMEOUT))
	{
		memset(buffer, 0, sizeof(buffer));
		n = recv(hSocket, buffer, 4096, 0);
		if(n == SOCKET_ERROR) return -2;
		if(n == 0) return -2;
		buffer[n] = 0;

		if(strstr(pgd->databuffer, cmd))
		{
			if(strstr(buffer, prompt)) 
			{
				char	buf[255] = {0};
				strncpy(buf, buffer + n - strlen(prompt), strlen(prompt));
				if(!strcmp(buf, prompt)) flag = 1;
			}
		}

		scan = buffer;
		while(n--) 
		{
			telnet_protocol(hSocket, *scan,pgd);
			TRACE("%c", *scan);
			scan ++;
		}

		char *ca = NULL;
		if(ca = strstr(pgd->databuffer, "--More--"))
		{
			memset(ca, 0x20, 8);
			send(hSocket, " ", 1, 0);
		}

		if(flag) break;
	}

	return pgd->datalen;
}
