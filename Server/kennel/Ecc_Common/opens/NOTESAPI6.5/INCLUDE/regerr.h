#ifndef REG_ERR_DEFS
#define REG_ERR_DEFS

/* 	Returned errors	------------------------------------------------------ */


#define	ERR_REG_INC_CONTEXT				PKG_REG + 1
	apitext(ERR_REG_INC_CONTEXT, "Insufficient information supplied for requested registration function.")

#define	ERR_REG_ADDRBOOK_ENTRY_EXISTS	PKG_REG + 2
	apitext(ERR_REG_ADDRBOOK_ENTRY_EXISTS, "Unable to update existing document in your Domino Directory")

#define ERR_REG_USERID_EXISTS 			PKG_REG + 3
	apitext(ERR_REG_USERID_EXISTS, "Unable to modify existing User ID file")

#define ERR_REG_BEGIN_CERTIFYING		PKG_REG + 4
	stringtext(ERR_REG_BEGIN_CERTIFYING, "Begin certifying %A...") 

#define ERR_REG_NO_MAILFILE_CREATED 	PKG_REG + 5
	errortext(ERR_REG_NO_MAILFILE_CREATED, "Mail file for %A could not be created")

#define ERR_REG_BEGIN_REGISTERING		PKG_REG + 6
	stringtext(ERR_REG_BEGIN_REGISTERING, "Begin registering %A...")

#define ERR_REG_CREATINGNEWMAIL			PKG_REG + 7
	stringtext(ERR_REG_CREATINGNEWMAIL, "Creating mail file for %A...")

#define ERR_REG_ADDIDTOBOOK				PKG_REG + 8
	stringtext(ERR_REG_ADDIDTOBOOK, "Adding %A to the Domino Directory...")

#define ERR_REG_DONE_REGISTERING		PKG_REG + 9
	stringtext(ERR_REG_DONE_REGISTERING, "%A successfully registered.")

#define	ERR_REG_LOG_ADDRBOOK_EXISTS		PKG_REG + 10
	stringtext(ERR_REG_LOG_ADDRBOOK_EXISTS, "A Domino Directory entry for %A already exists.")

#define ERR_REG_LOG_USERID_EXISTS 		PKG_REG + 11
	stringtext(ERR_REG_LOG_USERID_EXISTS, "UserID file %p already exists.")

#define ERR_REG_LOG_NO_PASSWORD 		PKG_REG + 12
	stringtext(ERR_REG_LOG_NO_PASSWORD, "No password supplied for %s.")

#define ERR_REG_BEGIN_IDFILE_CREATE		PKG_REG + 13
	stringtext(ERR_REG_BEGIN_IDFILE_CREATE, "Creating ID for %A...")

#define ERR_REG_TOO_MANY_ENTRIES		PKG_REG + 14
	errortext(ERR_REG_TOO_MANY_ENTRIES, "The name cannot be registered.  More than one entry was found in the Domino Directory with this name")

#define ERR_REG_DONE_CERTIFYING			PKG_REG + 15
	stringtext(ERR_REG_DONE_CERTIFYING, "Done certifying %A...") 

#define ERR_REG_NO_IDFILE_NAME			PKG_REG + 16
	stringtext(ERR_REG_NO_IDFILE_NAME,	"No ID file name specified") 

#define ERR_REG_NO_MAIL_INFO			PKG_REG + 17
	stringtext(ERR_REG_NO_MAIL_INFO,	"No mail server or mail database name specified") 

#define ERR_REG_NO_LASTNAME				PKG_REG + 18
	stringtext(ERR_REG_NO_LASTNAME,		"No last name specified") 

#define ERR_REG_NO_SERVER_NAME			PKG_REG + 19
	stringtext(ERR_REG_NO_SERVER_NAME,	"No server name specified") 

#define ERR_REG_NO_CERTIFIER			PKG_REG + 20
	stringtext(ERR_REG_NO_CERTIFIER,	"No certifier specified") 

#define ERR_REG_NO_PASSWORD				PKG_REG + 21
	stringtext(ERR_REG_NO_PASSWORD,		"No password specified") 

#define ERR_REG_OPEN_CERTLOG		 	PKG_REG + 22
	stringtext(ERR_REG_OPEN_CERTLOG,	"Could not open the certifier log") 

#define ERR_REG_ACCESS_CERTLOG			PKG_REG + 23
	stringtext(ERR_REG_ACCESS_CERTLOG,	"You are not allowed to update the certifier log") 

#define ERR_REG_UPDATE_CERTLOG		 	PKG_REG + 24
	stringtext(ERR_REG_UPDATE_CERTLOG,	"Error attempting to update the certifier log") 

#define ERR_REG_OURSERVERS				PKG_REG + 25
	stringtext(ERR_REG_OURSERVERS,		"LocalDomainServers")

#define STR_REG_UPDATING_CERTLOG		PKG_REG + 26
	stringtext(STR_REG_UPDATING_CERTLOG,"Updating Certification Log")

#define ERR_REG_USER_NOT_FOUND 			PKG_REG + 27
	errortext(ERR_REG_USER_NOT_FOUND,"User or server name not found in Domino Directory")

#define ERR_REG_IDFILE_NOT_FOUND		PKG_REG + 28
	errortext(ERR_REG_IDFILE_NOT_FOUND,"ID file not found in Domino Directory")

#define	ERR_REG_NOSUCHUSER				PKG_REG + 29
	errortext(ERR_REG_NOSUCHUSER,		"User name not found in Domino Directory")

#define	ERR_REG_NOTUNIQUE				PKG_REG + 30
	errortext(ERR_REG_NOTUNIQUE,		"Multiple users with same name found in Domino Directory")

#define	ERR_REG_NOSUCHSERVER	   		PKG_REG + 31
	errortext(ERR_REG_NOSUCHSERVER,		"Server name not found in Domino Directory")

#define	ERR_REG_SERVERNOTUNIQUE			PKG_REG + 32
	errortext(ERR_REG_SERVERNOTUNIQUE,	"Multiple servers with the same name found in Domino Directory")

#define STR_REG_NETWARE_NAME			PKG_REG + 33
	internaltext(ERR_REG_NETWARE_NAME, "NetWare")

#define ERR_REG_NO_STORED_CERTKEY		PKG_REG + 34
	errortext(ERR_REG_NO_STORED_CERTKEY, "The Domino Directory entry does not include a certified public key")

#define ERR_REG_NO_SRV_VIEW				PKG_REG + 35
	errortext(ERR_REG_NO_SRV_VIEW, "The Domino Directory you are registering the server to does not contain a $Servers view.  In order to register a server you must use a real Domino Directory.  You can replace the design of the Address Book you are using with the Domino Directory template.")

#define ERR_REG_LOG_PASSWORD_ADDRESSBOOK	PKG_REG + 36
	stringtext(ERR_REG_LOG_PASSWORD_ADDRESSBOOK, "No password supplied for %s (required for ID files to be stored in the Domino Directory).")

#define ERR_REG_LOG_MINPSWDCHARS		PKG_REG + 37
	stringtext(ERR_REG_LOG_MINPSWDCHARS, "You must supply the minimum number of characters required for a password for %s.")

#define ERR_REG_CREATING_ADMINREQTOCREATEMAIL	PKG_REG + 38
	stringtext(ERR_REG_CREATING_ADMINREQTOCREATEMAIL, "Creating administration request to create mail file for %A...")

#define ERR_REG_NO_ADMINREQTOCREATEMAIL 	PKG_REG + 39
	errortext(ERR_REG_NO_ADMINREQTOCREATEMAIL, "The administration request could not be created: %s")

