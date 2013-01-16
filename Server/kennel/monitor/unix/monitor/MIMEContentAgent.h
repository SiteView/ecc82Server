// MIMEContentAgent.h: interface for the CMIMEContentAgent class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __MIMECONTENTAGENT_H__
#define __MIMECONTENTAGENT_H__

class CMIMEContentAgent  
{
public:
	CMIMEContentAgent( int nMIMEType );
	virtual ~CMIMEContentAgent();

	BOOL QueryType( int nContentType );

	virtual BOOL AppendPart( LPCTSTR szContent,
							 LPCTSTR szParameters, 
							 int nEncoding, 
							 BOOL bPath, 
							 CString& sDestination ) = 0;
	virtual CString GetContentTypeString() = 0;

protected:
	virtual CString build_sub_header( LPCTSTR szContent,
									  LPCTSTR szParameters,
									  int nEncoding,
									  BOOL bPath ) = 0;


private:
	int m_nMIMETypeIHandle;
};

#endif // !defined(AFX_MIMECONTENTAGENT_H__A0F3A5EF_1A29_4E0B_8837_62EB5EF71239__INCLUDED_)
