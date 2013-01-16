// TuxedoMonitor.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TuxedoMonitor.h"

#include "io.h"
#include "stdlib.h"
#include "stdio.h"
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
BOOL _WebLogicList(CStringList &paramList, char * strReturn, int& nSize);

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

extern "C" __declspec(dllexport) 
BOOL WebLogic(const char * strParas, char * szReturn, int& nSize)
//(CStringList &paramList, char *szReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	//参数解析：
	CString		
        strServIp = _T(""), 
		strServPort = _T(""), 
		strUserName = _T(""), 
		strUserPwd = _T(""),
		strTaskType = _T(""),
		strTaskParam = _T("");
	
	CString	
        strProxyServ = _T(""), 
		strProxyUser = _T(""); 
	
	int	nServerPort = 21;		
	int nTimeout = 60;

	BOOL bRet = FALSE;
	
	// Check Content Change
	CStringList paramList;

	MakeStringListByChar(paramList,strParas);
	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__USERNAME__, 0) == 0)
		{
			strUserName = strTemp.Right(strTemp.GetLength() - strlen(__USERNAME__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strUserPwd = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__SERVERIP__, 0) == 0)
		{
			strServIp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERIP__));
		}
		else if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strServPort = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
		}
		else if(strTemp.Find(__TASKTYPE__, 0) == 0)
		{
			strTaskType = strTemp.Right(strTemp.GetLength() - strlen(__TASKTYPE__));
		}
		else if(strTemp.Find(__TASKPARAM__, 0) == 0)
		{
			strTaskParam = strTemp.Right(strTemp.GetLength() - strlen(__TASKPARAM__));
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

	if(strTaskType.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("IDS_Poor_TaskType"));//"缺少任务类型!");//<%IDS_Monitor_40%>"缺少FTP服务器地址"
		return FALSE;
	}	

	if(strTaskParam.IsEmpty())
	{
		//为解决大部分Weblogic监视器不能工作问题，修改代码。修改开始  苏合 2007-09-18
		//strTaskParam = "null";
		strTaskParam = "";
		//为解决大部分Weblogic监视器不能工作问题，修改代码。修改结束  苏合 2007-09-18
	}	

	
	if(nTimeout <= 0)
	{
        sprintf(szReturn, "error=%s", FuncGetStringFromIDS("IDS_TimeoutParamError"));//"超时参数有错误!");//<%IDS_Monitor_40%>"缺少FTP服务器地址"		  		  
		return FALSE;
	}
	

	//取数据的实现
	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\fcgi-bin", FuncGetInstallRootPath());

    //CString strCmdLine = "CMD /C " + strCustomerPath + "\\MonitorManager\\WeblogicMonitor.bat \"";

	CString strCmdLine = "CMD /C \"" + strCustomerPath + "\\WeblogicMonitor.bat\" ";

	strCmdLine += strUserName + " ";
	strCmdLine += strUserPwd + " ";
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改开始  苏合 2007-09-18
	//strCmdLine += "t3://" + strServIp + ":" + strServPort;
	//strCmdLine += " " + strTaskType;
	//strCmdLine += " " + strTaskParam;
	strCmdLine += strServIp + " ";
	strCmdLine += strServPort + " ";
	strCmdLine += strTaskType + " ";
	strCmdLine += strTaskParam;
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改结束  苏合 2007-09-18

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
						
						strSource.Replace("WaitingForConnectionCurrentCount","WaitConnetCurCount");
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
	MakeCharByString(szReturn,nSize,strInput);
	
	return bRet;
}

	
extern "C" __declspec(dllexport) 
BOOL WebLogicList_ConnectionPoolInfo(const char * strParas, char * strReturn, int& nSize)
//(CStringList &paramList, CStringList &lstText, CStringList &lstValue)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改开始  苏合 2007-09-18
	//CString temp = "_TaskType==ConnectionPoolInfo";
	CString temp = "_TaskType=ConnectionPoolInfo";
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改结束  苏合 2007-09-18
	paramList.AddTail(temp);

	
	

	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改开始  苏合 2007-09-18
	//return (_WebLogicList(paramList,strReturn));
	return (_WebLogicList(paramList,strReturn,nSize));
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改结束  苏合 2007-09-18
}

extern "C" __declspec(dllexport) 
BOOL WebLogicList_ReqExecuteQueueInfo(const char * strParas, char * strReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改开始  苏合 2007-09-18
	//CString temp = "_TaskType==ReqExecuteQueueInfo";
	CString temp = "_TaskType=ReqExecuteQueueInfo";
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改结束  苏合 2007-09-18
	paramList.AddTail(temp);


	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改开始  苏合 2007-09-18
	//return (_WebLogicList(paramList,strReturn));
	return (_WebLogicList(paramList,strReturn,nSize));
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改结束  苏合 2007-09-18

}

