/********************************************
 * �ļ�: ScanSvdbUtils.cpp
 * ����: NetScan �� SVDB �ӿڷ���ʵ��
 * ע��: ע�ⷽ��ʵ��ʱӦ��֤�̰߳�ȫ
 * ����: Benny 2007.8.21
 * �޸�: (���¼�޸���Ϣ)
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

#define SCAN_EXCUTABLE_FILE		"netScanC.exe"		// ɨ������ļ���( Python�� )
// #define SCAN_EXCUTABLE_FILE		"NetScan.exe"		// ɨ������ļ���( ԭ�� )
#define PROCESS_NAME_LEN		256					// �������Ƴ���
#define PROCESS_NUM_MAX			512					// ϵͳ��������

// �ྲ̬��Ա����
PROCESS_INFORMATION ScanUtils::m_piScanProcessInfo;
string ScanUtils::m_sRootDir = "";
ScanParameter ScanUtils::m_spScanParam;

////////////////////////////////////////////////////////////////////
DWORD WINAPI ScanMonThread( LPVOID lpParam );		// Sacan ����߳�

////////////////////////////////////////////////////////////////////
// ���ø�Ŀ¼
void ScanUtils::SetRootDir( const string &sDir )
{
	cout << "Call: ScanUtils::SetRootDir - RootDir: " << sDir <<endl;

	m_sRootDir = sDir;
}


////////////////////////////////////////////////////////////////////
// ��ɨ��������浽 SVDB ��
bool ScanUtils::WriteScanParamToSvdb( const ScanParameter& sp,	
										const string& sQuename )
{
	// �ݲ����浽 SVDB ��, ֻ�������ڴ���
	// Scan ����ͨ�� GetScanParameter �������Ի�ò���
	// Benny 2007.8.24

	cout << "Call: ScanUtils::WriteScanParamToSvdb" <<endl;

	return true;
}

////////////////////////////////////////////////////////////////////
// ���� Scan ����
// bool ScanUtils::LaunchScanProcess( const string &sQueName, bool bAsync )
DWORD WINAPI ScanMonThread( LPVOID lpParam )
{
	// ���մ������
	char szQueName[ 256 ];	
	memset( szQueName, 0x00, sizeof( szQueName ) );
	strncpy( szQueName, (char *)lpParam, sizeof( szQueName ) - 1 );

	cout << "Call: Thread - ScanMonThread - Queue name: " << szQueName << endl;
	SvLog::Write( string( "Call: Thread - ScanMonThread - Queue name: " + string( szQueName ) ).c_str(), 
				0, __FILE__, __LINE__ );

	char szScanExcutableFilePathName[ 1024 ];
	STARTUPINFO si;
	string sCurrentDir = ScanUtils::m_sRootDir + "\\fcgi-bin";		// Scan ����ĵ�ǰ·��

	::ZeroMemory( &ScanUtils::m_piScanProcessInfo, sizeof( PROCESS_INFORMATION ) );

	::ZeroMemory( &si, sizeof( si ) );	
    si.cb = sizeof( si );

	// ȡ�� Scan ����ִ���ļ�����Ŀ¼
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
	{	// ��������ʧ��
		SvLog::Write( string( "ScanUtils::LaunchScanProcess - *failed*, code : " ).c_str(), 
						::GetLastError(), __FILE__, __LINE__ );
		cout << "ScanUtils::LaunchScanProcess - *failed*, code : " << ::GetLastError() <<endl;
		return false;
	}
	// ͬ����ʽ(����, �ȴ����̽���)
	cout << "ScanUtils::ScanMonThread - WaitForSingleObject ... ..." <<endl;
	SvLog::Write( string( "ScanUtils::ScanMonThread - Wait for scan process exit ... ..." ).c_str(), 
				ScanUtils::m_piScanProcessInfo.dwProcessId, __FILE__, __LINE__ );
	
	// ::Sleep( 2*1000 );	// �ȴ� 2 ��
	::WaitForSingleObject( ScanUtils::m_piScanProcessInfo.hProcess, INFINITE );

	// �رղ��õľ��( ��Ҫ���ʱ����ͨ�� ProcessID ��� )
	::CloseHandle( ScanUtils::m_piScanProcessInfo.hProcess );
	::CloseHandle( ScanUtils::m_piScanProcessInfo.hThread );

	SvLog::Write( string( "ScanUtils::ScanMonThread - all done!" ).c_str(), 
					ScanUtils::m_piScanProcessInfo.dwProcessId, __FILE__, __LINE__ );

	cout << "ScanUtils::ScanMonThread - all done!" <<endl;

	return true;
}

////////////////////////////////////////////////////////////////////
// ��� Scan �����Ƿ��Ѿ�������
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
	else		// ���ϵͳ�������Ƿ���� Scan ����
	{
		cout << "ScanUtils::IsScanRunning - scan system process for ( "<< SCAN_EXCUTABLE_FILE << ") ... ..." <<endl;

		DWORD dwaPID[ PROCESS_NUM_MAX ];
		DWORD dwProcNum = 0;
		memset( dwaPID, 0x00, sizeof( dwaPID ) );

		// ö������ϵͳ����
		BOOL bSucc = ::EnumProcesses( dwaPID,               // array of process identifiers
									sizeof( dwaPID ),       // size of array
									&dwProcNum              // number of bytes returned
									);
		if( bSucc )
		{   // ��ÿ�������ж��Ƿ��ǲ��յ�Ŀ�����
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
					{   // ��ý��еĳ����ļ���
						::GetModuleBaseName( hProc, hMod, szProcessName, sizeof(szProcessName) );
	                   
						if ( string( szProcessName ) == string( SCAN_EXCUTABLE_FILE ) )
		    			{	// �н���ʵ��������
							m_piScanProcessInfo.dwProcessId = dwaPID[ i ];	// �������ID

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
					{	// �ҵ�һ���ͽ���
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
// ��ֹ(ɱ��) Scan ����
bool ScanUtils::KillScanProcess()
{
	cout << "Call: ScanUtils::KillScanProcess - check scan process whether running... ..." <<endl;

	if( !IsScanRunning() )
	{	// ����δ������״̬
		return true;
	}

	cout << "ScanUtils::KillScanProcess - OpenProcess for terminate... ..." <<endl;

	// �Կ�����ֹ��Ȩ�޴򿪽���
	HANDLE hProc = ::OpenProcess( PROCESS_TERMINATE, 
								FALSE, 
								m_piScanProcessInfo.dwProcessId );
	if( hProc == NULL )
	{	// ��Ȩ�޴򿪽���, ������Ѿ�����
		SvLog::Write( string( "ScanUtils::KillScanProcess - OpenProcess *failed*, code : " ).c_str(), 
						::GetLastError(), __FILE__, __LINE__ );
		cout << "ScanUtils::KillScanProcess - OpenProcess *failed*" <<endl;

		return false;
	}

	cout << "ScanUtils::KillScanProcess - TerminateProcess ... ..." <<endl;

	// ǿ����ֹ����
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
// ��ʼɨ��
bool ScanUtils::StartScan( const ScanParameter& sp, const string& sQueName,
						   bool bRestart, bool bAsync /* = false */ )
{
	SvLog::Write( string( "ScanUtils::StartScan - check scan process whether running ... ..." ).c_str(), 
					m_piScanProcessInfo.dwProcessId, __FILE__, __LINE__ );
	cout << "Call: ScanUtils::StartScan - check scan process whether running ... ..." <<endl;

	///////////////////////////////////////////
	// 1.��� Scan ���̵�����״̬
	bool bOk = IsScanRunning();
	if( bOk )
	{	// Scan ����������
		if( bRestart )
		{	// �������� Scan ����
			bOk = KillScanProcess();
			if( !bOk )
			{	// ������ǰ�� Scan ����ʧ��
				return false;
			}
			// �˴������ؽ�ִ�д����½��̴���
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
	// 2.����ɨ�����
	CopyScanParameter( m_spScanParam, sp );		// �����ɨ������ṹ���浽�ڴ���
	WriteScanParamToSvdb( sp,	sQueName );	// �����ɨ������ṹ���浽SVDB��

	cout << "ScanUtils::StartScan - LaunchScanProcess ... ..." <<endl;

	///////////////////////////////////////////
	// 3.����ɨ���߳�(���߳�������ɨ����̲��ȴ����̽���)
	DWORD dwThreadId = 0;
	HANDLE hScanThread = ::CreateThread( NULL, 
										0,
										&ScanMonThread,	// Thread function
										(LPVOID)sQueName.c_str(),	// Parameter to pass to thread
										0,		// Create and run immediately
										&dwThreadId );	// Thread ID

	bOk = ( hScanThread != NULL );
	if( !bOk )
	{	// �����߳�ʧ��
		DWORD dwErrorCode = ::GetLastError();
		SvLog::Write( string( "ScanUtils::StartScan - CreateThread *error*" ).c_str(), dwErrorCode, __FILE__, __LINE__ );	
		cout << "ScanUtils::StartScan - CreateThread *error* - code : " << dwErrorCode <<endl;
	}
	else
	{
		// ::Sleep( 3*1000 );	// �ȴ� 3 ��, ���߳̽� SCan ������������
	}

	cout << "ScanUtils::StartScan - all done! - " << ( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

////////////////////////////////////////////////////////////////////
// ȡ��ɨ��(����ɨ��)
bool ScanUtils::CancelScan( const string& sQuename )
{
	cout << "Call: ScanUtils::CancelScan - KillScanProcess ... ..." <<endl;

	return KillScanProcess();				// ��ֹ(ɱ��) Scan ����	
}

////////////////////////////////////////////////////////////////////
// �� SVDB ȡ��ɨ�����
bool ScanUtils::GetScanParameter( const string& sQueName, ScanParameter& sp )
{
	cout << "Call: ScanUtils::GetScanParameter - CopyScanParameter ... ..." <<endl;

	CopyScanParameter( sp, m_spScanParam );
	return true;
}

////////////////////////////////////////////////////////////////////
// �ύɨ�����
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

	// ���ݸ�ʽת��
	DatHandleUtils::TopologyChartSlice2Svdb( tcSlice, (*pTc) );

	cout << "ScanUtils::SubmitScanResult - Submit to svdb ... ..." <<endl;


	// �ύ��SVDB
	bool bOk = ::SubmitTopologyChart( tcObj );
	if( !bOk )
	{
		SvLog::Write( string( "ScanUtils::SubmitScanResult - SubmitTopologyChart *failed*" ).c_str(), 0, __FILE__, __LINE__ );
	}

	cout << "ScanUtils::SubmitScanResult - Submit to svdb : " << ( bOk ? "success!" : "*failed*" ) << endl;

	::CloseTopologyChart( tcObj );	//ɾ�����������ڴ�й©

	cout << "ScanUtils::SubmitScanResult - all done!" <<endl;

	return bOk;
}

////////////////////////////////////////////////////////////////////
// ȡ��ɨ����
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

	// ���ݸ�ʽת��
	DatHandleUtils::TopologyChartSvdb2Slice( (*pTc), tcSlice );

	::CloseTopologyChart( tcObj );	//ɾ�����������ڴ�й©

	cout << "ScanUtils::GetScanResult - all done!" << endl;

	return true;
}

// ����ɨ������ṹ
void ScanUtils::CopyScanParameter( ScanParameter &spDest, const ScanParameter &spSrc )
{
	try
	{
		// ��ʼ���������
		spDest.vcCommunityList.clear();
		spDest.vsSeedIpList.clear();
		spDest.viIpScopeList.clear();
		spDest.viFilterStartEndList.clear();
		spDest.viSubnetList.clear();

		// ��������(��ȿ���)
		spDest.sDefaultCommunityOfGet	= spSrc.sDefaultCommunityOfGet;
		spDest.sDefaultCommunityOfSet	= spSrc.sDefaultCommunityOfSet;
		spDest.nDepth					= spSrc.nDepth;
		spDest.nTryTimes				= spSrc.nTryTimes;
		spDest.nMaxTreadCount			= spSrc.nMaxTreadCount;
		spDest.nTimeout					= spSrc.nTimeout;
		spDest.bSubScan					= spSrc.bSubScan;	

		ScanParameter &spSource = const_cast< ScanParameter &>( spSrc );

		// ���� community
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

		// ���� SeedIpList
		for( VectorOfString::iterator it = spSource.vsSeedIpList.begin();
			it != spSource.vsSeedIpList.end(); it++ )
		{
			spDest.vsSeedIpList.push_back( ( *it ) );
		}

		// ���� IpScopeList
		for( VectorOfIpScope::iterator it = spSource.viIpScopeList.begin();
			it != spSource.viIpScopeList.end(); it++ )
		{
			IPScope ipScope;

			ipScope.sIpBegin	= it->sIpBegin;
			ipScope.sIpEnd		= it->sIpBegin;

			spDest.viIpScopeList.push_back( ipScope );
		}

		// ���� FilterStartEndList
		for( VectorOfIpScope::iterator it = spSource.viFilterStartEndList.begin();
			it != spSource.viFilterStartEndList.end(); it++ )
		{
			IPScope ipScope;

			ipScope.sIpBegin	= it->sIpBegin;
			ipScope.sIpEnd		= it->sIpBegin;

			spDest.viFilterStartEndList.push_back( ipScope );
		}

		// ���� SubnetList
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
// ��ʼʵʱɨ��
// �˹����� StartScan Ŀǰû������(����) Benny 2007.8.21
bool ScanUtils::StartRealTimeScan( const string& sSession, int nFreq, 
								  const ::SliceNNM::StringLst &idList )
{
	// �ú�������Ŀǰ��������Ҫʵ��
	// ��������: ����ƽ
	// Benny 2007.8.24

	cout << "Call: ScanUtils::StartRealTimeScan - Session: " << sSession <<endl;

	return false;
}

////////////////////////////////////////////////////////////////////
// ȡ��ʵʱɨ��(����ɨ��)
// �˹����� CancelScan Ŀǰû������(����) Benny 2007.8.21
bool ScanUtils::CancelRealTimeScan( const string& sSession )
{
	// �ú�������Ŀǰ��������Ҫʵ��
	// ��������: ����ƽ
	// Benny 2007.8.24

	cout << "Call: ScanUtils::CancelRealTimeScan - Session: " << sSession <<endl;

	return false;
}

////////////////////////////////////////////////////////////////////