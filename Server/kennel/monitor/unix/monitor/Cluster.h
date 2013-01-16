
#include "stdafx.h"

BOOL TELCLUSTER_MONITOR(char *server, int port, char *uid, char *pwd, char *pserver, int pport, 
						char *puid, char *ppwd, char *lprompt, char *pprompt, char *prompts, 
						char *ostype, char *clusterlog, char *cpath, char *szReturn);
BOOL SSHCLUSTER_MONITOR(char *server, int port, char *uid, char *pwd, char *ostype, char *clusterlog, 
						char *cpath, char *szReturn);
BOOL HTTPCLUSTER_MONITOR(char *url, char *uid, char *pwd, char *pserver, int pport, char *puid, 
						char *ppwd, char *ostype, char *clusterlog, char *cpath, char *szReturn);
BOOL RLOGINCLUSTER_MONITOR(char *server, int port, char *uid, char *pwd, char *pserver, int pport, 
						char *puid, char *ppwd, char *pprompt, char *prompts, char *ostype, 
						char *clusterlog, char *cpath, char *szReturn);
CString GetStringFromLineNo(CString strBuffer, int nLine);
CString GetColumnFromLine(CString strBuffer, int nColumn);