/********************************************
 * 文件: ScanSvdbUtils.cpp
 * 描述: NetScan 的 SVDB 接口方法实现
 * 注意: 注意方法实现时应保证线程安全
 * 创建: Benny 2007.8.21
 * 修改: (请记录修改信息)
********************************************/

#include <afx.h>
#include <Windows.h>
#include <Psapi.h>

#pragma once

#include "ScanSvdbUtils.h"
#include "svnnmapi.h"
#include "TopologyChart.h"
#include "DatHandleUtils.h"
#include "SliceUtil.h"
#include "LogUtils.h"

#include "svdbapi.h"

using namespace NS_DatHandleUtils;
using namespace NS_ScanSvdbUtils;
using namespace SV_LogUtils;

#define SCAN_EXCUTABLE_FILE		"netScanC.exe"		// 扫描程序文件名( Python版 )
// #define SCAN_EXCUTABLE_FILE		"NetScan.exe"		// 扫描程序文件名( 原版 )
#define PROCESS_NAME_LEN		256					// 进程名称长度
#define PROCESS_NUM_MAX			512					// 系统最大进程数

// 类静态成员定义
PROCESS_INFORMATION ScanUtils::m_piScanProcessInfo;
string ScanUtils::m_sRootDir = "";
ScanParameter ScanUtils::m_spScanParam;

////////////////////////////////////////////////////////////////////
DWORD WINAPI ScanMonThread( LPVOID lpParam );		// Sacan 监控线程

////////////////////////////////////////////////////////////////////
// 设置根目录
void ScanUtils::SetRootDir( const string &sDir )
{
	cout << "Call: ScanUtils::SetRootDir - RootDir: " << sDir <<endl;

	m_sRootDir = sDir;
}


////////////////////////////////////////////////////////////////////
// 将扫描参数保存到 SVDB 中
bool ScanUtils::WriteScanParamToSvdb( const ScanParameter& sp,	
										const string& sQuename )
{
	// 暂不保存到 SVDB 中, 只保存在内存中
	// Scan 进程通过 GetScanParameter 方法可以获得参数
	// Benny 2007.8.24

	cout << "Call: ScanUtils::WriteScanParamToSvdb" <<endl;

	return true;
}

////////////////////////////////////////////////////////////////////
// 启动 Scan 进程
// bool ScanUtils::LaunchScanProcess( const string &sQueName, bool bAsync )
DWORD WINAPI ScanMonThread( LPVOID lpParam )
{
	// 接收传入参数
	char szQueName[ 256 ];	
	memset( szQueName, 0x00, sizeof( szQueName ) );
	strncpy( szQueName, (char *)lpParam, sizeof( szQueName ) - 1 );

	cout << "Call: Thread - ScanMonThread - Queue name: " << szQueName << endl;
	SvLog::Write( string( "Call: Thread - ScanMonThread - Queue name: " + string( szQueName ) ).c_str(), 
				0, __FILE__, __LINE__ );

	char szScanExcutableFilePathName[ 1024 ];
	STARTUPINFO si;
	string sCurrentDir = ScanUtils::m_sRootDir + "\\fcgi-bin";		// Scan 程序的当前路径

	::ZeroMemory( &ScanUtils::m_piScanProcessInfo, sizeof( PROCESS_INFORMATION ) );

	::ZeroMemory( &si, sizeof( si ) );	
    si.cb = sizeof( si );

	// 取得 Scan 程序执行文件所在目录
	memset( szScanExcutableFilePathName, 0x00, sizeof( szScanExcutableFilePathName ) );

	sprintf( szScanExcutableFilePathName, "%s\\%s %s", sCurrentDir.c_str(), SCAN_EXCUTABLE_FILE, szQueName );
	
	cout << "ScanUtils::LaunchScanProcess - CreateProcess: " << szScanExcutableFilePathName << endl;
	SvLog::Write( string( string( "ScanUtils::LaunchScanProcess - CreateProcess: " ) + string( szScanExcutableFilePathName ) ).c_str(), 
					0, sCurrentDir.c_str(), __LINE__ );

	BOOL bSucc = ::CreateProcess( NULL,
								  szScanExcutableFilePathName,
								  NULL,
								  NULL,
								  FALSE,
								  CREATE_NEW_CONSOLE,
								  NULL,
								  sCurrentDir.c_str(),
								  &si,
								  &ScanUtils::m_piScanProcessInfo );
	if( FALSE == bSucc )
	{	// 启动进程失败
		SvLog::Write( string( "ScanUtils::LaunchScanProcess - *failed*, code : " ).c_str(), 
						::GetLastError(), __FILE__, __LINE__ );
		cout << "ScanUtils::LaunchScanProcess - *failed*, code : " << ::GetLastError() <<endl;
		return false;
	}
	// 同步方式(阻塞, 等待进程结束)
	cout << "ScanUtils::ScanMonThread - WaitForSingleObject ... ..." <<endl;
	SvLog::Write( string( "ScanUtils::ScanMonThread - Wait for scan process exit ... ..." ).c_str(), 
				ScanUtils::m_piScanProcessInfo.dwProcessId, __FILE__, __LINE__ );
	
	// ::Sleep( 2*1000 );	// 等待 2 秒
	::WaitForSingleObject( ScanUtils::m_piScanProcessInfo.hProcess, INFINITE );

	// 关闭不用的句柄( 需要句柄时可以通过 ProcessID 获得 )
	::CloseHandle( ScanUtils::m_piScanProcessInfo.hProcess );
	::CloseHandle( ScanUtils::m_piScanProcessInfo.hThread );

	SvLog::Write( string( "ScanUtils::ScanMonThread - all done!" ).c_str(), 
					ScanUtils::m_piScanProcessInfo.dwProcessId, __FILE__, __LINE__ );

	cout << "ScanUtils::ScanMonThread - all done!" <<endl;

	return true;
}

