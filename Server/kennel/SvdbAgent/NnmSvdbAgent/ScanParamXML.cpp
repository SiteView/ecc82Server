/********************************************
 * �ļ�: ScanParamXml.cpp
 * ����: ScanParamXml �ඨ���ļ�
 * ע��: ʹ�� MSXML 4.0 SDK, ע��ͷ�ļ���lib���ļ���·������
 * ����: Benny 2007.8.23
 * �޸�: (���¼�޸���Ϣ)
********************************************/

#include <objbase.h>		// COM ����ͷ�ļ�
#include <wtypes.h>
#include <tchar.h>

#include <iostream>

#include "ScanParamXML.h"	// ����������ļ�

//using namespace MSXML2;

#define XML_TRUE		"True"		// XML �ĵ��е� bool true
#define XML_FALSE		"False"		// XML �ĵ��е� bool false

bool ScanParamXML::m_bIsInit = false;	// ��̬��Ա����,��ʼ��

///////////////////////////////////////////////////////////////
// ��ʼ������s
bool ScanParamXML::Init()
{
	bool bOk = true;

	if( !m_bIsInit )
	{	// ��δ��ʼ��
		HRESULT hr = ::CoInitialize( NULL );	// ��ʼ��COM���� 
		bOk = ( S_OK == hr );
		m_bIsInit = bOk;
	}

	return bOk;
}

// �ͷŻ���
void ScanParamXML::Release()
{
	::CoUninitialize();		// �ͷ�COM����
	m_bIsInit = false;
}

// �����Ƿ��Ѿ���ʼ��
bool ScanParamXML::IsInit()		
{
	return m_bIsInit;
}

