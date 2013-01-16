// **********************************************************************
// 文件: NnmMethodsI.cpp
// 描述: NnmMethodsI 类的定义
// 创建: Benny 2007.8.6
// 
// Copyright (c) 2003-2007 Dragonflow, Inc. All rights reserved.
//
// **********************************************************************
#include <afx.h>
#include <Windows.h>
#include <Ice/Ice.h>


#include "NnmMethodsI.h"
#include "DatHandleUtils.h"		// 数据格式转换方法
#include "ScanSvdbUtils.h"		// 与扫描相关的 SVDB 方法
#include "LogUtils.h"			// 日志实用函数

// 使用 svdb 中的函数和接口
#include "svapi.h"
#include "svdbapi.h"
#include "svnnmapi.h"


#include "SliceUtil.h"
#include "Des.h"
#include "VirtualGroup.h"
#include "TopologyChart.h"
#include "MonitorTemplet.h"
#include "Monitor.h"
#include "EntityTemplet.h"
#include "Entity.h"

#include "NNM2ECC.h"

using namespace NS_DatHandleUtils;
using namespace SV_LogUtils;

const unsigned int MQ_MSG_BUF_SIZE = 4096;	// 消息缓冲区大小

int g_snmpRetries = 1;
int g_snmpTimeout = 200;
int g_snmpVersion = 1;

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

///////////////////////////////////////////////////////////
//  接口使用方法：
//	获取 Port , EntityRelation , Edge 等请采用如下模式
//	EntityEx *pe=reinterpret_cast<EntityEx *>(nnmentityobj);
//  pe->各种方法();
//  by: 陈建堂(北京)
///////////////////////////////////////////////////////////

/********************************************************** 
 说明：在数据库的底层操作中已经对并发读/写进行了控制
       因此，在本接口类各种方法的实现中就不进行并发
       控制，以提高读写效率。
 
 注释：Benny(CS) 2007.8.7
**********************************************************/

///////////////////////////////////////////////////////////
// 构造与析构
NnmMethodsI::NnmMethodsI()
{
	m_sSvdbRootDir = "C:\\SiteView\\Core";	// 初始化根目录路径(默认路径)
	GetRootDirFromSysReg();					// 从注册表读根路径配置参数

	myWriteLog = new cWriteLog();
	myWriteLog->set_OnlyInDebug( false );	// 在release版本中也打出log信息
	

	////////////////////////////////////////////////////////////
	// 装载 readDeviceInfo 动态库
	m_hReadDeviceInfoDll = ::LoadLibrary( DLL_READDEVICEINFO.c_str() );
	if( NULL == m_hReadDeviceInfoDll )
	{
		DWORD dwErrCode = ::GetLastError();
		SvLog::Write( string( "Load DLL failed: " + DLL_READDEVICEINFO ).c_str(), 
					dwErrCode, __FILE__, __LINE__ );

		cout << "Load DLL failed: " << DLL_READDEVICEINFO;
		cout << ", GetLastError code: " << dwErrCode << endl;
		cout << DLL_READDEVICEINFO << endl;

		throw Exception("Init ReadDeviceInfoDll failed.");
	}
	m_TwriteMib	= (F_TWRITEMIB) ::GetProcAddress(m_hReadDeviceInfoDll,STR_TWRITEMIB.c_str());
	if( !m_TwriteMib )
	{
		DWORD dwErrCode = ::GetLastError();
		SvLog::Write( string( "Load DLL function failed: " + STR_TWRITEMIB ).c_str(), 
					dwErrCode, __FILE__, __LINE__ );
		cout << "Load TwriteMib failed.\n";
		throw Exception("Load TwriteMib failed.");
	}

	m_TstartGetDeviceInfo = (F_TSTARTGETDEVICEINFO) ::GetProcAddress(m_hReadDeviceInfoDll,STR_TSTARTGETDEVICEINFO.c_str());
	if( !m_TstartGetDeviceInfo )
	{
		DWORD dwErrCode = ::GetLastError();
		SvLog::Write( string( "Load DLL function failed: " + STR_TSTARTGETDEVICEINFO ).c_str(), 
					dwErrCode, __FILE__, __LINE__ );

		cout << "Load TstartGetDeviceInfo failed.\n";
		throw Exception("Load TstartGetDeviceInfo failed.");
	}
}

NnmMethodsI::~NnmMethodsI()
{
	if( m_hReadDeviceInfoDll )
	{
		::FreeLibrary( m_hReadDeviceInfoDll );
		m_hReadDeviceInfoDll = NULL;
	}
	if( NULL != myWriteLog )
	{
		delete myWriteLog;
	}
}

///////////////////////////////////////////////////////////
// 取得 SVDB 的根目录
bool NnmMethodsI::GetRootDirFromSysReg()
{
	bool bOk = false;

	// 从注册表中读出根目录路径
	HKEY hkResult = 0;
	LONG lRet = ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, 
								"SOFTWARE\\Siteview\\siteviewcore",		// 键路径
								0,
								KEY_READ, 
								&hkResult );

	if( ERROR_SUCCESS == lRet )
	{	// 打开键成功
		DWORD dwType;
		DWORD dwBufSize = 256;
		BYTE bufData[ 256 ];

		memset( bufData, 0x00, sizeof( bufData ) );

		lRet = ::RegQueryValueEx( hkResult, 
								"root_path_7",		// 键名
								NULL,
								&dwType,
								bufData,
								&dwBufSize );
		if(  ERROR_SUCCESS == lRet )
		{	// 取值成功
			if( REG_SZ == dwType )
			{	// 取得字符串类型数据
				m_sSvdbRootDir = (char *)bufData;
				bOk = true;
			}
		}

		::RegCloseKey( hkResult );
	}
	
	SvLog::Write( string( "NnmMethodsI::GetRootDirFromSysReg - Root dir: " + m_sSvdbRootDir ).c_str(), 
					0, __FILE__, __LINE__ );
	cout << "---- NnmMethodsI::GetRootDirFromSysReg - " << string( bOk ? "success!" : "*failed*" ) << endl;
	cout << "---- Root dir : " << m_sSvdbRootDir <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
