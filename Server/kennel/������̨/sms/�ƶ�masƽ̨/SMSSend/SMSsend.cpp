#include <string>
#include <windows.h>

using namespace std;

#include "ImApi.h"
#pragma comment(lib, "ImApi.lib")
//===============================================================
// �ӿں�������
//===============================================================



//===============================================================
// ��ӡ��־����
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

	// �ж��ļ���С���ڲ����ļ��������ʵ��
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
// ��ӡ��־����
//===============================================================
extern "C" __declspec(dllexport) int getinfo( string& retstr )
{
	retstr = "�����ƶ�MAS����������ƽ̨";
	return 1;
}


//===============================================================
// ��ȡ����
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

	printf( "�����ļ�Ŀ¼��%s\n", szFileName );
	
	GetPrivateProfileString( "Common", "ServerName", "", pszServerName, 128, szFileName );
	GetPrivateProfileString( "Common", "Account", "", pszAccount, 128, szFileName );
	GetPrivateProfileString( "Common", "Password", "", pszPassword, 128, szFileName );
	GetPrivateProfileString( "Common", "EIEName", "", pszEIEName, 128, szFileName );
	GetPrivateProfileString( "Common", "EIEPassword", "", pszEIEPassword, 128, szFileName );

	GetPrivateProfileString( "Common", "SMSTo", "", pszSMSTo, 128, szFileName );


}


//===============================================================
//
//��̬���ӿ�����
//funname,���õķ��Ͷ��ŵĺ���
//szSMTo�����ŷ��͸�˭
//szContent,��������
//����Ϊ1��ʾ�ɹ�
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
		WriteLog( "������IPΪ�գ�" );
		return 0;
	}
	else
	{
		printf( "������IP��%s\n", szIp );
	}

	if( strlen(szUserName) == 0 )
	{
		WriteLog( "�û���Ϊ�գ�" );
		return 0;
	}
	else
	{
		printf( "�û�����%s\n", szUserName );
	}

	if( strlen(szPwd) == 0 )
	{
		WriteLog( "����Ϊ�գ�" );
		return 0;
	}
	else
	{
		printf( "���룺%s\n", szPwd );
	}

	if( strlen(szDBName) == 0 )
	{
		WriteLog( "���ݿ���Ϊ�գ�" );
		return 0;
	}
	else
	{
		printf( "���ݿ�����%s\n", szDBName );
	}

	if( strlen(szAPICode) == 0 )
	{
		WriteLog( "api����Ϊ�գ�" );
		return 0;
	}
	else
	{
		printf( "api���룺%s\n", szAPICode );
	}


	int ret(0);

	int nResult = initWithDB( szIp, szUserName, szPwd, szDBName, szAPICode );

	char szBuf[1024] = {0};

	if(nResult == 0)
	{
		sprintf(szBuf, " ��ʼ���ɹ�");
		WriteLog(szBuf);
	} 
	else
	{
		sprintf(szBuf, " ��ʼ��ʧ�ܣ�������룺%d",nResult);
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

	
	if(ret == 0)//�ɹ�
	{
		sprintf(szBuf, "�ύ���Ͷ��ųɹ���");
		WriteLog(szBuf);
		release();
		return 1;
	}
	else
	{
		sprintf(szBuf, "����ʧ�ܣ� ������룺%d\n", ret );
		WriteLog(szBuf);
		release();
		return 0;
	}

}