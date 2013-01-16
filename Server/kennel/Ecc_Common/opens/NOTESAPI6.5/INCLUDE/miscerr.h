
/* MISC error codes */

#ifndef MISCERR_DEFS
#define MISCERR_DEFS


#define	ERR_QUEUE_NOT_EMPTY				PKG_MISC+1
	internaltext(ERR_QUEUE_NOT_EMPTY,	"Deleting a non-empty queue")
#define ERR_QUEUE_EMPTY					PKG_MISC+2
	internaltext(ERR_QUEUE_EMPTY,		"No more entries to dequeue")
#define ERR_TEXT_PATTERN				PKG_MISC+3
	errortext(ERR_TEXT_PATTERN,			"Invalid text pattern")
#define ERR_NOT_FOUND					PKG_MISC+4
	errortext(ERR_NOT_FOUND,			"Entry not found in index")
#define ERR_FLOAT_CONVERSION			PKG_MISC+5
	errortext(ERR_FLOAT_CONVERSION,		"Cannot convert text to a number")
#define ERR_NO_SUBINDEX					PKG_MISC+6
	internaltext(ERR_NO_SUBINDEX,		"No subindex available")
#define ERR_NOT_EMPTY					PKG_MISC+7
	internaltext(ERR_NOT_EMPTY,			"Subindex cannot be deleted while non-empty")
#define ERR_DUPLICATE_KEY				PKG_MISC+8
	errortext(ERR_DUPLICATE_KEY,		"Entry already in index")
#define ERR_BUFFER_OVERFLOW				PKG_MISC+9
	errortext(ERR_BUFFER_OVERFLOW,		"Output Buffer Overflow")
#define ERR_IFMT						PKG_MISC+10
	errortext(ERR_IFMT,					"Invalid International Format Specifier")
#define ERR_TFMT						PKG_MISC+11
	errortext(ERR_TFMT,					"Invalid Time/Date Format Specifier")
#define	ERR_TDI_CONV					PKG_MISC+12
	errortext(ERR_TDI_CONV,				"Unable to interpret Time or Date")
#define	ERR_TDO_CONV					PKG_MISC+13
	errortext(ERR_TDO_CONV,				"Invalid Time or Date Encountered")
#define ERR_NAME_LENGTH					PKG_MISC+14
	errortext(ERR_NAME_LENGTH,			"Name too long")
#define ERR_VALUE_LENGTH				PKG_MISC+15
	errortext(ERR_VALUE_LENGTH,			"Value too long")
#define ERR_NOT_TDPAIR					PKG_MISC+16
	internaltext(ERR_NOT_TDPAIR,		"TIMEDATE, not TIMEDATE_PAIR")
#define ERR_TREQ						PKG_MISC+17
	errortext(ERR_TREQ,					"TIME must be specified, not just DATE")
#define ERR_MAXINTERVAL					PKG_MISC+18
	errortext(ERR_MAXINTERVAL,			"Maximum interval is 24 Hours")
#define ERR_HUFF0						PKG_MISC+19
	internaltext(ERR_HUFF0,				"There is at most 1 code with non-zero frequency")
#define ERR_HUFF1						PKG_MISC+20
	errortext(ERR_HUFF1,				"Invalid Encoded Data - Attachment may be corrupted")
#define ERR_HUFF2						PKG_MISC+21
	errortext(ERR_HUFF2,				"Encoded Data Checksum Mismatch - Attachment may be corrupted")
#define ERR_HUFF3						PKG_MISC+22
	errortext(ERR_HUFF3,				"Corrupted Encoded Data")
#define ERR_POOLFULL					PKG_MISC+23
	errortext(ERR_POOLFULL,				"Insufficient memory - a Notes memory pool is full (P)")
#define ERR_DISCARDED					PKG_MISC+24
	debugtext(ERR_DISCARDED,			"Pool cannot be locked or faulted")
#define ERR_INVALID_POOL				PKG_MISC+25
	errortext(ERR_INVALID_POOL,			"Index (pool) has been corrupted - will be rebuilt automatically when database is closed or server is restarted")
#define ERR_POOL_FREE_CHAIN				PKG_MISC+26
	debugtext(ERR_POOL_FREE_CHAIN,		"Invalid pool free chain")
#define ERR_LOCK_POOL_TWICE				PKG_MISC+27
	debugtext(ERR_LOCK_POOL_TWICE,		"Attempt to lock twice using same pool lock")
#define ERR_TOO_MANY_LOCKS				PKG_MISC+28
	debugtext(ERR_TOO_MANY_LOCKS,		"Too many locks on pool")
#define ERR_INVALID_BLOCK				PKG_MISC+29
	debugtext(ERR_INVALID_BLOCK,		"Invalid block handle")
#define ERR_MEMBER_NUMBER				PKG_MISC+30
	errortext(ERR_MEMBER_NUMBER,		"Index (link) has been corrupted - will be rebuilt automatically when database is closed or server is restarted")
#define ERR_ALREADY_SUBINDEX 			PKG_MISC+31
	internaltext(ERR_ALREADY_SUBINDEX,	"Subindex already created")
#define ERR_ACL_VERSION					PKG_MISC+32
	errortext(ERR_ACL_VERSION,			"Access control list version is unsupported")
#define ERR_ACL_CORRUPT					PKG_MISC+33
	errortext(ERR_ACL_CORRUPT,			"Access control list is corrupted")
#define ERR_ACL_FULL					PKG_MISC+34
	errortext(ERR_ACL_FULL,				"The access control list is full.  If you need to add new users, place existing users into groups on the list.")
#define ERR_ACL_INLIST					PKG_MISC+35
	errortext(ERR_ACL_INLIST,			"The name is already in the list.")
#define ERR_ACL_NOENTRY					PKG_MISC+36
	errortext(ERR_ACL_NOENTRY,			"The name is not in the list.")
#define ERR_ACL_UNDELETABLE				PKG_MISC+37
	errortext(ERR_ACL_UNDELETABLE,		"Default cannot be deleted.")
#define ERR_BAD_DATE_FMT				PKG_MISC+38
	errortext(ERR_BAD_DATE_FMT,			"Unrecognized DATE format specifier")
#define ERR_BAD_TIME_FMT				PKG_MISC+39
	errortext(ERR_BAD_TIME_FMT,			"Unrecognized TIME format specifier")
#define ERR_BAD_ZONE_FMT				PKG_MISC+40
	errortext(ERR_BAD_ZONE_FMT,			"Unrecognized ZONE format specifier")
#define ERR_BAD_STRUCT_FMT				PKG_MISC+41
	errortext(ERR_BAD_STRUCT_FMT,		"Unrecognized date/time STRUCTURE format specifier")
#define ERR_UNKNOWN_TFMT				PKG_MISC+42
	errortext(ERR_UNKNOWN_TFMT,			"Unrecognized date/time format specifier")
