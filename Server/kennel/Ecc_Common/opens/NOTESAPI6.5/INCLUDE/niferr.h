
#ifndef NIF_ERR_DEFS
#define NIF_ERR_DEFS

/* NIF error status codes */

#define	ERR_SORT_DATATYPE			PKG_NIF+1
	errortext(ERR_SORT_DATATYPE,	"Cannot collate using unsortable datatype")
/*	The ERR_BAD_COLLECTION error is obsolete is only retained for 
	compatibility with pre-109 systems. */
#define	ERR_BAD_COLLECTION			PKG_NIF+2
	errortext(ERR_BAD_COLLECTION,	"Index corrupted - will be rebuilt automatically when database is closed or server is restarted")
#define	ERR_COLLECTION_HANDLE		PKG_NIF+3
	debugtext(ERR_COLLECTION_HANDLE, "Collection not open")
#define ERR_MORE_TO_DO				PKG_NIF+4
	internaltext(ERR_MORE_TO_DO,	"(more to do - call again)")
#define	ERR_COLLECTION_DELETED		PKG_NIF+5
	errortext(ERR_COLLECTION_DELETED, "Collection object was deleted")
#define ERR_UNSUPPORTED_FLAGS		PKG_NIF+6
	errortext(ERR_UNSUPPORTED_FLAGS, "Unsupported return flag(s)")
#define ERR_LOOKUP_ERROR			PKG_NIF+7
	errortext(ERR_LOOKUP_ERROR,		"Error looking up name in Domino Directory")
#define ERR_TOO_MANY_LEVELS			PKG_NIF+8
	errortext(ERR_TOO_MANY_LEVELS,	"Index entry has too many levels")
#define ERR_NO_SUCH_NAMESPACE		PKG_NIF+9
	errortext(ERR_NO_SUCH_NAMESPACE,"Domino Directory does not contain a required view")
#define ERR_NO_NAMES_FILE			PKG_NIF+10
	errortext(ERR_NO_NAMES_FILE,	"Domino Directory does not exist")
#define ERR_COLLECTION_NOT_CREATED	PKG_NIF+11
	internaltext(ERR_COLLECTION_NOT_CREATED, "(Collection does not exist, and was not created)")
#define	ERR_NAVIGATE_FAILED			PKG_NIF+12
	errortext(ERR_NAVIGATE_FAILED,	"No document to navigate to")
#define	ERR_VIEW_NOACCESS			PKG_NIF+13
	errortext(ERR_VIEW_NOACCESS,	"You are not authorized to access the view")
#define ERR_WRITE_COLLECTION_ERR	PKG_NIF+14
	debugtext(ERR_WRITE_COLLECTION_ERR,	"Error writing %p (%s) index: %e")
#define ERR_NO_SUCH_NAMES_BOOK		PKG_NIF+15
	errortext(ERR_NO_SUCH_NAMES_BOOK,	"No more Domino Directory databases")
#define ERR_NO_SUCH_ITEM			PKG_NIF+16
	errortext(ERR_NO_SUCH_ITEM,		"Specified item is not present")
#define ERR_NO_MORE_MEMBERS			PKG_NIF+17
	internaltext(ERR_NO_MORE_MEMBERS,	"(No more members in list)")
#define ERR_UNSUPPORTED_TYPE		PKG_NIF+18
	errortext(ERR_UNSUPPORTED_TYPE,	"Unsupported datatype in Domino Directory document")
#define ERR_NO_MORE_MATCH			PKG_NIF+19
	internaltext(ERR_NO_MORE_MATCH,	 "(No more matches in table)")
#define ERR_NO_REMOTE_INDEX			PKG_NIF+20
	errortext(ERR_NO_REMOTE_INDEX,	 "Index is not to be generated on server.")
#define ERR_UPDATE_INTERRUPTED		PKG_NIF+21
	errortext(ERR_UPDATE_INTERRUPTED,"Someone else deleted this index while you were updating it.")
#define ERR_CONTAINER_ITEM_NOT_FOUND PKG_NIF+22
	internaltext(ERR_CONTAINER_ITEM_NOT_FOUND,"($Container item not found in view note)")
#define ERR_PARTIAL_MATCH			PKG_NIF+23
	internaltext(ERR_PARTIAL_MATCH,	"(Partial match found with TEXT_PARTIALCOMPARE)")
