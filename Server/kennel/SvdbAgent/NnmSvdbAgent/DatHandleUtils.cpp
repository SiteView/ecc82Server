// **********************************************************************
// 文件: DatHandleUtils.cpp
// 描述: 数据操作实用方法集的实现, 实现 slice 接口各种结构化数据的处理
// 创建: Benny 2007.8.7
// 
// Copyright (c) 2003-2007 Dragonflow, Inc. All rights reserved.
//
// **********************************************************************

#include <afx.h>
#include "DatHandleUtils.h"
#include "SliceUtil.h"

#include "LogUtils.h"		// 日志实用函数

using namespace NS_DatHandleUtils;
using namespace SV_LogUtils;

///////////////////////////////////////////////////////////
// svdb 的 StringMap 和 slice 的 StringDict 相互转换
///////////////////////////////////////////////////////////
int DatHandleUtils::StringMap2StringDict( const StringMap &strMap, ::SliceNNM::StringDict &strDict )
{
	// cout << "Call: DatHandleUtils::StringMap2StringDict ... ..." << endl;

	int nCount = 0;

	try
	{
		strDict.clear();

		StringMap &mapOfStr = const_cast< StringMap &>( strMap );
		StringMap::iterator it;
		while( mapOfStr.findnext( it ) )	
		{		
			strDict[ StringToUtf8( (*it).getkey().getword() ) ] = StringToUtf8( (*it).getvalue().getword() );
			nCount++;
		}
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::StringMap2StringDict - *exception*", nCount, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::StringMap2StringDict - *exception*" << endl;
	}

	cout << "****** DatHandleUtils::StringMap2StringDict - all done! - count : " << nCount << endl;

	return nCount;
}
///////////////////////////////////////////////////////////
int DatHandleUtils::StringDict2StringMap( const ::SliceNNM::StringDict &strDict, StringMap &strMap )
{
	// cout << "Call: DatHandleUtils::StringDict2StringMap ... ..." << endl;

	int nCount = 0;

	try
	{
		strMap.clear();
		::SliceNNM::StringDict &dictOfStr = const_cast< ::SliceNNM::StringDict &>( strDict );
		for( ::SliceNNM::StringDict::iterator it = dictOfStr.begin(); it != dictOfStr.end(); it++ )
		{
			strMap[ Utf8ToString( it->first ) ] = Utf8ToString( it->second );

			nCount++;
		}
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::StringDict2StringMap - *exception*", nCount, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::StringDict2StringMap - *exception*" << endl;
	}

	cout << "****** DatHandleUtils::StringDict2StringMap - all done! - count : count: " << nCount << endl;

	return nCount;
}

/////////////////////////////////////////////////////////////
// 复制字符映射
int DatHandleUtils::StringMapCopy( const StringMap &smSrc, StringMap &smDest )
{
	// cout << "Call: DatHandleUtils::StringMapCopy ... ..." << endl;

	int nCount = 0;

	try
	{
		smDest.clear();

		StringMap &smSrc = const_cast< StringMap &>( smSrc );
		StringMap::iterator it;
		while( smSrc.findnext( it ) )	
		{		
			smDest[ (*it).getkey().getword() ] = (*it).getvalue().getword();
			nCount++;
		}
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::StringMapCopy - *exception*", nCount, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::StringMapCopy - *exception*" << endl;
	}

	cout << "****** DatHandleUtils::StringMapCopy - all done! - count : " << nCount << endl;

	return nCount;
}