#define ERR_UNKNOWN_NFMT				PKG_MISC+43
	errortext(ERR_UNKNOWN_NFMT,			"Unrecognized number format specifier")
#define ERR_NO_MANAGER					PKG_MISC+44
	errortext(ERR_NO_MANAGER,			"Access control list must contain at least one Manager")
#define ERR_FREE_BLOCK_TWICE 			PKG_MISC+45
	debugtext(ERR_FREE_BLOCK_TWICE,		"Attempt to free block that is already free")
#define ERR_SERVER_ERR_MSG				PKG_MISC+46
	stringtext(ERR_SERVER_ERR_MSG,		"Server error: ")
#define ERR_WRONG_LICENSE				PKG_MISC+47
	errortext(ERR_WRONG_LICENSE,		"Your name is listed to access this database, but with a different ID")
#define ERR_CTF_FILEOPEN				PKG_MISC+48
	errortext(ERR_CTF_FILEOPEN,			"Error opening Character Set Translation File")
#define ERR_CTF_FILEREAD				PKG_MISC+49
	errortext(ERR_CTF_FILEREAD,			"Error reading Character Set Translation File")
#define ERR_DELIVERY_TIMEOUT			PKG_MISC+50
	errortext(ERR_DELIVERY_TIMEOUT, 	"Delivery time expired")
#define ERR_SHOW_TIMER1					PKG_MISC+51
	debugtext(ERR_SHOW_TIMER1,			"%ld msec%n")
#define ERR_SHOW_TIMER2					PKG_MISC+52
	debugtext(ERR_SHOW_TIMER2,			"%ld msec total, %ld iter -> %ld usec/iter%n")
#define ERR_POOL_MARKER1				PKG_MISC+53
	debugtext(ERR_POOL_MARKER1,			"Memory preceding pool block has been destroyed")
#define ERR_POOL_MARKER2				PKG_MISC+54
	debugtext(ERR_POOL_MARKER2,			"Memory following pool block has been destroyed")
#define ERR_POOL_BLOCK_DUMP				PKG_MISC+55
	debugtext(ERR_POOL_BLOCK_DUMP,		"%nPool block:   Length: %d     Offset: %04x%n")
#define ERR_DUMP_TRUNCATED				PKG_MISC+56
	debugtext(ERR_DUMP_TRUNCATED,		"        TRUNCATED from %d bytes to 1024%n")
#define ERR_DUMP_FORMAT4				PKG_MISC+57
	debugtext(ERR_DUMP_FORMAT4,			"%08lx %08lx %08lx %08lx:  %04x      '%s'%n")
#define ERR_DUMP_FORMAT2				PKG_MISC+58
	debugtext(ERR_DUMP_FORMAT2,			"%04x %04x %04x %04x %04x %04x %04x %04x:  %04x      '%s'%n")
#define ERR_POOLQ_ALREADY_THERE			PKG_MISC+59
	internaltext(ERR_POOLQ_ALREADY_THERE, "OSBlockQueueInsert: block already on queue")
#define ERR_POOLQ_BAD_CHAIN				PKG_MISC+60
	internaltext(ERR_POOLQ_BAD_CHAIN,	"OSBlockQueue: bad queue chain")
#define ERR_POOLQ_REMOVE_HEAD			PKG_MISC+61
	internaltext(ERR_POOLQ_REMOVE_HEAD,	"OSBlockQueueRemove: Attempted to remove queue head")
#define ERR_POOLREALLOCFIXED	  		PKG_MISC+62
	errortext(ERR_POOLREALLOCFIXED,		"Insufficient contiguous memory to reallocate in-place (P)")
#define	ERR_NO_BLOCK_SIZES				PKG_MISC+63
	internaltext(ERR_NO_BLOCK_SIZES,	"(Pool does not support block sizes)")
#define ERR_ATOM_TABLE_FULL				PKG_MISC+64
	internaltext(ERR_ATOM_TABLE_FULL,	"Atom table is full")
#define ERR_TOO_MANY_POOLS				PKG_MISC+65
	errortext(ERR_TOO_MANY_POOLS,		"The index is too big.  Change view to select fewer documents; or reduce number/size of columns in the view.")
#define ERR_DICT_FILEOPEN				PKG_MISC+66
	errortext(ERR_DICT_FILEOPEN,		"Error opening dictionary file")
#define ERR_POOL_NOT_EMPTY				PKG_MISC+67
	debugtext(ERR_POOL_NOT_EMPTY,		"Attempt to delete non-empty Local DPool")
#define ERR_LOCAL_POOL					PKG_MISC+68
	debugtext(ERR_LOCAL_POOL,			"Function not allowed for Local DPool")
#define ERR_ACL_DEFAULT_NAME			PKG_MISC+69
	stringtext(ERR_ACL_DEFAULT_NAME,	"-Default-")
#define ERR_CTF_INVALID					PKG_MISC+70
	errortext(ERR_CTF_INVALID,			"Invalid translation file. Check your User Preferences to make sure that file is properly specified.")
#define ERR_ARRAY_FULL					PKG_MISC+71
	internaltext(ERR_ARRAY_FULL,		"Array is full")
#define ERR_IDTABLE_INVALID				PKG_MISC+72
	errortext(ERR_IDTABLE_INVALID,		"Invalid ID table")
#define ERR_FILE_INVALID				PKG_MISC+73
	errortext(ERR_FILE_INVALID,			"Invalid file type")
#define ERR_DIRECTORY_INVALID			PKG_MISC+74
	errortext(ERR_DIRECTORY_INVALID,	"Directory does not exist")
#define ERR_NOT_YET_IMPLEMENTED			PKG_MISC+75
	errortext(ERR_NOT_YET_IMPLEMENTED,  "This feature is not yet implemented")
#define ERR_DUMP_ZEROS_SUPPRESSED		PKG_MISC+76
	errortext(ERR_DUMP_ZEROS_SUPPRESSED,"-- %lu (0x%04lx) bytes of 0 --")
#define ERR_DN_COMP_VERSION				PKG_MISC+77
	errortext(ERR_DN_COMP_VERSION,		"Invalid Name Parsing buffer size")

/* This resource identifies Notes V3.0c and later which is capable of */
/* 'setting up' a NLM server (remotely).  This ID should not be changed */
/* and is checked by GUIINST in the file directry.c. (by ID NOT name) */

#define NLMSETUP_IMPLEMENTED			PKG_MISC+78
	stringtext(NLMSETUP_IMPLEMENTED,	"1")

#define ERR_ENTRY_OR_VIEW_NOT_FOUND		PKG_MISC+79
	errortext(ERR_ENTRY_OR_VIEW_NOT_FOUND,"Entry not found in index or view's index not built")
#define ERR_LINEWRAP					PKG_MISC+80
	stringtext(ERR_LINEWRAP,			"Line wrap must be a numeric value greater then zero")
