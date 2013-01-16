#ifndef SERVER_ERR_DEFS
#define SERVER_ERR_DEFS


/*  Server error definitions */

#define ERR_NO_DIR_ACCESS			PKG_SERVER+0
	stringtext(ERR_NO_DIR_ACCESS,	"You are not authorized to access the specified directory")
#define ERR_CMD_SHOW_NLCACHE 		PKG_SERVER+1
	stringtext(ERR_CMD_SHOW_NLCACHE,"NLCACHE")
#define ERR_HELP_SHOW_NLCACHE 		PKG_SERVER+2
	stringtext(ERR_HELP_SHOW_NLCACHE,"* Dump the NAMELookup cache")
#define ERR_NO_SERVER_ACCESS_PORT	PKG_SERVER+3
	errortext(ERR_NO_SERVER_ACCESS_PORT,"You are not authorized to use the server on this port")
#define ERR_ABSOLUTE_PATH			PKG_SERVER+4
	errortext(ERR_ABSOLUTE_PATH,	"Pathname cannot be an absolute pathname")
#define ERR_CLOSE_SESSION			PKG_SERVER+5
	internaltext(ERR_CLOSE_SESSION,	"(Close session)")
#define ERR_CL_MSG_TOO_SMALL		PKG_SERVER+6
	errortext(ERR_CL_MSG_TOO_SMALL,
		"A transient network error or network adapter failure has truncated a message from a client.  If it recurs, run the appropriate adapter diagnostics.")
#define	ERR_CONSOLE_MEM_STATS		PKG_SERVER+7
	stringtext(ERR_CONSOLE_MEM_STATS,"Mem Allocations:   %u%n    Deallocations: %u%n    Usage:         %u%n    Compactions:   %u%n    Failures:      %u%n    Swapouts:      %u%n    Swapins:       %u%n    Reallocsup:    %u%n    Reallocs:      %u%n%n")
#define ERR_PROTOCOL				PKG_SERVER+8
	errortext(ERR_PROTOCOL,
		"This message does not conform to server's protocol requirements.  Client and server may be running incompatible versions of the product")
#define ERR_AMBIGUOUS				PKG_SERVER+9
	errortext(ERR_AMBIGUOUS,		"Command or option is ambiguous")
#define ERR_UNKNOWN_SVCTYPE			PKG_SERVER+10
	errortext(ERR_UNKNOWN_SVCTYPE,	"Service not available (unknown service type)")
#define ERR_VALUE					PKG_SERVER+11
	errortext(ERR_VALUE,			"Invalid value for command option")
#define ERR_VERSION_MISMATCH		PKG_SERVER+12
	errortext(ERR_VERSION_MISMATCH,	
		"Server and client are running different versions of the product.  Install the same version on all communicating servers and workstations.")
#define ERR_SRV_NO_SUCH_TASK			PKG_SERVER+13
	internaltext(ERR_SRV_NO_SUCH_TASK,	"No such task in server")
#define ERR_INV_PRIORITY			PKG_SERVER+14
	internaltext(ERR_INV_PRIORITY,		"Invalid task priority")
#define ERR_INV_SCHED_STATE			PKG_SERVER+15
	internaltext(ERR_INV_SCHED_STATE,	"Invalid task scheduler state")
#define ERR_VAR_BLOCK_ALLOC			PKG_SERVER+16
	internaltext(ERR_VAR_BLOCK_ALLOC,	"Variables block already allocated")
#define ERR_INSUFF_ARGUMENTS		PKG_SERVER+17
	errortext(ERR_INSUFF_ARGUMENTS,		"Insufficient arguments.  Enter 'HELP' for the correct syntax.")
#define ERR_RUNNING_TASK			PKG_SERVER+18
	errortext(ERR_RUNNING_TASK,		"Can't perform this operation on running task")
#define ERR_NOMATCH					PKG_SERVER+19
	errortext(ERR_NOMATCH,			"Command or option is not recognized")
#define ERR_NO_MORE					PKG_SERVER+20
	internaltext(ERR_NO_MORE,		"No more items")
#define ERR_NEED_SEND_BFR			PKG_SERVER+21
	internaltext(ERR_NEED_SEND_BFR,	"Need send buffer")
#define ERR_NEED_RCV_BFR			PKG_SERVER+22
	internaltext(ERR_NEED_RCV_BFR,	"Need receive buffer")
#define ERR_NO_OPEN_DB				PKG_SERVER+23
	errortext(ERR_NO_OPEN_DB,		"No such database open on server")
#define ERR_FUNC_NOT_IMPL			PKG_SERVER+24
	errortext(ERR_FUNC_NOT_IMPL,	
		"The requested function is not supported by the software on this server.")
#define ERR_BFR_MGMT				PKG_SERVER+25
	errortext(ERR_BFR_MGMT,			"Error in server buffer management")
#define ERR_PROGRAM_DIR				PKG_SERVER+26
	errortext(ERR_PROGRAM_DIR,		"Data Directory cannot be accessed.  Check the path given in the DIRECTORY= line in your NOTES.INI file.")
#define ERR_TASK_REMAINING			PKG_SERVER+27
	stringtext(ERR_TASK_REMAINING,	"Waiting for processes to exit")
#define	ERR_SVC_IN_USE				PKG_SERVER+28
	errortext(ERR_SVC_IN_USE,		"Service type already in use")
#define	ERR_FT_TOO_BUSY				PKG_SERVER+29
	errortext(ERR_FT_TOO_BUSY,		"The server's full text indexer is too busy right now to accept this request")
#define ERR_NO_OPEN_COLLECTION		PKG_SERVER+30
	errortext(ERR_NO_OPEN_COLLECTION,"No such collection open on server")
#define	ERR_SUBPGM_TERM				PKG_SERVER+31
	internaltext(ERR_SUBPGM_TERM,	"Subprogram terminated")
#define	ERR_WAIT_FOR_RQST			PKG_SERVER+32
	internaltext(ERR_WAIT_FOR_RQST,	"(Waiting for request)")
#define ERR_TASK_DESC_UBM_CLEANER_THREAD	PKG_SERVER+33
	stringtext(ERR_TASK_DESC_UBM_CLEANER_THREAD,"Buffer Manager Cleaner Thread")
#define	ERR_CONNECT_TO_SERVER		PKG_SERVER+34
	stringtext(ERR_CONNECT_TO_SERVER, "Connect to server")
#define	ERR_OPEN_SESSION			PKG_SERVER+35
	stringtext(ERR_OPEN_SESSION,		 "Open session")
#define	ERR_WAIT_EXTERNAL_EVENT		PKG_SERVER+36
	internaltext(ERR_WAIT_EXTERNAL_EVENT,	"Wait for external event to complete")
#define ERR_SAME_LICENSE			PKG_SERVER+37
	errortext(ERR_SAME_LICENSE,		"Requesting system's ID is the same as the server's ID.  You cannot use the same ID on two systems.")
#define ERR_COMPLETED				PKG_SERVER+38
	internaltext(ERR_COMPLETED,		"(function completed, no error)")
#define ERR_YIELD_NEED_SEND_BFR		PKG_SERVER+39
	internaltext(ERR_YIELD_NEED_SEND_BFR,	"(Yield and need send buffer)")
#define ERR_LICENSE_INVALID			PKG_SERVER+40
	errortext(ERR_LICENSE_INVALID,	"Workstation's license is invalid or unknown")
#define ERR_LICENSE_IN_USE			PKG_SERVER+41
	errortext(ERR_LICENSE_IN_USE,	"Your license is in use by another user")
#define ERR_TASK_NOT_FOUND			PKG_SERVER+42
	errortext(ERR_TASK_NOT_FOUND,	"Server Task executable file was not found.  Check that it exists in your program directory.")
#define ERR_CANNOT_MULTISERVER		PKG_SERVER+43
	errortext(ERR_CANNOT_MULTISERVER, "At least one network port requires the Domino Server to be restricted to a single process, and that process has exceeded the maximum number of threads.")
#define ERR_TASK_RUNNING			PKG_SERVER+44
	errortext(ERR_TASK_RUNNING,		"Task is already running")
#define ERR_HELP_SHOW_MSM 			PKG_SERVER+45
	stringtext(ERR_HELP_SHOW_MSM,"* Dump Mail Storage Manager information ")
#define ERR_NO_SERVER_ACCESS		PKG_SERVER+46
	errortext(ERR_NO_SERVER_ACCESS,	"You are not authorized to use the server")
#define ERR_SERVER_RUNNING			PKG_SERVER+47
	errortext(ERR_SERVER_RUNNING,	"Server is already running (in another process)")
#define ERR_STARTING_SERVER			PKG_SERVER+48
	errortext(ERR_STARTING_SERVER,	"Server exiting:")
#define ERR_WAIT_NETIO				PKG_SERVER+49
	internaltext(ERR_WAIT_NETIO,	"(wait for network I/O)")
#define ERR_FINAL_WAIT_NETIO		PKG_SERVER+50
	internaltext(ERR_FINAL_WAIT_NETIO,	"(wait for final network I/O)")
#define ERR_FUNCTION_COMPLETE		PKG_SERVER+51
	internaltext(ERR_FUNCTION_COMPLETE,	"(function is complete)")
#define ERR_PARTITIONSERVER_RUNNING         PKG_SERVER+52
	errortext(ERR_PARTITIONSERVER_RUNNING,  "Partition %s is already in use.")
#define ERR_SERVER_BUG				PKG_SERVER+53
	errortext(ERR_SERVER_BUG,		"Internal software problem.  Call Customer Support.")
#define	ERR_TASKS_WAITING_STATS		PKG_SERVER+54
	stringtext(ERR_TASKS_WAITING_STATS,"Waiting Tasks:          %lu%n")
#define ERR_TITLE					PKG_SERVER+55
	stringtext(ERR_TITLE,			"Lotus Domino Server")
#define ERR_NO_SERVER_NAME			PKG_SERVER+56
	errortext(ERR_NO_SERVER_NAME,	"The Domino Directory does not contain a server document for this server")
#define ERR_NO_NETWORK_NAME			PKG_SERVER+57
	errortext(ERR_NO_NETWORK_NAME,	"Server document in Domino Directory does not contain a network name (or one for this port)")
#define ERR_FIXUP_BEGIN				PKG_SERVER+58
	errortext(ERR_FIXUP_BEGIN,		"Begin scan of databases to be consistency checked")
#define ERR_FIXUP_END				PKG_SERVER+59
	errortext(ERR_FIXUP_END,		"End scan of databases: %d found")
#define ERR_TOO_MANY_TASKS_DE		PKG_SERVER+60
	errortext(ERR_TOO_MANY_TASKS_DE,"Too many users for the Demo and Evaluation version of the server")
#define ERR_CMD_LINE_TOO_LONG		PKG_SERVER+61
	errortext(ERR_CMD_LINE_TOO_LONG,"The command line is too long")
#define	ERR_TASK_DESC_POLL			PKG_SERVER+62
	stringtext(ERR_TASK_DESC_POLL,	"Perform housekeeping chores")
#define ERR_FIXUP_ERR				PKG_SERVER+63
	errortext(ERR_FIXUP_ERR,		"Error checking database %p")
#define	ERR_HELP_DEBUG_BOOST		PKG_SERVER+64
	stringtext(ERR_HELP_DEBUG_BOOST,"* Server priority boost interval")
#define ERR_TASK_DESC_UBM_IO_THREAD	PKG_SERVER+65
	stringtext(ERR_TASK_DESC_UBM_IO_THREAD,"Buffer Manager I/O Thread")
#define	ERR_HELP_DEBUG_PRIORITY		PKG_SERVER+66
	stringtext(ERR_HELP_DEBUG_PRIORITY,"* Server priority (low/high)")
