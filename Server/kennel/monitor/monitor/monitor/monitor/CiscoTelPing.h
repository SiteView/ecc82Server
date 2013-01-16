#define	MAX_READ_SIZE 100 * 1024

#include "stdafx.h"
int cisco_connect1(SOCKET hSocket, char *pwd, char *pwdprompt, char *prompt,LPGDATA pgd);
int cisco_command1(SOCKET hSocket, char *cmd, char *prompt,LPGDATA pgd);

BOOL CISCOTELNETPING_MONITOR(char *server, int port, char *pwd, char *enpwd, 
					  char *pwdprompt, char *prompt, char *enpwdprompt, 
					  char *enprompt, char *encmd, char *monitorid, 
					  char *proxy_server, int proxy_port, char *proxy_uid, 
					  char *proxy_pwd, char *custpath, char *szReturn);
