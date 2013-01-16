
#include "LogFile.h"
#include "TelMonitor.h"
#include "SSHMonitor.h"
#include "HttpMonitor.h"
#include "RloginMonitor.h"
#include "Utils.h"

#include "base/funcGeneral.h"
#include "svdb/svapi/svapi.h"
#include "svdb/svapi/svdbapi.h"
#include "webcgi/base/basetype.h"

#define WANGGANG

#ifndef NO_IMPLICIT_REGEXCLASS_NAMESPACE_USE
#define NO_IMPLICIT_REGEXCLASS_NAMESPACE_USE
#endif

#include "base/regx/RegularExpression.h"


#include <string>
using namespace std;

extern char	rlogin_databuffer[BUF_SIZE];
extern int	rlogin_datalen;

BOOL GetDyn(std::string strMid , int& nState, std::string&  strDyn  )
{
	nState=0;
	strDyn ="";

	sv_dyn dyn;                
	if(GetSVDYN(strMid, dyn ))
    {
		nState= dyn.m_state;
		if(dyn.m_displaystr!=NULL)
			strDyn = dyn.m_displaystr;
     return TRUE;   
    }
	return FALSE;
}


BOOL ParserContent( char *  content,long &lTotalLine, long & lMatches ,char *matchstr,char *strmid,char * szResult )
{

	WriteLog( "=============== ParserContent =================" );
	WriteLog( "ToMatchString:" );
	WriteLog( content );
	WriteLog( "MatchString:" );
	WriteLog( matchstr );

	RegularExpression RegularExpressionObject;
	char * ca=NULL;
	char * cb=NULL;
	char * tmpBuffer;

	ca =content;
	//puts(ca);
	BOOL bFlag = FALSE;
	

	if(*matchstr)
	{
		RegularExpressionObject.FormatType = RegularExpression::FormatTypeAll;
		RegularExpressionObject.LocaleModel = RegularExpression::LocaleModelCpp;
		RegularExpressionObject.Expression =matchstr;
		if (RegularExpressionObject.ParseExpression() != RegularExpression::REG_NOERROR)
		{
			sprintf(szResult,"error=%s", ::FuncGetStringFromIDS("IDS_SysLog_Monitor_ExpressionError"));//表达式错误");
			return FALSE;
		}
	}
	
	bool bWriteTime = true;
    while(cb =strstr(ca,"\n"))
	{

		if(ca != cb)
		{
			lTotalLine ++;
			
			if(*matchstr)
			{
				tmpBuffer =(char *)malloc(cb-ca+2);
				memset(tmpBuffer,0,cb-ca+2);
				strncpy(tmpBuffer, ca, cb - ca+1);
				RegularExpressionObject.StringToMatch=tmpBuffer;
				if(RegularExpressionObject.Search())
				{
					if(bWriteTime)
					{
						WriteLogTime(strmid);
						bWriteTime = false;
					}

					WriteLogData(tmpBuffer, strmid);
					lMatches ++;
				}
				
				free(tmpBuffer);
			}

			if(cb-content+ 1>=strlen(content )) 
				break;
			ca =cb+1;
			
		}
		else
		{
			if(cb-content+ 1>=strlen(content )) break;
			ca =cb+1;
		}

	}
	return TRUE;		
}

//nOnce 3  always monitor
//nOnce 0  monitor as soon as error state 
//nOnce .....
BOOL LOGFILE_TEL_VALUE(char *server, int port, char *uid, char *pwd, char *pserver, 
					   int pport, char *puid, char *ppwd, char *lprom, char *pprom, 
					   char *prom, char *inifile, char *logfile, char *match, 
					   char *rulefile, int append, int ngid, char* strmid, char *custpath, 
					   int nOnce, char *szReturn)
{
	WriteLog( "=============== LOGFILE_TEL_VALUE =================" );

	int		ret = 0;
	int		hSocket = 0;
	BOOL	bResult = TRUE;
	char	buffer[256] = {0};
	long	lSize = 0, lbSize = 0;
	char	szIniFile[256] = {0};
	BOOL	bError = FALSE;
	int nState;  //  for new logic   stop monitor as soon as error state
	std::string strDyn;

	/*************************************
	 * For Reason Of Prompt
	 */
	int		c = 0;
	char	*ca = prom, *cb = NULL;
	char	arprompt[PR_NUM][256];
	memset(arprompt, 0, sizeof(arprompt));
	char tmp[256] = {0};
	
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
	Tel_Param param;/////////////////////////////

	if((ret = telnet_connect(hSocket, uid, pwd, lprom, pprom, arprompt,pgd,&param )) < 0)
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

	sprintf(szIniFile, "%s", inifile);

	WriteLog( inifile );

	if(TelLogFileExist(hSocket, szIniFile, logfile))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_03%>"));
		bResult = FALSE;
		goto err;
	}

	if(TelLogFileSize(hSocket, szIniFile, logfile, lSize))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_01%>"));
		bResult = FALSE;
		goto err;
	}

	//----------------------------------------------------------------------
	// 更改内容： 去掉读配置文件，每次取值都检查整个日志，此次修改特意为好
	//            友王刚而做
	// 更改人：   邹晓
	// 更改时间： 2009.06.12
	//----------------------------------------------------------------------