#define	ERR_HELP_DEBUG_SCHEDULING	PKG_SERVER+67
	stringtext(ERR_HELP_DEBUG_SCHEDULING,"* Task scheduling")
#define	ERR_HELP_DEBUG_TASK			PKG_SERVER+68
	stringtext(ERR_HELP_DEBUG_TASK,"* Task latency threshold")
#define	ERR_HELP_DEBUG_TRANS	 	PKG_SERVER+69
	stringtext(ERR_HELP_DEBUG_TRANS,"* Transaction log")
#define	ERR_HELP_DEBUG_JOB			PKG_SERVER+70
	stringtext(ERR_HELP_DEBUG_JOB,"* Load job schedule database")
#define	ERR_HELP_SET_CONFIG			PKG_SERVER+71
	stringtext(ERR_HELP_SET_CONFIG,"Configuration variable")
#define	ERR_HELP_SET_DEBUG			PKG_SERVER+72
	stringtext(ERR_HELP_SET_DEBUG,"* Debug variable:")
#define ERR_TASK_STATUS_PARTITIONSERVER     PKG_SERVER+73
	stringtext(ERR_TASK_STATUS_PARTITIONSERVER,"Partition:              %s%n")
#define	ERR_HELP_SHOW_CONFIG		PKG_SERVER+74
	stringtext(ERR_HELP_SHOW_CONFIG,"Configuration variable")
#define	ERR_HELP_SHOW_MEMORY		PKG_SERVER+75
	stringtext(ERR_HELP_SHOW_MEMORY,"Memory information")
#define	ERR_HELP_SHOW_PORT			PKG_SERVER+76
	stringtext(ERR_HELP_SHOW_PORT,"Port specific information")
#define	ERR_HELP_SHOW_TASKS			PKG_SERVER+77
	stringtext(ERR_HELP_SHOW_TASKS,"Server tasks")
#define	ERR_HELP_SHOW_TRANS			PKG_SERVER+78
	stringtext(ERR_HELP_SHOW_TRANS,"* Transaction statistics")
#define	ERR_HELP_SHOW_USERS			PKG_SERVER+79
	stringtext(ERR_HELP_SHOW_USERS,"Users with open sessions")
#define	ERR_CMD_CMD_CLUSTER		 	PKG_SERVER+80
	stringtext(ERR_CMD_CMD_CLUSTER,"CLUSTER")
#define	ERR_HELP_SHOW_DISK			PKG_SERVER+81
	stringtext(ERR_HELP_SHOW_DISK,"Available disk space")
#define	ERR_HELP_SHOW_THREADS		PKG_SERVER+82
	stringtext(ERR_HELP_SHOW_THREADS,"* Threads")
#define	ERR_HELP_SHOW_SEM			PKG_SERVER+83
	stringtext(ERR_HELP_SHOW_SEM,"* Sem")
#define	ERR_HELP_SHOW_SESSIONS		PKG_SERVER+84
	stringtext(ERR_HELP_SHOW_SESSIONS,"* Sessions")
#define	ERR_HELP_CMD_BROADCAST		PKG_SERVER+85
	stringtext(ERR_HELP_CMD_BROADCAST,"Broadcast a message to user(s)")
#define	ERR_HELP_CMD_EXIT			PKG_SERVER+86
	stringtext(ERR_HELP_CMD_EXIT,"Exit server")
#define	ERR_HELP_CMD_HELP			PKG_SERVER+87
	stringtext(ERR_HELP_CMD_HELP,"Help (Displays this help information)")
#define	ERR_HELP_CMD_LOAD			PKG_SERVER+88
	stringtext(ERR_HELP_CMD_LOAD,"Load program")
#define	ERR_HELP_CMD_QUIT			PKG_SERVER+89
	stringtext(ERR_HELP_CMD_QUIT,"Quit (exit server)")
#define	ERR_HELP_CMD_REPLICATE		PKG_SERVER+90
	stringtext(ERR_HELP_CMD_REPLICATE,"Replicate two-way request")
#define	ERR_HELP_CMD_PULL			PKG_SERVER+91
	stringtext(ERR_HELP_CMD_PULL,"Replicate one-way (pull)")
#define	ERR_HELP_CMD_ROUTE			PKG_SERVER+92
	stringtext(ERR_HELP_CMD_ROUTE,"Route mail to server")
#define	ERR_HELP_CMD_SET			PKG_SERVER+93
	stringtext(ERR_HELP_CMD_SET,"Set server option:")
#define	ERR_HELP_CMD_SHOW			PKG_SERVER+94
	stringtext(ERR_HELP_CMD_SHOW,"Show server information:")
#define	ERR_HELP_CMD_TELL			PKG_SERVER+95
	stringtext(ERR_HELP_CMD_TELL,"Send command-string to a task")
#define	ERR_TASK_DESC_COMMANDS		PKG_SERVER+96
	stringtext(ERR_TASK_DESC_COMMANDS,"Perform console commands")
#define	ERR_CONSOLE_NO_REPLICATOR	PKG_SERVER+97
	errortext(ERR_CONSOLE_NO_REPLICATOR,"Replicator task not running%n")
#define	ERR_CONSOLE_NO_ROUTER		PKG_SERVER+98
	errortext(ERR_CONSOLE_NO_ROUTER,"Router task not running%n")
#define	ERR_CONSOLE_CANT_TELL		PKG_SERVER+99
#ifdef MAC
	errortext(ERR_CONSOLE_CANT_TELL,"The %s task either does not exist or does not accept \042Tell\042 commands.%n")
#else
	errortext(ERR_CONSOLE_CANT_TELL,"The %s task either does not exist or does not accept ""Tell"" commands.%n")
#endif
#define ERR_CMD_SHOW_MSM 			PKG_SERVER+100
	stringtext(ERR_CMD_SHOW_MSM,"MSM")
#define	ERR_CONSOLE_DS_MESS			PKG_SERVER+101
#ifdef NLM
	stringtext(ERR_CONSOLE_DS_MESS,"Available disk space %s: %s bytes%n")
#else
	stringtext(ERR_CONSOLE_DS_MESS,"Available disk space %s bytes%n")
#endif
#define	ERR_CONSOLE_BAD_NO_OF_PARMS	PKG_SERVER+102
	errortext(ERR_CONSOLE_BAD_NO_OF_PARMS,"Wrong number of parameters%n")
#define	ERR_CONSOLE_SESSION_HEADER	PKG_SERVER+103
	stringtext(ERR_CONSOLE_SESSION_HEADER,"ThreadID SessionID ProcessID  Use-Count MaxBufSize Port  Service  State%n")
#define	ERR_CONSOLE_MEM_AVAIL		PKG_SERVER+104
	stringtext(ERR_CONSOLE_MEM_AVAIL,"Memory Available: %s Mbytes%n")
#define	ERR_CONSOLE_DISK_AVAIL		PKG_SERVER+105
	stringtext(ERR_CONSOLE_DISK_AVAIL,"Memory Available (including virtual): %s Mbytes%n")
#define	ERR_TASK_DESC_FLUSH			PKG_SERVER+106
	stringtext(ERR_TASK_DESC_FLUSH,"Flush disk buffers every 60 secs.")
#define	ERR_TASK_DESC_LISTEN		PKG_SERVER+107
	stringtext(ERR_TASK_DESC_LISTEN,"Listen for connect requests on %s")
#define	ERR_TASK_DESC_SERVER		PKG_SERVER+108
	stringtext(ERR_TASK_DESC_SERVER,"Server for session %lx on %s")
#define	ERR_TASK_DESC_IDLE			PKG_SERVER+109
	stringtext(ERR_TASK_DESC_IDLE,"Idle task")
#define	ERR_INIT_CANT_LOAD			PKG_SERVER+110
	errortext(ERR_INIT_CANT_LOAD,"Error attempting to load or run %s:%n    %e%n")
#define	ERR_NUM_MAILBOXES			PKG_SERVER+111
	stringtext(ERR_NUM_MAILBOXES,	"Number of Mailboxes:    %lu%n")
#define	ERR_TASK_DESC_SUSP_NAMESERVER	PKG_SERVER+112
	stringtext(ERR_TASK_DESC_SUSP_NAMESERVER,"Suspended NetBIOS name server for %s")
#define	ERR_TASK_DESC_NAME			PKG_SERVER+113
	stringtext(ERR_TASK_DESC_NAME,"NetBIOS name server for %s")
#define	ERR_DEFAULT_SRV_NAME		PKG_SERVER+114
	stringtext(ERR_DEFAULT_SRV_NAME,"NetBIOS name server")
#define	ERR_TASK_DESC_DATABASE_DETAIL	PKG_SERVER+115
	stringtext(ERR_TASK_DESC_DATABASE_DETAIL,"%a on %s in %s")
#define	ERR_SRV_SESSION_UNKNOWN			PKG_SERVER+116
	stringtext(ERR_SRV_SESSION_UNKNOWN,"Unknown")
#define	ERR_SRV_SESSION_CREATED			PKG_SERVER+117
	stringtext(ERR_SRV_SESSION_CREATED,"Created")
#define	ERR_SRV_SESSION_CONNECTED	   	PKG_SERVER+118
	stringtext(ERR_SRV_SESSION_CONNECTED,"Connected")
#define	ERR_SRV_SESSION_OPEN			PKG_SERVER+119
	stringtext(ERR_SRV_SESSION_OPEN,"Open")
#define	ERR_SRV_SESSION_DISCONNECTED	PKG_SERVER+120
	stringtext(ERR_SRV_SESSION_DISCONNECTED,"Disconnected")
#define	ERR_SRV_SESSION_CLOSED			PKG_SERVER+121
	stringtext(ERR_SRV_SESSION_CLOSED,"Closed")
#define	ERR_TASK_STATUS_SUMMARY		PKG_SERVER+122
	stringtext(ERR_TASK_STATUS_SUMMARY,"%n%s (%s for %s) %s%n%n")
#define	ERR_TASK_STATUS_DIRECT		PKG_SERVER+123
	stringtext(ERR_TASK_STATUS_DIRECT,"Server directory:       %s%n")
#define	ERR_TASK_STATUS_TRANS		PKG_SERVER+124
#ifndef LINUX
	stringtext(ERR_TASK_STATUS_TRANS,  "Transactions: %-8lu  Max. concurrent: %d%n")
#else
	stringtext(ERR_TASK_STATUS_TRANS,  "Transactions:           %lu%n")
#endif
#define	ERR_TASK_STATUS_DAY_PLURAL	PKG_SERVER+125
	stringtext(ERR_TASK_STATUS_DAY_PLURAL,"%u days ")
#define	ERR_TASK_STATUS_DAY_SINGULAR	PKG_SERVER+126
	stringtext(ERR_TASK_STATUS_DAY_SINGULAR,"%u day ")
#define	ERR_TASK_STATUS_ELAPSED		PKG_SERVER+127
	stringtext(ERR_TASK_STATUS_ELAPSED,"Elapsed time:           %s%02u:%02u:%02u%n")
#define	ERR_TASK_STATUS_HEADING		PKG_SERVER+128
	stringtext(ERR_TASK_STATUS_HEADING,"%n      Task                 Description%n%n")
#define	ERR_TASK_STATUS_DBASE_SERVER	PKG_SERVER+129
	stringtext(ERR_TASK_STATUS_DBASE_SERVER,"Database Server")
#define	ERR_USER_STATUS_HEADING		PKG_SERVER+130
	stringtext(ERR_USER_STATUS_HEADING,"  User Name          Databases Open       Minutes Since Last Used%n%n")
#define	ERR_TASK_STATUS_SERVER_NAME	PKG_SERVER+131
	stringtext(ERR_TASK_STATUS_SERVER_NAME,"Server name:")