/////////////////////////////////////////////////////////////
// svdb 的 StringMap 和 slice 的 StringDict 相互转换
/////////////////////////////////////////////////////////////
int DatHandleUtils::EntityRelationList2RelationDict( const ENTITYRELATIONLIST &entRelaList, ::SliceNNM::RelationDict &relaDict )
{
	// cout << "Call: DatHandleUtils::EntityRelationList2RelationDict ... ..." << endl;

	int nCount = 0;

	try
	{
		relaDict.clear();

		ENTITYRELATIONLIST &listOfEntRela = const_cast< ENTITYRELATIONLIST &>( entRelaList );
		for( ENTITYRELATIONLIST::iterator it = listOfEntRela.begin();
			it != listOfEntRela.end(); it++ )
		{
			::SliceNNM::EntityRelation entRela;
			
			entRela.Type		= StringToUtf8( (*it)->GetType().getword() );
			entRela.Description = StringToUtf8( (*it)->GetDescription().getword() );
			entRela.PeerID		= StringToUtf8( (*it)->GetPeerID().getword() );
			entRela.LocalPortId = StringToUtf8( (*it)->GetLocalPortID().getword() );
			
			StringMap &prop = (*it)->GetProperty();
			StringMap2StringDict( prop, entRela.Property );

			relaDict.push_back( entRela );

			nCount++;
		}
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::EntityRelationList2RelationDict - *exception*", nCount, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::EntityRelationList2RelationDict - *exception*" << endl;
	}

	cout << "****** DatHandleUtils::EntityRelationList2RelationDict - all done! - count : " << nCount << endl;

	return nCount;


	//definition in svdb

	//typedef std::list< EntityRelation * >	ENTITYRELATIONLIST;


	//definition in slice

	//struct EntityRelation

	//::std::string Type;
 //   ::std::string Description;
 //   ::std::string PeerID;
 //   ::std::string LocalPortId;
 //   ::SliceNNM::StringDict Property;

	//typedef ::std::vector< ::SliceNNM::EntityRelation> RelationDict;

}
/////////////////////////////////////////////////////////////
int DatHandleUtils::RelationDict2EntityRelationList( const ::SliceNNM::RelationDict &relaDict, ENTITYRELATIONLIST &entRelaList )
{
	// cout << "Call: DatHandleUtils::RelationDict2EntityRelationList ... ..." << endl;


	int nCount = 0;

	try
	{
		entRelaList.clear();
		::SliceNNM::RelationDict &dictOfRela = const_cast< ::SliceNNM::RelationDict &>( relaDict );
		for( ::SliceNNM::RelationDict::iterator it = dictOfRela.begin(); it != dictOfRela.end(); it++ )
		{
			EntityRelation entiRela;

			entiRela.PutType( Utf8ToString( it->Type ) );
			entiRela.PutDescription( Utf8ToString( it->Description ) );
			entiRela.PutPeerID( Utf8ToString( it->PeerID ) );
			entiRela.PutLocalPortID( Utf8ToString( it->LocalPortId ) );

			StringMap &entiRelaProp = entiRela.GetProperty();
			StringDict2StringMap( it->Property, entiRelaProp );

			entRelaList.push_back( &entiRela );

			nCount++;
		}
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::RelationDict2EntityRelationList - *exception*", nCount, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::RelationDict2EntityRelationList - *exception*" << endl;
	}

	cout << "****** DatHandleUtils::RelationDict2EntityRelationList - all done! - count : " << nCount << endl;

	return nCount;
}

/////////////////////////////////////////////////////////////
// 将 svdb 的 PortMap 转换成 slice 的 PortDict
/////////////////////////////////////////////////////////////
int DatHandleUtils::PortMap2PortDict( const PORTMAP &portMap, ::SliceNNM::PortDict &portDict )
{
	// cout << "Call: DatHandleUtils::PortMap2PortDict ... ..." << endl;

	int nCount = 0;

	try
	{

		portDict.clear();

		PORTMAP &mapOfPort = const_cast< PORTMAP & >( portMap );
		PORTMAP::iterator it;
		while( mapOfPort.findnext( it ) )	
		{		
			::SliceNNM::Port portSlice;
			Port *pt = (*it).getvalue();
			if( NULL != pt )
			{
				portSlice.ID			= StringToUtf8( pt->GetID().getword() );
				portSlice.PeerPortID	= StringToUtf8( pt->GetPeerPortID().getword() );
				portSlice.MonitorID		= StringToUtf8( pt->GetMonitorID().getword() );
			
				StringMap &prop = pt->GetProperty();
				StringMap2StringDict( prop, portSlice.Property );	
			}		

			portDict[ StringToUtf8( (*it).getkey().getword() ) ] = portSlice;
			nCount++;
		}
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::PortMap2PortDict - *exception*", nCount, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::PortMap2PortDict - *exception*" << endl;
	}

	cout << "****** DatHandleUtils::PortMap2PortDict - all done! - count : " << nCount << endl;

	return nCount;



	//definition in svdb

	//typedef svutil::hashtable<svutil::word,Port * >	PORTMAP;



	//definition in slice

	//struct Port

 //   ::std::string ID;
 //   ::std::string PeerPortID;
 //   ::std::string MonitorID;
 //   ::SliceNNM::StringDict Property;

	//typedef ::std::map< ::std::string, ::SliceNNM::Port> PortDict;

}
/////////////////////////////////////////////////////////////
int DatHandleUtils::PortDict2PortMap( const ::SliceNNM::PortDict &portDict, PORTMAP &portMap )
{
	// cout << "Call: DatHandleUtils::PortDict2PortMap ... ..." << endl;

	int nCount = 0;

	try
	{
		::SliceNNM::PortDict &dictOfPort = const_cast< ::SliceNNM::PortDict &>( portDict );	
		for( ::SliceNNM::PortDict::iterator it = dictOfPort.begin(); it != dictOfPort.end(); it++ )
		{
			Port *pPort = new Port();
			// 此处分配的内存在功能完成后, 调用 FreePortMap 释放

			pPort->PutID( Utf8ToString( it->second.ID ) );
			pPort->PutPeerPortID( Utf8ToString( it->second.PeerPortID ) );
			pPort->PutMonitorID( Utf8ToString( it->second.MonitorID ) );

			StringMap &prop = pPort->GetProperty();
			StringDict2StringMap( it->second.Property, prop );

			portMap[ Utf8ToString( it->first ).c_str() ] = pPort;

			nCount++;
		}
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::PortDict2PortMap - *exception*", nCount, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::PortDict2PortMap - *exception*" << endl;
	}

	cout << "****** DatHandleUtils::PortDict2PortMap - all done! - count : " << nCount << endl;

	return nCount;
}
/////////////////////////////////////////////////////////////
// 释放端口表中的端口对象内存
void DatHandleUtils::FreePortMap( PORTMAP &portMap )
{
	try
	{
		PORTMAP::iterator it;
		while( portMap.findnext( it ) )
		{
			Port *pPort = (*it).getvalue();
			if( NULL != pPort )
			{
				delete pPort;
				pPort = NULL;
			}
		}
	}
	catch( ... )
	{	// 容错处理
		SvLog::Write( "******** DatHandleUtils::FreePortMap - *exception*", 0, __FILE__, __LINE__ );
	}
}

