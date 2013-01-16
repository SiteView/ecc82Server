// winScripts.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "Dragonflow.h" //include <OAIdl.h>  exe不需要
#include <atlstr.h> //CString
#include <list>
#include <fstream>
#include <iostream>



#pragma comment(lib,"ws2_32.lib")

using namespace std;



BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}

typedef class list<char*> StringList;
BOOL IsLocalHost(CString sHostName);
CString FindStrValue(const char *strParas, CString str);
bool MakeStringListByChar(StringList& pList, const char * pInput );
bool MakeStringListByChar(list<string> & pList, const char * pInput );
int executeExeFile(CString strCmdLine, CString &strContent);
bool isCmdSuccess(const char *fileName, char *matchStr);

extern "C" _declspec(dllexport) 
BOOL ExecWinScripts(const char * strParas, char * szReturn, int& nSize)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strMachine=_T("");
	CString strUser = _T("");
	CString strPSWD = _T("");
	CString strRemoteProgramFile = _T("");
	strMachine = FindStrValue(strParas, "_MachineName");
	strUser  = FindStrValue(strParas, "_UserAccount");
	strPSWD  = FindStrValue(strParas, "_PassWord");
	strRemoteProgramFile  = FindStrValue(strParas, "_remoteProgramPath");
	

	CoInitialize( NULL );
	_Alert *myScriptRef = NULL;

	
	HRESULT hr=CoCreateInstance(CLSID_Alert,NULL,
		CLSCTX_ALL,
		IID__Alert,(void **)&myScriptRef);

	if(SUCCEEDED(hr))
	{
		printf("creat com success\n");
		//CoUninitialize();
	}
	else 
	{
		printf("creat com failed\n");
		//CoUninitialize();
		return false;
	}

	char  cToServer[56] = {0},
		cLast[56] = {0};

	VARIANT  a,b,c,d,e, vResult;
	BSTR bstrTmp;
	VariantInit(&a);
	VariantInit(&b);
	VariantInit(&c);
	VariantInit(&d);
	VariantInit(&e);
	VariantInit(&vResult);

	//CString strServer("192.168.9.106");	
	////CString strUser("SiteViewEcc");
	////CString strPwd("DF2000sv!@#");
	//CString strUser("administrator");
	//CString strPwd("888888");
	
	cout << "strRemoteProgramFile = " << strRemoteProgramFile << endl;
	CString strToServer(strRemoteProgramFile);

	cout << "strToServer = " << strToServer << endl;
	CString strLast("");

	bstrTmp=strMachine.AllocSysString();
	a.vt=VT_BSTR;
	a.bstrVal=bstrTmp;

	bstrTmp=strUser.AllocSysString();
	b.bstrVal=bstrTmp;
	b.vt=VT_BSTR;

	bstrTmp=strPSWD.AllocSysString();
	c.vt=VT_BSTR;
	c.bstrVal=bstrTmp;

	bstrTmp=strToServer.AllocSysString();
	d.bstrVal=bstrTmp;
	d.vt=VT_BSTR;

	bstrTmp=strLast.AllocSysString();
	e.vt=VT_BSTR;
	e.bstrVal=bstrTmp;

	printf( "开始：" );
	myScriptRef->alertPlaysound(&a,&b,&c,&d,&e,&vResult);

	CString strResult(vResult.bstrVal);
	printf( "\n返回结果：" );
	printf( strResult.GetBuffer() );

	VariantClear(&a);
	VariantClear(&b);
	VariantClear(&c);
	VariantClear(&d);
	VariantClear(&e);
	VariantClear(&vResult);

	CoUninitialize();

	Sleep(1000);
	try
	{
		//CString strMachine = "";
		//CString strUser = "";
		//CString strPsword = "";
		CString strMonitorID = "";
		CString strMatchStr = "";
		CString remoteFileName ="";
		CString driverName = "";

		//strMachine = FindStrValue(strParas, "_MachineName");
		//strUser  = FindStrValue(strParas, "_UserAccount");
		//strPsword  = FindStrValue(strParas, "_PassWord");
		strMonitorID = FindStrValue(strParas, "_MonitorID");
		strMatchStr = FindStrValue(strParas, "_MatchStr");
		remoteFileName = FindStrValue(strParas, "_remoteDataFilePath");
		driverName = remoteFileName.Mid(0,1); //取磁盘名

		char  strErrorFile1[128] = {0};
		sprintf(strErrorFile1,"C:\\%s_Error1.log",strMonitorID.GetBuffer(strMonitorID.GetLength()));

		char cmdLine[256] = {0};
		sprintf(cmdLine, "cmd.exe /k \"net use \\\\%s\\%s$ %s /user:%s > %s && exit\"",
			strMachine.GetBuffer(strMachine.GetLength()),
			driverName.GetBuffer(driverName.GetLength()),strPSWD.GetBuffer(strPSWD.GetLength()),
			strUser.GetBuffer(strUser.GetLength()),strErrorFile1);

	//	cout << "cmdline1 = " << cmdLine << endl;
		CString content = "";
		executeExeFile(cmdLine, content);

		//if (!content.IsEmpty())
		//{
		//	cout << "cmd faild"<< endl;
		//	return FALSE;
		//}
		cout << "content = " << content << endl;

		if (!isCmdSuccess(strErrorFile1, "成功"))
		{
			printf("exe cmd error\n");
			sprintf(szReturn, "error=连接失败");
			return FALSE;
		}

		char  localFileName[128] = {0};
		char  strErrorFile2[128] = {0};

		sprintf(strErrorFile2,"C:\\%s_Error2.log",strMonitorID.GetBuffer(strMonitorID.GetLength()));
		sprintf(localFileName,"C:\\%s_dataFile.log",strMonitorID.GetBuffer(strMonitorID.GetLength()));

		strMachine.ReleaseBuffer();

		remoteFileName = remoteFileName.Mid(2,remoteFileName.GetLength()-2); //把"C:"去掉
		//cout << "remoteFileName = " << remoteFileName;
		memset(cmdLine, 0, sizeof(cmdLine));
		sprintf(cmdLine, "cmd.exe /k \"copy \\\\%s\\%s$\\%s %s > %s && exit\"",
			strMachine.GetBuffer(strMachine.GetLength()),
			driverName.GetBuffer(driverName.GetLength()),
			remoteFileName.GetBuffer(remoteFileName.GetLength()),
			localFileName,strErrorFile2);

		//	cout << "cmdline = " << cmdLine << endl;

		CString content2 = "";
		executeExeFile(cmdLine, content2);

		//if (!content.IsEmpty())
		//{
		//	cout << "cmd2 faild"<< endl;
		//	return FALSE;
		//}
		cout << "content2 = " << content2 << endl;

		if (!isCmdSuccess(strErrorFile2, "已复制"))
		{
			printf("exe cmd error\n");
			sprintf(szReturn, "error=找不到网络路径");

			return FALSE;
		}

		//结果是否匹配字符
		if (!isCmdSuccess(localFileName, strMatchStr.GetBuffer(strMatchStr.GetLength())))
		{
			printf("exe cmd error\n");
			strcpy(szReturn, "isFind=0$");	
		}
		else
		{
			printf("CHECK OK OK\n");
			strcpy(szReturn, "isFind=1$");
		}
	}
	catch(...) 
	{ 
		sprintf(szReturn, "error=Error ocured: %d",::GetLastError());
		return FALSE;
	} 
	return TRUE;
}