#define ERR_USER_NOT_FOUND 			PKG_NIF+24
	errortext(ERR_USER_NOT_FOUND,"User or server name not found in Domino Directory")
#define ERR_IDFILE_NOT_FOUND		PKG_NIF+25
	errortext(ERR_IDFILE_NOT_FOUND,"ID file not found in Directory")
#define ERR_WAS_PERMUTED			PKG_NIF+26
	debugtext(ERR_WAS_PERMUTED,		"Entry had multiple permutations")
#define ERR_BAD_COLLATION			PKG_NIF+27
	errortext(ERR_BAD_COLLATION,	"View has a bad collation description.  It can't be read.")
#define ERR_BAD_COLLECTION_DATA		PKG_NIF+28
	errortext(ERR_BAD_COLLECTION_DATA,"Invalid collection data was detected.")
#define ERR_REBUILD_VIEW			PKG_NIF+29
	errortext(ERR_REBUILD_VIEW,		"View is damaged.  Please rebuild it (by pressing shift-F9 or running UPDALL -r).")
#define ERR_RDONLY_DIFF_PLATFORM	PKG_NIF+30
	errortext(ERR_RDONLY_DIFF_PLATFORM,"View cannot be created.  This is a read-only database from a different platform.")
#define ERR_NIF_COLLREBUILDPDL 		PKG_NIF+31
	errortext(ERR_NIF_COLLREBUILDPDL,	"Informational, rebuilding view - notes have been purged since last update (reading %s %s note Title:'%s')")
#define ERR_NIF_POOLFULL			PKG_NIF+32
	errortext(ERR_NIF_POOLFULL,		"Insufficient memory - index pool is full.")
#define ERR_NIF_COLLREBUILD			PKG_NIF+33
	errortext(ERR_NIF_COLLREBUILD,		"Informational, rebuilding view - validation error (reading %s %s note Title:'%s')")
#define ERR_NIF_COLLREBUILDNCOI		PKG_NIF+34
	errortext(ERR_NIF_COLLREBUILDNCOI,	"Informational, rebuilding view - no container or index (reading %s %s note Title:'%s')")
#define ERR_NIF_COLLREBUILDCIL 		PKG_NIF+35
	errortext(ERR_NIF_COLLREBUILDCIL,	"Informational, rebuilding view - container integrity lost (reading %s %s note Title:'%s')")
#define ERR_NIF_COLLREBUILDCS  		PKG_NIF+36
	errortext(ERR_NIF_COLLREBUILDCS,	"Informational, rebuilding view - user specified REBUILD (reading %s %s note Title:'%s')")
#define ERR_NIF_COLLREBUILDDC  		PKG_NIF+37						 
	errortext(ERR_NIF_COLLREBUILDDC,	"Informational, rebuilding view - database ID changed (reading %s %s note Title:'%s')")
#define ERR_NIF_COLLREBUILDCDP  		PKG_NIF+38
	errortext(ERR_NIF_COLLREBUILDCDP,	"Informational, rebuilding view - database had cutoff-delete performed (reading %s %s note Title:'%s')")
#define ERR_NIF_COLLREBUILDCDL 		PKG_NIF+39
	errortext(ERR_NIF_COLLREBUILDCDL,	"Informational, rebuilding view - database cutoff date later than last update (reading %s %s note Title:'%s')")
#define ERR_NIF_COLLREBUILDRC  		PKG_NIF+40
	errortext(ERR_NIF_COLLREBUILDRC,	"Informational, rebuilding view - user roles changed (reading %s %s note Title:'%s')")
#define ERR_NIF_COLLREBUILDFC  		PKG_NIF+41
	errortext(ERR_NIF_COLLREBUILDFC,	"Informational, rebuilding view - selection or column formula changed (reading %s %s note Title:'%s')")
#define ERR_NIF_COLLREBUILDCC  		PKG_NIF+42
	errortext(ERR_NIF_COLLREBUILDCC,	"Informational, rebuilding view - collation changed (reading %s %s note Title:'%s')")
#define ERR_NIF_COLLREBUILDSCC 		PKG_NIF+43
	errortext(ERR_NIF_COLLREBUILDSCC,	"Informational, rebuilding view - subtotal codes changed (reading %s %s note '%s')")
#define ERR_BAD_COLLATION_NUM		PKG_NIF+45
	errortext(ERR_BAD_COLLATION_NUM,"Collation number specified negative or greater than number of collations in view.")
