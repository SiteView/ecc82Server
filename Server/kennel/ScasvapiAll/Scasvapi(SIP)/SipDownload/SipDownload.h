#include "stdafx.h"
#include <time.h>
#include <atltime.h>
#include <winsvc.h>
#include <sys/stat.h>
#include "Update.h"

/****************************************************/
/*				全局变量部分						*/
/****************************************************/
const	char *g_ServiceDName = "SiteView_Sip_Download";
const	char *g_ServiceName	= "SiteView_Sip_Download";

int nUpdating_Num;
int nMax_Updating_Num;

char *g_account=NULL;
char *g_password=NULL;
string g_mcPName="";

PROCESS_INFORMATION	g_pi;
PROCESS_INFORMATION	g_alertpi;
PROCESS_INFORMATION	g_reportpi;

BOOL	g_Disabled=FALSE;
BOOL g_SubProcessState=TRUE;

bool UnInstall();
bool Install();

BOOL Run();

void	WINAPI SVS_ServiceStart(DWORD argc , LPTSTR * argv);
void	WINAPI SVS_ServiceCtrlHandler(DWORD opcode);
DWORD SVS_ServiceInitialization(DWORD argc, LPTSTR *argv, DWORD *specificError) ;

void WriteLog(const char * strMsg);
//BOOL WriteLog(CString strError);

void GetErrorMsg(LPSTR lpBuffer);

SERVICE_STATUS			SVS_ServiceStatus;
SERVICE_STATUS_HANDLE	SVS_ServiceStatusHandle;

#define RUNNING_TIMES	60*1000

void CALLBACK TimerProc(HWND   hwnd,           //   handle   of   window   for   timer   messages
						UINT   uMsg,           //   WM_TIMER   message
						UINT   idEvent,     //   timer   identifier
						DWORD   dwTime
						);