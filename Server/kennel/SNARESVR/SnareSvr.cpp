// SNARE - Audit / EventLog analysis and forwarding
// Copyright 2001-2003 InterSect Alliance Pty Ltd
// http://www.intersectalliance.com/
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Library General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// See Readme.txt file for more information.
//

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <time.h>
#include <tchar.h>
#include <limits.h>
#include <process.h>
#include <sys/stat.h>

#include "NTServApp.h"
#include "SnareSvr.h"

#include <svapi.h>
#include <svdbapi.h>

// Global Variables.
ThreadStruct	g_Info;
int				SNAREDEBUG=1;

CRITICAL_SECTION csFilter; 
map<string, int, less<string> > mapFacility;
map<string, int, less<string> > mapSeverities;

UINT hTime;
DWORD dwSleep = 0;
bool bSet = true;
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime);
string strKeepDay;
//////////////SysLog��־���ݿ�Ĳ��뺯��///////

using namespace std;
using namespace svutil;
//
bool ParserToken(list<string >&pTokenList, const char * pQueryString, char *pSVSeps)
{
    char * token = NULL;
    // duplicate string
	char * cp = ::strdup(pQueryString);
    if (cp)
    {
        char * pTmp = cp;
        if (pSVSeps) // using separators
            token = strtok( pTmp , pSVSeps);
        else // using separators
			return false;
            //token = strtok( pTmp, chDefSeps);
        // every field
        while( token != NULL )
        {
            //triml(token);
            //AddListItem(token);
			pTokenList.push_back(token);
            // next field
            if (pSVSeps)
                token = strtok( NULL , pSVSeps);
            else
               return false;
				//token = strtok( NULL, chDefSeps);
        }
        // free memory
        free(cp);
    }
    return true;
}

//
struct RecordHead
{

    int prercord;

    int state;

    TTime createtime;

    int datalen;
}; 

//
char *buildbuf(int data,char *pt,int buflen)
{

    if(pt==NULL)
        return NULL;

    if(buflen<sizeof(int))
        return NULL;

    memmove(pt,&data,sizeof(int));

    pt+=sizeof(int);

    return pt;
}

//
char *buildbuf(float data,char *pt,int buflen)
{

    if(pt==NULL)

        return NULL;

    if(buflen<sizeof(float))

        return NULL;

    memmove(pt,&data,sizeof(float));

    pt+=sizeof(float);

    return pt;
}

//
char *buildbuf(string data,char *pt,int buflen)
{

    if(pt==NULL)

        return NULL;

    if(buflen<data.size()+1)

        return NULL;

    strcpy(pt,data.c_str());

    pt+=data.size();

    pt[0]='\0';

    pt++;

    return pt;
}

//���¼��SysLog��
bool InsertRecord(string strTableName, int nSysLogIndex, string strTime, string strSrcIp, string strSysLogMsg, int nFicility, int nLevel)
{
    char data[1024]={0};

    RecordHead *prd=(RecordHead*)data;

    char *pt=data+sizeof(RecordHead);

    char *pm=NULL;
	
	if((pm=::buildbuf(nSysLogIndex,pt,1024))==NULL)
    {
        //puts("build alertindex failed");

        return false;
	}
	
	if((pm=::buildbuf(strTime,pm,1024))==NULL)
    {
        //puts("build alerttime failed");

        return false;
	}
    
	if((pm=::buildbuf(strSrcIp,pm,1024))==NULL)
    {
        //puts("build rulename failed");

        return false;
	}

	if((pm=::buildbuf(strSysLogMsg,pm,1024))==NULL)
    {
        //puts("build entityname failed");

        return false;
	}
	
	if((pm=::buildbuf(nFicility,pm,1024))==NULL)
    {
        //puts("build monitorname failed");

        return false;
	}
	
	if((pm=::buildbuf(nLevel,pm,1024))==NULL)
    {
        //puts("build alertreceivename failed");

        return false;
	}

    prd->datalen=pm-pt;

    prd->state=1;

    prd->createtime=svutil::TTime::GetCurrentTimeEx();

    strcpy(pm,"DynString");

    int len=pm-data;

    len+=strlen(pm)+1;

    if(!::AppendRecord(strTableName,data,len))
    {
        //puts("Append record failed");
        return false;
    }
	//else
 //       puts("Append OK");

	return true;
}

//////////////////////////////////////////////////////


struct _node {
	char Line[MAX_EVENT];	// Could malloc and free this instead.
	int LogType;			// 0 for snare, 1 for syslog?
	char IPAddress[64];		// technically, we only need 16 for ipv4..
	struct _node *next;
};

typedef struct _node Node;

// Linked list. NOTE: ALL Changes to these variables should be locked in a Mutex.
static Node *head = NULL;
static Node *tail = NULL;
static Node *currentnode = NULL;
int NodeCounter = 0;

unsigned long SnarePackets=0;
unsigned long SyslogPackets=0;


// Locker
HANDLE hMutex;


//
CSnareSvrService::CSnareSvrService()
:CNTService("SNARESVR")
{
	// Initialize class members here
}

//
BOOL CSnareSvrService::OnInit()
{
	// Perform any initialization that needs to be done before intering the main loop
	char szError[MAX_EVENT];
	
	/*SNAREDEBUG=this->DEBUGSET;*/
	
	// Save this off so we can use it to write stuff to the registry.

	if( !InitWinsock( szError ) )
	{
		if(SNAREDEBUG) 
		{
			DebugMsg(szError); 
		}
		return FALSE;
	}

	m_hEventList[0] = CreateEvent(NULL, FALSE, FALSE, NULL); // terminate signal
	// m_hEventList[1] = CreateEvent(NULL, FALSE, FALSE, NULL); // terminate signal for 514
	m_hEventList[1] = CreateEvent(NULL, FALSE, FALSE, NULL); // PIPE server
	m_hEventList[2] = CreateEvent(NULL, FALSE, FALSE, NULL); // PIPE writer
	
	if(m_hEventList[0] == NULL || m_hEventList[1] == NULL || m_hEventList[2] == NULL) 
	{
		if(SNAREDEBUG) 
		{ 
			DebugMsg("CreateEvent() failed"); 
		}
		return FALSE;
	}

	hMutex = CreateMutex(NULL,FALSE,"SnareLock");
	if(hMutex == NULL)
	{
		if(SNAREDEBUG) 
		{
			DebugMsg("I cannot create the Snare Server 'Mutex' lock. This probably means that you already have another instance of the Snare Micro Server running.\nPlease stop the other incarnation of the Snare Micro Server (eg: net stop snaresvr) before continuing."); 
		}
		return FALSE;
	}

	
	if(SNAREDEBUG)
	{
		DebugMsg("SNARE Micro Server - Initialisation complete"); 
	}
	// return FALSE here if initialization failed & the service shouldn't start

	bool bInsert = InsertTable("syslog", 802);
	if(bInsert)
	{
		OutputDebugString(" InsertTable ok");	
	}
	else
	{
		OutputDebugString(" InsertTable failed");			
	}

	return TRUE;
}