#define ERR_REG_NO_ADMINPROCESS 	PKG_REG + 40
	errortext(ERR_REG_NO_ADMINPROCESS, "The administration process has not been initialized.")

#define ERR_REG_CREATEMAILSETUP			PKG_REG + 41
	stringtext(ERR_REG_CREATEMAILSETUP, "Mail file for %A will be created during user setup.")

#define ERR_REG_NT_PASSWORD_SIZE		PKG_REG + 42
	stringtext(ERR_REG_NT_PASSWORD_SIZE, "The NT user '%s' could not be added.  The Notes password violated the minimum or maximum NT password length.")

#define ERR_REG_NT_ACCESS_DENIED			PKG_REG + 43
	stringtext(ERR_REG_NT_ACCESS_DENIED, "You must be an NT Administrator or Account Operator to add NT users or groups.")

#define ERR_REG_NT_DC_NOT_FOUND			PKG_REG + 44
	stringtext(ERR_REG_NT_DC_NOT_FOUND, "The NT account(s) cannot be added.  A primary domain controller for the current NT domain cannot be found.")

#define ERR_REG_NT_DLL_NOT_FOUND		PKG_REG + 45
	stringtext(ERR_REG_NT_DLL_NOT_FOUND, "%s could not be found.  If available, place it in the Windows system directory (SYSTEM32).")

#define ERR_REG_NT_USER_GROUP_ABSENT		PKG_REG + 46
	stringtext(ERR_REG_NT_USER_GROUP_ABSENT, "The NT account for '%s' could not be added.  The NT user or group could not be found on the current NT domain.")

#define ERR_REG_NT_REG_FAILED		PKG_REG + 47
	stringtext(ERR_REG_NT_REG_FAILED, "The NT account for '%s' could not be added.")

#define ERR_REG_NT_USER_EXISTS		PKG_REG + 48
	stringtext(ERR_REG_NT_USER_EXISTS, "The NT account for '%s' could not be added; an NT user with the same Username already exists.")

#define ERR_REG_ADDCERTIFICATETOBOOK	PKG_REG + 49
	stringtext(ERR_REG_ADDCERTIFICATETOBOOK, "Adding the certificate to the Domino Directory...")

#define STR_REG_SETUP_NETWORK_NAME		(PKG_REG+50)
	stringtext(STR_REG_SETUP_NETWORK_NAME, "Network1")

/*	NOTE:  PKG_REG+51 through PKG_REG+151 are used by the setup program
			to provide network choices.

	The format of the stringtext is five fields separated by | (vertical bar):
		(1) List of platform names on which support is provided separated by
			; (semi-colons) -- must match platform name in global.h EXACTLY
		(2) Flags defined below (WORKSTATION_SUPPORT, etc.)
		(3) PortName in Ports= or DisabledPorts= line in notes.ini
		(4) Notes.ini PortName variable, specifying DLL name, unit number,
			number of simultaneous sessions, and buffer size in bytes
		(5) Dialog box description in Setup (TRANSLATE THIS ONE ONLY!).

	The order of the list of choices during setup for a given platform is
	determined by the order they are in the strings.  
		
	Network support may also be added by having a similar string in a
	text file which is shipped with the given network driver (like SNA).
	These networks will be at the end of the list.

	DO NOT USE PKG_REG+51 through PKG_REG+151!

*/

/************************** SETUP Network resource strings *****************/

#define NUMNETWORKSADDED	50
#define MAXDLGSTRING		50
#define MAXDLLNAME			9

#define WORKSTATION_SUPPORT 	0x00000001
#define SERVER_SUPPORT			0x00000002
#define PHONENUMBER_SUPPORT		0x00000004	/* Used for SNA support */
#define MODEM_SUPPORT			0x00000008

#define	PKG_SETUP				PKG_REG+51

/*	The following template is used so we can dynamically add the port name, unit
	number and driver name after querying the system.
*/
#define SETUP_XPC_TEMPLATE	PKG_SETUP
	stringtext(SETUP_XPC_TEMPLATE, "Macintosh|13|%s|XPC,%d,15,0,%s!%s|%s")

/* 	Note the strings, AppleTalk and VINES, in the port name are checked in 
	net\netsetup.c to exclude them from Win95 server setup selection.  If 
	you change the name here, change it there in subroutine, PutEntryInTable.
*/


#define SETUP_NETWORK_FIRST		(PKG_SETUP+10)
#define SETUP_NETWORK_LAST		(PKG_SETUP+100)

	stringtext(SETUP_NETWORK_FIRST+0, "Windows/16|1|TCP|TCP, 0, 15, 0|TCP/IP")
	stringtext(SETUP_NETWORK_FIRST+1, "OS/2v2|3|TCPIP|TCP, 0, 15, 0|TCP/IP")
	stringtext(SETUP_NETWORK_FIRST+2, "Windows/32;UNIX;Netware;OS/400|3|TCPIP|TCP, 0, 15, 0|TCP/IP")	

#ifdef OBSOLETE_IN_V6
	stringtext(SETUP_NETWORK_FIRST+3, "Windows/16|1|VINES|VINES, 0, 15, 0|Banyan VINES")
	stringtext(SETUP_NETWORK_FIRST+4, "OS/2v2;Windows/32|3|VINES|VINES, 0, 15, 0|Banyan VINES")
#endif

	stringtext(SETUP_NETWORK_FIRST+3, "Macintosh|1|TCP/IP|TCP, 0, 15, 0|TCP/IP")

#ifdef OBSOLETE_IN_V6
	stringtext(SETUP_NETWORK_FIRST+6, "Windows/32;OS/2v2;Netware|3|SPX|NWSPX, 0, 15, 0|NetWare SPX")
#else
	stringtext(SETUP_NETWORK_FIRST+4, "Windows/32;Netware|3|SPX|NWSPX, 0, 15, 0|NetWare SPX")
#endif

	stringtext(SETUP_NETWORK_FIRST+5, "Windows/16|1|SPX|NWSPX, 0, 15, 0|NetWare SPX")
	stringtext(SETUP_NETWORK_FIRST+6, "OS/2v2|3|IBM_LAN0|IBMEENB, 0, 50, 0|IBM EE LAN Requestor")

#ifdef OBSOLETE_IN_V6
	stringtext(SETUP_NETWORK_FIRST+9, "OS/2v2|2|AppleTalk|ATALK, 0, 15, 0|AppleTalk")
	stringtext(SETUP_NETWORK_FIRST+10, "Windows/32;Netware|2|AppleTalk|ATALK, 0, 15, 0|AppleTalk")
#endif

	stringtext(SETUP_NETWORK_FIRST+7, "Windows/32;Windows/16;OS/2v2;Netware|15|COM1|XPC, 1, 15, 0|COM1")
	stringtext(SETUP_NETWORK_FIRST+8, "Windows/32;Windows/16;OS/2v2;Netware|15|COM2|XPC, 2, 15, 0|COM2")
	stringtext(SETUP_NETWORK_FIRST+9, "Windows/32;Windows/16;OS/2v2;Netware|15|COM3|XPC, 3, 15, 0|COM3")
	stringtext(SETUP_NETWORK_FIRST+10, "Windows/32;Windows/16;OS/2v2;Netware|15|COM4|XPC, 4, 15, 0|COM4")
	stringtext(SETUP_NETWORK_FIRST+11, "Windows/32;Windows/16;OS/2v2;Netware|15|COM5|XPC, 5, 15, 0|COM5")
	stringtext(SETUP_NETWORK_FIRST+12, "UNIX|15|Serial1|XPC, 1, 15, 0|Serial1")
	stringtext(SETUP_NETWORK_FIRST+13, "UNIX|15|Serial2|XPC, 2, 15, 0|Serial2")

