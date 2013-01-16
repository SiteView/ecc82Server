// AppOctetStream.h: interface for the CAppOctetStream class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __APPOCTETSTREAM_H__
#define __APPOCTETSTREAM_H__

#include "MIMEContentAgent.h"

class CAppOctetStream : public CMIMEContentAgent  
{
public:
	virtual CString GetContentTypeString();
	CAppOctetStream( int nContentType );
	virtual ~CAppOctetStream();

	virtual BOOL AppendPart( LPCTSTR szContent, 
							 LPCTSTR szParameters, 
							 int nEncoding, 
							 BOOL bPath, 
							 CString& sDestination );

protected:
	virtual void attach_file( CStdioFile* pFileAtt, int nEncoding, CString& sDestination );
	virtual CString build_sub_header( LPCTSTR szContent, 
									  LPCTSTR szParameters, 
									  int nEncoding, 
									  BOOL bPath );
};

#endif // !defined(AFX_APPOCTETSTREAM_H__F3371394_FB8B_423A_9D73_D89FA10E2928__INCLUDED_)