//
void CSnareSvrService::InitFilterParam()
{
	InitializeCriticalSection(&csFilter);
	string strSeveritiesValue = "", strFacilityValue = "", strTmp = "";
	strFacilityValue = GetIniFileString("QueryCond", "Facility", "", "syslog.ini");	
	strSeveritiesValue = GetIniFileString("QueryCond", "Severities", "", "syslog.ini");	

	std::list<string> pTemptList;
	list <string>::iterator listitem;

	ParserToken(pTemptList, strFacilityValue.c_str(), ",");		
	for(listitem = pTemptList.begin(); listitem != pTemptList.end(); listitem++)
	{
		mapFacility[(*listitem)] = 0;
	}	

	pTemptList.clear();	
	ParserToken(pTemptList, strSeveritiesValue.c_str(), ",");		
	for(listitem = pTemptList.begin(); listitem != pTemptList.end(); listitem++)
	{
		mapSeverities[(*listitem)] = 0;
	}

	//�趨��ʱˢ��Ƶ��:1��
	svutil::TTime time = svutil::TTime::GetCurrentTimeEx();
	int nMinute = 60 - time.GetMinute();
	int nHour = 23 - time.GetHour();
	
	dwSleep =  (nHour * 60 + nMinute) * 60 * 1000;
	//hTime = SetTimer(NULL, NULL, (nHour * 60 + nMinute) * 60 * 1000, TimerProc);

	//MSG msg;
	//while (GetMessage(&msg, 0, 0, 0))
	//	DispatchMessage(&msg);
	//hTime = SetTimer(NULL, 1234, 5000, (TIMERPROC)TimerProc);

	strKeepDay = GetIniFileString("DelCond", "KeepDay", "", "syslog.ini");
	if(strKeepDay == "")
		strKeepDay = "0";
	DebugMsg(strKeepDay.c_str());
}

