#include "stdafx.h"
#include <list>
#include <stdio.h>
#include <string.h>           
#include "TuxedoGet.h"
#include "TuxedoMonitorFunc.h"
#include <fstream>

using namespace std;
typedef struct std::list<char*> StringList;

bool MakeStringListByChar(StringList& pList, const char * pInput )
{
	const char * p;
	int nSize;
	p=pInput;
	while(*p!='\0')
	{
		nSize =strlen(p);
		if( nSize>0 )
		{	
			//pList.AddHead(p);
			pList.push_back((char*)p);

		}
		p=p+nSize+1;
	}

	return true;
}

int GetCharLength(const char * pInput)
{
	const char * p;
	int nSize = 0;
	p=pInput;
	while(*p!='\0')
	{
		nSize += strlen(p) + 1;
		p += strlen(p)+1;
	}

	 return nSize;
}

bool MakeCharByString(char *pOut,int &nOutSize,CString strInput )
{
	 char *p;
	
	int nSize=strInput.GetLength();
	if(nSize+2 <nOutSize)
	{
		strcpy(pOut,strInput.GetBuffer(strInput.GetLength()));
	}else return false;
	p=pOut;
	//printf("%d\n",nSize);23028830 13602067678 王波
	for(int i=0;i<nSize;i++)
	{
		if(*p=='$') 	
			*p='\0';
		p++;
	}
	nOutSize=nSize+1;
	return true;
	
}

char *FindStrValue(const char *strParas, CString str)
{
	char *pValue = NULL;
	string m_TempStr;

	std::list<char*> strList;
	MakeStringListByChar(strList, strParas);
	std::list<char *>::iterator pos = strList.begin();

	 while(pos != strList.end())
	{
		//CString strTemp = strList.GetNext(pos);
		char * strTemp = *pos;
		std::string strTemp1 = *pos;
		int m_Fpos = 0;
		
		if((m_Fpos = strTemp1.find(str, 0)) >= 0)
		{
			m_TempStr = strTemp1.substr( m_Fpos + strlen(str)+1, strTemp1.size() - strlen(str) - 1); 
			pValue=(char*)malloc(m_TempStr.size()+1);
			strcpy(pValue, m_TempStr.c_str());
			
		}
		pos++;
	}

	return pValue;
	
}


extern "C" __declspec(dllexport) 
BOOL TuxedoList(const char *strParas, char * strReturn, int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString		strServIp = FindStrValue(strParas, "_ServerIp");
	CString		strServPort = FindStrValue(strParas, "_ServerPort");
	CString strServerip = "WSNADDR=//"+strServIp+":"+strServPort;

	//设置机器环境
	tuxputenv((LPTSTR)(LPCTSTR)strServerip);
	tuxputenv("WSENVFILE=");

	int nMTID = 0;
	nMTID = atoi(FindStrValue(strParas, "_TemplateID"));
	BOOL bRet = FALSE;
	switch(nMTID)
    {
    case 310:
        bRet = GetMachineList(strParas, strReturn, nSize);
		break;
    case 311:
        bRet = GetServerList(strParas, strReturn, nSize);
        break;
    case 312:
        bRet = GetMsgList(strParas, strReturn, nSize);
        break;
    case 313:
        bRet = GetClientList(strParas, strReturn, nSize);
        break;
    case 314:
        bRet = GetServiceList(strParas, strReturn, nSize);
        break;
    case 315:
        bRet = GetWSHList(strParas, strReturn, nSize);
        break;
    default:
        return FALSE;
    }
	
	CString strOutRet;
	strOutRet =strReturn;
	nSize = 1024*10;
	MakeCharByString(strReturn,nSize,strOutRet);	 
	return bRet;
}