#define ERR_BAD_CHASHTBL_DELETION	PKG_NIF+46
	internaltext(ERR_BAD_CHASHTBL_DELETION,	"The wrong entry was deleted from the collection hash table.")
#define ERR_BAD_CUHASHTBL_DELETION	PKG_NIF+47
	internaltext(ERR_BAD_CUHASHTBL_DELETION,"The wrong entry was deleted from the collection user hash table.")
#define ERR_NIF_BAD_KEYBUFFER		PKG_NIF+48
	errortext(ERR_NIF_BAD_KEYBUFFER,"Too many Items in Key Buffer pass to NIFFindKey.")
#define ERR_NIF_TOO_MANY_NAMES		PKG_NIF+49
	errortext(ERR_NIF_TOO_MANY_NAMES,"Recipient name not unique, too many found in Directory, buffer exceeded 64K byte limit.")
#define ERR_NAMED_FOLDER_NOT_FOUND	PKG_NIF+50
	errortext(ERR_NAMED_FOLDER_NOT_FOUND, 	"Named folder not found, operation not done.")
#define ERR_COLLHASH_POOLFULL		PKG_NIF+51
	errortext(ERR_COLLHASH_POOLFULL,		"NIF collection hash table is full.")
#define ERR_NIF_COLLOBJ		 		PKG_NIF+52
	errortext(ERR_NIF_COLLOBJ,				"Informational, rebuild view needed - collection object cannot be read (reading %s %s note Title:'%s')")
#define ERR_NIF_COLLHDR		 		PKG_NIF+53
	errortext(ERR_NIF_COLLHDR,				"Informational, rebuild view needed - invalid collection header (reading %s %s note Title:'%s')")
#define ERR_NIF_COLLTBL		 		PKG_NIF+54
	errortext(ERR_NIF_COLLTBL,				"Informational, rebuild view needed - collation table has changed (reading %s %s note Title:'%s')")
#define	ERR_NIF_REMOTE_NAB			PKG_NIF+55
	errortext(ERR_NIF_REMOTE_NAB,"Error attempting to access the Directory %p specified in notes.ini.  Error is ")
#define ERR_NIF_FULLSEARCH	 		PKG_NIF+56
	errortext(ERR_NIF_FULLSEARCH,	"Informational, rebuilding view - previous rebuild did not complete (reading %s %s note Title:'%s')")
#define ERR_NIF_UPDATE_NA_VIEW		PKG_NIF+57
	errortext(ERR_NIF_UPDATE_NA_VIEW,	"Updating Domino Directory view '%s'")
#define ERR_DA_BUF_INSUF			PKG_NIF+58
	errortext(ERR_DA_BUF_INSUF,		"Directory Assistance buffer insufficient.")
#define ERR_DA_INIT_FAILED		PKG_NIF+59
	errortext(ERR_DA_INIT_FAILED,	"Directory Assistance tables failed to load properly.")
#define ERR_DA_RELOAD_FAILED		PKG_NIF+60
	errortext(ERR_DA_RELOAD_FAILED,	"Directory Assistance tables failed to reload properly.")
#define ERR_DA_INVALID_RULE			PKG_NIF+61
	errortext(ERR_DA_INVALID_RULE,	"Directory Assistance database has one or more invalid rules for domain %s.")
#define ERR_DA_INVALID_REPLICA			PKG_NIF+62
	errortext(ERR_DA_INVALID_REPLICA,	"Directory Assistance database has one or more invalid replicas for domain %s.")

/* 	The following errors are used in lookup.c for the LDAP Gateway. */

#define ERR_NIF_BIND				PKG_NIF+63
	errortext(ERR_NIF_BIND,			"Unable to bind to LDAP Server %s.")
#define ERR_NIF_CONNECT				PKG_NIF+64
	errortext(ERR_NIF_CONNECT,		"Unable to connect to LDAP Server %s.")
#define ERR_NIF_BAD_URL				PKG_NIF+65
	errortext(ERR_NIF_BAD_URL,		"BAD URL for LDAP Server %s using port %ld.")
#define ERR_NIF_LDAP_LOOKUP			PKG_NIF+66
	errortext(ERR_NIF_LDAP_LOOKUP,	"Error looking up name on LDAP Server; See server log for further details.")
#define ERR_NIF_LDAP_SEARCH			PKG_NIF+67
	errortext(ERR_NIF_LDAP_SEARCH,	"Failed search on LDAP Server %s Reason: %s")
		

