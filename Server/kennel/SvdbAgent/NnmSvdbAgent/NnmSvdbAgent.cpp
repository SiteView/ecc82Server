// NnmSvdbAgent.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <Ice/Application.h>
#include "NnmMethodsI.h"

#include "LogUtils.h"			// 日志实用函数

///////////////////////////////////////////////////////////////////
// svdb 接口头文件
#include "svapi.h"

///////////////////////////////////////////////////////////////////

using namespace std;
using namespace SV_LogUtils;
///////////////////////////////////////////////////////////////////

string g_dbPName	= "";
char *g_account		= NULL;
char *g_password	= NULL;

#define NSA_ADPTER_NAME		"NNMMethod"		// Ice 适配器名称
#define NSA_SERANT_NAME		"NNMMethod"		// Ice Servant名称

///////////////////////////////////////////////////////////////////

// Ice应用程序类
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
		
		// Ice 通信器创建适配器
		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter( NSA_ADPTER_NAME );
	    
		cout << "==>> Create adapter done!" << endl;
		cout << "==>> Add servant ( " << NSA_SERANT_NAME << " ) to adapter... ..." << endl;
	
		// Ice 适配器添加 Servant
		adapter->add( new NnmMethodsI, communicator()->stringToIdentity( NSA_SERANT_NAME ));

		cout << "==>> Servant added!" << endl;

		adapter->activate();	// 激活适配器

		
		cout << "==>> Adapter activate( wait for shut down ) ... ..." << endl << endl;

		communicator()->waitForShutdown();	// 通信器等待结束
		communicator()->destroy();			// 销毁通信器
	}
	catch(...)
	{
		cout << "**** Exception to new NnmMethodsI." << endl;
		return -1;
	}
    
	return EXIT_SUCCESS;
}

//////////////////////////////////////////////////////
// 取得配置文件的全路径文件名
void gfGetConfigFilePathName( char *pFilePathName );



//////////////////////////////////////////////////////
// 程序入口
int _tmain(int argc, _TCHAR* argv[])
{
	////////////////////////////////////////////////
	// 初始化日志环境
	SvLog::SetLogFileName( "C:\\Svl_SvdbAgent.log" );
	SvLog::SetDumpFileName( "C:\\Svl_SvdbAgent.dmp" );
	SvLog::Write( "============================================================", 0, __FILE__, __LINE__ );
	SvLog::Write( "###### SVDB Agent for NNM log from this line", 0, __FILE__, __LINE__ );

	SetSvdbAddrByFile( "svapi.ini" );
//设置与 svdb.exe 通讯的强制地址,即设置调用 svapi.dll 的进程中所有 svapi.dll 函数的 string addr 
//注意：一旦调用该函数设置后，该进程中所有其他代码里传递给 svapi.dll 函数的 string addr 都将被抛弃，而引用该函数设置的 addr
	

	cout << endl;
	cout << "********************************************" << endl;
	cout << "======== SVDB Agent for NNM startup ========" << endl;
	cout << "********************************************" << endl;
	cout << endl;

	NnmSvdbAgent app;	// Ice 应用程序对象

	// 取得配置文件的全路径文件名
	char szFilePathName[ _MAX_PATH ];
	memset( szFilePathName, 0x00, sizeof( szFilePathName ) );
	gfGetConfigFilePathName( szFilePathName );

	cout << "Loading config file ... ...\n";
	cout << szFilePathName << endl;

	// 进入 Ice 应用程序
	int nRet = app.main( argc, argv, szFilePathName );


	cout << "Exit code: " << nRet << endl;
	cout << "------------------------------------------" << endl;
	cout << "-------- NNM SVDB Agent shutdown! --------" << endl;
	cout << "------------------------------------------" << endl;

    return nRet;

}


//////////////////////////////////////////////////////
// 取得配置文件的全路径文件名
void gfGetConfigFilePathName( char *pFilePathName )
{
	char   path[ _MAX_PATH ];
	::GetModuleFileName( NULL, (LPTSTR)path, _MAX_PATH );		// 取得当前程序全路径文件名
	int nIdx = strlen( path ) - 1;
	// 从当前程序全路径文件名中解析出当前路径(最后带"\")
	for( ; nIdx > 0; nIdx-- )
	{
		if( '\\' == path[ nIdx ] )
		{
			break;
		}
	}

	// 组装配置文件全路径文件名
	memcpy( pFilePathName, path, nIdx + 1 );
	strcat( pFilePathName, "config.server" );	// 文件名为 config.server

}
////////////////////////////////////////////////////// 