
/* BSAFE (Security Package) interface definitions */

#ifndef	BSAFE_DEFS
#define	BSAFE_DEFS

#ifdef __cplusplus
extern "C" {
#endif

STATUS LNPUBLIC SECAttachToID (void);		/* Don't let parent process 	*/
											/* control our ID file.			*/
STATUS LNPUBLIC SECReattachToRootID (void);	/* Resync ID file with our		*/
											/* root process but remain		*/
											/* attached to the new	file.	*/

#define	fSECToken_EnableRenewal		0x0001

typedef struct
	{
	MEMHANDLE	mhName;
	MEMHANDLE	mhDomainList;
	WORD		wNumDomains;
	BOOL		bSecureOnly;
	MEMHANDLE	mhData;
	}
	SSO_TOKEN;

STATUS LNPUBLIC SECTokenGenerate(	
	char		*ServerName,	/* Reserved as NULL (ignored) */
	char		*OrgName,
	char		*ConfigName,	
	char		*UserName,
	TIMEDATE	*Creation,
	TIMEDATE	*Expiration,
	MEMHANDLE	*retmhToken,
	DWORD		dwReserved,
	void		*vpReserved
	);

STATUS LNPUBLIC SECTokenValidate(
	char		*ServerName,	/* Reserved as NULL (ignored) */
	char		*OrgName,
	char		*ConfigName,	
	char		*TokenData,
	char		*retUsername,
	TIMEDATE	*retCreation,
	TIMEDATE	*retExpiration,
	DWORD		dwReserved,
	void		*vpReserved
	);

void LNPUBLIC SECTokenFree(
	MEMHANDLE	*mhToken
	);


#ifdef __cplusplus
}
#endif

#endif /* BSAFE_DEFS */