//
void CSnareSvrService::Run()
{
	InitFilterParam();
	// Define an eventlogrecord buffer of 8k.
	// Should be enough for an overwhelming majority of circumstances.
	// TCHAR EventLogRecordBuffer[MAX_EVENT]="";
	TCHAR EventLogRecordBuffer2[MAX_EVENT]="";
	char *pEvent=NULL;

	OVERLAPPED Overlapped;	// PIPE variable
	OVERLAPPED OverlappedWrite;	// PIPE variable

	// Destination for log events. Default it to something safe.
	// TCHAR lpszDestination[512]="127.0.0.1";

//	int retval;

//	SOCKET snare_socket;	// 6161
//	SOCKET syslog_socket;	// 514
//	SOCKADDR_IN snare_in;
//	SOCKADDR_IN syslog_in;

//	SOCKADDR_IN recv_socket;
//	int recv_socketlen=sizeof(recv_socket);
	
//	char *ipbuff;
//	char ipbuffer[16];

	char SourceName[256]="";
	char LastSourceName[256]="";
	char LogType[256]="";
	char LastLogType[256]="";
	int criticality=0;
	char Date[256]="";
	char StartDate[256]="";
	char CurrentDateTime[256]="";

	char Path[4096]=""; // 4k path
	char Filename[4096]="";

	struct tm *LocalTime1=NULL;
	int tyear=0,tday=0;

	time_t CurrentTime=0;
	time_t LastTime=0;
	
	int DateChanged=0;
	int LogTypeChanged=0;
	int SourceNameChanged=0;


	DWORD dwWaitRes=0;
	BOOL WritePipeConnected=FALSE;
	
	FILE * OutputFile=(FILE *)NULL;

	//hWritePipe=INVALID_HANDLE_VALUE;

	DebugMsg("SNARE Micro Server is now active");

	// READ in our configuration
//	DWORD SnarePort=MyGetProfileDWORD("Config","SnarePort",6161);
	// OUTPUT_TYPE_DATE=1  OUTPUT_TYPE_DATE_SVR=2 
	// OUTPUT_TYPE_DATE_LOG=3  OUTPUT_TYPE_DATE_LOG_SVR=4
	DWORD OutputType=MyGetProfileDWORD("Config","OutputType",OUTPUT_TYPE_DATE);	
	
	if(!MyGetProfileString("Config","Path",Path,sizeof(Path))) 
	{
		// Default to a reasonably sane value
		DebugMsg("No Path defined. Using SystemRoot\\System32\\LogFiles");
		ExpandEnvironmentStrings("%SystemRoot%\\system32\\LogFiles",Path,sizeof(Path));
	}

	if(strlen(Path) < 3) 
	{
		DebugMsg("Path shorter than 3 characters. Using SystemRoot\\System32\\LogFiles");
		ExpandEnvironmentStrings("%SystemRoot%\\system32\\LogFiles",Path,sizeof(Path));
	}

	if(DirExists(Path) != TRUE) 
	{
		// Hmm.. might be on a XP machine. Try the fallback.
		DebugMsg("Path not found. Falling back to the XP Default..");
		ExpandEnvironmentStrings("%SystemRoot%\\security\\Logs",Path,sizeof(Path));
		if(DirExists(Path) != TRUE) 
		{
			DebugMsg("Directory specified in the confiruation registry (%s) does not exist! Cannot continue.",Path);
			return;
		}
	}

	CurrentTime=time(NULL);
	LocalTime1 = localtime(&CurrentTime);
	strftime(StartDate, sizeof(StartDate),"%Y%m%d %H:%M:%S", LocalTime1);

/*
	// Set up our network ports.
	snare_socket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
	syslog_socket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if(!snare_socket || !syslog_socket) {
		DebugMsg("Cannot open sockets - Exiting.");
		return;
	}

	snare_in.sin_family=AF_INET;
	snare_in.sin_port = htons((u_short)SnarePort);
	snare_in.sin_addr.s_addr=htonl(INADDR_ANY);

	if(bind(snare_socket,(SOCKADDR *)&snare_in, sizeof(snare_in)) != 0) {
		DebugMsg("Bind Failed - snare socket. Do you already have something running on the snare UDP port?");
		return;
	}

	syslog_in.sin_family=AF_INET;
	syslog_in.sin_port = htons((u_short)514);
	syslog_in.sin_addr.s_addr=htonl(INADDR_ANY);

	if(bind(syslog_socket,(SOCKADDR *)&syslog_in, sizeof(syslog_in)) != 0) {
		DebugMsg("Bind Failed - syslog socket. Do you already have something running on the syslog UDP port?");
		return;
	}
	*/

	// Ok, zap up our threads.
	//if(_beginthread(SnareThread, 0, (HANDLE) m_hEventList[0] )==-1) {
	//	DebugMsg("Error in thread creation");
	//	return;
	//}
	if(!::CreateQueue("SiteView70-SysLog",1))
		OutputDebugString("Create SiteView70-SysLog queue failed");
	else
		OutputDebugString("Create SiteView70-SysLog ok");

	if(_beginthread(ReceiveThread, 0, (HANDLE) m_hEventList[0] )==-1) 
	{
		DebugMsg("Error in ReceiveThread thread creation");
		return;
	}
	
	if(_beginthread(SyslogThread, 0, (HANDLE) m_hEventList[0] )==-1) 
	{
		DebugMsg("Error in thread creation");
		return;
	}

	if(_beginthread(WinMsgThread, 0, (HANDLE) m_hEventList[0] )==-1) 
	{
		DebugMsg("Error in thread creation");
		return;
	}

	// THESE TWO GO INTO THE SECOND THREAD (or second/third??)
	// HMM... USE TWO THREADS - recvfrom will block until data arrives! :)
	// need to _beginthread, and pass a pointer to current?
	// actually, current should be a global. Remember to lock down before
	// current changes anywhere.
	// note: use select/recvfrom, and look for a shutdown signal every loop.

	//if(WSAEventSelect(snare_socket,m_hEventList[0],FD_READ) != 0) {
	//	DebugMsg("WSAEventSelect Failed - I cannot initialise the events that I need in order to monitor the network sockets");
	//	return;
	//}
	//if(WSAEventSelect(syslog_socket,m_hEventList[1],FD_READ) != 0) {	
	//	DebugMsg("WSAEventSelect Failed - I cannot initialise the events that I need in order to monitor the network sockets");
	//	return;
	//}

	if(SNAREDEBUG) 
	{ 
		DebugMsg("Creating named pipe."); 
	}

	//// Create the pipe here. If we create it earlier, then there's a chance that
	//// The GUI could lock onto the pipe before we have an event handler in place.
	//hPipe = CreateNamedPipe("\\\\.\\PIPE\\SnareSvr",PIPE_ACCESS_OUTBOUND|FILE_FLAG_OVERLAPPED,
	//						PIPE_WAIT,1,5000,5000,0,NULL);
	//
	//hWritePipe = CreateNamedPipe("\\\\.\\PIPE\\SnareSvrGUI",PIPE_ACCESS_OUTBOUND|FILE_FLAG_OVERLAPPED,
	//						PIPE_WAIT,1,5000,5000,0,NULL);

	//
	//if (hPipe == INVALID_HANDLE_VALUE || hWritePipe == INVALID_HANDLE_VALUE) {
	//	DebugMsg("Error in CreateNamedPipe = %d - This is not a critical problem.",GetLastError());
	//	// return FALSE; // This is not a terminal problem.
	//}


	//// Monitor the pipe
	//if (hPipe != INVALID_HANDLE_VALUE) {
	//	int piperc;
	//	Overlapped.hEvent = m_hEventList[1];
	//	// Despite what is written in the MS API, these values **_MUST_** be set
	//	Overlapped.Offset = 0;
	//	Overlapped.OffsetHigh = 0;

	//	if (ConnectNamedPipe(hPipe,&Overlapped) == 0) {
	//		piperc = GetLastError();
	//	}
	//
	//	if ((piperc != ERROR_IO_PENDING) && 
	//		(piperc != ERROR_PIPE_CONNECTED))
	//	{
	//		// Not going to be able to do any pipe transactions.
	//		hPipe=INVALID_HANDLE_VALUE;

	//	}
	//	// OK pipe connected.
	//	if(SNAREDEBUG) { DebugMsg("Pipe connected."); }
	//}

	//if (hWritePipe != INVALID_HANDLE_VALUE) {
	//	int piperc;
	//	OverlappedWrite.hEvent = m_hEventList[2];
	//	// Despite what is written in the MS API, these values **_MUST_** be set
	//	OverlappedWrite.Offset = 0;
	//	OverlappedWrite.OffsetHigh = 0;

	//	if (ConnectNamedPipe(hPipe,&OverlappedWrite) == 0) {
	//		piperc = GetLastError();
	//	}
	//
	//	if ((piperc != ERROR_IO_PENDING) && 
	//		(piperc != ERROR_PIPE_CONNECTED))
	//	{
	//		// Not going to be able to do any pipe transactions.
	//		hWritePipe=INVALID_HANDLE_VALUE;

	//	}
	//	// OK pipe connected.
	//	if(SNAREDEBUG) { DebugMsg("Write Pipe connected."); }
	//}


	// Set the terminate flag to zero.
	// setting this value to TRUE will terminate the service,
	// and ask it to save it's current status.
	g_Info.bTerminate = FALSE;

	
	if(SNAREDEBUG) 
	{ 
		DebugMsg("Entering main loop."); 
	}

	// This is the service's main run loop.
    while (m_bIsRunning) 
	{
	//	MSG msg;
	//	while(GetMessage(&msg, NULL, 0, 0))
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//		switch(msg.message )
	//		{
	//			case WM_QUIT:
	//				goto quit;
	//			default:
	//				break;
	//		}
	//	}
	//quit:
		// If we have been asked to terminate, do so.
		if(g_Info.bTerminate)
		{
			m_bIsRunning=0;
			break;
		}	
		dwWaitRes=WaitForMultipleObjects(3,m_hEventList,FALSE,1000);

	
		if(dwWaitRes == WAIT_FAILED)
		{
			// Keep spinning.
			continue;
		}
		else if(dwWaitRes == WAIT_OBJECT_0 || g_Info.bTerminate == TRUE) 
		{
			m_bIsRunning=0;
			g_Info.bTerminate=1;
			// Terminate!!
			break;
		}
		else if (dwWaitRes == WAIT_OBJECT_0 + 1) 
		{
			// PIPE event.
			int piperc;
			if(SNAREDEBUG)
			{ 
				DebugMsg("Pipe Event received"); 
			}

			
			//if (hPipe != INVALID_HANDLE_VALUE) {
			//	int pipecount=0;
			//	char str_temp[500] = "";
			//	DWORD dwWritten;

			//	// We can now write to the pipe, since we have a connected client
			//	_snprintf(str_temp,sizeof(str_temp),"SNARE Status:\n" \
			//			"SNARE is currently ACTIVE\nStarted at %s\nReceived on Snare Port: %ld\nReceived on Syslog Port: %ld\n\n",
			//			StartDate,SnarePackets,SyslogPackets);

			//	// If lpOverlapped is not NULL, lpNumberOfBytesWritten can be NULL.
			//	if (WriteFile(hPipe,str_temp,strlen(str_temp),NULL,&Overlapped) == 0)	{
			//		int GLError;
			//		GLError=GetLastError();
			//		if(SNAREDEBUG) { DebugMsg("WriteFile error = %d",GLError); }
			//		if (GLError == ERROR_IO_PENDING) {
			//			if (GetOverlappedResult(hPipe,&Overlapped,&dwWritten,TRUE) == 0) {
			//				if(SNAREDEBUG) { DebugMsg("Error in GetOverlappedResult = %d",GetLastError()); }
			//			} else {
			//				if(SNAREDEBUG) { DebugMsg("GetOverlappedResult bytes written = %d",dwWritten); }
			//			}
			//		}
			//	} else {
			//		if(SNAREDEBUG) { DebugMsg("Successfull WriteFile"); }
			//	}


			//	// Done. Close down the pipe, and reconnect it.
			//	DisconnectNamedPipe(hPipe);
			//		
			//	if (ConnectNamedPipe(hPipe,&Overlapped) == 0) {
			//		piperc = GetLastError();
			//	}
	
			//	if ((piperc != ERROR_IO_PENDING) && 
			//		(piperc != ERROR_PIPE_CONNECTED))
			//	{
			//		// Not going to be able to do any pipe transactions.
			//		hPipe=INVALID_HANDLE_VALUE;
			//	}
			//}
			
			continue;
		}
		else if (dwWaitRes == WAIT_OBJECT_0 + 2) 
		{
			//ResetEvent(m_hEventList[2]);
			WritePipeConnected=TRUE;
		}
		else if(dwWaitRes == WAIT_TIMEOUT) 
		{
//			Node * lastnode;

			// Ok, scan through our linked list, and zap stuff into the right place.
			//dwWaitRes = WaitForSingleObject(hMutex,0);
			//if(dwWaitRes != WAIT_OBJECT_0) {
			//	// Cannot grab the mutex at the moment. Try again in another second.
			//	// Keep spinning.
			//	continue;
			//}
			// Ok, we have the lock. Grab a pointer to the last element.
			//lastnode=tail;
			//ReleaseMutex(hMutex);

			currentnode=head;

			//if(currentnode && hPipe != INVALID_HANDLE_VALUE) 
			//{
			//	// We're about to enter a while loop, so don't respond to GUI
			//	// status requests for a little bit.
			//	// Tell the GUI to break off.
			//	DisconnectNamedPipe(hPipe);
			//}

			while(currentnode) 
			{
				// Add this to our GUI, and zap it into a file.
				// Note: we don't have to lock stuff here, until we remove the node.


				// HERE: DISABLE THE PIPE SOMEHOW???????

				//DateChanged=0;
				//
				//CurrentTime=time(NULL);
				//if(LastTime) 
				//{
				//	LocalTime1 = localtime(&LastTime);
				//	if(LocalTime1) 
				//	{
				//		tyear=LocalTime1->tm_year;
				//		tday=LocalTime1->tm_yday;
				//	}
				//	else 
				//	{
				//		LastTime=0;
				//	}
				//}
				//LocalTime1 = localtime(&CurrentTime);

				//if(LastTime) 
				//{
				//	if(LocalTime1) 
				//	{
				//		if(tday != LocalTime1->tm_yday || tyear != LocalTime1->tm_year) 
				//		{
				//			DateChanged=1;
				//			strftime(Date, sizeof(Date),"%Y%m%d", LocalTime1);
				//		}
				//	} 
				//	else 
				//	{
				//		DateChanged=1;
				//		strftime(Date, sizeof(Date),"%Y%m%d", LocalTime1);
				//	}
				//}
				//else 
				//{
				//	DateChanged=1;
				//	strftime(Date, sizeof(Date),"%Y%m%d", LocalTime1);
				//}
				//
				//LastTime=CurrentTime;
				//strftime(CurrentDateTime, sizeof(CurrentDateTime),"%d %b %Y %H:%M:%S", LocalTime1);
				

				int OutputLength=strlen(currentnode->Line);
				pEvent=currentnode->Line;
				if(OutputLength) 
				{
					// Kill off any final newlines
					char *stringp=&currentnode->Line[OutputLength-1];
					while(stringp > currentnode->Line) 
					{
						if(*stringp=='\n') 
						{
							*stringp='\0';
							OutputLength--;
						}
						else 
						{
							break;
						}
					}
				

					strncpy(SourceName,currentnode->IPAddress,sizeof(SourceName));
					
					criticality=0;

					if(currentnode->LogType == 0) 
					{
						char *tabpos;
						int copysize=0;

						strncpy(LogType,"Generic",sizeof(LogType));
						// 6161. There should be source DNS, type (criticality)
						stringp=currentnode->Line;

						// Is this from a snare reflector? If so, there's probably a [ip.address]\t at the
						// start of the message.
						if(*stringp == '[') 
						{
							stringp++;
							tabpos=strstr(stringp,"	");
							if(tabpos) 
							{
								copysize=tabpos-stringp-1;
								if(copysize > sizeof(SourceName)) 
								{
									copysize=sizeof(SourceName);
								}

								strncpy(SourceName,stringp,copysize);
								SourceName[copysize]='\0';
								stringp=tabpos+1;
								pEvent=tabpos+1;
							} 
							else 
							{
								stringp--;
							}
						}
						

						tabpos=strstr(stringp,"	");
						if(tabpos) 
						{
							pEvent=tabpos+1;
							copysize=tabpos-stringp;
							if(copysize > sizeof(SourceName)) 
							{
								copysize=sizeof(SourceName);
							}

							strncpy(SourceName,stringp,copysize);
							SourceName[copysize]='\0';

							stringp=tabpos+1;
							tabpos=strstr(stringp,"	");
							if(tabpos) 
							{
								pEvent=tabpos+1;
								copysize=tabpos-stringp;
								if(copysize > sizeof(LogType)) 
								{
									copysize=sizeof(LogType);
								}
								strncpy(LogType,stringp,copysize);
								
								stringp=tabpos+1;
								if((*stringp>='0' && *stringp <= '9') && *(stringp+1) == '	') 
								{
									char critstr[2];
									critstr[0]=*stringp;
									critstr[1]='\0';
									criticality=atoi(critstr);
									pEvent=stringp+2;
								}
							}
						}
					}
					else 
					{
						char *tabpos;
						int copysize=0;

						pEvent=currentnode->Line;
						stringp=currentnode->Line;
						// Is this from a snare reflector? If so, there's probably a [ip.address]\t at the
						// start of the message.
						if(*stringp == '[') 
						{
							stringp++;
							tabpos=strstr(stringp,"	");
							if(tabpos) 
							{
								copysize=tabpos-stringp-1;
								if(copysize > sizeof(SourceName)) 
								{
									copysize=sizeof(SourceName);
								}
								strncpy(SourceName,stringp,copysize);
								SourceName[copysize]='\0';
								stringp=tabpos+1;
								pEvent=tabpos+1;
							}
							else 
							{
								stringp--;
							}
						}
						
						strncpy(LogType,"Syslog",sizeof(LogType));
						criticality=5;	// Syslog
					}

					if(pEvent > currentnode->Line+strlen(currentnode->Line)) 
					{
						// Oh dear - past the end. Jump back.
						pEvent=currentnode->Line+strlen(currentnode->Line);
					}
					
					//if(OutputType == OUTPUT_TYPE_DATE_SVR || OutputType == OUTPUT_TYPE_DATE_LOG_SVR) 
					//{
					//	if(strcmp(SourceName,LastSourceName)) 
					//	{
					//		SourceNameChanged=1;
					//		strncpy(LastSourceName,SourceName,sizeof(LastSourceName));
					//	}
					//	else 
					//	{
					//		SourceNameChanged=0;
					//	}
					//}
					//else 
					//{
					//	SourceNameChanged=0;
					//}

					//if(OutputType == OUTPUT_TYPE_DATE_LOG || OutputType == OUTPUT_TYPE_DATE_LOG_SVR) 
					//{
					//	if(strcmp(LogType,LastLogType)) 
					//	{
					//		LogTypeChanged=1;
					//		strncpy(LastLogType,LogType,sizeof(LastLogType));
					//	}
					//	else 
					//	{
					//		LogTypeChanged=0;
					//	}
					//} 
					//else 
					//{
					//	LogTypeChanged=0;
					//}

					//if(DateChanged || SourceNameChanged || LogTypeChanged) 
					//{
					//	// Close the old filename.
					//	if(OutputFile) 
					//	{
					//		fclose(OutputFile);
					//	}
					//	// OUTPUT_TYPE_DATE=1  OUTPUT_TYPE_DATE_SVR=2
					//	// OUTPUT_TYPE_DATE_LOG=3  OUTPUT_TYPE_DATE_LOG_SVR=4
					//	if(OutputType == OUTPUT_TYPE_DATE) 
					//	{
					//		_snprintf(Filename,sizeof(Filename),"%s\\%s.log",Path,Date);
					//	}
					//	else if(OutputType == OUTPUT_TYPE_DATE_SVR) 
					//	{
					//		_snprintf(Filename,sizeof(Filename),"%s\\%s-%s.log",Path,Date,SourceName);
					//	}
					//	else if(OutputType == OUTPUT_TYPE_DATE_LOG) 
					//	{
					//		_snprintf(Filename,sizeof(Filename),"%s\\%s-%s.log",Path,Date,LogType);
					//	}
					//	else if(OutputType == OUTPUT_TYPE_DATE_LOG_SVR) 
					//	{
					//		_snprintf(Filename,sizeof(Filename),"%s\\%s-%s-%s.log",Path,Date,LogType,SourceName);
					//	}


					//	if(SNAREDEBUG)
					//	{
					//		DebugMsg("Opening file %s\n",Filename); 
					//	}
					//	
					//	OutputFile=fopen(Filename,"a");
					//	
					//	if(!OutputFile) 
					//	{
					//		DebugMsg("Snare Cannot open output file %s",Filename);
					//		strcpy(LastSourceName,"");
					//		strcpy(LastLogType,"");
					//	}
					//}

					//if(SNAREDEBUG)
					//{
					//	DebugMsg("Sending log to file: %s",currentnode->Line); 
					//}					
					//	
					//if(OutputFile) 
					{
						try 
						{
							//fprintf(OutputFile,"%s\n",currentnode->Line);
							//fflush(OutputFile);
							
							//����
							string strLine = currentnode->Line;
							string strFicility = "", strLevel = "";
							int i = strLine.find(">");
							string strHead = strLine.substr(1, i - 1);
							int nHead = 0;
							sscanf(strHead .c_str(), "%d", &nHead);
							nFicility = nHead / 8;
							nLevel = nHead % 8;

							char tmpBuf[12]  = {0};
							sprintf(tmpBuf, "%d", nFicility);
							strFicility = tmpBuf;

							sprintf(tmpBuf, "%d", nLevel);
							strLevel = tmpBuf;
							
							EnterCriticalSection(&csFilter); 
							//����ƥ��
							if((mapFacility.find(strFicility) != mapFacility.end()) && (mapSeverities.find(strLevel) != mapSeverities.end()))
							{
								TTime curTime = TTime::GetCurrentTimeEx();
								InsertRecord("syslog", 0, curTime.Format(), currentnode->IPAddress, currentnode->Line, nFicility, nLevel);
							}
							LeaveCriticalSection(&csFilter);
						}
						catch (...) 
						{
							DebugMsg("Cannot write line to outputfile! Exception recorded.\n");
						}
					}
					
				//	if(hWritePipe != INVALID_HANDLE_VALUE && WritePipeConnected==TRUE) 
				//	{
				//		// Send data to the GUI, if the write-pipe is open.
				//		// Make sure we include the maximum criticality.
				//		DWORD dwWriteSize=0;

				//		if(SNAREDEBUG)
				//		{
				//			DebugMsg("Sending data to the open GUI Pipe...."); 
				//		}	
				//		
				//		_snprintf(EventLogRecordBuffer2,sizeof(EventLogRecordBuffer2),"%s	%s	%s	%d	%s\0",CurrentDateTime,SourceName,LogType,criticality,pEvent);
				//		
				//		if(SNAREDEBUG) 
				//		{
				//			int tsize=strlen(EventLogRecordBuffer2);
				//			if(SNAREDEBUG)
				//			{
				//				DebugMsg("Sending: [%s] - %d bytes",EventLogRecordBuffer2,tsize); 
				//			}
				//		}


				//		// DEBUG: This is probably too CPU intensive to run in a loop.
				//		//DisconnectNamedPipe(hPipe);


				//		if(WriteFile(hWritePipe,EventLogRecordBuffer2,strlen(EventLogRecordBuffer2)+1,&dwWriteSize,NULL)==0) 
				//		{
				//			int piperc=0;
				//			if(SNAREDEBUG) 
				//			{
				//				DebugMsg("Warning: Could not write to GUI pipe");
				//			}
				//			
				//			CancelIo(hWritePipe);
				//			CancelIo(hPipe);

				//			DisconnectNamedPipe(hWritePipe);

				//			if (ConnectNamedPipe(hWritePipe,&OverlappedWrite) == 0) 
				//			{
				//				piperc = GetLastError();
				//			}
	
				//			if ((piperc != ERROR_IO_PENDING) && (piperc != ERROR_PIPE_CONNECTED)) 
				//			{
				//				// Not going to be able to do any pipe transactions.
				//				hWritePipe=INVALID_HANDLE_VALUE;
				//			}
				//			
				//		WritePipeConnected=FALSE;
				//		}
				//	}
				}



				// Ok, we have everything we need out of the current node.
				// Remove it from the linked list.

				// HMM... we receive the access violation here!! (so violation must be happening in one of the other threads!

				dwWaitRes = WaitForSingleObject(hMutex,INFINITE);
				if(dwWaitRes == WAIT_OBJECT_0) 
				{
					// Ok, we have a lock.
					currentnode=head->next;
					
					
					// HMmmm.. this 'free' is causing an access violation?
					free(head);
					head=currentnode;
					if(head == NULL) 
					{
						// Last element? Reset tail too.
						tail = NULL;
					}
					ReleaseMutex(hMutex);
				}
			} // end while(currentnode)
		}
    }

//quit:
	if(SNAREDEBUG) 
	{ 
		DebugMsg("SNARE Closing"); 
	}

	// Try and be nice about killing the linked list, by giving
	// other threads a few seconds to shutdown.
	dwWaitRes = WaitForSingleObject(hMutex,5000);

	// But even if we don't have the lock by now, try and kill our linked list anyway.
	while(head) 
	{
		currentnode=head->next;
		free(head);
		head=currentnode;
	}

	if(OutputFile) 
	{
		fclose(OutputFile);
	}
	

	//if(hPipe != INVALID_HANDLE_VALUE) {
	//	CloseHandle(hPipe);
	//}
	//if(hWritePipe != INVALID_HANDLE_VALUE) {
	//	CloseHandle(hWritePipe);
	//}
	
	TerminateWinsock();

	if( m_hEventList[0] ) ::CloseHandle(m_hEventList[0]);
	if( m_hEventList[1] ) ::CloseHandle(m_hEventList[1]);
	if( m_hEventList[2] ) ::CloseHandle(m_hEventList[2]);
}