/////////////////////////////////////////////////////////////
// svdb 的 PAIRLIST 转换成 slice 的 StringDict
/////////////////////////////////////////////////////////////
int DatHandleUtils::PairList2StringDict( const PAIRLIST &listOfPair, ::SliceNNM::StringDict &strDict )
{
	// cout << "Call: DatHandleUtils::PairList2StringDict ... ..." << endl;

	int nCount = 0;

	try
	{
		PAIRLIST &lp = const_cast< PAIRLIST & >( listOfPair );
		strDict.clear();

		for( PAIRLIST::iterator it = lp.begin(); it != lp.end(); it++ )
		{
			strDict[ StringToUtf8( it->name ) ] = StringToUtf8( it->value );

			// cout << StringToUtf8( it->name ) << " : " << StringToUtf8( it->value ) << endl;

			nCount++;
		}
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::PairList2StringDict - *exception*", nCount, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::PairList2StringDict - *exception*" << endl;
	}

	cout << "****** DatHandleUtils::PairList2StringDict - all done! - count : " << nCount << endl;

	return nCount;
}
/////////////////////////////////////////////////////////////
// slice 的 StringDict 转换成 svdb 的 PAIRLIST
int DatHandleUtils::StringDict2PairList( const ::SliceNNM::StringDict &strDict, PAIRLIST &listOfPair )
{
	// cout << "Call: DatHandleUtils::StringDict2PairList ... ..." << endl;

	int nCount = 0;

	try
	{
		listOfPair.clear();

		::SliceNNM::StringDict &dictOfStr = const_cast< ::SliceNNM::StringDict & >( strDict );
		for( ::SliceNNM::StringDict::iterator it = dictOfStr.begin(); it != dictOfStr.end(); it++ )
		{
			struct sv_pair svPair;

			svPair.name		= Utf8ToString( it->first );
			svPair.value	= Utf8ToString( it->second );

			listOfPair.push_back( svPair );
			nCount++;
		}
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::StringDict2PairList - *exception*", nCount, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::StringDict2PairList - *exception*" << endl;
	}

	cout << "****** DatHandleUtils::StringDict2PairList - all done! - count : " << nCount << endl;

	return nCount;
}

/////////////////////////////////////////////////////////////
// svdb 的 WORDLIST 和 slice 的 StringList 相互转换
int DatHandleUtils::WORDLIST2StringList( const WORDLIST &wdList, ::SliceNNM::StringLst &strList )
{
	// cout << "Call: DatHandleUtils::WORDLIST2StringList ... ..." << endl;

	int nCount = 0;

	try
	{
		strList.clear();
		WORDLIST &listOdWord = const_cast< WORDLIST &>( wdList );

		for( WORDLIST::iterator it = listOdWord.begin(); it != listOdWord.end(); it++ )
		{
			strList.push_back( StringToUtf8( it->getword() ) );

			nCount++;
		}
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::WORDLIST2StringList - *exception*", nCount, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::WORDLIST2StringList - *exception*" << endl;
	}

	cout << "****** DatHandleUtils::WORDLIST2StringList - all done! - count : " << nCount << endl;

	return nCount;
}

/////////////////////////////////////////////////////////////
int DatHandleUtils::StringList2WORDLIST( const ::SliceNNM::StringLst &strList, WORDLIST &wdList )
{
	// cout << "Call: DatHandleUtils::StringList2WORDLIST ... ..." << endl;

	int nCount = 0;

	try
	{
		wdList.clear();
		::SliceNNM::StringLst &listOfStr = const_cast< ::SliceNNM::StringLst &>( strList );

		for( ::SliceNNM::StringLst::iterator it = listOfStr.begin(); it != listOfStr.end(); it++ )
		{
			wdList.push_back( Utf8ToString( (*it) ) );

			nCount++;
		}
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::StringList2WORDLIST - *exception*", nCount, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::StringList2WORDLIST - *exception*" << endl;
	}

	cout << "****** DatHandleUtils::StringList2WORDLIST - all done! - count : " << nCount << endl;

	return nCount;
}

