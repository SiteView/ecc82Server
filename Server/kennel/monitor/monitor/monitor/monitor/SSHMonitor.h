
#include "stdafx.h"

#define			PLINK_PATH		"plink.exe"
#define         SSH_PATH        "ssh.exe"
#define			CALL_TIMEOUT	60
#define			BUFFER_SIZE		8192

#define			DragonFlow_OK							0
#define			DragonFlow_Timeout						1
#define			DragonFlow_CreateOutputThread			2
#define			DragonFlow_CreateErrorOutputThread		3
#define			DragonFlow_CreateInputThread			4
#define			DragonFlow_WriteToStandardOutput		5
#define			DragonFlow_OpenConnection				6
#define			DragonFlow_OpenCommandFile				7
#define			DragonFlow_NotEnoughSpace				8
#define			DragonFlow_InitialiseWinSock			9
#define			DragonFlow_WinSockVersionIncompatible	10
#define			DragonFlow_FatalError					11
#define         DragonFlow_OtherError					12
#define         UnixServerConnectionLimit				13


bool SSHMonitorValue(char *server, int port, char *uid, char *pwd,  
				  int mtype, char *inifile, char *extraparam, char *custpath, char *privatekeyfile, char *szReturn);
bool ZHWSSHMonitorValue(char *server, int port, char *uid, char *pwd,  
				  int mtype, char *inifile, char *strMatchStr,char *strMatchALLStr, char *custpath, char *privatekeyfile,
				  char *szReturn);
BOOL SSHUnixPerfMonitorValue(char *server, int port, char *uid, char *pwd,  
				  char *inifile, char *custpath, char *privatekeyfile, char *szReturn);

int executeExeFile(CString strCmdLine, CString &strContent);

int ssh_command(char *server, int port, char *uid, char *pwd, char *cmd, char *privatekeyfile, CString &strResult);