////////////////////////////////////////////////////////////////////
// 检查 Scan 进程是否已经在运行
bool ScanUtils::IsScanRunning()				
{

	cout << "Call: ScanUtils::IsScanRunning - OpenProcess ... ..." <<endl;

	HANDLE hProc = ::OpenProcess( PROCESS_QUERY_INFORMATION, 
								FALSE, 
								m_piScanProcessInfo.dwProcessId );
	bool bRunning = ( NULL != hProc );
	if( bRunning )
	{
		::CloseHandle( hProc );
	}
	else		// 检查系统进程中是否存在 Scan 进程
	{
		cout << "ScanUtils::IsScanRunning - scan system process for ( "<< SCAN_EXCUTABLE_FILE << ") ... ..." <<endl;

		DWORD dwaPID[ PROCESS_NUM_MAX ];
		DWORD dwProcNum = 0;
		memset( dwaPID, 0x00, sizeof( dwaPID ) );

		// 枚举所有系统进程
		BOOL bSucc = ::EnumProcesses( dwaPID,               // array of process identifiers
									sizeof( dwaPID ),       // size of array
									&dwProcNum              // number of bytes returned
									);
		if( bSucc )
		{   // 打开每个进程判断是否是查收的目标进程
			for( DWORD i = 0; i < dwProcNum/sizeof( DWORD ); i++ )
			{
				hProc = ::OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
										FALSE,
										dwaPID[ i ] );
				if ( hProc )
    			{
	    			HMODULE hMod;
					DWORD cbNeeded;
					char szProcessName[ PROCESS_NAME_LEN ];
					memset( szProcessName, 0x00, sizeof(szProcessName) );
					if ( ::EnumProcessModules( hProc, &hMod, sizeof( hMod ), &cbNeeded) )
					{   // 获得进行的程序文件名
						::GetModuleBaseName( hProc, hMod, szProcessName, sizeof(szProcessName) );
	                   
						if ( string( szProcessName ) == string( SCAN_EXCUTABLE_FILE ) )
		    			{	// 有进程实例在运行
							m_piScanProcessInfo.dwProcessId = dwaPID[ i ];	// 保存进程ID

							bRunning = true;
						}                     
					}
					else
					{
						SvLog::Write( string( "ScanUtils::IsScanRunning - EnumProcessModules *failed*, code : " ).c_str(), 
									::GetLastError(), __FILE__, __LINE__ );
					}
					::CloseHandle( hProc );
					if( bRunning )
					{	// 找到一个就结束
						break;
					}
				}
				else
				{
					SvLog::Write( string( "ScanUtils::IsScanRunning - OpenProcess *failed*, code : " ).c_str(), 
								::GetLastError(), __FILE__, __LINE__ );
				}
			}
			cout << "ScanUtils::IsScanRunning - EnumProcesses : " << ( bRunning ? "found running!" : "not found!" ) <<endl;
		}
		else
		{
			SvLog::Write( string( "ScanUtils::IsScanRunning - EnumProcesses *failed*, code : " ).c_str(), 
						::GetLastError(), __FILE__, __LINE__ );
			cout << "ScanUtils::IsScanRunning - EnumProcesses : *failed*" <<endl;
		}
	}

	cout << "ScanUtils::IsScanRunning - " << ( bRunning ? "running!" : "not run!" ) << " - return!" <<endl;

	return bRunning;
}

