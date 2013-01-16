// TuxedoMonitor.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TuxedoMonitor.h"

#include "io.h"
#include "stdlib.h"
#include "stdio.h"
#include <fstream>
using namespace std;

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
// CTuxedoMonitorApp

BEGIN_MESSAGE_MAP(CTuxedoMonitorApp, CWinApp)
//{{AFX_MSG_MAP(CTuxedoMonitorApp)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTuxedoMonitorApp construction

CTuxedoMonitorApp::CTuxedoMonitorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTuxedoMonitorApp object

CTuxedoMonitorApp theApp;

//BOOL _WebLogicList(CStringList &paramList, CStringList &lstText, CStringList &lstValue);
BOOL _WebLogicList(CStringList &paramList, char * strReturn);

int WriteLog(const char* str)
{
	//#ifndef _DEBUG
	//		return 0;
	//#endif
	return 0;

	char timeBuf[128], dateBuf[128], wyt[4096];
	
	_tzset();
	
	_strtime( timeBuf );
	_strdate( dateBuf );
	
    sprintf(wyt, "%s %s\t", dateBuf, timeBuf );
	
	FILE* log;
		
	log = fopen("sanMonitor.log", "a+");
	
	if(log != NULL)
	{
		strcat(wyt, str);
		fprintf(log, "%s\n", wyt);
		fclose(log);
	}
	
	return 0;
}

BOOL GetProperty(const char * szProperty, const CString strSource, CString& strRet)
{
	BOOL bRet = FALSE;
	
	int nLeftIndex = strSource.Find(szProperty, 0);
	if (nLeftIndex == -1)
		return bRet;
	
	int nRightIndex = strSource.Find("$", nLeftIndex);
	if (nRightIndex == -1)
		return bRet;
	
	int nLength = strlen(szProperty);
	
	CString str = strSource.Mid(nLeftIndex + nLength , nRightIndex - nLeftIndex - nLength);
	strRet = str;
	
	bRet = TRUE;
	return bRet;
}

BOOL GetStringlistByMark(CStringList &lst,CString strContent, CString strMark)
{
	CString strTemp=_T("");
	int pos=-1;
	int curpos=0;

	if(strContent.IsEmpty())
		return FALSE;
	
    pos=strContent.Find(strMark,0);

    while(pos>0)
	{
		strTemp=strContent.Mid(curpos,pos-curpos);
		lst.AddTail(strTemp);
		curpos=pos+strlen(strMark);
		pos=strContent.Find(strMark,curpos);
	}

	return TRUE;
}






//调试 lish
bool FileLog(CString strFileName,const CString strLog)
{
	return true;

	char szDataBuf[128]		= _T("");
	char szTimeBuf[128]		= _T("");
	char szTempBuf[1024*10] = _T("");
	_strdate(szDataBuf);
	_strtime(szTimeBuf);
	sprintf(szTempBuf,"%s-%s",szDataBuf,szTimeBuf);

	CString szFileName="";
	szFileName.Format("%s.log",strFileName);

	ofstream stFileStream;
	stFileStream.open(szFileName, ios::app);
	stFileStream<<szTempBuf<<"\t"<<strLog<<endl;
	stFileStream.close();
	return true;
}

//输入参数拆分 将 "\0" 替换为 "; " 以便打印日志
bool MakeStringByChar(CString& strOut , const char * pInput )
{
	if (pInput == NULL)
	{
		return false;
	}
	//if (!strOut.IsEmpty())
	//{
	//	strOut.Empty();
	//}
	const char * p;
	int nSize;
	p=pInput;
	while(*p!='\0')
	{
		nSize =(int)strlen(p);
		if( nSize>0 )
		{
			strOut += p;
			strOut += "; ";
		}
		p=p+nSize+1;
	}

	return true;
}// end of MakeStringByChar

