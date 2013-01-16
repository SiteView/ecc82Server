// **********************************************************************
// 文件: DatHandleUtils.h
// 描述: 数据操作实用方法集, 实现 slice 接口各种结构化数据的处理
// 创建: Benny 2007.8.7
// 
// Copyright (c) 2003-2007 Dragonflow, Inc. All rights reserved.
//
// **********************************************************************


#ifndef DATHANDLEUTILS_H
#define DATHANDLEUTILS_H
//----------------------------------------------------------------------

#include "sliceDataWrap.h"
#include "StringMap.h"
#include "EntityRelation.h"
#include "Port.h"
#include "Edge.h"
#include "libutil/time.h"
#include "RecordSet.h"
#include "TopologyChart.h"

#include "ScanSvdbUtils.h"		// 与扫描相关的 SVDB 方法

using namespace std;
using namespace NS_ScanSvdbUtils;

typedef std::list< string > ListOfString;		// 字符串列表

//----------------------------------------------------------------------
// 名空间
namespace NS_DatHandleUtils
{

// 类声明
class DatHandleUtils
{
public:
	// 在下面添加类的公开静态方法

	/////////////////////////////////////////////////////////////////////
	// Entity 相关的数据转换方法
	// svdb 的 StringMap 和 slice 的 StringDict 相互转换
	static int StringMap2StringDict( const StringMap &strMap, ::SliceNNM::StringDict &strDict );
	static int StringDict2StringMap( const ::SliceNNM::StringDict &strDict, StringMap &strMap );
	static int StringMapCopy( const StringMap &smSrc, StringMap &smDest );

	// svdb 的 EntityRelationList 和 slice 的 RelationDict 相互转换
	static int EntityRelationList2RelationDict( const ENTITYRELATIONLIST &entRelaList, ::SliceNNM::RelationDict &relaDict );
	static int RelationDict2EntityRelationList( const ::SliceNNM::RelationDict &relaDict, ENTITYRELATIONLIST &entRelaList );

	// svdb 的 PortMap 和 slice 的 PortDict 相互转换
	static int PortMap2PortDict( const PORTMAP &portMap, ::SliceNNM::PortDict &portDict );
	static int PortDict2PortMap( const ::SliceNNM::PortDict &portDict, PORTMAP &portMap );
	static void FreePortMap( PORTMAP &portMap );	// 释放端口表中的端口对象内存

	// svdb 的 PAIRLIST 和 slice 的 StringDict 相互转换
	static int PairList2StringDict( const PAIRLIST &listOfPair, ::SliceNNM::StringDict &strDict );
	static int StringDict2PairList( const ::SliceNNM::StringDict &strDict, PAIRLIST &listOfPair );

	// svdb 的 WORDLIST 和 slice 的 StringList 相互转换
	static int WORDLIST2StringList( const WORDLIST &wdList, ::SliceNNM::StringLst &strList );
	static int StringList2WORDLIST( const ::SliceNNM::StringLst &strList, WORDLIST &wdList );

	// svdb 的 EDGELIST 和 slice 的 EDGELIST 相互转换
	static int EDGELISTSvdb2Slice( const EDGELIST &elSvdb, ::SliceNNM::EDGELIST &elSlice );
	static int EDGELISTSlice2Svdb( const ::SliceNNM::EDGELIST &elSlice, EDGELIST &elSvdb );

	// svdb 的 STRINGMAPLIST 和 slice 的 STRMAPLIST 相互转换
	static int STRINGMAPLISTSvdb2Slice( const STRMAPLIST &smlSvdb, ::SliceNNM::STRINGMAPLIST &smlSlice );
	static int STRINGMAPLISTSlice2Svdb( const ::SliceNNM::STRINGMAPLIST &smlSlice, STRMAPLIST &smlSvdb );

	// svdb 的 RECORDLIST 和 slice 的 RecordList 相互转换
	static int RecordSetSvdb2Slice( ::RecordSet &rsSvdb, ::SliceNNM::RecordSet &rsSlice );

	// svdb 的扫描参数与 slice 的扫描参数转换
	static int ScanParaSlice2Svdb( const ::SliceNNM::ScanConfig& sc, ScanParameter &sp );
	static int ScanParaSvdb2Slice( const ScanParameter &sp, ::SliceNNM::ScanConfig& sc );

	// svdb 的 TopologyChart 与 Slice 相互转换
	static int TopologyChartSlice2Svdb( const ::SliceNNM::TopologyChart& tcSlice, TopologyChart &tcSvdb );
	static int TopologyChartSvdb2Slice( const TopologyChart &tcSvdb, ::SliceNNM::TopologyChart& tcSlice );

	// 将时间字符串转换成时间格式, 接受的格式: yyyy-mm-dd hh:mm:ss
	static bool String2TTime( const ::std::string &strTime, TTime &tm );

	

};	// end of class DatHandleUtils

};	// end of namespace NS_DatHandleUtils
//----------------------------------------------------------------------
#endif