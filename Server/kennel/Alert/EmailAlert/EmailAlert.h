//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

#ifndef _SV_EMAIL_ALERT_H_
#define _SV_EMAIL_ALERT_H_

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef WIN32
#define DLL_EXPORT extern "C" __declspec(dllexport)
#else
#define DLL_EXPORT extern "C"
#endif

DLL_EXPORT bool DoAlert(const char * szContent, const char * szDest = NULL ,
                        const char * szSubject = NULL);

DLL_EXPORT bool SendEmail(const char *pszServer, const char *pszMailfrom, 
                          const char *pszMailTo, const char *pszSubject,
                          const char * pszMailContent, const char *pszUser, 
                          const char *pszPassword);

#endif

