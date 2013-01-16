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
bool Tuxedo(const char *pcszParam, char *szReturn, int &nSize)
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
    int nMTID = 0;

	bool bRet = false;
	
	// Check Content Change
    CStringList paramList;
	MakeStringListByChar(paramList, pcszParam);

	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		
		if(strTemp.Find(__TIMEOUT__, 0) == 0)
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
        else if(strTemp.Find("_TemplateID=", 0) == 0)
        {
            nMTID = atoi(strTemp.Right(strTemp.GetLength() - strlen("_TemplateID=")));
        }
	}
	
	if(strServIp.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_73%>"));//<%IDS_Monitor_73%>"缺少服务器"
		return false;
	}
	
	if(strServPort.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_74%>"));//<%IDS_Monitor_74%>"输入端口错误"
		return false;
	}

    strTaskType = "";
    switch(nMTID)
    {
    case 310:
        strTaskType = "T_MACHINE";
        break;
    case 311:
        strTaskType = "T_SERVER";
        break;
    case 312:
        strTaskType = "T_MSG";
        break;
    case 313:
        strTaskType = "T_CLIENT";
        break;
    case 314:
        strTaskType = "T_SERVICE";
        break;
    case 315:
        strTaskType = "T_WSH";
        break;
    }

	if(strTaskType.IsEmpty())
	{
		sprintf(szReturn, "error=%s", FuncGetStringFromIDS("IDS_Poor_Task"));
		return false;
	}

	if(strTaskParam.IsEmpty())
	{
		strTaskParam = "null";
	}	
	
	if(nTimeout <= 0)
	{
        sprintf(szReturn, "error=%s", FuncGetStringFromIDS("<%IDS_Monitor_11%>"));
		return false;
	}
	
	//取数据的实现
	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\fcgi-bin", FuncGetInstallPath());

    CString strCmdLine = strCustomerPath + "\\TuxedoGet.exe \"";
	strCmdLine += " ";
	strCmdLine +=  strServIp;
	strCmdLine += " ";
	strCmdLine += strServPort;
	strCmdLine += " ";
	strCmdLine += strTaskType;
	strCmdLine += " ";
	strCmdLine += strTaskParam;

	strCmdLine="CMD /C \"" + strCmdLine;
	OutputDebugString("command line output");
	OutputDebugString(strCmdLine.GetBuffer(strCmdLine.GetLength()));
	OutputDebugString("\n");
	
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

						    char * buffer = NULL;
						    buffer = new char[nFileLength + 1];						
						    pFile->Read(buffer, nFileLength);
    						
						    CString strSource = _T("");
						    strSource.Format("%s", buffer);
    						
						    //WeblogicMonitor(去掉call setWLSEnv.cmd所生成的输出)
						    int nOrder = strSource.Find("FileSplitFlag");
						    nOrder += 14;
						    strSource = strSource.Mid(nOrder);

						    nOrder = strSource.Find("EndSplitFlag");
						    strSource = strSource.Left(nOrder);
    						
						    //生成正确格式的输出串
						    //strcpy(szReturn, strSource);
							char dbuf[256] = {0};
							sprintf(dbuf, "before size:%d\n", strSource.GetLength());
							OutputDebugString(dbuf);
														
							strSource.TrimRight(" ");
							strSource.TrimRight("$");							

                            MakeCharByString(szReturn, nSize, strSource);
						    delete []buffer;

						    if (pFile != NULL)   
						    {
							    pFile->Close();
							    delete pFile;
						    }
                        }
						CATCH(CFileException, pEx)
						{
							// Do Nothing.
						}
                        END_CATCH
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
			}
		}
		else 
		{
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("IDS_DataCollectFailed"));//<%IDS_UrlMonitor_3%>"error=下载超时$"
		}
	}
	
	return bRet;
}

