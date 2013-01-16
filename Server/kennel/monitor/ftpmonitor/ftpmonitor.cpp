// FtpMonitor.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "FtpMonitor.h"

#include "FTP.h"			// FTP Monitor

#include <Base\funcGeneral.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CFtpMonitorApp

BEGIN_MESSAGE_MAP(CFtpMonitorApp, CWinApp)
	//{{AFX_MSG_MAP(CFtpMonitorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFtpMonitorApp construction

CFtpMonitorApp::CFtpMonitorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFtpMonitorApp object

CFtpMonitorApp theApp;

/****************************************************************************
	FTP Monitor Export Function
****************************************************************************/

void WriteLogFile(const char* szMsg)
{
    FILE *stream;
    char chTime[50] = {0};
    //打开文件
#if _DEBUG
    stream=fopen(FuncGetInstallRootPath() + "\\Temp\\FtpMonitor.log","a");
#else
    stream=fopen(FuncGetInstallRootPath() + "\\Monitormanager\\Temp\\FtpMonitor.log","a");
#endif
    if(stream == NULL)
        return;//打开文件失败，直接返回
   	SYSTEMTIME time;
	//取系统当前时间
	GetLocalTime(&time);
    //sprintf(chTime ,"%02d年-%02d月-%02d日 %02d时-%02d分-%02d秒 ",
    sprintf(chTime ,"%dY-%dM-%dD %dH-%dM-%dS ",
				//time.wYear - 2000,
                time.wYear,
				time.wMonth,
				time.wDay,
				time.wHour,
				time.wMinute,
				time.wSecond
			);
    fputs(chTime , stream);
	fputs(szMsg,stream);
    fputs("\n",stream);
	fclose(stream);
}

extern "C" __declspec(dllexport) 
BOOL FTP(const char * strParas, char * szReturn, int& nSize)
//(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	//参数解析：
	CString		strFTPServ = _T(""), 
				strDownFile = _T(""), 
				strFTPUser = _T(""), 
				strFTPPass = _T("");
	int			nFTPPort = 21;
	
	int			passive_mode;

	CString		strProxyServ = _T(""), 
				strProxyUser = _T(""), 
				strProxyPass = _T(""), 
				strFTPMatch = _T("");
	int			nProxyType = 0, 
				nProxyPort = 21, 
				nTimeout = 60;

	CStringList paramList ;

	MakeStringListByChar(paramList,strParas);

	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\temp", FuncGetInstallRootPath());
	
	// Check Content Change

	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strFTPServ = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				nFTPPort = atoi(strTemp);
			}
			else
			{
				strFTPServ = strTemp;
			}
		}
        else if(strTemp.Find(__MACHINENAME__,0)==0)
		{
			strFTPServ=strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
        else if(strTemp.Find(__PORT__,0)==0)
        {
            nFTPPort = atoi(strTemp.Right(strTemp.GetLength()-strlen(__PORT__)));
        }
		else if(strTemp.Find(__DOWNLOADFILE__, 0) == 0)
		{
			strDownFile = strTemp.Right(strTemp.GetLength() - strlen(__DOWNLOADFILE__));
		}
		else if(strTemp.Find(__FTPUSERACCOUNT__, 0) == 0)
		{
			strFTPUser = strTemp.Right(strTemp.GetLength() - strlen(__FTPUSERACCOUNT__));
		}
		else if(strTemp.Find(__FTPPASSWORD__, 0) == 0)
		{
			strFTPPass = strTemp.Right(strTemp.GetLength() - strlen(__FTPPASSWORD__));
		}
		else if(strTemp.Find(__PROXYTYPE__, 0) == 0)
		{
			nProxyType = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PROXYTYPE__)));
		}
		else if(strTemp.Find(__PROXYSERVERPORT__, 0) == 0)
		{
			strTemp = strTemp.Right(strTemp.GetLength() - strlen(__PROXYSERVERPORT__));
			int index = strTemp.Find(':', 0);
			if(index >= 0)
			{
				strProxyServ = strTemp.Left(index);
				strTemp.Delete(0, index + 1);
				index = strTemp.Find(':', 0);
				if(index >= 0)
				{
					nProxyPort = atoi(strTemp.Left(index));
					strTemp.Delete(0, index + 1);
					nProxyType = atoi(strTemp);
				}
				else
				{
					nProxyPort = atoi(strTemp);
				}
			}
			else
			{
				strProxyServ = strTemp;
			}
		}
		else if(strTemp.Find(__PROXYUSERNAME__, 0) == 0)
		{
			strProxyUser = strTemp.Right(strTemp.GetLength() - strlen(__PROXYUSERNAME__));
		}
		else if(strTemp.Find(__PROXYPASSWORD__, 0) == 0)
		{
			strProxyPass = strTemp.Right(strTemp.GetLength() - strlen(__PROXYPASSWORD__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__PASSIVEMODE__, 0) == 0)
		{
			passive_mode = atoi(strTemp.Right(strTemp.GetLength() - strlen(__PASSIVEMODE__)));
		}
		else if(strTemp.Find(__MATCHSTR__, 0) == 0)
		{
			strFTPMatch = strTemp.Right(strTemp.GetLength() - strlen(__MATCHSTR__));
		}
		else if(strTemp.Find(__CUSTOMERPATH__, 0) == 0)
		{
			strCustomerPath.Format("%s\\%s\\temp", FuncGetInstallRootPath(), strTemp.Right(strTemp.GetLength() - strlen(__CUSTOMERPATH__)));
		}
		else
		{
		}
	}

	if(strFTPServ.IsEmpty())
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
            "FTP_SERVER_NAME_NULL"));//<%IDS_Monitor_40%>"缺少FTP服务器地址"		
