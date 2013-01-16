
/*	Notes Database Driver Error Codes */

#ifndef DBD_ERR_DEFS
#define DBD_ERR_DEFS

/*	Define some things so that the DATALENS group can use this without our
	massive set of header files. */

#ifndef NOERROR
#define	NOERROR	0
#endif

#ifndef NOERROR
#define	PKG_DBD			0x3100		/* database driver error codes */
#endif

#ifndef errortext
#define errortext(code,text)
#endif

#if (PKG_DBD-0x3100)
	error!  PKG_DBD has been changed and drivers need to be recompiled!
#endif


/*	Database Driver Error Definitions */

#define ERR_DBD_INSUFF_ARGS					PKG_DBD+1
	errortext(ERR_DBD_INSUFF_ARGS, "Insufficient arguments for database function")
#define ERR_DBD_DATATYPE					PKG_DBD+2
	errortext(ERR_DBD_DATATYPE, "Incorrect datatype for database function")
#define ERR_DBD_CLASSDRV					PKG_DBD+3
	errortext(ERR_DBD_CLASSDRV, "The necessary external database driver cannot be located")
#define ERR_DBD_SESSIONS_ACTIVE				PKG_DBD+4
	errortext(ERR_DBD_SESSIONS_ACTIVE, "Database sessions still open - must be closed by the time Term is called")
#define ERR_DBD_RDATABASE					PKG_DBD+5
	errortext(ERR_DBD_RDATABASE, "Cannot open specified database, no match for specified Replica ID can be found")
#define ERR_DBD_FUNCTION					PKG_DBD+6
	errortext(ERR_DBD_FUNCTION, "The specified database driver does not support this function")
#define ERR_DBD_TOOMANY_ARGS				PKG_DBD+7
	errortext(ERR_DBD_TOOMANY_ARGS, "Too many arguments for database function")
#define ERR_DBD_INCOMPAT_TYPE				PKG_DBD+8
	errortext(ERR_DBD_INCOMPAT_TYPE, "The data types of two matching results are incompatible")
#define ERR_DBD_MOD_RETRY					PKG_DBD+9
	internaltext(ERR_DBD_MOD_RETRY, "The table was modified - retry after updating")
#define ERR_DBD_NO_VIEW						PKG_DBD+10
	errortext(ERR_DBD_NO_VIEW, "A view of that name cannot be found in the specified database")

#ifdef removed
removed because security restriction is gone for R6
#define ERR_DBD_NO_REMOTE	         		PKG_DBD+11
	errortext(ERR_DBD_NO_REMOTE, "For security reasons, remote Notes database lookups are not permitted")
#endif

#define ERR_DBD_64K							PKG_DBD+12
	errortext(ERR_DBD_64K, "The specified database lookup generated more than 65,000 bytes of results, which is too large for Notes to handle in this context.")

#define ERR_DBD_NOT_CAPABLE         		PKG_DBD+21
	errortext(ERR_DBD_NOT_CAPABLE, "<Database Driver>Driver does not have capability required")
#define ERR_DBD_NOT_ALLOWED         		PKG_DBD+22
	errortext(ERR_DBD_NOT_ALLOWED, "<Database Driver>Insufficient user privileges")
#define ERR_DBD_NOT_INITIALIZED     		PKG_DBD+23
	errortext(ERR_DBD_NOT_INITIALIZED, "<Database Driver>DataLens support system not initialized")
#define ERR_DBD_SYSTEM_FAILURE      		PKG_DBD+24
	errortext(ERR_DBD_SYSTEM_FAILURE, "<Database Driver>Complete driver/connection failure")
#define ERR_DBD_TRANSPORT_ERROR     		PKG_DBD+25
	errortext(ERR_DBD_TRANSPORT_ERROR, "<Database Driver>Unrecoverable network or IPC error")
#define ERR_DBD_TRANSPORT_FAILURE   		PKG_DBD+26
	errortext(ERR_DBD_TRANSPORT_FAILURE, "<Database Driver>Network connection not responding")
#define ERR_DBD_CHILD_HANDLE_OPEN   		PKG_DBD+27
	errortext(ERR_DBD_CHILD_HANDLE_OPEN, "<Database Driver>Tried to close parent with active child")
