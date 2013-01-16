#ifdef __cplusplus
extern "C" {
#endif


#ifndef ALARM_DEFS
#define ALARM_DEFS

#ifndef MQ_DEFS
#include "mq.h"
#endif

#ifndef NSF_DATA_DEFS
#include "nsfdata.h"				/*	UNID */
#endif


/*	Clientname passed in during register.  This is the clientname of the
	client which wants to register itself to receive alarms.  */

#define MAX_ALARM_CLIENTNAME	32

/*	Clients to the alarms daemon should append the client name to the
	following string in order to come up with a MQ Name to create before
	registering with the alarms daemon for alarms.  Alarms daemon will use 
	the name <MQPREFIX_FOR_CLIENT><ClientName> as a string for opening the
	MQ for the client and send alarms to it */

#define MQPREFIX_FOR_CLIENT		"AD$"

/*	Constants for the TYPE field of the ALARM_DATA structure:
	Messages sent by the alarms daemon in the MQ of the client.  Client
	should periodically check it's queue in order to receive these and
	do the appropriate tasks. */

/*	Message sent when there are 'past alarms' - alarms prior to today.
	Client will get a buffer with all the past alarms and a count of how
	many alarms there are.  All the alarms have the structure defined in
	this file 
*/

#define ALARM_MSG_PENDING_ALARMS	0x0001
										  
/*	Client receives this message when there is an alarm to be displayed */

#define ALARM_MSG_NEW_ALARM			0x0002

/*	Alarms Daemon sends the next message to all the registered clients
	except for the one who started it before it terminates. */

#define ALARM_MSG_IS_TERMINATING	0x0003


/*	Structure defining the MQ request sent by the alarms daemon to the
	client who has registered for receiving alarms.  */

typedef struct						/*  Alarms Daemon request message */
	{
	WORD	Type;					/*  Type of request (ALARM_MSG_xxx) */
	WORD	Flags;					/*  Options - not used now */
	WORD	wNumOfAlarms;			/* 	Number of alarms */
	HANDLE	hAlarmsData;			/*	Handle to an array of alarms structures */
	} ALARM_MSG;

/*	Message structure sent TO THE CLIENT, one per alarm.  There is an array
	of these structures pointed to by ALARM_MSG. */

typedef struct
	{
	TIMEDATE	tmEventTime;			/*	Time for the event  */
	TIMEDATE	tmAlarmTime;			/*	Time for the alarm */
	HANDLE		hSubject;				/*	Handle to alarm subject */
	WORD		Flags;
	UNID		EventUNID;				/*	UNID for the event note */
	HANDLE 		hAlarmTuneName;			/*	Tune to play */
	HANDLE		hSendMailAddr;			/*	Address to send message to */
	} ALARM_DATA, far *LPALARM_DATA;

#define ALARM_DATA_FLAG_DISPLAY_ALARM		0x0001	/* Display the alarm */
#define ALARM_DATA_FLAG_PLAY_TUNE			0x0002	/* Play the tune */
#define ALARM_DATA_FLAG_SEND_MAIL			0x0004	/* Send mail */
/*	API calls a client can make to get various things performed by the
	Alarms Daemon. */

STATUS LNPUBLIC Alarm_RegisterInterest (char far *pszClientName, 
										WORD Flags);

STATUS LNPUBLIC Alarm_DeregisterInterest (char far *pszClientName,
											WORD Flags);


STATUS LNPUBLIC Alarm_ProcessAlarm (char far *pszClientName,
										WORD wSnoozeMinutes,
										UNID EventUNID,
										WORD Flags);


/*	Alarm daemon should re-check the mailfile and refresh it's list of 
	alarms because new alarms might have been added, or some might have
	been deleted. */

STATUS LNPUBLIC Alarm_RefreshAlarms	(char far *pszClientName);

/*	Inform alarms daemon to now re-read the location record */

STATUS LNPUBLIC Alarm_ChangeLocation (char far *pszClientName, 
									  WORD Flags);


#endif
	
#ifdef __cplusplus
}
#endif