// Entity 相关的操作方法
///////////////////////////////////////////////////////////
// 取得 Entity 对象
bool NnmMethodsI::GetEntity(const ::std::string& sEntId, 
							::SliceNNM::Entity& entity, 
							const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::GetEntity - id: " << Utf8ToString( sEntId ) << endl;

	// 1.从 svdb 中取数据	
	OBJECT entObj = ::GetNNMEntity( Utf8ToString( sEntId ) );
	if( entObj == NULL )
	{
		SvLog::Write( "NnmMethods::GetEntity - GetNNMEntity : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetEntity - GetNNMEntity : *failed*" << endl;

		return false;
	}
	EntityEx *pe = reinterpret_cast<EntityEx *>( entObj );
	if( NULL == pe )
	{
		SvLog::Write( "NnmMethods::GetEntity - reinterpret_cast : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetEntity - reinterpret_cast : *failed*" << endl;

		return false;
	}
	
	// 2.将取得的数据转换成 sliceNNM 格式
	entity.Identifier	= StringToUtf8( pe->GetIdentifier().getword() );
    entity.Description	= StringToUtf8( pe->GetDescription().getword() );
    entity.Type			= StringToUtf8( pe->GetType().getword() );
    entity.EccEntityID	= StringToUtf8( pe->GetEccEntityID().getword() );

	// 取得属性
	StringMap &prop = pe->GetProperty();	// 取得 entity 的属性
	ENTITYRELATIONLIST &entRelaList = pe->GetRelations();	// 取得 entity 的关系列表( Relation List )
	PORTMAP &portMap = pe->GetPorts();		// 取得 entity 的端口映射( Port map )
/*
	// 测试输出开始( 转换前的数据 svdb 原始数据 )
	SvLog::Write( "======================= Get entity from svdb ======", 0, __FILE__, __LINE__ );
	SvLog::Write( "entity.Identifier", 0, pe->GetIdentifier().getword(), __LINE__ );
	SvLog::Write( "entity.EccEntityID", 0, pe->GetEccEntityID().getword(), __LINE__ );
	SvLog::Write( "entity.Type", 0, pe->GetType().getword(), __LINE__ );
	SvLog::Write( "entity.Description", 0, pe->GetDescription().getword(), __LINE__ );

	cout << " SVDB ------------------------------------------------" << endl;
	cout << "ID: " << pe->GetIdentifier().getword() << endl;
	cout << "EccEntityID: " << pe->GetEccEntityID().getword() << endl;
	cout << "Type: " << pe->GetType().getword() << endl;
	cout << "Description: " << pe->GetDescription().getword() << endl;

	cout << "Properties : " << endl;
	SvLog::Write( "Properties : ", 0, __FILE__, __LINE__ );

	StringMap::iterator it;
	while( prop.findnext( it ) )	
	{
		cout << (*it).getkey().getword() << " = " << (*it).getvalue().getword() << endl;
		SvLog::Write( (*it).getkey().getword(), 0, (*it).getvalue().getword(), __LINE__ );
	}
	cout << " ------------------------------------------------" << endl;
	SvLog::Write( "======================= Get entity from svdb ======", 0, __FILE__, __LINE__ );

	// 测试输出结束
*/
	// 属性转换
	DatHandleUtils::StringMap2StringDict( prop, entity.Property );	
	DatHandleUtils::EntityRelationList2RelationDict( entRelaList, entity.Relations );	
	DatHandleUtils::PortMap2PortDict( portMap, entity.Ports );
/*
	// 测试输出开始( 转换后的数据 slice 返回数据 )
	SvLog::Write( "======================= Entity to slice ======", 0, __FILE__, __LINE__ );
	SvLog::Write( "entity.Identifier", 0, entity.Identifier.c_str(), __LINE__ );
	SvLog::Write( "entity.EccEntityID", 0, entity.EccEntityID.c_str(), __LINE__ );
	SvLog::Write( "entity.Type", 0, entity.Type.c_str(), __LINE__ );
	SvLog::Write( "entity.Description", 0, entity.Description.c_str(),  __LINE__ );

	cout << " Slice ------------------------------------------------" << endl;
	cout << "Identifier: " << entity.Identifier << endl;
	cout << "EccEntityID: " << entity.EccEntityID << endl;
	cout << "Type: " << entity.Type << endl;
	cout << "Description: " << entity.Description << endl;


	SvLog::Write( "Properties : ", 0, __FILE__, __LINE__ );
	cout << "Properties : " << endl;
	for( ::SliceNNM::StringDict::iterator it = entity.Property.begin();
		it != entity.Property.end(); it++ )	
	{
		cout << it->first << " = " << it->second << endl;
		SvLog::Write( it->first.c_str(), 0, it->second.c_str(), __LINE__ );
	}
	cout << " ------------------------------------------------" << endl;
	SvLog::Write( "======================= Entity to slice ======", 0, __FILE__, __LINE__ );
	// 测试输出结束
*/
	::CloseNNMEntity( entObj );	//删除对象，以免内存泄漏

	cout << "** NnmMethods::GetEntity - all done!" << endl;

	return true;
}
///////////////////////////////////////////////////////////
// 取得所有 Entity 对象列表
bool NnmMethodsI::GetAllEntityList(const ::std::string& sValue, 
								   ::SliceNNM::StringDict& idList, 
								   const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::GetAllEntityList ... ..." << endl;

	idList.clear();
	///////////////////////////////////////////////////
	// 从SVDB中取出所有的 Entity
	PAIRLIST lpRetPairList;
	bool bOk = ::GetAllNNMEntitysInfo( lpRetPairList );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::GetAllEntityList - GetAllNNMEntitysInfo : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetAllEntityList - GetAllNNMEntitysInfo : *false*" << endl;
		return false;
	}

	///////////////////////////////////////////////////
	// 将取得的数据转换成 slice 格式
	DatHandleUtils::PairList2StringDict( lpRetPairList, idList );

	cout << "** NnmMethods::GetAllEntityList - all done!" << endl;

	return true;
}

///////////////////////////////////////////////////////////
// 修改 Entity 对象
bool NnmMethodsI::SubmitEntity(const ::std::string& sEntId , 
							   const ::SliceNNM::Entity& entity, 
							   const ::Ice::Current& )
{
	cout << "==>> Invoke - NnmMethods::SubmitEntity (" << Utf8ToString( sEntId ) << ") ... ..." << endl;

	cout << "EntityID: " << sEntId << endl;
	cout << "entity.Identifier: " << entity.Identifier << endl;

	// 检查参数
	if( Utf8ToString( sEntId ).empty() )
	{
		cout << "*****　NnmMethods::SubmitEntity - Entity ID is empty! *failed*" <<endl;
		return false;
	}

	///////////////////////////////////////////////////
	// 1.用 sEntId 从 SVDB 中查询出目标 Entity
	OBJECT entObj = ::CreateNNMEntity( Utf8ToString( sEntId ) );
	if( entObj == NULL )
	{
		SvLog::Write( "NnmMethods::SubmitEntity - CreateNNMEntity : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::SubmitEntity - CreateNNMEntity : *false*" << endl;

		return false;
	}
	EntityEx *pe = reinterpret_cast<EntityEx *>( entObj );
	if( NULL == pe )
	{
		SvLog::Write( "NnmMethods::SubmitEntity - reinterpret_cast : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::SubmitEntity - reinterpret_cast : *failed*" << endl;

		return false;
	}

	// 测试输出开始( 接收的 slice 数据 )
	SvLog::Write( "======================= Entity from slice ======", 0, __FILE__, __LINE__ );
	SvLog::Write( "entity.Identifier", 0, entity.Identifier.c_str(), __LINE__ );
	SvLog::Write( "entity.EccEntityID", 0, entity.EccEntityID.c_str(), __LINE__ );
	SvLog::Write( "entity.Type", 0, entity.Type.c_str(), __LINE__ );
	SvLog::Write( "entity.Description", 0, entity.Description.c_str(),  __LINE__ );

	cout << " Slice ------------------------------------------------" << endl;
	cout << "Identifier: " << entity.Identifier << endl;
	cout << "EccEntityID: " << entity.EccEntityID << endl;
	cout << "Type: " << entity.Type << endl;
	cout << "Description: " << entity.Description << endl;


	SvLog::Write( "Properties : ", 0, __FILE__, __LINE__ );
	cout << "Properties : " << endl;
	::SliceNNM::Entity &ent = const_cast< ::SliceNNM::Entity &>( entity );
	for( ::SliceNNM::StringDict::iterator it = ent.Property.begin();
		it != ent.Property.end(); it++ )	
	{
		cout << it->first << " = " << it->second << endl;
		SvLog::Write( it->first.c_str(), 0, it->second.c_str(), __LINE__ );
	}
	cout << " ------------------------------------------------" << endl;
	SvLog::Write( "======================= Entity from slice ======", 0, __FILE__, __LINE__ );
	// 测试输出结束

	///////////////////////////////////////////////////
	// 2.设置目标 Entity 的属性
	pe->PutDescription( svutil::word( Utf8ToString( entity.Description ) ) );
	pe->PutEccEntityID( svutil::word( Utf8ToString( entity.EccEntityID ) ) );
	pe->PutIdentifier( svutil::word( Utf8ToString( entity.Identifier ) ) );
	pe->PutType( svutil::word( Utf8ToString( entity.Type ) ) );

	// 引用得到属性,端口映射,关系列表
	StringMap &prop		= pe->GetProperty();
	PORTMAP &portMap	= pe->GetPorts();
	ENTITYRELATIONLIST &relationList = pe->GetRelations();

	// 转换属性,端口映射,关系列表
	DatHandleUtils::StringDict2StringMap( entity.Property, prop );
	DatHandleUtils::RelationDict2EntityRelationList( entity.Relations, relationList );
	DatHandleUtils::PortDict2PortMap( entity.Ports, portMap );

	// 测试输出开始( 转换前的数据 svdb 原始数据 )
	SvLog::Write( "======================= Entity to svdb ======", 0, __FILE__, __LINE__ );
	SvLog::Write( "entity.Identifier", 0, pe->GetIdentifier().getword(), __LINE__ );
	SvLog::Write( "entity.EccEntityID", 0, pe->GetEccEntityID().getword(), __LINE__ );
	SvLog::Write( "entity.Type", 0, pe->GetType().getword(), __LINE__ );
	SvLog::Write( "entity.Description", 0, pe->GetDescription().getword(), __LINE__ );

	cout << " SVDB ------------------------------------------------" << endl;
	cout << "ID: " << pe->GetIdentifier().getword() << endl;
	cout << "EccEntityID: " << pe->GetEccEntityID().getword() << endl;
	cout << "Type: " << pe->GetType().getword() << endl;
	cout << "Description: " << pe->GetDescription().getword() << endl;

	cout << "Properties : " << endl;
	SvLog::Write( "Properties : ", 0, __FILE__, __LINE__ );

	StringMap::iterator it;
	while( prop.findnext( it ) )	
	{
		cout << (*it).getkey().getword() << " = " << (*it).getvalue().getword() << endl;
		SvLog::Write( (*it).getkey().getword(), 0, (*it).getvalue().getword(), __LINE__ );
	}
	cout << " ------------------------------------------------" << endl;
	SvLog::Write( "======================= Entity to svd ======", 0, __FILE__, __LINE__ );

	
	// 测试输出结束
	

	
	//struct Entity
    //::std::string Identifier;
    //::std::string Description;
    //::std::string Type;
    //::std::string EccEntityID;
    //::SliceNNM::StringDict Property;
    //::SliceNNM::RelationDict Relations;
    //::SliceNNM::PortDict Ports;	

	///////////////////////////////////////////////////
	// 3.将目标 Entity 更新到SVDB中
	svutil::word checkid= pe->GetIdentifier();
	cout<<checkid.getword()<<endl;
	if(checkid.empty())
	{
		//报错
		cout << "*****　NnmMethods::SubmitEntity - Check Identifier failed *failed*" <<endl;
	}

	bool bOk = ::SubmitNNMEntity( entObj );
	CNNM2ECC tnnm;
	tnnm.SubmitEntityExToEcc(pe);

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::SubmitNNMEntity - SubmitNNMEntity : *failed*", 0, __FILE__, __LINE__ );
	}

	// 测试输出开始
	cout << " After submit------------------------------------------------" << endl;
	cout << "ID: " << pe->GetIdentifier().getword() << endl;
	cout << "EccEntityID: " << pe->GetEccEntityID().getword() << endl;
	cout << "Type: " << pe->GetType().getword() << endl;
	cout << "Description: " << pe->GetDescription().getword() << endl;


	cout << "Properties : " << endl;
	StringMap::iterator it2;
	while( prop.findnext( it2 ) )	
	{
		cout << (*it2).getkey().getword() << " = " << (*it2).getvalue().getword() << endl;
	}
	cout << " ------------------------------------------------" << endl;
	// 测试输出结束

	try{
		cout << "---- NnmMethods::SubmitEntity - SubmitNNMEntity : " << string( bOk ? "success!" : "*failed*" ) <<endl;
		cout<< " ----- 828"<<endl;
		// 此处若调用SubmitTopologyChart失败会导致程序自动退出
		// 调用成功不会出错!
		// Benny 2007.8.26 注释下面一行代码进行调试

		::CloseNNMEntity( entObj );	//删除对象，以免内存泄漏

		DatHandleUtils::FreePortMap( portMap );		// 释放端口表中的端口对象内存
	}
	catch(...)
	{
		cout<<"Exeption to CloseNNMEntity"<<endl;
	}
	cout << "** NnmMethods::SubmitEntity - all done!" << endl;

	return bOk;
}

///////////////////////////////////////////////////////////
// 删除 Entity 对象
bool NnmMethodsI::DeleteEntity(const ::std::string& sEntId, 
							   const ::Ice::Current& )
{
	cout << "==>> Invoke - NnmMethods::DeleteEntity (" << Utf8ToString( sEntId ) << ") ... ..." << endl;

	bool bOk = ::DeleteNNMEntity( Utf8ToString( sEntId ) );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::DeleteEntity - DeleteNNMEntity : *failed*", 0, __FILE__, __LINE__ );
	}

	cout << "** NnmMethods::DeleteEntity - DeleteNNMEntity : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
// 查找 Entity 对象列表()
bool NnmMethodsI::FindEntityList(const ::std::string& sKey, 
								 const ::std::string& sValue, 
								 ::SliceNNM::StringLst& idList, 
								 const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::FindEntityList ( key: ";
	cout << Utf8ToString( sKey ) << ", value: ";
	cout << Utf8ToString( sValue ) << " ) ... ..." << endl;

	std::list< std::string > lsIdList;

	idList.clear();

	// 1.从 SVDB 中查询出idlist
	bool bOk = GetAllNNMEntitysInfoInWords( Utf8ToString( sKey ), 
											Utf8ToString( sValue ), 
											lsIdList );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::FindEntityList - GetAllNNMEntitysInfoInWords : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::FindEntityList - GetAllNNMEntitysInfoInWords : *false*" << endl;
		return false;
	}
	bOk = true;

	// 2.转换成 slice 格式
	try
	{
		for( std::list< std::string >::iterator it = lsIdList.begin(); 
			it != lsIdList.end(); it++ )
		{
			idList.push_back( StringToUtf8( *it ) );
		}
	}
	catch( ... )
	{
		bOk = false;
		
		SvLog::Write( "NnmMethods::GetAllNNMEntitysInfoInWords - *exception*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetAllNNMEntitysInfoInWords - *exception*" << endl;
	}

	cout << "** NnmMethods::SubmitEntity - all done!" << endl;

	return bOk;
}

///////////////////////////////////////////////////////////
// 查找 Entity 对象列表
bool NnmMethodsI::FindEntityListByProperty(const ::std::string& sKey, 
										   const ::std::string& sVal, 
										   ::SliceNNM::StringLst& idList, 
										   const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::FindEntityListByProperty ( key: ";
	cout << Utf8ToString( sKey ) << ", value: ";
	cout << Utf8ToString( sVal ) << " ) ... ..." << endl;

	std::list< std::string > lsIdList;

	idList.clear();

	// 1.从 SVDB 中查询出idlist
	bool bOk = GetAllNNMEntitysInfoInProperty( Utf8ToString( sKey ), 
												Utf8ToString( sVal ), 
												lsIdList );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::FindEntityListByProperty - GetAllNNMEntitysInfoInProperty : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::FindEntityListByProperty - GetAllNNMEntitysInfoInProperty : *false*" << endl;

		return false;
	}
	
	bOk = true;
	try
	{
		// 2.转换成 slice 格式
		for( std::list< std::string >::iterator it = lsIdList.begin(); 
			it != lsIdList.end(); it++ )
		{
			idList.push_back( StringToUtf8( *it ) );
		}
	}
	catch( ... )
	{
		bOk = false;

		SvLog::Write( "NnmMethods::FindEntityListByProperty - *exception*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::FindEntityListByProperty - *exception*" << endl;
	}

	cout << "** NnmMethods::FindEntityListByProperty - all done!" << endl;

	return bOk;
}

///////////////////////////////////////////////////////////
// VirtualGroup 相关的操作方法
bool NnmMethodsI::GetVirtualGroup(const ::std::string& sId, 
								  ::SliceNNM::VirtualGroup& vg, 
								  const ::Ice::Current& ) const
{
	// cout << "==>> Invoke - NnmMethods::GetVirtualGroup ( ";
	// cout << Utf8ToString( sId ) << " ) ... ..." << endl;

	// 1.从 svdb 中取数据
	OBJECT vgObj = ::GetVirtualGroup( Utf8ToString( sId ) );
	if( vgObj == NULL )
	{
		SvLog::Write( "NnmMethods::GetVirtualGroup - GetVirtualGroup : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetVirtualGroup - GetVirtualGroup : *false*" << endl;

		return false;
	}

	VirtualGroup *pVg = reinterpret_cast<VirtualGroup *>( vgObj );
	if( NULL == pVg )
	{
		SvLog::Write( "NnmMethods::GetVirtualGroup - reinterpret_cast : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetVirtualGroup - reinterpret_cast : *failed*" << endl;

		return false;
	}

	// 2.将 svdb 数据转换为 slice 格式
	vg.ID				= StringToUtf8( pVg->GetID().getword() );
	vg.Label			= StringToUtf8( pVg->GetLabel().getword() );
	vg.Description		= StringToUtf8( pVg->GetDescription().getword() );

	StringMap &prop		= pVg->GetProperty();
	WORDLIST &lstOfEnt	= pVg->GetEntityList();

	// typedef std::list<svutil::word>	WORDLIST;
	DatHandleUtils::WORDLIST2StringList( lstOfEnt, vg.EntityList );
	DatHandleUtils::StringMap2StringDict( prop, vg.Property );

	::CloseVirtualGroup( vgObj );	//删除对象，以免内存泄漏

	cout << "** NnmMethods::GetVirtualGroup - all done!" << endl;

	return true;

	//struct VirtualGroup
    //::std::string ID;
    //::std::string Label;
    //::std::string Description;
    //::SliceNNM::StringDict Property;
    //::SliceNNM::StringLst EntityList;	
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::GetAllVirtualGroupList(const ::std::string& sValue, 
										 ::SliceNNM::StringDict& idList, 
										 const ::Ice::Current& ) const
{
	// cout << "==>> Invoke - NnmMethods::GetAllVirtualGroupList ( ";
	// cout << Utf8ToString( sValue ) << " ) ... ..." << endl;

	PAIRLIST lpIdList;
	idList.clear();

	// 1.从 svdb 中取得 idlist
	bool bOk = ::GetAllVirtualGroupInfo( lpIdList );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::GetAllVirtualGroupList - GetAllVirtualGroupInfo : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetAllVirtualGroupList - GetAllVirtualGroupInfo : *false*" << endl;

		return false;
	}

	// 2.转换成 slice 格式
	DatHandleUtils::PairList2StringDict( lpIdList, idList );

	cout << "** NnmMethods::GetAllVirtualGroupList - all done!" << endl;

	return true;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::SubmitVirtualGroup(const ::std::string& sId, 
									 const ::SliceNNM::VirtualGroup& vg, 
									 const ::Ice::Current& )
{
	// cout << "==>> Invoke - NnmMethods::SubmitVirtualGroup ( ";
	// cout << Utf8ToString( sId ) << " ) ... ..." << endl;

	///////////////////////////////////////////////////
	// 1.用 sId 从 SVDB 中查询出目标 VirtualGroup
	OBJECT vgObj = ::CreateVirtualGroup( Utf8ToString( sId ) );
	if( vgObj == NULL )
	{
		SvLog::Write( "NnmMethods::SubmitVirtualGroup - CreateVirtualGroup : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::SubmitVirtualGroup - CreateVirtualGroup : *false*" << endl;

		return false;
	}
	VirtualGroup *pVg = reinterpret_cast<VirtualGroup *>( vgObj );
	if( NULL == pVg )
	{
		SvLog::Write( "NnmMethods::SubmitVirtualGroup - reinterpret_cast : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::SubmitVirtualGroup - reinterpret_cast : *failed*" << endl;

		return false;
	}

	///////////////////////////////////////////////////
	// 2.设置目标 Entity 的属性
	pVg->PutID( Utf8ToString( vg.ID ) );
	pVg->PutLabel( Utf8ToString( vg.Label ) );
	pVg->PutDescription( Utf8ToString( vg.Description ) );
	
	StringMap &prop		= pVg->GetProperty();
	WORDLIST &lstOfEnt	= pVg->GetEntityList();

	// 转换属性,端口映射,关系列表
	DatHandleUtils::StringDict2StringMap( vg.Property, prop );
	DatHandleUtils::StringList2WORDLIST( vg.EntityList, lstOfEnt );

	///////////////////////////////////////////////////
	// 3.将目标 Entity 更新到SVDB中
	bool bOk = ::SubmitVirtualGroup( vgObj );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::SubmitVirtualGroup - SubmitVirtualGroup : *failed*", 0, __FILE__, __LINE__ );
	}

	cout << "---- NnmMethods::SubmitVirtualGroup - SubmitVirtualGroup : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	::CloseVirtualGroup( vgObj );	// 删除对象，以免内存泄漏

	cout << "** NnmMethods::SubmitVirtualGroup - all done!" << endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::DeleteVirtualGroup(const ::std::string& sId, 
									 const ::Ice::Current& )
{
	// cout << "==>> Invoke - NnmMethods::DeleteVirtualGroup ( ";
	// cout << Utf8ToString( sId ) << " ) ... ..." << endl;

	bool bOk = ::DeleteVirtualGroup( Utf8ToString( sId ) );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::DeleteVirtualGroup - DeleteVirtualGroup : *failed*", 0, __FILE__, __LINE__ );
	}

	cout << "** NnmMethods::SubmitVirtualGroup - all done : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::FindVirtualGroupList(const ::std::string& sKey, 
									   const ::std::string& sValue, 
									   ::SliceNNM::StringLst& idlist, 
									   const ::Ice::Current& ) const
{
	// cout << "==>> Invoke - NnmMethods::FindVirtualGroupList ( key: ";
	// cout << Utf8ToString( sKey ) << ", value: ";
	// cout << Utf8ToString( sValue ) << " ) ... ..." << endl;

	std::list<string> lsIdList;

	// 宴始化输出参数
	idlist.clear();

	// 从 svdb 中取得数据
	bool bOk = ::GetAllVirtualGroupInfoInWords( Utf8ToString( sKey ),
											Utf8ToString( sValue ),
											lsIdList );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::FindVirtualGroupList - GetAllVirtualGroupInfoInWords : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::FindVirtualGroupList - GetAllVirtualGroupInfoInWords : *false*" << endl;

		return false;
	}

	bOk = true;
	try
	{
		// 转换成 slice 格式
		for( std::list<string>::iterator it = lsIdList.begin();
			it != lsIdList.end(); it++ )
		{
			idlist.push_back( ( *it ) );
		}
	}
	catch( ... )
	{
		bOk = false;

		SvLog::Write( "NnmMethods::FindVirtualGroupList - *exception*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::FindVirtualGroupList - *exception*" << endl;
	}

	cout << "** NnmMethods::FindVirtualGroupList - all done!" << endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::FindVirtualGroupListByProperty(const ::std::string& sKey, 
												 const ::std::string& sValue, 
												 ::SliceNNM::StringLst& idlist, 
												 const ::Ice::Current& ) const
{
	// cout << "==>> Invoke - NnmMethods::FindVirtualGroupList ( key: ";
	// cout << Utf8ToString( sKey ) << ", value: ";
	// cout << Utf8ToString( sValue ) << " ) ... ..." << endl;

	std::list<string> lsIdList;

	// 宴始化输出参数
	idlist.clear();

	// 从 svdb 中取得数据
	bool bOk = ::GetAllVirtualGroupInfoInProperty( Utf8ToString( sKey ),
											Utf8ToString( sValue ),
											lsIdList );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::FindVirtualGroupListByProperty - GetAllVirtualGroupInfoInProperty : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::FindVirtualGroupListByProperty - GetAllVirtualGroupInfoInProperty : *false*" << endl;
		
		return false;
	}

	bOk = true;
	try
	{
		// 转换成 slice 格式
		for( std::list<string>::iterator it = lsIdList.begin();
			it != lsIdList.end(); it++ )
		{
			idlist.push_back( ( *it ) );
		}
	}
	catch( ... )
	{
		bOk = false;
		SvLog::Write( "NnmMethods::FindVirtualGroupListByProperty - *exception*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::FindVirtualGroupList - *exception*" << endl;
	}

	cout << "** NnmMethods::FindVirtualGroupList - all done!" << endl;

	return true;
}

///////////////////////////////////////////////////////////
// TopologyChart 相关的操作方法
bool NnmMethodsI::GetTopologyChart(const ::std::string& sId, 
								   ::SliceNNM::TopologyChart& tc, 
								   const ::Ice::Current& ) const
{
	// cout << "==>> Invoke - NnmMethods::GetTopologyChart ( ";
	// cout << Utf8ToString( sId ) << " ) ... ..." << endl;

	/////////////////////////////////////////////////////////
	// 从SVDB中取得数据对象
	OBJECT tcObj = ::GetTopologyChart( Utf8ToString( sId ) );
	if( tcObj == NULL )
	{
		SvLog::Write( "NnmMethods::GetTopologyChart - GetTopologyChart : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetTopologyChart - GetTopologyChart : *false*" << endl;

		return false;
	}
	TopologyChart *pTc = reinterpret_cast<TopologyChart *>( tcObj );
	if( NULL == pTc )
	{
		SvLog::Write( "NnmMethods::GetTopologyChart - reinterpret_cast : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetTopologyChart - reinterpret_cast : *failed*" << endl;

		return false;
	}

	/////////////////////////////////////////////////////////
	// 将数据对象转换成slice数据格式
	tc.ID			= StringToUtf8( pTc->GetID().getword() );
	tc.Description	= StringToUtf8( pTc->GetDescription().getword() );
	tc.CreateTime	= StringToUtf8( pTc->GetCreateTime().Format() );

	EDGELIST &lstEdg	= pTc->GetEdges();
	StringMap &prop		= pTc->GetProperty();

	DatHandleUtils::StringMap2StringDict( prop, tc.Property );
	DatHandleUtils::EDGELISTSvdb2Slice( lstEdg, tc.Edges );

	::CloseTopologyChart( tcObj );	// 删除对象，以免内存泄漏

	cout << "** NnmMethods::GetTopologyChart - all done!" << endl;

	return true;

	//struct TopologyChart
 //   ::std::string ID;
 //   ::std::string CreateTime;		// yyyy-mm-dd hh:nn:ss
 //   ::std::string Description;
 //   ::SliceNNM::StringDict Property;
 //   ::SliceNNM::EDGELIST Edges;

	//struct Edge
 //   ::std::string LeftPortID;
 //   ::std::string RightPortID;
 //   ::std::string DependMonitorID;
 //   ::SliceNNM::StringDict Property;

	//typedef ::std::vector< ::SliceNNM::Edge> EDGELIST;

}

///////////////////////////////////////////////////////////
bool NnmMethodsI::GetTopologyChartList(const ::std::string& sValue, 
									   ::SliceNNM::StringDict& idList, 
									   const ::Ice::Current& ) const
{
	// cout << "==>> Invoke - NnmMethods::GetTopologyChartList ( ";
	// cout << Utf8ToString( sValue ) << " ) ... ..." << endl;

	/////////////////////////////////////////////////////////
	// 从 svdb 中找到对象
	PAIRLIST lpRet;
	bool bOk = ::GetAllTopologyChartInfo( lpRet );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::GetTopologyChartList - GetAllTopologyChartInfo : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetTopologyChartList - GetAllTopologyChartInfo : *false*" << endl;

		return false;
	}

	/////////////////////////////////////////////////////////
	// 转换成 slice 格式
	DatHandleUtils::PairList2StringDict( lpRet, idList );

	cout << "** NnmMethods::GetTopologyChartList - all done!" << endl;

	return true;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::SubmitTopologyChart(const ::std::string& sId, 
									  const ::SliceNNM::TopologyChart& tc, 
									  const ::Ice::Current& )
{
	cout << "==>> Invoke - NnmMethods::SubmitTopologyChart ( ";
	cout << Utf8ToString( sId ) << " ) ... ..." << endl;

	///////////////////////////////////////////////////
	// 1.用 sId 从 SVDB 中查询出目标 TopologyChart
	OBJECT tcObj = ::CreateTopologyChart( Utf8ToString( sId ) );
	if( tcObj == NULL )
	{
		SvLog::Write( "NnmMethods::SubmitTopologyChart - CreateTopologyChart : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::SubmitTopologyChart - CreateTopologyChart : *false*" << endl;

		return false;
	}
	TopologyChart *pTc = reinterpret_cast<TopologyChart *>( tcObj );
	if( NULL == pTc )
	{
		SvLog::Write( "NnmMethods::SubmitTopologyChart - reinterpret_cast : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::SubmitTopologyChart - reinterpret_cast : *failed*" << endl;

		return false;
	}

	///////////////////////////////////////////////////
	// 2.设置目标 Entity 的属性
	pTc->PutID( Utf8ToString( tc.ID ) );
	pTc->PutDescription( Utf8ToString( tc.Description ) );

	svutil::TTime tmCreate;
	DatHandleUtils::String2TTime( tc.CreateTime, tmCreate );
	pTc->PutCreateTime( tmCreate );

	EDGELIST &lstEdg	= pTc->GetEdges();
	StringMap &prop		= pTc->GetProperty();

	DatHandleUtils::StringDict2StringMap( tc.Property, prop );
	DatHandleUtils::EDGELISTSlice2Svdb( tc.Edges, lstEdg  );

	///////////////////////////////////////////////////
	// 3.将目标 Entity 更新到SVDB中
	bool bOk = ::SubmitTopologyChart( tcObj );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::SubmitTopologyChart - SubmitTopologyChart : *failed*", 0, __FILE__, __LINE__ );
	}

	cout << "---- NnmMethods::SubmitTopologyChart - SubmitTopologyChart : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	// 此处若调用SubmitTopologyChart失败会导致程序自动退出
	// 调用成功不会出错!
	// Benny 2007.8.26 注释下面一行代码进行调试
	// ::CloseTopologyChart( tcObj );//删除对象，以免内存泄漏

	cout << "** NnmMethods::SubmitTopologyChart - all done!" << endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::DeleteTopologyChart(const ::std::string& sId, 
									  const ::Ice::Current& )
{
	cout << "==>> Invoke - NnmMethods::DeleteTopologyChart ( ";
	cout << Utf8ToString( sId ) << " ) ... ..." << endl;

	bool bOk = ::DeleteTopologyChart( Utf8ToString( ( sId ) ) );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::DeleteTopologyChart - DeleteTopologyChart : *failed*", 0, __FILE__, __LINE__ );
	}

	cout << "** NnmMethods::DeleteTopologyChart - DeleteTopologyChart : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::FindTopologyChartList(const ::std::string& sKey, 
										const ::std::string& sValue, 
										::SliceNNM::StringLst& idList, 
										const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::FindTopologyChartList ( key: ";
	cout << Utf8ToString( sKey ) << ", value: ";
	cout << Utf8ToString( sValue ) << " ) ... ..." << endl;

	std::list<string> lsIdList;

	// 宴始化输出参数
	idList.clear();

	// 从 svdb 中取得数据
	bool bOk = ::GetAllTopologyChartInfoInWords( Utf8ToString( sKey ),
											Utf8ToString( sValue ),
											lsIdList );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::FindTopologyChartList - GetAllTopologyChartInfoInWords : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::FindTopologyChartList - GetAllTopologyChartInfoInWords : *false*" << endl;

		return false;
	}

	bOk = true;

	try
	{
		// 转换成 slice 格式
		for( std::list<string>::iterator it = lsIdList.begin();
			it != lsIdList.end(); it++ )
		{
			idList.push_back( ( *it ) );
		}
	}
	catch( ... )
	{
		bOk = false;
		SvLog::Write( "NnmMethods::FindTopologyChartList - GetAllTopologyChartInfoInWords : *exception*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::FindTopologyChartList - *exception*" << endl;
	}

	cout << "** NnmMethods::FindTopologyChartList - all done!" << endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::FindTopologyChartListByProperty(const ::std::string& sKey, 
												  const ::std::string& sValue, 
												  ::SliceNNM::StringLst& idList, 
												  const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::FindTopologyChartListByProperty ( key: ";
	cout << Utf8ToString( sKey ) << ", value: ";
	cout << Utf8ToString( sValue ) << " ) ... ..." << endl;

	std::list<string> lsIdList;

	// 宴始化输出参数
	idList.clear();

	// 从 svdb 中取得数据
	bool bOk = ::GetAllTopologyChartInfoInProperty( Utf8ToString( sKey ),
											Utf8ToString( sValue ),
											lsIdList );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::FindTopologyChartListByProperty - GetAllTopologyChartInfoInProperty : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::FindTopologyChartListByProperty - GetAllTopologyChartInfoInProperty : *false*" << endl;

		return false;
	}

	bOk = true;

	try
	{
		// 转换成 slice 格式
		for( std::list<string>::iterator it = lsIdList.begin();
			it != lsIdList.end(); it++ )
		{
			idList.push_back( ( *it ) );
		}
	}
	catch( ... )
	{
		bOk = false;
		SvLog::Write( "NnmMethods::FindTopologyChartListByProperty - GetAllTopologyChartInfoInProperty : *exception*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::FindTopologyChartListByProperty - *exception*" << endl;
	}

	cout << "** NnmMethods::FindTopologyChartListByProperty - all done!" << endl;

	return bOk;
}

///////////////////////////////////////////////////////////
// EntityTemplet 相关的操作方法
bool NnmMethodsI::GetEntityTemplet(const ::std::string& sId, 
								   ::SliceNNM::EntityTemplet& et, 
								   const ::Ice::Current& ) const
{
	// cout << "==>> Invoke - NnmMethods::GetEntityTemplet ( ";
	// cout << Utf8ToString( sId ) << " ) ... ..." << endl;

	/////////////////////////////////////////////////////////
	// 从SVDB中取得数据对象
	OBJECT etObj = ::GetEntityTemplet( Utf8ToString( sId ) );
	if( etObj == NULL )
	{
		SvLog::Write( "NnmMethods::GetEntityTemplet - GetAllTopologyChartInfoInProperty : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetEntityTemplet - GetEntityTemplet : *failed*" << endl;

		return false;
	}
	EntityTemplet *pEt = reinterpret_cast<EntityTemplet *>( etObj );
	if( NULL == pEt )
	{
		SvLog::Write( "NnmMethods::GetEntityTemplet - reinterpret_cast : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetEntityTemplet - reinterpret_cast : *failed*" << endl;

		return false;
	}

	/////////////////////////////////////////////////////////
	// 将数据对象转换成slice数据格式
	et.ID = StringToUtf8( pEt->GetID().getword() );

	StringMap &prop = pEt->GetProperty();
	STRMAPLIST &ctrlList = pEt->GetContrlList();
	std::list<int> & monKinds = pEt->GetMonitorKinds();

	DatHandleUtils::StringMap2StringDict( prop, et.Property );
	DatHandleUtils::STRINGMAPLISTSvdb2Slice( ctrlList, et.ContrlList );

	bool bOk = true;

	try
	{
		et.MonitorKinds.clear();
		for( std::list<int>::iterator it = monKinds.begin(); 
			it != monKinds.end(); it++ )
		{
			et.MonitorKinds.push_back( *it );
		}
	}
	catch( ... )
	{
		bOk = false;
		SvLog::Write( "NnmMethods::GetEntityTemplet - reinterpret_cast : *exception*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetEntityTemplet - *exception*" << endl;
	}
	
	::CloseEntityGroup( etObj );//删除对象，以免内存泄漏

	cout << "** NnmMethods::GetEntityTemplet - all done!" << endl;

	return bOk;


	//struct EntityTemplet
 //   ::std::string ID;
 //   ::SliceNNM::StringDict Property;
 //   ::SliceNNM::STRINGMAPLIST ContrlList;
 //   ::SliceNNM::INTLIST MonitorKinds;

	//typedef ::std::vector< ::Ice::Int> INTLIST;

}

///////////////////////////////////////////////////////////
bool NnmMethodsI::GetEntityTempletList(const ::std::string& sValue, 
									   ::SliceNNM::StringDict& idList, 
									   const ::Ice::Current& ) const
{
	// cout << "==>> Invoke - NnmMethods::GetEntityTempletList ( ";
	// cout << Utf8ToString( sValue ) << " ) ... ..." << endl;

	PAIRLIST retlist;

	idList.clear();		// 初始化输出参数

	// 从 svdb 中获取数据
	bool bOk = ::GetAllEntityTemplets( retlist, 
									Utf8ToString( sValue ) );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::GetEntityTempletList - GetAllEntityTemplets : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetEntityTempletList - GetAllEntityTemplets : *false*" << endl;

		return false;
	}

	// 数据格式转换 svdb => slice
	DatHandleUtils::PairList2StringDict( retlist, idList );

	cout << "** NnmMethods::GetEntityTempletList - all done!" << endl;

	return true;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::SubmitEntityTemplet(const ::std::string& sId, 
									  const ::SliceNNM::EntityTemplet& et, 
									  const ::Ice::Current& )
{
	// cout << "==>> Invoke - NnmMethods::SubmitEntityTemplet ( ";
	// cout << Utf8ToString( sId ) << " ) ... ..." << endl;

	/////////////////////////////////////////////////////////
	// 从SVDB中取得数据对象
	OBJECT etObj = ::CreateEntityTemplet( Utf8ToString( sId ) );
	if( etObj == NULL )
	{
		SvLog::Write( "NnmMethods::SubmitEntityTemplet - CreateEntityTemplet : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::SubmitEntityTemplet - CreateEntityTemplet : *failed*" << endl;

		return false;
	}
	EntityTemplet *pEt = reinterpret_cast<EntityTemplet *>( etObj );
	if( NULL == pEt )
	{
		SvLog::Write( "NnmMethods::SubmitEntityTemplet - reinterpret_cast : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::SubmitEntityTemplet - reinterpret_cast : *failed*" << endl;

		return false;
	}

	/////////////////////////////////////////////////////////
	// 将数据对象转换成 SVDB 数据格式
	pEt->PutID( Utf8ToString( et.ID ) );

	StringMap &prop = pEt->GetProperty();
	STRMAPLIST &ctrlList = pEt->GetContrlList();
	std::list<int> & monKinds = pEt->GetMonitorKinds();

	DatHandleUtils::StringDict2StringMap( et.Property, prop );
	DatHandleUtils::STRINGMAPLISTSlice2Svdb( et.ContrlList, ctrlList );

	bool bOk = true;
	try
	{
		monKinds.clear();
		::SliceNNM::INTLIST &liEtMonList = const_cast< ::SliceNNM::INTLIST & >( et.MonitorKinds );
		for( ::SliceNNM::INTLIST::iterator it = liEtMonList.begin(); 
			it != liEtMonList.end(); it++ )
		{
			monKinds.push_back( *it );
		}
	}
	catch( ... )
	{
		bOk = false;
		SvLog::Write( "NnmMethods::SubmitEntityTemplet - reinterpret_cast : Exception occurred", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::SubmitEntityTemplet - *exception*" << endl;
	}

	if( bOk )
	{
		bOk = ::SubmitEntityTemplet( etObj );

		if( !bOk )
		{
			SvLog::Write( "NnmMethods::SubmitEntityTemplet - SubmitEntityTemplet : *failed*", 0, __FILE__, __LINE__ );
		}

		cout << "#### NnmMethods::SubmitEntityTemplet - SubmitEntityTemplet : " << string( bOk ? "success!" : "*failed*" ) <<endl;
	}

	::CloseEntityTemplet( etObj );	// 删除对象，以免内存泄漏

	cout << "** NnmMethods::SubmitEntityTemplet - all done!" << endl;

	return bOk;
	
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::DeleteEntityTemplet(const ::std::string& sId, 
									  const ::Ice::Current& )
{
	// cout << "==>> Invoke - NnmMethods::DeleteEntityTemplet ( ";
	// cout << Utf8ToString( sId ) << " ) ... ..." << endl;

	bool bOk = ::DeleteEntityTemplet( sId );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::DeleteEntityTemplet - DeleteEntityTemplet : *failed*", 0, __FILE__, __LINE__ );
	}

	cout << "** NnmMethods::DeleteEntityTemplet - DeleteEntityTemplet : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::FindEntityTempletList(const ::std::string& sKey, 
										const ::std::string& sValue, 
										::SliceNNM::StringLst& idlist, 
										const ::Ice::Current& ) const
{
	// cout << "==>> Invoke - NnmMethods::FindEntityTempletList ( key: ";
	// cout << Utf8ToString( sKey ) << ", value: ";
	// cout << Utf8ToString( sValue ) << " ) ... ..." << endl;

	std::list< std::string > lsIdList;

	idlist.clear();
	bool bOk = ::GetAllEntityTempletsInProperty( Utf8ToString( sKey ),
											Utf8ToString( sValue ),
											lsIdList );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::FindEntityTempletList - GetAllEntityTempletsInProperty : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::FindEntityTempletList - GetAllEntityTempletsInProperty : *false*" << endl;

		return false;
	}

	bOk = true;

	try
	{
		for( std::list< std::string >::iterator it = lsIdList.begin();
			it != lsIdList.end(); it++ )
		{
			idlist.push_back( *it );
		}
	}
	catch( ... )
	{
		bOk = false;

		SvLog::Write( "NnmMethods::FindEntityTempletList - *exception*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::FindEntityTempletList - *exception*" << endl;
	}

	cout << "** NnmMethods::FindEntityTempletList - all done!" << endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::FindEntityTempletListByProperty(const ::std::string& sKey, 
												  const ::std::string& sValue, 
												  ::SliceNNM::StringLst& idList, 
												  const ::Ice::Current& ) const
{
	// cout << "==>> Invoke - NnmMethods::FindEntityTempletListByProperty ( key: ";
	// cout << Utf8ToString( sKey ) << ", value: ";
	// cout << Utf8ToString( sValue ) << " ) ... ..." << endl;

	bool bOk = true;
	try
	{
		std::list< string > slId;

		idList.clear();
		bOk = ::GetAllEntityTempletsInProperty( Utf8ToString( sKey ), 
													Utf8ToString( sValue ), 
													slId );
		if( !bOk )
		{
			SvLog::Write( "NnmMethods::FindEntityTempletListByProperty - GetAllEntityTempletsInProperty : *failed*", 0, __FILE__, __LINE__ );
			cout << "#### NnmMethods::FindEntityTempletListByProperty - GetAllEntityTempletsInProperty : *false*" << endl;

			return false;
		}	
	
		for( std::list< string >::iterator it = slId.begin(); 
			it != slId.end(); it++ )
		{
			idList.push_back( StringToUtf8( *it ) );
		}
	}
	catch( ... )
	{
		bOk = false;

		SvLog::Write( "NnmMethods::FindEntityTempletListByProperty - *exception*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::FindEntityTempletListByProperty - *exception*" << endl;
	}

	cout << "** NnmMethods::FindEntityTempletListByProperty - all done!" << endl;

	return bOk;

	//slice data struct
	//typedef ::std::vector< ::std::string> StringLst;
}


///////////////////////////////////////////////////////////
// Ini 配置文件相关的操作方法
///////////////////////////////////////////////////////////
::Ice::Int NnmMethodsI::GetIniFileInt(const ::std::string& sSection, 
									  const ::std::string& sKey, 
									  ::Ice::Int nDefault, 
									  const ::std::string& sFileName, 
									  const ::Ice::Current& ) const
{
	// cout << "==>> Invoke - NnmMethods::GetIniFileInt ( file: " << Utf8ToString( sFileName );
	// cout << ", section: " << Utf8ToString( sSection ) << ", key: ";
	// cout << Utf8ToString( sKey ) << " ) ... ..." << endl;

	bool bOk = ::GetIniFileInt( Utf8ToString( sSection ), 
								Utf8ToString( sKey ), 
								nDefault, 
								Utf8ToString( sFileName ) );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::GetIniFileInt - GetIniFileInt : *failed*", 0, __FILE__, __LINE__ );
	}
	// cout << "** NnmMethods::GetIniFileInt - GetIniFileInt : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::WriteIniFileInt(const ::std::string& sSection, 
								  const ::std::string& sKey, 
								  ::Ice::Int nValue, 
								  const ::std::string& sFileName, 
								  const ::Ice::Current& )
{
	// cout << "==>> Invoke - NnmMethods::WriteIniFileInt ( file: " << Utf8ToString( sFileName );
	// cout << ", section: " << Utf8ToString( sSection );
	// cout << ", key: " << Utf8ToString( sKey ) << " ) ... ..." << endl;

	bool bOk = ::WriteIniFileInt( Utf8ToString( sSection ), 
								Utf8ToString( sKey ), 
								nValue, 
								Utf8ToString( sFileName ) );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::WriteIniFileInt - WriteIniFileInt : *failed*", 0, __FILE__, __LINE__ );
	}

	// cout << "** NnmMethods::GetIniFileInt - WriteIniFileInt : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
::std::string NnmMethodsI::GetIniFileString(const ::std::string& sSection, 
											const ::std::string& sKey, 
											const ::std::string& sDefault,
											const ::std::string& sFileName, 
											const ::Ice::Current& ) const
{
	// cout << "==>> Invoke - NnmMethods::GetIniFileString ( file: " << Utf8ToString( sFileName );
	// cout << ", section: " << Utf8ToString( sSection ) << ", key: " << Utf8ToString( sKey ) << " ) ... ..." << endl;

	string sRet = StringToUtf8( ::GetIniFileString( Utf8ToString( sSection ), 
											Utf8ToString( sKey ), 
											Utf8ToString( sDefault ), 
											Utf8ToString( sFileName ) ) );

	// cout << "** NnmMethods::GetIniFileString - GetIniFileString : " << StringToUtf8( sRet ) <<endl;

	return StringToUtf8( sRet );
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::WriteIniFileString(const ::std::string& sSection, 
									 const ::std::string& sKey, 
									 const ::std::string& sValue, 
									 const ::std::string& sFileName, 
									 const ::Ice::Current& )
{
	// cout << "==>> Invoke - NnmMethods::GetIniFileString ( file: " << Utf8ToString( sFileName );
	// cout << ", section: " << Utf8ToString( sSection ) << ", key: " << Utf8ToString( sKey ) << " ) ... ..." << endl;

	bool bOk =  ::WriteIniFileString( Utf8ToString( sSection ), 
										Utf8ToString( sKey ), 
										Utf8ToString( sValue ), 
										Utf8ToString( sFileName ) );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::WriteIniFileString - WriteIniFileString : *failed*", 0, __FILE__, __LINE__ );
	}
	// cout << "** NnmMethods::WriteIniFileString - WriteIniFileString : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::DeleteIniFileSection(const ::std::string& sSection, 
									   const ::std::string& sFileName, 
									   const ::Ice::Current& )
{
	// cout << "==>> Invoke - NnmMethods::DeleteIniFileSection ( file: ";
	// cout << Utf8ToString( sFileName ) << ", section: " << Utf8ToString( sSection ) << " ) ... ..." << endl;

	bool bOk =  ::DeleteIniFileSection( Utf8ToString( sSection ), 
										Utf8ToString( sFileName ) );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::DeleteIniFileSection - DeleteIniFileSection : *failed*", 0, __FILE__, __LINE__ );
	}
	// cout << "** NnmMethods::DeleteIniFileSection - DeleteIniFileSection : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::DeleteIniFileKey(const ::std::string& sSection, 
								   const ::std::string& sKey, 
								   const ::std::string& sFileName, 
								   const ::Ice::Current& )
{
	// cout << "==>> Invoke - NnmMethods::DeleteIniFileKey ( file: " << Utf8ToString( sFileName );
	// cout << ", section: " << Utf8ToString( sSection ) << ", key: " << Utf8ToString( sKey ) << " ) ... ..." << endl;

	bool bOk = ::DeleteIniFileKey( Utf8ToString( sSection ), 
									Utf8ToString( sKey ), 
									Utf8ToString( sFileName ) );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::DeleteIniFileKey - DeleteIniFileKey : *failed*", 0, __FILE__, __LINE__ );
	}
	// cout << "** NnmMethods::DeleteIniFileKey - DeleteIniFileKey : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::GetIniFileSections(const ::std::string& sFileName, 
									 ::SliceNNM::StringLst& sectionList, 
									 const ::Ice::Current& ) const
{
	// cout << "==>> Invoke - NnmMethods::GetIniFileSections ( file: ";
	// cout << Utf8ToString( sFileName ) << " ) ... ..." << endl;

	std::list<string> slSections;
	sectionList.clear();

	bool bOk = ::GetIniFileSections( slSections, Utf8ToString( sFileName ) );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::GetIniFileSections - GetIniFileSections : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetIniFileSections - GetIniFileSections : *failed*" << endl;
	
		return false;
	}
	
	try
	{
		for( std::list<string>::iterator it = slSections.begin(); 
			it != slSections.end() && bOk; it++ )
		{
			sectionList.push_back( StringToUtf8( *it ) );
		}
	}
	catch( ... )
	{
		bOk = false;
		SvLog::Write( "NnmMethods::GetIniFileSections - GetIniFileSections : *exception*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetIniFileSections - *exception*" << endl;
	}

	cout << "** NnmMethods::GetIniFileSections - GetIniFileSections : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::GetIniFileKeys(const ::std::string& sSection, 
								 const ::std::string& sFileName, 
								 ::SliceNNM::StringLst& keyList, 
								 const ::Ice::Current& ) const
{
	// cout << "==>> Invoke - NnmMethods::GetIniFileKeys ( file: ";
	// cout << Utf8ToString( sFileName ) << ", section: ";
	// cout << Utf8ToString( sSection ) << " ) ... ..." << endl;

	std::list<string> slKeys;
	keyList.clear();

	bool bOk = ::GetIniFileKeys( Utf8ToString( sSection ), 
								slKeys, 
								Utf8ToString( sFileName ) );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::GetIniFileKeys - GetIniFileKeys : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetIniFileKeys - GetIniFileKeys : *failed*" << endl;
	
		return false;
	}

	try
	{
		for( std::list<string>::iterator it = slKeys.begin(); 
			it != slKeys.end() && bOk; it++ )
		{
			keyList.push_back( StringToUtf8( *it ) );
		}
	}
	catch( ... )
	{
		bOk = false;
		SvLog::Write( "NnmMethods::GetIniFileKeys - GetIniFileKeys : *exception*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetIniFileSections - *exception*" << endl;
	}
	
	cout << "** NnmMethods::GetIniFileKeys - GetIniFileKeys : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
::Ice::Int NnmMethodsI::GetIniFileValueType(const ::std::string& sSection, 
											const ::std::string& sKey, 
											const ::std::string& sFileName, 
											const ::Ice::Current& ) const
{
	// cout << "==>> Invoke - NnmMethods::GetIniFileValueType ( file: " << Utf8ToString( sFileName );
	// cout << ", section: " << Utf8ToString( sSection ) << ", key: ";
	// cout << Utf8ToString( sKey ) << " ) ... ..." << endl;

	bool bOk = ::GetIniFileValueType( Utf8ToString( sSection ), 
								Utf8ToString( sKey ), 
								Utf8ToString( sFileName ) );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::GetIniFileValueType - GetIniFileValueType : *failed*", 0, __FILE__, __LINE__ );
	}
	cout << "** NnmMethods::GetIniFileValueType - GetIniFileValueType : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::EditIniFileSection(const ::std::string& sOldSection, 
									 const ::std::string& sNewSection, 
									 const ::std::string& sFileName, 
									 const ::Ice::Current& )
{
	// cout << "==>> Invoke - NnmMethods::EditIniFileSection ( file: " << Utf8ToString( sFileName );
	// cout << ", section old: " << Utf8ToString( sOldSection ) << ", section new: ";
	// cout << Utf8ToString( sNewSection ) << " ) ... ..." << endl;


	bool bOk = ::EditIniFileSection( Utf8ToString( sOldSection ), 
								Utf8ToString( sNewSection ), 
								Utf8ToString( sFileName ) );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::EditIniFileSection - EditIniFileSection : *failed*", 0, __FILE__, __LINE__ );
	}
	cout << "** NnmMethods::EditIniFileSection - EditIniFileSection : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::EditIniFileKey(const ::std::string& sSection, 
								 const ::std::string& sOldKey, 
								 const ::std::string& sNewKey, 
								 const ::std::string& sFileName, 
								 const ::Ice::Current& )
{
	// cout << "==>> Invoke - NnmMethods::EditIniFileKey ( file: " << Utf8ToString( sFileName );
	// cout << ", section: " << Utf8ToString( sSection ) << ", key: ";
	// cout << Utf8ToString( sOldKey ) << " | " << Utf8ToString( sNewKey );
	// cout << " ) ... ..." << endl;

	bool bOk = ::EditIniFileKey( Utf8ToString( sSection ), 
								Utf8ToString( sOldKey ), 
								Utf8ToString( sNewKey ), 
								Utf8ToString( sFileName ) );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::EditIniFileKey - EditIniFileKey : *failed*", 0, __FILE__, __LINE__ );
	}
	cout << "** NnmMethods::EditIniFileKey - EditIniFileKey : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}


///////////////////////////////////////////////////////////
// MQ 相关的操作方法
///////////////////////////////////////////////////////////
::Ice::Int NnmMethodsI::CreateQueue(const ::std::string& sQueName, 
									::Ice::Int nType, 
									const ::Ice::Current& )
{
	cout << "==>> Invoke - NnmMethods::CreateQueue ( ";
	cout << Utf8ToString( sQueName ) << " ) ... ..." << endl;
	nType = 1;

	bool bOk = ::CreateQueue( Utf8ToString( sQueName ), 
							nType );	// 这个type别管它，取1就行 svdbapi.h

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::CreateQueue - CreateQueue : *failed*", 0, __FILE__, __LINE__ );
	}
	cout << "** NnmMethods::CreateQueue - CreateQueue : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk ? 1 : -1;
}

///////////////////////////////////////////////////////////
::Ice::Int NnmMethodsI::PushMessage(const ::std::string& sQueName, 
									const ::std::string& sLabel, 
									const ::std::string& sMsg, 
									const ::Ice::Current& )
{
	cout << "==>> Invoke - NnmMethods::PushMessage ... ..." << endl;
	cout << "Queue name : " << Utf8ToString( sQueName ) << endl;

	string sQName = Utf8ToString( sQueName );
	string sLab = Utf8ToString( sLabel );
	string sMsgText = Utf8ToString( sMsg );

	cout << "Queue Name : " << sQName << endl;
	cout << "Label : " << sLab << endl;
	cout << "Message text : " << sMsgText << endl;

	SvLog::Write( string( "==>> PushMessage - Queue Name : " + Utf8ToString( sQueName ) ).c_str(), 0, __FILE__, __LINE__ );

	bool bOk = ::PushMessage( Utf8ToString( sQueName ), 
								Utf8ToString( sLabel ), 
								Utf8ToString( sMsg ).c_str(), 
								Utf8ToString( sMsg ).length() );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::PushMessage - PushMessage : *failed*", 0, __FILE__, __LINE__ );
	}
	cout << "** NnmMethods::PushMessage - PushMessage : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk ? 1 : -1;
}

///////////////////////////////////////////////////////////
::Ice::Int NnmMethodsI::PopMessage(const ::std::string& sQueName, 
								   ::Ice::Int nTimer, 
								   ::std::string& sLabel, 
								   ::std::string& sCreateTime, 
								   ::std::string& sMsg, 
								   const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::PopMessage ... ..." << endl;
	cout << "Queue name : " << Utf8ToString( sQueName ) << endl;

	unsigned int nMsgLen = MQ_MSG_BUF_SIZE;
	char bufMsg[ MQ_MSG_BUF_SIZE ];
	string sLab = Utf8ToString( sLabel );
	TTime tmCreate;

	SvLog::Write( string( "==>> PopMessage - Queue Name : " + Utf8ToString( sQueName ) ).c_str(), 0, __FILE__, __LINE__ );

	memset( bufMsg, 0x00, sizeof( bufMsg ) );	

	bool bOk = ::PopMessage( Utf8ToString( sQueName ), 
							sLab, 
							tmCreate, 
							bufMsg, nMsgLen, nTimer );
	if( bOk )
	{
		sCreateTime = StringToUtf8( tmCreate.Format() );
		sLabel = StringToUtf8( sLab );
		sMsg = StringToUtf8( bufMsg );
		
		cout << "Label : " << sLab << endl;
		cout << "Message text : " << bufMsg << endl;
		cout << "Create time : " << sCreateTime << endl;
	}
	else
	{	
		SvLog::Write( "NnmMethods::PopMessage - PopMessage : *failed*", 0, __FILE__, __LINE__ );
	}

		

	cout << "** NnmMethods::PopMessage - PopMessage : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk ? 1 : -1;
}

///////////////////////////////////////////////////////////
::Ice::Int NnmMethodsI::BlockPopMessage(const ::std::string& sQueName, 
										::std::string& sLabel, 
										::std::string& sCreateTime, 
										::std::string& sMsg, 
										const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::BlockPopMessage ( ";
	cout << Utf8ToString( sQueName ) << " ) ... ..." << endl;

	unsigned int nMsgLen = MQ_MSG_BUF_SIZE;
	char bufMsg[ MQ_MSG_BUF_SIZE ];
	string sLab = Utf8ToString( sLabel );
	TTime tmCreate;

	// DatHandleUtils::String2TTime( sCreateTime, tmCreate );

	memset( bufMsg, 0x00, sizeof( bufMsg ) );
	bool bOk = ::BlockPopMessage( Utf8ToString( sQueName ), 
								sLab, 
								tmCreate,
								bufMsg, nMsgLen );
	if( bOk )
	{
		sLabel = StringToUtf8( sLab );
		sCreateTime = StringToUtf8( tmCreate.Format() );
		sMsg = StringToUtf8( bufMsg );
	}
	else
	{
		SvLog::Write( "NnmMethods::BlockPopMessage - BlockPopMessage : *failed*", 0, __FILE__, __LINE__ );
	}

	cout << "** NnmMethods::BlockPopMessage - BlockPopMessage : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk ? 1 : -1;
}

///////////////////////////////////////////////////////////
::Ice::Int NnmMethodsI::PeekMQMessage(const ::std::string& sQueName, 
									  ::Ice::Int nTimer, 
									  ::std::string& sLabel, 
									  ::std::string& sCreateTime, 
									  ::std::string& sMsg, 
									  const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::PeekMQMessage ( ";
	cout << Utf8ToString( sQueName ) << " ) ... ..." << endl;

	unsigned int nMsgLen = MQ_MSG_BUF_SIZE;
	char bufMsg[ MQ_MSG_BUF_SIZE ];
	string sLab = Utf8ToString( sLabel );
	TTime tmCreate;

	// DatHandleUtils::String2TTime( sCreateTime, tmCreate );

	memset( bufMsg, 0x00, sizeof( bufMsg ) );
	bool bOk = ::PeekMQMessage( Utf8ToString( sQueName ), 
								sLab, 
								tmCreate,
								bufMsg, nMsgLen, nTimer );
	if( bOk )
	{
		sLabel = StringToUtf8( sLab );		
		sCreateTime = StringToUtf8( tmCreate.Format() );
		sMsg = StringToUtf8( bufMsg );
	}
	else
	{
		SvLog::Write( "NnmMethods::PeekMQMessage - PeekMQMessage : *failed*", 0, __FILE__, __LINE__ );
	}

	cout << "** NnmMethods::PeekMQMessage - PeekMQMessage : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk ? 1 : -1;
}


///////////////////////////////////////////////////////////
::Ice::Int NnmMethodsI::BlockPeekMQMessage(const ::std::string& sQueName, 
										   ::std::string& sLabel, 
										   ::std::string& sCreateTime, 
										   ::std::string& sMsg, 
										   const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::BlockPeekMQMessage ( ";
	cout << Utf8ToString( sQueName ) << " ) ... ..." << endl;

	unsigned int nMsgLen = MQ_MSG_BUF_SIZE;
	char bufMsg[ MQ_MSG_BUF_SIZE ];
	string sLab = Utf8ToString( sLabel );
	TTime tmCreate;

	// DatHandleUtils::String2TTime( sCreateTime, tmCreate );

	memset( bufMsg, 0x00, sizeof( bufMsg ) );
	bool bOk = ::BlockPeekMQMessage( Utf8ToString( sQueName ), 
									sLab, 
									tmCreate,
									bufMsg, nMsgLen );
	if( bOk )
	{
		sLabel = StringToUtf8( sLab );
		sCreateTime = StringToUtf8( tmCreate.Format() );
		sMsg = StringToUtf8( bufMsg );
	}
	else
	{
		SvLog::Write( "NnmMethods::BlockPeekMQMessage - BlockPeekMQMessage : *failed*", 0, __FILE__, __LINE__ );
	}

	cout << "** NnmMethods::BlockPeekMQMessage - BlockPeekMQMessage : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk ? 1 : -1;
}

///////////////////////////////////////////////////////////
::Ice::Int NnmMethodsI::GetMQRecordCount(const ::std::string& sQueName, 
										 ::Ice::Int& nCount, 
										 const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::GetMQRecordCount ( ";
	cout << Utf8ToString( sQueName ) << " ) ... ..." << endl;

	unsigned int nCnt = 0;
	bool bOk = ::GetMQRecordCount( Utf8ToString( sQueName ), nCnt );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::GetMQRecordCount - GetMQRecordCount : *failed*", 0, __FILE__, __LINE__ );
	}
	nCount = bOk ? nCnt : -1;

	cout << "** NnmMethods::GetMQRecordCount - GetMQRecordCount : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk ? 1 : -1;
}

///////////////////////////////////////////////////////////
::Ice::Int NnmMethodsI::GetAllQueueNames(::SliceNNM::StringLst& nameList, 
										 const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::GetAllQueueNames ... ..." << endl;

	nameList.clear();

	std::list<string> slNameList;
	bool bOk = ::GetAllQueueNames( slNameList );
	
	try
	{
		if( bOk )
		{	// 取名称列表成功, 将名称列表复制到输出参数
			for( std::list<string>::iterator it = slNameList.begin();
				it != slNameList.end(); it++ )
			{
				nameList.push_back( StringToUtf8( *it ) );
			}
		}
		else
		{
			SvLog::Write( "NnmMethods::GetAllQueueNames - GetAllQueueNames : *failed*", 0, __FILE__, __LINE__ );
		}
	}
	catch( ... )
	{
		bOk = false;
		SvLog::Write( "NnmMethods::GetAllQueueNames - *exception*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetAllQueueNames - *exception*" << endl;
	}

	cout << "** NnmMethods::GetAllQueueNames - GetAllQueueNames : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk ? 1 : -1;
}

///////////////////////////////////////////////////////////
::Ice::Int NnmMethodsI::ClearQueueMessage(const ::std::string& sQueName, 
										  const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::ClearQueueMessage ( ";
	cout << Utf8ToString( sQueName ) << " ) ... ..." << endl;

	bool bOk = ::ClearQueueMessage( Utf8ToString( sQueName ) );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::ClearQueueMessage - ClearQueueMessage : *failed*", 0, __FILE__, __LINE__ );
	}

	cout << "** NnmMethods::ClearQueueMessage - ClearQueueMessage : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk ? 1 : -1;
}

///////////////////////////////////////////////////////////
::Ice::Int NnmMethodsI::DeleteQueue(const ::std::string& sQueName, 
									const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::DeleteQueue ( ";
	cout << Utf8ToString( sQueName ) << " ) ... ..." << endl;

	bool bOk = ::DeleteQueue( Utf8ToString( sQueName ) );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::DeleteQueue - DeleteQueue : *failed*", 0, __FILE__, __LINE__ );
	}
	cout << "** NnmMethods::DeleteQueue - DeleteQueue : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk ? 1 : -1;
}



///////////////////////////////////////////////////////////
// 扫描相关方法
///////////////////////////////////////////////////////////
bool NnmMethodsI::StartScan(const ::SliceNNM::ScanConfig& sc, 
							const ::std::string& sQueName, 
							const ::Ice::Current& )
{
	cout << "==>> Invoke - NnmMethods::StartScan ( ";
	cout << Utf8ToString( sQueName ) << " ) ... ..." << endl;

	ScanParameter sp;
	DatHandleUtils::ScanParaSlice2Svdb( sc, sp );
	
	GetRootDirFromSysReg();						// 注册表中读取SVDB路目录路径
	ScanUtils::SetRootDir( m_sSvdbRootDir );	// 传递 SVDB 根目录路径给 Scan 类

	bool bOk = false;
	// 创建与扫描进程关联的 Queue
	/*
	bOk = this->CreateQueue( Utf8ToString( sQueName ), 1 );
	cout << "---- NnmMethods::StartScan - StartScan : " << ( bOk ? "success!" : "*failed*" ) << endl;
	if( !bOk )
	{
		SvLog::Write( string( "NnmMethods::StartScan - StartScan : *failed*" ).c_str(), 
					0, __FILE__, __LINE__ );	
		return false;
	}
	*/

	// 启动扫描进程
	bOk = ScanUtils::StartScan( sp,			// 启动扫描
								Utf8ToString( sQueName ),
								false,			// 若已经启动,就不重新启动
								false );		// 非异步方式

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::StartScan - ScanUtils::StartScan : *failed*", 0, __FILE__, __LINE__ );
	}
	cout << "** NnmMethods::StartScan - StartScan : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::CancelScan(const ::std::string& sQueName, 
							 const ::Ice::Current& )
{
	cout << "==>> Invoke - NnmMethods::CancelScan ( ";
	cout << Utf8ToString( sQueName ) << " ) ... ..." << endl;

	bool bOk = ScanUtils::CancelScan( Utf8ToString( sQueName ) );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::CancelScan - ScanUtils::CancelScan : *failed*", 0, __FILE__, __LINE__ );
	}
	cout << "** NnmMethods::CancelScan - CancelScan : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::GetScanParameter(const ::std::string& sQueName, 
								   ::SliceNNM::ScanConfig& sc, 
								   const ::Ice::Current& )
{
	cout << "==>> Invoke - NnmMethods::GetScanParameter ( ";
	cout << Utf8ToString( sQueName ) << " ) ... ..." << endl;

	ScanParameter spParam;
	bool bOk = ScanUtils::GetScanParameter( sQueName, spParam );
	if( bOk )
	{
		DatHandleUtils::ScanParaSvdb2Slice( spParam, sc );
	}
	else
	{
		SvLog::Write( "NnmMethods::GetScanParameter - ScanUtils::GetScanParameter : *failed*", 0, __FILE__, __LINE__ );
	}

	cout << "** NnmMethods::GetScanParameter - GetScanParameter : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::SubmitScanResult(const ::std::string& sQueName, 
								   const ::SliceNNM::TopologyChart& tc, 
								   const ::Ice::Current& )
{
	cout << "==>> Invoke - NnmMethods::SubmitScanResult ( ";
	cout << Utf8ToString( sQueName ) << " ) ... ..." << endl;

	bool bOk = ScanUtils::SubmitScanResult( sQueName, tc );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::SubmitScanResult - ScanUtils::SubmitScanResult : *failed*", 0, __FILE__, __LINE__ );
	}
	cout << "** NnmMethods::SubmitScanResult - SubmitScanResult : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::GetScanResult(const ::std::string& sQueName, 
								::SliceNNM::TopologyChart& tc, 
								const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::GetScanResult ( ";
	cout << Utf8ToString( sQueName ) << " ) ... ..." << endl;

	bool bOk = ScanUtils::GetScanResult( sQueName, tc );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::GetScanResult - ScanUtils::GetScanResult : *failed*", 0, __FILE__, __LINE__ );
	}
	cout << "** NnmMethods::GetScanResult - GetScanResult : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::StartRealTimeScan(const ::std::string& sSession, 
									::Ice::Int nFreq, 
									const ::SliceNNM::StringLst& idList, 
									const ::Ice::Current& )
{
	cout << "==>> Invoke - NnmMethods::StartRealTimeScan ( ";
	cout << Utf8ToString( sSession ) << " ) ... ..." << endl;

	bool bOk = ScanUtils::StartRealTimeScan( sSession, nFreq, idList );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::StartRealTimeScan - ScanUtils::StartRealTimeScan : *failed*", 0, __FILE__, __LINE__ );
	}
	cout << "** NnmMethods::StartRealTimeScan - StartRealTimeScan : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::CancelRealTimeScan(const ::std::string& sSession, 
									 const ::Ice::Current& )
{
	cout << "==>> Invoke - NnmMethods::StartRealTimeScan ( ";
	cout << Utf8ToString( sSession ) << " ) ... ..." << endl;

	bool bOk = ScanUtils::CancelRealTimeScan( sSession );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::CancelRealTimeScan - ScanUtils::CancelRealTimeScan : *failed*", 0, __FILE__, __LINE__ );
	}
	cout << "** NnmMethods::StartRealTimeScan - StartRealTimeScan : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	return bOk;
}


///////////////////////////////////////////////////////////
// DB 操作方法
///////////////////////////////////////////////////////////
bool NnmMethodsI::GetSVDYN(const ::std::string& sMonId, 
						   ::SliceNNM::SVDYN& dyn, 
						   const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::GetSVDYN ( ";
	cout << Utf8ToString( sMonId ) << " ) ... ..." << endl;

	SVDYN dynSvdb;

	// 从 svdb 中读出 dyn
	bool bOk = ::GetSVDYN( Utf8ToString( sMonId ), dynSvdb );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::GetSVDYN - GetSVDYN : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetSVDYN - GetSVDYN : *failed*" << endl;

		return false;
	}

	// 转换成 slice 格式
	dyn.createtime			= StringToUtf8( dynSvdb.m_time.Format() );
	dyn.state				= dynSvdb.m_state;
	dyn.keeplaststatetime	= dynSvdb.m_keeplaststatetime.GetTotalSeconds();
	dyn.laststatekeeptime	= dynSvdb.m_laststatekeeptimes;
	dyn.displaystring		= StringToUtf8( dynSvdb.m_displaystr );


	cout << "** NnmMethods::GetSVDYN - all done!" << endl;

	return true;

	// svdb
	//TTime m_time;
	//int	 m_state;
	//TTimeSpan m_keeplaststatetime;
	//unsigned int m_laststatekeeptimes;
	//char *m_displaystr;

	// slice
	//struct SVDYN
 //   ::std::string createtime;
 //   ::Ice::Int state;
 //   ::Ice::Int keeplaststatetime;
 //   ::Ice::Int laststatekeeptime;
 //   ::std::string displaystring;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::QueryRecords(const ::std::string& sMonId, 
							   const ::std::string& sBeginTime, 
							   const ::std::string& sEndTime, 
							   ::SliceNNM::RecordSet& rs, 
							   const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::QueryRecords ( ";
	cout << Utf8ToString( sMonId ) << " ) ... ..." << endl;

	// 将查询参数转换成 svdb 格式
	TTimeSpan timeSpan;
	TTime tmBegin;
	TTime tmEnd;

	// 将查询参数转化为 Svdb 格式
	DatHandleUtils::String2TTime( sBeginTime, tmBegin );
	DatHandleUtils::String2TTime( sEndTime, tmEnd );

	// 从 Svdb 数据库中查询出记录
	RECORDSET rsSvdb = ::QueryRecords( Utf8ToString( sMonId ), 
										tmBegin, tmEnd );
	if( rsSvdb == NULL )
	{
		SvLog::Write( "NnmMethods::QueryRecords - QueryRecords : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::QueryRecords - QueryRecords : *failed*" << endl;

		return false;
	}
	RecordSet *pRsSvdb = reinterpret_cast<RecordSet *>( rsSvdb );
	if( NULL == pRsSvdb )
	{
		SvLog::Write( "NnmMethods::QueryRecords - reinterpret_cast : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::QueryRecords - reinterpret_cast : *failed*" << endl;

		return false;
	}

	// 将记录格式由 Svdb 格式转化为 Slice 格式
	DatHandleUtils::RecordSetSvdb2Slice( *pRsSvdb, rs );

	cout << "** NnmMethods::QueryRecords - all done!" << endl;

	return true;
}

///////////////////////////////////////////////////////////
// SNMP 相关的操作方法
///////////////////////////////////////////////////////////
bool NnmMethodsI::SnmpSetParam(::Ice::Int nRetries, 
							   ::Ice::Int nTimeOut, 
							   ::Ice::Int nVer, 
							   const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::SnmpSetParam ... ..." << endl;

	 myWriteLog->WriteLog("SnmpSetParam start.\n");

	 ::g_snmpRetries = nRetries;
	 ::g_snmpTimeout = nTimeOut;
	 ::g_snmpVersion = nVer;

	 cout << "** NnmMethods::SnmpSetParam - all done!" << endl;

	 return true; 
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::SnmpSetValueEx(const ::std::string& sEntId, 
								 const ::std::string& sOid, 
								 const ::std::string& sCommSet, 
								 const ::std::string& sValue, 
								 bool bNumber, 
								 const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::SnmpSetValueEx ( " << sEntId << " ) ... ..." << endl;
	cout << "Oid             : " << sOid << endl;
	cout << "Community of Set: " << sCommSet << endl;
	cout << "Value           : " << sValue << endl;


	myWriteLog->WriteLog("Write Mib value.");

	// call TwriteMib() that exported by readDeviceInfo.dll to write Mib value

	//Find Entity
	std::string sid=::Utf8ToString(sEntId);
	//if(!this->m_pMain->m_pEntityEx->Find(sOid))
	//{
	//	myWriteLog->WriteLog("查找 entity 失败！");
	//	return false;
	//}

	// Get Entity
	OBJECT obj= GetNNMEntity(sid);
	if(obj==INVALID_VALUE)
	{
		myWriteLog->WriteLog("GetNNMEntity 失败！");
		return false;
	}
	EntityEx *pEn=reinterpret_cast<EntityEx *>(obj);
	if(pEn==NULL)
	{
		myWriteLog->WriteLog("获取 entity 失败！");
		CloseNNMEntity(obj);
		return false;
	}

	StringMap &sm = pEn->GetProperty();

	TwriteParameter wPara;

	strcpy(wPara.value, sValue.c_str());
    wPara.portNo  = -1;
    wPara.isNumber= bNumber;
    wPara.indicatorIndex = -1;
    wPara.retries = 3;
    wPara.timeout = 500;
    wPara.version = 1;

	strcpy(wPara.oid, sOid.c_str());
    strcpy(wPara.writeCommunity, sCommSet.c_str());
	strcpy(wPara.ip, sm["ip"].getword());

	CloseNNMEntity(obj);
	if(m_TwriteMib(wPara) > 0 )
	{
		myWriteLog->WriteLog("写 Mib 成功！");
		return true;
	}
	else
	{
		myWriteLog->WriteLog("写 Mib 失败！");
		return false;
	}
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::SnmpGetValue(const ::std::string& sEntId, 
							   ::Ice::Int nInfoType , 
							   ::Ice::Int& nRecNo, 
							   ::Ice::Int& nColNo, 
							   ::SliceNNM::SnmpValue& sv, 
							   const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::SnmpGetValue ( ";
	cout << Utf8ToString( sEntId ) << " ) ... ..." << endl;

	myWriteLog->WriteLog("SnmpGetValue start.");

	//clear return value
	nRecNo = 0;
	nColNo = 0;
	sv.resize(0);
				
	//Find Entity
	std::string sid=::Utf8ToString(sEntId);
	OBJECT obj= GetNNMEntity(sid);
	if(obj==INVALID_VALUE)
	{
		myWriteLog->WriteLog("GetNNMEntity 失败！");
		return false;
	}
	EntityEx *pEn=reinterpret_cast<EntityEx *>(obj);
	if(pEn==NULL)
	{
		myWriteLog->WriteLog("获取 entity 失败！");
		CloseNNMEntity(obj);
		return false;
	}


	TreadParameter tp;
	tp.indicatorIndex = nInfoType;
	tp.isTable = false;
	tp.retries = g_snmpRetries;
	tp.timeout = g_snmpTimeout;
	tp.version = g_snmpVersion;
	tp.isNumber = FALSE;

	StringMap &sm = pEn->GetProperty();

	//Init Value
	memset(tp.ip,0,32);
	memset(tp.readCommunity,0,260);
	memset(tp.oid,0,OIDLENGTH);
	memset(tp.sysOid,0,OIDLENGTH);

	//set readcomm
	std::string readcomm = "public";
	try
	{
		readcomm = sm["_Community"];
	}
	catch(...)
	{
	}

	if(readcomm.length()<=0)
	{
		readcomm = "public";
	}
	
	int len = MIN(readcomm.length(),260);
	memcpy(tp.readCommunity,readcomm.c_str(),len);

	//set ip
	std::string ip = "";
	try
	{
		ip = sm["ip"];
	}
	catch(...)
	{
	}

	if(ip.length()<6)	
	{
		myWriteLog->WriteLog("IP 地址错误！.");
		CloseNNMEntity(obj);
		return false;					// Luowan 2007.5.23
										//ip = "192.168.0.254";
	}

	len = MIN(ip.length(),32);
	memcpy(tp.ip,ip.c_str(),len);
	
	std::string oid = "";
	try
	{
		oid = sm["entityOID"];
	}
	catch(...)
	{
	}

	len = MIN(oid.length(),OIDLENGTH);
	memcpy(tp.sysOid,oid.c_str(),len);

	//Get Device Info
	TmibData *myMibData;

	//Tmibtable *pTmb = NULL;
	char pBuf[1024];
	int i, realCount;

	bool bOk = false;
	try
	{
		int ret = 0;	//TLoadIniFile(NULL);

		sprintf(pBuf, "TstartGetDeviceInfo ptr:%d",m_TstartGetDeviceInfo);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "参数 ip:%s",tp.ip);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "参数 indic:%d",tp.indicatorIndex);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "参数 istable:%d",tp.isTable);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "参数 sysOID:%s",tp.sysOid);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "参数 oid:%s",tp.oid);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "参数 readComm:%s",tp.readCommunity);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "参数 retries:%d",tp.retries);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "参数 timeout:%d",tp.timeout);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "参数 version:%d",tp.version);
		myWriteLog->WriteLog(pBuf);

		 // Luowan 2007.5.23

		//pTmb = new Tmibtable;
		realCount = 1000;

		int mibSize;
		
		myMibData = new TmibData[realCount];

		/*
		pTmb->recordNo = realCount;
		pTmb->fields = new Tfields[realCount];
		for( i=0;i<realCount;i++)
			pTmb->fields[i].field = new Tfield[3];
		*/
		//ret = m_TstartGetDeviceInfo(tp,nRecNo,nColNo, pTmb);
		nRecNo = realCount;
		try
		{
			ret = m_TstartGetDeviceInfo(tp,nRecNo,nColNo, myMibData);
		}
		catch(...)
		{
			myWriteLog->WriteLog("读信息发生异常！");
			CloseNNMEntity(obj);
			return false;
		}

		if(ret == -1 && nRecNo > realCount)				// pTmb 分配的空间小于实际要求
		{
			myWriteLog->WriteLog("返回数据超过1000行，重新分配空间。");
			delete myMibData;
			/*
			for( i=0;i<realCount;i++)
				delete pTmb->fields[i].field;
			delete pTmb->fields;
			*/
			realCount = nRecNo+1;

			myMibData = new TmibData[realCount];
			/*
			pTmb->recordNo = realCount;
			pTmb->fields = new Tfields[realCount];
			for( i=0;i<realCount;i++)
				pTmb->fields[i].field = new Tfield[3];
			ret = m_TstartGetDeviceInfo(tp,nRecNo,nColNo, pTmb);	// 重新获取一次
			*/
			nRecNo = realCount;
			ret = m_TstartGetDeviceInfo(tp,nRecNo,nColNo, myMibData);	// 重新获取一次
		}

		if(ret<=0)
		{
			/*
			for( i=0;i<realCount;i++)
				delete pTmb->fields[i].field;
			delete pTmb->fields;
			*/
			delete myMibData;

			myWriteLog->WriteLog("SnmpGetValue 没有取到信息！");
			CloseNNMEntity(obj);
			return false;
		}
		else
		{
			for(i=0;i<nRecNo;i++)
			{
				/*
				strcpy(pBuf, pTmb->fields[i].field[0].volume);
				strcat(pBuf, " -> ");
				strcat(pBuf, pTmb->fields[i].field[1].volume);
				*/
				strcpy(pBuf, myMibData[i].oidStr);
				strcat(pBuf, " -> ");
				strcat(pBuf, myMibData[i].valueStr);

				myWriteLog->WriteLog(pBuf);
			}
		}

		if(nColNo>0 && nRecNo>0)
		{
			/*
			pTmb = new Tmibtable;
			pTmb->recordNo = nRecNo;
			pTmb->fields = new Tfields[nRecNo];
			for(int irec=0;irec<nRecNo;irec++)
			{
				pTmb->fields[irec].fieldNo = nColNo;
				pTmb->fields[irec].field = new Tfield[nColNo];			
			}

			ret = this->m_pMain->m_TreadDeviceInfo(pTmb);
			*/

			//Fill snmp value
			if(ret!=0)
			{
				sv.resize(nRecNo);
				for(int irec1=0;irec1<nRecNo;irec1++)
				{
					::SliceNNM::StringLst slst ;
					slst.resize(nColNo);

					slst[0]=::StringToUtf8(myMibData[irec1].oidStr);
					slst[1]=::StringToUtf8(myMibData[irec1].valueStr);

					/*
					for(int icol=0;icol<nColNo;icol++)
					{
						//std::string tmp=::Utf8ToString(pTmb->fields[irec1].field[icol].volume);
						//slst[icol]=pTmb->fields[irec1].field[icol].volume;
					}
					*/
					sv[irec1]=slst;
				}
				bOk = true;
			}
		}	
	}
	catch(...)
	{
		myWriteLog->WriteLog("SnmpGetValue 发生异常！.");
		CloseNNMEntity(obj);
		bOk = false;
	} 
	CloseNNMEntity(obj);
	//Free memory
	delete myMibData;
	/*
	for( i=0;i<realCount;i++)
		delete pTmb->fields[i].field;
	delete pTmb->fields;
	delete pTmb;
	*/
	
	myWriteLog->WriteLog("SnmpGetValue Success.");

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::SnmpGetValueEx(const ::std::string& sEntId, 
								 const ::std::string& sOid, 
								 bool bTable, 
								 bool bNumber, 
								 ::Ice::Int& nRecNo, 
								 ::Ice::Int& nColNo, 
								 ::SliceNNM::SnmpValue& sv, 
								 const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::SnmpGetValueEx ( ";
	cout << Utf8ToString( sEntId ) << " ) ... ..." << endl;
	cout << "Oid: " << Utf8ToString( sOid ) << endl;

	myWriteLog->WriteLog("SnmpGetValueEx Beginning.");

	if(sOid.compare("1.3.6.1.2.1.25.2.3.1.3")==0||sOid.compare("1.3.6.1.2.1.25.6.3.1.2")==0)
	{
		int i = 1;
	}

	//clear return value
	nRecNo = 0;
	nColNo = 0;
	sv.resize(0);
		
	//Find Entity
	std::string sid=::Utf8ToString(sEntId);
	OBJECT obj= GetNNMEntity(sid);
	if(obj==INVALID_VALUE)
	{
		myWriteLog->WriteLog("GetNNMEntity 失败！");
		return false;
	}
	EntityEx *pEn=reinterpret_cast<EntityEx *>(obj);
	if(pEn==NULL)
	{
		myWriteLog->WriteLog("获取 entity 失败！");
		CloseNNMEntity(obj);
		return false;
	}

	TreadParameter tp;
	tp.indicatorIndex = 0;
	tp.isTable = bTable;	//false;
	tp.retries = g_snmpRetries;
	tp.timeout = g_snmpTimeout;
	tp.version = g_snmpVersion;
	tp.isNumber = bNumber;

	//Init Value
	memset(tp.ip,0,32);
	memset(tp.readCommunity,0,260);
	memset(tp.oid,0,OIDLENGTH);
	memset(tp.sysOid,0,OIDLENGTH);

	StringMap &sm = pEn->GetProperty();
	std::string readcomm = "public";
	try
	{
		readcomm = sm["_Community"];
	}
	catch(...)
	{
		myWriteLog->WriteLog("取共同体错误！");
	}

	if(readcomm.length()==0)
	{
		readcomm = "public";
	}

	int len = MIN(readcomm.length(),260);
	memcpy(tp.readCommunity,readcomm.c_str(),len);
	
	//set ip
	std::string ip = "";
	try
	{
		 ip = sm["ip"];
	}
	catch(...)
	{
		myWriteLog->WriteLog("取IP地址错误！");
	}	

	if(ip.length()<6)	
	{
		ip = "192.168.0.254";
	}

	len = MIN(ip.length(),32);
	memcpy(tp.ip,ip.c_str(),len);

	len = MIN(sOid.length(),OIDLENGTH);
	memcpy(tp.oid,sOid.c_str(),len);

	tp.isTable = bTable;
	
	//Get Device Info
	TmibData *myMibData;
	//Tmibtable *pTmb = NULL;
	int realCount;
	char pBuf[1024];
	int i;

	bool bOk = false;
	try
	{
		int ret = 0;//TLoadIniFile(NULL);

		sprintf(pBuf, "TstartGetDeviceInfo ptr:%d",m_TstartGetDeviceInfo);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "参数 ip:%s",tp.ip);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "参数 indic:%d",tp.indicatorIndex);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "参数 istable:%d",tp.isTable);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "参数 sysOID:%s",tp.sysOid);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "参数 oid:%s",tp.oid);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "参数 readComm:%s",tp.readCommunity);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "参数 retries:%d",tp.retries);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "参数 timeout:%d",tp.timeout);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "参数 version:%d",tp.version);
		myWriteLog->WriteLog(pBuf);

		 // Luowan 2007.5.23		

		//pTmb = new Tmibtable;
		realCount		= 1000;
		myMibData = new TmibData[realCount];

		//ret = this->m_pMain->m_TstartGetDeviceInfo(tp,nRecNo,nColNo, pTmb);
		nRecNo = realCount;
		try
		{
			// cout << "Call startGetDeviceInfo in DLL..." << endl;

			ret = m_TstartGetDeviceInfo(tp,nRecNo,nColNo, myMibData);

			// cout << "Call startGetDeviceInfo in DLL done!" << endl;
		}
		catch(...)
		{
			myWriteLog->WriteLog("读信息发生异常！");
			CloseNNMEntity(obj);
			cout << "DEBUG - Line : " << __LINE__ << endl;
			return false;
		}

		// cout << "DEBUG - Line : " << __LINE__ << endl;
		if(ret == -1 && nRecNo > realCount)
		{
			myWriteLog->WriteLog("返回数据超过1000行，重新分配空间。");

			delete []myMibData;
			realCount = nRecNo+1;

			myMibData = new TmibData[realCount];

			nRecNo = realCount;
			ret = m_TstartGetDeviceInfo(tp,nRecNo,nColNo, myMibData);		// 重新获取一次
		}

		// cout << "DEBUG - Line : " << __LINE__ << endl;
		if(ret<=0)
		{
			if(myMibData != NULL)
				delete []myMibData;

			myWriteLog->WriteLog("SnmpGetValueEx 没有取到信息！");
			
			CloseNNMEntity(obj);

			// cout << "DEBUG - Line : " << __LINE__ << endl;

			return false;
		}
		else
		{
			for(i=0;i<nRecNo;i++)
			{
				strcpy(pBuf, myMibData[i].oidStr);
				strcat(pBuf, " -> ");
				strcat(pBuf, myMibData[i].valueStr);

				myWriteLog->WriteLog(pBuf);
			}
		}

		// cout << "DEBUG - Line : " << __LINE__ << endl;

		// ret = this->m_pMain->m_TstartGetDeviceInfo(tp,nRecNo,nColNo);
		if(nColNo>0 && nRecNo>0)
		{
			//Fill snmp value
			if(ret!=0)
			{
				sv.resize(nRecNo);
				for(int irec1=0;irec1<nRecNo;irec1++)
				{
					::SliceNNM::StringLst slst ;
					slst.resize(nColNo);

					slst[0]=::StringToUtf8(myMibData[irec1].oidStr);
					slst[1]=::StringToUtf8(myMibData[irec1].valueStr);

					/*
					for(int icol=0;icol<nColNo;icol++)
					{
						//std::string tmp=::Utf8ToString(pTmb->fields[irec1].field[icol].volume);
						slst[icol]=pTmb->fields[irec1].field[icol].volume;
					}
					*/
					sv[irec1]=slst;
				}
				bOk = true;
			}
		}	
	}
	catch(...)
	{
		myWriteLog->WriteLog("SnmpGetValueEx 遇到异常!.");
		bOk = false;

		// cout << "DEBUG - Line : " << __LINE__ << endl;
	} 

	// cout << "DEBUG - Line : " << __LINE__ << endl;

	CloseNNMEntity(obj);
	//Free memory
	delete []myMibData;

	myWriteLog->WriteLog("SnmpGetValueEx Success");

	cout << "DEBUG - GetSnmpVAlue all done!" << __LINE__ << endl;

	return bOk;
}

///////////////////////////////////////////////////////////
// Monitor相关的操作方法
///////////////////////////////////////////////////////////
bool NnmMethodsI::GetMonitorTemplete(const ::std::string& sMonId, 
									 ::SliceNNM::Monitor& mon, 
									 const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::GetMonitorTemplete ( ";
	cout << Utf8ToString( sMonId ) << " ) ... ..." << endl;
	
	// 从 svdb 中取得对象数据
	OBJECT	mtObj = ::GetMonitorTemplet( atoi( Utf8ToString( sMonId ).c_str() ) );
	if( mtObj == NULL )
	{
		SvLog::Write( "NnmMethods::GetMonitorTemplete - GetMonitorTemplet : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetMonitorTemplete - GetMonitorTemplet : *failed*" << endl;

		return false;
	}

	// 转换成对应的类对象
	MonitorTemplet *pMt = reinterpret_cast< MonitorTemplet * >( mtObj );
	if( NULL == pMt )
	{
		SvLog::Write( "NnmMethods::GetMonitorTemplete - reinterpret_cast : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetMonitorTemplete - reinterpret_cast : *failed*" << endl;

		return false;
	}

	// 将 svdb 数据格式转换成 slice 格式
	int nId = pMt->GetID();
	char szBuf[ 64 ];
	memset( szBuf, 0x00, sizeof( szBuf ) );
	sprintf( szBuf, "%d", nId );
	mon.monitorID	= StringToUtf8( szBuf );
	mon.templeteID	= StringToUtf8( szBuf);

	StringMap ** pAltCondArray	= pMt->GetAlertCondition();
	StringMap &prop			= pMt->GetProperty();
	STRMAPLIST &smlAdvPara	= pMt->GetAdvanceParameter();
	STRMAPLIST &smlParam	= pMt->GetParameter();
	STRMAPLIST &smlReturn	= pMt->GetReturn();

	::SliceNNM::StringDict sdMonProp;
	DatHandleUtils::StringMap2StringDict( prop, sdMonProp );
	mon.Property.push_back( sdMonProp );

	DatHandleUtils::STRINGMAPLISTSvdb2Slice( smlAdvPara, mon.AdvanceParameter );
	DatHandleUtils::STRINGMAPLISTSvdb2Slice( smlParam, mon.Parameter );
	DatHandleUtils::STRINGMAPLISTSvdb2Slice( smlReturn, mon.Return );

	bool bOk = true;
	try
	{
		for( int i = 0; i < 3; i++ )
		{
			::SliceNNM::StringDict sdAltCond;
			DatHandleUtils::StringMap2StringDict( *pAltCondArray[ i ], sdAltCond );

			mon.AlertCondition.push_back( sdAltCond );
		}
	}
	catch( ... )
	{
		bOk = false;
		SvLog::Write( "NnmMethods::GetMonitorTemplete - *exception*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetMonitorTemplete - *exception*" << endl;
	}

	::CloseMonitorTemplet( mtObj );	// 删除对象，以免内存泄漏

	cout << "** NnmMethods::GetMonitorTemplete - all done!" << endl;

	return bOk;


	//struct Monitor
 //   ::std::string monitorID;
 //   ::std::string templeteID;
 //   ::SliceNNM::STRINGMAPLIST Property;
 //   ::SliceNNM::STRINGMAPLIST Parameter;
 //   ::SliceNNM::STRINGMAPLIST AdvanceParameter;
 //   ::SliceNNM::STRINGMAPLIST AlertCondition;
 //   ::SliceNNM::STRINGMAPLIST Return;

	//typedef ::std::vector< ::SliceNNM::Monitor> MonitorLst;

	//typedef ::std::vector< ::SliceNNM::StringDict> STRINGMAPLIST;

}

///////////////////////////////////////////////////////////
bool NnmMethodsI::SubmitMonitor(const ::std::string& sEntId, 
								const ::SliceNNM::Monitor& mon, 
								const ::Ice::Current& )
{
	cout << "==>> Invoke - NnmMethods::SubmitMonitor ( ";
	cout << Utf8ToString( sEntId ) << " ) ... ..." << endl;

	// 从 svdb 中取得对象数据
	OBJECT	monObj = ::CreateMonitor();
	if( monObj == NULL )
	{
		SvLog::Write( "NnmMethods::SubmitMonitor - CreateMonitor : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::SubmitMonitor - CreateMonitor : *failed*" << endl;

		return false;
	}

	// 转换成对应的类对象
	Monitor *pMon = reinterpret_cast< Monitor * >( monObj );
	if( NULL == pMon )
	{
		SvLog::Write( "NnmMethods::SubmitMonitor - reinterpret_cast : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::SubmitMonitor - reinterpret_cast : *failed*" << endl;

		return false;
	}

	bool bOk = true;

	try
	{
		// 将 slice 数据格式转换成 svdb 格式
		pMon->PutID( Utf8ToString( mon.monitorID ) );	// ID号	
		
		// 强制类型转换成非 const 变量
		::SliceNNM::Monitor &monRef = const_cast< ::SliceNNM::Monitor &>( mon );

		StringMap &prop		= pMon->GetProperty();
		StringMap &para		= pMon->GetParameters();
		StringMap &adPara	= pMon->GetADParameters();
		StringMap *altCond	= pMon->GetAlertCondition();

		// 属性	
		prop.clear();
		for( ::SliceNNM::STRINGMAPLIST::iterator it = monRef.Property.begin(); 
			it != monRef.Property.end(); it++ )
		{
			DatHandleUtils::StringDict2StringMap( *it, prop );
			break;
		}	

		// 参数	
		para.clear();
		for( ::SliceNNM::STRINGMAPLIST::iterator it = monRef.Parameter.begin(); 
			it != monRef.Parameter.end(); it++ )
		{
			DatHandleUtils::StringDict2StringMap( *it, para );
			break;
		}

		// 高级参数
		adPara.clear();
		for( ::SliceNNM::STRINGMAPLIST::iterator it = 
				monRef.AdvanceParameter.begin(); 
			it != monRef.AdvanceParameter.end(); it++ )
		{
			DatHandleUtils::StringDict2StringMap( *it, adPara );
			break;
		}

		// 可选条件
		int i = 0;
		for( ::SliceNNM::STRINGMAPLIST::iterator it = 
				monRef.AlertCondition.begin(); 
			it != monRef.AlertCondition.end(); it++ )
		{
			altCond[ i ].clear();
			DatHandleUtils::StringDict2StringMap( *it, altCond[ i ] );
			i++;
			if( i == 2 )
			{
				break;
			}
		}
	}
	catch( ... )
	{
		bOk = false;
		SvLog::Write( "NnmMethods::SubmitMonitor - *exception*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::SubmitMonitor - *exception*" << endl;
	}

	if( bOk )
	{
		bOk = ::SubmitMonitor( monObj );

		if( !bOk )
		{
			SvLog::Write( "NnmMethods::SubmitMonitor - SubmitMonitor : *failed*", 0, __FILE__, __LINE__ );
		}

		cout << "#### NnmMethods::SubmitMonitor - SubmitMonitor : " << string( bOk ? "success!" : "*failed*" ) <<endl;
	}

	:: CloseMonitor( monObj );	// 删除对象，以免内存泄漏

	cout << "** NnmMethods::SubmitMonitor - all done!" << endl;

	return bOk;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::GetAllMonitorID(const ::std::string& sMonId, 
								  ::SliceNNM::StringLst& monIdList, 
								  const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::GetAllMonitorID ( ";
	cout << Utf8ToString( sMonId ) << " ) ... ..." << endl;

	std::string sid=::Utf8ToString( sMonId );
	OBJECT obj=::GetEntity( sid );
	if(obj == INVALID_VALUE)
	{
		SvLog::Write( "NnmMethods::GetAllMonitorID - GetEntity : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetAllMonitorID - GetEntity : *failed*" << endl;

		return false;
	}

	std::list<string> idlist;
	GetSubMonitorsIDByEntity(obj,idlist);
	Entity *pEnt = reinterpret_cast<Entity *>(obj);
	if(pEnt == NULL)
	{
		SvLog::Write( "NnmMethods::GetAllMonitorID - reinterpret_cast : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetAllMonitorID - reinterpret_cast : *failed*" << endl;

		return false;
	}

	DatHandleUtils::WORDLIST2StringList(pEnt->GetMonitors(),monIdList);

	cout << "** NnmMethods::GetAllMonitorID - all done!" << endl;

	return true;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::GetMonitor(const ::std::string& sEntId, 
							 ::SliceNNM::Monitor& mon, 
							 const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::GetMonitor ( ";
	cout << Utf8ToString( sEntId ) << " ) ... ..." << endl;

	// 从 svdb 中取得对象数据
	OBJECT	monObj = ::GetMonitor( Utf8ToString( sEntId ) );
	if( monObj == NULL )
	{
		SvLog::Write( "NnmMethods::GetMonitor - GetMonitor : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetMonitor - GetMonitor : *failed*" << endl;

		return false;
	}

	// 转换成对应的类对象
	Monitor *pMon = reinterpret_cast< Monitor * >( monObj );
	if( NULL == pMon )
	{
		SvLog::Write( "NnmMethods::GetMonitor - reinterpret_cast : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetMonitor - reinterpret_cast : *failed*" << endl;

		return false;
	}
	
	bool bOk = true;
	try
	{
		// 将 svdb 数据格式转换成 slice 格式
		mon.monitorID = StringToUtf8( pMon->GetID().getword() );	// ID号	
		
		::SliceNNM::StringDict sdSlice;

		mon.Property.clear();
		mon.Parameter.clear();
		mon.AdvanceParameter.clear();
		mon.AlertCondition.clear();

		// 属性
		StringMap &prop = pMon->GetProperty();	
		DatHandleUtils::StringMap2StringDict( prop, sdSlice );	
		mon.Property.push_back( sdSlice );

		// 参数
		StringMap &para = pMon->GetParameters();
		DatHandleUtils::StringMap2StringDict( para, sdSlice );	
		mon.Parameter.push_back( sdSlice );

		// 高级参数
		StringMap &adPara = pMon->GetADParameters();
		DatHandleUtils::StringMap2StringDict( adPara, sdSlice );	
		mon.AdvanceParameter.push_back( sdSlice );

		// 可选条件
		StringMap *altCond	= pMon->GetAlertCondition();
		for( int i = 0; i < 3; i++ )
		{
			DatHandleUtils::StringMap2StringDict( altCond[ i ], sdSlice );
			mon.AlertCondition.push_back( sdSlice );
		}
	}
	catch( ... )
	{
		bOk = false;
		SvLog::Write( "NnmMethods::GetMonitor - reinterpret_cast : *exception*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetMonitor - *exception*" << endl;
	}

	:: CloseMonitor( monObj );	// 删除对象，以免内存泄漏

	cout << "** NnmMethods::GetMonitor - all done!" << endl;

	return bOk;
}

///////////////////////////////////////////////////////////
// DES 加/解密
///////////////////////////////////////////////////////////
// 解密
bool NnmMethodsI::desDecrypt(const ::std::string& sIn, 
							 ::std::string& sOut, 
							 const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::desDecrypt ( " << Utf8ToString( sIn ) << " ) ... ..." << endl;

	bool bOk = false;
	char *pOutBuf = NULL;

	try
	{
		unsigned int nInLen = sIn.length();
		unsigned int nOutLen = nInLen % 8 ? nInLen + 8 - nInLen % 8 : nInLen;
		nOutLen += 1;	// 增加一个结束符空间
		
		pOutBuf = new char[ nOutLen ];
		memset( pOutBuf, 0x00, nOutLen );
		
		sOut = "";
		Des des;
		bOk = des.Decrypt( sIn.c_str(), pOutBuf );
		if( bOk )
		{
			sOut.assign( pOutBuf );
		}
		
		memset( pOutBuf, 0x00, nOutLen );
		delete[] pOutBuf;
		pOutBuf = NULL;
	}
	catch( ... )
	{
		if( pOutBuf != NULL )
		{
			delete[] pOutBuf;
		}

		SvLog::Write( "NnmMethods::desDecrypt - *exception*", 0, sIn.c_str(), __LINE__ );
		cout << "#### NnmMethods::desDecrypt - *exception*" << endl;
		return false;
	}

	cout << "** NnmMethods::desDecrypt - all done! : " << string( bOk ? "success!" : "*failed*" ) << endl;

	return bOk;
}

///////////////////////////////////////////////////////////
// 加密
bool NnmMethodsI::desEncrypt(const ::std::string& sIn, 
							 ::std::string& sOut, 
							 const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::desDecrypt ( " << Utf8ToString( sIn ) << " ) ... ..." << endl;

	bool bOk = false;
	char *pOutBuf = NULL;

	try
	{
		unsigned int nInLen = sIn.length();
		unsigned int nOutLen = nInLen % 8 ? nInLen + 8 - nInLen % 8 : nInLen;
		nOutLen *= 2;	// BCD -> ASCII需要大一倍的空间
		nOutLen += 1;	// 增加一个结束符空间
	
		pOutBuf = new char[ nOutLen ];
		memset( pOutBuf, 0x00, nOutLen );
		
		sOut = "";
		Des des;
		bOk = des.Encrypt( sIn.c_str(), pOutBuf );
		if( bOk )
		{
			sOut.assign( pOutBuf );
		}

		delete[] pOutBuf;
		pOutBuf = NULL;
	}
	catch( ... )
	{
		if( NULL != pOutBuf )
		{
			delete[] pOutBuf;
			pOutBuf = NULL;
		}

		SvLog::Write( "NnmMethods::desDecrypt - *exception*", 0, sIn.c_str(), __LINE__ );
		cout << "#### NnmMethods::desEncrypt - *exception*" << endl;
		return false;
	}

	cout << "** NnmMethods::desEncrypt - all done! : " << string( bOk ? "success!" : "*failed*" ) << endl;

	return bOk;
}

///////////////////////////////////////////////////////////
// 设备厂商型号维护
// 设备类型：0-三层交换，1-二层交换，2-路由器，3-防火墙，4-服务器，5-PC，6-其他
// StringDict.Keys：Factory, Model, SysOid, DeviceType

// 与“设备厂商型号维护”相关的所有方法未在 Svdb 提供的接口库找到 Benny 2007.8.20
bool NnmMethodsI::WriteFactoryInfo(const ::SliceNNM::STRINGMAPLIST& facInfoList, 
								   const ::Ice::Current& )
{
	return false;
}

///////////////////////////////////////////////////////////
bool NnmMethodsI::GetFactoryInfo(::SliceNNM::STRINGMAPLIST& facInfoList, 
								 const ::Ice::Current& ) const
{
	return false;
}

///////////////////////////////////////////////////////////
// 一个用于测试 Ice 通信的函数, 与具体的业务逻辑无关 Benny 2007.8.20
::Ice::Int NnmMethodsI::sayHello(const ::std::string& sClientMsg, 
								 ::std::string& sServerMsg, 
								 const ::Ice::Current& /* = ::Ice::Current() */ ) const
{
	cout << "==>> Invoke - NnmMethods::sayHello ... ..." << endl;

	static int MSG_COUNT = 0;

	cout << "Client message: " << sClientMsg << endl;

	char buf[ 20 ];
	memset( buf, 0x00, sizeof( buf ) );
	sprintf( buf, "%d", ++MSG_COUNT );

	sServerMsg = "This message is come from server, No: " + string( buf ) + "!";

	cout << "** NnmMethods::sayHello - all done!" << endl;

	return MSG_COUNT;
}
/////////////// End of file