#define ERR_DBD_BAD_CONNECTION_HANDLE 		PKG_DBD+28
	errortext(ERR_DBD_BAD_CONNECTION_HANDLE, "<Database Driver>Bad connection handle")
#define ERR_DBD_BAD_BROWSER_HANDLE  		PKG_DBD+29
	errortext(ERR_DBD_BAD_BROWSER_HANDLE, "<Database Driver>Invalid browsing handle")

#define ERR_DBD_NO_HANDLE_MEMORY   			PKG_DBD+30
	errortext(ERR_DBD_NO_HANDLE_MEMORY, "<Database Driver>Gab couldn't allocate memory for handle")
#define ERR_DBD_BAD_NAME           			PKG_DBD+31
	errortext(ERR_DBD_BAD_NAME, "<Database Driver>Bad name for driver or database")
#define ERR_DBD_DRIVER_LOAD_ERROR  			PKG_DBD+32
	errortext(ERR_DBD_DRIVER_LOAD_ERROR, "<Database Driver>Driver located but could not be loaded")
#define ERR_DBD_NO_COLLECTION_NODE 			PKG_DBD+33
	errortext(ERR_DBD_NO_COLLECTION_NODE, "<Database Driver>Not a valid collection node name")
#define ERR_DBD_END_OF_DATA        			PKG_DBD+34
	errortext(ERR_DBD_END_OF_DATA, "<Database Driver>No more records or browse items")
#define ERR_DBD_UNPOSITIONED       			PKG_DBD+35
	errortext(ERR_DBD_UNPOSITIONED, "<Database Driver>Browser handle not positioned yet")
#define ERR_DBD_NAME_TRUNCATED     			PKG_DBD+36
	errortext(ERR_DBD_NAME_TRUNCATED, "<Database Driver>Returned name truncated to fit in buffer")
#define ERR_DBD_BAD_USER_NAME      			PKG_DBD+37
	errortext(ERR_DBD_BAD_USER_NAME, "<Database Driver>Bad user/application string")
#define ERR_DBD_BAD_PASSWORD       			PKG_DBD+38
	errortext(ERR_DBD_BAD_PASSWORD, "<Database Driver>Bad authorization string")
#define ERR_DBD_DRIVER_OUT_OF_MEMORY 		PKG_DBD+39
	errortext(ERR_DBD_DRIVER_OUT_OF_MEMORY, "<Database Driver>Memory allocation/deallocation error")

#define ERR_DBD_DRIVER_FAILURE     			PKG_DBD+40
	errortext(ERR_DBD_DRIVER_FAILURE, "<Database Driver>Driver-specific error")
#define ERR_DBD_NO_REGISTRATION    			PKG_DBD+41
	errortext(ERR_DBD_NO_REGISTRATION, "<Database Driver>Can't locate driver registration list")
#define ERR_DBD_BAD_BROWSE_DATA    			PKG_DBD+42
	errortext(ERR_DBD_BAD_BROWSE_DATA, "<Database Driver>Can't read browse item")
#define ERR_DBD_BROWSE_ACCESS_ERROR 		PKG_DBD+43
	errortext(ERR_DBD_BROWSE_ACCESS_ERROR, "<Database Driver>Can't read registration data")
#define ERR_DBD_INVALID_ARGUMENT   			PKG_DBD+44
	errortext(ERR_DBD_INVALID_ARGUMENT, "<Database Driver>Bad argument to driver")
#define ERR_DBD_DUPLICATE_HANDLE   			PKG_DBD+45
	errortext(ERR_DBD_DUPLICATE_HANDLE, "<Database Driver>Return handle to already open driver or database")
#define ERR_DBD_MULTIPLE_USERS     			PKG_DBD+46
	errortext(ERR_DBD_MULTIPLE_USERS, "<Database Driver>Warning: database opened with another userid")

#define ERR_DBD_READ_ONLY		 			PKG_DBD+52
	errortext(ERR_DBD_READ_ONLY, "<Database Driver>Attempt to alter read-only data")
