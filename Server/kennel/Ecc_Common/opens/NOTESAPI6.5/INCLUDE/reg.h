#ifdef __cplusplus
extern "C" {
#endif

#ifndef	USERREG_DEFS
#define	USERREG_DEFS

#ifndef NAME_DEFS
#include "names.h"
#endif

#ifndef KFM_DEFS
#include "kfm.h"
#endif

#ifndef OSFILE_DEFS
#include "osfile.h"
#endif

#ifndef NSF_DATA_DEFS
#include "nsfdata.h"			/* for DBHANDLE */
#endif

/* 	Definitions ---------------------------------------------------------- */
/* The following InfoType codes are defined for REGGetIDInfo */
/* Note that the Certifier Flag can only exist on a hierarchical ID */
/* and that Certifier, NotesExpress, and Desktop flags are not */
/* present in safe copies of ID files */

#define REGIDGetUSAFlag				1
/* Data structure returned is BOOL */

#define REGIDGetHierarchicalFlag	2
/* Data structure returned is BOOL */

#define REGIDGetSafeFlag			3
/* Data structure returned is BOOL */

#define REGIDGetCertifierFlag		4
/* Data structure returned is BOOL */

#define REGIDGetNotesExpressFlag	5
/* Data structure returned is BOOL */

#define REGIDGetDesktopFlag			6
/* Data structure returned is BOOL */

#define REGIDGetName				7
/* Data structure returned is char xx[MAXUSERNAME] */

#define REGIDGetPublicKey			8
/* Data structure returned is char xx[xx] */

#define REGIDGetPrivateKey			9
/* Data structure returned is char xx[xx] */

#define REGIDGetIntlPublicKey		10
/* Data structure returned is char xx[xx] */

#define REGIDGetIntlPrivateKey		11
/* Data structure returned is char xx[xx] */


typedef void (LNCALLBACKPTR REGSIGNALPROC)(char far *);
typedef WORD REGFlags;
typedef DWORD REGFlagsExt;

/* The following bits are defined in REGFlags
*
*  NOTE: fREGCreateMailFileUsingAdminp and fREGCreateMailFileNow are mutually exclusive.
*  If both flags are specified, then fREGCreateMailFileNow is the action
*  that will be performed.
*/
#define fREGCreateIDFileNow				0x0001
#define fREGUSARequested				0x0002
#define fREGCreateMailFileNow			0x0004
#define fREGCreateAddrBookEntry			0x0008
#define fREGOkayToModifyID				0x0010
#define fREGOkayToModifyAddrbook		0x0020
#define fREGSaveIDInFile				0x0040
#define fREGCreateLimitedClient			0x0080
#define fREGCreateDesktopClient			0x0100
#define fREGSaveIDInAddrBook			0x0200
#define fREGCreateMailFileUsingAdminp	0x0800	/* use admin process to create mail file */
#define fREGSetInternetPassword			0x1000

#define fREGExtCreateMailFTIndex		0x00000001
#define fREGExtReturnPersonNote			0x00000002
#define fREGExtEnforceUniqueShortName	0x00000004

/*  Use the structure below with REGNewUser */
typedef struct
	{
	char 	*LastName;
	char 	*FirstName;
	char 	*MidInitial;
	char 	*OrgUnit;
	char 	*ShortName;
	char 	*AlternateName;
	char	*AltOrgUnit;
	char	*AltLanguage;
	DWORD	Spare[4];

	} REG_USERNAME_INFO;

typedef struct
	{
	WORD 	MailSystem;
	WORD 	MailOwnerAccess;
	DWORD	DbQuotaSizeLimit;
	DWORD	DbQuotaWarningThreshold;
	char 	*pMailServerName;
	char 	*pMailFileName;
	char 	*pMailTemplateName;
	char 	*pMailForwardAddress;
	char 	*pMailACLManager;
	DWORD	Spare[4];

	} REG_MAIL_INFO;


/*	REG_MAIL_OWNER_ACL_XXX defines used with
	MailOwnerAccess element of REG_MAIL_INFO struct */
#define REG_MAIL_OWNER_ACL_MANAGER		0
#define REG_MAIL_OWNER_ACL_DESIGNER		1
#define REG_MAIL_OWNER_ACL_EDITOR		2


/*	Routines -------------------------------------------------------------- */

STATUS LNPUBLIC	 REGNewWorkstation (HCERTIFIER	hCertCtx,
									WORD		MakeIDType,
									char 	far *RegServer,
								  	char 	far *OrgUnit,
									char 	far *LastName, 
									char 	far *FirstName, 
									char 	far *MidInitial, 
									char 	far *Password,
									char 	far *IDFileName,
									char 	far *Location,
									char 	far *Comment,
									WORD		MailSystem,
									char 	far *MailServerName,
									char 	far *MailFileName,
									char 	far *ForwardAddress,
									REGFlags	Flags,
									WORD		MinPasswordLength,
									REGSIGNALPROC signalstatus,
									char	far *ErrorPathName);

STATUS LNPUBLIC	 REGNewWorkstationExtended (HCERTIFIER	hCertCtx,
									WORD		MakeIDType,
									char 	far *RegServer,
								  	char 	far *OrgUnit,
									char 	far *LastName, 
									char 	far *FirstName, 
									char 	far *MidInitial, 
									char 	far *Password,
									char 	far *IDFileName,
									char 	far *Location,
									char 	far *Comment,
									WORD		MailSystem,
									char 	far *MailServerName,
									char 	far *MailFileName,
									char 	far *ForwardAddress,
									char	far *ProfileName,
									char	far	*LocalAdminName,
									REGFlags	Flags,
									WORD		MinPasswordLength,
									REGSIGNALPROC signalstatus,
									char	far *ErrorPathName);

STATUS LNPUBLIC REGNewUser (	HCERTIFIER	hCertCtx,
						   		WORD		MakeIDType,
								char 	far *RegServer,
								REG_USERNAME_INFO far *RegUserNameInfo,
								REG_MAIL_INFO far *RegMailInfo,
								char 	far *Password,
								void	far	*pGroupList,
								char 	far *IDFileName,
								char 	far *Location,
								char 	far *Comment,
								char 	far *InternetAddress,
								char	far *ProfileName,
								char	far	*LocalAdminName,
								REGFlags	Flags,
								REGFlagsExt	FlagsExt,
								WORD		MinPasswordLength,
								NOTEHANDLE far	*phUserNote,
								DBHANDLE far	*phUserNoteNAB,
								REGSIGNALPROC signalstatus,
								char	far *ErrorPathName,
								void	far *Reserved,
								void	far *Spare);

STATUS LNPUBLIC	 REGNewServer	   (HCERTIFIER	hCertCtx,
									WORD		MakeIDType,
									char 	far *RegServer,
								  	char 	far *OrgUnit,
									char 	far *EntryName, 
									char 	far *Password,
									char 	far *IDFileName,
									char 	far *Location,
									char 	far *Comment,
									char 	far *DomainName,
									char 	far *NetworkName,
									char 	far *AdminName,
									char 	far *ServerTitle,
									REGFlags	Flags,
									WORD		MinPasswordLength,
									REGSIGNALPROC signalstatus,
									char	far *ErrorPathName);

STATUS LNPUBLIC	 REGNewServerExtended	   (HCERTIFIER	hCertCtx,
									WORD		MakeIDType,
									char 	far *RegServer,
								  	char 	far *OrgUnit,
									char 	far *EntryName, 
									char 	far *Password,
									char 	far *IDFileName,
									char 	far *Location,
									char 	far *Comment,
									char 	far *DomainName,
									char 	far *NetworkName,
									char 	far *AdminName,
									char 	far *ServerTitle,
									char	far	*LocalAdminName,
									REGFlags	Flags,
									WORD		MinPasswordLength,
									REGSIGNALPROC signalstatus,
									char	far *ErrorPathName);

STATUS LNPUBLIC	 REGNewCertifier (HCERTIFIER	hCertCtx,
									WORD		MakeIDType,
									char	far	*RegServer,
									char 	far *Country,
								  	char 	far *Org,
								  	char 	far *OrgUnit,
									char 	far *Password,
									char 	far *IDFileName,
									char 	far *Location,
									char 	far *Comment,
									char 	far *MailServerName,
									char 	far *MailFileName,
									char 	far *ForwardAddress,
									REGFlags	Flags,
									WORD		KeyWidth,
									WORD		MinPasswordLength,
									REGSIGNALPROC signalstatus,
									char	far	*ErrorPathName);

STATUS LNPUBLIC	 REGFindAddressBookEntry(
									DBHANDLE hAddressBook, 
				 					char far *NameSpace,
									char far *Name, 
									NOTEID far *EntryNoteID);

STATUS LNPUBLIC	 REGCrossCertifyID (HCERTIFIER		hCertCtx,
									 WORD			Spare1,
									 char far		*RegServer,
									 char far		*IDFileName,
									 char far		*Location,
									 char far		*Comment,
									 char far		*ForwardAddress,
									 WORD			Spare2,
									 REGSIGNALPROC 	signalstatus,
									 char far	 	*ErrorPathName);

STATUS LNPUBLIC	 REGGetIDInfo 	   (char	far	*IDFileName,
									WORD		InfoType,
									void	far *OutBufr,
									WORD		OutBufrLen,
									WORD	far *ActualLen);

STATUS LNPUBLIC	 REGReCertifyID	(HCERTIFIER		hCertCtx,
									 WORD			Spare1,
									 char far		*RegServer,
									 char far		*OrgUnit,
									 char far		*IDFileName,
									 TIMEDATE far	*ExpirationTime,
									 WORD			Spare2,
									 VOID far		*Spare3,
									 REGSIGNALPROC 	signalstatus,
									 char far	 	*ErrorPathName);


#endif


#ifdef __cplusplus
}
#endif