/////////////////////////////////////////////////////////////
// svdb 的 EDGELIST 和 slice 的 EDGELIST 相互转换
/////////////////////////////////////////////////////////////
int DatHandleUtils::EDGELISTSvdb2Slice( const EDGELIST &elSvdb, ::SliceNNM::EDGELIST &elSlice )
{

	// cout << "Call: DatHandleUtils::EDGELISTSvdb2Slice ... ..." << endl;

	int nCount = 0;

	try
	{
		elSlice.clear();
		EDGELIST &listOfEdge = const_cast< EDGELIST & >( elSvdb );

		SvLog::Write( "*************** EDGELIST of svdb begin ", elSvdb.size(), __FILE__, __LINE__ );

		for( EDGELIST::iterator it = listOfEdge.begin(); it != listOfEdge.end(); it++ )
		{
			::SliceNNM::Edge edgSlice;

			edgSlice.LeftPortID			= StringToUtf8( (*it)->GetLeftPortID().getword() );
			edgSlice.RightPortID		= StringToUtf8( (*it)->GetRightPortID().getword() );
			edgSlice.DependMonitorID	= StringToUtf8( (*it)->GetDependMonitorID().getword() );

			StringMap &prop = (*it)->GetProperty();
			StringMap2StringDict( prop, edgSlice.Property );
		
			elSlice.push_back( edgSlice );
			
			SvLog::Write( "*************** Edge begin ", nCount, __FILE__, __LINE__ );
			SvLog::Write( "LeftPortID - count : ", nCount, (*it)->GetLeftPortID().getword(), __LINE__ );
			SvLog::Write( "RightPortID - count : ", nCount, (*it)->GetRightPortID().getword(), __LINE__ );
			SvLog::Write( "DependMonitorID - count : ", nCount, (*it)->GetDependMonitorID().getword(), __LINE__ );

			SvLog::Write( "Properties : ", 0, (*it)->GetDependMonitorID().getword(), __LINE__ );
			StringMap::iterator it2;
			int nIdx = 0;
			while( prop.findnext( it2 ) )	
			{
				SvLog::Write( (*it2).getkey().getword(), ++nIdx, (*it2).getvalue().getword(), __LINE__ );
			}
			SvLog::Write( "*************** Edge end ", nCount, __FILE__, __LINE__ );

			nCount++;
		}

		SvLog::Write( "*************** EDGELIST of svdb end ", nCount, __FILE__, __LINE__ );
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::EDGELISTSvdb2Slice - *exception*", nCount, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::EDGELISTSvdb2Slice - *exception*" << endl;
	}

	cout << "****** DatHandleUtils::EDGELISTSvdb2Slice - all done! - count : " << nCount << endl;
	SvLog::Write( "DatHandleUtils::EDGELISTSvdb2Slice - count : ", nCount, __FILE__, __LINE__ );

	return nCount;

	//::SliceNNM::Edge
	//::std::string LeftPortID;
 //   ::std::string RightPortID;
 //   ::std::string DependMonitorID;
 //   ::SliceNNM::StringDict Property;

}
/////////////////////////////////////////////////////////////
int DatHandleUtils::EDGELISTSlice2Svdb( const ::SliceNNM::EDGELIST &elSlice, EDGELIST &elSvdb )
{

	// cout << "Call: DatHandleUtils::EDGELISTSlice2Svdb ... ..." << endl;

	int nCount = 0;

	try
	{
		elSvdb.clear();
		::SliceNNM::EDGELIST &listOfEdge = const_cast< ::SliceNNM::EDGELIST & >( elSlice );
		for( ::SliceNNM::EDGELIST::iterator it = listOfEdge.begin(); it != listOfEdge.end(); it++ )
		{
			Edge edgSvdb;

			edgSvdb.PutLeftPortID( Utf8ToString( it->LeftPortID ) );
			edgSvdb.PutRightPortID( Utf8ToString( it->RightPortID ) );
			edgSvdb.PutDependMonitorID( Utf8ToString( it->DependMonitorID ) );

			StringMap &prop = edgSvdb.GetProperty();
			StringDict2StringMap( it->Property, prop );
		
			elSvdb.push_back( &edgSvdb );

			nCount++;
		}
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::EDGELISTSlice2Svdb - *exception*", nCount, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::EDGELISTSlice2Svdb - *exception*" << endl;
	}

	cout << "****** DatHandleUtils::EDGELISTSlice2Svdb - all done! - count : " << nCount << endl;

	return nCount;
}

/////////////////////////////////////////////////////////////
// svdb 的 STRINGMAPLIST 和 slice 的 STRMAPLIST 相互转换
/////////////////////////////////////////////////////////////
int DatHandleUtils::STRINGMAPLISTSvdb2Slice( const STRMAPLIST &smlSvdb, ::SliceNNM::STRINGMAPLIST &smlSlice )
{

	// cout << "Call: DatHandleUtils::STRINGMAPLISTSvdb2Slice ... ..." << endl;

	int nCount = 0;

	try
	{
		// 初始输出数据结构(清空)
		for( ::SliceNNM::STRINGMAPLIST::iterator it = smlSlice.begin(); it != smlSlice.end(); it++ )
		{
			it->clear();	// 清除每个 StringMapList 无素中的 StringDict 中的元素
		}
		smlSlice.clear();

		// 转换并将数据赋给输入结构
		STRMAPLIST &lmOfStrSvdb = const_cast< STRMAPLIST & >( smlSvdb );
		for( STRMAPLIST::iterator it = lmOfStrSvdb.begin(); it != lmOfStrSvdb.end(); it++ )
		{
			::SliceNNM::StringDict strDict;

			StringMap2StringDict( *(*it), strDict );
		
			smlSlice.push_back( strDict );

			nCount++;
		}
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::STRINGMAPLISTSvdb2Slice - *exception*", nCount, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::STRINGMAPLISTSvdb2Slice - *exception*" << endl;
	}

	cout << "****** DatHandleUtils::STRINGMAPLISTSvdb2Slice - all done! - count : " << nCount << endl;

	return nCount;
}

