
#include "stdafx.h"
//Delete 2005-04-13
//#define		BUF_SIZE			100 * 1024
#define		PR_NUM				10
#define		C_STA				"DF-CMD-STA"
#define		C_END				"DF-CMD-END"

#define		RLOGIN_CLIENT_PORT			1023
#define		RLOGIN_READ_TIMEOUT			180
#define		RLOGIN_CONNECT_TIMEOUT		60

#define		TERM_TYPE			"vt100"
#define		TERM_SPEED			"38400"

SOCKET rlogin_init(char *server, int port, char *pserver, int pport, char *puid, char *ppwd);
int rlogin_connect(SOCKET hSocket, char *uid, char *pwd, char *pwdprompt, char arprompt[PR_NUM][256]);
void rlogin_protocol(SOCKET server, unsigned char code);
int rlogin_command(SOCKET hSocket, char *cmd);

BOOL RloginMonitorValue(char *server, int port, char *uid, char *pwd, 
						char *pserver, int pport, char *puid, char *ppwd, 
						char *pwdprompt, char *prompts, 
						char *inifile, int mtype, char *extraparam, char *custpath, 
						char *szReturn);
BOOL RloginUnixPerfMonitorValue(char *server, int port, char *uid, char *pwd, 
						char *pserver, int pport, char *puid, char *ppwd, 
						char *pwdprompt, char *prompts, 
						char *inifile, char *custpath, char *szReturn);
