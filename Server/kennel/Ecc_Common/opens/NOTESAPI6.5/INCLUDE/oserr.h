#ifndef OSERR_DEFS
#define OSERR_DEFS

/* error codes  */

#define ERR_NTMULTI			PKG_OS+0
	errortext(ERR_NTMULTI,		"Cannot restart as new user.  Please be sure the multi-user cleanup service has been installed and started.")
#define ERR_UNKNOWN			PKG_OS+1
	errortext(ERR_UNKNOWN,		"Unknown OS error")
#define ERR_PROTECTED		PKG_OS+2
	errortext(ERR_PROTECTED,	"Cannot write or create file (file or disk is read-only)")
#define ERR_NOEXIST			PKG_OS+3
	errortext(ERR_NOEXIST,		"File does not exist")
#define ERR_EXISTS			PKG_OS+4
	errortext(ERR_EXISTS,		"File already exists")
#define ERR_FILES			PKG_OS+5
	errortext(ERR_FILES,		"Too many open files")
#define ERR_DISK			PKG_OS+6
	errortext(ERR_DISK,			"Insufficient disk space")
#define ERR_MEMORY			PKG_OS+7
#ifdef OS2
	errortext(ERR_MEMORY,		"Insufficient memory.  Check space on disk used for OS/2 swapping.")
#else
#ifdef MAC
	errortext(ERR_MEMORY,		"Insufficient memory. Please close any unneeded windows. You may also increase Notes' available memory by quitting and choosing 'Get Info...' from the Finder.")
#else
	errortext(ERR_MEMORY,		"Insufficient memory.")
#endif
#endif
#define ERR_CREATE			PKG_OS+8
	errortext(ERR_CREATE,		"File cannot be created")
#define ERR_TRUNC			PKG_OS+9
	errortext(ERR_TRUNC,		"File truncated - file may have been damaged")
#define ERR_LOCKS			PKG_OS+10
	errortext(ERR_LOCKS,		"too many file locks")
#define	ERR_NO_MORE_FILES	PKG_OS+11
	errortext(ERR_NO_MORE_FILES,"no more files to search")
#define ERR_LOCK			PKG_OS+12
	errortext(ERR_LOCK,			"File is in use by another program")
#define ERR_FD				PKG_OS+13
	errortext(ERR_FD,			"invalid file handle")
#define ERR_IOERROR			PKG_OS+14
	errortext(ERR_IOERROR,		"I/O data error")
#define ERR_DRIVE_NOT_READY	PKG_OS+15
	errortext(ERR_DRIVE_NOT_READY,"Drive is not ready")
#define ERR_ACCESS		PKG_OS+16
	errortext(ERR_ACCESS,		 "Access to data denied.")
#define ERR_PROGDIR			PKG_OS+17
	errortext(ERR_PROGDIR,	"Unable to access files directory")
#define ERR_INV_SERVER_NAME PKG_OS+18
	errortext(ERR_INV_SERVER_NAME,	"Invalid server or network name syntax")
#define ERR_INV_FILE_NAME 	PKG_OS+19
	errortext(ERR_INV_FILE_NAME,	"File name too long or invalid file name syntax")
#define ERR_DS_MEMORY		PKG_OS+20
	errortext(ERR_DS_MEMORY,		"Insufficient memory - local heap is full")
#define ERR_OS_ERROR		PKG_OS+21
	errortext(ERR_OS_ERROR,			"An OS/2 error has occurred.  The code is %d (0x%04x).  Consult your OS/2 documentation or ask for assistance.")
#define ERR_REALLOC_LOCKED	PKG_OS+22
	errortext(ERR_REALLOC_LOCKED,	"Warning: Novell OS/2 Requestor left I/O buffer locked.  Retrying...")
#define ERR_LOCALDEBUG1		PKG_OS+23
	debugtext(ERR_LOCALDEBUG1,		"OSLocalAlloc: DS Moved %04x -> %04x%n")
#define ERR_LOCALDEBUG2		PKG_OS+24
	debugtext(ERR_LOCALDEBUG2,		"OSLocalAlloc: DS Grew  %ld -> %ld%n")
#define ERR_LOCAL_REFCNT	PKG_OS+25
	debugtext(ERR_LOCAL_REFCNT,		"OSLocalAlloc: DS lock count = %u -- CAN'T EXTEND DS%n")
#define ERR_LIBLOAD			PKG_OS+26
	errortext(ERR_LIBLOAD,			"Insufficient memory to load the executable program library.")
#define ERR_PATHCHAR		PKG_OS+27
	errortext(ERR_PATHCHAR,			"Invalid character in file name")
#define ERR_LOCALCHECK1		PKG_OS+28
	debugtext(ERR_LOCALCHECK1,		"OSLocalCheck: First arena entry blown")
#define ERR_LOCALCHECK2		PKG_OS+29
	debugtext(ERR_LOCALCHECK2,		"OSLocalCheck: Object count does not match chain")
#define ERR_LOCALCHECK3		PKG_OS+30
	debugtext(ERR_LOCALCHECK3,		"OSLocalCheck: Prev Link Destroyed")
#define ERR_LOCALCHECK4		PKG_OS+31
	debugtext(ERR_LOCALCHECK4,		"OSLocalCheck: Invalid Local Object")
#define ERR_CIRC_DIR		PKG_OS+32
	errortext(ERR_CIRC_DIR,			"Circular Directory Scan Reference")
#define ERR_OSTIME			PKG_OS+33
	errortext(ERR_OSTIME,			"Operating System Time or Date is invalid!")
#define ERR_NO_USER			PKG_OS+34
	errortext(ERR_NO_USER,			"User information not available")
#define ERR_OLDEXE			PKG_OS+35
	errortext(ERR_OLDEXE,			"Driver file format error")