#define ERR_DBD_NO_ROOM		 				PKG_DBD+53
	errortext(ERR_DBD_NO_ROOM, "<Database Driver>Database memory exhausted")
#define ERR_DBD_TRANSACTION_ABORT 			PKG_DBD+54
	errortext(ERR_DBD_TRANSACTION_ABORT, "<Database Driver>Current transaction aborted")
#define ERR_DBD_BAD_MEMORY_MANAGER 			PKG_DBD+55
	errortext(ERR_DBD_BAD_MEMORY_MANAGER, "<Database Driver>Driver uses less restrictive model than application")
#define ERR_DBD_BAD_DATABASE_NAME  			PKG_DBD+56
	errortext(ERR_DBD_BAD_DATABASE_NAME, "<Database Driver>Database could not be located")
#define ERR_DBD_OPEN_DATABASE 	 			PKG_DBD+57
	errortext(ERR_DBD_OPEN_DATABASE, "<Database Driver>Database already opened by current thread")
#define ERR_DBD_OPEN_CURSOR 	 			PKG_DBD+58
	errortext(ERR_DBD_OPEN_CURSOR, "<Database Driver>Cursor still open on close database request")
#define ERR_DBD_BAD_EXTENSION	 			PKG_DBD+59
	errortext(ERR_DBD_BAD_EXTENSION, "<Database Driver>Requested extension does not exist")

#define ERR_DBD_BAD_ACTION		 			PKG_DBD+60
	errortext(ERR_DBD_BAD_ACTION, "<Database Driver>Invalid action")
#define ERR_DBD_BAD_COLUMN		 			PKG_DBD+61
	errortext(ERR_DBD_BAD_COLUMN, "<Database Driver>Column does not exist")
#define ERR_DBD_BAD_NEW_NAME	 			PKG_DBD+62
	errortext(ERR_DBD_BAD_NEW_NAME, "<Database Driver>New name for table or column is invalid")
#define ERR_DBD_BAD_TABLE		 			PKG_DBD+63
	errortext(ERR_DBD_BAD_TABLE, "<Database Driver>Table or view does not exist")
#define ERR_DBD_TRUNCATED		 			PKG_DBD+64
	errortext(ERR_DBD_TRUNCATED, "<Database Driver>Input comment/label has been truncated")
#define ERR_DBD_BAD_INDEX		 			PKG_DBD+65
	errortext(ERR_DBD_BAD_INDEX, "<Database Driver>Invalid index specification")
#define ERR_DBD_BAD_COUNT		 			PKG_DBD+66
	errortext(ERR_DBD_BAD_COUNT, "<Database Driver>Invalid column, etc. count")
#define ERR_DBD_BAD_QUERY		 			PKG_DBD+67
	errortext(ERR_DBD_BAD_QUERY, "<Database Driver>Invalid query specification")
#define ERR_DBD_BAD_SYNONYM 	 			PKG_DBD+68
	errortext(ERR_DBD_BAD_SYNONYM, "<Database Driver>Invalid synonym specification")
#define ERR_DBD_BAD_PRIVILEGE	 			PKG_DBD+69
	errortext(ERR_DBD_BAD_PRIVILEGE, "<Database Driver>Invalid privilege specification")

#define ERR_DBD_BAD_USERID		 			PKG_DBD+70
	errortext(ERR_DBD_BAD_USERID, "<Database Driver>Invalid User ID specification")
#define ERR_DBD_BAD_WHERE		 			PKG_DBD+71
	errortext(ERR_DBD_BAD_WHERE, "<Database Driver>Invalid wheretree specification")
#define ERR_DBD_TOO_MANY_CURSORS  			PKG_DBD+72
	errortext(ERR_DBD_TOO_MANY_CURSORS, "<Database Driver>Cursor storage is full")
#define ERR_DBD_BAD_CURSOR		 			PKG_DBD+73
	errortext(ERR_DBD_BAD_CURSOR, "<Database Driver>Invalid cursor handle")
#define ERR_DBD_NOT_OPEN		 			PKG_DBD+74
	errortext(ERR_DBD_NOT_OPEN, "<Database Driver>Attempt to access unopened item")
