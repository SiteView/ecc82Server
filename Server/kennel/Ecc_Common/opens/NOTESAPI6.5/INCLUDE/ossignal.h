#ifdef __cplusplus
extern "C" {
#endif

#ifndef OSSIGNAL_DEFS
#define OSSIGNAL_DEFS

#ifndef NSF_DATA_DEFS		/* Need HCOMPUTE typedef */
#include "nsfdata.h"
#endif

#ifndef MISC_DEFS
#include "misc.h"
#endif

/*	OS Signal Definitions */


#define	OS_SIGNAL_MESSAGE			3	/* Indirect way to call NEMMessageBox */
										/*	STATUS = Proc(Message, OSMESSAGETYPE_xxx) */
typedef STATUS (LNCALLBACKPTR OSSIGMSGPROC)(char far *Message, WORD Type);

#define	OS_SIGNAL_BUSY				4	/* Paint busy indicator on screen */
										/*	STATUS = Proc(BUSY_xxx) */
typedef STATUS (LNCALLBACKPTR OSSIGBUSYPROC)(WORD BusyType);

#define	OS_SIGNAL_CHECK_BREAK		5	/* Called from NET to see if user cancelled I/O */
										/*	STATUS = Proc(void) */
typedef STATUS (LNCALLBACKPTR OSSIGBREAKPROC)(void);


#define	OS_SIGNAL_DIAL				10	/* Prompt to dial a remote system */
										/*  pServer = Desired server name (or NULL) */
										/*  pPort = Desired port name (or NULL) */
										/*  pDialParams = Reserved */
										/*  pRetServer = Actual server name to be called */
										/*				(or NULL if not desired) */
										/*  pRetPort = Actual port name being used */
										/*				(or NULL if not desired) */

typedef STATUS (LNCALLBACKPTR OSSIGDIALPROC)(char far *pServer, 
											   char far *pPort,
											   void far *pDialParams, 
											   char far *pRetServer, 
											   char far *pRetPort);


#define OS_SIGNAL_PROGRESS			13	/* Put up and manipulate the system */
										/* wide progress indicator. */


typedef STATUS (LNCALLBACKPTR OSSIGPROGRESSPROC)(WORD Option, DWORD Data1, DWORD Data2);


#define OS_SIGNAL_REPL				15

typedef void (LNCALLBACKPTR OSSIGREPLPROC)(WORD State, 
											char far *pText1, 
											char far *pText2);


typedef void (LNCALLBACKPTR OSSIGPROC)();	/* Defined only for following lines */
OSSIGPROC LNPUBLIC	OSSetSignalHandler(WORD, OSSIGPROC);
OSSIGPROC LNPUBLIC	OSGetSignalHandler(WORD);

/*	Definitions specific to message signal handler */

#define	OSMESSAGETYPE_OK			0
#define	OSMESSAGETYPE_OKCANCEL		1
#define	OSMESSAGETYPE_YESNO			2
#define	OSMESSAGETYPE_YESNOCANCEL	3
#define	OSMESSAGETYPE_RETRYCANCEL	4
#define	OSMESSAGETYPE_POST			5
#define	OSMESSAGETYPE_POST_NOSERVER	6


/*	Definitions specific to busy signal handler */

#define	BUSY_SIGNAL_FILE_INACTIVE	0
#define	BUSY_SIGNAL_FILE_ACTIVE		1
#define	BUSY_SIGNAL_NET_INACTIVE	2
#define	BUSY_SIGNAL_NET_ACTIVE		3
#define	BUSY_SIGNAL_POLL			4
#define BUSY_SIGNAL_WAN_SENDING		5
#define	BUSY_SIGNAL_WAN_RECEIVING	6


/*	Definitions specific to progress signal handler. */

/*	The progress position will generally start at 0 and end at Range.  
		The current progress supplied is either absolute (SETPOS) or a 
		delta from the previous progress state (DELTAPOS).  
	As the operation which is supplying progress information is peformed,
		the range may change.  If it does, an additional SETRANGE will 
		be signaled.
*/
											/*	Data1					Data2	*/

#define PROGRESS_SIGNAL_BEGIN		0		/*	N/A						N/A		*/
#define PROGRESS_SIGNAL_END			1		/*	N/A						N/A		*/
#define PROGRESS_SIGNAL_SETRANGE	2		/*	Range					N/A		*/
#define PROGRESS_SIGNAL_SETTEXT		3		/*	pText1					pText2 - usually NULL. */
#define PROGRESS_SIGNAL_SETPOS		4		/*	New progress pos		N/A		*/
#define PROGRESS_SIGNAL_DELTAPOS	5		/*	Delta of progress pos	N/A		*/
#define PROGRESS_SIGNAL_SETBYTERANGE	6	/*  Total Bytes */
#define PROGRESS_SIGNAL_SETBYTEPOS		7	/*	Bytes Done	*/


/*  Definitions for replication state signal handler */
											/*	pText1		pText2. */

#define REPL_SIGNAL_IDLE				0	/*	None					*/
#define REPL_SIGNAL_PICKSERVER			1	/*	None					*/
#define REPL_SIGNAL_CONNECTING			2	/*	pServer		pPort		*/
#define REPL_SIGNAL_SEARCHING			3	/*	pServer		pPort		*/
#define REPL_SIGNAL_SENDING				4	/*	pServerFile	pLocalFile	*/
#define REPL_SIGNAL_RECEIVING			5	/*	pServerFile	pLocalFile	*/
#define REPL_SIGNAL_SEARCHINGDOCS		6	/*	pSrcFile				*/
#define REPL_SIGNAL_DONEFILE			7	/*	pLocalFile	pReplFileStats */


/*	End of OS Signal Definitions */

#endif

#ifdef __cplusplus
}
#endif