#define ERR_ONEMOMENT		PKG_OS+36
	stringtext(ERR_ONEMOMENT,		"One moment, please...")
#define ERR_SWAPIN_MEMORY	PKG_OS+37
	errortext(ERR_SWAPIN_MEMORY,	"Insufficient memory for swapper - cannot continue")
#define ERR_SWAPIN_DISK		PKG_OS+38
	errortext(ERR_SWAPIN_DISK,		"Disk or network error (reading swap file) - cannot continue")
#define ERR_OSMESSAGE_OK	PKG_OS+39
	internaltext(ERR_OSMESSAGE_OK,		"(internal, used by message signal handler")
#define ERR_OSMESSAGE_YES	PKG_OS+40
	internaltext(ERR_OSMESSAGE_YES,	"(internal, used by message signal handler")
#define ERR_OSMESSAGE_NO	PKG_OS+41
	internaltext(ERR_OSMESSAGE_NO,		"(internal, used by message signal handler")
#define ERR_OSMESSAGE_CANCEL PKG_OS+42
	internaltext(ERR_OSMESSAGE_CANCEL,	"(internal, used by message signal handler")
#define ERR_OSMESSAGE_RETRY	PKG_OS+43
	internaltext(ERR_OSMESSAGE_RETRY,	"(internal, used by message signal handler")
#define ERR_NOTMULTITASK	PKG_OS+44
	errortext(ERR_NOTMULTITASK,			"Cannot support multiple processes using Notes in this environment")
#define ERR_RESTORED		PKG_OS+45
	internaltext(ERR_RESTORED,			"(restored from memory image")
#define ERR_WRITETRUNC		PKG_OS+46
	errortext(ERR_WRITETRUNC,		"Cannot write to file (possibly it is READ-ONLY or the disk is out of space or not ready)")
#define ERR_LOCALDEBUG3		PKG_OS+47
	debugtext(ERR_LOCALDEBUG3,		"OSLocalAlloc: retAddress arg is in DS being moved!!!")
#define	ERR_BADQUOTE_FORMAT	PKG_OS+48
	errortext(ERR_BADQUOTE_FORMAT,	"Invalid quoted argument format")
#define	ERR_DIVIDE_BY_ZERO	PKG_OS+49
	debugtext(ERR_DIVIDE_BY_ZERO,	"Divide By Zero")
#define	ERR_MARKERS_PRECEED	PKG_OS+50
	debugtext(ERR_MARKERS_PRECEED,	"CheckMarkers: Memory preceding object has been destroyed")
#define	ERR_MARKERS_FOLLOW	PKG_OS+51
	debugtext(ERR_MARKERS_FOLLOW,	"CheckMarkers: Memory following object has been destroyed")
#define	ERR_MARKERS_DUMP	PKG_OS+52
	debugtext(ERR_MARKERS_DUMP,	"CheckMarkers: Segment length: %u%n")
#define	ERR_INVALID_DRIVE	PKG_OS+53
	errortext(ERR_INVALID_DRIVE,	"Invalid drive")
#define	ERR_OS_NULL_HANDLE	PKG_OS+54
	debugtext(ERR_OS_NULL_HANDLE,	"LookupHandle: null handle")
#define	ERR_OS_HANDLE_RANGE	PKG_OS+55
	debugtext(ERR_OS_HANDLE_RANGE,	"LookupHandle: handle out of range")
#define	ERR_OS_FREE_HANDLE	PKG_OS+56
	debugtext(ERR_OS_FREE_HANDLE,	"LookupHandle: handle not allocated")
#define	ERR_OS_STILL_LOCKED PKG_OS+57
	debugtext(ERR_OS_STILL_LOCKED,	"MemFree: object still locked")
#define	ERR_OS_ALREADY_UNLOCKED PKG_OS+58
	debugtext(ERR_OS_ALREADY_UNLOCKED, "UnlockObject: already unlocked")
#define	ERR_SEGMENT_TOO_BIG	PKG_OS+59
	errortext(ERR_SEGMENT_TOO_BIG, "Memory allocation request exceeded 65,000 bytes")
#define	ERR_FILE_CLOSE_ACC	PKG_OS+60
	internaltext(ERR_FILE_CLOSE_ACC, "Attempt to close file that still has accessors")
#define	ERR_THREAD_CREATE	PKG_OS+61
	errortext(ERR_THREAD_CREATE, "Cannot create thread")
#define	ERR_SIGNAL_CHANGE	PKG_OS+62
	internaltext(ERR_SIGNAL_CHANGE, "Cannot change a signal handler!")
#define	ERR_PCB				PKG_OS+63
	internaltext(ERR_PCB,	 "There is already a PCB for this process!")
#define ERR_LIBLOAD2	 	PKG_OS+64
	errortext(ERR_LIBLOAD2,	"Program library not loaded. Required .DLL missing or could not load.")
#define ERR_OS_LAUNCH_UNAVAILABLE	 PKG_OS+65
	errortext(ERR_OS_LAUNCH_UNAVAILABLE,	"Launching attachments only works with System 7 or later.")
#define ERR_SETUP_NOT_DONE	PKG_OS+66
#ifndef OS400
	errortext(ERR_SETUP_NOT_DONE,"Launching the Domino setup wizard to setup your server...")
#else
	errortext(ERR_SETUP_NOT_DONE,"Server is not setup.  Please run CFGDOMSVR or the Setup Wizard to setup your Domino server...")
#endif
#define ERR_MEMORY_WENT_AWAY PKG_OS+67
	debugtext(ERR_MEMORY_WENT_AWAY,"Halt: ANOTHER process crashed (not this one). Report previous crash!")
#define ERR_UNLOCK_UNDERFLOW PKG_OS+68
	debugtext(ERR_UNLOCK_UNDERFLOW,"DosUnlockSeg expected error (%e)%n")
