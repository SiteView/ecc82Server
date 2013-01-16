/********************************************
 * 文件: ScanParamXml.h
 * 描述: ScanParamXml 类声明文件
 * 功能: 完成扫描参数结构( ScanParam ) 与 XML 配置文档
 *       之间的转换
 * 注意: 1.使用 MSXML 4.0 SDK, 注意头文件和lib库文件的路径设置
 * 创建: Benny 2007.8.23
 * 修改: (请记录修改信息)
********************************************/

#ifndef ScanParamXML_H
#define ScanParamXML_H

#include <string>
#include <comutil.h>
#include "msxml2.h"			// XML SDK头文件

#include "ScanSvdbUtils.h"	// 数据格式定义文件


using namespace std;
using namespace NS_ScanSvdbUtils;
////////////////////////////////////////////////////////////////////
// 扫描参数结构与XML文档转换类
// XML 文档解析器采用 MSXML 4.0
// 需要调用都初始化和反初始化 COM 环境
// 即调用: ::CoInitialize(NULL) 和 ::CoUninitialize();
// 
// 此类为静态方法输出类
class ScanParamXML
{

public:
	// 环境初始化方法
	static bool Init();				// 初始化环境
	static void Release();			// 释放环境
	static bool IsInit();			// 环境是否已经初始化

	///////////////////////////////////////////////////////////////
	// 功能: 扫描参数结构转换成 XML 文档
	// 参数: sp				[in]	扫描参数结构
	//		 sXMLDocText	[out]	XML文档的文本
	// 返回: true - 成功, false - 失败
	static bool ScanParamToXML( const ScanParameter &scanParam,
								string &sXMLDocText );

	///////////////////////////////////////////////////////////////
	// 功能: XML 文档文本转换成扫描参数结构
	// 参数: sXMLDocText	[in]	XML文档的文本
	//		 sp				[out]	扫描参数结构
	// 返回: true - 成功, false - 失败
	static bool XMLToScanParam( const string &sXMLDocText,
								ScanParameter &scanParam );

protected:
	// 内部保护方法
	static IXMLDOMNode * AddChildElem( IXMLDOMNode *const pParentNode,		// 创建一个子结点(非数据结点)
										IXMLDOMDocument *const pXmlDoc,
										const string &sElemName );		
	static IXMLDOMNode * AddChildNodeWithValue( IXMLDOMNode *const pParentNode,	// 创建一个子结点(Text数据结点)
										IXMLDOMDocument *const pXmlDoc,
										const string &sNodeName,
										_variant_t varNodeVal );	
	static _variant_t ReadNodeText( IXMLDOMNode *const pTheNode );		// 读结点Text(数据结点)
	static bool ReadCommunityListNode( IXMLDOMNode *const pTheNode,		// 读CommunityList结点
										VectorOfCommunity &vCommunity );	
	static bool ReadIpScopeListNode( IXMLDOMNode *const pTheNode,		// 读IpScopeList结点
										VectorOfIpScope &vIpScope );	
	static bool ReadStringListNode( IXMLDOMNode *const pTheNode,		// 读StringList结点
										VectorOfString &vStr );	
		
protected:
	static bool m_bIsInit;	// COM 环境是否已经实始化

};	// class ScanParamXML

////////////////////////////////////////////////////////////////////
#endif