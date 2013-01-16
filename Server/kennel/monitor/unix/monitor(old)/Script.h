
#include "stdafx.h"

BOOL TELEXESCRIPT(char *server, int port, char *uid, char *pwd, 
				  char *proxy_server, int proxy_port, char *proxy_uid, char *proxy_pwd, 
				  char *logprompt, char *pwdprompt, char *prompts, 
				  char *inifile, char *script, char *params, char *match, 
				  char *custpath, char *szReturn);

CString TELGETSCRIPT(char *server, int port, char *uid, char *pwd, 
					 char *proxy_server, int proxy_port, char *proxy_uid, char *proxy_pwd, 
					 char *logprompt, char *pwdprompt, char *prompts, 
					 char *inifile, char *custpath);

BOOL SSHEXESCRIPT(char *server, int port, char *uid, char *pwd, 
				  char *inifile, char *script, char *params, char *match, 
				  char *custpath, char *szReturn);

CString SSHGETSCRIPT(char *server, int port, char *uid, char *pwd, 
					 char *inifile, char *custpath);

BOOL HTTPEXESCRIPT(char *url, char *uid, char *pwd, char *pserver, int pport, 
				   char *puid, char *ppwd, char *inifile, char *script, char *params, 
				   char *match, char *custpath, char *szReturn);

CString HTTPGETSCRIPT(char *url, char *uid, char *pwd, char *pserver, int pport, 
					  char *puid, char *ppwd, char *inifile, char *custpath);

BOOL RLOGINEXESCRIPT(char *server, int port, char *uid, char *pwd, 
					 char *pserver, int pport, char *puid, char *ppwd, 
				     char *pwdprompt, char *prompts, 
				     char *inifile, char *script, char *params, char *match, 
				     char *custpath, char *szReturn);

CString RLOGINGETSCRIPT(char *server, int port, char *uid, char *pwd, 
					    char *pserver, int pport, char *puid, char *ppwd, 
					    char *pwdprompt, char *prompts, 
					    char *inifile, char *custpath);

BOOL TELEXESCRIPT2(char *server, int port, char *uid, char *pwd, 
				  char *proxy_server, int proxy_port, char *proxy_uid, char *proxy_pwd, 
				  char *logprompt, char *pwdprompt, char *prompts, 
				  char *inifile, char *script, char *params, char *match, 
				  char *custpath, char *szReturn);