extern "C" __declspec(dllexport) 
bool TuxedoList(const char *strParas, char * strReturn, int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	char	szReturn[1024] = {0};
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
    int nMTID = 0;

	bool bRet = false;
	
	CStringList paramList;

	MakeStringListByChar(paramList,strParas);
	
	POSITION pos = paramList.GetHeadPosition();
	while(pos)
	{
		CString strTemp = paramList.GetNext(pos);
		
		if(strTemp.Find(__TIMEOUT__, 0) == 0)
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
        else if(strTemp.Find("_TemplateID=", 0) == 0)
        {
            nMTID = atoi(strTemp.Right(strTemp.GetLength() - strlen("_TemplateID=")));
        }
	}

	OutputDebugString("------------start TuxedoMonitor---------\n");
	
	if(strServIp.IsEmpty())
	{
		return false;
	}
	
	if(strServPort.IsEmpty())
	{
		return false;
	}

	
	switch(nMTID)
    {
    case 310:
        strTaskType = "T_MACHINE";
        break;
    case 311:
        strTaskType = "T_SERVER";
        break;
    case 312:
        strTaskType = "T_MSG";
        break;
    case 313:
        strTaskType = "T_CLIENT";
        break;
    case 314:
        strTaskType = "T_SERVICE";
        break;
    case 315:
        strTaskType = "T_WSH";
        break;
    default:
        return false;
    }

	

	if(strTaskParam.IsEmpty())
	{
		strTaskParam = "null";
	}	
	
	if(nTimeout <= 0)
	{
        return false;
	}
		
	
	//取数据的实现
	CString		strCustomerPath = _T("");
	strCustomerPath.Format("%s\\fcgi-bin", FuncGetInstallPath());

    CString strCmdLine = strCustomerPath + "\\TuxedoGet.exe\" ";
	strCmdLine += " ";
	strCmdLine +=  strServIp;
	strCmdLine += " ";
	strCmdLine += strServPort;
	strCmdLine += " ";
	strCmdLine += strTaskType;
	strCmdLine += " ";
	strCmdLine += strTaskParam;

	strCmdLine="CMD /C \"" + strCmdLine;


	OutputDebugString("---------tuxedo command line--------\n");
	OutputDebugString(strCmdLine.GetBuffer(strCmdLine.GetLength()));
	OutputDebugString("\n");

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

							
						    char * buffer = NULL;
						    buffer = (char*)malloc(nFileLength+1);
    						
						    //
						    pFile->Read(buffer, nFileLength);
    						
						    CString strSource = _T("");
						    strSource.Format("%s", buffer);

							OutputDebugString("-----------tuxedoget.exe return value-----------\n");
							OutputDebugString(strSource.GetBuffer(strSource.GetLength()));
							OutputDebugString("\n");
    						
						    //WeblogicMonitor(去掉call setWLSEnv.cmd所生成的输出)
						    int nOrder = strSource.Find("FileSplitFlag");
						    nOrder += 13;
						    strSource = strSource.Mid(nOrder);

						    //
						    nOrder = strSource.Find("EndSplitFlag");
						    //nOrder += 13;
						    strSource = strSource.Left(nOrder);
    						
						    //生成正确格式的输出串
						    CStringList tmpList;
						    GetStringlistByMark(tmpList, strSource, "*@");
						
                            CString strBuffer = _T("");
						    POSITION pos = tmpList.GetHeadPosition();
						    while(pos)
						    {
							    CString strTemp = tmpList.GetNext(pos);
                                strBuffer += strTemp;								
                                strBuffer += "$";

								OutputDebugString("--------*@ -> $----------\n");
								OutputDebugString(strBuffer.GetBuffer(strBuffer.GetLength()));
								OutputDebugString("\n");
							    //int nPos = strTemp.Find("=");

							    //lstText.AddTail(strTemp.Right(strTemp.GetLength() - nPos - 1));
							    //lstValue.AddTail(strTemp.Right(strTemp.GetLength() - nPos - 1));
						    }
                            strBuffer = strBuffer.Left(strBuffer.GetLength() - 1);
                            MakeCharByString(szReturn, nSize, strBuffer);

							
						//
						    free(buffer);
    						
						    if (pFile != NULL)   
						    {
							    pFile->Close();
							    delete pFile;
						    }
                        }
						CATCH(CFileException, pEx) 
						{
							// Do Nothing.
						}
						END_CATCH
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
					
					sprintf(szReturn, "error=%s", FuncGetStringFromIDS("IDS_DataCollectFailed"));				
				}
				else 
				{
					
				}
			}
		}
		else 
		{
			OutputDebugString("------------tuxedoget.exe process failure-----------\n");
			sprintf(szReturn, "error=%s", FuncGetStringFromIDS("IDS_DataCollectFailed"));
		}
	}
	
	memcpy(strReturn, szReturn, nSize);
	OutputDebugString("--------result end----------\n");
	char rbuf[256] = {0};
	sprintf(rbuf, "%s size:%d\n", szReturn, nSize);
	OutputDebugString(rbuf);
	OutputDebugString("\n");
	return bRet;
}


