
/* ODS Error Codes */

#ifndef ODS_ERR_DEFS
#define ODS_ERR_DEFS

#define ERR_ODS			PKG_ODS+1
	errortext(ERR_ODS,		"This database cannot be read due to an invalid on disk structure")
#define ERR_DATATYPE	PKG_ODS+2
	errortext(ERR_DATATYPE,	"Cannot convert field - unsupported datatype")
#define ERR_ODS_TEXT_TOOBIG		PKG_ODS+3
	errortext(ERR_ODS_TEXT_TOOBIG,"Text is too big")
#define ERR_ODS_ENUM_COMPLETE	PKG_ODS+4
	internaltext(ERR_ODS_ENUM_COMPLETE,"Done enumerating CD buffer")
#define ERR_ODS_NO_SUCH_ENTRY	PKG_ODS+5
	apitext(ERR_ODS_NO_SUCH_ENTRY,"No such entry")
#define ERR_ODS_FILE_ZEROLENGTH		PKG_ODS+6
	errortext(ERR_ODS_FILE_ZEROLENGTH,"Zero length scratch file")
#define ERR_ODS_REC_ZEROLENGTH		PKG_ODS+7
	errortext(ERR_ODS_REC_ZEROLENGTH,"Encountered zero length record.")
#define ERR_ODS_REC_UNKNOWN			PKG_ODS+8
	errortext(ERR_ODS_REC_UNKNOWN,	"Encountered unknown record type.")
#define ERR_ODS_SHORT_INPUT			PKG_ODS+9
	debugtext(ERR_ODS_SHORT_INPUT,	"Input text is too short to be canonicalized")
#define ERR_ODS_SHORT_BUFFER		PKG_ODS+10
	errortext(ERR_ODS_SHORT_BUFFER,	"Input buffer is too small for read")
#define ERR_ODS_INVALID_LIST_ENTRY_COUNT	PKG_ODS+11
	errortext(ERR_ODS_INVALID_LIST_ENTRY_COUNT, "List Entry count in TEXT_LIST is invalid.")
#define ERR_ODS_INVALID_TIME_RANGE_ENTRY_COUNT	PKG_ODS+12
	errortext(ERR_ODS_INVALID_TIME_RANGE_ENTRY_COUNT, "Range List or Range Entry count in TIME_RANGE is invalid.")
#define ERR_ODS_INVALID_SCHED_LIST_EXT		PKG_ODS+13
	errortext(ERR_ODS_INVALID_SCHED_LIST_EXT, "Invalid SCHED_ENTRY_EXT size.")
#define ERR_ODS_INVALID_SCHED_DETAIL_LIST	PKG_ODS+14
	errortext(ERR_ODS_INVALID_SCHED_DETAIL_LIST, "Invalid SCHED_DETAIL_LIST size.")

	
/* NOTE:	PKG_CODE+47 is the LAST code available for this file.  See */
/* globerr.h for dependency which restricts this string ID space. */

#endif

