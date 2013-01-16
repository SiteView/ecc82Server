#ifdef __cplusplus
extern "C" {
#endif

/*
	Public Definitions for Full Text Package
*/

#ifndef FT_DEFS
#define FT_DEFS

#ifndef NIF_DEFS
#include "nif.h"
#endif


/*	Define Indexing options */

#define FT_INDEX_REINDEX		0x0002		/* Re-index from scratch */
#define FT_INDEX_CASE_SENS		0x0004		/* Build case sensitive index */
#define FT_INDEX_STEM_INDEX		0x0008		/* Build stem index */
#define FT_INDEX_PSW			0x0010		/* Index paragraph & sentence breaks*/
#define FT_INDEX_OPTIMIZE		0x0020		/* Optimize index (e.g. for CDROM) (Not used) */
#define FT_INDEX_ATT			0x0040		/* Index Attachments */
#define FT_INDEX_ENCRYPTED_FIELDS	0x0080 	/* Index Encrypted Fields */
#define FT_INDEX_AUTOOPTIONS	0x0100		/* Get options from database */
#define FT_INDEX_SUMMARY_ONLY	0x0200		/* Index summary data only */
#define FT_INDEX_ATT_BINARY		0x1000		/* Index all attachments including BINARY formats */
/*	Define Search options */

#define FT_SEARCH_SET_COLL		0x00000001	/* Store search results in NIF collections;
												Don't return them to caller */
#define FT_SEARCH_NUMDOCS_ONLY	0x00000002	/* Return # hits only; not the documents */
#define FT_SEARCH_REFINE		0x00000004	/* Refine the query using the IDTABLE */
#define FT_SEARCH_SCORES		0x00000008	/* Return document scores (default sort) */
#define FT_SEARCH_RET_IDTABLE	0x00000010	/* Return ID table */
#define FT_SEARCH_SORT_DATE		0x00000020	/* Sort results by date */
#define FT_SEARCH_SORT_ASCEND	0x00000040	/* Sort in ascending order */
#define FT_SEARCH_TOP_SCORES	0x00000080	/* Use Limit arg. to return only top scores */
#define FT_SEARCH_STEM_WORDS	0x00000200	/* Stem words in this query */
#define FT_SEARCH_THESAURUS_WORDS	0x00000400	/* Thesaurus words in this query */
#define FT_SEARCH_FUZZY			0x00004000		/* set if fuzzy search wanted */
#define FT_SEARCH_EXT_RET_URL	0x00008000		/* Return url-based results (FTSearchExt only) */
#define FT_SEARCH_SORT_DATE_CREATED	0x00010000	/* Sort by created date (default is to sort by modified date) */
#define FT_SEARCH_EXT_DOMAIN	0x00040000		/* this is a domain search */
#define FT_SEARCH_EXT_FILESYSTEM	0x00100000	/* search the filesystem index (Domain Search only) */
#define FT_SEARCH_EXT_DATABASE	0x00200000		/* search the database index (Domain Search only) */

/*	Define search results data structure */

#define FT_RESULTS_SCORES		0x0001	/* Array of scores follows */
#define FT_RESULTS_EXPANDED		0x0002  /* Search results are series of FT_SEARCH_RESULT_ENTRY structures */
#define FT_RESULTS_URL			0x0004	/* Url expanded format returned by FTSearchExt only */

/*	Define search results entries for catalog index entries */

#define FT_RESULT_ENTRY_RESTRICTED			0x0001	/* Entry has read fields */
#define FT_RESULT_ENTRY_TITLE_RESTRICTED	0x0002	/* Title of Db is restricted */


/*	Struct returned by FTSearch when the FT_RESULTS_EXPANDED is specified.
	The rethResults parameters contains an array of these following the 
	FT_RESULTS struct. */

typedef struct
	{
	DBID		ReplicaID;
	UNID		Unid;
	UNID		ViewUnid;	
	TIMEDATE	ModifiedTime;		/* Last modified time of note */
	WORD		Flags;				/* Flags (FT_RESULT_ENTRY_xxxx) */
	WORD		ServerHintLength;	/* Length of Server hint */
	WORD		TitleLength;		/* Length of Databaes Title */
	WORD		CategoriesLength;	/* Length of Database Categories */
	WORD		HeadingLength;		/* Length of View Column Heading */
	WORD		SummaryLength;		/* Length of View Summary */
	BYTE		Score;				
	BYTE		Spare;				
	}
FT_SEARCH_RESULT_ENTRY;

/*	Struct returned by FTSearch in the rethResults parameter.  The data 
	following this struct depends on the FT_RESULTS_xxx flags returned
	from FTSearch.
	
		No Flags			An array of NoteIDs follows.

		FT_RESULTS_SCORES	A BYTE array of scores follows the NoteID
							array.  NOTE: A no-op if FT_RESULTS_EXPANDED
							is specified.

		FT_RESULTS_EXPANDED	An array of FT_SEARCH_RESULT_ENTRY's follows.
							That array is followed by Server, Title,
							Categories, Heading and Summary string fields 
*/

typedef struct
	{
	DWORD		NumHits;			/* Number of search hits following */
	WORD		Flags;				/* Flags (FT_RESULTS_xxx) */
	WORD		VarLength;			/* Length of variable strings after FT_SEARCH_RESULT_ENTRY array */
									/* Variable data follows (see above) */
	}
FT_SEARCH_RESULTS;


typedef struct
	{
	DWORD		DocsAdded;			/* # of new documents */
	DWORD		DocsUpdated;		/* # of revised documents */
	DWORD		DocsDeleted;		/* # of deleted documents */
	DWORD		BytesIndexed;		/* # of bytes indexed */
	}
FT_INDEX_STATS;


/*	Function prototypes */

STATUS LNPUBLIC FTIndex(DBHANDLE hDB, WORD Options, 
						char far *StopFile, FT_INDEX_STATS far *retStats);
STATUS LNPUBLIC FTDeleteIndex(DBHANDLE hDB);
STATUS LNPUBLIC FTGetLastIndexTime(DBHANDLE hDB, TIMEDATE far *retTime);

STATUS LNPUBLIC FTOpenSearch(HANDLE far *rethSearch);
STATUS LNPUBLIC FTSearch(DBHANDLE hDB, HANDLE far *phSearch, HCOLLECTION hColl,
							char far *Query,
							DWORD Options, WORD Limit, HANDLE hIDTable,
							DWORD far *retNumDocs,
							HANDLE far *Reserved,
							HANDLE far *rethResults);
STATUS LNPUBLIC FTCloseSearch(HANDLE hSearch);


/*	Extended Full Text Search API supporting url-style results,
	domain search, paged results, etc.
*/
STATUS LNPUBLIC FTSearchExt(	/* Extended full text search */
	DBHANDLE hDB, 				/* Database to search */
	HANDLE far *phSearch, 		/* Previously allocated search handle */
	HCOLLECTION hColl,
	char far *Query,			/* Pointer to search query */
	DWORD Options, 				/* Search options */
	WORD Limit, 				/* Overall limit for number of hits */
	HANDLE hIDTable,
	DWORD far *retNumDocs,		/* Number of documents returned in results */
	HANDLE far *Reserved,
	HANDLE far *rethResults,	/* results format depends on options selected */
	DWORD far *retNumHits,  	/* Actual number of hits this search */
	DWORD Start,				/* Paged results starting point */
	WORD Count,					/* Paged results retrieval count */
	WORD Arg,					/* Paged results additional argument */
	HANDLE hNames); 			/* User's Name List (NAMES_LIST) or NULL */


/*	Basic layout of url search results from FTSearchExt

	FT_SEARCH_RESULTS - one with FT_RESULTS_URL set to indicate this new expanded format.  
	FT_SEARCH_URL_RESULTS - one; always present when FT_RESULTS_URL set.
			
	If non-clustered:
	
		For each result (defined by NumHits):
		
			FT_SEARCH_URL_RESULT_ENTRY
				
		Variable length data contains:
			 Variable length data associated with the FT_SEARCH_URL_RESULTS
			 Variable length data associated with each FT_SEARCH_URL_RESULT_ENTRY in sequence
*/

/*	
	FT_SEARCH_URL_RESULTS
	 
	Appears after FT_SEARCH_RESULTS when FT_RESULTS_URL is set and contains additional
	information from extended, multi-database searches.
	
	Variable length data consists of:  
		reserved for future use
*/
typedef struct
{
	DWORD	HitCount;			/* Number of search hits (not necessarily number returned) */
	WORD	VarLength;			/* Length of variable length strings; currently unused. */

} FT_SEARCH_URL_RESULTS;

/*	FT_SEARCH_URL_RESULT_ENTRY

	Result entry returned by FTSearchExt when FT_RESULTS_URL set.
	If there is a nonzero UrlLength, this is for a "foreign" url and the
	string should be used.  Otherwise the ReplicaID/Unid/ViewUnid/ServerHint
	represent the hit.
	
	Variable length data consist of:
		url string
		server hint string
		db title string
		db categories strings (each null terminated; total size DbCategoriesLength)
		heading string
		doc summary string
		doc title string
		doc author string
		
*/
typedef struct
{
	TIMEDATE CreatedTime;		/* Creation time of note */
	TIMEDATE ModifiedTime;		/* Last modified time of note */
	WORD	Flags;				/* Flags */

	WORD	UrlLength;			/* Length of url string, or 0 if notes link */
	DBID	ReplicaID;			/* replica id */
	UNID	Unid;				/* unid of note */
	UNID	ViewUnid;			/* unid of view containing note */
	WORD	ServerHintLength;	/* Length of Server hint */

	WORD	DbTitleLength;		/* Length of Database Title */
	WORD	DbCategoriesLength;	/* Length of Database Categories */
	WORD	HeadingLength;		/* Length of View Column Heading */
	WORD	DocSummaryLength;	/* Length of Document Summary */
	WORD	DocTitleLength;		/* Length of Document Title */
	WORD	DocAuthorLength;	/* Length of Document Author */
	BYTE	Score;				/* Relevance score */
	BYTE	Spare;		   

} FT_SEARCH_URL_RESULT_ENTRY;



#endif	/* FT_DEFS */

#ifdef __cplusplus
}
#endif

