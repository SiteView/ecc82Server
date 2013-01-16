// TextPlain.h: interface for the CTextPlain class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __TEXTPLAIN_H__
#define __TEXTPLAIN_H__

#include "MIMEContentAgent.h"

class CTextPlain : public CMIMEContentAgent  
{
public:
	CTextPlain( int nContentType, UINT nWrapPos = 72 );
	virtual ~CTextPlain();

	virtual BOOL AppendPart( LPCTSTR szContent, LPCTSTR szParameters, int nEncoding, BOOL bPath, CString& sDestination );
	virtual CString GetContentTypeString();

protected:
	UINT m_nWrapPos;

	CString wrap_text( LPCTSTR szText );
	virtual CString build_sub_header( LPCTSTR szContent, LPCTSTR szParameters, int nEncoding, BOOL bPath );
};

#endif // !defined(AFX_TEXTPLAIN_H__39D5D373_A5B0_4FFD_904E_18E2A42A4B05__INCLUDED_)
