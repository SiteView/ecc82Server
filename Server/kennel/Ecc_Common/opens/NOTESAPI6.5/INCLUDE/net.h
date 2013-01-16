#ifdef __cplusplus
extern "C" {
#endif


/* Network Package Public Definitions */

#ifndef NET_DEFS
#define NET_DEFS


/*  Option flags for Network Routines  */

/* Options for NetSend:  */

#define NETOPT_SEND_NOW				0x0001	/*  Send now (do not buffer) */
#define NETOPT_FLUSH				0x0008	/*  Flush remaining receive data */

/* Options for NetReceive: */

#define NETOPT_WAIT_FOR_SOME_DATA	0x0001	/*  Wait for some data */
#define NETOPT_WAIT_FOR_ALL_DATA	0x0002	/*  Wait for all data */
#define NETOPT_PEEK_AT_DATA			0x0004	/*  Peek at data */

/*  Options for NetSetSessionMode: */

#define NETOPT_STREAM_MODE			0x0001	/*  Stream mode */


/*  Session Id  */

typedef struct
	{
	WORD Index;						/* Index */
	WORD SeqNo;						/* Sequence number */
	}
SESSIONID;

/* Network protocol flags */

#define PROTOCOL_TCP                    0x0001
#define PROTOCOL_ATALK                  0x0002
#define PROTOCOL_SPX                    0x0004
#define PROTOCOL_VNS                    0x0008
#define PROTOCOL_NETBIOS                0x0010
#define PROTOCOL_XPC                    0x0020
#define PROTOCOL_APPC                   0x0040
#define PROTOCOL_X25                    0x0100


/*	Definitions used for creating a Remote LAN DLL, which contains "hooks"
	that can get control during network operations (for monitoring, etc). */

/* Action types for execution by remote lan library */
#define REMOTE_LAN_SERVICE_CONNECT 1
#define REMOTE_LAN_SERVICE_DISCONNECT 2
#define REMOTE_LAN_SERVICE_TERMINATE 3
#define REMOTE_LAN_SERVICE_CHECK_CONNECTED 4
#define REMOTE_LAN_SERVICE_GET_EXISTING_LINKS 5
#define REMOTE_LAN_SERVICE_GET_DIAL_ENTRY_INFO 6
#define REMOTE_LAN_SERVICE_CREATE_DIAL_ENTRY 7
#define REMOTE_LAN_SERVICE_CREATE_DIAL_ENTRY_DIALOG 8
#define REMOTE_LAN_SERVICE_GET_DIAL_ENTRY_LIST 9

/* Used by StatusDisplayCallback */
#define REMOTE_LAN_INIT_THREAD 1
#define REMOTE_LAN_TERM_THREAD 2
#define REMOTE_LAN_DISPLAY_STATUS 3
#define REMOTE_LAN_CHECK_ABORT 4
#define REMOTE_LAN_DISPLAY_ERR0R_TEXT 5

#define REMOTE_LAN_STATUS_STARTING_CONNECTION 10
#define REMOTE_LAN_STATUS_PHYSICALLY_CONNECTED 11
#define REMOTE_LAN_STATUS_AUTHENTICATING 12
#define REMOTE_LAN_STATUS_AUTHENTICATED 13
#define REMOTE_LAN_STATUS_WAITING_FOR_CALLBACK 14
#define REMOTE_LAN_STATUS_LINK_ESTABLISHED 15
#define REMOTE_LAN_STATUS_LINK_FAILED 16
#define REMOTE_LAN_STATUS_HANGING_UP 17
#define REMOTE_LAN_STATUS_HANGUP_COMPLETE 18

typedef BOOL (LNCALLBACKPTR PREMOTE_LAN_STATUS_CALLBACK)(WORD Action, STATUS status, char *pErrText);
typedef STATUS (LNCALLBACKPTR PREMOTE_LAN_SERVICE_ENTRY)(VARARG_PTR	Ap);

/* The following two constants	are involved with storing the RLAN User ID. The maximum length of the */
/*	User ID for Windows RAS is 256 characters + 1 character for null termination. */
#define MAX_REMOTE_LAN_SERVICE_NAME 256 +1 	/* DLL Name for Remote Lan Service - used to be 20*/
#define MAX_REMOTE_LAN_PARAM_STRING 256 + 1	/* For parameter storage. This
										applies to params and param labels. - used to be 50*/

#define MAX_REMOTE_LAN_TAG_STRING (MAXPATH)	/* For some Remote LAN adaptors this will be a filename */


/*	Function prototypes */

STATUS LNPUBLIC NetLink (char far *pRemoteSystem, char far *pPortName, 
						   char far *pConnectInfo, void far *ReservedMustBeNull, 
						   SESSIONID far *pRetSessionId);
STATUS LNPUBLIC NetSend (SESSIONID SessionID, void far *Buffer, WORD Length, WORD Options);
STATUS LNPUBLIC NetReceive (SESSIONID SessionID, void far *retBuffer, WORD Length, WORD Options, WORD far *retSize);
STATUS LNPUBLIC NetSetSessionMode (SESSIONID SessionID, WORD Options,
							DWORD SendTimeout, DWORD ReceiveTimeout,
							WORD NumBuffers, WORD BufferSize);
void LNPUBLIC NetCloseSession (SESSIONID SessionID);
#endif


#ifdef __cplusplus
}
#endif