#define ERR_FREESEG			PKG_OS+69
	debugtext(ERR_FREESEG,	"DosFreeSeg error")
#define ERR_OSMESSAGE_CANNOT_PROMPT PKG_OS+70
	internaltext(ERR_OSMESSAGE_CANNOT_PROMPT, "(internal, used by message signal handler")
#define ERR_LIBLOAD3	 	PKG_OS+71
	errortext(ERR_LIBLOAD3,	"The executable program library cannot be found.")
#define ERR_EXCEPTION	 	PKG_OS+72
	debugtext(ERR_EXCEPTION,"Fatal program exception interrupt")
#define ERR_OS_EXECERRORNF	PKG_OS+73
	errortext(ERR_OS_EXECERRORNF,"Unable to locate program")
#define ERR_OS_EXECERROR	PKG_OS+74
	errortext(ERR_OS_EXECERROR,"Unable to invoke program")
#define ERR_OS_ABSOLUTE 	PKG_OS+75
	errortext(ERR_OS_ABSOLUTE,	"File name must be an absolute path")

/*** PKG_OS+76 through PKG_OS+124 are reserved for internationalization use */

/* Daylight saving in GMT is called Bristish Summer Time (BST)  SPR #CLE3GUFC9 */

#define STR_OS_ZONE_0		PKG_OS+77
	stringtext(STR_OS_ZONE_0,	"0 GMT GDT Greenwich Mean Time")
#define STR_OS_ZONE_1		PKG_OS+78
	stringtext(STR_OS_ZONE_1,	"1 ZW1 YW1 One hour west of GMT")
#define STR_OS_ZONE_2		PKG_OS+79
	stringtext(STR_OS_ZONE_2,	"2 ZW2 YW2 Two hours west of GMT")
#define STR_OS_ZONE_3		PKG_OS+80
	stringtext(STR_OS_ZONE_3,	"3 ZW3 ZW3 Three hours west of GMT")
#define STR_OS_ZONE_4		PKG_OS+81
	stringtext(STR_OS_ZONE_4,	"3003 NST NDT Newfoundland")
#define STR_OS_ZONE_5		PKG_OS+82
	stringtext(STR_OS_ZONE_5,	"4 AST ADT Atlantic Standard Time")
#define STR_OS_ZONE_6		PKG_OS+83
	stringtext(STR_OS_ZONE_6,	"5 EST EDT Eastern Standard Time")
#define STR_OS_ZONE_7		PKG_OS+84
	stringtext(STR_OS_ZONE_7,	"6 CST CDT Central Standard Time")
#define STR_OS_ZONE_8		PKG_OS+85
	stringtext(STR_OS_ZONE_8,	"7 MST MDT Mountain Standard Time")
#define STR_OS_ZONE_9		PKG_OS+86
	stringtext(STR_OS_ZONE_9,	"8 PST PDT Pacific Standard Time")
#define STR_OS_ZONE_10		PKG_OS+87
	stringtext(STR_OS_ZONE_10,	"9 YST YDT Alaska Standard Time")
#define STR_OS_ZONE_11		PKG_OS+88
	stringtext(STR_OS_ZONE_11,	"3009 ZW9B ZW9B 9 1/2 hours west of GMT")
#define STR_OS_ZONE_12		PKG_OS+89
	stringtext(STR_OS_ZONE_12,	"10 HST HDT Hawaii-Aleutian Standard Time")
#define STR_OS_ZONE_13		PKG_OS+90
	stringtext(STR_OS_ZONE_13,	"11 BST BDT Bering Standard Time")
#define STR_OS_ZONE_14		PKG_OS+91
	stringtext(STR_OS_ZONE_14,	"12 ZW12 ZW12 Twelve hours west of GMT")
#define STR_OS_ZONE_15		PKG_OS+92
	stringtext(STR_OS_ZONE_15,	"-4512 ZE12C ZE12C 12 3/4 hours east of GMT")
#define STR_OS_ZONE_16		PKG_OS+93
	stringtext(STR_OS_ZONE_16,	"-12 ZE12 ZE12 Twelve hours east of GMT")
#define STR_OS_ZONE_17		PKG_OS+94
	stringtext(STR_OS_ZONE_17,	"-3011 ZE11B ZE11B 11 1/2 east of GMT")
#define STR_OS_ZONE_18		PKG_OS+95
	stringtext(STR_OS_ZONE_18,	"-11 ZE11 ZE11 Eleven hours east of GMT")
#define STR_OS_ZONE_19		PKG_OS+96
	stringtext(STR_OS_ZONE_19,	"-3010 ZE10B ZE10B 10 1/2 hours east of GMT")
#define STR_OS_ZONE_20		PKG_OS+97
	stringtext(STR_OS_ZONE_20,	"-10 ZE10 ZE10 Ten hours east of GMT")
#define STR_OS_ZONE_21		PKG_OS+98
	stringtext(STR_OS_ZONE_21,	"-3009 ZE9B ZE9B 9 1/2 hours east of GMT")
#define STR_OS_ZONE_22		PKG_OS+99
	stringtext(STR_OS_ZONE_22,	"-9 ZE9 ZE9 Nine hours east of GMT")
#define STR_OS_ZONE_23		PKG_OS+100
	stringtext(STR_OS_ZONE_23,	"-8 ZE8 ZE8 Eight hours east of GMT")
#define STR_OS_ZONE_24		PKG_OS+101
	stringtext(STR_OS_ZONE_24,	"-7 ZE7 ZE7 Seven hours east of GMT")
#define STR_OS_ZONE_25		PKG_OS+102
	stringtext(STR_OS_ZONE_25,	"-3006 ZE6B ZE6B 6 1/2 hours east of GMT")
#define STR_OS_ZONE_26		PKG_OS+103
	stringtext(STR_OS_ZONE_26,	"-6 ZE6 ZE6 Six hours east of GMT")
