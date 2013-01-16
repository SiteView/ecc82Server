// **********************************************************************
// �ļ�: NnmMethodsI.cpp
// ����: NnmMethodsI ��Ķ���
// ����: Benny 2007.8.6
// 
// Copyright (c) 2003-2007 Dragonflow, Inc. All rights reserved.
//
// **********************************************************************
#include <afx.h>
#include <Windows.h>
#include <Ice/Ice.h>


#include "NnmMethodsI.h"
#include "DatHandleUtils.h"		// ���ݸ�ʽת������
#include "ScanSvdbUtils.h"		// ��ɨ����ص� SVDB ����
#include "LogUtils.h"			// ��־ʵ�ú���

// ʹ�� svdb �еĺ����ͽӿ�
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

const unsigned int MQ_MSG_BUF_SIZE = 4096;	// ��Ϣ��������С

int g_snmpRetries = 1;
int g_snmpTimeout = 200;
int g_snmpVersion = 1;

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

///////////////////////////////////////////////////////////
//  �ӿ�ʹ�÷�����
//	��ȡ Port , EntityRelation , Edge �����������ģʽ
//	EntityEx *pe=reinterpret_cast<EntityEx *>(nnmentityobj);
//  pe->���ַ���();
//  by: �½���(����)
///////////////////////////////////////////////////////////

/********************************************************** 
 ˵���������ݿ�ĵײ�������Ѿ��Բ�����/д�����˿���
       ��ˣ��ڱ��ӿ�����ַ�����ʵ���оͲ����в���
       ���ƣ�����߶�дЧ�ʡ�
 
 ע�ͣ�Benny(CS) 2007.8.7
**********************************************************/