extern "C" __declspec(dllexport) 
BOOL WebLogicList_WebApp(const char * strParas, char * strReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改开始  苏合 2007-09-18
	//CString temp = "_TaskType=WebApp";
	CString temp = "_TaskType=WebApp";
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改结束  苏合 2007-09-18
	paramList.AddTail(temp);

	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改开始  苏合 2007-09-18
	//return (_WebLogicList(paramList,strReturn));
	return (_WebLogicList(paramList,strReturn,nSize));
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改结束  苏合 2007-09-18
	
}

extern "C" __declspec(dllexport) 
BOOL WebLogicList_Cluster(const char * strParas, char * strReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改开始  苏合 2007-09-18
	//CString temp = "_TaskType==Cluster";
	CString temp = "_TaskType=Cluster";
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改结束  苏合 2007-09-18	
	paramList.AddTail(temp);

	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改开始  苏合 2007-09-18
	//return (_WebLogicList(paramList,strReturn));
	return (_WebLogicList(paramList,strReturn,nSize));
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改结束  苏合 2007-09-18
}

extern "C" __declspec(dllexport) 
BOOL WebLogicList_JVM(const char * strParas, char * strReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改开始  苏合 2007-09-18
	//CString temp = "_TaskType==JVM";
	CString temp = "_TaskType=JVM";
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改结束  苏合 2007-09-18
	paramList.AddTail(temp);

	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改开始  苏合 2007-09-18
	//return (_WebLogicList(paramList,strReturn));
	return (_WebLogicList(paramList,strReturn,nSize));
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改结束  苏合 2007-09-18
}

extern "C" __declspec(dllexport) 
BOOL WebLogicList_Server(const char * strParas, char * strReturn, int& nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStringList paramList;
	MakeStringListByChar(paramList,strParas);

	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改开始  苏合 2007-09-18
	//CString temp = "_TaskType==Server";
	CString temp = "_TaskType=Server";
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改结束  苏合 2007-09-18
	paramList.AddTail(temp);

	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改开始  苏合 2007-09-18
	//return (_WebLogicList(paramList,strReturn));
	return (_WebLogicList(paramList,strReturn,nSize));
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改结束  苏合 2007-09-18
}


