#ifndef KFM_DEFS
#define KFM_DEFS

#ifndef NSF_DATA_DEFS         /* NOTEHANDLE */
#include "nsfdata.h"
#endif
#ifdef __cplusplus
extern "C" {
#endif

/*	Structure returned by KFMCreatePassword to encode a password securely
	in memory to avoid scavenging. 

	NOTE:	This structure is to inclue information on the password hash
			ONLY.  The 'Type' field is expected to take other values as the
			product evolves, but will always have the semanitics of how
			the hash was constructed, and which bytes in the 
			'HashedPassword' field contain the actual hash bytes.  Sometimes
			more than one hash will be stored in the hash array.
*/

typedef struct
	{
	BYTE	Type;				/* Type of hash.  This value should be		*/
								/* interpreted only by the lower Notes 		*/
								/* layers and never by software above the	*/
								/* ABPI layer.								*/
								/* hook for future compatibility.			*/
	BYTE	HashedPassword [48];  /* Hashed password						*/
	}
	KFM_PASSWORD;

typedef HANDLE HCERTIFIER;	
#define NULLHCERTIFIER ((HCERTIFIER)0)
#define KFM_ui_GetUserInfo				 1

/*	Function codes for routine SECKFMGetPublicKey
*/

#define KFM_pubkey_Primary				0
#define KFM_pubkey_International		1

STATUS LNPUBLIC SECKFMUserInfo			(WORD Function, 
										 char far *lpName, 
										 LICENSEID far *lpLicense);
STATUS LNPUBLIC SECKFMGetUserName		(char far *retUserName);

STATUS LNPUBLIC SECKFMSwitchToIDFile	(char	*pIDFileName
										,char	*pPassword
										,char	*pUserName
										,WORD	MaxUserNameLength
										,DWORD	ReservedFlags
										,void	*pReserved
										);								

typedef BOOL (LNCALLBACKPTR SECNABENUMPROC) (void  *pCallCtx,
											 void  *pCert,
											 DWORD CertSize,
											 WORD  Reserved1,
											 WORD  Reserved2);


STATUS LNPUBLIC SECNABEnumerateCertificates (NOTEHANDLE	hNote,
											 SECNABENUMPROC CallBack,
											 void	*pCallCtx,
											 DWORD	ReservedFlags,
											 void	*pReserved);

STATUS LNPUBLIC SECNABAddCertificate	(NOTEHANDLE	hNote
										,void		*pCertificate
										,DWORD		CertificateSize
										,DWORD		ReservedFlags
										,void		*pReserved
										);

STATUS LNPUBLIC SECNABRemoveCertificate (NOTEHANDLE	hNote
										,void	*pCertificate
										,DWORD  CertificateSize
										,DWORD	ReservedFlags
										,void	*pReserved
										);


STATUS LNPUBLIC SECKFMGetCertifierCtx   (char far		  *pCertFile, 
										 KFM_PASSWORD far *pKfmPW,
										 char far		  *pLogFile, 
										 TIMEDATE far	  *pExpDate, 
										 char far		  *retCertName,
										 HCERTIFIER far   *rethKfmCertCtx,
										 BOOL far		  *retfIsHierarchical, 
										 WORD far		  *retwFileVersion);

VOID   LNPUBLIC SECKFMFreeCertifierCtx	(HCERTIFIER hKfmCertCtx);

VOID   LNPUBLIC SECKFMCreatePassword	(char far		  *pPassword
										,KFM_PASSWORD far *retHashedPassword
										);
STATUS LNPUBLIC SECKFMSetCertifierExpiration   (HCERTIFIER hKfmCertCtx,
												TIMEDATE far *pExpirationDate);
STATUS LNPUBLIC SECKFMGetPublicKey		(char far	*pName
										,WORD		Function
										,WORD		Flags
										,HANDLE far *rethPubKey
										);
STATUS LNPUBLIC SECKFMChangePassword    (char far   *pIDFile
										,char far   *pOldPassword
										,char far   *pNewPassword
										);


/*	Constants used to indicate various types of IDs that can be created. */

#define KFM_IDFILE_TYPE_FLAT	0	/* Flat name space (V2 compatible)		*/
#define KFM_IDFILE_TYPE_STD		1	/* Standard (user/server hierarchical)	*/
#define KFM_IDFILE_TYPE_ORG		2	/* Organization certifier				*/
#define KFM_IDFILE_TYPE_ORGUNIT	3	/* Organizational unit certifier		*/
#define KFM_IDFILE_TYPE_DERIVED 4	/* Derived from certifer context.		*/
									/*  hierarchical => STD					*/
									/*  non-hierarchical => FLAT			*/ 
#define KFM_IDFILE_TYPE_INET	5	/*	Internet certifier.					*/

typedef void far * SCMCTX;
#define NULLSCMCTX ((SCMCTX) 0)
#define SC_MANIP_VERSION 1

/* Opcodes defined for SECManipulateSC
*/
#define SC_manip_GetVersion		   1
#define SC_manip_InitializeContext 2
#define SC_manip_TerminateContext  3
#define SC_manip_CheckCard         4
#define SC_manip_EnterIDFile	   5
#define SC_manip_EnterPIN		   6
#define SC_manip_SCEnableID		   7
#define SC_manip_PushInetKey	   8
#define SC_manip_FindMatchedCerts  9
#define SC_manip_GetMatchedCert	  10
#define SC_manip_ImportInetCert   11
/* Opcode added in 6.0.3 */
#define SC_manip_RefreshTokenInfo 12


/* Structure used for SC_manip_GetMatchedCerts 
*/
typedef struct
{
	DWORD Version;     /* This typedef describes version 0 */
	DWORD dwSize; 
	DWORD dwUnused;
	DWORD retIDLen;
	DWORD retLabelLen;
	DWORD retCertLen;
	DWORD IDOffset;    /* Offset in bytes from beginning of structure */
	DWORD LabelOffset; /* Offset in bytes from beginning of structure */
	DWORD CertOffset;  /* Offset in bytes from beginning of structure */
	/* ID */
	/* Label */
	/* Cert */
}
SC_MANIP_IMPORTABLE_CERT;

STATUS LNPUBLIC SECManipulateSC (DWORD  OpCode,
								 SCMCTX *pContext,
								 DWORD  *pdwFlags,
								 DWORD  *pdwParam1,
								 void   *pvParam2);


#ifdef __cplusplus
/* End brace for extern "C" */
}
#endif

#endif /* KFM_DEFS */