//
VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT idEvent,DWORD dwTime)
{
	//
	if(bSet)
	{
		KillTimer(NULL, hTime);
		SetTimer(NULL, NULL, 24*60*60*1000, TimerProc);
		//MSG msg;
		//while (GetMessage(&msg, 0, 0, 0))
		//	DispatchMessage(&msg);
		bSet = false;
	}

	//��ȡ��ǰʱ�䲢��ȥ���������Եõ�ɾ����¼����ֹʱ��	
	//OutputDebugString("TimerProc\r\n");
	//OutputDebugString(strKeepDay.c_str());
	
	//ɾ����¼
	int nKeepDay = 0;
	sscanf(strKeepDay.c_str(), "%d", &nKeepDay);
	
	if(nKeepDay != 0)
	{
		TTime curTime = TTime::GetCurrentTimeEx();
		
		TTimeSpan ts(0, nKeepDay*24, 0, 0);
		curTime -= ts;

		//OutputDebugString("Delete\r\n");
		DeleteRecords("syslog", curTime);
	}
}

//
///////////////////////////////////////////////////////////////////
// InitWinsock
//              starts up winsock.dll or wsock32.dll
BOOL CSnareSvrService::InitWinsock( char *szError )
{
	WSAData wsData;

	if(!szError) return(FALSE);
	
	WORD wVersionRequested = WINSOCK_VERSION;
	
	if(WSAStartup(wVersionRequested, &wsData) != 0)
	{
		// :( error
		if( szError )
		{
			sprintf(szError,"WSAStartup failed: WSA ERROR: %d\r\n",
				WSAGetLastError());
			if(SNAREDEBUG) 
			{ 
				if(szError)
				{
					DebugMsg(szError); 
				}
			}
		}
		return FALSE;
	}
	
	// all is well
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// TerminateWinsock
//      call this function with the current socket or INVALID_SOCKET
void CSnareSvrService::TerminateWinsock()
{
	// cancel blocking calls, if any
	WSACancelBlockingCall();
		
	// unload winsock
	WSACleanup();
}


//
void CSnareSvrService::OnShutdown() 
{
	// Audit: Set that global variable to TRUE so that the threads
	// receive the terminate message
	g_Info.bTerminate=TRUE;
	
	if(SNAREDEBUG) 
	{ 
		DebugMsg("SNARE Shutdown request received"); 
	}

	// Call a fake event so that the subroutine	gets the shutdown message
	// through the setting of g_Info.bTerminate
	if(m_hEventList[0]) 
	{
		::SetEvent(m_hEventList[0]);
	}
}


// Process user control requests
BOOL CSnareSvrService::OnUserControl(DWORD dwOpcode)
{
    switch (dwOpcode) 
	{
    case SERVICE_CONTROL_USER + 0:

        // Save the current status in the registry
        SaveStatus();
        return TRUE;

    default:
        break;
    }
    return FALSE; // say not handled
}

// Save the current status in the registry
void CSnareSvrService::SaveStatus()
{
	// TODO: add code to save the sate of the service in the registry
}

//
int CSnareSvrService::DirExists(char * dir)
{
    WIN32_FIND_DATA data;
    HANDLE hFile = FindFirstFile(dir, &data);

	if(!dir) 
	{
		return(-1);
	}

    if (hFile == INVALID_HANDLE_VALUE) 
	{ // directory doesn't exist
        return FALSE;
    }
	else
	{
        // is it folder or file?
        FindClose(hFile);
        if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
		{
            return TRUE;
		}
        return -1;
    }
}


// Note: The way I've designed this, means that concurrent syslog AND snare packets
// will trample over each other for access to the mutex.
// recommend users EITHER use snare, OR syslog - not both.
// I could get around this by having separate snare & syslog linked lists,
// but that would be a pain.

//void SnareThread(HANDLE event)
//{
//	SOCKET snare_socket;
//	Node * newNode=NULL;
//	char * ipbuff=NULL;
//
//	SOCKADDR_IN snare_in;
//	SOCKADDR_IN recv_socket;
//	int recv_socketlen=sizeof(recv_socket);
//	
//	DWORD dwWaitResult;
//	int retval;
//
//	fd_set rfds;
//	struct timeval tv;
//
//
//	DWORD SnarePort=MyGetProfileDWORD("Config","SnarePort",6161);
//
//	// Set up our network ports.
//	snare_socket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
//
//	if(!snare_socket) {
//		printf("Cannot open sockets - Exiting.\n");
//		return;
//	}
//
//
//	snare_in.sin_family=AF_INET;
//	snare_in.sin_port = htons((u_short)SnarePort);
//	snare_in.sin_addr.s_addr=htonl(INADDR_ANY);
//
//	if(bind(snare_socket,(SOCKADDR *)&snare_in, sizeof(snare_in)) != 0) {
//		printf("Bind Failed - snare socket. Do you already have something running on the snare UDP port?\n");
//		return;
//	}
//
//	// While we aren't being requested to terminate.
//	while(WaitForSingleObject(event,0)!=WAIT_OBJECT_0) {
//		
//		// Select.. if no data, continue.
//		FD_ZERO(&rfds);
//		FD_SET(snare_socket,&rfds);
//		tv.tv_sec=1;
//		tv.tv_usec=0;
//
//		
//		retval=select(snare_socket+1,&rfds,NULL,NULL,&tv);
//
//		if(retval <= 0) {
//			// No data. keep spinning.
//			continue;
//		}
//		
//		// Request ownership of the mutex.
//		// Note: Only circumstance where this may fail, is if the syslog grabs it first.
//		dwWaitResult = WaitForSingleObject(hMutex,5000);
//		if(dwWaitResult != WAIT_OBJECT_0) {
//			// Keep spinning.
//			continue;
//		}
//
//		do {
//			// Ok, we have data. Create a new node.
//			newNode = (Node *) malloc(sizeof(Node));
//
//			retval = recvfrom(snare_socket,newNode->Line,sizeof(newNode->Line),0,
//						(struct sockaddr *)&recv_socket,&recv_socketlen);
//
//			if(retval > 0) {
//				newNode->Line[retval]='\0';
//			}
//
//
//			ipbuff=inet_ntoa(recv_socket.sin_addr);
//			if(ipbuff) {
//				strncpy(newNode->IPAddress,ipbuff,sizeof(newNode->IPAddress));
//			} else {
//				strncpy(newNode->IPAddress,"\0",sizeof(newNode->IPAddress));
//			}
//		
//			newNode->next=NULL;
//			newNode->LogType=0;	// Snare, not syslog.
//
//			// Add the new node to the end of the list.
//			// First, lock the mutex.
//		
//			// Add this to the list.
//			if(tail) {
//				tail->next=newNode;
//			}
//			tail=newNode;
//			if(!head) {
//				head=tail;
//			}
//			SnarePackets++;
//			if(SnarePackets == ULONG_MAX) {
//				SnarePackets=1;
//			}
//
//			FD_ZERO(&rfds);
//			FD_SET(snare_socket,&rfds);
//			tv.tv_sec=1;
//			tv.tv_usec=0;
//
//			retval=select(snare_socket+1,&rfds,NULL,NULL,&tv);
//		} while(retval); // Keep going until we have no more data.
//
//		// Release our lock.
//		if(!ReleaseMutex(hMutex)) {
//			// Failed??? You're kidding me.
//			// Better clear the events, since we're the only one with the lock, then die.
//			Node * tempnode;
//
//			currentnode = head;
//			while(currentnode) {
//				tempnode=currentnode->next;
//				free(currentnode);
//				currentnode=tempnode;
//			}
//			
//			closesocket(snare_socket);
//
//			::SetEvent(event);
//			return;
//		}
//	}
//	closesocket(snare_socket);
//}

void ReceiveThread(HANDLE event)
{
	DWORD dwWaitResult;
	while(WaitForSingleObject(event,0)!=WAIT_OBJECT_0) 
	{	
		::Sleep(50);

		MQRECORD mrd;

		mrd=::BlockPopMessage("SiteView70-SysLog");

		if(mrd!=INVALID_VALUE)
		{
			//puts("Pop message failed");

			string label;
			svutil::TTime ct;
			unsigned int len=0;

			if(!::GetMessageData(mrd, label, ct, NULL, len))
			{
				OutputDebugString("Get message data failed");
			}

			//printf("Data len is :%d\n",len);
			char * buf = NULL;
			buf = new char[len];

			if(!::GetMessageData(mrd, label, ct, buf, len))
			{
				OutputDebugString("Get message data failed");
			}
			
			::CloseMQRecord(mrd);
			
			OutputDebugString(label.c_str());
			OutputDebugString("\n");
			string szSmsTo = "";
			std::list<string> listIni;
			std::list<string>::iterator listIniItem;
			EnterCriticalSection(&csFilter);
			if(label == "FacilityChange")
			{

				mapFacility.clear();

				//FacilityChange
				OutputDebugString("SysLog FacilityChange\n");
				OutputDebugString(buf);


				szSmsTo += buf;				
				ParserToken(listIni, szSmsTo.c_str(), ",");
				for(listIniItem = listIni.begin(); listIniItem!=listIni.end(); listIniItem++)
				{
					mapFacility[(*listIniItem)] = 0;
				}
			}
			else if(label == "SvrChange")
			{
				mapSeverities.clear();

				//SvrChange
				OutputDebugString("SysLog SvrChange\n");
				OutputDebugString(buf);

				szSmsTo += buf;
				ParserToken(listIni, szSmsTo.c_str(), ",");
				for(listIniItem = listIni.begin(); listIniItem!=listIni.end(); listIniItem++)
				{
					mapSeverities[(*listIniItem)] = 0;
				}	
			}
			else if(label == "KeepDayChange")
			{
				//KeepDayChange
				OutputDebugString("KeepDayChange \n");
				OutputDebugString(buf);

				szSmsTo += buf;
				strKeepDay = szSmsTo;
			}
			else
			{
				
			}
			LeaveCriticalSection(&csFilter);
		}

		dwWaitResult = WaitForSingleObject(hMutex,5000);
		if(dwWaitResult != WAIT_OBJECT_0) 
		{			
			continue;
		}

		// Release our lock.
		if(!ReleaseMutex(hMutex)) 
		{			
			::SetEvent(event);
			return;
		}
	}
}

//
void WinMsgThread(HANDLE event)
{
	DWORD dwWaitResult;
	while(WaitForSingleObject(event,0)!=WAIT_OBJECT_0) 
	{	
		if(bSet)		
			::Sleep(dwSleep);
		else		
		{
			//KillTimer(NULL, hTime);
			//SetTimer(NULL, NULL, 24*60*60*1000, TimerProc);
			//MSG msg;
			::Sleep(24*60*60*1000);
			bSet = false;
		}

		//��ȡ��ǰʱ�䲢��ȥ���������Եõ�ɾ����¼����ֹʱ��	
		//OutputDebugString("TimerProc\r\n");
		//OutputDebugString(strKeepDay.c_str());
		
		//ɾ����¼
		int nKeepDay = 0;
		sscanf(strKeepDay.c_str(), "%d", &nKeepDay);
		
		if(nKeepDay != 0)
		{
			TTime curTime = TTime::GetCurrentTimeEx();
			
			TTimeSpan ts(0, nKeepDay*24, 0, 0);
			curTime -= ts;

			//OutputDebugString("Delete\r\n");
			DeleteRecords("syslog", curTime);
		}

		//MSG msg;
		//while(GetMessage(&msg, NULL, 0, 0))
		//{
		//	TranslateMessage(&msg);
		//	DispatchMessage(&msg);
		//	//switch(msg.message )
		//	//{
		//	//	case WM_QUIT:
		//	//		goto quit;
		//	//	default:
		//	//		break;
		//	//}
		//}

		dwWaitResult = WaitForSingleObject(hMutex,5000);
		if(dwWaitResult != WAIT_OBJECT_0) 
		{			
			continue;
		}
		
		// Release our lock.
		if(!ReleaseMutex(hMutex)) 
		{			
			::SetEvent(event);
			return;
		}
	}
}
//
void SyslogThread(HANDLE event)
{
	SOCKET syslog_socket;
	SOCKADDR_IN syslog_in;
	SOCKADDR_IN recv_socket;
	int recv_socketlen=sizeof(recv_socket);
	
	DWORD dwWaitResult;
	int retval;

	Node * newNode=NULL;
	char * ipbuff=NULL;

	fd_set rfds;
	struct timeval tv;

	// Set up our network ports.	
	syslog_socket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if(!syslog_socket) 
	{
		printf("Cannot open sockets - Exiting.\n");
		return;
	}

	syslog_in.sin_family=AF_INET;
	syslog_in.sin_port = htons((u_short)514);
	syslog_in.sin_addr.s_addr=htonl(INADDR_ANY);

	if(bind(syslog_socket,(SOCKADDR *)&syslog_in, sizeof(syslog_in)) != 0) 
	{
		printf("Bind Failed - syslog socket. Do you already have something running on the syslog UDP port?\n");
		return;
	}

	// While we aren't being requested to terminate.
	while(WaitForSingleObject(event,0)!=WAIT_OBJECT_0) 
	{
		
		// Select.. if no data, continue.
		FD_ZERO(&rfds);
		FD_SET(syslog_socket,&rfds);
		tv.tv_sec=1;
		tv.tv_usec=0;

		
		retval=select(syslog_socket+1,&rfds,NULL,NULL,&tv);

		if(retval <= 0) 
		{
			// No data. keep spinning.
			continue;
		}
		
		// Request ownership of the mutex.
		// Note: Only circumstance where this may fail, is if the syslog grabs it first.
		dwWaitResult = WaitForSingleObject(hMutex,5000);
		if(dwWaitResult != WAIT_OBJECT_0) 
		{
			// Keep spinning.
			continue;
		}

		do
		{
			// Ok, we have data. Create a new node.
			newNode = (Node *) malloc(sizeof(Node));

			retval = recvfrom(syslog_socket,newNode->Line,sizeof(newNode->Line),0,
						(struct sockaddr *)&recv_socket,&recv_socketlen);

			if(retval > 0) 
			{
				newNode->Line[retval]='\0';
			}


			ipbuff=inet_ntoa(recv_socket.sin_addr);
			if(ipbuff) 
			{
				strncpy(newNode->IPAddress,ipbuff,sizeof(newNode->IPAddress));
			}
			else
			{
				strncpy(newNode->IPAddress,"\0",sizeof(newNode->IPAddress));
			}
		
			newNode->next=NULL;
			newNode->LogType=1;	// syslog.

			// Add the new node to the end of the list.
			// First, lock the mutex.
		
			// Add this to the list.
			if(tail) 
			{
				tail->next=newNode;
			}
			tail=newNode;

			if(!head) 
			{
				head=tail;
			}
			SyslogPackets++;
			
			if(SyslogPackets == ULONG_MAX) 
			{
				SyslogPackets=1;
			}
			
			FD_ZERO(&rfds);
			FD_SET(syslog_socket,&rfds);
			tv.tv_sec=1;
			tv.tv_usec=0;

			retval=select(syslog_socket+1,&rfds,NULL,NULL,&tv);
		} while(retval);

		// Release our lock.
		if(!ReleaseMutex(hMutex)) 
		{
			// Failed??? You're kidding me.
			// Better clear the events, since we're the only one with the lock, then die.
			Node * tempnode;

			currentnode = head;
			while(currentnode) 
			{
				tempnode=currentnode->next;
				free(currentnode);
				currentnode=tempnode;
			}
			closesocket(syslog_socket);
				
			::SetEvent(event);
			return;
		}
	}
	closesocket(syslog_socket);
}