#define STR_OS_ZONE_27		PKG_OS+104
	stringtext(STR_OS_ZONE_27,	"-4505 ZE5C ZE5C 5 3/4 hours east of GMT")
#define STR_OS_ZONE_28		PKG_OS+105
	stringtext(STR_OS_ZONE_28,	"-3005 ZE5B ZE5B 5 1/2 hours east of GMT")
#define STR_OS_ZONE_29		PKG_OS+106
	stringtext(STR_OS_ZONE_29,	"-5 ZE5 ZE5 Five hours east of GMT")
#define STR_OS_ZONE_30		PKG_OS+107
	stringtext(STR_OS_ZONE_30,	"-3004 ZE4B ZE4B 4 1/2 hours east of GMT")
#define STR_OS_ZONE_31		PKG_OS+108
	stringtext(STR_OS_ZONE_31,	"-4 ZE4 ZE4 Four hours east of GMT")
#define STR_OS_ZONE_32		PKG_OS+109
	stringtext(STR_OS_ZONE_32,	"-3003 ZE3B ZE3B 3 1/2 hours east of GMT")
#define STR_OS_ZONE_33		PKG_OS+110
	stringtext(STR_OS_ZONE_33,	"-3 ZE3 ZE3 Three hours east of GMT")
#define STR_OS_ZONE_34		PKG_OS+111
	stringtext(STR_OS_ZONE_34,	"-2 ZE2 ZE2 Two hours east of GMT")
#define STR_OS_ZONE_35		PKG_OS+112
	stringtext(STR_OS_ZONE_35,	"-1 CET CEDT Central European Time")

#define	OS_ZONE_STRING_BASE (STR_OS_ZONE_0)
#define	OS_ZONE_STRINGS ((PKG_OS+123) - OS_ZONE_STRING_BASE)

/*** PKG_OS+77 through PKG_OS+124 are reserved for internationalization use */

#define STR_OS_DEFAULT_DSTLAW			PKG_OS+125
#ifdef INTL_ENGLISH
	stringtext(STR_OS_DEFAULT_DSTLAW, 	"3 -1 1 10 -1 1")
#else
	stringtext(STR_OS_DEFAULT_DSTLAW, 	"4 1 1 10 -1 1")
#endif

#define STR_OS_DEFAULT_TIMEZONE			PKG_OS+126
#ifdef INTL_ENGLISH
	stringtext(STR_OS_DEFAULT_TIMEZONE, 	"0")
#else
	stringtext(STR_OS_DEFAULT_TIMEZONE, 	"5")
#endif
#define STR_OS_DEFAULT_DST				PKG_OS+127
	stringtext(STR_OS_DEFAULT_DST, 		"1")
#define STR_OS_DEFAULT_FLAGS			PKG_OS+128
#ifdef INTL_ENGLISH
	stringtext(STR_OS_DEFAULT_FLAGS,        "88")
#else
	stringtext(STR_OS_DEFAULT_FLAGS, 		"48")
#endif
#define STR_OS_DEFAULT_CURRENCYDIGITS 	PKG_OS+129
	stringtext(STR_OS_DEFAULT_CURRENCYDIGITS, "2")
#define STR_OS_DEFAULT_AMSTRING 		PKG_OS+130
	stringtext(STR_OS_DEFAULT_AMSTRING, 	"AM")
#define STR_OS_DEFAULT_PMSTRING 		PKG_OS+131
	stringtext(STR_OS_DEFAULT_PMSTRING,	"PM")
#define STR_OS_DEFAULT_CURRENCYSTRING 	PKG_OS+132
#ifdef INTL_ENGLISH
	stringtext(STR_OS_DEFAULT_CURRENCYSTRING, "\x9c")
#else
	stringtext(STR_OS_DEFAULT_CURRENCYSTRING, "$")
#endif
#define STR_OS_DEFAULT_DECIMALSTRING 	PKG_OS+133
	stringtext(STR_OS_DEFAULT_DECIMALSTRING, ".")

/*	Note to translators: STR_OS_DEFAULT_DATESTRING_OS2 must not be a space! */ 
#define STR_OS_DEFAULT_DATESTRING_OS2 	PKG_OS+134
	stringtext(STR_OS_DEFAULT_DATESTRING_OS2, "-")
/*	Note to translators: STR_OS_DEFAULT_DATESTRING_WIN must not be a space! */ 
#define STR_OS_DEFAULT_DATESTRING_WIN 	PKG_OS+135
	stringtext(STR_OS_DEFAULT_DATESTRING_WIN, "/")

#define STR_OS_DEFAULT_TIMESTRING 		PKG_OS+136
	stringtext(STR_OS_DEFAULT_TIMESTRING, ":")
#define STR_OS_DEFAULT_THOUSANDSTRING 	PKG_OS+137
	stringtext(STR_OS_DEFAULT_THOUSANDSTRING, ",")
#define STR_OS_DEFAULT_YESTERDAY 		PKG_OS+138
	stringtext(STR_OS_DEFAULT_YESTERDAY, "Yesterday")
#define STR_OS_DEFAULT_TODAY 			PKG_OS+139
	stringtext(STR_OS_DEFAULT_TODAY, 	"Today")
#define ERR_OS_STD_CLOSE				PKG_OS+140
	debugtext(ERR_OS_STD_CLOSE,			"Attempt to close a standard file descriptor")
#define ERR_OS_NO_CONREAD				PKG_OS+141
	errortext(ERR_OS_NO_CONREAD,		"Cannot prompt in this environment")
#define ERR_OS_INCOMPATIBLE_HANDLES		PKG_OS+142
	internaltext(ERR_OS_INCOMPATIBLE_HANDLES, "Handles are not compatible for operation")
