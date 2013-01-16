/************************************
 * 文件: 日志类实现
 * 描述: 用于程序中输出日志信息到文件
 * 创建: Benny 2007.8.20
 * 修改:
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

// 类静态成员初始化
char SvLog::m_szLogFile[ LOG_FILE_NAME_LEN ]     = "C:\\SvLog.log";     // 日志文件路径名(缺省)
char SvLog::m_szDumpFile[ LOG_FILE_NAME_LEN ]    = "C:\\SvDump.dat";    // 数据倾卸文件路径名(缺省)

//---------------------------------------------------------------------------

#pragma package(smart_init)


//---------------------------------------------------------------------------
// 功能: 写日志
// 参数: pLogMsg    [in]    日志信息
//       nCode      [in]    代码
//       pFile      [in]    产生日志的文件, 可以使用 __FILE__ 宏
//       nLine      [in]    产生日志的行号, 可以使用 __LINE__ 宏
// 返回: 无
void SvLog::Write( const char * const pLogMsg, int nCode,
                   const char * const pFile, int nLine )
{
	///////////////////////////////////////////
	// 打开文件
    FILE *fp = NULL;
    if( ( fp = fopen( m_szLogFile, "a" ) ) == NULL )
    {
        return;
    }

	///////////////////////////////////////////
    // 取系统时间
    char bufTime[ 12 ];
    // struct tm tmNow;
	time_t tmNow;
	time( &tmNow );		// 取得系统时间
	struct tm *ltm = localtime( &tmNow );
	
    memset( bufTime, 0x00, sizeof( bufTime ) );
	sprintf( bufTime, "%02d:%02d:%02d", ltm->tm_hour, ltm->tm_min, ltm->tm_sec );

	///////////////////////////////////////////
    // 只取文件名部分,丢掉路径
    char szLogFile[ LOG_FILE_NAME_LEN ];
	memset( szLogFile, 0x00, sizeof( szLogFile ) ); 
	int nIdx = -1;
	int nLen = strlen( pFile );
	for( int i = 0; i < nLen; i++ )
	{
		if( pFile[ i ] == '\\' )
		{   // 找到最后一个反斜杠
			nIdx = i;
		}
	}
	strncpy( szLogFile, pFile + nIdx + 1, sizeof( szLogFile ) - 1 );

	///////////////////////////////////////////
    // 按格式写入日志信息到日志文件
    fprintf( fp, "%s  %s | %d | %s | %d\n", bufTime, pLogMsg, nCode, szLogFile, nLine );
    
	///////////////////////////////////////////
    fclose(  fp );  // 关闭文件

}
//---------------------------------------------------------------------------

// 功能: 倾卸数据
// 参数: pBuf       [in]    缓冲区指针
//       nBufLen    [in]    缓冲区大小(字节)
// 返回: 无
void SvLog::DumpBuf( const unsigned char * const pBuf, unsigned int nBufLen )
{
    FILE *fp = NULL;
    if( ( fp = fopen( m_szDumpFile, "a" ) ) == NULL )
    {
        return;
    }

    fwrite( pBuf, nBufLen, 1, fp );     // 写数据
    
    fclose(  fp );  // 关闭文件
}
//---------------------------------------------------------------------------
// 功能: 设置日志文件路径名
void SvLog::SetLogFileName( const char * const pFileName )
{
    memset( m_szLogFile, 0x00, sizeof( m_szLogFile ) );
    strncpy( m_szLogFile, pFileName, sizeof( m_szLogFile ) - 1 );    
}

//---------------------------------------------------------------------------
// 功能: 设置数据倾卸文件路径名
void SvLog::SetDumpFileName(  const char * const pFileName )
{
    memset( m_szDumpFile, 0x00, sizeof( m_szDumpFile ) );
    strncpy( m_szDumpFile, pFileName, sizeof( m_szDumpFile ) - 1 );
}
//---------------------------------------------------------------------------