#define ERR_QUEUED_TO_BKGND				PKG_MISC+81
	stringtext(ERR_QUEUED_TO_BKGND,		"The requested operation will be performed in the background.")
#define ERR_STAT_NOT_FOUND				PKG_MISC+82
	errortext(ERR_STAT_NOT_FOUND,		" not found in server statistics table")
#define ERR_IDTABLE_CHECKSUM			PKG_MISC+83
	internaltext(ERR_IDTABLE_CHECKSUM,	"ID table checksum mismatch")
#define ERR_STAT_POOLFULL				PKG_MISC+84
	errortext(ERR_STAT_POOLFULL,		"Insufficient memory - Statistics pool is full.")
#define ERR_DPOOLFULL					PKG_MISC+85
	errortext(ERR_DPOOLFULL, 			"Insufficient memory - a Notes memory pool is full (DP)")
#define ERR_VPOOLFULL					PKG_MISC+86
	errortext(ERR_VPOOLFULL,		  	"Insufficient memory - a Notes memory pool is full (VP)")
#define ERR_LDPOOLFULL					PKG_MISC+87
	errortext(ERR_LDPOOLFULL,	  		"Insufficient memory - a Notes memory pool is full (LDP)")
#define ERR_FOLDER_OLDSERVER			PKG_MISC+88
	errortext(ERR_FOLDER_OLDSERVER,		"Sorry, this database is on a server running a previous version of Lotus Notes that does not support folders.")
#define ERR_SPARE_FULL					PKG_MISC+89
	errortext(ERR_SPARE_FULL,			"Unable to extend an ID table - insufficient memory.")
#define ERR_CORRUPT_VPOOL				PKG_MISC+90
	errortext(ERR_CORRUPT_VPOOL,		"Invalid Signature - VPOOL structure corrupt")
#define	ERR_ABSTRACT_INVALID_SIZE  	  	PKG_MISC+91
	errortext(ERR_ABSTRACT_INVALID_SIZE,"Abstract: the abstract size must be greater than 0")
#define ERR_OPCODE						PKG_MISC+92
	errortext(ERR_OPCODE,				"Invalid ID table - unexpected opcode")
#define ERR_ASSIGN						PKG_MISC+93
	errortext(ERR_ASSIGN,				"Invalid ID table - expected Assign opcode")
#define	ERR_MQ_POOLFULL  	  			PKG_MISC+94
	errortext(ERR_MQ_POOLFULL,			"Insufficient memory - Message Queue pool is full.")
#define	ERR_MQ_TIMEOUT  	  			PKG_MISC+95
	errortext(ERR_MQ_TIMEOUT,			"Timeout occurred waiting for message.")
#define	ERR_MQSCAN_ABORT  	  			PKG_MISC+96
	apitext(ERR_MQSCAN_ABORT,			"Message Queue scan was aborted.")
#define	ERR_DUPLICATE_MQ				PKG_MISC+97
	errortext(ERR_DUPLICATE_MQ,			"Message Queue name already in use.")
#define	ERR_NO_SUCH_MQ					PKG_MISC+98
	errortext(ERR_NO_SUCH_MQ, 			"No Message Queue with that name.")
#define	ERR_MQ_EXCEEDED_QUOTA			PKG_MISC+99
	errortext(ERR_MQ_EXCEEDED_QUOTA,	"Message Queue is full.")
#define	ERR_MQ_EMPTY					PKG_MISC+100
	errortext(ERR_MQ_EMPTY,				"Message Queue is empty.")
#define	ERR_MQ_BFR_TOO_SMALL			PKG_MISC+101
	errortext(ERR_MQ_BFR_TOO_SMALL,		"Message is larger than the buffer provided.")
#define	ERR_MQ_QUITTING					PKG_MISC+102
	errortext(ERR_MQ_QUITTING,			"Quit is pending on the Message Queue")
#define ERR_STAT_NOT_SET				PKG_MISC+103
	errortext(ERR_STAT_NOT_SET,			" cannot be reset.")
#define ERR_STAT_SET					PKG_MISC+104
	errortext(ERR_STAT_SET,				" was set to zero.")
#define ERR_STAT_SET_COMPLETE			PKG_MISC+105
	errortext(ERR_STAT_SET_COMPLETE,	" was not reset, please use complete statistic name (Facility.Statname).")
#define	ERR_MQSCAN_DEQUEUE				PKG_MISC+106
	internaltext(ERR_MQSCAN_DEQUEUE,	"Dequeue current message in enumeration")
#define	ERR_MQSCAN_DELETE				PKG_MISC+107
	internaltext(ERR_MQSCAN_DELETE,		"Delete current message in enumeration")
#define	ERR_ACL_CREATED					PKG_MISC+108
	stringtext(ERR_ACL_CREATED,			"%A %A created list")
#define	ERR_ACL_UPDATED					PKG_MISC+109
	stringtext(ERR_ACL_UPDATED,			"%A %A updated %A")
#define	ERR_ACL_ADDED					PKG_MISC+110
	stringtext(ERR_ACL_ADDED,			"%A %A added %A")
#define	ERR_ACL_DELETED					PKG_MISC+111
	stringtext(ERR_ACL_DELETED,			"%A %A deleted %A")
#define	ERR_ACL_PRIVNAME_SET			PKG_MISC+112
	stringtext(ERR_ACL_PRIVNAME_SET,	"%A %A updated role %s")
#define	ERR_ACL_PRIVNAME_CLEARED		PKG_MISC+113
	stringtext(ERR_ACL_PRIVNAME_CLEARED,"%A %A deleted role %s")
#define STR_ACL_WHOIS_INLIST			PKG_MISC+114
	stringtext(STR_ACL_WHOIS_INLIST,	"%A is already in the list.")

#define	ERR_IDTABLE_TOOMANY				PKG_MISC+115
	debugtext(ERR_IDTABLE_TOOMANY,		"Too many IDs counted vs. header")
#define	ERR_IDTABLE_TOOLITTLE			PKG_MISC+116
	debugtext(ERR_IDTABLE_TOOLITTLE,	"Too few IDs counted vs. header")
#define	ERR_IDTABLE_REPEAT				PKG_MISC+117
	debugtext(ERR_IDTABLE_REPEAT,		"Same ID encountered in enumeration")
#define	ERR_IDTABLE_BACKWARDS	   		PKG_MISC+118
	debugtext(ERR_IDTABLE_BACKWARDS,	"ID value encountered less than previous ID")
#define	ERR_IDTABLE_MISSING_ID	   		PKG_MISC+119
	debugtext(ERR_IDTABLE_MISSING_ID,	"ID missing from encountered table")

#define	ERR_FOLD_NAME_DUPLICATE			PKG_MISC+120
	errortext(ERR_FOLD_NAME_DUPLICATE,	"A folder or view with this name already exists.")