//        WriteLogFile(szReturn);
        return FALSE;
	}

	//if(strDownFile.IsEmpty())
	//{
	//	sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
    //        "FTP_FILE_NOT_FOUND"));//<%IDS_Monitor_41%>"缺少下载文件名"
	//	return FALSE;
	//}

	if(strFTPUser.IsEmpty())
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
            "FTP_USERNAME_IS_NULL"));//<%IDS_Monitor_42%>"缺少用户名"		
//        WriteLogFile(szReturn);
        return FALSE;
	}

	if(nFTPPort <= 0)
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
            "FTP_PORT_ERROR"));//<%IDS_Monitor_43%>"端口错误"
//        WriteLogFile(szReturn);
        return FALSE;
	}

	if(nTimeout <= 0)
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_FTP",
            "FTP_TIMEOUT"));//<%IDS_Monitor_44%>"超时错误"		
//        WriteLogFile(szReturn);
        return FALSE;
	}

	if(!strProxyServ.IsEmpty() && nProxyPort <= 0)
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_PROXY",
            "PFOXY_SERVER_PORT_ERROR"));//<%IDS_Monitor_45%>"代理服务器端口错误"		
//        WriteLogFile(szReturn);
        return FALSE;
	}

	if(WSA_Init())
	{
		sprintf(szReturn, "error=%s$", FuncGetStringFromIDS("SV_BASIC",
            "BASIC_WSA_INIT_FAILED"));//<%IDS_Monitor_46%>"通信初始化错误"		
//        WriteLogFile(szReturn);
        return FALSE;
	}

	//监测的实现：
	BOOL bResult = FTP_MONITOR(strFTPServ.GetBuffer(strFTPServ.GetLength()), 
							nFTPPort, 
							strDownFile.GetBuffer(strDownFile.GetLength()), 
							strFTPUser.GetBuffer(strFTPUser.GetLength()), 
							strFTPPass.GetBuffer(strFTPPass.GetLength()), 
							nTimeout, 
							nProxyType, 
							strProxyServ.GetBuffer(strProxyServ.GetLength()), 
							nProxyPort, 
							strProxyUser.GetBuffer(strProxyPass.GetLength()), 
							strProxyPass.GetBuffer(strProxyPass.GetLength()), 
							strFTPMatch.GetBuffer(strFTPMatch.GetLength()), 
							strCustomerPath.GetBuffer(strCustomerPath.GetLength()), 
							passive_mode,
							szReturn);

	WSA_Free();
	CString strInput;
	strInput =szReturn;
	MakeCharByString(szReturn,nSize,strInput);

//    WriteLogFile(szReturn);
	return bResult;
}
