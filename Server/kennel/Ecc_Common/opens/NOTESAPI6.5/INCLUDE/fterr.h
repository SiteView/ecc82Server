
#ifndef FT_ERR_DEFS
#define FT_ERR_DEFS

/* FT (Full text) error status codes */

#define ERR_FT					PKG_FT+0
	errortext(ERR_FT,			"Unknown full-text error")
#define ERR_FT_RAW				PKG_FT+1
	errortext(ERR_FT_RAW,		"Error from full text package")
#define ERR_FT_NOT_INDEXED		PKG_FT+2  
	errortext(ERR_FT_NOT_INDEXED,"Database is not full-text indexed")
   

#define ERR_FT_INIT_API			PKG_FT+3
	errortext(ERR_FT_INIT_API,	"Unable to initialize full text package")
#define ERR_FT_NOT_FOUND		PKG_FT+4
	errortext(ERR_FT_NOT_FOUND,	"Full text index not found for this database")
#define ERR_FT_NO_EXIST			PKG_FT+5
	errortext(ERR_FT_NO_EXIST,	"Document is not in the search results")

#define LOG_FT_SUMM_ERR			PKG_FT+6
	errortext(LOG_FT_SUMM_ERR, "FT Summarize error - %s")

#define ERR_FT_DB_NOT_FOUND		PKG_FT+7
	errortext(ERR_FT_DB_NOT_FOUND,"Specified database is not being searched")
#define ERR_FT_HIGHLIGHTS		PKG_FT+8
	errortext(ERR_FT_HIGHLIGHTS,"Unable to obtain word highlights")
#define ERR_FT_BAD_QUERY		PKG_FT+9
	errortext(ERR_FT_BAD_QUERY,	"Query is not understandable")
#define	ERR_STALE_HIGHLIGHTS	PKG_FT+10
	errortext(ERR_STALE_HIGHLIGHTS,	"Highlights are not available; this document has been modified since being indexed")

#define ERR_FT_BAD_SEARCH_HANDLE PKG_FT+11
	errortext(ERR_FT_BAD_SEARCH_HANDLE,	"Unable to obtain search results; re-open database")
#define ERR_FT_IN_USE			PKG_FT+12
	errortext(ERR_FT_IN_USE,	"The full text index for this database is in use")
#define ERR_FT_REBUILD			PKG_FT+13
	errortext(ERR_FT_REBUILD,	"The full text index needs to be rebuilt")
#define ERR_FT_NEWER_VER		PKG_FT+14
	errortext(ERR_FT_NEWER_VER,	"The existing full text index was built by later version of this product")
#define ERR_FT_STRUCT_SIZE		PKG_FT+15
	internaltext(ERR_FT_STRUCT_SIZE,"(The allocated structure size is too small for this engine)")
#define ERR_FT_VER0				PKG_FT+16
	internaltext(ERR_FT_VER0,	"(The full text index is corrupt - version 0)")
#define ERR_FT_TOPIC			PKG_FT+17
	errortext(ERR_FT_TOPIC,		"Full text error; see log for more information")
#define ERR_FT_DIR_LINK			PKG_FT+18
	errortext(ERR_FT_DIR_LINK,	"Full text directory links must contain a valid directory name")
#define ERR_FT_BEING_INDEXED	PKG_FT+19
	errortext(ERR_FT_BEING_INDEXED,"Database is currently being indexed by another process")
#define ERR_FT_TEMP_MAXDOCS		PKG_FT+20
	errortext(ERR_FT_TEMP_MAXDOCS,	"Maximum allowable documents exceeded for a temporary full text index")


/*	Log message codes */

#define LOG_FT_TOPIC_MSG		PKG_FT+21
	errortext(LOG_FT_TOPIC_MSG,	"Full Text message: %s")
#define LOG_FT_TOPIC_ERR_STACK	PKG_FT+22
	errortext(LOG_FT_TOPIC_ERR_STACK,"Topic error stack [%lu]:  %s")