#define	ERR_MIX_PRIVATE_SHARED			PKG_MISC+121
	errortext(ERR_MIX_PRIVATE_SHARED,	"You may not mix shared folders or views with private folders or views.")
#define ERR_FOLD_NAME_LENGTH 			PKG_MISC+122
	errortext(ERR_FOLD_NAME_LENGTH,		"The name specified is too long or contains cascading items that are too long.")
#define ERR_DUP_PRIV_NAME 				PKG_MISC+123
	errortext(ERR_DUP_PRIV_NAME,		"Duplicate privilege name found.")
#define STR_DAYS   						PKG_MISC+124
	stringtext(STR_DAYS, 				"Sunday,Monday,Tuesday,Wednesday,Thursday,Friday,Saturday")
#define STR_MONTHS   				   	PKG_MISC+125
	stringtext(STR_MONTHS, 				"January,February,March,April,May,June,July,August,September,October,November,December")
#define ERR_TEXT_LIST_TOO_BIG			PKG_MISC+126
	errortext(ERR_TEXT_LIST_TOO_BIG,	"Adding entry will cause text list to exceed 64K. Entry not added.")
#define STR_ECL_NO_SIG_NAME			   	PKG_MISC+127
	stringtext(STR_ECL_NO_SIG_NAME,		"-No Signature-")
#define STR_DAYSABBREV					PKG_MISC+128
	stringtext(STR_DAYSABBREV,			"Su,Mo,Tu,We,Th,Fr,Sa")

#define	ERR_TOO_MUCH_RING_DATA			PKG_MISC+129
	errortext(ERR_TOO_MUCH_RING_DATA,	"Ring buffer overflow")
#define ERR_TIMEGAP_TOOBIG				PKG_MISC+130
	internaltext(ERR_TIMEGAP_TOOBIG,	"Time range to find will never be found.")

#define ERR_MAIL_FOROTHERUSER			PKG_MISC+131
	errortext(ERR_MAIL_FOROTHERUSER,	"Did not deliver mail messages created by another user.  Check MAIL.BOX for further information.")
#define ERR_MISC_HDL_NOT_DISASSOCIATED	(PKG_MISC+132)
	errortext(ERR_MISC_HDL_NOT_DISASSOCIATED, "Operation requires handle without shared reference count")
#define ERR_MISC_DATAPIPE_UNEXPECTED	(PKG_MISC+133)
	internaltext(ERR_MISC_DATA_PIPE_UNEXPECTED, "Unexpected error in use of CDataPipe facility")
#define ERR_LZ1_DECOMPRESS_BAD_DATA		(PKG_MISC+134)
	errortext(ERR_LZ1_DECOMPRESS_BAD_DATA,	"Corrupted LZ1 Encoded Data.")
#define ERR_DATEISINPAST				(PKG_MISC+135)
	errortext(ERR_DATEISINPAST,			"The date and time specified is in the past.")
#define	ERR_MQ_MSGSPENDING 	  			(PKG_MISC+136)
	errortext(ERR_MQ_MSGSPENDING,		"Message Queue has messages outstanding.")
#define ERR_STR_CONVERT					(PKG_MISC+137)
	errortext(ERR_STR_CONVERT,			"String conversion failed.")
#define ERR_MISC_BAD_UUENCODED_DATA		(PKG_MISC+138)
	errortext(ERR_MISC_BAD_UUENCODED_DATA,	"Corrupt UUEncoded Data")
#define ERR_IMAP_BAD_MESSAGESET			(PKG_MISC+139)
	limitedasciitext(ERR_IMAP_BAD_MESSAGESET,	"Syntax error in IMAP message set")
#define ERR_LDAP_POOLFULL				(PKG_MISC+140)
	errortext(ERR_LDAP_POOLFULL, 		"LDAP BPool Full")
#define ERR_CERT_CONVERT_FAILED			(PKG_MISC+141)
	errortext(ERR_CERT_CONVERT_FAILED,	"Certificate conversion failed")

/*	This can be translated, but the translation must be strictly US-ASCII chars.
	It is used in cases where a resourced string which must be sent to an
	IMAP (or other)	client in US-ASCII contains non-ascii characters.  In
	that case we construct a string like "Code 03:21" where the numbers are
	a hex value of the resource code.
*/
#define ERR_MISC_NOTASCII_CODE_PREFIX	(PKG_MISC+142)
	limitedasciitext(ERR_MISC_NOTASCII_CODE_PREFIX, "Code ")

#define ERR_ILLEGAL_BLOCK_SIZE			(PKG_MISC+143)
	errortext(ERR_ILLEGAL_BLOCK_SIZE,	"Block size illegal for BPOOL.")
#define ERR_CORRUPT_BPOOL				(PKG_MISC+144)
	errortext(ERR_CORRUPT_BPOOL,		"Invalid Signature - BPOOL structure corrupt")
#define ERR_BPOOLFULL					(PKG_MISC+145)
	errortext(ERR_BPOOLFULL,		  	"Insufficient memory - a Notes memory pool is full (BP)")

#define ERR_LZ1FAILED					(PKG_MISC+146)
	errortext(ERR_LZ1FAILED,			"Could not compress buffer.")
#define ERR_LZ1NOBUFFER					(PKG_MISC+147)
	errortext(ERR_LZ1NOBUFFER,			"Invalid or NULL handle passed to compression function.")
#define ERR_ALREADY_EXIST				(PKG_MISC+148)
	internaltext(ERR_ALREADY_EXIST,		"An entry already exists.")
#define ERR_MISC_INVALID_ARGS			(PKG_MISC+149)
	errortext(ERR_MISC_INVALID_ARGS,	"Invalid arguments")
#define ERR_WRITE_ONLY					(PKG_MISC+150)
	internaltext(ERR_WRITE_ONLY,		"Cannot Prepend or Read from a write only stream.")
#define DBG_MISC_INVALID_ARGS			(PKG_MISC+151)
	debugtext(DBG_MISC_INVALID_ARGS,	"[%12s,%4d] Invalid input arguments: %e")
#define DBG_MISC_UNEXPECTED_ERROR		(PKG_MISC+152)
	debugtext(DBG_MISC_UNEXPECTED_ERROR,"[%12s,%4d] Unexpected internal error: %e")
#define DBG_MISC_FUNCTION_STARTED		(PKG_MISC+153)
	debugtext(DBG_MISC_FUNCTION_STARTED,"[%12s,%4d] %s started...")
#define DBG_MISC_FUNCTION_FINISH		(PKG_MISC+154)
	debugtext(DBG_MISC_FUNCTION_FINISH,	"[%12s,%4d] %s finished: %e")
#define ERR_MISC_RETURN_TRUNC			(PKG_MISC+155)
	internaltext(ERR_MISC_RETURN_TRUNC,	"Return value was truncated")
#define ERR_NEWVIEWTYPE_OLDSERVER		(PKG_MISC+156)
	errortext(ERR_NEWVIEWTYPE_OLDSERVER,"Sorry, this database is on a server running a previous version of Lotus Notes that does not support this type of view or folder.")