/////////////////////////////////////////////////////////////
int DatHandleUtils::STRINGMAPLISTSlice2Svdb( const ::SliceNNM::STRINGMAPLIST &smlSlice, STRMAPLIST &smlSvdb )
{
	// cout << "Call: DatHandleUtils::STRINGMAPLISTSlice2Svdb ... ..." << endl;

	int nCount = 0;

	try
	{
		// 初始输出数据结构(清空)
		for( STRMAPLIST::iterator it = smlSvdb.begin(); it != smlSvdb.end(); it++ )
		{
			(*it)->clear();	// 清除每个 StringMapList 无素中的 StringDict 中的元素
		}
		smlSvdb.clear();

		// 转换并将数据赋给输入结构
		::SliceNNM::STRINGMAPLIST &mapOfStrSlice = const_cast< ::SliceNNM::STRINGMAPLIST & >( smlSlice );
		for( ::SliceNNM::STRINGMAPLIST::iterator it = mapOfStrSlice.begin(); it != mapOfStrSlice.end(); it++ )
		{
			StringMap strMap;

			StringDict2StringMap( *it, strMap );
		
			smlSvdb.push_back( &strMap );

			nCount++;
		}
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::STRINGMAPLISTSlice2Svdb - *exception*", nCount, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::STRINGMAPLISTSlice2Svdb - *exception*" << endl;
	}

	cout << "****** DatHandleUtils::STRINGMAPLISTSlice2Svdb - all done! - count : " << nCount << endl;

	return nCount;
}


/////////////////////////////////////////////////////////////
// svdb 的 RECORDLIST 和 slice 的 RecordList 相互转换
/////////////////////////////////////////////////////////////
int DatHandleUtils::RecordSetSvdb2Slice( ::RecordSet &rsSvdb, ::SliceNNM::RecordSet &rsSlice )
{	
	// cout << "Call: DatHandleUtils::RecordSetSvdb2Slice ... ..." << endl;

	try
	{
		// 初始化输出参数(清空输出记录集)
		for( ::SliceNNM::RecordList::iterator it = rsSlice.records.begin(); 
			it != rsSlice.records.end(); it++ )
		{
			it->data.clear();		// 清空每条记录的字段值列表 data ( StringLst 类型 )
		}
		rsSlice.fileds.clear();	// 清空字段名列表 fields( StringLst 类型 )

		bool bFieldsFilled = false;	// 字段名是否填充的标志

		// 将 svdb 中的数据复制到 slice RecordSet 中
		::RecordSet &rsSvdbSrc = const_cast< ::RecordSet & >( rsSvdb );
		rsSlice.recordcount = rsSvdbSrc.m_records.size();		// 记录数
		rsSlice.filedcount = 0;									// 字段数

		// 复制每一条记录
		// 在此迭代过程中, 迭代器 it 为 Record*, 见 RecordSet.h 中的定义
		// 定义: typedef std::list<Record *> RECORDLIST;
		for( RECORDLIST::iterator itRecPtr = rsSvdb.m_records.begin(); 
			itRecPtr != rsSvdb.m_records.end(); itRecPtr++ )
		{
			::SliceNNM::Record recSlice;		// Slice 中定义的记录

			recSlice.state = (*itRecPtr)->GetState();	// 记录状态		

			// 复制每一个字段
			// 定义: typedef svutil::hashtable< svutil::word,RecordValue * > VALUEMAP;

			VALUEMAP::iterator itField;
			while( (*itRecPtr)->GetVALUEMAP().findnext( itField ) )	
			{
				char szBuf[ 128 ];			// 临时缓冲区,用于非字符串转化成字符串
				::std::string sFieldVal;	// 每个字段的值(各种类型均转化成字符串)
				
				memset( szBuf, 0x00, sizeof( szBuf ) );

				if( !bFieldsFilled )
				{	// 尚未填充字段, 将字段填充到 fields( StringLst 类型 )中
					rsSlice.fileds.push_back( StringToUtf8( (*itField).getkey().getword() ) );
					++rsSlice.filedcount;
				}

				sFieldVal = "";

				// 根据字段的类型进行转化
				switch( (*itField).getvalue()->vt )
				{	
				case DataType::valuetype::inttype:		// 整数类型
					sprintf( szBuf, "%d", (*itField).getvalue()->dvalue.iv );
					sFieldVal = string( szBuf );
					break;

				case DataType::valuetype::floattype:	// 浮点数类型
					sprintf( szBuf, "%lf", (*itField).getvalue()->dvalue.fv );
					sFieldVal = string( szBuf );
					break;

				case DataType::valuetype::stringtype:	// 字符串类型
					sFieldVal = StringToUtf8( string( (*itField).getvalue()->dvalue.sv ) );
					break;

				case DataType::valuetype::nulltype:
				default:	// 未定义类型
					break;
				}

				recSlice.data.push_back( sFieldVal );		// 将字段值加到行数据中
			}

			rsSlice.records.push_back( recSlice );	// 将记录加到记录集中
		}
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::RecordSetSvdb2Slice - *exception*", 0, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::RecordSetSvdb2Slice - *exception*" << endl;
	}

	cout << "****** DatHandleUtils::RecordSetSvdb2Slice - all done! - count : " << rsSlice.recordcount << endl;

	return rsSlice.recordcount;

	//slice
	//struct RecordSet
 //   ::Ice::Int filedcount;
 //   ::SliceNNM::StringLst fileds;
 //   ::Ice::Int recordcount;
 //   ::SliceNNM::RecordList records;
	// typedef ::std::vector< ::SliceNNM::Record> RecordList;
//	struct Record
//     ::Ice::Int state;
//    ::SliceNNM::StringLst data;
}