#define ERR_CMD_DEBUG_BOOST			PKG_SERVER+132
	stringtext(ERR_CMD_DEBUG_BOOST,"BOOST interval")
#define ERR_CMD_DEBUG_IDLE			PKG_SERVER+133
	stringtext(ERR_CMD_DEBUG_IDLE,"IDLE_WAIT waittime")
#define ERR_CMD_DEBUG_PRIORITY		PKG_SERVER+134
	stringtext(ERR_CMD_DEBUG_PRIORITY,"PRIORITY [L/H]")
#define ERR_CMD_DEBUG_SCHEDULING	PKG_SERVER+135
	stringtext(ERR_CMD_DEBUG_SCHEDULING,"SCHEDULING [0/1]")
#define ERR_CMD_DEBUG_TASK			PKG_SERVER+136
	stringtext(ERR_CMD_DEBUG_TASK,"TASK_LATENCY latency")
#define ERR_CMD_DEBUG_TRANS			PKG_SERVER+137
	stringtext(ERR_CMD_DEBUG_TRANS,"TRANSACTIONS [0/1]")
#define ERR_CMD_DEBUG_JOB			PKG_SERVER+138
	stringtext(ERR_CMD_DEBUG_JOB,"JOB [0/1]")
#define ERR_CMD_SET_CONFIG			PKG_SERVER+139
#ifdef MAC
	stringtext(ERR_CMD_SET_CONFIG,"CONFIGURATION \042variable=value\042 [update/noupdate]")
#else
	stringtext(ERR_CMD_SET_CONFIG,"CONFIGURATION ""variable=value"" [update/noupdate]")
#endif
#define ERR_CMD_SET_DEBUG			PKG_SERVER+140
	stringtext(ERR_CMD_SET_DEBUG,"DEBUG")
#define ERR_CLUSTER_INSERVICE			PKG_SERVER+141
	stringtext(ERR_CLUSTER_INSERVICE,"INSERVICE database_name")
#define ERR_CMD_SHOW_CONFIG			PKG_SERVER+142
	stringtext(ERR_CMD_SHOW_CONFIG,"CONFIGURATION variable")
#define ERR_CMD_SHOW_MEMORY			PKG_SERVER+143
	stringtext(ERR_CMD_SHOW_MEMORY,"MEMORY")
#define ERR_CMD_SHOW_PORT			PKG_SERVER+144
	stringtext(ERR_CMD_SHOW_PORT,"PORT portname")
#define ERR_CMD_SHOW_TASKS			PKG_SERVER+145
	stringtext(ERR_CMD_SHOW_TASKS,"TASKS [only/user/all]")
#define ERR_CMD_SHOW_TRANS			PKG_SERVER+146
	stringtext(ERR_CMD_SHOW_TRANS,"TRANSACTIONS")
#define ERR_CMD_SHOW_USERS			PKG_SERVER+147
	stringtext(ERR_CMD_SHOW_USERS,"USERS")
#define	ERR_HELP_CLUSTER_INSERVICE	 	PKG_SERVER+148
	stringtext(ERR_HELP_CLUSTER_INSERVICE,"* Mark Database In Service")
#define ERR_CMD_SHOW_DISK			PKG_SERVER+149
#ifdef UNIX
	stringtext(ERR_CMD_SHOW_DISK,"DISKSPACE filesystem")
#elif defined (NLM)
	stringtext(ERR_CMD_SHOW_DISK,"DISKSPACE volume")
#else
	stringtext(ERR_CMD_SHOW_DISK,"DISKSPACE drive-letter")
#endif
#define ERR_CMD_SHOW_THREADS		PKG_SERVER+150
	stringtext(ERR_CMD_SHOW_THREADS,"THREADS")
#define ERR_CMD_SHOW_SEM			PKG_SERVER+151
	stringtext(ERR_CMD_SHOW_SEM,"SEM")
#define ERR_CMD_SHOW_SESSIONS		PKG_SERVER+152
	stringtext(ERR_CMD_SHOW_SESSIONS,"SESSIONS [Service / index seqno]")
#define ERR_CMD_CMD_BROADCAST		PKG_SERVER+153
#ifdef MAC
	stringtext(ERR_CMD_CMD_BROADCAST,"BROADCAST \042msg\042 [\042user/database\042]")
#else
	stringtext(ERR_CMD_CMD_BROADCAST,"BROADCAST ""msg"" [""user/database""]")
#endif
#define ERR_CMD_CMD_EXIT			PKG_SERVER+154
	stringtext(ERR_CMD_CMD_EXIT,"EXIT [password]")
#define ERR_CMD_CMD_HELP			PKG_SERVER+155
	stringtext(ERR_CMD_CMD_HELP,"HELP")
#define ERR_CMD_CMD_LOAD			PKG_SERVER+156
	stringtext(ERR_CMD_CMD_LOAD,"LOAD pgmname")
#define ERR_CMD_CMD_QUIT			PKG_SERVER+157
	stringtext(ERR_CMD_CMD_QUIT,"QUIT [password]")
#define ERR_CMD_CMD_REPLICATE		PKG_SERVER+158
	stringtext(ERR_CMD_CMD_REPLICATE,"REPLICATE server [database]")
#define ERR_CMD_CMD_PULL			PKG_SERVER+159
	stringtext(ERR_CMD_CMD_PULL,"PULL server [database]")
#define ERR_CMD_CMD_ROUTE			PKG_SERVER+160
	stringtext(ERR_CMD_CMD_ROUTE,"ROUTE servername")
#define ERR_CMD_CMD_SET				PKG_SERVER+161
	stringtext(ERR_CMD_CMD_SET,"SET")
#define ERR_CMD_CMD_SHOW			PKG_SERVER+162
	stringtext(ERR_CMD_CMD_SHOW,"SHOW")
#define ERR_CMD_CMD_TELL			PKG_SERVER+163
	stringtext(ERR_CMD_CMD_TELL,"TELL taskname command-string")
#define ERR_TOO_MANY_ARGUMENTS		PKG_SERVER+164
	errortext(ERR_TOO_MANY_ARGUMENTS, "Too many arguments.  Enter 'HELP' for the correct syntax.")
#define ERR_CMD_SHOW_STAT			PKG_SERVER+165
	stringtext(ERR_CMD_SHOW_STAT,"STATISTIC variable")
#define	ERR_HELP_SHOW_STAT			PKG_SERVER+166
	stringtext(ERR_HELP_SHOW_STAT,"Statistic variable")
#define	ERR_TASK_STATUS_TPM			PKG_SERVER+167
	stringtext(ERR_TASK_STATUS_TPM,	"Transactions/minute:    Last minute: %u; Last hour: %u; Peak: %u%n")
#define	ERR_TASK_PEAK_SESSIONS		PKG_SERVER+168
	stringtext(ERR_TASK_PEAK_SESSIONS,"Peak # of sessions:     %u at %s%n")
#define ERR_MAIL_STATS				PKG_SERVER+169
	stringtext(ERR_MAIL_STATS,		 "Pending mail: %-5lu     Dead mail: %-5lu%n")
#define ERR_CMD_SHOW_PERF			PKG_SERVER+170
	stringtext(ERR_CMD_SHOW_PERF,"PERFORMANCE")
#define	ERR_HELP_SHOW_PERF			PKG_SERVER+171
	stringtext(ERR_HELP_SHOW_PERF,"* Toggle display of server performance on/off")
#define	ERR_MSG_PERF_ENABLED		PKG_SERVER+172
	stringtext(ERR_MSG_PERF_ENABLED,	"Server Performance Monitoring is now enabled.")
#define	ERR_MSG_PERF_DISABLED		PKG_SERVER+173
	stringtext(ERR_MSG_PERF_DISABLED,"Server Performance Monitoring is now disabled.")
#define	ERR_MSG_PERF				PKG_SERVER+174
	stringtext(ERR_MSG_PERF,		"%s%d Transactions/Minute, %d Notes Users")
#define ERR_SERVER_NOT_AUTH			PKG_SERVER+175
	errortext(ERR_SERVER_NOT_AUTH, 	"You are not authorized to perform this function on this server")
#define ERR_SERVER_NOT_CONFIG		PKG_SERVER+176
	errortext(ERR_SERVER_NOT_CONFIG,"This server is not configured for that function")
#define ERR_NO_REMOTE_CONSOLE_ACCESS PKG_SERVER+177
	errortext(ERR_NO_REMOTE_CONSOLE_ACCESS,"You are not authorized to use the remote console on this server")
#define ERR_SERVER_NOT_RESP			PKG_SERVER+178
	errortext(ERR_SERVER_NOT_RESP,	"Server is not responding to remote console commands (try again later)")
#define ERR_SERVER_NO_RESPONSE		PKG_SERVER+179
	errortext(ERR_SERVER_NO_RESPONSE,"No response from server for this command")
#define ERR_CON_PASS				PKG_SERVER+180
	errortext(ERR_CON_PASS,		"Incorrect console security password")
#define	ERR_HELP_SET_SECURE			PKG_SERVER+181
	stringtext(ERR_HELP_SET_SECURE,"Secure Console Password")
#define ERR_CMD_SET_SECURE			PKG_SERVER+182
	stringtext(ERR_CMD_SET_SECURE,"SECURE [current-password] [new-password]")
#define ERR_CMD_SECURE				PKG_SERVER+183
	stringtext(ERR_CMD_SECURE,		"This command is not permitted when console security is in effect")
#define ERR_CLUSTER_OUTOFSERVICE	PKG_SERVER+184
	stringtext(ERR_CLUSTER_OUTOFSERVICE,"OUTOFSERVICE database_name")
#define	ERR_HELP_CLUSTER_OUTOFSERVICE	PKG_SERVER+185
	stringtext(ERR_HELP_CLUSTER_OUTOFSERVICE,"* Mark Database Out Of Service")
#define ERR_NO_NA_PORT_NAME			PKG_SERVER+186
	errortext(ERR_NO_NA_PORT_NAME,	"Server entry in Name & Address database does not contain any information for port %s")
#define ERR_YIELD_NEED_INUSE_BUFFER	PKG_SERVER+187
	internaltext(ERR_YIELD_NEED_INUSE_BUFFER,	"Need InUse buffer")
#define ERR_NETHEAPSIZE		   		PKG_SERVER+188
	errortext(ERR_NETHEAPSIZE,		"In order to run the Domino Server, you must first use a text editor to edit your Windows SYSTEM.INI file.")
#define ERR_NETHEAPSIZE_0	   		PKG_SERVER+189
	errortext(ERR_NETHEAPSIZE_0,	"In the [386Enh] section, add: NetHeapSize=%d")
#define ERR_NETHEAPSIZE_N	   		PKG_SERVER+190
	errortext(ERR_NETHEAPSIZE_N,	"In the [386Enh] section, change the line reading 'NetHeapSize=%d' to 'NetHeapSize=%d'")
#define ERR_NAMESERVER_TABLES_ERR	PKG_SERVER+191
	errortext(ERR_NAMESERVER_TABLES_ERR, "Error reading server information for name server")
#define ERR_CMD_QUIT_NEEDS_PASSWORD	PKG_SERVER+192
	stringtext(ERR_CMD_QUIT_NEEDS_PASSWORD,	"This server requires a console password. Enter Exit [password] or Quit [password].")
#define ERR_SRV_NO_REPL_REQ			PKG_SERVER+193
	errortext(ERR_SRV_NO_REPL_REQ,	 "Replication requests are disabled")
#define ERR_HELP_CMD_CLUSTER			PKG_SERVER+194
	stringtext(ERR_HELP_CMD_CLUSTER,"* Cluster Administration commands")
#define ERR_USE_LIVE_CONSOLE			PKG_SERVER+195
	stringtext(ERR_USE_LIVE_CONSOLE,"Command has been executed on remote server. Use 'Live' console option, in future, to view response from server.")
