/************************************
 * �ļ�: ��־��ʵ��
 * ����: ���ڳ����������־��Ϣ���ļ�
 * ����: Benny 2007.8.20
 * �޸�:
************************************/
//---------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <dos.h>        // only Win32 platform
#include <memory.h>
#include <string.h>

#pragma hdrstop

#include "LogUtils.h"

using namespace SV_LogUtils;

// �ྲ̬��Ա��ʼ��
char SvLog::m_szLogFile[ LOG_FILE_NAME_LEN ]     = "C:\\SvLog.log";     // ��־�ļ�·����(ȱʡ)
char SvLog::m_szDumpFile[ LOG_FILE_NAME_LEN ]    = "C:\\SvDump.dat";    // ������ж�ļ�·����(ȱʡ)

//---------------------------------------------------------------------------

#pragma package(smart_init)


//---------------------------------------------------------------------------
// ����: д��־
// ����: pLogMsg    [in]    ��־��Ϣ
//       nCode      [in]    ����
//       pFile      [in]    ������־���ļ�, ����ʹ�� __FILE__ ��
//       nLine      [in]    ������־���к�, ����ʹ�� __LINE__ ��
// ����: ��
void SvLog::Write( const char * const pLogMsg, int nCode,
                   const char * const pFile, int nLine )
{
	///////////////////////////////////////////
	// ���ļ�
    FILE *fp = NULL;
    if( ( fp = fopen( m_szLogFile, "a" ) ) == NULL )
    {
        return;
    }

	///////////////////////////////////////////
    // ȡϵͳʱ��
    char bufTime[ 12 ];
    // struct tm tmNow;
	time_t tmNow;
	time( &tmNow );		// ȡ��ϵͳʱ��
	struct tm *ltm = localtime( &tmNow );
	
    memset( bufTime, 0x00, sizeof( bufTime ) );
	sprintf( bufTime, "%02d:%02d:%02d", ltm->tm_hour, ltm->tm_min, ltm->tm_sec );

	///////////////////////////////////////////
    // ֻȡ�ļ�������,����·��
    char szLogFile[ LOG_FILE_NAME_LEN ];
	memset( szLogFile, 0x00, sizeof( szLogFile ) ); 
	int nIdx = -1;
	int nLen = strlen( pFile );
	for( int i = 0; i < nLen; i++ )
	{
		if( pFile[ i ] == '\\' )
		{   // �ҵ����һ����б��
			nIdx = i;
		}
	}
	strncpy( szLogFile, pFile + nIdx + 1, sizeof( szLogFile ) - 1 );

	///////////////////////////////////////////
    // ����ʽд����־��Ϣ����־�ļ�
    fprintf( fp, "%s  %s | %d | %s | %d\n", bufTime, pLogMsg, nCode, szLogFile, nLine );
    
	///////////////////////////////////////////
    fclose(  fp );  // �ر��ļ�

}
//---------------------------------------------------------------------------

// ����: ��ж����
// ����: pBuf       [in]    ������ָ��
//       nBufLen    [in]    ��������С(�ֽ�)
// ����: ��
void SvLog::DumpBuf( const unsigned char * const pBuf, unsigned int nBufLen )
{
    FILE *fp = NULL;
    if( ( fp = fopen( m_szDumpFile, "a" ) ) == NULL )
    {
        return;
    }

    fwrite( pBuf, nBufLen, 1, fp );     // д����
    
    fclose(  fp );  // �ر��ļ�
}
//---------------------------------------------------------------------------
// ����: ������־�ļ�·����
void SvLog::SetLogFileName( const char * const pFileName )
{
    memset( m_szLogFile, 0x00, sizeof( m_szLogFile ) );
    strncpy( m_szLogFile, pFileName, sizeof( m_szLogFile ) - 1 );    
}

//---------------------------------------------------------------------------
// ����: ����������ж�ļ�·����
void SvLog::SetDumpFileName(  const char * const pFileName )
{
    memset( m_szDumpFile, 0x00, sizeof( m_szDumpFile ) );
    strncpy( m_szDumpFile, pFileName, sizeof( m_szDumpFile ) - 1 );
}
//---------------------------------------------------------------------------