#ifndef WANGGANG
	sprintf(buffer, "%s", "logfile.ini");
	lbSize = DFN_GetPrivateProfileInt(strmid, "size", 0, buffer);
	DFN_WritePrivateProfileString(strmid, "size", _ltoa(lSize, tmp, 10) , buffer);
	puts(buffer);
	puts(_ltoa(lSize, tmp, 10));
#endif 
	



	//----------------------------------------------------------------------
	// 更改内容： 更改head和tail的执行方式
	//            head -lbSize 
	//            tail +lbSize
	//			  其中，lbSize是至上次执行结束，所取到的数据量（行或字节）
	//			  以上两个命令须在oscmd.ini进行配置
	// 更改人：   邹晓
	// 更改时间： 2009.06.02
	//----------------------------------------------------------------------

	/*
	if(lbSize == -1 )
	{
		sprintf(szReturn, "totalline=0$matches=0$");
		puts("totalline=0$matches=0$");		
		bResult = TRUE;
		goto err;
	}
	*/

	printf("--------%d\n",nOnce);
	GetDyn(strmid,nState,strDyn);
	if(nOnce==AlAWYSMONITOR)
		bError =FALSE;
	else
	{
		if( (nState==dyn_error ) )
			bError =TRUE;
		else bError =FALSE;
		
	}


	if(lbSize == lSize)
	{
		if (bError)
		{
			puts("false");
			sprintf(szReturn,"error=%s",strDyn.c_str());
		}
		else
		{
			puts("true");
			sprintf(szReturn, "totalline=%d$matches=%d$", 0, 0);
		}		
	}
	else
	{
		char tmp[256] = {0};
		
		if(append)
		{
			DFNParser_GetPrivateProfileString("tail", "command", "", buffer, sizeof(buffer), szIniFile);
		
			if( strlen(buffer)<= 0)
			{
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_02%>"));
				bResult = FALSE;
				goto err;
			}
		}
		else
		{
			DFNParser_GetPrivateProfileString("head", "command", "", buffer, sizeof(buffer), szIniFile);
			if( strlen(buffer)<= 0)
			{
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_02%>"));
				bResult = FALSE;
				goto err;
			}
		}

		CString strCmdInfo(buffer);
		/*
		if(lbSize == -1 || lbSize > lSize)
		{
			strCmdInfo.Replace("<bytes>", "1");
			strCmdInfo.Replace("<file>", logfile);
		}
		else
		{
			sprintf(buffer, "%d", lSize-lbSize);
			strCmdInfo.Replace("<bytes>", buffer);
			strCmdInfo.Replace("<file>", logfile);
		}
		*/
		sprintf(buffer, "%d", lbSize);
		strCmdInfo.Replace( "<bytes>", buffer );
		strCmdInfo.Replace( "<file>", logfile );


	
		puts(strCmdInfo);
		if((ret = telnet_command(hSocket, strCmdInfo.GetBuffer(strCmdInfo.GetLength()),pgd)) < 0)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_01%>"));
			bResult = FALSE;
			goto err;
		}

	
		long	lTotalLine = 0, lMatches = 0;
		char	tmpBuffer[1024] = {0};	

		ca = pgd->databuffer;
		if(!ParserContent( pgd->databuffer,lTotalLine, lMatches ,match,strmid,szReturn ))
		goto err;
		

  	   //sprintf(buffer, "%s", "logfile.ini");

		if (!bError)
		{
			puts("false");

			sprintf(szReturn, "totalline=%ld$matches=%ld$", lTotalLine, lMatches);
			/*
			if (lMatches>0)
			{
				//DFN_WritePrivateProfileString(strmid, "needconfirm", "1", buffer);
				//DFN_WritePrivateProfileString(strmid, "TotalLine",_ltoa( lTotalLine, tmp, 10),  buffer);
				//DFN_WritePrivateProfileString(strmid, "MatchLine",_ltoa( lMatches, tmp, 10) , buffer);
			}*/
		}
		else
		{
			puts("true");
			//long lTotal = DFN_GetPrivateProfileInt(strmid, "TotalLine", FALSE, buffer);
			//long lMatch = DFN_GetPrivateProfileInt(strmid, "MatchLine", FALSE, buffer);
			//sprintf(szReturn, "totalline=%ld$matches=%ld$", lTotalLine+lTotal, lMatches+lMatch);
			sprintf(szReturn, "error=%s",strDyn.c_str() );
			//DFN_WritePrivateProfileString(strmid, "TotalLine", _ltoa(lTotalLine+lTotal, tmp, 10), buffer);
			//DFN_WritePrivateProfileString(strmid, "MatchLine", _ltoa(lMatches+lMatch, tmp, 10), buffer);
		}
	}