BOOL IsLocalHost(CString sHostName)
{
	sHostName.TrimRight();
	sHostName.MakeLower();
	if(sHostName == "127.0.0.1" || sHostName == "localhost")
		return TRUE;

	WSADATA wsaData;
	char name[155];
	char *ip;
	PHOSTENT hostinfo; 
	if ( WSAStartup( MAKEWORD(2,0), &wsaData ) == 0 ) 
	{ 
		if( gethostname ( name, sizeof(name)) == 0) 
		{ 
			if(sHostName == name)
				return TRUE;
			if((hostinfo = gethostbyname(name)) != NULL) 
			{ //这些就是获得IP的函数
				char strTemp[128] = {0};
				int i = 0;

				while(hostinfo->h_addr_list[i]!=NULL)
				{
					//ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list); 
					ip = inet_ntoa (*(struct in_addr *)hostinfo->h_addr_list[i]);
					if(strcmp(sHostName.GetBuffer(1), ip) == 0)
					{
						printf("IP =%s\n",ip);
						return TRUE;
					}
					i++;
				}

			} 
			//			if((hostinfo = gethostbyname(name)) != NULL) 
			//			{ //这些就是获得IP的函数
			//				ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list); 
			////				 ip = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list[i]);
			//
			//				if(sHostName == ip)
			//				{
			//					printf("IP =%s\n",ip);
			//					return TRUE;
			//				}
			//			} 
		} 
		WSACleanup( );
	}
	return FALSE;
}