#define ERR_NEWVIEWTYPE_OLDODS			(PKG_MISC+157)
	errortext(ERR_NEWVIEWTYPE_OLDODS,	"Sorry, this database version does not support this type of view or folder.")
#define ERR_MISC_EOS					(PKG_MISC+158)
	errortext(ERR_MISC_EOS,				"End of stream")
#define ERR_ONLY_ONE_CATEGORY			(PKG_MISC+159)
	errortext(ERR_ONLY_ONE_CATEGORY,	"Category must be a single string.")
#define ERR_MISC_MIMEPART_NOT_FOUND			(PKG_MISC+160)
	errortext(ERR_MISC_MIMEPART_NOT_FOUND,	"MIME part not found")
#define ERR_LINKINFO_NOT_FOUND			(PKG_MISC+161)
	errortext(ERR_LINKINFO_NOT_FOUND,	"Link Info not found")
#define ERR_MISC_CHARSET_UNCHANGED		(PKG_MISC+162)
	internaltext(ERR_MISC_CHARSET_UNCHANGED,"Charset info not changed")
#define ERR_NAMED_FRAME_NOT_FOUND 		(PKG_MISC+163)
	errortext(ERR_NAMED_FRAME_NOT_FOUND,	"No frame of specified name exists.")
#define ERR_FRAMESET_UNKNOWN			(PKG_MISC+164)
	errortext(ERR_FRAMESET_UNKNOWN, "Nonexistent frameset specified.")

#define ERR_MONTH3_LIST		(PKG_MISC+165)
	stringtext(ERR_MONTH3_LIST,	"Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec")
#define ERR_MONTH_FULL_LIST	(PKG_MISC+166)
	stringtext(ERR_MONTH_FULL_LIST,	"January|February|March|April|May|June|July|August|September|October|November|December")

/* Index (1-relative) of ERR_EDIT_MONTH* above */
#define IDX_MONTH_JAN	1
#define IDX_MONTH_FEB	2
#define IDX_MONTH_MAR	3
#define IDX_MONTH_APR	4
#define IDX_MONTH_MAY	5
#define IDX_MONTH_JUN	6
#define IDX_MONTH_JUL	7
#define IDX_MONTH_AUG	8
#define IDX_MONTH_SEP	9
#define IDX_MONTH_OCT	10
#define IDX_MONTH_NOV	11
#define IDX_MONTH_DEC	12

#define ERR_DOW3_LIST			(PKG_MISC+167)
	stringtext(ERR_DOW3_LIST,	"Sun|Mon|Tue|Wed|Thu|Fri|Sat")
#define ERR_DOW_FULL_LIST		(PKG_MISC+168)
	stringtext(ERR_DOW_FULL_LIST,	"Sunday|Monday|Tuesday|Wednesday|Thursday|Friday|Saturday")
#define ERR_DOW3P_LIST			(PKG_MISC+169)
	stringtext(ERR_DOW3P_LIST,	"(Sun)|(Mon)|(Tue)|(Wed)|(Thu)|(Fri)|(Sat)")
#define ERR_DOW_FULLP_LIST		(PKG_MISC+170)
	stringtext(ERR_DOW_FULLP_LIST,	"(Sunday)|(Monday)|(Tuesday)|(Wednesday)|(Thursday)|(Friday)|(Saturday)")

/* Index (1-relative) of substrings within MSG_EDIT_DOW* above */
#define	IDX_DOW_SUN		1
#define	IDX_DOW_MON		2
#define	IDX_DOW_TUE		3
#define	IDX_DOW_WED		4
#define	IDX_DOW_THU		5
#define	IDX_DOW_FRI		6
#define	IDX_DOW_SAT		7

#define ERR_MISC_BUFFER_SIZE			(PKG_MISC+171)
	errortext(ERR_MISC_BUFFER_SIZE,		"Buffer size error")
#define ERR_MISC_UNEXPECTED_ERROR		(PKG_MISC+172)
	errortext(ERR_MISC_UNEXPECTED_ERROR,"Unexpected internal error")
#define DBG_MISC_TRACE_MSG				(PKG_MISC+173)
	debugtext(DBG_MISC_TRACE_MSG,		"[%12s,%4d] %s: %e")
#define DBG_MISC_LOG_MSG				(PKG_MISC+174)
	debugtext(DBG_MISC_LOG_MSG,			"%s> [%12s,%4d] %s")
#define ERR_MISC_TSRECORDS_POOL_FULL		(PKG_MISC+175)
	errortext(ERR_MISC_TSRECORDS_POOL_FULL,	"Insufficient Memory -- Tasks/Stats records pool is full")
#define STR_ALPHABET_FOR_LIST  			(PKG_MISC+176)
	stringtext(STR_ALPHABET_FOR_LIST,   "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz")
#define ERR_VIEW_NOFORMATITEM			(PKG_MISC+177)
	errortext(ERR_VIEW_NOFORMATITEM,	"Insufficient information to update view or folder design")
#define STR_ABBREVIATED_SUBZONE				(PKG_MISC+178)
	stringtext(STR_ABBREVIATED_SUBZONE,		"GMT%s%02d:%02d")
#define ERR_MAIL_FOROTHERUSER_SMTP			PKG_MISC+179
	errortext(ERR_MAIL_FOROTHERUSER_SMTP,	"Did not deliver mail messages created by another user.  Check SMTP.BOX for further information.")
#define ERR_MISC_4DIGIT_YEAR	(PKG_MISC+180)
	errortext(ERR_MISC_4DIGIT_YEAR, "Four digit year is required for this field.")
#define ERR_MIME_PART_TRUNCATED				(PKG_MISC+181)
	errortext(ERR_MIME_PART_TRUNCATED, 		"MIME part truncated.")
#define STR_RFC822_MESSAGE_BANNER			(PKG_MISC+182)
	stringtext(STR_RFC822_MESSAGE_BANNER,	"----- Message from %s on %s -----")
#define STR_RFC822_HEADER_LIST				(PKG_MISC+183)
	stringtext(STR_RFC822_HEADER_LIST,		"Unknown|Unknown|To|cc|Subject")

/* Index (1-relative) of substrings within STR_RFC822_HEADER_LIST above */
#define IDX_RFC822_HEADER_UNKNOWN_NAME	1
#define IDX_RFC822_HEADER_UNKNOWN_DATE	2
#define IDX_RFC822_HEADER_TO			3
#define IDX_RFC822_HEADER_CC			4
#define IDX_RFC822_HEADER_SUBJECT		5

#define ERR_MISC_MIME_TOO_MANY_PARTS		(PKG_MISC+184)
	errortext(ERR_MISC_MIME_TOO_MANY_PARTS,	"MIME stream has too many parts")
