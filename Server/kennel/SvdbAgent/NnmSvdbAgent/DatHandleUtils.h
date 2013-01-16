// **********************************************************************
// �ļ�: DatHandleUtils.h
// ����: ���ݲ���ʵ�÷�����, ʵ�� slice �ӿڸ��ֽṹ�����ݵĴ���
// ����: Benny 2007.8.7
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

#include "ScanSvdbUtils.h"		// ��ɨ����ص� SVDB ����

using namespace std;
using namespace NS_ScanSvdbUtils;

typedef std::list< string > ListOfString;		// �ַ����б�

//----------------------------------------------------------------------
// ���ռ�
namespace NS_DatHandleUtils
{

// ������
class DatHandleUtils
{
public:
	// �����������Ĺ�����̬����

	/////////////////////////////////////////////////////////////////////
	// Entity ��ص�����ת������
	// svdb �� StringMap �� slice �� StringDict �໥ת��
	static int StringMap2StringDict( const StringMap &strMap, ::SliceNNM::StringDict &strDict );
	static int StringDict2StringMap( const ::SliceNNM::StringDict &strDict, StringMap &strMap );
	static int StringMapCopy( const StringMap &smSrc, StringMap &smDest );

	// svdb �� EntityRelationList �� slice �� RelationDict �໥ת��
	static int EntityRelationList2RelationDict( const ENTITYRELATIONLIST &entRelaList, ::SliceNNM::RelationDict &relaDict );
	static int RelationDict2EntityRelationList( const ::SliceNNM::RelationDict &relaDict, ENTITYRELATIONLIST &entRelaList );

	// svdb �� PortMap �� slice �� PortDict �໥ת��
	static int PortMap2PortDict( const PORTMAP &portMap, ::SliceNNM::PortDict &portDict );
	static int PortDict2PortMap( const ::SliceNNM::PortDict &portDict, PORTMAP &portMap );
	static void FreePortMap( PORTMAP &portMap );	// �ͷŶ˿ڱ��еĶ˿ڶ����ڴ�

	// svdb �� PAIRLIST �� slice �� StringDict �໥ת��
	static int PairList2StringDict( const PAIRLIST &listOfPair, ::SliceNNM::StringDict &strDict );
	static int StringDict2PairList( const ::SliceNNM::StringDict &strDict, PAIRLIST &listOfPair );

	// svdb �� WORDLIST �� slice �� StringList �໥ת��
	static int WORDLIST2StringList( const WORDLIST &wdList, ::SliceNNM::StringLst &strList );
	static int StringList2WORDLIST( const ::SliceNNM::StringLst &strList, WORDLIST &wdList );

	// svdb �� EDGELIST �� slice �� EDGELIST �໥ת��
	static int EDGELISTSvdb2Slice( const EDGELIST &elSvdb, ::SliceNNM::EDGELIST &elSlice );
	static int EDGELISTSlice2Svdb( const ::SliceNNM::EDGELIST &elSlice, EDGELIST &elSvdb );

	// svdb �� STRINGMAPLIST �� slice �� STRMAPLIST �໥ת��
	static int STRINGMAPLISTSvdb2Slice( const STRMAPLIST &smlSvdb, ::SliceNNM::STRINGMAPLIST &smlSlice );
	static int STRINGMAPLISTSlice2Svdb( const ::SliceNNM::STRINGMAPLIST &smlSlice, STRMAPLIST &smlSvdb );

	// svdb �� RECORDLIST �� slice �� RecordList �໥ת��
	static int RecordSetSvdb2Slice( ::RecordSet &rsSvdb, ::SliceNNM::RecordSet &rsSlice );

	// svdb ��ɨ������� slice ��ɨ�����ת��
	static int ScanParaSlice2Svdb( const ::SliceNNM::ScanConfig& sc, ScanParameter &sp );
	static int ScanParaSvdb2Slice( const ScanParameter &sp, ::SliceNNM::ScanConfig& sc );

	// svdb �� TopologyChart �� Slice �໥ת��
	static int TopologyChartSlice2Svdb( const ::SliceNNM::TopologyChart& tcSlice, TopologyChart &tcSvdb );
	static int TopologyChartSvdb2Slice( const TopologyChart &tcSvdb, ::SliceNNM::TopologyChart& tcSlice );

	// ��ʱ���ַ���ת����ʱ���ʽ, ���ܵĸ�ʽ: yyyy-mm-dd hh:mm:ss
	static bool String2TTime( const ::std::string &strTime, TTime &tm );

	

};	// end of class DatHandleUtils

};	// end of namespace NS_DatHandleUtils
//----------------------------------------------------------------------
#endif