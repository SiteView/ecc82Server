
/*	Definitions for routines available to all server add-ins */

#ifndef ADDIN_DEFS
#define ADDIN_DEFS

#ifdef __cplusplus
extern "C" {
#endif

#define	MAX_CONSOLE_CMD_LEN	256


/*	Public entry points */

/*	Definition for the add-in's main entry point */

STATUS LNPUBLIC	AddInMain (HMODULE hModule, int argc, char far *argv[]);

/*	Service routines available to add-ins */

BOOL LNPUBLIC		AddInIdle (void);
BOOL LNPUBLIC		AddInIdleDelay (DWORD Delay);
BOOL LNPUBLIC		AddInSecondsHaveElapsed (DWORD Seconds);
#define AddInMinutesHaveElapsed(x) AddInSecondsHaveElapsed((DWORD)(x)*60L)
BOOL LNPUBLIC		AddInDayHasElapsed (void);
BOOL LNPUBLIC		AddInShouldTerminate (void);

void LNVARARGS		AddInFormatErrorText (char far *retString, char far *String, ...);
void LNPUBLIC		AddInLogErrorText (char far *String, STATUS AdditionalErrorCode, char far *Arg);
void LNVARARGS		AddInLogMessageText (char far *String, STATUS AdditionalErrorCode, ...);
void LNVARARGS		AddInSetStatusText (char far *String, ...);

/*	To Notes API customers (only):
	The following 5 functions ARE ONLY AVAILABLE ON PLATFORMS THAT
	SUPPORT USER-CREATED STRING RESOURCES.  Since the Notes API does
	not provide a portable way to create customer-written string resources,
	THEY ARE NOT PORTABLE!  Use the other variants of these routines
	which take "char *" rather than STATUS to achieve portability. */
void LNVARARGS		AddInFormatError (char far *retString, STATUS StringID, ...);
void LNPUBLIC		AddInLogError (STATUS StringID, STATUS AdditionalErrorCode, char far *Arg);
#define AddInLogMsg(s, e) AddInLogError(s, NOERROR, e)
void LNVARARGS		AddInLogMessage (STATUS StringID, STATUS AdditionalErrorCode, ...);
void LNVARARGS		AddInSetStatus (STATUS StringID, ...);

void LNPUBLIC		AddInQueryDefaults(HMODULE far *rethModule, HANDLE far *rethDesc);
void LNPUBLIC		AddInSetDefaults(HMODULE hNewModule, HANDLE hDesc);
HANDLE LNPUBLIC		AddInCreateStatusLine(char far *TaskName);
void LNPUBLIC 		AddInDeleteStatusLine(HANDLE hDesc);
void LNVARARGS		AddInSetStatusLine (HANDLE hDesc, char far *String, ...);

#ifdef __cplusplus
}
#endif

#endif