#define ERR_SRV_SETUP				PKG_SERVER+196
	errortext(ERR_SRV_SETUP,		"Server not setup")
#define ERR_CMD_CMD_CALL			PKG_SERVER+197
	stringtext(ERR_CMD_CMD_CALL,	"CALL servername portname    ")
#define ERR_NO_PRIV_PASSTHRU_TARGET PKG_SERVER+198
	errortext(ERR_NO_PRIV_PASSTHRU_TARGET, "This server is not permitted to passthru to the specified server")
#define ERR_NO_PRIV_PASSTHRU_CALLING PKG_SERVER+199
	errortext(ERR_NO_PRIV_PASSTHRU_CALLING, "The port is not connected to a server, and you are not allowed to make a call on that port")
#define ERR_NO_PRIV_PASSTHRU_ACCESS PKG_SERVER+200
	errortext(ERR_NO_PRIV_PASSTHRU_ACCESS, "You are not allowed to use passthru to access this server")
#define ERR_AUTHENTICATION_MESSAGE	PKG_SERVER+201
	internaltext(ERR_AUTHENTICATION_MESSAGE, "(authentication message)")
#define ERR_SRV_SETUP_COMPLETE		PKG_SERVER+202
	errortext(ERR_SRV_SETUP_COMPLETE,	"Completing Domino server setup")
#define ERR_SRV_SETUP_DIALING	 	PKG_SERVER+203
	errortext(ERR_SRV_SETUP_DIALING,	"Dialing server ")
#define ERR_SRV_SETUP_GET_ID_ADDR	PKG_SERVER+204
	errortext(ERR_SRV_SETUP_GET_ID_ADDR,"Getting server ID from Domino Directory on ")
#define ERR_SRV_SETUP_REPL_ADDR		PKG_SERVER+205
	errortext(ERR_SRV_SETUP_REPL_ADDR,	"Replicating system databases from ")
#define ERR_SRV_SETUP_REG_ADDR		PKG_SERVER+206
	errortext(ERR_SRV_SETUP_REG_ADDR,	"Creating Domino Directory connection entry on ")
#define ERR_SRV_SETUP_DEL_ID		PKG_SERVER+207
	errortext(ERR_SRV_SETUP_DEL_ID,		"Deleting Server ID from Domino Directory on ")
#define ERR_SRV_SETUP_DEL_ID_OK		PKG_SERVER+208
	errortext(ERR_SRV_SETUP_DEL_ID_OK,	"Deleted Server ID from Domino Directory on ")
#define ERR_SRV_SETUP_DONE			PKG_SERVER+209
	errortext(ERR_SRV_SETUP_DONE,		"Domino server setup completed")
#define ERR_SRV_SETUP_DEL_ID_NOACCESS	PKG_SERVER+210
	errortext(ERR_SRV_SETUP_DEL_ID_NOACCESS,	"Insufficient access to delete Server ID from Domino Directory on ")
#define ERR_SRV_SETUP_DEL_ID_ERROR	PKG_SERVER+211
	errortext(ERR_SRV_SETUP_DEL_ID_ERROR,	"Error while deleting Server ID from Domino Directory on %a.%nERROR: %e%nServer ID NOT deleted from Domino Directory.")
#define ERR_NO_PRIV_PASSTHRU_CLIENT PKG_SERVER+212
	errortext(ERR_NO_PRIV_PASSTHRU_CLIENT, "You are not allowed to route through this server")
#define	ERR_TASK_DESC_PASSTHRU		PKG_SERVER+213
	stringtext(ERR_TASK_DESC_PASSTHRU,	"Passthru server on session %lx for %a")
#define ERR_SVC_CTRL_ERROR	PKG_SERVER+214
	errortext(ERR_SVC_CTRL_ERROR,	"Error during service control setup")
#define ERR_SVC_ACCESS_DENIED		PKG_SERVER+215
	errortext(ERR_SVC_ACCESS_DENIED, "Access denied during service control setup")
#define ERR_SVC_ALREADY_EXISTS		PKG_SERVER+216
	errortext(ERR_SVC_ALREADY_EXISTS, "The Domino Server service already exists in the registry") 
#define ERR_SVC_DOES_NOT_EXIST		PKG_SERVER+217
	errortext(ERR_SVC_DOES_NOT_EXIST, "The Domino Server service does not exist in the registry") 
#define ERR_EXEC_POOLFULL			PKG_SERVER+218
	errortext(ERR_EXEC_POOLFULL,	"Insufficient memory - server executive pool is full.")
#define	ERR_TRANS_INFO_HEADING		PKG_SERVER+219
	stringtext(ERR_TRANS_INFO_HEADING, "Function                       Count     Min     Max      Total    Average%n%n")
#define ERR_CMD_SHOW_SCHEDULE		PKG_SERVER+220
	stringtext(ERR_CMD_SHOW_SCHEDULE,"SCHEDULE")
#define	ERR_HELP_SHOW_SCHEDULE		PKG_SERVER+221
	stringtext(ERR_HELP_SHOW_SCHEDULE,"Next Schedule [Server/Program/Location] [Appl]")
#define	ERR_SERVER_BUSY				PKG_SERVER+222
#define	ERR_SERVER_UNAVAILABLE		ERR_SERVER_BUSY
	stringtext(ERR_SERVER_BUSY, "Access to this server has been restricted due to excessive load")
#define	ERR_SERVER_REPID_NOT_FOUND	PKG_SERVER+223
	stringtext(ERR_SERVER_REPID_NOT_FOUND, "Server has no information for database")
#define	ERR_TASK_DESC_DATABASE_IDLE	PKG_SERVER+224
	stringtext(ERR_TASK_DESC_DATABASE_IDLE,"%a on %s is idle")
#define ERR_CMD_CMD_DROP			PKG_SERVER+225
#ifdef MAC
	stringtext(ERR_CMD_CMD_DROP,"DROP [\042username/database\042] [ALL]")
#else
	stringtext(ERR_CMD_CMD_DROP,"DROP [""username/database""] [ALL]")
#endif
#define	ERR_HELP_CMD_DROP			PKG_SERVER+226
	stringtext(ERR_HELP_CMD_DROP,"Drop one or more sessions")
#define	ERR_HELP_CALL				PKG_SERVER+227
	stringtext(ERR_HELP_CALL,		"*Call the specified server")
#define ERR_REFRESH_SERVER_INFO		PKG_SERVER+228
	stringtext(ERR_REFRESH_SERVER_INFO, "Searching Server document for security lists")
#define ERR_REFRESH_CONFIG_INFO		PKG_SERVER+229
	stringtext(ERR_REFRESH_CONFIG_INFO, "Searching Server Configuration document(s) for parameters...")
#define ERR_CL_LENGTH_MISMATCH	   	PKG_SERVER+230
	stringtext(ERR_CL_LENGTH_MISMATCH,	"Multi-Segment ID table length from client is not the length expected")
#define ERR_CMD_CMD_PUSH			PKG_SERVER+231
	stringtext(ERR_CMD_CMD_PUSH,	"PUSH server [database]")
#define	ERR_HELP_CMD_PUSH			PKG_SERVER+232
	stringtext(ERR_HELP_CMD_PUSH,	"Replicate one-way (push)")
#define ERR_ERROR_CONFIG_INFO		PKG_SERVER+233
	errortext(ERR_ERROR_CONFIG_INFO, "Error Searching Server Configuration document(s)")
#define ERR_CMD_SET_STAT			PKG_SERVER+234
	stringtext(ERR_CMD_SET_STAT,"STAT [Facility] [Statname]")
#define	ERR_HELP_SET_STAT			PKG_SERVER+235
	stringtext(ERR_HELP_SET_STAT,"Reset statistics")
#define	ERR_TASK_DESC_DATABASE		PKG_SERVER+236
	stringtext(ERR_TASK_DESC_DATABASE,"Server for %a on %s")
#define ERR_RECEIVED_DATA			PKG_SERVER+237
	errortext(ERR_RECEIVED_DATA, "Error receiving data from workstation")
#define ERR_SERVERCONFIG_SETINFO	PKG_SERVER+238
	errortext(ERR_SERVERCONFIG_SETINFO, "Error searching Server Configuration document - it will not be updated with '%s'")
#define	ERR_DATABASE_UNAVAILABLE	PKG_SERVER+239
	stringtext(ERR_DATABASE_UNAVAILABLE, "Access to the database has been restricted by the administrator")
#define	ERR_CONSOLE_CLUSTER_HEADER	PKG_SERVER+240
	stringtext(ERR_CONSOLE_CLUSTER_HEADER, "Cluster information:")
#define	ERR_HELP_SHOW_CLUSTER		PKG_SERVER+241
	stringtext(ERR_HELP_SHOW_CLUSTER,"Cluster information")
#define ERR_CMD_SHOW_CLUSTER		PKG_SERVER+242
	stringtext(ERR_CMD_SHOW_CLUSTER,"CLUSTER")
#define ERR_CMD_SHOW_SERVER			PKG_SERVER+243
	stringtext(ERR_CMD_SHOW_SERVER,"SERVER")
#define	ERR_HELP_SHOW_SERVER	 	PKG_SERVER+244
	stringtext(ERR_HELP_SHOW_SERVER,"Server information")
#define ERR_HELP_SHOW_DIRECTORY		PKG_SERVER+245
	stringtext(ERR_HELP_SHOW_DIRECTORY,"File Directory Information")
#define ERR_CMD_SHOW_DIRECTORY		PKG_SERVER+246
	stringtext(ERR_CMD_SHOW_DIRECTORY,"DIRECTORY")
#define	ERR_TASK_DESC_CLUSTER		PKG_SERVER+247
	stringtext(ERR_TASK_DESC_CLUSTER,"Cluster Manager is updating cluster cache")
#define	ERR_TASK_DESC_CLUSTER_IDLE	PKG_SERVER+248
	stringtext(ERR_TASK_DESC_CLUSTER_IDLE,"Cluster Manager is idle")
#define ERR_FIXUP_CHECKING			PKG_SERVER+249
	stringtext(ERR_FIXUP_CHECKING,	"Consistency checking ")
#define ERR_CLUSTER_NOT_CONFIG		PKG_SERVER+250
	errortext(ERR_CLUSTER_NOT_CONFIG,"This server is not configured for a cluster.")
#define ERR_FILENAME_CONFLICT		PKG_SERVER+251
	errortext(ERR_FILENAME_CONFLICT,"The specified file name conflicts with another database which you can not delete")
#define	ERR_SERVER_RESTRICTED		PKG_SERVER+252
	errortext(ERR_SERVER_RESTRICTED,"Access to this server has been restricted by the administrator")
#define ERR_BAD_NTREGISTRY_VALUE	PKG_SERVER+253
	errortext(ERR_BAD_NTREGISTRY_VALUE, "Informational: The registry value HKEY_LOCAL_MACHINE\\%s is currently set to %d. For better server performance, change the setting to %d")
#define ERR_CMD_CMD_HANGUP			PKG_SERVER+254
	stringtext(ERR_CMD_CMD_HANGUP,	"HANGUP portname             ")
#define	ERR_HELP_HANGUP				PKG_SERVER+255
	stringtext(ERR_HELP_HANGUP,		"*Hangup the specified port")

/* Use SERVER2 error code package which goes up to 47. */

#define	ERR_HELP_CMD_RESTART			PKG_SERVER2+0
	stringtext(ERR_HELP_CMD_RESTART,	"Restart information:")
#define	ERR_NO_EVENT_SCHEDULED		PKG_SERVER2+1
	errortext(ERR_NO_EVENT_SCHEDULED,"No events are scheduled at this time.%n%n")
#define ERR_FIXUP_DELAY				PKG_SERVER2+2
	errortext(ERR_FIXUP_DELAY,		"Consistency check will start in %d seconds")
