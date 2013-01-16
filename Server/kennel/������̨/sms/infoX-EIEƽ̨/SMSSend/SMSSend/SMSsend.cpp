#include <string>
#include <windows.h>

using namespace std;
//===============================================================
// 接口函数声明
//===============================================================
// 初始化接口
typedef int ( __stdcall *STARTUP )( char* Server, char* Account, char* Password );
// 登录服务器
typedef int ( __stdcall *LOGIN )( char* Operator, char* Password );
// 发送信息
typedef int (__stdcall *SUBMITSHORTMESSAGE)( char* AtTime, 
											 char* SourceAddr, 
											 char* DestAddr, 
											 char* Content, 
											 unsigned long ContentLen, 
											 int NeedStateReport, 
											 char* ServiceID, 
											 char* FeeType, 
											 char* FeeCode );

// 注销用户
typedef int ( __stdcall *LOGOUT )();
// 释放接口
typedef void ( __stdcall *CLEANUP )(void);



//===============================================================
// 打印日志函数
//===============================================================
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>

void WriteLog( const char* str )
{
	char timebuf[128],datebuf[128];

	_tzset();
	_strtime( timebuf );
	_strdate( datebuf );

	char szLogFile[] = "SMSSend.log";

	// 判断文件大小：在不打开文件的情况下实现
	struct _stat buf;
	if( _stat( szLogFile, &buf ) == 0 )
	{
		if( buf.st_size > 10*1024 )
		{
			FILE *log = fopen( szLogFile, "w" );
			if( log != NULL )
				fclose( log );
		}
	}


	FILE *log = fopen( szLogFile,"a+");
	if( log != NULL )
	{
		fprintf( log, "%s %s \t%s\n", datebuf, timebuf, str );
		fclose( log );
	}

}


//===============================================================
// 打印日志函数
//===============================================================
extern "C" __declspec(dllexport) int getinfo( string& retstr )
{
	retstr = "连接华为MAS服务器短信平台";
	return 1;
}


//===============================================================
// 获取参数
//===============================================================
void GetParam( char* pszServerName, char* pszAccount, char* pszPassword,
			   char* pszEIEName, char* pszEIEPassword, char* pszSMSTo )
{
	char szFileName[1024] = {0};
	GetModuleFileName( NULL, szFileName, sizeof(szFileName) );

	char* pszEnd = strrchr( szFileName, '\\' );
	if( pszEnd != NULL )
		*pszEnd = '\0';

	strcat( szFileName, "\\SMSSend.ini" );

	printf( "配置文件目录：%s\n", szFileName );
	
	GetPrivateProfileString( "Common", "ServerName", "", pszServerName, 128, szFileName );
	GetPrivateProfileString( "Common", "Account", "", pszAccount, 128, szFileName );
	GetPrivateProfileString( "Common", "Password", "", pszPassword, 128, szFileName );
	GetPrivateProfileString( "Common", "EIEName", "", pszEIEName, 128, szFileName );
	GetPrivateProfileString( "Common", "EIEPassword", "", pszEIEPassword, 128, szFileName );

	GetPrivateProfileString( "Common", "SMSTo", "", pszSMSTo, 128, szFileName );


}