#define ERR_DBD_BAD_STATEMENT	 			PKG_DBD+75
	errortext(ERR_DBD_BAD_STATEMENT, "<Database Driver>Invalid statement handle")
#define ERR_DBD_BAD_XFERBLOCK	 			PKG_DBD+76
	errortext(ERR_DBD_BAD_XFERBLOCK, "<Database Driver>Invalid xferblock format")
#define ERR_DBD_NOT_CLOSED		 			PKG_DBD+77
	errortext(ERR_DBD_NOT_CLOSED, "<Database Driver>Cursor is open")
#define ERR_DBD_TOO_FEW_PARAMETERS 			PKG_DBD+78
	errortext(ERR_DBD_TOO_FEW_PARAMETERS, "<Database Driver>Too few values supplied to where-parms")
#define ERR_DBD_TOO_MANY_PARAMETERS 		PKG_DBD+79
	errortext(ERR_DBD_TOO_MANY_PARAMETERS, "<Database Driver>Too many values supplied to where-parms")

#define ERR_DBD_BAD_ROW		     			PKG_DBD+80
	errortext(ERR_DBD_BAD_ROW, "<Database Driver>Tried to move cursor past existing rows")
#define ERR_DBD_NO_DATA_YET 	 			PKG_DBD+81
	errortext(ERR_DBD_NO_DATA_YET, "<Database Driver>First record has not arrived yet")
#define ERR_DBD_NO_LONG_DATA	 			PKG_DBD+82
	errortext(ERR_DBD_NO_LONG_DATA, "<Database Driver>Referenced column does not contain long data")
#define ERR_DBD_TOO_MANY_LONGS	 			PKG_DBD+83
	errortext(ERR_DBD_TOO_MANY_LONGS, "<Database Driver>Driver limit on open long data columns exceeded")
#define ERR_DBD_BAD_LENGTH		 			PKG_DBD+84
	errortext(ERR_DBD_BAD_LENGTH, "<Database Driver>Invalid buffer length")
#define ERR_DBD_BAD_POSITION	 			PKG_DBD+85
	errortext(ERR_DBD_BAD_POSITION, "<Database Driver>Invalid position specified in long data column")
#define ERR_DBD_BAD_VIEW_CHECK	 			PKG_DBD+86
	errortext(ERR_DBD_BAD_VIEW_CHECK, "<Database Driver>An inserted record failed a view check")
#define ERR_DBD_NO_NULLS		 			PKG_DBD+87
	errortext(ERR_DBD_NO_NULLS, "<Database Driver>Setting field null not permitted")
#define ERR_DBD_NOT_UNIQUE		 			PKG_DBD+88
	errortext(ERR_DBD_NOT_UNIQUE, "<Database Driver>Can't add duplicate val to unique index")
#define ERR_DBD_BAD_DATA		 			PKG_DBD+89
	errortext(ERR_DBD_BAD_DATA, "<Database Driver>Data type mismatch on output")

#define ERR_DBD_BAD_STATEMENT_NAME 			PKG_DBD+90
	errortext(ERR_DBD_BAD_STATEMENT_NAME, "<Database Driver>Proposed statement name is invalid")
#define ERR_DBD_ALREADY_OPEN	 			PKG_DBD+91
	errortext(ERR_DBD_ALREADY_OPEN, "<Database Driver>Attempt to open a table already open")
#define ERR_DBD_ALREADY_ISOLATED  			PKG_DBD+92
	errortext(ERR_DBD_ALREADY_ISOLATED, "<Database Driver>Isolation level cannot be altered")
#define ERR_DBD_BAD_ISOLATION	 			PKG_DBD+93
	errortext(ERR_DBD_BAD_ISOLATION, "<Database Driver>Invalid isolation level specification")
#define ERR_DBD_NO_CURRENT_TABLE  			PKG_DBD+94
	errortext(ERR_DBD_NO_CURRENT_TABLE, "<Database Driver>Browser is not positioned at a table")
#define ERR_DBD_NO_CURRENT_COLUMN 			PKG_DBD+95
	errortext(ERR_DBD_NO_CURRENT_COLUMN, "<Database Driver>Browser is not positioned at a column")
