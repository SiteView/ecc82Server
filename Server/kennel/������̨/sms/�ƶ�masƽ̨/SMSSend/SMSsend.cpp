#include <string>
#include <windows.h>

using namespace std;

#include "ImApi.h"
#pragma comment(lib, "ImApi.lib")
//===============================================================
// 接口函数声明
//===============================================================



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
	retstr = "连接移动MAS服务器短信平台";
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
	char szIp[64] = {0};
	char szUserName[256] = {0};
	char szPwd[256] = {0};
	char szDBName[256] = {0};
	char szAPICode[256] = {0};

	string strTmp = szFunName;
	WriteLog( szFunName );

	basic_string<char>::size_type pos,off(0);
	while( (pos=strTmp.find("\"", off) )!= basic_string<char>::npos )
	{
		strTmp.replace( pos, 1, "" );

		off = pos + 1;
	}

	WriteLog( strTmp.c_str() );

	sscanf( strTmp.c_str(), "%s %s %s %s %s", szIp, szUserName, szPwd, szDBName, szAPICode );

	if( strlen(szIp) == 0 )
	{
		WriteLog( "服务器IP为空！" );
		return 0;
	}
	else
	{
		printf( "服务器IP：%s\n", szIp );
	}

	if( strlen(szUserName) == 0 )
	{
		WriteLog( "用户名为空！" );
		return 0;
	}
	else
	{
		printf( "用户名：%s\n", szUserName );
	}

	if( strlen(szPwd) == 0 )
	{
		WriteLog( "密码为空！" );
		return 0;
	}
	else
	{
		printf( "密码：%s\n", szPwd );
	}

	if( strlen(szDBName) == 0 )
	{
		WriteLog( "数据库名为空！" );
		return 0;
	}
	else
	{
		printf( "数据库名：%s\n", szDBName );
	}

	if( strlen(szAPICode) == 0 )
	{
		WriteLog( "api代码为空！" );
		return 0;
	}
	else
	{
		printf( "api代码：%s\n", szAPICode );
	}


	int ret(0);

	int nResult = initWithDB( szIp, szUserName, szPwd, szDBName, szAPICode );

	char szBuf[1024] = {0};

	if(nResult == 0)
	{
		sprintf(szBuf, " 初始化成功");
		WriteLog(szBuf);
	} 
	else
	{
		sprintf(szBuf, " 初始化失败，错误代码：%d",nResult);
		WriteLog(szBuf);
		return 0;
	}  

	try
	{
		ret = sendSM( szSMTo, szContent, 0 );
	}
	catch( ... )
	{

	}

	
	if(ret == 0)//成功
	{
		sprintf(szBuf, "提交发送短信成功！");
		WriteLog(szBuf);
		release();
		return 1;
	}
	else
	{
		sprintf(szBuf, "发送失败， 错误代码：%d\n", ret );
		WriteLog(szBuf);
		release();
		return 0;
	}

}