/********************************************
 * 文件: ScanParamXml.cpp
 * 描述: ScanParamXml 类定义文件
 * 注意: 使用 MSXML 4.0 SDK, 注意头文件和lib库文件的路径设置
 * 创建: Benny 2007.8.23
 * 修改: (请记录修改信息)
********************************************/

#include <objbase.h>		// COM 环境头文件
#include <wtypes.h>
#include <tchar.h>

#include <iostream>

#include "ScanParamXML.h"	// 本类的声明文件

//using namespace MSXML2;

#define XML_TRUE		"True"		// XML 文档中的 bool true
#define XML_FALSE		"False"		// XML 文档中的 bool false

bool ScanParamXML::m_bIsInit = false;	// 静态成员定义,初始化

///////////////////////////////////////////////////////////////
// 初始化环境s
bool ScanParamXML::Init()
{
	bool bOk = true;

	if( !m_bIsInit )
	{	// 尚未初始化
		HRESULT hr = ::CoInitialize( NULL );	// 初始化COM环境 
		bOk = ( S_OK == hr );
		m_bIsInit = bOk;
	}

	return bOk;
}

// 释放环境
void ScanParamXML::Release()
{
	::CoUninitialize();		// 释放COM环境
	m_bIsInit = false;
}

// 环境是否已经初始化
bool ScanParamXML::IsInit()		
{
	return m_bIsInit;
}

