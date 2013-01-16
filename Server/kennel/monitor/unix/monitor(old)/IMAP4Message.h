// IMAP4Message.h: interface for the CIMAP4Message class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAP4MESSAGE_H__D71156F5_04F0_4274_A7E4_6C3CEEE71D96__INCLUDED_)
#define AFX_IMAP4MESSAGE_H__D71156F5_04F0_4274_A7E4_6C3CEEE71D96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIMAP4Message  
{
public:
	CIMAP4Message();
	virtual ~CIMAP4Message();

	CString		GetHeader() const;
	LPCSTR		GetRawBody() const;
	CString		GetBody() const;
	CString		GetHeaderItem(const CString& sName, int nItem = 0) const;
	CString		GetFrom() const	{ return GetHeaderItem(_T("From")); }
	CString		GetTo() const { return GetHeaderItem(_T("To")); }
	CString		GetDate() const	{ return GetHeaderItem(_T("Date")); }
	CString		GetSubject() const	{ return GetHeaderItem(_T("Subject")); }
	CString		GetReplyTo() const;

public:
  char* m_pszMessage;
};

#endif // !defined(AFX_IMAP4MESSAGE_H__D71156F5_04F0_4274_A7E4_6C3CEEE71D96__INCLUDED_)