CString FindStrValue(const char *strParas, CString str)
{
	//char *pValue = NULL;
	CString strValue = "";
	string m_TempStr;

	list< string > strList;
	MakeStringListByChar(strList, strParas);
	list< string >::iterator pos = strList.begin();

	while(pos != strList.end())
	{
		//CString strTemp = strList.GetNext(pos);
		//char * strTemp = *pos;
		string strTemp1 = *pos;
		int m_Fpos = 0;

		if((m_Fpos = static_cast<int>(strTemp1.find(str, 0))) >= 0)
		{
			m_TempStr = strTemp1.substr( m_Fpos + strlen(str)+1, strTemp1.size() - strlen(str) - 1); 
			//pValue=(char*)malloc(m_TempStr.size()+1);
			//strcpy(pValue, m_TempStr.c_str());
			strValue = m_TempStr.c_str();

		}
		pos++;
	}

	return strValue;
}

bool MakeStringListByChar(StringList& pList, const char * pInput )
{
	const char * p;
	int nSize;
	p=pInput;
	while(*p!='\0')
	{
		nSize =static_cast<int>((strlen(p)));
		if( nSize>0 )
		{	
			//pList.AddHead(p);
			pList.push_back((char*)p);

		}
		p=p+nSize+1;
	}

	return true;
}

bool MakeStringListByChar(list<string> & pList, const char * pInput )
{
	const char * p;
	int nSize;
	p=pInput;
	while(*p!='\0')
	{
		nSize =(int)strlen(p);
		if( nSize>0 )
			pList.push_back(p);
		p=p+nSize+1;
	}

	return true;
}

//段毅 dos命令返回的结果输入到fileName里面，再查看里面是否有和matchStr匹配的字段，有就是成功
bool isCmdSuccess(const char *fileName, char *matchStr)
{
	//printf("isCmdSuccess fileName = %s\n", fileName);
	FILE *fp = NULL;
	char errorMes[100] = {0};

	fp = fopen(fileName, "r");
	if (fp==NULL)
	{
		printf("file isn't exist\n");
		return false;
	}
	fread(errorMes, 1, 100, fp);
	fclose(fp);

	printf("errorMes = %s\n", errorMes);

	char *p = NULL;
	p = strstr(errorMes, matchStr);

	if (p==NULL)
	{
		printf("cmd error\n");
		DeleteFile(fileName);
		return false;
	}
	else
	{
		printf("cmd ok\n");
	}

	if (DeleteFile(fileName))
	{
		//cout << "DeleteFile " << fileName << " OK" << endl;
	}
	else
	{
		//cout << "DeleteFile " << fileName << " faild" << endl;
		return false;
	}

	return true;
}

int executeExeFile(CString strCmdLine, CString &strContent)
{
	HANDLE hOutputRead, hOutputWrite;
	SECURITY_ATTRIBUTES sa;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;

	PROCESS_INFORMATION pi;  //dy
	DWORD dwResult;			 //dy

	try
	{
		if (CreatePipe(&hOutputRead, &hOutputWrite, &sa, 1024*50)) 
		{

			STARTUPINFO si;
			memset(&si, 0, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			si.dwFlags = STARTF_USESTDHANDLES;
			si.hStdOutput = hOutputWrite;
			si.hStdError = hOutputWrite;

			memset(&pi, 0, sizeof(PROCESS_INFORMATION));

			if (CreateProcess(NULL, strCmdLine.GetBuffer(strCmdLine.GetLength()),  NULL, NULL, TRUE, /*CREATE_NEW_CONSOLE*/CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
			{

				if(WaitForSingleObject(pi.hProcess, 180000) == WAIT_OBJECT_0 )
				{
					//关闭写句柄
					if (!CloseHandle(hOutputWrite)) 
					{
						OutputDebugString("关闭写句柄失败！");
					}


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
					strContent="Timeout";
					//即使时间超时，也不结束子进程cmd.exe，而是让其自己结束！
				}

				CloseHandle(pi.hProcess);
				CloseHandle(pi.hThread);
			}

			CloseHandle(hOutputRead);
			GetExitCodeThread(pi.hProcess, &dwResult);

		}
	}
	catch(...)
	{
		CloseHandle(hOutputRead);
		CloseHandle(hOutputWrite);
	}
	strCmdLine.ReleaseBuffer();//dy2010-11-02 GetBuffer 最好有ReleaseBuffer，免得内存泄露
	return 1;
}