#define ERR_CLUSTER_ADD			PKG_SERVER2+3
	stringtext(ERR_CLUSTER_ADD,	"ADD cluster_name [overwrite]")
#define	ERR_HELP_CLUSTER_ADD	 	PKG_SERVER2+4
	stringtext(ERR_HELP_CLUSTER_ADD,"* Add server to cluster")
#define ERR_CLUSTER_REMOVE		PKG_SERVER2+5
	stringtext(ERR_CLUSTER_REMOVE,	"REMOVE")
#define	ERR_HELP_CLUSTER_REMOVE	 	PKG_SERVER2+6
	stringtext(ERR_HELP_CLUSTER_REMOVE,"* Remove server from cluster")
#define ERR_CLUSTER_COPY		PKG_SERVER2+7
	stringtext(ERR_CLUSTER_COPY,	"COPY source target [options]")
#define	ERR_HELP_CLUSTER_COPY	 	PKG_SERVER2+8
	stringtext(ERR_HELP_CLUSTER_COPY,"* Make a copy of a database")
#define ERR_CLUSTER_DELETE			PKG_SERVER2+9
	stringtext(ERR_CLUSTER_DELETE,	"DELETE database")
#define	ERR_HELP_CLUSTER_DELETE	 	PKG_SERVER2+10
	stringtext(ERR_HELP_CLUSTER_DELETE,"* Delete a database on last close")
#define ERR_CMD_CMD_START			PKG_SERVER2+11
	errortext(ERR_CMD_CMD_START,	"START")
#define	ERR_TASK_SHARED_MAIL		PKG_SERVER2+12
	stringtext(ERR_TASK_SHARED_MAIL,"Shared mail:            %s%n")
#define	ERR_TASK_SHARED_MAIL_0			PKG_SERVER2+13
	stringtext(ERR_TASK_SHARED_MAIL_0,	"Not Enabled")
#define	ERR_TASK_SHARED_MAIL_1			PKG_SERVER2+14
	stringtext(ERR_TASK_SHARED_MAIL_1,	"Enabled for delivery")
#define	ERR_TASK_SHARED_MAIL_2			PKG_SERVER2+15
	stringtext(ERR_TASK_SHARED_MAIL_2,	"Enabled for delivery and transfer")
#define	ERR_TASK_SHARED_MAIL_DB			PKG_SERVER2+16
	stringtext(ERR_TASK_SHARED_MAIL_DB, "Shared mail database:   %p (%lu bytes)%n")
#define ERR_CMD_CMD_STOP			PKG_SERVER2+17
	errortext(ERR_CMD_CMD_STOP,		"STOP")
#define ERR_NOT_IN_CLUSTER			PKG_SERVER2+18
	stringtext(ERR_NOT_IN_CLUSTER,	"This server is not currently a member of a cluster")
#define ERR_HELP_CMD_START				PKG_SERVER2+19
	stringtext(ERR_HELP_CMD_START,	"Starts the specified port")
#define ERR_HELP_CMD_STOP				PKG_SERVER2+20
	stringtext(ERR_HELP_CMD_STOP,	"Stops the specified port")
#define ERR_SERVER_ACCESS_LIST		PKG_SERVER2+21
	stringtext(ERR_SERVER_ACCESS_LIST, "Error reading server access restriction list")
#define ERR_CMD_SHOW_DATABASE		PKG_SERVER2+22
	stringtext(ERR_CMD_SHOW_DATABASE,"DATABASE filename")
#define ERR_HELP_SHOW_DATABASE		PKG_SERVER2+23
	stringtext(ERR_HELP_SHOW_DATABASE, "Show Database Information")
#define	ERR_SERVER_MAXUSERS			PKG_SERVER2+24
	stringtext(ERR_SERVER_MAXUSERS, "Access to the server is restricted due to maximum number of users.")
#define ERR_TASK_MSG_TRACKING_STATE	PKG_SERVER2+25
	stringtext(ERR_TASK_MSG_TRACKING_STATE, "Mail Tracking:|Not Enabled|Enabled")
#define ERR_TASK_MSG_TRACKING_DISABLED_IDX 1
#define ERR_TASK_MSG_TRACKING_ENABLED_IDX  2 

#define	ERR_CLU_NOREP				PKG_SERVER2+26
	stringtext(ERR_CLU_NOREP,		"Error replicating Domino Directory and cluster database directory to another cluster member.  Please replicate manually.")
#define	ERR_CLU_CLDBDIR_EXISTS		PKG_SERVER2+27
	stringtext(ERR_CLU_CLDBDIR_EXISTS, "Error adding server to cluster:  cluster database directory found on system")
#define	ERR_TASK_DESC_DBCACHE		PKG_SERVER2+28
	stringtext(ERR_TASK_DESC_DBCACHE, "Perform Database Cache maintenance")
#define	ERR_CMD_CMD_DBCACHE			PKG_SERVER2+29
	stringtext(ERR_CMD_CMD_DBCACHE,"DBCACHE")
#define ERR_HELP_CMD_DBCACHE		PKG_SERVER2+30
	stringtext(ERR_HELP_CMD_DBCACHE,"Database Cache management commands")
#define ERR_DBCACHE_FLUSH			PKG_SERVER2+31
	stringtext(ERR_DBCACHE_FLUSH,	"FLUSH")
#define	ERR_HELP_DBCACHE_FLUSH		PKG_SERVER2+32
	stringtext(ERR_HELP_DBCACHE_FLUSH,"Clear out database cache")
#define ERR_DBCACHE_DISABLE			PKG_SERVER2+33
	stringtext(ERR_DBCACHE_DISABLE,	"DISABLE")
#define	ERR_HELP_DBCACHE_DISABLE	PKG_SERVER2+34
	stringtext(ERR_HELP_DBCACHE_DISABLE,"Disable use of database cache")
#define ERR_INVALID_OPERATION		PKG_SERVER2+35
	stringtext(ERR_INVALID_OPERATION, "Requested operation is unavailable on this platform.")
#define ERR_SL_TITLE				PKG_SERVER2+36
	stringtext(ERR_SL_TITLE,		"Lotus Notes SL")
#define ERR_SL_ERROR				PKG_SERVER2+37
	stringtext(ERR_SL_ERROR,		"Lotus Notes SL Error")
#define ERR_DBCACHE_SHOW			PKG_SERVER2+38
	stringtext(ERR_DBCACHE_SHOW,	"SHOW")
#define	ERR_HELP_DBCACHE_SHOW		PKG_SERVER2+39
	stringtext(ERR_HELP_DBCACHE_SHOW,"Show contents of database cache")
#define ERR_CMD_SHOW_DBS			PKG_SERVER2+40
	stringtext(ERR_CMD_SHOW_DBS,"DBS")
#define ERR_HELP_SHOW_DBS			PKG_SERVER2+41
	stringtext(ERR_HELP_SHOW_DBS, "* Show Open Database Information")
#define ERR_SERVER_CHECKPW_MUST_BE_HI		PKG_SERVER2+42
	stringtext(ERR_SERVER_CHECKPW_MUST_BE_HI, "Client password checking has been enabled for this server but will be ignored because the server is not running with a hierarchical name")
#define ERR_CMD_SHOW_FILE			PKG_SERVER2+43
	stringtext(ERR_CMD_SHOW_FILE,"FILE")
#define ERR_HELP_SHOW_FILE			PKG_SERVER2+44
	stringtext(ERR_HELP_SHOW_FILE, "* Show Open File Information")
#define ERR_NO_ANON_ACCESS			PKG_SERVER2+45
	errortext(ERR_NO_ANON_ACCESS,	"You are not allowed to perform this operation with Anonymous access")
#define ERR_CMD_SHOW_DBHASH			PKG_SERVER2+46
	stringtext(ERR_CMD_SHOW_DBHASH,"DBHASH")
#define ERR_HELP_SHOW_DBHASH		PKG_SERVER2+47
	stringtext(ERR_HELP_SHOW_DBHASH, "* Show Database Hash Table Information")
/* SERVER2 error code package goes up to 47 only. */

/* Use SERVER3 error code package which go from 0 to 31. */

#define ERR_HELP_RESTART_PORT		PKG_SERVER3+0
	stringtext(ERR_HELP_RESTART_PORT,	"Disable/Enable transactions on port")
#define ERR_CMD_SHOW_NSTABLE		PKG_SERVER3+1
	stringtext(ERR_CMD_SHOW_NSTABLE, "NSTABLE")
#define ERR_HELP_SHOW_NSTABLE		PKG_SERVER3+2
	stringtext(ERR_HELP_SHOW_NSTABLE, "* Show the contents of the Server's Name Server Table")
#define ERR_CMD_STARTSTOP_PORT		PKG_SERVER3+3
	errortext(ERR_CMD_STARTSTOP_PORT,	"PORT portname")
#define ERR_CMD_RESTART_SERVER			PKG_SERVER3+4
	errortext(ERR_CMD_RESTART_SERVER,	"SERVER [password]")
#define ERR_HELP_START_PORT				PKG_SERVER3+5
	stringtext(ERR_HELP_START_PORT,	"Enable transactions on port")
#define ERR_HELP_STOP_PORT				PKG_SERVER3+6
	stringtext(ERR_HELP_STOP_PORT,	"Disable transactions on port")
#define ERR_RA_LOAD_TABLE_ERROR		PKG_SERVER3+7
	errortext(ERR_RA_LOAD_TABLE_ERROR, "Error loading remote addresses ")
#define ERR_RA_LOAD_TABLE_GENERIC		PKG_SERVER3+8
	errortext(ERR_RA_LOAD_TABLE_GENERIC, "An error was encountered while loading remote addresses%n")
#define ERR_NO_STAT	  					(PKG_SERVER3+9)
	errortext(ERR_NO_STAT,				"Statistic not found on server %a")
#define ERR_STAT_SERVER_ACCESS	  		(PKG_SERVER3+10)
	errortext(ERR_STAT_SERVER_ACCESS,	"Could not get statistics from server %a")
#define ERR_MT_NOT_RUNNING			PKG_SERVER3+11
	errortext(ERR_MT_NOT_RUNNING,	"Message tracking is not available on this server")

#define	ERR_TASK_DESC_GENERIC		PKG_SERVER3+12
	stringtext(ERR_TASK_DESC_GENERIC, "Load Monitor is updating server load|Database Directory Manager Cache Refresher is checking cache|Cluster Administrator is updating cluster")
#define ERR_TASK_DESC_LOADMON_IDX	1
#define ERR_TASK_DESC_SVDBDIR_IDX	2
#define ERR_TASK_DESC_CLADMIN_IDX	3

#define	ERR_TASK_DESC_GENERIC_IDLE	PKG_SERVER3+13
	stringtext(ERR_TASK_DESC_GENERIC_IDLE, "Load Monitor is idle|Database Directory Manager Cache Refresher is idle|Cluster Administrator is idle")
#define ERR_TASK_DESC_LOADMON_IDLE_IDX	1
#define ERR_TASK_DESC_SVDBDIR_IDLE_IDX	2
#define ERR_TASK_DESC_CLADMIN_IDLE_IDX	3

#define	ERR_TASK_DESC_THREADPOOL		PKG_SERVER3+14
	stringtext(ERR_TASK_DESC_THREADPOOL,"Thread pool member on %s")
#define	ERR_TASK_STATUS_AVAILABILITY_INDEX	PKG_SERVER3+15
	stringtext(ERR_TASK_STATUS_AVAILABILITY_INDEX,"Availability Index:     %d (state: %e)%n")
#define	ERR_CLIENT_TOO_OLD				PKG_SERVER3+16
	stringtext(ERR_CLIENT_TOO_OLD ,"Client version not supported by server at this time - please use newer client")