#define LOG_FT_DELETE_ERR		PKG_FT+23
	errortext(LOG_FT_DELETE_ERR,"Topic error %ld deleting document from full text index")
#define LOG_FT_LEGACY_ERR		PKG_FT+24
	errortext(LOG_FT_LEGACY_ERR,"Full Text Index structure for %s is incompatible. Index is being Rebuilt")
#define LOG_FT_INDEX_ERR		PKG_FT+25
	errortext(LOG_FT_INDEX_ERR, "Error full text indexing document NT%08lx %s %s")
#define LOG_FT_DOCNOTITLE		PKG_FT+26
	errortext(LOG_FT_DOCNOTITLE, "Document has no title")
#define LOG_FT_INDEX_MERGE_DISK_WARN	PKG_FT+28
	errortext(LOG_FT_INDEX_MERGE_DISK_WARN, "Warning: cannot merge full text index %p due to insufficient disk space")


/*	More error codes */

#define ERR_FT_DOMAIN_INDEX_LIMIT				PKG_FT+29
	errortext(ERR_FT_DOMAIN_INDEX_LIMIT,	"Full Text Error (FTG): Exceeded max configured index size while indexing document NT%08lx for database %p in domain index %p")

#define ERR_FT_MORE_THAN_5000_DOCS	PKG_FT+30
	errortext(ERR_FT_MORE_THAN_5000_DOCS,	"More than 5000 documents found; please refine the search")
#define ERR_FT_BAD_FIELD		PKG_FT+31
	errortext(ERR_FT_BAD_FIELD,	"The field name used in the query cannot be found in the database.")
#define ERR_FT_BAD_PAREN		PKG_FT+32
	errortext(ERR_FT_BAD_PAREN,	"Unbalanced parentheses in query.")
#define ERR_FT_BAD_OPERATOR		PKG_FT+33
	errortext(ERR_FT_BAD_OPERATOR,	"Unknown operator in field query.")
#define ERR_FT_NOMATCHES		PKG_FT+34
	errortext(ERR_FT_NOMATCHES,	"No documents found")
#define ERR_FT_TEXT_FIELD		PKG_FT+35
	errortext(ERR_FT_TEXT_FIELD,	"Relational operators are not supported in text fields")
#define ERR_FT_FIELD_NOT_INDEXED PKG_FT+36
	errortext(ERR_FT_FIELD_NOT_INDEXED,	"The query contains a new number or date field that was added after the full text index was created.  Please re-create the full text index to use this field in queries")
#define ERR_FT_INDEX_BAD		PKG_FT+37
	errortext(ERR_FT_INDEX_BAD,	"Full text index is missing a necessary field")
#define ERR_FT_TOPIC_NOSEE_LOG	PKG_FT+38
	errortext(ERR_FT_TOPIC_NOSEE_LOG,  "Full text error from Topic")
#define ERR_FT_TOPIC_CORRUPT_INDEX	PKG_FT+39
	errortext(ERR_FT_TOPIC_CORRUPT_INDEX,  "Full text index Corrupt, Try to Delete and Re-add Index")


/* String definitions */

#define STR_FT_LIBNAME_KEY		PKG_FT+40
	stringtext(STR_FT_LIBNAME_KEY,"FT")
#define ERR_FT_INDEXING_TOO_SLOW			PKG_FT+41
	stringtext(ERR_FT_INDEXING_TOO_SLOW,"Indexing rate too slow. (target ms/100 documents:%ld, actual:%ld) for %s")
#define STR_FT_PROGRESS			PKG_FT+42
	stringtext(STR_FT_PROGRESS,	"Indexing %,lu documents")
#define STR_FT_MERGING			PKG_FT+43
	stringtext(STR_FT_MERGING,	"Merging full text index; this may take a while...")