extern "C" __declspec(dllexport) 
BOOL TuxedoMachine(const char *strParas, char * strReturn, int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    CString		strServIp = FindStrValue(strParas, "_ServerIp");
	if(strServIp.IsEmpty())
	{
		sprintf(strReturn, "error=Server address is null!");
		return FALSE;
	}

	CString		strServPort = FindStrValue(strParas, "_ServerPort");
	if(strServPort.IsEmpty())
	{
        sprintf(strReturn, "error=Server port address is null!");
		return FALSE;
	}

	CString strTaskParam = FindStrValue(strParas, "_TaskParam");
	if(strTaskParam.IsEmpty())
	{
		sprintf(strReturn, "error=TaskParam is null!");
		return FALSE;
	}	


	HANDLE hOutputRead, hOutputWrite;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	CString strContent;
	
	if (CreatePipe(&hOutputRead, &hOutputWrite, &sa, 1024*100)) 
	{
		STARTUPINFO si;
		memset(&si, 0, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdOutput = hOutputWrite;
		si.hStdError = hOutputWrite;
		
		PROCESS_INFORMATION pi;
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));

		CString strcmd = "TuxedoGet.exe "+strServIp+" "+strServPort+" T_MACHINE"+" "+strTaskParam;

		OutputDebugString(strcmd.GetBuffer());

		if (CreateProcess(NULL, strcmd.GetBuffer(),  NULL, NULL, TRUE, /*CREATE_NEW_CONSOLE*/CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
		{
			try
			{
				if(WaitForSingleObject(pi.hProcess, 60000) == WAIT_OBJECT_0 )
				{
					char buffer[1024*10] = {0};
					DWORD bytesToRead = sizeof(buffer)-1;//保留一位放字符串结束符
					DWORD bytesRead = 0;
					int index = 0;

					if(GetFileSize(hOutputRead,NULL) > 0)//判断管道中是否有数据，必须有数据，方可执行ReadFile
					{
						while(ReadFile(hOutputRead, buffer, bytesToRead, &bytesRead, NULL))//用循环确保管道中的数据被全部读完
						{
							buffer[bytesRead] = 0;
							strContent += buffer;
							
							if(bytesToRead > bytesRead)//当读完管道中所有数据时，结束读数
							{
								break;
							}
						}
					}
				}

				else
				{
					sprintf(strReturn, "error=Dragonflow Timeout!");
					TerminateProcess(pi.hProcess, 1);
					
					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);

					CloseHandle(hOutputRead);
					CloseHandle(hOutputWrite);

					return FALSE;

				}
				
			}
			catch(...)
			{
				TerminateProcess(pi.hProcess, 1);
			}

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}

		CloseHandle(hOutputRead);
		CloseHandle(hOutputWrite);
	}
	
	int nOrder;
	if((nOrder= strContent.Find("FileSplitFlag"))>= 0)
	{
		nOrder += 13;
		strContent = strContent.Mid(nOrder);
		nOrder = strContent.Find("EndSplitFlag");
		strContent = strContent.Left(nOrder);
	}
	else
	{
		sprintf(strReturn, "error=init error!");
		return FALSE;
	}

	CString strOutRet;
	strOutRet = strContent;
	nSize = 1024*10;
	MakeCharByString(strReturn,nSize,strOutRet);	 
	return TRUE;
}


