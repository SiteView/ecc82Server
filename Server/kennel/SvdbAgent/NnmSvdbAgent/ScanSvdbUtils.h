/********************************************
 * 文件: ScanSvdbUtils.h
 * 描述: NetScan 的 SVDB 接口方法
 * 注意: 只支持一个 NetScan 进程模式
 * 创建: Benny 2007.8.21
 * 修改: (请记录修改信息)
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
// 数据结构定义
typedef vector< string > VectorOfString;		// 字符串向量
typedef list< string > ListOfString;			// 字符串链表

// 扫描共同体结构
typedef struct _Community
{
	string sStartIp;	// 开始IP地址
    string sEndIp;		// 结束IP地址
    string sGet;		// Get 共同体
    string sSet;		// Set 共同体
}Community;
typedef vector< Community > VectorOfCommunity;	// 共同体向量

// 扫描IP地址范围结构
typedef struct _IPScope
{
	string sIpBegin;	// 起始IP地址
	string sIpEnd;		// 截止IP地址
}IPScope;
typedef vector< IPScope > VectorOfIpScope;		// IP地址范围向量

// 扫描参数结构
typedef struct _ScanParameter
{	
	string				sDefaultCommunityOfGet;	// Get 共同体名
    string				sDefaultCommunityOfSet;	// Set 共同体名
    VectorOfCommunity	vcCommunityList;		// 共同体列表
    VectorOfString		vsSeedIpList;			// 种子IP列表
	VectorOfIpScope		viIpScopeList;			// 扫描范围列表
	VectorOfIpScope		viFilterStartEndList;	// 过滤IP列表
	VectorOfIpScope		viSubnetList;			// 子网列表

	int	nDepth;				// 扫描浓度
	int	nTryTimes;			// 重试次数
	int nMaxTreadCount;		// 跳跃计数
	int	nTimeout;			// 超时时间
	bool bSubScan;			// 是否为子网扫描
}ScanParameter;

////////////////////////////////////////////////////////////////////
// 接口方法类
// (此类为静态类, 故只适用于一个的 Scan 进程的情况, 若要使用多进程需改造本类)

class ScanUtils
{
public:
	static void SetRootDir( const string &sDir );				// 设置根目录, 注意: 目录名最后不要加 "/"
	
	// 以下为接口方法
	static bool StartScan( const ScanParameter& sp,				// 开始扫描
						const string& sQuename, 
						bool bRestart, bool bAsync = false );	
	static bool CancelScan( const string& sQuename );			// 取消扫描(结束扫描)
	static bool GetScanParameter( const string& sQueName,		// 从 SVDB 取得扫描参数
						ScanParameter& sp );	
	static bool SubmitScanResult( const string& sQueName,		// 提交扫描参数
						const ::SliceNNM::TopologyChart& tcSlice );	
	static bool GetScanResult( const string& sQueName,			// 取得扫描结果
						::SliceNNM::TopologyChart& tcSlice );	
	static bool StartRealTimeScan( const string& sSession,		// 开始实时扫描
						int nFreq, const ::SliceNNM::StringLst &idList );	
	static bool CancelRealTimeScan( const string& sSession );	// 取消实时扫描(结束扫描)

protected:
	static bool IsScanRunning();		// 检查 Scan 进程是否已经在运行
	static bool KillScanProcess();		// 终止(杀死) Scan 进程

	static bool WriteScanParamToSvdb( const ScanParameter& sp,	// 将扫描参数保存到 SVDB 中
										const string& sQuename );

	// 复制扫描参数结构
	static void CopyScanParameter( ScanParameter &spDest, const ScanParameter &spSrc );
	
public:
	static PROCESS_INFORMATION m_piScanProcessInfo;		// Scan 进程信息
	static string m_sRootDir;							// Scan 进程根目录
	static ScanParameter m_spScanParam;					// 扫描参数结构

};	// class ScanUtils

};	// namespace NS_ScanSvdbUtils

////////////////////////////////////////////////////////////////////
#endif