/********************************************
 * �ļ�: ScanParamXml.h
 * ����: ScanParamXml �������ļ�
 * ����: ���ɨ������ṹ( ScanParam ) �� XML �����ĵ�
 *       ֮���ת��
 * ע��: 1.ʹ�� MSXML 4.0 SDK, ע��ͷ�ļ���lib���ļ���·������
 * ����: Benny 2007.8.23
 * �޸�: (���¼�޸���Ϣ)
********************************************/

#ifndef ScanParamXML_H
#define ScanParamXML_H

#include <string>
#include <comutil.h>
#include "msxml2.h"			// XML SDKͷ�ļ�

#include "ScanSvdbUtils.h"	// ���ݸ�ʽ�����ļ�


using namespace std;
using namespace NS_ScanSvdbUtils;
////////////////////////////////////////////////////////////////////
// ɨ������ṹ��XML�ĵ�ת����
// XML �ĵ����������� MSXML 4.0
// ��Ҫ���ö���ʼ���ͷ���ʼ�� COM ����
// ������: ::CoInitialize(NULL) �� ::CoUninitialize();
// 
// ����Ϊ��̬���������
class ScanParamXML
{

public:
	// ������ʼ������
	static bool Init();				// ��ʼ������
	static void Release();			// �ͷŻ���
	static bool IsInit();			// �����Ƿ��Ѿ���ʼ��

	///////////////////////////////////////////////////////////////
	// ����: ɨ������ṹת���� XML �ĵ�
	// ����: sp				[in]	ɨ������ṹ
	//		 sXMLDocText	[out]	XML�ĵ����ı�
	// ����: true - �ɹ�, false - ʧ��
	static bool ScanParamToXML( const ScanParameter &scanParam,
								string &sXMLDocText );

	///////////////////////////////////////////////////////////////
	// ����: XML �ĵ��ı�ת����ɨ������ṹ
	// ����: sXMLDocText	[in]	XML�ĵ����ı�
	//		 sp				[out]	ɨ������ṹ
	// ����: true - �ɹ�, false - ʧ��
	static bool XMLToScanParam( const string &sXMLDocText,
								ScanParameter &scanParam );

protected:
	// �ڲ���������
	static IXMLDOMNode * AddChildElem( IXMLDOMNode *const pParentNode,		// ����һ���ӽ��(�����ݽ��)
										IXMLDOMDocument *const pXmlDoc,
										const string &sElemName );		
	static IXMLDOMNode * AddChildNodeWithValue( IXMLDOMNode *const pParentNode,	// ����һ���ӽ��(Text���ݽ��)
										IXMLDOMDocument *const pXmlDoc,
										const string &sNodeName,
										_variant_t varNodeVal );	
	static _variant_t ReadNodeText( IXMLDOMNode *const pTheNode );		// �����Text(���ݽ��)
	static bool ReadCommunityListNode( IXMLDOMNode *const pTheNode,		// ��CommunityList���
										VectorOfCommunity &vCommunity );	
	static bool ReadIpScopeListNode( IXMLDOMNode *const pTheNode,		// ��IpScopeList���
										VectorOfIpScope &vIpScope );	
	static bool ReadStringListNode( IXMLDOMNode *const pTheNode,		// ��StringList���
										VectorOfString &vStr );	
		
protected:
	static bool m_bIsInit;	// COM �����Ƿ��Ѿ�ʵʼ��

};	// class ScanParamXML

////////////////////////////////////////////////////////////////////
#endif