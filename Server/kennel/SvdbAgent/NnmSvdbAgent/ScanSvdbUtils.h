/********************************************
 * �ļ�: ScanSvdbUtils.h
 * ����: NetScan �� SVDB �ӿڷ���
 * ע��: ֻ֧��һ�� NetScan ����ģʽ
 * ����: Benny 2007.8.21
 * �޸�: (���¼�޸���Ϣ)
********************************************/

#ifndef ScanSvdbUtilsH
#define ScanSvdbUtilsH

#include <string>
#include <vector>
#include <list>

#include "sliceDataWrap.h"

using namespace std;
////////////////////////////////////////////////////////////////////

namespace NS_ScanSvdbUtils
{

////////////////////////////////////////////////////////////////////
// ���ݽṹ����
typedef vector< string > VectorOfString;		// �ַ�������
typedef list< string > ListOfString;			// �ַ�������

// ɨ�蹲ͬ��ṹ
typedef struct _Community
{
	string sStartIp;	// ��ʼIP��ַ
    string sEndIp;		// ����IP��ַ
    string sGet;		// Get ��ͬ��
    string sSet;		// Set ��ͬ��
}Community;
typedef vector< Community > VectorOfCommunity;	// ��ͬ������

// ɨ��IP��ַ��Χ�ṹ
typedef struct _IPScope
{
	string sIpBegin;	// ��ʼIP��ַ
	string sIpEnd;		// ��ֹIP��ַ
}IPScope;
typedef vector< IPScope > VectorOfIpScope;		// IP��ַ��Χ����

// ɨ������ṹ
typedef struct _ScanParameter
{	
	string				sDefaultCommunityOfGet;	// Get ��ͬ����
    string				sDefaultCommunityOfSet;	// Set ��ͬ����
    VectorOfCommunity	vcCommunityList;		// ��ͬ���б�
    VectorOfString		vsSeedIpList;			// ����IP�б�
	VectorOfIpScope		viIpScopeList;			// ɨ�跶Χ�б�
	VectorOfIpScope		viFilterStartEndList;	// ����IP�б�
	VectorOfIpScope		viSubnetList;			// �����б�

	int	nDepth;				// ɨ��Ũ��
	int	nTryTimes;			// ���Դ���
	int nMaxTreadCount;		// ��Ծ����
	int	nTimeout;			// ��ʱʱ��
	bool bSubScan;			// �Ƿ�Ϊ����ɨ��
}ScanParameter;

////////////////////////////////////////////////////////////////////
// �ӿڷ�����
// (����Ϊ��̬��, ��ֻ������һ���� Scan ���̵����, ��Ҫʹ�ö��������챾��)

class ScanUtils
{
public:
	static void SetRootDir( const string &sDir );				// ���ø�Ŀ¼, ע��: Ŀ¼�����Ҫ�� "/"
	
	// ����Ϊ�ӿڷ���
	static bool StartScan( const ScanParameter& sp,				// ��ʼɨ��
						const string& sQuename, 
						bool bRestart, bool bAsync = false );	
	static bool CancelScan( const string& sQuename );			// ȡ��ɨ��(����ɨ��)
	static bool GetScanParameter( const string& sQueName,		// �� SVDB ȡ��ɨ�����
						ScanParameter& sp );	
	static bool SubmitScanResult( const string& sQueName,		// �ύɨ�����
						const ::SliceNNM::TopologyChart& tcSlice );	
	static bool GetScanResult( const string& sQueName,			// ȡ��ɨ����
						::SliceNNM::TopologyChart& tcSlice );	
	static bool StartRealTimeScan( const string& sSession,		// ��ʼʵʱɨ��
						int nFreq, const ::SliceNNM::StringLst &idList );	
	static bool CancelRealTimeScan( const string& sSession );	// ȡ��ʵʱɨ��(����ɨ��)

protected:
	static bool IsScanRunning();		// ��� Scan �����Ƿ��Ѿ�������
	static bool KillScanProcess();		// ��ֹ(ɱ��) Scan ����

	static bool WriteScanParamToSvdb( const ScanParameter& sp,	// ��ɨ��������浽 SVDB ��
										const string& sQuename );

	// ����ɨ������ṹ
	static void CopyScanParameter( ScanParameter &spDest, const ScanParameter &spSrc );
	
public:
	static PROCESS_INFORMATION m_piScanProcessInfo;		// Scan ������Ϣ
	static string m_sRootDir;							// Scan ���̸�Ŀ¼
	static ScanParameter m_spScanParam;					// ɨ������ṹ

};	// class ScanUtils

};	// namespace NS_ScanSvdbUtils

////////////////////////////////////////////////////////////////////
#endif