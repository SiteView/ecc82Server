// SmsSend(USMP).cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "SmsSend(USMP).h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "SmsSend.h"
#include <sys/stat.h>
#include <sys/types.h>

//
//	注意！
//
//		如果此 DLL 动态链接到 MFC
//		DLL，从此 DLL 导出并
//		调入 MFC 的任何函数在函数的最前面
//		都必须添加 AFX_MANAGE_STATE 宏。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CSmsSendUSMPApp

BEGIN_MESSAGE_MAP(CSmsSendUSMPApp, CWinApp)
END_MESSAGE_MAP()


// CSmsSendUSMPApp 构造

CSmsSendUSMPApp::CSmsSendUSMPApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSmsSendUSMPApp 对象

CSmsSendUSMPApp theApp;


// CSmsSendUSMPApp 初始化

BOOL CSmsSendUSMPApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	return TRUE;
}

bool InitInfo(SmsSend &s, char * destination, char * content);
bool SendMsg(SmsSend &s);
//打印日志函数
void WriteLog( const char* str )
{
//	return;
	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );
	char * p = datebuf;
	while(*p)
	{
		if(*p == '/')
			*p = '-';
		p++;
	}

	char szProgramName[128] = {0};
	sprintf(szProgramName , "%s.log" , "SmsSend(USMP)");
	string	strEccPath = GetSiteViewRootPath();
	char szLogFile[128];
	sprintf( szLogFile, "%s\\temp\\%s", 
		     strEccPath.c_str(), szProgramName );
	
	// 判断文件大小：在不打开文件的情况下实现
	struct _stat buf;
	if( _stat( szLogFile, &buf ) == 0 )
	{
		if( buf.st_size > 10000*1024 )
		{
			FILE *log = fopen( szLogFile, "w" );
			if( log != NULL )
				fclose( log );
		}
	}

	FILE *log = fopen( szLogFile,"a+");
	if( log != NULL )
	{
		fprintf( log, "%s \t%s\n", timebuf, str );
		fclose( log );
	}

}

extern "C" __declspec(dllexport)
int getinfo(string &retstr)
{
	retstr = "USMP短信平台";
	return 1;
}

bool InitInfo(SmsSend &s, char * destination, char * content)
{
	try
	{	
		list<string> DestIdList;
		char * pDest = NULL;
		char * strTok = ",";
		pDest = strtok(destination , strTok);
		while(pDest != NULL)
		{
			DestIdList.push_back(pDest);
			pDest = strtok(NULL , strTok);
		}
		bool bRet(false);
		bRet = s.GetDestID(DestIdList);
		if(!bRet)
		{
			WriteLog("获取目标手机号失败！");
			return false;
		}
			
		bRet = s.GetMsgContent(content);
		if(!bRet)
		{
			WriteLog("获取短信内容失败");
			return false;
		}

		bRet = s.InitServer();
		if(!bRet)
		{
			WriteLog("初始化服务器信息失败！");
			return false;
		}
		
		bRet = s.InitSocket();
		if(!bRet)
		{
			WriteLog("初始化Socket连接失败！");
			return false;
		}
	}
	catch(...)
	{
		DWORD dwErr = GetLastError();
		LPSTR lpBuffer = NULL;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwErr,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		char strMsg[128] = {0};
		sprintf(strMsg , "Exception occured in InitInfo! error number:%d\terror msg:%s" , dwErr , lpBuffer);
		LocalFree(lpBuffer);
		WriteLog(strMsg);
		return false;
	}
	return true;
}
inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
}

string urlEncoding( string &sIn )
{
	cout << "size: " << sIn.size() << endl;
	string sOut;
	for( int ix = 0; ix < sIn.size(); ix++ )
	{
		BYTE buf[4];
		memset( buf, 0, 4 );
		if( isalnum( (BYTE)sIn[ix] ) )
		{
			buf[0] = sIn[ix];
		}
		else if ( isspace( (BYTE)sIn[ix] ) )
		{
			buf[0] = '+';
		}
		else
		{
			buf[0] = '%';
			buf[1] = toHex( (BYTE)sIn[ix] >> 4 );
			buf[2] = toHex( (BYTE)sIn[ix] % 16);
		}
		sOut += (char *)buf;
	}
	return sOut;
}
void ConvertToHexStr(string strSour , string &strDes)
{
	for(int i=0; i<strSour.length(); i++)
	{
		char buf[2] = {0};
		sprintf(buf, "%0x", (unsigned char)strSour[i]);	
		strDes += buf;
	}
}
bool SendMsg(SmsSend &s)
{
	try
	{
		bool bRet = s.SendMsg();
		if(bRet)
			return true;
		else
			return false;
	}
	catch(...)
	{
		DWORD dwErr = GetLastError();
		LPSTR lpBuffer = NULL;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwErr,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		char strMsg[128] = {0};
		sprintf(strMsg , "Exception occured in SendMsg! error number:%d\terror msg:%s" , dwErr , lpBuffer);
		LocalFree(lpBuffer);
		WriteLog(strMsg);
		return false;
	}
	return true;
}
//extern "C" __declspec(dllexport) int run(char * szFunName, char * szSMTo, char *szContent)   
extern "C" __declspec(dllexport) int run(char * source, char * destination, char * content)
{
	SmsSend s;
	cout<<"test sms"<<endl;
	WriteLog(">>>>>>>>run start<<<<<<<<");
	try
	{
		bool bRet = InitInfo(s,destination, content);//"卓望短信");
		if(bRet)
		{
			bRet = SendMsg(s);
			if(bRet)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			WriteLog("Init failed!");
			return 0;
		}
	}
	catch(...)
	{
		DWORD dwErr = GetLastError();
		LPSTR lpBuffer = NULL;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dwErr,
			0,
			(LPTSTR)&lpBuffer,
			0,
			NULL
			);
		char strMsg[128] = {0};
		sprintf(strMsg , "Exception occured in run! error number:%d\terror msg:%s" , dwErr , lpBuffer);
		LocalFree(lpBuffer);
		WriteLog(strMsg);
		return 0;
	}
}