///////////////////////////////////////////////////////////////
// 功能: 扫描参数结构转换成 XML 文档
// 参数: sp				[in]	扫描参数结构
//		 sXMLDocText	[out]	XML文档的文本
// 返回: true - 成功, false - 失败
bool ScanParamXML::ScanParamToXML( const ScanParameter &sp, string &sXMLDocText )
{
	try
	{
		IXMLDOMDocument *pXmlDoc = NULL;
		IXMLDOMNode *pXmlDomNode = NULL;
			
		// 得到关于IXMLDOMDocument 接口的指针 pXMLDOC
		HRESULT hr = ::CoCreateInstance( CLSID_DOMDocument,
										NULL, 
										CLSCTX_INPROC_SERVER,
										IID_IXMLDOMDocument, 
										( void ** )&pXmlDoc );
		if( S_OK != hr )
		{
			return false;
		}

		//得到关于IXMLDOMNode接口的指针pXDN 
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


		// 创建根结点
		bstrName = _T("TopDiscoverParas");		
		hr = pXmlDoc->createNode( varNodeType , bstrName, 0, &pNode );
		pXmlDoc->appendChild( pNode, &pRootNode );

		// 创建Depth结点
		varNodeVal = sp.nDepth;
		AddChildNodeWithValue( pRootNode, pXmlDoc, "Depth", varNodeVal );

		// 创建Tineout结点
		varNodeVal = sp.nTimeout;
		AddChildNodeWithValue( pRootNode, pXmlDoc, "Timeout", varNodeVal );

		// 创建TryTimes结点
		varNodeVal = sp.nTryTimes;
		AddChildNodeWithValue( pRootNode, pXmlDoc, "TryTimes", varNodeVal );

		// 创建SubScan结点
		varNodeVal = sp.bSubScan ? XML_TRUE : XML_FALSE;
		AddChildNodeWithValue( pRootNode, pXmlDoc, "SubScan", varNodeVal );

		// 创建MaxTreadCount结点
		varNodeVal = sp.nMaxTreadCount;
		AddChildNodeWithValue( pRootNode, pXmlDoc, "MaxTreadCount", varNodeVal );

		// 创建DefaultCommunityGet结点
		varNodeVal = sp.sDefaultCommunityOfGet.c_str();
		AddChildNodeWithValue( pRootNode, pXmlDoc, "DefaultCommunityGet", varNodeVal );

		// 创建DefaultCommunitySet结点
		varNodeVal = sp.sDefaultCommunityOfSet.c_str();
		AddChildNodeWithValue( pRootNode, pXmlDoc, "DefaultCommunitySet", varNodeVal );
		

		// CommunityList 向量操作
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

		// IpScopeList 向量操作
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

		// FilterStartEndList 向量操作
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

		// SubnetList 向量操作
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

		// SeedIpList 向量操作
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

		// 处理输出参数
		BSTR bstrXml;
		pXmlDoc->get_xml( &bstrXml );
		bstrName = bstrXml;

		sXMLDocText = bstrName.operator char *();

		SysFreeString( bstrXml );

		// 保存到文件(用于测试)
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
// 功能: XML 文档文本转换成扫描参数结构
// 参数: sXMLDocText	[in]	XML文档的文本
//		 sp				[out]	扫描参数结构
// 返回: true - 成功, false - 失败
bool ScanParamXML::XMLToScanParam( const string &sXMLDocText, ScanParameter &sp )
{
	try
	{
		IXMLDOMDocument *pXmlDoc = NULL;
		IXMLDOMNode *pXmlDomNode = NULL;
			
		// 得到关于IXMLDOMDocument 接口的指针 pXMLDOC
		HRESULT hr = ::CoCreateInstance( CLSID_DOMDocument,
										NULL, 
										CLSCTX_INPROC_SERVER,
										IID_IXMLDOMDocument, 
										( void ** )&pXmlDoc );
		if( S_OK != hr )
		{
			return false;
		}

		//得到关于IXMLDOMNode接口的指针pXDN 
		hr = pXmlDoc->QueryInterface( IID_IXMLDOMNode,
									( void ** )&pXmlDomNode );
		if( S_OK != hr )
		{
			return false;
		}
		
		VARIANT_BOOL bOk;
		_variant_t varSrc = sXMLDocText.c_str();
		IXMLDOMElement *pRootNode;	// 根结点
		IXMLDOMNode *pNode;
		IXMLDOMNode *pNodeOut;
		
		pXmlDoc->loadXML( varSrc.bstrVal, &bOk  );	// 从字符串建立XML文档

		// 取得根结点
		pXmlDoc->get_documentElement( &pRootNode );		
		pRootNode->get_nodeName( &varSrc.bstrVal );

		// 遍历根结点的子结点
		for( pRootNode->get_firstChild( &pNode );
			pNode != NULL; pNode->get_nextSibling( &pNode ) )
		{
			pNode->get_nodeName( &varSrc.bstrVal );
			// cout << "Node name: " << varSrc.operator _bstr_t().operator char *() << endl;

			string sNodeName = _bstr_t( varSrc.bstrVal ).operator char *();
			if( sNodeName == string( "Depth" ) )
			{	// 发现 Depth 结点
				varSrc = ReadNodeText( pNode );
				sp.nDepth = atoi( _bstr_t( varSrc.bstrVal ).operator char *() );

				//cout << "Depth value: " << sp.nDepth << endl;
			}
			else if( sNodeName == string( "TryTimes" ) )
			{	// 发现 TryTimes 结点
				varSrc = ReadNodeText( pNode );
				sp.nTryTimes = atoi( _bstr_t( varSrc.bstrVal ).operator char *() );

				//cout << "TryTimes value: " << sp.nTryTimes << endl;
			}	
			else if( sNodeName == string( "Timeout" ) )
			{	// 发现 Timeout 结点
				varSrc = ReadNodeText( pNode );
				sp.nTimeout = atoi( _bstr_t( varSrc.bstrVal ).operator char *() );

				//cout << "Timeout value: " << sp.nTimeout << endl;
			}		
			else if( sNodeName == string( "MaxTreadCount" ) )
			{	// 发现 MaxTreadCount 结点
				varSrc = ReadNodeText( pNode );
				sp.nMaxTreadCount = atoi( _bstr_t( varSrc.bstrVal ).operator char *() );

				//cout << "MaxTreadCount value: " << sp.nMaxTreadCount << endl;
			}
			else if( sNodeName == string( "SubScan" ) )
			{	// 发现 SubScan 结点
				varSrc = ReadNodeText( pNode );
				sp.bSubScan = ( string( _bstr_t( varSrc.bstrVal ).operator char *() ) == XML_TRUE ) ;

				//cout << "SubScan value: " << sp.bSubScan << endl;
			}		
			else if( sNodeName == string( "DefaultCommunityGet" ) )
			{	// 发现 DefaultCommunityGet 结点
				varSrc = ReadNodeText( pNode );
				sp.sDefaultCommunityOfGet = _bstr_t( varSrc.bstrVal ).operator char *();

				//cout << "DefaultCommunityGet value: " << sp.sDefaultCommunityOfGet << endl;
			}		
			else if( sNodeName == string( "DefaultCommunitySet" ) )
			{	// 发现 DefaultCommunitySet 结点
				varSrc = ReadNodeText( pNode );
				sp.sDefaultCommunityOfSet = _bstr_t( varSrc.bstrVal ).operator char *();

				//cout << "DefaultCommunityOfSet value: " << sp.sDefaultCommunityOfSet << endl;
			}		
			else if( sNodeName == string( "Communitys" ) )
			{	// 发现 Communitys 结点
				ReadCommunityListNode( pNode, sp.vcCommunityList );
			}		
			else if( sNodeName == string( "AddStartEnds" ) )
			{	// 发现 AddStartEnds 结点
				ReadIpScopeListNode( pNode, sp.viIpScopeList );	// 读IpScopeList结点
			}		
			else if( sNodeName == string( "FilterStartEnds" ) )
			{	// 发现 FilterStartEnds 结点
				ReadIpScopeListNode( pNode, sp.viFilterStartEndList );	// 读IpScopeList结点
			}		
			else if( sNodeName == string( "SubnetList" ) )
			{	// 发现 SubnetList 结点
				ReadIpScopeListNode( pNode, sp.viSubnetList );	// 读IpScopeList结点
			}		
			else if( sNodeName == string( "SeedIpList" ) )
			{	// 发现 SeedIp 结点
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
// 非公开方法
///////////////////////////////////////////////////////////////
// 读结点Text(数据结点)
_variant_t ScanParamXML::ReadNodeText( IXMLDOMNode *const pTheNode )
{
	_variant_t varVal = 0;
	try
	{
		IXMLDOMNode *pTextNode;
		VARIANT_BOOL bOk;
		pTheNode->hasChildNodes( &bOk );
		if( bOk )
		{	// 有子结点(即Text结点,保存着数据)
			pTheNode->get_firstChild( &pTextNode );
			pTextNode->get_nodeValue( &varVal );
		}
	}
	catch( ... )
	{
		// 无处理
	}

	return varVal;
}

///////////////////////////////////////////////////////////////
// 读CommunityList结点
bool ScanParamXML::ReadCommunityListNode( IXMLDOMNode *const pTheNode, VectorOfCommunity &vCommunity )
{
	if( NULL == pTheNode )
	{
		return false;
	}
	try
	{
		vCommunity.clear();		// 初始化输出参数

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
// 读IpScopeList结点
bool ScanParamXML::ReadIpScopeListNode( IXMLDOMNode *const pTheNode, VectorOfIpScope &vIpScope )
{
	if( NULL == pTheNode )
	{
		return false;
	}
	try
	{
		vIpScope.clear();		// 初始化输出参数

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
// 读StringList结点
bool ScanParamXML::ReadStringListNode( IXMLDOMNode *const pTheNode, VectorOfString &vStr )
{
	if( NULL == pTheNode )
	{
		return false;
	}
	try
	{
		vStr.clear();		// 初始化输出参数

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
// 创建一个子结点(非数据结点)
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
// 创建一个子结点(数据结点)
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