#ifdef OBSOLETE_IN_V6
	stringtext(SETUP_NETWORK_FIRST+16, "UNIX|3|SPX|SPX, 0, 15, 0|SPX")
#endif

	stringtext(SETUP_NETWORK_FIRST+14, "Windows/32;Windows/16;OS/2v2|3|LAN0|NETBIOS, 0, 15, 0|NetBIOS (NetWare, LAN Manager, etc.)")	
	stringtext(SETUP_NETWORK_FIRST+15, "Windows/32;Windows/16;OS/2v2|3|LAN1|NETBIOS, 1, 15, 0|NetBIOS (NetWare, LAN Manager, etc.)")	
	stringtext(SETUP_NETWORK_FIRST+16, "Windows/32;Windows/16;OS/2v2|3|LAN2|NETBIOS, 2, 15, 0|NetBIOS (NetWare, LAN Manager, etc.)")	
	stringtext(SETUP_NETWORK_FIRST+17, "Windows/32;Windows/16;OS/2v2|3|LAN3|NETBIOS, 3, 15, 0|NetBIOS (NetWare, LAN Manager, etc.)")	
	stringtext(SETUP_NETWORK_FIRST+18, "Windows/32;Windows/16;OS/2v2|3|LAN4|NETBIOS, 4, 15, 0|NetBIOS (NetWare, LAN Manager, etc.)")	
	stringtext(SETUP_NETWORK_FIRST+19, "Windows/32;Windows/16;OS/2v2|3|LAN5|NETBIOS, 5, 15, 0|NetBIOS (NetWare, LAN Manager, etc.)")	
	stringtext(SETUP_NETWORK_FIRST+20, "Windows/32;Windows/16;OS/2v2|3|LAN6|NETBIOS, 6, 15, 0|NetBIOS (NetWare, LAN Manager, etc.)")	
	stringtext(SETUP_NETWORK_FIRST+21, "Windows/32;Windows/16;OS/2v2|3|LAN7|NETBIOS, 7, 15, 0|NetBIOS (NetWare, LAN Manager, etc.)")	
	stringtext(SETUP_NETWORK_FIRST+22, "Windows/32;Windows/16;OS/2v2|3|LAN8|NETBIOS, 8, 15, 0|NetBIOS (NetWare, LAN Manager, etc.)")	

/*	NOTE:  PKG_REG+51 through PKG_REG+151 are used by the setup program
			to provide network choices.

	DO NOT USE PKG_REG+51 through PKG_REG+151!
*/

/* Minimal setup hardcodes network setup to TCP (first entry) */
#define SETUP_NETWORK_TCPIP	0

#define STR_REG_NEWSETUP_NETWORK_SUFFIX			(PKG_REG+152)
	stringtext(STR_REG_NEWSETUP_NETWORK_SUFFIX,		"Network")
#define	STR_REG_ADDRESSBOOK_TITLE_STRING		(PKG_REG+153)
	stringtext(STR_REG_ADDRESSBOOK_TITLE_STRING,	"%a's Address Book")
#define ERR_REG_OTHERSERVERS					(PKG_REG+154)
	stringtext(ERR_REG_OTHERSERVERS,			"OtherDomainServers")

	 /* AVAILABLE:  PKG_REG+155 */

#define STR_REG_SETUP_ADDSERVER					(PKG_REG+156)
	stringtext(STR_REG_SETUP_ADDSERVER, 		"Setting Up Additional Domino Server")
#define STR_REG_SETUP_FIRSTSERVER				(PKG_REG+157)
	stringtext(STR_REG_SETUP_FIRSTSERVER, 		"Setting Up First Domino Server")
#define	ERR_REG_NOMODEMFILES					(PKG_REG+158)
	errortext(ERR_REG_NOMODEMFILES,				"No modem files are available on your system.")
#define ERR_REG_CERT_NAME_TOO_SHORT				(PKG_REG+159)
	errortext(ERR_REG_CERT_NAME_TOO_SHORT, 		"Certifier names must be at least three characters long.")
#define ERR_REG_PHONEREQUIRED					(PKG_REG+160)
	stringtext(ERR_REG_PHONEREQUIRED, 			"You must enter a phone number.")
#define ERR_REG_CERTREQUIRED					(PKG_REG+161)
	stringtext(ERR_REG_CERTREQUIRED, 			"You must enter a filename for the certifier ID.")
#define ERR_REG_USERREQUIRED					(PKG_REG+162)
	stringtext(ERR_REG_USERREQUIRED, 			"You must enter a filename for the user ID.")
#define ERR_REG_SERVERREQUIRED					(PKG_REG+163)
	stringtext(ERR_REG_SERVERREQUIRED, 			"You must enter a filename for the server ID.")
#define STR_REG_CONVERTING_MAILFILE				(PKG_REG+164)
	stringtext(STR_REG_CONVERTING_MAILFILE,  	"Registration completed.  Now converting mail file for %s...")
#define ERR_REG_STR_IDCHARS						(PKG_REG+165)
	stringtext(ERR_REG_STR_IDCHARS,				"&-. _")
#define STR_REG_DOMAINCHARS						(PKG_REG+166)
	stringtext(STR_REG_DOMAINCHARS,				"&-. _")
#define ERR_REG_UPDATING_ENTRY					(PKG_REG+167)
	errortext(ERR_REG_UPDATING_ENTRY,			"Updating Domino Directory entry for %A")
#define ERR_REG_UPGRADETEMPLATES				(PKG_REG+168)
	errortext(ERR_REG_UPGRADETEMPLATES,			"Upgrading the design of your databases")
#define ERR_REG_LAUNCH_SETUPDB					(PKG_REG+169)
	errortext(ERR_REG_LAUNCH_SETUPDB,			"You must complete server setup via the Domino setup database.")
#define ERR_REG_INITIALIZATION					(PKG_REG+170)
	stringtext(ERR_REG_INITIALIZATION,			"Setting default information in preferences file...")
#define ERR_REG_LANG_TAG_MISSING_FROM_CERT		(PKG_REG+171)
    errortext(ERR_REG_LANG_TAG_MISSING_FROM_CERT, "The alternate language specified for %s was not contained in the new certifier specified.  Certifier changes were not made for this user.")
#define ERR_REG_CERT_NOT_UPDATED				(PKG_REG+172)
    errortext(ERR_REG_CERT_NOT_UPDATED, 		"The certifier was not changed for this user; the existing alternate language for this user did not exist in the new certifier specified.")
#define ERR_REG_UPGRADENAMES					(PKG_REG+173)
	errortext(ERR_REG_UPGRADENAMES,				"Do you want to upgrade the design of the Domino Directory? This replaces the standard forms and views with ones from the template.")
#define ERR_REG_DO_SETUP						(PKG_REG+174)
	internaltext(ERR_REG_DO_SETUP,				"do client setup")
#define STR_REG_RLAN_RAS_STATIC					(PKG_REG+175)
	stringtext(STR_REG_RLAN_RAS_STATIC,			"RAS$%^Remote connection name:")
#define STR_REG_RLAN_ARA_STATIC					(PKG_REG+176)
	stringtext(STR_REG_RLAN_ARA_STATIC,			"ARA$%^Connection document location:")
#define STR_REG_RLAN_RAS_DRIVER					(PKG_REG+177)
	stringtext(STR_REG_RLAN_RAS_DRIVER,			"Microsoft Dial-Up Networking|RAS")