#define STR_FT_FINISH			PKG_FT+44
	stringtext(STR_FT_FINISH,	"Finishing full text index; this may take a while...")
#define STR_FT_KEYWORDS			PKG_FT+45
	stringtext(STR_FT_KEYWORDS,	"ACCRUE AND CONTAINS FIELD NEAR NOT OR PARAGRAPH SENTENCE TOPIC TERMWEIGHT EXACTCASE VERITYQUERY USETHESAURUS MATCHINGLEVEL IS PRESENT ")
#define ERR_FT_NEEDQUOTES		PKG_FT+46
	errortext(ERR_FT_NEEDQUOTES,	"Expected Verity format query must be inside double quotes")
#define ERR_FT_BADARG_STRUCT	PKG_FT+47
	errortext(ERR_FT_BADARG_STRUCT,	"Invalid Argument Structure")
#define ERR_FT_BADHANDLE_TYPE	PKG_FT+48
	errortext(ERR_FT_BADHANDLE_TYPE,	"Wrong Handle Type")
#define ERR_FT_SRCH_HDLOCK		PKG_FT+49
	errortext(ERR_FT_SRCH_HDLOCK,	"Warning - search handle remains locked")
/*	More error codes */

#define ERR_FT_NO_DOCS			PKG_FT+50
	errortext(ERR_FT_NO_DOCS,	"No documents specified")
#define ERR_FT_INVALIDDOC	PKG_FT+51
	errortext(ERR_FT_INVALIDDOC,	"Error in Full Text document streaming.  See preceding log messages.")
#define ERR_FT_LOW_MEMORY	PKG_FT+52
	errortext(ERR_FT_LOW_MEMORY,	"Not enough memory for Full Text Indexing or Search")
#define ERR_FT_DOWN		PKG_FT+53
	errortext(ERR_FT_DOWN,	"Full Text Index is Down for Repairs")
#define ERR_FT_INDEXING_FILE				PKG_FT+54
	errortext(ERR_FT_INDEXING_FILE,	"Full text indexing documents in (%p) '%p' for multi db index")
#define ERR_FT_INDEXING_DONE				PKG_FT+55
	errortext(ERR_FT_INDEXING_DONE,	"%lu documents (%lu bytes) indexed in (%p) '%p' for multi db index")
#define ERR_FT_NO_EXT_SUPPORT				PKG_FT+56
	errortext(ERR_FT_NO_EXT_SUPPORT,"Extended search features not supported by server")

#define ERR_FT_DOMAIN_POOLFULL				PKG_FT+57
	errortext(ERR_FT_DOMAIN_POOLFULL,	"Domain Search Pool is Full")
#define ERR_FT_FILTER_FILE				PKG_FT+58
	errortext(ERR_FT_FILTER_FILE,	"Full Text unable to Filter File")

#define ERR_FT_INDEX_LIMIT				PKG_FT+59
	errortext(ERR_FT_INDEX_LIMIT,	"Full Text Error (FTG): Exceeded max configured index size while indexing document NT%08lx in database index %p")
#define ERR_FT_NO_SPACE					PKG_FT+60
	errortext(ERR_FT_NO_SPACE,		"Full Text Error - not enough space on disk to build index")
#define ERR_FT_KV_INIT					PKG_FT+61
	errortext(ERR_FT_KV_INIT,		"Full Text Error - failure to init KV filter")

#define LOG_FT_KEYVIEW_ERR_DB		PKG_FT+62
	errortext(LOG_FT_KEYVIEW_ERR_DB, "FT KV Text Filter error processing NT%08lx, Attachment = %s in database %p - %s")

#define LOG_FT_KEYVIEW_ERR_FS		PKG_FT+63
	errortext(LOG_FT_KEYVIEW_ERR_FS, "FT KV Text Filter error processing %p - %s")

/*  now have up to PKG_FT + 63 */

#endif /* FT_ERR_DEFS */