#define ERR_MISC_CMEMSTR_TOO_BIG			(PKG_MISC+185)
	errortext(ERR_MISC_CMEMSTR_TOO_BIG,		"CMemStr object cannot exceed 64K")
#define ERR_LTPA_NOT_INITED					(PKG_MISC+186)
	errortext(ERR_LTPA_NOT_INITED,			"LtpaTokenFactory: class not initialized")
#define ERR_LTPA_INTERNAL_FAILURE			(PKG_MISC+187)
	errortext(ERR_LTPA_INTERNAL_FAILURE,	"Unexpected Single Sign-On error")
#define ERR_LTPA_TOKEN_INVALID				(PKG_MISC+188)
	errortext(ERR_LTPA_TOKEN_INVALID,		"Single Sign-On token is invalid")
#define ERR_LTPA_TOKEN_EXPIRED				(PKG_MISC+189)
	errortext(ERR_LTPA_TOKEN_EXPIRED,		"Single Sign-On token is expired")
#define ERR_LTPA_WSREALM_INVALID			(PKG_MISC+190)
	errortext(ERR_LTPA_WSREALM_INVALID,		"Single Sign-On token contains an unknown WebSphere LDAP realm")
#define ERR_LTPA_NOT_CONFIGURED				(PKG_MISC+191)
	errortext(ERR_LTPA_NOT_CONFIGURED,		"Single Sign-On configuration is invalid")

/*	PKG_MISC only goes to 191 !!! */

/* PKG_MISC2 only goes to 31*/
#define STR_ALTCAL_HIJRI_MONTH_LIST		(PKG_MISC2+1)
	stringtext(STR_ALTCAL_HIJRI_MONTH_LIST,		"Muharram|Safar|Rabia1|Rabia2|Jumada1|Jumada2")
#define STR_ALTCAL_HIJRI_MONTH_LIST2		(PKG_MISC2+2)
	stringtext(STR_ALTCAL_HIJRI_MONTH_LIST2,	"Rajab|Shaaban|Ramadan|Shawwal|Dhu'l-qa'da|Dhu'l-hijja")
#define STR_ALTCAL_JEWISH_MONTH_LIST		(PKG_MISC2+3)
	stringtext(STR_ALTCAL_JEWISH_MONTH_LIST,	"Nisan|Iyyar|Sivan|Tammuz|Av|Elul")
#define STR_ALTCAL_JEWISH_MONTH_LIST2		(PKG_MISC2+4)
	stringtext(STR_ALTCAL_JEWISH_MONTH_LIST2,	"Tishri|Heshvan|Kislev|Teveth|Shevat|Adar|Adar2")
#define STR_ALTCAL_SIXDAY_LIST			(PKG_MISC2+5)
	stringtext(STR_ALTCAL_SIXDAY_LIST,	"Sensho|Tomobiki|Senbu|Butsumetsu|Taian|Shakko")
#define ERR_IOR_ALREADY_EXIST				(PKG_MISC2+6)
	errortext(ERR_IOR_ALREADY_EXIST,		"IOR is already registered.")
#define ERR_IORTBL_FULL						(PKG_MISC2+7)
	errortext(ERR_IORTBL_FULL,				"IOR Table is full.")
#define ERR_IOR_NOT_FOUND					(PKG_MISC2+8)
	errortext(ERR_IOR_NOT_FOUND,			"Debugger task not found on server.")
#define ERR_IOR_BUFFER_TOO_SMALL			(PKG_MISC2+9)
	errortext(ERR_IOR_BUFFER_TOO_SMALL,		"buffer to receive the requested IOR is too small.")
#define ERR_STAT_COMPLETED					(PKG_MISC2+10)
	stringtext(ERR_STAT_COMPLETED,			"  %d statistics found%n%n")
#define ERR_HOTFIX_FIXPACK_NUMBERS			(PKG_MISC2+11)
	stringtext(ERR_HOTFIX_FIXPACK_NUMBERS,	"0|0")
#define STR_CURRENT_TIME_ZONE	 			(PKG_MISC2+12)
	stringtext(STR_CURRENT_TIME_ZONE,		"Local time")

#define ERR_SIGNING_HOTSPOTS				(PKG_MISC2+13)
	errortext(ERR_SIGNING_HOTSPOTS,			"Admin Process: Error signing hotspots in %p - (NoteID 0x%x)")

#define ERR_ECL_NOACCESS					(PKG_MISC2+14)
	errortext(ERR_ECL_NOACCESS,				"You have insufficient access to modify your ECL.")
#define ERR_ECL_NOTINCACHE					(PKG_MISC2+15)
	errortext(ERR_ECL_NOTINCACHE,			"ECL not found in cache.")

#define STR_CAL_TWN_YEAR_FULL				(PKG_MISC2+16)
	stringtext(STR_CAL_TWN_YEAR_FULL, "Zhonghua Minguo")
#define STR_CAL_JPN_YEAR_FULL_LIST			(PKG_MISC2+17)
	stringtext(STR_CAL_JPN_YEAR_FULL_LIST, "Meiji|Taisho|Showa|Heisei")
#define STR_CAL_JPN_YEAR3_LIST				(PKG_MISC2+18)
	stringtext(STR_CAL_JPN_YEAR3_LIST, "Mei|Tai|Sho|Hei")
#define STR_CAL_JPN_YEAR1_LIST				(PKG_MISC2+19)
	stringtext(STR_CAL_JPN_YEAR1_LIST, "M|T|S|H")
/* Indices for the above */
#define IDX_JPN_MEIJI		1
#define IDX_JPN_TAISHO		2
#define IDX_JPN_SHOWA		3
#define	IDX_JPN_HEISEI		4
#define IDX_JPN_SIZE		4

#define STR_CAL_TWN_YEAR_SHORT				(PKG_MISC2+20)
	stringtext(STR_CAL_TWN_YEAR_SHORT, "Minguo")
#define IDX_TWN_ZHONGHUA	1
#define IDX_TWN_SIZE		1

#define ERR_TDI_CONV_ALPHA_MONTH			(PKG_MISC2+21)
	errortext(ERR_TDI_CONV_ALPHA_MONTH, "Alphabetic month required. ")

#define ERR_HIDDEN_TASK						(PKG_MISC2+22)
	errortext(ERR_HIDDEN_TASK, 				"Hidden Task")

#define ERR_TOO_MANY_ROLES					(PKG_MISC2+23)
	errortext(ERR_TOO_MANY_ROLES, 			"Cannot add role, maximum number of entries has been reached.")

#define ERR_CANT_SET_BREAKPOINT					(PKG_MISC2+24)
	errortext(ERR_CANT_SET_BREAKPOINT, 			"Can't set breakpoint, program busy or ended.")

#define ERR_ACL_COPY							(PKG_MISC2+25)
	errortext(ERR_ACL_COPY, 					"%A %A copied acl from %A")