#define IDX_REG_RLAN_RAS_DISPLAY			1
#define IDX_REG_RLAN_RAS_SERVICE			2

#define STR_REG_RLAN_ARA_DRIVER					(PKG_REG+178)
	stringtext(STR_REG_RLAN_ARA_DRIVER,			"AppleTalk Remote Access|ARA")
#define IDX_REG_RLAN_ARA_DISPLAY			1
#define IDX_REG_RLAN_ARA_SERVICE			2

#define STR_REG_REPORTS_TITLE					(PKG_REG+179)
	stringtext(STR_REG_REPORTS_TITLE,			"Reports for")
#define ERR_REG_EVENTS4_SAVED					(PKG_REG+180)
	errortext(ERR_REG_EVENTS4_SAVED,			"Your current Statistics & Events database has been copied to %p. The database %p will be upgraded when the Event task runs and old messages will be deleted.")
#define ERR_REG_MULT_LANG_TAGS_MISSING			(PKG_REG+181)
    errortext(ERR_REG_MULT_LANG_TAGS_MISSING, 	"The new certifier specified does not contain the alternate language(s) set for one or more selected users.  The certifier will not be changed for these users.")
#define ERR_REG_NOT_PRIMARY_DIRECTORY			(PKG_REG+182)
    errortext(ERR_REG_NOT_PRIMARY_DIRECTORY, 	"A duplicate full or short name was found for this user in a non-primary directory (%a).  Full and short user names must be unique to be registered.")
#define ERR_REG_SKIPPED_TASK					(PKG_REG+183)
	errortext(ERR_REG_SKIPPED_TASK,				"Default server task %s will NOT be added to %s in NOTES.INI because %s is set.")
#define ERR_REG_PERSONAL_SUBDIR_NOT_UNIQUE		(PKG_REG+184)
    errortext(ERR_REG_PERSONAL_SUBDIR_NOT_UNIQUE, "A personal sub-directory could not be created for this roaming user.  A sub-directory with the same name already exists.")
#define ERR_REG_PERSONAL_SUBDIR_NO_ACCESS		(PKG_REG+185)
    errortext(ERR_REG_PERSONAL_SUBDIR_NO_ACCESS, "A personal sub-directory could not be created for this roaming user.  Create access was denied at the file system level.")
#define STR_REG_CREATINGNEWPERSONALNAB	   		(PKG_REG+186)
	stringtext(STR_REG_CREATINGNEWPERSONALNAB, 	"Creating personal address book file for %A...")
#define STR_REG_CREATINGNEWBOOKMARKDB			(PKG_REG+187)
	stringtext(STR_REG_CREATINGNEWBOOKMARKDB, 	"Creating bookmark file for %A...")
#define STR_REG_JOURNAL_DB_TITLE				(PKG_REG+188)
	stringtext (STR_REG_JOURNAL_DB_TITLE, 		"Personal Journal")

#ifdef PUT_ROAMING_HEADLINE_BACK
#define STR_REG_CREATINGNEWHEADLINEDB		  	(PKG_REG+188)
	stringtext(STR_REG_CREATINGNEWHEADLINEDB,	"Creating headline file for %A...")
#endif

#define STR_REG_CREATINGNEWJOURNALDB		  	(PKG_REG+189)
	stringtext(STR_REG_CREATINGNEWJOURNALDB,	"Creating personal journal file for %A...")

#define ERR_REG_BTREE_NO_EXT_INFO			  	(PKG_REG+190)
	errortext(ERR_REG_BTREE_NO_EXT_INFO,		"Entry contains no extended information")
#define ERR_REG_CANCEL_ALL					  	(PKG_REG+191)
	errortext(ERR_REG_CANCEL_ALL,				"Cancel migration for all entries")
#define ERR_REG_WIN95_ENTRIES_LIMIT			  	(PKG_REG+192)
	errortext(ERR_REG_WIN95_ENTRIES_LIMIT,		"Upgrade application exceeded the displayable users/groups limit for MS Windows 95/98. Please use MS Windows NT/2000 to run this upgrade application.")
#define STR_DOLADMIN_TITLE						(PKG_REG+193)
	stringtext(STR_DOLADMIN_TITLE,				"Offline Services")
#define ERR_REG_SERVER_CONFIGURE_ASP			(PKG_REG+194)
	stringtext(ERR_REG_SERVER_CONFIGURE_ASP,	"Configuring Application Service Provider Environment...")
#define ERR_REG_SETTING_ACLS					(PKG_REG+195)
	stringtext(ERR_REG_SETTING_ACLS,			"Configuring default ACLs for all databases...")
#define ERR_REG_COMPACT_NAB						(PKG_REG+196)
	stringtext(ERR_REG_COMPACT_NAB,				"Compacting your Domino Directory...")
#define ERR_REG_UPGRADENAMES_ERR 				(PKG_REG+197)
	errortext(ERR_REG_UPGRADENAMES_ERR,			"Error upgrading the design of your Domino Directory.  Continue with server upgrade?")

/*	198 - 208 reserved for cross-launching support. Since Mac file types/creators
	can contain spaces, whitespace is NOT ignored in these strings. Only use a space
	where a filetype/creator needs it! The wildcard character '?' is supported in
	file extensions.
*/

#define ERR_XLAUNCH_WP					(PKG_REG+198)
	stringtext(ERR_XLAUNCH_WP,			"WPC2,.WP?,.WP5")
#define ERR_XLAUNCH_WORD				(PKG_REG+199)
	stringtext(ERR_XLAUNCH_WORD,		"MSWD,.DOC,TEXT,.RTF,TEXT")
#define ERR_XLAUNCH_EXCEL				(PKG_REG+200)
	stringtext(ERR_XLAUNCH_EXCEL,		"XCEL,.XLC,XLC ,.XLS,TEXT")
#define ERR_XLAUNCH_123					(PKG_REG+201)
	stringtext(ERR_XLAUNCH_123,			"L123,.WK?,LWKS,.WK1,LWKS,.WK3,LWKS")
#define ERR_XLAUNCH_TEXT				(PKG_REG+202)
	stringtext(ERR_XLAUNCH_TEXT,		"ttxt,.TXT,TEXT,.00?,TEXT")
#define ERR_XLAUNCH_MOVIE				(PKG_REG+203)
	stringtext(ERR_XLAUNCH_MOVIE,		"TVOD,.MOV,Moov")
#define ERR_XLAUNCH_PPOINT				(PKG_REG+204)
	stringtext(ERR_XLAUNCH_PPOINT,		"PPT3,.PPT,SLD3")
#define ERR_XLAUNCH_ACROBAT				(PKG_REG+205)
	stringtext(ERR_XLAUNCH_ACROBAT,		"CARO,.PDF,TEXT")
#define ERR_XLAUNCH_PHOTOSHOP			(PKG_REG+206)
	stringtext(ERR_XLAUNCH_PHOTOSHOP,	"8BIM,.PDS,8BPS,.TIF,TIFF,.GIF,GIFf")
#define ERR_XLAUNCH_STUFFIT_EXPANDER	(PKG_REG+207)
	stringtext(ERR_XLAUNCH_STUFFIT_EXPANDER,	"SITx,.SEA,TEXT,.SIT,TEXT,.HQX,TEXT,.BIN,TEXT")
	
#define ERR_REG_INETADDR_NOT_UNIQUE				(PKG_REG+209)
	errortext(ERR_REG_INETADDR_NOT_UNIQUE,		"The internet address for this user is not unique.")