/////////////////////////////////////////////////////////////
// svdb 的扫描参数与 slice 的扫描参数转换
/////////////////////////////////////////////////////////////
int DatHandleUtils::ScanParaSlice2Svdb( const ::SliceNNM::ScanConfig& sc, ScanParameter &sp )
{
	// cout << "Call: DatHandleUtils::ScanParaSlice2Svdb ... ..." << endl;

	try
	{
		// 初始化输出参数
		sp.vcCommunityList.clear();
		sp.vsSeedIpList.clear();
		sp.viIpScopeList.clear();
		sp.viFilterStartEndList.clear();
		sp.viSubnetList.clear();

		// 数据格式转换
		sp.sDefaultCommunityOfGet	= Utf8ToString( sc.DefaultCommunityGet );
		sp.sDefaultCommunityOfSet	= Utf8ToString( sc.DefaultCommunitySet );
		sp.nDepth					= sc.Depth;
		sp.nTryTimes				= sc.TryTimes;
		sp.nMaxTreadCount			= sc.MaxTreadCount;
		sp.nTimeout					= sc.Timeout;
		sp.bSubScan					= sc.isSubScan;	

		::SliceNNM::ScanConfig &scSlice = const_cast< ::SliceNNM::ScanConfig &>( sc );

		// 复制 community
		for( ::SliceNNM::CommunityLst::iterator it = scSlice.CommunityList.begin();
			it != scSlice.CommunityList.end(); it++ )
		{
			NS_ScanSvdbUtils::Community community;

			community.sStartIp	= Utf8ToString( it->StartIp );
			community.sEndIp	= Utf8ToString( it->EndIp );
			community.sGet		= Utf8ToString( it->Get );
			community.sSet		= Utf8ToString( it->Set );
			
			sp.vcCommunityList.push_back( community );		
		}

		// 复制 SeedIpList
		for( ::SliceNNM::SeedIpLst::iterator it = scSlice.SeedIpList.begin();
			it != scSlice.SeedIpList.end(); it++ )
		{
			sp.vsSeedIpList.push_back( Utf8ToString( *it ) );
		}

		// 复制 IpScopeList
		for( ::SliceNNM::AddStartEndLst::iterator it = scSlice.AddStartEndList.begin();
			it != scSlice.AddStartEndList.end(); it++ )
		{
			IPScope ipScope;

			ipScope.sIpBegin	= Utf8ToString( it->EndIp );
			ipScope.sIpEnd		= Utf8ToString( it->StartIp );

			sp.viIpScopeList.push_back( ipScope );
		}

		// 复制 FilterStartEndList
		for( ::SliceNNM::FilterStartEndLst::iterator it = scSlice.FilterStartEndList.begin();
			it != scSlice.FilterStartEndList.end(); it++ )
		{
			IPScope ipScope;

			ipScope.sIpBegin	= Utf8ToString( it->EndIp );
			ipScope.sIpEnd		= Utf8ToString( it->StartIp );

			sp.viFilterStartEndList.push_back( ipScope );
		}

		// 复制 SubnetList
		for( ::SliceNNM::SubnetLst::iterator it = scSlice.SubnetList.begin();
			it != scSlice.SubnetList.end(); it++ )
		{
			IPScope ipScope;

			ipScope.sIpBegin	= Utf8ToString( it->EndIp );
			ipScope.sIpEnd		= Utf8ToString( it->StartIp );

			sp.viSubnetList.push_back( ipScope );
		}
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::ScanParaSlice2Svdb - *exception*", 0, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::ScanParaSlice2Svdb - *exception*" << endl;
	}
	
	cout << "****** DatHandleUtils::ScanParaSlice2Svdb - all done!" << endl;

	return 1;

	/*
	struct ScanConfig // slice
    ::std::string DefaultCommunityGet;
    ::std::string DefaultCommunitySet;

    ::SliceNNM::CommunityLst CommunityList;
    ::SliceNNM::SeedIpLst SeedIpList;
    ::SliceNNM::AddStartEndLst AddStartEndList;
    ::SliceNNM::FilterStartEndLst FilterStartEndList;
    ::SliceNNM::SubnetLst SubnetList;

    ::Ice::Byte Depth;
    ::Ice::Byte TryTimes;
    ::Ice::Byte MaxTreadCount;
    ::Ice::Int Timeout;
    bool isSubScan;


	struct Community
{
    ::std::string StartIp;
    ::std::string EndIp;
    ::std::string Get;
    ::std::string Set;

	typedef struct _ScanParameter
{	
	string				sDefaultCommunityOfGet;
    string				sDefaultCommunityOfSet;
    VectorOfCommunity	vcCommunityList;
    VectorOfString		vsSeedIpList;
	VectorOfIpScope		viIpScopeList;
	VectorOfIpScope		viFilterStartEndList;
	VectorOfIpScope		viSubnetList;

	unsigned char		nDepth;
	unsigned char		nTryTimes;
	unsigned char		nMaxTreadCount;
	int					nTimeout;
	bool				bSubScan;
}ScanParameter;
	*/
}

