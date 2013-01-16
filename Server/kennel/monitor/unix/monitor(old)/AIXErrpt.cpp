
#include "AIXErrpt.h"
#include "TelMonitor.h"
#include "SSHMonitor.h"
#include "HttpMonitor.h"
#include "RloginMonitor.h"
#include "Utils.h"

#include "..\..\base\funcGeneral.h"

//extern char	databuffer[BUF_SIZE];
//extern int	datalen;

extern char	rlogin_databuffer[BUF_SIZE];
extern int	rlogin_datalen;

int GetConfirms(CString strMonitorID)
{
	CString strFileName, strDate;
	strFileName.Format("..\\data\\tmpinifile\\Confirms_%s.ini", strMonitorID);
	strDate = COleDateTime::GetCurrentTime().Format("%Y%m%d");
	int nRet = GetPrivateProfileInt(strDate, "Times", -1, strFileName);
	WritePrivateProfileString(strDate, "Times", "0", strFileName);
	if(nRet == -1)
		return 0;
	else
		return nRet;
}

bool AIXERRPT_TEL_VALUE(char *server, int port, char *uid, char *pwd, 
						char *pserver, int pport, char *puid, char *ppwd, 
						char *lprom, char *pprom, char *prom, char *inifile, 
						int ngid, char* strmid, char *custpath, char *szReturn)
{

	int		ret = 0;
	int		hSocket = 0;
	bool	bResult = TRUE;
	char	buffer[256] = {0};
	char	mbuffer[256] = {0};
	char	mdate[256] = {0};
	char	datebuff[64] = {0};
	char	curdate[64] = {0};
	//char	szIniFile[256] = {0};
	char	szTmpIniFile[256] = {0};
	int		isChinese = 0;

	//puts("ok");
	/*************************************
	 * For Reason Of Prompt
	 */
	int		c = 0;
	char	*ca = prom, *cb = NULL;
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
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_1%>"));//<%IDS_MQ_1%>"连接失败：域名解析"
		else if(hSocket == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_2%>"));//<%IDS_MQ_2%>"连接失败：通信初始化"
		else if(hSocket == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_3%>"));//<%IDS_MQ_3%>"连接失败：连接超时"
		else if(hSocket == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_4%>"));//<%IDS_MQ_4%>"代理服务器连接失败"
		else if(hSocket == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_5%>"));//<%IDS_MQ_5%>"代理服务器错误"
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_6%>"));//<%IDS_MQ_6%>"连接失败：未知错误"

		bResult = FALSE;
		goto err;
	}

	Tel_Param param;
	if((ret = telnet_connect(hSocket, uid, pwd, lprom, pprom, arprompt,pgd,&param)) < 0)
	{
		if(ret == -1)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_7%>"));//<%IDS_MQ_7%>"登录失败：读数据超时"
		else if(ret == -2)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_8%>"));//<%IDS_MQ_8%>"登录失败：读数据错误"
		else if(ret == -3)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_9%>"));//<%IDS_MQ_9%>"登录失败：查找提示符"
		else if(ret == -4)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_10%>"));//<%IDS_MQ_10%>"登录失败：查找登录提示符"
		else if(ret == -5)
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_11%>"));//<%IDS_MQ_11%>"登录失败：查找密码提示符"
		else 
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_MQ_12%>"));//<%IDS_MQ_12%>"登录失败：未知错误"

		bResult = FALSE;
		goto err;
	}

	if(DFNParser_GetPrivateProfileString("errorlog", "command", "", buffer, sizeof(buffer), inifile) <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_02%>"));
		bResult = FALSE;
		goto err;
	}

	if(DFNParser_GetPrivateProfileString("errorlog", "matchline", "", mbuffer, sizeof(mbuffer), inifile) <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_02%>"));
		bResult = FALSE;
		goto err;
	}

	if(DFNParser_GetPrivateProfileString("date", "command", "", mdate, sizeof(mdate), inifile) <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_02%>"));
		bResult = FALSE;
		goto err;
	}
	
	isChinese = DFNParser_GetPrivateProfileInt("date", "isChinese", 0, inifile);

	if((ret = telnet_command(hSocket, mdate,pgd,&param)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_01%>"));
		bResult = FALSE;
		goto err;
	}

	if(isChinese)
	{
		char	week[64] = {0};
		int		years = 0, months = 0, days = 0, 
				hours = 0, minutes = 0, seconds = 0;

		if(sscanf(pgd->databuffer, "公元%d年%d月%d日 %s %d时%d分%d秒", 
				&years, &months, &days, week, &hours, &minutes, &seconds) != 7)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_01%>"));
			bResult = FALSE;
			goto err;
		}

		sprintf(curdate, "%2.2d%2.2d%2.2d%2.2d%2.2d", months, days, hours, minutes, years % 100);
	}
	else
	{
		char	week[16] = {0};
		char	month[16] = {0}; 
		int		day = 0;
		int		hours = 0, mins = 0, secs = 0;
		char	div[16] = {0};
		int		year = 0;
		if(sscanf(pgd->databuffer, "%s %s %d %d:%d:%d %s %d", week, month, &day, &hours, &mins, &secs, div, &year) != 8)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_01%>"));
			bResult = FALSE;
			goto err;
		}

		if(convert_date(month, day, hours, mins, year, curdate))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_01%>"));
			bResult = FALSE;
			goto err;
		}
	}

	sprintf(szTmpIniFile, "logfile.ini");
	DFN_GetPrivateProfileString(strmid, "lastdate", "", datebuff, sizeof(datebuff), szTmpIniFile);
	
	if(!*datebuff) strncpy(datebuff, curdate, 10);

	if(ca = strchr(buffer, '<')) 
	{
		strncpy(ca, datebuff, 10);
		*(ca + 10) = 0;
	}

	memset(pgd->databuffer, 0, BUF_SIZE);
	pgd->datalen = 0;
    printf("Error Report Command is :%s\n", buffer);
	if((ret = telnet_command(hSocket, buffer,pgd)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_01%>"));
		bResult = FALSE;
		goto err;
	}

	if(ret == 0 || !strstr(pgd->databuffer, mbuffer))
	{
		char tmp [256] = {0};
		
		//ofstream fout("aixerr.log",ios::app);
		//int i = GetConfirms(strmid);
		//fout << itoa(i, tmp, 10) <<"\r\n"; 
		//fout << flush; 
		//fout.close();

		if(GetConfirms(strmid) > 0)
			DFN_WritePrivateProfileString(strmid, "needconfirm", _ltoa(FALSE, tmp, 10), szTmpIniFile);

		BOOL bChange = DFN_GetPrivateProfileInt(strmid, "needconfirm", FALSE, szTmpIniFile);
		if (!bChange)
		{
			sprintf(szReturn, "status=0$" ); //0: 未改变 1:改变
		}
		else
		{
			sprintf(szReturn, "status=1$" );
		}		
	}
	else
	{
		sprintf(szReturn, "status=1$" );

		FILE *fp = NULL;
		char szErrptLogFile[256] = {0};
		sprintf(szErrptLogFile, "%s\\data\\TmpIniFile\\logfile_%s.log", FuncGetInstallRootPath(), strmid);
		if(fp = fopen(szErrptLogFile, "a+"))
		{
			fprintf(fp, "%s", pgd->databuffer);
			fclose(fp);
		}

		char tmp [256] = {0};
        printf("Track record\n");
		DFN_WritePrivateProfileString(strmid, "needconfirm", _ltoa(TRUE, tmp, 10), szTmpIniFile);
	}
	DFN_WritePrivateProfileString(strmid, "lastdate", curdate, szTmpIniFile);
err:
	shutdown(hSocket, 0x02);
	closesocket(hSocket);
	delete pgd;
	return bResult;
}

BOOL AIXERRPT_SSH_VALUE(char *server, int port, char *uid, char *pwd, 
						char *inifile, int ngid, char* strmid, char *custpath, char *privatekeyfile,
						char *szReturn)
{
	int		ret = 0;
	char	buffer[256] = {0};
	char	mbuffer[256] = {0};
	char	mdate[256] = {0};
	char	datebuff[64] = {0};
	char	curdate[64] = {0};
	char	szTmpIniFile[256] = {0};
	int		isChinese = 0;
	CString	strResult = _T("");

	if(DFNParser_GetPrivateProfileString("errorlog", "command", "", buffer, sizeof(buffer), inifile) <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_02%>"));
		return FALSE;
	}

	if(DFNParser_GetPrivateProfileString("errorlog", "matchline", "", mbuffer, sizeof(mbuffer), inifile) <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_02%>"));
		return FALSE;
	}

	if(DFNParser_GetPrivateProfileString("date", "command", "", mdate, sizeof(mdate), inifile) <= 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_02%>"));
		return FALSE;
	}

	isChinese = DFNParser_GetPrivateProfileInt("date", "isChinese", 0, inifile);

	if((ret = ssh_command(server,port, uid, pwd, mdate, privatekeyfile, strResult)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_01%>"));
		return FALSE;
	}

	if(isChinese)
	{
		char	week[64] = {0};
		int		years = 0, months = 0, days = 0, 
				hours = 0, minutes = 0, seconds = 0;

		if(sscanf(strResult, "公元%d年%d月%d日 %s %d时%d分%d秒", 
				&years, &months, &days, week, &hours, &minutes, &seconds) != 7)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_01%>"));
			return FALSE;
		}

		sprintf(curdate, "%2.2d%2.2d%2.2d%2.2d%2.2d", months, days, hours, minutes, years % 100);
	}
	else
	{
		char	week[16] = {0};
		char	month[16] = {0}; 
		int		day = 0;
		int		hours = 0, mins = 0, secs = 0;
		char	div[16] = {0};
		int		year = 0;
		if(sscanf(strResult, "%s %s %d %d:%d:%d %s %d", week, month, &day, &hours, &mins, &secs, div, &year) != 8)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_01%>"));
			return FALSE;
		}

		if(convert_date(month, day, hours, mins, year, curdate))
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_01%>"));
			return FALSE;
		}
	}

    sprintf(szTmpIniFile, "logfile.ini");
	DFN_GetPrivateProfileString(strmid, "lastdate",  "", datebuff, sizeof(datebuff), szTmpIniFile);
	
	if(!*datebuff) strncpy(datebuff, curdate, 10);

	char *ca = NULL;
	if(ca = strchr(buffer, '<')) 
	{
		strncpy(ca, datebuff, 10);
		*(ca + 10) = 0;
	}

	strResult = _T("");
	if((ret = ssh_command(server, port, uid, pwd, buffer, privatekeyfile, strResult)) < 0)
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_01%>"));
		return FALSE;
	}

	if(ret == 0 || !strstr(strResult, mbuffer))
	{
		char tmp [256] = {0};
		if(GetConfirms(strmid) > 0)
			DFN_WritePrivateProfileString(strmid, "needconfirm", _ltoa(FALSE, tmp, 10), szTmpIniFile);

		BOOL bChange = DFN_GetPrivateProfileInt(strmid, "needconfirm", FALSE, szTmpIniFile);
		if (!bChange)
		{
			//sprintf(szReturn, "status=%s$", FuncGetStringFromIDS("<%IDS_AIXErrpt_01%>"));
			sprintf(szReturn, "status=0$" ); //0: 未改变 1:改变
		}
		else
		{
			//sprintf(szReturn, "status=%s$", FuncGetStringFromIDS("<%IDS_AIXErrpt_02%>"));
			sprintf(szReturn, "status=1$" ); //0: 未改变 1:改变
		}
	}
	else
	{
		//sprintf(szReturn, "status=%s$", FuncGetStringFromIDS("<%IDS_AIXErrpt_02%>"));
		sprintf(szReturn, "status=1$" ); //0: 未改变 1:改变
	
		FILE *fp = NULL;
		char szErrptLogFile[256] = {0};
        sprintf(szErrptLogFile, "%s\\data\\TmpIniFile\\logfile_%s.log", FuncGetInstallRootPath(), strmid);

		if(fp = fopen(szErrptLogFile, "a+"))
		{
			fprintf(fp, "%s", strResult);
			fclose(fp);
		}
		char tmp [256] = {0};
		DFN_WritePrivateProfileString(strmid, "needconfirm", _ltoa(TRUE, tmp, 10), szTmpIniFile);
	}
	DFN_WritePrivateProfileString(strmid, "lastdate", curdate, szTmpIniFile);
	return TRUE;
}




int convert_date(char *month, int day, int hours, int mins, int year, char *curdate)
{
	int mons = 0;

	if(!strncmp(month, "Jan", 3))
		mons = 1;
	else if(!strncmp(month, "Feb", 3))
		mons = 2;
	else if(!strncmp(month, "Mar", 3))
		mons = 3;
	else if(!strncmp(month, "Apr", 3))
		mons = 4;
	else if(!strncmp(month, "May", 3))
		mons = 5;
	else if(!strncmp(month, "Jun", 3))
		mons = 6;
	else if(!strncmp(month, "Jul", 3))
		mons = 7;
	else if(!strncmp(month, "Aug", 3))
		mons = 8;
	else if(!strncmp(month, "Sep", 3))
		mons = 9;
	else if(!strncmp(month, "Oct", 3))
		mons = 10;
	else if(!strncmp(month, "Nov", 3))
		mons = 11;
	else if(!strncmp(month, "Dec", 3))
		mons = 12;
	else
		return 1;

	sprintf(curdate, "%2.2d%2.2d%2.2d%2.2d%2.2d", mons, day, hours, mins, year % 100);
	return 0;
}