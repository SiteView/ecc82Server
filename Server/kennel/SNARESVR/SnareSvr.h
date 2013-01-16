// FILE: SnareSvr.h
#include <iostream>
#include <string>
#include <list>
#include <map>
using namespace std;

#include "NTService.h"
#include "support.h"
#include <ntsecapi.h>
#include <stdio.h>
#include <windows.h>
#define MAX_EVENT 8192

#define OUTPUT_TYPE_DATE 1
#define OUTPUT_TYPE_DATE_SVR 2
#define OUTPUT_TYPE_DATE_LOG 3
#define OUTPUT_TYPE_DATE_LOG_SVR 4

class CSnareSvrService : public CNTService
{
public:
	CSnareSvrService();
	virtual	BOOL OnInit();
    virtual	void Run();
    virtual	BOOL OnUserControl(DWORD dwOpcode);
	void	OnShutdown();
	BOOL	InitWinsock				(char *);
	void	TerminateWinsock		();
	int		DirExists				(char *);
    void	SaveStatus();
	
	void InitFilterParam();	
	//CCriticalSection CriticalSection;
	int nFicility;
	int nLevel;
// 	int SNAREDEBUG;

private:
	HANDLE m_hEventList[3]; // Three elements.
	//HANDLE hPipe;
	//HANDLE hWritePipe;		// Pipe to the Audit GUI
};


// thread structure
typedef struct
{
	SOCKET hSocket;
	BOOL bTerminate;
} ThreadStruct;

//void    SnareThread				(HANDLE event);
void    SyslogThread			(HANDLE event);
void    ReceiveThread			(HANDLE event);
void    WinMsgThread			(HANDLE event);