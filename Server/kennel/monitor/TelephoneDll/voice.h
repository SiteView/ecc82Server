#ifndef __VOICE_H__
#define __VOICE_H__

#include "afxwin.h"

#include <fcntl.h>
#include <io.h>
#include <winver.h>

#include <srllib.h>
#include <dxxxlib.h>
#include <dtilib.h>
#include <sctools.h>

#include <gclib.h>

#include "voice.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SZ_ARRAY              256
#define WM_USR_ACTIVECHDEV    WM_USER + 1
#define GN_MAX_DIAL_STRING    128
#define GN_DEF_DIAL_STRING    "013366003888"
#define NUMGETDIGITS          5
#define NUMRINGS              2	// this must be at least 2 for U.S. Caller ID 
#define GN_NOT_DIGIT         -2

#define ST_IDLE               0
#define ST_RINGSRECEIVED      1

#define MAX_PHY_BOARD_IN_SYSTEM  10
#define MAX_DEVNAME				 50

#define MAX_TONES             9

#define TITLE_TNGEN           ": Tone Generation template"
#define TITLE_SETVOL          ": Volume setting"
#define TITLE_DXCAP           ": Common DX_CAP parameters"
#define TITLE_WAVEFMT         ": Record WAVE format"

#define DM3 1
#define SP  0

#define YES 1
#define NO  0

typedef struct
{
   int   ca_dtn_pres;   // duration dial tone must be present
   int   ca_dtn_npres;  // time to wait before declaring no dial tone
   int   ca_dtn_deboff; // max gap allowed in an otherwise continuous dial tone
   int   ca_noanswer;   // wait period after first ringback before no answer
   int   ca_intflg;     // PVD and PAMD mode flag
} _CAP;

typedef struct
{
   int   freq;          // frequency Hz
   int   deviation;     // deviation in Hz
} Freq_T;

typedef struct
{
   int   time;          // time in 10ms
   int   deviation;     // deviation in ms
} State_T;

typedef struct
{
   char *   str;        // informational string
   int      tid;        // tone id
   Freq_T   freq1;      // frequency 1
   Freq_T   freq2;      // frequency 2
   State_T  on;         // on time
   State_T  off;        // off time
   int      repcnt;     // repitition count
} Tone_T;

/*
 * Channel status information structure
 */
typedef struct
{
   char name[10];       // channel device name
   HWND hwndChild;      // associated child window handle
   int  funcState;      // function state
   int  xpbIndex;       // index into xpb table
   _CAP callp;          // parameters for PerfectCall
   Tone_T tmplts[MAX_TONES];  // PerfectCall tone definitions
   TN_GEN tngen;        // tone generation template
   DX_IOTT iott;        // I/O transfer table
   int volPos;          // current volume setting for playback
   int CIDState;     // state of Caller ID (0=disabled, 1= enabled, -1 = N/A)
   int AudInState;      // state of Audio In jack (0=disabled, 1= enabled, -1 = N/A)
   DV_DIGIT dig;        // buffer to receive digits
   CHAR text[80];       // text buffer for child window
   int TSdev;        // associated DTI TS handle, if any
   int dti_present;     // is channel associated with DTI handle ?
   int dti_type;        // T1 or E1 ?
   //FOR Dm3 support
   int prod;        //if DM3 OR SP
   LINEDEV linedev;
   char    linedev_name[50];
   CRN crn;
   int blocked;     
} CHINFO;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

DX_XPB xpbWavTbl[] = {
   {FILE_FORMAT_WAVE, DATA_FORMAT_PCM,            DRT_8KHZ,  8},  // default
   {FILE_FORMAT_WAVE, DATA_FORMAT_PCM,            DRT_11KHZ, 8},
   {FILE_FORMAT_WAVE, DATA_FORMAT_DIALOGIC_ADPCM, DRT_6KHZ,  4},
   {FILE_FORMAT_WAVE, DATA_FORMAT_DIALOGIC_ADPCM, DRT_8KHZ,  4}
};