/////////////////////////////////////////////////////////////
int DatHandleUtils::ScanParaSvdb2Slice( const ScanParameter &sp, ::SliceNNM::ScanConfig& sc )
{
	// cout << "Call: DatHandleUtils::ScanParaSvdb2Slice ... ..." << endl;

	try
	{
		// 初始化输出参数	
		sc.CommunityList.clear();
		sc.SeedIpList.clear();
		sc.AddStartEndList.clear();
		sc.FilterStartEndList.clear();
		sc.SubnetList.clear();
		
		// 数据格式转换
		sc.DefaultCommunityGet	= StringToUtf8( sp.sDefaultCommunityOfGet );
		sc.DefaultCommunitySet	= StringToUtf8( sp.sDefaultCommunityOfSet );
		sc.Depth				= sp.nDepth;
		sc.TryTimes				= sp.nTryTimes;
		sc.MaxTreadCount		= sp.nMaxTreadCount;
		sc.Timeout				= sp.nTimeout;
		sc.isSubScan			= sp.bSubScan;

		NS_ScanSvdbUtils::ScanParameter &spSvdb = const_cast< NS_ScanSvdbUtils::ScanParameter &>( sp);

		// 复制 community
		for( NS_ScanSvdbUtils::VectorOfCommunity::iterator it = spSvdb.vcCommunityList.begin();
			it != spSvdb.vcCommunityList.end(); it++ )
		{
			::SliceNNM::Community community;

			community.StartIp	= StringToUtf8( it->sStartIp );
			community.EndIp		= StringToUtf8( it->sEndIp );
			community.Get		= StringToUtf8( it->sGet );
			community.Set		= StringToUtf8( it->sSet );
			
			sc.CommunityList.push_back( community );		
		}

		// 复制 SeedIpList
		for( NS_ScanSvdbUtils::VectorOfString::iterator it = spSvdb.vsSeedIpList.begin();
			it != spSvdb.vsSeedIpList.end(); it++ )
		{
			sc.SeedIpList.push_back( StringToUtf8( *it ) );
		}

		// 复制 IpScopeList
		for( NS_ScanSvdbUtils::VectorOfIpScope::iterator it = spSvdb.viIpScopeList.begin();
			it != spSvdb.viIpScopeList.end(); it++ )
		{
			::SliceNNM::StartEnd ipStartEnd;

			ipStartEnd.EndIp	= StringToUtf8( it->sIpBegin );
			ipStartEnd.StartIp	= StringToUtf8( it->sIpEnd );

			sc.AddStartEndList.push_back( ipStartEnd );
		}

		// 复制 FilterStartEndList
		for( NS_ScanSvdbUtils::VectorOfIpScope::iterator it = spSvdb.viFilterStartEndList.begin();
			it != spSvdb.viFilterStartEndList.end(); it++ )
		{
			::SliceNNM::StartEnd ipStartEnd;

			ipStartEnd.EndIp	= StringToUtf8( it->sIpBegin );
			ipStartEnd.StartIp	= StringToUtf8( it->sIpEnd );

			sc.FilterStartEndList.push_back( ipStartEnd );
		}

		// 复制 SubnetList
		for( NS_ScanSvdbUtils::VectorOfIpScope::iterator it = spSvdb.viSubnetList.begin();
			it != spSvdb.viSubnetList.end(); it++ )
		{
			::SliceNNM::StartEnd ipStartEnd;

			ipStartEnd.EndIp	= StringToUtf8( it->sIpBegin );
			ipStartEnd.StartIp	= StringToUtf8( it->sIpEnd );

			sc.FilterStartEndList.push_back( ipStartEnd );
		}
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::ScanParaSvdb2Slice - *exception*", 0, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::ScanParaSvdb2Slice - *exception*" << endl;
	}
	
	cout << "****** DatHandleUtils::ScanParaSvdb2Slice - all done!" << endl;

	return 1;
}