#define ERR_CMD_SHOW_ALLPORTS				PKG_SERVER3+17
	stringtext(ERR_CMD_SHOW_ALLPORTS,		"ALLPORTS")
#define	ERR_TASK_THREADPOOL_ENABLED		PKG_SERVER3+18
	stringtext(ERR_TASK_THREADPOOL_ENABLED	,"Thread Pools:           Enabled%n")
#define	ERR_TASK_THREADPOOL_DISABLED		PKG_SERVER3+19
	stringtext(ERR_TASK_THREADPOOL_DISABLED	,"Thread Pools:           Not Enabled%n")
#define	ERR_CHECK_NETINFO_ERROR			PKG_SERVER3+20
	stringtext(ERR_CHECK_NETINFO_ERROR,	"Unable to verify server document network info")
#define	ERR_HELP_CMD_BATCH				PKG_SERVER3+21
	stringtext(ERR_HELP_CMD_BATCH,"Execute server commands listed in a file")
#define	ERR_CMD_CMD_BATCH				PKG_SERVER3+22
	stringtext(ERR_CMD_CMD_BATCH,"< filename")
#define	ERR_LNAB_AGGREGATOR				PKG_SERVER3+23
	stringtext(ERR_LNAB_AGGREGATOR,	"Directory Cataloger")
#define STR_JS_LNAB_ITEM_NAMES			PKG_SERVER3+24
	stringtext(STR_JS_LNAB_ITEM_NAMES,	"EDFilenames;Enabled;Interval;Schedule;Weekdays;CatalogEnabled;CatalogInterval;CatalogSchedule;CatalogWeekdays")
#define STR_JS_SERVER_NAMESPACE			PKG_SERVER3+25
	stringtext(STR_JS_SERVER_NAMESPACE,		"1\\$Servers")
#define	ERR_CMD_SHOW_SESSIONS_IMAP		PKG_SERVER3+26
	stringtext(ERR_CMD_SHOW_SESSIONS_IMAP,"IMAP")
#define	ERR_CMD_SHOW_SESSIONS_LDAP		PKG_SERVER3+27
	stringtext(ERR_CMD_SHOW_SESSIONS_LDAP,"LDAP")
#define	ERR_CMD_SHOW_SESSIONS_NNTP		PKG_SERVER3+28
	stringtext(ERR_CMD_SHOW_SESSIONS_NNTP,"NNTP")
#define	ERR_CMD_SHOW_SESSIONS_SMTP		PKG_SERVER3+29
	stringtext(ERR_CMD_SHOW_SESSIONS_SMTP,"SMTP")
#define	ERR_CMD_SHOW_SESSIONS_POP3		PKG_SERVER3+30
	stringtext(ERR_CMD_SHOW_SESSIONS_POP3,"POP3")
#define	ERR_CMD_SHOW_SESSIONS_PROXY	PKG_SERVER3+31
	stringtext(ERR_CMD_SHOW_SESSIONS_PROXY,"Proxy")

/* PKG_SERVER3 last code is +31 */

#define ERR_NO_SERVER_DOC				PKG_SERVER4+0
	errortext(ERR_NO_SERVER_DOC, 		"WARNING: Unable to locate a server document for this server - %a")
#define ERR_ENABLED_PORTS				PKG_SERVER4+1
	stringtext(ERR_ENABLED_PORTS,		"Enabled Ports:")
#define ERR_DISABLED_PORTS				PKG_SERVER4+2
	stringtext(ERR_DISABLED_PORTS,		"Disabled Ports:")
#define ERR_HELP_SHOW_ALLPORTS			PKG_SERVER4+3
	stringtext(ERR_HELP_SHOW_ALLPORTS,	"Show configuration for all ports")
#define	ERR_DOMAIN_INDEXER				PKG_SERVER4+4
	stringtext(ERR_DOMAIN_INDEXER,		"Domain Indexer")
#define	ERR_CMD_SHOW_SESSIONS_ICM		PKG_SERVER4+5
	stringtext(ERR_CMD_SHOW_SESSIONS_ICM,"ICM")
#define	ERR_CMD_SHOW_SESSIONS_DBSRV	PKG_SERVER4+6
	stringtext(ERR_CMD_SHOW_SESSIONS_DBSRV,"NRPC")
#define	ERR_HELP_SHOW_SESSIONS_BY_SERVICE		PKG_SERVER4+7
	stringtext(ERR_HELP_SHOW_SESSIONS_BY_SERVICE,"* by Service")
#define	ERR_TASK_TXN_LOGGING_ENABLED				PKG_SERVER4+8
	stringtext(ERR_TASK_TXN_LOGGING_ENABLED, "Transactional Logging:  Enabled%n")
#define	ERR_TASK_TXN_LOGGING_DISABLED				PKG_SERVER4+9
	stringtext(ERR_TASK_TXN_LOGGING_DISABLED,"Transactional Logging:  Not Enabled%n")

#define ERR_CMD_CMD_RESTART				PKG_SERVER4+10
	stringtext(ERR_CMD_CMD_RESTART,		"RESTART")
#define	ERR_HELP_CMD_RESTART_SERVER		PKG_SERVER4+11
	stringtext(ERR_HELP_CMD_RESTART_SERVER,	"Restart Server")
#define ERR_CMD_CMD_TRACE				PKG_SERVER4+12
	stringtext(ERR_CMD_CMD_TRACE,		"TRACE servername")
#define	ERR_HELP_CMD_TRACE				PKG_SERVER4+13
	stringtext(ERR_HELP_CMD_TRACE,		"Trace server connection")
#define	ERR_TASK_DESC_THREADPOOL_SES	PKG_SERVER4+14
	stringtext(ERR_TASK_DESC_THREADPOOL_SES,"Thread pool utility task on %s")
#define ERR_CMD_SHOW_IOCP           	PKG_SERVER4+15
	stringtext(ERR_CMD_SHOW_IOCP,		"IOCP")

/* PKG_SERVER4 LAST CODE IS +15 */

#define ERR_CMD_SHOW_AGENTS				PKG_SERVER5+0
	stringtext(ERR_CMD_SHOW_AGENTS,"AGENTS filename [-verbose]")
#define	ERR_HELP_SHOW_AGENTS			PKG_SERVER5+1
	stringtext(ERR_HELP_SHOW_AGENTS,"Show all agents in a database")
#define ERR_TASK_MAIL_JOURNALLING_STATE	PKG_SERVER5+2
	stringtext(ERR_TASK_MAIL_JOURNALLING_STATE, "Mail Journaling:|Not Enabled|Enabled, Local Destination|Enabled, Remote Destination")
#define ERR_TASK_MAIL_JOURNALLING_DISABLED_IDX 1
#define ERR_TASK_MAIL_JOURNALLING_ENABLED_IDX  2

#define ERR_SRV_ACTIVITY_INIT_FAILED	PKG_SERVER5+3
	errortext(ERR_SRV_ACTIVITY_INIT_FAILED, 	"Activity logging failed to initialize")
#define ERR_HELP_SHOW_DIRECTORIES		PKG_SERVER5+4
	stringtext(ERR_HELP_SHOW_DIRECTORIES,"Extended Directory Assistance Information")
#define ERR_CMD_SHOW_DIRECTORIES		PKG_SERVER5+5
	stringtext(ERR_CMD_SHOW_DIRECTORIES,"XDIR")
#define ERR_USERS_ACCESSING				PKG_SERVER5+6
	stringtext(ERR_USERS_ACCESSING,"Users accessing")
#define	ERR_TASK_SHOW_ELAPSED		PKG_SERVER5+7
	stringtext(ERR_TASK_SHOW_ELAPSED,"%a's elapsed time: %d seconds%n")
#define ERR_CMD_SHOW_SERVER_HEARTBEAT			PKG_SERVER5+8
	stringtext(ERR_CMD_SHOW_SERVER_HEARTBEAT,"HEARTBEAT")
#define	ERR_HELP_SHOW_SERVER_HEARTBEAT	 	PKG_SERVER5+9
	stringtext(ERR_HELP_SHOW_SERVER_HEARTBEAT,"Server heartbeat")
#define ERR_SRV_UNABLE_TO_FIND_PORT		PKG_SERVER5+10
	errortext(ERR_SRV_UNABLE_TO_FIND_PORT, "Unable to find port %s")
#define ERR_VIEW_ONLY_CONSOLE_ACCESS 	PKG_SERVER5+11
	errortext(ERR_VIEW_ONLY_CONSOLE_ACCESS,"You are not authorized to use this remote console command on this server")
#define ERR_HELP_SHOW_ADMINS		PKG_SERVER5+12
	stringtext(ERR_HELP_SHOW_ADMINS,"* List server administrators")
#define ERR_CMD_SHOW_ADMINS		PKG_SERVER5+13
	stringtext(ERR_CMD_SHOW_ADMINS,"ADMINS")
#define ERR_CMD_SHOW_ALLUSER_INFO		PKG_SERVER5+14
	stringtext(ERR_CMD_SHOW_ALLUSER_INFO,"ALLUSERS")
#define ERR_HELP_SHOW_ALLUSER_INFO		PKG_SERVER5+15
	stringtext(ERR_HELP_SHOW_ALLUSER_INFO,"* List all users")

/* PKG_SERVER5 limit to 0 - 15 */

#define ERR_CLDBDIR_NAME			PKG_SERVER6+0
	stringtext(ERR_CLDBDIR_NAME,	"Cluster Directory")
#define ERR_CLREPL_NAME				PKG_SERVER6+1
	stringtext(ERR_CLREPL_NAME,		"Cluster Replicator")
#define ERR_ADMINP_NAME				PKG_SERVER6+2
	stringtext(ERR_ADMINP_NAME,		"Admin Process")
#define ERR_CLU_NOT_IN_CLUSTER		PKG_SERVER6+3
	stringtext(ERR_CLU_NOT_IN_CLUSTER,	"This server is not currently a member of a cluster")
#define ERR_CLU_NO_MATES_AVAIL		PKG_SERVER6+4
	stringtext(ERR_CLU_NO_MATES_AVAIL, "Cannot perform operation.  No other cluster members available")
#define ERR_CLU_NO_UPDATE			PKG_SERVER6+5
	stringtext(ERR_CLU_NO_UPDATE,	"No instance of the cluster directory was updated in the cluster.")
#define	ERR_CLU_NODEL_CLDBDIR		PKG_SERVER6+6
	stringtext(ERR_CLU_NODEL_CLDBDIR, "Error deleting cluster database directory.  Please remove manually.")
#define ERR_CLU_IN_CLUSTER			PKG_SERVER6+7
	errortext(ERR_CLU_IN_CLUSTER,	"This server is currently a member of a cluster")
#define ERR_CLADMIN_REQUEST			PKG_SERVER6+8
	errortext(ERR_CLADMIN_REQUEST,	"Error processing request")
#define	ERR_VDIR_STATUS			PKG_SERVER6+9
	stringtext(ERR_VDIR_STATUS, 	"Hosted Organizations:   %s%n")
#define	ERR_VDIR_INFO				PKG_SERVER6+10
	stringtext(ERR_VDIR_INFO,		"Hosted Organizations:   NAB: %s  Server: %s  $Users view: %s %n")
#define ERR_VDIR_STS_ENABLED		PKG_SERVER6+11
	stringtext(ERR_VDIR_STS_ENABLED, "Enabled")
#define ERR_VDIR_STS_DISABLED		PKG_SERVER6+12
	stringtext(ERR_VDIR_STS_DISABLED, "Not Enabled")
#define ERR_VDIR_STS_ERROR			PKG_SERVER6+13
	stringtext(ERR_VDIR_STS_ERROR,	"Error")
