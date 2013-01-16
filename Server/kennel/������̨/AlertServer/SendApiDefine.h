//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SV_SENDAPIDEFINE_H_
#define _SV_SENDAPIDEFINE_H_
#if _MSC_VER > 1000
#pragma once
#endif

#ifdef WIN32
#include <windows.h>
#endif

#ifdef WIN32
	//typedef void(SendEmailAlert)(CString,CString,CString,CString,CString,CString,CString,int&);	
	//typedef bool(SendEmail)(const char *pszServer, const char *pszMailfrom, 
	//						const char *pszMailTo, const char *pszSubject,
	//						const char * pszMailContent, const char *pszUser, 
	//						const char *pszPassword);
	typedef void(SendEmailAlert)(const char *pszSmtpServer, const char *pszFromAddress,
							const char *pszEmailSubject, const char *pszMail,
							const char *pszEmailContent, const char *pszUser,
							const char *pszPassword, int &iEmailRet);
	typedef bool(SendEmail)(const char *pszServer, const char *pszMailfrom, 
                          const char *pszMailTo, const char *pszSubject,
                          const char * pszMailContent, const char *pszUser, 
                          const char *pszPassword, const char* pszAttachName);
	typedef bool (EXECUTESCRIPT)(const char* szQuery, char* szReturn, int &nSize);

 //  typedef bool (DoAlert)(const char * szContent, const char * szDest /* = NULL  */, const char * szSubject /* = NULL */);
#endif

#endif