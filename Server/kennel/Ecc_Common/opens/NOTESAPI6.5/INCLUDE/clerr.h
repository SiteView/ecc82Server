
#ifndef CLIENT_ERR_DEFS
#define CLIENT_ERR_DEFS

/* CLIENT access routines error code */

#define ERR_RENAME						PKG_CLIENT+1
	errortext(ERR_RENAME,				"Cannot rename a file to a new server")
#define ERR_VERSION						PKG_CLIENT+2
	errortext(ERR_VERSION,				"Server does not support this version of the network protocol")
#define ERR_SERVER_NOT_FOUND			PKG_CLIENT+3
	errortext(ERR_SERVER_NOT_FOUND,		"Unable to find path to server")
#define ERR_SERVER_NAME_CHANGED			PKG_CLIENT+4
	errortext(ERR_SERVER_NAME_CHANGED,	"Connection denied. The server you connected to has a different name from the one requested.")
#define ERR_NOT_IMPLEMENTED				PKG_CLIENT+5
	errortext(ERR_NOT_IMPLEMENTED,		"This function is not implemented on this version of the server")
#define ERR_MULTI_THREAD				PKG_CLIENT+6
	apitext(ERR_MULTI_THREAD,			"A database handle to a remote database cannot be used by more than one thread.")
#define ERR_SERVER_NOT_RESPONDING		PKG_CLIENT+7
	errortext(ERR_SERVER_NOT_RESPONDING, "The server is not responding. The server may be down or you may be experiencing network problems. Contact your system administrator if this problem persists.")
#define ERR_RETRY_NOTE_UPDATE			PKG_CLIENT+8
	internaltext(ERR_RETRY_NOTE_UPDATE,	"(Retry NoteUpdate)")
#define ERR_RETRY_TRANSACTION			PKG_CLIENT+9
	internaltext(ERR_RETRY_TRANSACTION,	"(internal status; retry the transaction)")
#define ERR_MSG_TOO_SMALL				PKG_CLIENT+10
	errortext(ERR_MSG_TOO_SMALL,		"Network protocol error: message from server is too small")
#define ERR_MSG_FROM_SERVER				PKG_CLIENT+11
	errortext(ERR_MSG_FROM_SERVER,		"Network protocol error: message from server cannot be deciphered")
#define ERR_SERVER_NOT_RESPONDING1		PKG_CLIENT+12
	errortext(ERR_SERVER_NOT_RESPONDING1, "Server ")
#define ERR_SERVER_NOT_RESPONDING2		PKG_CLIENT+13
	stringtext(ERR_SERVER_NOT_RESPONDING2, " is not responding")
#define ERR_NOT_AUTHENTIC				PKG_CLIENT+14
	errortext(ERR_NOT_AUTHENTIC,		"Remote system's identity is either unknown or fraudulent")
#define ERR_SCAN_LOCK_LOST				PKG_CLIENT+15
	errortext(ERR_SCAN_LOCK_LOST,		"Session to server lost while database replication or copy in progress")
#define ERR_MULTI_TRANS					PKG_CLIENT+16
	errortext(ERR_MULTI_TRANS,			"In order to do multiple transactions simultaneously, you cannot use the same DB handle.")
#define ERR_SERVER_NOT_RESPONDING3		PKG_CLIENT+17
	errortext(ERR_SERVER_NOT_RESPONDING3, "Name server is not responding")
#define ERR_CL_NO_REMOTE_CONSOLE_ACCESS PKG_CLIENT+18
	errortext(ERR_CL_NO_REMOTE_CONSOLE_ACCESS,"You are not authorized to use the remote console on this server")
#define ERR_CL_NO_RESPONSE 				PKG_CLIENT+19
	errortext(ERR_CL_NO_RESPONSE,		"No response from server for this command")
#define ERR_TERMINATE					PKG_CLIENT+20
	errortext(ERR_TERMINATE,			"Session Closed")
#define ERR_CL_NOT_RESPONDING 	  		PKG_CLIENT+21
	errortext(ERR_CL_NOT_RESPONDING,	"Server is not responding to remote console commands (try again later)")
#define ERR_NEED_RECEIVE_DATA			PKG_CLIENT+22
	internaltext(ERR_NEED_RECEIVE_DATA,	"(Out of received data, need more)")
#define ERR_SRV_LENGTH_MISMATCH			PKG_CLIENT+23
	errortext(ERR_SRV_LENGTH_MISMATCH,	"Multi-Segment ID table length from server is not the length expected")