#define ERR_MUI_DIR_NOTFOUND				(PKG_MISC2+26)
	errortext(ERR_MUI_DIR_NOTFOUND,			"Invalid Language specified for the User interface; proceeding with the default installed language.")

#define	ERR_ACL_XACLENABLED						PKG_MISC2+27
	stringtext(ERR_ACL_XACLENABLED,				"%A %A enabled extended access in %A")
#define	ERR_ACL_XACLDISABLED					PKG_MISC2+28
	stringtext(ERR_ACL_XACLDISABLED,			"%A %A disabled extended access in %A")
#define  ERR_MC_EXCEPTION              (PKG_MISC2+29)
   errortext(ERR_MC_EXCEPTION,            "MIME-CD Fatal error: note %08x, db %s, process %s (%04x:%04x)")
#define  ERR_CM_EXCEPTION              (PKG_MISC2+30)
   errortext(ERR_CM_EXCEPTION,            "CD-MIME Fatal error: note %08x, db %s, process %s (%04x:%04x)")
#define ERR_CNV_EXCEPTION_NO_INFO      (PKG_MISC2+31)
   errortext(ERR_CNV_EXCEPTION_NO_INFO,   "MIME/CD converter exception -- no information available")   


/*	PKG_MISC2 only goes to 31 !!! */

/*  PKG_MISC3 goes to 31 !!! */
#define ERR_INTL_INVALID_PROPERTY					(PKG_MISC3+1)
	errortext(ERR_INTL_INVALID_PROPERTY,			"Invalid Property Specified")
#define ERR_INTL_INVALID_PROPERTY_VALUE				(PKG_MISC3+2)
	errortext(ERR_INTL_INVALID_PROPERTY_VALUE,		"Invalid Property Value for this Property")
#define ERR_INTL_INVALID_PROPERTY_VALUE_LEN			(PKG_MISC3+3)
	errortext(ERR_INTL_INVALID_PROPERTY_VALUE_LEN,	"Invalied Buffer Length to Property Value")
#define ERR_AUTH_NAME_LOOKUP_EMAIL					(PKG_MISC3+4)
	errortext(ERR_AUTH_NAME_LOOKUP_EMAIL,			"Name ambiguity found during check of Internet email address.")
#define ERR_AUTH_NAME_AMBIGUOUS_MATCH				(PKG_MISC3+5)
	errortext(ERR_AUTH_NAME_AMBIGUOUS_MATCH,		"Multiple Notes name matches found in directory")
#define	ERR_MISMATCH_VERSION							(PKG_MISC3+6)
	errortext(ERR_MISMATCH_VERSION,						"There are mismatched versions of program files running. Please correct this to avoid possible problems.")


/* PKG_MISC_STR goes to 255 */
/* Note:  If you change these STR_MISC_ECL_ strings, also change desk\ecledit.dlg DESK_ECL_xxx */
#define STR_MISC_ECL_FILES						(PKG_MISC_STR+1)
	stringtext(STR_MISC_ECL_FILES,				"Access to file system")
#define STR_MISC_ECL_DOCS_DBS					(PKG_MISC_STR+2)
	stringtext(STR_MISC_ECL_DOCS_DBS,			"Access to current database")
#define STR_MISC_ECL_ENVIRON					(PKG_MISC_STR+3)
	stringtext(STR_MISC_ECL_ENVIRON,			"Access to environment variables")
#define STR_MISC_ECL_EXTERN_DBS					(PKG_MISC_STR+4)
	stringtext(STR_MISC_ECL_EXTERN_DBS,			"Access to non-Notes databases")
#define STR_MISC_ECL_EXTERN_CODE				(PKG_MISC_STR+5)
	stringtext(STR_MISC_ECL_EXTERN_CODE,		"Access to external code")
#define STR_MISC_ECL_EXTERN_PROGRAMS			(PKG_MISC_STR+6)
	stringtext(STR_MISC_ECL_EXTERN_PROGRAMS,	"Access to external programs")
#define STR_MISC_ECL_SEND_MAIL					(PKG_MISC_STR+7)
	stringtext(STR_MISC_ECL_SEND_MAIL,			"Ability to send mail")
#define STR_MISC_ECL_ECL						(PKG_MISC_STR+8)
	stringtext(STR_MISC_ECL_ECL,				"Access to ECL")
#define STR_MISC_ECL_READ_OTHER_DBS				(PKG_MISC_STR+9)
	stringtext(STR_MISC_ECL_READ_OTHER_DBS,		"Ability to read other databases")
#define STR_MISC_ECL_WRITE_OTHER_DBS			(PKG_MISC_STR+10)
	stringtext(STR_MISC_ECL_WRITE_OTHER_DBS,	"Ability to modify other databases")
#define STR_MISC_ECL_EXPORT_DATA	   			(PKG_MISC_STR+11)
	stringtext(STR_MISC_ECL_EXPORT_DATA,		"Ability to export data")
#define STR_MISC_ECL_NETWORK_ACCCESS 			(PKG_MISC_STR+12)
	stringtext(STR_MISC_ECL_NETWORK_ACCCESS,	"Access to Network")
#define STR_MISC_ECL_NOTES_CLASSES				(PKG_MISC_STR+13)
	stringtext(STR_MISC_ECL_NOTES_CLASSES,		"Access to Notes Java classes")
#define STR_MISC_ECL_NETWORK					(PKG_MISC_STR+14)
	stringtext(STR_MISC_ECL_NETWORK,			"Access to network addresses")
#define STR_MISC_ECL_PRINTING					(PKG_MISC_STR+15)
	stringtext(STR_MISC_ECL_PRINTING,			"Printing")
#define STR_MISC_ECL_PROPS						(PKG_MISC_STR+16)
	stringtext(STR_MISC_ECL_PROPS,				"Access to system properties")
#define STR_MISC_ECL_DIALOGS					(PKG_MISC_STR+17)
	stringtext(STR_MISC_ECL_DIALOGS,			"Dialog and clipboard access")
#define STR_MISC_ECL_PROCESS					(PKG_MISC_STR+18)
	stringtext(STR_MISC_ECL_PROCESS,			"Process-level access")
#define STR_MISC_ECL_JAVASCRIPT_READ_MY_WINDOW				(PKG_MISC_STR+19)
	stringtext(STR_MISC_ECL_JAVASCRIPT_READ_MY_WINDOW,		"Read access to same window")
#define STR_MISC_ECL_JAVASCRIPT_WRITE_MY_WINDOW				(PKG_MISC_STR+20)
	stringtext(STR_MISC_ECL_JAVASCRIPT_WRITE_MY_WINDOW,		"Write access to same window")
#define STR_MISC_ECL_JAVASCRIPT_READ_OTHER_WINDOW_SAME			(PKG_MISC_STR+21)
	stringtext(STR_MISC_ECL_JAVASCRIPT_READ_OTHER_WINDOW_SAME,	"Read access to different window from originating host")