///////////////////////////////////////////////////////////////
// ����: ɨ������ṹת���� XML �ĵ�
// ����: sp				[in]	ɨ������ṹ
//		 sXMLDocText	[out]	XML�ĵ����ı�
// ����: true - �ɹ�, false - ʧ��
bool ScanParamXML::ScanParamToXML( const ScanParameter &sp, string &sXMLDocText )
{
	try
	{
		IXMLDOMDocument *pXmlDoc = NULL;
		IXMLDOMNode *pXmlDomNode = NULL;
			
		// �õ�����IXMLDOMDocument �ӿڵ�ָ�� pXMLDOC
		HRESULT hr = ::CoCreateInstance( CLSID_DOMDocument,
										NULL, 
										CLSCTX_INPROC_SERVER,
										IID_IXMLDOMDocument, 
										( void ** )&pXmlDoc );
		if( S_OK != hr )
		{
			return false;
		}

		//�õ�����IXMLDOMNode�ӿڵ�ָ��pXDN 
		hr = pXmlDoc->QueryInterface( IID_IXMLDOMNode,
									( void ** )&pXmlDomNode );
		if( S_OK != hr )
		{
			return false;
		}


		IXMLDOMNode *pNode;
		IXMLDOMNode *pNodeOut;
		IXMLDOMNode *pRootNode;		

		_variant_t varNodeType = NODE_ELEMENT;
		_variant_t varNodeVal;
		_bstr_t bstrName; 
		_bstr_t bstrDataType = NODE_CDATA_SECTION ;


		// ���������
		bstrName = _T("TopDiscoverParas");		
		hr = pXmlDoc->createNode( varNodeType , bstrName, 0, &pNode );
		pXmlDoc->appendChild( pNode, &pRootNode );

		// ����Depth���
		varNodeVal = sp.nDepth;
		AddChildNodeWithValue( pRootNode, pXmlDoc, "Depth", varNodeVal );

		// ����Tineout���
		varNodeVal = sp.nTimeout;
		AddChildNodeWithValue( pRootNode, pXmlDoc, "Timeout", varNodeVal );

		// ����TryTimes���
		varNodeVal = sp.nTryTimes;
		AddChildNodeWithValue( pRootNode, pXmlDoc, "TryTimes", varNodeVal );

		// ����SubScan���
		varNodeVal = sp.bSubScan ? XML_TRUE : XML_FALSE;
		AddChildNodeWithValue( pRootNode, pXmlDoc, "SubScan", varNodeVal );

		// ����MaxTreadCount���
		varNodeVal = sp.nMaxTreadCount;
		AddChildNodeWithValue( pRootNode, pXmlDoc, "MaxTreadCount", varNodeVal );

		// ����DefaultCommunityGet���
		varNodeVal = sp.sDefaultCommunityOfGet.c_str();
		AddChildNodeWithValue( pRootNode, pXmlDoc, "DefaultCommunityGet", varNodeVal );

		// ����DefaultCommunitySet���
		varNodeVal = sp.sDefaultCommunityOfSet.c_str();
		AddChildNodeWithValue( pRootNode, pXmlDoc, "DefaultCommunitySet", varNodeVal );
		

		// CommunityList ��������
		if( sp.vcCommunityList.size() > 0 )
		{
			VectorOfCommunity &vComm = const_cast< VectorOfCommunity & >( sp.vcCommunityList );
			pNodeOut = AddChildElem( pRootNode, pXmlDoc, "Communitys" );
			for( VectorOfCommunity::iterator it = vComm.begin();
				pNodeOut != NULL && it != vComm.end(); it++ )
			{
				IXMLDOMNode *pNodeCommunity = AddChildElem( pNodeOut, pXmlDoc, "Community" );

				_variant_t varNodeVal = it->sStartIp.c_str();
				AddChildNodeWithValue( pNodeCommunity, pXmlDoc, "StartIp", varNodeVal );
				varNodeVal = it->sEndIp.c_str();
				AddChildNodeWithValue( pNodeCommunity, pXmlDoc, "EndIp", varNodeVal );
				varNodeVal = it->sGet.c_str();
				AddChildNodeWithValue( pNodeCommunity, pXmlDoc, "Get", varNodeVal );
				varNodeVal = it->sSet.c_str();
				AddChildNodeWithValue( pNodeCommunity, pXmlDoc, "Set", varNodeVal );
			}
		}

		// IpScopeList ��������
		if( sp.viIpScopeList.size() > 0 )
		{
			VectorOfIpScope &vIpScope = const_cast< VectorOfIpScope & >( sp.viIpScopeList );
			pNodeOut = AddChildElem( pRootNode, pXmlDoc, "AddStartEnds" );
			for( VectorOfIpScope::iterator it = vIpScope.begin();
				pNodeOut != NULL && it != vIpScope.end(); it++ )
			{
				IXMLDOMNode *pNodeIpScope = AddChildElem( pNodeOut, pXmlDoc, "AddStartEnd" );

				_variant_t varNodeVal = it->sIpBegin.c_str();
				AddChildNodeWithValue( pNodeIpScope, pXmlDoc, "StartIp", varNodeVal );
				varNodeVal = it->sIpEnd.c_str();
				AddChildNodeWithValue( pNodeIpScope, pXmlDoc, "EndIp", varNodeVal );
			}
		}

		// FilterStartEndList ��������
		if( sp.viFilterStartEndList.size() > 0 )
		{
			VectorOfIpScope &vIpScope = const_cast< VectorOfIpScope & >( sp.viFilterStartEndList );
			pNodeOut = AddChildElem( pRootNode, pXmlDoc, "FilterStartEnds" );
			for( VectorOfIpScope::iterator it = vIpScope.begin();
				pNodeOut != NULL && it != vIpScope.end(); it++ )
			{
				IXMLDOMNode *pNodeIpScope = AddChildElem( pNodeOut, pXmlDoc, "FilterStartEnd" );

				_variant_t varNodeVal = it->sIpBegin.c_str();
				AddChildNodeWithValue( pNodeIpScope, pXmlDoc, "StartIp", varNodeVal );
				varNodeVal = it->sIpEnd.c_str();
				AddChildNodeWithValue( pNodeIpScope, pXmlDoc, "EndIp", varNodeVal );
			}
		}		

		// SubnetList ��������
		if( sp.viSubnetList.size() > 0 )
		{
			VectorOfIpScope &vIpScope = const_cast< VectorOfIpScope & >( sp.viSubnetList );
			pNodeOut = AddChildElem( pRootNode, pXmlDoc, "SubnetList" );
			for( VectorOfIpScope::iterator it = vIpScope.begin();
				pNodeOut != NULL && it != vIpScope.end(); it++ )
			{
				IXMLDOMNode *pNodeIpScope = AddChildElem( pNodeOut, pXmlDoc, "Subnet" );

				_variant_t varNodeVal = it->sIpBegin.c_str();
				AddChildNodeWithValue( pNodeIpScope, pXmlDoc, "StartIp", varNodeVal );
				varNodeVal = it->sIpEnd.c_str();
				AddChildNodeWithValue( pNodeIpScope, pXmlDoc, "EndIp", varNodeVal );
			}
		}

		// SeedIpList ��������
		if( sp.vsSeedIpList.size() > 0 )
		{
			VectorOfString &vIpSeed = const_cast< VectorOfString & >( sp.vsSeedIpList );
			pNodeOut = AddChildElem( pRootNode, pXmlDoc, "SeedIpList" );
			for( VectorOfString::iterator it = vIpSeed.begin();
				pNodeOut != NULL && it != vIpSeed.end(); it++ )
			{
				_variant_t varNodeVal = (*it).c_str();
				AddChildNodeWithValue( pNodeOut, pXmlDoc, "Seed", varNodeVal );				
			}
		}

		// �����������
		BSTR bstrXml;
		pXmlDoc->get_xml( &bstrXml );
		bstrName = bstrXml;

		sXMLDocText = bstrName.operator char *();

		SysFreeString( bstrXml );

		// ���浽�ļ�(���ڲ���)
		// _variant_t varString = _T("C:\\sample.xml");
		// pXmlDoc->save( varString );
	} 
	catch(...)
	{
		return false;
	}

	return true;
}