extern "C" __declspec(dllexport) 
BOOL TuxedoServer(const char *strParas, char * strReturn, int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strServIp = FindStrValue(strParas, "_ServerIp");
	if(strServIp.IsEmpty())
	{
		sprintf(strReturn, "error=Server address is null!");
		return FALSE;
	}

	CString		strServPort = FindStrValue(strParas, "_ServerPort");
	if(strServPort.IsEmpty())
	{
        sprintf(strReturn, "error=Server port address is null!");
		return FALSE;
	}

	CString strTaskParam = FindStrValue(strParas, "_TaskParam");
	if(strTaskParam.IsEmpty())
	{
		sprintf(strReturn, "error=TaskParam is null!");
		return FALSE;
	}	


	HANDLE hOutputRead, hOutputWrite;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	CString strContent;
	
	if (CreatePipe(&hOutputRead, &hOutputWrite, &sa, 1024*100)) 
	{
		STARTUPINFO si;
		memset(&si, 0, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdOutput = hOutputWrite;
		si.hStdError = hOutputWrite;
		
		PROCESS_INFORMATION pi;
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));

		CString strcmd = "TuxedoGet.exe "+strServIp+" "+strServPort+" T_SERVER"+" "+strTaskParam;

		OutputDebugString(strcmd.GetBuffer());

		if (CreateProcess(NULL, strcmd.GetBuffer(),  NULL, NULL, TRUE, /*CREATE_NEW_CONSOLE*/CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
		{
			try
			{
				if(WaitForSingleObject(pi.hProcess, 60000) == WAIT_OBJECT_0 )
				{
					char buffer[1024*10] = {0};
					DWORD bytesToRead = sizeof(buffer)-1;//保留一位放字符串结束符
					DWORD bytesRead = 0;
					int index = 0;

					if(GetFileSize(hOutputRead,NULL) > 0)//判断管道中是否有数据，必须有数据，方可执行ReadFile
					{
						while(ReadFile(hOutputRead, buffer, bytesToRead, &bytesRead, NULL))//用循环确保管道中的数据被全部读完
						{
							buffer[bytesRead] = 0;
							strContent += buffer;
							
							if(bytesToRead > bytesRead)//当读完管道中所有数据时，结束读数
							{
								break;
							}
						}
					}
				}

				else
				{
					sprintf(strReturn, "error=Dragonflow Timeout!");
					TerminateProcess(pi.hProcess, 1);
					
					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);

					CloseHandle(hOutputRead);
					CloseHandle(hOutputWrite);

					return FALSE;

				}
				
			}
			catch(...)
			{
				TerminateProcess(pi.hProcess, 1);
			}

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}

		CloseHandle(hOutputRead);
		CloseHandle(hOutputWrite);
	}
	
	int nOrder;
	if((nOrder= strContent.Find("FileSplitFlag"))>= 0)
	{
		nOrder += 13;
		strContent = strContent.Mid(nOrder);
		nOrder = strContent.Find("EndSplitFlag");
		strContent = strContent.Left(nOrder);
	}
	else
	{
		sprintf(strReturn, "error=init error!");
		return FALSE;
	}

	CString strOutRet;
	strOutRet = strContent;
	nSize = 1024*10;
	MakeCharByString(strReturn,nSize,strOutRet);	 
	return TRUE;

}

extern "C" __declspec(dllexport) 
BOOL TuxedoMsg(const char *strParas, char * strReturn, int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strServIp = FindStrValue(strParas, "_ServerIp");
	if(strServIp.IsEmpty())
	{
		sprintf(strReturn, "error=Server address is null!");
		return FALSE;
	}

	CString		strServPort = FindStrValue(strParas, "_ServerPort");
	if(strServPort.IsEmpty())
	{
        sprintf(strReturn, "error=Server port address is null!");
		return FALSE;
	}

	CString strTaskParam = FindStrValue(strParas, "_TaskParam");
	if(strTaskParam.IsEmpty())
	{
		sprintf(strReturn, "error=TaskParam is null!");
		return FALSE;
	}	


	HANDLE hOutputRead, hOutputWrite;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	CString strContent;
	
	if (CreatePipe(&hOutputRead, &hOutputWrite, &sa, 1024*100)) 
	{
		STARTUPINFO si;
		memset(&si, 0, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdOutput = hOutputWrite;
		si.hStdError = hOutputWrite;
		
		PROCESS_INFORMATION pi;
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));

		CString strcmd = "TuxedoGet.exe "+strServIp+" "+strServPort+" T_MSG"+" "+strTaskParam;

		OutputDebugString(strcmd.GetBuffer());

		if (CreateProcess(NULL, strcmd.GetBuffer(),  NULL, NULL, TRUE, /*CREATE_NEW_CONSOLE*/CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
		{
			try
			{
				if(WaitForSingleObject(pi.hProcess, 60000) == WAIT_OBJECT_0 )
				{
					char buffer[1024*10] = {0};
					DWORD bytesToRead = sizeof(buffer)-1;//保留一位放字符串结束符
					DWORD bytesRead = 0;
					int index = 0;

					if(GetFileSize(hOutputRead,NULL) > 0)//判断管道中是否有数据，必须有数据，方可执行ReadFile
					{
						while(ReadFile(hOutputRead, buffer, bytesToRead, &bytesRead, NULL))//用循环确保管道中的数据被全部读完
						{
							buffer[bytesRead] = 0;
							strContent += buffer;
							
							if(bytesToRead > bytesRead)//当读完管道中所有数据时，结束读数
							{
								break;
							}
						}
					}
				}

				else
				{
					sprintf(strReturn, "error=Dragonflow Timeout!");
					TerminateProcess(pi.hProcess, 1);
					
					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);

					CloseHandle(hOutputRead);
					CloseHandle(hOutputWrite);

					return FALSE;

				}
				
			}
			catch(...)
			{
				TerminateProcess(pi.hProcess, 1);
			}

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}

		CloseHandle(hOutputRead);
		CloseHandle(hOutputWrite);
	}
	
	int nOrder;
	if((nOrder= strContent.Find("FileSplitFlag"))>= 0)
	{
		nOrder += 13;
		strContent = strContent.Mid(nOrder);
		nOrder = strContent.Find("EndSplitFlag");
		strContent = strContent.Left(nOrder);
	}
	else
	{
		sprintf(strReturn, "error=init error!");
		return FALSE;
	}

	CString strOutRet;
	strOutRet = strContent;
	nSize = 1024*10;
	MakeCharByString(strReturn,nSize,strOutRet);	 
	return TRUE;

}