#define ERR_DBD_SAME_RESULT 	 			PKG_DBD+96
	errortext(ERR_DBD_SAME_RESULT, "<Database Driver>Second open cursor with no change to result set")
#define ERR_DBD_UNTRANSLATABLE_LMBCS		PKG_DBD+97
	errortext(ERR_DBD_UNTRANSLATABLE_LMBCS, "<Database Driver>Can't translate LMBCS code to native")
#define ERR_DBD_NO_CURRENT_RECORD 			PKG_DBD+98
	errortext(ERR_DBD_NO_CURRENT_RECORD, "<Database Driver>Cursor not at record for update or deletion")
#define ERR_DBD_STRING_TRUNCATED  			PKG_DBD+99
	errortext(ERR_DBD_STRING_TRUNCATED, "<Database Driver>Unacceptable string truncation")

#define ERR_DBD_TRANSFER_ERROR	 			PKG_DBD+100
	errortext(ERR_DBD_TRANSFER_ERROR, "<Database Driver>Unrecoverable transfer error")
#define ERR_DBD_BAD_DCCB		 			PKG_DBD+101
	errortext(ERR_DBD_BAD_DCCB, "<Database Driver>Driver connection block corrupted")
#define ERR_DBD_BAD_DDCB		 			PKG_DBD+102
	errortext(ERR_DBD_BAD_DDCB, "<Database Driver>Database connection block corrupted")
#define ERR_DBD_BAD_ENVBLK		 			PKG_DBD+103
	errortext(ERR_DBD_BAD_ENVBLK, "<Database Driver>Corrupted envblk")
#define ERR_DBD_BAD_QUERYTREE	 			PKG_DBD+104
	errortext(ERR_DBD_BAD_QUERYTREE, "<Database Driver>Invalid querytree")
#define ERR_DBD_BAD_TABNAME 	 			PKG_DBD+105
	errortext(ERR_DBD_BAD_TABNAME, "<Database Driver>Invalid tabname")
#define ERR_DBD_BAD_CBROWSER	 			PKG_DBD+106
	errortext(ERR_DBD_BAD_CBROWSER, "<Database Driver>Invalid catalog browser")
#define ERR_DBD_BAD_COLID		 			PKG_DBD+107
	errortext(ERR_DBD_BAD_COLID, "<Database Driver>Invalid colid")
#define ERR_DBD_BAD_WHERENODE	 			PKG_DBD+108
	errortext(ERR_DBD_BAD_WHERENODE, "<Database Driver>Invalid wherenode")
#define ERR_DBD_BAD_NEW_DEFINITION  		PKG_DBD+109
	errortext(ERR_DBD_BAD_NEW_DEFINITION, "<Database Driver>New column definition is not valid")

#define ERR_DBD_SCROLLED_CURSOR	 			PKG_DBD+110
	errortext(ERR_DBD_SCROLLED_CURSOR, "<Database Driver>Scroll option with current cursor invalid")
#define ERR_DBD_CARDINALITY 	 			PKG_DBD+111
	errortext(ERR_DBD_CARDINALITY, "<Database Driver>Cardinality error")
#define ERR_DBD_BACKEND_DB_ERROR  			PKG_DBD+112
	errortext(ERR_DBD_BACKEND_DB_ERROR, "<Database Driver>Back end database-specific error")
#define ERR_DBD_BAD_CONFIG_STRING 			PKG_DBD+113
	errortext(ERR_DBD_BAD_CONFIG_STRING, "<Database Driver>Configuration string is not valid")
#define ERR_DBD_BAD_PLATFORM	 			PKG_DBD+114
	errortext(ERR_DBD_BAD_PLATFORM, "<Database Driver>Driver can't use application's platform")
#define ERR_DBD_CODEPAGE_LOAD_ERROR 		PKG_DBD+115
	errortext(ERR_DBD_CODEPAGE_LOAD_ERROR, "<Database Driver>Translation table cannot be loaded")
#define ERR_DBD_MAPMEM_ERROR	 			PKG_DBD+116
	errortext(ERR_DBD_MAPMEM_ERROR, "<Database Driver>Memory map error")