#define ERR_NIF_SORT_EOF			PKG_NIF+68
	internaltext(ERR_NIF_SORT_EOF,		"No more sort results - EOF.")
#define ERR_LOOKUP_CACHE_FULL 			PKG_NIF+69
	errortext(ERR_LOOKUP_CACHE_FULL, 	"Insufficient memory - NAMELookup data cache is full.")
#define ERR_LOOKUP_HASH_FULL 			PKG_NIF+70
	errortext(ERR_LOOKUP_HASH_FULL, 	"Insufficient memory - NAMELookup data hash pool is full.")
#define ERR_NIF_VRB_SCRATCH_DIR_INV	PKG_NIF+71
	errortext(ERR_NIF_VRB_SCRATCH_DIR_INV,	"Informational - VIEW_REBUILD_SCRATCH_DIR invalid, using default temp directory instead.")
#define ERR_MUST_BE_SAME_DB				PKG_NIF+72
	errortext(ERR_MUST_BE_SAME_DB,		"Anti-folder views must be stored in data database")
#define ERR_NIF_SORT_NO_BUF				PKG_NIF+73
	internaltext(ERR_NIF_SORT_NO_BUF,	"Sort did not produce a buffer of results due to time constraints. Call again.")
#define ERR_CANTWRITE_NONIF				PKG_NIF+74
	internaltext(ERR_CANTWRITE_NONIF,	"NIF hasn't been initialized in this process so buffer cannot be written.")


/* More ldap gateway errors */

#define ERR_NIF_LDAP_ATTRNF			PKG_NIF+75
	errortext(ERR_NIF_LDAP_ATTRNF,	"Failed search on LDAP Server %s Reason: %s, DN: %s, Attribute: %s")
#define ERR_NIF_LDAP_INVSYNTAX 		PKG_NIF+76
	errortext(ERR_NIF_LDAP_INVSYNTAX,	"Failed search on LDAP Server %s, DN: %s, Attribute %s has invalid syntax: %d")
#define	ERR_DA_CANT_ACCESS_AB					PKG_NIF+78
	errortext(ERR_DA_CANT_ACCESS_AB,"Directory Assistance could not access Directory %p, error: ")
#define ERR_DA_ALTERNATE_REPLICA_UNAVAILABLE	PKG_NIF+79
	errortext(ERR_DA_ALTERNATE_REPLICA_UNAVAILABLE,"Directory Assistance could not find an alternate replica for domain %s")
#define ERR_DA_ALTERNATE_REPLICA_FOUND		PKG_NIF+80
	errortext(ERR_DA_ALTERNATE_REPLICA_FOUND,"Directory Assistance found alternate Directory replica %p for domain %s")
#define	ERR_NIF_REMOTE_NAB2			PKG_NIF+81
	errortext(ERR_NIF_REMOTE_NAB2,"Error attempting to access the Directory %p (no available alternatives),  error is ")
#define ERR_DA_RELOADING			PKG_NIF+82
	errortext(ERR_DA_RELOADING,"Directory Assistance is reloading internal tables due to change in %p.")
#define ERR_NIF_SORT_DATA_TOO_BIG	PKG_NIF+83
	internaltext(ERR_NIF_SORT_DATA_TOO_BIG, "Data too large for a sort buffer.")
#define	LOG_NIF_SKIP_REBUILD_OPT	PKG_NIF+84
	errortext(LOG_NIF_SKIP_REBUILD_OPT,"Unable to use optimized view rebuild for view '%s' due to insufficient disk space at %s. Need approximately %ld MB for this view. Using standard rebuild instead.")
#define	ERR_NAMELOOKUP_TOO_BIG	PKG_NIF+85
	errortext(ERR_NAMELOOKUP_TOO_BIG, "NAMELookups are limited on this server to a size smaller than what would be returned.  See your Domino Administrator for more information.")
#define	LOG_NIF_ERR_REBUILD_DIR	PKG_NIF+86
	errortext(LOG_NIF_ERR_REBUILD_DIR,"Unable to access view rebuild directory %s. Using default temp directory instead.")
#define	ERR_LOCK_ON_WAIT_FOR_REBUILD		PKG_NIF+87
	errortext(ERR_LOCK_ON_WAIT_FOR_REBUILD,	"Unable to wait for rebuild because collection sem is locked.")
