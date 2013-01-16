
#include "stdafx.h"

BOOL http_command(char *url, char *cmd, int timeout, char *uid, char *pwd, 
				  char *pserver, int pport, char *puid, char *ppwd, char *szReturn);

BOOL HttpMonitorValue(char *url, char *uid, char *pwd, char *pserver, int pport, 
				  char *puid, char *ppwd, char *inifile, int mtype, 
				  char *extraparam, char *custpath, char *szReturn);
BOOL HttpUnixPerfMonitorValue(char *url, char *uid, char *pwd, char *pserver, int pport, 
			char *puid, char *ppwd, char *inifile, char *custpath, char *szReturn);