///////////////////////////////////////////////////////////////
// ����: XML �ĵ��ı�ת����ɨ������ṹ
// ����: sXMLDocText	[in]	XML�ĵ����ı�
//		 sp				[out]	ɨ������ṹ
// ����: true - �ɹ�, false - ʧ��
bool ScanParamXML::XMLToScanParam( const string &sXMLDocText, ScanParameter &sp )
{
	try
	{
		IXMLDOMDocument *pXmlDoc = NULL;
		IXMLDOMNode *pXmlDomNode = NULL;
			
		// �õ�����IXMLDOMDocument �ӿڵ�ָ�� pXMLDOC
		HRESULT hr = ::CoCreateInstance( CLSID_DOMDocument,
										NULL, 
										CLSCTX_INPROC_SERVER,
										IID_IXMLDOMDocument, 
										( void ** )&pXmlDoc );
		if( S_OK != hr )
		{
			return false;
		}

		//�õ�����IXMLDOMNode�ӿڵ�ָ��pXDN 
		hr = pXmlDoc->QueryInterface( IID_IXMLDOMNode,
									( void ** )&pXmlDomNode );
		if( S_OK != hr )
		{
			return false;
		}
		
		VARIANT_BOOL bOk;
		_variant_t varSrc = sXMLDocText.c_str();
		IXMLDOMElement *pRootNode;	// �����
		IXMLDOMNode *pNode;
		IXMLDOMNode *pNodeOut;
		
		pXmlDoc->loadXML( varSrc.bstrVal, &bOk  );	// ���ַ�������XML�ĵ�

		// ȡ�ø����
		pXmlDoc->get_documentElement( &pRootNode );		
		pRootNode->get_nodeName( &varSrc.bstrVal );

		// �����������ӽ��
		for( pRootNode->get_firstChild( &pNode );
			pNode != NULL; pNode->get_nextSibling( &pNode ) )
		{
			pNode->get_nodeName( &varSrc.bstrVal );
			// cout << "Node name: " << varSrc.operator _bstr_t().operator char *() << endl;

			string sNodeName = _bstr_t( varSrc.bstrVal ).operator char *();
			if( sNodeName == string( "Depth" ) )
			{	// ���� Depth ���
				varSrc = ReadNodeText( pNode );
				sp.nDepth = atoi( _bstr_t( varSrc.bstrVal ).operator char *() );

				//cout << "Depth value: " << sp.nDepth << endl;
			}
			else if( sNodeName == string( "TryTimes" ) )
			{	// ���� TryTimes ���
				varSrc = ReadNodeText( pNode );
				sp.nTryTimes = atoi( _bstr_t( varSrc.bstrVal ).operator char *() );

				//cout << "TryTimes value: " << sp.nTryTimes << endl;
			}	
			else if( sNodeName == string( "Timeout" ) )
			{	// ���� Timeout ���
				varSrc = ReadNodeText( pNode );
				sp.nTimeout = atoi( _bstr_t( varSrc.bstrVal ).operator char *() );

				//cout << "Timeout value: " << sp.nTimeout << endl;
			}		
			else if( sNodeName == string( "MaxTreadCount" ) )
			{	// ���� MaxTreadCount ���
				varSrc = ReadNodeText( pNode );
				sp.nMaxTreadCount = atoi( _bstr_t( varSrc.bstrVal ).operator char *() );

				//cout << "MaxTreadCount value: " << sp.nMaxTreadCount << endl;
			}
			else if( sNodeName == string( "SubScan" ) )
			{	// ���� SubScan ���
				varSrc = ReadNodeText( pNode );
				sp.bSubScan = ( string( _bstr_t( varSrc.bstrVal ).operator char *() ) == XML_TRUE ) ;

				//cout << "SubScan value: " << sp.bSubScan << endl;
			}		
			else if( sNodeName == string( "DefaultCommunityGet" ) )
			{	// ���� DefaultCommunityGet ���
				varSrc = ReadNodeText( pNode );
				sp.sDefaultCommunityOfGet = _bstr_t( varSrc.bstrVal ).operator char *();

				//cout << "DefaultCommunityGet value: " << sp.sDefaultCommunityOfGet << endl;
			}		
			else if( sNodeName == string( "DefaultCommunitySet" ) )
			{	// ���� DefaultCommunitySet ���
				varSrc = ReadNodeText( pNode );
				sp.sDefaultCommunityOfSet = _bstr_t( varSrc.bstrVal ).operator char *();

				//cout << "DefaultCommunityOfSet value: " << sp.sDefaultCommunityOfSet << endl;
			}		
			else if( sNodeName == string( "Communitys" ) )
			{	// ���� Communitys ���
				ReadCommunityListNode( pNode, sp.vcCommunityList );
			}		
			else if( sNodeName == string( "AddStartEnds" ) )
			{	// ���� AddStartEnds ���
				ReadIpScopeListNode( pNode, sp.viIpScopeList );	// ��IpScopeList���
			}		
			else if( sNodeName == string( "FilterStartEnds" ) )
			{	// ���� FilterStartEnds ���
				ReadIpScopeListNode( pNode, sp.viFilterStartEndList );	// ��IpScopeList���
			}		
			else if( sNodeName == string( "SubnetList" ) )
			{	// ���� SubnetList ���
				ReadIpScopeListNode( pNode, sp.viSubnetList );	// ��IpScopeList���
			}		
			else if( sNodeName == string( "SeedIpList" ) )
			{	// ���� SeedIp ���
				ReadStringListNode( pNode, sp.vsSeedIpList );
			}
		}		
		
		// cout << "XMLToScanParam done successfully!" << endl;
	} 
	catch(...)
	{
		return false;
	}
	return true;
}