#define ERR_REG_DUSFULLNAME_CONFLICT			(PKG_REG+210)
	errortext(ERR_REG_DUSFULLNAME_CONFLICT,		"Different Domino Directory matches were found for the Notes hierarchical name check and the migrating user name check. No Domino Directory entry can be overwritten.")

/*	211 - 230 reserved for Domino Upgrade Application (DUS) support.
	(migrating foreign directory users) */
#define ERR_DUS_EXTENDED_ERROR 	(PKG_REG+211)
	errortext(ERR_DUS_EXTENDED_ERROR, 	"The upgrade application '%s' returned this error: %s.")

#define ERR_DUS_NO_MORE_USERS		(PKG_REG+212)
	errortext(ERR_DUS_NO_MORE_USERS,	"No more people available from the current upgrade application.")
#define ERR_DUS_NO_MORE_GROUPS		(PKG_REG+213)
	errortext(ERR_DUS_NO_MORE_GROUPS,	"No more groups available from the current upgrade application.")
#define ERR_DUS_NO_USERS		 	(PKG_REG+214)
	errortext(ERR_DUS_NO_USERS,	 	"No people are available from the current upgrade application.")
#define ERR_DUS_CONTEXT_CORRUPT  	(PKG_REG+215)
	errortext(ERR_DUS_CONTEXT_CORRUPT,	"Context information for the current Notes Upgrade Application has been corrupted.")
#define ERR_DUS_UPGRADE_DLL_FAILED 	(PKG_REG+216)
	errortext(ERR_DUS_UPGRADE_DLL_FAILED, 	"The upgrade application dll '%s' could not be found or could not be loaded/initialized.")
#define ERR_DUS_RETRIEVE_FAILED 	(PKG_REG+217)
	errortext(ERR_DUS_RETRIEVE_FAILED,	"Person information could not be retrieved from the current upgrade application.")
#define ERR_DUS_NO_UPGRADE_DLLPATH 	(PKG_REG+218)
	errortext(ERR_DUS_NO_UPGRADE_DLLPATH, 	"No file path was specified for the current upgrade application.")
#define ERR_DUS_MAIL_CONVERSION_FAILED 	(PKG_REG+219)
	errortext(ERR_DUS_MAIL_CONVERSION_FAILED, 	"Mail file conversion for the migrating user '%s' failed or is incomplete.")
#define ERR_DUS_EXTENDED_ERROR2 	(PKG_REG+220)
	errortext(ERR_DUS_EXTENDED_ERROR2, 	"The upgrade application for '%s' returned this error: %s.")
#define ERR_DUS_EXTENDED_ERROR3 	(PKG_REG+221)
	errortext(ERR_DUS_EXTENDED_ERROR3, 	"The current upgrade application returned this error: %s.")
#define ERR_DUS_SKIP_THIS_GROUP 	(PKG_REG+222)
	errortext(ERR_DUS_SKIP_THIS_GROUP, 	"The current group '%s' and its members will not be migrated into Notes.")
#define ERR_DUS_REG_SERVER_DOWN 	(PKG_REG+223)
	errortext(ERR_DUS_REG_SERVER_DOWN, 	"The registration server is down or the Domino directory on that registration server is unavailable.  Change registration servers and try migration again.")
#define ERR_DUS_GROUPWISE_DLLS_ABSENT		(PKG_REG+224)
	errortext(ERR_DUS_GROUPWISE_DLLS_ABSENT, "One or more Groupwise dlls required for Groupwise to Notes migration are not present.")
#define ERR_DUS_NETSCAPE_DLLS_ABSENT		(PKG_REG+225)
	errortext(ERR_DUS_NETSCAPE_DLLS_ABSENT, "One or more Netscape dlls required for Netscape to Notes migration are not present.")
#define ERR_REG_DUS_MAIL_CONVERSION_ONLY 	(PKG_REG+226)
	errortext(ERR_REG_DUS_MAIL_CONVERSION_ONLY, "The mail system must be set to 'None' for users migrated with the 'Convert mail ONLY' option.")
#define STR_REG_DUS_PREREG_NOTIFY		 	(PKG_REG+227)
	stringtext(STR_REG_DUS_PREREG_NOTIFY, "Notifying the current DUS (Domino Upgrade Services) application that registration is about to begin...")
#define ERR_REG_CERT_ALT_LANGUAGE_PROMPT	(PKG_REG+228)
	errortext(ERR_REG_CERT_ALT_LANGUAGE_PROMPT,	"The certifier you have selected does not contain the alternate language tag '%s' (set for the current user).  If you continue, the alternate language tag will be set to 'none' for the selected user(s).  Do you want to continue?")
#define ERR_REG_WITH_POLICY_FAILED		  	(PKG_REG+229)
	errortext(ERR_REG_WITH_POLICY_FAILED,	"Registration failed:  A policy could not be retrieved from the Domino directory.")
#define ERR_REG_HOSTED_ORG_USER_REG 	(PKG_REG+230)
	errortext(ERR_REG_HOSTED_ORG_USER_REG,	"An internet password and a password quality of at least 1 are required when registering a user into a hosted organization.")

/*	211 - 230 reserved for Domino Upgrade Application (DUS) support.
	(migrating foreign directory users) */

#define	ERR_REG_CREATINGADDRESSFILE	(PKG_REG+231)
	stringtext(ERR_REG_CREATINGADDRESSFILE,	"Creating the Domino Directory for your domain...")
#define ERR_REG_CREATING_STATS_DB		(PKG_REG+232)
	stringtext(ERR_REG_CREATING_STATS_DB,	"Creating Statistics & Events Configuration Database...")
#define ERR_REG_SERVER_NETWORK_SETTINGS (PKG_REG+233)
	stringtext(ERR_REG_SERVER_NETWORK_SETTINGS, "Updating network settings...")
#define ERR_REG_SERVER_CONFIGURE_SMTP (PKG_REG+234)
	stringtext(ERR_REG_SERVER_CONFIGURE_SMTP, "Configuring SMTP...")
#define ERR_REG_MINPSWDCHARS			(PKG_REG+235)
	errortext(ERR_REG_MINPSWDCHARS,	"Your password is insufficiently complex. Add more characters or varied characters.")
#define ERR_REG_CREATING_ADMINREQ_DB			(PKG_REG+236)
	stringtext(ERR_REG_CREATING_ADMINREQ_DB,	"Creating Administration Request Database...")
#define ERR_REG_ALT_NAME_NOT_UNIQUE		(PKG_REG+237)
	errortext(ERR_REG_ALT_NAME_NOT_UNIQUE,	"The alternate name '%s' is not unique in the Domino Directory.")
#define ERR_REG_CONVERT_ONLY_FAILED		(PKG_REG+238)
	errortext(ERR_REG_CONVERT_ONLY_FAILED,	"Mail conversion failed.  Either the user could not be found or the original full name was not supplied.")
#define STR_REG_YES						(PKG_REG+239)
	stringtext(STR_REG_YES,				"Yes")
#define STR_REG_DATABASES_BM			(PKG_REG+240)
	stringtext(STR_REG_DATABASES_BM,	"Databases")
#define ERR_REG_IMAP_REQUIRES_PASSWORD	(PKG_REG+241)
	errortext(ERR_REG_IMAP_REQUIRES_PASSWORD,	"A Notes password is required for new IMAP mail users.")
#define STR_REG_NO_DC_CONTINUE_PROMPT	(PKG_REG+242)
	stringtext(STR_REG_NO_DC_CONTINUE_PROMPT, "A primary domain controller for the current NT domain cannot be found.  NT Accounts can only be added locally.  Continue?")
