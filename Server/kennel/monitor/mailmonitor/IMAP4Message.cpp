// IMAP4Message.cpp: implementation of the CIMAP4Message class.
//
//////////////////////////////////////////////////////////////////////
#include "IMAP4Message.h"
#include "../base/SVDefines.h"

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

string CIMAP4Message::GetHeader() const
{
	//Value which will be returned by this function
	string sHeader;
	
	//Find the divider between the header and body
	string sMessage(m_pszMessage);
	int nFind = static_cast<int>(sMessage.find(("\r\n\r\n")));
	if (nFind != -1)
		sHeader = sMessage.substr(0, nFind);
	else
	{
		//No divider, then assume all the text is the header
		sHeader = sMessage;
	}
	
	return sHeader;
}

const char* CIMAP4Message::GetRawBody() const
{
	char* pszStartBody = strstr(m_pszMessage, "\r\n\r\n");
	if (pszStartBody == NULL) 
		return NULL;
	else 
		return pszStartBody + 4;
}

string CIMAP4Message::GetBody() const
{
	string sBody;
	const char* pszBody = GetRawBody();
	if (pszBody)
		sBody = pszBody;
	return sBody;
}

string CIMAP4Message::GetHeaderItem(const string& sName, int nItem) const
{
	//Value which will be returned by this function
	string sField;
	
	//Get the message header (add an extra "\r\n" at the
	//begining to aid in the parsing)  
	string sHeader(("\r\n"));
	sHeader += GetHeader();
	string sUpCaseHeader(sHeader);
	strupr(const_cast<char*>(sUpCaseHeader.c_str()));
	
	string sUpCaseName(sName);
	strupr(const_cast<char*>(sUpCaseName.c_str()));
	
	//Find the specified line in the header
	string sFind(string(("\r\n")) + sUpCaseName + (":"));
	int nFindLength = static_cast<int>(sFind.length());
	int nFindStart = static_cast<int>(sUpCaseHeader.find(sFind));
	int nFind = nFindStart;
	for (int i=0; i<nItem; i++) 
	{
		//Get ready for the next loop around
		sUpCaseHeader = sUpCaseHeader.substr(nFind + nFindLength);
		nFind = static_cast<int>(sUpCaseHeader.find(sFind));
		
		if (nFind == -1)
			return (""); //Not found
		else
			nFindStart += (nFind + nFindLength);
	}
	
	if (nFindStart != -1)
		nFindStart += (3 + static_cast<int>(sName.length()));
	if (nFindStart != -1)
	{
		bool bFoundEnd = false;
		int i = nFindStart;
		int nLength = static_cast<int>(sHeader.length());
		do
		{
			//Examine the current 3 characters
			unsigned char c1 = ('\0');
			if (i < nLength)
				c1 = sHeader[i];
			unsigned char c2 = ('\0');
			if (i < (nLength-1))
				c2 = sHeader[i+1];
			unsigned char c3 = ('\0');
			if (i < (nLength-2))
				c3 = sHeader[i+2];
			
			//Have we found the terminator
			if ((c1 == ('\0')) ||
				((c1 == ('\r')) && (c2 == ('\n')) && (c3 != (' ')) && c3 != ('\t')))
			{
				bFoundEnd = true;
			}
			else
			{
				//Move onto the next character  
				++i;
			}
		}
		while (!bFoundEnd);
		sField = sHeader.substr(nFindStart, i - nFindStart);
		
		//Remove any embedded "\r\n" sequences from the field
		int nEOL = static_cast<int>(sField.find(("\r\n")));
		while (nEOL != -1)
		{
			sField = sField.substr(0, nEOL) + sField.substr( nEOL + 2);
			nEOL = static_cast<int>(sField.find(("\r\n")));
		}
		
		//Replace any embedded "\t" sequences with spaces
		int nTab = static_cast<int>(sField.find(('\t')));
		while (nTab != -1)
		{
			sField = sField.substr(0, nTab) + (' ') + sField.substr(nTab + 1);
			nTab = static_cast<int>(sField.find(('\t')));
		}
		
		//Remove any leading or trailing white space from the Field Body
        sField = strtriml(sField.c_str());
		sField = strtrimr(sField.c_str());
	}
	
	return sField;
}

string CIMAP4Message::GetReplyTo() const
{
	string sRet = GetHeaderItem("Reply-To");
	if (sRet.empty())
	{
		sRet = GetFrom();
		if (sRet.empty())
		{
			sRet = GetHeaderItem(("Sender"));
			if (sRet.empty())
				sRet = GetHeaderItem(("Return-Path"));
		}
	}
	return sRet;
}