extern "C" __declspec(dllexport) 
BOOL TuxedoClient(const char *strParas, char * strReturn, int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strServIp = FindStrValue(strParas, "_ServerIp");
	if(strServIp.IsEmpty())
	{
		sprintf(strReturn, "error=Server address is null!");
		return FALSE;
	}

	CString		strServPort = FindStrValue(strParas, "_ServerPort");
	if(strServPort.IsEmpty())
	{
        sprintf(strReturn, "error=Server port address is null!");
		return FALSE;
	}

	CString strTaskParam = FindStrValue(strParas, "_TaskParam");
	if(strTaskParam.IsEmpty())
	{
		sprintf(strReturn, "error=TaskParam is null!");
		return FALSE;
	}	


	HANDLE hOutputRead, hOutputWrite;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	CString strContent;
	
	if (CreatePipe(&hOutputRead, &hOutputWrite, &sa, 1024*100)) 
	{
		STARTUPINFO si;
		memset(&si, 0, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdOutput = hOutputWrite;
		si.hStdError = hOutputWrite;
		
		PROCESS_INFORMATION pi;
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));

		CString strcmd = "TuxedoGet.exe "+strServIp+" "+strServPort+" T_CLIENT"+" "+strTaskParam;

		OutputDebugString(strcmd.GetBuffer());

		if (CreateProcess(NULL, strcmd.GetBuffer(),  NULL, NULL, TRUE, /*CREATE_NEW_CONSOLE*/CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
		{
			try
			{
				if(WaitForSingleObject(pi.hProcess, 60000) == WAIT_OBJECT_0 )
				{
					char buffer[1024*10] = {0};
					DWORD bytesToRead = sizeof(buffer)-1;//保留一位放字符串结束符
					DWORD bytesRead = 0;
					int index = 0;

					if(GetFileSize(hOutputRead,NULL) > 0)//判断管道中是否有数据，必须有数据，方可执行ReadFile
					{
						while(ReadFile(hOutputRead, buffer, bytesToRead, &bytesRead, NULL))//用循环确保管道中的数据被全部读完
						{
							buffer[bytesRead] = 0;
							strContent += buffer;
							
							if(bytesToRead > bytesRead)//当读完管道中所有数据时，结束读数
							{
								break;
							}
						}
					}
				}

				else
				{
					sprintf(strReturn, "error=Dragonflow Timeout!");
					TerminateProcess(pi.hProcess, 1);
					
					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);

					CloseHandle(hOutputRead);
					CloseHandle(hOutputWrite);

					return FALSE;

				}
				
			}
			catch(...)
			{
				TerminateProcess(pi.hProcess, 1);
			}

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}

		CloseHandle(hOutputRead);
		CloseHandle(hOutputWrite);
	}
	
	int nOrder;
	if((nOrder= strContent.Find("FileSplitFlag"))>= 0)
	{
		nOrder += 13;
		strContent = strContent.Mid(nOrder);
		nOrder = strContent.Find("EndSplitFlag");
		strContent = strContent.Left(nOrder);
	}
	else
	{
		sprintf(strReturn, "error=init error!");
		return FALSE;
	}

	CString strOutRet;
	strOutRet = strContent;
	nSize = 1024*10;
	MakeCharByString(strReturn,nSize,strOutRet);	 
	return TRUE;

}

