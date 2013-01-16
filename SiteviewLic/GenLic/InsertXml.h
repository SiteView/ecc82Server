#pragma once

#include "XmlNodeWrapper.h"

class CInsertXml
{
public:
	CInsertXml(void);
	~CInsertXml(void);
	CXmlDocumentWrapper m_xmlDoc;
	void InsertXml(LPCTSTR UserName,LPCTSTR Company,LPCTSTR Operator,LPCTSTR Remark,int SerialNumber,DWORD ExpiresDate,DWORD IssCLIPS,DWORD IsLC);
};





	