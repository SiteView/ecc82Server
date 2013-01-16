#ifndef EVENT_ERR_DEFS
#define EVENT_ERR_DEFS

/*	Message codes for the Event Package */

#define ERR_EVENT_EXCEED_MEM	PKG_EVENT+1
	errortext(ERR_EVENT_EXCEED_MEM, 	"Warning: Cannot record event - cannot keep up with event occurrence rate!\nThe EVENT_POOL_SIZE param in NOTES.INI can be set up to 5Mb. It defaults to 3Mb.")
#define ERR_EVENT_DUPL_QUEUE	PKG_EVENT+2
	errortext(ERR_EVENT_DUPL_QUEUE, 	"A queue with that name already exists.")
#define ERR_EVENT_NOT_Q			PKG_EVENT+3
	errortext(ERR_EVENT_NOT_Q, 	"Not a known event queue.")
#define ERR_EVENT_EXCEED_QUOTA	PKG_EVENT+4
	errortext(ERR_EVENT_EXCEED_QUOTA, 	"Cannot enqueue event - quota exceeded!")
#define ERR_EVTQUEUE_EMPTY		PKG_EVENT+5
	errortext(ERR_EVTQUEUE_EMPTY, 	"No entries in this queue")
#define ERR_EVENT_NO_SUCH_QUEUE	PKG_EVENT+6
	errortext(ERR_EVENT_NO_SUCH_QUEUE, 	"No such queue")
#define ERR_EVENT_NOT_YOUR_Q 	PKG_EVENT+7
	errortext(ERR_EVENT_NOT_YOUR_Q, 	"Can only access your queue.")
#define ERR_EVENT_WRONG_VERSION	PKG_EVENT+8
	errortext(ERR_EVENT_WRONG_VERSION,"Wrong version of event package.")
#define ERR_EVENT_BAD_SEVERITY	PKG_EVENT+9
	errortext(ERR_EVENT_BAD_SEVERITY,"Invalid severity encountered.")
#define ERR_EVENT_NOT_INITIALIZED	PKG_EVENT+10
	errortext(ERR_EVENT_NOT_INITIALIZED,"Event support not initialized.")
#define ERR_EVENT_POOLFULL		PKG_EVENT+11
	errortext(ERR_EVENT_POOLFULL,"Insufficient memory - event pool is full.")
#define ERR_EVENT_SHUTDOWN		PKG_EVENT+12
	internaltext(ERR_EVENT_SHUTDOWN,"Shutdown requested")
#define ERR_EVENT_BAD_MONITOR_METHOD	PKG_EVENT+13
	errortext(ERR_EVENT_BAD_MONITOR_METHOD,"Invalid monitor method encountered.")
#define ERR_EVENT_BAD_TYPE	PKG_EVENT+14
	errortext(ERR_EVENT_BAD_TYPE,"Invalid type encountered.")

#define TASK_NAME_STRINGS			PKG_EVENT+15
	donottranslatetext(TASK_NAME_STRINGS, 	"Chronos|Database Compactor|Admin Process|Agent Manager|Billing|Calendar Connector|Cluster Directory|Cluster Replicator|Collector|Database Server")

#define TASK_NAME_STRINGS_2			PKG_EVENT+16
	donottranslatetext(TASK_NAME_STRINGS_2, "Event|FTP Server|HTTP Web Server|IMAP Server|Indexer|LDAP Server|MTBFManager|NNTP Server|POP3 Server|Router|Replicator|Schedule Manager|Stats|WEB Retriever|Reporter|SMTPMTA|Health Monitoring")

#define ERR_EVENT_REPORT_DB_NOT_OPEN	PKG_EVENT+17
	errortext(ERR_EVENT_REPORT_DB_NOT_OPEN,"The report database could not be opened.")
#define ERR_EVENT_NO_SRV_ACCESS_VIEW	PKG_EVENT+18
	errortext(ERR_EVENT_NO_SRV_ACCESS_VIEW,"The server access view does not exist. Update the Statistics Report template.")

#define ERR_REMOTE_COLLECTOR_QUEUE		PKG_EVENT+19
	errortext(ERR_REMOTE_COLLECTOR_QUEUE,"The Statistics Collector is not running on the proxy server.")

#define ERR_CONFIG_DB_TIMEOUT		PKG_EVENT+20
	errortext(ERR_CONFIG_DB_TIMEOUT,"Task timed out waiting for Monitoring Configuration database to be created. Please restart task.")

#define ERR_ADDIN_CREATE_CONFIG		PKG_EVENT+21
	errortext(ERR_ADDIN_CREATE_CONFIG,"Upgrading or creating the Monitoring Configuration database...")

#define ERR_ADDIN_NO_CONFIG			PKG_EVENT+22
	errortext(ERR_ADDIN_NO_CONFIG,"Cannot create the Monitoring Configuration database.")

#define ERR_ADDIN_SETUP_EVENTS				PKG_EVENT+23
	stringtext(ERR_ADDIN_SETUP_EVENTS,		"If a %s event of Failure severity occurs, log notification to %s.")
#define ERR_ADDIN_SETUP_ACL					PKG_EVENT+24
	stringtext(ERR_ADDIN_SETUP_ACL,			"Generate a Security event of Warning (High) severity if the ACL for database %s changes.  In addition, notify %a by mail.")
#define ERR_ADDIN_SETUP_REPL				PKG_EVENT+25
	stringtext(ERR_ADDIN_SETUP_REPL,		"Generate a Replication event of Warning (High) severity if the database %s on the server %a has not replicated in 24 hours with ANY server. In addition, notify %a via mail.")

#define ERR_ADDIN_DELETE_GROUP				PKG_EVENT+26
	stringtext(ERR_ADDIN_DELETE_GROUP,		"Do you want to delete the selected groups?")

#define ERR_REMOTE_COLLECTOR_R4				PKG_EVENT+27
	stringtext(ERR_REMOTE_COLLECTOR_R4,		"The remote collection server must be R5 or greater.")

#define ERR_ADDIN_CREATE_REDZONE			PKG_EVENT+28
	errortext(ERR_ADDIN_CREATE_REDZONE,		"Creating the Server Health Monitoring database...")

#define ERR_ADDIN_NO_REDZONE				PKG_EVENT+29
	errortext(ERR_ADDIN_NO_REDZONE,			"Cannot create the Server Health Monitoring database.")

#define ERR_ADDIN_UPDATE_REDZONE			PKG_EVENT+30
	errortext(ERR_ADDIN_UPDATE_REDZONE,		"Updating the Server Health Monitoring database design and configuration documents.")

/* Since this name space is so underutilized, it will share its top */
/* space with the briefcase.  So unlike most error ranges, PKG_EVENT */
/* can only range from 0 to 47.  PKG_BCASE goes from 128 to 256. */
#endif