#define ERR_NO_SERIAL_PORT				(PKG_REG+243)
	errortext(ERR_NO_SERIAL_PORT,		"You must specify a serial port in order to connect via a modem")
#define STR_REG_LOG_DUPLICATE_NAME		(PKG_REG+244)
	stringtext(STR_REG_LOG_DUPLICATE_NAME,	"The name %a already exits in a different entry in the directory.")
#define ERR_REG_DUPLICATE_NAME			(PKG_REG+245)
	errortext(ERR_REG_DUPLICATE_NAME,	"One of the names to be added to the directory already exists in a different entry in the directory.  See the Notes log for details.")
#define STR_REG_LOG_NAME_REFUSED		(PKG_REG+246)
	errortext(STR_REG_LOG_NAME_REFUSED, "%a did not accept a name change")
#define ERR_REG_NAB_NOT_PUBLIC			(PKG_REG+247)
	errortext(ERR_REG_NAB_NOT_PUBLIC,	"The Domino directory from the currently selected server is not based on the public directory template (pubnames.ntf).")
#define STR_REG_PERSON_LEFTTOSTORE	  	(PKG_REG+248)
	stringtext(STR_REG_PERSON_LEFTTOSTORE, 	"1 person left to add to the registration queue")
#define STR_REG_PERSON_LEFTTOREAD		(PKG_REG+249)
	stringtext(STR_REG_PERSON_LEFTTOREAD,	"1 person left to retrieve from the registration queue")
#define	ERR_REG_UPDATE_USER_PROMPT		(PKG_REG+250)
	errortext(ERR_REG_UPDATE_USER_PROMPT,	"An entry with the specified person name '%s' is already in the Domino Directory. Update the entry?")
#define	ERR_REG_NO_REG_SERVER_SPECIFIED	(PKG_REG+251)
	errortext(ERR_REG_NO_REG_SERVER_SPECIFIED,	"The registration server cannot be found, is not responding or was not specified/set.")
#define	ERR_REG_ADDRBOOK_SHORTNAME_EXISTS (PKG_REG+252)
	apitext(ERR_REG_ADDRBOOK_SHORTNAME_EXISTS, "No permission (flag) to modify existing Domino Directory entry (short name is duplicated)")
#define	ERR_REG_UPDATE_USER_SHORTNAME_PROMPT	(PKG_REG+253)
	errortext(ERR_REG_UPDATE_USER_SHORTNAME_PROMPT,	"A person entry with the short name '%s' already exists in the Domino Directory. Update the entry?")
#define ERR_REG_REQUIRES_PERSON_NOTE	(PKG_REG+254)
	errortext(ERR_REG_REQUIRES_PERSON_NOTE, "Entry found is not a person entry")
#define	ERR_REG_CERTIFIER_NO_RECOVERY_INFO		(PKG_REG+255)
	errortext(ERR_REG_CERTIFIER_NO_RECOVERY_INFO,	"The current certifier ID contains no recovery information and will not allow ID/password recovery on IDs created with this certifier.  The 'EscrowAgent' ID recovery mechanism is no longer supported.\n\nDisplay this warning in the future?")

/* PKG_REG (above) is allocated codes from 0-255 
 * DO NOT USE CODES ABOVE #255 FOR THIS PACKAGE 
 */

#define	ERR_REG_CREATING_ADMINREQTOCREATE_MAILREPLICA	(PKG_REG2+0)
	errortext(ERR_REG_CREATING_ADMINREQTOCREATE_MAILREPLICA,	"Creating administration request to create mail file for %A...")
#define	ERR_REG_CREATING_MAILREPLICA	(PKG_REG2+1)
	errortext(ERR_REG_CREATING_MAILREPLICA,	"Creating mail file replica stub for %A...")
#define	ERR_REG_NO_MAILREPLICAS_CREATED	(PKG_REG2+2)
	errortext(ERR_REG_NO_MAILREPLICAS_CREATED,	"Mail file replica stub(s) for %A could not be created")
#define	ERR_REG_USER_FILE_REPLICATION	(PKG_REG2+3)
	errortext(ERR_REG_USER_FILE_REPLICATION,	"Creation of the mail or roaming file replica stub '%A' failed with the following error:  %s")
#define	ERR_REG_USER_MAILREPS_ADMINP_REQUEST	(PKG_REG2+4)
	errortext(ERR_REG_USER_MAILREPS_ADMINP_REQUEST,	"Creation of an admin process request to create a mail or roaming file replica on server '%A' failed with this error:  %s")
#define	ERR_REG_NO_CACERTS_AVAILABLE	(PKG_REG2+5)
	errortext(ERR_REG_NO_CACERTS_AVAILABLE,	"No CA enabled certifiers could be found in the directory.")

#define ERR_REG_MISSING_ORG_STORAGE						(PKG_REG2+6)
	errortext(ERR_REG_MISSING_ORG_STORAGE, "You must specify a storage location for this organization.")
#define ERR_REG_ORG_STORAGE_MUSTBERELATIVE				(PKG_REG2+7)
	errortext(ERR_REG_ORG_STORAGE_MUSTBERELATIVE, "The organization's storage directory must be relative to data directory.")
#define ERR_REG_MISSING_ORG_INTERNET_DOMAIN				(PKG_REG2+8)
	errortext(ERR_REG_MISSING_ORG_INTERNET_DOMAIN, "You must specify an internet domain for this organization")
#define ERR_REG_MISSING_CA_SERVER						(PKG_REG2+9)
	errortext(ERR_REG_MISSING_CA_SERVER, "Since the organization certifier is CA enabled, you must specify a certificate authority server name.")
#define ERR_REG_NAB_NOT_VIRTUALIZED				(PKG_REG2+10)
    errortext(ERR_REG_NAB_NOT_VIRTUALIZED, "The Domino Directory on the Directory Server is not enabled for Hosted Organizations.")
#define ERR_REG_ORG_STORAGE_NOT_UNIQUE					(PKG_REG2+11)
    errortext(ERR_REG_ORG_STORAGE_NOT_UNIQUE, "The storage location could not be used for this organization.  A storage location with the same name exists or is already in use.")
#define ERR_REG_POLICY_HOSTING_SERVER_NEEDED			(PKG_REG2+12)
    errortext(ERR_REG_POLICY_HOSTING_SERVER_NEEDED, "The effective registration policy 'Choose the mail server' field must contain a server that you have administrator rights on.")
#define ERR_REG_POLICY_PASSWORD_QUALITY_NEEDED			(PKG_REG2+13)
    errortext(ERR_REG_POLICY_PASSWORD_QUALITY_NEEDED, "The effective registration policy 'Choose a password quality' field can not be 'Password is optional' to register an organization.")
#define ERR_REG_POLICY_INTERNET_PASSWORD_NEEDED			(PKG_REG2+14)
    errortext(ERR_REG_POLICY_INTERNET_PASSWORD_NEEDED, "The effective registration policy 'Set Internet Password' field must be a checked to register an organization.")
#define ERR_REG_POLICY_DOLS_SERVER_NEEDED				(PKG_REG2+15)
    errortext(ERR_REG_POLICY_DOLS_SERVER_NEEDED, "The effective registration policy 'Choose the mail server' field must be a DOLS server to register an organization.")
#define ERR_REG_POLICY_ID_CREATION_NEEDED				(PKG_REG2+16)
    errortext(ERR_REG_POLICY_ID_CREATION_NEEDED, "The effective registration policy 'Create a Notes ID' field must be a checked to support the CA process, DOLS and Lotus Notes mail.")