#define	ERR_TYPEAHEAD_NOT_SUPPORTED		PKG_NIF+88
	errortext(ERR_TYPEAHEAD_NOT_SUPPORTED,	"Typeahead not supported on Server.")
#define ERR_CMD_SHOW_NIFPOOL		PKG_NIF+89
	stringtext(ERR_CMD_SHOW_NIFPOOL, "NIFPOOL")
#define ERR_HELP_SHOW_NIFPOOL		PKG_NIF+90
	stringtext(ERR_HELP_SHOW_NIFPOOL, "* Show the contents of the Server's NIF pool.")
#define ERR_DA_INITIALIZING			PKG_NIF+91
	stringtext(ERR_DA_INITIALIZING, "Directory Assistance Initializing.")
#define ERR_CMD_SHOW_LDAPPOOL		PKG_NIF+92
	stringtext(ERR_CMD_SHOW_LDAPPOOL, "LDAPPOOL")
#define ERR_HELP_SHOW_LDAPPOOL		PKG_NIF+93
	stringtext(ERR_HELP_SHOW_LDAPPOOL, "* Show the contents of the Server's LDAP pool.")
#define ERR_DIRECTORY_CATALOG_CORRUPT	PKG_NIF+94
	stringtext(ERR_DIRECTORY_CATALOG_CORRUPT, "Directory Catalog is Corrupt, Should be rebuilt.")
#define ERR_DA_FLUSH_GROUPCACHE		PKG_NIF+95
	stringtext(ERR_DA_FLUSH_GROUPCACHE, "Directory Assistance flushed server's group cache!")

#define ERR_LDAP_NEED_XCERT			PKG_NIF+96
	errortext(ERR_LDAP_NEED_XCERT,		"Cross cert is needed for LDAP Server: %s:%d")

#define MSG_LDAP_LOADSCHEMABEGIN					(PKG_NIF+97)
	errortext(MSG_LDAP_LOADSCHEMABEGIN,				"LDAP Schema: Started loading...")
#define MSG_LDAP_LOADSCHEMAEND						(PKG_NIF+98)
	errortext(MSG_LDAP_LOADSCHEMAEND,				"LDAP Schema: Finished loading")
#define MSG_LDAP_LOADSCHEMAFAIL						(PKG_NIF+99)
	errortext(MSG_LDAP_LOADSCHEMAFAIL,				"LDAP Schema: Failed loading")
#define MSG_LDAP_EXPORTSCHEMABEGIN					(PKG_NIF+100)
	errortext(MSG_LDAP_EXPORTSCHEMABEGIN,			"LDAP Schema: Started exporting...")
#define MSG_LDAP_EXPORTSCHEMAEND					(PKG_NIF+101)
	errortext(MSG_LDAP_EXPORTSCHEMAEND,				"LDAP Schema: Finished exporting")
#define MSG_LDAP_EXPORTSCHEMAFAIL					(PKG_NIF+102)
	errortext(MSG_LDAP_EXPORTSCHEMAFAIL,			"LDAP Schema: Failed exporting")

#define ERR_DA_INVALID_ENCRYPT_KEY					(PKG_NIF+103)
	errortext(ERR_DA_INVALID_ENCRYPT_KEY,	"Directory Assistance unable to decrypt document for domain %s, skipping this domain.")

#define ERR_DA_RETRY_LOOKUP							(PKG_NIF+104)
	errortext(ERR_DA_RETRY_LOOKUP,					"Directory Assistance has detected unavailable replica, retry namelookup")

#define ERR_LSCHEMA_NOTSERVERMACHINE				(PKG_NIF+105)
	errortext(ERR_LSCHEMA_NOTSERVERMACHINE,			"LDAP Schema: This is not a server machine!")

#define ERR_LSCHEMA_CALLINITFIRST					(PKG_NIF+106)
	errortext(ERR_LSCHEMA_CALLINITFIRST,			"LDAP Schema: Call LSchemaInit() first!")

#define ERR_LSCHEMA_REPLICATESCHEMA					(PKG_NIF+107)
	errortext(ERR_LSCHEMA_REPLICATESCHEMA,			"LDAP Schema: Failed to create replica of schema database on LDAP server: %s from admin server: %s!")

#define ERR_LSCHEMA_EXPORTDUPLICATE					(PKG_NIF+108)
	errortext(ERR_LSCHEMA_EXPORTDUPLICATE,			"LDAP Schema: Failed to export schema since duplicate schema entries with OID: %s found!")