//===============================================================
//
//动态链接库的入口
//funname,调用的发送短信的函数
//szSMTo，短信发送给谁
//szContent,短信内容
//返回为1表示成功
//
//===============================================================
extern "C" __declspec(dllexport) int run( char* szFunName, char* szSMTo, char* szContent )   
{
	// 获取参数
	char szServerName[128] = {0};
	char szAccount[128] = {0};
	char szPassword[128] = {0};

	char szEIEName[128] = {0};
	char szEIEPassword[128] = {0};

	char szSMSTo[128] = {0};
	char szSMSContent[1024] = "This is test!";
	szSMTo = szSMSTo;
	szContent = szSMSContent;

	GetParam( szServerName, szAccount, szPassword, szEIEName, szEIEPassword, szSMSTo );

	/*
	if( strlen(szServerName) == 0 )
	{
		WriteLog( "接口内部使用数据库的服务器名称为空！" );
		return 0;
	}
	else
	{
		printf( "服务器名：%s\n", szServerName );
	}

	if( strlen(szAccount) == 0 )
	{
		WriteLog( "连接数据库的帐号名称为空！" );
		return 0;
	}
	else
	{
		printf( "数据库的帐号名：%s\n", szAccount );
	}

	if( strlen(szPassword) == 0 )
	{
		WriteLog( "连接数据库的帐号密码为空！" );
		return 0;
	}
	else
	{
		printf( "数据库的帐号密码：%s\n", szPassword );
	}

	if( strlen(szEIEName) == 0 )
	{
		WriteLog( "企业信息机帐号名为空！" );
		return 0;
	}
	else
	{
		printf( "企业信息机帐号名：%s\n", szEIEName );
	}

	if( strlen(szEIEPassword) == 0 )
	{
		WriteLog( "企业信息机帐号密码为空！" );
		return 0;
	}
	else
	{
		printf( "企业信息机帐号密码：%s\n", szEIEPassword );
	}
	*/



	// 显示载入动态库
	HMODULE hSMSDll = NULL;
	hSMSDll = LoadLibrary( "SMEntry.dll" );

	if( hSMSDll == NULL )
	{
		WriteLog( "载入SMEntry.dll失败！" );
	}
	else
	{
		// 初始化接口
		STARTUP pStartUp = NULL;
		pStartUp = (STARTUP)GetProcAddress( hSMSDll, "SP_Startup" );
		if( pStartUp == NULL )
		{
			WriteLog( "获取SP_Startup失败！" );
		}
		else
		{
			char szUser[1024] = {0};
			char szPass[1024] = {0};

			//sprintf( szUser, "%s,%s", szAccount, szEIEName );
			//sprintf( szPass, "%s,%s", szPassword, szEIEPassword );
			int nRetVal = pStartUp( szServerName, szAccount, szPassword );
			WriteLog( szServerName );
			WriteLog( szAccount );
			WriteLog( szPassword );

			if( nRetVal != 0 )
			{
				char szTemp[1024] = {0};
				sprintf( szTemp, "接口初始化失败，错误码=%d", nRetVal );
				WriteLog( szTemp );
				FreeLibrary( hSMSDll );
				return 0;
			}			
		}

		// 登录华为MAS服务器
		LOGIN pLogin = NULL;
		pLogin = (LOGIN)GetProcAddress( hSMSDll, "SP_Login" );
		if( pLogin == NULL )
		{
			WriteLog( "获取SP_Login失败！" );
		}
		else
		{
			int nRetVal = pLogin( szEIEName, szEIEPassword );
			if( nRetVal != 0 )
			{
				char szTemp[1024] = {0};
				sprintf( szTemp, "登录失败，错误码=%d", nRetVal );
				WriteLog( szTemp );
				goto clean;
			}	

		}

		// 发送信息
		SUBMITSHORTMESSAGE pSubmit = NULL;
		pSubmit = (SUBMITSHORTMESSAGE)GetProcAddress( hSMSDll, "SubmitShortMessage" );
		if( pSubmit == NULL )
		{
			WriteLog( "获取SubmitShortMessage失败！" );
		}
		else
		{
			int nRetVal = pSubmit( NULL, NULL, szSMTo, szContent, strlen(szContent), 1, "EIE", "02", "10" );
			if( nRetVal != 0 )
			{
				char szTemp[1024] = {0};
				sprintf( szTemp, "发送单条短信失败，错误码=%d", nRetVal );
				WriteLog( szTemp );
			}	

		}

		// 注销用户
		LOGOUT pLogOut = NULL;
		pLogOut = (LOGOUT)GetProcAddress( hSMSDll, "SP_Logout" );
		if( pLogOut == NULL )
		{
			WriteLog( "获取SP_Logout失败！" );
		}
		else
		{
			int nRetVal = pLogOut();
			if( nRetVal != 0 )
			{
				char szTemp[1024] = {0};
				sprintf( szTemp, "注销失败，错误码=%d", nRetVal );
				WriteLog( szTemp );
			}	

		}


clean:	// 释放接口
		CLEANUP pCleanUp = NULL;
		pCleanUp = (CLEANUP)GetProcAddress( hSMSDll, "SP_Cleanup" );
		if( pCleanUp == NULL )
		{
			WriteLog( "获取SP_Cleanup失败！" );
		}
		else
		{
			pCleanUp();	
		}


		// 释放动态库
		FreeLibrary( hSMSDll );	
		return 1;
	}
}