#define ERR_CLREPL_MEMORY			PKG_SERVER6+14
	errortext(ERR_CLREPL_MEMORY,	"Cluster Replicator is not replicating all databases")
#define ERR_SRV_MAX_CONCURRENT_TRANSACTIONS	PKG_SERVER6+15
	stringtext(ERR_SRV_MAX_CONCURRENT_TRANSACTIONS,	"Max Concurrent Transactions: %d%n")
#define ERR_SRV_NUM_THREADPOOL_THREADS	PKG_SERVER6+16
	stringtext(ERR_SRV_NUM_THREADPOOL_THREADS,	"ThreadPool Threads:     %d%n")
#define ERR_SRV_CNET_IOCP_TOTALS		PKG_SERVER6+17
	stringtext(ERR_SRV_CNET_IOCP_TOTALS,	"%n      Total Virtual Threads: %d,  Total TaskIDs: %d,  Total Task Blocks: %d%n%n")
#define ERR_SRV_CNET_IOCP_TASKS			PKG_SERVER6+18
	stringtext(ERR_SRV_CNET_IOCP_TASKS, "%s: PID/VTID %08x:%08x   TaskID %04x%04x   SessionId %04x%04x %n")
#define ERR_SRV_CNET_IOCP_TIME		PKG_SERVER6+19
	errortext(ERR_SRV_CNET_IOCP_TIME,	"      StartTime: %s%n      RunTime:   %s%n")
#define ERR_SRV_CNET_IOCP_STATE		PKG_SERVER6+20
	errortext(ERR_SRV_CNET_IOCP_STATE,	"      TaskState: (%xh)     wEventIOCP: (%xh)     wEventType: (%xh)%n")
#define ERR_SRV_CNET_IOCP_INUSE		PKG_SERVER6+21
	errortext(ERR_SRV_CNET_IOCP_INUSE,	"      aInUse: (%d)         fSSLPort: (%d)        fDeletePending: (%d)%n%n")
#define ERR_CMD_SHOW_OPENDATABASES	PKG_SERVER6+22
	stringtext(ERR_CMD_SHOW_OPENDATABASES,	"OPENDATABASES")
#define ERR_HELP_SHOW_OPENDATABASES	PKG_SERVER6+23
	stringtext(ERR_HELP_SHOW_OPENDATABASES,	"Show Open Database Information")
#define ERR_SCHEDULE_HEADER			PKG_SERVER6+24
	stringtext(ERR_SCHEDULE_HEADER,	"Scheduled\t\t\tType\t\t\tNext schedule")
#define ERR_CMD_RESTART_TASK		PKG_SERVER6+25
	errortext(ERR_CMD_RESTART_TASK,"TASK taskname")
#define ERR_HELP_RESTART_TASK		PKG_SERVER6+26
	stringtext(ERR_HELP_RESTART_TASK,"Restart specified server task")
#define ERR_RESTART_FAILED_ERR		PKG_SERVER6+27
	stringtext(ERR_RESTART_FAILED_ERR,"Unable to restart %s%n")
#define ERR_RESTARTING_TASK			PKG_SERVER6+28
	stringtext(ERR_RESTARTING_TASK,	"Waiting %d seconds to restart %s...%n")
#define ERR_CLUSTER_NAME			PKG_SERVER6+29
	stringtext(ERR_CLUSTER_NAME,	"Member of cluster:\t%s\n")
#define ERR_NOAGENT_SCHEDULE		PKG_SERVER6+30
	stringtext(ERR_NOAGENT_SCHEDULE,"Agent Manager not loaded, no agent schedule available.\n")
/* PKG_SERVER6 limit to 0 - 31 */

#define ERR_TASK_FAULTRECOVERY_STATE	PKG_SERVER7+0
	stringtext(ERR_TASK_FAULTRECOVERY_STATE, "Fault Recovery:|Not Enabled|Enabled")
#define ERR_DEBUG_VARS1	PKG_SERVER7+1
	stringtext(ERR_DEBUG_VARS1, "NOTES.INI contains the following %s parameters:\n")
#define ERR_DEBUG_VARS2	PKG_SERVER7+2
	stringtext(ERR_DEBUG_VARS2, "Warning: Debug parameters could impact operation or performance.\n")
#define ERR_DEBUG_VARS3	PKG_SERVER7+3
	stringtext(ERR_DEBUG_VARS3, "Contact your appropriate support vendor.\n")
#define ERR_START_CONTROLLER	PKG_SERVER7+4
	stringtext(ERR_START_CONTROLLER, "DominoController is running or died abnormally leaving lock file(in latter case, delete .jsc_lock in data directory and run).")
#define ERR_QUIT_CONTROLLER	PKG_SERVER7+5
	stringtext(ERR_QUIT_CONTROLLER, "DominoController is not currently running in this data directory.")
#define ERR_CMD_SHOW_LOCALE				PKG_SERVER7+7
	stringtext(ERR_CMD_SHOW_LOCALE,		"LOCALE")
#define ERR_HELP_SHOW_LOCALE			PKG_SERVER7+8
	stringtext(ERR_HELP_SHOW_LOCALE,	"* Locale information")
#define ERR_CONSOLE_REGION				PKG_SERVER7+9
	stringtext(ERR_CONSOLE_REGION,		"Region:    %s [%s]")
#define ERR_CONSOLE_COLLATION			PKG_SERVER7+10
	stringtext(ERR_CONSOLE_COLLATION,	"Collation: %s%s [%s]")
#define ERR_CONSOLE_CODEPAGE			PKG_SERVER7+11
	stringtext(ERR_CONSOLE_CODEPAGE,	"CSID:      %x(Hex)")
#define ERR_CMD_SHOW_TIMEZONE			PKG_SERVER7+12
	stringtext(ERR_CMD_SHOW_TIMEZONE,	"TIMEZONE")
#define ERR_HELP_SHOW_TIMEZONE			PKG_SERVER7+13
	stringtext(ERR_HELP_SHOW_TIMEZONE,	"* Time zone information")
#define ERR_CONSOLE_TZ_STDTIME			PKG_SERVER7+14
	stringtext(ERR_CONSOLE_TZ_STDTIME,	"Standard Time: GMT%s%d:%02d")
#define ERR_CONSOLE_TZ_DST				PKG_SERVER7+15
	stringtext(ERR_CONSOLE_TZ_DST,		"DST:           Observed|DST:           Not Observed")
#define ERR_CONSOLE_TZ_DSTBEGIN			PKG_SERVER7+16
	stringtext(ERR_CONSOLE_TZ_DSTBEGIN,	"DST Begin:     Month[%2d] Week[%2d] Weekday[%s]|DST Begin:     Month[%2d] Day[%2d]")
#define ERR_CONSOLE_TZ_DSTEND			PKG_SERVER7+17
	stringtext(ERR_CONSOLE_TZ_DSTEND,	"DST End:       Month[%2d] Week[%2d] Weekday[%s]|DST End:       Month[%2d] Day[%2d]")
#define ERR_CMD_SHOW_SESSIONS_DIIOP		PKG_SERVER7+18
	stringtext(ERR_CMD_SHOW_SESSIONS_DIIOP,"DIIOP")
#define ERR_CMD_SHOW_SESSIONS_RDEBUG	PKG_SERVER7+19
	stringtext(ERR_CMD_SHOW_SESSIONS_RDEBUG,"RDEBUG")
#define ERR_CLIENT_VERSION_TOO_LOW		PKG_SERVER7+20
	stringtext(ERR_CLIENT_VERSION_TOO_LOW, "The version of this client is below the minimum level required to access the server")
#define ERR_CLIENT_VERSION_TOO_HIGH		PKG_SERVER7+21
	stringtext(ERR_CLIENT_VERSION_TOO_HIGH, "The version of this client is above the maximum level allowed to access the server")
#define ERR_CMD_STOP_START			PKG_SERVER7+22
	stringtext(ERR_CMD_STOP_START, "Stop|Start")
#define ERR_CMD_STOP_IDX 1
#define ERR_CMD_START_IDX 2
#define ERR_CONSOLE_LOG				PKG_SERVER7+23
	stringtext(ERR_CONSOLE_LOG,		"Console Logging is ENABLED\n|Console Logging is DISABLED\n|Console Logging is already ENABLED\n|Console Logging is already DISABLED\n|The Console Log file is undefined\n|The Console file is %s\n")
#define ERR_LOG_ENABLED				1
#define ERR_LOG_DISABLED			2
#define ERR_LOG_ALREADY_ENABLED		3
#define ERR_LOG_ALREADY_DISABLED	4
#define ERR_LOG_UNDEFINED			5
#define ERR_LOG_FILE				6
#define ERR_FULL_ADMIN_DENIED	  		PKG_SERVER7+24
	errortext(ERR_FULL_ADMIN_DENIED,	"%a was denied full administrator access.")
#define ERR_FULL_ADMIN_ALLOWED	  		PKG_SERVER7+25
	errortext(ERR_FULL_ADMIN_ALLOWED,	"%a was granted full administrator access.")
#define ERR_ATC_CONFIG_CHECK			PKG_SERVER7+26
	stringtext(ERR_ATC_CONFIG_CHECK, 		"Unable to retrieve activity trends configuration.")
#define ERR_TASK_DESC_LIVE_CONSOLE_THREAD	PKG_SERVER7+27
	stringtext(ERR_TASK_DESC_LIVE_CONSOLE_THREAD,"Remote live console task on %s")
#define ERR_UNSETTABLE_VARIABLE			PKG_SERVER7+28
	errortext(ERR_UNSETTABLE_VARIABLE,	"This system variable cannot be set via the server console. You must edit NOTES.INI to set this variable.")
#define ERR_TASK_CONTROLLER_STATE		PKG_SERVER7+29
	stringtext(ERR_TASK_CONTROLLER_STATE, "Server Controller:|Not Enabled|Enabled")
#define ERR_CMD_STARTSTOP_CONLOG		PKG_SERVER7+30
	errortext(ERR_CMD_STARTSTOP_CONLOG,	"CONSOLELOG")
#define ERR_HELP_START_CONLOG			PKG_SERVER7+31
	stringtext(ERR_HELP_START_CONLOG,	"Enable console logging")
#define ERR_HELP_STOP_CONLOG			PKG_SERVER7+32
	stringtext(ERR_HELP_STOP_CONLOG,	"Disable console logging")
#define ERR_CMD_SHOW_OFFSETS                  PKG_SERVER7+33
      	stringtext(ERR_CMD_SHOW_OFFSETS,"OFFSETS")
#define ERR_HELP_SHOW_OFFSETS                 PKG_SERVER7+34
      	stringtext(ERR_HELP_SHOW_OFFSETS, "* Show OFSGD Offsets Information")
#define ERR_CMD_SHOW_INTUSER_INFO		PKG_SERVER7+35
	stringtext(ERR_CMD_SHOW_INTUSER_INFO,"INETUSERS")
#define ERR_HELP_SHOW_INTUSER_INFO		PKG_SERVER7+36
	stringtext(ERR_HELP_SHOW_INTUSER_INFO,"* List internet server users")
#define ERR_NO_USER_INFO				PKG_SERVER7+37
	stringtext(ERR_NO_USER_INFO,		"No user info")
#define ERR_TASK_STATUS_CONFILE_DIAGDIR     PKG_SERVER7+38
	stringtext(ERR_TASK_STATUS_CONFILE_DIAGDIR,"Console Log File:|Diagnostic Directory:")
#define ERR_TASK_STATUS_CONFILE_DIAGDIR_CON_IDX		1
#define ERR_TASK_STATUS_CONFILE_DIAGDIR_DIAG_IDX	2
#define ERR_TASK_CONSOLE_LOGGING_STATE	PKG_SERVER7+39
	stringtext(ERR_TASK_CONSOLE_LOGGING_STATE, "Console Logging:|Not Enabled|Enabled")