#define ERR_NIF_REBUILD_IN_PROGRESS					(PKG_NIF+109)
	errortext(ERR_NIF_REBUILD_IN_PROGRESS,			"A view rebuild is in progress")

/* For ldap gateway custom filter error */
#define ERR_NIF_AUTHENTICATION_FILTER			PKG_NIF+110
	errortext(ERR_NIF_AUTHENTICATION_FILTER,	"Directory Assistance: Bad filter for customized authentication filter. The system default filter will be used.")
#define ERR_NIF_AUTHORIZATION_FILTER			PKG_NIF+111
	errortext(ERR_NIF_AUTHORIZATION_FILTER,		"Directory Assistance: Bad filter for customized authorization filter. The system default filter will be used.")
#define ERR_NIF_MAIL_FILTER						PKG_NIF+112
	errortext(ERR_NIF_MAIL_FILTER,				"Directory Assistance: Bad filter for customized mail filter. The system default filter will be used.")
	
/* Errors for Roaming Users (NameLookupUserID) */
#define ERR_LOOKUP_RNAB_REMOTE					PKG_NIF+113
	errortext(ERR_LOOKUP_RNAB_REMOTE,			"Roaming User Name & Address book is not local. ")
/* Directory Type strings */
#define STR_DIRECTORY_LDAP					PKG_NIF+114
	errortext(STR_DIRECTORY_LDAP,			" (LDAP)")
#define STR_DIRECTORY_CONFIGNAB				PKG_NIF+115
	errortext(STR_DIRECTORY_CONFIGNAB,		"Configuration")
#define STR_DIRECTORY_DIRCAT				PKG_NIF+116
	errortext(STR_DIRECTORY_DIRCAT,			"Directory Catalog '%s' in use")
#define STR_DIRECTORY_REMOTE_PRIMARY		PKG_NIF+117
	errortext(STR_DIRECTORY_REMOTE_PRIMARY,	"Remote Primary")
#define STR_DIRECTORY_PRIMARY				PKG_NIF+118
	errortext(STR_DIRECTORY_PRIMARY,		"Primary")
#define STR_DIRECTORY_SECONDARY				PKG_NIF+119
	errortext(STR_DIRECTORY_SECONDARY,		"Secondary")
#define ERR_DA_RELOADING2					PKG_NIF+120
	errortext(ERR_DA_RELOADING2,"Directory Assistance is reloading internal tables due to remote server failure.")
#define ERR_LSCHEMA_LDIFFILE_MISSING		PKG_NIF+121
	errortext(ERR_LSCHEMA_LDIFFILE_MISSING,	"LDAP Schema: lschema.ldif")
#define	CACHE_ENTRY_TOO_LARGE				PKG_NIF+122
	errortext(CACHE_ENTRY_TOO_LARGE,		"Cache entry exceeded the maximum size.")
#define ERR_DATA_STALE						PKG_NIF+123
	errortext(ERR_DATA_STALE,				"Cache entry was stale.")
#define	ERR_MEM_PAINFUL						PKG_NIF+124
	errortext(ERR_MEM_PAINFUL,				"Too little memory available to initialize the cache.")
#define	ERR_LOCK_CONFLICT					PKG_NIF+125
	errortext(ERR_LOCK_CONFLICT,			"Unable to complete operation due to a lock heirarchy conflict.")
#define	ERR_DUPLICATE_ENTRY					PKG_NIF+126
	errortext(ERR_DUPLICATE_ENTRY,			"Duplicate entry already added to cache.")
#define ERR_NLCACHE2_MEM_ERROR				PKG_NIF+127
	errortext(ERR_NLCACHE2_MEM_ERROR,	  	"Could not lock NLCache2 object block.")

/* 	NOTE: NIF errors can only go up to PKG_NIF+127 - higher values are used in NSF as PKG_NSF2 */

/*	NOTE: NIF2 errors from PKG_NIF2+0 - PKG_NIF2+127 */
#define ERR_NLCACHE2_SEM_ERROR				PKG_NIF2+0
	errortext(ERR_NLCACHE2_SEM_ERROR,		"Could not obtain NLCache2 master semaphore.")
#define ERR_NLCACHE2_ACTIVE					PKG_NIF2+1
	errortext(ERR_NLCACHE2_ACTIVE,			"NLCache2 already active.")
#define ERR_NLCACHE2_NOT_ACTIVE			  	PKG_NIF2+2
	errortext(ERR_NLCACHE2_NOT_ACTIVE,	  	"NLCache2 not active.")
