#ifndef PKCS12_H
#define PKCS12_H

#ifdef __cplusplus
extern "C" {
#endif


#ifndef GLOBAL_DEFS
#include "global.h"
#endif

#ifndef NIF_DEFS
#include "nif.h"
#endif


#define	PKCS12_EXCLUDE_PRIVATEKEYS	0x00000001


STATUS LNPUBLIC PKCS12_ImportFileToIDFile (char *pPKCS12Filename, 
								  	   char  *pPKCS12Filepassword, 
								  	   char	 *pIdFilename, 
								  	   char	 *pIdFilepassword, 
									   DWORD ImportFlags,
									   DWORD ReservedFlags,
									   void  *pReserved);

STATUS LNPUBLIC PKCS12_ExportIDFileToFile (	char		*pIdFilename, 
								  	   		char		*pIdFilepassword, 
											char 		*pPKCS12Filename, 
								  	   		char 		*pPKCS12Filepassword, 
										   	DWORD 		ExportFlags,
									   		DWORD 		ReservedFlags,
									   		void 		*pReserved);



#ifdef __cplusplus
}
#endif

#endif