#define STR_OS_MAC_PREFS_FOLDER			PKG_OS+143
	stringtext(STR_OS_MAC_PREFS_FOLDER,	"Preferences:")
#define STR_OS_MAC_PREFS_FILE			PKG_OS+144
	stringtext(STR_OS_MAC_PREFS_FILE,	"Notes Preferences")
#define ERR_INITIALIZE_STATIC_MEM		PKG_OS+145
	internaltext(ERR_INITIALIZE_STATIC_MEM,"(no error, initialize statics NOW)")
#define ERR_THREADSLEFT					PKG_OS+146
	debugtext(ERR_THREADSLEFT,			"OSTerm of a process with threads still active!")
#define ERR_INTERNAL_MM					PKG_OS+147
	internaltext(ERR_INTERNAL_MM,		"(internal memory manager halt)")
#define	ERR_FILE_CLOSE_REFCNT			PKG_OS+148
	internaltext(ERR_FILE_CLOSE_REFCNT, "Attempt to close file that still has access refcnt")
#define ERR_OUTOFSEMS					PKG_OS+149
#if defined(UNIX) && !defined(OS400)
	errortext(ERR_OUTOFSEMS,			"Insufficient semaphores to handle number of concurrent owned critical regions. Please reconfigure your kernel.")
#else
	errortext(ERR_OUTOFSEMS,			"Insufficient semaphores to handle number of concurrent owned critical regions")
#endif
#define ERR_SEMCONTROL					PKG_OS+150
	errortext(ERR_SEMCONTROL,			"Unable to perform control function on semaphore")
#define ERR_MEM_ACCESS					PKG_OS+151
	internaltext(ERR_MEM_ACCESS,		"Memory object cannot be accessed")
#define ERR_HANDLES						PKG_OS+152
	errortext(ERR_HANDLES,				"Insufficient file handles available to run program")
#define ERR_POKE_FULL					PKG_OS+153
	errortext(ERR_POKE_FULL,			"Insufficient room in poke buffer")
#define ERR_POKE_NOT_EMPTY				PKG_OS+154
	errortext(ERR_POKE_NOT_EMPTY,		"Poke buffer already contains characters")
#define	ERR_OS_SEM_INVALID				PKG_OS+155
	debugtext(ERR_OS_SEM_INVALID,		"semaphore invalid or not allocated")
#define	ERR_OS_ATOM_UNALIGNED			PKG_OS+156
	debugtext(ERR_OS_ATOM_UNALIGNED,	"** WARNING ** atomic_int not aligned on natural boundary")
#define ERR_CANCEL						PKG_OS+157
	errortext(ERR_CANCEL,				"Operation stopped at your request")
#define ERR_MM_MISMATCH					PKG_OS+158
	errortext(ERR_MM_MISMATCH,			"Someone else is running a different version of Notes right now.  You cannot run 2 different versions at the same time on the same machine.")
#define ERR_NESTED_OSINIT				PKG_OS+159
	internaltext(ERR_NESTED_OSINIT,		"We've already been initialized via OSInit()")
#define ERR_TOO_MANY_PROCESSES			PKG_OS+160
	errortext(ERR_TOO_MANY_PROCESSES,	"Too many concurrent users of the Notes API package.")
#define ERR_ONE_PROCESS					PKG_OS+161
	errortext(ERR_ONE_PROCESS,			"Only one Notes-related program may be used on this computer at any given time.")
#define ERR_MEM_HANDLES					PKG_OS+162
	errortext(ERR_MEM_HANDLES,			"Maximum number of memory segments that Notes can support has been exceeded")
#define ERR_OS_LAUNCHFAIL				PKG_OS+163
	errortext(ERR_OS_LAUNCHFAIL,		"Unable to launch program")
#define ERR_OS_SHARE_NOT_LOADED		 	PKG_OS+164
	errortext(ERR_OS_SHARE_NOT_LOADED,	"SHARE.EXE not loaded")
#define ERR_NO_NOTESINI					PKG_OS+165
#ifdef MAC
	errortext(ERR_NO_NOTESINI,			"The Notes Preferences file cannot be found.")
#else
	errortext(ERR_NO_NOTESINI,			"The NOTES.INI file cannot be found on the search path (PATH)")
#endif
#define ERR_NO_MAC_NOTESINI 			PKG_OS+166
	errortext(ERR_NO_MAC_NOTESINI,		"The file 'Notes Preferences' has a damaged resource fork.")
#define ERR_OTHER_THREAD_HEAP 			PKG_OS+167
	errortext(ERR_OTHER_THREAD_HEAP,	"OSLocalAlloc/Free may only be used within a single thread!")
#define STR_OS_DEFAULT_TOMORROW 		PKG_OS+168
	stringtext(STR_OS_DEFAULT_TOMORROW, "Tomorrow")
#define ERR_BAD_QUIT_PROCESS_GROUP 		PKG_OS+169
	errortext(ERR_BAD_QUIT_PROCESS_GROUP, "Invalid process group ID")
#define ERR_PORT_IN_USE					PKG_OS+170
	errortext(ERR_PORT_IN_USE,			"This port is currently in use by another process")
#define	ERR_PORT_NOT_CONFIGURED			PKG_OS+171
	errortext(ERR_PORT_NOT_CONFIGURED,	"Communications port hardware is not configured or enabled")
#define ERR_LIBLOAD_MAX	 				PKG_OS+172
	errortext(ERR_LIBLOAD_MAX,			"Program library not loaded, exceeded maximum libraries.")
#define	ERR_SEGMENT_TOO_HUGE			PKG_OS+173
	errortext(ERR_SEGMENT_TOO_HUGE,		"Memory allocation request exceeded maximum HUGE allocation size")
#define ERR_ASSERTION_FAILED			PKG_OS+174
	debugtext(ERR_ASSERTION_FAILED,		"ASSERT(%s) failed at line %d in file %s")