err:
	shutdown(hSocket, 0x02);
	closesocket(hSocket);
	delete pgd;
	return bResult;
}

BOOL LOGFILE_SSH_VALUE(char *server, int port, char *uid, char *pwd, char *inifile, 
					   char *logfile, char *match, char *rulefile, int append, int ngid, 
					    char* strmid, char *custpath,int nOnce,char *privatekeyfile, char *szReturn)

{
	char szTemp[1024] = {0};
	WriteLog( "=============== LOGFILE_SSH_VALUE =================" );

	int		ret = 0;

	BOOL	bResult = TRUE;
	char	buffer[256] = {0};
	long	lSize = 0, lbSize = 0;
	char	szIniFile[256] = {0};
	BOOL	bError = FALSE;
	int nState;  //  for new logic   stop monitor as soon as error state
	std::string strDyn;

	/*************************************
	 * For Reason Of Prompt
	 */
	int		c = 0;
	char tmp[256] = {0};
	

	sprintf(szIniFile, "%s", inifile);	
	sprintf( szTemp, "ini:%s, log:%s", inifile, logfile );
	WriteLog( szTemp );
	
	if(SshLogFileExist(server, port, uid, pwd, szIniFile, logfile, privatekeyfile))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_03%>"));
		WriteLog( "SshLogFileExist error" );
		return FALSE;
	}

	if(SshLogFileSize(server, port, uid, pwd, szIniFile, logfile, privatekeyfile, lSize))
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_01%>"));
		WriteLog( "SshLogFileSize error" );
		return FALSE;
	}


	//----------------------------------------------------------------------
	// 更改内容： 去掉读配置文件，每次取值都检查整个日志，此次修改特意为好
	//            友王刚而做
	// 更改人：   邹晓
	// 更改时间： 2009.06.12
	//----------------------------------------------------------------------
#ifndef WANGGANG
	sprintf(buffer, "%s", "logfile.ini");
	lbSize = DFN_GetPrivateProfileInt(strmid, "size", 0, buffer);
	DFN_WritePrivateProfileString(strmid, "size", _ltoa(lSize, tmp, 10) , buffer);
	puts(buffer);
	puts(_ltoa(lSize, tmp, 10));
#endif 


	//----------------------------------------------------------------------
	// 更改内容： 更改head和tail的执行方式
	//            head -lbSize 
	//            tail +lbSize
	//			  其中，lbSize是至上次执行结束，所取到的数据量（行或字节）
	//			  以上两个命令须在oscmd.ini进行配置
	// 更改人：   邹晓
	// 更改时间： 2009.06.02
	//----------------------------------------------------------------------
	/*
	if(lbSize == -1 )
	{
		sprintf(szReturn, "totalline=0$matches=0$");
		puts("totalline=0$matches=0$");		
		bResult = TRUE;
		goto err;
	}
	*/

