
/* Message Queue Package Public Definitions */

#ifndef MQ_DEFS
#define MQ_DEFS

#ifdef __cplusplus
extern "C" {
#endif

typedef DWORD MQHANDLE;	

#define MQ_MAX_MSGSIZE 		(MAXONESEGSIZE - 0x40)
#define NOPRIORITY			MAXWORD
#define	LOWPRIORITY			MAXWORD
#define HIGHPRIORITY		0

/*	Function pointer type for MQScan() callback */

typedef	STATUS (LNCALLBACKPTR MQSCAN_CALLBACK) (const char far *pBuffer, 
		 						 			  WORD Length, WORD Priority,
								 			  void far *Ctx);

/*	Options to MQGet */

#define	MQ_WAIT_FOR_MSG			0x0001

/* Options to MQOpen */
#define MQ_OPEN_CREATE			0x00000001	/* Create the queue if it doesn't exist*/

																	

/*	Routine definitions */

STATUS LNPUBLIC	 MQCreate(const char far *QueueName, WORD Quota, DWORD Options);
STATUS LNPUBLIC	 MQOpen(const char far *QueueName, DWORD Options, MQHANDLE far *RetQueue);
STATUS LNPUBLIC	 MQClose(MQHANDLE Queue, DWORD Options);
STATUS LNPUBLIC	 MQPut(MQHANDLE Queue, WORD Priority, char far *Buffer, WORD Length, 
						DWORD Options);
STATUS LNPUBLIC	 MQGet(MQHANDLE Queue, char far *Buffer, WORD BufLength,
					  	DWORD Options, DWORD Timeout, WORD far *retMsgLength);
STATUS LNPUBLIC	 MQScan(MQHANDLE Queue, char far *Buffer, WORD BufLength, 
						 DWORD Options, MQSCAN_CALLBACK ActionRoutine,
						 void far *Ctx, WORD far *retMsgLength);

void LNPUBLIC	 MQPutQuitMsg(MQHANDLE Queue);
BOOL LNPUBLIC	 MQIsQuitPending(MQHANDLE Queue);
WORD LNPUBLIC	 MQGetCount(MQHANDLE Queue);

#ifdef __cplusplus
}
#endif

#endif /* MQ_DEFS */

