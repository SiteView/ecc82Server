// MailMessage.h: interface for the CMailMessage class.
//
//////////////////////////////////////////////////////////////////////
#include <afxtempl.h>

#ifndef __MAILMESSAGE_H__
#define __MAILMESSAGE_H__

class CMailMessage  
{
public:
	CMailMessage();
	virtual ~CMailMessage();

	enum RECIPIENTS_TYPE { TO, CC, BCC };

	void FormatMessage();
	int GetNumRecipients(RECIPIENTS_TYPE type = TO);
	BOOL GetRecipient( CString& sEmailAddress, CString& sFriendlyName, int nIndex = 0, RECIPIENTS_TYPE type = TO);
	BOOL AddRecipient( LPCTSTR szEmailAddress, LPCTSTR szFriendlyName = "", RECIPIENTS_TYPE type = TO);
	BOOL AddMultipleRecipients( LPCTSTR szRecipients = NULL, RECIPIENTS_TYPE type = TO );
	UINT GetCharsPerLine();
	void SetCharsPerLine( UINT nCharsPerLine );

	CString m_sFrom;
	CString m_sSubject;
	CString m_sEnvelope;
	CString m_sMailerName;
	CString m_sHeader;
	CTime m_tDateTime;
	CString m_sBody;

private:
	UINT m_nCharsPerLine;
	class CRecipient
	{
		public:
			CString m_sEmailAddress;
			CString m_sFriendlyName;
	};
	CArray <CRecipient, CRecipient&> m_Recipients;
	CArray <CRecipient, CRecipient&> m_CCRecipients;    //*** <JFO>
	CArray <CRecipient, CRecipient&> m_BCCRecipients;   //*** <JFO>

protected:
	// When overriding prepare_header(), call base class 
	// version first, then add specialized 
	// add_header_line calls.
	// This ensures that the base class has a chance to
	// create the header lines it needs.
	virtual void prepare_header();
	virtual void prepare_body();
	virtual void end_header();
	virtual void start_header();

	// This rarely needs overwriting, but is virtual just in case.
	// Do not include the trailing CR/LF in parameter.
	virtual void add_header_line( LPCTSTR szHeaderLine );
};

#endif // !defined(AFX_MAILMESSAGE_H__755047AE_BDE4_4EB6_AE17_43D26FD41E44__INCLUDED_)
