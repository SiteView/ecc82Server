/************************************
 * 文件: 日志类
 * 描述: 用于程序中输出日志信息到文件
 * 创建: Benny 2007.8.20
 * 修改:
************************************/
//---------------------------------------------------------------------------

#ifndef LogUtilsH
#define LogUtilsH

#define LOG_FILE_NAME_LEN   256     // 文件路径名最大长度
//---------------------------------------------------------------------------
namespace SV_LogUtils
{
class SvLog
{
public:
    // 功能: 写日志
    // 参数: pLogMsg    [in]    日志信息
    //       nCode      [in]    代码
    //       pFile      [in]    产生日志的文件, 可以使用 __FILE__ 宏
    //       nLine      [in]    产生日志的行号, 可以使用 __LINE__ 宏
    // 返回: 无
    static void Write( const char * const pLogMsg, int nCode,
                       const char * const pFile, int nLine );

    // 功能: 倾卸数据
    // 参数: pBuf       [in]    缓冲区指针
    //       nBufLen    [in]    缓冲区大小(字节)
    // 返回: 无
    static void DumpBuf( const unsigned char * const pBuf, unsigned int nBufLen );

    // 功能: 设置日志文件路径名
    static void SetLogFileName( const char * const pFileName );

    // 功能: 设置数据倾卸文件路径名
    static void SetDumpFileName(  const char * const pFileName );

protected:

    static char m_szLogFile[ LOG_FILE_NAME_LEN ];       // 日志文件路径名
    static char m_szDumpFile[ LOG_FILE_NAME_LEN ];      // 数据倾卸文件路径名
};


};
//---------------------------------------------------------------------------
#endif
