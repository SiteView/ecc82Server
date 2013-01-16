// NnmSvdbAgent.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <Ice/Application.h>
#include "NnmMethodsI.h"

#include "LogUtils.h"			// ��־ʵ�ú���

///////////////////////////////////////////////////////////////////
// svdb �ӿ�ͷ�ļ�
#include "svapi.h"

///////////////////////////////////////////////////////////////////

using namespace std;
using namespace SV_LogUtils;
///////////////////////////////////////////////////////////////////

string g_dbPName	= "";
char *g_account		= NULL;
char *g_password	= NULL;

#define NSA_ADPTER_NAME		"NNMMethod"		// Ice ����������
#define NSA_SERANT_NAME		"NNMMethod"		// Ice Servant����

///////////////////////////////////////////////////////////////////

// IceӦ�ó�����
class NnmSvdbAgent : public Ice::Application
{
public:

    virtual int run(int, char*[]);
};

int NnmSvdbAgent::run(int argc, char* argv[])
{
	try
	{
		cout << "==>> Communicator create adapter ( " << NSA_ADPTER_NAME << " )  ... ..." << endl;
		
		// Ice ͨ��������������
		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter( NSA_ADPTER_NAME );
	    
		cout << "==>> Create adapter done!" << endl;
		cout << "==>> Add servant ( " << NSA_SERANT_NAME << " ) to adapter... ..." << endl;
	
		// Ice ��������� Servant
		adapter->add( new NnmMethodsI, communicator()->stringToIdentity( NSA_SERANT_NAME ));

		cout << "==>> Servant added!" << endl;

		adapter->activate();	// ����������

		
		cout << "==>> Adapter activate( wait for shut down ) ... ..." << endl << endl;

		communicator()->waitForShutdown();	// ͨ�����ȴ�����
		communicator()->destroy();			// ����ͨ����
	}
	catch(...)
	{
		cout << "**** Exception to new NnmMethodsI." << endl;
		return -1;
	}
    
	return EXIT_SUCCESS;
}

//////////////////////////////////////////////////////
// ȡ�������ļ���ȫ·���ļ���
void gfGetConfigFilePathName( char *pFilePathName );



//////////////////////////////////////////////////////
// �������
int _tmain(int argc, _TCHAR* argv[])
{
	////////////////////////////////////////////////
	// ��ʼ����־����
	SvLog::SetLogFileName( "C:\\Svl_SvdbAgent.log" );
	SvLog::SetDumpFileName( "C:\\Svl_SvdbAgent.dmp" );
	SvLog::Write( "============================================================", 0, __FILE__, __LINE__ );
	SvLog::Write( "###### SVDB Agent for NNM log from this line", 0, __FILE__, __LINE__ );

	SetSvdbAddrByFile( "svapi.ini" );
//������ svdb.exe ͨѶ��ǿ�Ƶ�ַ,�����õ��� svapi.dll �Ľ��������� svapi.dll ������ string addr 
//ע�⣺һ�����øú������ú󣬸ý������������������ﴫ�ݸ� svapi.dll ������ string addr �����������������øú������õ� addr
	

	cout << endl;
	cout << "********************************************" << endl;
	cout << "======== SVDB Agent for NNM startup ========" << endl;
	cout << "********************************************" << endl;
	cout << endl;

	NnmSvdbAgent app;	// Ice Ӧ�ó������

	// ȡ�������ļ���ȫ·���ļ���
	char szFilePathName[ _MAX_PATH ];
	memset( szFilePathName, 0x00, sizeof( szFilePathName ) );
	gfGetConfigFilePathName( szFilePathName );

	cout << "Loading config file ... ...\n";
	cout << szFilePathName << endl;

	// ���� Ice Ӧ�ó���
	int nRet = app.main( argc, argv, szFilePathName );


	cout << "Exit code: " << nRet << endl;
	cout << "------------------------------------------" << endl;
	cout << "-------- NNM SVDB Agent shutdown! --------" << endl;
	cout << "------------------------------------------" << endl;

    return nRet;

}


//////////////////////////////////////////////////////
// ȡ�������ļ���ȫ·���ļ���
void gfGetConfigFilePathName( char *pFilePathName )
{
	char   path[ _MAX_PATH ];
	::GetModuleFileName( NULL, (LPTSTR)path, _MAX_PATH );		// ȡ�õ�ǰ����ȫ·���ļ���
	int nIdx = strlen( path ) - 1;
	// �ӵ�ǰ����ȫ·���ļ����н�������ǰ·��(����"\")
	for( ; nIdx > 0; nIdx-- )
	{
		if( '\\' == path[ nIdx ] )
		{
			break;
		}
	}

	// ��װ�����ļ�ȫ·���ļ���
	memcpy( pFilePathName, path, nIdx + 1 );
	strcat( pFilePathName, "config.server" );	// �ļ���Ϊ config.server

}
////////////////////////////////////////////////////// 