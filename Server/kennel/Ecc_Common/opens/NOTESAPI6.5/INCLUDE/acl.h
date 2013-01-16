#ifdef __cplusplus
extern "C" {
#endif


/* Access Control List Package */

#ifndef ACL_DEFS
#define ACL_DEFS

/*	ACL-wide flags.  Get and set with ACLGetFlags and ACLSetFlags */

#define ACL_UNIFORM_ACCESS		0x00000001	/* Require same ACL in ALL replicas of database */
/* Access Levels  */

#define ACL_LEVEL_NOACCESS	0
#define ACL_LEVEL_DEPOSITOR	1
#define ACL_LEVEL_READER	2
#define ACL_LEVEL_AUTHOR	3
#define ACL_LEVEL_EDITOR	4
#define ACL_LEVEL_DESIGNER	5
#define ACL_LEVEL_MANAGER	6

#define ACL_LEVEL_HIGHEST	6			/* Highest access level */
#define ACL_LEVEL_COUNT		7			/* Number of access levels */


#define ACL_LEVEL_STRINGMAX	128			/* size to allocate for access descriptors */


/* Named privilege parameters */

#define ACL_PRIVCOUNT		80			/* Number of privilege bits (10 bytes) */
#define ACL_PRIVNAMEMAX		16			/* Privilege name max (including null) */
#define ACL_PRIVSTRINGMAX	16+2		/* Privilege string max */
										/* (including parentheses and null) */

#define ACL_BITPRIVCOUNT	5			/* Original "bit" privileges count */
#define ACL_BITPRIVS		0x1f		/* Original "bit" privileges mask */

#define ACL_BITPRIV_LEFT_PAREN '('		/* Original "bit" privilege name syntax */
#define ACL_BITPRIV_RIGHT_PAREN ')'		/* Original "bit" privilege name syntax */

#define ACL_SUBGROUP_LEFT_PAREN '['		/* Subgroup name syntax */
#define ACL_SUBGROUP_RIGHT_PAREN ']'	/* Subgroup name syntax */

/*  Access level modifier flags */

#define ACL_FLAG_AUTHOR_NOCREATE	0x0001	/* Authors can't create new notes 
												(only edit existing ones) */
#define ACL_FLAG_SERVER				0x0002	/* Entry represents a Server (V4) */
#define ACL_FLAG_NODELETE			0x0004	/* User cannot delete notes */

#define ACL_FLAG_CREATE_PRAGENT 	0x0008	/* User can create personal agents (V4) */
#define ACL_FLAG_CREATE_PRFOLDER	0x0010	/* User can create personal folders (V4) */
#define ACL_FLAG_PERSON				0x0020 	/* Entry represents a Person (V4) */
#define ACL_FLAG_GROUP				0x0040 	/* Entry represents a group (V4) */
#define ACL_FLAG_CREATE_FOLDER		0x0080	/* User can create and update shared views & folders (V4)
												This allows an Editor to assume some Designer-level access */
#define ACL_FLAG_CREATE_LOTUSSCRIPT	0x0100	/* User can create LotusScript */
#define ACL_FLAG_PUBLICREADER		0x0200  /* User can read public notes */
#define ACL_FLAG_PUBLICWRITER		0x0400  /* User can write public notes */
#define ACL_FLAG_MONITORS_DISALLOWED	0x800	/* User CANNOT register monitors for this database */
#define ACL_FLAG_NOREPLICATE		0x1000	/* User cannot replicate or copy this database */
#define ACL_FLAG_ADMIN_READERAUTHOR 0X4000	/* Admin server can modify reader and author fields in db */
#define ACL_FLAG_ADMIN_SERVER		0x8000	/* Entry is administration server (V4) */

/* ACLUpdateEntry flags - Set flag if parameter is being modified */

#define ACL_UPDATE_NAME 			0x01
#define ACL_UPDATE_LEVEL 			0x02
#define ACL_UPDATE_PRIVILEGES		0x04
#define ACL_UPDATE_FLAGS 			0x08

/* Usernames list structure */

typedef struct {
	WORD		NumNames;			/*  Number of names in list */
	LICENSEID	License;			/*	User's license - now obsolete 	*/
									/*	MUST BE ZERO. 					*/

	#if defined(UNIX) || defined(OS2_2x) || defined(W32)
	DWORD		Authenticated;		/*	Authentication flags */
	#else							
	WORD		Authenticated;
	#endif
									/*  Names follow as packed ASCIZ strings */
									/*  First name is Username. */
									/*  Subsequent names are ALL the group */
									/*	names that User is a member of */
									/*	(directly or indirectly). */
	} NAMES_LIST;

/*	Defines for Authentication flags */

#define NAMES_LIST_AUTHENTICATED 			0x0001	/* 	Set if names list has been 	*/
													/*	authenticated via Notes		*/
#define NAMES_LIST_PASSWORD_AUTHENTICATED 	0x0002	/* 	Set if names list has been 	*/
													/*	authenticated using external */
													/*	password -- Triggers "maximum */
													/*	password access allowed" feature */
#define NAMES_LIST_FULL_ADMIN_ACCESS	 	0x0004	/* 	Set if user requested full admin access and it was granted */

/* Privileges bitmap structure */

typedef struct {BYTE BitMask[10];} ACL_PRIVILEGES;

/* Privileges bitmap access */

#define ACLIsPrivSet(privs, num)	((privs).BitMask[num / 8] &   (1 << (num % 8)))
#define ACLSetPriv(privs, num)		((privs).BitMask[num / 8] |=  (1 << (num % 8)))
#define ACLClearPriv(privs, num)	((privs).BitMask[num / 8] &= ~(1 << (num % 8)))
#define ACLInvertPriv(privs, num)	((privs).BitMask[num / 8] ^=  (1 << (num % 8)))

/* Public Functions */

STATUS LNPUBLIC ACLLookupAccess (HANDLE hACL,
								NAMES_LIST far *pNamesList,
								WORD far *retAccessLevel,
								ACL_PRIVILEGES far *retPrivileges,
								WORD far *retAccessFlags,
								HANDLE far *rethPrivNames);
STATUS LNPUBLIC ACLCreate (HANDLE far *rethACL);
STATUS LNPUBLIC ACLAddEntry (HANDLE hACL,
								const char far *Name,
								WORD AccessLevel,
								ACL_PRIVILEGES far *Privileges,
								WORD AccessFlags);
STATUS LNPUBLIC ACLDeleteEntry (HANDLE hACL,
								const char far *Name);
STATUS LNPUBLIC ACLUpdateEntry (HANDLE hACL,
								const char far *Name,
								WORD UpdateFlags,
								const char far *NewName,
								WORD NewAccessLevel,
								ACL_PRIVILEGES far *NewPrivileges,
								WORD NewAccessFlags);
STATUS LNPUBLIC ACLEnumEntries (HANDLE hACL,
								void (LNCALLBACKPTR EnumFunc)(void far *EnumFuncParam,
														char far *Name,
														WORD AccessLevel,
														ACL_PRIVILEGES far *Privileges,
														WORD AccessFlags),
								void far *EnumFuncParam);
STATUS LNPUBLIC ACLGetPrivName (HANDLE hACL,
								WORD PrivNum,
								char far *retPrivName);
STATUS LNPUBLIC ACLSetPrivName (HANDLE hACL,
								WORD PrivNum,
								char far *PrivName);

STATUS LNPUBLIC ACLGetHistory (HANDLE hACL, HANDLE far *hHistory, WORD far *HistoryCount);
STATUS LNPUBLIC ACLGetFlags (HANDLE hACL, DWORD far *Flags);
STATUS LNPUBLIC ACLSetFlags (HANDLE hACL, DWORD Flags);
STATUS LNPUBLIC ACLGetAdminServer (HANDLE hList, char far *ServerName);
STATUS LNPUBLIC ACLSetAdminServer (HANDLE hList, char far *ServerName);

#endif

#ifdef __cplusplus
}
#endif