#define STR_MISC_ECL_JAVASCRIPT_WRITE_OTHER_WINDOW_SAME			(PKG_MISC_STR+22)
	stringtext(STR_MISC_ECL_JAVASCRIPT_WRITE_OTHER_WINDOW_SAME,	"Write access to different window from originating host")
#define STR_MISC_ECL_JAVASCRIPT_OPEN_NEW_WINDOW_SAME			(PKG_MISC_STR+23)
	stringtext(STR_MISC_ECL_JAVASCRIPT_OPEN_NEW_WINDOW_SAME,   "Access to Open URL on originating host")
#define STR_MISC_ECL_JAVASCRIPT_READ_OTHER_WINDOW_DIFF			(PKG_MISC_STR+24)
	stringtext(STR_MISC_ECL_JAVASCRIPT_READ_OTHER_WINDOW_DIFF,	"Read access to different window from other than originating host")
#define STR_MISC_ECL_JAVASCRIPT_WRITE_OTHER_WINDOW_DIFF			(PKG_MISC_STR+25)
	stringtext(STR_MISC_ECL_JAVASCRIPT_WRITE_OTHER_WINDOW_DIFF,	"Write access to different window from other than originating host")
#define STR_MISC_ECL_JAVASCRIPT_OPEN_NEW_WINDOW_DIFF			(PKG_MISC_STR+26)
	stringtext(STR_MISC_ECL_JAVASCRIPT_OPEN_NEW_WINDOW_DIFF,   "Access to Open URL from other than originating host")
#define STR_MISC_ECL_STORED_FORM								(PKG_MISC_STR+27)
	stringtext(STR_MISC_ECL_STORED_FORM,						"Stored Form")
#define STR_MISC_ECL_STORED_SUBFORM								(PKG_MISC_STR+28)
	stringtext(STR_MISC_ECL_STORED_SUBFORM,						"Stored SubForm")
#define STR_MISC_ECL_STORED_FORM_OLD							(PKG_MISC_STR+29)
	stringtext(STR_MISC_ECL_STORED_FORM_OLD,					"Stored Form(Old Format)")
#define STR_MISC_DEFAULT_ADMIN_ECL								(PKG_MISC_STR+30)
	stringtext(STR_MISC_DEFAULT_ADMIN_ECL,						"Default Admin ECL")

#define STR_MISC_REFRESH_DESIGN_STRINGS				(PKG_MISC_STR+31)
	stringtext(STR_MISC_REFRESH_DESIGN_STRINGS,	"All Databases|Verifying...|Design refresh completed|Design refresh cancelled")
	#define IDX_STR_DESK_REFRESH_DESIGN_ALL_DBS			1
	#define IDX_STR_DESK_REFRESH_DESIGN_VERIFYING		2
	#define IDX_STR_DESK_REFRESH_DESIGN_COMPLETED		3
	#define IDX_STR_DESK_REFRESH_DESIGN_CANCELLED		4
#define STR_MISC_LEFTTOCOPY							(PKG_MISC_STR+32)
	stringtext(STR_MISC_LEFTTOCOPY,	"Copying %s - %s|please wait...|%,lu design elements left to copy|%,lu documents left to copy")
	#define IDX_STR_DESK_LEFTTOCOPY_COPY_DB			1
	#define IDX_STR_DESK_LEFTTOCOPY_PLEASE_WAIT		2
	#define IDX_STR_DESK_LEFTTOCOPY_DESIGNS			3
	#define IDX_STR_DESK_LEFTTOCOPY_DOCUMENTS		4


#define IDX_NUMBERS_HOTFIX		1
#define IDX_NUMBERS_FIXPACK		2

#define STR_MISC_REPL_CHAR							(PKG_MISC_STR+33)
	stringtext(STR_MISC_REPL_CHAR,		"Warning: notes.ini setting SMTPMTA_SPACE_REPL_CHAR is incompatible with Notes domains(s) separator specified in Global Domain document. Using '_' as space replacement character.")

#define	ERR_ACL_PASTED								(PKG_MISC_STR+34)
	stringtext(ERR_ACL_PASTED,						"%A %A pasted %A")
#define ACL_ALL_ENTRIES								(PKG_MISC_STR+35)
	stringtext(ACL_ALL_ENTRIES,						"entire ACL")

#define STR_MISC_REFRESH_DESIGN_STRINGS_2			(PKG_MISC_STR+36)
	stringtext(STR_MISC_REFRESH_DESIGN_STRINGS_2,	"Design refresh failed")
	#define IDX_STR_DESK_REFRESH_DESIGN_FAILED		1


/* For OS400 collection services task */
#define STR_COLSRV400								(PKG_MISC_STR+37)
	stringtext(STR_COLSRV400,		"Collection Services Initializing|Collection Services Idle|Collection Services Checking For Requests|Collection Services Processing Request")
	#define IDX_COLSRV400_INIT		1
	#define	IDX_COLSRV400_IDLE		2
	#define IDX_COLSRV400_CHECKING		3
	#define IDX_COLSRV400_PROCESSING	4

#define STR_COLSRV400_2								(PKG_MISC_STR+38)
	stringtext(STR_COLSRV400_2,		"Collection Services Shutdown|Collection Services is Already Running|Collection Services Encountered an Error|Collection Services Started")
	#define IDX_COLSRV400_SHUTDOWN		1
	#define IDX_COLSRV400_DUPLICATE		2
	#define IDX_COLSRV400_ERROR		3
	#define IDX_COLSRV400_STARTED		4

/* The following are used by HasFeature in misc/hasfeature.cpp */
/* THESE VALUES ARE USED SPECIFICALLY IN THE MAKEFILE FOR MAIN AND MUST NOT BE MOVED! */
/* I.e., NOTES_BUILD_ID_ENHANCED must STAY as PKG_MISC_STR+41  */
#define NOTES_FEATURES						PKG_MISC_STR+39
     stringtext(NOTES_FEATURES,			NOTES_FEATURES_STRING)
#define NOTES_FEATURES_ENHANCED					PKG_MISC_STR+40
     stringtext(NOTES_FEATURES_ENHANCED,		NOTES_FEATURES_STRING_ENHANCED)
#define NOTES_BUILD_ID_ENHANCED					PKG_MISC_STR+41
     stringtext(NOTES_BUILD_ID_ENHANCED,		NOTES_BUILD_ENHANCED)

#define	ADMIN_FAULT_REPORTS_TITLE			PKG_MISC_STR+42
	stringtext(ADMIN_FAULT_REPORTS_TITLE,		"Lotus Notes/Domino Fault Reports")
#define ADMIN_FAULT_REPORTS_DESC			PKG_MISC_STR+43
	stringtext(ADMIN_FAULT_REPORTS_DESC,		"Mail-in database for fault reports from Notes clients and Domino servers")

/* PKG_MISC_STR goes to 255 */

#endif