#define ERR_DBD_RESOURCE_LOAD_ERROR 		PKG_DBD+117
	errortext(ERR_DBD_RESOURCE_LOAD_ERROR, "<Database Driver>Required resource cannot be loaded")
#define ERR_DBD_STATUS_ERROR	 			PKG_DBD+118
	errortext(ERR_DBD_STATUS_ERROR, "<Database Driver>Error building statdef, no details")
#define ERR_DBD_UNTRANSLATABLE_COLTYPE 		PKG_DBD+119
	errortext(ERR_DBD_UNTRANSLATABLE_COLTYPE, "<Database Driver>Column type is not valid")

#define ERR_DBD_STATEMENT_ABORT	 			PKG_DBD+120
	errortext(ERR_DBD_STATEMENT_ABORT, "<Database Driver>Operation on statement aborted")
#define ERR_DBD_DATA_TOO_LONG	 			PKG_DBD+121
	errortext(ERR_DBD_DATA_TOO_LONG, "<Database Driver>Data is too long for column")
#define ERR_DBD_BAD_NUMERIC_VALUE 			PKG_DBD+122
	errortext(ERR_DBD_BAD_NUMERIC_VALUE, "<Database Driver>Numeric value is outside accepted range")
#define ERR_DBD_STATE_VIOLATION	 			PKG_DBD+123
	errortext(ERR_DBD_STATE_VIOLATION, "<Database Driver>Function call is not allowed in state")
#define ERR_DBD_DEPENDENT_BROWSER_OPEN 		PKG_DBD+124
	errortext(ERR_DBD_DEPENDENT_BROWSER_OPEN, "<Database Driver>Browser still open on close driver/database")
#define ERR_DBD_NO_DB						PKG_DBD+125
	errortext(ERR_DBD_NO_DB, "Database not specified")

#define ERR_DAL_BAD_SESSION  				PKG_DBD+130
	errortext(ERR_DAL_BAD_SESSION, "<Data Access Application Layer>Invalid session handle")
#define ERR_DAL_BAD_QINDEX  					PKG_DBD+131
	errortext(ERR_DAL_BAD_QINDEX, "<Data Access Application Layer>Invalid index to queue")
#define ERR_DAL_REGFILE_ERROR  			PKG_DBD+132
	errortext(ERR_DAL_REGFILE_ERROR, "<Data Access Application Layer>Invalid registration file entry")
#define ERR_DAL_MORE_LEVELS_REQUIRED 		PKG_DBD+133
	errortext(ERR_DAL_MORE_LEVELS_REQUIRED, "<Data Access Application Layer>Too few data-source connection levels")
#define ERR_DAL_OPEN_SESSION  				PKG_DBD+134
	errortext(ERR_DAL_OPEN_SESSION, "<Data Access Application Layer>At least one session is still open")
#define ERR_DAL_BAD_COMMAND  				PKG_DBD+135
	errortext(ERR_DAL_BAD_COMMAND, "<Data Access Application Layer>Unknown database command")
#define ERR_DAL_TOO_FEW_ARGS 				PKG_DBD+136
	errortext(ERR_DAL_TOO_FEW_ARGS, "<Data Access Application Layer>Too few command arguments")
#define ERR_DAL_USERID_REQUIRED  			PKG_DBD+137
	errortext(ERR_DAL_USERID_REQUIRED, "<Data Access Application Layer>User ID required")
#define ERR_DAL_PASSWORD_REQUIRED  			PKG_DBD+138
	errortext(ERR_DAL_PASSWORD_REQUIRED, "<Data Access Application Layer>Password required")
#define ERR_DAL_TOO_MANY_ARGS  				PKG_DBD+139
	errortext(ERR_DAL_TOO_MANY_ARGS, "<Data Access Application Layer>Too many command arguments")
#define ERR_DAL_BAD_API_VERSION  			PKG_DBD+140
	errortext(ERR_DAL_BAD_API_VERSION, "<Data Access Application Layer>Driver's API version is not supported")

