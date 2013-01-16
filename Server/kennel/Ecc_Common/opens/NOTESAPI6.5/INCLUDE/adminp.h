
#ifndef ADMINP_DEFS
#define ADMINP_DEFS

#ifdef __cplusplus
extern "C" {
#endif

#ifndef KFM_DEFS
#include "kfm.h"
#endif


/*      Values for "dwDeleteInNABType" field below. */

#define DELETE_PERSON_IN_NAB 1
#define DELETE_SERVER_IN_NAB 2
#define DELETE_GROUP_IN_NAB 3

typedef struct{
	DWORD           Flags;                          /* Reserved */

/* Original 4.x structure ended here.  The following fields were added for V5 */

	DWORD           dwDeleteInNABType;      /* DELETE_xxxx_IN_NAB defined above */
	char far*       chGroupName;            /* if dwDeleteInNABType equals DELETE_PERSON_IN_NAB
										a pointer to a group (termination perhaps) to have
										the name added */
	char far*       chAltName;                      /* if dwDeleteInNABType equals DELETE_PERSON_IN_NAB
										a pointer to the person's Alternate Name */
	char far*       chFirstName;            /* for ADMINReqMoveComplete, a pointer to a new first
										name for the person */  
	char far*       chMiddleInitial;        /* for ADMINReqMoveComplete, a pointer to a new middle
										initial for the person */  
	char far*       chLastName;                     /* for ADMINReqMoveComplete, a pointer to a new last
										name for the person */  
	char far*       chAltCommonName;        /* for ADMINReqRename, ADMINReqRecertify, and 
										ADMINReqMoveComplete, a pointer to a new alternate
										common name for the person */  
	char far*       chAltOrgUnitName;       /* for ADMINReqRename, ADMINReqRecertify, and 
										ADMINReqMoveComplete, a pointer to a new alternate   
										org unit for the person */  
	char far*       chAltLanguage;          /* for ADMINReqRename, ADMINReqRecertify, and 
										ADMINReqMoveComplete, a pointer to a new alternate
										language for the person */ 
	BOOL            fDontUseV1ChangeRequest;        /* for ADMINReqMoveUserInHier, TRUE indicates that 
											support for a simultaneous hierarchy move and name change.
											Recognized only by v5.02 and above clients and servers */

/* 5.x structure ended here.  The following fields were added for Rnext */

	DBHANDLE        dbhDirectory;		/* for ADMINReqDeleteInNAB, a handle to the directory from
											which the Person, Server, or Group is to be deleted if the 
											target directory is not names.nsf */
	}ADMINReqParams;

/* When the Administration Process passes parameters to an Addin process
	through an MQueue, the following structure defines the format of the 
	data the Addin process receives */

typedef struct{
	NOTEID RequestNoteId;   /* the note id of the Admin Request note */ 
	NOTEID ResponseNoteId;  /* the note id of the Admin Log note */ 
	} AdminpAddInMessage;

STATUS LNPUBLIC ADMINReqDeleteInNAB( DBHANDLE dbhAdmin4,
				     char far *chAuthor,
				     char far *chUserName,
				     char far *chMailServerName,
				     char far *chMailFileName,
				     char far *chDeleteMailFile,
									 ADMINReqParams far *arpAdminReqParamsPtr,
									 WORD wAdminReqParamsSize 
				     );
STATUS LNPUBLIC ADMINReqDeleteInACL( DBHANDLE dbhAdmin4,
				     char far *chAuthor,
				     char far *chUserName,
				     char far *chMailServerName,
				     char far *chMailFileName,
				     char far *chDeleteMailFile,
									 ADMINReqParams far* arpAdminReqParamsPtr,
									 WORD wAdminReqParamsSize 
				     );
STATUS LNPUBLIC ADMINReqChkAccessNewReplica( DBHANDLE dbhAdmin4,
					     char far *chAuthor,
					     char far *chSrcServer,
					     char far *chSrcPathName,
					     char far *chTitle,
					     DBREPLICAINFO *Replicainfo,
					     char far *chDesServer,
					     char far *chDesPathName,
											 ADMINReqParams far* arpAdminReqParamsPtr,
											 WORD wAdminReqParamsSize 
					     );
STATUS LNPUBLIC ADMINReqChkAccessMoveReplica( DBHANDLE dbhAdmin4,
					      char far *chAuthor,
					      char far *chSrcServer,
					      char far *chSrcPathName,
					      char far *chTitle,
					      DBREPLICAINFO *Replicainfo,
					      char far *chDesServer,
					      char far *chDesPathName,
											  ADMINReqParams far* arpAdminReqParamsPtr,
											  WORD wAdminReqParamsSize 
					      );
STATUS LNPUBLIC ADMINReqRename( HCERTIFIER      hCertCtx,
									  DBHANDLE        dbhNab,
									  NOTEHANDLE    nhNote,
									  char far    *pFirstName,
									  char far    *pMiddleInitial,  
									  char far    *pLastName,
									  char far    *pOU,
									  BOOL far    *retfWeLoggedThisEntry,
									  BOOL far    *retfFatalError,
											  ADMINReqParams far* arpAdminReqParamsPtr,
											  WORD wAdminReqParamsSize 
							  );
STATUS LNPUBLIC ADMINReqRecertify( HCERTIFIER   hCertCtx,
									     DBHANDLE           dbhNab,
									     NOTEHANDLE nhNote,
									     BOOL far   *retfWeLoggedThisEntry,
									     BOOL far   *retfFatalError,
												 ADMINReqParams far* arpAdminReqParamsPtr,
											     WORD wAdminReqParamsSize 
									     );
STATUS LNPUBLIC ADMINReqMoveUserInHier( HCERTIFIER      hCertCtx,
										  DBHANDLE        dbhNab,
										  NOTEHANDLE    nhNote,
										  char far    *pTargetCert,
										  BOOL far    *retfWeLoggedThisEntry,
										  BOOL far    *retfFatalError, 
													  ADMINReqParams far* arpAdminReqParamsPtr,
												  WORD wAdminReqParamsSize 
										  );
STATUS LNPUBLIC ADMINReqMoveComplete( HCERTIFIER        hCertCtx,
										      DBHANDLE          dbhAdmin4,
										      NOTEHANDLE        nhAdminReq,
										      char far    *pTargetCert,
										      BOOL far    *retfWeLoggedThisEntry,
										      BOOL far    *retfFatalError,
													  ADMINReqParams far* arpAdminReqParamsPtr,
												  WORD wAdminReqParamsSize 
										      );
STATUS LNPUBLIC ADMINReqUpgradeToHier(	HCERTIFIER	hCertCtx,
								DBHANDLE	dbhNab,
								NOTEHANDLE	nhNote,
								char far    *pOU,
								BOOL far    *retfWeLoggedThisEntry,
								BOOL far    *retfFatalError,
								ADMINReqParams far* arpAdminReqParamsPtr,
								WORD		wAdminReqParamsSize 
							  );


#ifdef __cplusplus
}

#endif

#endif  /*      ADMINP_DEFS     */