#define ERR_REG_POLICY_ID_IN_NAB_NEEDED					(PKG_REG2+17)
    errortext(ERR_REG_POLICY_ID_IN_NAB_NEEDED, "The effective registration policy 'In Domino Directory' ID storage field must be a checked to support CA process, DOLS and Lotus Notes mail.")
#define ERR_REG_BAD_IP_ADDRESS_FORMAT					(PKG_REG2+18)
	errortext(ERR_REG_BAD_IP_ADDRESS_FORMAT, "You must specify a properly formatted IP address.")
#define STR_REG_ALL_HOSTED_ADMINS		(PKG_REG2+19)
	stringtext(STR_REG_ALL_HOSTED_ADMINS, "HostedOrganizationAdmins")
#define STR_REG_HOSTED_ADMINS		(PKG_REG2+20)
	stringtext(STR_REG_HOSTED_ADMINS, "Administrators")
/* 21, 22 unused */
#define ERR_REG_ORG_STORAGE_MUSTBEABSOLUTE				(PKG_REG2+23)
	errortext(ERR_REG_ORG_STORAGE_MUSTBEABSOLUTE, "The organization's physical storage directory must be an absolute path.")

#define STR_REG_INTERNET_SITES		(PKG_REG2+24)
	stringtext(STR_REG_INTERNET_SITES,	"Web Site|IMAP Site|POP3 Site|SMTP Inbound Site|Directory Site|IIOP Site|File Protection")
#define IDX_INTERNETSITE_WEB			1
#define IDX_INTERNETSITE_IMAP			2
#define IDX_INTERNETSITE_POP3			3
#define IDX_INTERNETSITE_SMTPINBOUND	4
#define IDX_INTERNETSITE_DIRECTORY		5
#define IDX_INTERNETSITE_IIOP			6
#define IDX_INTERNETSITE_FILE_PROTECT	7

#define	STR_REG_LOCAL					(PKG_REG2+25)
	stringtext(STR_REG_LOCAL,			"Local")

#define STR_REG_SEPARATOR_LIST			(PKG_REG2+26)
	stringtext(STR_REG_SEPARATOR_LIST, 	"None|Underscore|Dot|Equal|Percent|_|.|=|%")

#define	ERR_REG_USER_NOT_UPDATED_FULLNAME			(PKG_REG2+27)
	errortext(ERR_REG_USER_NOT_UPDATED_FULLNAME, 	"Existing Domino Directory entry (duplicate full name) not updated by administrator.")
#define	ERR_REG_USER_NOT_UPDATED_SHORTNAME			(PKG_REG2+28)
	errortext(ERR_REG_USER_NOT_UPDATED_SHORTNAME, 	"Existing Domino Directory entry (duplicate short name) not updated by administrator.")
#define	ERR_REG_USERID_NOT_UPDATED					(PKG_REG2+29)
	errortext(ERR_REG_USERID_NOT_UPDATED,			"Existing UserID file not updated by administrator.")
#define	ERR_REG_ROAMING_SERVER_VERSION				(PKG_REG2+30)
	errortext(ERR_REG_ROAMING_SERVER_VERSION,		"The specified roaming server is pre 6 and does not support roaming features.  Select a different roaming server.")

#define ERR_REG_ASP_CERT_NEEDED			(PKG_REG2+31)
	errortext(ERR_REG_ASP_CERT_NEEDED, "Only the ASP certifier can be used to register a hosted organization.")
#define ERR_REG_ORIGINAL_ORG_DIR_EXISTS	(PKG_REG2+32)
	errortext(ERR_REG_ORIGINAL_ORG_DIR_EXISTS, "You must remove the original organization directory before you can move the organization storage location.")
#define STR_REG_CREATINGSTORAGE			(PKG_REG2 + 33)
	stringtext(STR_REG_CREATINGSTORAGE, "Creating server storage for %A...")
#define STR_REG_NAMESACL				(PKG_REG2 + 34)
	stringtext(STR_REG_NAMESACL, 		"Configuring access control and extended access control lists in the Domino Directory...")
#define STR_REG_ADMIN4ACL				(PKG_REG2 + 35)
	stringtext(STR_REG_ADMIN4ACL, 		"Configuring access control and extended access control lists in the Administration Request Database...")
#define STR_REG_DOMAINCFG				(PKG_REG2 + 36)
	stringtext(STR_REG_DOMAINCFG,		"Configuring %A domain and internet site documents...")
#define STR_REG_CAENABLE				(PKG_REG2 + 37)
	stringtext(STR_REG_CAENABLE,		"Migrating %A to certificate authority...")
#define ERR_REG_ORG_STORAGE_DRIVE_INVALID			(PKG_REG2 + 38)
	stringtext(ERR_REG_ORG_STORAGE_DRIVE_INVALID,	"The organization's physical storage must contain at least one directory.  Drive names are not valid")

#define ERR_REG_USERREG_BAD_INETADDR	   	(PKG_REG2+39)
	errortext(ERR_REG_USERREG_BAD_INETADDR,	"Internet address failed with the following error:  %e")

#define	STR_REG_DIRECTORY_TITLE_STRING		(PKG_REG2+40)
	stringtext(STR_REG_DIRECTORY_TITLE_STRING,	"%a's Directory")

#define	ERR_REG_CREATING_ROAMINGREPLICA		(PKG_REG2+41)
	errortext(ERR_REG_CREATING_ROAMINGREPLICA,	"Creating roaming file replica stubs for %A...")
#define	ERR_REG_NO_ROAMINGREPLICAS_CREATED	(PKG_REG2+42)
	errortext(ERR_REG_NO_ROAMINGREPLICAS_CREATED,	"Roaming file replica stubs for %A could not be created")

#define	ERR_REG_GROUPS_ERROR	(PKG_REG2+43)
	errortext(ERR_REG_GROUPS_ERROR,	"The user could not be added to the group(s) specified.")

#define	ERR_REG_GROUPS_ERROR_DETAIL	(PKG_REG2+44)
	errortext(ERR_REG_GROUPS_ERROR_DETAIL,	"%a could not be added to the group: '%a'.")

#define STR_REG_GLOBALRULE_DOLS					(PKG_REG2 + 45)
	stringtext(STR_REG_GLOBALRULE_DOLS,			"Global Rule for DOLS Download")
#define STR_REG_GLOBALRULE_INOTES_HELP			(PKG_REG2 + 46)
	stringtext(STR_REG_GLOBALRULE_INOTES_HELP,	"Global Rule for Domino Web Access Help Files")
#define STR_REG_GLOBALRULE_INOTES_CAB			(PKG_REG2 + 47)
	stringtext(STR_REG_GLOBALRULE_INOTES_CAB,	"Global Rule for INOTES.CAB")

#define	ERR_REG_ID_IN_MAILFILE_SERVER_VERSION	(PKG_REG2+48)
	errortext(ERR_REG_ID_IN_MAILFILE_SERVER_VERSION,	"The specified mail server is pre 6 and does not support the ID in mail file feature.  Select a different mail server.")

#define ERR_REG_PASSWORD_ID_IN_NSF	(PKG_REG2+49)
	stringtext(ERR_REG_PASSWORD_ID_IN_NSF, "No password supplied for %s (required for ID files to be stored in Notes database).")

#define	STR_REG_LOCAL_DOMAIN_ADMINS	(PKG_REG2+50)
	errortext(STR_REG_LOCAL_DOMAIN_ADMINS,	"LocalDomainAdmins")