#define ERR_DA_NODOMAIN						PKG_NIF2+3
	stringtext(ERR_DA_NODOMAIN, 			"Warning:  Please define a domain in notes.ini file by setting the DOMAIN= variable")

#define STR_LDAP_PROTOCOL			  		PKG_NIF2+4
	errortext(STR_LDAP_PROTOCOL,	  		"LDAP")
#define STR_NRPC_PROTOCOL			  		PKG_NIF2+5
	errortext(STR_NRPC_PROTOCOL,	  		"Notes")
#define STR_UNKNOWN_PROTOCOL			  	PKG_NIF2+6
	errortext(STR_UNKNOWN_PROTOCOL,			"UNKNOWN")
#define STR_SHOW_DIRECTORY_HEADER           PKG_NIF2+7
	errortext(STR_SHOW_DIRECTORY_HEADER,	"   DomainName      DirectoryType         ClientProtocol Replica/LDAP Server\n   --------------- --------------------- -------------- -----------------------")
#define ERR_NIF_VIEW_DELETED_MSG			PKG_NIF2+8
	errortext(ERR_NIF_VIEW_DELETED_MSG,		"View has been deleted: %p (%s)")
#define ERR_NIF_VIEW_DELETED				PKG_NIF2+9
	errortext(ERR_NIF_VIEW_DELETED,			"View has been deleted")

#define ERR_CACHE_INITIALIZER_MISMATCH	  	PKG_NIF2+10
	errortext(ERR_CACHE_INITIALIZER_MISMATCH,	"Shared cache parameter mismatch.")
#define ERR_NIF_LDAP_UNAVAILABLE			PKG_NIF2+11
	errortext(ERR_NIF_LDAP_UNAVAILABLE,		"LDAP Server %s is NOT available.")

#define ERR_BAD_ITEMTABLE_DATA				PKG_NIF2+12
	errortext(ERR_BAD_ITEMTABLE_DATA,		"ItemTable data is out of range")
#define ERR_NULL_COLLECTION_CIDB			PKG_NIF2+13
	errortext(ERR_NULL_COLLECTION_CIDB,		"Collection container is NULL")
#define ERR_VIEW_FORMULA_CHANGED	  		PKG_NIF2+14
	errortext(ERR_VIEW_FORMULA_CHANGED,		"View selection or column formula changed")
#define ERR_NAMELOOKUP_DOMAIN_UNAVAILABLE  	PKG_NIF2+15
	errortext(ERR_NAMELOOKUP_DOMAIN_UNAVAILABLE,  "One or more Directory Assistance Domains used to process NAMELookup requests was unavailable.")
#define ERR_VIEW_FORMULA_CHANGED_MSG		PKG_NIF2+16
	errortext(ERR_VIEW_FORMULA_CHANGED_MSG,	"View selection or column formula changed: %p (%s)")
#define ERR_DA_ALTERNATE_CLUSTER_REPLICA_FOUND		PKG_NIF2+17
	errortext(ERR_DA_ALTERNATE_CLUSTER_REPLICA_FOUND,"Directory Assistance found alternate Directory Cluster replica %p for domain %s")
#define ERR_NIF_LDAP_REPLICA_UNAVAILABLE	PKG_NIF2+18
	errortext(ERR_NIF_LDAP_REPLICA_UNAVAILABLE,	"LDAP Server is NOT available.")
#define ERR_DA_NO_REMOTE_PRIMARY			PKG_NIF2+19
	errortext(ERR_DA_NO_REMOTE_PRIMARY,	"WARNING: Domain %s is your remote primary domain, and no replicas are available.  Your server cannot process users and groups.  Please contact your system administrator.")
#define ERR_NIF_COLLREBUILDDD  				PKG_NIF2+20
	errortext(ERR_NIF_COLLREBUILDDD,	"Informational, rebuilding view - removing deleted documents(reading %s %s note Title:'%s')")
#define ERR_LSCHEMA_ADMINNOTSET					(PKG_NIF2+21)
	errortext(ERR_LSCHEMA_ADMINNOTSET,			"LDAP Schema: Failed to create replica of schema database on LDAP server: %s.  Because the administration server of Domino Directory has not yet been set!")
#define ERR_NIF_COLLNOTUPDATED				PKG_NIF2+22
	errortext(ERR_NIF_COLLNOTUPDATED,	"Informational, collection was not updated to avoid blocking.")
