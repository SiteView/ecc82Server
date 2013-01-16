// IMAP4Message.h: interface for the CIMAP4Message class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAP4MESSAGE_H__D71156F5_04F0_4274_A7E4_6C3CEEE71D96__INCLUDED_)
#define AFX_IMAP4MESSAGE_H__D71156F5_04F0_4274_A7E4_6C3CEEE71D96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
using namespace std;

class CIMAP4Message  
{
public:
	CIMAP4Message();
	virtual ~CIMAP4Message();

	string		GetHeader() const;
	const char* GetRawBody() const;
	string		GetBody() const;
	string		GetHeaderItem(const string& sName, int nItem = 0) const;
	string		GetFrom() const	{ return GetHeaderItem("From"); }
	string		GetTo() const { return GetHeaderItem("To"); }
	string		GetDate() const	{ return GetHeaderItem("Date"); }
	string		GetSubject() const	{ return GetHeaderItem("Subject"); }
	string		GetReplyTo() const;

public:
  char* m_pszMessage;
};

#endif // !defined(AFX_IMAP4MESSAGE_H__D71156F5_04F0_4274_A7E4_6C3CEEE71D96__INCLUDED_)