// universal DV_TPT used for playing files
DV_TPT tptplay[] = {{IO_CONT, DX_MAXDTMF, 1, TF_MAXDTMF},
   {IO_EOT, DX_LCOFF, 1, TF_LCOFF}};

/*
 * Perfect Call tone definition table with default values
 * {{description string}, {TID}, {f1,d1}, {f2,d2}, {t1,d1}, {t2,d2}, r}
 */
Tone_T DefaultTones[MAX_TONES] = {
   {{": Local dial tone"}, {TID_DIAL_LCL},  {400,125},  {400,125}, {0,0},     {0,0}, 0},
   {{": Int'l dial tone"}, {TID_DIAL_INTL}, {402,125},  {402,125}, {0,0},     {0,0}, 0},
   {{": Extra dial tone"}, {TID_DIAL_XTRA}, {401,125},  {401,125}, {0,0},     {0,0}, 0},
   {{": Busy tone 1"},     {TID_BUSY1},     {500,200},  {0,0},     {55,40},   {55,40}, 4},
   {{": Busy tone 2"},     {TID_BUSY2},     {500,200},  {500,200}, {55,40},   {55,40}, 4},
   {{": Ringback tone 1"}, {TID_RNGBK1},    {450,150},  {0,0},     {130,105}, {580,415}, 0},
   {{": Ringback tone 2"}, {TID_RNGBK2},    {450,150},  {450,150}, {130,105}, {580,415}, 0},
   {{": Faxtone 1"},       {TID_FAX1},      {2150,150}, {0,0},     {25,-25},  {0,0}, 0},
   {{": Faxtone 2"},       {TID_FAX2},      {1100, 50}, {0,0},     {25,-25},  {0,0}, 0}
};

//number of dti boards
int dticnt = 0;
//active chan device handle based on active child window
int g_ActiveChdev;
// channel status information structure array
CHINFO chinfo[SZ_ARRAY];

