
#define	MAX_READ_SIZE 100 * 1024

#include "stdafx.h"

BOOL CISCOCONFIG_MONITOR(char *server, int port, char *pwd, char *enpwd, 
					  char *pwdprompt, char *prompt, char *enpwdprompt, 
					  char *enprompt, char *encmd, int groupid, int monitorid, 
					  char *proxy_server, int proxy_port, char *proxy_uid, 
					  char *proxy_pwd, char *custpath, char *szReturn);
int cisco_connect(SOCKET hSocket, char *pwd, char *pwdprompt, char *prompt,LPGDATA pgd);
int cisco_command(SOCKET hSocket, char *cmd, char *prompt,LPGDATA pgd);

BOOL CISCOCONFIG_MONITOR62(char *server, int port, char *pwd, char *enpwd, 
					  char *pwdprompt, char *prompt, char *enpwdprompt, 
					  char *enprompt, char *encmd, char *monitorid, 
					  char *proxy_server, int proxy_port, char *proxy_uid, 
					  char *proxy_pwd, char *custpath, char *szReturn);
