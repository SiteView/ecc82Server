// MIMECode.h: interface for the CMIMECode class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __MIMECODE_H__
#define __MIMECODE_H__

class CMIMECode  
{
public:
	CMIMECode();
	virtual ~CMIMECode();

	virtual int Decode( LPCTSTR szDecoding, LPTSTR szOutput ) = 0;
	virtual CString Encode( LPCTSTR szEncoding, int nSize ) = 0;
};

#endif // !defined(AFX_MIMECODE_H__1534CB92_D988_4F0E_9E29_4E894E407232__INCLUDED_)