///////////////////////////////////////////////////////////
// ����������
NnmMethodsI::NnmMethodsI()
{
	m_sSvdbRootDir = "C:\\SiteView\\Core";	// ��ʼ����Ŀ¼·��(Ĭ��·��)
	GetRootDirFromSysReg();					// ��ע������·�����ò���

	myWriteLog = new cWriteLog();
	myWriteLog->set_OnlyInDebug( false );	// ��release�汾��Ҳ���log��Ϣ
	

	////////////////////////////////////////////////////////////
	// װ�� readDeviceInfo ��̬��
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
// ȡ�� SVDB �ĸ�Ŀ¼
bool NnmMethodsI::GetRootDirFromSysReg()
{
	bool bOk = false;

	// ��ע����ж�����Ŀ¼·��
	HKEY hkResult = 0;
	LONG lRet = ::RegOpenKeyEx( HKEY_LOCAL_MACHINE, 
								"SOFTWARE\\Siteview\\siteviewcore",		// ��·��
								0,
								KEY_READ, 
								&hkResult );

	if( ERROR_SUCCESS == lRet )
	{	// �򿪼��ɹ�
		DWORD dwType;
		DWORD dwBufSize = 256;
		BYTE bufData[ 256 ];

		memset( bufData, 0x00, sizeof( bufData ) );

		lRet = ::RegQueryValueEx( hkResult, 
								"root_path_7",		// ����
								NULL,
								&dwType,
								bufData,
								&dwBufSize );
		if(  ERROR_SUCCESS == lRet )
		{	// ȡֵ�ɹ�
			if( REG_SZ == dwType )
			{	// ȡ���ַ�����������
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
// Entity ��صĲ�������
///////////////////////////////////////////////////////////
// ȡ�� Entity ����
bool NnmMethodsI::GetEntity(const ::std::string& sEntId, 
							::SliceNNM::Entity& entity, 
							const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::GetEntity - id: " << Utf8ToString( sEntId ) << endl;

	// 1.�� svdb ��ȡ����	
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
	
	// 2.��ȡ�õ�����ת���� sliceNNM ��ʽ
	entity.Identifier	= StringToUtf8( pe->GetIdentifier().getword() );
    entity.Description	= StringToUtf8( pe->GetDescription().getword() );
    entity.Type			= StringToUtf8( pe->GetType().getword() );
    entity.EccEntityID	= StringToUtf8( pe->GetEccEntityID().getword() );

	// ȡ������
	StringMap &prop = pe->GetProperty();	// ȡ�� entity ������
	ENTITYRELATIONLIST &entRelaList = pe->GetRelations();	// ȡ�� entity �Ĺ�ϵ�б�( Relation List )
	PORTMAP &portMap = pe->GetPorts();		// ȡ�� entity �Ķ˿�ӳ��( Port map )
/*
	// ���������ʼ( ת��ǰ������ svdb ԭʼ���� )
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

	// �����������
*/
	// ����ת��
	DatHandleUtils::StringMap2StringDict( prop, entity.Property );	
	DatHandleUtils::EntityRelationList2RelationDict( entRelaList, entity.Relations );	
	DatHandleUtils::PortMap2PortDict( portMap, entity.Ports );
/*
	// ���������ʼ( ת��������� slice �������� )
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
	// �����������
*/
	::CloseNNMEntity( entObj );	//ɾ�����������ڴ�й©

	cout << "** NnmMethods::GetEntity - all done!" << endl;

	return true;
}
///////////////////////////////////////////////////////////
// ȡ������ Entity �����б�
bool NnmMethodsI::GetAllEntityList(const ::std::string& sValue, 
								   ::SliceNNM::StringDict& idList, 
								   const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::GetAllEntityList ... ..." << endl;

	idList.clear();
	///////////////////////////////////////////////////
	// ��SVDB��ȡ�����е� Entity
	PAIRLIST lpRetPairList;
	bool bOk = ::GetAllNNMEntitysInfo( lpRetPairList );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::GetAllEntityList - GetAllNNMEntitysInfo : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetAllEntityList - GetAllNNMEntitysInfo : *false*" << endl;
		return false;
	}

	///////////////////////////////////////////////////
	// ��ȡ�õ�����ת���� slice ��ʽ
	DatHandleUtils::PairList2StringDict( lpRetPairList, idList );

	cout << "** NnmMethods::GetAllEntityList - all done!" << endl;

	return true;
}

///////////////////////////////////////////////////////////
// �޸� Entity ����
bool NnmMethodsI::SubmitEntity(const ::std::string& sEntId , 
							   const ::SliceNNM::Entity& entity, 
							   const ::Ice::Current& )
{
	cout << "==>> Invoke - NnmMethods::SubmitEntity (" << Utf8ToString( sEntId ) << ") ... ..." << endl;

	cout << "EntityID: " << sEntId << endl;
	cout << "entity.Identifier: " << entity.Identifier << endl;

	// ������
	if( Utf8ToString( sEntId ).empty() )
	{
		cout << "*****��NnmMethods::SubmitEntity - Entity ID is empty! *failed*" <<endl;
		return false;
	}

	///////////////////////////////////////////////////
	// 1.�� sEntId �� SVDB �в�ѯ��Ŀ�� Entity
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

	// ���������ʼ( ���յ� slice ���� )
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
	// �����������

	///////////////////////////////////////////////////
	// 2.����Ŀ�� Entity ������
	pe->PutDescription( svutil::word( Utf8ToString( entity.Description ) ) );
	pe->PutEccEntityID( svutil::word( Utf8ToString( entity.EccEntityID ) ) );
	pe->PutIdentifier( svutil::word( Utf8ToString( entity.Identifier ) ) );
	pe->PutType( svutil::word( Utf8ToString( entity.Type ) ) );

	// ���õõ�����,�˿�ӳ��,��ϵ�б�
	StringMap &prop		= pe->GetProperty();
	PORTMAP &portMap	= pe->GetPorts();
	ENTITYRELATIONLIST &relationList = pe->GetRelations();

	// ת������,�˿�ӳ��,��ϵ�б�
	DatHandleUtils::StringDict2StringMap( entity.Property, prop );
	DatHandleUtils::RelationDict2EntityRelationList( entity.Relations, relationList );
	DatHandleUtils::PortDict2PortMap( entity.Ports, portMap );

	// ���������ʼ( ת��ǰ������ svdb ԭʼ���� )
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

	
	// �����������
	

	
	//struct Entity
    //::std::string Identifier;
    //::std::string Description;
    //::std::string Type;
    //::std::string EccEntityID;
    //::SliceNNM::StringDict Property;
    //::SliceNNM::RelationDict Relations;
    //::SliceNNM::PortDict Ports;	

	///////////////////////////////////////////////////
	// 3.��Ŀ�� Entity ���µ�SVDB��
	svutil::word checkid= pe->GetIdentifier();
	cout<<checkid.getword()<<endl;
	if(checkid.empty())
	{
		//����
		cout << "*****��NnmMethods::SubmitEntity - Check Identifier failed *failed*" <<endl;
	}

	bool bOk = ::SubmitNNMEntity( entObj );
	CNNM2ECC tnnm;
	tnnm.SubmitEntityExToEcc(pe);

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::SubmitNNMEntity - SubmitNNMEntity : *failed*", 0, __FILE__, __LINE__ );
	}

	// ���������ʼ
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
	// �����������

	try{
		cout << "---- NnmMethods::SubmitEntity - SubmitNNMEntity : " << string( bOk ? "success!" : "*failed*" ) <<endl;
		cout<< " ----- 828"<<endl;
		// �˴�������SubmitTopologyChartʧ�ܻᵼ�³����Զ��˳�
		// ���óɹ��������!
		// Benny 2007.8.26 ע������һ�д�����е���

		::CloseNNMEntity( entObj );	//ɾ�����������ڴ�й©

		DatHandleUtils::FreePortMap( portMap );		// �ͷŶ˿ڱ��еĶ˿ڶ����ڴ�
	}
	catch(...)
	{
		cout<<"Exeption to CloseNNMEntity"<<endl;
	}
	cout << "** NnmMethods::SubmitEntity - all done!" << endl;

	return bOk;
}

///////////////////////////////////////////////////////////
// ɾ�� Entity ����
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
// ���� Entity �����б�()
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

	// 1.�� SVDB �в�ѯ��idlist
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

	// 2.ת���� slice ��ʽ
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
// ���� Entity �����б�
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

	// 1.�� SVDB �в�ѯ��idlist
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
		// 2.ת���� slice ��ʽ
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
// VirtualGroup ��صĲ�������
bool NnmMethodsI::GetVirtualGroup(const ::std::string& sId, 
								  ::SliceNNM::VirtualGroup& vg, 
								  const ::Ice::Current& ) const
{
	// cout << "==>> Invoke - NnmMethods::GetVirtualGroup ( ";
	// cout << Utf8ToString( sId ) << " ) ... ..." << endl;

	// 1.�� svdb ��ȡ����
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

	// 2.�� svdb ����ת��Ϊ slice ��ʽ
	vg.ID				= StringToUtf8( pVg->GetID().getword() );
	vg.Label			= StringToUtf8( pVg->GetLabel().getword() );
	vg.Description		= StringToUtf8( pVg->GetDescription().getword() );

	StringMap &prop		= pVg->GetProperty();
	WORDLIST &lstOfEnt	= pVg->GetEntityList();

	// typedef std::list<svutil::word>	WORDLIST;
	DatHandleUtils::WORDLIST2StringList( lstOfEnt, vg.EntityList );
	DatHandleUtils::StringMap2StringDict( prop, vg.Property );

	::CloseVirtualGroup( vgObj );	//ɾ�����������ڴ�й©

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

	// 1.�� svdb ��ȡ�� idlist
	bool bOk = ::GetAllVirtualGroupInfo( lpIdList );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::GetAllVirtualGroupList - GetAllVirtualGroupInfo : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetAllVirtualGroupList - GetAllVirtualGroupInfo : *false*" << endl;

		return false;
	}

	// 2.ת���� slice ��ʽ
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
	// 1.�� sId �� SVDB �в�ѯ��Ŀ�� VirtualGroup
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
	// 2.����Ŀ�� Entity ������
	pVg->PutID( Utf8ToString( vg.ID ) );
	pVg->PutLabel( Utf8ToString( vg.Label ) );
	pVg->PutDescription( Utf8ToString( vg.Description ) );
	
	StringMap &prop		= pVg->GetProperty();
	WORDLIST &lstOfEnt	= pVg->GetEntityList();

	// ת������,�˿�ӳ��,��ϵ�б�
	DatHandleUtils::StringDict2StringMap( vg.Property, prop );
	DatHandleUtils::StringList2WORDLIST( vg.EntityList, lstOfEnt );

	///////////////////////////////////////////////////
	// 3.��Ŀ�� Entity ���µ�SVDB��
	bool bOk = ::SubmitVirtualGroup( vgObj );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::SubmitVirtualGroup - SubmitVirtualGroup : *failed*", 0, __FILE__, __LINE__ );
	}

	cout << "---- NnmMethods::SubmitVirtualGroup - SubmitVirtualGroup : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	::CloseVirtualGroup( vgObj );	// ɾ�����������ڴ�й©

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

	// ��ʼ���������
	idlist.clear();

	// �� svdb ��ȡ������
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
		// ת���� slice ��ʽ
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

	// ��ʼ���������
	idlist.clear();

	// �� svdb ��ȡ������
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
		// ת���� slice ��ʽ
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
// TopologyChart ��صĲ�������
bool NnmMethodsI::GetTopologyChart(const ::std::string& sId, 
								   ::SliceNNM::TopologyChart& tc, 
								   const ::Ice::Current& ) const
{
	// cout << "==>> Invoke - NnmMethods::GetTopologyChart ( ";
	// cout << Utf8ToString( sId ) << " ) ... ..." << endl;

	/////////////////////////////////////////////////////////
	// ��SVDB��ȡ�����ݶ���
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
	// �����ݶ���ת����slice���ݸ�ʽ
	tc.ID			= StringToUtf8( pTc->GetID().getword() );
	tc.Description	= StringToUtf8( pTc->GetDescription().getword() );
	tc.CreateTime	= StringToUtf8( pTc->GetCreateTime().Format() );

	EDGELIST &lstEdg	= pTc->GetEdges();
	StringMap &prop		= pTc->GetProperty();

	DatHandleUtils::StringMap2StringDict( prop, tc.Property );
	DatHandleUtils::EDGELISTSvdb2Slice( lstEdg, tc.Edges );

	::CloseTopologyChart( tcObj );	// ɾ�����������ڴ�й©

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
	// �� svdb ���ҵ�����
	PAIRLIST lpRet;
	bool bOk = ::GetAllTopologyChartInfo( lpRet );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::GetTopologyChartList - GetAllTopologyChartInfo : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetTopologyChartList - GetAllTopologyChartInfo : *false*" << endl;

		return false;
	}

	/////////////////////////////////////////////////////////
	// ת���� slice ��ʽ
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
	// 1.�� sId �� SVDB �в�ѯ��Ŀ�� TopologyChart
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
	// 2.����Ŀ�� Entity ������
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
	// 3.��Ŀ�� Entity ���µ�SVDB��
	bool bOk = ::SubmitTopologyChart( tcObj );

	if( !bOk )
	{
		SvLog::Write( "NnmMethods::SubmitTopologyChart - SubmitTopologyChart : *failed*", 0, __FILE__, __LINE__ );
	}

	cout << "---- NnmMethods::SubmitTopologyChart - SubmitTopologyChart : " << string( bOk ? "success!" : "*failed*" ) <<endl;

	// �˴�������SubmitTopologyChartʧ�ܻᵼ�³����Զ��˳�
	// ���óɹ��������!
	// Benny 2007.8.26 ע������һ�д�����е���
	// ::CloseTopologyChart( tcObj );//ɾ�����������ڴ�й©

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

	// ��ʼ���������
	idList.clear();

	// �� svdb ��ȡ������
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
		// ת���� slice ��ʽ
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

	// ��ʼ���������
	idList.clear();

	// �� svdb ��ȡ������
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
		// ת���� slice ��ʽ
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
// EntityTemplet ��صĲ�������
bool NnmMethodsI::GetEntityTemplet(const ::std::string& sId, 
								   ::SliceNNM::EntityTemplet& et, 
								   const ::Ice::Current& ) const
{
	// cout << "==>> Invoke - NnmMethods::GetEntityTemplet ( ";
	// cout << Utf8ToString( sId ) << " ) ... ..." << endl;

	/////////////////////////////////////////////////////////
	// ��SVDB��ȡ�����ݶ���
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
	// �����ݶ���ת����slice���ݸ�ʽ
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
	
	::CloseEntityGroup( etObj );//ɾ�����������ڴ�й©

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

	idList.clear();		// ��ʼ���������

	// �� svdb �л�ȡ����
	bool bOk = ::GetAllEntityTemplets( retlist, 
									Utf8ToString( sValue ) );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::GetEntityTempletList - GetAllEntityTemplets : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetEntityTempletList - GetAllEntityTemplets : *false*" << endl;

		return false;
	}

	// ���ݸ�ʽת�� svdb => slice
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
	// ��SVDB��ȡ�����ݶ���
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
	// �����ݶ���ת���� SVDB ���ݸ�ʽ
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

	::CloseEntityTemplet( etObj );	// ɾ�����������ڴ�й©

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
// Ini �����ļ���صĲ�������
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
// MQ ��صĲ�������
///////////////////////////////////////////////////////////
::Ice::Int NnmMethodsI::CreateQueue(const ::std::string& sQueName, 
									::Ice::Int nType, 
									const ::Ice::Current& )
{
	cout << "==>> Invoke - NnmMethods::CreateQueue ( ";
	cout << Utf8ToString( sQueName ) << " ) ... ..." << endl;
	nType = 1;

	bool bOk = ::CreateQueue( Utf8ToString( sQueName ), 
							nType );	// ���type�������ȡ1���� svdbapi.h

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
		{	// ȡ�����б�ɹ�, �������б��Ƶ��������
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
// ɨ����ط���
///////////////////////////////////////////////////////////
bool NnmMethodsI::StartScan(const ::SliceNNM::ScanConfig& sc, 
							const ::std::string& sQueName, 
							const ::Ice::Current& )
{
	cout << "==>> Invoke - NnmMethods::StartScan ( ";
	cout << Utf8ToString( sQueName ) << " ) ... ..." << endl;

	ScanParameter sp;
	DatHandleUtils::ScanParaSlice2Svdb( sc, sp );
	
	GetRootDirFromSysReg();						// ע����ж�ȡSVDB·Ŀ¼·��
	ScanUtils::SetRootDir( m_sSvdbRootDir );	// ���� SVDB ��Ŀ¼·���� Scan ��

	bool bOk = false;
	// ������ɨ����̹����� Queue
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

	// ����ɨ�����
	bOk = ScanUtils::StartScan( sp,			// ����ɨ��
								Utf8ToString( sQueName ),
								false,			// ���Ѿ�����,�Ͳ���������
								false );		// ���첽��ʽ

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
// DB ��������
///////////////////////////////////////////////////////////
bool NnmMethodsI::GetSVDYN(const ::std::string& sMonId, 
						   ::SliceNNM::SVDYN& dyn, 
						   const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::GetSVDYN ( ";
	cout << Utf8ToString( sMonId ) << " ) ... ..." << endl;

	SVDYN dynSvdb;

	// �� svdb �ж��� dyn
	bool bOk = ::GetSVDYN( Utf8ToString( sMonId ), dynSvdb );
	if( !bOk )
	{
		SvLog::Write( "NnmMethods::GetSVDYN - GetSVDYN : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetSVDYN - GetSVDYN : *failed*" << endl;

		return false;
	}

	// ת���� slice ��ʽ
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

	// ����ѯ����ת���� svdb ��ʽ
	TTimeSpan timeSpan;
	TTime tmBegin;
	TTime tmEnd;

	// ����ѯ����ת��Ϊ Svdb ��ʽ
	DatHandleUtils::String2TTime( sBeginTime, tmBegin );
	DatHandleUtils::String2TTime( sEndTime, tmEnd );

	// �� Svdb ���ݿ��в�ѯ����¼
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

	// ����¼��ʽ�� Svdb ��ʽת��Ϊ Slice ��ʽ
	DatHandleUtils::RecordSetSvdb2Slice( *pRsSvdb, rs );

	cout << "** NnmMethods::QueryRecords - all done!" << endl;

	return true;
}

///////////////////////////////////////////////////////////
// SNMP ��صĲ�������
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
	//	myWriteLog->WriteLog("���� entity ʧ�ܣ�");
	//	return false;
	//}

	// Get Entity
	OBJECT obj= GetNNMEntity(sid);
	if(obj==INVALID_VALUE)
	{
		myWriteLog->WriteLog("GetNNMEntity ʧ�ܣ�");
		return false;
	}
	EntityEx *pEn=reinterpret_cast<EntityEx *>(obj);
	if(pEn==NULL)
	{
		myWriteLog->WriteLog("��ȡ entity ʧ�ܣ�");
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
		myWriteLog->WriteLog("д Mib �ɹ���");
		return true;
	}
	else
	{
		myWriteLog->WriteLog("д Mib ʧ�ܣ�");
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
		myWriteLog->WriteLog("GetNNMEntity ʧ�ܣ�");
		return false;
	}
	EntityEx *pEn=reinterpret_cast<EntityEx *>(obj);
	if(pEn==NULL)
	{
		myWriteLog->WriteLog("��ȡ entity ʧ�ܣ�");
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
		myWriteLog->WriteLog("IP ��ַ����.");
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
		sprintf(pBuf, "���� ip:%s",tp.ip);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "���� indic:%d",tp.indicatorIndex);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "���� istable:%d",tp.isTable);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "���� sysOID:%s",tp.sysOid);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "���� oid:%s",tp.oid);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "���� readComm:%s",tp.readCommunity);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "���� retries:%d",tp.retries);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "���� timeout:%d",tp.timeout);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "���� version:%d",tp.version);
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
			myWriteLog->WriteLog("����Ϣ�����쳣��");
			CloseNNMEntity(obj);
			return false;
		}

		if(ret == -1 && nRecNo > realCount)				// pTmb ����Ŀռ�С��ʵ��Ҫ��
		{
			myWriteLog->WriteLog("�������ݳ���1000�У����·���ռ䡣");
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
			ret = m_TstartGetDeviceInfo(tp,nRecNo,nColNo, pTmb);	// ���»�ȡһ��
			*/
			nRecNo = realCount;
			ret = m_TstartGetDeviceInfo(tp,nRecNo,nColNo, myMibData);	// ���»�ȡһ��
		}

		if(ret<=0)
		{
			/*
			for( i=0;i<realCount;i++)
				delete pTmb->fields[i].field;
			delete pTmb->fields;
			*/
			delete myMibData;

			myWriteLog->WriteLog("SnmpGetValue û��ȡ����Ϣ��");
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
		myWriteLog->WriteLog("SnmpGetValue �����쳣��.");
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
		myWriteLog->WriteLog("GetNNMEntity ʧ�ܣ�");
		return false;
	}
	EntityEx *pEn=reinterpret_cast<EntityEx *>(obj);
	if(pEn==NULL)
	{
		myWriteLog->WriteLog("��ȡ entity ʧ�ܣ�");
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
		myWriteLog->WriteLog("ȡ��ͬ�����");
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
		myWriteLog->WriteLog("ȡIP��ַ����");
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
		sprintf(pBuf, "���� ip:%s",tp.ip);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "���� indic:%d",tp.indicatorIndex);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "���� istable:%d",tp.isTable);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "���� sysOID:%s",tp.sysOid);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "���� oid:%s",tp.oid);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "���� readComm:%s",tp.readCommunity);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "���� retries:%d",tp.retries);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "���� timeout:%d",tp.timeout);
		myWriteLog->WriteLog(pBuf);
		sprintf(pBuf, "���� version:%d",tp.version);
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
			myWriteLog->WriteLog("����Ϣ�����쳣��");
			CloseNNMEntity(obj);
			cout << "DEBUG - Line : " << __LINE__ << endl;
			return false;
		}

		// cout << "DEBUG - Line : " << __LINE__ << endl;
		if(ret == -1 && nRecNo > realCount)
		{
			myWriteLog->WriteLog("�������ݳ���1000�У����·���ռ䡣");

			delete []myMibData;
			realCount = nRecNo+1;

			myMibData = new TmibData[realCount];

			nRecNo = realCount;
			ret = m_TstartGetDeviceInfo(tp,nRecNo,nColNo, myMibData);		// ���»�ȡһ��
		}

		// cout << "DEBUG - Line : " << __LINE__ << endl;
		if(ret<=0)
		{
			if(myMibData != NULL)
				delete []myMibData;

			myWriteLog->WriteLog("SnmpGetValueEx û��ȡ����Ϣ��");
			
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
		myWriteLog->WriteLog("SnmpGetValueEx �����쳣!.");
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
// Monitor��صĲ�������
///////////////////////////////////////////////////////////
bool NnmMethodsI::GetMonitorTemplete(const ::std::string& sMonId, 
									 ::SliceNNM::Monitor& mon, 
									 const ::Ice::Current& ) const
{
	cout << "==>> Invoke - NnmMethods::GetMonitorTemplete ( ";
	cout << Utf8ToString( sMonId ) << " ) ... ..." << endl;
	
	// �� svdb ��ȡ�ö�������
	OBJECT	mtObj = ::GetMonitorTemplet( atoi( Utf8ToString( sMonId ).c_str() ) );
	if( mtObj == NULL )
	{
		SvLog::Write( "NnmMethods::GetMonitorTemplete - GetMonitorTemplet : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetMonitorTemplete - GetMonitorTemplet : *failed*" << endl;

		return false;
	}

	// ת���ɶ�Ӧ�������
	MonitorTemplet *pMt = reinterpret_cast< MonitorTemplet * >( mtObj );
	if( NULL == pMt )
	{
		SvLog::Write( "NnmMethods::GetMonitorTemplete - reinterpret_cast : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetMonitorTemplete - reinterpret_cast : *failed*" << endl;

		return false;
	}

	// �� svdb ���ݸ�ʽת���� slice ��ʽ
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

	::CloseMonitorTemplet( mtObj );	// ɾ�����������ڴ�й©

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

	// �� svdb ��ȡ�ö�������
	OBJECT	monObj = ::CreateMonitor();
	if( monObj == NULL )
	{
		SvLog::Write( "NnmMethods::SubmitMonitor - CreateMonitor : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::SubmitMonitor - CreateMonitor : *failed*" << endl;

		return false;
	}

	// ת���ɶ�Ӧ�������
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
		// �� slice ���ݸ�ʽת���� svdb ��ʽ
		pMon->PutID( Utf8ToString( mon.monitorID ) );	// ID��	
		
		// ǿ������ת���ɷ� const ����
		::SliceNNM::Monitor &monRef = const_cast< ::SliceNNM::Monitor &>( mon );

		StringMap &prop		= pMon->GetProperty();
		StringMap &para		= pMon->GetParameters();
		StringMap &adPara	= pMon->GetADParameters();
		StringMap *altCond	= pMon->GetAlertCondition();

		// ����	
		prop.clear();
		for( ::SliceNNM::STRINGMAPLIST::iterator it = monRef.Property.begin(); 
			it != monRef.Property.end(); it++ )
		{
			DatHandleUtils::StringDict2StringMap( *it, prop );
			break;
		}	

		// ����	
		para.clear();
		for( ::SliceNNM::STRINGMAPLIST::iterator it = monRef.Parameter.begin(); 
			it != monRef.Parameter.end(); it++ )
		{
			DatHandleUtils::StringDict2StringMap( *it, para );
			break;
		}

		// �߼�����
		adPara.clear();
		for( ::SliceNNM::STRINGMAPLIST::iterator it = 
				monRef.AdvanceParameter.begin(); 
			it != monRef.AdvanceParameter.end(); it++ )
		{
			DatHandleUtils::StringDict2StringMap( *it, adPara );
			break;
		}

		// ��ѡ����
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

	:: CloseMonitor( monObj );	// ɾ�����������ڴ�й©

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

	// �� svdb ��ȡ�ö�������
	OBJECT	monObj = ::GetMonitor( Utf8ToString( sEntId ) );
	if( monObj == NULL )
	{
		SvLog::Write( "NnmMethods::GetMonitor - GetMonitor : *failed*", 0, __FILE__, __LINE__ );
		cout << "#### NnmMethods::GetMonitor - GetMonitor : *failed*" << endl;

		return false;
	}

	// ת���ɶ�Ӧ�������
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
		// �� svdb ���ݸ�ʽת���� slice ��ʽ
		mon.monitorID = StringToUtf8( pMon->GetID().getword() );	// ID��	
		
		::SliceNNM::StringDict sdSlice;

		mon.Property.clear();
		mon.Parameter.clear();
		mon.AdvanceParameter.clear();
		mon.AlertCondition.clear();

		// ����
		StringMap &prop = pMon->GetProperty();	
		DatHandleUtils::StringMap2StringDict( prop, sdSlice );	
		mon.Property.push_back( sdSlice );

		// ����
		StringMap &para = pMon->GetParameters();
		DatHandleUtils::StringMap2StringDict( para, sdSlice );	
		mon.Parameter.push_back( sdSlice );

		// �߼�����
		StringMap &adPara = pMon->GetADParameters();
		DatHandleUtils::StringMap2StringDict( adPara, sdSlice );	
		mon.AdvanceParameter.push_back( sdSlice );

		// ��ѡ����
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

	:: CloseMonitor( monObj );	// ɾ�����������ڴ�й©

	cout << "** NnmMethods::GetMonitor - all done!" << endl;

	return bOk;
}

///////////////////////////////////////////////////////////
// DES ��/����
///////////////////////////////////////////////////////////
// ����
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
		nOutLen += 1;	// ����һ���������ռ�
		
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
// ����
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
		nOutLen *= 2;	// BCD -> ASCII��Ҫ��һ���Ŀռ�
		nOutLen += 1;	// ����һ���������ռ�
	
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
// �豸�����ͺ�ά��
// �豸���ͣ�0-���㽻����1-���㽻����2-·������3-����ǽ��4-��������5-PC��6-����
// StringDict.Keys��Factory, Model, SysOid, DeviceType

// �롰�豸�����ͺ�ά������ص����з���δ�� Svdb �ṩ�Ľӿڿ��ҵ� Benny 2007.8.20
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
// һ�����ڲ��� Ice ͨ�ŵĺ���, ������ҵ���߼��޹� Benny 2007.8.20
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