#define ERR_SEMACCESS					PKG_OS+175
	errortext(ERR_SEMACCESS,			"Cannot access operating system semaphore that was created by another process")
#define ERR_STATIC_STORAGE				PKG_OS+176
	errortext(ERR_STATIC_STORAGE,		"Too many processes.  Exceeded maximum limit on static storage.")
#define ERR_CANNOT_ACCESS_SHMEM			PKG_OS+177
	errortext(ERR_CANNOT_ACCESS_SHMEM,	"Cannot attach to shared memory region, due to insufficient access (probably owned by another user or group)")
#define ERR_MAXDPOOLS_EXCEEDED			PKG_OS+178
	errortext(ERR_MAXDPOOLS_EXCEEDED,	"Exceeded maximum limit of shared memory")
#define STR_KBYTES						PKG_OS+179
	stringtext(STR_KBYTES,				"%ldK bytes")
#define STR_BYTES						PKG_OS+180
	stringtext(STR_BYTES,				"%ld bytes")
#define STR_MODIFIED_KBYTES				PKG_OS+181
	stringtext(STR_MODIFIED_KBYTES,		"%z %ldK bytes")
#define STR_MODIFIED_BYTES				PKG_OS+182
	stringtext(STR_MODIFIED_BYTES,		"%z %ld bytes")
#define STR_PRINT_SETUP					PKG_OS+183
	stringtext(STR_PRINT_SETUP,			"Print Setup")
#define	ERR_PROC_KILL					PKG_OS+184
	internaltext(ERR_PROC_KILL,			"(remove yield proc)")
#define	ERR_PROC_REPOST					PKG_OS+185
	internaltext(ERR_PROC_REPOST,		"(re-post yield proc)")
#define ERR_PTERM						PKG_OS+186
	errortext(ERR_PTERM,				"Too many PROCESS termination handlers")
#define ERR_TTERM						PKG_OS+187
	errortext(ERR_TTERM,				"Too many THREAD termination handlers")
#define ERR_OS_LAUNCHFAILMEM			PKG_OS+188
     errortext(ERR_OS_LAUNCHFAILMEM,	"There is not enough memory available to launch the application. To make more memory available, try quitting other open applications.") 
#define ERR_BUSY						PKG_OS+189
	errortext(ERR_BUSY,					"Device is Busy/In Use")
#define ERR_REGFILENAME					PKG_OS+190
	errortext(ERR_REGFILENAME,			"~notetmp.reg")
#define ERR_NOTES_ALREADY_RUNNING 		PKG_OS+191
	errortext(ERR_NOTES_ALREADY_RUNNING,"The Notes Data Directory is in use by another Notes program (possibly on another computer).  You must first shut down all Notes programs (for example, the Administration program) that share this Data Directory.")
#define ERR_SEMTIMEOUT					PKG_OS+192
	errortext(ERR_SEMTIMEOUT,			"The caller's SemWait timeout expired.")
#define ERR_CANNOT_LOAD_STRING			PKG_OS+193
	errortext(ERR_CANNOT_LOAD_STRING,	"Cannot load a needed string resource.")
#define ERR_CANNOT_CLAIM_MODULE 		PKG_OS+194
	errortext(ERR_CANNOT_CLAIM_MODULE,	"Cannot determine module handle for newly loaded library/program")
#define ERR_FILENAME_POOLFULL	 		PKG_OS+195
	errortext(ERR_FILENAME_POOLFULL,	"Insufficient memory - Filename pool is full")
#define ERR_ENV_CHANGE			 		PKG_OS+196
	errortext(ERR_ENV_CHANGE,			"%s changed to %s.")
#define ERR_ENV_CHANGE_ON_RESTART		PKG_OS+197
	errortext(ERR_ENV_CHANGE_ON_RESTART,"%s changed to %s, and will be recognized when Notes is restarted.")
#define STR_CONSOLE_END			 		PKG_OS+198
	errortext(STR_CONSOLE_END,			"Click close box to exit...")
#define ERR_INICACHE_POOL_FULL	 		PKG_OS+199
#ifdef MAC
	errortext(ERR_INICACHE_POOL_FULL,	"Insufficient memory - The Notes Preferences pool cache is full")
#else
	errortext(ERR_INICACHE_POOL_FULL,	"Insufficient memory - The NOTES.INI pool cache is full")
#endif
#define ERR_STATIC_POOLFULL				PKG_OS+200
	errortext(ERR_STATIC_POOLFULL,		"Insufficient memory - Static pool is full")
#define ERR_NO_READ_PERMS				PKG_OS+201
	errortext(ERR_NO_READ_PERMS,		"Cannot read file")
#define ERR_LOCK_FAILED					PKG_OS+202
	errortext(ERR_LOCK_FAILED,			"Cannot set file lock.  Access denied")
#define ERR_NFSLOCKD_FAILED				PKG_OS+203
	errortext(ERR_NFSLOCKD_FAILED,		"Cannot lock a remote mounted file because the nfs lock daemon is not responding")
#define ERR_MEM_NOSIZE			 		PKG_OS+204
	errortext(ERR_MEM_NOSIZE,			"Attempt to allocate 0 bytes of memory.")
#define ERR_SEM_LOCK_BOUNCED			PKG_OS+205
	errortext(ERR_SEM_LOCK_BOUNCED,		"An attempt to lock a semaphore has bounced.")
#define ERR_PATH_NAME_TOO_LONG		 	PKG_OS+206
#if defined(MAC) || defined(NLM) || defined(UNIX)
	errortext(ERR_PATH_NAME_TOO_LONG,	"The server and file name specified exceeds 256 characters.")
#else
	errortext(ERR_PATH_NAME_TOO_LONG,	"The server and file name specified exceeds 100 characters.")