// default structures
DX_IOTT iottDflt = {IO_DEV | IO_EOT, 0, 0, 0, 0, -1};    // I/O xfer table
_CAP capDflt = {100, 300, 10, 3000, DX_PVDENABLE};       // PerfectCall params
TN_GEN tngenDflt = {TN_SINGLE, 440, 350, -10, -10, 500}; // tone generation template

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////报警函数
int Warning(char *sChannel,char *sPhoneNum,char *sFileName,int PlayInterval,int PlayNum);

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//API函数
int init();
int OpenChannel(char *sChannel);
int DialNum(char *sNum);
int OnHook();
int PlayWav(char *sFileName);
void DoError(int chdev);
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//报警函数
//函数说明:播放文件大则PlayInterval应该设置大些,PlayNum设置小些
int Warning(char *sChannel,char *sPhoneNum,char *sFileName,int PlayInterval,int PlayNum)
{
	//init();

	printf("OnHook start\n");
	OpenChannel(sChannel);
	OnHook();

	Sleep(3000);
	printf("OnHook end\n");
	
	printf("DialNum star\n");
	OpenChannel(sChannel);
	DialNum(sPhoneNum);
	//等待9秒
	Sleep(9000);
	printf("DialNum end\n");

	int Num = 0;
	while(1)
	{
	
		printf("star play\n");
		PlayWav(sFileName);

		Sleep(PlayInterval*1000);

		Num++;
		if(Num>PlayNum)
			break;
	}
	printf("play end\n");

	printf("star OnHook\n");
	OpenChannel(sChannel);
	OnHook();
	Sleep(9000);
	printf("OnHook end\n");

	return 1;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//子函数

int PlayWav(char *sFileName)
{

	int mode = EV_ASYNC;
	int error = FALSE;
	int multitask = TRUE;    // is function multitasking ?

	// open the file
	if ((chinfo[g_ActiveChdev].iott.io_fhandle = dx_fileopen(sFileName, _O_RDONLY|_O_BINARY)) == -1)
	{
		sprintf(chinfo[g_ActiveChdev].text,"Could not open file %s.",sFileName);
		error = TRUE;
		return 0;
	}

	// clear out digit buffer
	dx_clrdigbuf(g_ActiveChdev);
	sprintf(chinfo[g_ActiveChdev].text,"Playing WAV file: %s...",sFileName);
	if (dx_playiottdata(g_ActiveChdev,&chinfo[g_ActiveChdev].iott,(DV_TPT*)&tptplay,&xpbWavTbl[0],mode) == -1)
	{
		dx_fileclose(chinfo[g_ActiveChdev].iott.io_fhandle);
		DoError(g_ActiveChdev);
		error = TRUE;
	}

	return 1;
}

int OnHook()
{

	int mode = EV_ASYNC;
	int error = FALSE;
	int multitask = TRUE;    // is function multitasking ?

	if (chinfo[g_ActiveChdev].prod == SP )
	{
	 if (chinfo[g_ActiveChdev].AudInState == 1)
	 {
		MessageBox(NULL, "Audio Input Jack must be disabled before changing the channel hook state.", "SetHook error", MB_ICONSTOP);
    
		return 0;
	 }
	 sprintf(chinfo[g_ActiveChdev].text,"Setting Onhook...");
	 if (chinfo[g_ActiveChdev].dti_present != TRUE)
	 {
		if (dx_sethook(g_ActiveChdev,DX_ONHOOK,mode) == -1)
		{
		   DoError(g_ActiveChdev);
		   return 0;
		}
	 }
	 else if (chinfo[g_ActiveChdev].dti_type == CT_NTT1) // DTI present, raise the A bit instead
		if (dt_settssigsim(chinfo[g_ActiveChdev].TSdev,DTB_AOFF|DTB_BOFF) == -1)
		{
		   printf("dt_settssigsim error : %s",ATDV_ERRMSGP(chinfo[g_ActiveChdev].TSdev));
		   DoError(chinfo[g_ActiveChdev].TSdev);
		   return 0;
		}
		else
		{
		   strcat(chinfo[g_ActiveChdev].text," Done!");
		   chinfo[g_ActiveChdev].funcState = ST_IDLE;
		   return 0;
		}

	 else if (chinfo[g_ActiveChdev].dti_type == CT_NTE1) // DTI E1 present
		if (dt_settssigsim(chinfo[g_ActiveChdev].TSdev,DTB_AON) == -1)
		{
		   printf("dt_settssigsim error : %s",ATDV_ERRMSGP(chinfo[g_ActiveChdev].TSdev));
		   DoError(chinfo[g_ActiveChdev].TSdev);
		   return 0;
		}
		else
		{
		   strcat(chinfo[g_ActiveChdev].text," Done!");
		   chinfo[g_ActiveChdev].funcState = ST_IDLE;
		   return 0;
		}
	}
	else
	 MessageBox(NULL, "Not supported on DM3", "SetHook error", MB_ICONSTOP);

	  
	if (!error && multitask)
	{
		chinfo[g_ActiveChdev].funcState = 40002;
	}

	  return 1;
}

int DialNum(char *sNum)
{

	int error = FALSE;
	int mode = EV_ASYNC;
	DX_CAP cap;

	if (chinfo[g_ActiveChdev].prod == SP )
	{
		if (chinfo[g_ActiveChdev].dti_present != TRUE)
		{
			if (ATDX_HOOKST(g_ActiveChdev) == DX_ONHOOK)
			{
			   // Note: For simplicity, we are calling dx_sethook to go offhook
			   // in EV_SYNC mode. This will slow down GUI response time a bit.
			   // A better way would be to call dx_sethook in EV_ASYNC mode.
			   // However, synchronization problems with calling dx_dial would
			   // have to be taken care of.
			   if (dx_sethook(g_ActiveChdev,DX_OFFHOOK,EV_SYNC) == -1)
			   {
				  sprintf(chinfo[g_ActiveChdev].text,"Tried to go offhook for dial...");
				  DoError(g_ActiveChdev);
				  error = TRUE;
				  return 0;
			   }
			}
		}
	 }

	// clear DX_CAP structure
    dx_clrcap(&cap);

	// get parameters for PerfectCall
	cap.ca_dtn_pres = chinfo[g_ActiveChdev].callp.ca_dtn_pres;
	cap.ca_dtn_npres = chinfo[g_ActiveChdev].callp.ca_dtn_npres;
	cap.ca_dtn_deboff = chinfo[g_ActiveChdev].callp.ca_dtn_deboff;
	cap.ca_noanswer = chinfo[g_ActiveChdev].callp.ca_noanswer;
	cap.ca_intflg = chinfo[g_ActiveChdev].callp.ca_intflg;

	if (dx_dial(g_ActiveChdev,sNum,&cap,mode) == -1)
	{
		DoError(g_ActiveChdev);
		error = TRUE;
	}

	return 0;
}

int OpenChannel(char *sChannel)
{
	//
	int parm;
	CT_DEVINFO CtDevinfo;
	
	if ((g_ActiveChdev = dx_open(sChannel, 0)) == -1)
	{
		printf("dx_open failed");
		return 0;
	}

	if (dx_getctinfo(g_ActiveChdev, &CtDevinfo) == -1)
	{
		printf("dx_getctinfo failed");
		return 0;
	}

	chinfo[g_ActiveChdev].prod = SP;
	chinfo[g_ActiveChdev].dti_present = FALSE;
	sprintf(chinfo[g_ActiveChdev].text, "Device opened.");
    strcpy(chinfo[g_ActiveChdev].name, "dxxxB1C1");

	chinfo[g_ActiveChdev].dti_present = FALSE;
	if (dx_setevtmsk(g_ActiveChdev, DM_RINGS) == -1)
	{
		DoError(g_ActiveChdev);
		// not real critical, so continue
	}
	parm = NUMRINGS;
	if (dx_setparm(g_ActiveChdev, DXCH_RINGCNT, &parm) == -1)
	{
		DoError(g_ActiveChdev);
		// not real critical, so continue
	}

	// use default volume SVMT table
    if (dx_setsvmt(g_ActiveChdev, SV_VOLUMETBL, NULL, SV_SETDEFAULT) == -1)
    {
       DoError(g_ActiveChdev);
       // not real critical, so continue
    }

	// determine if channel supports Caller ID
    if (dx_getparm(g_ActiveChdev, DXCH_CALLID, &parm) == 0)
    {
       chinfo[g_ActiveChdev].CIDState = (parm == DX_CALLIDDISABLE) ? 0 : 1;
    }
	else
    {
       // not supported
       chinfo[g_ActiveChdev].CIDState = -1;
    }

	// determine if channel supports Audion Input jack
    if (dx_getparm(g_ActiveChdev, DXCH_AUDIOLINEIN, &parm) == 0)
    {
       chinfo[g_ActiveChdev].AudInState = (parm == DX_LINEINDISABLE) ? 0 : 1;
    }
    else
    {
       // not supported
       chinfo[g_ActiveChdev].AudInState = -1;
    }

	// set default channel information
	chinfo[g_ActiveChdev].volPos = 0;
	chinfo[g_ActiveChdev].callp = capDflt;
	chinfo[g_ActiveChdev].iott = iottDflt;
	chinfo[g_ActiveChdev].tngen = tngenDflt;
	chinfo[g_ActiveChdev].xpbIndex = 0;   // default: 8KHz PCM

	if (chinfo[g_ActiveChdev].prod == SP)
	{
		// use default PerfectCall tone definitions
		for (int index=0; index<MAX_TONES; index++)
		{
		   chinfo[g_ActiveChdev].tmplts[index] = DefaultTones[index];
		}
	}
	// set function state to idle
	chinfo[g_ActiveChdev].funcState = ST_IDLE;

	return 0;
}

void DoError(int chdev)
{
   char buf[80];


   if (ATDV_LASTERR(chdev) == EDX_SYSTEM)
   {
      sprintf(buf,"\nSystem error: %d",errno);
      strcat(chinfo[chdev].text,buf);
   }
   else
   {
      sprintf(buf,"\nError: ");
      strcat(buf,ATDV_ERRMSGP(chdev));
      strcat(chinfo[chdev].text,buf);
   }
}

int init()
{
	//int          brdcnt;    // number of voice boards in system
	//// query for number of voice boards in system
	//sr_getboardcnt("Voice", &brdcnt);

	//sr_getboardcnt("DTI", &dticnt);    // query number of dti boards

	//int			 tslotcnt;
	//int          bddev;
	//char         buf[15];
	//int          index;
	//int			 cnt;
	//int          subdevs;
	//int			 chncnt;
	//long		 retVal;
	//SRLDEVICEINFO   *chnamesp = NULL;
	//SRLDEVICEINFO   *dtnamesp = NULL;

	//for (index=1; index<=brdcnt; index++)
	//{
	//  // open board to query number of channels
	//  sprintf(buf,"dxxxB%d",index);
	//  if ((bddev = dx_open(buf, 0)) == -1)
	//  {
	//	 printf("Error during startup\n");
	//	 return 1;
	//  }

	//  // get channel device names
	//  //     subdevs = ATDV_SUBDEVS(bddev);
	//  //      chnamesp = ATDX_CHNAMES(bddev);
	//  subdevs = 0;
	//  do
	//  {
	//	 if (subdevs)
	//	 {
	//		free(chnamesp);
	//		chnamesp = (SRLDEVICEINFO *)malloc(subdevs * sizeof(SRLDEVICEINFO)); 
	//	 }
	//	 else
	//	 {
	//		chnamesp = NULL;
	//	 }
	//	 retVal = SRLGetSubDevicesOnVirtualBoard(buf, &subdevs, chnamesp);
	//  }   while( ESR_INSUFBUF   ==   retVal);
	//  
	//  if (ESR_NOERR != retVal)
	//  {
	//	 printf("Error during startup\n");
	//	 return 1;
	//  }

	//  // put channel device names in table
	//  for (cnt=0; cnt<(int)subdevs; cnt++)
	//  {
	//	  CString temp;
	//	  temp = (chnamesp + cnt)->szDevName;
	//	 //strcpy(dlgctrls[chncnt].str, (chnamesp + cnt)->szDevName );
	//	 //dlgctrls[chncnt].ctrlId = IDC_COMBO1;
	//	 //dlgctrls[chncnt].def = FALSE; 
	//	 chncnt++;
	//  }

	//  dx_close(bddev);

	//}


	////Put DTI device names in the table
 //  for (int dtibrd=1;dtibrd<=dticnt;dtibrd++)
 //  {
 //     // open board to query number of tslots
 //     printf(buf,"dtiB%d",dtibrd);
 //     if ((bddev = dt_open(buf, 0)) == -1)
 //     {
 //        printf("Error during startup\n");
 //        return 1;
 //     }

 //     // put tslot device names in table
 //     //      subdevs = ATDV_SUBDEVS(bddev);
 //     // Get dti subdevs by passing subdevs = 0 so the SRLGetSubDevicesOnVirtualBoard() can returns 
 //     // Number of dit subdevices So here it should always return ESR_INSUFBUF
 //     subdevs = 0;
 //     dtnamesp = NULL;
 //     retVal = SRLGetSubDevicesOnVirtualBoard(buf, &subdevs, dtnamesp);
 //     if (ESR_INSUFBUF != retVal)
 //     {
 //        printf("Error during startup\n");
 //        return 1;
 //     }


 //     for (cnt=0; cnt<(int)subdevs; cnt++)
 //     {
 //        if (tslotcnt > chncnt)
 //           break;      // make sure we don't go beyond allocation

	//	 char dtistr[25] = {0};
 //        printf(dtistr,"dtiB%dT%d",dtibrd,cnt+1);
 //        tslotcnt++;
 //     }

 //     dt_close(bddev);
 //  }

	return 0;
}

#endif /* !__QUEUE_H__ */