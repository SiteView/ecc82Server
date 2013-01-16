#include "StdAfx.h"
#include "InsertXml.h"
#include "Blowfish.h"


const char Key[] = {"[;'.]94-31==-%&@!^+]\0"};

CInsertXml::CInsertXml(void)
{
	m_xmlDoc.CryptoNew(_T("keyProgrammer.log"),"<?xml version=\"1.0\" encoding=\"utf-8\"?><log></log>");
	//xmlfileok = m_xmlDoc.Load(_T("keyProgrammer.log"));
	m_xmlDoc.CryptoLoad(_T("keyProgrammer.log"));
	
}

CInsertXml::~CInsertXml(void)
{


}

void CInsertXml::InsertXml(LPCTSTR UserName,LPCTSTR Company,LPCTSTR Operator,LPCTSTR Remark,int SerialNumber,DWORD ExpiresDate,DWORD IssCLIPS,DWORD IsLC)
{

	CXmlNodeWrapper node(m_xmlDoc.AsNode());

    CXmlNodeWrapper   node2(node.InsertNode(0,_T("item")));  

	CXmlNodeWrapper   node_UserName(node2.InsertNode(0,_T("UserName"))); 
	node_UserName.SetText(UserName); 

	CXmlNodeWrapper   node_Company(node2.InsertNode(0,_T("Company"))); 
	node_Company.SetText(Company); 

	CXmlNodeWrapper   node_Operator(node2.InsertNode(0,_T("Operator"))); 
	node_Operator.SetText(Operator); 

	CXmlNodeWrapper   node_Remark(node2.InsertNode(0,_T("Remark"))); 
	node_Remark.SetText(Remark); 

    CXmlNodeWrapper   node3(node2.InsertNode(0,_T("SerialNumber")));
	CString str;
	str.Format(_T("%X"),SerialNumber);   
	node3.SetText(str);   

    CXmlNodeWrapper   node4(node2.InsertNode(0,_T("ExpiresDate")));
	str.Format(_T("%d"),ExpiresDate);   
	node4.SetText(str); 

	CXmlNodeWrapper   node5(node2.InsertNode(0,_T("IssCLIPS")));
	
	if (IssCLIPS == 1)
	{
		node5.SetText(_T("True")); 
	}
	else
	{
	    node5.SetText(_T("False")); 
	}

	CXmlNodeWrapper   node6(node2.InsertNode(0,_T("IsLC")));
	
	if (IsLC == 1)
	{
		node6.SetText(_T("True")); 
	}
	else
	{
	    node6.SetText(_T("False")); 
	}
	CXmlNodeWrapper   node7(node2.InsertNode(0,_T("ProgramDate")));
	
	CTime now = CTime::GetCurrentTime();
	
	node7.SetText(now.Format(_T("%Y-%m-%d %H:%M:%S"))); 
	

	m_xmlDoc.CryptoSave(_T("keyProgrammer.log"));


}