#define ERR_CMD_SAVE_NOTE			PKG_SERVER7+40
	stringtext(ERR_CMD_SAVE_NOTE,"NOTEID noteid")
#define ERR_CMD_CMD_SAVE			PKG_SERVER7+41
	stringtext(ERR_CMD_CMD_SAVE,"SAVE")
#define ERR_HELP_CMD_SAVE				PKG_SERVER7+42
	stringtext(ERR_HELP_CMD_SAVE,		"Save a supported item")
#define ERR_HELP_SAVE_NOTE			PKG_SERVER7+43
	stringtext(ERR_HELP_SAVE_NOTE,"Save documents specified by noteid from the Domino Directory as DXL")
#define ERR_SAVE_NOTE_STATUS			PKG_SERVER7+44
	stringtext(ERR_SAVE_NOTE_STATUS	,"Save NOTE as DXL failed|NOTE %s saved in file %s")
#define  ERR_SAVE_NOTE_IDX_FAILED 1
#define  ERR_SAVE_NOTE_IDX_SUCCESS 2
#define ERR_DEBUG_VARS4				PKG_SERVER7+45
	stringtext(ERR_DEBUG_VARS4,"The System Environment contains the following Notes %s parameters:")
#define ERR_SERVER_SU_MAXIMUM_KITS			PKG_SERVER7+46
	stringtext(ERR_SERVER_SU_MAXIMUM_KITS,"The Smart Upgrade database %s on %a has exceeded the %d maximum number of kits supported.")

/* PKG_SERVER7 limit to 0 - 103 */

#define ERR_CMD_CMD_PERF				PKG_PERFSTAT+0
	stringtext(ERR_CMD_CMD_PERF,		"PLATFORM")
#define ERR_HELP_CMD_PERF				PKG_PERFSTAT+1
	stringtext(ERR_HELP_CMD_PERF,		"Platform Statistics")
#define ERR_CMD_PERF_TIME				PKG_PERFSTAT+2
	stringtext(ERR_CMD_PERF_TIME,		"TIME")
#define ERR_HELP_CMD_PERF_TIME			PKG_PERFSTAT+3
	stringtext(ERR_HELP_CMD_PERF_TIME,	"Show/Set Platform Statistics sampling rate")
#define ERR_CMD_PERF_RESET				PKG_PERFSTAT+4
	stringtext(ERR_CMD_PERF_RESET,		"RESET [interval enable/disable]")
#define ERR_HELP_CMD_PERF_RESET			PKG_PERFSTAT+5
	stringtext(ERR_HELP_CMD_PERF_RESET,	"Reset, or enable/disable interval reset")
#define ERR_CMD_PERF_WAIT				PKG_PERFSTAT+6
	stringtext(ERR_CMD_PERF_WAIT,		"PAUSE")
#define ERR_HELP_CMD_PERF_WAIT			PKG_PERFSTAT+7
	stringtext(ERR_HELP_CMD_PERF_WAIT,	"Temporarily halt Platform Statistics sampling")
#define ERR_CMD_PERF_RUN				PKG_PERFSTAT+8
	stringtext(ERR_CMD_PERF_RUN,		"RESUME")
#define ERR_HELP_CMD_PERF_RUN			PKG_PERFSTAT+9
	stringtext(ERR_HELP_CMD_PERF_RUN,	"Resume Platform Statistics sampling")
#define ERR_PERFSTAT_GENERROR			(PKG_PERFSTAT+10)
	errortext(ERR_PERFSTAT_GENERROR,	"Platform Stats: error!")
#define ERR_PERFSTAT_INITREAD			(PKG_PERFSTAT+11)
	errortext(ERR_PERFSTAT_INITREAD,	"Platform Stats: Unable to read initialization file %s")
#define MSG_PERFSTAT_CMDDEFAULT			(PKG_PERFSTAT+12)
	errortext(MSG_PERFSTAT_CMDDEFAULT,	"Platform Stats: Using default counters and update interval.")
#define ERR_PERFSTAT_PSCLPOOLFULL		(PKG_PERFSTAT+13)
	errortext(ERR_PERFSTAT_PSCLPOOLFULL, "Platform Stats: PSCounterList VPool is full.")
#define ERR_PERFSTAT_CNTRINIT			(PKG_PERFSTAT+14)
	errortext(ERR_PERFSTAT_CNTRINIT,	"Platform Stats: Unable to set up performance counters.")
#define ERR_PERFSTAT_CNTRUPDATE			(PKG_PERFSTAT+15)
	errortext(ERR_PERFSTAT_CNTRUPDATE,	"Platform Stats: Unable to update performance counters.")
#define ERR_PERFSTAT_CREATECNTR			(PKG_PERFSTAT+16)
	errortext(ERR_PERFSTAT_CREATECNTR,	"Platform Stats: Unable to create counter %s of context %s.")
#define ERR_PERFSTAT_INTRVLSET			(PKG_PERFSTAT+17)
	errortext(ERR_PERFSTAT_INTRVLSET,	"Platform Stats: Update Interval is already set to %s.")
#define ERR_PERFSTAT_INTRVLINC			(PKG_PERFSTAT+18)
	errortext(ERR_PERFSTAT_INTRVLINC,	"Platform Stats: Update Interval command is incomplete.")
#define ERR_PERFSTAT_INTRVLBAD			(PKG_PERFSTAT+19)
	errortext(ERR_PERFSTAT_INTRVLBAD,	"Platform Stats: Bad Update Interval %s. Must be an integer between 1 and %s.")
#define ERR_PERFSTAT_NOTINST			(PKG_PERFSTAT+20)
	errortext(ERR_PERFSTAT_NOTINST,	"Platform Stats: %s is not a valid instance of %s.")
#define ERR_PERFSTAT_NOTSTAT			(PKG_PERFSTAT+21)
	errortext(ERR_PERFSTAT_NOTSTAT,	"Platform Stats: %s is not a valid statistic of %s.")
#define ERR_PERFSTAT_NONODE				(PKG_PERFSTAT+22)
	errortext(ERR_PERFSTAT_NONODE,	"Platform Stats: Unable to monitor statistic %s of context %s.")
#define ERR_PERFSTAT_UNKWNCMD			(PKG_PERFSTAT+23)
	errortext(ERR_PERFSTAT_UNKWNCMD,	"Platform Stats: Unknown command starting with: %s.")
#define ERR_PERFSTAT_UNKNWNOPER			(PKG_PERFSTAT+24)
	errortext(ERR_PERFSTAT_UNKNWNOPER,	"Platform Stats: Unknown operation: %s.")
#define ERR_PERFSTAT_NTPOOLFULL			(PKG_PERFSTAT+25)
	errortext(ERR_PERFSTAT_NTPOOLFULL,	"Platform Stats: Unable to add to Platform Statistics name table.")
#define MSG_PERFSTAT_CMDTIME				PKG_PERFSTAT+26
	stringtext(MSG_PERFSTAT_CMDTIME,		"Platform Stats: Set monitoring interval to %s minutes.")
#define MSG_PERFSTAT_CMDTIME_DSPLY			PKG_PERFSTAT+27
	stringtext(MSG_PERFSTAT_CMDTIME_DSPLY,	"Platform Stats: Monitoring interval is currently %s minute(s).")
#define MSG_PERFSTAT_CMDRESET				PKG_PERFSTAT+28
	stringtext(MSG_PERFSTAT_CMDRESET,		"Platform Stats: Re-initialized statistics for monitoring.")

#define MSG_PERFSTAT_CMDWAIT				PKG_PERFSTAT+29
	stringtext(MSG_PERFSTAT_CMDWAIT,		"Platform Stats: Pausing statistics monitoring.")
#define MSG_PERFSTAT_CMDRUN					PKG_PERFSTAT+30
	stringtext(MSG_PERFSTAT_CMDRUN,			"Platform Stats: Resuming statistics monitoring.")
#define ERR_PL_POOLFULL			(PKG_PERFSTAT+31)
	errortext(ERR_PL_POOLFULL,	"Unable to create pool for PerfList")

/* PKG_PERFSTAT LAST CODE IS +31 */

#define ERR_REG_KEY_NOT_SET					PKG_PLAT_STAT_ERR+0
	errortext(ERR_REG_KEY_NOT_SET,			"Platform Stats: Please check that the Registry Key HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\notestat\\Performance\\Library is set to NNOTES.DLL, including the path to your Domino server executable directory.")
#define ERR_DLL_DIR_NOT_SAME_AS_EXE			PKG_PLAT_STAT_ERR+1
	errortext(ERR_DLL_DIR_NOT_SAME_AS_EXE,	"Platform Stats: Notes DLL directory is different from executable directory.\n Edit the following Registry setting to conform to the server being run.\nHKEY_LOCAL_MACHINE\\SYSTEM\\ControlSet\\Services\\notestat\\Performance\\Library")
#define ERR_NETWRK_CNTRS_NOT_ENABLED        PKG_PLAT_STAT_ERR+2
	errortext(ERR_NETWRK_CNTRS_NOT_ENABLED, "Platform Stats: Please see online help to enable SNMP service in order to monitor network performance.")
#define ERR_DISK_CNTRS_NOT_ENABLED          PKG_PLAT_STAT_ERR+3
	errortext(ERR_DISK_CNTRS_NOT_ENABLED,   "Platform Stats: Please run 'diskperf.exe -y' to enable Logical Disk performance counters.")
#define ERR_WIN2K_IMPROPER_INSTALL          PKG_PLAT_STAT_ERR+4
	errortext(ERR_WIN2K_IMPROPER_INSTALL,   "Platform Stats: Unable to obtain performance data. Please check Microsoft MSDN on how to enable performance data collection.")
#define ERR_PROCSTAT_MEM					PKG_PLAT_STAT_ERR+5
	errortext(ERR_PROCSTAT_MEM,				"Platform Stats: Unable to allocate memory for process stats.")
#define ERR_PROCSTAT_FILE					PKG_PLAT_STAT_ERR+6
	errortext(ERR_PROCSTAT_FILE,			"Platform Stats: File I/O error.")
#define ERR_PROCSTAT_GET_PROC				PKG_PLAT_STAT_ERR+7
	errortext(ERR_PROCSTAT_GET_PROC,		"Platform Stats: Unable to obtain process data.")
#define ERR_HELP_CMD_PERF_RESET_OFF			PKG_PLAT_STAT_ERR+8
	stringtext(ERR_HELP_CMD_PERF_RESET_OFF,	"Platform Stats: Reset interval now disabled.")
#define ERR_HELP_CMD_PERF_RESET_ON			PKG_PLAT_STAT_ERR+9
	stringtext(ERR_HELP_CMD_PERF_RESET_ON,	"Platform Stats: Reset interval now enabled.")
#define ERR_TASK_DESC_PLATSTATS				PKG_PLAT_STAT_ERR+10
	stringtext(ERR_TASK_DESC_PLATSTATS, "Platform Stats is initializing|Platform Stats is idle|Platform Stats is gathering statistics|Platform Stats is paused|Platform Stats is terminating")
#define ERR_TASK_DESC_PLATSTATS_INIT_IDX	1
#define ERR_TASK_DESC_PLATSTATS_IDLE_IDX	2
#define ERR_TASK_DESC_PLATSTATS_ACTIVE_IDX	3
#define ERR_TASK_DESC_PLATSTATS_PAUSED_IDX	4
#define ERR_TASK_DESC_PLATSTATS_TERM_IDX	5
#define ERR_BAD_COUNTER_VALUE				PKG_PLAT_STAT_ERR+11
	errortext(ERR_BAD_COUNTER_VALUE,		"Platform Stats: Bad counter value generated.")


/* PKG_PLAT_STAT_ERR LAST CODE IS + 15 */
	
#endif 