/////////////////////////////////////////////////////////////
// svdb 的 TopologyChart 与 Slice 相互转换
/////////////////////////////////////////////////////////////
int DatHandleUtils::TopologyChartSlice2Svdb( const ::SliceNNM::TopologyChart& tcSlice, TopologyChart &tcSvdb )
{

	// cout << "Call: DatHandleUtils::TopologyChartSlice2Svdb ... ..." << endl;

	try
	{
		// 数据转换
		tcSvdb.PutID( Utf8ToString( tcSlice.ID ) );		// ID
		tcSvdb.PutDescription( Utf8ToString( tcSlice.Description ) );	// 描述

		// 创建时间
		svutil::TTime tmCreate;
		String2TTime( tcSlice.CreateTime, tmCreate );
		tcSvdb.PutCreateTime( tmCreate );

		// 属性
		StringMap &prop = tcSvdb.GetProperty();
		StringDict2StringMap( tcSlice.Property, prop );

		// 边
		EDGELIST &edgs = tcSvdb.GetEdges();
		EDGELISTSlice2Svdb( tcSlice.Edges, edgs );
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::TopologyChartSlice2Svdb - *exception*", 0, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::TopologyChartSlice2Svdb - *exception*" << endl;
	}

	cout << "****** DatHandleUtils::TopologyChartSlice2Svdb - all done!" << endl;

	return 1;
}

/////////////////////////////////////////////////////////////
int DatHandleUtils::TopologyChartSvdb2Slice( const TopologyChart &tcSvdb, 
											::SliceNNM::TopologyChart& tcSlice )
{
	// cout << "Call: DatHandleUtils::TopologyChartSvdb2Slice ... ..." << endl;

	try
	{
		TopologyChart &svdbTC = const_cast< TopologyChart &>( tcSvdb );
		// 数据转换
		tcSlice.ID = StringToUtf8( svdbTC.GetIDStr() );		// ID
		tcSlice.Description = StringToUtf8( svdbTC.GetDescriptionStr() );	// 描述

		// 创建时间		
		tcSlice.CreateTime = svdbTC.GetCreateTime().Format();

		// 属性
		StringMap &prop = svdbTC.GetProperty();
		StringMap2StringDict( prop, tcSlice.Property );

		// 边
		EDGELIST &edgs = svdbTC.GetEdges();
		EDGELISTSvdb2Slice( edgs, tcSlice.Edges );
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::TopologyChartSvdb2Slice - *exception*", 0, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::TopologyChartSvdb2Slice - *exception*" << endl;
	}

	cout << "****** DatHandleUtils::TopologyChartSvdb2Slice - all done!" << endl;

	return 1;
}

/////////////////////////////////////////////////////////////
// 将时间字符串转换成时间格式, 接受的格式: yyyy-mm-dd hh:mm:ss
bool DatHandleUtils::String2TTime( const ::std::string &strTime, TTime &tm )
{
	// cout << "Call: DatHandleUtils::String2TTime ( " << strTime << " ) ... ..." << endl;

	try
	{
		int nYear	= 2000;
		int nMonth	= 1;
		int nDay	= 1;
		int nHour	= 0;
		int nMin	= 0;
		int nSec	= 0;
		unsigned int nIdx = 0;
		unsigned int nLen = strTime.length();
		unsigned short nCount = 0;

		// 从字符串中解析时间数据	
		for( unsigned int i = 0; i < nLen; i++ )
		{
			if( strTime.at( i ) == '-' )
			{	// 日期分隔符(减号)
				nCount++;
				switch( nCount )
				{
				case 1:		// 年份分隔
					nYear = atoi( strTime.substr( nIdx, i - nIdx ).c_str() );
					break;
				case 2:		// 月份分隔
					nMonth = atoi( strTime.substr( nIdx, i - nIdx ).c_str() );
					break;
				}
				nIdx = i + 1;
			}
			else if( strTime.at( i ) == ' ' )
			{	// 空格分隔符(日期与时间分隔)
				nCount++;
				nDay = atoi( strTime.substr( nIdx, i - nIdx ).c_str() );
				nIdx = i + 1;			
			}
			else if( strTime.at( i ) == ':' )
			{	// 时间分隔符(空格)
				nCount++;
				switch( nCount )
				{
				case 4:		// 小时分隔
					nHour = atoi( strTime.substr( nIdx, i - nIdx ).c_str() );
					break;
				case 5:		// 分钟分隔
					nMin = atoi( strTime.substr( nIdx, i - nIdx ).c_str() );
					break;
				}
			}
		}

		// 最后一段为秒分隔
		nSec = atoi( strTime.substr( nIdx, nLen - nIdx - 1 ).c_str() );

		tm = TTime( nYear, nMonth, nDay, nHour, nMin, nSec);
	}
	catch( ... )
	{
		SvLog::Write( "DatHandleUtils::String2TTime - *exception*", 0, __FILE__, __LINE__ );
		cout << "###### DatHandleUtils::String2TTime - *exception*" << endl;
	}

	cout << "****** DatHandleUtils::String2TTime - all done!" << endl;

	return true;
}
//----------------------------------------------------------------------