extern "C" __declspec(dllexport) 
BOOL TuxedoService(const char *strParas, char * strReturn, int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strServIp = FindStrValue(strParas, "_ServerIp");
	if(strServIp.IsEmpty())
	{
		sprintf(strReturn, "error=Server address is null!");
		return FALSE;
	}

	CString		strServPort = FindStrValue(strParas, "_ServerPort");
	if(strServPort.IsEmpty())
	{
        sprintf(strReturn, "error=Server port address is null!");
		return FALSE;
	}

	CString strTaskParam = FindStrValue(strParas, "_TaskParam");
	if(strTaskParam.IsEmpty())
	{
		sprintf(strReturn, "error=TaskParam is null!");
		return FALSE;
	}	


	HANDLE hOutputRead, hOutputWrite;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	CString strContent;
	
	if (CreatePipe(&hOutputRead, &hOutputWrite, &sa, 1024*100)) 
	{
		STARTUPINFO si;
		memset(&si, 0, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdOutput = hOutputWrite;
		si.hStdError = hOutputWrite;
		
		PROCESS_INFORMATION pi;
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));

		CString strcmd = "TuxedoGet.exe "+strServIp+" "+strServPort+" T_SERVICE"+" "+strTaskParam;

		OutputDebugString(strcmd.GetBuffer());

		if (CreateProcess(NULL, strcmd.GetBuffer(),  NULL, NULL, TRUE, /*CREATE_NEW_CONSOLE*/CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
		{
			try
			{
				if(WaitForSingleObject(pi.hProcess, 60000) == WAIT_OBJECT_0 )
				{
					char buffer[1024*10] = {0};
					DWORD bytesToRead = sizeof(buffer)-1;//保留一位放字符串结束符
					DWORD bytesRead = 0;
					int index = 0;

					if(GetFileSize(hOutputRead,NULL) > 0)//判断管道中是否有数据，必须有数据，方可执行ReadFile
					{
						while(ReadFile(hOutputRead, buffer, bytesToRead, &bytesRead, NULL))//用循环确保管道中的数据被全部读完
						{
							buffer[bytesRead] = 0;
							strContent += buffer;
							
							if(bytesToRead > bytesRead)//当读完管道中所有数据时，结束读数
							{
								break;
							}
						}
					}
				}

				else
				{
					sprintf(strReturn, "error=Dragonflow Timeout!");
					TerminateProcess(pi.hProcess, 1);
					
					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);

					CloseHandle(hOutputRead);
					CloseHandle(hOutputWrite);

					return FALSE;

				}
				
			}
			catch(...)
			{
				TerminateProcess(pi.hProcess, 1);
			}

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}

		CloseHandle(hOutputRead);
		CloseHandle(hOutputWrite);
	}
	
	int nOrder;
	if((nOrder= strContent.Find("FileSplitFlag"))>= 0)
	{
		nOrder += 13;
		strContent = strContent.Mid(nOrder);
		nOrder = strContent.Find("EndSplitFlag");
		strContent = strContent.Left(nOrder);
	}
	else
	{
		sprintf(strReturn, "error=init error!");
		return FALSE;
	}

	CString strOutRet;
	strOutRet = strContent;
	nSize = 1024*10;
	MakeCharByString(strReturn,nSize,strOutRet);	 
	return TRUE;

}