//	bError = DFN_GetPrivateProfileInt(strmid, "needconfirm", FALSE, buffer);
	GetDyn(strmid,nState,strDyn);

	if(nOnce==AlAWYSMONITOR)
		bError =FALSE;
	else
	{
		if( nState==dyn_error  )
			bError =TRUE;
		else bError =FALSE;
		
	}

	if(lbSize == lSize)
	{
		if (bError)
		{
			sprintf(szReturn,"error=%s",strDyn.c_str());
		}
		else
		{
			sprintf(szReturn, "totalline=%d$matches=%d$", 0, 0);
		}		
	}
	
	else
	{
		
		char tmp[256] = {0};
		if(append)
		{
			if(DFNParser_GetPrivateProfileString("tail", "command", "", buffer, sizeof(buffer), szIniFile) <= 0)
			{
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_02%>"));
				bResult = FALSE;
				goto err;
			}
		}
		else
		{
			if(DFNParser_GetPrivateProfileString("head", "command", "", buffer, sizeof(buffer), szIniFile) <= 0)
			{
				sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_02%>") );
				bResult = FALSE;
				goto err;
			}
		}

		CString strCmdInfo(buffer);
		/*
		if(lbSize == -1 || lbSize > lSize)
		{
			strCmdInfo.Replace("<bytes>", "1");
			strCmdInfo.Replace("<file>", logfile);
		}
		else
		{
			sprintf(buffer, "%d", lSize- lbSize );
			strCmdInfo.Replace("<bytes>", buffer);
			strCmdInfo.Replace("<file>", logfile);
		}
		*/
		sprintf(buffer, "%d", lbSize);
		strCmdInfo.Replace( "<bytes>", buffer );
		strCmdInfo.Replace( "<file>", logfile );

		puts( strCmdInfo.GetBuffer() );

		CString strResult;
		if(ssh_command(server, port, uid, pwd, strCmdInfo.GetBuffer(strCmdInfo.GetLength()), privatekeyfile, strResult) < 0)
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_LOGFILE_01%>"));
			return FALSE;
		}

		
		long	lTotalLine = 0, lMatches = 0;
		char	tmpBuffer[1024] = {0};	

		
		if(!ParserContent( strResult.GetBuffer(strResult.GetLength()),lTotalLine, lMatches ,match,strmid,szReturn ))
		goto err;

		sprintf(buffer, "%s", "logfile.ini");

		if (!bError)
		{
			sprintf(szReturn, "totalline=%ld$matches=%ld$", lTotalLine, lMatches);
		}
		else
		{
			sprintf(szReturn,"error=%s",strDyn.c_str());
		}
	}

err:
	return bResult;
}


int TelLogFileSize(int hSocket, char *inifile, char *logfile, long &size)
{
	int		n = 0, sizeindex = 0;
	char	cmd[256] = {0};
	char	*scan = NULL, *ca = NULL, *cb = NULL;

	DFNParser_GetPrivateProfileString("filedetail", "command", "", cmd, sizeof(cmd), inifile) ;
	if(strlen(cmd)<=0)  	return 1;

	sizeindex = DFNParser_GetPrivateProfileInt("filedetail", "size", 0, inifile);
	if(sizeindex <= 0)
		return 1;

	CString strCmdInfo(cmd);
	strCmdInfo.Replace("<file>", logfile);

	LPGDATA pgd=new GDATA;
	memset(pgd,0,sizeof(GDATA));


	if(telnet_command(hSocket, strCmdInfo.GetBuffer(strCmdInfo.GetLength()),pgd) < 0)
	{
		delete pgd;
		return 1;
	}
	n=GetColumnFromLine(pgd->databuffer, sizeindex, size);

	return 	n;
}

int RloginLogFileSize(int hSocket, char *inifile, char *logfile, long &size)
{
	int		n = 0, sizeindex = 0;
	char	cmd[256] = {0};
	char	*scan = NULL, *ca = NULL, *cb = NULL;



	if(DFN_GetPrivateProfileString("filedetail", "command", NULL, cmd, sizeof(cmd), inifile) <= 0)
		return 1;

	sizeindex = DFN_GetPrivateProfileInt("filedetail", "size", 0, inifile);
	if(sizeindex <= 0)
		return 1;

	CString strCmdInfo(cmd);
	strCmdInfo.Replace("<file>", logfile);

	LPGDATA pgd=new GDATA;
	memset(pgd,0,sizeof(GDATA));


	if(telnet_command(hSocket, strCmdInfo.GetBuffer(strCmdInfo.GetLength()),pgd) < 0)
	{
		delete pgd;
		return 1;
	}

	
	n=GetColumnFromLine(pgd->databuffer, sizeindex, size);
	delete pgd;

	return 	n;
}