#define ERR_LOG_NOFAILOVER				PKG_CLIENT+24
	errortext(ERR_LOG_NOFAILOVER,		"Unable to failover replica ID (%h) from server %A to any other cluster member")
#define ERR_LOOKFOR_REPLICA				PKG_CLIENT+25
	errortext(ERR_LOOKFOR_REPLICA,		"Looking for replica on server %A")
#define ERR_LOOKUP						PKG_CLIENT+26
	errortext(ERR_LOOKUP,				"Lookup on server %a")
#define ERR_FOUND_REPLICA				PKG_CLIENT+27
	errortext(ERR_FOUND_REPLICA,		"Failover on replica ID (%h) from server %A to %A")
#define ERR_LOOKFOR_REPLICA_EXT			PKG_CLIENT+28
	errortext(ERR_LOOKFOR_REPLICA_EXT,	"Looking for replica (%h) on server %A")
#define ERR_RETRY_WITH_NEW_SESSION		PKG_CLIENT+29
	internaltext(ERR_RETRY_WITH_NEW_SESSION,	"(internal status; retry the transaction after dropping the session to the server)")
#define ERR_CLUSTER_NOMATES		 		PKG_CLIENT+30
	errortext(ERR_CLUSTER_NOMATES,		"No cluster mates found")
#define ERR_NO_CS_SUPPORT		 		PKG_CLIENT+31
	errortext(ERR_NO_CS_SUPPORT,		"The server doesn't support schedule free time lookups.")

#define ERR_REACHED_WRONG_SERVER		PKG_CLIENT+32
	errortext(ERR_REACHED_WRONG_SERVER, "Connection denied. You requested server %A but connected to %A.")
#define ERR_BAD_SUMMARY_BUFFER			PKG_CLIENT+33
	errortext(ERR_BAD_SUMMARY_BUFFER,  "Bad Summary buffer received from server.")
#define ERR_EXPANDED_LENGTH				PKG_CLIENT+34
	errortext(ERR_EXPANDED_LENGTH,		"Expanded length of data bigger than MAXWORD")

#define ERR_CREATE_SERVER_OBJECT		PKG_CLIENT+35
	errortext(ERR_CREATE_SERVER_OBJECT, "Unable to find/create Notes Server Object in NDS")

#define ERR_PORT_INFO_CHANGED			PKG_CLIENT+36
	errortext(ERR_PORT_INFO_CHANGED, 	"Port information has changed. Please retry")

#define ERR_NO_EDS						PKG_CLIENT+37
	errortext(ERR_NO_EDS, 				"You cannot administer Enterprise Directories on a pre-R5 server")

#define ERR_NO_REMOTE_QUEUE_PUT			PKG_CLIENT+38
	errortext(ERR_NO_REMOTE_QUEUE_PUT, 	"You cannot do a remote queue put to a pre-R5 server")

#define ERR_NO_ACCEL_ON_ENCRYPTED_DB			PKG_CLIENT+39
	errortext(ERR_NO_ACCEL_ON_ENCRYPTED_DB, 	"Accelerated replica creation cannot be used with an encrypted database")

#define ERR_MSG_TO_LARGE				PKG_CLIENT+40
	errortext(ERR_MSG_TO_LARGE,		"Network protocol error: message from server is too large")

#define ERR_LOOKUP_EXTENDED				PKG_CLIENT+41
	errortext(ERR_LOOKUP_EXTENDED,		"LookupExtended on server %a")

#define ERR_XACL_NOUPDATE				PKG_CLIENT+42
	errortext(ERR_XACL_NOUPDATE,		"Extended access controls are enabled on this database.  You must modify the database on a version 6 or later Domino server.")

#define ERR_ROAMING_UPGRADE				PKG_CLIENT+43
	errortext(ERR_ROAMING_UPGRADE,		"Upgrade to Roaming User error")


#define ERR_NSFSEARCHSTART_LOOP			PKG_CLIENT+44
	errortext(ERR_NSFSEARCHSTART_LOOP, "Cannot close a database within an NSFSearchStart - NSFSearchStop loop that was opened outside of the loop")

#define ERR_NOTE_LOCK_NOT_SUPPORTED			PKG_CLIENT+45
	errortext(ERR_NOTE_LOCK_NOT_SUPPORTED, "Design or Data Locking is only supported on a server running Domino 6 or later")

#define STR_ERROR_CONNECTING			PKG_CLIENT+46
	stringtext(STR_ERROR_CONNECTING,	"Error connecting to server %a: %e")

	/* NOTE:	PKG_CLIENT+47 is the LAST code available for this file.  See */
/* globerr.h for dependency which restricts this string ID space. */

#endif