extern "C" __declspec(dllexport) 
BOOL TuxedoWSH(const char *strParas, char * strReturn, int & nSize)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString		strServIp = FindStrValue(strParas, "_ServerIp");
	if(strServIp.IsEmpty())
	{
		sprintf(strReturn, "error=Server address is null!");
		return FALSE;
	}

	CString		strServPort = FindStrValue(strParas, "_ServerPort");
	if(strServPort.IsEmpty())
	{
        sprintf(strReturn, "error=Server port address is null!");
		return FALSE;
	}

	CString strTaskParam = FindStrValue(strParas, "_TaskParam");
	if(strTaskParam.IsEmpty())
	{
		sprintf(strReturn, "error=TaskParam is null!");
		return FALSE;
	}	


	HANDLE hOutputRead, hOutputWrite;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	CString strContent;
	
	if (CreatePipe(&hOutputRead, &hOutputWrite, &sa, 1024*100)) 
	{
		STARTUPINFO si;
		memset(&si, 0, sizeof(STARTUPINFO));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESTDHANDLES;
		si.hStdOutput = hOutputWrite;
		si.hStdError = hOutputWrite;
		
		PROCESS_INFORMATION pi;
		memset(&pi, 0, sizeof(PROCESS_INFORMATION));

		CString strcmd = "TuxedoGet.exe "+strServIp+" "+strServPort+" T_WSH"+" "+strTaskParam;

		OutputDebugString(strcmd.GetBuffer());

		if (CreateProcess(NULL, strcmd.GetBuffer(),  NULL, NULL, TRUE, /*CREATE_NEW_CONSOLE*/CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
		{
			try
			{
				if(WaitForSingleObject(pi.hProcess, 60000) == WAIT_OBJECT_0 )
				{
					char buffer[1024*10] = {0};
					DWORD bytesToRead = sizeof(buffer)-1;//保留一位放字符串结束符
					DWORD bytesRead = 0;
					int index = 0;

					if(GetFileSize(hOutputRead,NULL) > 0)//判断管道中是否有数据，必须有数据，方可执行ReadFile
					{
						while(ReadFile(hOutputRead, buffer, bytesToRead, &bytesRead, NULL))//用循环确保管道中的数据被全部读完
						{
							buffer[bytesRead] = 0;
							strContent += buffer;
							
							if(bytesToRead > bytesRead)//当读完管道中所有数据时，结束读数
							{
								break;
							}
						}
					}
				}

				else
				{
					sprintf(strReturn, "error=Dragonflow Timeout!");
					TerminateProcess(pi.hProcess, 1);
					
					CloseHandle(pi.hProcess);
					CloseHandle(pi.hThread);

					CloseHandle(hOutputRead);
					CloseHandle(hOutputWrite);

					return FALSE;

				}
				
			}
			catch(...)
			{
				TerminateProcess(pi.hProcess, 1);
			}

			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
		}

		CloseHandle(hOutputRead);
		CloseHandle(hOutputWrite);
	}
	
	int nOrder;
	if((nOrder= strContent.Find("FileSplitFlag"))>= 0)
	{
		nOrder += 13;
		strContent = strContent.Mid(nOrder);
		nOrder = strContent.Find("EndSplitFlag");
		strContent = strContent.Left(nOrder);
	}
	else
	{
		sprintf(strReturn, "error=init error!");
		return FALSE;
	}

	CString strOutRet;
	strOutRet = strContent;
	nSize = 1024*10;
	MakeCharByString(strReturn,nSize,strOutRet);	 
	return TRUE;

}


extern "C" __declspec(dllexport) 
BOOL TuxedoTest(const char *strParas, char * strReturn, int & nSize)
{
	//printf("----------- TuxedoTest Start ---------------");

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	printf("----------- TuxedoTest Start ---------------");

	CString		strServIp = FindStrValue(strParas, "_ServerIp");
	CString		strServPort = FindStrValue(strParas, "_ServerPort");
	CString strServerip = "WSNADDR=//"+strServIp+":"+strServPort;

	////设置机器环境
	tuxputenv((LPTSTR)(LPCTSTR)strServerip);
	tuxputenv("WSENVFILE=");

	GetMachineList(strParas, strReturn, nSize);

	int nCount = strlen(strReturn);

	printf("strReturn=%s",strReturn);

	sprintf(strReturn,"TestValue=%d$",nCount);
	CString strOutRet;
	strOutRet = strReturn;
	nSize = 1024*10;
	
	MakeCharByString(strReturn,nSize,strOutRet);

	printf("----------- TuxedoTest End ---------------");

	return TRUE;

}