//为解决大部分Weblogic监视器不能工作问题，修改代码。修改开始  苏合 2007-09-18
//inline BOOL _WebLogicList(CStringList &paramList, char * strReturn)
BOOL _WebLogicList(CStringList &paramList, char * strReturn, int& nSize)
//为解决大部分Weblogic监视器不能工作问题，修改代码。修改结束  苏合 2007-09-18
//(CStringList &paramList, CStringList &lstText, CStringList &lstValue)
{
	//参数解析：
	CString		
        strServIp = _T(""), 
		strServPort = _T(""), 
		strUserName = _T(""), 
		strUserPwd = _T(""),
		strTaskType = _T(""),
		strTaskParam = _T("");
	
	CString	
        strProxyServ = _T(""), 
		strProxyUser = _T("");
	
	int	nServerPort = 21;		
	int nTimeout = 60;

	BOOL bRet = FALSE;
	
	// Check Content Change
	
	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		if(strTemp.Find(__USERNAME__, 0) == 0)
		{
			strUserName = strTemp.Right(strTemp.GetLength() - strlen(__USERNAME__));
		}
		else if(strTemp.Find(__PASSWORD__, 0) == 0)
		{
			strUserPwd = strTemp.Right(strTemp.GetLength() - strlen(__PASSWORD__));
		}
		else if(strTemp.Find(__TIMEOUT__, 0) == 0)
		{
			nTimeout = atoi(strTemp.Right(strTemp.GetLength() - strlen(__TIMEOUT__)));
		}
		else if(strTemp.Find(__SERVERIP__, 0) == 0)
		{
			strServIp = strTemp.Right(strTemp.GetLength() - strlen(__SERVERIP__));
		}
		else if(strTemp.Find(__SERVERPORT__, 0) == 0)
		{
			strServPort = strTemp.Right(strTemp.GetLength() - strlen(__SERVERPORT__));
		}
		else if(strTemp.Find(__TASKTYPE__, 0) == 0)
		{
			strTaskType = strTemp.Right(strTemp.GetLength() - strlen(__TASKTYPE__));
		}
		else if(strTemp.Find(__TASKPARAM__, 0) == 0)
		{
			strTaskParam = strTemp.Right(strTemp.GetLength() - strlen(__TASKPARAM__));
		}
		else
		{
		
		}
	}
				
	if(strUserName.IsEmpty())
	{

		sprintf(strReturn,"error=%", FuncGetStringFromIDS("IDS_Poor_UserName")); //缺少用户姓名!");
		//return FALSE;
	}
	
	if(strUserPwd.IsEmpty())
	{
		sprintf(strReturn,"error=%", FuncGetStringFromIDS("IDS_Poor_Password")); //缺少服务器地址!");		
		//return FALSE;
	}
	
	if(strServIp.IsEmpty())
	{
		sprintf(strReturn,"error=%", FuncGetStringFromIDS("IDS_Poor_ServerPort")); //缺少服务器地址!");
		//return FALSE;
	}
	
	if(strServPort.IsEmpty())
	{
		sprintf(strReturn,"error=%", FuncGetStringFromIDS("IDS_Poor_ServerPort")); //缺少服务器端口地址!");
		//return FALSE;
	}

	if(strTaskType.IsEmpty())
	{
		sprintf(strReturn,"error=%", FuncGetStringFromIDS("IDS_Poor_TaskType")); //缺少任务类型!");
		//return FALSE;
	}	

	if(strTaskParam.IsEmpty())
	{
		//为解决大部分Weblogic监视器不能工作问题，修改代码。修改开始  苏合 2007-09-18
		//strTaskParam = "null";
		strTaskParam = "";
		//为解决大部分Weblogic监视器不能工作问题，修改代码。修改结束  苏合 2007-09-18
	}	

	
	if(nTimeout <= 0)
	{
		sprintf(strReturn,"error=%", FuncGetStringFromIDS("IDS_TimeoutParamError")); //超时参数有错误!");
		//return FALSE;
	}
	
	//取数据的实现

	/*5.6
	//取数据的实现
	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\cgi-bin", FuncGetInstallPath());

    CString strCmdLine = "CMD /C " + strCustomerPath + "\\WeblogicMonitor.bat ";
	strCmdLine += strUserName + " ";
	strCmdLine += strUserPwd + " ";
	strCmdLine += "t3://" + strServIp + ":" + strServPort;
	strCmdLine += " " + strTaskType;
	strCmdLine += " " + strTaskParam;
	*/

	//取数据的实现
	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\fcgi-bin", FuncGetInstallRootPath());

    //CString strCmdLine = "CMD /C " + strCustomerPath + "\\MonitorManager\\WeblogicMonitor.bat ";

	CString strCmdLine = "CMD /C \"" + strCustomerPath + "\\WeblogicMonitor.bat\" ";

	strCmdLine += strUserName + " ";
	strCmdLine += strUserPwd + " ";
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改开始  苏合 2007-09-18
	//strCmdLine += "t3://" + strServIp + ":" + strServPort;
	//strCmdLine += " " + strTaskType;
	//strCmdLine += " " + strTaskParam;
	strCmdLine += strServIp + " ";
	strCmdLine += strServPort + " ";
	strCmdLine += strTaskType + " ";
	strCmdLine += strTaskParam;
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改结束  苏合 2007-09-18

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
		//si.wShowWindow =SW_HIDE;
        si.wShowWindow =SW_SHOW;
		
		PROCESS_INFORMATION pi;
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));
		OutputDebugString(strCmdLine.GetBuffer(strCmdLine.GetLength()));
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
						
						//生成正确格式的输出串
						
						//为解决大部分Weblogic监视器不能工作问题，修改代码。修改开始  苏合 2007-09-18
						//strcpy(szReturn, strSource);
						strcpy(strReturn, strSource);
						//注释此段
					/*CStringList tmpList;
						GetStringlistByMark(tmpList, strSource, "*@");
						
						POSITION pos = tmpList.GetHeadPosition();
						char *ca;
						ca =strReturn;
						while(pos)
						{
						
							CString strTemp = tmpList.GetNext(pos);
							
							int nPos = strTemp.Find("=");

							//stText.AddTail(strTemp.Right(strTemp.GetLength() - nPos - 1));
							//lstValue.AddTail(strTemp.Right(strTemp.GetLength() - nPos - 1));
							
							
							sprintf(ca,"%s=%s",  strTemp.Right(strTemp.GetLength() - nPos - 1),
								strTemp.Right(strTemp.GetLength() - nPos - 1));
							//puts(strTemp);
							ca=ca+2*( strTemp.GetLength() - nPos - 1)+2;
						}
						//
						*/
						//为解决大部分Weblogic监视器不能工作问题，修改代码。修改结束  苏合 2007-09-18
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
					
					sprintf(strReturn, "error=%", FuncGetStringFromIDS("IDS_DataCollectFailed")); //数据采集失败");					
				}
				else 
				{

					
				}
			}
		}
		else 
		{
			sprintf(strReturn, "error=%", FuncGetStringFromIDS("IDS_DataCollectFailed")); //数据采集失败");					
			//lstText.AddTail("error=数据采集失败$");
		}
	}

	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改开始  苏合 2007-09-18
	CString strInput;
	strInput =strReturn;
	MakeCharByString(strReturn,nSize,strInput);
	//为解决大部分Weblogic监视器不能工作问题，修改代码。修改结束  苏合 2007-09-18
	
	return bRet;
}


