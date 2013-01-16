/************************************
 * �ļ�: ��־��
 * ����: ���ڳ����������־��Ϣ���ļ�
 * ����: Benny 2007.8.20
 * �޸�:
************************************/
//---------------------------------------------------------------------------

#ifndef LogUtilsH
#define LogUtilsH

#define LOG_FILE_NAME_LEN   256     // �ļ�·������󳤶�
//---------------------------------------------------------------------------
namespace SV_LogUtils
{
class SvLog
{
public:
    // ����: д��־
    // ����: pLogMsg    [in]    ��־��Ϣ
    //       nCode      [in]    ����
    //       pFile      [in]    ������־���ļ�, ����ʹ�� __FILE__ ��
    //       nLine      [in]    ������־���к�, ����ʹ�� __LINE__ ��
    // ����: ��
    static void Write( const char * const pLogMsg, int nCode,
                       const char * const pFile, int nLine );

    // ����: ��ж����
    // ����: pBuf       [in]    ������ָ��
    //       nBufLen    [in]    ��������С(�ֽ�)
    // ����: ��
    static void DumpBuf( const unsigned char * const pBuf, unsigned int nBufLen );

    // ����: ������־�ļ�·����
    static void SetLogFileName( const char * const pFileName );

    // ����: ����������ж�ļ�·����
    static void SetDumpFileName(  const char * const pFileName );

protected:

    static char m_szLogFile[ LOG_FILE_NAME_LEN ];       // ��־�ļ�·����
    static char m_szDumpFile[ LOG_FILE_NAME_LEN ];      // ������ж�ļ�·����
};


};
//---------------------------------------------------------------------------
#endif