///////////////////////////////////////////////////////////////
// �ǹ�������
///////////////////////////////////////////////////////////////
// �����Text(���ݽ��)
_variant_t ScanParamXML::ReadNodeText( IXMLDOMNode *const pTheNode )
{
	_variant_t varVal = 0;
	try
	{
		IXMLDOMNode *pTextNode;
		VARIANT_BOOL bOk;
		pTheNode->hasChildNodes( &bOk );
		if( bOk )
		{	// ���ӽ��(��Text���,����������)
			pTheNode->get_firstChild( &pTextNode );
			pTextNode->get_nodeValue( &varVal );
		}
	}
	catch( ... )
	{
		// �޴���
	}

	return varVal;
}

///////////////////////////////////////////////////////////////
// ��CommunityList���
bool ScanParamXML::ReadCommunityListNode( IXMLDOMNode *const pTheNode, VectorOfCommunity &vCommunity )
{
	if( NULL == pTheNode )
	{
		return false;
	}
	try
	{
		vCommunity.clear();		// ��ʼ���������

		_variant_t varVal = 0;
		IXMLDOMNode *pCommunityNode, *pTextNode;
		for( pTheNode->get_firstChild( &pCommunityNode ); pCommunityNode != NULL; 
			pCommunityNode->get_nextSibling( &pCommunityNode  ) )
		{
			Community community;

			for( pCommunityNode->get_firstChild( &pTextNode ); pTextNode != NULL;
				pTextNode->get_nextSibling( &pTextNode ) )
			{	
				_variant_t varNodeName;
				pTextNode->get_nodeName( &varNodeName.bstrVal );
				varVal = ReadNodeText( pTextNode );
				string sNodeName = _bstr_t( varNodeName.bstrVal ).operator char *();;
				if( sNodeName == string( "StartIp" ) )
				{
					community.sStartIp = _bstr_t( varVal.bstrVal ).operator char *();
				}
				else if( sNodeName == string( "EndIp" ) )
				{
					community.sEndIp = _bstr_t( varVal.bstrVal ).operator char *();
				}
				else if( sNodeName == string( "Get" ) )
				{
					community.sGet = _bstr_t( varVal.bstrVal ).operator char *();
				}
				else if( sNodeName == string( "Set" ) )
				{
					community.sSet = _bstr_t( varVal.bstrVal ).operator char *();
				}				
			}
			//cout << "Start IP: " << community.sStartIp << ", End Ip :" << community.sEndIp << ", Get: " << community.sGet << ", Set: " << community.sSet << endl;
			vCommunity.push_back( community );
		}

	}
	catch( ... )
	{
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////
// ��IpScopeList���
bool ScanParamXML::ReadIpScopeListNode( IXMLDOMNode *const pTheNode, VectorOfIpScope &vIpScope )
{
	if( NULL == pTheNode )
	{
		return false;
	}
	try
	{
		vIpScope.clear();		// ��ʼ���������

		_variant_t varVal = 0;
		IXMLDOMNode *pIpScopeNode, *pTextNode;
		for( pTheNode->get_firstChild( &pIpScopeNode ); pIpScopeNode != NULL; 
			pIpScopeNode->get_nextSibling( &pIpScopeNode  ) )
		{
			IPScope ipScope;

			for( pIpScopeNode->get_firstChild( &pTextNode ); pTextNode != NULL;
				pTextNode->get_nextSibling( &pTextNode ) )
			{	
				_variant_t varNodeName;
				pTextNode->get_nodeName( &varNodeName.bstrVal );
				varVal = ReadNodeText( pTextNode );
				string sNodeName = _bstr_t( varNodeName.bstrVal ).operator char *();;
				if( sNodeName == string( "StartIp" ) )
				{
					ipScope.sIpBegin = _bstr_t( varVal.bstrVal ).operator char *();
				}
				else if( sNodeName == string( "EndIp" ) )
				{
					ipScope.sIpEnd = _bstr_t( varVal.bstrVal ).operator char *();
				}				
			}
			//cout << "Start IP: " << ipScope.sIpBegin << ", End Ip :" << ipScope.sIpEnd << endl;
			vIpScope.push_back( ipScope );
		}
	}
	catch( ... )
	{
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////
// ��StringList���
bool ScanParamXML::ReadStringListNode( IXMLDOMNode *const pTheNode, VectorOfString &vStr )
{
	if( NULL == pTheNode )
	{
		return false;
	}
	try
	{
		vStr.clear();		// ��ʼ���������

		_variant_t varVal = 0;
		IXMLDOMNode *pIpSeedNode;
		for( pTheNode->get_firstChild( &pIpSeedNode ); pIpSeedNode != NULL; 
			pIpSeedNode->get_nextSibling( &pIpSeedNode  ) )
		{			
			varVal = ReadNodeText( pIpSeedNode );
			
			//cout << "Seed IP: " << _bstr_t( varVal.bstrVal ).operator char *() << endl;
			
			vStr.push_back( _bstr_t( varVal.bstrVal ).operator char *() );
		}
	}
	catch( ... )
	{
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////
// ����һ���ӽ��(�����ݽ��)
IXMLDOMNode * ScanParamXML::AddChildElem( IXMLDOMNode *const pParentNode,	
									IXMLDOMDocument *const pXmlDoc,
									const string &sElemName )
{
	IXMLDOMNode *pNode = NULL, *pNode2 = NULL;
	try
	{
		
		_variant_t varNodeType = NODE_ELEMENT;
		_bstr_t bstrName = _bstr_t( sElemName.c_str() );

		pXmlDoc->createNode( varNodeType , bstrName, 0, &pNode );
		pParentNode->appendChild( pNode, &pNode2 );

	}
	catch( ... )
	{
		return NULL;
	}
	return pNode2;
}
///////////////////////////////////////////////////////////////
// ����һ���ӽ��(���ݽ��)
IXMLDOMNode * ScanParamXML::AddChildNodeWithValue( IXMLDOMNode *const pParentNode,	
									IXMLDOMDocument *const pXmlDoc,
									const string &sNodeName, 
									_variant_t varNodeVal )
{
	IXMLDOMNode *pNode = NULL, *pNode2 = NULL;
	IXMLDOMNode *pNodeOut = NULL;
	try
	{
		_variant_t varNodeType = NODE_ELEMENT;
		_bstr_t bstrName; 
		_bstr_t bstrDataType = NODE_CDATA_SECTION;

		bstrName = _bstr_t( sNodeName.c_str() );
		pXmlDoc->createNode( varNodeType , bstrName, 0, &pNode );
		pParentNode->appendChild( pNode, &pNodeOut );

		varNodeType = NODE_TEXT;
		pXmlDoc->createNode( varNodeType , bstrName, 0, &pNode2 );
		pNode2->put_nodeValue( varNodeVal );
		pNode->appendChild( pNode2, &pNodeOut );
	}
	catch( ... )
	{
		return NULL;
	}
	return pNodeOut;
}

///////////////////////////////////////////////////////////////