#endif
#define ERR_MAC_FINDER_LOCKED			PKG_OS+207
	errortext(ERR_MAC_FINDER_LOCKED,	"Cannot perform this action because the file is locked. Unlock the file using the Finder's Get Info command.")
#define ERR_GROUP_QUIT_SUPPRESSED		PKG_OS+208
	errortext(ERR_GROUP_QUIT_SUPPRESSED, "Process group termination is currently suppressed")
#define ERR_SEMWAITER_POOLFULL			PKG_OS+209
	errortext(ERR_SEMWAITER_POOLFULL,	"Insufficient memory - Sem Waiter pool is full")
#define ERR_NO_PARTITION				PKG_OS+210
	internaltext(ERR_NO_PARTITION,		"(internal, not using partitioned servers")
#define ERR_PARTITION_NOT_SUPPORTED		PKG_OS+211
	internaltext(ERR_PARTITION_NOT_SUPPORTED, "(internal, partitioned servers not supported on this platform")
#define ERR_NO_COMMAND_LINE				PKG_OS+212
	errortext(ERR_NO_COMMAND_LINE,		"No command line supplied")
#define ERR_COMMAND_LINE_SUPPLIED		PKG_OS+213
	errortext(ERR_COMMAND_LINE_SUPPLIED,"Command line supplied")
#define ERR_IO_COMPLETE					PKG_OS+214
	errortext(ERR_IO_COMPLETE,			"Asynchronous IO event is not complete")
#define ERR_NO_ASYNC_DATA				PKG_OS+215
	errortext(ERR_NO_ASYNC_DATA,		"No asynchronous IO operation is in progress")
#define ERR_IO_PENDING					PKG_OS+216
	errortext(ERR_IO_PENDING,			"Asynchronous IO is in progress")
#define ERR_ASYNCIO_WAIT				PKG_OS+217
	errortext(ERR_ASYNCIO_WAIT,			"Unexpected result from asynchronous IO wait")
#define ERR_DELETE_USER					PKG_OS+218
	errortext(ERR_DELETE_USER,			"Net user account could not be deleted for %s")
#define ERR_NT_ACCESS_DENIED			PKG_OS+219 
	stringtext(ERR_NT_ACCESS_DENIED, "You must be a Windows NT or 2000 Administrator or Account Operator to delete or rename Windows users.")
#define ERR_NT_DC_NOT_FOUND				PKG_OS+220
	stringtext(ERR_NT_DC_NOT_FOUND, "Windows user accounts cannot be deleted or renamed.  A primary domain controller for the current Windows domain cannot be found.")
#define ERR_NT_DLL_NOT_FOUND			PKG_OS+221
	stringtext(ERR_NT_DLL_NOT_FOUND, "Netapi32.dll (Windows NT) or nadreg.dll (Windows 2000 or higher) could not be found.")
#define ERR_NT_USER_GROUP_ABSENT		PKG_OS+222
	stringtext(ERR_NT_USER_GROUP_ABSENT, "Windows account for '%s' not deleted or renamed.  The Windows user or Windows group could not be found on the current Windows domain.")
#define ERR_NT_MODIFY_FAILED			PKG_OS+223
	stringtext(ERR_NT_MODIFY_FAILED, "Windows user '%s' could not be deleted or renamed.")
#define ERR_NOT_SUPPORTED				PKG_OS+224
	errortext(ERR_NOT_SUPPORTED,		"This function is not supported on this operating system.")
#define ERR_TOO_MANY_PIPES              PKG_OS+225
	errortext(ERR_TOO_MANY_PIPES,   "Trying to create more pipes than the notes system limit.")
#define ERR_OSPIPE_TIMEOUT              PKG_OS+226
	errortext(ERR_OSPIPE_TIMEOUT,   "ospipe api function timed out.")
#define ERR_WEB_DOWNLOAD_PROMPT			PKG_OS+227
	errortext(ERR_WEB_DOWNLOAD_PROMPT,	"The feature you have chosen is not available in this version of Notes.\nTo access this feature, you will need to install the shipping version of Notes")
#define ERR_WEB_DOWNLOAD_HELP			PKG_OS+228
	errortext(ERR_WEB_DOWNLOAD_HELP,	"Help files not available in this version of Notes. To use this feature,\naccess help on a Domino server or the web, or install the shipping version of Notes.")
#define ERR_WEB_DOWNLOAD_SPELL			PKG_OS+229
	errortext(ERR_WEB_DOWNLOAD_SPELL,	"Spelling and dictionary files requested are not available in this version of Notes.\nTo access this feature, you will need to install the shipping version of Notes")

#define STR_OS_MONTHS					PKG_OS+230
	stringtext(STR_OS_MONTHS,			"Jan|Feb|Mar|Apr|May|Jun|Jul|Aug|Sep|Oct|Nov|Dec")
#define ERR_FILE_TOO_LARGE				PKG_OS+231
	errortext(ERR_FILE_TOO_LARGE,		"Attempt to extend file past supported size for platform.")
#define ERR_REGFILENAME2	  			PKG_OS+232
	errortext(ERR_REGFILENAME2,			"~notetp2.reg")

/* Use OSLoadSubString to load substrings - english total length<80! */
#define STR_CONSOLE_PROMPTS				PKG_OS+233
	stringtext(STR_CONSOLE_PROMPTS,		" (OK)| (OK/Cancel)| (Yes/No)| (Yes/No/Cancel)| (Retry/Cancel)| ")
#define IDX_OK			1
#define IDX_OKCANCEL	2
#define IDX_YESNO		3
#define IDX_YESNOCANCEL	4
#define IDX_RETRYCANCEL	5
#define IDX_DEFAULT		6

#define STR_CONSOLE_REPLIES				PKG_OS+234
	stringtext(STR_CONSOLE_REPLIES,		"O|C|Y|N|R")