int SshLogFileSize(char *server, int port, char *uid, char *pwd, char *inifile, char *logfile,char *privatekeyfile, long &size)
{
	int		sizeindex = 0;
	char	cmd[256] = {0};
	char	*ca = NULL;

	if(DFNParser_GetPrivateProfileString("filedetail", "command", "", cmd, sizeof(cmd), inifile) <= 0)
		return 1;

	sizeindex = DFNParser_GetPrivateProfileInt("filedetail", "size", 0, inifile);
	if(sizeindex <= 0)
		return 1;

	if(ca = strchr(cmd, '<'))
	{
		strcpy(ca, logfile);
		*(ca + strlen(logfile)) = 0;
	}

	CString strResult = _T("");
	if(ssh_command(server, port, uid, pwd, cmd, privatekeyfile, strResult) < 0)
		return 1;

	return GetColumnFromLine(strResult, sizeindex, size);	
}

int HttpLogFileSize(char *url, char *uid, char *pwd, char *pserver, int pport, 
					char *puid, char *ppwd, char *inifile, char *logfile, long &size)
{
	int		n = 0, sizeindex = 0;
	char	cmd[256] = {0};
	char	*scan = NULL, *ca = NULL, *cb = NULL;

	if(DFN_GetPrivateProfileString("filedetail", "command", NULL, cmd, sizeof(cmd), inifile) <= 0)
		return 1;

	sizeindex = DFN_GetPrivateProfileInt("filedetail", "size", 0, inifile);
	if(sizeindex <= 0)
		return 1;

	CString strCmdInfo(cmd);
	strCmdInfo.Replace("<file>", logfile);

	char msg[8192] = {0};
	if(!http_command(url, strCmdInfo.GetBuffer(strCmdInfo.GetLength()), 
				READ_TIMEOUT, uid, pwd, pserver, pport, puid, ppwd, msg))
		return 1;

	return GetColumnFromLine(msg, sizeindex, size);	
}

int HttpLogFileExist(char *url, char *uid, char *pwd, char *pserver, int pport, 
					char *puid, char *ppwd, char *inifile, char *logfile)
{
	int		n = 0;
	char	cmd[256] = {0};
	
	if(DFN_GetPrivateProfileString("fileexist", "command", NULL, cmd, sizeof(cmd), inifile) <= 0)
		return 1;

	CString	strCmdInfo(cmd);
	strCmdInfo.Replace("<file>", logfile);

	char msg[8192] = {0};
	if(!http_command(url, strCmdInfo.GetBuffer(strCmdInfo.GetLength()), 
				READ_TIMEOUT, uid, pwd, pserver, pport, puid, ppwd, msg))
		return 1;

	long v = 0;
	if(GetColumnFromLine(msg, 1, v))
		return 1;
	else
	{
		return v;
	}
}

int TelLogFileExist(int hSocket, char *inifile, char *logfile)
{
	int		n = 0;
	char	cmd[256] = {0};

	DFNParser_GetPrivateProfileString("fileexist", "command", "", cmd, sizeof(cmd), inifile) ;
	if(strlen(cmd)<=0)		return 1;

	CString strCmdInfo(cmd);
	strCmdInfo.Replace("<file>", logfile);

	LPGDATA pgd=new GDATA;
	memset(pgd,0,sizeof(GDATA));

	
	if(telnet_command(hSocket, strCmdInfo.GetBuffer(strCmdInfo.GetLength()),pgd) < 0)
	{
		delete pgd;
		return 1;
	}

	long v = 0;
	if(GetColumnFromLine(pgd->databuffer, 1, v))
	{
		delete pgd;
		return 1;
	}
	else
	{
		delete pgd;
		return v;
	}
}