////////////////////////////////////////////////////////////////////
// 终止(杀死) Scan 进程
bool ScanUtils::KillScanProcess()
{
	cout << "Call: ScanUtils::KillScanProcess - check scan process whether running... ..." <<endl;

	if( !IsScanRunning() )
	{	// 进程未在运行状态
		return true;
	}

	cout << "ScanUtils::KillScanProcess - OpenProcess for terminate... ..." <<endl;

	// 以可以终止的权限打开进程
	HANDLE hProc = ::OpenProcess( PROCESS_TERMINATE, 
								FALSE, 
								m_piScanProcessInfo.dwProcessId );
	if( hProc == NULL )
	{	// 无权限打开进程, 或进程已经消亡
		SvLog::Write( string( "ScanUtils::KillScanProcess - OpenProcess *failed*, code : " ).c_str(), 
						::GetLastError(), __FILE__, __LINE__ );
		cout << "ScanUtils::KillScanProcess - OpenProcess *failed*" <<endl;

		return false;
	}

	cout << "ScanUtils::KillScanProcess - TerminateProcess ... ..." <<endl;

	// 强行终止进程
	bool bOk = ( TRUE == ::TerminateProcess( hProc, 0 ) );
	if( !bOk )
	{
		SvLog::Write( string( "ScanUtils::KillScanProcess - TerminateProcess *failed*, code : " ).c_str(), 
						::GetLastError(), __FILE__, __LINE__ );
	}
	
	::CloseHandle( hProc );

	cout << "ScanUtils::KillScanProcess - TerminateProcess: " << string( bOk ? "success!" : "*failed*" );
	cout << " - code: " << ::GetLastError() << endl;

	return bOk;
}
////////////////////////////////////////////////////////////////////
// 开始扫描
bool ScanUtils::StartScan( const ScanParameter& sp, const string& sQueName,
						   bool bRestart, bool bAsync /* = false */ )
{
	SvLog::Write( string( "ScanUtils::StartScan - check scan process whether running ... ..." ).c_str(), 
					m_piScanProcessInfo.dwProcessId, __FILE__, __LINE__ );
	cout << "Call: ScanUtils::StartScan - check scan process whether running ... ..." <<endl;

	///////////////////////////////////////////
	// 1.检查 Scan 进程的运行状态
	bool bOk = IsScanRunning();
	if( bOk )
	{	// Scan 进程在运行
		if( bRestart )
		{	// 重新启动 Scan 进程
			bOk = KillScanProcess();
			if( !bOk )
			{	// 结束当前的 Scan 进程失败
				return false;
			}
			// 此处不返回将执行创建新进程代码
		}
		else
		{
			return false;
		}		
	}

	SvLog::Write( string( "ScanUtils::StartScan - Save scan parameter ... ..." ).c_str(), 
					m_piScanProcessInfo.dwProcessId, __FILE__, __LINE__ );	
	cout << "ScanUtils::StartScan - Save scan parameter ... ..." <<endl;

	///////////////////////////////////////////
	// 2.保存扫描参数
	CopyScanParameter( m_spScanParam, sp );		// 传入的扫描参数结构保存到内存中
	WriteScanParamToSvdb( sp,	sQueName );	// 传入的扫描参数结构保存到SVDB中

	cout << "ScanUtils::StartScan - LaunchScanProcess ... ..." <<endl;

	///////////////////////////////////////////
	// 3.启动扫描线程(在线程中启动扫描进程并等待进程结束)
	DWORD dwThreadId = 0;
	HANDLE hScanThread = ::CreateThread( NULL, 
										0,
										&ScanMonThread,	// Thread function
										(LPVOID)sQueName.c_str(),	// Parameter to pass to thread
										0,		// Create and run immediately
										&dwThreadId );	// Thread ID

	bOk = ( hScanThread != NULL );
	if( !bOk )
	{	// 创建线程失败
		DWORD dwErrorCode = ::GetLastError();
		SvLog::Write( string( "ScanUtils::StartScan - CreateThread *error*" ).c_str(), dwErrorCode, __FILE__, __LINE__ );	
		cout << "ScanUtils::StartScan - CreateThread *error* - code : " << dwErrorCode <<endl;
	}
	else
	{
		// ::Sleep( 3*1000 );	// 等待 3 秒, 等线程将 SCan 进程启动起来
	}

	cout << "ScanUtils::StartScan - all done! - " << ( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

////////////////////////////////////////////////////////////////////
// 取消扫描(结束扫描)
bool ScanUtils::CancelScan( const string& sQuename )
{
	cout << "Call: ScanUtils::CancelScan - KillScanProcess ... ..." <<endl;

	return KillScanProcess();				// 终止(杀死) Scan 进程	
}

////////////////////////////////////////////////////////////////////
// 从 SVDB 取得扫描参数
bool ScanUtils::GetScanParameter( const string& sQueName, ScanParameter& sp )
{
	cout << "Call: ScanUtils::GetScanParameter - CopyScanParameter ... ..." <<endl;

	CopyScanParameter( sp, m_spScanParam );
	return true;
}

////////////////////////////////////////////////////////////////////
// 提交扫描参数
bool ScanUtils::SubmitScanResult( const string& sQueName, const ::SliceNNM::TopologyChart& tcSlice )
{
	cout << "Call: ScanUtils::SubmitScanResult - CreateTopologyChart ... ..." <<endl;

	OBJECT tcObj = ::CreateTopologyChart( Utf8ToString( sQueName ) );
	if( tcObj == NULL )
	{
		SvLog::Write( string( "ScanUtils::SubmitScanResult - CreateTopologyChart *failed*" ).c_str(), 0, __FILE__, __LINE__ );
		cout << "ScanUtils::SubmitScanResult - CreateTopologyChart : *failed*" <<endl;

		return false;
	}

	TopologyChart *pTc = reinterpret_cast<TopologyChart *>( tcObj );
	if( NULL == pTc )
	{
		SvLog::Write( string( "ScanUtils::SubmitScanResult - reinterpret_cast *failed*" ).c_str(), 0, __FILE__, __LINE__ );
		cout << "ScanUtils::SubmitScanResult - reinterpret_cast : *failed*" <<endl;

		return false;
	}

	cout << "ScanUtils::SubmitScanResult - Transform data format ... ..." <<endl;

	// 数据格式转换
	DatHandleUtils::TopologyChartSlice2Svdb( tcSlice, (*pTc) );

	cout << "ScanUtils::SubmitScanResult - Submit to svdb ... ..." <<endl;


	// 提交到SVDB
	bool bOk = ::SubmitTopologyChart( tcObj );
	if( !bOk )
	{
		SvLog::Write( string( "ScanUtils::SubmitScanResult - SubmitTopologyChart *failed*" ).c_str(), 0, __FILE__, __LINE__ );
	}

	cout << "ScanUtils::SubmitScanResult - Submit to svdb : " << ( bOk ? "success!" : "*failed*" ) << endl;

	::CloseTopologyChart( tcObj );	//删除对象，以免内存泄漏

	cout << "ScanUtils::SubmitScanResult - all done!" <<endl;

	return bOk;
}

////////////////////////////////////////////////////////////////////
// 取得扫描结果
bool ScanUtils::GetScanResult( const string& sQueName, ::SliceNNM::TopologyChart& tcSlice )
{
	cout << "Call: ScanUtils::GetScanResult - GetTopologyChart ... ..." <<endl;

	OBJECT tcObj = ::GetTopologyChart( Utf8ToString( sQueName ) );
	if( tcObj == NULL )
	{
		SvLog::Write( string( "ScanUtils::GetScanResult - GetTopologyChart *failed*" ).c_str(), 0, __FILE__, __LINE__ );
		cout << "ScanUtils::GetScanResult - GetTopologyChart : *failed*" <<endl;

		return false;
	}
	TopologyChart *pTc = reinterpret_cast<TopologyChart *>( tcObj );
	if( NULL == pTc )
	{
		SvLog::Write( string( "ScanUtils::GetScanResult - reinterpret_cast *failed*" ).c_str(), 0, __FILE__, __LINE__ );
		cout << "ScanUtils::GetScanResult - reinterpret_cast : *failed*" <<endl;

		return false;
	}

	cout << "ScanUtils::GetScanResult - Transform data format ... ..." <<endl;

	// 数据格式转换
	DatHandleUtils::TopologyChartSvdb2Slice( (*pTc), tcSlice );

	::CloseTopologyChart( tcObj );	//删除对象，以免内存泄漏

	cout << "ScanUtils::GetScanResult - all done!" << endl;

	return true;
}

// 复制扫描参数结构
void ScanUtils::CopyScanParameter( ScanParameter &spDest, const ScanParameter &spSrc )
{
	try
	{
		// 初始化输出参数
		spDest.vcCommunityList.clear();
		spDest.vsSeedIpList.clear();
		spDest.viIpScopeList.clear();
		spDest.viFilterStartEndList.clear();
		spDest.viSubnetList.clear();

		// 复制数据(深度拷贝)
		spDest.sDefaultCommunityOfGet	= spSrc.sDefaultCommunityOfGet;
		spDest.sDefaultCommunityOfSet	= spSrc.sDefaultCommunityOfSet;
		spDest.nDepth					= spSrc.nDepth;
		spDest.nTryTimes				= spSrc.nTryTimes;
		spDest.nMaxTreadCount			= spSrc.nMaxTreadCount;
		spDest.nTimeout					= spSrc.nTimeout;
		spDest.bSubScan					= spSrc.bSubScan;	

		ScanParameter &spSource = const_cast< ScanParameter &>( spSrc );

		// 复制 community
		for( VectorOfCommunity::iterator it = spSource.vcCommunityList.begin();
			it != spSource.vcCommunityList.end(); it++ )
		{
			Community community;

			community.sStartIp	= it->sStartIp;
			community.sEndIp	= it->sEndIp;
			community.sGet		= it->sGet;
			community.sSet		= it->sSet;
			
			spDest.vcCommunityList.push_back( community );		
		}

		// 复制 SeedIpList
		for( VectorOfString::iterator it = spSource.vsSeedIpList.begin();
			it != spSource.vsSeedIpList.end(); it++ )
		{
			spDest.vsSeedIpList.push_back( ( *it ) );
		}

		// 复制 IpScopeList
		for( VectorOfIpScope::iterator it = spSource.viIpScopeList.begin();
			it != spSource.viIpScopeList.end(); it++ )
		{
			IPScope ipScope;

			ipScope.sIpBegin	= it->sIpBegin;
			ipScope.sIpEnd		= it->sIpBegin;

			spDest.viIpScopeList.push_back( ipScope );
		}

		// 复制 FilterStartEndList
		for( VectorOfIpScope::iterator it = spSource.viFilterStartEndList.begin();
			it != spSource.viFilterStartEndList.end(); it++ )
		{
			IPScope ipScope;

			ipScope.sIpBegin	= it->sIpBegin;
			ipScope.sIpEnd		= it->sIpBegin;

			spDest.viFilterStartEndList.push_back( ipScope );
		}

		// 复制 SubnetList
		for( VectorOfIpScope::iterator it = spSource.viSubnetList.begin();
			it != spSource.viSubnetList.end(); it++ )
		{
			IPScope ipScope;

			ipScope.sIpBegin	= it->sIpBegin;
			ipScope.sIpEnd		= it->sIpBegin;

			spDest.viSubnetList.push_back( ipScope );
		}
	}
	catch( ... )
	{
		SvLog::Write( string( "ScanUtils::GetScanResult - reinterpret_cast *exception*" ).c_str(), 0, __FILE__, __LINE__ );
		cout << "ScanUtils::CopyScanParameter - *exception*" <<endl;
	}
}


////////////////////////////////////////////////////////////////////
// 开始实时扫描
// 此过程与 StartScan 目前没有区别(罗万) Benny 2007.8.21
bool ScanUtils::StartRealTimeScan( const string& sSession, int nFreq, 
								  const ::SliceNNM::StringLst &idList )
{
	// 该函数根据目前的需求不需要实现
	// 需求来自: 吴子平
	// Benny 2007.8.24

	cout << "Call: ScanUtils::StartRealTimeScan - Session: " << sSession <<endl;

	return false;
}

////////////////////////////////////////////////////////////////////
// 取消实时扫描(结束扫描)
// 此过程与 CancelScan 目前没有区别(罗万) Benny 2007.8.21
bool ScanUtils::CancelRealTimeScan( const string& sSession )
{
	// 该函数根据目前的需求不需要实现
	// 需求来自: 吴子平
	// Benny 2007.8.24

	cout << "Call: ScanUtils::CancelRealTimeScan - Session: " << sSession <<endl;

	return false;
}

////////////////////////////////////////////////////////////////////