extern "C" __declspec(dllexport) 
BOOL SanStatus(const char * strParas, char * szReturn, int& nSize)
//(CStringList &paramList, char *szReturn)
{
	WriteLog("\n\n===============WebLogic===============");
	//////////////////////////////////////////////////////
	// 调试日志打印  lish
	CString strLog;
	CString strFileName="san";
	strLog = _T("Call san! Input Param: ");
	MakeStringByChar(strLog, strParas);
	FileLog(strFileName,strLog);
	//////////////////////////////////////////////////////

	AFX_MANAGE_STATE(AfxGetStaticModuleState());


	const char* pInput=strParas;
	int i=0;
	WriteLog("输入参数：");

	while(*pInput)
	{
        WriteLog(pInput);
		pInput=pInput+strlen(pInput)+1;
	}
	
	//参数解析：
	CString		
        strServIp = _T(""), 
		strServPort = _T(""), 
		strUserName = _T(""), 
		strUserPwd = _T(""),
		strNameSpace = _T(""),
		strProtocol = _T(""),
	    strTaskType=_T("");

	BOOL bRet = FALSE;
	
	// Check Content Change
	CStringList paramList;

	MakeStringListByChar(paramList,strParas);

	

	POSITION pos = paramList.GetHeadPosition();

	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);

		if(strTemp.Find(__MACHINENAME__, 0) == 0)
		{
			strServIp = strTemp.Right(strTemp.GetLength() - strlen(__MACHINENAME__));
		}
		else if(strTemp.Find(__PORT__, 0) == 0)
		{
			strServPort = strTemp.Right(strTemp.GetLength() - strlen(__PORT__));
		}
		else if(strTemp.Find(__USER__, 0) == 0)
		{
			strUserName = strTemp.Right(strTemp.GetLength() - strlen(__USER__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strUserPwd = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__NAMESPACE__, 0) == 0)
		{
			strNameSpace = strTemp.Right(strTemp.GetLength() - strlen(__NAMESPACE__));
		}
		else if(strTemp.Find(__TASKTYPE__, 0) == 0)
		{
			strTaskType = strTemp.Right(strTemp.GetLength() - strlen(__TASKTYPE__));
		}
		else if(strTemp.Find(__PROTOCOL__, 0) == 0)
		{
			strProtocol = strTemp.Right(strTemp.GetLength() - strlen(__PROTOCOL__));
		}
		else
		{
		
		}
	}
				
	if(strUserName.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("IDS_Poor_UserName"));//"缺少用户姓名!");//<%IDS_Monitor_40%>"缺少FTP服务器地址"
		return FALSE;
	}
	
	if(strUserPwd.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("IDS_Poor_Password"));//"缺少用户密码!");//<%IDS_Monitor_40%>
		//WriteLog("Poor_PassWord");
		return FALSE;
	}
	
	if(strServIp.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("IDS_Poor_ServerAddress"));//"缺少服务器地址!");//<%IDS_Monitor_40%>"缺少FTP服务器地址"
		return FALSE;
	}
	
	if(strServPort.IsEmpty())
	{
        sprintf(szReturn, "error=%s", FuncGetStringFromIDS("IDS_Poor_ServerPort"));//"缺少服务器端口地址!");//<%IDS_Monitor_40%>"缺少FTP服务器地址"
		return FALSE;
	}

	//if(strTaskType.IsEmpty())
	//{
	//	sprintf(szReturn, "error=%s", FuncGetStringFromIDS("IDS_Poor_TaskType"));//"缺少任务类型!");//<%IDS_Monitor_40%>"缺少FTP服务器地址"
	//	return FALSE;
	//}	



	WriteLog("输入参数验证通过！");
	strLog = "输入参数验证通过";
	FileLog(strFileName, strLog);
	



	
	//取数据的实现
	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\fcgi-bin", FuncGetInstallRootPath());

    //CString strCmdLine = "CMD /C " + strCustomerPath + "\\MonitorManager\\WeblogicMonitor.bat \"";
	CString strCmdLine;



	
		strCmdLine = "CMD /C \"\"" + strCustomerPath + "\\SiteViewSanPerfmon.bat\"";
		strCmdLine +=" \""+ strServIp +"\"";
		strCmdLine +=" \""+ strServPort +"\"";
		strCmdLine +=" \""+ strUserName +"\"";
		strCmdLine +=" \""+ strUserPwd +"\"";
		strCmdLine +=" \""+ strNameSpace +"\"";
		strCmdLine +=" \""+ strProtocol+"\"";
		strCmdLine +=" \"\"";
		strCmdLine +=" \"SanStatus\"\"";


	WriteLog("输出命令行：");
	WriteLog(strCmdLine.GetBuffer(strCmdLine.GetLength()));
	FileLog(strFileName,strCmdLine);
	/*
	CString strCmdLine= "CMD /C \"" + strCustomerPath + "\\WeblogicMonitor.bat\" ";
	strCmdLine += strUserName + " ";
	strCmdLine += strUserPwd + " ";
	strCmdLine += "t3://" + strServIp + ":" + strServPort;
	strCmdLine += " " + strTaskType;
	strCmdLine += " " + strTaskParam;
	*/

	SECURITY_ATTRIBUTES sa;	
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	
	HANDLE hRead, hWrite;
	long retval = CreatePipe(&hRead, &hWrite, &sa, 12000) ;
	if(retval == 0)
	{
		return false;
	}
	else
	{
		STARTUPINFO si;
		memset(&si, 0, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESTDHANDLES|STARTF_USESHOWWINDOW;
		si.hStdOutput = hWrite;
		si.hStdError = hWrite;
	//	si.wShowWindow =SW_HIDE;
		
		PROCESS_INFORMATION pi;
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));
		puts(strCmdLine);
		if (CreateProcess(NULL, strCmdLine.GetBuffer(strCmdLine.GetLength()),  &sa, &sa, TRUE, CREATE_NEW_CONSOLE/*CREATE_NO_WINDOW*/, NULL, NULL, &si, &pi)) 
		{
			bRet = TRUE;
			DWORD dwResult = WaitForSingleObject(pi.hProcess, 6000*1000);
			if (dwResult == WAIT_OBJECT_0) 
			{				
				DWORD dwExitCode = 0;
				if (GetExitCodeProcess(pi.hProcess, &dwExitCode)) 
				{
					//if (dwExitCode == 0) 
					{ 
						//
						CFile* pFile = NULL;
						int nFileLength = 0;
						TRY	
						{
							pFile = new CFile(hRead);								
							nFileLength = pFile->GetLength();
						}
						CATCH(CFileException, pEx) 
						{
							// Do Nothing.
						}
						END_CATCH
							
						char * buffer = NULL;
						buffer = (char*)malloc(nFileLength+1);
						
						//
						pFile->Read(buffer, nFileLength);

						WriteLog("buffer=");
						WriteLog(buffer);
						
						CString strSource = _T("");
						strSource.Format("%s", buffer);
						
						//WeblogicMonitor(去掉call setWLSEnv.cmd所生成的输出)
						int nOrder = strSource.Find("FileSplitFlag");
						nOrder += 13;
						strSource = strSource.Mid(nOrder);

						//
						nOrder = strSource.Find("EndSplitFlag");
						//nOrder += 13;
						strSource = strSource.Left(nOrder);
						
						strSource.Replace("WLSRunStatus","weblogic服务运行状态");
						strSource.Replace( "Error=","error=" );
						//生成正确格式的输出串
						strcpy(szReturn, strSource);
						
						//sprintf(szReturn, "error=%s",buffer);
						
						//
						free(buffer);
						
						if (pFile != NULL)   
						{
							pFile->Close();
							delete pFile;
						}					
					}
					//else
					{
						
					}
				}
				else if (dwResult == WAIT_TIMEOUT)
				{
					HANDLE hProcess;
					hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pi.dwProcessId);
					if (hProcess) 
					{
						TerminateProcess(hProcess, 1);
						CloseHandle(hProcess);
					}
					sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_UrlMonitor_3%>"));//<%IDS_UrlMonitor_3%>"error=下载超时$"
				}
				else 
				{
					
				}
			}
		}
		else 
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("IDS_DataCollectFailed"));//<%IDS_UrlMonitor_3%>"error=下载超时$"
		}
	}


	CString strInput;
	strInput =szReturn;

	WriteLog("\n\nresult=");
	WriteLog(szReturn);

	MakeCharByString(szReturn,nSize,strInput);
	
	return bRet;
}

	