#define IDX_OK			1
#define IDX_CANCEL		2
#define IDX_YES			3
#define IDX_NO			4
#define IDX_RETRY		5

#define ERR_MEMALLOC_BPOOL_POOLFULL	 	PKG_OS+235
	errortext(ERR_MEMALLOC_BPOOL_POOLFULL,		"Insufficient memory - OSMemAlloc BPOOL is full")

#define ERR_DUPLICATE_DIRLINK			PKG_OS+236
	errortext(ERR_DUPLICATE_DIRLINK,		"Unable to register directory link %s. Directory link %s for path %s already exists. Please remove one of the duplicate links.")

#define ERR_USER_ROOT				PKG_OS+237
	errortext(ERR_USER_ROOT, 		"Do not run Domino as root.  Domino exiting\n")
#define ERR_SYSREQ					PKG_OS+238
	errortext(ERR_SYSREQ, 		"Warning!! Current file descriptor limit %d is too low.|Refer to System Requirements documentation.")
#define SR_IDX_FDLIM	1
#define SR_IDX_DOC	2
#define SR_IDX_MAX	2	/* maximum substring index */
#define ERR_FAULT_RECOVERY1			PKG_OS+239
	errortext(ERR_FAULT_RECOVERY1,	"Fault recovery is in progress|Termination is in progress|Too many crashes, restart disabled|Running cleanup script|Terminating tasks|Warning! Failed to allocate message queue resource; post-crash/fault recovery shutdown system disabled.")
#define FR_IDX_FRSTARTED	1
#define FR_IDX_TRMSTARTED	2
#define FR_IDX_NORESTART	3
#define FR_IDX_CLNUPSCRIPT	4
#define FR_IDX_TERMTASKS	5
#define FR_IDX_DISABLE		6
#define FR_IDX_MAX1			6	/* maximum substring index */
#define ERR_FAULT_RECOVERY2			PKG_OS+240
	errortext(ERR_FAULT_RECOVERY2,	"Freeing resources|Recovery error|Fault recovery completed|Termination completed|Domino Halted. Type 'quit' to Terminate/Cleanup |quit|Shutdown system disabled, use 'nsd -kill' to terminate")
#define FR_IDX_FREERES		1
#define FR_IDX_ERROR		2
#define FR_IDX_FRCOMPLETED	3
#define FR_IDX_TRMCOMPLETED	4
#define FR_IDX_TERMCLEANUP	5
#define FR_IDX_QUIT			6
#define FR_IDX_USENSD		7
#define FR_IDX_MAX2			7	/* maximum substring index */
#define ERR_FAULT_RECOVERY3			PKG_OS+241
	errortext(ERR_FAULT_RECOVERY3,	"Server Restart Notification|Fault Recovery Notification: Server %s was restarted after a fault on %s|Running NSD")
#define FR_IDX_SUBJ	1
#define FR_IDX_BODY	2
#define FR_IDX_RUNNSD	3
#define FR_IDX_MAX3	3	/* maximum substring index */

#define ERR_OSPIPE_READ_WOULDBLOCK      PKG_OS+242	/* from 4.61 */
	errortext(ERR_OSPIPE_READ_WOULDBLOCK,   "ospipe read would block, pipe is empty.")
#define ERR_BAD_DIRECTORY				PKG_OS+243
	errortext(ERR_BAD_DIRECTORY, 		"Invalid directory name or device not ready.")
#define ERR_SERVER_RESTART				PKG_OS+244
	internaltext(ERR_SERVER_RESTART, 	"Restart the server")
#define ERR_SHMEM_NOT_FOUND				PKG_OS+245
	errortext(ERR_SHMEM_NOT_FOUND,		"Cannot attach to shared memory region.  Region was deallocated due to abnormal termination of another process.")
#define ERR_STACKWALK_FAILED			PKG_OS+246
	debugtext(ERR_STACKWALK_FAILED,		"stack walk failed")
#define ERR_DIR_NOT_EMPTY				PKG_OS+247
	errortext(ERR_DIR_NOT_EMPTY,		"Folder is not empty")
#define ERR_CANNOT_LINK_TO_DATADIR		PKG_OS+248
	errortext(ERR_CANNOT_LINK_TO_DATADIR,	"This link points within the existing data directory tree")
#define ERR_LINK_DEST_NO_EXIST			PKG_OS+249
	errortext(ERR_LINK_DEST_NO_EXIST,	"Destination path does not exist")
#define ERR_SERVER_MEMORY				PKG_OS+250
	errortext(ERR_SERVER_MEMORY,		"Insufficient memory.")

/*	!!!!!!!!!!Note - In the 5.0x stream, messages 251-254 were used for different things and not moved here so we have
	an inconsistency which needs to be cleaned up.  Unfortunately, since we're at the end of available OS error codes
	this isn't trivial.  We need to do reviews of all the error code files to find these problems before 6.0 FCS.  RLH 11/21/2001 */

#define ERR_OSMESSAGE_IGNORE			PKG_OS+251
	internaltext(ERR_OSMESSAGE_IGNORE,		"(internal, used by message signal handler")
#define ERR_OSMESSAGE_ABORT				PKG_OS+252
	internaltext(ERR_OSMESSAGE_ABORT,		"(internal, used by message signal handler")
#define ERR_BAD_PATH					PKG_OS+253
	errortext(ERR_BAD_PATH,				"The specified path is invalid")
#define ERR_PIDFILE						PKG_OS+254
	errortext(ERR_PIDFILE,				"Error writing to process file %s, terminate other applications which are accessing this file")
#define ERR_FILE_NOT_LINK				PKG_OS+255
	errortext(ERR_FILE_NOT_LINK,		"File is not a link")

/* PKG_OS goes to 255 */

#endif

