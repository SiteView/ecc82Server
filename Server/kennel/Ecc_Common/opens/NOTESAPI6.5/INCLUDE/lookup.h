#ifdef __cplusplus
extern "C" {
#endif

#ifndef LOOKUP_DEFS
#define LOOKUP_DEFS

#ifndef NSF_DATA_DEFS
#include "nsfdata.h"					/* We need NOTEID */
#endif


#ifndef NIF_DEFS
#include "nif.h"					/* We need HCOLLECTION */
#endif


/*	Name & Address Book lookup package definitions */

#define NAME_GET_AB_TITLES		0x0001
#define NAME_DEFAULT_TITLES		0x0002
#define NAME_GET_AB_FIRSTONLY	0x0004
#define NAME_GET_MAB_ONLY		0x0008	/* Get Master address book name only */
#define NAME_GET_ED_ONLY		0x0010  /* Get Server Based Enterprise Directory name only */
#define NAME_INCLUDE_ED 		0x0020  /* Include Server based ED as last book */
#define NAME_GET_ALL_EDS		0x0040  /* Get All enterprise Directories */
#define NAME_ADMIN_ONLY			0x0100  /* Include only NAB's that this server is the
											admin server of */
#define NAME_INCLUDE_CONFIGNAB	0x0200  /* Include Config (userless) NAB's */
#define NAME_CONFIG_ONLY		0x0400  /* Include First AB that has config info */
	

STATUS LNPUBLIC NAMEGetAddressBooks (
								const char far *pszServer,
								WORD wOptions,	/* NAME_GET_AB_xxx */
								WORD far *pwReturnCount,
								WORD far *pwReturnLength,
								HANDLE far *phReturn);

void LNPUBLIC		NAMEGetModifiedTime (TIMEDATE far *retModified);


STATUS LNPUBLIC	NAMELookup (const char far *ServerName, WORD Flags,
								WORD NumNameSpaces, const char far *NameSpaces,
								WORD NumNames, const char far *Names,
								WORD NumItems, const char far *Items,
								HANDLE far *rethBuffer);
STATUS LNPUBLIC	NAMELookup2 (const char far *ServerName, DWORD Flags,
								WORD NumNameSpaces, const char far *NameSpaces,
								WORD NumNames, const char far *Names,
								WORD NumItems, const char far *Items,
								HANDLE far *rethBuffer);

void far * LNPUBLIC NAMELocateNextName(void far *pLookup,
								void far *pName,
								WORD far *retNumMatches);
void far * LNPUBLIC NAMELocateNextName2(void far *pLookup,
								void far *pName,
								DWORD far *retNumMatches);

void far * LNPUBLIC NAMELocateNextMatch(void far *pLookup,
								void far *pName,
								void far *pMatch);
void far * LNPUBLIC NAMELocateNextMatch2(void far *pLookup,
								void far *pName,
								void far *pMatch);

void far * LNPUBLIC NAMELocateItem(void far *pMatch,
								WORD Item,
								WORD far *retDataType,
								WORD far *retSize);
void far * LNPUBLIC NAMELocateItem2(void far *pMatch,
								WORD Item,
								WORD far *retDataType,
								WORD far *retSize);

STATUS LNPUBLIC NAMEGetTextItem(void far *pMatch,
								WORD Item,
								WORD Member,
								char far *Buffer,
								WORD BufLen);
STATUS LNPUBLIC NAMEGetTextItem2(void far *pMatch,
								WORD Item,
								WORD Member,
								char far *Buffer,
								WORD BufLen);

STATUS LNPUBLIC NAMELocateMatchAndItem(void far *pLookup,
								WORD MatchNum,
								WORD Item,
								WORD far *retDataType,
								void far *retpMatch, 
								void far *retpItem,
								WORD far *retSize);
STATUS LNPUBLIC NAMELocateMatchAndItem2(void far *pLookup,
								DWORD MatchNum,
								WORD Item,
								WORD far *retDataType,
								void far *retpMatch, 
								void far *retpItem,
								WORD far *retSize);

/*	NAMELookup flags */

#define NAME_LOOKUP_ALL			0x0001	/* Return all entries in the view */
										/* (Note: a Names value of "" must also be specified) */
#define NAME_LOOKUP_NOSEARCHING	0x0002	/* Only look in first names database containing */
										/* desired namespace (view) for specified names */
										/* rather than searching other names databases */
										/* if name was not found.  Note that this may not */
										/* necessarily be the first names database in the */
										/* search path - just the first one containing */
										/* the desired view. */
#define NAME_LOOKUP_EXHAUSTIVE	0x0020	/* Do not stop searching when the first */
										/* matching entry is found. */
#define NAME_LOOKUP_UPDATE		0x0100	/* Force the namespaces (views) to be made current */


/*	NAMELookup programming notes: 

	NAMELookup offers the capability to lookup an arbitrary number of 
	"items" of information for an arbitrary number of "names" in a
	single procedure call.  Furthermore, NAMELookup may return 
	multiple "matches" for each name, each with the selected items of 
	information.  Finally, this lookup is performed in one or more
	"name spaces" which really refer to the names of views in the
	name & address book(s) on a specified server.

	Note the terminology: "names", "matches", and "items".  These 
	concepts relate directly to the API calls.

	An example will help to illustrate this.  Suppose a piece of mail
	is being sent to 3 names: Bill Smith, Ted Jones, and Marketing 
	(Marketing is the name of a group).

	For each user name, the mailer needs the mail domain, the mail server,
	and the public key.  For the group, the mailer needs the members of
	the group.

	So the "names" are Bill Smith, Ted Jones, and Marketing.

	The "items" are domain, server, public key, and members.

	However, if there are two individuals named "Bill Smith" NAMELookup
	will return two matches for the name "Bill Smith".  The mailer can
	then use the returned information (e.g. domain) to display a dialog
	box to ask the user which Bill Smith to send to (e.g. Bill Smith @ Iris
	or Bill Smith @ Lotus).

	In this example, the mailer doesn't know prior to the NAMELookup call
	which names refer to individuals and which names refer to groups.  
	The results from the NAMELookup can be used to determine this.
	For example, if for a given name, the member item is not returned,
	the name can be assumed to be an individual.  If the member item
	is returned, then it is a group.  The routine NAMELocateItem
	returns a null pointer and a zero size if the item is not present
	in this match.

	The following is a sample code fragment that illustrates the API calls
	to perform this NAMELookup:

	#define USERNAMESSPACE "$Users"
	#define Names			"Bill Smith\0Ted Jones\0Marketing"
	#define MAIL_LOOKUPITEMS "Domain\0Server\0PublicKey\0Members\0Certificate"
	#define ITEM_DOMAIN		 0
	#define ITEM_SERVER		 1
	#define ITEM_PUBLICKEY	 2
	#define ITEM_MEMBERS	 3
	#define	ITEM_CERTIFICATE 4
	
	NumNames = 3;							** 3 names to look up **
	NumItems = 5;
	error = NAMELookup(pMailServerName,		** Ptr to mail server name **
						0,					** Flags **
						1, 					** Number of name spaces **
						USERNAMESSPACE,		** Name of name space (view name) **
						NumNames,	 		** # of names **
						Names,				** Ptr to the names themselves **
						NumItems, 			** # of items desired **
						MAIL_LOOKUPITEMS,	** Ptr to the item names **
						&hLookup);			**  Handle of returned buffer **
	if (error)
		goto Abort;

	pLookup = OSLockObject(hLookup);

	**	For each name requested, traverse the results for each name **

	for (pName = NULL, i = 0; i < NumNames; i++)
		{
		**	Locate the set of matches for the next name searched for **

		pName = NAMELocateNextName(pLookup, pName, &NumMatches);

		**	Traverse all matches for this name **

		for (pMatch = NULL, j = 0; j < NumMatches; j++)
			{
			pMatch = NAMELocateNextMatch(pLookup, pName, pMatch);

			**	Here, handle a simple (non-list) item for this match **

			pDomain = NAMELocateItem(pMatch, ITEM_DOMAIN, &DataType, &Size);
			if (pDomain == NULL)		** if item not present in this match **
				continue;				** go on to next match **

			**	Here, handle a text list or text item (this handles
				both kinds) and traverse all members **

			for (k = 0; ; k++)
				{
				if (NAMEGetTextItem(pMatch, ITEM_MEMBERS, k, 
										Buffer, sizeof(Buffer)) != NOERROR)
					break;
				}
			}
		}
	OSUnlockObject(hLookup);

*/

/*	Structure of the header of the return buffer. */

typedef struct
	{
	WORD	Length;						/* Length of entire buffer */
	WORD	NumItems;					/* # items returned with each match */
/*	LOOKUP_INFO NameInfo[NumNames];		** Array of info for each name looked up */
	} LOOKUP_HEADER;


/*	Structure which is returned for each name to be looked up, in the
	same order as the names were provided in the request. */

typedef struct
	{
	WORD	NumMatches;					/* # records which match the name */
/*	LOOKUP_MATCH Matches[NumMatches];	** Array of match buffers... */
	} LOOKUP_INFO;


/*	Structure which is returned for every matching record of a name. */

typedef struct
	{
/*	WORD	Offset[NumItems];			** Array of offsets from base of */
										/* LOOKUP_MATCH structure to each */
										/* item value, in the same order */
										/* as the items were provided in */
										/* the request. */
	WORD	Length;						/* Length of LOOKUP_MATCH buffer */
										/* Acts as Offset[N] array entry */
										/* so that to find the length of */
										/* any item, you can subtract */
										/* Offset[i+1] - Offset[i]. */
/*	Item values (datatype and value)... */
	} LOOKUP_MATCH;




#endif


#ifdef __cplusplus
}
#endif