#define ERR_DAL_MEMORY_ERROR			PKG_DBD+141	
	errortext(ERR_DAL_MEMORY_ERROR, "<Data Access Application Layer>Memory allocation/deallocation error")
#define ERR_DAL_INVALID_ARGUMENT		PKG_DBD+142
	errortext(ERR_DAL_INVALID_ARGUMENT, "<Data Access Application Layer>Invalid command argument")
#define ERR_DAL_DRIVER_NOT_CAPABLE	PKG_DBD+143
	errortext(ERR_DAL_DRIVER_NOT_CAPABLE, "<Data Access Application Layer>Driver has insufficient capabilities")
#define ERR_DAL_DRIVER_LOAD_ERROR	PKG_DBD+144
	errortext(ERR_DAL_DRIVER_LOAD_ERROR, "<Data Access Application Layer>Error loading driver")
#define ERR_DAL_BAD_COLUMN				PKG_DBD+145
	errortext(ERR_DAL_BAD_COLUMN, "<Data Access Application Layer>Requested column doesn't exist")
#define ERR_DAL_UNSUPPORTED_TYPE		PKG_DBD+146
	errortext(ERR_DAL_UNSUPPORTED_TYPE, "<Data Access Application Layer>Unsupported data type")
#define ERR_DAL_ARG_TOO_LONG			PKG_DBD+147
	errortext(ERR_DAL_ARG_TOO_LONG, "<Data Access Application Layer>Command argument string too long")
#define ERR_DAL_CONVERSION_ERROR		PKG_DBD+148
	errortext(ERR_DAL_CONVERSION_ERROR, "<Data Access Application Layer>Data conversion error")
#define ERR_DAL_INVALID_CHARSET			PKG_DBD+149
	errortext(ERR_DAL_INVALID_CHARSET, "<Data Access Application Layer>Data-source charset out-of-range")

#define ERR_DAL_EXCEPTION_ERROR		PKG_DBD+171
	errortext(ERR_DAL_EXCEPTION_ERROR, "<Data Access Application Layer>Exception error")
#define ERR_DAL_CONNECTCANCEL_ERROR		PKG_DBD+172
	errortext(ERR_DAL_CONNECTCANCEL_ERROR, "<Data Access Application Layer>Connection cancelled")
#define ERR_DAL_SERVERPROCESS_ERROR		PKG_DBD+173
	errortext(ERR_DAL_SERVERPROCESS_ERROR, "<Data Access Application Layer>Must supply ID and password in script")
#define ERR_DAL_UNSUPPORTEDFUNC_ERROR		PKG_DBD+174
	errortext(ERR_DAL_UNSUPPORTEDFUNC_ERROR, "<Data Access Application Layer>@DbCommand contains unsupported function")
#define ERR_DAL_SQLPREPARE_ERROR		PKG_DBD+175
	errortext(ERR_DAL_SQLPREPARE_ERROR, "<Data Access Application Layer>Could not prepare SQL statement - possible invalid column or table name")
#define ERR_DAL_SQLEXECUTE_ERROR		PKG_DBD+176
	errortext(ERR_DAL_SQLEXECUTE_ERROR, "<Data Access Application Layer>Could not execute @db function")
#define ERR_DAL_PASSWORD_ERROR		PKG_DBD+177
	errortext(ERR_DAL_PASSWORD_ERROR, "<Data Access Application Layer>Bad authorization string")
#define ERR_DAL_NULLSFOUND_ERROR		PKG_DBD+178
	errortext(ERR_DAL_NULLSFOUND_ERROR, "<Data Access Application Layer>Null values found - cancelling @db function")
#define ERR_DAL_INVALID_NULL_REPLACE_ERROR	PKG_DBD+179
	errortext(ERR_DAL_INVALID_NULL_REPLACE_ERROR, "<Data Access Application Layer>Invalid NULL replace value")
#define ERR_DBD_RESULT_COUNT_LIMIT_EXCEED	PKG_DBD+180
	errortext(ERR_DBD_RESULT_COUNT_LIMIT_EXCEED, "The specified database lookup generated more than 65,534 results, which is too large for Notes to handle.")

/* End of Error Codes (don't exceed PKG_DBD+183!) */

#endif

