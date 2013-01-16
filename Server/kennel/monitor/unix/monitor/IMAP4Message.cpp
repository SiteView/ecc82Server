// IMAP4Message.cpp: implementation of the CIMAP4Message class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Mail.h"
#include "IMAP4Message.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIMAP4Message::CIMAP4Message()
{
	m_pszMessage = NULL;
}

CIMAP4Message::~CIMAP4Message()
{
	if (m_pszMessage)
	{
		delete [] m_pszMessage;
		m_pszMessage = NULL;
	}
}

CString CIMAP4Message::GetHeader() const
{
	//Value which will be returned by this function
	CString sHeader;
	
	//Find the divider between the header and body
	CString sMessage(m_pszMessage);
	int nFind = sMessage.Find(_T("\r\n\r\n"));
	if (nFind != -1)
		sHeader = sMessage.Left(nFind);
	else
	{
		//No divider, then assume all the text is the header
		sHeader = sMessage;
	}
	
	return sHeader;
}

LPCSTR CIMAP4Message::GetRawBody() const
{
	char* pszStartBody = strstr(m_pszMessage, "\r\n\r\n");
	if (pszStartBody == NULL) 
		return NULL;
	else 
		return pszStartBody + 4;
}

CString CIMAP4Message::GetBody() const
{
	CString sBody;
	LPCSTR pszBody = GetRawBody();
	if (pszBody)
		sBody = pszBody;
	return sBody;
}

CString CIMAP4Message::GetHeaderItem(const CString& sName, int nItem) const
{
	//Value which will be returned by this function
	CString sField;
	
	//Get the message header (add an extra "\r\n" at the
	//begining to aid in the parsing)  
	CString sHeader(_T("\r\n"));
	sHeader += GetHeader();
	CString sUpCaseHeader(sHeader);
	sUpCaseHeader.MakeUpper();
	
	CString sUpCaseName(sName);
	sUpCaseName.MakeUpper();
	
	//Find the specified line in the header
	CString sFind(CString(_T("\r\n")) + sUpCaseName + _T(":"));
	int nFindLength = sFind.GetLength();
	int nFindStart = sUpCaseHeader.Find(sFind);
	int nFind = nFindStart;
	for (int i=0; i<nItem; i++) 
	{
		//Get ready for the next loop around
		sUpCaseHeader = sUpCaseHeader.Right(sUpCaseHeader.GetLength() - nFind - nFindLength);
		nFind = sUpCaseHeader.Find(sFind);
		
		if (nFind == -1)
			return _T(""); //Not found
		else
			nFindStart += (nFind + nFindLength);
	}
	
	if (nFindStart != -1)
		nFindStart += (3 + sName.GetLength());
	if (nFindStart != -1)
	{
		BOOL bFoundEnd = FALSE;
		int i = nFindStart;
		int nLength = sHeader.GetLength();
		do
		{
			//Examine the current 3 characters
			TCHAR c1 = _T('\0');
			if (i < nLength)
				c1 = sHeader[i];
			TCHAR c2 = _T('\0');
			if (i < (nLength-1))
				c2 = sHeader[i+1];
			TCHAR c3 = _T('\0');
			if (i < (nLength-2))
				c3 = sHeader[i+2];
			
			//Have we found the terminator
			if ((c1 == _T('\0')) ||
				((c1 == _T('\r')) && (c2 == _T('\n')) && (c3 != _T(' ')) && c3 != _T('\t')))
			{
				bFoundEnd = TRUE;
			}
			else
			{
				//Move onto the next character  
				++i;
			}
		}
		while (!bFoundEnd);
		sField = sHeader.Mid(nFindStart, i - nFindStart);
		
		//Remove any embedded "\r\n" sequences from the field
		int nEOL = sField.Find(_T("\r\n"));
		while (nEOL != -1)
		{
			sField = sField.Left(nEOL) + sField.Right(sField.GetLength() - nEOL - 2);
			nEOL = sField.Find(_T("\r\n"));
		}
		
		//Replace any embedded "\t" sequences with spaces
		int nTab = sField.Find(_T('\t'));
		while (nTab != -1)
		{
			sField = sField.Left(nTab) + _T(' ') + sField.Right(sField.GetLength() - nTab - 1);
			nTab = sField.Find(_T('\t'));
		}
		
		//Remove any leading or trailing white space from the Field Body
		sField.TrimLeft();
		sField.TrimRight();
	}
	
	return sField;
}

CString CIMAP4Message::GetReplyTo() const
{
	CString sRet = GetHeaderItem("Reply-To");
	if (sRet.IsEmpty())
	{
		sRet = GetFrom();
		if (sRet.IsEmpty())
		{
			sRet = GetHeaderItem(_T("Sender"));
			if (sRet.IsEmpty())
				sRet = GetHeaderItem(_T("Return-Path"));
		}
	}
	return sRet;
}