#define ERR_BAD_SORT_FILE					PKG_NIF2+23
	errortext(ERR_BAD_SORT_FILE,			"Sort file is invalid.")
#define ERR_COLLECTION_WRONG_PROCESS		PKG_NIF2+24
	errortext(ERR_COLLECTION_WRONG_PROCESS,	"Collections cannot be shared between processes.")			 
#define ERR_VIEW_TIME_VARYING				PKG_NIF2+25
	internaltext(ERR_VIEW_TIME_VARYING,		"Collection is time-varying and has not been opened.")			 
#define ERR_DA_REMOTE_IS_CONFIG				PKG_NIF2+26
	errortext(ERR_DA_REMOTE_IS_CONFIG,	"Warning!!  Remote Primary configured in Directory Assistance is a Configuration Directory.  Ignoring this directory.")			 
#define	ERR_DA_CANT_ACCESS_DBLINK 			PKG_NIF2+27
	errortext(ERR_DA_CANT_ACCESS_DBLINK, "Could not access a directory on server %p specified by a database link in Directory Assistance; error:  ")
#define	ERR_DA_DBLINK_NOHINT 				PKG_NIF2+28
	errortext(ERR_DA_DBLINK_NOHINT, 	 "Could not access a directory specified by a database link in Directory Assistance: link contains no server hint ")
#define ERR_EXISTS_ON_PRIVATEQUEUE			PKG_NIF2+29
	errortext(ERR_EXISTS_ON_PRIVATEQUEUE,	"Entry exists on thread-specific Name Collection Queue")			 
#define ERR_NCQUEUE_ENTRY_LDAP				PKG_NIF2+30
	errortext(ERR_NCQUEUE_ENTRY_LDAP,		"Entry returned from thread-specific Name Collection Queue is an LDAP entry")			 
#define STR_DIRECTORY_MAB					PKG_NIF2+31
	errortext(STR_DIRECTORY_MAB,			"Directory Assistance Database '%s' in use")
/* New Errors used to log the occurrences that cause ERR_DA_INIT_FAILED to be returned */
#define	ERR_DA_NOT_ENABLED		 			PKG_NIF2+32
	errortext(ERR_DA_NOT_ENABLED, 	 		"Directory Assistance not enabled")
#define	ERR_DA_PRIMARYAB_FAILED 			PKG_NIF2+33
	errortext(ERR_DA_PRIMARYAB_FAILED, 	 "Directory Assistance failed opening Primary Domino Directory %s, error: ")
#define	ERR_DA_REG_TERMINATION_FAILED 	  	PKG_NIF2+34
	errortext(ERR_DA_REG_TERMINATION_FAILED, "Failed registering Directory Assistance termination routine, error: ") 
#define	ERR_DA_DB_FAILED 					PKG_NIF2+35
	errortext(ERR_DA_DB_FAILED, 	 	 "Error opening Directory Assistance Database %s, error: ")
#define	ERR_DA_INSUFFICIENT_MEMORY  		PKG_NIF2+36
	errortext(ERR_DA_INSUFFICIENT_MEMORY, "Insufficient memory to initialize Directory Assistance")
/* End log messages caused by ERR_DA_INIT_FAILED */
#define ERR_LOOKUP_DBNAME_NOT_FOUND			PKG_NIF2+37
	errortext(ERR_LOOKUP_DBNAME_NOT_FOUND,	"WARNING: NAMELookups from this server to the remote Domino Directory %s when Extended ACLs are enabled is prohibited.  Correct by creating a Directory Assistance document for it on %s.")
#define ERR_NAMELOOKUP_VIEW_ERROR			PKG_NIF2+38
	errortext(ERR_NAMELOOKUP_VIEW_ERROR,	"Directory Assistance could not access a Directory in Domain %s, View %s can not be opened, error: ")
#define ERR_NAMELOOKUP_DIRECTORY_ERROR	   	PKG_NIF2+39
	errortext(ERR_NAMELOOKUP_DIRECTORY_ERROR,	"Error searching a directory in Domain %s for %s, error: ")
#define ERR_MULTIPLE_DB_MATCHES				PKG_NIF2+40
	errortext(ERR_MULTIPLE_DB_MATCHES,		"More than one database with the name %s was found.")

/* 	NOTE: NIF2 errors can only go up to PKG_NIF2+127 */
	
#endif