int RloginLogFileExist(int hSocket, char *inifile, char *logfile)
{
	int		n = 0;
	char	cmd[256] = {0};

	if(DFN_GetPrivateProfileString("fileexist", "command", NULL, cmd, sizeof(cmd), inifile) <= 0)
		return 1;

	CString strCmdInfo(cmd);
	strCmdInfo.Replace("<file>", logfile);

	LPGDATA pgd=new GDATA;
	memset(pgd,0,sizeof(GDATA));
	
	if(telnet_command(hSocket, strCmdInfo.GetBuffer(strCmdInfo.GetLength()),pgd) < 0)
	{
		delete pgd;
		return 1;
	}

	long v = 0;
	if(GetColumnFromLine(pgd->databuffer, 1, v))
	{
		delete pgd;
		return 1;
	}
	else
	{
		delete pgd;
		return v;
	}
}

int SshLogFileExist(char *server, int port, char *uid, char *pwd, char *inifile, char *logfile, char *privatekeyfile)
{
	char	cmd[256] = {0};
	char	*ca = NULL;
	
	if(DFNParser_GetPrivateProfileString("fileexist", "command", "", cmd, sizeof(cmd), inifile) <= 0)
		return 1;

	
	CString strCmdInfo(cmd);
	strCmdInfo.Replace("<file>", logfile);

	CString strResult = _T("");
	if(ssh_command(server, port, uid, pwd, strCmdInfo.GetBuffer(strCmdInfo.GetLength()),  privatekeyfile, strResult) < 0)
		return 1;

	long v = 0;
	if(GetColumnFromLine(strResult, 1, v))
		return 1;
	else
		return v;
}

int GetColumnFromLine(CString strIn, int nColumn, long &size)
{
	int nLen = strIn.GetLength();
	TCHAR ch;
	CString strOut  = _T("");

	if(nColumn == 999)
	{
		int index = strIn.ReverseFind(' ');
		if(index >= 0)
		{
			strOut = strIn.Right(strIn.GetLength() - index - 1);
			if(isdigit(strOut.GetAt(0)))
			{
				size = atoi(strOut);
				return 0;
			}
		}
	}
	else
	{
		for (int i = 0; i < nLen; i++) 
		{		
			ch = strIn.GetAt(i);
			if (ch == 0x20)
				continue;
			nColumn--;
			strOut = strIn.Right(nLen - i);
			strOut = strOut.SpanExcluding(" ");

			if (nColumn == 0) 
			{
				if(isdigit(strOut.GetAt(0)))
				{
					size = atoi(strOut);
					return 0;
				}
			}

			i += strOut.GetLength();
		}
	}

	return 1;
}

void WriteLogData(char *msg, const char* strmid)
{
	//FILE *fp = NULL;
	//char szLogFile[256] = {0};

	////sprintf(szLogFile, "%s\\logs\\logfile_%d_%d.log", FuncGetInstallRootPath(), ngid, nmid);
	//sprintf(szLogFile, "%s\\temp\\logfile_%s.log", FuncGetInstallRootPath(), strmid);

	//if(fp = fopen(szLogFile, "a+"))
	//{
	//	fprintf(fp, "%s", msg);
	//	fclose(fp);
	//}

	FILE *fp = NULL;
	CString csTmp, csPath;
	csPath.Format("%s\\%s\\%s.txt", GetSiteViewRootPath().c_str(), "data\\Temp", strmid);
	csTmp.Format("%s\r\n", msg);
	
	fp = fopen(csPath,"a+");
	if(fp != NULL)
	{
		fprintf(fp, csTmp);
		fclose(fp);	
	}	
}

//
void WriteLogTime(const char* strmid)
{
	FILE *fp = NULL;
	TTime curTime = TTime::GetCurrentTimeEx();
	CString csTmp, csPath;
	csPath.Format("%s\\%s\\%s.txt", GetSiteViewRootPath().c_str(), "data\\Temp", strmid);
	csTmp.Format("[Time is%s]\r\n", curTime.Format().c_str());

	fp = fopen(csPath,"a+");
	if(fp != NULL)
	{
		fprintf(fp, csTmp);
		fclose(fp);	
	}	
}

BOOL PaseString(CString strScr,CStringList&strList)
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


void AddToLogFileTemLog(char *msg, int ngid, int nmid)
{
	FILE *fp = NULL;
	char szLogFile[256] = {0};

	sprintf(szLogFile, "%s\\logs\\logfiletem_%d_%d.log", FuncGetInstallRootPath(), ngid, nmid);
	if(fp = fopen(szLogFile, "a+"))
	{
		fprintf(fp, "%s\r\n", msg);
		fclose(fp);
	}
	
}

