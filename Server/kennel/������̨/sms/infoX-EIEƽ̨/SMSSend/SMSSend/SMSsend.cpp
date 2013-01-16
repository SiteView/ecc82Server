#include <string>
#include <windows.h>

using namespace std;
//===============================================================
// �ӿں�������
//===============================================================
// ��ʼ���ӿ�
typedef int ( __stdcall *STARTUP )( char* Server, char* Account, char* Password );
// ��¼������
typedef int ( __stdcall *LOGIN )( char* Operator, char* Password );
// ������Ϣ
typedef int (__stdcall *SUBMITSHORTMESSAGE)( char* AtTime, 
											 char* SourceAddr, 
											 char* DestAddr, 
											 char* Content, 
											 unsigned long ContentLen, 
											 int NeedStateReport, 
											 char* ServiceID, 
											 char* FeeType, 
											 char* FeeCode );

// ע���û�
typedef int ( __stdcall *LOGOUT )();
// �ͷŽӿ�
typedef void ( __stdcall *CLEANUP )(void);



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
	retstr = "���ӻ�ΪMAS����������ƽ̨";
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
	// ��ȡ����
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
		WriteLog( "�ӿ��ڲ�ʹ�����ݿ�ķ���������Ϊ�գ�" );
		return 0;
	}
	else
	{
		printf( "����������%s\n", szServerName );
	}

	if( strlen(szAccount) == 0 )
	{
		WriteLog( "�������ݿ���ʺ�����Ϊ�գ�" );
		return 0;
	}
	else
	{
		printf( "���ݿ���ʺ�����%s\n", szAccount );
	}

	if( strlen(szPassword) == 0 )
	{
		WriteLog( "�������ݿ���ʺ�����Ϊ�գ�" );
		return 0;
	}
	else
	{
		printf( "���ݿ���ʺ����룺%s\n", szPassword );
	}

	if( strlen(szEIEName) == 0 )
	{
		WriteLog( "��ҵ��Ϣ���ʺ���Ϊ�գ�" );
		return 0;
	}
	else
	{
		printf( "��ҵ��Ϣ���ʺ�����%s\n", szEIEName );
	}

	if( strlen(szEIEPassword) == 0 )
	{
		WriteLog( "��ҵ��Ϣ���ʺ�����Ϊ�գ�" );
		return 0;
	}
	else
	{
		printf( "��ҵ��Ϣ���ʺ����룺%s\n", szEIEPassword );
	}
	*/



	// ��ʾ���붯̬��
	HMODULE hSMSDll = NULL;
	hSMSDll = LoadLibrary( "SMEntry.dll" );

	if( hSMSDll == NULL )
	{
		WriteLog( "����SMEntry.dllʧ�ܣ�" );
	}
	else
	{
		// ��ʼ���ӿ�
		STARTUP pStartUp = NULL;
		pStartUp = (STARTUP)GetProcAddress( hSMSDll, "SP_Startup" );
		if( pStartUp == NULL )
		{
			WriteLog( "��ȡSP_Startupʧ�ܣ�" );
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
				sprintf( szTemp, "�ӿڳ�ʼ��ʧ�ܣ�������=%d", nRetVal );
				WriteLog( szTemp );
				FreeLibrary( hSMSDll );
				return 0;
			}			
		}

		// ��¼��ΪMAS������
		LOGIN pLogin = NULL;
		pLogin = (LOGIN)GetProcAddress( hSMSDll, "SP_Login" );
		if( pLogin == NULL )
		{
			WriteLog( "��ȡSP_Loginʧ�ܣ�" );
		}
		else
		{
			int nRetVal = pLogin( szEIEName, szEIEPassword );
			if( nRetVal != 0 )
			{
				char szTemp[1024] = {0};
				sprintf( szTemp, "��¼ʧ�ܣ�������=%d", nRetVal );
				WriteLog( szTemp );
				goto clean;
			}	

		}

		// ������Ϣ
		SUBMITSHORTMESSAGE pSubmit = NULL;
		pSubmit = (SUBMITSHORTMESSAGE)GetProcAddress( hSMSDll, "SubmitShortMessage" );
		if( pSubmit == NULL )
		{
			WriteLog( "��ȡSubmitShortMessageʧ�ܣ�" );
		}
		else
		{
			int nRetVal = pSubmit( NULL, NULL, szSMTo, szContent, strlen(szContent), 1, "EIE", "02", "10" );
			if( nRetVal != 0 )
			{
				char szTemp[1024] = {0};
				sprintf( szTemp, "���͵�������ʧ�ܣ�������=%d", nRetVal );
				WriteLog( szTemp );
			}	

		}

		// ע���û�
		LOGOUT pLogOut = NULL;
		pLogOut = (LOGOUT)GetProcAddress( hSMSDll, "SP_Logout" );
		if( pLogOut == NULL )
		{
			WriteLog( "��ȡSP_Logoutʧ�ܣ�" );
		}
		else
		{
			int nRetVal = pLogOut();
			if( nRetVal != 0 )
			{
				char szTemp[1024] = {0};
				sprintf( szTemp, "ע��ʧ�ܣ�������=%d", nRetVal );
				WriteLog( szTemp );
			}	

		}


clean:	// �ͷŽӿ�
		CLEANUP pCleanUp = NULL;
		pCleanUp = (CLEANUP)GetProcAddress( hSMSDll, "SP_Cleanup" );
		if( pCleanUp == NULL )
		{
			WriteLog( "��ȡSP_Cleanupʧ�ܣ�" );
		}
		else
		{
			pCleanUp();	
		}


		// �ͷŶ�̬��
		FreeLibrary( hSMSDll );	
		return 1;
	}
}