#define	ERR_REG_SECURITY_POLICY_DENY_OPERATION	(PKG_REG2+51)
	errortext(ERR_REG_SECURITY_POLICY_DENY_OPERATION,	"The security policy is set to deny this operation")

#define ERR_REG_USER_CREATOR_ROLE			(PKG_REG2+52)
	errortext(ERR_REG_USER_CREATOR_ROLE,	"You are not authorized to add Person documents in this Domino Directory")
#define ERR_REG_USER_MODIFIER_ROLE			(PKG_REG2+53)
	errortext(ERR_REG_USER_MODIFIER_ROLE,	"You are not authorized to modify Person documents in this Domino Directory")
#define ERR_REG_GROUP_CREATOR_ROLE			(PKG_REG2+54)
	errortext(ERR_REG_GROUP_CREATOR_ROLE,	"You are not authorized to add Group documents in this Domino Directory")
#define ERR_REG_GROUP_MODIFIER_ROLE			(PKG_REG2+55)
	errortext(ERR_REG_GROUP_MODIFIER_ROLE,	"You are not authorized to modify Group documents in this Domino Directory")
#define ERR_REG_SERVER_CREATOR_ROLE			(PKG_REG2+56)
	errortext(ERR_REG_SERVER_CREATOR_ROLE,	"You are not authorized to add Server documents in this Domino Directory")
#define ERR_REG_SERVER_MODIFIER_ROLE		(PKG_REG2+57)
	errortext(ERR_REG_SERVER_MODIFIER_ROLE,	"You are not authorized to modify Server documents in this Domino Directory")
#define ERR_REG_POLICY_CREATOR_ROLE			(PKG_REG2+58)
	errortext(ERR_REG_POLICY_CREATOR_ROLE,	"You are not authorized to add Policy documents in this Domino Directory")
#define ERR_REG_POLICY_MODIFIER_ROLE		(PKG_REG2+59)
	errortext(ERR_REG_POLICY_MODIFIER_ROLE,	"You are not authorized to modify Policy documents in this Domino Directory")
#define ERR_REG_NET_CREATOR_ROLE			(PKG_REG2+60)
	errortext(ERR_REG_NET_CREATOR_ROLE,		"You are not authorized to add that document type to this Domino Directory")
#define ERR_REG_NET_MODIFIER_ROLE			(PKG_REG2+61)
	errortext(ERR_REG_NET_MODIFIER_ROLE,	"You are not authorized to modify that document type in this Domino Directory")

#define	ERR_REG_BAD_ASP_SETUP				(PKG_REG2+62)
	errortext(ERR_REG_BAD_ASP_SETUP,		"The Domino Directory is missing a required item (created during setup): %s.")

#define ERR_REG_USERLESS_REGSERVER			(PKG_REG2+63)
	errortext(ERR_REG_USERLESS_REGSERVER,	"The Domino Directory on the registration server cannot be a configuration-only (userless) directory")
#define ERR_REG_NO_ROLES					(PKG_REG2+64)
	errortext(ERR_REG_NO_ROLES,				"You do not have any of the required roles in the Domino Directory")
#define ERR_REG_CLIENT_DATA_DIR				(PKG_REG2+65)
	errortext(ERR_REG_CLIENT_DATA_DIR,		"The local data directory is client-based and cannot be used for server-based operations")
#define ERR_REG_PERSONAL_SUBDIR_ERROR		(PKG_REG2+66)
    errortext(ERR_REG_PERSONAL_SUBDIR_ERROR, "Creation of the sub-directory %A failed with the following error:  %s")

#define ERR_REG_COMMON_NAME_TOO_LONG		(PKG_REG2+67)
	errortext(ERR_REG_COMMON_NAME_TOO_LONG,	"The combination of first, middle and last names is %d byte(s) too long.")

#define	STR_REG_SSLCONFIG	(PKG_REG2+68)
	errortext(STR_REG_SSLCONFIG,	"Configuring SSL for %A")
#define	ERR_REG_SSL_HOSTNAME	(PKG_REG2+69)
	errortext(ERR_REG_SSL_HOSTNAME,	"A hostname is required to configure SSL.")

#define STR_CERTLOG_TITLE					(PKG_REG2+70)
	errortext(STR_CERTLOG_TITLE,			"%a's Certification Log")

#define	ERR_REG_POLICY_SIGNER				(PKG_REG2+71)
	errortext(ERR_REG_POLICY_SIGNER,		"%a is the signer of policy %a and will not be deleted until the policy has been signed (saved) by someone else.")

#define	STR_REG_HAS_INET_CERT				(PKG_REG2+72)
	stringtext(STR_REG_HAS_INET_CERT,		"%a's internet certificates (issued by the following authorities:  %a) contain the internet address %s.")

#define	ERR_REG_HAS_NO_INET_CERT			(PKG_REG2+73)
	errortext(ERR_REG_HAS_NO_INET_CERT,		"None of %a's internet certificates (issued by the following authorities:  %a) contain the new internet address %s.")

#define STR_REG_BOOKMARK_DB_TITLE			(PKG_REG2+74)
stringtext (STR_REG_BOOKMARK_DB_TITLE, 		"Bookmarks (6)")

#define	ERR_REG_PROFILE						(PKG_REG2+75)
	errortext(ERR_REG_PROFILE,				"Admin Process: Received the error %e trying to add %a's name to the %s profile of the Directory.")

#define ERR_REG_REPLICATING_DIR				(PKG_REG2+76)
	errortext(ERR_REG_REPLICATING_DIR,		"Replicating Domino Directory changes...")

#define	STR_REG_WARN_INET_CERT				(PKG_REG2+77)
	stringtext(STR_REG_WARN_INET_CERT,		"  If the internet address is changed, new certificates will have to be issued for SMIME mail.")

#define ERR_REG_NO_HOSTED_ORG_STORAGE			(PKG_REG2+78)
	errortext(ERR_REG_NO_HOSTED_ORG_STORAGE,	"The server does not contain storage for the hosted organization")

#define ERR_REG_PROMPTING_SERVER_PASSWD			(PKG_REG2+79)
	stringtext(ERR_REG_PROMPTING_SERVER_PASSWD,	"Prompting for Server ID password")

#define	ERR_REG_ADMINP_PREFIX						(PKG_REG2+80)
	errortext(ERR_REG_ADMINP_PREFIX,				"Admin Process: ")

#define STR_REG_AGENT_RUN_UNRESTRICTED				(PKG_REG2+81)
	errortext(STR_REG_AGENT_RUN_UNRESTRICTED,		"%a needs Run unrestricted methods and operations rights on %a in order to have the Administration Process reschedule the agent %s in %s")

#define	STR_REG_AGENT_RUN_RESTRICTED				(PKG_REG2+82)
	errortext(STR_REG_AGENT_RUN_RESTRICTED,			"%a needs Run restricted LotusScript/Java agents rights on %a in order to have the Administration Process reschedule the agent %s in %s")
	
#define	STR_REG_AGENT_RUN_SIMPLE					(PKG_REG2+83)
	errortext(STR_REG_AGENT_RUN_SIMPLE,				"%a needs Run Simple and Formula agents rights on %a in order to have the Administration Process reschedule the agent %s in %s")

#define	ERR_REG_NOT_RA						(PKG_REG2+84)
	errortext(ERR_REG_NOT_RA,				"You are not a Registration Authority for the certifier.")

/* PKG_REG2 (above) is allocated codes from 0-127 
 * DO NOT USE CODES ABOVE #127 FOR THIS PACKAGE 
 */

#endif

