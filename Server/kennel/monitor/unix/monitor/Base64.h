// Base64.h: interface for the CBase64 class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_BASE64_H__82C227AB_ACD3_419B_863D_8BEB6B27B6FB__INCLUDED_)
#define AFX_BASE64_H__82C227AB_ACD3_419B_863D_8BEB6B27B6FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Mail.h"
#include "MIMECode.h"

class CBase64 : public CMIMECode
{
public:
	CBase64();
	virtual ~CBase64();

// Override the base class mandatory functions
	virtual int Decode( LPCTSTR szDecoding, LPTSTR szOutput );
	virtual CString Encode( LPCTSTR szEncoding, int nSize );

protected:
	void write_bits( UINT nBits, int nNumBts, LPTSTR szOutput, int& lp );
	UINT read_bits( int nNumBits, int* pBitsRead, int& lp );

	int m_nInputSize;
	int m_nBitsRemaining;
	ULONG m_lBitStorage;
	LPCTSTR m_szInput;

	static int m_nMask[];
	static CString m_sBase64Alphabet;
private:
};

#endif // !defined(AFX_BASE64_H__82C227AB_ACD3_419B_863D_8BEB6B27B6FB__INCLUDED_)
